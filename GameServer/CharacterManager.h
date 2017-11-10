#ifndef _CHARACTERMANAGER
#define _CHARACTERMANAGER

#include <Base.h>
#include <Def.h>
#include <Thread.h>

#include <map>

#include "CharacterProfile.h"

class GameServer;
class GameClient;

class CharacterManager : public RunObject
{
public:
	CharacterManager();
	~CharacterManager();

	void Init();
	void Release();
	void CreateThread();
	void Run();

	void UpdateClientData(GameClient* pClient, DWORD dwCurrTick);

	bool HasClients() { return (cList.size() > 0); }

	bool AddClient(GameClient* pClient);
	void RemoveClient(GameClient* pClient);
	bool FindClient(GameClient* pClient);
	void SendAll(void* pData, int nSize);
	void SendAll2(void* pData, int nSize, GameClient* pClient);
	void SendOthers(void* pData, int nSize, GameClient* pClient, bool distCheck = false);
	void RecvOthers(eOPCODE_GU Opcode, GameClient* pClient, bool distCheck = false);

	typedef std::map<unsigned int, GameClient*> clientList;
	typedef clientList::iterator cliIt;
	typedef clientList::value_type cliVal;

	clientList cList;

private:
	Thread * pThread;
	GameServer* pServer;
	DWORD dwLastTick;
};

#endif