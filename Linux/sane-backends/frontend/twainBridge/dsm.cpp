#include "dsm.h"

DSMENTRY DSM_Entry(TW_IDENTITY *_pOrigin,
                   TW_IDENTITY *_pDest,
                   TW_UINT32 _DG,
                   TW_UINT16 _DAT,
                   TW_UINT16 _MSG,
                   TW_MEMREF _pData)
{
    TW_UINT16 rcDSM;

    // Validate...
    if (0 == _pOrigin)
    {
        return TWRC_FAILURE;
    }

    // If we're processing DG_CONTROL/DAT_PARENT/MSG_OPENDSM then see
    // if we need to create our CTwnDsm object.  We don't want to
    // allocate any resources prior to new CTwnDsm!!!
    if ((_MSG == MSG_OPENDSM) && (_DAT == DAT_PARENT) && (_DG == DG_CONTROL) && (0 == g_ptwndsm))
    {
        g_ptwndsm = new CTwnDsm;
        if (0 == g_ptwndsm)
        {
            writeToLog("Failed to new CTwnDsm!!!");
            return TWRC_FAILURE;
        }
    }

    // If we have no CTwnDsm object, then we're in trouble, but
    // try to handle DAT_STATUS in case it gets called before
    // MSG_OPENDSM or after MSG_CLOSEDSM...
    if (0 == g_ptwndsm)
    {
        if (((_MSG == MSG_GET) || (_MSG == MSG_CHECKSTATUS)) && (_DAT == DAT_STATUS) && (_DG == DG_CONTROL) && (0 != _pData))
        {
            ((TW_STATUS *)_pData)->ConditionCode = TWCC_BUMMER;
            return (TWRC_SUCCESS);
        }
        else
        {
            //kLOG((kLOGERR,"DAT_STATUS called before MSG_OPENDSM or after MSG_CLOSEDSM..."));
            return (TWRC_FAILURE);
        }
    }

    // Transfer control over to our dsm object, otherwise we'll
    // be doing g_ptwndsm all over the place...
    rcDSM = g_ptwndsm->DSM_Entry(_pOrigin, _pDest, _DG, _DAT, _MSG, _pData);

    // If we successfully processed DG_CONTROL/DAT_PARENT/MSG_CLOSEDSM,
    // and don't have any other applications with it open,
    // then destroy our object.  We don't want to have any resources
    // lingering around after we destroy our CTwnDsm object!!!
    if ((TWRC_SUCCESS == rcDSM) && (_MSG == MSG_CLOSEDSM) && (_DAT == DAT_PARENT) && (_DG == DG_CONTROL) && g_ptwndsm->DSMGetState() != dsmState_Open)
    {
        delete g_ptwndsm;
        g_ptwndsm = 0;
    }

    // All done...
    return rcDSM;
}

/*
* Our constructor...
* Clean out the pod and set stuff.  Get logging set up so we
* can have a clue what's going on...
*/
CTwnDsm::CTwnDsm()
{
    // Zero out the pod...
    memset(&pod, 0, sizeof(pod));

    // Get our logging object...

    // If logging is on, then this is a good chance to dump information
    // about ourselves...

    // Get our application object...
    pod.m_ptwndsmapps = new CTwnDsmApps();
    if (!pod.m_ptwndsmapps)
    {
        // kPANIC("Failed to new CTwnDsmApps!!!");
    }
}

CTwnDsm::~CTwnDsm()
{
    if (pod.m_ptwndsmapps)
    {
        delete pod.m_ptwndsmapps;
    }
    if (g_ptwndsmlog)
    {
        delete g_ptwndsmlog;
    }
    memset(&pod, 0, sizeof(pod));
}

TW_UINT16 CTwnDsm::DSM_Entry(TW_IDENTITY *_pOrigin, TW_IDENTITY *_pDest, TW_UINT32 _DG, TW_UINT16 _DAT, TW_UINT16 _MSG, TW_MEMREF _pData){

    TW_UINT16 rcDSM = TWRC_SUCCESS;
    bool bPrinted;
    TW_CALLBACK2 *ptwcallback2;
    TW_IDENTITY *pAppId = _pOrigin;
    TW_IDENTITY *pDSId = _pDest;

    // Do a test to see if pOrigin is a DS instead of App, if so then switch pAppId and pDSId
    // MSG_INVOKE_CALLBACK was only used on the Mac and is now deprecated (ver 2.1)
    // it is here for backwords capabiltiy
    if ((_DAT == DAT_NULL /*&& _DG == DG_CONTROL */) || (_DAT == DAT_CALLBACK && _MSG == MSG_INVOKE_CALLBACK /*&& _DG == DG_CONTROL */))
    {
        pAppId = _pDest;
        pDSId = _pOrigin;
    }

    // Print the triplets to stdout for information purposes
    bPrinted = printTripletsInfo(_pOrigin, _pDest, _DG, _DAT, _MSG, _pData);

    // Sniff for the application forwarding an event to the
    // DS. It may be possible that the app has a message waiting for
    // it because it didn't register a callback.
    if ((DAT_EVENT == _DAT) && (MSG_PROCESSEVENT == _MSG))
    {
        // Check that the AppID and DSID are valid...
        if (!pod.m_ptwndsmapps->AppValidateIds(pAppId, pDSId))
        {
            kLOG((kLOGINFO, "Bad TW_IDENTITY"));
            pod.m_ptwndsmapps->AppSetConditionCode(0, TWCC_BADPROTOCOL);
            rcDSM = TWRC_FAILURE;
        }
        else if (pod.m_ptwndsmapps->DsCallbackIsWaiting(pAppId, (TWID_T)pDSId->Id))
        {
            ptwcallback2 = pod.m_ptwndsmapps->DsCallback2Get(pAppId, (TWID_T)pDSId->Id);
            ((TW_EVENT *)(_pData))->TWMessage = ptwcallback2->Message;
            if (g_ptwndsmlog)
            {
                char szMsg[64];
                StringFromMsg(szMsg, NCHARS(szMsg), ptwcallback2->Message);
                kLOG((kLOGINFO, "%.32s retrieving DAT_EVENT / %s\n", pAppId->ProductName, szMsg));
            }
            ptwcallback2->Message = 0;
            pod.m_ptwndsmapps->DsCallbackSetWaiting(pAppId, (TWID_T)pDSId->Id, FALSE);
            rcDSM = TWRC_DSEVENT;
        }
        // No callback, so fall on through...
    }

    // Is this msg for us?
    if (TWRC_SUCCESS == rcDSM)
    {
        switch (_DAT)
        {
        case DAT_IDENTITY:
            // If the pDSId is 0 then the message is intended for us.  We're
            // going to force the matter if _MSG is MSG_CLOSEDS, otherwise
            // we send the MSG_CLOSEDS to the driver, but never process it
            // ourselves, which seems like a terrible idea...
            if ((pDSId == 0) || (_MSG == MSG_CLOSEDS))
            {
                rcDSM = DSM_Identity(pAppId, _MSG, (TW_IDENTITY *)_pData);
                break;
            }
            // else we fall thru to send the message onto the DS

        default:
            // check if the application is open or not.  If it isn't, we have a bad sequence
            if (dsmState_Open == pod.m_ptwndsmapps->AppGetState(pAppId))
            {
                // Check that the AppID and DSID are valid...
                if (!pod.m_ptwndsmapps->AppValidateIds(pAppId, pDSId))
                {
                    kLOG((kLOGINFO, "Bad TW_IDENTITY"));
                    pod.m_ptwndsmapps->AppSetConditionCode(0, TWCC_BADPROTOCOL);
                    rcDSM = TWRC_FAILURE;
                }

                // Issue the command...
                else if (0 != pod.m_ptwndsmapps->DsGetEntryProc(pAppId, (TWID_T)pDSId->Id))
                {
                    // Don't send a new message if the DS is still processing a previous message
                    // or if the application has not returned back from recieving callback.
                    // Place a Try | Catch around the function so we can maintain correct state
                    // in the case of an exception
                    //
                    // We are only enforcing this new behavior for TWAIN 2.2 applications and
                    // and higher.  Older apps can still use the 'wrong' behavior.  We need this
                    // to preserve backwards compability, and to give ourselves a chance to
                    // inform developers of the new requirement...
                    //
                    if (((((pAppId->ProtocolMajor * 10) + (pAppId->ProtocolMinor)) <= 201) ||
                         !pod.m_ptwndsmapps->DsIsProcessingMessage(pAppId, (TWID_T)pDSId->Id)) &&
                        ((((pAppId->ProtocolMajor * 10) + (pAppId->ProtocolMinor)) <= 202) ||
                         !pod.m_ptwndsmapps->DsIsAppProcessingCallback(pAppId, (TWID_T)pDSId->Id)))
                    {
                        pod.m_ptwndsmapps->DsSetProcessingMessage(pAppId, (TWID_T)pDSId->Id, TRUE);
                        try
                        {
                            // Create a local copy of the AppIdentity
                            TW_IDENTITY AppId = *pod.m_ptwndsmapps->AppGetIdentity(pAppId);

                            rcDSM = (pod.m_ptwndsmapps->DsGetEntryProc(&AppId, (TWID_T)pDSId->Id))(
                                &AppId,
                                _DG,
                                _DAT,
                                _MSG,
                                _pData);
                        }
                        catch (...)
                        {
                            rcDSM = TWRC_FAILURE;
                            pod.m_ptwndsmapps->AppSetConditionCode(pAppId, TWCC_BUMMER);
                            kLOG((kLOGERR, "Exception caught while DS was processing message.  Returning Failure."));
                        }
                        pod.m_ptwndsmapps->DsSetProcessingMessage(pAppId, (TWID_T)pDSId->Id, FALSE);
                    }
                    else
                    {
                        if (_DAT == DAT_EVENT && _MSG == MSG_PROCESSEVENT)
                        {
                            kLOG((kLOGINFO, "Nested DAT_EVENT / MSG_PROCESSEVENT Ignored"));
                            rcDSM = TWRC_NOTDSEVENT;
                            ((TW_EVENT *)(_pData))->TWMessage = MSG_NULL;
                        }
                        else
                        {
                            kLOG((kLOGERR, "Nested calls back to the DS.  Returning Failure."));
                            pod.m_ptwndsmapps->AppSetConditionCode(pAppId, TWCC_SEQERROR);
                            rcDSM = TWRC_FAILURE;
                        }
                    }
                }

                // For some reason we have no pointer to the dsentry function...
                else
                {
                    kLOG((kLOGERR, "Unable to find driver, check your AppId and DsId values..."));
                    pod.m_ptwndsmapps->AppSetConditionCode(pAppId, TWCC_OPERATIONERROR);
                    kLOG((kLOGERR, "DS_Entry is null...%ld", (TWID_T)pAppId->Id));
                    rcDSM = TWRC_FAILURE;
                }
            }
            else
            {
                kLOG((kLOGINFO, "DS is not open"));
                pod.m_ptwndsmapps->AppSetConditionCode(pAppId, TWCC_SEQERROR);
                rcDSM = TWRC_FAILURE;
            }
            break;

        case DAT_PARENT:
            rcDSM = DSM_Parent(pAppId, _MSG, _pData);
            break;

        case DAT_TWUNKIDENTITY:
            rcDSM = DSM_TwunkIdentity(pAppId, _MSG, (TW_TWUNKIDENTITY *)_pData);
            break;

        case DAT_ENTRYPOINT:
            rcDSM = DSM_Entrypoint(pAppId, _MSG, (TW_ENTRYPOINT *)_pData);
            break;

        case DAT_STATUS:
            if (_MSG == MSG_CHECKSTATUS)
            {
                _MSG = MSG_GET;
                kLOG((kLOGINFO, "MSG_CHECKSTATUS is Depreciated using MSG_GET"));
            }

            // If we get a DSId then it is intended to be passed along to the driver.
            // If the DSId is null then the request is handled by the DSM
            // If we're talking to a driver (state 4 or higher), then we
            // will pass the DAT_STATUS request down to it...
            if (0 != pDSId && (dsmState_Open == pod.m_ptwndsmapps->AppGetState(pAppId)) && pod.m_ptwndsmapps->AppValidateIds(pAppId, pDSId) && (0 != pod.m_ptwndsmapps->DsGetEntryProc(pAppId, (TWID_T)pDSId->Id)))
            {
                // Create a local copy of the AppIdentity
                TW_IDENTITY AppId = *pod.m_ptwndsmapps->AppGetIdentity(pAppId);

                rcDSM = (pod.m_ptwndsmapps->DsGetEntryProc(&AppId, (TWID_T)pDSId->Id))(
                    &AppId,
                    _DG,
                    _DAT,
                    _MSG,
                    _pData);
            }
            // Otherwise, handle it ourself...
            else
            {
                rcDSM = DSM_Status(pAppId, _MSG, (TW_STATUS *)_pData);
            }
            break;

        case DAT_CALLBACK:
            // DAT_CALLBACK can be either from an Application registering its Callback,
            // or from a DS Invoking a request to send a message to the Application
            rcDSM = DSM_Callback(_pOrigin, _pDest, _MSG, (TW_CALLBACK *)_pData);
            break;

        case DAT_CALLBACK2:
            // DAT_CALLBACK2 can be either from an Application registering its Callback,
            // or from a DS Invoking a request to send a message to the Application
            rcDSM = DSM_Callback2(_pOrigin, _pDest, _MSG, (TW_CALLBACK2 *)_pData);
            break;

        case DAT_NULL:
            // Note how the origin and destination are switched for this
            // call (and only this call).  Because, of course, this
            // message is being send from the driver to the application...
            rcDSM = DSM_Null(pAppId, pDSId, _MSG);
            break;
        }
    }

    // Log how it went...
    if (bPrinted)
    {
        printResults(_DG, _DAT, _MSG, _pData, rcDSM);
    }

    return rcDSM;
}

TW_INT16 CTwnDsm::DSM_Parent(TW_IDENTITY  *_pAppId,
                             TW_UINT16     _MSG,
                             TW_MEMREF     _MemRef)
{
  TW_UINT16 result;

  // Validate...
  if (0 == _pAppId)
  {
      //kLOG((kLOGERR,"_pAppId is null"));
      pod.m_ptwndsmapps->AppSetConditionCode(_pAppId,TWCC_BADVALUE);
      return TWRC_FAILURE;
  }

  // Init stuff...
  result = TWRC_SUCCESS;

  // Process the message...
  switch (_MSG)
  {
    case MSG_OPENDSM:
      // Try to add the proposed item...
      result = pod.m_ptwndsmapps->AddApp(_pAppId,_MemRef);
      break;

    case MSG_CLOSEDSM:
      // Try to remove the proposed item...
      result = pod.m_ptwndsmapps->RemoveApp(_pAppId);
      break;

    default:
      result = TWRC_FAILURE;
      pod.m_ptwndsmapps->AppSetConditionCode(_pAppId,TWCC_BADPROTOCOL);
      break;
  }

  return result;
}