function OpenWeaponsRankPanel()
	local panel = UIManager:GetUIItem("WeaponsRankPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\WeaponsRank.xml");
		--[[
		local Menu = LuaUI.GetMenuHelpProcess();
		Menu:HandleEvent(23);
		panel = UIManager:GetUIItem("WeaponsRankPanel");
		local bigMap = LuaUI.GetBigMap();
		if bigMap ~= nil then
		    bigMap:DrawButton();
		    bigMap:SetOpenBigMap(true);
		end
		if panel ~= nil then
		    UIManager:AddDirtyUIItem(panel);
		end
		PlayOpenInterfaceEffect();
		]]
	end
	
	WeaponsRankScrollUp();
end

function CloseWeaponsRankPanel()
	UIManager:StopDrag();
	local WeaponsRankProcess = LuaUI.GetWeaponsRankProcess();
	if WeaponsRankProcess ~= nil then
		WeaponsRankProcess:CloseWeaponsRank();
	end
	UIManager:DeletePanel("WeaponsRankPanel");
	UIManager: SetKeyInItem("EditLine");
end

function WeaponsRankScroll()
	local WeaponsRankProcess = LuaUI.GetWeaponsRankProcess();
	if WeaponsRankProcess ~= nil then
		WeaponsRankProcess:WeaponsRankScroll(2);
	end
end

function WeaponsRankScrollUp()
	local WeaponsRankProcess = LuaUI.GetWeaponsRankProcess();
	if WeaponsRankProcess ~= nil then
		WeaponsRankProcess:WeaponsRankScroll(0);	      	
	end
end

function WeaponsRankScrollDown()
	local WeaponsRankProcess = LuaUI.GetWeaponsRankProcess();
	if WeaponsRankProcess ~= nil then
		WeaponsRankProcess:WeaponsRankScroll(1);
	end
end

function WeaponsRankSearchList()
	LuaUI.WeaponsRankSearchList(0);
end

function WeaponsRankSearchListInChi()
    LuaUI.WeaponsRankSearchList(1);
end

function WeaponsRankChangeKeyInToChi()
    UIManager:SetKeyInItem("WeaponsRankSearchChiTextBox");
end

function SelectWeaponsRank(index)
	local WeaponsRankProcess = LuaUI.GetWeaponsRankProcess();
	if WeaponsRankProcess ~= nil then
		WeaponsRankProcess:SelectWeaponsRank(index);	      	
	end
end

function SelectWeaponsRank00()
	SelectWeaponsRank(0);
end

function SelectWeaponsRank01()
    SelectWeaponsRank(1);
end

function SelectWeaponsRank02()
	SelectWeaponsRank(2);
end

function SelectWeaponsRank03()
	SelectWeaponsRank(3);
end

function SelectWeaponsRank04()
	SelectWeaponsRank(4);
end

function SelectWeaponsRank05()
	SelectWeaponsRank(5);
end

function SelectWeaponsRank06()
	SelectWeaponsRank(6);
end

function SelectWeaponsRank07()
	SelectWeaponsRank(7);
end

function SelectWeaponsRank08()
	SelectWeaponsRank(8);
end

function SelectWeaponsRank09()
	SelectWeaponsRank(9);
end