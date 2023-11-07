function OpenItemMenuForMixShop()

    local itemPanel = UIManager:GetUIItem("ItemMenuPanel");
     if itemPanel == nil then
    	BBItem();
    	itemPanel = UIManager:GetUIItem("ItemMenuPanel");
    end

	itemPanel:SetPos(400, 40);
	UIManager:AddDirtyUIItem(itemPanel);
end

function CancelMixShopMaking()
	local blockImg = UIManager:GetUIItem("MixShopBlockImage");
	if blockImg ~= nil then
		blockImg:SetEnable(false);
		UIManager:AddDirtyUIItem(blockImg);		
	end
    local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
	    if mixShopProcess:IsStartMaking() then
	        mixShopProcess:CancelMake();
	        local button = UIManager:FindUIButton("MixShopButtonMake");
	        local temp = UIManager:GetUIItem("MixShopButtonMake");
	        if button ~= nil and temp ~= nil then
	        	button:SetNormalState();
	        	
				UIManager:AddDirtyUIItem(temp);
	        end
	    else
	        mixShopProcess:CancelMake();
	        CloseMixShopMake();
	    end
	end
end

function CancelMixShopMixing()
    local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
	    if mixShopProcess:IsStartMaking() then
	        mixShopProcess:CancelMake();
	        local button = UIManager:FindUIButton("MixShopButtonRepair");
	        local temp = UIManager:GetUIItem("MixShopButtonRepair");
	        if button ~= nil and temp ~= nil then
	        	button:SetNormalState();
	        	
				UIManager:AddDirtyUIItem(temp);
	        end
	    else
	        mixShopProcess:CancelMake();
	        CloseMixShop();
	    end
	end
end

function CloseMixShop()
	UIManager:StopDrag();
    local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:ResetMixShop();
	end
	UIManager:DeletePanel("MixShopPanel");
	UIManager:DeletePanel("MixShopMovingPanel");
	UIManager:DeletePanel("MixShopCountPanel");
end

function CloseMixShopMake()
	UIManager:StopDrag();
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:ResetMixShopMake();
	end
	UIManager:DeletePanel("MixShopMakePanel");
	UIManager:DeletePanel("MixShopMakeMovingPanel");
end

function CloseIronShopMake()
	UIManager:StopDrag();
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:ResetIronShop();
	end
	UIManager:DeletePanel("IronShopMakePanel");
	UIManager:DeletePanel("IronShopMakeMovingPanel");
end

function CloseMixShopGem()
	UIManager:StopDrag();
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:ResetMixShop();
	end
	UIManager:DeletePanel("MixShopGemPanel");
	UIManager:DeletePanel("MixShopGemMovingPanel");
end

function SeletMixShopMakeIndex(index)
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:SetSelectedIndex(index);
	end
end

function SeletMixShopMakeIndex0()
	SeletMixShopMakeIndex(0);
end

function SeletMixShopMakeIndex1()
	SeletMixShopMakeIndex(1);
end

function SeletMixShopMakeIndex2()
	SeletMixShopMakeIndex(2);
end

function SeletMixShopMakeIndex3()
	SeletMixShopMakeIndex(3);
end

function SeletMixShopMakeIndex4()
	SeletMixShopMakeIndex(4);
end

function SeletMixShopMakeIndex5()
	SeletMixShopMakeIndex(5);
end

function SeletMixShopMakeIndex6()
	SeletMixShopMakeIndex(6);
end

function SeletMixShopMakeIndex7()
	SeletMixShopMakeIndex(7);
end

function SeletMixShopMakeIndex8()
	SeletMixShopMakeIndex(8);
end

function SeletMixShopMakeIndex9()
	SeletMixShopMakeIndex(9);
end

function MixShopShowItemScroll(index)
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:MixShopMakeProductScroll(index)
	end
end

function MixShopShowItemScrollUp()
	MixShopShowItemScroll(0);
end

function MixShopShowItemScrollDown()
	MixShopShowItemScroll(1);
end

function MixShopShowItemScrollMove()
	MixShopShowItemScroll(2);
end

function MixShopSourceItemScroll(index)
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:MixShopMakeSourceScroll(index)
	end
end

function MixShopSourceItemScrollUp()
	MixShopSourceItemScroll(0);
end

function MixShopSourceItemScrollDown()
	MixShopSourceItemScroll(1);
end

function MixShopSourceItemScrollMove()
	MixShopSourceItemScroll(2);
end

function MixShopStartMake()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:StartServerTime();
	end
end

function ConfirmMakeItemNum()
    local mixShopProcess = LuaUI.GetMixProcess();

	if mixShopProcess ~= nil then
  		mixShopProcess:SetSourceItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 24 * _G.BagState);
	end
	_G.GivenItemOrgIndex = -1;
    _G.PlacedItemIndex = -1;
end

function ConfirmIronShopItemNum()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:SetIronShopItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 24 * _G.BagState, 1);
	end
	_G.GivenItemOrgIndex = -1;
    _G.PlacedItemIndex = -1;
end

function ConfirmMixShopItemNum()
    local mixShopProcess = LuaUI.GetMixProcess();
	local editBox = UIManager:FindUIEditBox("NumberOfMixShopSourceItem");
	if mixShopProcess ~= nil and editBox ~= nil then
	    local value = editBox:GetInt();
	    if value > 0 then
	        if mixShopProcess:IsStoneMixShop() and value > mixShopProcess:GetMaxStoneNumByItemSlot(_G.GivenItemOrgIndex + 24 * _G.BagState) then
			    value = mixShopProcess:GetMaxStoneNumByItemSlot(_G.GivenItemOrgIndex + 24 * _G.BagState);
			end
  			mixShopProcess:SetMixShopItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 24 * _G.BagState, value);
		end
	end
	CloseMixShopCount();
	_G.GivenItemOrgIndex = -1;
    _G.PlacedItemIndex = -1;
end

function ConfirmMixShopSoulBoundItemNum()
    local mixShopProcess = LuaUI.GetMixProcess();
	local editBox = UIManager:FindUIEditBox("NumberOfMixShopSourceItem");
	if mixShopProcess ~= nil and editBox ~= nil then
	    local value = editBox:GetInt();
	    if value > 0 then
	        if mixShopProcess:IsUnSoulBoundShop() and value > mixShopProcess:GetUnSoulBoundStoneNum() then
			   value = mixShopProcess:GetUnSoulBoundStoneNum();
			end
  			mixShopProcess:SetMixShopItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 24 * _G.BagState, value);
		end
	end
	CloseMixShopCount();
	_G.GivenItemOrgIndex = -1;
    _G.PlacedItemIndex = -1;
end

function IronSourceMove00()
	IronSourceMove(0);
end

function IronSourceMove01()
	IronSourceMove(1);
end

function IronSourceMove(index)
	local mixShopProcess = LuaUI.GetMixProcess();
	if index >= 0 and index < 2 and mixShopProcess ~= nil and mixShopProcess:IsStartMaking() ~= true then
		local word = "";
		if mixShopProcess:IsIronShopMake() then
			word = "IronMakeDragItem0"..index;
		elseif mixShopProcess:IsIronShopRepair() then
			word = "IronRepairDragItem0"..index;
		end
		local orgIcon = UIManager:GetUIItem(word);
		local aniIcon = UIManager:FindUIAniIcon(word);
		local itemIcon = UIManager:FindUIAniIcon("IronShopMakeMovingIcon");
		local panel = UIManager:GetUIItem("IronShopMakeMovingPanel");
		if panel ~= nil and aniIcon ~= nil and itemIcon ~= nil and orgIcon ~= nil and orgIcon:GetRenderStyle() ~= 1 then
			_G.DragItemIndex = index;
			
			itemIcon:ClearAniData();
			itemIcon:SetAnimationData(aniIcon);
			
			UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
			orgIcon:SetRenderStyle(1);
			panel:SetEnable(true);
			
		end
	end
end

function MixShopSourceMove00()
	MixShopSourceMove(0);
end

function MixShopSourceMove01()
	MixShopSourceMove(1);
end

function MixShopSourceMove02()
	MixShopSourceMove(2);
end

function MixShopSourceMove03()
	MixShopSourceMove(3);
end

function MixShopSourceMove(index)
	local mixShopProcess = LuaUI.GetMixProcess();
	if index >= 0 and index < 4 and mixShopProcess ~= nil and mixShopProcess:IsStartMaking() ~= true then
		local word = "MixShopDragItem0"..index;
		local orgIcon = UIManager:GetUIItem(word);
		local aniIcon = UIManager:FindUIAniIcon(word);
		local itemIcon = UIManager:FindUIAniIcon("MixShopMakeMovingIcon");
		local panel = UIManager:GetUIItem("MixShopMakeMovingPanel");
		if panel ~= nil and aniIcon ~= nil and itemIcon ~= nil and orgIcon ~= nil and orgIcon:GetRenderStyle() ~= 1 then
			_G.DragItemIndex = index;
			
			itemIcon:ClearAniData();
			itemIcon:SetAnimationData(aniIcon);
			
			UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
			orgIcon:SetRenderStyle(1);
			panel:SetEnable(true);
			
		end
	end
end

function MixShopMove00()
	MixShopMove(0);
end

function MixShopMove01()
	MixShopMove(1);
end

function MixShopMove02()
	MixShopMove(2);
end

function MixShopMove(index)
	local mixShopProcess = LuaUI.GetMixProcess();
	if index >= 0 and index < 3 and mixShopProcess ~= nil and mixShopProcess:IsStartMaking() ~= true then
		local word = "MixShopItem0"..index;
		local orgIcon = UIManager:GetUIItem(word);
		local aniIcon = UIManager:FindUIAniIcon(word);
		local itemIcon = UIManager:FindUIAniIcon("MixShopMovingIcon");
		local panel = UIManager:GetUIItem("MixShopMovingPanel");
		if panel ~= nil and aniIcon ~= nil and itemIcon ~= nil and orgIcon ~= nil and orgIcon:GetRenderStyle() ~= 1 then
			_G.DragItemIndex = index;
			
			itemIcon:ClearAniData();
			itemIcon:SetAnimationData(aniIcon);
			
			UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
			orgIcon:SetRenderStyle(1);
			panel:SetEnable(true);

		end
	end
end

function ResetIronShopMakingDrag()
	UIManager:ResetDragItem();
	UIManager:StopDrag();
	local iconName;
	local mixShopProcess = LuaUI.GetMixProcess();
	local panel = UIManager:GetUIItem("IronShopMakeMovingPanel"); 
	if panel ~= nil then
		if mixShopProcess:IsIronShopMake() then
			iconName = "IronMakeDragItem0".._G.DragItemIndex;
		elseif mixShopProcess:IsIronShopRepair() then
			iconName = "IronRepairDragItem0".._G.DragItemIndex;
		end	    
     	panel:SetPos(0,0);
		panel:SetEnable(false);
	end
	
	local orgIcon = UIManager:GetUIItem(iconName);
	if orgIcon ~= nil then
		orgIcon:SetRenderStyle(0);
		UIManager:AddDirtyUIItem(orgIcon);
	end
	_G.DragItemIndex = -1;
end

function ResetMixShopMakingDrag()
	UIManager:ResetDragItem();
	UIManager:StopDrag();
	local iconName;
	local panel = UIManager:GetUIItem("MixShopMakeMovingPanel"); 
	if panel ~= nil then
	    iconName = "MixShopDragItem0".._G.DragItemIndex;
     	panel:SetPos(0,0);
		panel:SetEnable(false);
	else
		panel = UIManager:GetUIItem("MixShopMovingPanel"); 
		if panel ~= nil then
		    iconName = "MixShopItem0".._G.DragItemIndex;
			panel:SetPos(0,0);
			panel:SetEnable(false);
		end
	end
	
	local orgIcon = UIManager:GetUIItem(iconName);
	if orgIcon ~= nil then
		orgIcon:SetRenderStyle(0);
		UIManager:AddDirtyUIItem(orgIcon);
	end
	_G.DragItemIndex = -1;
end

function ResetMixShopDrag()
    UIManager:ResetDragItem();
	UIManager:StopDrag();

	local panel = UIManager:GetUIItem("MixShopMakeMovingPanel");
	if panel ~= nil then
     	panel:SetPos(0,0);
		panel:SetEnable(false);
	else
		panel = UIManager:GetUIItem("MixShopMovingPanel");
		if panel ~= nil then
			panel:SetPos(0,0);
			panel:SetEnable(false);
		end
	end
	local panel2 = UIManager:GetUIItem("IronShopMakeMovingPanel");
	if panel2 ~= nil then
     	panel2:SetPos(0,0);
		panel2:SetEnable(false);
	end
	_G.DragItemIndex = -1;
end

function IronShopMakeEndDrag(value)
	removed = false;
	local panel = UIManager:GetUIItem("IronShopMakeMovingPanel");
	local mixShopProcess = LuaUI.GetMixProcess();
	if value ~= 3 then
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();	
		local itemMenu = UIManager:GetUIItem("ItemMenuPanel");
		if itemMenu ~= nil then
		    local itemx = itemMenu:GetX();
		    local itemy = itemMenu:GetY();

			local diffx = mx - itemx;
			local diffy = my - itemy;
			
			if diffx >= 25 and diffx < 272 and mixShopProcess ~= nil and
					diffy > 281 and diffy < 446 and  _G.DragItemIndex >= 0 then
				if panel ~= nil then					
				 	mixShopProcess:RemoveIronItem(_G.DragItemIndex);
				end
				 removed = true;
			end		
		end
	end
	if removed == false then
		local iconName;
		if panel ~= nil then
			if mixShopProcess:IsIronShopMake() then
				iconName = "IronMakeDragItem0".._G.DragItemIndex;
			elseif mixShopProcess:IsIronShopRepair() then
				iconName = "IronRepairDragItem0".._G.DragItemIndex;
			end
		end
		
		local orgIcon = UIManager:GetUIItem(iconName);
		if orgIcon ~= nil then
			orgIcon:SetRenderStyle(0);
			UIManager:AddDirtyUIItem(orgIcon);
		end
	end
	ResetMixShopDrag();
end

function MixShopMakeEndDrag(value)

	removed = false;
	local panel = UIManager:GetUIItem("MixShopMakeMovingPanel");
	if value ~= 3 then
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();
		
		local itemMenu = UIManager:GetUIItem("ItemMenuPanel");

		if itemMenu ~= nil then
		    local itemx = itemMenu:GetX();
		    local itemy = itemMenu:GetY();

			local diffx = mx - itemx;
			local diffy = my - itemy;
			local mixShopProcess = LuaUI.GetMixProcess();
			
			if diffx >= 25 and diffx < 272 and mixShopProcess ~= nil and
					diffy > 281 and diffy < 446 and  _G.DragItemIndex >= 0 then
				if panel ~= nil then					
				 	mixShopProcess:RemoveMakeItem(_G.DragItemIndex);
				else
					mixShopProcess:RemoveMixItem(_G.DragItemIndex);
				end
				 removed = true;
			end		
		end
	end
	if removed == false then
		local iconName;
		if panel ~= nil then
			iconName = "MixShopDragItem0".._G.DragItemIndex;
		else
			iconName = "MixShopItem0".._G.DragItemIndex;
		end
		
		local orgIcon = UIManager:GetUIItem(iconName);
		if orgIcon ~= nil then
			orgIcon:SetRenderStyle(0);
			UIManager:AddDirtyUIItem(orgIcon);
		end
	end
 	ResetMixShopDrag();
end

function MixShopStartMix()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:StartServerTime();
	end
end

function MixShopStartMix2()
	local mixShopProcess = LuaUI.GetMixProcess();
	local numEdit = UIManager:FindUIEditBox("MixShopMake2Count");
	if mixShopProcess ~= nil and numEdit ~= nil then
		local value = numEdit:GetInt();
		if value > 0 then	
			mixShopProcess:SetRemainItemCount(value);
			mixShopProcess:StartServerTime();
		end
	end
end

function MixShopAddOne()
	local numEdit = UIManager:FindUIEditBox("MixShopMake2Count");
	local temp = UIManager:GetUIItem("MixShopMake2Count");
	if numEdit ~= nil then
		local value = numEdit:GetInt();
		value = value + 1;
		numEdit:SetText(value.."");
		numEdit:Redraw();
		UIManager:AddDirtyUIItem(temp);
	end
end

function MixShopSubOne()
   	local numEdit = UIManager:FindUIEditBox("MixShopMake2Count");
	local temp = UIManager:GetUIItem("MixShopMake2Count");
	if numEdit ~= nil then
		local value = numEdit:GetInt();
		if value > 0 then
			value = value - 1;
		end
		
		numEdit:SetText(value.."");
		numEdit:Redraw();
		UIManager:AddDirtyUIItem(temp);
	end
end

function MixShopStartMixAll()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:SetMakeAll(true);
		mixShopProcess:StartServerTime();
	end
end

function IronShopStartCancel()
	local blockImg = UIManager:GetUIItem("IronShopBlockImage");
	if blockImg ~= nil then
		blockImg:SetEnable(false);
		UIManager:AddDirtyUIItem(blockImg);		
	end
    local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
	    if mixShopProcess:IsStartMaking() then
	        mixShopProcess:CancelMake();
	    else
	        mixShopProcess:CancelMake();
	        CloseIronShopMake();
	    end
	end
end

function CancelMixShopMaking2()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
	    if mixShopProcess:IsStartMaking() then
	        mixShopProcess:CancelMake();
	    
	    else
	        mixShopProcess:CancelMake();
	        CloseMixShopMake();
	    end
	end
end

function MixShopMinStack()
    local numEdit = UIManager:FindUIEditBox("NumberOfMixShopSourceItem");
	local temp = UIManager:GetUIItem("NumberOfMixShopSourceItem");
	if numEdit ~= nil and temp ~= nil then
 		numEdit:SetText("1");
		numEdit:Redraw();
		UIManager:AddDirtyUIItem(temp);
	end
end

function MixShopMixAddOne()
    local mixShopProcess = LuaUI.GetMixProcess();
    local numEdit = UIManager:FindUIEditBox("NumberOfMixShopSourceItem");
	local temp = UIManager:GetUIItem("NumberOfMixShopSourceItem");
	if numEdit ~= nil and temp ~= nil and mixShopProcess ~= nil then
		local value = numEdit:GetInt();
		local stoneMaxValue = mixShopProcess:GetMaxStoneNumByItemSlot(_G.GivenItemOrgIndex + 24 * _G.BagState);
		local itemMaxValue = LuaUI.GetMaxStack(_G.GivenItemOrgIndex + 24 * _G.BagState);
		local maxValue = 0;
		if stoneMaxValue > itemMaxValue then
		    maxValue = itemMaxValue;
		else
		    maxValue = stoneMaxValue
		end
		if value < maxValue then
			value = value + 1;
		else
		    value = maxValue;
		end

		numEdit:SetText(value.."");
		numEdit:Redraw();
		UIManager:AddDirtyUIItem(temp);
	end
end

function MixShopMixDiffOne()
    local numEdit = UIManager:FindUIEditBox("NumberOfMixShopSourceItem");
	local temp = UIManager:GetUIItem("NumberOfMixShopSourceItem");
	if numEdit ~= nil and temp ~= nil then
		local value = numEdit:GetInt();
		if value > 1 then
			value = value - 1;
		end

		numEdit:SetText(value.."");
		numEdit:Redraw();
		UIManager:AddDirtyUIItem(temp);
	end
end

function MixShopMaxStack()
	local mixShopProcess = LuaUI.GetMixProcess();
    local numEdit = UIManager:FindUIEditBox("NumberOfMixShopSourceItem");
	local temp = UIManager:GetUIItem("NumberOfMixShopSourceItem");
	if mixShopProcess ~= nil and numEdit ~= nil and temp ~= nil then
     local stoneMaxValue = mixShopProcess:GetMaxStoneNumByItemSlot(_G.GivenItemOrgIndex + 24 * _G.BagState);
		local itemMaxValue = LuaUI.GetMaxStack(_G.GivenItemOrgIndex + 24 * _G.BagState);
		local maxValue = 0;
		if stoneMaxValue > itemMaxValue then
		    maxValue = itemMaxValue;
		else
		    maxValue = stoneMaxValue
		end
    	numEdit:SetText(maxValue.."");
		numEdit:Redraw();
		UIManager:AddDirtyUIItem(temp);
	end
end

function CloseMixShopCount()
   ResetKeyIn();
    local countPanel = UIManager:GetUIItem("MixShopCountPanel");
    local numEdit = UIManager:FindUIEditBox("NumberOfMixShopSourceItem");
    if countPanel ~= nil and numEdit ~= nil then
    	countPanel:SetEnable(false);
    	numEdit:SetText("1");
		numEdit:Redraw();
    end

    _G.GivenItemOrgIndex = -1;
    _G.PlacedItemIndex = -1;
    UIManager:ClearMonoPanel();
end

function CancelMixShopGem()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
	    if mixShopProcess:IsStartMaking() then
	        mixShopProcess:CancelMake();
			
	        local temp = UIManager:GetUIItem("MixShopGemPanel");
	        if temp ~= nil then
				if mixShopProcess:IsGemMixShop() then
					local button = UIManager:FindUIButton("MixShopGemMix");
					button:SetNormalState();
				elseif mixShopProcess:IsGemInlayShop() then
					local button = UIManager:FindUIButton("MixShopGemEqInlay");
					button:SetNormalState();
				elseif mixShopProcess:IsGemRemoveShop() then
					local button = UIManager:FindUIButton("MixShopGemEqRemove");
					button:SetNormalState();
				elseif mixShopProcess:IsGemMeltingShop() then
					local button = UIManager:FindUIButton("MixShopGemMelting");
					button:SetNormalState();
				elseif mixShopProcess:IsGemPolishingShop() then
					local button = UIManager:FindUIButton("MixShopGemPolishing");
					button:SetNormalState();
				elseif mixShopProcess:IsColorStoneMixShop() then
					local button = UIManager:FindUIButton("MixShopColorStoneMix");
					button:SetNormalState();
				end
	        	
				UIManager:AddDirtyUIItem(temp);
	        end
	    else
	        mixShopProcess:CancelMake();
	        CloseMixShopGem();
	    end
	end
end

function ConfirmGemMixShopItemNum()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
	    if mixShopProcess:IsGemMixShop() then
		   mixShopProcess:SetGemMixShopItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 24 * _G.BagState, 1);
		elseif mixShopProcess:IsGemInlayShop() then
			mixShopProcess:SetGemMixShopItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 24 * _G.BagState, 1);
		elseif mixShopProcess:IsGemRemoveShop() then
			mixShopProcess:SetGemMixShopItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 24 * _G.BagState, 1);
		elseif mixShopProcess:IsGemMeltingShop() then
			mixShopProcess:SetGemMixShopItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 24 * _G.BagState, 1);
		elseif mixShopProcess:IsGemPolishingShop() then
			mixShopProcess:SetGemMixShopItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 24 * _G.BagState, 1);
		elseif mixShopProcess:IsColorStoneMixShop() then
			mixShopProcess:SetGemMixShopItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex + 24 * _G.BagState, 1);
		end
	end
	CloseMixShopCount();
	_G.GivenItemOrgIndex = -1;
    _G.PlacedItemIndex = -1;
end

function MixShopGemStartMix()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:StartServerTime();
	end
end

function MixShopGemStartEqInlay()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:StartServerTime();
	end
end

function MixShopGemStartEqRemove()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:StartServerTime();
	end
end

function MixShopGemStartMelting()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:StartServerTime();
	end
end

function MixShopGemStartPolishing()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:StartServerTime();
	end
end

function IronShopStartMake()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:StartServerTime();
	end
end

function IronShopStartRepair()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:StartServerTime();
	end
end

function MixShopStartColorStoneMix()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
		mixShopProcess:StartServerTime();
	end
end

function MixShopGemRemoveGemItemMove00()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
	    if mixShopProcess:IsGemRemoveShop() then
			mixShopProcess:SetGemRemoveIndex(0);
		end
	end
end

function MixShopGemRemoveGemItemMove01()
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
	    if mixShopProcess:IsGemRemoveShop() then
			mixShopProcess:SetGemRemoveIndex(1);
		end
	end
end

function MixItemDetail(pMouseIn, pItemPos)
	local XOffset = 60;
	local YOffset = 8;
	local pX = UIManager:GetCursorX();
	local pY = UIManager:GetCursorY();
	local panel = UIManager:GetUIItem("MixDetailMenuPanel");

	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MixItemDetail.xml");
		panel = UIManager:GetUIItem("MixDetailMenuPanel")
		local mixShopProcess = LuaUI.GetMixProcess();
		if mixShopProcess ~= nil then
			mixShopProcess:UpdateMixItemDetail(pItemPos)
		end

		local width = panel:GetWidth();
		local height = panel:GetHeight();
		
		m_ItemDetailX = pX + XOffset;
		m_ItemDetailY = pY + YOffset;
		if m_ItemDetailY + height > 599 then
			m_ItemDetailY = pY - height - YOffset;
		end
		if m_ItemDetailX + width > 799 then
			m_ItemDetailX = pX - width - XOffset;
		end
		if m_ItemDetailY < 0 then
			m_ItemDetailY = 0;
		end
		if m_ItemDetailX < 0 then
			m_ItemDetailX = 0;
		end
		panel:SetPos(m_ItemDetailX, m_ItemDetailY);
		UIManager:AddDirtyUIItem(panel);
	end
end

function MixDetailMouseOut()
	CloseMixDetail(0, 0)
end

function MixOver(itemPos)
	local mixShopProcess = LuaUI.GetMixProcess();
	if mixShopProcess ~= nil then
	    if mixShopProcess:isShowItem(itemPos) then
			MixItemDetail(1, itemPos);
		end
	end
end

function MixOver00()
	MixOver(0);
end

function MixOver01()
	MixOver(1);
end

function MixOver02()
	MixOver(2);
end

function MixOver03()
	MixOver(3);
end

function MixOver04()
	MixOver(4);
end

function MixOver05()
	MixOver(5);
end

function MixOver06()
	MixOver(6);
end

function MixOver07()
	MixOver(7);
end

function MixOver08()
	MixOver(8);
end

function MixOver09()
	MixOver(9);
end