//-- Common
#include "Common.h"
//-- Self
#include "SimClient/ScriptSystem.h"
#include "SimClient/Global.h"
#include "SimClient/QuestSystem.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "Common/StateCode.h"
#include "Common/CallLuaFunc.h"

PROTO_LuaPush(CScriptSystem)
REGISTER_CLASSTYPE(CScriptSystem, ScriptSystem_CLASSID);

CScriptSystem::CScriptSystem()
{
	m_NPCShop = 0;
	m_bAskString = BOOLEAN_FALSE;
	m_bRequire = BOOLEAN_FALSE;
}

RPCResult CScriptSystem::CB_TalkEnd(LPCPEER pPeer, RPCContext &context, 
									const Boolean &bWait)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Chating with NPC ended."));


	if ((gGlobal.m_pSimClient == NULL)||(gGlobal.m_pSimClient->m_pQuestSys == NULL))
		return RPC_RESULT_OK;

	if (gGlobal.m_pSimClient->m_talkInProcess)
        gGlobal.m_pSimClient->m_talkInProcess = FALSE;

	if (gGlobal.m_pSimClient->m_pQuestSys->IsOutstandingQuestTask()&&gGlobal.m_pSimClient->m_bRobot)
	{
		CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CheckGetSubQuestEnd", String());
	}	


	StopTalk(pPeer, context, BOOLEAN_FALSE);

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_TalkNext(LPCPEER pPeer, RPCContext &context)
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pQuestSys == NULL)
		return RPC_RESULT_OK;

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_Choose(LPCPEER pPeer, RPCContext &context, 
								   const UInt8 &askerType, const UInt32 &askerId,
								   const UInt8 &total, const Boolean &waitOther,
								   const UInt16 &timeout, const StringVector &msgList)
{
	StringVector npcMsg = msgList;

	npcMsg.pop_back();
	if (askerType == 0)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("NPC %d Ask: "), askerId);
	}
	else if (askerType == 1)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("char_id %d Ask: "), askerId);
	}
	else
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("Item Ask: "));
	}
	
	int count = (int) npcMsg.size();
	int i = 0;
	for (StringVector::iterator iter = npcMsg.begin(); iter != npcMsg.end(); ++iter, ++i)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("The answer option %d:"), i);
	}
	if ((gGlobal.m_pSimClient == NULL)||(gGlobal.m_pSimClient->m_pQuestSys == NULL))
		return RPC_RESULT_OK;

	if (gGlobal.m_pSimClient->m_bRobot)
	{
		if (gGlobal.m_pSimClient->m_pQuestSys->IsOutstandingQuestTask())
		{
			int questID = gGlobal.m_pSimClient->m_pQuestSys->FindQuestIDByLocation();
			CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "NPCQuestTalkOption", String(), questID);
		}
		else
            CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "NPCTalkOption", String(), count);  
		              
	}

	if (gGlobal.m_pSimClient->m_talkStep == 0)
        gGlobal.m_pSimClient->m_talkInProcess = TRUE;

	++gGlobal.m_pSimClient->m_talkStep;
  
	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_Say(LPCPEER pPeer, RPCContext &context, const String &msg, const int &imageType, const Boolean &bLeft, const UInt32 &npc_id)
{

	if (gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_OK;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("NPC %d Say: "), npc_id);
	if (gGlobal.m_pSimClient->m_talkStep == 0)
        gGlobal.m_pSimClient->m_talkInProcess = TRUE;

	++gGlobal.m_pSimClient->m_talkStep;
    

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_OpenInterface(LPCPEER pPeer, RPCContext &context, const StringA &menu, 
						   const Int32 &param1)
{
	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_OpenShopMenu(LPCPEER pPeer, RPCContext &context, const UInt32 &shopId, 
						  const UInt8 &title, const UInt8 &enable, const UInt16 &buyDc, 
						  const UInt16 &sellDc, const UInt16 &repairDc, const UInt16 &uiShopType,
						  const UInt32Vector &itemLst)
{
    ///////// store the values related to the item being sold in NPC Shop//////////////////
	m_NPCShopItem.clear();	
	m_NPCShop = shopId;
	m_sellDiscount = sellDc;
	m_buyDiscount = buyDc;
	m_repairDiscount = repairDc;
	m_NPCShopItem = itemLst;

	if (gGlobal.m_resource == NULL)
		return RPC_RESULT_OK; 
	///////
	int count = 0;
	for (int i = 0; i < (int) m_NPCShopItem.size(); ++i)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Item %d, item_id %d"),
			i, m_NPCShopItem[i]);	
		++count;
	}
	
	gGlobal.m_pSimClient->m_talkInProcess = FALSE;	

	if (gGlobal.m_pSimClient->m_bRobot)
	{
		CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "NPCShopAction", String(), count);          
	
	}

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_ShowHint(LPCPEER pPeer, RPCContext &context, const String &hint)
{

	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));
	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_OnPlaySound(LPCPEER pPeer, RPCContext &context, const StringA &Name)
{
	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_AskGiven(LPCPEER pPeer, RPCContext &context, 
									 const String &msg, const UInt32 &npc_id, 
									 const UInt8Vector &gtypes, 
									 const UInt32Vector &gids)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("NPC %d Given gtype %s, gid %s"), 
		npc_id, GetVectorString(gtypes).c_str(), 
		GetVectorString(gids).c_str());

	m_givenTypes = gtypes;
	m_givenIds = gids;
	m_givenUIds.clear();
	m_givenCount = 0;

	if (!gtypes.empty()) {
		m_bRequire = BOOLEAN_TRUE;
		m_givenUIds.resize(gtypes.size(), 0);
	} else {
		m_bRequire = BOOLEAN_FALSE;
	}

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_GivenFinish(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CB_GivenFinish: gtype %s, gid %s, uid %s"), 
		GetVectorString(m_givenTypes).c_str(), GetVectorString(m_givenIds).c_str(), 
		GetVectorString(m_givenUIds).c_str());
	ClearGiven();

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_AskString(LPCPEER pPeer, RPCContext &context, 
									  const String &text, const String &defAns, 
									  const Boolean &bLeft, const UInt8 &askerType, 
									  const UInt32 &askerId)
{
	if (askerType == 0)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("NPC %d AskString: "), askerId);
	}
	else if (askerType == 1)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("char_id %d AskString: "), askerId);
	}
	else
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("Item AskString: "));
	}
	m_bAskString = BOOLEAN_TRUE;

	return RPC_RESULT_OK;
}

bool CScriptSystem::GetTalkStatus()
{
	return gGlobal.m_pSimClient->m_talkInProcess;
}

void CScriptSystem::ClearGiven()
{
	m_givenTypes.clear();
	m_givenIds.clear();
	m_givenUIds.clear();
	m_givenCount = 0;
}

void CScriptSystem::ResetGiven()
{
	for (UInt32Vector::iterator it = m_givenUIds.begin(); 
		it != m_givenUIds.end(); ++it)
	{
		*it = 0;
	}
}

Boolean CScriptSystem::AddGivenObject(UInt8 gtype, UInt32 gid, UInt32 uid)
{
	if (m_bRequire) {
		UInt16 count = (UInt16) m_givenTypes.size();
		bool found = false;
		UInt8 t_gtype;
		UInt32 t_gid;
		UInt32 t_uid;
		for (UInt16 i = 0 ; i < count; i++) {
			t_gtype = m_givenTypes[i];
			t_gid = m_givenIds[i];
			t_uid = m_givenUIds[i];
			if (t_gtype == gtype && t_gid == gid) {
				if (t_uid == 0) {
					m_givenUIds[i] = uid;
					m_givenCount ++;
					found = true;
					break;
				} else if (t_uid == uid) {
					TRACE_WARNDTL_2(GLOBAL_LOGGER, 
						_T("NpcGive Warning: duplicate object, gtype %d, uid %d"),
						gtype, uid);
					ResetGiven();
					return false;
				}
			}
		}
		if (!found) {
			TRACE_WARNDTL_2(GLOBAL_LOGGER, 
				_T("NpcGive Warning: object not required, gtype %d, uid %d"),
					gtype, uid);
			ResetGiven();
			return false;
		}
	} else {
		m_givenTypes.push_back(gtype);
		m_givenIds.push_back(gid);
		m_givenUIds.push_back(uid);
		m_givenCount ++;
	}
	return true;
}

Boolean CScriptSystem::ReplyGiven()
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pProxyPeer == NULL)
		return BOOLEAN_FALSE;

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ReplyGiven: gtype %s, gid %s, uid %s"), 
		GetVectorString(m_givenTypes).c_str(), GetVectorString(m_givenIds).c_str(), 
		GetVectorString(m_givenUIds).c_str());

	ScriptSystem_Proxy::ReplyGiven(gGlobal.m_pSimClient->m_pProxyPeer, 
		gGlobal.m_pSimClient->m_context, m_givenTypes, m_givenUIds);
	
	return BOOLEAN_TRUE;
}
