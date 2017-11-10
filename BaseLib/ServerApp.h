#ifndef _SERVERAPP
#define _SERVERAPP

#pragma once

#include <tchar.h>

#include "Base.h"
#include "Network.h"
#include "Thread.h"
#include "Singleton.h"

class SessionFactory;

class ServerApp : public RunObject, public Singleton<ServerApp>
{
public:
	ServerApp();
	virtual ~ServerApp();

	virtual int OnInitApp();
	virtual int OnCreate();
	virtual int OnCommandArgument(int argc, char* argv[]);
	virtual int OnConfiguration(const char* ConfigFile);
	virtual int OnAppStart();
	virtual void OnRun();

	int Create(int argc, char* argv[]);
	void Destroy();
	void Start(bool IsAutoDelete = false);
	void Stop();
	void WaitForTerminate();
	int Send(unsigned int hSession, Packet* pPacket) { return _network.Send(hSession, pPacket); }
	int SendTo(Session* pSession, Packet* pPacket) { return _network.Send(pSession, pPacket); }
	Network* GetNetwork() { return &_network; }
	void SetConfigFileName(const char* AppName);

protected:
	void Init();


private:
	void run() { this->OnRun(); }

protected:
	SessionFactory* _SessionFactory;
	Network _network;
	int _MaxSessionCount;
	std::string _strConfigFile;

private:
	Thread* _AppThread;
};

#define _GetApp() ServerApp::GetInstance()
#define gApp _GetApp()

#endif