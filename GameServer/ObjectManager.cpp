#include "ObjectManager.h"
#include "GameNetwork.h"
#include "CharacterProfile.h"
#include "NpcProfile.h"
#include "MobProfile.h"

#include <ppl.h>

ObjectManager::ObjectManager()
{
	pServer = (GameServer*)_GetApp();
}

ObjectManager::~ObjectManager()
{
	Release();
}

void ObjectManager::Init()
{
	CreateThread();
}

void ObjectManager::Release()
{
}


void ObjectManager::Run()
{
	while (IsRunnable())
	{
		if (HasPcs())
		{
			try
			{
				if (pcList.size()>0)
				{
					for (OBJPCLISTIT it = pcList.begin(); it != pcList.end(); it++)
					{
						GameClient* pClient = it->second->GetClient();

						if (pClient && pClient->IsSpawnReady())
						{
							SpawnToClient(pClient);
						}
					}
				}

			}
			catch (Exception e){
				Logger::Log("Error on SpawnToClient: %s Removing from map...", e.GetWhat());
				continue;
			}
		}
		Sleep(1000);
	}
}

void ObjectManager::CreateThread()
{
	pThread = ThreadFactory::CreateThread(this, "ObjectManagerThread");
	pThread->Start();
}

//By Luiz45 Insert the DB on our list
bool ObjectManager::AddDragonBall(TBLIDX tblDragonBallItem, sDRAGONBALL_TBLDAT* pDb)
{
	if (false == dbList.insert(OBJDRAGONBALLLISTVAL(tblDragonBallItem, pDb)).second)
		return false;
	return true;
}
//By Luiz45 Find DB
bool ObjectManager::FindDragonBall(TBLIDX idxDB)
{
	OBJDRAGONBALLIT it = dbList.find(idxDB);
	if (it == dbList.end())
		return false;
	return true;
}
bool ObjectManager::AddObject(HOBJECT hObject, void* pObj, eOBJTYPE eType)
{
	if (eType == eOBJTYPE::OBJTYPE_PC)
	{
		if (false == pcList.insert(OBJPCLISTVAL(hObject, (CharacterProfile*)pObj)).second)
			return false;
		return true;
	}
	if (eType == eOBJTYPE::OBJTYPE_NPC)
	{
		if (false == npcList.insert(OBJNPCLISTVAL(hObject, (NpcProfile*)pObj)).second)
			return false;
		return true;
	}
	if (eType == eOBJTYPE::OBJTYPE_MOB)
	{
		if (false == mobList.insert(OBJMOBLISTVAL(hObject, (MobProfile*)pObj)).second)
			return false;
		return true;
	}
	if (eType == eOBJTYPE::OBJTYPE_TOBJECT)
	{
		if (false == objMapList.insert(OBJMAPLISTVAL(hObject, (sOBJECT_TBLDAT*)pObj)).second)
			return false;
		return true;
	}
	if (eType == eOBJTYPE::OBJTYPE_DYNAMIC)
	{
		if (false == objDynamicList.insert(OBJDYNAMICLISTVAL(hObject, (sDYNAMIC_OBJECT_TBLDAT*)pObj)).second)
			return false;
		return true;
	}
	return false;
}

void ObjectManager::RemoveObject(HOBJECT hObject, eOBJTYPE eType)
{
	if (eType == eOBJTYPE::OBJTYPE_PC)
	{
		OBJPCLISTIT it = pcList.find(hObject);
		if (it != pcList.end())
			pcList.erase(it);
	}
	if (eType == eOBJTYPE::OBJTYPE_NPC)
	{
		OBJNPCLISTIT it = npcList.find(hObject);
		if (it != npcList.end())
			npcList.erase(it);
	}
	if (eType == eOBJTYPE::OBJTYPE_MOB)
	{
		OBJMOBLISTIT it = mobList.find(hObject);
		if (it != mobList.end())
			mobList.erase(it);
	}
	if (eType == eOBJTYPE::OBJTYPE_TOBJECT)
	{
		OBJMAPLISTIT it = objMapList.find(hObject);
		if (it != objMapList.end())
			objMapList.erase(it);
	}
	if (eType == eOBJTYPE::OBJTYPE_DYNAMIC)
	{
		OBJDYNAMICLISTIT it = objDynamicList.find(hObject);
		if (it != objDynamicList.end())
			objDynamicList.erase(it);
	}
}

bool ObjectManager::FindObject(HOBJECT hObject, eOBJTYPE eType)
{
	if (eType == eOBJTYPE::OBJTYPE_PC)
	{
		OBJPCLISTIT it = pcList.find(hObject);
		if (it == pcList.end())
			return false;
		return true;
	}
	if (eType == eOBJTYPE::OBJTYPE_NPC)
	{
		OBJNPCLISTIT it = npcList.find(hObject);
		if (it == npcList.end())
			return false;
		return true;
	}
	if (eType == eOBJTYPE::OBJTYPE_MOB)
	{
		OBJMOBLISTIT it = mobList.find(hObject);
		if (it == mobList.end())
			return false;
		return true;
	}
	if (eType == eOBJTYPE::OBJTYPE_TOBJECT)
	{
		OBJMAPLISTIT it = objMapList.find(hObject);
		if (it == objMapList.end())
			return false;
		return true;
	}
	if (eType == eOBJTYPE::OBJTYPE_DYNAMIC)
	{
		OBJDYNAMICLISTIT it = objDynamicList.find(hObject);
		if (it == objDynamicList.end())
			return false;
		return true;
	}
	return false;
}

void ObjectManager::UpdateCharState(HOBJECT hObject, sCHARSTATE CharState)
{
	for (OBJPCLISTIT it = pcList.begin(); it != pcList.end(); it++)
	{
		if (hObject == it->second->GetSerialID())
		{
			memcpy(&it->second->sCharState, &CharState, sizeof(sCHARSTATE));
		}
	}
	for (OBJNPCLISTIT it = npcList.begin(); it != npcList.end(); it++)
	{
		if (hObject == it->second->GetSerialID())
		{
			memcpy(&it->second->sCharState, &CharState, sizeof(sCHARSTATE));
		}
	}
	for (OBJMOBLISTIT it = mobList.begin(); it != mobList.end(); it++)
	{
		if (hObject == it->second->GetSerialID())
		{
			memcpy(&it->second->sCharState, &CharState, sizeof(sCHARSTATE));
		}
	}
}

void ObjectManager::SpawnToClient(GameClient* pClient)
{
	if (false == pClient->IsClosed() && pClient->IsConnected() && pClient->GetProfile() && pClient && pClient->IsSpawnReady())
	{
		for (OBJPCLISTIT it = pcList.begin(); it != pcList.end(); it++)
		{
			CharacterProfile* obj = it->second;

			if (pClient->GetProfile()->GetSerialID() == obj->GetSerialID())
				continue;

			if (pClient->GetProfile()->sWorldInfo.tblidx != obj->sWorldInfo.tblidx)
				continue;

			float objx = obj->sCharState.sCharStateBase.vCurLoc.x;
			float objz = obj->sCharState.sCharStateBase.vCurLoc.z;
			float pcx = pClient->GetProfile()->sCharState.sCharStateBase.vCurLoc.x;
			float pcz = pClient->GetProfile()->sCharState.sCharStateBase.vCurLoc.z;

			float dist = NtlGetDistance(pcx, pcz, objx, objz);

			if (dist < 100)
			{
				if (false == pClient->FindSpawn(obj->GetSerialID(), eOBJTYPE::OBJTYPE_PC))
				{
					pClient->AddSpawn(obj->GetSerialID(), eOBJTYPE::OBJTYPE_PC);
					sGU_OBJECT_CREATE sPacket;
					obj->GetObjectCreate(sPacket);
					pClient->PushPacket(&sPacket, sizeof(sPacket));
				}
			}
			else
			{
				if (pClient->FindSpawn(obj->GetSerialID(), eOBJTYPE::OBJTYPE_PC))
				{
					pClient->RemoveSpawn(obj->GetSerialID());
					sGU_OBJECT_DESTROY sPacket;
					memset(&sPacket, 0, sizeof(sPacket));
					sPacket.wOpCode = GU_OBJECT_DESTROY;
					sPacket.handle = obj->GetSerialID();
					pClient->PushPacket(&sPacket, sizeof(sPacket));
				}
			}
		}
		for (OBJNPCLISTIT it = npcList.begin(); it != npcList.end(); it++)
		{
			NpcProfile* obj = it->second;

			if (pClient->GetProfile()->GetSerialID() == obj->GetSerialID())
				continue;

			if (pClient->GetProfile()->sWorldInfo.tblidx != obj->GetWorldID())
				continue;

			float objx = obj->sCharState.sCharStateBase.vCurLoc.x;
			float objz = obj->sCharState.sCharStateBase.vCurLoc.z;
			float pcx = pClient->GetProfile()->sCharState.sCharStateBase.vCurLoc.x;
			float pcz = pClient->GetProfile()->sCharState.sCharStateBase.vCurLoc.z;

			float dist = NtlGetDistance(pcx, pcz, objx, objz);

			if (dist < 100)
			{
				if (false == pClient->FindSpawn(obj->GetSerialID(), eOBJTYPE::OBJTYPE_NPC))
				{
					pClient->AddSpawn(obj->GetSerialID(), eOBJTYPE::OBJTYPE_NPC);
					sGU_OBJECT_CREATE sPacket;
					obj->GetObjectCreate(sPacket);
					pClient->PushPacket(&sPacket, sizeof(sPacket));
				}
			}
			else
			{
				if (pClient->FindSpawn(obj->GetSerialID(), eOBJTYPE::OBJTYPE_NPC))
				{
					pClient->RemoveSpawn(obj->GetSerialID());
					sGU_OBJECT_DESTROY sPacket;
					memset(&sPacket, 0, sizeof(sPacket));
					sPacket.wOpCode = GU_OBJECT_DESTROY;
					sPacket.handle = obj->GetSerialID();
					pClient->PushPacket(&sPacket, sizeof(sPacket));
				}
			}
		}
		for (OBJMOBLISTIT it = mobList.begin(); it != mobList.end(); it++)
		{
			MobProfile* obj = it->second;

			if (pClient->GetProfile()->GetSerialID() == obj->GetSerialID())
				continue;

			if (pClient->GetProfile()->sWorldInfo.tblidx != obj->GetWorldID())
				continue;

			float objx = obj->sCharState.sCharStateBase.vCurLoc.x;
			float objz = obj->sCharState.sCharStateBase.vCurLoc.z;
			float pcx = pClient->GetProfile()->sCharState.sCharStateBase.vCurLoc.x;
			float pcz = pClient->GetProfile()->sCharState.sCharStateBase.vCurLoc.z;

			float dist = NtlGetDistance(pcx, pcz, objx, objz);

			if (dist < 100.0)
			{
				if (false == pClient->FindSpawn(obj->GetSerialID(), eOBJTYPE::OBJTYPE_MOB))
				{
					pClient->AddSpawn(obj->GetSerialID(), eOBJTYPE::OBJTYPE_MOB);
					sGU_OBJECT_CREATE sPacket;
					obj->GetObjectCreate(sPacket);
					pClient->PushPacket(&sPacket, sizeof(sPacket));
				}
			}
			else
			{
				if (pClient->FindSpawn(obj->GetSerialID(), eOBJTYPE::OBJTYPE_MOB))
				{
					pClient->RemoveSpawn(obj->GetSerialID());
					sGU_OBJECT_DESTROY sPacket;
					memset(&sPacket, 0, sizeof(sPacket));
					sPacket.wOpCode = GU_OBJECT_DESTROY;
					sPacket.handle = obj->GetSerialID();
					pClient->PushPacket(&sPacket, sizeof(sPacket));
				}
			}
		}
	}
}