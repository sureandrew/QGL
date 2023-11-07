UIManager = LuaUI.GetUIManager()
--OggPlayer = LuaUI.GetOggPlayer();
FSoundManager = LuaUI.GetFSoundManager();

SkillProcess =  LuaUI.GetSkillProcess();
SkillTimeOutFlag = 0;

function OpenSkillPanel(px, py)
	SkillProcess =  LuaUI.GetSkillProcess();
	local panel = UIManager:GetUIItem("SkillPanel");
   if panel == nil then
       LuaUI.AddXMLFile2UIManger("Data\\SkillMenu.xml");
       PlayOpenInterfaceEffect();
       SkillProcess:UpdateSkillMenu();
       local subpanel = UIManager:GetUIItem("SkillDetailMenuPanel");
       if subpanel == nil then
			LuaUI.AddXMLFile2UIManger("Data\\SkillDetail.xml");
			subpanel = UIManager:GetUIItem("SkillDetailMenuPanel");
			if subpanel ~= nil then
				subpanel:SetEnable(false);
			end
       end
       if LuaUI.IsOpenSkillShop() then
			ShowSkillShop();
       else
			HideSkillShop();
       end
       if _G.UseSpecial then
	    SkillCheckSpecialLua();
       else--if _G.InBattle then
	    SkillCheckBattleLua();
	   --else
		--SkillCheckGangLua();
	    end
	elseif panel:IsEnable() then
		CloseSkillPanel(px,py);
	else
		panel:SetEnable(true);
		 if LuaUI.IsOpenSkillShop() then
			ShowSkillShop();
       else
			HideSkillShop();
       end
       if _G.UseSpecial then
	    SkillCheckSpecialLua();
       else--if _G.InBattle then
	    SkillCheckBattleLua();
	   --else
		--SkillCheckGangLua();
	    end
   end
end

function CloseSkillPanel()
	UIManager:StopDrag();
	ResetShortCutDropItemPos();
	local panel = UIManager:GetUIItem("SkillPanel");
	LuaUI.LuaSetShopSkillFlag(false)
	if panel ~= nil then
		UIManager:AddDirtyUIItem(panel);
		panel:SetEnable(false);
		local subpanel = UIManager:GetUIItem("SkillDetailMenuPanel");
		if subpanel ~= nil then
			UIManager:AddDirtyUIItem(subpanel);
			--subpanel:SetEnable(false);
			UIManager:DeletePanel("SkillDetailMenuPanel");
		end
		UIManager:DeletePanel("SkillPanel");
		CloseSkillDetail(0, 0);
		if LuaUI.IsBattlePartnerTurn() ~= true then
			ShowBattlePanel();
		end
		if LuaUI.IsOpenSkillShop() then
			SkillProcess =  LuaUI.GetSkillProcess();
			SkillProcess:SetSkillShop(0);
		end
		PlayCloseInterfaceEffect();
	end

	local panel = UIManager:GetUIItem("SkillShopPanel");
	if panel ~= nil then
		UIManager:DeletePanel("SkillShopPanel");
	end
end

function SkillCheckGangLua()
	if not _G.InBattle then
	ResetAllSkillCheckBox();
	UIManager:FindUICheckBox("SkillCheckGang"):SetState(true);
	SkillProcess:UpdateSkillMenu();
	UIManager:RenderAll();
	else
	UIManager:FindUICheckBox("SkillCheckGang"):SetState(false);
	UIManager:AddDirtyUIItem(UIManager:FindUICheckBox("SkillCheckGang"));
	end
end

function SkillCheckLiveLua()
	if not _G.InBattle then
	ResetAllSkillCheckBox();
	UIManager:FindUICheckBox("SkillCheckLive"):SetState(true);
	SkillProcess:UpdateSkillMenu();
	UIManager:RenderAll();
	else
	UIManager:FindUICheckBox("SkillCheckLive"):SetState(false);
	UIManager:AddDirtyUIItem(UIManager:FindUICheckBox("SkillCheckGang"));
	end
end

function SkillCheckGuildLua()
	if not _G.InBattle then
	ResetAllSkillCheckBox();
	UIManager:FindUICheckBox("SkillCheckGuild"):SetState(true);
	SkillProcess:UpdateSkillMenu();
	UIManager:RenderAll();
	else
	UIManager:FindUICheckBox("SkillCheckGuild"):SetState(false);
	UIManager:AddDirtyUIItem(UIManager:FindUICheckBox("SkillCheckGang"));
	end
end

function SkillCheckSpecialLua()
	if _G.UseSpecial or not _G.InBattle then
	ResetAllSkillCheckBox();
	UIManager:FindUICheckBox("SkillCheckSpecial"):SetState(true);
	SkillProcess:UpdateSkillMenu();
	UIManager:RenderAll();
	else
	UIManager:FindUICheckBox("SkillCheckSpecial"):SetState(false);
	UIManager:AddDirtyUIItem(UIManager:FindUICheckBox("SkillCheckGang"));
	end
end

function SkillCheckBattleLua()
	if not _G.UseSpecial then
	ResetAllSkillCheckBox();
	UIManager:FindUICheckBox("SkillCheckBattle"):SetState(true);
	SkillProcess:UpdateSkillMenu();
	UIManager:RenderAll();
	else
	UIManager:FindUICheckBox("SkillCheckBattle"):SetState(false);
	UIManager:AddDirtyUIItem(UIManager:FindUICheckBox("SkillCheckGang"));
	end
end

function ResetAllSkillCheckBox()
	UIManager:FindUICheckBox("SkillCheckGang"):SetState(false);
	UIManager:FindUICheckBox("SkillCheckLive"):SetState(false);
	UIManager:FindUICheckBox("SkillCheckGuild"):SetState(false);
	UIManager:FindUICheckBox("SkillCheckSpecial"):SetState(false);
	UIManager:FindUICheckBox("SkillCheckBattle"):SetState(false);
end

function ResetAllMainSkillCheckBox()
	UIManager:FindUICheckBox("SkillMainSel1"):SetState(false);
	UIManager:FindUICheckBox("SkillMainSel2"):SetState(false);
	UIManager:FindUICheckBox("SkillMainSel3"):SetState(false);
	UIManager:FindUICheckBox("SkillMainSel4"):SetState(false);
	UIManager:FindUICheckBox("SkillMainSel5"):SetState(false);
end

function ResetAllSubSkillCheckBox()
	UIManager:FindUICheckBox("SkillSubSel1"):SetState(false);
	UIManager:FindUICheckBox("SkillSubSel2"):SetState(false);
	UIManager:FindUICheckBox("SkillSubSel3"):SetState(false);
	UIManager:FindUICheckBox("SkillSubSel4"):SetState(false);
	UIManager:FindUICheckBox("SkillSubSel5"):SetState(false);
	UIManager:FindUICheckBox("SkillSubSel6"):SetState(false);
	UIManager:FindUICheckBox("SkillSubSel7"):SetState(false);
end

function SkillMainButtonUpLua()
	SkillProcess:SkillMenuMainUp();
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillMainButtonDownLua()
	SkillProcess:SkillMenuMainDown();
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillSubButtonUpLua()
	SkillProcess:SkillMenuMainSubUp();
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillSubButtonDownLua()
	SkillProcess:SkillMenuMainSubDown();
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillMainScrollLua()
	SkillProcess:SkillMenuMainScroll();
end

function SkillSubScrollLua()
	SkillProcess:SkillMenuSubScroll();
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillMainSel1Lua()
	ResetAllMainSkillCheckBox();
	SkillProcess:UpdateMainSkillMenu(1);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillMainSel2Lua()
	ResetAllMainSkillCheckBox();
	SkillProcess:UpdateMainSkillMenu(2);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillMainSel3Lua()
	ResetAllMainSkillCheckBox();
	SkillProcess:UpdateMainSkillMenu(3);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillMainSel4Lua()
	ResetAllMainSkillCheckBox();
	SkillProcess:UpdateMainSkillMenu(4);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillMainSel5Lua()
	ResetAllMainSkillCheckBox();
	SkillProcess:UpdateMainSkillMenu(5);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillSubSel1Lua()
	ResetAllSubSkillCheckBox();
	SkillProcess:UpdateSubSkillMenu(1);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillSubSel2Lua()
	ResetAllSubSkillCheckBox();
	SkillProcess:UpdateSubSkillMenu(2);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillSubSel3Lua()
	ResetAllSubSkillCheckBox();
	SkillProcess:UpdateSubSkillMenu(3);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillSubSel4Lua()
	ResetAllSubSkillCheckBox();
	SkillProcess:UpdateSubSkillMenu(4);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillSubSel5Lua()
	ResetAllSubSkillCheckBox();
	SkillProcess:UpdateSubSkillMenu(5);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillSubSel6Lua()
	ResetAllSubSkillCheckBox();
	SkillProcess:UpdateSubSkillMenu(6);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function SkillSubSel7Lua()
	ResetAllSubSkillCheckBox();
	SkillProcess:UpdateSubSkillMenu(7);
	local panel = UIManager:GetUIItem("SkillPanel");
	UIManager:AddDirtyUIItem(panel);
end

function UseSkill1Lua()
	SkillSubSel1Lua();
	local skilluse = SkillProcess:UseSkill(1);
	if skilluse then
		CloseSkillPanel();
		HideBattlePanel();
		HideBattlePartnerPanel();
	end
end

function UseSkill2Lua()
	SkillSubSel2Lua();
	local skilluse = SkillProcess:UseSkill(2);
	if skilluse then
		CloseSkillPanel();
		HideBattlePanel();
		HideBattlePartnerPanel();
	end
end

function UseSkill3Lua()
	SkillSubSel3Lua();
	local skilluse = SkillProcess:UseSkill(3);
	if skilluse then
		CloseSkillPanel();
		HideBattlePanel();
		HideBattlePartnerPanel();
	end
end

function UseSkill4Lua()
	SkillSubSel4Lua();
	local skilluse = SkillProcess:UseSkill(4);
	if skilluse then
		CloseSkillPanel();
		HideBattlePanel();
		HideBattlePartnerPanel();
	end
end

function UseSkill5Lua()
	SkillSubSel5Lua();
	local skilluse = SkillProcess:UseSkill(5);
	if skilluse then
		CloseSkillPanel();
		HideBattlePanel();
		HideBattlePartnerPanel();
	end
end

function UseSkill6Lua()
	SkillSubSel7Lua();
	local skilluse = SkillProcess:UseSkill(6);
	if skilluse then
		CloseSkillPanel();
		HideBattlePanel();
		HideBattlePartnerPanel();
	end
end

function UseSkill7Lua()
	SkillSubSel7Lua();
	local skilluse = SkillProcess:UseSkill(7);
	if skilluse then
		CloseSkillPanel();
		HideBattlePanel();
		HideBattlePartnerPanel();
	end
end

function DisableAllTimeout()
	UIManager:GetUIItem("SkillMainSelIcon1"):SetEnableTimeOutEvent(false);
	UIManager:GetUIItem("SkillMainSelIcon2"):SetEnableTimeOutEvent(false);
	UIManager:GetUIItem("SkillMainSelIcon3"):SetEnableTimeOutEvent(false);
	UIManager:GetUIItem("SkillMainSelIcon4"):SetEnableTimeOutEvent(false);
	UIManager:GetUIItem("SkillMainSelIcon5"):SetEnableTimeOutEvent(false);
end
function SkillDetailPopupTimeOut()
	SkillProcess:OpenSkillDetail();
	--DisableAllTimeout();
	UIManager:GetUIItem("SkillPanelBg"):SetEnableTimeOutEvent(false);
	SkillTimeOutFlag = 0;
end

function SkillIconIn()
	local pPanel = UIManager:GetUIItem("SkillPanelBg");

	if pPanel ~= nil and not UIManager:DragNow() then
		pPanel:SetEnableTimeOutEvent(true);
		ResetAllSubSkillCheckBox();
		--
		local index = 1;
		--UIManager:Detect("A", UIManager:GetCursorX(), UIManager:GetCursorY())
		while index <= 7 do
			local name = string.format("SkillSubSel%d", index);
			if UIManager:Detect(name, UIManager:GetCursorX(), UIManager:GetCursorY()) then
				break;
			end
			index = index + 1;
		end
		SkillProcess:UpdateSubSkillMenu(index);
		local panel = UIManager:GetUIItem("SkillPanel");
		UIManager:AddDirtyUIItem(panel);
		pPanel:ResetTimer();
	end
	SkillTimeOutFlag = 1;
end

function SkillIconOut()
	--ResetAllSubSkillCheckBox();
	if SkillTimeOutFlag == 0 then
		UIManager:GetUIItem("SkillPanelBg"):SetEnableTimeOutEvent(false);
		CloseSkillDetail(0, 0);
	end
end

function CloseSkillDetail(pX, pY)
	local panel = UIManager:GetUIItem("SkillDetailMenuPanel");
	if panel ~= nil then
		UIManager:AddDirtyUIItem(panel);
		panel:SetEnable(false);
		--UIManager:DeletePanel("SkillDetailMenuPanel");
	end
end

function SkillPanelDrag(pX, pY)
	local panel = UIManager:GetUIItem("SkillPanel");
   if panel ~= nil then
		UIManager:SetDragItemXY(panel,panel:GetX()- UIManager:GetCursorX() ,  panel:GetY()- UIManager:GetCursorY());
   end
end

function ShowSkillShop()

	UIManager:GetUIItem("SkillShopPanel"):SetEnable(true);


	UIManager:GetUIItem("SkillCheckGang"):SetEnable(false);
	UIManager:GetUIItem("SkillCheckLive"):SetEnable(false)
	UIManager:GetUIItem("SkillCheckGuild"):SetEnable(false);
	UIManager:GetUIItem("SkillCheckSpecial"):SetEnable(false);
	UIManager:GetUIItem("SkillCheckBattle"):SetEnable(false);

	UIManager:AddDirtyUIItem(UIManager:GetUIItem("SkillPanel"));
	local pPanel = UIManager:GetUIItem("SkillShopPanel");
	local pPanel2 = UIManager:GetUIItem("SkillPanel");
	if pPanel and pPanel2 then
		pPanel:SetPos(pPanel2:GetX() + 337, pPanel2:GetY()+ 12);
	end

end

function HideSkillShop()
	UIManager:GetUIItem("SkillShopPanel"):SetEnable(false);

	UIManager:GetUIItem("SkillCheckGang"):SetEnable(true);
	UIManager:GetUIItem("SkillCheckLive"):SetEnable(true)
	UIManager:GetUIItem("SkillCheckGuild"):SetEnable(true);
	UIManager:GetUIItem("SkillCheckSpecial"):SetEnable(true);
	UIManager:GetUIItem("SkillCheckBattle"):SetEnable(true);

	UIManager:AddDirtyUIItem(UIManager:GetUIItem("SkillPanel"));
end

function LearnSkillLua()
	SkillProcess =  LuaUI.GetSkillProcess();
	SkillProcess:ShopLearn();
end



function HSkillShortCutMove(Index)

	local movingPanel = UIManager:GetUIItem("ShortCutMovingPanel");
	local IconName = "SkillSubSelIcon"..Index
	local orgIcon = UIManager:GetUIItem(IconName);

	if movingPanel ~= nil and orgIcon ~= nil then
		local orgAniIcon = UIManager:FindUIAniIcon(IconName);
		local targetAniIcon = UIManager:FindUIAniIcon("ShortCutMovingIcon");

		if orgAniIcon ~= nil and targetAniIcon ~= nil then
		    targetAniIcon:ClearAniData();
		    targetAniIcon:SetAnimationData(orgAniIcon);
		end

		g_ShorcutSelSlotRef = SkillProcess:GetUseSkillID(Index)
		g_ShorcutSelSlotType = 2

		UIManager:SetDragItemXY(movingPanel, orgIcon:GetX()- UIManager:GetCursorX() ,  orgIcon:GetY()- UIManager:GetCursorY());

  		movingPanel:SetEnable(true);
		UIManager:AddDirtyUIItem(movingPanel);

		CloseSkillDetail(0, 0);

		local sfx = FSoundManager:CreateSoundA("Data\\Wav\\SoundI0003_take.wav");
		if sfx ~= nil then
			sfx:SetVolume(LuaUI.GetSFXVolume() / 100);
			sfx:Play();
		end
	end
end


function PickUpSkill1(pX, pY)
	if _G.InBattle or LuaUI.LuaGetShopSkillFlag() then
		UseSkill1Lua()
	else
		HSkillShortCutMove(1)
	end
end

function PickUpSkill2(pX, pY)
	if _G.InBattle or LuaUI.LuaGetShopSkillFlag() then
		UseSkill2Lua()
	else
		HSkillShortCutMove(2)
	end
end

function PickUpSkill3(pX, pY)
	if _G.InBattle or LuaUI.LuaGetShopSkillFlag() then
		UseSkill3Lua()
	else
		HSkillShortCutMove(3)
	end
end

function PickUpSkill4(pX, pY)
	if _G.InBattle or LuaUI.LuaGetShopSkillFlag() then
		UseSkill4Lua()
	else
		HSkillShortCutMove(4)
	end
end

function PickUpSkill5(pX, pY)
	if _G.InBattle or LuaUI.LuaGetShopSkillFlag() then
		UseSkill5Lua()
	else
		HSkillShortCutMove(5)
	end
end

function PickUpSkill6(pX, pY)
	if _G.InBattle or LuaUI.LuaGetShopSkillFlag() then
		UseSkill6Lua()
	else
		HSkillShortCutMove(6)
	end
end

function PickUpSkill7(pX, pY)
	if _G.InBattle or LuaUI.LuaGetShopSkillFlag() then
		UseSkill7Lua()
	else
		HSkillShortCutMove(7)
	end
end

function SkillPanelWheel(pX, pY, pZ)

	local x = pX - UIManager:GetUIItem("SkillPanel"):GetX();
	if x <= 100 then
	UIManager:FindUIScrollBarV("SkillMainMenuScrollBar"):OnMouseWheel(UIManager:GetUIItem("SkillMainMenuScrollBar"):GetX()
	,UIManager:GetUIItem("SkillMainMenuScrollBar"):GetY(),pZ);
	else
	UIManager:FindUIScrollBarV("SkillSubMenuScrollBar"):OnMouseWheel(UIManager:GetUIItem("SkillSubMenuScrollBar"):GetX()
	,UIManager:GetUIItem("SkillSubMenuScrollBar"):GetY(),pZ);
	end

end

