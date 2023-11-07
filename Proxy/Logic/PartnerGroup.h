#pragma once
#ifndef _PARTNERGROUP_H_
#define _PARTNERGROUP_H_

#include "Proxy/Logic/PartnerGroup_Proxy.h"

class CPartner;
class CSkillGroup;

class CPartnerGroup : public PartnerGroup_Proxy
{
private:
	typedef PartnerGroup_Proxy Parent;
	typedef StlMap<UInt32, CSkillGroup*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CSkillGroup*> > > SkillMap;
	SkillMap m_partnerSkillMap;

public:
	DEFINE_CLASS(PartnerGroup_CLASSID);
	CPartnerGroup();
	virtual ~CPartnerGroup();
	virtual RPCResult CB_DeleteListSuccess(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerIDs, const bool &showDelMsg);
	virtual RPCResult CB_AddPartnerGroup(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup, NetGroup* skillGroups);
	virtual RPCResult CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AddPartner(LPCPEER pPeer, RPCContext &context, NetObject* partner, NetGroup* skillGroup);
	virtual RPCResult CB_UpdateAllPartner(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup);
	virtual RPCResult CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp);
	virtual RPCResult CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex);
	virtual RPCResult CB_PartnerCultivationUp(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt8 &msgValue, const UInt32 &src_partnerID);

	int GetPartnerCount(){ return GetCount();}
	CPartner* GetPartnerByIndex(int index);
	CPartner* GetPartner(UInt32 partner_id);
	Int32	  GetPartnerIndex(UInt32 partner_id);
	UInt8 GetPartnerCount(UInt32 mob_id);
	CPartner* GetCurrBattlePartner();
	void ResetBattleAllHP();

	void SetSkillGroup(UInt32 partnerID, CSkillGroup* skillGroup);
	CSkillGroup* GetSkillGroup(UInt32 partnerID);
	void RemoveSkillGroup(UInt32 partnerID, bool needDelete = true);

// OnUpdate can be implemented selectively
};

#endif //_PARTNERGROUP_H_
