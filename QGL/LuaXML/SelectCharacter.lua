Character = -1;
DeleteCharacter = -1;
RestoreCharacterIndex = -1;

function CreateCharacterShowUp()
	LuaUI.AddXMLFile2UIManger("Data\\CreateCharacter.xml");
	UIManager:ChangeFocus("CharacterName");
	local choosenCharacterPanel = UIManager:GetUIItem("CharacterChosenPanel");
	choosenCharacterPanel:SetEnable(false);
	InitCreateCharacter();
	LuaUI.ShowCreateCharacter(0,4);
	_G.CreateCharacter = 1;
	UIManager:RenderAll();

end

function SelectCharacter1()
	ResetCharacterSelected();
    _G.Character = 0;

	local button = UIManager:FindUIButton("CharacterSelected1");
	if button ~= nil then
	    button:SetClickState();
		button:SetLockState(true);
	end
	LuaUI.SetAniStateInLogin(_G.Character, 1);
	LuaUI.SetCharacterIndex(_G.Character);

	_G.state = 1;
end

function SelectCharacter2()
    ResetCharacterSelected();
	_G.Character = 1;

	local button = UIManager:FindUIButton("CharacterSelected2");
	if button ~= nil then
  		button:SetClickState();
		button:SetLockState(true);
	end
	LuaUI.SetAniStateInLogin(_G.Character, 1);
	LuaUI.SetCharacterIndex(_G.Character);
 	_G.state = 1;
end

function SelectCharacter3()
    ResetCharacterSelected();
	_G.Character = 2;

	local button = UIManager:FindUIButton("CharacterSelected3");
	if button ~= nil then
  		button:SetClickState();
		button:SetLockState(true);
	end
	LuaUI.SetAniStateInLogin(_G.Character, 1);
	LuaUI.SetCharacterIndex(_G.Character);
 	_G.state = 1;
end


function RecoverCharacter1()

    _G.RestoreCharacterIndex = 0;
 	LuaUI.AddXMLFile2UIManger("Data\\RestoreCharacter.xml");
	local panel = UIManager:FindUIPanel("ConfirmRestoreCharacterPanel");
	if panel ~= nil then
	    UIManager:SetMonoPanel(panel);
	    panel:SetEnable(true);
	end

end

function RecoverCharacter2()

    _G.RestoreCharacterIndex = 1;
 	LuaUI.AddXMLFile2UIManger("Data\\RestoreCharacter.xml");
	local panel = UIManager:FindUIPanel("ConfirmRestoreCharacterPanel");
	if panel ~= nil then
	    UIManager:SetMonoPanel(panel);
	    panel:SetEnable(true);
	end

end

function RecoverCharacter3()

    _G.RestoreCharacterIndex = 2;
 	LuaUI.AddXMLFile2UIManger("Data\\RestoreCharacter.xml");
	local panel = UIManager:FindUIPanel("ConfirmRestoreCharacterPanel");
	if panel ~= nil then
	    UIManager:SetMonoPanel(panel);
	    panel:SetEnable(true);
	end

end



function ResetCharacterSelected()
 	_G.state = 1;
	_G.Character = -1;
	
	local button;
	
	for i=1,3 do
		button = UIManager:FindUIButton("CharacterSelected"..i);
		if button ~= nil then
		    button:SetLockState(false);
			button:SetNormalState();
		end
	end
	
	local image;
	for i=1,8 do
		image = UIManager:GetUIItem("CharChosenLImage"..i);
		if image ~= nil then 
			image:SetEnable(false);
		end
	end
	
	for i=0,2 do
	    LuaUI.SetAniStateInLogin(i, 0);
	end



end

function GoBackLogin()
    UIManager:SetEvent(109);
    InitLogin();
    ResetCharacterSelected();
    _G.state = -1;
    _G.Character = -1;
    UIManager:ChangeFocus("LoginName");
end

function GoToLine()

	if (_G.Character >= 0 and _G.state == 1) then
	    if LuaUI.HasCharacter(_G.Character) then
			LuaUI.SetCharacterIndex(_G.Character);
			UIManager:SetEvent(111);
	  		_G.state = -1;
  		end
	end
end

function SetGameState(state)
    _G.state = state;
end

function SelectCharHighLight(index)
	local str = "CharacterSelected"..index;
	local button = UIManager:FindUIButton(str);
	
	if button ~= nil and button:IsLockState()  then
        str = "CharacterSelectedHighLight"..index;
		local image = UIManager:GetUIItem(str);
		if image ~= nil and image:IsEnable() == false then
			button:SetEnable(false);
			image:SetEnable(true);
			UIManager:AddDirtyUIItem(button);
			UIManager:AddDirtyUIItem(image);
		end
	end

end

function ReturnOrg(index)
    local str = "CharacterSelected"..index;
 	local button = UIManager:FindUIButton(str);
 		str = "CharacterSelectedHighLight"..index;
	local temp = UIManager:GetUIItem(str);
	if button ~= nil and temp ~= nil and button:IsEnable() == false then

		button:SetEnable(true);
		temp:SetEnable(false);

		UIManager:AddDirtyUIItem(button);
		UIManager:AddDirtyUIItem(temp);
	end
end

function SelectCharHighLight1()
	SelectCharHighLight(1);
end

function ReturnOrg1()
	ReturnOrg(1);
end

function SelectCharHighLight2()
	SelectCharHighLight(2);
end

function ReturnOrg2()
	ReturnOrg(2);
end

function SelectCharHighLight3()
	SelectCharHighLight(3);
end

function ReturnOrg3()
	ReturnOrg(3);
end

function HighLight1()
	LuaUI.SetHighLightInLogin(1, true);
end

function ResetHighLight1()
	LuaUI.SetHighLightInLogin(1, false);
end

function OpenDeleteCharacter()
	if _G.Character >= 0 then
		LuaUI.AddXMLFile2UIManger("Data\\DeleteCharacter.xml");
		local panel = UIManager:FindUIPanel("ConfirmDeleteCharacterPanel");
		if panel ~= nil then
		    LuaUI.ShowRemoveCharacterName();
		    UIManager:SetMonoPanel(panel);
		    _G.DeleteCharacter = 1;
		end
		UIManager: SetKeyInItem("DeleteCharacterEditBox");
	end
end

function ConfirmDeleteCharacter()
	LuaUI.DeleteCharacter();
	CloseConfirmDeleteCharacter();
end

function CloseConfirmDeleteCharacter()
	local panel = UIManager:FindUIPanel("ConfirmDeleteCharacterPanel");
	if panel ~= nil then
		UIManager:ClearMonoPanel();
		UIManager:ChangeFocus("LoginName");
		UIManager:DeletePanel("ConfirmDeleteCharacterPanel");
	end
	_G.DeleteCharacter = -1;
end

function DblClickCharacter1()
	SelectCharacter1();
	GoToLine();
end

function DblClickCharacter2()
	SelectCharacter2();
	GoToLine();
end

function DblClickCharacter3()
	SelectCharacter3();
	GoToLine();
end

function RestoreCharacterConfirm()

	LuaUI.RetrieveCharacter(_G.RestoreCharacterIndex);
	CloseRestoreCharacterPanel()

end

function CloseRestoreCharacterPanel()

    local panel = UIManager:FindUIPanel("ConfirmRestoreCharacterPanel");
	if panel ~= nil then
		UIManager:ClearMonoPanel();
		UIManager:DeletePanel("ConfirmRestoreCharacterPanel");
	end
	_G.RestoreCharacterIndex = -1;

end


