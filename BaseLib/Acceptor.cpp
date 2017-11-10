#include "stdafx.h"
#include "Acceptor.h"

#include "Session.h"
#include "SessionList.h"
#include "SessionFactory.h"
#include "Network.h"

#include "Logger.h"
#include "Thread.h"

const unsigned int ACCEPT_RESERVE_CHECK_TIME = 1;

class AcceptorThread : public RunObject
{
public:
	AcceptorThread(Acceptor* pAcceptor) { SetArg(pAcceptor); }
	void Run()
	{
		Acceptor* pAcceptor = (Acceptor*)GetArg();

		int rc = pAcceptor->ReserveAccept(pAcceptor->_CreateAcceptCount);
		if (0 != rc)
		{
			Logger::Log("%s Precreate ReserveAccept Fail :%d\n", GetName(), rc);
		}

		while (IsRunnable())
		{
			Wait(ACCEPT_RESERVE_CHECK_TIME);

			int AcceptCount = pAcceptor->GetReserveAcceptCount();
			if (AcceptCount > 0)
			{
				rc = pAcceptor->ReserveAccept(AcceptCount);
				if (0 != rc)
				{
					Logger::Log("%s ReserveAccept Fail :%d\n", GetName(), rc);
				}
			}

		}

	} 
};

Acceptor::Acceptor()
{
	Init();
}

Acceptor::~Acceptor()
{
	Destroy();
}

void Acceptor::Init()
{
	_Network = NULL;
	_Thread = NULL;
	_MaxAcceptCount = 0;
	_MinAcceptCount = 0;
	_PostAcceptCount = 0;
	_CreateAcceptCount = 0;
	_AcceptingCount = 0;
	_AcceptedCount = 0;
	_TotalAcceptCount = 0;
}

int Acceptor::Create(const char* Addr, unsigned short port, unsigned short sessionType, int MaxAcceptCount, int PostAcceptCount, int MinAcceptCount, int CreateAcceptCount)
{
	if (CreateAcceptCount > MaxAcceptCount)
	{
		Logger::Log("CreateAcceptCount > MaxAcceptCount, CreateAcceptCount = %d, MaxAcceptCount = %d\n", CreateAcceptCount, MaxAcceptCount);
		return  1;//ERR_SYS_INPUT_PARAMETER_WRONG
	}


	if (NULL != _Network)
	{
		Logger::Log("NULL != _Network, _Network = %016x\n", _Network);
		return 2;//ERR_SYS_OBJECT_ALREADY_CREATED
	}

	_sessionType = sessionType;
	_MaxAcceptCount = MaxAcceptCount;
	_PostAcceptCount = PostAcceptCount;
	_MinAcceptCount = MinAcceptCount;
	_CreateAcceptCount = CreateAcceptCount;
	_listenAddr.SetSocketAddr(Addr, htons(port));

	int rc = Listen();
	if (0 != rc)
	{
		Logger::Log("Listen() failed.(0 != rc), rc = %d\n", rc);
		return rc;
	}

	return 0;
}

void Acceptor::Destroy()
{
	_listenSocket.Close();
}

int Acceptor::Listen()
{
	int rc = _listenSocket.Create();
	if (0 != rc)
	{
		Logger::Log("_listenSocket.Create() failed.(0 != rc), rc = %d\n", rc);
		return rc;
	}

	rc = _listenSocket.Bind(_listenAddr);
	if (0 != rc)
	{
		Logger::Log("_listenSocket.Bind() failed.(0 != rc), rc = %d\n", rc);
		return rc;
	}

	rc = _listenSocket.SetReuseAddr(TRUE);
	if (0 != rc)
	{
		Logger::Log("_listenSocket.SetReuseAddr() failed.(0 != rc), rc = %d\n", rc);
		return rc;
	}

	rc = _listenSocket.Listen();
	if (0 != rc)
	{
		Logger::Log("_listenSocket.Listen() failed.(0 != rc), rc = %d\n", rc);
		return rc;
	}

	return 0;
}

int Acceptor::OnAssociated(Network* pNetwork)
{
	if (NULL == pNetwork)
	{
		Logger::Log("NULL == pNetwork\n");
		return 1;//ERR_SYS_INPUT_PARAMETER_WRONG
	}

	if (NULL != _Network)
	{
		Logger::Log("NULL != _Network, _Network = %016x\n", _Network);
		return 2;//ERR_NET_ACCEPTOR_ASSOCIATE_FAIL
	}

	_Network = pNetwork;

	int rc = CreateThread();
	if (0 != rc)
	{
		Logger::Log("CreateThread() failed.(0 != rc), rc = %d\n", rc);
		return rc;
	}

	return 0;
}

int Acceptor::CreateThread()
{
	GameString strName;
	strName.Format("AcceptorThread [%s:%u] Type[%u]", _listenAddr.GetDottedAddr(), _listenAddr.GetPort(), _sessionType);

	_Thread = new AcceptorThread(this);
	if (NULL == _Thread)
	{
		Logger::Log("\"new CAcceptorThread( this )\" failed.\n");
		return 1;//ERR_SYS_MEMORY_ALLOC_FAIL
	}

	Thread* pThread = ThreadFactory::CreateThread(_Thread, strName.c_str(), true);
	if (NULL == pThread)
	{
		Logger::Log("NULL == pThread\n");
		SAFE_DELETE(_Thread);
		return 2;//ERR_NET_THREAD_CREATE_FAIL
	}

	pThread->Start();

	return 0;
}

int Acceptor::ReserveAccept(int ReserveCount)
{
	if (NULL == _Network)
	{
		Logger::Log("NULL == m_pNetwork\n");
		return 1;//ERR_SYS_INPUT_PARAMETER_WRONG
	}


	for (int i = 0; i < ReserveCount; i++)
	{
		Session* pSession = _Network->GetSessionFactory()->CreateSession(_sessionType);
		if (NULL == pSession)
		{
			return 1;//ERR_NET_CONNECTION_POOL_ALLOC_FAIL
		}

		int rc = pSession->Create(_Network);
		if (0 != rc)
		{
			Logger::Log("pSession->Create() failed(0 != rc), rc = %d\n", rc);
			RELEASE_SESSION(pSession);
			return rc;
		}

		rc = pSession->PostAccept(this);
		if (0 != rc)
		{
			Logger::Log("pSession->PostAccept() failed(NTL_SUCCESS != rc), rc = %d\n", rc);
			RELEASE_SESSION(pSession);
			return rc;
		}
	}

	return 0;
}

int Acceptor::GetReserveAcceptCount()
{
	AutoMutex mutex(&_mutex);
	mutex.Lock();

	if (_AcceptingCount < _MinAcceptCount)
	{
		int AcceptDiff = _PostAcceptCount - _AcceptingCount;
		int AvailableAccept = _MaxAcceptCount - (_AcceptingCount + _AcceptedCount);

		return min(AvailableAccept, AcceptDiff);
	}

	return 0;
}
