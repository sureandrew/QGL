BIDragIndex=-1;
BIPrevCursorX=0;
BIPrevCursorY=0;

function OpenBattleItemMenu(pX, pY)
	local PopPanel = UIManager:FindUIPanel("BattleItemMenuPanel")
	if PopPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\BattleItemPanel.xml");
		
		UIManager:ClearMonoPanel();
		PopPanel = UIManager:FindUIPanel("BattleItemMenuPanel");
		UIManager:SetMonoPanel(PopPanel);

		LuaUI.UpdateBattleItemMenu();
		HideBattlePanel();
		HideBattlePartnerPanel();
		UIManager:RenderAll();
	else
		CloseBattleItemMenu(0, 0);
	end

end

function OpenWineItemMenu(pX, pY)
	local PopPanel = UIManager:FindUIPanel("BattleItemMenuPanel")
	if PopPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\BattleItemPanel.xml");
		
		UIManager:ClearMonoPanel();
		PopPanel = UIManager:FindUIPanel("BattleItemMenuPanel");
		UIManager:SetMonoPanel(PopPanel);

		LuaUI.UpdateBattleItemMenu(true);
		HideBattlePanel();
		HideBattlePartnerPanel();
		UIManager:RenderAll();
	else
		CloseBattleItemMenu(0, 0);
	end

end

function CloseBattleItemMenu(pX, pY)
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("BattleItemMenuPanel");
	if panel ~= nil then
		panel:SetEnable(false);
		UIManager:DeletePanel("BattleItemMenuPanel");

		SafeSetEnable("BattleItemDragDrop", false);
		UIManager:StopDrag();
		UIManager:DeletePanel("BattleItemDragDrop");
		CloseItemDetail(0, 0);

		UIManager:ClearMonoPanel();
		if LuaUI.IsBattlePartnerTurn() then
			ShowBattlePartnerPanel();
		else
			ShowBattlePanel();
		end
		
		LuaUI.OnCloseBattleItem();
		UIManager:RenderAll();
	end
end

function SafeSetEnable(pName, value)
	local panel = UIManager:GetUIItem(pName);
	if panel ~= nil then
		panel:SetEnable(value);
		UIManager:AddDirtyUIItem(panel);
	end	
end


function BIUse(pX, pY)
	
	HideBattlePanel();
	HideBattlePartnerPanel();
	LuaUI.BattleItemUse(GetSlotIndex(pX, pY));
	CloseBattleItemMenu(pX, pY);
	UIManager:RenderAll();
end

function BIDrag(pX, pY)
	BIDragIndex = GetSlotIndex(pX, pY);
	LuaUI.BattleItemDrag(BIDragIndex);
end

function BattleItemMove()
	local panel = UIManager:GetUIItem("BattleItemMenuPanel");
	if panel ~= nil then
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();
		local left = panel:GetX();
		local up = panel:GetY();
		local right = left + panel:GetWidth();
		local down = right + panel:GetHeight();
		if ( (mx >= left) and (mx <= right) and (my >= up) and (my <= down) ) then
			local result = GetSlotIndex(mx, my);

			if (result >= 0) and (result <= 23) and (BIDragIndex ~= -1) then
				
				LuaUI.BattleItemMove(BIDragIndex, result);
				SafeSetEnable("BattleItemDragDrop", false);
			end
		else
			SafeSetEnable("BattleItemDragDrop", false);
			-- drop item
		end
	end
	
end

function GetSlotIndex(MouseX, MouseY)
	local leftupX = UIManager:GetUIItem("BIS000Icon"):GetX();
	local leftupY = UIManager:GetUIItem("BIS000Icon"):GetY();
		
	local x = MouseX - leftupX;
	if x < 1 then 
		x = 0;
	end

	local y = MouseY - leftupY;
	if y < 1 then
		y = 0;
	end

	if ( x > 0 ) and ( y > 0 ) then
		return (LuaUI.Div(y, 41) * 6 + LuaUI.Div(x, 41));
	else
		return -1;
	end
end

function TestBITimeOut(pMouseIn)
end

function TestBIMove()
end

function TestBIOut()
end

function BattleItemDetail(pMouseIn)
	
		
	if PanelItemDetailState == 1 then
		ItemDetail(pMouseIn,GetSlotIndex(UIManager:GetCursorX(), UIManager:GetCursorY()));
	end
	
	
end

function BattleItemOver()
	local itemPos = GetSlotIndex(UIManager:GetCursorX(), UIManager:GetCursorY());
	local panel = UIManager:GetUIItem("BattleItemMenuPanel");
	local timer = UIManager:GetUIItem("BattleItemTimer");
	local image;
	if itemPos < 10 then
		image = UIManager:GetUIItem("BIS00"..itemPos.."Icon");
	else 
		image = UIManager:GetUIItem("BIS0"..itemPos.."Icon");
	end
	
	if image ~= nil then
		if timer ~= nil then
			timer:SetEnable(true);
			if timer:GetOffsetX() ~= (image:GetOffsetX() - 1) or timer:GetOffsetY() ~= (image:GetOffsetY() - 1) then
				timer:SetOffsetXY(image:GetOffsetX(), image:GetOffsetY());
			end
		end
		
			
		if PanelItemDetailState == 1 then
			ItemDetail(1, GetSlotIndex(UIManager:GetCursorX(), UIManager:GetCursorY()));
		end
	end

end

function BattleItemDetailMouseOut()
    CloseItemDetail(0, 0);
    PanelItemDetailState = 0;
    local timer = UIManager:GetUIItem("BattleItemTimer");
    timer:SetEnable(false);
    
    --[[local panel = UIManager:GetUIItem("ItemDetailMenuPanel");
    if panel ~= nil then
    	panel:SetEnable(false);
    end ]]
end

function ShowBattleItemDetail()

	local itemPos = GetSlotIndex(UIManager:GetCursorX(), UIManager:GetCursorY());
	local panel = UIManager:GetUIItem("BattleItemMenuPanel");
	local timer = UIManager:GetUIItem("BattleItemTimer");
	local image;
	if itemPos < 10 then
		image = UIManager:GetUIItem("BIS00"..itemPos.."Icon");
	else 
		image = UIManager:GetUIItem("BIS0"..itemPos.."Icon");
	end
	
	if (UIManager:GetCursorX() > (image:GetOffsetX() + panel:GetX() ) and UIManager:GetCursorX() < ( image:GetOffsetX() + 40 + panel:GetX() )
			and UIManager:GetCursorY() > (image:GetOffsetY() + panel:GetY() ) and UIManager:GetCursorY() < (image:GetOffsetY() + 40 + panel:GetY() )) then
	
		if PanelItemDetailState == 0 then
			ItemDetail(pMouseIn,GetSlotIndex(UIManager:GetCursorX(), UIManager:GetCursorY()));
			PanelItemDetailState = 1;
		else 
		    PanelItemDetailState = 0;
		end
	end
	
	
end
