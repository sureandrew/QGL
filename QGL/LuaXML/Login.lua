Font = "Data\\mingliu.ttc";
--Font = "DFFK_S3.ttc";
FontWidth = 12;
FontHeight = 12;
keyboardFlag = 0;
selectField = 1;
state = 0;


UIManager = LuaUI.GetUIManager()
--[[
OggPlayer = LuaUI.GetOggPlayer();
OggPlayer:SetVolume(0.02);
--]]

FSoundManager = LuaUI.GetFSoundManager();
FSoundManager:MusicSetVolume(0.02);

LuaUI.LoadLua("Data\\SelectServer.lua")
LuaUI.LoadLua("Data\\SelectCharacter.lua")
LuaUI.LoadLua("Data\\ForceLogout.lua")
LuaUI.LoadLua("Data\\Keyboard.lua")
LuaUI.LoadLua("Data\\CreateCharacter.lua")
LuaUI.LoadLua("Data\\AlertBox.lua")

function LoginNameOnChar(KeyCode)
	local EditBox = UIManager:FindUIEditBox("LoginName");

	if KeyCode == LuaUI.VK_RETURN then
		if EditString ~= nil then
			UIManager:RenderAll();
		end
	end
end



function PasswordOnChar(KeyCode)
	local EditBox = UIManager:FindUIEditBox("Password");

	if KeyCode == LuaUI.VK_RETURN then
	end
end


function CancelBtnDown(X, Y)
	LuaUI.PostQuitMessage(0);
end


function OKBtnDown(X, Y)
	UIManager:ChangeFocus("LoginName");
	local login = UIManager:FindUIEditBox("LoginName");
	local password = UIManager:FindUIEditBox("Password");
	local alertBox = UIManager:GetUIItem("AlertBoxPanel");
	local loginValue = login:GetText();
	local passwordValue = password:GetText();

 	if alertBox ~= nil then
	    local okButton = UIManager:GetUIItem("ConfirmAlert");
	    if okButton ~= nil and okButton:IsEnable() then
	        CloseAlertBox();
	        --UIManager:ChangeFocus("LoginName");
	    end
	    do return end;
	end

	if ( _G.state == 0 and _G.ForceLogoutFlag == 0 and
		 (loginValue ~= nil and loginValue ~= "") and
		 (passwordValue ~= nil and passwordValue ~= "")) then

		UIManager:SetEvent(100);

		_G.selectField = 1;
		_G.lockFlag = 0;
		_G.shiftFlag = 0;
		local panel = UIManager:GetUIItem("MiniKeyBoardPanel");

   		if panel ~= nill then
            panel:SetEnable(false);
		end
		_G.state = -1;
	elseif ( _G.ForceLogoutFlag == 1) then
			ForceLogout();
	elseif ( (_G.state == 1) and (_G.Character >= 0) and (_G.ForceLogoutFlag == 0)
		 and (_G.CreateCharacter == -1) and (_G.DeleteCharacter == -1) ) then
	          GoToLine();
    elseif ((_G.state == 2) and (_G.Server >= 0) and (_G.ForceLogoutFlag == 0) ) then
	        GoToGame();
	end

end

function InitLogin()
	local LoginNameEdit = UIManager:FindUIEditBox("LoginName");
	LoginNameEdit:ClearText();
	local PasswordEdit = UIManager:FindUIEditBox("Password");
	PasswordEdit:ClearText();

	local tempItem1 = UIManager:GetUIItem("LoginName");
	local tempItem2 = UIManager:GetUIItem("Password");
	UIManager:AddDirtyUIItem(tempItem1);
	UIManager:AddDirtyUIItem(tempItem2);
	_G.selectField = 1;
	_G.lockFlag = 0;
	_G.shiftFlag = 0;
	_G.state = 0;

	local Keyboard = UIManager:FindUICheckBox("KeyboardEnable");
	Keyboard:SetState(false);
	local SaveLogin = UIManager:FindUICheckBox("SaveLogin");
	SaveLogin:SetState(false);
	LuaUI.InitLoginScreen();

end

function CapKeyUp(Key1, Key2)

end

function ShowKeyboard()
   local panel = UIManager:GetUIItem("MiniKeyBoardPanel");

   	if panel ~= nill then
        if _G.keyboardFlag == 0 then
            panel:SetEnable(true);
            _G.keyboardFlag = 1;
		else
		    panel:SetEnable(false);
		    _G.keyboardFlag = 0;
		end
	else
	    LuaUI.AddXMLFile2UIManger("Data\\MiniKeyboard.xml");
	    _G.keyboardFlag = 1;
    end
   UIManager:RenderAll();
end

function SelectNameField()
   _G.selectField = 1;
end

function SelectPasswordField()
   _G.selectField = 2;
end

function SetLoginState(i)
   _G.state = i  ;
end

function CloseMessageBox()
	local panel = UIManager:FindUIPanelEx("MessageBoxPanel");
	if panel ~= nil then
	    UIManager:DeletePanel("MessageBoxPanel");
	end
end

function LoginChangeEditBox(KeyCode)
    if KeyCode == LuaUI.VK_TAB then
        if _G.selectField == 1 then
            SelectPasswordField();
        else
            SelectNameField();
        end
    end
end

function DisconnectConfirm()
    LuaUI.AddXMLFile2UIManger("Data\\DisconnectConfirm.xml");
	local panel = UIManager:FindUIPanel("DisconnectConfirmPanel");
	if panel ~= nil then
	    UIManager:ClearMonoPanel();
		UIManager:SetMonoPanel(panel);
	end
	UIManager:RenderAll();
end

function DisconnectYes()
    LuaUI.PostQuitMessage(0);
end
