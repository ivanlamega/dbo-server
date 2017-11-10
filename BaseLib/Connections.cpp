#include "stdafx.h"
#include "Connections.h"

#include "Network.h"
#include "Acceptor.h"
#include "Connector.h"
#include "Logger.h"	
#include "Thread.h"

class FunctionHelper
{
public:
	FunctionHelper(const char* FunctionName, void* pointer)
		:_FunctionName(FunctionName), _pointer(pointer) {}
	~FunctionHelper() {}
	const char* _FunctionName;
	void* _pointer;
};

#define FUNCTION_BEGIN() FunctionHelper __functionHelper(__FUNCTION__, this)

const DWORD CONNECTION_KEEP_ALIVE_TIME = 5 * 1000;
const DWORD CONNECTION_KEEP_ALIVE_INTERVAL = 1 * 1000;

char * connection_status_string[CONN_MAX_STATUS] =
{
	"STATUS_INIT",
	"STATUS_CREATE",
	"STATUS_ACCEPT",
	"STATUS_CONNECT",
	"STATUS_ACTIVE",
	"STATUS_CLOSE",
	"STATUS_SHUTDOWN",
	"STATUS_DESTROY",
};

const char* Connections::GetStatusString()
{
	if (_status >= CONN_MAX_STATUS)
		return "Unknown STATUS";

	return connection_status_string[_status];
}

Connections::Connections()
{
	FUNCTION_BEGIN();

	Init();
}

Connections::~Connections()
{
	FUNCTION_BEGIN();

	Destroy();
}

void Connections::Init()
{
	FUNCTION_BEGIN();

	_status = CONNSTATUS_INIT;

	_NetworkRef = NULL;
	_AcceptorRef = NULL;
	_ConnectorRef = NULL;
	_IoPostCount = 0;

	_recvBuffer.Clear();
	_sendBuffer.Clear();
	_recvContext.Clear();
	_sendContext.Clear();
	_recvContext.param = this;
	_sendContext.param = this;
	_PacketEncoder = NULL;

	_ControlFlag = 0;
	_AliveTime = 0;

	_IsSending = false;
	_IsDisconnect = false;
	_IsConnected = false;

	_ConnectTime = 0;
	_BytesRecvSize = 0;
	_BytesSendSize = 0;
	_BytesRecvSizeMax = 0;
	_BytesSendSizeMax = 0;
	_PacketRecvCount = 0;
	_PacketSendCount = 0;
}

int Connections::Create(Network* pNetwork)
{
	FUNCTION_BEGIN();


	if (IsStatus(CONNSTATUS_CREATE))
	{
		Logger::Log("true == IsStatus( STATUS_CREATE )\n");
		return 1;
	}


	if (NULL == pNetwork)
	{
		Logger::Log("NULL == pNetwork\n");
		return 2;
	}


	if (NULL != _NetworkRef)
	{
		Logger::Log("NULL != _NetworkRef, _NetworkRef = %016x\n", _NetworkRef);
		return 3;
	}


	if (false == _recvBuffer.Create(GetMaxRecvPacketCount(), GetMaxRecvPacketSize()))
	{
		return 4;
	}


	if (false == _sendBuffer.Create(GetMaxSendPacketCount(), GetMaxRecvPacketSize()))
	{
		return 5;
	}


	int rc = _socket.Create();
	if (0 != rc)
	{
		return rc;
	}


	rc = _socket.SetKeepAlive(CONNECTION_KEEP_ALIVE_TIME, CONNECTION_KEEP_ALIVE_INTERVAL);
	if (0 != rc)
	{
		return rc;
	}


	rc = _socket.SetLinger(true, 0);
	if (0 != rc)
	{
		return rc;
	}

	_PacketEncoder = GetPacketEncoder();

	_NetworkRef = pNetwork;


	SetStatus(CONNSTATUS_CREATE);

	return 0;
}

void Connections::Destroy()
{
	FUNCTION_BEGIN();

	if (false == ExchangeStatus(CONNSTATUS_DESTROY, false, CONNSTATUS_DESTROY))
	{
		Logger::Log("Status( CONNSTATUS_DESTROY )");
		return;
	}

	_socket.Shutdown(SD_BOTH);
	_socket.Close();

	if (_AcceptorRef)
	{
		_AcceptorRef->OnDisconnected(_IsConnected);
	}

	if (_ConnectorRef)
	{
		_ConnectorRef->OnDisconnected(_IsConnected);
	}
}

void Connections::Close(bool Force)
{
	UNREFERENCED_PARAMETER(Force);

	FUNCTION_BEGIN();

	if (false == ExchangeStatus(CONNSTATUS_CLOSE, false, CONNSTATUS_CLOSE))
	{
		return;
	}
}

int Connections::Disconnect(bool Graceful)
{
	FUNCTION_BEGIN();

	if (Graceful)
	{
		_socket.Shutdown(SD_SEND);
	}
	else
	{
		_socket.Shutdown(SD_BOTH);
		_socket.Close();
	}

	_IsDisconnect = true;

	return 0;
}

int Connections::Shutdown()
{
	FUNCTION_BEGIN();

	SetStatus(CONNSTATUS_SHUTDOWN);

	return 0;
}

int Connections::CompleteIO(IOCONTEXT* IOContext, DWORD Param)
{
	switch (IOContext->iomode)
	{
	case IOMODE_ACCEPT:
		return CompleteAccept(Param);

	case IOMODE_CONNECT:
		return CompleteConnect(Param);

	case IOMODE_RECV:
		return CompleteRecv(Param);

	case IOMODE_SEND:
		return CompleteSend(Param);

	default:
		return -1; //ERR_NET_INVALID_COMPLETE_IO_MODE
	}
}

int Connections::PostRecv()
{
	FUNCTION_BEGIN();

	if (false == IsStatus(CONNSTATUS_ACTIVE))
	{
		Disconnect(false);
		return 0;
	}

	DWORD Flags = 0;
	DWORD TransferedBytes = 0;

	_recvContext.Reset();
	_recvContext.iomode = IOMODE_RECV;
	_recvContext.wsabuf.buf = (char*)_recvBuffer.GetQueuePushPtr();
	_recvContext.wsabuf.len = _recvBuffer.GetPushAvailableSize();

	if (_recvBuffer.GetPushAvailableSize() <= 0)
	{
		Disconnect(false);
		Logger::Log("Session[%X] _recvBuffer.GetPushAvailableSize() <= 0, _recvBuffer.GetPushAvailableSize() = %d\n", this, _recvBuffer.GetPushAvailableSize());
		return -1; //ERR_NET_SESSION_RECV_BUFFER_OVERFLOW
	}

	IncreasePostIoCount();

	int rc = _socket.RecvEx(&_recvContext.wsabuf,
		1,
		&TransferedBytes,
		&Flags,
		&_recvContext);

	if (0 != rc)
	{
		DecreasePostIoCount();
		Logger::Log("Session[%X] RecvEx Function Failed (%d)/n", this, rc);
		return rc;
	}

	return 0;
}

int Connections::PostSend()
{
	FUNCTION_BEGIN();

	if (false == IsStatus(CONNSTATUS_ACTIVE))
	{
		Disconnect(false);
		return 0;
	}


	DWORD Flags = 0;
	DWORD SendBytes = 0;

	_sendContext.Reset();
	_sendContext.iomode = IOMODE_SEND;
	_sendContext.wsabuf.buf = (char*)_sendBuffer.GetQueuePopPtr();

	if (_sendBuffer.GetPopPos() + _sendBuffer.GetCurSize() >= _sendBuffer.GetQueueSize())
	{
		_sendContext.wsabuf.len = _sendBuffer.GetQueueSize() - _sendBuffer.GetPopPos();
	}
	else
	{
		_sendContext.wsabuf.len = _sendBuffer.GetCurSize();
	}

	IncreasePostIoCount();

	int rc = _socket.SendEx(&_sendContext.wsabuf,
		1,
		&SendBytes,
		Flags,
		&_sendContext);

	if (0 != rc)
	{
		DecreasePostIoCount();
		Logger::Log("Session[%X] SendEx Function Failed: (%d)\n", this, rc);
		return rc;
	}

	return 0;
}

int Connections::PostAccept(Acceptor* pAcceptor)
{
	FUNCTION_BEGIN();

	if (NULL == pAcceptor)
	{
		return 1;
	}

	SetStatus(CONNSTATUS_ACCEPT);

	_AcceptorRef = pAcceptor;

	DWORD Bytes = 0;

	_recvContext.Reset();
	_recvContext.iomode = IOMODE_ACCEPT;
	_recvContext.wsabuf.buf = (char*)_recvBuffer.GetQueuePushPtr();
	_recvContext.wsabuf.len = 0;

	IncreasePostIoCount();
	_AcceptorRef->IncreaseCurAcceptingCount();


	int rc = pAcceptor->GetListenSocket().AcceptEx(_socket,
		_recvContext.wsabuf.buf,
		0,
		sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16,
		&Bytes,
		&_recvContext);

	if (0 != rc)
	{
		DecreasePostIoCount();
		_AcceptorRef->DecreaseCurAcceptingCount();
		Logger::Log("Session[%X] AcceptEx Function Failed: (%d)\n", this, rc);
		return rc;
	}

	return 0;
}

int Connections::PostConnect(Connector* pConnector)
{
	FUNCTION_BEGIN();


	if (NULL == pConnector)
	{
		return 1;
	}

	SetStatus(CONNSTATUS_CONNECT);

	_ConnectorRef = pConnector;


#if !defined( __USE_CONNECTEX__ )
	int rc = _socket.Connect(pConnector->GetConnectAddr());
	if (0 != rc)
	{
		return rc;
	}
#else
	DWORD Bytes = 0;

	_recvContext.Reset();
	_recvContext.iomode = IOMODE_CONNECT;
	_recvContext.wsabuf.buf = (char*)_sendBuffer.GetQueuePushPtr();
	_recvContext.wsabuf.len = 0;
	IncreasePostIoCount();

	int rc = _socket.ConnectEx(pConnector->GetConnectAddr(),
		sizeof(struct sockaddr_in),
		_recvContext.wsabuf.buf,
		0,
		&Bytes,
		&_sendContext);

	if (0 != rc)
	{
		DecreasePostIoCount();
		return rc;
	}

#endif

	return 0;
}

int Connections::CompleteRecv(DWORD TransferedBytes)
{
	FUNCTION_BEGIN();

	if (0 == TransferedBytes)
	{
		return 1;
	}

	IncreaseBytesRecv(TransferedBytes);

	if (false == _recvBuffer.IncreasePushPos(TransferedBytes))
	{
		Logger::Log("Session[%X] Recv Buffer OverFlow : BufferCur[%d] BufferMax[%d] TransferedBytes[%u]\n", this, _recvBuffer.GetCurSize(), _recvBuffer.GetQueueSize(), TransferedBytes);
		return 2;
	}

	int rc = 0;
	WORD PacketLength = 0;

	Packet packet;
	do
	{
		rc = MakeSureCompletedPacket(&_recvBuffer, &PacketLength, _PacketEncoder);
		if (0 != rc)
		{
			if (2 == rc || 4 == rc)
			{
				break;
			}

			Logger::Log("Session[%X] MakeSureCompletedPacket Error[%d]\n", this, rc);
			return rc;
		}

		if (true != IsValidPacket(_recvBuffer.GetQueueWorkPtr(), PacketLength))
		{
			Logger::Log("true != IsValidPacket()\n");
		}

		packet.AttachData(_recvBuffer.GetQueueWorkPtr(), PacketLength);

		_recvBuffer.IncreaseWorkPos(packet.GetUsedSize());

		LPPACKETDATA pData = (LPPACKETDATA)packet.GetPacketData();

		if (_PacketEncoder && pData->wOpCode != 4)
		{
			rc = _PacketEncoder->RxDecrypt(packet);
			if (0 != rc)
			{
				Logger::Log("Session[%X] RxDecrypt Error[%d]\n", this, rc);
				return rc;
			}
		}

		IncreasePacketRecv();

		_NetworkRef->PostNetEventMessage((WPARAM)NETEVENT_RECV, (LPARAM)this);


	} while (0 == rc);

	rc = PostRecv();
	if (0 != rc)
	{
		return rc;
	}

	return 0;
}

int Connections::CompleteSend(DWORD TransferedBytes)
{
	UNREFERENCED_PARAMETER(TransferedBytes);

	FUNCTION_BEGIN();


	IncreaseBytesSend(TransferedBytes);


	_mutexSend.Lock();
	_sendBuffer.IncreasePopPos(TransferedBytes);

	int rc = 0;
	if (IsSetControlFlag(CONTROL_FLAG_USE_SEND_QUEUE))
	{
		Packet * SendPacket = _sendQueue.PeekPacket();
		while (SendPacket)
		{
			if (_sendBuffer.GetPushAvailableSize() <= SendPacket->GetUsedSize())
			{
				break;
			}

			if (_PacketEncoder)
			{
				rc = _PacketEncoder->TxEncrypt(*SendPacket);
				if (0 != rc)
				{
					_mutexSend.Unlock();

					Logger::Log("Session[%X] TxEncrypt Error[%d]\n", this, rc);
					return 1; //ERR_NET_PACKET_ENCRYPT_FAIL
				}
			}

			memcpy(_sendBuffer.GetQueuePushPtr(), SendPacket->GetPacketBuffer(), SendPacket->GetUsedSize());
			_sendBuffer.IncreasePushPos(SendPacket->GetUsedSize());
			IncreasePacketSend();

			_sendQueue.PopPacket();
			SAFE_DELETE(SendPacket);

			SendPacket = _sendQueue.PeekPacket();
		}
	}


	if (_sendBuffer.GetCurSize())
	{
		rc = PostSend();
		if (0 != rc)
		{
			_IsSending = false;
			_mutexSend.Unlock();
			Disconnect(false);

			Logger::Log("Session[%X] PostSend Error[%d]\n", this, rc);
			return 0;
		}
	}
	else
	{
		_IsSending = false;
	}


	_mutexSend.Unlock();

	return 0;
}

int Connections::CompleteAccept(DWORD TransferedBytes)
{
	FUNCTION_BEGIN();

	UNREFERENCED_PARAMETER(TransferedBytes);

	SOCKADDR_IN* LocalAddr = NULL;
	SOCKADDR_IN* RemoteAddr = NULL;
	int LocalAddrLen = 0;
	int RemoteAddrLen = 0;

	_socket.GetAcceptExSockaddrs(_recvContext.wsabuf.buf,
		0,
		sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16,
		(SOCKADDR**)&LocalAddr,
		&LocalAddrLen,
		(SOCKADDR**)&RemoteAddr,
		&RemoteAddrLen);

	SetAddress(LocalAddr, RemoteAddr);
	ZeroMemory(_recvContext.wsabuf.buf, sizeof(SOCKADDR_IN) + 16 + sizeof(SOCKADDR_IN) + 16);
	_IsConnected = true;

	int rc = _NetworkRef->Associate(this, true);
	if (0 != rc)
	{
		Logger::Log("Session[%X] Associate Error[%d]\n", this, rc);
		return rc;
	}

	SetStatus(CONNSTATUS_ACTIVE);
	_ConnectTime = GetTickCount();

	Logger::Log("Client connected %s:%d [%d]\n", GetRemoteIP(), GetRemotePort(), this);

	_AcceptorRef->OnAccepted();

	_NetworkRef->PostNetEventMessage((WPARAM)NETEVENT_ACCEPT, (LPARAM)this);

	rc = PostRecv();
	if (0 != rc)
	{
		return rc;
	}

	return 0;
}

int Connections::CompleteConnect(DWORD TransferedBytes)
{
	FUNCTION_BEGIN();

	UNREFERENCED_PARAMETER(TransferedBytes);

	_socket.GetPeerAddr(_remoteAddr);
	_socket.GetLocalAddr(_localAddr);
	_IsConnected = true;

	int rc = _NetworkRef->Associate(this, true);
	if (0 != rc)
	{
		Logger::Log("Session[%X] CompleteConnect Error[%d]\n", this, rc);
		return rc;
	}

	SetStatus(CONNSTATUS_ACTIVE);
	_ConnectTime = GetTickCount();
	_ConnectorRef->OnConnected();
	_NetworkRef->PostNetEventMessage((WPARAM)NETEVENT_CONNECT, (LPARAM)this);


	rc = PostRecv();
	if (0 != rc)
	{
		return rc;
	}

	return 0;
}

void Connections::SetAddress(SOCKADDR_IN* LocalAddr, SOCKADDR_IN* RemoteAddr)
{
	FUNCTION_BEGIN();
	_localAddr = *LocalAddr;
	_remoteAddr = *RemoteAddr;
}

bool Connections::PopPacket(Packet* pPacket)
{
	FUNCTION_BEGIN();

	if (NULL == pPacket)
	{
		Logger::Log("NULL == pPacket\n");
		return false;
	}

	int PacketLen = GetPacketLen(_recvBuffer.GetQueuePopPtr());

	pPacket->AttachData(_recvBuffer.GetQueuePopPtr(), (WORD)(GetHeaderSize() + PacketLen));

	if (false == pPacket->IsValidPacket())
	{
		return false;
	}

	return true;
}

bool Connections::IsValidPacket(void* PacketHeader, WORD PacketLength)
{
	if (GetHeaderSize() > PacketLength)
	{
		return false;
	}

	Packet packet;
	packet.AttachData((BYTE*)PacketHeader, PacketLength);

	return packet.IsValidPacket();
}

int Connections::PushPacket(void* pData, int nSize)
{
	Packet* packet = new Packet((unsigned char*)pData, nSize);
	return PushPacket(packet);
}

int Connections::PushPacket(Packet* pPacket)
{
	FUNCTION_BEGIN();

	if (false == IsStatus(CONNSTATUS_ACTIVE))
	{
		return 1;//ERR_NET_CONNECTION_STATUS_WRONG
	}


	if (false == pPacket->IsValidPacket())
	{
		return 2;//ERR_NET_PACKET_INVALID
	}

	_mutexSend.Lock();

	if (pPacket->GetPacketLen() > GetMaxSendPacketSize())
	{
		_mutexSend.Unlock();
		return 3;//ERR_NET_PACKET_EXCEED_ALLOWED_SIZE
	}


	if (_sendBuffer.GetPushAvailableSize() <= pPacket->GetUsedSize())
	{
		if (false == IsSetControlFlag(CONTROL_FLAG_USE_SEND_QUEUE))
		{
			_mutexSend.Unlock();
			return 4;//ERR_NET_SESSION_SEND_BUFFER_OVERFLOW
		}
		else
		{
			Packet * QueuedPacket = new Packet(*pPacket);
			if (NULL == QueuedPacket)
			{
				_mutexSend.Unlock();
				return 5;//ERR_SYS_MEMORY_ALLOC_FAIL
			}

			if (false == _sendQueue.PushPacket(QueuedPacket))
			{
				SAFE_DELETE(QueuedPacket);

				_mutexSend.Unlock();
				return 6;//ERR_NET_SESSION_SEND_BUFFER_OVERFLOW;
			}

			_mutexSend.Unlock();
			return 0;
		}
	}

	memcpy(_sendBuffer.GetQueuePushPtr(), pPacket->GetPacketBuffer(), pPacket->GetUsedSize());

	Packet sendPacket(_sendBuffer.GetQueuePushPtr());

	int rc = 0;

	if (_PacketEncoder)
	{
		rc = _PacketEncoder->TxEncrypt(sendPacket);
		if (0 != rc)
		{
			_mutexSend.Unlock();
			return 7;//ERR_NET_PACKET_ENCRYPT_FAIL
		}
	}

	_sendBuffer.IncreasePushPos(pPacket->GetUsedSize());
	IncreasePacketSend();

	if (false == _IsSending)
	{
		rc = PostSend();
		if (0 != rc)
		{
			_mutexSend.Unlock();
			return rc;
		}

		_IsSending = true;
	}

	_mutexSend.Unlock();

	return 0;
}

bool Connections::ValidCheck(DWORD TickDiff)
{
	if (IsSetControlFlag(CONTROL_FLAG_CHECK_ALIVE))
	{
		_AliveTime += TickDiff;

		if (_AliveTime >= GetAliveCheckTime())
		{
			return false;
		}
	}

	return true;
}

bool Connections::IsShutdownable()
{
	if (IsStatus(CONNSTATUS_CLOSE))
	{
		if (0 == InterlockedCompareExchange((LONG*)&_IoPostCount, 0, 0))
		{
			return true;
		}
	}

	return false;
}

bool Connections::GetAddressInfo(GameString* String, WORD * Port, bool Remote)
{
	if (Remote)
	{
		String->GetString().assign(GetRemoteAddr().GetDottedAddr());
		*Port = GetRemoteAddr().GetPort();
	}
	else
	{
		String->GetString().assign(GetLocalAddr().GetDottedAddr());
		*Port = GetLocalAddr().GetPort();
	}

	return true;
}

int Connections::GetPacketLen(BYTE* HeaderPointer)
{
	PACKETHEADER* Header = (PACKETHEADER*)HeaderPointer;
	return Header->wPacketLen;
}