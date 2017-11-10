#pragma once

#include "Table.h"
#include "NtlVector.h"

const DWORD		DBO_MAX_LENGTH_WORLD_NAME = 32;
const DWORD		DBO_MAX_LENGTH_WORLD_NAME_IN_UNICODE = 32;
const DWORD		DBO_MAX_LENGTH_WORLD_MOB_SPAWN_TABLE_NAME = 64;
const DWORD		DBO_MAX_LENGTH_WORLD_NPC_SPAWN_TABLE_NAME = 64;
const DWORD		DBO_MAX_LENGTH_WORLD_OBJ_SPAWN_TABLE_NAME = 64;
const DWORD		DBO_MAX_LENGTH_WORLD_RESOURCE_FOLDER_NAME = 64;
const DWORD		DBO_MAX_LENGTH_WORLD_RESOURCE_FLASH_NAME = 32;

#pragma pack(push, 4)
struct sWORLD_TBLDAT : public sTBLDAT
{
public:
	char			szName[DBO_MAX_LENGTH_WORLD_NAME + 1];
	WCHAR			wszName[DBO_MAX_LENGTH_WORLD_NAME_IN_UNICODE + 1];
	bool			bDynamic;
	int				nCreateCount;
	BYTE			byDoorType;

	DWORD			dwDestroyTimeInMilliSec;

	WCHAR			wszMobSpawn_Table_Name[DBO_MAX_LENGTH_WORLD_MOB_SPAWN_TABLE_NAME + 1];
	WCHAR			wszNpcSpawn_Table_Name[DBO_MAX_LENGTH_WORLD_NPC_SPAWN_TABLE_NAME + 1];
	WCHAR			wszObjSpawn_Table_Name[DBO_MAX_LENGTH_WORLD_OBJ_SPAWN_TABLE_NAME + 1];

	CNtlVector		vStart;
	CNtlVector		vEnd;
	CNtlVector		vStandardLoc;
	CNtlVector		vBattleStartLoc;
	CNtlVector		vBattleEndLoc;
	CNtlVector		vOutSideBattleStartLoc;
	CNtlVector		vOutSideBattleEndLoc;
	CNtlVector		vSpectatorStartLoc;
	CNtlVector		vSpectatorEndLoc;
	CNtlVector		vDefaultLoc;
	CNtlVector		vDefaultDir;
	CNtlVector		vStart1Loc;
	CNtlVector		vStart1Dir;
	CNtlVector		vStart2Loc;
	CNtlVector		vStart2Dir;

	float			fSplitSize;
	bool			bNight_Able;
	BYTE			byStatic_Time;
	WORD			wFuncFlag;

	BYTE			byWorldRuleType;	// eGAMERULE_TYPE
	TBLIDX			worldRuleTbldx;		// 해당하는 룰 테이블 인덱스 ( 룰마다 테이블 틀림 : 랭크배틀/타임퀘스트/ 등등... )

	TBLIDX			outWorldTblidx;
	CNtlVector		outWorldLoc;
	CNtlVector		outWorldDir;
	WCHAR			wszResourceFolder[DBO_MAX_LENGTH_WORLD_RESOURCE_FOLDER_NAME + 1];
	float			fBGMRestTime;
	DWORD			dwWorldResourceID;
	float			fFreeCamera_Height;

	WCHAR			wszEnterResourceFlash[DBO_MAX_LENGTH_WORLD_RESOURCE_FLASH_NAME + 1];
	WCHAR			wszLeaveResourceFlash[DBO_MAX_LENGTH_WORLD_RESOURCE_FLASH_NAME + 1];

	TBLIDX			wpsLinkIndex;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};

struct sWORLD_TBLDAT2
{
public:
	TBLIDX			tblidx;
	char			szName[DBO_MAX_LENGTH_WORLD_NAME + 1];
	WCHAR			wszName[DBO_MAX_LENGTH_WORLD_NAME_IN_UNICODE + 1];
	bool			bDynamic;
	int				nCreateCount;

	TBLIDX			unknownTblidx;

	BYTE			byDoorType;

	DWORD			dwDestroyTimeInMilliSec;

	WCHAR			wszMobSpawn_Table_Name[DBO_MAX_LENGTH_WORLD_MOB_SPAWN_TABLE_NAME + 1];
	WCHAR			wszNpcSpawn_Table_Name[DBO_MAX_LENGTH_WORLD_NPC_SPAWN_TABLE_NAME + 1];
	WCHAR			wszObjSpawn_Table_Name[DBO_MAX_LENGTH_WORLD_OBJ_SPAWN_TABLE_NAME + 1];

	CNtlVector		vStart;
	CNtlVector		vEnd;
	CNtlVector		vStandardLoc;
	CNtlVector		vBattleStartLoc;
	CNtlVector		vBattleEndLoc;
	CNtlVector		vOutSideBattleStartLoc;
	CNtlVector		vOutSideBattleEndLoc;
	CNtlVector		vSpectatorStartLoc;
	CNtlVector		vSpectatorEndLoc;
	CNtlVector		vDefaultLoc;
	CNtlVector		vDefaultDir;
	CNtlVector		vStart1Loc;
	CNtlVector		vStart1Dir;
	CNtlVector		vStart2Loc;
	CNtlVector		vStart2Dir;
	CNtlVector unknown2[6];

	float			fSplitSize;
	bool			bNight_Able;
	BYTE			byStatic_Time;
	WORD			wFuncFlag;

	BYTE			byWorldRuleType;	// eGAMERULE_TYPE
	TBLIDX			worldRuleTbldx;		// 해당하는 룰 테이블 인덱스 ( 룰마다 테이블 틀림 : 랭크배틀/타임퀘스트/ 등등... )

	TBLIDX			outWorldTblidx;
	CNtlVector		outWorldLoc;
	CNtlVector		outWorldDir;
	WCHAR			wszResourceFolder[DBO_MAX_LENGTH_WORLD_RESOURCE_FOLDER_NAME + 1];
	float			fBGMRestTime;
	DWORD			dwWorldResourceID;
	float			fFreeCamera_Height;

	WCHAR			wszEnterResourceFlash[DBO_MAX_LENGTH_WORLD_RESOURCE_FLASH_NAME + 1];
	WCHAR			wszLeaveResourceFlash[DBO_MAX_LENGTH_WORLD_RESOURCE_FLASH_NAME + 1];

	TBLIDX			wpsLinkIndex;

	BYTE			unknown3[11];
	DWORD			unknown4;
};
#pragma pack(pop)

class WorldTable : public Table
{
public:

	WorldTable(void);
	virtual ~WorldTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

	sTBLDAT* FindData(TBLIDX tblidx);

	virtual bool LoadFromBinary(Serializer& serializer, bool bReload);
	virtual bool SaveToBinary(Serializer& serializer);

protected:
	void Init();

	WCHAR** GetSheetListInWChar() { return &(WorldTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

private:
	static WCHAR* m_pwszSheetList[];
};
