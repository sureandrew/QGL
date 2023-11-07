//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Pet_stub.h"

void Pet_Stub::SetId(const UInt32 &pet_uid, const UInt32 &pet_id)
{
	bool modified = false;
	if (m.pet_uid != pet_uid) { m.pet_uid = pet_uid; modified = true; }
	if (m.pet_id != pet_id) { m.pet_id = pet_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void Pet_Stub::SetPet_uid(const UInt32 &pet_uid)
{
	bool modified = false;
	if (m.pet_uid != pet_uid) { m.pet_uid = pet_uid; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void Pet_Stub::SetPet_id(const UInt32 &pet_id)
{
	bool modified = false;
	if (m.pet_id != pet_id) { m.pet_id = pet_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void Pet_Stub::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(owner_idMask, modified);
}

void Pet_Stub::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(nameMask, modified);
}

void Pet_Stub::SetSex(const UInt8 &sex)
{
	bool modified = false;
	if (m.sex != sex) { m.sex = sex; modified = true; }
	MarkUpdateAll(sexMask, modified);
}

void Pet_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void Pet_Stub::SetExp(const UInt32 &exp)
{
	bool modified = false;
	if (m.exp != exp) { m.exp = exp; modified = true; }
	MarkUpdateAll(expMask, modified);
}

void Pet_Stub::SetState(const UInt16 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}

void Pet_Stub::SetBirthCount(const UInt16 &birthCount)
{
	bool modified = false;
	if (m.birthCount != birthCount) { m.birthCount = birthCount; modified = true; }
	MarkUpdateAll(birthCountMask, modified);
}

void Pet_Stub::SetFirstMaster(const String &firstMaster)
{
	bool modified = false;
	if (m.firstMaster != firstMaster) { m.firstMaster = firstMaster; modified = true; }
	MarkUpdateAll(firstMasterMask, modified);
}

void Pet_Stub::SetFather(const UInt32 &father_id, const String &father_name)
{
	bool modified = false;
	if (m.father_id != father_id) { m.father_id = father_id; modified = true; }
	if (m.father_name != father_name) { m.father_name = father_name; modified = true; }
	MarkUpdateAll(fatherMask, modified);
}

void Pet_Stub::SetFather_id(const UInt32 &father_id)
{
	bool modified = false;
	if (m.father_id != father_id) { m.father_id = father_id; modified = true; }
	MarkUpdateAll(fatherMask, modified);
}

void Pet_Stub::SetFather_name(const String &father_name)
{
	bool modified = false;
	if (m.father_name != father_name) { m.father_name = father_name; modified = true; }
	MarkUpdateAll(fatherMask, modified);
}

void Pet_Stub::SetMother(const UInt32 &mother_id, const String &mother_name)
{
	bool modified = false;
	if (m.mother_id != mother_id) { m.mother_id = mother_id; modified = true; }
	if (m.mother_name != mother_name) { m.mother_name = mother_name; modified = true; }
	MarkUpdateAll(motherMask, modified);
}

void Pet_Stub::SetMother_id(const UInt32 &mother_id)
{
	bool modified = false;
	if (m.mother_id != mother_id) { m.mother_id = mother_id; modified = true; }
	MarkUpdateAll(motherMask, modified);
}

void Pet_Stub::SetMother_name(const String &mother_name)
{
	bool modified = false;
	if (m.mother_name != mother_name) { m.mother_name = mother_name; modified = true; }
	MarkUpdateAll(motherMask, modified);
}

void Pet_Stub::SetHappiness(const UInt16 &happiness)
{
	bool modified = false;
	if (m.happiness != happiness) { m.happiness = happiness; modified = true; }
	MarkUpdateAll(happinessMask, modified);
}

void Pet_Stub::SetVP(const UInt16 &VP, const UInt16 &maxVP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	if (m.maxVP != maxVP) { m.maxVP = maxVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void Pet_Stub::SetVP(const UInt16 &VP)
{
	bool modified = false;
	if (m.VP != VP) { m.VP = VP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void Pet_Stub::SetMaxVP(const UInt16 &maxVP)
{
	bool modified = false;
	if (m.maxVP != maxVP) { m.maxVP = maxVP; modified = true; }
	MarkUpdateAll(VPMask, modified);
}

void Pet_Stub::SetElement(const UInt8 &element)
{
	bool modified = false;
	if (m.element != element) { m.element = element; modified = true; }
	MarkUpdateAll(elementMask, modified);
}

void Pet_Stub::SetExpireDate(const UInt32 &expireDate)
{
	bool modified = false;
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(expireDateMask, modified);
}

void Pet_Stub::SetSoulbound(const UInt8 &soulbound)
{
	bool modified = false;
	if (m.soulbound != soulbound) { m.soulbound = soulbound; modified = true; }
	MarkUpdateAll(soulboundMask, modified);
}

void Pet_Stub::SetColor(const UInt16 &color)
{
	bool modified = false;
	if (m.color != color) { m.color = color; modified = true; }
	MarkUpdateAll(colorMask, modified);
}

void Pet_Stub::SetSpirit(const UInt16 &spirit)
{
	bool modified = false;
	if (m.spirit != spirit) { m.spirit = spirit; modified = true; }
	MarkUpdateAll(spiritMask, modified);
}

void Pet_Stub::SetDepute(const UInt32 &depute_master, const UInt16 &depute_time, const UInt16 &depute_useTime, const UInt32 &depute_exp)
{
	bool modified = false;
	if (m.depute_master != depute_master) { m.depute_master = depute_master; modified = true; }
	if (m.depute_time != depute_time) { m.depute_time = depute_time; modified = true; }
	if (m.depute_useTime != depute_useTime) { m.depute_useTime = depute_useTime; modified = true; }
	if (m.depute_exp != depute_exp) { m.depute_exp = depute_exp; modified = true; }
	MarkUpdateAll(deputeMask, modified);
}

void Pet_Stub::SetDepute_master(const UInt32 &depute_master)
{
	bool modified = false;
	if (m.depute_master != depute_master) { m.depute_master = depute_master; modified = true; }
	MarkUpdateAll(deputeMask, modified);
}

void Pet_Stub::SetDepute_time(const UInt16 &depute_time)
{
	bool modified = false;
	if (m.depute_time != depute_time) { m.depute_time = depute_time; modified = true; }
	MarkUpdateAll(deputeMask, modified);
}

void Pet_Stub::SetDepute_useTime(const UInt16 &depute_useTime)
{
	bool modified = false;
	if (m.depute_useTime != depute_useTime) { m.depute_useTime = depute_useTime; modified = true; }
	MarkUpdateAll(deputeMask, modified);
}

void Pet_Stub::SetDepute_exp(const UInt32 &depute_exp)
{
	bool modified = false;
	if (m.depute_exp != depute_exp) { m.depute_exp = depute_exp; modified = true; }
	MarkUpdateAll(deputeMask, modified);
}

void Pet_Stub::SetInWareHouse(const UInt8 &inWareHouse)
{
	bool modified = false;
	if (m.inWareHouse != inWareHouse) { m.inWareHouse = inWareHouse; modified = true; }
	MarkUpdateAll(inWareHouseMask, modified);
}

void Pet_Stub::SetAssetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}

void Pet_Stub::SetLock(const Boolean &lock)
{
	bool modified = false;
	if (m.lock != lock) { m.lock = lock; modified = true; }
	MarkUpdateAll(assetLockMask, modified);
}



void Pet_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Pet_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult Pet_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Pet_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Pet_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Pet_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_Pet_ChangePetName:
		{
			String newName;
			pBuf->Read<String>(newName);
			_result = ChangePetName(pPeer, context, newName);
		}
		break;
	case RPC_Pet_ChangePetExp:
		{
			UInt32 addExp;
			pBuf->Read<UInt32>(addExp);
			_result = ChangePetExp(pPeer, context, addExp);
		}
		break;
	case RPC_Pet_ChangePetVp:
		{
			int addVp;
			pBuf->Read<int>(addVp);
			_result = ChangePetVp(pPeer, context, addVp);
		}
		break;
	case RPC_Pet_ChangePetFavor:
		{
			int addFavor;
			pBuf->Read<int>(addFavor);
			_result = ChangePetFavor(pPeer, context, addFavor);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult Pet_Stub::CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Pet_CB_RaiseAttrib);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1127433689>(pPeer, pBuf);
}

RPCResult Pet_Stub::CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Pet_CB_LearnSkillMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(skillID);
	return RPCSend<568205284>(pPeer, pBuf);
}

RPCResult Pet_Stub::CB_ChangePetName(LPCPEER pPeer, RPCContext &context, const bool &success)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Pet_CB_ChangePetName);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<bool>(success);
	return RPCSend<764498754>(pPeer, pBuf);
}

