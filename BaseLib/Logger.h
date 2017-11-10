#ifndef _LOGGER_H
#define _LOGGER_H

#pragma once

#include "Base.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
//<<<<<<< HEAD
#include <fstream>
//=======
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")
//>>>>>>> d23e846e5701bea5d1f3006f0d9cdca96f9308a4

#define	BLACK 0
#define BLUE 1
#define GREEN 2
#define	CYAN 3
#define	RED 4
#define	MAGENTA 5
#define BROWN 6
#define	LIGHTGRAY 7
#define	DARKGRAY 8
#define	LIGHTBLUE 9
#define	LIGHTGREEN 10
#define	LIGHTCYAN 11
#define	LIGHTRED 12
#define	LIGHTMAGENTA 13
#define	YELLOW 14
#define	WHITE 15

#define CONSOLECOLOR(p) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), p);
#define MAKEDIR(p) CreateFolder(p);

void CreateFolder(const char * path);

namespace Logger
{
	void Log(char* szText, ...);
	//<<<<<<< HEAD
	void SavePacket(BYTE* pData);
	void SavePacket(BYTE* pData, const char* packetName);
	//=======
	void StartLog();
	//>>>>>>> d23e846e5701bea5d1f3006f0d9cdca96f9308a4
};

#endif