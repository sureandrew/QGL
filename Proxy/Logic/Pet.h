#pragma once
#ifndef _PET_H_
#define _PET_H_

#include "Proxy/Logic/Pet_Proxy.h"

class CSkill;
class CSkillGroup;
class CPet : public Pet_Proxy
{
public:
	DEFINE_CLASS(Pet_CLASSID);
	CSkillGroup * m_pSkillGrp;
	bool m_bGiven;
	CPet();
	virtual ~CPet();
	void SetSkillGroup(CSkillGroup *grp);

	INLINE void SetGiven(bool given){ m_bGiven = given;}
	INLINE bool IsGiven(){return m_bGiven;}

private:
	typedef Pet_Proxy Parent;
	virtual RPCResult CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &pet_uid, const UInt32 &pet_id);
	virtual void OnUpdateOwner(const UInt32 &pet_master);
	virtual void OnUpdateInfo(const String &pet_Name);
	virtual void OnUpdateLevel(const UInt16 &pet_lv);
	virtual void OnUpdateExp(const UInt32 &pet_exp);
	virtual void OnUpdateState(const UInt16 &pet_State);
	virtual void OnUpdateFather(const UInt32 &father_id, const String &father_name);
	virtual void OnUpdateMother(const UInt32 &mother_id, const String &mother_name);
	virtual void OnUpdateBirth(const UInt16 &birthCount);
	virtual void OnUpdateFirstMaster(const String &pet_firstmaster_name);
	virtual void OnUpdateHappiness(const UInt16 &pet_Happiness);
	virtual void OnUpdateVP(const UInt32 &pet_VP, const UInt32 &pet_maxVP);
	virtual void OnUpdateElement(const UInt8 &pet_element);
	virtual void OnUpdateExpireDate(const UInt32 &pet_ExpireDate);
	virtual void OnUpdateSoulbound(const bool &pet_soulbound_state);
	virtual void OnUpdateOutlook(const UInt16 &pet_color);
	virtual void OnUpdateSpirit(const UInt16 &spirit);
	virtual void OnUpdateDepute(const UInt32 &pet_depute_Master, const UInt32 &pet_depute_Time, const UInt32 &pet_depute_UseTime, const UInt32 &pet_depute_Exp);
	virtual void OnUpdateAssetLock(const Boolean &lock);	
	virtual RPCResult CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID);
	virtual RPCResult CB_ChangePetName(LPCPEER pPeer, RPCContext &context, const bool &success);
};

#endif //_PET_H_
