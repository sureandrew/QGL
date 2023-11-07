//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/RankGuild_proxy.h"


void RankGuild_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.guild_uid);
	}
	if (m_updateMask.IsBit(nameMask))
	{
		buf.Read<String>(m.name);
	}
	if (m_updateMask.IsBit(masterMask))
	{
		buf.Read<String>(m.master);
	}
	if (m_updateMask.IsBit(guildPvPScoreMask))
	{
		buf.Read<UInt32>(m.guildPvPScore);
	}
}

void RankGuild_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void RankGuild_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.guild_uid);
	if (IsUpdated(nameMask)) OnUpdateName(m.name);
	if (IsUpdated(masterMask)) OnUpdateMaster(m.master);
	if (IsUpdated(guildPvPScoreMask)) OnUpdateGuildPvPScore(m.guildPvPScore);
}

#pragma optimize("", off)
RPCResult RankGuild_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_RankGuild_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult RankGuild_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_RankGuild_UP_STATE:
			_result = RPCDecode<500916192>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
