function OpenWantedPanel()
	local panel = UIManager:GetUIItem("WantedPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\Wanted.xml");
		panel = UIManager:GetUIItem("WantedPanel");
		panel:SetEnable(true);
	end

end

function CloseWantedPanel()
	local wantedProcess = LuaUI.GetWantedProcess();
	if wantedProcess ~= nil then
		wantedProcess:ClearAllWanted();
	end
	UIManager:DeletePanel("WantedPanel");
end

function WantedScroll()
	local wantedProcess = LuaUI.GetWantedProcess();
	if wantedProcess ~= nil then
		wantedProcess:WantedScroll(2);
	end
end

function WantedScrollUp()
	local wantedProcess = LuaUI.GetWantedProcess();
	if wantedProcess ~= nil then
		wantedProcess:WantedScroll(0);
	end
end

function WantedScrollDown()
    local wantedProcess = LuaUI.GetWantedProcess();
	if wantedProcess ~= nil then
		wantedProcess:WantedScroll(1);
	end
end

function SelectWanted(index)
	local wantedProcess = LuaUI.GetWantedProcess();
	if wantedProcess ~= nil then
		wantedProcess:SelectWanted(index);
	end
end

function WantedButtonClass3()
	local wantedProcess = LuaUI.GetWantedProcess();
	if wantedProcess ~= nil then
		wantedProcess:SelectWantedClass(3);
	end
end

function WantedButtonClass2()
	local wantedProcess = LuaUI.GetWantedProcess();
	if wantedProcess ~= nil then
		wantedProcess:SelectWantedClass(2);
	end
end

function WantedButtonClass1()
	local wantedProcess = LuaUI.GetWantedProcess();
	if wantedProcess ~= nil then
		wantedProcess:SelectWantedClass(1);
	end
end
function SelectWanted00()
	SelectWanted(0);
end

function SelectWanted01()
    SelectWanted(1);
end

function SelectWanted02()
	SelectWanted(2);
end

function SelectWanted03()
	SelectWanted(3);
end

function SelectWanted04()
	SelectWanted(4);
end

function SelectWanted05()
	SelectWanted(5);
end

function SelectWanted06()
	SelectWanted(6);
end

function SelectWanted07()
	SelectWanted(7);
end

function SelectWanted08()
	SelectWanted(8);
end

function SelectWanted09()
	SelectWanted(9);
end

function SelectWanted10()
	SelectWanted(10);
end


function WantedButtonReward()
	local wantedProcess = LuaUI.GetWantedProcess();
	if wantedProcess ~= nil then
		wantedProcess:RequestWantedReward();
	end
end

function WantedButtonDetail()
	local wantedProcess = LuaUI.GetWantedProcess();
	if wantedProcess ~= nil then
		wantedProcess:WantedDetail();
	end
end
--³öÉÍ½ð¼©ÄÃ
function OpenWantedRewardPanel()
	local panel = UIManager:GetUIItem("WantedRewardPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\WantedReward.xml");
		UIManager: SetKeyInItem("WantedRewardEditBox");
		panel:SetEnable(true);
	end
end
function CloseWantedRewardPanel()
	UIManager:DeletePanel("WantedRewardPanel");
	UIManager: SetKeyInItem("EditLine");
end

function WantedRewardOK()
	local editline = UIManager:FindUIEditBox("WantedRewardEditBox");
	if editline ~= nil then
		local menu = LuaUI.GetWantedProcess();
		menu:WantedReward(editline:GetInt());
	end
	CloseWantedRewardPanel();
end

function WantedRewardCancel()
	CloseWantedRewardPanel();
end
--½ÙÓü
function OpenWantedJailDeliveryPanel()
	local panel = UIManager:GetUIItem("WantedJailDeliveryPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\WantedJailDelivery.xml");
		UIManager: SetKeyInItem("WantedJailDeliveryEditBox");
		panel:SetEnable(true);
	end
end
function CloseWantedJailDeliveryPanel()
	UIManager:DeletePanel("WantedJailDeliveryPanel");
	UIManager: SetKeyInItem("EditLine");
end

function WantedJailDeliveryOK()
	local editline = UIManager:FindUIEditBox("WantedJailDeliveryEditBox");
	if editline ~= nil then
		local menu = LuaUI.GetWantedProcess();
		menu:WantedJailDelivery(editline:GetInt());
	end
	CloseWantedJailDeliveryPanel();
end

function WantedJailDeliveryCancel()
	CloseWantedJailDeliveryPanel();
end

--±£ÊÍ
function OpenWantedBailPanel()
	local panel = UIManager:GetUIItem("WantedBailPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\WantedBail.xml");
		UIManager: SetKeyInItem("WantedBailEditBox");
		panel:SetEnable(true);
	end
end
function CloseWantedBailPanel()
	UIManager:DeletePanel("WantedBailPanel");
	UIManager: SetKeyInItem("EditLine");
end

function WantedBailOK()
	local editline = UIManager:FindUIEditBox("WantedBailEditBox");
	if editline ~= nil then
		local menu = LuaUI.GetWantedProcess();
		menu:WantedBail(editline:GetInt());
	end
	CloseWantedBailPanel();
end

function WantedBailCancel()
	CloseWantedBailPanel();
end

--×·×Ù
function OpenWantedTracePanel()
	local panel = UIManager:GetUIItem("WantedTracePanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\WantedTrace.xml");
		UIManager: SetKeyInItem("WantedTraceEditBox");
		panel:SetEnable(true);
	end
end
function CloseWantedTracePanel()
	UIManager:DeletePanel("WantedTracePanel");
	UIManager: SetKeyInItem("EditLine");
end

function WantedTraceOK()
	local editline = UIManager:FindUIEditBox("WantedTraceEditBox");
	if editline ~= nil then
		local menu = LuaUI.GetWantedProcess();
		menu:WantedTrace(editline:GetInt());
	end
	CloseWantedTracePanel();
end

function WantedTraceCancel()
	CloseWantedTracePanel();
end
