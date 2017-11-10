#include "stdafx.h"
#include "TwPC.h"

int TwPCTable::LoadEDF(const char* edf_file)
{
	return TableReader::LoadEDF(edf_file, sizeof(sPC_TBLDAT2));
}

int TwPCTable::FillStruct(void* record, size_t size)
{
	sPC_TBLDAT2* pc = (sPC_TBLDAT2*)record;

	printf("%s: %d %d %d %d [%s] [%s]\n", __FUNCTION__, pc->tblidx,
		pc->byRace, pc->byClass, pc->byGender,
		pc->szModel_Child, pc->szModel_Adult);

	if (pc->tblidx != 0xcd)
		m_PcList[pc->byClass][pc->byGender] = *pc;
	return 0;
}

sPC_TBLDAT2* TwPCTable::FindTbldat(BYTE byRace, BYTE byClass, BYTE byGender)
{
	if (byRace < RACE_FIRST ||
		byRace >= RACE_COUNT ||
		byClass < PC_CLASS_1_FIRST ||
		byClass >= PC_CLASS_2_LAST ||
		byGender < GENDER_FIRST ||
		byGender >= GENDER_COUNT)
	{
		return NULL;
	}

	return &m_PcList[byClass][byGender];
}