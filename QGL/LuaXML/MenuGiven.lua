GivenItemOrgIndex = -1; --itemMenu index
PlacedItemIndex = -1;
DragItemIndex = -1; --given menu item index
DragPartnerIndex = -1;
DragPetIndex = -1;

function OpenGivenPanel()
	local panel = UIManager:GetUIItem("GivenItemPanel");
	local itemPanel = UIManager:GetUIItem("ItemMenuPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuGive.xml");
		local charName = UIManager:FindUIStaticText("AcceptPersonName");
		local charID = UIManager:FindUIStaticText("AcceptPersonID");
		local givenMenu = LuaUI.GetGivenProcess();
		panel = UIManager:GetUIItem("GivenItemPanel");
		if charID ~= nil and charName ~= nil and givenMenu ~= nil and panel ~= nil then
			panel:SetPos(20, 40);
			givenMenu:SetTargetCharIDNameToPanel();
			UIManager:AddDirtyUIItem(panel);
		end
		
		if itemPanel == nil then
			BBItem();
			itemPanel = UIManager:GetUIItem("ItemMenuPanel");
		end
		

		if itemPanel ~= nil then
			itemPanel:SetPos(400, 40);
			UIManager:AddDirtyUIItem(itemPanel);
		end 

		local tempGivenPanel = UIManager:FindUIPanel("GivenItemPanel");
		local tempItemPanel = UIManager:FindUIPanel("ItemMenuPanel");
		
		if _G.LockState or _G.UnlockState then
		    ResetAllLockStatus(); -- cancel asset lock or unlock status
		end
		
		UIManager:FreezeAll();
		if tempGivenPanel ~= nil and tempItemPanel ~= nil then

            tempGivenPanel:Unfreeze();
            tempItemPanel:Unfreeze();
		end
	else
		CloseGivenPanel();
	end
end

function CloseGivenPanel()
	UIManager:StopDrag();
	CloseGivenCount();
	UIManager:UnfreezeAll();
	local givenMenu = LuaUI.GetGivenProcess();
	if givenMenu ~= nil then 
		givenMenu:ResetAll();
	end
	UIManager:DeletePanel("GivenItemCountPanel");
	UIManager:DeletePanel("GivenMovingPanel");
	UIManager:DeletePanel("GivenItemPanel");
	ResetKeyIn();
end

function GivenAddOne()
	local numEdit = UIManager:FindUIEditBox("NumberOfGivenItem");
	local temp = UIManager:GetUIItem("NumberOfGivenItem");
	
	local value = numEdit:GetInt();
	
	if value >= 0 and value < LuaUI.GetMaxStack(_G.GivenItemOrgIndex + 24 * _G.BagState) then
		value = value + 1;
	end
	
	numEdit:SetText(value.."");--value.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);--RenderAll();

end

function GivenDiffOne()

	local numEdit = UIManager:FindUIEditBox("NumberOfGivenItem");
	local temp = UIManager:GetUIItem("NumberOfGivenItem");
	
	local value = numEdit:GetInt();
	
	if value > 1 then
		value = value - 1;
	end
	
	numEdit:SetText(value.."");--value.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function GivenSetZero()
	local numEdit = UIManager:FindUIEditBox("NumberOfGivenItem");
	local temp = UIManager:GetUIItem("NumberOfGivenItem");
	numEdit:SetText("1");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
 end

function GivenMaxStack()

	local numEdit = UIManager:FindUIEditBox("NumberOfGivenItem");
	local temp = UIManager:GetUIItem("NumberOfGivenItem");
	local max = LuaUI.GetMaxStack(_G.GivenItemOrgIndex + 24 * _G.BagState);
	
	numEdit:SetText(max.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function CloseGivenCount()
	ResetKeyIn();
    local countPanel = UIManager:GetUIItem("GivenItemCountPanel");
    local numEdit = UIManager:FindUIEditBox("NumberOfGivenItem");
    if countPanel ~= nil and numEdit ~= nil then
    	countPanel:SetEnable(false);
    	numEdit:SetText("1");
		numEdit:Redraw();
    end
    
    _G.GivenItemOrgIndex = -1;
    _G.PlacedItemIndex = -1;
    UIManager:ClearMonoPanel();
end

function ConfirmGivenNum()

	local givenProcess = LuaUI.GetGivenProcess();
	 local numEdit = UIManager:FindUIEditBox("NumberOfGivenItem");
	 
	if givenProcess ~= nil and numEdit ~= nil then
		local value = numEdit:GetInt();
		
		if value > 0  and value <= LuaUI.GetMaxStack(_G.GivenItemOrgIndex + 24 * _G.BagState) and _G.PlacedItemIndex >= 0 then
			
			givenProcess:SetItem(_G.PlacedItemIndex, _G.GivenItemOrgIndex, value);
			
			local IconName = "";
				
							
		 	if _G.GivenItemOrgIndex < 10 then	
				IconName = "ItemSlot00".._G.GivenItemOrgIndex.."Icon";
				
			elseif _G.GivenItemOrgIndex >= 10 and _G.GivenItemOrgIndex < 24 then
				IconName = "ItemSlot0".._G.GivenItemOrgIndex.."Icon";
				
			end
				
			--LuaUI.CopyImageForUIImage(IconName,"GivenItem00".._G.PlacedItemIndex.."Icon");
			
			local aniIcon = UIManager:FindUIAniIcon(IconName);
			local givenAniIcon = UIManager:FindUIAniIcon("GivenItem00".._G.PlacedItemIndex.."Icon");
			
			if aniIcon ~= nil and givenAniIcon ~= nil then
			    givenAniIcon:ClearAniData();
			    givenAniIcon:SetAnimationData(aniIcon);
			end
			
			local orgIcon = UIManager:GetUIItem(IconName);
			local text = UIManager:FindUIStaticText("GivenItem00".._G.PlacedItemIndex.."Stack");
			local textTemp = UIManager:GetUIItem("GivenItem00".._G.PlacedItemIndex.."Stack");
			local image = UIManager:GetUIItem("GivenItem00".._G.PlacedItemIndex.."Icon");
			if text ~= nil and image ~= nil and orgIcon ~= nil then
				--[[if value == 1 then
					text:LuaSetText("");
				else
					text:LuaSetText(value.."");
				end
				text:Redraw();
				textTemp:SetEnable(true);
				UIManager:AddDirtyUIItem(textTemp);
				    ]]
				--orgIcon:SetRenderStyle(1);
				--UIManager:AddDirtyUIItem(orgIcon);
				image:SetEnable(true);
				UIManager:AddDirtyUIItem(image);
			end
			
		end 
	end

	CloseGivenCount();
	
end

function GivenItemMove(index)
	local panel = UIManager:GetUIItem("GivenMovingPanel"); 
	if panel ~= nil then
		local iconName;
		if index >= 0 and index < 6 then
			iconName = "GivenItem00"..index.."Icon";
			_G.DragItemIndex = index;
			
			local orgIcon = UIManager:GetUIItem(iconName);
			if orgIcon ~= nil then
				--LuaUI.CopyImageForUIImage(iconName, "GivenMovingIcon");
				local aniIcon = UIManager:FindUIAniIcon(iconName);
				local givenAniIcon = UIManager:FindUIAniIcon("GivenMovingIcon");

				if aniIcon ~= nil and givenAniIcon ~= nil then
				    givenAniIcon:ClearAniData();
				    givenAniIcon:SetAnimationData(aniIcon);
				end
				UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
				orgIcon:SetRenderStyle(1);
			end
						
		end
		panel:SetEnable(true);
		UIManager:AddDirtyUIItem(panel);
	end

end

function ResetGivenDrag()

	UIManager:ResetDragItem();
	UIManager:StopDrag();
	 
	local iconName;
	if _G.DragItemIndex >= 0 then
	 	iconName = "GivenItem00".._G.DragItemIndex.."Icon";
	elseif _G.DragPartnerIndex >= 0 then 
		iconName = "GivenPartner00".._G.DragPartnerIndex.."Icon";
	elseif _G.DragPetIndex >= 0 then
		iconName = "GivenPet00".._G.DragPetIndex.."Icon";
	end
	
	local orgIcon = UIManager:GetUIItem(iconName);
	if orgIcon ~= nil then
		orgIcon:SetRenderStyle(0);
		UIManager:AddDirtyUIItem(orgIcon);
	end
	
	local panel = UIManager:GetUIItem("GivenMovingPanel"); 
	if panel ~= nil then
		panel:SetPos(0,0);
		panel:SetEnable(false);
	end
	
	_G.DragItemIndex = -1;
	_G.DragPartnerIndex = -1;
	_G.DragPetIndex = -1;
end

function GivenItemMove000()
	GivenItemMove(0);
end

function GivenItemMove001()
    GivenItemMove(1);
end

function GivenItemMove002()
	GivenItemMove(2);
end

function GivenItemMove003()
	GivenItemMove(3);
end

function GivenItemMove004()
	GivenItemMove(4);
end

function GivenItemMove005()
	GivenItemMove(5);
end

function GivenItemMove006()
	GivenItemMove(6);
end


function GivenEndDrag(value)
	if value ~= 3 then
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();

		local iconName;
		local stackName;
		local levelName;

		if _G.DragItemIndex >= 0 then
		 	iconName = "GivenItem00".._G.DragItemIndex.."Icon";
		 	stackName = "GivenItem00".._G.DragItemIndex.."Stack";
		 	levelName = "GivenItem00".._G.DragItemIndex.."Lvl";
		elseif _G.DragPartnerIndex >= 0 then
			iconName = "GivenPartner00".._G.DragPartnerIndex.."Icon";
			levelName = "GivenPartner00".._G.DragPartnerIndex.."Lvl";
		elseif _G.DragPetIndex >= 0 then
			iconName = "GivenPet00".._G.DragPetIndex.."Icon";
			levelName = "GivenPet00".._G.DragPetIndex.."Lvl";
		end

		local itemMenu = UIManager:GetUIItem("ItemMenuPanel");

		if itemMenu ~= nil then
		    local itemx = itemMenu:GetX();
		    local itemy = itemMenu:GetY();

			local diffx = mx - itemx;
			local diffy = my - itemy;
			local givenProcess = LuaUI.GetGivenProcess();

            if givenProcess ~= nil then
				if diffx >= 25 and diffx < 272 and 
					diffy > 281 and diffy < 446 and  _G.DragItemIndex >= 0 then
					givenProcess:RemoveItem(_G.DragItemIndex);
					
					local img = UIManager:GetUIItem(iconName);
					local stack = UIManager:GetUIItem(stackName);
					local level = UIManager:GetUIItem(levelName);
		
					if img ~= nil and stack ~= nil and level ~= nil then
						img:SetRenderStyle(0);
						img:SetEnable(false);
						stack:SetEnable(false);
						level:SetEnable(false);
					end
					
				elseif diffx >= 0 and diffx < itemMenu:GetWidth() and 
					diffy > 0 and diffy < itemMenu:GetHeight() then
					
					if _G.DragPartnerIndex >= 0 or _G.DragPetIndex >= 0 then
						if _G.DragPartnerIndex >= 0 then
							givenProcess:RemovePartner(_G.DragPartnerIndex);
						elseif _G.DragPetIndex >= 0 then
							givenProcess:RemovePet(_G.DragPetIndex);
						end
						
						local img = UIManager:GetUIItem(iconName);
						local level = UIManager:GetUIItem(levelName);
			
						if img ~= nil and level ~= nil then
							img:SetRenderStyle(0);
							img:SetEnable(false);
							level:SetEnable(false);
						end
					end
				end
		    end
		end
	end
	ResetGivenDrag();
end

function GivenPartnerMove(index)

	local panel = UIManager:GetUIItem("GivenMovingPanel"); 
	if panel ~= nil then
		local iconName;
		if index >= 0 and index < 3 then
			iconName = "GivenPartner00"..index.."Icon";
			_G.DragPartnerIndex = index;
			
			local orgIcon = UIManager:GetUIItem(iconName);
			if orgIcon ~= nil then
				--LuaUI.CopyImageForUIImage(iconName, "GivenMovingIcon");
				local aniIcon = UIManager:FindUIAniIcon(iconName);
				local givenAniIcon = UIManager:FindUIAniIcon("GivenMovingIcon");

				if aniIcon ~= nil and givenAniIcon ~= nil then
				    givenAniIcon:ClearAniData();
				    givenAniIcon:SetAnimationData(aniIcon);
				end
				UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
				orgIcon:SetRenderStyle(1);
			end
		end
		panel:SetEnable(true);
		UIManager:AddDirtyUIItem(panel);
	end

end

function GivenPartnerMove000()
	GivenPartnerMove(0);
end

function GivenPartnerMove001()
	GivenPartnerMove(1);
end

function GivenPartnerMove002()
	GivenPartnerMove(2);
end

function GivenPetMove(index)
	local panel = UIManager:GetUIItem("GivenMovingPanel"); 
	if panel ~= nil then
		local iconName;
		if index >= 0 and index < 3 then
			iconName = "GivenPet00"..index.."Icon";
			_G.DragPetIndex = index;
			
			local orgIcon = UIManager:GetUIItem(iconName);
			if orgIcon ~= nil then
				--LuaUI.CopyImageForUIImage(iconName, "GivenMovingIcon");
				local aniIcon = UIManager:FindUIAniIcon(iconName);
				local givenAniIcon = UIManager:FindUIAniIcon("GivenMovingIcon");

				if aniIcon ~= nil and givenAniIcon ~= nil then
				    givenAniIcon:ClearAniData();
				    givenAniIcon:SetAnimationData(aniIcon);
				end
				UIManager:SetDragItemXY(panel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
				orgIcon:SetRenderStyle(1);
			end
		end
		panel:SetEnable(true);
		UIManager:AddDirtyUIItem(panel);
	end
end

function GivenPetMove000()
	GivenPetMove(0);
end

function GivenPetMove001()
	GivenPetMove(1);
end

function GivenPetMove002()
	GivenPetMove(2);
end

function GivenConfirm()
	local givenProcess = LuaUI.GetGivenProcess();
	if givenProcess ~= nil then
		givenProcess:ConfirmGiven();	   	
	end

end

function ResetAllGivenImage()
	local iconName;
	local stackName;
	local levelName;
	local icon;
	local stack;
	local level;
	
	for i=0,5 do
		iconName = "GivenItem00"..i.."Icon";
		stackName = "GivenItem00"..i.."Stack";
		levelName = "GivenItem00"..i.."Lvl";
		icon = UIManager:GetUIItem(iconName);
		if icon ~= nil then
			icon:SetEnable(false);
		end
		
		stack = UIManager:GetUIItem(stackName);
		if stack ~= nil then
			stack:SetEnable(false);
		end
		
		level = UIManager:GetUIItem(levelName);
		if level ~= nil then
			level:SetEnable(false);		   	
		end
	end
	
	for i=0,2 do 
		iconName = "GivenPartner00"..i.."Icon";
		levelName = "GivenPartner00"..i.."Lvl";
		
		icon = UIManager:GetUIItem(iconName);
		if icon ~= nil then
			icon:SetEnable(false);
		end
		
		level = UIManager:GetUIItem(levelName);
		if level ~= nil then
			level:SetEnable(false);		   	
		end
	end
	
	for i=0,2 do 
		iconName = "GivenPet00"..i.."Icon";
		levelName = "GivenPet00"..i.."Lvl";
		
		icon = UIManager:GetUIItem(iconName);
		if icon ~= nil then
			icon:SetEnable(false);
		end
		
		level = UIManager:GetUIItem(levelName);
		if level ~= nil then
			level:SetEnable(false);		   	
		end
	end
	
	local numEdit = UIManager:FindUIEditBox("GivenMoney");
	if numEdit ~= nil then
		value = 0;
		numEdit:SetText(value.."");--value.."");
		numEdit:Redraw();
	end
	
	local panel = UIManager:GetUIItem("GivenItemPanel");
	if panel ~= nil then
		UIManager:AddDirtyUIItem(panel);	   	
	end
end

function SetKeyInMoney()
	UIManager:SetKeyInItem("GivenMoney");
end

function SetMoneyColour()
	local givenProcess = LuaUI.GetGivenProcess();
	if givenProcess ~= nil then
		givenProcess:ShowMoneyColour();	   	
	end
end