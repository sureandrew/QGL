///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PET_PROXY_H_
#define _RPC_PET_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Pet_enum.h"

class Pet_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID);
	virtual RPCResult CB_ChangePetName(LPCPEER pPeer, RPCContext &context, const bool &success);
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &pet_uid, const UInt32 &pet_id);
	virtual void OnUpdateOwner_id(const UInt32 &owner_id);
	virtual void OnUpdateName(const String &name);
	virtual void OnUpdateSex(const UInt8 &sex);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateExp(const UInt32 &exp);
	virtual void OnUpdateState(const UInt16 &state);
	virtual void OnUpdateBirthCount(const UInt16 &birthCount);
	virtual void OnUpdateFirstMaster(const String &firstMaster);
	virtual void OnUpdateFather(const UInt32 &father_id, const String &father_name);
	virtual void OnUpdateMother(const UInt32 &mother_id, const String &mother_name);
	virtual void OnUpdateHappiness(const UInt16 &happiness);
	virtual void OnUpdateVP(const UInt16 &VP, const UInt16 &maxVP);
	virtual void OnUpdateElement(const UInt8 &element);
	virtual void OnUpdateExpireDate(const UInt32 &expireDate);
	virtual void OnUpdateSoulbound(const UInt8 &soulbound);
	virtual void OnUpdateColor(const UInt16 &color);
	virtual void OnUpdateSpirit(const UInt16 &spirit);
	virtual void OnUpdateDepute(const UInt32 &depute_master, const UInt16 &depute_time, const UInt16 &depute_useTime, const UInt32 &depute_exp);
	virtual void OnUpdateInWareHouse(const UInt8 &inWareHouse);
	virtual void OnUpdateAssetLock(const Boolean &lock);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		owner_idMask	= Parent::NextFreeMask + 1,
		nameMask	= Parent::NextFreeMask + 2,
		sexMask	= Parent::NextFreeMask + 3,
		levelMask	= Parent::NextFreeMask + 4,
		expMask	= Parent::NextFreeMask + 5,
		stateMask	= Parent::NextFreeMask + 6,
		birthCountMask	= Parent::NextFreeMask + 7,
		firstMasterMask	= Parent::NextFreeMask + 8,
		fatherMask	= Parent::NextFreeMask + 9,
		motherMask	= Parent::NextFreeMask + 10,
		happinessMask	= Parent::NextFreeMask + 11,
		VPMask	= Parent::NextFreeMask + 12,
		elementMask	= Parent::NextFreeMask + 13,
		expireDateMask	= Parent::NextFreeMask + 14,
		soulboundMask	= Parent::NextFreeMask + 15,
		colorMask	= Parent::NextFreeMask + 16,
		spiritMask	= Parent::NextFreeMask + 17,
		deputeMask	= Parent::NextFreeMask + 18,
		inWareHouseMask	= Parent::NextFreeMask + 19,
		assetLockMask	= Parent::NextFreeMask + 20,
		NextFreeMask	= Parent::NextFreeMask + 21
	};
	virtual RPCResult CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult ChangePetName(LPCPEER pPeer, RPCContext &context, const String &newName);
	virtual RPCResult CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID) = 0;
	virtual RPCResult CB_ChangePetName(LPCPEER pPeer, RPCContext &context, const bool &success) = 0;
	virtual RPCResult ChangePetExp(LPCPEER pPeer, RPCContext &context, const UInt32 &addExp);
	virtual RPCResult ChangePetVp(LPCPEER pPeer, RPCContext &context, const int &addVp);
	virtual RPCResult ChangePetFavor(LPCPEER pPeer, RPCContext &context, const int &addFavor);
	const UInt32& GetPet_uid() const { return m.pet_uid; }
	const UInt32& GetPet_id() const { return m.pet_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const String& GetName() const { return m.name; }
	const UInt8& GetSex() const { return m.sex; }
	const UInt16& GetLevel() const { return m.level; }
	const UInt32& GetExp() const { return m.exp; }
	const UInt16& GetState() const { return m.state; }
	const UInt16& GetBirthCount() const { return m.birthCount; }
	const String& GetFirstMaster() const { return m.firstMaster; }
	const UInt32& GetFather_id() const { return m.father_id; }
	const String& GetFather_name() const { return m.father_name; }
	const UInt32& GetMother_id() const { return m.mother_id; }
	const String& GetMother_name() const { return m.mother_name; }
	const UInt16& GetHappiness() const { return m.happiness; }
	const UInt16& GetVP() const { return m.VP; }
	const UInt16& GetMaxVP() const { return m.maxVP; }
	const UInt8& GetElement() const { return m.element; }
	const UInt32& GetExpireDate() const { return m.expireDate; }
	const UInt8& GetSoulbound() const { return m.soulbound; }
	const UInt16& GetColor() const { return m.color; }
	const UInt16& GetSpirit() const { return m.spirit; }
	const UInt32& GetDepute_master() const { return m.depute_master; }
	const UInt16& GetDepute_time() const { return m.depute_time; }
	const UInt16& GetDepute_useTime() const { return m.depute_useTime; }
	const UInt32& GetDepute_exp() const { return m.depute_exp; }
	const UInt8& GetInWareHouse() const { return m.inWareHouse; }
	const Boolean& GetLock() const { return m.lock; }
	virtual void OnUpdateId(const UInt32 &pet_uid, const UInt32 &pet_id) {};
	virtual void OnUpdateOwner_id(const UInt32 &owner_id) {};
	virtual void OnUpdateName(const String &name) {};
	virtual void OnUpdateSex(const UInt8 &sex) {};
	virtual void OnUpdateLevel(const UInt16 &level) {};
	virtual void OnUpdateExp(const UInt32 &exp) {};
	virtual void OnUpdateState(const UInt16 &state) {};
	virtual void OnUpdateBirthCount(const UInt16 &birthCount) {};
	virtual void OnUpdateFirstMaster(const String &firstMaster) {};
	virtual void OnUpdateFather(const UInt32 &father_id, const String &father_name) {};
	virtual void OnUpdateMother(const UInt32 &mother_id, const String &mother_name) {};
	virtual void OnUpdateHappiness(const UInt16 &happiness) {};
	virtual void OnUpdateVP(const UInt16 &VP, const UInt16 &maxVP) {};
	virtual void OnUpdateElement(const UInt8 &element) {};
	virtual void OnUpdateExpireDate(const UInt32 &expireDate) {};
	virtual void OnUpdateSoulbound(const UInt8 &soulbound) {};
	virtual void OnUpdateColor(const UInt16 &color) {};
	virtual void OnUpdateSpirit(const UInt16 &spirit) {};
	virtual void OnUpdateDepute(const UInt32 &depute_master, const UInt16 &depute_time, const UInt16 &depute_useTime, const UInt32 &depute_exp) {};
	virtual void OnUpdateInWareHouse(const UInt8 &inWareHouse) {};
	virtual void OnUpdateAssetLock(const Boolean &lock) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 pet_uid;
	UInt32 pet_id;
	//owner_id
	UInt32 owner_id;
	//name
	String name;
	//sex
	UInt8 sex;
	//level
	UInt16 level;
	//exp
	UInt32 exp;
	//state
	UInt16 state;
	//birthCount
	UInt16 birthCount;
	//firstMaster
	String firstMaster;
	//father
	UInt32 father_id;
	String father_name;
	//mother
	UInt32 mother_id;
	String mother_name;
	//happiness
	UInt16 happiness;
	//VP
	UInt16 VP;
	UInt16 maxVP;
	//element
	UInt8 element;
	//expireDate
	UInt32 expireDate;
	//soulbound
	UInt8 soulbound;
	//color
	UInt16 color;
	//spirit
	UInt16 spirit;
	//depute
	UInt32 depute_master;
	UInt16 depute_time;
	UInt16 depute_useTime;
	UInt32 depute_exp;
	//inWareHouse
	UInt8 inWareHouse;
	//assetLock
	Boolean lock;
} m;

public:
	INLINE Pet_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Pet_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Pet_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1040144042>(pPeer, pBuf);
}

INLINE RPCResult Pet_Proxy::ChangePetName(LPCPEER pPeer, RPCContext &context, const String &newName)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Pet_ChangePetName);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(newName);
	return RPCSend<1504335914>(pPeer, pBuf);
}

INLINE RPCResult Pet_Proxy::ChangePetExp(LPCPEER pPeer, RPCContext &context, const UInt32 &addExp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Pet_ChangePetExp);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(addExp);
	return RPCSend<187706123>(pPeer, pBuf);
}

INLINE RPCResult Pet_Proxy::ChangePetVp(LPCPEER pPeer, RPCContext &context, const int &addVp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Pet_ChangePetVp);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(addVp);
	return RPCSend<1436422192>(pPeer, pBuf);
}

INLINE RPCResult Pet_Proxy::ChangePetFavor(LPCPEER pPeer, RPCContext &context, const int &addFavor)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Pet_ChangePetFavor);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<int>(addFavor);
	return RPCSend<1227570555>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(756689664)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1040144042)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1127433689)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1504335914)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(568205284)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(764498754)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(187706123)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1436422192)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1227570555)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_PET_PROXY_H_
