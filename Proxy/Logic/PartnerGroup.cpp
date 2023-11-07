//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/PartnerGroup.h"
//-- Library
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/QuestLog.h"
#include "Proxy/Logic/QuestSystem.h"
#include "Proxy/Logic/Skill.h"
#include "Proxy/Logic/SkillGroup.h"
#include "QGL/Global.h"
#include "QGL/MenuPartnerProcess.h"
#include "QGL/CharacterControl.h"
#include "QGL/MenuItemProcess.h"
#include "QGL/MenuHawkerProcess.h"
#include "QGL/MenuHelpProcess.h"
#include "Common/CallLuaFunc.h"
#include "QGL/MenuWareHouseProcess.h"
#include "QGL/MenuPartnerCultivationUpProcess.h"

REGISTER_CLASSTYPE(CPartnerGroup, PartnerGroup_CLASSID);

CPartnerGroup::CPartnerGroup()
{
	m_partnerSkillMap.clear();
}

CPartnerGroup::~CPartnerGroup()
{
	for (SkillMap::iterator itr = m_partnerSkillMap.begin(); itr != m_partnerSkillMap.end(); ++itr)
		DeleteNetGroup(itr->second, BOOLEAN_TRUE);
	
	m_partnerSkillMap.clear();
}

CSkillGroup* CPartnerGroup::GetSkillGroup(UInt32 partnerID)
{
	SkillMap::iterator itr = m_partnerSkillMap.find(partnerID);
	if (itr != m_partnerSkillMap.end())
		return itr->second;
	return NULL;
}

void CPartnerGroup::RemoveSkillGroup(UInt32 partnerID, bool needDelete)
{
	SkillMap::iterator itr = m_partnerSkillMap.find(partnerID);
	if (itr != m_partnerSkillMap.end())
	{
		if (needDelete)
			DeleteNetGroup(itr->second, BOOLEAN_TRUE);
		m_partnerSkillMap.erase(itr);
	}
}

void CPartnerGroup::SetSkillGroup(UInt32 partnerID, CSkillGroup* skillGroup)
{
	if (skillGroup)
	{
		SkillMap::iterator itr = m_partnerSkillMap.find(partnerID);
		if (itr != m_partnerSkillMap.end())
		{
			if (skillGroup != itr->second)
			{
				DeleteNetGroup(itr->second);
				itr->second = skillGroup;
			}
		}
		else
			m_partnerSkillMap.insert(std::make_pair(partnerID, skillGroup));
	}
}

RPCResult CPartnerGroup::CB_DeleteListSuccess(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerIDs, const bool &showDelMsg)
{
	bool related = false;
	if (partnerIDs.size() > 0)
	{
		for(UInt i = 0; i < partnerIDs.size(); ++i)
		{
			CPartner* partner = GetPartner(partnerIDs[i]);
			if (partner == NULL)
				partner = gGlobal.g_pWareHouseProcess->GetPartner(partnerIDs[i]);	
			if (partner)
			{
				if (!related && gGlobal.g_QuestSystem && 
					gGlobal.g_QuestSystem->IsRelated(Q_TARGET_PARTNER, partner->GetMob_id()))
					related = true;
				if (showDelMsg)
				{
					String delMessage;
					delMessage.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_DELETE")), partner->GetRaname().c_str());
					gGlobal.PrintMessage(delMessage, CHANNELTYPE_SYSTEM);
				}
				
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Partner: CB_DeleteListSuccess: partner_id %d"), partner->GetPartner_id());
				
				if (partner->GetInWareHouse() == 0)
					Remove(partner);
				else
					gGlobal.g_pWareHouseProcess->RemovePartner(partner);

				if (partner->GetState() == CPartner::STATE_FIGHT && partner->GetInWareHouse() == 0)
					gGlobal.g_pPartnerProcess->UpdateSmallIcon(false);
				
				RemoveSkillGroup(partner->GetPartner_id());
				DeleteNetObject(partner);
			}
		}

		gGlobal.g_pPartnerProcess->ResetValue();
		gGlobal.g_pPartnerProcess->UpdatePartnerMenu();

		if (GetCount() == 0)
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ClosePartner", String());
		
		if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
			gGlobal.g_pItemProcess->UpdateItemPartner(0);
		if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
			gGlobal.g_pHawkerProcess->UpdateOwnerPartner(0);

		if (related)	// refresh npc hint
			gGlobal.g_QuestSystem->RefreshNpcHint();
	}

	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::CB_AddPartnerGroup(LPCPEER pPeer, RPCContext &context, 
											NetGroup* partnerGroup, NetGroup* skillGroups)
{
	CPartner* pPartner = NULL;
	CSkillGroup* pSkillGroup = NULL;
	bool related = false;

	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerGroup->GetCount() == 0 )
	{
		if( partnerGroup->GetCount() == 1)
		{
			pPartner = partnerGroup->GetChild<CPartner>(0);
			if(pPartner && pPartner->GetState() != CPartner::STATE_FIGHT)
			{	
				UIAniIcon* PartnerFaceSparkUi = gGlobal.pUIManager->FindUIAniIcon("PartnerFaceSpark");
				if (PartnerFaceSparkUi)
				{
					if(!PartnerFaceSparkUi->GetAniEnt()->HasAniFile())
						PartnerFaceSparkUi->SetAnimationData(String(_T("HeadIconT.ent")).c_str());
					if (!PartnerFaceSparkUi->IsEnable())
						PartnerFaceSparkUi->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(PartnerFaceSparkUi);
				}
			}
		}
	}

	for (int i=0; i< partnerGroup->GetCount(); ++i)
	{
		pPartner = partnerGroup->GetChild<CPartner>(i);
		pSkillGroup = skillGroups->GetChild<CSkillGroup>(i);
		if (pPartner) {
			SetSkillGroup(pPartner->GetPartner_id(), pSkillGroup);
			pPartner->SetSkillGroup(pSkillGroup);
			Add(pPartner);
			if (!related && gGlobal.g_QuestSystem && 
				gGlobal.g_QuestSystem->IsRelated(Q_TARGET_PARTNER, pPartner->GetMob_id()))
				related = true;
		}
	}
	// update partner icon
	pPartner = gGlobal.g_pPartnerGroup->GetCurrBattlePartner();
	if (pPartner)
		gGlobal.g_pPartnerProcess->UpdateSmallIcon(true, pPartner->GetPartner_id());
	else
		gGlobal.g_pPartnerProcess->UpdateSmallIcon(false);

	if (partnerGroup->GetCount() > 0)
		gGlobal.g_pMenuHelpProcess->HandleEvent(HELP_TIPS_FIRST_GETPARTNER);

	gGlobal.g_pPartnerProcess->SetOnShowPartner(NULL);
	gGlobal.g_pPartnerProcess->UpdatePartnerMenu();

	DeleteNetGroup(partnerGroup);

	if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
		gGlobal.g_pItemProcess->UpdateItemPartner(0);
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPartner(0);

	if (related)	// refresh npc hint
		gGlobal.g_QuestSystem->RefreshNpcHint();

	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::CB_AddPartner(LPCPEER pPeer, RPCContext &context, NetObject* partner,
									   NetGroup* skillGroup)
{
	CPartner* pPartner = NULL;
	CSkillGroup* grp = CastNetObject<CSkillGroup>(skillGroup);

	pPartner = CastNetObject<CPartner>(partner);
	if (pPartner) {
		SetSkillGroup(pPartner->GetPartner_id(), grp);
		pPartner->SetSkillGroup(grp);
		Add(pPartner);
		gGlobal.g_pPartnerProcess->SetOnShowPartner(NULL);
		gGlobal.g_pPartnerProcess->UpdatePartnerMenu();
	} else {
		DeleteNetObject(partner);
		DeleteNetGroup(skillGroup, BOOLEAN_TRUE);
	}

	if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
		gGlobal.g_pItemProcess->UpdateItemPartner(0);
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPartner(0);

	// refresh npc hint
	if (pPartner && gGlobal.g_QuestSystem && 
		gGlobal.g_QuestSystem->IsRelated(Q_TARGET_PARTNER, pPartner->GetMob_id()))
		gGlobal.g_QuestSystem->RefreshNpcHint();

	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

CPartner* CPartnerGroup::GetPartnerByIndex(int index)
{
	CPartner* pPartner = NULL;
	if (index >= 0 && index < GetCount())
		pPartner = GetChild<CPartner>(index);
	
	return pPartner;
}


CPartner* CPartnerGroup::GetPartner(UInt32 partner_id)
{
	CPartner* partner;
	for (Index i = C_INDEX(0); i < GetCount(); ++i)
	{
		partner = GetChild<CPartner>(i);
		if (partner && partner->GetPartner_id() == partner_id)
			return partner;
	}
	return NULL;
}

Int32 CPartnerGroup::GetPartnerIndex(UInt32 partner_id)
{
	CPartner* partner;
	for (Index i = C_INDEX(0); i < GetCount(); ++i)
	{
		partner = GetChild<CPartner>(i);
		if (partner && partner->GetPartner_id() == partner_id)
			return i;
	}
	return -1;
}

RPCResult CPartnerGroup::CB_UpdateAllPartner(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup)
{
	if (partnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pWareHouseProcess)
	{		
		CPartner* partner = NULL;

		for (Index i = C_INDEX(0); i < partnerGroup->GetCount(); ++i)
		{
			partner = partnerGroup->GetChild<CPartner>(i);
			if (partner && partner->IsUpdated(CPartner::inWareHouseMask))
			{
				if (partner->GetInWareHouse() == 0 && GetPartner(partner->GetPartner_id()) == NULL)
				{
					gGlobal.g_pWareHouseProcess->RemovePartner(partner);
					Add(partner);
					SetSkillGroup(partner->GetPartner_id(), partner->m_pSkillGrp);
					partner->SetGiven(false);
				}
				else if (partner->GetInWareHouse() == 1 && gGlobal.g_pWareHouseProcess->GetPartner(partner->GetPartner_id()) == NULL)
				{
					Remove(partner);
					RemoveSkillGroup(partner->GetPartner_id(), false);
					gGlobal.g_pWareHouseProcess->AddPartner(partner);
					partner->SetGiven(false);
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SelectWarehousePartner", String());
				}
				gGlobal.g_pWareHouseProcess->UpdateWareHouse();
			}			
		}
		if (GetCount() == 0)
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ClosePartner", String());
		else
		{
			gGlobal.g_pPartnerProcess->SetRemoving(false);
			gGlobal.g_pPartnerProcess->SetOnShowPartner(NULL);
			gGlobal.g_pPartnerProcess->UpdatePartnerMenu();
		}

		if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
			gGlobal.g_pItemProcess->UpdateItemPartner();
		if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
			gGlobal.g_pHawkerProcess->UpdateOwnerPartner();

		DeleteNetGroup(partnerGroup);
	}
	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp)
{
	CPartner* pPartner = NULL;

	for (Index j = C_INDEX(0); j < skillGrp->GetCount(); ++j)
	{
		CSkill* skill = skillGrp->GetChild<CSkill>(j);
		if (skill && skill->GetOwner_type() == SKILL_OWNER_TYPE_PARTNER)
		{
			pPartner = GetPartner(skill->GetOwner_id());
			if (pPartner && pPartner->m_pSkillGrp)
				pPartner->m_pSkillGrp->Add(skill);
		}
	}

	return RPC_RESULT_OK;
}

CPartner* CPartnerGroup::GetCurrBattlePartner()
{
	for ( Index i = C_INDEX(0); i < GetCount(); ++i )
	{
		CPartner* pPartner = GetChild<CPartner>(i);
		if ( pPartner && pPartner->GetState() == CPartner::STATE_FIGHT )
			return pPartner;
	}
	return NULL;
}

void CPartnerGroup::ResetBattleAllHP()
{
	for (Index i = C_INDEX(0); i < GetCount(); ++i)
	{
		CPartner* pPartner = GetChild<CPartner>(i);
		if (pPartner)
		{
			pPartner->SetBattleHP(-1);
			pPartner->SetBattleSP(-1);
			pPartner->SetAttrib2(-1, -1, -1, -1, -1, -1);
		}
	}
}

UInt8 CPartnerGroup::GetPartnerCount(UInt32 mob_id)
{
	UInt count = 0;
	CPartner* partner;
	for (Index i = C_INDEX(0); i < GetCount(); ++i)
	{
		partner = GetChild<CPartner>(i);
		if (partner && partner->GetMob_id() == mob_id)
			count++;
	}
	return count;
}

RPCResult CPartnerGroup::CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex)
{
	if (gGlobal.g_pWareHouseProcess)
	{
		gGlobal.g_pWareHouseProcess->ShowResult(processIndex, msgIndex);

		if (msgIndex != WAREHOUSE_SUCCESS)
			gGlobal.g_pWareHouseProcess->UpdateWareHouse();		
	}
	return RPC_RESULT_OK;
}

RPCResult CPartnerGroup::CB_PartnerCultivationUp(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt8 &msgValue, const UInt32 &src_partnerID)
{
	if (gGlobal.g_pPartnerCultivationUpProcess)
	{
		gGlobal.g_pPartnerCultivationUpProcess->PartnerCultivationUpReply(msgIndex, msgValue, src_partnerID);
	}
	return RPC_RESULT_OK;
}