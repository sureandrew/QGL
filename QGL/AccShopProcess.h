#ifndef _ACC_SHOP_PROCESS_H_
#define _ACC_SHOP_PROCESS_H_

const int ACCSHOPMAXITEMPERPAGE = 10;
class CAccShopProcess
{
public:
	CAccShopProcess();
	~CAccShopProcess();
public:
	void ShowAccShop();
	void SelectAccShopItem(int index);
	void AccShopPrevPage();
	void AccShopNextPage();
	void AccShopBuyItem();
	void OnMouseMoveOver(UInt32 uiIndex);
private:
	UInt32 m_ui32StarIndex;
	UInt32 m_uiSelIndex;
	UInt32 m_uiTotalRecord;
	UInt32 m_uiCurPage;
};

#endif