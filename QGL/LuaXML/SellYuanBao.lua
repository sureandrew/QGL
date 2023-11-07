SellYuanBaoMenuState = -1;
TotalYBTNumber = -1;
PricePerUnit = -1;

function OpenSellYuanBaoChoice()
	local panel = UIManager:GetUIItem("SellYBChoicePanel");
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\SellYBChoice.xml");
	    local sellYBProcess = LuaUI.GetSellYBProcess();
    	if sellYBProcess ~= nil then
    		sellYBProcess:InitStartPos();
    	end
	end
	
	panel = UIManager:GetUIItem("SellYBBuySellPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\SellYBBuySell.xml");
	end
	
	panel = UIManager:GetUIItem("SellYBTempBankPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\SellYBTempBank.xml");
	end
	SwitchOnOffSellYBTempBank(false);
	SwitchOnOffSellYBBuySell(false);
	OpenSellYBMain();
end

function SwitchOffAllSellYBButtonField()
    ShowSellYBMainButton(false);
    ShowChangeYBYBTButton(false);
    ShowExchangeYBYBTButtonField(false, 0);
end

function OpenSellYBMain()
    _G.SellYuanBaoMenuState = 0;
    SwitchOnOffSellYBChoice(true);
    SwitchOffAllSellYBButtonField();
    ShowSellYBMainButton(true);
    local sellYBProcess = LuaUI.GetSellYBProcess();
    if sellYBProcess ~= nil then
        sellYBProcess:OpenSellYBMenuChoice();
        ShowSellYBMainButton(true);
    end
end

function OpenChangeYBSelect()
    SwitchOffAllSellYBButtonField();
    ShowChangeYBYBTButton(true);
    local sellYBProcess = LuaUI.GetSellYBProcess();
    if sellYBProcess ~= nil then
        sellYBProcess:OpenChangeYBSelect();
    end
end

function OpenExchangeYBYBT(type)
    SwitchOffAllSellYBButtonField();
    ShowExchangeYBYBTButtonField(true, type);
    local sellYBProcess = LuaUI.GetSellYBProcess();
    if sellYBProcess ~= nil then
        sellYBProcess:OpenExchangeYBYBT(type);
    end
end

function OpenExchangeYBToYBT()
    OpenExchangeYBYBT(0);
end

function OpenExchangeYBTToYB()
    OpenExchangeYBYBT(1);
end

function CloseSellYuanBaoChoice()
	UIManager:ClearMonoPanel();
	UIManager:StopDrag();
	local sellYBProcess = LuaUI.GetSellYBProcess();
    if sellYBProcess ~= nil then
        sellYBProcess:CloseSellYuanBao();
    end
    _G.SellYuanBaoMenuState = -1;
    _G.TotalYBTNumber = -1;
	UIManager:DeletePanel("SellYBChoicePanel");
	UIManager:DeletePanel("SellYBBuySellPanel");
	UIManager:DeletePanel("SellYBChoiceConfirmPanel");
	UIManager:DeletePanel("SellYBBuySellConfirmPanel");
	ResetKeyIn();
end

function SwitchOnOffSellYBChoice(on)
	SwitchOffAllSellYBButtonField();
    local panel = UIManager:GetUIItem("SellYBChoicePanel");
    if panel ~= nil then
        panel:SetEnable(on);
    end
end

function SwitchOnOffSellYBBuySell(on)
	local panel = UIManager:GetUIItem("SellYBBuySellPanel");
	if panel ~= nil then
		panel:SetEnable(on);
	end
end

function ShowSellYBSellButton(show)
	local BG = UIManager:GetUIItem("SellYBSellCountBG");
	local editbox = UIManager:GetUIItem("SellYBSellAmount");
	local editbox2 = UIManager:GetUIItem("SellYBSellPrice");
	local text = UIManager:GetUIItem("SellYBSellTax");
	local text2 = UIManager:GetUIItem("SellYBSellTotalPrice");
	local button = UIManager:GetUIItem("SellYBSell");
	local title = UIManager:GetUIItem("SellYBSellTitle");
	
	if BG ~= nil and editbox ~= nil and editbox2 ~= nil and 
		text ~= nil and text2 ~= nil and button ~= nil and title ~= nil then
		BG:SetEnable(show);
		editbox:SetEnable(show);
		editbox2:SetEnable(show);
		text:SetEnable(show);
		text2:SetEnable(show);
		button:SetEnable(show);
		title:SetEnable(show);
	end
	
	local tempEditBox = UIManager:FindUIEditBox("SellYBSellAmount");
	local tempEditBox2 = UIManager:FindUIEditBox("SellYBSellPrice");
	if tempEditBox ~= nil and editbox ~= nil and 
		tempEditBox2 ~= nil and editbox2 ~= nil then
		tempEditBox:SetText("1");
		tempEditBox:Redraw();
  		tempEditBox2:SetText("10000");
  		tempEditBox2:Redraw();
		UIManager:AddDirtyUIItem(editbox);
		UIManager:AddDirtyUIItem(editbox2);
		ShowSellYBTotalPrice();
	end
end

function ShowSellYBBuyButton(show)
	local button = UIManager:GetUIItem("SellYBBuy");
	local title = UIManager:GetUIItem("SellYBBuyTitle");
		
	if button ~= nil and title ~= nil then
		button:SetEnable(show);
		title:SetEnable(show);
	end
	
	if show ~= true then
		for i = 0, 7 do
		    local iconName = "SellYBBuySelect0"..i;
			local img = UIManager:GetUIItem(iconName);
			if img ~= nil then
			    img:SetEnable(show);
			end
		end
	end
end

function OpenSellYBSell()
	SwitchOnOffSellYBChoice(false);
	SwitchOnOffSellYBBuySell(true);
	ShowSellYBSellButton(true);
	ShowSellYBBuyButton(false);
	local sellYBProcess = LuaUI.GetSellYBProcess();
    if sellYBProcess ~= nil then
        sellYBProcess:OpenSellYBBuySell(0);
    end
end

function OpenSellYBBuy()
	SwitchOnOffSellYBChoice(false);
	SwitchOnOffSellYBBuySell(true);
	ShowSellYBSellButton(false);
	ShowSellYBBuyButton(true);
	local sellYBProcess = LuaUI.GetSellYBProcess();
    if sellYBProcess ~= nil then
        sellYBProcess:OpenSellYBBuySell(1);
    end
end

function ShowSellYBMainButton(show)
	local button = UIManager:GetUIItem("SellYBChoiceYBYBTExchange");
	local button1 = UIManager:GetUIItem("SellYBChoiceSellYB");
	local button2 = UIManager:GetUIItem("SellYBChoiceBuyYB");
	local button3 = UIManager:GetUIItem("SellYBChoiceTempBank");
	local title = UIManager:GetUIItem("SellYBChoiceTitle00");
	
	if button ~= nil and button1 ~= nil and
		button2 ~= nil and button3 ~= nil and title ~= nil then
	    button:SetEnable(show);
	    button1:SetEnable(show);
	    button2:SetEnable(show);
	    button3:SetEnable(show);
	    title:SetEnable(show);
	end
end

function ShowChangeYBYBTButton(show)
    local button = UIManager:GetUIItem("SellYBChoiceWantYBT");
	local button1 = UIManager:GetUIItem("SellYBChoiceWantYB");
	local button2 = UIManager:GetUIItem("SellYBChoiceWantBack");
    local title = UIManager:GetUIItem("SellYBChoiceTitle00");

 	if button ~= nil and button1 ~= nil and
		button2 ~= nil and title ~= nil then
	    button:SetEnable(show);
	    button1:SetEnable(show);
	    button2:SetEnable(show);
	    title:SetEnable(show);
	end
end

function ShowExchangeYBYBTButtonField(show, type)
	local button = UIManager:GetUIItem("SellYBChoiceConfirm");
	local button1 = UIManager:GetUIItem("SellYBChoiceBack");
	local editbox = UIManager:GetUIItem("NumberOfSellYBChoiceChangeAmount");
	local img = UIManager:GetUIItem("SellYBChoiceChangeAmount");
	local amountText = UIManager:GetUIItem("SellYBChoiceTotalAmount");
	local title = UIManager:GetUIItem("SellYBChoiceTitle01");
	local title1 = UIManager:GetUIItem("SellYBChoiceTitle02");
	local amountTitle = UIManager:GetUIItem("SellYBChoiceSpendYBTitle");
	local amountTitle1 = UIManager:GetUIItem("SellYBChoiceGetYBTitle");
	
	if button ~= nil and button1 ~= nil and editbox ~= nil and
	    img ~= nil and amountText ~= nil and title ~= nil and
		title1 ~= nil and amountTitle ~= nil and amountTitle1 ~= nil then
		button:SetEnable(show);
		button1:SetEnable(show);
		editbox:SetEnable(show);
		img:SetEnable(show);
		amountText:SetEnable(show);
		
		if show == true then
		    if type == 0 then
		        title:SetEnable(true);
		    	title1:SetEnable(false);
		    	amountTitle:SetEnable(true);
		    	amountTitle1:SetEnable(false);
		    else
		        title:SetEnable(false);
		    	title1:SetEnable(true);
		    	amountTitle:SetEnable(false);
		    	amountTitle1:SetEnable(true);
		    end
		else
		    title:SetEnable(false);
		    title1:SetEnable(false);
		    amountTitle:SetEnable(false);
		    amountTitle1:SetEnable(false);
		end
	end
	
 	local tempEditbox = UIManager:FindUIEditBox("NumberOfSellYBChoiceChangeAmount");
 	local tempAmountText = UIManager:FindUIStaticText("SellYBChoiceTotalAmount");
 	if editbox ~= nil and tempEditbox ~= nil and
	 	amountText ~= nil and tempAmountText ~= nil then
      	tempEditbox:SetText("1");
		tempEditbox:Redraw();
  		tempAmountText:LuaSetText("1000");
  		local sellYBProcess = LuaUI.GetSellYBProcess();
	    if sellYBProcess ~= nil then
	 		sellYBProcess:SetEditBoxColor("SellYBChoiceTotalAmount", 1000);
	 	end	
  		tempAmountText:Redraw();
		UIManager:AddDirtyUIItem(editbox);
		UIManager:AddDirtyUIItem(amountText);
 	end
end

function ShowSellYBChoiceTotalAmount()
    local sellYBProcess = LuaUI.GetSellYBProcess();
    if sellYBProcess ~= nil then
        sellYBProcess:CheckYBTAmount();
    end
    ShowSellYBTotalPrice();
end


function ShowSellYBChoiceTotalAmount_DelKey(KeyCode)
	if KeyCode == LuaUI.VK_DELETE then
		ShowSellYBChoiceTotalAmount()
	end
end

function ConfirmChangeYBYBT()
	local tempValue = _G.TotalYBTNumber;
    CancelConfirmChangeYBYBT();
	local sellYBProcess = LuaUI.GetSellYBProcess();
	if sellYBProcess ~= nil then
		sellYBProcess:ConfirmChangeYBYBT(tempValue);
	end   	
end

function SellYBBuySellPageUp()
    local sellYBProcess = LuaUI.GetSellYBProcess();
	if sellYBProcess ~= nil then
		sellYBProcess:PageUpAndDown(true);
	end
end

function SellYBBuySellPageDown()
    local sellYBProcess = LuaUI.GetSellYBProcess();
	if sellYBProcess ~= nil then
		sellYBProcess:PageUpAndDown(false);
	end
end

function SellYBBuySellReturn()
    SwitchOnOffSellYBBuySell(false);
    SwitchOnOffSellYBChoice(true);
    OpenSellYBMain();
    local sellYBProcess = LuaUI.GetSellYBProcess();
	if sellYBProcess ~= nil then
		sellYBProcess:ResetSelectBuyYBT();
	end
end

function ConfirmSellYuanBao()
	local editBox = UIManager:FindUIEditBox("SellYBSellAmount");
	local editBox2 = UIManager:FindUIEditBox("SellYBSellPrice");
	local sellYBProcess = LuaUI.GetSellYBProcess();
	
	if editBox ~= nil and editBox2 ~= nil and sellYBProcess ~= nil then
	    local amount = editBox:GetInt();
	    local pricePerUnit = editBox2:GetInt();
	    
	    if amount > 0 and pricePerUnit > 0 then
	        sellYBProcess:ConfirmSellYB(amount, pricePerUnit);
	    end
	end
end

function SwitchOnOffSellYBChoiceConfirm(show)

	UIManager:ClearMonoPanel();
	local panel = UIManager:GetUIItem("SellYBChoiceConfirmPanel");
	if panel ~= nil then
	    panel:SetEnable(show);
	    if show then
	        local temp = UIManager:FindUIPanel("SellYBChoiceConfirmPanel");
	        if temp ~= nil then
	        	UIManager:SetPanel2Top(temp);
	            UIManager:SetMonoPanel(temp);
	        end
	    end
	end
end

function ShowConfirmChangeYBYBT()
    SwitchOnOffSellYBChoiceConfirm(true);
    local editbox = UIManager:FindUIEditBox("NumberOfSellYBChoiceChangeAmount");
    local sellYBProcess = LuaUI.GetSellYBProcess();
	if editbox ~= nil and sellYBProcess ~= nil then
		_G.TotalYBTNumber = editbox:GetInt();
		sellYBProcess:SetConfirmBoxText(_G.TotalYBTNumber, 0);
	end
	local editbox = UIManager:FindUIEditBox("SellYBChoiceConfrimTextPassword");
	if editbox ~= nil then
		editbox:ClearText();
		editbox:Redraw();
	end
end

function CancelConfirmChangeYBYBT()
    SwitchOnOffSellYBChoiceConfirm(false);
    _G.TotalYBTNumber = -1;
end

function SwitchOnOffSellYBSellConfirm(show)
	local text1 = UIManager:GetUIItem("SellYBSellConfrimText");
	if text1 ~= nil then
	    text1:SetEnable(show);
	end
end

function SwitchOnOffSellYBBuyConfirm(show)
	local text1 = UIManager:GetUIItem("SellYBBuyAmountTitle");
	local text2 = UIManager:GetUIItem("SellYBBuyPricePerUnit");
	local text3 = UIManager:GetUIItem("SellYBBuyTotalPrice");
	
	local img1 = UIManager:GetUIItem("SellYBBuyAmountBG");
	local editbox = UIManager:GetUIItem("SellYBBuyAmount");
	
	if text1 ~= nil and text2 ~= nil and text3 ~= nil and
		img1 ~= nil and editbox ~= nil then
	  text1:SetEnable(show);
	  text2:SetEnable(show);
	  text3:SetEnable(show);
	  
	  img1:SetEnable(show);
	  editbox:SetEnable(show);
 	end
 	
 	local tempEditbox = UIManager:FindUIEditBox("SellYBBuyAmount");
 	if tempEditbox ~= nil and editbox ~= nil then
 	    tempEditbox:SetText("1");
		tempEditbox:Redraw();
  		UIManager:AddDirtyUIItem(editbox);
 	end
end

function ShowSellYBSellConfirm()
    local editbox = UIManager:FindUIEditBox("SellYBSellAmount");
	local editbox2 = UIManager:FindUIEditBox("SellYBSellPrice");
	local sellYBProcess = LuaUI.GetSellYBProcess();
	
	if editbox ~= nil and editbox2 ~= nil and sellYBProcess ~= nil then
	    local value1 = editbox:GetInt();
	    local value2 = editbox2:GetInt();
	    if value1 <= 0 or value2 <= 0 then
	        do return end
	    end
	    _G.TotalYBTNumber = value1;
		_G.PricePerUnit = value2;
		sellYBProcess:SetConfirmBoxText(_G.TotalYBTNumber, _G.PricePerUnit);
	end
	
	SwitchOnOffSellYBBuySellConfirm(true);
    SwitchOnOffSellYBSellConfirm(true);
    SwitchOnOffSellYBBuyConfirm(false);
    
    local editbox = UIManager:FindUIEditBox("SellYBBuySellConfrimTextPassword");
	if editbox ~= nil then
		editbox:ClearText();
		editbox:Redraw();
	end
end

function ShowSellYBBuyConfirm()
 	local editbox = UIManager:GetUIItem("SellYBBuyAmount");
 	local tempEditbox = UIManager:FindUIEditBox("SellYBBuyAmount");
 	if editbox ~= nil and tempEditbox ~= nil then
 	    tempEditbox:SetText("1");
 	    tempEditbox:Redraw();
 	    UIManager:AddDirtyUIItem(editbox);
 	end	
 	
 	local hasSelect = false;
 	for i = 0, 7 do 
 		local iconName = "SellYBBuyHighLight0"..i;
		local img = UIManager:GetUIItem(iconName);
		if img ~= nil and img:IsEnable() then
			hasSelect = true;
		end	
 	end
 	
 	if hasSelect then
 		local sellYBProcess = LuaUI.GetSellYBProcess();
	    if sellYBProcess ~= nil then
	 		sellYBProcess:DisplayBuyYBConfirmText();
	 	end
	    SwitchOnOffSellYBBuySellConfirm(true);
	    SwitchOnOffSellYBSellConfirm(false);
	    SwitchOnOffSellYBBuyConfirm(true);
    end
    local editbox = UIManager:FindUIEditBox("SellYBBuySellConfrimTextPassword");
	if editbox ~= nil then
		editbox:ClearText();
		editbox:Redraw();
	end
end

function SwitchOnOffSellYBBuySellConfirm(show)

	UIManager:ClearMonoPanel();
	local panel = UIManager:GetUIItem("SellYBBuySellConfirmPanel");
	if panel ~= nil then
	    panel:SetEnable(show);
	    if show then
	        local temp = UIManager:FindUIPanel("SellYBBuySellConfirmPanel");
	        if temp ~= nil then
	        	UIManager:SetPanel2Top(temp);
	            UIManager:SetMonoPanel(temp);
	        end
	    end
	end
end

function CancelSellYBBuySellConfirm()
    SwitchOnOffSellYBBuySellConfirm(false);
    _G.TotalYBTNumber = -1;
	_G.PricePerUnit = -1;
	local editbox = UIManager:GetUIItem("SellYBBuySellConfrimTextPassword");
	local tempEditbox = UIManager:FindUIEditBox("SellYBBuySellConfrimTextPassword");
	if editbox ~= nil and tempEditbox ~= nil then
		tempEditbox:SetText(" ");
		tempEditbox:Redraw();
		UIManager:AddDirtyUIItem(editbox);
	end
end

function ConfirmSellYBBuySell()
    local sellYBProcess = LuaUI.GetSellYBProcess();
    if sellYBProcess ~= nil then
		if _G.PricePerUnit >= 0 then
	        local value1 = _G.TotalYBTNumber;
			local value2 = _G.PricePerUnit;
			CancelSellYBBuySellConfirm();

			if value1 > 0 and value2 > 0 then
	             sellYBProcess:ConfirmSellYB(value1, value2);
			end
		else
            CancelSellYBBuySellConfirm();
            sellYBProcess:ConfirmBuyYB();
		end
	end
end

function ResetSellYBHighLight()
	for i = 0, 7 do
		local iconName = "SellYBBuyHighLight0"..i;
		local img = UIManager:GetUIItem(iconName);
		if img ~= nil then
			img:SetEnable(false);
		end
	end
end

function SelectBuySellYB(index)
    local sellYBProcess = LuaUI.GetSellYBProcess();
    ResetSellYBHighLight();
	if sellYBProcess ~= nil then
	    sellYBProcess:SelectBuyYBT(index);
	end
	local iconName = "SellYBBuyHighLight0"..index;
	local img = UIManager:GetUIItem(iconName);
	if img ~= nil then
		img:SetEnable(true);
	end
end

function SelectBuySellYB00()
    SelectBuySellYB(0);
end

function SelectBuySellYB01()
    SelectBuySellYB(1);
end

function SelectBuySellYB02()
    SelectBuySellYB(2);
end

function SelectBuySellYB03()
    SelectBuySellYB(3);
end

function SelectBuySellYB04()
    SelectBuySellYB(4);
end

function SelectBuySellYB05()
    SelectBuySellYB(5);
end

function SelectBuySellYB06()
    SelectBuySellYB(6);
end

function SelectBuySellYB07()
    SelectBuySellYB(7);
end

function DisplayBuyYBInfo()
	local sellYBProcess = LuaUI.GetSellYBProcess();
    if sellYBProcess ~= nil then
 		sellYBProcess:DisplayBuyYBConfirmText();
 	end
end 

function GetSellYBTempBank()
	local sellYBProcess = LuaUI.GetSellYBProcess();
    if sellYBProcess ~= nil then
 		sellYBProcess:GetSellYBTempBank();
 	end	   	
end   

function SwitchOnOffSellYBTempBank(show)
	UIManager:ClearMonoPanel();
	local panel = UIManager:GetUIItem("SellYBTempBankPanel");
	local tempPanel = UIManager:FindUIPanel("SellYBTempBankPanel");
	if panel ~= nil then
		panel:SetEnable(show);		
		if show then			
			UIManager:SetPanel2Top(tempPanel);
			UIManager:SetMonoPanel(tempPanel);
		end
	end
end

function OpenSellYBTempBank()
	SwitchOnOffSellYBChoice(false);
	SwitchOnOffSellYBTempBank(true);
end

function CloseSellYBTempBankPanel()
	SwitchOnOffSellYBTempBank(false);
	SwitchOnOffSellYBChoice(true);
	OpenSellYBMain();
	local editbox = UIManager:FindUIEditBox("SellYBTempBankConfirmTextBG");
	if editbox ~= nil then
		editbox:ClearText();
		editbox:Redraw();
	end
end

function GetAllTBMoneyYBT()
	local sellYBProcess = LuaUI.GetSellYBProcess();
    if sellYBProcess ~= nil then
 		sellYBProcess:GetAllTBMoneyYBT();
 	end	
end

function ShowSellYBTotalPrice()
	local text = UIManager:FindUIStaticText("SellYBSellTotalPrice");
	local tempText = UIManager:GetUIItem("SellYBSellTotalPrice");
	local editbox = UIManager:FindUIEditBox("SellYBSellPrice");
	local editbox2 = UIManager:FindUIEditBox("SellYBSellAmount");
	
	if text ~= nil and editbox ~= nil and editbox2 ~= nil and tempText ~= nil then
		local value = editbox:GetInt() * editbox2:GetInt();
		text:LuaSetText(value.."");
		local sellYBProcess = LuaUI.GetSellYBProcess();
	    if sellYBProcess ~= nil then
	 		sellYBProcess:SetEditBoxColor("SellYBSellTotalPrice", value);
	 	end	
		text:Redraw();
		UIManager:AddDirtyUIItem(tempText);
	end
end

