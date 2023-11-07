function FunOpenPointsShop()
	local panel = UIManager:GetUIItem("PointsShopPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\PointsShopUi.xml");
		--local pPointsShopProcess = LuaUI.GetPointsShopProcess();
		--if pPointsShopProcess ~= nil then
		--	pPointsShopProcess:ShowPointsShop();
		--end
		PlayOpenInterfaceEffect();
	else
		FunPointsShopClose();
	end
end

function FunSelectPointsShop00()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:SelectPointsShopItem(0);
	end
end

function FunSelectPointsShop01()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:SelectPointsShopItem(1);
	end
end

function FunSelectPointsShop02()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:SelectPointsShopItem(2);
	end
end

function FunSelectPointsShop03()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:SelectPointsShopItem(3);
	end
end

function FunSelectPointsShop04()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:SelectPointsShopItem(4);
	end
end

function FunSelectPointsShop05()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:SelectPointsShopItem(5);
	end
end

function FunSelectPointsShop06()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:SelectPointsShopItem(6);
	end
end

function FunSelectPointsShop07()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:SelectPointsShopItem(7);
	end
end

function FunSelectPointsShop08()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:SelectPointsShopItem(8);
	end
end

function FunSelectPointsShop09()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:SelectPointsShopItem(9);
	end
end

function FunPointsShopClose()
	local panel = UIManager:GetUIItem("PointsShopPanel");
	if panel ~= nil then
		panel:SetEnable(false);
		UIManager:AddDirtyUIItem(panel);
		UIManager:DeletePanel("PointsShopPanel");
	end
end

function FunPointsShopPrevPage()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:PointsShopPrevPage();
	end
end

function FunPointsShopNextPage()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:PointsShopNextPage();
	end
end

function FunPointsShopBuySelItem()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:PointsShopBuyItem();
	end
end

function FunPointsShopMouseOut()
	CloseItemDetail(0,0);
end

function FunPointsShopMouseOver00()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:OnMouseMoveOver(0);
	end
end

function FunPointsShopMouseOver01()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:OnMouseMoveOver(1);
	end
end

function FunPointsShopMouseOver02()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:OnMouseMoveOver(2);
	end
end

function FunPointsShopMouseOver03()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:OnMouseMoveOver(3);
	end
end

function FunPointsShopMouseOver04()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:OnMouseMoveOver(4);
	end
end

function FunPointsShopMouseOver05()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:OnMouseMoveOver(5);
	end
end

function FunPointsShopMouseOver06()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:OnMouseMoveOver(6);
	end
end

function FunPointsShopMouseOver07()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:OnMouseMoveOver(7);
	end
end

function FunPointsShopMouseOver08()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:OnMouseMoveOver(8);
	end
end

function FunPointsShopMouseOver09()
	local pPointsShopProcess = LuaUI.GetPointsShopProcess();
	if pPointsShopProcess ~= nil then
		pPointsShopProcess:OnMouseMoveOver(9);
	end
end
