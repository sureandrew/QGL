
function CancelAuto()
	LuaUI.CancelAuto();
end

function TestEntityBase(aniEntity)
	aniEntity:GetEntity():InitChainData();
	aniEntity:GetEntity():SetJumpDataChain(100, 0, 0, 1000, 0, false);
	aniEntity:GetEntity():PushChainData();
end

function PerformSkillAttack(AtkLst, DefLst)
	local Atk = AtkLst:GetEntity();
	Atk:InitChainData();
	Atk:SetAnimIndexChain(4, -1, true, false);
	Atk:PushChainData();
end