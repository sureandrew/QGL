#pragma once
#ifndef _HAWKGROUP_H_
#define _HAWKGROUP_H_

#include "Stub/Logic/HawkGroup_Stub.h"

#include "Stub/Logic/Character.h"

class CHawkItem;
//class CPartnerGroup;
//class CItemBag;
//class CPetGroup;
class CPartner;
class CItem;
class CPet;

class CHawkGroup : public HawkGroup_Stub, 
				public Reuben::Simulation::IEventHandler<CHawkGroup>
{
private:
	typedef HawkGroup_Stub Parent;
	CCharacter* m_pHawker;

	typedef StlMap<UInt32, CHawkItem*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CHawkItem*> > > HawkItemMap;
	HawkItemMap m_itemList;
	HawkItemMap m_partnerList;
	HawkItemMap m_petList;
	typedef StlMap<UInt32, RPCContext, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, RPCContext> > > CharContextList;
	CharContextList m_charContextList;
	UInt32 m_iStartTime;

public:
	DEFINE_CLASS(HawkGroup_CLASSID);

	CHawkGroup();

	virtual ~CHawkGroup();

	INLINE static CHawkGroup* NewInstance();

	INLINE void SetHawker(CCharacter* pChar){ m_pHawker = pChar;}
	void SetHawkItem(HawkerItemInfoVector itemVector, HawkerItemInfoVector partnerVector, HawkerItemInfoVector petVector);
	void GetItemNetGroup(NetGroup* itemGroup);
	void GetPartnerNetGroup(NetGroup* partnerGroup, UInt16Vector& partnerSkillID);
	void GetPetNetGroup(NetGroup* petGroup, UInt16Vector& petSkillID);
	void PreSend();
	void AddCharContext(UInt32 charID, RPCContext context);
	void RemoveCharID(UInt32 charID);

	void RaiseHawkNameUpdate();
	void CancelHawkNameUpdate();

	void RaiseHawkTimeUpUpdate(UInt32 timeup);
	void CancelHawkTimeUpUpdate();

	void StopHawk();
	void StartOfflineCount();
	void StopOfflineCount();

	virtual RPCResult CloseHawkForUser(LPCPEER pPeer, RPCContext &context, const UInt32 &charID);
	virtual RPCResult ChangeHawkName(LPCPEER pPeer, RPCContext &context, const String &hawkName);
	virtual RPCResult BuyHawkItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemStack);
	virtual RPCResult StopHawk(LPCPEER pPeer, RPCContext &context);

	void AddStartHawkItemInfo(HawkerItemInfo itemInfo,StartHawkAddType addType);

	// Event & State
	virtual void HandleEvent(CONST Event &event);

	enum EventEnum
	{
		EVT_UPDATE_NAME	= MAKE_EVENTID(HawkGroup_CLASSID, 1),	// update object
		EVT_UPDATE_TIMEUP	= MAKE_EVENTID(HawkGroup_CLASSID, 2),	// update object
	};
	
};

INLINE CHawkGroup* CHawkGroup::NewInstance()
{
	return CreateNetObject<CHawkGroup>();
}

INLINE void CHawkGroup::RaiseHawkNameUpdate()
{
	RaiseUniqEventID(EVT_UPDATE_NAME);
}

INLINE void CHawkGroup::CancelHawkNameUpdate()
{
	CancelUniqEvent(EVT_UPDATE_NAME);
}

INLINE void CHawkGroup::RaiseHawkTimeUpUpdate(UInt32 timeup)
{
	RaiseUniqEventID(EVT_UPDATE_TIMEUP, timeup);
}

INLINE void CHawkGroup::CancelHawkTimeUpUpdate()
{
	CancelUniqEvent(EVT_UPDATE_TIMEUP);
}

#endif //_HAWKGROUP_H_

