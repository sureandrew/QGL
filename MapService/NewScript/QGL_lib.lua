-- Add quest to character
function Preload(filename, skip)
	if filename == "" or skip then return end
	luaFilename = filename
	local curEnv
	local loadedFiles
	if script then
		curEnv = getfenv(2)
		loadedFiles = curEnv["loadedFiles"]
	end
	local func = nil
	if loadedFiles and loadedFiles[filename] then
		--print("Preload from memory:", filename)
		func = loadedFiles[filename]
	else
		local path = string.lower(filename)
		if string.find(path, "^quest%d+") then
			path = "Quest/" .. path
		elseif string.find(path, "^item%d+") then
			path = "Item/" .. path
		elseif string.find(path, "^npc%d+") then
			path = "Npc/" .. path
		elseif string.find(path, "^bat%d+") then
			path = "Battle/" .. path
		elseif string.find(path, "^inst%d+") then
			_inst_preload = true
			path = "Instance/" .. path
		end
		path = "./Data/NewScript/" .. path
		--print("Preloading:", filename, path)
		func = loadfile(path)
		if func == nil then  				-- try load another file
			if string.find(path, ".out", 0, false) then
				path = string.gsub(path, ".out", ".lua")
			elseif string.find(path, ".lua", 0, false) then
				path = string.gsub(path, ".lua", ".out")
			end
			func = loadfile(path)
		end
		if curEnv then
			if loadedFiles == nil then
				curEnv["loadedFiles"] = {}
			end
			curEnv["loadedFiles"][filename] = func
			--DebugTable(curEnv["loadedFiles"], true)
		end
	end
	if func then func() end
	if _inst_preload then _inst_preload = nil end
end

function DebugTable(t, showFunc, indent)
	if indent == nil then 
		indent = ""
		print("DebugTable:")
	end
	for k,v in pairs(t) do
		if showFunc or type(v) ~= "function" then
			if type(v) == "table" then
				print(indent, k .. ":")
				DebugTable(v, showFunc, indent .. "\t")
			else
				print(indent, k .. ":", v)
			end
		end
	end
end

-- character has quest
-- quest_ids : table of quest id
-- character : character to check the quest. if nil, then character is "player"
function HasQuest(quest_ids, character)
	if quest_ids == nil then return false end
	if character == nil then character = player end
	local qs = quest_ids
	if type(quest_ids) ~= "table" then
		qs = { quest_ids }
	end
	local questSys = character:GetQuests()
	for i,v in ipairs(qs) do
		if questSys:HasQuest(v) then
			return true
		end
	end
	return false
end

-- Can add quest round for character
-- return true or false with msg
function CanAddRound(type_id, character)
	if character == nil then character = player end
	local qtype = character:GetQuests():GetType(type_id)
	if qtype == nil then return true end

	local maxRound = qtype:GetMaxRound()
	local limitRound = qtype:GetLimitRound()
	return (limitRound == 0 or qtype:GetTotalRound() < limitRound)
end

-- Add quest to character
-- quest_t : lua quest table to add
-- character : character to add the quest. if nil, then character is "player"
function AddQuest(quest_t, character, addRound)
	if not IsProcess() then return nil end
	if character == nil then character = player end
	if character == nil then return nil end
	local questSys = character:GetQuests()

	DebugTable(quest_t)
	if questSys:HasQuest(quest_t.quest_id) then
		ShowError(string.format("玩家\"%s\"身上已有\“%s\”，无法再次接获。", character:GetName(), quest_t.name))
		return nil
	end

	if quest_t.round_info then
		local t = quest_t.round_info
		questSys:SetRoundInfo(quest_t.type_id, t.max_round or 0, 
			t.limit_round or 0, t.limit_type or 0)
	end

	if addRound == nil or addRound then
		local qtype = questSys:GetType(quest_t.type_id)
		if qtype then
			local limitRound = qtype:GetLimitRound()
			local limitType = qtype:GetLimitType()
			if limitRound > 0 and qtype:GetTotalRound() >= limitRound then
				if limitType == 1 then
					NpcSay(string.format("你今天领取的每日任务已到达领取上限%d次，请明天再领取。", limitRound))
				elseif limitType == 2 then
					NpcSay(string.format("你今天领取的每周任务已到达领取上限%d次，请下星期再领取。", limitRound))
				end
				return nil
			end
			qtype:ChangeRound(1)
			qtype:ChangeTotalRound(1)
		end
	end

	local q = questSys:AddQuest(quest_t.quest_id, quest_t.name, 
			quest_t.type_id, 
			quest_t.type_name or string.format("T %d", quest_t.type_id))
	local v

	if q == nil then return nil end

	if quest_t.descript then 
		if quest_t.tips then
			q:SetDescription(quest_t.descript .. "##" .. quest_t.tips)
		else
			q:SetDescription(quest_t.descript)
		end
	end
	v = quest_t.start_npc
	if type(v) ~= "table" and npc then
		v = { npc_id = npc:GetNpc_id(), map_id = npc:GetMap_id(), 
				x = npc:GetCurPosX(), y = npc:GetCurPosY() }
	end
	if v then
		q:SetStartNPC(v.npc_id, v.map_id, v.x or 0, v.y or 0)
	end
	v = quest_t.end_npc
	if type(v) ~= "table" and npc then
		v = { npc_id = npc:GetNpc_id(), map_id = npc:GetMap_id(), 
				x = npc:GetCurPosX(), y = npc:GetCurPosY() }
	end
	if v then
		q:SetEndNPC(v.npc_id, v.map_id, v.x or 0, v.y or 0)
	end
	if quest_t.target_npc and type(quest_t.target_npc) == "table" then
		if quest_t.target_npc.npc_id then
			v = quest_t.target_npc
			q:SetTargetNPC(1, v.npc_id, v.map_id, v.x or 0, v.y or 0)
		else
			for i,v in ipairs(quest_t.target_npc) do
				q:SetTargetNPC(i, v.npc_id, v.map_id, v.x or 0, v.y or 0)
			end
		end
	end
	if quest_t.target_item and type(quest_t.target_item) == "table" then
		if quest_t.target_item.item_id then
			v = quest_t.target_item
			q:SetTargetItem(1, v.item_id, v.count or 1, v.t1 or 0,
				v.v1 or 0, v.t2 or 0, v.v2 or 0)
		else
			for i,v in ipairs(quest_t.target_item) do
				q:SetTargetItem(i, v.item_id, v.count or 1, v.t1 or 0,
					v.v1 or 0, v.t2 or 0, v.v2 or 0)
			end
		end
	end
	if quest_t.target_mob and type(quest_t.target_mob) == "table" then
		if quest_t.target_mob.mob_id then
			v = quest_t.target_mob
			q:SetTargetMOB(1, v.mob_id, v.count or 1)
		else
			for i,v in ipairs(quest_t.target_mob) do
				q:SetTargetMOB(i, v.mob_id, v.count or 1)
			end
		end
	end
	if quest_t.target_partner and type(quest_t.target_partner) == "table" then
		if quest_t.target_partner.mob_id then
			v = quest_t.target_partner
			q:SetTargetPartner(1, v.mob_id, v.count or 1, v.t1 or 0,
				v.v1 or 0, v.t2 or 0)
		else
			for i,v in ipairs(quest_t.target_partner) do
				q:SetTargetPartner(i, v.mob_id, v.count or 1, v.t1 or 0,
					v.v1 or 0, v.t2 or 0)
			end
		end
	end
	if quest_t.target_pet and type(quest_t.target_pet) == "table" then
		if quest_t.target_pet.pet_id then
			v = quest_t.target_pet
			q:SetTargetPet(1, v.pet_id, 0, v.count or 1, v.t1 or 0,
				v.v1 or 0, v.t2 or 0)
		else
			for i,v in ipairs(quest_t.target_pet) do
				q:SetTargetPet(i, v.pet_id, 0, v.count or 1, v.t1 or 0,
					v.v1 or 0, v.t2 or 0)
			end
		end
	end
	if quest_t.target_map and type(quest_t.target_map) == "table" then
		if quest_t.target_map.map_id then
			v = quest_t.target_map
			q:SetTargetMap(1, v.map_id, v.x or 0, v.y or 0)
		else
			for i,v in ipairs(quest_t.target_map) do
				q:SetTargetMap(i, v.map_id, v.x or 0, v.y or 0)
			end
		end
	end
	if quest_t.target_newbie and type(quest_t.target_newbie) == "table" then
		if quest_t.target_newbie.min_lv then
			v = quest_t.target_newbie
			q:SetTargetNewBie(1, v.min_lv, v.max_lv, v.count or 1)
		else
			for i,v in ipairs(quest_t.target_newbie) do
				q:SetTargetNewBie(i, v.min_lv, v.max_lv, v.count or 1)
			end
		end
	end
	if quest_t.encount_npc and type(quest_t.encount_npc) == "table" then
		if quest_t.encount_npc.npc_id then
			v = quest_t.encount_npc
			q:SetEncountNPC(1, v.npc_id, v.map_id or 0, v.count or -1, 
							v.step or math.random(100,300))
		else
			for i,v in ipairs(quest_t.encount_npc) do
				q:SetEncountNPC(i, v.npc_id, v.map_id, v.count or -1, 
								v.step or math.random(100,300))
			end
		end
	end
	if quest_t.target_follower and type(quest_t.target_follower) == "table" then
		if quest_t.target_follower.mob_id then
			v = quest_t.target_follower
			q:SetTargetFollower(1, v.mob_id, v.count or 1)
		else
			for i,v in ipairs(quest_t.target_follower) do
				q:SetTargetFollower(i, v.mob_id, v.count or 1)
			end
		end
	end
	if quest_t.feature and type(quest_t.feature) == "table" then
		for i,v in ipairs(quest_t.feature) do
			if v == "cancel" then q:SetCancel(true)
			elseif v == "nocancel" then q:SetCancel(false)
			elseif v == "failOverTime" then q:SetFailOverTime(true)
			elseif v == "failEscape" then q:SetFailEscape(true)
			elseif v == "failPlayerDie" then q:SetFailPlayerDie(true)
			elseif v == "failTeamDie" then q:SetFailTeamDie(true)
			elseif v == "failFollowerDie" then q:SetFailFlowerDie(true)
			elseif v == "failNpcDie" then q:SetFailNpcDie(true)
			elseif v == "failJoinParty" then q:SetFailJoinParty(true)
			elseif v == "cancelOverTime" then q:SetCancelOverTime(true)
			end
		end
	end
	if quest_t.time_limit then
		q:SetTimeLimit(quest_t.time_limit)
	end
	if quest_t.time_mark then
		questSys:SetTimeMark(quest_t.time_mark)
	end
	questSys:RaiseUpdate()

	return q
end

-- Add quest to all party members
-- quest_t : lua quest table to add
function AddQuestParty(quest_t, addRound)
	if party then
		for i = 0, party:GetCount() - 1 do
			local member = party:GetMember(i)
			AddQuest(quest_t, member, addRound)
		end
	elseif player then
		AddQuest(quest_t, player, addRound)
	end
end

-- Check player fulfill requirement to get the quest
-- quest_t : lua quest table
-- character : character to check. if party, check all members. 
---            if nil, then character is "player"
function QuestFulfill(quest_t, character)
	local allow, err = RequireFulfill(quest_t.req, character)
	if not allow then
		ShowError(string.format("任务限制：%s", err))
	end
end

-- Check player fulfill requirement to get the quest or enter instance
-- req : lua requirement table
-- character : character to check. if party, check all members. 
---            if nil, then character is "player"
function RequireFulfill(req, character)
	if character == nil then
		character = party or player
	end
	if character == nil or req == nil or type(req) ~= "table" then
		return false, "nil"
	end

	local char_t = {}

	if req.partyMin and (party and party:GetCount() or 1) < req.partyMin then
		return false, string.format("队伍人数不足 %d 人", req.partyMin)
	end
	if req.partyMax and (party and party:GetCount() or 1) > req.partyMax then
		return false, string.format("队伍人数多於 %d 人", req.partyMax)
	end

	if character == party then
		for i = 0, party:GetCount() - 1 do
			local member = party:GetMember(i)
			if member then
				table.insert(char_t, member)
			end
		end
	else
		table.insert(char_t, character)
	end
    local guildUID = 0
	for j,character in ipairs(char_t) do
		if req.flag then
			local flags = req.flag
			if type(req) == "table" then
				for i = 1, table.maxn(flags) do
					local f = character:GetQuests():GetFlag(flags[i][1])
					if f == nil or f ~= flags[i][2] then
						return false, "还没达到任务要求"
					end
				end
			else
				local f = character:GetQuests():GetFlag(flags[1])
				if f == nil or f ~= flags[2] then
					return false, "还没达到任务要求"
				end
			end
		end
		if req.sex and character:GetSex() ~= req.sex then
			return false, "性别不乎合"
		end
		if req.job and character:GetJob() ~= req.job then
			return false, "职业不乎合"
		end
		if req.faction and character:GetFaction() ~= req.faction then
			return false, "帮派不乎合"
		end
		if req.minLv and character:GetLevel() < req.minLv then
			return false, string.format("有队伍员不足等级 %d", req.minLv)
		end
		if req.maxLv and character:GetLevel() > req.maxLv then
			return false, string.format("有队伍员大於等级 %d", req.maxLv)
		end
		if req.manorScale then
			if character:GetGuildUid() == 0 then
				return false, "你或队员没有附属任何帮派, 故无法进入此庄园"
			else
				if character:GetGuildManorScale() ~= req.manorScale then
					return false, "你或队员不符合进入此山庄的山庄规模要求"
				end				
				if j == 1 then
					guildUID = character:GetGuildUid()
				else
					if character:GetGuildUid() ~= guildUID then
						return false, "你和队员不属于同一帮派, 无法进入此庄园"
					end
				end
			end
		end		
	end
	return true, ""
end

-- Remove quest from player
-- quest_t : lua quest table to remove
-- character : character to remove quest. if nil, then character is "player"
function RemoveQuest(quest_t, character)
	if not IsProcess() then return false end
	if character == nil then character = player end
	if character == nil then return false end
	local questSys = character:GetQuests()
	if quest_t.time_mark then
		questSys:RemoveTimeMark(quest_t.time_mark)
	end
	return questSys:RemoveQuest(quest_t.quest_id)
end

-- Check if quest is completed
-- quest_t : lua quest table
-- character : character to check. if nil, then character is "player"
function QuestCompleted(quest_t, character)
	if character == nil then character = player end
	if character == nil then return false end
	--print("QuestCompleted", quest_t.char_id, character:GetChar_id(), quest_t.completed)
	if quest_t.char_id == character:GetChar_id() and quest_t.completed ~= nil then
		return quest_t.completed
	end
	local npcId = character:GetScript():GetNpcId()
	local mapId = character:GetMap_id()
	quest_t.char_id = character:GetChar_id()
	quest_t.completed = character:GetQuests():CheckFinished(quest_t.quest_id, npcId, mapId)
	return quest_t.completed
end

-- get instance owner id
-- inst_t : lua instance table
function GetInstanceOwnerId(inst_t)
	if player == nil then return 0 end
	local owner_id
	if inst_t.inst_type == 6 then
		owner_id = player:GetGuildUid()
	elseif  inst_t.inst_type == 8 then
		local relate = player:GetRelationSystem()
		if relate ~= nil then
		    owner_id = relate:GetMarriageId()
		else
			owner_id = 0
		end
	else
		owner_id = player:GetChar_id()
	end
	return owner_id
end

-- create instance
-- inst_t : lua instance table
function CreateInstance(inst_t)
	if inst_t == nil or type(inst_t) ~= "table" then return end
	local tmpScript = script

	instance = LuaI.CInstance_CreateInstance(inst_t.inst_id, 
					inst_t.owner_id, 
		inst_t.inst_type)
	if instance == nil then return end

	if inst_t.lasting then
		instance:SetLasting(inst_t.lasting)
	end
	if inst_t.player_limit then
		instance:SetPlayerLimit(inst_t.player_limit)
	end
	for i, map_id in ipairs(inst_t.maps) do
		instance:AddMap(map_id)
	end
	-- restore script object
	script = tmpScript
	inst_t:OnInit()
end

-- force to destroy instance manually
-- inst_t : lua instance table
function DestroyInstance(inst_t)
	if inst_t == nil or type(inst_t) ~= "table" then return end

	LuaI.CInstance_DestroyInstance(inst_t.inst_id, inst_t.owner_id)
end

-- player can enter instance?
-- inst_t : lua instance table
function CanEnterInstance(inst_t)
	if inst_t == nil or type(inst_t) ~= "table" then return false end
	local map = player:GetMap()
	local allow, err = false, ""
	if map and map:GetInst_id() ~= inst_t.inst_id then
		allow, err = RequireFulfill(inst_t.require)
	end
	if allow and inst_t.CanEnter then
		allow, err = inst_t:CanEnter()
	end
	return allow, err or ""
end

-- player can leave instance?
-- inst_t : lua instance table
function CanLeaveInstance(inst_t)
	if inst_t == nil or type(inst_t) ~= "table" then return false end
	local map = player:GetMap()
	local allow = map and map:GetInst_id() == inst_t.inst_id
	local err = ""
	if allow and inst_t.CanLeave then
		allow, err = inst_t:CanLeave()
	end
	return allow, err or ""
end

-- enter instance
-- inst_t : lua instance table
-- map_id, x, y : jump to instance map location
function EnterInstance(inst_t, map_id, x, y)
	if inst_t == nil or y == nil then
		return false
	end

	player:EnterInstance(inst_t.luafile, inst_t.inst_id, inst_t.owner_id, 
						map_id, x, y)
	return true
end

-- leave instance
-- inst_t : lua instance table
-- map_id, x, y, line_id : jump to normal map location
function LeaveInstance(inst_t, map_id, x, y, line_id)
	if inst_t == nil or y == nil then
		return false
	end

	player:LeaveInstance(inst_t.inst_id, inst_t.owner_id, map_id, x, y, line_id)
	return true
end

function JumpMap(character, map_id, x, y, line_id)
	if character == nil then return end
	if character:GetMap_ownerId() > 0 then
		if line_id == nil then
			line_id = SelectMapLines("请选择要跳往的线：", map_id, false)
		end
		if IsProcess() then
			local map = character:GetMap()
			local inst_id = map and map:GetInst_id() or 0
			local owner_id = map and map:GetOwner_id() or 0
			character:LeaveInstance(inst_id, owner_id, map_id, x, y, line_id)
		end
	else
		if IsProcess() then
			if line_id == nil then line_id = 0 end
			character:JumpMap(map_id, x, y, line_id)
		end
	end
end

-- Find target npc for all party member
function FindTargetNpc(quest_id, index)
	if party then
		for i = 0, party:GetCount() - 1 do
			local member = party:GetMember(i)
			local q = member:GetQuests():GetQuest(quest_id)
			if q then
				q:FindTargetNPC(index)
			end
		end
	elseif player then
		local q = player:GetQuests():GetQuest(quest_id)
		if q then
			q:FindTargetNPC(index)
		end
	end
end

-- Utility function to find lua table name in global space
function FindTableName(t, n)
	for k,v in pairs(getfenv(n)) do
		if v == t then
			return k
		end
	end
	return nil
end

-- Add quest table to system for system processing
-- quest_t : lua quest table
function AddQuestTable(quest_t)
end

-- Add Npc table to system for system processing
-- npc_t : lua Npc table
function AddNpcTable(npc_t)
	if script then
		local tn = FindTableName(npc_t, 3)
		script:SetNpcTable(tn)
		script:SetProcess(false)		-- no process at first
		script:SetAsking(true)			-- show asking question first
		script:SetSaying(true)			-- show saying text
		--print(string.format("Npc %d Talk 1", npc_t.npc_id))
		npc_t:Talk(false)
		script:TalkEnd(true)			-- talking end
		coroutine.yield()				-- wait for client to stop talk
		script:SetProcess(true)			-- process after all finish
		script:SetAsking(false)			-- no asking question shown
		script:SetSaying(false)			-- no saying text
		--print(string.format("Npc %d Talk 2", npc_t.npc_id))
		npc_t:Talk(true)
	elseif npc and npc_t.quests then
		-- read quest hints
		--print("load quest: npc_id", npc:GetNpc_id())
		for i,quest_t in ipairs(npc_t.quests) do
			local req = quest_t.require
			if type(req) == "table" and next(req) then
				local minLv = req.minLv or 0
				local maxLv = req.maxLv or 0
				local sex = req.sex or 0
				local job = req.job or 0
				local faction = req.faction or npc_t.faction or 0
				npc:SetQuestHint(quest_t.quest_id, minLv, maxLv, 
								sex, job, faction)
				if type(req.flag) == "table" then
					if type(req.flag[1]) == "table" then
						for j,v in ipairs(req.flag) do
							if type(v) == "table" and v[2] ~= nil then
								npc:AddHintFlag(quest_t.quest_id, v[1], v[2])
							end
						end
					elseif type(req.flag[2]) == "number" then
						local v = req.flag
						npc:AddHintFlag(quest_t.quest_id, v[1], v[2])
					end
				end
			else
				--npc:SetQuestHint(quest_t.quest_id, 0, 0, 0, 0, 0)
			end
		end
		
	end
end

-- Add Item table to system for system processing
-- item_t : lua item table
function AddItemTable(item_t)
	if script then
		local tn = FindTableName(item_t, 3)
		script:SetProcess(true)			-- process first
		script:SetAsking(true)			-- show asking question first
		script:SetSaying(true)			-- show saying text
		script:SetItemTable(tn)
		if battle == nil then
			coroutine.yield()
		end
	end
end

-- Add Battle table to system for system processing
-- bat_t : lua battle table
function AddBattleTable(bat_t)
	if script then
		local tn = FindTableName(bat_t, 3)
		script:SetProcess(true)			-- process first
		script:SetAsking(true)			-- show asking question first
		script:SetSaying(true)			-- show saying text
		script:SetBatTable(tn)
	end
end

-- Add instance table to system for system processing
-- inst_t : lua instance table
function AddInstTable(inst_t)
	--print("AddInstTable:", inst_t, script)
	if script then
		local tn = FindTableName(inst_t, 3)
		script:SetInstTable(tn)
		if inst_t.owner_id == nil then
			inst_t.owner_id = GetInstanceOwnerId(inst_t)
		end
		if luaFilename then
			inst_t.luafile = luaFilename
		end
		if _inst_preload == nil then			-- execute independantly
			script:SetProcess(true)			-- process first
			script:SetAsking(true)			-- show asking question first
			script:SetSaying(true)			-- show saying text
			coroutine.yield()				-- wait for client 
		end
	end
end

-- text enrichment
function TextEnrich(text, npc_id, character)
	local t = ""
	if type(text) == "table" then
		t = RandTable(text)
	else
		t = text
	end
	local h = string.sub(t, 1, 3)
	if npc_id and h ~= "\#Y[" then
		t = string.format("\#Y[%s]:\#W%s", LuaI.GetNpcName(npc_id), t)
	elseif character and h ~= "\#B[" then
		t = string.format("\#B[%s]:\#W%s", character:GetName(), t)
	end
	return t
end

-- Npc ask player question to choose
-- text : npc name insert at beginning. if begin with "\#Y[", no name insert
--        if it is a table, random select a text in table
-- bleft : image shown at left side? if nil, set to true
-- npc_id : show another npc image
-- bParty : ask to all party member or team member
function NpcAsk(text, bLeft, npc_id, bParty)
	if script == nil then return end
	if bParty == nil then bParty = false end
	if script:IsAsking() then
		if bLeft == nil then bLeft = true end
		if npc_id == nil then npc_id = script:GetNpcId() end
		local t = TextEnrich(text, npc_id)
		script:SetQuestion(t, bLeft, 0, npc_id, bParty)
	else
		script:NextAnswer(bParty)
	end
end

-- Npc ask all party members question and let leader to choose
-- arguments same as NpcAsk()
function NpcAskParty(text, bLeft, npc_id)
	NpcAsk(text, bLeft, npc_id, true)
end

-- Character ask player question to choose
-- character : character that asking question
-- text : npc name insert at beginning. if begin with "\#Y[", no name insert
--        if it is a table, random select a text in table
-- bleft : image shown at left side? if nil, set to true
-- bParty : ask to all party member or team member
function CharAsk(character, text, bLeft, bParty)
	if script == nil or character == nil then return end
	if bParty == nil then bParty = false end
	if script:IsAsking() then
		if bLeft == nil then bLeft = true end
		local t = TextEnrich(text, npc_id)
		script:SetQuestion(t, bLeft, 1, character:GetChar_id(), bParty)
	else
		script:NextAnswer(bParty)
	end
end

-- Character ask all party members question and let leader to choose
-- arguments same as CharAsk()
function CharAskParty(character, text, bLeft)
	CharAsk(character, text, bLeft, true)
end

-- My player ask question to choose
-- arguments same as CharSay()
function IAsk(text, bLeft, bParty)
	if player == nil then return end
	CharAsk(player, text, bLeft, bParty)
end

-- My player ask all party members question and let leader to choose
-- arguments same as ISay()
function IAskParty(text, bLeft)
	if player == nil then return end
	CharAsk(player, text, bLeft, true)
end

-- Ask player for item, partner or pet
-- arguments similiar as NpcAsk()
function ItemAsk(text)
	if script == nil then return end
	if script:IsAsking() then
		local t = TextEnrich(text)
		script:SetQuestion(t, true, 255, 0, false)
	else
		script:NextAnswer(false)
	end
end

-- Npc ask player question for string text
-- text : npc name insert at beginning. if begin with "\#Y[", no name insert
--        if it is a table, random select a text in table
-- defAns : default answer text
-- bleft : image shown at left side? if nil, set to true
-- npc_id : show another npc image
function AskString(text, defAns, bLeft, npc_id, character)
	if script == nil then return end
	if bParty == nil then bParty = false end
	if script:IsAsking() then
		if defAns == nil then defAns = "" end
		if bLeft == nil then bLeft = true end
		if npc_id == nil then npc_id = script:GetNpcId() end
		local askerType = 0
		local askerId = npc_id
		if npc_id == 0 then
			if character then
				askerType = 1
				askerId = character:GetChar_id()
			else
				askerType = 2
			end
		end
		--local t = TextEnrich(text, npc_id, character)
		script:AskString(text, defAns, bLeft, askerType, askerId)
		coroutine.yield()
	end
	return script:GetAnswerString()
end

-- Ask for given item, partner or pet
-- return given success or not
-- text : given message
-- reqTbl : required object list with gtype and gid table
--			gtype: item = 1, partner = 2, pet = 3
--			gid: item = item id, partner = partner mob id, pet = pet id
-- npc_id : npc id for show name
function AskGiven(text, reqTbl, npc_id)
	if script == nil then return false end

	if script:IsAsking() then
		if type(reqTbl) ~= "table" then return false end
		if npc_id == nil then npc_id = script:GetNpcId() end
		script:ClearGiven()
		for i,v in ipairs(reqTbl) do
			if type(v) == "table" then
				script:AskGivenObj(v.gtype or 0, v.gid or 0)
			end
		end
		if script:AskGiven(text, npc_id) then
			coroutine.yield()
			return (script:GetGivenCount() > 0)
		else
			return false
		end
	else
		return (script:GetGivenCount() > 0)
	end
end

-- Remove all given objects in player
function RemoveAllGiven()
	if not IsProcess() then return end
	if script then
		script:RemoveAllGiven()
	end
end

-- Get a given table with gtype, gid and uid
--		gtype: item = 1, partner = 2, pet = 3
--		gid: item = item id, partner = partner mob id, pet = pet id
--		uid: item = item uid, partner = partner id, pet = pet uid
function GetGiven()
	if script then
		local count = script:GetGivenCount()
		if count > 0 then
			local givenTbl
			for i = 0, count - 1 do
				local v = {}
				v.gtype = script:GetGivenType(i)
				v.gid = script:GetGivenID(i)
				v.uid = script:GetGivenUID(i)
				givenTbl.insert(v)
			end
			return givenTbl
		end
	end
	return {}
end

-- Npc say message to player
-- arguments same as NpcAsk()
function NpcSay(text, bLeft, npc_id, bParty)
	if script and script:IsSaying() then
		if bLeft == nil then bLeft = true end
		if npc_id == nil then npc_id = script:GetNpcId() end
		if bParty == nil then bParty = false end
		local t = TextEnrich(text, npc_id)
		if battle then
			battle:BatSay(t, 0, bLeft, npc_id, 255)
		else
			script:Say(t, 0, bLeft, npc_id, bParty)
		end
	end
end

-- Npc say message to player
-- character : character that npc talking to 
-- other arguments similar to NpcAsk()
function NpcSayTo(character, text, bLeft, npc_id)
	if character and script and script:IsSaying() then
		if bLeft == nil then bLeft = true end
		if npc_id == nil then npc_id = script:GetNpcId() end
		local t = TextEnrich(text, npc_id)
		if battle then
			battle:BatSay(t, 0, bLeft, npc_id, character:GetIndex())
		else
			character:GetScript():Say(t, 0, bLeft, npc_id, false)
		end
	end
end

-- Npc say message to all party members
-- arguments same as NpcAsk()
function NpcSayParty(text, bLeft, npc_id)
	NpcSay(text, bLeft, npc_id, true)
end

-- Character say message in player dialog
-- character : character that speaking
-- text : player name insert at beginning. if begin with "\#B[", no name insert
-- text : if it is a table, random select a text
-- bleft : image shown at left side? if nil, set to false
-- bParty : ask to all party member or team member
function CharSay(character, text, bLeft, bParty)
	if script and script:IsSaying() then
		if character == nil then return end
		if bLeft == nil then bLeft = true end
		if bParty == nil or party == nil then bParty = false end
		local t = TextEnrich(text, nil, character)
		if battle then
			battle:BatSay(t, 1, bLeft, character:GetChar_id(), 255)
		else
			script:Say(t, 1, bLeft, character:GetChar_id(), bParty)
		end
	end
end

-- Character say message to all party members
-- arguments same as CharSay()
function CharSayParty(character, text, bLeft)
	if character == nil then return end
	CharSay(character, text, bLeft, true)
end

-- My player say message in dialog
-- arguments same as CharSay()
function ISay(text, bLeft, bParty)
	if player == nil then return end
	CharSay(player, text, bLeft, bParty)
end

-- My player say message to all player's party members
-- arguments same as ISay()
function ISayParty(text, bLeft)
	if player == nil then return end
	CharSay(player, text, bLeft, true)
end

-- Item say message in dialog
-- text : text to say
-- bParty : ask to all party member or team member
function ItemSay(text, bParty)
	if script and script:IsSaying() then
		if bParty == nil then bParty = false end
		local t = TextEnrich(text)
		if battle then
			battle:BatSay(t, 2, true, 0, 255)
		else
			script:Say(t, 2, true, 0, bParty)
		end
	end
end

-- Item say message to all party members
-- arguments same as ItemSay()
function ItemSayParty(text)
	ItemSay(text, true)
end

-- Npc set answer for player to choose in dialog
-- index : answer index for checking
-- text : answer text shown in dialog
function SetAnswer(index, text)
	if script and script:IsAsking() then
		script:AddAnswer(index, text)
	end
end

-- System wait for player answer
-- default : default answer index when no answer provided. 
--           if nil, default answer is the last answer provided
-- timeout : time to answer. if nil, default timeout is 30s
function WaitAnswer(default, timeout)
	if script then
		if script:IsAsking() then
			if default == nil then default = 0 end
			if timeout == nil then timeout = 30 end
			-- pause thread to wait answer
			script:WaitAnswer(default, timeout)
			coroutine.yield()
		else
			script:SetAnswer(default)
		end
	end
end

-- Get answer index after player reply
function GetAnswer(index)
	if script then
		if index == nil then index = 0 end
		return script:GetAnswer(index)
	else
		return nil
	end
end

-- Roll back answer
function BackAnswer(level)
	if script then
		if level then
			script:BackAnswer(level)
		else
			script:BackAnswer()
		end
	end
end

-- Show hint message in player dialog
-- bSystem : is system message? if nil, it show a normal message
function ShowHint(text, character)
	if script and script:IsSaying() then
		if battle then
			if character == nil and user then character = user end
			if character then
				local owner = character:GetOwner()
				if owner then
					character:GetOwner():GetScript():ShowHint(text)
				end
			else
				script:ShowHint(text)
			end
		else
			if character then
				character:GetScript():ShowHint(text)
			else
				script:ShowHint(text)
			end
		end
	end
end

-- Show system message in player chat dialog
function ShowMessage(text, character)
	if script and script:IsSaying() then
		if character then
			character:GetScript():ShowError(text)
		else
			script:ShowError(text)
		end
	end
end

-- Show error message in player chat dialog
function ShowError(text, character)
	if script and script:IsSaying() then
		if battle then
			if character then
				character:ItemFail(text)
			elseif user then
				user:ItemFail(text)
			else
				script:ShowError(text)
			end
		else
			if character then
				character:GetScript():ShowError(text)
			else
				script:ShowError(text)
			end
		end
	end
end

-- log system message to log file
function LogMessage(level, text)
	if level == nil then return end
	if text == nil then
		text = level
		level = 3
	end

	if script then
		script:LogMessage(level, text)
	else
		LuaI.LogMessage(level, text)
	end
end

-- fail quest to all party members
-- quest_t : lua quest table to add
function FailQuest(quest_id, character)
	if quest_id == nil then return end
	if character then
		local q = character:GetQuests():GetQuest(quest_id)
		if q then
			q:SetFail(true)
		end
	elseif party then
		for i = 0, party:GetCount() - 1 do
			local member = party:GetMember(i)
			local q = member:GetQuests():GetQuest(quest_id)
			if q then
				q:SetFail(true)
			end
		end
	elseif player then
		local q = player:GetQuests():GetQuest(quest_id)
		if q then
			q:SetFail(true)
		end
	end
end

-- generate one NPC in normal map or instance map
-- npc_id : NPC id
-- map_id, x, y : NPC location
-- enableAll : NPC can talk by all players?
-- lifetime : NPC life time
-- misc : optional, line id for normal map, instance table for instance map
function GenerateNPC(npc_id, map_id, x, y, enableAll, lifetime, misc)
	if enableAll == nil then enableAll = true end
	if lifetime == nil then lifetime = 10 end
	local line_id = 0
	local owner_id = 0
	if misc then
		if type(misc) == "table" then
			owner_id = GetInstanceOwnerId(misc)
		elseif type(misc) == "number" then
			line_id = misc
		end
	end
	--print("GenerateNPC", npc_id, map_id, x, y, enableAll, lifetime, line_id, owner_id)
	script:GenerateNPC(npc_id, map_id, x, y, enableAll, lifetime, line_id, owner_id)
end

-- Get map lines count
-- map_id : lines count for specified map_id
-- return lines status list
function GetMapLines(map_id)
	if map_id == nil then return end
	local lines = {}
	local counts = {}
	if script then
		script:GetMapLines(map_id)
		coroutine.yield()				-- wait for server return
		--for i,v in ipairs(lines) do
		--	print("GetMapLines", map_id, i, v)
		--end
		table.remove(lines, 1)			-- remove line 0
	end
	return lines, counts
end

-- Select map lines dialog
-- text : asking text as NpcAsk
-- map_id : lines count for specified map_id
-- canCancel : optional, allow cancel selection
-- return selected line id, return 0 if fail or cancel
function SelectMapLines(text, map_id, canCancel)
	if map_id == nil then return 0 end
	statusText = { " (正常)", " (繁忙)", " (满载)" }
	--print("SelectMapLines", text, map_id)
	local lines, counts = GetMapLines(map_id)
	if table.maxn(lines) == 0 then return 0 end
	if npc then
		NpcAsk(text)
	else
		ItemAsk(text)
	end
	-- check least count line and if all line are full or not
	local allFull = true
	local leastLine = 1
	local leastCount = 65535
	for i,v in ipairs(lines) do
		if v > 0 then
			if v ~= 3 then
				allFull = false
			end
			--print("Line", i, v, counts[i+1])
			if counts[i+1] >= 0 and counts[i+1] < leastCount then
				leastLine = i
				leastCount = counts[i+1]
			end
		end
	end
	for i,v in ipairs(lines) do
		if v > 0 then
			SetAnswer(i, "线" .. i .. statusText[v])
		end
	end
	if canCancel then
		SetAnswer(0, "取消选择")
		WaitAnswer(0)
	else
		WaitAnswer(lines[leastLine])
	end

	local ans = GetAnswer()
	if ans ~= 0 and not allFull and lines[ans] == 3 then
		ShowError(string.format("%d线的人数已满，现在将你传送到%d线。",
			ans, leastLine), player)
		ans = leastLine
	end

	return ans
end

-- Get if script is in processing stage
function IsProcess()
	return player ~= nil and player:GetScript():IsProcess()
end

-- Get if script is asking question
function IsAsking()
	return player ~= nil and player:GetScript():IsAsking()
end

-- Get a random object in a table
function RandTable(t)
	return t and t[math.random(table.maxn(t))]
end

-- Calculate distance between 2 locations
function CalcDistance(x1, y1, x2, y2)
	local dx = math.abs(x1 - x2)
	local dy = math.abs(y1 - y2)
    local mn
    if (dx <= dy) then
        mn = dx
    else
        mn = dy
    end
    return (dx + dy - (mn / 2) - (mn / 4) + (mn / 8))
end

-- directional table
directionMap = {
	[0]="SW",[1]="S",[2]="SE",[3]="E",[4]="NE",[5]="N",[6]="NW",[7]="W"
}

-- Calculate direction between 2 locations
-- return direction string "N,NE,E,SE,S,SW,W,NW"
function CalcDirection(x1, y1, x2, y2)
    local dx = math.abs(x1 - x2)
    local dy = math.abs(y1 - y2)
    local atan = 0
    
    if (x1 == x2 and y1 == y2) then
        atan = 0
    elseif (x1 == x2 and y1 < y2) then
        atan = 0
    elseif (x1 < x2 and y1 == y2) then
        atan = math.pi / 2
    elseif (x1 == x2 and y1 > y2) then
        atan = math.pi
    elseif (x1 > x2 and y1 == y2) then
        atan = math.pi / 2 + math.pi
    elseif (x1 < x2 and y1 < y2) then
        atan = math.atan(dx / dy)
    elseif (x1 < x2 and y1 > y2) then
        atan = math.atan(dy / dx) + math.pi / 2
    elseif (x1 > x2 and y1 > y2) then
        atan = math.atan(dx / dy) + math.pi
    else
        atan = math.atan(dy / dx) + math.pi / 2 + math.pi
    end
        
    atan = atan + math.pi / 8
    if (atan > math.pi * 2) then
        atan = atan - math.pi * 2
    end
    atan = atan / (math.pi / 4)
    
    return directionMap[math.floor(atan)]
end

-- Are 2 positions in range r ?
function PosInRange(x1, y1, x2, y2, r)
	local dx = math.abs(x1 - x2)
	local dy = math.abs(y1 - y2)
	return (dx < r or dy < (r * 2))
end

-- Random map location from map tables {map_id, x1, y1, x2, y2}
-- return {map_id, x, y}
function RandMapLoc(map_t)
	if type(map_t) ~= "table" or table.maxn(map_t) == 0 then
		return nil
	end

	local map, x, y, r
	local i = 0
	while i < 100 do	-- prevent dead loop
		r = map_t[math.random(1, table.maxn(map_t))]
		map = LuaI.GetMap(r.map_id)
		r.x = math.random(r.x1, r.x2)
		r.y = math.random(r.y1, r.y2)
		if map and not map:IsOccupied(r.x, r.y) then
			return r
		end
		i = i + 1
	end

	return nil
end

-- Get map Messengers count
-- relation : Messengers count for specified relation
-- return Messengers list
function GetMessengers(char_id, relation)
	if char_id == nil or relation == nil then return end
	local messengers = {}
	if script then
		script:GetMessengers(char_id, relation)
		--print("GetMessengers coroutine.yield()");
		coroutine.yield()				-- wait for server return
	end
	return messengers
end

-- Get map Messengers name count
-- relation : Messengers name count for specified relation
-- return Messengers name list
function GetMessengersName(char_id, relation)
	if char_id == nil or relation == nil then return end
	local messengers = {}
	if script then
		script:GetMessengersName(char_id, relation)
		--print("GetMessengersName coroutine.yield()");
		coroutine.yield()				-- wait for server return
	end
	return messengers
end

-- Get Friendly
-- return Friendly
function GetFriendly(char_id1, char_id2)
	if char_id1 == nil or char_id2 == nil then return end
	local friendly
	if script then
		script:GetFriendly(char_id1, char_id2)
		--print("GetFriendly coroutine.yield()");
		coroutine.yield()				-- wait for server return
	end
	return friendly
end

function IsFriend(char_id1,char_id2)
	if char_id1 == nil or char_id2 == nil then return end
	local isFriend;
	script:IsFriend(char_id1,char_id2)
	--print("IsFriend coroutine.yield()");
	coroutine.yield()				-- wait for server return
	return isFriend
end
function HasGlobalNpcFlag(npc_id,owner_id,flag_id)
	local hasFlag;
	script:HasGlobalNpcFlag(npc_id,owner_id,flag_id);
	coroutine.yield()				-- wait for server return
	return hasFlag
end

function GetGlobalNpcFlag(npc_id,owner_id,flag_id)
	local valueFlag;
	script:GetGlobalNpcFlag(npc_id,owner_id,flag_id);
	coroutine.yield()				-- wait for server return
	return valueFlag
end

function SetGlobalNpcFlag(npc_id,owner_id,flag_id, flag)
	script:SetGlobalNpcFlag(npc_id,owner_id,flag_id,flag);
	coroutine.yield()				-- wait for server return
end

function RemoveGlobalNpcFlag(npc_id,owner_id,flag_id)
	script:RemoveGlobalNpcFlag(npc_id,owner_id,flag_id);
	coroutine.yield()				-- wait for server return
end

function ChangeFriendly(char_id1,char_id2,changed)
	script:ChangeFriendly(char_id1,char_id2,changed);
	coroutine.yield()				-- wait for server return
end

-- GetNpcName(npc_id) - to get Npc name by npc_id
GetNpcName = LuaI.GetNpcName

-- GetMobName(mob_id) - to get Mob name by mob_id
GetMobName = LuaI.GetMobName

-- GetMapName(map_id) - to get Map name by map_id
GetMapName = LuaI.GetMapName

-- GetItemName(item_id) - to get Item name by item_id
GetItemName = LuaI.GetItemName

-- GetMap(map_id) - to get Map object by map_id
GetMap = LuaI.GetMap

-- GetBuffData(buff_id) - to get Buff data by buff_id
GetBuffData = LuaI.GetBuffData

-- SetTempResult(result) - set temp result
SetTempResult = LuaI.SetTempResult

-- GetWorldEvent(event_id) - to get world event by event_id
GetWorldEvent = LuaI.GetWorldEvent

-- GetInstance(inst_id, owner_id) - to get instance by inst_id and owner_id
GetInstance = LuaI.CInstance_GetInstance
