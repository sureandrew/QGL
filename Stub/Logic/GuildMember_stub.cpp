//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/GuildMember_stub.h"

void GuildMember_Stub::SetKey(const UInt32 &char_id, const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void GuildMember_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void GuildMember_Stub::SetGuild_uid(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void GuildMember_Stub::SetDonate(const UInt32 &donate)
{
	bool modified = false;
	if (m.donate != donate) { m.donate = donate; modified = true; }
	MarkUpdateAll(donateMask, modified);
}

void GuildMember_Stub::SetTotalDonate(const UInt32 &totalDonate)
{
	bool modified = false;
	if (m.totalDonate != totalDonate) { m.totalDonate = totalDonate; modified = true; }
	MarkUpdateAll(totalDonateMask, modified);
}

void GuildMember_Stub::SetPost(const UInt8 &post)
{
	bool modified = false;
	if (m.post != post) { m.post = post; modified = true; }
	MarkUpdateAll(postMask, modified);
}

void GuildMember_Stub::SetJoinDate(const UInt32 &joinDate)
{
	bool modified = false;
	if (m.joinDate != joinDate) { m.joinDate = joinDate; modified = true; }
	MarkUpdateAll(joinDateMask, modified);
}

void GuildMember_Stub::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(nameMask, modified);
}

void GuildMember_Stub::SetSexClass(const UInt8 &sexClass)
{
	bool modified = false;
	if (m.sexClass != sexClass) { m.sexClass = sexClass; modified = true; }
	MarkUpdateAll(sexClassMask, modified);
}

void GuildMember_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void GuildMember_Stub::SetOfflineTime(const UInt32 &offlineTime)
{
	bool modified = false;
	if (m.offlineTime != offlineTime) { m.offlineTime = offlineTime; modified = true; }
	MarkUpdateAll(offlineTimeMask, modified);
}

void GuildMember_Stub::SetBusiness(const UInt16 &businessCount, const UInt32 &businessProfit)
{
	bool modified = false;
	if (m.businessCount != businessCount) { m.businessCount = businessCount; modified = true; }
	if (m.businessProfit != businessProfit) { m.businessProfit = businessProfit; modified = true; }
	MarkUpdateAll(businessMask, modified);
}

void GuildMember_Stub::SetBusinessCount(const UInt16 &businessCount)
{
	bool modified = false;
	if (m.businessCount != businessCount) { m.businessCount = businessCount; modified = true; }
	MarkUpdateAll(businessMask, modified);
}

void GuildMember_Stub::SetBusinessProfit(const UInt32 &businessProfit)
{
	bool modified = false;
	if (m.businessProfit != businessProfit) { m.businessProfit = businessProfit; modified = true; }
	MarkUpdateAll(businessMask, modified);
}

void GuildMember_Stub::SetInactive(const Boolean &inactive)
{
	bool modified = false;
	if (m.inactive != inactive) { m.inactive = inactive; modified = true; }
	MarkUpdateAll(inactiveMask, modified);
}



void GuildMember_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void GuildMember_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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
	if (mask.IsBit(nameMask))
	{
		buf.Write<String>(m.name);
	}
	if (mask.IsBit(sexClassMask))
	{
		buf.Write<UInt8>(m.sexClass);
	}
	if (mask.IsBit(levelMask))
	{
		buf.Write<UInt16>(m.level);
	}
	if (mask.IsBit(offlineTimeMask))
	{
		buf.Write<UInt32>(m.offlineTime);
	}
	if (mask.IsBit(businessMask))
	{
		buf.Write<UInt16>(m.businessCount);
		buf.Write<UInt32>(m.businessProfit);
	}
	if (mask.IsBit(inactiveMask))
	{
		buf.Write<Boolean>(m.inactive);
	}
}

RPCResult GuildMember_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildMember_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult GuildMember_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_GuildMember_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_GuildMember_ResponseApplicant:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			bool accept;
			pBuf->Read<bool>(accept);
			_result = ResponseApplicant(pPeer, context, char_id, accept);
		}
		break;
	case RPC_GuildMember_PositionChange:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 newPost;
			pBuf->Read<UInt8>(newPost);
			_result = PositionChange(pPeer, context, char_id, newPost);
		}
		break;
	case RPC_GuildMember_ChangeAim:
		{
			String text;
			pBuf->Read<String>(text);
			_result = ChangeAim(pPeer, context, text);
		}
		break;
	case RPC_GuildMember_ChangeRule:
		{
			String text;
			pBuf->Read<String>(text);
			_result = ChangeRule(pPeer, context, text);
		}
		break;
	case RPC_GuildMember_ClearGuildBusinessLog:
		{
			_result = ClearGuildBusinessLog(pPeer, context);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult GuildMember_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildMember_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(errCode);
	pBuf->Write<String>(errMsg);
	return RPCSend<986982435>(pPeer, pBuf);
}

RPCResult GuildMember_Stub::CB_PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &src, const UInt32 &target, const UInt8 &position)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildMember_CB_PositionChange);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(src);
	pBuf->Write<UInt32>(target);
	pBuf->Write<UInt8>(position);
	return RPCSend<944125522>(pPeer, pBuf);
}

