#pragma once
#ifndef STRUCUTRES_H
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

typedef struct devInfoStruct {
	char name[64];
	char vendor[64];
	char model[64];
	char type[64];

} devInfoStruct;

typedef struct retStruct
{
	TW_UINT16 twrc;
	char message[MAX_MESSAGE_SIZE];
	devInfoStruct devInfo[MAX_DEVICES];
	int numOfDevices = 0;
	//char*  devInfo[MAX_DEVICES];


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

	int numOfDevices;
}; 


	

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.



#endif