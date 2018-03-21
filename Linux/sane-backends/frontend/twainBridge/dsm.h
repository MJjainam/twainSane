#include "log.h"
#include <string.h>
#define MAX_NUM_DS 50
#if defined(__GNUC__)
#define TWNDSM_CMP TWNDSM_CMP_GNUGPP
#define TWNDSM_CMP_VERSION __GNUC__
#if defined(__APPLE__)
#define TWNDSM_OS TWNDSM_OS_MACOSX
#else
#define TWNDSM_OS TWNDSM_OS_LINUX
#endif
#if defined(__x86_64__) || defined(__LP64__)
#define TWNDSM_OS_64BIT 1
#else
#define TWNDSM_OS_32BIT 1
#endif
#endif

#if (TWNDSM_CMP == TWNDSM_CMP_GNUGPP)
#define DllExport
#define NCHARS(s) sizeof(s)/sizeof(s[0])
#define PATH_SEPERATOR '/'
#if (TWNDSM_OS == TWNDSM_OS_MACOSX)
#define LOADLIBRARY(lib,hook,DSID) \
  CFBundleCreate(0, CFURLCreateWithFileSystemPath(0, CFStringCreateWithCStringNoCopy(0, _pPath, kCFStringEncodingUTF8, 0), kCFURLPOSIXPathStyle, TRUE))
#define UNLOADLIBRARY(lib,unhook,DSID) 0; CFRelease((CFBundleRef)(lib))
#else
#define LOADLIBRARY(lib,hook,DSID) dlopen(lib, RTLD_LAZY)
#define UNLOADLIBRARY(lib,unhook,DSID) dlclose(lib)
#endif
#define LOADFUNCTION(lib, func) dlsym(lib, func)
#define READ read
#define CLOSE close
#define SNPRINTF snprintf
#define UNLINK unlink
#define STRNICMP strncasecmp
#define GETTHREADID gettid
#define FOPEN(pf,name,mode) pf = fopen(name,mode)
#ifndef kTWAIN_DS_DIR
  #if (TWNDSM_OS == TWNDSM_OS_MACOSX)
    #define kTWAIN_DS_DIR "/Library/Image Capture/TWAIN Data Sources"
  #else
    #define kTWAIN_DS_DIR "/usr/local/lib/twain"
  #endif
#endif
typedef unsigned int UINT;
typedef void* HINSTANCE;
typedef void* HWND;
#define DSMENTRY FAR PASCAL TW_UINT16

#if (TWNDSM_OS == TWNDSM_OS_MACOSX)
  #if TWNDSM_OS_64BIT
    #define TWID_T unsigned long long
    #define TWIDDEST_T TW_MEMREF
  #else
    #define TWID_T unsigned long
    #define TWIDDEST_T TW_MEMREF
  #endif
#else
  #define TWID_T TW_UINT32
  #define TWIDDEST_T TW_UINT32
#endif

#include <twain.h>

#if !defined(TRUE)
  #define FALSE   0
  #define TRUE    1
#endif

#endif

#define FILENAME_MAX 260

typedef enum
{
  dsmState_PreSession = 1, /**< Source Manager not loaded. */
  dsmState_Loaded     = 2, /**< Source Manager is loaded, but not open. */
  dsmState_Open       = 3  /**< Source Manager is open. */
} DSM_State;

class CTwnDsm
{
    //
    // All of our public functions go here...
    //
  public:
    /**
        * Our CTwnDsm constructor...
        */
    CTwnDsm();

    /**
        * Our CTwnDsm destructor...
        */
    ~CTwnDsm();

    /**
        * The guts of the DSM_Entry, the resource management portion
        * resides in a our DSM_Entry entry point, which isn't a part
        * of this class.  Hopefully it's not confusing that they have
        * the same name...
        * @param[in] _pOrigin Origin of message in this case a DS
        * @param[in] _pDest destination of message in this case an App
        * @param[in] _DG message id: DG_xxxx
        * @param[in] _DAT message id: DAT_xxxx
        * @param[in] _MSG message id: MSG_xxxx
        * @param[in] _pData the Data
        * @return a valid TWRC_xxxx return code
        */
    TW_UINT16 DSM_Entry(TW_IDENTITY *_pOrigin,
                        TW_IDENTITY *_pDest,
                        TW_UINT32 _DG,
                        TW_UINT16 _DAT,
                        TW_UINT16 _MSG,
                        TW_MEMREF _pData);



    /**
        * Get the state of the DSM by checking the state of all applications
        * @return DSM_State, Open if at least one application has DSM open
        */
    DSM_State DSMGetState();

    //
    // All of our private functions go here...
    //
  private:
    /**
        * Handles DAT_NULL calls from DS for Application.
        * @param[in] _pAppId Origin of message
        * @param[in] _pDsId TW_IDENTITY structure
        * @param[in] _MSG message id: MSG_xxxx
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_Null(TW_IDENTITY *_pAppId,
                      TW_IDENTITY *_pDsId,
                      TW_UINT16 _MSG);

    /**
        * Returns the current DSM status. Resets pod.m_ConditionCode to
        * TWCC_SUCCESS per the specification.
        * @param[in] _pAppId Orgin of message
        * @param[in] _MSG message id: MSG_xxxx
        * @param[out] _pStatus TW_STATUS structure
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_Status(TW_IDENTITY *_pAppId,
                        TW_UINT16 _MSG,
                        TW_STATUS *_pStatus);

    /**
        * Initializes or closes the DSM
        * @param[in] _pAppId Orgin of message
        * @param[in] _MSG message id: MSG_xxxx
        * @param[in] _MemRef for Windows during MSG_OPENDSM it is HWND, null otherwise
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_Parent(TW_IDENTITY *_pAppId,
                        TW_UINT16 _MSG,
                        TW_MEMREF _MemRef);

    /**
        * Source operations
        * @param[in] _pAppId Origin of message
        * @param[in] _MSG message id: MSG_xxxx
        * @param[in] _pDsId TW_IDENTITY structure
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_Identity(TW_IDENTITY *_pAppId,
                          TW_UINT16 _MSG,
                          TW_IDENTITY *_pDsId);

    /**
        * This routine will return the path to a DS.  
        * This is here for backwards compatibility. DAT_TWUNKIDENTITY is 
        * undocumented.  It was used by the Twunking layer.  Some old 
        * applications use it to get the path to the DS.  We need to 
        * continue to support it.
        * @param[in] _pAppId Origin of message
        * @param[in] _MSG message id: MSG_GET
        * @param[in,out] _pTwunkId TW_TWUNKIDENTITY structure with a valid TW_IDENTITY, returns path
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_TwunkIdentity(TW_IDENTITY *_pAppId,
                               TW_UINT16 _MSG,
                               TW_TWUNKIDENTITY *_pTwunkId);

    /**
        * Gets entry points
        * @param[in] _pAppId Origin of message
        * @param[in] _MSG message id: MSG_xxxx
        * @param[out] _pEntrypoint TW_IDENTITY structure
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_Entrypoint(TW_IDENTITY *_pAppId,
                            TW_UINT16 _MSG,
                            TW_ENTRYPOINT *_pEntrypoint);

    /**
        * Register application's callback.
        * @param[in] _pAppId Origin of message
        * @param[in] _pDsId TW_IDENTITY structure
        * @param[in] _MSG message id: MSG_xxxx valid = MSG_REGISTER_CALLBACK
        * @param[in] _pData pointer to a callback struct
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_Callback(TW_IDENTITY *_pAppId,
                          TW_IDENTITY *_pDsId,
                          TW_UINT16 _MSG,
                          TW_CALLBACK *_pData);

    /**
        * Register application's callback.
        * @param[in] _pAppId Origin of message
        * @param[in] _pDsId TW_IDENTITY structure
        * @param[in] _MSG message id: MSG_xxxx valid = MSG_REGISTER_CALLBACK
        * @param[in] _pData pointer to a callback2 struct
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_Callback2(TW_IDENTITY *_pAppId,
                           TW_IDENTITY *_pDsId,
                           TW_UINT16 _MSG,
                           TW_CALLBACK2 *_pData);

    /**
        * Opens the Data Source specified by pDSIdentity.  
        * pDSIdentity must be valid, but if a null name and id
        * is 0 then open default.
        * @param[in] _pAppId Origin of message
        * @param[in] _pDsId TW_IDENTITY structure
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 OpenDS(TW_IDENTITY *_pAppId,
                    TW_IDENTITY *_pDsId);

    /**
        * Closes the Data Source specified by pDSIdentity.
        * @param[in] _pAppId Origin of message
        * @param[in] _pDsId TW_IDENTITY structure
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 CloseDS(TW_IDENTITY *_pAppId,
                     TW_IDENTITY *_pDsId);

    /**
        * Displays the source select dialog and sets the default source.
        * @param[in] _pAppId Origin of message
        * @param[in,out] _pDsId TW_IDENTITY structure
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_SelectDS(TW_IDENTITY *_pAppId,
                          TW_IDENTITY *_pDsId);

    /**
        * Set the default source.
        * @param[in] _pAppId Origin of message
        * @param[in] _pDsId TW_IDENTITY structure
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_SetDefaultDS(TW_IDENTITY *_pAppId,
                              TW_IDENTITY *_pDsId);

    /**
        * Goes through the applications supported data sources looking for one that has
        * the exact same name as product name in the passed in identity. Will update the
        * _pDsId structure to match the name.
        * @param[in] _pAppId Origin of message
        * @param[in,out] _pDsId TW_IDENTITY structure
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 GetDSFromProductName(TW_IDENTITY *_pAppId,
                                  TW_IDENTITY *_pDsId);

    /**
        * Copies the applications first available source into _pDsId.
        * @param[in] _pAppId The origin identity structure
        * @param[out] _pDsId the identity structure to copy data into
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_GetFirst(TW_IDENTITY *_pAppId,
                          TW_IDENTITY *_pDsId);

    /**
        * Copies the applications next available source into _pDsId. A call to
        * DSM_GetFirst must have been made at least once before calling this function.
        * @param[in] _pAppId The origin identity structure
        * @param[out] _pDsId the identity structure to copy data into
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 DSM_GetNext(TW_IDENTITY *_pAppId,
                         TW_IDENTITY *_pDsId);

    /**
        * This routine will check if the current default source matches the
        * applications supported groups.  If it does it will copy it into the default
        * Source's identity (_pDsId), otherwise this routine will search for a source that
        * does match the app's supported groups and copy it into _pDsId.
        * @param[in] _pAppId The application identity
        * @param[in,out] _pDsId A pointer reference that will be set to point to the default identity.
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 GetMatchingDefault(TW_IDENTITY *_pAppId,
                                TW_IDENTITY *_pDsId);

    /**
        * Return back the tw_identity of the current source.  In state 3
        * this will be the default source.  In state 4 this will be the
        * currently opened source.
        * @param[in] _pAppId The application identity
        * @param[in,out] _pDsId A pointer reference that will be set to point to the current identity.
        * @return a valid TWRC_xxxx return code
        */
    TW_INT16 GetIdentity(TW_IDENTITY *_pAppId,
                         TW_IDENTITY *_pDsId);

    /**
        * prints to stdout information about the triplets.
        * @param[in] _pOrigin the Orgin to print the Product Name
        * @param[in] _pDest the Destination to print the Product Name
        * @param[in] _DG the Data Group
        * @param[in] _DAT the Data Argument Type
        * @param[in] _MSG the Message
        * @param[in] _pData the Data
        * @return return true if actually printed triplet
        */
    bool printTripletsInfo(const TW_IDENTITY *_pOrigin,
                           const TW_IDENTITY *_pDest,
                           const TW_UINT32 _DG,
                           const TW_UINT16 _DAT,
                           const TW_UINT16 _MSG,
                           const TW_MEMREF _pData);

    /**
        * prints to stdout information about result of processing the triplets.
        * @param[in] _DG the Data Group
        * @param[in] _DAT the Data Argument Type
        * @param[in] _MSG the Message
        * @param[in] _pData the Data
        * @param[in] _RC the Return Code after 
        */
    void printResults(const TW_UINT32 _DG,
                      const TW_UINT16 _DAT,
                      const TW_UINT16 _MSG,
                      const TW_MEMREF _pData,
                      const TW_UINT16 _RC);

    /**
        * Translates the _MSG passed in into a string and returns it
        * @param[out] _szMsg string to copy into
        * @param[in] _nChars max chars in _szMsg
        * @param[in] _MSG the TWAIN message to translate
        */
    void StringFromMsg(char *_szMsg,
                       const int _nChars,
                       const TW_UINT16 _MSG);

    /**
        * Translates the _DAT passed in into a string and returns it
        * @param[out] _szDat string to copy into
        * @param[in] _nChars max chars in _szDat
        * @param[in] _DAT the TWAIN data argument type to translate
        */
    void StringFromDat(char *_szDat,
                       const int _nChars,
                       const TW_UINT16 _DAT);

    /**
        * Translates the _DG passed in into a string and returns it
        * @param[out] _szDg string to copy into
        * @param[in] _nChars max chars in _szDg
        * @param[in] _DG the TWAIN data group to translate
        */
    void StringFromDg(char *_szDg,
                      const int _nChars,
                      const TW_UINT32 _DG);

    /**
        * Translates the _Cap passed in into a string and returns it
        * @param[out] _szCap string to copy into
        * @param[in] _nChars max chars in _szCap
        * @param[in] _Cap the TWAIN Capability to translate
        */
    void StringFromCap(char *_szCap,
                       const int _nChars,
                       const TW_UINT16 _Cap);

    /**
        * Translates the _ConType and _hContainer passed in into a string and returns it
        * @param[out] _szConType string to copy into
        * @param[in] _nChars max chars in _szCap
        * @param[in] _ConType the TWAIN Container Type to translate
        */
    void StringFromConType(char *_szConType,
                           const int _nChars,
                           const TW_UINT16 _ConType);

    /**
        * Translates the rc passed in into a string and returns it
        * @param[out] _szRc string to copy into
        * @param[in] _nChars max chars in szRc
        * @param[in] _rc the TWAIN Return Code to translate
        */
    void StringFromRC(char *_szRc,
                      const int _nChars,
                      const TW_UINT16 _rc);

    /**
        * Translates the Condition Code passed in into a string and returns it
        * @param[out] _szCondCode string to copy into
        * @param[in] _nChars max chars in szRc
        * @param[in] _cc the TWAIN Condition Code to translate
        */
    void StringFromConditionCode(char *_szCondCode,
                                 const int _nChars,
                                 const TW_UINT16 _cc);

    //
    // All of our attributes should be private.  Encapsulation
    // is a good thing...  :)
    //
  private:
    /*
        **  If you add a class in future, declare it here and not
        **  in the pod, or the memset we do on pod will ruin your
        **  day...
        */

    /**
        *  We use a pod system because it help prevents us from
        *  making dumb initialization mistakes.
        */
    struct _pod
    {
        /**
            * The class takes care of our list of applications and drivers.
            */
        CTwnDsmApps *m_ptwndsmapps;

        /**
            * The path to the default DS.  The Default DS is identified when
            * the DSM is opened.  A new Default is saved if SelectDlg is used.
            * So this value will be compared against DsGetPath()...
            */
        char m_DefaultDSPath[FILENAME_MAX];

        /**
            * The next id to test for GetFirst/GetNext...
            */
        TWID_T m_nextDsId;

        /**
            * The DS ID we end up with from SelectDlgProc.  This is only
            * used on the Windows platform.
            */
        TW_IDENTITY *m_pSelectDlgDsId;

        /**
            * The Application ID we're using inside of SelectDlgProc. This
            * is only used on the Windows platform.
            */
        TW_IDENTITY *m_pSelectDlgAppId;
    } pod; /**< Pieces of Data for the DSM class*/
};

class CTwnDsmApps
{
  public:

    /**
    * The CTwnDsmApps constructor.
    */
    CTwnDsmApps();

    /**
    * The CTwnDsmApps destructor.
    */
    ~CTwnDsmApps();

    /**
    * Add an application.
    * This supports MSG_OPENDSM.
    * @param[out] _pAppId Origin of message
    * @param[in] _MemRef the HWND on Window, null otherwise
    * @return a valid TWRC_xxxx return code
    */
    TW_UINT16 AddApp(TW_IDENTITY *_pAppId,
                     TW_MEMREF _MemRef);

    /**
    * Remove an application.
    * This supports MSG_CLOSEDSM.
    * @param[in] _pAppId Origin of message
    * @return a valid TWRC_xxxx return code
    */
    TW_UINT16 RemoveApp(TW_IDENTITY *_pAppId);

    /**
    * Loads a DS from disk and adds it to a global list of DS's.
    * @param[in] _pAppId Origin of message
    * @param[in] _DsId the source index of the library to open
    * @return a valid TWRC_xxxx return code
    */
    TW_INT16 LoadDS(TW_IDENTITY *_pAppId,
                    TWID_T      _DsId);

    /**
    * Unloads a DS and frees all its resources...
    * @param[in] _pAppId Origin of message
    * @param[in] _DsId the source index
    */
    void UnloadDS(TW_IDENTITY *_pAppId,
                  TWID_T      _DsId);

    /**
    * Validate that an id is in range...
    * @param[in] _pAppId id of App to test
    * @return TRUE if valid, else FALSE
    */
    TW_BOOL AppValidateId(TW_IDENTITY *_pAppId);

    /**
    * Validate that the App ID and DS ID are in range...
    * @param[in] _pAppId id of App to test
    * @param[in] _pDSId id of DS to test
    * @return TRUE if valid, else FALSE
    */
    TW_BOOL AppValidateIds(TW_IDENTITY *_pAppId, TW_IDENTITY *_pDSId);

    /**
    * Return a pointer to the application's identity.
    * Yeah, I know, this sorta violates encapsulation, but we do not
    * want to get silly about this...
    * @param[in] _pAppId id of identity to get
    * @return pointer to identity or NULL
    */
    TW_IDENTITY *AppGetIdentity(TW_IDENTITY *_pAppId);

    /**
    * Get the condition code, then reset it internally to TWCC_SUCCESS,
    * so you can only get it once, per the specification...
    * @param[in] _pAppId id of app, or NULL if we have no apps
    * @return TWCC_ value
    */
    TW_UINT16 AppGetConditionCode(TW_IDENTITY *_pAppId);

    /**
    * Set the condition code
    * @param[in] _pAppId id of app, or NULL if we have no apps
    * @param[in] _conditioncode the code to use
    */
    void AppSetConditionCode(TW_IDENTITY *_pAppId,
                             TW_UINT16 _conditioncode);

    /**
    * Get the state of the DSM for all applications
    * @return DSM_State, Open if at least one application has DSM open
    */
    DSM_State AppGetState();

    /**
    * Get the state of the DSM for the specified application
    * @param[in] _pAppId id of app
    * @return DSM_State of the application
    */
    DSM_State AppGetState(TW_IDENTITY *_pAppId);

    /**
    * Get the hwnd sent in with the call to MSG_OPENDSM
    * @param[in] _pAppId id of app
    * @return hwnd for the application that is calling us
    */
    void *AppHwnd(TW_IDENTITY *_pAppId);

    /**
    * Get the number of drivers we found as the result of a
    * successful call to LoadDS with _boolKeepOpen set to
    * false (meaning that we were just browsing)...
    * @param[in] _pAppId id of app
    * @return DSM_State of the application
    */
    TWID_T AppGetNumDs(TW_IDENTITY *_pAppId);

    /**
    * Poke the application to wake it up when sending a
    * DAT_NULL message to it...
    * @param[in] _pAppId id of app
    */
    void AppWakeup(TW_IDENTITY *_pAppId);

    /**
    * Get a pointer to the identity of the specified driver...
    * @param[in] _pAppId id of app
    * @param[in] _DsId numeric id of driver
    * @return pointer to drivers identity or NULL
    */
    TW_IDENTITY *DsGetIdentity(TW_IDENTITY *_pAppId,
                               TWID_T       _DsId);

    /**
    * Get a pointer to the DS_Entry function of the specified driver...
    * @param[in] _pAppId id of app
    * @param[in] _DsId numeric id of driver
    * @return pointer to DS_Entry for this driver or NULL
    */
    DSENTRYPROC  DsGetEntryProc(TW_IDENTITY *_pAppId,
                                TWID_T       _DsId);

    /**
    * Get a pointer to the driver file path and name, which is guaranteed to
    * be unique, even if the ProductName's aren't for some horrible
    * reason...
    * @param[in] _pAppId id of app
    * @param[in] _DsId numeric id of driver
    * @return pointer to file path and name for this driver or NULL
    */
    char *DsGetPath(TW_IDENTITY *_pAppId,
                    TWID_T       _DsId);

    /**
    * Get a pointer to TW_CALLBACK structure for the specified driver...
    * reason...
    * @param[in] _pAppId id of app
    * @param[in] _DsId numeric id of driver
    * @return pointer to the callback structure for this driver or NULL
    */
    TW_CALLBACK2 *DsCallback2Get(TW_IDENTITY *_pAppId,
                                 TWID_T       _DsId);

    /**
    * Test if the driver has a callback pending for attention...
    * @param[in] _pAppId id of app
    * @param[in] _DsId numeric id of driver
    * @return TRUE if the driver needs its callback called
    */
    TW_BOOL DsCallbackIsWaiting(TW_IDENTITY *_pAppId,
                                TWID_T       _DsId);

    /**
    * Set the callback flag for the driver to TRUE if the callback
    * needs to have its callback called, and set it to FALSE after
    * the call has been made...
    * @param[in] _pAppId id of app
    * @param[in] _DsId numeric id of driver
    * @param[in] _Waiting the new state for the waiting flag
    */
    void DsCallbackSetWaiting(TW_IDENTITY *_pAppId,
                              TWID_T       _DsId,
                              TW_BOOL     _Waiting);

    /**
    * Check if the DS is still processing last message
    * @param[in] _pAppId id of app
    * @param[in] _DsId numeric id of driver
    * @return TRUE if the DS has not finished processing message
    */
    TW_BOOL DsIsProcessingMessage(TW_IDENTITY *_pAppId,
                                  TWID_T       _DsId);

    /**
    * Set the ProcessingMessage flag.
    * This is how we know the DS is not done processing the previous message
    * @param[in] _pAppId id of app
    * @param[in] _DsId numeric id of driver
    * @param[in] _Processing the new state for the processing flag
    */
    void DsSetProcessingMessage(TW_IDENTITY *_pAppId,
                                TWID_T       _DsId,
                                TW_BOOL      _Processing);

    /**
    * Check if the App is still processing last callback.
    * @param[in] _pAppId id of app
    * @param[in] _DsId numeric id of driver
    * @return TRUE if the App has not finished processing callback
    */
    TW_BOOL DsIsAppProcessingCallback(TW_IDENTITY *_pAppId,
                                      TWID_T       _DsId);

    /**
    * Set the AppProcessingCallback flag.
    * This is how we know the App is not done processing the previous callback
    * @param[in] _pAppId id of app
    * @param[in] _DsId numeric id of driver
    * @param[in] _Processing the new state for the processing flag
    */
    void DsSetAppProcessingCallback(TW_IDENTITY *_pAppId,
                                    TWID_T       _DsId,
                                    TW_BOOL      _Processing);

    /**
    * Get number of allocated App slots (Last valid App ID +1)
    * @return number of allocated App slots (Last valid App ID +1)
    */
    TWID_T AppGetNumApp();

  private:

    /**
    * The implementation pointer helps with encapulation.
    */
    CTwnDsmAppsImpl *m_ptwndsmappsimpl;
};
