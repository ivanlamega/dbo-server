#include "stdafx.h"
#include "SessionList.h"
#include "Network.h"
#include "Session.h"
#include "Logger.h"

SessionList::SessionList()
{
	Init();
}

SessionList::~SessionList()
{
	Destroy();
}

int SessionList::Create(Network* pNetwork, int SessionSize, int ExtraSize)
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	if (false == _sessionList.Create(SessionSize, ExtraSize))
	{
		return -1;
	}

	_NetworkRef = pNetwork;

	return 0;
}

void SessionList::Destroy()
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	Session** ppSession = NULL;
	for (LISTIT it = _sessionList.Begin(); it != _sessionList.End(); it = _sessionList.Next(it))
	{
		ppSession = _sessionList.GetPtr(it);
		if (*ppSession)
		{
			SAFE_DELETE(*ppSession);
		}
	}
}

void SessionList::Init()
{
	_NetworkRef = NULL;
}

Session* SessionList::Find(LISTIT pos)
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	Session** ppSession = _sessionList.GetPtr(pos);
	if (NULL == ppSession)
	{
		return NULL;
	}

	return *ppSession;
}

bool SessionList::Add(Session* pSession)
{
	if (NULL == pSession)
	{
		Logger::Log("NULL == pSession\n");
		return false;
	}

	if (INVALID_HSESSION != pSession->GetHandle())
	{
		Logger::Log("INVALID_HSESSION != pSession->GetHandle(), pSession->GetHandle() = %u\n", pSession->GetHandle());
		return false;
	}

	AutoMutex mutex(&_mutex);
	mutex.Lock();

	LISTIT it = _sessionList.Insert(pSession);
	if (it == _sessionList.End())
	{
		return false;
	}

	pSession->_hSession = it;

	return true;
}

bool SessionList::Remove(Session * pSession)
{
	if (NULL == pSession)
	{
		Logger::Log("NULL == pSession\n");
		return false;
	}

	if (INVALID_HSESSION == pSession->GetHandle())
	{
		return true;
	}

	AutoMutex mutex(&_mutex);
	mutex.Lock();

	_sessionList.Remove(pSession->GetHandle());

	pSession->_hSession = INVALID_HSESSION;

	return true;
}

int SessionList::GetCurCount()
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	return _sessionList.GetSize();
}

int SessionList::GetMaxCount()
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	return _sessionList.GetMaxSize();
}

void SessionList::ValidCheck(DWORD TickTime)
{
	UNREFERENCED_PARAMETER(TickTime);

	AutoMutex mutex(&_mutex);
	mutex.Lock();

	Session* pSession = NULL;
	for (LISTIT it = _sessionList.Begin(); it != _sessionList.End();)
	{
		pSession = *(_sessionList.GetPtr(it));

		if (pSession)
		{
			if (false == pSession->ValidCheck(TickTime))
			{
				Logger::Log("The session[%X] should be disconnected due to timeout.\n", pSession);
				pSession->Disconnect(false);
			}

			if (true == pSession->IsShutdownable())
			{
				pSession->Shutdown();
				_NetworkRef->PostNetEventMessage((WPARAM)NETEVENT_CLOSE, (LPARAM)pSession);
				it = _sessionList.Remove(it);
				continue;
			}
		}

		it = _sessionList.Next(it);
	}

}