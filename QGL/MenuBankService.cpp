// Common
#include "Common.h"
#include "QGL.h"
// self
#include ".\menubankservice.h"
// Library
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "QGL/CharacterControl.h"



CMenuBankService::CMenuBankService(void)
{

}

CMenuBankService::~CMenuBankService(void)
{

}

void CMenuBankService::OpenDepositInterface()
{
	if ((gGlobal.g_pCharacter)&&(gGlobal.g_pCharacter->GetCharNetObject()))
	{   //open the panel and display the value of pocket cash and bank saving account
		if (gGlobal.g_pCharacter->GetCharNetObject()->GetBankLock()){
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BANK_LOCK_CANT_USE")));	
			return;
		}	
		UInt32 pocketCash = gGlobal.g_pCharacter->GetCharNetObject()->GetMoney();
		UInt32 bankDeposit = gGlobal.g_pCharacter->GetCharNetObject()->GetBankMoney();
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenBankDepositPanel", String());
		String pCash, bDeposit;
		pCash.Format(_T("%d"), pocketCash);
		bDeposit.Format(_T("%d"), bankDeposit);
		UIStaticText* DcurrentText = gGlobal.pUIManager->FindUIStaticText("DCurrentCash");
		UIStaticText* DbankText = gGlobal.pUIManager->FindUIStaticText("DBankDeposit");
		gGlobal.SetUIStaticText("DCurrentCash", pCash);
		gGlobal.SetUIStaticText("DBankDeposit", bDeposit);	
		if (DcurrentText)
			SetTextMoneyColor(DcurrentText,pocketCash);
		if (DbankText)
			SetTextMoneyColor(DbankText,bankDeposit);

		UIPanel *panel = gGlobal.pUIManager->FindUIPanel("BankDepositPanel");
		if (panel)
		{
			gGlobal.pUIManager->ClearMonoPanel();
			gGlobal.pUIManager->SetPanel2Top(panel);	
			gGlobal.pUIManager->SetMonoPanel(panel);	
		}	
	}

}

void CMenuBankService::OpenWithdrawalInterface()
{
	if ((gGlobal.g_pCharacter)&&(gGlobal.g_pCharacter->GetCharNetObject()))
	{   //open the panel and display the value of pocket cash and bank saving account
		if (gGlobal.g_pCharacter->GetCharNetObject()->GetBankLock()){
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BANK_LOCK_CANT_USE")));	
			return;
		}	
		UInt32 pocketCash = gGlobal.g_pCharacter->GetCharNetObject()->GetMoney();
		UInt32 bankDeposit = gGlobal.g_pCharacter->GetCharNetObject()->GetBankMoney();
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenBankWithdrawalPanel", String());
		String pCash, bDeposit;
		pCash.Format(_T("%d"), pocketCash);
		bDeposit.Format(_T("%d"), bankDeposit);
		UIStaticText* WcurrentText = gGlobal.pUIManager->FindUIStaticText("WCurrentCash");
		UIStaticText* WbankText = gGlobal.pUIManager->FindUIStaticText("WBankDeposit");
		gGlobal.SetUIStaticText("WCurrentCash", pCash);
		gGlobal.SetUIStaticText("WBankDeposit", bDeposit);
		if (WcurrentText)
			SetTextMoneyColor(WcurrentText,pocketCash);
		if (WbankText)
			SetTextMoneyColor(WbankText,bankDeposit);

		UIPanel *panel = gGlobal.pUIManager->FindUIPanel("BankWithdrawalPanel");	
		if (panel)
		{
			gGlobal.pUIManager->ClearMonoPanel();
			gGlobal.pUIManager->SetPanel2Top(panel);	
			gGlobal.pUIManager->SetMonoPanel(panel);	
		}	
	}

}



void CMenuBankService::ConfirmBankAction(Boolean credit)
{

	if (credit)// if player want to deposit
	{
		if (CheckDepositAmount())
		{
			UIEditBox* DMoneyBox = gGlobal.pUIManager->FindUIEditBox("DepositMoney");
			if (gGlobal.g_pCharacter&&gGlobal.g_pCharacter->GetCharNetObject()&&DMoneyBox)
			{	// send deposit amount to server for setting the bank saving account
				UInt32 money = DMoneyBox->GetInt();
				gGlobal.g_pCharacter->GetCharNetObject()->DepositInBank(gGlobal.pPeerProxy, gGlobal.g_context,
					                                                    money);
			}
			//close the panel
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseBankDepositPanel", String());	
		}

	}
	else// if player want to withdraw
	{
		if (CheckWithdrawalAmount())
		{
			UIEditBox* WMoneyBox = gGlobal.pUIManager->FindUIEditBox("WithdrawMoney");
			if (gGlobal.g_pCharacter&&gGlobal.g_pCharacter->GetCharNetObject()&&WMoneyBox)
			{// send withdrawal amount to server for setting the bank saving account	
				UInt32 money = WMoneyBox->GetInt();	
				gGlobal.g_pCharacter->GetCharNetObject()->WithdrawFmBank(gGlobal.pPeerProxy, gGlobal.g_context,
					                                                     money);
			}//close the panel
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseBankWithdrawalPanel", String());	

		}	

	}

}

Boolean CMenuBankService::CheckDepositAmount()
{	// get the input value of deposit sum
	UIEditBox* DMoneyBox = gGlobal.pUIManager->FindUIEditBox("DepositMoney");
	UInt32 money = 0;
	Boolean valid = TRUE;
	if (DMoneyBox && DMoneyBox->GetInt() > 0)
	{
		money = DMoneyBox->GetInt();
	
		// now check if the input value is valid or not
		if ((gGlobal.g_pCharacter)&&(gGlobal.g_pCharacter->GetCharNetObject()))
		{
			UInt32 pocketCash = gGlobal.g_pCharacter->GetCharNetObject()->GetMoney();
			UInt32 bankDeposit = gGlobal.g_pCharacter->GetCharNetObject()->GetBankMoney();
			if (money <= pocketCash)// if proposed deposit money <= cash in pocket
			{
				//if ((money + bankDeposit) > MAX_DEPOSIT_AMT) // if the total deposit exceeds 10 million
				//{
				//	money = MAX_DEPOSIT_AMT - bankDeposit;
				//	String depstMsg = gGlobal.GetStringTable()->Get(_T("MSG_BANK_DEPOSIT_EXCEED_TEN_MILL"));
				//	gGlobal.PrintPlayerMessage(depstMsg);
				//	RefreshInputValue(money, TRUE);
				//	valid = FALSE; // the input value is invalid
				//}
			}
			else
			{// if the proposed deposit amount exceed what player has in his pocket 
				money = pocketCash;	// set the proposed deposit equal to what you have in pocket
				String depstMsg = gGlobal.GetStringTable()->Get(_T("MSG_BANK_DEPOSIT_EXCEED_CASH"));
				gGlobal.PrintPlayerMessage(depstMsg);
				if ((money + bankDeposit) > MAX_DEPOSIT_AMT) // if the total deposit exceeds 10 million
					money = MAX_DEPOSIT_AMT - bankDeposit;// set a value such that the overall deposit equals to 10 million
				RefreshInputValue(money, TRUE);
				valid = FALSE;

			}
		}
	}
	else
		 valid = FALSE;	

	return valid;
		
}

Boolean CMenuBankService::CheckWithdrawalAmount()
{	// get the input value of withdrawal sum
	UIEditBox* DMoneyBox = gGlobal.pUIManager->FindUIEditBox("WithdrawMoney");
	UInt32 money = 0;
	Boolean valid = TRUE;	
	if (DMoneyBox && DMoneyBox->GetInt() > 0)
	{
		money = DMoneyBox->GetInt();
		// now check if the input value is valid or not
		if ((gGlobal.g_pCharacter)&&(gGlobal.g_pCharacter->GetCharNetObject()))
		{
			UInt32 pocketCash = gGlobal.g_pCharacter->GetCharNetObject()->GetMoney();
			UInt32 bankDeposit = gGlobal.g_pCharacter->GetCharNetObject()->GetBankMoney();
			
			if (money > bankDeposit)
			{// if the withdrawal amount exceed what player has in his bank saving account 
				money = bankDeposit;	
				String withdrwMsg = gGlobal.GetStringTable()->Get(_T("MSG_BANK_WITHDRAW_EXCEED_SAVE"));
				gGlobal.PrintPlayerMessage(withdrwMsg);	
				RefreshInputValue(money, FALSE);
				valid = FALSE;	
			}
		}
	}
	else
		valid = FALSE;

	return valid;

}

void CMenuBankService::RefreshInputValue(UInt32 money, Boolean saving)
{
    /// reset the input value of withdrawal/deposit with the value of parameter "money".
	UIEditBox* DMoneyBox = NULL;
	if (gGlobal.g_pCharacter&&gGlobal.g_pCharacter->GetCharNetObject())
	{
		UInt32 pocketCash = gGlobal.g_pCharacter->GetCharNetObject()->GetMoney();
		UInt32 bankDeposit = gGlobal.g_pCharacter->GetCharNetObject()->GetBankMoney();
		String pCash, bDeposit;
		pCash.Format(_T("%d"), pocketCash);
		bDeposit.Format(_T("%d"), bankDeposit);
		if (saving)
		{   //reset the value of current pocket cash and saving account 

			UIStaticText* DcurrentText = gGlobal.pUIManager->FindUIStaticText("DCurrentCash");
			UIStaticText* DbankText = gGlobal.pUIManager->FindUIStaticText("DBankDeposit");
			gGlobal.SetUIStaticText("DCurrentCash", pCash);
			gGlobal.SetUIStaticText("DBankDeposit", bDeposit);	
			DMoneyBox = gGlobal.pUIManager->FindUIEditBox("DepositMoney");// get the "deposit" editbox
			SetTextMoneyColor(DcurrentText,pocketCash);
			SetTextMoneyColor(DbankText,bankDeposit);
		}
		else
		{	//reset the value of current pocket cash and saving account
			UIStaticText* WcurrentText = gGlobal.pUIManager->FindUIStaticText("WCurrentCash");
			UIStaticText* WbankText = gGlobal.pUIManager->FindUIStaticText("WBankDeposit");
			gGlobal.SetUIStaticText("WCurrentCash", pCash);
			gGlobal.SetUIStaticText("WBankDeposit", bDeposit);
			DMoneyBox = gGlobal.pUIManager->FindUIEditBox("WithdrawMoney");// get the "withdrawal" editbox
			SetTextMoneyColor(WcurrentText,pocketCash);
			SetTextMoneyColor(WbankText,bankDeposit);
		}
	}
	if (DMoneyBox)
	{// reset the value in editbox of withdrawal/deposit with the value "money" provided
		String dollors;
		if (money != 0)
            dollors.Format(_T("%d"), money);
		else
			dollors.Format(_T(""));
		DMoneyBox->SetText(dollors.c_str());
		DMoneyBox->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(DMoneyBox);

	}
 
}

void CMenuBankService::SetInputMoneyColour()
{
	UIEditBox* WithmoneyBox = gGlobal.pUIManager->FindUIEditBox("WithdrawMoney");	
	UIEditBox* DepositmoneyBox = gGlobal.pUIManager->FindUIEditBox("DepositMoney");	
	if(WithmoneyBox)
		SetEditBoxMoneyColor(WithmoneyBox);
	if (DepositmoneyBox)
		SetEditBoxMoneyColor(DepositmoneyBox);
}