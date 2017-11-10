#ifndef _CHARACTERPROFILE
#define _CHARACTERPROFILE

#include "Base.h"
#include "Def.h"
#include "AttributeManager.h"
#include <TableAll.h>

class GameServer;
class GameClient;
class AttributeManager;

class CharacterProfile
{
public:
	CharacterProfile(GameClient* pClient);
	~CharacterProfile();

	void Init();

	void InsertOnlineData();
	void RemoveOnlineData();
	void LoadCharacterData();
	void LoadWorldInfoData();
	unsigned int GetPCTblidx(BYTE Race, BYTE Gender, BYTE Class);
	void UpdatePositions(sVECTOR2 Dir, sVECTOR3 Loc);
	void UpdatePositions(sVECTOR3 Dir, sVECTOR3 Loc);
	int LoadItemData();
	int LoadSkillData();
	int LoadHTBData();
	int LoadQuickslotData();
	void CalculateAtributes();
	void GetObjectCreate(sGU_OBJECT_CREATE& sPacket);
	bool InsertNextBagSlot(sGU_ITEM_CREATE& sPacket, ITEMID item, BYTE qtd = 1);
	HOBJECT GetInventoryItemSerialID(BYTE byPlace, BYTE byPos);
	TBLIDX GetInventoryItemID(BYTE byPlace, BYTE byPos);
	void UpdateItemInventoryPosition(HOBJECT hItem, BYTE byPlace, BYTE byPos);
	void GetItemBrief(sITEM_BRIEF& sBrief, HOBJECT hItem);
	void LoadWarFogFlags();
	bool ExistItemInGame(HOBJECT hItemObject);
	bool CheckWarFogFlags(HOBJECT hObject);
	bool AddWarFogFlags(HOBJECT hObject);
	int ScanForFreeBag();
	int ScanForFreePosition(DWORD dwBag);
	void insertItemIntoInventory(sITEM_DATA& sItemData, HOBJECT itemHandle);
	void RemoveItemFromInventory(HOBJECT hItem);
	void UpdateCharLevel();
	void UpdateCharExp();
	void UpdateStackCount(HOBJECT hItem, BYTE byStackCount);
	TBLIDX GetSkillBySlot(int iIdx);
	void UpdateSkill(TBLIDX tblBefore, TBLIDX tblNext);
	BYTE GetStackCount(HOBJECT itemSerialID);
	void UpdateMoneyDataBase();

	// Set
	void SetAvatartype(BYTE byAvatarType) { this->byAvatarType = byAvatarType; }
	void SetAccountid(ACCOUNTID AccountID) { this->AccountID = AccountID; }
	void SetCharid(HOBJECT hCharID) { this->hCharID = hCharID; }
	void SetTutorialmode(bool bTutorialMode) { this->bTutorialMode = bTutorialMode; }
	void SetMovedirection(BYTE byMoveDirection) { this->byMoveDirection = byMoveDirection; }
	void SetTarget(HOBJECT hTarget) { this->hTarget = hTarget; }
	void SetBagStatus(DWORD dwStatus){ this->dwBagStatus = dwStatus; }
	void SetSpawnedShenron(HOBJECT hObj){ this->spawnedShenron = hObj; }

	// Get
	WCHAR* GetUsername() { return wszUserName; }
	WCHAR* GetPassword() { return wszPassWord; }
	WCHAR* GetCharname() { return wszCharName; }
	WCHAR* GetGuildname() { return wszGuildName; }
	BYTE* GetAuthkey() { return abyAuthKey; }
	ACCOUNTID GetAccountid() { return AccountID; }
	BYTE GetLastserverid() { return byLastServerID; }
	DWORD GetAclevel() { return dwAcLevel; }
	BYTE GetChannelid() { return byChannelID; }
	HOBJECT GetSerialID() { return hSerialID; }
	HOBJECT GetCharID() { return hCharID; }
	BYTE GetServerID() { return byServerID; }
	bool GetTutorialMode() { return bTutorialMode; }
	bool GetGameMaster() { return bIsGameMaster; }
	BYTE GetRace() { return byRace; }
	BYTE GetClass() { return byClass; }
	BYTE GetGender() { return byGender; }
	BYTE GetAvatartype() { return byAvatarType; }
	BYTE GetMovedirection() { return byMoveDirection; }
	DWORD GetBagStatus();
	HOBJECT GetTarget() { return hTarget; }
	GameClient* GetClient() { return pClient; }
	AttributeManager* GetAttributeManager(){ return pAttributeManager; }
	HOBJECT GetSpawnedShenron(){ return spawnedShenron; }

	// Structs
	sPC_PROFILE sPcProfile;
	sCHARSTATE sCharState;
	sWORLD_INFO	sOldWorldInfo;
	sWORLD_INFO sWorldInfo;
	BYTE byItemCount;
	sITEM_PROFILE asItemProfile[NTL_MAX_COUNT_USER_HAVE_INVEN_ITEM];
	BYTE bySkillCount;
	sSKILL_INFO asSkillInfo[NTL_MAX_PC_HAVE_SKILL];
	BYTE byHTBCount;
	sHTB_SKILL_INFO asHTBInfo[NTL_HTB_MAX_PC_HAVE_HTB_SKILL];
	BYTE byBuffCount;
	sBUFF_INFO asBuffInfo[NTL_MAX_BLESS_BUFF_CHARACTER_HAS + NTL_MAX_CURSE_BUFF_CHARACTER_HAS];
	bool bSetQuickSlotInfo;
	BYTE byQuickSlotInfoCount;
	sQUICK_SLOT_PROFILE asQuickSlotData[NTL_CHAR_QUICK_SLOT_MAX_COUNT];
	sQUEST_COMPLETE_INFO sQuestCompleteInfo;
	BYTE byQuestProgressInfoCount;
	sQUEST_PROGRESS_INFO sQuestProgressInfo[eMAX_CAN_PROGRESS_QUEST_NUM];
	sQUEST_INVENTORY_FULLDATA sQuestInventoryInfo;
	BYTE byMaxRpStock;
	BYTE byMailCount;
	BYTE byUnreadMailCountNormal;
	BYTE byUnreadMailCountManager;
	char acWarFogFlag[NTL_MAX_SIZE_WAR_FOG];   
	sRANKBATTLE_SCORE_INFO sRankBattleScoreInfo;
	BYTE byDojoBriefCount;
	sDBO_DOJO_BRIEF sDojoBrief[DBO_MAX_COUNT_DOJO_IN_WORLD];

	bool bIsSit;
	bool bIsMoving;
	DWORD dwBagStatus;
	DWORD dwLastAttack;

private:
	WCHAR wszUserName[NTL_MAX_SIZE_USERID_UNICODE + 1];
	WCHAR wszPassWord[NTL_MAX_SIZE_USERPW_UNICODE + 1];
	WCHAR wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WCHAR wszGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
	BYTE abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
	ACCOUNTID AccountID;
	BYTE byLastServerID;
	DWORD dwAcLevel;
	BYTE byServerID;
	BYTE byChannelID;
	HOBJECT hCharID;
	bool bIsGameMaster;
	bool bTutorialMode;
	HOBJECT hSerialID;
	BYTE byMoveDirection;
	BYTE byAvatarType;
	BYTE byRace;
	BYTE byClass;
	BYTE byGender;
	HOBJECT hTarget;
	HOBJECT spawnedShenron;

	GameServer* pServer;
	GameClient* pClient;
	AttributeManager* pAttributeManager;
};

#endif