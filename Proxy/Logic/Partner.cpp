//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Partner.h"

#ifdef QGL
#include "QGL/Global.h"
#include "QGL/MenuPartnerProcess.h"
#include "QGL/CharacterControl.h"
#include "QGL/MenuItemProcess.h"
#include "QGL/MenuHawkerProcess.h"
#include "Proxy/Logic/Skill.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Resource/resskill.h"
#include "Resource/resskill.h"
#include "Resource/ClientResManager.h"
#include "QGL/MenuSystemSetting.h"
#include "Resource/ResMob.h"
#include "QGL/MenuPartnerCultivationUpProcess.h"
#endif // QGL

REGISTER_CLASSTYPE(CPartner, Partner_CLASSID);

CPartner::CPartner()
{
	m_iBattleHP = -1;
	m_iBattleSP = -1;
	m_bGiven = false;
	//m_bLevelUpSound = false;
	m_pSkillGrp = NULL;
	m_data = NULL;
	m_iBattleATK = -1;
	m_iBattleDEF = -1;
	m_iBattleHIT = -1;
	m_iBattleEVA = -1;
	m_iBattlePOW = -1;
	m_iBattleSPD = -1;
	m_iBattleMaxHP = -1;
}

void CPartner::OnUpdateId(const UInt32 &partner_id, const UInt32 &mob_id)
{
#ifdef QGL
	if (gGlobal.m_resource)
		m_data = gGlobal.LoadMobTemplate(mob_id);
#endif // QGL
}

void CPartner::OnUpdateOwner(const UInt32 &owner_id)
{
}

void CPartner::OnUpdateInfo(const String &raname)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess)
	{
		gGlobal.g_pPartnerProcess->UpdatePartnerNameList();
		if (GetState() == STATE_FIGHT)
		{
			gGlobal.g_pPartnerProcess->UpdateSmallIcon(true, GetPartner_id());
		}
	}

	if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
	{
		gGlobal.g_pItemProcess->UpdateItemPartnerNameList();
	}
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPartnerNameList();
#endif // QGL
}

void CPartner::OnUpdateLevel(const UInt16 &level)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
		gGlobal.g_pPartnerProcess->UpdateLevel();
#endif // QGL
}

void CPartner::OnUpdateNextLvExp(const UInt32 &nextLvExp)
{
}

void CPartner::OnUpdateExp(const UInt32 &exp)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
		gGlobal.g_pPartnerProcess->UpdateExp();
#endif // QGL
}

void CPartner::OnUpdateAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
		gGlobal.g_pPartnerProcess->UpdateAttrib1();
#endif // QGL
}

void CPartner::OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
		gGlobal.g_pPartnerProcess->UpdateAttrib2();
#endif // QGL
}

void CPartner::OnUpdateAttrPoint(const UInt16 &attrPoint)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
		gGlobal.g_pPartnerProcess->UpdateAttrPoint();
#endif // QGL
}

void CPartner::OnUpdateHP(const UInt32 &HP,  const UInt32 &maxHP)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
	{
		gGlobal.g_pPartnerProcess->UpdateHP();
	}

	if (GetState() == CPartner::STATE_FIGHT && gGlobal.g_pPartnerGroup )
	{
		gGlobal.g_pPartnerProcess->UpdateSmallIcon(true, GetPartner_id());
	}

	if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
	{
		gGlobal.g_pItemProcess->UpdateItemPartner();
	}
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPartner();
#endif // QGL
}

void CPartner::OnUpdateSP(const UInt32 &SP,  const UInt32 &maxSP)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
	{
		gGlobal.g_pPartnerProcess->UpdateSP();
	}

	if (GetState() == STATE_FIGHT && gGlobal.g_pPartnerGroup )
	{
		gGlobal.g_pPartnerProcess->UpdateSmallIcon(true, GetPartner_id());
	}

	if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
	{
		gGlobal.g_pItemProcess->UpdateItemPartner();
	}
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPartner();
#endif // QGL
}

void CPartner::OnUpdateGrow(const float &grow)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
		gGlobal.g_pPartnerProcess->UpdateNative();
#endif // QGL
}

void CPartner::OnUpdateFavor(const float &favor)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
		gGlobal.g_pPartnerProcess->UpdateFavor();

	if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
	{
		gGlobal.g_pItemProcess->UpdateItemPartner();
	}

	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPartner();
#endif // QGL
}

void CPartner::OnUpdateHealth(const UInt16 &health)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
	{
		gGlobal.g_pPartnerProcess->UpdateHealth();
	}
#endif // QGL
}

void CPartner::OnUpdateState(const UInt8 &state)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess)
	{
		gGlobal.g_pPartnerProcess->UpdatePartnerNameList();
		gGlobal.g_pPartnerProcess->UpdateStateButton();

		if (state == CPartner::STATE_FIGHT)
			gGlobal.g_pPartnerProcess->UpdateSmallIcon(true, GetPartner_id());
	}

	if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
	{
		gGlobal.g_pItemProcess->UpdateItemPartnerNameList();
	}
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPartnerNameList();
#endif // QGL
}

void CPartner::OnUpdateBuff(const String &buff)
{
	//if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess)
	//	gGlobal.g_pPartnerProcess->UpdatePartnerMenu();
}

void CPartner::OnUpdateGoodness(const Int32 &goodness)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
	{
		gGlobal.g_pPartnerProcess->UpdateGoodness();
	}
#endif // QGL
}

void CPartner::OnUpdateOutLookColor(const UInt16 &outLookColor)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
	{
		gGlobal.g_pPartnerProcess->UpdatePartnerModel();
	}

	if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
	{
		gGlobal.g_pItemProcess->UpdateItemPartner();
	}
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPartner();
#endif // QGL
}

void CPartner::OnUpdateAssetLock(const Boolean &lock)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess)
	{
		gGlobal.g_pPartnerProcess->UpdatePartnerNameList();
	}
	if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
	{
		gGlobal.g_pItemProcess->UpdateItemPartnerNameList();
	}

#endif // QGL
}

void CPartner::OnUpdateCultivation(const UInt16 &cultivation)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
	{
		gGlobal.g_pPartnerProcess->UpdateCultivation();
	}

	if (gGlobal.g_pPartnerCultivationUpProcess && gGlobal.g_pPartnerCultivationUpProcess->GetSrcPartnerID() == GetPartner_id())
	{
		gGlobal.g_pPartnerCultivationUpProcess->UpdatePartnerCultivationUp();
	}
#endif // QGL
}

void CPartner::OnUpdatePractice(const UInt16 &practice)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
	{
		gGlobal.g_pPartnerProcess->UpdatePractice();
	}
#endif // QGL
}

void CPartner::OnUpdateGrowLevel(const UInt8 &growLevel)
{
#ifdef QGL
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CPartner OnUpdateGrowLevel: %d."), growLevel);

	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
		gGlobal.g_pPartnerProcess->UpdateNative();
#endif // QGL
}

RPCResult CPartner::CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CPartner CB_RaiseAttrib:"));

	return RPC_RESULT_OK;
}

RPCResult CPartner::CB_UpdateSuccess(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CPartner CB_UpdateSuccess:"));

	return RPC_RESULT_OK;
}

RPCResult CPartner::CB_ChangePartnerName(LPCPEER pPeer, RPCContext &context, const bool &success)
{
#ifdef QGL
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CPartner CB_ChangeName:"));
	
	if (success)
	{

	}
	else
	{
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CREATE_CHECK_FAIL")), CHANNELTYPE_SYSTEM);
	}
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult CPartner::CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CPartner CB_ChangePartnerState:"));

	return RPC_RESULT_OK;
}

RPCResult CPartner::CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CPartner CB_LearnSkillMessage:"));

#ifdef QGL
	if (skillID > 0)
	{
		String text;
	
		const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(skillID);
		if (pSkillData)
		{
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_LEARN_SKILL")), GetRaname().c_str(), String(pSkillData->name).c_str() );
			gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
			gGlobal.PrintPlayerMessage(text);
		}
	}
#endif // QGL
	return RPC_RESULT_OK;
}

void CPartner::SetSkillGroup(CSkillGroup *grp)
{
#ifdef QGL
	if (grp == NULL)
		return;

	if (m_pSkillGrp && m_pSkillGrp != grp)
		gGlobal.g_pPartnerGroup->RemoveSkillGroup(GetPartner_id());

	m_pSkillGrp = grp;
	m_pSkillGrp->SetOwnerType(SKILL_OWNER_TYPE_PARTNER);
#endif // QGL
	
}

void CPartner::SetBattleHP(Int bhp)
{
#ifdef QGL
	m_iBattleHP = bhp;
	if (gGlobal.g_pPartnerProcess  && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
		gGlobal.g_pPartnerProcess->UpdateHP();

	/*if (GetState() == 1 && gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE) 
	{
		gGlobal.g_pPartnerProcess->UpdateSmallIcon(true, GetPartner_id());
	}*/
#endif // QGL
}

void CPartner::SetBattleMaxHP(Int bmaxhp)
{
#ifdef QGL
	m_iBattleMaxHP = bmaxhp;
	if (gGlobal.g_pPartnerProcess  && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
		gGlobal.g_pPartnerProcess->UpdateHP();

	/*if (GetState() == 1 && gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE) 
	{
		gGlobal.g_pPartnerProcess->UpdateSmallIcon(true, GetPartner_id());
	}*/
#endif // QGL
}


void CPartner::SetBattleSP(Int bsp)
{
#ifdef QGL
	m_iBattleSP = bsp;

	if (gGlobal.g_pPartnerProcess  && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
		gGlobal.g_pPartnerProcess->UpdateSP();

	/*if (GetState() == 1 && gGlobal.g_pCharacter->GetAction() == CCharacter::ST_BATTLE)
	{
		gGlobal.g_pPartnerProcess->UpdateSmallIcon(true, GetPartner_id());
	}*/
#endif // QGL
}

void CPartner::OnUpdateNative(const UInt16 &HP_native, const UInt16 &SP_native, const UInt16 &ATK_native, const UInt16 &DEF_native, const UInt16 &SPD_native, const UInt16 &POW_native, const UInt16 &HIT_native, const UInt16 &EVA_native)
{
#ifdef QGL
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerProcess->IsOnShowPartner(GetPartner_id()))
	{
		gGlobal.g_pPartnerProcess->UpdateNative();
	}
#endif // QGL
}

RPCResult CPartner::CB_FalseJoinBattle(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType)
{
#ifdef QGL
	switch(MsgType)
	{
		case BATTLE_ERROR_MSG_NOT_FAVOR:
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_ERROR_MSG_NOT_FAVOR")), GetRaname().c_str());
			gGlobal.PrintPlayerMessage(text.c_str());

		break;
		}
	case BATTLE_ERROR_MSG_LEVEL_TOO_HIGH:
		{
				
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_LEVEL_TOO_HIGH")), GetRaname().c_str());
			gGlobal.PrintPlayerMessage(text.c_str());
	
		break;
		}
	case BATTLE_ERROR_MSG_NOT_HEALTH:
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_NO_HEALTH")), GetRaname().c_str());
			gGlobal.PrintPlayerMessage(text.c_str());
	
		break;

		}
	}
	if (gGlobal.g_pPartnerProcess)
		gGlobal.g_pPartnerProcess->UpdateSmallIcon(false);
#endif // QGL
	return RPC_RESULT_OK;
}

CPartner::~CPartner()
{
	//if (m_pSkillGrp)
		//DeleteNetGroup(m_pSkillGrp, BOOLEAN_TRUE);
		m_pSkillGrp = NULL;

}

Boolean CPartner::CanCall()
{
#ifdef QGL
	if (gGlobal.g_pCharacter)
		return (GetLevel() < gGlobal.g_pCharacter->GetLevel() + 10);
	else
#endif // QGL
		return BOOLEAN_FALSE;
}

UInt16 CPartner::GetSkillCount()
{
#ifdef QGL
	if (m_pSkillGrp && !FindNetObject(m_pSkillGrp->GetNetId())) {
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("Partner Warning: [partner skill group already deleted], partner_id %d, netId %d,%d, objId %d"), 
			GetPartner_id(), m_pSkillGrp->GetNetId().srvId, m_pSkillGrp->GetNetId().objId,
			m_pSkillGrp->GetID());
		m_pSkillGrp = NULL;
#endif // QGL
		return 0;
#ifdef QGL
	}

	return (m_pSkillGrp ? m_pSkillGrp->GetCount() : 0);
#endif // QGL
}

CSkill* CPartner::GetSkill(UInt16 i)
{
#ifdef QGL
	if (m_pSkillGrp && i < m_pSkillGrp->GetCount()) {
		return m_pSkillGrp->GetChild<CSkill>(i);
	} else {
#endif // QGL
		return NULL;
#ifdef QGL
	}
#endif // QGL
}

CSkill* CPartner::GetSkillById(UInt16 skill_id)
{
	if (m_pSkillGrp == NULL)
		return NULL;

#ifdef QGL
	CSkill *pSkill;
	for (UInt16 i = 0; i < m_pSkillGrp->GetCount(); i++) {
		pSkill =  m_pSkillGrp->GetChild<CSkill>(i);
		if (pSkill && pSkill->GetSkill_id() == skill_id)
			return pSkill;
	}
#endif // QGL
	return NULL;
}
