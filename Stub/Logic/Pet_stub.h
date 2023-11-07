///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PET_STUB_H_
#define _RPC_PET_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Pet_enum.h"

class Pet_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult ChangePetName(LPCPEER pPeer, RPCContext &context, const String &newName);
	virtual RPCResult ChangePetExp(LPCPEER pPeer, RPCContext &context, const UInt32 &addExp);
	virtual RPCResult ChangePetVp(LPCPEER pPeer, RPCContext &context, const int &addVp);
	virtual RPCResult ChangePetFavor(LPCPEER pPeer, RPCContext &context, const int &addFavor);
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
	virtual RPCResult CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ChangePetName(LPCPEER pPeer, RPCContext &context, const String &newName) = 0;
	virtual RPCResult CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID);
	virtual RPCResult CB_ChangePetName(LPCPEER pPeer, RPCContext &context, const bool &success);
	virtual RPCResult ChangePetExp(LPCPEER pPeer, RPCContext &context, const UInt32 &addExp) = 0;
	virtual RPCResult ChangePetVp(LPCPEER pPeer, RPCContext &context, const int &addVp) = 0;
	virtual RPCResult ChangePetFavor(LPCPEER pPeer, RPCContext &context, const int &addFavor) = 0;
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
	RPCMask m_modifyMask;
	INLINE Boolean IsModified() { return (!m_modifyMask.IsZero()); }
	INLINE Boolean IsModified(UInt bit) { return (m_modifyMask.IsBit(bit)); }
	INLINE Boolean IsModified(CONST RPCMask& mask) { return (m_modifyMask.IsMask(mask)); }
	INLINE VOID SetModifyMask(CONST RPCMask& mask) { m_modifyMask = mask; }
	INLINE CONST RPCMask& GetModifyMask() CONST { return m_modifyMask; }
	INLINE VOID ClearModifyMask() { m_modifyMask.Clear(); }
	INLINE VOID MarkModify(UInt bit) { m_modifyMask.MaskBit(bit); }
	INLINE VOID MarkModify(CONST RPCMask& mask) { m_modifyMask.Mask(mask); }
	INLINE VOID UnmarkModify(UInt bit) { m_modifyMask.UnmaskBit(bit); }
	INLINE VOID UnmarkModify(CONST RPCMask& mask) { m_modifyMask.Unmask(mask); }
	INLINE virtual VOID ClearAllMask() {
		Parent::ClearAllMask();
		m_modifyMask.Clear();
	}
	INLINE virtual VOID MarkUpdateAll(UInt bit, bool modified) {
		Parent::MarkUpdateAll(bit, modified);
		if (modified) m_modifyMask.MaskBit(bit);
	}
	void SetId(const UInt32 &pet_uid, const UInt32 &pet_id);
	void SetPet_uid(const UInt32 &pet_uid);
	void SetPet_id(const UInt32 &pet_id);
	void SetOwner_id(const UInt32 &owner_id);
	void SetName(const String &name);
	void SetSex(const UInt8 &sex);
	void SetLevel(const UInt16 &level);
	void SetExp(const UInt32 &exp);
	void SetState(const UInt16 &state);
	void SetBirthCount(const UInt16 &birthCount);
	void SetFirstMaster(const String &firstMaster);
	void SetFather(const UInt32 &father_id, const String &father_name);
	void SetFather_id(const UInt32 &father_id);
	void SetFather_name(const String &father_name);
	void SetMother(const UInt32 &mother_id, const String &mother_name);
	void SetMother_id(const UInt32 &mother_id);
	void SetMother_name(const String &mother_name);
	void SetHappiness(const UInt16 &happiness);
	void SetVP(const UInt16 &VP, const UInt16 &maxVP);
	void SetVP(const UInt16 &VP);
	void SetMaxVP(const UInt16 &maxVP);
	void SetElement(const UInt8 &element);
	void SetExpireDate(const UInt32 &expireDate);
	void SetSoulbound(const UInt8 &soulbound);
	void SetColor(const UInt16 &color);
	void SetSpirit(const UInt16 &spirit);
	void SetDepute(const UInt32 &depute_master, const UInt16 &depute_time, const UInt16 &depute_useTime, const UInt32 &depute_exp);
	void SetDepute_master(const UInt32 &depute_master);
	void SetDepute_time(const UInt16 &depute_time);
	void SetDepute_useTime(const UInt16 &depute_useTime);
	void SetDepute_exp(const UInt32 &depute_exp);
	void SetInWareHouse(const UInt8 &inWareHouse);
	void SetAssetLock(const Boolean &lock);
	void SetLock(const Boolean &lock);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE Pet_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_PET_STUB_H_
