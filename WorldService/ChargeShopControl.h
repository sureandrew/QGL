#pragma once
#ifndef _CHARGESHOP_CONTROL_H_
#define _CHARGESHOP_CONTROL_H_

class CChargeShopData;

#define CChargeShopControl_CLASSID 41900

class CChargeShopControl : 
	public Reuben::Simulation::Object
	, public Reuben::Simulation::IEventHandler<CChargeShopControl>
{
public:
	
	DEFINE_CLASS(CChargeShopControl_CLASSID);	
	DEFINE_PARENT_CLASS(Reuben::Simulation::Object);

	CChargeShopControl();
	virtual ~CChargeShopControl();
	void InitChargeShop();
	RPCResult InitChargeShop(RPCContext &context);
	time_t ConvertStringToTime(String timeStr);

	UInt8 CanBuyLimitedItem(UInt32 chargeShopID, UInt16 stack, UInt32 char_id);
	void ReduceLimitItem(UInt32 chargeShopID, Boolean buySuccess, UInt16 reduceStack, UInt32 char_id);
	void UpdateLimitedItem();

	virtual void HandleEvent(CONST Event &event);

	enum EventEnum
	{
		EVT_UPDATE_LIMITEDITEM	= MAKE_EVENTID(CChargeShopControl_CLASSID, 1),	// update object
		EVT_UPDATE_CLEAR_BOUGHT_LIMITED_CHAR_ID	= MAKE_EVENTID(CChargeShopControl_CLASSID, 2),	// update object
	};

private:
	typedef StlMap<UInt32, CChargeShopData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CChargeShopData*> > > LimitItemList;

	LimitItemList m_limitItemList;
	UInt32Vector m_boughtLimitedCharID;
};


#endif //_CHARGESHOP_CONTROL_H_