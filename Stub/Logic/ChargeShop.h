#pragma once
#ifndef _CHARGESHOP_H_
#define _CHARGESHOP_H_

#include "Stub/Logic/ChargeShop_Stub.h"

struct ChargeShopData;
class CChargeShopItem;
class CCharacter;
class CChargeShopData;


class CChargeShop : public ChargeShop_Stub,
					public Reuben::Simulation::IEventHandler<CChargeShop>
{
public:
	DEFINE_CLASS(ChargeShop_CLASSID);
	CChargeShop();
	virtual ~CChargeShop();
	INLINE static CChargeShop* NewInstance();
	void PreSend();
	//void BuyItem(CCharacter* pChar, UInt8 chargeShopType, UInt32 chargeShopID, UInt16 stack, UInt32 itemID);

	void InitChargeShop();
	RPCResult InitChargeShop(RPCContext &context);


	NetGroup GetDiscountItem();

	time_t ConvertStringToTime(String timeStr);
	
	CChargeShopItem* GetLimitedItem();
	void AddCharID(UInt32 charID, RPCContext context);
	void RemoveCharID(UInt32 charID);

	virtual RPCResult BuyItemInChargeShop(LPCPEER pPeer, RPCContext &context, const UInt32 &chargeShopID, const UInt16 &stack);
	void UpdateLimitedItem(CChargeShopData* pData);

	//void BuyItem(UInt32 charID, UInt32 chargeShopID, UInt16 stack, RPCContext &context);
	//RPCResult BuyItem(RPCContext &context);

	RPCResult UpdateToObject(RPCContext &context);	// update chargeShopItem to object service
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event

	// Event & State
	virtual void HandleEvent(CONST Event &event);

	enum EventEnum
	{
		EVT_UPDATE_OBJECT = MAKE_EVENTID(ChargeShop_CLASSID, 1),	// update object
	};

private:
	typedef ChargeShop_Stub Parent;
	
	typedef StlMap<UInt32, CChargeShopItem*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CChargeShopItem*> > > ChargeShopCategory;
	typedef StlMap<UInt32, RPCContext, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, RPCContext> > > CharContextList;
	
	ChargeShopCategory m_AllItem;
	ChargeShopCategory m_limitedItem;
	ChargeShopCategory m_discountItem;

	CharContextList m_charIDList;

	void BuyItem(CCharacter* pChar, CChargeShopItem* item, UInt16 stack);
	UInt8 CheckCharCanBuy(CCharacter* pChar, CChargeShopItem* item, UInt16 stack, bool needReserve = false);
	void ReleaseReservedItem(CCharacter* pChar, CChargeShopItem* item, UInt16 stack);
};

INLINE CChargeShop* CChargeShop::NewInstance()
{
	return CreateNetObject<CChargeShop>();
}

INLINE void CChargeShop::RaiseUpdate()
{
	RaiseUniqEventID(EVT_UPDATE_OBJECT);
}

INLINE void CChargeShop::CancelUpdate()
{
	CancelUniqEvent(EVT_UPDATE_OBJECT);
}

#endif //_CHARGESHOP_H_
