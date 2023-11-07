//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Skill_stub.h"

void Skill_Stub::SetInfo(const UInt16 &skill_id)
{
	bool modified = false;
	if (m.skill_id != skill_id) { m.skill_id = skill_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Skill_Stub::SetSkill_id(const UInt16 &skill_id)
{
	bool modified = false;
	if (m.skill_id != skill_id) { m.skill_id = skill_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Skill_Stub::SetOwner_info(const UInt32 &owner_id, const BYTE &owner_type)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	if (m.owner_type != owner_type) { m.owner_type = owner_type; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void Skill_Stub::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void Skill_Stub::SetOwner_type(const BYTE &owner_type)
{
	bool modified = false;
	if (m.owner_type != owner_type) { m.owner_type = owner_type; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void Skill_Stub::SetLevel(const UInt16 &skill_level)
{
	bool modified = false;
	if (m.skill_level != skill_level) { m.skill_level = skill_level; modified = true; }
	MarkUpdateAll(LevelMask, modified);
}

void Skill_Stub::SetSkill_level(const UInt16 &skill_level)
{
	bool modified = false;
	if (m.skill_level != skill_level) { m.skill_level = skill_level; modified = true; }
	MarkUpdateAll(LevelMask, modified);
}

void Skill_Stub::SetCoolDown(const UInt32 &cool_down)
{
	bool modified = false;
	if (m.cool_down != cool_down) { m.cool_down = cool_down; modified = true; }
	MarkUpdateAll(CoolDownMask, modified);
}

void Skill_Stub::SetCool_down(const UInt32 &cool_down)
{
	bool modified = false;
	if (m.cool_down != cool_down) { m.cool_down = cool_down; modified = true; }
	MarkUpdateAll(CoolDownMask, modified);
}

void Skill_Stub::SetPartnerSkillType(const BYTE &partner_skill_type)
{
	bool modified = false;
	if (m.partner_skill_type != partner_skill_type) { m.partner_skill_type = partner_skill_type; modified = true; }
	MarkUpdateAll(PartnerSkillTypeMask, modified);
}

void Skill_Stub::SetPartner_skill_type(const BYTE &partner_skill_type)
{
	bool modified = false;
	if (m.partner_skill_type != partner_skill_type) { m.partner_skill_type = partner_skill_type; modified = true; }
	MarkUpdateAll(PartnerSkillTypeMask, modified);
}



void Skill_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Skill_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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
	if (mask.IsBit(PartnerSkillTypeMask))
	{
		buf.Write<BYTE>(m.partner_skill_type);
	}
}

RPCResult Skill_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Skill_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Skill_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Skill_REQ_STATE:
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

