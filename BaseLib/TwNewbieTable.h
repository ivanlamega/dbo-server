#ifndef _TW_NEWBIE_TABLE
#define _TW_NEWBIE_TABLE

#include "TableReader.h"
#include "NewbieTable.h"
class TableReader;

class TwNewbieTable :public TableReader{
public:
	TwNewbieTable() {};
	virtual ~TwNewbieTable() {};

	int LoadEDF(const char *edf_file);
	sNEWBIE_TBLDAT2* GetNewbieTable(BYTE byCharRace, BYTE byCharClass);
protected:
	int FillStruct(void *record, size_t record_size);
private:
	sNEWBIE_TBLDAT2 m_newbieList[PC_CLASS_COUNT];

};

#endif