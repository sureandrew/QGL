function OpenBankWithdrawalPanel()
    local panel = UIManager:GetUIItem("BankWithdrawalPanel");

	if panel == nil then
	   LuaUI.AddXMLFile2UIManger("Data\\MenuWithdrawalBank.xml");
	   panel = UIManager:GetUIItem("BankWithdrawalPanel");
	end
	if panel ~= nil then
        panel:SetPos(200, 210);
     	panel:SetEnable(true);
     	SetWithdrawFocus();
	end

end

function CloseBankWithdrawalPanel()
	local panel = UIManager:GetUIItem("BankWithdrawalPanel");
	if panel ~= nil then
		SetEditLineFocus();
	    UIManager:DeletePanel("BankWithdrawalPanel");
	end	
end


function ChkWithdrawAmount()

	local bank = LuaUI.GetBankProcess();
	if bank then
		bank:CheckWithdrawalAmount()
	end		
end


function SetWithdrawFocus()

	UIManager:SetKeyInItem("WithdrawMoney");

end


function WithdrawConfirm()

	local bank = LuaUI.GetBankProcess();
	if bank then
		bank:ConfirmBankAction(false);	
	end

end


function WithdrawCancel()

	local bank = LuaUI.GetBankProcess();
	if bank then
		bank:RefreshInputValue(0, false);
	end

end


function SetWmoneyColour()
	local bank = LuaUI.GetBankProcess();
	if bank then
		bank:SetInputMoneyColour();	
	end
end