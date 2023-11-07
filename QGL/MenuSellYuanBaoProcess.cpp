//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuSellYuanBaoProcess.h"
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "QGL/CharacterControl.h"
#include "Math.h"
#include "Proxy/Logic/SellYuanBaoGroup.h"
#include "Proxy/Logic/SellYuanBao.h"

#define MAX_SELL_YB_RECORD_PER_PAGE 8
#define YB_TO_YBT_RATIO 1000 // 1000 YB = 1 YBT
#define SELL_YB_TAX 10000
#define MAX_SELL_YBT 99

CMenuSellYuanBaoProcess::CMenuSellYuanBaoProcess()
{
	m_iMenuIndex = SELLYBMENU_MAINPAGE;
	m_iStartX = 0;
	m_iStartY = 0;
	m_iSelectedSellRecordID = 0;
}

CMenuSellYuanBaoProcess::~CMenuSellYuanBaoProcess()
{

}

void CMenuSellYuanBaoProcess::InitStartPos()
{
	if (gGlobal.g_pCharacter)
	{
		m_iStartX = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
		m_iStartY = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;
	}
}

void CMenuSellYuanBaoProcess::OpenSellYBMenuChoice()
{	
	m_iMenuIndex = SELLYBMENU_MAINPAGE;
	UpdateOwnerYBInfo();	
	UpdateSellYuanBaoRule();
}

void CMenuSellYuanBaoProcess::OpenChangeYBSelect()
{
	m_iMenuIndex = SELLYBMENU_CHANGEYBSELECT;
	UpdateSellYuanBaoRule();
}
	
void CMenuSellYuanBaoProcess::OpenExchangeYBYBT(int type) //0:YBTOYBT 1:YBTOYB
{
	if (type == 0 )
		m_iMenuIndex = SELLYBMENU_YBTOYBT;
	else if (type == 1)
		m_iMenuIndex = SELLYBMENU_YBTTOYB;
	UpdateSellYuanBaoRule();
}

void CMenuSellYuanBaoProcess::UpdateOwnerYBInfo()
{
	UIChatPanel* pChat = NULL;
	UInt formatStyle = 0;
	switch (m_iMenuIndex)
	{
	case SELLYBMENU_MAINPAGE:
	case SELLYBMENU_CHANGEYBSELECT:
	case SELLYBMENU_YBTOYBT:
	case SELLYBMENU_YBTTOYB:
		formatStyle = 1;
		pChat = gGlobal.pUIManager->FindUIChatPanel("SellYBChoiceOwnInfo");
		break;
	case SELLYBMENU_SELL:
	case SELLYBMENU_BUY:
		formatStyle = 2;
		pChat = gGlobal.pUIManager->FindUIChatPanel("SellYBBuySellOwnInfo");
		break;
	}
	
	if (pChat && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	{
		String tabSpace;
		String msg;
		pChat->ClearAllRow();

		switch (formatStyle)
		{
		case 1:
			tabSpace = _T("\\T99:");
			break;
		case 2:
			tabSpace = _T("\\T44:");
			break;
		}

		msg = gGlobal.GetStringTable()->Get(_T("MSG_CHAR_OWN"));
		pChat->AddStringW(msg.c_str());
		msg.Format(_T("%s%s\\c%d %d"), gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT3")),
			tabSpace.c_str(), GetFontColorIndexByMoney(gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBaoTicket()), 
			gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBaoTicket());
		pChat->AddStringW(msg.c_str());
		msg.Format(_T("%s%s\\c%d %d"), gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT4")),
			tabSpace.c_str(), GetFontColorIndexByMoney(gGlobal.g_pCharacter->GetCharNetObject()->GetMoney()),
			gGlobal.g_pCharacter->GetCharNetObject()->GetMoney());
		pChat->AddStringW(msg.c_str());
		msg.Format(_T("%s%s\\c%d %d"), gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT1")),
			tabSpace.c_str(), GetFontColorIndexByMoney(gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBao()), 
			gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBao());
		pChat->AddStringW(msg.c_str());		
		pChat->EndRow();
		pChat->Redraw();
		pChat->SetEnable(true);
		gGlobal.pUIManager->AddDirtyUIItem(pChat);
	}
}

void CMenuSellYuanBaoProcess::CloseSellYuanBao()
{
	if (gGlobal.g_pSellYuanBaoGroup)
	{
		gGlobal.g_pSellYuanBaoGroup->CloseSellYuanBao(gGlobal.pPeerProxy, gGlobal.g_context);
		DeleteNetGroup(gGlobal.g_pSellYuanBaoGroup, BOOLEAN_TRUE);
	}
	gGlobal.g_pSellYuanBaoGroup = NULL;

	m_iStartX = 0;
	m_iStartY = 0;
	m_iSelectedSellRecordID = 0;
}

void CMenuSellYuanBaoProcess::CheckYBTAmount()
{
	if (m_iMenuIndex == SELLYBMENU_SELL)
	{
		UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SellYBSellAmount");
		if (pEditBox && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() )
		{
			UInt inputValue = pEditBox->GetInt();
			if (inputValue > MAX_SELL_YBT)
				inputValue = MAX_SELL_YBT;
			if (inputValue > gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBaoTicket())
				inputValue = gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBaoTicket();

			if (inputValue != pEditBox->GetInt())
			{
				String text;
				text.Format(_T("%d"), inputValue);
				pEditBox->SetText(text.c_str());
				pEditBox->Redraw();
				gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
			}
		}
	}
	else
	{
		//1 YBT = 1000 YB
		UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("NumberOfSellYBChoiceChangeAmount");
		UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText("SellYBChoiceTotalAmount");

		if (pEditBox && pText && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
		{
			UInt inputYBValue = pEditBox->GetInt();
			String text;
			if (m_iMenuIndex == SELLYBMENU_YBTOYBT)
			{
				inputYBValue *= YB_TO_YBT_RATIO;
				if (inputYBValue > gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBao())
				{			
					UInt maxYBT = gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBao() / YB_TO_YBT_RATIO ;
					inputYBValue = maxYBT * YB_TO_YBT_RATIO;			
					text.Format(_T("%d"), maxYBT);
					pEditBox->SetText(text.c_str());
					pEditBox->Redraw();
					gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
				}
			}
			else if (m_iMenuIndex == SELLYBMENU_YBTTOYB)
			{
				if (inputYBValue > gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBaoTicket())
				{				
					inputYBValue = gGlobal.g_pCharacter->GetCharNetObject()->GetYuanBaoTicket() * YB_TO_YBT_RATIO;
					text.Format(_T("%d"), inputYBValue / YB_TO_YBT_RATIO);
					pEditBox->SetText(text.c_str());
					pEditBox->Redraw();
					gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
				}
				else
					inputYBValue *= YB_TO_YBT_RATIO;
			}

			text.Format(_T("%d"), inputYBValue);
			pText->SetText(text.c_str());
			pText->SetColor(gGlobal.GetDisplayMoneyColor(inputYBValue));
			pText->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(pText);
		}
	}
}

void CMenuSellYuanBaoProcess::ConfirmChangeYBYBT(int amount)
{
	UIEditBox* passwordBox = gGlobal.pUIManager->FindUIEditBox("SellYBChoiceConfrimTextPassword");
	if (amount > 0 && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() && passwordBox)
	{
		if (passwordBox->GetTextSize() > 0)
		{
			UInt type = CHANGEYBT_YB_TO_YBT;
			if (m_iMenuIndex == SELLYBMENU_YBTTOYB)
				type = CHANGEYBT_YBT_TO_YB;
			gGlobal.g_pCharacter->Stop();
			gGlobal.g_pCharacter->GetCharNetObject()->ChangeBetweenYBandYBT(gGlobal.pPeerProxy, gGlobal.g_context, type, amount, passwordBox->GetHashedPassword());
			gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_SYS_EXCHANGEING")), false);
			passwordBox->ClearText();
			passwordBox->Redraw();
		}
		else
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_PLEASE_ENTER_PASSWORD")));
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PLEASE_ENTER_PASSWORD")), CHANNELTYPE_SYSTEM);
		}
	}
}

void CMenuSellYuanBaoProcess::DisppearSellYB()
{
	if (m_iStartX > 0 || m_iStartY > 0)
	{
		BlockPos currPos = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
		BlockPos center;
		center.PosX = m_iStartX;
		center.PosY = m_iStartY;
		PixelDirection dummy;

		if (QGLMath::CalculateBlockPosDistance(currPos, center, dummy) > 400)
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseSellYuanBaoChoice", String());
		}
	}
}

void CMenuSellYuanBaoProcess::OpenSellYBBuySell(int type)
{
	if (type == 0)
		m_iMenuIndex = SELLYBMENU_SELL;
	else 
		m_iMenuIndex = SELLYBMENU_BUY;

	UpdateSellYBRecord(0);
	UpdateOwnerYBInfo();
	UpdateSellYuanBaoRule();
}

void CMenuSellYuanBaoProcess::UpdateSellYBRecord(int page)
{
	if (gGlobal.g_pSellYuanBaoGroup == NULL)
		return;

	UIStaticText* pText1 = NULL;
	UIStaticText* pText2 = NULL;
	UIStaticText* pText3 = NULL;
	UIStaticText* pText4 = NULL;

	UIImage* pIm1 = NULL;
	UIImage* pIm2 = NULL;
	UIImage* pIm3 = NULL;
	UIImage* pIm4 = NULL;

	if (page >= 0)
		m_iPage = page;

	StringA iconName;

	for (Index i = C_INDEX(0); i < MAX_SELL_YB_RECORD_PER_PAGE; ++i)
	{
		iconName.Format("SellYBYBTIcon%02d", i);
		pIm1 = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		iconName.Format("SellYBMoneyIcon%02d", i);
		pIm2 = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		iconName.Format("SellYBBuySellName%02d", i );
		pText1 = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("SellYBBuySellPrice%02d", i);
		pText2 = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("SellYBBuySellCount%02d", i);
		pText3 = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("SellYBBuySelect%02d", i);
		pIm3 = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		iconName.Format("SellYBBuyHighLight%02d", i);
		pIm4 = gGlobal.pUIManager->FindUIImage(iconName.c_str());

		if (pIm1 && pIm2 && pIm3 && pIm4 && pText1 && pText2 && pText3 && gGlobal.g_pSellYuanBaoGroup)
		{
			CSellYuanBao* pRecord = gGlobal.g_pSellYuanBaoGroup->GetSellYBRecord(m_iPage * MAX_SELL_YB_RECORD_PER_PAGE + i);
			if (pRecord)
			{
				String text;
				pIm1->SetEnable(true);
				pIm2->SetEnable(true);
				text.Format(_T("%s"), gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT3")));
				pText1->SetText(text.c_str());
				pText1->Redraw();
				pText1->SetEnable(true);

				text.Format(_T("%d%s"), pRecord->GetMoneyPerYBT(), gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT2")));
				pText2->SetText(text.c_str());
				Color32 textColor = gGlobal.GetDisplayMoneyColor(pRecord->GetMoneyPerYBT());
				if (textColor != 0)
					pText2->SetColor(textColor);
				pText2->Redraw();
				pText2->SetEnable(true);
				
				text.Format(_T("%d"), pRecord->GetRemain());
				pText3->SetText(text.c_str());
				pText3->Redraw();
				pText3->SetEnable(true);

				if (IsSellYuanBaoMenu())
					pIm3->SetEnable(false);								
				else
					pIm3->SetEnable(true);

				if (m_iSelectedSellRecordID == pRecord->GetSellYuanBao_id())
					pIm4->SetEnable(true);
				else
					pIm4->SetEnable(false);
			}
			else
			{
				pIm1->SetEnable(false);
				pIm2->SetEnable(false);
				pIm3->SetEnable(false);
				pIm4->SetEnable(false);
				pText1->SetEnable(false);
				pText2->SetEnable(false);
				pText3->SetEnable(false);
			}
		}
	}
	pText4 = gGlobal.pUIManager->FindUIStaticText("SellYBBuySellPageNo");
	if (pText4)
	{
		UInt totalPage = gGlobal.g_pSellYuanBaoGroup->GetRecordSize() / MAX_SELL_YB_RECORD_PER_PAGE;
		if (gGlobal.g_pSellYuanBaoGroup->GetRecordSize() % MAX_SELL_YB_RECORD_PER_PAGE > 0)
			++totalPage;

		if (totalPage == 0)
			totalPage = 1;
		String text;
		text.Format(_T("%d / %d"), m_iPage + 1, totalPage);
		pText4->SetText(text.c_str());
		pText4->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pText4);
	}
}

void CMenuSellYuanBaoProcess::PageUpAndDown(bool up)
{
	if (up)
	{
		if (m_iPage > 0)
			--m_iPage;
	}
	else if (gGlobal.g_pSellYuanBaoGroup)
	{
		if ((m_iPage + 1) * MAX_SELL_YB_RECORD_PER_PAGE < gGlobal.g_pSellYuanBaoGroup->GetRecordSize())
			++m_iPage;
	}

	UpdateSellYBRecord();
}

void CMenuSellYuanBaoProcess::ConfirmSellYB(int amount, int pricePerUnit)
{
	UIEditBox* passwordBox = gGlobal.pUIManager->FindUIEditBox("SellYBBuySellConfrimTextPassword");
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() && passwordBox)
	{
		if (passwordBox->GetTextSize() > 0)
		{
			if (gGlobal.g_pCharacter->GetCharNetObject()->GetMoney() >= SELL_YB_TAX) 
			{
				gGlobal.g_pCharacter->Stop();
				gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_SELLING")), false);
				gGlobal.g_pCharacter->GetCharNetObject()->AddSellYuanBao(gGlobal.pPeerProxy, gGlobal.g_context, amount, pricePerUnit, passwordBox->GetHashedPassword());
				passwordBox->ClearText();
				passwordBox->Redraw();
			}
			else
			{
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_NO_MONEY")));
			}
		}
		else
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_PLEASE_ENTER_PASSWORD")));
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PLEASE_ENTER_PASSWORD")), CHANNELTYPE_SYSTEM);
		}
	}
}

void CMenuSellYuanBaoProcess::SetConfirmBoxText(int YBT, int pricePerUnit)
{
	UIStaticText* pText = NULL;
	UIEditBox* pEditBox = NULL;
	String text;
	switch (m_iMenuIndex)
	{
	case SELLYBMENU_YBTOYBT:
		{
			pText = gGlobal.pUIManager->FindUIStaticText("SellYBChoiceConfrimText");
			pEditBox = gGlobal.pUIManager->FindUIEditBox("SellYBChoiceConfrimTextPassword");
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_YBT_CONFIRM")), YBT * YB_TO_YBT_RATIO, YBT); //1 YBT = 1000 YB
		}
		break;
	case SELLYBMENU_YBTTOYB:
		{
			pText = gGlobal.pUIManager->FindUIStaticText("SellYBChoiceConfrimText");
			pEditBox = gGlobal.pUIManager->FindUIEditBox("SellYBChoiceConfrimTextPassword");
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_YB_CONFIRM")), YBT, YBT * YB_TO_YBT_RATIO); //1 YBT = 1000 YB
		}
		break;
	case SELLYBMENU_SELL:
		{
			pText = gGlobal.pUIManager->FindUIStaticText("SellYBSellConfrimText");
			pEditBox = gGlobal.pUIManager->FindUIEditBox("SellYBBuySellConfrimTextPassword");
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_SELL_CONFIRM")), pricePerUnit, YBT);
		}
		break;
	}

	if (pText)
	{
		pText->SetText(text.c_str());
		pText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pText);
	}

	if (pEditBox)
	{
		pEditBox->ClearText();
		pEditBox->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
	}
}

void CMenuSellYuanBaoProcess::SelectBuyYBT(int index)
{
	if (m_iMenuIndex == SELLYBMENU_BUY && index >= 0 && gGlobal.g_pSellYuanBaoGroup)
	{
		UInt realIndex = m_iPage * MAX_SELL_YB_RECORD_PER_PAGE + index;
		CSellYuanBao* pData = gGlobal.g_pSellYuanBaoGroup->GetSellYBRecord(realIndex);
		if (pData)
		{
			m_iSelectedSellRecordID = pData->GetSellYuanBao_id();
		}
		else
			m_iSelectedSellRecordID = 0;
	}
	else
		m_iSelectedSellRecordID = 0;
}

void CMenuSellYuanBaoProcess::ConfirmBuyYB()
{
	UInt amount = 0;
	UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SellYBBuyAmount");
	if (pEditBox)
		amount = pEditBox->GetInt();

	UIEditBox* passwordBox = gGlobal.pUIManager->FindUIEditBox("SellYBBuySellConfrimTextPassword");

	if (gGlobal.g_pSellYuanBaoGroup && passwordBox)
	{
		if (passwordBox->GetTextSize() > 0)
		{
			CSellYuanBao* pData = gGlobal.g_pSellYuanBaoGroup->GetSellYBRecordByID(m_iSelectedSellRecordID);
			if (pData)
			{
				if (pData->GetChar_id() == gGlobal.g_pCharacter->GetChar_id())
				{
					gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_OWNER")));
					gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_OWNER")), CHANNELTYPE_SYSTEM);
					return;
				}

				if (gGlobal.g_pCharacter->GetCharNetObject()->GetMoney() < (amount * pData->GetMoneyPerYBT()))
				{
					gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_NO_MONEY")));
					gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_NO_MONEY")), CHANNELTYPE_SYSTEM);
					return;
				}

				if (pData->GetRemain() >= amount )
				{
					gGlobal.g_pCharacter->Stop();
					gGlobal.g_pCharacter->GetCharNetObject()->BuySellYuanBao(gGlobal.pPeerProxy, gGlobal.g_context, 
						m_iSelectedSellRecordID, amount, pData->GetMoneyPerYBT(), passwordBox->GetHashedPassword());
					gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_BUYING")), false);
					passwordBox->ClearText();
					passwordBox->Redraw();
				}
			}
		}
		else
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_PLEASE_ENTER_PASSWORD")));
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PLEASE_ENTER_PASSWORD")), CHANNELTYPE_SYSTEM);
		}
	}
	else
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_ERROR")));
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_ERROR")), CHANNELTYPE_SYSTEM);
	}
}

void CMenuSellYuanBaoProcess::DisplayBuyYBConfirmText()
{	
	if (gGlobal.g_pSellYuanBaoGroup && m_iSelectedSellRecordID > 0)
	{
		UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("SellYBBuyAmount");
		UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText("SellYBBuyPricePerUnit");
		UIStaticText* pText1 = gGlobal.pUIManager->FindUIStaticText("SellYBBuyTotalPrice");
		CSellYuanBao* pData = gGlobal.g_pSellYuanBaoGroup->GetSellYBRecordByID(m_iSelectedSellRecordID);
		if (pEditBox && pText && pText1)
		{
			UInt amount = pEditBox->GetInt();
			if (amount > pData->GetRemain())
			{
				amount = pData->GetRemain();
				String tempText;
				tempText.Format(_T("%d"), amount);
				pEditBox->SetText(tempText.c_str());
				pEditBox->Redraw();
				gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
			}
			UInt totalPrice = amount * pData->GetMoneyPerYBT();
			String text;
			text.Format(_T("%s: %d"), gGlobal.GetStringTable()->Get(_T("MSG_HAWK_SINGLE_PIRCE")), pData->GetMoneyPerYBT());
			pText->SetText(text.c_str());

			Color32 textColor = gGlobal.GetDisplayMoneyColor(totalPrice);
			pText->SetColor(textColor);
			pText->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(pText);

			text.Format(_T("%s: %d"), gGlobal.GetStringTable()->Get(_T("MSG_PRICE_TITLE_TOTAL_PRICE")), totalPrice);
			pText1->SetText(text.c_str());
			pText1->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(pText1);
		}
	}
}

void CMenuSellYuanBaoProcess::ResetSelectBuyYBT()
{
	m_iSelectedSellRecordID = 0;
}

void CMenuSellYuanBaoProcess::GetSellYBTempBank()
{
	if (gGlobal.g_pSellYuanBaoGroup)
		gGlobal.g_pSellYuanBaoGroup->GetTempBank(gGlobal.pPeerProxy, gGlobal.g_context);
}

void CMenuSellYuanBaoProcess::OpenTempBank(UInt32 YBT, UInt32 money)
{
	m_iMenuIndex = SELLYBMENU_TEMPBANK;
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenSellYBTempBank", String());
	String text;
	text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_TEMP_BANK_TEXT")), money, YBT);
	UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText("SellYBTempBankText");
	if (pText)
	{
		pText->SetText(text.c_str());
		pText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pText);
	}
}

void CMenuSellYuanBaoProcess::GetAllTBMoneyYBT()
{
	UIEditBox* passwordBox = gGlobal.pUIManager->FindUIEditBox("SellYBTempBankConfirmTextBG");
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() && passwordBox)
	{
		if (passwordBox->GetTextSize() > 0)
		{
			gGlobal.g_pCharacter->Stop();
			gGlobal.g_pCharacter->GetCharNetObject()->GetAllMoneyAndYBT(gGlobal.pPeerProxy, gGlobal.g_context, passwordBox->GetHashedPassword());
			gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_TEMPBANK_GETTING")), false);
			passwordBox->ClearText();
			passwordBox->Redraw();
		}
		else
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_PLEASE_ENTER_PASSWORD")));
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PLEASE_ENTER_PASSWORD")), CHANNELTYPE_SYSTEM);
		}
	}
}

void CMenuSellYuanBaoProcess::UpdateSellYuanBaoRule()
{
	UIChatPanel* pChat = NULL;
	TCHAR rule[101];
	switch (m_iMenuIndex)
	{
	case SELLYBMENU_MAINPAGE:
		{
			GetPrivateProfileString(_T("SELLYUANBAORULE"), _T("MAIN_RULE"), _T("\0"), rule, 100, _T(".\\Config\\Decs.ini"));
			pChat = gGlobal.pUIManager->FindUIChatPanel("SellYBChoiceRule");
		}
		break;
	case SELLYBMENU_CHANGEYBSELECT:
		{
			GetPrivateProfileString(_T("SELLYUANBAORULE"), _T("CHANGE_YB_YBT_RULE"), _T("\0"), rule, 100, _T(".\\Config\\Decs.ini"));
			pChat = gGlobal.pUIManager->FindUIChatPanel("SellYBChoiceRule");
		}
		break;
	case SELLYBMENU_YBTOYBT:
		{
			GetPrivateProfileString(_T("SELLYUANBAORULE"), _T("YB_TO_YBT_RULE"), _T("\0"), rule, 100, _T(".\\Config\\Decs.ini"));
			pChat = gGlobal.pUIManager->FindUIChatPanel("SellYBChoiceRule");
		}
		break;
	case SELLYBMENU_YBTTOYB:
		{
			GetPrivateProfileString(_T("SELLYUANBAORULE"), _T("YBT_TO_YB_RULE"), _T("\0"), rule, 100, _T(".\\Config\\Decs.ini"));
			pChat = gGlobal.pUIManager->FindUIChatPanel("SellYBChoiceRule");
		}
		break;
	case SELLYBMENU_SELL:
		{
			GetPrivateProfileString(_T("SELLYUANBAORULE"), _T("SELL_YB_RULE"), _T("\0"), rule, 100, _T(".\\Config\\Decs.ini"));
			pChat = gGlobal.pUIManager->FindUIChatPanel("SellYBBuySellRule");
		}
		break;
	case SELLYBMENU_BUY:
		{
			GetPrivateProfileString(_T("SELLYUANBAORULE"), _T("BUY_YB_RULE"), _T("\0"), rule, 100, _T(".\\Config\\Decs.ini"));
			pChat = gGlobal.pUIManager->FindUIChatPanel("SellYBBuySellRule");
		}
		break;
	}
	rule[100] = _T('\0');

	if (pChat)
	{
		pChat->ClearAllRow();
		pChat->AddStringW(String(rule).c_str());
		pChat->EndRow();
		pChat->SetEnable(true);
		gGlobal.pUIManager->AddDirtyUIItem(pChat);
	}
}

void CMenuSellYuanBaoProcess::SetEditBoxColor(PCSTRINGA staticTextName, UInt32 value)
{
	UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText(staticTextName);
	if (pText)
	{
		pText->SetColor(gGlobal.GetDisplayMoneyColor(value));
		pText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pText);
	}
}