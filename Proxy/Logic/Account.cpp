//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Account.h"
//-- Library
#include "QGL/Global.h"
#include "Proxy/Service/WorldService.h"
#include "QGL/LuaPush.h"
#include "QGL/CallLuaFunc.h"
#include "QGL/CharacterControl.h"


REGISTER_CLASSTYPE(CAccount, Account_CLASSID);

CAccount::CAccount()
{
}

CAccount::~CAccount()
{

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
	m_AccName = loginName;
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


