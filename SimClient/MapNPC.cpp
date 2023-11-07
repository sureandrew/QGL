//-- Common
#include "Common.h"
//-- Self
#include "SimClient/MapNPC.h"
//-- Library
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CMapNPC, MapNPC_CLASSID);

void CMapNPC::ShowSummary(int i)
{
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("%3d: NPC %d: netId %d,%d, sex %d, cclass %d"),
		i, GetNpc_uid(), GetNetId().srvId, GetNetId().objId, GetSex(), GetCclass());
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("   : curPos %d,%d, dstPos %d,%d, action %d"),
		GetCurPosX(), GetCurPosY(), GetDstPosX(), GetDstPosY(), GetAction());
}

String CMapNPC::DebugInfo()
{
	String s(Parent::DebugInfo());
	s.AppendFormat(_T(", npc_id %d, template_id %d"), 
		GetNpc_uid(), GetNpc_id());
	return s;
}