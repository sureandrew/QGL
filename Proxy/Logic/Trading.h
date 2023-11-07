#pragma once
#ifndef _TRADING_H_
#define _TRADING_H_

#include "Proxy/Logic/Trading_Proxy.h"

class CPartner;
class CItem;
class CPet;

class CTrading : public Trading_Proxy
{
private:
	typedef Trading_Proxy Parent;
	typedef StlMap<UInt8,  CPartner *, std::less<UInt8>, ManagedAllocator<std::pair<UInt8, CPartner*> > > TargetPartnerList;
	typedef StlMap<UInt8,  UInt16Vector, std::less<UInt8>, ManagedAllocator<std::pair<UInt8, UInt16Vector> > > TargetSkillList;
	TargetPartnerList m_targetPartnerList;
	TargetSkillList m_targetPartnerSkillList;
	typedef StlMap<UInt8,  CItem *, std::less<UInt8>, ManagedAllocator<std::pair<UInt8, CItem*> > > TargetItemList;
	TargetItemList m_targetItemList;
	typedef StlMap<UInt8,  CPet *, std::less<UInt8>, ManagedAllocator<std::pair<UInt8, CPet*> > > TargetPetList;
	TargetPetList m_targetPetList;
	TargetSkillList m_targetPetSkillList;
	
	//StlVector< CPet *, ManagedAllocator<CPet*> > m_targetPetList;
	Boolean m_bTargetLock;
	Boolean m_bTargetConfirm;
public:
	DEFINE_CLASS(Trading_CLASSID);
	virtual RPCResult CB_SetItemSlot(LPCPEER pPeer, RPCContext &context, NetObject* item, const UInt8 &position);
	virtual RPCResult CB_SetPartnerSlot(LPCPEER pPeer, RPCContext &context, NetObject* partner, const UInt16Vector &skillID, const UInt8 &position);
	virtual RPCResult CB_SetPetSlot(LPCPEER pPeer, RPCContext &context, NetObject* pet, const UInt16Vector &skillID, const UInt8 &position);
	virtual RPCResult CB_SetLock(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Boolean &lock);
	virtual RPCResult CB_SetMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney);
	virtual RPCResult CB_CancelTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id);
	virtual RPCResult CB_RemoveItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position);
	virtual RPCResult CB_RemovePartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position);
	virtual RPCResult CB_RemovePetSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position);
	virtual RPCResult CB_ConfirmTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id);
	virtual RPCResult CB_TradingProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const String &message, const UInt32 &errorCharID);
	virtual RPCResult CB_TradingItem(LPCPEER pPeer, RPCContext &context, const UInt32Vector &itemID, const UInt8Vector &itemStacks);
	virtual RPCResult CB_TradingPartner(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerID);
	virtual RPCResult CB_TradingPet(LPCPEER pPeer, RPCContext &context, const UInt32Vector &petID);
	virtual RPCResult CB_TradingMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney);

	void ResetAllData();
	CTrading()
	{
		m_bTargetLock = FALSE;
		m_bTargetConfirm = FALSE;
	}
	virtual ~CTrading();
	Boolean IsTargetLocked(){return m_bTargetLock;}
	Boolean IsTargetConfirmed(){return m_bTargetConfirm;}
	CItem* GetTargetItem(UInt8 index);
	CPartner* GetTargetPartner(UInt8 index);
	UInt16Vector GetTargetPartnerSkill(UInt8 index);
	CPet* GetTargetPet(UInt8 index);
	UInt16Vector GetTargetPetSkill(UInt8 index);

// OnUpdate can be implemented selectively
};

#endif //_TRADING_H_
