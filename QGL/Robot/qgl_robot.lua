-- robot script
qglRobotVer = "1"

math.randomseed(os.time())

function ScriptEnd()
	
end

function MapAction(map, character)
	if map == nil or character == nil then
		return
	end

	local isLeader = not character:HasParty() or character:IsLeader()

	if isLeader then MoveAction(map, character) end
end

function MoveAction(map, character)
	if character:GetAction() ~= LuaI.ST_BATTLE 
	and character:GetAction() ~= LuaI.ST_MONITOR 
	and character:GetAction() ~= LuaI.ST_MOVE then
		map:MoveToRandomGuidePos();
	end
end



