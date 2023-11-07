//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/SkillData_stub.h"

void SkillData_Stub::SetInfo(const UInt16 &skill_id)
{
	bool modified = false;
	if (m.skill_id != skill_id) { m.skill_id = skill_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void SkillData_Stub::SetSkill_id(const UInt16 &skill_id)
{
	bool modified = false;
	if (m.skill_id != skill_id) { m.skill_id = skill_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void SkillData_Stub::SetOwner_info(const UInt32 &owner_id, const BYTE &owner_type)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	if (m.owner_type != owner_type) { m.owner_type = owner_type; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void SkillData_Stub::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void SkillData_Stub::SetOwner_type(const BYTE &owner_type)
{
	bool modified = false;
	if (m.owner_type != owner_type) { m.owner_type = owner_type; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void SkillData_Stub::SetLevel(const UInt16 &skill_level)
{
	bool modified = false;
	if (m.skill_level != skill_level) { m.skill_level = skill_level; modified = true; }
	MarkUpdateAll(LevelMask, modified);
}

void SkillData_Stub::SetSkill_level(const UInt16 &skill_level)
{
	bool modified = false;
	if (m.skill_level != skill_level) { m.skill_level = skill_level; modified = true; }
	MarkUpdateAll(LevelMask, modified);
}

void SkillData_Stub::SetCoolDown(const UInt32 &cool_down)
{
	bool modified = false;
	if (m.cool_down != cool_down) { m.cool_down = cool_down; modified = true; }
	MarkUpdateAll(CoolDownMask, modified);
}

void SkillData_Stub::SetCool_down(const UInt32 &cool_down)
{
	bool modified = false;
	if (m.cool_down != cool_down) { m.cool_down = cool_down; modified = true; }
	MarkUpdateAll(CoolDownMask, modified);
}


void SkillData_Stub::Clone(SkillData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(infoMask) && !(obj->m.skill_id == m.skill_id)) {
		m.skill_id = obj->m.skill_id;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(owner_infoMask) && !(obj->m.owner_id == m.owner_id)) {
		m.owner_id = obj->m.owner_id;
		MarkUpdateAll(owner_infoMask, true);
	}
	if (mask.IsBit(owner_infoMask) && !(obj->m.owner_type == m.owner_type)) {
		m.owner_type = obj->m.owner_type;
		MarkUpdateAll(owner_infoMask, true);
	}
	if (mask.IsBit(LevelMask) && !(obj->m.skill_level == m.skill_level)) {
		m.skill_level = obj->m.skill_level;
		MarkUpdateAll(LevelMask, true);
	}
	if (mask.IsBit(CoolDownMask) && !(obj->m.cool_down == m.cool_down)) {
		m.cool_down = obj->m.cool_down;
		MarkUpdateAll(CoolDownMask, true);
	}
}

void SkillData_Stub::ReadAttr(CONST Buffer &buf)
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

void SkillData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult SkillData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_SkillData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult SkillData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_SkillData_REQ_STATE:
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

