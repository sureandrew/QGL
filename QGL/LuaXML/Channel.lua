function PopChannelPanel(pX, pY)
	local PopPanel = UIManager:FindUIPanel("ChannelPopPanel")
	
	if PopPanel ~= nil then
		UIManager:DeletePanel("ChannelPopPanel")
		UIManager:RenderAll()
	else
		LuaUI.AddXMLFile2UIManger("Data\\ChannelPopPanel.xml");
		LuaUI.InitChannelEnable();
		UIManager:GetUIItem("ChannelPopPanel"):SetPos(8, 394);
	end
end

function  ChangeMapChannel()
	LuaUI.ChangeChannel(0)
end

function  ChangePartyChannel()  
	LuaUI.ChangeChannel(1)
end

function  ChangeGangChannel()  
	LuaUI.ChangeChannel(2)
end

function  ChangeGuildChannel()  
	LuaUI.ChangeChannel(3)
end

function  ChangePrivateChannel()  
	LuaUI.ChangeChannel(4)
end

function  ChangeGlobalChannel()  
	LuaUI.ChangeChannel(5)
end


function  ChangeCoupleChannel()  
	LuaUI.ChangeChannel(6)
end

function  ChangeSpeakerChannel()  
	LuaUI.ChangeChannel(12)
end

function  ChangeRumorChannel()
	LuaUI.ChangeChannel(8)
end

function  ChangeFriendChannel()
	LuaUI.ChangeChannel(13)
end

function ReceiveMapChannelFuc()
	local Box = UIManager:FindUICheckBox("ReceiveMapChannel");
		
	if Box:GetState() then
		LuaUI.RejectChannel(0)
	else
		LuaUI.ReceiveChannel(0)
	end
end

function ReceivePartyChannelFuc()
	local Box = UIManager:FindUICheckBox("ReceivePartyChannel");
		
	if Box:GetState() then
		LuaUI.RejectChannel(1)
	else
		LuaUI.ReceiveChannel(1)
	end
end


function ReceiveGangChannelFuc()
	local Box = UIManager:FindUICheckBox("ReceiveGangChannel");
		
	if Box:GetState() then
		LuaUI.RejectChannel(2)
	else
		LuaUI.ReceiveChannel(2)
	end
end

function ReceiveGuildChannelFuc()
	local Box = UIManager:FindUICheckBox("ReceiveGuildChannel");
		
	if Box:GetState() then
		LuaUI.RejectChannel(3)
	else
		LuaUI.ReceiveChannel(3)
	end
end


function ReceivePrivateChannelFuc()
	local Box = UIManager:FindUICheckBox("ReceivePrivateChannel");
		
	if Box:GetState() then
		LuaUI.RejectChannel(4)
	else
		LuaUI.ReceiveChannel(4)
	end
end

function ReceiveGlobalChannelFuc()
	local Box = UIManager:FindUICheckBox("ReceiveGlobalChannel");
		
	if Box:GetState() then
		LuaUI.RejectChannel(5)
	else
		LuaUI.ReceiveChannel(5)
	end
end


function ReceiveCoupleChannelFuc()
	local Box = UIManager:FindUICheckBox("ReceiveCoupleChannel");
		
	if Box:GetState() then
		LuaUI.RejectChannel(6)
	else
		LuaUI.ReceiveChannel(6)
	end
end

function ReceiveRumorChannelFuc()
	local Box = UIManager:FindUICheckBox("ReceiveRumorChannel");
		
	if Box:GetState() then
		LuaUI.RejectChannel(8)
	else
		LuaUI.ReceiveChannel(8)
	end
end


function ReceiveSpeakerChannelFuc()
	local Box = UIManager:FindUICheckBox("ReceiveSpeakerChannel");
	LuaUI.RejectChannel(12)
end

function ReceiveFriendChannelFuc()
	local Box = UIManager:FindUICheckBox("ReceiveFriendChannel");
		
	if Box:GetState() then
		LuaUI.RejectChannel(13)
	else
		LuaUI.ReceiveChannel(13)
	end
end