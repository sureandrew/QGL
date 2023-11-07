#pragma once
#ifndef _PETGROUP_H_
#define _PETGROUP_H_

#include "Proxy/Logic/PetGroup_Proxy.h"

class CPet;
class CSkillGroup;

class CPetGroup : public PetGroup_Proxy
{
private:
	typedef PetGroup_Proxy Parent;
	virtual RPCResult CB_AddPets(LPCPEER pPeer, RPCContext &context, NetGroup* petGrp, NetGroup* skillGroup);
	virtual RPCResult CB_RemovePets(LPCPEER pPeer, RPCContext &context, const UInt32Vector &pet_ids);
	virtual RPCResult CB_UpdateAllPets(LPCPEER pPeer, RPCContext &context, NetGroup* petlGrp);
	virtual RPCResult CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex);
// OnUpdate can be implemented selectively
	virtual void OnUpdateMaster(const UInt32 &master_id);

	typedef StlMap<UInt32, CSkillGroup*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CSkillGroup*> > > SkillMap;
	SkillMap m_petSkillMap;

public:
	DEFINE_CLASS(PetGroup_CLASSID);
	Int32 GetPetIndex(UInt32 pet_uid);
	CPet* GetPet(UInt32 pet_uid);
	CPet* GetPetByIndex(int index);
	CPetGroup();
	virtual ~CPetGroup();
	void SetSkillGroup(UInt32 pet_uid, CSkillGroup* skillGroup);
	CSkillGroup* GetSkillGroup(UInt32 pet_uid);
	void RemoveSkillGroup(UInt32 pet_uid, bool needDelete = false);
};

#endif //_PETGROUP_H_
