#ifndef STRUCTURES_H
#define STRUCTURES_H
#include "twain.h"
#include "defines.h"

typedef struct fParameters {
	TW_IDENTITY Origin;
	TW_IDENTITY Dest;
	TW_UINT32 DG;
	TW_UINT16 DAT;
	TW_UINT16 MSG;
	//TW_MEMREF pData;
}fParameters;

typedef struct
{
  TW_IDENTITY   Identity;               /* Identity info for data source */
  TW_HANDLE     pHandle;                /* returned by LOADLIBRARY(...) */
  DSENTRYPROC   DS_Entry;               /* function pointer to the DS_Entry function -- set by dlsym(...) */
  char          szPath[FILENAME_MAX];   /* location of the DS */
  TW_CALLBACK2  twcallback2;            /* callback structure (we're using callback2 because it's 32-bit and 64-bit safe) */
  TW_BOOL       bCallbackPending;       /* True if an application is old style and a callback was supposed to be made to it */
  TW_BOOL       bDSProcessingMessage;   /* True if the application is still waiting for the DS to return from processing a message */
  TW_BOOL       bAppProcessingCallback; /* True if the application is still waiting for the DS to return from processing a message */
} DS_INFO;

typedef struct devInfoStruct {
  char name[64];
  char vendor[64];
  char model[64];
  char type[64];
  //DS_INFO winDataSource;

} devInfoStruct;

typedef struct scannerDevs{
	devInfoStruct devinfo[MAX_DEVICES];
	int numOfDevices = 0;
	int curDevID = 0;
}scannerDevs;

typedef struct retStruct
{
	TW_UINT16 twrc; //Error code that has to be returned
	char message[MAX_MESSAGE_SIZE];
//	devInfoStruct devInfo[MAX_DEVICES];
	scannerDevs devs; //This is a structure which contains list of scanner devices found and the number of them.

} retStruct;

struct _pod
{

	/*The class takes care of our list of applications and drivers.
	*/
	//	CTwnDsmApps * m_ptwndsmapps;

	/*The path to the default DS.The Default DS is identified when
	* the DSM is opened.A new Default is saved if SelectDlg is used.
	* So this value will be compared against DsGetPath()...
	*/
	char m_DefaultDSPath[FILENAME_MAX];

	/*The next id to test for GetFirst / GetNext...
	*/
	TWID_T m_nextDsId;

	/*The DS ID we end up with from SelectDlgProc.This is only
	* used on the Windows platform.
	*/
	TW_IDENTITY * m_pSelectDlgDsId;

	/*The Application ID we're using inside of SelectDlgProc. This
	* is only used on the Windows platform.
	*/
	TW_IDENTITY * m_pSelectDlgAppId;
	int devId = 0;
	int numOfDevices;
}; 


	

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.



#endif