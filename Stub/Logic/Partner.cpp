//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/SettingTable.h"
#include "Common/Procedure.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Logic/Partner.h"
//-- Library
#include "Configure.h"
#include "Proxy/Service/ObjectService.h"
#include "Proxy/Data/BuffData.h"
#include "Proxy/Data/SkillData.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/SkillGroup.h"
#include "MapService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResSkill.h"
#include "Resource/ResSkillCombination.h"
#include "Resource/ResMob.h"
#include "Resource/ResBuff.h"
#include "Common/BattleCode.h"
#include "Stub/Logic/ChargeShop.h"
#include "MapService/antigameaddiction.h"

REGISTER_CLASSTYPE(CPartner, Partner_CLASSID);

PROTO_LuaPush(CPartner)
PROTO_LuaPush(AttriStruct)
PROTO_LuaPush(ExtractedBuffData)
PROTO_LuaPush(AttriStruct2)

CPartner::CPartner()
{
	m_iNewSkillID = 0;
	m_bHasAddSkill = false;
	m_skillMap.clear();
	m_pSkillGroup = CSkillGroup::NewInstance();
	m_pSkillGroup->SetOwner(this);
	m_LearnSkillID.clear();
	m_pOwnerGroup = NULL;
	m_data = NULL;
	m_pOwnerGroup = NULL;
	m_cmd = BATCMD_ATTACK;
	m_cmdTarget = 0;
	m_cmdTargetType = 1;
	m_sub_type = 0;
	m_additional = 0;
}

CPartner::~CPartner()
{
	CancelAllEvent();
	m_skillMap.clear();
	m_LearnSkillID.clear();
	DeleteNetGroup(m_pSkillGroup, BOOLEAN_TRUE);
}

void CPartner::SetPartnerGroup(CPartnerGroup* ownerGroup)
{
	// unset original ownership
	if (m_pOwnerGroup && m_pOwnerGroup != ownerGroup)
	{
		UnsetOwnObject(m_pOwnerGroup->GetOwnerContext().sessionId, this);
		UnsetOwnObject(m_pOwnerGroup->GetOwnerContext().sessionId, m_pSkillGroup);
	}

	// set new ownership
	m_pOwnerGroup = ownerGroup;
	if (ownerGroup) {
		SetOwnObject(m_pOwnerGroup->GetOwnerContext().sessionId, this);
		SetOwnObject(m_pOwnerGroup->GetOwnerContext().sessionId, m_pSkillGroup);
	}
}

void CPartner::CalcAttrib()
{
	bool needDecrease = false;
	if (m_data && (m_data->partner_type == LEVEL_FOLLOW_MOB_PARTNER || 
		m_data->partner_type == FORCE_BATTLE_PARTNER))
	{
		needDecrease = true;
	}

	CallLuaFunc(gGlobal.g_pLuaState, "CalPartnerAttr", String(), this, needDecrease);

	AttriStruct tempAttri;
	tempAttri.uIntAtk = (int)GetATK();
	tempAttri.uIntDef = (int)GetDEF();
	tempAttri.uIntHit = (int)GetHIT();
	tempAttri.uIntEva = (int)GetEVA();
	tempAttri.uIntPow = (int)GetPOW();
	tempAttri.uIntSpd = (int)GetSPD();
	tempAttri.uIntMaxHp = (int)GetMaxHP();
	tempAttri.uIntMaxSp = (int)GetMaxSP();
	tempAttri.uIntMaxDp = 0;
	tempAttri.uIntMaxWc = 0;
	for (SkillMap::iterator Itr = m_skillMap.begin(); Itr != m_skillMap.end(); ++Itr)
	{
		const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(Itr->second->GetSkill_id());
		if (pSkillData)
		{			
			StringA LuaFunc;
			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second->GetSkill_id(), "skill").c_str(), CommonSkillFuncName[SKILL_FUNC_NAME_ATTRI].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), this,  
					Itr->second->GetSkill_level(), &tempAttri);
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
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(),  this, 
							&Itr->second, &tempAttri);
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
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), this,  
							&Itr->second, &tempAttri);
				}
			}
		}
	}

	SetHP(GetHP(), (UInt16)tempAttri.uIntMaxHp);
	if (GetHP() > GetMaxHP())
		SetHP(GetMaxHP());
	
	SetSP(GetSP(), (UInt16)tempAttri.uIntMaxSp);
	if (GetSP() > GetMaxSP())
		SetSP(GetMaxSP());

	SetHasAddSkill(false);
	SetAttrib2( (UInt16)tempAttri.uIntAtk, (UInt16)tempAttri.uIntDef, (UInt16)tempAttri.uIntHit, (UInt16)tempAttri.uIntEva, (UInt16)tempAttri.uIntPow, (UInt16)tempAttri.uIntSpd);
	
}

void CPartner::CalcAttrib2()
{
	bool needDecrease = false;
	if (m_data && (m_data->partner_type == LEVEL_FOLLOW_MOB_PARTNER || 
		m_data->partner_type == FORCE_BATTLE_PARTNER))
	{
		needDecrease = true;
	}

	CallLuaFunc(gGlobal.g_pLuaState, "CalPartnerAttr", String(), this, needDecrease);

	AttriStruct tempAttri;
	tempAttri.uIntAtk = (int)GetATK();
	tempAttri.uIntDef = (int)GetDEF();
	tempAttri.uIntHit = (int)GetHIT();
	tempAttri.uIntEva = (int)GetEVA();
	tempAttri.uIntPow = (int)GetPOW();
	tempAttri.uIntSpd = (int)GetSPD();
	tempAttri.uIntMaxHp = (int)GetMaxHP();
	tempAttri.uIntMaxSp = (int)GetMaxSP();
	tempAttri.uIntMaxDp = 0;
	tempAttri.uIntMaxWc = 0;
	for (SkillMap::iterator Itr = m_skillMap.begin(); Itr != m_skillMap.end(); ++Itr)
	{
		const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(Itr->second->GetSkill_id());
		if (pSkillData)
		{			
			StringA LuaFunc;
			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second->GetSkill_id(), "skill").c_str(), CommonSkillFuncName[SKILL_FUNC_NAME_ATTRI].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), this,  
				Itr->second->GetSkill_level(), &tempAttri);
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
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(),  this, 
						&Itr->second, &tempAttri);
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
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), this,  
						&Itr->second, &tempAttri);
				}
			}
		}
	}

	SetHP(GetHP(), (UInt16)tempAttri.uIntMaxHp);
	if (GetHP() > GetMaxHP())
		SetHP(GetMaxHP());

	SetSP(GetSP(), (UInt16)tempAttri.uIntMaxSp);
	if (GetSP() > GetMaxSP())
		SetSP(GetMaxSP());

	SetHasAddSkill(false);
	SetAttrib2( (UInt16)tempAttri.uIntAtk, (UInt16)tempAttri.uIntDef, (UInt16)tempAttri.uIntHit, (UInt16)tempAttri.uIntEva, (UInt16)tempAttri.uIntPow, (UInt16)tempAttri.uIntSpd);

}

void CPartner::SetData(CPartnerData* pPartnerData)
{
	if (pPartnerData->IsUpdated(CPartnerData::idMask)) {
		SetId(pPartnerData->GetPartner_id(), pPartnerData->GetMob_id());
		m_pSkillGroup->SetOwner(GetPartner_id(), SKILL_OWNER_TYPE_PARTNER);
		m_data = gGlobal.m_resource->GetMobData(GetMob_id());
	}

	if (pPartnerData->IsUpdated(CPartnerData::ownerMask))
		SetOwner(pPartnerData->GetOwner_id());

	if (pPartnerData->IsUpdated(CPartnerData::infoMask))
		SetInfo(pPartnerData->GetRaname());

	if (pPartnerData->IsUpdated(CPartnerData::levelMask))
		SetLevel(pPartnerData->GetLevel());

	if (pPartnerData->IsUpdated(CPartnerData::expMask))
		SetExp(pPartnerData->GetExp());

	if (pPartnerData->IsUpdated(CPartnerData::attrib1Mask))
		SetAttrib1(pPartnerData->GetSTA(), pPartnerData->GetSPR(), 
			pPartnerData->GetSTR(), pPartnerData->GetCON(), pPartnerData->GetAGI());

	if (pPartnerData->IsUpdated(CPartnerData::attrPointMask))
		SetAttrPoint(pPartnerData->GetAttrPoint());

	if (pPartnerData->IsUpdated(CPartnerData::HPMask))
		SetHP(pPartnerData->GetHP(), pPartnerData->GetMaxHP());

	if (pPartnerData->IsUpdated(CPartnerData::SPMask))
		SetSP(pPartnerData->GetSP(), pPartnerData->GetMaxSP());

	if (pPartnerData->IsUpdated(CPartnerData::nativeMask))
		SetNative(pPartnerData->GetHP_native(), pPartnerData->GetSP_native(), 
			pPartnerData->GetATK_native(), pPartnerData->GetDEF_native(), pPartnerData->GetSPD_native(),
			pPartnerData->GetPOW_native(), pPartnerData->GetHIT_native(), pPartnerData->GetEVA_native());

	if (pPartnerData->IsUpdated(CPartnerData::growMask))
		SetGrow(pPartnerData->GetGrow());

	if (pPartnerData->IsUpdated(CPartnerData::favorMask))
		SetFavor(pPartnerData->GetFavor());

	if (pPartnerData->IsUpdated(CPartnerData::healthMask))
		SetHealth(pPartnerData->GetHealth());

	if (pPartnerData->IsUpdated(CPartnerData::goodnessMask))
		SetGoodness(pPartnerData->GetGoodness());

	if (pPartnerData->IsUpdated(CPartnerData::stateMask))
		SetState(pPartnerData->GetState());

	if (pPartnerData->IsUpdated(CPartnerData::buffMask))
	{
		SetBuff(pPartnerData->GetBuff());
		ExtractBuffString();
	}

	if (pPartnerData->IsUpdated(CPartnerData::soulboundStateMask))
		SetSoulboundState(pPartnerData->GetSoulboundState());

	if (pPartnerData->IsUpdated(CPartnerData::ExpireDateMask))
	{
		SetExpireDate(pPartnerData->GetExpireDate());
		if (m_data && m_data->TimeLimitType != PARTNER_TIME_LIMIT_TYPE_NONE &&
			GetExpireDate() != 0)
		{
			UInt32 now = (UInt32) time(NULL);
			if (GetExpireDate() > (UInt32)now)
				RaiseUniqEventID(EVT_UPDATE_PARTNER_EXPIRE, (GetExpireDate() - (UInt32)now) * 1000);
			else
				RaiseUniqEventID(EVT_UPDATE_PARTNER_EXPIRE);
		}
	}

	if (pPartnerData->IsUpdated(CPartnerData::ElementMask))
		SetElement(pPartnerData->GetElement());
	if (pPartnerData->IsUpdated(CPartnerData::OutLookColorMask))
		SetOutLookColor(pPartnerData->GetOutLookColor());
	if (pPartnerData->IsUpdated(CPartnerData::inWareHouseMask))
		SetInWareHouse(pPartnerData->GetInWareHouse());
	if (pPartnerData->IsUpdated(CPartnerData::assetLockMask))
		SetLock(pPartnerData->GetLock());

	if (pPartnerData->IsUpdated(CPartnerData::CultivationMask))
		SetCultivation(pPartnerData->GetCultivation());
	if (pPartnerData->IsUpdated(CPartnerData::PracticeMask))
		SetPractice(pPartnerData->GetPractice());

	SetGrowLevel(CalcGrowLevel());

	ClearAllMask();
}

RPCResult CPartner::RaiseAttrib(LPCPEER pPeer, RPCContext &context, const UInt16 &sta, 
								const UInt16 &spr, const UInt16 &str, const UInt16 &con, 
								const UInt16 &agi)
{
	UInt16 totalAp = sta + spr + str + con + agi;
	if (totalAp == 0) {
		CB_Error(pPeer, context, 1, _T("CPartner RaiseAttrib Error: empty attribute points assigned"));
		return RPC_RESULT_FAIL;
	} else if (totalAp > GetAttrPoint()) {
		CB_Error(pPeer, context, 1, _T("CPartner RaiseAttrib Error: not enough attribute points"));
		return RPC_RESULT_FAIL;
	}

	SetAttrib1(Parent::GetSTA() + sta, Parent::GetSPR() + spr, Parent::GetSTR() + str,
		Parent::GetCON() + con, Parent::GetAGI() + agi);
	SetAttrPoint(GetAttrPoint() - totalAp);
	CalcAttrib();

	RaiseUpdate();
	CB_RaiseAttrib(pPeer, context);

	return RPC_RESULT_OK;
}

void CPartner::ChangeHealth(Int change)
{
	if (IsUsePartnerCollection() && change < 0)
	{
		change *= 8;
	}
	SetHealth(MAX((Int) GetHealth() + change, 0));
	RaiseUpdate();
}

void CPartner::ChangeFavor(float change)
{
	SetFavor(WITHIN(GetFavor() + change, 0.0f, 100.0f));
	RaiseUpdate();
}

void CPartner::ChangeHP(Int change)
{
	SetHP(WITHIN((Int)GetHP() + change, 0, (Int)GetMaxHP()));
	RaiseUpdate();
}

void CPartner::ChangeSP(Int change)
{
	SetSP(WITHIN((Int)GetSP() + change, 0, (Int)GetMaxSP()));
	RaiseUpdate();
}

void CPartner::ChangeHP_native(Int change)
{
	SetHP_native(MAX((Int)GetHP_native() + change, 0));
	RaiseUpdate();
}

void CPartner::ChangeSP_native(Int change)
{
	SetSP_native(MAX((Int)GetSP_native() + change, 0));
	RaiseUpdate();
}
 
void CPartner::ChangeATK_native(Int change)
{
	SetATK_native(MAX((Int)GetATK_native() + change, 0));
	RaiseUpdate();
}

void CPartner::ChangeDEF_native(Int change)
{
	SetDEF_native(MAX((Int)GetDEF_native() + change, 0));
	RaiseUpdate();
}

void CPartner::ChangeSPD_native(Int change)
{
	SetSPD_native(MAX((Int)GetSPD_native() + change, 0));
	RaiseUpdate();
}

void CPartner::ChangePOW_native(Int change)
{
	SetPOW_native(MAX((Int)GetPOW_native() + change, 0));
	RaiseUpdate();
}

void CPartner::ChangeHIT_native(Int change)
{
	SetHIT_native(MAX((Int)GetHIT_native() + change, 0));
	RaiseUpdate();
}

void CPartner::ChangeEVA_native(Int change)
{
	SetEVA_native(MAX((Int)GetEVA_native() + change, 0));
	RaiseUpdate();
}

void CPartner::ChangeGrow(float change)
{
	SetGrow(MAX(GetGrow() + change, 0.0f));
	SetGrowLevel(CalcGrowLevel());
	RaiseUpdate();
}

void CPartner::SetFull(bool HP, bool SP, bool favor)
{
	if (HP) SetHP(GetMaxHP());
	if (SP) SetSP(GetMaxSP());
	if (favor) SetFavor(100.0f);
}

RPCResult CPartner::ChangePartnerName(LPCPEER pPeer, RPCContext &context, const String &newName)
{
	
	if (newName.length() == 0 || newName.length() > 8)
		return RPC_RESULT_FAIL;

	if (!gGlobal.CanUsedName(newName))
	{
		CB_ChangePartnerName(pPeer, context, false);
		return RPC_RESULT_FAIL;
	}

	SetInfo(newName);

	//change the partner name in the map if the partner is following the character
	if (m_pOwnerGroup && m_pOwnerGroup->GetOwner() && 
		m_pOwnerGroup->GetOwner()->GetPartner_id() == GetPartner_id())
	{
		m_pOwnerGroup->GetOwner()->SetPartner(m_pOwnerGroup->GetOwner()->GetPartner_id(),
			m_pOwnerGroup->GetOwner()->GetPartnerMob_id(), newName, 
			m_pOwnerGroup->GetOwner()->GetPartnerColorIndex());
	}

	RaiseUpdate();
	CB_ChangePartnerName(pPeer, context, true);

	return RPC_RESULT_OK;
}


RPCResult CPartner::ChangePartnerState(LPCPEER pPeer, RPCContext &context, 
									   const UInt8 &state)
{
	//if (gGlobal.m_pChargeShop)
	//	gGlobal.m_pChargeShop->InitChargeShop();

	SetState(state);
	RaiseUpdate();
	CB_ChangePartnerState(pPeer, context);

	return RPC_RESULT_OK;
}

void CPartner::ChangeState(UInt8 state)
{
	if (GetState() == state) return;			// no need to change

	if (state == ST_FIGHT && GetSoulboundState() == SOULBOUND_USE_BOUND)
		SetSoulboundState(SOULBOUND_ALREADY_BOUND);
	SetState(state);
	RaiseUpdate();
}

void CPartner::ChangeExp(Int32 change, Int32 exp_type)
{	
	CCharacter* pOwner = m_pOwnerGroup ? m_pOwnerGroup->GetOwner() : NULL;
	if (pOwner && pOwner->m_pAntiGameAddict && (pOwner->m_pAntiGameAddict->m_playerInterest < 1) && (change > 0))
	{
        change = (Int)(change * pOwner->m_pAntiGameAddict->m_playerInterest);
		UInt8 interest = (pOwner->m_pAntiGameAddict->m_playerInterest == 0.5)? 1 : 0;
		pOwner->CB_PlyInterestDwn(pOwner->GetPeer(), pOwner->GetContext(), interest);
	}
	SetExp(MAX((Int32) GetExp() + change, 0));
	LevelUp();					// try to level up after
	RaiseUpdate();
}

UInt8 CPartner::GetPartnerType()
{
	return (m_data ? m_data->partner_type : 0);
}

UInt16 CPartner::GetReqLv()
{
	return (m_data ? m_data->req_lv : 0);
}

UInt8 CPartner::GetRace()
{
	return (m_data ? m_data->race : 0);
}

void CPartner::ResetPartner()
{
	ResetPotential();
	ResetCultivation();
	ResetLevel(0);
	SetExp(0);
	if (m_pOwnerGroup && m_pOwnerGroup->GetOwner()) {
		PROCEDURE_DO_METHOD(CPartner, RegenSkill, this, m_pOwnerGroup->GetOwner()->GetContext())
	}
}

void CPartner::ResetPotential()
{
	//const MobData* pData = gGlobal.m_resource->GetMobData(GetMob_id());
	if ( m_data )
	{
		SetNative((UInt16)(GetRand(m_data->HP_native_d, m_data->HP_native_u)),
								(UInt16)(GetRand(m_data->SP_native_d, m_data->SP_native_u)),
								(UInt16)(GetRand(m_data->ATK_native_d, m_data->ATK_native_u)),
								(UInt16)(GetRand(m_data->DEF_native_d, m_data->DEF_native_u)),
								(UInt16)(GetRand(m_data->SPD_native_d, m_data->SPD_native_u)),
								(UInt16)(GetRand(m_data->POW_native_d, m_data->POW_native_u)),
								(UInt16)(GetRand(m_data->HIT_native_d, m_data->HIT_native_u)),
								(UInt16)(GetRand(m_data->EVA_native_d, m_data->EVA_native_u)) );

		this->SetGrow(GetRand(m_data->grow_d, m_data->grow_u) );

		this->SetGrowLevel(CalcGrowLevel());
	}
}

void CPartner::ResetCultivation()
{
	if ( m_data )
	{
		SetCultivation(0);
		if (m_data->race == 1)
		{
			if(GetRand(100) < 70)
				SetPractice(0);
			else
				SetPractice(GetRand(1, 10));
		}
		else
			SetPractice(0);
	}
}

void CPartner::ResetLevel(CONST UInt16 lv)
{
	SetLevel(lv);
	//SetExp(0);
	SetAttrPoint(lv * 5);

	//const MobData* pData = gGlobal.m_resource->GetMobData(GetMob_id());
	if ( m_data )
	{
		UInt8 potential = m_data->potential;
		if (potential > 9 )
			potential = 0;
		const MobPotentSet &f = gMobPotentSet[potential];

		UInt16 sta = GetRand(m_data->STA_base_d, m_data->STA_base_u) + f.sta;
		UInt16 spr = GetRand(m_data->SPR_base_d, m_data->SPR_base_u) + f.spr;
		UInt16 str = GetRand(m_data->STR_base_d, m_data->STR_base_u) + f.str;
		UInt16 con = GetRand(m_data->CON_base_d, m_data->CON_base_u) + f.con;
		UInt16 agi = GetRand(m_data->AGI_base_d, m_data->AGI_base_u) + f.agi;

		SetAttrib1(sta + lv, spr + lv, str + lv, con + lv, agi + lv);
	}

	CalcAttrib();
	//CalcNextLevel();
	SetHP(GetMaxHP(), GetMaxHP());
	SetSP(GetMaxSP(), GetMaxSP());

	RaiseUpdate();
}

RPCResult CPartner::RegenSkill(RPCContext &context)
{
	PROCEDURE_START_METHOD(CPartner, RegenSkill, this, context)

		UInt16Vector skill_ids;

		for (UInt8 i = 0; i < m_pSkillGroup->GetCount(); ++i)
		{
			CSkill* pSkill = m_pSkillGroup->GetChild<CSkill>(i);
			if (pSkill)
				skill_ids.push_back(pSkill->GetSkill_id());
		}

		if (skill_ids.size() > 0)
		{
			PROCEDURE_DO_METHOD1(CPartner, DoRemoveSkill, this, context, UInt16Vector, skill_ids)
		}
		else
		{
			PROCEDURE_GO_STEP(1)
		}
		
	PROCEDURE_WAIT_RETURN(1)

		MakeLearnSkillLst(true);

		UInt8 level = 1;
		bool forNewPartner = true;

		if (GetLearnSkillLst().size() > 0)
		{
			PROCEDURE_DO_METHOD3(CPartner, DoLearnSkill, this, context, UInt16Vector, GetLearnSkillLst(), UInt8, level, bool, forNewPartner)
			ClearLearnSkillLst();
		}
		else
		{
			ClearLearnSkillLst();
			PROCEDURE_GO_STEP(2)
		}

	PROCEDURE_WAIT_RETURN(2)

	PROCEDURE_CATCH

		//PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		//TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPartner UpdateToObject Error: ObjectService:CB_Error, errcode %d, %s"),
		//err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;

}

Boolean CPartner::CanLevelUp()
{
	CCharacter* pOwner = m_pOwnerGroup ? m_pOwnerGroup->GetOwner() : NULL;
	return (GetExp() >= GetNextLvExp() && 
		(pOwner == NULL || GetLevel() < pOwner->GetLevel() + 5) && 
		(m_data && m_data->partner_type != LEVEL_NOT_CHANGE_PARTNER && m_data->partner_type != LEVEL_NOT_CHANGE_FOLLOW_OWNER));
}

void CPartner::LevelUp()
{
	if (!CanLevelUp())
		return;

	bool needDecrease = false;
	if (m_data && (m_data->partner_type == LEVEL_FOLLOW_MOB_PARTNER || 
		m_data->partner_type == FORCE_BATTLE_PARTNER))
	{
		needDecrease = true;
	}

	do {
		SetExp(GetExp() - GetNextLvExp());	
		SetLevel(GetLevel() + 1);
		SetAttrib1(Parent::GetSTA() + 1, Parent::GetSPR() + 1, Parent::GetSTR() + 1, Parent::GetCON() + 1, Parent::GetAGI() + 1);
		SetAttrPoint(GetAttrPoint() + 5);
		MakeLearnSkillLst(false, true);
		CallLuaFunc(gGlobal.g_pLuaState, "CalPartnerAttr", String(), this, needDecrease);
	} while (CanLevelUp());

	CalcAttrib();
	SetHP((UInt16)GetMaxHP(), (UInt16)GetMaxHP());
	SetSP((UInt16)GetMaxSP(), (UInt16)GetMaxSP());
	RaiseUpdate();
}

void CPartner::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	/*case EVT_UPDATE_OBJECT:				// update all partner to object service
		if (m_pOwnerGroup) {
			PROCEDURE_DO_METHOD(CPartner, UpdateToObject, this, m_pOwnerGroup->GetOwnerContext())
		}
		break;*/
	case EVT_UPDATE_PARTNER_EXPIRE:
		if (m_pOwnerGroup)
		{
			if (m_data && m_data->TimeLimitType != PARTNER_TIME_LIMIT_TYPE_NONE &&
				GetExpireDate() != 0)
			{
				if (m_pOwnerGroup == NULL || m_pOwnerGroup->GetOwner() == NULL || m_pOwnerGroup->GetOwner()->GetPeer() == NULL)
				{
					//wait for valid peer
					RaiseUniqEventID(EVT_UPDATE_PARTNER_EXPIRE, 1000);
				}
				else
				{
					time_t now;
					time(&now);
					if (GetExpireDate() <= (UInt32)now)
					{
						RPCContext context = m_pOwnerGroup->GetOwnerContext();

						LPCPEER pPeer = m_pOwnerGroup->GetOwner()->GetPeer();
						PROCEDURE_DO_RPC1(PartnerGroup, DeletePartner, m_pOwnerGroup,  pPeer, context, UInt32, GetPartner_id())
					}
					else
						RaiseUniqEventID(EVT_UPDATE_PARTNER_EXPIRE, (GetExpireDate() - (UInt32)now) * 1000);
				}
			}
		}
	}
}

void CPartner::WriteData(CPartnerData* pPartnerData)
{
	TRACE_ENSURE(pPartnerData);

	pPartnerData->ClearUpdateMask();
	if (IsModified(infoMask))
		pPartnerData->SetInfo(GetRaname());
	if (IsModified(ownerMask))
		pPartnerData->SetOwner(GetOwner_id());
	if (IsModified(levelMask))
		pPartnerData->SetLevel(GetLevel());
	if (IsModified(expMask))
		pPartnerData->SetExp(GetExp());
	if (IsModified(attrib1Mask))
		pPartnerData->SetAttrib1(Parent::GetSTA(), Parent::GetSPR(), Parent::GetSTR(), Parent::GetCON(), Parent::GetAGI());
	if (IsModified(HPMask))
		pPartnerData->SetHP(GetHP(), GetMaxHP());
	if (IsModified(SPMask))
		pPartnerData->SetSP(GetSP(), GetMaxSP());
	if (IsModified(nativeMask))
		pPartnerData->SetNative(GetHP_native(), GetSP_native(), GetATK_native(),
			GetDEF_native(), GetSPD_native(), GetPOW_native(), GetHIT_native(), GetEVA_native() );
	if (IsModified(growMask))
		pPartnerData->SetGrow(GetGrow());
	if (IsModified(stateMask))
		pPartnerData->SetState(GetState());
	if (IsModified(buffMask))
		pPartnerData->SetBuff(GetBuff());
	if (IsModified(attrPointMask))
		pPartnerData->SetAttrPoint(GetAttrPoint());
	if (IsModified(favorMask))
		pPartnerData->SetFavor(GetFavor());
	if (IsModified(healthMask))
		pPartnerData->SetHealth(GetHealth());
	if (IsModified(goodnessMask))
		pPartnerData->SetGoodness(GetGoodness());
	if (IsModified(soulboundStateMask))
		pPartnerData->SetSoulboundState(GetSoulboundState());
	if (IsModified(ExpireDateMask))
		pPartnerData->SetExpireDate(GetExpireDate());
	if (IsModified(ElementMask))
		pPartnerData->SetElement(GetElement());
	if (IsModified(OutLookColorMask))
		pPartnerData->SetOutLookColor(GetOutLookColor());
	if (IsModified(inWareHouseMask))
		pPartnerData->SetInWareHouse(GetInWareHouse());
	if (IsModified(assetLockMask))
		pPartnerData->SetLock(GetLock());
	if (IsModified(CultivationMask))
		pPartnerData->SetCultivation(GetCultivation());
	if (IsModified(PracticeMask))
		pPartnerData->SetPractice(GetPractice());

	if (pPartnerData->IsUpdated()) {
		pPartnerData->SetId(GetPartner_id(), GetMob_id());
		pPartnerData->SetSendMask(pPartnerData->GetUpdateMask());
	}
}

Boolean CPartner::NeedUpdate()
{
	RPCMask mask;

	mask.MaskBits(infoMask, ownerMask, levelMask, expMask, attrib1Mask, HPMask, SPMask, nativeMask,
		stateMask, buffMask, attrPointMask, favorMask, healthMask, goodnessMask, ExpireDateMask,
		OutLookColorMask, inWareHouseMask, assetLockMask, CultivationMask, PracticeMask, growLevelMask, -1);
	return IsModified(mask);
}

void CPartner::PreSend(NetGroup* skillGroup)
{
	SetSendMask(CPartner::AllMask);
	m_pSkillGroup->PreSend();
	if (skillGroup)
		skillGroup->Add(m_pSkillGroup);
}

RPCResult CPartner::DoLearnSkill(RPCContext &context)
{
	PROCEDURE_START_METHOD3(CPartner, DoLearnSkill, this, context, UInt16Vector, skill_ids, 
							UInt8, Level, Boolean, forNewPartner)

		if (skill_ids.empty()) {
			PROCEDURE_RETURN
		} else {
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("DoLearnSkill: sessionId %s, partner_id %d, skill_ids %s"),
				context.sessionId.GetString().c_str(), GetPartner_id(), 
				GetVectorString(skill_ids).c_str());
		}

		UInt32Vector owner_ids;
		UInt16Vector skill_levels;
		owner_ids.resize(skill_ids.size());
		skill_levels.resize(skill_ids.size());
		std::fill_n(owner_ids.begin(), skill_ids.size(), GetPartner_id());
		std::fill_n(skill_levels.begin(), skill_ids.size(), Level);
		ObjectService_Proxy::CreateSkillList(gGlobal.pObjectService->pPeer, context, 
			SKILL_OWNER_TYPE_PARTNER, owner_ids, skill_ids, skill_levels);
	
	PROCEDURE_WAIT1(1, ObjectService, CB_GetSkillList, NetGroup*, skillDataGroup)

		TRACE_ENSURE(skillDataGroup);

		NetGroup grp;
		for (Int i = 0; i < skillDataGroup->GetCount(); i++)
		{
			CSkillData * pSkillData = skillDataGroup->GetChild<CSkillData >(i);
			if (pSkillData )
			{
				const SkillData * pSkillDataStr = gGlobal.m_resource->GetSkillData(pSkillData->GetSkill_id());
				if (pSkillDataStr)
				{
					CSkill * pSkill = CSkill::NewInstance();
					pSkill->SetData(pSkillData);
					pSkill->PreSend();
					
					m_pSkillGroup->Add(pSkill);
					grp.Add(pSkill);
					m_skillMap.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));
					
					if (pSkillDataStr->type == SKILL_TYPE_PARTNER_SPECIAL)
						pSkill->SetPartnerSkillType(PARTNER_SPECIAL_SKILL);
					else 
						pSkill->SetPartnerSkillType(PARTNER_NORMAL_SKILL);
				}	
			}
		}

		CalcAttrib();
		if (forNewPartner)
		{
			SetHP(GetMaxHP(), GetMaxHP());
			SetSP(GetMaxSP(), GetMaxSP());
		}

		DeleteNetGroup(skillDataGroup, BOOLEAN_TRUE);
		if (m_pOwnerGroup && m_pOwnerGroup->GetOwner())
		{
			m_pSkillGroup->CB_AddSkills(m_pOwnerGroup->GetOwner()->GetPeer(), 
				m_pOwnerGroup->GetOwner()->GetContext(), &grp);
			if (m_iNewSkillID > 0)
			{
				CB_LearnSkillMessage(m_pOwnerGroup->GetOwner()->GetPeer(), m_pOwnerGroup->GetOwner()->GetContext(), m_iNewSkillID);
				m_iNewSkillID = 0;
			}
		}

		if (IsUpdated())
			RaiseUpdate();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPartner DoLearnSkill Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CPartner::DoRemoveSkill(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CPartner, DoRemoveSkill, this, context, UInt16Vector, skill_ids)

		if (skill_ids.empty()) {
			PROCEDURE_RETURN
		} else {
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("DoRemoveSkill: sessionId %s, partner_id %d, skill_ids %s"),
				context.sessionId.GetString().c_str(), GetPartner_id(), 
				GetVectorString(skill_ids).c_str());
		}

		ObjectService_Proxy::DeleteSkillList(gGlobal.pObjectService->pPeer, context, 
			GetPartner_id(), skill_ids, SKILL_OWNER_TYPE_PARTNER);

	PROCEDURE_WAIT(1, ObjectService, CB_DeleteSKillDone)

		for (UInt16Vector::const_iterator it = skill_ids.begin(); it != skill_ids.end(); ++it)
		{
			for (UInt16 i = 0; i < m_pSkillGroup->GetCount(); ++i)
			{
				CSkill * pChild = m_pSkillGroup->GetChild<CSkill>(i);
				if (pChild && pChild->GetSkill_id() == *it)
				{
					if (m_cmd == BATCMD_SKILL && m_sub_type == pChild->GetSkill_id())
					{
						m_cmd = BATCMD_ATTACK;
						m_sub_type = 0;
					}
					m_pSkillGroup->Remove(pChild);
					DeleteNetObject(pChild);
					break;
				}
			}

			SkillMap::iterator Itr = m_skillMap.find(*it);
			if (Itr != m_skillMap.end())
			{
				m_skillMap.erase(Itr);
			}
		}
		CalcAttrib();
		if (m_pOwnerGroup && m_pOwnerGroup->GetOwner())
			m_pSkillGroup->CB_RemoveSkills(m_pOwnerGroup->GetOwner()->GetPeer(), 
				m_pOwnerGroup->GetOwner()->GetContext(), skill_ids);

		if (IsUpdated())
			RaiseUpdate();


	PROCEDURE_CATCH

	PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPartner DoLearnSkill Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}


void CPartner::RemoveSkill(UInt16 skillId)
{
	SkillMap::iterator Itr = m_skillMap.find(skillId);
	if (Itr != m_skillMap.end())
	{
		UInt16Vector IdLst;
		gGlobal.m_resource->GetSkillLink(skillId, IdLst);

		PROCEDURE_DO_METHOD1(CPartner, DoRemoveSkill, this, m_pOwnerGroup->GetOwner()->GetContext(), UInt16Vector, IdLst)
	}
}

void CPartner::ClearSkillMap()
{
	m_skillMap.clear();
	DeleteNetChilds(m_pSkillGroup);
}

Boolean CPartner::AddSkill(CSkill* pSkill)
{
	// no skill or skill data
	if (pSkill == NULL || pSkill->m_data == NULL)
		return BOOLEAN_FALSE;

	if (m_skillMap.find(pSkill->GetSkill_id()) != m_skillMap.end())
		return BOOLEAN_FALSE;

	m_pSkillGroup->Add(pSkill);
	SetHasAddSkill(true);
	m_skillMap.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));

	const SkillData* tempData = gGlobal.m_resource->GetSkillData(pSkill->GetSkill_id());
	if (tempData)
	{
		if (tempData->type == SKILL_TYPE_PARTNER_SPECIAL)
			pSkill->SetPartnerSkillType(PARTNER_SPECIAL_SKILL);
		else //if (tempData->type == SKILL_TYPE_PARTNER_NORMAL)
			pSkill->SetPartnerSkillType(PARTNER_NORMAL_SKILL);
		
	}
	
	return BOOLEAN_TRUE;
}

void CPartner::AddSkill(UInt16 skillId)
{
	if (m_pSkillGroup == NULL)
		return;

	// check if it is a partner skill
	const SkillData* skillData = gGlobal.m_resource->GetSkillData(skillId);

	if (skillData == NULL)
		return;

	if (skillData->type != SKILL_TYPE_PARTNER_NORMAL && 
		skillData->type != SKILL_TYPE_PARTNER_SPECIAL)
		return;

	SkillMap::iterator itr = m_skillMap.find(skillId);
	if (itr == m_skillMap.end())
	{
		UInt16Vector idLst;
		idLst.push_back(skillId);
		PROCEDURE_DO_METHOD3(CPartner, DoLearnSkill, this, 
			m_pSkillGroup->GetOwnerContext(), UInt16Vector, idLst, UInt8, 1, 
			Boolean, BOOLEAN_FALSE)
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CPartner AddSkill Warning : skill %d exist"),
			skillId);
	}
}

NetGroup* CPartner::GetSkillGroup(bool forSend)
{
	if (forSend)
	{
		m_pSkillGroup->PreSend();
		for (UInt16 i = 0; i < m_pSkillGroup->GetCount(); ++i)
		{
			CSkill * pSkill = m_pSkillGroup->GetChild<CSkill>(i);
			if (pSkill)
				pSkill->PreSend();
		}
	}

	return m_pSkillGroup;
}

void CPartner::MakeLearnSkillLst(bool forNewPartner, bool toObject)
{
	m_LearnSkillID.clear();
	UInt existSpecMobSkill = 0;
	UInt existNormMobSkill = 0;
	UInt maxSpecMobSkill = 7;
	UInt maxNormMobSkill = 0;
	
	//const MobData *pData = gGlobal.m_resource->GetMobData(GetMob_id());
	if (m_data == NULL)
		return;

	StringA mobSkillFile;
	mobSkillFile.Format("MobSkill%05d.csv",m_data->skill_file_id); 
	const MobSkillCombination* mobSkillLst = gGlobal.m_resource->GetMobSkillCombination(mobSkillFile.c_str());
	if (mobSkillLst == NULL)
		return;

	switch (m_data->talent)
	{
	case MOB_TALENT_A:	maxNormMobSkill = gConfigure.learnSkillMaxNormMobSkillA;	break;
	case MOB_TALENT_B:	maxNormMobSkill = gConfigure.learnSkillMaxNormMobSkillB;	break;
	case MOB_TALENT_C:	maxNormMobSkill = gConfigure.learnSkillMaxNormMobSkillC;	break;
	}

	if (forNewPartner)
	{
		m_iNewSkillID = 0;
		for (Index i = C_INDEX(0); i < mobSkillLst->skillLst.size(); ++i)
		{
			UInt32 rand = (UInt32)(GetRand(0.0f, 100.0f) * 1000);
			MobSkillDef* pDef = mobSkillLst->skillLst[i];
			if (pDef && rand <= pDef->skillRate)
			{
				if ( (pDef->skillType == PARTNER_NORMAL_SKILL && existNormMobSkill < maxNormMobSkill) )
				{
					m_LearnSkillID.push_back(pDef->skillId);
					++existNormMobSkill;
				}
				else if (pDef->skillType == PARTNER_SPECIAL_SKILL && existSpecMobSkill < maxSpecMobSkill)
				{
					m_LearnSkillID.push_back(pDef->skillId);
					++existSpecMobSkill;
				}		
			}
		}
	}
	else
	{
		// 1000 = 1%
		UInt32 iRand = (UInt32)GetRand(0.0f, 100.0f) * 1000;
		UInt32 successRate = gConfigure.learnSkillSuccessRate;
		if (iRand <= successRate)
		{
			existNormMobSkill = GetNormalSkillCount();

			if (existNormMobSkill < 3)
			{
				for (UInt8 i = 0; i < mobSkillLst->skillLst.size(); ++i)
				{
					UInt32 rand = (UInt32)(GetRand(0.0f, 100.0f) * 1000);
					MobSkillDef* pDef = mobSkillLst->skillLst[i];
					if (pDef && rand <= pDef->skillRate && pDef->skillType == PARTNER_NORMAL_SKILL)
					{
						m_iNewSkillID = pDef->skillId;
						if (m_skillMap.find(pDef->skillId) != m_skillMap.end())
							break;

						if (pDef->skillType == PARTNER_NORMAL_SKILL && existNormMobSkill < maxNormMobSkill) 
						{
							m_LearnSkillID.push_back(pDef->skillId);
							++existNormMobSkill;
							break;
						}	
					}
				}
			}
		}	
	}

	if (toObject && m_pOwnerGroup && !m_LearnSkillID.empty()) {
		PROCEDURE_DO_METHOD3(CPartner, DoLearnSkill, this, m_pOwnerGroup->GetOwnerContext(),
			UInt16Vector, m_LearnSkillID, UInt8, 1, bool, forNewPartner)
		m_LearnSkillID.clear();
	}
}

bool CPartner::HasSkill(UInt16 skillID)
{
	if (m_skillMap.find(skillID) != m_skillMap.end())
		return true;
	return false;
}


UInt16	CPartner::GetRandomSkillID(UInt16 Type)
{
	if(m_skillMap.size())
	{
		if(Type == 0)
		{
			UInt32 Count = 0;
			UInt32 r = gGlobal.m_rand.GetRandom(GetNormalSkillCount());
			for (SkillMap::iterator itr = m_skillMap.begin(); itr != m_skillMap.end(); ++itr)
			{
				if (itr->second->GetPartner_skill_type() == PARTNER_NORMAL_SKILL)
				{
					if(Count == r)
						return itr->first;
					++Count;
				}
			}
		}
		else
		if(Type == 1)
		{
			UInt32 Count = 0;
			UInt32 r = gGlobal.m_rand.GetRandom(GetSpecialSkillCount());
			for (SkillMap::iterator itr = m_skillMap.begin(); itr != m_skillMap.end(); ++itr)
			{
				if (itr->second->GetPartner_skill_type() == PARTNER_SPECIAL_SKILL)
				{
					if(Count == r)
						return itr->first;
					++Count;
				}
			}
		}
		else
		if(Type == 2)
		{
			UInt32 Count = 0;
			UInt32 r = gGlobal.m_rand.GetRandom((Int)m_skillMap.size());
			for (SkillMap::iterator itr = m_skillMap.begin(); itr != m_skillMap.end(); ++itr)
			{
				if(Count == r)
					return itr->first;
				++Count;
			}
		}
	}

	return 0;
}

UInt8 CPartner::GetNormalSkillCount()
{
	UInt8 count = 0;
	for (SkillMap::iterator itr = m_skillMap.begin(); itr != m_skillMap.end(); ++itr)
	{
		if (itr->second->GetPartner_skill_type() == PARTNER_NORMAL_SKILL)
			++count;
	}
	return count;
}

UInt8 CPartner::GetSpecialSkillCount()
{
	UInt8 count = 0;
	for (SkillMap::iterator itr = m_skillMap.begin(); itr != m_skillMap.end(); ++itr)
	{
		if (itr->second->GetPartner_skill_type() == PARTNER_SPECIAL_SKILL)
			++count;
	}
	return count;
}

///////////////////////////////////////////////////////////////////////////////
///																			///
///								Buff methods								///
///																			///
///////////////////////////////////////////////////////////////////////////////

bool CPartner::AddBuff(UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro)
{
	ExtractedBuffData Data;
	Data.ID = Buff_id;
	Data.CoolDown = Buff_CoolDown;
	Data.Value1 = Buff_Value1;
	Data.Value2 = Buff_Value2;
	Data.Value3 = Buff_Value3;
	Data.Pro = Pro;

	bool added = false;
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		Data.pBuffData = pBuffData;
		if (pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_MINUTES)
			Data.CoolDown *= 60;
		if (pBuffData->Buff_RemoveAftOffline)
		{
			BuffMap::iterator Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
			if (Itr != m_BuffCacheMap.end())
			{
				if (Data >= Itr->second)
				{
					m_BuffCacheMap.insert(std::make_pair(pBuffData->Buff_school, Data));
					ConstructBuffCacheString();
					added = true;
				}
			}
			else
			{
				m_BuffCacheMap.insert(std::make_pair(pBuffData->Buff_school, Data));
				ConstructBuffCacheString();
				added = true;
			}
		}
		else
		{
			if (m_BuffMap.size() == MAX_BUFF_NUM)
				return false;
			BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
			if (Itr != m_BuffMap.end())
			{
				if (Data >= Itr->second)
				{
					m_BuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));
					ConstructBuffString();
					added = true;
				}
			}
			else
			{
				m_BuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));
				ConstructBuffString();
				added = true;
			}
		}

		if (added)
		{
			
			StringA LuaFunc;

			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
				CommonBuffFuncName[BUFF_FUNC_NAME_ADD].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), &Data,  this);

			for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
			{
				if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
					CalcAttrib();
			}
		}
	}

	return added;
}

void CPartner::RemoveBuff(UInt16 Buff_id)
{
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffMap.end() &&
			Itr->second.ID == Buff_id)
		{
			m_BuffMap.erase(Itr);
			ConstructBuffString();
			return;
		}

		Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffCacheMap.end() &&
			Itr->second.ID == Buff_id)
		{
			m_BuffCacheMap.erase(Itr);
			ConstructBuffCacheString();
			return;
		}
	}
}

void CPartner::RemoveBuffSchool(UInt16 school_id)
{
	BuffMap::iterator it = m_BuffMap.find(school_id);
	if (it != m_BuffMap.end()) {
		RemoveBuff(it->second.ID);
	} else {
		it = m_BuffCacheMap.find(school_id);
		if (it != m_BuffCacheMap.end())
			RemoveBuff(it->second.ID);
	}
}

bool CPartner::HasBuffSchool(UInt16 school_id)
{
	BuffMap::iterator it = m_BuffMap.find(school_id);
	if (it != m_BuffMap.end()) {
		return true;
	} else {
		it = m_BuffCacheMap.find(school_id);
		if (it != m_BuffCacheMap.end())
			return true;
	}
	return false;
}

bool CPartner::HasBuff(UInt16 Buff_id)
{
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffMap.end() &&
			Itr->second.ID == Buff_id)
		{
			return true;
		}

		Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffCacheMap.end() &&
			Itr->second.ID == Buff_id)
			return true;
	}
	return false;
}

ExtractedBuffData * CPartner::FindBuff(UInt16 Buff_id)
{
	
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffMap.end() &&
			Itr->second.ID == Buff_id)
		{
			return &Itr->second;
		}

		Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffCacheMap.end() &&
			Itr->second.ID == Buff_id)
			return &Itr->second;
	}
	return NULL;
}

VOID CPartner::ExtractBuffCacheString()
{
	m_BuffCacheMap.clear();
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	TCHAR *value = NULL; 
	TCHAR *buf = SafeAllocate(TCHAR, GetBuffCache().size() + 1);
	buf[GetBuffCache().size()] = _T('\0');
	_tcscpy(buf, GetBuffCache().c_str());
	_tcstok(buf, valueDelim);
	bool first = true;
	while (true)
	{
		ExtractedBuffData Data;
		//buff id
		if (first)
		{
			if (buf)
				Data.ID = _ttoi(buf);
			else
				break;
			first = false;
		}
		else
		{

			value = _tcstok(NULL, valueDelim);
			if (value)
				Data.ID = _ttoi(value);
			else
				break;
		}

		//buff Value1
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value1 = _ttoi(value);
		else
			break;

		//buff Value2
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value2 = _ttoi(value);
		else
			break;

		//DOT Value3
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value3 = _ttoi(value);
		else
			break;

		//Pro
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Pro = _ttoi(value);
		else
			break;

		//buff CoolDown
		value = _tcstok(NULL, buffDelim);
		if (value)
		{
			Data.CoolDown = _ttoi(value);
			while (value && (((*value >= 48) && (*value <= 57)) || (*value == 44) || (*value == 130))) // while the values are numbers or ","while (value != NULL)
			{
				if (value && (*value == 44 || *value == 130)) // check if there is a comma "," in the string
					break;
				value++;
			}
			if (value && (*value == 44 || *value == 130))// buff Repeat
			{
				value++;
				if (value && (*value >= 48) && (*value <= 57))
				{
					Data.Repeat = _ttoi(value);
				}
				else
					Data.Repeat = 0;
			}
			else
				Data.Repeat = 0;
		}
		else
			break;

		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Data.ID);
		if (pBuffData)
		{
			Data.pBuffData = pBuffData;
			m_BuffCacheMap.insert(std::make_pair(pBuffData->Buff_school, Data));
		}
	}
	SafeDeallocate(buf);
}

void CPartner::ConstructBuffCacheString()
{
	String BuffList;
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); ++ Itr)
	{
		String Buff;
		Buff.Format(_T("%d%s%d%s%d%s%d%s%d%s%d%s%d%s"), Itr->second.ID, valueDelim, Itr->second.Value1,
			valueDelim, Itr->second.Value2, valueDelim, Itr->second.Value3, valueDelim, Itr->second.Pro, valueDelim, Itr->second.CoolDown, valueDelim, Itr->second.Repeat, buffDelim);
		BuffList += Buff.c_str();
	}

	SetBuffCache(BuffList);
}

//extract the buff string
VOID CPartner::ExtractBuffString()
{
	m_BuffMap.clear();
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	TCHAR *value = NULL; 
	TCHAR *buf = SafeAllocate(TCHAR, GetBuff().size() + 1);
	buf[GetBuff().size()] = _T('\0');
	_tcscpy(buf, GetBuff().c_str());
	_tcstok(buf, valueDelim);
	bool first = true;
	while (true)
	{
		ExtractedBuffData Data;
		//buff id
		if (first)
		{
			if (buf)
				Data.ID = _ttoi(buf);
			else
				break;
			first = false;
		}
		else
		{

			value = _tcstok(NULL, valueDelim);
			if (value)
				Data.ID = _ttoi(value);
			else
				break;
		}

		//buff Value1
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value1 = _ttoi(value);
		else
			break;

		//buff Value2
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value2 = _ttoi(value);
		else
			break;

		//DOT Value3
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value3 = _ttoi(value);
		else
			break;

		//Pro
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Pro = _ttoi(value);
		else
			break;

		//buff CoolDown
		value = _tcstok(NULL, buffDelim);
		if (value)
		{
			Data.CoolDown = _ttoi(value);
			while (value && (((*value >= 48) && (*value <= 57)) || (*value == 44) || (*value == 130))) // while the values are numbers or ","
			{
				if (value && (*value == 44 || *value == 130)) // check if there is a comma "," in the string
					break;
				value++;
			}
			if (value && (*value == 44 || *value == 130))// buff Repeat
			{
				value++;
				if (value && (*value >= 48) && (*value <= 57))
				{
					Data.Repeat = _ttoi(value);
				}
				else
					Data.Repeat = 0;
			}
			else
				Data.Repeat = 0;
		}
		else
			break;

		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Data.ID);
		if (pBuffData)
		{
			Data.pBuffData = pBuffData;
			m_BuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));
		}
	}
	SafeDeallocate(buf);
}

void CPartner::ConstructBuffString()
{
	String BuffList;
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); ++ Itr)
	{
		String Buff;
		Buff.Format(_T("%d%s%d%s%d%s%d%s%d%s%d%s%d%s"), Itr->second.ID, valueDelim, Itr->second.Value1,
			valueDelim, Itr->second.Value2, valueDelim, Itr->second.Value3, valueDelim, Itr->second.Pro, valueDelim, Itr->second.CoolDown, valueDelim, Itr->second.Repeat, buffDelim);
		BuffList += Buff.c_str();
	}

	SetBuff(BuffList);
}

void CPartner::SetSkillCoolDown(UInt16 ID, DWORD countDown)
{
	SkillMap::iterator Itr = m_skillMap.find(ID);
	if (Itr != m_skillMap.end())
	{
		Itr->second->SetCoolDown(countDown);
		m_pSkillGroup->RaiseUpdate();
	}
}

LPCPEER CPartner::GetOwnerPeer()
{
	return (m_pOwnerGroup && m_pOwnerGroup->GetOwner() ? 
				m_pOwnerGroup->GetOwner()->GetPeer() : NULL);
}

RPCContext &CPartner::GetOwnerContext()
{
	static RPCContext dummyContext;

	return (m_pOwnerGroup ? m_pOwnerGroup->GetOwnerContext() : dummyContext);
}

void CPartner::ChangeOutlookColor(UInt16 colorIndex)
{
	SetOutLookColor(colorIndex);

	if (m_pOwnerGroup && m_pOwnerGroup->GetOwner() && 
		m_pOwnerGroup->GetOwner()->GetPartner_id() == GetPartner_id())
	{
		m_pOwnerGroup->GetOwner()->SetPartner(m_pOwnerGroup->GetOwner()->GetPartner_id(),
			m_pOwnerGroup->GetOwner()->GetPartnerMob_id(), 
			m_pOwnerGroup->GetOwner()->GetPartnerName(), colorIndex);
	}

	RaiseUpdate();
}

void CPartner::ClonePartner(CPartner* pPartner)
{
	if (pPartner)
	{
		pPartner->SetId(GetPartner_id(), GetMob_id());
		pPartner->m_data = m_data;
		pPartner->SetOwner(GetOwner_id());
		pPartner->SetInfo(GetRaname());
		pPartner->SetLevel(GetLevel());

		pPartner->SetExp(GetExp());
		pPartner->SetAttrib1(Parent::GetSTA(), Parent::GetSPR(), Parent::GetSTR(), Parent::GetCON(), Parent::GetAGI());
		pPartner->SetAttrib2(GetATK(), GetDEF(), GetHIT(), GetEVA(), GetPOW(), GetSPD());

		pPartner->SetAttrPoint(GetAttrPoint());
		pPartner->SetHP(GetHP(), GetMaxHP());
		pPartner->SetSP(GetSP(), GetMaxSP());

		pPartner->SetNative(GetHP_native(), GetSP_native(), GetATK_native(), 
			GetDEF_native(), GetSPD_native(), GetPOW_native(), GetHIT_native(), GetEVA_native());

		pPartner->SetGrow(GetGrow());
		pPartner->SetFavor(GetFavor());
		pPartner->SetHealth(GetHealth());

		pPartner->SetGoodness(GetGoodness());
		pPartner->SetState(GetState());
		pPartner->SetBuff(GetBuff());

		pPartner->ExtractBuffString();
		pPartner->SetSoulboundState(GetSoulboundState());
		pPartner->SetExpireDate(GetExpireDate());

		pPartner->SetElement(GetElement());
		pPartner->SetOutLookColor(GetOutLookColor());
		pPartner->SetInWareHouse(GetInWareHouse());

		pPartner->SetCultivation(GetCultivation());
		pPartner->SetPractice(GetPractice());

		pPartner->SetGrowLevel(pPartner->CalcGrowLevel());
	}
}

void CPartner::RaiseUpdate()
{
	if (m_pOwnerGroup)
		m_pOwnerGroup->RaiseUpdate();
}

void CPartner::CancelUpdate()
{
	if (m_pOwnerGroup)
		m_pOwnerGroup->CancelUpdate();
}

Boolean CPartner::ReverseAttrib(UInt8 attrType, UInt16 point)
{
	if (m_data == NULL)
		return BOOLEAN_FALSE;

	UInt16 level = GetLevel();
	UInt16 sta = Parent::GetSTA();
	UInt16 spr = Parent::GetSPR();
	UInt16 str = Parent::GetSTR();
	UInt16 con = Parent::GetCON();
	UInt16 agi = Parent::GetAGI();

	switch(attrType)
	{
	case 1:
		if (sta < point || sta - point < m_data->STA_base_d + level)
			return BOOLEAN_FALSE;
		sta -= point;
		break;
	case 2:
		if (spr < point || spr - point < m_data->SPR_base_d + level)
			return BOOLEAN_FALSE;
		spr -= point;
		break;
	case 3:
		if (str < point || str - point < m_data->STR_base_d + level)
			return BOOLEAN_FALSE;
		str -= point;
		break;
	case 4:
		if (con < point || con - point < m_data->CON_base_d + level)
			return BOOLEAN_FALSE;
		con -= point;
		break;
	case 5:
		if (agi < point || agi - point < m_data->AGI_base_d + level)
			return BOOLEAN_FALSE;
		agi -= point;
		break;
	default:
		return BOOLEAN_FALSE;
	}

	SetAttrib1(sta, spr, str, con, agi);
	SetAttrPoint(GetAttrPoint() + point);
	
	CalcAttrib();
	SetHP(GetMaxHP(), GetMaxHP());
	SetSP(GetMaxSP(), GetMaxSP());
	RaiseUpdate();

	return BOOLEAN_TRUE;
}

const UInt16 CPartner::GetSTA()
{
	return Parent::GetSTA() + m_AttriOfPartnerCollection.GetSta();
}

const UInt16 CPartner::GetSPR()
{
	return Parent::GetSPR() + m_AttriOfPartnerCollection.GetSpr();
}

const UInt16 CPartner::GetSTR()
{
	return Parent::GetSTR() + m_AttriOfPartnerCollection.GetStr();
}

const UInt16 CPartner::GetCON()
{
	return Parent::GetCON() + m_AttriOfPartnerCollection.GetCon();
}

const UInt16 CPartner::GetAGI()
{
	return Parent::GetAGI() + m_AttriOfPartnerCollection.GetAgi();
}

bool CPartner::IsUsePartnerCollection()
{
	return (m_AttriOfPartnerCollection.GetSta() > 0
		|| m_AttriOfPartnerCollection.GetSpr() > 0
		|| m_AttriOfPartnerCollection.GetStr() > 0
		|| m_AttriOfPartnerCollection.GetCon() > 0
		|| m_AttriOfPartnerCollection.GetAgi() > 0);
}

void CPartner::ChangeCultivation(UInt16 change)
{
	UInt16 cultivation = MAX((Int16)GetCultivation() + change, 0);
	if (cultivation > 10)
		cultivation = 10;
	SetCultivation(cultivation);
	CalcAttrib();
	RaiseUpdate();
}

void CPartner::ChangePractice(UInt16 change)
{
	SetPractice(MAX((Int16)GetPractice() + change, 0));
	RaiseUpdate();
}

UInt8 CPartner::CalcGrowLevel()
{
	gGlobal.g_TempResult = 0;
	UInt16 grow = (UInt16)(GetGrow() * 1000);
	UInt16 grow_d = (UInt16)(m_data->grow_d * 1000);
	UInt16 grow_u = (UInt16)(m_data->grow_u * 1000);
	CallLuaFunc(gGlobal.g_pLuaState, "GetPartnerGrowLevel", String(), grow, grow_d, grow_u);
	UInt8 level = (UInt8)gGlobal.g_TempResult;
	return level;
}