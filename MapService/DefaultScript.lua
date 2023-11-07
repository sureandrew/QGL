
-- ///////////////  Private Data  /////////////// -- 
-- Data share only with the "DefaultScript.lua" 
local LuaOwner;

local OBJECT_OWNER_UID = 0;
local ITEM_INSTANCE_UID = 0;

local DefaultScript = {
  						SetOwner,
  						Suicide,
  						UnitExists,
  						MapExists,
  						
  						CalcPercent,
  						
  						Say,
  						UnitSay,
  						Rumor,
  						ShowHint,
  						
						Battle,
						SpecBattle,

						SetChooseImage,
  						ChooseEx,
  						Choose,
  						RChoose,
  						PChoose,
  						
  						OpenLuaFile,
  						OpenInterface,
  						  						
  						JumpMap,
  						
						CallMount,
  						GenerateNPC,
  						IsValidBlock,
  						
  						CalcTimeMark,
  						
						UpdateQuest,
						FlagExists,
						TypeExists,
						UpdateRound,
  						
  						IsNight,
  						IsLeader,
  						GetGameTime,
  						
  						GetMemberNum,

  						GetUnitStatus,
  						GetLevel,
  						GetJob,
  						GetSex,
  						
  						GetHP,
  						GetSP,
  						GetDP,
  						GetVP,
  						GetWC,
  						
  						GetFaction,
  						GetUnitArray,
  						GetPKPoint,
  						
  						GetPartnerTotalCount,
  						GetPartnerCount,

						GetPetID,
  						GetPetTotalCount,
  						GetPetCount,
						GetPetSpirit,
						GetPetLv,
						GetPetExp,
						GetPetVP,
						GetPetHappiness,

  						
  						GetCharID,
  						
  						GetExp,
  						GetMoney,
  						
  						GetGoodness,
  						GetHonor,
  						
  						GetDonate,
  						
  						GetGameMoney,
  						GetBankMoney,
  						
						GetPlayerName,
						GetMobName,
						GetNpcName,
						GetMapName,
						GetTypeName,
						GetItemName,
						GetUseItemTarget,
						
						GetItemUserTypeId,
						GetMobPartnerType,
						GetPetType,
						
						GetLineId,
						GetMapId,
						GetMapX,
						GetMapY,
						
						GetItemCount,
						GetFreeSlot,
						GetTotalFreeSlot,
						GetItemRootId,
						
						GetItemValue,
						
						GetSkillLevel,
      					GetTeamLevel,
      					GetTeamJob,
  						GetTeamSex,
						
						GetMaxPartner,
						GetMaxPet,
						
						GetMobReqLV,
						
						GetPartnerSkillCount,
						HasPartnerSkill,
						
						HasEquiped,
						
						SkillExists,
						BuffExists,
						
						GetQuest,
						GetFlag,
						GetTimeMark,
										
						GetMaxRound,
						GetLimitRound,
						GetLimitType,
						GetRound,
						GetTotalRound,
  						
  						AddQuest,
  						AddFlag,
  						AddRound,
  						AddTotalRound,
  						
  						AddPartner,
  						AddPet,
  						AddItem,
  						AddItemFromCSV,
  						AddUnitStatus,
  						AddLevel,
  						
  						AddHP,
  						AddSP,
  						AddDP,
  						AddVP,
						AddWC,
						
						AddPKPoint,
						
						AddPartnerHealth,
						AddPartnerFeel,
  						
  						AddMoney,
  						AddExp,
  						AddHonor,
  						AddGoodness,
  						
  						AddGameMoney,
  						AddBankMoney,
  						
  						AddDonate,
  						
  						AddPartnerExp,
						AddPartnerSkill,
						AddPetExp,
						AddPetSkill,
						AddPetLv,
  						
  						AddSkill,
  						AddSkillLevel,
						
  						AddBuff,
  						
  						SetFlag,
  						SetRoundInfo,
  						SetRound,
  						SetTotalRound,
  						
  						SetReborn,
  						
  						SetLevel,
  						
  						SetFaction,
  						
  						SetMoney,
  						SetExp,
  						
  						SetFullHP,
  						SetFullSP,
  						SetFullVP,
  						
						SetFullPHP,
						SetFullPSP,
  						
  						SetPKPoint,
						PVPEnable,
						
						SetTimeMark,
						
						SetItemValue,
						
						SetMaxPartner,
						SetMaxPet,
						
						ReplacePartnerSkill,
						
						ResetPartner,
						ResetPet,
						
						ReversPoint,

  						RemoveItem,
  						RemovePartner,
  						RemovePet,
  						
  						RemoveUnitStatus,
  						
  						RemoveLevel,

						RemoveMoney,
						RemoveExp,
						RemoveHonor,
						RemoveGoodness,
						
						RemoveDonate,
						
						RemoveGameMoney,
						RemoveBankMoney,
						
						RemovePartnerExp,
						RemovePetExp,

  						RemoveHP,
  						RemoveSP,
  						RemoveDP,
  						RemoveVP,
						RemoveWC,
						
						RemovePKPoint,
						
						RemovePartnerHealth,
						RemovePartnerFeel,
						
						RemoveSkill,
						RemoveSkillLevel,
						
						RemoveBuff,
						
						RemoveFlag,
						RemoveQuest,
						
						RemoveRound,
						RemoveTotalRound,
						
						RemoveTimeMark,
						
						RemoveThisItem,
						
						AddSpeakCount,
						
						GetSpeakCount,
						
						Rank,
						
						AddPetVP,
						AddPetHappiness,
						AddPetSpirit,
						RemovePetVP,
						RemovePetHappiness,
						RemovePetSpirit,
						
						RemovePetLv,
						
						AddTitle,
						RemoveTitle,
						
						CanAddItem,

						PlaySound,
};

--  // Unit Status UInt16 Ref Num
local UNIT_STATUS_REFNUM_LEVEL = 12;
local UNIT_STATUS_REFNUM_JOB = 13;
local UNIT_STATUS_REFNUM_SEX = 14;

local UNIT_STATUS_REFNUM_HP = 15;
local UNIT_STATUS_REFNUM_MHP = 16;
local UNIT_STATUS_REFNUM_MAXHP = 17;
local UNIT_STATUS_REFNUM_FULLHP = 18;

local UNIT_STATUS_REFNUM_SP = 19;
local UNIT_STATUS_REFNUM_MSP = 20;
local UNIT_STATUS_REFNUM_MAXSP = 21;
local UNIT_STATUS_REFNUM_FULLSP = 22;

local UNIT_STATUS_REFNUM_DP = 23;
local UNIT_STATUS_REFNUM_MAXDP = 24;
local UNIT_STATUS_REFNUM_FULLDP = 25;

local UNIT_STATUS_REFNUM_VP = 26;
local UNIT_STATUS_REFNUM_MAXVP = 27;
local UNIT_STATUS_REFNUM_FULLVP = 28;

local UNIT_STATUS_REFNUM_WC = 29;
local UNIT_STATUS_REFNUM_WCEXP = 30;
local UNIT_STATUS_REFNUM_MAXWC = 31;

local UNIT_STATUS_REFNUM_FACTION = 32;
local UNIT_STATUS_REFNUM_CURRARRAY = 33;
local UNIT_STATUS_REFNUM_PKPOINT = 34;
local UNIT_STATUS_REFNUM_PKENABLE = 35;

local UNIT_STATUS_REFNUM_PARTNERTOTAL = 36;
local UNIT_STATUS_REFNUM_PARTNERHEALTH = 37;
local UNIT_STATUS_REFNUM_PARTNERFEEL = 38;
local UNIT_STATUS_REFNUM_PARTNERFULLHP = 39;
local UNIT_STATUS_REFNUM_PARTNERFULLSP = 40;

local UNIT_STATUS_REFNUM_MAPX = 41;
local UNIT_STATUS_REFNUM_MAPY = 42;
local UNIT_STATUS_REFNUM_MAPID = 43;

local UNIT_STATUS_REFNUM_BUFFID = 44;

local UNIT_STATUS_REFNUM_LINEID = 45;

local UNIT_STATUS_REFNUM_RESETPARTNER = 46;

local UNIT_STATUS_REFNUM_SPEAKCOUNT = 47;

local UNIT_STATUS_REFNUM_RESETPET = 48;

--  // Unit Status UInt32 Ref Num
local UNIT_STATUS_REFNUM_MONEY = 101;
local UNIT_STATUS_REFNUM_EXP = 102;

--local UNIT_STATUS_REFNUM_GOODNESS = 103;
local UNIT_STATUS_REFNUM_HONOR = 103;

local UNIT_STATUS_REFNUM_FDONATE = 104;
local UNIT_STATUS_REFNUM_TFDONATE = 105;
local UNIT_STATUS_REFNUM_MDONATE = 106;
local UNIT_STATUS_REFNUM_TMDONATE = 107;

local UNIT_STATUS_REFNUM_GAMEMONEY = 108;
local UNIT_STATUS_REFNUM_BANKMONEY = 109;

local UNIT_STATUS_REFNUM_CHARID = 110;

local UNIT_STATUS_REFNUM_PARTNEREXP = 111;

local UNIT_STATUS_REFNUM_CHOOSEIMAGE = 112;

local UNIT_STATUS_REFNUM_MOBPARTNERTYPE = 113;

local UNIT_STATUS_REFNUM_PETTOTAL = 114;
local UNIT_STATUS_REFNUM_PETEXP = 115;
local UNIT_STATUS_REFNUM_PETTYPE = 116;
local UNIT_STATUS_REFNUM_PETVP = 117;
local UNIT_STATUS_REFNUM_PETHAPPINESS = 118;
local UNIT_STATUS_REFNUM_PETSPIRIT = 119;
local UNIT_STATUS_REFNUM_PETLV = 120;
local UNIT_STATUS_REFNUM_PETID = 121;

-- // Quest Attribute Ref Num
local QUEST_ATTRI_REFNUM_MAINTYPEID = 1;
local QUEST_ATTRI_REFNUM_ITEMID = 2;
local QUEST_ATTRI_REFNUM_ITEMMAXNUM = 3;
local QUEST_ATTRI_REFNUM_PARTNERID = 4;
local QUEST_ATTRI_REFNUM_PARTNERMAXNUM = 5;
local QUEST_ATTRI_REFNUM_NPCID_1 = 6;
local QUEST_ATTRI_REFNUM_NPCID_2 = 7;

local QUEST_ATTRI_REFNUM_MOBNUM_1 = 8;
local QUEST_ATTRI_REFNUM_MOBNUM_2 = 9;
local QUEST_ATTRI_REFNUM_MOBNUM_3 = 10;

local QUEST_ATTRI_REFNUM_MOBMAX_1 = 11;
local QUEST_ATTRI_REFNUM_MOBMAX_2 = 12;
local QUEST_ATTRI_REFNUM_MOBMAX_3 = 13;

local QUEST_ATTRI_REFNUM_MOBID_1 = 14;
local QUEST_ATTRI_REFNUM_MOBID_2 = 15;
local QUEST_ATTRI_REFNUM_MOBID_3 = 16;
                                
local QUEST_ATTRI_REFNUM_ENCOUNTNUM = 17;
local QUEST_ATTRI_REFNUM_ENCOUNTMAX = 18;

local QUEST_ATTRI_REFNUM_ARRIVEMAP_1 = 19;
local QUEST_ATTRI_REFNUM_ARRIVEMAP_2 = 20;

local QUEST_ATTRI_REFNUM_NEWBIENUM = 21;
local QUEST_ATTRI_REFNUM_NEWBIEMAX = 22;

local QUEST_ATTRI_REFNUM_PETID = 23;
local QUEST_ATTRI_REFNUM_PETMAXNUM = 24;

-- //////////////  Local Function  ////////////////// --
local function L_MatchString(m, s1, s2, s3, s4)
	if m == 1 then
	    return s1;
	elseif m == 2 then
	    return s2;
	elseif m == 3 then
		return s3;
	elseif m == 4 then
	    return s4;
	else
	    return nil;
	end
end

local function NewGameQuestClass(questclasstype)
	local mt = {
		__metatable = questclasstype;--GameQuestSys;
		__index = questclasstype;--GameQuestSys;
	};
	local tempquest = {};
	setmetatable(tempquest, mt);
	return tempquest;
end

-- //////////////  Local Quest Data Structure  ////////////// --
local GameQuestSys ={
						m_pQuest,
						Assign,						
						ResetData,
						UpdateData,
						
						SetMainInfo,
						SetQuestName,
						SetDescription,
						SetFail,
						SetFailCondition,
						SetCancelEnable,
						SetTimeLimit,
						SetStartNPC,
						SetEndNPC,
						SetTargetNPC,
						FindTargetNPC,
						GetTargetNPC,
						SetTargetItem,
						SetTargetMOB,
						SetTargetPartner,
						SetTargetPet,
						SetTargetNewBie,
						
						SetEncountNPC,
						SetTargetMap,
						ArriveTargetMap,
						
						GetTypeId,
						GetTargetItemId,
						GetTargetItemMax,

						GetTargetPartnerId,
						GetTargetPartnerMax,
						
						GetTargetPetId,
						GetTargetPetMax,
						
						GetTargetMobNum,
						GetTargetMobMax,
						GetTargetMobId,
						
						GetEncountNum,
						GetEncountMax,
						
						GetTargetNpcId,
						GetTargetNewBieNum,
						GetTargetNewBieMax,
						
						IsFailed,
						IsOverTime,
						
};

function GameQuestSys:Assign(ptr)
	self.m_pQuest = ptr;
end

function GameQuestSys:ResetData()
     if self.m_pQuest ~= nil then
		print("Do Reset Quest Data Command");
		self.m_pQuest:LuaResetQuestData();
	 end
end

function GameQuestSys:UpdateData()
    if self.m_pQuest ~= nil then
		print("Do Update Quest Data To DB Command");
        self.m_pQuest:LuaUpdateQuestData();
	end
end

function GameQuestSys:SetMainInfo(id, name, typeId, typeName)
end

function GameQuestSys:SetQuestName(name)
	if self.m_pQuest ~= nil then
		if ( (name~=nil) and (type(name)=="string") ) then
			print("Do Set Quest Name Command");
			self.m_pQuest:LuaSetQuestName(name);
		end
	end
end

function GameQuestSys:SetDescription(descript)
	if self.m_pQuest ~= nil then
		if ( (descript ~= nil) and (type(descript) == "string") ) then
		    print("Do Set Quest Description Command");
		    self.m_pQuest:LuaSetQuestDescription(descript);
		end
	end
end

function GameQuestSys:SetFail(bfail)
    if self.m_pQuest ~= nil then
        if ( (bfail ~= nil) and (type(bfail) == "boolean") ) then
            print("Do Set Quest Failed Command");
            self.m_pQuest:LuaSetFailed(bfail);
        end
    end
end

function GameQuestSys:SetFailCondition(failconditon)
	if self.m_pQuest ~= nil then
		if ( (failconditon ~= nil) and (type(failconditon) == "string") ) then
		    print("Do Set Quest Fail Condition");
		    self.m_pQuest:LuaSetQuestFailCondition(failconditon);
		end
	end
end

function GameQuestSys:SetCancelEnable(bCancel)
    if self.m_pQuest ~= nil then
        if ( (bCancel ~= nil) and (type(bCancel) == "boolean") ) then
			print("Do Set Quest Can Be Cancel Command");
        	self.m_pQuest:LuaSetQuestCancelEnable(bCancel);
        end
	end
end

function GameQuestSys:SetStartNPC(npcId, mapId, x, y)
    if self.m_pQuest ~= nil then
        if ((npcId ~= nil) and (mapId ~= nil) and (x ~= nil) and (y ~= nil) ) then
            if ( (type(npcId) == "number") and (type(mapId) == "number") and (type(x) == "number") and (type(y) == "number") ) then
				print("Do Set Quest Start NPC Command");
				self.m_pQuest:LuaSetQuestStartNPC(npcId, mapId, x, y);
			end
        end
    end
end

function GameQuestSys:SetEndNPC(npcId, mapId, x, y)
   if self.m_pQuest ~= nil then
        if ((npcId ~= nil) and (mapId ~= nil) and (x ~= nil) and (y ~= nil) ) then
            if ( (type(npcId) == "number") and (type(mapId) == "number") and (type(x) == "number") and (type(y) == "number") ) then
                print("Do Set Quest End NPC Command");
				self.m_pQuest:LuaSetQuestEndNPC(npcId, mapId, x, y);
			end
        end
    end
end

function GameQuestSys:SetTargetNPC(orderNum, npcId, mapId, x, y)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (npcId ~= nil) and (mapId ~= nil) and (x ~= nil) and (y ~= nil) ) then
            if ( (type(orderNum) == "number") and (type(mapId) == "number") and (type(x) == "number") and (type(y) == "number") ) then
				if (type(npcId) == "number") then
                    print("Do Set Quest Target NPC Command");
                	self.m_pQuest:LuaSetQuestTargetNPC(orderNum, npcId, mapId, x, y);
				elseif (type(npcId) == "string") then
				    print("Do Set Random Quest NPC Command");
				    self.m_pQuest:LuaSetRandomQuestNPC(orderNum, npcId, mapId, x, y);
				end
            end
        end
    end
end

function GameQuestSys:FindTargetNPC(orderNum)
    if self.m_pQuest ~= nil then
		if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
		    print("Do Set Find Target NPC Success Command");
		    self.m_pQuest:LuaFindTargetNPC(orderNum);
		end
	end
end

function GameQuestSys:GetTargetNPC(orderNum)
    if self.m_pQuest ~= nil then
		if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
		    print("Do Get Target NPC Success Command");
		    return self.m_pQuest:LuaGetTargetNPC(orderNum);
		end
	end
	return false
end

function GameQuestSys:SetTargetItem(orderNum, itemId, maxCount, infoT1, infoV1, infoT2, infoV2)
    if self.m_pQuest ~= nil then
		if ( (orderNum ~= nil) and (itemId ~= nil) and (maxCount ~= nil) and (infoT1 ~= nil) and (infoV1 ~= nil ) and (infoT2 ~= nil) and (infoV2 ~= nil) ) then
            if ( (type(orderNum) == "number") and (type(maxCount) == "number") and (type(infoT1) == "number") and (type(infoV1) == "number") and (type(infoT2) == "number") and (type(infoV2) == "number") ) then
				if ( type(itemId) == "number" ) then
				    print("Do Set Quest Target Item Command");
               		self.m_pQuest:LuaSetQuestTargetItem(orderNum, itemId, maxCount, infoT1, infoV1, infoT2, infoV2);
				elseif ( type(itemId) == "string" ) then
				    print("Do Set Quest Target Item By Random Command");
					self.m_pQuest:LuaSetRandomQuestItem(orderNum, itemId, maxCount, infoT1, infoV1, infoT2, infoV2);
				end
			end
			--[[
			if ( (type(orderNum) == "number") and (type(itemId) == "number") and (type(maxCount) == "number") and (type(infoT1) == "number") and (type(infoV1) == "number") and (type(infoT2) == "number") and (type(infoV2) == "number") ) then
				print("Do Set Quest Target Item Command");
                self.m_pQuest:LuaSetQuestTargetItem(orderNum, itemId, maxCount, infoT1, infoV1, infoT2, infoV2);
			end
			]]
		end
	end
end

function GameQuestSys:SetTargetMOB(orderNum, mobId, maxCount)
    if self.m_pQuest ~= nil then
		if ( (orderNum ~= nil) and (mobId ~= nil) and (maxCount ~= nil) ) then
			if ( (type(orderNum) == "number" ) and (type(maxCount) == "number") ) then
			    if (type(mobId) == "number") then
			    	print("Do Set Target Mob Command");
			    	self.m_pQuest:LuaSetQuestTargetMOB(orderNum, mobId, maxCount);
				elseif ( type(mobId) == "string" ) then
				    print("Do Set Random Target Mob Command");
				    self.m_pQuest:LuaSetRandomQuestMOB(orderNum, mobId, maxCount);
			    end
			end
		end
	end
end

function GameQuestSys:SetTargetPartner(orderNum, mobId, maxCount, infoT1, infoV1, infoT2)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (mobId ~= nil) and (maxCount ~= nil) and (infoT1 ~= nil) and (infoV1 ~= nil) and (infoT2 ~= nil) ) then
            if ( (type(orderNum) == "number") and (type(maxCount) == "number") and (type(infoT1) == "number") and (type(infoV1) == "number") and (type(infoT2) == "number") ) then
				if (type(mobId) == "number") then
					print("Do Set Quest Target Partner Command");
					self.m_pQuest:LuaSetQuestTargetPartner(orderNum, mobId, maxCount, infoT1, infoV1, infoT2);
				elseif (type(mobId) == "string") then
				    print("Do Set Random Quest Target Partner Command");
				    self.m_pQuest:LuaSetRandomQuestPartner(orderNum, mobId, maxCount, infoT1, infoV1, infoT2);
				end
			end
        end
    end
end

function GameQuestSys:SetTargetPet(orderNum, mobId, maxCount, infoT1, infoV1, infoT2)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (mobId ~= nil) and (maxCount ~= nil) and (infoT1 ~= nil) and (infoV1 ~= nil) and (infoT2 ~= nil) ) then
            if ( (type(orderNum) == "number") and (type(maxCount) == "number") and (type(infoT1) == "number") and (type(infoV1) == "number") and (type(infoT2) == "number") ) then
				if (type(mobId) == "number") then
					print("Do Set Quest Target Pet Command");
					self.m_pQuest:LuaSetQuestTargetPet(orderNum, mobId, maxCount, infoT1, infoV1, infoT2);
				elseif (type(mobId) == "string") then
				    print("Do Set Random Quest Target Pet Command");
				    self.m_pQuest:LuaSetRandomQuestPet(orderNum, mobId, maxCount, infoT1, infoV1, infoT2);
				end
			end
        end
    end
end

function GameQuestSys:SetTargetNewBie(minlv, maxlv, num)
    if self.m_pQuest ~= nil then
		if ( (minlv ~= nil) and (maxlv ~= nil) and (num ~= nil) ) then
		    if ( (type(minlv) == "number") and (type(maxlv) == "number") and (type(num) == "number") ) then
				print("Do Set Quest Target New Bie Command");
                self.m_pQuest:LuaSetTargetNewBie(minlv, maxlv, num);
			end
		end
	end
end

function GameQuestSys:SetEncountNPC(npcId, mapId, maxCount, step, meettime)
    if self.m_pQuest ~= nil then
        if ( (npcId ~= nil) and (mapId ~= nil) and (maxCount ~= nil) and (step ~= nil) and (meettime ~= nil)) then
            if ( (type(mapId) == "number") and (type(maxCount) == "number") and (type(step) == "number") and (type(meettime) == "number")) then
				if (type(npcId) == "number") then
					print("Do Set Quest Encount NPC");
					self.m_pQuest:LuaSetEncountNPC(npcId, mapId, maxCount, step, meettime);
				elseif (type(npcId) == "string") then
				    print("Do Set Random Quest Encount NPC");
				    self.m_pQuest:LuaSetRandomEncountNPC(npcId, mapId, maxCount, step, meettime);
				end
			end
        end
    end
end

function GameQuestSys:SetTargetMap(orderNum, mapId, x, y)
	if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (mapId ~= nil) and (x ~= nil) and (y ~= nil) ) then
            if ( (type(orderNum) == "number") and (type(mapId) == "number") and (type(x) == "number") and (type(y) == "number") ) then
				print("Do Set Quest Target Map Command");
                self.m_pQuest:LuaSetTargetMap(orderNum, mapId, x, y);
            end
        end
    end
end

function GameQuestSys:ArriveTargetMap(orderNum)
	if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Arrive Quest Target Map Command");
           	self.m_pQuest:LuaArriveTargetMap(orderNum);
        end
    end
end

function GameQuestSys:GetTypeId()
	if self.m_pQuest ~= nil then
		print("Do Get Quest Main Type Id Command"); 
		return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_MAINTYPEID);
	end
	return 0;
end

function GameQuestSys:GetTargetItemId(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Target Item Id Command");
            return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_ITEMID);
        end
    end
    return 0;
end

function GameQuestSys:GetTargetItemMax(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Target Item Max Count Command");
            return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_ITEMMAXNUM);
        end
    end
    return 0;
end

function GameQuestSys:GetTargetPartnerId(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Target Partner Id Command");
            return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_PARTNERID);
        end
    end
    return 0;
end

function GameQuestSys:GetTargetPartnerMax(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Target Partner Max Count Command");
            return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_PARTNERMAXNUM);
		end
    end
    return 0;
end

function GameQuestSys:GetTargetPetId(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Target Pet Id Command");
            return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_PETID);
        end
    end
    return 0;
end

function GameQuestSys:GetTargetPetMax(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Target Pet Max Count Command");
            return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_PETMAXNUM);
		end
    end
    return 0;
end

function GameQuestSys:GetTargetMobNum(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Target Mob Current Count Command"); 
            if orderNum == 1 then
            	return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_MOBNUM_1);
			elseif orderNum == 2 then
				return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_MOBNUM_2);
			elseif orderNum == 3 then
				return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_MOBNUM_3);
			end        
		end
    end
    return 0;
end

function GameQuestSys:GetTargetMobMax(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Target Mob Max Count Command");
            if orderNum == 1 then
            	return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_MOBMAX_1);
			elseif orderNum == 2 then
				return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_MOBMAX_2);
			elseif orderNum == 3 then
				return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_MOBMAX_3);
			end   
		end
    end
    return 0;
end 

function GameQuestSys:GetTargetMobId(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Target Mob Id Command");
            if orderNum == 1 then
            	return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_MOBID_1);
			elseif orderNum == 2 then
				return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_MOBID_2);
			elseif orderNum == 3 then
				return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_MOBID_3);
			end   
		end
    end
    return 0;
end 

function GameQuestSys:GetEncountNum(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Encount Curr Count Command");
            return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_ENCOUNTNUM);
		end
    end
    return 0;
end

function GameQuestSys:GetEncountMax(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Encount Max Count Command");
            return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_ENCOUNTMAX);
		end
    end
    return 0;
end

function GameQuestSys:GetTargetNpcId(orderNum)
	if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            print("Do Get Quest Target Npc Id Command");
            if ( orderNum == 1) then
            	return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_NPCID_1);
        	elseif ( orderNum == 2 ) then
        	    return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_NPCID_2);
        	end
        end
    end
    return 0;
end

function GameQuestSys:GetTargetNewBieNum(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            if ( orderNum == 1) then
				print("Do Get Quest Target New Bie Number");
				return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_NEWBIENUM);
			end
		end
	end
	return 0;
end

function GameQuestSys:GetTargetNewBieMax(orderNum)
    if self.m_pQuest ~= nil then
        if ( (orderNum ~= nil) and (type(orderNum) == "number") ) then
            if ( orderNum == 1) then
				print("Do Get Quest Target New Bie Max Number");
				return self.m_pQuest:LuaGetQuestTargetAttri(QUEST_ATTRI_REFNUM_NEWBIEMAX);
			end
		end
	end
	return 0;
end

function GameQuestSys:IsFailed()
    if self.m_pQuest ~= nil then
		print("Do Check Quest Fail Command");
		return self.m_pQuest:LuaIsQuestFailed();
	end
	return true;
end

function GameQuestSys:IsOverTime()
    if self.m_pQuest ~= nil then
    	print("Do Check Quest OverTime Command");
    	return self.m_pQuest:LuaIsQuestOverTime();
    end
	return false;
end

function GameQuestSys:SetTimeLimit(limit)
    if self.m_pQuest ~= nil then
        if ( (limit ~= nil) and (type(limit) == "number") ) then
			print("Do Set Quest Time Limit Command");
			--self.m_pQuest:LuaSetTimeLimit(limit);
			self.m_pQuest:LuaSetQuestTimeLimit(limit);
        end
    end
end

-- //////////////  Default Script Command ////////////// --  

-- [ set lua owner ]
DefaultScript.SetOwner = function(ScriptSys, tempId, itemuid)
	LuaOwner = ScriptSys;
	OBJECT_OWNER_UID = tempId;
	ITEM_INSTANCE_UID = itemuid;
end

-- [ Suicide start ]
DefaultScript.Suicide = function()
	if ( LuaOwner ~= nil ) then
		print("Do Suicide Command");
		print("Suicide Npc Id : "..OBJECT_OWNER_UID);
		LuaOwner:LuaSuicide(OBJECT_OWNER_UID);
	end
end
-- [ Suicide end ]

-- [ check unit exist start ] 
DefaultScript.UnitExists = function(unitid)
	if LuaOwner == nil then
	 	return false;
	end
	print("Do Unit Exists Command");
	-- check nil
	if ( unitid == nil ) then
		print("unit id is nil");
		return false;
	end;
	-- check type
	if ( (type(unitid)) ~= "string" ) then
		print("unit id is not a string");
		return false;
	end
	
	
	return LuaOwner:LuaUnitExist(unitid);
end
-- [ check unit exist end ] 

-- [ Map exist start ]
DefaultScript.MapExists = function(mapid)
	if ( (mapid~=nil) and (type(mapid)=="number") ) then
		print("Do Map Exists Command");
		return LuaOwner:LuaMapExists(mapid);
	end
	return nil;
end
-- [ Map exist end ]

-- [ Calc percent start ]
DefaultScript.CalcPercent = function(perLst)
	if ( (perLst~=nil) and (type(perLst) == "table") ) then
		local num = table.getn(perLst);
		
		local total = 0;
		for i in pairs(perLst) do
			total = total + perLst[i]; 
		end
		
		if total > 0 then
			local r = math.random(1, total);
			for j in pairs(perLst) do
				local n2 = 0;
				for k = 1, j do
					n2 = n2 + perLst[k];
				end
		  
			    if r <= n2  then
					return j;	 	
			    end
			end
		end
	end
	return nil;
end
-- [ Calc percent end ]

-- [ say command start ]
DefaultScript.Say = function(text) 
	if ( (DefaultScript.UnitExists("player")) == true ) then
		if ( (text ~= nil) and (type(text)=="string") ) then
			print("Do Say Commond");
			LuaOwner:LuaSay("player", text, "npc");
		end
	end
end
-- [ say command end]

-- [ Unit say command start ]
DefaultScript.UnitSay = function(unitid, text, imagetype, bleft, npcid)
	local bvalid = false;
 	if ( (unitid~=nil) and (unitid == "team") ) then
 	    bvalid = true;
 	elseif ( (DefaultScript.UnitExists(unitid)) == true ) then
 	    bvalid = true;
	end
	if ( (bvalid == true) and (text ~= nil) and (type(text)=="string") ) then
		if ( (imagetype~=nil) and (bleft~=nil) ) then
			if ( (type(imagetype)=="string") and (type(bleft)=="boolean") ) then
				if ( (npcid~=nil) and (type(npcid)=="number") ) then
					print("Do Unit Say (S + Id Mode) Command");
					LuaOwner:LuaSay(unitid, text, imagetype, bleft, npcid);
				else
					print("Do Unit Say (S Mode) Command");
					LuaOwner:LuaSay(unitid, text, imagetype, bleft, 0);
	 			end
			end
		else
			print("Do Unit Say (N Mode) Command");
			LuaOwner:LuaSay(unitid, text, "npc");
		end
	end
end
-- [ Unit say command end ]

-- [ Rank command start ]
DefaultScript.Rank = function(rankType)
	if ((rankType ~= nil) and (type(rankType)=="number")) then
		print("Do Rank Command");
        LuaOwner:LuaRank(rankType);
	end
end
--Rank

-- [ Rumor command start ]
DefaultScript.Rumor = function(text, bShowTitle)
	if (text~=nil) and (type(text)=="string") then
		if (bShowTitle~=nil) and (type(bShowTitle)=="boolean") then
			print("Do Rumor Command");
            LuaOwner:LuaRumor(text, bShowTitle);
		end
	end
end
--Rumor
-- [ Rumor command end ]

-- [ Show hint start ]
DefaultScript.ShowHint = function(unitid, text)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (text ~= nil) and (type(text) == "string") ) then
			print("Do Show Hint Command");
			LuaOwner:LuaShowHint(unitid, text);
		end
	end
end
-- [ Show hint end ]

-- [ Battle command start ]
DefaultScript.Battle = function(NpcId)
    if ( (DefaultScript.UnitExists("player")) == true ) then
        if ( (NpcId ~= nil) and ((type(NpcId)) == "number") ) then
            print("Do Battle Command");
            --LuaOwner:LuaBattle(NpcId);
        	LuaOwner:LuaBattle(NpcId, 0);
		end
    end
end
-- [ Battle command end ]

-- [ Quest battle command start ]
DefaultScript.SpecBattle = function(NpcId)
	if ( (DefaultScript.UnitExists("player")) == true ) then
        if ( (NpcId ~= nil) and ((type(NpcId)) == "number") ) then
        	print("Do Quest Battle Command");
        	LuaOwner:LuaBattle(NpcId, OBJECT_OWNER_UID);
        end
    end
end
-- [ Quest battle command end ]

-- [ Set choose command start ]
DefaultScript.SetChooseImage = function(npcId)
	if (LuaOwner ~= nil) then
	    if ( (npcId~=nil) and (type(npcId)=="number") ) then
	        print("Do Set Choose Image Command");
            LuaOwner:LuaSetUnitStatusU32("player", UNIT_STATUS_REFNUM_CHOOSEIMAGE, npcId);
	    end
	end
end
-- [ Set choose command end ]

-- [ ChooseEx command start ]
DefaultScript.ChooseEx = function(Question, slist, flist)
   	if LuaOwner == nil then
	 	return;
	end
	-- check nil
	if ( (slist == nil) or (flist == nil) ) then
	    return;
	end
	-- check type
	if ( (type(slist) ~= "table") or (type(flist) ~= "table") ) then
		return;
	end
	
	local ntotal = table.getn(slist);
	if ( ntotal == 0 ) then
	    return;
	elseif ( ntotal ~= table.getn(flist) ) then
	    return;
	end

	print("Do ChooseEx Command");

 	local bvalid = false;
 	
  	if ntotal == 1 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1]);
	elseif ntotal == 2 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1], slist[2]);
	elseif ntotal == 3 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1], slist[2], slist[3]);
	elseif ntotal == 4 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1], slist[2], slist[3], slist[4]);
	elseif ntotal == 5 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1], slist[2], slist[3], slist[4], slist[5]);
	elseif ntotal == 6 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1], slist[2], slist[3], slist[4], slist[5], slist[6]);
	elseif ntotal == 7 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1], slist[2], slist[3], slist[4], slist[5], slist[6], slist[7]);
	elseif ntotal == 8 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1], slist[2], slist[3], slist[4], slist[5], slist[6], slist[7], slist[8]);
	elseif ntotal == 9 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1], slist[2], slist[3], slist[4], slist[5], slist[6], slist[7], slist[8], slist[9]);
	elseif ntotal == 10 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1], slist[2], slist[3], slist[4], slist[5], slist[6], slist[7], slist[8], slist[9], slist[10]);
	elseif ntotal == 11 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1], slist[2], slist[3], slist[4], slist[5], slist[6], slist[7], slist[8], slist[9], slist[10], slist[11]);
	elseif ntotal == 12 then
		bvalid = LuaOwner:LuaChooseEx(Question, slist[1], slist[2], slist[3], slist[4], slist[5], slist[6], slist[7], slist[8], slist[9], slist[10], slist[11], slist[12]);
	end
		
	if ( bvalid == false ) then
		return;
	end
		
	coroutine.yield();
	print("Resume ChooseEx Command");
	local result = LuaOwner:GetChooseResult();
	print(string.format("player choose %d", result));
	 
	local bresumesucess = false;
	if (result > 0) and (result <= ntotal) then  
		local cf = flist[result];
		if ((cf ~= nil) and (type(cf) =="function")) then
			cf();
			bresumesucess = true;
		end 
	end
	
	if bresumesucess == false then
		LuaOwner:LuaExitScript("Resume ChooseEx Result Failed");
	end
end
-- [ ChooseEx command end ]

-- [ Choose command start ]
DefaultScript.Choose = function(Question, C1, R1, C2, R2, C3, R3, C4, R4, C5, R5, C6, R6, C7, R7, C8, R8, C9, R9, C10, R10, C11, R11, C12, R12)
    if LuaOwner == nil then
	 	return;
	end
    print("Do Choose Command");
    
    -- check type of string
	if ( (type(Question) ~= "string") or (Question == nil) ) then
		print("Question is nil or not a string");
	elseif ( (C1 == nil) or (type(C1) ~= "string") ) then
		print("First choose is nil or is not a string");
	elseif ( (C2 ~= nil) and (type(C2) ~= "string")) then
		print("Second choose is not a string");
	elseif ( (C3 ~= nil) and (type(C3) ~= "string") ) then
		print("Third choose is not a string");
	elseif ( (C4 ~= nil) and (type(C4) ~= "string") ) then
		print("Fourth choose is not a string");
	elseif ( (C5 ~= nil) and (type(C5) ~= "string") ) then
		print("Fifth choose is not a string");
    elseif ( (C6 ~= nil) and (type(C6) ~= "string") ) then
		print("6th choose is not a string");
	elseif ( (C7 ~= nil) and (type(C7) ~= "string") ) then
		print("7th choose is not a string");
    elseif ( (C8 ~= nil) and (type(C8) ~= "string") ) then
		print("8th choose is not a string");
    elseif ( (C9 ~= nil) and (type(C9) ~= "string") ) then
		print("9th choose is not a string");
    elseif ( (C10 ~= nil) and (type(C10) ~= "string") ) then
		print("10th choose is not a string");
    elseif ( (C11 ~= nil) and (type(C11) ~= "string") ) then
		print("11th choose is not a string");
    elseif ( (C12 ~= nil) and (type(C12) ~= "string") ) then
		print("12th choose is not a string");
	else
		-- count the choose num
		local ntotal = 0;
		if ( C1 == nil ) then
			return;
		elseif ( C2 == nil ) then
		    ntotal = 1;
		elseif ( C3 == nil ) then
		    ntotal = 2;
		elseif ( C4 == nil ) then
		    ntotal = 3;
		elseif ( C5 == nil ) then
		    ntotal = 4;
		elseif ( C6 == nil ) then
		    ntotal = 5;
        elseif ( C7 == nil ) then
		    ntotal = 6;
        elseif ( C8 == nil ) then
		    ntotal = 7;
        elseif ( C9 == nil ) then
		    ntotal = 8;
        elseif ( C10 == nil ) then
		    ntotal = 9;
        elseif ( C11 == nil ) then
		    ntotal = 10;
        elseif ( C12 == nil ) then
		    ntotal = 11;
		else
		    ntotal = 12;
		end

		local bvalid = false;
		if ntotal == 1 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1);
		elseif ntotal == 2 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1, C2);
		elseif ntotal == 3 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1, C2, C3);
		elseif ntotal == 4 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1, C2, C3, C4);
        elseif ntotal == 5 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1, C2, C3, C4, C5);
        elseif ntotal == 6 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1, C2, C3, C4, C5, C6);
        elseif ntotal == 7 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1, C2, C3, C4, C5, C6, C7);
        elseif ntotal == 8 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1, C2, C3, C4, C5, C6, C7, C8);
        elseif ntotal == 9 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1, C2, C3, C4, C5, C6, C7, C8, C9);
        elseif ntotal == 10 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10);
        elseif ntotal == 11 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11);
        elseif ntotal == 12 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12);
		end

		if ( bvalid == false ) then
		    return;
		end

		coroutine.yield();
		print("resume Choose Command");
		local result = LuaOwner:GetChooseResult();
	    print(string.format("player choose %d", result));
		if ( result == 1 ) then
   			if ( type(R1) == "function" ) then
				R1();
			end
		elseif result == 2 then
			if ( type(R2) == "function" ) then
				R2();
			end
		elseif result == 3 then
			if ( type(R3) == "function" ) then
				R3();
			end
		elseif result == 4 then
			if ( type(R4) == "function" ) then
				R4();
			end
        elseif result == 5 then
			if ( type(R5) == "function" ) then
				R5();
			end
        elseif result == 6 then
			if ( type(R6) == "function" ) then
				R6();
			end
        elseif result == 7 then
			if ( type(R7) == "function" ) then
				R7();
			end
        elseif result == 8 then
			if ( type(R8) == "function" ) then
				R8();
			end
        elseif result == 9 then
			if ( type(R9) == "function" ) then
				R9();
			end
        elseif result == 10 then
			if ( type(R10) == "function" ) then
				R10();
			end
        elseif result == 11 then
			if ( type(R11) == "function" ) then
				R11();
			end
        elseif result == 12 then
			if ( type(R12) == "function" ) then
				R12();
			end
		else
			print("Choose Failed");
			LuaOwner:LuaExitScript("Resume Choose Result Failed");
		end
	end
end
-- [ Choose command end ]

-- [ random choose command start ]
DefaultScript.RChoose = function(Question, C1, R1, C2, R2, C3, R3, C4, R4, C5, R5, C6, R6, C7, R7, C8, R8, C9, R9, C10, R10, C11, R11, C12, R12)
   	if LuaOwner == nil then
	 	return;
	end

	print("Do Random Choose Command");

    -- check type of string
	if ( (type(Question) ~= "string") or (Question == nil) ) then
		print("Question is nil or not a string");
	elseif ( (C1 == nil) or (type(C1) ~= "string") ) then
		print("First choose is nil or is not a string");
	elseif ( (C2 ~= nil) and (type(C2) ~= "string")) then
		print("Second choose is not a string");
	elseif ( (C3 ~= nil) and (type(C3) ~= "string") ) then
		print("Third choose is not a string");
	elseif ( (C4 ~= nil) and (type(C4) ~= "string") ) then
		print("Fourth choose is not a string");
	elseif ( (C5 ~= nil) and (type(C5) ~= "string") ) then
		print("Fifth choose is not a string");
    elseif ( (C6 ~= nil) and (type(C6) ~= "string") ) then
		print("6th choose is not a string");
	elseif ( (C7 ~= nil) and (type(C7) ~= "string") ) then
		print("7th choose is not a string");
    elseif ( (C8 ~= nil) and (type(C8) ~= "string") ) then
		print("8th choose is not a string");
    elseif ( (C9 ~= nil) and (type(C9) ~= "string") ) then
		print("9th choose is not a string");
    elseif ( (C10 ~= nil) and (type(C10) ~= "string") ) then
		print("10th choose is not a string");
    elseif ( (C11 ~= nil) and (type(C11) ~= "string") ) then
		print("11th choose is not a string");
    elseif ( (C12 ~= nil) and (type(C12) ~= "string") ) then
		print("12th choose is not a string");
	else
  		-- count the choose num
		local ntotal = 0;
		local p;
		if ( C1 == nil ) then
			return;
		elseif ( C2 == nil ) then
		    ntotal = 1;
			p = { C1 };
		elseif ( C3 == nil ) then
		    ntotal = 2;
			p = { C1, C2 };
		elseif ( C4 == nil ) then
		    ntotal = 3;
     		p = { C1, C2, C3 };
		elseif ( C5 == nil ) then
		    ntotal = 4;
      		p = { C1, C2, C3, C4 };
		elseif ( C6 == nil ) then
		    ntotal = 5;
      		p = { C1, C2, C3, C4, C5 };
        elseif ( C7 == nil ) then
		    ntotal = 6;
      		p = { C1, C2, C3, C4, C5, C6 };
        elseif ( C8 == nil ) then
		    ntotal = 7;
      		p = { C1, C2, C3, C4, C5, C6, C7 };
        elseif ( C9 == nil ) then
		    ntotal = 8;
      		p = { C1, C2, C3, C4, C5, C6, C7, C8 };
        elseif ( C10 == nil ) then
		    ntotal = 9;
      		p = { C1, C2, C3, C4, C5, C6, C7, C8, C9 };
        elseif ( C11 == nil ) then
		    ntotal = 10;
      		p = { C1, C2, C3, C4, C5, C6, C7, C8, C9, C10 };
        elseif ( C12 == nil ) then
		    ntotal = 11;
      		p = { C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11 };
		else
		    ntotal = 12;
      		p = { C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12 };
		end
		
		--////////////////
		local ch_list = {};
		for i=1, ntotal do
			table.insert(ch_list, i);
		end
		
		local re_list = {};

        local rnum = 0;
        for j=1, ntotal do
            rnum = math.random(table.getn(ch_list));
            table.insert(re_list, ch_list[rnum]);
			table.remove(ch_list, rnum);
        end
        
        local bvalid = false;
        if ntotal == 1 then
			bvalid = LuaOwner:LuaChooseEx(Question, C1);
  		elseif ntotal == 2 then
  		    bvalid = LuaOwner:LuaChooseEx(Question, p[re_list[1]], p[re_list[2]]);
        elseif ntotal == 3 then
  		    bvalid = LuaOwner:LuaChooseEx(Question, p[re_list[1]], p[re_list[2]], p[re_list[3]]);
        elseif ntotal == 4 then
  		    bvalid = LuaOwner:LuaChooseEx(Question, p[re_list[1]], p[re_list[2]], p[re_list[3]], p[re_list[4]]);
        elseif ntotal == 5 then
  		    bvalid = LuaOwner:LuaChooseEx(Question, p[re_list[1]], p[re_list[2]], p[re_list[3]], p[re_list[4]], p[re_list[5]]);
        elseif ntotal == 6 then
  		    bvalid = LuaOwner:LuaChooseEx(Question, p[re_list[1]], p[re_list[2]], p[re_list[3]], p[re_list[4]], p[re_list[5]], p[re_list[6]]);
        elseif ntotal == 7 then
  		    bvalid = LuaOwner:LuaChooseEx(Question, p[re_list[1]], p[re_list[2]], p[re_list[3]], p[re_list[4]], p[re_list[5]], p[re_list[6]], p[re_list[7]]);
        elseif ntotal == 8 then
  		    bvalid = LuaOwner:LuaChooseEx(Question, p[re_list[1]], p[re_list[2]], p[re_list[3]], p[re_list[4]], p[re_list[5]], p[re_list[6]], p[re_list[7]], p[re_list[8]]);
        elseif ntotal == 9 then
  		    bvalid = LuaOwner:LuaChooseEx(Question, p[re_list[1]], p[re_list[2]], p[re_list[3]], p[re_list[4]], p[re_list[5]], p[re_list[6]], p[re_list[7]], p[re_list[8]], p[re_list[9]]);
        elseif ntotal == 10 then
  		    bvalid = LuaOwner:LuaChooseEx(Question, p[re_list[1]], p[re_list[2]], p[re_list[3]], p[re_list[4]], p[re_list[5]], p[re_list[6]], p[re_list[7]], p[re_list[8]], p[re_list[9]], p[re_list[10]]);
        elseif ntotal == 11 then
  		    bvalid = LuaOwner:LuaChooseEx(Question, p[re_list[1]], p[re_list[2]], p[re_list[3]], p[re_list[4]], p[re_list[5]], p[re_list[6]], p[re_list[7]], p[re_list[8]], p[re_list[9]], p[re_list[10]], p[re_list[11]]);
        elseif ntotal == 12 then
  		    bvalid = LuaOwner:LuaChooseEx(Question, p[re_list[1]], p[re_list[2]], p[re_list[3]], p[re_list[4]], p[re_list[5]], p[re_list[6]], p[re_list[7]], p[re_list[8]], p[re_list[9]], p[re_list[10]], p[re_list[11]], p[re_list[12]]);
		end

		if ( bvalid == false ) then
		    print("RChoose Command Call LuaChoose Failed");
		    return;
		end
		
		coroutine.yield();
		print("resume Random Choose Command");
		local result = LuaOwner:GetChooseResult();
	    print(string.format("player choose %d", result));
	    
		if ((result > 0) and (result <= table.getn(re_list) )) then
			local re = re_list[result];
			if ( re == 1 ) then
			    if ( type(R1) == "function" ) then
			        R1();
			    end
			elseif ( re == 2 ) then
			    if ( type(R2) == "function" ) then
					R2();
				end
			elseif ( re == 3 ) then
			    if ( type(R3) == "function" ) then
					R3();
				end
			elseif ( re == 4 ) then
			    if ( type(R4) == "function" ) then
					R4();
				end
            elseif ( re == 5 ) then
			    if ( type(R5) == "function" ) then
					R5();
				end
            elseif ( re == 6 ) then
			    if ( type(R6) == "function" ) then
					R6();
				end
            elseif ( re == 7 ) then
			    if ( type(R7) == "function" ) then
					R7();
				end
            elseif ( re == 8 ) then
			    if ( type(R8) == "function" ) then
					R8();
				end
            elseif ( re == 9 ) then
			    if ( type(R9) == "function" ) then
					R9();
				end
            elseif ( re == 10 ) then
			    if ( type(R10) == "function" ) then
					R10();
				end
            elseif ( re == 11 ) then
			    if ( type(R11) == "function" ) then
					R11();
				end
            elseif ( re == 12 ) then
			    if ( type(R12) == "function" ) then
					R12();
				end
			end
		else
			LuaOwner:LuaExitScript("Resume Random Choose Result Failed");
		end
	end
end
-- [ random choose command end ]

-- [ party choose command start ]
DefaultScript.PChoose = function(Question, C1, R1, C2, R2, C3, R3, C4, R4)
	if LuaOwner == nil then
	 	return;
	end

	print("Do Party Choose Command");

	-- check type of string
	if ( (type(Question) ~= "string") or (Question == nil) ) then
		print("Question is nil or not a string");
	elseif ( (C1 == nil) or (type(C1) ~= "string") ) then
		print("Default choose is nil or not a string");
	elseif ( (C2 == nil) or (type(C2) ~= "string")) then
		print("first choose is nil or not a string");
	elseif ( (C3 == nil) or (type(C3) ~= "string") ) then
		print("Second choose is not a string");
	elseif ( (C4 ~= nil) and (type(C4) ~= "string") ) then
		print("Third choose is not a string");
	else
		-- count the choose num
		local ntotal = 0;

		if  ( C2 ~= nil ) then
			ntotal = ntotal + 1;
		end

		if  ( C3 ~= nil ) then
			ntotal = ntotal + 1;
		end

		if  ( C4 ~= nil ) then
			ntotal = ntotal + 1;
		end

		local bvalid = false;
		if ntotal == 1 then
			bvalid = LuaOwner:LuaPartyChoose(Question, C1, C2);
		elseif ntotal == 2 then
			bvalid = LuaOwner:LuaPartyChoose(Question, C1, C2, C3);
		elseif ntotal == 3 then
			bvalid = LuaOwner:LuaPartyChoose(Question, C1, C2, C3, C4);
		end
		
		if ( bvalid == false ) then
		    print("PChoose Command Call LuaPartyChoose Failed");
		    return;
		end

		coroutine.yield();
		print("Resume Party Choose Command");
		local result = LuaOwner:GetChooseResult();

	    print(string.format("player choose %d", result));
		if result == 1 then
			print("Choose 1");
			if ( type(R1) == "function" ) then
				R1();
			end
		elseif result == 2 then
			print("Choose 2");
			if ( type(R2) == "function" ) then
				R2();
			end
		elseif result == 3 then
			print("Choose 3");
			if ( type(R3) == "function" ) then
				R3();
			end
		elseif result == 4 then
			print("Choose 4");
			if ( type(R4) == "function" ) then
				R4();
			end
		else
			print("Choose Failed");
			LuaOwner:LuaExitScript("Resume Party Choose Result Failed");
		end

	end
end
-- [ party choose command end ]


--[ Load other lua file start ]
DefaultScript.OpenLuaFile = function(filename)
	if ( (filename ~= nil) and (type(filename) == "string") ) then
		local newfile = assert(loadfile(filename));
   		if newfile ~= nil then
        	print("Do Open Lua File Command");
   			newfile();
		else
	    	print("OpenLuaFile Command Err: file not find");
		end
	end
end
--[ Load other lua file end ]

-- [ Open interface start ]
DefaultScript.OpenInterface = function(name, p1)
	if ( (name ~= nil) and (type(name) == "string") ) then
    	if name == "shop" then
        	if ( (p1 ~= nil) and (type(p1)=="number") ) then
                print("Do Open Shop Interface Command");
        		LuaOwner:LuaOpenShopMenu(p1);
			end
		elseif ( name =="skillshop" ) then
	   	 	if ( (p1 ~= nil) and (type(p1)=="number") ) then
                print("Do Open Skill Shop Interface Command");
        		LuaOwner:LuaOpenInterface(name , p1);
			end	
		elseif ( name =="mixshop" ) then
	   	 	if ( (p1 ~= nil) and (type(p1)=="number") ) then
                print("Do Open Mix Shop Interface Command");
        		LuaOwner:LuaOpenInterface(name , p1);
			end	
		elseif ( name =="bankdeposit" ) then
	   	 	if ( (p1 ~= nil) and (type(p1)=="number") ) then
                print("Do Open Bank Deposit Interface Command");
        		LuaOwner:LuaOpenInterface(name , p1);
			end	
		elseif ( name =="bankwithdraw" ) then
	   	 	if ( (p1 ~= nil) and (type(p1)=="number") ) then
                print("Do Open Bank Withdraw Interface Command");
        		LuaOwner:LuaOpenInterface(name , p1);
			end			
		end
   	end
end
-- [ Open interface end ]

-- [ Jump map start ]
DefaultScript.JumpMap = function(target, mapid, posx, posy)
	if ( (DefaultScript.UnitExists("player")) == true ) then
		if ( (target ~= nil) and ((type(target)) == "string") )  then
			if ( target == "map" ) then
				if ( (mapid ~= nil) and (posx ~= nil) and (posy ~= nil) ) then
					if ( ((type(mapid)) == "number") and ((type(posx))=="number") and ((type(posy))=="number") ) then
						print("Do Jump Map Command");
						LuaOwner:LuaJumpMap(0, mapid, posx, posy);
				    end
				end
			elseif ( target == "reborn" ) then
				print("Do Jump To Reborn Command");
				LuaOwner:LuaJumpMap(1, 0, 0, 0);		
			end
		end
	end
end
-- [ Jump map end] 

-- [ Call mount start ]
DefaultScript.CallMount = function(modId)
	if ( (modId~=nil) and (type(modId)=="number") ) then
	    print("Do Call Mount Command");
	    return LuaOwner:LuaCallMount(modId);
	end
end
-- [ Call mount end ]

-- [ Generate npc start ]
DefaultScript.GenerateNPC = function(npcid, mapid, posx, posy, limitclick, lifetime)
	if ( (npcid ~= nil) and (mapid ~= nil) and (posx ~= nil) and (posy ~= nil) and (lifetime ~= nil) and (limitclick~=nil)) then
		if ( (type(npcid) == "number") and (type(mapid) == "number") and (type(posx) == "number") and (type(posy) == "number") and (type(lifetime)=="number") and (type(limitclick)=="boolean")) then
			print("Do Generate NPC Command");
			return LuaOwner:LuaGenerateNPC(npcid, mapid, posx, posy, limitclick, lifetime);
		end 	
	end 
	return nil;
end
-- [ Generate npc end ]

-- [ Detect blockade start ]
DefaultScript.IsValidBlock = function(mapid, x, y)
	if ( (mapid ~= nil) and (type(mapid)=="number")) then
		if ( DefaultScript.MapExists(mapid) == true ) then
	   		if ( (x ~= nil) and (y ~= nil) ) then
				if ( (type(x) == "number") and (type(y) == "number") ) then
					print("Do Detect Valid Blockade Command");
					return LuaOwner:LuaDetectValidBlockade(mapid, x, y);
				end   
			end 	
		end	
	end
	return false;
end
-- [ Detect blockade end ]

-- [ Calc time mark start ]
DefaultScript.CalcTimeMark = function(unitid, markid, dtime)
    if ( (DefaultScript.GetTimeMark(unitid, markid)) == true ) then
		if ( (dtime~=nil) and (type(dtime)=="number") ) then
		    print("Calc Time Mark Diff Time Command");
		    return LuaOwner:LuaCalcTimeMark(unitid, markid, dtime);
		end
	end
    return nil;
end
-- [ Calc time mark end]

-- [ Update quest start ]
DefaultScript.UpdateQuest = function()
	if LuaOwner ~= nil then
	    print("Do Temp Update Quest Data Command");
	    LuaOwner:LuaUpdateQuest();
	end
end
-- [ Update quest end ]

-- [ flag exists start ]
DefaultScript.FlagExists = function(unitid, flagid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (flagid ~= nil) and ( type(flagid) == "number" ) ) then
			print("Do Check Flag Exists Command");
			return LuaOwner:LuaQuestFlagExist(unitid, flagid);
		end
    end
    return false;
end
-- [ flag exists end ]

-- [ Type exists start ]
DefaultScript.TypeExists = function(unitid, maintype)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (maintype ~= nil) and (type(maintype) == "number") ) then
			print("Do Quest Type Exists Command");
			return LuaOwner:LuaQuestTypeExists(unitid, maintype); 
		end
	end
	return false;
end
-- [ Type exists end ]

-- [ Update round start ]
DefaultScript.UpdateRound = function(unitid, maintype)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		print("Do Update Quest Round Info Command");
		LuaOwner:LuaUpdateRound(unitid, maintype);
	end
end
-- [ Update round end ]


-- /////////////////   Get Command   ////////////////////////
-- [ Is night start ]
DefaultScript.IsNight = function()
	if ( LuaOwner ~= nil ) then
		return LuaOwner:LuaGetGameTime(0);
	end
	return nil; 
end
-- [ Is night end ]

-- [ Is Leader start ]
DefaultScript.IsLeader = function()
   	if ( LuaOwner ~= nil ) then
		return LuaOwner:LuaIsLeader(); 
   	end
   	return nil; 
end
-- [ Is night end ]

-- [ Get game time start ]
DefaultScript.GetGameTime = function()
	if ( LuaOwner ~= nil ) then
		return LuaOwner:LuaGetGameTime(1);
	end
	return nil;
end
-- [ Get game time end ]


-- [ get member numuber start ]
DefaultScript.GetMemberNum = function()
   print("Do Get Member Number Command");
   return LuaOwner:LuaGetMemberNum();
end
-- [ get member number end]

-- [ get base attribute start ]
DefaultScript.GetUnitStatus = function(unitid, attrtype)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ((attrtype ~= nil) and ((type(attrtype)) == "number")) then
	       	print("Do Unit Status Command");
			return LuaOwner:LuaGetUnitStatusU16(unitid, attrtype);
		end				
	end
	return nil;
end
-- [ get base attribute end ]

-- [ Get exp start ] 
DefaultScript.GetExp = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do GetExp Command");
		return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_EXP);
	end
	return nil;
end
-- [ Get exp end]

-- [ Get level start ]
DefaultScript.GetLevel = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do GetLevel Command");
		return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_LEVEL);
	end
	return nil;
end
-- [ Get level end ]

-- [ Get Job start ]
DefaultScript.GetJob = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Job Command");
		return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_JOB);
	end
	return nil;
end
-- [ Get Job end ]

-- [ Get Sex start ]
DefaultScript.GetSex = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Sex Command");
		return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_SEX);
	end
	return nil;
end
-- [ Get Sex end ]

-- [ Get HP start ]
DefaultScript.GetHP = function(unitid, hptype)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (hptype ~= nil) and ((type(hptype)) == "number") ) then
	        print("Do Get HP Command");
	        if ( hptype == 0 ) then
	            -- hp
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_HP);
	        elseif ( hptype == 1 ) then
	            -- mhp
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MHP);
	        elseif ( hptype == 2 ) then
	            -- maxhp
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXHP);
			end
	    end
	end
	return nil;
end
-- [ Get HP end ]

-- [ Get SP start ]
DefaultScript.GetSP = function(unitid, sptype)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (sptype ~= nil) and ((type(sptype)) == "number") ) then
	        print("Do Get SP Command");
	        if ( sptype == 0 ) then
	            -- sp
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_SP);
	        elseif ( sptype == 1 ) then
	            -- msp
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MSP);
	        elseif ( sptype == 2 ) then
	            -- maxsp
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXSP);
			end
	    end
	end
	return nil;
end
-- [ Get SP end ]

-- [ Get DP start ]
DefaultScript.GetDP = function(unitid, dptype)
	print("test");
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (dptype ~= nil) and ((type(dptype)) == "number") ) then
	        print("Do Get DP Command");
	        if ( dptype == 0 ) then
	            -- dp
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_DP);
	        elseif ( dptype == 1 ) then
	            -- maxdp
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXDP);
			end
	    end
	end
	return nil;
end
-- [ Get DP end ]

-- [ Get VP start ]
DefaultScript.GetVP = function(unitid, vptype)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (vptype ~= nil) and ((type(vptype)) == "number") ) then
	        print("Do Get VP Command");
	        if ( vptype == 0 ) then
	            -- vp
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_VP);
	        elseif ( vptype == 1 ) then
	            -- maxvp
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXVP);
			end
	    end
	end
	return nil;
end
-- [ Get VP end ]

-- [ Get WC start ]
DefaultScript.GetWC = function(unitid, wctype)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (wctype ~= nil) and ((type(wctype)) == "number") ) then
	        print("Do Get WC Command");
	        if ( wctype == 0 ) then
	            -- wc
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_WC);
	        elseif ( wctype == 1 ) then
	            -- wcexp
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_WCEXP);
			elseif ( wctype == 2 ) then
	            -- maxwc
	            return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXWC);
			end
	    end
	end
	return nil;
end
-- [ Get WC end ]

-- [ Get faction start ]
DefaultScript.GetFaction = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Faction Command");
		return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_FACTION);
	end
	return nil;
end
-- [ Get faction end ]

-- [ Get unit array start ]
DefaultScript.GetUnitArray = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Unit Array Command");
		return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_CURRARRAY);
	end
	return nil;
end
-- [ Get unit array end ]

-- [ Get pk point start ]
DefaultScript.GetPKPoint = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get PK Point Command");
		return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PKPOINT);
	end
	return nil;
end
-- [ Get pk point end ]

-- [ Get partner total count start ]
DefaultScript.GetPartnerTotalCount = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Partner Total Count Command");
		return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PARTNERTOTAL);
	end
	return nil;
end
-- [ Get partner total count end ]

-- [ Get partner count start ]
DefaultScript.GetPartnerCount = function(unitid, modid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (modid ~= nil) and (type(modid) == "number") ) then
	   		print("Do Get Partner Count Command");
	   		return LuaOwner:LuaGetIndividualPartnerCount(unitid, modid);
		end
	end
	return nil;
end
-- [ Get partner count end ]


DefaultScript.GetPetID = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Pet Total Count Command");
		return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETID);
	end
	return nil;
end


-- [ Get pet total count start ]
DefaultScript.GetPetTotalCount = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Pet Total Count Command");
		return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETTOTAL);
	end
	return nil;
end
-- [ Get pet total count end ]

-- [ Get pet count start ]
DefaultScript.GetPetCount = function(unitid, modid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (modid ~= nil) and (type(modid) == "number") ) then
	   		print("Do Get Pet Count Command");
	   		return LuaOwner:LuaGetIndividualPetCount(unitid, modid);
		end
	end
	return nil;
end
-- [ Get pet count end ]


DefaultScript.GetPetLv = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		print("Do Get Pet Lv Command");
		return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETLV);
	end
	return nil;
end


DefaultScript.GetPetExp = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		print("Do Get Pet Exp Command");
		return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETEXP);
	end
	return nil;
end


DefaultScript.GetPetVP = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		print("Do Get Pet VP Command");
		return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETVP);
	end
	return nil;
end

DefaultScript.GetPetSpirit = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
   		print("Do Get Pet Spirit Command");
     	return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETSPIRIT);
	end
	return nil;
end



DefaultScript.GetPetHappiness = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		print("Do Get Pet Happiness Command");
		return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETHAPPINESS);
	end
	return nil;
end

-- [ Get char id start ]
DefaultScript.GetCharID = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Character ID Command");
		return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_CHARID);
	end
	return nil;
end
-- [ Get char id end ]

-- [ Get money start ]
DefaultScript.GetMoney = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do GetMoney Command");
		return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_MONEY);
	end
	return nil;
end
-- [ Get money end ]

-- [ Get goodness start ]
DefaultScript.GetGoodness = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Goodness Command");
	    return LuaOwner:LuaGetGoodness(unitid);
	end
	return nil;
end
-- [ Get goodness end ]

-- [ Get honor start ]
DefaultScript.GetHonor = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Honor Command");
		return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_HONOR);
	end
	return nil;
end
-- [ Get honor end ]


-- [ Get donate start ]
DefaultScript.GetDonate = function(unitid, donatetype)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (donatetype ~= nil) and ((type(donatetype)) == "number") ) then
	        print("Do Get Donate Command");
	        
	        if ( donatetype == 0 ) then
	            -- faction donate
	            return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_FDONATE);
	        elseif ( donatetype == 1 ) then
	            -- total faction donate
	            return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_TFDONATE);
	        elseif ( donatetype == 2 ) then
	            -- manor donate
	            return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_MDONATE);
	        elseif ( donatetype == 3 ) then
				-- total manor donate
	        	return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_TMDONATE);
	        end
	    end
	end
	return nil;
end
-- [ Get donate end ]

-- [ Get game money start ]
DefaultScript.GetGameMoney = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Game Money Command");
	    return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_GAMEMONEY);
	end
	return nil;
end
-- [ Get game money end]


-- [ Get bank money start ]
DefaultScript.GetBankMoney = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do Get Bank Money Command");
	    return LuaOwner:LuaGetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_BANKMONEY);
	end
	return nil;
end
-- [ Get bank money end]


-- [ Get player name start ]
DefaultScript.GetPlayerName = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    print("Do GetPlayerName Command");
	    return LuaOwner:LuaGetUnitName(unitid, 0);
	end
	return nil;
end
-- [ Get player name end]

-- [ Get mob name start ]
DefaultScript.GetMobName = function(modid)
    if ( (DefaultScript.UnitExists("player")) == true ) then
    	if ( (modid ~= nil) and ((type(modid)) == "number") ) then
    		print("Do Get MOB Name Command");
	   	 	return LuaOwner:LuaGetUnitName("mob", modid);	
    	end	    
	end
	return nil;
end
-- [ Get mob name end]

-- [ Get npc name start ]
DefaultScript.GetNpcName = function(npcid)
    if ( (DefaultScript.UnitExists("player")) == true ) then
    	if ( (npcid ~= nil) and ((type(npcid)) == "number") ) then
    		print("Do Get NPC Name Command");
	   	 	return LuaOwner:LuaGetUnitName("npc", npcid);	
    	end	    
	end
	return nil;
end
-- [ Get npc name end ]

-- [ Get map name start ]
DefaultScript.GetMapName = function(mapId)
    if ( (DefaultScript.UnitExists("player")) == true ) then
        if ( (mapId~=nil) and (type(mapId)=="number") ) then
	   		print("Do Get Map Name Command");
			return LuaOwner:LuaGetUnitName("map", mapId);
		end
	end
	return nil;
end
-- [ Get map name end]

-- [ Get quest main type name start ]
DefaultScript.GetTypeName = function(unitid, subid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (subid ~= nil) and (type(subid) == "number") ) then
			print("Do Get Quest Type Main Command");
			return LuaOwner:LuaGetTypeName(unitid, subid);
		end		
	end
end
-- [ Get quest main type name end ] 

-- [ Get quest item name start ]
DefaultScript.GetItemName = function(itemid)
	if ( (DefaultScript.UnitExists("player")) == true ) then
    	if ( (itemid ~= nil) and (type(itemid) == "number") ) then
			print("Do Get Item Name Command");
    		return LuaOwner:LuaGetUnitName("item", itemid);
		end 
	end
end
-- [ Get quest item name end ]

-- [ Get use item target start ]
DefaultScript.GetUseItemTarget = function()
    if ( (DefaultScript.UnitExists("player")) == true ) then
		print("Do Get Use Item Target Name");
		return LuaOwner:LuaGetUnitName("target", 0);
	end
	return nil;
end
-- [ Get use item target end ]

-- [ Get item user type id start ]
DefaultScript.GetItemUserTypeId = function()
    if ( (DefaultScript.UnitExists("player")) == true ) then
        print("Do Get Item User Type Id Command");
        local res = LuaOwner:LuaGetItemUserTypeId();
        if res == 0 then
            return nil;
		else
			return res;
		end
    end
    return nil;
end
-- [ Get item user type id end ]

-- [ Get mob partner type start ]
DefaultScript.GetMobPartnerType = function()
	if ( (DefaultScript.UnitExists("player")) == true ) then
        local res = LuaOwner:LuaGetItemUserTypeId();
        if res == 2 then
		    return LuaOwner:LuaGetUnitStatusU32("target", UNIT_STATUS_REFNUM_MOBPARTNERTYPE);
		end
	end
	return nil;
end
-- [ Get mob partner type end ]

-- [ Get mob pet type start ]
DefaultScript.GetPetType = function()
	if ( (DefaultScript.UnitExists("player")) == true ) then
        local res = LuaOwner:LuaGetItemUserTypeId();
        if res == 3 then
		    return LuaOwner:LuaGetUnitStatusU32("target", UNIT_STATUS_REFNUM_PETTYPE);
		end
	end
	return nil;
end
-- [ Get mob pet type end ]


DefaultScript.GetMobReqLV = function()
	if ( (DefaultScript.UnitExists("player")) == true ) then
		local res = LuaOwner:LuaGetItemUserTypeId();
        if res == 2 then
			print("Do Get mob request Level");
			return LuaOwner:LuaGetMobReqLV("target");
		end
    end
	return nil;
end


DefaultScript.GetPartnerSkillCount = function(unitid, Type)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( ( Type ~= nil) and ((type( Type)) == "number") )  then
			print("Do GetPartnerSkillCount Command");
			return LuaOwner:LuaGetPartnerSkillCount(unitid, Type);
		end
    end
	return nil;
end


DefaultScript.HasPartnerSkill = function(unitid, ID)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( ( ID ~= nil) and ((type( ID)) == "number") )  then
			print("Do GetPartnerSkill Command");
			return LuaOwner:LuaHasPartnerSkill(unitid, ID);
		end
    end
	return nil;
end


-- [ Get map id start ]
DefaultScript.GetMapId = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        print("Do Get Map Id Command");
        return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAPID);
    end
end
-- [ Get map id end ]

-- [ Get speak count start ]
DefaultScript.GetSpeakCount = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        print("Do Get Speak Count Command");
        return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_SPEAKCOUNT);
    end
end
-- [ Get speak count end ]

-- [ Get Line Id start ]
DefaultScript.GetLineId = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        print("Do Get Line Id Command");
        return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_LINEID);
    end
end
-- [ Get Line Id end ]

-- [ Get map x start ]
DefaultScript.GetMapX = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        print("Do Get Map X Command");
        return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAPX);
    end
end
-- [ Get map x end ]

-- [ Get map y start ]
DefaultScript.GetMapY = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        print("Do Get Map Y Command");
        return LuaOwner:LuaGetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAPY);
    end
end
-- [ Get map y end ]

-- [ Get item count start ]
DefaultScript.GetItemCount = function(unitid, itemid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (itemid ~= nil) and ((type(itemid)) == "number") )  then
	   		print("Do Get Item Count Command");
			return LuaOwner:LuaGetItemCount(unitid, itemid);
		end
	end
	return nil;
end
-- [ Get item count end ]

-- [ Get free slot start ]
DefaultScript.GetFreeSlot = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	print("Do Get Free Slot Command");
		return LuaOwner:LuaGetFreeSlot(unitid, 0);
	end
	return nil;
end
-- [ Get free slot end ]

-- [ Get total free slot start ]
DefaultScript.GetTotalFreeSlot = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		print("Do Get Total Free Slot Command");
		return LuaOwner:LuaGetFreeSlot(unitid, 255);
	end
end
-- [ Get total free slot end ]

-- [ Can add item start ]
DefaultScript.CanAddItem = function(unitid, itemid, quantity)
    if ( (DefaultScript.UnitExists(unitid))==true ) then
		if ( (itemid~=nil) and (type(itemid)=="number") ) then
			print("Do Can Add Item Command 1");
			if ( (quantity~=nil) and (type(quantity)=="number") ) then
				print("Do Can Add Item Command 2");
				return LuaOwner:LuaCanAddItem(unitid, 0, itemid, quantity);
			end
		end
	end
end
-- [ Can add item end ]

-- [ Get item root id start ]
DefaultScript.GetItemRootId = function(itemuid)
	if ( (itemuid~=nil) and (type(itemuid)=="number") ) then
    	print("Do Get Item Root Id Command");
    	return LuaOwner:LuaGetItemRootId(itemuid);
	end
	return 0;
end
-- [ Get item root id end ]

-- [ Get item value start ]
DefaultScript.GetItemValue = function(vType)
	if ( (vType~=nil) and (type(vType)=="number") ) then
	    if ( (ITEM_INSTANCE_UID~=nil) and (type(ITEM_INSTANCE_UID)=="number") and (ITEM_INSTANCE_UID~=0) ) then
	        if ( LuaOwner:LuaIsItemExists(ITEM_INSTANCE_UID) == true ) then
	            if ( (vType>=1) and (vType<=3) ) then
	            	print("Do Get Item Value Command");
	   				return LuaOwner:LuaGetItemValue(ITEM_INSTANCE_UID, vType);
				end
	        end
	    end
	end
	return nil;
end
-- [ Get item value end ]

-- [ Get skill level start ]
DefaultScript.GetSkillLevel = function(unitid, skillid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (skillid ~= nil) and (type(skillid) == "number") ) then
		    print("Do Get Skill Level Command");
			return LuaOwner:LuaGetSkillLevel(unitid, skillid);
		end
    end
end
-- [ Get skill level end ]

-- [ Get team level start ]
DefaultScript.GetTeamLevel = function()
	print("Do Get Team Level Command");
	return LuaOwner:LuaGetTeamAttr(UNIT_STATUS_REFNUM_LEVEL, 0);
end
-- [ Get team level end ]

-- [ Get team job start ]
DefaultScript.GetTeamJob = function(jobType)
	if ( (jobType~=nil) and (type(jobType)=="number") ) then
		print("Do Get Team Job Command");
		return LuaOwner:LuaGetTeamAttr(UNIT_STATUS_REFNUM_JOB, jobType);
	end
	return 0;
end
-- [ Get team job end ]

-- [ Get team sex start ]
DefaultScript.GetTeamSex = function(sexType)
	if ( (sexType~=nil) and (type(sexType)=="number") ) then
		print("Do Get Team Sex Command");
  		return LuaOwner:LuaGetTeamAttr(UNIT_STATUS_REFNUM_SEX, sexType);
	end
	return 0;
end
-- [ Get team sex end ]

-- [ Has equiped start ]
DefaultScript.HasEquiped = function(unitid, itemid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (itemid ~= nil) and ((type(itemid) == "number") ) ) then
			print("Do Has Equiped Command");
			return LuaOwner:LuaHasEquiped(unitid, itemid);
		end
	end
	return nil;
end
-- [ Has equiped end ]

-- [ Skill exists start ]
DefaultScript.SkillExists = function(unitid, skillid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if( (skillid ~= nil) and (type(skillid) == "number") ) then
		    print("Do Skill Exists Command");
		    return LuaOwner:LuaSkillExists(unitid, skillid);
		end
    end
end
-- [ Skill exists end ]

-- [ Buff exists start ]
DefaultScript.BuffExists = function(unitid, buffid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (buffid~=nil) and (type(buffid) == "number") ) then
            print("Do Buff Exists Command");
            return LuaOwner:LuaGetBuff(unitid, buffid);
        end
    end
    return false;
end
-- [ Buff exists end ]

-- [ Get quest start ]
DefaultScript.GetQuest = function(unitid, subid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (subid ~= nil) and (type(subid) == "number") ) then
			print("Do Get Quest Command");
			
			local temp_q = NewGameQuestClass(GameQuestSys);
   			if temp_q ~= nil then
   			    local l_quest_ptr = LuaOwner:LuaGetQuest(unitid, subid);
				if l_quest_ptr ~= nil then
				    temp_q:Assign(l_quest_ptr);
				else
				    print("get quest assign class failed");
			    	return nil;
				end
			else
			    print("get quest new class failed");
   			end
   			
   			return temp_q;
        end
    end
end
-- [ Get quest end ]

-- [ Get quest flag start ]
DefaultScript.GetFlag = function(unitid, flagid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (flagid ~= nil) and (type(flagid) == "number") ) then
		    if ( (DefaultScript.FlagExists(unitid, flagid)) == true ) then
		    	print("Do Get Quest Flag Command");
		    	return LuaOwner:LuaGetFlag(unitid, flagid);
			end
		end
	end
	return nil;
end
-- [ Get quest flag end ]

-- [ Get time mark start ]
DefaultScript.GetTimeMark = function(unitid, markid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (markid~=nil) and (type(markid)=="number") ) then
			print("Do Get Time Mark Command");
			return LuaOwner:LuaGetTimeMark(unitid, markid);
		end
    end
    return false;
end
-- [ Get time mark end ]

-- [ Get quest max round start ]
DefaultScript.GetMaxRound = function(unitid, maintype)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		if ( (maintype ~= nil) and (type(maintype) == "number") ) then
			print("Do Get Quest Max Round Command");
			return LuaOwner:LuaGetRoundAttri(unitid, maintype, 1); -- 1: MaxRound, 2:LimitRound, 3:LimitType, 4:Round, 5:TotalRound
		end
	end
	return nil;
end
-- [ Get quest max round end ]

-- [ Get limit round start ]
DefaultScript.GetLimitRound = function(unitid, maintype)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		if ( (maintype ~= nil) and (type(maintype) == "number") ) then
			print("Do Get Quest Limit Round Command");
			return LuaOwner:LuaGetRoundAttri(unitid, maintype, 2);
		end
	end
	return nil;
end 
-- [ Get limit round end ]

-- [ Get limit type start ]
DefaultScript.GetLimitType = function(unitid, maintype)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		if ( (maintype ~= nil) and (type(maintype) == "number") ) then
			print("Do Get Quest Limit Type Command");
			return LuaOwner:LuaGetRoundAttri(unitid, maintype, 3);
		end
	end
	return nil;
end 
-- [ Get limit type end ]

-- [ Get quest curr round start ]
DefaultScript.GetRound = function(unitid, maintype)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		if ( (maintype ~= nil) and (type(maintype) == "number") ) then
			print("Do Get Quest Round Command");
			return LuaOwner:LuaGetRoundAttri(unitid, maintype, 4);
		end
	end
	return nil;
end 
-- [ Get quest curr round end ]

-- [ Get quest total round start ]
DefaultScript.GetTotalRound = function(unitid, maintype)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		if ( (maintype ~= nil) and (type(maintype) == "number") ) then
			print("Do Get Quest Total Round Command");
			return LuaOwner:LuaGetRoundAttri(unitid, maintype, 5);
		end
	end
	return nil;
end 
-- [ Get quest total round end ]

--- ///////////////////    Add Command    /////////////////////////
-- [ add quest start ]
DefaultScript.AddQuest = function(unitid, questid, questname, typeid, typeName)
	 if ( (DefaultScript.UnitExists(unitid)) == true ) then	 	
	 	if ( (questid ~= nil) and (questname ~= nil) and (typeid ~= nil) and (typeName ~= nil) ) then
	 		if ( (type(questid) == "number") and (type(questname) == "string") and (type(typeid) == "number") and (type(typeName) == "string") ) then

				print("Do Add Quest Command");
				local temp_q = NewGameQuestClass(GameQuestSys);
				if temp_q ~= nil then
				    local l_quest_ptr = LuaOwner:LuaNewQuest(unitid, questid, questname, typeid, typeName);
				    if l_quest_ptr ~= nil then
				        print("Do Quest Assign Pointer");
                    	temp_q:Assign(l_quest_ptr);
					else
					    print("Can not new a quest");
						return nil;
					end
				else
				    print("new quest class failed");
				end

			   	return temp_q;
	 		end
	 		
		elseif ( (questid ~= nil) and (type(questid) == "number") ) then
		    print("Do Add Quest By Id Command");
            local temp_q = NewGameQuestClass(GameQuestSys);
			if temp_q ~= nil then
				local l_quest_ptr = LuaOwner:LuaNewQuestById(unitid, questid);

				if l_quest_ptr ~= nil then
					print("Do Quest Assign Pointer");
             	   temp_q:Assign(l_quest_ptr);
				else
					print("Can not new a quest");
					return nil;
				end
			else
			    print("new quest class failed");
			end

			return temp_q;
	 	end
	end
	return nil;
end
-- [ add quest end ]

-- [ Add quest flag start ]
DefaultScript.AddFlag = function(unitid, flagid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (flagid ~= nil) and (type(flagid) == "number") ) then
            print("Do Add Quest Flag Command");
            LuaOwner:LuaAddFlag(unitid, flagid, 0);
        end
    end
end
-- [ Add quest flag end ]

-- [ Add quest curr round start ]
DefaultScript.AddRound = function(unitid, maintype)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		print("Do Add Quest Curr Round Command");
		LuaOwner:LuaSetRound(unitid, maintype, 3, 1);	
	end
end
-- [ Add quest curr round end ]

-- [ Add quest total round start ]
DefaultScript.AddTotalRound = function(unitid, maintype)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		print("Do Add Quest Total Round Command");
		LuaOwner:LuaSetRound(unitid, maintype, 4, 1);	
	end
end
-- [ Add quest total round end ]

-- [ add partner command start ]
DefaultScript.AddPartner = function(unitid, modid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (modid ~= nil) and ((type(modid))== "number" )) then
            print("Do Add Partner Command");
            LuaOwner:LuaAddPartner(unitid, modid);
        end
    end
end
-- [ add partner command end ]

-- [ add pet command start ]
DefaultScript.AddPet = function(unitid, modid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (modid ~= nil) and ((type(modid))== "number" )) then
            print("Do Add Pet Command");
            LuaOwner:LuaAddPet(unitid, modid);
        end
    end
end
-- [ add pet command end ]

-- [ add item command start ]
DefaultScript.AddItem = function(unitid, itemid, itemcount)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (itemid ~= nil) and (itemcount ~= nil) ) then
		    if ( ((type(itemid)) == "number") and ((type(itemcount)) == "number" ) ) then
		        print("Do Add Item Command");
		        LuaOwner:LuaAddItem(unitid, itemid, itemcount);
		    end
  		end
	end
end
-- [ add item command end ]

-- [ Add item from csv start ]
DefaultScript.AddItemFromCSV = function(unitid, csvfile)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (csvfile ~= nil) and ((type(csvfile)) == "string") ) then
            print("Do Add Item From CSV Command");
            LuaOwner:LuaAddItemFromCSV(unitid, csvfile);
        end
    end
end
-- [ Add item from csv end ]

-- [ Add basse attribute start ] 
DefaultScript.AddUnitStatus = function(unitid, attrtype, value)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (attrtype ~= nil) and (value ~= nil) ) then
		   	if ( ((type(attrtype)) == "number" ) and ((type(value)) == "number" ) )  then
		 		print("Do Add Unit Status Command");
		    	LuaOwner:LuaAddUnitStatusU16(unitid, attrtype, value);
			end
		end	
	end
end
-- [ Add base attribute end ]

-- [ Add level start ]
DefaultScript.AddLevel = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Level Command");
	   		LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_LEVEL, value);
		end
	end
end
-- [ Add level end ]

-- [ Add speak count start ]
DefaultScript.AddSpeakCount = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add speak count Command");
	   		LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_SPEAKCOUNT, value);
		end
	end
end
-- [ Add speak count end ]

-- [ Add hp start ]
DefaultScript.AddHP = function(unitid, hptype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (hptype ~= nil) and (value ~= nil) ) then
	        if ( ((type(hptype)) == "number") and ((type(value)) == "number") ) then
	            print("Do Add HP Command");
	            
	            if ( hptype == 0 ) then
	                -- hp
                    LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_HP, value);
				elseif ( hptype == 1 ) then
				    -- mhp
				    LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MHP, value);
	            elseif ( hptype == 2 ) then
	                -- maxhp
                    LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXHP, value);
	            end
	        end
	    end
	end
end
-- [ Add hp end ]

-- [ Add sp start ]
DefaultScript.AddSP = function(unitid, sptype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (sptype ~= nil) and (value ~= nil) ) then
	        if ( ((type(sptype)) == "number") and ((type(value)) == "number") ) then
	            print("Do Add SP Command");

	            if ( sptype == 0 ) then
	                -- sp
                    LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_SP, value);
				elseif ( sptype == 1 ) then
				    -- msp
				    LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MSP, value);
	            elseif ( sptype == 2 ) then
	                -- maxsp
                    LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXSP, value);
	            end
	        end
	    end
	end
end
-- [ Add sp end ]

-- [ Add dp start ]
DefaultScript.AddDP = function(unitid, dptype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (dptype ~= nil) and (value ~= nil) ) then
	        if ( ((type(dptype)) == "number") and ((type(value)) == "number") ) then
	            print("Do Add DP Command");

	            if ( dptype == 0 ) then
	                -- dp
                    LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_DP, value);
	            elseif ( dptype == 1 ) then
	                -- maxdp
                    LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXDP, value);
	            end
	        end
	    end
	end
end
-- [ Add dp end ]

-- [ Add vp start ]
DefaultScript.AddVP = function(unitid, vptype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (vptype ~= nil) and (value ~= nil) ) then
	        if ( ((type(vptype)) == "number") and ((type(value)) == "number") ) then
	            print("Do Add VP Command");

	            if ( vptype == 0 ) then
	                -- vp
                   	LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_VP, value);
	            elseif ( vptype == 1 ) then
	                -- maxvp
                    LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXVP, value);
	            end
	        end
	    end
	end
end
-- [ Add vp end ]

-- [ Add wc start ]
DefaultScript.AddWC = function(unitid, wctype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (wctype ~= nil) and (value ~= nil) ) then
	        if ( ((type(wctype)) == "number") and ((type(value)) == "number") ) then
	            print("Do Add WC Command");

	            if ( wctype == 0 ) then
	                -- wc
                   	LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_WC, value);
	            elseif ( wctype == 1 ) then
	                -- wcexp
                    LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_WCEXP, value);
				elseif ( wctype == 2 ) then
				    -- maxwc
				    LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXWC, value);
	            end
	        end
	    end
	end
end
-- [ Add wc end ]

-- [ Add pk point start ]
DefaultScript.AddPKPoint = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add PK Point Command");
	   		LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PKPOINT, value);
		end
	end
end
-- [ Add pk point end ]

-- [ Add partner health start ]
DefaultScript.AddPartnerHealth = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Partner Health Command");
	   		LuaOwner:LuaAddUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PARTNERHEALTH, value);
		end
	end
end
-- [ Add partner health end ]

-- [ Add partner feeling start ]
DefaultScript.AddPartnerFeel = function(unitid, value, targetType)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Partner Feeling Command");
	   		if ( (targetType~=nil) and (type(targetType)=="number") ) then
	   			LuaOwner:LuaAddPartnerFeel(unitid, value, targetType);
			else
			    LuaOwner:LuaAddPartnerFeel(unitid, value);
			end
		end
	end
end
-- [ Add partner feeling end ]

-- [ Add money start ]
DefaultScript.AddMoney = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Money Command");
	   		LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_MONEY, value);
		end
	end
end
-- [ Add money end ]

-- [ Add exp start ]
DefaultScript.AddExp = function(unitid, value)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Exp Command");
	   		LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_EXP, value);
		end
	end
--[[
	if ( unitid == "target" ) then
	    local tname = LuaOwner:LuaGetUnitName("target", 0);
	    if (tname~=nil) and (tname~="") then
	        if ( tname == "player" ) then
	            print("Do Add Exp Command");
	   			LuaOwner:LuaAddUnitStatusU32("player", UNIT_STATUS_REFNUM_EXP, value);
			else
			     print("Do Add Partner Exp Command");
			     LuaOwner:LuaSay("player", tname, "npc");
			    LuaOwner:LuaAddUnitStatusU32("player", UNIT_STATUS_REFNUM_PARTNEREXP, value);
			end
		else
		    print("\n\n\n Failed \n\n\n");
	    end
	    --	return LuaOwner:LuaGetUnitName("target", 0);
	elseif ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Exp Command");
	   		LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_EXP, value);
		end
	end
	]]
end
-- [ Add exp end ]

-- [ Add honor start ]
DefaultScript.AddHonor = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Honor Command");
	   		LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_HONOR, value);
		end
	end
end
-- [ Add honor end ]

-- [ Add goodness start ]
DefaultScript.AddGoodness = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Goodness Command");
	   		LuaOwner:LuaAddGoodness(unitid, value);
		end
	end
end
-- [ Add goodness end ]

-- [ Add game money start ]
DefaultScript.AddGameMoney = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Game Money Command");
      		LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_GAMEMONEY, value);
		end
	end
end
-- [ Add game money end ]

-- [ Add bank money start ]
DefaultScript.AddBankMoney = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Bank Money Command");
      		LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_BANKMONEY, value);
		end
	end
end
-- [ Add bank money end ]


-- [ Add donate start ]
DefaultScript.AddDonate = function(unitid, donatetype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (donatetype ~= nil) and (value ~= nil) ) then
	        if ( ((type(donatetype)) == "number") and ((type(value)) == "number") ) then
	             print("Do Add Donate Command");

	             if ( donatetype == 0 ) then
	                -- faction donate
                    LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_FDONATE, value);
	             elseif( donatetype == 1 ) then
	                -- manor donate
                    LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_MDONATE, value);
	             end
	        end
	    end
	end
end
-- [ Add donate end ]

-- [ Add partner exp start ]
DefaultScript.AddPartnerExp = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Partner Exp Command");
      		LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PARTNEREXP, value);
		end
	end
end
-- [ Add parnter exp end ]

-- [ Add partner skill start ]
DefaultScript.AddPartnerSkill = function(unitid, skillid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (skillid ~= nil) and (type(skillid) == "number") ) then
			print("Do Add Partner Skill Command");
        	LuaOwner:LuaAddSkill(unitid, skillid);
		else
		    print("\n\n\n\n\n\n add partner skill failed");
		end
	else
	    print("\n\n\n\n\n\n add partner skill failed");
	end
end
-- [ Add partner skill end ]


DefaultScript.AddPetLv = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Pet Lv Command");
			LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETLV, value);
		end
	end
end


-- [ Add pet exp start ]
DefaultScript.AddPetExp = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Pet Exp Command");
      		LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETEXP, value);
		end
	end
end
-- [ Add pet exp end ]

-- [ Add pet vp start ]
DefaultScript.AddPetVP = function(unitid,  value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( ( value ~= nil) and (type( value) == "number")) then
			print("Do Add Pet VP Command");
			LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETVP, value);
        end
    end
end
-- [ Add pet vp end ]

-- [ Add title start ]
DefaultScript.AddTitle = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Pet VP Command");
      		LuaOwner:LuaAddTitle(unitid, value);
		end
	end
end
-- [ Add title end ]

-- [ Remove title start ]
DefaultScript.RemoveTitle = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Pet VP Command");
      		LuaOwner:LuaRemoveTitle(unitid, value);
		end
	end
end
-- [ Remove title end ]

-- [ Add pet Spirit start ]
DefaultScript.AddPetSpirit = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Pet Spirit Command");
      		LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETSPIRIT, value);
		end
	end
end
-- [ Add pet Spirit end ]

-- [ Add pet Happiness start ]
DefaultScript.AddPetHappiness = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Add Pet Happiness Command");
      		LuaOwner:LuaAddUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETHAPPINESS, value);
		end
	end
end
-- [ Add pet Happiness end ]

-- [ Add pet skill start ]
DefaultScript.AddPetSkill = function(unitid, skillid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (skillid ~= nil) and (type(skillid) == "number") ) then
			print("Do Add Pet Skill Command");
        	LuaOwner:LuaAddSkill(unitid, skillid);
		else
		    print("\n\n\n\n\n\n add pet skill failed");
		end
	else
	    print("\n\n\n\n\n\n add pet skill failed");
	end
end
-- [ Add pet skill end ]

-- [ Add skill start ]
DefaultScript.AddSkill = function(unitid, skillid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (skillid ~= nil) and (type(skillid) == "number") ) then
            print("Do Add Skill Command");
            LuaOwner:LuaAddSkill(unitid, skillid);
        end
    end
end
-- [ Add skill end ]

-- [ Add skill level start ]
DefaultScript.AddSkillLevel = function(unitid, skillid, level)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (skillid ~= nil) and (level ~= nil) ) then
            if ( (type(skillid) == "number") and (type(level) == "number") ) then
                print("Do Add Skill Level Command");
            	LuaOwner:LuaSetSkillLevel(unitid, skillid, level, 1); -- mode : 1(add), 2(remove)
            end
        end
    end
end
-- [ Add skill level end ]




-- [ Add buff start ]
DefaultScript.AddBuff = function(unitid, buffId, v1, v2, v3, coolDown, per)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (buffId~=nil) and (v1~=nil) and (v2~=nil) and (v3~=nil) and (coolDown~=nil) and (per~=nil) ) then
			if ( (type(buffId)=="number") and (type(v1)=="number") and (type(v2)=="number") and (type(v3)=="number") and (type(coolDown)=="number") and (type(per)=="number") ) then
				print("Do Add Buff Command");
                LuaOwner:LuaAddBuff(unitid, buffId, v1, v2, v3, coolDown, per);
			end
        end
    end
end
-- [ Add buff end ]

--- ///////////////////    Set Command    /////////////////////////
-- [ Set quest flag start ]
DefaultScript.SetFlag = function(unitid, flagid, step)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (flagid ~= nil) and (step ~= nil) ) then
		    if ( (type(flagid) == "number") and (type(step) == "number") ) then
		        print("Do Set Quest Flag Command");
		        LuaOwner:LuaSetFlag(unitid, flagid, step);
		    end
		end
	end
end
-- [ Set quest flag end ]

-- [ Set quest round info start ]
DefaultScript.SetRoundInfo = function(unitid, mainType, maxRound, limitRound, limitType)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (mainType ~= nil) and (maxRound ~= nil) and (limitRound ~= nil) and (limitType ~= nil) ) then			
			if ( (type(mainType) == "number") and (type(maxRound) == "number") and (type(limitRound) == "number") and (type(limitType) == "number") ) then
				print("Do Set Quest Round Information Command");
				LuaOwner:LuaSetQuestRound(unitid, mainType, maxRound, limitRound, limitType);
			end
		end	 
	end	
end
-- [ Set quest round info end ]

-- [ Set quest curr round start ]
DefaultScript.SetRound = function(unitid, maintype, round)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		if ( (maintype ~= nil) and (round ~= nil) ) then
			if ( (type(maintype) == "number") and (type(round) == "number") ) then
				print("Do Set Quest Curr Round Command");
				LuaOwner:LuaSetRound(unitid, maintype, 1, round);
			end
		end
	end
end
-- [ Set quest curr round end ]

-- [ Set quest total round start ]
DefaultScript.SetTotalRound = function(unitid, maintype, totalround)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		if ( (maintype ~= nil) and (totalround ~= nil) ) then
			if ( (type(maintype) == "number") and (type(totalround) == "number") ) then
				print("Do Set Quest Total Round Command");
				LuaOwner:LuaSetRound(unitid, maintype, 2, totalround);
			end
		end
	end
end
-- [ Set quest total round end ]


-- [ Set reborn start ]
DefaultScript.SetReborn = function(unitid, mapid, posx, posy)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (mapid ~= nil) and (posx ~= nil) and (posy ~= nil) ) then
			if ( ((type(mapid)) == "number") and ((type(posx))=="number") and ((type(posy))=="number") ) then
				print("Do Set Reborn Command");
				LuaOwner:LuaSetUnitReborn(unitid, mapid, posx, posy);
			end
		end
	end
end
-- [ Set reborn end ]

-- [ Set level end ]
DefaultScript.SetLevel = function(unitid, value) 
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then     
	   		print("Do Set Level Command");
	   		LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_LEVEL, value);
		end
	end
end
-- [ Set level end ]


-- [ Set faction end ]
DefaultScript.SetFaction = function(unitid, value) 
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then     
	   		print("Do Set Faction Command");
	   		LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_FACTION, value);
		end
	end
end
-- [ Set faction end ]

-- [ Set money start ]
DefaultScript.SetMoney = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Set Money Command");
	   		LuaOwner:LuaSetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_MONEY, value);
		end
	end
end
-- [ Set money end ]

-- [ Set exp start ]
DefaultScript.SetExp = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Set Exp Command");
	   		LuaOwner:LuaSetUnitStatusU32(unitid, UNIT_STATUS_REFNUM_EXP, value);
		end
	end
end
-- [ Set exp end ]

-- [ Set full hp start ]
DefaultScript.SetFullHP = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	print("Do Set Full HP Command");
	   	LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_FULLHP, 0);
	end
end
-- [ Set full hp end ]

-- [ Set full sp start ]
DefaultScript.SetFullSP = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	print("Do Set Full SP Command");
	   	LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_FULLSP, 0);
	end
end
-- [ Set full sp end ]

-- [ Set full vp start ]
DefaultScript.SetFullVP = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	print("Do Set Full VP Command");
	   	LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_FULLVP, 0);
	end
end
-- [ Set full vp end ]

-- [ Set partner full hp start ]
DefaultScript.SetFullPHP = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	print("Do Set Partner Full HP Command");
	   	LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PARTNERFULLHP, 0);
	end
end
-- [ Set partner full hp end ]

-- [ Set partner full sp start ]
DefaultScript.SetFullPSP = function(unitid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	print("Do Set Partner Full SP Command");
	   	LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PARTNERFULLSP, 0);
	end
end
-- [ Set partner full sp end ]

-- [ Set pk point start ]
DefaultScript.SetPKPoint = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Set PK Point Command");
	   		LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PKPOINT, value);
		end
	end
end
-- [ Set pk point end ]

-- [ Set pvp enable start ]
DefaultScript.PVPEnable = function(unitid, flag)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (flag ~= nil) and ((type(flag)) == "boolean") ) then
	   		print("Do PVP Enable Command");
			if ( flag == true ) then
			    LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PKENABLE, 1);
			else
			    LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PKENABLE, 0);
			end
		end
	end
end
-- [ Set pvp enable end ]

-- [ Set time mark start ]
DefaultScript.SetTimeMark = function(unitid, markid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (markid~=nil) and (type(markid)=="number") ) then
            print("Do Set Time Mark Command");
            LuaOwner:LuaSetTimeMark(unitid, markid);
        end
    end
end
-- [ Set time mark end ]

-- [ Set item value start ]
DefaultScript.SetItemValue = function(vType, value)
	if ( (vType~=nil) and (value~=nil) ) then
	    if ( (type(vType)=="number") and (type(value)=="number") ) then
	        if ( (ITEM_INSTANCE_UID~=nil) and (type(ITEM_INSTANCE_UID)=="number") and (ITEM_INSTANCE_UID~=0) ) then
	            if ( LuaOwner:LuaIsItemExists(ITEM_INSTANCE_UID) == true ) then
	            	if ( (vType>=1) and (vType<=3) ) then
	        			print("Do Set Item Value Command");
            			LuaOwner:LuaSetItemValue(ITEM_INSTANCE_UID, vType, value);
					end
				end
			end
	    end
	end
end
-- [ Set item value end ]

-- [ Reset partner start ]
DefaultScript.ResetPartner = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		print("Do reset partner command");
		LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_RESETPARTNER, 0);	
	end
end
-- [ Reset partner end ]

-- [ Reset pet start ]
DefaultScript.ResetPet = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		print("Do reset pet command");
		LuaOwner:LuaSetUnitStatusU16(unitid, UNIT_STATUS_REFNUM_RESETPET, 0);	
	end
end
-- [ Reset pet end ]

DefaultScript.ReversPoint = function(unitid, AttrType, point)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (AttrType ~= nil) and (point ~= nil) ) then
		    if ( ((type(AttrType)) == "number") and ( (type(point)) == "number" ) ) then
				print("Do revers point");
				return LuaOwner:LuaReversPoint(unitid, AttrType, point);	
			end
		end
	end
end


-- //////////////////////   Remove Command   ///////////////////////// --

-- [ Remove item start ]
DefaultScript.RemoveItem = function(unitid, itemid, count)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (itemid ~= nil) and (count ~= nil) ) then
		    if ( ((type(itemid)) == "number") and ( (type(count)) == "number" ) ) then
		        print("Do Remove Item Command");
		        return LuaOwner:LuaRemoveItem(unitid, itemid, count);
		    end
		end
    end
    return false;
end
-- [ Remove item end]

-- [ Remove partner start ]
DefaultScript.RemovePartner = function(unitid, modid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
      	if ( (modid ~= nil) and ((type(modid))== "number" )) then
            print("Do Remove Partner Command");
            LuaOwner:LuaRemovePartner(unitid, modid);
        end
    end
end
-- [ Remove partner end ]

-- [ Remove Pet start ]
DefaultScript.RemovePet = function(unitid, modid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
      	if ( (modid ~= nil) and ((type(modid))== "number" )) then
            print("Do Remove Pet Command");
            LuaOwner:LuaRemovePet(unitid, modid);
        end
    end
end
-- [ Remove Pet end ]

-- [ Remove Unit Status start ]
DefaultScript.RemoveUnitStatus = function(unitid, attrtype, value)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (attrtype ~= nil) and (value ~= nil) ) then
		    if ( ((type(attrtype)) == "number") and ( (type(value)) == "number" ) ) then
		        print("Do Remove Unit Status Command");
		        LuaOwner:LuaRemoveUnitStatusU16(unitid, attrtype, value);
		    end
		end
    end
end
-- [ Remove Unit Status end ]

-- [ Remove level start ]
DefaultScript.RemoveLevel = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Level Command");
	   		LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_LEVEL, value);
		end
	end
end
-- [ Remove level end ]

-- [ Remove money start ]
DefaultScript.RemoveMoney = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Money Command");
	   		LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_MONEY, value);
		end
	end
end
-- [ Remove money end ]

-- [ Remove exp start ]
DefaultScript.RemoveExp = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Exp Command");
	   		LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_EXP, value);
		end
	end
end
-- [ Remove exp end ]


-- [ Remove honor start ]
DefaultScript.RemoveHonor = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Honor Command");
	   		LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_HONOR, value);
		end
	end
end
-- [ Remove honor end ]

-- [ Remove goodness start ]
DefaultScript.RemoveGoodness = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Goodness Command");
	   		LuaOwner:LuaRemoveGoodness(unitid, value);
		end
	end
end
-- [ Remove goodness end ]

-- [ Remove donate start ]
DefaultScript.RemoveDonate = function(unitid, donatetype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (donatetype ~= nil) and (value ~= nil) ) then
	        if ( ((type(donatetype)) == "number") and ((type(value)) == "number") ) then
	             print("Do Remove Donate Command");

	             if ( donatetype == 0 ) then
	                -- faction donate
                    LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_FDONATE, value);
	             elseif( donatetype == 1 ) then
	                -- manor donate
                    LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_MDONATE, value);
	             end
	        end
	    end
	end
end
-- [ Remove donate end ]

-- [ Remove bank money start ]
DefaultScript.RemoveGameMoney = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Game Money Command");
      		LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_GAMEMONEY, value);
		end
	end
end
-- [ Remove bank money end ]

-- [ Remove bank money start ]
DefaultScript.RemoveBankMoney = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Bank Money Command");
      		LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_BANKMONEY, value);
		end
	end
end
-- [ Remove bank money end ]

-- [ Remove parnter exp start ]
DefaultScript.RemovePartnerExp = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Partner Exp Command");
      		LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PARTNEREXP, value);
		end
	end
end
-- [ Remove partner exp end ]

-- [ Remove pet exp start ]
DefaultScript.RemovePetExp = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Pet Exp Command");
      		LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETEXP, value);
		end
	end
end
-- [ Remove pet exp end ]

-- [ Remove pet vp start ]
DefaultScript.RemovePetVP = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	--if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Pet Exp Command");
      		LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETVP, value);
		--end
	end
end
-- [ Remove pet vp end ]

-- [ Remove pet Spirit start ]
DefaultScript.RemovePetSpirit = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Pet Spirit Command");
      		LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETSPIRIT, value);
		end
	end
end
-- [ Remove pet Spirit end ]

DefaultScript.RemovePetLv = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Pet lv Command");
      		LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETLV, value);
		end
	end
end


-- [ Remove pet Happiness start ]
DefaultScript.RemovePetHappiness = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   	if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Pet Happiness Command");
      		LuaOwner:LuaRemoveUnitStatusU32(unitid, UNIT_STATUS_REFNUM_PETHAPPINESS, value);
		end
	end
end
-- [ Remove pet Happiness end ]

-- [ Remove hp start ]
DefaultScript.RemoveHP = function(unitid, hptype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (hptype ~= nil) and (value ~= nil) ) then
	        if ( ((type(hptype)) == "number") and ((type(value)) == "number") ) then
	            print("Do Remove HP Command");

	            if ( hptype == 0 ) then
	                -- hp
                    LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_HP, value);
				elseif ( hptype == 1 ) then
				    -- mhp
				    LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MHP, value);
	            elseif ( hptype == 2 ) then
	                -- maxhp
                    LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXHP, value);
	            end
	        end
	    end
	end
end
-- [ Remove hp end ]

-- [ Remove sp start ]
DefaultScript.RemoveSP = function(unitid, sptype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (sptype ~= nil) and (value ~= nil) ) then
	        if ( ((type(sptype)) == "number") and ((type(value)) == "number") ) then
	            print("Do Remove SP Command");

	            if ( sptype == 0 ) then
	                -- sp
                    LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_SP, value);
				elseif ( sptype == 1 ) then
				    -- msp
				    LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MSP, value);
	            elseif ( sptype == 2 ) then
	                -- maxsp
                    LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXSP, value);
	            end
	        end
	    end
	end
end
-- [ Remove sp end ]

-- [ Remove dp start ]
DefaultScript.RemoveDP = function(unitid, dptype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (dptype ~= nil) and (value ~= nil) ) then
	        if ( ((type(dptype)) == "number") and ((type(value)) == "number") ) then
	            print("Do Remove DP Command");

	            if ( dptype == 0 ) then
	                -- dp
                    LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_DP, value);
	            elseif ( dptype == 1 ) then
	                -- maxdp
                    LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXDP, value);
	            end
	        end
	    end
	end
end
-- [ Remove dp end ]

-- [ Remove vp start ]
DefaultScript.RemoveVP = function(unitid, vptype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (vptype ~= nil) and (value ~= nil) ) then
	        if ( ((type(vptype)) == "number") and ((type(value)) == "number") ) then
	            print("Do Remove VP Command");

	            if ( vptype == 0 ) then
	                -- vp
                   	LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_VP, value);
	            elseif ( vptype == 1 ) then
	                -- maxvp
                    LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXVP, value);
	            end
	        end
	    end
	end
end
-- [ Remove vp end ]

-- [ Remove wc start ]
DefaultScript.RemoveWC = function(unitid, wctype, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	    if ( (wctype ~= nil) and (value ~= nil) ) then
	        if ( ((type(wctype)) == "number") and ((type(value)) == "number") ) then
	            print("Do Remove WC Command");

	            if ( wctype == 0 ) then
	                -- wc
                   	LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_WC, value);
	            elseif ( wctype == 1 ) then
	                -- wcexp
                    LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_WCEXP, value);
				elseif ( wctype == 2 ) then
				    -- maxwc
				    LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_MAXWC, value);
	            end
	        end
	    end
	end
end
-- [ Remove wc end ]

-- [ Remove pk point start ]
DefaultScript.RemovePKPoint = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove PK Point Command");
	   		LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PKPOINT, value);
		end
	end
end
-- [ Remove pk point end ]

-- [ Remove partner health start ]
DefaultScript.RemovePartnerHealth = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Partner Health Command");
	   		LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PARTNERHEALTH, value);
		end
	end
end
-- [ Remove partner health end ]

-- [ Remove partner feel start ]
DefaultScript.RemovePartnerFeel = function(unitid, value)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
	   if ( (value ~= nil) and ((type(value)) == "number") ) then
	   		print("Do Remove Partner Feel Command");
	   		LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_PARTNERFEEL, value);
		end
	end
end
-- [ Remove partner feel end ]

-- [ Remove skill start ]
DefaultScript.RemoveSkill = function(unitid, skillid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (skillid ~= nil) and (type(skillid) == "number") ) then
            print("Do Remove Skill Command");
            LuaOwner:LuaRemoveSkill(unitid, skillid);
        end
    end
end
-- [ Remove skill end ]

-- [ Remove skill level start ]
DefaultScript.RemoveSkillLevel = function(unitid, skillid, level)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (skillid ~= nil) and (level ~= nil) ) then
            if ( (type(skillid) == "number") and (type(level) == "number") ) then
                print("Do Remove Skill Level Command");
            	LuaOwner:LuaSetSkillLevel(unitid, skillid, level, 2); -- mode : 1(add), 2(remove)
            end
        end
    end
end
-- [ Remove skill level end ]

-- [ Remove buff start ]
DefaultScript.RemoveBuff = function(unitid, buffid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (buffid~=nil) and (type(buffid)=="number") ) then
            print("Do Remove Buff Command");
            LuaOwner:LuaRemoveUnitStatusU16(unitid, UNIT_STATUS_REFNUM_BUFFID, buffid);
        end
    end
end
-- [ Remove buff end ]

-- [ Remove quest flag start ]
DefaultScript.RemoveFlag = function(unitid, flagid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (flagid ~= nil) and (type(flagid) == "number") ) then
		    print("Do Remove Quest Flag Command");
		    LuaOwner:LuaRemoveFlag(unitid, flagid);
		end
	end
end
-- [ Remove quest flag end ]

-- [ Remove quest start ]
DefaultScript.RemoveQuest = function(unitid, subid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (subid ~= nil) and (type(subid) == "number") ) then
            print("Do Remove Quest Command");
            LuaOwner:LuaRemoveQuest(unitid, subid);
            
            -----------------------
			--print("Do yield RemoveQuest Command");
            --coroutine.yield();
            --print("Do Resume RemoveQuest Command"); 
            ---------------------------
        end
    end
end
-- [ Remove quest end ]

-- [ Remove quest curr round start ]
DefaultScript.RemoveRound = function(unitid, maintype, value)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		if ( (value ~= nil) and (type(value) == "number") ) then
			print("Do Remove Quest Curr Round Command");
			LuaOwner:LuaSetRound(unitid, maintype, 5, value);	
		end	
	end
end
-- [ Remove quest curr round end ]

-- [ Remove quest total round start ]
DefaultScript.RemoveTotalRound = function(unitid, maintype, value)
	if ( (DefaultScript.TypeExists(unitid, maintype)) == true ) then
		if ( (value ~= nil) and (type(value) == "number") ) then
			print("Do Remove Quest Total Round Command");
			LuaOwner:LuaSetRound(unitid, maintype, 6, value);	
		end	
	end
end
-- [ Remove quest total round end ]

-- [ Remove time mark start ]
DefaultScript.RemoveTimeMark = function(unitid, markid)
    if ( (DefaultScript.UnitExists(unitid)) == true ) then
        if ( (markid~=nil) and (type(markid)=="number") ) then
            print("Do Remove Time Mark Command");
            LuaOwner:LuaRemoveTimeMark(unitid, markid);
        end
    end
end
-- [ Remove time mark end ]

-- [ Remove this item start ]
DefaultScript.RemoveThisItem = function()
	if ( (ITEM_INSTANCE_UID~=nil) and (type(ITEM_INSTANCE_UID)=="number") and (ITEM_INSTANCE_UID~=0) ) then
	    if ( LuaOwner:LuaIsItemExists(ITEM_INSTANCE_UID) == true ) then
	        print("Do Remove This Item Command");
	        return LuaOwner:LuaRemoveThisItem(ITEM_INSTANCE_UID);
	    end
	end
	return false;
end
-- [ Remove this item end ]


DefaultScript.GetMaxPartner = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		print("Do GetMaxPartner");
		return LuaOwner:LuaGetMaxPartner(unitid)
	end
	return 0
end

DefaultScript.SetMaxPartner = function(unitid, Count)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (Count ~= nil) and (type(Count) == "number") ) then
			print("Do SetMaxPartner");
			return LuaOwner:LuaSetMaxPartner(unitid, Count)
		end
	end
end

DefaultScript.GetMaxPet = function(unitid)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		print("Do GetMaxPet");
		return LuaOwner:LuaGetMaxPet(unitid)
	end
	return 0
end

DefaultScript.SetMaxPet = function(unitid, Count)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (Count ~= nil) and (type(Count) == "number") ) then
			print("Do SetMaxPet");
			return LuaOwner:LuaSetMaxPet(unitid, Count)
		end
	end
end



DefaultScript.ReplacePartnerSkill = function(unitid, NewID, Type)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (NewID ~= nil) and (type(NewID) == "number") and 
			(Type ~= nil) and (type(Type) == "number")) then
			print("Do ReplacePartnerSkill");
			return LuaOwner:LuaReplacePartnerSkill(unitid, NewID, Type)
		end
	end
	return nil
end

DefaultScript.PlaySound = function(Name)
	if ( (DefaultScript.UnitExists(unitid)) == true ) then
		if ( (Name ~= nil) and (type(Name) == "number")) then
			print("Do ReplacePartnerSkill");
			return LuaOwner:LuaPlaySound(Name)
		end
	end
	return nil
end

-- //////////////  Define Global NameSpace For Sharing  /////////////////// --

SetDefaultLuaOwner = DefaultScript.SetOwner; 
GDS = DefaultScript;

function GDSBattle(actor)
	local script = actor:GetScriptSystem();
	LuaOwner = script;
	return GDS;
end
--- ////////////////////////////////////////////// ---
 --[[
NameSpace = {
	testlocal = testlocal
};

return NameSpace;

function SetDefaultLuaOwner(ScriptSys)
	LuaOwner = ScriptSys;
end

function ReleaseDefaultLua()
	print("release");
	LuaOwner = nil;
end
]]--

