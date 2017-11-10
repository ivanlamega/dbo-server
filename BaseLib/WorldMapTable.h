#pragma once

#include "Table.h"
#include "NtlVector.h"

const DWORD		DBO_MAX_LENGTH_WORLD_MAP_NAME = 32;
const DWORD		DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG = 8;
#pragma pack(push, 4)
struct sWORLD_MAP_TBLDAT : public sTBLDAT
{
public:

	TBLIDX			World_Tblidx;
	TBLIDX			Zone_Tblidx;
	TBLIDX			Worldmap_Name;
	WCHAR			wszNameText[DBO_MAX_LENGTH_WORLD_MAP_NAME + 1];
	bool			bValidityAble;
	BYTE			byMapType;
	CNtlVector		vStandardLoc;
	float			fWorldmapScale;
	WORD			wWarfog[DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG];
public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class WorldMapTable : public Table
{
public:
	WorldMapTable(void);
	virtual ~WorldMapTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

	sTBLDAT* FindData(TBLIDX tblidx);

	virtual bool LoadFromBinary(Serializer& serializer, bool bReload);
	virtual bool SaveToBinary(Serializer& serializer);

protected:
	void Init();

	WCHAR** GetSheetListInWChar() { return &(WorldMapTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

private:
	static WCHAR* m_pwszSheetList[];
};