#include "CharNetwork.h"
#include "CharProtocol.h"

bool CharClient::PacketControl(Packet* pPacket)
{
	LPPACKETDATA data = (LPPACKETDATA)pPacket->GetPacketData();
	Logger::SavePacket(pPacket->GetPacketBuffer());

	switch (data->wOpCode)
	{
	case UC_LOGIN_REQ: SendLoginResult((sUC_LOGIN_REQ*)data); break;
	case UC_CHARACTER_SERVERLIST_REQ: SendServerlist(false); break;
	//case UC_CHARACTER_SERVERLIST_ONE_REQ: SendServerlist(true); break;
	case UC_CHARACTER_ADD_REQ: SendCharCreateRes((sUC_CHARACTER_ADD_REQ*)data); break;
	//case UC_CHARACTER_DEL_REQ: SendCharDelRes((sUC_CHARACTER_DEL_REQ*)data); break;
	case UC_CHARACTER_SELECT_REQ: SendCharSelectRes((sUC_CHARACTER_SELECT_REQ*)data); break;
	case UC_CHARACTER_EXIT_REQ: SendCharExitRes((sUC_CHARACTER_EXIT_REQ*)data); break;
	case UC_CHARACTER_LOAD_REQ: SendCharLoadResult((sUC_CHARACTER_LOAD_REQ*)data); break;
	//case UC_CHARACTER_DEL_CANCEL_REQ: SendCharDelCancelRes((sUC_CHARACTER_DEL_CANCEL_REQ*)data); break;
	case UC_CONNECT_WAIT_CHECK_REQ: SendCharConnWaitCheckRes((sUC_CONNECT_WAIT_CHECK_REQ*)data); break;
	case UC_CONNECT_WAIT_CANCEL_REQ: SendCancelWaitReq((sUC_CONNECT_WAIT_CANCEL_REQ*)data); break;
	//case UC_CHARACTER_RENAME_REQ: SendCharRenameRes((sUC_CHARACTER_RENAME_REQ*)data); break;
	case UC_CHAR_SERVERLIST_REQ: SendServerlist(false); break;
	case UC_CASHITEM_HLSHOP_REFRESH_REQ: break;

	default:
		Logger::Log("Recv Opcode[%d]: %s\n", data->wOpCode, NtlGetPacketName_UC(data->wOpCode));
		return false;
		break;
	}
	return true;
}

void CharClient::SendLoginResult(sUC_LOGIN_REQ* data)
{
	CurrServerID = data->serverID;
	AccountID = data->accountId;
	memcpy(AuthKey, data->abyAuthKey, NTL_MAX_SIZE_AUTH_KEY);
	DBGetGMAccess();

	Logger::Log("Client[%d] entering char server (%d)\n", this, AccountID);

	sCU_LOGIN_RES lRes;
	memset(&lRes, 0, sizeof(sCU_LOGIN_RES));
	lRes.lastServerFarmId = CurrServerID;
	lRes.wOpCode = CU_LOGIN_RES;
	lRes.wResultCode = CHARACTER_SUCCESS;
	lRes.dwRaceAllowedFlag = GetDBAllowedRaces();
	lRes.dwUnknow = 0;
	Send((unsigned char*)&lRes, sizeof(lRes));

	printf("resultcode: %d, serverfarmid: %d, dwracedallowedflag: %d, unknown: %d",
		sizeof(lRes.wResultCode), sizeof(lRes.lastServerFarmId), sizeof(lRes.dwRaceAllowedFlag), sizeof(lRes.dwUnknow));
}

void CharClient::SendServerlist(bool one)
{
	for (int i = 0; i < pServer->ServerCfg->GetInt("ServerCount"); i++)
	{
		char snode[20];
		sprintf_s(snode, "Server%d", i + 1);
		sCU_SERVER_FARM_INFO sinfo;
		memset(&sinfo, 0, sizeof(sCU_SERVER_FARM_INFO));
		sinfo.wOpCode = CU_SERVER_FARM_INFO;
		sinfo.serverFarmInfo.serverFarmId = i + 1;
		memcpy(sinfo.serverFarmInfo.wszGameServerFarmName, charToWChar(pServer->ServerCfg->GetStr(snode, "Name")), NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE);
		sinfo.serverFarmInfo.byServerStatus = DBO_SERVER_STATUS_UP;
		sinfo.serverFarmInfo.dwLoad = 1;
		sinfo.serverFarmInfo.dwMaxLoad = pServer->ServerCfg->GetInt(snode, "MaxLoad");
		Send((unsigned char*)&sinfo, sizeof(sinfo));
	}

	if (one)
	{
		sCU_CHARACTER_SERVERLIST_ONE_RES slone;
		memset(&slone, 0, sizeof(sCU_CHARACTER_SERVERLIST_ONE_RES));
		slone.wOpCode = CU_CHARACTER_SERVERLIST_ONE_RES;
		slone.wResultCode = CHARACTER_SUCCESS;
		Send((unsigned char*)&slone, sizeof(slone));
	}
	else
	{
		sCU_CHARACTER_SERVERLIST_RES slres;
		memset(&slres, 0, sizeof(slres));
		slres.wOpCode = CU_CHARACTER_SERVERLIST_RES;
		slres.wResultCode = CHARACTER_SUCCESS;
		Send((unsigned char*)&slres, sizeof(slres));
	}
}

void CharClient::SendCharLoadResult(sUC_CHARACTER_LOAD_REQ* data)
{
	CurrServerID = data->serverFarmId;
	AccountID = data->accountId;
	DBUpdateLastServer();

	sCU_SERVER_CHANNEL_INFO cninfo;
	memset(&cninfo, 0, sizeof(sCU_SERVER_CHANNEL_INFO));
	cninfo.wOpCode = CU_SERVER_CHANNEL_INFO;
	char snode[20];
	sprintf_s(snode, "Server%d", CurrServerID);
	cninfo.byCount = pServer->ServerCfg->GetInt(snode, "Count");
	for (int x = 0; x < cninfo.byCount; x++)
	{
		char cnode[20];
		sprintf_s(cnode, "Channel%d", x + 1);
		cninfo.serverChannelInfo[x].byServerChannelIndex = x + 1;
		cninfo.serverChannelInfo[x].serverFarmId = CurrServerID;
		cninfo.serverChannelInfo[x].bIsVisible = true;
		cninfo.serverChannelInfo[x].dwLoad = 0;
		cninfo.serverChannelInfo[x].dwMaxLoad = 0;// pServer->ServerCfg->GetChildInt(snode, cnode, "MaxLoad");
		cninfo.serverChannelInfo[x].byServerStatus = DBO_SERVER_STATUS_UP;
		cninfo.serverChannelInfo[x].bIsScramble = false;
	}
	Send((unsigned char*)&cninfo, sizeof(cninfo));

	sCU_CHARACTER_INFO cinfo;
	memset(&cinfo, 0, sizeof(sCU_CHARACTER_INFO));
	GetDBAccCharListData(cinfo);
	cinfo.wOpCode = CU_CHARACTER_INFO;
	Send(&cinfo, sizeof(cinfo));

	Logger::Log("Loaded %d characters from client[%d] (%d)\n", cinfo.byCount, this, AccountID);

	sCU_CHARACTER_LOAD_RES clres;
	memset(&clres, 0, sizeof(sCU_CHARACTER_LOAD_RES));
	clres.wOpCode = CU_CHARACTER_LOAD_RES;
	clres.wResultCode = CHARACTER_SUCCESS;
	clres.ServerFarmId = CurrServerID;
	clres.byOpenCharSlots = 8;
	clres.byVIPCharSlots = 0;
	Send((unsigned char*)&clres, sizeof(clres));
}

void CharClient::SendCharExitRes(sUC_CHARACTER_EXIT_REQ* data)
{
	if (data->bIsGameMove) goGameServer = true;
	sCU_CHARACTER_EXIT_RES cexit;
	memset(&cexit, 0, sizeof(cexit));
	cexit.wOpCode = CU_CHARACTER_EXIT_RES;
	cexit.wResultCode = CHARACTER_SUCCESS;
	Send((unsigned char*)&cexit, sizeof(cexit));
}

void CharClient::SendCharCreateRes(sUC_CHARACTER_ADD_REQ* data)
{
	sCU_CHARACTER_ADD_RES Res;
	memset(&Res, 0, sizeof(Res));
	Res.wOpCode = CU_CHARACTER_ADD_RES;
	Res.wResultCode = CheckUsedName(data->awchCharName);
	/*Logger::Log("Nombre %s\nClase %d\nCara %d\nGenero %d\nPelo %d\nColor pelo %d\nRaza %d\nColor piel %d\nCodePage %d\nbyBlood %d\n",
		data->awchCharName, data->byClass, data->byFace, data->byGender, data->byHair, data->byHairColor,
		data->byRace, data->bySkinColor, data->dwCodePage, data->byBlood);*/
	
	if (Res.wResultCode == CHARACTER_SUCCESS)
	{
		//sNEWBIE_TBLDAT nbdata = *(sNEWBIE_TBLDAT*)pServer->GetTableContainer()->GetNewbieTable()->GetNewbieTbldat(data->byRace, data->byClass);
		sNEWBIE_TBLDAT2* nbdata = (sNEWBIE_TBLDAT2*)pServer->GetTwTable()->GetNewbieTable()->GetNewbieTable(data->byRace, data->byClass);
		memcpy(Res.sPcDataSummary.awchName, data->awchCharName, NTL_MAX_SIZE_CHAR_NAME_UNICODE);
		Res.sPcDataSummary.byClass = data->byClass;
		Res.sPcDataSummary.byFace = data->byFace;
		Res.sPcDataSummary.byGender = data->byGender;
		Res.sPcDataSummary.byHair = data->byHair;
		Res.sPcDataSummary.byHairColor = data->byHairColor;
		Res.sPcDataSummary.byRace = data->byRace;
		Res.sPcDataSummary.bySkinColor = data->bySkinColor;
		Res.sPcDataSummary.byLevel = 1;
		Res.sPcDataSummary.dwMapInfoIndex = nbdata->mapNameTblidx;
		Res.sPcDataSummary.worldId = nbdata->world_Id;
		Res.sPcDataSummary.worldTblidx = nbdata->world_Id;
		Res.sPcDataSummary.fPositionX = nbdata->vSpawn_Loc.x + (float)(rand() % 5);
		Res.sPcDataSummary.fPositionY = nbdata->vSpawn_Loc.y + (float)(rand() % 5);
		Res.sPcDataSummary.fPositionZ = nbdata->vSpawn_Loc.z + (float)(rand() % 5);
		Res.sPcDataSummary.bTutorialFlag = false;
		Res.sPcDataSummary.dwMoney = 10000;
		Res.sPcDataSummary.dwMoneyBank = 100000;
		Res.sPcDataSummary.bIsAdult = false;
		Res.sPcDataSummary.bNeedNameChange = false;
		for (int x = 0; x < EQUIP_SLOT_TYPE_COUNT; x++)
		{
			memset(&Res.sPcDataSummary.sItem[x], 0xFF, sizeof(sITEM_SUMMARY));
		}
		for (int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
		{
			int slot = nbdata->abyPos[i];
			if ((slot >= 0) && (slot <= 12)) 
			{ 
				Res.sPcDataSummary.sItem[slot].tblidx = nbdata->aitem_Tblidx[i];
			}
		}
		Res.sPcDataSummary.charId = DBInsertCharData(Res.sPcDataSummary, nbdata);
	}
	Logger::Log("Client[%d] created character '%S' (%d)\n", this, Res.sPcDataSummary.awchName, Res.sPcDataSummary.charId);
	Send((unsigned char*)&Res, sizeof(Res));

	/*sCU_CHARACTER_ADD_RES Res;
	memset(&Res, 0, sizeof(Res));
	Res.wOpCode = CU_CHARACTER_ADD_RES;
	Res.wResultCode = CheckUsedName(data->awchCharName);
	if (Res.wResultCode == CHARACTER_SUCCESS)
	{
		Logger::Log("Nombre %s\nClase %d\nCara %d\nGenero %d\nPelo %d\nColor pelo %d\nRaza %d\nColor piel %d", 
			data->awchCharName, data->byClass, data->byFace, data->byGender, data->byHair, data->byHairColor,
			data->byRace, data->bySkinColor);
		//sNEWBIE_TBLDAT nbdata = *(sNEWBIE_TBLDAT*)pServer->GetTableContainer()->GetNewbieTable()->GetNewbieTbldat(data->byRace, data->byClass);
		sNEWBIE_TBLDAT2 nbdata = *(sNEWBIE_TBLDAT2*)pServer->GetTwTable()->GetNewbieTable()->GetNewbieTable(data->byRace, data->byClass);
		memcpy(Res.sPcDataSummary.awchName, data->awchCharName, NTL_MAX_SIZE_CHAR_NAME_UNICODE);
		Res.sPcDataSummary.byClass = data->byClass;
		Res.sPcDataSummary.byFace = data->byFace;
		Res.sPcDataSummary.byGender = data->byGender;
		Res.sPcDataSummary.byHair = data->byHair;
		Res.sPcDataSummary.byHairColor = data->byHairColor;
		Res.sPcDataSummary.byRace = data->byRace;
		Res.sPcDataSummary.bySkinColor = data->bySkinColor;
		Res.sPcDataSummary.byLevel = 1;
		Res.sPcDataSummary.dwMapInfoIndex = nbdata.mapNameTblidx;
		Res.sPcDataSummary.worldId = nbdata.world_Id;
		Res.sPcDataSummary.worldTblidx = nbdata.world_Id;
		Res.sPcDataSummary.fPositionX = nbdata.vSpawn_Loc.x + (float)(rand() % 5);
		Res.sPcDataSummary.fPositionY = nbdata.vSpawn_Loc.y + (float)(rand() % 5);
		Res.sPcDataSummary.fPositionZ = nbdata.vSpawn_Loc.z + (float)(rand() % 5);
		Res.sPcDataSummary.bTutorialFlag = false;
		Res.sPcDataSummary.dwMoney = 10000;
		Res.sPcDataSummary.dwMoneyBank = 100000;
		Res.sPcDataSummary.bIsAdult = false;
		Res.sPcDataSummary.bNeedNameChange = false;
		for (int x = 0; x < EQUIP_SLOT_TYPE_COUNT; x++)
		{
			memset(&Res.sPcDataSummary.sItem[x], 0xFF, sizeof(sITEM_SUMMARY));
		}
		for (int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
		{
			int slot = nbdata.abyPos[i];
			if ((slot >= 0) && (slot <= 12)) { 
				Res.sPcDataSummary.sItem[slot].tblidx = nbdata.aitem_Tblidx[i];
			}
		}
		Res.sPcDataSummary.charId = DBInsertCharData(Res.sPcDataSummary, nbdata);
	}
	Logger::Log("Client[%d] created character '%S' (%d)\n", this, Res.sPcDataSummary.awchName, Res.sPcDataSummary.charId);
	Send((unsigned char*)&Res, sizeof(Res));*/
}

void CharClient::SendCharDelRes(sUC_CHARACTER_DEL_REQ* data)
{
	sCU_CHARACTER_DEL_RES Res;
	memset(&Res, 0, sizeof(Res));
	Res.wOpCode = CU_CHARACTER_DEL_RES;
	Res.charId = data->charId;
	if (pServer->ServerDB->ExecuteQuery("UPDATE `character` SET `ToDelete` = '1' WHERE `ID` = '%d';", Res.charId))
		Res.wResultCode = CHARACTER_SUCCESS;
	else
		Res.wResultCode = CHARACTER_DELETE_CHAR_FAIL;
	Send((unsigned char*)&Res, sizeof(Res));
}

void CharClient::SendCharDelCancelRes(sUC_CHARACTER_DEL_CANCEL_REQ* data)
{
	sCU_CHARACTER_DEL_CANCEL_RES Res;
	memset(&Res, 0, sizeof(Res));
	Res.wOpCode = CU_CHARACTER_DEL_CANCEL_RES;
	Res.charId = data->charId;
	if (pServer->ServerDB->ExecuteQuery("UPDATE `character` SET `ToDelete` = '0' WHERE `ID` = '%d';", Res.charId))
		Res.wResultCode = CHARACTER_SUCCESS;
	else
		Res.wResultCode = CHARACTER_DB_QUERY_FAIL;
	Send((unsigned char*)&Res, sizeof(Res));
}

void CharClient::SendCharConnWaitCheckRes(sUC_CONNECT_WAIT_CHECK_REQ* data)
{
	CurrChannelID = data->byServerChannelIndex;

	sCU_CONNECT_WAIT_CHECK_RES checkRes;
	checkRes.wOpCode = CU_CONNECT_WAIT_CHECK_RES;
	checkRes.wResultCode = GAME_SUCCESS;
	Send((unsigned char*)&checkRes, sizeof(checkRes));

	sCU_CONNECT_WAIT_COUNT_NFY connNfy;
	connNfy.wOpCode = CU_CONNECT_WAIT_COUNT_NFY;
	connNfy.dwCountWaiting = 0;
	Send((unsigned char*)&connNfy, sizeof(connNfy));
}

void CharClient::SendCharSelectRes(sUC_CHARACTER_SELECT_REQ* data)
{
	CurrCharID = data->charId;
	CurrChannelID = data->byServerChannelIndex;

	sCU_CHARACTER_SELECT_RES selRes;
	memset(&selRes, 0, sizeof(selRes));
	selRes.wOpCode = CU_CHARACTER_SELECT_RES;
	memcpy(selRes.abyAuthKey, AuthKey, NTL_MAX_SIZE_AUTH_KEY);
	selRes.charId = CurrCharID;
	char snode[20], cnode[20];
	sprintf_s(snode, "Server%d", CurrServerID);
	sprintf_s(cnode, "Channel%d", CurrChannelID);
	memcpy(selRes.szGameServerIP, pServer->ServerCfg->GetChildStr(snode, cnode, "IP"), NTL_MAX_LENGTH_OF_IP);
	selRes.wGameServerPortForClient = pServer->ServerCfg->GetChildInt(snode, cnode, "Port");
	selRes.wResultCode = CHARACTER_SUCCESS;
	Logger::Log("Client[%d] selected character (%d)\n", this, selRes.charId);
	Send((unsigned char*)&selRes, sizeof(selRes));
}

void CharClient::SendCharRenameRes(sUC_CHARACTER_RENAME_REQ* data)
{
	sCU_CHARACTER_RENAME_RES res;
	res.wOpCode = CU_CHARACTER_RENAME_RES;
	res.charId = data->charId;
	res.wResultCode = DBChangeCharName(data->awchCharName, data->charId);
	Send(&res, sizeof(res));
}

void CharClient::SendCancelWaitReq(sUC_CONNECT_WAIT_CANCEL_REQ* data)
{
	sCU_CONNECT_WAIT_CANCEL_RES res;
	res.wOpCode = CU_CONNECT_WAIT_CANCEL_RES;
	res.wResultCode = CHARACTER_SUCCESS;
	Send(&res, sizeof(res));
}