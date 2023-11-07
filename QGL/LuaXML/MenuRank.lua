function OpenRankPanel()
	local panel = UIManager:FindUIPanel("RankPanel");
	
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuRank.xml");
		UIManager:SetKeyInItem("RankSerach");
	end
end

function CloseRankPanel()
	UIManager:StopDrag();
	UIManager:SetKeyInItem("EditLine");
	UIManager:DeletePanel("RankPanel");
end

function RankPanelUp()
	local menu = LuaUI.GetMenuRankProcess();
	menu:PageUp();
end

function RankPanelDown()
	local menu = LuaUI.GetMenuRankProcess();
	menu:PageDown();
end

function RankPanelScrollLua()
	local menu = LuaUI.GetMenuRankProcess();
	menu:RankScroll();
end

function SelectRankItem()
	local mouseX = UIManager:GetCursorX();
	local mouseY = UIManager:GetCursorY();
	local index = -1;
	local panel = UIManager:GetUIItem("RankPanel");
	if panel ~= nil then
		local relX = UIManager:GetCursorX() - panel:GetX();
		local relY = UIManager:GetCursorY() - panel:GetY();
		local TargetX = 9;
		local TargetY = 95;
		local width = 460;
		local height = 26;
		for i = 0, 9 do
			if	relX >= TargetX and
				relX <= TargetX + width and
				relY >= TargetY and
				relY <= TargetY + height then
				
				index = i;
				local imgName = "RankHightLight";
				local UIImage = UIManager:GetUIItem("RankHightLight");
				if UIImage ~= nil then
						UIImage:SetOffsetXY(TargetX, TargetY)
						UIImage:SetEnable(true);
				end
				local menu = LuaUI.GetMenuRankProcess();
				menu:SetSelIndex(index);
				break;
			end
			TargetY = TargetY + height;
		end
	end
	
	if index == -1 then
		local UIImage = UIManager:GetUIItem("RankHightLight");
		if UIImage ~= nil then
			UIImage:SetEnable(false);
		end
	end
end

function FindRankContent()
	local editline = UIManager:FindUIEditBox("RankSerach");
	if editline ~= nil then
		local menu = LuaUI.GetMenuRankProcess();
		menu:FindRank(editline:GetText());
	end
end

function RankSubSel1Lua()
	local menu = LuaUI.GetMenuRankProcess();
	menu:SetRankSubType(1);
end

function RankSubSel2Lua()
	local menu = LuaUI.GetMenuRankProcess();
	menu:SetRankSubType(2);
end

function RankSubSel3Lua()
	local menu = LuaUI.GetMenuRankProcess();
	menu:SetRankSubType(3);
end

function RankSubSel4Lua()
	local menu = LuaUI.GetMenuRankProcess();
	menu:SetRankSubType(4);
end

function RankSubSel5Lua()
	local menu = LuaUI.GetMenuRankProcess();
	menu:SetRankSubType(5);
end

function RankSubSel6Lua()
	local menu = LuaUI.GetMenuRankProcess();
	menu:SetRankSubType(6);
end

function RankSubSel7Lua()
	local menu = LuaUI.GetMenuRankProcess();
	menu:SetRankSubType(7);
end

function RankSubSel8Lua()
	local menu = LuaUI.GetMenuRankProcess();
	menu:SetRankSubType(8);
end

function RankPanelWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("RankPanelScroll"):OnMouseWheel(UIManager:GetUIItem("RankPanelScroll"):GetX()
		,UIManager:GetUIItem("RankPanelScroll"):GetY(),pZ);
end
