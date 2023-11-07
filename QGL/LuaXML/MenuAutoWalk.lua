

function OpenAutoWalk()
	local AutoWalk = LuaUI.GetAutoWalkProcess();
	if AutoWalk ~= nil then
		AutoWalk:OnOpenAutoWalk();
	end
end

function OnStopAutoWalk()
	local AutoWalk = LuaUI.GetAutoWalkProcess();
	if AutoWalk ~= nil then
		AutoWalk:StopAutoWalk();
	end
end

function OnCloseAutoWalk()
	local panel = UIManager:FindUIPanel("AutoWalkPanel");
	if panel then
	UIManager:DeletePanel("AutoWalkPanel");
	end
end

function OnStartAutoWalk()
	local AutoWalk = LuaUI.GetAutoWalkProcess();
	if AutoWalk ~= nil then
		AutoWalk:StartAutoWalk(60);
	end
end

function OnChooseHitBoss()
	local AutoWalk = LuaUI.GetAutoWalkProcess();
	if AutoWalk ~= nil then
		AutoWalk:ChangeHitBoss();
	end
end
