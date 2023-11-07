//-- Common
#include "Common.h"
//-- Library
#include "Proxy/Logic/Map.h"
#include "Proxy/Logic/BatActor.h"
#include "Proxy/Logic/BatCharacter.h"
#include "Proxy/Logic/BatMob.h"
#include "Proxy/Logic/Party.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/PetGroup.h"
#include "Proxy/Logic/Skill.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Proxy/Logic/ChargeShop.h"
#include "Proxy/Logic/HawkGroup.h"
#include "Proxy/Logic/QuestSystem.h"
#include "Proxy/Logic/Guild.h"
#include "QGL/MapActorControl.h"
#include "QGL/MapNPCControl.h"
#include "QGL/CharacterControl.h"
#include "QGL/ScriptControl.h"
#include "QGL/QGLApplication.h"
#include "QGL/Global.h"

void HelloWorld()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Got Lua Call: QGL Hello World"));
}

CMapActorControl* GetMapActorCtrl(CMapActor *pActor)
{
	if (pActor)
		return QGLApplication::GetInstance().FindMapActorCtrlByNetID(pActor->GetNetId());
	else
		return NULL;
}

CMapNPCControl* GetMapNPCCtrl(CMapNPC *pNpc)
{
	if (pNpc)
		return QGLApplication::GetInstance().FindNPCCtrlByID(pNpc->GetNpc_uid());
	else
		return NULL;
}

CCharacterControl* GetCharacterCtrl(CCharacter *pChar)
{
	if (pChar)
		return QGLApplication::GetInstance().FindCharCtrlByNetObject(pChar);
	else
		return NULL;
}

CCharacterControl* GetCharacter()
{
	return gGlobal.g_pCharacter;
}

CMap* GetMap()
{
	return gGlobal.g_pMap;
}

CScriptControl* GetScript()
{
	return gGlobal.g_pScriptControl;
}

CQuestSystem* GetQuestSys()
{
	return gGlobal.g_QuestSystem;
}

CHawkGroup* GetMyHawk()
{
	return gGlobal.g_pMyHawkGroup;
}

CHawkGroup* GetTargetHawk()
{
	return gGlobal.g_pTargetHawkGroup;
}

CBattleControl* GetBattle()
{
	return gGlobal.g_pBattleControl;
}

CParty* GetParty()
{
	if (NULL != gGlobal.g_pParty) 
		return gGlobal.g_pParty;
	else
		return gGlobal.g_pTempLeaveParty;
}

CItemBag* GetItemBag()
{
	return gGlobal.m_pBag;
}

CPartnerGroup* GetPartnerGroup()
{
	return gGlobal.g_pPartnerGroup;
}

CPetGroup* GetPetGroup()
{
	return gGlobal.g_PetLst;
}

UInt16 GetSkillCount() 
{
	if (gGlobal.g_SkillLst)
		return gGlobal.g_SkillLst->GetCount();
	else
		return 0;
}

CSkill* GetSkill(UInt16 i)
{
	if (gGlobal.g_SkillLst && i < gGlobal.g_SkillLst->GetCount())
		return (gGlobal.g_SkillLst->GetChild<CSkill>(i));
	else
		return NULL;
}
