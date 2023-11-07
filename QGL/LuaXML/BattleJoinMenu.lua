BattleJoinIndex = -1;

function CloseJoinBattleApplicantPopup()
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("JoinBattleApplicantPopupPanel");
	if panel ~= nil then
		UIManager:AddDirtyUIItem(panel);
		panel:SetEnable(false);
	end
end

function JoinBattleClearAllRequestUp()
	LuaUI.JoinBattleClearAllRequestUp();
end

function JoinBattleUp()
	LuaUI.JoinBattleScroll(1);
end

function JoinScrollDown()
	LuaUI.JoinBattleScroll(0);
end



function CloseJoinBattleListPanel()
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("JoinBattleListPanel");
	if panel ~= nil then
		UIManager:AddDirtyUIItem(panel);
		UIManager:DeletePanel("JoinBattleListPanel");
	end
	
	panel = UIManager:GetUIItem("JoinBattleApplicantPopupPanel");
	if panel ~= nil then
		UIManager:AddDirtyUIItem(panel);
		UIManager:DeletePanel("JoinBattleApplicantPopupPanel");
	end
end

function JoinBattleApplicant_Popup(pX, pY)
	local applicantpop = UIManager:GetUIItem("JoinBattleApplicantPopupPanel");
	local applicantpopPanel = UIManager:FindUIPanelEx("JoinBattleApplicantPopupPanel");
	local panel = UIManager:GetUIItem("JoinBattleListPanel");
	
	local relX = UIManager:GetCursorX() - panel:GetX();
	local relY = UIManager:GetCursorY() - panel:GetY();
	
	if relX >= 3 and relX <= 421 and relY >= 54 and relY <= 203 then
		relY = relY - 54;
		BattleJoinIndex = LuaUI.Div(relY, 15);
		if applicantpop ~= nil and applicantpopPanel ~= nil then
			UIManager:SetPanelEx2Top(applicantpopPanel);
			applicantpop:SetEnable(true)
			applicantpop:SetPos(pX, pY)
			--UIManager:RenderAll();
		end
	end
end

function JoinBattleApplicantPopup(pX, pY)
	local applicantpop = UIManager:FindUIPopUpMenu("JoinBattleApplicantPopup")
	local PopSel = applicantpop:GetBarPos()

	if BattleJoinIndex ~= -1 then
		if PopSel == 0 then
			LuaUI.PartyPrivateCall(BattleJoinIndex);
		elseif PopSel == 1 then
			LuaUI.BattleJoinConfirm(BattleJoinIndex, true);
		elseif PopSel == 2 then
			LuaUI.BattleJoinConfirm(BattleJoinIndex, false);
		end
	end
	
	local apppop = UIManager:GetUIItem("JoinBattleApplicantPopupPanel");
	BattleJoinIndex = -1;
	apppop:SetEnable(false);

	UIManager:RenderAll();
end

function OpenJoinBattleList(pX, pY)
   local panel = UIManager:GetUIItem("JoinBattleListPanel");
   if panel == nil then
       LuaUI.AddXMLFile2UIManger("Data\\BattleJoinMenu.xml");
       LuaUI.UpdateJoinBattleMenu();
	    UIManager:RenderAll();
	    --local sfx = FSoundManager:CreateSoundA("Data\\Wav\\SoundI0001_open.wav");
	    --sfx:Play();
	else
		CloseJoinBattleListPanel();
   end

end
