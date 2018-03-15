#ifndef WINCREATE_H
#define WINCREATE_H



#include <Windows.h>
#include "twain.h"
#include "structures.h"
#include "tools.h"
#include <direct.h>

extern HWND twackerHandle; //handle of the application which is also declared in main
extern struct _pod pod;  //Pod stores the information related to the application
extern scannerDevs devs; 

 

#define LOADLIBRARY(lib,hook,DSID) InstallTwain32DllHooks(lib,hook,DSID)

int getWinDataSource(char *szProductName, DS_INFO* pwinDataSource);
int ud_createWindow(HINSTANCE g_hinstance, retStruct ret1);
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
BOOL SelectDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM /*lParam - unused*/);
int scanDSDir(char *szProductName, char *_szAbsPath, DS_INFO* pwinDataSource);
TW_UINT16 LoadDS(char *szProductName,char *_pPath, /* This is the path to data source*/ DS_INFO* pwinDataSource,bool         _boolKeepOpen);
void SaveBitmapToFile(BYTE* pBitmapBits, LONG lWidth, LONG lHeight, WORD wBitsPerPixel, LPCTSTR lpszFileName);


typedef struct
{
	TW_INT16  Language;
	BYTE      CharSet;
	LANGID    LangId;
	char      *Title;
	char      *Sources;
	char      *Select;
	char      *Cancel;      /**< the Cancel string */
} TwLocalize;

static TwLocalize s_twlocalize[] =
{
	{ TWLG_AFRIKAANS,          ANSI_CHARSET,       MAKELANGID(LANG_AFRIKAANS,SUBLANG_NEUTRAL),                 "Select Source","Sources:","Select","\x4b\x61\x6e\x73\x65\x6c\x6c\x65\x65\x72" },
{ TWLG_ALBANIA,            EASTEUROPE_CHARSET, MAKELANGID(LANG_ALBANIAN,SUBLANG_NEUTRAL),                  "","","","" },
{ TWLG_ARABIC,             ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_NEUTRAL),                    "","","","" },
{ TWLG_ARABIC_ALGERIA,     ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_ALGERIA),             "","","","" },
{ TWLG_ARABIC_BAHRAIN,     ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_BAHRAIN),             "","","","" },
{ TWLG_ARABIC_EGYPT,       ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_EGYPT),               "","","","" },
{ TWLG_ARABIC_IRAQ,        ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_IRAQ),                "","","","" },
{ TWLG_ARABIC_JORDAN,      ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_JORDAN),              "","","","" },
{ TWLG_ARABIC_KUWAIT,      ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_KUWAIT),              "","","","" },
{ TWLG_ARABIC_LEBANON,     ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_LEBANON),             "","","","" },
{ TWLG_ARABIC_LIBYA,       ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_LIBYA),               "","","","" },
{ TWLG_ARABIC_MOROCCO,     ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_MOROCCO),             "","","","" },
{ TWLG_ARABIC_OMAN,        ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_OMAN),                "","","","" },
{ TWLG_ARABIC_QATAR,       ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_QATAR),               "","","","" },
{ TWLG_ARABIC_SAUDIARABIA, ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_SAUDI_ARABIA),        "","","","" },
{ TWLG_ARABIC_SYRIA,       ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_SYRIA),               "","","","" },
{ TWLG_ARABIC_TUNISIA,     ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_TUNISIA),             "","","","" },
{ TWLG_ARABIC_UAE,         ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_UAE),                 "","","","" },
{ TWLG_ARABIC_YEMEN,       ARABIC_CHARSET,     MAKELANGID(LANG_ARABIC,SUBLANG_ARABIC_YEMEN),               "","","","" },
{ TWLG_ASSAMESE,           ANSI_CHARSET,       MAKELANGID(LANG_ASSAMESE,SUBLANG_NEUTRAL),                  "","","","" },
{ TWLG_BASQUE,             ANSI_CHARSET,       MAKELANGID(LANG_BASQUE,SUBLANG_NEUTRAL),                    "Select Source","Sources:","Select","\x55\x74\x7a\x69" },
{ TWLG_BENGALI,            ANSI_CHARSET,       MAKELANGID(LANG_BENGALI,SUBLANG_NEUTRAL),                   "","","","" },
{ TWLG_BIHARI,             0,                  0,                                                          "","","","" },
{ TWLG_BODO,               0,                  0,                                                          "","","","" },
{ TWLG_BULGARIAN,          RUSSIAN_CHARSET,    MAKELANGID(LANG_BULGARIAN,SUBLANG_NEUTRAL),                 "","","","" },
{ TWLG_BYELORUSSIAN,       RUSSIAN_CHARSET,    MAKELANGID(LANG_BELARUSIAN,SUBLANG_NEUTRAL),                "","","","" },
{ TWLG_CATALAN,            ANSI_CHARSET,       MAKELANGID(LANG_CATALAN,SUBLANG_NEUTRAL),                   "Select Source","Sources:","Select","\x43\x61\x6e\x63\x65\x6c\xc2\xb7\x6c\x61" },
{ TWLG_CHINESE,            GB2312_CHARSET,     MAKELANGID(LANG_CHINESE,SUBLANG_NEUTRAL),                   "\xe9\x80\x89\xe6\x8b\xa9\xe6\x95\xb0\xe6\x8d\xae\xe6\xba\x90","\xe6\x95\xb0\xe6\x8d\xae\xe6\xba\x90\x3a","\xe9\x80\x89\xe6\x8b\xa9","\xe5\x8f\x96\xe6\xb6\x88" },
{ TWLG_CHINESE_HONGKONG,   CHINESEBIG5_CHARSET,MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_HONGKONG),          "\xe9\x81\xb8\xe6\x93\x87\xe5\xbd\xb1\xe5\x83\x8f\xe4\xbe\x86\xe6\xba\x90","\xe5\xbd\xb1\xe5\x83\x8f\xe4\xbe\x86\xe6\xba\x90\x3a","\xe7\xa2\xba\xe5\xae\x9a","\xe5\x8f\x96\xe6\xb6\x88" },
{ TWLG_CHINESE_PRC,        GB2312_CHARSET,     MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),        "\xe9\x80\x89\xe6\x8b\xa9\xe6\x95\xb0\xe6\x8d\xae\xe6\xba\x90","\xe6\x95\xb0\xe6\x8d\xae\xe6\xba\x90\x3a","\xe9\x80\x89\xe6\x8b\xa9","\xe5\x8f\x96\xe6\xb6\x88" },
{ TWLG_CHINESE_SIMPLIFIED, GB2312_CHARSET,     MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),        "\xe9\x80\x89\xe6\x8b\xa9\xe6\x95\xb0\xe6\x8d\xae\xe6\xba\x90","\xe6\x95\xb0\xe6\x8d\xae\xe6\xba\x90\x3a","\xe9\x80\x89\xe6\x8b\xa9","\xe5\x8f\x96\xe6\xb6\x88" },
{ TWLG_CHINESE_SINGAPORE,  GB2312_CHARSET,     MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),        "\xe9\x80\x89\xe6\x8b\xa9\xe6\x95\xb0\xe6\x8d\xae\xe6\xba\x90","\xe6\x95\xb0\xe6\x8d\xae\xe6\xba\x90\x3a","\xe9\x80\x89\xe6\x8b\xa9","\xe5\x8f\x96\xe6\xb6\x88" },
{ TWLG_CHINESE_TAIWAN,     CHINESEBIG5_CHARSET,MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_TRADITIONAL),       "\xe9\x81\xb8\xe6\x93\x87\xe5\xbd\xb1\xe5\x83\x8f\xe4\xbe\x86\xe6\xba\x90","\xe5\xbd\xb1\xe5\x83\x8f\xe4\xbe\x86\xe6\xba\x90\x3a","\xe7\xa2\xba\xe5\xae\x9a","\xe5\x8f\x96\xe6\xb6\x88" },
{ TWLG_CHINESE_TRADITIONAL,CHINESEBIG5_CHARSET,MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_TRADITIONAL),       "\xe9\x81\xb8\xe6\x93\x87\xe5\xbd\xb1\xe5\x83\x8f\xe4\xbe\x86\xe6\xba\x90","\xe5\xbd\xb1\xe5\x83\x8f\xe4\xbe\x86\xe6\xba\x90\x3a","\xe7\xa2\xba\xe5\xae\x9a","\xe5\x8f\x96\xe6\xb6\x88" },
{ TWLG_CROATIA,            EASTEUROPE_CHARSET, MAKELANGID(LANG_CROATIAN,SUBLANG_NEUTRAL),                  "Select Source","Sources:","Select","\x4f\x64\x75\x73\x74\x61\x6e\x69" },
{ TWLG_CZECH,              EASTEUROPE_CHARSET, MAKELANGID(LANG_CZECH,SUBLANG_DEFAULT),                     "Select Source","Sources:","Select","\x53\x74\x6f\x72\x6e\x6f" },
{ TWLG_DANISH,             ANSI_CHARSET,       MAKELANGID(LANG_DANISH,SUBLANG_NEUTRAL),                    "\x56\xC3\xA6\x6C\x67\x20\x45\x6E\x68\x65\x64","\x45\x6E\x68\x65\x64","\x56\xE6\x6C\x67","\x41\x6E\x6E\x75\x6C\x6C\x65\x72" },
{ TWLG_DOGRI,              0,                  0,                                                          "","","","" },
{ TWLG_DUTCH,              ANSI_CHARSET,       MAKELANGID(LANG_DUTCH,SUBLANG_DUTCH),                       "\x53\x65\x6c\x65\x63\x74\x65\x65\x72\x20\x62\x72\x6f\x6e","\x42\x72\x6f\x6e\x6e\x65\x6e\x3a","\x53\x65\x6c\x65\x63\x74\x65\x72\x65\x6e","\x41\x6e\x6e\x75\x6c\x65\x72\x65\x6e" },
{ TWLG_DUTCH_BELGIAN,      ANSI_CHARSET,       MAKELANGID(LANG_DUTCH,SUBLANG_DUTCH_BELGIAN),               "\x53\x65\x6c\x65\x63\x74\x65\x65\x72\x20\x62\x72\x6f\x6e","\x42\x72\x6f\x6e\x6e\x65\x6e\x3a","\x53\x65\x6c\x65\x63\x74\x65\x72\x65\x6e","\x41\x6e\x6e\x75\x6c\x65\x72\x65\x6e" },
{ TWLG_ENGLISH,            ANSI_CHARSET,       MAKELANGID(LANG_ENGLISH,SUBLANG_NEUTRAL),                   "Select Source","Sources:","Select","Cancel" },
{ TWLG_ENGLISH_AUSTRALIAN, ANSI_CHARSET,       MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_AUS),               "Select Source","Sources:","Select","Cancel" },
{ TWLG_ENGLISH_CANADIAN,   ANSI_CHARSET,       MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_CAN),               "Select Source","Sources:","Select","Cancel" },
{ TWLG_ENGLISH_IRELAND,    ANSI_CHARSET,       MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_EIRE),              "Select Source","Sources:","Select","Cancel" },
{ TWLG_ENGLISH_NEWZEALAND, ANSI_CHARSET,       MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_NZ),                "Select Source","Sources:","Select","Cancel" },
{ TWLG_ENGLISH_SOUTHAFRICA,ANSI_CHARSET,       MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_SOUTH_AFRICA),      "Select Source","Sources:","Select","Cancel" },
{ TWLG_ENGLISH_UK,         ANSI_CHARSET,       MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_UK),                "Select Source","Sources:","Select","Cancel" },
{ TWLG_ENGLISH_USA,        ANSI_CHARSET,       MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),                "Select Source","Sources:","Select","Cancel" },
{ TWLG_ESTONIAN,           BALTIC_CHARSET,     MAKELANGID(LANG_ESTONIAN,SUBLANG_NEUTRAL),                  "Select Source","Sources:","Select","\x4b\x75\x73\x74\x75\x74\x61" },
{ TWLG_FAEROESE,           EASTEUROPE_CHARSET, MAKELANGID(LANG_FAEROESE,SUBLANG_NEUTRAL),                  "","","","" },
{ TWLG_FARSI,              ARABIC_CHARSET,     MAKELANGID(LANG_FARSI,SUBLANG_NEUTRAL),                     "","","","" },
{ TWLG_FINNISH,            ANSI_CHARSET,       MAKELANGID(LANG_FINNISH,SUBLANG_NEUTRAL),                   "Select Source","Sources:","Select","\x50\x65\x72\x75\x75\x74\x61" },
{ TWLG_FRENCH,             ANSI_CHARSET,       MAKELANGID(LANG_FRENCH,SUBLANG_FRENCH),                     "\x53\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72\x20\x73\x6f\x75\x72\x63\x65","\x53\x6f\x75\x72\x63\x65\x73\x3a","\x53\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72","\x41\x6e\x6e\x75\x6c\x65\x72" },
{ TWLG_FRENCH_BELGIAN,     ANSI_CHARSET,       MAKELANGID(LANG_FRENCH,SUBLANG_FRENCH_BELGIAN),             "\x53\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72\x20\x73\x6f\x75\x72\x63\x65","\x53\x6f\x75\x72\x63\x65\x73\x3a","\x53\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72","\x41\x6e\x6e\x75\x6c\x65\x72" },
{ TWLG_FRENCH_CANADIAN,    ANSI_CHARSET,       MAKELANGID(LANG_FRENCH,SUBLANG_FRENCH_CANADIAN),            "\x53\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72\x20\x73\x6f\x75\x72\x63\x65","\x53\x6f\x75\x72\x63\x65\x73\x3a","\x53\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72","\x41\x6e\x6e\x75\x6c\x65\x72" },
{ TWLG_FRENCH_LUXEMBOURG,  ANSI_CHARSET,       MAKELANGID(LANG_FRENCH,SUBLANG_FRENCH_LUXEMBOURG),          "\x53\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72\x20\x73\x6f\x75\x72\x63\x65","\x53\x6f\x75\x72\x63\x65\x73\x3a","\x53\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72","\x41\x6e\x6e\x75\x6c\x65\x72" },
{ TWLG_FRENCH_SWISS,       ANSI_CHARSET,       MAKELANGID(LANG_FRENCH,SUBLANG_FRENCH_SWISS),               "\x53\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72\x20\x73\x6f\x75\x72\x63\x65","\x53\x6f\x75\x72\x63\x65\x73\x3a","\x53\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72","\x41\x6e\x6e\x75\x6c\x65\x72" },
{ TWLG_GERMAN,             ANSI_CHARSET,       MAKELANGID(LANG_GERMAN,SUBLANG_GERMAN),                     "\x51\x75\x65\x6c\x6c\x65\x20\x77\xc3\xa4\x68\x6c\x65\x6e","\x51\x75\x65\x6c\x6c\x65\x6e\x3a","\x41\x75\x73\x77\xc3\xa4\x68\x6c\x65\x6e","\x41\x62\x62\x72\x65\x63\x68\x65\x6e" },
{ TWLG_GERMAN_AUSTRIAN,    ANSI_CHARSET,       MAKELANGID(LANG_GERMAN,SUBLANG_GERMAN_AUSTRIAN),            "\x51\x75\x65\x6c\x6c\x65\x20\x77\xc3\xa4\x68\x6c\x65\x6e","\x51\x75\x65\x6c\x6c\x65\x6e\x3a","\x41\x75\x73\x77\xc3\xa4\x68\x6c\x65\x6e","\x41\x62\x62\x72\x65\x63\x68\x65\x6e" },
{ TWLG_GERMAN_LIECHTENSTEIN,ANSI_CHARSET,      MAKELANGID(LANG_GERMAN,SUBLANG_GERMAN_LIECHTENSTEIN),       "\x51\x75\x65\x6c\x6c\x65\x20\x77\xc3\xa4\x68\x6c\x65\x6e","\x51\x75\x65\x6c\x6c\x65\x6e\x3a","\x41\x75\x73\x77\xc3\xa4\x68\x6c\x65\x6e","\x41\x62\x62\x72\x65\x63\x68\x65\x6e" },
{ TWLG_GERMAN_LUXEMBOURG,  ANSI_CHARSET,       MAKELANGID(LANG_GERMAN,SUBLANG_GERMAN_LUXEMBOURG),          "\x51\x75\x65\x6c\x6c\x65\x20\x77\xc3\xa4\x68\x6c\x65\x6e","\x51\x75\x65\x6c\x6c\x65\x6e\x3a","\x41\x75\x73\x77\xc3\xa4\x68\x6c\x65\x6e","\x41\x62\x62\x72\x65\x63\x68\x65\x6e" },
{ TWLG_GERMAN_SWISS,       ANSI_CHARSET,       MAKELANGID(LANG_GERMAN,SUBLANG_GERMAN_SWISS),               "\x51\x75\x65\x6c\x6c\x65\x20\x77\xc3\xa4\x68\x6c\x65\x6e","\x51\x75\x65\x6c\x6c\x65\x6e\x3a","\x41\x75\x73\x77\xc3\xa4\x68\x6c\x65\x6e","\x41\x62\x62\x72\x65\x63\x68\x65\x6e" },
{ TWLG_GREEK,              GREEK_CHARSET,      MAKELANGID(LANG_GREEK,SUBLANG_DEFAULT),                     "","","","" },
{ TWLG_GUJARATI,           GREEK_CHARSET,      MAKELANGID(LANG_GUJARATI,SUBLANG_DEFAULT),                  "","","","" },
{ TWLG_HARYANVI,           0,                  0,                                                          "","","","" },
{ TWLG_HEBREW,             HEBREW_CHARSET,     MAKELANGID(LANG_HEBREW,SUBLANG_NEUTRAL),                    "","","","" },
{ TWLG_HINDI,              HEBREW_CHARSET,     MAKELANGID(LANG_HINDI,SUBLANG_NEUTRAL),                     "","","","" },
{ TWLG_HUNGARIAN,          EASTEUROPE_CHARSET, MAKELANGID(LANG_HUNGARIAN,SUBLANG_NEUTRAL),                 "Select Source","Sources:","Select","\x4d\xc3\xa9\x67\x73\x65" },
{ TWLG_ICELANDIC,          ANSI_CHARSET,       MAKELANGID(LANG_ICELANDIC,SUBLANG_NEUTRAL),                 "","","","" },
{ TWLG_INDONESIAN,         ANSI_CHARSET,       MAKELANGID(LANG_INDONESIAN,SUBLANG_NEUTRAL),                "\x50\x69\x6c\x69\x74\x68\x20\x53\x75\x6d\x62\x65\x72","\x53\x75\x6d\x62\x65\x72\x3a","\x50\x69\x6c\x69\x74\x68","\x42\x61\x74\x61\x6c" },
{ TWLG_ITALIAN,            ANSI_CHARSET,       MAKELANGID(LANG_ITALIAN,SUBLANG_ITALIAN),                   "\x53\x65\x6c\x65\x7a\x69\x6f\x6e\x61\x20\x6f\x72\x69\x67\x69\x6e\x65","\x4f\x72\x69\x67\x69\x6e\x69\x3a","\x53\x65\x6c\x65\x7a\x69\x6f\x6e\x61","\x41\x6e\x6e\x75\x6c\x6c\x61" },
{ TWLG_ITALIAN_SWISS,      ANSI_CHARSET,       MAKELANGID(LANG_ITALIAN,SUBLANG_ITALIAN_SWISS),             "\x53\x65\x6c\x65\x7a\x69\x6f\x6e\x61\x20\x6f\x72\x69\x67\x69\x6e\x65","\x4f\x72\x69\x67\x69\x6e\x69\x3a","\x53\x65\x6c\x65\x7a\x69\x6f\x6e\x61","\x41\x6e\x6e\x75\x6c\x6c\x61" },
{ TWLG_JAPANESE,           SHIFTJIS_CHARSET,   MAKELANGID(LANG_JAPANESE,SUBLANG_DEFAULT),                  "\xe5\x8e\x9f\xe7\xa8\xbf\xe3\x81\xae\xe9\x81\xb8\xe6\x8a\x9e","\xe5\x8e\x9f\xe7\xa8\xbf\x3a","\xe9\x81\xb8\xe6\x8a\x9e","\xe3\x82\xad\xe3\x83\xa3\xe3\x83\xb3\xe3\x82\xbb\xe3\x83\xab" },
{ TWLG_KANNADA,            ANSI_CHARSET,       MAKELANGID(LANG_KANNADA,SUBLANG_NEUTRAL),                   "","","","" },
{ TWLG_KASHMIRI,           ANSI_CHARSET,       MAKELANGID(LANG_KASHMIRI,SUBLANG_NEUTRAL),                  "","","","" },
{ TWLG_KOREAN,             HANGUL_CHARSET,     MAKELANGID(LANG_KOREAN,SUBLANG_KOREAN),                     "\xec\x9e\xa5\xec\xb9\x98\x20\xec\x84\xa0\xed\x83\x9d","\xec\x9e\xa5\xec\xb9\x98","\xec\x84\xa0\xed\x83\x9d","\xec\xb7\xa8\xec\x86\x8c" },
{ TWLG_KOREAN_JOHAB,       JOHAB_CHARSET,      MAKELANGID(LANG_KOREAN,SUBLANG_KOREAN),                     "\xec\x9e\xa5\xec\xb9\x98\x20\xec\x84\xa0\xed\x83\x9d","\xec\x9e\xa5\xec\xb9\x98","\xec\x84\xa0\xed\x83\x9d","\xec\xb7\xa8\xec\x86\x8c" },
{ TWLG_LATVIAN,            BALTIC_CHARSET,     MAKELANGID(LANG_LATVIAN,SUBLANG_NEUTRAL),                   "","","","" },
{ TWLG_LITHUANIAN,         BALTIC_CHARSET,     MAKELANGID(LANG_LITHUANIAN,SUBLANG_NEUTRAL),                "","","","" },
{ TWLG_MALAYALAM,          BALTIC_CHARSET,     MAKELANGID(LANG_MALAYALAM,SUBLANG_NEUTRAL),                 "","","","" },
{ TWLG_MARATHI,            ANSI_CHARSET,       MAKELANGID(LANG_MARATHI,SUBLANG_NEUTRAL),                   "","","","" },
{ TWLG_MARWARI,            0,                  0,                                                          "","","","" },
{ TWLG_MEGHALAYAN,         0,                  0,                                                          "","","","" },
{ TWLG_MIZO,               0,                  0,                                                          "","","","" },
{ TWLG_NAGA,               0,                  0,                                                          "","","","" },
{ TWLG_NORWEGIAN,          ANSI_CHARSET,       MAKELANGID(LANG_NORWEGIAN,SUBLANG_NEUTRAL),                 "","","","" },
{ TWLG_NORWEGIAN_BOKMAL,   ANSI_CHARSET,       MAKELANGID(LANG_NORWEGIAN,SUBLANG_NORWEGIAN_BOKMAL),        "","","","" },
{ TWLG_NORWEGIAN_NYNORSK,  ANSI_CHARSET,       MAKELANGID(LANG_NORWEGIAN,SUBLANG_NORWEGIAN_NYNORSK),       "","","","" },
{ TWLG_ORISSI,             0,                  0,                                                          "","","","" },
{ TWLG_POLISH,             EASTEUROPE_CHARSET, MAKELANGID(LANG_POLISH,SUBLANG_NEUTRAL),                    "Select Source","Sources:","Select","\x41\x6e\x75\x6c\x75\x6a" },
{ TWLG_PORTUGUESE,         EASTEUROPE_CHARSET, MAKELANGID(LANG_PORTUGUESE,SUBLANG_PORTUGUESE),             "\x53\x65\x6c\x65\x63\x69\x6f\x6e\x61\x72\x20\x4f\x72\x69\x67\x65\x6d","\x4f\x72\x69\x67\x65\x6e\x73\x3a","\x53\x65\x6c\x65\x63\x69\x6f\x6e\x61\x72","\x43\x61\x6e\x63\x65\x6c\x61\x72" },
{ TWLG_PORTUGUESE_BRAZIL,  ANSI_CHARSET,       MAKELANGID(LANG_PORTUGUESE,SUBLANG_PORTUGUESE_BRAZILIAN),   "\x53\x65\x6c\x65\x63\x69\x6f\x6e\x61\x72\x20\x4f\x72\x69\x67\x65\x6d","\x4f\x72\x69\x67\x65\x6e\x73\x3a","\x53\x65\x6c\x65\x63\x69\x6f\x6e\x61\x72","\x43\x61\x6e\x63\x65\x6c\x61\x72" },
{ TWLG_PUNJABI,            ANSI_CHARSET,       MAKELANGID(LANG_PUNJABI,SUBLANG_NEUTRAL),                   "","","","" },
{ TWLG_PUSHTU,             0,                  0,                                                          "","","","" },
{ TWLG_ROMANIAN,           EASTEUROPE_CHARSET, MAKELANGID(LANG_ROMANIAN,SUBLANG_NEUTRAL),                  "","","","" },
{ TWLG_RUSSIAN,            RUSSIAN_CHARSET,    MAKELANGID(LANG_RUSSIAN,SUBLANG_DEFAULT),                   "\xd0\x92\xd1\x8b\xd0\xb1\xd1\x80\xd0\xb0\xd1\x82\xd1\x8c\x20\xd0\xb8\xd1\x81\xd1\x82\xd0\xbe\xd1\x87\xd0\xbd\xd0\xb8\xd0\xba","\xd0\x98\xd1\x81\xd1\x82\xd0\xbe\xd1\x87\xd0\xbd\xd0\xb8\xd0\xba\xd0\xb8\x3a","\xd0\x92\xd1\x8b\xd0\xb1\xd1\x80\xd0\xb0\xd1\x82\xd1\x8c","\xd0\x9e\xd1\x82\xd0\xbc\xd0\xb5\xd0\xbd\xd0\xb8\xd1\x82\xd1\x8c" },
{ TWLG_SERBIAN_CYRILLIC,   ANSI_CHARSET,       MAKELANGID(LANG_SERBIAN,SUBLANG_SERBIAN_CYRILLIC),          "","","","" },
{ TWLG_SERBIAN_LATIN,      EASTEUROPE_CHARSET, MAKELANGID(LANG_SERBIAN,SUBLANG_SERBIAN_LATIN),             "","","","" },
{ TWLG_SIKKIMI,            0,                  0,                                                          "","","","" },
{ TWLG_SLOVAK,             EASTEUROPE_CHARSET, MAKELANGID(LANG_SLOVAK,SUBLANG_NEUTRAL),                    "","","","" },
{ TWLG_SLOVENIAN,          EASTEUROPE_CHARSET, MAKELANGID(LANG_SLOVENIAN,SUBLANG_NEUTRAL),                 "Select Source","Sources:","Select","\x50\x72\x65\x6b\x69\x6e\x69" },
{ TWLG_SPANISH,            ANSI_CHARSET,       MAKELANGID(LANG_SPANISH,SUBLANG_SPANISH),                   "\x53\x65\x6c\x65\x63\x63\x69\xc3\xb3\x6e\x20\x64\x65\x20\x66\x75\x65\x6e\x74\x65","\x46\x75\x65\x6e\x74\x65\x73\x3a","\x53\x65\x6c\x65\x63\x63\x69\x6f\x6e\x61\x72","\x43\x61\x6e\x63\x65\x6c\x61\x72" },
{ TWLG_SPANISH_MEXICAN,    ANSI_CHARSET,       MAKELANGID(LANG_SPANISH,SUBLANG_SPANISH_MEXICAN),           "\x53\x65\x6c\x65\x63\x63\x69\xc3\xb3\x6e\x20\x64\x65\x20\x66\x75\x65\x6e\x74\x65","\x46\x75\x65\x6e\x74\x65\x73\x3a","\x53\x65\x6c\x65\x63\x63\x69\x6f\x6e\x61\x72","\x43\x61\x6e\x63\x65\x6c\x61\x72" },
{ TWLG_SPANISH_MODERN,     ANSI_CHARSET,       MAKELANGID(LANG_SPANISH,SUBLANG_SPANISH_MODERN),            "\x53\x65\x6c\x65\x63\x63\x69\xc3\xb3\x6e\x20\x64\x65\x20\x66\x75\x65\x6e\x74\x65","\x46\x75\x65\x6e\x74\x65\x73\x3a","\x53\x65\x6c\x65\x63\x63\x69\x6f\x6e\x61\x72","\x43\x61\x6e\x63\x65\x6c\x61\x72" },
{ TWLG_SWEDISH,            ANSI_CHARSET,       MAKELANGID(LANG_SWEDISH,SUBLANG_SWEDISH),                   "Select Source","Sources:","Select","\x41\x76\x62\x72\x79\x74" },
{ TWLG_SWEDISH_FINLAND,    ANSI_CHARSET,       MAKELANGID(LANG_SWEDISH,SUBLANG_SWEDISH_FINLAND),           "Select Source","Sources:","Select","\x41\x76\x62\x72\x79\x74" },
{ TWLG_TAMIL,              ANSI_CHARSET,       MAKELANGID(LANG_TAMIL,SUBLANG_NEUTRAL),                     "","","","" },
{ TWLG_TELUGU,             ANSI_CHARSET,       MAKELANGID(LANG_TELUGU,SUBLANG_NEUTRAL),                    "","","","" },
{ TWLG_THAI,               THAI_CHARSET,       MAKELANGID(LANG_THAI,SUBLANG_NEUTRAL),                      "","","","" },
{ TWLG_TRIPURI,            0,                  0,                                                          "","","","" },
{ TWLG_TURKISH,            TURKISH_CHARSET,    MAKELANGID(LANG_TURKISH,SUBLANG_DEFAULT),                   "\x4b\x61\x79\x6e\x61\x6b\x20\x73\x65\xc3\xa7\x69\x6e\x69\x7a","\x4b\x61\x79\x6e\x61\x6b","\x53\x65\xc3\xa7\x69\x6e\x69\x7a","\xc4\xb0\x70\x74\x61\x6c" },
{ TWLG_UKRANIAN,           RUSSIAN_CHARSET,    MAKELANGID(LANG_UKRAINIAN,SUBLANG_NEUTRAL),                 "","","","" },
{ TWLG_URDU,               ANSI_CHARSET,       MAKELANGID(LANG_URDU,SUBLANG_NEUTRAL),                      "","","","" },
{ TWLG_VIETNAMESE,         VIETNAMESE_CHARSET, MAKELANGID(LANG_VIETNAMESE,SUBLANG_NEUTRAL),                "","","","" },
{ -1, 0, 0, 0, 0, 0, 0 } // must be last...
};



#endif

