#include "stdafx.h"
#include "Iocp.h"
#include "Thread.h"
#include "Session.h"

const ULONG_PTR THREAD_CLOSE = (ULONG_PTR)(-1);

class IocpWorkerThread : public RunObject
{
public:
	IocpWorkerThread(Iocp * pIOCP) { SetArg(pIOCP); }
	void Run()
	{
		Iocp* pIOCP = (Iocp*)GetArg();

		int rc = 0;
		BOOL Result = FALSE;
		DWORD BytesTransferred = 0;

		while (IsRunnable())
		{
			Session* pSession = NULL;
			IOCONTEXT* IOContext = NULL;

			Result = GetQueuedCompletionStatus(pIOCP->_hIOCP,
				&BytesTransferred,
				(ULONG_PTR*)&pSession,
				(LPOVERLAPPED*)&IOContext,
				INFINITE);


			if (THREAD_CLOSE == (ULONG_PTR)pSession)
			{
				Logger::Log("Thread Close\n");
				break;
			}


			if (NULL == IOContext)
			{
				Logger::Log("NULL == IOContext\n");
				continue;
			}


			pSession = (Session*)IOContext->param;
			if (NULL == pSession)
			{
				Logger::Log("IOContext->param is NULL.(NULL == pSession)\n");
				continue;
			}


			if (FALSE == Result)
			{
				rc = GetLastError();
				//Logger::Log("Session[%X] GQCS Error : Err:%d\n", pSession, rc);
				pSession->Close(true);
			}
			else
			{
				rc = pSession->CompleteIO(IOContext, BytesTransferred);
				if (0 != rc)
				{
					//Logger::Log("Session[%X] CompleteIO Error : Err:%d\n", pSession, rc);
					pSession->Close();
				}
			}

			pSession->DecreasePostIoCount();

		}
	}

	void Close()
	{
		Iocp* pIocp = (Iocp*)GetArg();
		if (pIocp)
		{
			PostQueuedCompletionStatus(pIocp->_hIOCP, 0, THREAD_CLOSE, NULL);
		}

		RunObject::Close();
	}
};

Iocp::Iocp()
	:_hIOCP(INVALID_HANDLE_VALUE), _CreatedThreads(0), _NetworkRef(NULL)
{
}

Iocp::~Iocp()
{
	Destroy();
}

int Iocp::Create(Network* pNetwork, int nCreateThreads, int ConcurrentThreads)
{
	if (NULL == pNetwork || NULL != _NetworkRef)
	{
		Logger::Log("(NULL == pNetwork || NULL != m_pNetworkRef) m_pNetworkRef = %016x\n", _NetworkRef);
		return -1;
	}


	_NetworkRef = pNetwork;

	if (0 == nCreateThreads)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		nCreateThreads = si.dwNumberOfProcessors * 2 + 2;
	}


	int rc = CreateIOCP(ConcurrentThreads);
	if (0 != rc)
	{
		Logger::Log("CreateIOCP( nConcurrentThreads ) failed.(NTL_SUCCESS != rc) nConcurrentThreads = %d, rc = %d\n", ConcurrentThreads, rc);
		return rc;
	}


	rc = CreateThreads(nCreateThreads);
	if (0 != rc)
	{
		Logger::Log("CreateThreads( nCreateThreads ) failed.(NTL_SUCCESS != rc) nCreateThreads = %d, rc = %d\n", nCreateThreads, rc);
		return rc;
	}


	return 0;
}

void Iocp::Destroy()
{
	CloseThreads();

	::CloseHandle(_hIOCP);
}

int Iocp::CreateIOCP(int ConcurrentThreads)
{
	if (INVALID_HANDLE_VALUE != _hIOCP)
	{
		Logger::Log("(INVALID_HANDLE_VALUE != m_hIOCP) m_hIOCP = %016x\n", _hIOCP);
		return -1;
	}

	_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, ConcurrentThreads);
	if (NULL == _hIOCP)
	{
		return GetLastError();
	}


	return 0;
}

int Iocp::CreateThreads(int OpenThreads)
{
	if (0 == OpenThreads)
	{
		Logger::Log("(0 == OpenThreads)\n");
		return -1;
	}


	for (int i = 0; i < OpenThreads; i++)
	{
		GameString strName;
		strName.Format("IOCP Worker[%03d]", i);

		IocpWorkerThread* IOCPWorker = new IocpWorkerThread(this);
		if (NULL == IOCPWorker)
		{
			Logger::Log("\"new CIocpWorkerThread( this )\" failed.\n");
			return -1;
		}

		Thread* pThread = ThreadFactory::CreateThread(IOCPWorker, strName.c_str(), true);
		if (NULL == pThread)
		{
			Logger::Log("CNtlThreadFactory::CreateThread( pIOCPWorker, strName, true ) failed.(NULL == pThread)\n");
			CloseThreads();
			SAFE_DELETE(IOCPWorker);
			return -1;
		}

		pThread->Start();

		_CreatedThreads++;
	}

	return 0;
}

void Iocp::CloseThreads()
{
	for (int i = 0; i < _CreatedThreads; i++)
	{
		PostQueuedCompletionStatus(_hIOCP, 0, THREAD_CLOSE, NULL);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int Iocp::Associate(SOCKET Sock, LPCVOID CompletionKey)
{
	if (NULL == _hIOCP)
	{
		Logger::Log("(NULL == m_hIOCP)\n");
		return -1;
	}

	HANDLE handle = CreateIoCompletionPort((HANDLE)Sock, _hIOCP, (ULONG_PTR)CompletionKey, 0);
	if (NULL == handle)
	{
		Logger::Log("I/O completion port couldn't be created.(NULL == handle) hSock = %016x, m_hIOCP = %016x, pCompletionKey = %016x\n", Sock, _hIOCP, CompletionKey);
		return GetLastError();
	}

	return 0;
}
