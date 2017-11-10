#ifndef _CONNECTOR
#define _CONNECTOR

#pragma once

#include "Base.h"
#include "Iocp.h"
#include "SocketAddr.h"
#include "Socket.h"
#include "Mutex.h"

//#define __USE_CONNECTEX__

class Session;
class Connections;

class Connector
{
	friend class ConnectorThread;
public:
	Connector();
	virtual ~Connector();

	int	Create(LPCTSTR ConnectAddr, WORD ConnectPort, unsigned short sessionType, DWORD RetryTime = 1000, DWORD ProcessTime = 1000);
	void Destroy();
	unsigned short GetSessionType() { return _sessionType; }
	SocketAddr&	GetConnectAddr() { return _connectAddr; }
	const char*	GetConnectIP() { return _connectAddr.GetDottedAddr(); }
	WORD GetConnectPort() { return _connectAddr.GetPort(); }
	DWORD GetTotalConnectCount() { return _TotalConnectCount; }
	int OnAssociated(Network* pNetwork);
	void OnConnected();
	void OnDisconnected(bool IsConnected);

protected:
	int CreateThread();
	int DoConnect();
	void Init();

private:
	Network* _Network;
	SocketAddr _connectAddr;
	unsigned short _sessionType;
	DWORD _RetryTime;
	DWORD _ProcessTime;
	DWORD _TotalConnectCount;
	BOOL _IsConnected;
	ConnectorThread* _Thread;
	Mutex _mutex;
};

inline void	Connector::OnConnected()
{
	InterlockedExchange((LONG*)&_IsConnected, TRUE);
	++_TotalConnectCount;
}

inline void	Connector::OnDisconnected(bool IsConnected)
{
	if (IsConnected)
	{
		InterlockedExchange((LONG*)&_IsConnected, FALSE);
	}
}

#endif