#pragma once
#ifndef _MENUSELLYBPROCESS_H_
#define _MENUSELLYBPROCESS_H_

class CMenuSellYuanBaoProcess
{
private:
	UInt m_iMenuIndex;
	UInt m_iStartX;
	UInt m_iStartY;
	UInt m_iPage;
	UInt m_iSelectedSellRecordID;

public:
	enum SellYBMenuState
	{
		SELLYBMENU_MAINPAGE = 0,
		SELLYBMENU_CHANGEYBSELECT,
		SELLYBMENU_YBTOYBT,
		SELLYBMENU_YBTTOYB,
		SELLYBMENU_SELL,
		SELLYBMENU_BUY,		
		SELLYBMENU_TEMPBANK,
	};
	
	CMenuSellYuanBaoProcess();
	~CMenuSellYuanBaoProcess();

	void OpenSellYBMenuChoice();
	void OpenChangeYBSelect();
	void OpenExchangeYBYBT(int type); //0:YBTOYBT 1:YBTOYB
	void UpdateOwnerYBInfo();
	
	void CloseSellYuanBao();

	void CheckYBTAmount();
	void ConfirmChangeYBYBT(int amount);

	void DisppearSellYB();
	void InitStartPos();

	void OpenSellYBBuySell(int type); //0: SELL 1:BUY
	void UpdateSellYBRecord(int page = -1);

	void PageUpAndDown(bool up);

	void ConfirmSellYB(int amount, int pricePerUnit);

	void SetConfirmBoxText(int YBT, int pricePerUnit);
	void SelectBuyYBT(int index);
	void ConfirmBuyYB();

	bool IsSellYuanBaoMenu() { return (m_iMenuIndex == SELLYBMENU_SELL);}

	void DisplayBuyYBConfirmText();
	void ResetSelectBuyYBT();

	UInt GetSelectedRecordID() {  return m_iSelectedSellRecordID; }
	void GetSellYBTempBank();
	
	void OpenTempBank(UInt32 YBT, UInt32 money);
	void GetAllTBMoneyYBT();

	void UpdateSellYuanBaoRule();

	void SetEditBoxColor(PCSTRINGA staticTextName, UInt32 value);
};

#endif //_MENUSELLYBPROCESS_H_
