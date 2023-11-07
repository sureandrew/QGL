Color =
{
	"FFFFFFFF",
	"FF00FFFF",
	"FFFF00FF",
	"FFFFFF00",
	"FFFFFF00",
	"FFFF0000",
	"FF00FF00",
	"FF4040FF",
}

InBattle = false;
UseSpecial = false;
bNoteOpen = false;

ChatColor = Color[1]

CloseAllPanel = false;

MissionButtonState = 0;

function HideBattlePanel()
	UIManager:CloseTips()
	UIManager:AddDirtyUIItem(UIManager:GetUIItem("BattlePanel"));
	UIManager:GetUIItem("BattlePanel"):SetEnable(false);
	UIManager:GetUIItem("BattleWinePanel"):SetEnable(false);

	local partnerProcess = LuaUI.GetPartnerProcess();
	if partnerProcess ~= nil then
	    partnerProcess:UpdateStateButton();
	end
end

function HideBattlePartnerPanel()
	UIManager:CloseTips()
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

 	UIManager:GetUIItem("BattlePartnerPanel"):SetEnable(false);
    UIManager:AddDirtyUIItem(UIManager:GetUIItem("BattlePartnerPanel"));
end

function HideBattleWatchPanel()
	UIManager:CloseTips()
	UIManager:GetUIItem("BattleWatchPanel"):SetEnable(false);
	UIManager:AddDirtyUIItem(UIManager:GetUIItem("BattleWatchPanel"));

end

function HideWaitPanel()
	UIManager:CloseTips()
	UIManager:GetUIItem("BattleWait"):SetEnable(false);
	UIManager:AddDirtyUIItem(UIManager:GetUIItem("BattleWait"));

end

function HideAllBattleMenu()
	UIManager:CloseTips()
	SwitchOffPartnerSkill();
	ClosePartner();
	CloseBattleItemMenu(0, 0);
	CloseCharacterMenu(0, 0);
	CloseSkillPanel();
	ClosePartnerBattleSkill();
end

function ShowBattleWatchPanel()
	UIManager:CloseTips()
	UIManager:GetUIItem("BattleWatchPanel"):SetEnable(true);
	UIManager:FindUIButton("BattleJoinB"):SetDisableState();
	UIManager:FindUIButton("BattleWatchLeaveB"):SetNormalState();
	UIManager:AddDirtyUIItem(UIManager:GetUIItem("BattleWatchPanel"));

end

function ShowBattlePanel()
	if _G.InBattle then
	UIManager:CloseTips()
	if not LuaUI.GetHelpEnable() then
		UIManager:FindUIButton("BattleHelpB"):SetDisableState();
	else
		UIManager:FindUIButton("BattleHelpB"):SetNormalState();
	end

	if not LuaUI.GetSkillEnable() then
		UIManager:FindUIButton("BattleSkillB"):SetDisableState();
	else
		UIManager:FindUIButton("BattleSkillB"):SetNormalState();
	end

	if not LuaUI.GetActionEnable() then
		UIManager:FindUIButton("BattleSpecialB"):SetDisableState();
	else
		UIManager:FindUIButton("BattleSpecialB"):SetNormalState();
	end

	if not LuaUI.GetItemEnable() then
		UIManager:FindUIButton("OpenBattleItemMenuB"):SetDisableState();
	else
		UIManager:FindUIButton("OpenBattleItemMenuB"):SetNormalState();
	end



	if not LuaUI.GetDefenceEnable() then
		UIManager:FindUIButton("BattleDefenceB"):SetDisableState();
	else
		UIManager:FindUIButton("BattleDefenceB"):SetNormalState();
	end

	if not LuaUI.GetFriendEnable() then
		UIManager:FindUIButton("BattleFriendB"):SetDisableState();
	else
		UIManager:FindUIButton("BattleFriendB"):SetNormalState();

	end

	if not LuaUI.GetProtectEnable() then
		UIManager:FindUIButton("BattleProtectB"):SetDisableState();
	else
		UIManager:FindUIButton("BattleProtectB"):SetNormalState();
	end

	if not LuaUI.GetCatchEnable() then
		UIManager:FindUIButton("BattleCatchB"):SetDisableState();
	else
		UIManager:FindUIButton("BattleCatchB"):SetNormalState();
	end

	if not LuaUI.GetAutoEnable() then
		UIManager:FindUIButton("BattleAutoB"):SetDisableState();
	else
		UIManager:FindUIButton("BattleAutoB"):SetNormalState();
	end

	if not LuaUI.GetEscapeEnable() then
		UIManager:FindUIButton("BattleEscapeB"):SetDisableState();
	else
		UIManager:FindUIButton("BattleEscapeB"):SetNormalState();
	end

	UIManager:GetUIItem("BattlePanel"):SetEnable(true);
	UIManager:GetUIItem("BattleWinePanel"):SetEnable(false);
	local partnerProcess = LuaUI.GetPartnerProcess();
	if partnerProcess ~= nil then
	    partnerProcess:UpdateStateButton();
	end
	UIManager:AddDirtyUIItem(UIManager:GetUIItem("BattlePanel"));
	end
end

function ShowBattlePartnerPanel()
	if _G.InBattle then
		UIManager:CloseTips()
		local partnerProcess = LuaUI.GetPartnerProcess();
		if partnerProcess ~= nil then
			if partnerProcess:IsBattleSkillExist() then
				if not LuaUI.GetPartnerSkillEnable() then
					UIManager:FindUIButton("BattlePartnerSkill"):SetDisableState();
				else
					UIManager:FindUIButton("BattlePartnerSkill"):SetNormalState();
				end
			else
				UIManager:FindUIButton("BattlePartnerSkill"):SetDisableState();
			end
		end


		if not LuaUI.GetPartnerItemEnable() then
			UIManager:FindUIButton("BattlePartnerItem"):SetDisableState();
		else
			UIManager:FindUIButton("BattlePartnerItem"):SetNormalState();
		end


		if not LuaUI.GetPartnerDefenceEnable() then
			UIManager:FindUIButton("BattlePartnerDefence"):SetDisableState();
		else
			UIManager:FindUIButton("BattlePartnerDefence"):SetNormalState();
		end
		--UIManager:FindUIButton("BattlePartnerProtect"):SetNormalState();
		if not LuaUI.GetPartnerProtectEnable() then
			UIManager:FindUIButton("BattlePartnerProtect"):SetDisableState();
		else
			UIManager:FindUIButton("BattlePartnerProtect"):SetNormalState();
		end
		--UIManager:FindUIButton("BattlePartnerEscapeB"):SetNormalState();
		if not LuaUI.GetPartnerEscapeEnable() then
			UIManager:FindUIButton("BattlePartnerEscapeB"):SetDisableState();
		else
			UIManager:FindUIButton("BattlePartnerEscapeB"):SetNormalState();
		end
	    UIManager:GetUIItem("BattlePartnerPanel"):SetEnable(true);
		UIManager:AddDirtyUIItem(UIManager:GetUIItem("BattlePartnerPanel"));
		UIManager:GetUIItem("BattlePartnerPanel"):SetPos(725, 160);

		if g_CurrentShortCut == 0 then
			local HShortCutPanel = UIManager:FindUIPanel("HShortCutPanel");
			HShortCutPanel:SetEnable(false)
		elseif g_CurrentShortCut == 1 then
			local VShortCutPanel = UIManager:FindUIPanel("VShortCutPanel");
			VShortCutPanel:SetEnable(false)
		elseif g_CurrentShortCut == 2 then
			local HideShortCutPanel = UIManager:FindUIPanel("HideShortCutPanel");
			HideShortCutPanel:SetEnable(false)
		end

		PerloadBattlePartnerSkillMenu()
	end
end

function ShowWaitPanel()
	UIManager:CloseTips()
	UIManager:GetUIItem("BattleWait"):SetEnable(true);
	UIManager:AddDirtyUIItem(UIManager:GetUIItem("BattleWait"));
end

function EnterBattle()
	local panel = UIManager:GetUIItem("QuestLogNotePanel");
	
	if panel ~= nil then
	UIManager:AddDirtyUIItem(panel);
	panel:SetEnable(false);
	UIManager:DeletePanel("QuestLogNotePanel");
	_G.bNoteOpen = true;
	end

	_G.InBattle = true;
	_G.UseSpecial = false;

	UIManager:CloseTips()

	--VItemFlag = UIManager:GetUIItem("VItem"):IsEnable();

	UIManager:GetUIItem("Char"):SetPos(627, 0);
	UIManager:GetUIItem("Partner"):SetPos(627 + 25, 56);
	UIManager:GetUIItem("MyFace"):SetPos(627 + 20, 6);
	--UIManager:GetUIItem("Enemy"):SetEnable(false)
	UIManager:GetUIItem("MiniMap"):SetEnable(false);
	UIManager:GetUIItem("ClockPanel"):SetEnable(false);
	UIManager:GetUIItem("MapInfoButton"):SetEnable(false);
	UIManager:GetUIItem("Location"):SetEnable(false);
	UIManager:GetUIItem("MapInfo"):SetPos(0, 0);
	UIManager:GetUIItem("Time"):SetEnable(false);
	local AutoWalkpanel = UIManager:FindUIPanel("AutoWalkPanel");
	if AutoWalkpanel then
		UIManager:GetUIItem("AutoWalkPanel"):SetEnable(false);
	end
	--UIManager:GetUIItem("Friend1"):SetEnable(false);
	--UIManager:GetUIItem("Friend2"):SetEnable(false);
	--UIManager:GetUIItem("Friend3"):SetEnable(false);
	--UIManager:GetUIItem("Friend4"):SetEnable(false);
	--UIManager:GetUIItem("VItem"):SetEnable(false);
	--UIManager:GetUIItem("HItem"):SetEnable(false);
	--UIManager:GetUIItem("HroseLight"):SetEnable(false);
	UIManager:GetUIItem("BattlePanel"):SetEnable(true);
	UIManager:GetUIItem("BattlePanel"):SetPos(725, 160);
	UIManager:GetUIItem("BattleWinePanel"):SetEnable(false);

	UIManager:FindUIButton("DownBAttack"):SetDisableState();
	UIManager:FindUIButton("DownBTrade"):SetDisableState();
	UIManager:FindUIButton("DownBGive"):SetDisableState();

	UIManager:FindUIButton("DownBParty"):SetLockState(false);
	UIManager:FindUIButton("DownBParty"):SetDisableState();
	UIManager:FindUIButton("DownBChargeShop"):SetDisableState();
	--UIManager:FindUIButton("DownBItem"):SetDisableState();
	--UIManager:FindUIButton("DownBFriend"):SetDisableState();
	UIManager:FindUIButton("DownBSkill"):SetDisableState();
	--UIManager:FindUIButton("DownBHousen"):SetDisableState();
	
	local petusefood = UIManager:GetUIItem("PetUseFoodPanel");
	if petusefood then
		petusefood:SetEnable(false);
	end

	local petPanel = UIManager:GetUIItem("PetPanel");
	if petPanel then
		petPanel:SetEnable(false);
	end
	
	local uiButton = UIManager:FindUIButton("DownBMission");
	if uiButton ~= nil then
	    _G.MissionButtonState = uiButton:GetState();
	    uiButton:SetLockState(false);
	    uiButton:SetDisableState();
	end
	--UIManager:FindUIButton("DownBMission"):SetDisableState();
	UIManager:FindUIButton("DownBPet"):SetDisableState();

	CloseItemMenu(0, 0);
	CloseGivenPanel();
	--CloseCharacterMenu(0,0);
	CharMenuButtonStateAdjust();
	CloseDialogueBox(0,0);
	CloseTeammateMenu(0,0);
	CloseHawkerOwner();
	CloseMiddleMap();
	CloseNPCSearch();
	CloseShopMenu(0, 0);
	CloseBigMap();
	CloseChangeLine();
	CloseOnlineShop();
	CancelMixShopMaking();
	CloseMixShopMake();
	CloseIronShopMake();
	CloseNewGuide();
	CloseNewArrow();
	CloseHawk();
	CloseWarehousePanel();
	CloseAssetUnlockPanel();
	CloseAssetLockPanel();
	CloseSellYuanBaoChoice();

	CancelMixShopGem();
	CloseMixShopGem();
	CloseWantedPanel();
	CloseWantedRewardPanel();
	CloseWantedBailPanel();
	CloseWantedJailDeliveryPanel();
	CloseWantedTracePanel();
	ClosePartnerCultivationUp();
	CloseBankDepositPanel();
	CloseBankWithdrawalPanel();
	

	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		partnerMenu:UpdatePartnerMenu();
		SwitchOffPartnerSkill();
		SwitchOffPartnerDesc();
		SwitchOffPartnerCollection();
	end
	--ClosePartner();
	--UIManager:DeletePanel("CharacterMenuPanel");
	--UIManager:DeletePanel("TeammatePanel");

	local musicNo = math.random(10000)
	local isInBattle = LuaUI.IsBattle();
	if isInBattle == true then
		local isPVP = LuaUI.IsPvP();
		if isPVP == true then
			if musicNo < 5000 then
				--OggPlayer:ChangeOgg("Data\\music0030_Battle05.ogg");
				FSoundManager:SetMusic("Data\\music0030_Battle05.ogg");
			else
				--OggPlayer:ChangeOgg
				FSoundManager:SetMusic("Data\\music0031_Battle06.ogg");
			end
		else
			if musicNo < 2500 then
				--OggPlayer:ChangeOgg
				FSoundManager:SetMusic("Data\\music0026_Battle01.ogg");
			elseif musicNo < 5000 then
				--OggPlayer:ChangeOgg
				FSoundManager:SetMusic("Data\\music0027_Battle02.ogg");
			elseif musicNo < 7500 then
				--OggPlayer:ChangeOgg
				FSoundManager:SetMusic("Data\\music0028_Battle03.ogg");
			else
				--OggPlayer:ChangeOgg
				FSoundManager:SetMusic("Data\\music0029_Battle04.ogg");
			end
		end
	end

	local targetMenu = LuaUI.GetMenuTargetProcess();
	if targetMenu ~= nil then
		targetMenu:HideTargetMenu();
	end
	ResetKeyIn();

end

function LeaveBattle()
	if 	_G.bNoteOpen == true then
		LuaUI.GetQuestLogProcess():OnUpdateNoteUI();
		_G.bNoteOpen = false;
	end

	_G.InBattle = false;
	_G.UseSpecial = false;
	UIManager:CloseTips()
	UIManager:GetUIItem("Char"):SetPos(0, 0);
	UIManager:GetUIItem("Partner"):SetPos(25, 56);
	UIManager:GetUIItem("MyFace"):SetPos(20, 6);
	--UIManager:GetUIItem("Enemy"):SetEnable(true)
	UIManager:GetUIItem("MiniMap"):SetEnable(true);
	UIManager:GetUIItem("ClockPanel"):SetEnable(false);
	UIManager:GetUIItem("MapInfoButton"):SetEnable(true);
	UIManager:GetUIItem("MapInfo"):SetPos(635, 0);
	UIManager:GetUIItem("Location"):SetEnable(true);
	UIManager:GetUIItem("Time"):SetEnable(true);
local petusefood = UIManager:GetUIItem("PetUseFoodPanel");
	if petusefood then
		petusefood:SetEnable(true);
	end

	local petPanel = UIManager:GetUIItem("PetPanel");
	if petPanel then
		petPanel:SetEnable(true);
	end
	
	CloseBattleItemMenu(0, 0);
    ClosePartnerBattleSkill();

	CharMenuButtonStateAdjust();
	--UIManager:GetUIItem("Friend1"):SetEnable(true);
	--UIManager:GetUIItem("Friend2"):SetEnable(true);
	--UIManager:GetUIItem("Friend3"):SetEnable(true);
	--UIManager:GetUIItem("Friend4"):SetEnable(true);
	--UIManager:GetUIItem("VItem"):SetEnable(VItemFlag);
	--UIManager:GetUIItem("HItem"):SetEnable(not VItemFlag);
	UIManager:GetUIItem("BattlePartnerPanel"):SetEnable(false);
	UIManager:GetUIItem("BattlePanel"):SetEnable(false);
	UIManager:GetUIItem("BattleWinePanel"):SetEnable(false);
	UIManager:GetUIItem("BattleWatchPanel"):SetEnable(false);

	UIManager:FindUIButton("DownBAttack"):SetNormalState();
	UIManager:FindUIButton("DownBTrade"):SetNormalState();
	UIManager:FindUIButton("DownBGive"):SetNormalState();
	local AutoWalkpanel = UIManager:FindUIPanel("AutoWalkPanel");
	if AutoWalkpanel then
		UIManager:GetUIItem("AutoWalkPanel"):SetEnable(true);
	end

	--[[if UIManager:GetUIItem("PartyBlockImage"):IsEnable() then
		UIManager:FindUIButton("DownBParty"):SetMouseOverState();
	else
		UIManager:FindUIButton("DownBParty"):SetNormalState();
	end
	]]
	LuaUI.ShowPartyButton();
	UIManager:FindUIButton("DownBChargeShop"):SetNormalState();
	--UIManager:FindUIButton("DownBItem"):SetNormalState();
	UIManager:FindUIButton("DownBSkill"):SetNormalState();
	--UIManager:FindUIButton("DownBFriend"):SetNormalState();
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		--partnerMenu:ShowPartnerButton();
		partnerMenu:UpdatePartnerMenu();
	end
	--UIManager:FindUIButton("DownBHousen"):SetDisableState();
	local uiButton = UIManager:FindUIButton("DownBMission");
	if uiButton ~= nil then
	    if _G.MissionButtonState == 4 then
            uiButton:SetNoticeState();
            uiButton:SetLockState(true);
	    else
	        uiButton:SetNormalState();
		end
	    _G.MissionButtonState = 0;

	end
	--UIManager:FindUIButton("DownBMission"):SetNormalState();
	--UIManager:FindUIButton("DownBFriendShip"):SetDisableState();
	UIManager:FindUIButton("DownBPet"):SetNormalState();

	UIManager:GetUIItem("BattleWait"):SetEnable(false);

	--OggPlayer:ChangeOgg
	--FSoundManager:SetMusic("Data\\music0009_Field01.ogg");

end


function EditLineOnChar(KeyCode)
	if KeyCode == LuaUI.VK_RETURN then
		local EditString = EditBox:GetText()

		--?¡È?¸G
		--"\139\149\203\89\48\98\37\155\0\0",

		if not EditBox:Empty() then
			MyChat:UnLockChatPanel();
			EditBox:ClearText();
			UIManager:AddDirtyUIItem(UIManager:GetUIItem("EditLine"));
		end
	end
end

function ChatScrollUp(X, Y)
	MyChat:UpRow();
	MyChat:LockChatPanel();
end


function ChatScrollDown(X, Y)
	MyChat:DownRow();
	MyChat:LockChatPanel();
	if MyChat:IsEndRow() then
	    MyChat:UnLockChatPanel();
		MyChat:EndRow();
	end
end


function OKBTNOnLButtonUp(pX, pY)
	Beep()
end


function ScrollLockDown(pX, pY)
	local box = UIManager:FindUICheckBox("ScrollLockButton");
	MyChat:SetEnableScroll(not box:GetState())
end

function EndRowDown(pX, pY)
	MyChat:UnLockChatPanel();
	MyChat:EndRow();
end

function ClearMsgDown(pX, pY)
	MyChat:ClearAllRow();
	LuaUI.GetChatPool():ResetCopyIndex();
	UIManager:RenderAll();
end


function AddChatRow(pX, pY)
	local ChatPanel = UIManager:FindUIPanelEx("ChatPanel")
	local LineCount = ChatPanel:GetLineCount();
	local ChatUpRowBtn = UIManager:FindUIButton("ChatUpRow")

	if LineCount < 22 then
		ChatPanel:ChangeCount(LineCount + 1)
		ChatPanel:SetPos(ChatPanel:GetX(), ChatPanel:GetY() - 17)
		ChatPanel:SetDetectRect();

		--ChatUpRowBtn:SetPos(ChatUpRowBtn:GetX(), ChatUpRowBtn:GetY() - 17)
		--ChatUpRowBtn:SetDetectRect();
		MyChat:SetImage(MyChat:GetWidth(), MyChat:GetHeight() + 17)
		MyChat:SetDetectRect();
		MyChat:EndRow();
		
		local SpeakerPanel = UIManager:FindUIPanelEx("SpeakerPanel")
		if SpeakerPanel ~= nil and ChatPanel ~= nil then
			SpeakerPanel:SetPos(SpeakerPanel:GetX(), ChatPanel:GetY() - 60)
			SpeakerPanel:SetDetectRect();
		end

		UIManager:RenderAll();
	end
end


function CutChatRow(pX, pY)
	local ChatPanel = UIManager:FindUIPanelEx("ChatPanel")
	local LineCount = ChatPanel:GetLineCount();
	local ChatUpRowBtn = UIManager:FindUIButton("ChatUpRow")

	if LineCount > 9 then
		ChatPanel:ChangeCount(LineCount - 1)
		ChatPanel:SetPos(ChatPanel:GetX(), ChatPanel:GetY() + 17)
		ChatPanel:SetDetectRect();

		--ChatUpRowBtn:SetPos(ChatUpRowBtn:GetX(), ChatUpRowBtn:GetY() - 17)
		--ChatUpRowBtn:SetDetectRect();
		MyChat:SetImage(MyChat:GetWidth(), MyChat:GetHeight() - 17)
		MyChat:SetDetectRect();
		MyChat:EndRow();
		
		local SpeakerPanel = UIManager:FindUIPanelEx("SpeakerPanel")
		if SpeakerPanel ~= nil and ChatPanel ~= nil then
			SpeakerPanel:SetPos(SpeakerPanel:GetX(), ChatPanel:GetY() - 60)
			SpeakerPanel:SetDetectRect();
		end

		UIManager:RenderAll();
	end

end


function ChatWheelProcess(pX, pY, pZ)
	if pZ > 0 then
		ChatScrollUp(pX, pY)
	else
		ChatScrollDown(pX, pY)
	end
end



function ChatLinkProcess(pX, pY)
	local ChatLink = MyChat:GetLinkItem()
	local FloatNamePanel = UIManager:FindUIPanel("FloatNamePanel")

	if ChatLink ~= nil and FloatNamePanel ~= nil then
		if ChatLink:GetLinkType() == 1 then
			local Float = UIManager:FindUIChatPanel("FloatNameStr")
			FloatNamePanel:SetEnable(true)
			Float:ClearAllRow();
			g_LinkString = ChatLink:GetLinkSubString(5)
			Float:AddStringW(g_LinkString);
			Float:HomeRow()
			Float:Redraw()


			local Row = Float:GetHomeRow();
			local Width = Row:GetWidth() + 2;

			FloatNamePanel:SetWH(Width, 20);
			FloatNamePanel:SetTextureWH(Width, 20);
			FloatNamePanel:SetTextureUI(UIManager);
			FloatNamePanel:SetDetectRect();

			UIManager:RenderAll();
			UIManager:SetDragItemXY(FloatNamePanel, 0,  0);
			UIManager:AddDirtyUIItem(FloatNamePanel);
		elseif ChatLink:GetLinkType() == 3 then
			LuaUI.CharSetMapPos(ChatLink:GetLinkSubString(5));
		elseif ChatLink:GetLinkType() == 4 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local ItemID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);

			local itemProcess = LuaUI.GetItemProcess();
			if itemProcess ~= nil then
				itemProcess:ShowPublicItemDetail(CharID, ItemID);
			end
		elseif ChatLink:GetLinkType() == 5 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local PartnerID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);

			local PartnerProcess = LuaUI.GetPartnerProcess();
			if  PartnerProcess ~= nil then
				 PartnerProcess:ShowPublicPartnerDetail(CharID, PartnerID);
			end
		elseif ChatLink:GetLinkType() == 6 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local PetID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);

			local PetProcess = LuaUI.GetMenuPetProcess();
			if  PetProcess ~= nil then
				 PetProcess:ShowPublicPetDetail(CharID, PetID);
			end
		end
	end
end


function ChatLinkProcessR(pX, pY)
	local ChatLink = MyChat:GetLinkItem()
	local FloatNamePanel = UIManager:FindUIPanel("FloatNamePanel")

	if ChatLink ~= nil and FloatNamePanel ~= nil then
		if ChatLink:GetLinkType() == 1 then
			local menu = LuaUI.GetMenuTargetProcess();
			g_LinkString = ChatLink:GetLinkSubString(5)
			menu:FindCharNameFriend(g_LinkString);

		elseif ChatLink:GetLinkType() == 4 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local ItemID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);

			local itemProcess = LuaUI.GetItemProcess();
			if itemProcess ~= nil then
				itemProcess:ShowPublicItemDetail(CharID, ItemID);
			end
		end
	end
end

function SpeakerLinkProcess(pX, pY)
	local ChatLink = MySpeaker:GetLinkItem()
	local FloatNamePanel = UIManager:FindUIPanel("FloatNamePanel")

	if ChatLink ~= nil and FloatNamePanel ~= nil then
		if ChatLink:GetLinkType() == 1 then
			local Float = UIManager:FindUIChatPanel("FloatNameStr")
			FloatNamePanel:SetEnable(true)
			Float:ClearAllRow();
			g_LinkString = ChatLink:GetLinkSubString(5)
			Float:AddStringW(g_LinkString);
			Float:HomeRow()
			Float:Redraw()


			local Row = Float:GetHomeRow();
			local Width = Row:GetWidth() + 2;

			FloatNamePanel:SetWH(Width, 20);
			FloatNamePanel:SetTextureWH(Width, 20);
			FloatNamePanel:SetTextureUI(UIManager);
			FloatNamePanel:SetDetectRect();

			UIManager:RenderAll();
			UIManager:SetDragItemXY(FloatNamePanel, 0,  0);
			UIManager:AddDirtyUIItem(FloatNamePanel);
		elseif ChatLink:GetLinkType() == 3 then
			LuaUI.CharSetMapPos(ChatLink:GetLinkSubString(5));
		elseif ChatLink:GetLinkType() == 4 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local ItemID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);

			local itemProcess = LuaUI.GetItemProcess();
			if itemProcess ~= nil then
				itemProcess:ShowPublicItemDetail(CharID, ItemID);
			end
		elseif ChatLink:GetLinkType() == 5 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local PartnerID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);

			local PartnerProcess = LuaUI.GetPartnerProcess();
			if  PartnerProcess ~= nil then
				 PartnerProcess:ShowPublicPartnerDetail(CharID, PartnerID);
			end
		elseif ChatLink:GetLinkType() == 6 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local PetID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);

			local PetProcess = LuaUI.GetMenuPetProcess();
			if  PetProcess ~= nil then
				 PetProcess:ShowPublicPetDetail(CharID, PetID);
			end
		end
	end
end


function SpeakerLinkProcessR(pX, pY)
	local ChatLink = MySpeaker:GetLinkItem()
	local FloatNamePanel = UIManager:FindUIPanel("FloatNamePanel")

	if ChatLink ~= nil and FloatNamePanel ~= nil then
		if ChatLink:GetLinkType() == 1 then
			local menu = LuaUI.GetMenuTargetProcess();
			g_LinkString = ChatLink:GetLinkSubString(5)
			menu:FindCharNameFriend(g_LinkString);

		elseif ChatLink:GetLinkType() == 4 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local ItemID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);

			local itemProcess = LuaUI.GetItemProcess();
			if itemProcess ~= nil then
				itemProcess:ShowPublicItemDetail(CharID, ItemID);
			end
		end
	end
end



--[[
function ChatLinkProcess(pX, pY)
	local ChatLink = MyChat:GetLinkItem()
	local EditBox = UIManager:FindUIEditBox("EditLine")
	if ChatLink ~= nil then
		EditBox:AddText("/p ")
		EditBox:AddText(ChatLink:GetLinkSubString(5))
		EditBox:AddText(" ")
		EditBox:Redraw();
		UIManager:RenderAll();
		MyChat:ClearLinkItem();
	end
end
---]]

function CapKeyUp(Key1, Key2)

end


--function TestPop(pX, pY)
--	local	PopSel = MyPop:GetBarPos()
--
--	if PopSel ~= -1 then
--		local MyRow = MyChat:GetSafeLastRow()
--
--		MyRow:AddStringW(
--			MyPop:GetBarString(),
--			LuaUI.HexToDWORD("FFFF0000"),
--			LuaUI.HexToDWORD("FF000000"),
--			LuaUI.EDGE_SHADOW,
--			MyChat:GetFontHeight());
--		MyChat:EndRow();
--		MyChat:CreateRow();
--		EditBox:ClearText();
--		UIManager:RenderAll();
--	end
--end

function TestMenu(pX, pY)
	local PopPanel = UIManager:FindUIPanel("ColorPanel")

	if PopPanel ~= nil then
		UIManager:DeletePanel("ColorPanel")
		UIManager:RenderAll()
	else
		--[[
		local MyRow = MyChat:GetSafeLastRow()
		MyRow:AddStringA(
				"\135\81\112\0\111\0\112\0\0\0",      --?pop
				LuaUI.HexToDWORD("FFFFFFFF"),
				LuaUI.HexToDWORD("FF000000"),
				LuaUI.EDGE_SHADOW,
				MyChat:GetFontHeight());
		MyChat:CreateRow();
		MyChat:EndRow();
		--]]

		LuaUI.AddXMLFile2UIManger("Data\\PopPanel.xml");
		UIManager:RenderAll();
	end
end


function TalkPanel(pX, pY, msg)
	local PopPanel = UIManager:FindUIPanel("TalkPanel")

	if PopPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\TalkPanel.xml");

		local debugmsg = UIManager:FindUIStaticText("TalkMessage");
		if debugmsg ~= nil then
			debugmsg:LuaSetText(msg);
			debugmsg:Redraw();
		end

		UIManager:RenderAll();
	end

end

function RemoveTestDebug(pX, pY)
		local PopPanel = UIManager:FindUIPanel("TalkPanel")

		if PopPanel ~= nil then
			PopPanel:SetEnable(false);
			UIManager:DeletePanel("TalkPanel")
			UIManager:RenderAll();
		end
end


function PopColorPanel(pX, pY)
	local PopPanel = UIManager:FindUIPanel("ColorPanel")

	if PopPanel ~= nil then
		UIManager:DeletePanel("ColorPanel")
	else
		LuaUI.AddXMLFile2UIManger("Data\\PopPanel.xml");
		PopPanel = UIManager:FindUIPanel("ColorPanel");
		PopPanel:SetPos(290, 460);
	end
end


function ChangeColor(pX, pY)
	local ColorPanel = UIManager:FindUIPanel("Color" .. "Panel")

	local Index = LuaUI.Div(pY - ColorPanel:GetY() - 3, 13) + 1
	LuaUI.ChangeEditLineColor(Index);
end

function OpenSubWindow(pX, pY)
	if LuaUI.IsFullScreen() == false then
		local systemSetting = LuaUI.GetMenuSystemSetting();
		if systemSetting ~= nil then
		   systemSetting:SetSubChatEnable(true);
		end
		LuaUI.OpenSubWindow()
		UIManager:GetUIItem("ChatPanel"):SetEnable(false);
		UIManager:GetUIItem("ChatButtonArea"):SetEnable(false);
		UIManager:GetUIItem("ChatRightButtonArea"):SetEnable(false);
		UIManager:RenderAll();
		InitSubWindow();
		SubEndRow();
		SubWindowFlag = true;
		local SpeakerPanel = UIManager:FindUIPanelEx("SpeakerPanel")
		if SpeakerPanel ~= nil then
			SpeakerPanel:SetPos(SpeakerPanel:GetX(), 495)
			SpeakerPanel:SetDetectRect();
		end
	end
end

function CloseSubWindow(pX, pY)
	UIManager:SetEvent(101)
	local SpeakerPanel = UIManager:FindUIPanelEx("SpeakerPanel")
	local ChatPanel = UIManager:FindUIPanelEx("ChatPanel")
	if SpeakerPanel ~= nil and ChatPanel ~= nil then
		SpeakerPanel:SetPos(SpeakerPanel:GetX(), ChatPanel:GetY() - 60)
		SpeakerPanel:SetDetectRect();
	end
end

function InitMainChatPanel()
	local MyPool = LuaUI.GetChatPool()
	local CopyIndex = MyPool:GetCopyIndex();
	MyChat:ClearAllRow()
	if MyPool ~= nil then
		local MsgHistory = MyPool:GetStartMsg();
		while MsgHistory do
			if MyPool:GetIndex() >= CopyIndex then
				MyChat:AddStringW(MsgHistory);
			end
			MsgHistory  = MyPool:GetNextMsg();
		end
		MyChat:EndRow();
	end
end
function CloseSubWindowNow()
	if _G.CloseAllPanel ~= true then
		LuaUI.CloseSubWindow()
		local systemSetting = LuaUI.GetMenuSystemSetting();
		if systemSetting ~= nil then
		   systemSetting:SetSubChatEnable(false);
		end
		UIManager:GetUIItem("ChatPanel"):SetEnable(true);
		UIManager:GetUIItem("ChatButtonArea"):SetEnable(true);
		UIManager:GetUIItem("ChatRightButtonArea"):SetEnable(true);
		UIManager:GetUIItem("ChatDownRow"):SetEnable(true);
		UIManager:GetUIItem("ScrollLockButton"):SetEnable(true);
		UIManager:GetUIItem("ClearMsg"):SetEnable(true);
		UIManager:FindUIButton("SubWindow"):SetNormalState();
		InitMainChatPanel();
		EndRowDown();
		UIManager:RenderAll();
		SubWindowFlag = false;
	end

end


function  BattleAction()
	LuaUI.BattleAction()
end

function  BattleSkill()
	_G.UseSpecial = false;
	LuaUI.BattleSkill();
end

function  BattleSpecial()
	_G.UseSpecial = true;
	LuaUI.BattleSpecial();
end

function  BattleItem()
	LuaUI.BattleItem()
end


function  BattleDefence()
	LuaUI.BattleDefence()
end


function  BattleFriend()
	--LuaUI.BattleFriend()
	OpenPartner();
end


function  BattleProtect()
	LuaUI.BattleProtect()
end


function  BattleCatch()
    HideBattlePanel();
	LuaUI.BattleCatch();

end


function  BattleHelp()
	LuaUI.BattleHelp()
end


function  BattleAuto()
	LuaUI.BattleAuto()
end


function  BattleEscape()
	LuaUI.BattleEscape()
end

function  BattleJoin()
	LuaUI.BattleJoin()
end


function  BattleWatchLeave()
	LuaUI.BattleWatchLeave()
end

function PrintMessage()
	local EditString = EditBox:GetText()
		if not EditBox:Empty() then
			local MyPool = LuaUI.GetChatPool()
			MyPool:Add(EditBox:GetText())
			local SubWinUIManager = LuaUI.GetSubUIManager()
			if SubWinUIManager ~= nil then
				local SubChat = SubWinUIManager:FindUIChatPanel("SubChat");
				SubChat:AddStringW(EditBox:GetText());
				SubChat:EndRow();

				local ScrollBar = SubWinUIManager:FindUIScrollBarV("SubChatScrollButton")
				ScrollBar:SetNumCount(SubChat:RowListCount())
				scrollBar:SetSelNum(SubChat:RowListCount() - 1);
				SubWinUIManager:RenderAll();
			else
				MyChat:AddStringW(EditBox:GetText())
				MyChat:EndRow();

			end
			EditBox:ClearText();
		UIManager:RenderAll();
	end
end

function HideChatWindow()

		local chatPanel = UIManager:GetUIItem("ChatPanel")
		if chatPanel ~= nil then
			if chatPanel:IsEnable() then
				UIManager:GetUIItem("ChatDownRow"):SetEnable(false);
				UIManager:GetUIItem("ScrollLockButton"):SetEnable(false);
				UIManager:GetUIItem("ClearMsg"):SetEnable(false);
				chatPanel:SetEnable(false);
			else
				UIManager:GetUIItem("ChatDownRow"):SetEnable(true);
				UIManager:GetUIItem("ScrollLockButton"):SetEnable(true);
				UIManager:GetUIItem("ClearMsg"):SetEnable(true);
				chatPanel:SetEnable(true);
			end
		end

		UIManager:RenderAll();

end

function SetCloseAllPanelFlag(close)
	_G.CloseAllPanel = close;
end

function SetEditLineFocus()
    UIManager:SetKeyInItem("EditLine");
end

function BattlePartnerEscape()
	LuaUI.BattlePartnerEscape();
end


function PerloadBattlePartnerSkillMenu()
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		if partnerMenu:IsBattleSkillExist() then
			local panel = UIManager:GetUIItem("PartnerBattleSkillPanel");
			local temp = UIManager:FindUIPanel("PartnerBattleSkillPanel");
			if panel == nil then
				LuaUI.AddXMLFile2UIManger("Data\\MenuPartnerSkill.xml");
				panel = UIManager:GetUIItem("PartnerBattleSkillPanel");
				temp = UIManager:FindUIPanel("PartnerBattleSkillPanel");
			end

			if panel ~= nil then
				panel = UIManager:GetUIItem("PartnerBattleSkillShortCut");
				if panel ~= nil then
					partnerMenu:UpdateBattleSkillMenu();
					panel:SetEnable(true);
				end
			end
		end
	end
end


function OpenBattlePartnerSkillMenu()
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		if partnerMenu:IsBattleSkillExist() then
			HideBattlePartnerPanel();
			local panel = UIManager:GetUIItem("PartnerBattleSkillPanel");
			local temp = UIManager:FindUIPanel("PartnerBattleSkillPanel");
			if panel == nil then
				LuaUI.AddXMLFile2UIManger("Data\\MenuPartnerSkill.xml");
				panel = UIManager:GetUIItem("PartnerBattleSkillPanel");
				temp = UIManager:FindUIPanel("PartnerBattleSkillPanel");
			end

			if panel ~= nil then
				panel:SetPos(400 - panel:GetWidth() / 2, 300 - panel:GetHeight() );
				partnerMenu:UpdateBattleSkillMenu();
				--UIManager:SetMonoPanel(temp);
				panel:SetEnable(true);

				panel = UIManager:GetUIItem("PartnerBattleSkillShortCut");
				if panel ~= nil then
					panel:SetEnable(true);
				end
			end
		end
	end
end

function BuffDetailPopupTimeOut()
	local partnerMenu = LuaUI.GetMenuBuffIconProcess();
	local index = -1;
	for i = 1, 25  do
		local name = string.format("BuffIcon%d", i);
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();
		if UIManager:Detect(name, mx, my) then
			index = i;
			local panel = UIManager:GetUIItem("BuffIconPanel");
			panel:SetEnableTimeOutEvent(false);
			break;
		end
	end
	partnerMenu:OpenBuffDetail(index);
end

function BuffIconIn()
	local panel = UIManager:GetUIItem("BuffIconPanel");
	panel:SetEnableTimeOutEvent(true);
end

function BuffIconOut()
	local panel = UIManager:GetUIItem("BuffIconPanel");
	panel:SetEnableTimeOutEvent(false);
	local partnerMenu = LuaUI.GetMenuBuffIconProcess();
	partnerMenu:HideBuffDetail();
end

g_CurrentShortCut = 0;

function EnableVShortCut(px, py)
	--[[
	if LuaUI.LuaGetBPShortCutFlag() then
		return
	end
	local HShortCutPanel = UIManager:FindUIPanel("HShortCutPanel");
	local VShortCutPanel = UIManager:FindUIPanel("VShortCutPanel");
	local HideShortCutPanel = UIManager:FindUIPanel("HideShortCutPanel");

	local x = HShortCutPanel:GetX();
	local y = HShortCutPanel:GetY();

	HShortCutPanel:SetEnable(false)
	VShortCutPanel:SetEnable(true)
	HideShortCutPanel:SetEnable(false)

	local ShortCutPanel = VShortCutPanel:GetUIItemPtr()
	for i = 0, 9  do
		local UIName = "HShortCut" .. i
		local ShortCut = UIManager:FindUIAniIcon(UIName)
		ShortCut:ChangeParent(ShortCutPanel)
		ShortCut:SetOffsetXY(3, 4 + 35 * i)
		ShortCut:SetDetectRect()

		UIName = "ShortCutF" .. i + 1
		local FuncKey = UIManager:FindUIImage(UIName)
		FuncKey:ChangeParent(ShortCutPanel)
		FuncKey:SetOffsetXY(3, 1 + 35 * i)
		FuncKey:SetDetectRect()

		UIName = "HShortText" .. i
		local Text = UIManager:FindUIStaticText(UIName)
		Text:ChangeParent(ShortCutPanel)
		Text:SetOffsetXY(3, 22 + 35 * i)
		Text:SetDetectRect()
	end
	VShortCutPanel:SetPos(x + 334, y - 335)
	g_CurrentShortCut = 1;
	--]]
end

function EnableHideShortCut(px, py)
	if LuaUI.LuaGetBPShortCutFlag() then
		return
	end
	local HShortCutPanel = UIManager:FindUIPanel("HShortCutPanel");
	local VShortCutPanel = UIManager:FindUIPanel("VShortCutPanel");
	local HideShortCutPanel = UIManager:FindUIPanel("HideShortCutPanel");

	local x = VShortCutPanel:GetX();
	local y = VShortCutPanel:GetY();

	HideShortCutPanel:SetPos(x - 3, y + 340)
	HShortCutPanel:SetEnable(false)
	VShortCutPanel:SetEnable(false)
	HideShortCutPanel:SetEnable(true)
	g_CurrentShortCut = 2;
end

function EnableHShortCut(px, py)
	if LuaUI.LuaGetBPShortCutFlag() then
		return
	end
	local HShortCutPanel = UIManager:FindUIPanel("HShortCutPanel");
	local VShortCutPanel = UIManager:FindUIPanel("VShortCutPanel");
	local HideShortCutPanel = UIManager:FindUIPanel("HideShortCutPanel");

	local x = HideShortCutPanel:GetX();
	local y = HideShortCutPanel:GetY();

	HShortCutPanel:SetEnable(true)
	VShortCutPanel:SetEnable(false)
	HideShortCutPanel:SetEnable(false)

	--[[
	local ShortCutPanel = HShortCutPanel:GetUIItemPtr()
	for i = 0, 9  do
		local UIName = "HShortCut" .. i
		local ShortCut = UIManager:FindUIAniIcon(UIName)
		ShortCut:ChangeParent(ShortCutPanel)
		ShortCut:SetOffsetXY(3 + 35 * i, 4)

		UIName = "ShortCutF" .. i + 1
		local FuncKey = UIManager:FindUIImage(UIName)
		FuncKey:ChangeParent(ShortCutPanel)
		FuncKey:SetOffsetXY(3 + 35 * i, 1)

		UIName = "HShortText" .. i
		local Text = UIManager:FindUIStaticText(UIName)
		Text:ChangeParent(ShortCutPanel)
		Text:SetOffsetXY(3 + 35 * i, 22)
	end
	--]]
	HShortCutPanel:SetPos(x - 355, y - 2)
	g_CurrentShortCut = 0;
end

function FloatNameDragEnd(value)

	local DropPanel = UIManager:GetDropPanel();
	local BottomPanel = UIManager:GetUIItem("Bottom");


	if DropPanel ~= nil and DropPanel:Equal(BottomPanel) then
		local EditBox = UIManager:FindUIEditBox("EditLine")
		local Focus = UIManager:GetFocusItem()
		if Focus ~= nil and Focus:CheckName("EditLine") then
			EditBox:ClearWaitText()
			EditBox:AddWaitText("\15 /p ")
			EditBox:AddWaitText(g_LinkString)
			EditBox:AddWaitText(" \16\17\19[")
			EditBox:AddWaitText(g_LinkString)
			EditBox:AddWaitText("]\19\18")
			EditBox:SendWaitText()

			--[[
			EditBox:AddText("\015 /p ")
			EditBox:AddText(g_LinkString)
			EditBox:AddText(" \16\17\19[")
			EditBox:AddText(g_LinkString)
			EditBox:AddText("]\19\18")
			--]]
			EditBox:Redraw();
			UIManager:RenderAll();
			g_LinkString = nil
		end
	end

	FloatNameReset()
end


function FloatNameReset()
	UIManager:ResetDragItem();
	UIManager:StopDrag();

	local movingPanel = UIManager:GetUIItem("FloatNamePanel");

	if movingPanel ~= nil then
		movingPanel:SetPos(0,0);
		movingPanel:SetEnable(false);
		movingPanel:SetNotDetected(false);
	end

	local SubWinUIManager = LuaUI.GetSubUIManager()
	SubWinUIManager:ResetDragItem();
	SubWinUIManager:StopDrag();

	movingPanel = SubWinUIManager:GetUIItem("SubFloatNamePanel");

	if movingPanel ~= nil then
		movingPanel:SetPos(0,0);
		movingPanel:SetEnable(false);
		movingPanel:SetNotDetected(false);
	end
end

function FaceOnOff(Face, Flag)
	local MyFace = UIManager:GetUIItem(Face);

	if MyFace ~= nil then
		MyFace:SetEnable(Flag);
	end
end

function MyFaceEffectOn()
	FaceOnOff("MouseOnMyFace", true);
end


function MyFaceEffectOff()
	FaceOnOff("MouseOnMyFace", false);
end


function PartnerFaceEffectOn()
	FaceOnOff("MouseOnPartnerFace", true);
end


function PartnerFaceEffectOff()
	FaceOnOff("MouseOnPartnerFace", false);
end

function OpenStringReply()
	local panel = UIManager:FindUIPanel("MenuStringReplyPanel");

	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuStringReply.xml");
		UIManager:SetKeyInItem("StringReplyEdit");
	else
		CloseStringReply();
	end
end

function ConfirmStringReply()
	LuaUI.ConfirmReplyString();
	UIManager:DeletePanel("MenuStringReplyPanel");
	UIManager:SetKeyInItem("EditLine");
end
function CloseStringReply()
	LuaUI.ReplyStringNULL();
	UIManager:DeletePanel("MenuStringReplyPanel");
	UIManager:SetKeyInItem("EditLine");
end

function EnterQuestion()

	UIManager:CloseTips()


	UIManager:GetUIItem("Char"):SetPos(627, 0);
	--UIManager:GetUIItem("Partner"):SetPos(627 + 25, 56);
	UIManager:GetUIItem("Partner"):SetEnable(false);
	UIManager:GetUIItem("MyFace"):SetPos(627 + 20, 6);
	UIManager:GetUIItem("MiniMap"):SetEnable(false);
	UIManager:GetUIItem("ClockPanel"):SetEnable(false);
	UIManager:GetUIItem("MapInfoButton"):SetEnable(false);
	UIManager:GetUIItem("Location"):SetEnable(false);
	UIManager:GetUIItem("MapInfo"):SetPos(0, 0);
	UIManager:GetUIItem("Time"):SetEnable(false);
	--UIManager:GetUIItem("Friend1"):SetEnable(false);
	--UIManager:GetUIItem("Friend2"):SetEnable(false);
	--UIManager:GetUIItem("Friend3"):SetEnable(false);
	--UIManager:GetUIItem("Friend4"):SetEnable(false);
	--UIManager:GetUIItem("VItem"):SetEnable(false);
	--UIManager:GetUIItem("HItem"):SetEnable(false);
	--UIManager:GetUIItem("HroseLight"):SetEnable(false);
	--UIManager:GetUIItem("BattlePanel"):SetEnable(true);
	UIManager:GetUIItem("BattleWinePanel"):SetEnable(false);

	UIManager:FindUIButton("DownBAttack"):SetDisableState();
	UIManager:FindUIButton("DownBTrade"):SetDisableState();
	UIManager:FindUIButton("DownBGive"):SetDisableState();

	UIManager:FindUIButton("DownBParty"):SetLockState(false);
	UIManager:FindUIButton("DownBParty"):SetDisableState();
	UIManager:FindUIButton("DownBChargeShop"):SetDisableState();
	UIManager:FindUIButton("DownBItem"):SetDisableState();
	--UIManager:FindUIButton("DownBFriend"):SetDisableState();
	UIManager:FindUIButton("DownBSkill"):SetDisableState();
	UIManager:FindUIButton("DownBHousen"):SetDisableState();

	local uiButton = UIManager:FindUIButton("DownBMission");
	if uiButton ~= nil then
	    _G.MissionButtonState = uiButton:GetState();
	    uiButton:SetLockState(false);
	    uiButton:SetDisableState();
	end
	--UIManager:FindUIButton("DownBMission"):SetDisableState();
	UIManager:FindUIButton("DownBPet"):SetDisableState();

	CloseItemMenu(0, 0);
	CloseGivenPanel();
	--CloseCharacterMenu(0,0);
	CharMenuButtonStateAdjust();
	CloseDialogueBox(0,0);
	CloseTeammateMenu(0,0);
	CloseHawkerOwner();
	CloseMiddleMap();
	CloseNPCSearch();
	CloseShopMenu(0, 0);
	CloseBigMap();
	CloseChangeLine();
	CloseOnlineShop();
	CancelMixShopMaking();
	CloseMixShopMake();
	CloseIronShopMake();
	CloseNewGuide();
	CloseNewArrow();
	CloseQuestJournalPanel();
	CloseWarehousePanel();
	CancelMixShopGem();
	CloseMixShopGem();
	ClosePartnerCultivationUp();

	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		partnerMenu:UpdatePartnerMenu();
		SwitchOffPartnerSkill();
		SwitchOffPartnerDesc();
	end
	--ClosePartner();
	--UIManager:DeletePanel("CharacterMenuPanel");
	--UIManager:DeletePanel("TeammatePanel");

	local targetMenu = LuaUI.GetMenuTargetProcess();
	if targetMenu ~= nil then
		targetMenu:HideTargetMenu();
	end
	ResetKeyIn();

end

function ExitQuestion()
	UIManager:CloseTips()
	UIManager:GetUIItem("Char"):SetPos(0, 0);
	--UIManager:GetUIItem("Partner"):SetPos(25, 56);
	UIManager:GetUIItem("Partner"):SetEnable(true);
	UIManager:GetUIItem("MyFace"):SetPos(20, 6);
	--UIManager:GetUIItem("Enemy"):SetEnable(true)
	UIManager:GetUIItem("MiniMap"):SetEnable(true);
	UIManager:GetUIItem("ClockPanel"):SetEnable(false);
	UIManager:GetUIItem("MapInfoButton"):SetEnable(true);
	UIManager:GetUIItem("MapInfo"):SetPos(635, 0);
	UIManager:GetUIItem("Location"):SetEnable(true);
	UIManager:GetUIItem("Time"):SetEnable(true);

	CloseBattleItemMenu(0, 0);
    ClosePartnerBattleSkill();

	CharMenuButtonStateAdjust();
	--UIManager:GetUIItem("Friend1"):SetEnable(true);
	--UIManager:GetUIItem("Friend2"):SetEnable(true);
	--UIManager:GetUIItem("Friend3"):SetEnable(true);
	--UIManager:GetUIItem("Friend4"):SetEnable(true);
	--UIManager:GetUIItem("VItem"):SetEnable(VItemFlag);
	--UIManager:GetUIItem("HItem"):SetEnable(not VItemFlag);
	UIManager:GetUIItem("BattlePartnerPanel"):SetEnable(false);
	UIManager:GetUIItem("BattlePanel"):SetEnable(false);
	UIManager:GetUIItem("BattleWinePanel"):SetEnable(false);
	UIManager:GetUIItem("BattleWatchPanel"):SetEnable(false);

	UIManager:FindUIButton("DownBAttack"):SetNormalState();
	UIManager:FindUIButton("DownBTrade"):SetNormalState();
	UIManager:FindUIButton("DownBGive"):SetNormalState();

	--[[if UIManager:GetUIItem("PartyBlockImage"):IsEnable() then
		UIManager:FindUIButton("DownBParty"):SetMouseOverState();
	else
		UIManager:FindUIButton("DownBParty"):SetNormalState();
	end
	]]
	LuaUI.ShowPartyButton();
	UIManager:FindUIButton("DownBChargeShop"):SetNormalState();
	UIManager:FindUIButton("DownBItem"):SetNormalState();
	UIManager:FindUIButton("DownBSkill"):SetNormalState();
	--UIManager:FindUIButton("DownBFriend"):SetNormalState();
	local partnerMenu = LuaUI.GetPartnerProcess();
	if partnerMenu ~= nil then
		--partnerMenu:ShowPartnerButton();
		partnerMenu:UpdatePartnerMenu();
	end
	UIManager:FindUIButton("DownBHousen"):SetNormalState();
	local uiButton = UIManager:FindUIButton("DownBMission");
	if uiButton ~= nil then
	    if _G.MissionButtonState == 4 then
            uiButton:SetNoticeState();
            uiButton:SetLockState(true);
	    else
	        uiButton:SetNormalState();
		end
	    _G.MissionButtonState = 0;

	end
	--UIManager:FindUIButton("DownBMission"):SetNormalState();
	--UIManager:FindUIButton("DownBFriendShip"):SetDisableState();
	UIManager:FindUIButton("DownBPet"):SetNormalState();

	UIManager:GetUIItem("BattleWait"):SetEnable(false);

	--OggPlayer:ChangeOgg
	--FSoundManager:SetMusic("Data\\music0009_Field01.ogg");

end
