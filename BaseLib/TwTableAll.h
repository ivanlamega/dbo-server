#ifndef _TW_TABLE_ALL_LOAD
#define _TW_TABLE_ALL_LOAD

#include "TwNewbieTable.h"
#include "TwPC.h"
#include "TwWorldTable.h"
#include <stdlib.h>

using namespace std;
class TwNewbieTable;
class TwPCTable;
class TwWorldTable;

class TwTableAll{
public:
	TwTableAll();
	~TwTableAll();
	void LoadTables();
	void SetPath(string path){ this->sPath = path; };
	TwNewbieTable* GetNewbieTable(){ return m_pNewbieTable;}
	TwPCTable* GetPcTable(){ return m_pPcTable; }
	TwWorldTable* GetWorldTable(){ return m_pWorldTable; }
private:
	string sPath;
	TwNewbieTable* m_pNewbieTable;
	TwPCTable* m_pPcTable;
	TwWorldTable* m_pWorldTable;

};

#endif