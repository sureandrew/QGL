//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/RankGuild_stub.h"

void RankGuild_Stub::SetId(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void RankGuild_Stub::SetGuild_uid(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void RankGuild_Stub::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(nameMask, modified);
}

void RankGuild_Stub::SetMaster(const String &master)
{
	bool modified = false;
	if (m.master != master) { m.master = master; modified = true; }
	MarkUpdateAll(masterMask, modified);
}

void RankGuild_Stub::SetGuildPvPScore(const UInt32 &guildPvPScore)
{
	bool modified = false;
	if (m.guildPvPScore != guildPvPScore) { m.guildPvPScore = guildPvPScore; modified = true; }
	MarkUpdateAll(guildPvPScoreMask, modified);
}



void RankGuild_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void RankGuild_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.guild_uid);
	}
	if (mask.IsBit(nameMask))
	{
		buf.Write<String>(m.name);
	}
	if (mask.IsBit(masterMask))
	{
		buf.Write<String>(m.master);
	}
	if (mask.IsBit(guildPvPScoreMask))
	{
		buf.Write<UInt32>(m.guildPvPScore);
	}
}

RPCResult RankGuild_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_RankGuild_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult RankGuild_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_RankGuild_REQ_STATE:
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

