#pragma once
#ifndef _TRADING_H_
#define _TRADING_H_

#include "Stub/Logic/Trading_Stub.h"

class CCharacter;

class CTrading : public Trading_Stub
{
private:
	typedef Trading_Stub Parent;

	enum ITEMBAGSTATE
	{
		NO_ITEM_INDEX = 255,
	};

	struct ItemSlot
	{
		UInt8 itemBagIndex;
		UInt8 stack;

		ItemSlot()
		{
			itemBagIndex = NO_ITEM_INDEX; //255 : no item
			stack = 0;
		}
	};

	typedef StlVector< ItemSlot, ManagedAllocator< ItemSlot> > ItemSlotVector;
	ItemSlotVector m_char1ItemVector;
	ItemSlotVector m_char2ItemVector;

	UInt32Vector m_char1PartnerVector;
	UInt32Vector m_char2PartnerVector;

	UInt32Vector m_char1PetVector;
	UInt32Vector m_char2PetVector;

	UInt32 m_char1Gmoney;
	UInt32 m_char2Gmoney;

	Boolean m_bChar1Lock;
	Boolean m_bChar2Lock;

	Boolean m_bChar1Confirm;
	Boolean m_bChar2Confirm;

	RPCNetID m_CharNetID1;
	RPCNetID m_CharNetID2;
	

public:
	DEFINE_CLASS(Trading_CLASSID);
	CTrading();
	virtual ~CTrading();

	INLINE static CTrading* NewInstance();

	void SetCharacterNetID1(RPCNetID charNetID){ m_CharNetID1 = charNetID; }
	void SetCharacterNetID2(RPCNetID charNetID){ m_CharNetID2 = charNetID; }

	void DeleteTrading();
	void StopTrading(UInt32 owner_id = 0);
	bool IsAllCharReady(){ return (m_CharNetID1 != RPCNetID() && m_CharNetID2 != RPCNetID() ); }

	virtual RPCResult SetTradingItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &itemBagIndex, const UInt8 &stack, const UInt8 &tradingPosition);
	virtual RPCResult RemoveTradingItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &tradingPosition);
	virtual RPCResult SetTradingPartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID, const UInt8 &position);
	virtual RPCResult RemoveTradingPartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position);
	virtual RPCResult SetTradingPetSlot(LPCPEER pPeer, RPCContext &context, const UInt32 &petID, const UInt8 &position);
	virtual RPCResult RemoveTradingPetSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position);
	virtual RPCResult SetTradingLock(LPCPEER pPeer, RPCContext &context, const Boolean &lock);
	virtual RPCResult SetTradingMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney);
	virtual RPCResult CancelTrading(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ConfirmTrading(LPCPEER pPeer, RPCContext &context);

	void TradingItem(CCharacter* source, CCharacter* target, ItemSlotVector sourceItemVector);
	void TradingPartner(CCharacter* source, CCharacter* target, UInt32Vector sourcePartnerVector);
	void TradingPet(CCharacter* source, CCharacter* target, UInt32Vector sourcePetVector);

	void ResetLock();


};

INLINE CTrading* CTrading::NewInstance()
{
	return CreateNetObject<CTrading>();
}

#endif //_TRADING_H_
