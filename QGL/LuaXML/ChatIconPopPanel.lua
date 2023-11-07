function PopChatIconPanel(pX, pY)
	local PopPanel = UIManager:FindUIPanel("ChatIconPopPanel")
	
	if PopPanel ~= nil then
		UIManager:DeletePanel("ChatIconPopPanel")
	else
		LuaUI.AddXMLFile2UIManger("Data\\ChatIconPopPanel.xml");
		PopPanel = UIManager:FindUIPanel("ChatIconPopPanel");
		if PopPanel ~= nil then
			PopPanel:SetPos(100, 70);
			PopPanel:SetDetectRect();
		end
	end
	
	MyChatIconPanel = UIManager:FindUIChatPanel("ChatIconPanel");	
	LuaUI.AddChatIconToPanel();
	UIManager:RenderAll(); 
end


function CloseChatIconPanel(pX, pY)
	UIManager:SetEvent(106)
end

function ChatIconPanelUp(pX, pY)
	MyChatIconPanel:UpRow()
	if MyChatIconPanel:IsStartRow() then
		local ScrollBar = UIManager:FindUIScrollBarV("ChatIconScrollBar")
		ScrollBar:HomeScroll()
	end
end

function ChatIconPanelDown(pX, pY)
	MyChatIconPanel:DownRow()
	if MyChatIconPanel:IsEndRow() then
		local ScrollBar = UIManager:FindUIScrollBarV("ChatIconScrollBar")
		ScrollBar:EndScroll()
	end
end

function ChatIconPanelWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("ChatIconScrollBar"):OnMouseWheel(UIManager:GetUIItem("ChatIconScrollBar"):GetX()
	,UIManager:GetUIItem("ChatIconScrollBar"):GetY(),pZ);
end

function ChatIconPanelScroll(pX, pY)
	local ScrollBar = UIManager:FindUIScrollBarV("ChatIconScrollBar")
	MyChatIconPanel:SetRow(ScrollBar:GetSelNum(), ScrollBar:GetNumCount())
	UIManager:RenderAll()
end

function AddChatIconToEditLine()
	LuaUI.AddChatIconToEditLine();
end