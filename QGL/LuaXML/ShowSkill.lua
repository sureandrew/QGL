isCloseShowPanel=false;
isShowPanel=true;

function OpenShowPanel()
	if isShowPanel then
		local panelist = UIManager:GetUIItem("ShowActorNameist");
		local panelsnd = UIManager:GetUIItem("ShowActorNamesnd");
		local panelimg = UIManager:GetUIItem("ShowSkillPanelBKImage");
		if panelist == nil or panelsnd == nil or panelimg == nil then
			LuaUI.AddXMLFile2UIManger("Data\\ShowSkill.xml");
		else
			panelist:SetEnable(true);
			panelsnd:SetEnable(true);
			panelimg:SetEnable(true);
		end
	end
end

function onClosePanel()
	local panelist = UIManager:GetUIItem("ShowActorNameist");
	local panelsnd = UIManager:GetUIItem("ShowActorNamesnd");
	local panelimg = UIManager:GetUIItem("ShowSkillPanelBKImage");
	if panelist ~= nil and panelsnd ~= nil and panelimg ~= nil then
		panelist:SetEnable(false);
		panelsnd:SetEnable(false);
		panelimg:SetEnable(false);
	end
end

function OpenShowButton()
	local panel = UIManager:GetUIItem("ShowSkillButton");
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\ShowSkill.xml");
	else
		panel:SetEnable(true);
	end
end

function onCloseShowButton()
	local panel = UIManager:GetUIItem("ShowSkillButton");
	if panel ~= nil then 
		panel:SetEnable(false);
	end
end

function onClickShow()
	isCloseShowPanel = not isCloseShowPanel;
	isShowPanel = not isShowPanel;
	if isCloseShowPanel == true then
		onClosePanel();
	elseif isCloseShowPanel == false then
		OpenShowPanel();
	end
end