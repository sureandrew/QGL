//-- Common
#include "Common.h"
#include "Common/BattleCode.h"
//-- Self
#include "Proxy/Logic/BatActor.h"
//-- Library
#include "SimClient/Global.h"
#include "SimClient/ItemBag.h"
#include "SimClient/Item.h"
#include "Common/CallLuaFunc.h"

REGISTER_CLASSTYPE(CBatActor, BatActor_CLASSID);

PROTO_LuaPush(CBattle)
PROTO_LuaPush(CBatCharacter)

CBatActor::CBatActor()
{
	m_commandOk = BOOLEAN_FALSE;
}

RPCResult CBatActor::SendCommand(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd, const UInt8 &target, const UInt32 &sub_type, const UInt32 &additional)
{
	return Parent::SendCommand(pPeer, context, cmd, target, sub_type, additional);
}

RPCResult CBatActor::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Error: errcode %d, %s"), errCode, errMsg.c_str());

	// try again for failed command
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_bRobot && !m_commandOk) {
		// just attack if command failed and won't input again
		m_commandOk = BOOLEAN_TRUE;
		SendCommand(pPeer, context, BATCMD_ATTACK, 0, 0, 0);
	}

	return RPC_RESULT_OK;
}

RPCResult CBatActor::CB_CommandSuccess(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_CommandSuccess: index %d, command %d"), GetIndex(), cmd);
	m_commandOk = BOOLEAN_TRUE;

	return RPC_RESULT_OK;
}

RPCResult CBatActor::CB_Inputed(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_Inputed: index %d"), GetIndex());
	return RPC_RESULT_OK;
}

RPCResult CBatActor::CB_AutoCount(LPCPEER pPeer, RPCContext &context, const UInt16 &count)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_AutoCount: turn count %d"), count);
	return RPC_RESULT_OK;
}

RPCResult CBatActor::CB_BattleMsg(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType, const String &Msg, const UInt16 &subType)
{
	return RPC_RESULT_OK;
}