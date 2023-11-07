//-- Common
#include "Common.h"
//-- Self
#include "BattleService/BatScriptSystem.h"
//-- Library
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/BatActor.h"
#include "Stub/Logic/BatMob.h"
#include "Stub/Logic/BatCharacter.h"
#include "BattleService/Global.h"
#include "Resource/ResNpc.h"
#include "Resource/ServerResManager.h"
#include "Common/CallLuaFunc.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"

	//int Luai_Init(lua_State* L);		// declare the wrapped module
}

#define SZ_UNIT_PLAYER						"player"
#define SZ_UNIT_NPC							"npc"
#define SZ_UNIT_WHOLETEAM					"team"
#define SZ_UNIT_PARTY						"party"
#define SZ_UNIT_PARTY1						"party1"
#define SZ_UNIT_PARTY2						"party2"
#define SZ_UNIT_PARTY3						"party3"
#define SZ_UNIT_PARTY4						"party4"
#define SZ_UNIT_PARTY5						"party5"

#define SZ_BATSCRIPT_DEFAULTLUA				"DefaultBatScriptFuncS.lua"
#define SZ_BATSCRIPT_DEFAULTOUT				"DefaultBatScriptFuncS.out"

#define SZ_BATSCRIPTFUNC_INITPART			"OnInitBatPart"
#define SZ_BATSCRIPTFUNC_TURNEND			"OnTurnEndPart"
#define SZ_BATSCRIPTFUNC_MOBCMDPART			"OnMobCmdPart"
#define SZ_BATSCRIPTFUNC_PLAYERCMDPART		"OnPlayerCmdPart"
#define SZ_BATSCRIPTFUNC_PRETURNPART		"OnPreTurnPart"
#define SZ_BATSCRIPTFUNC_BATWINPART			"OnWinPart"
#define SZ_BATSCRIPTFUNC_BATLOSEPART		"OnLosePart"

PROTO_LuaPush(CBatScriptSystem)
PROTO_LuaPush(CBatActor)



/////////////         local function     //////////////////
template <typename T1>
void LuaCallThreadFunc1(lua_State *L, PCSTRINGA func, T1 P1)
{
	if(L && func)
	{
		lua_getglobal(L, func);  
		if ( lua_isfunction(L, -1) )
		{
			LuaPush(L, P1);

			int nRes = lua_resume(L, 1);
			if ( nRes == 0 )
			{
				printf("[Battle Script] : Call function %s successed", func);
			}
			else if ( nRes == LUA_YIELD )
			{
				printf("[Battle Script] : Yield in the function %s", func);
			}
			else
			{
				const char* pErr = lua_tostring(L, -1);
				if ( pErr )
					printf("%s\n", pErr);
			}
		}
		else
		{
			lua_pop(L, -1);
			printf("[Battle Script]: Can not find the funciton [%s]", func);
		}
	}
}

template <typename T1, typename T2>
void LuaCallThreadFunc2(lua_State *L, PCSTRINGA func, T1 P1, T2 P2)
{
	if(L && func)
	{
		lua_getglobal(L, func);  
		if ( lua_isfunction(L, -1) )
		{
			LuaPush(L, P1);
			LuaPush(L, P2);

			int nRes = lua_resume(L, 2);
			if ( nRes == 0 )
			{
				printf("[Battle Script] : Call function %s successed", func);
			}
			else if ( nRes == LUA_YIELD )
			{
				printf("[Battle Script] : Yield in the function %s", func);
			}
			else
			{
				const char* pErr = lua_tostring(L, -1);
				if ( pErr )
					printf("%s\n", pErr);
			}
		}
		else
		{
			lua_pop(L, -1);
			printf("[Battle Script]: Can not find the funciton [%s]", func);
		}
	}
}

template <typename T1, typename T2, typename T3>
void LuaCallThreadFunc3(lua_State *L, PCSTRINGA func, T1 P1, T2 P2, T3 P3)
{
	if(L && func)
	{
		lua_getglobal(L, func);  
		if ( lua_isfunction(L, -1) )
		{
			LuaPush(L, P1);
			LuaPush(L, P2);
			LuaPush(L, P3);

			int nRes = lua_resume(L, 3);
			if ( nRes == 0 )
			{
				printf("[Battle Script] : Call function %s successed", func);
			}
			else if ( nRes == LUA_YIELD )
			{
				printf("[Battle Script] : Yield in the function %s", func);
			}
			else
			{
				const char* pErr = lua_tostring(L, -1);
				if ( pErr )
					printf("%s\n", pErr);
			}
		}
		else
		{
			lua_pop(L, -1);
			printf("[Battle Script]: Can not find the funciton [%s]", func);
		}
	}
}


template <typename T1, typename T2, typename T3, typename T4, typename T5>
void LuaCallThreadFunc5(lua_State *L, PCSTRINGA func, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5)
{
	if(L && func)
	{
		lua_getglobal(L, func);  
		if ( lua_isfunction(L, -1) )
		{
			LuaPush(L, P1);
			LuaPush(L, P2);
			LuaPush(L, P3);
			LuaPush(L, P4);
			LuaPush(L, P5);

			int nRes = lua_resume(L, 5);
			if ( nRes == 0 )
			{
				printf("[Battle Script] : Call function %s successed", func);
			}
			else if ( nRes == LUA_YIELD )
			{
				printf("[Battle Script] : Yield in the function %s", func);
			}
			else
			{
				const char* pErr = lua_tostring(L, -1);
				if ( pErr )
					printf("%s\n", pErr);
			}
		}
		else
		{
			lua_pop(L, -1);
			printf("[Battle Script]: Can not find the funciton [%s]", func);
		}
	}
}


CBatScriptSystem::CBatScriptSystem()
{
	m_pOwner = NULL;
	m_pBattleLua = NULL;
	m_nChildRefKey = 0;

	ReInitParam();
}

CBatScriptSystem::~CBatScriptSystem()
{
	m_pOwner = NULL;
	ReleaseChildState();
}

VOID CBatScriptSystem::ReInitParam()
{
	m_bScriptRunning = BOOLEAN_FALSE;
	m_iTotalSelection = -1;

	m_bImageLeft = BOOLEAN_TRUE;
	m_SelectNpcImage = 0;
	m_bModDialogImage = BOOLEAN_FALSE;

	m_BatScriptEventLst.clear();

	m_TempChooseMsg.optLst.clear();
}

VOID CBatScriptSystem::InitTurn()
{
	m_iTotalSelection = -1;
	m_BatScriptEventLst.clear();

	m_TempChooseMsg.optLst.clear();

	m_bChooseTurn = BOOLEAN_FALSE;
}

VOID CBatScriptSystem::ReleaseChildState()
{
	if ( m_pBattleLua )
	{
		int nStatus = lua_status(m_pBattleLua);
		printf("[Battle Lua] Curr State When Release: %d\n", nStatus);
		if ( nStatus == 0 )
		{
			int normal = 1;
		}
		else if ( nStatus == LUA_YIELD )
		{
			//if ( m_pPlayerLua )
			//	lua_pushnil(m_pPlayerLua);
			int yield = 1;
		}
		else
		{
			PrintLuaErr();
		}
		///
		//printf("Before UnRef Parent Use Memory: %d\n", lua_gc( gGlobal.GetParentLua(), LUA_GCCOUNT, 0 ));

		//printf("Before UnRef Child Use Memory: %d\n", lua_gc( m_pBattleLua, LUA_GCCOUNT, 0 ));


		luaL_unref(m_pBattleLua, LUA_REGISTRYINDEX, m_nChildRefKey);

		//printf("After UnRef Parent Use Memory: %d\n", lua_gc( gGlobal.GetParentLua(), LUA_GCCOUNT, 0 ));

		//printf("After UnRef Child Use Memory: %d\n", lua_gc( m_pBattleLua, LUA_GCCOUNT, 0 ));
		//

		/////////////////////////////////////////////////////
		//printf("Before Collection Parent Use Memory: %d\n", lua_gc( gGlobal.GetParentLua(), LUA_GCCOUNT, 0 ));

		//printf("Before Collection Child Use Memory: %d\n", lua_gc( m_pBattleLua, LUA_GCCOUNT, 0 ));

		lua_gc(m_pBattleLua, LUA_GCCOLLECT, 0);

		//printf("After Collection Child Use Memory: %d\n", lua_gc( m_pBattleLua, LUA_GCCOUNT, 0 ));

		//printf("After Collection Parent Use Memory: %d\n", lua_gc( gGlobal.GetParentLua(), LUA_GCCOUNT, 0 ));

		
		m_pBattleLua = NULL;
	}
}

VOID CBatScriptSystem::PrintLuaErr()
{
	if ( m_pBattleLua )
	{
		const char* pErr = lua_tostring(m_pBattleLua, -1);
		if ( pErr )
			printf("%s", pErr);
	}
}

Int32 CBatScriptSystem::UTF82StringW(StringW &Str, PCSTRINGA Text)
{
	if ( Text )
	{
		Int32 Len = MultiByteToWideChar(CP_UTF8, 0, Text, -1, NULL, 0);
		PSTRINGW TempStr = SafeAllocate(CharW, Len);
		if ( TempStr )
		{
			MultiByteToWideChar(CP_UTF8, 0, Text, -1, TempStr, Len);
			//Str = TempStr;
			Str.Format(_T("%s"), TempStr);
			SafeDeallocate(TempStr);
			return Len;
		}
	}

	Str = _T("");
	return 0;
}

Int32 CBatScriptSystem::StringW2UTF8(StringA &Des, PCSTRINGW Src)
{
	if ( Src )
	{
		Int32 Len = WideCharToMultiByte(CP_UTF8, 0, Src, -1, NULL, 0, NULL, NULL);
		PSTRINGA TempStr = SafeAllocate(CharA, Len);
		if ( TempStr )
		{
			WideCharToMultiByte(CP_UTF8, 0, Src, -1, TempStr, Len, NULL, NULL);
			Des.Format("%s", TempStr);
			SafeDeallocate(TempStr);
			return Len;
		}
	}

	Des = "";
	return 0;
}

Boolean CBatScriptSystem::InitChildState()
{
	if ( m_pBattleLua )
		ReleaseChildState();

	m_pBattleLua = gGlobal.CreateChildState(m_nChildRefKey);
	if ( !m_pBattleLua )
	{
		ReleaseChildState();
		return BOOLEAN_FALSE;
	}
	return BOOLEAN_TRUE;
}

VOID CBatScriptSystem::CallInitBatPart()
{
	if( m_pOwner && m_pBattleLua )
	{
		LuaCallThreadFunc1(m_pBattleLua, SZ_BATSCRIPTFUNC_INITPART, this);
	}
}

VOID CBatScriptSystem::CallMobCmdPart(CBatActor * pActor)
{
	if( m_pOwner && m_pBattleLua && pActor)
	{
		int turn = ( m_pOwner )?m_pOwner->m_curTurn:-1;
		LuaCallThreadFunc5(m_pBattleLua, SZ_BATSCRIPTFUNC_MOBCMDPART, this, turn, pActor, pActor->GetHP(), pActor->GetSP());
	}
}

VOID CBatScriptSystem::CallTurnEndPart()
{
	if( m_pOwner && m_pBattleLua )
	{
		for (CBattle::BatClients::iterator it = m_pOwner->m_clients.begin(); it != m_pOwner->m_clients.end(); ++it)
		{
			BatClient &client = it->second;
			if (client.status != CBattle::CLIENT_BATTLE || client.pPeer == NULL || client.pBatChar == NULL)
				continue;

			if ( client.pBatChar && client.pBatChar->m_bLeader && (client.teamIndex == 0) ) 
			{
				int turn = ( m_pOwner )?m_pOwner->m_curTurn:-1;
				LuaCallThreadFunc2(m_pBattleLua, SZ_BATSCRIPTFUNC_TURNEND, this, turn);
				break;
			}
		}	
	}
}

VOID CBatScriptSystem::CallPlayerCmdPart(CBatActor * pActor)
{
	if( m_pOwner && m_pBattleLua && pActor )
	{
		int turn = ( m_pOwner )?m_pOwner->m_curTurn:-1;
		LuaCallThreadFunc5(m_pBattleLua, SZ_BATSCRIPTFUNC_PLAYERCMDPART, this, turn, pActor, pActor->GetHP(), pActor->GetSP());
	}
}

VOID CBatScriptSystem::CallPreTurnPart(CBatActor * pActor)
{
	InitTurn();

	if( m_pOwner && m_pBattleLua && pActor )
	{
		int turn = ( m_pOwner )?m_pOwner->m_curTurn:-1;
		LuaCallThreadFunc3(m_pBattleLua, SZ_BATSCRIPTFUNC_PRETURNPART, this, turn, pActor);
	}
}

VOID CBatScriptSystem::CallDefaultChooseAnsPart()
{
	if ( m_pOwner ) 
	{
		for (CBattle::BatClients::iterator it = m_pOwner->m_clients.begin(); it != m_pOwner->m_clients.end(); ++it)
		{
			BatClient &client = it->second;
			if (client.status != CBattle::CLIENT_BATTLE || client.pPeer == NULL || client.pBatChar == NULL)
				continue;

			if ( client.pBatChar && client.pBatChar->m_bLeader && (client.teamIndex == 0) ) 
			{
				if ( IsScriptRunning() && (m_iTotalSelection != -1) && m_pBattleLua )
				{
					m_iTotalSelection = -1;
					int turn = ( m_pOwner )?m_pOwner->m_curTurn:-1;
					LuaCallThreadFunc2(m_pBattleLua, "OnBatChooseNoAns", this, turn);
				}
				
				break;
			}
		}
	
		m_pOwner->DoBatResumeBatCmdMenu(BOOLEAN_TRUE);
	}
}

VOID CBatScriptSystem::CallBatWinPart()
{
	if( m_pBattleLua && m_pOwner )
	{
		int turn = ( m_pOwner )?m_pOwner->m_curTurn:-1;
		LuaCallThreadFunc2(m_pBattleLua, SZ_BATSCRIPTFUNC_BATWINPART, this, turn);
	}
}

VOID CBatScriptSystem::CallBatLosePart()
{
	if( m_pBattleLua && m_pOwner )
	{
		int turn = ( m_pOwner )?m_pOwner->m_curTurn:-1;
		LuaCallThreadFunc2(m_pBattleLua, SZ_BATSCRIPTFUNC_BATLOSEPART, this, turn);
	}
}

Boolean CBatScriptSystem::LoadBattleScript(UInt32 npcId)
{
	if ( m_bScriptRunning )
	{
		if ( lua_status(m_pBattleLua) == LUA_YIELD )
		{
			int nRes = lua_resume(m_pBattleLua, 0);
			if ( nRes == 0 )
			{// normal
				ReInitParam();
				ReleaseChildState();
				return BOOLEAN_TRUE;
			}
			else if ( nRes == LUA_YIELD )
			{
				return BOOLEAN_TRUE;
			}
			else
			{
				PrintLuaErr();

				ReInitParam();
				ReleaseChildState();
				return BOOLEAN_FALSE;
			}
		}
		else
			return BOOLEAN_FALSE;
	} 

	const NpcData* pnpc = gGlobal.m_resource->GetNpcData(npcId);
	if ( !pnpc ) return BOOLEAN_FALSE;

	if ( pnpc->battleScript && ( _tcsicmp(pnpc->battleScript, _T("")) != 0 ) )
	{
		StringA filename(pnpc->battleScript);
		//StringA filename("batscript.lua");

		filename.Replace(".lua", ".out");

		size_t templen = 0;
	
		//gGlobal.m_resource->LoadLuaData();

		if ( gGlobal.m_resource->GetLuaData(filename, templen, TRUE) != NULL )
		{
			m_bScriptRunning = BOOLEAN_TRUE;
			m_iTotalSelection = -1;

			ExecuteLua(filename);

			return BOOLEAN_TRUE;
		}
	}

	return BOOLEAN_FALSE;
}

Boolean CBatScriptSystem::LoadDefaultBatLuaFunc(CONST StringA& szfilename)
{
	size_t defaultlen = 0;
	CONST char* pdefault = gGlobal.m_resource->GetLuaData(szfilename, defaultlen, TRUE);
	if ( !pdefault ) 
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _T("CBatScriptSystem LoadBatLuaFunc: Can't find the default battle lua function file"));
	}
	else
	{
		if ( luaL_loadbuffer(m_pBattleLua, pdefault, defaultlen, szfilename.c_str()) !=0 )
		{
			PrintLuaErr();
		}
		else
		{
			int nRes = lua_resume(m_pBattleLua, 0);
			if ( nRes == 0 )
			{
				/*
				lua_getglobal(m_pPlayerLua, "SetDefaultLuaOwner");
				LuaPush(m_pPlayerLua, this);
				LuaPush(m_pPlayerLua, m_InstanceNpcId);
				if ( lua_resume(m_pPlayerLua, 2 ) != 0 )
				{
					printf("Load Lua function SetDefaultLuaOwner failed\n");
					PrintLuaErr();
				}
				else
				{
					bLoad = BOOLEAN_TRUE;
				}
				*/
				return BOOLEAN_TRUE;
			}
			else if ( nRes == LUA_YIELD )
			{
				int yield = 1;
			}
			else
			{
				PrintLuaErr();
			}
		}
	}

	return BOOLEAN_FALSE;
}

Boolean CBatScriptSystem::LoadNpcLuaFunc(CONST StringA& szfilename)
{
	size_t len = 0;
	CONST char* pmem = gGlobal.m_resource->GetLuaData(szfilename, len, TRUE);
	if ( luaL_loadbuffer(m_pBattleLua, pmem, len, szfilename.c_str()) ==0 )
	{
		int nRes = lua_resume(m_pBattleLua, 0);
		if ( nRes == 0 )
		{
			//ReInitParam();
			//ReleaseChildState();

			return BOOLEAN_TRUE;
		}
		else if ( nRes == LUA_YIELD )
		{
			return BOOLEAN_TRUE;
		}
		else 
		{
			PrintLuaErr();
		}
	}
	else
	{
		PrintLuaErr();
	}
	return BOOLEAN_FALSE;
}

Boolean CBatScriptSystem::ExecuteLua(CONST StringA& szfilename)
{
	if ( InitChildState() )
	{
		StringA DefaultLua = SZ_BATSCRIPT_DEFAULTOUT;

		if ( LoadDefaultBatLuaFunc(DefaultLua) )
		{
			if ( LoadNpcLuaFunc(szfilename) )
			{
				return BOOLEAN_TRUE;
			}
		}

		ReInitParam();
		ReleaseChildState();
	}
	
	return BOOLEAN_FALSE;
}

void CBatScriptSystem::LuaSetBatNoPunish(bool bPunish)
{
	printf("\n Set Battle No Punish\n");
	m_BatConfigure.m_bNoPunishment = (bPunish)?BOOLEAN_TRUE:BOOLEAN_FALSE;
}

void CBatScriptSystem::LuaDisEnableBatCmd(UInt32 disEnbale, bool bUpdate)
{
	printf("\n Set Battle Disenable\n");
	m_BatConfigure.m_dwDisEnableCmd = disEnbale;

	if ( m_pOwner )
	{
		DisEnableBatCmd();
	}
}

void CBatScriptSystem::LuaBatSay(PCSTRINGA szUnitID, PCSTRINGA Msg, PCSTRINGA szImage, bool bLeft, UInt32 NpcId)
{
	if ( !m_pOwner || !m_pBattleLua || !Msg || !szUnitID ) return;

	UInt8 teamIndex = 100;

	// teamIndex, 100:do not send, 255:send for whole team, 254: same as 255, except leader
	{
		StringA Target("");
		Target.Format("%s", szUnitID);

		if ( Target == SZ_UNIT_PLAYER )
			teamIndex = 0;
		else if ( Target == SZ_UNIT_PARTY1 || Target == SZ_UNIT_PARTY2 || Target == SZ_UNIT_PARTY3 || Target == SZ_UNIT_PARTY4 || Target == SZ_UNIT_PARTY5 )
		{
			Target.Replace(SZ_UNIT_PARTY, "");
			int pos = atoi(Target.c_str());
			if ( pos > 0 && pos < 6 )
				teamIndex = pos;
			else
				teamIndex = 100;
		}
		else if ( Target == SZ_UNIT_WHOLETEAM )
			teamIndex = 255;
		else
			teamIndex = 100;
	}

	if ( teamIndex != 100 )
	{
		StringW str(_T(""));
		if ( UTF82StringW(str, Msg) > 0 )
		{
			String SayMsg;
			SayMsg.Format(_T("%s"), str.c_str());

			if ( m_pOwner )
			{
				Int imagetype = ConvertUnit2Int(szImage);
				UInt32 imageid = (m_pOwner->m_pNpcData)?(m_pOwner->m_pNpcData->npc_id):0;
				if ( NpcId != 0 )
					imageid = NpcId;
				
				m_pOwner->DoBatSay(teamIndex, imageid, SayMsg, imagetype, bLeft);
			}
		}
	}
}

bool CBatScriptSystem::LuaBatChoose(PCSTRINGA szQuestion, PCSTRINGA C1, PCSTRINGA C2)
{
	if ( !m_pOwner || !m_pBattleLua || !szQuestion || !C1 ) 
	{
		ResetDialogImage();
		return false;
	}

	StringVector OptMsg;
	OptMsg.clear();

	Int nTotal = 0;
	StringW str(_T(""));
	if ( C1 && ( UTF82StringW(str, C1) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}

	if ( C2 && ( UTF82StringW(str, C2) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}
/*
	if ( C3 && ( UTF82StringW(str, C3) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}

	if ( C4 && ( UTF82StringW(str, C4) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}

	if ( C5 && ( UTF82StringW(str, C5) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}

	if ( C6 && ( UTF82StringW(str, C6) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}

	if ( C7 && ( UTF82StringW(str, C7) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}

	if ( C8 && ( UTF82StringW(str, C8) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}

	if ( C9 && ( UTF82StringW(str, C9) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}

	if ( C10 && ( UTF82StringW(str, C10) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}

	if ( C11 && ( UTF82StringW(str, C11) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}

	if ( C12 && ( UTF82StringW(str, C12) > 0 ) )
	{
		++nTotal;
		OptMsg.push_back(str.c_str());
	}
*/	
	// push question
	if ( szQuestion && ( UTF82StringW(str, szQuestion) > 0 ) )
	{
		OptMsg.push_back(str.c_str());
	}

	m_iTotalSelection = nTotal;

	if (m_pOwner)
	{ 
		for (CBattle::BatClients::iterator it = m_pOwner->m_clients.begin(); it != m_pOwner->m_clients.end(); ++it)
		{
			BatClient &client = it->second;
			if (client.status != CBattle::CLIENT_BATTLE || client.pPeer == NULL || client.pBatChar == NULL)
				continue;
			if ( client.teamIndex == 0 )
			{
				//Int image = ConvertUnit2Int(szImage);
				Boolean bLeft(BOOLEAN_TRUE);
				UInt32 imageid = (m_pOwner->m_pNpcData)?(m_pOwner->m_pNpcData->npc_id):0;
				if ( m_bModDialogImage )
				{
					imageid = m_SelectNpcImage;
					bLeft = m_bImageLeft;
					m_bModDialogImage = BOOLEAN_FALSE;
				}

				m_TempChooseMsg.optLst.clear();
				m_TempChooseMsg.optLst = OptMsg;
				m_TempChooseMsg.imageId = imageid;
				m_TempChooseMsg.bleft = bLeft;
				m_TempChooseMsg.bDone = BOOLEAN_TRUE;
				
				m_pOwner->CB_OnBatChoose(client.pPeer, client.context, imageid, nTotal, bLeft, OptMsg);

				m_bChooseTurn = BOOLEAN_TRUE;
				//return true;
			}

			m_pOwner->CB_OnBatUpdateCmdMenu(client.pPeer, client.context, client.pBatChar->GetIndex(), BOOLEAN_TRUE, BOOLEAN_FALSE, 0);
		}
	}

	ResetDialogImage();
	return false;
}

void CBatScriptSystem::LuaSetBatEvent(int nTurn, int eventType)
{
	if ( eventType == BSEVENT_SURRENDER ) 
	{
		SBatEventInfo batevent;
		batevent.eventType = BSEVENT_SURRENDER;

		m_BatScriptEventLst.insert(std::make_pair(nTurn, batevent));
	}
	else if ( eventType == BSEVENT_ENDBATTLE )
	{
		SBatEventInfo batevent;
		batevent.eventType = BSEVENT_ENDBATTLE;

		m_BatScriptEventLst.insert(std::make_pair(nTurn, batevent));
	}
	else if ( eventType == BSEVENT_SKIPTURN )
	{
		SBatEventInfo batevent;
		batevent.eventType = BSEVENT_SKIPTURN;

		m_BatScriptEventLst.insert(std::make_pair(nTurn, batevent));
	}
}

void CBatScriptSystem::LuaAddExp(PCSTRINGA szUnitID, UInt32 exp)
{
	if ( !m_pOwner || !m_pBattleLua || !szUnitID ) return;

	Int teamIndex = Change2TeamIndex(szUnitID);
	if ( teamIndex != -1 )
	{
		m_pOwner->DoGiveBatWinReWard(teamIndex, 1, exp);
	}
}

void CBatScriptSystem::LuaAddGameMoney(PCSTRINGA szUnitID, UInt32 money)
{
	if ( !m_pOwner || !m_pBattleLua || !szUnitID ) return;

	Int teamIndex = Change2TeamIndex(szUnitID);
	if ( teamIndex != -1 )
	{
		m_pOwner->DoGiveBatWinReWard(teamIndex, 2, money);
	}
}

void CBatScriptSystem::LuaAddHonor(PCSTRINGA szUnitID, UInt32 honor)
{
	if ( !m_pOwner || !m_pBattleLua || !szUnitID ) return;

	Int teamIndex = Change2TeamIndex(szUnitID);
	if ( teamIndex != -1 )
	{
		m_pOwner->DoGiveBatWinReWard(teamIndex, 4, honor);
	}
}

void CBatScriptSystem::LuaAddItem(PCSTRINGA szUnitID, UInt32 itemid, UInt32 count)
{
	if ( !m_pOwner || !m_pBattleLua || !szUnitID ) return;

	Int teamIndex = Change2TeamIndex(szUnitID);
	if ( teamIndex != -1 )
	{
		m_pOwner->DoGiveBatWinReWard(teamIndex, 3, itemid, count);
	}
}

void CBatScriptSystem::LuaSetGoodness(PCSTRINGA szUnitID, Int32 goodness)
{
	if ( !m_pOwner || !m_pBattleLua || !szUnitID ) return;

	Int teamIndex = Change2TeamIndex(szUnitID);
	if ( teamIndex != -1 )
	{
		m_pOwner->DoBatSetGoodness(teamIndex, goodness);
	}
}

void CBatScriptSystem::LuaBatEndCmdMsg()
{
	if (m_pOwner)
	{ 
		m_pOwner->DoBatSendEndCmdMsg();
	}
}

void CBatScriptSystem::LuaBatResumeBattleMenu()
{
	if (m_pOwner)
	{ 
		m_pOwner->DoBatResumeBatCmdMenu();
	}
}

void CBatScriptSystem::LuaBatSetChooseImage(PCSTRINGA szUnitID, UInt32 NpcId, bool bLeft)
{
	m_bModDialogImage = BOOLEAN_TRUE;
	m_bImageLeft = bLeft;
	m_SelectNpcImage = NpcId;
}

VOID CBatScriptSystem::ResetDialogImage()
{
	if ( m_bModDialogImage )
	{
		m_bImageLeft = BOOLEAN_TRUE;
		m_SelectNpcImage = 0;
		m_bModDialogImage = BOOLEAN_FALSE;
	}
}

VOID CBatScriptSystem::ShowChooseResult2Member(Int Opt)
{
	if ( m_TempChooseMsg.bDone && !(m_TempChooseMsg.optLst.empty()) )
	{
		if ( m_pOwner )
		{
			String Msg(_T(""));
			Msg.append(m_TempChooseMsg.optLst.back());
			size_t count = m_TempChooseMsg.optLst.size();
			for ( size_t i = 0; i < (count-1); ++i )
			{
				if ( Opt == i )
					Msg.append(_T("\\r\\#P"));
				else
					Msg.append(_T("\\r\\#G"));
				
				Msg.append(m_TempChooseMsg.optLst[i]);
			}

			UInt8 Index = 254;
			m_pOwner->DoBatSay(Index, m_TempChooseMsg.imageId, Msg, m_TempChooseMsg.imageType, m_TempChooseMsg.bleft);
		}
	}
}

VOID CBatScriptSystem::OnBatSelectHandle(Int Opt)
{
	if ( m_pOwner && m_pOwner->CanInput() )
	{
		if ( IsScriptRunning() )
		{
			if ( m_iTotalSelection > Opt && m_pBattleLua )
			{
				m_iTotalSelection = -1;

				ShowChooseResult2Member(Opt);

				StringA funcName = "";
				if ( Opt == 0 )
					funcName = "OnBatChooseAnsC1";
				else if ( Opt == 1 )
					funcName = "OnBatChooseAnsC2";
				else
					funcName = "OnBatChooseNoAns";

				{
					int turn = ( m_pOwner )?m_pOwner->m_curTurn:-1;
					LuaCallThreadFunc2(m_pBattleLua, funcName.c_str(), this, turn);
				
					if ( !HasEvent(turn) )
					{
						SetSkipEvent(turn);
					}
				}
				
				m_pOwner->DoBatResumeBatCmdMenu(BOOLEAN_TRUE);
			}
		}
	}
}

Int	CBatScriptSystem::ConvertUnit2Int(PCSTRINGA szUnit)
{
	if ( !szUnit ) return -1;

	StringA Target("");
	Target.Format("%s", szUnit);
	if ( Target == SZ_UNIT_NPC )
		return 0;
	else if ( Target == SZ_UNIT_PLAYER || Target == SZ_UNIT_PARTY1 )
		return 1;
	else if ( Target == SZ_UNIT_PARTY2 )
		return 2;
	else if ( Target == SZ_UNIT_PARTY3 )
		return 3;
	else if ( Target == SZ_UNIT_PARTY4 )
		return 4;
	else if ( Target == SZ_UNIT_PARTY5 )
		return 5;
	else 
		return -1;

	return -1;
}

Int CBatScriptSystem::Change2TeamIndex(PCSTRINGA szUnitID)
{
	Int teamIndex = -1;
	StringA Target("");
	Target.Format("%s", szUnitID);
	if ( Target == SZ_UNIT_PLAYER )
		teamIndex = 0;
	else if ( Target == SZ_UNIT_PARTY1 || Target == SZ_UNIT_PARTY2 || Target == SZ_UNIT_PARTY3 || Target == SZ_UNIT_PARTY4 || Target == SZ_UNIT_PARTY5 )
	{
		Target.Replace(SZ_UNIT_PARTY, "");
		int pos = atoi(Target.c_str());
		if ( pos > 0 && pos < 6 )
			teamIndex = pos;
		else
			teamIndex = -1;
	}
	else if ( Target == SZ_UNIT_WHOLETEAM )
		teamIndex = 255;
	else
		teamIndex = -1;

	return teamIndex;
}

VOID CBatScriptSystem::PreTurnScriptEventHandle(Boolean bEndCmd)
{
	if ( m_pOwner )
	{
		BatScriptEvtLst::iterator itr = m_BatScriptEventLst.find(m_pOwner->m_curTurn);
		if ( itr != m_BatScriptEventLst.end() )
		{
			SBatEventInfo tempevent = itr->second;
			if ( tempevent.bFinishPreTurn == false ) 
			{
				if ( !bEndCmd && !(m_pOwner->CanInput()) )
					return;

				if ( tempevent.eventType == BSEVENT_SURRENDER )
				{
					itr->second.bFinishPreTurn = true;

					m_pOwner->DoAllClientSkipTurn();
				}
				else if ( tempevent.eventType == BSEVENT_ENDBATTLE )
				{
					itr->second.bFinishPreTurn = true;

					m_pOwner->DoAllClientSkipTurn();
				}
				else if ( tempevent.eventType == BSEVENT_SKIPTURN )
				{
					itr->second.bFinishPreTurn = true;

					m_pOwner->DoAllClientSkipTurn();
				}
			}
		}
	}
}

Boolean CBatScriptSystem::EnforceEndBattleEventHandle(UInt8 nextTurn)
{
	if ( m_pOwner )
	{
		BatScriptEvtLst::iterator itr = m_BatScriptEventLst.find(nextTurn-1);
		if ( itr != m_BatScriptEventLst.end() )
		{
			SBatEventInfo tempevent = itr->second;

			if ( tempevent.eventType == BSEVENT_ENDBATTLE )
			{
				m_pOwner->DoSetEndBatTurn();
				return BOOLEAN_TRUE;
			}
		}
	}

	return BOOLEAN_FALSE;
}

Boolean CBatScriptSystem::MobCmdEventHandle(CBatActor * pActor)
{
	if ( m_pOwner && pActor )
	{
		BatScriptEvtLst::iterator itr = m_BatScriptEventLst.find(m_pOwner->m_curTurn);
		if ( itr != m_BatScriptEventLst.end() )
		{
			SBatEventInfo tempevent = itr->second;
			if ( tempevent.eventType == BSEVENT_SURRENDER )
			{
				pActor->InputCommand(BATCMD_ESCAPE, pActor->GetIndex(), 0, 0);
				return BOOLEAN_TRUE;
			}
			else if ( tempevent.eventType == BSEVENT_ENDBATTLE )
			{
				pActor->InputCommand(BATCMD_EVENT, pActor->GetIndex(), 0, 0);
				return BOOLEAN_TRUE;
			}
			else if ( tempevent.eventType == BSEVENT_SKIPTURN )
			{
				pActor->InputCommand(BATCMD_EVENT, pActor->GetIndex(), 0, 0);
				return BOOLEAN_TRUE;
			}
		}
	}
	return BOOLEAN_FALSE;
}

void CBatScriptSystem::LuaTestSetAttri(CBatActor* pActor, int type, int value)
{
	if ( m_pOwner && pActor ) 
	{
		m_pOwner->DoTestChangeAttri(pActor, type, value);
	}
}

void CBatScriptSystem::LuaAddMob(UInt32 modId, int batPos)
{
	if ( m_pOwner )
	{
		m_pOwner->DoAddMob(modId, batPos);
	}
}

UInt32 CBatScriptSystem::LuaHasMob(int batPos)
{
	if ( m_pOwner )
	{
		return m_pOwner->DoGetBatPos(batPos);
	}
	return 0;
}

int CBatScriptSystem::LuaGetFreeSlot(PCSTRINGA szUnitID, int bagIndex)
{
	if ( !m_pOwner || !m_pBattleLua || !szUnitID ) return 0;

	Int teamIndex = Change2TeamIndex(szUnitID);
	if ( teamIndex != -1 )
	{
		return m_pOwner->DoGetFreeSlot(teamIndex, bagIndex);
	}
	return 0;
}

bool CBatScriptSystem::LuaNoMobExist()
{
	if ( m_pOwner )
	{
		if (!(m_pOwner->DoCheckEnemyTeamExist()))
			return true;
	}
	return false;
}

void CBatScriptSystem::LuaNoEndBat()
{
	if ( m_pOwner )
	{
		m_pOwner->DoNoEndBat();
	}
}

Boolean CBatScriptSystem::IsSkipTurn()
{
	if ( m_pOwner )
	{
		BatScriptEvtLst::iterator itr = m_BatScriptEventLst.find(m_pOwner->m_curTurn);
		if ( itr != m_BatScriptEventLst.end() )
		{
			SBatEventInfo tempevent = itr->second;
			if ( tempevent.eventType == BSEVENT_SKIPTURN )
				return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}

VOID CBatScriptSystem::SetSkipEvent(UInt8 turn)
{
	if ( m_pOwner )
	{
		BatScriptEvtLst::iterator itr = m_BatScriptEventLst.find(turn);
		if ( itr == m_BatScriptEventLst.end() )
		{
			SBatEventInfo batevent;
			batevent.eventType = BSEVENT_SKIPTURN;

			m_BatScriptEventLst.insert(std::make_pair(turn, batevent));
		}
	}
}

Boolean CBatScriptSystem::HasEvent(UInt8 turn)
{
	if ( m_pOwner )
	{
		BatScriptEvtLst::iterator itr = m_BatScriptEventLst.find(turn);
		if ( itr != m_BatScriptEventLst.end() )
		{
			return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}

Boolean CBatScriptSystem::IsNoPunishment()
{
	return m_BatConfigure.m_bNoPunishment;
}

void CBatScriptSystem::LuaBatCmd(CBatActor* pActor, int cmdType)
{
	if ( !m_pOwner || !m_pBattleLua || !pActor ) return; 

	switch ( cmdType )
	{
	case BATCMD_ATTACK:
		{
			pActor->DefaultInput();

			/*
			function AIAction(battle, mob)

	local actorCount = battle:GetActorCount()
	print(string.format("Lua AIAction: mob_id %d, mob index %d" .. 
		", actorCount %d", mob:GetMob_id(), mob:GetIndex(), actorCount))
	local r = math.random(1, actorCount - 1)
	local index = mob:GetIndex()
	for i = 0, actorCount - 1 do
		local actor = battle:GetActor(math.fmod((index + r + i) , actorCount));
		if (actor:GetTeam() ~= mob:GetTeam() and actor:IsValidTarget()) then
			print(string.format("Lua InputCommand: attack target %d", 
				actor:GetIndex()))
			mob:InputCommand(LuaI.BATCMD_ATTACK, actor:GetIndex(), 0, 0)
			break
		end
	end
end
			*/
		}
		break;
	case BATCMD_DEFEND:
	case BATCMD_ESCAPE:
		pActor->InputCommand((UInt8)cmdType, pActor->GetIndex(), 0, 0);
		break;
	}
}


VOID CBatScriptSystem::DisEnableBatCmd()
{
	if ( m_BatConfigure.m_dwDisEnableCmd != 0 )
	{
		for (CBattle::BatClients::iterator it = m_pOwner->m_clients.begin(); it != m_pOwner->m_clients.end(); ++it)
		{
			BatClient &client = it->second;
			if (client.status != CBattle::CLIENT_BATTLE || client.pPeer == NULL || client.pBatChar == NULL)
				continue;

			m_pOwner->CB_OnBatDisenableCmd(client.pPeer, client.context, m_BatConfigure.m_dwDisEnableCmd, BOOLEAN_TRUE);
		}
	}
}

Boolean CBatScriptSystem::NeedToAdjustEscapeRate(CBatActor * pActor)
{
	if ( m_pOwner && pActor )
	{
		BatScriptEvtLst::iterator itr = m_BatScriptEventLst.find(m_pOwner->m_curTurn);
		if ( itr != m_BatScriptEventLst.end() )
		{
			SBatEventInfo tempevent = itr->second;
			if ( tempevent.eventType == BSEVENT_SURRENDER )
			{
				if (pActor->m_pMob && (pActor->m_pMob->GetOwner_id() == 0)) 
				{
					return BOOLEAN_TRUE;
				}
			}
		}
	}
	return BOOLEAN_FALSE;
}