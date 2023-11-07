#pragma once
#ifndef _CHARGESHOPITEM_H_
#define _CHARGESHOPITEM_H_

#include "Stub/Logic/ChargeShopItem_Stub.h"

class CChargeShopData;

class CChargeShopItem : public ChargeShopItem_Stub//,
						//public Reuben::Simulation::IEventHandler<CChargeShopItem>
{
private:
	typedef ChargeShopItem_Stub Parent;
	UInt m_iPreBuy;
public:
	DEFINE_CLASS(ChargeShopItem_CLASSID);

	CChargeShopItem()
	{
		m_iPreBuy = 0;
	}
	virtual ~CChargeShopItem(){}
	void PreSend();
	INLINE static CChargeShopItem* NewInstance();
	void SetData(CChargeShopData* pChargeShopData, bool bClear=true);
	void WriteData(CChargeShopData* pChargeShopData);
	/*RPCResult UpdateToObject(RPCContext &context);	// update chargeShopItem to object service
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event*/
	Boolean NeedUpdate();
	Boolean IsOnSaleDay();
	//void Restore();
	void SetPreBuy(UInt preBuy){m_iPreBuy = preBuy;}
	UInt GetPreBuy(){return m_iPreBuy;}


	// Event & State
	/*virtual void HandleEvent(CONST Event &event);

	enum EventEnum
	{
		EVT_UPDATE_OBJECT	= MAKE_EVENTID(ChargeShopItem_CLASSID, 1),	// update object
	};*/
};

INLINE CChargeShopItem* CChargeShopItem::NewInstance()
{
	return CreateNetObject<CChargeShopItem>();
}


/*INLINE void CChargeShopItem::RaiseUpdate()
{
	RaiseUniqEventID(EVT_UPDATE_OBJECT);
}

INLINE void CChargeShopItem::CancelUpdate()
{
	CancelUniqEvent(EVT_UPDATE_OBJECT);
}*/

#endif //_CHARGESHOPITEM_H_
