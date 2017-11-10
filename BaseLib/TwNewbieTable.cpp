#include "stdafx.h"
#include "TwNewbieTable.h"

int TwNewbieTable::LoadEDF(const char* edf_file)
{
	return TableReader::LoadEDF(edf_file, sizeof(sNEWBIE_TBLDAT2));
}

int TwNewbieTable::FillStruct(void* record, size_t size)
{

	sNEWBIE_TBLDAT2* newbie = (sNEWBIE_TBLDAT2*)record;

	printf("%s: %d %d %d %d [%s] [%s]\n", __FUNCTION__, newbie->tblidx,
		newbie->byRace, newbie->byClass, newbie->world_Id);

	if (newbie->tblidx != 0xcd)
		m_newbieList[newbie->byClass] = *newbie;
	return 0;
}

sNEWBIE_TBLDAT2* TwNewbieTable::GetNewbieTable(BYTE byCharRace, BYTE byCharClass)
{
	if (byCharRace < RACE_FIRST ||
		byCharRace >= RACE_COUNT ||
		byCharClass < PC_CLASS_1_FIRST ||
		byCharClass >= PC_CLASS_2_LAST)
	{
		return NULL;
	}

	return &m_newbieList[byCharRace];
}