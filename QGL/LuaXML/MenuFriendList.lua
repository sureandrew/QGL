FriendSelIndex = -1;
FriendSelPreIndex = -1;
FriendChatIconIndex = -1;
FriendChatIconSelIndex = -1;
FriendDragNameIndex = -1;
FriendOnShowingIndex = -1;

FriendChatIconStartIndex = 0;
FriendChatIconTotalNum = 0;
local FriendChatIconRowCount = 6;

function OpenFriendList()
	local button = UIManager:FindUIButton("DownBFriendShip");
	local Menu = LuaUI.GetMenuFriendProcess();
	if button ~= nil and Menu:HasNextMessage() then
		FriendChatNextLua();
	else
		local panel = UIManager:GetUIItem("MessagerPanel");
		if panel == nil then
			LuaUI.AddXMLFile2UIManger("Data\\MenuFriendList.xml");
			FriendListGroup1Lua();
		else
			if panel:IsEnable() then
				CloseFriendList();
			else
				panel:SetEnable(true);
				FriendListGroup1Lua();
			end
		end
	end
end

function OpenFriendChat()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:OpenFriendChat(FriendSelIndex);
	_G.FriendOnShowingIndex = _G.FriendSelIndex;
	FriendDragEnd();
end
function OpenFriendMsgPanel()
	local panel = UIManager:GetUIItem("FriendChatPanel");
	if panel == nil then
		local Menu = LuaUI.GetMenuFriendProcess();
		LuaUI.AddXMLFile2UIManger("Data\\MenuFriendChat.xml");
		UIManager:SetKeyInItem("ChatEditLine");
	else
		local Menu = LuaUI.GetMenuFriendProcess();
		if panel:IsEnable() == false then
			panel:SetEnable(true);
		end
	end
end
function CloseFriendList()
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("MessagerPanel");
	if panel ~= nil then
		UIManager:DeletePanel("MessagerPanel");
	end
	UIManager:DeletePanel("FriendMovingPanel");
	CloseFriendChatPopUp();
end

function CloseFriendChat()
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("FriendChatPanel");
	if panel ~= nil then
		UIManager:SetKeyInItem("EditLine");
		UIManager:DeletePanel("FriendChatPanel");
		local Menu = LuaUI.GetMenuFriendProcess();
		_G.FriendOnShowingIndex = -1;
		Menu:ResetTalkingTarget();
	end
end

function OpenSelfSetting()
	local panel = UIManager:GetUIItem("FriendSelfSettingPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuFriendSelfSetting.xml");
		UIManager:SetKeyInItem("FriendSelfSettingSign");
	else
		if panel:IsEnable() then
			CloseFriendSelfSettingMenuLua();
		else
			panel:SetEnable(true);
		end
	end	
	
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:RefreshSelfSetting();
end

function CloseFriendSelfSettingMenuLua()
	local panel = UIManager:GetUIItem("FriendSelfSettingPanel");
	if panel ~= nil then
		local panel2 = UIManager:GetUIItem("FriendChatPanel");
		if panel2 ~= nil then
			UIManager:SetKeyInItem("ChatEditLine");
		else
			UIManager:SetKeyInItem("EditLine");
		end
		UIManager:DeletePanel("FriendSelfSettingPanel");
		UIManager:DeletePanel("FriendSelfSettingPopPanel");
	end
end

function OpenChangeIconLua()
	local panel = UIManager:GetUIItem("FriendChatIconSelPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuFriendChatIconSel.xml");
		SetChatIconSelScrollNum();
		FriendChatIconStartIndex = 0;
		RefreshFriendChatIconMenu();
	else
		if panel:IsEnable() then
			CloseFriendChatIconSel();
		else
			panel:SetEnable(true);
		end
	end	
end

function CloseFriendChatIconSel()
	local panel = UIManager:GetUIItem("FriendChatIconSelPanel");
	if panel ~= nil then
		UIManager:DeletePanel("FriendChatIconSelPanel");
	end
end

function OpenFriendMsgHisLua()
	local panel = UIManager:GetUIItem("FriendChatHisPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuFriendChatHis.xml");
		local Menu = LuaUI.GetMenuFriendProcess();
		Menu:OpenHistory();
	else
		if panel:IsEnable() then
			CloseFriendMsgHisLua();
		else
			panel:SetEnable(true);
			local Menu = LuaUI.GetMenuFriendProcess();
			Menu:OpenHistory();
		end
	end	
end

function RefreshHistoryLua()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:OpenHistory();
end

function ClearHistoryLua()
	local chat = UIManager:FindUIChatPanel("FriendChatHisMsg");
	chat:ClearAllRow();
	chat:EndRow();
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:ClearHistory();
end

function CloseFriendMsgHisLua()
	local panel = UIManager:GetUIItem("FriendChatHisPanel");
	if panel ~= nil then
		UIManager:DeletePanel("FriendChatHisPanel");
	end
end

function ConnectToWebSiteLua()
	local Menu = LuaUI.GetMenuFriendProcess();
	LuaUI.OpenHomePage();
end

function MouseMoveOnMessager()
	FriendSelPreIndex = FriendSelIndex;
	FriendSelIndex = -1;
	local mouseX = UIManager:GetCursorX();
	local mouseY = UIManager:GetCursorY();
	local panel = UIManager:GetUIItem("MessagerPanel");
	if panel ~= nil then
		local relX = UIManager:GetCursorX() - panel:GetX();
		local relY = UIManager:GetCursorY() - panel:GetY();
		local TargetX = 6;
		local TargetY = 75;
		local width = 115;
		local height = 25;
		for i = 0, 6 do
			if	relX >= TargetX and
				relX <= TargetX + width and
				relY >= TargetY and
				relY <= TargetY + height then
				
				local textName = string.format("Friend%dName", i + 1);
			
				local UIText = UIManager:GetUIItem(textName);
				if UIText ~= nil and UIText:IsEnable() then
					local UIImage = UIManager:GetUIItem("FriendHighLight");
					if UIImage ~= nil then
						UIImage:SetOffsetXY(TargetX, TargetY)
						UIImage:SetEnable(true);
					end
					FriendSelIndex = i;
					if FriendSelPreIndex ~= FriendSelIndex then
						UIImage:SetEnableTimeOutEvent(true);
						UIImage:ResetTimer();
						CloseFriendChatPopUp();
					end
				end
				break;
			end
		TargetY = TargetY + height;
		end
	end
	
	if FriendSelIndex == -1 then
		local UIImage = UIManager:GetUIItem("FriendHighLight");
		if UIImage ~= nil then
			UIImage:SetEnable(false);
		end
		CloseFriendChatPopUp();
	end
	
	if FriendDragNameIndex ~= -1 then
		local movingPanel = UIManager:GetUIItem("FriendMovingPanel");
		if movingPanel ~= nil then
		movingPanel:SetEnable(true);
		UIManager:SetDragItemXY(movingPanel,-49 ,  
		-15);
		CloseFriendChatPopUp();
		end
	end
end

function DisableAllChangeGroup()
	UIManager:FindUICheckBox("FriendListChangeGroup1"):SetState(false);
	UIManager:FindUICheckBox("FriendListChangeGroup2"):SetState(false);
	UIManager:FindUICheckBox("FriendListChangeGroup3"):SetState(false);
	UIManager:FindUICheckBox("FriendListChangeGroup4"):SetState(false);
	UIManager:FindUICheckBox("TempFriendChangeGroup"):SetState(false);
	UIManager:FindUICheckBox("BlackListChangeGroup"):SetState(false);
end

function ChangeFriendListGroup1Lua()
	DisableAllChangeGroup();
	UIManager:FindUICheckBox("FriendListChangeGroup1"):SetState(true);
end

function ChangeFriendListGroup2Lua()
	DisableAllChangeGroup();
	UIManager:FindUICheckBox("FriendListChangeGroup2"):SetState(true);
end

function ChangeFriendListGroup3Lua()
	DisableAllChangeGroup();
	UIManager:FindUICheckBox("FriendListChangeGroup3"):SetState(true);
end

function ChangeFriendListGroup4Lua()
	DisableAllChangeGroup();
	UIManager:FindUICheckBox("FriendListChangeGroup4"):SetState(true);
end

function ChangeFriendListGroupTempLua()
	DisableAllChangeGroup();
	UIManager:FindUICheckBox("TempFriendChangeGroup"):SetState(true);
end

function ChangeBlackListGroupLua()
	DisableAllChangeGroup();
	UIManager:FindUICheckBox("BlackListChangeGroup"):SetState(true);
end

function DisableAllGroup()
	UIManager:FindUICheckBox("FriendListGroup1"):SetState(false);
	UIManager:FindUICheckBox("FriendListGroup2"):SetState(false);
	UIManager:FindUICheckBox("FriendListGroup3"):SetState(false);
	UIManager:FindUICheckBox("FriendListGroup4"):SetState(false);
	UIManager:FindUICheckBox("TempFriendGroup"):SetState(false);
	UIManager:FindUICheckBox("BlackListGroup"):SetState(false);
end

function FriendListGroup1Lua()
	DisableAllGroup();
	UIManager:FindUICheckBox("FriendListGroup1"):SetState(true);
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:ChangeGroup(2);
	Menu:RefreshMenu();
end

function FriendListGroup2Lua()
	DisableAllGroup();
	UIManager:FindUICheckBox("FriendListGroup2"):SetState(true);
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:ChangeGroup(3);
	Menu:RefreshMenu();
end

function FriendListGroup3Lua()
	DisableAllGroup();
	UIManager:FindUICheckBox("FriendListGroup3"):SetState(true);
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:ChangeGroup(4);
	Menu:RefreshMenu();
end

function FriendListGroup4Lua()
	DisableAllGroup();
	UIManager:FindUICheckBox("FriendListGroup4"):SetState(true);
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:ChangeGroup(5);
	Menu:RefreshMenu();
end

function FriendListGroupTempLua()
	DisableAllGroup();
	UIManager:FindUICheckBox("TempFriendGroup"):SetState(true);
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:ChangeGroup(0);
	Menu:RefreshMenu();
end

function BlackListGroupLua()
	DisableAllGroup();
	UIManager:FindUICheckBox("BlackListGroup"):SetState(true);
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:ChangeGroup(1);
	Menu:RefreshMenu();
end

function MessagerPanelUp()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:PageUp();
end

function MessagerPanelDown()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:PageDown();
end

function SendMsgToFriend()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:SendMessageToFriend();
end
function MessagerPanelScrollLua()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:FriendScroll();
end

function FriendChatScrollLua()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:FriendChatScroll();
end

function FriendChatHisScrollLua()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:FriendHisScroll();
end
function AddFriendToList()
	LuaUI.ChangeFriendIcon();
end

function OpenChangeGroup()
	local panel = UIManager:GetUIItem("FriendChangeGroupPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\FriendChangeGroup.xml");
	else
		panel:SetEnable(true);
	end
	
end

function ChangeGroupConfirmLua()
	local index = -1;
	if UIManager:FindUICheckBox("TempFriendChangeGroup"):GetState() then
		index = 0;
	elseif UIManager:FindUICheckBox("BlackListChangeGroup"):GetState() then
		index = 1;
	elseif UIManager:FindUICheckBox("FriendListChangeGroup1"):GetState() then
		index = 2;
	elseif UIManager:FindUICheckBox("FriendListChangeGroup2"):GetState() then
		index = 3;
	elseif UIManager:FindUICheckBox("FriendListChangeGroup3"):GetState() then
		index = 4;
	elseif UIManager:FindUICheckBox("FriendListChangeGroup4"):GetState() then
		index = 5;
	end
	
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:ConfirmChangeToGroup(index);
	CloseChangeGroup();
end
function CloseChangeGroup()
	UIManager:DeletePanel("FriendChangeGroupPanel");
end

function FriendChatPanelUp()
	local chat = UIManager:FindUIChatPanel("FriendChatMsg");
	if chat ~= nil then
		chat:UpRow();
	end
end

function FriendChatPanelDown()
	local chat = UIManager:FindUIChatPanel("FriendChatMsg");
	if chat ~= nil then
		chat:DownRow();
	end
end

function FriendChatHisPanelUp()
	local chat = UIManager:FindUIChatPanel("FriendChatHisMsg");
	if chat ~= nil then
		chat:UpRow();
	end
end

function FriendChatHisPanelDown()
	local chat = UIManager:FindUIChatPanel("FriendChatHisMsg");
	if chat ~= nil then
		chat:DownRow();
	end
end

function OpenFindFriendMenu()
	local panel = UIManager:GetUIItem("FindFriendPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuFriendFind.xml");
		UIManager:SetKeyInItem("FindFriendID");
	else
		CloseFindFriendPanelLua();
	end
end

function CloseFindFriendPanelLua()
	local panel = UIManager:GetUIItem("FindFriendPanel");
	if panel ~= nil then
		UIManager:SetKeyInItem("EditLine");
		UIManager:DeletePanel("FindFriendPanel");
	end
end
	
function FindFriendConfirm()
	local Menu = LuaUI.GetMenuFriendProcess();
	
	local editbox = UIManager:FindUIEditBox("FindFriendID");
	local editbox2 = UIManager:FindUIEditBox("FindFriendName");
	Menu:FindFriend(editbox:GetText(), editbox2:GetText());
	CloseFindFriendPanelLua();
end

function MouseMoveOnChatIconSel()
	FriendChatIconIndex = -1;
	local mouseX = UIManager:GetCursorX();
	local mouseY = UIManager:GetCursorY();
	local panel = UIManager:GetUIItem("FriendChatIconSelPanel");
	if panel ~= nil then
		local relX = UIManager:GetCursorX() - panel:GetX();
		local relY = UIManager:GetCursorY() - panel:GetY();
		local TargetX = 13;
		local TargetY = 58;
		local width = 45;
		local height = 45;
		for i = 0, 5 do
			for j = 0, 5 do 
			if	relX >= TargetX and
				relX <= TargetX + width and
				relY >= TargetY and
				relY <= TargetY + height then
				
				local textName = string.format("FriendChatIcon%d", i * 6 + j + 1);
			
				local UIText = UIManager:FindUIAniIcon(textName);
				if UIText ~= nil and UIText:GetHasAni() then
					local UIImage = UIManager:GetUIItem("FriendChatIconHLight");
					if UIImage ~= nil then
						UIImage:SetOffsetXY(TargetX, TargetY)
						UIImage:SetEnable(true);
					end
					FriendChatIconIndex = i * 6 + j;
				end
				break;
			end
			TargetX = TargetX + width;
			end
			TargetX = 13;
		TargetY = TargetY + height;
		end
	end
	
	if FriendChatIconIndex == -1 then
		local UIImage = UIManager:GetUIItem("FriendChatIconHLight");
		if UIImage ~= nil then
			UIImage:SetEnable(false);
		end
	end
end

function SelChatIcon()
	local UIImage = UIManager:GetUIItem("FriendChatIconHLight");
	if UIImage ~= nil then
		local UIImage2 = UIManager:GetUIItem("FriendChatIconSel");
		if UIImage2 ~= nil then
			UIImage2:SetOffsetXY(UIImage:GetOffsetX(), UIImage:GetOffsetY())
			UIImage2:SetEnable(true);
			FriendChatIconSelIndex = FriendChatIconIndex;
		end
	end
end

function FriendChatIconSelConfirmLua()
	if LuaUI.IsCurrMsgIcon(FriendChatIconSelIndex + FriendChatIconStartIndex * FriendChatIconRowCount) ~= true then
		LuaUI.OpenFriendChatBox("MSG_CHAT_ICON_CHANGE_CONFIRM", "ConfirmChangeIcon");
	end
end

function ConfirmChangeIcon()
	LuaUI.ChangeChatIcon(FriendChatIconSelIndex + FriendChatIconStartIndex * FriendChatIconRowCount);
	CloseFriendBox();
end

function CloseFriendBox()
	LuaUI.CloseFriendBox();
end

function OpenFriendChatPopUp()
	local panel = UIManager:GetUIItem("FriendChatPopUpPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuFriendPopUp.xml");
	end
	panel = UIManager:GetUIItem("FriendChatPopUpPanel");
	local mPanel = UIManager:GetUIItem("MessagerPanel");
	if mPanel ~= nil then
		local mouseY = UIManager:GetCursorY();
		local posY = mouseY - 20;
		local posX = mPanel:GetX() - 198;
		if posX < 0 then
			posX = 0;
		end
		
		if posY < 0 then 
			posY = 0;
		elseif posY > 600 then
			posY = 600 - panel:GetHeight();
		end
		panel:SetPos(posX, posY);
	end
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:RefreshPopUp(FriendSelIndex);
end

function CloseFriendChatPopUp()
	local panel = UIManager:GetUIItem("FriendChatPopUpPanel");
	if panel ~= nil then
		UIManager:DeletePanel("FriendChatPopUpPanel");
	end
end

function ChatDetailPopupTimeOut()
	OpenFriendChatPopUp();
	local UIImage = UIManager:GetUIItem("FriendHighLight");
	if UIImage ~= nil then
		UIImage:SetEnableTimeOutEvent(false);
	end
end

function MessagerPanelOut()
	FriendSelPreIndex = FriendSelIndex;
	FriendSelIndex = -1;
	local UIImage = UIManager:GetUIItem("FriendHighLight");
	if UIImage ~= nil then
		UIImage:SetEnable(false);
	end
	CloseFriendChatPopUp();
end

function CopySelectedText()
	FriendDragNameIndex = FriendSelIndex;
	local textName = string.format("Friend%dName", FriendSelIndex + 1);
			
	local UIText = UIManager:FindUIStaticText(textName);
	if UIText ~= nil then
		local UIText2 = UIManager:FindUIStaticText("FriendMovingName");
		if UIText2 ~= nil then
			UIText2:SetText(UIText:GetText());
			UIText2:Redraw();
		end
	end
end

function FriendDragEnd()
	local Menu = LuaUI.GetMenuFriendProcess();
	if UIManager:Detect("FriendListGroup1", UIManager:GetCursorX(), UIManager:GetCursorY()) then
		Menu:MoveGroup(FriendDragNameIndex, 2);
	elseif UIManager:Detect("FriendListGroup2", UIManager:GetCursorX(), UIManager:GetCursorY()) then
		Menu:MoveGroup(FriendDragNameIndex, 3);
	elseif UIManager:Detect("FriendListGroup3", UIManager:GetCursorX(), UIManager:GetCursorY()) then
		Menu:MoveGroup(FriendDragNameIndex, 4);
	elseif UIManager:Detect("FriendListGroup4", UIManager:GetCursorX(), UIManager:GetCursorY()) then
		Menu:MoveGroup(FriendDragNameIndex, 5);
	elseif UIManager:Detect("TempFriendGroup", UIManager:GetCursorX(), UIManager:GetCursorY()) then
		Menu:MoveGroup(FriendDragNameIndex, 0);
	elseif UIManager:Detect("BlackListGroup", UIManager:GetCursorX(), UIManager:GetCursorY()) then
		Menu:MoveGroup(FriendDragNameIndex, 1);
	end
	FriendDragNameIndex = -1;
	local panel = UIManager:GetUIItem("FriendMovingPanel");
	if panel ~= nil then
		panel:SetEnable(false);
	end
end

function ConfirmChangeGroup()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:MoveGroupImm();
	CloseFriendBox();
end

function TargetInfoGetEquipLua()
	local TargetEquipPanel = UIManager:FindUIPanel("TargetEquipPanel")
	
	if TargetEquipPanel ~= nil then
		UIManager:DeletePanel("TargetEquipPanel")
	else
		RequestTargetEquip();
	end
end

function TargetInfoUpdateLua()
	local Menu = LuaUI.GetMenuTargetProcess();
	Menu:UpdateInfo();
end

function TargetInfoPrivateLua()
	local Menu = LuaUI.GetMenuTargetProcess();
	Menu:PrivateMsg();
end

function TargetInfoAddFriendLua()
	local Menu = LuaUI.GetMenuTargetProcess();
	Menu:AddToFriend();
end

function TargetInfoChangeGrpLua()
	local Menu = LuaUI.GetMenuTargetProcess();
	Menu:ChangeGrp();
end

function TargetInfoBreakLua()
	local Menu = LuaUI.GetMenuTargetProcess();
	Menu:BreakRelation();
end

function TargetInfoBlackListLua()
	local Menu = LuaUI.GetMenuTargetProcess();
	Menu:BlackList();
end

function OpenFriendDetail()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:OpenFriendDetail(FriendSelIndex);
end

function CheckFriendInfoLua()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:OpenTalkingFriendDetail();
end

function FriendSelfSettingConfirmLua()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:ConfirmSettingChange();
	CloseFriendSelfSettingMenuLua();
end

function SelfSettingReplyChange()
	local panel = UIManager:FindUIPanel("FriendSelfSettingPopPanel");
	panel:SetPos(UIManager:GetCursorX() - 24, UIManager:GetCursorY() - 50);
	if panel:IsEnable() then
		panel:SetEnable(false);
	else
		panel:SetEnable(true);
		UIManager:SetPanel2Top(panel);
	end
end

function SelfSettingReplyChangeOnline()
	local buttonOnline = UIManager:GetUIItem("FriendSelfSettingOnline");
	local buttonOffline = UIManager:GetUIItem("FriendSelfSettingOffline");
	buttonOnline:SetEnable(true);
	buttonOffline:SetEnable(false);
	
	local panel = UIManager:GetUIItem("FriendSelfSettingPopPanel");
	panel:SetEnable(false);
end

function SelfSettingReplyChangeOffline()
	local buttonOnline = UIManager:GetUIItem("FriendSelfSettingOnline");
	local buttonOffline = UIManager:GetUIItem("FriendSelfSettingOffline");
	buttonOnline:SetEnable(false);
	buttonOffline:SetEnable(true);
	
	local panel = UIManager:GetUIItem("FriendSelfSettingPopPanel");
	panel:SetEnable(false);
end

function FriendSelfSettingAutoSaveLua()
	local menu = UIManager:GetMenuSystemSetting();
	menu:SetSaveChatHistory(UIManager:FindUICheckBox("FriendSelfSettingAutoSave"):GetState());
end

function FriendChatNextLua()
	local menu = LuaUI.GetMenuFriendProcess();
	menu:NextChatMessage();
end

function FriendChatClearMsg()
	local chat = UIManager:FindUIChatPanel("FriendChatMsg");
	chat:ClearAllRow();
	chat:EndRow();
end

function FriendChatPopColorPanel()
	local PopPanel = UIManager:FindUIPanel("ColorPanel")
	
	if PopPanel ~= nil then
		UIManager:DeletePanel("ColorPanel")
	else
		LuaUI.AddXMLFile2UIManger("Data\\PopPanel.xml");
		PopPanel = UIManager:FindUIPanel("ColorPanel");
		PopPanel:SetPos(UIManager:GetCursorX(), UIManager:GetCursorY() - 120);
	end
end

function ConfirmDeleteFriend()
	local Menu = LuaUI.GetMenuFriendProcess();
	Menu:ConfirmDelete();
	CloseFriendBox();
end


function RefreshFriendChatIconMenu()
	for i = 1, 36 do
		local name = string.format("FriendChatIcon%d", i);
		local aniName = string.format("facechatL%05da.ent", i - 1 + FriendChatIconStartIndex * FriendChatIconRowCount);
		local pAniIcon = UIManager:FindUIAniIcon(name);
		if pAniIcon ~= nil then
			pAniIcon:SetAnimationDataA(aniName);
			if pAniIcon:GetHasAni() then
				pAniIcon:SetEnable(true);
			else
				pAniIcon:SetEnable(false);
			end
		end
	end
end

function FriendChatIconSelUp()
	if FriendChatIconStartIndex > 0 then
		FriendChatIconStartIndex = FriendChatIconStartIndex - 1;
		RefreshFriendChatIconMenu();
	end
end


function FriendChatIconSelDown()
	if FriendChatIconStartIndex + FriendChatIconRowCount < FriendChatIconTotalNum / FriendChatIconRowCount then
		FriendChatIconStartIndex = FriendChatIconStartIndex + 1;
		RefreshFriendChatIconMenu();
	end
end

function FriendChatIconSelLua()
	local scroll = UIManager:FindUIScrollBarV("FriendChatIconSelScroll");
	if scroll ~= nil then
		local num = scroll:GetSelNum();
		if num  > math.floor(FriendChatIconTotalNum / FriendChatIconRowCount) - FriendChatIconRowCount + 1 then
			num = math.floor(FriendChatIconTotalNum / FriendChatIconRowCount) - FriendChatIconRowCount + 1;
		end
		FriendChatIconStartIndex = num;
		RefreshFriendChatIconMenu();
	end
end

function SetChatIconSelScrollNum()
	FriendChatIconTotalNum = 0;
	local i = 0;
	local pAniIcon = UIManager:FindUIAniIcon("FriendChatIcon1");
	while true do		
		local aniName = string.format("facechatL%05da.ent", i);		
		pAniIcon:SetAnimationDataA(aniName);
		if not pAniIcon:GetHasAni() then
			break;
		end		
		i = i + 1;
	end
	FriendChatIconTotalNum = i;
	
	local scroll = UIManager:FindUIScrollBarV("FriendChatIconSelScroll");
	if scroll ~= nil then
		scroll:SetNumCount(math.ceil(FriendChatIconTotalNum / FriendChatIconRowCount));
	end
end

function MessagerPanelWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("MessagerPanelScroll"):OnMouseWheel(UIManager:GetUIItem("MessagerPanelScroll"):GetX()
	,UIManager:GetUIItem("MessagerPanelScroll"):GetY(),pZ);
	
end

function FriendChatIconSelWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("FriendChatIconSelScroll"):OnMouseWheel(UIManager:GetUIItem("FriendChatIconSelScroll"):GetX()
	,UIManager:GetUIItem("FriendChatIconSelScroll"):GetY(),pZ);
end

function FriendChatPanelWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("FriendChatScroll"):OnMouseWheel(UIManager:GetUIItem("FriendChatScroll"):GetX()
	,UIManager:GetUIItem("FriendChatScroll"):GetY(),pZ);
end

function FriendChatHisWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("FriendChatHisScroll"):OnMouseWheel(UIManager:GetUIItem("FriendChatHisScroll"):GetX()
	,UIManager:GetUIItem("FriendChatHisScroll"):GetY(),pZ);
end