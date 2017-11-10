#include "stdafx.h"
#include "NetworkProcessor.h"

#include "Network.h"
#include "Session.h"
#include "Packet.h"
#include "Logger.h"

const ULONG_PTR THREAD_CLOSE = (ULONG_PTR)(-1);	// thread terminate value

NetworkProcessor::NetworkProcessor(Network* pNetwork)
	:_hEventIOCP(0)
{
	SetArg(pNetwork);
}

NetworkProcessor::~NetworkProcessor()
{
	Destroy();
}

int NetworkProcessor::Create()
{
	if (NULL != _hEventIOCP)
	{
		Logger::Log("(NULL != m_hEventIOCP) m_hEventIOCP = %016x\n", _hEventIOCP);
	}

	_hEventIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
	if (NULL == _hEventIOCP)
	{
		return GetLastError();
	}

	return 0;
}

void NetworkProcessor::Destroy()
{
	if (NULL != _hEventIOCP)
	{
		CloseHandle(_hEventIOCP);
	}
}

void NetworkProcessor::Run()
{
	Network* pNetwork = (Network*)GetArg();
	if (NULL == pNetwork)
	{
		Logger::Log("(NULL == pNetwork)\n");
		return;
	}


	BOOL Result = FALSE;
	DWORD BytesTransferred = 0;
	ULONG_PTR netEvent = INVALID_NETEVENT;
	Session* pSession = NULL;

	while (IsRunnable())
	{
		Result = GetQueuedCompletionStatus(_hEventIOCP,
			&BytesTransferred,
			(ULONG_PTR*)&netEvent,
			(LPOVERLAPPED*)&pSession,
			INFINITE);


		if (THREAD_CLOSE == (ULONG_PTR)netEvent)
		{
			Logger::Log("Thread Close\n");
			break;
		}

		if (FALSE == Result)
		{
			int rc = GetLastError();
			//Logger::Log("Dispatcher\tGQCS Failed : Err:%d\n", rc);
			continue;
		}

		if (NULL == pSession)
		{
			continue;
		}

		switch (netEvent)
		{
		case NETEVENT_ACCEPT:
		{
			BYTE rawData[8] = { 0x06, 0x00, 0x03, 0x00, 0x30, 0x2C, 0x67, 0x4C };
			pSession->GetSocket().SendStream(rawData, sizeof(rawData), false);

			int rc = pSession->OnAccept();
			if (0 != rc)
			{
				pSession->Disconnect(false);
			}

		}
		break;

		case NETEVENT_CONNECT:
		{
			int rc = pSession->OnConnect();
			if (0 != rc)
			{
				pSession->Disconnect(false);
			}
		}
		break;

		case NETEVENT_CLOSE:
		{
			static int nCount = 0;
			Logger::Log("Client disconnected %s:%d [%d]\n", pSession->GetRemoteIP(), pSession->GetRemotePort(), pSession);
			pSession->OnClose();
			RELEASE_SESSION(pSession);
		}
		break;

		case NETEVENT_RECV:
		{
			int rc = 0;
			Packet packet;
			if (pSession->PopPacket(&packet))
			{
				LPPACKETDATA pData = (LPPACKETDATA)packet.GetPacketData();

				switch (pData->wOpCode)
				{
					case SYS_ALIVE: { pSession->ResetAliveTime(); } break;
					case SYS_PING: break;
					case SYS_HANDSHAKE_RES: {
						BYTE rawData[36] =
						{
							0x22, 0x00, 0x10, 0x00, 0x49, 0xD1, 0xF1, 0x1C, 0x6D, 0x58, 0xF9, 0xC5, 0x30, 0x26, 0xA4, 0x7B,
							0xB2, 0xD8, 0x2C, 0x86, 0x58, 0x60, 0x7B, 0xDD, 0xF0, 0x77, 0xCF, 0x25, 0x48, 0xB3, 0x65, 0x45,
							0x38, 0x80, 0x14, 0x72
						};
						pSession->GetSocket().SendStream(rawData, sizeof(rawData), false);
					} break;
					default: { rc = pSession->OnDispatch(&packet); } break;
				}

				int PacketLen = pSession->GetPacketLen((BYTE*)(packet.GetPacketHeader()));
				pSession->GetRecvBuffer()->IncreasePopPos(pSession->GetHeaderSize() + PacketLen);

				if (pSession->IsSetControlFlag(CONTROL_FLAG_CHECK_OPCODE))
				{
					if (0 != rc)
					{
						pSession->Disconnect(false);
						break;
					}
				}
			}
		}
		break;

		default:
			Logger::Log("netEvent is not valid. netEvent = %d\n", netEvent);
			break;

		}

	}
}

int NetworkProcessor::PostNetEvent(WPARAM wParam, LPARAM lParam)
{
	if (NULL == _hEventIOCP)
	{
		Logger::Log("(NULL == m_hEventIOCP)\n");
	}

	if (0 == PostQueuedCompletionStatus(_hEventIOCP, 0, (ULONG_PTR)wParam, (LPOVERLAPPED)lParam))
	{
		return GetLastError();
	}

	return 0;
}

void NetworkProcessor::Close()
{
	PostQueuedCompletionStatus(_hEventIOCP, 0, THREAD_CLOSE, NULL);
	RunObject::Close();
}
