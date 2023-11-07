#pragma once
#ifndef _MENUTRADING_H_
#define _MENUTRADING_H_

#include <Reuben\Network\Network.h>

struct TradingItemInfo
{
	UInt32 item_uid;
	UInt32 item_id;
	UInt8 item_stack;
	UInt8 item_level;
	UInt8 itemIndex_inbag;

	TradingItemInfo()
	{
		item_uid = 0;
		item_id = 0;
		item_stack = 0;
		item_level = 0;
		itemIndex_inbag = 0;
	}
};

typedef StlVector<TradingItemInfo, ManagedAllocator<TradingItemInfo> > TradingItemList;
typedef StlVector<String, ManagedAllocator<String> > TradingNameVector;

#define TRADINGITEM_MAX		6
#define TRADINGPARTNER_MAX	3
#define TRADINGPET_MAX		3


class CMenuTradingProcess
{
private:
	RPCNetID m_targetNetID;
	String m_targetName;
	UInt32 m_iTargetCharID;
	TradingItemList m_itemList;
	UInt32Vector m_partnerList;
	UInt32Vector m_petList;
	bool m_bConfirmTrading;
	bool m_bLockTrading;
	TradingNameVector m_partnerName;
	TradingNameVector m_petName;
	UInt32 m_iMoney;

public:
	CMenuTradingProcess();
	~CMenuTradingProcess();

	void SetTargetNetID(RPCNetID targetNetID);
	INLINE RPCNetID GetTargetNetID(){ return m_targetNetID;}
	INLINE void ResetTargetNetID(){
		m_targetNetID = RPCNetID(); 
		m_targetName = _T("");
		m_iTargetCharID = 0;
	}

	INLINE String GetTargetName() {return m_targetName;}
	INLINE UInt32 GetTargetCharID(){ return m_iTargetCharID;}
	void OpenRequestTradingBox();
	bool hasTarget();
	void ConfirmReqTrading();
	void RejectReqTrading();

	void SetItem(int tradingIndex, int itemSlot, int stack);
	void RemoveItem(int tradingIndex);
	void SetPartner(int tradingIndex, UInt32 partnerID);
	void RemovePartner(int tradingIndex);
	void SetPet(int tradingIndex, UInt32 petID);
	void RemovePet(int tradingIndex);

	void DisplayTargetItemSlot(UInt32 itemID, UInt8 stack, UInt8 position);
	void DisplayTargetPartnerSlot(UInt32 mobID, UInt8 position);
	void DisplayTargetPetSlot(UInt32 petID, UInt8 position);
	void DisplayTargetMoney(UInt32 gmoney);

	void ResetTradingProcess();
	void CancelTrading();

	void SetLock(bool lock);
	INLINE void ResetLock(){m_bLockTrading = false;}

	void OpenTradingPanel();
	void SetMoney();

	void CheckMoney();

	void ConfirmTrading();
	void ResetItemPartnerPet();

	bool IsLocked(){return m_bLockTrading;}
	bool IsConfirm();

	void ShowItemDetail();
	void ShowPartnerDetail();
	void ShowPetDetail();

	void ShowTradingResultText(); //what thing you have given to other

	void ShowMoneyColour();	

};






#endif 
