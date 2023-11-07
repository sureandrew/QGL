firstopenclock = false

function CloseClockPanel(pX, pY)
	UIManager:SetEvent(107);
end

function CallClockPanel(pX, pY)
	--local Panel = UIManager:FindUIPanel("ClockPanel")
	--if Panel == nil then
	--	LuaUI.AddXMLFile2UIManger("Data\\Clock.xml")
	--end
	if UIManager:GetUIItem("MiniMap"):IsEnable() == false then
		UIManager:GetUIItem("MiniMap"):SetEnable(true);
		UIManager:GetUIItem("ClockPanel"):SetEnable(false);
	else
		
		UIManager:GetUIItem("MiniMap"):SetEnable(false);
		UIManager:GetUIItem("ClockPanel"):SetEnable(true);
		LuaUI.UpdateClockPanel(true);
	end

	UIManager:RenderAll();
	--CloseClockPanel(0, 0)
end

