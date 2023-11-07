//-- Common
#include "Common.h"
#include "Common/CallLuaFunc.h"
#include "Common/PathFinder.h"
//-- Library
#include "MapService/Global.h"
#include "Resource/ResNPC.h"
#include "Resource/ResMob.h"
#include "Resource/ResItem.h"
#include "Resource/ResBuff.h"
#include "Resource/ServerResManager.h"
#include "QuestionControl.h"
#include "Proxy/Service/MessageService_proxy.h"

class CCharacter;

void HelloWorld()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Got Lua Call: Hello World"));
}

int GetGameTime(int mode)
{
	return gGlobal.GetGameTime(mode);
}

UInt32 GetServiceTime()
{
	return gGlobal.GetServiceTime();
}

void SetTempResult(double result)
{
	gGlobal.g_TempResult = result;
}

const ItemData * GetItemCsvData(UInt32 item_id)
{
	return gGlobal.GetItemCsvData(item_id);
}

double GetTempResult()
{
	return gGlobal.g_TempResult;
}

PCSTRINGA ConvertANSII(PCSTRINGA src)
{
	static char buf[1024];
	static TCHAR wbuf[512];
	int len;

	len = MultiByteToWideChar(CP_UTF8, 0, src, (int)strlen(src), wbuf, 512);
	len = WideCharToMultiByte(CP_ACP, 0, wbuf, len, buf, 1024, 0, 0);
	if (len > 0)
		buf[len] = '\0';
	else
		buf[0] = '\0';

	return buf;
}

PCSTRINGA ConvertUTF8(const String &src)
{
	static char buf[1024];
	StringA dst;
	StrToUTF8(src.c_str(), dst);
	strcpy(buf, dst.c_str());

	return buf;
}

const String& ConvertString(PCSTRINGA src)
{
	static String dst;
	UTF8ToStr(src, dst);

	return dst;
}

PCSTRINGA GetNpcName(UInt32 npc_id)
{
	const NpcData* npcData = gGlobal.m_resource->GetNpcData(npc_id);
	if (npcData) {
		String s;
		s.Format(_T("%s"), npcData->mid_name);
		return ConvertUTF8(s);
	} else {
		return "";
	}
}

PCSTRINGA GetMobName(UInt32 mob_id)
{
	const MobData* mobData = gGlobal.m_resource->GetMobData(mob_id);
	if (mobData) {
		String s;
		s.Format(_T("%s%s"), mobData->pre_name, mobData->mid_name);
		return ConvertUTF8(s);
	} else {
		return "";
	}
}

PCSTRINGA GetMapName(UInt16 map_id)
{
	const MapDB* mapData = gGlobal.m_resource->GetMapData(map_id);
	if (mapData) {
		String name(mapData->m_basicinfo->m_mapname);
		return ConvertUTF8(name);
	} else {
		return "";
	}
}

PCSTRINGA GetItemName(UInt32 item_id)
{
	const ItemData* itemData = gGlobal.m_resource->GetItemData(item_id);
	if (itemData) {
		String name(itemData->name);
		return ConvertUTF8(name);
	} else {
		return "";
	}
}

CMap* GetMap(UInt16 map_id)
{
	for (MapIndexList::iterator it = gGlobal.m_mapIndices.begin(); 
		it != gGlobal.m_mapIndices.end(); ++it)
	{
		if (it->first.map_id == map_id) {
			CMap* pMap = it->second;
			if (pMap)
				return pMap;
		}
	}

	return NULL;
}

const BuffData* GetBuffData(UInt16 buff_id)
{
	return gGlobal.m_resource->GetBuffData(buff_id);
}

UInt8 GetRealDay(UInt32 time)
{
	time_t now = time==0?::time(NULL):time;
	struct tm* ptime = localtime(&now);

	return ptime->tm_mday;
}

UInt8 GetRealWDay(UInt32 time)
{
	time_t now = time==0?::time(NULL):time;
	struct tm* ptime = localtime(&now);

	return ptime->tm_wday;
}

UInt8 GetRealMonth(UInt32 time)
{
	time_t now = time==0?::time(NULL):time;
	struct tm* ptime = localtime(&now);

	return ptime->tm_mon + 1;
}

UInt16 GetRealYear(UInt32 time)
{
	time_t now = time==0?::time(NULL):time;
	struct tm* ptime = localtime(&now);

	return ptime->tm_year + 1900;
}

UInt8 GetRealHour(UInt32 time)
{
	time_t now = time==0?::time(NULL):time;
	struct tm* ptime = localtime(&now);

	return ptime->tm_hour;
}

UInt8 GetRealMinute(UInt32 time)
{
	time_t now = time==0?::time(NULL):time;
	struct tm* ptime = localtime(&now);

	return ptime->tm_min;
}

UInt8 GetGuildWarStatus()
{
	return gGlobal.g_GuildWarState;
}

void QuestionPrepare()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Call Lua:QuestionPrepare"));
	CQuestionControl::GetInstance().Prepare();
}

void QuestionStart()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Call Lua:QuestionStart"));
	CQuestionControl::GetInstance().Start();
}

CWorldEvent* GetWorldEvent(UInt16 eventId)
{
	WorldEventMap::iterator it = gGlobal.m_worldEventMap.find(eventId);

	if (it != gGlobal.m_worldEventMap.end())
		return it->second;
	else
		return NULL;
}

void LogMessage(int level, PCSTRINGA msg)
{
	String s;
	UTF8ToStr(msg, s);
	
	TRACE_LOGDTL_1(GLOBAL_LOGGER, (Reuben::Diagnostics::Logger::LevelEnum) level, 
		_T("LogMessage: %s"), s.c_str());
}

void GuildWarPrepare()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Call Lua:GuildWarPrepare"));
	gGlobal.g_GuildWarState = GUILD_WAR_STATE_WAR_PREPARE;
}

void GuildWarStart()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Call Lua:GuildWarStart"));
	gGlobal.g_GuildWarState = GUILD_WAR_STATE_IN_WAR;
	gGlobal.StartGuildWar();
}

void GuildWarEndContinue()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Call Lua:GuildWarEndContinue"));
	gGlobal.g_GuildWarState = GUILD_WAR_STATE_WAR_FINISH_CONTINUE;
	MessageService_Proxy::GenerateGuildGroup(gGlobal.pMessageService->pPeer, RPCContext(), GUILD_WAR_STATE_WAR_FINISH_CONTINUE);
	gGlobal.EndGuildWar();
}

void GuildWarEndAll()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Call Lua:GuildWarEndAll"));
	gGlobal.g_GuildWarState = GUILD_WAR_STATE_WAR_FINISH_END;
	MessageService_Proxy::GenerateGuildGroup(gGlobal.pMessageService->pPeer, RPCContext(), GUILD_WAR_STATE_WAR_FINISH_END);
	gGlobal.EndGuildWar();
}

void GuildWarGenGroup()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Call Lua:GuildWarGenGroup"));
	MessageService_Proxy::GenerateGuildGroup(gGlobal.pMessageService->pPeer, RPCContext(), GUILD_WAR_STATE_BID_END);
}

void GuildWarSendGroupRumor()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Call Lua:GuildWarSendGroupRumor"));
	MessageService_Proxy::GenerateGuildGroup(gGlobal.pMessageService->pPeer, RPCContext(), GUILD_WAR_STATE_SEND_RUMOR_GROUP);
}


void IncognitoPKStart()
{
	gGlobal.StartIncognitoPK();
}

void IncognitoPKEnd()
{
	gGlobal.SetCharLstBattleOver();
}

void InIncognitoPkLstOne(UInt32 charID)
{
	gGlobal.AddCharInIncognitoPkLstOne(charID);
}

void InIncognitoPkLstTwo(UInt32 charID)
{
	gGlobal.AddCharInIncognitoPkLstTwo(charID);
}

bool isIncognitoPkTimeOver()
{
	return gGlobal.IncognitoPkIsTimeOver();
}

void RandomIncognitoPKBox(UInt16 map_id,UInt8 boxType)
{
	gGlobal.RandomIncognitoBox(map_id,boxType);
}
