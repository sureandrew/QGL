//-- Common
#include "Common.h"
//-- Self
#include "SimClient/Partner.h"
//-- Library
#include "SimClient/Character.h"
#include "SimClient/Skill.h"
#include "SimClient/SkillGroup.h"
#include "SimClient/SimClient.h"
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CPartner, Partner_CLASSID);

CPartner::~CPartner()
{
	// safe delete check
	if (m_pSkillGroup && m_pSkillGroup->GetOwner_id() == GetPartner_id())
		DeleteNetGroup(m_pSkillGroup, BOOLEAN_TRUE);

	m_STA = 0;
	m_STR = 0;
	m_CON = 0;
	m_SPR = 0;
	m_AGI = 0;
}

void CPartner::SetSkillGroup(CSkillGroup *grp)
{
	if (grp == NULL)
		return;

//	if (m_pSkillGroup != grp)
//		DeleteNetGroup(m_pSkillGroup);
	m_pSkillGroup = grp;
}

Boolean CPartner::CanCall()
{
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_pCharacter)
		return (GetLevel() < gGlobal.m_pSimClient->m_pCharacter->GetLevel() + 10);
	else
		return BOOLEAN_FALSE;
}

UInt16 CPartner::GetSkillCount()
{
	if (!FindNetObject(GetNetId())) {
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("Partner Warning: [partner already deleted], partner_id %d, netId %d,%d, objId %d"), 
			GetPartner_id(), GetNetId().srvId, GetNetId().objId, GetID());
		return 0;
	}

	if (m_pSkillGroup && !FindNetObject(m_pSkillGroup->GetNetId())) {
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("Partner Warning: [partner skill group already deleted], partner_id %d, netId %d,%d, objId %d"), 
			GetPartner_id(), m_pSkillGroup->GetNetId().srvId, m_pSkillGroup->GetNetId().objId,
			m_pSkillGroup->GetID());
		m_pSkillGroup = NULL;
		return 0;
	}

	return (m_pSkillGroup ? m_pSkillGroup->GetCount() : 0);
}

CSkill* CPartner::GetSkill(UInt16 i)
{
	if (m_pSkillGroup && i < m_pSkillGroup->GetCount()) {
		return m_pSkillGroup->GetChild<CSkill>(i);
	} else {
		return NULL;
	}
}

CSkill* CPartner::GetSkillById(UInt16 skill_id)
{
	if (m_pSkillGroup == NULL)
		return NULL;

	CSkill *pSkill;
	for (UInt16 i = 0; i < m_pSkillGroup->GetCount(); i++) {
		pSkill =  m_pSkillGroup->GetChild<CSkill>(i);
		if (pSkill && pSkill->GetSkill_id() == skill_id)
			return pSkill;
	}
	return NULL;
}

void CPartner::ShowDetail()
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Show Partner: partner_id %d, netId %d,%d, objId %d"), 
		GetPartner_id(), GetNetId().srvId, GetNetId().objId, GetID());
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("  mob_id %d, owner_id %d"), 
		GetMob_id(), GetOwner_id());
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("  level %d, exp %d, next level %d"), 
		GetLevel(), GetExp(), GetNextLvExp());
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("  STA %d, SPR %d, STR %d, CON %d, AGI %d"), 
		GetSTA(), GetSPR(), GetSTR(), GetCON(), GetAGI());
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("  ATK %d, DEF %d, HIT %d, EVA %d, POW %d, SPD %d"), 
		GetATK(), GetDEF(), GetHIT(), GetEVA(), GetPOW(), GetSPD());
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("  HP %d/%d, SP %d/%d"),
		GetHP(), GetMaxHP(), GetSP(), GetMaxSP());
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("  state %d, favor %.0f, health %d, buff %s"), 
		GetState(), GetFavor(), GetHealth(), GetBuff().c_str());
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("  cultivation %d, practice %d"),
		GetCultivation(), GetPractice());
	ShowSkills();
}

void CPartner::ShowSkills()
{
	if (m_pSkillGroup == NULL)
		return;

	String s;
	CSkill *pSkill;
	for (UInt16 i = 0; i < m_pSkillGroup->GetCount(); i++) {
		pSkill =  m_pSkillGroup->GetChild<CSkill>(i);
		if (pSkill == NULL) continue;
		s.AppendFormat(_T("%d (Lv%d), "), pSkill->GetSkill_id(), pSkill->GetSkill_level());
	}
	if (!s.IsEmpty())
		TRACE_INFOL_2(GLOBAL_LOGGER, _F("Partner Skill: partner_id %d, %s"), 
			GetPartner_id(), s.c_str());
}

void CPartner::OnUpdateId(const UInt32 &partner_id, const UInt32 &mob_id)
{
}

void CPartner::OnUpdateOwner(const UInt32 &owner_id)
{
}

void CPartner::OnUpdateInfo(const String &raname)
{
}

void CPartner::OnUpdateLevel(const UInt16 &level)
{
}

void CPartner::OnUpdateNextLvExp(const UInt32 &nextLvExp)
{
}

void CPartner::OnUpdateExp(const UInt32 &exp)
{
}

void CPartner::OnUpdateAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI)
{
}

void CPartner::OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD)
{
}

void CPartner::OnUpdateAttrPoint(const UInt16 &attrPoint)
{
}

void CPartner::OnUpdateHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP)
{
}

void CPartner::OnUpdateSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP)
{
}

void CPartner::OnUpdateGrow(const float &grow)
{
}

void CPartner::OnUpdateFavor(const float &favor)
{
}

void CPartner::OnUpdateHealth(const UInt16 &health)
{
}

void CPartner::OnUpdateState(const UInt8 &state)
{
}

void CPartner::OnUpdateBuff(const String &buff)
{
}

RPCResult CPartner::CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CPartner CB_RaiseAttrib:"));

	return RPC_RESULT_OK;
}

RPCResult CPartner::CB_ChangePartnerName(LPCPEER pPeer, RPCContext &context, const bool &success)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CPartner CB_ChangeName:"));

	return RPC_RESULT_OK;
}

RPCResult CPartner::CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CPartner CB_ChangePartnerState:"));

	return RPC_RESULT_OK;
}

RPCResult CPartner::CB_UpdateSuccess(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CPartner CB_UpdateSuccess:"));

	return RPC_RESULT_OK;
}

RPCResult CPartner::CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CPartner CB_LearnSkillMessage: %d"), skillID);

	return RPC_RESULT_OK;
}

RPCResult CPartner::CB_FalseJoinBattle(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CPartner CB_FalseJoinBattle: %d"), MsgType);

	return RPC_RESULT_OK;
}