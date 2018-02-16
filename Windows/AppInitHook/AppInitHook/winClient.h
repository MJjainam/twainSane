#ifndef WINCLIENT_H
#define WINCLIENT_H

#pragma once



//#include <stdio.h>
//#include <iostream>
//#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <tchar.h>
#include "twain.h"
#include "sane.h"
#define BUFSIZE 512
#include "log.h"
#include "structures.h"




//int connectTo(std::string buffer,char serialData[BUFSIZE], char ipAddress[], int portno);
void deserializeRetStruct(char *data, retStruct *pRet);
int connectTo(char ipAddress[], int portno);
int sendMsg(std::string buffer);
int sendfParams(fParameters *pmsgPacket);
int receiveMsg();
int receiveRetStruct(retStruct *pRet);
int closeSocket();

#endif