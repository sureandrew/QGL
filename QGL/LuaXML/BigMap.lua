ShowBigMapDescFlag = false;

function OpenBigMap()
	local panel = UIManager:GetUIItem("BigMapPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\BigMap.xml");
		local Menu = LuaUI.GetMenuHelpProcess();
		Menu:HandleEvent(23);
		panel = UIManager:GetUIItem("BigMapPanel");
		local bigMap = LuaUI.GetBigMap();
		if bigMap ~= nil then
		    bigMap:DrawButton();
		    bigMap:SetOpenBigMap(true);
		end
		if panel ~= nil then
		    UIManager:AddDirtyUIItem(panel);
		end
		PlayOpenInterfaceEffect();
	else
		CloseBigMap();
		
	end
end

function CloseBigMap()
	UIManager:StopDrag();
	if _G.InBattle then
	    CloseBigMapDesc();
	    local bigMap = LuaUI.GetBigMap();
		if bigMap ~= nil then
		    bigMap:StartBigMapDescMode(false);
		    bigMap:SetOpenBigMap(false);
		end
	    _G.ShowBigMapDescFlag = false;
	    local panel = UIManager:GetUIItem("BigMapPanel");
	    if panel ~= nil then
	    	PlayCloseInterfaceEffect();
	    end
	    UIManager:DeletePanel("BigMapPanel");
		DeleteBigMapDesc();
		
	else
		if _G.ShowBigMapDescFlag then
			local bigMapDescPanel = UIManager:GetUIItem("BigMapDescPanel");
			if bigMapDescPanel ~= nil and bigMapDescPanel:IsEnable() then
				CloseBigMapDesc();
			else
			    local bigMap = LuaUI.GetBigMap();
				if bigMap ~= nil then
				    bigMap:StartBigMapDescMode(false);
				end
				_G.ShowBigMapDescFlag = false;
			end
		else
		    local bigMap = LuaUI.GetBigMap();
			if bigMap ~= nil then
			    bigMap:SetOpenBigMap(false);
			end
			local panel = UIManager:GetUIItem("BigMapPanel");
		    if panel ~= nil then
		    	PlayCloseInterfaceEffect();
		    end
			UIManager:DeletePanel("BigMapPanel");
			DeleteBigMapDesc();
			
		end
	end
end

function StartBigMapDescMode()
    local bigMap = LuaUI.GetBigMap();
	if bigMap ~= nil then
	    bigMap:StartBigMapDescMode(true);
	end
	_G.ShowBigMapDescFlag = true;
end

function StopBigMapDescMode()
	ShowBigMapDescFlag = false;
end

function ShowAllPath()
	local img = UIManager:GetUIItem("BigMapPath");
	local shadow = UIManager:GetUIItem("BigMapShadow1");
	local checkBox = UIManager:FindUICheckBox("BigMapCheck8");
	
	if checkBox ~= nil and img ~= nil and shadow ~= nil then
		shadow:SetEnable(checkBox:GetState());
		UIManager:AddDirtyUIItem(shadow);
		img:SetEnable(checkBox:GetState());
		UIManager:AddDirtyUIItem(img);
	end
end

--[[
	BigMapA : ³¥¥~
	BigMapB : «°Âí
	BigMapC : °g®c
	BigMapD : À°¬£
	BigMapE : ¤s²ø
	BigMapF : ªù¬£
]]


function ShowBigMapText(engWord, buttonCount, show)
    	for i=0, buttonCount do
			iconName = "BigMap"..engWord..i;
			local button2 = UIManager:GetUIItem(iconName);
			if button2 ~= nil then
			    button2:SetEnable(show);
			    UIManager:AddDirtyUIItem(button2);
			end
			iconName = iconName.."Text";
			button2 = UIManager:GetUIItem(iconName);
			if button2 ~= nil then
			    button2:SetEnable(show);
			    UIManager:AddDirtyUIItem(button2);
			end
	    end
end


function HideAllBigMapText()

    local bigMap = LuaUI.GetBigMap();
	if bigMap ~= nil then
    	ShowBigMapText("F", bigMap:GetButtonCount(5) , false);
    	ShowBigMapText("D", bigMap:GetButtonCount(3) , false);
    	ShowBigMapText("C", bigMap:GetButtonCount(2) , false);
    	ShowBigMapText("A", bigMap:GetButtonCount(0) , false);
    	ShowBigMapText("B", bigMap:GetButtonCount(1) , false);
    	ShowBigMapText("E", bigMap:GetButtonCount(4) , false);
    end

end

function OffCheckBoxExcept(index)
    local iconName = "BigMapCheck";
	for i = 1,7 do
		if index ~= i then
            local checkBox = UIManager:FindUICheckBox(iconName..i);
            if checkBox ~= nil then
                checkBox:SetState(false);
            end
		end
	end
end

function ShowMapButton1()
    HideAllBigMapText();
	local checkBox = UIManager:FindUICheckBox("BigMapCheck1");
	local bigMap = LuaUI.GetBigMap();
	if bigMap ~= nil and checkBox ~= nil then
     	ShowBigMapText("F", bigMap:GetButtonCount(5) , checkBox:GetState());
     	OffCheckBoxExcept(1);
    end
end

function ShowMapButton2()
 	HideAllBigMapText();
	local checkBox = UIManager:FindUICheckBox("BigMapCheck2");
	local bigMap = LuaUI.GetBigMap();
	if bigMap ~= nil and checkBox ~= nil then
      ShowBigMapText("D", bigMap:GetButtonCount(3) , checkBox:GetState());
      OffCheckBoxExcept(2);
    end
end

function ShowMapButton3()
 HideAllBigMapText();
	local checkBox = UIManager:FindUICheckBox("BigMapCheck3");
	local bigMap = LuaUI.GetBigMap();
	if bigMap ~= nil and checkBox ~= nil then
    	ShowBigMapText("C", bigMap:GetButtonCount(2) , checkBox:GetState());
    	OffCheckBoxExcept(3);
    end
end

function ShowMapButton4()
 HideAllBigMapText();
	local checkBox = UIManager:FindUICheckBox("BigMapCheck4");
	local bigMap = LuaUI.GetBigMap();
	if bigMap ~= nil and checkBox ~= nil then

    	ShowBigMapText("A", bigMap:GetButtonCount(0) , checkBox:GetState());
    	OffCheckBoxExcept(4);
    end
end

function ShowMapButton5()
 HideAllBigMapText();
	local checkBox = UIManager:FindUICheckBox("BigMapCheck5");
	local bigMap = LuaUI.GetBigMap();
	if bigMap ~= nil and checkBox ~= nil then

    	ShowBigMapText("B", bigMap:GetButtonCount(1) , checkBox:GetState());
    	OffCheckBoxExcept(5);
    end
end

function ShowMapButton6()
 	HideAllBigMapText();
	local checkBox = UIManager:FindUICheckBox("BigMapCheck6");
	local bigMap = LuaUI.GetBigMap();
	if bigMap ~= nil and checkBox ~= nil then

    	ShowBigMapText("E", bigMap:GetButtonCount(4) , checkBox:GetState());
    	OffCheckBoxExcept(6);
    end
end

function ShowAllMapText()
     local bigMap = LuaUI.GetBigMap();
     local checkBox = UIManager:FindUICheckBox("BigMapCheck7");
	if bigMap ~= nil and checkBox then
    	ShowBigMapText("F", bigMap:GetButtonCount(5) , checkBox:GetState());
    	ShowBigMapText("D", bigMap:GetButtonCount(3) , checkBox:GetState());
    	ShowBigMapText("C", bigMap:GetButtonCount(2) , checkBox:GetState());
    	ShowBigMapText("A", bigMap:GetButtonCount(0) , checkBox:GetState());
    	ShowBigMapText("B", bigMap:GetButtonCount(1) , checkBox:GetState());
    	ShowBigMapText("E", bigMap:GetButtonCount(4) , checkBox:GetState());
    	OffCheckBoxExcept(7);
    end
end

function ShowBigMapPath()
	if _G.ShowBigMapDescFlag ~= true then
	    local bigMap = LuaUI.GetBigMap();
		if bigMap ~= nil then
		    bigMap:ShowPath();
		end

		local checkBox = UIManager:FindUICheckBox("BigMapCheck8");
	 	local img = UIManager:GetUIItem("BigMapPath");
		local shadow = UIManager:GetUIItem("BigMapShadow1");
		if img:IsEnable() ~= true then
			if shadow ~= nil then
			    shadow:SetEnable(true);
			    UIManager:AddDirtyUIItem(shadow);
			end
		    img:SetEnable(true);
		    UIManager:AddDirtyUIItem(img);
		    checkBox:SetState(true);
			UIManager:AddDirtyUIItem(UIManager:GetUIItem("BigMapCheck7"));
		end
	else
	    local bigMap = LuaUI.GetBigMap();
		if bigMap ~= nil then
			bigMap:ShowDesc();
			local panel = UIManager:GetUIItem("BigMapDescPanel");
			if panel ~= nil then
			    panel:SetEnable(true);
				UIManager:AddDirtyUIItem(panel);
			end
		end

	end
end

function CloseBigMapDesc()
	--UIManager:DeletePanel("BigMapDescPanel");
	--_G.ShowBigMapDescFlag = false;
	local descPanel = UIManager:GetUIItem("BigMapDescPanel");
	if descPanel ~= nil then 
		descPanel:SetEnable(false);
		UIManager:AddDirtyUIItem(descPanel);
	end
end

function DeleteBigMapDesc()
	UIManager:StopDrag();
	UIManager:DeletePanel("BigMapDescPanel");
end

function ClearFlag()
	if _G.ShowBigMapDescFlag == false then
		local bigMap = LuaUI.GetBigMap();
		if bigMap ~= nil then
			bigMap:DisableAllFlag();
			bigMap:ResetDes();
		end
	end
end