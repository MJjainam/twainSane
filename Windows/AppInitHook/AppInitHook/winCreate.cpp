#pragma once
#include "winCreate.h"
#include "log.h"
#include <locale>
#include <codecvt>
#include <string>


retStruct ret;
extern HWND g_hwnd;
extern struct _pod pod;

std::wstring GetLastErrorAsString()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::wstring(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);
	std::wstring wide = converter.from_bytes(message);
	//Free the buffer.
	LocalFree(messageBuffer);

	return wide;
}


int ud_createWindow(HINSTANCE g_hinstance, HWND g_hwnd1, struct _pod pod1, retStruct ret1) {
	int result;
	writeToLog("Entered ud_createWindow");
	if (g_hinstance == 0) {
		writeToLog("The g_hisntance is null");
	}
	if (g_hwnd == 0) {
		writeToLog("the g_hwnd is null");
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
	}

	// Something back happened...
	else if (ret == -1)
	{
		writeToLog("in ret ==-1");

		
		std::wstring errMsg = GetLastErrorAsString();
		writeToLog("Error " + std::string(errMsg.begin(), errMsg.end()));
		MessageBox(NULL, (LPCWSTR)errMsg.c_str(),(LPCWSTR) L"Error", MB_OK | MB_ICONINFORMATION);
		//pod.m_ptwndsmapps->AppSetConditionCode(_pAppId, TWCC_BUMMER);
		result = TWRC_FAILURE;
	}
	writeToLog("returning result");
	std::wstring errMsg = GetLastErrorAsString();
	writeToLog("Error " + std::string(errMsg.begin(), errMsg.end()));
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

			for (x = 0; x < 1/*MAX_DEVICES*/; ++x)
			{
				// We expect the list to be contiguous...
				szProductName = ret.devInfo[x].name;
				
				if (!szProductName[0])
				{
					break;
				}
				// Display the name...
				nIndex = (int)SendMessage(hListBox, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)GetWC(szProductName));
				if (LB_ERR == nIndex)
				{
					break;
				}
				// Associate the id with the name...
				nIndex = (int)SendMessage(hListBox,
					LB_SETITEMDATA,
					(WPARAM)nIndex,
					(LPARAM)/*pPod->m_ptwndsmapps->DsGetIdentity(pAppId, x)->Id*/1);
				if (LB_ERR == nIndex)
				{
					break;
				}
				// Remember this item if it's the default...
				//if (!strcmp(pPod->m_ptwndsmapps->DsGetPath(pAppId, x), pPod->m_DefaultDSPath))
				//{
					nSelect = x;
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
					(LPARAM)ret.devInfo[0].name);
				if (LB_ERR == nIndex)
				{
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
						return TRUE;
					}
					nIndex = (int)SendMessage(hListBox, LB_GETITEMDATA, (WPARAM)nIndex, (LPARAM)0);
					if (LB_ERR != nIndex)
					{
						//pPod->m_pSelectDlgDsId = pod.m_ptwndsmapps->DsGetIdentity(pAppId, nIndex);
						writeToLog("something is wrong in winCreate.cpp");
					}
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
