#include "stdafx.h"
#include "Network.h"
#include "Session.h"
#include "Acceptor.h"
#include "Connector.h"
#include "SessionFactory.h"
#include "NetworkProcessor.h"
#include "Thread.h"
#include "Queue.h"

template <class DESTTYPE, class SRCTYPE> DESTTYPE check_cast(SRCTYPE ptr)
{
	return static_cast<DESTTYPE>(ptr);
}

const ULONG_PTR THREAD_CLOSE = (ULONG_PTR)(-1);	// thread terminate value

class NetworkMonitor : public RunObject
{
public:
	NetworkMonitor(Network* pNetwork) { SetArg(pNetwork); }
	void Run()
	{
		Network* pNetwork = (Network*)GetArg();
		if (NULL == pNetwork)
		{
			Logger::Log("fail : NULL == pNetwork\n");
			return;
		}

		DWORD TickCur = 0;
		DWORD TickOld = 0;
		DWORD TickDiff = 0;

		TickCur = TickOld = ::GetTickCount();

		while (IsRunnable())
		{
			Wait(1000);

			TickCur = ::GetTickCount();
			TickDiff = TickCur - TickOld;
			TickOld = TickCur;

			if (NULL == pNetwork->GetSessionList())
			{
				continue;
			}

			pNetwork->GetSessionList()->ValidCheck(TickDiff);
		}
	}

};

Network::Network()
{
	Init();
}

Network::~Network()
{
	Destroy();
}

int	Network::Create(SessionFactory* Factory, int SessionSize, int CreateThreads, int ConcurrentThreads)
{
	if (NULL == Factory)
	{
		Logger::Log("(NULL == pFactory)\n");
		return 1;//ERR_SYS_INPUT_PARAMETER_WRONG
	}


	int rc = StartUp();
	if (0 != rc)
	{
		Logger::Log("StartUp() failed.(0 != rc) rc = %d\n", rc);
		return rc;
	}

	_SessionList = new SessionList;
	if (NULL == _SessionList)
	{
		Logger::Log("\"new SessionList\" failed.\n");
		return 2;//ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	_AcceptorList = new AcceptorList;
	if (NULL == _AcceptorList)
	{
		Logger::Log("\"new AcceptorList\" failed.\n");
		return 2;//ERR_SYS_MEMORY_ALLOC_FAIL
	}


	_ConnectorList = new ConnectorList;
	if (NULL == _ConnectorList)
	{
		Logger::Log("\"new ConnectorList\" failed.\n");
		return 2;//ERR_SYS_MEMORY_ALLOC_FAIL;
	}


	rc = _SessionList->Create(this, SessionSize, SessionSize / 10);
	if (0 != rc)
	{
		Logger::Log("_SessionList->Create( this, SessionSize, SessionSize / 10 ) failed.(0 != rc) rc = %d\n", rc);
		return rc;
	}


	rc = _iocp.Create(this, CreateThreads, ConcurrentThreads);
	if (0 != rc)
	{
		Logger::Log("_iocp.Create(this, CreateThreads, ConcurrentThreads ) failed.(0 != rc) rc = %d\n", rc);
		return rc;
	}

	rc = CreateMonitorThread();
	if (0 != rc)
	{
		Logger::Log("CreateMonitorThread() failed.(0 != rc) rc = %d\n", rc);
		return rc;
	}

	rc = CreateDispatcherThread();
	if (0 != rc)
	{
		Logger::Log("CreateDispatcherThread() failed.(0 != rc) rc = %d\n", rc);
		return rc;
	}

	_SessionFactoryRef = Factory;

	return 0;
}

void Network::Destroy()
{
	SAFE_DELETE(_SessionList);

	if (NULL != _AcceptorList)
	{
		for (AcceptorList::iterator it = _AcceptorList->begin(); it != _AcceptorList->end(); it++)
		{
			SAFE_DELETE(it->second);
		}
		SAFE_DELETE(_AcceptorList);
	}

	if (NULL != _ConnectorList)
	{
		for (ConnectorList::iterator it = _ConnectorList->begin(); it != _ConnectorList->end(); it++)
		{
			SAFE_DELETE(it->second);
		}
		SAFE_DELETE(_ConnectorList);
	}
}

void Network::Init()
{
	_SessionFactoryRef = NULL;
	_NetworkMonitor = NULL;
	_NetworkProcessor = NULL;
	_SessionList = NULL;
	_AcceptorList = NULL;
	_ConnectorList = NULL;
}

int Network::StartUp()
{
	int rc = Socket::StartUp();
	if (0 != rc)
	{
		Logger::Log("Socket::StartUp() failed.(0 != rc) rc = %d\n", rc);
		return rc;
	}

	return 0;
}

int Network::Shutdown()
{
	int rc = Socket::CleanUp();
	if (0 != rc)
	{
		Logger::Log("Socket::CleanUp() failed.(0 != rc) rc = %d\n", rc);
		return rc;
	}

	return 0;
}

int Network::CreateMonitorThread()
{
	GameString strName;
	strName.Format("Network Monitor");

	_NetworkMonitor = new NetworkMonitor(this);
	if (NULL == _NetworkMonitor)
	{
		Logger::Log("\"new NetworkMonitor(this)\" failed.\n");
		return 1;//ERR_SYS_MEMORY_ALLOC_FAIL
	}

	Thread * pThread = ThreadFactory::CreateThread(_NetworkMonitor, strName.c_str(), true);
	if (NULL == pThread)
	{
		Logger::Log("ThreadFactory::CreateThread( _NetworkMonitor, strName, false) failed.(NULL == pThread)\n");
		SAFE_DELETE(_NetworkMonitor);
		return 2;//ERR_NET_THREAD_CREATE_FAIL
	}

	pThread->Start();

	return 0;
}

int Network::CreateDispatcherThread()
{
	GameString strName;
	strName.Format("Network Processor");

	_NetworkProcessor = new NetworkProcessor(this);
	if (NULL == _NetworkProcessor)
	{
		Logger::Log("\"new NetworkProcessor( this )\" failed.\n");
		return 1;//ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	int rc = _NetworkProcessor->Create();
	if (0 != rc)
	{
		Logger::Log("_NetworkProcessor->Create() failed.(0 != rc) rc = %d\n", rc);
		return 2;//ERR_NET_THREAD_CREATE_FAIL
	}

	Thread* pThread = ThreadFactory::CreateThread(_NetworkProcessor, strName.c_str(), true);
	if (NULL == pThread)
	{
		Logger::Log("ThreadFactory::CreateThread( _NetworkProcessor, strName, false ) failed.(NULL == pThread)\n");
		SAFE_DELETE(_NetworkProcessor);
		return 2;//ERR_NET_THREAD_CREATE_FAIL
	}

	pThread->Start();

	return 0;
}

int Network::Associate(Connections* pConnection, bool IsAssociate)
{
	Session* pSession = check_cast<Session*>(pConnection);
	if (NULL == pSession)
	{
		Logger::Log("(NULL == pSession) pConnection = %016x\n", pConnection);
		return 1;//ERR_SYS_INPUT_PARAMETER_WRONG
	}

	if (IsAssociate)
	{
		if (false == _SessionList->Add(pSession))
		{
			Logger::Log("_SessionList->Add( pSession ) failed.\n");
			return 2;//ERR_NET_CONNECTION_ADD_FAIL
		}

		int rc = _iocp.Associate(pSession->GetSocket().GetRawSocket(), pSession);
		if (0 != rc)
		{
			Logger::Log("_iocp.Associate( pSession->GetSocket().GetRawSocket(), pSession ) failed.(0 != rc), rc = %d\n", rc);
			return rc;
		}
	}
	else
	{
		if (false == _SessionList->Remove(pSession))
		{
			Logger::Log("_SessionList->Remove( pSession ) failed.\n");
			return 2;//ERR_NET_CONNECTION_ADD_FAIL;
		}
	}

	return 0;
}

int Network::Associate(Acceptor* pAcceptor, bool IsAssociate)
{
	if (NULL == pAcceptor)
	{
		Logger::Log("(NULL == pAcceptor)\n");
		return 1;//ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	if (IsAssociate)
	{
		AcceptorList::iterator it = _AcceptorList->insert(AcceptorList::value_type(pAcceptor->GetSessionType(), pAcceptor));
		if (_AcceptorList->end() == it)
		{
			Logger::Log("_AcceptorList->insert( AcceptorList::value_type( pAcceptor->GetSessionType(), pAcceptor ) ) failed.(_AcceptorList->end() == it), pAcceptor->GetSessionType() = %u\n", pAcceptor->GetSessionType());
			return 2;//ERR_NET_ACCEPTOR_ALREADY_CREATED;
		}


		int rc = pAcceptor->OnAssociated(this);
		if (0 != rc)
		{
			Logger::Log("pAcceptor->OnAssociated( this ) failed.(0 != rc), rc = %d\n", rc);
			return rc;
		}

		rc = _iocp.Associate(pAcceptor->GetListenSocket().GetRawSocket(), pAcceptor);
		if (0 != rc)
		{
			Logger::Log("_iocp.Associate( pAcceptor->GetListenSocket().GetRawSocket(), pAcceptor ) failed.(0 != rc), rc = %d\n", rc);
			return rc;
		}

	}

	return 0;
}

int Network::Associate(Connector* pConnector, bool IsAssociate)
{
	if (NULL == pConnector)
	{
		Logger::Log("(NULL == pConnector)\n");
		return 1;//ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	if (IsAssociate)
	{
		ConnectorList::iterator it = _ConnectorList->insert(ConnectorList::value_type(pConnector->GetSessionType(), pConnector));
		if (_ConnectorList->end() == it)
		{
			Logger::Log("_ConnectorList->insert( ConnectorList::value_type(pConnector->GetSessionType(), pConnector) ) failed.(_ConnectorList->end() == it), pConnector->GetSessionType() = %u\n", pConnector->GetSessionType());
			return 2;//ERR_NET_CONNECTOR_ALREADY_CREATED;
		}


		int rc = pConnector->OnAssociated(this);
		if (0 != rc)
		{
			Logger::Log("pConnector->OnAssociated() failed.(0 != rc) rc = %d\n", rc);
			return rc;
		}
	}

	return 0;
}

Acceptor* Network::GetAcceptor(unsigned short sessionType, const char* Addr, WORD Port)
{
	AcceptorList::iterator beg = _AcceptorList->lower_bound(sessionType);
	AcceptorList::iterator end = _AcceptorList->upper_bound(sessionType);

	for (AcceptorList::iterator it = beg; it != end; it++)
	{
		Acceptor* pAcceptor = it->second;
		if (NULL == pAcceptor)
		{
			Logger::Log("The acceptor object is NULL in _AcceptorList.(NULL == pAcceptor) sessionType = %u\n", sessionType);
			continue;
		}

		if (0 != strncmp(pAcceptor->GetListenIP(), Addr, strlen(Addr)))
			continue;

		if (Port == pAcceptor->GetListenPort())
			return pAcceptor;
	}

	return NULL;
}

Connector * Network::GetConnector(unsigned short sessionType, const char* Addr, WORD Port)
{
	ConnectorList::iterator beg = _ConnectorList->lower_bound(sessionType);
	ConnectorList::iterator end = _ConnectorList->upper_bound(sessionType);

	for (ConnectorList::iterator it = beg; it != end; it++)
	{
		Connector* pConnector = it->second;
		if (NULL == pConnector)
		{
			Logger::Log("The connector object is NULL in _ConnectorList.(NULL == pConnector) sessionType = %u\n", sessionType);
			continue;
		}

		if (0 != strncmp(pConnector->GetConnectIP(), Addr, strlen(Addr)))
			continue;

		if (Port == pConnector->GetConnectPort())
			return pConnector;
	}

	return NULL;
}

int Network::Send(unsigned int hSession, Packet* pPacket)
{
	if (NULL == pPacket)
	{
		Logger::Log("(NULL == pPacket)\n");
		return 1;//ERR_NET_PACKET_INVALID;
	}

	if (NULL == _SessionList)
	{
		return 2;//ERR_NET_NETWORK_NOT_CREATED;
	}

	Session* pSession = _SessionList->Find(hSession);
	if (NULL == pSession)
	{
		return 3;//ERR_NET_CONNECTION_NOT_EXIST;
	}

	int rc = pSession->PushPacket(pPacket);
	if (0 != rc)
	{
		pSession->Disconnect(false);
		Logger::Log("Session[%X] PushPacket Error : Err:%d\n", pSession, rc);
		return rc;
	}

	return 0;
}

int Network::Send(Session* pSession, Packet* pPacket)
{
	if (NULL == pSession)
	{
		return 1;//ERR_NET_CONNECTION_NOT_EXIST;
	}

	if (NULL == pPacket)
	{
		Logger::Log("(NULL == pPacket)\n");
		return 2;//ERR_NET_PACKET_INVALID;
	}

	int rc = pSession->PushPacket(pPacket);
	if (0 != rc)
	{
		pSession->Disconnect(false);
		Logger::Log("Session[%X] PushPacket Error : Err:%d\n", pSession, rc);
		return rc;
	}

	return 0;
}

int Network::PostNetEventMessage(WPARAM wParam, LPARAM lParam)
{
	return _NetworkProcessor->PostNetEvent(wParam, lParam);
}