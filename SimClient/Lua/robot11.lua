-- robot script
robotVer = "11"
robotInterval = 2000
cmdList = {}

OneSubQuestRunning = false
SubQTStartTalk = false
SubQuestDone = 0
RepeatNPCTalk = false
QTT_ID = 0

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
	--print("Lua MapAction", map, character)

	if map == nil or character == nil then
		print("Lua MapAction Error: nil map or character")
		return
	end

	-- 1%: disconnect at any time
	local r = math.random(0, 99)
	if r < 1 then
		DoCommand("disconnect")
		OneSubQuestRunning = false
		SubQTStartTalk = false
	    RepeatNPCTalk = false
        QTT_ID = 0
        return
	end

	local isLeader = not character:HasParty() or character:IsLeader()
	if isLeader then MoveAction(map, character) end
	if isLeader then PartyAction(map, character) end
	ItemAction(map, character)
	SkillAction(map, character)
	ChatAction(map, character)
	FriendAction(map, character)
	PartnerAction(map, character)
	ShopAction(map, character)
	PetAction(map, character)
	MiscAction(map, character)
--  QuestAction()
	GiveAction(map, character)
	TradeRequestAction(map, character)
	WaitConfirm()
--  CheckExistingQuestTask()
--  NPCQuestTalkStart(map)
	NPCTalking(map, character)
	GuildAction()
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

function GuildAction()
	local cmd = nil
	if LuaI.CheckGuildMembership() == false then -- if still not belongs to any guild
		local guildrand = math.random(0, 99)
		if guildrand < 2 then -- 2% for create a new guild
			cmd = "cheat guild create" -- create your own guild
		elseif guildrand < 4 then --  2% for joining the existing guild
			local totalGuildNumber = LuaI.GetTotalGuildNumber()
			if totalGuildNumber > 0 then
				totalGuildNumber = totalGuildNumber - 1
				cmd = "guild apply " .. math.random(0, totalGuildNumber) 
			else
				cmd = "cheat guild apply" -- apply for existing guild as a member
			end
		end	  
	else
		local r = math.random(0, 99)
		if r < 2 then -- 2% for handling the membership application
			if LuaI.GetTotalGuildApplicantNumber() > 0 then
				local applyRand = math.random(0, 99)
				if applyRand < 90 then  -- 90% approval the membership application
					local totalApply =  LuaI.GetTotalGuildApplicantNumber() - 1
					cmd = "guild addmember " .. math.random(0, totalApply)
				else
					local totalApply =  LuaI.GetTotalGuildApplicantNumber() - 1
					cmd = "guild rejectmember " .. math.random(0, totalApply)
				end	
			end
		elseif r < 4 then -- 2% for kicking out member
			local totalMember = LuaI.GetTotalGuildMemberNumber() - 1    
			cmd = "guild kickmember " .. math.random(0, totalMember)
		elseif r < 7 then -- 3% for changing member position
			local totalMember = LuaI.GetTotalGuildMemberNumber() - 1    
			cmd = "guild changepost " .. math.random(0, totalMember) .. " " .. math.random(1, 10)			  
		elseif r < 8 then -- 1% for leaving your own guild
			cmd = "guild leave"
		elseif r < 10 then -- 16% for writing down your own guild aim
			local lenghtRand = math.random(1, 20)
			local msg = "No aim is good aim "
			cmd = "guild aim "
			for i = 1, lenghtRand do
				cmd = string.format("%s%s", cmd, msg)
			end
		elseif r < 12 then -- 18% for writing down your own guild rule
			local lenghtRand = math.random(1, 20)
			local msg = "No rule is good rule "
			cmd = "guild rule "
			for i = 1, lenghtRand do
				cmd = string.format("%s%s", cmd, msg)
			end			
		end 
	end
	if cmd then DoCommand(cmd) end
end

function RunQuest()
   	
	local temp = LuaI.GetQuestSys();
	if temp then
		local id = temp:GetRandomQuestID();
		if temp:CheckQuestTaskInProcess(id) == false then
		    dofile("quest.lua")
		    local q_t = GetQuestFormat(id)
		    if q_t ~= 500 then
			    if temp:AddQuestTask(id, q_t) then
				    --ResetQuestData()
				    if GetQuestFormat(id) == Main_Quest then
					   CurrentQuest = id
				    end
			    end
		    end
		end
	end
end



function CheatMobQuest(MobID, MAX)

	local cmd = "cheat battle win 1 " .. MobID .. "x" .. MAX 
	DoCommand(cmd)
	
end

function CheatNPCFindQuest(Target_ID, MAP_ID)

	local cmd = "cheat npc find " .. Target_ID .. " " .. MAP_ID
	DoCommand(cmd)
	
end

function CheatItemAddQuest(Target_ID, MAX)

	local cmd = "cheat item add " .. Target_ID .. "x" .. MAX
	DoCommand(cmd)
	

end

function CheatPartnerAddQuest(Target_ID, lineID)

	local cmd = "cheat partner add " .. Target_ID
	DoCommand(cmd)
    local temp = LuaI.GetQuestSys();
    if temp then
	   	local mapID = temp:GetQuestJumpMap(CurrentQuest)
		local mapX = temp:GetQuestJumpXCor(CurrentQuest)
		local mapY = temp:GetQuestJumpYCor(CurrentQuest)
        cmd = "cheat jump " .. mapID .. " " .. lineID .. " ".. mapX .. " " .. mapY
		DoCommand(cmd)
		temp:SetQuestTaskCheated(CurrentQuest);
	end
	
end

function CheatJumpQuest(QT_ID, lineID)

    local temp = LuaI.GetQuestSys();
    if temp then
	    local npcID = temp:GetFinishNPCID(QT_ID)
	   	local mapID = temp:GetQuestJumpMap(QT_ID)
		local mapX = temp:GetQuestJumpXCor(QT_ID)
		local mapY = temp:GetQuestJumpYCor(QT_ID)
        local cmd = "cheat jump " .. mapID .. " " .. lineID .. " ".. mapX .. " " .. mapY
        DoCommand(cmd)
	end

end

function CheckExistingQuestTask()

	local temp = LuaI.GetQuestSys();
	if temp then
	    if temp:IsOutstandingQuestTask() then
			temp:CheckAllQuestRemainingTasks()
   	    end
	end
end


function TalkCondition(QT_ID, lineID)

    local temp = LuaI.GetQuestSys();
    if temp then
	    local npcID = temp:GetFinishNPCID(QT_ID)
	   	local mapID = temp:GetQuestJumpMap(QT_ID)
		local mapX = temp:GetQuestJumpXCor(QT_ID)
		local mapY = temp:GetQuestJumpYCor(QT_ID)
	
    
		dofile("quest.lua")
	    local con = GetQuestTalkCondition(QT_ID)
	    if con ==  No_Condition then
			-- then just jump to finish NPC
			local cmd = "cheat jump " .. mapID .. " " .. lineID .. " ".. mapX .. " " .. mapY
			DoCommand(cmd)
			temp:SetQuestTaskCheated(QT_ID);
	    elseif con == Levelup_Condition then
	        -- cheat level up and then jump to finish NPC
	        local levelVal = GetQuestConditionValue(QT_ID) + 5
	        local cmd = "cheat level " .. levelVal
	        DoCommand(cmd)
	        cmd = "cheat jump " .. mapID .. " " .. lineID .. " ".. mapX .. " " .. mapY
	        DoCommand(cmd)
	        temp:SetQuestTaskCheated(QT_ID);
		elseif con == Money_Condition then
		    -- cheat money and then jump to finish NPC
		    local levelVal = GetQuestConditionValue(QT_ID) + 1000
		    local cmd = "cheat money " .. levelVal
		    DoCommand(cmd)
		    cmd = "cheat jump " .. mapID .. " " .. lineID .. " ".. mapX .. " " .. mapY
	        DoCommand(cmd)
	        temp:SetQuestTaskCheated(QT_ID);
		elseif con == Party_Condition then
			-- cheat party and then jump to finsih NPC
	        local cmd = "party create"
	        DoCommand(cmd)
	        cmd = "cheat jump " .. mapID .. " " .. lineID .. " ".. mapX .. " " .. mapY
	        DoCommand(cmd)
	        temp:SetQuestTaskCheated(QT_ID);	
		elseif con == Quest_Condition then
		    -- check if all sub-quests are done
		    local subValue = GetQuestConditionValue(QT_ID)
 		    if (SubQuestDone < subValue) then  -- if not all subquests are finsihed 
			   if (OneSubQuestRunning == false) then
				   if OneSubQuestRunning then
				   		print("One sub quest is running.")
				   else	
				   		print("No sub quest is running.")
				   end 	
				   GetSubQuest(QT_ID, lineID)
			   end
			else
				if OneSubQuestRunning == false then
				    local cmd = "cheat jump " .. mapID .. " " .. lineID .. " ".. mapX .. " " .. mapY  -- jump to finish NPC of MAIN QUEST
	   			    print("Number of SubQuest done: ", SubQuestDone)	
				    print("Total SubQuests: ", subValue)	
				    SubQuestDone = 0
				    print("Subquest number is reset to 0.")	
				    DoCommand(cmd)
				    temp:SetQuestTaskCheated(QT_ID);	
			    end
			end
		end
	end
        
end

function GetSubQuest(QT_ID, LID)

	if OneSubQuestRunning == false then
	     dofile("quest.lua")		
	     local mapID = GetQuestBeginNPCMapID(QT_ID)
	     local x = GetQuestBeginNPCPositionX(QT_ID)
	     local y = GetQuestBeginNPCPositionY(QT_ID)
	     -- jump to subQuest begin NPC
	     local cmd = "cheat jump " .. mapID .. " " .. LID .. " ".. x .. " " .. y
		 DoCommand(cmd)		 	     
         SubQTStartTalk = true
         OneSubQuestRunning = true	
	end
	


end



function NPCQuestTalkStart(map)

	if RepeatNPCTalk then
	
		local temp = LuaI.GetQuestSys();
  		local script = LuaI.GetScript();
		if script:GetTalkStatus() == true then
			RepeatNPCTalk = false
		else
			if temp and script then
				local npcID = 0
				if SubQTStartTalk == true then
			         -- talk to the npc with ID provided as a "begin_npc" in MainQuest
			         dofile("quest.lua")
			         local idd = GetQuestBeginNPC(CurrentQuest)
			         if map then
			            npcID = map:FindNpcUID(idd)
					 end
				else
					 npcID = temp:GetFinishNPCID(QTT_ID)
				end
				if (npcID ~= 0) then
					local cmd = "npctalk start " .. npcID
					QuestTalkOptionIndex = 1;
					DoCommand(cmd)

				end
			end
		end
	end 
end

function QuestStartRepeatNPCTalk(qtID)

	QTT_ID = qtID
	RepeatNPCTalk = true
	
end


function NPCQuestTalkOption(QT_ID)

	dofile("quest.lua")
	local option = 0
	if SubQTStartTalk == true then
		option = GetQuestBeginNPCTalkOption(CurrentQuest, QuestTalkOptionIndex)
	else
		option = GetQuestTalkOption(QT_ID, QuestTalkOptionIndex)
	end
	
	if option ~= 800 then
		local cmd = "npctalk " .. option
		QuestTalkOptionIndex = QuestTalkOptionIndex + 1
		DoCommand(cmd)
	end
end

function CheckSubQuestEnd(QT_ID)
   -- to check if the quest which is done is subquest or not
   if SubQTStartTalk then -- if Simclient call this just because it just have talked to begin NPC, then 
   		SubQTStartTalk = false
   else
   		if GetQuestFormat(QT_ID) == Sub_Quest then -- otherwise, SimClient just talked to finish NPC of MainQuest or SubQuest
   			OneSubQuestRunning = false
			SubQuestDone = SubQuestDone + 1   	
   		end
   end
 
end


function CheckGetSubQuestEnd()

   if SubQTStartTalk then -- if Simclient call this just because it just have talked to begin NPC, then
   		SubQTStartTalk = false
   end
end




function ResetQuestData()		
    SubQTStartTalk = true

--	QuestID = 0
--	QuestType = 10
	QuestTalkOptionIndex = 1

	
	
end

function PetAction(map, character)

	local r = math.random(0, 99)
	local cmd = nil
	local petIndex = math.random(0, 1)
	
 
	if r < 8 then -- 8% to use
		cmd = "pet use " .. petIndex
	elseif r < 16 then -- 8% to standby
		cmd = "pet standby " .. petIndex
	elseif r < 17 then -- 1% to remove
		cmd = "pet remove " .. petIndex
	elseif r < 20 then -- 3% to change name
		cmd = "pet name_chg " .. petIndex .. " abc"
	end

	if cmd then DoCommand(cmd) end		

end


function ShopAction(map, character)

	local r = math.random(0, 99)
	if r < 6 then  --%6 for buying in onlineShop
    	OpenShop()
	end

end

function CloseShop()

  local cmd = nil
  cmd = "shop close"
  DoCommand(cmd)

end

function OpenShop()

  local cmd = nil
  cmd = "shop open"
  DoCommand(cmd)

end


function BuyItem()

    local cmd = nil

--	local chargeShop = LuaI.GetShop();
--	if chargeShop then
--		local totalItem = chargShop:GetTotalShopItem();
        local totalItem = LuaI.GetShopItemCount();
		totalItem = totalItem - 1
		local itemRan = math.random(0, totalItem)
  		local stack = 1
		local stackRan = math.random(1, 13)
		if (stackRan > 10) and (stackRan < 13) then
	    	stack = 2
		elseif stackRan == 13 then
	    	stack = 3
	    end
	    cmd = "shop buy " .. itemRan .. " " .. stack
--	end
 	LuaI.Execute(cmd)

end

function QuestAction()
	local r = math.random(0, 99)
	local cmd = nil
	local tp = LuaI.GetQuestSys();
	if tp then
		print("QuestAction 1: quest system object intact")		
		if (r < 2) and (tp:IsOutstandingQuestTask() == false) then -- 2% for quest cancel
		    cmd = "quest cancel"
		    print("QuestAction 2a: cancel quest")	
		elseif (r < 30) or (OneSubQuestRunning) then-- 28% for completing the quest while 100% if subquests is running
		    print("QuestAction 2b: run quest")
			local script = LuaI.GetScript();
	        if script then
	        	print("QuestAction 3: script object intact")
			    if (script:GetTalkStatus() == false) then
					RunQuest()
					print("QuestAction 4: run quest")	
				end
			end
		end
	end
	if cmd then DoCommand(cmd) end
end



function NPCTalking(map, character)
    
	local temp = LuaI.GetQuestSys();
	if temp then

		if temp:IsOutstandingQuestTask() == false then
			local r = math.random(0, 99)
			if r < 10 then -- %10 for NPC talking
				local script = LuaI.GetScript();
				if script then
					if script:GetTalkStatus() == false then
						local cmd = "npctalk start"  -- randomly talk to NPC
						DoCommand(cmd)
					end
				end
			end
		end
	
	end
end


function NPCTalkOption(count)
	if count > 0 then
		local cmd = nil
		count = count - 1
		cmd = "npctalk " .. math.random(0, count)
		DoCommand(cmd)
	end

end

function NPCShopAction(count)
	if count > 0 then
		count = count - 1
		local buyItem = math.random(0, count)
		local cmd = nil
		local r = math.random(0, 99)
		if r < 70 then
			cmd = "npcshop buy " .. buyItem .. " " .. math.random(1, 3)		
		elseif r < 72 then
			cmd = "npcshop sell " .. math.random(0, 10)
		end
		if cmd then LuaI.Execute(cmd) end
	end

end

function FriendAction(map, character)
	local r = math.random(0, 99)
	local cmd = nil
	-- 3%: addfriend
	if r < 3 then
       local char2 = map:GetNearCharacter(character:GetCurPosX(),
	                	                  character:GetCurPosY(), 500, 0)
       if char2 then
	      cmd = "friend add_nearby " .. char2:GetChar_id() .. " 3 jj\n"
	   end

	elseif r < 6 then -- 3%: delete a friend
		cmd = "friend delete 0"
	elseif r < 12 then -- 6%: send message to a friend
		cmd = "friend send_msg 0 private This is a test message!!!"
		
	end
	
	if cmd then DoCommand(cmd) end
end



	
function GiveAction(map, character)
	local r = math.random(0, 99)
	local cmd = nil
	-- first check if anybody nearby
	local char2 = map:GetNearCharacter(character:GetCurPosX(),
									   character:GetCurPosY(), 500, 0)
	if char2 then -- if a person is nearby, carry out the "give" command
		if r < 3 then -- 3%: give money
		    local r1 = math.random(1,10 )
		    local money = (r1/100) * character:GetMoney()
            cmd = "give money " .. money .. " " .. char2:GetChar_id()
    	elseif r < 5 then -- 2%: give partner
			cmd = "give partner " .. math.random(0, 1) .. " " .. char2:GetChar_id()
		elseif r < 7 then -- 2%: give pet
 		    cmd = "give pet " .. math.random(0, 1) .. " " .. char2:GetChar_id()
 	   	elseif r < 12 then -- 5%: give item
     	    cmd = "give item 555 " .. char2:GetChar_id()
    	end
	end
  	
	if cmd then DoCommand(cmd) end
  	
end



function TradeRequestAction(map, character)
 	local r = math.random(0, 99)
	local cmd = nil
	
	local char2 = map:GetNearCharacter(character:GetCurPosX(),
									   character:GetCurPosY(), 800, 0)
									   
									   
	if (char2) and (LuaI.TradeInProcess() == false) then
		if LuaI.CheckBattle() == false then
			if r < 50 then  --- %5 for trading
		    cmd = "trade request " .. char2:GetChar_id()
			end
		end
	end
	
	if cmd then
	DoCommand(cmd)
	end
	
end

function TradeAcceptAction()

    local cmd = "trade accept"
	if cmd then LuaI.Execute(cmd) end
end

function TradeAction(itemID, petID, partnerID)

	TradeAdd(itemID, petID, partnerID)
	TradeRemove(itemID, petID, partnerID)
	TradeLock()
	ExecCommand()
end


function TradeLock()

    local cmd = "trade lock"
    if cmd then DoCommand(cmd) end

end

function WaitConfirm()

	local cmd = nil
	if (LuaI.OppTradeLock() == true)and(LuaI.TradeInProcess() == true)and(LuaI.SelfTradeLock() == true) then
		cmd = "trade confirm"
	end

	if cmd then DoCommand(cmd) end
end

function TradeRemove(itemID, petID, partnerID)
   	local r = math.random(0, 99)
	local cmd = nil

	cmd = "trade remove"
	local pet = " pet 0"
	local item = " item 0"
	local partner = " partner 0"

	local petChance = 0
	local itemChance = 0
	local partnerChance = 0
	for i = 1,3 do
		local addRandom = math.random(1, 3 * i )
		if addRandom < 2 then -- a random draw: 1/3(chance) for removing 1st item, 1/6 for 2nd item, 1/9 for 3rd item
		    local typeRandom = math.random(0, 2) -- in each item removal, another lucky draw to determine if remove pet, item or partner
		    if typeRandom == (0 - petChance) then
		        cmd = string.format("%s%s", cmd, pet)
				petChance = 1 -- change to 1, stand no chance for next draw
		    elseif typeRandom == (1 - itemChance) then
		        cmd = string.format("%s%s", cmd, item)
		        itemChance = 2 -- change to 2, stand no chance for next draw
		    elseif typeRandom == (2 - partnerChance) then
		        cmd = string.format("%s%s", cmd, partner)
		        partnerChance = 3 -- change to 3, stand no chance for next draw
			else -- if no items are selected by random draw above, select any "unselected item" below DIRECLY.
			    if petChance == 0 then
			       cmd = string.format("%s%s", cmd, pet)
				   petChance = 1
				elseif itemChance == 0 then
			       cmd = string.format("%s%s", cmd, item)
       			   itemChance = 2
				elseif partnerChance == 0 then
			       cmd = string.format("%s%s", cmd, partner)
       			   partnerChance = 3
				end
			end
		end
	end
    if cmd then DoCommand(cmd) end
end

function TradeAdd(itemID, petID, partnerID)
   	local r = math.random(0, 99)
	local cmd = nil
	
	cmd = "trade add"
	local pet = " pet " .. petID .. " " .. math.random(0, 2)
	local item = " item " .. itemID .. " " .. math.random(0, 5)
	local partner = " partner " .. partnerID .. " " ..  math.random(0, 2)
--	local value = (math.random(1, 10)/100) * character:GetMoney()
    local value = 10
	local money = " money " .. value .. " 3" 

	
	local petChance = 0
	local itemChance = 0
	local partnerChance = 0
	local moneyChance = 0

	if itemID == 100 then
	   itemChance = 2
	end
	if petID == 0 then
    	petChance = 1
	end
	if partnerID == 0 then
    	partnerChance = 3
	end
	
	for i = 1,4 do
		local addRandom = math.random(1, i)
		if addRandom < 2 then -- a random draw: 100%(chance) for adding 1st item, 50% for adding 2nd item, 30% for adding 3rd item
		    local typeRandom = math.random(0, 3) -- in each item addition, another lucky draw to determine if add pet, item or partner
		    if typeRandom == (0 - petChance) then
		        cmd = string.format("%s%s", cmd, pet)
				petChance = 1 -- change to 1, stand no chance for next draw
		    elseif typeRandom == (1 - itemChance) then
		        cmd = string.format("%s%s", cmd, item)
		        itemChance = 2 -- change to 2, stand no chance for next draw
		    elseif typeRandom == (2 - partnerChance) then
		        cmd = string.format("%s%s", cmd, partner)
		        partnerChance = 3 -- change to 3, stand no chance for next draw
		    elseif typeRandom == (3 - moneyChance) then
		    	cmd = string.format("%s%s", cmd, money)
		    	moneyChance = 4 -- change to 4, stand no chance for next draw
			else -- if no items are selected by random draw above, select any "unselected item" below DIRECLY.
			    if petChance == 0 then
			       cmd = string.format("%s%s", cmd, pet)
				   petChance = 1
				elseif itemChance == 0 then
			       cmd = string.format("%s%s", cmd, item)
       			   itemChance = 2
				elseif partnerChance == 0 then
			       cmd = string.format("%s%s", cmd, partner)
       			   partnerChance = 3
       			elseif moneyChance == 0 then
       			   cmd = string.format("%s%s", cmd, money)
				   moneyChance = 4 
				end
			end
		end
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
		local bag = LuaI.GetItemBag()
		if bag then
			local item_id = bag:RandItemId()
			if item_id ~= 0 then
				--print("Lua robot: add item " .. item_id)
				cmd = "cheat item add " .. item_id
			end
		end

	--  2%: drop item
	elseif r < 16 then
		local bag = LuaI.GetItemBag()
		local r2 = math.random(0, 23)
		for i = 0, 23 do
			local item = bag:GetItem((r2 + i) % 23)
			if item then
				cmd = "drop " .. item:GetBagIndex()
				break
			end
		end
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
	
	-- 3%: add attribute
	elseif r < 29 then	
		PartnerAddAttr()
 	end
 
	if cmd then DoCommand(cmd) end
end

function PartnerAddAttr()

	local cmd = nil
	local attrChg = nil
		
	cmd = "partner " .. math.random(0, 2) .. " " .. "attr_chg"
	for i = 1,5 do
		local addRandom = math.random(1, i)
		if addRandom < 3 then -- a random draw: 100%(chance) for adding 1st & 2nd attr, 2/3 for 3rd attr and so on
		    local typeRandom = math.random(1, 5) -- in each attr, another lucky draw to determine the type
		    if typeRandom == 1 then
		    	attrChg = " +STA " .. math.random(1, 5)
		    	cmd = string.format("%s%s", cmd, attrChg)	
			elseif typeRandom == 2 then
				attrChg = " +STR " .. math.random(1, 5)
				cmd = string.format("%s%s", cmd, attrChg)	
			elseif typeRandom == 3 then
				attrChg = " +SPR " .. math.random(1, 5)
				cmd = string.format("%s%s", cmd, attrChg)	
			elseif typeRandom == 4 then
				attrChg = " +CON " .. math.random(1, 5)
				cmd = string.format("%s%s", cmd, attrChg)	
			elseif typeRandom == 5 then
				attrChg = " +AGI " .. math.random(1, 5)
				cmd = string.format("%s%s", cmd, attrChg)	
			end
		end
	end								

	if cmd then DoCommand(cmd) end					

end

function StopShopOverHawker()
	local cmd = "hawker stop_shop"
    LuaI.Execute(cmd)
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
		-- the initialization of quest values before disconnect --
		OneSubQuestRunning = false
		SubQTStartTalk = false
	    RepeatNPCTalk = false
        QTT_ID = 0
        ----------------------------------------------------------
		cmd = "disconnect"

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

function BuyHawkerItem(maxItem, maxPet, maxPartner)
	local r = math.random(0, 2)
	local cmd = nil
	if r == 0 then
     	cmd = "hawker buy 0 " .. math.random(0, maxItem) .. " 1"
   	elseif r == 1 then
		cmd = "hawker buy 1 " .. math.random(0, maxPartner) .. " 1"
 	elseif r == 2 then
		cmd = "hawker buy 2 " .. math.random(0, maxPet) .. " 1"
	end
	
    if cmd then LuaI.Execute(cmd) end
end

