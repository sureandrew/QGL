-- robot script
robotVer = "10"
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
	if r < 3 then
		DoCommand("disconnect")
		return
	end

	if isLeader then MoveAction(map, character) end
	if isLeader then PartyAction(map, character) end
	ItemAction(map, character)
	SkillAction(map, character)
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

		-- 5%: change map line
		elseif r < 35 then
			local line_id = LuaI.GetNextLine(map:GetLine_id())
			if line_id ~= map:GetLine_id() then
				cmd = "mapline " .. line_id
			end

		-- 2%: try monitor other character if this character is single
		elseif r < 37 and not character:HasParty() then
			local char2 = map:GetNearCharacter(character:GetCurPosX(), 
				character:GetCurPosY(), 100, 3)
			if char2 then
				cmd = "monitor " .. char2:GetChar_id()
			end

		-- 2%: try do PK battle with other character
		elseif r < 39 and character:GetEnablePK() then
			local char2 = map:GetNearCharacter(character:GetCurPosX(), 
				character:GetCurPosY(), 100, 5)
			if char2 then
				cmd = "battle pk " .. char2:GetChar_id()
			end

		-- 5%: nothing
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
		local t = {11053, 11059, 11065, 11077}
		local r2 = math.random(1, table.maxn(t))
		--print("Lua robot: create item " .. r2)
		cmd = "cheat item create " .. t[r2]
	end

	if cmd then DoCommand(cmd) end
end

function SkillAction(map, character)
	local r = math.random(0, 99)
	local cmd = nil

	--  2%: learn or upgrade new skill
	if r < 2 then
		local t = {1, 14, 28, 43, 55, 69, 82, 97, 111, 127, 146, 161}
		local r2 = math.random(1, table.maxn(t))
		local skill = LuaI.GetSkill(r2)
		if skill then
			--print("Lua robot: skill upgrade " .. r2)
			cmd = "cheat skill upgrade " .. skill:GetSkill_id()
		elseif LuaI.GetSkillCount() < 10 then
			--print("Lua robot: skill learn " .. r2)
			cmd = "cheat skill learn " .. t[r2] .. " 10"
		end

	--  1%: remove skill
	elseif r < 3 then
		local c = LuaI.GetSkillCount()
		if c > 0 then
			local skill = LuaI.GetSkill(math.random(0, c - 1))
			if skill then
				--print("Lua robot: skill remove " .. r2)
				cmd = "cheat skill remove " .. skill:GetSkill_id()
			end
		end

	--  2%: learn or remove one partner skill
	elseif r < 5 then
		local partnerGroup = LuaI.GetPartnerGroup()
		local partner = nil
		local pi = 0
		if partnerGroup:GetCount() > 0 then
			pi = math.random(0, partnerGroup:GetCount() - 1)
			partner = partnerGroup:Get(pi)
		end
		if partner then
			local r2 = math.random(248, 327)	-- partner skills range
			local skill = partner:GetSkillById(r2)
			if skill then
				--print("Lua robot: partner skill remove " .. pi, r2)
				cmd = "cheat partner " .. pi .. " removeskill " .. r2
			else
				--print("Lua robot: partner skill learn " .. pi, r2)
				cmd = "cheat partner " .. pi .. " skill " .. r2
			end
		end

	--  2%: use skill in map
	elseif r < -5 then
		local c = LuaI.GetSkillCount()
		if c > 0 then
			r = math.random(0, c - 1)
			local skill = LuaI.GetSkill(r)
			if skill:CanUse(true) then
				cmd = "skill " .. r
			end
		end
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
	local partnerGroup = LuaI.GetPartnerGroup()

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
		cmd = "partner"

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
			a[i] = math.min(math.random(0, ap), math.floor(ap / (6 - i)) + 3)
			ap = ap - a[i]
			args = args .. " " .. a[i]
		end
		cmd = "ap_add" .. args

	--  5%: enable/disable PK
	--elseif r < 15 then
	--	local enable = character:GetEnablePK() and 0 or 1
	--	cmd = "set_pk " .. enable

	end

	if cmd then DoCommand(cmd) end
end

function JoinRequest(requester)
	-- accept or reject join request
end

function InviteRequest(requester)
	-- accept or reject invite request
end

function BattleTurn(battle, batchar, turnNum)
	--print("Lua BattleTurn", battle, batchar, turnNum)

	if battle == nil or batchar == nil then
		print("Lua BattleTurn Error: nil battle or batchar")
		return
	end

	-- random number for action
	local r = math.random(0, 99)
	--local r = math.random(85, 94)
	local cmd = nil

	--  50%: need help in first turn before other command
	if r < 50 and turnNum == 0 then
		cmd = "help"
		r = 100
	end

	-- 40%: attack random enemy
	if r < 40 then
		--print("Lua BattleTurn: attack")
		local target = GetRandTarget(battle, 1 - batchar:GetTeam())
		if target then
			cmd = "attack " .. target:GetIndex()
		end

	-- 10%: use skill
	elseif r < 50 then
		local c = LuaI.GetSkillCount()
		--print("Lua BattleTurn: skill", c)
		if c > 0 then
			c = c - 1
			local ri = math.random(0, c)
			for i = 0, c do
				local skill = LuaI.GetSkill((ri + i) % c)
				if skill and skill:CanUse(false) then
					local t = skill:GetTargetType()
					local target = -1
					if t == 0 then
						target = math.random(0, battle:GetActorCount() - 1)
					elseif t == 1 then
						target = batchar:GetIndex()
					elseif t == 2 then
						local partner = LuaI.GetBatPartner()
						if partner then
							target = partner:GetIndex()
						end
					elseif t == 4 then
						local t = GetRandTarget(battle, batchar:GetTeam())
						if t then
							target = t:GetIndex()
						end
					else
						local t = GetRandTarget(battle, 1 - batchar:GetTeam())
						if t then
							target = t:GetIndex()
						end
					end

					-- cheat character sp to use skill	
					if batchar:GetSP() < 100 then
						cmd = "cheat sp " .. (batchar:GetSP() + 100)
						LuaI.Execute(cmd)
					end
					cmd = "skill " .. skill:GetSkill_id() .. " " .. target
					break
				end
			end
		end

	-- 10%: use random usable item
	elseif r < 60 then
		local bag = LuaI.GetItemBag()
		local ri = math.random(0, 23)
		local item = nil

		--print("Lua BattleTurn: use item", ri)

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
		--print("Lua BattleTurn: defend")
		cmd = "defend"

	-- 10%: protect lowest life member
	elseif r < 80 then
		--print("Lua BattleTurn: protect")
		local lowlife = GetLowLife(battle, batchar, batchar:GetTeam())
		if lowlife then
			cmd = "protect " .. lowlife:GetIndex()
		end

	--  5%: escape
	elseif r < 85 then
		--print("Lua BattleTurn: escape")
		cmd = "escape"

	--  5%: catch
	elseif r < 90 then
		--print("Lua BattleTurn: catch")
		local lowlife = GetLowLife(battle, batchar, 1 - batchar:GetTeam())
		if lowlife then
			-- cheat character sp & mob hp to catch
			if batchar:GetSP() < 100 then
				cmd = "cheat sp " .. (batchar:GetSP() + 100)
				LuaI.Execute(cmd)
			end
			cmd = "cheat " .. lowlife:GetIndex() .. " hp 1"
			LuaI.Execute(cmd)
			cmd = "catch " .. lowlife:GetIndex()
		end

	--  5%: call friend
	elseif r < 95 then
		--print("Lua BattleTurn: call friend")
		local partnerGroup = LuaI.GetPartnerGroup()
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
				cmd = "call " .. index
			end
		end

	--  2%: disconnect
	elseif r < 99 then
		--print("Lua BattleTurn: disconnect")
		cmd = "disconnect"

	--  3%: nothing (repeat previous command)
	end

	if cmd == nil then cmd = "repeat" end

	if cmd then
		--print(string.format("Lua Execute: %s", cmd))
		LuaI.Execute(cmd)
	end

	local partner = LuaI.GetBatPartner()
	if partner then
		PartnerTurn(battle, batchar, partner)
	end
end

function PartnerTurn(battle, batchar, partner)
	--print("Lua PartnerTurn", battle, partner)

	if battle == nil or partner == nil then
		print("Lua PartnerTurn Error: nil battle or partner")
		return
	end

	-- random number for action
	local r = math.random(0, 99)
	local cmd = nil

	-- 50%: attack random enemy
	if r < 50 then
		local target = GetRandTarget(battle, 1 - partner:GetTeam())
		if target then
			cmd = "p_attack " .. target:GetIndex()
		end

	-- 10%: use skill
	elseif r < 60 then
		local c = partner:GetSkillCount()
		--print("Lua BattleTurn: partner skill", c)
		if c > 0 then
			c = c - 1
			local ri = math.random(0, c)
			for i = 0, c do
				local skill = partner:GetSkill((ri + i) % c)
				if skill and skill:CanUse(false) then
					local t = skill:GetTargetType()
					local target = -1
					if t == 0 then
						target = math.random(0, battle:GetActorCount() - 1)
					elseif t == 1 then
						target = partner:GetIndex()
					elseif t == 4 then
						local t = GetRandTarget(battle, partner:GetTeam())
						if t then
							target = t:GetIndex()
						end
					else
						local t = GetRandTarget(battle, 1 - partner:GetTeam())
						if t then
							target = t:GetIndex()
						end
					end

					cmd = "p_skill " .. skill:GetSkill_id() .. " " .. target
					break
				end
			end
		end

	-- 20%: defend
	elseif r < 80 then
		cmd = "p_defend"

	-- 10%: protect character
	elseif r < 90 then
		cmd = "p_protect " .. batchar:GetIndex()

	-- 10%: escape
	else
		cmd = "p_escape"
	end

	if cmd == nil then cmd = "p_attack 0" end

	if cmd then
		--print(string.format("Lua Execute: %s", cmd))
		LuaI.Execute(cmd)
	end
end

function MonitorTurn(battle, turnNum)
	--print("Lua MonitorTurn", battle)

	if battle == nil then
		print("Lua MonitorTurn Error: nil battle")
		return
	end

	-- random number for action
	local r = math.random(0, 99)
	local cmd = nil

	-- 50%: join battle if need help
	if r < 50 then
		if battle:IsNeedHelp() then
			cmd = "battle join"
		end

	-- 5%: leave monitor
	elseif r < 55 then
		cmd = "monitor leave"
	end

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
		if actor and batchar:GetIndex() ~= actor:GetIndex() and 
			not actor:GetLeftBattle() and
			actor:GetTeam() == team
		then
			if not lowlife or actor:GetHP() < lowlife:GetHP() then
				lowlife = actor
			end
		end	
	end

	return lowlife
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

