#pragma once
#ifndef _MENUBANK_H_
#define _MENUBANK_H_

const int MAX_DEPOSIT_AMT = 10000000;

class CMenuBankService
{
public:
	CMenuBankService(void);
	~CMenuBankService(void);

	///// Bank Service member functions

	// player confirm his input deposit/withdrawal value
	void ConfirmBankAction(Boolean credit);
	// check if the input deposit amount is valid or not	
	Boolean CheckDepositAmount(); 
	// check if the input withdrawal amount is valid or not
	Boolean CheckWithdrawalAmount();
	// if the input amount not valid, display the correct input value
	void RefreshInputValue(UInt32 money, Boolean saving);// bool "saving" indicate if it refers to deposit value or
	// open the deposit panel                            // withdrawal value 
	void OpenDepositInterface();
	// open the withdrawal panel
	void OpenWithdrawalInterface();


	void SetInputMoneyColour();


};


#endif

