#pragma once
#include "winCreate.h"
#include "log.h"
#include <locale>
#include <codecvt>
#include <string>
#include "tools.h"

retStruct ret;




//DS_INFO winDataSources[MAX_DEVICES];



int ud_createWindow(HINSTANCE g_hinstance, retStruct ret1) {
	int result;
	writeToLog("Entered ud_createWindow");
	if (g_hinstance == 0) {
		writeToLog("The g_hisntance is null");
		//return TW;
	}
	if (twackerHandle == 0) {
		writeToLog("the g_hwnd is null");
		//return -1;
	}

	ret = ret1;


	int ret = DialogBoxW(g_hinstance,
		(LPCWSTR)MAKEINTRESOURCE(IDD_DLG_SOURCE),
		(HWND)twackerHandle,
		(DLGPROC)SelectDlgProc);
	result = TWRC_SUCCESS;
	if (ret == IDOK)
	{

		writeToLog("in ret == IDOK");
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


				// Display the name...
				nIndex = (int)SendMessage(hListBox, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)GetWC(szProductName));
				if (LB_ERR == nIndex)
				{
					writeToLog("Could not display the name");
					break;
				}
				
				// Associate the id with the name...
				//I am assosiating the index of DS with the name
				nIndex = (int)SendMessage(hListBox,
					LB_SETITEMDATA,
					(WPARAM)nIndex,
					(LPARAM)x);
				if (LB_ERR == nIndex)
				{
					writeToLog("Could not assosiate ID with the name");
					break;
				}
				
			}
			
			//Default scanner selected is always the first scanner in the hlistbox
			nIndex = (int)SendMessage(hListBox,
				LB_FINDSTRINGEXACT,
				(WPARAM)-1,
				(LPARAM)GetWC(ret.devs.devinfo[0].name));
			if (LB_ERR == nIndex)
			{
				writeToLog("Could not get the default DS by product name");
				nIndex = 0;
			}

			//Select the nIndex'th device as default (i.e 1st scanner)
			SendMessage(hListBox, LB_SETCURSEL, (WPARAM)nIndex, (LPARAM)NULL); 
		
		}

		// Center our dialog on the window reported to us in MSG_OPENDS...
		hParent = twackerHandle;
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
					nIndex = (int)SendMessage(hListBox, LB_GETITEMDATA, (WPARAM)nIndex, (LPARAM)0);

					
					if (LB_ERR != nIndex)
					{
						//Store the selected index in devs.curDevID
						devs.curDevID = nIndex;
						writeToLog("initialized devs.curDevID to: " + std::to_string(nIndex));
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

//

void SaveBitmapToFile(BYTE* pBitmapBits, LONG lWidth, LONG lHeight, WORD wBitsPerPixel, LPCTSTR lpszFileName)
{

	writeToLog("Entered SaveBitmapToFile function");
	const int PALSIZE = 256;
	
	/*RGBQUAD palette[PALSIZE];
	for (int i = 0; i < PALSIZE; ++i)
	{
		palette[i].rgbBlue = (byte)0;
		palette[i].rgbGreen = (byte)0;
		palette[i].rgbRed = (byte)0;
	}*/

	BITMAPINFOHEADER bmpInfoHeader = { 0 };
	// Set the size
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	// Bit count
	bmpInfoHeader.biBitCount = wBitsPerPixel;
	// Use all colors
	bmpInfoHeader.biClrImportant = 0;
	// Use as many colors according to bits per pixel
	bmpInfoHeader.biClrUsed = 0;
	// Store as un Compressed
	bmpInfoHeader.biCompression = BI_RGB;
	// Set the height in pixels
	bmpInfoHeader.biHeight = -lHeight;
	// Width of the Image in pixels
	bmpInfoHeader.biWidth = lWidth;
	// Default number of planes
	bmpInfoHeader.biPlanes = 1;
	// Calculate the image size in bytes
	bmpInfoHeader.biSizeImage = lWidth * lHeight * (wBitsPerPixel) / 8;

	BITMAPFILEHEADER bfh = { 0 };
	// This value should be values of BM letters i.e 0x4D42
	// 0x4D = M 0×42 = B storing in reverse order to match with endian

	bfh.bfType = 'B' + ('M' << 8);
	// <<8 used to shift ‘M’ to end

	// Offset to the RGBQUAD
	bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);// +sizeof(RGBQUAD) * PALSIZE;
	// Total size of image including size of headers
	bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage;
	// Create the file in disk to write
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!hFile) // return if error opening file
	{
		writeToLog("Error opening the file");
		std::wstring errMsg = GetLastErrorAsString();
		writeToLog("Error ::" + std::string(errMsg.begin(), errMsg.end()));
		return;
	}
	else {
		writeToLog("No error while opening the file");
	}

	DWORD dwWritten = 0;
	// Write the File header
	WriteFile(hFile, &bfh, sizeof(bfh), &dwWritten, NULL);
	// Write the bitmap info header
	WriteFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwWritten, NULL);
	 //Write the palette
	//WriteFile(hFile, &palette[0], sizeof(RGBQUAD) * PALSIZE, &dwWritten, NULL);
	// Write the RGB Data
	if (lWidth % 4 == 0)
	{
		writeToLog("Inside lWidth%4==0");
		WriteFile(hFile, pBitmapBits, bmpInfoHeader.biSizeImage, &dwWritten, NULL);
	}
	else
	{
		writeToLog("Inside else lWidth%4==0");

		char* empty = new char[4 - lWidth % 4];
		for (int i = 0; i < lHeight; ++i)
		{
			WriteFile(hFile, &pBitmapBits[i * lWidth], lWidth, &dwWritten, NULL);
			WriteFile(hFile, empty, 4 - lWidth % 4, &dwWritten, NULL);
		}
	}

	// Close the file handle
	writeToLog("Done writing to bmp file");
	CloseHandle(hFile);
}


