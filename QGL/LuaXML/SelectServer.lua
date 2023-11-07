Server = -1;
DoubleClick = 0;

function SelectServer()
    local ServerSelected = UIManager:GetUIItem("ServerSelected");
    local buttonName = "Server"..(_G.Server+1);
    local button = UIManager:GetUIItem(buttonName);
    if ServerSelected ~= nil and button ~= nil then
        ServerSelected:SetEnable(true);
        ServerSelected:SetOffsetXY( button:GetOffsetX(), button:GetOffsetY());

  	end
 	_G.state = 2;

end

function SelectServer1()
	if _G.Server == 0 then
		_G.DoubleClick = _G.DoubleClick + 1;
	else
		_G.DoubleClick = 0;
	end

 if _G.DoubleClick == 1 then
		GoToGame()
	else
   		_G.Server = 0;
   		SelectServer();
   	end
end

function SelectServer2()
	if _G.Server == 1 then
		_G.DoubleClick = _G.DoubleClick + 1;
	else
		_G.DoubleClick = 0;
	end

	if _G.DoubleClick == 1 then
		GoToGame()
	else
   		_G.Server = 1;
   		SelectServer();
    end
end


function SelectServer3()
   	if _G.Server == 2 then
		_G.DoubleClick = _G.DoubleClick + 1;
	else
		_G.DoubleClick = 0;
	end

	if _G.DoubleClick == 1 then
		GoToGame()
	else
   		_G.Server = 2;
   		SelectServer();
    end
end


function SelectServer4()
   	if _G.Server == 3 then
		_G.DoubleClick = _G.DoubleClick + 1;
	else
		_G.DoubleClick = 0;
	end

	if _G.DoubleClick == 1 then
		GoToGame()
	else
   		_G.Server = 3;
   		SelectServer();
    end
end


function SelectServer5()
   	if _G.Server == 4 then
		_G.DoubleClick = _G.DoubleClick + 1;
	else
		_G.DoubleClick = 0;
	end

	if _G.DoubleClick == 1 then
		GoToGame()
	else
   		_G.Server = 4;
   		SelectServer();
    end
end


function SelectServer6()
   	if _G.Server == 5 then
		_G.DoubleClick = _G.DoubleClick + 1;
	else
		_G.DoubleClick = 0;
	end

	if _G.DoubleClick == 1 then
		GoToGame()
	else
   		_G.Server = 5;
   		SelectServer();
    end
end


function SelectServer7()
   	if _G.Server == 6 then
		_G.DoubleClick = _G.DoubleClick + 1;
	else
		_G.DoubleClick = 0;
	end

	if _G.DoubleClick == 1 then
		GoToGame()
	else
   		_G.Server = 6;
   		SelectServer();
    end
end


function SelectServer8()
   	if _G.Server == 7 then
		_G.DoubleClick = _G.DoubleClick + 1;
	else
		_G.DoubleClick = 0;
	end

	if _G.DoubleClick == 1 then
		GoToGame()
	else
   		_G.Server = 7;
   		SelectServer();
    end
end


function SelectServer9()
   	if _G.Server == 8 then
		_G.DoubleClick = _G.DoubleClick + 1;
	else
		_G.DoubleClick = 0;
	end

	if _G.DoubleClick == 1 then
		GoToGame()
	else
   		_G.Server = 8;
   		SelectServer();
    end
end


function SelectServer10()
   	if _G.Server == 9 then
		_G.DoubleClick = _G.DoubleClick + 1;
	else
		_G.DoubleClick = 0;
	end

	if _G.DoubleClick == 1 then
		GoToGame()
	else
   		_G.Server = 9;
   		SelectServer();
    end
end

function GoBackCharacter()
	if _G.ForceLogoutFlag == 0 then
		UIManager:SetEvent(105);
		ResetCharacterSelected();
		InitServerChosen();
	 	_G.state = 1;
		_G.Server = -1;
		_G.DoubleClick = 0;
	end
end

function InitServerChosen()

	local ServerSelected = UIManager:GetUIItem("ServerSelected");
    if ServerSelected ~= nil then
        ServerSelected:SetEnable(false);
    end
end

function GoToGame()

	if (_G.state == 2 and _G.Server >= 0 and _G.ForceLogoutFlag == 0) then
    	LuaUI.SetLineIndex(_G.Server + 1);
		--UIManager:SetEvent(110);
		_G.state = 3;
		UIManager:SetEvent(104);
	end
end
