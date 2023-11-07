-- robot2 script

-- set random seed to time
math.randomseed(os.time())

function Hello()
	print("Lua: Robot1 Hello World")
	LuaI.HelloWorld()
end

function EnterMap(map, character)
	-- force to do move action after enter map
	print("Lua EnterMap:")
end

function RandWait(t1, t2)
	-- random next wait time within t1 to t2
	LuaI.gActionTime = math.random(t1, t2)
	print(string.format("Lua: wait for %.1f second for next robot action",
		LuaI.gActionTime / 1000))
end

function MapAction(map, character)
	--print("Lua MapAction", map, character)

	if map == nil or character == nil then
		print("Lua MapAction Error: nil map or character")
		return
	end

	local isLeader = not character:HasParty() or character:IsLeader()
	
	-- 2%: reconnect at any time
	local r = math.random(0, 99)
	if r < 2 then
		DoCommand("reconnect")
		RandWait(1000, 30000)		-- wait action within 1s to 30s
		return
	end

	if isLeader then MoveAction(map, character) end
	if isLeader then PartyAction(map, character) end
	ItemAction(map, character)
	ChatAction(map, character)

end

function DoCommand(cmd)
	print(string.format("Lui Execute: %s", cmd))
	LuaI.Execute(cmd)
end

function AccountAction()
	local r = math.random(0, 99)
	local cmd = nil

	-- 50%: login
	if r < 50 then
		cmd = "login"
	end
	
	if cmd then DoCommand(cmd) end
end

function MoveAction(map, character)
	local r = math.random(0, 99)
	local cmd = nil

	-- character is moving
	if character:GetAction() == LuaI.CMapActor_ST_MOVE then

		-- 10%: stop move
		if r < 10 then
			cmd = "move stop"

		-- 10%: move to random guide point
		elseif r < 20 then
			cmd = "move g" .. math.random(1000)

		-- 10%: move to random jump point
		elseif r < 30 then
			cmd = "move j" .. math.random(1000)

		-- 60%: nothing
		end

	-- character is stopped
	elseif character:GetAction() == LuaI.CMapActor_ST_STOP then
		-- 40%: move to random guide point
		if r < 40 then
			cmd = "move g" .. math.random(1000)

		-- 40%: move to random jump point
		elseif r < 80 then
			cmd = "move j" .. math.random(1000)

		-- 20%: nothing
		end

	-- move again after battle
	elseif character:GetAction() == LuaI.CMapActor_ST_NOBATTLE then
		local lastMove = LuaI.GetLastMove()
		print(string.format("Lua: last move %d,%d", lastMove.PosX, lastMove.PosY))
		if lastMove.PosX ~= 0 or lastMove.PosY ~= 0 then
			cmd = "move again"
		else
			-- 40%: move to random guide point
			if r < 40 then
				cmd = "move g" .. math.random(1000)

			-- 40%: move to random jump point
			elseif r < 80 then
				cmd = "move j" .. math.random(1000)
				
			-- 20%: nothing
			end
		end

	end

	if cmd then DoCommand(cmd) end
end

function PartyAction(map, character)
	local r = math.random(0, 99)
	local cmd = nil
	local party = LuaI.GetParty()

	-- no party
	if party == nil then
		--  5%: join any nearby party
		if r < 5 then
			print("Lua robot: try to join party")
			local char2 = map:GetNearCharacter(character:GetCurPosX(), 
				character:GetCurPosY(), 100, 2)
			if char2 then
				cmd = "party join " .. char2:GetChar_id()
			end
		--  3%: create own party
		elseif r < 8 then
			print("Lua robot: try to create party")
			cmd = "party create"
		end

	-- has party
	else
		--  5%: invite any nearby character
		if r < 5 and party:GetCount() < 5 then
			print("Lua robot: try to invite someone")
			local char2 = map:GetNearCharacter(character:GetCurPosX(), 
				character:GetCurPosY(), 100, 1)
			if char2 then
				cmd = "party invite " .. char2:GetChar_id()
			end
		--  5%: leave party when single
		elseif r < 10 and party:GetCount() == 1 then
			print("Lua robot: try to leave own party")
			cmd = "party leave"

		--  5%: kick one member out of party
		elseif r < 15 and party:GetCount() > 1 then
			print("Lua robot: try to kick member 1 out of party")
			local char2 = party:GetMember(1)
			if char2 then
				cmd = "party kick " .. char2:GetChar_id()
			end
		end
	end

	if cmd then DoCommand(cmd) end
end

function ItemAction(map, character)
	local r = math.random(0, 99)
	local cmd = nil

	--  5%: use random usable item
	if r < 5 then
		print("Lua robot: try use item")
		local bag = LuaI.GetItemBag()
		local r2 = math.random(0, 23)
		for i = 0, 23 do
			local item = bag:GetItem((r2 + i) % 23)
			if item and item:CanUse(true) then
				cmd = "use " .. item:GetBagIndex()
				break
			end
		end

	--  5%: cheat to create item
	elseif r < 10 then
		r2 = math.random(1000, 1002)
		print("Lua robot: create item " .. r2)
		cmd = "cheat item create " .. r2
	end

	if cmd then DoCommand(cmd) end
end

function ChatAction(map, character)
	local r = math.random(0, 99)
	local cmd = nil

	--  5%: chat local area random message
	if r < 5 then
		cmd = "say \\m" .. math.random(1000)

	--  5%: chat any nearby character with random message
	elseif r < 10 then
		local char2 = map:GetNearCharacter(character:GetCurPosX(), 
			character:GetCurPosY(), 100, 0)
		if char2 then
			cmd = "whisper " .. char2:GetChar_id() .. " \\m" .. 
				math.random(1000)
		end
	end

	if cmd then DoCommand(cmd) end
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

		for i = 0, 23 do
			local t = bag:GetItem((ri + i) % 23)
			if t and t:CanUse(false) then
				item = t
				break
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
		end

	--  5%: escape
	elseif r < 85 then
		cmd = "escape"

	--  2%: reconnect
	elseif r < 87 then
		cmd = "reconnect"

	-- 13%: nothing (repeat previous command)
	end

	if cmd == nil then cmd = "repeat" end

	if cmd then
		print(string.format("Lua Execute: %s", cmd))
		LuaI.Execute(cmd)
		if cmd == "reconnect" then 
			RandWait(1000, 30000)		-- wait action within 1 to 30s
		end
	end
end

function BattleEnd()
	--print("Lua BattleEnd")
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

