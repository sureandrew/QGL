//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/GuildMemberData_stub.h"

void GuildMemberData_Stub::SetKey(const UInt32 &char_id, const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void GuildMemberData_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void GuildMemberData_Stub::SetGuild_uid(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void GuildMemberData_Stub::SetDonate(const UInt32 &donate)
{
	bool modified = false;
	if (m.donate != donate) { m.donate = donate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void GuildMemberData_Stub::SetTotalDonate(const UInt32 &totalDonate)
{
	bool modified = false;
	if (m.totalDonate != totalDonate) { m.totalDonate = totalDonate; modified = true; }
	MarkUpdateAll(totalDonateMask, modified);
}

void GuildMemberData_Stub::SetPost(const UInt8 &post)
{
	bool modified = false;
	if (m.post != post) { m.post = post; modified = true; }
	MarkUpdateAll(postMask, modified);
}

void GuildMemberData_Stub::SetJoinDate(const UInt32 &joinDate)
{
	bool modified = false;
	if (m.joinDate != joinDate) { m.joinDate = joinDate; modified = true; }
	MarkUpdateAll(joinDateMask, modified);
}

void GuildMemberData_Stub::SetBusiness(const UInt16 &businessCount, const UInt32 &businessProfit)
{
	bool modified = false;
	if (m.businessCount != businessCount) { m.businessCount = businessCount; modified = true; }
	if (m.businessProfit != businessProfit) { m.businessProfit = businessProfit; modified = true; }
	MarkUpdateAll(businessMask, modified);
}

void GuildMemberData_Stub::SetBusinessCount(const UInt16 &businessCount)
{
	bool modified = false;
	if (m.businessCount != businessCount) { m.businessCount = businessCount; modified = true; }
	MarkUpdateAll(businessMask, modified);
}

void GuildMemberData_Stub::SetBusinessProfit(const UInt32 &businessProfit)
{
	bool modified = false;
	if (m.businessProfit != businessProfit) { m.businessProfit = businessProfit; modified = true; }
	MarkUpdateAll(businessMask, modified);
}


void GuildMemberData_Stub::Clone(GuildMemberData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(keyMask) && !(obj->m.char_id == m.char_id)) {
		m.char_id = obj->m.char_id;
		MarkUpdateAll(keyMask, true);
	}
	if (mask.IsBit(keyMask) && !(obj->m.guild_uid == m.guild_uid)) {
		m.guild_uid = obj->m.guild_uid;
		MarkUpdateAll(keyMask, true);
	}
	if (mask.IsBit(donateMask) && !(obj->m.donate == m.donate)) {
		m.donate = obj->m.donate;
		MarkUpdateAll(donateMask, true);
	}
	if (mask.IsBit(totalDonateMask) && !(obj->m.totalDonate == m.totalDonate)) {
		m.totalDonate = obj->m.totalDonate;
		MarkUpdateAll(totalDonateMask, true);
	}
	if (mask.IsBit(postMask) && !(obj->m.post == m.post)) {
		m.post = obj->m.post;
		MarkUpdateAll(postMask, true);
	}
	if (mask.IsBit(joinDateMask) && !(obj->m.joinDate == m.joinDate)) {
		m.joinDate = obj->m.joinDate;
		MarkUpdateAll(joinDateMask, true);
	}
	if (mask.IsBit(businessMask) && !(obj->m.businessCount == m.businessCount)) {
		m.businessCount = obj->m.businessCount;
		MarkUpdateAll(businessMask, true);
	}
	if (mask.IsBit(businessMask) && !(obj->m.businessProfit == m.businessProfit)) {
		m.businessProfit = obj->m.businessProfit;
		MarkUpdateAll(businessMask, true);
	}
}

void GuildMemberData_Stub::ReadAttr(CONST Buffer &buf)
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

void GuildMemberData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult GuildMemberData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildMemberData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult GuildMemberData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_GuildMemberData_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

