-- simple lua AI script

function Hello()
	print("Lua: Battle Service Hello World")
	LuaI.HelloWorld()
end

-- simple random attack enemy
function AIAction(battle, mob)
	local actorCount = battle:GetActorCount()
	print(string.format("Lua AIAction: mob_id %d, mob index %d" .. 
		", actorCount %d", mob:GetMob_id(), mob:GetIndex(), actorCount))
	local r = math.random(1, actorCount - 1)
	local index = mob:GetIndex()
	for i = 0, actorCount - 1 do
		local actor = battle:GetActor(math.fmod((index + r + i) , actorCount));
		if (actor:GetTeam() ~= mob:GetTeam() and actor:IsValidTarget()) then
			print(string.format("Lua InputCommand: attack target %d", 
				actor:GetIndex()))
			mob:InputCommand(LuaI.BATCMD_ATTACK, actor:GetIndex(), 0, 0)
			break
		end
	end
end

function LoseMind(battle, char)
	local actorCount = battle:GetActorCount();
	local r = math.random(1, actorCount - 1);
	local index = char:GetIndex();
	for i = 0, actorCount - 1 do
		local actor = battle:GetActor(math.fmod((index + r + i) , actorCount));
		if (actor:IsValidTarget()) then	
			local randomSkill = char:GetRandomSkill();
			if randomSkill == 0 then
				char:InputCommand(LuaI.BATCMD_ATTACK, actor:GetIndex(), 0, 0);
			else
				char:InputCommand(LuaI.BATCMD_SKILL, actor:GetIndex(), randomSkill, 0);
			end
			break
		end
	end
	
end