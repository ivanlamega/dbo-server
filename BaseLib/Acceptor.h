#ifndef _ACCEPTOR
#define _ACCEPTOR

#pragma once

#include "Base.h"
#include "Iocp.h"
#include "Socket.h"
#include "SocketAddr.h"
#include "Mutex.h"


class Network;
class Acceptor
{
	friend class AcceptorThread;
public:
	Acceptor();
	virtual ~Acceptor();

	int	Create(const char* Addr, unsigned short port, unsigned short sessionType, int MaxAcceptCount, int PostAcceptCount, int MinAcceptCount, int CreateAcceptCount);
	void Destroy();
	int OnAssociated(Network* pNetwork);
	void OnAccepted();
	void OnDisconnected(bool IsConnected);
	void IncreaseCurAcceptingCount();
	void DecreaseCurAcceptingCount();
	void IncreaseCurAcceptedCount();
	void DecreaseCurAcceptedCount();
	int GetReserveAcceptCount();
	unsigned short GetSessionType() { return _sessionType; }
	Socket& GetListenSocket() { return _listenSocket; }
	SocketAddr& GetListenAddr() { return _listenAddr; }
	const char* GetListenIP() { return _listenAddr.GetDottedAddr(); }
	WORD GetListenPort() { return _listenAddr.GetPort(); }
	DWORD GetTotalAcceptCount() { return _TotalAcceptCount; }

	int _MaxAcceptCount;
	int _MinAcceptCount;
	int _PostAcceptCount;
	int _CreateAcceptCount;
	int _AcceptingCount;
	int _AcceptedCount;
	DWORD _TotalAcceptCount;

protected:
	void Init();
	int Listen();
	int CreateThread();
	int ReserveAccept(int ReserveCount);

private:
	Network* _Network;
	SocketAddr _listenAddr;
	Socket _listenSocket;
	unsigned short _sessionType;
	AcceptorThread* _Thread;
	Mutex _mutex;
};

inline void	Acceptor::IncreaseCurAcceptingCount()
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	++_AcceptingCount;
}

inline void Acceptor::DecreaseCurAcceptingCount()
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	--_AcceptingCount;
}

inline void Acceptor::IncreaseCurAcceptedCount()
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	++_AcceptedCount;
}


inline void Acceptor::DecreaseCurAcceptedCount()
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	--_AcceptedCount;
}


inline void	Acceptor::OnAccepted()
{
	DecreaseCurAcceptingCount();
	IncreaseCurAcceptedCount();

	++_TotalAcceptCount;
}


inline void Acceptor::OnDisconnected(bool IsConnected)
{
	if (IsConnected)
		DecreaseCurAcceptedCount();
	else
		DecreaseCurAcceptingCount();
}

#endif