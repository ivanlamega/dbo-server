#include "stdafx.h"
#include "TwWorldTable.h"

int TwWorldTable::LoadEDF(const char* edf_file)
{
	return TableReader::LoadEDF(edf_file, sizeof(sWORLD_TBLDAT2));
}

int TwWorldTable::FillStruct(void* record, size_t size)
{
	sWORLD_TBLDAT2* world = (sWORLD_TBLDAT2*)record;

	printf("%d %s, is_dynamic %d, cnt %d, rule_type %d\n",
		world->tblidx,
		world->szName,
		world->bDynamic,
		world->nCreateCount,
		world->byWorldRuleType);

	memcpy(world, record, sizeof(sWORLD_TBLDAT2));
	worlds_[world->tblidx] = world;
	return 0;
}