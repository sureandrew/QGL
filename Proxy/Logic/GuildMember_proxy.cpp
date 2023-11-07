//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/GuildMember_proxy.h"


void GuildMember_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(keyMask))
	{
		buf.Read<UInt32>(m.char_id);
		buf.Read<UInt32>(m.guild_uid);
	}
	if (m_updateMask.IsBit(donateMask))
	{
		buf.Read<UInt32>(m.donate);
	}
	if (m_updateMask.IsBit(totalDonateMask))
	{
		buf.Read<UInt32>(m.totalDonate);
	}
	if (m_updateMask.IsBit(postMask))
	{
		buf.Read<UInt8>(m.post);
	}
	if (m_updateMask.IsBit(joinDateMask))
	{
		buf.Read<UInt32>(m.joinDate);
	}
	if (m_updateMask.IsBit(nameMask))
	{
		buf.Read<String>(m.name);
	}
	if (m_updateMask.IsBit(sexClassMask))
	{
		buf.Read<UInt8>(m.sexClass);
	}
	if (m_updateMask.IsBit(levelMask))
	{
		buf.Read<UInt16>(m.level);
	}
	if (m_updateMask.IsBit(offlineTimeMask))
	{
		buf.Read<UInt32>(m.offlineTime);
	}
	if (m_updateMask.IsBit(businessMask))
	{
		buf.Read<UInt16>(m.businessCount);
		buf.Read<UInt32>(m.businessProfit);
	}
	if (m_updateMask.IsBit(inactiveMask))
	{
		buf.Read<Boolean>(m.inactive);
	}
}

void GuildMember_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void GuildMember_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(keyMask)) OnUpdateKey(m.char_id, m.guild_uid);
	if (IsUpdated(donateMask)) OnUpdateDonate(m.donate);
	if (IsUpdated(totalDonateMask)) OnUpdateTotalDonate(m.totalDonate);
	if (IsUpdated(postMask)) OnUpdatePost(m.post);
	if (IsUpdated(joinDateMask)) OnUpdateJoinDate(m.joinDate);
	if (IsUpdated(nameMask)) OnUpdateName(m.name);
	if (IsUpdated(sexClassMask)) OnUpdateSexClass(m.sexClass);
	if (IsUpdated(levelMask)) OnUpdateLevel(m.level);
	if (IsUpdated(offlineTimeMask)) OnUpdateOfflineTime(m.offlineTime);
	if (IsUpdated(businessMask)) OnUpdateBusiness(m.businessCount, m.businessProfit);
	if (IsUpdated(inactiveMask)) OnUpdateInactive(m.inactive);
}

#pragma optimize("", off)
RPCResult GuildMember_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_GuildMember_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_GuildMember_CB_Error:
		{
			UInt32 errCode;
			pBuf->Read<UInt32>(errCode);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, errCode, errMsg);
		}
		break;
	case RPC_GuildMember_CB_PositionChange:
		{
			UInt32 src;
			pBuf->Read<UInt32>(src);
			UInt32 target;
			pBuf->Read<UInt32>(target);
			UInt8 position;
			pBuf->Read<UInt8>(position);
			_result = CB_PositionChange(pPeer, context, src, target, position);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult GuildMember_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_GuildMember_UP_STATE:
			_result = RPCDecode<1010581609>(pPeer, pBuf);
		break;
	case RPC_GuildMember_CB_Error:
			_result = RPCDecode<986982435>(pPeer, pBuf);
		break;
	case RPC_GuildMember_CB_PositionChange:
			_result = RPCDecode<944125522>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
