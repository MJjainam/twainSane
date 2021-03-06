#pragma once
#ifndef TOOLS_H
#define TOOLS_H
#include <string>
#include <locale>
#include <codecvt>
#include <Windows.h>
//#include <afx.h>



std::wstring convertToWideString(std::string s);
std::string convertToString(std::wstring ws);
const wchar_t *GetWC(const char *c);
std::wstring GetLastErrorAsString();
bool ConvertToDIB(HBITMAP& hBitmap);

#endif