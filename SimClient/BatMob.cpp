//-- Common
#include "Common.h"
//-- Self
#include "SimClient/BatMob.h"
//-- Library
#include "SimClient/PartnerGroup.h"
#include "SimClient/Partner.h"
#include "SimClient/SimClient.h"
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CBatMob, BatMob_CLASSID);

void CBatMob::OnUpdateInfo(const UInt32 &mob_id, const String &raname)
{
}

void CBatMob::OnUpdateOwner(const UInt32 &owner_id)
{
}

void CBatMob::OnUpdatePartner(const UInt32 &partner_id)
{
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_pPartnerGroup) {
		m_partner = gGlobal.m_pSimClient->m_pPartnerGroup->GetPartner(partner_id);
	}
}


UInt16 CBatMob::GetSkillCount()
{
	return (m_partner ? m_partner->GetSkillCount() : 0);
}

CSkill* CBatMob::GetSkill(UInt16 i)
{
	return (m_partner ? m_partner->GetSkill(i) : NULL);
}

CSkill* CBatMob::GetSkillById(UInt16 skill_id)
{
	return (m_partner ? m_partner->GetSkillById(skill_id) : NULL);
}

void CBatMob::ShowDetail()
{
	if (m_partner)
		m_partner->ShowDetail();
}

void CBatMob::ShowSkills()
{
	if (m_partner)
		m_partner->ShowSkills();
}