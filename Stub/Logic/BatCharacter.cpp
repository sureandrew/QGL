//-- Common
#include "Common.h"
#include "Common/StateCode.h"
#include "Common/BattleCode.h"
#include "Common/SettingTable.h"
#include "Common/dataCode.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Logic/BatCharacter.h"
//-- Library
#include "Stub/Logic/BatMob.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Item.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/Character.h"
#include "MapService/Global.h"
#include "MapService/AnimTimer.h"
#include "Resource/ResMob.h"
#include "Resource/ResSkill.h"
#include "Resource/ResBuff.h"
#include "Resource/ResCharacter.h"
#include "Resource/ServerResManager.h"
#include "Party.h"
#include "Stub/Logic/PetGroup.h"
#include "Proxy/Data/SkillData.h"
#include "Resource/ResSkillCombination.h"

REGISTER_CLASSTYPE(CBatCharacter, BatCharacter_CLASSID);

PROTO_LuaPush(AttriStruct)
PROTO_LuaPush(CBatActor)
PROTO_LuaPush(ExtractedBuffData)
PROTO_LuaPush(StartCharData)

CBatCharacter::CBatCharacter()
{
	m_pClient = NULL;
	m_partnerGroup = NULL;
	m_pBatChar = this;
	m_pMob = NULL;
	m_pOwner = NULL;
}

CBatCharacter::~CBatCharacter()
{
	DeleteNetGroup(m_partnerGroup);
}

CBatMob* CBatCharacter::GetCurPartner()
{
	return (m_pClient ? m_pClient->pPartner : NULL);
}

void CBatCharacter::DoEscape()
{
	Parent::DoEscape();

	// if success escape, mark escape time
	if (GetState() == CHAR_ST_ESCAPED)
		m_escapeWait = m_pBattle->m_animTime;
}

void CBatCharacter::DoHelp()
{
	TRACE_ENSURE(m_pBattle);

	// no action when need help
	m_command = BATCMD_IDLE;

	if (m_pOwner == NULL || !m_pOwner->IsLeader() || 
		m_pOwner->GetAction() == CCharacter::ST_NEEDHELP || m_pBattle->m_teamData[GetTeam()].playerCount >= 5)
	{
		SetAction(BATCMD_IDLE);
		return;
	}

	m_pOwner->SetAction(CCharacter::ST_NEEDHELP, ::timeGetTime());
	m_pBattle->m_teamData[GetTeam()].needHelp = true;

	for (CBattle::BatClients::iterator cit = m_pBattle->m_clients.begin(); 
		cit != m_pBattle->m_clients.end(); ++ cit)
	{
		BatClient &client = cit->second;
		if (client.team == GetTeam() && client.IsMonitor() && client.IsConnect() )
		{
			Boolean bCanNeedHelp = BOOLEAN_TRUE;
			if (GetOwner() != NULL) 
			{
				if (GetOwner()->GetParty()!= NULL || GetOwner()->GetTempParty() != NULL) 
				{
					/*if (GetOwner()->GetParty()->GetCount() >= PARTY_MAX_MEMBER && !GetOwner()->GetParty()->IsMember(client.pOwner))*/
						bCanNeedHelp =BOOLEAN_FALSE;
				}
				/*else if (GetOwner()->GetTempParty() != NULL) 
				{
					bCanNeedHelp = BOOLEAN_FALSE;
				}*/
			}
			if (bCanNeedHelp) 
				m_pBattle->CB_NeedHelp(client.pOwner->GetPeer(), client.pOwner->GetContext(),
					BOOLEAN_TRUE);
		}
	}
}

void CBatCharacter::DoCatch()
{
	if (!IsValidTarget())
	{
		SetAction(BATCMD_IDLE);
		return;
	}

	if (m_cmdTarget != GetTarget())
		SetTarget(m_cmdTarget);


	CBatActor *pTarget = m_pBattle->GetActor(m_cmdTarget);


	if (pTarget == NULL || !pTarget->IsValidTarget())
	{
		SendBattleMsg(BATTLE_ERROR_MSG_CATCH_DIE, _T(""), GetSubType());
		SetAction(BATCMD_IDLE);
		return;
	}

	if (pTarget->GetTeam() == GetTeam())
	{
		SendBattleMsg(BATTLE_ERROR_MSG_CATCH_SAME_TEAM, _T(""), GetSubType());
		SetAction(BATCMD_IDLE);
		return;
	}

	if (pTarget->m_pMob)
	{
		pTarget->SetTarget(GetIndex());

		// already have owner
		if (pTarget->m_pMob->GetOwner_id() != 0)
		{
			SetAction(BATCMD_IDLE);
			return;
		}
		
	
		UInt16 catchSP = pTarget->m_pMob->GetLevel() * 2 + 5;
		
		if (GetSP() < catchSP)
		{
			SendBattleMsg(BATTLE_ERROR_MSG_CATCH_NO_SP, _T(""), GetSubType());
			SetAction(BATCMD_IDLE);
			return;
		}
		else
		{			
			SetSP(GetSP() - catchSP, - catchSP, 0);
		}

		if (pTarget->m_pMob->m_data->req_lv > GetLevel())
		{
			SendBattleMsg(BATTLE_ERROR_MSG_CATCH_LEVEL, _T(""), GetSubType());
			SetAction(BATCMD_IDLE);
			return;
		}

		if (m_partnerGroup && m_partnerGroup->GetCount() >= GetMaxPartner())
		{
			SendBattleMsg(BATTLE_ERROR_MSG_CATCH_FULL_PARTNER, _T(""), GetSubType());
			SetAction(BATCMD_IDLE);
			return;
		}
		
		if (pTarget->m_pMob->GetPartner_type() != CANNOT_BE_PARTNER && GetMaxPartner() != 0  )
		{
			
			gGlobal.g_TempResult = 0;
			CallLuaFunc(gGlobal.g_pLuaState, "CanCatch",  String(), this, pTarget);
			if (gGlobal.g_TempResult > 0.5)
			{
				pTarget->SetReaction(BATCMD_CATCHED);
				pTarget->SetState(CHAR_ST_CATCHED);
				pTarget->SetLinkNext(BOOLEAN_FALSE);
				pTarget->SetLeftBattle(BOOLEAN_TRUE);
				m_command = BATCMD_IDLE;

				gGlobal.g_TempResult = 0;
				if(HasLuaFunc(gGlobal.g_pLuaState, "CatchMobIdToPetId"))
					CallLuaFunc(gGlobal.g_pLuaState, "CatchMobIdToPetId",  String(), this, pTarget->m_pMob->GetMob_id());
				if(gGlobal.g_TempResult > 0)
				{
					m_pOwner->m_pPetGroup->AddPet(gGlobal.g_TempResult);

				}
				else
				{
					if (pTarget->m_pMob->GetPartner_type() == LEVEL_NOT_CHANGE_FOLLOW_OWNER)
						pTarget->m_pMob->SetLevel(GetLevel() + 5);
					else if (pTarget->m_pMob->GetPartner_type() == LEVEL_FOLLOW_OWNER)
						pTarget->m_pMob->SetLevel(GetLevel());
					else if (pTarget->m_pMob->GetPartner_type() != LEVEL_FOLLOW_MOB_PARTNER)
						pTarget->m_pMob->SetLevel(0);
					
					pTarget->m_pMob->SetPartner(0);
					pTarget->m_pMob->SetOwner(GetChar_id());
					if (m_partnerGroup == NULL)
						m_partnerGroup = CreateNetGroup();
					m_partnerGroup->Add(pTarget->m_pMob);
				}
			}
			else
			{
				pTarget->SetReaction(BATCMD_CATCHED_FAIL);
				pTarget->SetLinkNext(BOOLEAN_FALSE);
			}
		}
		else
		{
			SetAction(BATCMD_IDLE);
			return;
		}
		
		SetAction(BATCMD_CATCH);

		MarkUpdate(targetMask);
		SetLinkNext(BOOLEAN_FALSE);

		// calculate catch animation time
		m_pBattle->m_animTime += gGlobal.m_animTimer->GetActionTime(this, NULL, NULL, GetAction());
		//m_pBattle->m_animTime += gGlobal.m_animTimer->GetActionTime(this, NULL,
		//	(pTarget->GetReaction() == BATCMD_CATCHED_FAIL ? 
		//		BATCMD_CATCHED_FAIL : BATCMD_CATCH));

		WriteAction();
		pTarget->WriteReaction();
		m_bAction = BOOLEAN_TRUE;
	}
	
}

void CBatCharacter::DoFriend()
{

	if (!IsValidTarget() || m_partnerGroup == NULL)
		return;

	UInt8 teamExist[2] = {0, 0};
	
	for (Actors::iterator it = m_pBattle->m_actors.begin(); it != m_pBattle->m_actors.end(); ++it)
	{
		CBatActor *actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL)
			continue;
		UInt8 myTeam = actor->GetTeam();
		if (actor->IsValidTarget())
			teamExist[myTeam]++;
	}

	// if the battle is win, no need to change partner
	if (teamExist[ENEMY_TEAM] == 0 && teamExist[PLAYER_TEAM] > 0)	// player team win
		return;


	int index = -1;

	CBatMob* pmob = NULL;
	CBatMob* pmob2;
	for (UInt16 i = 0; i < m_partnerGroup->GetCount(); i++) {
		pmob2 = m_partnerGroup->GetChild<CBatMob>(i);
		if (!pmob2->GetLeftBattle() && pmob2->GetPartner_id() == m_nextPartnerID) {
			pmob = pmob2;
			break;
		}
	}

	if (pmob == NULL)
		return;

	if (pmob->GetLevel() > (GetLevel() + 5) || ( pmob->m_data && pmob->m_data->req_lv > GetLevel()))
	{
		SendBattleMsg(BATTLE_ERROR_MSG_LEVEL_TOO_HIGH, _T(""), pmob->GetPartner_id());
		return;
	}
		

	if (pmob->GetHealth() > 50)
	{
		
		gGlobal.g_TempResult = 0;
		CallLuaFunc(gGlobal.g_pLuaState, "PartnerJoinBattle", String(), pmob->GetFavor());

		if (gGlobal.g_TempResult > 0.5 )
		{
			if (m_pClient->pPartner && m_pClient->pPartner != pmob)
			{
				m_pClient->pPartner->DoPartnerLeft();
				m_pClient->pPartner->SetReaction(BATCMD_INVISIBLE);
				m_pClient->pPartner->WriteReaction();
			}
				
			m_pClient->pPartner = pmob;
			pmob->m_pClient = m_pClient;
			
			CParty* pParty = m_pOwner->GetParty();
			BOOL bResetPos = FALSE;
			if (NULL != pParty) {
				if (pParty->GetM_uLineupId() != 13) {
					UInt8 uIndex = pParty->GetCharactorIndexById(m_pOwner->GetChar_id());
					if (uIndex < TEAMMATE_MAX) {
						pmob->SetPosition(10+uIndex);
						bResetPos=TRUE;
					}
				}
			}

			if (!bResetPos) {
				pmob->SetPosition(GetPosition());
			}

			m_pBattle->InsertActor(pmob);
			m_nextPartnerID = 0;
			m_pClient->pPartner->SetNew(true);
			if (pmob->m_pPartner)
				pmob->m_pPartner->ChangeState(CPartner::ST_FIGHT);

			pmob->PackBuff();

			m_pBattle->m_friendGroup.Add(pmob);		// added to coming friend group
			SetReaction(BATCMD_SHOW_FRIEND);
			WriteReaction();
			m_bAction = BOOLEAN_TRUE;
		}
		else
		{
			SendBattleMsg(BATTLE_ERROR_MSG_NOT_FAVOR, _T(""), pmob->GetPartner_id());
		}
	}
	else
	{
		SendBattleMsg(BATTLE_ERROR_MSG_NOT_HEALTH, _T(""), pmob->GetPartner_id());
	}

	// reset action to default idle
	m_command = BATCMD_IDLE;
}

void CBatCharacter::DoEvent()
{
}

void CBatCharacter::TurnClear()
{
	CBatActor::TurnClear();

	if (m_pBag && m_command == BATCMD_ITEM)
		m_pBag->SyncTryCount();
}

void CBatCharacter::CalcAttrib()
{
	if (m_pBag == NULL)
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("BatCharacter CalcAttrib Warning: null bag"));
		return;
	}

	UInt16 orgSpd = GetSPD();
	// base STA, SPR, STR, CON, AGI
	Int16 sta = GetSTA();
	Int16 spr = GetSPR();
	Int16 str = GetSTR();
	Int16 con = GetCON();
	Int16 agi = GetAGI();
	
	// first increase STA, SPR, STR, CON, AGI by equipment
	for (UInt8 i = BAG_MAX_ITEM; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; i++)
	{
		CItem *item = m_pBag->GetItem(i);
		if (item && item->GetEndure() > 0) {
			sta += item->GetSTA();
			spr += item->GetSPR();
			str += item->GetSTR();
			con += item->GetCON();
			agi += item->GetAGI();
		}
	}

	// increase ... by itemset
	m_pBag->CalcItemSetBasicAttrib(sta, spr, str, con, agi);

	if (sta < 0)
		sta = 0;
	if (spr < 0)
		spr = 0;
	if (str < 0)
		str = 0;
	if (con < 0)
		con = 0;
	if (agi < 0)
		agi = 0;

	// attribute formula table index
		const StartCharData* pChar = gGlobal.m_resource->GetStartCharData(GetSexClass());

	StartCharData temp;
	temp = *pChar; 
	CallLuaFunc(gGlobal.g_pLuaState, "CalBatCharAttr", String(), (CBatActor*)this, &temp, 
		sta, spr, str, con, agi);

	
	float atk = GetATK();
	float def = GetDEF();
	float hit = GetHIT();
	float eva = GetEVA();
	float pow = GetPOW();
	float spd = GetSPD();
	UInt16 eq_maxHp = 0;
	UInt16 eq_maxSp = 0;

	// second increase ATK, DEF, HIT, EVA, POW, SPD by equipment
	for (UInt8 i = BAG_MAX_ITEM; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; i++)
	{
		CItem *item = m_pBag->GetItem(i);
		if (item) {
			atk += item->GetATK();
			def += item->GetDEF();
			hit += item->GetHIT();
			eva += item->GetEVA();
			pow += item->GetPOW();
			spd += item->GetSPD();
			eq_maxHp += item->GetMaxHP();
			eq_maxSp += item->GetMaxSP();
		}
	}

	// increase ... by itemset
	m_pBag->CalcItemSetBatAttrib(atk, def, hit, eva, pow, spd, eq_maxHp, eq_maxSp);

	if (atk < 0)
		atk = 0;
	if (def < 0)
		def = 0;
	if (hit < 0)
		hit = 0;
	if (eva < 0)
		eva = 0;
	if (pow < 0)
		pow = 0;
	if (spd < 0)
		spd = 0;
	if (eq_maxHp < 0)
		eq_maxHp = 0;
	if (eq_maxSp < 0)
		eq_maxSp = 0;

	// calc Max HP,SP by attribute
	
	float maxHp = (float)GetMaxHP() + eq_maxHp;
	float maxSp = (float)GetMaxSP() + eq_maxSp;

	// increase ... by itemset
	m_pBag->CalcItemSetBatAttribMaxHpSp(maxHp, maxSp);

	UInt16 maxDp = GetMaxDP();
	UInt16 maxWc = GetMaxWC();

	AttriStruct TempAttri;
	TempAttri.uIntAtk = (int)atk;
	TempAttri.uIntDef = (int)def;
	TempAttri.uIntHit = (int)hit;
	TempAttri.uIntEva = (int)eva;
	TempAttri.uIntPow = (int)pow;
	TempAttri.uIntSpd = (int)spd;
	TempAttri.uIntMaxHp = (int)maxHp;
	TempAttri.uIntMaxSp = (int)maxSp;
	TempAttri.uIntMaxDp = maxDp;
	TempAttri.uIntMaxWc = maxWc;
	//if (m_pSkillMap) 
	{
		for (SkillMap::iterator Itr = m_SkillMap.begin(); Itr != m_SkillMap.end(); ++Itr)
		{
			const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(Itr->second->GetSkill_id());
			if (pSkillData)
			{
				{
					StringA LuaFunc;
					LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second->GetSkill_id(), "skill").c_str(), CommonSkillFuncName[SKILL_FUNC_NAME_ATTRI].c_str());
					if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), 
							(CBatActor *)this, Itr->second->GetSkill_level(), &TempAttri);
				}

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
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), (CBatActor *)this, &Itr->second, 
							&TempAttri);
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
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), (CBatActor *)this, &Itr->second, &TempAttri);
				}
			}
		}
	}

	SetAttrib2((UInt16) TempAttri.uIntAtk, (UINT16) TempAttri.uIntDef , (UINT16) TempAttri.uIntHit , 
		(UINT16) TempAttri.uIntEva , (UINT16) TempAttri.uIntPow , (UINT16) TempAttri.uIntSpd);
	
	SetMaxHP((UInt32) TempAttri.uIntMaxHp );
	if (GetHP() > GetMaxHP())
		SetHP(GetMaxHP());
	if (GetMHP() > GetMaxHP())
		SetMHP(GetMaxHP());

	SetMaxSP((UInt32) TempAttri.uIntMaxSp );
	if (GetSP() > GetMaxSP())
		SetSP(GetMaxSP());
	if (GetMSP() > GetMaxSP())
		SetMSP(GetMaxSP());

	SetMaxDP((UInt16) TempAttri.uIntMaxDp );
	SetMaxWC((UInt16) TempAttri.uIntMaxWc );

	if (orgSpd != GetSPD())
		CalSpeed();
}

void CBatCharacter::SetData(CCharacter* pChar)
{
	SetInfo(pChar->GetAcct_id(), pChar->GetChar_id(),
			pChar->GetNickName(), pChar->GetSex());
	SetInfo2(pChar->GetCclass(), pChar->GetSexClass(), pChar->GetFaction());
	if (pChar->GetModel_mobid() > 0)
	{
		const MobData *pData = NULL;
		pData = gGlobal.m_resource->GetMobData(pChar->GetModel_mobid());
		if (pData)
		{
			UINT32 hpAptitude = pData->HP_native_d;
			UINT32 spAptitude = pData->SP_native_d;
			UINT32 atkAptitude = pData->ATK_native_d;
			UINT32 defAptitude = pData->DEF_native_d;
			UINT32 spdAptitude = pData->SPD_native_d;
			UINT32 powAptitude = pData->POW_native_d;
			UINT32 hitAptitude = pData->HIT_native_d;
			UINT32 evaAptitude = pData->EVA_native_d;
			float Grow = pData->grow_d;
			UINT32 level = pData->lv_d;
			UINT32 staAptitude = pData->STA_base_d;
			UINT32 sprAptitude = pData->SPR_base_d;
			UINT32 strAptitude = pData->STR_base_d;
			UINT32 conAptitude = pData->CON_base_d;
			UINT32 agiAptitude = pData->AGI_base_d;

			UINT32 ATK = 1.16*(0.75*Grow*strAptitude+2*level*Grow+(atkAptitude+400)*level/400+30);
			UINT32 DEF = 0.9*(1.5*Grow*conAptitude+2*level*Grow+defAptitude*level/350);
			UINT32 SPD = agiAptitude*spdAptitude/1440;
			UINT32 POW = 0.95*((0.7*Grow*(0.95*sprAptitude+0.5*strAptitude+0.4*staAptitude+0.3*conAptitude)+1.75*level*Grow)+powAptitude*level/900);
			UINT32 HIT = 2*Grow*agiAptitude+0.5*Grow*strAptitude+hitAptitude*level/350;
			UINT32 EVA = Grow*agiAptitude+1.25*level*Grow+evaAptitude*level/1000;
			UINT32 HP = 1.3*(5.5*Grow*staAptitude+hpAptitude*level/750);
			UINT32 SP = 1.3*(3*Grow*sprAptitude+spAptitude*level/1500);

			SetLevel(level);
			SetHP(HP, 0, 0);
			SetMHP(HP, 0, 0);
			SetMaxHP(HP);
			SetSP(SP, 0, 0);
			SetMSP(SP, 0, 0);
			SetMaxSP(SP);
			SetAttrib1(staAptitude, sprAptitude, strAptitude, conAptitude, 
				agiAptitude);
			SetAttrib2(ATK,DEF,HIT,EVA,POW,SPD);
			TRACE_INFODTL_8(GLOBAL_LOGGER, _F("CBatCharacter SetData staAptitude = %d, sprAptitude = %d, strAptitude = %d ,conAptitude = %d,agiAptitude = %d,m Grow = %f,level = %d,hpAptitude = %d"),
				staAptitude,sprAptitude,strAptitude,conAptitude,agiAptitude,Grow,level,hpAptitude);
		}
	}
	else
	{
		SetLevel(pChar->GetLevel());
		SetHP(pChar->GetHP(), 0, 0);
		SetMHP(pChar->GetMHP(), 0, 0);
		SetMaxHP(pChar->GetMaxHP());
		SetSP(pChar->GetSP(), 0, 0);
		SetMSP(pChar->GetMSP(), 0, 0);
		SetMaxSP(pChar->GetMaxSP());

		SetAttrib1(pChar->GetSTA(), pChar->GetSPR(), pChar->GetSTR(), pChar->GetCON(), 
			pChar->GetAGI());
		SetAttrib2(pChar->GetATK(), pChar->GetDEF(), pChar->GetHIT(), pChar->GetEVA(), 
			pChar->GetPOW(), pChar->GetSPD());
	}
	
	
	SetDP(pChar->GetDP(), 0, pChar->GetMaxDP());
	SetMDP(pChar->GetMDP());
	SetMaxDP(pChar->GetMaxDP());
	SetWC(pChar->GetWC(), 0, 0);
	SetWcExp(pChar->GetWcExp());
	SetMaxWC(pChar->GetMaxWC());
	SetVP(pChar->GetVP(), 0, 0);
	SetMaxVP(pChar->GetMaxVP());
	SetState(pChar->GetState());
	SetBuff(pChar->GetBuff());
	SetBuffCache(pChar->GetBuffCache());

	UInt16 hairStyle = pChar->GetOrg_hairStyle1();
	UInt16 hairColor = pChar->GetOrg_hairColor();
	if ( pChar->GetHairStyle1() > 0)
	{
		hairStyle = pChar->GetHairStyle1();
		hairColor = pChar->GetHairColor();
	}
	SetStyle(pChar->GetPhoto_id(), hairStyle, pChar->GetHairStyle2(), 
		hairColor);
	SetEqWeapon(pChar->GetEq_weapon(), pChar->GetEq_weaponColor());
	SetEqHead(pChar->GetEq_headBand(), pChar->GetEq_headColor());

	UInt32 eqCloth = pChar->GetOrg_eq_cloth();
	UInt16 eqClothColor = pChar->GetOrg_eq_clothColor();

	if (pChar->GetEq_cloth() > 0)
	{
		eqCloth = pChar->GetEq_cloth();
		eqClothColor = pChar->GetEq_clothColor();
	}

	SetEqCloth(eqCloth, eqClothColor);
	SetPKInfo(pChar->GetPK_Value());
	SetMaxPartner(pChar->GetMaxPartner());
	SetGoodness(pChar->GetGoodness());
	SetPet(pChar->GetPet_uid(), pChar->GetPet_id(), pChar->GetPetName(), pChar->GetPetColorIndex());

	SetTP(pChar->GetTP(),pChar->GetModel_mobid());
	SetState(CHAR_ST_NORM);
	SetTarget(0);
	SetSubType(0);
	SetLinkNext(BOOLEAN_FALSE);

	ExtractBuffString();
	ExtractBuffCacheString();

	CalSpeed();

	ClearAllMask();
}

void CBatCharacter::WriteData(CCharacter* pChar)
{
	// BatCharacter goodness shouldn't changed
	//if (GetGoodness() != pChar->GetGoodness())
	//	pChar->SetGoodness(GetGoodness());
	if (GetHP() != pChar->GetHP() || GetMHP() != pChar->GetMHP() || 
		GetMaxHP() != pChar->GetMaxHP())
		pChar->SetHP(GetHP(), GetMHP(), GetMaxHP());
	if (GetSP() != pChar->GetSP() || GetMSP() != pChar->GetMSP() || 
		GetMaxSP() != pChar->GetMaxSP())
		pChar->SetSP(GetSP(), GetMSP(), GetMaxSP());
		
	if (GetWC() != pChar->GetWC() || GetMaxWC() != pChar->GetMaxWC() || 
		GetWcExp() != pChar->GetWcExp())
	{
		pChar->SetWC(GetWC());
		pChar->SetMaxWC(GetMaxWC());
		pChar->SetWcExp(GetWcExp());
		while (pChar->GetWcExp() >= pChar->GetNextWCExp())
		{
			pChar->SetWcExp(m_pOwner->GetWcExp() - m_pOwner->GetNextWCExp());
			pChar->WCLevelUp();
		}		
	}
	//if (GetVP() != pChar->GetVP() || GetMaxVP() != pChar->GetMaxVP())
	//	pChar->SetVP(GetVP(), GetMaxVP());
	pChar->SetVP(MIN(GetVP(), pChar->GetVP()), MIN(GetMaxVP(), pChar->GetMaxVP()));
	//check buff value update
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin();
		Itr != m_BuffCacheMap.end(); ++ Itr)
	{
		BuffMap::iterator Itr2 = pChar->m_BuffCacheMap.find(Itr->second.pBuffData->Buff_school);
		if (Itr2 != pChar->m_BuffCacheMap.end() &&
			Itr2->second.GetBuffID() == Itr->second.GetBuffID())
		{
			Itr2->second.SetBuffValue1(Itr->second.GetBuffValue1());
			Itr2->second.SetBuffValue2(Itr->second.GetBuffValue2());
			Itr2->second.SetBuffValue3(Itr->second.GetBuffValue3());
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_TURN)
			{
				Itr2->second.CoolDown = Itr->second.CoolDown;
			}
		}
	}

	for (BuffMap::iterator Itr = m_BuffMap.begin();
		Itr != m_BuffMap.end(); ++ Itr)
	{
		BuffMap::iterator Itr2 = pChar->m_BuffMap.find(Itr->second.pBuffData->Buff_school);
		if (Itr2 != pChar->m_BuffMap.end() &&
			Itr2->second.GetBuffID() == Itr->second.GetBuffID())
		{
			Itr2->second.SetBuffValue1(Itr->second.GetBuffValue1());
			Itr2->second.SetBuffValue2(Itr->second.GetBuffValue2());
			Itr2->second.SetBuffValue3(Itr->second.GetBuffValue3());
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_TURN)
			{
				Itr2->second.CoolDown = Itr->second.CoolDown;
			}
		}
	}

	pChar->CheckBuffUpdate();
	pChar->ConstructBuffString();
	pChar->ConstructBuffCacheString();
	//
	if (GetPK_Value() != pChar->GetPK_Value())
		pChar->SetPKInfo(GetPK_Value());
}

void CBatCharacter::PreSend()
{
	Parent::PreSend();

	m_sendMask.MaskBits(infoMask, info2Mask, eqWeaponMask, eqHeadMask, eqClothMask, PKInfoMask, styleMask,
		DPMask, maxDPMask, WCMask, maxWCMask, WcExpMask, VPMask, maxVPMask,TPMask, -1);
}

void CBatCharacter::PreSendEnemy()
{
	Parent::PreSendEnemy();

	m_sendMask.MaskBits(infoMask, info2Mask, eqWeaponMask, eqHeadMask, eqClothMask, PKInfoMask, styleMask,
		DPMask, maxDPMask, WCMask, maxWCMask, VPMask, maxVPMask,TPMask, -1);
}

void CBatCharacter::WriteAction()
{
	//check dead status before write buffer
	CheckDeadStatus();
	RPCMask mask;						// set mask for writing buffer

	mask.MaskBits(actionMask, targetMask, subTypeMask, linkNextMask, leftBattleMask,
		HPMask, mHPMask, SPMask, mSPMask, DPMask, maxDPMask, WCMask, maxWCMask, VPMask, maxVPMask, 
		buffMask, buffCacheMask, pet_activeMask, stateMask, hitTogetherMask,TPMask, -1);

	mask &= m_updateMask;				// only update updated attributes

	m_pBattle->m_turnBuffer1->Write<UInt8>(GetIndex());
	m_pBattle->m_turnBuffer2->Write<UInt8>(GetIndex());
	m_pBattle->m_turnBufferMon->Write<UInt8>(GetIndex());
	if (GetTeam() == ENEMY_TEAM)
	{
		WriteAttr(*m_pBattle->m_turnBuffer1, mask);
		UInt32 orgHP = GetHP();
		UInt32 orgSP = GetSP();
		if (GetHP() > 0)
			SetHP(1);
		else
			SetHP(0);
		SetSP(0);

		WriteAttr(*m_pBattle->m_turnBuffer2, mask);
		WriteAttr(*m_pBattle->m_turnBufferMon, mask);
		
		SetHP(orgHP);
		SetSP(orgSP);
	}
	else
	{
		WriteAttr(*m_pBattle->m_turnBuffer2, mask);
		UInt32 orgHP = GetHP();
		UInt32 orgSP = GetSP();
		if (GetHP() > 0)
			SetHP(1);
		else
			SetHP(0);
		SetSP(0);

		WriteAttr(*m_pBattle->m_turnBuffer1, mask);
		WriteAttr(*m_pBattle->m_turnBufferMon, mask);
		
		SetHP(orgHP);
		SetSP(orgSP);
	}

	UnmarkUpdate(mask);			// clear update mask after writing buffer
	ResetDropValue();
}

void CBatCharacter::WriteReaction()
{
	//check dead status before write buffer
	CheckDeadStatus();
	m_pProtector = NULL;
	RPCMask mask;						// set mask for writing buffer

	mask.MaskBits(HPMask, mHPMask, SPMask, mSPMask, stateMask, DPMask, maxDPMask, WCMask, maxWCMask, VPMask, maxVPMask,
		reactionMask, targetMask, subTypeMask, linkNextMask, leftBattleMask, buffMask, buffCacheMask, pet_activeMask, hitTogetherMask, -1);
	mask &= m_updateMask;				// only update updated attributes
	m_pBattle->m_turnBuffer1->Write<UInt8>(GetIndex());
	m_pBattle->m_turnBuffer2->Write<UInt8>(GetIndex());
	m_pBattle->m_turnBufferMon->Write<UInt8>(GetIndex());
	if (GetTeam() == ENEMY_TEAM)
	{
		WriteAttr(*m_pBattle->m_turnBuffer1, mask);
		UInt32 orgHP = GetHP();
		UInt32 orgSP = GetSP();
		UInt32 orgMHP = GetMHP();
		UInt32 orgMSP = GetMSP();
		if (GetHP() > 0)
			SetHP(1);
		else
			SetHP(0);
		SetSP(0);
		SetMHP(0);
		SetMSP(0);

		WriteAttr(*m_pBattle->m_turnBuffer2, mask);
		WriteAttr(*m_pBattle->m_turnBufferMon, mask);
		
		SetHP(orgHP);
		SetSP(orgSP);
		SetMHP(orgMHP);
		SetMSP(orgMSP);
	}
	else
	{
		WriteAttr(*m_pBattle->m_turnBuffer2, mask);
		UInt32 orgHP = GetHP();
		UInt32 orgSP = GetSP();
		UInt32 orgMHP = GetMHP();
		UInt32 orgMSP = GetMSP();
		if (GetHP() > 0)
			SetHP(1);
		else
			SetHP(0);
		SetSP(0);
		SetMHP(0);
		SetMSP(0);

		WriteAttr(*m_pBattle->m_turnBuffer1, mask);
		WriteAttr(*m_pBattle->m_turnBufferMon, mask);
		
		SetHP(orgHP);
		SetSP(orgSP);
		SetMHP(orgMHP);
		SetMSP(orgMSP);
	}
	//check miss but get hurt 
	if (IsUpdated(HPMask) && IsUpdated(reactionMask) && GetReaction() == BATCMD_MISS && GetDShowHP() != 0)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CBatActor WriteReaction Error: miss but show hurt: DHP: %d DShowHP: %d"),
			GetDHP(), GetDShowHP());
	}

	UnmarkUpdate(mask);					// clear update mask after writing buffer
	ResetDropValue();
	
}

void CBatCharacter::SetAuto(BatClient* pClient)
{
	if (m_pBattle && m_pBattle->CanInput()) {
		m_pClient = pClient;
		RaiseAutoEvent();
	}
}

void CBatCharacter::RaiseAutoEvent()
{
	RaiseUniqEventID(EVT_AUTO_TIMEOUT, AUTO_TIMEOUT + TIMEOUT_DELAY);
}

void CBatCharacter::StopAutoEvent()
{
	CancelUniqEvent(EVT_AUTO_TIMEOUT);
}

void CBatCharacter::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_AUTO_TIMEOUT:		// auto command timeout
		if (m_pBattle && m_pClient && m_pClient->autoCount > 0 && 
			m_pClient->pBatChar == this && !GetLeftBattle())
		{
			if (m_pOwner)
				TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("Client Auto Command sessionId %s, char_id %d"),
					m_pOwner->GetContext().sessionId.GetString().c_str(), GetChar_id());

			if (!m_pClient->pBatChar->m_bInput)
			{
				AutoInput();
				m_pBattle->NotifyInput(m_pClient->pBatChar);		// notify inputed
			}

			if (m_pClient->pPartner && !m_pClient->pPartner->m_bInput) {
				m_pClient->pPartner->AutoInput();
				m_pBattle->NotifyInput(m_pClient->pPartner);		// notify inputed
			}
			//if (m_pClient->autoCount != 255) {			// 255 = forever auto
			if (m_pClient->autoCount != 65535) {			// 65535 = forever auto
				m_pClient->autoCount--;
				if (m_pClient->autoCount == 0 && m_pOwner){
					CB_AutoCount(m_pOwner->GetPeer(), m_pOwner->GetContext(), 0);
					m_pClient->bAuto = BOOLEAN_FALSE;
				}
			}
		}
		break;
	}
}

void CBatCharacter::ChangeDP(Int change, bool show)
{
	//just store the real change in DDP
	UInt16 FinalDP = WITHIN((Int) GetDP() + change, 0, GetMaxDP());
	m_RDDP += FinalDP - GetDP();
	SetDP(FinalDP, GetDDP() + change, (show ? GetDShowDP() + change : 0));

	if (GetDP() == 0)
		ClearWineBuff();
}

void CBatCharacter::ChangeTP(Int change, bool show)
{
	if ((Int)GetTP() + change <= 0)
		SetTP(0);
	else
		SetTP(GetTP() + change);
}

void CBatCharacter::ChangeWC(Int change, bool show)
{
	//SetWC(WITHIN((Int) GetWC() + change, 0, GetMaxWC()), change, (show ? change : 0));
	if ((Int)GetWC() + change <= 0)
		SetWC(0);
	else
		SetWC(GetWC() + change);

}
void CBatCharacter::ChangeWcExp(Int change)
{
	//if (m_pOwner)
	{
		//m_pOwner->SetWcExp(WITHIN((Int) GetWcExp() + change, 0));
		if ((Int)GetWcExp() + change <= 0)
			SetWcExp(0);
		else
			SetWcExp(GetWcExp() + change);
	}
}
void CBatCharacter::ChangeVP(Int change, bool show)
{
	//just store the real change in DVP
	UInt16 FinalVP = WITHIN((Int) GetVP() + change, 0, GetMaxVP());
	m_RDVP = FinalVP - GetVP();
	SetVP(FinalVP, change, (show ? change : 0));
}

bool CBatCharacter::AllCmdBlock()
{
	return (m_cmdBlockMap[MAX_ACTION_CMD] > 0);
}

void CBatCharacter::ResetDpChange()
{
	
	SetDP(GetDP() - m_RDDP, 0, 0);
	m_RDDP = 0;
}

void CBatCharacter::ResetVPChange()
{
	SetVP(GetVP() - m_RDVP, 0, 0);
	m_RDVP = 0;
}

bool CBatCharacter::IsLeader()
{
	return (m_pOwner ? m_pOwner->IsLeader() : BOOLEAN_FALSE);
}

bool CBatCharacter::HasParty()
{
	return (m_pOwner ? m_pOwner->HasParty() : BOOLEAN_FALSE);
}

void CBatCharacter::ClearAllPartnerItemBag()
{
	if (m_partnerGroup == NULL)
		return;

	CBatMob* pmob;
	for (UInt16 i = 0; i < m_partnerGroup->GetCount(); ++i) 
	{
		pmob = m_partnerGroup->GetChild<CBatMob>(i);
		if (pmob)
			pmob->m_pBag = NULL;
	}
}

void CBatCharacter::InitPrevCmd()
{
	if (m_pOwner == NULL)
		return;

	m_command = m_pOwner->m_cmd;
	if (m_command == BATCMD_IDLE)			// no need to restore following if idle
		return;

	m_cmdTarget = 0;
	m_cmdTargetType = m_pOwner->m_cmdTargetType;
	m_cmdSubType = m_pOwner->m_sub_type;
	m_cmdAdditional = m_pOwner->m_additional;
	if (m_cmdTargetType == TARGET_ENEMY)
	{
		NextTarget(BOOLEAN_TRUE);
	}
	else if (m_cmdTargetType == TARGET_CHARACTER || 
		m_cmdTargetType == TARGET_PARTNER)
	{
		// get actor by target type and target id
		CBatActor* pTarget = m_pBattle->GetActor(m_cmdTargetType, m_pOwner->m_cmdTarget);
		if (pTarget)
			m_cmdTarget = pTarget->GetIndex();		// set target index if found
		else
			m_command = BATCMD_IDLE;				// otherwise, idle command
	}
}

void CBatCharacter::SaveLastCmd()
{
	if (m_pOwner == NULL)
		return;

	ResetAction();
	m_pOwner->m_cmd = m_command;
	if (m_command == BATCMD_IDLE)			// no need to store following if idle
		return;

	m_pOwner->m_cmdTarget = m_cmdTarget;
	m_pOwner->m_cmdTargetType = m_cmdTargetType;
	m_pOwner->m_sub_type = m_cmdSubType;
	m_pOwner->m_additional = m_cmdAdditional;
	// store target id instead of index
	CBatActor* pTarget = m_pBattle->GetActor(m_cmdTarget);
	if (pTarget) {
		if (pTarget->GetTeam() != GetTeam()) {
			m_pOwner->m_cmdTargetType = CBatActor::TARGET_ENEMY;
			m_pOwner->m_cmdTarget = 0;
		} else if (pTarget->IsPlayer()) {
			m_pOwner->m_cmdTargetType = CBatActor::TARGET_CHARACTER;
			m_pOwner->m_cmdTarget = pTarget->m_pBatChar->GetChar_id();
		} else if (pTarget->IsParnter()) {
			m_pOwner->m_cmdTargetType = CBatActor::TARGET_PARTNER;
			m_pOwner->m_cmdTarget = pTarget->m_pMob->GetPartner_id();
		} else {
			m_pOwner->m_cmd = BATCMD_IDLE;
		}
	} else {
		m_pOwner->m_cmd = BATCMD_IDLE;
	}
}

Boolean CBatCharacter::GetAttackBoss()
{
	if(m_pOwner)
		return m_pOwner->GetHitBoss();
	return true;
}