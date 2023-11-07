//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/SkillData_proxy.h"

void SkillData_Proxy::SetInfo(const UInt16 &skill_id)
{
	bool modified = false;
	if (m.skill_id != skill_id) { m.skill_id = skill_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void SkillData_Proxy::SetSkill_id(const UInt16 &skill_id)
{
	bool modified = false;
	if (m.skill_id != skill_id) { m.skill_id = skill_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void SkillData_Proxy::SetOwner_info(const UInt32 &owner_id, const BYTE &owner_type)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	if (m.owner_type != owner_type) { m.owner_type = owner_type; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void SkillData_Proxy::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void SkillData_Proxy::SetOwner_type(const BYTE &owner_type)
{
	bool modified = false;
	if (m.owner_type != owner_type) { m.owner_type = owner_type; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void SkillData_Proxy::SetLevel(const UInt16 &skill_level)
{
	bool modified = false;
	if (m.skill_level != skill_level) { m.skill_level = skill_level; modified = true; }
	MarkUpdateAll(LevelMask, modified);
}

void SkillData_Proxy::SetSkill_level(const UInt16 &skill_level)
{
	bool modified = false;
	if (m.skill_level != skill_level) { m.skill_level = skill_level; modified = true; }
	MarkUpdateAll(LevelMask, modified);
}

void SkillData_Proxy::SetCoolDown(const UInt32 &cool_down)
{
	bool modified = false;
	if (m.cool_down != cool_down) { m.cool_down = cool_down; modified = true; }
	MarkUpdateAll(CoolDownMask, modified);
}

void SkillData_Proxy::SetCool_down(const UInt32 &cool_down)
{
	bool modified = false;
	if (m.cool_down != cool_down) { m.cool_down = cool_down; modified = true; }
	MarkUpdateAll(CoolDownMask, modified);
}


void SkillData_Proxy::ReadAttr(CONST Buffer &buf)
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
}

void SkillData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(infoMask))
	{
		buf.Write<UInt16>(m.skill_id);
	}
	if (mask.IsBit(owner_infoMask))
	{
		buf.Write<UInt32>(m.owner_id);
		buf.Write<BYTE>(m.owner_type);
	}
	if (mask.IsBit(LevelMask))
	{
		buf.Write<UInt16>(m.skill_level);
	}
	if (mask.IsBit(CoolDownMask))
	{
		buf.Write<UInt32>(m.cool_down);
	}
}

void SkillData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.skill_id);
	if (IsUpdated(owner_infoMask)) OnUpdateOwner_info(m.owner_id, m.owner_type);
	if (IsUpdated(LevelMask)) OnUpdateLevel(m.skill_level);
	if (IsUpdated(CoolDownMask)) OnUpdateCoolDown(m.cool_down);
}

#pragma optimize("", off)
RPCResult SkillData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_SkillData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult SkillData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_SkillData_UP_STATE:
			_result = RPCDecode<950277788>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
