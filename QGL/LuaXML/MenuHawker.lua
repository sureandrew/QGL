HawkerBagState = 0;
HawkerOwnerItemIndex = -1;
HawkerOwnerPPIndex = -1;
HawkerCPPState = 0; --CPP : character, partner, pet; 0:character, 1:partner, 2:pet
HawkerSellMenuIndex = 0; -- 0: item, 1: partner, 2: pet, 3: record
DragHawkerSellItemIndex = -1;
HawkClickedButtonIndex = -1;
HawkItemStack = 0;

function OpenHawkerOwner()
	local panel = UIManager:GetUIItem("HawkerOwnerPanel");

	if panel ~= nil then
	    CloseHawkerOwner();
	else
		LuaUI.AddXMLFile2UIManger("Data\\MenuHawkerOwner.xml");

    	local temp = UIManager:GetUIItem("HawkerOwnerHawk");
    	local temp1 = UIManager:GetUIItem("HawkerOwnerStopHawk");
    	
    	local button = UIManager:FindUIButton("HawkerOwnerChangeName");
    	local tempButton = UIManager:GetUIItem("HawkerOwnerChangeName");
    	
		local hawkerProcess = LuaUI.GetHawkerProcess();
		local autowalkprocess = LuaUI.GetAutoWalkProcess();
		if hawkerProcess ~= nil and temp ~= nil and temp1 ~= nil and button ~= nil and tempButton ~= nil then
			SelectHawkerOwnerCharacter();
			SelectHawkerOwnerCurrentBag();
			SelectHawkerOwnerSellItem();
			
			if hawkerProcess:IsStartHawk() == true then
				temp:SetEnable(false);
				temp1:SetEnable(true);
				button:SetNormalState();
				UIManager:AddDirtyUIItem(tempButton);
			else
			    temp:SetEnable(true);
			    temp1:SetEnable(false);
			    button:SetDisableState();
				UIManager:AddDirtyUIItem(tempButton);
			end
            SetHawkNameKeyIn();
            hawkerProcess:InitItemPartnerPet();
		end
		
		if autowalkprocess ~= nil then
			autowalkprocess:StopAutoWalk();
			OnCloseAutoWalk();
		end
		-- reset the lock and unlock cursor
		if _G.LockState or _G.UnlockState then
			ResetAllLockStatus();
		end
		PlayOpenInterfaceEffect();
		SwitchOffPartnerCollection()
	end
end

function CloseHawkerOwner()
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("HawkerOwnerPanel");
	--local dropPanel = UIManager:GetUIItem("DropItemPanel");
	--local confirmDropPanel = UIManager:GetUIItem("ConfirmDropItemPanel");
	if panel ~= nil then
		CloseHawkerOwnerCount();
		UIManager:DeletePanel("HawkerOwnerPanel");
		UIManager:DeletePanel("HawkerOwnerItemCountPanel");
		
		local hawkerProcess = LuaUI.GetHawkerProcess();
		if hawkerProcess ~= nil then
			hawkerProcess:ResetHawkerOwner();
  		end
		--CloseDropPanel();

		PlayCloseInterfaceEffect();

		panel = UIManager:GetUIItem("HawkerOwnerMovingPanel");
		if panel ~= nil then
			panel:SetEnable(false);
			UIManager:DeletePanel("HawkerOwnerMovingPanel");
		end
		
		panel = UIManager:GetUIItem("HawkerOwnerMovingPanel2");
		if panel ~= nil then
			panel:SetEnable(false);
			UIManager:DeletePanel("HawkerOwnerMovingPanel2");
		end
		
		CloseItemDetail(0, 0);
	end
end

function ResetHawkerOwnerBagState()
	local button = UIManager:FindUIButton("HawkerOwnerCurrentBag");
	local tempButton = UIManager:GetUIItem("HawkerOwnerCurrentBag");
	if button ~= nil and tempButton ~= nil then
		button:SetNormalState();
		UIManager:AddDirtyUIItem(tempButton);
	end
	
	button = UIManager:FindUIButton("HawkerOwnerCurrentBag");
	tempButton = UIManager:GetUIItem("HawkerOwnerCurrentBag");
	if button ~= nil and tempButton ~= nil then
		button:SetNormalState();
		UIManager:AddDirtyUIItem(tempButton);
	end
	
	button = UIManager:FindUIButton("HawkerOwnerSideBag");
	tempButton = UIManager:GetUIItem("HawkerOwnerSideBag");
	if button ~= nil and tempButton ~= nil then
		button:SetNormalState();
		UIManager:AddDirtyUIItem(tempButton);
	end
	
	button = UIManager:FindUIButton("HawkerOwnerBagExtra01");
	tempButton = UIManager:GetUIItem("HawkerOwnerBagExtra01");
	if button ~= nil and tempButton ~= nil then
		button:SetNormalState();
		UIManager:AddDirtyUIItem(tempButton);
	end
	
	button = UIManager:FindUIButton("HawkerOwnerBagExtra02");
	tempButton = UIManager:GetUIItem("HawkerOwnerBagExtra02");
	if button ~= nil and tempButton ~= nil then
		button:SetNormalState();
		UIManager:AddDirtyUIItem(tempButton);
	end
	
	button = UIManager:FindUIButton("HawkerOwnerBagExtra03");
	tempButton = UIManager:GetUIItem("HawkerOwnerBagExtra03");
	if button ~= nil and tempButton ~= nil then
		button:SetNormalState();
		UIManager:AddDirtyUIItem(tempButton);
	end
end

function HawkerOwnerNameListScrollUp()
    local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
	    hawkerProcess:HawkerOwnerNameListScroll(0);
	end
end

function HawkerOwnerNameListScrollDown()
    local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
	    hawkerProcess:HawkerOwnerNameListScroll(1);
	end
end

function HawkerOwnerNameListScroll()
    local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
	    hawkerProcess:HawkerOwnerNameListScroll(2);
	end
end

function SelectHawkerOwnerCurrentBag()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		ResetHawkerOwnerBagState();
		local button = UIManager:FindUIButton("HawkerOwnerCurrentBag");
		local tempButton = UIManager:GetUIItem("HawkerOwnerCurrentBag");
		if button ~= nil and tempButton ~= nil then
			button:SetDisableState();
			UIManager:AddDirtyUIItem(tempButton);
		end
		hawkerProcess:UpdateOwnerItem(0);
		_G.HawkerBagState = 0;
	end
end

function SelectHawkerOwnerSideBag()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		ResetHawkerOwnerBagState();
		local button = UIManager:FindUIButton("HawkerOwnerSideBag");
		local tempButton = UIManager:GetUIItem("HawkerOwnerSideBag");
		if button ~= nil and tempButton ~= nil then
			button:SetDisableState();
			UIManager:AddDirtyUIItem(tempButton);
		end
		hawkerProcess:UpdateOwnerItem(1);
		_G.HawkerBagState = 1;
	end
end

function SelectHawkerOwnerBagExtra1()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		ResetHawkerOwnerBagState();
		local button = UIManager:FindUIButton("HawkerOwnerBagExtra01");
		local tempButton = UIManager:GetUIItem("HawkerOwnerBagExtra01");
		if button ~= nil and tempButton ~= nil then
			button:SetDisableState();
			UIManager:AddDirtyUIItem(tempButton);
		end
		hawkerProcess:UpdateOwnerItem(2);
		_G.HawkerBagState = 2;
	end
end

function SelectHawkerOwnerBagExtra2()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		ResetHawkerOwnerBagState();
		local button = UIManager:FindUIButton("HawkerOwnerBagExtra02");
		local tempButton = UIManager:GetUIItem("HawkerOwnerBagExtra02");
		if button ~= nil and tempButton ~= nil then
			button:SetDisableState();
			UIManager:AddDirtyUIItem(tempButton);
		end
		hawkerProcess:UpdateOwnerItem(3);
		_G.HawkerBagState = 3;
	end
end

function SelectHawkerOwnerBagExtra3()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		ResetHawkerOwnerBagState();
		local button = UIManager:FindUIButton("HawkerOwnerBagExtra03");
		local tempButton = UIManager:GetUIItem("HawkerOwnerBagExtra03");
		if button ~= nil and tempButton ~= nil then
			button:SetDisableState();
			UIManager:AddDirtyUIItem(tempButton);
		end
		hawkerProcess:UpdateOwnerItem(4);
		_G.HawkerBagState = 4;
	end
end

function HawkerOwnerItemMove000()	
	HawkerOwnerItemMove(0);
end

function HawkerOwnerItemMove001()
	HawkerOwnerItemMove(1);
end

function HawkerOwnerItemMove002()
	HawkerOwnerItemMove(2);
end

function HawkerOwnerItemMove003()
	HawkerOwnerItemMove(3);
end

function HawkerOwnerItemMove004()
	HawkerOwnerItemMove(4);
end

function HawkerOwnerItemMove005()
	HawkerOwnerItemMove(5);
end

function HawkerOwnerItemMove006()
	HawkerOwnerItemMove(6);
end

function HawkerOwnerItemMove007()
	HawkerOwnerItemMove(7);
end

function HawkerOwnerItemMove008()
	HawkerOwnerItemMove(8);
end

function HawkerOwnerItemMove009()
	HawkerOwnerItemMove(9);
end

function HawkerOwnerItemMove010()
	HawkerOwnerItemMove(10);
end

function HawkerOwnerItemMove011()
	HawkerOwnerItemMove(11);
end

function HawkerOwnerItemMove012()
	HawkerOwnerItemMove(12);
end

function HawkerOwnerItemMove013()
	HawkerOwnerItemMove(13);
end

function HawkerOwnerItemMove014()
	HawkerOwnerItemMove(14);
end

function HawkerOwnerItemMove015()
	HawkerOwnerItemMove(15);
end

function HawkerOwnerItemMove016()
	HawkerOwnerItemMove(16);
end

function HawkerOwnerItemMove017()
	HawkerOwnerItemMove(17);
end

function HawkerOwnerItemMove018()
	HawkerOwnerItemMove(18);
end

function HawkerOwnerItemMove019()
	HawkerOwnerItemMove(19);
end

function HawkerOwnerItemMove020()
	HawkerOwnerItemMove(20);
end

function HawkerOwnerItemMove021()
	HawkerOwnerItemMove(21);
end

function HawkerOwnerItemMove022()
	HawkerOwnerItemMove(22);
end

function HawkerOwnerItemMove023()
	HawkerOwnerItemMove(23);
end

function HawkerOwnerItemMove(picIndex)
	
	CloseItemDetail(0, 0);
	
	local hawkerProcess = LuaUI.GetHawkerProcess();
	--if hawkerProcess ~= nil and hawkerProcess:IsStartHawk() then
	--	hawkerProcess:ShowHawkMessage(0);
	--	do return end
	--end
	
 	local itemboard = UIManager:GetUIItem("HawkerOwnerBoard");
 	itemboard:SetEnable(false);
	UIManager:AddDirtyUIItem(itemboard);

	local movingPanel = UIManager:GetUIItem("HawkerOwnerMovingPanel");

	local IconName = "";
	local StackName = "";
	local LevelName = "";

 	if picIndex < 24 then
		local zero = "00";
		if picIndex >= 10 then
			zero = "0";
		end
		IconName = "HawkerOwnerSlot"..zero..picIndex.."Icon";
		StackName = "HawkerOwnerSlot"..zero..picIndex.."Stack";
		LevelName = "HawkerOwnerSlot"..zero..picIndex.."Lvl";
	else
	    local subIndex = picIndex - 120;
    	IconName = "HawkerOwnerEquip00"..subIndex.."Icon";
		StackName = "HawkerOwnerEquip00"..subIndex.."Stack";
		LevelName = "HawkerOwnerEquip00"..subIndex.."Lvl";
	end

	local orgIcon = UIManager:GetUIItem(IconName);
	local orgStack = UIManager:GetUIItem(StackName);
	local orgLevel = UIManager:GetUIItem(LevelName);
	
	local itemProcess = LuaUI.GetItemProcess();
	
	if itemProcess ~= nil then
		local itemSlot = 0;
		if picIndex < 24 then
			itemSlot = picIndex + 24 * _G.HawkerBagState;
		else
			itemSlot = picIndex;
		end
		
		if itemProcess:IsItemGiven(itemSlot) == true or itemProcess:IsItemBind(itemSlot) == true or itemProcess:IsItemSent(itemSlot) == true then
		    hawkerProcess:ShowHawkMessage(1);
		    do return end
		end
	
		if movingPanel ~= nil and orgIcon ~= nil then
	
			local orgAniIcon = UIManager:FindUIAniIcon(IconName);
			local targetAniIcon = UIManager:FindUIAniIcon("HawkerOwnerMovingIcon");
			
			if orgAniIcon ~= nil and targetAniIcon ~= nil then
			    targetAniIcon:ClearAniData();
			    targetAniIcon:SetAnimationData(orgAniIcon);
			end
			
			UIManager:SetDragItemXY(movingPanel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
	
			_G.DragItemPicIndex = picIndex;
					
			orgIcon:SetRenderStyle(1);
			_G.DragType = 0;
			movingPanel:SetEnable(true);
			UIManager:AddDirtyUIItem(movingPanel);
			
			PlayDragItemUpEffect();
		end
		
	end
end

function ResetHawkerOwnerDrag()
	UIManager:ResetDragItem();
	UIManager:StopDrag();
	ResetHawkerOwnerDragedOrgIcon();
end

function MoveHawkerOwnerItemToBag(index)
   local bag1 = UIManager:GetUIItem("HawkerOwnerCurrentBag");
   local bag2 = UIManager:GetUIItem("HawkerOwnerSideBag");
   local bag3 = UIManager:GetUIItem("HawkerOwnerBagExtra01");
   local bag4 = UIManager:GetUIItem("HawkerOwnerBagExtra02");
   local bag5 = UIManager:GetUIItem("HawkerOwnerBagExtra03");

   if bag1 ~= nil and bag2 ~= nil and bag3 ~= nil and bag4 ~= nil and bag5 ~= nil then
		local X = UIManager:GetCursorX();
		local Y = UIManager:GetCursorY();

        if bag1:GetX() <= X and (bag1:GetX()+40) >= X and bag1:GetY() <= Y and (bag1:GetY()+23) >= Y then
			if index >= 24 then
				LuaUI.MoveToBag(index,0);
			else
				ResetHawkerOwnerDrag();
			end
		elseif bag2:GetX() <= X and (bag2:GetX()+40) >= X and bag2:GetY() <= Y and (bag2:GetY()+23) >= Y then
			if index < 24 or index >= 48 then
				LuaUI.MoveToBag(index,1);
			else
				ResetHawkerOwnerDrag();
			end
		elseif bag3:GetX() <= X and (bag3:GetX()+40) >= X and bag3:GetY() <= Y and (bag3:GetY()+23) >= Y then
           LuaUI.MoveToBag(index,2);

		elseif bag4:GetX() <= X and (bag4:GetX()+40) >= X and bag4:GetY() <= Y and (bag4:GetY()+23) >= Y then
           LuaUI.MoveToBag(index,3);

		elseif bag5:GetX() <= X and (bag5:GetX()+40) >= X and bag5:GetY() <= Y and (bag5:GetY()+23) >= Y then
           LuaUI.MoveToBag(index,4);

		else
			local noMove = true;

			local bgImage = UIManager:GetUIItem("HawkerOwnerSlotBg");
			local X2 =  UIManager:GetCursorX() - bgImage:GetX();
			local Y2 =  UIManager:GetCursorY() - bgImage:GetY();

			if X2 > 0 and X2 < bgImage:GetWidth()
			    and Y2 > 0 and Y2 < bgImage:GetHeight()
				then

				local bagIndex = LuaUI.Div(X2, 41) + 6 * LuaUI.Div(Y2, 41);
				
				local itemProcess = LuaUI.GetItemProcess();
				if itemProcess ~= nil and itemProcess:IsItemGiven(bagIndex + 24 * _G.HawkerBagState) ~= true and 
				itemProcess:IsItemBind(bagIndex + 24 * _G.HawkerBagState) == false and itemProcess:IsItemSent(bagIndex + 24 * _G.HawkerBagState) == false then
    				LuaUI.MoveToBag(index, _G.BagState, bagIndex);
    			else
    				ResetHawkerOwnerDrag();
    			end
  			
    			noMove = false;
			end
			if noMove  then
				local zero = "00";
				if _G.DragItemPicIndex >= 10 then
					zero = "0";
				end 
			    local itemIcon = UIManager:GetUIItem("HawkerOwnerSlot"..zero.._G.DragItemPicIndex.."Icon");
			    local itemStack = UIManager:GetUIItem("HawkerOwnerSlot"..zero.._G.DragItemPicIndex.."Stack");
			    local itemLevel = UIManager:GetUIItem("HawkerOwnerSlot"..zero.._G.DragItemPicIndex.."Lvl");
			    
			    itemIcon:SetEnable(true);
			    itemStack:SetEnable(true);
			    itemLevel:SetEnable(true);
			end
		
		end
   end
end

function HawkerOwnerDragEnd(value)
	if _G.DragType == 0 then
	    HawkerOwnerItemDragEnd(value);
	elseif _G.DragType == 1 or _G.DragType == 2 then
		if value == 3 then
	    	ResetHawkerOwnerDrag();
	    else
	    	local panel = UIManager:GetUIItem("HawkerOwnerPanel");
			local mx = UIManager:GetCursorX() - panel:GetX();
			local my = UIManager:GetCursorY() - panel:GetY();
		   	
		   	local topborder = UIManager:GetUIItem("HawkerOwnerTopBorder");
		   	local rightborder = UIManager:GetUIItem("HawkerOwnerRightBorder");
		   	local bottomborder = UIManager:GetUIItem("HawkerOwnerBottomBorder");
		   	
		   	if mx > topborder:GetOffsetX() and my > topborder:GetOffsetY() and 
				   mx < rightborder:GetOffsetX() and my < bottomborder:GetOffsetY() then
				HawkerOwnerHawkPP();
			end	
			ResetHawkerOwnerDrag();
		end
	end
end



function HawkerOwnerItemDragEnd(value)
	if value == 3 then
	    ResetHawkerOwnerDrag();
	else
		local panel = UIManager:GetUIItem("HawkerOwnerPanel");
		if panel ~= nil then
			local mx = UIManager:GetCursorX() - panel:GetX();
			local my = UIManager:GetCursorY() - panel:GetY();
		   	local bag1 = UIManager:GetUIItem("HawkerOwnerCurrentBag");
		   	local bgImage = UIManager:GetUIItem("HawkerOwnerSlotBg");
		   	
		   	local topborder = UIManager:GetUIItem("HawkerOwnerTopBorder");
		   	local rightborder = UIManager:GetUIItem("HawkerOwnerRightBorder");
		   	local bottomborder = UIManager:GetUIItem("HawkerOwnerBottomBorder");
		   	
		   	local imx = mx - bag1:GetOffsetX();
		   	local imy = my - bag1:GetOffsetY();
		   	if bag1 ~= nil and bgImage ~= nil and imx > 0 and imy > 0 
			   and imx < (bgImage:GetOffsetX() + bgImage:GetWidth()) 
			   and imy < (bgImage:GetOffsetY() + bgImage:GetHeight()) then
			   
		   		MoveHawkerOwnerItemToBag(_G.DragItemPicIndex + 24 * _G.HawkerBagState);
		   	elseif mx > topborder:GetOffsetX() and my > topborder:GetOffsetY() and 
			   mx < rightborder:GetOffsetX() and my < bottomborder:GetOffsetY() then
			    HawkerOwnerHawkItem(_G.DragItemPicIndex);
		   	end
	   	end
	   	
	end
	ResetHawkerOwnerDrag();
end 

function ResetHawkerOwnerDragedOrgIcon()
	local movingPanel;
	
	if _G.DragHawkerSellItemIndex >= 0 then
		movingPanel = UIManager:GetUIItem("HawkerOwnerMovingPanel2");
	else
		movingPanel = UIManager:GetUIItem("HawkerOwnerMovingPanel");
	end
	
	if movingPanel ~= nil then
		movingPanel:SetPos(0,0);
		movingPanel:SetEnable(false);
		movingPanel:SetNotDetected(false);
	end
	
	if _G.DragHawkerSellItemIndex >= 0 then
			
		local iconName = "";
	
		if _G.DragHawkerSellItemIndex >= 10 then
			iconName = "HawkerOwnerSell0".._G.DragHawkerSellItemIndex.."Icon" ;
		else
		    iconName = "HawkerOwnerSell00".._G.DragHawkerSellItemIndex.."Icon" ;
		end
		
		local itemIcon = UIManager:GetUIItem(iconName);
		if itemIcon ~= nil then
			itemIcon:SetRenderStyle(0);
			itemIcon:SetEnable(true);
		end
		_G.DragHawkerSellItemIndex = -1;
	else
	
		if _G.DragType == 0 then
			local IconName = "";
			local StackName = "";
			local LevelName = "";
	
			if _G.DragItemPicIndex < 24 and _G.DragItemPicIndex >= 0 then
				local zero = "00";
				if _G.DragItemPicIndex >= 10 then
					zero = "0";
				end
				IconName = "HawkerOwnerSlot"..zero.._G.DragItemPicIndex.."Icon";
				StackName = "HawkerOwnerSlot"..zero.._G.DragItemPicIndex.."Stack";
				LevelName = "HawkerOwnerSlot"..zero.._G.DragItemPicIndex.."Lvl";
			else
			    local subIndex = _G.DragItemPicIndex - 120;
			    IconName = "HawkerOwnerEquip00"..subIndex.."Icon";
				StackName = "HawkerOwnerEquip00"..subIndex.."Stack";
				LevelName = "HawkerOwnerEquip00"..subIndex.."Lvl";
			end
	
			local itemIcon = UIManager:GetUIItem(IconName);
			local itemStack = UIManager:GetUIItem(StackName);
			local itemLevel = UIManager:GetUIItem(LevelName);
	
			if itemIcon ~= nil and itemStack ~= nil and itemLevel ~= nil then
				local itemProcess = LuaUI.GetItemProcess();
				if itemProcess ~= nil then
					if itemProcess:IsItemGiven(_G.DragItemPicIndex + 24 * _G.HawkerBagState) then
						itemIcon:SetRenderStyle(1);
					else
						itemIcon:SetRenderStyle(0);
					end
				end
				itemIcon:SetEnable(true);
				itemStack:SetEnable(true);
				itemLevel:SetEnable(true);
			end
	
			_G.DragItemPicIndex = -1;
		end
		
		_G.DragType = -1;
	end
end

function ResetHawkerOwnerMenuButton()
	local button = UIManager:FindUIButton("HawkerOwnerCharacter");
	local tempButton = UIManager:GetUIItem("HawkerOwnerCharacter");
	if button ~= nil and tempButton ~= nil then
		button:SetNormalState();
		UIManager:AddDirtyUIItem(tempButton);
	end
	button = UIManager:FindUIButton("HawkerOwnerPartner");
	tempButton = UIManager:GetUIItem("HawkerOwnerPartner");
	if button ~= nil and tempButton ~= nil then
		button:SetNormalState();
		UIManager:AddDirtyUIItem(tempButton);
	end
	
	button = UIManager:FindUIButton("HawkerOwnerPet");
	tempButton = UIManager:GetUIItem("HawkerOwnerPet");
	if button ~= nil and tempButton ~= nil then
		button:SetNormalState();
		UIManager:AddDirtyUIItem(tempButton);
	end
end

function HawkerOwnerPetMenuSetEnable(flag)
    ItemMenuSetEnable("HawkerOwnerPetPanelImage1", flag);
	ItemMenuSetEnable("HawkerOwnerPetPanelImage2", flag);
	ItemMenuSetEnable("HawkerOwnerPetHappyBar", flag);
	ItemMenuSetEnable("HawkerOwnerPetVPBar", flag);
	ItemMenuSetEnable("HawkerOwnerPetLVText", flag);
	ItemMenuSetEnable("HawkerOwnerPetHappyText", flag);
    ItemMenuSetEnable("HawkerOwnerPetVPText", flag);
end

function HawkerOwnerPartnerMenuSetEnable(flag)
    ItemMenuSetEnable("HawkerOwnerPartnerPanelImage1", flag);
	ItemMenuSetEnable("HawkerOwnerPartnerPanelImage2", flag);
	ItemMenuSetEnable("HawkerOwnerPartnerHPBar", flag);
	ItemMenuSetEnable("HawkerOwnerPartnerSPBar", flag);
    ItemMenuSetEnable("HawkerOwnerPartnerLikeText", flag);
	ItemMenuSetEnable("HawkerOwnerPartnerHPText", flag);
	ItemMenuSetEnable("HawkerOwnerPartnerSPText", flag);
end

function HawkerOwnerMenuSetEnable(flag)
	ItemMenuSetEnable("HawkerOwnerEquipPanel", flag);
	ItemMenuSetEnable("HawkerOwnerMoneyInterface1", flag);
	ItemMenuSetEnable("HawkerOwnerMoneyInterface2", flag);
	ItemMenuSetEnable("HawkerOwnerSCashDollar", flag);
	ItemMenuSetEnable("HawkerOwnerCashDollar", flag);
	ItemMenuSetEnable("HawkerOwnerBankDollar", flag);
	ItemMenuSetEnable("HawkerOwnerGoldDollar", flag);
end

function HawkerOwnerNameListMenuSetEnable(flag)
	for i=1, 6 do
		local word = "HawkerOwnerNameListLine"..i;
		ItemMenuSetEnable(word, flag);
		word = "HawkerOwnerNameImage"..i;
		ItemMenuSetEnable(word, false);
		word = "HawkerOwnerNameSelected"..i;
		ItemMenuSetEnable(word, false);
		ItemMenuSetEnable("HawkerOwnerPartnerState"..i.."1", false);
		ItemMenuSetEnable("HawkerOwnerPartnerState"..i.."2", false);
		ItemMenuSetEnable("HawkerOwnerPartnerSell"..i,false);
		local lockname = "HawkerPetPartnerLock"..i;
		ItemMenuSetEnable(lockname, false); 		
	end
    
	ItemMenuSetEnable("HawkerOwnerNameListPanel", flag);
	ItemMenuSetEnable("HawkerOwnerNameListScroll", flag);

end

function SelectHawkerOwnerCharacter()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		ResetHawkerOwnerMenuButton();
		HawkerOwnerPetMenuSetEnable(false);	
		HawkerOwnerNameListMenuSetEnable(false);
		HawkerOwnerPartnerMenuSetEnable(false);
		HawkerOwnerMenuSetEnable(true);
		local button = UIManager:FindUIButton("HawkerOwnerCharacter");
		local tempButton = UIManager:GetUIItem("HawkerOwnerCharacter");
		if button ~= nil and tempButton ~= nil then
			button:SetDisableState();
			UIManager:AddDirtyUIItem(tempButton);
		end
		hawkerProcess:UpdateOwner();
		_G.HawkerCPPState = 0;
	end
end

function SelectHawkerOwnerPartner()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		ResetHawkerOwnerMenuButton();
		HawkerOwnerPetMenuSetEnable(false);	
		HawkerOwnerNameListMenuSetEnable(true);
		HawkerOwnerPartnerMenuSetEnable(true);
		HawkerOwnerMenuSetEnable(false);
		
		for i=0,8 do
	  		IconSetEnable("HawkerOwnerEquip00"..i.."Icon", false, 120+i);
			ItemMenuSetEnable("HawkerOwnerEquip00"..i.."Stack", false);
			ItemMenuSetEnable("HawkerOwnerEquip00"..i.."Lvl", false);
		end		
		local button = UIManager:FindUIButton("HawkerOwnerPartner");
		local tempButton = UIManager:GetUIItem("HawkerOwnerPartner");
		if button ~= nil and tempButton ~= nil then
			button:SetDisableState();
			UIManager:AddDirtyUIItem(tempButton);
		end
		hawkerProcess:UpdateOwnerPartner();
		hawkerProcess:ResetOwnerScroll();
		_G.HawkerCPPState = 1;
	end	
end

function SelectHawkerOwnerPet()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		ResetHawkerOwnerMenuButton();
		HawkerOwnerPetMenuSetEnable(true);	
		HawkerOwnerNameListMenuSetEnable(true);
		HawkerOwnerPartnerMenuSetEnable(false);
		HawkerOwnerMenuSetEnable(false);
		
		for i=0,8 do
	  		IconSetEnable("HawkerOwnerEquip00"..i.."Icon", false, 120+i);
			ItemMenuSetEnable("HawkerOwnerEquip00"..i.."Stack", false);
			ItemMenuSetEnable("HawkerOwnerEquip00"..i.."Lvl", false);
		end	
		local button = UIManager:FindUIButton("HawkerOwnerPet");
		local tempButton = UIManager:GetUIItem("HawkerOwnerPet");
		if button ~= nil and tempButton ~= nil then
			button:SetDisableState();
			UIManager:AddDirtyUIItem(tempButton);
		end
		hawkerProcess:UpdateOwnerPet();
		hawkerProcess:ResetOwnerScroll();
		_G.HawkerCPPState = 2;
	end
end

function ShowHawkerOwnerBoard()
	local panel = UIManager:GetUIItem("HawkerOwnerBoard");
	local hawkerPanel = UIManager:GetUIItem("HawkerOwnerPanel");
	if panel ~= nil and hawkerPanel ~= nil then
		if (UIManager:GetCursorX() > (panel:GetOffsetX() + hawkerPanel:GetX() ) and UIManager:GetCursorX() < ( panel:GetOffsetX() + 40 + hawkerPanel:GetX() )
			and UIManager:GetCursorY() > (panel:GetOffsetY() + hawkerPanel:GetY() ) and UIManager:GetCursorY() < (panel:GetOffsetY() + 40 + hawkerPanel:GetY() )) then
			ItemDetail(1, _G.ItemIndexNow);			
		else
			panel:SetOffsetXY(0,0);
		    panel:SetEnable(false);		    
			CloseItemDetail(0, 0);
		end
		panel:SetEnableTimeOutEvent(false);	
	end
end

function HawkerOwnerItemOver(itemNo)
	local itemName = "";
	if itemNo < 24 then
		if itemNo < 10 then
			itemName = "HawkerOwnerSlot00"..itemNo;
		elseif itemNo >= 10 then
			itemName = "HawkerOwnerSlot0"..itemNo;
		end
	else
	    local subIndex = itemNo - 120;
        itemName = "HawkerOwnerEquip00"..subIndex;
    end

	itemName = itemName.."Icon";
	local itemboard = UIManager:GetUIItem("HawkerOwnerBoard");
	local item = UIManager:GetUIItem(itemName);

	if itemboard:GetOffsetX() ~= (item:GetOffsetX() - 1) or itemboard:GetOffsetY() ~= (item:GetOffsetY() - 1) then
		itemboard:SetOffsetXY(item:GetOffsetX() - 1 , item:GetOffsetY() - 1 );
		itemboard:SetEnable(true);
		UIManager:AddDirtyUIItem(itemboard);
		itemboard:SetEnableTimeOutEvent(true);
		itemboard:ResetTimer();
	end
	

	if itemNo < 24 then
		_G.ItemIndexNow = itemNo + 24 * _G.HawkerBagState;
 	else
 	    _G.ItemIndexNow = itemNo;
	end
	
end

function HawkerOwnerItemOver000()
	HawkerOwnerItemOver(0);
end

function HawkerOwnerItemOver001()
	HawkerOwnerItemOver(1);
end

function HawkerOwnerItemOver002()
	HawkerOwnerItemOver(2);
end

function HawkerOwnerItemOver003()
	HawkerOwnerItemOver(3) ;
end

function HawkerOwnerItemOver004()
	HawkerOwnerItemOver(4);
end

function HawkerOwnerItemOver005()
	HawkerOwnerItemOver(5);
end

function HawkerOwnerItemOver006()
	HawkerOwnerItemOver(6);
end

function HawkerOwnerItemOver007()
	HawkerOwnerItemOver(7);
end

function HawkerOwnerItemOver008()
	HawkerOwnerItemOver(8);
end

function HawkerOwnerItemOver009()
	HawkerOwnerItemOver(9);
end

function HawkerOwnerItemOver010()
	HawkerOwnerItemOver(10);
end

function HawkerOwnerItemOver011()
	HawkerOwnerItemOver(11);
end

function HawkerOwnerItemOver012()
	HawkerOwnerItemOver(12);
end

function HawkerOwnerItemOver013()
	HawkerOwnerItemOver(13);
end

function HawkerOwnerItemOver014()
	HawkerOwnerItemOver(14);
end

function HawkerOwnerItemOver015()
	HawkerOwnerItemOver(15)
end

function HawkerOwnerItemOver016()
	HawkerOwnerItemOver(16);
end

function HawkerOwnerItemOver017()
	HawkerOwnerItemOver(17);
end

function HawkerOwnerItemOver018()
	HawkerOwnerItemOver(18);
end

function HawkerOwnerItemOver019()
	HawkerOwnerItemOver(19);
end

function HawkerOwnerItemOver020()
	HawkerOwnerItemOver(20);
end

function HawkerOwnerItemOver021()
	HawkerOwnerItemOver(21);
end

function HawkerOwnerItemOver022()
	HawkerOwnerItemOver(22);
end

function HawkerOwnerItemOver023()
	HawkerOwnerItemOver(23);
end

function EquipHawkerOwnerOver000()
	HawkerOwnerItemOver(120);
end

function EquipHawkerOwnerOver001()
	HawkerOwnerItemOver(121);
end

function EquipHawkerOwnerOver002()
	HawkerOwnerItemOver(122);
end

function EquipHawkerOwnerOver003()
	HawkerOwnerItemOver(123);
end

function EquipHawkerOwnerOver004()
	HawkerOwnerItemOver(124);
end                               

function EquipHawkerOwnerOver005()
	HawkerOwnerItemOver(125);
end

function EquipHawkerOwnerOver006()
	HawkerOwnerItemOver(126);
end

function EquipHawkerOwnerOver007()
	HawkerOwnerItemOver(127);
end

function EquipHawkerOwnerOver008()
	HawkerOwnerItemOver(128);
end

function HawkerOwnerDetailMouseOut()
	local panel = UIManager:GetUIItem("HawkerOwnerBoard");
	if panel ~= nil then
		panel:SetEnableTimeOutEvent(true);
		panel:ResetTimer();	
	end
	CloseItemDetail(0, 0);
end


function HawkerOwnerHawkItem(index)
	
	local hawkerProcess = LuaUI.GetHawkerProcess();
	--if hawkerProcess ~= nil and hawkerProcess:IsStartHawk() then
	--	hawkerProcess:ShowHawkMessage(0);
	--	do return end
	--end

	local itemProcess = LuaUI.GetItemProcess();
	local itemSlot = index + 24 * _G.HawkerBagState
	if itemProcess ~= nil and itemProcess:IsItemGiven(itemSlot) ~= true and itemProcess:IsItemBind(itemSlot) ~= true and itemProcess:IsItemSent(itemSlot) ~= true and _G.HawkerBagState == 0 then
		_G.HawkerOwnerItemIndex = index;
		CloseItemDetail(0, 0);
		local countPanel = UIManager:GetUIItem("HawkerOwnerItemCountPanel");
		local tempPanel = UIManager:FindUIPanel("HawkerOwnerItemCountPanel");
		if countPanel ~= nil and tempPanel ~= nil then
			UIManager:SetPanel2Top(tempPanel);
			countPanel:SetEnable(true);
			UIManager:SetKeyInItem("PriceOfHawkerItem");
			local priceEdit = UIManager:FindUIEditBox("PriceOfHawkerItem");
			if priceEdit ~= nil then
				priceEdit:End();
				priceEdit:Redraw();
			end
			UIManager:ClearMonoPanel();
			UIManager:SetMonoPanel(tempPanel);	
			
			local temp = UIManager:GetUIItem("NumberOfHawkerItemBg");
			local temp1 = UIManager:GetUIItem("HawkerItemCountText");
			local temp2 = UIManager:GetUIItem("NumberOfHawkerItem");
			local temp3 = UIManager:GetUIItem("HawkerOwnerMaxStack");
			local temp4 = UIManager:GetUIItem("HawkerOwnerSetMin");
			local temp5 = UIManager:GetUIItem("AddOneHawkerOwnerStack");
			local temp6 = UIManager:GetUIItem("DiffOneHawkerOwnerStack");
			
			if temp ~= nil and temp1 ~= nil and temp2 ~= nil and temp3 ~= nil and 
				temp4 ~= nil and temp5 ~= nil and temp6 ~= nil then
				temp:SetEnable(true);	
				temp1:SetEnable(true);
				temp2:SetEnable(true);
				temp3:SetEnable(true);
				temp4:SetEnable(true);
				temp5:SetEnable(true);
				temp6:SetEnable(true);
			end
			
			UIManager:AddDirtyUIItem(countPanel);
	    end
    end
	
end

function HawkerOwnerHawk000()
	HawkerOwnerHawkItem(0);
end

function HawkerOwnerHawk001()
	HawkerOwnerHawkItem(1);
end

function HawkerOwnerHawk002()
	HawkerOwnerHawkItem(2);
end

function HawkerOwnerHawk003()
	HawkerOwnerHawkItem(3);
end

function HawkerOwnerHawk004()
	HawkerOwnerHawkItem(4);
end

function HawkerOwnerHawk005()
	HawkerOwnerHawkItem(5);
end

function HawkerOwnerHawk006()
	HawkerOwnerHawkItem(6);
end

function HawkerOwnerHawk007()
	HawkerOwnerHawkItem(7);
end

function HawkerOwnerHawk008()
	HawkerOwnerHawkItem(8);
end

function HawkerOwnerHawk009()
	HawkerOwnerHawkItem(9);
end

function HawkerOwnerHawk010()
	HawkerOwnerHawkItem(10);
end

function HawkerOwnerHawk011()
	HawkerOwnerHawkItem(11);
end

function HawkerOwnerHawk012()
	HawkerOwnerHawkItem(12);
end

function HawkerOwnerHawk013()
	HawkerOwnerHawkItem(13);
end

function HawkerOwnerHawk014()
	HawkerOwnerHawkItem(14);
end

function HawkerOwnerHawk015()
	HawkerOwnerHawkItem(15);
end

function HawkerOwnerHawk016()
	HawkerOwnerHawkItem(16);
end

function HawkerOwnerHawk017()
	HawkerOwnerHawkItem(17);
end

function HawkerOwnerHawk018()
	HawkerOwnerHawkItem(18);
end

function HawkerOwnerHawk019()
	HawkerOwnerHawkItem(19);
end

function HawkerOwnerHawk020()
	HawkerOwnerHawkItem(20);
end

function HawkerOwnerHawk021()
	HawkerOwnerHawkItem(21);
end

function HawkerOwnerHawk022()
	HawkerOwnerHawkItem(22);
end

function HawkerOwnerHawk023()
	HawkerOwnerHawkItem(23);
end

function SetKeyInPriceHawkerItem()
	UIManager:SetKeyInItem("PriceOfHawkerItem");
end

function SetKeyInNumHawkerItem()
	UIManager:SetKeyInItem("NumberOfHawkerItem");
end

function CloseHawkerOwnerCount()
	ResetKeyIn();
    local countPanel = UIManager:GetUIItem("HawkerOwnerItemCountPanel");
    local numEdit = UIManager:FindUIEditBox("PriceOfHawkerItem");
    local numEdit1 = UIManager:FindUIEditBox("NumberOfHawkerItem");
    if countPanel ~= nil and numEdit ~= nil and numEdit1 ~= nil then
    	countPanel:SetEnable(false);
    	--numEdit:SetText("1");
		SetPutItemMoneyColour();
		numEdit:Redraw();
		numEdit1:SetText("1");
		numEdit1:End();
		numEdit1:Redraw();
    end
    
    _G.HawkerOwnerItemIndex = -1;
	_G.HawkerOwnerPPIndex = -1;
    UIManager:ClearMonoPanel();
end

function ConfirmHawkerOwnerNum()
	local numEdit = UIManager:FindUIEditBox("PriceOfHawkerItem");
    local numEdit1 = UIManager:FindUIEditBox("NumberOfHawkerItem");
    local hawkerProcess = LuaUI.GetHawkerProcess();
    if numEdit ~= nil and numEdit1 ~= nil and hawkerProcess ~= nil then
    	local canOffCount = false;
    	local numCount = numEdit1:GetInt();
    	local price = numEdit:GetInt();
    	if  _G.HawkerOwnerItemIndex >= 0 then
	        if numCount > 0 and   price > 0 and numCount <= LuaUI.GetMaxStack(_G.HawkerOwnerItemIndex + 24 * _G.HawkerBagState) 
				and  price <= 99999999 then
	        	
	        	
        		hawkerProcess:SetItemHawk( (_G.HawkerOwnerItemIndex + 24 * _G.HawkerBagState), numCount, price);
        		SelectHawkerOwnerSellItem();
        		canOffCount = true;	
    		end
    	elseif _G.HawkerOwnerPPIndex > 0 then
			if price > 0 and price <= 99999999 then
				ResetHawkerOwnerMenuButton();
				if _G.HawkerCPPState == 1 then
					hawkerProcess:SetPartnerHawk(price);
					SelectHawkerOwnerSellPartner();
				elseif _G.HawkerCPPState == 2 then
					hawkerProcess:SetPetHawk(price);
					SelectHawkerOwnerSellPet();
				end
				canOffCount = true;
			end
    	end
		
		if 	canOffCount then
			CloseHawkerOwnerCount();
		end
	end
end

function HawkerOwnerMaxStack()
	local numEdit = UIManager:FindUIEditBox("NumberOfHawkerItem");
	local temp = UIManager:GetUIItem("NumberOfHawkerItem");
	local max = LuaUI.GetMaxStack(_G.HawkerOwnerItemIndex + 24 * _G.HawkerBagState);
	
	UIManager:SetKeyInItem("NumberOfHawkerItem");
	numEdit:SetText(max.."");
	numEdit:End();
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function HawkerOwnerSetMin()
	local numEdit = UIManager:FindUIEditBox("NumberOfHawkerItem");
	local temp = UIManager:GetUIItem("NumberOfHawkerItem");
	UIManager:SetKeyInItem("NumberOfHawkerItem");
	numEdit:SetText("1");
	numEdit:End();
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function HawkerOwnerAddOne()
	local numEdit = UIManager:FindUIEditBox("NumberOfHawkerItem");
	local temp = UIManager:GetUIItem("NumberOfHawkerItem");
	
	local value = numEdit:GetInt();
	
	if value >= 0 and value < LuaUI.GetMaxStack(_G.HawkerOwnerItemIndex + 24 * _G.HawkerBagState) then
		value = value + 1;
	end
	
	UIManager:SetKeyInItem("NumberOfHawkerItem");
	numEdit:SetText(value.."");--value.."");
	numEdit:End();
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);--RenderAll();
end

function HawkerOwnerDiffOne()
	local numEdit = UIManager:FindUIEditBox("NumberOfHawkerItem");
	local temp = UIManager:GetUIItem("NumberOfHawkerItem");
	
	local value = numEdit:GetInt();
	
	if value > 1 then
		value = value - 1;
	end
	
	UIManager:SetKeyInItem("NumberOfHawkerItem");
	numEdit:SetText(value.."");--value.."");
	numEdit:End();
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function SelectHawkerOwnerSellItem()
	ResetAllHawkerHistory();
	local checkBox = UIManager:FindUICheckBox("HawkerOwnerSellItem");
	local temp = UIManager:GetUIItem("HawkerOwnerSellItem");
	local hawkerProcess = LuaUI.GetHawkerProcess();
	ResetAllHawkerOwnerCheckBox();
	if checkBox ~= nil and temp ~= nil and hawkerProcess ~= nil then
		checkBox:SetState(true);
		UIManager:AddDirtyUIItem(temp);
		hawkerProcess:SelectHawkItem();		
		
	end
end

function SelectHawkerOwnerSellPartner()
	ResetAllHawkerHistory();
	local checkBox = UIManager:FindUICheckBox("HawkerOwnerSellPartner");
	local temp = UIManager:GetUIItem("HawkerOwnerSellPartner");
	local hawkerProcess = LuaUI.GetHawkerProcess();
	ResetAllHawkerOwnerCheckBox();
	if checkBox ~= nil and temp ~= nil and hawkerProcess ~= nil then
		checkBox:SetState(true);
		UIManager:AddDirtyUIItem(temp);
		hawkerProcess:SelectHawkPartner();			   	
		
	end
end

function SelectHawkerOwnerSellPet()
	ResetAllHawkerHistory();
	local checkBox = UIManager:FindUICheckBox("HawkerOwnerSellPet");
	local temp = UIManager:GetUIItem("HawkerOwnerSellPet");
	local hawkerProcess = LuaUI.GetHawkerProcess();
	ResetAllHawkerOwnerCheckBox();
	if checkBox ~= nil and temp ~= nil and hawkerProcess ~= nil then
		checkBox:SetState(true);
		UIManager:AddDirtyUIItem(temp);
		hawkerProcess:SelectHawkPet();			   	
		
	end
end

function SelectHawkerOwnerSellLog()
	ResetAllHawkerButton();
	local checkBox = UIManager:FindUICheckBox("HawkerOwnerSellLog");
	local temp = UIManager:GetUIItem("HawkerOwnerSellLog");
	local hawkerProcess = LuaUI.GetHawkerProcess();
	ResetAllHawkerOwnerCheckBox();
	if checkBox ~= nil and temp ~= nil and hawkerProcess ~= nil then
		checkBox:SetState(true);
		UIManager:AddDirtyUIItem(temp);
		hawkerProcess:SelectHawkExchangeRecord();			   	
		
	end
end

function ResetAllHawkerHistory()
	for i = 0, 5 do 
		local name = "HawkerOwnerRecordDate00"..i;
		local text = UIManager:GetUIItem(name);
		if text ~= nil then 
			text:SetEnable(false);
		end
		
		name = "HawkerOwnerRecord00"..i;
		text = UIManager:GetUIItem(name);
		if text ~= nil then 
			text:SetEnable(false);
		end
	end
	
	for i = 0, 11 do 
		local name;
		if i < 10 then
			name = "HawkerOwnerSellSlot0"..i;
		else
			name = "HawkerOwnerSellSlot"..i
		end
		
		local button = UIManager:GetUIItem(name);
		if button ~= nil then
			button:SetEnable(true);
		end
	end
end

function ResetAllHawkerButton()
	for i = 0, 11 do 
		local name;
		if i < 10 then
			name = "HawkerOwnerSellSlot0"..i;
		else
			name = "HawkerOwnerSellSlot"..i
		end
		
		local uiItem = UIManager:GetUIItem(name);
		if uiItem ~= nil then
			uiItem:SetEnable(false);
		end 
		
		if i < 10 then
			name = "HawkerOwnerSell00"..i.."Bg";
		else
			name = "HawkerOwnerSell0"..i.."Bg";
		end
		
		uiItem = UIManager:GetUIItem(name);
		if uiItem ~= nil then
			uiItem:SetEnable(false);
		end 
		
		if i < 10 then
			name = "HawkerOwnerSell00"..i.."Icon";
		else
			name = "HawkerOwnerSell0"..i.."Icon";
		end
		
		uiItem = UIManager:GetUIItem(name);
		if uiItem ~= nil then
			uiItem:SetEnable(false);
		end 
		
		if i < 10 then
			name = "HawkerOwnerSell00"..i.."Name";
		else
			name = "HawkerOwnerSell0"..i.."Name";
		end
		
		uiItem = UIManager:GetUIItem(name);
		if uiItem ~= nil then
			uiItem:SetEnable(false);
		end 
		
		if i < 10 then
			name = "HawkerOwnerSell00"..i.."PriceTitle";
		else
			name = "HawkerOwnerSell0"..i.."PriceTitle";
		end
		
		uiItem = UIManager:GetUIItem(name);
		if uiItem ~= nil then
			uiItem:SetEnable(false);
		end 
		
		if i < 10 then
			name = "HawkerOwnerSell00"..i.."Price";
		else
			name = "HawkerOwnerSell0"..i.."Price";
		end
		
		uiItem = UIManager:GetUIItem(name);
		if uiItem ~= nil then
			uiItem:SetEnable(false);
		end 
		
		if i < 10 then
			name = "HawkerOwnerSell00"..i.."Stack";
		else
			name = "HawkerOwnerSell0"..i.."Stack";
		end
		
		uiItem = UIManager:GetUIItem(name);
		if uiItem ~= nil then
			uiItem:SetEnable(false);
		end 
	end	
end

function ResetAllHawkerOwnerCheckBox()
	local checkBox = UIManager:FindUICheckBox("HawkerOwnerSellItem");
	local temp = UIManager:GetUIItem("HawkerOwnerSellItem");
	
	if checkBox ~= nil and temp ~= nil then
		checkBox:SetState(false);
		UIManager:AddDirtyUIItem(temp);
	end
	
	checkBox = UIManager:FindUICheckBox("HawkerOwnerSellPartner");
	temp = UIManager:GetUIItem("HawkerOwnerSellPartner");
	
	if checkBox ~= nil and temp ~= nil then
		checkBox:SetState(false);
		UIManager:AddDirtyUIItem(temp);
	end
	
	checkBox = UIManager:FindUICheckBox("HawkerOwnerSellPet");
	temp = UIManager:GetUIItem("HawkerOwnerSellPet");
	
	if checkBox ~= nil and temp ~= nil then
		checkBox:SetState(false);
		UIManager:AddDirtyUIItem(temp);
	end
	
	checkBox = UIManager:FindUICheckBox("HawkerOwnerSellLog");
	temp = UIManager:GetUIItem("HawkerOwnerSellLog");
	
	if checkBox ~= nil and temp ~= nil then
		checkBox:SetState(false);
		UIManager:AddDirtyUIItem(temp);
	end
end

function SelectHawkerOwnerPrevPage()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		hawkerProcess:TurnPage(false);
	end
end

function SelectHawkerOwnerNextPage()
    local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		hawkerProcess:TurnPage(true);
	end
end

function HawkerOwnerHawkPP()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
	
		--if hawkerProcess:IsStartHawk() then
		--	hawkerProcess:ShowHawkMessage(0);
		--	do return end
		--end
		_G.HawkerOwnerPPIndex = 1;
		local canShowCount = false;
		if _G.HawkerCPPState == 1 and hawkerProcess:NotForSellPartner(true) ~= true then
			canShowCount = true;
		elseif _G.HawkerCPPState == 2 and hawkerProcess:NotForSellPet(true) ~= true then
			canShowCount = true;
		end 
		
		if canShowCount then
			CloseItemDetail(0, 0);
			local countPanel = UIManager:GetUIItem("HawkerOwnerItemCountPanel");
			local tempPanel = UIManager:FindUIPanel("HawkerOwnerItemCountPanel");
			if countPanel ~= nil and tempPanel ~= nil then
				UIManager:SetPanel2Top(tempPanel);
				countPanel:SetEnable(true);
				UIManager:SetKeyInItem("PriceOfHawkerItem");
				UIManager:ClearMonoPanel();
				UIManager:SetMonoPanel(tempPanel);	
				
				local temp = UIManager:GetUIItem("NumberOfHawkerItemBg");
				local temp1 = UIManager:GetUIItem("HawkerItemCountText");
				local temp2 = UIManager:GetUIItem("NumberOfHawkerItem");
				local temp3 = UIManager:GetUIItem("HawkerOwnerMaxStack");
				local temp4 = UIManager:GetUIItem("HawkerOwnerSetMin");
				local temp5 = UIManager:GetUIItem("AddOneHawkerOwnerStack");
				local temp6 = UIManager:GetUIItem("DiffOneHawkerOwnerStack");
				
				if temp ~= nil and temp1 ~= nil and temp2 ~= nil and temp3 ~= nil and 
					temp4 ~= nil and temp5 ~= nil and temp6 ~= nil then
					temp:SetEnable(false);	
					temp1:SetEnable(false);
					temp2:SetEnable(false);
					temp3:SetEnable(false);
					temp4:SetEnable(false);
					temp5:SetEnable(false);
					temp6:SetEnable(false);
				end
				
				UIManager:AddDirtyUIItem(countPanel);
		    end
		end
	end
end


function SelectHawkerOwnerPPName(index)
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		hawkerProcess:SelectPartnerOrPet(index);
	end
end

function SelectHawkerOwnerPPName1()
	SelectHawkerOwnerPPName(0);
end

function SelectHawkerOwnerPPName2()
	SelectHawkerOwnerPPName(1);
end

function SelectHawkerOwnerPPName3()
	SelectHawkerOwnerPPName(2);
end

function SelectHawkerOwnerPPName4()
	SelectHawkerOwnerPPName(3);
end

function SelectHawkerOwnerPPName5()
	SelectHawkerOwnerPPName(4);
end

function SelectHawkerOwnerPPName6()
	SelectHawkerOwnerPPName(5);
end

function SelectedHawkerOwnerPPDrag()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	local movingPanel = UIManager:GetUIItem("HawkerOwnerMovingPanel");
	if hawkerProcess ~= nil and movingPanel ~= nil then
		
		--if hawkerProcess:IsStartHawk() then
		--	hawkerProcess:ShowHawkMessage(0);
		--	do return end
		--end
		if _G.HawkerCPPState == 1  and hawkerProcess:NotForSellPartner(true) ~= true then
			movingPanel:SetEnable(true);
			UIManager:SetDragItemXY(movingPanel,-20,-20);
			hawkerProcess:DragPartner();
			_G.DragType = 1;
		elseif _G.HawkerCPPState == 2  and hawkerProcess:NotForSellPet(true) ~= true then
			movingPanel:SetEnable(true);
			UIManager:SetDragItemXY(movingPanel,-20,-20);
			hawkerProcess:DragPet();
			_G.DragType = 2;
		end
	end		
end

function TakeOffHawkerSellItem(index)
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		if hawkerProcess:IsStartHawk() then
			hawkerProcess:ShowHawkMessage(0);
		else
			hawkerProcess:ResetSellItem(index);
		end
	end
end

function TakeOffHawkerSellItem000()
	TakeOffHawkerSellItem(0);
end

function TakeOffHawkerSellItem001()
	TakeOffHawkerSellItem(1);
end

function TakeOffHawkerSellItem002()
	TakeOffHawkerSellItem(2);
end

function TakeOffHawkerSellItem003()
	TakeOffHawkerSellItem(3);
end

function TakeOffHawkerSellItem004()
	TakeOffHawkerSellItem(4);
end

function TakeOffHawkerSellItem005()
	TakeOffHawkerSellItem(5);
end

function TakeOffHawkerSellItem006()
	TakeOffHawkerSellItem(6);
end

function TakeOffHawkerSellItem007()
	TakeOffHawkerSellItem(7);
end

function TakeOffHawkerSellItem008()
	TakeOffHawkerSellItem(8);
end

function TakeOffHawkerSellItem009()
	TakeOffHawkerSellItem(9);
end

function TakeOffHawkerSellItem010()
	TakeOffHawkerSellItem(10);
end

function TakeOffHawkerSellItem011()
	TakeOffHawkerSellItem(11);
end

function DragHawkerSellItem(index)
	local hawkerProcess = LuaUI.GetHawkerProcess();
	--if hawkerProcess ~= nil and hawkerProcess:IsStartHawk() then
	--	hawkerProcess:ShowHawkMessage(0);
	--	do return end
	--end
	_G.DragHawkerSellItemIndex = index;  
	local movingPanel = UIManager:GetUIItem("HawkerOwnerMovingPanel2");

	local iconName = "";
	
	if index >= 10 then
		iconName = "HawkerOwnerSell0"..index.."Icon" ;
	else
	    iconName = "HawkerOwnerSell00"..index.."Icon" ;
	end
	
	local orgIcon = UIManager:GetUIItem(iconName);
	local orgAniIcon = UIManager:FindUIAniIcon(iconName);
	local targetAniIcon = UIManager:FindUIAniIcon("HawkerOwnerMovingIcon2");
				
	if movingPanel ~= nil and orgIcon ~= nil and orgAniIcon ~= nil and targetAniIcon ~= nil then

	    targetAniIcon:ClearAniData();
	    targetAniIcon:SetAnimationData(orgAniIcon);	
	
		UIManager:SetDragItemXY(movingPanel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
		orgIcon:SetRenderStyle(1);
		movingPanel:SetEnable(true);
		UIManager:AddDirtyUIItem(movingPanel);
		PlayDragItemUpEffect();
	end
end

function DragHawkerSellItem000()
	DragHawkerSellItem(0);
end

function DragHawkerSellItem001()
	DragHawkerSellItem(1);
end

function DragHawkerSellItem002()
	DragHawkerSellItem(2);
end

function DragHawkerSellItem003()
	DragHawkerSellItem(3);
end

function DragHawkerSellItem004()
	DragHawkerSellItem(4);
end

function DragHawkerSellItem005()
	DragHawkerSellItem(5);
end

function DragHawkerSellItem006()
	DragHawkerSellItem(6);
end

function DragHawkerSellItem007()
	DragHawkerSellItem(7);
end

function DragHawkerSellItem008()
	DragHawkerSellItem(8);
end

function DragHawkerSellItem009()
	DragHawkerSellItem(9);
end

function DragHawkerSellItem010()
	DragHawkerSellItem(10);
end

function DragHawkerSellItem011()
	DragHawkerSellItem(11);
end

function HawkerOwnerDragEnd2(value)
	if value == 3 then
	    ResetHawkerOwnerDrag();
	else
		local panel = UIManager:GetUIItem("HawkerOwnerPanel");
		local bag1 = UIManager:GetUIItem("HawkerOwnerCurrentBag");
		if panel ~= nil and bag1 ~= nil then
			local mx = UIManager:GetCursorX() - panel:GetX();
			local my = UIManager:GetCursorY() - panel:GetY();
			
			local temp = _G.DragHawkerSellItemIndex	;
			ResetHawkerOwnerDrag();
			
			if mx > bag1:GetOffsetX() and my > 0 and 
			   mx < panel:GetWidth() and my < panel:GetHeight() then
			   	
				TakeOffHawkerSellItem(temp);			   	
			end
		end
	end
end

function HawkerOwnerStartHawk()
    local hawkerProcess = LuaUI.GetHawkerProcess();
    local button = UIManager:FindUIButton("HawkerOwnerHawk");
    local temp = UIManager:GetUIItem("HawkerOwnerHawk");
	if hawkerProcess ~= nil and button ~= nil and temp ~= nil then
		hawkerProcess:StartHawk();
		button:SetDisableState();
        UIManager:AddDirtyUIItem(temp);
	end
end

function HawkerOwnerStopHawk()
    local hawkerProcess = LuaUI.GetHawkerProcess();
    local button = UIManager:FindUIButton("HawkerOwnerStopHawk");
    local temp = UIManager:GetUIItem("HawkerOwnerStopHawk");
    
    local button1 = UIManager:FindUIButton("HawkerOwnerChangeName");
    local tempButton1 = UIManager:GetUIItem("HawkerOwnerChangeName");
    
	if hawkerProcess ~= nil and button ~= nil and temp ~= nil and button1 ~= nil and tempButton1 ~= nil then
		hawkerProcess:StopHawk();
		button:SetDisableState();
		UIManager:AddDirtyUIItem(temp);
		button1:SetDisableState();
		UIManager:AddDirtyUIItem(tempButton1);
		hawkerProcess:ShowHawkMessage(3);
	end
end

function HawkClick()
    local hawkerProcess = LuaUI.GetHawkerProcess();
    if hawkerProcess ~= nil then
        hawkerProcess:ClickHawkAlertPanel();
    end
end

function OpenHawk()
    local panel = UIManager:GetUIItem("HawkPanel");

	if panel ~= nil then
	    CloseHawk();
	else
		LuaUI.AddXMLFile2UIManger("Data\\MenuHawk.xml");
		SelectHawkSellItem();
		panel:SetEnableTimeOutEvent(false);
	end

end

function CloseHawk()
    UIManager:StopDrag();
    local hawkerProcess = LuaUI.GetHawkerProcess();
    if hawkerProcess ~= nil then
        hawkerProcess:CloseHawkFromTarget();
    end
    CloseHawkCount();
    CloseConfirmHawkBuyPanel();
	UIManager:DeletePanel("HawkPanel");
	UIManager:DeletePanel("HawkItemCountPanel");
	UIManager:DeletePanel("ConfirmBuyHawkItemPanel");
    _G.HawkClickedButtonIndex = -1;
end

function ResetHawkMenuButton()
    local checkBox = UIManager:FindUICheckBox("HawkSellItem");
	local temp = UIManager:GetUIItem("HawkSellItem");

	if checkBox ~= nil and temp ~= nil then
		checkBox:SetState(false);
		UIManager:AddDirtyUIItem(temp);
	end

	checkBox = UIManager:FindUICheckBox("HawkSellPartner");
	temp = UIManager:GetUIItem("HawkSellPartner");

	if checkBox ~= nil and temp ~= nil then
		checkBox:SetState(false);
		UIManager:AddDirtyUIItem(temp);
	end

	checkBox = UIManager:FindUICheckBox("HawkSellPet");
	temp = UIManager:GetUIItem("HawkSellPet");

	if checkBox ~= nil and temp ~= nil then
		checkBox:SetState(false);
		UIManager:AddDirtyUIItem(temp);
	end
end

function ResetHawkButton()

	local uiItem, uiItem1, uiItem2, uiItem3, uiItem4, uiItem5, uiItem6,
			uiItem7, uiItem8, uiItem9, uiItem10, uiItem11, uiItem12, uiItem13;
	local name, name1, name2, name3, name4, name5, name6,
			name7, name8, name9, name10, name11, name12, name13;
	for i = 0, 23 do
		if i < 10 then
		    name = "HawkSellSlot00"..i;
		    name1 = "HawkSell00"..i.."Icon"
		    name2 = "HawkSell00"..i.."Name"
		    name3 = "HawkSell00"..i.."Price"
		    name4 = "HawkSell00"..i.."Stack"
            name5 = "HawkSellPPSlot00"..i;
		    name6 = "HawkSellPP00"..i.."Icon"
		    name7 = "HawkSellPP00"..i.."Name"
		    name8 = "HawkSellPP00"..i.."Price"
		    name9 = "HawkSell00"..i.."PriceTitle"
		    name10 = "HawkSellPP00"..i.."PriceTitle"
		    name11 = "HawkSellPP00"..i.."Bg"
		    name12 = "HawkSellSlot00"..i.."ButtonBg"
		    name13 = "HawkSellPPSlot00"..i.."ButtonBg"
		else
		    name = "HawkSellSlot0"..i;
		    name1 = "HawkSell0"..i.."Icon"
		    name2 = "HawkSell0"..i.."Name"
		    name3 = "HawkSell0"..i.."Price"
		    name4 = "HawkSell0"..i.."Stack"
		    name5 = "HawkSellPPSlot0"..i;
		    name6 = "HawkSellPP0"..i.."Icon"
		    name7 = "HawkSellPP0"..i.."Name"
		    name8 = "HawkSellPP0"..i.."Price"
		    name9 = "HawkSell0"..i.."PriceTitle"
		    name10 = "HawkSellPP0"..i.."PriceTitle"
		    name11 = "HawkSellPP0"..i.."Bg"
		    name12 = "HawkSellSlot0"..i.."ButtonBg"
		    name13 = "HawkSellPPSlot0"..i.."ButtonBg"
		end
		uiItem = UIManager:GetUIItem(name);
		uiItem1 = UIManager:GetUIItem(name1);
		uiItem2 = UIManager:GetUIItem(name2);
		uiItem3 = UIManager:GetUIItem(name3);
		uiItem4 = UIManager:GetUIItem(name4);
		uiItem5 = UIManager:GetUIItem(name5);
		uiItem6 = UIManager:GetUIItem(name6);
		uiItem7 = UIManager:GetUIItem(name7);
		uiItem8 = UIManager:GetUIItem(name8);
		uiItem9 = UIManager:GetUIItem(name9);
		uiItem10 = UIManager:GetUIItem(name10);
		uiItem11 = UIManager:GetUIItem(name11);
		uiItem12 = UIManager:GetUIItem(name12);
		uiItem13 = UIManager:GetUIItem(name13);
		
		if uiItem ~= nil and uiItem1 ~= nil and uiItem2 ~= nil and uiItem3 ~= nil
			and uiItem4 ~= nil and uiItem9 ~= nil and uiItem12 ~= nil then
		    uiItem:SetEnable(false);
		    uiItem1:SetEnable(false);
		    uiItem2:SetEnable(false);
		    uiItem3:SetEnable(false);
		    uiItem4:SetEnable(false);
		    uiItem9:SetEnable(false);
		    uiItem12:SetEnable(false);

			if i < 12 and uiItem5 ~= nil and uiItem6 ~= nil and uiItem7 ~= nil
				and uiItem8 ~= nil and uiItem10 ~= nil and uiItem11 ~= nil and uiItem13 ~= nil then
		        uiItem5:SetEnable(false);
			    uiItem6:SetEnable(false);
			    uiItem7:SetEnable(false);
			    uiItem8:SetEnable(false);
			    uiItem10:SetEnable(false);
			    uiItem11:SetEnable(false);
			    uiItem13:SetEnable(false);
		    end
		end
	end
end


function SelectHawkSellItem()
    ResetHawkSellSlot();
    ResetHawkMenuButton();
    local checkBox = UIManager:FindUICheckBox("HawkSellItem");
	local temp = UIManager:GetUIItem("HawkSellItem");
	local hawkerProcess = LuaUI.GetHawkerProcess();

	if checkBox ~= nil and temp ~= nil and hawkerProcess ~= nil then
	    _G.HawkerSellMenuIndex = 0;
		checkBox:SetState(true);
		UIManager:AddDirtyUIItem(temp);
		ResetHawkButton();
		local name, uiItem;
		for i = 0, 23 do
			if i < 10 then
			    name = "HawkSellSlot00"..i.."ButtonBg";
			else
			    name = "HawkSellSlot0"..i.."ButtonBg";
			end
			uiItem = UIManager:GetUIItem(name);
			if uiItem ~= nil then
			    uiItem:SetEnable(true);
			end
		end
  		hawkerProcess:SelectHawkSellItem();
	end
end

function SelectHawkSellPartner()
    ResetHawkSellSlot();
    ResetHawkMenuButton();
    local checkBox = UIManager:FindUICheckBox("HawkSellPartner");
	local temp = UIManager:GetUIItem("HawkSellPartner");
	local hawkerProcess = LuaUI.GetHawkerProcess();

	if checkBox ~= nil and temp ~= nil and hawkerProcess ~= nil then
	    _G.HawkerSellMenuIndex = 1;
		checkBox:SetState(true);
		UIManager:AddDirtyUIItem(temp);
		ResetHawkButton();
		local name, uiItem;
		for i = 0, 11 do
			if i < 10 then
			    name = "HawkSellPPSlot00"..i.."ButtonBg";
			else
			    name = "HawkSellPPSlot0"..i.."ButtonBg";
			end
			uiItem = UIManager:GetUIItem(name);
			if uiItem ~= nil then
			    uiItem:SetEnable(true);
			end
		end
  		hawkerProcess:SelectHawkSellPartner();
	end
end

function SelectHawkSellPet()
    ResetHawkSellSlot();
    ResetHawkMenuButton();
    local checkBox = UIManager:FindUICheckBox("HawkSellPet");
	local temp = UIManager:GetUIItem("HawkSellPet");
	local hawkerProcess = LuaUI.GetHawkerProcess();

	if checkBox ~= nil and temp ~= nil and hawkerProcess ~= nil then
	    _G.HawkerSellMenuIndex = 2;
		checkBox:SetState(true);
		UIManager:AddDirtyUIItem(temp);
		ResetHawkButton();
		local name, uiItem;
		for i = 0, 11 do
			if i < 10 then
			    name = "HawkSellPPSlot00"..i.."ButtonBg";
			else
			    name = "HawkSellPPSlot0"..i.."ButtonBg";
			end
			uiItem = UIManager:GetUIItem(name);
			if uiItem ~= nil then
			    uiItem:SetEnable(true);
			end
		end
  		hawkerProcess:SelectHawkSellPet();
	end
end

function ChangeHawkName()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		hawkerProcess:ChangeHawkName();
	end
end

function ResetHawkSellSlot()
    if _G.HawkClickedButtonIndex >= 0 then
        local text = "";
        if _G.HawkerSellMenuIndex == 0 then
			if _G.HawkClickedButtonIndex < 10 then
			    text = "HawkSellSlot00".._G.HawkClickedButtonIndex;
			else
			    text = "HawkSellSlot0".._G.HawkClickedButtonIndex;
			end
        elseif _G.HawkerSellMenuIndex > 0 then
            if _G.HawkClickedButtonIndex < 10 then
			    text = "HawkSellPPSlot00".._G.HawkClickedButtonIndex;
			else
			    text = "HawkSellPPSlot0".._G.HawkClickedButtonIndex;
			end
        end
        
        local button = UIManager:FindUIButton(text);
        local tempButton = UIManager:GetUIItem(text);
        if button ~= nil and tempButton ~= nil then
            button:SetNormalState();
            UIManager:AddDirtyUIItem(tempButton);
        end
    
    end
    _G.HawkClickedButtonIndex = -1;
end

function SelectHawkSellSlot(index)
    ResetHawkSellSlot();
    _G.HawkClickedButtonIndex = index;
    local text = "";

	if _G.HawkClickedButtonIndex < 10 then
	    text = "HawkSellSlot00".._G.HawkClickedButtonIndex;
	else
	    text = "HawkSellSlot0".._G.HawkClickedButtonIndex;
	end
	
    local button = UIManager:FindUIButton(text);
    local tempButton = UIManager:GetUIItem(text);
    if button ~= nil and tempButton ~= nil then
        button:SetDisableState();
        UIManager:AddDirtyUIItem(tempButton);
    end
end

function SelectHawkSellSlot000()
    SelectHawkSellSlot(0);
end

function SelectHawkSellSlot001()
    SelectHawkSellSlot(1);
end

function SelectHawkSellSlot002()
    SelectHawkSellSlot(2);
end

function SelectHawkSellSlot003()
    SelectHawkSellSlot(3);
end

function SelectHawkSellSlot004()
    SelectHawkSellSlot(4);
end

function SelectHawkSellSlot005()
    SelectHawkSellSlot(5);
end

function SelectHawkSellSlot006()
    SelectHawkSellSlot(6);
end

function SelectHawkSellSlot007()
    SelectHawkSellSlot(7);
end

function SelectHawkSellSlot008()
    SelectHawkSellSlot(8);
end

function SelectHawkSellSlot009()
    SelectHawkSellSlot(9);
end

function SelectHawkSellSlot010()
    SelectHawkSellSlot(10);
end

function SelectHawkSellSlot011()
    SelectHawkSellSlot(11);
end

function SelectHawkSellSlot012()
    SelectHawkSellSlot(12);
end

function SelectHawkSellSlot013()
    SelectHawkSellSlot(13);
end

function SelectHawkSellSlot014()
    SelectHawkSellSlot(14);
end

function SelectHawkSellSlot015()
    SelectHawkSellSlot(15);
end

function SelectHawkSellSlot016()
    SelectHawkSellSlot(16);
end

function SelectHawkSellSlot017()
    SelectHawkSellSlot(17);
end

function SelectHawkSellSlot018()
    SelectHawkSellSlot(18);
end

function SelectHawkSellSlot019()
    SelectHawkSellSlot(19);
end

function SelectHawkSellSlot020()
    SelectHawkSellSlot(20);
end

function SelectHawkSellSlot021()
    SelectHawkSellSlot(21);
end

function SelectHawkSellSlot022()
    SelectHawkSellSlot(22);
end

function SelectHawkSellSlot023()
    SelectHawkSellSlot(23);
end

function SelectHawkSellPPSlot(index)
    ResetHawkSellSlot();
    _G.HawkClickedButtonIndex = index;
    local text = "";
    if _G.HawkClickedButtonIndex < 10 then
	    text = "HawkSellPPSlot00".._G.HawkClickedButtonIndex;
	else
	    text = "HawkSellPPSlot0".._G.HawkClickedButtonIndex;
	end

    local button = UIManager:FindUIButton(text);
    local tempButton = UIManager:GetUIItem(text);
    if button ~= nil and tempButton ~= nil then
        button:SetDisableState();
        UIManager:AddDirtyUIItem(tempButton);
    end
end

function  SelectHawkSellPPSlot000()
    SelectHawkSellPPSlot(0);
end

function  SelectHawkSellPPSlot001()
    SelectHawkSellPPSlot(1);
end

function  SelectHawkSellPPSlot002()
    SelectHawkSellPPSlot(2);
end

function  SelectHawkSellPPSlot003()
    SelectHawkSellPPSlot(3);
end

function  SelectHawkSellPPSlot004()
    SelectHawkSellPPSlot(4);
end

function  SelectHawkSellPPSlot005()
    SelectHawkSellPPSlot(5);
end

function  SelectHawkSellPPSlot006()
    SelectHawkSellPPSlot(6);
end

function  SelectHawkSellPPSlot007()
    SelectHawkSellPPSlot(7);
end

function  SelectHawkSellPPSlot008()
    SelectHawkSellPPSlot(8);
end

function  SelectHawkSellPPSlot009()
    SelectHawkSellPPSlot(9);
end

function  SelectHawkSellPPSlot010()
    SelectHawkSellPPSlot(10);
end

function  SelectHawkSellPPSlot011()
    SelectHawkSellPPSlot(11);
end

function HawkPreBuy()
	if _G.HawkClickedButtonIndex >= 0 then
		if _G.HawkerSellMenuIndex == 0 then
	  		local countPanel = UIManager:GetUIItem("HawkItemCountPanel");
			local tempPanel = UIManager:FindUIPanel("HawkItemCountPanel");
			if countPanel ~= nil and tempPanel ~= nil then
				UIManager:SetPanel2Top(tempPanel);
				countPanel:SetEnable(true);
				UIManager:SetKeyInItem("NumberOfHawkItem");
				UIManager:ClearMonoPanel();
				UIManager:SetMonoPanel(tempPanel);
				UIManager:AddDirtyUIItem(countPanel);
				local hawkerProcess = LuaUI.GetHawkerProcess();
				if hawkerProcess ~= nil then
					hawkerProcess:ShowItemPriceForHawk(_G.HawkClickedButtonIndex, 1);
				end
			end
		elseif _G.HawkerSellMenuIndex == 1 or _G.HawkerSellMenuIndex == 2 then
		    ShowConfirmHawkBuy();
		end
	end
end

function ShowConfirmHawkBuy()
    local numEdit = UIManager:FindUIEditBox("NumberOfHawkItem");
    if numEdit ~= nil then
    	_G.HawkItemStack = numEdit:GetInt();
	end
	if _G.HawkItemStack > 0 then
	
	    CloseHawkCount();
	    UIManager:ClearMonoPanel();
	    local confirmPanel = UIManager:GetUIItem("ConfirmBuyHawkItemPanel");
	    local tempPanel = UIManager:FindUIPanel("ConfirmBuyHawkItemPanel");
	    if tempPanel ~= nil and confirmPanel ~= nil then
	        UIManager:SetMonoPanel(tempPanel);
	        UIManager:SetPanel2Top(tempPanel);
	        confirmPanel:SetEnable(true);
	        UIManager:AddDirtyUIItem(confirmPanel);
	    end
	end
end

function ConfirmHawkBuy()
    local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
	    hawkerProcess:BuyHawkItem(_G.HawkClickedButtonIndex, _G.HawkerSellMenuIndex, _G.HawkItemStack);
	end
    CloseConfirmHawkBuyPanel();
end

function CloseConfirmHawkBuyPanel()
    local confirmPanel = UIManager:GetUIItem("ConfirmBuyHawkItemPanel");
    if confirmPanel ~= nil then
        confirmPanel:SetEnable(false);
    end
    UIManager:ClearMonoPanel();
end

function CloseHawkCount()
    ResetKeyIn();
    local countPanel = UIManager:GetUIItem("HawkItemCountPanel");

    local numEdit = UIManager:FindUIEditBox("NumberOfHawkItem");
    if countPanel ~= nil and numEdit ~= nil then
    	countPanel:SetEnable(false);
    	numEdit:SetText("1");
    	numEdit:End();
		numEdit:Redraw();

    end

    UIManager:ClearMonoPanel();
end

function HawkMaxStack()
    local numEdit = UIManager:FindUIEditBox("NumberOfHawkItem");
	local temp = UIManager:GetUIItem("NumberOfHawkItem");

	local max = 0;
 	local hawkerProcess = LuaUI.GetHawkerProcess();

	if hawkerProcess ~= nil then
	    max = hawkerProcess:GetMaxItemStackForHawk(_G.HawkClickedButtonIndex);
    	numEdit:SetText(max.."");
    	numEdit:End();
		numEdit:Redraw();
		UIManager:AddDirtyUIItem(temp);
		hawkerProcess:ShowItemPriceForHawk(_G.HawkClickedButtonIndex, max);
	
	end
end

function HawkSetMin()
    local numEdit = UIManager:FindUIEditBox("NumberOfHawkItem");
	local temp = UIManager:GetUIItem("NumberOfHawkItem");
	numEdit:SetText("1");
	numEdit:End();
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
	
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		hawkerProcess:ShowItemPriceForHawk(_G.HawkClickedButtonIndex, 1);
	end
end

function HawkAddOne()
    local numEdit = UIManager:FindUIEditBox("NumberOfHawkItem");
	local temp = UIManager:GetUIItem("NumberOfHawkItem");

	local value = numEdit:GetInt();
	
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then

		if value >= 0 and value < hawkerProcess:GetMaxItemStackForHawk(_G.HawkClickedButtonIndex) then
			value = value + 1;
  		else
  		    value = hawkerProcess:GetMaxItemStackForHawk(_G.HawkClickedButtonIndex);
		end


		numEdit:SetText(value.."");--value.."");
		numEdit:End();
		numEdit:Redraw();
		UIManager:AddDirtyUIItem(temp);--RenderAll();
    	hawkerProcess:ShowItemPriceForHawk(_G.HawkClickedButtonIndex, value);
	end
end

function HawkDiffOne()
    local numEdit = UIManager:FindUIEditBox("NumberOfHawkItem");
	local temp = UIManager:GetUIItem("NumberOfHawkItem");

	local value = numEdit:GetInt();

	if value > 1 then
		value = value - 1;
	end

	numEdit:SetText(value.."");--value.."");
	numEdit:End();
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
		hawkerProcess:ShowItemPriceForHawk(_G.HawkClickedButtonIndex, value);
	end
end

function NumberOfHawkerItemKeyUp()
	local numCount = UIManager:FindUIEditBox("NumberOfHawkerItem");
	local temp = UIManager:GetUIItem("NumberOfHawkerItem");
	if numCount ~= nil and numGount:GetInt() > LuaUI.GetMaxStack(_G.HawkerOwnerItemIndex + 24 * _G.HawkerBagState) then
		local value = LuaUI.GetMaxStack(_G.HawkerOwnerItemIndex + 24 * _G.HawkerBagState);
		numCount:SetText(value.."");--value.."");
		numCount:End();
		numCount:Redraw();
		UIManager:AddDirtyUIItem(temp);
	end
end

function NumberOfHawkItemKeyUp()
	local numEdit = UIManager:FindUIEditBox("NumberOfHawkItem");
	local temp = UIManager:GetUIItem("NumberOfHawkItem");
	
 	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil and numEdit ~= nil and temp ~= nil and numEdit:GetInt() > hawkerProcess:GetMaxItemStackForHawk(_G.HawkClickedButtonIndex) then
	    local value = hawkerProcess:GetMaxItemStackForHawk(_G.HawkClickedButtonIndex);
	    numEdit:SetText(value.."");
	    numEdit:End();
		numEdit:Redraw();
		UIManager:AddDirtyUIItem(temp);
		hawkerProcess:ShowItemPriceForHawk(_G.HawkClickedButtonIndex, value);
	end
end

function CloseHawkSellItemDetail()
	local panel = UIManager:GetUIItem("ItemDetailMenuPanel");
	local panel2 = UIManager:GetUIItem("PartnerDetailPanel");
	local panel3 = UIManager:GetUIItem("PetDetailPanel");
	if panel ~= nil then 
		UIManager:DeletePanel("ItemDetailMenuPanel");
	elseif panel2 ~= nil then
		UIManager:DeletePanel("PartnerDetailPanel");
	elseif panel3 ~= nil then
		UIManager:DeletePanel("PetDetailPanel");
	end
end

function HawkSellItemMouseIn()
	local pPanel = UIManager:GetUIItem("HawkPanel");
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(true);
		pPanel:ResetTimer();
	end
end

function HawkSellItemMouseOut()
	local pPanel = UIManager:GetUIItem("HawkPanel");
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(false);
	end
	CloseHawkSellItemDetail();
end

function HawkDetailPopupTimeOut()
	local panel = UIManager:FindUIPanel("HawkPanel");
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil and panel ~= nil then
	
		local cursorX = UIManager:GetCursorX();
	    local cursorY = UIManager:GetCursorY();
	    CloseHawkSellItemDetail();
	    if cursorX > panel:GetX() and cursorY > panel:GetY() and cursorX < (panel:GetX() + panel:GetWidth()) and cursorY < (panel:GetY() + panel:GetHeight()) then
			hawkerProcess:ShowHawkSelltemDetail();
		end		
		panel:SetEnableTimeOutEvent(false);
	end
end

function SetHawkNameKeyIn()
	UIManager:SetKeyInItem("HawkerOwnerNameBox");
end

function SetPutItemMoneyColour()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
    	hawkerProcess:SetItemMoneyColour();
	end
end

function CheckConcernTarget()
	local hawkerProcess = LuaUI.GetHawkerProcess();
	if hawkerProcess ~= nil then
    	hawkerProcess:SetConcernTarget();
	end
end

function OnJpackItem()
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
		itemProcess:JpackBag(BagState);
	end
end