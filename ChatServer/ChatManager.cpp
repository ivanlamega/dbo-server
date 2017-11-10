#include "ChatManager.h"
#include "ChatNetwork.h"

ChatManager::ChatManager()
{
	pServer = (ChatServer*)_GetApp();
}

ChatManager::~ChatManager()
{
	Release();
}

void ChatManager::Init()
{
	CreateThread();
}

void ChatManager::Release()
{
}


void ChatManager::Run()
{
	while (IsRunnable())
	{
		Sleep(1000);
	}
}

void ChatManager::CreateThread()
{
	pThread = ThreadFactory::CreateThread(this, "ChatManagerThread");
	pThread->Start();
}

bool ChatManager::AddClient(ChatClient* pClient)
{
	if (false == cList.insert(cliVal(pClient->GetHandle(), pClient)).second)
	{
		return false;
	}
	return true;
}

void ChatManager::RemoveClient(ChatClient* pClient)
{
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
		if (pClient->GetHandle() == it->second->GetHandle())
			cList.erase(it);
	}
}

//By Luiz45 - Just Return the ChatClient(he will search through CharName)
ChatClient* ChatManager::GetChatClient(WCHAR* pCharName){
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
		if (FindClient(it->second))
		{
			if (wcscmp(it->second->GetCharName(), pCharName) == 0)
				return it->second;
		}
	}
	return NULL;
}

bool ChatManager::FindClient(ChatClient* pClient)
{
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
		if (pClient->GetHandle() == it->second->GetHandle())
			return true;
	}
	return false;
}

void ChatManager::SendAll2(void* pData, int nSize, ChatClient* pClient)
{
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
		if (pClient->GetHandle() == it->second->GetHandle())
			it->second->PushPacket(pData, nSize);
		else if (pServer->GetChatManager()->FindClient(it->second))
			it->second->PushPacket(pData, nSize);
	}
}

void ChatManager::SendAll(void* pData, int nSize)
{
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
			it->second->PushPacket(pData, nSize);
	}
}

//By Luiz45 - Send A packet directly to a specific client
void ChatManager::SendTo(void* pData, int nSize, ChatClient* pClient)
{
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
		if (pClient->GetHandle() == it->second->GetHandle())
			it->second->PushPacket(pData, nSize);
	}
}

void ChatManager::SendOthers(void* pData, int nSize, ChatClient* pClient, bool distCheck)
{
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
		if (pClient->GetHandle() != it->second->GetHandle())
			it->second->PushPacket(pData, nSize);
	}
}

void ChatManager::RecvOthers(eOPCODE_TU Opcode, ChatClient* pClient, bool distCheck)
{
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
		if (pClient->GetHandle() != it->second->GetHandle())
		{
			switch (Opcode)
			{
			case 0:
			{
			} break;
			}
		}
	}
}