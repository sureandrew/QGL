#pragma once
#ifndef _MENUSHOPPROCESS_H_
#define _MENUSHOPPROCESS_H_

#include "Common.h"
#include "Common/common_binary.h"

class CMenuShopProcess
{
private:
	enum					_ShopOperationMode
	{
							SHOP_NORMAL = 0,
							SHOP_BUY,
							SHOP_SELL,
	};

	enum					_ShopBuyResult
	{
							SHOP_BUY_SUCCESS = 0,
							SHOP_BUY_NOMONEY,
							SHOP_BUY_NOSPACE,
							SHOP_BUY_NOITEM,
							SHOP_BUY_COUNTERR,
							SHOP_BUY_FAILED,
	};


	UInt32Vector			m_ItemLst;
	UInt16					m_BuyDiscount;
	UInt16					m_SellDiscount;
	UInt16					m_RepairDiscount;
	Int32					m_nShopId;
	Boolean					m_bSellEnable;
	Boolean					m_bRepairEnable;
	UInt16					m_StartX;
	UInt16					m_StartY;

	Int						m_nMode;	// 0: Nothing, 1: Buy, 2: Sell
	Int						m_SellPos;
	Int						m_BuyPos;
	Int						m_SellCount;
	Int						m_BuyCount;
	// business item
	BusinessShopItemInfoVector			m_BusinessShopItemLst;	
	UInt32Vector			m_BusinessItemLst;
	Boolean					m_bBusinessSell;// whether can sell business item to shop
	Boolean					m_bBusinessItemMode;
	UInt32					m_NextBusinessItemShopSynTime;
	UInt32					m_BusinessMoney;
public:
	CMenuShopProcess();
	virtual ~CMenuShopProcess();
private:
	VOID Init();
public:
	VOID ResetParam();
public:
	// Main Menu Of Shop Operation
	UInt32 GetItemId(const UInt8 SlotIndex);
	
	Boolean HaveEnoughMoney(UInt32 eachPrice, Int nCount);
	Boolean IsEnoughFreeSlot(Int nCount, Int nMaxOverlap);
	Int GetTotalFreeSlot();
	Int CanBuy(UInt32 ItemId, Int nCount = 1, Boolean bShowErrMsg = BOOLEAN_FALSE);
	INLINE Boolean CanSell() { return m_bSellEnable; };
	INLINE Boolean CanRepair() { return m_bRepairEnable; };
	INLINE UInt16 GetBuyDiscount() { return m_BuyDiscount; };
	INLINE UInt16 GetSellDiscount() { return m_SellDiscount; };

	VOID ChangeTitle(UInt8 Title);
	VOID SetButtonDisable(UInt8 Enable);
	VOID SetDiscount(UInt16 buy, UInt16 sell, UInt16 repair);
	VOID SetStartUpPos();

	VOID OpenShopMenu(Int32 ShopId, UInt8 Title, UInt8 Enable, UInt16 Buy, UInt16 Sell, UInt16 Repair, UInt16 uiShopType, UInt32Vector Lst);
	VOID CloseShopMenu();

	VOID ConfirmAction();

	VOID DragItem(Int Pos);

	VOID OnClickSellButton();
	VOID OnLButtonUpForSell();
	VOID OnRButtonUpForCancelSell();

	VOID OnDisappear();

	// Sub Menu of Shop(Confirm Box) Operation
	Boolean ResetConfirmBoxUI(UInt32 ItemId);
	VOID OpenConfirmBox(Int Pos, Int Mode);

	// Sub Menu of shop(Adjust Count Box) Operation
	Boolean ResetCountBoxUI(UInt32 ItemId);
	VOID OpenAdjustCountBox(bool bSell, Int nPos);
	VOID SetMaxNum(bool bMax);
	VOID IncCount(bool bInc);
	VOID SetPrice(UInt32 price);
	VOID SetEditNum(Int Num, Boolean bUpdate = BOOLEAN_TRUE);
	VOID OnEditBoxKeyUp(Int keyCode);
	Boolean OnSellClickInValid();
	VOID UpdatePrice();
	// Business Shop
	VOID OpenBusinessShopMenu(Boolean sell, UInt16 shop_id,	UInt8 shopName, 
		const UInt32 &SynTime, const UInt32 &BusinessMoney, const BusinessShopItemInfoVector &ItemInfoList);
	VOID UpdateBusinessShopMenu(const BusinessShopItemInfoVector &ItemInfoList);
	UInt32 GetBuyPrice(UInt32 item_id);
	UInt32 GetSellPrice(UInt32 item_id);
	void UpdateBusinessShopItem(UInt32 CurrentTime);
	void ChangeBusinessShopMoney(Int32 nChange);
};


#endif // _MENUSHOPPROCESS_H_