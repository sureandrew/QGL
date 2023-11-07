function CalPlayerNextLvExp(Lv)
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
	LuaUI.SetTempResult(result);
	return result;
end

function CalSkillNextLvExp(Lv, shopID)
	local result = CalPlayerNextLvExp(Lv) / 12 ;
	
	--if shopID == 1 then
		--LuaUI.SetTempResult(result * 2);
	--else	
		LuaUI.SetTempResult(result);
	--end
end

function CalSkillNextLvMoney(Lv, shopID)
	local result = 0;
    result = 0.3 * Lv^3 + 10 ;
	LuaUI.SetTempResult(result);
end

function CalSkillNextLvYuanBao(Lv, shopID)
	local result = CalPlayerNextLvExp(Lv) / 12 ;	
	LuaUI.SetTempResult(result);
end

function CalSkillNextLvHonor(Lv, shopID)
	local result = CalPlayerNextLvExp(Lv) / 12 ;	
	LuaUI.SetTempResult(result);
end

function CalSkillNextLvDonateFaction(Lv, shopID)
	local result = CalPlayerNextLvExp(Lv) / 12 ;	
	LuaUI.SetTempResult(result);
end

function CalSkillNextLvDonateManor(Lv, shopID)
	local result = CalPlayerNextLvExp(Lv) / 12 ;	
	LuaUI.SetTempResult(result);
end

function CalPetNextLvExp(Lv)
	--local result = 5 * Lv * (Lv  + 1) + 90;
	local result = 40 * Lv + 80;
	LuaUI.SetTempResult(result);
end