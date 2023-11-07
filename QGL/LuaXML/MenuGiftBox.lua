DragGiftIndex = -1;
ConfirmGiftIndex = -1;
NeedResetConfirmGift = true;
NeedShowDetail = true;

function OpenGiftBox()
    local panel = UIManager:FindUIPanel("GiftBoxPanel");
	local itemPanel = UIManager:GetUIItem("ItemMenuPanel");
	local giftBoxProcess = LuaUI.GetGiftBoxProcess();
	if panel == nil and giftBoxProcess ~= nil then
		LuaUI.AddXMLFile2UIManger("Data\\GiftBox.xml");

		panel = UIManager:GetUIItem("GiftBoxPanel");
  		panel:SetPos(20, 40);
		UIManager:AddDirtyUIItem(panel);
        SelectGiftBoxItem();
		if itemPanel ~= nil then
			itemPanel:SetPos(400, 40);
			UIManager:AddDirtyUIItem(itemPanel);
		else
		    BBItem();
		    itemPanel = UIManager:GetUIItem("ItemMenuPanel");
		    if itemPanel ~= nil then
				itemPanel:SetPos(400, 40);
				UIManager:AddDirtyUIItem(itemPanel);
			end
		end
	else
		CloseGiftBox();
	end
end

function CloseGiftBox()
    UIManager:StopDrag();
    CloseOnlineShopDetail();
    local giftBoxProcess = LuaUI.GetGiftBoxProcess();
    if giftBoxProcess ~= nil then
    	giftBoxProcess:CloseGiftBox();
    end
	UIManager:DeletePanel("GiftBoxPanel");
	UIManager:DeletePanel("GiftBoxDragPanel");
end

function SelectGiftBoxItem()
    ResetAllGiftBoxTab();
    local checkBox = UIManager:FindUICheckBox("GiftBoxItem");
	local temp = UIManager:GetUIItem("GiftBoxItem");
	if checkBox ~= nil and temp ~= nil then
	    checkBox:SetState(true);
	    UIManager:AddDirtyUIItem(temp);
	end
    local giftBoxProcess = LuaUI.GetGiftBoxProcess();
    if giftBoxProcess ~= nil then
        giftBoxProcess:ShowGiftBoxItem()
    end
end

function SelectGiftBoxPartner()
    ResetAllGiftBoxTab();
    local checkBox = UIManager:FindUICheckBox("GiftBoxPartner");
	local temp = UIManager:GetUIItem("GiftBoxPartner");
	if checkBox ~= nil and temp ~= nil then
	    checkBox:SetState(true);
	    UIManager:AddDirtyUIItem(temp);
	end
    local giftBoxProcess = LuaUI.GetGiftBoxProcess();
    if giftBoxProcess ~= nil then
        giftBoxProcess:ShowGiftBoxPartner()
    end
end

function SelectGiftBoxPet()
    ResetAllGiftBoxTab();
    local checkBox = UIManager:FindUICheckBox("GiftBoxPet");
	local temp = UIManager:GetUIItem("GiftBoxPet");
	if checkBox ~= nil and temp ~= nil then
	    checkBox:SetState(true);
	    UIManager:AddDirtyUIItem(temp);
	end
    local giftBoxProcess = LuaUI.GetGiftBoxProcess();
    if giftBoxProcess ~= nil then
        giftBoxProcess:ShowGiftBoxPet()
    end
end

function ResetAllGiftBoxTab()
	local checkBox = UIManager:FindUICheckBox("GiftBoxItem");
	local temp = UIManager:GetUIItem("GiftBoxItem");
	if checkBox ~= nil and temp ~= nil then
	    checkBox:SetState(false);
	    UIManager:AddDirtyUIItem(temp);
	end
	
	checkBox = UIManager:FindUICheckBox("GiftBoxPartner");
	temp = UIManager:GetUIItem("GiftBoxPartner");
	if checkBox ~= nil and temp ~= nil then
	    checkBox:SetState(false);
	    UIManager:AddDirtyUIItem(temp);
	end
	
	checkBox = UIManager:FindUICheckBox("GiftBoxPet");
	temp = UIManager:GetUIItem("GiftBoxPet");
	if checkBox ~= nil and temp ~= nil then
	    checkBox:SetState(false);
	    UIManager:AddDirtyUIItem(temp);
	end
end

function GetGiftBoxSlotIndex(MouseX, MouseY)
	local uim = UIManager:GetUIItem("GiftBox000Icon");
	if uim ~= nil then

		local leftupX = uim:GetX();
		local leftupY = uim:GetY();
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


function DragGiftBox()
    local mx = UIManager:GetCursorX();
	local my = UIManager:GetCursorY();
	local slotindex = GetGiftBoxSlotIndex(mx, my);
	
	local panel = UIManager:GetUIItem("GiftBoxDragPanel");
	if panel ~= nil then
		local iconName;
		if slotindex >= 0 and slotindex < 48 then
		    if slotindex < 10 then
				iconName = "GiftBox00"..slotindex.."Icon";
			else
			    iconName = "GiftBox0"..slotindex.."Icon";
			end
			_G.DragGiftIndex = slotindex;

			local orgIcon = UIManager:GetUIItem(iconName);
			if orgIcon ~= nil then
				local aniIcon = UIManager:FindUIAniIcon(iconName);
				local givenAniIcon = UIManager:FindUIAniIcon("GiftBoxDragIcon");

				if aniIcon ~= nil and givenAniIcon ~= nil then
				    givenAniIcon:ClearAniData();
				    givenAniIcon:SetAnimationData(aniIcon);
				end
				UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
				orgIcon:SetRenderStyle(1);
			end
			
			panel:SetEnable(true);
			UIManager:AddDirtyUIItem(panel);
		end
	end
	
	
end

function GetGiftBox()
    local mx = UIManager:GetCursorX();
	local my = UIManager:GetCursorY();
	_G.ConfirmGiftIndex = GetGiftBoxSlotIndex(mx, my);
	OpenConfirmGetGiftBox();
end


function GiftBoxResetDrag()
    UIManager:ResetDragItem();
	UIManager:StopDrag();
	
	local iconName;
 	if _G.DragGiftIndex < 10 then
		iconName = "GiftBox00".._G.DragGiftIndex.."Icon";
	else
	    iconName = "GiftBox0".._G.DragGiftIndex.."Icon";
	end

	local orgIcon = UIManager:GetUIItem(iconName);
	if orgIcon ~= nil then
		orgIcon:SetRenderStyle(0);
		UIManager:AddDirtyUIItem(orgIcon);
	end

	local panel = UIManager:GetUIItem("GiftBoxDragPanel");
	if panel ~= nil then
		panel:SetPos(0,0);
		panel:SetEnable(false);
	end
	
	_G.DragGiftIndex = -1;
end

function GiftBoxDragEnd(value)
	if value ~= 3 then
	    local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();

		local iconName;
		local stackName;

    	if _G.DragGiftIndex < 10 then
			iconName = "GiftBox00".._G.DragGiftIndex.."Icon";
		else
		    iconName = "GiftBox0".._G.DragGiftIndex.."Icon";
		end

		local itemMenu = UIManager:GetUIItem("ItemMenuPanel");

		if itemMenu ~= nil then
		    local itemx = itemMenu:GetX();
		    local itemy = itemMenu:GetY();

			local diffx = mx - itemx;
			local diffy = my - itemy;
			local giftBoxProcess = LuaUI.GetGiftBoxProcess();

            if giftBoxProcess ~= nil and diffx >= 0 and
				diffx < itemMenu:GetWidth() and	diffy > 0 and
				diffy < itemMenu:GetHeight() then
                _G.ConfirmGiftIndex = _G.DragGiftIndex;
                OpenConfirmGetGiftBox();			
		    end
		end
	end
	GiftBoxResetDrag();
end

function OpenConfirmGetGiftBox()

    local panel = UIManager:FindUIPanel("ConfirmGetGiftPanel");
    local giftBoxProcess = LuaUI.GetGiftBoxProcess();
    if panel == nil then
        LuaUI.AddXMLFile2UIManger("Data\\ConfirmGetGift.xml");
        panel = UIManager:FindUIPanel("ConfirmGetGiftPanel");
	end

	if panel ~= nil and giftBoxProcess ~= nil then
		UIManager:SetMonoPanel(panel);
        giftBoxProcess:ShowConfirmGetGiftName(_G.ConfirmGiftIndex);
	end
	
end

function CloseConfirmGetGiftPanel()
	UIManager:ClearMonoPanel();
	UIManager:DeletePanel("ConfirmGetGiftPanel");
	if _G.NeedResetConfirmGift then
		_G.ConfirmGiftIndex = -1;
 	end
end

function ConfirmGetGiftOK()
    _G.NeedResetConfirmGift = false;
    CloseConfirmGetGiftPanel();
    local giftBoxProcess = LuaUI.GetGiftBoxProcess();
    if giftBoxProcess ~= nil then
        giftBoxProcess:ConfirmGetGift(_G.ConfirmGiftIndex);
    end
    _G.ConfirmGiftIndex = -1;
    _G.NeedResetConfirmGift = true;
end

function GiftBoxDetailPopupTimeOut()
	local pPanel = UIManager:GetUIItem("GiftBoxPanel");
	local giftBoxProcess = LuaUI.GetGiftBoxProcess();
    if giftBoxProcess ~= nil and pPanel ~= nil then
		CloseOnlineShopDetail();	
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();
		local slotindex = GetGiftBoxSlotIndex(mx, my);
		if slotindex >= 0 then
			giftBoxProcess:ShowDetail(slotindex);
			pPanel:SetEnableTimeOutEvent(false);
		end
	end
end

function GiftBoxMouseOut()
	local pPanel = UIManager:GetUIItem("GiftBoxPanel");
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(false);
		_G.NeedShowDetail = true;
	end
	CloseOnlineShopDetail();
end

function GiftBoxMouseOver()
	if _G.NeedShowDetail then
		local pPanel = UIManager:GetUIItem("GiftBoxPanel");
		if pPanel ~= nil then
			pPanel:SetEnableTimeOutEvent(true);
			pPanel:ResetTimer();
			_G.NeedShowDetail = false;
		end
	end
end

function UpdateGiftBox()
    local giftBoxProcess = LuaUI.GetGiftBoxProcess();
    if giftBoxProcess ~= nil then
        giftBoxProcess:UpdateGiftBoxFromServer();
    end
end