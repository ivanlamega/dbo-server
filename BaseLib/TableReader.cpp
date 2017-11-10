#include "stdafx.h"
#include "TableReader.h"

int TableReader::LoadEDF(const char* file, size_t size)
{
	UINT8 padding;
	char *record;
	size_t ret;
	UINT32 data_size;
	FILE *f;

	fopen_s(&f,file, "rb");

	if (!f)
	{
		return -1;
	}

	/* seems to be always 0 */
	fread(&padding, sizeof(padding), 1, f);

	fread(&data_size, sizeof(data_size), 1, f);

	/* seems to be always 1 */
	fread(&padding, sizeof(padding), 1, f);

	record = new char[size];

	memset(record, 0, size);

	while (!feof(f))
	{
		ret = fread(record, size, 1, f);

		if (ret == 1)
		{
			if (FillStruct(record, ret) != 0)
			{
				return -1;
			}
		}
		else if (!feof(f))
		{
			printf("error loading [%s]: read %d vs %d\n",file,size,ret);
			return -1;
		}
	}

	delete[] record;
	fclose(f);
	f = NULL;

	return 0;
}
int TableReader::FillStruct(void* record, size_t size)
{
	return 0;
}