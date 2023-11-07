isCloseAssist = false;

function CloseNewGuide()
	local panel = UIManager:GetUIItem("NewGuidePanel");	
	if panel ~= nil then
		panel:SetEnable(false);
	end
end

function CloseNewArrow()
	local panel = UIManager:GetUIItem("NewGuideArrow");
	if panel ~= nil then
	    panel:SetEnable(false);
	end
end

function IsCloseAssist()
	isCloseAssist = not isCloseAssist;
	local Menu = LuaUI.GetMenuHelpProcess();
	if Menu ~= nil then
		if isCloseAssist == true then
			Menu:CloseNewGuide(1);
		elseif isCloseAssist == false then
			Menu:CloseNewGuide(0);
		end
	end
end

function OpenNewGuide()
	local panel = UIManager:GetUIItem("NewGuidePanel");
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\NewGuide.xml");
	else
		panel:SetEnable(true);
	end
end

function OpenGuideArrow()
	local panel = UIManager:GetUIItem("NewGuideArrow");
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\NewGuide.xml");
	else
		panel:SetEnable(true);
	end
end