//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "SimClient/Account.h"
//-- Library
#include "SimClient/CharacterInfo.h"
#include "SimClient/SimClient.h"
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CAccount, Account_CLASSID);

CAccount::CAccount()
{
}

CAccount::~CAccount()
{
}

RPCResult CAccount::LoginAccount(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CAccount, LoginAccount, this, context, SYSTEMTIME, sysTime)
	
		if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pAccount != this)
		{
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("LoginAccount: acct_id %d"), GetAcct_id());
		GetOwnCharacters(gGlobal.m_pSimClient->m_pProxyPeer, context);

	PROCEDURE_WAIT_OBJ1(1, Account, CB_GetOwnCharacters, account, NetGroup*, characters)

		if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pAccount != this)
		{
			DeleteNetGroup(characters, BOOLEAN_TRUE);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Please select character: count %d"), 
			characters->GetCount());
		//if (gGlobal.m_pSimClient->m_characters && 
		//	gGlobal.m_pSimClient->m_characters != characters)
		//	DeleteNetGroup(gGlobal.m_pSimClient->m_characters);
		//gGlobal.m_pSimClient->m_characters = characters;

		CCharacterInfo* pSelChar = NULL;
		UInt16 charCount = characters->GetCount();
		UInt8 line_id = 0;

		for (UInt16 i = 0; i < characters->GetCount(); i++)
		{
			CCharacterInfo *pchar = characters->GetChild<CCharacterInfo>(i);
			if (pchar)
			{
				TRACE_INFODTL_6(GLOBAL_LOGGER, 
					_T("%d: char_id %d, name %s, status %d, map %d, line %d"),
					i, pchar->GetChar_id(), pchar->GetNickName().c_str(), pchar->GetStatus(), 
					pchar->GetMap_id(), pchar->GetLine_id());
				if (pchar->GetStatus() == CCharacterInfo::ST_DISCONNECT)
				{
					pSelChar = pchar;
					line_id = pchar->GetLine_id();
				}
			}
		}

		if (gGlobal.m_pSimClient->m_bRobot)
		{
			if (charCount == 0)
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("no characters to select, acct_id %d"),
					GetAcct_id());
				DeleteNetGroup(characters, BOOLEAN_TRUE);
				gGlobal.m_pSimClient->DisconnectServer();
				PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
			}
			else if (pSelChar == NULL)
			{
				// random select character and line
				pSelChar = characters->GetChild<CCharacterInfo>(GetRand(charCount));
			}
		}
		else if (IsOption(OPT_AUTO_SELECT))
		{
			if (charCount == 1 && pSelChar == NULL)
			{
				// manual auto select for only 1 character and select previous line
				pSelChar = characters->GetChild<CCharacterInfo>(0);
				if (pSelChar)
					line_id = pSelChar->GetLine_id();
			}
		}
		else
		{
			pSelChar = NULL;
		}

		if (pSelChar)
		{
			PROCEDURE_DO_METHOD2(CAccount, SelectCharacter, this, context, 
					UInt32, pSelChar->GetChar_id(), UInt8, line_id)
		}
		else
		{
			DeleteNetGroup(characters, BOOLEAN_TRUE);
			PROCEDURE_RETURN
		}
		DeleteNetGroup(characters, BOOLEAN_TRUE);
	
	PROCEDURE_WAIT_RETURN(2)

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Login Error: Account:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

		PROCEDURE_EXCEPTION_DEFAULT

			if (gGlobal.m_pSimClient)
				gGlobal.m_pSimClient->DisconnectServer();

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CAccount::Logout(RPCContext &context)
{
	PROCEDURE_START_METHOD(CAccount, Logout, this, context)

		if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pAccount != this)
		{
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Try Logout: acct_id %d"), GetAcct_id());
		Parent::Logout(gGlobal.m_pSimClient->m_pProxyPeer, context);

	PROCEDURE_WAIT_OBJ(2, Account, CB_LogoutSuccess, account)

		if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pAccount != this)
		{
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Logout: successful acct_id %d"), GetAcct_id());
		//gGlobal.m_pSimClient->DeleteObjects();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Logout: Account::CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

		PROCEDURE_EXCEPTION_DEFAULT

			if (gGlobal.m_pSimClient)
				gGlobal.m_pSimClient->DisconnectServer();

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CAccount::SelectCharacter(RPCContext &context)
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pProxyPeer == NULL)
	{
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD2(CAccount, SelectCharacter, this, context, UInt32, char_id,
							UInt8, line_id)

		if (gGlobal.m_pSimClient->m_login != LS_LOGINED) {
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("SelectCharacter Error: account not logined"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CheckCharacter(gGlobal.m_pSimClient->m_pProxyPeer, context, char_id);
		//PROCEDURE_SET_TIMEOUT(3000)

	PROCEDURE_WAIT_OBJ3(1, Account, CB_CheckCharacter, account, UInt8, charStatus,
						Int32Vector, lineCounts, UInt16Vector, lineState)

		UInt8 minLine = 255;
		Int32 minCount = INT32_MAX_VALUE;
		UInt8Vector	maplines;				// map line number

		if (charStatus == CCharacterInfo::ST_NOT_FOUND) {
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("SelectCharacter Error: character %d not found"),
				char_id);
			gGlobal.m_pSimClient->DisconnectServer();
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		} else if (charStatus == CCharacterInfo::ST_NOT_AVAIL) {
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("SelectCharacter Error: character %d not availiable"), char_id);
			gGlobal.m_pSimClient->DisconnectServer();
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		} else if (charStatus == CCharacterInfo::ST_ENTER) {
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SelectCharacter: force out other client playing character %d "), char_id);
		} else if (charStatus == CCharacterInfo::ST_DISCONNECT) {
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SelectCharacter: resume playing character %d "), char_id);
		} else {
			String strLines;

			for (int i = 1; i < (int) lineCounts.size(); ++i)
			{
				if (lineCounts[i] != -1) {
					strLines.AppendFormat(_T("L%d=%d, "), i, lineCounts[i]);
					maplines.push_back(i);
					if (lineCounts[i] < minCount) {
						minLine = i;
						minCount = lineCounts[i];
					}
				}
			}
			if (maplines.empty()) {
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("no line to select, char_id %d"), char_id);
				if (gGlobal.m_pSimClient)
					gGlobal.m_pSimClient->DisconnectServer();
				PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
			} else {
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SelectCharacter: lines can selected: %s"), 
					strLines.c_str());
			}
		}
		UInt8 selectLine = line_id;
		if (line_id == 0) {
			if (maplines.size() > 1) {
				//if (minLine != 255)
				//	selectLine = minLine;					// select min line
				//else
				//	selectLine = maplines[GetRand((int) maplines.size())];
				if (gGlobal.m_nextLine < (UInt8) maplines.size()) {
					selectLine = maplines[gGlobal.m_nextLine];
					gGlobal.m_nextLine ++;
				} else {
					selectLine = maplines[0];
					gGlobal.m_nextLine = 1;
				}
			} else if (!maplines.empty()) {
				selectLine = maplines[0];
			}
		}
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SelectCharacter: selected char_id %d, line %d"),
			char_id, selectLine);
		Parent::SelectCharacter(gGlobal.m_pSimClient->m_pProxyPeer, context, char_id, selectLine);
		PROCEDURE_SET_TIMEOUT(60000, BOOLEAN_TRUE);		// 1 min timeout for select character

	PROCEDURE_WAIT_OBJ(2, Account, CB_SelectCharacter, account)

		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_SelectCharacter: successful to select character"));

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("SelectCharacter Error: Account:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//gGlobal.m_pSimClient->DisconnectServer();

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CAccount::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("LoginError: errcode %d: %s"), errCode, errMsg.c_str());
	return RPC_RESULT_OK;
}

RPCResult CAccount::CB_LogoutSuccess(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_LogoutSuccesss:"));
	return RPC_RESULT_OK;
}

RPCResult CAccount::CB_GetOwnCharacters(LPCPEER pPeer, RPCContext &context, NetGroup *characters)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_GetOwnCharacters: count %d"), characters->GetCount());
	return RPC_RESULT_OK;
}

RPCResult CAccount::CB_CheckCharacter(LPCPEER pPeer, RPCContext &context, 
									  const UInt8 &status, const Int32Vector &lineCounts, const UInt16Vector &lineState)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_CheckCharacter:"));
	return RPC_RESULT_OK;
}

RPCResult CAccount::CB_SelectCharacter(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_SelectCharacter:"));
	return RPC_RESULT_OK;
}

void CAccount::OnUpdateInfo(const UInt32 &acct_id, const String &loginName)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CAccount UpdateInfo: acct_id %d, loginName %s"), acct_id, loginName.c_str());
}

void CAccount::OnUpdateStatus(const UInt8 &status)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CAccount OnUpdateStatus: status %d"), status);
}

RPCResult CAccount::CB_CreateSuccess(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_CreateSuccesss:"));
	return RPC_RESULT_OK;
}

RPCResult CAccount::CB_RemoveCharSuccess(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_RemoveCharSuccesss:"));
	return RPC_RESULT_OK;
}

RPCResult CAccount::CB_CheckCharacterName(LPCPEER pPeer, RPCContext &context, const UInt8 &index)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_RemoveCharSuccesss:"));
	return RPC_RESULT_OK;
}



RPCResult CAccount::CB_AddictionWarning(LPCPEER pPeer, RPCContext &context, const String &warnMsg)
{
	return RPC_RESULT_OK;
}

RPCResult CAccount::CB_AddictCurStatus(LPCPEER pPeer, RPCContext &context, const String &statusMsg, const float &plyInterest)
{
	return RPC_RESULT_OK;
}

RPCResult CAccount::CB_AddictionShutDown(LPCPEER pPeer, RPCContext &context, const String &warnMsg, const UInt32 &accOffTime)
{
	return RPC_RESULT_OK;
}

RPCResult CAccount::CB_AddictionInterestChg(LPCPEER pPeer, RPCContext &context, const float &interestChg)
{
	return RPC_RESULT_OK;
}


RPCResult CAccount::CB_RecoverCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_RecoverCharSuccess:"));

	return RPC_RESULT_OK;
}

RPCResult CAccount::CB_FreezeCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_FreezeCharSuccess:"));

	return RPC_RESULT_OK;
}
