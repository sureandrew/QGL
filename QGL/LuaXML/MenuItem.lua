MeunEquipItemStartX = {178, 226, 274}
MenuEquipItemStartY = {42, 90, 138}

MenuItemStartX = {26, 67, 108, 149, 190, 231}
MenuItemStartY = {282, 323, 364, 405}


BagState = 0;

WindowState = 0; --0: character, 1:partner, 2:pet
ItemIndexNow = -1;

DragItemPicIndex = -1;

m_ItemDetailX = 0;
m_ItemDetailY = 0;
-- State 0:closed, 1:opened
PanelItemDetailState = 0;
-- Item Pos
PanelItemDetailCurNo = -1;

DragType = -1; --0: item, 1:partenr, 2:pet

g_ShorcutSelSlotType = 0;
g_ShorcutSelSlotRef = 0;

LockState = false;
UnlockState = false;

function CloseItemMenu(pX, pY)
	local panel = UIManager:GetUIItem("ItemMenuPanel");
	local dropPanel = UIManager:GetUIItem("DropItemPanel");
	local confirmDropPanel = UIManager:GetUIItem("ConfirmDropItemPanel");
	if panel ~= nil then
		UIManager:StopDrag();
		UIManager:DeletePanel("ItemMenuPanel");
		CloseDropPanel();

		PlayCloseInterfaceEffect();

		panel = UIManager:GetUIItem("MovingPanel");
		if panel ~= nil then
			panel:SetEnable(false);
			UIManager:DeletePanel("MovingPanel");
		end
		
		CloseItemDetail(0, 0);
		ResetAllLockStatus();
		-- reset the cursor to normal
	end
end

function ItemCharacter(pX, pY)
	ResetButtonState();
	
 	CharacterMenuSetEnable(true);
	NameListMenuSetEnable(false);
	PartyMenuSetEnable(false);
	PetMenuSetEnable(false);
	ItemTipsSetEnable(true);
		
	local Button = UIManager:FindUIButton("ItemCharacter");
	if Button ~= nil then
        Button:SetClickState();
	    Button:SetLockState(true);
	end
	
	_G.WindowState = 0;
	
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
		itemProcess:NewUpdateItemCharacter();
	end
	
end

function ItemTipsSetEnable(flag)
	UIManager:UIItemSetEnable("ItemNeckTips", flag);
	UIManager:UIItemSetEnable("ItemHeadTips", flag);
	UIManager:UIItemSetEnable("ItemHatTips", flag);
	
	UIManager:UIItemSetEnable("ItemWeaponTips", flag);
	UIManager:UIItemSetEnable("ItemShirtTips", flag);
	UIManager:UIItemSetEnable("ItemClothTips", flag);
	
	UIManager:UIItemSetEnable("ItemRingTips", flag);
	UIManager:UIItemSetEnable("ItemShoesTips", flag);
	UIManager:UIItemSetEnable("ItemBeltTips", flag);
end

function PetMenuSetEnable(flag)
    ItemMenuSetEnable("PetPanelImage1", flag);
	ItemMenuSetEnable("PetPanelImage2", flag);
	ItemMenuSetEnable("ItemPetHappyBar", flag);
	ItemMenuSetEnable("ItemPetVPBar", flag);
	ItemMenuSetEnable("ItemPetLVText", flag);
	ItemMenuSetEnable("ItemPetHappyText", flag);
    ItemMenuSetEnable("ItemPetVPText", flag);
end

function PartyMenuSetEnable(flag)
    ItemMenuSetEnable("PartyPanelImage1", flag);
	ItemMenuSetEnable("PartyPanelImage2", flag);
	ItemMenuSetEnable("ItemPartyHPBar", flag);
	ItemMenuSetEnable("ItemPartySPBar", flag);
    ItemMenuSetEnable("ItemPartyLikeText", flag);
	ItemMenuSetEnable("ItemPartyHPText", flag);
	ItemMenuSetEnable("ItemPartySPText", flag);
end

function CharacterMenuSetEnable(flag)
	ItemMenuSetEnable("ItemEquipPanel", flag);
	ItemMenuSetEnable("MoneyInterface1", flag);
	ItemMenuSetEnable("MoneyInterface2", flag);
	ItemMenuSetEnable("ItemCashDollar", flag);
	ItemMenuSetEnable("ItemBankDollar", flag);
	ItemMenuSetEnable("ItemGoldDollar", flag);
	if flag and (_G.LockState or _G.UnlockState) then
    	ItemMenuSetEnable("YuanBaoLock", true);
        ItemMenuSetEnable("BankMoneyLock", true);
	else
	    ItemMenuSetEnable("YuanBaoLock", false);
  	    ItemMenuSetEnable("BankMoneyLock", false);
	end
end

function NameListMenuSetEnable(flag)
	for i=1, 6 do
		local word = "ItemNameListLine"..i;
		ItemMenuSetEnable(word, flag);
		word = "ItemNameImage"..i;
		ItemMenuSetEnable(word, false);
		word = "ItemNameSelected"..i;
		ItemMenuSetEnable(word, false);
		ItemMenuSetEnable("ItemPartnerState"..i.."1", false);
		ItemMenuSetEnable("ItemPartnerState"..i.."2", false);
		local lockname = "PetPartnerLock"..i;
		ItemMenuSetEnable(lockname, false); 
	end
    
	ItemMenuSetEnable("ItemNameListPanel", flag);
	ItemMenuSetEnable("ItemNameListScroll", flag);

end

function ItemPartyMember(pX, pY)
	ResetButtonState();

    CharacterMenuSetEnable(false);
 	NameListMenuSetEnable(true);
	PartyMenuSetEnable(true);
	PetMenuSetEnable(false);
	ItemTipsSetEnable(false);
	
	local Button = UIManager:FindUIButton("ItemPartyMember");
	if Button ~= nil then
        Button:SetClickState();
	    Button:SetLockState(true);
	end
	
	for i=0,8 do
		IconSetEnable("ItemEquip00"..i.."Icon", false, 120 + i);
		ItemMenuSetEnable("ItemEquip00"..i.."Stack", false);
		ItemMenuSetEnable("ItemEquip00"..i.."Lvl", false);
	end
	_G.WindowState = 1;
	
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
		itemProcess:UpdateItemPartner();
		itemProcess:ResetScroll();
	end
end

function ItemPet(pX, pY)
    ResetButtonState();

    CharacterMenuSetEnable(false);
    NameListMenuSetEnable(true);
	PartyMenuSetEnable(false);
	PetMenuSetEnable(true);
	ItemTipsSetEnable(false);
	
	local Button = UIManager:FindUIButton("ItemPet");
	if Button ~= nil then
        Button:SetClickState();
	    Button:SetLockState(true);
	end
	
	for i=0,8 do
  		IconSetEnable("ItemEquip00"..i.."Icon", false, 120+i);
		ItemMenuSetEnable("ItemEquip00"..i.."Stack", false);
		ItemMenuSetEnable("ItemEquip00"..i.."Lvl", false);
	end
	_G.WindowState = 2;
	
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
	    
		itemProcess:UpdateItemPet();
		itemProcess:ResetScroll();
	end
end

function ItemCurrentBag(pX, pY)
	_G.BagState = 0;
    ResetBagButtonState();
	
	local Button = UIManager:FindUIButton("ItemCurrentBag");
	if Button ~= nil then
        Button:SetClickState();
	    Button:SetLockState(true);
	end

    local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
		itemProcess:NewUpdateItemMenu(0);
	end
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(6);
end

function ItemSideBag(pX, pY)
    _G.BagState = 1;
	ResetBagButtonState();

    local Button = UIManager:FindUIButton("ItemSideBag");
	if Button ~= nil then
        Button:SetClickState();
	    Button:SetLockState(true);
	end
	
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
		itemProcess:NewUpdateItemMenu(1);
	end
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(6);
end

function ItemBagExtra01(pX, pY)
    _G.BagState = 2;
    ResetBagButtonState();
	
	local Button = UIManager:FindUIButton("ItemBagExtra01");
	if Button ~= nil then
        Button:SetClickState();
	    Button:SetLockState(true);
	end
	
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
		itemProcess:NewUpdateItemMenu(2);
	end
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(5);
end

function ItemBagExtra02(pX, pY)
    _G.BagState = 3;
    ResetBagButtonState();
    	
	local Button = UIManager:FindUIButton("ItemBagExtra02");
	if Button ~= nil then
        Button:SetClickState();
	    Button:SetLockState(true);
	end
	
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
		itemProcess:NewUpdateItemMenu(3);
	end
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(5);
end

function ItemBagExtra03(pX, pY)
    _G.BagState = 4;
	ResetBagButtonState();

	local Button = UIManager:FindUIButton("ItemBagExtra03");
	if Button ~= nil then
        Button:SetClickState();
	    Button:SetLockState(true);
	end
	
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
		itemProcess:NewUpdateItemMenu(4);
	end	
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(5);
end

function ItemMenuSetEnable(pName, value)
	local panel = UIManager:GetUIItem(pName);
	if panel ~= nil then
		panel:SetEnable(value);
		UIManager:AddDirtyUIItem(panel);
	end	
end


function IconSetEnable(pName, value, index)
	local icon = UIManager:GetUIItem(pName);

	if icon ~= nil then
	    if value and LuaUI.HasItem(index) then
	       	icon:SetEnable(true);
    	else
		    icon:SetEnable(false);
		end
		UIManager:AddDirtyUIItem(icon);
	end
end

function ItemUse(slot)
	if _G.InBattle == false and _G.LockState == false and _G.UnlockState == false then
		local iconName;
		if slot < 10 then
		    iconName = "ItemSlot00"..slot.."Icon";
		else
		    iconName = "ItemSlot0"..slot.."Icon";
		end
		local icon = UIManager:GetUIItem(iconName);
		if icon ~= nil and icon:GetRenderStyle() ~= 1 then
		    local index = slot + _G.BagState * 24;
		    local wareHousePanel = UIManager:GetUIItem("WarehousePanel");
		    if wareHousePanel ~= nil then
		        _G.GivenItemOrgIndex = slot;
				_G.PlacedItemIndex = 255;
		        local max = LuaUI.GetMaxStack(index);
				local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseItem");
				if numEdit ~= nil then
					numEdit:SetText(max.."");
				end
				ConfirmWareHouseNum();
		    else
		    
			    local itemProcess = LuaUI.GetItemProcess();
				if itemProcess ~= nil then
					itemProcess:ItemUse(index);
				end
			end
			CloseItemDetail(0, 0);
		end
	end
end

function ItemUse000(pX, pY)
	ItemUse(0);
end

function ItemUse001(pX, pY)
 	ItemUse(1);
end

function ItemUse002(pX, pY)
 	ItemUse(2);
end

function ItemUse003(pX, pY)
    ItemUse(3);
end

function ItemUse004(pX, pY)
 	ItemUse(4);
end

function ItemUse005(pX, pY)
  	ItemUse(5);
end

function ItemUse006(pX, pY)
    ItemUse(6);
end

function ItemUse007(pX, pY)
    ItemUse(7);
end

function ItemUse008(pX, pY)
    ItemUse(8);
end

function ItemUse009(pX, pY)
    ItemUse(9);
end

function ItemUse010(pX, pY)
 	ItemUse(10);
end

function ItemUse011(pX, pY)
 	ItemUse(11);
end

function ItemUse012(pX, pY)
    ItemUse(12);
end

function ItemUse013(pX, pY)
    ItemUse(13);
end

function ItemUse014(pX, pY)
    ItemUse(14);
end

function ItemUse015(pX, pY)
    ItemUse(15);
end

function ItemUse016(pX, pY)
    ItemUse(16);
end

function ItemUse017(pX, pY)
    ItemUse(17);
end

function ItemUse018(pX, pY)
    ItemUse(18);
end

function ItemUse019(pX, pY)
    ItemUse(19);
end

function ItemUse020(pX, pY)
    ItemUse(20);
end

function ItemUse021(pX, pY)
    ItemUse(21);
end

function ItemUse022(pX, pY)
    ItemUse(22);
end

function ItemUse023(pX, pY)
    ItemUse(23);
end

function ItemEquip000(pX, pY)
    ItemEquip(120);
end

function ItemEquip001(pX, pY)
   	ItemEquip(121);
end

function ItemEquip002(pX, pY)
  	ItemEquip(122);
end

function ItemEquip003(pX, pY)
  	ItemEquip(123);
end

function ItemEquip004(pX, pY)
  	ItemEquip(124);
end

function ItemEquip005(pX, pY)
   	ItemEquip(125);
end

function ItemEquip006(pX, pY)
   	ItemEquip(126);
end

function ItemEquip007(pX, pY)
   	ItemEquip(127);
end

function ItemEquip008(pX, pY)
    ItemEquip(128);
end

function ItemEquip(index)
	if _G.InBattle == false then
	    local itemProcess = LuaUI.GetItemProcess();
		if itemProcess ~= nil then
			itemProcess:ItemUse(index);
		end
			CloseItemDetail(0, 0);
	end
end

function ItemDetail(pMouseIn, pItemPos)
	local XOffset = 20; --add by zc 2009 - 9 - 4
	local YOffset = 20;
	if LuaUI.HasItem(pItemPos) then 
		local pX = UIManager:GetCursorX();
		local pY = UIManager:GetCursorY();
		local panel = UIManager:GetUIItem("ItemDetailMenuPanel");

		if panel == nil then
			LuaUI.AddXMLFile2UIManger("Data\\ItemDetail.xml");
			panel = UIManager:GetUIItem("ItemDetailMenuPanel")
   			local itemProcess = LuaUI.GetItemProcess();
			if itemProcess ~= nil then
				itemProcess:UpdateItemDetail(pItemPos)
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
			PanelItemDetailState = 1;
			PanelItemDetailCurNo = pItemPos; 
		end

	end
end

function ItemDetailMouseOut()
    local itemboard = UIManager:GetUIItem("ItemBoard");
 	itemboard:SetEnable(false);
	UIManager:AddDirtyUIItem(itemboard);
	CloseItemDetail(0, 0);
end

function ItemMove000()	
	ItemMove(0);
end

function ItemMove001()
	ItemMove(1);
end

function ItemMove002()
	ItemMove(2);
end

function ItemMove003()
	ItemMove(3);
end

function ItemMove004()
	ItemMove(4);
end

function ItemMove005()
	ItemMove(5);
end

function ItemMove006()
	ItemMove(6);
end

function ItemMove007()
	ItemMove(7);
end

function ItemMove008()
	ItemMove(8);
end

function ItemMove009()
	ItemMove(9);
end

function ItemMove010()
	ItemMove(10);
end

function ItemMove011()
	ItemMove(11);
end

function ItemMove012()
	ItemMove(12);
end

function ItemMove013()
	ItemMove(13);
end

function ItemMove014()
	ItemMove(14);
end

function ItemMove015()
	ItemMove(15);
end

function ItemMove016()
	ItemMove(16);
end

function ItemMove017()
	ItemMove(17);
end

function ItemMove018()
	ItemMove(18);
end

function ItemMove019()
	ItemMove(19);
end

function ItemMove020()
	ItemMove(20);
end

function ItemMove021()
	ItemMove(21);
end

function ItemMove022()
	ItemMove(22);
end

function ItemMove023()
	ItemMove(23);
end

function MoveItemToBag(index)
   local bag1 = UIManager:GetUIItem("ItemCurrentBag");
   local bag2 = UIManager:GetUIItem("ItemSideBag");
   local bag3 = UIManager:GetUIItem("ItemBagExtra01");
   local bag4 = UIManager:GetUIItem("ItemBagExtra02");
   local bag5 = UIManager:GetUIItem("ItemBagExtra03");

   if bag1 ~= nil and bag2 ~= nil and bag3 ~= nil and bag4 ~= nil and bag5 ~= nil then
		local X = UIManager:GetCursorX();
		local Y = UIManager:GetCursorY();

        if bag1:GetX() <= X and (bag1:GetX()+40) >= X and bag1:GetY() <= Y and (bag1:GetY()+23) >= Y then
			if index >= 24 then
				LuaUI.MoveToBag(index,0);
			else
				ResetDropItemPos();
			end
		elseif bag2:GetX() <= X and (bag2:GetX()+40) >= X and bag2:GetY() <= Y and (bag2:GetY()+23) >= Y then
			if index < 24 or index >= 48 then
				LuaUI.MoveToBag(index,1);
			else
				ResetDropItemPos();
			end
		elseif bag3:GetX() <= X and (bag3:GetX()+40) >= X and bag3:GetY() <= Y and (bag3:GetY()+23) >= Y then
			if index < 48 or index >= 72 then
           		LuaUI.MoveToBag(index,2);
           	else
				ResetDropItemPos();
			end
		elseif bag4:GetX() <= X and (bag4:GetX()+40) >= X and bag4:GetY() <= Y and (bag4:GetY()+23) >= Y then
			if index < 72 or index >= 96 then
           		LuaUI.MoveToBag(index,3);
           	else
				ResetDropItemPos();
			end
		elseif bag5:GetX() <= X and (bag5:GetX()+40) >= X and bag5:GetY() <= Y and (bag5:GetY()+23) >= Y then
			if index < 96 then           		
				LuaUI.MoveToBag(index,4);
           	else
				ResetDropItemPos();
			end
		else
			local panel = UIManager:GetUIItem("ItemMenuPanel");
			local X2 =  UIManager:GetCursorX() - panel:GetX();
			local Y2 =  UIManager:GetCursorY() - panel:GetY();

			if X2 > _G.MenuItemStartX[1] and X2 < (_G.MenuItemStartX[6] + 40)
			    and Y2 > _G.MenuItemStartY[1] and Y2 < (_G.MenuItemStartY[4] + 40)
				then

				X2 = X2 - _G.MenuItemStartX[1];
				Y2 = Y2 - _G.MenuItemStartY[1];

				local bagIndex = LuaUI.Div(X2, 41) + 6 * LuaUI.Div(Y2, 41);
				
				local itemProcess = LuaUI.GetItemProcess();
				if itemProcess ~= nil and _G.DragItemPicIndex ~= bagIndex and itemProcess:IsItemGiven(bagIndex + 24 * _G.BagState) ~= true then
    				LuaUI.MoveToBag(index, _G.BagState, bagIndex);
    			else
    				ResetDropItemPos();
    			end
  			
    		else
				local zero = "00";
				if _G.DragItemPicIndex >= 10 then
					zero = "0";
				end 
			    local itemIcon = UIManager:GetUIItem("ItemSlot"..zero.._G.DragItemPicIndex.."Icon");
			    local itemStack = UIManager:GetUIItem("ItemSlot"..zero.._G.DragItemPicIndex.."Stack");
			    local itemLevel = UIManager:GetUIItem("ItemSlot"..zero.._G.DragItemPicIndex.."Lvl");
			    
			    itemIcon:SetEnable(true);
			    itemStack:SetEnable(true);
			    itemLevel:SetEnable(true);
			end
		
		end
   end
end


function ShortCutDragEnd(value)
	local HShortCutPanel = UIManager:GetUIItem("HShortCutPanel");
	local VShortCutPanel = UIManager:GetUIItem("VShortCutPanel");
	local DropPanel = UIManager:GetDropPanel();
		
	if DropPanel ~= nil then
		if DropPanel:Equal(HShortCutPanel) then
			local mx = UIManager:GetCursorX();
			local my = UIManager:GetCursorY();

			local imx = DropPanel:GetX();
			local imy = DropPanel:GetY();
			local Sel = LuaUI.Div(mx - imx, 35); 
			
			if Sel >= 0 and Sel <= 9 then
				if g_ShorcutSelSlotType == 1 then
					LuaUI.CopyToShortCut(g_ShorcutSelSlotRef, Sel);
				elseif g_ShorcutSelSlotType == 2 then
					LuaUI.CopySkillToShortCut(g_ShorcutSelSlotRef, Sel);
				end
			end
		elseif DropPanel:Equal(VShortCutPanel) then
			local mx = UIManager:GetCursorX();
			local my = UIManager:GetCursorY();

			local imx = DropPanel:GetX();
			local imy = DropPanel:GetY();
			local Sel = LuaUI.Div(my - imy, 35); 
			
			if Sel >= 0 and Sel <= 9 then
				if g_ShorcutSelSlotType == 1 then
					LuaUI.CopyToShortCut(g_ShorcutSelSlotRef, Sel);
				elseif g_ShorcutSelSlotType == 2 then
					LuaUI.CopySkillToShortCut(g_ShorcutSelSlotRef, Sel);
				end
			end
		end		
	else
		LuaUI.DelShortCutSlot(g_ShorcutSelSlotRef, g_ShorcutSelSlotType);
	end
	
	ResetShortCutDropItemPos()
end



function ResetShortCutDropItemPos()
	UIManager:ResetDragItem();
	UIManager:StopDrag();

	local movingPanel = UIManager:GetUIItem("ShortCutMovingPanel");
	
	if movingPanel ~= nil then
		movingPanel:SetPos(0,0);
		movingPanel:SetEnable(false);
		movingPanel:SetNotDetected(false);
	end
		
	g_ShorcutSelSlotType = 0;
	g_ShorcutSelSlotRef = 0;
		
end

function DragEnd(value)
	if _G.DragType == 0 then
	    ItemDragEnd(value);
	elseif _G.DragType == 1 then
	    PartnerDragEnd(value);
	elseif _G.DragType == 2 then
		PetDragEnd(value);
	end
end

function PartnerDragEnd(value)
	if value == 3 then
	    ResetDropItemPos();
	else
		local itemProcess = LuaUI.GetItemProcess();
		local givenProcess = LuaUI.GetGivenProcess();
		local givenPanel = UIManager:GetUIItem("GivenItemPanel");
		local movingPanel = UIManager:GetUIItem("MovingPanel");
		local tradingProcess = LuaUI.GetTradingProcess();
		local tradingPanel = UIManager:GetUIItem("TradingPanel");
		local wareHouseProcess = LuaUI.GetWareHouseProcess();
		local wareHousePanel = UIManager:GetUIItem("WarehousePanel");
		local partnerCultivationUpProcess = LuaUI.GetPartnerCultivationUpProcess();
		local partnerCultivationUpPanel = UIManager:GetUIItem("PartnerCultivationUpPanel");
		
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();
		
		if itemProcess ~= nil and movingPanel ~= nil then 
			if givenPanel ~= nil  and givenProcess ~= nil then
				local relativeX = mx - givenPanel:GetX();
				local relativeY = my - givenPanel:GetY();
				
				if relativeX > 23 and relativeX < 153 and relativeY > 161 and relativeY < 207 then
					if (givenProcess:IsNpcGiven() and givenProcess:IsSuitableNpcGivenPartner(itemProcess:GetSelectedPartnerID()))
						or givenProcess:IsNpcGiven() == false then
						if itemProcess ~= nil and itemProcess:IsPartnerLocked(itemProcess:GetSelectedPartnerID()) == false then
							local index =  LuaUI.Div( (relativeX - 24), 43);	
							local partnerIconName = "GivenPartner00"..index.."Icon";
						
							local aniIcon = UIManager:FindUIAniIcon("ItemMovingIcon");
							local givenAniIcon = UIManager:FindUIAniIcon(partnerIconName);
						
							if aniIcon ~= nil and givenAniIcon ~= nil then
							    givenAniIcon:ClearAniData();
							    givenAniIcon:SetAnimationData(aniIcon);
							end
						
							givenProcess:SetPartner(index, itemProcess:GetSelectedPartnerID());
							local icon = UIManager:GetUIItem(partnerIconName);
							if icon ~= nil then
								icon:SetEnable(true);
								UIManager:AddDirtyUIItem(icon);
							end
						end
					end
				end
				
			elseif tradingPanel ~= nil  and tradingProcess ~= nil and tradingProcess:IsConfirm() ~= true then
				local relativeX = mx - tradingPanel:GetX();
				local relativeY = my - tradingPanel:GetY();
				
				if relativeX > 23 and relativeX < 153 and relativeY > 332 and relativeY < 378 then
					local index =  LuaUI.Div( (relativeX - 24), 43);
					if itemProcess ~= nil and itemProcess:IsPartnerLocked(itemProcess:GetSelectedPartnerID()) == false then
						local partnerIconName = "TradingPartner00"..index.."Icon";
						
						local aniIcon = UIManager:FindUIAniIcon("ItemMovingIcon");
						local tradingAniIcon = UIManager:FindUIAniIcon(partnerIconName);
					
						if aniIcon ~= nil and tradingAniIcon ~= nil then
						    tradingAniIcon:ClearAniData();
					    	tradingAniIcon:SetAnimationData(aniIcon);
						end
					
						tradingProcess:SetPartner(index, itemProcess:GetSelectedPartnerID());
						local icon = UIManager:GetUIItem(partnerIconName);
						if icon ~= nil then
							icon:SetEnable(true);
							UIManager:AddDirtyUIItem(icon);
						end
					end
				    --ModifyLockState();
				end
			elseif wareHouseProcess ~= nil and wareHousePanel ~= nil then
				local relativeX = mx - wareHousePanel:GetX();
				local relativeY = my - wareHousePanel:GetY();
				
				if relativeX > 28 and relativeX < 272 and relativeY > 63 and relativeY < 390 then
					wareHouseProcess:PutPartnerToWareHouse(itemProcess:GetSelectedPartnerID());
				end
			elseif partnerCultivationUpProcess ~= nil and partnerCultivationUpPanel ~= nil then
				local relativeX = mx - partnerCultivationUpPanel:GetX();
				local relativeY = my - partnerCultivationUpPanel:GetY();
				
				if relativeX > 205 and relativeX < 248 and relativeY > 47 and relativeY < 90 then
					partnerCultivationUpProcess:SetSrcPartner(itemProcess:GetSelectedPartnerID());
				elseif relativeX > 205 and relativeX < 248 and relativeY > 109 and relativeY < 152 then
					if partnerCultivationUpProcess:GetPartnerCultivationUpType() == 0 then
						partnerCultivationUpProcess:SetDestPartner(itemProcess:GetSelectedPartnerID());
					else

					end
				end
			end
					
		end
		ResetDropItemPos();
	end
end

function PetDragEnd(value)
	if value == 3 then
	    ResetDropItemPos();
	else
		local itemProcess = LuaUI.GetItemProcess();
		local givenProcess = LuaUI.GetGivenProcess();
		local givenPanel = UIManager:GetUIItem("GivenItemPanel");
		local movingPanel = UIManager:GetUIItem("MovingPanel");
		local tradingProcess = LuaUI.GetTradingProcess();
		local tradingPanel = UIManager:GetUIItem("TradingPanel");
		local wareHouseProcess = LuaUI.GetWareHouseProcess();
		local wareHousePanel = UIManager:GetUIItem("WarehousePanel");
		
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();
		
		if itemProcess ~= nil and movingPanel ~= nil then 
			if givenPanel ~= nil  and givenProcess ~= nil then
				local relativeX = mx - givenPanel:GetX();
				local relativeY = my - givenPanel:GetY();
				
				if relativeX > 157 and relativeX < 287 and relativeY > 161 and relativeY < 207 then
					if (givenProcess:IsNpcGiven() and givenProcess:IsSuitableNpcGivenPet(itemProcess:GetSelectedPetID()))
						or givenProcess:IsNpcGiven() == false then
						
						if itemProcess ~= nil and itemProcess:IsPetLocked(itemProcess:GetSelectedPetID()) == false then
							local index =  LuaUI.Div( (relativeX - 158), 43);	
							local petIconName = "GivenPet00"..index.."Icon";
						
							--LuaUI.CopyImageForUIImage("ItemMovingIcon", partnerIconName);
							local aniIcon = UIManager:FindUIAniIcon("ItemMovingIcon");
							local givenAniIcon = UIManager:FindUIAniIcon(petIconName);
						
							if aniIcon ~= nil and givenAniIcon ~= nil then
							    givenAniIcon:ClearAniData();
							    givenAniIcon:SetAnimationData(aniIcon);
							end
						
							givenProcess:SetPet(index, itemProcess:GetSelectedPetID());
							local icon = UIManager:GetUIItem(petIconName);
							if icon ~= nil then
								icon:SetEnable(true);
								UIManager:AddDirtyUIItem(icon);
							end
						end
					end
				end
				
			elseif tradingPanel ~= nil  and tradingProcess ~= nil and tradingProcess:IsConfirm() ~= true then
				local relativeX = mx - tradingPanel:GetX();
				local relativeY = my - tradingPanel:GetY();
				
				if relativeX > 157 and relativeX < 287 and relativeY > 332 and relativeY < 378 then
					if itemProcess ~= nil and itemProcess:IsPetLocked(itemProcess:GetSelectedPetID()) == false then 
						local index =  LuaUI.Div( (relativeX - 158), 43);	
						local petIconName = "TradingPet00"..index.."Icon";
					
						--LuaUI.CopyImageForUIImage("ItemMovingIcon", partnerIconName);
						local aniIcon = UIManager:FindUIAniIcon("ItemMovingIcon");
						local tradingAniIcon = UIManager:FindUIAniIcon(petIconName);
					
						if aniIcon ~= nil and tradingAniIcon ~= nil then
					    	tradingAniIcon:ClearAniData();
					    	tradingAniIcon:SetAnimationData(aniIcon);
						end
					
						tradingProcess:SetPet(index, itemProcess:GetSelectedPetID());
						local icon = UIManager:GetUIItem(petIconName);
						if icon ~= nil then
							icon:SetEnable(true);
							UIManager:AddDirtyUIItem(icon);
						end
					end
				    --ModifyLockState();
				end
            elseif wareHouseProcess ~= nil and wareHousePanel ~= nil then
				local relativeX = mx - wareHousePanel:GetX();
				local relativeY = my - wareHousePanel:GetY();

				if relativeX > 28 and relativeX < 272 and relativeY > 63 and relativeY < 390 then
					wareHouseProcess:PutPetToWareHouse(itemProcess:GetSelectedPetID());
				end
			end
					
		end
		ResetDropItemPos();
	end
end

function UseShortCut(Sel)
	Slot, Type = LuaUI.GetShortCutRefNum(Sel), LuaUI.GetShortCutRefType(Sel);
	if Type == 1 then
		if not _G.InBattle then
			local itemProcess = LuaUI.GetItemProcess();
			if itemProcess ~= nil then
				itemProcess:ItemUse(Slot);
				local panel = UIManager:GetUIItem("ItemMenuPanel");
		
				if not (panel ~= nil and panel:IsEnable()) then
					itemProcess:UpdateShortCutSlot(Slot);
				end
			end
		else
			LuaUI.BattleItemUse(Slot);
			CloseBattleItemMenu(0, 0);
			UIManager:RenderAll();
		end
	elseif Type == 2 then
		local skilluse = SkillProcess:UseSkillID(Slot);		
	end
end


function UseShortCutHotKey(Sel)
	local IconName = "HShortCut"..Sel
	local ShortCut = UIManager:GetUIItem(IconName);
	
	--if ShortCut ~= nil and ShortCut:IsEnable() then
		UseShortCut(Sel)
	--end
end


function UseHShortCutSlot0(pX, pY)
	UseShortCut(0)
end

function UseHShortCutSlot1(pX, pY)
	UseShortCut(1)
end

function UseHShortCutSlot2(pX, pY)
	UseShortCut(2)
end

function UseHShortCutSlot3(pX, pY)
	UseShortCut(3)
end

function UseHShortCutSlot4(pX, pY)
	UseShortCut(4)
end

function UseHShortCutSlot5(pX, pY)
	UseShortCut(5)
end

function UseHShortCutSlot6(pX, pY)
	UseShortCut(6)
end

function UseHShortCutSlot7(pX, pY)
	UseShortCut(7)
end

function UseHShortCutSlot8(pX, pY)
	UseShortCut(8)
end

function UseHShortCutSlot9(pX, pY)
	UseShortCut(9)
end



function HShortCutMove(Index)
	
	local movingPanel = UIManager:GetUIItem("ShortCutMovingPanel");
	local IconName = "HShortCut"..Index
	local orgIcon = UIManager:GetUIItem(IconName);
	
	if movingPanel ~= nil and orgIcon ~= nil then
		local orgAniIcon = UIManager:FindUIAniIcon(IconName);
		local targetAniIcon = UIManager:FindUIAniIcon("ShortCutMovingIcon");
		
		if orgAniIcon ~= nil and targetAniIcon ~= nil then
		    targetAniIcon:ClearAniData();
		    targetAniIcon:SetAnimationData(orgAniIcon);
		end
		
		g_ShorcutSelSlotRef = LuaUI.GetShortCutRefNum(Index)
		g_ShorcutSelSlotType = LuaUI.GetShortCutRefType(Index)
			
		UIManager:SetDragItemXY(movingPanel, orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());
			
  		movingPanel:SetEnable(true);
		UIManager:AddDirtyUIItem(movingPanel);
		
		PlayDragItemUpEffect();
	end
end


function MoveHShortCutSlot0(pX, pY)
	if not _G.InBattle then
		HShortCutMove(0)
	else
		UseShortCut(0)
	end
end

function MoveHShortCutSlot1(pX, pY)
	if not _G.InBattle then
		HShortCutMove(1)
	else
		UseShortCut(1)
	end
end

function MoveHShortCutSlot2(pX, pY)
	if not _G.InBattle then
		HShortCutMove(2)
	else
		UseShortCut(2)
	end
end

function MoveHShortCutSlot3(pX, pY)
	if not _G.InBattle then
		HShortCutMove(3)
	else
		UseShortCut(3)
	end
end

function MoveHShortCutSlot4(pX, pY)
	if not _G.InBattle then
		HShortCutMove(4)
	else
		UseShortCut(4)
	end
end

function MoveHShortCutSlot5(pX, pY)
	if not _G.InBattle then
		HShortCutMove(5)
	else
		UseShortCut(5)
	end
end

function MoveHShortCutSlot6(pX, pY)
	if not _G.InBattle then
		HShortCutMove(6)
	else
		UseShortCut(6)
	end
end

function MoveHShortCutSlot7(pX, pY)
	if not _G.InBattle then
		HShortCutMove(7)
	else
		UseShortCut(7)
	end
end

function MoveHShortCutSlot8(pX, pY)
	if not _G.InBattle then
		HShortCutMove(8)
	else
		UseShortCut(8)
	end
end

function MoveHShortCutSlot9(pX, pY)
	if not _G.InBattle then
		HShortCutMove(9)
	else
		UseShortCut(9)
	end
end




function ItemDragEnd(value)
	local HShortCutPanel = UIManager:GetUIItem("HShortCutPanel");
	local VShortCutPanel = UIManager:GetUIItem("VShortCutPanel");
	local DropPanel = UIManager:GetDropPanel();
	
	if value == 3 then
	    ResetDropItemPos();
	elseif DropPanel ~= nil and DropPanel:Equal(HShortCutPanel) then
		-- ÓÃÁ¦¸Ä
		
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();

		local imx = DropPanel:GetX();
		local imy = DropPanel:GetY();
		local Sel = LuaUI.Div(mx - imx, 35); 
		
		
		if Sel >= 0 and Sel <= 9 and _G.BagState == 0 then
			LuaUI.CopyToShortCut(_G.DragItemPicIndex, Sel);
		end
		ResetDropItemPos();
	elseif DropPanel ~= nil and DropPanel:Equal(VShortCutPanel) then
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();

		local imx = DropPanel:GetX();
		local imy = DropPanel:GetY();
		local Sel = LuaUI.Div(my - imy, 35); 
		
		
		if Sel >= 0 and Sel <= 9 and _G.BagState == 0 then
			LuaUI.CopyToShortCut(_G.DragItemPicIndex, Sel);
		end
		ResetDropItemPos();
	else
	    local panel = UIManager:GetUIItem("ItemMenuPanel");
	    local boundPanel = UIManager:GetUIItem("ItemEquipPanel");

		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();

		local imx = panel:GetX();
		local imy = panel:GetY();

		if imx <= mx and (imx+339) >= mx and imy <= my and (imy+459) >= my then

	        local movingPanel = UIManager:GetUIItem("MovingPanel");
	        movingPanel:SetPos(0,0);
	        movingPanel:SetEnable(false);

	        if _G.DragItemPicIndex < 24 then
	        	local zero = "00";
				if _G.DragItemPicIndex >= 10 then
					zero = "0";
				end
		        local item = UIManager:GetUIItem("ItemSlot"..zero.._G.DragItemPicIndex.."Icon");
		        item:SetRenderStyle(0);
		        item:SetEnable(false);

		        if boundPanel:GetX() <= mx and (boundPanel:GetX()+142) >= mx
				    and boundPanel:GetY() <= my and (boundPanel:GetY()+142) >= my
				    and _G.WindowState == 0
				    then

					LuaUI.EquipItemUse(_G.DragItemPicIndex + 24 * _G.BagState);

				elseif (item:GetX() > mx or (item:GetX()+41) < mx)
				    or (item:GetY() > my or (item:GetY()+41) < my)
				    then
				    MoveItemToBag(_G.DragItemPicIndex + 24 * _G.BagState);
				else
		      		ResetDropItemPos();
				end
			else
	            EquipItemMove();
			end

		else
			
			--for itemdrag to giftbox panel
			local giftBoxPanel = UIManager:GetUIItem("GiftBoxPanel");
			if giftBoxPanel ~= nil then
				local gmx = giftBoxPanel:GetX();
				local gmy = giftBoxPanel:GetY();
				
				if ( mx >= gmx ) and ( mx <= (gmx + giftBoxPanel:GetWidth()) ) and
				   ( my >= gmy ) and ( my <= (gmy + giftBoxPanel:GetHeight()) ) then
				   	ResetDropItemPos();
					do return end;
				end
			end
			--for item drag to given panel
			local givenPanel = UIManager:GetUIItem("GivenItemPanel");
			if givenPanel ~= nil then
				local gmx = givenPanel:GetX();
				local gmy = givenPanel:GetY();
				
				if ( mx >= gmx ) and ( mx <= (gmx + givenPanel:GetWidth()) ) and
				   ( my >= gmy ) and ( my <= (gmy + givenPanel:GetHeight()) ) then
				   
					local locationX = mx - gmx;
					local locationY = my - gmy;    
					
					if _G.DragItemPicIndex < 24 and locationY >= 96 and 
						locationY <= 142 and locationX >= 28 and locationX < 286 then
						
						local index =  LuaUI.Div( (locationX - 28), 43);
						
						local itemProcess = LuaUI.GetItemProcess();
						if itemProcess == nil then
							ResetDropItemPos();
							do return end;
						end
						    
						local itemIndex = _G.DragItemPicIndex + 24 * _G.BagState;
					    if itemProcess:IsItemBind(itemIndex) then
					    	itemProcess:BindCantTrading(1);
					        ResetDropItemPos();
							do return end;
					    end
						
						local countPanel = UIManager:GetUIItem("GivenItemCountPanel");
						local tempPanel = UIManager:FindUIPanel("GivenItemCountPanel");
						local givenProcess = LuaUI.GetGivenProcess();
						if countPanel ~= nil and tempPanel ~= nil and givenProcess then
							if givenProcess:IsNpcGiven() and givenProcess:IsSuitableNpcGivenItem(_G.DragItemPicIndex + 24 * _G.BagState) == false then
					        	ResetDropItemPos();
								do return end;
							end
							_G.GivenItemOrgIndex = _G.DragItemPicIndex;
							_G.PlacedItemIndex = index;
							if LuaUI.GetMaxStack(_G.DragItemPicIndex + 24 * _G.BagState) == 1 or givenProcess:IsNpcGiven() then
								local numEdit = UIManager:FindUIEditBox("NumberOfGivenItem");
								if numEdit ~= nil then
									numEdit:SetText("1");
								end
								ResetDropItemPos();
								ConfirmGivenNum();
								do return end;	
							else
								UIManager:SetPanel2Top(tempPanel);
								countPanel:SetEnable(true);
								UIManager:SetKeyInItem("NumberOfGivenItem");
								UIManager:ClearMonoPanel();
						    	UIManager:SetMonoPanel(tempPanel);	
						    	UIManager:AddDirtyUIItem(countPanel);
					    	end
						end
					end
					ResetDropItemPos();
					do return end;
				end
			end
			
			--For item drag to trading panel
			local tradingPanel = UIManager:GetUIItem("TradingPanel");			
			if tradingPanel ~= nil then
				local tmx = tradingPanel:GetX();
				local tmy = tradingPanel:GetY();
				
				if ( mx >= tmx ) and ( mx <= (tmx + tradingPanel:GetWidth()) ) and
				   ( my >= tmy ) and ( my <= (tmy + tradingPanel:GetHeight()) ) then
				   
					local locationX = mx - tmx;
					local locationY = my - tmy;   
					
					local tradingProcess = LuaUI.GetTradingProcess();		
					if tradingProcess ~= nil and tradingProcess:IsConfirm() then
						ResetDropItemPos();
						do return end;
					end 
					
					if _G.DragItemPicIndex < 24 and locationY >= 268 and 
						locationY <= 313 and locationX >= 28 and locationX < 286 then

						local index =  LuaUI.Div( (locationX - 28), 43);
						local itemProcess = LuaUI.GetItemProcess();
						if itemProcess ~= nil then
						    local itemIndex = _G.DragItemPicIndex + 24 * _G.BagState;
						    if itemProcess:IsItemBind(itemIndex) then
								itemProcess:BindCantTrading(0);
						        ResetDropItemPos();
								do return end;
						    end
						end
						--ModifyLockState();
						local countPanel = UIManager:GetUIItem("TradingItemCountPanel");
						local tempPanel = UIManager:FindUIPanel("TradingItemCountPanel");
						if countPanel ~= nil and tempPanel ~= nil then
							_G.GivenItemOrgIndex = _G.DragItemPicIndex;
							_G.PlacedItemIndex = index;
							if LuaUI.GetMaxStack(_G.DragItemPicIndex + 24 * _G.BagState) == 1 then
								local numEdit = UIManager:FindUIEditBox("NumberOfTradingItem");
								if numEdit ~= nil then
									numEdit:SetText("1");
								end
								ResetDropItemPos();
								ConfirmTradingNum();
								do return end;	
							else
								UIManager:SetPanel2Top(tempPanel);
								countPanel:SetEnable(true);
								UIManager:SetKeyInItem("NumberOfTradingItem");
								UIManager:ClearMonoPanel();
						    	UIManager:SetMonoPanel(tempPanel);	
						    	UIManager:AddDirtyUIItem(countPanel);
					    	end
						end
					end
					ResetDropItemPos();
					do return end;
				end
			end
			
			--for item drag to warehouse panel
			local warehousePanel = UIManager:GetUIItem("WarehousePanel");
			if warehousePanel ~= nil then
				local wmx = warehousePanel:GetX();
				local wmy = warehousePanel:GetY();
				
				if ( mx >= wmx ) and ( mx <= (wmx + warehousePanel:GetWidth()) ) and
				   ( my >= wmy ) and ( my <= (wmy + warehousePanel:GetHeight()) ) then
				   
					local locationX = mx - wmx;
					local locationY = my - wmy;    
					
					if _G.DragItemPicIndex < 24 and locationY >= 63 and 
						locationY <= 390 and locationX >= 28 and locationX < 272 then
						
						local index =  LuaUI.Div( (locationX - 28), 41) + 6 *  LuaUI.Div( (locationY - 63), 41);
						
						local itemProcess = LuaUI.GetItemProcess();
						if itemProcess == nil then
							ResetDropItemPos();
							do return end;
						end
						
						local countPanel = UIManager:GetUIItem("WareHouseItemCountPanel");
						local tempPanel = UIManager:FindUIPanel("WareHouseItemCountPanel");
						local wareHouseProcess = LuaUI.GetWareHouseProcess();
						if countPanel ~= nil and tempPanel ~= nil and wareHouseProcess ~= nil then
							if wareHouseProcess:AvailSlot(index + 48 * _G.WareHousePageIndex) == false then
								if _G.WareHousePageIndex >= 2 then
									LuaUI.ShowErrMessage("MSG_WAREHOUSE_WRONG_ITEM_SLOT", true);
								else
									LuaUI.ShowErrMessage("MSG_WAREHOUSE_SLOT_NOT_OPEN", true);
								end
								ResetDropItemPos();
								do return end;
							end
							_G.GivenItemOrgIndex = _G.DragItemPicIndex;
							_G.PlacedItemIndex = index;
							local max = LuaUI.GetMaxStack(_G.DragItemPicIndex + 24 * _G.BagState);
							if wareHouseProcess:CheckCanSwap(_G.DragItemPicIndex + 24 * _G.BagState, index + 48 * _G.WareHousePageIndex) == true or max == 1 then
								local numEdit = UIManager:FindUIEditBox("NumberOfWareHouseItem");
								if numEdit ~= nil then
									numEdit:SetText(max.."");
								end
								ResetDropItemPos();
								ConfirmWareHouseNum();
								do return end;	
							else
								UIManager:SetPanel2Top(tempPanel);
								countPanel:SetEnable(true);
								UIManager:SetKeyInItem("NumberOfWareHouseItem");
								UIManager:ClearMonoPanel();
						    	UIManager:SetMonoPanel(tempPanel);	
						    	UIManager:AddDirtyUIItem(countPanel);
					    	end
						end
					end
					ResetDropItemPos();
					do return end;
				end
			end
			
			--For item drag to MixShopMake
			local mixPanel = UIManager:GetUIItem("MixShopMakePanel");
			
			if mixPanel ~= nil then
				local mmx = mixPanel:GetX();
				local mmy = mixPanel:GetY();
				
				if ( mx >= mmx ) and ( mx <= (mmx + mixPanel:GetWidth()) ) and
				   ( my >= mmy ) and ( my <= (mmy + mixPanel:GetHeight()) ) then
				   
					local locationX = mx - mmx;
					local locationY = my - mmy; 
					
					local mixProcess = LuaUI.GetMixProcess();
					if mixProcess ~= nil and mixProcess:HaveSelectedItemMix() ~= true and _G.DragItemPicIndex > 24  then
						ResetDropItemPos();
						do return end;
					end  
					
					local index = 4;
					
					if locationX > 13 and locationX < 53 then
						if locationY > 209 and locationY < 249 then
							index = 0;
						elseif locationY > 260 and locationY < 308 then
						   	index = 2;
						end
					elseif locationX > 168 and locationX < 207 then
					    if locationY > 209 and locationY < 249 then
							index = 1;
						elseif locationY > 260 and locationY < 308 then
						   	index = 3;
						end
					end
					
					local img = UIManager:GetUIItem("MixShopDragItemBlank0"..index);
					if img ~= nil and img:IsEnable() == false then
					   	ResetDropItemPos();
						do return end;
					end
					
					if mixProcess ~= nil and mixProcess:IsSuitableItemMix(index, _G.DragItemPicIndex + 24 * _G.BagState) ~= true then
						ResetDropItemPos();
						do return end;
					end
					
					
     				_G.GivenItemOrgIndex = _G.DragItemPicIndex;
					_G.PlacedItemIndex = index;
       				ConfirmMakeItemNum();

					ResetDropItemPos();
					do return end;
				end
			end
			
			--For item drag to MixShop
			local mixShopPanel = UIManager:GetUIItem("MixShopPanel");

			if mixShopPanel ~= nil then
				local mmx = mixShopPanel:GetX();
				local mmy = mixShopPanel:GetY();

				if ( mx >= mmx ) and ( mx <= (mmx + mixShopPanel:GetWidth()) ) and
				   ( my >= mmy ) and ( my <= (mmy + mixShopPanel:GetHeight()) ) then

					local locationX = mx - mmx;
					local locationY = my - mmy;

					local mixProcess = LuaUI.GetMixProcess();
					local index = 3;

					if locationX > 215 and locationX < 259 then
						if locationY > 62 and locationY < 106 then
							index = 0;
						elseif locationY > 110 and locationY < 154 then
						   	index = 1;
                        elseif locationY > 158 and locationY < 202 then
						   	index = 2;
						end
					end

					if mixProcess == nil or (_G.DragItemPicIndex > 24 or mixProcess:IsSuitableMixShopItem(index, _G.DragItemPicIndex + 24 * _G.BagState) ~= true) then
						ResetDropItemPos();
						do return end;
					end

					local countPanel = UIManager:GetUIItem("MixShopCountPanel");
					local tempPanel = UIManager:FindUIPanel("MixShopCountPanel");
					if countPanel ~= nil and tempPanel ~= nil then
	     				_G.GivenItemOrgIndex = _G.DragItemPicIndex;
						_G.PlacedItemIndex = index;
						
						if mixProcess:IsRepairShop() then
						    local numEdit = UIManager:FindUIEditBox("NumberOfMixShopSourceItem");
							if numEdit ~= nil then
								numEdit:SetText("1");
							end
       						ConfirmMixShopItemNum();
						elseif mixProcess:IsStoneMixShop() then
						    if mixProcess:HasResultStone(_G.DragItemPicIndex + 24 * _G.BagState, true) then
							    UIManager:SetPanel2Top(tempPanel);
								countPanel:SetEnable(true);
								UIManager:SetKeyInItem("NumberOfMixShopSourceItem");
								UIManager:ClearMonoPanel();
						    	UIManager:SetMonoPanel(tempPanel);
						    	UIManager:AddDirtyUIItem(countPanel);
					    	else
					    	    _G.GivenItemOrgIndex = -1;
								_G.PlacedItemIndex = -1;
					    	end
						elseif mixProcess:IsUnSoulBoundShop() then
						    local numEdit = UIManager:FindUIEditBox("NumberOfMixShopSourceItem");
							if numEdit ~= nil then
								numEdit:SetText("1");
							end
       						ConfirmMixShopSoulBoundItemNum()
						end
       				end
					ResetDropItemPos();
					do return end;
				end
			end
			
			local petUseFoodPanel = UIManager:GetUIItem("PetUseFoodPanel");
			
			if petUseFoodPanel ~= nil then
				local mmx = petUseFoodPanel:GetX();
				local mmy = petUseFoodPanel:GetY();

				if ( mx >= mmx ) and ( mx <= (mmx + petUseFoodPanel:GetWidth()) ) and
				   ( my >= mmy ) and ( my <= (mmy + petUseFoodPanel:GetHeight()) ) then

				    local locationX = mx - mmx;
					local locationY = my - mmy;

					local petProcess = LuaUI.GetMenuPetProcess();
					
					if petProcess then
						if locationY > 37 and locationY < 79 and locationX > 158 and locationX < 200 then
							if _G.DragItemPicIndex < 24 and
								petProcess:PetCanUseItem(_G.DragItemPicIndex + 24 * _G.BagState) then
									petProcess:SetUseFoodItem(_G.DragItemPicIndex + 24 * _G.BagState);
							end
						end
					end
					ResetDropItemPos();
					do return end;
				end
			
			end
			
			--for item drag to IronShop
			local IronShopPanel = UIManager:GetUIItem("IronShopMakePanel");
			
			if IronShopPanel ~= nil then
				local mmx = IronShopPanel:GetX();
				local mmy = IronShopPanel:GetY();

				if ( mx >= mmx ) and ( mx <= (mmx + IronShopPanel:GetWidth()) ) and
				   ( my >= mmy ) and ( my <= (mmy + IronShopPanel:GetHeight()) ) then

					local locationX = mx - mmx;
					local locationY = my - mmy;

					local mixProcess = LuaUI.GetMixProcess();
					local index = 2;

					if  mixProcess:IsIronShopMake() then
						if locationX > 235 and locationX <282 then
							if locationY > 129 and locationY < 176 then
								index = 0;
							elseif locationY > 210 and locationY < 257 then
								index = 1;
							end
						end
					elseif  mixProcess:IsIronShopRepair() then
						if locationX > 250 and locationX <297 then
							if locationY > 155 and locationY < 202 then
								index = 0;
							elseif locationY > 228 and locationY < 275 then
								index = 1;
							end
						end
					end
					
					if mixProcess == nil or mixProcess:IsSuitableIronShopItem(index, _G.DragItemPicIndex + 24 * _G.BagState) ~= true then
						ResetDropItemPos();
						do return end;
					end
					
					_G.GivenItemOrgIndex = _G.DragItemPicIndex;
					_G.PlacedItemIndex = index;				
					ConfirmIronShopItemNum();
					
					ResetDropItemPos();
					do return end;
				end
			end
			
			--For item drag to MixShopGem
			local mixShopGemPanel = UIManager:GetUIItem("MixShopGemPanel");
			
			if mixShopGemPanel ~= nil then
				local mmx = mixShopGemPanel:GetX();
				local mmy = mixShopGemPanel:GetY();

				if ( mx >= mmx ) and ( mx <= (mmx + mixShopGemPanel:GetWidth()) ) and
				   ( my >= mmy ) and ( my <= (mmy + mixShopGemPanel:GetHeight()) ) then

				   local locationX = mx - mmx;
					local locationY = my - mmy;

					local mixProcess = LuaUI.GetMixProcess();
					local index = 5;

					if mixProcess:IsGemMixShop() then
						if locationY > 190 and locationY < 232 then
							if locationX > 30 and locationX < 72 then
								index = 0;
							elseif locationX > 90 and locationX < 132 then
								index = 1;
							elseif locationX > 150 and locationX < 192 then
								index = 2;
							elseif locationX > 210 and locationX < 252 then
								index = 3;
							elseif locationX > 270 and locationX < 312 then
								index = 4;
							end
						end
					elseif mixProcess:IsGemInlayShop() then
						if locationX > 238 and locationX < 280 then
							if locationY > 59 and locationY < 101 then
								index = 0;
							elseif locationY > 129 and locationY < 171 then
								index = 1;
							elseif locationY > 199 and locationY < 241 then
								index = 2;
							end
						end
					elseif mixProcess:IsGemRemoveShop() then
						if locationX > 241 and locationX < 283 then
							if locationY > 61 and locationY < 103 then
								index = 0;
							elseif locationY > 181 and locationY < 223 then
								index = 1;
							end
						end
					elseif  mixProcess:IsGemMeltingShop() then
						if locationX > 251 and locationX <293 then
							if locationY > 170 and locationY < 212 then
								index = 0;
							end
						end
					elseif  mixProcess:IsGemPolishingShop() then
						if locationX > 236 and locationX <278 then
							if locationY > 97 and locationY < 139 then
								index = 0;
							elseif locationY > 151 and locationY < 193 then
								index = 1;
							end
						end
					elseif  mixProcess:IsColorStoneMixShop() then
						if locationX > 236 and locationX <278 then
							if locationY > 137 and locationY < 179 then
								index = 0;
							elseif locationY > 200 and locationY < 242 then
								index = 1;
							end
						end
					end

					if mixProcess == nil or (_G.DragItemPicIndex > 24 or mixProcess:IsSuitableGemMixShopItem(index, _G.DragItemPicIndex + 24 * _G.BagState) ~= true) then
						ResetDropItemPos();
						do return end;
					end
					
					_G.GivenItemOrgIndex = _G.DragItemPicIndex;
					_G.PlacedItemIndex = index;
					
					ConfirmGemMixShopItemNum();
					
					ResetDropItemPos();
					do return end;
				end
			end
			
			-- detect the range of drop is in the shop item range ?
			local shoppanel = UIManager:GetUIItem("ShopMenuPanel");
			if shoppanel ~= nil then

				local smx = shoppanel:GetX();
				local smy = shoppanel:GetY();
				if ( mx >= smx ) and ( mx <= (smx + shoppanel:GetWidth()) ) and
				   ( my >= smy ) and ( my <= (smy + shoppanel:GetHeight()) ) then
								
					-- Do Sell operation
					if _G.DragItemPicIndex < 24 then
						local itemslot = _G.DragItemPicIndex + 24 * _G.BagState;
						local itemProcess = LuaUI.GetItemProcess();
						if itemProcess ~= nil and itemProcess:IsItemSellable(itemslot) == true then	
							ShopEnterSellItemCount(itemslot);	
						else
							LuaUI.ShowErrMessage("MSG_SYS_ITEM_CANT_SELL");
						end

					end
					ResetDropItemPos();
					do return end;
				end
			end
			
		
			if _G.DragItemPicIndex < 24 then
				local mono = "";
				if LuaUI.GetMaxStack(_G.DragItemPicIndex + 24 * _G.BagState) == 1 then
					ShowConfirmDropItem();
				else
			        LuaUI.AddXMLFile2UIManger("Data\\DropItem.xml");
		
			        local item = UIManager:GetUIItem("MovingPanel");
				    item:SetNotDetected(true);
				    item:SetEnable(false);
				    
				    mono = UIManager:FindUIPanel("DropItemPanel");
				    if mono ~= nil then
					   	UIManager:ClearMonoPanel();
					    UIManager:SetMonoPanel(mono);
					end
			    end
			else
				ResetDropItemPos();				
		    end
		end
	end
end


function ItemMove(picIndex)
	if (_G.InBattle == false) and (_G.LockState == false) and (_G.UnlockState == false) then
		if LuaUI.CheckShiftDown() then
			local EditBox = UIManager:FindUIEditBox("EditLine")
			local itemSlot = 0;
			if EditBox ~= nil then
				if picIndex < 24 then
					itemSlot = picIndex + 24 * _G.BagState;
				else
					itemSlot = picIndex;
				end
				EditBox:ClearWaitText()
				EditBox:AddWaitText(string.format("\15#<\\[ITEM:%x:%x\\]\16\17[",
					--LuaUI.LuaGetItemColor(itemSlot), LuaUI.LuaGetBag(itemSlot), LuaUI.LuaGetChar_id()))
					LuaUI.LuaGetBag(itemSlot), LuaUI.LuaGetChar_id()))
				EditBox:AddWaitText(LuaUI.LuaGetItemName(itemSlot))
				--EditBox:AddWaitText(":")
				--EditBox:AddWaitText(LuaUI.LuaGetLineTextName())
				EditBox:AddWaitText("]\20\18")
				EditBox:SendWaitText()

				EditBox:Redraw();
				UIManager:RenderAll();
			end
			return
		end

		CloseItemDetail(0, 0);

	 	local itemboard = UIManager:GetUIItem("ItemBoard");
	 	itemboard:SetEnable(false);
		UIManager:AddDirtyUIItem(itemboard);

		local IconName = "";
		local StackName = "";
		local LevelName = "";

	 	if picIndex < 24 then
			local zero = "00";
			if picIndex >= 10 then
				zero = "0";
			end
			IconName = "ItemSlot"..zero..picIndex.."Icon";
			StackName = "ItemSlot"..zero..picIndex.."Stack";
			LevelName = "ItemSlot"..zero..picIndex.."Lvl";
		else
		    local subIndex = picIndex - 120;
	    	IconName = "ItemEquip00"..subIndex.."Icon";
			StackName = "ItemEquip00"..subIndex.."Stack";
			LevelName = "ItemEquip00"..subIndex.."Lvl";
		end

		local orgIcon = UIManager:GetUIItem(IconName);
		local orgStack = UIManager:GetUIItem(StackName);
		local orgLevel = UIManager:GetUIItem(LevelName);

		local itemProcess = LuaUI.GetItemProcess();
        local movingPanel = UIManager:GetUIItem("MovingPanel");
		if itemProcess ~= nil and movingPanel ~= nil and orgIcon ~= nil then
			local itemSlot = 0;
			if picIndex < 24 then
				itemSlot = picIndex + 24 * _G.BagState;
			else
				itemSlot = picIndex;
			end

			if itemProcess:IsItemGiven(itemSlot) ~= true then
				local orgAniIcon = UIManager:FindUIAniIcon(IconName);
				local targetAniIcon = UIManager:FindUIAniIcon("ItemMovingIcon");

				if orgAniIcon ~= nil and targetAniIcon ~= nil then
				    targetAniIcon:ClearAniData();
				    targetAniIcon:SetAnimationData(orgAniIcon);
				end
				
				UIManager:SetDragItemXY(movingPanel,orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());

				_G.DragItemPicIndex = picIndex;

				orgIcon:SetRenderStyle(1);
				_G.DragType = 0;
				g_ShorcutSelSlotType = 1

		  		movingPanel:SetEnable(true);
				UIManager:AddDirtyUIItem(movingPanel);
				PlayDragItemUpEffect();
			end
		end
	elseif (_G.InBattle == false) and (_G.LockState == true) then
         local itemSlot = 24 * _G.BagState + picIndex
         local itemProcess = LuaUI.GetItemProcess();
         if itemProcess ~= nil and _G.InBattle == false then
            if itemProcess:IsItemGiven(itemSlot) ~= true then
                LuaUI.LockItem(itemSlot, 0)
			end
		 end
		 ResetAllLockStatus();
	elseif (_G.InBattle == false) and (_G.UnlockState == true) then
         local itemSlot = 24 * _G.BagState + picIndex
         local itemProcess = LuaUI.GetItemProcess();
         if itemProcess ~= nil and _G.InBattle == false then
            if itemProcess:IsItemGiven(itemSlot) ~= true then
				LuaUI.UnlockItem(itemSlot, 0)
				_G.LockState = false
				_G.UnlockState = false
			end
		end
		ResetAllLockStatus();
	end
end



function DropItem()

	   if _G.DragItemPicIndex ~= -1 and _G.DragItemPicIndex < 120 then
	   		local numEdit = UIManager:FindUIEditBox("NumberOfDropItem");
	   		
	   		if numEdit ~= nil then
				local value = numEdit:GetInt();
				if value > 0  and value <= LuaUI.GetMaxStack(_G.DragItemPicIndex + 24 * _G.BagState) then
		      		local tempIndex = _G.DragItemPicIndex;
	      			local zero = "00";
					if _G.DragItemPicIndex >= 10 then
						zero = "0";
					end
					
		      		local itemIcon = UIManager:GetUIItem("ItemSlot"..zero..tempIndex.."Icon");
					local itemStack = UIManager:GetUIItem("ItemSlot"..zero..tempIndex.."Stack");
					local itemLevel = UIManager:GetUIItem("ItemSlot"..zero..tempIndex.."Lvl");
					
					itemIcon:SetEnable(false);
					itemStack:SetEnable(false);
					itemLevel:SetEnable(false);
					  
					  LuaUI.DropItem(_G.DragItemPicIndex + 24 * _G.BagState, value );
		      		
		      		CloseDropPanel();	
				else
		      		LuaUI.ShowErrMessage("MSG_SYS_ITEM_DROP_NUM_ERR");
		      	end
		    else
				local tempIndex = _G.DragItemPicIndex;
		      	local zero = "00";
				if _G.DragItemPicIndex >= 10 then
					zero = "0";
				end 
		    
		    	local itemIcon = UIManager:GetUIItem("ItemSlot"..zero..tempIndex.."Icon");
				local itemStack = UIManager:GetUIItem("ItemSlot"..zero..tempIndex.."Stack");
				local itemLevel = UIManager:GetUIItem("ItemSlot"..zero..tempIndex.."Lvl");
					
				itemIcon:SetEnable(false);
				itemStack:SetEnable(false);
				itemLevel:SetEnable(false);
				
		    	LuaUI.DropItem(_G.DragItemPicIndex + 24 * _G.BagState, 1 );
		      	
		      	CloseDropPanel();
		      	
		    end
	   end
	   

end

function CloseDropPanel()
	ResetKeyIn();
    
    UIManager:DeletePanel("DropItemPanel");
    UIManager:DeletePanel("ConfirmDropItemPanel");
    UIManager:ClearMonoPanel();
    ResetDropItemPos();
end

function ShowConfirmDropItem()
		
		LuaUI.AddXMLFile2UIManger("Data\\ConfirmDropItem.xml");			
		local confirmPanel = UIManager:FindUIPanel("ConfirmDropItemPanel");
		LuaUI.SetDropItemText(_G.DragItemPicIndex + 24 * _G.BagState, "ConfirmDropText");
		
		if confirmPanel ~= nil then	
			
		   	UIManager:ClearMonoPanel();
		   	
		   	local numPanel = UIManager:GetUIItem("DropItemPanel");
		   	if numPanel ~= nil then
		   	    numPanel:SetEnable(false);
			end
		   	
		    UIManager:SetMonoPanel(confirmPanel);
		end
		
end

function CloseConfirmDropPanel()
	UIManager:DeletePanel("ConfirmDropItemPanel");
	local panel = UIManager:FindUIPanel("DropItemPanel");
	local panel2 = UIManager:GetUIItem("DropItemPanel");
	UIManager:ClearMonoPanel();
	
	if panel ~= nil and panel2 ~= nil then
		panel2:SetEnable(true);
		UIManager:SetMonoPanel(panel);			
	else
    	ResetDropItemPos();
	end

end

function ResetDropItemPos()

	local movingPanel = UIManager:GetUIItem("MovingPanel");
	
	if movingPanel ~= nil then
		movingPanel:SetPos(0,0);
		movingPanel:SetEnable(false);
		movingPanel:SetNotDetected(false);
	end
	
	if _G.DragType == 0 then
		local IconName = "";
		local StackName = "";
		local LevelName = "";

		if _G.DragItemPicIndex < 24 and _G.DragItemPicIndex >= 0 then
			local zero = "00";
			if _G.DragItemPicIndex >= 10 then
				zero = "0";
			end
			IconName = "ItemSlot"..zero.._G.DragItemPicIndex.."Icon";
			StackName = "ItemSlot"..zero.._G.DragItemPicIndex.."Stack";
			LevelName = "ItemSlot"..zero.._G.DragItemPicIndex.."Lvl";
		else
		    local subIndex = _G.DragItemPicIndex - 120;
		    IconName = "ItemEquip00"..subIndex.."Icon";
			StackName = "ItemEquip00"..subIndex.."Stack";
			LevelName = "ItemEquip00"..subIndex.."Lvl";
		end

		local itemIcon = UIManager:GetUIItem(IconName);
		local itemStack = UIManager:GetUIItem(StackName);
		local itemLevel = UIManager:GetUIItem(LevelName);

		if itemIcon ~= nil and itemStack ~= nil and itemLevel ~= nil then
			local itemProcess = LuaUI.GetItemProcess();
			if itemProcess ~= nil then
				if itemProcess:IsItemGiven(_G.DragItemPicIndex + 24 * _G.BagState) then
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


function EquipItemMove000()
   ItemMove(120);
end

function EquipItemMove001()
   ItemMove(121);
end

function EquipItemMove002()
   ItemMove(122);
end

function EquipItemMove003()
   ItemMove(123);
end

function EquipItemMove004()
   ItemMove(124);
end

function EquipItemMove005()
   ItemMove(125);
end

function EquipItemMove006()
   ItemMove(126);
end

function EquipItemMove007()
   ItemMove(127);
end

function EquipItemMove008()
   ItemMove(128);
end


function EquipItemMove()

		local panel = UIManager:GetUIItem("ItemMenuPanel");
		local X =  UIManager:GetCursorX() - panel:GetX();
		local Y =  UIManager:GetCursorY() - panel:GetY();
		
		if X > _G.MenuItemStartX[1] and X < (_G.MenuItemStartX[6] + 40)
		    and Y > _G.MenuItemStartY[1] and Y < (_G.MenuItemStartY[4] + 40)
			then
			
			X = X - _G.MenuItemStartX[1];
			Y = Y - _G.MenuItemStartY[1];
			
			local bagIndex = LuaUI.Div(X, 41) + 6 * LuaUI.Div(Y, 41)
			
			local subIndex = _G.DragItemPicIndex - 120;
		    
			local item = UIManager:GetUIItem("ItemEquip00"..subIndex.."Icon");
			item:SetRenderStyle(0);
			item:SetEnable(false);
			LuaUI.MoveEquipToBag(_G.DragItemPicIndex , bagIndex + _G.BagState * 24);
			
 		else
	    	ResetDropItemPos();
		end
end

function ResetBagButtonState()
    local Button = UIManager:FindUIButton("ItemCurrentBag");
	if Button ~= nil then
	    Button:SetLockState(false);
		Button:SetNormalState();
	end

	Button = UIManager:FindUIButton("ItemSideBag");
	if Button ~= nil then
	    Button:SetLockState(false);
		Button:SetNormalState();
	end

	Button = UIManager:FindUIButton("ItemBagExtra01");
	if Button ~= nil then
	    Button:SetLockState(false);
		Button:SetNormalState();
	end

	Button = UIManager:FindUIButton("ItemBagExtra02");
	if Button ~= nil then
	    Button:SetLockState(false);
		Button:SetNormalState();
	end

	Button = UIManager:FindUIButton("ItemBagExtra03");
	if Button ~= nil then
	    Button:SetLockState(false);
		Button:SetNormalState();
	end
end

function ResetButtonState()


	
	local Button = UIManager:FindUIButton("ItemCharacter");
	if Button ~= nil then
	    Button:SetLockState(false);
		Button:SetNormalState();
	end
	
	Button = UIManager:FindUIButton("ItemPartyMember");
	if Button ~= nil then
	    Button:SetLockState(false);
		Button:SetNormalState();
	end
	
	Button = UIManager:FindUIButton("ItemPet");
	if Button ~= nil then
	    Button:SetLockState(false);
		Button:SetNormalState();
	end

end

function ItemOver000()
	ItemOver(0);
end

function ItemOver001()
	ItemOver(1);
end

function ItemOver002()
	ItemOver(2);
end

function ItemOver003()
	ItemOver(3);
end

function ItemOver004()
	ItemOver(4);
end

function ItemOver005()
	ItemOver(5);
end

function ItemOver006()
	ItemOver(6);
end

function ItemOver007()
	ItemOver(7);
end

function ItemOver008()
	ItemOver(8);
end

function ItemOver009()
	ItemOver(9);
end

function ItemOver010()
	ItemOver(10);
end

function ItemOver011()
	ItemOver(11);
end

function ItemOver012()
	ItemOver(12);
end

function ItemOver013()
	ItemOver(13);
end

function ItemOver014()
	ItemOver(14);
end

function ItemOver015()
	ItemOver(15);
end

function ItemOver016()
	ItemOver(16);
end

function ItemOver017()
	ItemOver(17);
end

function ItemOver018()
	ItemOver(18);
end

function ItemOver019()
	ItemOver(19);
end

function ItemOver020()
	ItemOver(20);
end

function ItemOver021()
	ItemOver(21);
end

function ItemOver022()
	ItemOver(22);
end

function ItemOver023()
	ItemOver(23);
end

function ItemEquipOver000()
	ItemOver(120);
end

function ItemEquipOver001()
	ItemOver(121);
end

function ItemEquipOver002()
	ItemOver(122);
end

function ItemEquipOver003()
	ItemOver(123);
end

function ItemEquipOver004()
	ItemOver(124);
end

function ItemEquipOver005()
	ItemOver(125);
end

function ItemEquipOver006()
	ItemOver(126);
end

function ItemEquipOver007()
	ItemOver(127);
end

function ItemEquipOver008()
	ItemOver(128);
end

function ItemOver(itemNo)
		
	local itemName = "";
	if itemNo < 24 then
		if itemNo < 10 then
			itemName = "ItemSlot00"..itemNo;
		elseif itemNo >= 10 then
			itemName = "ItemSlot0"..itemNo;
		end
	else
	    local subIndex = itemNo - 120;
        itemName = "ItemEquip00"..subIndex;
    end

		itemName = itemName.."Icon";
		
		local itemboard = UIManager:GetUIItem("ItemBoard");
		itemboard:SetEnable(true);
		UIManager:AddDirtyUIItem(itemboard);
	
		local item = UIManager:GetUIItem(itemName);
    
		if itemboard:GetOffsetX() ~= (item:GetOffsetX() - 1) or itemboard:GetOffsetY() ~= (item:GetOffsetY() - 1) then
   			itemboard:SetOffsetXY(item:GetOffsetX() - 1 , item:GetOffsetY() - 1 );
			UIManager:AddDirtyUIItem(itemboard);
		end
	

	if itemNo < 24 then
		_G.ItemIndexNow = itemNo + 24 * _G.BagState;
 	else
 	    _G.ItemIndexNow = itemNo;
	end
	
	if PanelItemDetailState == 1 then
		ItemDetail(1, _G.ItemIndexNow);  
	end
end

function ResetDrag()
 	UIManager:ResetDragItem();
	UIManager:StopDrag();

 	ResetDropItemPos();
	--UIManager:RenderAll();

end

function AddOne()
	local numEdit = UIManager:FindUIEditBox("NumberOfDropItem");
	local temp = UIManager:GetUIItem("NumberOfDropItem");
	
	local value = numEdit:GetInt();
	
	if value >= 0 and value < LuaUI.GetMaxStack(_G.DragItemPicIndex + 24 * _G.BagState) then
		value = value + 1;
	end
	
	numEdit:SetText(value.."");--value.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);--RenderAll();

end

function DiffOne()

	local numEdit = UIManager:FindUIEditBox("NumberOfDropItem");
	local temp = UIManager:GetUIItem("NumberOfDropItem");
	
	local value = numEdit:GetInt();
	
	if value > 1 then
		value = value - 1;
	end
	
	numEdit:SetText(value.."");--value.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function SetZero()
	local numEdit = UIManager:FindUIEditBox("NumberOfDropItem");
	local temp = UIManager:GetUIItem("NumberOfDropItem");
	numEdit:SetText("1");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
 end

function MaxStack()

	local numEdit = UIManager:FindUIEditBox("NumberOfDropItem");
	local temp = UIManager:GetUIItem("NumberOfDropItem");
	local max = LuaUI.GetMaxStack(_G.DragItemPicIndex + 24 * _G.BagState);
	
	numEdit:SetText(max.."");
	numEdit:Redraw();
	UIManager:AddDirtyUIItem(temp);
end

function ShowItemBoard()
	local panel = UIManager:GetUIItem("ItemBoard");
	local itempanel = UIManager:GetUIItem("ItemMenuPanel");
	if panel ~= nil and itempanel ~= nil then
		if (UIManager:GetCursorX() > (panel:GetOffsetX() + itempanel:GetX() ) and UIManager:GetCursorX() < ( panel:GetOffsetX() + 40 + itempanel:GetX() )
			and UIManager:GetCursorY() > (panel:GetOffsetY() + itempanel:GetY() ) and UIManager:GetCursorY() < (panel:GetOffsetY() + 40 + itempanel:GetY() )) then
			--panel:SetEnable(true);
			if PanelItemDetailState == 0 then
				ItemDetail(1, _G.ItemIndexNow);
				PanelItemDetailState = 1;
			end
		else
		    panel:SetEnable(false);
		    PanelItemDetailState = 0;
		end
	
	end
end

function SelectItemPartnerShift(Index)
	if LuaUI.CheckShiftDown() then
		local EditBox = UIManager:FindUIEditBox("EditLine")	
		
		local itemProcess = LuaUI.GetItemProcess();
		if itemProcess ~= nil then
			if EditBox ~= nil then
				EditBox:ClearWaitText()
				if _G.WindowState == 1 then
					PartnerAddLink(itemProcess:GetMenuSelectedPartnerIndex())
				elseif _G.WindowState == 2 then
					PetAddLink(itemProcess:GetMenuSelectedPetIndex())
				end
			end
		end
	end
end

function LockOrUnLockPartnerPet(index, type)
	local GivenPanel = UIManager:GetUIItem("GivenItemPanel");
	local TradePanel = UIManager:GetUIItem("TradingPanel");
	local HawkPanel1 = UIManager:GetUIItem("HawkerOwnerPanel");
	local HawkPanel2 = UIManager:GetUIItem("HawkPanel");
	if GivenPanel == nil and TradePanel == nil and HawkPanel1 == nil and  HawkPanel2 == nil then
		local itemProcess = LuaUI.GetItemProcess();
		if itemProcess ~= nil and _G.InBattle == false then
			local bGive = false
	    	if type == 1 then
				if itemProcess:IsGivenPartner() then
					bGive = true
				end
			elseif type == 2 then
				if itemProcess:IsGivenPet() then
					bGive = true
				end
			end
			if _G.LockState and (bGive == false)then
       			LuaUI.LockItem(index, type)
			elseif _G.UnlockState and (bGive == false) then
       			LuaUI.UnlockItem(index, type)
			end
		end
	end
	ResetAllLockStatus();
end

function SelectItemPartner1()
	LuaUI.SelectItemPartner(0);
	SelectItemPartnerShift(0);
    LockOrUnLockPartnerPet(0,_G.WindowState);
			 
end

function SelectItemPartner2()
	LuaUI.SelectItemPartner(1);
	SelectItemPartnerShift(1);
	LockOrUnLockPartnerPet(1,_G.WindowState);	
end

function SelectItemPartner3()
	LuaUI.SelectItemPartner(2);
	SelectItemPartnerShift(2);
	LockOrUnLockPartnerPet(2,_G.WindowState);	
end

function SelectItemPartner4()
	LuaUI.SelectItemPartner(3);
	SelectItemPartnerShift(3);
	LockOrUnLockPartnerPet(3,_G.WindowState);
end

function SelectItemPartner5()
	LuaUI.SelectItemPartner(4);
	SelectItemPartnerShift(4);
	LockOrUnLockPartnerPet(4,_G.WindowState);	
end

function SelectItemPartner6()
	LuaUI.SelectItemPartner(5);
	SelectItemPartnerShift(5);
	LockOrUnLockPartnerPet(5,_G.WindowState);	
end

function ItemNameListScrollUp()
	LuaUI.ItemScroll(0);
end

function ItemNameListScrollDown()
    LuaUI.ItemScroll(1);
end

function ItemNameListScroll()
	LuaUI.ItemScroll(2);
end


function SelectedPPDrag()

    local givenPanel = UIManager:GetUIItem("GivenItemPanel");
    local tradingPanel = UIManager:GetUIItem("TradingPanel");
    local wareHousePanel = UIManager:GetUIItem("WarehousePanel");
	local partnerCultivationUpPanel = UIManager:GetUIItem("PartnerCultivationUpPanel");
	if givenPanel ~= nil or tradingPanel ~= nil or wareHousePanel ~= nil or partnerCultivationUpPanel ~= nil then
		local itemProcess = LuaUI.GetItemProcess();
		local movingPanel = UIManager:GetUIItem("MovingPanel");
		if itemProcess ~= nil and movingPanel ~= nil then
		    
			if _G.WindowState == 1 then
			    if  itemProcess:IsGivenPartner() ~= true then
			        movingPanel:SetEnable(true);
					UIManager:SetDragItemXY(movingPanel,-20,-20);
					itemProcess:DragPartner();
					_G.DragType = 1;
	   			end
	   		elseif _G.WindowState == 2 then
	   		    if  itemProcess:IsGivenPet() ~= true then
			        movingPanel:SetEnable(true);
					UIManager:SetDragItemXY(movingPanel,-20,-20);
					itemProcess:DragPet();
					_G.DragType = 2;
	   			end
			end
		end
	end
end


function SelectedPPDrag1()
	SelectedPPDrag()
	SelectItemPartnerShift(0)
	LockOrUnLockPartnerPet(0, _G.WindowState)
end

function SelectedPPDrag2()
	SelectedPPDrag()
	SelectItemPartnerShift(1)
	LockOrUnLockPartnerPet(1, _G.WindowState)	
end

function SelectedPPDrag3()
	SelectedPPDrag()
	SelectItemPartnerShift(2)
	LockOrUnLockPartnerPet(2, _G.WindowState)	
end

function SelectedPPDrag4()
	SelectedPPDrag()
	SelectItemPartnerShift(3)
	LockOrUnLockPartnerPet(3, _G.WindowState)	
end

function ItemMenuPanelWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("ItemNameListScroll"):OnMouseWheel(UIManager:GetUIItem("ItemNameListScroll"):GetX()
	,UIManager:GetUIItem("ItemNameListScroll"):GetY(),pZ);
end

function SelectedPPDrag5()
	SelectedPPDrag()
	SelectItemPartnerShift(4)
	LockOrUnLockPartnerPet(4, _G.WindowState)	
end

function SelectedPPDrag6()
	SelectedPPDrag()
	SelectItemPartnerShift(5)
	LockOrUnLockPartnerPet(5, _G.WindowState)	
end

function IdentifyItemOK()
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
		itemProcess:IdentifyItem();
	end
	CloseIdentifyItemPanel();
end

function CloseIdentifyItemPanel()
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
		itemProcess:ResetIdentify();
	end
	UIManager:ClearMonoPanel();
	UIManager:DeletePanel("IdentifyItemPanel");
end

function ItemLock()
	local GivenPanel = UIManager:GetUIItem("GivenItemPanel");
	local TradePanel = UIManager:GetUIItem("TradingPanel");
	local HawkPanel1 = UIManager:GetUIItem("HawkerOwnerPanel");
	local HawkPanel2 = UIManager:GetUIItem("HawkPanel");
      
	if GivenPanel == nil and TradePanel == nil and HawkPanel1 == nil and  HawkPanel2 == nil and _G.InBattle == false then	
		_G.UnlockState = false
		if _G.LockState then
			ResetAllLockStatus();
		else
			_G.LockState = true
			LuaUI.SetLockCursor(true);
			SetYuanBaoBankMoneyLock(true);
			local itemPanel = UIManager:FindUIPanel("ItemMenuPanel");
			UIManager:SetMonoPanel(itemPanel);
		end
	else
		ResetAllLockStatus();
	end
end

function ItemUnLock()
	local GivenPanel = UIManager:GetUIItem("GivenItemPanel");
	local TradePanel = UIManager:GetUIItem("TradingPanel");
	local HawkPanel1 = UIManager:GetUIItem("HawkerOwnerPanel");
	local HawkPanel2 = UIManager:GetUIItem("HawkPanel");
      
	if GivenPanel == nil and TradePanel == nil and HawkPanel1 == nil and  HawkPanel2 == nil and _G.InBattle == false then
		_G.LockState = false
		if _G.UnlockState then
			ResetAllLockStatus();
		else
	    	_G.UnlockState = true
	    	LuaUI.SetUnLockCursor(true);	
			SetYuanBaoBankMoneyLock(true);
			local itemPanel = UIManager:FindUIPanel("ItemMenuPanel");
			UIManager:SetMonoPanel(itemPanel);
		end
	else
		ResetAllLockStatus();
	end	
end

function SetYuanBaoBankMoneyLock(turnOn)
    local yuanbaoLock = UIManager:GetUIItem("YuanBaoLock");
	local bankmoneyLock = UIManager:GetUIItem("BankMoneyLock");
	
	if yuanbaoLock and bankmoneyLock and _G.WindowState == 0 then
		yuanbaoLock:SetEnable(turnOn);
	    bankmoneyLock:SetEnable(turnOn);
	end
end


function SelectYuanBaoLock()
	if (_G.InBattle == false) and (_G.LockState == true) then
         LuaUI.LockItem(0, 4)
	elseif (_G.InBattle == false) and (_G.UnlockState == true) then
	    LuaUI.UnlockItem(0, 4)
	end
	ResetAllLockStatus();
end

function SelectBankMoneyLock()
	if (_G.InBattle == false) and (_G.LockState == true) then
         LuaUI.LockItem(0, 3)
	elseif (_G.InBattle == false) and (_G.UnlockState == true) then
	    LuaUI.UnlockItem(0, 3)
	end
	ResetAllLockStatus();
end

function ResetAllLockStatus()
	 _G.LockState = false
	 _G.UnlockState = false
	 LuaUI.SetLockCursor(false);
	 LuaUI.SetUnLockCursor(false);	
	 SetYuanBaoBankMoneyLock(false);
	 UIManager:ClearMonoPanel();
end

function JpackItem()
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
		itemProcess:JpackBag(BagState);
	end
end