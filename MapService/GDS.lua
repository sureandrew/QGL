
GDS = { target = nil }

function GDS.SetTarget(name)
	if player == nil then
		print("GDS Warning: 'player' is nil!")
	end
	if name == "player" then
		GDS.target = player
	elseif name == "target" then
		GDS.target = target
	elseif name == "team" then
		GDS.target = party
	elseif party and name == "party1" then
		GDS.target = party:GetMember(0)
	elseif party and name == "party2" then
		GDS.target = party:GetMember(1)
	elseif party and name == "party3" then
		GDS.target = party:GetMember(2)
	elseif party and name == "party4" then
		GDS.target = party:GetMember(3)
	elseif party and name == "party5" then
		GDS.target = party:GetMember(4)
	else
		print("GDS Warning: GDS.target is nil!")
		GDS.target = nil
	end

	return (GDS.target ~= nil)
end

function GDS.GetCharID(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetChar_id()
end

function GDS.GetFaction(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetFaction()
end

function GDS.GetLevel(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetLevel()
end

function GDS.GetTeamLevel()
	if party then
		return party:GetAvgLevel()
	else
		return player:GetLevel()
	end
end

function GDS.GetTeamJob(jobType)
	if party then
		local c = 0
		for i = 0, party:GetCount() - 1 do
			local member = party:GetMember(i)
			if member and member:GetJob() == jobType then
				c = c + 1
			end
		end
		return c
	else
		return (player:GetJob() == jobType and 1)
	end
end

function GDS.GetTeamSex(sexType)
	if party then
		local c = 0
		for i = 0, party:GetCount() - 1 do
			local member = party:GetMember(i)
			if member and member:GetSex() == sexType then
				c = c + 1
			end
		end
		return c
	else
		return (player:GetSex() == sexType and 1)
	end
end

function GDS.GetExp(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetExp()
end

function GDS.GetMemberNum()
	if party then
		return party:GetCount()
	else
		return 0
	end
end

function GDS.GetJob(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetCclass()
end

function GDS.GetSex(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetSex()
end

function GDS.GetUnitStatus(name, unit)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 1 then
		return GDS.target:GetSTA()
	elseif unit == 2 then
		return GDS.target:GetSPR()
	elseif unit == 3 then
		return GDS.target:GetSTR()
	elseif unit == 4 then
		return GDS.target:GetCON()
	elseif unit == 5 then
		return GDS.target:GetAGI()
	elseif unit == 6 then
		return GDS.target:GetATK()
	elseif unit == 7 then
		return GDS.target:GetDEF()
	elseif unit == 8 then
		return GDS.target:GetHIT()
	elseif unit == 9 then
		return GDS.target:GetEVA()
	elseif unit == 10 then
		return GDS.target:GetPWD()
	elseif unit == 11 then
		return GDS.target:GetSPD()
	else
		return 0
	end
end

function GDS.GetHP(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetHP()
end

function GDS.GetSP(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetSP()
end

function GDS.GetDP(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetDP()
end

function GDS.GetWC(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetWC()
end

function GDS.GetVP(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetVP()
end

function GDS.GetGoodness(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetGoodness()
end

function GDS.GetHonor(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetHonor()
end

function GDS.GetFaction(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetFaction()
end

function GDS.GetDonate(name, dtype)
	if not GDS.SetTarget(name) then return 0 end
	if dtype == 0 then
		return GDS.target:GetFDonate()
	elseif dtype == 1 then
		return GDS.target:GetTotalFDonate()
	elseif dtype == 2 then
		return GDS.target:GetMDonate()
	elseif dtype == 3 then
		return GDS.target:GetTotalMDonate()
	else
		return 0
	end
end

function GDS.GetMoney(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetMoney()
end

function GDS.GetGameMoney(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetGameMoney()
end

function GDS.GetBankMoney(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetBankMoney()
end

function GDS.GetItemCount(name, item_id)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetItems():GetItemCount(item_id)
end

function GDS.GetFreeSlot(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetItems():GetFreeSlot(0)
end

function GDS.GetTotalFreeSlot(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetItems():GetTotalFreeSlot()
end

function GDS.GetPartnerTotalCount(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetPartners():GetCount()
end

function GDS.GetPartnerCount(name, mob_id)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetPartners():GetPartnerCount(mob_id)
end

function GDS.GetPartnerSkillCount(name, skillType)
	if not GDS.SetTarget("target") then return 0 end
	if skillType == nil then skillType = 0 end

	if GDS.target:GetUserType() ~= 2 or skillType < 0 or skillType > 2 then
		return 0
	end
	if skillType == 0 then
		return GDS.target:GetNormalSkillCount()
	else
		return GDS.target:GetSpecialSkillCount()
	end
end

function GDS.HasPartnerSkill(name, skill_id)
	if not GDS.SetTarget("target") then return false end
	if GDS.target:GetUserType() ~= 2 then return false end

	return GDS.target:HasSkill(skill_id)
end

function GDS.GetGameTime()
	return LuaI.GetGameTime(1)
end

function GDS.GetPKPoint(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetPK_Value()
end

function GDS.GetSkillLevel(name, skill_id)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetSkillLv(skill_id)
end

function GDS.GetPlayerName(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetName()
end

function GDS.GetNpcName(npc_id)
	return LuaI.GetNpcName(npc_id)
end

function GDS.GetMobName(mob_id)
	return LuaI.GetMobName(mob_id)
end

function GDS.GetMapName(map_id)
	return LuaI.GetMapName(map_id)
end

function GDS.GetTypeName(name, type_id)
	if not GDS.SetTarget(name) then return 0 end
	local qt = GDS.target:GetQuests():GetType(type_id)
	if qt then
		return qt:GetName()
	else
		return ""
	end
end

function GDS.GetItemName(item_id)
	return LuaI.GetItemName(item_id)
end

function GDS.GetLineId(name)
	if not GDS.SetTarget(name) then return 0 end
	local map GDS.target:GetMap()
	if map then
		return map:GetLine_id()
	else
		return 0
	end
end

function GDS.GetMapId(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetMap_id()
end

function GDS.GetMapX(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetCurPosX()
end

function GDS.GetMapY(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetCurPosY()
end

function GDS.GetItemRootId(name, item_uid)
	if not GDS.SetTarget(name) then return 0 end
	local item = GDS.target:GetItems():FindItemByUid(item_uid)
	if item then
		return item:GetItem_id()
	else
		return 0
	end
end

function GDS.GetTeamJob(job)
	if party then
		return party:GetJobCount(job)
	else
		return 0
	end
end

function GDS.GetTeamSex(sex)
	if party then
		return party:GetSexCount(sex)
	else
		return 0
	end
end

function GDS.BuffExists(name, buff_id)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:HasBuff(buff_id)
end

function GDS.IsValidBlock(map_id, x, y)
	local map = LuaI.GetMap(map_id)
	if map then
		return map:IsBlocked(x, y)
	else
		return true
	end
end

function GDS.SkillExists(name, skill_id)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:HasSkill(skill_id)
end

function GDS.HasEquiped(name, item_id)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetItems():IsEquiped(item_id)
end

function GDS.IsNight()
	return LuaI.GetGameTime(0)
end

function GDS.IsLeader()
	if not GDS.SetTarget("player") then return 0 end
	return GDS.target:IsLeader()
end

function GDS.GetSpeakCount(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetSpeakCount()
end

function GDS.GetMaxPartner(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetMaxPartner()
end

function GDS.AddPartner(name, mob_id)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetPartners():AddPartner(mob_id)
end

function GDS.AddFlag(name, flag_id)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetQuests():SetFlag(flag_id, 0)
end

function GDS.AddItem(name, item_id, count)
	if not GDS.SetTarget(name) then return 0 end
	local item = GDS.target:GetItems():NewItem(item_id, count)
	if item then
		GDS.target:GetItems():AddItem(item, 0)
	end
end

function GDS.AddItemFromCSV(name, csvFile)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetItems():AddRandItem(csvFile)
end

function GDS.AddLevel(name, n)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetLevel(GDS.target:GetLevel() + n)
end

function GDS.AddExp(name, n)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetExp(GDS.target:GetExp() + n)
end

function GDS.AddUnitStatus(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 1 then
		GDS.target:ChangeSTA(value)
	elseif unit == 2 then
		GDS.target:ChangeSPR(value)
	elseif unit == 3 then
		GDS.target:ChangeSTR(value)
	elseif unit == 4 then
		GDS.target:ChangeCON(value)
	elseif unit == 5 then
		GDS.target:ChangeAGI(value)
	elseif unit == 6 then
		GDS.target:ChangeATK(value)
	elseif unit == 7 then
		GDS.target:ChangeDEF(value)
	elseif unit == 8 then
		GDS.target:ChangeHIT(value)
	elseif unit == 9 then
		GDS.target:ChangeEVA(value)
	elseif unit == 10 then
		GDS.target:ChangePWD(value)
	elseif unit == 11 then
		GDS.target:ChangeSPD(value)
	end
end

function GDS.AddHP(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeHP(value)
	elseif unit == 1 then
		GDS.target:ChangeMHP(value)
	elseif unit == 2 then
		GDS.target:ChangeMaxHP(value)
	end
end

function GDS.AddSP(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeSP(value)
	elseif unit == 1 then
		GDS.target:ChangeMSP(value)
	elseif unit == 2 then
		GDS.target:ChangeMaxSP(value)
	end
end

function GDS.AddDP(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeDP(value)
	elseif unit == 1 then
		GDS.target:ChangeMaxDP(value)
	end
end

function GDS.AddWC(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeWC(value)
	elseif unit == 1 then
		GDS.target:ChangeWcExp(value)
	elseif unit == 2 then
		GDS.target:ChangeMaxWC(value)
	end
end

function GDS.AddVP(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeVP(value)
	elseif unit == 1 then
		GDS.target:ChangeMaxVP(value)
	end
end

function GDS.AddGoodness(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetGoodness(GDS.target:GetGoodness() + value)
end

function GDS.AddHonor(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetHonor(GDS.target:GetHonor() + value)
end

function GDS.AddDonate(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeFDonate(value)
	elseif unit == 1 then
		GDS.target:ChangeMDonate(value)
	end
end

function GDS.AddMoney(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetMoney(GDS.target:GetMoney() + value)
end

function GDS.AddGameMoney(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetGameMoney(GDS.target:GetGameMoney() + value)
end

function GDS.AddBankMoney(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetBankMoney(GDS.target:GetBankMoney() + value)
end

function GDS.AddPKPoint(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetPK_Value(GDS.target:GetPKValue() + value)
end

function GDS.AddSkill(name, skill_id)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:AddSkill(skill_id)
end

function GDS.AddSkillLevel(name, skill_id, level)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:ChangeSkillLv(skill_id, level)
end

function GDS.AddPartnerExp(name, value)
	if not GDS.SetTarget(name) then return 0 end
	local partner
	if battle then
		partner = GDS.target:GetCurPartner()
	else
		partner = GDS.target:GetPartners():GetFightPartner()
	end
	if partner then
		partner:SetExp(partner:GetExp() + value)
	end
end

function GDS.AddPartnerHealth(name, value)
	if not GDS.SetTarget(name) then return 0 end
	local partner
	if battle then
		partner = GDS.target:GetCurPartner()
	else
		partner = GDS.target:GetPartners():GetFightPartner()
	end
	if partner then
		partner:SetHealth(partner:GetHealth() + value)
	end
end

function GDS.AddPartnerFeel(name, value, targetType)
	if not GDS.SetTarget(name) then return 0 end
	if targetType == nil then targetType = 0 end
	if battle then
		local partner = GDS.target:GetCurPartner()
		partner:ChangeFavor(value)
	else
		GDS.target:GetPartners():ChangeFavor(targetType, value)
	end
end

function GDS.AddBuff(name, buff_id, v1, v2, v3, cool, chance)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:AddBuff(buff_id, v1, v2, v3, cool, chance)
end

function GDS.AddSpeakCount(name, count)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:ChangeSpeakCount(count)
end

function GDS.RemoveSpeakCount(name, count)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:ChangeSpeakCount(-count)
end

function GDS.RemoveItem(name, item_id, count)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetItems():RemoveItemCount(item_id, count)
end

function GDS.RemoveFlag(name, flag_id)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetQuests():RemoveFlag(flag_id)
end

function GDS.RemovePartner(name, partner_id)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetPartners():RemovePartner(partner_id)
end

function GDS.RemoveLevel(name, level)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetLevel(GDS.target:GetLevel() - level)
end

function GDS.RemoveExp(name, exp)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetExp(GDS.target:GetExp() - exp)
end

function GDS.RemoveUnitStatus(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 1 then
		GDS.target:ChangeSTA(-value)
	elseif unit == 2 then
		GDS.target:ChangeSPR(-value)
	elseif unit == 3 then
		GDS.target:ChangeSTR(-value)
	elseif unit == 4 then
		GDS.target:ChangeCON(-value)
	elseif unit == 5 then
		GDS.target:ChangeAGI(-value)
	elseif unit == 6 then
		GDS.target:ChangeATK(-value)
	elseif unit == 7 then
		GDS.target:ChangeDEF(-value)
	elseif unit == 8 then
		GDS.target:ChangeHIT(-value)
	elseif unit == 9 then
		GDS.target:ChangeEVA(-value)
	elseif unit == 10 then
		GDS.target:ChangePWD(-value)
	elseif unit == 11 then
		GDS.target:ChangeSPD(-value)
	end
end

function GDS.RemoveHP(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeHP(-value)
	elseif unit == 1 then
		GDS.target:ChangeMHP(-value)
	elseif unit == 2 then
		GDS.target:ChangeMaxHP(-value)
	end
end

function GDS.RemoveSP(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeSP(-value)
	elseif unit == 1 then
		GDS.target:ChangeMSP(-value)
	elseif unit == 2 then
		GDS.target:ChangeMaxSP(-value)
	end
end

function GDS.RemoveDP(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeDP(-value)
	elseif unit == 1 then
		GDS.target:ChangeMaxDP(-value)
	end
end

function GDS.RemoveWC(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeWC(-value)
	elseif unit == 1 then
		GDS.target:ChangeWcExp(-value)
	elseif unit == 2 then
		GDS.target:ChangeMaxWC(-value)
	end
end

function GDS.RemoveVP(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeVP(-value)
	elseif unit == 1 then
		GDS.target:ChangeMaxVP(-value)
	end
end

function GDS.RemoveGoodness(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetGoodness(GDS.target:GetGoodness() - value)
end

function GDS.RemoveHonor(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetHonor(math.max(GDS.target:GetHonor() - value, 0))
end

function GDS.RemoveDonate(name, unit, value)
	if not GDS.SetTarget(name) then return 0 end
	if unit == 0 then
		GDS.target:ChangeFDonate(-value)
	elseif unit == 1 then
		GDS.target:ChangeMDonate(-value)
	end
end

function GDS.RemoveMoney(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetMoney(math.max(GDS.target:GetMoney() - value, 0))
end

function GDS.RemoveGameMoney(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetGameMoney(math.max(GDS.target:GetGameMoney() - value, 0))
end

function GDS.RemoveBankMoney(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetBankMoney(math.max(GGDS.target:GetBankMoney() - value, 0))
end

function GDS.RemovePKPoint(name, value)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetPK_Value(math.max(GGDS.target:GetPKValue() - value, 0))
end

function GDS.RemoveSkill(name, skill_id)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:RemoveSkill(skill_id)
end

function GDS.RemoveSkillLevel(name, skill_id, level)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:ChangeSkillLv(skill_id, -level)
end

function GDS.RemovePartnerExp(name, value)
	if not GDS.SetTarget(name) then return 0 end
	local partner
	if battle then
		partner = GDS.target:GetCurPartner()
	else
		partner = GDS.target:GetPartners():GetFightPartner()
	end
	if partner then
		partner:SetExp(math.max(partner:GetExp() + value, 0))
	end
end

function GDS.RemovePartnerHealth(name, value)
	if not GDS.SetTarget(name) then return 0 end
	local partner
	if battle then
		partner = GDS.target:GetCurPartner()
	else
		partner = GDS.target:GetPartners():GetFightPartner()
	end
	if partner then
		partner:SetHealth(math.max(partner:GetHealth() + value, 0))
	end
end

function GDS.RemovePartnerFeel(name, value)
	if not GDS.SetTarget(name) then return 0 end
	local partner
	if battle then
		partner = GDS.target:GetCurPartner()
	else
		partner = GDS.target:GetPartners():GetFightPartner()
	end
	if partner then
		partner:ChangeFavor(-value)
	end
end

function GDS.RemoveTimeMark(name, markId)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetQuests():RemoveTimeMark(markId)
end

function GDS.RemoveBuff(name, buff_id)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:RemoveBuff(buff_id)
end

function GDS.SetFaction(name, faction)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetFaction(faction)
end

function GDS.SetFlag(name, flag_id, flag)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetQuests():SetFlag(flag_id, flag)
end

function GDS.PVPEnable(name, enable)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetEnablePK(enable)
end

function GDS.SetPKPoint(name, point)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetPK_Value(point)
end

function GDS.SetReborn(name, map_id, x, y)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetReborn(map_id, x, y)
end

function GDS.SetFullHP(name)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetHP(GDS.target:GetMaxHP())
	GDS.target:SetMHP(GDS.target:GetMaxHP())
end

function GDS.SetFullSP(name)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetSP(GDS.target:GetMaxSP())
	GDS.target:SetMSP(GDS.target:GetMaxSP())
end

function GDS.SetFullDP(name)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetDP(GDS.target:GetMaxDP())
	GDS.target:SetMDP(GDS.target:GetMaxDP())
end

function GDS.SetFullPHP(name)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetPartners():SetFullHP();
end

function GDS.SetFullPSP(name)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetPartners():SetFullSP();
end

function GDS.SetTimeMark(name, markid)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetQuests():SetTimeMark(markid)
end

function GDS.SetLevel(name, level)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetLevel(level)
end

function GDS.SetMaxPartner(name, num)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetMaxPartner(num)
end

function GDS.Battle(npcId)
	if not GDS.SetTarget("player") then return 0 end
	GDS.target:EncounterBattle(npcId)
end

function GDS.JumpMap(type, map_id, x, y)
	if not GDS.SetTarget("player") then return 0 end
	if type == "map" then
		GDS.target:JumpMap(map_id, x, y)
	elseif type == "reborn" then
		GDS.target:JumpReborn()
	end
end

function GDS.OpenInterface(name, id)
	if not GDS.SetTarget("player") then return 0 end
	local script = GDS.target:GetScript()
	if script == nil then return 0 end
	script:OpenInterface(name, id)
end

function GDS.Choose(question, a1, f1, a2, f2, a3, f3, a4, f4, a5, f5, a6, f6,
					a7, f7, a8, f8, a9, f9, a10, f10, a11, f11, a12, f12, rand)
	if not GDS.SetTarget("player") then return 0 end
	local alist = {}
	local flist = {}
	if a1 and f1 then
		table.insert(alist, a1)
		table.insert(flist, f1)
	end
	if a2 and f2 then
		table.insert(alist, a2)
		table.insert(flist, f2)
	end
	if a3 and f3 then
		table.insert(alist, a3)
		table.insert(flist, f3)
	end
	if a4 and f4 then
		table.insert(alist, a4)
		table.insert(flist, f4)
	end
	if a5 and f5 then
		table.insert(alist, a5)
		table.insert(flist, f5)
	end
	if a6 and f6 then
		table.insert(alist, a6)
		table.insert(flist, f6)
	end
	if a7 and f7 then
		table.insert(alist, a7)
		table.insert(flist, f7)
	end
	if a8 and f8 then
		table.insert(alist, a8)
		table.insert(flist, f8)
	end
	if a9 and f9 then
		table.insert(alist, a9)
		table.insert(flist, f9)
	end
	if a10 and f10 then
		table.insert(alist, a10)
		table.insert(flist, f10)
	end
	if a11 and f11 then
		table.insert(alist, a11)
		table.insert(flist, f11)
	end
	if a12 and f12 then
		table.insert(alist, a12)
		table.insert(flist, f12)
	end

	GDS.ChooseEx(question, alist, flist)
end

function GDS.ChooseEx(question, alist, flist, rand)
	if not GDS.SetTarget("player") then return 0 end
	local script = GDS.target:GetScript()
	if script == nil then return 0 end

	local n = table.maxn(alist) 
	if type(alist) ~= "table" or type(flist) ~= "table" or 
		n == 0 or table.maxn(flist) ~= n then
		print("alist or flist not match")
		return
	end

	-- random answers if random choose
	local r = 0
	if rand then
		for i = 1, n do
			r = math.random(n)
			table.insert(alist, alist[r])
			talbe.remove(alist, r)
			table.insert(flist, flist[r])
			talbe.remove(flist, r)
		end
	end

	-- append empty string for C API call
	for i = n + 1, 12 do
		table.insert(alist, "")
	end

	--print(string.format("player choose total: %d", n))
	script:Choose(question, n, alist[1], alist[2], alist[3], alist[4], 
		alist[5], alist[6], alist[7], alist[8], alist[9], alist[10], 
		alist[11], alist[12])

	coroutine.yield()		-- thread pause

	local result = script:GetAnswer()
	--print(string.format("player answer: %d", result))
	if type(flist[result]) == "function" then
		(flist[result])()	-- run coresponding function
	end
end

function GDS.RChoose(question, a1, f1, a2, f2, a3, f3, a4, f4, a5, f5, a6, f6,
					a7, f7, a8, f8, a9, f9, a10, f10, a11, f11, a12, f12)
	GDS.Choose(question, a1, f1, a2, f2, a3, f3, a4, f4, a5, f5, a6, f6, 
				a7, f7, a8, f8, a9, f9, a10, f10, a11, f11, a12, f12, true)
end

function GDS.RChooseEx(question, alist, flist)
	GDS.ChooseEx(question, alist, flist, true)
end

function GDS.SetChooseImage(npc_id)
	-- useless
	if not GDS.SetTarget("player") then return 0 end
end

function GDS.Say(text)
	if not GDS.SetTarget("player") then return 0 end
	local script = GDS.target:GetScript()
	if script == nil then return 0 end
	script:Say(text, 0, true, 0)
end

function GDS.UnitSay(name, text, image, left, npc_id)
	if not GDS.SetTarget(name) then return 0 end
	if image == nil then image = "npc" end
	if left == nil then left = true end
	if npc_id == nil then npc_id = npc:GetNpc_id() end

	local imageType
	if image == "npc" then
		imageType = 0
	elseif image == "player" then
		imageType = 1
	elseif party and image == "party1" then
		imageType = 1
	elseif party and image == "party2" then
		imageType = 2
	elseif party and image == "party3" then
		imageType = 3
	elseif party and image == "party4" then
		imageType = 4
	elseif party and image == "party5" then
		imageType = 5
	else
		imageType = 99
	end

	--print(string.format("unit say: %s, %s, imageType %d, npc_id %d", 
	--	LuaI.ConvertANSII(npc:GetName()), LuaI.ConvertANSII(text), 
	--	imageType, npc_id))
	if party and name == "team" then
		for i = 0, party:GetCount() - 1 do
			local member = party:GetMember(i)
			local script = member and member:GetScript()
			if script then
				script:Say(text, imageType, left, npc_id)
			end
		end
	else
		local script = GDS.target:GetScript()
		if script then
			script:Say(text, imageType, left, npc_id)
		end
	end
end

function GDS.ShowHint(name, text)
	if not GDS.SetTarget(name) then return 0 end
	local script = GDS.target:GetScript()
	if script == nil then return 0 end

	script:ShowHint(text)
end

function GDS.CallMount(mountId)
	if not GDS.SetTarget("player") then return 0 end
	GDS.target:SetPet_id(mountId)
end

function GDS.CalcTimeMark(name, markId, minute)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetQuests():TimeMarkOver(markId, minute)
end

function GDS.Rumor(text, toWorld)
	if not GDS.SetTarget("player") then return end
	GDS.target:ShowRumor(text, toWorld)
end

function GDS.Rank(type)
	if not GDS.SetTarget("player") then return end
	GDS.target:ShowRank(type)
end

function GDS.AddQuest(name, quest_id, quest_name, type_id, type_name)
	print(string.format("AddQuest: quest_id %d, type_id %d", quest_id, type_id))
	--if not GDS.SetTarget(name) then return 0 end
	GDS.SetTarget(name)
	if GDS.target == nil then
		print("GDS Warning: GDS target is nil!")
	end
	return GDS.target:GetQuests():AddQuest(quest_id, quest_name, type_id, type_name)
end

function GDS.GetQuest(name, quest_id)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetQuests():GetQuest(quest_id)
end

function GDS.RemoveQuest(name, quest_id)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetQuests():RemoveQuest(quest_id)
end

function GDS.SetRoundInfo(name, type_id, maxRound, limitRound, limitType)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetQuests():SetRoundInfo(type_id, maxRound, limitRound, 
			limitType)
end

function GDS.SetRound(name, type_id, round)
	if not GDS.SetTarget(name) then return 0 end
	local qType = GDS.target:GetQuests():GetType(type_id)
	if qType then
		qType:SetRound(round)
	end
end

function GDS.SetTotalRound(name, type_id, round)
	if not GDS.SetTarget(name) then return 0 end
	local qType = GDS.target:GetQuests():GetType(type_id)
	if qType then
		qType:SetTotalRound(round)
	end
end

function GDS.AddTotalRound(name, type_id, round)
	if not GDS.SetTarget(name) then return 0 end
	local qType = GDS.target:GetQuests():GetType(type_id)
	if qType then
		qType:ChangeTotalRound(round)
	end
end

function GDS.AddRound(name, type_id, round)
	if not GDS.SetTarget(name) then return 0 end
	local qType = GDS.target:GetQuests():GetType(type_id)
	if qType then
		qType:ChangeRound(round)
	end
end

function GDS.RemoveTotalRound(name, type_id, round)
	if not GDS.SetTarget(name) then return 0 end
	local qType = GDS.target:GetQuests():GetType(type_id)
	if qType then
		qType:ChangeTotalRound(-round)
	end
end

function GDS.RemoveRound(name, type_id, round)
	if not GDS.SetTarget(name) then return 0 end
	local qType = GDS.target:GetQuests():GetType(type_id)
	if qType then
		qType:ChangeRound(-round)
	end
end

function GDS.UpdateRound(name, type_id)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetQuests():Update()
end

function GDS.GetMaxRound(name, type_id)
	if not GDS.SetTarget(name) then return 0 end
	local qType = GDS.target:GetQuests():GetType(type_id)
	return (qType and qType:GetMaxRound() or 0)
end

function GDS.GetLimitRound(name, type_id)
	if not GDS.SetTarget(name) then return 0 end
	local qType = GDS.target:GetQuests():GetType(type_id)
	return (qType and qType:GetLimitRound() or 0)
end

function GDS.GetLimitType(name, type_id)
	if not GDS.SetTarget(name) then return 0 end
	local qType = GDS.target:GetQuests():GetType(type_id)
	return (qType and qType:GetLimitType() or 0)
end

function GDS.GetRound(name, type_id)
	if not GDS.SetTarget(name) then return 0 end
	local qType = GDS.target:GetQuests():GetType(type_id)
	return (qType and qType:GetRound() or 0)
end

function GDS.GetTotalRound(name, type_id)
	if not GDS.SetTarget(name) then return 0 end
	local qType = GDS.target:GetQuests():GetType(type_id)
	return (qType and qType:GetTotalRound() or 0)
end

function GDS.GenerateNPC(npc_id, map_id, x, y, public, lifeTime)
	if not GDS.SetTarget("player") then return 0 end
	local script = GDS.target:GetScript()
	if script == nil then return 0 end

	return script:GenerateNPC(npc_id, map_id, x, y, public, lifeTime)
end

function GDS.Suicide()
	npc:Destroy()
end

function GDS.SpecBattle(npc_id)
	if not GDS.SetTarget("player") then return 0 end
	GDS.target:EncounterBattle(npc_id)
end

function GDS.GetFlag(name, flag_id)
	if not GDS.SetTarget(name) then return nil end
	if not GDS.target:GetQuests():HasFlag(flag_id) then
		return nil
	end
	return GDS.target:GetQuests():GetFlag(flag_id)
end

function GDS.RemoveThisItem()
	if item then
		return item:ReduceCount()
	else
		return false
	end
end

function GDS.SetItemValue(id, value)
	if id == 1 then
		item:SetItemValue1(value)
	elseif id == 2 then
		item:SetItemValue2(value)
	elseif id == 3 then
		item:SetItemValue3(value)
	end
end

function GDS.GetItemValue(id)
	if id == 1 then
		return item:GetItemValue1()
	elseif id == 2 then
		return item:GetItemValue2()
	elseif id == 3 then
		return item:GetItemValue3()
	else
		return 0
	end
end

function GDS.GetUseItemTarget()
	if not GDS.SetTarget("target") then return 0 end
	return GDS.target:GetName()
end

function GDS.GetItemUserTypeId()
	if not GDS.SetTarget("target") then return 0 end
	return GDS.target:GetUserType()
end

function GDS.ResetPartner(name)
	if not GDS.SetTarget(name) then return 0 end
	if GDS.target:GetUserType() == 2 then
		GDS.target:ResetPartner()
	end
end

function GDS.GetMobPartnerType()
	if not GDS.SetTarget("target") then return 0 end
	if GDS.target:GetUserType() == 2 then
		return GDS.target:GetPartnerType()
	else
		return 0
	end
end

function GDS.GetMobReqLV()
	if not GDS.SetTarget("target") then return end
	if GDS.target:GetUserType() == 2 then
		return GDS.target:GetReqLv()
	else
		return 0
	end
end

function GDS.AddPartnerSkill(name, skill_id)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetUserType() == 2 then
		GDS.target:AddSkill(skill_id)
	end
end

function GDS.ReplacePartnerSkill(name, skill_id, skillType)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetUserType() == 2 and not GDS.target:HasSkill(skill_id) then
		local old_id = GDS.target:GetRandomSkillID(skillType)
		GDS.target:RemoveSkill(old_id)
		GDS.target:AddSkill(skill_id)
	end
end

function GDS.ReversPoint(name, unit_id, value)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetUserType() == 1 then
		GDS.target:ReverseAttrib(unit_id, value)
	end
end

function GDS.AddPet(name, pet_id)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetPets():AddPet(pet_id)	
end

function GDS.RemovePet(name, pet_id)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:GetPets():RemovePet(pet_id)	
end

function GDS.SetMaxPet(name, num)
	if not GDS.SetTarget(name) then return 0 end
	GDS.target:SetMaxPet(num)
end

function GDS.GetMaxPet(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetMaxPet()
end

function GDS.GetPetType()
	if not GDS.SetTarget("player") then return 0 end
	if GDS.target:GetCurPet() == nil then return 0 end
	return GDS.target:GetCurPet():GetPetType()
end

function GDS.GetPetTotalCount(name)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetPets():GetCount()
end

function GDS.GetPetCount(name, pet_id)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetPets():GetIdCount(pet_id)
end

function GDS.GetPetID(name)
	if not GDS.SetTarget(name) then return 0 end
	if GDS.target:GetCurPet() == nil then return 0 end
	return GDS.target:GetCurPet():GetPet_id()
end

function GDS.GetPetLv(name)
	if not GDS.SetTarget(name) then return 0 end
	if GDS.target:GetCurPet() == nil then return 0 end
	return GDS.target:GetCurPet():GetLevel()
end

function GDS.GetPetExp(name)
	if not GDS.SetTarget(name) then return 0 end
	if GDS.target:GetCurPet() == nil then return 0 end
	return GDS.target:GetCurPet():GetExp()
end

function GDS.GetPetVP(name)
	if not GDS.SetTarget(name) then return 0 end
	if GDS.target:GetCurPet() == nil then return 0 end
	return GDS.target:GetCurPet():GetVP()
end

function GDS.GetPetHappiness(name)
	if not GDS.SetTarget(name) then return 0 end
	if GDS.target:GetCurPet() == nil then return 0 end
	return GDS.target:GetCurPet():GetHappiness()
end

function GDS.GetPetSpirit(name)
	if not GDS.SetTarget(name) then return 0 end
	if GDS.target:GetCurPet() == nil then return 0 end
	return GDS.target:GetCurPet():GetSpirit()
end

function GDS.AddPetLv(name, level)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():ChangeLevel(level)
end

function GDS.AddPetExp(name, exp)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():ChangeExp(exp)
end

function GDS.AddPetVP(name, vp)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():ChangeVP(vp)
end

function GDS.AddPetHappiness(name, happy)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():ChangeHappiness(happy)
end

function GDS.AddPetSpirit(name, spirit)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():ChangeSpirit(spirit)
end

function GDS.RemovePetLv(name, level)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():ChangeLevel(-level)
end

function GDS.RemovePetExp(name, exp)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():ChangeExp(-exp)
end

function GDS.RemovePetVP(name, vp)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():ChangeVP(-vp)
end

function GDS.RemovePetHappiness(name, happy)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():ChangeHappiness(-happy)
end

function GDS.RemovePetSpirit(name, spirit)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():ChangeSpirit(-spirit)
end

function GDS.AddPetSkill(name, skill_id)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():AddSkill(skill_id)
end

function GDS.ResetPet(name)
	if not GDS.SetTarget(name) then return end
	if GDS.target:GetCurPet() == nil then return end
	GDS.target:GetCurPet():ResetPet()
end

function GDS.AddTitle(name, title)
	if not GDS.SetTarget(name) then return end
	GDS.target:AddTitle(title)
end

function GDS.RemoveTitle(name, title)
	if not GDS.SetTarget(name) then return end
	GDS.target:RemoveTitle(title)
end

function GDS.CanAddItem(name, bagIndex, item_id, count)
	if not GDS.SetTarget(name) then return 0 end
	return GDS.target:GetItems():CanAddItem(bagIndex, item_id, count)
end

function GDS.PlaySound(soundName)
	if not GDS.SetTarget("player") then return end
	GDS.target:PlaySound(soundName)
end
