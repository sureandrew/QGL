//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/BatActor.h"
//-- Library
#include "Proxy/Logic/BatCharacter.h"
#include "Proxy/Logic/BatMob.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/Item.h"

#ifdef QGL
#include "QGL/Global.h"
#include "QGL/BattleControl.h"
#include "QGL/MenuCharProcess.h"
#include "QGL/MenuPartnerProcess.h"
#include "Resource/ResSkill.h"
#include "Resource/ClientResManager.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#endif // QGL

REGISTER_CLASSTYPE(CBatActor, BatActor_CLASSID);

CBatActor::CBatActor()
{
	pCharView = NULL;
}
RPCResult CBatActor::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CBatActor Error: errCode %d, %s"), errCode, errMsg.c_str());
#ifdef QGL
	if (!errMsg.IsEmpty()) {
		if (errMsg.Left(2) == _T("MS")) {
			String text(gGlobal.GetStringTable()->Get(errMsg.c_str()));
			if (!text.IsEmpty())
				gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
			else
				gGlobal.PrintMessage(errMsg, CHANNELTYPE_SYSTEM);
		} 
	}
	if (gGlobal.g_pBattleControl)
	{
		CCharacterControl * pCharCtrl = gGlobal.g_pBattleControl->FindCharCtrlByBatActor(this);
		if (pCharCtrl)
			pCharCtrl->SetIsSentCommand(false);
		gGlobal.g_pBattleControl->OnCancelUseItemInBattle();
	}

#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult CBatActor::CB_CommandSuccess(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_CommandSuccess: index %d, command %d"), GetIndex(), cmd);

	//if (pCharView)
	//	pCharView->GetCharMainView()->SetPlaySpeed(1.0f);
#ifdef QGL
	if (gGlobal.g_pBattleControl)
		gGlobal.g_pBattleControl->NotifyCommandSuccess(GetIndex(), cmd);
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult CBatActor::CB_Inputed(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_Inputed: index %d"), GetIndex());

	//if (pCharView)
	//	pCharView->GetCharMainView()->SetPlaySpeed(1.0f);

#ifdef QGL
	if (gGlobal.g_pBattleControl)
		gGlobal.g_pBattleControl->NotifyCommandSuccess(GetIndex(), cmd);
#endif // QGL

	return RPC_RESULT_OK;
}


RPCResult CBatActor::CB_AutoCount(LPCPEER pPeer, RPCContext &context, const UInt16 &count)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_AutoCount: turn count %d"), count);
	#ifdef QGL
	if (gGlobal.g_pBattleControl)
		gGlobal.g_pBattleControl->ShowAutoCountBox(count);
	#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult CBatActor::CB_BattleMsg(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType, const String &Msg, const UInt16 &subType)
{
	#ifdef QGL
	switch(MsgType)
	{
	case BATTLE_ERROR_MSG_BLOCK_ACTION:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_ERROR_MSG_BLOCK_ACTION")));
		break;
	case BATTLE_ERROR_MSG_CATCH_NO_SP:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_ERROR_MSG_CATCH_NO_SP")));
		break;
	case BATTLE_ERROR_MSG_CATCH_DIE:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_ERROR_MSG_CATCH_DIE")));
		break;
	case BATTLE_ERROR_MSG_CATCH_GOODNESS:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_ERROR_MSG_CATCH_GOODNESS")));
		break;
	case BATTLE_ERROR_MSG_CATCH_FULL_PARTNER:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_ERROR_MSG_CATCH_FULL_PARTNER")));
		break;
	case BATTLE_ERROR_MSG_CATCH_LEVEL:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_ERROR_MSG_CATCH_LEVEL")));
		break;
	case BATTLE_ERROR_MSG_CATCH_SAME_TEAM:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_ERROR_MSG_CATCH_SAME_LEVEL")));
		break;
	case BATTLE_ERROR_MSG_GIVEN:
		gGlobal.PrintPlayerMessage(Msg);
		break;
	case BATTLE_USE_MSG_GIVEN:
		gGlobal.PrintMessage(Msg, CHANNELTYPE_SYSTEM);
		break;
	case SKILL_USE_FAIL_TYPE_TARGET_INVALID:
		{
			const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(subType);
			if (pSkillData)
			{
				String Buf;
				Buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_FAIL_TYPE_TARGET_INVALID")),
					pSkillData->name);
				gGlobal.PrintPlayerMessage(Buf.c_str());
			}
			break;
		}
	case SKILL_USE_FAIL_TYPE_NO_TARGET:
		{
		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(subType);
			if (pSkillData)
			{
				String Buf;
				Buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_FAIL_TYPE_NO_TARGET")),
					pSkillData->name);
				gGlobal.PrintPlayerMessage(Buf.c_str());
			}
		
		break;
		}
	case SKILL_USE_FAIL_TYPE_BLOCK:
		{
		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(subType);
			if (pSkillData)
			{
				String Buf;
				Buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_FAIL_TYPE_BLOCK")),
					pSkillData->name);
				gGlobal.PrintPlayerMessage(Buf.c_str());
			}
		
		break;
		}
	case SKILL_USE_FAIL_TYPE_NOITEM:
		{
		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(subType);
			if (pSkillData)
			{
				String Buf;
				Buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_FAIL_TYPE_NOITEM")),
					pSkillData->name);
				gGlobal.PrintPlayerMessage(Buf.c_str());
			}
		
		break;
		}
	case BATTLE_ERROR_MSG_NOT_FAVOR:
		{
			if (subType > 0)
			{
				CPartner* pPartner = gGlobal.g_pPartnerGroup->GetPartner(subType);
				if (pPartner)
				{
					String text;
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_ERROR_MSG_NOT_FAVOR")), pPartner->GetRaname());
					gGlobal.PrintPlayerMessage(text.c_str());
				}
			}
	
		break;
		}
	case BATTLE_ERROR_MSG_LEVEL_TOO_HIGH:
		{
			if (subType > 0)
			{
				CPartner* pPartner = gGlobal.g_pPartnerGroup->GetPartner(subType);
				if (pPartner)
				{
					String text;
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_LEVEL_TOO_HIGH")), pPartner->GetRaname());
					gGlobal.PrintPlayerMessage(text.c_str());
				}
			}
		break;
		}
	default:
		gGlobal.PrintMessage(Msg, CHANNELTYPE_SYSTEM);
		break;
	}
	//gGlobal.PrintMessage(Msg, CHANNELTYPE_SYSTEM);
	#endif // QGL
	return RPC_RESULT_OK;
}

void CBatActor::OnUpdateHP(const UInt16 &HP, const Int16 &dHP, const Int16 &dShowHP)
{
	/*if ( gGlobal.g_pCharacter && (this == gGlobal.g_pCharacter->GetBatActorObject()) )
	{
		gGlobal.SetDisplayHP(HP, GetMHP(), GetMaxHP());
	}*/
}

void CBatActor::OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD)
{
	if ( gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer() )
	{
		if (this == gGlobal.g_pBattleControl->GetControlPlayer()->GetBatActorObject()) 
			gGlobal.g_pCharProcess->UpdateCharAttr1();
		else if (gGlobal.g_pBattleControl->GetControlPartner() && gGlobal.g_pBattleControl->GetControlPartner()->GetBatActorObject() == this)
		{
			CBatMob* pmob = CastNetObject<CBatMob>(gGlobal.g_pBattleControl->GetControlPartner()->GetBatActorObject());
			if (pmob)
			{
				CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(pmob->GetPartner_id());
				if (partner)
				{
					partner->SetAttrib2(ATK, DEF, HIT, EVA, POW, SPD);
					gGlobal.g_pPartnerProcess->UpdateAttrib2();
				}
			}
		}
	}
	
}

void CBatActor::OnUpdateBuff(const String &buff)
{
	if (pCharView)
		pCharView->OnUpdateBuff(buff);
}
void CBatActor::OnUpdateBuffCache(const String &buffCache)
{
	if (pCharView)
		pCharView->OnUpdateBuffCache(buffCache);
}



bool CBatActor::IsPlayer()
{
	return GetClassID() == BatCharacter_CLASSID;
}
bool CBatActor::IsMob()
{
	return GetClassID() == BatMob_CLASSID;
}
bool CBatActor::IsParnter()
{
	if (GetClassID() == BatMob_CLASSID)
	{
		CBatMob * pMob = CastNetObject<CBatMob>(this);
		if (pMob)
		{
			return pMob->GetOwner_id() > 0;
		}
	}
	return false;
}
bool CBatActor::IsBoss()
{
	if (GetClassID() == BatMob_CLASSID)
	{
		CBatMob * pMob = CastNetObject<CBatMob>(this);
		if (pMob)
		{
			return (pMob->GetBoss() == BOOLEAN_TRUE);
		}
	}
	return false;
}