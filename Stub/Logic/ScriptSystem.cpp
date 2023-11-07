//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/StateCode.h"
#include "Common/DataCode.h"
#include "Common/CallLuaFunc.h"
#include "Common/procedure.h"
//-- Self
#include "Stub/Logic/ScriptSystem.h"
//-- Library
#include "Proxy/Service/MessageService_proxy.h"
#include "Proxy/Service/WorldService_proxy.h"
#include "Stub/Logic/Map.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/Party.h"
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/Item.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/MapNPC.h"
#include "Stub/Logic/BatActor.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/PetGroup.h"
#include "MapService/Configure.h"
#include "MapService/Global.h"
#include "MapService/Instance.h"
#include "Resource/ResMob.h"
#include "Resource/ResNpc.h"
#include "Resource/ResShop.h"
#include "Resource/ResItem.h"
#include "Resource/ServerResManager.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

REGISTER_CLASSTYPE(CScriptSystem, ScriptSystem_CLASSID);

class CPartner;
class CPet;

PROTO_LuaPush(NetObject)
PROTO_LuaPush(CMapNPC)
PROTO_LuaPush(CScriptSystem)
PROTO_LuaPush(CBatActor)
PROTO_LuaPush(CCharacter)
PROTO_LuaPush(CParty)
PROTO_LuaPush(CPartner)
PROTO_LuaPush(CPet)
PROTO_LuaPush(CBattle)
PROTO_LuaPush(CItem)
PROTO_LuaPush(CInstance)

/////////////         function     //////////////////
CScriptSystem::CScriptSystem()
{
	m_scriptType = SCRIPT_TYPE_NONE;
	m_pOwner = NULL;
	m_pBattle = NULL;
	m_pUser = NULL;
	m_pTarget = NULL;
	m_pLuaState = NULL;
	m_refKey = 0;
	m_npcId = 0;
	m_itemNetId.objId = 0;
	m_npcNetId.objId = 0;
	m_answer = 0;
	m_answerTotal = 0;
	m_answerLevel = 0;
	m_bRunOnce = BOOLEAN_FALSE;
	m_bProcess = BOOLEAN_TRUE;
	m_bRunning = BOOLEAN_FALSE;
	m_bTalking = BOOLEAN_FALSE;
	m_bAsking = BOOLEAN_FALSE;
	m_bSaying = BOOLEAN_FALSE;
	m_bGiving = BOOLEAN_FALSE;
}

CScriptSystem::~CScriptSystem()
{
	m_pOwner = NULL;
	m_pBattle = NULL;
	ReleaseLua();
}

void CScriptSystem::StopScript()
{
	if (!m_bRunning || m_scriptFile.IsEmpty())
		return;

	if (m_pLuaState == NULL)
	{
		m_bRunning = BOOLEAN_FALSE;
		return;
	}

	// no asking question or saying anything to clients when stopping script
	m_bAsking = BOOLEAN_FALSE;
	m_bSaying = BOOLEAN_FALSE;

	if (m_pOwner && m_bGiving)
	{
		CB_GivenFinish(m_pOwner->GetPeer(), m_pOwner->GetContext());
		m_bGiving = BOOLEAN_FALSE;
	}

	if (m_question.bParty && m_pOwner && m_pOwner->GetParty() && m_pOwner->IsLeader())
	{
		// stop waiting answer if asking a party question
		CParty* pParty = m_pOwner->GetParty();
		CCharacter* member;
		CScriptSystem* pScript;
		for (UInt16 i = 0; i < pParty->GetCount(); i++)
		{
			if (pParty->GetM_bTempLeave(i))
				continue;
			member = pParty->GetChild<CCharacter>(i);
			pScript = member ? member->GetScript() : NULL;
			if (pScript->IsRunning())
			{
				if (i != 0)
					pScript->SetRunning(BOOLEAN_FALSE);
				if (pScript->IsTalking())
					pScript->TalkEnd(BOOLEAN_FALSE);
			}
		}
	}
	else if (m_bTalking)
	{
		// stop talking character state
		TalkEnd(BOOLEAN_FALSE);
	}
	CancelAllEvent();		// cancel all script events

	// run through script if in process status
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("file %s, state %d, top %d"), 
			String(m_scriptFile.c_str()).c_str(), lua_status(m_pLuaState), lua_gettop(m_pLuaState));

	SetLuaGlobal(BOOLEAN_TRUE);					// set global variable before resume
	int nRes = lua_status(m_pLuaState);
	String errMsg;
	int i = 0;
	while (nRes == LUA_YIELD)
	{
#ifdef LUA_STACK_TRACE_SERVER
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("sate %d, top %d before resume"), 
			lua_status(m_pLuaState), lua_gettop(m_pLuaState));
#endif //LUA_STACK_TRACE_SERVER
		nRes = lua_resume(m_pLuaState, 0);
#ifdef LUA_STACK_TRACE_SERVER
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("sate %d, top %d after resume"), 
			lua_status(m_pLuaState), lua_gettop(m_pLuaState));
#endif //LUA_STACK_TRACE_SERVER
		if (nRes == 0 && lua_gettop(m_pLuaState) > 0)
		{
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("top not zero, state %p, status %d, top %d"), 
				m_pLuaState, lua_status(m_pLuaState), lua_gettop(m_pLuaState));
		}
		if (nRes != 0 && nRes != LUA_YIELD)
		{
			errMsg = lua_tostring(m_pLuaState, -1);
			TRACE_ERRORDTL_5(GLOBAL_LOGGER, _FE("lua_resume failed, err %d, result %s, state %p, status %d, top %d"),
				nRes, errMsg.c_str(), m_pLuaState, lua_status(m_pLuaState), 
				lua_gettop(m_pLuaState));
			lua_pop(m_pLuaState, 1);
		}
		++i;
		if (i >= 100)		// prevent dead loop when stop script
			break;
	}

	m_scriptFile.Empty();
	ClearAnswer();

	ScriptReturn(nRes, errMsg);
}

Boolean CScriptSystem::InitLua()
{
	if (m_pLuaState)
		return BOOLEAN_TRUE;

	gGlobal.NewLuaThread(m_pLuaState, gGlobal.g_pScriptState, m_refKey);
	if (m_pLuaState == NULL) {
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CScriptSystem Error: fail to create lua thread state, char_id %d"),
			m_pOwner ? m_pOwner->GetChar_id() : -1);
		return BOOLEAN_FALSE;
	}

	SetRunning(BOOLEAN_FALSE);
	m_bProcess = BOOLEAN_TRUE;
	m_npcTable.Empty();
	m_itemTable.Empty();
	m_batTable.Empty();
	m_instTable.Empty();
	m_question.text.Empty();
	m_question.bLeft = BOOLEAN_TRUE;
	m_question.askerType = 0;
	m_question.askerId = 0;
	m_question.bParty = BOOLEAN_FALSE;
	m_bTalking = BOOLEAN_FALSE;
	m_bAsking = BOOLEAN_TRUE;
	m_bSaying = BOOLEAN_TRUE;
	m_bGiving = BOOLEAN_FALSE;
	m_jumpPos.map_id = 0;

	// clear all answer in party
	if (m_pOwner && m_pOwner->HasParty())
	{
		CParty* pParty = m_pOwner->GetParty();
		CCharacter* member;
		CScriptSystem* pScript;
		for (UInt16 i = 0; i < pParty->GetCount(); i++)
		{
			if (pParty->GetM_bTempLeave(i))
				continue;
			member = pParty->GetChild<CCharacter>(i);
			pScript = member ? member->GetScript() : NULL;
			if (pScript)
			{
				//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("clear answer, sesssionId %s, char_id %d"),
				//	member->GetContext().sessionId.GetString().c_str(),
				//	member->GetChar_id());
				pScript->ClearAnswer();
			}
		}
	}

	return BOOLEAN_TRUE;
}

void CScriptSystem::ClearAnswer()
{
	m_answer = 0;
	m_answerTotal = 0;
	m_answerLevel = 0;
	m_answerIndices.clear();
	m_answerList.clear();
}

void CScriptSystem::SetProcess(Boolean bProcess)
{
	m_bProcess = bProcess;
}

void CScriptSystem::SetRunning(Boolean bRunning)
{
	m_bRunning = bRunning;
}

void CScriptSystem::SetLuaGlobal(Boolean bSet)
{
	if (m_pLuaState == NULL) return;

	// set to global by _G.x
	lua_getglobal(m_pLuaState, "_G");

	if (lua_gettop(m_pLuaState) <= 0)			// can't get "_G" global table
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get _G table, state %p, top %d"),
			m_pLuaState, lua_gettop(m_pLuaState));
		return;
	}

	// set lua global variable "script" to this script system
	LuaPush(m_pLuaState, this);
	lua_setfield(m_pLuaState, -2, "script");

	// set lua global variable "battle" to battle object
	if (m_pBattle)
		LuaPush(m_pLuaState, m_pBattle);
	else
		lua_pushnil(m_pLuaState);
	lua_setfield(m_pLuaState, -2, "battle");

	// set lua global variable "player" to character object
	if (m_pOwner)
		LuaPush(m_pLuaState, m_pOwner);
	else
		lua_pushnil(m_pLuaState);
	lua_setfield(m_pLuaState, -2, "player");

	// set lua global variable "party" to character party object
	if (m_pOwner && m_pOwner->GetParty())
		LuaPush(m_pLuaState, m_pOwner->GetParty());
	else 
		lua_pushnil(m_pLuaState);
	lua_setfield(m_pLuaState, -2, "party");

	// set lua global variable "npc" to npc object
	CMapNPC *pNpc = NULL;
	if (m_npcNetId.IsValid())
		pNpc = FindNetObject<CMapNPC>(m_npcNetId);
	if (pNpc)
		LuaPush(m_pLuaState, pNpc);
	else
		lua_pushnil(m_pLuaState);
	lua_setfield(m_pLuaState, -2, "npc");

	// set lua global variable "item" to item object
	CItem *pItem = NULL;
	if (m_itemNetId.IsValid())
		pItem = FindNetObject<CItem>(m_itemNetId);
	if (pItem)
		LuaPush(m_pLuaState, pItem);
	else
		lua_pushnil(m_pLuaState);
	lua_setfield(m_pLuaState, -2, "item");

	// set lua global variable "user" to user object for item
	if (m_pUser)
		LuaPush(m_pLuaState, m_pUser);
	else
		lua_pushnil(m_pLuaState);
	lua_setfield(m_pLuaState, -2, "user");

	// set lua global variable "target" to target object for item
	if (m_pTarget)
		LuaPush(m_pLuaState, m_pTarget);
	else
		lua_pushnil(m_pLuaState);
	lua_setfield(m_pLuaState, -2, "target");

	lua_pop(m_pLuaState, 1);			// pop "_G"
}

void CScriptSystem::ReleaseLua()
{
	if (m_pLuaState == NULL)
		return;

	gGlobal.ReleaseLuaThread(m_pLuaState, gGlobal.g_pScriptState, m_refKey);
	m_pLuaState = NULL;
	m_scriptType = SCRIPT_TYPE_NONE;
	m_bRunning = BOOLEAN_FALSE;
	m_itemNetId.objId = 0;
	m_npcNetId.objId = 0;
	m_npcId = 0;
	m_pUser = NULL;
	m_pTarget = NULL;
	m_scriptFile.Empty();
	ClearAnswer();
}

void CScriptSystem::ExitScript(PCSTRINGA errEsg)
{
	if (m_pLuaState == NULL)
		return;

	if (errEsg)
	{
		StringA msg("");
		msg.Format("%s", errEsg);
		luaL_error(m_pLuaState, msg.c_str());
	}
	else
		luaL_error(m_pLuaState, "[Lua Stop]:Enforce Terminate\n");
}

void CScriptSystem::Say(PCSTRINGA msg, int imageType, bool bLeft, UInt32 npc_id, 
						Boolean bParty)
{
	if (m_pOwner == NULL) return;

	// stop owner when someone say something
	if (m_pOwner->IsState(CCharacter::ST_MOVE))
		m_pOwner->Stop();

	StringW s;
	UTF8ToStr(msg, s);

	if (imageType == 0 && npc_id == 0)
		npc_id = m_npcId;
	if (bParty && m_pOwner->GetParty())
	{
		CParty* pParty = m_pOwner->GetParty();
		for (UInt16 i = 0; i < pParty->GetCount(); i++)
		{
			if (pParty->GetM_bTempLeave(i))
				continue;
			CCharacter* member = pParty->GetMember(i);
			if (member)
				member->GetScript()->CB_Say(member->GetPeer(), member->GetContext(),
						s, imageType, bLeft, npc_id);
		}
	}
	else
	{
		m_pOwner->GetScript()->CB_Say(m_pOwner->GetPeer(), m_pOwner->GetContext(), s, imageType, 
				bLeft, npc_id);
	}
	m_bTalking = BOOLEAN_TRUE;
}

void CScriptSystem::Rumor(PCSTRINGA msg, bool bShowTitle)
{
	if (m_pOwner == NULL) return;

	if (m_pLuaState == NULL)
		return;

	StringW s;
	if (UTF8ToStr(msg, s) > 0)
		MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
			m_pOwner->GetContext(), s, m_pOwner->GetChar_id(), (bShowTitle ? 0 : 1));
}

void CScriptSystem::Rank(UInt8 rankType)
{
	if (m_pOwner)
		WorldService_Proxy::GetCharacterRank(gGlobal.pWorldService->pPeer, 
			m_pOwner->GetContext(), rankType);
}

void CScriptSystem::PlaySound(PCSTRINGA soundName)
{
	if (m_pOwner)
		CB_OnPlaySound(m_pOwner->GetPeer(), m_pOwner->GetContext(), StringA(soundName));
}

void CScriptSystem::ShowHint(PCSTRINGA msg)
{
	if (m_pOwner == NULL) return;

	StringW s;
	UTF8ToStr(msg, s);

	m_pOwner->GetScript()->CB_ShowHint(m_pOwner->GetPeer(), m_pOwner->GetContext(), s);
}

void CScriptSystem::ShowError(PCSTRINGA msg)
{
	if (m_pOwner == NULL) return;

	StringW s;
	UTF8ToStr(msg, s);

	m_pOwner->CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), 1, s);
}

Boolean CScriptSystem::ExecuteLua(StringA &filename)
{
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("ExecuteLua: system %d:%d:%d, state %d, script %s, char_id %d"),
		GetID(), GetNetId().srvId, GetNetId().objId, m_pLuaState,
		String(filename.c_str()).c_str(), m_pOwner ? m_pOwner->GetChar_id() : -1);

	String errMsg;
	do
	{
		if (m_pLuaState == NULL && !InitLua())
		{
			errMsg.Format(_T("Script Error: %s InitLua failed"), String(filename.c_str()).c_str());
			break;
		}
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ExecuteLua: status %d, top %d"),
			lua_status(m_pLuaState), lua_gettop(m_pLuaState));
		Boolean found = BOOLEAN_FALSE;
		Boolean loaded = BOOLEAN_FALSE;
		StringA outFile(filename);
		outFile.Replace(LUA_SRC_EXT, LUA_BIN_EXT);
		if (IsFileExist(filename.c_str()) || IsFileExist(outFile.c_str())) {
			int p = filename.ReverseFind(_T('/'));
			m_scriptFile = (p == -1 ? filename : filename.substr(p + 1));
			found = BOOLEAN_TRUE;
			loaded = gGlobal.LoadLua(m_pLuaState, filename.c_str());
		}
		if (!found)
			errMsg.Format(_T("Script Error: script %s not found"), String(filename.c_str()).c_str());
		else if (!loaded) {
			errMsg.Format(_T("Script Error: %s"), String(m_scriptFile.c_str()).c_str());
			
		}
	} while(false);

	if (!errMsg.IsEmpty() && m_pOwner) {
		TRACE_ERRORDTL(GLOBAL_LOGGER, errMsg.c_str());
		m_pOwner->CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), 1, errMsg);
		return BOOLEAN_FALSE;
	}

	return Resume(true, true);
}

void CScriptSystem::PrintErr()
{
	if (m_pOwner == NULL || m_pLuaState == NULL)
		return;

	const char* pErr = lua_tostring(m_pLuaState, -1);
	if (pErr)
	{
		StringW s;
		if (UTF8ToStr(pErr, s) > 0 ) 
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("CScriptSystem PrintErr: [Lua Warning] %s, owner_id %d"),
				s.c_str(), m_pOwner->GetChar_id());
		}	
	}
}

bool CScriptSystem::Resume(bool bReturn, bool start)
{
	if (m_pLuaState == NULL)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("lua state null"));
		return false;
	}

	if (!start && !m_bRunning)
	{
		StopScript();
		return false;
	}
	SetLuaGlobal(BOOLEAN_TRUE);					// set global variable before resume

	String errMsg;
	int nRes = lua_status(m_pLuaState);

#ifdef LUA_STACK_TRACE_SERVER
	TRACE_INFODTL_5(GLOBAL_LOGGER, 
		_F("before resume, char_id %d, state %p, status %d, top %d, script %s"),
		m_pOwner ? m_pOwner->GetChar_id() : -1, m_pLuaState, nRes, lua_gettop(m_pLuaState),
		String(m_scriptFile.c_str()).c_str());
#endif // LUA_STACK_TRACE_SERVER

	nRes = lua_resume(m_pLuaState, 0);

#ifdef LUA_STACK_TRACE_SERVER
	TRACE_INFODTL_5(GLOBAL_LOGGER, 
		_F("after resume, char_id %d, state %p, status %d, top %d, script %s"),
		m_pOwner ? m_pOwner->GetChar_id() : -1, m_pLuaState, nRes, 
		lua_gettop(m_pLuaState), String(m_scriptFile.c_str()).c_str());
#endif // LUA_STACK_TRACE_SERVER

	if (nRes == 0 && lua_gettop(m_pLuaState) > 0)
	{
		TRACE_WARNDTL_5(GLOBAL_LOGGER, 
			_FW("top not zero, char_id %d, state %p, status %d, top %d, script %s"),
			m_pOwner ? m_pOwner->GetChar_id() : -1, m_pLuaState, lua_status(m_pLuaState),
			lua_gettop(m_pLuaState), String(m_scriptFile.c_str()).c_str());
	}
	if (nRes != 0 && nRes != LUA_YIELD)
	{
		errMsg = GetLuaString(m_pLuaState);
		TRACE_WARNDTL_7(GLOBAL_LOGGER,
			_FW("lua_resume failed, char_id %d, err %d, result %s, state %p, status %d, top %d, script %s"),
			m_pOwner ? m_pOwner->GetChar_id() : -1, nRes, 
			errMsg.c_str(), m_pLuaState, 
			lua_status(m_pLuaState), lua_gettop(m_pLuaState), 
			String(m_scriptFile.c_str()).c_str());
		lua_pop(m_pLuaState, 1);
	}

	if (bReturn)
		return ScriptReturn(nRes, errMsg);
	else
		return (nRes == 0 || nRes == LUA_YIELD);
}

bool CScriptSystem::ScriptReturn(int r, CONST String& errMsg)
{
	if (m_pLuaState == NULL)
		return false;

	bool success = true;
	if (r == 0)
	{
		// script ended
		SetRunning(BOOLEAN_FALSE);
		if (m_pOwner && m_itemNetId.IsValid())
		{
			CItem* pItem = FindNetObject<CItem>(m_itemNetId);
			if (pItem && pItem->IsUpdated())
				m_pOwner->m_pBag->RaiseUpdate();
		}
		// tell client can finish npc giving process
		if (m_pOwner && m_bGiving)
		{
			CB_GivenFinish(m_pOwner->GetPeer(), m_pOwner->GetContext());
			m_bGiving = BOOLEAN_FALSE;
		}
		if (m_bRunOnce)
			ReleaseLua();
		if (m_pLuaState == NULL && m_pOwner)	// execute after script ended
		{
			// jump map
			if (m_jumpPos.map_id != 0)
			{
				m_pOwner->JumpMap(m_jumpPos.map_id, m_jumpPos.x, m_jumpPos.y, 
					m_jumpPos.line_id, m_jumpPos.owner_id, m_jumpPos.instId,m_jumpPos.bCheckLua);
				m_jumpPos.map_id = 0;
			}
		}
	}
	else if (r == LUA_YIELD)
	{
		// script continue
		SetRunning(BOOLEAN_TRUE);
	}
	else
	{
		// script error
		String msg;

		msg.Format(_T("Script Error: %s (%d), %s"),
			String(m_scriptFile.c_str()).c_str(), r, errMsg.c_str());

		TRACE_ERRORDTL(GLOBAL_LOGGER, msg.c_str());
		if (m_pOwner)
			m_pOwner->CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), r, msg);
		TalkEnd(BOOLEAN_FALSE);
		// tell client can finish npc giving process
		if (m_pOwner && m_bGiving)
		{
			CB_GivenFinish(m_pOwner->GetPeer(), m_pOwner->GetContext());
			m_bGiving = BOOLEAN_FALSE;
		}
		if (m_bRunOnce)
			ReleaseLua();
		success = false;
	}

	return success;
}

void CScriptSystem::TalkEnd(Boolean bWait)
{
	if (m_pOwner)
	{
		if (m_pBattle == NULL)
			CB_TalkEnd(m_pOwner->GetPeer(), m_pOwner->GetContext(), bWait);
		if (m_npcNetId.IsValid())
		{
			CMapNPC *pNpc = FindNetObject<CMapNPC>(m_npcNetId);
			if (pNpc) 
				pNpc->TalkEnd();
			if (m_pOwner->IsInMap())
				m_pOwner->ChangeState(CCharacter::ST_STOP);
		}
	}
	m_bTalking = BOOLEAN_FALSE;
}

bool CScriptSystem::GenerateNPC(UInt32 npc_id, UInt16 map_id, UInt16 x, UInt16 y, 
								bool enableAll, UInt32 lifeTime, UInt8 line_id, UInt32 owner_id,UInt8 dir)
{
	if (m_pOwner == NULL || m_pLuaState == NULL)
		return false;

	if (m_pOwner->GetMap())
	{
		CMap* pmap = m_pOwner->GetMap();
		UInt32 char_id = (enableAll ? 0 : m_pOwner->GetChar_id());

		if (map_id == 0)
			map_id = pmap->GetMap_id();
		if (line_id == 0 && owner_id == 0)
		{
			line_id = pmap->GetLine_id();
			owner_id = pmap->GetOwner_id();
		}

		pmap = gGlobal.GetMap(map_id, line_id, owner_id);
		if (pmap)
		{
			// local create npc
			CMapNPC * pNpc = pmap->CreateNpc(npc_id, x, y, dir, lifeTime, 0, char_id);
			if (pNpc)
			{
				pNpc->SetCreaterCharId(m_pOwner->GetChar_id());

				// set lua global again because CreateNpc may run other lua scripts
				SetLuaGlobal(BOOLEAN_TRUE);
				return true;
			}
		}
		else
		{
			// remote create npc
			WorldService_Proxy::RemoteCreateNpc(gGlobal.pWorldService->pPeer, RPCContext(),
				map_id, line_id, owner_id, npc_id, x, y, lifeTime, 0, char_id); 
		}
	}

	return false;
}

bool CScriptSystem::OpenInterface(PCSTRINGA menu, Int32 param1)
{
	if (m_pOwner ==	NULL || m_pLuaState == NULL || menu == NULL)
		return false;

	if (stricmp(menu, "shop") == 0) {
		UInt32 shop_id = (UInt32) param1;
		const ShopLstData* pShop = gGlobal.m_resource->GetShopInfo(shop_id);
		if (pShop) {
			UInt8 enable = 0;
			if (pShop->CanSell)
				enable |= 0x01;
			if (pShop->CanRepair)
				enable |= 0x02;
			CB_OpenShopMenu(m_pOwner->GetPeer(), m_pOwner->GetContext(), shop_id, 
				(UInt8) pShop->ShopName, enable, pShop->BuyDiscount, pShop->SellDiscount,
				pShop->RepairDiscount, pShop->uiShopType, pShop->ItemLst);

			 	StringA str;
			 	str.Format("shop_id  = %d, ShopName = %d, enable = %d,  BuyDiscount = %d, SellDiscount = %d, RepairDiscount = %d, uiShopType = %d, ItemLst.size() = %d. \n", 
					shop_id, (UInt8) pShop->ShopName, enable, pShop->BuyDiscount, pShop->SellDiscount,
					pShop->RepairDiscount, pShop->uiShopType, pShop->ItemLst.size());
			 	OutputDebugStringA(str.c_str());
		}
	} else {
		m_pOwner->m_tempSkillShopID = (UInt16)param1;
		CB_OpenInterface(m_pOwner->GetPeer(), m_pOwner->GetContext(), menu, param1);
	}

	return true;
}

bool CScriptSystem::OpenShopMenu(UInt32 shop_id)
{
	if (m_pOwner == NULL || m_pLuaState == NULL)
		return false;

	m_pOwner->Stop();

	CONST ShopLstData* pShop = gGlobal.m_resource->GetShopInfo(shop_id);
	if (pShop && pShop->ShopId == shop_id)
	{
		UInt8 title = (UInt8)(pShop->ShopName);
		UInt8 enable = 0;
		if (pShop->CanSell)
			enable |= 0x01;
		if (pShop->CanRepair)
			enable |= 0x02;

		UInt32Vector itemlist = pShop->ItemLst;
		if (itemlist.size() > 0)
		{
			CB_OpenShopMenu(m_pOwner->GetPeer(), m_pOwner->GetContext(), 
				shop_id, title, enable, pShop->BuyDiscount, pShop->SellDiscount, 
				pShop->RepairDiscount, pShop->uiShopType, itemlist); 
		}
	}

//	luaL_error(m_pLuaState, "OpenShopMenu");
	
	return false;
}

Boolean CScriptSystem::ExecuteNpcScript(CMapNPC* pNpc)
{
	if (pNpc == NULL)
		return BOOLEAN_FALSE;

	Boolean success = BOOLEAN_FALSE;

	TRACE_INFODTL_4(GLOBAL_LOGGER, 
		_T("ExecuteNpcScript: char_id %d, npc_id %d, npc_uid %d, map_id %d"),
		m_pOwner ? m_pOwner->GetChar_id() : -1, pNpc->GetNpc_id(), pNpc->GetNpc_uid(), 
		pNpc->GetMap_id());

	m_npcNetId.objId = 0;
	m_npcId = pNpc->GetNpc_id();
	if (pNpc->m_pData && pNpc->m_pData->script && 
		pNpc->m_pData->script[0] != _T('\0'))
	{
		m_scriptType = SCRIPT_TYPE_NPC;
		m_npcNetId = pNpc->GetNetId();
		StringA filename(pNpc->m_pData->script);
		filename.Insert(0, SCRIPT_NPC_PATH);
		success = ExecuteLua(filename);
		if (!success)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("talk end"));
			TalkEnd(BOOLEAN_FALSE);
		}
	}

	return success;
}

Boolean CScriptSystem::ExecuteItemScript(CItem *pItem, NetObject* pUser, 
										 NetObject* pTarget, PCSTRINGA method)
{
	if (pItem == NULL || pTarget == NULL)
		return BOOLEAN_FALSE;

	Boolean success = BOOLEAN_FALSE;
	StringA filename;
	m_itemNetId.objId = 0;
	m_pUser = NULL;
	m_pTarget = NULL;

	if (pItem->m_data && pItem->m_data->script && 
		pItem->m_data->script[0] != _T('\0'))
		filename = pItem->m_data->script;

	if (!filename.IsEmpty())
	{
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("ExecuteItemScript: method %s, owner_id %d, pItem %d, item_id %d"),
			String(method).c_str(), m_pOwner ? m_pOwner->GetChar_id() : -1, 
			pItem, pItem->GetItem_id());

		filename.Insert(0, SCRIPT_ITEM_PATH);
		success = ExecuteLua(filename);

		// new script system item method
		if (success)
		{
			m_scriptType = SCRIPT_TYPE_ITEM;
			m_itemNetId = pItem->GetNetId();
			m_pUser = pUser;
			m_pTarget = pTarget;
			success = CallItemMethod(method, pUser, pTarget);
		}
	}

	return success;
}

Boolean CScriptSystem::ExecuteBattleScript(UInt32 npc_id, const RPCNetID &netId)
{
	Boolean success = BOOLEAN_FALSE;
	m_npcId = npc_id;
	m_npcNetId = netId;
	const NpcData* pNpc = gGlobal.m_resource->GetNpcData(npc_id);
	if (pNpc && pNpc->battleScript && pNpc->battleScript[0] != _T('\0'))
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ExecuteBattleScript: npc_id %d, script %s, char_id %d"), 
			npc_id, pNpc->battleScript, m_pOwner ? m_pOwner->GetChar_id() : -1);
		StringA filename(pNpc->battleScript);
		filename.Insert(0, SCRIPT_BAT_PATH);
		success = ExecuteLua(filename);
		if (success)
			m_scriptType = SCRIPT_TYPE_BATTLE;
	}

	return success;
}

Boolean CScriptSystem::ExecuteInstScript(PCSTRINGA script, PCSTRINGA method, CInstance* pInst,
										 CCharacter* pChar)
{
	Boolean success = BOOLEAN_FALSE;
	if (HasInstMethod(method))
	{
		success = BOOLEAN_TRUE;
	}
	else
	{
		StringA filename;

		filename.Format("%s%s", SCRIPT_INST_PATH, script);
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("script %s, method %s, char_id %d"), 
			String(filename.c_str()).c_str(), String(method).c_str(),
			pChar ? pChar->GetChar_id() : (m_pOwner ? m_pOwner->GetChar_id() : -1));
		success = ExecuteLua(filename);
	}

	if (success)
	{
		m_scriptType = SCRIPT_TYPE_INST;
		success = CallInstMethod(method, pInst, pChar);
	}

	return success;
}

RPCResult CScriptSystem::ReplyAnswer(LPCPEER pPeer, RPCContext &context, 
									 const UInt8 &answer)
{
	CancelUniqEvent(EVT_ANS_TIMEOUT);
	ReplyAnswer(answer);
	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::ReplyAnswerString(LPCPEER pPeer, RPCContext &context, 
										   const String &answerStr)
{
	if (!m_bTalking)
		return RPC_RESULT_FAIL;

	if (!answerStr.IsEmpty())
		m_answerString = answerStr;
	m_bTalking = BOOLEAN_FALSE;			// for resume NPC pause before processing
	m_answerTotal = 0;

	// resume process after all answered
	Resume();

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::StopTalk(LPCPEER pPeer, RPCContext &context, 
								  const Boolean &bForce)
{
	if (!m_bRunning)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("failed, bRunning=FALSE"));
		return RPC_RESULT_FAIL;
	}

	if (bForce)
		StopScript();
	else
		Resume();
	
	return RPC_RESULT_OK;
}

void CScriptSystem::SetQuestion(PCSTRINGA text, bool bLeft, UInt8 askerType, 
								UInt32 askerId, Boolean bParty)
{
	UTF8ToStr(text, m_question.text);
	m_question.bLeft = bLeft;
	m_question.askerType = askerType;
	m_question.askerId = askerId;
	m_question.bParty = bParty;
	m_answerIndices.clear();
	m_answerTexts.clear();
	m_answerString.clear();
}

void CScriptSystem::ClearQuestion()
{
	m_question.askerType = 0;
	m_question.askerId = 0;
	m_question.bParty = BOOLEAN_FALSE;
}

void CScriptSystem::AddAnswer(UInt8 index, PCSTRINGA text)
{
	StringW s;
	UTF8ToStr(text, s);
	m_answerIndices.push_back(index);
	m_answerTexts.push_back(s);
	m_question.defAns = index;
}

void CScriptSystem::WaitAnswer(UInt8 defAns, UInt16 timeout)
{
	if (m_pOwner == NULL)
		return;

	// stop owner when need to answer question
	if (m_pOwner->IsState(CCharacter::ST_MOVE))
		m_pOwner->Stop();

	if (defAns != 0)
		m_question.defAns = defAns;
	m_answerTotal = (UInt8) m_answerTexts.size();
	Boolean waitOther = (m_question.bParty && m_pOwner->HasParty() && 
						m_pOwner->GetParty()->GetCount() > 1 && m_pOwner->IsLeader());
	m_answerTexts.push_back(m_question.text);
	if (m_pBattle) {
		m_pBattle->m_talkNum ++;
		m_pBattle->CB_Choose(m_pOwner->GetPeer(), m_pOwner->GetContext(), 
			m_question.askerType, m_question.askerId, m_question.bLeft, 
			m_answerTotal, m_answerTexts);
	} else {
		CB_Choose(m_pOwner->GetPeer(), m_pOwner->GetContext(), m_question.askerType, 
			m_question.askerId, m_answerTotal, waitOther, timeout, m_answerTexts);
	}

	if (waitOther)
	{
		// send question to other members if it is really a party question
		CParty* pParty = m_pOwner->GetParty();
		CCharacter* member;
		CScriptSystem* pScript;
		for (UInt16 i = 1; i < pParty->GetCount(); i++) 
		{
			if (pParty->GetM_bTempLeave(i))
				continue;
			member = pParty->GetChild<CCharacter>(i);
			pScript = member ? member->GetScript() : NULL;
			if (pScript) {
				// stop member script if running
				if (pScript->m_pLuaState && pScript->IsRunning())
					pScript->StopScript();
				
				pScript->SetRunning(BOOLEAN_TRUE);
				pScript->MemberWaitAnswer(m_question, m_answerTexts, 
					m_question.defAns, timeout);
			}
		}
	} 
	else if (m_question.bParty)
	{
		// reset party question if really not
		m_question.bParty = BOOLEAN_FALSE;
	}

	m_answerTexts.pop_back();
	// set default answer
	m_answer = m_question.defAns;
	m_answerList.push_back(m_question.defAns);
	m_bTalking = BOOLEAN_TRUE;

	// trigger timeout event
	if (timeout > 0)
		RaiseUniqEventID(EVT_ANS_TIMEOUT, (UInt32) timeout * 1000);
}

void CScriptSystem::MemberWaitAnswer(QuestInfo& question, 
									 StringVector &answerTexts, UInt8 defAns, 
									 UInt16 timeout)
{
	if (m_pOwner == NULL || answerTexts.size() < 2)
		return;

	// at least default timeout for member to answer
	UInt8 total = (UInt8) answerTexts.size() - 1;
	if (m_pBattle) {
		m_pBattle->m_talkNum ++;
		m_pBattle->CB_Choose(m_pOwner->GetPeer(), m_pOwner->GetContext(), 
			question.askerType, question.askerId, question.bLeft, total, answerTexts);
	} else {
		CB_Choose(m_pOwner->GetPeer(), m_pOwner->GetContext(), question.askerType, 
			question.askerId, total, BOOLEAN_TRUE, timeout, answerTexts);
	}
	// set default answer
	m_answerList.push_back(defAns);
	m_answer = defAns;
	m_bTalking = BOOLEAN_TRUE;
	m_bAsking = BOOLEAN_FALSE;
	m_question.defAns = defAns;
	m_question.bParty = BOOLEAN_TRUE;

	// trigger timeout event
	if (timeout > 0)
		RaiseUniqEventID(EVT_ANS_TIMEOUT, (UInt32) timeout * 1000);
}

void CScriptSystem::ReplyAnswer(UInt8 ansIndex)
{
	if (!m_bTalking)
		return;

	CCharacter* leader = NULL;
	if (m_question.bParty && m_pOwner && m_pOwner->GetParty())
		leader = m_pOwner->GetParty()->GetLeader();

	if (ansIndex == 255)						// use default answer
		m_answer = m_question.defAns;
	else if (leader && leader->GetScript())		// use leader reverse answer map
		m_answer = leader->GetScript()->ReverseAnswer(ansIndex);
	else 
		m_answer = ReverseAnswer(ansIndex);		// use own reverse answer map
	if (!m_answerList.empty())
		m_answerList.back() = m_answer;
	m_bTalking = BOOLEAN_FALSE;			// for resume NPC pause before processing
	m_answerTotal = 0;

	// notify main script character for someone answered
	if (leader && leader->GetScript())
		leader->GetScript()->NotifyAnswer();
	else
		NotifyAnswer();
}

void CScriptSystem::NotifyAnswer()
{
	// check if all member has answered for a party question
	if (m_question.bParty && m_pOwner && m_pOwner->GetParty()) 
	{
		CParty* pParty = m_pOwner->GetParty();
		CCharacter* member;
		CScriptSystem* pScript;
		for (UInt16 i = 0; i < pParty->GetCount(); i++) 
		{
			if (pParty->GetM_bTempLeave(i))
				continue;
			member = pParty->GetChild<CCharacter>(i);
			pScript = member ? member->GetScript() : NULL;
			if (pScript && pScript->IsRunning() && pScript->IsTalking())
				return;
		}
		// talk next page for all
		for (UInt16 i = 0; i < pParty->GetCount(); i++) 
		{
			if (pParty->GetM_bTempLeave(i))
				continue;
			member = pParty->GetChild<CCharacter>(i);
			pScript = member ? member->GetScript() : NULL;
			if (pScript && pScript->IsRunning())
			{
				pScript->ClearQuestion();
				pScript->CB_TalkNext(member->GetPeer(), member->GetContext());
				if (i != 0)		// stop member script running state
					pScript->SetRunning(BOOLEAN_FALSE);
			}
		}
	} 
	else 
	{
		// talk next page for owner
		m_pOwner->GetScript()->CB_TalkNext(m_pOwner->GetPeer(), m_pOwner->GetContext());
	}

	// resume process after all answered
	Resume();
}

void CScriptSystem::NextAnswer(Boolean bParty)
{
	m_answerLevel++;

	// set next answer of whole party members
	if (bParty && m_pOwner && m_pOwner->GetParty()) {
		CParty* pParty = m_pOwner->GetParty();
		CCharacter* member;
		CScriptSystem* pScript;
		for (UInt16 i = 1; i < pParty->GetCount(); i++) {
			if (pParty->GetM_bTempLeave(i))
				continue;
			member = pParty->GetMember(i);
			pScript = member ? member->GetScript() : NULL;
			if (pScript && pScript->IsRunning())
				pScript->NextAnswer(BOOLEAN_FALSE);
		}
	}
}

void CScriptSystem::BackAnswer(UInt8 level)
{
	level++;				// decrease 1 for current level 
	if (m_bAsking) {
		while (level > 0) {
			if (m_answerList.empty())
				break;
			m_answerList.pop_back();
			level--;
		}
	} else {
		m_answerLevel -= level;
	}
}

UInt8 CScriptSystem::ReverseAnswer(UInt8 ansIndex)
{
	return ((ansIndex < (UInt8) m_answerIndices.size()) ? 
			m_answerIndices[ansIndex] : m_question.defAns);
}

UInt8 CScriptSystem::GetAnswer(UInt8 index)
{
	//if (!m_pBattle && !m_bRunning)			// return 0 when character not joined before
	//	return 0;

	// get answer from other member
	if (m_pOwner && index > 0) {
		CParty* pParty = m_pOwner->GetParty();
		CCharacter* member;
		CScriptSystem* pScript;
		if (pParty && index < pParty->GetRealMemNum())
		{
			member = pParty->GetInPartyMember(index);
			pScript = member ? member->GetScript() : NULL;
			if (pScript)
				return pScript->GetOwnAnswer();
		}
	}
	return GetOwnAnswer();
}
		
UInt8 CScriptSystem::GetOwnAnswer()
{
	if (!m_bAsking && m_answerLevel > 0 && m_answerLevel <= m_answerList.size()) {
		return m_answerList[m_answerLevel - 1];
	} else {
		return m_answer;
	}
}

CItem* CScriptSystem::GetUseItem()
{
	if (m_itemNetId.IsValid())
		return FindNetObject<CItem>(m_itemNetId);
	else
		return NULL;
}

void CScriptSystem::GetMapLines(UInt16 map_id)
{
	// must called by "GetMapLines()" function in QGL_lib.lua
	if (m_pOwner && m_pLuaState && m_bRunning)
	{
		PROCEDURE_SET_FRONTLOCK		// set high priority for doing procedure for script
		PROCEDURE_DO_METHOD1(CScriptSystem, GetMapLines, this, m_pOwner->GetContext(),
			UInt16, map_id)
	}
}

RPCResult CScriptSystem::GetMapLines(RPCContext &context)
{
	if (m_pLuaState == NULL || m_pOwner == NULL)
	{
		// check script is already stopped or no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD1(CScriptSystem, GetMapLines, this, context, UInt16, map_id)

		TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("GetMapLines: char_id %d, sessionId %s, map_id %d"),
			m_pOwner->GetChar_id(), context.sessionId.GetString().c_str(), map_id);
		WorldService_Proxy::GetMapLines(gGlobal.pWorldService->pPeer, context, map_id);
		PROCEDURE_SET_TIMEOUT(10000, BOOLEAN_TRUE)

	PROCEDURE_WAIT2(1, WorldService, CB_GetMapLines, Int32Vector, lineCounts, 
					UInt16Vector, lineState)

		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, 
			_T("GetMapLines(1): char_id %d, sessionId %s, map_id %d, m_bRunning %d"),
			m_pOwner->GetChar_id(), context.sessionId.GetString().c_str(), map_id,
			m_bRunning);

		// push lineState list into local "lines" table variable
		PushIntArray(lineState, 2);
		// push lineCounts list into local "counts" table variable
		PushIntArray(lineCounts, 3);
		Resume();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT
			
			String errMsg = _T("GetMapLines Error");
			ScriptReturn(2, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}


void CScriptSystem::GetMessengers(UInt32 char_id,UInt16 relation)
{
	// must called by "GetMessengers()" function in QGL_lib.lua
	if (m_pOwner && m_pLuaState && m_bRunning)
	{
		PROCEDURE_SET_FRONTLOCK		// set high priority for doing procedure for script
		PROCEDURE_DO_METHOD2(CScriptSystem, GetMessengers, this, m_pOwner->GetContext(),
			UInt32, char_id, UInt16, relation)
	}
}

RPCResult CScriptSystem::GetMessengers(RPCContext &context)
{
	if (m_pLuaState == NULL || m_pOwner == NULL)
	{
		// check script is already stopped or no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD2(CScriptSystem, GetMessengers, this, context, 
		UInt32, char_id, UInt16, relation)

		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, 
			_T("GetMessengers: char_id %d, sessionId %s, target_id %d, relation %d"),
			m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), char_id, 
			relation);
		MessageService_Proxy::GetMessengers(gGlobal.pMessageService->pPeer,context,
			char_id, relation);
		PROCEDURE_SET_TIMEOUT(10000, BOOLEAN_TRUE)

	PROCEDURE_WAIT2(1, MessageService, CB_GetMessengers, UInt32, owner_id, UInt32Vector, messagers)

		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, 
			_T("GetMessengers(1): char_id %d, sessionId %s, target_id %d, m_bRunning %d"),
			m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), char_id,
			m_bRunning);

		// push messagers list into local "messengers" table variable
		PushIntArray(messagers, 3);
		Resume();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT
			String errMsg = _T("GetMessengers Error");
			ScriptReturn(2, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CScriptSystem::GetMessengersName(UInt32 char_id, UInt16 relation)
{
	// must called by "GetMessengersName()" function in QGL_lib.lua
	if (m_pOwner && m_pLuaState && m_bRunning)
	{
		PROCEDURE_SET_FRONTLOCK		// set high priority for doing procedure for script
		PROCEDURE_DO_METHOD2(CScriptSystem, GetMessengersName, this, 
			m_pOwner->GetContext(), UInt32, char_id,UInt16, relation)
	}
}

RPCResult CScriptSystem::GetMessengersName(RPCContext &context)
{
	if (m_pLuaState == NULL || m_pOwner == NULL)
	{
		// check script is already stopped or no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD2(CScriptSystem, GetMessengersName, this, context, UInt32, char_id,UInt16, relation)
		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, 
			_T("GetMessengersName: char_id %d, sessionId %s, target_id %d, relation %d"),
			m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), char_id, 
			relation);
		MessageService_Proxy::GetMessengersName(gGlobal.pMessageService->pPeer, context, 
			char_id, relation);

		PROCEDURE_SET_TIMEOUT(10000, BOOLEAN_TRUE)

	PROCEDURE_WAIT2(1, MessageService, CB_GetMessengersName, UInt32, owner_id,
			StringVector, messagers)

		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, 
			_T("GetMessengersName(1): char_id %d, sessionId %s, target_id %d, m_bRunning %d"),
			m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), char_id,
			m_bRunning);

		// push messagers list into local "messengers" table variable
		PushStringArray(messagers, 3);
		Resume();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT
			String errMsg = _T("GetMessengersName Error");
			ScriptReturn(2, errMsg);

	PROCEDURE_END_CATCH

			return RPC_RESULT_OK;
}

void CScriptSystem::GetFriendly(UInt32 char_id1,UInt32 char_id2)
{
	// must called by "GetFriendly()" function in QGL_lib.lua
	if (m_pOwner && m_pLuaState && m_bRunning)
	{
		PROCEDURE_SET_FRONTLOCK		// set high priority for doing procedure for script
		PROCEDURE_DO_METHOD2(CScriptSystem, GetFriendly, this, m_pOwner->GetContext(), UInt32, char_id1,UInt32, char_id2)
	}
}

RPCResult CScriptSystem::GetFriendly(RPCContext &context)
{
	if (m_pLuaState == NULL || m_pOwner == NULL)
	{
		// check script is already stopped or no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD2(CScriptSystem, GetFriendly, this, context, UInt32, char_id1,
		UInt32, char_id2)

		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, 
			_F("char_id %d, sessionId %s, char_id1 %d, char_id2 %d"),
			m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), char_id1, 
			char_id2);
		MessageService_Proxy::GetFriendly(gGlobal.pMessageService->pPeer,context, 
			char_id1, char_id2);

	PROCEDURE_WAIT1(1, MessageService, CB_GetFriendly, UInt32, friendly)

		// push friendly into local "friendly" variable
		PushLocalInt(friendly, 3);
		Resume();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT
			String errMsg = _T("GetFriendly Error");
			ScriptReturn(2, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CScriptSystem::ChangeFriendly(UInt32 char_id1,UInt32 char_id2,Int32 changed)
{
	// must called by "GetFriendly()" function in QGL_lib.lua
	if (m_pOwner && m_pLuaState && m_bRunning)
	{
		PROCEDURE_SET_FRONTLOCK		// set high priority for doing procedure for script
		PROCEDURE_DO_METHOD3(CScriptSystem, ChangeFriendly, this, m_pOwner->GetContext(), UInt32, char_id1,UInt32, char_id2, Int32, changed)
	}
}

RPCResult CScriptSystem::ChangeFriendly(RPCContext &context)
{
	if (m_pLuaState == NULL || m_pOwner == NULL)
	{
		// check script is already stopped or no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD3(CScriptSystem, ChangeFriendly, this, context, UInt32, char_id1,
		UInt32, char_id2, Int32, changed)

		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, 
		_F("char_id %d, sessionId %s, char_id1 %d, char_id2 %d"),
		m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), char_id1, 
		char_id2);
	MessageService_Proxy::ChangeFriendly(gGlobal.pMessageService->pPeer,context, 
		char_id1, char_id2, changed);

	PROCEDURE_WAIT(1, MessageService, CB_ChangeFriendly);

	Resume();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT;
		String errMsg = _F("Error");
		ScriptReturn(2, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CScriptSystem::IsFriend(UInt32 char_id1,UInt32 char_id2)
{
	// must called by "IsFriend()" function in QGL_lib.lua
	if (m_pOwner && m_pLuaState && m_bRunning)
	{
		PROCEDURE_SET_FRONTLOCK		// set high priority for doing procedure for script
		PROCEDURE_DO_METHOD2(CScriptSystem, IsFriend, this, m_pOwner->GetContext(), UInt32, char_id1,UInt32, char_id2)
	}
}
RPCResult CScriptSystem::IsFriend(RPCContext &context)
{
	if (m_pLuaState == NULL || m_pOwner == NULL)
	{
		// check script is already stopped or no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD2(CScriptSystem, IsFriend, this, context, UInt32, char_id1,
		UInt32, char_id2)

	TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, 
	_T("IsFriend: char_id %d, sessionId %s, char_id1 %d, char_id2 %d"),
	m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), char_id1, 
	char_id2);

	MessageService_Proxy::IsFriend(gGlobal.pMessageService->pPeer,context, 
		char_id1, char_id2);

	PROCEDURE_WAIT1(1, MessageService, CB_IsFriend, UInt32, isFriend)

	// push isFriend into local "isFriend" variable
	PushLocalInt(isFriend, 3);
	Resume();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT
		String errMsg = _T("IsFriend Error");
		ScriptReturn(2, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CScriptSystem::HasGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id)
{
	// must called by "HasGlobalNpcFlag()" function in QGL_lib.lua
	if (m_pOwner && m_pLuaState && m_bRunning)
	{
		PROCEDURE_SET_FRONTLOCK		// set high priority for doing procedure for script
			PROCEDURE_DO_METHOD3(CScriptSystem, HasGlobalNpcFlag, this, m_pOwner->GetContext(), UInt32, npc_id,UInt32, owner_id,UInt16, flag_id);
	}
}
RPCResult CScriptSystem::HasGlobalNpcFlag(RPCContext &context)
{
	if (m_pLuaState == NULL || m_pOwner == NULL)
	{
		// check script is already stopped or no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD3(CScriptSystem, HasGlobalNpcFlag, this, context, UInt32, npc_id,
		UInt32, owner_id,UInt16, flag_id)

		TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, 
		_F("char_id %d, sessionId %s, npc_id %d"),
		m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), npc_id);

	WorldService_Proxy::HasGlobalNpcFlag(gGlobal.pWorldService->pPeer,context, 
		npc_id, owner_id,flag_id);

	PROCEDURE_WAIT1(1, WorldService, CB_HasGlobalNpcFlag, Boolean, hasFlag)

	// push isFriend into local "isFriend" variable
	PushLocalInt(hasFlag, 4);
	Resume();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT
		String errMsg = _F("Error");
		ScriptReturn(2, errMsg);

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}
void CScriptSystem::GetGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id)
{
	// must called by "GetGlobalNpcFlag()" function in QGL_lib.lua
	if (m_pOwner && m_pLuaState && m_bRunning)
	{
		PROCEDURE_SET_FRONTLOCK		// set high priority for doing procedure for script
			PROCEDURE_DO_METHOD3(CScriptSystem, GetGlobalNpcFlag, this, m_pOwner->GetContext(), UInt32, npc_id,UInt32, owner_id,UInt16, flag_id);
	}
}
RPCResult CScriptSystem::GetGlobalNpcFlag(RPCContext &context)
{
	if (m_pLuaState == NULL || m_pOwner == NULL)
	{
		// check script is already stopped or no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD3(CScriptSystem, GetGlobalNpcFlag, this, context, UInt32, npc_id,
		UInt32, owner_id,UInt16, flag_id)

		TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, 
		_F("char_id %d, sessionId %s, npc_id %d"),
		m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), npc_id);

	WorldService_Proxy::GetGlobalNpcFlag(gGlobal.pWorldService->pPeer,context, 
		npc_id, owner_id,flag_id);

	PROCEDURE_WAIT1(1, WorldService, CB_GetGlobalNpcFlag, UInt32, flag)

		// push isFriend into local "isFriend" variable
		PushLocalInt(flag, 4);
	Resume();

	PROCEDURE_CATCH

	PROCEDURE_EXCEPTION_DEFAULT
		String errMsg = _F("Error");
		ScriptReturn(2, errMsg);

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}
void CScriptSystem::SetGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id, UInt16 flag)
{
	// must called by "SetGlobalNpcFlag()" function in QGL_lib.lua
	if (m_pOwner && m_pLuaState && m_bRunning)
	{
		PROCEDURE_SET_FRONTLOCK		// set high priority for doing procedure for script
			PROCEDURE_DO_METHOD4(CScriptSystem, SetGlobalNpcFlag, this, m_pOwner->GetContext(), UInt32, npc_id,UInt32, owner_id,UInt16, flag_id, UInt16, flag);
	}
}
RPCResult CScriptSystem::SetGlobalNpcFlag(RPCContext &context)
{
	if (m_pLuaState == NULL || m_pOwner == NULL)
	{
		// check script is already stopped or no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD4(CScriptSystem, SetGlobalNpcFlag, this, context, UInt32, npc_id,
		UInt32, owner_id,UInt16, flag_id,UInt16, flag)

		TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, 
		_F("char_id %d, sessionId %s, npc_id %d"),
		m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), npc_id);

	WorldService_Proxy::SetGlobalNpcFlag(gGlobal.pWorldService->pPeer,context, 
		npc_id, owner_id,flag_id,flag);

	PROCEDURE_WAIT(1, WorldService, CB_SetGlobalNpcFlag)

	Resume();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT
		String errMsg = _F("Error");
		ScriptReturn(2, errMsg);

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}
void CScriptSystem::RemoveGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id)
{
	// must called by "RemoveGlobalNpcFlag()" function in QGL_lib.lua
	if (m_pOwner && m_pLuaState && m_bRunning)
	{
		PROCEDURE_SET_FRONTLOCK		// set high priority for doing procedure for script
			PROCEDURE_DO_METHOD3(CScriptSystem, RemoveGlobalNpcFlag, this, m_pOwner->GetContext(), UInt32, npc_id,UInt32, owner_id,UInt16, flag_id);
	}
}
RPCResult CScriptSystem::RemoveGlobalNpcFlag(RPCContext &context)
{
	if (m_pLuaState == NULL || m_pOwner == NULL)
	{
		// check script is already stopped or no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD3(CScriptSystem, RemoveGlobalNpcFlag, this, context, UInt32, npc_id,
		UInt32, owner_id,UInt16, flag_id)

		TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, 
		_F("char_id %d, sessionId %s, npc_id %d"),
		m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), npc_id);

	WorldService_Proxy::RemoveGlobalNpcFlag(gGlobal.pWorldService->pPeer,context, 
		npc_id, owner_id,flag_id);

	PROCEDURE_WAIT(1, WorldService, CB_RemoveGlobalNpcFlag)
	Resume();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT
		String errMsg = _F("Error");
	ScriptReturn(2, errMsg);

	PROCEDURE_END_CATCH
		return RPC_RESULT_OK;
}

void CScriptSystem::CreatMasterAndStudent(UInt32 master_id, UInt32 student_id)
{
	if (m_pOwner == NULL)
		return;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("master_id %d, student_id %d"), master_id, student_id);
	CCharacter* pMaster = gGlobal.m_charToSession.FindCharacter(master_id);
	CCharacter* pStudent = gGlobal.m_charToSession.FindCharacter(student_id);
	if( pMaster == NULL || pStudent == NULL )
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER,
			_FE("[master or student not found], master_id %d, student_id %d"),
			master_id, student_id);
		return;
	}

	UInt16Vector relations;
	relations.push_back(FRIEND_RELATION_FRIEND);
	relations.push_back(FRIEND_RELATION_MASTER);
	MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), student_id, master_id,
		relations, FRIEND_GROUP_ONE, 0, pMaster->GetNickName());

	relations.clear();
	relations.push_back(FRIEND_RELATION_FRIEND);
	relations.push_back(FRIEND_RELATION_STUDENT);
	MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), master_id, student_id,
		relations, FRIEND_GROUP_ONE, 0, pStudent->GetNickName());
}

void CScriptSystem::DestroyMasterAndStudent(UInt32 master_id,UInt32 student_id)
{
	if (m_pOwner == NULL)
		return;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("master_id %d, student_id %d"), master_id, student_id);
	MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), student_id, master_id,FRIEND_RELATION_MASTER);
	MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), master_id, student_id,FRIEND_RELATION_STUDENT);
}

Boolean CScriptSystem::CanGiveObject(GivenType* pGiven)
{
	if (pGiven == NULL)
		return false;

	Boolean found = false;
	switch(pGiven->gtype)
	{
	case GIVEN_ITEM:
		found = (m_pOwner->GetBag()->GetItemCount(pGiven->gid) > 0);
		break;

	case GIVEN_PARTNER:
		found = (m_pOwner->GetPartners()->GetPartnerCount(pGiven->gid) > 0);
		break;

	case GIVEN_PET:
		found = (m_pOwner->GetPets()->GetIdCount(pGiven->gid) > 0);
		break;
	}

	return found;
}

Boolean CScriptSystem::HaveGivenObject(GivenType* pGiven)
{
	if (pGiven == NULL || pGiven->gtype == 0 || pGiven->uid == 0)
		return false;

	Boolean found = false;
	switch(pGiven->gtype)
	{
	case GIVEN_ITEM:
		{
			CItem* pItem = m_pOwner->GetBag()->GetItemByUID(pGiven->uid);
			if (pItem && pItem->IsExist()) {
				pGiven->gid = pItem->GetItem_id();
				found = true;
			}
		}
		break;

	case GIVEN_PARTNER:
		{
			CPartner* pPartner = m_pOwner->GetPartners()->GetPartnerByID(pGiven->uid);
			if (pPartner) {
				pGiven->gid = pPartner->GetMob_id();
				found = true;
			}
		}
		break;

	case GIVEN_PET:
		{
			CPet* pPet = m_pOwner->GetPets()->GetPet(pGiven->uid);
			if (pPet) {
				pGiven->gid = pPet->GetPet_id();
				found = true;
			}
		}
		break;
	}

	return found;
}

Boolean CScriptSystem::AskGiven(PCSTRINGA text, UInt32 npc_id)
{
	if (m_pOwner == NULL)
		return BOOLEAN_FALSE;

	StringW s;
	UInt8Vector gtypes;
	UInt32Vector gids;

	if (npc_id == 0)
		npc_id = m_npcId;
	UTF8ToStr(text, s);
	m_bGiving = BOOLEAN_FALSE;

	// check if player have all asked object
	for (GivenList::iterator it = m_givenList.begin(); it != m_givenList.end(); ++it)
	{
		GivenType &given = *it;
		if (CanGiveObject(&given)) {
			gtypes.push_back(given.gtype);
			gids.push_back(given.gid);
		} else {
			return BOOLEAN_FALSE;
		}
	}

	m_bGiving = BOOLEAN_TRUE;
	CB_AskGiven(m_pOwner->GetPeer(), m_pOwner->GetContext(), s, npc_id, gtypes, gids);

	return BOOLEAN_TRUE;
}

RPCResult CScriptSystem::ReplyGiven(LPCPEER pPeer, RPCContext &context, 
									const UInt8Vector &gtypeList, const UInt32Vector &uidList)
{
	if (m_pOwner == NULL) {
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("ReplyGiven Warning: [script no owner]"));
		return RPC_RESULT_FAIL;
	}
	if (!m_bGiving) {
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("ReplyGiven Warning: [no ask given]"));
		return RPC_RESULT_FAIL;
	}
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("ReplyGiven %s sessionId %s srvId %d, subId %d char_id %d"),
		String(m_scriptFile.c_str()).c_str(),
		m_pOwner->GetContext().sessionId.GetString().c_str(),
		m_pOwner->GetContext().pcdId.srvId, m_pOwner->GetContext().pcdId.subId,
		m_pOwner->GetChar_id());

	Boolean required = (!m_givenList.empty());
	String errMsg;
	GivenType newGiven;
	do {
		// cancel given process
		if (gtypeList.empty()) {
			m_givenList.clear();
			break;
		}

		// verify return list size to given list size
		if (gtypeList.size() != uidList.size() || 
			required && m_givenList.size() != uidList.size())
		{
			errMsg.Format(_T("mismatch reply given size %d:%d != %d"), 
				gtypeList.size(), uidList.size(), m_givenList.size());
			break;
		}

		// check if player have all asked object
		boolean cancel = false;
		for (UInt16 i = 0; !cancel && i < (UInt16) gtypeList.size(); i++)
		{
			newGiven.gtype = gtypeList[i];
			newGiven.uid = uidList[i];
			newGiven.gid = 0;
			if (newGiven.gtype == 0 || newGiven.uid == 0) {
				errMsg = _T("empty object");
				break;
			}
			if (!HaveGivenObject(&newGiven)) {
				errMsg = _T("haven't got object");
				break;
			}
			for (GivenList::iterator it = m_givenList.begin(); 
				it != m_givenList.end(); ++it)
			{
				GivenType &given = *it;
				if (given.gtype == newGiven.gtype && given.uid == newGiven.uid) {
					errMsg = _T("duplicate object");
					cancel = true;
					break;
				}
				// find required object and assign the uid
				if (required && given.uid == 0 && given.gtype == newGiven.gtype && 
					given.gid == newGiven.gid)
				{
					given.uid = newGiven.uid;
					break;
				}
			}
			// add to given list if not required
			if (!required)
				m_givenList.push_back(newGiven);
		}
		// check if all required object found
		if (required) {
			for (GivenList::iterator it = m_givenList.begin(); 
				it != m_givenList.end(); ++it)
			{
				GivenType &given = *it;
				if (given.uid == 0) {
					newGiven = given;
					errMsg = _T("missing required object");
				}
			}
		}

	} while (false);

	// show error message and clear given list to fail given
	if (!errMsg.IsEmpty()) {
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("ReplyGiven Warning: [%s], gtype %d, gid %d, uid %d"),
			errMsg.c_str(), newGiven.gtype, newGiven.gid, newGiven.uid);
		m_givenList.clear();
	}

	// resume process after given
	Resume();

	return RPC_RESULT_OK;
}

void CScriptSystem::ClearGiven()
{
	m_givenList.clear();
}

void CScriptSystem::AskGivenObj(UInt8 gtype, UInt32 gid)
{
	m_givenList.push_back(GivenType(gtype, gid));
}

UInt8 CScriptSystem::GetGivenType(UInt16 i)
{
	if (i < (UInt16) m_givenList.size())
		return m_givenList[i].gtype;
	else
		return 0;
}

UInt32 CScriptSystem::GetGivenID(UInt16 i)
{
	if (i < (UInt16) m_givenList.size())
		return m_givenList[i].gid;
	else
		return 0;
}

UInt32 CScriptSystem::GetGivenUID(UInt16 i)
{
	if (i < (UInt16) m_givenList.size())
		return m_givenList[i].uid;
	else
		return 0;
}

UInt16 CScriptSystem::GetGivenCount()
{
	return ((UInt16) m_givenList.size());
}

void CScriptSystem::RemoveAllGiven()
{
	if (m_givenList.empty())
		return;

	for (GivenList::iterator it = m_givenList.begin(); it != m_givenList.end(); ++it)
	{
		GivenType &given = *it;
		switch(given.gtype)
		{
		case GIVEN_ITEM:
			m_pOwner->GetBag()->RemoveItemByUID(given.uid);
			break;

		case GIVEN_PARTNER:
			m_pOwner->GetPartners()->RemovePartnerByUID(given.uid);
			break;

		case GIVEN_PET:
			m_pOwner->GetPets()->RemovePetByUID(given.uid);
			break;
		}
	}
	m_givenList.clear();
}

void CScriptSystem::AskString(PCSTRINGA text, PCSTRINGA defAns, bool bLeft, UInt8 askerType,
							  UInt32 askerId)
{
	if (m_pOwner == NULL)
		return;

	StringW s;
	UTF8ToStr(text, s);
	UTF8ToStr(defAns, m_answerString);

	CB_AskString(m_pOwner->GetPeer(), m_pOwner->GetContext(), s, m_answerString, bLeft, 
		askerType, askerId);
	m_bTalking = BOOLEAN_TRUE;
}

PCSTRINGA CScriptSystem::GetAnswerString()
{
	static char buf[512];
	StringA outStr;

	StrToUTF8(m_answerString.c_str(), outStr);
	strcpy(buf, outStr.c_str());
	return buf;
}

Boolean CScriptSystem::IsNewScript()
{
	return BOOLEAN_TRUE;
}

void CScriptSystem::SetNpc(CMapNPC* pNpc)
{
	if (pNpc)
		m_npcNetId = pNpc->GetNetId();
}

void CScriptSystem::CheckJump()
{
	if (m_pOwner && m_jumpPos.map_id != 0)
		m_pOwner->JumpMap(m_jumpPos.map_id, m_jumpPos.x, m_jumpPos.y, 
			m_jumpPos.line_id, m_jumpPos.owner_id,m_jumpPos.bCheckLua);
}

// for battle
Boolean CScriptSystem::HasBatMethod(PCSTRINGA method)
{
	if (m_pLuaState == NULL || m_pBattle == NULL || m_batTable.IsEmpty())
		return BOOLEAN_FALSE;

	return HasLuaMethod(m_pLuaState, m_batTable.c_str(), method);
}

void CScriptSystem::CallBatMethod(PCSTRINGA method, int turn, CBatActor *actor, int result)
{
	if (m_pLuaState == NULL || m_pBattle == NULL)
		return;
	
	int nRes = lua_status(m_pLuaState);
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("method %s, char_id %d, state %d, script %s"),
		String(method).c_str(), m_pOwner ? m_pOwner->GetChar_id() : -1, nRes, 
		String(m_scriptFile.c_str()).c_str());
	int r = 0;
	SetLuaGlobal(BOOLEAN_TRUE);
	String errMsg;
	if (turn == 0)
		r = CallLuaMethod(m_pLuaState, m_batTable.c_str(), method, errMsg);
	else if (result == 0 && actor == NULL)
		r = CallLuaMethod(m_pLuaState, m_batTable.c_str(), method, errMsg, turn);
	else if (actor)
		r = CallLuaMethod(m_pLuaState, m_batTable.c_str(), method, errMsg, turn, actor);
	else
		r = CallLuaMethod(m_pLuaState, m_batTable.c_str(), method, errMsg, turn, result);

	ScriptReturn(r, errMsg);
}

Boolean CScriptSystem::CallItemMethod(PCSTRINGA method, NetObject* pUser, NetObject* pTarget)
{
	if (m_pLuaState == NULL || !m_itemNetId.IsValid())
		return BOOLEAN_FALSE;

	int status = lua_status(m_pLuaState);
#ifdef LUA_STACK_TRACE_SERVER
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("char_id %d, state %p, status %d, top %d, script %s"),
		m_pOwner ? m_pOwner->GetChar_id() : -1, m_pLuaState, status, lua_gettop(m_pLuaState), 
		String(m_scriptFile.c_str()).c_str());
#endif // LUA_STACK_TRACE_SERVER
	// try to resume 
	if (status == LUA_YIELD)
		Resume(false);
	else
		SetLuaGlobal(BOOLEAN_TRUE);

	// treat no method return true
	if (!HasLuaMethod(m_pLuaState, m_itemTable.c_str(), method))
		return BOOLEAN_TRUE;

	int r = 0;
	String errMsg;
	if (pUser == NULL)
		r = CallLuaMethod(m_pLuaState, m_itemTable.c_str(), method, errMsg, pTarget);
	else
		r = CallLuaMethod(m_pLuaState, m_itemTable.c_str(), method, errMsg, pUser, pTarget);

	return ScriptReturn(r, errMsg);
}

// for instance
Boolean CScriptSystem::HasInstMethod(PCSTRINGA method)
{
	if (m_pLuaState == NULL || m_instTable.IsEmpty())
		return BOOLEAN_FALSE;

	return HasLuaMethod(m_pLuaState, m_instTable.c_str(), method);
}

Boolean CScriptSystem::CallInstMethod(PCSTRINGA method, CInstance* pInst, CCharacter* pChar)
{
	if (m_pLuaState == NULL)
		return false;
	
	int nRes = lua_status(m_pLuaState);
	int r = 0;
	String errMsg;

	if (nRes == LUA_YIELD)
		Resume(false);
	else
		SetLuaGlobal(BOOLEAN_TRUE);
	
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("method %s, char_id %d, state %d, script %s"),
		String(method).c_str(), m_pOwner ? m_pOwner->GetChar_id() : -1, nRes, 
		String(m_scriptFile.c_str()).c_str());
	if (stricmp(method, INSTANCE_CREATE_FUNC) == 0)
	{	
		// for create method, call create instance function
		LuaVar inst_t(m_instTable.c_str());
		r = CallLuaFunc(m_pLuaState, method, errMsg, inst_t);
	}
	else 
	{
		// set global instance variable
		lua_getglobal(m_pLuaState, "_G");
		// set lua global variable "player" if character is specified
		if (pChar)
			LuaPush(m_pLuaState, pChar);
		else
			lua_pushnil(m_pLuaState);
		lua_setfield(m_pLuaState, -2, "player");
		if (pChar && pChar->HasParty())
			LuaPush(m_pLuaState, pChar->GetParty());
		else
			lua_pushnil(m_pLuaState);
		lua_setfield(m_pLuaState, -2, "party");
		if (pInst)
			LuaPush(m_pLuaState, pInst);
		else
			lua_pushnil(m_pLuaState);
		lua_setfield(m_pLuaState, -2, "instance");
		lua_pop(m_pLuaState, 1);			// pop "_G"
		r = CallLuaMethod(m_pLuaState, m_instTable.c_str(), method, errMsg);
	}
	return ScriptReturn(r, errMsg);
}

void CScriptSystem::LogMessage(int level, PCSTRINGA msg)
{
	String s;
	UTF8ToStr(msg, s);
	
	if (m_pOwner)
	{
		TRACE_LOGDTL_3(GLOBAL_LOGGER, (Reuben::Diagnostics::Logger::LevelEnum) level, 
			_T("LogMessage: char_id %d, sessionId %s, %s"), m_pOwner->GetChar_id(),
			m_pOwner->GetSessionId().GetString().c_str(), s.c_str());
	}
	else
	{
		TRACE_LOGDTL_1(GLOBAL_LOGGER, (Reuben::Diagnostics::Logger::LevelEnum) level, 
			_T("LogMessage: %s"), s.c_str());
	}
}

template<typename T>
void CScriptSystem::PushLocalInt(T intValue, int varIndex)
{
	if (m_pLuaState == NULL)
		return;

	lua_Debug debugInfo;

	if (lua_getstack(m_pLuaState, 1, &debugInfo) != 1)	// get local variable stack
		return;
	lua_pushinteger(m_pLuaState, intValue);				// push integer value
	lua_setlocal(m_pLuaState, &debugInfo, varIndex);	// set to local value by index
}
		
void CScriptSystem::PushLocalString(const String &str, int varIndex)
{
	if (m_pLuaState == NULL)
		return;

	lua_Debug debugInfo;
	StringA strName;

	StrToUTF8(str.c_str(), strName);
	if (lua_getstack(m_pLuaState, 1, &debugInfo) != 1)	// get local variable stack
		return;
	lua_pushstring(m_pLuaState, strName.c_str());				// push string value
	lua_setlocal(m_pLuaState, &debugInfo, varIndex);	// set to local value by index
}
		
template<typename T>
void CScriptSystem::PushIntArray(const StlVector<T, ManagedAllocator<T> > &intList, int varIndex)
{
	if (m_pLuaState == NULL)
		return;

	lua_Debug debugInfo; 

	if (lua_getstack(m_pLuaState, 1, &debugInfo) != 1)	// get local variable stack
		return;
	lua_getlocal(m_pLuaState, &debugInfo, varIndex);	// get local table variable by index
	if (!intList.empty())
	{
		int i = 1;
		for (StlVector<T, ManagedAllocator<T> >::const_iterator it = intList.begin(); 
			it != intList.end(); ++it, ++i)
		{
			lua_pushinteger(m_pLuaState, *it);			// push integer value
			lua_rawseti(m_pLuaState, -2, i);			// set to array value [i]
		}
	}
	lua_pop(m_pLuaState, 1);							// pop var index table
}

void CScriptSystem::PushStringArray(const StringVector &strList, int varIndex)
{
	if (m_pLuaState == NULL)
		return;

	lua_Debug debugInfo;
	StringA strName;

	if (lua_getstack(m_pLuaState, 1, &debugInfo) != 1)	// get local variable stack
		return;
	lua_getlocal(m_pLuaState, &debugInfo, varIndex);	// get local table variable by index
	if (!strList.empty())
	{
		int i = 1;
		for (StringVector::const_iterator it = strList.begin(); 
			it != strList.end(); ++it, ++i)
		{
			StrToUTF8((*it).c_str(), strName);
			lua_pushstring(m_pLuaState, strName.c_str());	// push string value
			lua_rawseti(m_pLuaState, -2, i);				// set to array value [i]
		}
	}
	lua_pop(m_pLuaState, 1);						// pop var index table
}

void CScriptSystem::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_ANS_TIMEOUT:				// auto reply answer
		if (m_pOwner) {
			ReplyAnswer(255);
		}
		break;
	}
}

void CScriptSystem::SendPrivateMessage(PCSTRINGA msg, UInt32 SourceId, UInt32 TargetID, PCSTRINGA TargetName, Boolean isFriend)
{
	if (m_pOwner == NULL)
		return;
	String strMsg;
	UTF8ToStr(msg, strMsg);

	String strTargetName;
	UTF8ToStr(TargetName, strTargetName);

	MessageService_Proxy::SendPrivateMessage(gGlobal.pMessageService->pPeer, m_pOwner->GetContext(), strMsg, SourceId, TargetID, strTargetName, isFriend);
}

CCharacter* CScriptSystem::FindCharacter(UInt32 char_id)
{
	return gGlobal.m_charToSession.FindCharacter(char_id);
}


Boolean CScriptSystem::ExecutePvPBattleScript(UInt32 pvpBattleID)
{
	if(pvpBattleID <= 0)
		return false;
	Boolean success = BOOLEAN_FALSE;

	String file;
	file.Format(_T("BatPvP%d.lua"),pvpBattleID);
	StringA filename(file.c_str());;
	filename.Insert(0, SCRIPT_BAT_PATH);
	success = ExecuteLua(filename);
	if (success)
	{
		m_scriptType = SCRIPT_TYPE_BATTLE;
		TRACE_INFODTL_3(GLOBAL_LOGGER, _T("ExecutePvPBattleScript:script file name = %s,m_pOwner ID = %d,m_pOwner PvPBattleScriptID = %d"), 
			filename.c_str(),m_pOwner->GetChar_id(),m_pOwner->GetPvPBattleScriptID());
	}
	else
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _T("ExecutePvPBattleScript:script file name = %s Not Success"),filename.c_str());
	}
	return success;

}