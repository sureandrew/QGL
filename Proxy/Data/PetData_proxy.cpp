//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/PetData_proxy.h"

void PetData_Proxy::SetId(const UInt32 &pet_uid, const UInt32 &pet_id)
{
	bool modified = false;
	if (m.pet_uid != pet_uid) { m.pet_uid = pet_uid; modified = true; }
	if (m.pet_id != pet_id) { m.pet_id = pet_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void PetData_Proxy::SetPet_uid(const UInt32 &pet_uid)
{
	bool modified = false;
	if (m.pet_uid != pet_uid) { m.pet_uid = pet_uid; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void PetData_Proxy::SetPet_id(const UInt32 &pet_id)
{
	bool modified = false;
	if (m.pet_id != pet_id) { m.pet_id = pet_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void PetData_Proxy::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(owner_idMask, modified);
}

void PetData_Proxy::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(nameMask, modified);
}

void PetData_Proxy::SetSex(const UInt8 &sex)
{
	bool modified = false;
	if (m.sex != sex) { m.sex = sex; modified = true; }
	MarkUpdateAll(sexMask, modified);
}

void PetData_Proxy::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void PetData_Proxy::SetExp(const UInt32 &exp)
{
	bool modified = false;
	if (m.exp != exp) { m.exp = exp; modified = true; }
	MarkUpdateAll(expMask, modified);
}

void PetData_Proxy::SetState(const UInt16 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}

void PetData_Proxy::SetBirthCount(const UInt16 &birthCount)
{
	bool modified = false;
	if (m.birthCount != birthCount) { m.birthCount = birthCount; modified = true; }
	MarkUpdateAll(birthCountMask, modified);
}

void PetData_Proxy::SetFirstMaster(const String &firstMaster)
{
	bool modified = false;
	if (m.firstMaster != firstMaster) { m.firstMaster = firstMaster; modified = true; }
	MarkUpdateAll(firstMasterMask, modified);
}

void PetData_Proxy::SetFather(const UInt32 &father_id, const String &father_name)
{
	bool modified = false;
	if (m.father_id != father_id) { m.father_id = father_id; modified = true; }
	if (m.father_name != father_name) { m.father_name = father_name; modified = true; }
	MarkUpdateAll(fatherMask, modified);
}

void PetData_Proxy::SetFather_id(const UInt32 &father_id)
{
	bool modified = false;
	if (m.father_id != father_id) { m.father_id = father_id; modified = true; }
	MarkUpdateAll(fatherMask, modified);
}

void PetData_Proxy::SetFather_name(const String &father_name)
{
	bool modified = false;
	if (m.father_name != father_name) { m.father_name = father_name; modified = true; }
	MarkUpdateAll(fatherMask, modified);
}

void PetData_Proxy::SetMother(const UInt32 &mother_id, const String &mother_name)
{
	bool modified = false;
	if (m.mother_id != mother_id) { m.mother_id = mother_id; modified = true; }
	if (m.mother_name != mother_name) { m.mother_name = mother_name; modified = true; }
	MarkUpdateAll(motherMask, modified);
}

void PetData_Proxy::SetMother_id(const UInt32 &mother_id)
{
	bool modified = false;
	if (m.mother_id != mother_id) { m.mother_id = mother_id; modified = true; }
	MarkUpdateAll(motherMask, modified);
}

void PetData_Proxy::SetMother_name(const String &mother_name)
{
	bool modified = false;
	if (m.mother_name != mother_name) { m.mother_name = mother_name; modified = true; }
	MarkUpdateAll(motherMask, modified);
}

void PetData_Proxy::SetHappiness(const UInt16 &happiness)
{
	bool modified = false;
	if (m.happiness != happiness) { m.happiness = happiness; modified = true; }
	MarkUpdateAll(happinessMask, modified);
}

void PetData_Proxy::SetVP(const UInt16 &VP, const UInt16 &maxVP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	if (m.maxVP != maxVP) { m.maxVP = maxVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void PetData_Proxy::SetVP(const UInt16 &VP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void PetData_Proxy::SetMaxVP(const UInt16 &maxVP)
{
	bool modified = false;
	if (m.maxVP != maxVP) { m.maxVP = maxVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void PetData_Proxy::SetElement(const UInt8 &element)
{
	bool modified = false;
	if (m.element != element) { m.element = element; modified = true; }
	MarkUpdateAll(elementMask, modified);
}

void PetData_Proxy::SetExpireDate(const UInt32 &expireDate)
{
	bool modified = false;
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(expireDateMask, modified);
}

void PetData_Proxy::SetSoulbound(const UInt8 &soulbound)
{
	bool modified = false;
	if (m.soulbound != soulbound) { m.soulbound = soulbound; modified = true; }
	MarkUpdateAll(soulboundMask, modified);
}

void PetData_Proxy::SetColor(const UInt16 &color)
{
	bool modified = false;
	if (m.color != color) { m.color = color; modified = true; }
	MarkUpdateAll(colorMask, modified);
}

void PetData_Proxy::SetSpirit(const UInt16 &spirit)
{
	bool modified = false;
	if (m.spirit != spirit) { m.spirit = spirit; modified = true; }
	MarkUpdateAll(spiritMask, modified);
}

void PetData_Proxy::SetDepute(const UInt32 &depute_master, const UInt16 &depute_time, const UInt16 &depute_useTime, const UInt32 &depute_exp)
{
	bool modified = false;
	if (m.depute_master != depute_master) { m.depute_master = depute_master; modified = true; }
	if (m.depute_time != depute_time) { m.depute_time = depute_time; modified = true; }
	if (m.depute_useTime != depute_useTime) { m.depute_useTime = depute_useTime; modified = true; }
	if (m.depute_exp != depute_exp) { m.depute_exp = depute_exp; modified = true; }
	MarkUpdateAll(deputeMask, modified);
}

void PetData_Proxy::SetDepute_master(const UInt32 &depute_master)
{
	bool modified = false;
	if (m.depute_master != depute_master) { m.depute_master = depute_master; modified = true; }
	MarkUpdateAll(deputeMask, modified);
}

void PetData_Proxy::SetDepute_time(const UInt16 &depute_time)
{
	bool modified = false;
	if (m.depute_time != depute_time) { m.depute_time = depute_time; modified = true; }
	MarkUpdateAll(deputeMask, modified);
}

void PetData_Proxy::SetDepute_useTime(const UInt16 &depute_useTime)
{
	bool modified = false;
	if (m.depute_useTime != depute_useTime) { m.depute_useTime = depute_useTime; modified = true; }
	MarkUpdateAll(deputeMask, modified);
}

void PetData_Proxy::SetDepute_exp(const UInt32 &depute_exp)
{
	bool modified = false;
	if (m.depute_exp != depute_exp) { m.depute_exp = depute_exp; modified = true; }
	MarkUpdateAll(deputeMask, modified);
}

void PetData_Proxy::SetInWareHouse(const UInt8 &inWareHouse)
{
	bool modified = false;
	if (m.inWareHouse != inWareHouse) { m.inWareHouse = inWareHouse; modified = true; }
	MarkUpdateAll(inWareHouseMask, modified);
}

void PetData_Proxy::SetAssetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}

void PetData_Proxy::SetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}


void PetData_Proxy::ReadAttr(CONST Buffer &buf)
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

void PetData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.pet_uid);
		buf.Write<UInt32>(m.pet_id);
	}
	if (mask.IsBit(owner_idMask))
	{
		buf.Write<UInt32>(m.owner_id);
	}
	if (mask.IsBit(nameMask))
	{
		buf.Write<String>(m.name);
	}
	if (mask.IsBit(sexMask))
	{
		buf.Write<UInt8>(m.sex);
	}
	if (mask.IsBit(levelMask))
	{
		buf.Write<UInt16>(m.level);
	}
	if (mask.IsBit(expMask))
	{
		buf.Write<UInt32>(m.exp);
	}
	if (mask.IsBit(stateMask))
	{
		buf.Write<UInt16>(m.state);
	}
	if (mask.IsBit(birthCountMask))
	{
		buf.Write<UInt16>(m.birthCount);
	}
	if (mask.IsBit(firstMasterMask))
	{
		buf.Write<String>(m.firstMaster);
	}
	if (mask.IsBit(fatherMask))
	{
		buf.Write<UInt32>(m.father_id);
		buf.Write<String>(m.father_name);
	}
	if (mask.IsBit(motherMask))
	{
		buf.Write<UInt32>(m.mother_id);
		buf.Write<String>(m.mother_name);
	}
	if (mask.IsBit(happinessMask))
	{
		buf.Write<UInt16>(m.happiness);
	}
	if (mask.IsBit(VPMask))
	{
		buf.Write<UInt16>(m.VP);
		buf.Write<UInt16>(m.maxVP);
	}
	if (mask.IsBit(elementMask))
	{
		buf.Write<UInt8>(m.element);
	}
	if (mask.IsBit(expireDateMask))
	{
		buf.Write<UInt32>(m.expireDate);
	}
	if (mask.IsBit(soulboundMask))
	{
		buf.Write<UInt8>(m.soulbound);
	}
	if (mask.IsBit(colorMask))
	{
		buf.Write<UInt16>(m.color);
	}
	if (mask.IsBit(spiritMask))
	{
		buf.Write<UInt16>(m.spirit);
	}
	if (mask.IsBit(deputeMask))
	{
		buf.Write<UInt32>(m.depute_master);
		buf.Write<UInt16>(m.depute_time);
		buf.Write<UInt16>(m.depute_useTime);
		buf.Write<UInt32>(m.depute_exp);
	}
	if (mask.IsBit(inWareHouseMask))
	{
		buf.Write<UInt8>(m.inWareHouse);
	}
	if (mask.IsBit(assetLockMask))
	{
		buf.Write<Boolean>(m.lock);
	}
}

void PetData_Proxy::OnUpdate()
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
RPCResult PetData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_PetData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult PetData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_PetData_UP_STATE:
			_result = RPCDecode<1033896416>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
