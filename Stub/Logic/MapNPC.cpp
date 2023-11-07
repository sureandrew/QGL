//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Logic/MapNPC.h"
//-- Library
#include "Proxy/Service/WorldService_proxy.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/Map.h"
#include "Resource/ResNpc.h"
#include "MapService/Global.h"
#include "MapService/ViewManager.h"

REGISTER_CLASSTYPE(CMapNPC, MapNPC_CLASSID);

PROTO_LuaPush(CMapNPC)

#define GRACE_PERIOD_TIME		11000

CMapNPC::CMapNPC()
{
	m_pData = NULL;
	m_appearWorld = BOOLEAN_FALSE;
	m_appearTime = 0;
	m_lastCharId = 0;
	m_lifeTime = UINT32_MAX_VALUE;
	m_eventId = 0;
	m_enableBattle = BOOLEAN_TRUE;
	m_createrCharId = 0;
}

CMapNPC::~CMapNPC()
{
	RemoveAllLink();
	CancelAllEvent();
}

void CMapNPC::SetData(const NpcData* pData)
{
	if (pData == NULL)
		return;

	m_pData = pData;
	SetInfo(pData->mid_name, pData->sex, pData->job, 0, pData->faction);
	SetAction(ST_STOP, ::timeGetTime());

	//ClearAllMask();
}

void CMapNPC::OnLeaveMap()
{
	CMapActor::OnLeaveMap();
}

void CMapNPC::Destroy()
{
	m_lifeTime = 0;
	if (m_pMap) {
		NpcLiveInfo npc;

		npc.npcId = GetNpc_id();
		npc.mapId = m_pMap->GetMap_id();
		npc.lineId = m_pMap->GetLine_id();
		npc.ownerId = m_pMap->GetOwner_id();
		npc.x = GetCurPosX();
		npc.y = GetCurPosY();
		if (m_eventId > 0) {
			npc.eventId = m_eventId;
			m_eventId = 0;
		}
		WorldService_Proxy::NotifyNpcDead(gGlobal.pWorldService->pPeer, 
			RPCContext(), npc);
		String strDebugInfo = DebugInfo();
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CMapNPC::Destroy : [NPC Destroy removeNpc] map=%d(%d:%d) m_pMap %s"), 
			GetMap_id(), GetMap_x(), GetMap_y(), strDebugInfo.c_str());

		m_pMap->RemoveNpc(this);
	}
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CMapNPC::Destroy : [EventRaised GraceStartCount]"));
	RaiseUniqEventID(EVT_RELEASE, GRACE_PERIOD_TIME);
}

void CMapNPC::CheckAppear(CMap* pMap, UInt8 gameTime)
{
	if (m_pData == NULL || pMap == NULL)
		return;

	bool bAppear = ((m_pData->appearTime >> (gameTime * 2)) & 0x1);

	if (!m_appearWorld && bAppear)
	{
		// from disappear to appear 
		m_appearWorld = bAppear;
		pMap->AddActor(this);
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("[NPC Appear], map_id %d, %s"), 
			pMap->GetMap_id(), DebugInfo().c_str());
	}
	else if (m_appearWorld && !bAppear)
	{
		// from appear to disappear
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("[NPC Disappear], map_id %d, %s"), 
			pMap->GetMap_id(), DebugInfo().c_str());
		m_appearWorld = bAppear;
		pMap->RemoveActor(this);
	}
	if (m_pMap != pMap)
		m_pMap = pMap;
}

void CMapNPC::SetLifeTime(UInt32 lifeTime)
{
	m_lifeTime = lifeTime;
	if (lifeTime == UINT32_MAX_VALUE)
		CancelUniqEvent(EVT_DESTROY);
	else
		RaiseUniqEventID(EVT_DESTROY, lifeTime * 1000);
}

Boolean CMapNPC::CanTalk(CCharacter* pChar)
{
	if (pChar == NULL || !GetAppearWorld() || GetLifeTime() == 0 || 
		m_pData && m_pData->npc_type == NPC_VISIBLE_EMENY_FOR_ONE_USER && m_lastCharId != 0)	// one ppl talk only
		return BOOLEAN_FALSE;
	if (m_acceptIdSet.empty() ||
		m_acceptIdSet.find(pChar->GetChar_id()) != m_acceptIdSet.end())
	{
		m_lastCharId = pChar->GetChar_id();
		return BOOLEAN_TRUE;
	} else {
		return BOOLEAN_FALSE;
	}
}

void CMapNPC::TalkEnd()
{
	m_lastCharId = 0;
}

void CMapNPC::AddAcceptId(UInt32 char_id)
{
	m_acceptIdSet.insert(char_id);
}

void CMapNPC::ClearAcceptId()
{
	m_acceptIdSet.clear();
}

Boolean CMapNPC::Link(CMapActor* pActor)
{
	if (pActor->GetClassID() == Character_CLASSID)
	{
		ActorMap::iterator itfind = m_actorMap.find(pActor->GetNetId());
		if (itfind == m_actorMap.end())
		{
			m_actorMap.insert(std::make_pair(pActor->GetNetId(), pActor));
			return (BOOLEAN_TRUE);
		}
	}
	return (BOOLEAN_FALSE);
}

Boolean CMapNPC::Unlink(CMapActor* pActor)
{
	if (pActor->GetClassID() == Character_CLASSID)
	{
		ActorMap::iterator itfind = m_actorMap.find(pActor->GetNetId());
		if (itfind != m_actorMap.end())
		{
			TRACE_ENSURE(itfind->second == pActor);
			m_actorMap.erase(itfind);
			return (BOOLEAN_TRUE);
		}
	}
	return (BOOLEAN_FALSE);
}

void CMapNPC::RemoveAllLink(void)
{
	ActorMap tempmap;

	m_actorMap.swap(tempmap);
	for (ActorMap::iterator it = tempmap.begin(); it != tempmap.end(); ++it)
	{
		CCharacter* pchar = CastNetObject<CCharacter>(it->second);
		if (pchar)
			pchar->m_viewManager->RemoveView(this, BOOLEAN_TRUE);
	}
}

void CMapNPC::SetQuestHint(UInt16 quest_id, UInt16 minLv, UInt16 maxLv, 
						   UInt8 sex, UInt8 job, UInt8 faction)
{
	TRACE_VERBOSEDTL_8(GLOBAL_LOGGER, _F("SetQuestHint: npc netId %d:%d, quest_id %d, minLv %d, maxLv %d, sex %d, job %d, faction %d"),
		GetNetId().srvId, GetNetId().objId, quest_id, minLv, maxLv, sex, job, faction);
	QuestHint& hint = m_questHintMap[quest_id];
	hint.quest_id = quest_id;
	hint.minLv = minLv;
	hint.maxLv = maxLv;
	hint.sex = sex;
	hint.job = job;
	hint.faction = faction;
}

void CMapNPC::AddHintFlag(UInt16 quest_id, UInt16 flag_id, UInt16 value)
{
	TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("AddHintFlag: quest_id %d, flag_id %d, value %d"),
		quest_id, flag_id, value);
	QuestHintMap::iterator it = m_questHintMap.find(quest_id);
	if (it != m_questHintMap.end())
		it->second.AddFlag(flag_id, value);
}

void CMapNPC::LoadQuestHint()
{
	if (GetMap() == NULL || m_pData == NULL || m_pData->script == NULL || 
		m_pData->script[0] == 0)
		return;

	// read this npc lua script
	lua_State* pSubState = NULL;
	int refKey = 0;
	StringA filename(m_pData->script);
	StringA outFile;
	Boolean found = BOOLEAN_FALSE;
	Boolean loaded = BOOLEAN_FALSE;

	// create a new lua thread for preload scripts
	gGlobal.NewLuaThread(pSubState, gGlobal.g_pScriptState, refKey);
	m_questHintMap.clear();
	filename.Insert(0, SCRIPT_NPC_PATH);
	outFile = filename;
	outFile.Replace(LUA_SRC_EXT, LUA_BIN_EXT);
	if (IsFileExist(filename.c_str()) || IsFileExist(outFile.c_str())) {
		int p = filename.ReverseFind(_T('/'));
		StringA scriptFile = (p == -1 ? filename : filename.substr(p + 1));
		found = BOOLEAN_TRUE;
		loaded = gGlobal.LoadLua(pSubState, filename.c_str());
	}

	if (found && loaded) {					// load script success
		// set to global by _G.x
		lua_getglobal(pSubState, "_G");

		// set script global variable to nil
		lua_pushnil(pSubState);
		lua_setfield(pSubState, -2, "script");

		// set npc global variable
		LuaPush(pSubState, this);
		lua_setfield(pSubState, -2, "npc");

		lua_pop(pSubState, 1);			// pop "_G"

		// execute npc script
#ifdef LUA_STACK_TRACE_SERVER
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Lua status: CMapNPC::LoadQuestHint before resume file %s, state %d, top %d"), 
			String(filename).c_str(), lua_status(pSubState), lua_gettop(pSubState));
#endif //LUA_STACK_TRACE_SERVER
		int r = lua_resume(pSubState, 0);
#ifdef LUA_STACK_TRACE_SERVER
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Lua status: CMapNPC::LoadQuestHint after resume file %s, state %d, top %d"), 
			String(filename).c_str(), lua_status(pSubState), lua_gettop(pSubState));
#endif //LUA_STACK_TRACE_SERVER
		if (r == 0 && lua_gettop(pSubState) > 0)
		{
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("Lua status: CMapNPC::LoadQuestHint after resume get top not zero file %s, state %d, top %d"), 
				String(filename.c_str()).c_str(), lua_status(pSubState), lua_gettop(pSubState));
		}
		if (r != 0)
		{
			// script error
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("LoadQuestHint Warning: [script %s error], %s"),
				String(filename.c_str()).c_str(), String(lua_tostring(pSubState, -1)).c_str());
			lua_pop(pSubState, 1);
		}
	} else if (!found) {
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("LoadQuestHint Warning: [script %s not found]"),
			String(filename.c_str()).c_str());
	} else if (!loaded) {
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("LoadQuestHint Warning: [script %s not loaded], %s"),
			String(filename.c_str()).c_str(), String(lua_tostring(pSubState, -1)).c_str());
		lua_pop(pSubState, 1);
	}

	// release lua thread
	gGlobal.ReleaseLuaThread(pSubState, gGlobal.g_pScriptState, refKey);
}

void CMapNPC::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_DESTROY:				// destroy npc
		Destroy();
		break;

	case EVT_RELEASE:
		DeleteNetObject((CMapNPC*) this);
		break;

	case EVT_DISPLAY:
		DisPlayNpc();
		break;
	}
}

String CMapNPC::DebugInfo()
{
	String s(Parent::DebugInfo());
	s.AppendFormat(_T(", npc_uid %d, npc_id %d, lifeTime %d, npcid=(%d:%d)"), 
		GetNpc_uid(), GetNpc_id(), m_lifeTime, 
		GetNetId().srvId, GetNetId().objId);
	return s;
}

void CMapNPC::DisPlayNpc()
{
	if (m_pMap)
	{
		m_pMap->UpdateMapNpcVisbleMask(this,true);
	}
}