-- robot script
robotVer = "11"
robotInterval = 2000
cmdList = {}


GuildWar = {
				Bid_Start = 0,
				Bid_End = 1,
				WarPrepare = 2,
				WarStart = 3,
				FirstRoundEnd = 4,
				SecondRoundEnd = 5
			}
GuildWarMap = { 56400, 56401, 56402, 56403, 56404, 56405, 56406, 56407, } 

_G.GuildWarStatus = GuildWar.SecondRoundEnd
_G.GuildWarGuildNumber = 0
_G.GuildWarStartTime = 0
-- set random seed to time
math.randomseed(os.time())

function Hello()
	print("Lua: Hello Robot version", robotVer)
	LuaI.HelloWorld()
end

function DoCommand(cmd)
	table.insert(cmdList, cmd)
end

function ExecCommand()
	local n = table.maxn(cmdList)
	if n == 0 then
		return
	end

	-- random a command in command list to execute
	local cmd = cmdList[math.random(n)]
	--print(string.format("Lui Execute: %s", cmd))
	LuaI.Execute(cmd)
	cmdList = {}
end

function EnterMap(map, character)
	-- force to do move action after enter map
	--print("Lua EnterMap:")
end

function MapAction(map, character)

	if map == nil or character == nil then
		print("Lua MapAction Error: nil map or character")
		return
	end
	
	StartGuildWarBid()	
	GuildWarAction(map, character)
	ExecCommand()

end

function LoginDisconnect(x, y)
   local r = math.random(0, 99)
   local cmd = nil
--   local extraCmd = nil
   
    if r < 98 then
		cmd = "antihack login " .. x .. " " .. y
	else
		cmd = "disconnect"
--		extraCmd = "login"
	end

	if cmd then DoCommand(cmd) end
--	if extraCmd then DoCommand(extraCmd) end
	ExecCommand()
end

function AccountAction()
	local r = math.random(0, 99)
	local cmd = nil

	-- 50%: login
	if r < 50 then
		cmd = "login"
	end
	
	if cmd then LuaI.Execute(cmd) end
end


function StartGuildWarBid()
    if _G.GuildWarStatus == GuildWar.SecondRoundEnd then
        local r = math.random(0, 99)
        if r < 60 then
            _G.GuildWarStatus = GuildWar.Bid_Start
            LuaI.Execute("cheat guildwar endWarSecond")
        end
    end
end

function GuildWarAction(map, character)
	-- 	choose the war action
	if _G.GuildWarStatus == GuildWar.Bid_Start then
	-- if _G.GuildWarGuildNumber <= 16, increment it by one if the one hasn't bid yet
	    local myguild = LuaI.GetGuild();
	    if myguild then
	        if myguild:GetGuildFund() < 10 then
	            if character:GetMoney() < 30000 then
	                LuaI.Execute("cheat money 800000")
				else
        			LuaI.Execute("guild donate 10000")
				end
			else  	-- if _G.GuildWarGuildNumber == 16, change the status to Bid_End
			    if ((myguild:GetGuildBidAmount() == 0) and (myguild:GetGuildPosition() == 1)) then
                    LuaI.Execute("cheat guildwar Bid 2")
     				_G.GuildWarGuildNumber = _G.GuildWarGuildNumber + 1
					if _G.GuildWarGuildNumber >= 16 then
                       _G.GuildWarStatus = GuildWar.Bid_End
					end
				end
			end
   	    end
	elseif _G.GuildWarStatus == GuildWar.Bid_End then
	-- generate group if there is no group (for the guild supposed to be grouped) //once he got bidded but not being grouped
	    local myguild = LuaI.GetGuild();
	    if myguild then
	        if myguild:GetGuildWarGroup() == 0 and myguild:GetGuildBidAmount() ~= 0 then
                LuaI.Execute("cheat guildwar firstGen")
                _G.GuildWarStartTime = os.time()
			elseif myguild:GetGuildWarGroup() > 0 and myguild:GetGuildWarGroup() <= 8 then
			    local timenow = os.time()
			    if (timenow - _G.GuildWarStartTime) > 5 then
       				LuaI.Execute("cheat guildwar startPrepare")
			    	_G.GuildWarStatus = GuildWar.WarPrepare
			    end

			end
	    end
	-- otherwise if there is already grouping, change the status to WarStart and cheat WarPrepare // reset the bid to false if group is assigned
	elseif _G.GuildWarStatus == GuildWar.WarPrepare then
    	local myguild = LuaI.GetGuild();
     	if myguild then
     	    local timenow = os.time()
        	if myguild:GetGuildWarGroup() > 0 and myguild:GetGuildWarGroup() <= 8 and
				(timenow - _G.GuildWarStartTime) > 10  then
    			LuaI.Execute("cheat guildwar startWar")
            	_G.GuildWarStatus = GuildWar.WarStart
        	end
     	end
	elseif _G.GuildWarStatus == GuildWar.WarStart or _G.GuildWarStatus == GuildWar.FirstRoundEnd then
		local timenow = os.time()
		local minute = 60
	    local myguild = LuaI.GetGuild();
	    if myguild and map then
	        if myguild:GetGuildWarGroup() > 0 and myguild:GetGuildWarGroup() <= 8 then -- if the one with war group
	            if map:GetMap_id() ~= GuildWarMap[myguild:GetGuildWarGroup()] then
	                if character:GetLevel() < 20 then
	                    LuaI.Execute("cheat level 80")
					elseif character:GetVP() < 30 then
					    LuaI.Execute("cheat vp 500")
					else
					    if ((timenow - _G.GuildWarStartTime) < (15 * minute)) then
					    	local cmd = "cheat jump " .. GuildWarMap[myguild:GetGuildWarGroup()] .. " 1 30 130"
							LuaI.Execute(cmd)
						end
					end
				else
				    local isLeader = not character:HasParty() or character:IsLeader()
				    if isLeader then
				        if ((timenow - _G.GuildWarStartTime) > (16 * minute)) then
                 			LuaI.Execute("cheat jump 20300 1 177 123")
						else
					        local target = map:FindEnemyCharacterNearby();
					        if target then
					            local cmd = "battle pk " .. target:GetChar_id()
                 				LuaI.Execute(cmd)
					        end
				        end
					end
				end
			end
	    end
	    if _G.GuildWarStatus == GuildWar.WarStart and ((timenow - _G.GuildWarStartTime) > (10 * minute)) then
   			LuaI.Execute("cheat guildwar endWarFirst")
			_G.GuildWarStatus = GuildWar.FirstRoundEnd
		elseif _G.GuildWarStatus == GuildWar.FirstRoundEnd and ((timenow - _G.GuildWarStartTime) > (20 * minute)) then
   			LuaI.Execute("cheat guildwar endWarSecond")
            _G.GuildWarStatus = GuildWar.SecondRoundEnd
            _G.GuildWarGuildNumber = 0
			_G.GuildWarStartTime = 0
		end
	end
end

function HackAnswer(ansX, ansY)
	local randHackReply = math.random(0, 4)
	local cmd = nil
	if randHackReply < 3 then  -- 3/5 chance of giving the correct answer
		cmd = "hack " .. ansX .. " " .. ansY
	elseif randHackReply < 4 then -- 1/5 chance of giving the incorrect answer  
		cmd = "hack 10000 10000" -- give out a must-be incorrect answer
	end
	-- 1/5 chance of not giving any answer, wating for time-out	
	if cmd then LuaI.Execute(cmd) end
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
		-- the initialization of quest values before disconnect --
--		OneSubQuestRunning = false
--		SubQTStartTalk = false
--	    RepeatNPCTalk = false
--      QTT_ID = 0
        ----------------------------------------------------------
--		cmd = "disconnect"

	--  3%: nothing (repeat previous command)
	end

	if cmd == nil then cmd = "repeat" end

	if cmd then
		--print(string.format("Lua Execute: %s", cmd))
		LuaI.Execute(cmd)
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



