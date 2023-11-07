-- robot1 script

-- set random seed to time
math.randomseed(os.time())
actionRate = 100
endBattle = false

function Hello()
	print("Lua: Robot1 Hello World")
	LuaI.HelloWorld()
end

function Respond(msg, a1, a2, a3, a4, a5, a6, a7, a8, a9)
	print(string.format("Lua Respond: %s %d %d %d %d %d %d %d %d %d"), 
		msg, a1, a2, a3, a4, a5, a6, a7, a8, a9)
	pcall(func, a1, a2, a3, a4, a5, a6, a7, a8, a9)
end

function EnterMap(map, character)
	-- force to do move action after enter map
	print("Lua EnterMap:")
	actionRate = 25
end

function MapAction(map, character)
	--print("Lua MapAction", map, character)

	if map == nil or character == nil then
		print("Lua MapAction Error: nil map or character")
		return
	end

	-- random number for action
	local r = math.random(0, actionRate - 1)
	local party = LuaI.GetParty()
	local isLeader = party == nil or character:IsLeader()
	local cmd = nil

	-- move again after battle
	if endBattle then
		endBattle = false
		cmd = "move again"

	--  5%: stop move or move again
	elseif r < 5 and isLeader then
		if character:GetAction() == LuaI.CMapActor_ST_MOVE then
			cmd = "move stop"
		elseif character:GetAction() == LuaI.CMapActor_ST_STOP or
			character:GetAction() == LuaI.CMapActor_ST_NOBATTLE then
			cmd = "move again"
		end

	-- 10%: move to random guide point
	elseif r < 15 and isLeader then
		cmd = "move g" .. math.random(1000)

	-- 10%: move to random jump point
	elseif r < 25 and isLeader then
		cmd = "move j" .. math.random(1000)

	--  5%: join any nearby party or leave party
	elseif r < 30 then
		if party == nil then
			print("Lua robot: try to join party")
			local char2 = map:GetNearCharacter(character:GetCurPosX(), 
				character:GetCurPosY(), 100, 2)
			if char2 then
				cmd = "party join " .. char2:GetChar_id()
			end
		elseif party:GetCount() == 1 then
			print("Lua robot: try to leave own party")
			cmd = "party leave"
		end

	--  5%: invite any nearby character or create party
	elseif r < 35 then
		if isLeader and party and party:GetCount() < 5 then
			print("Lua robot: try to invite someone")
			local char2 = map:GetNearCharacter(character:GetCurPosX(), 
				character:GetCurPosY(), 100, 1)
			if char2 then
				cmd = "party invite " .. char2:GetChar_id()
			end
		elseif not party then
			print("Lua robot: try to create party")
			cmd = "party create"
		end

	--  5%: use random usable item or cheat to create item
	elseif r < 40 then
		print("Lua robot: try use item")
		local bag = LuaI.GetItemBag()
		local ri = math.random(0, 23)
		for i = 0, 23 do
			local item = bag:GetItem((ri + i) % 23)
			if item and item:CanUse(true) then
				cmd = "use " .. item:GetBagIndex()
				break;
			end
		end
		if cmd == nil then
			ri = math.random(1000, 1002)
			print("Lua robot: create item " .. ri)
			cmd = "cheat item create " .. ri
		end

	--  5%: chat local area random message
	elseif r < 45 then
		cmd = "say \\m" .. math.random(1000)

	--  5%: chat any nearby character with random message
	elseif r < 50 then
		local char2 = map:GetNearCharacter(character:GetCurPosX(), 
			character:GetCurPosY(), 100, 0)
		if char2 then
			cmd = "whisper " .. char2:GetChar_id() .. " \\m" .. 
				math.random(1000)
		end

	-- 50%: do nothing and decrease action rate
	else
		actionRate = math.max(0, actionRate - 10)
	end

	if cmd then
		print(string.format("Lui Execute: %s", cmd))
		actionRate = 100
		LuaI.Execute(cmd)
	end
end

function JoinRequest(requester)
	-- accept or reject join request
end

function InviteRequest(requester)
	-- accept or reject invite request
end

function BattleTurn(battle, batchar)
	--print("Lua BattleTurn", battle, batchar)

	if battle == nil or batchar == nil then
		print("Lua MapAction Error: nil battle or batchar")
		return
	end

	-- random number for action
	local r = math.random(0, 99)
	local cmd = nil

	-- 50%: attack random enemy
	if r < 50 then
		local enemyTeam = 1 - batchar:GetTeam()
		local enemyCount = battle:GetTeamCount(enemyTeam)
		local ri = math.random(0, enemyCount - 1)
		local target = GetTeammate(battle, enemyTeam, ri)
		cmd = "attack " .. target:GetIndex()

	-- 10%: use random usable item
	elseif r < 60 then
		local bag = LuaI.GetItemBag()
		local ri = math.random(0, 23)
		local item = nil
		cmd = "repeat"

		for i = 0, 23 do
			local item = bag:GetItem((ri + i) % 23)
			if item and item:CanUse(false) then
				break;
			end
		end
		if item then
			local teamCount = battle:GetTeamCount(batchar:GetTeam())
			if teamCount == 0 then
				cmd = "use " .. item:GetBagIndex() .. " " .. batchar:GetIndex()
			else
				ri = math.random(0, teamCount - 1)
				local target = GetTeammate(battle, batchar:GetTeam(), ri)
				cmd = "use " .. item:GetBagIndex() .. " " .. target:GetIndex()
			end
		end

	-- 10%: defend
	elseif r < 70 then
		cmd = "defend"

	-- 10%: protect lowest life member
	elseif r < 80 then
		local lowlife = GetLowLife(battle, batchar, batchar:GetTeam())
		if lowlife then
			cmd = "protect " .. lowlife:GetIndex()
		else
			cmd = "repeat"
		end

	-- 20%: escape
	else
		cmd = "escape"
	end

	if cmd then
		print(string.format("Lua Execute: %s", cmd))
		LuaI.Execute(cmd)
	end
end

function BattleEnd()
	--print("Lua BattleEnd")

	--endBattle = true
	actionRate = 5
end

function GetLowLife(battle, batchar, team)
	local actor = nil
	local lowlife = nil

	for i = 0, battle:GetActorCount() - 1 do
		actor = battle:GetActor(i)
		if actor and batchar ~= actor and 
			not actor:GetLeftBattle() and
			actor:GetTeam() == team
		then
			if not lowlife or actor:GetHP() < lowlife:GetHP() then
				lowlife = actor
			end
		end	
	end
end

function GetTeammate(battle, team, index)
	local actor = nil
	local teamIndex = 0
	local teamCount = battle:GetTeamCount(team)

	for i = 0, battle:GetActorCount() - 1 do
		actor = battle:GetActor(i)
		if actor and not actor:GetLeftBattle() and
			actor:GetTeam() == team
		then
			if index == teamIndex then
				return actor
			end
			teamIndex = teamIndex + 1
		end	
	end
end

