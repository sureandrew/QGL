//-- Common
#include "Common.h"
#include "Common/common_binary.h"
//-- Self
#include "SimClient/PartnerGroup.h"
//-- Library
#include "SimClient/Partner.h"
#include "SimClient/Skill.h"
#include "SimClient/SkillGroup.h"
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CPartnerGroup, PartnerGroup_CLASSID);

CPartnerGroup::CPartnerGroup()
{

}

CPartnerGroup::~CPartnerGroup()
{
	DeleteNetChilds(this);
}

void CPartnerGroup::SetSkillGroups(NetGroup* skillGroups)
{
	CSkillGroup* grp;
	CPartner* partner;

	for (UInt16 i = 0; i < skillGroups->GetCount(); i++)
	{
		grp = skillGroups->GetChild<CSkillGroup>(i);
		if (grp) {
			partner = GetPartner(grp->GetOwner_id());
			if (partner) {
				partner->SetSkillGroup(grp);
			} else {
				TRACE_WARNL_1(GLOBAL_LOGGER, _F("SetSkillGroups Warning: partner_id %d of skill group not found"), 
					grp->GetOwner_id());
				DeleteNetGroup(grp, BOOLEAN_TRUE);
			}
		}
	}
}

RPCResult CPartnerGroup::CB_DeleteListSuccess(LPCPEER pPeer, RPCContext &context, 
											  const UInt32Vector &partnerIDs, const bool &showDelMsg)
{
	CPartner* partner;
	for (UInt32Vector::const_iterator it = partnerIDs.begin(); it != partnerIDs.end(); ++it)
	{
		for (int i = 0; i < GetCount(); ++i)
		{
			partner = GetChild<CPartner>(i);
			if (partner && partner->GetPartner_id() == *it) {
				Remove(partner);
				DeleteNetObject(partner);
				break;
			}
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::CB_AddPartnerGroup(LPCPEER pPeer, RPCContext &context, 
											NetGroup* partnerGroup, NetGroup* skillGroups)
{
	CPartner* pPartner;
	CSkillGroup* pSkillGroup;

	for (int i=0; i< partnerGroup->GetCount(); ++i)
	{
		pPartner = partnerGroup->GetChild<CPartner>(i);
		pSkillGroup = skillGroups->GetChild<CSkillGroup>(i);
		if (pPartner) {
			pPartner->SetSkillGroup(pSkillGroup);
			Add(pPartner);
		}
	}
	DeleteNetGroup(partnerGroup);
	DeleteNetGroup(skillGroups);
	
	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::CB_AddPartner(LPCPEER pPeer, RPCContext &context, 
									   NetObject* partner, NetGroup* skillGroup)
{
	CPartner* pPartner = CastNetObject<CPartner>(partner);
	CSkillGroup* grp = CastNetObject<CSkillGroup>(skillGroup);

	if (pPartner) {
		pPartner->SetSkillGroup(grp);
		Add(pPartner);
	} else {
		DeleteNetObject(partner);
		DeleteNetGroup(skillGroup, BOOLEAN_TRUE);
	}

	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::CB_UpdateAllPartner(LPCPEER pPeer, RPCContext &context, 
											 NetGroup* partnerGroup)
{
	// already updated, just delete the temp NetGroup
	DeleteNetGroup(partnerGroup);
	return RPC_RESULT_OK;
}


RPCResult CPartnerGroup::CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp)
{
	TRACE_ENSURE(skillGrp);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CPartnerGroup CB_AddSkills: count %d"), 
		skillGrp->GetCount());

	CPartner* pPartner = NULL;
	CSkill* skill;
	CSkill* skill2;

	for (UInt8 j = 0; j < skillGrp->GetCount(); ++j)
	{
		skill = skillGrp->GetChild<CSkill>(j);
		if (skill && skill->GetOwner_type() == SKILL_OWNER_TYPE_PARTNER)
		{
			pPartner = GetPartner(skill->GetOwner_id());
			if (pPartner) {
				skill2 = pPartner->GetSkillById(skill->GetSkill_id());
				if (skill2 == NULL) {
					if (pPartner->m_pSkillGroup)
						pPartner->m_pSkillGroup->Add(skill);
					else {
						TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CPartnerGroup CB_AddSkills Warning: null skill group for partner_id %d"), 
							pPartner->m_pSkillGroup);
						DeleteNetObject(skill);
					}
				} else if (skill2 != skill) {
					TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("CPartnerGroup CB_AddSkills Warning: skill_id %d already exist, skill deleted netId %d,%d"), 
						skill->GetSkill_id(), skill->GetNetId().srvId, skill->GetNetId().objId);
						DeleteNetObject(skill);
				}
			}
		}
	}
	DeleteNetGroup(skillGrp);

	return RPC_RESULT_OK;
}

CPartner* CPartnerGroup::Get(UInt16 index)
{
	if (index < GetCount())
		return GetChild<CPartner>(index);
	else 
		return NULL;
}

CPartner* CPartnerGroup::GetPartner(UInt32 partner_id)
{
	CPartner* partner;
	for (int i = 0; i < GetCount(); ++i)
	{
		partner = GetChild<CPartner>(i);
		if (partner->GetPartner_id() == partner_id)
			return partner;
	}
	return NULL;
}

UInt8 CPartnerGroup::GetPartnerCount(UInt32 mob_id)
{
	UInt8 count = 0;
	CPartner* partner;
	for (int i = 0; i < GetCount(); ++i)
	{
		partner = GetChild<CPartner>(i);
		if (partner->GetMob_id() == mob_id)
			count++;
	}
	return count;
}

void CPartnerGroup::ShowAllPartner()
{
	if (GetCount() == 0)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("No partner exist"));
		return;
	}

	CPartner* partner;
	for (int i = 0; i < GetCount(); ++i)
	{
		partner = GetChild<CPartner>(i);
		if (partner)
		{
			TRACE_INFOL_8(GLOBAL_LOGGER, _F("Partner %d: partner_id %d, mob_id %d, state %d, hp %d/%d, sp %d/%d"), 
				i, partner->GetPartner_id(), partner->GetMob_id(), partner->GetState(), 
				partner->GetHP(), partner->GetMaxHP(), 
				partner->GetSP(), partner->GetMaxSP());
			partner->ShowSkills();
		}
	}
}

CPartner* CPartnerGroup::GetPartnerByIndex(int index)
{
	CPartner* pPartner = NULL;
	if (index >= 0 && index < GetCount())
		pPartner = GetChild<CPartner>(index);
	
	return pPartner;
}

RPCResult CPartnerGroup::CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::CB_PartnerCultivationUp(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt8 &msgValue, const UInt32 &src_partnerID)
{
	return RPC_RESULT_OK;
}