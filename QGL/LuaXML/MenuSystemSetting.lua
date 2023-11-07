RestartOnce = true;

QuitMenuOldMonoPanel = nil
QuitMenuOldMonoFlag = nil

function OpenConfirmQuitGame()
	local panel = UIManager:GetUIItem("ConfirmQuitGamePanel");
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\QuitGame.xml");
	    panel = UIManager:FindUIPanel("ConfirmQuitGamePanel");
		if panel ~= nil then
			QuitMenuOldMonoPanel = UIManager:GetMonoPanel();
			if QuitMenuOldMonoPanel == nil then
				QuitMenuOldMonoPanel = UIManager:GetMonoPanelEx()
			end
			QuitMenuOldMonoFlag = UIManager:IsLockMonoPanel();
			UIManager:SetLockMonoPanel(false);
			UIManager:ClearMonoPanel();
			UIManager:SetMonoPanel(panel);
	 	end
	else
	    CloseConfirmQuitGame()
	end
end

function OpenSystemSetting()

	local panel = UIManager:FindUIPanel("SystemSettingPanel");
	if panel == nil then
	    local monoPanel = UIManager:GetMonoItem();
	    if monoPanel == nil then
		    LuaUI.AddXMLFile2UIManger("Data\\MenuSystemSetting.xml");
		    ShowAllSettingResult();
		    PlayOpenInterfaceEffect();
		    panel = UIManager:FindUIPanel("SystemSettingPanel");
		    if panel ~= nil then
		        LuaUI.StopCharacter();
		        UIManager:SetMonoPanel(panel);
		    end
		end
	else
	    CloseSystemSetting();
	end
		
end

function CloseSystemSetting()
	UIManager:StopDrag();
	UIManager:ClearMonoPanel();
	local systemSetting = LuaUI.GetMenuSystemSetting();
	if systemSetting ~= nil then
		systemSetting:ResetTempVolume();
	end
	local panel = UIManager:GetUIItem("SystemSettingPanel");
    if panel ~= nil then
    	PlayCloseInterfaceEffect();
    end
	UIManager:DeletePanel("SystemSettingPanel");
end

function CloseConfirmQuitGame()
	UIManager:StopDrag();
    UIManager:ClearMonoPanel();
    UIManager:DeletePanel("ConfirmQuitGamePanel");
	if QuitMenuOldMonoPanel then
		UIManager:SetMonoPanel(QuitMenuOldMonoPanel)
		UIManager:SetLockMonoPanel(QuitMenuOldMonoFlag)
	end
	--[[
    local panel = UIManager:FindUIPanel("SystemSettingPanel");
    if panel ~= nil then
        UIManager:SetMonoPanel(panel);
    end
	--]]
end

function ConfirmQuitGame()
    LuaUI.PostQuitMessage(0);
end

function ResetSystemSetting()

		local systemSetting = LuaUI.GetMenuSystemSetting();
	    if systemSetting ~= nil then
	    
	    	local checkIcon = UIManager:FindUICheckBox("WindowModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(true);
	    	end
	    	
	    	checkIcon = UIManager:FindUICheckBox("NightModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(true);
	    	end
	    	
	    	checkIcon = UIManager:FindUICheckBox("GivenModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(false);
	    	end
	    	
	    	checkIcon = UIManager:FindUICheckBox("PartyModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(false);
	    	end
	    	
	    	checkIcon = UIManager:FindUICheckBox("MessageModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(false);
	    	end
	    	
	    	checkIcon = UIManager:FindUICheckBox("MusicModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(true);
	    	end
	    	
	    	checkIcon = UIManager:FindUICheckBox("SoundEffectModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(true);
	    	end
	    	
	    	checkIcon = UIManager:FindUICheckBox("MapAnimeModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(true);
	    	end
	    	
	    	checkIcon = UIManager:FindUICheckBox("TradingModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(false);
	    	end
	    	
	    	checkIcon = UIManager:FindUICheckBox("WeatherModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(true);
	    	end
	    	
	    	checkIcon = UIManager:FindUICheckBox("PKModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(false);
	    	end
	    	
	    	checkIcon = UIManager:FindUICheckBox("MakeFriendModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(false);
	    	end
	    	
			checkIcon = UIManager:FindUICheckBox("ShowEquipModeSetting");
    		if	checkIcon ~= nil then 
	    			checkIcon:SetState(true);
	    	end
			
	    	for i=1, 10 do
	    		local str = "MusicVolume"..i;
	    		local image = UIManager:GetUIItem(str);
	    		if image ~= nil then	    			
	    		    if i <= 8 then
	    				image:SetEnable(true);
					else
					    image:SetEnable(false);
					end
	    		end
	    		                        
	    		str = "SoundEffectVolume"..i;
	    		image = UIManager:GetUIItem(str);
	    		if image ~= nil then
        			if i <= 8 then
	    				image:SetEnable(true);
					else
					    image:SetEnable(false);
					end
	    		end
	    	end
	    	systemSetting:RestoreTempVolumeToDefault();
	    	
	    end	
	    panel = UIManager:GetUIItem("SystemSettingPanel");
	    UIManager:AddDirtyUIItem(panel);
end

function SaveSystemSetting()
	local systemSetting = LuaUI.GetMenuSystemSetting();
	if systemSetting ~= nil then
		local checkIcon = UIManager:FindUICheckBox("WindowModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetWindowMode(checkIcon:GetState());
    			if LuaUI.IsFullScreen() == false and systemSetting:IsWindowModeEnable() == false then
    				checkIcon:SetState(true);
    				systemSetting:SetWindowMode(checkIcon:GetState());
    				local temp = UIManager:GetUIItem("WindowModeSetting"); 
	    			UIManager:AddDirtyUIItem(temp);
	    			LuaUI.ShowErrMessage("MSG_SYS_FULL_SCREEN_FAIL");
    			end
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("NightModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetNightMode(checkIcon:GetState());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("GivenModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetBlockGivenMode(checkIcon:GetState());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("PartyModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetBlockPartyMode(checkIcon:GetState());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("MessageModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetBlockMessageMode(checkIcon:GetState());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("MusicModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetMusicMode(checkIcon:GetState());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("SoundEffectModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetSoundEffectMode(checkIcon:GetState());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("MapAnimeModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetMapAnimeMode(checkIcon:GetState());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("TradingModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetBlockTradingMode(checkIcon:GetState());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("WeatherModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetWeatherMode(checkIcon:GetState());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("PKModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetBlockPKMode(checkIcon:GetState());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("MakeFriendModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetBlockMakeFriendMode(checkIcon:GetState());
    	end			
		checkIcon = UIManager:FindUICheckBox("ShowEquipModeSetting");
		if	checkIcon ~= nil then 
    			systemSetting:SetShowEquipMode(checkIcon:GetState());
    	end		
    	systemSetting:SaveBlockSetting();
    	systemSetting:WriteSystemSetting();
	end		
end

function ShowAllSettingResult()
	local systemSetting = LuaUI.GetMenuSystemSetting();
    if systemSetting ~= nil then
    
    	local checkIcon = UIManager:FindUICheckBox("WindowModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsWindowModeEnable());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("NightModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsNightModeEnable());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("GivenModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsBlockGivenModeEnable());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("PartyModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsBlockPartyModeEnable());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("MessageModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsBlockMessageModeEnable());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("MusicModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsMusicModeEnable());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("SoundEffectModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsSoundEffectModeEnable());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("MapAnimeModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsMapAnimeModeEnable());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("TradingModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsBlockTradingModeEnable());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("WeatherModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsWeatherModeEnable());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("PKModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsBlockPKModeEnable());
    	end
    	
    	checkIcon = UIManager:FindUICheckBox("MakeFriendModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsBlockMakeFriendModeEnable());
    	end
    	
		checkIcon = UIManager:FindUICheckBox("ShowEquipModeSetting");
		if	checkIcon ~= nil then 
    			checkIcon:SetState(systemSetting:IsShowEquipModeEnable());
    	end
		
    	for i=1, 10 do
    		local str = "MusicVolume"..i;
    		local image = UIManager:GetUIItem(str);
    		if image ~= nil then
    			if i <= systemSetting:GetTempMusicVolume() and systemSetting:IsMusicModeEnable() then
    				image:SetEnable(true);
    			else
    				image:SetEnable(false);
    			end
    		end
    		
    		str = "SoundEffectVolume"..i;
    		image = UIManager:GetUIItem(str);
    		if image ~= nil then
    			if i <= systemSetting:GetTempSoundEffectVolume() and systemSetting:IsSoundEffectModeEnable() then
    				image:SetEnable(true);
    			else
    				image:SetEnable(false);
    			end
    		end
    	end
        panel = UIManager:GetUIItem("SystemSettingPanel");
        UIManager:AddDirtyUIItem(panel);
    end	
end

function AddMusicVolume()
	local checkIcon = UIManager:FindUICheckBox("MusicModeSetting");
	local systemSetting = LuaUI.GetMenuSystemSetting();
	if checkIcon ~= nil and systemSetting ~= nil then
		local str = "";
		local volumeImage = "";
		if checkIcon:GetState() ~= true then
			checkIcon:SetState(true);
			for i=1, systemSetting:GetTempMusicVolume() do
				str = "MusicVolume"..i;
				volumeImage = UIManager:GetUIItem(str);
				if volumeImage ~= nil then
					volumeImage:SetEnable(true);
				end
			end
		end
		
	    systemSetting:AddMusicVolume();			
		str = "MusicVolume"..systemSetting:GetTempMusicVolume();
		volumeImage = UIManager:GetUIItem(str);
		if volumeImage ~= nil then
			volumeImage:SetEnable(true);
		end
	end
end

function SubMusicVolume()
	local checkIcon = UIManager:FindUICheckBox("MusicModeSetting");
	local systemSetting = LuaUI.GetMenuSystemSetting();
	if checkIcon ~= nil and systemSetting ~= nil then
		if checkIcon:GetState() then
			if systemSetting:GetTempMusicVolume() == 1 then
				checkIcon:SetState(false);
			end
		
			local str = "MusicVolume"..systemSetting:GetTempMusicVolume();
			local volumeImage = UIManager:GetUIItem(str);
			if volumeImage ~= nil then
				volumeImage:SetEnable(false);
			end
		    systemSetting:SubMusicVolume();	
		end		
	end
end

function AddSoundEffectVolume()
	local checkIcon = UIManager:FindUICheckBox("SoundEffectModeSetting");
	local systemSetting = LuaUI.GetMenuSystemSetting();
	if checkIcon ~= nil and systemSetting ~= nil then
		local str = "";
		local volumeImage = "";
		if checkIcon:GetState() ~= true then
			checkIcon:SetState(true);
			for i=1, systemSetting:GetTempSoundEffectVolume() do
				str = "SoundEffectVolume"..i;
				volumeImage = UIManager:GetUIItem(str);
				if volumeImage ~= nil then
					volumeImage:SetEnable(true);
				end
			end
		end
		
	    systemSetting:AddSoundEffectVolume();			
		str = "SoundEffectVolume"..systemSetting:GetTempSoundEffectVolume();
		volumeImage = UIManager:GetUIItem(str);
		if volumeImage ~= nil then
			volumeImage:SetEnable(true);
		end
	end
end

function SubSoundEffectVolume()
	local checkIcon = UIManager:FindUICheckBox("SoundEffectModeSetting");
	local systemSetting = LuaUI.GetMenuSystemSetting();
	if checkIcon ~= nil and systemSetting ~= nil then
		if checkIcon:GetState() then
			if systemSetting:GetTempSoundEffectVolume() == 1 then
				checkIcon:SetState(false);
			end
		
			local str = "SoundEffectVolume"..systemSetting:GetTempSoundEffectVolume();
			local volumeImage = UIManager:GetUIItem(str);
			if volumeImage ~= nil then
				volumeImage:SetEnable(false);
			end
		    systemSetting:SubSoundEffectVolume();	
		end		
	end
end

function MusicModeLua()
	local checkIcon = UIManager:FindUICheckBox("MusicModeSetting");
	local systemSetting = LuaUI.GetMenuSystemSetting();
	if checkIcon ~= nil and systemSetting ~= nil then
		
		if checkIcon:GetState() then
			for i=1, 10 do
	    		local str = "MusicVolume"..i;
	    		local image = UIManager:GetUIItem(str);
	    		if image ~= nil then
	    			if i <= systemSetting:GetTempMusicVolume() then
	    				image:SetEnable(true);
	    			else
	    				image:SetEnable(false);
	    			end
	    		end
	    	end				
		else
			for i=1, 10 do
	    		local str = "MusicVolume"..i;
	    		local image = UIManager:GetUIItem(str);
	    		if image ~= nil then
	    			image:SetEnable(false);	    			
	    		end
	    	end
		end
	end	
end

function SoundEffectModeLua()
	local checkIcon = UIManager:FindUICheckBox("SoundEffectModeSetting");
	local systemSetting = LuaUI.GetMenuSystemSetting();
	if checkIcon ~= nil and systemSetting ~= nil then
		
		if checkIcon:GetState() then
			for i=1, 10 do
	    		local str = "SoundEffectVolume"..i;
	    		local image = UIManager:GetUIItem(str);
	    		if image ~= nil then
	    			if i <= systemSetting:GetTempSoundEffectVolume() then
	    				image:SetEnable(true);
	    			else
	    				image:SetEnable(false);
	    			end
	    		end
	    	end				
		else
			for i=1, 10 do
	    		local str = "SoundEffectVolume"..i;
	    		local image = UIManager:GetUIItem(str);
	    		if image ~= nil then
	    			image:SetEnable(false);	    			
	    		end
	    	end
		end
	end	
end

function RestartGame()
	CloseMiddleMap();
	LuaUI.PostQuitMessage(0);
	if _G.RestartOnce then
    	LuaUI.RunNewGame();
    	_G.RestartOnce = false;
	end
end