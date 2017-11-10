#include "CharacterManager.h"
#include "GameNetwork.h"

#include <ppl.h>

CharacterManager::CharacterManager()
{
	pServer = (GameServer*)_GetApp();
	dwLastTick = 0;
}

CharacterManager::~CharacterManager()
{
	Release();
}

void CharacterManager::Init()
{
	CreateThread();
}

void CharacterManager::Release()
{
}


void CharacterManager::Run()
{
	while (IsRunnable())
	{
		DWORD dwTick = GetTickCount();
		if (HasClients())
		{
			for(cliIt it = cList.begin(); it != cList.end(); it++)
			{
				if (it->second) UpdateClientData(it->second, dwTick);
			}
		}
		dwLastTick = dwTick;
		Sleep(1000);
	}
}

void CharacterManager::UpdateClientData(GameClient* pClient, DWORD dwCurrTick)
{
	// check if is alive
	if (false == pClient->IsClosed() && HasClients() && pClient->IsReadyToUpdate())
	{
		// Update LP/EP
		pClient->SendLPEPUpdate(pClient->GetProfile()->sPcProfile.dwCurLP,
			pClient->GetProfile()->sPcProfile.avatarAttribute.dwBaseMaxLP,
			pClient->GetProfile()->sPcProfile.wCurEP,
			pClient->GetProfile()->sPcProfile.avatarAttribute.wBaseMaxEP,
			pClient->GetProfile()->GetSerialID());

		// check player attack
		pClient->SendCharAttack(dwCurrTick);

		// Check Levelup
		pClient->CheckLevelUpdate();
	}
}

void CharacterManager::CreateThread()
{
	pThread = ThreadFactory::CreateThread(this, "CharacterManagerThread");
	pThread->Start();
}

bool CharacterManager::AddClient(GameClient* pClient)
{
	if (false == cList.insert(cliVal(pClient->GetHandle(), pClient)).second)
	{
		return false;
	}
	return true;
}

void CharacterManager::RemoveClient(GameClient* pClient)
{
	cliIt it = cList.find(pClient->GetHandle());
	if (it != cList.end())
	{
		if (pClient->GetHandle() == it->second->GetHandle())
		{
			cList.erase(it);
		}
	}
}

bool CharacterManager::FindClient(GameClient* pClient)
{
	cliIt it = cList.find(pClient->GetHandle());
	if (it != cList.end())
	{
		if (pClient->GetHandle() == it->second->GetHandle())
		{
			return true;
		}
	}
	return false;
}

void CharacterManager::SendAll2(void* pData, int nSize, GameClient* pClient)
{
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
		if (pClient->GetHandle() == it->second->GetHandle())
			it->second->PushPacket(pData, nSize);
		else if (pClient->FindSpawn(it->second->GetHandle(), 0))
			it->second->PushPacket(pData, nSize);
	}
}

void CharacterManager::SendAll(void* pData, int nSize)
{
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
		it->second->PushPacket(pData, nSize);
	}
}

void CharacterManager::SendOthers(void* pData, int nSize, GameClient* pClient, bool distCheck)
{
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
		if (pClient->GetHandle() != it->second->GetHandle())
			it->second->PushPacket(pData, nSize);
	}
}

void CharacterManager::RecvOthers(eOPCODE_GU Opcode, GameClient* pClient, bool distCheck)
{
	for (cliIt it = cList.begin(); it != cList.end(); ++it)
	{
		if (pClient->GetHandle() != it->second->GetHandle())
		{
			switch (Opcode)
			{
			case GU_OBJECT_CREATE:
				{
					sGU_OBJECT_CREATE sPacket;
					it->second->GetProfile()->GetObjectCreate(sPacket);
					pClient->PushPacket(&sPacket, sizeof(sPacket));
				} break;
			}
		}
	}
}