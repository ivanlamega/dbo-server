#include "stdafx.h"
#include "Session.h"
#include "Logger.h"

Session::Session(unsigned short sessionType)
	: _sessionType(sessionType)
{
	Init();
}

Session::~Session()
{
	Destroy();
}

void Session::Init()
{
	_hSession = INVALID_HSESSION;
	_ReferenceCount = 1;
}

void Session::Destroy()
{
	if (IsConnected())
	{
		//Log somethings
	}
}

void Session::Acquire()
{
	InterlockedIncrement((LONG*)&_ReferenceCount);
}

void Session::Release()
{
	if (0 == InterlockedDecrement((LONG*)&_ReferenceCount))
	{
		//if(this) delete this;
	}
}

bool Session::IsInternalConnection(char* Ip)
{
#if defined(INTERNAL_ADDRESS_PREFIX)
#undef INTERNAL_ADDRESS_PREFIX
#endif

#define INTERNAL_ADDRESS_PREFIX		"10.0.0."
	if (NULL == Ip)
	{
		return false;
	}

	if (0 == strncmp(Ip, INTERNAL_ADDRESS_PREFIX, strlen(INTERNAL_ADDRESS_PREFIX)))
	{
		return true;
	}
	else
	{
		return false;
	}
#undef NTL_INTERNAL_ADDRESS_PREFIX
}

int Session::OnDispatch(Packet* pPacket)
{
	PACKETDATA* pPacketData = (PACKETDATA*)pPacket->GetPacketData();
	switch (pPacketData->wOpCode)
	{
	case SYS_ALIVE: { ResetAliveTime(); } break;
	case SYS_PING: break;
	default:
		Logger::Log("Session[%s:%X] Type[%u] Send Wrong Packet[%u]", typeid(this), this, _sessionType, pPacketData->wOpCode);
		return -1;
	}

	return 0;
}
