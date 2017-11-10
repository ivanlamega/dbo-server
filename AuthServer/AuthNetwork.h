#ifndef _AUTHNETWORK_H
#define _AUTHNETWORK_H

#include <iostream>
#include <fstream>
#include <Network.h>
#include <Acceptor.h>
#include <Connector.h>
#include <Session.h>
#include <SessionFactory.h>
#include <Packet.h>
#include <Config.h>
#include <ServerApp.h>
#include <Database.h>
#include <Encoder.h>
#include "AuthProtocol.h"

enum AUTH_SESSION
{
	SESSION_CLIENT,
	SESSION_SERVER_ACTIVE,
};

const DWORD	MAX_NUMOF_GAME_CLIENT = 3;
const DWORD	MAX_NUMOF_SERVER = 1;
const DWORD	MAX_NUMOF_SESSION = MAX_NUMOF_GAME_CLIENT + MAX_NUMOF_SERVER;

class AuthServer;
class AuthClientFactory;

class AuthClient : public Session
{
	friend class AuthClientFactory;
public:
	AuthClient(bool IsAliveCheck = false, bool IsOpcodeCheck = false);
	~AuthClient();

	int	OnAccept();
	void OnClose();
	int	OnDispatch(Packet* pPacket);
	void Send(void* pData, int nSize);

	// DATA Methods
	char* GenAuthKey();
	int GetDBAccountAcLevel();
	BYTE GetDBLastServerID();
	int GetDBAccountID();
	eRESULTCODE LoginVerifyAccount();

	// Opcode Control
	bool PacketControl(Packet* pPacket);

	// Protocol Resolvers
	void SendCommercialNfy();
	void SendLoginRes(sUA_LOGIN_REQ* data);
	void SendDisconnectRes(sUA_LOGIN_DISCONNECT_REQ* data);

private:
	PacketEncoder _packetEncoder;
	AuthServer* pServer;

	WCHAR userName[NTL_MAX_SIZE_USERID_UNICODE + 1];
	WCHAR passWord[NTL_MAX_SIZE_USERPW_UNICODE + 1];
	BYTE AuthKey[NTL_MAX_SIZE_AUTH_KEY];
	int AccountID;
	BYTE LastServerID;
	DWORD AcLevel;
	BYTE CurrServerID;
	BYTE CurrChannelID;
	unsigned int CurrCharID;
	bool goCharServer;
};

class AuthClientFactory : public SessionFactory
{
public:
	Session* CreateSession(unsigned short sessionType)
	{
		Session* pSession = NULL;
		switch (sessionType)
		{
		case SESSION_CLIENT:
		{
			pSession = new AuthClient;
		}
		break;

		default:
			break;
		}
		return pSession;
	}
};

class AuthServer : public ServerApp
{
public:
	int	OnInitApp();
	int	OnCreate();
	void OnDestroy() {}
	int	OnConfiguration(const char* ConfigFile);
	int OnCommandArgument(int argc, _TCHAR* argv[]) { return 0; }
	int	OnAppStart();
	void Run()
	{
		DWORD TickCur, TickOld = ::GetTickCount();

		while (IsRunnable())
		{
			TickCur = ::GetTickCount();
			if (TickCur - TickOld >= 10000)
			{
				TickOld = TickCur;
			}
		}
	}

private:
	Acceptor _clientAcceptor;

public:
	Config* ServerCfg;
	Database* ServerDB;
	int ServerID;
};

#endif