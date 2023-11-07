#pragma once
#ifndef _PET_H_
#define _PET_H_

#include "Stub/Logic/Pet_Stub.h"
#include "Resource/ResPet.h"

class CPetData;
class CPetGroup;
class CSkill;
class CSkillGroup;

const struct PetData;



class CPet : public Pet_Stub,
			public Reuben::Simulation::IEventHandler<CPet>
{
private:
	typedef Pet_Stub Parent;

	CPetGroup*	m_pOwnerGroup;
	
	UInt16Vector m_LearnSkillID;
	bool		m_bHasAddSkill;

public:
	DEFINE_CLASS(Pet_CLASSID);

	CPet();
	virtual ~CPet();
	INLINE static CPet* NewInstance();

	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event
	virtual void HandleEvent(CONST Event &event);

	void SetData(CPetData *pData);				// set pet by CPetData
	void WriteData(CPetData *pData);			// write pet to CPetData

	Boolean AddSkill(CSkill* pSkill);

	void SetPetGroup(CPetGroup* ownerGroup);
	enum EventEnum
	{
		//EVT_UPDATE_OBJECT	= MAKE_EVENTID(Pet_CLASSID, 1),	// update object
		EVT_UPDATE_PET_INFO = MAKE_EVENTID(Pet_CLASSID, 1),	// update object
		EVT_UPDATE_PET_EXPIRE = MAKE_EVENTID(Pet_CLASSID, 2),	// update object
		EVT_UPDATE_PET_VP	= MAKE_EVENTID(Pet_CLASSID, 3),	// update object
		EVT_UPDATE_PET_FAVOR = MAKE_EVENTID(Pet_CLASSID, 4),	// update object
	};

	NetGroup* GetSkillGroup(bool forSend = false);
	void MakeLearnSkillLst(bool forNewPet=false, bool toObject=false);
	UInt16Vector GetLearnSkillLst(){ return m_LearnSkillID; }
	void ClearLearnSkillLst(){ m_LearnSkillID.clear(); }
	RPCResult DoLearnSkill(RPCContext &context);
	RPCResult DoRemoveSkill(RPCContext &context);
	void ChangeState(BYTE state);
	void UpdateInfo();

	virtual RPCResult ChangePetName(LPCPEER pPeer, RPCContext &context, const String &newName);
	virtual RPCResult ChangePetExp(LPCPEER pPeer, RPCContext &context, const UInt32 &addExp);
	virtual RPCResult ChangePetVp(LPCPEER pPeer, RPCContext &context, const int &addVp);
	virtual RPCResult ChangePetFavor(LPCPEER pPeer, RPCContext &context, const int &addFavor);
	
	INLINE void SetHasAddSkill(bool hasAdd){ m_bHasAddSkill = hasAdd; };
	INLINE bool HasAddSkill(){ return m_bHasAddSkill;}

	//RPCResult UpdateToObject(RPCContext &context);

	void PreSend(NetGroup* skillGroup=NULL);
	void ChangeLevel(Int change);
	void ChangeVP(Int change);
	void ChangeHappiness(Int change);
	void ChangeExp(Int change);
	bool HasSkill(UInt16 skill_id);
	void ResetPet();
	void ChangeSpirit(Int change);
	void ChangeLv(Int change);
	void ResetLevel(UInt16 value);
	RPCResult RegenSkill(RPCContext &context);

	void ClonePet(CPet* pPet);

	RPCContext &GetOwnerContext();

	UInt8 GetPetType();
	UInt8 GetPetTimeLimitType();
	void RemoveSkill(UInt16 skillId);
	void AddSkill(UInt16 skillId);
	UInt8 GetSkillCount();
	UInt16	GetRandomSkillID();
	Boolean GetPetState();

	CSkillGroup*	m_pSkillGroup;
	UInt16			m_iNewSkillID;

	typedef StlMap<UInt16, CSkill*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, CSkill*> > > SkillMap;

	SkillMap		m_skillMap;

	UInt32			m_PreUpdateTime;

	UInt32			m_newUsetime;
	UInt32			m_useTime;

	UInt32			m_newUsePetTwoTime;
	UInt32			m_usePetTwoTime;

	const PetData *	m_pPetData;
};

INLINE CPet* CPet::NewInstance()
{
	return CreateNetObject<CPet>();
}
#endif //_PET_H_
