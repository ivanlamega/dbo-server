#include "stdafx.h"
#include "Logger.h"
#include "Packet.h"

void Logger::StartLog()
{
	time_t rawtime;
	tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	timeinfo.tm_mon++;
	timeinfo.tm_year += 1900;


	TCHAR modulePath[MAX_PATH];
	TCHAR * moduleName;
	GetModuleFileName(NULL, modulePath, MAX_PATH);
	moduleName = PathFindFileName(modulePath);
	*(PathFindExtension(moduleName)) = 0;

	// Log to TextFiles
	char logFile[MAX_PATH];
	std::string logs_dir(".\\Logs");
	MAKEDIR(logs_dir.c_str());
	sprintf_s(logFile, "%s\\%s_%02d-%02d-%04d.txt", logs_dir.c_str(), moduleName, timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year);

	std::ofstream fsLog(logFile, std::ios::app);
	if (fsLog.is_open())
	{
		char timeData[MAX_PATH];
		sprintf_s(timeData, "%02d:%02d:%02d %02d-%02d-%04d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year);
		fsLog << "=============================================================================================" << std::endl;
		fsLog << "===\t\t\t\tLOG START " << timeData << "\t\t\t\t  ===" << std::endl;
		fsLog << "=============================================================================================" << std::endl;
		fsLog.close();
	}

	Log("%s - Dragon Ball Online Server\n", moduleName);
	SetConsoleTitle(moduleName);
}

void Logger::Log(char* szText, ...)
{
	va_list args;
	va_start(args, szText);
	time_t rawtime;
	tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	timeinfo.tm_mon++;
	timeinfo.tm_year += 1900;
	CONSOLECOLOR(YELLOW);
	printf(" %02d:%02d:%02d %02d-%02d-%04d ", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year);
	CONSOLECOLOR(LIGHTCYAN);
	printf("> ");
	CONSOLECOLOR(LIGHTGRAY);
	char logData[MAX_PATH];
	vsprintf_s(logData, szText, args);
	printf(logData);
	va_end(args);
//<<<<<<< HEAD
}

void Logger::SavePacket(BYTE* pData)
{
	try {
		WORD wLen = 0;
		memcpy(&wLen, &pData[0], 2);
		WORD wOpCode = 0;
		memcpy(&wOpCode, &pData[2], 2);

		if (GetFileAttributesA("./packets/") == INVALID_FILE_ATTRIBUTES)
		{
			_wmkdir(L"./packets/");
		}


		std::string filename = "";
		filename.append("./packets/packet_");
		filename.append(std::to_string(wOpCode));
		filename.append(".dat");

		std::ofstream fPacket(filename.c_str(), std::ios::out | std::ios::binary);
		fPacket.write((char*)pData, wLen + 2);
		fPacket.close();
	}
	catch (std::ofstream::failure e)
	{
		Logger::Log("Error writing packet: %s\n", e);
	}
}

void Logger::SavePacket(BYTE* pData, const char* packetName)
{
	try {
		WORD wLen = 0;
		memcpy(&wLen, &pData[0], 2);
		WORD wOpCode = 0;
		memcpy(&wOpCode, &pData[2], 2);

		if (GetFileAttributesA("./packets/") == INVALID_FILE_ATTRIBUTES)
		{
			_wmkdir(L"./packets/");
		}


		std::string filename = "";
		filename.append("./packets/packet_");
		filename.append(std::to_string(wOpCode));
		filename.append("_");
		filename.append(packetName);
		filename.append(".dat");

		std::ofstream fPacket(filename.c_str(), std::ios::out | std::ios::binary);
		fPacket.write((char*)pData, wLen + 2);
		fPacket.close();
	}
	catch (std::ofstream::failure e)
	{
		Logger::Log("Error writing packet: %s\n", e);
	}
}
//=======
/*
	TCHAR modulePath[MAX_PATH];
	TCHAR * moduleName;
	GetModuleFileName(NULL, modulePath, MAX_PATH);
	moduleName = PathFindFileName(modulePath);
	*(PathFindExtension(moduleName)) = 0;

	// Log to TextFiles
	char logFile[MAX_PATH];
	std::string logs_dir(".\\Logs");
	MAKEDIR(logs_dir.c_str());
	sprintf_s(logFile, "%s\\%s_%02d-%02d-%04d.txt", logs_dir.c_str(), moduleName, timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year);

	std::ofstream fsLog(logFile, std::ios::app);
	if (fsLog.is_open())
	{
		char timeData[MAX_PATH];
		sprintf_s(timeData, "%02d:%02d:%02d %02d-%02d-%04d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year);
		fsLog << timeData << " > ";
		fsLog << logData;
		fsLog.close();
	}
}*/

void CreateFolder(const char * path)
{
	if (!CreateDirectory(path, NULL))
	{
		return;
	}
}
//>>>>>>> d23e846e5701bea5d1f3006f0d9cdca96f9308a4
