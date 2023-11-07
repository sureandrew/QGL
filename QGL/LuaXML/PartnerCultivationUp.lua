function OpenItemMenuForPartnerCultivationUp()

    local itemPanel = UIManager:GetUIItem("ItemMenuPanel");
     if itemPanel == nil then
    	BBItem();
    	itemPanel = UIManager:GetUIItem("ItemMenuPanel");
    end

	itemPanel:SetPos(400, 40);
	UIManager:AddDirtyUIItem(itemPanel);
end

function ClosePartnerCultivationUp()
	local PartnerCultivationUpPanel = UIManager:GetUIItem("PartnerCultivationUpPanel");
	local PartnerCultivationUpProcess = LuaUI.GetPartnerCultivationUpProcess();
	if PartnerCultivationUpPanel ~= nil and PartnerCultivationUpProcess ~= nil then
		PartnerCultivationUpProcess:ClosePartnerCultivationUp();
		UIManager:DeletePanel("PartnerCultivationUpPanel");
	end
end

function PartnerCultivationUpByPartner()
	local PartnerCultivationUpProcess = LuaUI.GetPartnerCultivationUpProcess();
	if PartnerCultivationUpProcess ~= nil then
		PartnerCultivationUpProcess:PartnerCultivationUpByPartner();
	end
end

function PartnerCultivationUpByItem()
	local PartnerCultivationUpProcess = LuaUI.GetPartnerCultivationUpProcess();
	if PartnerCultivationUpProcess ~= nil then
		PartnerCultivationUpProcess:PartnerCultivationUpByItem();
	end
end

function GetPartnerCultivationUpConsumeMoney(Type, cultivation)
	local money = 0;
	if Type == 0 then
		money = (cultivation + 1) * 1000 + 6000;
	else
		money = 100;
	end
	LuaUI.SetTempResult(money);
	return money;
end

function GetPartnerCultivationUpRate(cultivation)

	local UpRate = 0;
	if cultivation == 0 then
		UpRate = 100;
	elseif cultivation == 1 then
		UpRate = 85;
	elseif cultivation == 2 then
		UpRate = 75;
	elseif cultivation == 3 then
		UpRate = 60;
	elseif cultivation == 4 then
		UpRate = 20;
	elseif cultivation == 5 then
		UpRate = 30;
	elseif cultivation == 6 then
		UpRate = 30;
	elseif cultivation == 7 then
		UpRate = 3;
	elseif cultivation == 8 then
		UpRate = 7;
	elseif cultivation == 9 then
		UpRate = 1;
	elseif cultivation == 10 then
		UpRate = 0;
	end

	LuaUI.SetTempResult(UpRate);
	return UpRate;
end

function GetPartnerGrowLevel(grow, grow_d, grow_u)
	local grow_range = grow_u - grow_d + 1;
	local grow_level_4 = grow_d + math.floor(grow_range * 5 / 100);
    local grow_level_3 = grow_level_4 + math.floor(grow_range * 25 / 100);
    local grow_level_2 = grow_level_3 + math.floor(grow_range * 40 / 100);
    local grow_level_1 = grow_level_2 + math.floor(grow_range * 25 / 100);
	
	if grow <= grow_level_4 then
		LuaUI.SetTempResult(4);
        return 4;
    elseif grow > grow_level_4 and grow <= grow_level_3 then
		LuaUI.SetTempResult(3);
        return 3;
    elseif grow > grow_level_3 and grow <= grow_level_2 then
		LuaUI.SetTempResult(2);
        return 2;
    elseif grow > grow_level_2 and grow <= grow_level_1 then
		LuaUI.SetTempResult(1);
        return 1;
    elseif grow > grow_level_1 then
		LuaUI.SetTempResult(0);
        return 0;
    end
	LuaUI.SetTempResult(0);
	return 0;
end

function CheckPartnerCultivationUpItem(item_id, cultivation)
	
	local Re = 0;
	if item_id == 63036 then
		if cultivation >= 0 and cultivation <= 2 then
			Re = 0;
		else
			Re = 2;
		end
	elseif item_id == 63037 then
		if cultivation >= 3 and cultivation <= 5 then
			Re = 0;
		else
			Re = 2;
		end
	elseif item_id == 63038 then
		if cultivation >= 6 and cultivation <= 9 then
			Re = 0;
		else
			Re = 2;
		end
	else
		Re = 1;
	end
	LuaUI.SetTempResult(Re);
	return Re;
end