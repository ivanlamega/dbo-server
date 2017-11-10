#ifndef _TW_WORLD_TABLE
#define _TW_WORLD_TABLE

#include "TableReader.h"
#include "WorldTable.h"
class TableReader;

class TwWorldTable :public TableReader{
private:
	typedef std::map<TBLIDX, sWORLD_TBLDAT2 *> worldMap;
	worldMap worlds_;
public:
	TwWorldTable() {};
	virtual ~TwWorldTable() {};

	int LoadEDF(const char *edf_file);
	worldMap* GetWorldMap(){ return &worlds_; }
protected:
	int FillStruct(void *record, size_t record_size);
};

#endif