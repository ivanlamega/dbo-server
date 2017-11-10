#ifndef _HEADER_SESSION
#define _HEADER_SESSION

#include "Base.h"
#include "Connections.h"

#pragma once

#define RELEASE_SESSION(p) if(p) { p->Release(); p = NULL; }
#define INVALID_HSESSION 0xFFFFFFFF


class Session : public Connections
{
	friend class SessionList;
public:
	Session(unsigned short sessionType);
	virtual ~Session();

	virtual int OnConnect() { return 0; }
	virtual int OnAccept() { return 0; }
	virtual void OnClose() {}
	virtual int OnDispatch(Packet* pPacket);
	unsigned int GetHandle() { return _hSession; }
	unsigned short GetSessionType() { return _sessionType; }
	void Acquire();
	void Release();
	static bool IsInternalConnection(char* Ip);

protected:
	void Init();
	void Destroy();

private:
	Session& operator=(Session&);
	DWORD _ReferenceCount;
	unsigned int _hSession;
	const unsigned short _sessionType;
};

class SessionAutoPtr
{
public:
	explicit SessionAutoPtr(Session* pSession) : _Session(pSession) {}
	~SessionAutoPtr() {	RELEASE_SESSION(_Session); }

private:
	Session * _Session;
};

#endif