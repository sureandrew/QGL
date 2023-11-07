-- robot script
robotVer = "7"
robotInterval = 2000

-- set random seed to time
math.randomseed(os.time())

function Hello()
	print("Lua: Hello Robot version", robotVer)
	LuaI.HelloWorld()
end

function EnterMap(map, character)
	-- force to do move action after enter map
	--print("Lua EnterMap:")
end

function MapAction(map, character)
	--print("Lua MapAction", map, character)

	if map == nil or character == nil then
		print("Lua MapAction Error: nil map or character")
		return
	end

	local isLeader = not character:HasParty() or character:IsLeader()
	
	-- 1%: disconnect at any time
	local r = math.random(0, 99)
	if r < 1 then
		DoCommand("disconnect")
		return
	end

	if isLeader then MoveAction(map, character) end
	if isLeader then PartyAction(map, character) end
	ItemAction(map, character)
	ChatAction(map, character)
	PartnerAction(map, character)
	MiscAction(map, character)

end

function DoCommand(cmd)
	--print(string.format("Lui Execute: %s", cmd))
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

		-- 5%: nothing
		--elseif r < 35 then
		--	cmd = "battle"		-- try battle

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
		--print(string.format("Lua: last move %d,%d", lastMove.PosX, lastMove.PosY))
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
	local party = character:GetParty()

	-- no party
	if party == nil then
		--  5%: join any nearby party
		if r < 5 then
			--print("Lua robot: try to join party")
			local char2 = map:GetNearCharacter(character:GetCurPosX(), 
				character:GetCurPosY(), 100, 2)
			if char2 then
				cmd = "party join " .. char2:GetChar_id()
			end
		--  3%: create own party
		elseif r < 8 then
			--print("Lua robot: try to create party")
			cmd = "party create"
		end

	-- has party
	else
		--  5%: invite any nearby character
		if r < 5 and party:GetCount() < 5 then
			--print("Lua robot: try to invite someone")
			local char2 = map:GetNearCharacter(character:GetCurPosX(), 
				character:GetCurPosY(), 100, 1)
			if char2 then
				cmd = "party invite " .. char2:GetChar_id()
			end
		--  5%: leave party when single
		elseif r < 10 then -- and party:GetCount() == 1 then
			--print("Lua robot: try to leave own party")
			cmd = "party leave"

		--  5%: kick one member out of party
		elseif r < 15 and party:GetCount() > 1 then
			--print("Lua robot: try to kick member 1 out of party")
			local char2 = party:GetMember(1)
			if char2 then
				cmd = "party kick " .. char2:GetChar_id()
			end

		--  5%: promote member
		elseif r < 20 and party:GetCount() > 1 then
			--print("Lua robot: try to promote member 1 as party leader")
			local char2 = party:GetMember(1)
			if char2 then
				cmd = "party promote " .. char2:GetChar_id()
			end
		end
	end

	if cmd then DoCommand(cmd) end
end

function PartyRequest(character, target)
	local r = math.random(0, 99)
	local cmd = nil
	local party = character.GetParty()
end

function ItemAction(map, character)
	local r = math.random(0, 99)
	local cmd = nil

	--  5%: use random usable item
	if r < 5 then
		--print("Lua robot: try use item")
		local bag = LuaI.GetItemBag()
		local r2 = math.random(0, 23)
		for i = 0, 23 do
			local item = bag:GetItem((r2 + i) % 23)
			if item and item:CanUse(true) then
				cmd = "use " .. item:GetBagIndex()
				break
			end
		end

	--  2%: equip item
	elseif r < 7 then
		--print("Lua robot: try equip item")
		local bag = LuaI.GetItemBag()
		local r2 = math.random(0, 23)
		for i = 0, 23 do
			local item = bag:GetItem((r2 + i) % 23)
			if item and item:CanEquip() then
				cmd = "equip " .. item:GetBagIndex()
				break
			end
		end

	--  2%: unequip item
	elseif r < 9 then
		--print("Lua robot: try unequip item")
		local bag = LuaI.GetItemBag()
		local r2 = math.random(0, 8) + 120
		local item = bag:GetItem(r2)
		if item then
			cmd = "equip " .. item:GetBagIndex()
		end

	--  5%: cheat to create item
	elseif r < 14 then
		local r2 = math.random(0, 3)
		if r2 == 0 then r2 = 11053
		elseif r2 == 1 then r2 = 11059
		elseif r2 == 2 then r2 = 11065
		elseif r2 == 3 then r2 = 11077
		end
		--print("Lua robot: create item " .. r2)
		cmd = "cheat item create " .. r2
	end

	if cmd then DoCommand(cmd) end
end

function ChatAction(map, character)
	local r = math.random(0, 99)
	local cmd = nil

	--  3%: chat local area random message
	if r < 3 then
		cmd = "say \\m" .. math.random(1000)

	--  3%: chat any nearby character with random message
	elseif r < 6 then
		local char2 = map:GetNearCharacter(character:GetCurPosX(), 
			character:GetCurPosY(), 100, 0)
		if char2 then
			cmd = "whisper " .. char2:GetChar_id() .. " \\m" .. 
				math.random(1000)
		end
	end

	if cmd then DoCommand(cmd) end
end

function PartnerAction(map, character)
	local partnerGroup = LuaI:GetPartnerGroup()

	if partnerGroup == nil or partnerGroup:GetCount() == 0 then
		return
	end

	local r = math.random(0, 99)
	local cmd = nil
	local index = math.random(0, partnerGroup:GetCount() - 1)
	local partner = partnerGroup:Get(index)

	if partner == nil then return end

	--  5%: show all partners
	if r < 5 then
		cmd = "partner showall"

	--  5%: partner follow
	elseif r < 10 then
		cmd = "partner " .. index .. " follow"

	--  5%: partner stop follow
	elseif r < 15 then
		cmd = "partner " .. index .. " stop"

	--  5%: partner set fight
	elseif r < 20 and partner:GetState() ~= 1 then
		cmd = "partner " .. index .. " fight"

	--  5%: partner set standby
	elseif r < 25 and partner:GetState() ~= 0 then
		cmd = "partner " .. index .. " standby"

	--  1%: delete partner
	elseif r < 26 then
		cmd = "partner " .. index .. " delete"

	end

	if cmd then DoCommand(cmd) end
end

function MiscAction(map, character)
	local r = math.random(0, 99)
	local cmd = nil

	--  5%: check level up
	if r < 5 and character:GetExp() >= character:GetNextLvExp() then
		cmd = "levelup"

	--  5%: random assign attribute points
	elseif r < 10 and character:GetAttrPoint() > 0 then
		local ap = character:GetAttrPoint()
		local a = {}
		local args = ""
		for i = 1, 5 do
			a[i] = math.random(0, ap)
			ap = ap - a[i]
			args = " " .. a[i]
		end
		cmd = "ap_add" .. args
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

	-- 40%: attack random enemy
	if r < 40 then
		local target = GetRandTarget(battle, 1 - batchar:GetTeam())
		if target then
			cmd = "attack " .. target:GetIndex()
		end

	-- 10%: use skill
	elseif r < 50 then

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
			local target = GetRandTarget(battle, batchar:GetTeam())
			if target == nil then
				cmd = "use " .. item:GetBagIndex() .. " " .. batchar:GetIndex()
			else
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

	--  5%: catch
	elseif r < 90 then
		local lowlife = GetLowLife(battle, batchar, 1 - batchar:GetTeam())
		if lowlife then
			cmd = "catch " .. lowlife:GetIndex()
		end

	--  5%: call friend
	elseif r < 95 then
		local partnerGroup = LuaI:GetPartnerGroup()
		if partnerGroup and partnerGroup:GetCount() > 0 then
			local partner = nil
			local index = -1
			for i = 0, partnerGroup:GetCount() do
				partner = partnerGroup:Get(i)
				if partner and partner:CanCall() then
					index = i
					break
				end
			end
			if index ~= -1 then
				cmd = "callfriend " .. index
			end
		end

	--  1%: disconnect
	elseif r < 96 then
		cmd = "disconnect"

	-- 13%: nothing (repeat previous command)
	end

	if cmd == nil then cmd = "repeat" end

	if cmd then
		--print(string.format("Lua Execute: %s", cmd))
		LuaI.Execute(cmd)
	end

	local partner = LuaI:GetBatPartner()
	if partner then
		PartnerTurn(battle, batchar, partner)
	end
end

function PartnerTurn(battle, batchar, partner)
	--print("Lua PartnerTurn", battle, partner)

	if battle == nil or partner == nil then
		print("Lua MapAction Error: nil battle or partner")
		return
	end

	-- random number for action
	local r = math.random(0, 99)
	local cmd = nil

	-- 50%: attack random enemy
	if r < 50 then
		local target = GetRandTarget(battle, 1 - batchar:GetTeam())
		if target then
			cmd = "partner_attack " .. target:GetIndex()
		end

	-- 20%: defend
	elseif r < 70 then
		cmd = "partner_defend"

	-- 20%: protect character
	elseif r < 90 then
		cmd = "partner_protect " .. batchar:GetIndex()

	-- 10%: escape
	else
		cmd = "partner_escape"
	end

	if cmd == nil then cmd = "attack 0" end

	if cmd then
		--print(string.format("Lua Execute: %s", cmd))
		LuaI.Execute(cmd)
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

	return nil
end

function GetRandTarget(battle, team)
	-- get random enemy
	local count = battle:GetTeamCount(team)
	local ri = math.random(0, count - 1)
	return GetTeammate(battle, team, ri)
end

