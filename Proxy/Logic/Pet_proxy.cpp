//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Pet_proxy.h"


void Pet_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.pet_uid);
		buf.Read<UInt32>(m.pet_id);
	}
	if (m_updateMask.IsBit(owner_idMask))
	{
		buf.Read<UInt32>(m.owner_id);
	}
	if (m_updateMask.IsBit(nameMask))
	{
		buf.Read<String>(m.name);
	}
	if (m_updateMask.IsBit(sexMask))
	{
		buf.Read<UInt8>(m.sex);
	}
	if (m_updateMask.IsBit(levelMask))
	{
		buf.Read<UInt16>(m.level);
	}
	if (m_updateMask.IsBit(expMask))
	{
		buf.Read<UInt32>(m.exp);
	}
	if (m_updateMask.IsBit(stateMask))
	{
		buf.Read<UInt16>(m.state);
	}
	if (m_updateMask.IsBit(birthCountMask))
	{
		buf.Read<UInt16>(m.birthCount);
	}
	if (m_updateMask.IsBit(firstMasterMask))
	{
		buf.Read<String>(m.firstMaster);
	}
	if (m_updateMask.IsBit(fatherMask))
	{
		buf.Read<UInt32>(m.father_id);
		buf.Read<String>(m.father_name);
	}
	if (m_updateMask.IsBit(motherMask))
	{
		buf.Read<UInt32>(m.mother_id);
		buf.Read<String>(m.mother_name);
	}
	if (m_updateMask.IsBit(happinessMask))
	{
		buf.Read<UInt16>(m.happiness);
	}
	if (m_updateMask.IsBit(VPMask))
	{
		buf.Read<UInt16>(m.VP);
		buf.Read<UInt16>(m.maxVP);
	}
	if (m_updateMask.IsBit(elementMask))
	{
		buf.Read<UInt8>(m.element);
	}
	if (m_updateMask.IsBit(expireDateMask))
	{
		buf.Read<UInt32>(m.expireDate);
	}
	if (m_updateMask.IsBit(soulboundMask))
	{
		buf.Read<UInt8>(m.soulbound);
	}
	if (m_updateMask.IsBit(colorMask))
	{
		buf.Read<UInt16>(m.color);
	}
	if (m_updateMask.IsBit(spiritMask))
	{
		buf.Read<UInt16>(m.spirit);
	}
	if (m_updateMask.IsBit(deputeMask))
	{
		buf.Read<UInt32>(m.depute_master);
		buf.Read<UInt16>(m.depute_time);
		buf.Read<UInt16>(m.depute_useTime);
		buf.Read<UInt32>(m.depute_exp);
	}
	if (m_updateMask.IsBit(inWareHouseMask))
	{
		buf.Read<UInt8>(m.inWareHouse);
	}
	if (m_updateMask.IsBit(assetLockMask))
	{
		buf.Read<Boolean>(m.lock);
	}
}

void Pet_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Pet_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.pet_uid, m.pet_id);
	if (IsUpdated(owner_idMask)) OnUpdateOwner_id(m.owner_id);
	if (IsUpdated(nameMask)) OnUpdateName(m.name);
	if (IsUpdated(sexMask)) OnUpdateSex(m.sex);
	if (IsUpdated(levelMask)) OnUpdateLevel(m.level);
	if (IsUpdated(expMask)) OnUpdateExp(m.exp);
	if (IsUpdated(stateMask)) OnUpdateState(m.state);
	if (IsUpdated(birthCountMask)) OnUpdateBirthCount(m.birthCount);
	if (IsUpdated(firstMasterMask)) OnUpdateFirstMaster(m.firstMaster);
	if (IsUpdated(fatherMask)) OnUpdateFather(m.father_id, m.father_name);
	if (IsUpdated(motherMask)) OnUpdateMother(m.mother_id, m.mother_name);
	if (IsUpdated(happinessMask)) OnUpdateHappiness(m.happiness);
	if (IsUpdated(VPMask)) OnUpdateVP(m.VP, m.maxVP);
	if (IsUpdated(elementMask)) OnUpdateElement(m.element);
	if (IsUpdated(expireDateMask)) OnUpdateExpireDate(m.expireDate);
	if (IsUpdated(soulboundMask)) OnUpdateSoulbound(m.soulbound);
	if (IsUpdated(colorMask)) OnUpdateColor(m.color);
	if (IsUpdated(spiritMask)) OnUpdateSpirit(m.spirit);
	if (IsUpdated(deputeMask)) OnUpdateDepute(m.depute_master, m.depute_time, m.depute_useTime, m.depute_exp);
	if (IsUpdated(inWareHouseMask)) OnUpdateInWareHouse(m.inWareHouse);
	if (IsUpdated(assetLockMask)) OnUpdateAssetLock(m.lock);
}

#pragma optimize("", off)
RPCResult Pet_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Pet_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_Pet_CB_RaiseAttrib:
		{
			_result = CB_RaiseAttrib(pPeer, context);
		}
		break;
	case RPC_Pet_CB_LearnSkillMessage:
		{
			UInt16 skillID;
			pBuf->Read<UInt16>(skillID);
			_result = CB_LearnSkillMessage(pPeer, context, skillID);
		}
		break;
	case RPC_Pet_CB_ChangePetName:
		{
			bool success;
			pBuf->Read<bool>(success);
			_result = CB_ChangePetName(pPeer, context, success);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Pet_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Pet_UP_STATE:
			_result = RPCDecode<756689664>(pPeer, pBuf);
		break;
	case RPC_Pet_CB_RaiseAttrib:
			_result = RPCDecode<1127433689>(pPeer, pBuf);
		break;
	case RPC_Pet_CB_LearnSkillMessage:
			_result = RPCDecode<568205284>(pPeer, pBuf);
		break;
	case RPC_Pet_CB_ChangePetName:
			_result = RPCDecode<764498754>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
