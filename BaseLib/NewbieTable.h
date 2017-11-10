#pragma once

#include "Table.h"
#include "NtlVector.h"
#include "NtlCharacter.h"

#pragma pack(push, 4)
struct sNEWBIE_TBLDAT : public sTBLDAT
{
public:

	BYTE			byRace;
	BYTE			byClass;
	WORLDID			world_Id;
	TBLIDX			tutorialWorld;
	CNtlVector      vSpawn_Loc;
	CNtlVector      vSpawn_Dir;
	CNtlVector      vBind_Loc;
	CNtlVector      vBind_Dir;
	TBLIDX			aitem_Tblidx[NTL_MAX_NEWBIE_ITEM];
	BYTE			abyPos[NTL_MAX_NEWBIE_ITEM];
	BYTE			abyStack_Quantity[NTL_MAX_NEWBIE_ITEM];
	TBLIDX			mapNameTblidx;
	TBLIDX			aSkillTblidx[NTL_MAX_NEWBIE_SKILL];
	sNEWBIE_QUICKSLOT_DATA	asQuickData[NTL_MAX_NEWBIE_QUICKSLOT_COUNT];
	PORTALID		defaultPortalTblidx;
	TBLIDX			qItemTblidx1;
	BYTE			byQPosition1;
	BYTE			byQStackQuantity1;
protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};

struct sNEWBIE_TBLDAT2
{
public:
	TBLIDX          tblidx;

	BYTE			byRace;
	BYTE			byClass;

	WORLDID			world_Id;
	TBLIDX			tutorialWorld;

	CNtlVector      vSpawn_Loc;
	CNtlVector      vSpawn_Dir;

	CNtlVector      vBind_Loc;
	CNtlVector      vBind_Dir;

	TBLIDX			aitem_Tblidx[NTL_MAX_NEWBIE_ITEM2];
	BYTE			abyPos[NTL_MAX_NEWBIE_ITEM2];
	BYTE			abyStack_Quantity[NTL_MAX_NEWBIE_ITEM2];

	TBLIDX			mapNameTblidx;

	TBLIDX			aSkillTblidx[NTL_MAX_NEWBIE_SKILL2];

	sNEWBIE_QUICKSLOT_DATA	asQuickData[NTL_MAX_NEWBIE_QUICKSLOT_COUNT];

	PORTALID		defaultPortalTblidx;

	BYTE			unknown1;

	BYTE			unknown2;

	TBLIDX			qItemTblidx1;
	BYTE			byQPosition1;
	BYTE			byQStackQuantity1;

	BYTE			unknown3;
	BYTE			unknown4;
};

#pragma pack(pop)

class NewbieTable :
	public Table
{
public:
	NewbieTable(void);
	virtual ~NewbieTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

	sTBLDAT* FindData(TBLIDX tblidx);
	sTBLDAT* GetNewbieTbldat(BYTE byRace, BYTE byClass);
	bool SetNewbieTbldat(BYTE byRace, BYTE byClass, sTBLDAT * pTbldat);

	virtual bool LoadFromBinary(Serializer& serializer, bool bReload);
	virtual bool SaveToBinary(Serializer& serializer);

protected:
	void Init();

	WCHAR** GetSheetListInWChar() { return &(NewbieTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

	sTBLDAT* m_aNewbieTbldat[RACE_COUNT][PC_CLASS_COUNT];

private:
	static WCHAR* m_pwszSheetList[];
};
