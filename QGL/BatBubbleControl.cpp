//-- Common
#include "Common.h"
//-- Self
#include "BatBubbleControl.h"
//-- Library
#include "Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "BattleControl.h"
//#include "Proxy/Logic/BatMob.h"

#define SZ_DEFAULT_DIALOGUE_FOLDER		".\\Data\\Dialogue\\"
#define SZ_DEFAULT_ANIMATION_LUA		"DefaultAnimFuncC.lua"

const int BATTLE_BUBBLE_RESET_TIME = 3600000;//1000 * 60 * 60

CBatBubbleControl::CBatBubbleControl()
{
	m_pBubbleLua = NULL;
	
	Init();
	//gGlobal.InitBubbleLua(m_pBubbleLua);
}

CBatBubbleControl::~CBatBubbleControl()
{
	ClearAllDialogueLst();
	ReleaseBubbleLua();
}

VOID CBatBubbleControl::InitBubbleLua()
{
	gGlobal.InitUILua(m_pBubbleLua);
}

VOID CBatBubbleControl::ReleaseBubbleLua()
{
	gGlobal.ReleaseUILua(m_pBubbleLua);
}

VOID CBatBubbleControl::PrintLuaErr()
{
	if ( m_pBubbleLua )
	{
		const char* pErr = lua_tostring(m_pBubbleLua, -1);
		if ( pErr )
			printf("%s", pErr);
	}
}

VOID CBatBubbleControl::ResetParam()
{
	ClearMobIdLst();
	ClearAllDialogueLst();
}

VOID CBatBubbleControl::ClearEnemyBubbleLst()
{
	m_EnemyDialogueLst.clear();	
}

VOID CBatBubbleControl::ClearUserBubbleLst()
{
	for ( UserBubbleLst::iterator it = m_DialogueLst.begin(); it != m_DialogueLst.end(); ++it )
	{
		(it->second).clear();
	}
	m_DialogueLst.clear();
}

VOID CBatBubbleControl::ClearAllDialogueLst()
{
	ClearEnemyBubbleLst();
	ClearUserBubbleLst();
}

VOID CBatBubbleControl::Init()
{
	m_bCanRun = BOOLEAN_FALSE;
	m_BatMobIdLst.clear();
	ClearAllDialogueLst();
	InitBubbleLua();
	m_ResetTime = ::timeGetTime();
	if ( LoadDefaultBubbleFunc() )
		m_bLoadDefault = BOOLEAN_TRUE;
	else
		m_bLoadDefault = BOOLEAN_FALSE;
}

Boolean CBatBubbleControl::IsTimeToResetLua()
{
	UInt32 curr = ::timeGetTime();
	if ( ( curr - m_ResetTime ) < BATTLE_BUBBLE_RESET_TIME ) 
		return BOOLEAN_FALSE;
	m_ResetTime = curr;
	return BOOLEAN_TRUE;
}

VOID CBatBubbleControl::ShowUserBubble(Int posIndex, Int msgType)
{
	if ( m_DialogueLst.empty() || !m_bCanRun ) return;

	if ( !m_pBubbleLua || !gGlobal.g_pBattleControl ) return;

	if ( posIndex > (Int)(gGlobal.g_pBattleControl->m_BattlePlayerLst.size() ) ) return;

	{
		Int selfpos = gGlobal.g_pBattleControl->CalcFinalBatPosition(gGlobal.g_pBattleControl->m_BattlePlayerLst[posIndex].pChar);

		UInt32 keyindex = (((UInt32)msgType) << 16) + (UInt32)selfpos;

		UserBubbleLst::iterator find = m_DialogueLst.find(keyindex);
		if ( find != m_DialogueLst.end() )
		{
			String name = gGlobal.g_pBattleControl->m_BattlePlayerLst[posIndex].pChar->GetCharName();

			for ( StringVector::iterator itr = find->second.begin(); itr != find->second.end(); ++itr )
			{
				if ( (name.CompareNoCase(_T("")) != 0) )
				{					
					String bubble;
					bubble.Format(_T("[Pos:%02d] : %s"), selfpos, (*itr).c_str());
					gGlobal.AddChatBalloon(bubble, CHANNELTYPE_MAP, name, posIndex);
				}
			}
		}
	}
}

VOID CBatBubbleControl::ShowEnemyBubble(Int posIndex, Int msgType)
{
	if ( m_EnemyDialogueLst.empty() || !m_bCanRun ) return;

	if ( !m_pBubbleLua || !gGlobal.g_pBattleControl ) return;

	if ( posIndex > (Int)(gGlobal.g_pBattleControl->m_BattlePlayerLst.size() ) ) return;

	UInt32 mobId = gGlobal.g_pBattleControl->GetBatMobIdByIndex(posIndex);
	if ( mobId != 0 )
	{
		UInt64 keyIndex = (((UInt64)msgType) << 32) + (UInt64)mobId;

		EnemyBubbleLst::iterator find = m_EnemyDialogueLst.find(keyIndex);
		if ( find != m_EnemyDialogueLst.end() )
		{
			String name = gGlobal.g_pBattleControl->m_BattlePlayerLst[posIndex].pChar->GetCharName();
			if ( (name.CompareNoCase(_T("")) != 0) )
			{
				int per = find->second.Percentage;
				if ( (per != 0 ) && ( (rand() % 100) <= per ) )
				{
					String bubble = find->second.Msg;
					//gGlobal.AddChatBalloon(bubble, CHANNELTYPE_MAP, name, posIndex);
					if ( msgType == BATBUBBLE_CATCH )
						gGlobal.AddChatBalloon(bubble, CHANNELTYPE_MAP, name, posIndex, 2000);
					else
						gGlobal.AddChatBalloon(bubble, CHANNELTYPE_MAP, name, posIndex);
				}
			}
		}
	}
}

Boolean CBatBubbleControl::LoadDefaultBubbleFunc()
{
	if ( !m_pBubbleLua ) return BOOLEAN_FALSE;

	StringA defaultfile;
	defaultfile.Format("%s%s", SZ_DEFAULT_DIALOGUE_FOLDER, SZ_DEFAULT_ANIMATION_LUA);

	int nRes = luaL_dofile(m_pBubbleLua, defaultfile.c_str()); 
	if ( nRes != 0 )
	{
		PrintLuaErr();
		return BOOLEAN_FALSE;
	}
	
	return BOOLEAN_TRUE;
}

Boolean CBatBubbleControl::LoadUserBubbleLua(UInt32 npcId)
{
	if ( !m_pBubbleLua || !m_bLoadDefault ) return BOOLEAN_FALSE;

	ClearAllDialogueLst();

	NPCDB_Client* pNpcTemplate = gGlobal.LoadNpcTemplate(npcId);
	if ( !pNpcTemplate ) return BOOLEAN_FALSE;

	StringA filename(pNpcTemplate->m_basicinfo->m_npc_dialoguectrl);
	if ( filename.CompareNoCase("") != 0 )
	{
		StringA szpath;
		szpath.Format("%s%s", SZ_DEFAULT_DIALOGUE_FOLDER, filename.c_str());
		//szpath = ".\\Data\\Dialogue\\bubble.lua";

		if ( szpath.CompareNoCase("") != 0 )
		{
			int nRes = luaL_dofile(m_pBubbleLua, szpath.c_str()); 
			if ( nRes != 0 )
			{
				PrintLuaErr();
			}
			else
			{
				CallLuaFunc(m_pBubbleLua, "RunBatEnemyAction", String(), this);

				return BOOLEAN_TRUE;
			}
		}
	}
	
	return BOOLEAN_FALSE;

	
}

VOID CBatBubbleControl::RunUserBubbleLua()
{
	if ( m_pBubbleLua && m_bCanRun && m_bLoadDefault )
	{
		ClearUserBubbleLst();
		//ClearAllDialogueLst();
		CallLuaFunc(m_pBubbleLua, "RunBatActionOnTurn", String(), this);
	}
}

VOID CBatBubbleControl::ResetBatBubbleLua(UInt32 npcId)
{
	m_bCanRun = BOOLEAN_FALSE;
	InitBubbleLua();

	if ( LoadDefaultBubbleFunc() )
	{
		m_bLoadDefault = BOOLEAN_TRUE;
		if ( LoadUserBubbleLua(npcId) )
			m_bCanRun = BOOLEAN_TRUE;
	}
	else
		m_bLoadDefault = BOOLEAN_FALSE;
}

Int32 CBatBubbleControl::UTF82StringW(StringW &Str, PCSTRINGA Text)
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

Int32 CBatBubbleControl::StringW2UTF8(StringA &Des, PCSTRINGW Src)
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

VOID CBatBubbleControl::StartUp(UInt32 npcId)
{
	if ( npcId == 0 )
	{
		m_bCanRun = BOOLEAN_FALSE;
	}
	else
	{
		if ( IsTimeToResetLua() )
		{
			ResetBatBubbleLua( npcId );
		}
		else
		{
			if ( LoadUserBubbleLua(npcId) )
				m_bCanRun = BOOLEAN_TRUE;
			else
				m_bCanRun = BOOLEAN_FALSE;
		}
	}
}

void CBatBubbleControl::LuaAddBatBubble(Int pos, Int msgType, PCSTRINGA text)
{
	if ( !m_pBubbleLua || !text ) return ;

	if ( (msgType >= BATBUBBLE_IDLE) && (msgType < MAX_BATBUBBLE_COUNT) )
	{
		StringW szMsg(_T(""));
		if ( UTF82StringW(szMsg, text) > 0 )
		{
			UInt32 keyIndex = (((UInt32)(msgType)) << 16 ) + pos;
			UserBubbleLst::iterator find = m_DialogueLst.find(keyIndex);
			if ( find != m_DialogueLst.end() )
			{
				find->second.push_back(szMsg);
			}
			else
			{
				StringVector msgLst;
				msgLst.clear();
				msgLst.push_back(szMsg);

				m_DialogueLst.insert(std::make_pair(keyIndex, msgLst));
			}
		}
	}
}

VOID CBatBubbleControl::AddMobId(UInt32 mobId)
{
	if ( mobId == 0 ) return;
	UInt32Vector::iterator itr = std::find(m_BatMobIdLst.begin(), m_BatMobIdLst.end(), mobId);
	if ( itr == m_BatMobIdLst.end() )
	{
		m_BatMobIdLst.push_back(mobId);
	}
}

VOID CBatBubbleControl::ClearMobIdLst()
{
	m_bCanRun = BOOLEAN_FALSE;
	m_BatMobIdLst.clear();
}

int CBatBubbleControl::LuaGetMobIdCount()
{
	if ( m_BatMobIdLst.empty() ) return 0;

	return (int)(m_BatMobIdLst.size());
}

UInt32 CBatBubbleControl::LuaGetMobId(int index)
{
	if ( index < (int)(m_BatMobIdLst.size()) )
		return m_BatMobIdLst[index];
	return 0;
}

PCSTRINGA CBatBubbleControl::LuaGetMobDialgoueFile(int index)
{
	if ( index < (int)(m_BatMobIdLst.size()) )
	{
		UInt32 mobId = m_BatMobIdLst[index];

		const MobDB_Client* pMobTemplate = gGlobal.LoadMobTemplate(mobId);
		if ( pMobTemplate )
		{
			StringA filename(pMobTemplate->m_basicinfo->mob_dialogue);
			if ( filename.CompareNoCase("") != 0 )
			{
				StringA szpath;
				szpath.Format("%s%s", SZ_DEFAULT_DIALOGUE_FOLDER, filename.c_str());
				//szpath.Format("%s%s", SZ_DEFAULT_DIALOGUE_FOLDER, "mobdialogue.lua");

				m_szC2LuaBuff = szpath;
				return m_szC2LuaBuff.c_str(); 
				//if ( StringW2UTF8(m_szC2LuaBuff, filename.c_str()) > 0 )
				//		return m_szC2LuaBuff.c_str();  
			}
		}
	}
	return NULL;
}

void CBatBubbleControl::LuaAddEnemyDialogue(UInt32 mobId, int msgType, PCSTRINGA text, int per)
{
	if ( !m_pBubbleLua || !text ) return;

	if ( (msgType >= BATBUBBLE_IDLE) && (msgType < MAX_BATBUBBLE_COUNT) )
	{
		StringW szMsg(_T(""));
		if ( UTF82StringW(szMsg, text) > 0 )
		{
			UInt64 keyIndex = (((UInt64)(msgType)) << 32 ) + mobId;

			EnemyBubbleLst::iterator find = m_EnemyDialogueLst.find(keyIndex);
			if ( find != m_EnemyDialogueLst.end() )
			{
				find->second.Msg = szMsg;
				find->second.Percentage = per;
			}
			else
			{
				BatBubbleMsg bubble;
				bubble.Msg = szMsg;
				bubble.Percentage = per;

				m_EnemyDialogueLst.insert(std::make_pair(keyIndex, bubble));
			}
		}
	}
}
