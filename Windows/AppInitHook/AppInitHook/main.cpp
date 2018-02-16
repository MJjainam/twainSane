#pragma once
#include "main.h"

#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)
#define PACKETSIZE sizeof(fParameters)

HINSTANCE g_hinstance;
HWND g_hwnd;

struct _pod pod;



typedef TW_UINT16(FAR PASCAL *pDSM_Entry)(pTW_IDENTITY pOrigin, pTW_IDENTITY pDest,TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData );

//////////////////////////////////////////////////////////////////////////
// Original function

HMODULE twainDLL = LoadLibrary(_T("twaindsm"));


pDSM_Entry original_DSM_Entry = (pDSM_Entry)GetProcAddress((HMODULE)twainDLL, "DSM_Entry");

//////////////////////////////////////////////////////////////////////////
// Hooked function


TW_UINT16 TW_CALLINGSTYLE hooked_DSM_Entry(pTW_IDENTITY pOrigin, pTW_IDENTITY pDest, TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData ){

	//TW_UINT16 ret = (*original_DSM_Entry)(pOrigin, pDest, DG, DAT, MSG, pData);
	//return ret;
	//std::string buffer = "Twain dll hooked succesfully";
	//writeToLog(buffer);
	
	
	writeToLog("\n\n\n");
	std::string buffer;
	buffer = (std::string)("WINDOWS: DG, DAT, MSG:  ");
	buffer = buffer + StringFromDg(DG) +"////" + StringFromDat(DAT) + "////" + StringFromMsg(MSG);

	writeToLog(buffer);
	
	fParameters msgPacket;
	if (pDest == NULL) {
		msgPacket = {
			(TW_IDENTITY)(*pOrigin),
			{0},
			DG,
			DAT,
			MSG
		};
		
	}
	else {
		msgPacket = {
			(TW_IDENTITY)(*pOrigin),
			(TW_IDENTITY)(*pDest),
			DG,
			DAT,
			MSG
		};
	}
	

		if (DG == DG_CONTROL && DAT == DAT_PARENT && MSG == MSG_OPENDSM) {
			

			sendMsg(buffer);
			sendfParams(&msgPacket);
			retStruct *pRet = (retStruct *) malloc(sizeof(retStruct));
			receiveRetStruct(pRet);
			buffer = "Received from linux pRet message" + std::string(pRet->message);
			writeToLog(buffer);

			g_hwnd = (HWND)*((HWND*)pData); //Initialize g_hwnd

			//This is the default app which is stored in the pod structure. 
			//There is only one app stored and this is the one
			//pPod= (struct _pod*)malloc(sizeof(struct _pod));
			pod.m_pSelectDlgAppId = (TW_IDENTITY *)malloc(sizeof(TW_IDENTITY)); 
			pod.m_pSelectDlgAppId = pOrigin;
			return pRet->twrc;
			


		}

		else if (DG == DG_CONTROL && DAT == DAT_IDENTITY && MSG == MSG_GETDEFAULT) {
			//TW_IDENTITY *pRetData = (TW_IDENTITY*)pData;
			//TW_IDENTITY retData = (TW_IDENTITY)(*pRetData);
			writeToLog("I am inside MSG_GETDEFAULT");
			sendMsg(buffer);
			sendfParams(&msgPacket);
			retStruct *pRet = (retStruct *)malloc(sizeof(retStruct));
			memset(pRet, sizeof(retStruct), 0);
			receiveRetStruct(pRet);
			buffer = "received from linux pRet message" + std::string(pRet->message);

			writeToLog(buffer);
			buffer = "Device name received " + std::string(pRet->devInfo[0].name);
			writeToLog(buffer);
			TW_IDENTITY* pDataIdentity = (TW_IDENTITY*)pData;
			pDataIdentity->SupportedGroups = DF_APP2 | DF_DSM2 | DF_DS2; //Deliberately did this without checking  
			strncpy_s(pDataIdentity->Manufacturer,"Jainam",6);
			pDataIdentity->Manufacturer[6] = '\0';
			strncpy_s(pDataIdentity->ProductFamily, "MJ", 2);
			pDataIdentity->Manufacturer[2] = '\0';
			strncpy_s(pDataIdentity->ProductName, pRet->devInfo[0].name,sizeof(pRet->devInfo[0].name));
			pDataIdentity->Manufacturer[sizeof(pRet->devInfo[0].name)] = '\0';



			return pRet->twrc;
				
			//buffer = buffer + "\nThis is the product name when MSG_GETDEFAULT is sent: " + retData.ProductName;
		}

		else if (DG == DG_CONTROL && DAT == DAT_IDENTITY && MSG == MSG_USERSELECT) {
			//TW_IDENTITY *pRetData = (TW_IDENTITY*)pData;
			//TW_IDENTITY retData = (TW_IDENTITY)(*pRetData);

			writeToLog("Inside third switch case");
			buffer = "Inside third switch case";
			sendMsg(buffer);
			sendfParams(&msgPacket);
			retStruct *pRet = (retStruct *)malloc(sizeof(retStruct));
			receiveRetStruct(pRet);
			TW_IDENTITY* pDataIdentity = (TW_IDENTITY*)pData;
			pDataIdentity->SupportedGroups = DF_APP2 | DF_DSM2 | DF_DS2; //Deliberately did this without checking  
			strncpy_s(pDataIdentity->Manufacturer, "Jainam", 6);
			pDataIdentity->Manufacturer[6] = '\0';
			strncpy_s(pDataIdentity->ProductFamily, "MJ", 2);
			pDataIdentity->Manufacturer[2] = '\0';
			strncpy_s(pDataIdentity->ProductName, pRet->devInfo[0].name, sizeof(pRet->devInfo[0].name));
			pDataIdentity->Manufacturer[sizeof(pRet->devInfo[0].name)] = '\0';


			ud_createWindow(twainDLL,g_hwnd, pod, *pRet);



			return pRet->twrc;
			//buffer = buffer + "\nThis is the product name when MSG_USERSELECT is sent: "  + retData.ProductName;
			//connectTo(buffer, serialData, "10.106.132.64", 2222);




		}
		return 0;
		//return retVal;

}
//////////////////////////////////////////////////////////////////////////
// Entry point

	BOOL WINAPI DllMain(
		__in HINSTANCE  hInstance,
		__in DWORD      Reason,
		__in LPVOID     Reserved
	)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		Mhook_SetHook((PVOID*)&original_DSM_Entry, hooked_DSM_Entry);
		g_hinstance = hInstance;  //Initialize g_hinstance
		break;

	case DLL_PROCESS_DETACH:
		Mhook_Unhook((PVOID*)&original_DSM_Entry);
		g_hinstance = 0;
		break;
	}

	return TRUE;
}