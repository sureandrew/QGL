local bShopItemDetailTimeOut = false;
local nShopDragItemIndex = -1;

function ShopReInitParam()
	bShopItemDetailTimeOut = false;
	nShopDragItemIndex = -1;
end

function ShopOpenCharItemMenu(pX, pY)
	-- open character item menu
	local itempanel = UIManager:FindUIPanel("ItemMenuPanel");
	if itempanel ~= nil then
		if itempanel:IsEnable() == false then
			itempanel:SetEnable(true);
			itempanel:SetPos(400, 48);

			UIManager:RenderAll();
			--UIManager:AddDirtyUIItem(itempanel);
		end
	else	
		LuaUI.AddXMLFile2UIManger("Data\\MenuItem.xml");
		
		itempanel = UIManager:FindUIPanel("ItemMenuPanel");
		if itempanel ~= nil then
			itempanel:SetEnable(true);
			itempanel:SetPos(400, 48);
		end
		
		ItemCharacter(0, 0);
		ItemCurrentBag(0, 0);
	end
end

function CloseShopMenu(pX, pY)
	ShopReInitParam();
	LuaUI.ShopCloseMainMenu();
	CloseItemDetail(0, 0);
	SICB_AnsNo(0, 0);
end

function BuyShopItem(pX, pY)
	local mx = UIManager:GetCursorX();
	local my = UIManager:GetCursorY();
			
	local slotindex = GetShopItemSlotIndex(mx, my);
	if slotindex ~= -1 then
		ShopEnterBuyItemCount(slotindex);
	end
end

function ShopSellItem(pX, pY)
	LuaUI.ShopOnClickSellButton();
end

function ShopRepairItem(pX, pY)
	--TalkPanel(0, 0, "test");
	--[[
	local test = UIManager:FindUIPanel("123");
	if test == nil then
		TalkPanel(0, 0, "this is 0");
	else if test == 0 then
		TalkPanel(0, 0, "Nil");
	end
	]]
end

function GetShopItemSlotIndex(MouseX, MouseY)
	local uim = UIManager:GetUIItem("SIS000Icon");
	if uim ~= nil then
		
		local leftupX = uim:GetX();
		local leftupY = uim:GetY();
		--[[
		if leftupX >= 0 then
			--local tempa = leftupX;
			TalkPanel(0,0, string.format("large :%d", leftupX));
			--leftupX = -tempa;
		else
			leftupX = -leftupX;
			TalkPanel(0,0, string.format("small :%d", leftupX));
		end
		
		if leftupY < 0 then
			leftupY = -leftupY;
		end
		]]
		local x = MouseX - leftupX;
		if x < 1 then 
			x = 0;
		end

		local y = MouseY - leftupY;
		if y < 1 then
			y = 0;
		end

		if ( x > 0 ) and ( x < (41 * 6) ) and ( y > 0 ) and ( y < (41 * 8)) then
			return (LuaUI.Div(y, 41) * 6 + LuaUI.Div(x, 41));
		else
			return -1;
		end
	end
	return -1;
end

function ShopItemDetail(pMouseIn)
	if bShopItemDetailTimeOut == true then
	
		local detail = UIManager:GetUIItem("ItemDetailMenuPanel");
		if detail == nil then
			LuaUI.AddXMLFile2UIManger("Data\\ItemDetail.xml");
			detail = UIManager:GetUIItem("ItemDetailMenuPanel");
			
			if detail == nil then
				return;
			else
				detail:SetEnable(false);
			end
			local mx = UIManager:GetCursorX();
			local my = UIManager:GetCursorY();
			
			local slotindex = GetShopItemSlotIndex(mx, my);
			if slotindex ~= -1 then 
				--detail:SetEnable(true);
				LuaUI.UpdateShopItemDetail(slotindex);
				
				-- reset detail pos 
				local height = detail:GetHeight();
				local width = detail:GetWidth();
			
				local reset_x = mx;
				local reset_y = my;
			
				if mx < 400 then
					reset_x = mx + 40;
				else
					reset_x = mx - width - 40; 
				end
			
				if my < 300 then
					reset_y = my + 5;
				else
					reset_y = my - height - 5;
				end
				
				if (reset_y + height) > 599 then
					reset_y = reset_y - height;
				end
			
				if reset_x < 0 then
					reset_x = 0;
				end
				if reset_y < 0 then
					reset_y = 0;
				end
				--[[
				if mx + width > 799  then
					reset_x = mx - width;
				end
			
				if my + height > 599  then
					reset_y = my - height;
				end
				]]--
			
				detail:SetPos(reset_x, reset_y);  
				UIManager:RenderAll();	
			end
		end
		--ItemDetail(pMouseIn, GetShopItemSlotIndex(UIManager:GetCursorX(), UIManager:GetCursorY()));
	end	
end

function ShowShopSelectRect()
	local image = UIManager:GetUIItem("ShopSelectRect");
	local mainpanel = UIManager:GetUIItem("ShopMenuPanel");
	if ( image ~= nil ) and ( mainpanel ~= nil ) then
		--local leftupx = mainpanel:GetX() + image:GetOffsetX();
		--local leftupy = mainpanel:GetY() + image:GetOffsetY();
		local leftupx = image:GetX();
		local leftupy = image:GetY();
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();
		if ( ( mx > leftupx ) and ( mx < (leftupx + 40 ) ) and
			 ( my > leftupy ) and ( my < (leftupy + 40) ) ) then
			
			image:SetEnable(true);
			UIManager:AddDirtyUIItem(image);
			if bShopItemDetailTimeOut == false then
				bShopItemDetailTimeOut = true;
				ShopItemDetail(1);
				--bShopItemDetailTimeOut = true;
			end
		else
			image:SetEnable(false);
			bShopItemDetailTimeOut = false;
		end
	end
end

function ShopItemMouseOut()
	local selectrect = UIManager:GetUIItem("ShopSelectRect");
	if selectrect ~= nil then
		selectrect:SetEnable(false);
		UIManager:AddDirtyUIItem(selectrect);
	end
	CloseItemDetail(0, 0);
	bShopItemDetailTimeOut = false;
end

function ShopItemOver()
	local panel = UIManager:GetUIItem("ShopMenuPanel");
	if panel == nil then
		return;
	end
	
	local itemPos = GetShopItemSlotIndex(UIManager:GetCursorX(), UIManager:GetCursorY());
	if itemPos == -1 then
		return;
	end
	
	local icon = UIManager:GetUIItem(string.format("SIS%03dIcon", itemPos));
	if icon == nil then
		return;
	end
	
	-- set select rect pos 
	local selectrect = UIManager:GetUIItem("ShopSelectRect");
	if selectrect ~= nil then
		if selectrect:IsEnable() == false then
			selectrect:SetEnable(true);	
			UIManager:AddDirtyUIItem(selectrect);
		end
		if (selectrect:GetOffsetX() ~= (icon:GetOffsetX() - 1 )) or 
			(selectrect:GetOffsetY() ~= (icon:GetOffsetY() - 1 )) then	
			selectrect:SetOffsetXY(icon:GetOffsetX() - 1, icon:GetOffsetY() - 1);
			UIManager:AddDirtyUIItem(selectrect);
		end
	end
	
	-- need to show shop item details
	if bShopItemDetailTimeOut == true then
		ShopItemDetail(1);
	end
end

--  //////           confirm   dialogue  operation     /////////
function SCB_ConfirmAction(pos)
	--ShopConfirm
	local confirm = UIManager:FindUIPanel("ShopConfirm");
	if confirm == nil then
		LuaUI.AddXMLFile2UIManger("Data\\ShopMenu.xml");
	end
	
	LuaUI.ShopOpenConfirmBox(pos);
end

function SCB_ConfirmAnsNo(pX, pY)
	local confirm = UIManager:GetUIItem("ShopConfirm");
	if confirm ~= nil then
		confirm:SetEnable(false);
		UIManager:ClearMonoPanel();
	end
end

function SCB_ConfirmAnsYes(pX, pY)
	
		LuaUI.ShopConfirmAction();
		SCB_ConfirmAnsNo(0, 0);
	
end

-- ///////////		shop  item count dialogue operation		/////////////// 
function ShopEnterSellItemCount(pos)
	local countpanel = UIManager:FindUIPanel("ShopItemCountBox");
	if ( countpanel == nil ) then
		LuaUI.AddXMLFile2UIManger("Data\\ShopMenu.xml");
	end
	
	LuaUI.ShopEnterItemCount(true, pos);
end

function ShopEnterBuyItemCount(pos)
	local countpanel = UIManager:FindUIPanel("ShopItemCountBox");
	if ( countpanel == nil ) then
		LuaUI.AddXMLFile2UIManger("Data\\ShopMenu.xml");
	end
	
	ShopItemMouseOut();
	LuaUI.ShopEnterItemCount(false, pos);
end

function SICB_AnsYes(pX, pY)
	local editBox = UIManager:FindUIEditBox("SICB_EditBox");
	if editBox ~= nil  then
		if editBox:GetInt() == 0 then
			editBox:SetText("1");
			editBox:Redraw();
			editBox:End();
			LuaUI.ShopUpdatePrice();
			return 
		end
		SICB_AnsNo(0, 0);
		
		--SCB_ConfirmAction(-1);
		LuaUI.ShopConfirmAction();
	end
end

function SICB_AnsNo(pX, pY)
	local countpanel = UIManager:GetUIItem("ShopItemCountBox");
	if countpanel ~= nil then
		UIManager:SetKeyInItem("EditLine");
	
		countpanel:SetEnable(false);
		UIManager:ClearMonoPanel();	
	end
end

function SICB_SetMax(pX, pY)
	LuaUI.ShopSetItemCount(true);
end

function SICB_SetMin(pX, pY)
	LuaUI.ShopSetItemCount(false);
end

function SICB_Increase(pX, pY)
	LuaUI.ShopAdjustItemCount(true);
end

function SICB_Decrease(pX, pY)
	LuaUI.ShopAdjustItemCount(false);
end

function ShopCancelAction()
	LuaUI.ShopResetAction();
end

function ResetShopItemDrapPanel()
	local dragpanel = UIManager:GetUIItem("ShopItemDragPanel");
		
	if dragpanel ~= nil then
		dragpanel:SetPos(0,0);
		dragpanel:SetEnable(false);
		dragpanel:SetNotDetected(false);
	end
end

function ShopItemResetDrag()
 	UIManager:ResetDragItem();
	UIManager:StopDrag();
	ResetShopItemDrapPanel();

	UIManager:RenderAll();
end

function DragShopItem()
	local mx = UIManager:GetCursorX();
	local my = UIManager:GetCursorY();
			
	local slotindex = GetShopItemSlotIndex(mx, my);
	if slotindex ~= -1 then 
		-- drag image
		local dragpanel = UIManager:GetUIItem("ShopItemDragPanel");
		if dragpanel ~= nil then
			nShopDragItemIndex = slotindex;
			LuaUI.ShopDragItemAction(nShopDragItemIndex);
		end
	end
end

function ShopDetectDragPos()
	local itempanel = UIManager:GetUIItem("ItemMenuPanel");
	if ( itempanel ~= nil ) and ( itempanel:IsEnable() == true ) then
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();
				
		local ipx = itempanel:GetX();
		local ipy = itempanel:GetY();
		local width = itempanel:GetWidth();
		local height = itempanel:GetHeight();
		
		if ((mx >= ipx) and (mx <= (ipx + width))) and ((my >= ipy) and (my <= (ipy + height))) then
			if nShopDragItemIndex ~= -1 then
				return true;
			end
		end
	end
	
	return false;
end

function ShopItemDragEnd(dragtype)
	if dragtype == 3 then
		ResetShopItemDrapPanel();
	else
		local bresult = ShopDetectDragPos();
		ResetShopItemDrapPanel();
		if ( bresult == true ) then
			ShopEnterBuyItemCount(nShopDragItemIndex);
		end
		nShopDragItemIndex = -1;
	end
end

function SEB_OnKeyUp(code)
	LuaUI.ShopOnEditBoxKeyUp(code);
end
