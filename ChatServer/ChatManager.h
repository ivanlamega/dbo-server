#ifndef _CHATMANAGER
#define _CHATMANAGER

#include <Base.h>
#include <Def.h>
#include <Thread.h>
#include <Mutex.h>

#include <iostream>
#include <memory>
#include <map>

class ChatServer;
class ChatClient;

class ChatManager : public RunObject
{
public:
	ChatManager();
	~ChatManager();

	void Init();
	void Release();
	void CreateThread();
	void Run();

	bool AddClient(ChatClient* pClient);
	void RemoveClient(ChatClient* pClient);
	bool FindClient(ChatClient* pClient);
	void SendAll(void* pData, int nSize);
	void SendAll2(void* pData, int nSize, ChatClient* pClient);
	void SendTo(void* pData, int nSize, ChatClient* pClient);
	void SendOthers(void* pData, int nSize, ChatClient* pClient, bool distCheck = false);
	void RecvOthers(eOPCODE_TU Opcode, ChatClient* pClient, bool distCheck = false);
	ChatClient* GetChatClient(WCHAR* pCharName);

private:
	Thread * pThread;
	ChatServer* pServer;
	typedef std::map<unsigned int, ChatClient*> clientList;
	typedef clientList::iterator cliIt;
	typedef clientList::value_type cliVal;
	clientList cList;
};

#endif