#include "CharNetwork.h"

int CharServer::OnInitApp()
{
	_MaxSessionCount = MAX_NUMOF_SESSION;

	_SessionFactory = new CharClientFactory;
	if (NULL == _SessionFactory)
	{
		return 1;//ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	return 0;
}

int CharServer::OnCreate()
{
	int rc = 0;
	rc = _clientAcceptor.Create(ServerCfg->GetStr("Server", "IP"),
		ServerCfg->GetInt("Server", "Port"),
		SESSION_CLIENT,
		MAX_NUMOF_GAME_CLIENT, 5, 2, MAX_NUMOF_GAME_CLIENT);
	if (0 != rc)
	{
		return rc;
	}

	rc = _network.Associate(&_clientAcceptor, true);
	if (0 != rc)
	{
		return rc;
	}

	ServerDB = new Database();
	if (!ServerDB->Connect(
		ServerCfg->GetStr("MySQL", "Host"),
		ServerCfg->GetStr("MySQL", "Database"),
		ServerCfg->GetStr("MySQL", "User"),
		ServerCfg->GetStr("MySQL", "Password"),
		ServerCfg->GetInt("MySQL", "Port")))
		return 2;//ERR_DBSERVER_CONNECT

	// TABLE CONTAINER LOAD
	gameDataPath = ServerCfg->GetStr("GameData", "Path");
	Logger::Log("Loading Game Data Tables ... PLEASE WAIT\n");
	if (LoadTableData())
		Logger::Log("LOADED!\n");
	else
		return 3;//ERR_TABLE_LOAD

	m_uiSerialID = INVALID_DWORD;
	return 0;
}

int CharServer::OnConfiguration(const char* ConfigFile)
{
	ServerCfg = new Config(ConfigFile);
	return 0;
}

int CharServer::OnAppStart()
{
	Logger::Log("Server listening on %s:%d\n", _clientAcceptor.GetListenIP(), _clientAcceptor.GetListenPort());
	return 0;
}

bool CharServer::LoadTableData()
{
	m_pTwTableAll = new TwTableAll();
	m_pTwTableAll->SetPath("TwTables/decrypted");
	m_pTwTableAll->LoadTables();
	//return true;
	CNtlBitFlagManager flagManager;
	if (false == flagManager.Create(TableContainer::TABLE_COUNT)) return false;

	TableFileNameList fileNameList;
	if (false == fileNameList.Create())	return false;

	flagManager.Set(TableContainer::TABLE_WORLD);
	flagManager.Set(TableContainer::TABLE_PC);
	flagManager.Set(TableContainer::TABLE_MOB);
	flagManager.Set(TableContainer::TABLE_NPC);
	flagManager.Set(TableContainer::TABLE_ITEM);
	flagManager.Set(TableContainer::TABLE_ITEM_OPTION);
	flagManager.Set(TableContainer::TABLE_SKILL);
	flagManager.Set(TableContainer::TABLE_NEWBIE);
	flagManager.Set(TableContainer::TABLE_WORLD_MAP);
	flagManager.Set(TableContainer::TABLE_WORLD_ZONE);
	flagManager.Set(TableContainer::TABLE_FORMULA);
	flagManager.Set(TableContainer::TABLE_EXP);

	fileNameList.SetFileName(TableContainer::TABLE_WORLD, "Table_World_Data");
	fileNameList.SetFileName(TableContainer::TABLE_PC, "Table_PC_Data");
	fileNameList.SetFileName(TableContainer::TABLE_ITEM, "Table_Item_Data");
	fileNameList.SetFileName(TableContainer::TABLE_ITEM_OPTION, "Table_Item_Option_Data");
	fileNameList.SetFileName(TableContainer::TABLE_SKILL, "Table_Skill_Data");
	fileNameList.SetFileName(TableContainer::TABLE_NEWBIE, "Table_Newbie_Data");
	fileNameList.SetFileName(TableContainer::TABLE_WORLD_MAP, "Table_Worldmap_Data");
	fileNameList.SetFileName(TableContainer::TABLE_WORLD_ZONE, "Table_World_Zone_Data");
	fileNameList.SetFileName(TableContainer::TABLE_FORMULA, "TD_Formula");
	fileNameList.SetFileName(TableContainer::TABLE_GAME_MANIA_TIME, "Table_GameManiaTime_Data");
	fileNameList.SetFileName(TableContainer::TABLE_EXP, "table_exp_data");

	m_pTableContainer = new TableContainer();

	return m_pTableContainer->Create(flagManager, gameDataPath, &fileNameList, eLOADING_METHOD::LOADING_METHOD_SECURED_BINARY, GetACP(), NULL);
	
}

unsigned int CharServer::AcquireSerialID()
{
	if (m_uiSerialID++)
	{
		if (m_uiSerialID == INVALID_DWORD)
			m_uiSerialID = 0;
	}

	return m_uiSerialID;
}