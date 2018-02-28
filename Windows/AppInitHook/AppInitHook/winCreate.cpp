#pragma once
#include "winCreate.h"
#include "log.h"
#include <locale>
#include <codecvt>
#include <string>
#include "tools.h"

retStruct ret;
extern HWND g_hwnd; //handle of the application which is also declared in main
extern struct _pod pod; 
extern scannerDevs devs;

DS_INFO winDataSources[MAX_DEVICES];



int ud_createWindow(HINSTANCE g_hinstance, HWND g_hwnd1, struct _pod pod1, retStruct ret1) {
	int result;
	writeToLog("Entered ud_createWindow");
	if (g_hinstance == 0) {
		writeToLog("The g_hisntance is null");
		//return TW;
	}
	if (g_hwnd == 0) {
		writeToLog("the g_hwnd is null");
		//return -1;
	}
	pod = pod1;
	ret = ret1;
	g_hwnd = g_hwnd1;

	int ret = DialogBoxW(g_hinstance,
		(LPCWSTR)MAKEINTRESOURCE(IDD_DLG_SOURCE),
		(HWND)g_hwnd,
		(DLGPROC)SelectDlgProc);
	result = TWRC_SUCCESS;
	if (ret == IDOK)
	{

		writeToLog("in ret == IDOK");
		//devs.curDevID = 
		//pod.devId = 
		// Validate the result...
		//if (!pod.m_pSelectDlgDsId)
		//{
		//	//kLOG((kLOGERR, "We came out of the Select Dialog with a null..."));
		//	pod.m_ptwndsmapps->AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
		//	return TWRC_FAILURE;
		//}

		// Copy the data over...
		//*_pDsId = *pod.m_pSelectDlgDsId;

		// save default source to Registry  
		// sanity check...
		//if ((pod.m_pSelectDlgDsId->Id < 1)
		//	|| (pod.m_pSelectDlgDsId->Id >= MAX_NUM_DS))
		//{
		//	// Failed to save default DS to registry
		//	kLOG((kLOGERR, "Id is out of range 0 - 49..."));
		//	// Nothing preventing us from using the default right now
		//	pod.m_ptwndsmapps->AppSetConditionCode(_pAppId, TWCC_BUMMER);
		//}

		//else
		//{
			// Get the path we're using...
			//status = ERROR_SUCCESS;
			//szPath = pod.m_ptwndsmapps->DsGetPath(pod.m_pSelectDlgAppId, pod.m_pSelectDlgDsId->Id);

			// Open the key, creating it if it doesn't exist.
			//if (RegCreateKeyEx(HKEY_CURRENT_USER,
			//	TWNDSM_DS_REG_LOC,
			//	NULL,
			//	NULL,
			//	NULL,
			//	KEY_READ | KEY_WRITE, NULL,
			//	&hKey,
			//	NULL) == ERROR_SUCCESS)
			//{
			//	status = RegSetValueEx(hKey, "Default Source", 0, REG_SZ, (LPBYTE)szPath, (DWORD)strlen((char*)szPath) + 1);
			//	if (status != ERROR_SUCCESS)
			//	{
			//		// Failed to save default DS to registry
			//		kLOG((kLOGERR, "Failed to save default DS to registry"));
			//		// Nothing preventing us from using the default right now
			//		pod.m_ptwndsmapps->AppSetConditionCode(_pAppId, TWCC_BUMMER);
			//	}
			//}
			// Close the key.
			//RegCloseKey(hKey);
		//}
	}

	// We're cancelling...
	else if (ret == IDCANCEL)
	{
		writeToLog("in ret ==IDCANCEL");

		result = TWRC_CANCEL;
		//return -1;
	}

	// Something back happened...
	else if (ret == -1)
	{
		writeToLog("in ret ==-1, something bad happened");

		
		std::wstring errMsg = GetLastErrorAsString();
		writeToLog("Error " + std::string(errMsg.begin(), errMsg.end()));
		MessageBox(NULL, (LPCWSTR)errMsg.c_str(),(LPCWSTR) L"Error", MB_OK | MB_ICONINFORMATION);
		//pod.m_ptwndsmapps->AppSetConditionCode(_pAppId, TWCC_BUMMER);
		result = TWRC_FAILURE;
		//return -1;
	}
	//writeToLog("returning result");
	//std::wstring errMsg = GetLastErrorAsString();
	//writeToLog("Error " + std::string(errMsg.begin(), errMsg.end()));
	return result;


	
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam){

	SendMessage(hwnd, WM_SETFONT, (WPARAM)lParam, (LPARAM)TRUE);
	return TRUE;
}

BOOL SelectDlgProc(HWND hWnd,UINT Message,WPARAM wParam,LPARAM /*lParam - unused*/){

	//writeToLog("Inside SelectDlgProc");

	TW_IDENTITY  *pAppId;
	int           nIndex;
	int           nSelect;
	TW_UINT32     x;
	char         *szProductName;
	HWND          hListBox;
	RECT          rectParent;
	RECT          rectSelect;
	HWND          hParent;
	LANGID        LangId;
	wchar_t       uzUnicode[128];
	HFONT         hfont;
	LOGFONT       lf;
	POINT         point;
	int           nWidth;
	int           nHeight;

	// Init stuff...
	nSelect = 0;
	pAppId = pod.m_pSelectDlgAppId;  //Tells about the application

	// Process the message...
	switch (Message)
	{
	writeToLog("inside switch in winCreate");
	case WM_INITDIALOG:

		// If the caller wants us to figure out the language, do this...
		if (pAppId->Version.Language == (TW_UINT16)TWLG_USERLOCALE)
		{
			// Try to find the user's default language...
			LangId = ::GetUserDefaultLangID();
			for (nIndex = 0;
				s_twlocalize[nIndex].Language >= 0;
				nIndex++)
			{
				if ((s_twlocalize[nIndex].LangId == LangId)
					&& s_twlocalize[nIndex].LangId
					&&  s_twlocalize[nIndex].Title
					&&  s_twlocalize[nIndex].Title[0]
					&& s_twlocalize[nIndex].Sources
					&&  s_twlocalize[nIndex].Sources[0]
					&& s_twlocalize[nIndex].Select
					&&  s_twlocalize[nIndex].Select[0]
					&& s_twlocalize[nIndex].Cancel
					&&  s_twlocalize[nIndex].Cancel[0])
				{
					break;
				}
			}

			// If that doesn't work, try for the primary language...
			if (s_twlocalize[nIndex].Language < 0)
			{
				LangId &= 0xFF;
				for (nIndex = 0;
					s_twlocalize[nIndex].Language >= 0;
					nIndex++)
				{
					if (((s_twlocalize[nIndex].LangId & 0xFF) == LangId)
						&& s_twlocalize[nIndex].LangId
						&&  s_twlocalize[nIndex].Title
						&&  s_twlocalize[nIndex].Title[0]
						&& s_twlocalize[nIndex].Sources
						&&  s_twlocalize[nIndex].Sources[0]
						&& s_twlocalize[nIndex].Select
						&&  s_twlocalize[nIndex].Select[0]
						&& s_twlocalize[nIndex].Cancel
						&&  s_twlocalize[nIndex].Cancel[0])
					{
						break;
					}
				}
			}
		}

		// Otherwise, use whatever the caller gave us...
		else
		{
			for (nIndex = 0;
				s_twlocalize[nIndex].Language >= 0;
				nIndex++)
			{
				if ((s_twlocalize[nIndex].Language == pAppId->Version.Language)
					&& s_twlocalize[nIndex].LangId
					&&  s_twlocalize[nIndex].Title
					&&  s_twlocalize[nIndex].Title[0]
					&& s_twlocalize[nIndex].Sources
					&&  s_twlocalize[nIndex].Sources[0]
					&& s_twlocalize[nIndex].Select
					&&  s_twlocalize[nIndex].Select[0]
					&& s_twlocalize[nIndex].Cancel
					&&  s_twlocalize[nIndex].Cancel[0])
				{
					break;
				}
			}
		}

		// If we didn't find our language, go for English...
		if (s_twlocalize[nIndex].Language < 0)
		{
			for (nIndex = 0;
				s_twlocalize[nIndex].Language >= 0;
				nIndex++)
			{
				if ((s_twlocalize[nIndex].Language == TWLG_ENGLISH)
					&& s_twlocalize[nIndex].LangId
					&&  s_twlocalize[nIndex].Title
					&&  s_twlocalize[nIndex].Title[0]
					&& s_twlocalize[nIndex].Sources
					&&  s_twlocalize[nIndex].Sources[0]
					&& s_twlocalize[nIndex].Select
					&&  s_twlocalize[nIndex].Select[0]
					&& s_twlocalize[nIndex].Cancel
					&&  s_twlocalize[nIndex].Cancel[0])
				{
					break;
				}
			}
		}
		if (s_twlocalize[nIndex].Language >= 0)
		{
			// Set our font...
			memset(&lf, 0, sizeof(lf));
			lf.lfHeight = 16;
			lf.lfCharSet = s_twlocalize[nIndex].CharSet;
			hfont = CreateFontIndirect(&lf);
			EnumChildWindows(hWnd, EnumChildProc, (LPARAM)hfont);

			MultiByteToWideChar(CP_UTF8, 0, s_twlocalize[nIndex].Title, -1, uzUnicode, sizeof(uzUnicode) / sizeof(*uzUnicode));
			SetWindowTextW(hWnd, uzUnicode);

			MultiByteToWideChar(CP_UTF8, 0, s_twlocalize[nIndex].Sources, -1, uzUnicode, sizeof(uzUnicode) / sizeof(*uzUnicode));
			SetWindowTextW(::GetDlgItem(hWnd, IDC_STATIC), uzUnicode);

			MultiByteToWideChar(CP_UTF8, 0, s_twlocalize[nIndex].Select, -1, uzUnicode, sizeof(uzUnicode) / sizeof(*uzUnicode));
			SetWindowTextW(::GetDlgItem(hWnd, IDOK), uzUnicode);

			MultiByteToWideChar(CP_UTF8, 0, s_twlocalize[nIndex].Cancel, -1, uzUnicode, sizeof(uzUnicode) / sizeof(*uzUnicode));
			SetWindowTextW(::GetDlgItem(hWnd, IDCANCEL), uzUnicode);
		}

		hListBox = ::GetDlgItem(hWnd, ID_LST_SOURCES);
		if (hListBox)
		{
			writeToLog("Got the hListBox");
			SendMessage(hListBox, LB_RESETCONTENT, (WPARAM)NULL, (LPARAM)NULL);
			writeToLog("Number of devices found are " + std::to_string(ret.devs.numOfDevices));
			for (int x = 1; x <= ret.devs.numOfDevices/*MAX_DEVICES*/; ++x)
			{
				// We expect the list to be contiguous...
				szProductName = ret.devs.devinfo[x - 1].vendor;
				strncat(szProductName, " ", 1);
				strncat(szProductName, ret.devs.devinfo[x - 1].model, strlen(ret.devs.devinfo[x - 1].model) + 1);
				if (!szProductName[x-1])
				{
					break;
				}

				//int stat = getWinDataSource(szProductName, &winDataSources[x-1]);

				//getWinDataSource(szProductName, &winDataSources[x-1]);

				//if (stat == -1) {
					//continue;
				//}
				//else {

					nIndex = (int)SendMessage(hListBox, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)GetWC(szProductName));
					if (LB_ERR == nIndex)
					{
						writeToLog("Could not display the name");
						break;
					}
				//}
				// Display the name...
				// Associate the id with the name...
				//I am assosiating the index of DS with the name
				nIndex = (int)SendMessage(hListBox,
					LB_SETITEMDATA,
					(WPARAM)nIndex,
					(LPARAM)x/*pPod->m_ptwndsmapps->DsGetIdentity(pAppId, x)->Id*/);
				if (LB_ERR == nIndex)
				{
					writeToLog("Could not assosiate ID with the name");
					break;
				}
				// Remember this item if it's the default...
				//if (!strcmp(pPod->m_ptwndsmapps->DsGetPath(pAppId, x), pPod->m_DefaultDSPath))
				//{
					//nSelect = x; //Select the last found device
				//}
			}
			// If we have no drivers, then disable the OK button...
			/*if (pPod->m_ptwndsmapps->AppGetNumDs(pAppId) < 1)
			{
				HWND hOK = ::GetDlgItem(hWnd, IDOK);
				EnableWindow(hOK, FALSE);
			}*/
			// Otherwise select the defaulted item...
			/*else
			{*/
				nIndex = (int)SendMessage(hListBox,
					LB_FINDSTRINGEXACT,
					(WPARAM)-1,
					(LPARAM)GetWC(ret.devs.devinfo[0].name));
				if (LB_ERR == nIndex)
				{
					writeToLog("Could not get the default DS by product name");
					nIndex = 0;
				}
				SendMessage(hListBox, LB_SETCURSEL, (WPARAM)nIndex, (LPARAM)NULL);
			//}
		}

		// Center our dialog on the window reported to us in MSG_OPENDS...
		hParent = g_hwnd;
		if (hParent)
		{
			GetClientRect(hParent, &rectParent);
			GetWindowRect(hWnd, &rectSelect);
			nWidth = (rectSelect.right - rectSelect.left);
			nHeight = (rectSelect.bottom - rectSelect.top);
			point.x = (rectParent.right - rectParent.left) / 2;
			point.y = (rectParent.bottom - rectParent.top) / 2;
			ClientToScreen(hParent, &point);
			point.x -= nWidth / 2;
			point.y -= nHeight / 2;
			// keep the dialog visible on the screen
			if (point.x < 0)
			{
				point.x = 0;
			}
			if (point.y < 0)
			{
				point.y = 0;
			}
			MoveWindow(hWnd, point.x, point.y, nWidth, nHeight, FALSE);
		}

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_LST_SOURCES:
			if (HIWORD(wParam) != LBN_DBLCLK)
				break;
			// drop through...

		case IDOK:
			{
				hListBox = ::GetDlgItem(hWnd, ID_LST_SOURCES);
				nIndex = 0;
				if (hListBox)
				{
					nIndex = (int)SendMessage(hListBox, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
					if (LB_ERR == nIndex)
					{
						// if there is no selection should not have OK available
						// to press in the first place.
						writeToLog("No devices found and hence returning ");
						return TRUE;
					}
					//writeToLog("This is the first index: " + std::to_string(nIndex));
					nIndex = (int)SendMessage(hListBox, LB_GETITEMDATA, (WPARAM)nIndex, (LPARAM)0);
					if (LB_ERR != nIndex)
					{
						//pPod->m_pSelectDlgDsId = pod.m_ptwndsmapps->DsGetIdentity(pAppId, nIndex);
						devs.curDevID = nIndex;
						writeToLog("initialized devs.curDevID to: " + std::to_string(nIndex));
					}
					//writeToLog("This is the second index: " + std::to_string(nIndex));
					//pod.devId = nIndex;

				}
				EndDialog(hWnd, IDOK);
				return TRUE;
			}
		case IDCANCEL:
			EndDialog(hWnd, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int getWinDataSource(char *szProductName, DS_INFO* pwinDataSource) {
	writeToLog("in getWinDataSource");
	char dsDir[FILENAME_MAX];
	GetWindowsDirectoryA(dsDir, sizeof(dsDir));
	strncat(dsDir, "\\", 3);
	strncat(dsDir, "twain_32", 9); //This is the directory where the data sources are stored
	writeToLog("The value in dsDir is this " + std::string(dsDir));
	int stat = scanDSDir(szProductName, dsDir, pwinDataSource);
	if (stat == EXIT_FAILURE) {
		return -1;
	}
	else {
		return 0;
	}



}

int scanDSDir(char *szProductName, char *_szAbsPath, DS_INFO* pwinDataSource)
{
	// Validate...
	writeToLog("In scanDSDir with path " + std::string(_szAbsPath));
	if (!_szAbsPath)
	{
		return EXIT_FAILURE;
	}


	WIN32_FIND_DATAA   FileData;             // Data structure describes the file found
	HANDLE            hSearch;              // Search handle returned by FindFirstFile
	char              szABSFilename[FILENAME_MAX];
	BOOL              bFinished = FALSE;
	char              szPrevWorkDir[FILENAME_MAX];

	// Start searching for .ds files in the root directory.
	strncpy(szABSFilename, _szAbsPath, strlen(_szAbsPath)+1);
	strncat(szABSFilename, "\\*.ds", 7);
	writeToLog("searching in this file" + std::string(szABSFilename));
	hSearch = FindFirstFileA(szABSFilename, &FileData);

	// If we find something, squirrel it away and anything else we find...
	if (hSearch != INVALID_HANDLE_VALUE)   //If we find ds
	{
		/* Save the current working directory: */
		char *szResult = _getcwd(szPrevWorkDir, sizeof(szPrevWorkDir));
		if (szResult == (char*)NULL)
		{
			return EXIT_FAILURE;
		}
		int iResult = _chdir(_szAbsPath);
		if (iResult != 0)
		{
			return EXIT_FAILURE;
		}

		while (!bFinished)
		{
			if (snprintf(szABSFilename, FILENAME_MAX, "%s\\%s", _szAbsPath, FileData.cFileName) > 0)
			{
				if (
					TWRC_SUCCESS == LoadDS(szProductName,
					szABSFilename,
					pwinDataSource,
					/*m_AppInfo[(TWID_T)_pAppId->Id].pDSList->NumFiles + 1,*/
					false)
					)
				{
					//m_AppInfo[(TWID_T)_pAppId->Id].pDSList->NumFiles++;
					//As we habe found the similar product name return 
					return EXIT_SUCCESS;

				}
			}

			if (!FindNextFileA(hSearch, &FileData))
			{
				bFinished = TRUE;
			}
		}

		if (!FindClose(hSearch))
		{
			(void)_chdir(szPrevWorkDir);
			return EXIT_FAILURE;
		}
	}

	// Start searching sub directories.
	strncpy(szABSFilename,  _szAbsPath, strlen(_szAbsPath)+1);
	strncat(szABSFilename, "\\*.*", 6);
	hSearch = FindFirstFileA(szABSFilename, &FileData);
	bFinished = FALSE;
	if (hSearch == INVALID_HANDLE_VALUE)
	{
		(void)_chdir(szPrevWorkDir);
		return EXIT_FAILURE;
	}
	while (!bFinished)
	{
		if ((strcmp(".", FileData.cFileName) != 0)
			&& (strcmp("..", FileData.cFileName) != 0)  
			&& (strncmp("$",FileData.cFileName,1)!=0)
			&& (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (snprintf(szABSFilename,  FILENAME_MAX, "%s\\%s", _szAbsPath, FileData.cFileName) > 0)
			{
				scanDSDir(szProductName, szABSFilename, pwinDataSource);
			}
		}

		if (!FindNextFileA(hSearch, &FileData))
		{
			bFinished = TRUE;
		}
	}

	(void)_chdir(szPrevWorkDir);

	if (!FindClose(hSearch))
	{
		return EXIT_FAILURE;
	}
	
	return EXIT_FAILURE; //We did not find a similar name data source




}


TW_UINT16 LoadDS(char *szProductName, 
	char *_pPath, // This is the path to data source
	DS_INFO* pwinDataSource,
	bool         _boolKeepOpen)
{

	writeToLog("inside LoadDS with path " + std::string(_pPath));
	TW_INT16  result = TWRC_SUCCESS;
	DS_INFO  *pDSInfo = (DS_INFO*)malloc(sizeof(*pDSInfo));
	bool hook;
	TW_IDENTITY_LINUX64SAFE twidentitylinux64safe;
	char szUseAppid[8];

	// Validate...
	if (0 == _pPath)
	{
		// bad path
		writeToLog("In LoadDs, Bad path");
		//AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
		return TWRC_FAILURE;
	}
	//if (_DsId >= MAX_NUM_DS)
	//{
	//	// too many DS's already open
	//	kLOG((kLOGINFO, "Too many DS's already open."));
	//	AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
	//	return TWRC_FAILURE;
	//}



	// Initialize stuff...
	//pDSInfo = &m_AppInfo[(TWID_T)_pAppId->Id].pDSList->DSInfo[_DsId];
	//DS_INFO* pDSInfo;
	// Only log DS details when processing a MSG_OPENDS message
	if (_boolKeepOpen)
	{
		/*writeToLog("Datasource: \"%0.32s\"" + pDSInfo->Identity.Manufacturer));
		writeToLog("            \"%0.32s\"" + pDSInfo->Identity.ProductFamily));
		writeToLog("            \"%0.32s\" version: %u.%u" + pDSInfo->Identity.ProductName, pDSInfo->Identity.Version.MajorNum, pDSInfo->Identity.Version.MinorNum));
		writeToLog("            TWAIN %u.%u"+ pDSInfo->Identity.ProtocolMajor, pDSInfo->Identity.ProtocolMinor));*/
	}

	// Only hook this driver if we've been asked to keep the driver
	// open (meaning we're processing a MSG_OPENDS) and if we see
	// that the driver is 1.x...(by checking the absence of DF_DS2)
	hook = _boolKeepOpen && !(pDSInfo->Identity.SupportedGroups & DF_DS2);

	// Try to load the driver...  We load the driver again if we are keeping
	// it open.  This LoadLibrary is always closed so we dont hook this time.

	//pDSInfo->pHandle = (TW_HANDLE)LOADLIBRARY(_pPath, false, 0);
	pDSInfo->pHandle = LoadLibraryA(_pPath);
#if (TWNDSM_CMP == TWNDSM_CMP_VISUALCPP)
	if (0 == pDSInfo->pHandle)
	{
		writeToLog("Could not load library: " + std::string(_pPath));
		//AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
		return TWRC_FAILURE;
	}

#endif

	// Try to get the entry point...

#if (TWNDSM_CMP == TWNDSM_CMP_VISUALCPP)
		// The WIATwain.ds does not have an entry point 
		if (0 != strstr(_pPath, "wiatwain.ds"))
		{
			writeToLog("We're deliberately skipping this file: " +std::string(_pPath));
		}
		else
		{
			pDSInfo->DS_Entry = (DSENTRYPROC)GetProcAddress((HMODULE)pDSInfo->pHandle, MAKEINTRESOURCEA(1));

			if (pDSInfo->DS_Entry == 0)
			{
				//writeToLog("Could not find Entry 1 in DS: " +std::string(_pPath));
				writeToLog("could not get DS entry for the path " + std::string(_pPath));
			}
		}
#else
		kLOG((kLOGERR, "Could not find DS_Entry function in DS: %s", _pPath));
#endif
		if (pDSInfo->DS_Entry == 0)
		{
			(void)FreeLibrary((HMODULE)pDSInfo->pHandle);
			pDSInfo->pHandle = NULL;

//			AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
			return TWRC_FAILURE;
		}

	// Allllrighty then!  So the original TWAIN_32.DLL passes in
	// a value of NULL for the origin.  This is not documented
	// anywhere in the TWAIN Spec.  It was decided to maintain this
	// behavior in TWAINDSM.DLL.  All fine and well for Window and
	// Linux.  But Mac had it's own DSM, and it didn't pass in a
	// NULL.  So now we have a conundrum.
	//
	// I'm adding an event variable so that an application can
	// override stuff, but the default behavior is going to be:
	// Windows - NULL
	// Linux   - _pAppId
	// Mac     - _pAppId
//	memset(&szUseAppid, 0, sizeof(szUseAppid));
	//SGETENV(szUseAppid, NCHARS(szUseAppid), "TWAINDSM_USEAPPID");
	// No data received, set the default based on the platform...
//	if (szUseAppid[0] != 0)
	//{
//#if (TWNDSM_OS == TWNDSM_OS_WINDOWS)
//		szUseAppid[0] = '0'; // Windows is NULL	
//#elif (TWNDSM_OS == TWNDSM_OS_LINUX)
//		szUseAppid[0] = '1'; // Linux is _pAppId
//#elif (TWNDSM_OS == TWNDSM_OS_MACOSX)
//		szUseAppid[0] = '1'; // Linux is _pAppId
//#else
//		Unsupported...
//#endif
//	}
	// Otherwise, force the value to be '0' or '1'...
	/*else if (szUseAppid[0] != '0')
	{
		szUseAppid[0] = '1';
	}*/

	// Report success and squirrel away the index...
	//kLOG((kLOGINFO, "Loaded library: %s (TWAINDSM_USEAPPID:%c)", _pPath, szUseAppid[0]));
	//pDSInfo->Identity.Id = (TWIDDEST_T)_DsId;

	// Get the source to fill in the identity structure
	// This operation should never fail on any DS
	//
	// We need the NULL to be backwards compatible with the
	// older DSM.  This is the only way a driver can tell if
	// it's being talked to directly by the DSM instead of
	// by the application (with the DSM as a passthru).
	//
	// Okay, this is where we make the actual call.  I left
	// the original comments in place...
	memset(&twidentitylinux64safe, 0, sizeof(twidentitylinux64safe));
	//twidentitylinux64safe.twidentity.Id = (TWIDDEST_T)_DsId;
	//if (szUseAppid[0] == '1')
	//{
	//	// this is what the spec calls for
	//	//result = pDSInfo->DS_Entry(_pAppId, DG_CONTROL, DAT_IDENTITY, MSG_GET, (TW_MEMREF)&twidentitylinux64safe);
	//	result = pDSInfo->DS_Entry(NULL, DG_CONTROL, DAT_IDENTITY, MSG_GET, (TW_MEMREF)&twidentitylinux64safe);

	//}
	//else
	//{
	//	// this is out of spec, but we need it for Windows
	//	result = pDSInfo->DS_Entry(NULL, DG_CONTROL, DAT_IDENTITY, MSG_GET, (TW_MEMREF)&twidentitylinux64safe);
	//}

	result = pDSInfo->DS_Entry(NULL, DG_CONTROL, DAT_IDENTITY, MSG_GET, (TW_MEMREF)&twidentitylinux64safe);

	if (result != TWRC_SUCCESS)
	{
		(void)FreeLibrary((HMODULE)pDSInfo->pHandle);
		pDSInfo->pHandle = NULL;
		pDSInfo->DS_Entry = NULL;
		writeToLog("could not get DS entry for the path " + std::string(_pPath));
		//AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
		return TWRC_FAILURE;
	}

	// We're going to do a sanity check on the data if we
	// are running on Linux as a 64-bit process.  This is
	// because we messed up the definition of TW_INT32 and
	// TW_UINT32, making them 64-bit values (based on long)
	// rather than 32-bit values (based on int).  Starting
	// with TWAIN 2.4 this is fixed, but we have to be able
	// to handle old drivers.  These will be in trouble
	// because their Id and SupportedGroups will be 64-bit,
	// shifting data in the structure.
	//
	// This is a heuristic, meaning that it's possible to
	// get it wrong.  Add as many checks as possible.  All
	// TWAIN drivers must support DG_CONTROL and DG_IMAGE,
	// and we're going to validate a whole mess of protocol
	// versions...
#if (TWNDSM_OS == TWNDSM_OS_LINUX) && (TWNDSM_OS_64BIT == 1)
	if (((twidentitylinux64safe.twidentity.SupportedGroups & (DG_CONTROL | DG_IMAGE)) == (DG_CONTROL | DG_IMAGE))
		&& (((twidentitylinux64safe.twidentity.ProtocolMajor >= 3) && (twidentitylinux64safe.twidentity.ProtocolMinor <= 9))
			|| ((twidentitylinux64safe.twidentity.ProtocolMajor == 2) && (twidentitylinux64safe.twidentity.ProtocolMinor >= 4) && (twidentitylinux64safe.twidentity.ProtocolMinor <= 9))
			|| ((twidentitylinux64safe.twidentity.ProtocolMajor == 1) && (twidentitylinux64safe.twidentity.ProtocolMinor == 5))
			|| ((twidentitylinux64safe.twidentity.ProtocolMajor == 1) && (twidentitylinux64safe.twidentity.ProtocolMinor == 6))
			|| ((twidentitylinux64safe.twidentity.ProtocolMajor == 1) && (twidentitylinux64safe.twidentity.ProtocolMinor == 7))
			|| ((twidentitylinux64safe.twidentity.ProtocolMajor == 1) && (twidentitylinux64safe.twidentity.ProtocolMinor == 8))
			|| ((twidentitylinux64safe.twidentity.ProtocolMajor == 1) && (twidentitylinux64safe.twidentity.ProtocolMinor == 9))
			|| ((twidentitylinux64safe.twidentity.ProtocolMajor == 1) && (twidentitylinux64safe.twidentity.ProtocolMinor == 91))
			|| ((twidentitylinux64safe.twidentity.ProtocolMajor == 2) && (twidentitylinux64safe.twidentity.ProtocolMinor == 0))
			|| ((twidentitylinux64safe.twidentity.ProtocolMajor == 2) && (twidentitylinux64safe.twidentity.ProtocolMinor == 1))
			|| ((twidentitylinux64safe.twidentity.ProtocolMajor == 2) && (twidentitylinux64safe.twidentity.ProtocolMinor == 2))
			|| ((twidentitylinux64safe.twidentity.ProtocolMajor == 2) && (twidentitylinux64safe.twidentity.ProtocolMinor == 3))))
	{
		// We're good, keep going...
	}
	else
	{
		(void)UNLOADLIBRARY(pDSInfo->pHandle, false, 0);
		pDSInfo->pHandle = NULL;
		pDSInfo->DS_Entry = NULL;
		kLOG((kLOGINFO, "DG_CONTROL,DAT_IDENTITY,MSG_GET failed (rejected as old 64-bit TW_INT32/TW_UINT32)"));
		AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
		return TWRC_FAILURE;
	}
#endif

	// Okay, we can keep this TW_IDENTITY, so copy it over,
	// but be careful to use the TW_IDENTITY size...
	memcpy(&pDSInfo->Identity, &twidentitylinux64safe.twidentity, sizeof(pDSInfo->Identity));


	std::string buffer;
	buffer = "This is the name of the scanner connected to windows";
	buffer = buffer +  + "\n Manufacturer: " + pDSInfo->Identity.Manufacturer;
	buffer = buffer + +"Product Family:  " + pDSInfo->Identity.ProductFamily;
	buffer = buffer + +"ProductName:  " + pDSInfo->Identity.ProductName;
	
	writeToLog(buffer);


		
	if (strcmp(pDSInfo->Identity.ProductName, szProductName) != 0) {

		(void)FreeLibrary((HMODULE)pDSInfo->pHandle);
		pDSInfo->pHandle = NULL;
		pDSInfo->DS_Entry = NULL;
		//writeToLog("DG_CONTROL,DAT_IDENTITY,MSG_GET failed");
		std::string buffer = "Found product name is" + std::string(pDSInfo->Identity.ProductName) + "But we are looking for " + std::string(szProductName);
		//buffer = buffer + "\n" +std::string()
		writeToLog(buffer);
		//AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
		return TWRC_FAILURE;
	}

	// Compare the supported groups.  Note that the & is correct
	// because we are comparing bits...
	// we do not want to compare DG_CONTROL because is it supported by all
	//if (!((_pAppId->SupportedGroups & DG_MASK & ~DG_CONTROL)              // app supports
	//	& (pDSInfo->Identity.SupportedGroups & DG_MASK & ~DG_CONTROL))) // source supports
	//{
	//	(void)UNLOADLIBRARY(pDSInfo->pHandle, false, 0);
	//	pDSInfo->pHandle = NULL;
	//	pDSInfo->DS_Entry = NULL;
	//	kLOG((kLOGINFO, "The SupportedGroups do not match."));
	//	AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
	//	return TWRC_FAILURE;
	//}

	// The DS should not modify the Id even though the spec states
	// that the id will not be assigned until DSM sends MSG_OPENDS to DS, and
	// by the way...don't do the copy of the src and dst are the same address...
	//pDSInfo->Identity.Id = (TWIDDEST_T)_DsId;
	if (pDSInfo->szPath != _pPath)
	{
		strncpy(pDSInfo->szPath, _pPath, FILENAME_MAX);
	}

	// We clear the library to avoid cluttering up the virtual address space, and
	// to prevent scary weirdness that can result from multiple drivers being
	// loaded (if the application wants to load multiple drivers, that's its risk).
	(void)FreeLibrary((HMODULE)pDSInfo->pHandle);
	pDSInfo->pHandle = NULL;
	pDSInfo->DS_Entry = NULL;

	// At this point you're probably scratching your head.  Here's the deal.
	// When the DSM issues DG_CONTROL/DAT_IDENTITY/MSG_GET without an
	// AppIdentity structure it alerts the driver that it's being called by
	// the DSM and not by the application, most likely to bring up the user
	// selection dialog.  A driver should use this information to create --
	// and more importantly -- to destroy its internal data structures,
	// because it will get no other chance to clean itself up.
	//
	// It's worth interjecting at this point that Microsoft warns against
	// any but the most minimal activity in DllMain, so relying on doing
	// the create/destroy in there is very risky.  The same goes for the
	// __attribute(constructor)/__attribute(destructor) with GNU.
	//
	// The problem is that the DSM issues DG_CONTROL/DAT_IDENTITY/MSG_GET
	// just prior to DG_CONTROL/DAT_IDENTITY/MSG_OPEN.  If a driver is keyed
	// to the AppIdentity being NULL, it'll incorrectly clean itself up.
	//
	// This means we need to unload and reload the library, to give the
	// driver a consistent look.
	if (_boolKeepOpen == true)
	{
		pDSInfo->pHandle = (TW_HANDLE)LoadLibraryA(_pPath);		
#if (TWNDSM_CMP == TWNDSM_CMP_VISUALCPP)
		if (0 == pDSInfo->pHandle)
		{
			writeToLog("Could not load library: "+ std::string(_pPath));
			//AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
			return TWRC_FAILURE;
		}
#elif (TWNDSM_CMP == TWNDSM_CMP_GNUGPP)
		if (0 == pDSInfo->pHandle)
		{
			// This is a bit skanky, and not the sort of thing I really want
			// a user to have to see, but more info is better than less, so
			// hopefully someone will be able to sort out what the cryptic
			// message means and we can FAQ it...
			fprintf(stderr, ">>> error loading <%s>\r\n", _pPath);
			fprintf(stderr, ">>> %s\r\n", dlerror());
			fprintf(stderr, ">>> please contact your scanner or driver vendor for more\r\n");
			fprintf(stderr, ">>> help, if that doesn't help then check out the FAQ at\r\n");
			fprintf(stderr, ">>> http://www.twain.org\r\n");
			kLOG((kLOGERR, "Could not load library: %s", _pPath));
			kLOG((kLOGERR, dlerror()));
			AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
			return TWRC_FAILURE;
		}
#else
#error Sorry, we do not recognize this system...
#endif

		// Try to get the entry point...
	
#if (TWNDSM_CMP == TWNDSM_CMP_VISUALCPP)
			// The WIATwain.ds does not have an entry point 
			if (0 != strstr(_pPath, "wiatwain.ds"))
			{
				writeToLog("We're deliberately skipping this file: " + std::string(_pPath));
			}
			else
			{
				pDSInfo->DS_Entry = (DSENTRYPROC)GetProcAddress((HMODULE)pDSInfo->pHandle, MAKEINTRESOURCEA(1));

				if (pDSInfo->DS_Entry == 0)
				{
					writeToLog("Could not find Entry 1 in DS: %s" +std::string(_pPath));
				}
			}
#else
			kLOG((kLOGERR, "Could not find DS_Entry function in DS: %s", _pPath));
#endif
			if (pDSInfo->DS_Entry == 0)
			{
				(void)FreeLibrary((HMODULE)pDSInfo->pHandle);
				pDSInfo->pHandle = NULL;
				//AppSetConditionCode(_pAppId, TWCC_OPERATIONERROR);
				return TWRC_FAILURE;
			}
			//else if(pDSInfo->DS_Entry)
	}

	// All done...
	return result;
}


