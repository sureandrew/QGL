local PanelIndex = 0;

function OpenPlayerMessage()
	local PopPanel = UIManager:FindUIPanelEx("PlayerMessage1");
	local DirtyUI = UIManager:GetUIItem("PlayerMessage1");
	
	if PopPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\PlayerMessage.xml");
		PopPanel = UIManager:FindUIPanelEx("PlayerMessage1");
		DirtyUI = UIManager:GetUIItem("PlayerMessage1");
		--UIManager:FindUIPanelEx("PlayerMessage1"):ChangeCount(1);
		--UIManager:FindUIPanelEx("PlayerMessage2"):ChangeCount(1);
		--UIManager:FindUIPanelEx("PlayerMessage3"):ChangeCount(1);
	end
	
	local ChatMessPanel = UIManager:FindUIChatPanel("PlayerMessageContext1");
	
	if PanelIndex == 1 then
		PopPanel = UIManager:FindUIPanelEx("PlayerMessage2");
		ChatMessPanel = UIManager:FindUIChatPanel("PlayerMessageContext2");
		DirtyUI = UIManager:GetUIItem("PlayerMessage2");
	end
	
	if PanelIndex == 2 then
		PopPanel = UIManager:FindUIPanelEx("PlayerMessage3");
		ChatMessPanel = UIManager:FindUIChatPanel("PlayerMessageContext3");
		DirtyUI = UIManager:GetUIItem("PlayerMessage3");
	end
	
	PanelIndex = PanelIndex + 1;
	if PanelIndex == 3 then
		PanelIndex = 0;
	end
	
	ChatMessPanel:ClearAllRow();
	ChatMessPanel:AddStringW(LuaUI.GetTempMsg());
	ChatMessPanel:EndRow();
	ChatMessPanel:Redraw();
	
	if ChatMessPanel:GetTotalHeight() == 0 then
		do return end
	end
	PopPanel:SetEnable(true);
	UIManager:SetPanelEx2Top(PopPanel);
	
	local LineCount = LuaUI.DivUInt32(ChatMessPanel:GetTotalHeight() , 3);
			
	PopPanel:ChangeCount(LineCount);
	
	UIManager:AddDirtyUIItem(DirtyUI);
end

function PlayerMessageTimeout1()
	local PopPanel = UIManager:FindUIPanelEx("PlayerMessage1")
	if PopPanel ~= nil then
		PopPanel:SetEnable(false);
		PanelIndex = 0;
		local DirtyUI = UIManager:GetUIItem("PlayerMessage1");
		UIManager:AddDirtyUIItem(DirtyUI); 
	end
end

function PlayerMessageTimeout2()
	local PopPanel = UIManager:FindUIPanelEx("PlayerMessage2")
	if PopPanel ~= nil then
		if PanelIndex > 1 then
			PanelIndex = 1;
		end
		PopPanel:SetEnable(false); 
		local DirtyUI = UIManager:GetUIItem("PlayerMessage2");
		UIManager:AddDirtyUIItem(DirtyUI);
	end
end

function PlayerMessageTimeout3()
	local PopPanel = UIManager:FindUIPanelEx("PlayerMessage3")
	if PopPanel ~= nil then
		PopPanel:SetEnable(false); 
		local DirtyUI = UIManager:GetUIItem("PlayerMessage3");
		UIManager:AddDirtyUIItem(DirtyUI);
	end
end