function OpenBankDepositPanel()
    local panel = UIManager:GetUIItem("BankDepositPanel");

	if panel == nil then
	   LuaUI.AddXMLFile2UIManger("Data\\MenuDepositBank.xml");
	   panel = UIManager:GetUIItem("BankDepositPanel");
	end
	if panel ~= nil then
        panel:SetPos(200, 210);
     	panel:SetEnable(true);
     	SetDepositFocus();
	end

end

function CloseBankDepositPanel()
	local panel = UIManager:GetUIItem("BankDepositPanel");
	if panel ~= nil then
		SetEditLineFocus();
	    UIManager:DeletePanel("BankDepositPanel");
	end	
end


function ChkDepositAmount()

	local bank = LuaUI.GetBankProcess();
	if bank then
		bank:CheckDepositAmount()
	end	
end


function SetDepositFocus()

	UIManager:SetKeyInItem("DepositMoney");

end


function DepositConfirm()
	
	local bank = LuaUI.GetBankProcess();
	if bank then
		bank:ConfirmBankAction(true);	
	end

end


function DepositCancel()

	local bank = LuaUI.GetBankProcess();
	if bank then
		bank:RefreshInputValue(0, true);
	end	


end

function SetDemoneyColour()
	local bank = LuaUI.GetBankProcess();
	if bank then
		bank:SetInputMoneyColour();	
	end
end