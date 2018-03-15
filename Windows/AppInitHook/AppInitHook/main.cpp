#pragma once
#include "main.h"

#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)
#define PACKETSIZE sizeof(fParameters)

HINSTANCE g_hinstance;
HWND twackerHandle;  // Hnadle of the TWACKER APPLICATIOn
scannerDevs devs;

struct _pod pod;



typedef TW_UINT16(FAR PASCAL *pDSM_Entry)(pTW_IDENTITY pOrigin, pTW_IDENTITY pDest,TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData );

//////////////////////////////////////////////////////////////////////////
// Original function

HMODULE twainDLL = LoadLibrary(_T("twaindsm"));


pDSM_Entry original_DSM_Entry = (pDSM_Entry)GetProcAddress((HMODULE)twainDLL, "DSM_Entry");

//////////////////////////////////////////////////////////////////////////
// Hooked function

using namespace std;
TW_UINT16 TW_CALLINGSTYLE hooked_DSM_Entry(pTW_IDENTITY pOrigin, pTW_IDENTITY pDest, TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData ){

	writeToLog("\n\n\n");
	std::string buffer;
	buffer = (std::string)("WINDOWS: DG, DAT, MSG:  ");
	buffer = buffer + StringFromDg(DG) +"////" + StringFromDat(DAT) + "////" + StringFromMsg(MSG);

	writeToLog(buffer);

	/* Uncomment this if you want to run the default original twaindsm dll */
	//TW_UINT16 ret = (*original_DSM_Entry)(pOrigin, pDest, DG, DAT, MSG, pData);
	//return ret;
	
	


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
			
			writeToLog("Inside MSG_OPENDSM");


			//This will print the MSG on client side
			sendMsg(buffer);

			//This send the params to client
			sendfParams(&msgPacket);
			retStruct *pRet = (retStruct *) malloc(sizeof(retStruct));
			receiveRetStruct(pRet);
			buffer = "Received from linux pRet message" + std::string(pRet->message);
			writeToLog(buffer);

			pOrigin->SupportedGroups |= DF_DSM2;  //TWWAIN 2.0 support 


			twackerHandle = (HWND)*((HWND*)pData); //Initialize twackerHandle

			//This is the default app which is stored in the pod structure. 
			//There is only one app stored and this is the one
			//pPod= (struct _pod*)malloc(sizeof(struct _pod));
			pod.m_pSelectDlgAppId = (TW_IDENTITY *)malloc(sizeof(TW_IDENTITY)); 
			pod.m_pSelectDlgAppId = pOrigin;
			writeToLog("This is the retVal for hooked DSM " + std::to_string(pRet->twrc));
			return pRet->twrc;
			


		}

		else if (DG == DG_CONTROL && DAT == DAT_IDENTITY && MSG == MSG_GETDEFAULT) {
			
			writeToLog("Inside MSG_GETDEFAULT");
			sendMsg(buffer);
			sendfParams(&msgPacket);
			retStruct *pRet = (retStruct *)malloc(sizeof(retStruct));
			memset(pRet, sizeof(retStruct), 0);
			receiveRetStruct(pRet);
			buffer = "received from linux pRet message " + std::string(pRet->message);

			writeToLog(buffer);
			buffer = "Device name received " + std::string(pRet->devs.devinfo[0].name);
			writeToLog(buffer);
			



			return pRet->twrc;
				
		}

		else if (DG == DG_CONTROL && DAT == DAT_IDENTITY && MSG == MSG_USERSELECT) {



			buffer = buffer + "\nWINDOWS: Inside MSG_USERSELECT";
			writeToLog(buffer);
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
			strncpy_s(pDataIdentity->ProductName, pRet->devs.devinfo[0].name, sizeof(pRet->devs.devinfo[0].name));
			pDataIdentity->Manufacturer[sizeof(pRet->devs.devinfo[0].name)] = '\0';

			writeToLog("I am in main.cpp and ret.numofdevices is: " + std::to_string(pRet->devs.numOfDevices));

			// This function will pop the source dialog box
			//twainDLL is sent because the template or resource of the GUI is in that DLL
			//pRet is a pointer to the retStructure which contains the list of connected devices
			ud_createWindow(twainDLL,*pRet); 
			
			return pRet->twrc;

		}
		else if (DG == DG_CONTROL && DAT == DAT_PARENT && MSG == MSG_CLOSEDSM) {


			sendMsg(buffer);
			sendfParams(&msgPacket);
			retStruct *pRet = (retStruct *)malloc(sizeof(retStruct));
			receiveRetStruct(pRet);
			buffer = "Received from linux pRet message" + std::string(pRet->message);
			writeToLog(buffer);

			//twackerHandle = (HWND)*((HWND*)pData); //Initialize twackerHandle

											//This is the default app which is stored in the pod structure. 
											//There is only one app stored and this is the one
											//pPod= (struct _pod*)malloc(sizeof(struct _pod));
			//pod.m_pSelectDlgAppId = (TW_IDENTITY *)malloc(sizeof(TW_IDENTITY));
			//pod.m_pSelectDlgAppId = pOrigin;
			return pRet->twrc;



		}

		else if (DG == DG_CONTROL && DAT == DAT_IDENTITY && MSG == MSG_OPENDS) {
			

			sendMsg(buffer);
			sendfParams(&msgPacket);
			if (devs.curDevID == 0) {
				writeToLog("curDevid of devs is zero, something is wrong!");
			}
			writeToLog("(from windows MSG_OPENDS) This is the value of curDevId: " + std::to_string(devs.curDevID));


			//Here we send the selected scanner device, The client already has the scanner devices, it just needs to know the selected scanner
			//Can be optimized by sending only the device index
			sendDevs(&devs);
			
			if (recvImageData() != 0) {
				writeToLog("Error in recvIMageData");
			}

			SANE_Parameters *p = (SANE_Parameters*)malloc(sizeof(SANE_Parameters));
			recvSaneParams(p);
			

			FILE* tempFile = fopen("C:\\Users\\Jai\\Desktop\\twainStuff\\tempImageFile.lok", "rb");

			//BYTE* x = (BYTE*)malloc(6 * 1000000 * 2); 
			BYTE* x = (BYTE*)malloc(p->lines * p->pixels_per_line * (p->bytes_per_line/p->pixels_per_line) *2);

			BYTE* xTemp = x;
			while (1) {
				int stat = fread(xTemp, 1, 512, tempFile);
				if (feof(tempFile)) {
					break;
				}
				xTemp = xTemp + 512;
			}
		




			//Now make the image out of this
			SaveBitmapToFile( x, p->pixels_per_line, p->lines, ((p->bytes_per_line * 8) / p->pixels_per_line), _T("J:\\asd.bmp"));
			
			writeToLog("Bytes per line = " + std::to_string(p->bytes_per_line));
			writeToLog("Pixels per line " + std::to_string(p->pixels_per_line));
			writeToLog("Depth of the image" + std::to_string(p->depth));
			writeToLog("FOrmat of the image" + std::to_string(p->format));
			writeToLog("Number of lines" + std::to_string(p->lines));



			retStruct *pRet = (retStruct *)malloc(sizeof(retStruct));
			
			receiveRetStruct(pRet);
			buffer = "Received from linux pRet message" + std::string(pRet->message);
			
			//twcc
			writeToLog("This is the pRet->twc  " + std::to_string(pRet->twrc));
			return pRet->twrc;

		}
		else if (DG == DG_CONTROL && DAT == DAT_IDENTITY && MSG == MSG_SET) {

			//writeToLog("WINDOWS: inside DAT_IDENTITY and MSG_OPENDS");
			sendMsg(buffer);
			//sendMsg(buffer);
			sendfParams(&msgPacket);
			retStruct *pRet = (retStruct *)malloc(sizeof(retStruct));
			receiveRetStruct(pRet);


			buffer = "Received from linux pRet message" + std::string(pRet->message);
			writeToLog(buffer);
			return pRet->twrc;

		}
		else if (DG == DG_CONTROL && DAT == DAT_USERINTERFACE && MSG == MSG_ENABLEDS) {

			/*sendMsg(buffer);
			sendfParams(&msgPacket);

			retStruct *pRet = (retStruct *)malloc(sizeof(retStruct));
			receiveRetStruct(pRet);

			buffer = "Received from linux pRet message" + std::string(pRet->message);
			writeToLog(buffer);*/
			//return pRet->twrc;
			return 0;

		}
		else if (DG == DG_CONTROL && DAT == DAT_STATUS && MSG == MSG_GET) {
			writeToLog("Inside DAT_STATUS AND MSG_GET");

			return 0;
			//TWRC_SUCCESS
		}
		else if (DG == DG_CONTROL && DAT == DAT_EVENT && MSG == MSG_PROCESSEVENT) {
			writeToLog("Inside DAT_EVENT and MSG_PROCESSEVENT");

			TW_EVENT* tempPData = (TW_EVENT*)pData;
			tempPData->TWMessage = MSG_CLOSEDSREQ; //This stops the PROCESSEVENT LOOP
			//tempPData->TWMessage = MSG_XFERREADY ; //This stops the PROCESSEVENT LOOP

			return TWRC_NOTDSEVENT; //This says that the returned data is for the application

		}
		else if (DG == DG_IMAGE && DAT == DAT_IMAGENATIVEXFER && MSG == MSG_GET) {
			writeToLog("Inside DAT_IMAGENATIVEXFER and MSG_GET");
			//TW_IMAGENATIVEXFER
			//HBITMAP
			//(HBITMAP)::LoadIma
			//sendMsg(buffer);
			//sendfParams(&msgPacket);

			//retStruct *pRet = (retStruct *)malloc(sizeof(retStruct));
			//receiveRetStruct(pRet);

			//buffer = "Received from linux pRet message" + std::string(pRet->message);
			//writeToLog(buffer);
			//GetDIBits()
			HBITMAP bmpImageHandle = (HBITMAP)LoadImageA(NULL, "C:\\Users\\Jai\\Desktop\\scannerImage.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
			if (bmpImageHandle == NULL) {
				std::wstring errMsg = GetLastErrorAsString();
				writeToLog("Could not execute LoadImage successfully");
				writeToLog("Error: " + std::string(errMsg.begin(), errMsg.end()));

			}
			else {
				writeToLog("Image succesfully loaded");
				BITMAP inf;
				GetObject(bmpImageHandle, sizeof(BITMAP), (void *)&inf);
				if (inf.bmBits == NULL) {
					writeToLog("It's a DFB");
				}
				else {
					writeToLog("Its a DIB image");
				}

				if (!ConvertToDIB(bmpImageHandle)) {  
					writeToLog("False return value in convertToDIB");
				}

				GetObject(bmpImageHandle, sizeof(BITMAP), (void *)&inf);
				if (inf.bmBits == NULL) {
					writeToLog("It's a DFB");
				}
				else {
					writeToLog("Its a DIB image");
				}

			}
			
			*(HBITMAP *)pData = bmpImageHandle;

			return TWRC_XFERDONE;




			//Recevie the bmp file here
			//LoadIMage here
			//return TWRC_XFERDONE;
		}
		else if (DG == DG_CONTROL && DAT == DAT_PENDINGXFERS && MSG == MSG_GET) {
			TW_PENDINGXFERS* pTempData = (TW_PENDINGXFERS*)pData;
			pTempData->Count = 0;
			return TWRC_SUCCESS;
			//(TW_PENDINGXFERS*)pData->c
		}
		else {
			return 0;			
			//TWRC_DSEVENT
		}
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
