#pragma once
#ifndef _SKILLGROUP_H_
#define _SKILLGROUP_H_

#include "Stub/Logic/SkillGroup_Stub.h"

class CSkill;
class CCharacter;
class CPartner;
class CPet;

class CSkillGroup : public SkillGroup_Stub,
		public Reuben::Simulation::IEventHandler<CSkillGroup>
{
public:
	DEFINE_CLASS(SkillGroup_CLASSID);
	CSkillGroup();
	virtual ~CSkillGroup();
	INLINE static CSkillGroup* NewInstance();

	void PreSend();
	void SendAll();
	Boolean Add(NetObject *obj);
	Boolean Remove(NetObject *obj);
	CSkill* GetSkill(UInt16 skillId);
	LPCPEER GetOwnerPeer();
	RPCContext& GetOwnerContext();
	void SetOwner(CCharacter *pChar);
	void SetOwner(CPartner *pPartner);
	void SetOwner(CPet *pPartner);
	void SetOwner(const UInt32 &owner_id, const BYTE &owner_type);
	RPCResult UpdateSkills(RPCContext &context);		// update all skills to object service
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event
	virtual void HandleEvent(CONST Event &event);
	void RemoveAllSkill();
	enum EventEnum
	{
		EVT_UPDATE_OBJECT	= MAKE_EVENTID(SkillGroup_CLASSID, 1),	// update object
	};
private:
	typedef SkillGroup_Stub Parent;
	typedef StlMap<UInt16, CSkill*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, CSkill*> > > SkillMap;
	SkillMap		m_skillMap;
	CCharacter*		m_pCharOwner;
	CPartner*		m_pPartnerOwner;
	CPet*			m_pPetOwner;
};

INLINE CSkillGroup* CSkillGroup::NewInstance()
{
	return CreateNetObject<CSkillGroup>();
}

INLINE void CSkillGroup::SetOwner(CCharacter *pChar)
{
	m_pCharOwner = pChar; 
	m_pPartnerOwner = NULL;
	m_pPetOwner = NULL;
}

INLINE void CSkillGroup::SetOwner(CPartner *pPartner)
{
	m_pCharOwner = NULL;
	m_pPartnerOwner = pPartner;
	m_pPetOwner = NULL;
}

INLINE void CSkillGroup::SetOwner(CPet *pPet)
{
	m_pCharOwner = NULL;
	m_pPartnerOwner = NULL;
	m_pPetOwner = pPet;
}

INLINE void CSkillGroup::SetOwner(const UInt32 &owner_id, const BYTE &owner_type)
{
	Parent::SetOwner(owner_id, owner_type);
}

#endif //_SKILLGROUP_H_
