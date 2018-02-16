#include "log.h"



//static HANDLE hLog = (void *)0;
//static int count = 0;

	//printf("%d",a);



int writeToLog(std::string buffer) {

	if (hLog == (void*)0) {
		//SYSTEMTIME systemTime;
		//GetSystemTime(&systemTime);
		//std::cout << systemTime.wHour;
		//std::cout << "\n";
		//std::cout << systemTime.wMinute;
		//std::cout << "\n";
		//std::cout << systemTime.wSecond;
		//std::string filePath = "C:\\Users\\Jai\\Downloads\\AppInitHook\\AppInitHook\\Debug\\" + systemTime.wHour + systemTime.wMinute + systemTime.wSecond + (std::string)(".log");
		//std::wstring(filePath.begin(), filePath.end()).c_str()
		hLog = CreateFile(_T("C:\\Users\\Jai\\Downloads\\AppInitHook\\AppInitHook\\Debug\\twainHook.log"),
			FILE_APPEND_DATA,
			FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

	}
	DWORD bytesWritten ;
	buffer = std::to_string(count++) +". " + buffer +  "\n";
	BOOL errFlag = WriteFile(hLog, buffer.c_str(), buffer.length(), &bytesWritten, NULL);
	if (errFlag == FALSE) {

		return -1;
	}
	else {
		if (buffer.length() != bytesWritten) {
			return -1;
		}
		else {

			return 0;
		}
	}
}

int closeLogFile() {
	if (hLog != (void*)0) {
		writeToLog("Closing File handle");
		CloseHandle(hLog);
		count = 0;
	}
	return 0;
}








