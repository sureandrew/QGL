function OpenHelperPanel(pX, pY)
	local panel = UIManager:FindUIPanel("HelperPanel");

	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuHelp.xml");
		local Menu = LuaUI.GetMenuHelpProcess();
		Menu:CalOpenCount();
		Menu:RefreshMenu();
		Menu:HandleEvent(14);
	else
		CloseHelperPanel();
	end
end

function CloseHelperPanel()
	UIManager:StopDrag();
	UIManager:SetKeyInItem("EditLine");
	UIManager:DeletePanel("HelperPanel");
end

function OpenChange1Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(1);
end

function OpenChange2Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(2);
end

function OpenChange3Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(3);
end

function OpenChange4Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(4);
end

function OpenChange5Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(5);
end

function OpenChange6Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(6);
end

function OpenChange7Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(7);
end

function OpenChange8Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(8);
end

function OpenChange9Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(9);
end

function OpenChange10Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(10);
end

function OpenChange11Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(11);
end

function OpenChange12Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(12);
end

function OpenChange13Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(13);
end

function OpenChange14Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(14);
end

function OpenChange15Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(15);
end

function OpenChange16Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(16);
end

function OpenChange17Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(17);
end

function OpenChange18Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(18);
end

function OpenChange19Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(19);
end

function OpenChange20Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(20);
end

function OpenChange21Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(21);
end

function OpenChange21Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:IsOpenChange(23);
end

function ShowContent1Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(1);
	Menu:ResetSearch();
end

function ShowContent2Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(2);
	Menu:ResetSearch();
end

function ShowContent3Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(3);
	Menu:ResetSearch();
end

function ShowContent4Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(4);
	Menu:ResetSearch();
end

function ShowContent5Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(5);
	Menu:ResetSearch();
end

function ShowContent6Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(6);
	Menu:ResetSearch();
end

function ShowContent7Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(7);
	Menu:ResetSearch();
end

function ShowContent8Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(8);
	Menu:ResetSearch();
end

function ShowContent9Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(9);
	Menu:ResetSearch();
end

function ShowContent10Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(10);
	Menu:ResetSearch();
end

function ShowContent11Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(11);
	Menu:ResetSearch();
end

function ShowContent12Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(12);
	Menu:ResetSearch();
end

function ShowContent13Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(13);
	Menu:ResetSearch();
end

function ShowContent14Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(14);
	Menu:ResetSearch();
end

function ShowContent15Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(15);
	Menu:ResetSearch();
end

function ShowContent16Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(16);
	Menu:ResetSearch();
end

function ShowContent17Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(17);
	Menu:ResetSearch();
end

function ShowContent18Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(18);
	Menu:ResetSearch();
end

function ShowContent19Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(19);
	Menu:ResetSearch();
end

function ShowContent20Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(20);
	Menu:ResetSearch();
end

function ShowContent21Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(21);
	Menu:ResetSearch();
end

function ShowContent21Lua()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:ShowTextContent(23);
	Menu:ResetSearch();
end

function ShowContentLua()
	for i = 1, 21 do
		local name = string.format("HelperItemName%d", i);
		if UIManager:Detect(name, UIManager:GetCursorX(), UIManager:GetCursorY()) then
			local Menu = LuaUI.GetMenuHelpProcess();
			Menu:ShowTextContent(i);
			Menu:ResetSearch();
			break;
		end
	end
end

function FindHelperContent()
	local edit = UIManager:FindUIEditBox("HelperSerach");
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:SearchContent(edit:GetText());
end

function LeftHelperPanelScrollLua()
	local scroll = UIManager:FindUIScrollBarV("LeftHelperPanelScroll");
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:SetIndex(scroll:GetSelNum());
	Menu:RefreshMenu();
end

function LeftHelperPanelUp()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:PageUp();
	Menu:RefreshMenu();
end

function LeftHelperPanelDown()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:PageDown();
	Menu:RefreshMenu();
end

function RightHelperPanelScrollLua()
	local scroll = UIManager:FindUIScrollBarV("RightHelperPanelScroll");
	local chat = UIManager:FindUIChatPanel("HelperPanelContext");
	chat:SetRow(scroll:GetSelNum(), chat:RowListCount());
end

function RightHelperPanelUp()
	local chat = UIManager:FindUIChatPanel("HelperPanelContext");
	chat:UpRow();
end

function RightHelperPanelDown()
	local chat = UIManager:FindUIChatPanel("HelperPanelContext");
	chat:DownRow();
end

function HelperPanelWheel(pX, pY, pZ)
	local x = pX - UIManager:GetUIItem("HelperPanel"):GetX();
	if x <= 210 then
		UIManager:FindUIScrollBarV("LeftHelperPanelScroll"):OnMouseWheel(UIManager:GetUIItem("LeftHelperPanelScroll"):GetX()
		,UIManager:GetUIItem("LeftHelperPanelScroll"):GetY(),pZ);
	else
		UIManager:FindUIScrollBarV("RightHelperPanelScroll"):OnMouseWheel(UIManager:GetUIItem("RightHelperPanelScroll"):GetX()
		,UIManager:GetUIItem("RightHelperPanelScroll"):GetY(),pZ);
	end

end
