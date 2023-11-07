PartnerSkillTimeOutFlag = 0;
PartnerCollectionState = 1;
PartnerCollectionTimeOutFlag = 0;

function OpenPartner()
    local panel = UIManager:GetUIItem("PartnerMainPanel");

	if panel ~= nil then
	    ClosePartner();
	else
		local partnerCollectionPanel = UIManager:GetUIItem("PartnerCollectionPanel");
		if partnerCollectionPanel == nil then
			LuaUI.AddXMLFile2UIManger("Data\\MenuPartnerCollection.xml");
			PartnerCollectionRace1();
			SwitchOffPartnerCollection()
		end
		LuaUI.AddXMLFile2UIManger("Data\\MenuPartner.xml");
		local partnerSkillPanel = UIManager:GetUIItem("PartnerSkillPanel");
		if partnerSkillPanel == nil then
			LuaUI.AddXMLFile2UIManger("Data\\MenuPartnerSkill.xml");
		end
		
		local partnerMenu = LuaUI.GetPartnerProcess();
		if partnerMenu ~= nil then 
			partnerMenu:ResetValue();
			partnerMenu:UpdatePartnerMenu();
		end
		--LuaUI.UpdatePartnerPanel();
		PlayOpenInterfaceEffect();
	end
	
	local PartnerFaceSparkUI = UIManager:FindUIAniIcon("PartnerFaceSpark");
	local MyPartnerFightSparkUi = UIManager:FindUIAniIcon("MyPartnerFightSpark");
	if PartnerFaceSparkUI ~= nil and MyPartnerFightSparkUi ~= nil then
		if PartnerFaceSparkUI:IsEnable() and PartnerFaceSparkUI:GetHasAni() then	
				MyPartnerFightSparkUi:SetEnable(true);
		else
			MyPartnerFightSparkUi:SetEnable(false);
		end
	end

end

function ClosePartner()
	UIManager:StopDrag();	
    ClosePartnerMain();
    ClosePartnerSkill();
    ClosePartnerDesc();
    CloseChangePartnerName();
	ClosePartnerCollection();
    local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:ResetValue();
	end
    --LuaUI.ResetPartnerValue();
    
end

function ClosePartnerMain()
	local panel = UIManager:GetUIItem("PartnerMainPanel");
	
	if panel ~= nil then
		PlayCloseInterfaceEffect();
	    UIManager:DeletePanel("PartnerMainPanel");
	end
end

function ClosePartnerSkill()
	local panel = UIManager:GetUIItem("PartnerSkillPanel");
	

	if panel ~= nil then
		SwitchOffPartnerSkill();	
	    UIManager:DeletePanel("PartnerSkillPanel");
	end
end

function ClosePartnerCollection()
	local panel = UIManager:GetUIItem("PartnerCollectionPanel");

	if panel ~= nil then
	    UIManager:DeletePanel("PartnerCollectionPanel");
	end
	ResetPartnerCollectionDrag()
end

function ClosePartnerBattleSkill()
	local panel = UIManager:GetUIItem("PartnerBattleSkillPanel");

	if panel ~= nil then
	    UIManager:DeletePanel("PartnerBattleSkillPanel");
	end
	
	panel = UIManager:GetUIItem("PartnerBattleSkillShortCut");
	if panel ~= nil then
		LuaUI.LuaSetBPShortCutFlag(false)		
		panel:SetEnable(false)
		UIManager:DeletePanel("PartnerBattleSkillShortCut");
	end

	
	
	CloseSkillDetail(0, 0);
	
	if LuaUI.IsBattlePartnerTurn() then
		ShowBattlePartnerPanel();	
	end
		
	UIManager:ClearMonoPanel();	
	
	--[[
	if g_CurrentShortCut == 0 then
		local HShortCutPanel = UIManager:FindUIPanel("HShortCutPanel"); 
		HShortCutPanel:SetEnable(true)
	elseif g_CurrentShortCut == 1 then
		local VShortCutPanel = UIManager:FindUIPanel("VShortCutPanel"); 
		VShortCutPanel:SetEnable(true)
	elseif g_CurrentShortCut == 2 then
		local HideShortCutPanel = UIManager:FindUIPanel("HideShortCutPanel"); 
		HideShortCutPanel:SetEnable(true)
	end
	--]]
	
end


function ClosePartnerDesc()
	local panel = UIManager:GetUIItem("PartnerDescPanel");

	if panel ~= nil then
	    UIManager:DeletePanel("PartnerDescPanel");
	end
end


function ClosePartnerDetail()
	local panel = UIManager:GetUIItem("PartnerDetailPanel");
	if panel ~= nil then 
		UIManager:DeletePanel("PartnerDetailPanel");
	end
end

function OpenPartnerDesc()
    local panel = UIManager:GetUIItem("PartnerDescPanel");
    local partnerPanel = UIManager:GetUIItem("PartnerMainPanel");

	if panel ~= nil and partnerPanel ~= nil then
		panel:SetPos( partnerPanel:GetX() + 345, partnerPanel:GetY());
     	panel:SetEnable(true);
	end
end

function OpenPartnerSkill()
    local panel = UIManager:GetUIItem("PartnerSkillPanel");
    local partnerPanel = UIManager:GetUIItem("PartnerMainPanel");
    
	if panel ~= nil and partnerPanel ~= nil then
        panel:SetPos( partnerPanel:GetX() + 345, partnerPanel:GetY() + 210);
     	panel:SetEnable(true);
	end
end

function OpenPartnerCollection()
    local panel = UIManager:GetUIItem("PartnerCollectionPanel");
    local partnerPanel = UIManager:GetUIItem("PartnerMainPanel");
	local tradingPanel = UIManager:GetUIItem("TradingPanel");
	local hawkerOwnerPanel = UIManager:GetUIItem("HawkerOwnerPanel");
	if tradingPanel ~= nil or hawkerOwnerPanel ~= nil then
		return
	end
    
	if panel ~= nil and partnerPanel ~= nil then
		local Menu = LuaUI.GetMenuHelpProcess();
		Menu:HandleEvent(36);
        panel:SetPos( partnerPanel:GetX() + 345, partnerPanel:GetY());
     	panel:SetEnable(true);
	end
end

function SwitchOffPartnerDesc()
    local panel = UIManager:GetUIItem("PartnerDescPanel");

	if panel ~= nil then
     panel:SetEnable(false);
	end
end

function SwitchOffPartnerSkill()
    local panel = UIManager:GetUIItem("PartnerSkillPanel");
    CloseSkillDetail(0, 0);
		
	UIManager:ClearMonoPanel();	
		
	if panel ~= nil then
     	panel:SetEnable(false);
	end
end

function SwitchOffPartnerCollection()
    local panel = UIManager:GetUIItem("PartnerCollectionPanel");
		
	UIManager:ClearMonoPanel();	
		
	if panel ~= nil then
     	panel:SetEnable(false);
	end
	SwitchOffPartnerDesc()
	
	ResetPartnerCollectionDrag()
end

function OpenPartnerSkillInt()
    local panel = UIManager:GetUIItem("PartnerSkillPanel");
    local panel2 = UIManager:GetUIItem("PartnerDescPanel");
	local panel3 = UIManager:GetUIItem("PartnerCollectionPanel");
	
	if panel3 ~= nil and panel3:IsEnable() then
		SwitchOffPartnerCollection();
	end
    
    if panel ~= nil and panel2 ~= nil then
        if panel:IsEnable() or panel2:IsEnable() then
            SwitchOffPartnerSkill();
            SwitchOffPartnerDesc();
        else
			local partnerMenu = LuaUI.GetPartnerProcess();
			if partnerMenu ~= nil then 
				local word
				for i=1, 4 do
					word = "SelectPartner"..i;
					panel = UIManager:GetUIItem(word);
					if panel ~= nil and panel:IsEnable() then
						partnerMenu:SelectPartner(i);
						break;
					end
				end
			end
	    	OpenPartnerSkill();
	    	OpenPartnerDesc();
	    	PlayOpenInterfaceEffect();
		end
    end
end

function OpenPartnerCollectionInt()
	
    local panel = UIManager:GetUIItem("PartnerSkillPanel");
    local panel2 = UIManager:GetUIItem("PartnerDescPanel");
    local panel3 = UIManager:GetUIItem("PartnerCollectionPanel");
	
	if panel ~= nil and panel:IsEnable() then
		SwitchOffPartnerSkill();
	end
	
	if panel2 ~= nil and panel2:IsEnable() then
		SwitchOffPartnerDesc();
	end
    
	if panel3 ~= nil then
		if panel3:IsEnable() then
			SwitchOffPartnerCollection();
		else
			OpenPartnerCollection();
	    	PlayOpenInterfaceEffect();
		end
	end
end

--[[function DeletePartner()
	LuaUI.DeleteOnShowPartner();
end ]]

function PartnerSTRAdd()
	--LuaUI.PartnerAttrUp(0);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:AttrUp(0);
	end
	PlayPointAddEffect();
end

function PartnerCONAdd()
	--LuaUI.PartnerAttrUp(1);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:AttrUp(1);
	end
	PlayPointAddEffect();
end

function PartnerSTAAdd()
	--LuaUI.PartnerAttrUp(2);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:AttrUp(2);
	end
	PlayPointAddEffect();
end

function PartnerAGIAdd()
	--LuaUI.PartnerAttrUp(3);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:AttrUp(3);
	end
	PlayPointAddEffect();
end

function PartnerSPRAdd()
	--LuaUI.PartnerAttrUp(4);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:AttrUp(4);
	end
	PlayPointAddEffect();
end

function PartnerSTRSub()
	--LuaUI.PartnerAttrDown(0);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:AttrDown(0);
	end
	PlayPointDecreaseEffect();
end

function PartnerCONSub()
	--LuaUI.PartnerAttrDown(1);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:AttrDown(1);
	end
	PlayPointDecreaseEffect();
end

function PartnerSTASub()
	--LuaUI.PartnerAttrDown(2);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:AttrDown(2);
	end
	PlayPointDecreaseEffect();
end

function PartnerAGISub()
	--LuaUI.PartnerAttrDown(3);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:AttrDown(3);
	end
	PlayPointDecreaseEffect();
end

function PartnerSPRSub()
	--LuaUI.PartnerAttrDown(4);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:AttrDown(4);
	end
	PlayPointDecreaseEffect();
end

function PartnerAddConfirm()
	--LuaUI.PartnerAddConfirm();
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:AddConfirm();
	end
	PlayConfirmPointEffect();
end

function PartnerAddLink(Index)
	if LuaUI.CheckShiftDown() then
		local EditBox = UIManager:FindUIEditBox("EditLine")	
		if EditBox ~= nil then
			EditBox:ClearWaitText()
			EditBox:AddWaitText(string.format("\15#<\\[PTNR:%x:%x\\]\16\17[", 
				LuaUI.LuaGetPartnerID(Index), LuaUI.LuaGetChar_id()))
			EditBox:AddWaitText(LuaUI.LuaGetPartnerOriginName(Index))
			--EditBox:AddWaitText(":")
			--EditBox:AddWaitText(LuaUI.LuaGetLineTextName())
			EditBox:AddWaitText("]\20\18")
			EditBox:SendWaitText()
			
			EditBox:Redraw();
			UIManager:RenderAll();
		end
	end
end

function SelectPartner1()
	--LuaUI.SelectPartner(1);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:SelectPartner(1);
		PartnerAddLink(partnerMenu:GetSelectIndex())
	end
	SelectPartner()
end

function SelectPartner2()
    --LuaUI.SelectPartner(2);
    local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:SelectPartner(2);
		PartnerAddLink(partnerMenu:GetSelectIndex())
	end
	SelectPartner()
end

function SelectPartner3()
	--LuaUI.SelectPartner(3);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:SelectPartner(3);
		PartnerAddLink(partnerMenu:GetSelectIndex())
	end
	SelectPartner()
end

function SelectPartner4()
	--LuaUI.SelectPartner(4);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:SelectPartner(4);
		PartnerAddLink(partnerMenu:GetSelectIndex())
	end
	SelectPartner()
end


function OpenChangePartnerName()
    local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil and partnerMenu:HasPartner() then
		local panel = UIManager:GetUIItem("PartnerRenamePanel");
		local partnerPanel = UIManager:GetUIItem("PartnerMainPanel");

		if panel == nil and partnerPanel ~= nil then
			LuaUI.AddXMLFile2UIManger("Data\\PartnerRename.xml");
			panel = UIManager:GetUIItem("PartnerRenamePanel");
			if panel ~= nil then
			    panel:SetPos(partnerPanel:GetX() + 70, partnerPanel:GetY() + 200);
			end
		end
	end
end

function CloseChangePartnerName()
	ResetKeyIn();
    UIManager:DeletePanel("PartnerRenamePanel");
end

function ChanegPartnerName()
	local partnerRenameText = UIManager:FindUIEditBox("PartnerNewName");
	
	if partnerRenameText ~= nil then
		if partnerRenameText:Empty() ~= true then
			local partnerMenu = LuaUI.GetPartnerProcess();
			if partnerMenu ~= nil then 
				partnerMenu:ChangePartnerName();
			end
			--LuaUI.ChangePartnerName();
			CloseChangePartnerName();
		end
	end
end

function SetPartnerStandBy()
	--LuaUI.SetPartnerState(0);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:SetPartnerState(0);
	end
end

function SetPartnerFight()
	--LuaUI.SetPartnerState(1);
	local MyPartnerFightSparkUi = UIManager:FindUIAniIcon("MyPartnerFightSpark");
	if MyPartnerFightSparkUi ~= nil then
		if MyPartnerFightSparkUi:IsEnable()  then	
			MyPartnerFightSparkUi:SetEnable(false);
		end
	end
	
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:SetPartnerState(1);
	end
end

function PartnerScrollDown()
	--LuaUI.PartnerScroll(1);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:ScrollPartner(1);
	end
end

function PartnerScrollUp()
    --LuaUI.PartnerScroll(0);
    local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:ScrollPartner(0);
	end
end

function PartnerScroll()
	--LuaUI.PartnerScroll(2);
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:ScrollPartner(2);
	end
end

function SetPartnerFollow()
	--LuaUI.SetPartnerFollow();
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:SetFollow();
	end
end

function OpenConfirmDropPartner()

	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
  		if partnerMenu:HasPartner() then
		    LuaUI.AddXMLFile2UIManger("Data\\ConfirmDropPartner.xml");
			local partnerPanel = UIManager:GetUIItem("PartnerMainPanel");
			local panel = UIManager:FindUIPanel("ConfirmDropPartnerPanel");
	  		if panel ~= nil then
			    panel:SetPos(partnerPanel:GetX() + 70, partnerPanel:GetY() + 200);
				partnerMenu:ShowRemovePartnerName();
				UIManager:SetMonoPanel(panel);
			end
		end

	end
end

function CloseConfirmDropPartner()
    local panel = UIManager:FindUIPanel("ConfirmDropPartnerPanel");
	if panel ~= nil then
		UIManager:ClearMonoPanel();
		UIManager:DeletePanel("ConfirmDropPartnerPanel");
	end
end

function ConfirmDropPartner()
    --LuaUI.DeleteOnShowPartner();
    local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		partnerMenu:DeleteOnShowPartner();
	end
    CloseConfirmDropPartner();
end

function PartnerWheel(px, py, pz)
	if (px > 0 ) then
		PartnerScrollUp();
	else 
		PartnerScrollDown();
	end
end

function PartnerSkillIconIn()
	local pPanel = nil;
	if _G.InBattle then
		pPanel = UIManager:GetUIItem("PartnerBattleSkillPanel");
	else
		pPanel = UIManager:GetUIItem("PartnerSkillPanel");
	end
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(true);
		pPanel:ResetTimer();
	end
	_G.PartnerSkillTimeOutFlag = 1;
end

function PartnerSkillIconOut()
	if _G.PartnerSkillTimeOutFlag == 0 then
		if _G.InBattle then
			UIManager:GetUIItem("PartnerBattleSkillPanel"):SetEnableTimeOutEvent(false);	
		else
			UIManager:GetUIItem("PartnerSkillPanel"):SetEnableTimeOutEvent(false);
		end
		CloseSkillDetail(0, 0);
	end
end

function PartnerSkillDetailPopupTimeOut()
	local partnerMenu = LuaUI.GetPartnerProcess();
	local panel = UIManager:GetUIItem("PartnerSkillPanel");
	if _G.InBattle then
		panel = UIManager:GetUIItem("PartnerBattleSkillPanel");
	end
	if partnerMenu ~= nil and panel ~= nil then
	    local cursorX = UIManager:GetCursorX();
	    local cursorY = UIManager:GetCursorY();
	    if cursorX > panel:GetX() and cursorY > panel:GetY() and cursorX < (panel:GetX() + panel:GetWidth()) and cursorY < (panel:GetY() + panel:GetHeight()) then
			partnerMenu:ShowSkillDetail();
		else
		    CloseSkillDetail(0, 0);
		end
	end
	if _G.InBattle then
		UIManager:GetUIItem("PartnerBattleSkillPanel"):SetEnableTimeOutEvent(false);	
	else
		UIManager:GetUIItem("PartnerSkillPanel"):SetEnableTimeOutEvent(false);
	end
	_G.PartnerSkillTimeOutFlag = 0;
end

function PartnerUseSkill()
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		local partnerSkillUse = partnerMenu:SelectSkill();
		if partnerSkillUse then
			ClosePartnerBattleSkill();
			HideBattlePartnerPanel();			
		end	
	end
end


function SCPartnerUseSkillGo(Index)
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then 
		local partnerSkillUse = partnerMenu:ShortCutSelectSkill(Index);
		if partnerSkillUse then
			ClosePartnerBattleSkill();
			HideBattlePartnerPanel();			
		end	
	end
end

function SCPartnerUseSkill0()
	SCPartnerUseSkillGo(0)
end

function SCPartnerUseSkill1()
	SCPartnerUseSkillGo(1)
end

function SCPartnerUseSkill2()
	SCPartnerUseSkillGo(2)
end

function SCPartnerUseSkill3()
	SCPartnerUseSkillGo(3)
end

function SCPartnerUseSkill4()
	SCPartnerUseSkillGo(4)
end

function SCPartnerUseSkill5()
	SCPartnerUseSkillGo(5)
end

function SCPartnerUseSkill6()
	SCPartnerUseSkillGo(6)
end

function SCPartnerUseSkill7()
	SCPartnerUseSkillGo(7)
end

function SCPartnerUseSkill8()
	SCPartnerUseSkillGo(8)
end

function SCPartnerUseSkill9()
	SCPartnerUseSkillGo(9)
end

function PartnerWheel(pX, pY, pZ)
	if pZ > 0 then
		PartnerScrollUp();
	else
	  	PartnerScrollDown();
	end
end

function SetPartnerCollectionButtonState(race)
	_G.PartnerCollectionState = race;
	local Button;
	local word;
	for i=1, 10 do
		word = "PartnerCollectionRace"..i;
	    Button = UIManager:FindUIButton(word);
		if Button ~= nil then
		    Button:SetLockState(false);
			Button:SetNormalState();
		end
	end
	word = "PartnerCollectionRace"..race;
	Button = UIManager:FindUIButton(word);
	if Button ~= nil then
        Button:SetClickState();
	    Button:SetLockState(true);
	end

    local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		partnerMenu:UpdatePartnerCollectionMenu(race,1);
	end
end

function PartnerCollectionRace1()
	SetPartnerCollectionButtonState(1)
end

function PartnerCollectionRace2()
	SetPartnerCollectionButtonState(2)
end

function PartnerCollectionRace3()
	SetPartnerCollectionButtonState(3)
end

function PartnerCollectionRace4()
	SetPartnerCollectionButtonState(4)
end

function PartnerCollectionRace5()
	SetPartnerCollectionButtonState(5)
end

function PartnerCollectionRace6()
	SetPartnerCollectionButtonState(6)
end

function PartnerCollectionRace7()
	SetPartnerCollectionButtonState(7)
end

function PartnerCollectionRace8()
	SetPartnerCollectionButtonState(8)
end

function PartnerCollectionRace9()
	SetPartnerCollectionButtonState(9)
end

function PartnerCollectionRace10()
	SetPartnerCollectionButtonState(10)
end

function PartnerCollectionMouseIn()
	local pPanel = UIManager:GetUIItem("PartnerCollectionPanel");
	if pPanel ~= nil then
		pPanel:SetEnableTimeOutEvent(true);
		pPanel:ResetTimer();
	end
	_G.PartnerCollectionTimeOutFlag = 1;
end

function PartnerCollectionMouseOut()
	--if _G.PartnerCollectionTimeOutFlag == 0 then
		local panel = UIManager:GetUIItem("PartnerCollectionPanel")
		if panel ~= nil then
			panel:SetEnableTimeOutEvent(false);	
			SwitchOffPartnerDesc();
		end
	--end
end

function PartnerCollectionDetailPopupTimeOut()
	local partnerMenu = LuaUI.GetPartnerProcess();
	local panel = UIManager:GetUIItem("PartnerCollectionPanel");
	if partnerMenu ~= nil and panel ~= nil then
	    local cursorX = UIManager:GetCursorX();
	    local cursorY = UIManager:GetCursorY();
	    if cursorX > panel:GetX() and cursorY > panel:GetY() and cursorX < (panel:GetX() + panel:GetWidth()) and cursorY < (panel:GetY() + panel:GetHeight()) then
			local panel2 = UIManager:GetUIItem("PartnerDescPanel");
			if panel2 ~= nil then
				partnerMenu:ShowPartnerCollectionDetail(PartnerCollectionState);
				if cursorX > 600 then
					cursorX = cursorX - 300;
				end
				if cursorY > 400 then
					cursorY = cursorY - 210;
				end
				panel2:SetPos( cursorX, cursorY);
		     	panel2:SetEnable(true);
			end
		else
			SwitchOffPartnerDesc();
		end
	end
	UIManager:GetUIItem("PartnerCollectionPanel"):SetEnableTimeOutEvent(false);	
	_G.PartnerCollectionTimeOutFlag = 0;
end

function PartnerCollectionPrevPage()
    local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		partnerMenu:PartnerCollectionNextPage(_G.PartnerCollectionState,false);
	end
end

function PartnerCollectionNextPage()
    local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		partnerMenu:PartnerCollectionNextPage(_G.PartnerCollectionState,true);
	end
end

function PartnerCollectionUseBuffTemp()
    local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		partnerMenu:PartnerCollectionUseBuffTemp(true);
	end
end

function PartnerCollectionUnUseBuffTemp()
    local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		partnerMenu:PartnerCollectionUseBuffTemp(false);
	end
end

function PartnerCollectionDragEnd(value)
	if value == 3 then
		ResetPartnerCollectionDrag()
	else
		local partnerMenu = LuaUI.GetPartnerProcess();
		local movingPanel = UIManager:GetUIItem("PartnerCollectionMovingPanel");
		local partnerPanel = UIManager:GetUIItem("PartnerMainPanel");
		local partnerCollectionPanel = UIManager:GetUIItem("PartnerCollectionPanel");
		
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();
		if partnerMenu ~= nil and movingPanel~= nil and partnerPanel~= nil and partnerCollectionPanel~= nil and partnerCollectionPanel:IsEnable() then
			local relativeX = mx - partnerCollectionPanel:GetX();
			local relativeY = my - partnerCollectionPanel:GetY();
			if relativeX > 15 and relativeX < 400 and relativeY > 90 and relativeY < 410 then
				SwitchOffPartnerDesc()
				local ConfirmPanel = UIManager:GetUIItem("PartnerCollectionConfirmPanel");
				local tempPanel = UIManager:FindUIPanel("PartnerCollectionConfirmPanel");
				if ConfirmPanel ~= nil and tempPanel ~= nil then
					UIManager:SetPanel2Top(tempPanel);
					ConfirmPanel:SetEnable(true);
					UIManager:ClearMonoPanel();
					UIManager:SetMonoPanel(tempPanel);	
					UIManager:AddDirtyUIItem(ConfirmPanel);
				end
				return;
			end
		end
		ResetPartnerCollectionDrag()
	end
end

function ResetPartnerCollectionDrag()
 	UIManager:ResetDragItem();
	UIManager:StopDrag();
	local movingPanel = UIManager:GetUIItem("PartnerCollectionMovingPanel");
	if movingPanel ~= nil then
		movingPanel:SetPos(0,0);
		movingPanel:SetEnable(false);
		movingPanel:SetNotDetected(false);
	end
end

function SelectPartner()
	local partnerMenu = LuaUI.GetPartnerProcess();
	local PartnerColletionPanel = UIManager:GetUIItem("PartnerCollectionPanel");
	local movingPanel = UIManager:GetUIItem("PartnerCollectionMovingPanel");
	if partnerMenu ~= nil and PartnerColletionPanel~= nil and PartnerColletionPanel:IsEnable() and movingPanel~= nil then
		movingPanel:SetEnable(true);
		UIManager:SetDragItemXY(movingPanel,-20,-20);
		partnerMenu:DragPartner();
	end
end

function ClosePartnerCollectionConfirmPanel()
	local PartnerCollectionConfirmPanel = UIManager:GetUIItem("PartnerCollectionConfirmPanel");
	if PartnerCollectionConfirmPanel ~= nil then
		PartnerCollectionConfirmPanel:SetEnable(false);
	end
	ResetPartnerCollectionDrag()
	UIManager:ClearMonoPanel();
end

function PartnerCollectionConfirmOK()
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		partnerMenu:AddPartnerCollection();
	end
	ClosePartnerCollectionConfirmPanel();
end

function PartnerCollectionConfirmCancel()
	ClosePartnerCollectionConfirmPanel();
end

function ReSetPartnerCollectionButtonState(page)
    local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		partnerMenu:UpdatePartnerCollectionMenu(_G.PartnerCollectionState,page);
	end
end
