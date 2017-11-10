#ifndef _IOCP
#define _IOCP

#pragma once
#include "Base.h"

enum IOMODE
{
	IOMODE_ACCEPT = 1,
	IOMODE_CONNECT,
	IOMODE_RECV,
	IOMODE_SEND,

	MAX_IOMODE
};

struct IOCONTEXT : public OVERLAPPED
{
	WSABUF wsabuf;
	IOMODE iomode;
	LPCVOID param;

	void Clear() { ZeroMemory(this, sizeof(IOCONTEXT)); }
	void Reset() { ZeroMemory(this, sizeof(OVERLAPPED)); }
};

class Network;

class Iocp
{
	friend class IocpWorkerThread;
public:
	Iocp();
	virtual ~Iocp();

	int Create(Network* pNetwork, int nCreateThreads, int ConcurrentThreads);
	void Destroy();
	int Associate(SOCKET Sock, LPCVOID CompletionKey);

protected:
	int CreateIOCP(int ConcurrentThreads);
	int CreateThreads(int OpenThreads);
	void CloseThreads();

private:
	Network* _NetworkRef;
	HANDLE _hIOCP;
	int _CreatedThreads;
};

#endif