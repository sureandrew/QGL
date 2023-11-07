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