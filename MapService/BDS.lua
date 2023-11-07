
BDS = { target = nil }

function GDSBattle(actor)
	if actor and actor:IsPlayer() then
		player = actor:ToBatChar()
		print(string.format("player = char_id %d", player:GetChar_id()))
	end
	return GDS
end

function BDS.SetTarget(name)
	if player == nil then
		print("BDS Warning: 'player' is nil!")
	end
	if name == "player" then
		BDS.target = player
	else
		BDS.target = nil
	end

	return (BDS.target ~= nil)
end

function BDS:LuaAddExp(name, exp)
	battle:GiveReward(1, 1, exp)
end

function BDS:LuaAddGameMoney(name, money)
	battle:GiveReward(1, 2, money)
end

function BDS:LuaAddItem(name, item_id, count)
	battle:GiveReward(1, 3, item_id, count)
end

function BDS:LuaAddMob(mob_id, pos)
	battle:AddMob(mob_id, pos)
end

function BDS:LuaBatChoose(question, c1, c2)
	local n = 0

	if c1 then n = 1 end
	if c2 then n = 2 end
	battle:GetScript():Choose(question, n, c1, c2)
end

function BDS:LuaBatEndCmdMsg()
end

function BDS:LuaBatSay(name, text, image, left, npc_id)
	if image == nil then image = "npc" end
	if left == nil then left = true end
	if npc_id == nil then npc_id = 0 end

	local teamIndex = 0
	if name == "player" then
		teamIndex = 255				-- should be 0, but want to say to all
	elseif name == "team" then
		teamIndex = 255
	end
	
	local imageType
	if image == "npc" then
		imageType = 0
	elseif image == "player" then
		imageType = 1
	else
		imageType = 99
	end

	--print(string.format("BatSay: %s, imageType %d, npc_id %d", text, 
	--	imageType, npc_id))
	battle:BatSay(text, imageType, left, npc_id, teamIndex)
end

function BDS:LuaBatSetChooseImage(name, npc_id, left)
	if name ~= "npc" then npc_id = 0 end
	battle:GetScript():SetChooseImage(npc_id, left)
end

function BDS:LuaDisEnableBatCmd(cmd)
	battle:DisableCmd(cmd)
end

function BDS:LuaGetFreeSlot(name)
	if not BDS.SetTarget(name) then return 0 end
	return BDS.target:GetItems():GetFreeSlot(0)
end

function BDS:LuaNoEndBat()
	battle:CheckTeamStatus()
	return battle:SetForceEnd(false)
end

function BDS:LuaNoMobExist()
	battle:CheckTeamStatus()
	return (battle:GetTeamExist(0) == 0)
end

function BDS:LuaNoPlayerExit()
	return (battle:GetTeamExist(1) == 0)
end

function BDS:LuaRemoveGameMoney(name, money)
	battle:GivePenalty(1, 2, money)
end

function BDS:LuaSetBatEvent(turn, eventType)
	battle:SetBatEvent(turn, eventType)
end

function BDS:LuaSetBatNoPunish()
	battle:SetPunish(false)
end

function BDS:LuaGetLeader()
	return battle:GetTeamLeader(1)
end

function BDS:LuaGetActor(i)
	return battle:GetActor(i)
end
