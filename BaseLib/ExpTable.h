#pragma once

#include "Table.h"

#pragma pack(push, 4)
struct sEXP_TBLDAT : public sTBLDAT
{
public:
	DWORD		dwExp;		// Required exp to become the level from level 1.
	DWORD		dwNeed_Exp;		// Required exp to become the level from the previous level.
	BYTE		UNKNOW[40];		// Need to reasearch, but i assume its level atributes from newer client

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class ExpTable : public Table
{
public:
	ExpTable(void);
	virtual ~ExpTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

	sTBLDAT* FindData(TBLIDX tblidx);

	virtual bool LoadFromBinary(Serializer& serializer, bool bReload);
	virtual bool SaveToBinary(Serializer& serializer);

protected:
	void Init();

	WCHAR** GetSheetListInWChar() { return &(ExpTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

private:
	static WCHAR* m_pwszSheetList[];
};