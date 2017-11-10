#include "stdafx.h"
#include "Connector.h"
#include "Network.h"
#include "Session.h"
#include "SessionFactory.h"
#include "Thread.h"

class ConnectorThread : public RunObject
{
public:
	ConnectorThread(Connector* pConnector) { SetArg(pConnector); }
	void Run()
	{
		Connector* pConnector = (Connector*)GetArg();

		while (IsRunnable())
		{
			if (TRUE == InterlockedCompareExchange((LONG*)& pConnector->_IsConnected, TRUE, TRUE))
			{
				Wait(pConnector->_ProcessTime);
			}
			else
			{
				Logger::Log("%s Connector Try Connect\n", GetName());

				int rc = pConnector->DoConnect();
				if (0 != rc)
				{
					Logger::Log("%s Connector Connect Fail :%d\n", GetName(), rc);
					Wait(pConnector->_RetryTime);
				}
				else
				{
					Logger::Log("%s Connector Connect Success\n", GetName());
				}
			}

		}
	}
};

Connector::Connector()
{
	Init();
}

Connector::~Connector()
{
	Destroy();
}

void Connector::Init()
{
	_Network = NULL;
	_Thread = NULL;
	_RetryTime = 0;
	_ProcessTime = 0;
	_TotalConnectCount = 0;
	_IsConnected = FALSE;
}

int Connector::Create(LPCTSTR ConnectAddr, WORD ConnectPort, unsigned short sessionType, DWORD RetryTime, DWORD ProcessTime)
{
	_connectAddr.SetSocketAddr(ConnectAddr, htons(ConnectPort));
	_sessionType = sessionType;
	_ProcessTime = ProcessTime;
	_RetryTime = RetryTime;
	return 0;
}

void Connector::Destroy()
{
}

int Connector::CreateThread()
{
	GameString strName;
	strName.Format("ConnectorThread [%s:%u] Type[%u]", _connectAddr.GetDottedAddr(), _connectAddr.GetPort(), _sessionType);

	_Thread = new ConnectorThread(this);
	if (NULL == _Thread)
	{
		Logger::Log("\"new ConnectorThread( this )\" failed.\n");
		return 1;//ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	Thread* pThread = ThreadFactory::CreateThread(_Thread, strName.c_str(), true);
	if (NULL == pThread)
	{
		Logger::Log("ThreadFactory::CreateThread( _Thread, strName, false ) failed.(NULL == pThread)\n");
		SAFE_DELETE(_Thread);
		return 2;//ERR_NET_THREAD_CREATE_FAIL;
	}

	pThread->Start();

	return 0;
}

int Connector::DoConnect()
{
	Session * pSession = _Network->GetSessionFactory()->CreateSession(_sessionType);
	if (NULL == pSession)
	{
		return 1;//ERR_NET_CONNECTION_POOL_ALLOC_FAIL;
	}

	int rc = pSession->Create(_Network);
	if (0 != rc)
	{
		RELEASE_SESSION(pSession);
		return rc;
	}

	rc = pSession->PostConnect(this);
	if (0 != rc)
	{
		RELEASE_SESSION(pSession);
		return rc;
	}


#if !defined( __USE_CONNECTEX__ )
	rc = pSession->CompleteConnect(_sessionType);
	if (0 != rc)
	{
		Logger::Log("Session[%X] CompleteConnect Error : Err:%d\n", pSession, rc);
		pSession->Close();
		return rc;
	}
#endif

	return 0;
}

int Connector::OnAssociated(Network* pNetwork)
{
	if (NULL == pNetwork)
	{
		Logger::Log("(NULL == pNetwork)\n");
		return 1;//ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	if (NULL != _Network)
	{
		Logger::Log("(NULL != _Network) _Network = %016x\n", _Network);
		return 2;//ERR_SYS_OBJECT_ALREADY_CREATED;
	}

	_Network = pNetwork;

	int rc = CreateThread();
	if (0 != rc)
	{
		Logger::Log("CreateThread() failed.(0 != rc) rc = %d\n", rc);
		return 3;//ERR_NET_THREAD_CREATE_FAIL;
	}

	return 0;
}
