function PrivateMsgPopUpFunc()
	local PopUp = UIManager:FindUIPopUpMenu("PrivateMsgPopUp")
	
	if PopUp ~= nil then
		LuaUI.SendPrivateMessageTo(PopUp:GetBarString());
		UIManager:DeletePanel("PrivateMsgPopUpPanel");
	end
end

function OpenPrivateMsgPopPanel()
	local PopPanel = UIManager:FindUIPanel("PrivateMsgPopUpPanel")
	
	if PopPanel ~= nil then
		UIManager:DeletePanel("PrivateMsgPopUpPanel")
	else
		LuaUI.AddXMLFile2UIManger("Data\\PrivateMsgPopPanel.xml");
		LuaUI.InitRecentChatListBox();
	end
	
	UIManager:RenderAll(); 
end