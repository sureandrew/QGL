#ifndef _POINTSSHOPPROCESS_H_
#define _POINTSSHOPPROCESS_H_

const int POINTSSHOPMAXITEMPERPAGE = 10;
class CPointsShopProcess
{
public:
	CPointsShopProcess();
	~CPointsShopProcess();
public:
	void SetData(UInt32 uiPointsShopID,UInt32 uiPointsShopTypeID, UInt32Vector& Lst);
	void ShowPointsShop();
	void SelectPointsShopItem(int index);
	void PointsShopPrevPage();
	void PointsShopNextPage();
	void PointsShopBuyItem();
	void OnMouseMoveOver(UInt32 uiIndex);
	void SetTotalPoints(UInt32 uiPoints);
	UInt32 GetTotalPoints() const {return m_uiTotalPoints;}
	void SetItemLst(UInt32Vector& Lst){m_itemLst = Lst;}
	UInt32Vector& GetItemLst(){return m_itemLst;}

private:
	UInt32 m_uiPointsShopID;
	UInt32 m_uiPointsShopTypeID;
	UInt32 m_ui32StarIndex;
	UInt32 m_uiSelIndex;
	UInt32 m_uiTotalRecord;
	UInt32 m_uiCurPage;
	UInt32 m_uiTotalPoints;
	UInt32Vector m_itemLst;
};

#endif