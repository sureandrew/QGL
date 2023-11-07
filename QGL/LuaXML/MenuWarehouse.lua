
DragWarehouseItemPicIndex = -1
DragWarehousePartnerPicIndex = -1
DragWarehousePetPicIndex = -1
WareHousePageIndex = 0
WareHouseTimeOutFlag = 0;

function OpenWarehousePanel()
	local panel = UIManager:GetUIItem("WarehousePanel");
	local itemPanel = UIManager:GetUIItem("ItemMenuPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuWarehouse.xml");
		panel = UIManager:GetUIItem("WarehousePanel");
		if panel ~= nil then
			panel:SetPos(20, 40);
	        panel:SetEnable(true);
			UIManager:AddDirtyUIItem(panel);
		end
		
		if itemPanel == nil then
			BBItem();
   			itemPanel = UIManager:GetUIItem("ItemMenuPanel");
		end
		
		if itemPanel ~= nil then
			itemPanel:SetPos(400, 40);
			itemPanel:SetEnable(true);
			UIManager:AddDirtyUIItem(itemPanel);
		end 
		
		SelectWarehouseItem0();
		
	else
		CloseWarehousePanel();
	end
end

function CloseWarehousePanel()
	local wareHouseProcess = LuaUI.GetWareHouseProcess();
    if wareHouseProcess ~= nil then
        wareHouseProcess:CloseWareHouse();
    end
	UIManager:StopDrag();
	CloseWareHouseCount();
	CloseWareHouseToItemCount();
	UIManager:DeletePanel("WareHouseItemCountPanel");
	UIManager:DeletePanel("WareHouseToItemCountPanel");
	UIManager:DeletePanel("WarehousePanel");
	UIManager:DeletePanel("WarehouseMovingPanel");
	ResetKeyIn();
end

function ResetWarehouseBlockImg(show)
	local name;
	local img;
	for i = 0, 47 do
	    if i < 10 then
			name = "WarehouseItem00"..i.."Block";
		else
		    name = "WarehouseItem0"..i.."Block";
		end
		img = UIManager:GetUIItem(name);
		if img ~= nil then
		    img:SetEnable(show);
		end
	end
end

function ResetAllWareHouseTab()
	local checkBox = UIManager:FindUICheckBox("WarehouseSelectItem0");
	local uiItem = UIManager:GetUIItem("WarehouseSelectItem0");
	
	if checkBox ~= nil and uiItem ~= nil then
		checkBox:SetState(false);
	    UIManager:AddDirtyUIItem(uiItem);
	end
	
	checkBox = UIManager:FindUICheckBox("WarehouseSelectItem1");
	uiItem = UIManager:GetUIItem("WarehouseSelectItem1");	
	if checkBox ~= nil and uiItem ~= nil then
		checkBox:SetState(false);
	    UIManager:AddDirtyUIItem(uiItem);
	end
	
	checkBox = UIManager:FindUICheckBox("WarehouseSelectPartner");
	uiItem = UIManager:GetUIItem("WarehouseSelectItem1");	
	if checkBox ~= nil and uiItem ~= nil then
		checkBox:SetState(false);
	    UIManager:AddDirtyUIItem(uiItem);
	end
	
	checkBox = UIManager:FindUICheckBox("WarehouseSelectPet");
	uiItem = UIManager:GetUIItem("WarehouseSelectItem1");	
	if checkBox ~= nil and uiItem ~= nil then
		checkBox:SetState(false);
	    UIManager:AddDirtyUIItem(uiItem);
	end
end

function SelectWarehouseItem0()
	_G.WareHousePageIndex = 0;
	ResetWareHousePPDisplay(false);
    ResetWarehouseBlockImg(true);
    ResetAllWareHouseTab();
    local checkBox = UIManager:FindUICheckBox("WarehouseSelectItem0");
	local temp = UIManager:GetUIItem("WarehouseSelectItem0");
	if checkBox ~= nil and temp ~= nil then
	    checkBox:SetState(true);
	    UIManager:AddDirtyUIItem(temp);
	end
    local wareHouseProcess = LuaUI.GetWareHouseProcess();
    if wareHouseProcess ~= nil then
        wareHouseProcess:ShowWareHouseItem(0);
    end
end

function SelectWarehouseItem1()
	_G.WareHousePageIndex = 1;
	ResetWareHousePPDisplay(false);
    ResetWarehouseBlockImg(true);
    ResetAllWareHouseTab();
    local checkBox = UIManager:FindUICheckBox("WarehouseSelectItem1");
	local temp = UIManager:GetUIItem("WarehouseSelectItem1");
	if checkBox ~= nil and temp ~= nil then
	    checkBox:SetState(true);
	    UIManager:AddDirtyUIItem(temp);
	end
    local wareHouseProcess = LuaUI.GetWareHouseProcess();
    if wareHouseProcess ~= nil then
        wareHouseProcess:ShowWareHouseItem(1);
    end
end

function ResetAllWareHouseItemIcon()
	local name;
	local stackName;
	local img1;
	local img2;
	for i = 0, 47 do
      	if i < 10 then
			name = "WarehouseItem00"..i.."Icon";
			stackName = "WarehouseItem00"..i.."Stack";
		else
		    name = "WarehouseItem0"..i.."Icon";
		    stackName = "WarehouseItem0"..i.."Stack";
		end
		img1 = UIManager:GetUIItem(name);
		img2 = UIManager:GetUIItem(stackName);
		if img1 ~= nil and img2 ~= nil then
		    img1:SetEnable(false);
		    img2:SetEnable(false);
		end
	end
end

function ResetWareHousePPDisplay(show)
	local name;
	local img;
	local ppIcon;
	local ppLevel;
	local ppName;
	local ppBlock;
	
	for i = 0, 7 do
		name = "WarehousePPBack0"..i.."";
		img = UIManager:GetUIItem(name);
		if img ~= nil then
		    img:SetEnable(show);
		end
		if show == false then
		    name = "WarehousePP0"..i.."Icon";
            ppIcon = UIManager:GetUIItem(name);
            name = "WarehousePP0"..i.."Name";
            ppName = UIManager:GetUIItem(name);
            name = "WarehousePP0"..i.."Lvl";
            ppLevel = UIManager:GetUIItem(name);
            name = "WarehousePP0"..i.."Block";
            ppBlock = UIManager:GetUIItem(name);
            if ppIcon ~= nil and ppName ~= nil and
				ppLevel ~= nil and ppBlock ~= nil then
				
                ppIcon:SetEnable(false);
                ppName:SetEnable(false);
                ppLevel:SetEnable(false);
                ppBlock:SetEnable(false);
            end
		end
	end
end

function SelectWarehousePartner()
	_G.WareHousePageIndex = 2;
	ResetWarehouseBlockImg(false);
	ResetAllWareHouseItemIcon();
	ResetWareHousePPDisplay(true);
	ResetAllWareHouseTab();
    local checkBox = UIManager:FindUICheckBox("WarehouseSelectPartner");
	local temp = UIManager:GetUIItem("WarehouseSelectPartner");
	if checkBox ~= nil and temp ~= nil then
	    checkBox:SetState(true);
	    UIManager:AddDirtyUIItem(temp);
	end
	local wareHouseProcess = LuaUI.GetWareHouseProcess();
    if wareHouseProcess ~= nil then
        wareHouseProcess:ShowWareHousePartner();
    end
end

function SelectWarehousePet()
	_G.WareHousePageIndex = 3;
	ResetWarehouseBlockImg(false);
	ResetAllWareHouseItemIcon();
	ResetWareHousePPDisplay(true);
	ResetAllWareHouseTab();
    local checkBox = UIManager:FindUICheckBox("WarehouseSelectPet");
	local temp = UIManager:GetUIItem("WarehouseSelectPet");
	if checkBox ~= nil and temp ~= nil then
	    checkBox:SetState(true);
	    UIManager:AddDirtyUIItem(temp);
	end
	local wareHouseProcess = LuaUI.GetWareHouseProcess();
    if wareHouseProcess ~= nil then
        wareHouseProcess:ShowWareHousePet();
    end
end

function WareHouseAddOne()
	local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseItem");
	local temp = UIManager:GetUIItem("NumberOfWareHouseItem");
	
	local value = numEdit:GetInt();
	
	if value >= 0 and value < LuaUI.GetMaxStack(_G.GivenItemOrgIndex + 24 * _G.BagState) then
		value = value + 1;
	end
	
	numEdit:SetText(value.."");--value.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);--RenderAll();

end

function WareHouseDiffOne()

	local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseItem");
	local temp = UIManager:GetUIItem("NumberOfWareHouseItem");
	
	local value = numEdit:GetInt();
	
	if value > 1 then
		value = value - 1;
	end
	
	numEdit:SetText(value.."");--value.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function WareHouseItemSetZero()
	local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseItem");
	local temp = UIManager:GetUIItem("NumberOfWareHouseItem");
	numEdit:SetText("1");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
 end

function WareHouseMaxStack()

	local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseItem");
	local temp = UIManager:GetUIItem("NumberOfWareHouseItem");
	local max = LuaUI.GetMaxStack(_G.GivenItemOrgIndex + 24 * _G.BagState);
	
	numEdit:SetText(max.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function CloseWareHouseCount()
	ResetKeyIn();
    local countPanel = UIManager:GetUIItem("WareHouseItemCountPanel");
    local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseItem");
    if countPanel ~= nil and numEdit ~= nil then
    	countPanel:SetEnable(false);
    	numEdit:SetText("1");
		numEdit:Redraw();
    end
    
    _G.GivenItemOrgIndex = -1;
    _G.PlacedItemIndex = -1;
    UIManager:ClearMonoPanel();
end

function ConfirmWareHouseNum()

	local wareHouseProcess = LuaUI.GetWareHouseProcess();
	local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseItem");
	if _G.WareHousePageIndex == 2 or _G.WareHousePageIndex == 3 then 
		SelectWarehouseItem0();
	end
	if wareHouseProcess ~= nil and numEdit ~= nil then
		local value = numEdit:GetInt();		
		if value > 0  and value <= LuaUI.GetMaxStack(_G.GivenItemOrgIndex + 24 * _G.BagState) and _G.PlacedItemIndex >= 0 then
			if _G.PlacedItemIndex ~= 255 then
				wareHouseProcess:PutToWareHouse(_G.PlacedItemIndex + 48 * _G.WareHousePageIndex, _G.GivenItemOrgIndex + 24 * _G.BagState, value);
			else
				wareHouseProcess:PutToWareHouse(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 24 * _G.BagState, value);
			end
		else
		    LuaUI.ShowErrMessage("MSG_WAREHOUSE_ITEM_STACK_ERROR", true);
		end 
	end
	CloseWareHouseCount();	
end

function WareHouseToItemAddOne()
	local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseToItem");
	local temp = UIManager:GetUIItem("NumberOfWareHouseToItem");
	
	local value = numEdit:GetInt();
	local wareHouse = LuaUI.GetWareHouseProcess();
	if value >= 0 and value < wareHouse:GetMaxStack(_G.GivenItemOrgIndex + 48 * _G.WareHousePageIndex) then
		value = value + 1;
	end 
	
	numEdit:SetText(value.."");--value.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);--RenderAll();

end

function WareHouseToItemDiffOne()

	local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseToItem");
	local temp = UIManager:GetUIItem("NumberOfWareHouseToItem");
	
	local value = numEdit:GetInt();
	
	if value > 1 then
		value = value - 1;
	end
	
	numEdit:SetText(value.."");--value.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function WareHouseToItemSetZero()
	local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseToItem");
	local temp = UIManager:GetUIItem("NumberOfWareHouseToItem");
	numEdit:SetText("1");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
 end

function WareHouseToItemMaxStack()

	local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseToItem");
	local temp = UIManager:GetUIItem("NumberOfWareHouseToItem");
	local wareHouse = LuaUI.GetWareHouseProcess();
	local max = wareHouse:GetMaxStack(_G.GivenItemOrgIndex + 48 * _G.WareHousePageIndex);
	
	numEdit:SetText(max.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function CloseWareHouseToItemCount()
	ResetKeyIn();
    local countPanel = UIManager:GetUIItem("WareHouseToItemCountPanel");
    local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseToItem");
    if countPanel ~= nil and numEdit ~= nil then
    	countPanel:SetEnable(false);
    	numEdit:SetText("1");
		numEdit:Redraw();
    end
    
    _G.GivenItemOrgIndex = -1;
    _G.PlacedItemIndex = -1;
    UIManager:ClearMonoPanel();
end

function ConfirmWareHouseToItemNum()

	local wareHouseProcess = LuaUI.GetWareHouseProcess();
	local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseToItem");
	 
	if wareHouseProcess ~= nil and numEdit ~= nil then
		local value = numEdit:GetInt();		
		if value > 0  and value <= wareHouseProcess:GetMaxStack(_G.GivenItemOrgIndex + 48 * _G.WareHousePageIndex) and _G.PlacedItemIndex >= 0 then
			wareHouseProcess:GetFromWareHouse(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 48 * _G.WareHousePageIndex, value);
		else
		    LuaUI.ShowErrMessage("MSG_WAREHOUSE_ITEM_STACK_ERROR", true);
		end 
	end
	CloseWareHouseToItemCount();	
end

function WarehouseItemMove()
	local index = GetWareHouseSlotIndex();
	if index == -1 then
	    do return end
	end
	
	_G.DragWarehouseItemPicIndex = index	
	local panel = UIManager:GetUIItem("WarehouseMovingPanel"); 
	if panel ~= nil then
		local iconName;
		if index >= 0 and index < 10 then
			iconName = "WarehouseItem00".._G.DragWarehouseItemPicIndex.."Icon";
		else
			iconName = "WarehouseItem0".._G.DragWarehouseItemPicIndex.."Icon";
		end
		local orgIcon = UIManager:GetUIItem(iconName);
		
		if orgIcon ~= nil then
			local aniIcon = UIManager:FindUIAniIcon(iconName);
			local warehusAniIcon = UIManager:FindUIAniIcon("WarehouseMovingIcon");
			
			if aniIcon ~= nil and warehusAniIcon ~= nil then
			    warehusAniIcon:ClearAniData();
			    warehusAniIcon:SetAnimationData(aniIcon);
			end

			UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
			orgIcon:SetRenderStyle(1);
		end
		panel:SetEnable(true);
		UIManager:AddDirtyUIItem(panel);
	end	
end

function WarehouseItemEndDrag()
	local wareHouseProcess = LuaUI.GetWareHouseProcess();
	if wareHouseProcess ~= nil then
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();
		
		local wareHousePanel = UIManager:GetUIItem("WarehousePanel");
		if wareHousePanel ~= nil then
		    local diffx = mx - wareHousePanel:GetX();
		    local diffy = my - wareHousePanel:GetY();
		    if diffy >= 63 and diffy <= 390 and
				diffx >= 28 and diffx < 272 then
				local index =  LuaUI.Div( (diffx - 28), 41) + 6 *  LuaUI.Div( (diffy - 63), 41);
				if index ~= _G.DragWarehouseItemPicIndex and
					wareHouseProcess:AvailSlot(index + 48 * _G.WareHousePageIndex) == true then

					wareHouseProcess:MoveItem(
					_G.DragWarehouseItemPicIndex + 48 * _G.WareHousePageIndex,
					index + 48 * _G.WareHousePageIndex );
				end
				ResetWarehouseDrag();
				do return end
			end
		end
	
		local itemMenu = UIManager:GetUIItem("ItemMenuPanel");
		if itemMenu ~= nil then

			local diffx = mx - itemMenu:GetX();
			local diffy = my - itemMenu:GetY();
            local countPanel = UIManager:GetUIItem("WareHouseToItemCountPanel");
			local tempPanel = UIManager:FindUIPanel("WareHouseToItemCountPanel");
        
		    if diffx >= 25 and diffx < 272 and
				diffy > 281 and diffy < 446 then

				if _G.BagState > 0 then
					LuaUI.ShowErrMessage("MSG_WAREHOUSE_PUT_IN_WRONG_BAG", true);
					ResetWarehouseDrag();
					do return end;
				end
				_G.GivenItemOrgIndex = _G.DragWarehouseItemPicIndex;
				_G.PlacedItemIndex = LuaUI.Div( (diffx - 25), 41) + 6 *  LuaUI.Div( (diffy - 281), 41);
				local max = wareHouseProcess:GetMaxStack(_G.DragWarehouseItemPicIndex + 48 * _G.WareHousePageIndex);
				if wareHouseProcess:CheckCanSwap(_G.PlacedItemIndex, _G.DragWarehouseItemPicIndex + 48 * _G.WareHousePageIndex) == true or  max == 1 then
					local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseToItem");
					if numEdit ~= nil then
						numEdit:SetText(max.."");
					end
					ResetWarehouseDrag();
					ConfirmWareHouseToItemNum();
					do return end;	
				else
					UIManager:SetPanel2Top(tempPanel);
					countPanel:SetEnable(true);
					UIManager:SetKeyInItem("NumberOfWareHouseToItem");
					UIManager:ClearMonoPanel();
			    	UIManager:SetMonoPanel(tempPanel);	
			    	UIManager:AddDirtyUIItem(countPanel);
		    	end
		    end
		end
	end
end

function WarehousePartnerEndDrag()
	local wareHouseProcess = LuaUI.GetWareHouseProcess();
	local mx = UIManager:GetCursorX();
	local my = UIManager:GetCursorY();
	local itemMenu = UIManager:GetUIItem("ItemMenuPanel");
	
	if wareHouseProcess ~= nil and itemMenu ~= nil then
	    local itemx = itemMenu:GetX();
	    local itemy = itemMenu:GetY();

		local diffx = mx - itemx;
		local diffy = my - itemy;
		if diffx >= 0 and diffx < itemMenu:GetWidth() and 
			diffy > 0 and diffy < itemMenu:GetHeight() then
			wareHouseProcess:GetPartnerFromWareHouse(_G.DragWarehousePartnerPicIndex);
					
		end		
	end
end

function WarehousePetEndDrag()
	local wareHouseProcess = LuaUI.GetWareHouseProcess();
	local mx = UIManager:GetCursorX();
	local my = UIManager:GetCursorY();
	local itemMenu = UIManager:GetUIItem("ItemMenuPanel");

	if wareHouseProcess ~= nil and itemMenu ~= nil then
	    local itemx = itemMenu:GetX();
	    local itemy = itemMenu:GetY();

		local diffx = mx - itemx;
		local diffy = my - itemy;
		if diffx >= 0 and diffx < itemMenu:GetWidth() and
			diffy > 0 and diffy < itemMenu:GetHeight() then
			wareHouseProcess:GetPetFromWareHouse(_G.DragWarehousePetPicIndex);

		end
	end
end

function WarehouseEndDrag(value)
    
	if value ~= 3 then
		if _G.DragWarehouseItemPicIndex >= 0 then
			WarehouseItemEndDrag();
		elseif _G.DragWarehousePartnerPicIndex >= 0 then
			WarehousePartnerEndDrag();
		elseif _G.DragWarehousePetPicIndex >= 0 then
		    WarehousePetEndDrag();
		end 
	end
	ResetWarehouseDrag();
end

function ResetWarehouseDrag()
	UIManager:ResetDragItem();
	UIManager:StopDrag();
	
	local iconName;
	if _G.DragWarehouseItemPicIndex >= 0 then
	 	if _G.DragWarehouseItemPicIndex < 10 then
			iconName = "WarehouseItem00".._G.DragWarehouseItemPicIndex.."Icon";
		else
		    iconName = "WarehouseItem0".._G.DragWarehouseItemPicIndex.."Icon";
		end
	elseif _G.DragWarehousePartnerPicIndex >= 0 then
		iconName = "WarehousePP0".._G.DragWarehousePartnerPicIndex.."Icon";
	elseif _G.DragWarehousePetPicIndex >= 0 then
		iconName = "WarehousePP0".._G.DragWarehousePetPicIndex.."Icon";
	end

	local orgIcon = UIManager:GetUIItem(iconName);
	if orgIcon ~= nil then
		orgIcon:SetRenderStyle(0);
		UIManager:AddDirtyUIItem(orgIcon);
	end
	
	local movingPanel = UIManager:GetUIItem("WarehouseMovingPanel");
	
	if movingPanel ~= nil then
		movingPanel:SetPos(0,0);
		movingPanel:SetEnable(false);
		movingPanel:SetNotDetected(false);
	end
	
	_G.DragWarehouseItemPicIndex = -1	
	_G.DragWarehousePartnerPicIndex = -1
	_G.DragWarehousePetPicIndex = -1	
	_G.DragType = -1;	

end

function WareHouseMouseIn()
	local pPanel = UIManager:GetUIItem("WarehousePanel");
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(true);
		pPanel:ResetTimer();
	end
	_G.WareHouseTimeOutFlag = 1;
end

function WareHouseMouseOut()
	if _G.WareHouseTimeOutFlag == 0 then
		local pPanel = UIManager:GetUIItem("WarehousePanel");
		if pPanel ~= nil then
			pPanel:SetEnableTimeOutEvent(false);
		end
	end
	CloseOnlineShopDetail();
end

function WareHouseDetailPopupTimeOut()
	local panel = UIManager:FindUIPanel("WarehousePanel");
 	local wareHouseProcess = LuaUI.GetWareHouseProcess();
	if wareHouseProcess ~= nil and panel ~= nil then

		local cursorX = UIManager:GetCursorX();
	    local cursorY = UIManager:GetCursorY();
	    if cursorX > panel:GetX() and cursorY > panel:GetY() and
			cursorX < (panel:GetX() + panel:GetWidth()) and
			cursorY < (panel:GetY() + panel:GetHeight()) then

			if _G.WareHousePageIndex == 0 or _G.WareHousePageIndex == 1 then
				wareHouseProcess:ShowItemDetail();
			elseif _G.WareHousePageIndex == 2 then
				wareHouseProcess:ShowPartnerDetail();
			elseif _G.WareHousePageIndex == 3 then
    			wareHouseProcess:ShowPetDetail();
			end
		else
		    CloseOnlineShopDetail();
		end

		panel:SetEnableTimeOutEvent(false);
	end
	_G.WareHouseTimeOutFlag = 0;
end

function WarehouseItemUse()
    CloseOnlineShopDetail();
	local index = GetWareHouseSlotIndex();
	local wareHouseProcess = LuaUI.GetWareHouseProcess();
	if index >= 0 and wareHouseProcess ~= nil then
	    _G.GivenItemOrgIndex = index;
		_G.PlacedItemIndex = 255;
		local max = wareHouseProcess:GetMaxStack(index + 48 * _G.WareHousePageIndex);
		local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseToItem");
		if numEdit ~= nil then
			numEdit:SetText(max.."");
		end
		ResetWarehouseDrag();
		ConfirmWareHouseToItemNum();
		do return end;
	end
end

function GetWareHouseSlotIndex()
	local wareHousePanel = UIManager:GetUIItem("WarehousePanel");
	if wareHousePanel ~= nil then
        local diffx = UIManager:GetCursorX() - wareHousePanel:GetX();
	    local diffy = UIManager:GetCursorY() - wareHousePanel:GetY();
	    if diffy >= 63 and diffy <= 390 and
			diffx >= 28 and diffx < 272 then
			return LuaUI.Div( (diffx - 28), 41) + 6 *  LuaUI.Div( (diffy - 63), 41);
		end
	end
	return -1;
end

function WarehousePPMove(index)

	
	local panel = UIManager:GetUIItem("WarehouseMovingPanel"); 
	if panel ~= nil then
		local iconName;
		if index >= 0 and index < 8 then
			iconName = "WarehousePP0"..index.."Icon";
			
			if _G.WareHousePageIndex == 2 then
				_G.DragWarehousePartnerPicIndex = index;
			else
				_G.DragWarehousePetPicIndex = index;
			end
		
			local orgIcon = UIManager:GetUIItem(iconName);
			if orgIcon ~= nil then
				--LuaUI.CopyImageForUIImage(iconName, "GivenMovingIcon");
				local aniIcon = UIManager:FindUIAniIcon(iconName);
				local wareHouseAniIcon = UIManager:FindUIAniIcon("WarehouseMovingIcon");

				if aniIcon ~= nil and wareHouseAniIcon ~= nil then
				    wareHouseAniIcon:ClearAniData();
				    wareHouseAniIcon:SetAnimationData(aniIcon);
				end
				UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
				orgIcon:SetRenderStyle(1);
			end
		end
		panel:SetEnable(true);
		UIManager:AddDirtyUIItem(panel);		
	end
	
end

function WarehousePPMove00()
	WarehousePPMove(0);
end

function WarehousePPMove01()
	WarehousePPMove(1);
end

function WarehousePPMove02()
	WarehousePPMove(2);
end

function WarehousePPMove03()
	WarehousePPMove(3);
end

function WarehousePPMove04()
	WarehousePPMove(4);
end

function WarehousePPMove05()
	WarehousePPMove(5);
end

function WarehousePPMove06()
	WarehousePPMove(6);
end

function WarehousePPMove07()
	WarehousePPMove(7);
end

function WarehousePPUse(index)
	local wareHouseProcess = LuaUI.GetWareHouseProcess();
	if wareHouseProcess ~= nil then
		if _G.WareHousePageIndex == 2 then
			_G.DragWarehousePartnerPicIndex = index;
			wareHouseProcess:GetPartnerFromWareHouse(_G.DragWarehousePartnerPicIndex);
		else
			_G.DragWarehousePetPicIndex = index;
			wareHouseProcess:GetPetFromWareHouse(_G.DragWarehousePetPicIndex);
		end
	end
	
end

function WarehousePPUse00()
	WarehousePPUse(0);
end

function WarehousePPUse01()
	WarehousePPUse(1);
end

function WarehousePPUse02()
	WarehousePPUse(2);
end

function WarehousePPUse03()
	WarehousePPUse(3);
end

function WarehousePPUse04()
	WarehousePPUse(4);
end

function WarehousePPUse05()
	WarehousePPUse(5);
end

function WarehousePPUse06()
	WarehousePPUse(6);
end

function WarehousePPUse07()
	WarehousePPUse(7);
end

function CleanWarehouse()
	local wareHouseProcess = LuaUI.GetWareHouseProcess();
	if wareHouseProcess ~= nil then
		wareHouseProcess:JpackWareHouse(WareHousePageIndex);
	end
end