#include "tools.h"

std::wstring convertToWideString(std::string s) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(s);
	return wide;
}
std::string convertToString(std::wstring ws) {

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string narrow = converter.to_bytes(ws);
	return narrow;

}

/*converts char* to wchar* */
const wchar_t *GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

std::wstring GetLastErrorAsString()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		//writeToLog("No ")
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

bool ConvertToDIB(HBITMAP& hBitmap)
{
	bool bConverted = false;
	BITMAP stBitmap;
	if (GetObject(hBitmap, sizeof(stBitmap),
		&stBitmap) && !stBitmap.bmBits)
	{
		// that is a DFB. Now we attempt to create
		// a DIB with the same sizes and pixel format.
		HDC hScreen = GetDC(NULL);
		if (hScreen)
		{
			union {
				BITMAPINFO stBitmapInfo;
				BYTE pReserveSpace[sizeof(BITMAPINFO)
					+ 0xFF * sizeof(RGBQUAD)];
			};
			ZeroMemory(pReserveSpace, sizeof(pReserveSpace));
			stBitmapInfo.bmiHeader.biSize = sizeof(stBitmapInfo.bmiHeader);
			stBitmapInfo.bmiHeader.biWidth = stBitmap.bmWidth;
			stBitmapInfo.bmiHeader.biHeight = stBitmap.bmHeight;
			stBitmapInfo.bmiHeader.biPlanes = 1;
			stBitmapInfo.bmiHeader.biBitCount = stBitmap.bmBitsPixel;
			stBitmapInfo.bmiHeader.biCompression = BI_RGB;

			if (stBitmap.bmBitsPixel <= 8)
			{
				stBitmapInfo.bmiHeader.biClrUsed =
					1 << stBitmap.bmBitsPixel;
				// This image is paletted-managed.
				// Hence we have to synthesize its palette.
			}
			stBitmapInfo.bmiHeader.biClrImportant =
				stBitmapInfo.bmiHeader.biClrUsed;

			PVOID pBits;
			HBITMAP hDib = CreateDIBSection(hScreen,
				&stBitmapInfo, DIB_RGB_COLORS, &pBits, NULL, 0);

			if (hDib)
			{
				// ok, we're lucky. Now we have
				// to transfer the image to the DFB.
				HDC hMemSrc = CreateCompatibleDC(NULL);
				if (hMemSrc)
				{
					HGDIOBJ hOldSrc = SelectObject(hMemSrc, hBitmap);
					if (hOldSrc)
					{
						HDC hMemDst = CreateCompatibleDC(NULL);
						if (hMemDst)
						{
							HGDIOBJ hOldDst = SelectObject(hMemDst, hDib);
							if (hOldDst)
							{
								if (stBitmap.bmBitsPixel <= 8)
								{
									// take the DFB's palette and set it to our DIB
									HPALETTE hPalette =
										(HPALETTE)GetCurrentObject(hMemSrc, OBJ_PAL);
									if (hPalette)
									{
										PALETTEENTRY pPaletteEntries[0x100];
										UINT nEntries = GetPaletteEntries(hPalette,
											0, stBitmapInfo.bmiHeader.biClrUsed,
											pPaletteEntries);
										if (nEntries)
										{
											if (!(nEntries <= 0x100)) {
												return false;
											}
											for (UINT nIndex = 0; nIndex < nEntries; nIndex++)
												pPaletteEntries[nEntries].peFlags = 0;
											if (!(SetDIBColorTable(hMemDst, 0,
												nEntries, (RGBQUAD*)pPaletteEntries) == nEntries)) {
												return false;
											}

										}
									}
								}

								// transfer the image using BitBlt function.
								// It will probably end in the
								// call to driver's DrvCopyBits function.
								if (BitBlt(hMemDst, 0, 0, stBitmap.bmWidth,
									stBitmap.bmHeight, hMemSrc, 0, 0, SRCCOPY))
									bConverted = true; // success

								if (!SelectObject(hMemDst, hOldDst)) {
									return false;
								}
							}
							if (!DeleteDC(hMemDst)) {
								return false;
							}

						}
						if (!SelectObject(hMemSrc, hOldSrc)) {
							return false;
						}
					}
					if (!DeleteDC(hMemSrc)) {
						return false;
					}
				}

				if (bConverted)
				{
					if (!DeleteObject(hBitmap)) {
						return false;
					} // it's no longer needed
					hBitmap = hDib;
				}
				else
					if (!DeleteObject(hDib)) {
						return false;
					}
			}
			ReleaseDC(NULL, hScreen);
		}
	}
	return bConverted;
}