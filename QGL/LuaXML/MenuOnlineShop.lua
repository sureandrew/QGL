OnlineShopTimeOutFlag = 0; -- 0: nothing 1: normal item 2: discount item 3: limited item

function OpenOnlineShop()
	local panel = UIManager:GetUIItem("OnlineShopPanel");

	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\MenuOnlineShop.xml");
    
	    panel = UIManager:GetUIItem("OnlineShopPanel");
	    if panel ~= nil then
	    	--local button1 = UIManager:FindUIButton("OnlineShopPreview");
	    	local button2 = UIManager:FindUIButton("OnlineShopGift");
	    	--local button3 = UIManager:FindUIButton("OnlineShopCharge");
	    	--local button4 = UIManager:FindUIButton("OnlineShopChangeMoney");
	    	
	    	if button2 ~= nil then
	    		button2:SetDisableState();
	    	end
	    	
	    	panel:SetEnableTimeOutEvent(false);
	    	panel:SetPos(50, 53);
      		UIManager:AddDirtyUIItem(panel);
	    end
	end
	UIManager:FindUIButton("DownBChargeShop"):SetNormalState();
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(26);
end

function AskForOnlineShop()
    local onlineShopProcess = LuaUI.GetOnlineShopProcess();
    local panel = UIManager:GetUIItem("OnlineShopPanel");

	if panel == nil and _G.InBattle ~= true then
    	if onlineShopProcess ~= nil then
    	    onlineShopProcess:AskForOnlineShop();
    	end
	else
        CloseOnlineShop();
    end
end

function CloseOnlineShop()
	UIManager:StopDrag();
	UIManager:ClearMonoPanel();
	UIManager:DeletePanel("OnlineShopPanel");
	UIManager:DeletePanel("ChargeShopItemCountBox");
	UIManager:DeletePanel("ConfirmBuyOnlineShopItemPanel");
	
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
	    onlineShopProcess:CloseOnlineShop();
    end
    CloseOnlineShopDetail();
    CloseOnlineShopPreview();
    UIManager:SetKeyInItem("EditLine");
end

function OnlineShopShowHotSale()
	ResetMenuButton();
	local word = "OnlineShopHotSale";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowHotSale(0);
	end
end

function OnlineShopShowMix()
	ResetMenuButton();
	local word = "OnlineShopMix";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowMix(0);
	end
end

function OnlineShopShowAssc()
	ResetMenuButton();
	local word = "OnlineShopAssc";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowAssc(0);
	end
end

function OnlineShopShowPartner()
	ResetMenuButton();
	local word = "OnlineShopPartner";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowPartner(0);
	end
end

function OnlineShopShowPet()
	ResetMenuButton();
	local word = "OnlineShopPet";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowPet(0);
	end
end

function OnlineShopShowDiamond()
	ResetMenuButton();
	local word = "OnlineShopDiamond";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowDiamond(0);
	end
end

function OnlineShopShowBook()
	ResetMenuButton();
	local word = "OnlineShopBook";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowBook(0);
	end
end

function OnlineShopShowFireworks()
	ResetMenuButton();
	local word = "OnlineShopFireworks";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowFireworks(0);
	end
end

function OnlineShopShowMould()
	ResetMenuButton();
	local word = "OnlineShopMould";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowMould(0);
	end
end

function OnlineShopShowCloth()
	ResetMenuButton();
	local word = "OnlineShopCloth";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowCloth(0);
	end
end

function OnlineShopShowCap()
	ResetMenuButton();
	local word = "OnlineShopCap";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowCap(0);
	end
end

function OnlineShopShowOther()
	ResetMenuButton();
	local word = "OnlineShopOther";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if button ~= nil and onlineShopProcess ~= nil then
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
		onlineShopProcess:ShowOther(0);
	end
end

function ResetMenuButton()
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
		local showIndex = onlineShopProcess:GetShowIndex();
		local word = "";
		if showIndex == 0 then
			word = "OnlineShopMix";
		elseif showIndex == 1 then
		    word = "OnlineShopAssc";
		elseif showIndex == 2 then
			word = "OnlineShopPet";
		elseif showIndex == 3 then
			word = "OnlineShopPartner";
		elseif showIndex == 4 then
			word = "OnlineShopCloth";
		elseif showIndex == 5 then
			word = "OnlineShopOther";
		elseif showIndex == 6 then
			word = "OnlineShopHotSale";
		elseif showIndex == 9 then
			word = "OnlineShopDiamond";
		elseif showIndex == 10 then
			word = "OnlineShopBook";
		elseif showIndex == 11 then
			word = "OnlineShopCap";
		elseif showIndex == 12 then
			word = "OnlineShopFireworks";
		elseif showIndex == 13 then
			word = "OnlineShopMould";
		end
		
		local button = UIManager:FindUIButton(word);
		if button ~= nil then
			button:SetNormalState();
			local temp = UIManager:GetUIItem(word);
			UIManager:AddDirtyUIItem(temp);
		end
	end
	ResetItemButton();
end

function ResetItemButton()
    CloseOnlineShopPreview();
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
		local selectedIndex = onlineShopProcess:GetSelectedIndex();
		if selectedIndex >= 0 and selectedIndex < 10 then
			local word = "OnlineShopItem0"..selectedIndex;
			local button = UIManager:FindUIButton(word);
			if button ~= nil then
				button:SetNormalState();
				onlineShopProcess:ResetSelectedIndex();
				local temp = UIManager:GetUIItem(word);
				UIManager:AddDirtyUIItem(temp);
			end
			
			button = UIManager:FindUIButton("OnlineShopPreview");
			if button ~= nil then
				button:SetDisableState();
				local temp = UIManager:GetUIItem("OnlineShopPreview");
				UIManager:AddDirtyUIItem(temp);					
			end
		end	
	end
end

function OnlineShopItem00Select()
	OnlineShopItemSelect(0);
end

function OnlineShopItem01Select()
    OnlineShopItemSelect(1);
end

function OnlineShopItem02Select()
	OnlineShopItemSelect(2);
end

function OnlineShopItem03Select()
	OnlineShopItemSelect(3);
end

function OnlineShopItem04Select()
	OnlineShopItemSelect(4);
end

function OnlineShopItem05Select()
	OnlineShopItemSelect(5);
end

function OnlineShopItem06Select()
	OnlineShopItemSelect(6);
end

function OnlineShopItem07Select()
	OnlineShopItemSelect(7);
end

function OnlineShopItem08Select()
	OnlineShopItemSelect(8);
end

function OnlineShopItem09Select()
	OnlineShopItemSelect(9);
end

function OnlineShopItemSelect(sel)
	if sel >= 0 and sel < 10 then
		ResetItemButton();
		ResetLimitedButton();
		ResetDiscountButton();
		local word = "OnlineShopItem0"..sel;
		local button = UIManager:FindUIButton(word);
		local onlineShopProcess = LuaUI.GetOnlineShopProcess();
		if onlineShopProcess ~= nil and button ~= nil then
			onlineShopProcess:SetSelectedIndex(sel);
			button:SetDisableState();
			local temp = UIManager:GetUIItem(word);
			UIManager:AddDirtyUIItem(temp);
		end
	end
end

function ResetLimitedButton()
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
		if onlineShopProcess:GetSelectedLimitedIndex() >= 0 then
			local word = "OnlineShopLimitedItem";
			local button = UIManager:FindUIButton(word);
			if button ~= nil then
				button:SetNormalState();
				onlineShopProcess:ResetSelectedLimitedIndex();
				local temp = UIManager:GetUIItem(word);
				UIManager:AddDirtyUIItem(temp);
			end
		end	
	end
end

function ResetDiscountButton()
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
		local selectedIndex = onlineShopProcess:GetSelectedDiscountIndex();
		if selectedIndex >= 0 and selectedIndex < 3 then
			local word = "OnlineShopDiscountItem0"..selectedIndex;
			local button = UIManager:FindUIButton(word);
			if button ~= nil then
				button:SetNormalState();
				onlineShopProcess:ResetSelectedDiscountIndex();
				local temp = UIManager:GetUIItem(word);
				UIManager:AddDirtyUIItem(temp);
			end
		end	
	end
end

function OnlineShopLimitedItemSelect()
	ResetItemButton();
	ResetDiscountButton();
	
	local word = "OnlineShopLimitedItem";
	local button = UIManager:FindUIButton(word);
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil and button ~= nil then
		onlineShopProcess:SetSelectedLimitedIndex(1);
		button:SetDisableState();
		local temp = UIManager:GetUIItem(word);
		UIManager:AddDirtyUIItem(temp);
	end
end

function OnlineShopDiscountItem00Select()
	OnlineShopDiscountItemSelect(0)
end

function OnlineShopDiscountItem01Select()
	OnlineShopDiscountItemSelect(1)
end

function OnlineShopDiscountItem02Select()
	OnlineShopDiscountItemSelect(2)
end

function OnlineShopDiscountItemSelect(sel)
	if sel >= 0 and sel < 3 then
		ResetItemButton();
		ResetLimitedButton();
		ResetDiscountButton();
		local word = "OnlineShopDiscountItem0"..sel;
		local button = UIManager:FindUIButton(word);
		local onlineShopProcess = LuaUI.GetOnlineShopProcess();
		if onlineShopProcess ~= nil and button ~= nil then
			onlineShopProcess:SetSelectedDiscountIndex(sel);
			button:SetDisableState();
			local temp = UIManager:GetUIItem(word);
			UIManager:AddDirtyUIItem(temp);
		end
	end
end

function OnlineShopNextPage()
	ResetItemButton();
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
	   	onlineShopProcess:NextPage();
	end
end

function OnlineShopPrevPage()
	ResetItemButton();
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
		onlineShopProcess:PrevPage();
	end
end

function OnlineShopPreConfirm()
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
		
		if onlineShopProcess:GetSelectedItemType() == 0 and onlineShopProcess:GetSelectedLimitedIndex() < 0 then			   
			OnlineShopShowCount();
		elseif onlineShopProcess:GetSelectedItemOverLap() == 1 or onlineShopProcess:GetSelectedLimitedIndex() > 0 then
			ShowOnlineShopConfirmBox();
			onlineShopProcess:SetBuyItemStack(1);
		end
	end
end

function ShowOnlineShopConfirmBox()
	
	local numEdit = UIManager:FindUIEditBox("ChargeShopItem_EditBox");
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	local confirmBuyPanel = UIManager:GetUIItem("ConfirmBuyOnlineShopItemPanel");
	local tempPanel = UIManager:FindUIPanel("ConfirmBuyOnlineShopItemPanel");
	if onlineShopProcess ~= nil and numEdit ~= nil and confirmBuyPanel ~= nil then
		local value = numEdit:GetInt();
		if value > 0 then
			SwitchOffOnlineShopCount();
			UIManager:ClearMonoPanel();
			UIManager:SetMonoPanel(tempPanel);
			UIManager:SetPanel2Top(tempPanel);
			onlineShopProcess:SetBuyItemStack(value);
			confirmBuyPanel:SetEnable(true);
		end
	end
end

function OnlineShopShowCount()
	local countPanel = UIManager:GetUIItem("ChargeShopItemCountBox");
	local tempPanel = UIManager:FindUIPanel("ChargeShopItemCountBox");
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if countPanel ~= nil and onlineShopProcess ~= nil and tempPanel ~= nil then
	
		UIManager:SetKeyInItem("ChargeShopItem_EditBox");
		UIManager:ClearMonoPanel();
		UIManager:SetPanel2Top(tempPanel);
    	UIManager:SetMonoPanel(tempPanel);
    	countPanel:SetEnable(true);
    	
    	local temp = UIManager:GetUIItem("ChargeShopItem_Price");
		local text = UIManager:FindUIStaticText("ChargeShopItem_Price");
		if text ~= nil and temp ~= nil then
			local money = onlineShopProcess:GetSelectedItemPrice(1);
			money = money.."";
			text:LuaSetText(money);
			text:Redraw();
			UIManager:AddDirtyUIItem(temp);
		end	
		OnlineShopCountSetMin();
		UIManager:AddDirtyUIItem(countPanel);
	end
end

function SwitchOffOnlineShopCount()
	local countPanel = UIManager:GetUIItem("ChargeShopItemCountBox");
	if countPanel ~= nil then
		local numEdit = UIManager:FindUIEditBox("ChargeShopItem_EditBox");
		if numEdit ~= nil then
			numEdit:SetText("1");
			numEdit:End();
			numEdit:Redraw();
		end
		UIManager:SetKeyInItem("EditLine");
		UIManager:ClearMonoPanel();
		countPanel:SetEnable(false);
	end
end

function OnlineShopItemMouseIn()
	local pPanel = UIManager:GetUIItem("OnlineShopPanel");
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(true);
		pPanel:ResetTimer();
	end
	_G.OnlineShopTimeOutFlag = 1;
end

function OnlineShopDiscountItemMouseIn()
	local pPanel = UIManager:GetUIItem("OnlineShopPanel");
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(true);
		pPanel:ResetTimer();
	end
	_G.OnlineShopTimeOutFlag = 2;

end

function OnlineShopLimitItemMouseIn()
	local pPanel = UIManager:GetUIItem("OnlineShopPanel");
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(true);
		pPanel:ResetTimer();
	end
	_G.OnlineShopTimeOutFlag = 3;

end

function OnlineShopItemMouseOut()
	if _G.OnlineShopTimeOutFlag == 0 then
		local pPanel = UIManager:GetUIItem("OnlineShopPanel");
		if pPanel ~= nil then
			pPanel:SetEnableTimeOutEvent(false);
		end
		CloseOnlineShopDetail();
	end
end


function OnlineShopDetailPopupTimeOut()
	local panel = UIManager:FindUIPanel("OnlineShopPanel");
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil and panel ~= nil then
	
		local cursorX = UIManager:GetCursorX();
	    local cursorY = UIManager:GetCursorY();
	    CloseOnlineShopDetail();
	    if cursorX > panel:GetX() and cursorY > panel:GetY() and cursorX < (panel:GetX() + panel:GetWidth()) and cursorY < (panel:GetY() + panel:GetHeight()) then
			if _G.OnlineShopTimeOutFlag == 1 then
				onlineShopProcess:ShowItemDetail();
			elseif _G.OnlineShopTimeOutFlag == 2 then
				onlineShopProcess:ShowDiscountItemDetail();
			elseif _G.OnlineShopTimeOutFlag == 3 then
				onlineShopProcess:ShowLimitedItemDetail();
			end
		end
		
		panel:SetEnableTimeOutEvent(false);
	end
	_G.OnlineShopTimeOutFlag = 0;
end

function CloseOnlineShopDetail()
	local panel = UIManager:GetUIItem("ItemDetailMenuPanel");
	local panel2 = UIManager:GetUIItem("PartnerDetailPanel");
	local panel3 = UIManager:GetUIItem("PetDetailPanel");
	if panel ~= nil then 
		UIManager:DeletePanel("ItemDetailMenuPanel");
	elseif panel2 ~= nil then
		UIManager:DeletePanel("PartnerDetailPanel");
	elseif panel3 ~= nil then
		UIManager:DeletePanel("PetDetailPanel");
	end
end

function OnlineShopCountSetMax()
	local numEdit = UIManager:FindUIEditBox("ChargeShopItem_EditBox");
	local temp = UIManager:GetUIItem("ChargeShopItem_EditBox");
	local text = UIManager:FindUIStaticText("ChargeShopItem_Price");
	local temp2 = UIManager:GetUIItem("ChargeShopItem_Price");
	
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil and text ~= nil then
		local stack = onlineShopProcess:GetMaxSelectedItemStack();
		local tempText = stack.."";
		numEdit:SetText(tempText);
		numEdit:End();
		numEdit:Redraw();
		local money = onlineShopProcess:GetSelectedItemPrice(stack);		
		money = money.."";
		text:LuaSetText(money);
		text:Redraw();
	end
	UIManager:AddDirtyUIItem(temp);
	UIManager:AddDirtyUIItem(temp2);	
end 

function OnlineShopCountSetMin()
	local numEdit = UIManager:FindUIEditBox("ChargeShopItem_EditBox");
	local temp = UIManager:GetUIItem("ChargeShopItem_EditBox");
	local text = UIManager:FindUIStaticText("ChargeShopItem_Price");
	local temp2 = UIManager:GetUIItem("ChargeShopItem_Price");
	numEdit:SetText("1");
	numEdit:End();
	numEdit:Redraw();
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil and text ~= nil then
		local money = onlineShopProcess:GetSelectedItemPrice(1);
		money = money.."";
		text:LuaSetText(money);
		text:Redraw();
	end
	UIManager:AddDirtyUIItem(temp);
	UIManager:AddDirtyUIItem(temp2);
end

function OnlineShopCountAddOne()
	local numEdit = UIManager:FindUIEditBox("ChargeShopItem_EditBox");
	local temp = UIManager:GetUIItem("ChargeShopItem_EditBox");
	local text = UIManager:FindUIStaticText("ChargeShopItem_Price");
	local temp2 = UIManager:GetUIItem("ChargeShopItem_Price");
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if numEdit ~= nil and text ~= nil and onlineShopProcess ~= nil then
		local value = numEdit:GetInt();
		if value >= 0 then
			value = value + 1;
		end
		
		numEdit:SetText(value.."");--value.."");
		numEdit:End();
		numEdit:Redraw();
		UIManager:AddDirtyUIItem(temp);
		
		local money = onlineShopProcess:GetSelectedItemPrice(value);
		money = money.."";
		text:LuaSetText(money);
		text:Redraw();
		UIManager:AddDirtyUIItem(temp2);
	end
end

function OnlineShopCountSubOne()
	local numEdit = UIManager:FindUIEditBox("ChargeShopItem_EditBox");
	local temp = UIManager:GetUIItem("ChargeShopItem_EditBox");
	local text = UIManager:FindUIStaticText("ChargeShopItem_Price");
	local temp2 = UIManager:GetUIItem("ChargeShopItem_Price");
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	
	if numEdit ~= nil and text ~= nil and onlineShopProcess ~= nil then
		local value = numEdit:GetInt();
		if value > 1 then
			value = value - 1;
		end
		
		numEdit:SetText(value.."");--value.."");
		numEdit:End();
		numEdit:Redraw();
		UIManager:AddDirtyUIItem(temp);
		
		local money = onlineShopProcess:GetSelectedItemPrice(value);
		money = money.."";
		text:LuaSetText(money);
		text:Redraw();
		UIManager:AddDirtyUIItem(temp2);
	end
end

function OnlineShopCountKeyUp()
	local numEdit = UIManager:FindUIEditBox("ChargeShopItem_EditBox");
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	local text = UIManager:FindUIStaticText("ChargeShopItem_Price");
	local temp2 = UIManager:GetUIItem("ChargeShopItem_Price");
	
	if onlineShopProcess ~= nil and numEdit ~= nil then
		local value = numEdit:GetInt();
		
		local money = onlineShopProcess:GetSelectedItemPrice(value);
		money = money.."";
		text:LuaSetText(money);
		text:Redraw();
		UIManager:AddDirtyUIItem(temp2);
	end
end


function CloseConfirmBuyOnlineShopItem()
	local confirmBuyPanel = UIManager:GetUIItem("ConfirmBuyOnlineShopItemPanel");
	if confirmBuyPanel ~= nil then
		confirmBuyPanel:SetEnable(false);
		UIManager:ClearMonoPanel();
	end
end

function ConfirmBuyOnlineShopItem()

	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
		CloseConfirmBuyOnlineShopItem();
	    onlineShopProcess:ConfirmBuyItem();
	end
end

function ShowOnlineShopPreview()

    local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
	    onlineShopProcess:Preview();
	end
end

function CloseOnlineShopPreview()
	UIManager:StopDrag();
    UIManager:DeletePanel("OnlineShopPreviewPanel");
end

function OnlineShopPreviewLeft()
    local onlineShopProcess = LuaUI.GetOnlineShopProcess();

	if onlineShopProcess ~= nil then
	    onlineShopProcess:PreviewRotate(true);
	end
end

function OnlineShopPreviewRight()
    local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
	    onlineShopProcess:PreviewRotate(false);
	end
end

function OnlineShopShowChargePage()
    local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
	    onlineShopProcess:OpenChargeShopWebSite(0);
	end
end

function OnlineShopShowChangePage()
    local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
	    onlineShopProcess:OpenChargeShopWebSite(1);
	end
end

function OnlineShopUpdateMoney()
	local onlineShopProcess = LuaUI.GetOnlineShopProcess();
	if onlineShopProcess ~= nil then
	    onlineShopProcess:UpdateYuanBao(true);
	end
end