#pragma once
#ifndef _MENUONLINESHOP_H_
#define _MENUONLINESHOP_H_

class CChargeShop;
class CChargeShopItem;

enum ChargeShopPreviewType
{
	PREVIEW_NONE = 0,
	PREVIEW_PARTNER,
	PREVIEW_PET_FOLLOW,
	PREVIEW_PET_MOUNT,
	PREVIEW_CLOTH,
};


class CMenuOnlineShopProcess
{
private:
	UInt8 m_iShowIndex;
	UInt8 m_iPage;
	int m_iSelItemIndex;
	int m_iSelLimitedItemIndex;
	int m_iSelDiscountItemIndex;
	CChargeShop* m_pChargeShop;

	typedef StlVector<CChargeShopItem*, ManagedAllocator<CChargeShopItem* > > ChargeShopCategory;

	ChargeShopCategory m_hotSaleItem;
	ChargeShopCategory m_mixItem;
	ChargeShopCategory m_asscItem;
	ChargeShopCategory m_petItem;
	ChargeShopCategory m_DiamondItem;
	ChargeShopCategory m_BookItem;
	ChargeShopCategory m_FireWorkItem;
	ChargeShopCategory m_CapItem;
	ChargeShopCategory m_MouldItem;
	ChargeShopCategory m_partnerItem;
	ChargeShopCategory m_clothItem;
	ChargeShopCategory m_otherItem;
	ChargeShopCategory m_limitedItem;
	ChargeShopCategory m_discountItem;

	CChargeShopItem* m_pSelectedItem;
	UInt16 m_iStack;
	UInt32 m_iUpdateYuanTimer;

public:
	
	CMenuOnlineShopProcess();
	~CMenuOnlineShopProcess();
	void ShowHotSale(UInt8 pageNo);
	void ShowMix(UInt8 pageNo);
	void ShowAssc(UInt8 pageNo);
	void ShowPet(UInt8 pageNo);
	void ShowDiamond(UInt8 pageNo);
	void ShowBook(UInt8 pageNo);
	void ShowCap(UInt8 pageNo);
	void ShowMould(UInt8 pageNo);
	void ShowFireworks(UInt8 pageNo);
	void ShowPartner(UInt8 pageNo);
	void ShowCloth(UInt8 pageNo);
	void ShowOther(UInt8 pageNo);
	void ShowLimitedItem();
	void ShowDiscountItem();
	UInt8 GetShowIndex();
	UInt8 GetPageNo();
	void UpdateYuanBao(bool fromServer = false);
	void OpenOnlineShop();
	int GetSelectedIndex();
	int GetSelectedDiscountIndex();
	int GetSelectedLimitedIndex();
	void ResetSelectedIndex();
	void ResetSelectedDiscountIndex();
	void ResetSelectedLimitedIndex();
	void SetSelectedIndex(int sel);
	void SetSelectedDiscountIndex(int sel);
	void SetSelectedLimitedIndex(int sel);

	void SetChargeShop(CChargeShop* shop);
	void AskForOnlineShop();
	void CloseOnlineShop(bool removeFromWorld = true);
	void ShowItem(ChargeShopCategory itemList, UInt8 pageNo);

	void NextPage();
	void PrevPage();

	void ShowItemDetail();
	void ShowDiscountItemDetail();
	void ShowLimitedItemDetail();

	void ShowOnlineShopItemDetail(CChargeShopItem* item);

	int GetSelectedItemType();
	UInt32 GetSelectedItemPrice(UInt16 stack);
	UInt16 GetMaxSelectedItemStack();

	CChargeShopItem* GetSelectedItem();
	void SetBuyItemStack(UInt16 stack);
	void ConfirmBuyItem();

	UInt8 GetSelectedItemOverLap();
	void Preview();

	void PreviewRotate(bool left);
	void ShowLackMoneyMessage();

	void OpenChargeShopWebSite(int index);
	void ResetUpdateTimer(){ m_iUpdateYuanTimer = 0; }
	Color32 GetRareColor(UInt rare);

};


#endif 