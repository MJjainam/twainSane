
#ifndef LOG_H
#define LOG_H

#include "stdafx.h"
#include <cstdio>
#include <stdio.h>


#include <tchar.h>
#include <string>

//int writeToLog(char* buffer);


static HANDLE hLog = (void*)0;
static int count =0;

int writeToLog(std::string buffer);
int closeLogFile();


#endif