function OpenChannelHistory(pX, pY)
	if _G.CloseAllPanel ~= true then
		local PopPanel = UIManager:FindUIPanel("ChannelHistoryPopPanel")
		
		if PopPanel ~= nil then
			UIManager:DeletePanel("ChannelHistoryPopPanel")
		else
			LuaUI.AddXMLFile2UIManger("Data\\ChannelHistoryPopPanel.xml");
		end
	
		MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
		if MyChatHistory ~= nil then
		local MyPool = LuaUI.GetChatPool()
			if MyPool ~= nil then
				local MsgHistory = MyPool:GetStartMsg();
				while MsgHistory do
					MyChatHistory:AddStringW(MsgHistory);
					MyChatHistory:EndRow();
					MsgHistory  = MyPool:GetNextMsg();
				end
			end
		end
		
		ResetScrollNumCount();
		ResetAllHistoryCheckBox();
		UIManager:FindUICheckBox("AllChannelHis"):SetState(true);
	
		UIManager:RenderAll(); 
	end
end

function ResetScrollNumCount()
	MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
	if MyChatHistory ~= nil then
		local scrollV = UIManager:FindUIScrollBarV("ChannelHisScroll");
		if scrollV ~= nil then
			if MyChatHistory:GetAllRowHeight() > 180 then
				scrollV:SetNumCount(MyChatHistory:RowListCount());
			else
				scrollV:SetNumCount(0);
			end
			scrollV:EndScroll();
		end
	end
end
function MapChannelHisLua(pX, pY)
	if UIManager:FindUICheckBox("MapChannelHis"):GetState() then
	MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
	if MyChatHistory ~= nil then
	MyChatHistory:ClearAllRow()
	local MyPool = LuaUI.GetChatPool()
		if MyPool ~= nil then
			local MsgHistory = MyPool:GetStartMsg();
			while MsgHistory do
				if LuaUI.IsChannelMsg(MsgHistory, 0) then
					MyChatHistory:AddStringW(MsgHistory);
					MyChatHistory:EndRow();
				end
					MsgHistory  = MyPool:GetNextMsg();
				end
			end
		end
	end
	ResetScrollNumCount();
	ResetAllHistoryCheckBox();
	UIManager:FindUICheckBox("MapChannelHis"):SetState(true);
	UIManager:RenderAll();
end

function PartyChannelHisLua(pX, pY)
if UIManager:FindUICheckBox("PartyChannelHis"):GetState() then
	MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
	if MyChatHistory ~= nil then
	MyChatHistory:ClearAllRow()
	local MyPool = LuaUI.GetChatPool()
		if MyPool ~= nil then
			local MsgHistory = MyPool:GetStartMsg();
			while MsgHistory do
				if LuaUI.IsChannelMsg(MsgHistory, 1) then
					MyChatHistory:AddStringW(MsgHistory);
					MyChatHistory:EndRow();
				end
				MsgHistory  = MyPool:GetNextMsg();
			end
		end
	end
	end
	ResetScrollNumCount();
	ResetAllHistoryCheckBox();
	UIManager:FindUICheckBox("PartyChannelHis"):SetState(true);
	UIManager:RenderAll();
end
function GangChannelHisLua(pX, pY)
if UIManager:FindUICheckBox("GangChannelHis"):GetState() then
	MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
	if MyChatHistory ~= nil then
	MyChatHistory:ClearAllRow()
	local MyPool = LuaUI.GetChatPool()
		if MyPool ~= nil then
			local MsgHistory = MyPool:GetStartMsg();
			while MsgHistory do
				if LuaUI.IsChannelMsg(MsgHistory, 2) then
					MyChatHistory:AddStringW(MsgHistory);
					MyChatHistory:EndRow();
				end
				MsgHistory  = MyPool:GetNextMsg();
			end
		end
	end
	end
	ResetScrollNumCount();
	ResetAllHistoryCheckBox();
	UIManager:FindUICheckBox("GangChannelHis"):SetState(true);
	UIManager:RenderAll();
end
function GuildChannelHisLua(pX, pY)
if UIManager:FindUICheckBox("GuildChannelHis"):GetState() then
	MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
	if MyChatHistory ~= nil then
	MyChatHistory:ClearAllRow()
	local MyPool = LuaUI.GetChatPool()
		if MyPool ~= nil then
			local MsgHistory = MyPool:GetStartMsg();
			while MsgHistory do
				if LuaUI.IsChannelMsg(MsgHistory, 3) then
					MyChatHistory:AddStringW(MsgHistory);
					MyChatHistory:EndRow();
				end
				MsgHistory  = MyPool:GetNextMsg();
			end
		end
	end
	end
	ResetScrollNumCount();
	ResetAllHistoryCheckBox();
	UIManager:FindUICheckBox("GuildChannelHis"):SetState(true);
	UIManager:RenderAll();
end
function PrivateChannelHisLua(pX, pY)
if UIManager:FindUICheckBox("PrivateChannelHis"):GetState() then
	MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
	if MyChatHistory ~= nil then
	MyChatHistory:ClearAllRow()
	local MyPool = LuaUI.GetChatPool()
		if MyPool ~= nil then
			local MsgHistory = MyPool:GetStartMsg();
			while MsgHistory do
				if LuaUI.IsChannelMsg(MsgHistory, 4) then
					MyChatHistory:AddStringW(MsgHistory);
					MyChatHistory:EndRow();
				end
				MsgHistory  = MyPool:GetNextMsg();
			end
		end
	end
	end
	ResetScrollNumCount();
	ResetAllHistoryCheckBox();
	UIManager:FindUICheckBox("PrivateChannelHis"):SetState(true);
	UIManager:RenderAll();
end
function GlobalChannelHisLua(pX, pY)
if UIManager:FindUICheckBox("GlobalChannelHis"):GetState() then
	MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
	if MyChatHistory ~= nil then
	MyChatHistory:ClearAllRow()
	local MyPool = LuaUI.GetChatPool()
		if MyPool ~= nil then
			local MsgHistory = MyPool:GetStartMsg();
			while MsgHistory do
				if LuaUI.IsChannelMsg(MsgHistory, 5) then
					MyChatHistory:AddStringW(MsgHistory);
					MyChatHistory:EndRow();
				end
				MsgHistory  = MyPool:GetNextMsg();
			end
		end
	end
	end
	ResetScrollNumCount();
	ResetAllHistoryCheckBox();
	UIManager:FindUICheckBox("GlobalChannelHis"):SetState(true);
	UIManager:RenderAll();
end
function CoupleChannelHisLua(pX, pY)
if UIManager:FindUICheckBox("CoupleChannelHis"):GetState() then
	MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
	if MyChatHistory ~= nil then
	MyChatHistory:ClearAllRow()
	local MyPool = LuaUI.GetChatPool()
		if MyPool ~= nil then
			local MsgHistory = MyPool:GetStartMsg();
			while MsgHistory do
				if LuaUI.IsChannelMsg(MsgHistory, 6) then
					MyChatHistory:AddStringW(MsgHistory);
					MyChatHistory:EndRow();
				end
				MsgHistory  = MyPool:GetNextMsg();
			end
		end
	end
	end
	ResetScrollNumCount();
	ResetAllHistoryCheckBox();
	UIManager:FindUICheckBox("CoupleChannelHis"):SetState(true);
	UIManager:RenderAll();
end
function SystemChannelHisLua(pX, pY)
if UIManager:FindUICheckBox("SystemChannelHis"):GetState() then
	MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
	if MyChatHistory ~= nil then
	MyChatHistory:ClearAllRow()
	local MyPool = LuaUI.GetChatPool()
		if MyPool ~= nil then
			local MsgHistory = MyPool:GetStartMsg();
			while MsgHistory do
				if LuaUI.IsChannelMsg(MsgHistory, 7) then
					MyChatHistory:AddStringW(MsgHistory);
					MyChatHistory:EndRow();
				end
				MsgHistory  = MyPool:GetNextMsg();
			end
		end
	end
	end
	ResetScrollNumCount();
	ResetAllHistoryCheckBox();
	UIManager:FindUICheckBox("SystemChannelHis"):SetState(true);
	UIManager:RenderAll();
end

function AllChannelHisLua(pX, pY)
	MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
	if MyChatHistory ~= nil then
	MyChatHistory:ClearAllRow()
	local MyPool = LuaUI.GetChatPool()
		if MyPool ~= nil then
			local MsgHistory = MyPool:GetStartMsg();
			while MsgHistory do
				MyChatHistory:AddStringW(MsgHistory);
				MyChatHistory:EndRow();
				MsgHistory  = MyPool:GetNextMsg();
			end
		end
	end
	ResetScrollNumCount();
	ResetAllHistoryCheckBox();
	UIManager:FindUICheckBox("AllChannelHis"):SetState(true);
	
	UIManager:RenderAll();
end

function RumorChannelHisLua(pX, pY)
	if UIManager:FindUICheckBox("RumorChannelHis"):GetState() then
	MyChatHistory = UIManager:FindUIChatPanel("ChatHistory");	
	if MyChatHistory ~= nil then
	MyChatHistory:ClearAllRow()
	local MyPool = LuaUI.GetChatPool()
		if MyPool ~= nil then
			local MsgHistory = MyPool:GetStartMsg();
			while MsgHistory do
				if LuaUI.IsChannelMsg(MsgHistory, 8) then
					MyChatHistory:AddStringW(MsgHistory);
					MyChatHistory:EndRow();
				end
				MsgHistory  = MyPool:GetNextMsg();
			end
		end
	end
	end
	ResetScrollNumCount();
	ResetAllHistoryCheckBox();
	UIManager:FindUICheckBox("RumorChannelHis"):SetState(true);
	UIManager:RenderAll();
end

function CoverChannelHisLua(pX, pY)
	
end

function ExportChannelHisLua(pX, pY)
	
end

function ChannelHisUpLua(pX, pY)
	MyChatHistory:UpRow()
end

function ChannelHisDownLua(pX, pY)
	MyChatHistory:DownRow()
end

function ChannelHisScrollLua(pX, pY)
	local scrollV = UIManager:FindUIScrollBarV("ChannelHisScroll");
	if scrollV ~= nil then
		local selNum = scrollV:GetSelNum();
		MyChatHistory:SetRow(selNum, MyChatHistory:RowListCount());
		local PopPanel = UIManager:GetUIItem("ChannelHistoryPopPanel");
		UIManager:AddDirtyUIItem(PopPanel);
	end
	
end

function ChatHisMouseWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("ChannelHisScroll"):OnMouseWheel(UIManager:GetUIItem("ChannelHisScroll"):GetX()
	,UIManager:GetUIItem("ChannelHisScroll"):GetY(),pZ);
end

function CloseChannelHisLua(pX, pY)
	UIManager:SetEvent(104);
end

function ResetAllHistoryCheckBox()
	UIManager:FindUICheckBox("MapChannelHis"):SetState(false);
	UIManager:FindUICheckBox("PartyChannelHis"):SetState(false);
	UIManager:FindUICheckBox("GangChannelHis"):SetState(false);
	UIManager:FindUICheckBox("GuildChannelHis"):SetState(false);
	UIManager:FindUICheckBox("PrivateChannelHis"):SetState(false);
	UIManager:FindUICheckBox("GlobalChannelHis"):SetState(false);
	UIManager:FindUICheckBox("CoupleChannelHis"):SetState(false);
	UIManager:FindUICheckBox("SystemChannelHis"):SetState(false);
	UIManager:FindUICheckBox("AllChannelHis"):SetState(false);
	UIManager:FindUICheckBox("RumorChannelHis"):SetState(false);
end