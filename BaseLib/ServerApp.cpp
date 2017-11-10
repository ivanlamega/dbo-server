#include "stdafx.h"
#include "ServerApp.h"
#include "SessionFactory.h"
#include "GameString.h"
#include "Logger.h"

ServerApp::ServerApp()
{
	Init();
}

ServerApp::~ServerApp()
{
	Destroy();
}

void ServerApp::Init()
{
	_SessionFactory = NULL;
	_MaxSessionCount = 0;
	Logger::StartLog();
}

void ServerApp::Destroy()
{
	ThreadFactory::CloseAll();
	SAFE_DELETE(_SessionFactory);
}

int ServerApp::Create(int argc, char* argv[])
{
	int rc = 0;

	SetConfigFileName(argv[0]);
	rc = ServerApp::OnConfiguration(_strConfigFile.c_str());
	if (0 != rc)
	{
		return rc;
	}
	rc = OnConfiguration(_strConfigFile.c_str());
	if (0 != rc)
	{
		return rc;
	}

	rc = ServerApp::OnCommandArgument(argc, argv);
	if (0 != rc)
	{
		return rc;
	}
	rc = OnCommandArgument(argc, argv);
	if (0 != rc)
	{
		return rc;
	}

	rc = ServerApp::OnInitApp();
	if (0 != rc)
	{
		return rc;
	}
	rc = OnInitApp();
	if (0 != rc)
	{
		return rc;
	}

	rc = ServerApp::OnCreate();
	if (0 != rc)
	{
		return rc;
	}
	rc = OnCreate();
	if (0 != rc)
	{
		return rc;
	}

	return 0;
}

int	ServerApp::OnInitApp()
{
	return 0;
}

int	ServerApp::OnCreate()
{
	int rc = _network.Create(_SessionFactory, _MaxSessionCount);
	if (0 != rc)
	{
		return rc;
	}

	return 0;
}

int ServerApp::OnCommandArgument(int argc, char* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	return 0;
}

int ServerApp::OnConfiguration(const char * lpszConfigFile)
{
	UNREFERENCED_PARAMETER(lpszConfigFile);

	return 0;
}

int ServerApp::OnAppStart()
{
	return 0;
}

void ServerApp::OnRun()
{
	while (IsRunnable())
	{
		Logger::Log("Run\n");
	}
}

void ServerApp::Start(bool IsAutoDelete)
{
	if (NULL == _SessionFactory)
	{
		Logger::Log("NULL == m_pSessionFactory\n");
	}

	_AppThread = ThreadFactory::CreateThread(this, "ServerAppThread", IsAutoDelete);
	_AppThread->Start();

	OnAppStart();
}

void ServerApp::Stop()
{
	ThreadFactory::CloseAll();
}

void ServerApp::WaitForTerminate()
{
	ThreadFactory::JoinAll();
}

void ServerApp::SetConfigFileName(const char*AppName)
{
	std::string::size_type pos;
	_strConfigFile = AppName;

	pos = _strConfigFile.rfind(".");
	_strConfigFile.erase(pos);
	pos = _strConfigFile.rfind("\\");
	_strConfigFile.erase(0, pos + 1);
}
