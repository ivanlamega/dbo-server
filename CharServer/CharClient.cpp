#include "CharNetwork.h"

CharClient::CharClient(bool IsAliveCheck, bool IsOpcodeCheck)
	:Session(SESSION_CLIENT)
{
	SetControlFlag(CONTROL_FLAG_USE_SEND_QUEUE);

	if (IsAliveCheck)
	{
		SetControlFlag(CONTROL_FLAG_CHECK_ALIVE);
	}
	if (IsOpcodeCheck)
	{
		SetControlFlag(CONTROL_FLAG_CHECK_OPCODE);
	}
	
	// Disable Encoding
	//SetPacketEncoder(&_packetEncoder);
	pServer = (CharServer*)_GetApp();
	bIsGM = false;
}

CharClient::~CharClient()
{
}

int	CharClient::OnAccept()
{
	return 0;
}

void CharClient::OnClose()
{
	if (goGameServer) pServer->ServerDB->ExecuteQuery("UPDATE `account` SET `State` = '3' WHERE `ID` = '%d';", AccountID);
	else pServer->ServerDB->ExecuteQuery("UPDATE `account` SET `State` = '0' WHERE `ID` = '%d';", AccountID);
}

int CharClient::OnDispatch(Packet* pPacket)
{
	PacketControl(pPacket);
	//	return OnDispatch(pPacket);
	return 0;
}

void CharClient::Send(void* pData, int nSize)
{
	Packet* packet = new Packet((unsigned char*)pData, nSize);
	Logger::SavePacket(packet->GetPacketBuffer());
	int rc = pServer->Send(this->GetHandle(), packet);
	if (0 != rc)
	{
		Logger::Log("Failed to send packet %d\n", rc);
	}
}

void CharClient::DBGetGMAccess()
{
	if (pServer->ServerDB->ExecuteSelect("SELECT `isGameMaster` FROM `account` WHERE ID='%d'", AccountID))
	{
		while (pServer->ServerDB->Fetch())
		{
			bIsGM = pServer->ServerDB->getBoolean("isGameMaster");
		}
	}
}

void CharClient::DBUpdateLastServer()
{
	if (pServer->ServerDB->ExecuteQuery("UPDATE `account` SET `LastServerID` = '%d' WHERE `ID` = '%d';", CurrServerID, AccountID))
		LastServerID = CurrServerID;
}

int CharClient::GetDBAllowedRaces()
{
	int allraces = 0xFF;
	if (pServer->ServerDB->ExecuteSelect("SELECT `AllowedRace` FROM account WHERE ID='%d'", AccountID))
	{
		while (pServer->ServerDB->Fetch())
		{
			allraces = pServer->ServerDB->getInt("AllowedRace");
		}
	}
	return allraces;
}

eRESULTCODE CharClient::CheckUsedName(WCHAR* Name)
{
	if (pServer->ServerDB->ExecuteSelect("SELECT * FROM `character` WHERE `Name`='%S';", Name))
	{
		pServer->ServerDB->Fetch();
		if (pServer->ServerDB->rowsCount() == 0)
			return CHARACTER_SUCCESS;
		else
			return CHARACTER_SAMENAME_EXIST;
	}
	return CHARACTER_DB_QUERY_FAIL;
}

eRESULTCODE CharClient::DBChangeCharName(WCHAR* Name, int charId)
{
	if (pServer->ServerDB->ExecuteSelect("SELECT * FROM `character` WHERE `Name`='%S';", Name))
	{
		pServer->ServerDB->Fetch();
		if (pServer->ServerDB->rowsCount() == 0)
		{
			pServer->ServerDB->ExecuteQuery("UPDATE `character` SET `Name` = '%S' WHERE `ID` = '%d';", Name, charId);
			return CHARACTER_SUCCESS;
		}
		else
			return CHARACTER_SAMENAME_EXIST;
	}
	return CHARACTER_DB_QUERY_FAIL;
}

int CharClient::DBInsertCharData(sPC_SUMMARY data, sNEWBIE_TBLDAT2* nbdata)
{
	int charid = 0;
	sITEM_TBLDAT sItem;
	//sPC_TBLDAT pcdata = *(sPC_TBLDAT*)pServer->GetTableContainer()->GetPcTable()->GetPcTbldat(data.byRace, data.byClass, data.byGender);
	sPC_TBLDAT2 pcdata = *(sPC_TBLDAT2*)pServer->GetTwTable()->GetPcTable()->FindTbldat(data.byRace, data.byClass, data.byGender);
	
	// CALL `spInsertCharacter` ('AccID','ServerID','Name','Race','Class','Gender','Face','Hair','HairColor','SkinColor','Level','0','MapInfoId', 'worldTblidx', 'worldId', '0', '0', '0', 'PositionX', 'PositionY', 'PositionZ', 'DirectionX', 'DirectionY', 'DirectionZ', 'Money', 'MoneyBank', 'Marking', 'Adult', 'TutorialFlag', 'NeedNameChange', 'ToDelete', 'ChangeClass', 'IsGameMaster', 'TutorialHint', 'Reputation', 'MudosaPoint', 'SpPoint');
	pServer->ServerDB->ExecuteQuery(
		"CALL `spInsertCharacter` ('%u','%u','%S','%d','%d','%d','%d','%d','%d','%d','%d','0','%u','%u','%u','0','0','0','%f','%f','%f','%f','%f','%f','%u','%u','255','%d','%d','%d','0','0','%d','0','0','0','0');",
		AccountID, CurrServerID, data.awchName, data.byRace, data.byClass, data.byGender, data.byFace, data.byHair, data.byHairColor, data.bySkinColor, data.byLevel, data.dwMapInfoIndex, data.worldTblidx, data.worldId, data.fPositionX, data.fPositionY,
		data.fPositionZ, nbdata->vSpawn_Dir.x, nbdata->vSpawn_Dir.y, nbdata->vSpawn_Dir.z, data.dwMoney, data.dwMoneyBank, data.bIsAdult, data.bTutorialFlag, data.bNeedNameChange, bIsGM);
	if (pServer->ServerDB->ExecuteSelect("SELECT `ID` FROM `character` WHERE `AccID`='%d' AND `Name`='%S';", AccountID, data.awchName))
	{
		while (pServer->ServerDB->Fetch())
		{
			charid = pServer->ServerDB->getInt("ID");
		}
	}

	// BAGS CONAINERS
	sItem = *(sITEM_TBLDAT*)pServer->GetTableContainer()->GetItemTable()->FindData(19901);
	// (nItemID,nCharID,nPlace,nSlot,nStack,nRank,nCurDur,nNeedToIdentify,nGrade,nBattleAttribute,nRestrictType,nMaker,nOpt1,nOpt2,nDurationType);
	pServer->ServerDB->ExecuteQuery("CALL `spQueryInsertItem`('%u','%u','%d','%d','%d','%d','%d','0','0','%d','0','','%u','0','%d','%d');",
		sItem.tblidx, charid, CONTAINER_TYPE_BAGSLOT, 0, 1, sItem.byRank, sItem.byDurability, sItem.byBattle_Attribute, sItem.Item_Option_Tblidx, sItem.byDurationType, pServer->AcquireSerialID());
	sItem = *(sITEM_TBLDAT*)pServer->GetTableContainer()->GetItemTable()->FindData(19991);
	// (nItemID,nCharID,nPlace,nSlot,nStack,nRank,nCurDur,nNeedToIdentify,nGrade,nBattleAttribute,nRestrictType,nMaker,nOpt1,nOpt2,nDurationType);
	pServer->ServerDB->ExecuteQuery("CALL `spQueryInsertItem`('%u','%u','%d','%d','%d','%d','%d','0','0','%d','0','','%u','0','%d','%d');",
		sItem.tblidx, charid, CONTAINER_TYPE_BANKSLOT, 0, 1, sItem.byRank, sItem.byDurability, sItem.byBattle_Attribute, sItem.Item_Option_Tblidx, sItem.byDurationType, pServer->AcquireSerialID());

	// START ITEMS
	for (int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
	{
		int slot = nbdata->abyPos[i];
		if ((slot >= 0) && (slot <= 12))
		{
			//aqui hay un error
			Logger::Log("Items: %d", nbdata->aitem_Tblidx[i]);
			sItem = *(sITEM_TBLDAT*)pServer->GetTableContainer()->GetItemTable()->FindData(19901);
			// (nItemID,nCharID,nPlace,nSlot,nStack,nRank,nCurDur,nNeedToIdentify,nGrade,nBattleAttribute,nRestrictType,nMaker,nOpt1,nOpt2,nDurationType);
			HOBJECT handleItem = pServer->AcquireSerialID();
			pServer->ServerDB->ExecuteQuery("CALL `spQueryInsertItem`('%u','%u','%d','%d','%d','%d','%d','0','0','%d','0','','%u','0','%d','%d');",
				sItem.tblidx, charid, CONTAINER_TYPE_EQUIP, slot, nbdata->abyStack_Quantity[i], sItem.byRank, sItem.byDurability, sItem.byBattle_Attribute, sItem.Item_Option_Tblidx, sItem.byDurationType, handleItem);
		}
	}

	// START QUICKSLOTS CONFIG
	for (int i = 0; i < NTL_MAX_NEWBIE_QUICKSLOT_COUNT; i++)
	{
		pServer->ServerDB->ExecuteQuery("INSERT INTO `quickslot` (`CharID`,`TblID`,`Slot`,`Type`) VALUES ('%u','%u','%u','%u');",
			charid, nbdata->asQuickData[i].tbilidx, nbdata->asQuickData[i].byQuickSlot, nbdata->asQuickData[i].byType);
	}

	// START SKILLS
	for (int i = 0; i < NTL_MAX_NEWBIE_SKILL; i++)
	{
		if (nbdata->aSkillTblidx[i] != INVALID_DWORD)
			pServer->ServerDB->ExecuteQuery("INSERT INTO `skills` (`CharID`, `SkillID`, `Slot`) VALUES ('%u','%u','%u');",
			charid, nbdata->aSkillTblidx[i], i + 1);
	}
	return charid;
	/*int charid = 0;
	sITEM_TBLDAT sItem;
	//sPC_TBLDAT pcdata = *(sPC_TBLDAT*)pServer->GetTableContainer()->GetPcTable()->GetPcTbldat(data.byRace, data.byClass, data.byGender);
	sPC_TBLDAT2 pcdata = *(sPC_TBLDAT2*)pServer->GetTwTable()->GetPcTable()->FindTbldat(data.byRace, data.byClass, data.byGender);
	// CALL `spInsertCharacter` ('AccID','ServerID','Name','Race','Class','Gender','Face','Hair','HairColor','SkinColor','Level','0','MapInfoId', 'worldTblidx', 'worldId', '0', '0', '0', 'PositionX', 'PositionY', 'PositionZ', 'DirectionX', 'DirectionY', 'DirectionZ', 'Money', 'MoneyBank', 'Marking', 'Adult', 'TutorialFlag', 'NeedNameChange', 'ToDelete', 'ChangeClass', 'IsGameMaster', 'TutorialHint', 'Reputation', 'MudosaPoint', 'SpPoint');
	pServer->ServerDB->ExecuteQuery(
		"CALL `spInsertCharacter` ('%u','%u','%S','%d','%d','%d','%d','%d','%d','%d','%d','0','%u','%u','%u','0','0','0','%f','%f','%f','%f','%f','%f','%u','%u','255','%d','%d','%d','0','0','%d','0','0','0','0');",
		AccountID, CurrServerID, data.awchName, data.byRace, data.byClass, data.byGender, data.byFace, data.byHair, data.byHairColor, data.bySkinColor, data.byLevel, data.dwMapInfoIndex, data.worldTblidx, data.worldId, data.fPositionX, data.fPositionY,
		data.fPositionZ, nbdata->vSpawn_Dir.x, nbdata->vSpawn_Dir.y, nbdata->vSpawn_Dir.z, data.dwMoney, data.dwMoneyBank, data.bIsAdult, data.bTutorialFlag, data.bNeedNameChange, bIsGM);
	if (pServer->ServerDB->ExecuteSelect("SELECT `ID` FROM `character` WHERE `AccID`='%d' AND `Name`='%S';", AccountID, data.awchName))
	{
		while (pServer->ServerDB->Fetch())
		{
			charid = pServer->ServerDB->getInt("ID");
		}
	}


	// BAGS CONAINERS
	sItem = *(sITEM_TBLDAT*)pServer->GetTableContainer()->GetItemTable()->FindData(19901);
	// (nItemID,nCharID,nPlace,nSlot,nStack,nRank,nCurDur,nNeedToIdentify,nGrade,nBattleAttribute,nRestrictType,nMaker,nOpt1,nOpt2,nDurationType);
	pServer->ServerDB->ExecuteQuery("CALL `spQueryInsertItem`('%u','%u','%d','%d','%d','%d','%d','0','0','%d','0','','%u','0','%d','%d');",
		sItem.tblidx, charid, CONTAINER_TYPE_BAGSLOT, 0, 1, sItem.byRank, sItem.byDurability, sItem.byBattle_Attribute, sItem.Item_Option_Tblidx, sItem.byDurationType,pServer->AcquireSerialID());
	sItem = *(sITEM_TBLDAT*)pServer->GetTableContainer()->GetItemTable()->FindData(19991);
	// (nItemID,nCharID,nPlace,nSlot,nStack,nRank,nCurDur,nNeedToIdentify,nGrade,nBattleAttribute,nRestrictType,nMaker,nOpt1,nOpt2,nDurationType);
	pServer->ServerDB->ExecuteQuery("CALL `spQueryInsertItem`('%u','%u','%d','%d','%d','%d','%d','0','0','%d','0','','%u','0','%d','%d');",
		sItem.tblidx, charid, CONTAINER_TYPE_BANKSLOT, 0, 1, sItem.byRank, sItem.byDurability, sItem.byBattle_Attribute, sItem.Item_Option_Tblidx, sItem.byDurationType, pServer->AcquireSerialID());
	

	// START ITEMS
	for (int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
	{
		int slot = nbdata->abyPos[i];
		if ((slot >= 0) && (slot <= 12))
		{
			sItem = *(sITEM_TBLDAT*)pServer->GetTableContainer()->GetItemTable()->FindData(nbdata->aitem_Tblidx[i]);
			// (nItemID,nCharID,nPlace,nSlot,nStack,nRank,nCurDur,nNeedToIdentify,nGrade,nBattleAttribute,nRestrictType,nMaker,nOpt1,nOpt2,nDurationType);
			HOBJECT handleItem = pServer->AcquireSerialID();
			pServer->ServerDB->ExecuteQuery("CALL `spQueryInsertItem`('%u','%u','%d','%d','%d','%d','%d','0','0','%d','0','','%u','0','%d','%d');",
				sItem.tblidx, charid, CONTAINER_TYPE_EQUIP, slot, nbdata->abyStack_Quantity[i], sItem.byRank, sItem.byDurability, sItem.byBattle_Attribute, sItem.Item_Option_Tblidx, sItem.byDurationType, handleItem);
		}
	}


	// START QUICKSLOTS CONFIG
	for (int i = 0; i < NTL_MAX_NEWBIE_QUICKSLOT_COUNT; i++)
	{
		pServer->ServerDB->ExecuteQuery("INSERT INTO `quickslot` (`CharID`,`TblID`,`Slot`,`Type`) VALUES ('%u','%u','%u','%u');",
			charid, nbdata->asQuickData[i].tbilidx, nbdata->asQuickData[i].byQuickSlot, nbdata->asQuickData[i].byType);
	}


	// START SKILLS
	for (int i = 0; i < NTL_MAX_NEWBIE_SKILL; i++)
	{
		if (nbdata->aSkillTblidx[i] != INVALID_DWORD)
			pServer->ServerDB->ExecuteQuery("INSERT INTO `skills` (`CharID`, `SkillID`, `Slot`) VALUES ('%u','%u','%u');",
				charid, nbdata->aSkillTblidx[i], i + 1);
	}
	return charid;*/
}

void CharClient::GetDBAccCharListData(sCU_CHARACTER_INFO& outdata)
{
	try
	{
		// Clear Structs
		memset(outdata.asDelData, 0xFF, sizeof(outdata.asDelData));
		memset(outdata.sPcData, 0xFF, sizeof(outdata.sPcData));

		int c = 0;
		if (pServer->ServerDB->ExecuteSelect("SELECT * FROM `character` WHERE `AccID`='%d' AND `ServerID`='%d' LIMIT 8;", AccountID, CurrServerID))
		{
			while (pServer->ServerDB->Fetch())
			{
				outdata.sPcData[c].charId = pServer->ServerDB->getInt("ID");
				memcpy(outdata.sPcData[c].awchName, charToWChar(pServer->ServerDB->getString("Name")), NTL_MAX_SIZE_CHAR_NAME_UNICODE);
				outdata.sPcData[c].byClass = pServer->ServerDB->getInt("Class");
				outdata.sPcData[c].byFace = pServer->ServerDB->getInt("Face");
				outdata.sPcData[c].byGender = pServer->ServerDB->getInt("Gender");
				outdata.sPcData[c].byHair = pServer->ServerDB->getInt("Hair");
				outdata.sPcData[c].byHairColor = pServer->ServerDB->getInt("HairColor");
				outdata.sPcData[c].bIsAdult = pServer->ServerDB->getBoolean("Adult");
				outdata.sPcData[c].byLevel = pServer->ServerDB->getInt("Level");
				outdata.sPcData[c].bNeedNameChange = pServer->ServerDB->getBoolean("NeedNameChange");
				outdata.sPcData[c].byRace = pServer->ServerDB->getInt("Race");
				outdata.sPcData[c].bySkinColor = pServer->ServerDB->getInt("SkinColor");
				outdata.sPcData[c].worldTblidx = pServer->ServerDB->getInt("worldTblidx");
				outdata.sPcData[c].worldId = pServer->ServerDB->getInt("worldId");
				outdata.sPcData[c].fPositionX = pServer->ServerDB->getFloat("PositionX");
				outdata.sPcData[c].fPositionY = pServer->ServerDB->getFloat("PositionY");
				outdata.sPcData[c].fPositionZ = pServer->ServerDB->getFloat("PositionZ");
				outdata.sPcData[c].dwMoney = pServer->ServerDB->getInt("Money");
				outdata.sPcData[c].dwMoneyBank = pServer->ServerDB->getInt("MoneyBank");
				outdata.sPcData[c].dwMapInfoIndex = pServer->ServerDB->getInt("MapInfoId");
				outdata.sPcData[c].bTutorialFlag = pServer->ServerDB->getBoolean("TutorialFlag");
				outdata.sPcData[c].sMarking.dwCode = pServer->ServerDB->getInt("Marking");
				outdata.sPcData[c].wUnknow1 = 0;

				// Check to delete char flag
				if (pServer->ServerDB->getBoolean("ToDelete"))
				{
					outdata.asDelData[c].charId = pServer->ServerDB->getInt("ID");
					outdata.asDelData[c].dwPastTick = GetTickCount();
				}
				else
				{
					outdata.asDelData[c].charId = INVALID_DWORD;
					outdata.asDelData[c].dwPastTick = INVALID_DWORD;
				}
				c++;
			}
		}
		for (int i = 0; i < c; i++)
		{
			if (pServer->ServerDB->ExecuteSelect("SELECT * FROM `inventory` WHERE `CharID`='%u';", outdata.sPcData[i].charId))
			{
				while (pServer->ServerDB->Fetch())
				{
					int slot = pServer->ServerDB->getInt("Slot");
					int place = pServer->ServerDB->getInt("Place");
					if (((slot >= EQUIP_SLOT_TYPE_FIRST) && (slot <= EQUIP_SLOT_TYPE_LAST)) && place == CONTAINER_TYPE_EQUIP)
					{
						outdata.sPcData[i].sItem[slot].tblidx = pServer->ServerDB->getInt("ItemID");
						outdata.sPcData[i].sItem[slot].byRank = pServer->ServerDB->getInt("Rank");
						outdata.sPcData[i].sItem[slot].byGrade = pServer->ServerDB->getInt("Grade");
						outdata.sPcData[i].sItem[slot].byBattleAttribute = pServer->ServerDB->getInt("BattleAttribute");
					}
				}
			}
		}
		outdata.byCount = c;
	}
	catch (SQLException &e) {
		Logger::Log("# ERROR #\n");
		std::cout << "\t\tSQLException in " << __FILE__ << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "\t\t" << e.what() << " (MySQL error code: " << e.getErrorCode() << " SQLState: " << e.getSQLState() << std::endl;
	}
}