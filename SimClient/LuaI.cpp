//-- Common
#include "Common.h"
#include "Common/CallLuaFunc.h"
#include "Common/common_char_map.h"
//-- Library
#include "Proxy/Logic/MapActor.h"
#include "SimClient/Character.h"
#include "SimClient/MapNPC.h"
#include "SimClient/Map.h"
#include "SimClient/Battle.h"
#include "Proxy/Logic/BatActor.h"
#include "SimClient/BatCharacter.h"
#include "SimClient/BatMob.h"
#include "SimClient/Party.h"
#include "SimClient/Item.h"
#include "SimClient/ItemBag.h"
#include "SimClient/Partner.h"
#include "SimClient/PartnerGroup.h"
#include "SimClient/Pet.h"
#include "SimClient/PetGroup.h"
#include "SimClient/Skill.h"
#include "SimClient/SkillGroup.h"
#include "SimClient/SimClient.h"
#include "SimClient/Global.h"
#include "SimClient/ChargeShop.h"
#include "SimClient/ScriptSystem.h"
#include "SimClient/QuestSystem.h"
#include "SimClient/Guild.h"
#include "Proxy/Service/MessageService.h"


void Execute(char* cmd)
{
	String s(cmd);

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Got Lua Call: Execute %s"), s.c_str());

	if (gGlobal.m_pSimClient)
		gGlobal.m_pSimClient->Execute(s.c_str());
}

void HelloWorld()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Got Lua Call: Hello World"));
}

BlockPos* GetLastMove()
{
	static BlockPos pos;

	if (gGlobal.m_pSimClient) {
		pos.PosX = gGlobal.m_pSimClient->m_lastMoveX;
		pos.PosY = gGlobal.m_pSimClient->m_lastMoveY;
	}

	return &pos;
}

bool CheckBattle()
{
	if (gGlobal.m_pSimClient->m_pBatChar)
		return TRUE;
	else
		return FALSE;
}


bool OppTradeLock()
{
	if (gGlobal.m_pSimClient->m_btarlock)
		return TRUE;
	else
		return FALSE;
 
}

bool SelfTradeLock()
{
	if (gGlobal.m_pSimClient->m_btradelock)
		return TRUE;
	else
		return FALSE;
 
}


bool TradeInProcess()
{
	if (gGlobal.m_pSimClient->m_bTradReqCfm)
		return TRUE;
	else
		return FALSE;
}



CCharacter* GetCharacter()
{
	if (gGlobal.m_pSimClient) {
		return gGlobal.m_pSimClient->m_pCharacter;
	} else {
		return NULL;
	}
}

CMap* GetMap()
{
	if (gGlobal.m_pSimClient) {
		return gGlobal.m_pSimClient->m_pMap;
	} else {
		return NULL;
	}
}

CScriptSystem* GetScript()
{
	if (gGlobal.m_pSimClient)
		return gGlobal.m_pSimClient->m_pScriptSys;
	else
		return NULL;
}

CQuestSystem* GetQuestSys()
{
	if (gGlobal.m_pSimClient)
		return gGlobal.m_pSimClient->m_pQuestSys;
	else
		return NULL;
    
}

CGuild* GetGuild()
{
	if (gGlobal.m_pSimClient)
		return gGlobal.m_pSimClient->m_ownGuild;
	else
		return NULL;
}

CHawkGroup* GetMyHawk()
{
	if (gGlobal.m_pSimClient)
		return gGlobal.m_pSimClient->m_pMyHawkGroup;
	else
		return NULL;
}

CHawkGroup* GetTargetHawk()
{
	if (gGlobal.m_pSimClient)
		return gGlobal.m_pSimClient->m_pTargetHawkGroup;
	else
		return NULL;
}




UInt32 GetShopItemCount()
{
	if (gGlobal.m_pSimClient->m_pshop)
		return gGlobal.m_pSimClient->m_pshop->GetTotalShopItem();
	else
		return 0;
    
}

CChargeShop* GetShop()
{
	if (gGlobal.m_pSimClient)
		return gGlobal.m_pSimClient->m_pshop;
	else
		return NULL;
}

CBatCharacter* GetBatChar()
{
	if (gGlobal.m_pSimClient) {
		return gGlobal.m_pSimClient->m_pBatChar;
	} else {
		return NULL;
	}
}

CBattle* GetBattle()
{
	if (gGlobal.m_pSimClient) {
		return gGlobal.m_pSimClient->m_pBattle;
	} else {
		return NULL;
	}
}

CParty* GetParty()
{
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_pCharacter) {
		return gGlobal.m_pSimClient->m_pCharacter->m_pParty;
	} else {
		return NULL;
	}
}

CItemBag* GetItemBag()
{
	if (gGlobal.m_pSimClient) {
		return gGlobal.m_pSimClient->m_pBag;
	} else {
		return NULL;
	}
}

CPartnerGroup* GetPartnerGroup()
{
	if (gGlobal.m_pSimClient) {
		return gGlobal.m_pSimClient->m_pPartnerGroup;
	} else {
		return NULL;
	}
}

CPetGroup* GetPetGroup()
{
	if (gGlobal.m_pSimClient) {
		return gGlobal.m_pSimClient->m_pPetGroup;
	} else {
		return NULL;
	}
}

CBatMob* GetBatPartner()
{
	if (gGlobal.m_pSimClient) {
		return gGlobal.m_pSimClient->m_pPartner;
	} else {
		return NULL;
	}
}

UInt16 GetSkillCount() 
{
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_pSkillGroup)
		return gGlobal.m_pSimClient->m_pSkillGroup->GetCount();
	else
		return 0;
}

CSkill* GetSkill(UInt16 i)
{
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_pSkillGroup &&
		i < gGlobal.m_pSimClient->m_pSkillGroup->GetCount())
		return (gGlobal.m_pSimClient->m_pSkillGroup->GetChild<CSkill>(i));
	else
		return NULL;
}

UInt8 GetLineCount()
{
	if (gGlobal.m_pSimClient == NULL)
		return 0;

	return (UInt8) gGlobal.m_pSimClient->m_maplines.size();
}

UInt8 GetNextLine(UInt8 line_id)
{
	if (gGlobal.m_pSimClient == NULL)
		return line_id;

	int i;
	int count = (int) gGlobal.m_pSimClient->m_maplines.size();

	if (count < 2)
		return line_id;

	for (i = 0; i < count; i++)
	{
		if (gGlobal.m_pSimClient->m_maplines[i] == line_id)
			break;
	}
	if (i >= count)
		return line_id;

	return gGlobal.m_pSimClient->m_maplines[(i + 1) % count];
}

UInt32 GetTotalGuildNumber()
{
	if (gGlobal.m_pSimClient)
		return (UInt32) gGlobal.m_pSimClient->m_guildMap.size();
	else
		return 0;
}

UInt32 GetTotalGuildMemberNumber()
{
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_ownGuild)
		return gGlobal.m_pSimClient->m_ownGuild->GetMemberCount();
	else
		return 0;
}

UInt32 GetTotalGuildApplicantNumber()
{
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_ownGuild)
		return gGlobal.m_pSimClient->m_ownGuild->GetApplicantCount();
	else
		return 0;
}

Boolean CheckGuildMembership()
{
	if (!gGlobal.m_pSimClient)
		return FALSE;

	if (gGlobal.m_pSimClient->m_ownGuild)
		return TRUE;
	else
		return FALSE;

	return FALSE;

}

UInt8 GetShadowNum()
{
	return gGlobal.m_shadowNum;
}

UInt32 GetClientTotal()
{
	return gGlobal.m_clientTotal;
}