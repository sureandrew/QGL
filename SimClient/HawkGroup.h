#pragma once
#ifndef _HAWKGROUP_H_
#define _HAWKGROUP_H_

#include "Proxy/Logic/HawkGroup_Proxy.h"

class CHawkItem;
typedef StlMap<UInt32, CHawkItem*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CHawkItem*> > > HawkItemMap;

class CHawkGroup : public HawkGroup_Proxy
{
private:
	typedef HawkGroup_Proxy Parent;
	HawkItemMap m_itemList;
	HawkItemMap m_partnerList;
	HawkItemMap m_petList;
    	

public:
	DEFINE_CLASS(HawkGroup_CLASSID);
// OnUpdate can be implemented selectively

	CHawkGroup();
	virtual ~CHawkGroup();

	void ConstructHawkItem();
	void ConstructHawkItem(NetGroup* itemGroup, NetGroup* partnerGroup, UInt16Vector partnerSkillID, NetGroup* petGroup, UInt16Vector petSkillID);
	INLINE HawkItemMap GetItemList(){ return m_itemList;}
	INLINE HawkItemMap GetPartnerList(){ return m_partnerList;}
	INLINE HawkItemMap GetPetList(){ return m_petList;}

	UInt16Vector GetPartnerSkillList(UInt32 partnerID);
	UInt16Vector GetPetSkillList(UInt32 petID);

	virtual RPCResult CB_CloseHawkForUser(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_ChangeHawkName(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_UpdateHawkItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemID, const UInt8 &itemType);
	virtual RPCResult CB_StopHawk(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_BuyHawkItemSuccess(LPCPEER pPeer, RPCContext &context, const Boolean &owner, const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemStack, const UInt32 &tax);
	virtual RPCResult CB_AddSellStartHawkItemInfo(LPCPEER pPeer, RPCContext &context,NetGroup* addHawkItem);

};

#endif //_HAWKGROUP_H_
