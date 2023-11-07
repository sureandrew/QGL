//-- Common
#include "Common.h"
//-- Self
#include "BattleService/Global.h"
//-- Library
#include "Reuben/Platform/File.h"
#include "Stub/Logic/MapBattle.h"
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/BatCharacter.h"
#include "Stub/Logic/BatMob.h"
#include "BattleService/Configure.h"
#include "Resource/ServerResManager.h"
#include "Common/Procedure.h"
#include "Common/CallLuaFunc.h"
#include "Common/BattleCode.h"
#include "Resource/ResSkill.h"
#include "Resource/ResBuff.h"
#include "Resource/ResItem.h"
#include "BattleService/AnimTimer.h"

#define FILENAME_LENGTH				1024
#define CONFIGURATION_INI_FILE		_T(".\\Data\\Setting\\fileconfig.ini")
#define LUA_INIT_FILE				_T(".\\Data\\AIScript")


#pragma warning(disable : 4099 4806)			// disable linking warning

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
	int Luai_Init(lua_State* L);		// declare the wrapped module
}

VOID Session::Release(CONST SessionID &sid)
{
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _T("Session Deleted: session %s"), 
		sid.GetString().c_str());

	ProcedureManager::GetInstance().RemoveBySession(sid);
}

INLINE Boolean IsFileExist(const char* filename)
{
	DWORD attr = GetFileAttributesA(filename);
	return (attr != INVALID_FILE_ATTRIBUTES && attr != FILE_ATTRIBUTE_DIRECTORY);
}

Global::Global()
{
	pManagerPeer = NULL;
	pObjectService = NULL;
	pMapService = NULL;
	m_mobdb_header = NULL;
	m_resource = NULL;
	m_luaState = NULL;
	g_pParentBatLua = NULL;
	m_bCreateChild = BOOLEAN_FALSE;
	m_bExit = BOOLEAN_FALSE;
	m_animTimer = NULL;
}

Global::~Global()
{
}

Boolean Global::Initialize()
{
	m_rand.Randomize();
	m_resource = SafeCreateObject(CServerResManager);
	Int bReadNpcDat  = 0;
	Int bReadMobDat  = 0;
	Int bReadItemDat = 0;
	Int bReadSkillDat	= 0;
	Int bReadBuffDat	= 0;

	TRACE_INFODTL(GLOBAL_LOGGER, _T("Initialization: Reading Npc..."));
	bReadNpcDat = GetPrivateProfileInt(_T("ReadDataOption"), _T("ReadNpcDat"), 0, CONFIGURATION_INI_FILE);
	if( bReadNpcDat )
		m_resource->LoadNpcDAT();
	else
		m_resource->LoadNpcCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _T("Initialization: Reading Mob..."));
	bReadMobDat = GetPrivateProfileInt(_T("ReadDataOption"), _T("ReadMobDat"), 0, CONFIGURATION_INI_FILE);
	if( bReadMobDat )
		m_resource->LoadMobDAT();
	else
		m_resource->LoadMobCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _T("Initialization: Reading Item..."));
	bReadItemDat = GetPrivateProfileInt(_T("ReadDataOption"), _T("ReadItemDat"), 0, CONFIGURATION_INI_FILE);
	if( bReadItemDat )
		m_resource->LoadItemDAT();
	else
		m_resource->LoadItemCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _T("Initialization: Reading Skill..."));
	bReadSkillDat = GetPrivateProfileInt(_T("ReadDataOption"), _T("ReadSkillDat"), 0, CONFIGURATION_INI_FILE);
	if( bReadSkillDat )
		m_resource->LoadSkillDAT();
	else
		m_resource->LoadSkillCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _T("Initialization: Reading Buff..."));
	bReadBuffDat = GetPrivateProfileInt(_T("ReadDataOption"), _T("ReadBuffDat"), 0, CONFIGURATION_INI_FILE);
	if( bReadBuffDat )
		m_resource->LoadBuffDAT();
	else
		m_resource->LoadBuffCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _T("Initialization: Reading MobSkill..."));
	m_resource->LoadSkillCombinationCSV();

	TRACE_INFODTL(GLOBAL_LOGGER, _T("Initialization: Reading StartCharData..."));
	m_resource->LoadStartCharCSV();

	g_ComboRate = GetPrivateProfileInt(_T("BattleSetting"), _T("comboRate"), 10, CONFIGURATION_INI_FILE);

	g_MaxBattleTurn = GetPrivateProfileInt(_T("BattleSetting"), _T("maxBattleTurn"), 150, CONFIGURATION_INI_FILE);
	TRACE_INFODTL(GLOBAL_LOGGER, _T("Initialization: Reading CSV Item (Random sheet)..."));
		m_resource->LoadCSVItemTable();

	TRACE_INFODTL(GLOBAL_LOGGER, _T("Initialization: Reading Lua..."));
	// init lua library
	m_luaState = luaL_newstate();
	TRACE_ENSURE(m_luaState);
	luaopen_base(m_luaState);
	luaopen_string(m_luaState);
	luaopen_math(m_luaState);
	//luaopen_debug(m_luaState);
	Luai_Init(m_luaState);
	//RunLua(LUA_INIT_FILE);
	RunLua(BATTLE_LUA_INIT_FILE);
	
	String strPath;

	strPath.Format( _T("%s\\*.lua"), LUA_INIT_FILE);

	//Load Texture From Current Directory
	WIN32_FIND_DATA data = {0};
	HANDLE hFile = FindFirstFile(strPath.c_str(), &data);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			//TCHAR LuaWholePath[_MAX_PATH];
			String LuaWholePath;
			LuaWholePath.Format(_T("%s\\%s"), LUA_INIT_FILE, data.cFileName);
			StringA temp = LuaWholePath.c_str();
			RunLua(temp.c_str());
		}while (FindNextFile(hFile, &data));
			FindClose(hFile);
	}

	// Init Battle script lua
	InitParentBattleLua();

	// Load All Battle script lua file
	TRACE_INFODTL(GLOBAL_LOGGER, _T("Initialization: Load all battle script lua file..."));
	m_resource->LoadLuaData();
	
	//load all skill lua file
	strPath.Format( _T("%s*.lua"), String(SKILL_LUA_INIT).c_str());

	//Load Texture From Current Directory
	hFile = FindFirstFile(strPath.c_str(), &data);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			//TCHAR LuaWholePath[_MAX_PATH];
			String LuaWholePath;
			LuaWholePath.Format(_T("%s%s"), String(SKILL_LUA_INIT).c_str(), data.cFileName);
			StringA temp = LuaWholePath.c_str();
			RunLua(temp.c_str());
		}while (FindNextFile(hFile, &data));
			FindClose(hFile);
	}

	//load all buff lua file
	strPath.Format( _T("%s*.lua"), String(BUFF_LUA_INIT).c_str());

	//Load Texture From Current Directory
	hFile = FindFirstFile(strPath.c_str(), &data);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			//TCHAR LuaWholePath[_MAX_PATH];
			String LuaWholePath;
			LuaWholePath.Format(_T("%s%s"), String(BUFF_LUA_INIT).c_str(), data.cFileName);
			StringA temp = LuaWholePath.c_str();
			RunLua(temp.c_str());
		}while (FindNextFile(hFile, &data));
			FindClose(hFile);
	}

	strPath.Format( _T("%s*.lua"), String(ITEM_BATTLE_LUA_INIT).c_str());

	//Load Texture From Current Directory
	hFile = FindFirstFile(strPath.c_str(), &data);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			//TCHAR LuaWholePath[_MAX_PATH];
			String LuaWholePath;
			LuaWholePath.Format(_T("%s%s"), String(ITEM_BATTLE_LUA_INIT).c_str(), data.cFileName);
			StringA temp = LuaWholePath.c_str();
			RunLua(temp.c_str());
		}while (FindNextFile(hFile, &data));
			FindClose(hFile);
	}

	m_animTimer = SafeCreateObject(CAnimTimer);
	m_animTimer->Initialize();

	return (BOOLEAN_TRUE);
}

VOID Global::Uninitialize(VOID)
{
	m_animTimer->Uninitialize();
	SafeDeleteObject(m_animTimer);

	lua_close(m_luaState);				// close lua library

	// Release battle script lua 
	ReleaseParentBattleLua();		

	ClearDeadSession();

	// clear dead battles
	for (MapBattleList::iterator it = m_deadBattles.begin(); it != m_deadBattles.end(); ++it)
	{
		DeleteNetObject(*it);
	}
	m_deadBattles.clear();

	// clear all alive battles
	TRACE_INFODTL_1(GLOBAL_LOGGER, _T("Clear Alive Battles: %d"), m_aliveBattles.size());
	for (MapBattleList::iterator it = m_aliveBattles.begin(); it != m_aliveBattles.end(); ++it)
	{
		DeleteNetObject(*it);
	}
	m_aliveBattles.clear();

	DestroyGameContext();
}

void Global::DestroyGameContext()
{
	SafeDeleteObject(m_mobdb_header);
	for (MobDBMap::iterator itr1 = m_mobdb.begin(); itr1!=m_mobdb.end(); itr1++)
	{
		SafeDeleteObject(itr1->second->m_basicinfo);
		SafeDeleteObject(itr1->second);
	}
	m_mobdb.clear();
	SafeDeleteObject(m_resource);
}

Boolean Global::RunLua(const char* filename)
{
	if (filename == NULL)
		return false;

	int r = luaL_dofile(m_luaState, filename);

	if (r != 0)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _T("Error running Lua file: %s"), String(filename).c_str());
		TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(m_luaState, -1)).c_str());
		return false;
	}

	// test by hello world
	CallLuaFunc(m_luaState, "Hello");

	return true;
}

bool Global::IsValidTargetType(UInt8 targetType, CBatActor * pSrc, CBatActor * pTarget)
{
	if (pSrc == NULL ||
		pTarget == NULL)
		return false;

	bool ignoreTeam = pSrc->m_ignoreTeam;
	switch(targetType)
	{
	case TARGET_TYPE_ANY:
		break;
	case TARGET_TYPE_SELF:
		if (pSrc != pTarget)
			return false;
		break;
	case TARGET_TYPE_ANY_SAME_TEAM:
		if (!ignoreTeam && pSrc->GetTeam() != pTarget->GetTeam())
			return false;
		break;
	case TARGET_TYPE_ANY_ENEMY_TEAM:
		if (!ignoreTeam && pSrc->GetTeam() == pTarget->GetTeam())
			return false;
		break;
	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_MOB_PARNTER:
		if (!ignoreTeam && pSrc->GetTeam() == pTarget->GetTeam())
			return false;

		if (pTarget->IsParnter() ||
			pTarget->IsMob())
			return false;

		break;
	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_BOSS_PLAYER:
		if (!ignoreTeam && pSrc->GetTeam() == pTarget->GetTeam())
			return false;
		if (pTarget->IsBoss() ||
			pTarget->IsPlayer())
			return false;
		
		break;
	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_MOB_PLAYER_PARNTER:
		if (!ignoreTeam && pSrc->GetTeam() == pTarget->GetTeam())
			return false;
		if (pTarget->IsParnter() ||
			pTarget->IsPlayer() ||
			(pTarget->IsMob() && !pTarget->IsBoss()))
			return false;

			break;
	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_BOSS:
		{
			if (!ignoreTeam && pSrc->GetTeam() == pTarget->GetTeam())
				return false;
			if (pTarget->IsBoss())
				return false;
			break;
		}
	case TARGET_TYPE_ANY_PARNTER:
		{
			if (!pTarget->IsParnter())
				return false;
		}
		//pSrc->GetTeam() != pTarget->GetTeam())
		break;
	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_PLAYER_PARNTER:
		{
			if (!ignoreTeam && pSrc->GetTeam() == pTarget->GetTeam())
				return false;
			if (pTarget->IsParnter() ||
				pTarget->IsPlayer())
				return false;
		}	
		break;
	case TARGET_TYPE_ANY_PET:
		break;
	case TARGET_TYPE_ANY_SAME_TEAM_PLAYER:
		{
			if (!ignoreTeam && pSrc->GetTeam() != pTarget->GetTeam())
				return false;
			if (!pTarget->IsPlayer())
				return false;
			break;
		}
	case TARGET_TYPE_ANY_SAME_TEAM_PARNTER:
		{
			if (!ignoreTeam && pSrc->GetTeam() != pTarget->GetTeam())
				return false;
			if (!pTarget->IsParnter())
				return false;
			break;
		}
	case TARGET_TYPE_ANY_SAME_TEAM_PET:
		break;
	}

	return true;
}

LPCPEER GetMapPeer(ServiceID mapId)
{
	ServicePointerMap::iterator it = gGlobal.mapServices.find(mapId);

	if (it != gGlobal.mapServices.end())
		return (it->second->pPeer);
	else
		return (NULL);
}

StringA	Global::GetPrefixStr(UInt16 ID, StringA pre)
{
	StringA Result;
	Result.Format("%s%04i", pre.c_str(), ID);
	return Result;
}

const char* ConvAnsiString(const String& s)
{
	static char buf[1024];
	int len;

	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), (int)s.length(), 
			buf, 1024, 0, 0);
	if (len > 0)
		buf[len] = _T('\0');
	else
		buf[0] = _T('\0');

	return buf;
}

VOID Global::InitParentBattleLua()
{
	if ( g_pParentBatLua )
		ReleaseParentBattleLua();

	g_pParentBatLua = luaL_newstate();
	TRACE_ENSURE(g_pParentBatLua);
	luaopen_base(g_pParentBatLua);
	luaopen_table(g_pParentBatLua);
	luaopen_string(g_pParentBatLua);
	luaopen_math(g_pParentBatLua);
	
	Luai_Init(g_pParentBatLua);
}

VOID Global::ReleaseParentBattleLua()
{
	if ( g_pParentBatLua )
	{
		lua_close(g_pParentBatLua);
		g_pParentBatLua = NULL;
	}
}

lua_State * Global::CreateChildState(int &RefKey)
{
	if ( g_pParentBatLua && !m_bCreateChild )
	{
		m_bCreateChild = BOOLEAN_TRUE;
		//
		//printf("Before Create Child, Parent Do Collection Use Memory: %d\n", lua_gc( g_pParentBatLua, LUA_GCCOUNT, 0 ));
		lua_gc(g_pParentBatLua, LUA_GCCOLLECT, 0);
		//printf("After Create Child, Parent Do Collection Use Memory: %d\n", lua_gc( g_pParentBatLua, LUA_GCCOUNT, 0 ));
		//

		lua_State * ChildState = lua_newthread( g_pParentBatLua );

		lua_pushvalue( g_pParentBatLua, -1 ); // copy childstate and then push into stack

		RefKey = luaL_ref( g_pParentBatLua, LUA_REGISTRYINDEX );

		// copy parent state global table
		lua_newtable( g_pParentBatLua );
		lua_pushvalue( g_pParentBatLua, -1 );
		lua_setmetatable( g_pParentBatLua, -2 ); //Set itself as metatable

		lua_pushvalue( g_pParentBatLua, LUA_GLOBALSINDEX );

		lua_setfield( g_pParentBatLua, -2, "__index" );
		lua_setfenv( g_pParentBatLua, -2 );
		lua_pop( g_pParentBatLua, 1 );

		//
		//printf("After Create Child, Parent Use Memory: %d\n", lua_gc( g_pParentBatLua, LUA_GCCOUNT, 0 ));
		//

		m_bCreateChild = BOOLEAN_FALSE;

		return ChildState;
	}
	return NULL;
}
