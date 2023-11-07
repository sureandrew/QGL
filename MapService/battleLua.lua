function speedOrder(char)
	local speedOrder = char:GetSPD() * math.random(9, 11) / 10;
	LuaI.SetTempResult(speedOrder);
end

function CalcDamage(Atk, Def)
	
	local dam = 0;
	if Atk:GetTeam() == Def:GetTeam() then
		if Atk:HasBuff(51) then
			local num = math.random(1, 100);
			if Atk:GetATK() * 0.8 > Def:GetDEF() and num <= 5 then
				dam = (Atk:GetATK() - Def:GetDEF() / 2) * math.random(900, 1100) / 1000;
			else
				dam = (Atk:GetATK() - Def:GetDEF()) * math.random(900, 1100) / 1000;
			end
		end			
	else
		local num = math.random(1, 100);
		if Atk:GetATK() * 0.8 > Def:GetDEF() and num <= 5 then
			dam = (Atk:GetATK() - Def:GetDEF() / 2) * math.random(900, 1100) / 1000;
		else
			dam = (Atk:GetATK() - Def:GetDEF()) * math.random(900, 1100) / 1000;
		end
	end
	
	if Atk:IsMob() == true and Atk:IsBoss() == true then
		if dam <= Atk:GetLevel() then
			dam = math.random(Atk:GetLevel(), Atk:GetLevel() * 2 + 8);
		--elseif dam >= Atk:GetLevel() * 8 then
			--dam = math.random(Atk:GetLevel() * 6 - 5, Atk:GetLevel() * 10);
		end
	elseif Atk:IsMob() == true and Atk:IsParnter() == false then
		if dam <= Atk:GetLevel() then
			dam = math.random(Atk:GetLevel(), Atk:GetLevel() * 2 + 8);
		--elseif dam >= Atk:GetLevel() * 5 then
			--dam = math.random(Atk:GetLevel() * 4 - 3, Atk:GetLevel() * 6 + 4);
		end
	end
	
	if dam <= 1 then
		dam = math.random(1, 10);
		if Atk:GetTeam() == Def:GetTeam() then
			dam = 1;
		end
	end
	LuaI.SetTempResult(dam);
end

function ComboAttack(list, Def)--合击概率
	local dam = 0;
	
	local curr = list;
	local count = 0;
	while curr ~= nil do
		CalcDamage(curr:GetCurrActor(), Def);
		dam = dam + LuaI.GetTempResult();
		curr = curr:GetNext();
		count = count + 1;
	end
	
	dam = dam * (1 + 0.1 *  count)
	LuaI.SetTempResult(dam);
end

function CanEscape(actor)--逃跑几率设定（宠物救主技能）
	local rate1 = 70;
	if math.random(1, 100) <= rate1 then
		LuaI.SetTempResult(1);	
	else
		local pet = actor:GetUsingPet();
		if pet ~= nil then
			if pet:HasSkill(7006) then
			local rate2 = 50;
				if math.random(1, 100) <= rate2 then
					LuaI.SetTempResult(1);
					actor:PetActive();
				else
					LuaI.SetTempResult(0);
				end	
			end
		else	
			LuaI.SetTempResult(0);
		end
	end
end

function PetProtectActive(actor)-- 宠物护主技能设定
	local pet = actor:GetUsingPet();
	if pet ~= nil then
		if pet:HasSkill(7007) then
		local rate3 = 10;
			if math.random(1, 100) <= rate3 then
				LuaI.SetTempResult(1);
				actor:PetActive();
			else
				LuaI.SetTempResult(0);
			end
		end
	else
		LuaI.SetTempResult(0);
	end
end

function CanHit(Atk, Def)--战斗命中率设定
	local rate = 1.5 * Atk:GetHIT() / (Atk:GetHIT() + 1.2 * Def:GetEVA()) * 100;
	
	if rate < 35 then
		rate = 35;
	elseif rate > 95 then
		rate = 95;
	end
	
	if Atk:IsMob() == true and Atk:IsBoss() == true then
		rate = (Atk:GetHIT() * 1.5 / (Atk:GetHIT() + 1.2 * Def:GetEVA())) * 100 + 10;
		if rate < 70 then
			rate = 70;
		end
	elseif Atk:IsMob() == true and Atk:IsParnter() == false then
		rate = (Atk:GetHIT() * 1.5 / (Atk:GetHIT() + 1.2 * Def:GetEVA())) * 100 + 5;
		if rate < 55 then
			rate = 55;
		end	
	end
	
	if Atk:HasBuff(42) then
		rate = 100;
	end
	
	if math.random(1, 100) <= rate then
		LuaI.SetTempResult(1);	
	else
		LuaI.SetTempResult(0);
	end
	
end

function CanCritical(Atk, Def)--爆击率设定
	local rate = 5;
	if Atk:GetHP() < Atk:GetMaxHP() / 4 then
		rate = 10;
	end
	if math.random(1, 100) <= rate then
		LuaI.SetTempResult(1);	
	else
		LuaI.SetTempResult(0);
	end
end

function playerExpGain(actor, killCount, npcLevel, npcExp, teamCount, mapLevel)--玩家获取经验设定（角色练功丸）
	local teamCountModify = 1 - (teamCount - 1) * 0.1;
	local lv = actor:GetLevel();
	local lvDiff = math.abs(lv - mapLevel);
	local lvDiffModify = 0;
	if lvDiff <= 2 then
	    lvDiffModify = 1;
	elseif lvDiff <= 4 then
	    lvDiffModify = 0.9; 
	elseif lvDiff <= 6 then
	    lvDiffModify = 0.75;
	elseif lvDiff <= 8 then
	    lvDiffModify = 0.55;
	elseif lvDiff <= 10 then
	    lvDiffModify = 0.3	;
	elseif lvDiff <= 30 then
	    lvDiffModify = 0.3 - lvDiff / 100 ; 
	end		
	--双倍经验
	local DoubleModify = 1;
	if actor:HasBuff(218) and actor:HasBuff(254) then
		DoubleModify = 4;
	elseif actor:HasBuff(218) or actor:HasBuff(254) then
		DoubleModify = 2;
	end	
	--5%加成
	local BattleModify = 1;
	if actor:HasBuff(242) then
		BattleModify = 1.05;
	end	
	--队长加成10%
	local leadModify = 0 ;
	if actor:IsLeader() then
	    leadModify = 0.1 ;
	end
	    
    --玩家获得经验	
	expGain = math.floor(npcExp * (4 * npcLevel + 10) / (4 * mapLevel + 14) * killCount * teamCountModify * lvDiffModify * DoubleModify * BattleModify) + 1;
	LuaI.SetTempResult(expGain);
	--职业加成
	local jobCount = GetPartyDifferentClass(actor );
	if jobCount == 3 then 
    	actor: SetBoundsExp(expGain * (0.02+leadModify));
	elseif jobCount == 4 then 
    	actor: SetBoundsExp(expGain * (0.1+leadModify));
	elseif teamCount >= 3 then
	    actor: SetBoundsExp(expGain * leadModify);
	end
end

function partnerExpGain(actor, owner, killCount, npcLevel, npcExp, teamCount, mapLevel)--同伴获取经验设定（同伴练功丸）
	local teamCountModify = 1 - (teamCount - 1) * 0.1
	local lv = actor:GetLevel();
	local lvDiff = math.abs(lv - mapLevel);
	local lvDiffModify = 0;
	if lvDiff <= 4 then
	    lvDiffModify = 1;
	elseif lvDiff <= 6 then
	    lvDiffModify = 0.9; 
	elseif lvDiff <= 8 then
	    lvDiffModify = 0.75;
	elseif lvDiff <= 10 then
	    lvDiffModify = 0.55;
	elseif lvDiff <= 12 then
	    lvDiffModify = 0.3	;
	elseif lvDiff <= 30 then
	    lvDiffModify = 0.3 - lvDiff / 100 ; 
	end			
	local DoubleModify = 1;
	if actor:HasBuff(218) and actor:HasBuff(254) then
		DoubleModify = 4;
	elseif actor:HasBuff(218) or actor:HasBuff(254) then
		DoubleModify = 2;
	end	
	--5%加成
	local BattleModify = 1;
	if actor:HasBuff(242) then
		BattleModify = 1.05;
	end	
	--队长加成10%
	local leadModify = 0 ;
	if actor:IsLeader() then
	    leadModify = 0.1 ;
	end
	local lvDiff_owner = actor:GetLevel() - owner:GetLevel();
	if lvDiff_owner < 5 then	
	    expGain = math.floor(npcExp * (4 * npcLevel + 10) / (4 * mapLevel + 14) * killCount * teamCountModify * lvDiffModify * DoubleModify * BattleModify) + 1 
	    LuaI.SetTempResult(expGain);
	    local jobCount = GetPartyDifferentClass(owner);
		if jobCount == 3 then 
    	    actor: SetBoundsExp(expGain * (0.02+leadModify) );
	    elseif jobCount == 4 then 
    	    actor: SetBoundsExp(expGain * (0.1+leadModify) );
		elseif teamCount >= 3 then
	        actor: SetBoundsExp(expGain * leadModify);
	    end
	end
end
function ValidActionTarget(Atk, Def)--隐身技能对战斗的影响
	LuaI.SetTempResult(1);
	if Def == 0 then
		LuaI.SetTempResult(0);
	elseif Atk == Def then
		do return end	
	elseif Atk:GetCommand() == LuaI.BATCMD_SKILL then
	    if Def:HasBuff(70) and Atk:GetSubType() ~= 79 then
			LuaI.SetTempResult(0);
		elseif Def:HasBuff(2005) and Atk:HasBuff(2006) == false and Atk:HasBuff(2039) == false then
			LuaI.SetTempResult(0);
		elseif Def:HasBuff(2038) and Atk:HasBuff(2039) == false then
			LuaI.SetTempResult(0);
		end
	elseif Atk:GetCommand() == LuaI.BATCMD_ATTACK then
		if Def:HasBuff(70) then
			LuaI.SetTempResult(0);
		elseif Def:HasBuff(2005) and Atk:HasBuff(2006) == false and Atk:HasBuff(2039) == false then
			LuaI.SetTempResult(0);
		elseif Def:HasBuff(2038) and Atk:HasBuff(2039) == false then
			LuaI.SetTempResult(0);
		end
	end	
end

function CheckEncounterBattle(player, m_movedStep, m_stepMod, mapEncRate, 
						m_encounterMod, mapLevel )--遇敌几率的设定（护身烟的影响）
	
	local map_id = player:GetMap_id()
	local needbuff = 1
	if map_id == 53900 or map_id == 54000 or map_id == 54100 or map_id == 54200 or map_id == 54300 
	or map_id == 54400 or map_id == 54500 or map_id == 54600 or map_id == 54700 or map_id == 54800 
	or map_id == 54900 or map_id == 55000 or map_id == 55100 or map_id == 55200 or map_id == 55300
	or map_id == 55400 or map_id == 55500 or map_id == 55600 or map_id == 55700 or map_id == 55800 then
		needbuff = 0
	end
	
	if player:HasBuff(213) and needbuff == 1 then
		--m_encounterMod = 1.35;
		--m_stepMod = 0;
	elseif player:HasBuff(214) and needbuff == 1 then
	    --m_encounterMod = 1.40;
		--m_stepMod = 0;
	elseif player:HasBuff(215) and needbuff == 1 then
		--m_encounterMod = 2.0;
		--m_stepMod = 0;
	end	
	
    --encRate = (1.0 * m_movedStep) / 50 * 100;
	local encRate = m_encounterMod * math.max(0, m_movedStep - m_stepMod ) / math.max(1, mapEncRate) * 100;
	
	if player:HasBuff(213) and needbuff == 1 then
	    
	    m_encounterMod = 1.35; --吃收费道具后，基础遇怪几率提高
		m_stepMod = 0; --吃收费道具后，不再有秒怪惩罚
		encRate = m_encounterMod * math.max(0, m_movedStep - m_stepMod) / math.max(1, mapEncRate) * 100; 
		--吃收费道具后，10步之内必遇怪
		if m_movedStep >= 30 then
		    encRate = 100;
		end
		
	elseif player:HasBuff(214) and needbuff == 1 then
	
	elseif player:HasBuff(215) and needbuff == 1 then
	
	end
	 
			
	local i = player:GetLevel() - mapLevel + 2;
	if player:HasBuff(210) and needbuff == 1 and i > 0 then
		encRate = 0;
	elseif player:HasBuff(211) and needbuff == 1 and i < -5 then
		encRate = 0;
	elseif player:HasBuff(212) and needbuff == 1 then
		encRate = 0;
	end		
	
	if player:HasBuff(256) and needbuff == 0 then
		encRate = 0
	end
	
	if math.random(1, 100) <= encRate then
   		player:SetEncounterBattle(true);
	else
   		player:SetEncounterBattle( false);
	end
	
	
end

function CanCatch(Atk, Def)--设定捕捉MOB几率（宠物技能的影响）
		local prob = 20; --20%
    local hpRatio =  Def:GetHP() / Def:GetMaxHP() * 100;
	local Mob_id = Def:GetMob_id(); 
    --[[local correctGoodness = true;
    if Def:GetGoodness() > 0 then    	
    	if Atk:GetGoodness() < Def:GetGoodness() then
    		correctGoodness = false;
    	end
    elseif Def:GetGoodness() < 0 then
		if Atk:GetGoodness() > Def:GetGoodness() then
    		correctGoodness = false;
    	end
	end	]]--
	
	
	--if correctGoodness then
		if Mob_id == 900 or Mob_id == 901 then
			prob = 80;
	    elseif hpRatio >= 80 and hpRatio < 100 then
	        prob = 30;
		elseif hpRatio >= 50 and hpRatio < 80 then
		    prob = 40;
		elseif hpRatio >= 10 and hpRatio < 50 then
		    prob = 50;
		elseif hpRatio < 10 then
		    prob = 60;
		end
	
		
	
		local ratio1 = math.random(1, 100);
		
		if ratio1 < prob then
		    LuaI.SetTempResult(1);
		else			
			local pet = Atk:GetUsingPet();
			if pet ~= nil then
				if pet:HasSkill(7005) then
				local ratio2 = math.random(1, 100);
					if ratio2 < prob then
						LuaI.SetTempResult(1);
						Atk:PetActive();
					else
						LuaI.SetTempResult(0);
					end	
				else
					LuaI.SetTempResult(0);
				end
			else
				LuaI.SetTempResult(0);
			end    
		end
	--[[else
		LuaI.SetTempResult(0);
	end]]--
end

function CalStepMod(char, turnNum)
    char:GetCclass();
	if char ~= nil then
   		--char:SetStepMod( math.max(0, (turnNum - 3) * 10));
		if turnNum ==1 then
		    char:SetStepMod((2 - turnNum) * 15);
		else
		    char:SetStepMod(0);
		end
   	end
end


function PartnerJoinBattle(favor)--好感度影响同伴参战设定
	local noJoinBattle = 0; -- 0% will not join the battle
	
	if favor >= 70 then
	    noJoinBattle = 0;
	elseif favor >= 50 and favor < 70 then
	    noJoinBattle = 10;
    elseif favor >= 30 and favor < 50 then
	    noJoinBattle = 40;
 	elseif favor >= 20 and favor < 30 then
	    noJoinBattle = 60;
	else
	    noJoinBattle = 100;
	end
	
	local rand = math.random(0, 100);
	if rand >= noJoinBattle then
	    LuaI.SetTempResult(1);
	else
	    LuaI.SetTempResult(0);
	end
	
end

function CalCatchSP(pmobLevel)--捕捉MOB需要的SP设定
	local catchSP = pmobLevel * 2 + 5;
	LuaI.SetTempResult(catchSP);
end

function CalPartnerFav(char, partner)--同伴好感度处理
	--[[local goodnessDiff = 0;
	if char:GetGoodness() > partner:GetGoodness() then
	    goodnessDiff = char:GetGoodness() - partner:GetGoodness();
	else
	    goodnessDiff = partner:GetGoodness() - char:GetGoodness();
	end
	
	local favSub = 0;

	if goodnessDiff >= 100 and goodnessDiff < 300 then
		favSub = 5;
	elseif goodnessDiff >= 300 and goodnessDiff < 500 then
		favSub = 15;
	elseif goodnessDiff >= 500 then
		favSub = 50;
	end
	
	if favSub > partner:GetFavor() then
	    partner:SetFavor(0);
	else
		local newFav = partner:GetFavor() - favSub;
		partner:SetFavor(newFav);
	end   --]]
end

function CalPlayerNextLvExp(Lv)--玩家升级所需经验公式
	local result = 0;
	if Lv >= 1 and Lv <= 10 then
		result = 0.0345 * Lv + 0.01 ;
	elseif Lv >= 11 and Lv <= 20 then
		result = 0.2627 * Lv - 2.272 ;
	elseif Lv >= 21 and Lv <= 30 then
		result = 0.5488 * Lv - 7.994 ;
	elseif Lv >= 31 and Lv <= 40 then
		result = 1.369 * Lv - 32.6 
	elseif  Lv >= 41 and Lv <= 50 then
	    result = 0.3346 * Lv + 8.776 
	elseif  Lv >= 51 and Lv <= 60 then
	    result = 1.908 * Lv - 69.894 ;	
	elseif  Lv >= 61 and Lv <= 70 then
	    result = 0.6208 * Lv + 7.338 ;
	elseif  Lv >= 71 and Lv <= 80 then
	    result = 0.2193 * Lv + 35.443 ; 
	elseif  Lv >= 81 and Lv <= 90 then
	    result = 0.366 * Lv + 23.707 ; 	
	end
	result = result * 6.84 * 30 * (4 * Lv + 10) + 100 ;
	LuaI.SetTempResult(result);
	return result;
end

function CalPartnerAttr(partner, needDecrease)--同伴战斗中属性
	local sta = partner:GetSTA();
	local spr = partner:GetSPR();
	local str = partner:GetSTR();
	local con = partner:GetCON();
	local agi = partner:GetAGI();
	local grow = partner:GetGrow();
	local level = partner:GetLevel();

	--local atk = str * 1.2 * grow  + level * 1.8 * grow + (2.5 + partner:GetATK_native() / 600 ) * level ;
	local atk = str * 1.2 * grow  + level * 1.8 * grow + (partner:GetATK_native() / 600 ) * level +60;
	
	--local def = con * 1.8 * grow + level * 1.2 * grow + (2.0 + partner:GetDEF_native() / 800 ) *level  ;
	local def = con * 1.8 * grow + level * 1.2 * grow + (partner:GetDEF_native() / 800 ) *level  ;
	
	local spd = agi * partner:GetSPD_native() / 1500;
	
	--local power = (spr * 1.2 + str * 0.7 + sta * 0.4 + con * 0.3) + level * (3 * grow-2.8) + (1 + partner:GetPOW_native()/1500) * level ;
	local power = (spr * 1.2 + str * 0.7 + sta * 0.4 + con * 0.3) + level * grow + (partner:GetPOW_native()/3000) * level ;
	
	local hit = agi * (grow * 5 - 1.5) + level * grow * 2 + (partner:GetHIT_native() - 600) * level / 1200;
	local eva = agi * (grow * 2.5 - 2) + level * grow * 0.8 + partner:GetEVA_native() * level/ 2000;

	local maxHP = sta * grow * 7.2 + partner:GetHP_native() * level / 1000;
	
	local maxSP = spr * 3.6 * grow  + partner:GetSP_native() * level / 500;
	
	partner:SetAttrib2(atk, def, hit, eva, power, spd);
	local hp = partner:GetHP();
	partner:SetHP(hp, maxHP);
	local sp = partner:GetSP();
	partner:SetSP(sp, maxSP);
	
	level = level + 1; --for cal next level
	local nextLevelExp = CalPlayerNextLvExp(level) / 3;
	partner:SetNextLvExp(nextLevelExp);

end

function CalBatMobAttr(batActor, batMob, needDecrease)--怪物战斗中属性
--[[batActor and batMob is pointing to same object
	but some function is belong to batmob. Therefore, this
	function need to have both batmob and batactor pointer.
	]]--

	local sta = batActor:GetSTA();
	local spr = batActor:GetSPR();
	local str = batActor:GetSTR();
	local con = batActor:GetCON();
	local agi = batActor:GetAGI();
	local grow = batMob:GetGrow();
	local level = batActor:GetLevel();
		
	--local atk = str * 1.2 * grow  + level * 1.8 * grow + (2.5 + batMob:GetATK_native() / 600 ) * level ;
	local atk = str * 1.2 * grow  + level * 1.8 * grow + (3.0 + batMob:GetATK_native() / 600 ) * level ;
	
	--local def = con * 1.8 * grow + level * 1.2 * grow + (2.0 + batMob:GetDEF_native() / 800 ) *level  ;
	local def = con * 1.8 * grow + level * 1.2 * grow + (batMob:GetDEF_native() / 800 ) *level  ;
	
	local spd = agi * batMob:GetSPD_native() / 1500;
	
	--local power = (spr * 1.2 + str * 0.7 + sta * 0.4 + con * 0.3) + level * (3 * grow-2.8) + ( batMob:GetPOW_native()/3000) * level ;
	local power = (spr * 1.2 + str * 0.7 + sta * 0.4 + con * 0.3) + level * grow + (2.0 + batMob:GetPOW_native()/3000) * level ;
	
	local hit = agi * (grow * 5 - 1.5) + level * grow * 2 + (batMob:GetHIT_native() - 600) * level / 1200;
	local eva = agi * (grow * 2.5 - 2) + level * grow * 0.8 + batMob:GetEVA_native() * level/ 2000;
	
	local atklevelmodify = 1;
	local deflevelmodify = 1;
	local powerlevelmodify = 1;
	local hitlevelmodify = 1;
	local evalevelmodify = 1;
	local spdlevelmodify = 1;
	local maxHPlevelmodify = 1;
	local maxSPlevelmodify = 1;
	if level < 10 then
	    atklevelmodify = 0.5;
		deflevelmodify = 0.4;
		powerlevelmodify = 0.5;
		hitlevelmodify = 0.8;
		evalevelmodify = 0.3;
		spdlevelmodify = 0.7;
		maxHPlevelmodify = 0.6;
		maxSPlevelmodify = 1;
	elseif level < 20 then
	    atklevelmodify = 0.5;
		deflevelmodify = 0.4;
		powerlevelmodify = 0.5;
		hitlevelmodify = 0.8;
		evalevelmodify = 0.3;
		spdlevelmodify = 0.7;
		maxHPlevelmodify = 0.5;
		maxSPlevelmodify = 1;
	elseif level < 30 then
	    atklevelmodify = 0.55;
		deflevelmodify = 0.4;
		powerlevelmodify = 0.5;
		hitlevelmodify = 0.8;
		evalevelmodify = 0.3;
		spdlevelmodify = 0.7;
		maxHPlevelmodify = 0.5;
		maxSPlevelmodify = 1;
	elseif level < 40 then
	    atklevelmodify = 0.55;
		deflevelmodify = 0.5;
		powerlevelmodify = 0.55;
		hitlevelmodify = 0.8;
		evalevelmodify = 0.4;
		spdlevelmodify = 0.8;
		maxHPlevelmodify = 0.6;
		maxSPlevelmodify = 1;
	elseif level < 50 then
	    atklevelmodify = 0.6;
		deflevelmodify = 0.6;
		powerlevelmodify = 0.6;
		hitlevelmodify = 0.85;
		evalevelmodify = 0.5;
		spdlevelmodify = 0.85;
		maxHPlevelmodify = 0.65;
		maxSPlevelmodify = 1;
	elseif level < 60 then
	    atklevelmodify = 0.65;
		deflevelmodify = 0.65;
		powerlevelmodify = 0.65;
		hitlevelmodify = 0.9;
		evalevelmodify = 0.6;
		spdlevelmodify = 0.9;
		maxHPlevelmodify = 0.7;
		maxSPlevelmodify = 1;
	elseif level < 70 then
	    atklevelmodify = 0.7;
		deflevelmodify = 0.7;
		powerlevelmodify = 0.7;
		hitlevelmodify = 0.95;
		evalevelmodify = 0.7;
		spdlevelmodify = 0.95;
		maxHPlevelmodify = 0.75;
		maxSPlevelmodify = 1;
	end
	
	if needDecrease then
	    atk = atk * atklevelmodify;
	    def = def * deflevelmodify;
	    spd = spd * spdlevelmodify;
	    power = power * powerlevelmodify;
	    hit = hit * hitlevelmodify;
	    eva = eva * evalevelmodify;
	end

	local maxHP = sta * grow * 7.2 + batMob:GetHP_native() * level / 1000;
	
	local maxSP = spr * 3.6 * grow  + batMob:GetSP_native() * level / 500;
	
	batActor:SetAttrib2(atk, def, hit, eva, power, spd);
	batActor:SetMaxHP(maxHP * maxHPlevelmodify);
	batActor:SetMaxSP(maxSP * maxSPlevelmodify);

end

function CalCharAttr(pChar, pCharData, sta, spr, str, con, agi )--玩家属性
--[[
	the sta, spr... agi is modified by the item before passing in here.
	Therefore, the value of pChar:GetSTA(), pChar:GetSPR()... is not the same
	as sta, spr.
]]
    local atk = str * pCharData:GetAtk_STR() + 60;
	local def = con * pCharData:GetDef_CON();
	local hit = agi * pCharData:GetHit_AGI() + 6;
	local eva = agi * pCharData:GetEva_AGI();
	local power = spr * pCharData:GetPwd_SPR() + sta * pCharData:GetPwd_STA() + con * pCharData:GetPwd_CON() + str * pCharData:GetPwd_STR();
	local spd = agi * pCharData:GetSpd_AGI() + str * pCharData:GetSpd_STR();

    local maxHP = sta * pCharData:GetMaxHP_STA() + 160;
	local maxSP = spr * pCharData:GetMaxSP_SPR() + 80;
	
	pChar:SetAttrib2(atk, def, hit, eva, power, spd);
	pChar:SetHP(pChar:GetHP(), pChar:GetMHP(), maxHP);
	pChar:SetSP(pChar:GetSP(), pChar:GetMSP(), maxSP);

end

function CalBatCharAttr(pBatActor, pCharData, sta, spr, str, con, agi )--玩家战斗中属性
--[[
	the sta, spr... agi is modified by the item before passing in here.
	Therefore, the value of pChar:GetSTA(), pChar:GetSPR()... is not the same
	as sta, spr.
]]
    local atk = str * pCharData:GetAtk_STR() + 60;
	local def = con * pCharData:GetDef_CON();
	local hit = agi * pCharData:GetHit_AGI() + 6;
	local eva = agi * pCharData:GetEva_AGI();
	local power = spr * pCharData:GetPwd_SPR() + sta * pCharData:GetPwd_STA() + con * pCharData:GetPwd_CON() + str * pCharData:GetPwd_STR();
	local spd = agi * pCharData:GetSpd_AGI() + str * pCharData:GetSpd_STR();

    local maxHP = sta * pCharData:GetMaxHP_STA() + 160;
    local maxSP = spr * pCharData:GetMaxSP_SPR() + 80;
	
	pBatActor:SetAttrib2(atk, def, hit, eva, power, spd);
 	pBatActor:SetMaxHP(maxHP);
	pBatActor:SetMaxSP(maxSP);
end

function EndBattle(pBatActor)--玩家战斗后处理（碧血丹、碧灵丹）
	if pBatActor:HasBuff(249) then
		local CurHP = pBatActor:GetHP();
		local MaxHP = pBatActor:GetMHP();
		local Buff249 = pBatActor:FindBuff(249);
		if CurHP > 0 then
			if Buff249 then
				local value1 = Buff249:GetBuffValue1();
				if value1 <= MaxHP - CurHP then
					pBatActor:ChangeHp(value1);
					pBatActor:RemoveBuff(249);
				else
					pBatActor:ChangeHp(MaxHP - CurHP);
					value1 = value1 - (MaxHP - CurHP);
					Buff249:SetBuffValue1(value1);
				end
			end
		end	
	end
	if pBatActor:HasBuff(250) then
		local CurSP = pBatActor:GetSP();
		local MaxSP = pBatActor:GetMSP();
		local Buff250 = pBatActor:FindBuff(250);
		if CurSP >= 0 then
			if Buff250 then
				local value1 = Buff250:GetBuffValue1();
				if value1 <= MaxSP - CurSP then
					pBatActor:ChangeSp(value1);
					pBatActor:RemoveBuff(250);
				else
					pBatActor:ChangeSp(MaxSP - CurSP);
					value1 = value1 - (MaxSP - CurSP);
					Buff250:SetBuffValue1(value1);
				end
			end
		end	
	end
end

function partnerEndBattle(pPlayer, pPartner)--同伴战斗后处理（碧血丹、碧灵丹）
	if pPlayer:HasBuff(249) then
		local pCurHP = pPartner:GetHP();
		local pMaxHP = pPartner:GetMHP();
		local Buff249 = pPlayer:FindBuff(249);
	if pCurHP > 0 then
			if Buff249 then
				local value1 = Buff249:GetBuffValue1();
				if value1 <= pMaxHP - pCurHP then
					pPartner:ChangeHp(value1);
					pPlayer:RemoveBuff(249);
				else
					pPartner:ChangeHp(pMaxHP - pCurHP);
					value1 = value1 - (pMaxHP - pCurHP);
					Buff249:SetBuffValue1(value1);
				end
			end
		end	
	end	
	if pPlayer:HasBuff(250) then
		local pCurSP = pPartner:GetSP();
		local pMaxSP = pPartner:GetMSP();
		local Buff250 = pPlayer:FindBuff(250);
		if pCurSP >= 0 then
			if Buff250 then
				local value1 = Buff250:GetBuffValue1();
				if value1 <= pMaxSP - pCurSP then
					pPartner:ChangeSp(value1);
					pPlayer:RemoveBuff(250);
				else
					pPartner:ChangeSp(pMaxSP - pCurSP);
					value1 = value1 - (pMaxSP - pCurSP);
					Buff250:SetBuffValue1(value1);
				end
			end
		end	
	end
end

function CalMobCount(leader, minLackey, maxLackey, partyCount)--战斗中呼出喽罗个数设定
	
	local min = minLackey;
	if minLackey == 255 then
		min  = partyCount;
	end	
	local max = maxLackey;
	if maxLackey == 255 then
		max  = partyCount * 3 - 1/2 * (partyCount - 1) * (partyCount - 2) + 1/24 * (partyCount - 1) * (partyCount - 2) * (partyCount - 3) * (partyCount - 4);
	end	
	local nMob = math.random(min, max);
	if  minLackey == 255 and maxLackey == 255 then
		if partyCount <=2 then
		    local num = math.random(1, 10);
	        if num <= 3 then
	            nMob = 2 * partyCount;
	        end
		end	
		if leader:HasBuff(251) then
			nMob = 10;
		end
	end	
	LuaI.SetTempResult(nMob);
end

function DeadPenaltyCheck(pBatActor)--战斗死亡后身上有替身娃娃的处理
	--[[if pBatActor:HasBuff(61010) then
		pBatActor:SetPunish(false);
		pBatActor:SetJumpBack(false);
	end]]--
end



function CalPetNextLvExp(Lv)--宠物所需升级经验
	--local result = 5 * Lv * (Lv  + 1) + 90;
	local result = 40 * Lv + 80;
	LuaI.SetTempResult(result);
end

function DeadPenaltyCheck(pBatActor)--战斗死亡处理
	local item = pBatActor:FindItemByid(31002);
	local lv = actor:GetLevel();
	
	if lv < 10 then
		pBatActor:SetPunish(false);
		pBatActor:SetJumpBack(false);
	elseif item ~= nil then
		item:ReduceCount();
		pBatActor:SetPunish(false);
		pBatActor:SetJumpBack(false);
	end	
end

function itemMul(user, target)
	if Def:GetFaction() == 6 then
		LuaI.SetTempResult(1.5);
	else
		LuaI.SetTempResult(1);
	end
end

function CalWCNextLvExp(Lv)
	local result = Lv * 2;
	LuaI.SetTempResult(result);
end
   
function GetPartyDifferentClass(pActor)
	
	local battle = pActor:GetBattle();
	local teamNo = pActor:GetTeam();
	local listCount = 0;
	local list = {};
	local i = 0;
	while(true)   do
		local actor = battle:GetActor(i);
		if actor == nil then
			break;
		end
		
		if actor:IsPlayer() and actor:GetTeam() == teamNo and actor:GetHP() > 0 and not actor:IsEscaped() then
			local found = false;
			for x = 0, listCount - 1 do
				if  list[x] == actor:GetCclass() then
					found = true;
					break;
				end
			end
			if not found then
				 list[listCount] = actor:GetCclass();
				 listCount = listCount + 1;
			end
		end		
		i = i + 1;
	end
	
	return listCount;
end

function CalPlayerMaxVP(Char)
	LuaI.SetTempResult(40 + Char:GetLevel() * 10);
end

function CalSkillNextLvExp(Lv, shopID)
	local result = CalPlayerNextLvExp(Lv) / 12 ;
	
	if shopID == 1 then
		LuaI.SetTempResult(result * 2);
	else	
		LuaI.SetTempResult(result);
	end
end

function CalSkillNextLvMoney(Lv, shopID)
	local result = 0;
    result = 0.3 * Lv^3 + 10 ;
	LuaI.SetTempResult(result);
end

function CalSkillNextLvYuanBao(Lv, shopID)
	local result = CalPlayerNextLvExp(Lv) / 12 ;	
	LuaI.SetTempResult(result);
end

function CalSkillNextLvHonor(Lv, shopID)
	local result = CalPlayerNextLvExp(Lv) / 12 ;	
	LuaI.SetTempResult(result);
end

function CalSkillNextLvDonateFaction(Lv, shopID)
	local result = CalPlayerNextLvExp(Lv) / 12 ;	
	LuaI.SetTempResult(result);
end

function CalSkillNextLvDonateManor(Lv, shopID)
	local result = CalPlayerNextLvExp(Lv) / 12 ;	
	LuaI.SetTempResult(result);
end
   