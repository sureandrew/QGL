TradingTimeOutFlag = 0;

function OpenTradingPanel()

    local panel = UIManager:GetUIItem("TradingPanel");
    local itemPanel = UIManager:GetUIItem("ItemMenuPanel");
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\MenuTrading.xml");
	    
	    if itemPanel == nil then
	    	BBItem();
	    	itemPanel = UIManager:GetUIItem("ItemMenuPanel");
	    end
	    
		itemPanel:SetPos(400, 40);
		UIManager:AddDirtyUIItem(itemPanel);
		local gmoney = UIManager:GetUIItem("TradingMoney");
		if gmoney ~= nil then
		    gmoney:SetEnableTimeOutEvent(false);
		end
		
  		local confirmButton = UIManager:FindUIButton("TradingYes");
		local confirmButtonItem = UIManager:GetUIItem("TradingYes");
		if confirmButton ~= nil and confirmButtonItem ~= nil then
		    confirmButton:SetDisableState();
		    UIManager:AddDirtyUIItem(confirmButtonItem);
		end
		
		panel = UIManager:GetUIItem("TradingPanel");
		if panel ~= nil then
			panel:SetPos(20, 40);
			panel:SetEnableTimeOutEvent(false);
		end
		if _G.LockState or _G.UnlockState then
		    ResetAllLockStatus(); -- cancel asset lock or unlock status
		end
		SwitchOffPartnerCollection()
	else
	    CloseTradingPanel();
	end

end

function CloseTradingPanel()
	UIManager:StopDrag();
	local tradingProcess = LuaUI.GetTradingProcess();
	if tradingProcess ~= nil then
		tradingProcess:CancelTrading();
	end
	DeleteTradingPanel();
	CloseTradingDetail();
end

function DeleteTradingPanel()
    UIManager:DeletePanel("TradingPanel");
	UIManager:DeletePanel("TradingMovingPanel");
	UIManager:DeletePanel("TradingItemCountPanel");
	ResetKeyIn();
end

function OpenReqTrading()
    local panel = UIManager:GetUIItem("ReqTradingBox");

	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\ReqTradingBox.xml");
 	end
end

function CloseReqTrading()
    UIManager:DeletePanel("ReqTradingBox");
end

function ConfirmReqTrading()
	local tradingProcess = LuaUI.GetTradingProcess();
	if tradingProcess ~= nil then
		tradingProcess:ConfirmReqTrading()
	end
	CloseReqTrading();
end

function RejectReqTrading()
	local tradingProcess = LuaUI.GetTradingProcess();
	if tradingProcess ~= nil then
		tradingProcess:RejectReqTrading()
	end
	CloseReqTrading();
end


function TradingAddOne()
	local numEdit = UIManager:FindUIEditBox("NumberOfTradingItem");
	local temp = UIManager:GetUIItem("NumberOfTradingItem");
	
	local value = numEdit:GetInt();
	
	if value >= 0 and value < LuaUI.GetMaxStack(_G.GivenItemOrgIndex + 24 * _G.BagState) then
		value = value + 1;
	end
	
	numEdit:SetText(value.."");--value.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);--RenderAll();

end

function TradingDiffOne()

	local numEdit = UIManager:FindUIEditBox("NumberOfTradingItem");
	local temp = UIManager:GetUIItem("NumberOfTradingItem");
	
	local value = numEdit:GetInt();
	
	if value > 1 then
		value = value - 1;
	end
	
	numEdit:SetText(value.."");--value.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function TradingSetZero()
	local numEdit = UIManager:FindUIEditBox("NumberOfTradingItem");
	local temp = UIManager:GetUIItem("NumberOfTradingItem");
	numEdit:SetText("1");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
 end

function TradingMaxStack()

	local numEdit = UIManager:FindUIEditBox("NumberOfTradingItem");
	local temp = UIManager:GetUIItem("NumberOfTradingItem");
	local max = LuaUI.GetMaxStack(_G.GivenItemOrgIndex + 24 * _G.BagState);
	
	numEdit:SetText(max.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function CloseTradingCount()
	ResetKeyIn();
    local countPanel = UIManager:GetUIItem("TradingItemCountPanel");
    local numEdit = UIManager:FindUIEditBox("NumberOfTradingItem");
    if countPanel ~= nil and numEdit ~= nil then
    	countPanel:SetEnable(false);
    	numEdit:SetText("1");
		numEdit:Redraw();
    end
    
    _G.GivenItemOrgIndex = -1;
    _G.PlacedItemIndex = -1;
    UIManager:ClearMonoPanel();
end

function ConfirmTradingNum()

	local tradingProcess = LuaUI.GetTradingProcess();
	local numEdit = UIManager:FindUIEditBox("NumberOfTradingItem");
	 
	if tradingProcess ~= nil and numEdit ~= nil then
		local value = numEdit:GetInt();
		
		if value > 0  and value <= LuaUI.GetMaxStack(_G.GivenItemOrgIndex + 24 * _G.BagState) and _G.PlacedItemIndex >= 0 then
			
			tradingProcess:SetItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex, value);
			
			local IconName = "";
				
							
		 	if _G.GivenItemOrgIndex < 10 then	
				IconName = "ItemSlot00".._G.GivenItemOrgIndex.."Icon";
				
			elseif _G.GivenItemOrgIndex >= 10 and _G.GivenItemOrgIndex < 24 then
				IconName = "ItemSlot0".._G.GivenItemOrgIndex.."Icon";
				
			end
				
			--LuaUI.CopyImageForUIImage(IconName,"GivenItem00".._G.PlacedItemIndex.."Icon");
			
			local aniIcon = UIManager:FindUIAniIcon(IconName);
			local tradingAniIcon = UIManager:FindUIAniIcon("TradingItem00".._G.PlacedItemIndex.."Icon");
			
			if aniIcon ~= nil and tradingAniIcon ~= nil then
			    tradingAniIcon:ClearAniData();
			    tradingAniIcon:SetAnimationData(aniIcon);
			end
			
			local orgIcon = UIManager:GetUIItem(IconName);
			local text = UIManager:FindUIStaticText("TradingItem00".._G.PlacedItemIndex.."Stack");
			local textTemp = UIManager:GetUIItem("TradingItem00".._G.PlacedItemIndex.."Stack");
			local image = UIManager:GetUIItem("TradingItem00".._G.PlacedItemIndex.."Icon");
			if text ~= nil and image ~= nil and orgIcon ~= nil then
				--[[if value == 1 then
					text:LuaSetText("");
				else
					text:LuaSetText(value.."");
				end
				text:Redraw();
				textTemp:SetEnable(true);
				UIManager:AddDirtyUIItem(textTemp);
				    ]]
				--orgIcon:SetRenderStyle(1);
				--UIManager:AddDirtyUIItem(orgIcon);
				image:SetEnable(true);
				UIManager:AddDirtyUIItem(image);
			end
			
		end 
	end
	CloseTradingCount();
end

function SetKeyInTradingMoney()
	local tradingProcess = LuaUI.GetTradingProcess();
	if tradingProcess:IsConfirm() then
		do return end;
	end
	UIManager:SetKeyInItem("TradingMoney");
end

function TradingItemMove(index)
	local tradingProcess = LuaUI.GetTradingProcess();
	if tradingProcess:IsConfirm() == true then
		do return end;
	end
	local panel = UIManager:GetUIItem("TradingMovingPanel"); 
	if panel ~= nil then
		local iconName;
		if index >= 0 and index < 6 then
			iconName = "TradingItem00"..index.."Icon";
			_G.DragItemIndex = index;
			
			local orgIcon = UIManager:GetUIItem(iconName);
			if orgIcon ~= nil then
				--LuaUI.CopyImageForUIImage(iconName, "GivenMovingIcon");
				local aniIcon = UIManager:FindUIAniIcon(iconName);
				local tradingAniIcon = UIManager:FindUIAniIcon("TradingMovingIcon");

				if aniIcon ~= nil and tradingAniIcon ~= nil then
				    tradingAniIcon:ClearAniData();
				    tradingAniIcon:SetAnimationData(aniIcon);
				end
				UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
				orgIcon:SetRenderStyle(1);
			end
						
		end
		panel:SetEnable(true);
		UIManager:AddDirtyUIItem(panel);
		ModifyLockState();
	end

end

function ResetTradingDrag()

	UIManager:ResetDragItem();
	UIManager:StopDrag();
	 
	local iconName;
	if _G.DragItemIndex >= 0 then
	 	iconName = "TradingItem00".._G.DragItemIndex.."Icon";
	elseif _G.DragPartnerIndex >= 0 then 
		iconName = "TradingPartner00".._G.DragPartnerIndex.."Icon";
	elseif _G.DragPetIndex >= 0 then
		iconName = "TradingPet00".._G.DragPetIndex.."Icon";
	end
	
	local orgIcon = UIManager:GetUIItem(iconName);
	if orgIcon ~= nil then
		orgIcon:SetRenderStyle(0);
		UIManager:AddDirtyUIItem(orgIcon);
	end
	
	local panel = UIManager:GetUIItem("TradingMovingPanel"); 
	if panel ~= nil then
		panel:SetPos(0,0);
		panel:SetEnable(false);
	end
	
	_G.DragItemIndex = -1;
	_G.DragPartnerIndex = -1;
	_G.DragPetIndex = -1;
end

function TradingItemMove000()
	TradingItemMove(0);
end

function TradingItemMove001()
    TradingItemMove(1);
end

function TradingItemMove002()
	TradingItemMove(2);
end

function TradingItemMove003()
	TradingItemMove(3);
end

function TradingItemMove004()
	TradingItemMove(4);
end

function TradingItemMove005()
	TradingItemMove(5);
end

function TradingItemMove006()
	TradingItemMove(6);
end


function TradingEndDrag(value)
	if value ~= 3 then
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();

		local iconName;
		local stackName;
		local levelName;

		if _G.DragItemIndex >= 0 then
		 	iconName = "TradingItem00".._G.DragItemIndex.."Icon";
		 	stackName = "TradingItem00".._G.DragItemIndex.."Stack";
		 	levelName = "TradingItem00".._G.DragItemIndex.."Lvl";
		elseif _G.DragPartnerIndex >= 0 then
			iconName = "TradingPartner00".._G.DragPartnerIndex.."Icon";
			levelName = "TradingPartner00".._G.DragPartnerIndex.."Lvl";
		elseif _G.DragPetIndex >= 0 then
			iconName = "TradingPet00".._G.DragPetIndex.."Icon";
			levelName = "TradingPet00".._G.DragPetIndex.."Lvl";
		end

		local itemMenu = UIManager:GetUIItem("ItemMenuPanel");

		if itemMenu ~= nil then
		    local itemx = itemMenu:GetX();
		    local itemy = itemMenu:GetY();

			local diffx = mx - itemx;
			local diffy = my - itemy;
			local tradingProcess = LuaUI.GetTradingProcess();

            if tradingProcess ~= nil then
				if diffx >= 25 and diffx < 272 and 
					diffy > 281 and diffy < 446 and  _G.DragItemIndex >= 0 then
					tradingProcess:RemoveItem(_G.DragItemIndex);
					
					local img = UIManager:GetUIItem(iconName);
					local stack = UIManager:GetUIItem(stackName);
					local level = UIManager:GetUIItem(levelName);
		
					if img ~= nil and stack ~= nil and level ~= nil then
						img:SetRenderStyle(0);
						img:SetEnable(false);
						stack:SetEnable(false);
						level:SetEnable(false);
					end
					
				elseif diffx >= 0 and diffx < itemMenu:GetWidth() and 
					diffy > 0 and diffy < itemMenu:GetHeight() then
					
					if _G.DragPartnerIndex >= 0 or _G.DragPetIndex >= 0 then
					
						if _G.DragPartnerIndex >= 0 then
							tradingProcess:RemovePartner(_G.DragPartnerIndex);
						elseif _G.DragPetIndex >= 0 then
							tradingProcess:RemovePet(_G.DragPetIndex);
						end
						
						local img = UIManager:GetUIItem(iconName);
						local level = UIManager:GetUIItem(levelName);
			
						if img ~= nil and level ~= nil then
							img:SetRenderStyle(0);
							img:SetEnable(false);
							level:SetEnable(false);
						end
					end
				end
		    end
		end
	end
	ModifyLockState();
	ResetTradingDrag();
end

function TradingPartnerMove(index)
	local tradingProcess = LuaUI.GetTradingProcess();
	if tradingProcess:IsConfirm() == true then
		do return end;
	end
	local panel = UIManager:GetUIItem("TradingMovingPanel"); 
	if panel ~= nil then
		local iconName;
		if index >= 0 and index < 3 then
			iconName = "TradingPartner00"..index.."Icon";
			_G.DragPartnerIndex = index;
			
			local orgIcon = UIManager:GetUIItem(iconName);
			if orgIcon ~= nil then
				--LuaUI.CopyImageForUIImage(iconName, "GivenMovingIcon");
				local aniIcon = UIManager:FindUIAniIcon(iconName);
				local tradingAniIcon = UIManager:FindUIAniIcon("TradingMovingIcon");

				if aniIcon ~= nil and tradingAniIcon ~= nil then
				    tradingAniIcon:ClearAniData();
				    tradingAniIcon:SetAnimationData(aniIcon);
				end
				UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
				orgIcon:SetRenderStyle(1);
			end
		end
		panel:SetEnable(true);
		UIManager:AddDirtyUIItem(panel);
		ModifyLockState();
	end

end

function TradingPartnerMove000()
	TradingPartnerMove(0);
end

function TradingPartnerMove001()
	TradingPartnerMove(1);
end

function TradingPartnerMove002()
	TradingPartnerMove(2);
end

function TradingPetMove(index)
	local tradingProcess = LuaUI.GetTradingProcess();
	if tradingProcess:IsConfirm() == true then
		do return end;
	end
	local panel = UIManager:GetUIItem("TradingMovingPanel"); 
	if panel ~= nil then
		local iconName;
		if index >= 0 and index < 3 then
			iconName = "TradingPet00"..index.."Icon";
			_G.DragPetIndex = index;
			
			local orgIcon = UIManager:GetUIItem(iconName);
			if orgIcon ~= nil then
				--LuaUI.CopyImageForUIImage(iconName, "GivenMovingIcon");
				local aniIcon = UIManager:FindUIAniIcon(iconName);
				local tradingAniIcon = UIManager:FindUIAniIcon("TradingMovingIcon");

				if aniIcon ~= nil and tradingAniIcon ~= nil then
				    tradingAniIcon:ClearAniData();
				    tradingAniIcon:SetAnimationData(aniIcon);
				end
				UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
				orgIcon:SetRenderStyle(1);
			end
		end
		panel:SetEnable(true);
		UIManager:AddDirtyUIItem(panel);
		ModifyLockState();
	end

end


function TradingPetMove000()
	TradingPetMove(0);
end

function TradingPetMove001()
	TradingPetMove(1);
end

function TradingPetMove002()
	TradingPetMove(2);
end

function SetTradingLock()
	local checkBox = UIManager:FindUICheckBox("TradingLock");
	local tradingProcess = LuaUI.GetTradingProcess();
	local img = UIManager:GetUIItem("TradingLockedImage");
	
	if checkBox ~= nil and tradingProcess ~= nil and img ~= nil then
		if tradingProcess:IsConfirm() == true then
			checkBox:SetState(true);
			local checkBoxItem = UIManager:GetUIItem("TradingLock");
			if checkBoxItem ~= nil then
				UIManager:AddDirtyUIItem(checkBoxItem);
			end
            			
		else
			tradingProcess:SetLock(checkBox:GetState());
			img:SetEnable(checkBox:GetState());
		end
	end
	
end

function TradingMoneyStartTimer()
	local gmoney = UIManager:GetUIItem("TradingMoney");
	local tradingProcess = LuaUI.GetTradingProcess();
	local checkBox = UIManager:FindUICheckBox("TradingLock");
	if gmoney ~= nil and tradingProcess and checkBox ~= nil then
	    gmoney:SetEnableTimeOutEvent(true);
		gmoney:ResetTimer();
		tradingProcess:CheckMoney();
        ModifyLockState();
	end
	
end


function ModifyLockState()
    local checkBox = UIManager:FindUICheckBox("TradingLock");
    local tradingProcess = LuaUI.GetTradingProcess();
    if tradingProcess ~= nil and checkBox ~= nil then
    	if checkBox:GetState() == true then
		    checkBox:SetState(false);
		    tradingProcess:SetLock(checkBox:GetState());
		    local img = UIManager:GetUIItem("TradingLockedImage");
		    if img ~= nil then
				img:SetEnable(checkBox:GetState());
			end
			local checkBoxItem = UIManager:GetUIItem("TradingLock");
			if checkBoxItem ~= nil then
				UIManager:AddDirtyUIItem(checkBoxItem);
			end
            
		end
	end
end

function TradingShowMoney()
    local gmoney = UIManager:GetUIItem("TradingMoney");
    local tradingProcess = LuaUI.GetTradingProcess();
	if gmoney ~= nil and tradingProcess ~= nil then
	    gmoney:SetEnableTimeOutEvent(false);
	    tradingProcess:SetMoney();
	end
end

function ConfirmTrading()
    local tradingProcess = LuaUI.GetTradingProcess();
	if tradingProcess ~= nil then
	    tradingProcess:ConfirmTrading();
	end
	local confirmButton = UIManager:FindUIButton("TradingYes");
	local confirmButtonItem = UIManager:GetUIItem("TradingYes");
	if confirmButton ~= nil and confirmButtonItem ~= nil then
	    confirmButton:SetDisableState();
	    UIManager:AddDirtyUIItem(confirmButtonItem);
	    local img = UIManager:GetUIItem("TradingLockedImage");
	    if img ~= nil then
			img:SetDetectImage(true);
			ResetKeyIn();
		end
	end
end

function TradingItemMouseIn()
	local pPanel = UIManager:GetUIItem("TradingPanel");
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(true);
		pPanel:ResetTimer();
	end
	_G.TradingTimeOutFlag = 1;
end

function TradingItemMouseOut()
	if _G.TradingTimeOutFlag == 0 then
		local pPanel = UIManager:GetUIItem("TradingPanel");
		if pPanel ~= nil then
			pPanel:SetEnableTimeOutEvent(false);
		end
		CloseTradingDetail();
	end
end

function TradingPartnerMouseIn()
	local pPanel = UIManager:GetUIItem("TradingPanel");
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(true);
		pPanel:ResetTimer();
	end
	_G.TradingTimeOutFlag = 2;
end

function TradingPetMouseIn()
	local pPanel = UIManager:GetUIItem("TradingPanel");
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(true);
		pPanel:ResetTimer();
	end
	_G.TradingTimeOutFlag = 3;
end

function TradingDetailPopupTimeOut()
	local panel = UIManager:FindUIPanel("TradingPanel");
	local tradingProcess = LuaUI.GetTradingProcess();
	if tradingProcess ~= nil and panel ~= nil then
	
		local cursorX = UIManager:GetCursorX();
	    local cursorY = UIManager:GetCursorY();
	    if cursorX > panel:GetX() and cursorY > panel:GetY() and cursorX < (panel:GetX() + panel:GetWidth()) and cursorY < (panel:GetY() + panel:GetHeight()) then
			if _G.TradingTimeOutFlag == 1 then
				tradingProcess:ShowItemDetail();
			elseif _G.TradingTimeOutFlag == 2 then
				tradingProcess:ShowPartnerDetail();
			elseif _G.TradingTimeOutFlag == 3 then
				tradingProcess:ShowPetDetail();
			end
		else
		    CloseTradingDetail();
		end
		
		panel:SetEnableTimeOutEvent(false);
	end
	_G.TradingTimeOutFlag = 0;
end

function CloseTradingDetail()
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

function SetmoneyColour()
	local tradingProcess = LuaUI.GetTradingProcess();
	if tradingProcess ~= nil then
	    tradingProcess:ShowMoneyColour();
	end
end