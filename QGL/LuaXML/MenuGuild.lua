function OpenGuildPanel(pX, pY)
	local menu = LuaUI.GetMenuGuildProcess();
	if not menu:HasGuild() then
		LuaUI.ShowErrMessage("MSG_NO_GUILD", true);
		do return end;
	end
	local panel = UIManager:FindUIPanel("MenuGuild");
	
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuGuild.xml");
		GuildInfoCheckLua();
		local Menu = LuaUI.GetMenuHelpProcess();
		Menu:HandleEvent(20);	
		menu:RequestApplicantInfo();
		menu:RequestGuildInfo();
		menu:RequestMemberInfo();
		menu:RefreshGuildButton();
		menu:RefreshManorMenu();	
	else
		CloseGuildPanel();
	end
end

function CloseGuildPanel()
	UIManager:DeletePanel("MenuGuild");
end
 
function SelectGuildApplyItem()
	local mouseX = UIManager:GetCursorX();
	local mouseY = UIManager:GetCursorY();
	local index = -1;
	local panel = UIManager:GetUIItem("MenuGuildApply");
	if panel ~= nil then
		local relX = UIManager:GetCursorX() - panel:GetX();
		local relY = UIManager:GetCursorY() - panel:GetY();
		local TargetX = 6;
		local TargetY = 56;
		local width = 410;
		local height = 18;
		for i = 0, 9 do
			if	relX >= TargetX and
				relX <= TargetX + width and
				relY >= TargetY and
				relY <= TargetY + height then
				
				index = i;
				local imgName = "RankHightLight";
				local UIImage = UIManager:GetUIItem("GuildApplyHighLightBar");
				if UIImage ~= nil then
						UIImage:SetOffsetXY(TargetX, TargetY)
						UIImage:SetEnable(true);
				end
				
				local menu = LuaUI.GetMenuGuildProcess();
				menu:ShowAim(index);
				break;
			end
			TargetY = TargetY + height;
		end
	end
	
	if index == -1 then
		local UIImage = UIManager:GetUIItem("GuildApplyHighLightBar");
		if UIImage ~= nil then
			UIImage:SetEnable(false);
		end
	end
end
function SelectGuildItem()
	local mouseX = UIManager:GetCursorX();
	local mouseY = UIManager:GetCursorY();
	local index = -1;
	local panel = UIManager:GetUIItem("MenuGuild");
	if panel ~= nil then
		local relX = UIManager:GetCursorX() - panel:GetX();
		local relY = UIManager:GetCursorY() - panel:GetY();
		local TargetX = 24;
		local TargetY = 112;
		local width = 460;
		local height = 25;
		for i = 0, 9 do
			if	relX >= TargetX and
				relX <= TargetX + width and
				relY >= TargetY and
				relY <= TargetY + height then
				
				index = i;
				
				local UIImage = UIManager:GetUIItem("GuildHighLightIm");
				if UIImage ~= nil then
						UIImage:SetOffsetXY(TargetX, TargetY)
						UIImage:SetEnable(true);
				end
				local menu = LuaUI.GetMenuGuildProcess();
				menu:SetMenuIndex(index);
				break;
			end
			TargetY = TargetY + height;
		end
	end
	
	if index == -1 then
		local UIImage = UIManager:GetUIItem("GuildHighLightIm");
		if UIImage ~= nil then
			UIImage:SetEnable(false);
		end
	end
end

function HideGuildInfoUI()
	UIManager:GetUIItem("GuildInfoIm"):SetEnable(false);
	
	UIManager:GetUIItem("GuildInfoName"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoFirstMaster"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoCurMaster"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoVice"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoNum"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoPerName"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoPerLevel"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoMoney"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoRich"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoMemNum"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoAlly"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoEnemy"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoJob"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoDonation"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoPvPScore"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoBid"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoGroup"):SetEnable(false);
	UIManager:GetUIItem("GuildInfoGroupVSGuild"):SetEnable(false);
	
	
end

function ShowGuildInfoUI()
	UIManager:GetUIItem("GuildInfoIm"):SetEnable(true);
	
	UIManager:GetUIItem("GuildInfoName"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoFirstMaster"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoCurMaster"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoVice"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoNum"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoPerName"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoPerLevel"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoMoney"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoRich"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoMemNum"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoAlly"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoEnemy"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoJob"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoDonation"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoPvPScore"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoBid"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoGroup"):SetEnable(true);
	UIManager:GetUIItem("GuildInfoGroupVSGuild"):SetEnable(true);
end

function HideGuildMemInfoUI()
	UIManager:GetUIItem("GuildHighLightIm"):SetEnable(false);
	UIManager:GetUIItem("GuildMemInfoIm"):SetEnable(false);
	UIManager:GetUIItem("GuildMemInfoBar1Im"):SetEnable(false);
	UIManager:GetUIItem("GuildMemInfoBar2Im"):SetEnable(false);
	UIManager:GetUIItem("GuildMemInfoScrollBarIm"):SetEnable(false);
	UIManager:GetUIItem("GuildMemInfoScrollBar"):SetEnable(false);
	UIManager:GetUIItem("GuildHighLightBg"):SetEnable(false);
	
	
	local index = 1;
	while index <= 10 do
		local name = string.format("GuildMem%dName", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildMem%dLevel", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildMem%dClass", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildMem%dPosition", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildMem%dDonation", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildMem%dJoinTime", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildMem%dOfflineTime", index);
		UIManager:GetUIItem(name):SetEnable(false);
		index = index + 1;
	end
end

function ShowGuildMemInfoUI()
	UIManager:GetUIItem("GuildMemInfoIm"):SetEnable(true);
	UIManager:GetUIItem("GuildMemInfoBar1Im"):SetEnable(true);
	UIManager:GetUIItem("GuildMemInfoBar2Im"):SetEnable(true);
	UIManager:GetUIItem("GuildMemInfoScrollBarIm"):SetEnable(true);
	UIManager:GetUIItem("GuildMemInfoScrollBar"):SetEnable(true);
	UIManager:GetUIItem("GuildHighLightBg"):SetEnable(true);
end

function HideGuildRuleUI()
	UIManager:GetUIItem("GuildRuleBar1Im"):SetEnable(false);
	UIManager:GetUIItem("GuildRuleBar2Im"):SetEnable(false);
	UIManager:GetUIItem("GuildRuleChat"):SetEnable(false);
	UIManager:GetUIItem("GuildAimChat"):SetEnable(false);
	UIManager:GetUIItem("GuildAimChatUpRow"):SetEnable(false);
	UIManager:GetUIItem("GuildAimChatDownRow"):SetEnable(false);
end

function ShowGuildRuleUI()
	UIManager:GetUIItem("GuildRuleBar1Im"):SetEnable(true);
	UIManager:GetUIItem("GuildRuleBar2Im"):SetEnable(true);
	UIManager:GetUIItem("GuildRuleChat"):SetEnable(true);
	UIManager:GetUIItem("GuildAimChat"):SetEnable(true);
	UIManager:GetUIItem("GuildAimChatUpRow"):SetEnable(true);
	UIManager:GetUIItem("GuildAimChatDownRow"):SetEnable(true);
end

function GuildMemInfoScrollLua()
	local menu = LuaUI.GetMenuGuildProcess();
	menu:ScrollMember();
end

function GuildMemInfoButtonUpLua()
	local menu = LuaUI.GetMenuGuildProcess();
	menu:MemberPageUp();
end

function GuildMemInfoDownLua()
	local menu = LuaUI.GetMenuGuildProcess();
	menu:MemberPageDown();
end

function HideGuildApplicationUI()
	UIManager:GetUIItem("GuildHighLightIm"):SetEnable(false);
	UIManager:GetUIItem("GuildApplyInfoIm"):SetEnable(false);
	UIManager:GetUIItem("GuildHighLightIm"):SetEnable(false);
	UIManager:GetUIItem("GuildMemInfoBar1Im"):SetEnable(false);
	UIManager:GetUIItem("GuildMemInfoBar2Im"):SetEnable(false);
	UIManager:GetUIItem("GuildMemInfoScrollBarIm"):SetEnable(false);
	UIManager:GetUIItem("GuildMemInfoScrollBar"):SetEnable(false);
	UIManager:GetUIItem("GuildHighLightBg"):SetEnable(false);
	
	local index = 1;
	while index <= 10 do
		local name = string.format("GuildApply%dName", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildApply%dID", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildApply%dLevel", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildApply%dDate", index);
		UIManager:GetUIItem(name):SetEnable(false);
		
		index = index + 1;
	end
	
end

function ShowGuildApplicatioUI()
	UIManager:GetUIItem("GuildApplyInfoIm"):SetEnable(true);
	UIManager:GetUIItem("GuildMemInfoBar1Im"):SetEnable(true);
	UIManager:GetUIItem("GuildMemInfoBar2Im"):SetEnable(true);
	UIManager:GetUIItem("GuildMemInfoScrollBarIm"):SetEnable(true);
	UIManager:GetUIItem("GuildMemInfoScrollBar"):SetEnable(true);
	UIManager:GetUIItem("GuildHighLightBg"):SetEnable(true);
	
end

function ResetGuildCheck()
	UIManager:FindUICheckBox("GuildManorCheck"):SetState(false);
	UIManager:FindUICheckBox("GuildApplicantCheck"):SetState(false);
	UIManager:FindUICheckBox("GuildRuleCheck"):SetState(false);
	UIManager:FindUICheckBox("GuildMemberCheck"):SetState(false);
	UIManager:FindUICheckBox("GuildInfoCheck"):SetState(false);
end

function HideAllSubUI()
	HideGuildInfoUI();
	HideGuildMemInfoUI();
	HideGuildRuleUI();
	HideGuildApplicationUI();
	SetOnOffManor(false);
	
end

function GuildInfoCheckLua()
	ResetGuildCheck();
	UIManager:FindUICheckBox("GuildInfoCheck"):SetState(true);
	HideAllSubUI();
	ShowGuildInfoUI();
	local menu = LuaUI.GetMenuGuildProcess();
	menu:RefreshInfoMenu();
end

function GuildMemberCheckLua()
	ResetGuildCheck();
	UIManager:FindUICheckBox("GuildMemberCheck"):SetState(true);
	HideAllSubUI();
	ShowGuildMemInfoUI();
	local menu = LuaUI.GetMenuGuildProcess();
	menu:RefreshMemberMenu();
end

function GuildRuleCheckLua()
	ResetGuildCheck();
	UIManager:FindUICheckBox("GuildRuleCheck"):SetState(true);
	HideAllSubUI();
	ShowGuildRuleUI();
	local menu = LuaUI.GetMenuGuildProcess();
	menu:RefreshRuleMenu();
end

function GuildApplicantCheckLua()
	ResetGuildCheck();
	UIManager:FindUICheckBox("GuildApplicantCheck"):SetState(true);
	HideAllSubUI();
	ShowGuildApplicatioUI();
	local menu = LuaUI.GetMenuGuildProcess();
	menu:RefreshApplicantMenu();
end

function GuildManorCheckLua()
	ResetGuildCheck();
	UIManager:FindUICheckBox("GuildManorCheck"):SetState(true);
	HideAllSubUI();
	SetOnOffManor(true)	
	local menu = LuaUI.GetMenuGuildProcess();
	menu:RefreshManorMenu();
end

function OpenGuildAimPanel()
	local panel = UIManager:FindUIPanel("MenuGuildAimEdit");
	
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuGuildAim.xml");
		
		UIManager:SetKeyInItem("MenuGuildAimEdit");
		local menu = LuaUI.GetMenuGuildProcess();
		menu:RefreshGuildAimEdit();
	else
		CloseGuildAimPanel();
	end
end

function CloseGuildAimPanel()
	UIManager:DeletePanel("MenuGuildAimEdit");
	UIManager:SetKeyInItem("EditLine");
end

function ConfirmGuildAimChange()
	local menu = LuaUI.GetMenuGuildProcess();
	menu:ConfirmGuildAimChange();
	CloseGuildAimPanel();
end

function OpenGuildRulePanel()
	local panel = UIManager:FindUIPanel("MenuGuildRuleEdit");
	
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuGuildRule.xml");
		local menu = LuaUI.GetMenuGuildProcess();
		menu:RefreshGuildRuleEdit();
		UIManager:SetKeyInItem("MenuGuildRuleEditLine");
	else
		CloseGuildRulePanel();
	end
end

function CloseGuildRulePanel()
	UIManager:DeletePanel("MenuGuildRuleEdit");
	UIManager:SetKeyInItem("EditLine");
end

function ConfirmGuildRuleChange()
	local menu = LuaUI.GetMenuGuildProcess();
	menu:ConfirmGuildRuleChange();
	CloseGuildRulePanel();
end

function HideGuildApplyStuffs()
	UIManager:GetUIItem("GuildApplyAim"):SetEnable(false);
	local index = 1;
	while index <= 10 do
		local name = string.format("GuildApplyName%d", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildApplyID%d", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildApplyMaster%d", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildApplyMemberNum%d", index);
		UIManager:GetUIItem(name):SetEnable(false);
		
		index = index + 1;
	end
end

function OpenGuildApplyPanel()
	local panel = UIManager:FindUIPanel("MenuGuildApply");
	
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuGuildApply.xml");
		HideGuildApplyStuffs();
		local menu = LuaUI.GetMenuGuildProcess();
		menu:ScrollGuildApply();
		menu:RefreshGuildApplyPanel();
	else
		CloseGuildApplyPanel();
	end
end
function CloseGuildApplyPanel()
	local menu = LuaUI.GetMenuGuildProcess();
	menu:FreeGuild();
	UIManager:DeletePanel("MenuGuildApply");
end

function GuildApplyConfirmJoin()
	local menu = LuaUI.GetMenuGuildProcess();
	menu:ApplyConfirm();
	UIManager:DeletePanel("MenuGuildApply");
end

function CloseMenuGuildDeleteBox()
	UIManager:DeletePanel("MenuGuildDeleteBoxPanel");
	UIManager:SetKeyInItem("EditLine");
end

function GuildSorintLua()
	local i = 0;
	
	local image = UIManager:GetUIItem("GuildMemInfoIm");
	if image ~= nil then
		local x = UIManager:GetCursorX() - image:GetX() + 25;
		if x  >=  107 and x <= 152 then
			i = 1;
		elseif x  >=  155 and x <= 200 then
			i = 2;
		elseif x  >=  207 and x <= 252 then
			i = 3;
		elseif x  >=  293 and x <= 338 then
			i = 4;
		elseif x  >=  396 and x <= 459 then
			i = 5;
		elseif x  >=  470 and x <= 533 then
			i = 6;
		end				
	end
			
	local menu = LuaUI.GetMenuGuildProcess();
	menu:SortMember(i);
end
function MenuGuildDeleteBoxKeyIn()
	local editbox = UIManager:FindUIEditBoxEx("MenuGuildDeleteBoxEdit");
	if editbox ~= nil then
		if editbox:GetTextSize() > 0 then
			local button = UIManager:FindUIButton("MenuGuildDeleteBoxYes");
			if button ~= nil then
				button:SetNormalState();
			end
		else
			local button = UIManager:FindUIButton("MenuGuildDeleteBoxYes");
			if button ~= nil then
				button:SetDisableState();
			end
		end
	end
end

function MenuGuildDonateBoxKeyIn()
	local editbox = UIManager:FindUIEditBox("NumberOfGuildDonate");
	if editbox ~= nil then
		local num = editbox:GetInt();
		if num > 1000000 then
			editbox:SetText("1000000");
			editbox:Redraw();
		end
	end
end
function OpenMenuGuildDeleteBox()
	local panel = UIManager:FindUIPanel("MenuGuildDeleteBoxPanel");
	
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuGuildDeleteBox.xml");		
		local button = UIManager:FindUIButton("MenuGuildDeleteBoxYes");
		if button ~= nil then
			button:SetDisableState();
		end
		UIManager:SetKeyInItem("MenuGuildDeleteBoxEdit");
	else
		CloseMenuGuildDeleteBox();
	end
end

function KickGuildMemberLua()
	LuaUI.OpenDeleteMemberGuildBox();
	CloseMenuGuildDeleteBox();
end
function LeaveGuildLua()
	LuaUI.OpenLeaveGuildBox();
end

function AddGuildMemberLua()
	LuaUI.OpenGuildBox();
end

function CancelApplyBox()
	LuaUI.ClsoeGuildBox();
end

function ConfirmApply()
	local menu = LuaUI.GetMenuGuildProcess();
	menu:AddGuildMember();
	LuaUI.ClsoeGuildBox();
end

function ConfirmDelete()
	local menu = LuaUI.GetMenuGuildProcess();
	menu:DeleteGuildMember();
	LuaUI.ClsoeGuildBox();
end

function RejectApply()
	local menu = LuaUI.GetMenuGuildProcess();
	menu:RejectGuildMember();
	LuaUI.ClsoeGuildBox();
end

function OpenGuildPost()
	local panel = UIManager:FindUIPanel("GuildPostPanel");
	
	if panel == nil then
		local menu = LuaUI.GetMenuGuildProcess();
		if menu:IsSelectedMember(true) then
			LuaUI.AddXMLFile2UIManger("Data\\MenuGuildPost.xml");
			local menu = LuaUI.GetMenuGuildProcess();			
			menu:SetGuildPostSelectIndex(0);
			menu:RefreshPostMenu();
		end
	else
		CloseGuildPost();
	end
end
function CloseGuildPost()
	UIManager:DeletePanel("GuildPostPanel");
end

function GuildPostConfirmLua()
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:ConfirmPostChange();
	CloseGuildPost();
end

function ShowMemberInfo()
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:ShowInfo();
end

function SelectGuildPost()
	local SelIndex = -1;
	local mouseX = UIManager:GetCursorX();
	local mouseY = UIManager:GetCursorY();
	local panel = UIManager:GetUIItem("GuildPostPanel");
	if panel ~= nil then
		local relX = UIManager:GetCursorX() - panel:GetX();
		local relY = UIManager:GetCursorY() - panel:GetY();
		local TargetX = 9;
		local TargetY = 31;
		local width = 201;
		local height = 22;
		for i = 0, 9 do
			if	relX >= TargetX and
				relX <= TargetX + width and
				relY >= TargetY and
				relY <= TargetY + height then
				
				local textName = string.format("GuildPostName%d", i + 1);
			
				local UIText = UIManager:GetUIItem(textName);
				if UIText ~= nil and UIText:IsEnable() then
					local UIImage = UIManager:GetUIItem("GuildPostHighLight");
					if UIImage ~= nil then
						UIImage:SetOffsetXY(TargetX, TargetY)
						UIImage:SetEnable(true);
					end
					SelIndex = i;
				end
				break;
			end
		TargetY = TargetY + height;
		end
	end
	
	if SelIndex == -1 then
		local UIImage = UIManager:GetUIItem("GuildPostHighLight");
		if UIImage ~= nil then
			UIImage:SetEnable(false);
		end
	end
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:SetGuildPostSelectIndex(SelIndex);
end

function OpenGuildDonation()
	local panel = UIManager:FindUIPanel("MenuGuildDonatePanel");
	
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuGuildDonate.xml");
		local numEdit = UIManager:FindUIEditBox("NumberOfGuildDonate");
		if numEdit ~= nil then
			UIManager:SetKeyInItem("NumberOfGuildDonate");
			numEdit:End();
			numEdit:Redraw();
		end
	else
		CloseGuildDonation();
	end
end
function CloseGuildDonation()
	UIManager:DeletePanel("MenuGuildDonatePanel");
	UIManager:SetKeyInItem("EditLine");
end

function ConfirmGuildDonateNum()
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:ConfirmDonate();
	CloseGuildDonation();
end

function OpenGuildNameInput()
	local panel = UIManager:FindUIPanel("MenuGuildNameInputPanel");
	
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuGuildNameInput.xml");
		UIManager:SetKeyInItem("NumberOfGuildNameInput");
	else
		CloseGuildNameInput();
	end
end
function CloseGuildNameInput()
	UIManager:DeletePanel("MenuGuildNameInputPanel");
	UIManager:SetKeyInItem("EditLine");
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:CancelGuildNameInput();
end

function ConfirmGuildNameInput()
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:ConfirmGuildNameInput();
	CloseGuildNameInput();
end

function CancelGuildNameInput()	
	CloseGuildNameInput();
end

function GuildRuleScrollLua()
end

function GuildRuleEditLineOnChar(KeyCode)
end


function SetGuildRuleEditLineFocus()
end

function GuildApplyScrollLua()
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:ScrollGuildApply();
end

function GuildApplyButtonUpLua()
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:GuildApplyPageUp();
end

function GuildApplyDownLua()
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:GuildApplyPageDown();
end

function ConfirmLeaveGuild()
	local menu = LuaUI.GetMenuGuildProcess();
	menu:LeaveGuild();
	LuaUI.ClsoeGuildBox();
end

function MenuGuildApplyWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("GuildApplyScrollBar"):OnMouseWheel(UIManager:GetUIItem("GuildApplyScrollBar"):GetX()
		,UIManager:GetUIItem("GuildApplyScrollBar"):GetY(),pZ);
end

function MenuGuildWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("GuildMemInfoScrollBar"):OnMouseWheel(UIManager:GetUIItem("GuildMemInfoScrollBar"):GetX()
		,UIManager:GetUIItem("GuildMemInfoScrollBar"):GetY(),pZ);
end

function SetOnOffManor(bEnable)
	UIManager:GetUIItem("GuildManorIm"):SetEnable(bEnable);
	UIManager:GetUIItem("GuildManorScale"):SetEnable(bEnable);
	UIManager:GetUIItem("GuildManorAura"):SetEnable(bEnable);
	UIManager:GetUIItem("GuildSitIndex"):SetEnable(bEnable);
	UIManager:GetUIItem("GuildMaintainSuccess"):SetEnable(bEnable);
	UIManager:GetUIItem("GuildMaintainFee"):SetEnable(bEnable);
	UIManager:GetUIItem("GuildManorRoom"):SetEnable(bEnable);
	UIManager:GetUIItem("GuildManorGuard"):SetEnable(bEnable);
	UIManager:GetUIItem("GuildTigerLevel"):SetEnable(bEnable);
	UIManager:GetUIItem("GuildMaintainFail"):SetEnable(bEnable);

	UIManager:GetUIItem("ManorCharExPow"):SetEnable(bEnable);
	UIManager:GetUIItem("ManorCharInPow"):SetEnable(bEnable);
	UIManager:GetUIItem("ManorPartnerExPow"):SetEnable(bEnable);
	UIManager:GetUIItem("ManorPartnerInPow"):SetEnable(bEnable);
	UIManager:GetUIItem("ManorHP"):SetEnable(bEnable);
	UIManager:GetUIItem("ManorSP"):SetEnable(bEnable);
	UIManager:GetUIItem("ManorDrug"):SetEnable(bEnable);
	UIManager:GetUIItem("ManorCook"):SetEnable(bEnable);
	UIManager:GetUIItem("ManorWine"):SetEnable(bEnable);
	UIManager:GetUIItem("GuildMaterial"):SetEnable(bEnable);
	
	UIManager:GetUIItem("GuildNoManor"):SetEnable(false);		
end


function MenuGuildBusinessLogWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("GuildBusinessLogScrollBar"):OnMouseWheel(UIManager:GetUIItem("GuildBusinessLogScrollBar"):GetX()
		,UIManager:GetUIItem("GuildBusinessLogScrollBar"):GetY(),pZ);
end


function OpenGuildBusinessLogPanel()
	local panel = UIManager:FindUIPanel("MenuGuildBusinessLog");
	
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuGuildBusinessLog.xml");
		local panel2 = UIManager:FindUIPanel("MenuGuildBusinessLogSearchPanel");
		if panel2 ~= nil then
			panel2:SetEnable(false)
		end
		local menu = LuaUI.GetMenuGuildProcess();
		menu:ScrollGuildBusinessLog();
		menu:RefreshGuildBusinessLogPanel();
	else
		CloseGuildBusinessLogPanel();
	end
end

function CloseGuildBusinessLogPanel()
	UIManager:DeletePanel("MenuGuildBusinessLog");
	UIManager:DeletePanel("MenuGuildBusinessLogSearchPanel");
end

function GuildBusinessLogScrollLua()
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:ScrollGuildBusinessLog();
end

function GuildBusinessLogButtonUpLua()
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:GuildBusinessLogPageUp();
end

function GuildBusinessLogDownLua()
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:GuildBusinessLogPageDown();
end

function ConfirmGuildBusinessLogSearchYes()
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:ConfirmGuildBusinessLogSearchYes();
	CloseMenuGuildBusinessLogSearchPanel();
end

function GuildBusinessLogSearch()
	local panel = UIManager:FindUIPanel("MenuGuildBusinessLogSearchPanel");
	if panel ~= nil then
		panel:SetEnable(true);
		UIManager:SetPanel2Top(panel)
		UIManager:SetKeyInItem("NumberOfGuildBusinessLogSearch");
	end
end

function GuildBusinessLogClear()
	local Menu = LuaUI.GetMenuGuildProcess();
	if Menu ~= nil then
		Menu:ClearGuildBusinessLog();
	end
end

function CloseMenuGuildBusinessLogSearchPanel()
	local panel = UIManager:FindUIPanel("MenuGuildBusinessLogSearchPanel");
	if panel ~= nil then
		panel:SetEnable(false);
		UIManager:SetKeyInItem("EditLine");
	end
end

function HideGuildBusinessLogStuffs()
	local index = 1;
	while index <= 8 do
		local name = string.format("GuildBusinessLogName%d", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildBusinessLogLV%d", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildBusinessLogCount%d", index);
		UIManager:GetUIItem(name):SetEnable(false);
		name = string.format("GuildBusinessLogProfit%d", index);
		UIManager:GetUIItem(name):SetEnable(false);
		
		index = index + 1;
	end
end

function SelectGuildBusinessLogItem()
	local SelIndex = -1;
	local mouseX = UIManager:GetCursorX();
	local mouseY = UIManager:GetCursorY();
	local panel = UIManager:GetUIItem("MenuGuildBusinessLog");
	if panel ~= nil then
		local relX = UIManager:GetCursorX() - panel:GetX();
		local relY = UIManager:GetCursorY() - panel:GetY();
		local TargetX = 9;
		local TargetY = 65;
		local width = 350;
		local height = 25;
		for i = 0, 7 do
			if	relX >= TargetX and
				relX <= TargetX + width and
				relY >= TargetY and
				relY <= TargetY + height then
				
				local textName = string.format("GuildBusinessLogName%d", i + 1);
			
				local UIText = UIManager:GetUIItem(textName);
				if UIText ~= nil and UIText:IsEnable() then
					local UIImage = UIManager:GetUIItem("GuildBusinessLogHighLightBar");
					if UIImage ~= nil then
						UIImage:SetOffsetXY(TargetX, TargetY)
						UIImage:SetEnable(true);
					end
					SelIndex = i;
				end
				break;
			end
		TargetY = TargetY + height;
		end
	end
	
	if SelIndex == -1 then
		local UIImage = UIManager:GetUIItem("GuildBusinessLogHighLightBar");
		if UIImage ~= nil then
			UIImage:SetEnable(false);
		end
	end
	local Menu = LuaUI.GetMenuGuildProcess();
	Menu:SetGuildBusinessLogSelectIndex(SelIndex);
end

function HightLightGuildBusinessLog(SelIndex)
	local panel = UIManager:GetUIItem("MenuGuildBusinessLog");
	if panel ~= nil then
		local UIImage = UIManager:GetUIItem("GuildBusinessLogHighLightBar");
		if UIImage ~= nil then
			local TargetX = 9;
			local TargetY = 65;
			local width = 350;
			local height = 25;
			TargetY = TargetY + height * SelIndex
			UIImage:SetOffsetXY(TargetX, TargetY)
			UIImage:SetEnable(true);
		end
	end
end

function GuildAimChatScrollUp()
	local Chat = UIManager:FindUIChatPanel("GuildAimChat")
	Chat:UpRow();
end

function GuildAimChatScrollDown()
	local Chat = UIManager:FindUIChatPanel("GuildAimChat")
	Chat:DownRow();
end

