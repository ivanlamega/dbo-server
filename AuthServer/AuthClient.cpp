#include "AuthNetwork.h"

AuthClient::AuthClient(bool IsAliveCheck, bool IsOpcodeCheck)
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

	// Desable Encoding
	//SetPacketEncoder(&_packetEncoder);
	pServer = (AuthServer*)_GetApp();
}

AuthClient::~AuthClient()
{
}

int	AuthClient::OnAccept()
{
	#ifdef USE_DBO_TW
	//Some Code here?
	#endif
	return 0;
}

void AuthClient::OnClose()
{
	if (goCharServer) pServer->ServerDB->ExecuteQuery("UPDATE `account` SET `State` = '2' WHERE `ID` = '%d';", AccountID);
	else pServer->ServerDB->ExecuteQuery("UPDATE `account` SET `State` = '0' WHERE `ID` = '%d';", AccountID);
}

int AuthClient::OnDispatch(Packet* pPacket)
{
	PacketControl(pPacket);
	//	return OnDispatch(pPacket);
	return 0;
}

void AuthClient::Send(void* pData, int nSize)
{
	Packet* packet = new Packet((unsigned char*)pData, nSize);
	int rc = pServer->Send(this->GetHandle(), packet);
	if (0 != rc)
	{
		Logger::Log("Failed to send packet %d\n", rc);
	}
}

char* AuthClient::GenAuthKey()
{
	char Key[NTL_MAX_SIZE_AUTH_KEY];
	sprintf_s(Key, NTL_MAX_SIZE_AUTH_KEY, "%d", time(NULL));
	return (char*)&Key[0];
}

int AuthClient::GetDBAccountAcLevel()
{
	if (pServer->ServerDB->ExecuteSelect("SELECT `AcLevel` FROM account WHERE ID='%d'", AccountID))
	{
		while (pServer->ServerDB->Fetch())
		{
			return pServer->ServerDB->getInt("AcLevel");
		}
	}
	return 0;
}

BYTE AuthClient::GetDBLastServerID()
{
	if (pServer->ServerDB->ExecuteSelect("SELECT `LastServerID` FROM account WHERE ID='%d'", AccountID))
	{
		while (pServer->ServerDB->Fetch())
		{
			return pServer->ServerDB->getInt("LastServerID");
		}
	}
	return 0xFF;
}

int AuthClient::GetDBAccountID()
{
	if (pServer->ServerDB->ExecuteSelect("SELECT `ID` FROM account WHERE userName='%S' AND passWord='%S'", userName, passWord))
	{
		while (pServer->ServerDB->Fetch())
		{
			return pServer->ServerDB->getInt("ID");
		}
	}
	return 0;
}

eRESULTCODE AuthClient::LoginVerifyAccount()
{
	if (pServer->ServerDB->ExecuteSelect("SELECT `ID` FROM account WHERE userName='%S'", userName))
	{
		pServer->ServerDB->Fetch();
		if (pServer->ServerDB->rowsCount() == 0)
		{
			return AUTH_USER_NOT_FOUND;
		}
		else
		{
			if (pServer->ServerDB->ExecuteSelect("SELECT `ID` FROM account WHERE userName='%S' AND passWord='%S'", userName, passWord))
			{
				pServer->ServerDB->Fetch();
				if (pServer->ServerDB->rowsCount() == 0)
				{
					return AUTH_WRONG_PASSWORD;
				}
				else
				{
					if (pServer->ServerDB->ExecuteQuery("UPDATE `account` SET `State` = '1' WHERE `ID` = '%d';", AccountID))
					{
						Logger::Log("Client[%d] Logged In with Account '%S' (%d)\n", this, userName, AccountID);
						return AUTH_SUCCESS;
					}
					else
						return AUTH_DB_FAIL;
				}
			}
			else
			{
				return AUTH_DB_FAIL;
			}
		}
	}
	return AUTH_DB_FAIL;
}