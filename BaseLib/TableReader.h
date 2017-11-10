#ifndef DBO_TABLE_READER
#define DBO_TABLE_READER
#include "Base.h"
#include <iostream>
#include <fstream>
class TableReader{
	protected:
		int LoadEDF(const char* FileEdf, size_t size);
		virtual int FillStruct(void* record, size_t sizeRecord);
	public:
		TableReader(){};
		virtual ~TableReader(){};
		virtual int LoadEDF(const char* FileEdf) = 0;
};
#endif