#pragma once

#include "Table.h"

const int QUEST_REWARD_DEF_MAX_CNT = 4;			/* DboTSCoreDefine.h => MAX_DEFAULT_REWARD */
const int QUEST_REWARD_SEL_MAX_CNT = 4;			/* DboTSCoreDefine.h => MAX_SELECTION_REWARD */

#pragma pack(push, 4)
struct sQUEST_REWARD_DATASET
{
	BYTE						byRewardType;
	DWORD						dwRewardIdx;
	DWORD						dwRewardVal;
};

struct sQUEST_REWARD_TBLDAT : public sTBLDAT
{
	sQUEST_REWARD_TBLDAT()
	{
		memset(arsDefRwd, 0xff, sizeof(arsDefRwd));
		memset(arsSelRwd, 0xff, sizeof(arsSelRwd));
	}

public:
	sQUEST_REWARD_DATASET		arsDefRwd[QUEST_REWARD_DEF_MAX_CNT];
	sQUEST_REWARD_DATASET		arsSelRwd[QUEST_REWARD_SEL_MAX_CNT];

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)



class QuestRewardTable : public Table
{
public:
	QuestRewardTable(void);
	virtual ~QuestRewardTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

	sTBLDAT* FindData(TBLIDX tblidx);

	virtual bool LoadFromBinary(Serializer& serializer, bool bReload);
	virtual bool SaveToBinary(Serializer& serializer);

protected:
	void Init();

	WCHAR** GetSheetListInWChar() { return &(QuestRewardTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

private:
	static WCHAR* m_pwszSheetList[];
};

