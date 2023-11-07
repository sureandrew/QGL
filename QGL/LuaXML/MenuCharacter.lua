function CloseCharacterMenu(pX, pY)
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("CharacterMenuPanel");
	if panel ~= nil then 
		UIManager:AddDirtyUIItem(panel);
		panel:SetEnable(false);
		UIManager:DeletePanel("CharacterMenuPanel");
		CloseFactionDetailMenu();
		CloseTitle();

		PlayCloseInterfaceEffect();
	end
end

function CharLevelUp()
	--[[local button = UIManager:GetUIItem("CharLevelUp");
	if button ~= nil then
		button:SetDisableState();
		UIManager:AddDirtyUIItem(button);
	end ]]
	LuaUI.CharacterLevelUp();
end

function CharAttOk()
	LuaUI.CharAttOk();
	PlayConfirmPointEffect();
end

function CharSTRPlus(AInc)
  if AInc and (AInc > 0) then
    LuaUI.CharAttUp(2, AInc);
  else
    LuaUI.CharAttUp(2);
	end;
	PlayPointAddEffect();
end

function CharSTRMinus(ADec)
  if ADec and (ADec > 0) then
    LuaUI.CharAttDown(2, ADec);
  else
    LuaUI.CharAttDown(2);
	end;
	PlayPointDecreaseEffect();
end

function CharCONPlus()
	LuaUI.CharAttUp(3);
	PlayPointAddEffect();
end

function CharCONMinus()
	LuaUI.CharAttDown(3);
	PlayPointDecreaseEffect();
end

function CharSTAPlus()
	LuaUI.CharAttUp(0);
	PlayPointAddEffect();
end

function CharSTAMinus()
	LuaUI.CharAttDown(0);
	PlayPointDecreaseEffect();
end

function CharAGIPlus()
	LuaUI.CharAttUp(4);
	PlayPointAddEffect();
end

function CharAGIMinus()
	LuaUI.CharAttDown(4);
	PlayPointDecreaseEffect();
end

function CharSPRPlus()
	LuaUI.CharAttUp(1);
	PlayPointAddEffect();
end

function CharSPRMinus()
	LuaUI.CharAttDown(1);
	PlayPointDecreaseEffect();
end

function OpenFactionDetail()
	local panel = UIManager:GetUIItem("FactionDetailMenuPanel");
	if panel == nil then 
		LuaUI.AddXMLFile2UIManger("Data\\FactionDetail.xml");
		LuaUI.OpenFactionDetail();
	else
		CloseFactionDetailMenu();
	end
end

function CloseFactionDetailMenu()
	UIManager:DeletePanel("FactionDetailMenuPanel");
end