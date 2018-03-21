#ifndef LINSERVER_H
#define LINSERVER_H

#include "structures.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <errno.h>
#include <sane/sane.h>
#include <assert.h>




void serializeRetStruct(retStruct *pRet, char *data);
void deserializeRetStruct(char *data, retStruct *pRet);
void serializeFParameters(fParameters *pmsgPacket, char *data);
void deserializeFParameters(char *data, fParameters *pmsgPacket);
//char* recvMsg();
int recvMsg(char* message);
int recvDevs(scannerDevs *pdevs);
fParameters recvfParams();
int sendRetStruct(retStruct* pRet);
int connectTo(const char* a);
int closeSocket();
// int recvDevs(scannerDevs *pdevs);
scannerDevs recvDevs();
void deserializeScannerDevs(char *data, scannerDevs *pdevs);
int sendImage(char* path);
int sendImageStrip(char* buffer, int bufLen);
int send_r(int sock, void* buf, int totalBytes, int flags);
int sendSaneParams();


#endif