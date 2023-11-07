//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/GuildMemberData_proxy.h"

void GuildMemberData_Proxy::SetKey(const UInt32 &char_id, const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void GuildMemberData_Proxy::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void GuildMemberData_Proxy::SetGuild_uid(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void GuildMemberData_Proxy::SetDonate(const UInt32 &donate)
{
	bool modified = false;
	if (m.donate != donate) { m.donate = donate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void GuildMemberData_Proxy::SetTotalDonate(const UInt32 &totalDonate)
{
	bool modified = false;
	if (m.totalDonate != totalDonate) { m.totalDonate = totalDonate; modified = true; }
	MarkUpdateAll(totalDonateMask, modified);
}

void GuildMemberData_Proxy::SetPost(const UInt8 &post)
{
	bool modified = false;
	if (m.post != post) { m.post = post; modified = true; }
	MarkUpdateAll(postMask, modified);
}

void GuildMemberData_Proxy::SetJoinDate(const UInt32 &joinDate)
{
	bool modified = false;
	if (m.joinDate != joinDate) { m.joinDate = joinDate; modified = true; }
	MarkUpdateAll(joinDateMask, modified);
}

void GuildMemberData_Proxy::SetBusiness(const UInt16 &businessCount, const UInt32 &businessProfit)
{
	bool modified = false;
	if (m.businessCount != businessCount) { m.businessCount = businessCount; modified = true; }
	if (m.businessProfit != businessProfit) { m.businessProfit = businessProfit; modified = true; }
	MarkUpdateAll(businessMask, modified);
}

void GuildMemberData_Proxy::SetBusinessCount(const UInt16 &businessCount)
{
	bool modified = false;
	if (m.businessCount != businessCount) { m.businessCount = businessCount; modified = true; }
	MarkUpdateAll(businessMask, modified);
}

void GuildMemberData_Proxy::SetBusinessProfit(const UInt32 &businessProfit)
{
	bool modified = false;
	if (m.businessProfit != businessProfit) { m.businessProfit = businessProfit; modified = true; }
	MarkUpdateAll(businessMask, modified);
}


void GuildMemberData_Proxy::ReadAttr(CONST Buffer &buf)
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
	if (m_updateMask.IsBit(businessMask))
	{
		buf.Read<UInt16>(m.businessCount);
		buf.Read<UInt32>(m.businessProfit);
	}
}

void GuildMemberData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(keyMask))
	{
		buf.Write<UInt32>(m.char_id);
		buf.Write<UInt32>(m.guild_uid);
	}
	if (mask.IsBit(donateMask))
	{
		buf.Write<UInt32>(m.donate);
	}
	if (mask.IsBit(totalDonateMask))
	{
		buf.Write<UInt32>(m.totalDonate);
	}
	if (mask.IsBit(postMask))
	{
		buf.Write<UInt8>(m.post);
	}
	if (mask.IsBit(joinDateMask))
	{
		buf.Write<UInt32>(m.joinDate);
	}
	if (mask.IsBit(businessMask))
	{
		buf.Write<UInt16>(m.businessCount);
		buf.Write<UInt32>(m.businessProfit);
	}
}

void GuildMemberData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(keyMask)) OnUpdateKey(m.char_id, m.guild_uid);
	if (IsUpdated(donateMask)) OnUpdateDonate(m.donate);
	if (IsUpdated(totalDonateMask)) OnUpdateTotalDonate(m.totalDonate);
	if (IsUpdated(postMask)) OnUpdatePost(m.post);
	if (IsUpdated(joinDateMask)) OnUpdateJoinDate(m.joinDate);
	if (IsUpdated(businessMask)) OnUpdateBusiness(m.businessCount, m.businessProfit);
}

#pragma optimize("", off)
RPCResult GuildMemberData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_GuildMemberData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult GuildMemberData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_GuildMemberData_UP_STATE:
			_result = RPCDecode<759827620>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
