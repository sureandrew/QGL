//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/SkillGroup.h"
//-- Library
#include "Proxy/Logic/Skill.h"
#include "QGL/Global.h"
#include "QGL/MenuSkillProcess.h"
#include "QGL/MenuPartnerProcess.h"
#include "QGL/MenuPetProcess.h"
#include "QGL/MenuHelpProcess.h"

REGISTER_CLASSTYPE(CSkillGroup, SkillGroup_CLASSID);

CSkillGroup::CSkillGroup()
{
}

CSkillGroup::~CSkillGroup()
{
}

RPCResult CSkillGroup::CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp)
{
	CSkill* pSkill;

	for (UInt16 i = 0; i < skillGrp->GetCount(); i++)
	{
		pSkill = skillGrp->GetChild<CSkill>(i);
		if (pSkill)
			Add(pSkill);
	}

	

	if ( m_iOwnerType == SKILL_OWNER_TYPE_CHAR)
	{
		if (gGlobal.g_pSkillProcess->GetIsShop())
			gGlobal.g_pSkillProcess->ConstructByShopID(gGlobal.g_pSkillProcess->GetShopID());
		else
			gGlobal.g_pSkillProcess->ConstructData();
		gGlobal.g_pSkillProcess->UpdateSkillMenu();

		if (skillGrp->GetCount() > 0)
			gGlobal.g_pMenuHelpProcess->HandleEvent(HELP_TIPS_FIRST_LEARNSKILL);
	}
	else if (m_iOwnerType == SKILL_OWNER_TYPE_PARTNER)
	{
		if (gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->GetOnShowPartner())
			gGlobal.g_pPartnerProcess->UpdateSkillMenu(gGlobal.g_pPartnerProcess->GetOnShowPartner());
	}
	else if (m_iOwnerType == SKILL_OWNER_TYPE_PET)
	{
		gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	}

	
	return RPC_RESULT_OK;
}

RPCResult CSkillGroup::CB_RemoveSkills(LPCPEER pPeer, RPCContext &context, const UInt16Vector &skill_ids)
{
	CSkill* pSkill;

	for (UInt16Vector::const_iterator it = skill_ids.begin(); it != skill_ids.end(); ++it)
	{
		for (UInt16 i = 0; i < GetCount(); i++)
		{
			pSkill = GetChild<CSkill>(i);
			if (pSkill && pSkill->GetSkill_id() == (*it)) {
				Remove(pSkill);
				DeleteNetObject(pSkill);
				break;
			}
		}
	}

	if ( m_iOwnerType == SKILL_OWNER_TYPE_CHAR)
	{
		if (gGlobal.g_pSkillProcess->GetIsShop())
			gGlobal.g_pSkillProcess->ConstructByShopID(gGlobal.g_pSkillProcess->GetShopID());
		else
			gGlobal.g_pSkillProcess->ConstructData();
		gGlobal.g_pSkillProcess->UpdateSkillMenu();
	}
	else if (m_iOwnerType == SKILL_OWNER_TYPE_PARTNER)
	{
		if (gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->GetOnShowPartner())
			gGlobal.g_pPartnerProcess->UpdateSkillMenu(gGlobal.g_pPartnerProcess->GetOnShowPartner());
	}
	else if (m_iOwnerType == SKILL_OWNER_TYPE_PET)
	{
		gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	}
	
	return RPC_RESULT_OK;
}

CSkill* CSkillGroup::GetSkillById(UInt16 skill_id)
{
	CSkill* pSkill;

	for (UInt16 i = 0; i < GetCount(); i++)
	{
		pSkill = GetChild<CSkill>(i);
		if (pSkill && pSkill->GetSkill_id() == skill_id)
			return pSkill;
	}

	return NULL;
}