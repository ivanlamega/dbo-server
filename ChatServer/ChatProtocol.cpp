#include "ChatNetwork.h"
#include "ChatProtocol.h"

bool ChatClient::PacketControl(Packet* pPacket)
{
	LPPACKETDATA data = (LPPACKETDATA)pPacket->GetPacketData();

	switch (data->wOpCode)
	{
	case UT_ENTER_CHAT: {
		SendChatEnterRes((sUT_ENTER_CHAT*)data);
		SendFriendList();
		//SendGuildInfo();		
	}break;
	case UT_CHAT_MESSAGE_SAY: SendChatSay((sUT_CHAT_MESSAGE_SAY*)data); break;
	case UT_CHAT_MESSAGE_SHOUT: SendChatShout((sUT_CHAT_MESSAGE_SHOUT*)data); break;
	case UT_CHAT_MESSAGE_WHISPER: SendChatWhisper((sUT_CHAT_MESSAGE_WHISPER*)data); break;
	case UT_CHAT_MESSAGE_PRIVATESHOP_BUSINESS: SendChatPrivateShop((sUT_CHAT_MESSAGE_PRIVATESHOP_BUSINESS*)data); break;
	// FRIEND PACKETS
	case UT_FRIEND_ADD_REQ: SendFriendAdd((sUT_FRIEND_ADD_REQ*)data); break;
	case UT_FRIEND_DEL_REQ: SendFriendDel((sUT_FRIEND_DEL_REQ*)data); break;
	case UT_FRIEND_MOVE_REQ: SendFriendMove((sUT_FRIEND_MOVE_REQ*)data); break;
	case UT_FRIEND_BLACK_ADD_REQ: SendBlackListAdd((sUT_FRIEND_BLACK_ADD_REQ*)data); break;
	case UT_FRIEND_BLACK_DEL_REQ: SendBlackListDel((sUT_FRIEND_BLACK_DEL_REQ*)data); break;
	// SYS PACKETS
	case SYS_ALIVE: { ResetAliveTime(); } break;
	case SYS_PING: break;
	default:
		Logger::Log("Received Opcode: %s\n", NtlGetPacketName_UT(data->wOpCode));
		return false;
		break;
	}
	return true;
}

void ChatClient::SendChatEnterRes(sUT_ENTER_CHAT* pData)
{
	memcpy(AuthKey, pData->abyAuthKey, NTL_MAX_SIZE_AUTH_KEY);
	AccountID = pData->accountId;
	GetCharInfo();
	sTU_ENTER_CHAT_RES pRes;
	pRes.wOpCode = TU_ENTER_CHAT_RES;
	pRes.wResultCode = CHAT_SUCCESS;
	Send(&pRes, sizeof(pRes));

	pServer->GetChatManager()->AddClient(this);
}

void ChatClient::SendChatSay(sUT_CHAT_MESSAGE_SAY* pData)
{
	GetCharInfo();
	sTU_CHAT_MESSAGE_SAY pSay;
	memset(&pSay, 0, sizeof(pSay));
	pSay.wOpCode = TU_CHAT_MESSAGE_SAY;
	pSay.hSubject = CharSerialID;
	memcpy(pSay.awchSenderCharName, charName, NTL_MAX_SIZE_CHAR_NAME_UNICODE);
	memcpy(pSay.awchMessage, pData->awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE);
	pSay.wMessageLengthInUnicode = pData->wMessageLengthInUnicode;
	Logger::Log("%s says: %s\n", GameString(pSay.awchSenderCharName).c_str(), GameString(pSay.awchMessage).c_str());

	pServer->GetChatManager()->SendAll2(&pSay, sizeof(pSay),this);
	//Send(&pSay, sizeof(pSay));
}
//By Luiz45 Shouting the msg to everybody who is connected at channel
void ChatClient::SendChatShout(sUT_CHAT_MESSAGE_SHOUT* pData)
{
	GetCharInfo();
	sTU_CHAT_MESSAGE_SHOUT pShout;
	memset(&pShout, 0, sizeof(pShout));
	pShout.hSubject = CharSerialID;
	pShout.wOpCode = TU_CHAT_MESSAGE_SHOUT;
	pShout.wMessageLengthInUnicode = pData->wMessageLengthInUnicode;
	memcpy(pShout.awchSenderCharName, charName, NTL_MAX_SIZE_CHAR_NAME_UNICODE);
	memcpy(pShout.awchMessage, pData->awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE);
	Logger::Log("%s Say to everybody in the chat: %s\n", GameString(pShout.awchSenderCharName).c_str(), GameString(pShout.awchMessage).c_str());
	pServer->GetChatManager()->SendAll(&pShout, sizeof(pShout));
	//Send(&pShout, sizeof(pShout);
}
//By Luiz45 - WhisperChat(Aka Private Chat) - No log because is private.
void ChatClient::SendChatWhisper(sUT_CHAT_MESSAGE_WHISPER* pData)
{
	GetCharInfo();
	sTU_CHAT_MESSAGE_WHISPER pWhisper;
	memset(&pWhisper, 0, sizeof(pWhisper));
	pWhisper.wOpCode = TU_CHAT_MESSAGE_WHISPER;
	pWhisper.wMessageLengthInUnicode = pData->wMessageLengthInUnicode;
	memcpy(pWhisper.awchSenderCharName, charName, NTL_MAX_SIZE_CHAR_NAME_UNICODE);
	memcpy(pWhisper.awchMessage, pData->awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE);
	ChatClient* pChatToWhisper = pServer->GetChatManager()->GetChatClient(pData->awchReceiverCharName);
	if (NULL != pChatToWhisper){
		pServer->GetChatManager()->SendTo(&pWhisper, sizeof(pWhisper), pChatToWhisper);
		Send(&pWhisper, sizeof(pWhisper));
	}
}
//By Luiz45 - Private Shop
void ChatClient::SendChatPrivateShop(sUT_CHAT_MESSAGE_PRIVATESHOP_BUSINESS* pData)
{
	GetCharInfo();
	sTU_CHAT_MESSAGE_PRIVATESHOP_BUSINESS pBusiness;
	memset(&pBusiness, 0, sizeof(pBusiness));
	pBusiness.wOpCode = TU_CHAT_MESSAGE_PRIVATESHOP_BUSINESS;
	pBusiness.wMessageLengthInUnicode = pData->wMessageLengthInUnicode;
	memcpy(pBusiness.awchSenderCharName, charName, NTL_MAX_SIZE_CHAR_NAME_UNICODE);
	memcpy(pBusiness.awchMessage, pData->awchMessage, NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE);
	ChatClient* pChatToWhisper = pServer->GetChatManager()->GetChatClient(pData->awchReceiverCharName);
	if (NULL != pChatToWhisper){
		pServer->GetChatManager()->SendTo(&pBusiness, sizeof(pBusiness), pChatToWhisper);
		Send(&pBusiness, sizeof(pBusiness));
	}
}
//By Luiz45 - Send Add Friend to List
void ChatClient::SendFriendAdd(sUT_FRIEND_ADD_REQ* pData)
{
	sTU_FRIEND_ADD_RES pFriendAdd;
	memset(&pFriendAdd, 0, sizeof(pFriendAdd));
	pFriendAdd.wOpCode = TU_FRIEND_ADD_RES;
	memcpy(pFriendAdd.wchName, pData->wchName, NTL_MAX_SIZE_CHAR_NAME_UNICODE);
	ChatClient* pChatToWhisper = pServer->GetChatManager()->GetChatClient(pData->wchName);
	if (NULL == pChatToWhisper){
		pFriendAdd.wResultCode = COMMUNITY_FRIEND_CHAR_NOT_FOUND;
	}
	else{
		if (SearchFriendDB(pChatToWhisper->GetCharSerialID()))
		{
			pFriendAdd.wResultCode = COMMUNITY_FRIEND_CHAR_ARLEADY_ADDED;
		}
		else
		{
			pServer->ServerDB->ExecuteQuery("INSERT INTO `friendlist`(`owner_id`,`friend_id`) VALUES ('%u','%u');",this->GetCharSerialID(),pChatToWhisper->GetCharSerialID());
			pFriendAdd.targetID = pChatToWhisper->GetCharSerialID();
			pFriendAdd.wResultCode = GAME_SUCCESS;
		}
	}
	Send(&pFriendAdd, sizeof(pFriendAdd));
}
//By Luiz45 - Send Delete Friend from Friend List
void ChatClient::SendFriendDel(sUT_FRIEND_DEL_REQ* pData)
{
	sTU_FRIEND_DEL_RES pFriendDel;
	memset(&pFriendDel, 0, sizeof(pFriendDel));
	pFriendDel.targetID = pData->targetID;
	pFriendDel.wOpCode  = TU_FRIEND_DEL_RES;
	pFriendDel.wResultCode = GAME_SUCCESS;
	///TODO: GetDbManager()->DeleteFriend(targetID);Target ID is the CharID on Database
	Send(&pFriendDel, sizeof(pFriendDel));
}
//By Luiz45 - Friend Move
void ChatClient::SendFriendMove(sUT_FRIEND_MOVE_REQ* pData)
{
	sTU_FRIEND_MOVE_RES pFriendMove;
	memset(&pFriendMove, 0, sizeof(pFriendMove));
	pFriendMove.targetID = pData->targetID;
	pFriendMove.wOpCode  = TU_FRIEND_MOVE_RES;
	pFriendMove.wResultCode = GAME_SUCCESS;
	///TODO: GetDbManager()->MoveFriend(targetID);If i remember this is only to tell if he is in black list or not
	Send(&pFriendMove, sizeof(pFriendMove));
}
//By Luiz45 - Add a "Friend" to a blackList
void ChatClient::SendBlackListAdd(sUT_FRIEND_BLACK_ADD_REQ* pData)
{
	sTU_FRIEND_BLACK_ADD_RES pBlackAdd;
	memset(&pBlackAdd, 0, sizeof(pBlackAdd));
	pBlackAdd.wOpCode = TU_FRIEND_ADD_RES;
	memcpy(pBlackAdd.wchName, pData->awchName, NTL_MAX_SIZE_CHAR_NAME_UNICODE);
	ChatClient* pChatToWhisper = pServer->GetChatManager()->GetChatClient(pData->awchName);
	if (NULL == pChatToWhisper){
		pBlackAdd.wResultCode = COMMUNITY_FRIEND_CHAR_NOT_FOUND;
	}
	else{
		if (SearchFriendDB(pChatToWhisper->GetCharSerialID()))
		{
			pBlackAdd.wResultCode = COMMUNITY_FRIEND_CHAR_ARLEADY_ADDED;
		}
		else
		{
			pServer->ServerDB->ExecuteQuery("INSERT INTO `blacklist`(`owner_id`,`target_id`) VALUES ('%u','%u');", this->GetCharSerialID(), pChatToWhisper->GetCharSerialID());
			pBlackAdd.targetID = pChatToWhisper->GetCharSerialID();
			pBlackAdd.wResultCode = GAME_SUCCESS;
		}
	}
	Send(&pBlackAdd, sizeof(pBlackAdd));
}
//By Luiz45 - Remove a "Friend" from blacklist
void ChatClient::SendBlackListDel(sUT_FRIEND_BLACK_DEL_REQ* pData)
{
	sTU_FRIEND_BLACK_DEL_RES pBlackDel;
	memset(&pBlackDel, 0, sizeof(pBlackDel));
	pBlackDel.targetID = pData->targetID;
	pBlackDel.wOpCode  = TU_FRIEND_BLACK_DEL_RES;
	pBlackDel.wResultCode = GAME_SUCCESS;
	///TODO: GetDbManager()->DeleteFromBlackList(targetID);
	Send(&pBlackDel, sizeof(pBlackDel));
}
//By Luiz45 - Basic load of Friend List
void ChatClient::SendFriendList()
{
	sTU_FRIEND_LIST_INFO pFriendList;
	memset(&pFriendList, 0, sizeof(pFriendList));
	pFriendList.byCount = LoadFriendList();
	memcpy(pFriendList.asInfo, asInfo, sizeof(asInfo));
	pFriendList.wOpCode = TU_FRIEND_LIST_INFO;
	int psize = 3;
	psize += sizeof(sFRIEND_FULL_INFO) * pFriendList.byCount;
	Send(&pFriendList, psize);
}
//By Luiz45 - Basic load of Guild Info
void ChatClient::SendGuildInfo()
{
	sTU_GUILD_INFO pGuildInfo;
	//Example in how to load a guild
	pGuildInfo.guildInfo.guildId = 1;//ID From Database
	pGuildInfo.guildInfo.guildMaster = 1;//Who is the Owner of the Guild
	pGuildInfo.guildInfo.guildSecondMaster[0] = 2;//Who is the Vice-Leader of the Guild(Max 4)
	memcpy(pGuildInfo.guildInfo.awchName, charName, NTL_MAX_SIZE_CHAR_NAME_UNICODE);
	memcpy(pGuildInfo.guildInfo.wszName, L"NameOfGuild", NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE);
	memcpy(pGuildInfo.guildInfo.awchNotice, L"MOTD of Guild", NTL_MAX_LENGTH_OF_GUILD_NOTICE);
	///TODO: See the Color and Dogi things
	pGuildInfo.guildInfo.qwGuildFunctionFlag = DBO_GUILD_FUNCTION_MAX_MEMBER_30;//One value from guild function
	pGuildInfo.guildInfo.dwGuildReputation = 999;
	pGuildInfo.guildInfo.dwMaxGuildPointEver = 1500;	
	pGuildInfo.wOpCode = TU_GUILD_INFO;
	Send(&pGuildInfo, sizeof(pGuildInfo));
	//YOU MUST SEND THE MEMBERS OF YOUR GUILD - YOU CAN PUT THAT ON A LOOP
	sTU_GUILD_MEMBER_INFO pMemberInfo;
	pMemberInfo.wOpCode = TU_GUILD_MEMBER_INFO;
	pMemberInfo.guildMemberInfo.bIsOnline = true;//Or false
	pMemberInfo.guildMemberInfo.byClass = PC_CLASS_HUMAN_FIGHTER;//Class of Character
	pMemberInfo.guildMemberInfo.byLevel = 1;//Level
	pMemberInfo.guildMemberInfo.byRace = RACE_HUMAN;//Race of Char
	pMemberInfo.guildMemberInfo.charId = 1;//Char ID from DB;
	pMemberInfo.guildMemberInfo.dwReputation = 50;//Your contribution for guild
	memcpy(pMemberInfo.guildMemberInfo.wszMemberName, charName, NTL_MAX_SIZE_CHAR_NAME_UNICODE);//Name of that Member
	memset(&pMemberInfo, 0, sizeof(pMemberInfo));
	Send(&pMemberInfo, sizeof(pMemberInfo));
}