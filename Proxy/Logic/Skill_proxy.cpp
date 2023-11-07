//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Skill_proxy.h"


void Skill_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt16>(m.skill_id);
	}
	if (m_updateMask.IsBit(owner_infoMask))
	{
		buf.Read<UInt32>(m.owner_id);
		buf.Read<BYTE>(m.owner_type);
	}
	if (m_updateMask.IsBit(LevelMask))
	{
		buf.Read<UInt16>(m.skill_level);
	}
	if (m_updateMask.IsBit(CoolDownMask))
	{
		buf.Read<UInt32>(m.cool_down);
	}
	if (m_updateMask.IsBit(PartnerSkillTypeMask))
	{
		buf.Read<BYTE>(m.partner_skill_type);
	}
}

void Skill_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Skill_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.skill_id);
	if (IsUpdated(owner_infoMask)) OnUpdateOwner_info(m.owner_id, m.owner_type);
	if (IsUpdated(LevelMask)) OnUpdateLevel(m.skill_level);
	if (IsUpdated(CoolDownMask)) OnUpdateCoolDown(m.cool_down);
	if (IsUpdated(PartnerSkillTypeMask)) OnUpdatePartnerSkillType(m.partner_skill_type);
}

#pragma optimize("", off)
RPCResult Skill_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Skill_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Skill_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Skill_UP_STATE:
			_result = RPCDecode<923693173>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
