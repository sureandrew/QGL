ApplicantIndex = -1;
TeammateIndex = -1;     --from 0 ~ 4

function ResetApplicantIndex()
	_G.ApplicantIndex = -1;
	local img = UIManager:GetUIItem("SelectedApplicant");
	if img ~= nil then
	    img:SetEnable(false);
	end
end

function CloseTeammateMenu(pX, pY)
	UIManager:StopDrag();
	CloseLineupMenu();
	local panel = UIManager:GetUIItem("TeammatePanel");
	if panel ~= nil then
		UIManager:AddDirtyUIItem(panel);
		panel:SetEnable(false);
		UIManager:DeletePanel("TeammatePanel");
		CloseApplyMembershipListPanel(0,0);
		CloseMenuTeammatePopup();
		PlayCloseInterfaceEffect();
		CloseTeamDescription();
	end
end

function OpenApplyMembershipList(pX, pY)
   local panel = UIManager:GetUIItem("ApplyMembershipListPanel");
   if panel == nil then
       LuaUI.AddXMLFile2UIManger("Data\\ApplyMembershipListPanel.xml");
       LuaUI.UpdateApplicationMenu();
	    --UIManager:RenderAll();
	    --local sfx = FSoundManager:CreateSoundA("Data\\Wav\\SoundI0001_open.wav");
	    --sfx:Play();
	else
		CloseApplyMembershipListPanel(0,0);
   end

end

function InviteToJoin(pX, pY)
    CloseTeammateMenu(0,0);
    LuaUI.BBParty();

end

function InviteAccept()

	LuaUI.InviteConfirm(true);
	CloseTeammateMenu(0,0);
end

function InviteReject()
	LuaUI.InviteConfirm(false);

	if LuaUI.GetPartyListSize() == 0 then
		CloseTeammateMenu(0,0);
	else
		LuaUI.ShowPartyByIndex();

	end

end

function PrevTeam()
		LuaUI.ShowOtherParty(0); --0:prev team
		--UIManager:RenderAll();
end

function NextTeam()
		LuaUI.ShowOtherParty(1); --1:next team
		--UIManager:RenderAll();
end

function ApplicantPopupTimeOut()
	CloseApplicantPopup();
end

function CloseApplicantPopup()
	local applicantpop = UIManager:GetUIItem("ApplicantPopupPanel");
	if applicantpop ~= nil then
		applicantpop:SetEnable(false);
		UIManager:AddDirtyUIItem(applicantpop);
	end

end

function StartApplicantCount()
	local applicantpop = UIManager:GetUIItem("ApplicantPopupPanel");
	if applicantpop ~= nil then
		applicantpop:ResetTimer();
		applicantpop:SetEnableTimeOutEvent(true);
	end
end

function HoldApplicantPopup()
	local applicantpop = UIManager:GetUIItem("ApplicantPopupPanel");
	if applicantpop ~= nil then
		applicantpop:SetEnableTimeOutEvent(false);
	end
end

function ApplicantPopup(pX, pY)
	local applicantpop = UIManager:FindUIPopUpMenu("ApplicantPopup")
	local PopSel = applicantpop:GetBarPos()

	if _G.ApplicantIndex ~= -1 then
		if PopSel == 0 then
			LuaUI.ApplicantPrivateCall(_G.ApplicantIndex);
		elseif PopSel == 1 then
			LuaUI.JoinConfirm(_G.ApplicantIndex, true);
		elseif PopSel == 2 then
			LuaUI.JoinConfirm(_G.ApplicantIndex, false);
		end
	end

	local apppop = UIManager:GetUIItem("ApplicantPopupPanel");
 	ResetApplicantIndex();
	apppop:SetEnable(false);

	--UIManager:RenderAll();
end

function Applicant_Popup(pX, pY)
	local applicantpop = UIManager:GetUIItem("ApplicantPopupPanel");
	local applicantpopPanel = UIManager:FindUIPanelEx("ApplicantPopupPanel");
	local panel = UIManager:GetUIItem("ApplyMembershipListPanel");
	local img = UIManager:GetUIItem("SelectedApplicant");

	local relX = UIManager:GetCursorX() - panel:GetX();
	local relY = UIManager:GetCursorY() - panel:GetY();
	ResetApplicantIndex();

	if relX >= 3 and relX <= 421 and relY >= 54 and relY <= 203 then
		relY = relY - 54;
		_G.ApplicantIndex = LuaUI.Div(relY, 15);
		if applicantpop ~= nil and applicantpopPanel ~= nil and img ~= nil then
			UIManager:SetPanelEx2Top(applicantpopPanel);
			applicantpop:SetEnable(true);
			applicantpop:SetPos(pX, pY);
			local i = _G.ApplicantIndex + 1;
			local stringName = "Applicant"..i;
			local applicantImg = UIManager:GetUIItem(stringName);
			if applicantImg ~= nil then
				img:SetOffsetXY(applicantImg:GetOffsetX(), applicantImg:GetOffsetY());
				img:SetEnable(true);
			end
			--UIManager:RenderAll();
		end
	end
end

function SetApplicant()
    local panel = UIManager:GetUIItem("ApplyMembershipListPanel");
    local img = UIManager:GetUIItem("SelectedApplicant");
    local relX = UIManager:GetCursorX() - panel:GetX();
	local relY = UIManager:GetCursorY() - panel:GetY();

    ResetApplicantIndex();
	if relX >= 3 and relX <= 421 and relY >= 54 and relY <= 203 then
		relY = relY - 54;
		_G.ApplicantIndex = LuaUI.Div(relY, 15);
		local i = _G.ApplicantIndex + 1;
		local stringName = "Applicant"..i;
		local applicantImg = UIManager:GetUIItem(stringName);
		if applicantImg ~= nil and img ~= nil then
			img:SetOffsetXY(applicantImg:GetOffsetX(), applicantImg:GetOffsetY());
			img:SetEnable(true);
		end

	end
end

function JoinAcceptButton()
	if _G.ApplicantIndex > -1 and _G.ApplicantIndex < 10 then
	    LuaUI.JoinConfirm(_G.ApplicantIndex, true);
	end
end

function JoinRejectButton()
    if _G.ApplicantIndex > -1 and _G.ApplicantIndex < 10 then
        LuaUI.JoinConfirm(_G.ApplicantIndex, false);
	end
end

function ClearAllRequest()
	LuaUI.ClearAllRequest();
end

function ApplyScrollUp()
	LuaUI.ApplicantPanelScroll(1);
end

function ApplyScrollDown()
	LuaUI.ApplicantPanelScroll(0);
end

function ApplyScroll()
	LuaUI.ApplicantPanelScroll(2);
end

function ShowTeammatePopUp(pX, pY, index)

	if index >= 0 and index < 5 then
		local teammatepop = UIManager:GetUIItem("MenuTeammatePopupPanel");
		local teammatepopPanel = UIManager:FindUIPanelEx("MenuTeammatePopupPanel");
		local panel = UIManager:GetUIItem("TeammatePopUp"..(index+1));
		_G.TeammateIndex = index ;

		local relX = UIManager:GetCursorX() - panel:GetX();
		local relY = UIManager:GetCursorY() - panel:GetY();

		if relX >= 0 and relX <= 126 and relY >= 0 and relY <= 129 then
			if teammatepop ~= nil and teammatepopPanel ~= nil then
				LuaUI.ShowTeammatePopUp(index);
				UIManager:SetPanelEx2Top(teammatepopPanel);
				teammatepop:SetPos(pX, pY);
				teammatepop:SetEnable(true);

			end
		end
	end
end

function ShowTeammatePopUp1(pX, pY)
	ShowTeammatePopUp(pX, pY, 0);
end

function ShowTeammatePopUp2(pX, pY)
	ShowTeammatePopUp(pX, pY, 1);
end

function ShowTeammatePopUp3(pX, pY)
	ShowTeammatePopUp(pX, pY, 2);
end

function ShowTeammatePopUp4(pX, pY)
	ShowTeammatePopUp(pX, pY, 3);
end

function ShowTeammatePopUp5(pX, pY)
	ShowTeammatePopUp(pX, pY, 4);
end

function MenuTeammatePopupTimeOut()
	CloseMenuTeammatePopup();
end

function CloseMenuTeammatePopup()
	local teammatepop = UIManager:GetUIItem("MenuTeammatePopupPanel");
	if teammatepop ~= nil then
		teammatepop:SetEnable(false);
		UIManager:AddDirtyUIItem(teammatepop);
	end

end

function StartMenuTeammateCount()
	local teammatepop = UIManager:GetUIItem("MenuTeammatePopupPanel");
	if teammatepop ~= nil then
		teammatepop:ResetTimer();
		teammatepop:SetEnableTimeOutEvent(true);
	end
end

function HoldMenuTeammatePopup()
	local teammatepop = UIManager:GetUIItem("MenuTeammatePopupPanel");
	if teammatepop ~= nil then
		teammatepop:SetEnableTimeOutEvent(false);
	end
end

function MenuTeammatePopup()
	local teammatepop = UIManager:FindUIPopUpMenu("MenuTeammatePopup")
	local PopSel = teammatepop:GetBarPos()

	if  _G.TeammateIndex ~= -1 then
		--[[if PopSel == 0 then
			LuaUI.TeammatePrivateCall(_G.TeammateIndex);
		elseif PopSel == 1 then
			LuaUI.TeammateKick(_G.TeammateIndex);
		elseif PopSel == 2 then
		    LuaUI.TeammatePromote(_G.TeammateIndex);
		elseif PopSel == 3 then

		elseif PopSel == 4 then

		elseif PopSel == 5 then

		elseif PopSel == 6 then

		elseif PopSel == 7 then
			LuaUI.PartyQuit();
		end ]]
		LuaUI.SelectTeammatePopUp(PopSel, _G.TeammateIndex);
	end


	local teampop = UIManager:GetUIItem("MenuTeammatePopupPanel");

	teampop:SetEnable(false);
end

function LeaveParty()
    LuaUI.PartyQuit();
end

function SelectLineup(pX, pY)
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("LineupSelectPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\LineupSelect.xml");
		LuaUI.UpdateLineupSelectMenu();
	else
		CloseLineupMenu();
	end
end

function FunSetChangePos2()
	LuaUI.SetChangePos(1);
end

function FunSetChangePos3()
	LuaUI.SetChangePos(2);
end

function FunSetChangePos4()
	LuaUI.SetChangePos(3);
end

function FunSetChangePos5()
	LuaUI.SetChangePos(4);
end

function FunLineupInfoView()
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("LineupSelectPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\LineupSelect.xml");
		LuaUI.OpenLineupInfoView();
	else
		CloseLineupMenu();
	end
end


function LeaveTemporarily()
	LuaUI.LeaveTemporarily();
end

function ReturnParty()
	LuaUI.ReturnParty();
end

--TeamDescription
function TeamDescriptionButton()
	UIManager:StopDrag();
	OpenTeamDescription();
end

--AutoAcceptCheckBox
function AutoAcceptButton()
	local checkIcon = UIManager:FindUICheckBox("AutoAcceptCheckBox");
	if checkIcon ~= nil then
		if checkIcon:GetState() ~= true then
			checkIcon:SetState(true);
		else
			checkIcon:SetState(false);
		end
	end
	AutoAcceptCheckBox();
end

function AutoAcceptCheckBox()
	local checkIcon = UIManager:FindUICheckBox("AutoAcceptCheckBox");
	if checkIcon ~= nil then
		LuaUI.AutoAcceptCheckBox(checkIcon:GetState());
	end
end
