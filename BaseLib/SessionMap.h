#ifndef _SESSIONMAP
#define _SESSIONMAP

#pragma once

#include "Base.h"
#include <map>

#include "Mutex.h"
#include "Session.h"


template<class TKEY>
class SessionMap
{
public:
	SessionMap(void)
		: _CurCount(0), _MaxCount(0) { }
	virtual ~SessionMap(void) {}

	bool AddSession(TKEY keyVal, Session* pSession);
	void RemoveSession(TKEY keyVal);
	Session* FindSession(TKEY keyVal);
	Session* AcquireSession(TKEY keyVal);
	DWORD GetCurCount() { return _CurCount; }
	DWORD GetMaxCount() { return _MaxCount; }
	Session* GetFirst();
	Session* GetNext();
	void Lock() { _mutex.Lock(); }
	void Unlock() { _mutex.Unlock(); }

private:
	typedef std::map<TKEY, Session*> SESSIONMAP;
	typedef typename std::map<TKEY, Session*>::value_type SESSIONMAP_VAL;
	typedef typename std::map<TKEY, Session*>::iterator SESSIONMAP_IT;
	CNtlMutex _mutex;
	SESSIONMAP _sessionMap;
	DWORD _CurCount;
	DWORD _MaxCount;
	SESSIONMAP_IT _sessionIt;
};

template<class TKEY>
inline bool SessionMap<TKEY>::AddSession(TKEY keyVal, Session * pSession)
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	if (false == _sessionMap.insert(SESSIONMAP_VAL(keyVal, pSession)).second)
	{
		return false;
	}

	++_CurCount;

	if (_CurCount > _MaxCount)
	{
		_MaxCount = _CurCount;
	}

	pSession->Acquire();
	return true;
}

template<class TKEY>
inline void SessionMap<TKEY>::RemoveSession(TKEY keyVal)
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	SESSIONMAP_IT it = _sessionMap.find(keyVal);
	if (it == _sessionMap.end())
	{
		return;
	}

	--_CurCount;

	it->second->Release();

	_sessionMap.erase(it);
}

template<class TKEY>
inline Session* SessionMap<TKEY>::FindSession(TKEY TKEY)
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	SESSIONMAP_IT it = _sessionMap.find(keyVal);
	if (it == _sessionMap.end())
	{
		return NULL;
	}

	return it->second;
}

template<class TKEY>
inline Session * SessionMap<TKEY>::AcquireSession(TKEY TKEY)
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	SESSIONMAP_IT it = _sessionMap.find(TKEY);
	if (it == _sessionMap.end())
	{
		return NULL;
	}

	it->second->Acquire();

	return it->second;
}

template<class TKEY>
inline Session* SessionMap<TKEY>::GetFirst()
{
	_sessionIt = _sessionMap.begin();
	if (_sessionIt == _sessionMap.end())
	{
		return NULL;
	}

	return _sessionIt->second;
}

template<class TKEY>
inline Session* SessionMap<TKEY>::GetNext()
{
	_ASSERT(_sessionIt != _sessionMap.end());

	if (++_sessionIt == _sessionMap.end())
	{
		return NULL;
	}

	return _sessionIt->second;
}


#endif