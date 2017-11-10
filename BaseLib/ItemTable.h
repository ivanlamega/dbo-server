#pragma once

#include "Table.h"
#include "NtlItem.h"

// Hard Coding
#define	HOIPOISTONE_ITEM_INDEX	19954

const DWORD		DBO_MAX_LENGTH_ITEM_NAME_TEXT = 32;
const DWORD		DBO_MAX_LENGTH_ITEM_ICON_NAME = 32;
const DWORD		DBO_MAX_LENGTH_ITEM_MODEL_NAME = 32;
const DWORD		DBO_MAX_LENGTH_ITEM_SUBWEAPON_ACT_MODEL_NAME = 32;

#pragma pack(push, 4)
struct sITEM_TBLDAT : public sTBLDAT
{
	sITEM_TBLDAT()
	{
		bValidity_Able = true;
	};

public:
	bool			bValidity_Able;
	TBLIDX			Name;
	WCHAR			wszNameText[DBO_MAX_LENGTH_ITEM_NAME_TEXT + 1];
	char			szIcon_Name[DBO_MAX_LENGTH_ITEM_ICON_NAME + 1];
	BYTE			byModel_Type;
	char			szModel[DBO_MAX_LENGTH_ITEM_MODEL_NAME + 1];
	char			szSub_Weapon_Act_Model[DBO_MAX_LENGTH_ITEM_SUBWEAPON_ACT_MODEL_NAME + 1];
	BYTE			byItem_Type;
	BYTE			byEquip_Type;
	WORD			wEquip_Slot_Type_Bit_Flag;		// 슬롯 한개만 저장
	WORD			wFunction_Bit_Flag;
	BYTE			byMax_Stack;
	BYTE			byRank;
	DWORD			dwCost;
	BYTE			bySell_Price;
	BYTE			byDurability;
	BYTE			byDurability_Count;
	BYTE			byBattle_Attribute;
	WORD			wPhysical_Offence;
	WORD			wEnergy_Offence;
	WORD			wPhysical_Defence;
	WORD			wEnergy_Defence;
	DWORD			dwPhysical_OffenceUpgrade;
	DWORD			dwEnergy_OffenceUpgrade;
	DWORD			dwPhysical_DefenceUpgrade;
	DWORD			dwEnergy_DefenceUpgrade;

	float			fAttack_Range_Bonus;
	WORD			wAttack_Speed_Rate;  // 애니메이 속도에 대한 %
	BYTE			byNeed_Level;
	DWORD			dwNeed_Class_Bit_Flag;
	BYTE			byClass_Special;
	BYTE			byRace_Special;
	BYTE			byNeed_Str;
	BYTE			byNeed_Con;
	BYTE			byNeed_Foc;
	BYTE			byNeed_Dex;
	BYTE			byNeed_Sol;
	BYTE			byNeed_Eng;
	TBLIDX			set_Item_Tblidx;
	TBLIDX			Note;
	BYTE			byBag_Size;
	WORD			wScouter_Watt;
	DWORD			dwScouter_MaxPower;
	BYTE			byScouter_Parts_Type;
	BYTE			byScouter_Parts_Value;
	TBLIDX			Use_Item_Tblidx;
	TBLIDX			Item_Option_Tblidx;
	BYTE			byItemGroup;
	TBLIDX			Charm_Tblidx;
	WORD			wCostumeHideBitFlag;
	TBLIDX			NeedItemTblidx;		// 천하제일무도회 상점에서 구매에 필요한 아이템
	DWORD			CommonPoint;		// 무도사포인트, NetPY
	BYTE			byCommonPointType;
	BYTE			byNeedFunction;
	DWORD			dwUseDurationMax;	//아이템 최대 사용기간 ( 초 단위 )
	BYTE			byDurationType;		//eDURATIONTYPE
	TBLIDX			contentsTblidx;
	DWORD			dwDurationGroup;	// 같은 종류의 기간제 아이템인지를 나타내는 그룹
public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class ItemTable : public Table
{
public:

	ItemTable(void);
	virtual ~ItemTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

	sTBLDAT* FindData(TBLIDX tblidx);

	virtual bool LoadFromBinary(Serializer& serializer, bool bReload);
	virtual bool SaveToBinary(Serializer& serializer);

protected:
	void Init();

	WCHAR** GetSheetListInWChar() { return &(ItemTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

private:
	static WCHAR* m_pwszSheetList[];
};