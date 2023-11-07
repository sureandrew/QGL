function OpenSetSiblingTitlePrefixPanel()
	local panel = UIManager:GetUIItem("SetSiblingTitlePrefixPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\SetSiblingTitlePrefix.xml");
		panel = UIManager:GetUIItem("SetSiblingTitlePrefixPanel");
		panel:SetEnable(true);
	else
		CloseSetSiblingTitlePrefixPanel();
	end

end

function CloseSetSiblingTitlePrefixPanel()
	UIManager:DeletePanel("SetSiblingTitlePrefixPanel");
	UIManager: SetKeyInItem("EditLine");
end

function SetSiblingTitlePrefixOK()
	local relationSystemProcess = LuaUI.GetRelationSystemProcess();
	if relationSystemProcess ~= nil then
		relationSystemProcess:SetSiblingTitlePrefix();
	end
	
	CloseSetSiblingTitlePrefixPanel();
end

function SetSiblingTitlePrefixCancel()
	CloseSetSiblingTitlePrefixPanel();
end

function OpenSetSiblingTitleSuffixPanel()
	local panel = UIManager:GetUIItem("SetSiblingTitleSuffixPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\SetSiblingTitleSuffix.xml");
		panel = UIManager:GetUIItem("SetSiblingTitleSuffixPanel");
		panel:SetEnable(true);
	else
		CloseSetSiblingTitleSuffixPanel();
	end

end

function CloseSetSiblingTitleSuffixPanel()
	UIManager:DeletePanel("SetSiblingTitleSuffixPanel");
	UIManager: SetKeyInItem("EditLine");
end

function SetSiblingTitleSuffixOK()
	local relationSystemProcess = LuaUI.GetRelationSystemProcess();
	if relationSystemProcess ~= nil then
		relationSystemProcess:SetSiblingTitleSuffix();
	end
	CloseSetSiblingTitleSuffixPanel();
end

function SetSiblingTitleSuffixCancel()
	CloseSetSiblingTitleSuffixPanel();
end