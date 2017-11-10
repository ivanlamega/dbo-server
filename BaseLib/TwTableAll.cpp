#include "stdafx.h"
#include "TwTableAll.h"

TwTableAll::TwTableAll()
{
	m_pNewbieTable = new TwNewbieTable();
	m_pPcTable = new TwPCTable();
	m_pWorldTable = new TwWorldTable();
}
TwTableAll::~TwTableAll()
{
	m_pNewbieTable->~TwNewbieTable();
	m_pPcTable->~TwPCTable();
	m_pWorldTable->~TwWorldTable();

	m_pNewbieTable = NULL;
	m_pPcTable = NULL;
	m_pWorldTable = NULL;
}

void TwTableAll::LoadTables()
{
	m_pNewbieTable->LoadEDF((sPath+ "\\table_newbie_data.edf").c_str());
	m_pPcTable->LoadEDF((sPath + "\\table_pc_data.edf").c_str());
	m_pWorldTable->LoadEDF((sPath + "\\table_world_data.edf").c_str());
}