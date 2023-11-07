//-- Common
#include "Common.h"
#include "Common/common_binary.h"
//-- Self
#include "SimClient/GuildMember.h"
//-- Library
#include "SimClient/GuildApplicant.h"
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CGuildMember, GuildMember_CLASSID);



RPCResult CGuildMember::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));	
	return RPC_RESULT_OK;
}

RPCResult CGuildMember::CB_PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &src, const UInt32 &target, const UInt8 &position)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CB_PositionChange:: src :%d target %d position: %d"), src, target, position);	
	return RPC_RESULT_OK;
}