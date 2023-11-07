//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/SkillGroup_proxy.h"


void SkillGroup_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(ownerMask))
	{
		buf.Read<UInt32>(m.owner_id);
		buf.Read<BYTE>(m.owner_type);
	}
}

void SkillGroup_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void SkillGroup_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(ownerMask)) OnUpdateOwner(m.owner_id, m.owner_type);
}

#pragma optimize("", off)
RPCResult SkillGroup_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_SkillGroup_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_SkillGroup_CB_AddSkills:
		{
			NetGroup* skillGrp;
			pBuf->Read<NetGroup*>(skillGrp);
			_result = CB_AddSkills(pPeer, context, skillGrp);
		}
		break;
	case RPC_SkillGroup_CB_RemoveSkills:
		{
			UInt16Vector skill_ids;
			pBuf->Read<UInt16Vector>(skill_ids);
			_result = CB_RemoveSkills(pPeer, context, skill_ids);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult SkillGroup_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_SkillGroup_UP_STATE:
			_result = RPCDecode<423504350>(pPeer, pBuf);
		break;
	case RPC_SkillGroup_CB_AddSkills:
			_result = RPCDecode<148256391>(pPeer, pBuf);
		break;
	case RPC_SkillGroup_CB_RemoveSkills:
			_result = RPCDecode<393032429>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
