#ifndef _HEADER_CONNECTION
#define _HEADER_CONNECTION

#pragma once

#include "Base.h"
#include "Mutex.h"
#include "Socket.h"
#include "Iocp.h"
#include "Packet.h"
#include "Encoder.h"
#include "NetBuffer.h"

const DWORD CONNECTION_KEEP_ALIVE_CHECK_TIME = 15 * 1000;

enum CONTROLFLAG
{
	CONTROL_FLAG_CHECK_ALIVE = 0x01 << 0,
	CONTROL_FLAG_CHECK_OPCODE = 0x01 << 1,
	CONTROL_FLAG_USE_SEND_QUEUE = 0x01 << 2,
	CONTROL_FLAG_USE_RECV_QUEUE = 0x01 << 3,

	MAX_CONTROL_FLAG
};

enum CONNSTATUS
{
	CONNSTATUS_INIT = 0,
	CONNSTATUS_CREATE,
	CONNSTATUS_ACCEPT,
	CONNSTATUS_CONNECT,
	CONNSTATUS_ACTIVE,
	CONNSTATUS_CLOSE,
	CONNSTATUS_SHUTDOWN,
	CONNSTATUS_DESTROY,

	CONN_MAX_STATUS
};

class Network;
class Session;
class Acceptor;
class Connector;

class Connections
{
	friend class IocpWorkerThread;
public:
	Connections();
	virtual ~Connections();

	int Create(Network* pNetwork);
	int	CompleteIO(IOCONTEXT* IOContext, DWORD Param);
	void Close(bool Force = false);
	virtual DWORD GetMaxRecvPacketSize() { return PACKET_MAX_SIZE; }
	virtual DWORD GetMaxSendPacketSize() { return PACKET_MAX_SIZE; }
	virtual DWORD GetMaxRecvPacketCount() { return DEF_PACKET_MAX_COUNT; }
	virtual DWORD GetMaxSendPacketCount() { return DEF_PACKET_MAX_COUNT; }
	virtual DWORD GetAliveCheckTime() { return CONNECTION_KEEP_ALIVE_CHECK_TIME; }
	void ResetAliveTime() { _AliveTime = 0; }
	int Disconnect(bool Graceful);
	bool ValidCheck(DWORD TickDiff);
	bool IsShutdownable();
	int	Shutdown();
	int PostRecv();
	int PostSend();
	int PostAccept(Acceptor* pAcceptor);
	int PostConnect(Connector* pConnector);
	int CompleteRecv(DWORD TransferedBytes);
	int CompleteSend(DWORD TransferedBytes);
	int CompleteAccept(DWORD TransferedBytes);
	int CompleteConnect(DWORD TransferedBytes);
	int PushPacket(void* pData, int nSize);
	int PushPacket(Packet* pPacket);
	bool PopPacket(Packet* pPacket);
	virtual bool IsValidPacket(void* PacketHeader, WORD PacketLength);
	void Lock() { _mutex.Lock(); }
	void Unlock() { _mutex.Unlock(); }
	Mutex* GetMutex() { return &_mutex; }
	IOCONTEXT* GetRecvContext() { return &_recvContext; }
	IOCONTEXT* GetSendContext() { return &_sendContext; }
	NetBuffer* GetRecvBuffer() { return &_recvBuffer; }
	NetBuffer* GetSendBuffer() { return &_sendBuffer; }
	Socket&	GetSocket() { return _socket; }
	SocketAddr& GetRemoteAddr() { return _remoteAddr; }
	SocketAddr&	GetLocalAddr() { return _localAddr; }
	void SetAddress(SOCKADDR_IN* LocalAddr, SOCKADDR_IN* RemoteAddr);
	const char* GetRemoteIP() { return _remoteAddr.GetDottedAddr(); }
	WORD GetRemotePort() { return _remoteAddr.GetPort(); }
	const char*	GetLocalIP() { return _localAddr.GetDottedAddr(); }
	WORD GetLocalPort() { return _localAddr.GetPort(); }
	bool GetAddressInfo(GameString* String, WORD* Port, bool Remote);
	Connector* GetConnector() { return _ConnectorRef; }
	Acceptor* GetAcceptor() { return _AcceptorRef; }
	int	MakeSureCompletedPacket(NetBuffer* pBuffer, WORD* PacketLength, PacketEncoder* pPacketEncoder);
	void SetControlFlag(DWORD ControlFlag) { BIT_FLAG_SET(_ControlFlag, ControlFlag); }
	void UnsetControlFlag(DWORD ControlFlag) { BIT_FLAG_UNSET(_ControlFlag, ControlFlag); }
	bool IsSetControlFlag(DWORD ControlFlag) { return BIT_FLAG_TEST(_ControlFlag, ControlFlag); }
	void SetPacketEncoder(PacketEncoder* pPacketEncoder) { _PacketEncoder = pPacketEncoder; }
	PacketEncoder* GetPacketEncoder() { return _PacketEncoder; }
	CONNSTATUS GetStatus();
	void SetStatus(CONNSTATUS status);
	bool IsStatus(CONNSTATUS status);
	bool ExchangeStatus(const CONNSTATUS prevStatus, const bool Result, const CONNSTATUS newStatus);
	const char* GetStatusString();
	bool IsConnected() { return _IsConnected; }
	DWORD GetConnectTime() { return _ConnectTime; }
	DWORD GetBytesTotalSize() { return GetBytesRecvSize() + GetBytesSendSize(); }
	DWORD GetBytesRecvSize() { return _BytesRecvSize; }
	DWORD GetBytesRecvSizeMax() { return _BytesRecvSizeMax; }
	DWORD GetBytesSendSize() { return _BytesSendSize; }
	DWORD GetBytesSendSizeMax() { return _BytesSendSizeMax; }
	void IncreaseBytesRecv(DWORD Size);
	void IncreaseBytesSend(DWORD Size);
	DWORD GetPacktTotalCount() { return GetPacketRecvCount() + GetPacketSendCount(); }
	DWORD GetPacketRecvCount() { return _PacketRecvCount; }
	DWORD GetPacketSendCount() { return _PacketSendCount; }
	void IncreasePacketRecv();
	void IncreasePacketSend();
	DWORD GetRecvQueueMaxUseSize() { return _recvQueue.GetMaxUsedSize(); }
	DWORD GetSendQueueMaxUseSize() { return _sendQueue.GetMaxUsedSize(); }
	virtual int GetHeaderSize() { return HEADER_SIZE; }
	virtual int GetPacketLen(BYTE* HeaderPointer);

private:
	void Init();
	void Destroy();
	void IncreasePostIoCount() { InterlockedIncrement((LONG*)&_IoPostCount); }
	void DecreasePostIoCount() { InterlockedDecrement((LONG*)&_IoPostCount); }

	CONNSTATUS _status;
	Network* _NetworkRef;
	Acceptor* _AcceptorRef;
	Connector* _ConnectorRef;
	DWORD _IoPostCount;
	Socket _socket;
	SocketAddr _remoteAddr;
	SocketAddr _localAddr;
	IOCONTEXT _recvContext;
	IOCONTEXT _sendContext;
	DWORD _AliveTime;
	NetBuffer _recvBuffer;
	NetBuffer _sendBuffer;
	NetQueue _recvQueue;
	NetQueue _sendQueue;
	PacketEncoder* _PacketEncoder;
	DWORD _ControlFlag;
	bool _IsSending;
	bool _IsDisconnect;
	bool _IsConnected;
	Mutex _mutex;
	Mutex _mutexSend;
	DWORD _ConnectTime;
	DWORD _BytesRecvSize;
	DWORD _BytesSendSize;
	DWORD _BytesRecvSizeMax;
	DWORD _BytesSendSizeMax;
	DWORD _PacketRecvCount;
	DWORD _PacketSendCount;
};

inline void Connections::IncreaseBytesRecv(DWORD Size)
{
	InterlockedExchangeAdd((LONG*)&_BytesRecvSize, Size);

	if (Size > _BytesRecvSizeMax)
	{
		_BytesRecvSizeMax = Size;
	}
}

inline void Connections::IncreaseBytesSend(DWORD Size)
{
	InterlockedExchangeAdd((LONG*)&_BytesSendSize, Size);

	if (Size > _BytesSendSizeMax)
	{
		_BytesSendSizeMax = Size;
	}
}

inline void Connections::IncreasePacketRecv()
{
	InterlockedIncrement((LONG*)&_PacketRecvCount);
}

inline void Connections::IncreasePacketSend()
{
	InterlockedIncrement((LONG*)&_PacketSendCount);
}

inline CONNSTATUS Connections::GetStatus()
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	CONNSTATUS status = _status;

	return status;
}

inline void	Connections::SetStatus(CONNSTATUS status)
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	_status = status;
}

inline bool Connections::IsStatus(CONNSTATUS status)
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	return _status == status;
}

inline bool Connections::ExchangeStatus(const CONNSTATUS prevStatus, const bool Result, const CONNSTATUS newStatus)
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	if (Result)
	{
		if (prevStatus != _status)
			return false;
	}
	else
	{
		if (prevStatus == _status)
			return false;
	}

	_status = newStatus;

	return true;
}

inline int Connections::MakeSureCompletedPacket(NetBuffer* pBuffer, WORD* PacketLength, PacketEncoder * pPacketEncoder)
{
	if (NULL == PacketLength)
	{
		_ASSERT(0);
		return 1; //ERR_SYS_INPUT_PARAMETER_WRONG
	}

	if (pBuffer->GetWorkRemainSize() < GetHeaderSize())
	{
		return 2; //ERR_NET_PACKET_PENDING_HEADER
	}

	int nLinearSize = (int)(pBuffer->InGetQueueExtraPtr() - pBuffer->GetQueueWorkPtr());

	if (nLinearSize < GetHeaderSize())
	{
		::CopyMemory(pBuffer->InGetQueueExtraPtr(), pBuffer->InGetQueueBufferPtr(), GetHeaderSize() - nLinearSize);
	}

	int wPacketLen = GetPacketLen(pBuffer->GetQueueWorkPtr());

	int nPacketSize = GetHeaderSize() + wPacketLen;
	if (nPacketSize >= pBuffer->GetMaxPacketSize())
	{
		_ASSERT(0);
		return 3; //ERR_NET_PACKET_EXCEED_ALLOWED_SIZE
	}

	if (pBuffer->GetWorkRemainSize() < nPacketSize)
	{
		return 4; //ERR_NET_PACKET_PENDING_DATA
	}

	if (nLinearSize < nPacketSize)
	{
		if (nLinearSize < GetHeaderSize())
		{
			::CopyMemory(pBuffer->GetQueueWorkPtr() + GetHeaderSize(),
				pBuffer->InGetQueueBufferPtr() + GetHeaderSize() - nLinearSize,
				wPacketLen);
		}
		else
		{
			::CopyMemory(pBuffer->InGetQueueExtraPtr(),
				pBuffer->InGetQueueBufferPtr(),
				nPacketSize - nLinearSize);
		}
	}

	*PacketLength = (WORD)nPacketSize;


	return 0;
}

#endif