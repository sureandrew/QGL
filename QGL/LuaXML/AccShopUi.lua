function FunOpenAccShop()
	local panel = UIManager:GetUIItem("AccShopPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\AccShopUi.xml");
		local pAccShopProcess = LuaUI.GetAccShopProcess();
		if pAccShopProcess ~= nil then
			pAccShopProcess:ShowAccShop();
		end
		PlayOpenInterfaceEffect();
	else
		FunAccShopClose();
	end
end

function FunSelectAccShop00()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:SelectAccShopItem(0);
	end
end

function FunSelectAccShop01()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:SelectAccShopItem(1);
	end
end

function FunSelectAccShop02()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:SelectAccShopItem(2);
	end
end

function FunSelectAccShop03()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:SelectAccShopItem(3);
	end
end

function FunSelectAccShop04()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:SelectAccShopItem(4);
	end
end

function FunSelectAccShop05()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:SelectAccShopItem(5);
	end
end

function FunSelectAccShop06()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:SelectAccShopItem(6);
	end
end

function FunSelectAccShop07()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:SelectAccShopItem(7);
	end
end

function FunSelectAccShop08()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:SelectAccShopItem(8);
	end
end

function FunSelectAccShop09()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:SelectAccShopItem(9);
	end
end

function FunAccShopClose()
	local panel = UIManager:GetUIItem("AccShopPanel");
	if panel ~= nil then
		panel:SetEnable(false);
		UIManager:AddDirtyUIItem(panel);
		UIManager:DeletePanel("AccShopPanel");
	end
end

function FunAccShopPrevPage()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:AccShopPrevPage();
	end
end

function FunAccShopNextPage()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:AccShopNextPage();
	end
end

function FunAccShopBuySelItem()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:AccShopBuyItem();
	end
end

function FunAccShopMouseOut()
	CloseItemDetail(0,0);
end

function FunAccShopMouseOver00()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:OnMouseMoveOver(0);
	end
end

function FunAccShopMouseOver01()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:OnMouseMoveOver(1);
	end
end

function FunAccShopMouseOver02()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:OnMouseMoveOver(2);
	end
end

function FunAccShopMouseOver03()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:OnMouseMoveOver(3);
	end
end

function FunAccShopMouseOver04()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:OnMouseMoveOver(4);
	end
end

function FunAccShopMouseOver05()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:OnMouseMoveOver(5);
	end
end

function FunAccShopMouseOver06()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:OnMouseMoveOver(6);
	end
end

function FunAccShopMouseOver07()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:OnMouseMoveOver(7);
	end
end

function FunAccShopMouseOver08()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:OnMouseMoveOver(8);
	end
end

function FunAccShopMouseOver09()
	local pAccShopProcess = LuaUI.GetAccShopProcess();
	if pAccShopProcess ~= nil then
		pAccShopProcess:OnMouseMoveOver(9);
	end
end
