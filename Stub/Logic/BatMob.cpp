//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/BattleCode.h"
#include "Common/StateCode.h"
#include "Common/SettingTable.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Logic/BatMob.h"
//-- Library
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/BatCharacter.h"
#include "Proxy/Data/SkillData.h"
#include "MapService/Global.h"
#include "Resource/ResMob.h"
#include "Resource/ResSkill.h"
#include "Resource/ResBuff.h"
#include "Resource/ResSkillCombination.h"
#include "Resource/ServerResManager.h"

#define CONFIGURATION_INI_FILE		_T(".\\Data\\Setting\\fileconfig.ini")

PROTO_LuaPush(AttriStruct)
PROTO_LuaPush(ExtractedBuffData)
PROTO_LuaPush(CBatActor)
PROTO_LuaPush(CBatMob)
PROTO_LuaPush(CBattle)
REGISTER_CLASSTYPE(CBatMob, BatMob_CLASSID);

CBatMob::CBatMob()
{
	m_bNew = false;
	m_data = NULL;
	m_pMob = this;
	m_pBatChar = NULL;
	m_pPartner = NULL;
	m_bGuest = false;
}

CBatMob::~CBatMob()
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("mobUid %d, skillCount %d"),
		GetPartner_id(), (UInt32)m_SkillMap.size());
	if (!IsParnter())
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Not partner, skillCount %d"), (UInt32)m_SkillMap.size());
		for (SkillMap::iterator it = m_SkillMap.begin(); it != m_SkillMap.end(); ++it)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("skillid %d"), it->second->GetSkill_id());
			DeleteNetObject(it->second);
		}
	}
}

UInt8 CBatMob::GetSex()
{
	return (m_data ? m_data->sex : 0);
}

UInt8 CBatMob::GetRace()
{
	return (m_data ? m_data->race : 0);
}

void CBatMob::PreSend()
{
	Parent::PreSend();
	m_sendMask.MaskBits(infoMask, partnerTypeMask, mobTypeMask, -1);
	if (GetOwner_id() != 0)
		m_sendMask.MaskBits(ownerMask, partnerMask, partnerColorMask, -1);
}

void CBatMob::PreSendEnemy()
{
	Parent::PreSendEnemy();
	m_sendMask.MaskBits(infoMask, partnerTypeMask, mobTypeMask, -1);
	if (GetOwner_id() != 0)
		m_sendMask.MaskBits(ownerMask, partnerMask, partnerColorMask, -1);
}

void CBatMob::SetData(UInt32 mob_id, UInt16 level)
{
	const MobData *pData = NULL;

	// Get Mob data from ResourceManager
	pData = gGlobal.m_resource->GetMobData(mob_id);

	// no mob_id found
	if (!pData)
		return;

	String mobName;
	m_data = pData;
	mobName = m_data->mid_name;
	SetPartner(0);
	SetOwner(0);
	SetInfo(mob_id, mobName);
	if (level == 0)
		SetLevel(GetRand(m_data->lv_d, m_data->lv_u));
	else
		SetLevel(level);
	//SetModel(m_data->mo_mob_model);

	// Set attrib of STA, SPR, STR, CON, AGI
	UInt8 potential = m_data->potential;
	if (potential > 10 )
		potential = 0;
	if (potential == 10)
	{
		MobPotentSet f;
		UInt total = 0;

		while (total != 5)
		{
			for (Index i = C_INDEX(0); i < 5; ++i)
			{
				UInt randNo = GetRand(2);	
				if (total == 5)
					randNo = 0;
				else
				{
					total += randNo;
					if (total > 5)
					{
						randNo = 5 - (total - randNo);
						total = 5;
					}
				}
				switch(i)
				{
				case 0:	f.agi = 1 + randNo;	break;
				case 1:	f.con = 1 + randNo;	break;
				case 2:	f.spr = 1 + randNo;	break;
				case 3:	f.sta = 1 + randNo;	break;
				case 4:	f.str = 1 + randNo;	break;
				}
			}
		}
		SetAttrib1(	GetRand(m_data->STA_base_d, m_data->STA_base_u) + GetLevel() * f.sta,
					GetRand(m_data->SPR_base_d, m_data->SPR_base_u) + GetLevel() * f.spr,
					GetRand(m_data->STR_base_d, m_data->STR_base_u) + GetLevel() * f.str,
					GetRand(m_data->CON_base_d, m_data->CON_base_u) + GetLevel() * f.con,
					GetRand(m_data->AGI_base_d, m_data->AGI_base_u) + GetLevel() * f.agi);
			
	}
	else
	{
		const MobPotentSet &f = gMobPotentSet[potential];
		SetAttrib1(	GetRand(m_data->STA_base_d, m_data->STA_base_u) + GetLevel() * f.sta,
					GetRand(m_data->SPR_base_d, m_data->SPR_base_u) + GetLevel() * f.spr,
					GetRand(m_data->STR_base_d, m_data->STR_base_u) + GetLevel() * f.str,
					GetRand(m_data->CON_base_d, m_data->CON_base_u) + GetLevel() * f.con,
					GetRand(m_data->AGI_base_d, m_data->AGI_base_u) + GetLevel() * f.agi);
	}

	float grow = GetRand(m_data->grow_d, m_data->grow_u);
	SetGrow(grow);
	SetNative(GetRand(m_data->HP_native_d, m_data->HP_native_u), GetRand(m_data->SP_native_d, m_data->SP_native_u), 
		GetRand(m_data->ATK_native_d, m_data->ATK_native_u), GetRand(m_data->DEF_native_d, m_data->DEF_native_u),
		GetRand(m_data->SPD_native_d, m_data->SPD_native_u), GetRand(m_data->POW_native_d, m_data->POW_native_u),
		GetRand(m_data->HIT_native_d, m_data->HIT_native_u), GetRand(m_data->EVA_native_d, m_data->EVA_native_u) );
			

	SetPartnerType(m_data->partner_type);
	CalcAttrib();
	CalSpeed();
	SetMHP( GetMaxHP(), 0, 0);
	SetHP(GetMaxHP(), 0, 0);
	SetMSP( GetMaxSP(), 0, 0);
	SetSP(GetMaxSP(), 0, 0);
	SetGoodness(m_data->goodness);
	
	SetState(CHAR_ST_NORM);

	SetMobType(pData->mob_type);
	SetBoss(pData->boss);
	SetPartnerColor(0);

	ClearAllMask();
}

void CBatMob::SetCommandByAI()
{
	StringA funcName;
	if (m_data) {
		funcName.Format("AIAction%04i", m_data->ai_type);
	} else {
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CBatMob SetCommandByAI Warning: [m_data null, AIAction0001 used as default]"));
		funcName.Format("AIAction0001");
	}
	CallLuaFunc(gGlobal.g_pLuaState, funcName.c_str(), String(), m_pBattle, this);

	/*
	// sample AI: attack target randomly
	CBatActor *pTarget = NextTarget();

	// no target found
	if (!pTarget)
	{
		m_command = BATCMD_IDLE;
		return;
	}

	// attack target
	m_command = BATCMD_ATTACK;
	SetTarget(pTarget->GetIndex());
	*/
}

void CBatMob::SetState(const UInt8 &state)
{
	if ((state == CHAR_ST_DEAD && RealDead())|| state == CHAR_ST_CATCHED  )
	{
		SetLeftBattle(BOOLEAN_TRUE);
	}
	else if ((GetState() == CHAR_ST_DEAD && GetLeftBattle()) && state == CHAR_ST_NORM)
	{
		SetLeftBattle(BOOLEAN_FALSE);
	}
	
	Parent::SetState(state);
}


float CBatMob::GetFavor()
{
	return (m_pPartner ? m_pPartner->GetFavor() : 0.0f);
}

UInt16 CBatMob::GetHealth()
{
	return (m_pPartner ? m_pPartner->GetHealth() : 0);
}

void CBatMob::ChangeHealth(Int change)
{
	if (m_pPartner)
	{
		if (m_pPartner->IsUsePartnerCollection() && change < 0)
		{
			change *= 8;
		}
		m_pPartner->SetHealth(MAX((Int) m_pPartner->GetHealth() + change, 0));
	}
}

void CBatMob::ChangeFavor(float change)
{
	if (m_pPartner)
		m_pPartner->SetFavor(WITHIN(m_pPartner->GetFavor() + change, 0.0f, 100.0f));
}

void CBatMob::SetPartnerData(CPartner* partner)
{
	if (partner == NULL)
		return;

	SetPartner(partner->GetPartner_id());
	SetInfo(partner->GetMob_id(), partner->GetRaname());
	SetOwner(partner->GetOwner_id());
	SetLevel(partner->GetLevel());

	SetAttrib1(partner->GetSTA(), partner->GetSPR(), partner->GetSTR(), partner->GetCON(), partner->GetAGI());
	SetAttrib2(partner->GetATK(), partner->GetDEF(), partner->GetHIT(), partner->GetEVA(), partner->GetPOW(), partner->GetSPD());
	SetGrow(partner->GetGrow());
	SetNative(partner->GetHP_native(), partner->GetSP_native(), partner->GetATK_native(), partner->GetDEF_native(),
		partner->GetSPD_native(), partner->GetPOW_native(), partner->GetHIT_native(), partner->GetEVA_native());
	SetHP(partner->GetHP(), 0, 0);
	SetMHP(partner->GetMaxHP(), 0, 0);
	SetMaxHP(partner->GetMaxHP());
	SetSP(partner->GetSP(), 0, 0);
	SetMSP(partner->GetMaxSP(), 0, 0);
	SetMaxSP(partner->GetMaxSP());
	SetGoodness(partner->GetGoodness());
	SetBuff(partner->GetBuff());
	SetBuffCache(partner->GetBuffCache());
	SetState(CHAR_ST_NORM);
	SetPartnerColor(partner->GetOutLookColor());

	// Get Mob data from partner
	m_pPartner = partner;
	m_data = partner->m_data;
	if (m_data)
		SetPartnerType(m_data->partner_type);
	else
		SetPartnerType(0);
	CalSpeed();
}

void CBatMob::WritePartnerData(CPartner* partner)
{
	if (GetOwner_id() != partner->GetOwner_id())
		partner->SetOwner(GetOwner_id());
	if (GetHP() != partner->GetHP() || GetMaxHP() != partner->GetMaxHP())
		partner->SetHP(GetHP(), GetMaxHP());
	if (GetSP() != partner->GetSP() || GetMaxSP() != partner->GetMaxSP())
		partner->SetSP(GetSP(), GetMaxSP());
	if (GetBuff() != partner->GetBuff()) {
		partner->SetBuff(GetBuff());
		partner->ExtractBuffString();
	}
	if (GetBuffCache() != partner->GetBuffCache()) {
		partner->SetBuffCache(GetBuffCache());
		partner->ExtractBuffCacheString();
	}
	//if (GetGoodness() != partner->GetGoodness())
	//	partner->SetGoodness(GetGoodness());
}

/*
void CBatMob::DoEscape(Buffer &buffer)
{
	if (GetPartner_id() != 0)
	{
		DoPartnerLeft();
		SetState(CHAR_ST_ESCAPED);
		SetAction(BATCMD_ESCAPE);
		//SetLeftBattle(BOOLEAN_TRUE);
		
		WriteAction();
		
		m_pClient->pPartner = NULL;
		
		
		m_bAction = BOOLEAN_TRUE;
	}
	else
	{
		Parent::EscapeTogether();
		if ( m_pClient )
		{
			m_pClient->pPartner = NULL;
		}
	}
}
*/

void CBatMob::DoPartnerLeft()
{
	if (GetPartner_id() != 0)
	{
		UpdateHealthAndFavor();	
		if (m_pPartner)
			m_pPartner->ChangeState(CPartner::ST_REST);
	}
	SetLeftBattle(BOOLEAN_TRUE);
}

void CBatMob::CalcAttrib()
{

	if (GetOwner_id() == 0)
		CallLuaFunc(gGlobal.g_pLuaState, "CalBatMobAttr", String(), (CBatActor *)this, this, true);
	else if(m_pPartner)
	{
		SetAttrib1(m_pPartner->GetSTA(), m_pPartner->GetSPR(), m_pPartner->GetSTR(), m_pPartner->GetCON(), m_pPartner->GetAGI());
		SetAttrib2(m_pPartner->GetATK(), m_pPartner->GetDEF(), m_pPartner->GetHIT(), m_pPartner->GetEVA(), m_pPartner->GetPOW(), m_pPartner->GetSPD());
		SetMaxHP(m_pPartner->GetMaxHP());
		SetMaxSP(m_pPartner->GetMaxSP());
	}

	AttriStruct TempAttri;
	TempAttri.uIntAtk = (int)GetATK();
	TempAttri.uIntDef = (int)GetDEF();
	TempAttri.uIntHit = (int)GetHIT();
	TempAttri.uIntEva = (int)GetEVA();
	TempAttri.uIntPow = (int)GetPOW();
	TempAttri.uIntSpd = (int)GetSPD();
	TempAttri.uIntMaxHp = (int)GetMaxHP();
	TempAttri.uIntMaxSp = (int)GetMaxSP();

	UInt16 orgSpd = GetSPD();
	if (GetOwner_id() == 0)
	{
		for (SkillMap::iterator Itr = m_SkillMap.begin(); Itr != m_SkillMap.end(); ++Itr)
		{
			const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(Itr->second->GetSkill_id());
			if (pSkillData)
			{
				StringA LuaFunc;
				LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second->GetSkill_id(), "skill").c_str(), CommonSkillFuncName[SKILL_FUNC_NAME_ATTRI].c_str());
				if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
					CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), (CBatActor *)this,
						Itr->second->GetSkill_level(), &TempAttri);
			}
		}
	}

	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData)// && Itr->second.pBuffData->IsActive(ActiveType))
		{
			for (Int k = 0; k < (Int)Itr->second.pBuffData->Buff_acttimes.size(); k++)
			{
				if (Itr->second.pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
				{
					StringA LuaFunc;
					LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second.pBuffData->id, "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_ATTRI].c_str());
					if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), 
							(CBatActor *)this, &Itr->second, &TempAttri);
				}
			}
		}
	}

	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData)// && Itr->second.pBuffData->IsActive(ActiveType))
		{
			for (Int k = 0; k < (Int)Itr->second.pBuffData->Buff_acttimes.size(); k++)
			{
				if (Itr->second.pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
				{
					StringA LuaFunc;
					LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second.pBuffData->id, "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_ATTRI].c_str());
					if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), 
							(CBatActor *)this, &Itr->second, &TempAttri);
				}
			}
		}
	}


	SetAttrib2((UInt16) TempAttri.uIntAtk, (UINT16) TempAttri.uIntDef , (UINT16) TempAttri.uIntHit , 
		(UINT16) TempAttri.uIntEva , (UINT16) TempAttri.uIntPow , (UINT16) TempAttri.uIntSpd);
	SetMaxHP((UInt32) TempAttri.uIntMaxHp );
	if (GetHP() > GetMaxHP())
		SetHP(GetMaxHP());
	//mob always set mhp == maxHP
		SetMHP(GetMaxHP());
	SetMaxSP((UInt32) TempAttri.uIntMaxSp );
	if (GetSP() > GetMaxSP())
		SetSP(GetMaxSP());
	if (GetMSP() > GetMaxSP())
		SetMSP(GetMaxSP());

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Mob %d SetData:"), GetMob_id());
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("MHP: %d, MSP: %d"), 
		GetMaxHP(), GetMaxSP());
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("ATK %d, DEF %d, SPD %d, POW %d, HIT %d, EVA %d"), 
			GetATK(), GetDEF(), GetSPD(), GetPOW(), GetHIT(), GetEVA());
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GROW: %3.2f"), GetGrow());

	if (orgSpd != GetSPD())
		CalSpeed();
}

bool CBatMob::AllCmdBlock()
{
	return (m_cmdBlockMap[BATCMD_ATTACK] && m_cmdBlockMap[BATCMD_SKILL] &&
			m_cmdBlockMap[BATCMD_ITEM] && m_cmdBlockMap[BATCMD_DEFEND] &&
			m_cmdBlockMap[BATCMD_PROTECT] && m_cmdBlockMap[BATCMD_ESCAPE]);
}

void CBatMob::UpdateHealthAndFavor()
{
	if (GetPartner_id() == 0 || m_pBattle->GetBattleType() == MATCH_BATTLE  || m_pBattle->GetBattleType() == INCOGNITO_PK_BATTLE)
		return;

	Int health = 0;
	float favor = 0.0f;

	if (GetState() == CHAR_ST_DEAD)
	{
		SetHP(GetMaxHP(), 0, 0);
		SetSP(GetMaxSP(), 0, 0);

		if (m_pBattle->GetBattleType() == PK_BATTLE || 
			m_pBattle->GetBattleType() == FORCE_PK_BATTLE)
			health = -30;
		else
			health = -10;
		favor = -10.0f;	
	}
	else
	{
		if (m_pBattle->GetBattleType() == PK_BATTLE || 
			m_pBattle->GetBattleType() == FORCE_PK_BATTLE)
			health = -20;
		else
			health = -1;
		favor = -0.1f;
	}
	ChangeHealth(health);
	ChangeFavor(favor);
	if (GetHealth() == 0)		// partner left owner
		SetOwner(0);
}

bool CBatMob::IsBoss()
{
	return (m_data ? m_data->boss == BOOLEAN_TRUE : false);
}

void CBatMob::MobLearnSkill()
{
	if (GetMob_id() == 0)
		return;

	if (m_data == NULL)
	{
		// Get Mob data from ResourceManager
		m_data = gGlobal.m_resource->GetMobData(GetMob_id());
		if (m_data == NULL)
			return;
	}

	StringA mobSkillFile;
	mobSkillFile.Format("MobSkill%05d.csv",m_data->skill_file_id); 
	const MobSkillCombination* mobSkillLst = gGlobal.m_resource->GetMobSkillCombination(mobSkillFile.c_str());
	if (mobSkillLst == NULL)
		return;


	if (mobSkillLst && m_data)
	{
		UInt existSpecMobSkill = 0;
		UInt existNormMobSkill = 0;
		UInt maxSpecMobSkill = 7;
		UInt maxNormMobSkill = 0;

		switch (m_data->talent)
		{
		case MOB_TALENT_A:	maxNormMobSkill = GetPrivateProfileInt(_T("LoadLearnSkill"), _T("TalentAMaxNormalSkill"), 7, CONFIGURATION_INI_FILE);	break;
		case MOB_TALENT_B:	maxNormMobSkill = GetPrivateProfileInt(_T("LoadLearnSkill"), _T("TalentBMaxNormalSkill"), 5, CONFIGURATION_INI_FILE);	break;
		case MOB_TALENT_C:	maxNormMobSkill = GetPrivateProfileInt(_T("LoadLearnSkill"), _T("TalentCMaxNormalSkill"), 3, CONFIGURATION_INI_FILE);	break;
		}

		UInt16Vector skillIDs;

		for (Index i = C_INDEX(0); i < mobSkillLst->skillLst.size(); ++i)
		{
			UInt32 rand = (UInt32)(GetRand(0.0f, 100.0f) * 1000);
			MobSkillDef* pDef = mobSkillLst->skillLst[i];
			if (pDef && rand <= pDef->skillRate)
			{
				if ( (pDef->skillType == PARTNER_NORMAL_SKILL && existNormMobSkill < maxNormMobSkill) )
				{
					skillIDs.push_back(pDef->skillId);
					++existNormMobSkill;
				}
				else if (pDef->skillType == PARTNER_SPECIAL_SKILL && existSpecMobSkill < maxSpecMobSkill)
				{
					skillIDs.push_back(pDef->skillId);
					++existSpecMobSkill;
				}
			}
		}

		if (!skillIDs.empty()) {
			CSkillData skillData;
//			SkillMap* pSkillMap = SafeCreateObject(SkillMap);

			for (UInt16 j = 0; j < skillIDs.size(); ++j)
			{
				CSkill* pSkill = CSkill::NewInstance();
			
				skillData.SetOwner_info(GetMob_id(), 1); //0: char type 1: mob/partner type
				skillData.SetInfo(skillIDs[j]);
				skillData.SetLevel(1);
				skillData.SetCoolDown(1); //for temp
				pSkill->SetData(&skillData);
				m_SkillMap[pSkill->GetSkill_id()] = pSkill;
			}

			UpdateSkillBuff();
			CalcAttrib();
			SetMHP( GetMaxHP(), 0, 0);
			SetHP(GetMaxHP(), 0, 0);
			SetMSP( GetMaxSP(), 0, 0);
			SetSP(GetMaxSP(), 0, 0);
			ClearAllMask();
		}
	}
}

void CBatMob::InitPrevCmd()
{
	if (m_pPartner == NULL)
		return;

	m_command = m_pPartner->m_cmd;
	if (m_command == BATCMD_IDLE)			// no need to restore following if idle
		return;

	m_cmdTarget = 0;
	m_cmdTargetType = m_pPartner->m_cmdTargetType;
	m_cmdSubType = m_pPartner->m_sub_type;
	m_cmdAdditional = m_pPartner->m_additional;
	if (m_cmdTargetType == TARGET_ENEMY)
	{
		NextTarget(BOOLEAN_TRUE);
	}
	else if (m_cmdTargetType == TARGET_CHARACTER || 
		m_cmdTargetType == TARGET_PARTNER)
	{
		// get actor by target type and target id
		CBatActor* pTarget = m_pBattle->GetActor(m_cmdTargetType, m_pPartner->m_cmdTarget);
		if (pTarget)
			m_cmdTarget = pTarget->GetIndex();		// set target index if found
		else
			m_command = BATCMD_IDLE;				// otherwise, idle command
	}

}

void CBatMob::SaveLastCmd()
{
	if (m_pPartner == NULL)
		return;

	ResetAction();
	m_pPartner->m_cmd = m_command;
	if (m_command == BATCMD_IDLE)			// no need to store following if idle
		return;

	m_pPartner->m_cmdTarget = m_cmdTarget;
	m_pPartner->m_cmdTargetType = m_cmdTargetType;
	m_pPartner->m_sub_type = m_cmdSubType;
	m_pPartner->m_additional = m_cmdAdditional;
	// store target id instead of index
	CBatActor* pTarget = m_pBattle->GetActor(m_cmdTarget);
	if (pTarget) {
		if (pTarget->GetTeam() != GetTeam()) {
			m_pPartner->m_cmdTargetType = CBatActor::TARGET_ENEMY;
			m_pPartner->m_cmdTarget = 0;
		} else if (pTarget->IsPlayer()) {
			m_pPartner->m_cmdTargetType = CBatActor::TARGET_CHARACTER;
			m_pPartner->m_cmdTarget = pTarget->m_pBatChar->GetChar_id();
		} else if (pTarget->IsParnter()) {
			m_pPartner->m_cmdTargetType = CBatActor::TARGET_PARTNER;
			m_pPartner->m_cmdTarget = pTarget->m_pMob->GetPartner_id();
		} else {
			m_pPartner->m_cmd = BATCMD_IDLE;
		}
	} else {
		m_pPartner->m_cmd = BATCMD_IDLE;
	}
}