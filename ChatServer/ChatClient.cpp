#include "ChatNetwork.h"

#include <Def.h>

ChatClient::ChatClient(bool IsAliveCheck, bool IsOpcodeCheck)
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
	pServer = (ChatServer*)_GetApp();
}

ChatClient::~ChatClient()
{
	OnClose();
}

int	ChatClient::OnAccept()
{
	return 0;
}

void ChatClient::OnClose()
{
	/*if (pServer->GetChatManager()->FindClient(this))
		pServer->GetChatManager()->RemoveClient(this);*/
}

int ChatClient::OnDispatch(Packet* pPacket)
{
	PacketControl(pPacket);
	return 0;
}

void ChatClient::Send(void* pData, int nSize)
{
	Packet* packet = new Packet((unsigned char*)pData, nSize);
	int rc = pServer->Send(this->GetHandle(), packet);
	if (0 != rc)
	{
		Logger::Log("Failed to send packet %d\n", rc);
	}
}

void ChatClient::GetCharInfo()
{
	if (pServer->ServerDB->ExecuteSelect("SELECT * FROM `online` WHERE `AccountID` = '%u';", AccountID))
	{
		while (pServer->ServerDB->Fetch()) {
			CurrCharID = pServer->ServerDB->getInt("CharID");
			CurrServerID = pServer->ServerDB->getInt("ServerID");
			CurrChannelID = pServer->ServerDB->getInt("ChannelID");
			CharSerialID = pServer->ServerDB->getInt("Handle");
		}
	}
	if (pServer->ServerDB->ExecuteSelect("SELECT * FROM `character` WHERE `ID` = '%u';", CurrCharID))
	{
		while (pServer->ServerDB->Fetch()) {
			memcpy(this->charName, charToWChar(pServer->ServerDB->getString("Name")), NTL_MAX_SIZE_CHAR_NAME_UNICODE);
		}
	}
}
//By Luiz45 - Load Friend List
int ChatClient::LoadFriendList()
{
	CHARACTERID friendID[NTL_MAX_COUNT_FRIEND + NTL_MAX_COUNT_FRIEND];
	int count = 0;
	if (pServer->ServerDB->ExecuteSelect("SELECT * FROM `friendlist` WHERE `owner_id` = '%u'", this->GetCharSerialID()))
	{
		if (pServer->ServerDB->rowsCount() > 0)
		{
			while (pServer->ServerDB->Fetch())
			{
				friendID[count] = pServer->ServerDB->getInt("friend_id");
				asInfo[count].bIsBlack = pServer->ServerDB->getBoolean("toBlackList");
				count++;
			}
		}
	}
	for (int i = 0; i <= count; i++)
	{
		if (pServer->ServerDB->ExecuteSelect("SELECT * FROM `character` WHERE `ID`='%u'", friendID[i]))
		{
			while (pServer->ServerDB->Fetch())
			{
				asInfo[i].charID   = friendID[i];
				memcpy(asInfo[i].wchName, charToWChar(pServer->ServerDB->getString("Name")), NTL_MAX_SIZE_CHAR_NAME_UNICODE);
			}
		}
	}
	return count;
}
//By Luiz45 - Just check if friend already in list
bool ChatClient::SearchFriendDB(CHARACTERID charID)
{
	if (pServer->ServerDB->ExecuteSelect("SELECT * FROM `friendlist` WHERE `friend_id` = '%u'", charID))
	{
		if (pServer->ServerDB->rowsCount() > 0)
			return true;			
	}
	return false;
}