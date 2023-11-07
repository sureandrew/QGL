function CloseTitle()
	UIManager:StopDrag();
	UIManager:DeletePanel("TitlePanel");
end

function OpenTitle()
	local panel = UIManager:GetUIItem("TitlePanel");
	
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\MenuTitle.xml");
	    local panel = UIManager:GetUIItem("TitlePanel");
	    if panel ~= hil then
			panel:SetPos(UIManager:GetCursorX(), UIManager:GetCursorY());
	    end
	    local Menu = LuaUI.GetTitleProcess();
	    Menu:RefreshMenu();
	else
	    CloseTitle();
	end
end

function TitleConfirmLua()
	local Menu = LuaUI.GetTitleProcess();
	Menu:ConfirmChange();
end

function TitlePanelScrollLua()
	local Menu = LuaUI.GetTitleProcess();
	Menu:Sroll();
end

function TitlePanelUp()
	local Menu = LuaUI.GetTitleProcess();
	Menu:PageUp();
end

function TitlePanelDown()
	local Menu = LuaUI.GetTitleProcess();
	Menu:PageDown();
end

function TitlePanelWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("TitlePanelScroll"):OnMouseWheel(UIManager:GetUIItem("TitlePanelScroll"):GetX()
		,UIManager:GetUIItem("TitlePanelScroll"):GetY(),pZ);
end

function SelectTitle()
	local SelIndex = -1;
	local mouseX = UIManager:GetCursorX();
	local mouseY = UIManager:GetCursorY();
	local panel = UIManager:GetUIItem("TitlePanel");
	if panel ~= nil then
		local relX = UIManager:GetCursorX() - panel:GetX();
		local relY = UIManager:GetCursorY() - panel:GetY();
		local TargetX = 9;
		local TargetY = 31;
		local width = 201;
		local height = 22;
		for i = 0, 9 do
			if	relX >= TargetX and
				relX <= TargetX + width and
				relY >= TargetY and
				relY <= TargetY + height then
				
				local textName = string.format("TitleName%d", i + 1);
			
				local UIText = UIManager:GetUIItem(textName);
				if UIText ~= nil and UIText:IsEnable() then
					local UIImage = UIManager:GetUIItem("TitleHighLight");
					if UIImage ~= nil then
						UIImage:SetOffsetXY(TargetX, TargetY)
						UIImage:SetEnable(true);
					end
					SelIndex = i;
				end
				break;
			end
		TargetY = TargetY + height;
		end
	end
	
	if SelIndex == -1 then
		local UIImage = UIManager:GetUIItem("TitleHighLight");
		if UIImage ~= nil then
			UIImage:SetEnable(false);
		end
	end
	local Menu = LuaUI.GetTitleProcess();
	Menu:SetSelectIndex(SelIndex);
end