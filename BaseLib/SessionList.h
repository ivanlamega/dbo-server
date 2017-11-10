#ifndef _SESSIONLIST
#define _SESSIONLIST

#pragma once

#include "Base.h"
#include "LinkArray.h"
#include "Mutex.h"

class Network;
class Session;

class SessionList
{
public:
	SessionList();
	virtual ~SessionList();

	int Create(Network* pNetwork, int SessionSize, int ExtraSize);
	void Destroy();
	bool Add(Session* pSession);
	bool Remove(Session* pSession);
	Session* Find(unsigned int hSession);
	void ValidCheck(DWORD TickTime);
	int GetCurCount();
	int GetMaxCount();

protected:
	void Init();

private:
	typedef LinkArray_Static<Session*> LIST;
	typedef LIST::POSITION LISTIT;
	LIST _sessionList;
	Mutex _mutex;
	Network* _NetworkRef;
};

#endif