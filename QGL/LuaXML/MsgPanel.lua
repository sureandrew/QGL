UIManager = LuaUI.GetUIManager()
--OggPlayer = LuaUI.GetOggPlayer();
FSoundManager = LuaUI.GetFSoundManager();

--SoundManager = LuaUI.GetSoundManager();

EditBox = UIManager:FindUIEditBox("EditLine");
MyChat = UIManager:FindUIChatPanel("Chat");
MySpeaker = UIManager:FindUIChatPanel("Speaker");
--MyPop = UIManager:FindUIPopUpMenu("TestPop")

VItemFlag = true
FriendPopupNum = -1

LuaUI.LoadLua("Data\\LanguageLink.out")
LuaUI.LoadLua("Data\\MenuQuestLogNote.lua")
LuaUI.LoadLua("Data\\MapUI.lua")

LuaUI.LoadLua("Data\\MenuCharacter.lua")

LuaUI.LoadLua("Data\\MenuItem.lua")
LuaUI.LoadLua("Data\\ItemDetail.lua")
LuaUI.LoadLua("Data\\MixItemDetail.lua")

LuaUI.LoadLua("Data\\Party.lua")

LuaUI.LoadLua("Data\\Channel.lua")

LuaUI.LoadLua("Data\\ChannelHistory.lua")

LuaUI.LoadLua("Data\\BlockList.lua")

LuaUI.LoadLua("Data\\ChatIconPopPanel.lua")

LuaUI.LoadLua("Data\\Clock.lua")
LuaUI.LoadLua("Data\\SubChat.lua")

LuaUI.LoadLua("Data\\PrivateMsgPopPanel.lua")

LuaUI.LoadLua("Data\\BattleItemPanel.lua")
LuaUI.LoadLua("Data\\Dialogue.lua")
LuaUI.LoadLua("Data\\BatDialogue.lua")

LuaUI.LoadLua("Data\\MenuTeammate.lua")
LuaUI.LoadLua("Data\\ApplyMembershipList.lua")

LuaUI.LoadLua("Data\\ChatBalloonParty.lua")

LuaUI.LoadLua("Data\\Disconnect.lua");

LuaUI.LoadLua("Data\\PlayerMessage.lua");

LuaUI.LoadLua("Data\\BattlePerform.lua");
LuaUI.LoadLua("Data\\MiddleMap.lua");
LuaUI.LoadLua("Data\\MenuPartner.lua");

LuaUI.LoadLua("Data\\BattleJoinMenu.lua");
LuaUI.LoadLua("Data\\ShopMenuPanel.lua");

LuaUI.LoadLua("Data\\SkillMenu.lua");
LuaUI.LoadLua("Data\\MenuQuestLog.lua");
LuaUI.LoadLua("Data\\BigMap.lua");
LuaUI.LoadLua("Data\\MenuChangeLine.lua");

LuaUI.LoadLua("Data\\MenuFriendList.lua");
LuaUI.LoadLua("Data\\MenuSystemSetting.lua");
LuaUI.LoadLua("Data\\MenuPet.lua");
LuaUI.LoadLua("Data\\MenuGiven.lua");
LuaUI.LoadLua("Data\\MenuTrading.lua");

LuaUI.LoadLua("Data\\MenuHelp.lua");
LuaUI.LoadLua("Data\\MenuOnlineShop.lua");

LuaUI.LoadLua("Data\\SoundEffectFunction.lua");

LuaUI.LoadLua("Data\\MenuRank.lua");
LuaUI.LoadLua("Data\\MenuMixProcess.lua");

LuaUI.LoadLua("Data\\MenuTitle.lua");
LuaUI.LoadLua("Data\\MenuHawker.lua");

LuaUI.LoadLua("Data\\MenuAntiHack.lua");

LuaUI.LoadLua("Data\\MenuGuild.lua");
LuaUI.LoadLua("Data\\MenuWithdrawalBank.lua");
LuaUI.LoadLua("Data\\MenuDepositBank.lua");

LuaUI.LoadLua("Data\\AlertBox.lua");
LuaUI.LoadLua("Data\\MenuGiftBox.lua");
LuaUI.LoadLua("Data\\MenuQuestJournal.lua");
LuaUI.LoadLua("Data\\Question.lua");
LuaUI.LoadLua("Data\\EjectStudent.lua");

LuaUI.LoadLua("Data\\MenuWareHouse.lua");

LuaUI.LoadLua("Data\\AssetLockPassword.lua");
LuaUI.LoadLua("Data\\AssetUnLock.lua");

LuaUI.LoadLua("Data\\Keyboard.lua");

LuaUI.LoadLua("Data\\MenuAutoWalk.lua");

LuaUI.LoadLua("Data\\MenuTargetEquip.lua");
LuaUI.LoadLua("Data\\SellYuanBao.lua");

LuaUI.LoadLua("Data\\RelationSystem.lua");

LuaUI.LoadLua("Data\\LineupSelect.lua");
LuaUI.LoadLua("Data\\Wanted.lua");
LuaUI.LoadLua("Data\\GenerationPanel.lua");

LuaUI.LoadLua("Data\\PartnerCultivationUp.lua");

LuaUI.LoadLua("Data\\ChooseTimePanel.lua");
LuaUI.LoadLua("Data\\AccShopUi.lua");
LuaUI.LoadLua("Data\\PointsShopUi.lua");

LuaUI.LoadLua("Data\\ShowSkill.lua");
LuaUI.LoadLua("Data\\NewGuide.lua");

LuaUI.LoadLua("Data\\WeaponsRank.lua");
LuaUI.LoadLua("Data\\TeamDescription.lua");

function HideMiniMapPanel(pX, pY)
		UIManager:CloseTips()
		if UIManager:GetUIItem("Location"):IsEnable() then
			UIManager:GetUIItem("Location"):SetEnable(false);
			UIManager:GetUIItem("MiniMap"):SetEnable(false);
			UIManager:GetUIItem("Time"):SetEnable(false);
			UIManager:GetUIItem("ClockPanel"):SetEnable(false);
		else
			UIManager:GetUIItem("Location"):SetEnable(true);
			UIManager:GetUIItem("MiniMap"):SetEnable(true);
			UIManager:GetUIItem("Time"):SetEnable(true);
			UIManager:GetUIItem("ClockPanel"):SetEnable(false);
			LuaUI.UpdateRightUpTimeInfo();
		end
end

function BBAttack()
	LuaUI.BBAttack()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(15);
end

function BBTrade()
	LuaUI.BBTrade()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(16);
end

function BBGive()
	LuaUI.BBGive()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(17);
end

function BBParty()
	CloseNewArrow();
	if _G.InBattle ~= true then
	    local tradingPanel = UIManager:GetUIItem("TradingPanel");
	    local givenPanel = UIManager:GetUIItem("GivenItemPanel");

	    if tradingPanel ~= nil or givenPanel ~= nil then
	        do return end
	    end

		if LuaUI.HasParty() or LuaUI.HasTempParty() then
		    local popPanel = UIManager:FindUIPanel("TeammatePanel");
			if popPanel == nil then
			    LuaUI.AddXMLFile2UIManger("Data\\MenuTeammate.xml");

			    local Menu = LuaUI.GetMenuHelpProcess();
				Menu:HandleEvent(18);
			    LuaUI.UpdatePartyPanel();

			    if LuaUI.IsJoinRequestExist() then
			    	OpenApplyMembershipList(0, 0);
			    end

			    UIManager:RenderAll();
				PlayOpenInterfaceEffect();
			else
				CloseTeammateMenu(0,0);
			end

		elseif LuaUI.InviteComing() then
		    local popPanel = UIManager:FindUIPanel("TeammatePanel");
			if popPanel == nil then
			    LuaUI.AddXMLFile2UIManger("Data\\MenuTeammate.xml");


				local button = UIManager:GetUIItem("PrevTeam");
				button:SetEnable(true);
				button = UIManager:GetUIItem("NextTeam");
				button:SetEnable(true);
				button = UIManager:GetUIItem("InviteAccept");
				button:SetEnable(true);
				button = UIManager:GetUIItem("InviteReject");
				button:SetEnable(true);

				button = UIManager:GetUIItem("ApplyMembership");
				button:SetEnable(false);
				button = UIManager:GetUIItem("InviteToJoin");
				button:SetEnable(false);
				button = UIManager:GetUIItem("Signal");
				button:SetEnable(false);
				button = UIManager:GetUIItem("FormationChosen");
				button:SetEnable(false);
				button = UIManager:GetUIItem("LeaveParty");
				button:SetEnable(false);

				LuaUI.ShowPartyByIndex();

				UIManager:RenderAll();
			    local sfx = FSoundManager:CreateSoundA("Data\\Wav\\SoundI0001_open.wav");
			    local systemSetting = LuaUI.GetMenuSystemSetting();
				if systemSetting ~= nil and sfx ~= nil then
					sfx:Play();
				end
			else
				CloseTeammateMenu(0,0);
			end

		else
			LuaUI.BBParty();

		end
	else
		--UIManager:FindUIButton("DownBParty"):SetDisableState();
		OpenJoinBattleList(0, 0);
	end

end

function CharMenuButtonStateAdjust()

	local button = UIManager:FindUIButton("CharSTRPlus");
	if button == nil then
		do return end;
	end
	--[[if _G.InBattle == true then
		UIManager:FindUIButton("CharSTRPlus"):SetDisableState();
		UIManager:FindUIButton("CharSTRMinus"):SetDisableState();
		UIManager:FindUIButton("CharCONPlus"):SetDisableState();
		UIManager:FindUIButton("CharCONMinus"):SetDisableState();
		UIManager:FindUIButton("CharSTAPlus"):SetDisableState();
		UIManager:FindUIButton("CharSTAMinus"):SetDisableState();
		UIManager:FindUIButton("CharAGIPlus"):SetDisableState();
		UIManager:FindUIButton("CharAGIMinus"):SetDisableState();
		UIManager:FindUIButton("CharSPRPlus"):SetDisableState();
		UIManager:FindUIButton("CharSPRMinus"):SetDisableState();
		UIManager:FindUIButton("CharAttOk"):SetDisableState();
		UIManager:FindUIButton("CharLevelUp"):SetDisableState();

		--local panel = UIManager:FindUIPanel("CharacterMenuPanel");
		--if panel ~= nil then
		--	UIManager:AddDirtyUIItem(panel);
		--end
	--else
		UIManager:FindUIButton("CharSTRPlus"):SetNormalState();
		UIManager:FindUIButton("CharSTRMinus"):SetNormalState();
		UIManager:FindUIButton("CharCONPlus"):SetNormalState();
		UIManager:FindUIButton("CharCONMinus"):SetNormalState();
		UIManager:FindUIButton("CharSTAPlus"):SetNormalState();
		UIManager:FindUIButton("CharSTAMinus"):SetNormalState();
		UIManager:FindUIButton("CharAGIPlus"):SetNormalState();
		UIManager:FindUIButton("CharAGIMinus"):SetNormalState();
		UIManager:FindUIButton("CharSPRPlus"):SetNormalState();
		UIManager:FindUIButton("CharSPRMinus"):SetNormalState();
		--UIManager:FindUIButton("CharAttOk"):SetNormalState();
		if LuaUI.CanLevelUp() then
			UIManager:FindUIButton("CharLevelUp"):SetNormalState();
		else
			UIManager:FindUIButton("CharLevelUp"):SetDisableState();
		end

	--end ]]
	LuaUI.UpdateCharMenu();

end

function MyFaceLButtonDown()
	local MyFace = UIManager:GetUIItem("MyFace")
	if MyFace then
		MyFace:SetOffsetXY(21, 7)
	end
end

function MyFaceUnPickUp()
	local MyFace = UIManager:GetUIItem("MyFace")
	if MyFace then
		MyFace:SetOffsetXY(20, 6)
	end
end

function BBChar()
	--if _G.InBattle ~= true then
		local PopPanel = UIManager:FindUIPanel("CharacterMenuPanel")
		local MyFaceSparkUi = UIManager:FindUIAniIcon("MyFaceSpark")
		if PopPanel == nil then
			LuaUI.AddXMLFile2UIManger("Data\\MenuCharacter.xml");
			local Menu = LuaUI.GetMenuHelpProcess();
			Menu:HandleEvent(19);
			--CharMenuButtonStateAdjust();
			LuaUI.UpdateCharMenu()
			--UIManager:RenderAll();
			UIManager:AddDirtyUIItem(UIManager:GetUIItem("CharacterMenuPanel"));

			PlayOpenInterfaceEffect();
		else
			CloseCharacterMenu(0, 0)
		end

		if MyFaceSparkUi:IsEnable() then
			MyFaceSparkUi:SetEnable(false);
		end

end

function BBFriend()

	--LuaUI.BBFriend();
 --if _G.InBattle ~= true then
	    OpenPartner();

		local Menu = LuaUI.GetMenuHelpProcess();
		Menu:HandleEvent(9);


 --[[else
	 	UIManager:FindUIButton("DownBFriend"):SetDisableState();
	end]]


end

function BBItem()

	CloseNewArrow();
	local PopPanel = UIManager:FindUIPanel("ItemMenuPanel")
	if PopPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuItem.xml");

		ItemCharacter(0, 0);
		ItemCurrentBag(0, 0);

		PlayOpenInterfaceEffect();
	else
		CloseItemMenu(0, 0)
	end

	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(4);
end

function BBSkill()
	CloseNewArrow();
	OpenSkillPanel(0, 0);
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(7);
end

function BBHousen()
	OpenGuildPanel(0, 0);
end

function BBMission()
	OpenQuestLogPanel(0, 0);
	CloseNewArrow();
--	if _G.InBattle ~= true then
--		local PopPanel = UIManager:FindUIPanel("QuestLogMenuPanel")
--		if PopPanel == nil then
--			LuaUI.AddXMLFile2UIManger("Data\\MenuQuestLog.xml")
--
--			UIManager:RenderAll();
--
--			local sfx = FSoundManager:CreateSoundA("Data\\Wav\\SoundI0001_open.wav");
--			sfx:SetVolume(LuaUI.GetSFXVolume() / 100);
--			sfx:Play();
--		else
--			CloseQuestLog(0, 0)
--		end
--
--		local button = UIManager:FindUIButton("DownBMission")
--		if button ~= nil then
--			button:StopPlaySoundOnce();
--		end
--	else
--		UIManager:FindUIButton("DownBMission"):SetDisableState();
--	end
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(8);
end

function BBFriendShip()
	--LuaUI.BBFriendShip()
	OpenFriendList();
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(21);
end

function BBPet()
	--LuaUI.BBEmotion()
	OpenPet();
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(10);
end

function BBSystem()
	--LuaUI.BBSystem()
	--LuaUI.TestingFunction();
	OpenSystemSetting();
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(11);
end


function BBAutoWalk()
	OpenAutoWalk();
end

function ShowTime()
	local AutoWalk = LuaUI.GetAutoWalkProcess();
	AutoWalk:ShowTime();
end

function OpenMailPanel()
	local Menu = LuaUI.GetMenuHelpProcess();
	Menu:HandleEvent(22);
end
function  BattleAction()
	LuaUI.BattleAction()
end

--Set all the panel Disable expext the mini map
function SetAllPanelDisable()
	_G.CloseAllPanel = true;

	UIManager:SetAllPanelDisable();

end

function ConfirmPK()
	LuaUI.ConfirmPK();
end

function CancelPK()
	LuaUI.CancelPK();
end

function ConfirmMatch()
	LuaUI.ConfirmMatch();
end

function CancelMatch()
	LuaUI.CancelMatch();
end

function QuestCancelConfirm()
	LuaUI.QuestCancelConfirm();
end

function QuestCancelCancel()
	LuaUI.QuestCancelCancel();
end

function OpenTargetPopUp()
	local menu = LuaUI.GetMenuTargetProcess();
	menu:ShowTargetPopUp();

end

function TargetPopUpPanelClk()
	local menu = LuaUI.GetMenuTargetProcess();
	menu:PopUpFunctionCall();
	CloseTargetPopUpPanel();
end

function CloseTargetPopUpPanel()
	local panel = UIManager:GetUIItem("TargetPopUpPanel");
	if panel ~= nil then
		panel:SetEnable(false);
	end
end

function OnLeftClick()
	CloseTargetPopUpPanel();
end

function OnRightClick()
	CloseTargetPopUpPanel();
end

function CloseTargetInfoPanel()
	local panel = UIManager:GetUIItem("TargetDetailInfoPanel");
	if panel ~= nil then
		panel:SetEnable(false);
	end
	CloseTargetEquipMenu(0, 0);
end

function ClickPartnerSmallIcon()
	CloseNewArrow();
	local panel = UIManager:GetUIItem("Partner");
	local PartnerFaceSparkUi = UIManager:FindUIAniIcon("PartnerFaceSpark");
	if panel ~= nil then
		local x = UIManager:GetCursorX() - panel:GetX();
		local y = UIManager:GetCursorY() - panel:GetY();
		if x > 17 and y > 4 and x < 50 and y < 40 then
		    BBFriend();
		end
	end

	if PartnerFaceSparkUi ~= nil then
		if PartnerFaceSparkUi:IsEnable() then
				PartnerFaceSparkUi:SetEnable(false);
		end
	end
end

function PartnerFaceLButtonDown()
	local PartnerFace = UIManager:GetUIItem("PartnerFace")
	if PartnerFace then
		PartnerFace:SetOffsetXY(18, 5)
	end
end

function PartnerFaceUnPickUp()
	local PartnerFace = UIManager:GetUIItem("PartnerFace")
	if PartnerFace then
		PartnerFace:SetOffsetXY(17, 4)
	end
end

function MixShopMakePanelWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("MixShopProductScrollBar"):OnMouseWheel(UIManager:GetUIItem("MixShopProductScrollBar"):GetX()
		,UIManager:GetUIItem("MixShopProductScrollBar"):GetY(),pZ);
end

function ResetKeyIn()
    local item = UIManager:GetUIItem("FriendChatPanel");
	if item ~= nil then
		UIManager:SetKeyInItem("ChatEditLine");
	else
		UIManager:SetKeyInItem("EditLine");
	end
end

function BBOpenWebSite()
	LuaUI.OpenWebLink();
end

function addParnerHP()
	local partnerProcess = LuaUI.GetPartnerProcess();
	if partnerProcess ~= nil then
		partnerProcess:PartnerEatFastFood(0);
	end
end

function addParnerSP()
	local partnerProcess = LuaUI.GetPartnerProcess();
	if partnerProcess ~= nil then
		partnerProcess:PartnerEatFastFood(1);
	end
end

function addCharHP()
	LuaUI.CharEatFastFood(0);
end

function addCharSP()
	LuaUI.CharEatFastFood(1);
end
