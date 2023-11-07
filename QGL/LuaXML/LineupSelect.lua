function CloseLineupMenu(pX, pY)
	UIManager:StopDrag();
	local panel0 = UIManager:GetUIItem("LineupSelectPanel");
	if panel0 ~= nil then
		panel0:SetEnable(false);
		UIManager:AddDirtyUIItem(panel0);
		UIManager:DeletePanel("LineupSelectPanel");
	end
	local panel1 = UIManager:GetUIItem("ForgetLineupConfirmPanel");
	if panel1 ~= nil then
		panel1:SetEnable(false);
		UIManager:AddDirtyUIItem(panel1);
		UIManager:DeletePanel("ForgetLineupConfirmPanel");
	end
	
	local panel2 = UIManager:GetUIItem("MenuLearnedLineup");
	if panel2 ~= nil then
		panel2:SetEnable(false);
		UIManager:AddDirtyUIItem(panel2);
		UIManager:DeletePanel("MenuLearnedLineup");
	end
end

function FunForgetLineup()
	UIManager:StopDrag();
	local panel = UIManager:FindUIPanel("ForgetLineupConfirmPanel");
	if panel ~= nil then
		panel:SetEnable(true);
		UIManager:SetPanel2Top(panel);
	end
end

function FunSelectLineup()
	LuaUI.PartySelectLineup();
end

function FunKeyPosSelectA()
	LuaUI.PartySetKeyPos(0);
end

function FunKeyPosSelectB()
	LuaUI.PartySetKeyPos(1);
end

function FunKeyPosSelectC()
	LuaUI.PartySetKeyPos(2);
end

function FunKeyPosSelectD()
	LuaUI.PartySetKeyPos(3);
end

function FunKeyPosSelectE()
	LuaUI.PartySetKeyPos(4);
end

function FunOpenLearnedLineupList()
	UIManager:StopDrag();
	local LineupPanel = UIManager:GetUIItem("LineupSelectPanel");
	local panel = UIManager:FindUIPanelEx("MenuLearnedLineup");
	if panel ~= nil then
  	local xPos=LineupPanel:GetX()+81;
	  local yPos=LineupPanel:GetY()+58;
	  panel:SetPos(xPos,yPos);
		panel:SetEnable(true);
		local List = UIManager:GetUIItem("LearnedLineup");
		if List ~= nil then
			List:SetEnable(true);
		end
		UIManager:SetPanelEx2Top(panel);
	end
end

function FunSetSelectResualt()
  UIManager:StopDrag();
	local panel = UIManager:GetUIItem("MenuLearnedLineup");
	if panel ~= nil then
		panel:SetEnable(false);
		local List = UIManager:GetUIItem("LearnedLineup");
		if List ~= nil then
			List:SetEnable(false);
		end
	end
	LuaUI.PartySetSelectLineupResualt();
end

function MenuLearnedLineupPopupTimeOut()
	CloseMenuLearnedLineupPopup();
end

function CloseMenuLearnedLineupPopup()
	local teammatepop = UIManager:GetUIItem("MenuLearnedLineup");
	if teammatepop ~= nil then
		teammatepop:SetEnable(false);
		UIManager:AddDirtyUIItem(teammatepop);
	end

end

function StartMenuLearnedLineupCount()
	local teammatepop = UIManager:GetUIItem("MenuLearnedLineup");
	if teammatepop ~= nil then
		teammatepop:ResetTimer();
		teammatepop:SetEnableTimeOutEvent(true);
	end
end

function HoldMenuLearnedLineupPopup()
	local teammatepop = UIManager:GetUIItem("MenuLearnedLineup");
	if teammatepop ~= nil then
		teammatepop:SetEnableTimeOutEvent(false);
	end
end

function CancleForgiveLineup()
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("ForgetLineupConfirmPanel");
	if panel ~= nil then
		panel:SetEnable(false);
	end
end
function ComfirmForgiveLineup()
	LuaUI.ForgetLearnedLineup();
	CancleForgiveLineup();
end
