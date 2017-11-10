#ifndef _NETWORK
#define _NETWORK

#pragma once

#include "Base.h"
#include "Iocp.h"
#include "SessionList.h"
#include <map>

enum NETEVENT
{
	NETEVENT_ACCEPT = 1,
	NETEVENT_CONNECT,
	NETEVENT_CLOSE,
	NETEVENT_RECV,

	INVALID_NETEVENT,
	NETEVENT_COUNT,
};

class Connections;
class Session;
class Acceptor;
class Connector;
class SessionFactory;
class NetworkMonitor;
class NetworkProcessor;
class Packet;

class Network
{
	friend class NetworkMonitor;
	friend class NetworkProcessor;
	typedef std::multimap<unsigned short, Acceptor*> AcceptorList;
	typedef std::multimap<unsigned short, Connector*> ConnectorList;

public:
	Network();
	virtual ~Network();

	int Create(SessionFactory* Factory, int SessionSize, int CreateThreads = 0,	int ConcurrentThreads = 0);
	void Destroy();
	int Associate(Connections* pConnection, bool IsAssociate);
	int Associate(Acceptor* pAcceptor, bool IsAssociate);
	int Associate(Connector* pConnector, bool IsAssociate);
	int Send(unsigned int hSession, Packet * pPacket);
	int Send(Session* pSession, Packet * pPacket);
	int PostNetEventMessage(WPARAM wParam, LPARAM lParam);
	SessionList* GetSessionList() { return _SessionList; }
	SessionFactory* GetSessionFactory() { return _SessionFactoryRef; }
	Acceptor* GetAcceptor(unsigned short sessionType, const char* Addr, WORD Port);
	Connector* GetConnector(unsigned short sessionType, const char* Addr, WORD Port);

protected:
	void Init();
	int StartUp();
	int Shutdown();
	int CreateMonitorThread();
	int	CreateDispatcherThread();

private:
	Iocp _iocp;
	SessionList* _SessionList;
	AcceptorList* _AcceptorList;
	ConnectorList* _ConnectorList;
	SessionFactory* _SessionFactoryRef;
	NetworkMonitor* _NetworkMonitor;
	NetworkProcessor*_NetworkProcessor;
};

#endif