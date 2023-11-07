#pragma once
#ifndef _PARTNERGROUP_H_
#define _PARTNERGROUP_H_

#include "Proxy/Logic/PartnerGroup_Proxy.h"

class CPartner;

class CPartnerGroup : public PartnerGroup_Proxy
{
public:
	virtual RPCResult CB_DeleteListSuccess(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerIDs, const bool &showDelMsg);
	virtual RPCResult CB_AddPartnerGroup(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup, NetGroup* skillGroups);
	virtual RPCResult CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AddPartner(LPCPEER pPeer, RPCContext &context, NetObject* partner, NetGroup* skillGroup);
	virtual RPCResult CB_UpdateAllPartner(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup);
	virtual RPCResult CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp);
	virtual RPCResult CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex);
	virtual RPCResult CB_PartnerCultivationUp(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt8 &msgValue, const UInt32 &src_partnerID);

	DEFINE_CLASS(PartnerGroup_CLASSID);
	CPartnerGroup();
	virtual ~CPartnerGroup();
	CPartner* Get(UInt16 index);
	CPartner* GetPartner(UInt32 partner_id);
	CPartner* GetPartnerByIndex(int index);
	UInt8 GetPartnerCount(UInt32 mob_id);
	void SetSkillGroups(NetGroup* skillGroups);
	void ShowAllPartner();

private:
	typedef PartnerGroup_Proxy Parent;
};

#endif //_PARTNERGROUP_H_
