function  BattleAction()  
	LuaUI.BattleAction()  
end

function  BattleSkill()  
	LuaUI.BattleSkill()
end


function  BattleItem()  
	LuaUI.BattleItem()
end


function  BattleDefence()  
	LuaUI.BattleDefence()
end


function  BattleFriend()  
	--LuaUI.BattleFriend()
	--if _G.InBattle == true then
		OpenPartner();

	--end
	
end


function  BattleProtect()  
	LuaUI.BattleProtect()  
end


function  BattleCatch()  
	HideBattlePanel();
	LuaUI.BattleCatch();
	
end


function  BattleHelp()  
	LuaUI.BattleHelp()
end


function  BattleAuto()  
	LuaUI.BattleAuto()
end


function  BattleEscape()  
	LuaUI.BattleEscape()
end

function BattleJoin()
	LuaUI.BattleJoin()
end

function BattleWatchLeave()
	LuaUI.BattleWatchLeave()
end