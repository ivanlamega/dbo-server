#ifndef _NPCPROFILE
#define _NPCPROFILE

#include "Base.h"
#include "Def.h"

#include <TableAll.h>

class GameServer;

class NpcProfile
{
public:
	NpcProfile(TBLIDX worldTblidx, TBLIDX spawnTblidx, TBLIDX mobTblidx = 0, bool bCustom = false);
	~NpcProfile();

	bool Init();

	void GetObjectCreate(sGU_OBJECT_CREATE& sPacket);

	HOBJECT GetSerialID() { return hSerialID; }
	TBLIDX GetWorldID() { return worldTblIdx; }
	TBLIDX GetSpawnID() { return spawnTblIdx; }
	TBLIDX GetMobID() { return mobTblIdx; }
	HOBJECT GetTarget() { return hTarget; }

	void SetTarget(HOBJECT hTarget) { this->hTarget = hTarget; }

	sNPC_BRIEF sBrief;
	sCHARSTATE sCharState;

private:
	TBLIDX worldTblIdx;
	TBLIDX spawnTblIdx;
	TBLIDX mobTblIdx;
	HOBJECT hSerialID;
	HOBJECT hTarget;

	bool bCustomSpawn;

	GameServer* pServer;
};

#endif