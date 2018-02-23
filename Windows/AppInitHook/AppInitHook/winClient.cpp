#pragma once

#include "winClient.h"
#define IPADDRESS "10.144.7.21"

#pragma comment(lib,"ws2_32.lib")
#define BUFSIZE 512
//#define errno WSAGetLastError()





SOCKET s;
WSADATA wsa;
struct sockaddr_in server;


void serializeScannerDevs(scannerDevs* pdevs, char *data) {

	TW_UINT16* pdevsX = (TW_UINT16*)pdevs;
	TW_UINT16* dataX = (TW_UINT16*)data;
	TW_UINT16 temp;
	for (int i = 0; i < sizeof(scannerDevs) / 2; i++)
	{

		temp = htons(*pdevsX);
		memcpy(dataX, &temp, 2);
		dataX++;
		pdevsX++;
	}

}




void serializeFParameters(fParameters* pmsgPacket, char *data) {

	TW_UINT16* p = (TW_UINT16*)pmsgPacket;
	TW_UINT16* dataX = (TW_UINT16*)data;
	TW_UINT16 temp;
	//memcpy(data, pmsgPacket, 320);
	for (int i = 0; i < sizeof(fParameters) / 2; i++) {

		temp = htons(*p);
		memcpy(dataX, &temp, 2);
		dataX++;
		p++;
	}


}

void deserializeRetStruct(char *data, retStruct *pRet)
{

	TW_UINT16 *p = (TW_UINT16 *)pRet;
	TW_UINT16 *dataX = (TW_UINT16 *)data;
	TW_UINT16 temp;
	//ntohl converts 4 bytes of data and so we copy 4 bytes at a time
	for (int i = 0; i < sizeof(retStruct) / 2; i++)
	{

		temp = ntohs(*dataX);
		memcpy(p, &temp, 2);
		p++; //Gets incremented by 2 bytes everytime as it is a int pointer
		dataX++;
	}

	//printf("CLIENT: The product family is: %s \n", &pmsgPacket->Origin.ProductFamily);
}

int connectTo(char ipAddress[], int portno) {

	try {

		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			//writeToLog("\nFailed. Error Code: \n");
			//return 1;

		}
	}
	catch (...) {
		writeToLog("Some weird problem in WSA startup");
		return 0;
	}
	
	writeToLog("INtialized");

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		writeToLog("Could not create socket:");
	}
	else {
		writeToLog("Socket created");
	}

	server.sin_addr.s_addr = inet_addr(ipAddress);

	//printf("%s",&server.sin_addr.s_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(portno);

	//Connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
		writeToLog("Connect error");
		return 1;
	}
	writeToLog("Connected");

		//send(s, buffer.c_str(), buffer.length()+1, 0);
		//send(s, serialData, BUFSIZE, 0);

	return 0;


}

int sendMsg(std::string buffer) {
	if (s == NULL) {
		connectTo(IPADDRESS, 2222);
	}
	send(s, buffer.c_str(), buffer.length() + 1, 0);
	return 0;
}
int sendfParams(fParameters *pmsgPacket) {
	if (s == NULL) {
		connectTo(IPADDRESS, 2222);
	}
	char data[sizeof(*pmsgPacket)];
	serializeFParameters(pmsgPacket, data);
	send(s, data, sizeof(data), 0);
	return 0;
}

int sendDevs(scannerDevs* pdevs) {-//Tw wrong one
	writeToLog("in sendDevs function");
	if (s == NULL) {
		connectTo(IPADDRESS, 2222);
	}
	//char* data = (char*)malloc(sizeof(scannerDevs));
	//writeToLog("size of data for malloc is " + std::to_string());
	char data[sizeof(*pdevs)];   //Malloc doesnt work here. See what could have gone wrong



	serializeScannerDevs(pdevs, data);
	int stat = send(s, data, sizeof(data), 0);
	if (stat == SOCKET_ERROR) {
		writeToLog("Connection problem while sending dev data....!!!!!");
	}

	return 0;
}

//int sendDevs(scannerDevs* pdevs) { //The correct one
//	writeToLog("in sendDevs function");
//	if (s == NULL) {
//		connectTo(IPADDRESS, 2222);
//	}
//	char data[sizeof(*pdevs)];
//
//	//char* data = (char*)malloc(sizeof(scannerDevs));
//	//memset(data, 0, sizeof(scannerDevs));
//
//	writeToLog("WINDOWS: Value of curDevId is : " + std::to_string(pdevs->curDevID));
//
//	serializeScannerDevs(pdevs, data);
//	int stat = send(s, data, sizeof(data), 0);
//	if (stat == SOCKET_ERROR) {
//		writeToLog("Connection problem while sending dev data....!!!!!");
//	}
//
//	return 0;
//}

int receiveMsg() {
	return 0;
}

int receiveRetStruct(retStruct *pRet) {
	if (s == NULL) {
		connectTo(IPADDRESS, 2222);
	}
	char* data = (char *)malloc(sizeof(retStruct));
	memset(data, sizeof(data), 0);
	recv(s, data, sizeof(retStruct),0);
	writeToLog("in receive Ret structure function\n");
	//writeToLog("size of unsigned long is: " + std::to_string(sizeof(unsigned long)));
	//retS *pRet;
	deserializeRetStruct(data, pRet);
	writeToLog("retStructure size is : " + std::to_string(sizeof(retStruct)));
	writeToLog("pret->message value: " + std::string(pRet->message));	
	return 0;
	//return *pRet;
}




int closeSocket() {

	if (s != NULL) {
		closesocket(s);

	}
	return 0;

}