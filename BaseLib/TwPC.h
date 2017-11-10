#ifndef _TW_PC_TABLE
#define _TW_PC_TABLE
#include "TableReader.h"
#include "PCTable.h"

class TableReader;

class TwPCTable: public TableReader{
public:
	TwPCTable() {};
	virtual ~TwPCTable() {};

	int LoadEDF(const char *edf_file);
	sPC_TBLDAT2* FindTbldat(BYTE byRace,BYTE byClass,BYTE byGender);
protected:
	int FillStruct(void *record, size_t record_size);
private:
	sPC_TBLDAT2 m_PcList[PC_CLASS_COUNT][GENDER_COUNT];
};
#endif