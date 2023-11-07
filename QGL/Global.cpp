//-- Common
#include "Common.h"
#include "QGL/QGL.h"
//-- Self
#include "QGL/Global.h"
//-- Library
#include "Common/ChannelType.h"
#include "Proxy/Logic/Battle.h"
#include "Proxy/Logic/BatCharacter.h"
#include "Proxy/Logic/BatMob.h"
#include "Proxy/Logic/Party.h"
#include "Proxy/Logic/Channel.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/MessageCharacter.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/ScriptSystem.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Proxy/Logic/QuestSystem.h"
#include "Proxy/Logic/Guild.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResItem.h"
#include "Resource/ResPet.h"
#include "QGL/MapControl.h"
#include "QGL/CharacterControl.h"
#include "QGL/ChannelControl.h"
#include "QGL/Math.h"
#include "QGL/BattleControl.h"
#include "QGL/Configure.h"
#include "QGL/AccountControl.h"
#include "QGL/QGLApplication.h"
#include "ScriptControl.h"
#include "BatScriptControl.h"
#include "QGL/CallLuaFunc.h"
//#include "Common/CallLuaFunc.h"
#include "Common/PathFinder.h"
#include <Reuben/Platform/File.h>
#include "QGL/UIEntity.h"
#include "MenuItemProcess.h"
#include "MenuPartyProcess.h"
#include "CreateCharacterProcess.h"
#include "MessageBox.h"
#include "MiddleMap.h"
#include "MenuCharacterSimulator.h"
#include "MenuPartnerProcess.h"
#include "MenuJoinBattleProcess.h"
#include "MenuSkillProcess.h"
#include "MenuShopProcess.h"
#include "MenuCharProcess.h" 
#include "MenuSearchListProcess.h"
#include "MenuQuestLogProcess.h"
#include "MenuGivenProcess.h"
#include "Common/ExportVersion.h"
#include "Resource/resskill.h"
#include "Resource/resbuff.h"
#include "RumorMsgProcess.h"
#include "DirtyRectManager.h"
#include "BigMap.h"
#include "BatBubbleControl.h"
#include "MenuTargetProcess.h"
#include "MenuBuffIconProcess.h"
#include "NewBieHintCtrl.h"
#include "MenuSystemSetting.h"
#include "MenuTradingProcess.h"
#include "MenuFriendProcess.h"
#include "MenuHelpProcess.h"
#include "MenuOnlineShopProcess.h"
#include "MenuRankProcess.h"
#include "MenuPetProcess.h"
#include "MenuMixProcess.h"
#include "MenuTitleProcess.h"
#include "AntiHack.h"
#include "MenuHawkerProcess.h"
#include "Proxy/Logic/HawkGroup.h"
#include "Proxy/Logic/PetGroup.h"
#include "Proxy/Logic/Pet.h"
#include "MenuGuildProcess.h"
#include "MenuBankService.h"
#include "Proxy/Logic/GiftBox.h"
#include "MenuGiftBox.h"
#include "MenuQuestJournal.h"
#include "WeaponsRank.h"
#include "TeamDescription.h"
#include "MenuWareHouseProcess.h"
#include "MenuAutoWalkProcess.h"
#include "MenuSellYuanBaoProcess.h"
#include "MenuTargetEquipProcess.h"
#include "Proxy/Logic/SellYuanBaoGroup.h"
#include "Resource/ResLineup.h"
#include "GenerationProcess.h"
#include "MenuPartnerCultivationUpProcess.h"
#include "AccShopProcess.h"
#include "PointsShopProcess.h"
#include "Resource/ResAutoPath.h"
#include "Resource/AutoPath.h"
#include "CrossMapPathFinder.h"
#include "Proxy/Logic/PartyCharInfoClient.h"

#define FILENAME_LENGTH				1024

#define MAX_PARTY_NUM 5
#define FILENAME_LENGTH 1024
#define MAX_ICON	256
#define MINUTE_TIME				60000
#define SECOND_TIME				1000

#define MAX_SPEAKER_WIDTH	373
#define MIN_SPEAKER_WIDTH	20
#define SPEAKER_OFFSET		4
#define MAXSHOTNUM			9

#define GAMESTRINGTABLE_LOCALIZATION_FILENAME _T("Data/localization.csv")
typedef StlDeque<BlockPos, ManagedAllocator<BlockPos> > PointList;

const WORD TILE_IN_NORMAL_VALUE = 0x0000;
const WORD TILE_IN_BLOCK_VALUE = 0x0001;
const WORD TILE_IN_ALPHA_VALUE  = 0x0002;
const WORD TILE_IN_SHADOW_VALUE = 0x0004;
const WORD TILE_IN_SEADEPTH_VALUE = 0xFF00;
const WORD TILE_IN_FOOTPRINT_VALUE = 0x0010;
const WORD TILE_IN_NAVIGATION_VALUE = 0x0020;
const UInt MapChannelMinDis = 800;

void InitRecentChatListBox();

const Int HorseLightRemainTime = 60000;

#define SKILL_LUA_INIT				".\\Data\\Skill\\"
#define BUFF_LUA_INIT				".\\Data\\Buff\\"
#define ITEM_BATTLE_LUA_INIT		".\\Data\\ItemBattle\\"
#define LUA_SRC_EXT					".lua"
#define LUA_BIN_EXT					".out"


extern "C"
{
	#include <lualib.h>
	#include <lauxlib.h>
	int luaopen_CommonI(lua_State* L);		// declare the CommonI wrapped module
	int luaopen_LuaUI(lua_State* L);		// declare the UI wrapped module
	int luaopen_LuaI(lua_State* L);			// declare the LuaI wrapped module
}

UIManager* GetUIManager(VOID)
{
	return (gGlobal.pUIManager);
}

enum WEBLINK_TYPE
{
	WEBLINK_TYPE_WEB = 0,
	WEBLINK_TYPE_CHARGE,
	WEBLINK_TYPE_TRANSFER,
	WEBLINK_TYPE_CS,
};

Boolean Global::Initialize()
{
	int readPatch = GetPrivateProfileInt(_T("PATCHDATA"), _T("ReadPatch"), 0, CONFIGURATION_INI_FILE);
	if( readPatch )
	{
		g_GdfResourceEx = SafeCreateObject(GDFManager);
		String gdfDataExFile = GDF_DATAEX_FILE;
		if( !g_GdfResourceEx->LoadPatchFile(gdfDataExFile, GDFFILE_STATE_GAME) )
		{
			SafeDeleteObject(g_GdfResourceEx);
			g_GdfResourceEx = NULL;
		}
		g_GdfResource = SafeCreateObject(GDFManager);
		String gdfDataFile = GDF_DATA_FILE;
		if( !g_GdfResource->LoadPatchFile(gdfDataFile, GDFFILE_STATE_GAME) )
		{
			SafeDeleteObject(g_GdfResource);
			g_GdfResource = NULL;
		}
	}

	m_pStringTable = SafeCreateObject(CGameStringTable);
	if( g_GdfResource || g_GdfResourceEx )
	{
		String gamestring = GAMESTRINGTABLE_LOCALIZATION_FILENAME;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(gamestring, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(gamestring, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_pStringTable->LoadFromBuffer(buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
		m_pStringTable->Load(GAMESTRINGTABLE_LOCALIZATION_FILENAME);
		

	m_pBag = NULL;

	m_bgmVolume = GetPrivateProfileInt(_T("SOUND"), _T("DefaultBGMVolume"), 2, CONFIGURATION_INI_FILE);
	m_sfxVolume = GetPrivateProfileInt(_T("SOUND"), _T("DefaultSFXVolume"), 2, CONFIGURATION_INI_FILE);

	g_pLoginProcess = SafeCreateObject(CLoginProcess);

	InitClientData();
	m_MapChannelLst.clear();

	InitChannelInfo();
	m_resource = SafeCreateObject2(CClientResManager, g_GdfResource, g_GdfResourceEx);

	g_pItemProcess = SafeCreateObject(CMenuItemProcess);
	g_pPartyProcess = SafeCreateObject(CMenuPartyProcess);
	g_pCreateCharProcess = SafeCreateObject(CCreateCharacterProcess);
	g_pMiddleMap = SafeCreateObject(CMiddleMap);
	g_pPartnerProcess = SafeCreateObject(CMenuPartnerProcess);
	g_pSkillProcess = SafeCreateObject(CMenuSkillProcess);

	g_pShopProcess = SafeCreateObject(CMenuShopProcess);

	g_pMenuCharSimulator = SafeCreateObject(CMenuCharacterSimulator);
	g_pSearchListProcess = SafeCreateObject(CMenuSearchListProcess);

	g_pQuestLogProcess = SafeCreateObject(CMenuQuestLogProcess);

	g_pRumorMsgProcess = SafeCreateObject(CRumorMsgProcess);
	g_pCharProcess = SafeCreateObject(CMenuCharProcess);
	g_pGivenProcess = SafeCreateObject(CMenuGivenProcess);
	g_pBigMap = SafeCreateObject(CBigMap);
	g_MenuTargetProcess = SafeCreateObject(CMenuTargetProcess);
	g_pMenuBuffIconProcess = SafeCreateObject(CMenuBuffIconProcess);
	g_pGiftBoxProcess = SafeCreateObject(CMenuGiftBox);

	g_pNewBieHintCtrl = SafeCreateObject(CNewBieHintCtrl);
	g_pSystemSetting = SafeCreateObject(CMenuSystemSetting);
	g_pSystemSetting->ReadSystemSetting();
	g_pTradingProcess = SafeCreateObject(CMenuTradingProcess);

	g_pMenuFriendProcess = SafeCreateObject(CMenuFriendProcess);
	g_pMenuHelpProcess = SafeCreateObject(CMenuHelpProcess);
	g_pOnlineShopProcess = SafeCreateObject(CMenuOnlineShopProcess);
	g_pMenuRankProcess = SafeCreateObject(CMenuRankProcess);
	g_pScriptControl = SafeCreateObject(CScriptControl);
	g_pMenuPetProcess = SafeCreateObject(CMenuPetProcess);
	g_pMenuMixProcess = SafeCreateObject(CMenuMixProcess);
	g_pMenuTitleProcess = SafeCreateObject(CMenuTitleProcess);
	g_pAntiHack = SafeCreateObject(CAntiHack);
	g_pMenuGuildProcess = SafeCreateObject(CMenuGuildProcess);
	g_pMenuBankService = SafeCreateObject(CMenuBankService);

	g_pHawkerProcess = SafeCreateObject(CMenuHawkerProcess);
	g_pMenuQuestJournal = SafeCreateObject(CMenuQuestJournal);

	g_pWeaponsRank = SafeCreateObject(CWeaponsRank);

	g_pTeamDescription = SafeCreateObject(CTeamDescription);

	g_pGenerationProcess = SafeCreateObject(CGenerationProcess);
	g_pWareHouseProcess = SafeCreateObject(CMenuWareHouseProcess);
	g_pAutoWalkProcess = SafeCreateObject(CMenuAutoWalkProcess);
	g_pMenuSellYBProcess = SafeCreateObject(CMenuSellYuanBaoProcess);
	g_pTargetEquipProcess = SafeCreateObject(CMenuTargetEquipProcess);
	g_pPartnerCultivationUpProcess = SafeCreateObject(CMenuPartnerCultivationUpProcess);
	g_pAccShopProcess = SafeCreateObject(CAccShopProcess);
	g_pPointsShopProcess = SafeCreateObject(CPointsShopProcess);

	g_pScriptControl->Init();
	g_pTargetHawkGroup = NULL;
	g_pMyHawkGroup = NULL;
	g_pGiftBox = NULL;
	//g_pHawkerHistory = NULL;

	g_bOpenSubWindow = false;
	g_bCtrlButtonUp = true;

	g_pSellYuanBaoGroup = NULL;


	//m_resource->LoadNpcDAT();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Client Map Info..."));
	m_resource->LoadClientMapDAT();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Item..."));
	m_resource->LoadItemCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Skill..."));
	m_resource->LoadSkillCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Start Character..."));
	m_resource->LoadStartCharCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Buff..."));
	m_resource->LoadBuffCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading SkillShop..."));
	m_resource->LoadSkillShopCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading BigMap..."));
	m_resource->LoadBigMapCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading BlockWordList..."));
	m_resource->LoadBlockWordCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading MobDescList..."));
	m_resource->LoadMobDescCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Newbie..."));
	m_resource->LoadNewBieCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Pet..."));
	m_resource->LoadPetCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading WeaponsRank..."));
	m_resource->LoadWeaponsRankDataCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Newbietips..."));
	m_resource->LoadNewBieTipsCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading NewGuide..."));
	m_resource->LoadNewGuideCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading MobSkill..."));
	m_resource->LoadSkillCombinationCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Mix Shop..."));
	m_resource->LoadMixShopCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Title..."));
	m_resource->LoadTitleCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Faction..."));
	m_resource->LoadFactionsCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading PartnerCollection..."));
	m_resource->LoadPartnerCollectionCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading ItemSet..."));
	m_resource->LoadItemSetCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Lineup..."));
	m_resource->LoadLineupCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading Manor..."));
	m_resource->LoadManorCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading AccShopItems..."));
	m_resource->LoadAccShopItemCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading AutoPath..."));
	m_resource->LoadAutoPathCSV();
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Initialization: Reading PointsShopItem..."));
	m_resource->LoadPointsShopItemCSV();

	g_pMenuHelpProcess->ConstructData();
	m_autoX = 0;	m_autoY = 0;

	LeftClickStatus = LEFTCLICK_NORMAL;

	g_pJoinBattleProcess = SafeCreateObject(CMenuJoinBattleProcess);
	for (Int i = 0; i < 256; i ++)
	{
		for (Int j = 0; j < 256; j++)
		{
			Int result = i + j;
			if (result > 255)
				result = 255;
			AddtitiveTable[i][j] = (Byte)result;

			result = i - j;
			if (result < 0)
				result = 0;
			SubtractiveTable[i][j] = result;

			BYTE Result = (i * ( 255 - j)) / 255;
			/*if (Result)
			{
			Result ++;
			}*/
			AlphaOffsetTable[i][j] = Result;
		}
	}

	for (Int i = 0; i < 4; i ++)
	{
		for (Int j = 0; j < 256; j ++)
		{
			for (Int k = 0; k < 256; k ++)
			{
				if (i == 0)
					BgDDSTable1[i][j][k] = j;
				else if (i == 1)
					BgDDSTable1[i][j][k] = k;
				else if (i == 2)
					BgDDSTable1[i][j][k] = j * 2/ 3 + k / 3;
				else if (i == 3)
					BgDDSTable1[i][j][k] = j / 3 + k * 2 / 3;

				if (i == 0)
					BgDDSTable2[i][j][k] = j;
				else if (i == 1)
					BgDDSTable2[i][j][k] = k;
				else if (i == 2)
					BgDDSTable2[i][j][k] = j / 2 + k / 2;
				else if (i == 3)
					BgDDSTable2[i][j][k] = 0;

			}
		}
	}

	m_rand.Randomize();

	InitParentLua();

	//load all skill lua file
	SkillDataList skillList;
	m_resource->GetSkillList(skillList);
	for (SkillDataList::iterator it = skillList.begin(); it != skillList.end(); ++it)
	{
		StringA Name;
		Name.Format("%s%sC.lua", SKILL_LUA_INIT, GetPrefixStr((*it)->id, "skill"));
		LoadLuaToParent(Name.c_str());
	}

	BuffDataList buffList;
	m_resource->GetBuffList(buffList);
	for (BuffDataList::iterator it = buffList.begin(); it != buffList.end(); ++it)
	{
		StringA Name;
		Name.Format("%s%sC.lua", BUFF_LUA_INIT, GetPrefixStr((*it)->id, "buff"));
		LoadLuaToParent(Name.c_str());
	}

	StringA Name;
	Name.Format("%sGlobalBuffC.lua", BUFF_LUA_INIT);
	LoadLuaToParent(Name.c_str());

	LoadLuaToParent("Data\\GlobalLua.lua");

	g_HorseLightCount = 0;
	g_HorseLightDisplay = false;

	g_AllDirtyRectMan = SafeCreateObject(CDirtyRectManager);

	m_logined = LOGIN_WAITING_CHANGESTATE;
	g_pRobotLua = NULL;
	m_SpeakerLst.clear();

	g_mapPartyChar.clear();

	return (BOOLEAN_TRUE);
}

VOID Global::Uninitialize()
{
	DestroyGameContext();

	if (g_pTargetHawkGroup)
	{
		DeleteNetGroup(g_pTargetHawkGroup, BOOLEAN_TRUE);
		g_pTargetHawkGroup = NULL;
	}
	if (g_pMyHawkGroup)
	{
		DeleteNetGroup(g_pMyHawkGroup, BOOLEAN_TRUE);
		g_pMyHawkGroup = NULL;
	}
	if (g_pGiftBox)
	{
		DeleteNetGroup(g_pGiftBox, BOOLEAN_TRUE);
		g_pGiftBox = NULL;
	}	
	if (g_pSellYuanBaoGroup)
	{
		DeleteNetGroup(g_pSellYuanBaoGroup, BOOLEAN_TRUE);
		g_pSellYuanBaoGroup = NULL;
	}
	/*if (g_pHawkerHistory)
	{
		DeleteNetGroup(g_pHawkerHistory, BOOLEAN_TRUE);
		g_pHawkerHistory = NULL;
	}*/

	SafeDeleteObject(g_PathFinder);
	SafeDeleteObject(m_resource);
	SafeDeleteObject(g_pItemProcess);
	SafeDeleteObject(g_pPartyProcess);
	SafeDeleteObject(g_pCreateCharProcess);
	SafeDeleteObject(g_pMiddleMap);
	if( g_GdfResourceEx )
		SafeDeleteObject(g_GdfResourceEx);
	if( g_GdfResource )
		SafeDeleteObject(g_GdfResource);
	SafeDeleteObject(g_pMenuCharSimulator);
	SafeDeleteObject(g_pPartnerProcess);
	SafeDeleteObject(g_pJoinBattleProcess);
	SafeDeleteObject(g_pSkillProcess);
	SafeDeleteObject(g_pShopProcess);
	SafeDeleteObject(g_pSearchListProcess);
	SafeDeleteObject(g_pQuestLogProcess);
	SafeDeleteObject(g_pRumorMsgProcess);
	SafeDeleteObject(g_pLoginProcess);
	SafeDeleteObject(g_pCharProcess);
	SafeDeleteObject(g_pGivenProcess);
	SafeDeleteObject(g_AllDirtyRectMan);
	SafeDeleteObject(g_pBigMap);
	SafeDeleteObject(g_MenuTargetProcess);
	SafeDeleteObject(g_pMenuBuffIconProcess);
	SafeDeleteObject(g_pNewBieHintCtrl);
	SafeDeleteObject(g_pSystemSetting);
	SafeDeleteObject(g_pTradingProcess);
	SafeDeleteObject(g_pMenuFriendProcess);
	SafeDeleteObject(g_pMenuHelpProcess);
	SafeDeleteObject(g_pOnlineShopProcess);
	SafeDeleteObject(g_pMenuRankProcess);
	SafeDeleteObject(g_pMenuPetProcess);
	SafeDeleteObject(g_pMenuMixProcess);
	SafeDeleteObject(g_pMenuTitleProcess);
	SafeDeleteObject(g_pAntiHack);
	SafeDeleteObject(g_pMenuGuildProcess);

	SafeDeleteObject(g_pMenuBankService);	
	SafeDeleteObject(g_pHawkerProcess);
	SafeDeleteObject(g_pGiftBoxProcess);
	DeleteNetGroup(g_pPartnerGroup, BOOLEAN_TRUE);
	SafeDeleteObject(g_pMenuQuestJournal);
	SafeDeleteObject(g_pWeaponsRank);
	SafeDeleteObject(g_pTeamDescription);
	SafeDeleteObject(g_pGenerationProcess);
	SafeDeleteObject(g_pWareHouseProcess);
	SafeDeleteObject(g_pAutoWalkProcess);
	SafeDeleteObject(g_pMenuSellYBProcess);
	SafeDeleteObject(g_pTargetEquipProcess);
	SafeDeleteObject(g_pPartnerCultivationUpProcess);
	SafeDeleteObject(g_pAccShopProcess);
	SafeDeleteObject(g_pPointsShopProcess);

	ReleaseParentLua();

	SafeDeleteObject(m_pStringTable);

	if (g_pMapView)
		SafeDeleteObject(g_pMapView);

	m_CurrMusicFilename.Empty();

	g_HorseLightMsgLst.clear();

	g_HorseLightCountLst.clear();

	//g_pastInputMsgHis.clear();

	m_LineExist.clear();
	m_lineState.clear();

	TempMsgForUI.Empty();

	g_RecentPriChatChar.Empty();

	g_RecentPriChatCharsLst.clear();

	g_TempSkillString.Empty();

	g_EmotionList.clear();
	g_PKRequestBox.FreeString();
	g_QuestCancelBox.FreeString();
	g_FriendChatBox.FreeString();
	g_GuildBox.FreeString();
	g_ForceOutMsg.Empty();
	g_QuestionBox.FreeString();
	PartyCharInfoMap::iterator it = g_mapPartyChar.begin();
	for (;it!= g_mapPartyChar.end();it++) 
	{
		SafeDeleteObject(it->second);
	}
	g_mapPartyChar.clear();
}

VOID Global::ReleaseParentLua()
{
	ReleaseUILua(g_pParentLua);
}

VOID Global::DestroyGameContext()
{
	FreeClientMapData();
	SafeDeleteObject(m_mobdb_header);
	std::map<UInt32, MobDB_Client*>::iterator itr = m_mobdb.begin();
	while( itr!=m_mobdb.end() )
	{
		SafeDeleteObject(itr->second);
		itr++;
	}
	m_mobdb.clear();

	//	SafeDeleteObject(m_npcdb_header);
	std::map<UInt32, NPCDB_Client*>::iterator itr2 = m_npcdb.begin();
	while( itr2!=m_npcdb.end() )
	{
		SafeDeleteObject(itr2->second);
		itr2++;
	}
	m_npcdb.clear();

	BYTEChannelCtrlMap::iterator Itr3 = m_MapChannelLst.begin();
	while (Itr3 != m_MapChannelLst.end())
	{
		SafeDeleteObject(Itr3->second);
		Itr3 ++;
	}
	m_MapChannelLst.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Int Global::ImportClient_MapInfo()
{
	int mapid;
	if( g_pMap || gConfigure.uNetwork )
		mapid = g_pMap->GetMap_id();
	else
	{

		if (ChangeMapFlag)
			mapid = gConfigure.uMapID2;
		else
			mapid = gConfigure.uMapID1;

	}
	TCHAR* folderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
	ZeroMemory(folderpath, FILENAME_LENGTH*sizeof(TCHAR));
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportClientMapInfoFilename"), _T("\0"), folderpath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	folderpath[FILENAME_LENGTH-1] = 0;

	if( g_GdfResource || g_GdfResourceEx )
	{
		String patchedFile(folderpath);
		UInt32 beginOffset, fileSize;
		String datafilename(folderpath);
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
		if( fbuf )
		{
			if( m_mapdb )
				SafeDeleteObject(m_mapdb);
			if( m_mapdb_header )
				SafeDeleteObject(m_mapdb_header);
			BYTE* buf = fbuf + beginOffset;

			m_mapdb_header = SafeCreateObject(DefaultInfoHeader);
			memcpy(m_mapdb_header, buf, sizeof(DefaultInfoHeader));
			buf += sizeof(DefaultInfoHeader);

			TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, m_mapdb_header->version!=EXPORT_DATA_CLIENT_MAP_INFO_FORMAT_VERSION, _T("ImportClient_MapInfo from \"%s\", file version:%i, app. version:%i"), String(patchedFile).c_str(), m_mapdb_header->version, EXPORT_DATA_CLIENT_MAP_INFO_FORMAT_VERSION);

			m_mapdb = SafeCreateObject(MapDB_Client);
			m_mapdb->m_basicinfo = SafeCreateObject(BinaryClientMapInfo);

			bool found = false;
			UInt32 sizeofmaplist;
			memcpy(&sizeofmaplist, buf, sizeof(UInt32));
			buf += sizeof(UInt32);
			for( UInt i = 0; i<sizeofmaplist; i++ )
			{
				memcpy(m_mapdb->m_basicinfo, buf, sizeof(BinaryClientMapInfo));
				buf += sizeof(BinaryClientMapInfo);
				if( m_mapdb->m_basicinfo->m_mapid==mapid )
				{
					found = true;
					break;
				}
			}
			SafeDeallocate(folderpath);
			if( !found )
			{
				SafeDeleteObject(m_mapdb->m_basicinfo);
				SafeDeleteObject(m_mapdb);
				SafeDeleteObject(m_mapdb_header);
				UnmapViewOfFile(fbuf);
				return 0;
			}
			UnmapViewOfFile(fbuf);
			return 1;
		}else
		{
			char msg[64];
			sprintf(msg, "Err: ImportClientMapInfoFilename in Gdf Patch File open error\n");
			printf(msg);
			SafeDeallocate(folderpath);
			return 0;
		}
	}else
	{
		FILE *f0;
		if( folderpath[0]!=0 && (f0 = _tfopen(folderpath, _T("rb"))) )
		{
			if( m_mapdb )
				SafeDeleteObject(m_mapdb);
			if( m_mapdb_header )
				SafeDeleteObject(m_mapdb_header);

			m_mapdb_header = SafeCreateObject(DefaultInfoHeader);
			fread(m_mapdb_header, 1, sizeof(DefaultInfoHeader), f0);

			TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, m_mapdb_header->version!=EXPORT_DATA_CLIENT_MAP_INFO_FORMAT_VERSION, _T("ImportClient_MapInfo from \"%s\", file version:%i, app. version:%i"), String(folderpath).c_str(), m_mapdb_header->version, EXPORT_DATA_CLIENT_MAP_INFO_FORMAT_VERSION);

			m_mapdb = SafeCreateObject(MapDB_Client);
			m_mapdb->m_basicinfo = SafeCreateObject(BinaryClientMapInfo);

			bool found = false;
			UInt32 sizeofmaplist;
			fread( &sizeofmaplist, 1, sizeof(UInt32), f0 );
			for( UInt i = 0; i<sizeofmaplist; i++ )
			{
				fread( m_mapdb->m_basicinfo, 1, sizeof(BinaryClientMapInfo), f0 );
				if( m_mapdb->m_basicinfo->m_mapid==mapid )
				{
					found = true;
					break;
				}
			}
			fclose(f0);
			SafeDeallocate(folderpath);
			if( !found )
			{
				SafeDeleteObject(m_mapdb->m_basicinfo);
				SafeDeleteObject(m_mapdb);
				SafeDeleteObject(m_mapdb_header);
				return 0;
			}
			return 1;
		}else
		{
			char msg[64];
			sprintf(msg, "Err: ImportClientMapInfoFilename open error\n");
			printf(msg);
			SafeDeallocate(folderpath);
			return 0;
		}
	}
}

VOID Global::InitParentLua()
{
	InitUILua(g_pParentLua);
}

VOID Global::InitRobotLua(const String &luaFile)
{
	ReleaseRobotLua();
	g_pRobotLua = luaL_newstate();
	TRACE_ENSURE(g_pRobotLua);
	luaL_openlibs(g_pRobotLua);
	luaopen_CommonI(g_pRobotLua);
	luaopen_LuaI(g_pRobotLua);
	RunLua(g_pRobotLua, luaFile);
}

VOID Global::ReleaseRobotLua()
{
	if (g_pRobotLua)
	{
		lua_close(g_pRobotLua);
		g_pRobotLua = NULL;
	}
}

VOID Global::InitUILua(lua_State* &pState)
{
	ReleaseUILua(pState);
	pState = luaL_newstate();
	TRACE_ENSURE(pState);
	luaopen_base(pState);
	luaopen_table(pState);
	luaopen_string(pState);
	luaopen_math(pState);
	luaopen_CommonI(pState);
	luaopen_LuaUI(pState);
}

VOID Global::ReleaseUILua(lua_State* &pState)
{
	if (pState)
	{
		lua_close(pState);
		pState = NULL;
	}
}

Boolean Global::RunLua(lua_State* luaState, const String &filename)
{
	if (luaState == NULL || filename.IsEmpty())
		return BOOLEAN_FALSE;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Run Lua file: %s"), filename.c_str());

	if (!LoadLua(luaState, filename))
		return BOOLEAN_FALSE;

	int r = lua_pcall(luaState, 0, LUA_MULTRET, 0);

	if (r != 0)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Error running Lua file: %s"), filename.c_str());
		TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(luaState, -1)).c_str());
		return (BOOLEAN_FALSE);
	}

	return (BOOLEAN_TRUE);
}

Boolean Global::LoadLua(lua_State* lstate, const String &filename)
{
	int r = 0;

	if ( g_GdfResource || g_GdfResourceEx )
	{
		UInt32 beginOffset, fileSize;
		String luaFile(filename);
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(luaFile, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(luaFile, beginOffset, fileSize);
		if (fbuf)
		{
			if (fileSize > 0)
			{
				BYTE* buf = fbuf + beginOffset;
				r = luaL_loadbuffer(lstate, (char*)buf, fileSize, "line") || 
					lua_pcall(lstate, 0, 0, 0); // = buffered version of dofile
			}
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		StringA luaFile(filename.c_str());				// uncompiled lua file
		StringA outFile(filename.c_str());				// compiled lua file
		PCSTRINGA existFile = NULL;
		luaFile.Replace(LUA_BIN_EXT, LUA_SRC_EXT);
		outFile.Replace(LUA_SRC_EXT, LUA_BIN_EXT);
		
		if (IsFileExist(luaFile.c_str()))			// run .lua file first
			existFile = luaFile.c_str();
		else if (IsFileExist(outFile.c_str()))		// if not exist, run .out file
			existFile = outFile.c_str();
		if (existFile == NULL)
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("lua file %s not found"), 
				String(filename).c_str());
			return BOOLEAN_FALSE;
		}
		r = luaL_loadfile(lstate, existFile);
	}

	if (r != 0)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("lua file %s, %s"),
			filename.c_str(), String(lua_tostring(lstate, -1)).c_str());
		lua_pop(lstate, 1);
		return BOOLEAN_FALSE;
	}

	return BOOLEAN_TRUE;
}

NPCDB_Client* Global::LoadNpcTemplate(UInt32 npcid)
{
	std::map<UInt32, NPCDB_Client*>::iterator itr = m_npcdb.find(npcid);
	if( itr!=m_npcdb.end() )
		return itr->second;
	else
	{
		// load it from file and store it in a map
		TCHAR* tempfolderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
		ZeroMemory(tempfolderpath, FILENAME_LENGTH*sizeof(TCHAR));
		TCHAR* folderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
		ZeroMemory(folderpath, FILENAME_LENGTH*sizeof(TCHAR));
		GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportClientZoneDataDirectory"), _T("\0"), tempfolderpath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
		tempfolderpath[FILENAME_LENGTH-1] = 0;
		_stprintf(folderpath, _T("%sclient_NPC%08i_npc.dat"), tempfolderpath, npcid);
		folderpath[FILENAME_LENGTH-1] = 0;

		NPCDB_Client* tmpdb = NULL;
		if( g_GdfResource || g_GdfResourceEx )
		{
			String patchedFile(folderpath);
			UInt32 beginOffset, fileSize;
			BYTE* fbuf = NULL;
			if ( g_GdfResourceEx )
				fbuf = g_GdfResourceEx->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
			if ( (!fbuf) && g_GdfResource )
				fbuf = g_GdfResource->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
			if( fbuf )
			{
				BYTE* buf = fbuf + beginOffset;
				DefaultInfoHeader m_npcdb_header;
				memcpy(&m_npcdb_header, buf, sizeof(DefaultInfoHeader));
				buf += sizeof(DefaultInfoHeader);
				TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, m_npcdb_header.version!=EXPORT_DATA_CLIENT_NPC_TEMPLATE_FORMAT_VERSION, _T("LoadNpcTemplate from \"%s\", file version:%i, app. version:%i"), String(patchedFile).c_str(), m_npcdb_header.version, EXPORT_DATA_CLIENT_NPC_TEMPLATE_FORMAT_VERSION);
				tmpdb = SafeCreateObject(NPCDB_Client);
				tmpdb->m_basicinfo = SafeCreateObject(BinaryClientNPCTemplate);
				memcpy(tmpdb->m_basicinfo, buf, sizeof(BinaryClientNPCTemplate));
				buf += sizeof(BinaryClientNPCTemplate);
				m_npcdb.insert( std::make_pair(tmpdb->m_basicinfo->m_npc_id, tmpdb) );
				UnmapViewOfFile(fbuf);
			}
		}else
		{
			FILE *f0;
			if( folderpath[0]!=0 && (f0 = _tfopen(folderpath, _T("rb"))) )
			{
				DefaultInfoHeader m_npcdb_header;
				fread(&m_npcdb_header, 1, sizeof(DefaultInfoHeader), f0);

				TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, m_npcdb_header.version!=EXPORT_DATA_CLIENT_NPC_TEMPLATE_FORMAT_VERSION, _T("LoadNpcTemplate from \"%s\", file version:%i, app. version:%i"), String(folderpath).c_str(), m_npcdb_header.version, EXPORT_DATA_CLIENT_NPC_TEMPLATE_FORMAT_VERSION);

				tmpdb = SafeCreateObject(NPCDB_Client);
				tmpdb->m_basicinfo = SafeCreateObject(BinaryClientNPCTemplate);
				fread( tmpdb->m_basicinfo, 1, sizeof(BinaryClientNPCTemplate), f0 );
				m_npcdb.insert( std::make_pair(tmpdb->m_basicinfo->m_npc_id, tmpdb) );
				fclose(f0);
			}
		}
		SafeDeallocate(tempfolderpath);
		SafeDeallocate(folderpath);
		return tmpdb;
	}
}

Int Global::ImportClient_ZoneData(UInt32 mapid)
{
	TCHAR* tempfolderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
	ZeroMemory(tempfolderpath, FILENAME_LENGTH*sizeof(TCHAR));
	TCHAR* folderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
	ZeroMemory(folderpath, FILENAME_LENGTH*sizeof(TCHAR));
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportClientZoneDataDirectory"), _T("\0"), tempfolderpath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	tempfolderpath[FILENAME_LENGTH-1] = 0;
	_stprintf(folderpath, _T("%sclient_MAP%08i_zonedata.dat"), tempfolderpath, mapid);
	folderpath[FILENAME_LENGTH-1] = 0;

	MapDB_Client* mapdb = m_mapdb;
	if( g_GdfResource || g_GdfResourceEx )
	{
		String patchedFile(folderpath);
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			mapdb->m_zonedata_header = SafeCreateObject(BinaryBlockInfoHeader);
			memcpy(mapdb->m_zonedata_header, buf, sizeof(BinaryBlockInfoHeader));
			buf += sizeof(BinaryBlockInfoHeader);

			TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, mapdb->m_zonedata_header->version!=EXPORT_DATA_CLIENT_MAP_ZONEDATA_FORMAT_VERSION, _T("ImportClient_MapInfo from \"%s\", file version:%i, app. version:%i"), String(patchedFile).c_str(), mapdb->m_zonedata_header->version, EXPORT_DATA_CLIENT_MAP_ZONEDATA_FORMAT_VERSION);

			if (mapdb->m_combinedblock )
				SafeDeallocate(mapdb->m_combinedblock);
			mapdb->m_combinedblock = SafeAllocate(WORD, mapdb->m_zonedata_header->blocksizeinbyte);

			UInt32 sizeofzone;
			memcpy(&sizeofzone, buf, sizeof(UInt32));
			buf += sizeof(UInt32);

			ClientMapZoneData* tempblockdata = NULL;
			for( UInt i = 0; i<mapdb->m_zonedata_header->blocksizeinbyte; ++i )
			{
				tempblockdata = SafeCreateObject(ClientMapZoneData);
				memcpy(&tempblockdata->sBlockType, buf, sizeof(WORD));
				buf += sizeof(WORD);
				mapdb->m_zonedata.push_back(tempblockdata);
				mapdb->m_combinedblock[i] = tempblockdata->sBlockType;
			}
			SafeDeallocate(tempfolderpath);
			SafeDeallocate(folderpath);
			UnmapViewOfFile(fbuf);
			return 1;
		}
	}else
	{
		FILE *f0;

		if( folderpath[0]!=0 && (f0 = _tfopen(folderpath, _T("rb"))) )
		{
			mapdb->m_zonedata_header = SafeCreateObject(BinaryBlockInfoHeader);
			fread(mapdb->m_zonedata_header, 1, sizeof(BinaryBlockInfoHeader), f0);

			TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, mapdb->m_zonedata_header->version!=EXPORT_DATA_CLIENT_MAP_ZONEDATA_FORMAT_VERSION, _T("ImportClient_MapInfo from \"%s\", file version:%i, app. version:%i"), String(folderpath).c_str(), mapdb->m_zonedata_header->version, EXPORT_DATA_CLIENT_MAP_ZONEDATA_FORMAT_VERSION);

			if (mapdb->m_combinedblock )
				SafeDeallocate(mapdb->m_combinedblock);
			mapdb->m_combinedblock = SafeAllocate(WORD, mapdb->m_zonedata_header->blocksizeinbyte);

			UInt32 sizeofzone;
			fread(&sizeofzone, 1, sizeof(UInt32), f0);

			ClientMapZoneData* tempblockdata = NULL;
			for( UInt i = 0; i<mapdb->m_zonedata_header->blocksizeinbyte; ++i )
			{
				tempblockdata = SafeCreateObject(ClientMapZoneData);
				fread(&tempblockdata->sBlockType, 1, sizeof(WORD), f0);
				mapdb->m_zonedata.push_back(tempblockdata);
				mapdb->m_combinedblock[i] = tempblockdata->sBlockType;
			}
			fclose(f0);

			SafeDeallocate(tempfolderpath);
			SafeDeallocate(folderpath);
			return 1;
		}
	}
	SafeDeallocate(tempfolderpath);
	SafeDeallocate(folderpath);
	return 0;
}

Int Global::ImportClient_PFData(UInt32 mapid, PathFindLoadData& pPathFindData, UInt32& pNumGuide)
{
	TCHAR* tempfolderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
	ZeroMemory(tempfolderpath, FILENAME_LENGTH*sizeof(TCHAR));
	TCHAR* folderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
	ZeroMemory(folderpath, FILENAME_LENGTH*sizeof(TCHAR));
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportClientZoneDataDirectory"), _T("\0"), tempfolderpath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	tempfolderpath[FILENAME_LENGTH-1] = 0;
	_stprintf(folderpath, _T("%sclient_MAP%08i_pathfind.dat"), tempfolderpath, mapid);
	folderpath[FILENAME_LENGTH-1] = 0;

	if( g_GdfResource || g_GdfResourceEx )
	{
		String patchedFile(folderpath);
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			DefaultPFHeader header;
			memcpy(&header, buf, sizeof(DefaultPFHeader));
			buf += sizeof(DefaultPFHeader);

			TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, header.version!=EXPORT_DATA_MAP_PATHFIND_FORMAT_VERSION, _T("ImportClient_MapInfo from \"%s\", file version:%i, app. version:%i"), String(patchedFile).c_str(), header.version, EXPORT_DATA_MAP_PATHFIND_FORMAT_VERSION);

			UInt16* ubuf = NULL;
			Real32* fbufw = NULL;
			Real32* fbufd = NULL;
			if( header.sizeWeightGraph>0 )
			{
				fbufw = SafeAllocate(Real32, header.sizeWeightGraph);
				memcpy(fbufw, buf, header.sizeWeightGraph * sizeof(Real32));
				buf += header.sizeWeightGraph * sizeof(Real32);
			}
			if( header.sizeDistGraph>0 )
			{
				fbufd = SafeAllocate(Real32, header.sizeDistGraph);
				memcpy(fbufd, buf, header.sizeDistGraph * sizeof(Real32));
				buf += header.sizeDistGraph * sizeof(Real32);
			}
			if( header.sizeLinkGraph>0 )
			{
				ubuf = SafeAllocate(UInt16, header.sizeLinkGraph);
				memcpy(ubuf, buf, header.sizeLinkGraph * sizeof(UInt16));
				buf += header.sizeLinkGraph * sizeof(UInt16);
			}
			pPathFindData.weightData	= fbufw;
			pPathFindData.distData		= fbufd;
			pPathFindData.linkData		= ubuf;
			pNumGuide					= header.numGuildPoint;

			SafeDeallocate(tempfolderpath);
			SafeDeallocate(folderpath);
			UnmapViewOfFile(fbuf);
			return 1;
		}
		SafeDeallocate(tempfolderpath);
		SafeDeallocate(folderpath);
	}else
	{
		FILE *f0;
		if( folderpath[0]!=0 && (f0 = _tfopen(folderpath, _T("rb"))) )
		{

			DefaultPFHeader header;
			fread(&header, 1, sizeof(DefaultPFHeader), f0);

			TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, header.version!=EXPORT_DATA_MAP_PATHFIND_FORMAT_VERSION, _T("ImportClient_MapInfo from \"%s\", file version:%i, app. version:%i"), String(folderpath).c_str(), header.version, EXPORT_DATA_MAP_PATHFIND_FORMAT_VERSION);

			UInt16* ubuf = NULL;
			Real32* fbufw = NULL;
			Real32* fbufd = NULL;
			if( header.sizeWeightGraph>0 )
			{
				fbufw = SafeAllocate(Real32, header.sizeWeightGraph);
				fread(fbufw, sizeof(Real32), header.sizeWeightGraph, f0);
			}
			if( header.sizeDistGraph>0 )
			{
				fbufd = SafeAllocate(Real32, header.sizeDistGraph);
				fread(fbufd, sizeof(Real32), header.sizeDistGraph, f0);
			}
			if( header.sizeLinkGraph>0 )
			{
				ubuf = SafeAllocate(UInt16, header.sizeLinkGraph);
				fread(ubuf, sizeof(UInt16), header.sizeLinkGraph, f0);
			}
			fclose(f0);
			pPathFindData.weightData	= fbufw;
			pPathFindData.distData		= fbufd;
			pPathFindData.linkData		= ubuf;
			pNumGuide					= header.numGuildPoint;

			SafeDeallocate(tempfolderpath);
			SafeDeallocate(folderpath);
			return 1;
		}
		SafeDeallocate(tempfolderpath);
		SafeDeallocate(folderpath);
	}

	//return 0;
	return 1;			// if cannot load path finder data, do dynamic pre-calculation
}

Int Global::ImportClientAllZoneData(PathFindLoadData& pPathFindData, UInt32& pNumGuide)
{
	bool AllMapSucceeded = true;
	if( !ImportClient_ZoneData(m_mapdb->m_basicinfo->m_mapid) || !ImportClient_PFData(m_mapdb->m_basicinfo->m_mapid, pPathFindData, pNumGuide) )
	{
		AllMapSucceeded = false;
		char msg[64];
		sprintf(msg, "Err: ImportClient_ZoneData open error, map=%08i\n", m_mapdb->m_basicinfo->m_mapid);
		printf(msg);
	}
	return AllMapSucceeded;
}

BOOL Global::InitClientData()
{

	m_LineExist.clear();
	m_lineState.clear();
	/*if (g_pLoginProcess)
	{
	g_pLoginProcess->SetCharacterIndex( -1);
	g_pLoginProcess->SetLineIndex(-1);
	}*/
	m_nAccountID = 0;
	return TRUE;
}

BOOL Global::ReinitClientMapData(PathFindLoadData& pPathFindData, UInt32& pNumGuide)
{
	BOOL succeeded = true;
	succeeded = ImportClient_MapInfo();
	if( succeeded )
		succeeded = ImportClient_Instance();
	if( succeeded )
		succeeded = ImportClientAllZoneData(pPathFindData, pNumGuide);

	return succeeded;
}

BOOL Global::FreeClientMapData()
{
	if( m_mapdb )
		SafeDeleteObject(m_mapdb);
	if( m_mapdb_header )
		SafeDeleteObject(m_mapdb_header);
	if( m_npclist_header )
		SafeDeleteObject(m_npclist_header);
	if( m_objlist_header )
		SafeDeleteObject(m_objlist_header);
	if( m_jplist_header )
		SafeDeleteObject(m_jplist_header);

	for( size_t i = 0; i < m_npclist.size(); i++ )
		SafeDeleteObject(m_npclist[(int)i]);
	m_npclist.clear();
	for( size_t i = 0; i < m_objlist.size(); i++ )
		SafeDeleteObject(m_objlist[(int)i]);
	m_objlist.clear();
	for( size_t i = 0; i < m_jplist.size(); i++ )
		SafeDeleteObject(m_jplist[(int)i]);
	m_jplist.clear();
	return TRUE;
}

VOID Global::NotifyLeaveSuccess(Boolean bDelete)
{
	//set all characters net object to null

	if (g_pMap)
		DeleteNetObject(g_pMap);
	g_pMap = NULL;
	//m_pBag = NULL;
	if (g_pMapView != NULL)
		g_pMapView->SetMapObject(NULL);
	DeleteNetGroup(gGlobal.g_pParty);
	gGlobal.g_pParty = NULL;

	if (bDelete) {
		DeleteNetObject(m_pBag);
		DeleteNetGroup(gGlobal.g_SkillLst, BOOLEAN_TRUE);
		DeleteNetGroup(gGlobal.g_ChangeModelSkillLst,BOOLEAN_TRUE);
		g_pMenuQuestJournal->ClearAllJournal();
		DeleteNetObject(gGlobal.g_QuestSystem);

		QGLApplication::GetInstance().DeleteAllCharacters();
		QGLApplication::GetInstance().ResetAllCharactersNetObject();
	}
}

VOID Global::MisAdjust()
{
	if( m_mapdb && m_mapdb->m_basicinfo )
	{
		//change weather
		QGLApplication::GetInstance().RemoveParticleSystem(gGlobal.g_ParticleEffectFog);
		QGLApplication::GetInstance().RemoveParticleSystem(gGlobal.g_ParticleEffectRain);
		QGLApplication::GetInstance().RemoveParticleSystem(gGlobal.g_ParticleEffectSnow);

		QGLApplication::GetInstance().ReleaseNightData();
		gGlobal.Night = false;
		gGlobal.g_DrawAll = TRUE;

		if (m_mapdb->m_basicinfo->m_mapweathermode != 0 && gGlobal.g_pSystemSetting->IsWeatherModeEnable() )
		{
			if (m_mapdb->m_basicinfo->m_mapweathertype == 0)
				QGLApplication::GetInstance().AddParticleSystem(gGlobal.g_ParticleEffectRain);
			else if (m_mapdb->m_basicinfo->m_mapweathertype == 1)
				QGLApplication::GetInstance().AddParticleSystem(gGlobal.g_ParticleEffectFog);
			else if (m_mapdb->m_basicinfo->m_mapweathertype == 2)
				QGLApplication::GetInstance().AddParticleSystem(gGlobal.g_ParticleEffectSnow);
			else if (m_mapdb->m_basicinfo->m_mapweathertype == 3)
				QGLApplication::GetInstance().AddParticleSystem(gGlobal.g_ParticleEffectFog);
		}

		//m_mapdb->m_basicinfo->m_mapdaynight
		if (g_pSystemSetting && g_pSystemSetting->IsNightModeEnable())
		{
			if (m_mapdb->m_basicinfo->m_mapdaynight == 2 && gGlobal.NightMask)
			{
				gGlobal.Night = true;
				gGlobal.NightMask->m_Updated = TRUE;
				gGlobal.g_DrawAll = TRUE;
				QGLApplication::GetInstance().CreateNightData();
				UpdateNightAlpha();
			}
			else if (m_mapdb->m_basicinfo->m_mapdaynight == 1)
			{
				gGlobal.Night = false;
				gGlobal.g_DrawAll = TRUE;
				UpdateNightAlpha();
			}
			else
				UpdateNightAlpha();
		}

		QGLApplication::GetInstance().InitMiniMap(m_mapdb->m_basicinfo->m_minimapfile);

		if (gGlobal.MiniMap && gGlobal.g_pCharacter)
		{
			gGlobal.MiniMap->ResetPlayerPos(gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos().PixelPosX,
				gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos().PixelPosY);

			gGlobal.MiniMap->SetRedrawFlag(true);
			//gGlobal.MiniMap->RenderParent();		
		}

		UIStaticText* pST = pUIManager->FindUIStaticText("PlaceName");
		if( pST && m_mapdb->m_basicinfo->m_minimapfile )
		{
			pST->SetText(String(m_mapdb->m_basicinfo->m_mapname).c_str());
			pST->Redraw();
			AddDirtyUIItem("PlaceName");
		}

		if( gGlobal.m_mapdb->m_basicinfo->m_soundfile )
		{
			static CharA	CurFile[50] = "";

			StringA soundfilename(gGlobal.m_mapdb->m_basicinfo->m_soundfile);
			if( soundfilename.GetLength()>0 )
			{
				soundfilename.Format("Data\\%s", StringA(gGlobal.m_mapdb->m_basicinfo->m_soundfile).c_str());
				soundfilename.ToLower();
				if( soundfilename.Compare(CurFile))
				{
					::GetFSoundManager()->SetMusic(soundfilename.c_str());
					if (gGlobal.g_pSystemSetting->IsMusicModeEnable())
						::GetFSoundManager()->MusicSetVolume(gGlobal.g_pSystemSetting->GetMusicVolForFmod() );
					else
						::GetFSoundManager()->MusicSetVolume(0);
					//m_CurrMusicFilename = soundfilename;
					memcpy(CurFile, soundfilename.c_str(), soundfilename.GetLength() + 1);
				}
			}else
				::GetFSoundManager()->StopMusic();
		}else
			::GetFSoundManager()->StopMusic();

		//pre load npc
		for( UInt16 i = 0; i < (UInt16)gGlobal.m_npclist.size(); ++i )
		{
			NPCInfo temp;
			NPCDB_Client * tempNPC = gGlobal.LoadNpcTemplate(gGlobal.m_npclist[i]->nNPCUserID);
			if (tempNPC)
			{
				CResourcesManager::GetInstance().LoadEntityResourcesFile(tempNPC->m_basicinfo->m_npc_model,
					gGlobal.pD3DDevice);
				QGLApplication::GetInstance().GetRenderMap()->AddLoadedModelName(tempNPC->m_basicinfo->m_npc_model);
			}
		}

		if (g_pSystemSetting)
			g_pSystemSetting->SaveBlockSetting();

	}
}
VOID Global::NotifyCB_ReceiveEmotion(const UInt32 &emo_id, const String &SrcName, const String &TargetName, const bool &isPet)
{
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetAction() != CCharacter::ST_BATTLE )
	{
		CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByName(SrcName);
		if (pCtrl)
		{
			ParticleStructSetting PSS;
			PSS.Destory = TRUE;
			PSS.HaveEffect = FALSE;
			PSS.Loop = FALSE;
			PSS.NoOfParticle = 1;

			CAnimatedEntity Ent;
			Ent.LoadEntityFile(gGlobal.GetEmotionFilePathbyNum(emo_id).c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);
			Ent.SetOffsetType(ENTITYOFFSETTYPE_HEAD);
			if (isPet)
			{
				if (pCtrl->HasFollowPet())
					gGlobal.g_DisposalEntityEffect->AddAnimatedEntityToSystem(&Ent, PSS, pCtrl->GetPetMainView(), TRUE);
				else
				{				

					switch(pCtrl->GetCharMainView()->GetAnimationRotation())
					{
					case SOUTHEAST:
					case EAST:
					case NORTHEAST:
						Ent.SetRotation(SOUTHEAST);
						break;
					case SOUTHWEST:
					case WEST:
					case NORTHWEST:
						Ent.SetRotation(SOUTHWEST);
						break;
					}
					if (pCtrl->GetCharNetObject())
					{
						const  PetData * pPetData = gGlobal.m_resource->GetPetData(pCtrl->GetCharNetObject()->GetPet_id());
						if (pPetData)
						{
							PSS.SubOffsetY = pPetData->pet_Height * -1 - 10;
						}
					}
					Ent.SetOffsetType(ENTITYOFFSETTYPE_NORMAL);
					gGlobal.g_DisposalEntityEffect->AddAnimatedEntityToSystem(&Ent, PSS, pCtrl->GetCharMainView(), TRUE);
				}
			}
			else
				gGlobal.g_DisposalEntityEffect->AddAnimatedEntityToSystem(&Ent, PSS, pCtrl->GetCharMainView(), TRUE);
			//String Msg;
			//gGlobal.PrintMessage(Msg, m_ChannelType, SrcName);
		}
	}

}

VOID Global::NotifyEnterSuccess(LPCPEER pPeer, RPCContext &context, NetObject * map, NetObject * character)
{
	if( QGLApplication::GetInstance().GetMapStateID() != ST_APP_LOGIN)
	{
		//
		if ( g_pScriptControl )
			g_pScriptControl->StopTalkingBeforeBattle();

		if ( g_pShopProcess )
			g_pShopProcess->CloseShopMenu();

		CallLuaFunc(pUIManager->GetLuaState(), "CloseMiddleMap", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseBigMap", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseNPCSearch", String());

		CallLuaFunc(pUIManager->GetLuaState(), "CancelMixShopMaking", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CancelMixShopMixing", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseMixShop", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseMixShopMake", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseIronShopMake", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseNewGuide", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseNewArrow", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseHawk", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseGiftBox", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseGivenPanel", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseWarehousePanel", String());
		if (gGlobal.g_pWareHouseProcess)
			gGlobal.g_pWareHouseProcess->ResetWareHouse();

		CallLuaFunc(pUIManager->GetLuaState(), "CancelMixShopGem", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseMixShopGem", String());
		CallLuaFunc(pUIManager->GetLuaState(), "ClosePartnerCultivationUp", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseBankWithdrawalPanel", String());
		CallLuaFunc(pUIManager->GetLuaState(), "CloseBankDepositPanel", String());
	}
	

	
	QGLApplication::GetInstance().DeleteAllCharacters();
	UInt32 PreCount = 0;
	UInt32 CurrCount = 0;
	UInt32 PreCount2 = 1;
	UInt32 CurrCount2 = 1;
	UInt32 PreCount3 = 2;
	UInt32 CurrCount3 = 2;
	CCharacter * pChar = CastNetObject<CCharacter>(character);
	if(g_pCharacter == NULL)
		g_pCharacter = SafeCreateObject(CCharacterControl);

	bool firstLogin = g_pCharacter->GetCharNetObject() == NULL;

	if (g_pCharacter->GetCharNetObject() != character)
	{
		gGlobal.g_pBattleControl->ShowAutoCountBox(0);
		g_pCharacter->m_BuffMap.clear();
		g_pCharacter->m_BuffCacheMap.clear();
		g_pCharacter->LoadDefaultPart(pChar->GetSexClass());
		CCharacter * pCharOld = g_pCharacter->GetCharNetObject();
		if (pCharOld)
			DeleteNetObject(pCharOld);
		g_pCharacter->SetMapActorObject(pChar);

		CharMsgCharPtrMap::iterator Itr = gGlobal.g_pMenuFriendProcess->m_MessageCharacters.find(pChar->GetChar_id());
		if (Itr != gGlobal.g_pMenuFriendProcess->m_MessageCharacters.end())
		{
			CMessageCharacter * pMsgCharTmp = FindNetObject<CMessageCharacter>(Itr->second);
			if(pMsgCharTmp != NULL)
				gGlobal.g_pMenuFriendProcess->m_pOwnChar = pMsgCharTmp;
		}

		gGlobal.g_pAccountControl->m_iChangeMapLineTimer = GetTickCount();
	}
	g_pCharacter->OnUpdateInfo(pChar->GetNickName(), EDGE_SHADOW);

	g_pCharacter->SetFontColor(TextColor[ColorIndex_LightBlue]);
	g_pCharacter->GetCharNameView()->SetFontColor(g_pCharacter->GetFontColor());

	g_pCharacter->InitShadow();
	g_pCharacter->OnUpdateSpeed();
	g_pCharacter->OnUpdateFollowId(pChar->GetFollowId(), pChar->GetFollowOrder());

	g_pCharacter->GetCharMainView()->SetEntityType(ENTITY_TYPE_MAP_CONTROL_PLAYER);
	g_pCharacter->GetCharShadowView()->SetEntityType(ENTITY_TYPE_MAP_CONTROL_PLAYER);

	if (g_pMiddleMap)
	{
		g_pCharacter->GetCharMainView()->m_PathFindData.Init();
		g_pMiddleMap->ResetCharDes();
	}

	gGlobal.g_IsChangeLine = FALSE;
	if (g_pMap && map != g_pMap)
		DeleteNetObject(g_pMap);
	g_pMap = CastNetObject<CMap>(map);

	if (gGlobal.g_CurLineID != -1)
		gGlobal.g_IsChangeLine = (gGlobal.g_CurLineID != g_pMap->GetLine_id());

	gGlobal.g_CurLineID = g_pMap->GetLine_id();

	if (g_pMapView == NULL)
		g_pMapView = SafeCreateObject(CMapControl);

	g_pMapView->SetMapObject(g_pMap);

	PreCount = ::timeGetTime();
	QGLApplication::GetInstance().ReadChangeMapInfo();
	CurrCount = ::timeGetTime();

	// set title
	if (g_pCharacter)
	{
		UInt8 mapType = 0;
		if( m_mapdb && m_mapdb->m_basicinfo )
			mapType = m_mapdb->m_basicinfo->m_maptype;

		SetQGLTitle(
			gGlobal.GetStringTable()->Get(_T("APP_QGL_CAPTION")),
			g_pCharacter->GetMapActorID(),
			g_pCharacter->GetCharNetObject()->GetNickName().c_str(), 
			gConfigure.strServerName.c_str(),
			gGlobal.g_CurLineID,
			mapType
			);
	}

	String strMapFileName;
	if( m_mapdb && m_mapdb->m_basicinfo )
	{
		strMapFileName.Format(_T("%s"), m_mapdb->m_basicinfo->m_mapmapfile);
		String text;
		UInt tempintX, tempintY;
		text.Format(_T("MapX%i"), m_mapdb->m_basicinfo->m_mapid);
		tempintX = GetPrivateProfileInt(_T("AUTOWALK"), text.c_str(), 0, CONFIGURATION_INI_FILE);
		text.Format(_T("MapY%i"), m_mapdb->m_basicinfo->m_mapid);
		tempintY = GetPrivateProfileInt(_T("AUTOWALK"), text.c_str(), 0, CONFIGURATION_INI_FILE);
		gGlobal.SetAutoPos(tempintX, tempintY);
	}
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Map Read Error: %d"), g_pMap->GetMap_id());
	}

	PreCount2 = ::timeGetTime();
	if (QGLApplication::GetInstance().m_mapState->GetStateID() == ST_APP_LOGIN)
		QGLApplication::GetInstance().ChangeMapUI();
	CurrCount2 = ::timeGetTime();

	State::ChangeState(QGLApplication::GetInstance().m_mapState , SafeCreateObject(CMapState));

	if (g_pCharProcess)
		g_pCharProcess->UpdateCharExp();
	MisAdjust();

	if (firstLogin)
	{
		g_pMenuHelpProcess->ReadHelpTipsFile();
		g_pMenuHelpProcess->CheckHelpTipsEncounter();
		g_pMenuHelpProcess->HandleEvent(HELP_TIPS_FIRST_LOGIN);
		g_pMenuHelpProcess->ReadGuideFile();
		g_pMenuHelpProcess->CheckGuideEncounter();
	}
	g_pMenuHelpProcess->HandleEvent(HELP_TIPS_EVERY_LOGIN);
	g_pMenuHelpProcess->HandleEvent(HELP_TIPS_ENTER_MAP);

	//find guild
	g_pMenuGuildProcess->FindOwnGuild();

	g_pMenuFriendProcess->FindOwnCharacter();

	CCrossMapPathFinder* pcmpf = CCrossMapPathFinder::GetInstance();
	if (NULL != pcmpf && pcmpf->GetActiveState()) 
	{
		BOOL bStop = TRUE;
		UInt32 uSrcJumpPoint = pcmpf->GetCurPathPoint();
		const JUMPPOINT* pjpSrc = gGlobal.m_resource->GetAutoPathData(uSrcJumpPoint);
		if (NULL != pjpSrc) 
		{
			if (m_mapdb->m_basicinfo->m_mapid == pjpSrc->uMapId) 
				bStop=FALSE;
		}

		if (!bStop)//charactor jump to the right map,continue next step
		{
			UInt32 uJumpPoint = pcmpf->GetNextPathPoint();
			const JUMPPOINT* pjp = gGlobal.m_resource->GetAutoPathData(uJumpPoint);
			if (NULL != pjp)//need to jump to other map,run to the jump point
			{
				if (m_mapdb->m_basicinfo->m_mapid == pjp->uMapId) 
				{
					if (0 == pjp->uJumpType) 
						pcmpf->GetNextPathPoint();

					gGlobal.g_pCharacter->SetEntityDesPos(pjp->u16PosX, pjp->u16PosY);
					PointList ptList;
					ptList = gGlobal.g_pCharacter->GetCharMainView()->m_PathFindData.m_ptList;

					if (gGlobal.g_pMiddleMap->IsOpenMiddleMap())
						gGlobal.g_pMiddleMap->DrawPath(pjp->u16PosX, pjp->u16PosY);
					else
					{
						gGlobal.g_pMiddleMap->SetCharDes(pjp->u16PosX, pjp->u16PosY);
						gGlobal.g_pMiddleMap->SetPathEnd(pjp->u16PosX, pjp->u16PosY);
					}
				}
				else
				{
					if (0 == pjpSrc->uJumpType) 
					{
						gGlobal.g_pCharacter->SetEntityDesPos(pjpSrc->u16PosX, pjpSrc->u16PosY);
						PointList ptList;
						ptList = gGlobal.g_pCharacter->GetCharMainView()->m_PathFindData.m_ptList;

						if (gGlobal.g_pMiddleMap->IsOpenMiddleMap())
							gGlobal.g_pMiddleMap->DrawPath(pjpSrc->u16PosX, pjpSrc->u16PosY);
						else
						{
							gGlobal.g_pMiddleMap->SetCharDes(pjpSrc->u16PosX, pjpSrc->u16PosY);
							gGlobal.g_pMiddleMap->SetPathEnd(pjpSrc->u16PosX, pjpSrc->u16PosY);
						}
					}
					else
					{
						CCharacter* pSelf = gGlobal.g_pCharacter->GetCharNetObject();
						if (NULL != pSelf)//has character 
							pSelf->RequestCrossMapJump(gGlobal.pPeerProxy,gGlobal.g_context,uSrcJumpPoint,uJumpPoint);
						else
						{
							gGlobal.m_u32DestNpcId=0;
							bStop = TRUE;
						}
					}
				}
			}
			else if (UINT_MAX == uJumpPoint) //no jump point,in destinate map
			{
				BlockPos bpDest;
				pcmpf->GetDestPoint(bpDest);
				if (bpDest.PosX != 0 && bpDest.PosY != 0) //run to destinate position
				{
					gGlobal.g_pCharacter->SetEntityDesPos(bpDest.PosX,bpDest.PosY);
					PointList ptList;
					ptList = gGlobal.g_pCharacter->GetCharMainView()->m_PathFindData.m_ptList;
					
					if (ptList.size() > 10)
						gGlobal.g_pCharacter->SetEntityDesPos(ptList[ptList.size() - 10].PosX , ptList[ptList.size() - 10].PosY, true);
					else
						gGlobal.g_pCharacter->SetEntityDesPos(bpDest.PosX , bpDest.PosY, true);

					if (gGlobal.g_pMiddleMap->IsOpenMiddleMap())
						gGlobal.g_pMiddleMap->DrawPath(bpDest.PosX, bpDest.PosY);
					else
					{
						if (ptList.size() > 10)
							gGlobal.g_pMiddleMap->SetCharDes( ptList[ptList.size() - 10].PosX , ptList[ptList.size() - 10].PosY );
						else
							gGlobal.g_pMiddleMap->SetCharDes(bpDest.PosX, bpDest.PosY);
						gGlobal.g_pMiddleMap->SetPathEnd(bpDest.PosX, bpDest.PosY);
					}
				}
				bStop = TRUE;
			}
		}
		if (bStop) 
		{
			pcmpf->DestroyPathData();
		}
	}
}

BOOL Global::ImportClient_Instance()
{
	bool AllMapSucceeded = true;
	UInt32 currmapuid = m_mapdb->m_basicinfo->m_mapid;

	TCHAR* tempfolderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
	ZeroMemory(tempfolderpath, FILENAME_LENGTH*sizeof(TCHAR));
	TCHAR* folderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
	ZeroMemory(folderpath, FILENAME_LENGTH*sizeof(TCHAR));
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportClientZoneDataDirectory"), _T("\0"), tempfolderpath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	tempfolderpath[FILENAME_LENGTH-1] = 0;
	_stprintf(folderpath, _T("%sclient_MAP%08i_instance.dat"), tempfolderpath, currmapuid);
	folderpath[FILENAME_LENGTH-1] = 0;

	if( g_GdfResource || g_GdfResourceEx )
	{
		String patchedFile(folderpath);
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			DefaultInfoHeader m_instance_header;
			memcpy(&m_instance_header, buf, sizeof(DefaultInfoHeader));
			buf += sizeof(DefaultInfoHeader);

			TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, m_instance_header.version!=EXPORT_DATA_CLIENT_MAP_INSTANCE_FORMAT_VERSION, _T("ImportClient_Instance from \"%s\", file version:%i, app. version:%i"), String(patchedFile).c_str(), m_instance_header.version, EXPORT_DATA_CLIENT_MAP_INSTANCE_FORMAT_VERSION);

			BinaryClientNPCInstance* tmpnpc;

			Int16 sizeofnpc;
			memcpy(&sizeofnpc, buf, sizeof(Int16));
			buf += sizeof(Int16);
			for( int j = 0; j<sizeofnpc; j++ )
			{
				tmpnpc = SafeCreateObject(BinaryClientNPCInstance);
				memcpy(tmpnpc, buf, sizeof(BinaryClientNPCInstance));
				buf += sizeof(BinaryClientNPCInstance);
				m_npclist.push_back(tmpnpc);
			}

			BinaryClientOBJInstance* tmpobj;

			Int16 sizeofobj;
			memcpy(&sizeofobj, buf, sizeof(Int16));
			buf += sizeof(Int16);
			for( int j = 0; j<sizeofobj; j++ )
			{
				tmpobj = SafeCreateObject(BinaryClientOBJInstance);
				memcpy(tmpobj, buf, sizeof(BinaryClientOBJInstance));
				buf += sizeof(BinaryClientOBJInstance);
				m_objlist.push_back(tmpobj);
			}

			BinaryClientJumppointInstance* tmpjp;

			Int16 sizeofjp;
			memcpy(&sizeofjp, buf, sizeof(Int16));
			buf += sizeof(Int16);
			for( int j = 0; j<sizeofjp; j++ )
			{
				tmpjp = SafeCreateObject(BinaryClientJumppointInstance);
				memcpy(tmpjp, buf, sizeof(BinaryClientJumppointInstance));
				buf += sizeof(BinaryClientJumppointInstance);
				m_jplist.push_back(tmpjp);
			}
			SafeDeallocate(folderpath);
			SafeDeallocate(tempfolderpath);
			UnmapViewOfFile(fbuf);
			return TRUE;
		}
		else
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Err: ImportClientInstanceFilename open error"));
			SafeDeallocate(folderpath);
			SafeDeallocate(tempfolderpath);
			return FALSE;
		}
	}else
	{
		FILE *f0;
		if( folderpath[0]!=0 && (f0 = _tfopen(folderpath, _T("rb"))) )
		{
			DefaultInfoHeader m_instance_header;
			fread(&m_instance_header, sizeof(DefaultInfoHeader), 1, f0);

			TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, m_instance_header.version!=EXPORT_DATA_CLIENT_MAP_INSTANCE_FORMAT_VERSION, _T("ImportClient_Instance from \"%s\", file version:%i, app. version:%i"), String(folderpath).c_str(), m_instance_header.version, EXPORT_DATA_CLIENT_MAP_INSTANCE_FORMAT_VERSION);

			BinaryClientNPCInstance* tmpnpc;

			Int16 sizeofnpc;
			fread(&sizeofnpc, sizeof(Int16), 1, f0);
			for( int j = 0; j<sizeofnpc; j++ )
			{
				tmpnpc = SafeCreateObject(BinaryClientNPCInstance);
				fread( tmpnpc, sizeof(BinaryClientNPCInstance), 1, f0 );
				m_npclist.push_back(tmpnpc);
			}

			BinaryClientOBJInstance* tmpobj;

			Int16 sizeofobj;
			fread(&sizeofobj, sizeof(Int16), 1, f0);
			for( int j = 0; j<sizeofobj; j++ )
			{
				tmpobj = SafeCreateObject(BinaryClientOBJInstance);
				fread( tmpobj, sizeof(BinaryClientOBJInstance), 1, f0 );
				m_objlist.push_back(tmpobj);
			}

			BinaryClientJumppointInstance* tmpjp;

			Int16 sizeofjp;
			fread( &sizeofjp, sizeof(Int16), 1, f0 );
			for( int j = 0; j<sizeofjp; j++ )
			{
				tmpjp = SafeCreateObject(BinaryClientJumppointInstance);
				fread( tmpjp, sizeof(BinaryClientJumppointInstance), 1, f0 );
				m_jplist.push_back(tmpjp);
			}
			fclose(f0);
			SafeDeallocate(folderpath);
			SafeDeallocate(tempfolderpath);
			return TRUE;
		}
		else
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Err: ImportClientInstanceFilename open error"));
			SafeDeallocate(folderpath);
			SafeDeallocate(tempfolderpath);
			return FALSE;
		}
	}

	SafeDeallocate(folderpath);
	SafeDeallocate(tempfolderpath);
}

VOID Global::SetCombinedBlock(PathFindLoadData& pPathFindData, UInt32 pNumGuide)
{
	/*TCHAR* tempfolderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
	ZeroMemory(tempfolderpath, FILENAME_LENGTH*sizeof(TCHAR));
	TCHAR* folderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
	ZeroMemory(folderpath, FILENAME_LENGTH*sizeof(TCHAR));

	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportClientOBJBlockDirectory"), _T("\0"), tempfolderpath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	tempfolderpath[FILENAME_LENGTH-1] = 0;

	/*int pos;
	int i, j;
	int mapwidth = m_mapdb->m_zonedata_header->width;
	int mapheight = m_mapdb->m_zonedata_header->height;

	if( m_mapdb->m_combinedblock )
	SafeDeallocate(m_mapdb->m_combinedblock);
	m_mapdb->m_combinedblock = SafeAllocate(WORD, mapwidth * mapheight);


	//memcpy(m_mapdb->m_combinedblock, m_mapdb->m_zonedata, mapheight * mapwidth * sizeof(WORD));
	/*for( i = 0; i<mapheight; i++ )
	{
	for( j = 0; j<mapwidth; j++ )
	{
	pos = i * mapwidth + j;
	m_mapdb->m_combinedblock[pos] = m_mapdb->m_zonedata[pos]->sBlockType;
	}
	}
	*/

	/*ObjInstList::iterator itr = gGlobal.m_objlist.begin();
	while( itr!=gGlobal.m_objlist.end() )
	{
	BinaryClientOBJInstance* thisobj = (*itr);
	Int32 relativeX = thisobj->m_posx;
	Int32 relativeY = thisobj->m_posy;

	_stprintf(folderpath, _T("%s%s.blk"), tempfolderpath, thisobj->m_filename);
	folderpath[FILENAME_LENGTH-1] = 0;

	// read all OBJ files in current map
	Reuben::Platform::File * file = SafeCreateObject(Reuben::Platform::File);
	try
	{
	file->Open(folderpath, Reuben::Platform::File::MODE_READ_EXISTING);
	if (file->IsOpened())
	{
	DefaultInfoHeader header;
	Int8 state = 0;
	Int64 blocklen = 0;
	BYTE* buf = NULL;

	file->Read((BYTE*)&header, sizeof(header) );

	for( int i = 0; i<19; i++ )
	{
	file->Read((BYTE*)&state, sizeof(Int8) * 1);
	file->Read((BYTE*)&blocklen, sizeof(Int64) * 1);
	buf = SafeAllocate(BYTE, (UInt)(blocklen * sizeof(BlockPos)));
	if( blocklen!=0 )
	{
	file->Read(buf, (UInt)(sizeof(BlockPos) * blocklen));
	if( state==0 )
	{
	for( int j = 0; j<blocklen; j++ )
	{
	BlockPos* bpos = (BlockPos*)&buf[j*sizeof(BlockPos)];
	pos = ( relativeY + bpos->PosY ) * mapwidth + bpos->PosX + relativeX;

	if( !(m_mapdb->m_combinedblock[pos] & TILE_IN_BLOCK_VALUE) )
	m_mapdb->m_combinedblock[pos] = m_mapdb->m_combinedblock[pos] + TILE_IN_BLOCK_VALUE;
	if( m_mapdb->m_combinedblock[pos] & TILE_IN_NAVIGATION_VALUE )
	m_mapdb->m_combinedblock[pos] = m_mapdb->m_combinedblock[pos] - TILE_IN_NAVIGATION_VALUE;
	}
	}
	SafeDeallocate(buf);
	}
	}
	file->Close();
	}
	}
	catch(Reuben::Platform::FileException Er)
	{
	}
	SafeDeleteObject(file);
	itr++;
	}

	if( tempfolderpath )
	SafeDeallocate(tempfolderpath);
	if( folderpath )
	SafeDeallocate(folderpath);
	*/
	CreateServer_MapPathFinder(pPathFindData, pNumGuide); // Run only after Map and Zone are read
	return;
}

BOOL Global::CreateServer_MapPathFinder(PathFindLoadData& pPathFindData, UInt32 pNumGuide)
{
	//UInt32 PreCount = ::timeGetTime();
	PathFinder *finder = SafeCreateObject( PathFinder );

	SetPathFinderData(*finder, 
		m_mapdb->m_combinedblock,
		m_mapdb->m_zonedata_header->width,
		m_mapdb->m_zonedata_header->height);
	//UInt32 CurrCount = ::timeGetTime();
	//UInt32 PreCount2 = ::timeGetTime();
	if( pNumGuide!=0 && pNumGuide!=finder->GetGuideCount() )
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("INFO: Map %i, pathfind.DAT(GP=%i) does not match zoneinfo.DAT(GP=%i).  Dynamic Calculation is executed."),
			m_mapdb->m_basicinfo->m_mapid, pNumGuide, finder->GetGuideCount());
		finder->PreCalculate();
	}
	else
	{
		finder->PreCalculate(pPathFindData.weightData, pPathFindData.distData, pPathFindData.linkData);
	}

	if( gGlobal.g_PathFinder )
		SafeDeleteObject(gGlobal.g_PathFinder);

	gGlobal.g_PathFinder = finder;
	//UInt32 CurrCount2 = ::timeGetTime();
	return TRUE;
}

BOOL Global::SetPathFinderData(PathFinder &pf, WORD* block, Int width, Int height)
{
	// block array size check
	if( width==0 || height==0 )
		return FALSE;

	pf.SetMap((UInt16*) block, width, height);
	pf.FindGuidePoints();
	/*
	UInt16 x = 0;
	UInt16 y = 0;
	UInt32 vecpos;

	pf.CreateMap(width, height);
	for( y = 0; y < height; y++ )
	{
	for( x = 0; x < width; x++ )
	{
	vecpos = y * width + x;
	if( block[vecpos] & TILE_IN_BLOCK_VALUE )
	{
	pf.SetBlockType(x, y, BK_BLOCKED);
	}else if( block[vecpos] & TILE_IN_NAVIGATION_VALUE )
	{
	pf.AddGuidePoint(x, y);
	}else
	{
	pf.SetBlockType(x, y, BK_EMPTY);
	}
	}
	}
	*/
	return TRUE;

}


VOID Global::NotifyEnterBattle(LPCPEER pPeer, RPCContext &context, NetObject * battle, NetGroup * grp, const UInt8 & PlayerTeam,
							   const BYTE & battleState, const UInt32 & npcId)
{


	// character enter battle
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_EnterBattle:"));

	CBattle* pBattle = CastNetObject<CBattle>(battle);
	TRACE_ENSURE(pBattle != NULL);
	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("CB battle but client not finish npc id: %d battle state %d"), npcId, battleState);
	}

	g_pBattleControl->FreeBattleEntData();
	g_pBattleControl->SetBattleNetObject(pBattle);
	//	gGlobal.g_pCharacter->SetBatActor(pBatChar);
	//	State::ChangeState(gGlobal.g_pCharacter->m_mapState, SafeCreateObject(CBattleState));

	g_pBattleControl->m_EnemyTeamIndex = 0;
	g_pBattleControl->m_PlayerTeamIndex = 0;
	g_pBattleControl->m_NpcId = npcId;
	g_pBattleControl->ClearBatBubbleCtrlData();

	if (grp)
	{
		for (UInt16 i = 0; i < grp->GetCount(); i++)
		{
			CBatActor *actor = grp->GetChild<CBatActor>(i);
			TRACE_ENSURE(actor != NULL);
			{
				CCharacterControl * pCharCtrl = SafeCreateObject(CCharacterControl);
				CCharacterControl * pPetCtrl = NULL;
				if (actor->GetLeftBattle())
				{
					;	// BatActor has already left
				}
				else if (actor->GetClassID() == BatCharacter_CLASSID)
				{
					CBatCharacter * pBatChar = CastNetObject<CBatCharacter>(actor);
					if (pBatChar)
					{
						pCharCtrl->LoadDefaultPart(pBatChar->GetSexClass());
						pCharCtrl->InitShadow();
					}
				}
				else
				{
					CBatMob * pBatMob = CastNetObject<CBatMob>(actor);
					if (pBatMob)
					{
						//if (pBatMob->IsMob())
						// find in file for MOB model and color
						MobDB_Client * mobTemplate = LoadMobTemplate(pBatMob->GetMob_id());
						if( mobTemplate )
						{
							// load model
							pCharCtrl->LoadDefaultMob(String(mobTemplate->m_basicinfo->model), String(mobTemplate->m_basicinfo->color));

							//add bat bubble mod id into bubble control
							if ( (actor->GetTeam() != PlayerTeam) && (npcId != 0) )
								g_pBattleControl->AddBatBubbleMobId(pBatMob->GetMob_id());


						}
						else
						{
							TRACE_INFODTL(GLOBAL_LOGGER, _F("Global::NotifyEnterBattle Error (1) [cannot load model] "));
						}
						pCharCtrl->InitShadow();	
					}
				}
				pCharCtrl->SetBatActor(actor);

				if (actor->GetPet_uid() > 0)
				{
					const PetData* pPetData = gGlobal.m_resource->GetPetData(actor->GetPet_id());
					if (pPetData)
					{
						pPetCtrl = SafeCreateObject(CCharacterControl);
						String mobName = pPetData->pet_model;
						pPetCtrl->GetCharMainView()->SetEntityType(ENTITY_TYPE_MAP_PLAYER);
						pPetCtrl->GetCharMainView()->LoadEntityFile(mobName.c_str(), gGlobal.pD3DDevice);

						String colorFileName = mobName.substr(0, mobName.length() - 4);
						char abc = pPetData->pet_color + 96;
						colorFileName.Format(_T("%s%c.tga"), colorFileName.c_str(), abc);
						pPetCtrl->GetCharMainView()->ChangeColorP(mobName.c_str(), colorFileName.c_str());

						pPetCtrl->GetCharMainView()->SetExtraZBuffer(-1);



						pPetCtrl->GetCharShadowView()->InitEntPicInfo();
						pPetCtrl->GetCharShadowView()->LoadEntityFile(gConfigure.strShadowName.c_str(), gGlobal.pD3DDevice);
						pPetCtrl->GetCharShadowView()->SetExtraZBuffer(-2);
						pPetCtrl->GetCharShadowView()->SetEntParent(pPetCtrl->GetCharMainView());

						StringA strTemp = gConfigure.strFontStyle.c_str();
						pPetCtrl->GetCharNameView()->LoadText(actor->GetPetName().c_str(), 
							strTemp.c_str(), gConfigure.uNameFontSize, D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
							gConfigure.uNameFontColorG, gConfigure.uNameFontColorB), EDGE_SHADOW);
						pPetCtrl->GetCharNameView()->SetEntParent(pPetCtrl->GetCharMainView());
						pPetCtrl->GetCharNameView()->SetExtraZBuffer(NameExtraZ - 1);

						pPetCtrl->GetCharNameView()->SetFontColor(pCharCtrl->GetFontColor());

						pPetCtrl->GetCharMainView()->SetVisible(false ,true);
					}
				}
				//actor->GetTeam(); //0 enemy 1 player
				//actor->GetPosition(); 

				State::ChangeState(pCharCtrl->m_mapState, SafeCreateObject(CBattleState));

				g_pBattleControl->m_CurrTeam = PlayerTeam;

				if (actor->GetTeam() != PlayerTeam)
				{
					g_pBattleControl->AddCharacterToBattleLst(pCharCtrl, pD3DDevice, 
						BattlePos[actor->GetPosition() + 20][0], BattlePos[actor->GetPosition() + 20][1], FALSE, pPetCtrl);

					pCharCtrl->GetCharMainView()->SetRotation(SOUTHEAST);
					g_pBattleControl->m_EnemyTeamIndex ++;
				}
				else
				{
					g_pBattleControl->AddCharacterToBattleLst(pCharCtrl, pD3DDevice, 
						BattlePos[actor->GetPosition()][0], BattlePos[actor->GetPosition()][1], battleState != BATTLE_STATE_MONITOR, pPetCtrl);
					pCharCtrl->GetCharMainView()->SetRotation(NORTHWEST);
					g_pBattleControl->m_PlayerTeamIndex ++;
				}

				for (Int i = 0; i < (Int)pCharCtrl->addBuffList.size(); i ++)
				{
					pCharCtrl->GetCharMainView()->LoadEntityFile(pCharCtrl->addBuffList[i]->Buff_EffectFile, gGlobal.pD3DDevice, EQUIP_OTHERS);

					CallLuaFunc(gGlobal.g_pParentLua, "IsInvisibleBuff", String(), pCharCtrl->addBuffList[i]->id);
					if (gGlobal.g_TempResult > 0)
					{
						if (gGlobal.g_pBattleControl->GetControlPlayer() == NULL || 
							!gGlobal.g_pBattleControl->GetControlPlayer()->DetectBuff(pCharCtrl->addBuffList[i]->id))
						pCharCtrl->GetCharMainView()->SetAlphaOffset(InvisibleAlphaOffset, TRUE);
					}
				}
				pCharCtrl->addBuffList.clear();
			}
		}
		//DeleteNetGroup(grp, BOOLEAN_FALSE);
	}
	if (grp)
	{
		for (UInt16 i = 0; i < grp->GetCount(); i++)
		{
			CBatActor *actor = grp->GetChild<CBatActor>(i);
			TRACE_ENSURE(actor != NULL);
			CCharacterControl * pCharCtrl = g_pBattleControl->FindCharCtrlByBatActor(actor);
			if (pCharCtrl != NULL && 
				(battleState == BATTLE_STATE_MONITOR || g_pBattleControl->m_CurrTeam != pCharCtrl->GetTeam()))
			{
				pCharCtrl->RemoveHideEffectBuff();
			}
		}
		DeleteNetGroup(grp, BOOLEAN_FALSE);
	}

	QGLApplication::GetInstance().EnterBattle();

	{// load the npc dialogue lua 
		g_pBattleControl->LoadNpcDialogueLua(npcId);
	}

	{// Init battle script control
		if ( gGlobal.g_pBatScriptControl ) 
		{
			gGlobal.g_pBatScriptControl->ResetData();
			//gGlobal.g_pBatScriptControl->DisappearDialogHandle();
		}
	}

	CallLuaFunc(pUIManager->GetLuaState(), "HideBattlePanel", String());
	CallLuaFunc(pUIManager->GetLuaState(), "HideBattlePartnerPanel", String());
	if (battleState == BATTLE_STATE_MONITOR)
	{
		CallLuaFunc(pUIManager->GetLuaState(), "ShowBattleWatchPanel", String());
		g_helpEnable = FALSE;
		gGlobal.g_pBattleControl->SetCounterVisible(FALSE);
		//check whether monitoring this character
		/*CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(g_pCharacter->GetCharNetObject()->GetMonitorCharID());;
		if (pCtrl)
		{
		if (pCtrl->GetAction() ==  CCharacter::ST_NEEDHELP)
		{

		}
		}*/
	}
	else
		CallLuaFunc(pUIManager->GetLuaState(), "HideBattleWatchPanel", String());
	CallLuaFunc(pUIManager->GetLuaState(), "ShowWaitPanel", String());
	//gGlobal.g_pBattleControl->SetCounterVisible(FALSE);

	if (gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerGroup)
	{
		CPartner* partner = gGlobal.g_pPartnerGroup->GetCurrBattlePartner();
		if (!partner)
			gGlobal.g_pPartnerProcess->UpdateSmallIcon(false);
	}

	// randomly pop up the Anti-Hack menu(1 out of 20 times) if the previous answer is correct, while 100% chance
//	if (gGlobal.g_pAntiHack) // of poping up if the previous answer is incorrect.
//		gGlobal.g_pAntiHack->RandomAntiHackPopUp();   	

}

VOID Global::NotifyReceiveMessageChannel(LPCPEER pPeer, RPCContext &context, NetObject * channel, const BYTE &Type)
{
	CChannel* pChannel = CastNetObject<CChannel>(channel);

	if (pChannel == NULL)
		return;

	BYTEChannelCtrlMap::iterator Itr = m_MapChannelLst.find(Type);
	if (Itr != m_MapChannelLst.end())
	{
		CChannel* pChannelNetObject = Itr->second->GetChannnelNetObject();

		if (pChannel == pChannelNetObject)
			return;

		SafeDeleteObject(Itr->second);
		m_MapChannelLst.erase(Itr);
	}

	CChannelControl * pChannelControl = SafeCreateObject(CChannelControl);
	pChannelControl->SetChannelNetObject(pChannel);
	pChannelControl->SetChannnelType(Type);
	//find whether the player switch off the channel
	pChannelControl->SetListenToChannel(g_ClientistenToChannel[Type]);


	m_MapChannelLst.insert(std::make_pair(Type, pChannelControl));

	//check whether has party 
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (pParty)
	{
		pChannelControl = gGlobal.GetChannelControl(CHANNELTYPE_PARTY);
		if (pChannelControl == NULL)
		{
			pParty->JoinChannel(gGlobal.pPeerProxy, gGlobal.g_context,  
						gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id());
		}
	}

}

CAnimatedEntity Global::GetCharModel(UInt8 sexClass, UInt16 head, UInt16 headColor, UInt32 cloth, UInt16 clothColor, UInt32 eq_headBand, UInt32 eq_weapon)
{	
	CAnimatedEntity temp;
	temp.InitEntPicInfo();

	if (head == 0)
		head = 1;

	if (headColor == 0)
		headColor = 1;

	if (cloth == 0)
		cloth = 1;

	if (clothColor == 0)
		clothColor = 1;
	
	char abc = abc = 96 + headColor;

	String buf, buf2;
	buf2.Format(_T("h%05d_t%03d%c.tga"), sexClass, head, abc);
	buf.Format(_T("h%05d_t%03d.ent"), sexClass, head);
	temp.LoadEntityFile(buf.c_str(), gGlobal.pD3DDevice, EQUIP_HEAD);
	temp.ChangeColorP(buf.c_str(), buf2.c_str());

	//for hat or hair
	{
		const ItemData* pItemData = gGlobal.m_resource->GetItemData(eq_headBand );
		if( pItemData )
		{
			if (pItemData->eqType == EQTYPE_HAT_WITH_HAIR)
			{
				temp.RemovePart(EQUIP_HAIR);
				buf.Format(_T("j%05d_t%03d.ent"), sexClass, head);
				temp.LoadEntityFile(buf.c_str(), gGlobal.pD3DDevice, EQUIP_HAIR);
				temp.ChangeColorP(buf.c_str(), buf2.c_str());
			}
			else if (pItemData->eqType == EQTYPE_HAT_WITHOUT_HAIR)
				temp.RemovePart(EQUIP_HAIR);;
			
			if( sexClass==pItemData->reqSexClass1 )
			{
				temp.RemovePart(EQUIP_HEAD_ACCESSORY);
				temp.LoadEntityFile(String(pItemData->eq_Outlook1).c_str(), gGlobal.pD3DDevice, EQUIP_HEAD_ACCESSORY);
			}
			else if( sexClass==pItemData->reqSexClass2 )
			{
				temp.RemovePart(EQUIP_HEAD_ACCESSORY);
				temp.LoadEntityFile(String(pItemData->eq_Outlook2).c_str(), gGlobal.pD3DDevice, EQUIP_HEAD_ACCESSORY);
			}
			else
			{
				temp.RemovePart(EQUIP_HEAD_ACCESSORY);
				temp.RemovePart(EQUIP_HAIR);
				temp.LoadEntityFile(String().c_str(), gGlobal.pD3DDevice, EQUIP_HEAD_ACCESSORY);

				buf.Format(_T("j%05d_t%03d.ent"), sexClass, head);
				buf2.Format(_T("j%05d_t%03d%c.tga"), sexClass, head, abc);
				temp.LoadEntityFile(buf.c_str(), gGlobal.pD3DDevice, EQUIP_HAIR);
				temp.ChangeColorP(buf.c_str(), buf2.c_str());
			}
		}
		else
		{
			temp.RemovePart(EQUIP_HEAD_ACCESSORY);
			temp.RemovePart(EQUIP_HAIR);
			temp.LoadEntityFile(String().c_str(), gGlobal.pD3DDevice, EQUIP_HEAD_ACCESSORY);

			buf.Format(_T("j%05d_t%03d.ent"), sexClass, head);
			buf2.Format(_T("j%05d_t%03d%c.tga"), sexClass, head, abc);
			temp.LoadEntityFile(buf.c_str(), gGlobal.pD3DDevice, EQUIP_HAIR);
			temp.ChangeColorP(buf.c_str(), buf2.c_str());
		}
	}


	//for body
	{
		buf2.Empty();
		const ItemData* pItemData = gGlobal.m_resource->GetItemData(cloth );
		if( pItemData )
		{
			if (sexClass == pItemData->reqSexClass1)
				buf.Format(_T("%s"), pItemData->eq_Outlook1);
			else if (sexClass == pItemData->reqSexClass2)
				buf.Format(_T("%s"), pItemData->eq_Outlook2);
			else
				buf.Format(_T("b%05d_t%03d.ent"), sexClass, cloth);

			switch(clothColor)
			{
			case 1: buf2 = String(pItemData->eq_colorPalette1);	break;
			case 2: buf2 = String(pItemData->eq_colorPalette2);	break;
			case 3: buf2 = String(pItemData->eq_colorPalette3);	break;
			case 4: buf2 = String(pItemData->eq_colorPalette4);	break;
			case 5: buf2 = String(pItemData->eq_colorPalette5);	break;
			}
		}
		else
			buf.Format(_T("b%05d_t%03d.ent"), sexClass, cloth);
		
		temp.LoadEntityFile(buf.c_str(), gGlobal.pD3DDevice, EQUIP_BODY);

		if (buf2.IsEmpty() && pItemData == NULL)
		{
			abc = 96 + clothColor;
			//color of body
			buf2.Format(_T("%s%c.tga"), buf.substr(0 , buf.size() - 4).c_str(), abc);
		}
		temp.ChangeColorP(buf.c_str(), buf2.c_str());
	}

	const ItemData * pData = gGlobal.m_resource->GetItemData(eq_weapon);
	if (pData)
	{
		if (sexClass == pData->reqSexClass1)
			temp.LoadEntityFile(pData->eq_Outlook1, gGlobal.pD3DDevice, EQUIP_WEAPON);
		else
			temp.LoadEntityFile(pData->eq_Outlook2, gGlobal.pD3DDevice, EQUIP_WEAPON);
	}

	return temp;
}	

VOID Global::AddNameToRecentChatList(String Name)
{
	for (Int i = 0; i < (Int)g_RecentPriChatCharsLst.size(); i++)
	{
		if (g_RecentPriChatCharsLst[i] == Name)
			return;
	}

	if (g_RecentPriChatCharsLst.size() >= 5)
		g_RecentPriChatCharsLst.erase(g_RecentPriChatCharsLst.begin());
	g_RecentPriChatCharsLst.push_back(Name);
}

VOID Global::NotifyReceivePrivateMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName
										 , const String &TargetName)
{
	if (SrcName != g_pCharacter->GetCharNetObject()->GetNickName())
	{
		g_RecentPriChatChar = SrcName;

		AddNameToRecentChatList(SrcName);
		InitRecentChatListBox();
	}
	else
	{
		g_RecentPriChatChar = TargetName;
		AddNameToRecentChatList(TargetName);
		InitRecentChatListBox();
	}

	PrintMessage(Msg, CHANNELTYPE_PRIVATE, SrcName, TargetName);
}

CChannelControl * Global::GetChannelControl(BYTE Type)
{
	BYTEChannelCtrlMap::iterator Itr = m_MapChannelLst.find(Type);
	if (Itr != m_MapChannelLst.end())
	{
		return Itr->second;	
	}
	return NULL;
}


VOID Global::NotifyCB_SendPrivateMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const boolean &isFriend, const UInt32 &resultFlag)
{
	if (Result == MSG_RETURN_FAIL_TOO_FREQUENT)
	{
		String text;
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_TOOFREQ")),  resultFlag);
		if (isFriend)
			g_pMenuFriendProcess->PrintMessage(text, 
			g_pCharacter->GetCharName(), g_pCharacter->GetCharName());
		else
			PrintMessage(text, CHANNELTYPE_SYSTEM, String());


	}
	else if (Result == MSG_RETURN_FAIL_NOT_FOUND)
	{
		if (isFriend)
			g_pMenuFriendProcess->PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_NOTARGET")), 
			g_pCharacter->GetCharName(), g_pCharacter->GetCharName());
		else
			PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_NOTARGET")), CHANNELTYPE_SYSTEM, String());

	}
	else if (Result == MSG_RETURN_FAIL_TOO_MUCH)
	{
		if (isFriend)
			g_pMenuFriendProcess->PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_TOOMUCH")), 
			g_pCharacter->GetCharName(), g_pCharacter->GetCharName());
		else
			PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_TOOMUCH")), CHANNELTYPE_SYSTEM, String());
	}
	else if (Result == MSG_RETURN_FAIL_INACTIVE)
	{
		if (isFriend)
			g_pMenuFriendProcess->PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_INACTIVE")), 
			g_pCharacter->GetCharName(), g_pCharacter->GetCharName());
		else
			PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_INACTIVE")), CHANNELTYPE_SYSTEM, String());
	}
	else if (Result == MSG_RETURN_FAIL_NO_FRIEND_ONLINE)
	{
		PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_NO_FRIEND")), CHANNELTYPE_SYSTEM, String());
	}
}
VOID Global::UpDateHorseLightMsg(UInt32 CurrTime)
{
	if (!g_HorseLightDisplay || pUIManager == NULL)
		return;

	UIPanel * pPanel = pUIManager->FindUIPanel("HroseLight");
	if (pPanel == NULL)
		return;

	UIDynamicText * pDynamicText = dynamic_cast<UIDynamicText *>(pUIManager->GetUIItem("HroseLightText"));
	if (pDynamicText == NULL)
		return;

	if (pDynamicText->GetCount() >= g_HorseLightCount)
	{
		if (g_HorseLightMsgLst.size() == 0)
		{
			pDynamicText->SetEnable(false);
			pDynamicText->Reset();
			pPanel->SetEnable(false);
			pUIManager->AddDirtyUIItem(pPanel);
			g_HorseLightDisplay = false;
			g_HorseLightCount = 0;
		}
		else
		{
			pDynamicText->SetText(g_HorseLightMsgLst[0].c_str());
			pDynamicText->Reset();
			pDynamicText->Redraw();
			pDynamicText->SetEnable(true);
			pPanel->SetEnable(true);
			pUIManager->AddDirtyUIItem(pPanel);

			g_HorseLightMsgLst.erase(g_HorseLightMsgLst.begin());
			g_HorseLightDisplay = true;
			g_HorseLightCount = g_HorseLightCountLst[0];
			g_HorseLightCountLst.erase(g_HorseLightCountLst.begin());
		}
	}

}

VOID Global::MarkMouseDirtyRect()
{
	RECT rect;
	Int32	CX = pUIManager->GetCursorX() + pUIManager->GetCursorOffsetX();
	Int32	CY = pUIManager->GetCursorY() + pUIManager->GetCursorOffsetY();

	SetRect(&rect, CX, CY,
		CX + CursorEntity->m_EntityTotalRect.right - CursorEntity->m_EntityTotalRect.left,
		CY + CursorEntity->m_EntityTotalRect.bottom - CursorEntity->m_EntityTotalRect.top);
	pUIManager->GetDirtyRectMan()->AddDirtyRectToMan(rect);
}
VOID Global::NotifyReceiveHorseLightMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &count)
{
	if (!Msg.IsEmpty())
	{
		if (!g_HorseLightDisplay)
			g_HorseLightCount = 0;
		g_HorseLightMsgLst.push_back(Msg);
		g_HorseLightCountLst.push_back(count);
		g_HorseLightDisplay = true;

	}
}

VOID Global::LeaveChannel(BYTE Type)
{
	BYTEChannelCtrlMap::iterator Itr = m_MapChannelLst.find(Type);
	if (Itr != m_MapChannelLst.end())
	{
		if (Itr->second->GetChannnelNetObject())
			Itr->second->GetChannnelNetObject()->LeaveChannel(pPeerProxy, g_context, g_pCharacter->GetMapActorID());
		SafeDeleteObject(Itr->second);
		m_MapChannelLst.erase(Itr);
	}
}

VOID Global::NotifyCB_JoinMessageChannel(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	String CombineString = "";
	if (Result == CHANNEL_NO_ERR)
	{
		if (gGlobal.g_pMenuFriendProcess && gGlobal.g_pMenuFriendProcess->m_bInit)
		{
			CombineString =gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_JOIN"));
		}
	}
	else if (Result == CHANNELERR_NOT_FOUND)
		CombineString =gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_NOTFOUND"));
	else if (Result == CHANNELERR_CHANNEL_EXIST)
		CombineString =gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_EXISTED"));

	PrintMessage(CombineString, CHANNELTYPE_SYSTEM, String());
}


VOID Global::AddChatBalloon(String Msg, BYTE Type, String SrcName, Int Index, UInt32 delay,BYTE speaker_type)
{
	if (SrcName.IsEmpty() && gConfigure.uNetwork)
		return;

	//limit the balloon count
	if (gGlobal.g_balloonCount >= gConfigure.uballonLimit)
		return;

	switch (Type)
	{
	case CHANNELTYPE_MAP:
	case CHANNELTYPE_PARTY:
		{
			CCharacterControl * pChatCtrl = NULL;

			if  (gConfigure.uNetwork == 0)
			{
				pChatCtrl = gGlobal.g_pCharacter;
			}
			else
			{

				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && Type == CHANNELTYPE_MAP)
					pChatCtrl = QGLApplication::GetInstance().FindCharCtrlByName(SrcName);
				else if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					if ( Index != -1 )
						pChatCtrl = gGlobal.g_pBattleControl->m_BattlePlayerLst[Index].pChar;
					else
						pChatCtrl = QGLApplication::GetInstance().FindCharCtrlByNameInBattle(SrcName, speaker_type);
				}
			}

			if (pChatCtrl)
			{
				CUIEntity * pImEnt = SafeCreateObject(CUIEntity);

				pImEnt->ReadXMLData("Data\\ChatBalloon.xml");
				/*
				UIChatPanel * pChatPanel = gGlobal.pUIManager->FindUIChatPanel("ChatBalloonContext");
				pImEnt->SetChatPanel(pChatPanel);

				pChatPanel = pImEnt->GetUIManager()->FindUIChatPanel("ChatBalloonContext");
				pImEnt->SetChatPanel(pChatPanel);

				UIPanelEx * pPanelEx = pImEnt->GetUIManager()->FindUIPanelEx("ChatBalloon");
				pImEnt->SetPanelEx(pPanelEx);
				*/

				pImEnt->SetMsg(Msg.c_str());
				pImEnt->SetupPanel();

				/*
				if (pChatPanel)
				{
					
					
					pChatPanel->AddStringWNoPrefix(Msg.c_str());
					pChatPanel->Start2BottomRow();

					if (pChatPanel->GetEndRowIndex() != pChatPanel->RowListCount() - 1)
					{
						UIChatRow * pChatRow = pChatPanel->GetRow(pChatPanel->GetEndRowIndex());
						
						if (pChatRow)
						{
							pChatRow->ResetChatItemsPos();
							//remove last two chat item and insert two . .

							UIChatItem * pChatItem = pChatRow->GetChatItem((UInt32)pChatRow->GetRowItemList()->size() - 1);
							if (pChatItem)
							{
								UIChatAniIcon * pChatAniIcon = dynamic_cast<UIChatAniIcon *>(pChatItem);
								if (pChatAniIcon)
								{
									//Char FileName[50];
									//FileName = _T("..");
									//GetPrivateProfileString(_T("LOADFILE"), _T("MESSAGEOVER"), _T(".."), FileName, 50, CONFIGURATION_INI_FILE);

									StringW WStr;
									UInt32 Len;
									pChatPanel->GetFont()->DrawFontStrC(_T(".........................................."), WStr, pChatItem->GetPosX() + pChatAniIcon->GetWidth() + 5, 0, 
										pChatPanel->GetWidth(), pChatPanel->GetPitch(), pChatPanel->GetFontStyle(), Len);

									//pChatRow->GetRowItemList()->pop_back();
									UIChatString *MyStr = SafeCreateObject(UIChatString);
									MyStr->SetString(WStr.c_str());
									MyStr->SetFontColor(pChatPanel->GetFontColor());
									MyStr->SetShadowColor(pChatPanel->GetShadowColor());
									MyStr->SetFontStyle(pChatPanel->GetFontHeight());
									MyStr->SetWidth(pChatPanel->GetFont()->GetLastDrawWidth() - pChatItem->GetPosX());

									UInt32	PitchHeight, RealFontHeight;
									pChatPanel->GetFont()->TakePitchHeight(pChatPanel->GetFontStyle(), PitchHeight, RealFontHeight);

									MyStr->SetHeight(pChatPanel->GetFont()->GetHeight() + PitchHeight);

									pChatRow->AddItem(MyStr);
								}
								else
								{
									UIChatString * pChatString = dynamic_cast<UIChatString *>(pChatItem);
									if (pChatString)
									{
										StringW OrgText = pChatString->GetString();

										if (OrgText.size() >= 3)
											OrgText.erase(OrgText.size() - 3);
										else
											OrgText.clear();

										//Char FileName[50];
										//FileName =  _T("..");
										//GetPrivateProfileString(_T("LOADFILE"), _T("MESSAGEOVER"), _T(".."), FileName, 50, CONFIGURATION_INI_FILE);
										OrgText.append(_T(".........................................."));

										StringW WStr;
										UInt32 Len;
										pChatPanel->GetFont()->DrawFontStrC(OrgText.c_str(), WStr, pChatItem->GetPosX(), 0, 
											pChatPanel->GetWidth(), pChatPanel->GetPitch(), pChatPanel->GetFontStyle(), Len);
										pChatString->SetString(WStr.c_str());

									}
								}

							}
						}
					}
					pChatPanel->HomeRow();
				}

				UIPanelEx * pPanelEx = pImEnt->GetUIManager()->FindUIPanelEx("ChatBalloon");
				if (pPanelEx && (pPanelEx->GetLineRect()->bottom > pPanelEx->GetLineRect()->top))
				{

					UInt32 LineCount = (UInt32)ceil((Real32)pChatPanel->GetTotalHeight() / 
						(pPanelEx->GetLineRect()->bottom - pPanelEx->GetLineRect()->top));
					pPanelEx->ChangeCount(LineCount);
					pImEnt->ResetRect();
				}
				*/
				pChatCtrl->AddChatBalloon(pImEnt, delay);				
				
			}

			if (Type == CHANNELTYPE_PARTY)
			{
				if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_MAP)
					break;
				int partyBalloonEnabled = GetPrivateProfileInt(_T("DISPLAYCOMPONENT"), _T("PartyBalloonEnable"), 0, CONFIGURATION_INI_FILE);
				if( partyBalloonEnabled > 0 )
				{
					CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
					UInt memberPos = pParty->FindMemberPos(SrcName);
					if( memberPos > 0 )
					{
						StringA componentName;
						componentName.Format("ChatBalloonPartyContext%d", memberPos);
						UIChatPanel* pChatPanel;
						pChatPanel = pUIManager->FindUIChatPanel(componentName.c_str());
						if( pChatPanel==NULL )
						{
							XML_UI::GetInstance().AddFormFile(StringA("Data/ChatBalloonParty.xml").c_str(), gGlobal.pUIManager);
							pChatPanel = pUIManager->FindUIChatPanel(componentName.c_str());
						}
						if( pChatPanel )
						{
							pUIManager->AddDirtyUIItem(pChatPanel);
							pChatPanel->ClearAllRow();
							pChatPanel->SetEnable(true);
							pChatPanel->AddStringWNoPrefix(Msg.c_str());
							pChatPanel->Start2BottomRow();
							if (pChatPanel->GetEndRowIndex() != pChatPanel->RowListCount() - 1)
							{
								UIChatRow * pChatRow = pChatPanel->GetRow(pChatPanel->GetEndRowIndex());
								
								if (pChatRow)
								{
									pChatRow->ResetChatItemsPos();
									//remove last two chat item and insert two . .

									UIChatItem * pChatItem = pChatRow->GetChatItem((UInt32)pChatRow->GetRowItemList()->size() - 1);
									if (pChatItem)
									{
										UIChatAniIcon * pChatAniIcon = dynamic_cast<UIChatAniIcon *>(pChatItem);
										if (pChatAniIcon)
										{
//											Char FileName[50];
//											 FileName = _T("..");
											//GetPrivateProfileString(_T("LOADFILE"), _T("MESSAGEOVER"), _T(".."), FileName, 50, CONFIGURATION_INI_FILE);

											StringW WStr;
											UInt32 Len;
											pChatPanel->GetFont()->DrawFontStrC(_T(".........................................."), WStr, pChatItem->GetPosX() + pChatAniIcon->GetWidth() + 5, 0, 
												pChatPanel->GetWidth(), pChatPanel->GetPitch(), pChatPanel->GetFontStyle(), Len);

											//pChatRow->GetRowItemList()->pop_back();
											UIChatString *MyStr = SafeCreateObject(UIChatString);
											MyStr->SetString(WStr.c_str());
											MyStr->SetFontColor(pChatPanel->GetFontColor());
											MyStr->SetShadowColor(pChatPanel->GetShadowColor());
											MyStr->SetFontStyle(pChatPanel->GetFontHeight());
											MyStr->SetWidth(pChatPanel->GetFont()->GetLastDrawWidth() - pChatItem->GetPosX());

											UInt32	PitchHeight, RealFontHeight;
											pChatPanel->GetFont()->TakePitchHeight(pChatPanel->GetFontStyle(), PitchHeight, RealFontHeight);

											MyStr->SetHeight(pChatPanel->GetFont()->GetHeight() + PitchHeight);

											pChatRow->AddItem(MyStr);
										}
										else
										{
											UIChatString * pChatString = dynamic_cast<UIChatString *>(pChatItem);
											if (pChatString)
											{
												StringW OrgText = pChatString->GetString();

												if (OrgText.size() >= 3)
													OrgText.erase(OrgText.size() - 3);
												else
													OrgText.clear();

												//Char FileName[50];
												//FileName = _T("..");
												//GetPrivateProfileString(_T("LOADFILE"), _T("MESSAGEOVER"), _T(".."), FileName, 50, CONFIGURATION_INI_FILE);
												OrgText.append(_T(".........................................."));

												StringW WStr;
												UInt32 Len;
												pChatPanel->GetFont()->DrawFontStrC(OrgText.c_str(), WStr, pChatItem->GetPosX(), 0, 
													pChatPanel->GetWidth(), pChatPanel->GetPitch(), pChatPanel->GetFontStyle(), Len);
												pChatString->SetString(WStr.c_str());

											}
										}

									}
								}
							}
							pChatPanel->HomeRow();
							pChatPanel->SetEnable(true);
							pChatPanel->SetDetectRect();
							pUIManager->AddDirtyUIItem(pChatPanel);
						}
						componentName.Format("ChatBalloonParty%d", memberPos);
						UIPanelEx * pPanelEx = pUIManager->FindUIPanelEx(componentName.c_str());
						if (pPanelEx && (pPanelEx->GetLineRect()->bottom > pPanelEx->GetLineRect()->top) &&
							(pChatPanel->GetTotalHeight() > 10))
						{
							UInt32 LineCount = (UInt32)ceil(((Real32)pChatPanel->GetTotalHeight()-10) / // 10 is the height of header and footer minus margin
								(pPanelEx->GetLineRect()->bottom - pPanelEx->GetLineRect()->top));
							pPanelEx->ChangeCount(LineCount);
							pPanelEx->SetEnable(true);
							pPanelEx->SetDetectRect();
							pUIManager->AddDirtyUIItem(pPanelEx);
						}
					}
				}
				break;
			}
			break;
		}


	default:
		return;
	}
}

VOID Global::PrintMessage(String Msg, BYTE Type, String SrcName, String TargetName, BYTE speaker_type)
{
	VMProtectBegin; VM_START;

	if (Msg.IsEmpty())
		return;

	if (Type >= MAX_CHANNELTYPE)
		return;

	if (Type == CHANNELTYPE_RUMOR && !gGlobal.g_ClientistenToChannel[Type])
	{
		return;
	}
	if (Type == CHANNELTYPE_SPEAKER || Type == CHANNELTYPE_GLOBAL)
	{
		if (NoCodeStringSize(Msg) > gConfigure.maxMsgLen)
		{
			SubNoCodeString(Msg, gConfigure.maxMsgLen);
		}
	}

	AddChatBalloon(Msg, Type, SrcName, -1, speaker_type);

	String CombineString = "";
	if (Type != CHANNELTYPE_SPEAKER)
	{
		CombineString += gConfigure.strMsgDel.c_str();
		CombineString += gConfigure.strChannelMsgCom[Type];
	}
	if (Type == CHANNELTYPE_PRIVATE)
	{
		Char buf[256];

		if (gGlobal.g_pCharacter->GetCharName() == SrcName)
		{
			String			TempStr = "\\[NAME:";

			TempStr += TargetName.c_str();
			TempStr += "\\]";
			TempStr += TargetName.c_str();
			TempStr += "\\#";

			_stprintf(buf,gGlobal.GetStringTable()->Get(_T("MSE_CHANNEL_PRIVATE_SENDER")), TempStr.c_str());
		}
		else
		{
			String			TempStr = "\\[NAME:";

			TempStr += SrcName.c_str();
			TempStr += "\\]";
			TempStr += SrcName.c_str();
			TempStr += "\\#";

			_stprintf(buf,gGlobal.GetStringTable()->Get(_T("MSE_CHANNEL_PRIVATE_RECEIVER")), TempStr.c_str());
		}

		CombineString += "#";
		CombineString += ChannelColor[Type];
		CombineString += buf;
		CombineString += ":";
		CombineString += "#";
		CombineString += ChannelColor[Type];

	}
	else if (!SrcName.IsEmpty())//Type != CHANNELTYPE_SYSTEM)
	{
		CombineString += "#W";
		CombineString += "\\[NAME:";
		CombineString += SrcName.c_str();
		CombineString += "\\][";
		CombineString += SrcName.c_str();
		CombineString += "]\\#";

		CombineString += ":";
		if (Type != CHANNELTYPE_SYSTEM)
		{
			CombineString += "#";
			CombineString += ChannelColor[Type];
		}
	}
	else
	{
		CombineString += "#";
		CombineString += ChannelColor[Type];
	}

	CombineString += Msg.c_str();
	if (Type != CHANNELTYPE_SPEAKER)
	{
		GetChatPool()->Add(CombineString.c_str());
	}
	if (QGLApplication::GetInstance().GetSubWindow()->m_UIManager && Type != CHANNELTYPE_SPEAKER)
	{
		UIChatPanel * pChatPanel = QGLApplication::GetInstance().GetSubWindow()->m_UIManager->FindUIChatPanel("SubChat");
		if (pChatPanel)
		{
			pChatPanel->AddStringW(CombineString.c_str());
			pChatPanel->EndRow();
			UIScrollBarV * pScroll = QGLApplication::GetInstance().GetSubWindow()->m_UIManager->FindUIScrollBarV("SubChatScrollButton");
			if (pScroll)
			{
				if (pChatPanel->GetAllRowHeight() > pChatPanel->GetHeight())
				{
					pScroll->SetNumCount(pChatPanel->RowListCount());
				}
				else
					pScroll->SetNumCount(0);

				if (pChatPanel->GetAutoLock())
					pScroll->EndScroll();
			}
			QGLApplication::GetInstance().GetSubWindow()->m_UIManager->RenderAll();
		}
	}
	else
	{
		if (Type == CHANNELTYPE_SPEAKER)
		{
			UIChatPanel * pChatPanel = gGlobal.pUIManager->FindUIChatPanel("Speaker");
			if (pChatPanel)
			{
				String strTemp;
				String strFinal;
				bool bFindIcon(false);
				while (!CombineString.IsEmpty())
				{
					String::size_type pos = CombineString.find(_T("#"));
					strTemp = CombineString.substr(0,pos);
					strFinal += strTemp;
					CombineString = CombineString.substr(pos+1);
					if ((!strTemp.IsEmpty() || !CombineString.IsEmpty()) && pos != String::npos)
					{
						int num = _wtoi(CombineString.c_str());
						if (num > 0)
						{
							bFindIcon = true;
							if (num > 99)
							{
								num = 3;
							}
							else if (num > 9)
							{
								num = 2;
							}
							else
							{
								num = 1;
							}
							if (CombineString.GetAt(0) == '0')
							{
								num++;
							}
							if (CombineString.GetAt(1) == '0')
							{
								num++;
							}
							CombineString = CombineString.substr(num);
						}
						else
							strFinal += _T("#");
					}
					else
					{
						break;
					}
					pos = CombineString.find(_T("#"));
				}
				if (bFindIcon && gGlobal.g_pCharacter->GetCharName() == SrcName)
				{
					PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_CHANNEL_SPEAKER_NO_ICON")), CHANNELTYPE_SYSTEM);
				}
				m_uSpeakerDisappearTime = GetTickCount() + SPEAKER_DISAPPEAR_TIME;
				switch (m_SpeakerState)
				{
				case SPEAKER_STATE_HIDE:
					{
						UIPanelEx*		pSpeakerpanel = gGlobal.pUIManager->FindUIPanelEx("SpeakerPanel");
						UIChatPanel *	pChatPanel = gGlobal.pUIManager->FindUIChatPanel("Speaker");
						UIImage*		pImage = gGlobal.pUIManager->FindUIImage("SpeakerScrollRight");
						UIImage*		pImage2 = gGlobal.pUIManager->FindUIImage("SpeakerScrollLeft");
						if (pChatPanel && pSpeakerpanel && pImage && pImage2)
						{
							pSpeakerpanel->SetEnable(true);
							pSpeakerpanel->SetWH(MIN_SPEAKER_WIDTH,pSpeakerpanel->GetHeight());
							pSpeakerpanel->SetDetectRect();
							m_SpeakerState = SPEAKER_STATE_OPENING;
							pChatPanel->ClearAllRow();
							pChatPanel->AddStringW(strFinal.c_str());
							pChatPanel->EndRow();
							gGlobal.pUIManager->AddDirtyUIItem(pChatPanel);
							pImage->SetOffsetXY(pImage2->GetOffsetX() + MIN_SPEAKER_WIDTH / 2,pImage2->GetOffsetY());
							m_uSpeakerLastRenderTime = GetTickCount();
						}
					}
					break;
				case SPEAKER_STATE_SHOW:
				case SPEAKER_STATE_OPENING:
				case SPEAKER_STATE_CLOSING:
					{
						m_SpeakerLst.push_back(strFinal);
					}
					break;
				}
			}
		} 
		else
		{
			UIChatPanel * pChatPanel = gGlobal.pUIManager->FindUIChatPanel("Chat");
			if (pChatPanel)
			{
				pChatPanel->AddStringW(CombineString.c_str());
				pChatPanel->EndRow();
				gGlobal.pUIManager->AddDirtyUIItem(pChatPanel);
			}
		}
	}
	//UIEditBox * pBox = gGlobal.pUIManager->FindUIEditBox("EditLine");
	//if (pBox)
	//	pBox->SetText(CombineString.c_str());
	//CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PrintMessage", CombineString.c_str());
	//PrintMesage
	/*
	UIChatRow * pChatRow = pCharPanel->GetLastRow(); 
	pChatRow->AddStringW(CombineString.c_str(), TextColor, 0xFF000000, EDGE_SHADOW, pCharPanel->GetFontHeight());
	pCharPanel->CreateRow();
	pCharPanel->EndRow();
	pTempUIManager->RenderAll();
	*/

	VMProtectEnd; VM_END;
}

VOID Global::UpdateSpeaker()
{
	if (pUIManager == NULL)
		return;

	UInt32 uNowTime = GetTickCount();
	switch (m_SpeakerState)
	{
	case SPEAKER_STATE_OPENING:
		{
			UIPanelEx*		pSpeakerpanel = gGlobal.pUIManager->FindUIPanelEx("SpeakerPanel");
			UIImage*		pImage = gGlobal.pUIManager->FindUIImage("SpeakerScrollRight");
			UInt32			nOffSet = SPEAKER_OFFSET * (uNowTime - m_uSpeakerLastRenderTime) / 30;
			m_uSpeakerLastRenderTime = uNowTime;
			if (pSpeakerpanel && pImage)
			{
				UInt32 width = pSpeakerpanel->GetWidth() + nOffSet;
				if (width > MAX_SPEAKER_WIDTH)
				{
					m_SpeakerState = SPEAKER_STATE_SHOW;
				}
				pSpeakerpanel->SetWH(width,pSpeakerpanel->GetHeight());
				pSpeakerpanel->SetDetectRect();
				pImage->SetOffsetXY(pImage->GetOffsetX()+nOffSet,pImage->GetOffsetY());
				gGlobal.pUIManager->AddDirtyUIItem(pImage);
				gGlobal.pUIManager->AddDirtyUIItem(pSpeakerpanel);
			}
		}
		break;
	case SPEAKER_STATE_SHOW:
		{
			if(!m_SpeakerLst.empty() || uNowTime > m_uSpeakerDisappearTime)
			{
				m_SpeakerState = SPEAKER_STATE_CLOSING;
				m_uSpeakerLastRenderTime = uNowTime;
			}
		}
		break;
	case SPEAKER_STATE_CLOSING:
		{
			UIPanelEx*		pSpeakerpanel = gGlobal.pUIManager->FindUIPanelEx("SpeakerPanel");
			UIImage*		pImage = gGlobal.pUIManager->FindUIImage("SpeakerScrollRight");
			UIImage*		pImage2 = gGlobal.pUIManager->FindUIImage("SpeakerScrollLeft");
			UInt32			nOffSet = SPEAKER_OFFSET * (uNowTime - m_uSpeakerLastRenderTime) / 30;
			m_uSpeakerLastRenderTime = uNowTime;
			if (pSpeakerpanel && pImage)
			{
				UInt32 width = pSpeakerpanel->GetWidth() - nOffSet;
				pImage->SetOffsetXY(pImage->GetOffsetX()-nOffSet,pImage->GetOffsetY());
				if (width < MIN_SPEAKER_WIDTH || !m_SpeakerLst.empty())
				{
					width = MIN_SPEAKER_WIDTH;
					UIChatPanel *	pChatPanel = gGlobal.pUIManager->FindUIChatPanel("Speaker");
					if (pChatPanel)
					{
						pChatPanel->ClearAllRow();
						m_uSpeakerDisappearTime = 0;
						pChatPanel->EndRow();
						gGlobal.pUIManager->AddDirtyUIItem(pChatPanel);
					}
					if (!m_SpeakerLst.empty() && pChatPanel)
					{
						String strNextSpeaker;
						SpeakerList::iterator Iter = m_SpeakerLst.begin();
						if (Iter != m_SpeakerLst.end())
						{
							strNextSpeaker = *Iter;
							m_SpeakerLst.erase(Iter);
							m_uSpeakerDisappearTime = GetTickCount() + SPEAKER_DISAPPEAR_TIME;
						}
						m_SpeakerState = SPEAKER_STATE_OPENING;
						pChatPanel->ClearAllRow();
						pChatPanel->AddStringW(strNextSpeaker.c_str());
						pChatPanel->EndRow();
						gGlobal.pUIManager->AddDirtyUIItem(pChatPanel);
					} 
					else
					{
						m_SpeakerState = SPEAKER_STATE_HIDE;
						pSpeakerpanel->SetEnable(false);
					}
					pImage->SetOffsetXY(pImage2->GetOffsetX()+MIN_SPEAKER_WIDTH / 2,pImage2->GetOffsetY());
				}
				pSpeakerpanel->SetWH(width,pSpeakerpanel->GetHeight());
				pSpeakerpanel->SetDetectRect();
				gGlobal.pUIManager->AddDirtyUIItem(pImage);
				gGlobal.pUIManager->AddDirtyUIItem(pSpeakerpanel);
			}
		}
		break;
	}
}


VOID Global::InitChannelInfo()
{
	m_CurrChannelType = CHANNELTYPE_MAP;
}

CChannelControl * Global::GetCurrChannelControl()
{
	BYTEChannelCtrlMap::iterator Itr = m_MapChannelLst.find(m_CurrChannelType);
	if (Itr != m_MapChannelLst.end())
	{
		return Itr->second;	
	}
	return NULL;
}

VOID Global::UpdateChannelButtonState(Int Index)
{
	if (Index >= MAX_CHANNELTYPE)
		return;

	UICheckBox * pBuRec = gGlobal.pUIManager->FindUICheckBox(ReceiveChannelXMLName[Index]);
	if (pBuRec)
	{
		pBuRec->SetState(!g_ClientistenToChannel[Index]);
		gGlobal.pUIManager->AddDirtyUIItem(pBuRec);
	}

	if (QGLApplication::GetInstance().GetSubWindow()->m_UIManager)
	{
		UICheckBox * pSubBuRec = QGLApplication::GetInstance().GetSubWindow()->m_UIManager
			->FindUICheckBox(SubReceiveChannelXMLName[Index]);
		if (pSubBuRec)
		{
			pSubBuRec->SetState(!g_ClientistenToChannel[Index]);
			QGLApplication::GetInstance().GetSubWindow()->m_UIManager->AddDirtyUIItem(pSubBuRec);
		}

	}
}

VOID Global::SetCurrChannelType(BYTE Type)
{
	m_CurrChannelType = Type;
	for (BYTE i = 0; i < MAX_CHANNELTYPE; i++)
	{
		UIImage * pBu = pUIManager->FindUIImage(ChannelXMLName[i]);
		if (pBu)
		{
			if (i == Type)
				pBu->SetEnable(TRUE);
			else
				pBu->SetEnable(FALSE);
			pUIManager->AddDirtyUIItem(pBu);
		}
		
	}
	BYTEChannelCtrlMap::iterator Itr = gGlobal.m_MapChannelLst.find(Type);
	if (Itr != gGlobal.m_MapChannelLst.end())
	{
		Itr->second->SetListenToChannel(TRUE);
		g_ClientistenToChannel[Type] = true;
		UpdateChannelButtonState(Type);

		UIEditBox * pEdit = pUIManager->FindUIEditBox("EditLine");
		if (pEdit)
		{
			if (m_CurrChannelType == CHANNELTYPE_MAP ||
				m_CurrChannelType == CHANNELTYPE_GLOBAL ||
				m_CurrChannelType == CHANNELTYPE_SPEAKER)			
				pEdit->SetEditLimit(40);			
			else
				pEdit->SetEditLimit(100);
		}
	}
	else if (Type == CHANNELTYPE_MAP)
	{
		if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			gGlobal.g_pCharacter->GetCharNetObject()->SetListen(gGlobal.pPeerProxy, gGlobal.g_context, TRUE);
	}
	else if (Type == CHANNELTYPE_FRIEND)
	{
		if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			gGlobal.g_pCharacter->GetCharNetObject()->SetListen(gGlobal.pPeerProxy, gGlobal.g_context, TRUE);
	}
}

VOID Global::RemoveChannelCtrl(CChannelControl * pChannelCtrl)
{
	BYTEChannelCtrlMap::iterator Itr = m_MapChannelLst.begin();
	while (Itr != m_MapChannelLst.end())
	{
		if (Itr->second == pChannelCtrl)
		{
			SafeDeleteObject(Itr->second);
			m_MapChannelLst.erase(Itr);
			break;
		}
		Itr ++;
	}
}

CCharacterControl * Global::GetBattleController()
{
	if (g_pBattleControl)
	{
		return g_pBattleControl->GetControlPlayer();
	}
	return NULL;
}

NPCDB_Client* Global::FindNPCByTemplateID(UInt32 TemplateID)
{
	std::map<UInt32, NPCDB_Client*>::iterator Itr = m_npcdb.find(TemplateID);
	if (Itr != m_npcdb.end())
		return Itr->second;

	return NULL;
}

String Global::GetChatIconCodebyNum(UInt Code)
{
	String TempStr;
	TempStr.Format(_T("#%03d"), Code);

	return TempStr;
}

String Global::GetEmotionFilePathbyNum(UInt Code)
{
	String TempStr;
	TempStr.Format(_T("%s%03d.ent"), EMOTION_ICON_PATH, Code);

	return TempStr;
}

String Global::GetChatIconFilePathbyNum(UInt Code)
{
	String TempStr;
	TempStr.Format(_T("ChatIcon\\ChatIcon%03d.ent"), Code);
	return TempStr;
}

VOID Global::AddColorPrefix(String & Msg)
{
	if (g_SelectTextColorIndex >= 0  &&
		g_SelectTextColorIndex < MaxTextColor)
	{
		String Temp = "#";
		Temp += TextColorStr[g_SelectTextColorIndex];
		Temp += Msg.c_str();
		Msg = Temp;
		g_SelectTextColorIndex = -1;
		UIEditBox * pEditBox = gGlobal.pUIManager->FindUIEditBox("EditLine");
		if (pEditBox)
		{
			pEditBox->SetFontColor(0xFFFFFFFF);
		}
	}
}

VOID Global::BlockColorWord(String &Msg)
{
	/*
	const UInt MaxTextColor = 8;
	const static String TextColorStr[MaxTextColor] ={ 
	"#O",
	"#Y",
	"#G",
	"#A",
	"#B",
	"#V",
	"#P",		
	"#W",
	};
	*/
	//static String ColorCode = "OYGABVPW";

	String TempStr;
	bool	DontCare = false;

	for(UInt32 i = 0; i < Msg.size(); ++i)
	{
		WCHAR	W = Msg[i];
		if(Msg[i] == 15)
			DontCare = true;
		else
			if(Msg[i] == 16)
				DontCare = false;

		if(i + 1 < Msg.size() && !DontCare)
		{
			if(Msg[i] == _T('#'))
			{
				if(Msg[i + 1] < _T('0') || Msg[i + 1] > _T('9'))
				{
					TempStr += _T("##");
					continue;
				}
			}
			else
				if(Msg[i] == _T('\\'))
				{
					TempStr += _T("\\\\");
					continue;
				}
		}

		TempStr += Msg[i];
	}

	Msg = TempStr;

	/*
	String nilstr = _T("");
	for (UInt32 i = 0; i < MaxTextColor; ++i)
	{
	int temp = (int)Msg.find(TextColorStr[i].c_str());
	UInt32 j = 0;
	while (temp >= 0 && j < Msg.size() )
	{
	Msg.replace(temp, TextColorStr[i].length(), nilstr.c_str());
	temp = (int)Msg.find(TextColorStr[i].c_str());
	++j;
	}
	}
	*/
}
BOOL Global::CanUsedName(String name)
{
	BlockWordDataList* blockWord =	gGlobal.m_resource->GetNameBlockWordList();
	if (blockWord == NULL)
		return false;

	String checkingWord;

	UInt8 spaceHeadIndex = 0;
	UInt8 spaceLastIndex = 0;
	String space = _T(" ");

	bool cantUse = false;//cannot use
	//remove all the space first
	do 
	{
		spaceLastIndex = (UInt8)name.find(space.c_str(), spaceLastIndex);
		if (spaceLastIndex != 255)
		{
			if (spaceLastIndex > spaceHeadIndex)
			{
				checkingWord.append(name.substr(spaceHeadIndex, (spaceLastIndex - spaceHeadIndex) ).c_str()	);
			}
			++spaceLastIndex;
			spaceHeadIndex = spaceLastIndex;					
		}
		else
		{
			checkingWord.append(name.substr(spaceHeadIndex, (name.length() - spaceHeadIndex) ).c_str()	);
		}

	}while(spaceLastIndex != 255);

	checkingWord = checkingWord.ToLower();

	for (UInt16 j = 0; j < blockWord->size(); ++j)
	{
		int tempPos = (int)checkingWord.find(blockWord->at(j).c_str());
		if ( tempPos >= 0)
		{
			cantUse = true;	
			break;
		}
	}
	return (!cantUse);
}

VOID Global::ModifyMessage(String &message)
{
	BlockWordDataList* blockList = gGlobal.m_resource->GetMsgBlockWordList();

	if (blockList == NULL)
		return;

	String checkingWord;

	int spaceHeadIndex = 0;
	int spaceLastIndex = 0;
	String space = _T(" ");
	UInt16Vector wordPos;
	message = message.Trim();

	size_t cmdPos = gConfigure.strMsgDel.length();
	if (message.find(gConfigure.strCheatCom, cmdPos) == cmdPos)
		return;

	//remove all the space first
	do 
	{
		spaceLastIndex = (int)message.find(space.c_str(), spaceLastIndex);
		if (spaceLastIndex >= 0)
		{
			if (spaceLastIndex > spaceHeadIndex)
			{
				UInt16 len = spaceLastIndex - spaceHeadIndex;
				checkingWord.append(message.substr(spaceHeadIndex, len ).c_str()	);
				for (UInt16 k = spaceHeadIndex; k < (spaceHeadIndex + len); ++k )
				{
					wordPos.push_back(k);
				}

			}
			++spaceLastIndex;
			spaceHeadIndex = spaceLastIndex;					
		}
		else
		{
			UInt16 len = (UInt16)message.length() - spaceHeadIndex;
			checkingWord.append(message.substr(spaceHeadIndex, (message.length() - spaceHeadIndex) ).c_str()	);
			for (UInt16 k = spaceHeadIndex; k < (spaceHeadIndex + len); ++k )
			{
				wordPos.push_back(k);
			}
		}

	}while(spaceLastIndex >= 0);

	spaceHeadIndex = 0;
	UInt8Vector link1Pos, link2Pos;
	UInt8Vector checkOpenClose;
	for (Index i = C_INDEX(0); i < checkingWord.size(); ++i)
	{
		if (checkingWord[i] == 15)
			checkOpenClose.push_back(i);
		else if (checkingWord[i] == 16 && checkOpenClose.size() > 0)
		{
			link1Pos.push_back(checkOpenClose.back());
			link2Pos.push_back(i);
			checkOpenClose.pop_back();
		}
	}
	checkingWord = checkingWord.ToLower();

	for (UInt32 i = 0; i < blockList->size(); ++i)
	{
		spaceHeadIndex = 0;
		//spaceLastIndex = 0;
		do
		{
			spaceHeadIndex = (int)checkingWord.find(blockList->at(i).c_str(), spaceHeadIndex);

			if (spaceHeadIndex >= 0)
			{
				bool ignor = false;
				UInt pos = 0;
				if (link1Pos.size() > 0 && link1Pos.size() == link2Pos.size())
				{
					for (Index a = C_INDEX(0); a < link1Pos.size(); ++a)
					{
						if (link1Pos[a] < spaceHeadIndex && link2Pos[a] > spaceHeadIndex)
						{
							ignor = true;
							pos = a;
							break;
						}
					}
				}

				if (!ignor)
				{
					for (UInt16 k = spaceHeadIndex; k < spaceHeadIndex + blockList->at(i).length(); ++k )
						message.replace(wordPos[k], 1, space.c_str());
					
					spaceHeadIndex += (int)blockList->at(i).length();	
				}
				else
					spaceHeadIndex += link2Pos[pos];
			}
		}while (spaceHeadIndex >= 0);	
	}
}

bool Global::IsSuitableName(String name)
{

	name = name.Trim();

	if (name.length() == 0)
		return false;

	String checkingWord;
	UInt8 spaceIndex = 0;
	String space = _T(" ");

	bool cantUse = false;//cannot use
	BlockWordDataList* blockWord = gGlobal.m_resource->GetNameBlockWordList();

	if (blockWord == NULL)
		return cantUse;

	spaceIndex = (UInt8)name.find(space.c_str(), spaceIndex);
	if (spaceIndex != 255)
		return cantUse;

	checkingWord = name;
	checkingWord = checkingWord.ToLower();

	for (UInt16 j = 0; j < blockWord->size(); ++j)
	{
		int tempPos = (int)checkingWord.find(blockWord->at(j).c_str());
		if ( tempPos >= 0)
		{
			cantUse = true;	
			break;
		}
	}
	return !cantUse;
}

Color32 Global::GetDisplayMoneyColor(UInt32 money)
{
	Color32 textColor = 0;

	if ( 0 <= money && money < 10000)
		textColor = TextColor[ColorIndex_White];
	else if ( 10000 <= money && money < 100000)
		textColor = TextColor[ColorIndex_Green];
	else if ( 100000 <= money && money < 1000000)
		textColor = TextColor[ColorIndex_Blue];
	else if ( 1000000 <= money && money < 10000000)
		textColor = TextColor[ColorIndex_Violet];
	else
		textColor = ItemTextColor[ItemColorIndex_Golden];

	return textColor;
}

bool IsHawking()
{
	if ( (gGlobal.g_pMyHawkGroup && gGlobal.g_pCharacter->GetCharNetObject()->GetAction() == CCharacter::ST_HAWK))
		return true;
	return false;
}

bool IsPetUse()
{
	if ( gGlobal.g_pCharacter->GetCharNetObject()->GetAction() == CCharacter::ST_SIT)
		return true;
	return false;
}

///////////////////////////////////////////////
//  Battle function for LUA


void BattleAction()
{
}


void BattleSkill()
{
	VMProtectBegin; VM_START;

	//State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUISkillState));
	//	gGlobal.g_CursorState->Block();
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenSkillPanel", String(), 0, 0);

	VMProtectEnd; VM_END;
}

void BattleSpecial()
{
	VMProtectBegin; VM_START;

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenSkillPanel", String(), 0, 0);

	VMProtectEnd; VM_END;
}

void BattleItem()
{
	VMProtectBegin; VM_START;

	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIUseItemState));
	gGlobal.g_CursorState->Block();

	VMProtectEnd; VM_END;
}


void BattleDefence()
{
	VMProtectBegin; VM_START;

	if (gGlobal.GetBattleController()  && !gGlobal.GetBattleController()->IsSentCommand()) 
	{
		if (gGlobal.GetBattleController() && !gGlobal.GetBattleController()->m_ActionCmdAva[BATCMD_DEFEND])
			return;
		gGlobal.GetBattleController()->SendBattleCommand(BATCMD_DEFEND, 0, 0, 0);
	}
	else if (gGlobal.g_pBattleControl->GetControlPartner() && gGlobal.g_pBattleControl->GetControlPartner()->GetBattleHp() > 0 && !gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand())
	{
		if (gGlobal.g_pBattleControl->GetControlPartner() && !gGlobal.g_pBattleControl->GetControlPartner()->m_ActionCmdAva[BATCMD_DEFEND])
			return;
		gGlobal.g_pBattleControl->GetControlPartner()->SendBattleCommand(BATCMD_DEFEND, 0, 0, 0);
	}

	VMProtectEnd; VM_END;
}


void BattleFriend()
{
}


void BattleProtect()
{
	VMProtectBegin; VM_START;

	if ( (gGlobal.GetBattleController()  ) || 
		(gGlobal.g_pBattleControl->GetControlPartner() && gGlobal.g_pBattleControl->GetControlPartner()->GetBattleHp() > 0) )
	{
		if (!IsBattlePartnerTurn())
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePanel", String());
		else
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattlePartnerPanel", String());
		CCharacterControl* pChar =  dynamic_cast<CCharacterControl *>(QGLApplication::GetInstance().GetSelectedControl());
		gGlobal.g_CursorState->Unblock();
		if (pChar)
		{
			State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIProtectState));
			if (pChar->GetBatActorObject()->GetTeam() != gGlobal.g_pBattleControl->GetControlPlayerTeam() ||
				(pChar == gGlobal.g_pBattleControl->GetControlPlayer() && !gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand()) ||
				(pChar == gGlobal.g_pBattleControl->GetControlPartner() && gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand()) )
			{
				CMapUIBaseState * Cursor = dynamic_cast<CMapUIBaseState *>(gGlobal.g_CursorState);
				Cursor->ErrorState();									
			}
		}
		else
			State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIProtectState));


		gGlobal.g_CursorState->Block();
	}

	VMProtectEnd; VM_END;
}


void BattleCatch()
{
	VMProtectBegin; VM_START;

	{
		if (gGlobal.g_pCharacter->GetCharNetObject()->GetMaxPartner() > gGlobal.g_pPartnerGroup->GetCount() )
		{
			gGlobal.g_CursorState->Unblock();
			State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUICatchState));
			CCharacterControl* pChar =  dynamic_cast<CCharacterControl *>(QGLApplication::GetInstance().GetSelectedControl());
			if (pChar && pChar->GetBatActorObject()->GetTeam() == gGlobal.g_pBattleControl->GetControlPlayerTeam() )
			{
				CMapUIBaseState * Cursor = dynamic_cast<CMapUIBaseState *>(gGlobal.g_CursorState);					
				Cursor->ErrorState();		
			}
			gGlobal.g_CursorState->Block();
		}
		else
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARNTER_FULL")), CHANNELTYPE_SYSTEM, String());
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePanel", String());
		}
	}

	VMProtectEnd; VM_END;
}


void BattleHelp()
{
	if (gGlobal.GetBattleController())
	{
		if (gGlobal.GetBattleController() && !gGlobal.GetBattleController()->m_ActionCmdAva[BATCMD_HELP])
			return;
		gGlobal.GetBattleController()->SendBattleCommand(BATCMD_HELP, 0, 0, 0);
	}
}

void BattleJoin()
{
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
		gGlobal.g_pCharacter->GetCharNetObject()->RequestJoinBattle(gGlobal.pPeerProxy, gGlobal.g_context, PK_BATTLE);
}

void BattleWatchLeave()
{
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
		gGlobal.g_pCharacter->GetCharNetObject()->LeaveWatching(gGlobal.pPeerProxy, gGlobal.g_context);
}

void CancelAuto()
{
	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
	{
		/*if (gGlobal.GetBattleController() && !gGlobal.GetBattleController()->m_ActionCmdAva[BATCMD_AUTO])
		return;

		if (gGlobal.g_pBattleControl->GetControlPartner())
		gGlobal.g_pBattleControl->GetControlPartner()->SetIsSentCommand(true);

		if (gGlobal.GetBattleController())
		gGlobal.GetBattleController()->SendBattleCommand(BATCMD_AUTO, 0, 0, 0);
		*/
		if (gGlobal.g_pBattleControl->GetControlPlayer() && gGlobal.g_pBattleControl->GetControlPlayer()->GetBatActorObject())
			gGlobal.g_pBattleControl->GetControlPlayer()->GetBatActorObject()->CancelAuto(gGlobal.pPeerProxy, gGlobal.g_context);
	}
	else
	{
		if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			gGlobal.g_pCharacter->GetCharNetObject()->CancelAuto(gGlobal.pPeerProxy, gGlobal.g_context);
	}
}
void BattleAuto()
{
	VMProtectBegin; VM_START;

	if (gConfigure.uNetwork)
	{
		if (gGlobal.GetBattleController() && !gGlobal.GetBattleController()->m_ActionCmdAva[BATCMD_AUTO])
			return;

		if (gGlobal.g_pBattleControl->GetControlPartner())
			gGlobal.g_pBattleControl->GetControlPartner()->SetIsSentCommand(true);

		if (gGlobal.GetBattleController())
			gGlobal.GetBattleController()->SendBattleCommand(BATCMD_AUTO, 0, 0, 0);

		//	if (gGlobal.g_pBattleControl->GetControlPartner())
		//		gGlobal.g_pBattleControl->GetControlPartner()->SendBattleCommand(BATCMD_AUTO, 0, 0);
		//gGlobal.g_pCharacter->SendBattleCommand(BATCMD_AUTO, 0, 0);
	}
	else
	{
		gGlobal.g_pBattleControl->ShowAutoCountBox(25);
	}

	VMProtectEnd; VM_END;
}


void BattleEscape()
{
	VMProtectBegin; VM_START;

	if (gGlobal.GetBattleController() )
	{
		if (gGlobal.GetBattleController() && !gGlobal.GetBattleController()->m_ActionCmdAva[BATCMD_AUTO])
			return;
		gGlobal.GetBattleController()->SendBattleCommand(BATCMD_ESCAPE, 0, 0, 0);
		//gGlobal.g_pBattleControl->PerformPreEscape(gGlobal.GetBattleController());

		if (!gConfigure.uNetwork)
		{
			gGlobal.g_pBattleControl->PerformEscapeFail(gGlobal.GetBattleController());
		}

	}

	VMProtectEnd; VM_END;
}


//////////////////////////////////////////////////////////////////////////
//		Buttom Bar
//////////////////////////////////////////////////////////////////////////


void BBAttack()  
{
	if (gGlobal.g_pParty && !gGlobal.g_pParty->IsLeader())
	{
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_NOT_LEADER_CANT_PK")), CHANNELTYPE_SYSTEM);
		return;
	}

	if (gGlobal.g_pMyHawkGroup)
	{
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_PK")), CHANNELTYPE_SYSTEM);
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_PK")));
		return;
	}

	if (State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapAttackState)))
		gGlobal.g_CursorState->Block();
} 

void BBTrade()  
{
	if (gGlobal.g_pGivenProcess && gGlobal.g_pGivenProcess->GetTargetCharID() == 0 )
	{
		if (gGlobal.g_pTradingProcess && !gGlobal.g_pTradingProcess->hasTarget())
		{
			if (State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapTradingState)))
				gGlobal.g_CursorState->Block();
		}
		else
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_PLEASE_WAIT")), CHANNELTYPE_SYSTEM);
		}
	}
	else
	{
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_SELF_GIVEN_FAIL")), CHANNELTYPE_SYSTEM);
	}
} 

void BBGive()  
{
	if (gGlobal.g_pTradingProcess && !gGlobal.g_pTradingProcess->hasTarget())
	{
		if (gGlobal.g_pGivenProcess && gGlobal.g_pGivenProcess->GetTargetCharID() == 0 )
		{
			if (State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapGivenState)))
				gGlobal.g_CursorState->Block();
		}
	}
	else
	{
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_SELF_TRADING_FAIL")), CHANNELTYPE_SYSTEM);
	}
} 

void BBParty()  
{
	if (State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUIPartyState)))
		gGlobal.g_CursorState->Block();
} 

void BBChar()  
{
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
} 

void BBFriend()
{
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
} 

void BBItem()
{
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
}

void BBSkill()  
{
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
} 

void BBHousen()  
{
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
} 

void BBMission()  
{
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
} 

void BBFriendShip()  
{
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
} 

void BBEmotion()  
{
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
} 

void BBSystem()  
{
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
} 

void BBGeneration()
{
	if (State::ChangeState(gGlobal.g_CursorState,SafeCreateObject(CMapUIGenerationState)))
		gGlobal.g_CursorState->Block();
}
void PartyKick(int memPos)
{
	if( memPos >= MAX_PARTY_NUM || memPos < 0 )
		return;

	if( gGlobal.g_pParty==NULL )
		return;

	UInt size = gGlobal.g_pParty->GetCount();
	NetObject * netobj;
	CCharacter * pchar;
	int memberpos = 0;
	for( Index i = C_INDEX(0); i < size; ++i )
	{
		netobj = gGlobal.g_pParty->GetChild(i);
		if( netobj->GetClassID()==Character_CLASSID )
		{
			pchar = CastNetObject<CCharacter>(netobj);
			if( pchar && pchar->GetChar_id()!=gGlobal.g_pCharacter->GetMapActorID() )
			{
				if( memPos==memberpos )
				{
					pchar = CastNetObject<CCharacter>(netobj);
					if( pchar )
					{
						gGlobal.g_pParty->Kick(gGlobal.pPeerProxy, gGlobal.g_context, pchar->GetNetId());
						return;
					}
				}
				memberpos++;
			}
		}
	}
}

void PartyPromote(int memPos)
{
	if( memPos >= MAX_PARTY_NUM || memPos < 0 )
		return;

	if( gGlobal.g_pParty==NULL )
		return;

	UInt size = gGlobal.g_pParty->GetCount();
	NetObject * netobj;
	CCharacter * pchar;
	int memberpos = 0;
	for( Index i = C_INDEX(0); i < size; ++i )
	{
		netobj = gGlobal.g_pParty->GetChild(i);
		if( netobj->GetClassID()==Character_CLASSID )
		{
			pchar = CastNetObject<CCharacter>(netobj);
			if( pchar && pchar->GetChar_id()!=gGlobal.g_pCharacter->GetMapActorID() )
			{
				if( memPos==memberpos )
				{
					pchar = CastNetObject<CCharacter>(netobj);
					if( pchar )
					{
						gGlobal.g_pParty->Promote(gGlobal.pPeerProxy, gGlobal.g_context, pchar->GetNetId());
						return;
					}
				}
				memberpos++;
			}
		}
	}
}

void PartyQuit()
{
	if( gGlobal.g_pParty == NULL && gGlobal.g_pTempLeaveParty == NULL)
		return;

	gGlobal.g_pPartyProcess->ClearAllRequest();
	gGlobal.g_pPartyProcess->ClearAllInvite();
	if (NULL != gGlobal.g_pParty) 
		gGlobal.g_pParty->Leave(gGlobal.pPeerProxy, gGlobal.g_context);
	else
		gGlobal.g_pTempLeaveParty->Leave(gGlobal.pPeerProxy, gGlobal.g_context);
}


void PartyPrivateCall(int memPos)
{
	if( memPos >= MAX_PARTY_NUM-1 || memPos < 0 )
		return;

	if( gGlobal.g_pParty==NULL && gGlobal.g_pTempLeaveParty == NULL)
		return;
	
	CParty* pParty = gGlobal.g_pParty==NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	UInt size = pParty->GetCount();
	NetObject * netobj;
	CCharacter * pchar;
	String targetName;
	int memberpos = 0;
	for( Index i = C_INDEX(0); i < size; ++i )
	{
		netobj = pParty->GetChild(i);
		if( netobj->GetClassID()==Character_CLASSID )
		{
			pchar = CastNetObject<CCharacter>(netobj);
			if( pchar && pchar->GetChar_id()!=gGlobal.g_pCharacter->GetMapActorID() )
			{
				if( memPos==memberpos )
				{
					if( pchar )
					{
						targetName = pchar->GetNickName();
						i = size;
					}
				}
				memberpos++;
			}
		}
	}

	if( !targetName.IsEmpty() )
	{
		UIEditBox * pEditBox = gGlobal.pUIManager->GetUIThing<UIEditBox *>("EditLine");
		if (pEditBox)
		{
			String Temp;
			Temp.Format( _T("/p %s "), targetName.c_str() );
			pEditBox->SetText(Temp);
			pEditBox->SetFocus(true);
			pEditBox->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
		}
	}
}

void UpdateCharMenu()
{
	VMProtectBegin; VM_START;

	// Overall update
	/* update the following field:
	CharNickName,		CharTitle,		CharClass,		CharGoodness,		CharFaction,		CharFactionDonate,
	CharHonor,		CharLevel,		CharHP,		CharVP,		CharSP,		CharWc,		CharDP,		CharATK,
	CharSTR,		CharDEF,		CharCON,		CharHIT,		CharSTA,		CharEVA,		CharAGI,
	CharPWD,		CharSPR,		CharSPD,		Charpotential,		CharExp,
	*/
	if( gConfigure.uNetwork )
	{
		gGlobal.g_pCharProcess->UpdateCharMenu();
	}

	VMProtectEnd; VM_END;
}



void UpdateBattleItemMenu(bool wine)
{
	gGlobal.UpdateBattleItemMenu(false, 0, wine);
}

MobDB_Client* Global::LoadMobTemplate(UInt32 mobid)
{
	std::map<UInt32, MobDB_Client*>::iterator itr = m_mobdb.find(mobid);
	if( itr!=m_mobdb.end() )
		return itr->second;
	else
	{
		// load it from file and store it in a map
		TCHAR* tempfolderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
		ZeroMemory(tempfolderpath, FILENAME_LENGTH*sizeof(TCHAR));
		TCHAR* folderpath = SafeAllocate(TCHAR, FILENAME_LENGTH);
		ZeroMemory(folderpath, FILENAME_LENGTH*sizeof(TCHAR));
		GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportClientZoneDataDirectory"), _T("\0"), tempfolderpath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
		tempfolderpath[FILENAME_LENGTH-1] = 0;

		_stprintf(folderpath, _T("%sclient_MOB%08i_mob.dat"), tempfolderpath, mobid);
		folderpath[FILENAME_LENGTH-1] = 0;

		MobDB_Client* tmpdb = NULL;
		if( g_GdfResource || g_GdfResourceEx )
		{
			String patchedFile(folderpath);
			UInt32 beginOffset, fileSize;
			BYTE* fbuf = NULL;
			if ( g_GdfResourceEx )
				fbuf = g_GdfResourceEx->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
			if ( (!fbuf) && g_GdfResource )
				fbuf = g_GdfResource->GetMapViewOfFile(patchedFile, beginOffset, fileSize);
			if( fbuf )
			{
				BYTE* buf = fbuf + beginOffset;
				DefaultInfoHeader mobdb_header;
				memcpy(&mobdb_header, buf, sizeof(DefaultInfoHeader));
				buf += sizeof(DefaultInfoHeader);

				TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, mobdb_header.version!=EXPORT_DATA_CLIENT_MOB_TEMPLATE_FORMAT_VERSION, _T("LoadMobTemplate from \"%s\", file version:%i, app. version:%i"), String(patchedFile).c_str(), mobdb_header.version, EXPORT_DATA_CLIENT_MOB_TEMPLATE_FORMAT_VERSION);
				tmpdb = SafeCreateObject(MobDB_Client);
				tmpdb->m_basicinfo = SafeCreateObject(BinaryClientMOBInfo);
				memcpy(tmpdb->m_basicinfo, buf, sizeof(BinaryClientMOBInfo));
				m_mobdb.insert( std::make_pair(tmpdb->m_basicinfo->mob_id, tmpdb) );
				UnmapViewOfFile(fbuf);
			}
			SafeDeallocate(folderpath);
			SafeDeallocate(tempfolderpath);
			return tmpdb;
		}else
		{
			FILE *f0;
			if( folderpath[0]!=0 && (f0 = _tfopen(folderpath, _T("rb"))) )
			{
				DefaultInfoHeader m_mobdb_header;
				fread(&m_mobdb_header, 1, sizeof(DefaultInfoHeader), f0);

				TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, m_mobdb_header.version!=EXPORT_DATA_CLIENT_MOB_TEMPLATE_FORMAT_VERSION, _T("LoadMobTemplate from \"%s\", file version:%i, app. version:%i"), String(folderpath).c_str(), m_mobdb_header.version, EXPORT_DATA_CLIENT_MOB_TEMPLATE_FORMAT_VERSION);

				tmpdb = SafeCreateObject(MobDB_Client);
				tmpdb->m_basicinfo = SafeCreateObject(BinaryClientMOBInfo);
				fread( tmpdb->m_basicinfo, 1, sizeof(BinaryClientMOBInfo), f0 );
				m_mobdb.insert( std::make_pair(tmpdb->m_basicinfo->mob_id, tmpdb) );
				fclose(f0);
			}
			SafeDeallocate(folderpath);
			SafeDeallocate(tempfolderpath);
			return tmpdb;
		}
	}
}

void SetCharacterIndex(Int32 Index)
{
	gGlobal.g_pLoginProcess->SetCharacterIndex( Index);
}

bool HasCharacter(Int32 Index)
{
	if (gGlobal.g_pAccountControl->m_characters->GetChild(Index))
		return true;
	return false;
}

void SetLineIndex(Int32 Index)
{
	gGlobal.g_pLoginProcess->SetLineIndex( Index);
}

void ChangeChannel(int i)
{
	if (i == CHANNELTYPE_RUMOR)
	{
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_CANNOT_CHANGE_TO_RUMOR_CHANNEL")), CHANNELTYPE_SYSTEM);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PopChannelPanel", String(), 0, 0);
		return;
	}

	CChannelControl * pChannelCtrl = gGlobal.GetChannelControl((BYTE)i);
	if (pChannelCtrl)
	{
		gGlobal.SetCurrChannelType((BYTE)i);
	}
	else if (i == CHANNELTYPE_MAP)
		gGlobal.SetCurrChannelType(CHANNELTYPE_MAP);
	else if (i == CHANNELTYPE_FRIEND)
		gGlobal.SetCurrChannelType(CHANNELTYPE_FRIEND);
	else
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_NOTFOUND")), CHANNELTYPE_SYSTEM, String());

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PopChannelPanel", String(), 0, 0);
}
void ReceiveChannel(int i)
{
	if (i == CHANNELTYPE_PRIVATE)
	{
		QGLApplication::GetInstance().SetListenToPrivateMessage(TRUE);
	}
	else if (i == CHANNELTYPE_FRIEND)
	{
		QGLApplication::GetInstance().SetListenToFriendMessage(TRUE);
	}
	else
	{
		CChannelControl * pChannelCtrl = gGlobal.GetChannelControl((BYTE)i);
		if (pChannelCtrl)
		{
			pChannelCtrl->SetListenToChannel(TRUE);

		}
		else if (i == CHANNELTYPE_MAP)
		{
			if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
				gGlobal.g_pCharacter->GetCharNetObject()->SetListen(gGlobal.pPeerProxy, gGlobal.g_context, TRUE);
		}
	}

	gGlobal.g_ClientistenToChannel[i] = true;
	gGlobal.UpdateChannelButtonState(i);
}
void RejectChannel(int i)
{

	CChannelControl * pChannelCtrl = gGlobal.GetChannelControl((BYTE)i);
	if (i == CHANNELTYPE_PRIVATE)
	{
		QGLApplication::GetInstance().SetListenToPrivateMessage(FALSE);
	}
	else if (i == CHANNELTYPE_SPEAKER)
	{
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_CANNOT_REJECT_CHANNELTYPE_SPEAKER")), CHANNELTYPE_SYSTEM);
		gGlobal.UpdateChannelButtonState(i);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PopChannelPanel", String(), 0, 0);
		return;
	}
	else if (i == CHANNELTYPE_FRIEND)
	{
		QGLApplication::GetInstance().SetListenToFriendMessage(FALSE);
	}
	else
	{
		if (pChannelCtrl)
		{
			pChannelCtrl->SetListenToChannel(FALSE);
		}
		else if (i == CHANNELTYPE_MAP)
		{
			if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
				gGlobal.g_pCharacter->GetCharNetObject()->SetListen(gGlobal.pPeerProxy, gGlobal.g_context, FALSE);
		}
	}

	gGlobal.g_ClientistenToChannel[i] = false;
	gGlobal.UpdateChannelButtonState(i);

}

void InitChannelEnable()
{
	for (Int i = 0; i < MAX_CHANNELTYPE; i++)
	{
		gGlobal.UpdateChannelButtonState(i);
	}
}

void Global::SetUIStaticText(StringA component, String text, BOOL addDirty)
{
	UIStaticText* pST = gGlobal.pUIManager->FindUIStaticText(component.c_str());
	if (pST)
	{
		pST->SetText(text.c_str());
		pST->Redraw();
		if( addDirty )
			AddDirtyUIItem(component.c_str());
	}
}


void Global::SetUITips(StringA component, String text)
{
	UIItem	*pUIItem = gGlobal.pUIManager->GetUIItem(component.c_str());
	if (pUIItem)
	{
		pUIItem->SetTips(text.c_str());
	}
}


void SetUIEnable(StringA component, bool enabled)
{
	UIItem* pUI = gGlobal.pUIManager->GetUIItem(component.c_str());
	if (pUI)
	{
		pUI->SetEnable(enabled);
		gGlobal.pUIManager->AddDirtyUIItem(pUI);
	}
}

void SetUIPos(StringA component, UInt posX, UInt posY)
{
	UIItem* pUI = gGlobal.pUIManager->GetUIItem(component.c_str());
	if (pUI)
		pUI->SetOffsetXY(posX, posY);
}

BOOL GetUIPos(StringA component, UInt& posX, UInt& posY)
{
	UIItem* pUI = gGlobal.pUIManager->GetUIItem(component.c_str());
	if (pUI)
	{
		posX = pUI->GetOffsetX();
		posY = pUI->GetOffsetY();
		return TRUE;
	}
	else
	{
		posX = 0;
		posY = 0;
		return FALSE;
	}
}

UILinerImage *Global::SetUILinerImage(StringA component, UInt32 denumerator, UInt32 numerator)
{
	UILinerImage* pLI = gGlobal.pUIManager->FindUILinerImage(component.c_str());
	if( pLI )
	{
		if( denumerator > 0 )
		{
			if( numerator > denumerator )
			{
				pLI->SetDenumerator(denumerator);
				pLI->SetNumerator(denumerator);
			}else
			{
				pLI->SetDenumerator(denumerator);
				pLI->SetNumerator(numerator);
			}
		}else
		{
			pLI->SetDenumerator(1);
			pLI->SetNumerator(0);
		}

		return pLI;
	}

	return NULL;
}

bool IsChannelMsg(PCSTRING msg, int Type)
{
	if (Type >= MAX_CHANNELTYPE)
		return false;
	return gConfigure.strMsgDel.find(msg[0]) == 0 &&
		gConfigure.strChannelMsgCom[Type].find(msg[1]) == 0;

}


void BattleItemUse(int itemPos)
{
	if ( itemPos == -1 ) return;

	if( gGlobal.m_pBag && gGlobal.m_resource && itemPos<129 && itemPos>=0 )
	{
		CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);
		if( pItem )
		{
			if (pItem->GetRemain() == 0)
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_ITEM_NO_REMAIN")), CHANNELTYPE_SYSTEM);
				return;
			}
			const ItemData* pItemData = gGlobal.m_resource->GetItemData(pItem->GetItem_id());
			if( pItemData )
			{
				if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_SKILL)
				{
					if (gGlobal.g_pSkillProcess->m_pTempSkillData)
					{
						if (pItemData->item_type != gGlobal.g_pSkillProcess->m_pTempSkillData->Skill_ThrowItemType)
						{
							if (gGlobal.g_pCharacter->CheckBlockItem(pItemData->item_type))
							{
								String tempText;
								if (gGlobal.g_pCharacter->m_tempBuffData)
									tempText.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_ITEM_BLOCK")), gGlobal.g_pCharacter->m_tempBuffData->name);
								else
									tempText.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_ITEM_BLOCK")), _T(""));
								gGlobal.PrintMessage(tempText, CHANNELTYPE_SYSTEM);
								return;
							}
							CGameStringTable* stringTable = gGlobal.GetStringTable();

							String Text;
							switch( gGlobal.g_pSkillProcess->m_pTempSkillData->Skill_ThrowItemType )
							{
							case 0: Text = stringTable->Get(_T("MSG_ITEMTYPE_MISC")); break;
							case 1: Text = stringTable->Get(_T("MSG_ITEMTYPE_EQUIP")); break;
							case 2: Text = stringTable->Get(_T("MSG_ITEMTYPE_FOOD")); break;
							case 3: Text = stringTable->Get(_T("MSG_ITEMTYPE_FOODMAT")); break;
							case 4: Text = stringTable->Get(_T("MSG_ITEMTYPE_DRUG")); break;
							case 5: Text = stringTable->Get(_T("MSG_ITEMTYPE_DRUGMAT")); break;
							case 6: Text = stringTable->Get(_T("MSG_ITEMTYPE_WINE")); break;
							case 7: Text = stringTable->Get(_T("MSG_ITEMTYPE_WINEMAT")); break;
							case 8: Text = stringTable->Get(_T("MSG_ITEMTYPE_EQUIPMAT")); break;
							case 9: Text = stringTable->Get(_T("MSG_ITEMTYPE_CLOTHMAT")); break;
							case 10: Text = stringTable->Get(_T("MSG_ITEMTYPE_GEM")); break;
							case 11: Text = stringTable->Get(_T("MSG_ITEMTYPE_THROW")); break;
							case 12: Text = stringTable->Get(_T("MSG_ITEMTYPE_SKILLBOOK")); break;
							case 13: Text = stringTable->Get(_T("MSG_ITEMTYPE_SCRIPT")); break;
							case 14: Text = stringTable->Get(_T("MSG_ITEMTYPE_ADDON")); break;
							case 15: Text = stringTable->Get(_T("MSG_ITEMTYPE_ARRAYBOOK")); break;
							case 16: Text = stringTable->Get(_T("MSG_ITEMTYPE_SPECIAL")); break;
							case 17: Text = stringTable->Get(_T("MSG_ITEMTYPE_BAG")); break;
							}

							String buf;
							buf.Format(stringTable->Get(_T("MSG_SKILL_ITEM_INVALID")), Text.c_str());
							gGlobal.PrintMessage(buf, CHANNELTYPE_SYSTEM);
						}
						else
						{
							if (gGlobal.g_pCharacter->CheckBlockItem(pItemData->item_type))
							{
								String tempText;
								if (gGlobal.g_pCharacter->m_tempBuffData)
									tempText.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_ITEM_BLOCK")), gGlobal.g_pCharacter->m_tempBuffData->name);
								else
									tempText.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_ITEM_BLOCK")), _T(""));
								gGlobal.PrintMessage(tempText, CHANNELTYPE_SYSTEM);
								return;
							}
							gGlobal.g_pBattleControl->SetBattleItemUseIndex(itemPos);

						}
					}

				}
				else
				{
					if (gGlobal.g_pCharacter->CheckBlockItem(pItemData->item_type))
					{
						String tempText;
						if (gGlobal.g_pCharacter->m_tempBuffData)
							tempText.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_ITEM_BLOCK")), gGlobal.g_pCharacter->m_tempBuffData->name);
						else
							tempText.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_ITEM_BLOCK")), _T(""));
						gGlobal.PrintMessage(tempText, CHANNELTYPE_SYSTEM);
						return;
					}
					switch( pItemData->item_type )
					{
					case ITEMTYPE_EQUIP:
						{
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_FAILED_BATTLEEQUIP")), CHANNELTYPE_SYSTEM);
							//gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_FAILED_BATTLEEQUIP"));
							//gGlobal.PrintMessage(_T("??`??ugâA?oA2C"), CHANNELTYPE_SYSTEM);
							if (!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand())
							{
								UIPanel * pPanel = (UIPanel *)gGlobal.pUIManager->GetUIItem("BattlePanel");
								if (pPanel)
									pPanel->SetEnable(true);
								pPanel = (UIPanel *)gGlobal.pUIManager->GetUIItem("BattleWinePanel");
								if (pPanel)
									pPanel->SetEnable(false);

							}
							else if (!gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand())
							{
								UIPanel * pPanel = (UIPanel *)gGlobal.pUIManager->GetUIItem("BattlePartnerPanel");
								if (pPanel)
									pPanel->SetEnable(true);
							}
						}
						break;
					case ITEMTYPE_FOOD:
					case ITEMTYPE_DRUG:
					case ITEMTYPE_WINE:
					case ITEMTYPE_THROW:
						{
							if ( gGlobal.GetBattleController() )
							{
								/// test code
								if ( gGlobal.g_pBattleControl->GetCommandAvail() == TRUE )
								{
									gGlobal.g_pBattleControl->SetCommandAvial(FALSE);
									gGlobal.g_pBattleControl->SetBattleItemUseIndex(itemPos);
									if (pItemData->item_type != ITEMTYPE_WINE)
										BattleItem();
									UIPanel * pPanel = (UIPanel *)gGlobal.pUIManager->GetUIItem("BattlePanel");
									if (pPanel)
									{
										pPanel->SetEnable(FALSE);
										gGlobal.pUIManager->AddDirtyUIItem(pPanel);
									}

									pPanel = (UIPanel *)gGlobal.pUIManager->GetUIItem("BattleWinePanel");
									if (pPanel)
									{
										pPanel->SetEnable(FALSE);
										gGlobal.pUIManager->AddDirtyUIItem(pPanel);
									}
								}

							}
							FSound* sfx = FSOUND_Manager::GetInstance().CreateSound(_T("Data\\Wav\\SoundE0001_use_item_1.wav"));
							if (sfx)
							{
								sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
								sfx->Play();

							}

							if (pItemData->item_type == ITEMTYPE_WINE)
							{
								gGlobal.g_pBattleControl->UseBattleItemForSelectedChar(gGlobal.g_pBattleControl->GetControlPlayer());
							}
						}
						break;
					default:
						{
							gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_FAILED_BATTLEITEMUSE")), CHANNELTYPE_SYSTEM);
							//gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_FAILED_BATTLEITEMUSE"));
							//gGlobal.PrintMessage(_T("??AO?oA2C"), CHANNELTYPE_SYSTEM);
							if (!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand())
							{
								UIPanel * pPanel = (UIPanel *)gGlobal.pUIManager->GetUIItem("BattlePanel");
								if (pPanel)
								{
									pPanel->SetEnable(true);
								}
								pPanel = (UIPanel *)gGlobal.pUIManager->GetUIItem("BattleWinePanel");
								if (pPanel)
								{
									pPanel->SetEnable(false);
								}
							}
							else if (!gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand())
							{
								UIPanel * pPanel = (UIPanel *)gGlobal.pUIManager->GetUIItem("BattlePartnerPanel");
								if (pPanel)
								{
									pPanel->SetEnable(true);
								}
							}
						}
						break;
					}// end switch-case
				}
			}
		}
	}
}


Int32 GetShortCutRefType(Int32 Index)
{
	return gGlobal.g_ShortCutSlot[Index].m_RefType;
}


Int32 GetShortCutRefNum(Int32 Index)
{
	return gGlobal.g_ShortCutSlot[Index].m_RefNum;
}


void  ClearShortCutSlot(int Index)
{
	bool netUpdate = false;
	StringA num;

	num.Format("HShortCut%d", Index);					
	UIAniIcon *pAniIcon = gGlobal.pUIManager->FindUIAniIcon(num.c_str());
	pAniIcon->ClearAniData();
	pAniIcon->SetEnable(false);
	if (gGlobal.g_ShortCutSlot[Index].m_RefType != 0 && 
		gGlobal.g_ShortCutSlot[Index].m_RefNum != 0)
	{
		gGlobal.g_ShortCutSlot[Index].m_RefNum = 0;
		gGlobal.g_ShortCutSlot[Index].m_RefType = 0;
		netUpdate = true;
	}
	num.Format("HShortText%d", Index);					
	UIStaticText *pStaticText = gGlobal.pUIManager->FindUIStaticText(num.c_str());
	pStaticText->ClearText();
	pStaticText->Redraw();
	pStaticText->SetEnable(false);
	gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);

	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar && netUpdate)
	{	
		RPCContext context;
		pchar->UpdateShortCut(gGlobal.pPeerProxy, context, 0, 0, Index);
	}

}


void  DelShortCutSlot(int Ref, int Type)
{
	for(Int i = 0; i < 10; ++i)
	{
		if(gGlobal.g_ShortCutSlot[i].m_RefNum == Ref &&
			gGlobal.g_ShortCutSlot[i].m_RefType == Type)
		{
			::ClearShortCutSlot(i);
		}
	}

}


void CopyToShortCut(int itemPos, int targetPos)
{
	//gGlobal.g_pItemProcess->CopyToShortCut(itemPos, targetPos);

	if( gGlobal.m_pBag && gGlobal.m_resource && itemPos<24 && itemPos>=0 )
	{
		StringA num;
		Int32 OldSlot = -1;
		UIShortCutNode		OldNode;

		memcpy(&OldNode, gGlobal.g_ShortCutSlot + targetPos, sizeof(UIShortCutNode));

		for(UInt32 i = 0; i < 10; ++i)
		{
			if(gGlobal.g_ShortCutSlot[i].m_RefNum == itemPos && gGlobal.g_ShortCutSlot[i].m_RefType == 1)
			{
				if(OldSlot == -1)
					OldSlot = (Int32)i;
				ClearShortCutSlot(i);
			}
		}

		gGlobal.g_pItemProcess->WriteToShortCut(itemPos, targetPos);

		if(OldSlot > -1)
		{
			if(OldNode.m_RefType == 1 && OldNode.m_RefNum >= 0 && OldNode.m_RefNum < 24)
			{
				gGlobal.g_pItemProcess->WriteToShortCut(OldNode.m_RefNum, OldSlot);
			}
			else
				if(OldNode.m_RefType == 2)
				{
					gGlobal.g_pSkillProcess->WriteToShortCut(OldNode.m_RefNum, OldSlot);
				}
		}
	}
}


void CopySkillToShortCut(int SkillID, int targetPos)
{
	//gGlobal.g_pItemProcess->CopyToShortCut(itemPos, targetPos);

	if( gGlobal.m_pBag && gGlobal.m_resource)
	{
		StringA num;
		Int32 OldSlot = -1;
		UIShortCutNode		OldNode;

		memcpy(&OldNode, gGlobal.g_ShortCutSlot + targetPos, sizeof(UIShortCutNode));

		for(UInt32 i = 0; i < 10; ++i)
		{
			if(gGlobal.g_ShortCutSlot[i].m_RefNum == SkillID && gGlobal.g_ShortCutSlot[i].m_RefType == 2)
			{
				if(OldSlot == -1)
					OldSlot = (Int32)i;
				ClearShortCutSlot(i);
			}
		}


		gGlobal.g_pSkillProcess->WriteToShortCut(SkillID, targetPos);

		if(OldSlot > -1)
		{
			if(OldNode.m_RefType == 1 && OldNode.m_RefNum >= 0 && OldNode.m_RefNum < 24)
			{
				gGlobal.g_pItemProcess->WriteToShortCut(OldNode.m_RefNum, OldSlot);
			}
			else
				if(OldNode.m_RefType == 2)
				{
					gGlobal.g_pSkillProcess->WriteToShortCut(OldNode.m_RefNum, OldSlot);
				}
		}
	}
}



void MoveEquipToBag(int itemPos, int targetPos)
{
	gGlobal.g_pItemProcess->MoveEquipToBag(itemPos, targetPos);
}

void MoveToBag(int itemPos, int bag, int target)
{
	gGlobal.g_pItemProcess->MoveToBag(itemPos, bag, target);

	if(itemPos < 24)
	{
		if(target == -1)
		{
			for(UInt32 i = 0; i < 10; ++i)
			{
				if(gGlobal.g_ShortCutSlot[i].m_RefNum == itemPos && gGlobal.g_ShortCutSlot[i].m_RefType == 1)
				{
					ClearShortCutSlot(i);
				}
			}
		}
		else
		{
			for(UInt32 i = 0; i < 10; ++i)
			{
				if(gGlobal.g_ShortCutSlot[i].m_RefType == 1)
				{
					if(gGlobal.g_ShortCutSlot[i].m_RefNum == itemPos)
					{
						gGlobal.g_ShortCutSlot[i].m_RefNum = target;
					}
					else if(gGlobal.g_ShortCutSlot[i].m_RefNum == target)
					{
						gGlobal.g_ShortCutSlot[i].m_RefNum = itemPos;
					}
				}
			}
		}
	}
}

void DropItem(int itemPos, int dropSize)
{
	gGlobal.g_pItemProcess->DropItem(itemPos, dropSize);

	if(GetMaxStack(itemPos))
	{
		for(UInt32 i = 0; i < 10; ++i)
		{
			if(gGlobal.g_ShortCutSlot[i].m_RefNum == itemPos && gGlobal.g_ShortCutSlot[i].m_RefType == 1)
			{
				ClearShortCutSlot(i);
			}
		}
	}
}


void EquipItemUse(int itemPos)
{

	gGlobal.g_pItemProcess->EquipItemUse(itemPos);
}

VOID Global::UpdateBattleItemMenu(Boolean bCheck, UInt8 nbagindex, bool wine)
{
	if( !gConfigure.uNetwork ) return;

	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if( !pchar ) return;

	UIItem * uim = gGlobal.pUIManager->GetUIItem("BattleItemMenuPanel");
	if ( !uim || uim->IsEnable() == false ) return;

	if( gGlobal.m_pBag && gGlobal.m_resource )
	{
		String Text;
		CItem* item = NULL;
		StringA componentName, imageFilename;
		//UIImage* uIm = NULL;
		UIAniIcon* pAniIcon = NULL;

		for (Index i = C_INDEX(0); i < C_INDEX(24); ++i)
		{
			componentName.Format("BIS%03iIcon", i );
			//uIm = gGlobal.pUIManager->FindUIImage(componentName.c_str());
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(componentName.c_str());
			//if( uIm )
			if (pAniIcon)
			{
				item = gGlobal.m_pBag->GetItem((UInt8)i);
				if( item && item->GetStack() && item->GetRemain() && !item->GetLock())
				{
					//
					{
						StringA sztemp;
						sztemp.Format("BIS%03iIcon", i );
						UIItem * uim2 = gGlobal.pUIManager->GetUIItem(sztemp.c_str());
						if ( item->GetStack() < 1 )
							uim2->SetEnable(false);
						else
							uim2->SetEnable(true);

					}
					//
					const ItemData* pItemData = gGlobal.m_resource->GetItemData(item->GetItem_id());
					if( pItemData && pItemData->consume != 0 && 
						(pItemData->usePlace == FIELD_ANY || pItemData->usePlace == FIELD_BATTLE))
					{
						componentName.Format("BIS%03iStack", i );
						if( item->GetStack() > 1 )
							Text.Format(_T("%i"), item->GetStack()); 
						else
							Text = "";

						SetUIStaticText(componentName, Text);
						componentName.Format("BIS%03iLvl", i );
						if( item->GetGrade() > 0 )
							Text.Format(_T("+%i"), item->GetGrade()); 
						else
							Text = "";

						SetUIStaticText(componentName, Text);

						/*StringA temp(pItemData->icon2);
						imageFilename.Format("Data/%s", temp.c_str());
						uIm->SetEnable(true);
						uIm->SetImageFileA(imageFilename.c_str());
						uIm->SetDetectRect();*/
						String temp(pItemData->icon2);
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(temp.c_str());
						pAniIcon->SetEnable(true);
					}else
					{
						componentName.Format("BIS%03iStack", i );
						Text = "";
						SetUIStaticText(componentName, Text);
						componentName.Format("BIS%03iLvl", i );
						Text = "";
						SetUIStaticText(componentName, Text);
						//uIm->SetEnable(false);
						pAniIcon->SetEnable(false);
					}
				}else
				{
					componentName.Format("BIS%03iStack", i );
					Text = "";
					SetUIStaticText(componentName, Text);
					componentName.Format("BIS%03iLvl", i );
					Text = "";
					SetUIStaticText(componentName, Text);
					//uIm->SetEnable(false);
					pAniIcon->SetEnable(false);
				}
			}
		}


		if ( bCheck )
		{
			CItem* used = gGlobal.m_pBag->GetItem(nbagindex);
			if (used) 
			{
				if ( used->GetStack() < 1 )
				{
					StringA tempPanel;
					tempPanel.Format("BIS%03iIcon", nbagindex);
					UIItem * uitem = gGlobal.pUIManager->GetUIItem(tempPanel.c_str());
					if ( uitem )
						uitem->SetEnable(false);
				}
			}
		}

		gGlobal.pUIManager->AddDirtyUIItem(uim);//RenderAll();
	}
}

VOID Global::UpdateBattleItemMenu(Int BagIndex)
{
	if( gGlobal.m_pBag && BagIndex >= 0 )
	{
		CItem* item = gGlobal.m_pBag->GetItem(BagIndex);
		if ( item )
		{
			const ItemData* pItemData = gGlobal.m_resource->GetItemData(item->GetItem_id());
			if( pItemData )
			{
				StringA szBuff;
				String szText;
				UInt8 nCount = item->GetStack();
				if ( nCount < 1 )
				{
					szBuff.Format("BIS%03iIcon", BagIndex );
					UIItem* uim = gGlobal.pUIManager->FindUIImage(szBuff.c_str());
					if ( uim )
						uim->SetEnable(false);

					szBuff.Format("BIS%03iStack", BagIndex);
					uim = gGlobal.pUIManager->FindUIStaticText(szBuff.c_str());
					if ( uim )
						uim->SetEnable(false);

					szBuff.Format("BIS%03iLvl", BagIndex);
					uim = gGlobal.pUIManager->FindUIStaticText(szBuff.c_str());
					if ( uim )
						uim->SetEnable(false);
				}
				else
				{
					if ( nCount > 1 )
						szText.Format(_T("%i"), nCount);
					else
						szText = "";

					szBuff.Format("BIS%03iStack", BagIndex);
					SetUIStaticText(szBuff, szText);

					if( item->GetGrade() > 0 )
						szText.Format(_T("+%i"), item->GetGrade()); 
					else
						szText = "";

					szBuff.Format("BIS%03iLvl", BagIndex);
					SetUIStaticText(szBuff, szText);

					szBuff.Format("BIS%03iIcon", BagIndex);
					StringA temp(pItemData->icon2);
					StringA image;
					image.Format("Data/%s", temp.c_str());
					UIImage* uim = gGlobal.pUIManager->FindUIImage(szBuff.c_str());
					if ( uim )
					{
						uim->SetEnable(true);
						uim->SetImageFileA(image.c_str());
						uim->SetDetectRect();
					}

				}
			}
		}
	}
}

void AddChatIconToPanel()
{
	UIChatPanel * pChatPanel = gGlobal.pUIManager->FindUIChatPanel("ChatIconPanel");
	if (pChatPanel)
	{
		//add the aniicon here
		String AllChatIconStr;
		for (UInt i = 1; i <= MAX_ICON; i++)
		{
			AllChatIconStr += gGlobal.GetChatIconCodebyNum(i).c_str();
		}
		pChatPanel->AddStringW(AllChatIconStr.c_str());
		pChatPanel->EndRow();
		pChatPanel->HomeRow();

		UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("ChatIconScrollBar");
		if (scrollBar)
		{
			scrollBar->SetSelNum(0);
			scrollBar->SetNumCount(pChatPanel->RowListCount());
		}

	}
}

void AddChatIconToEditLine()
{
	UIChatPanel * pChatPanel = gGlobal.pUIManager->FindUIChatPanel("ChatIconPanel");
	if (pChatPanel)
	{
		UIChatItem * pChatItem = pChatPanel->GetLinkItem();
		if (pChatItem)
		{
			UIChatLink * pLinkItem = dynamic_cast<UIChatLink *>(pChatItem);
			if (pLinkItem)
			{				
				UIItem *pItem = (UIEditBox *)gGlobal.pUIManager->GetKeyInItem();
				
				if(pItem->GetTypeID() == typeid(UIEditBox *))
				{
					UIEditBox * pBox = (UIEditBox *)gGlobal.pUIManager->GetKeyInItem();
					if (pBox)
					{
						pBox->InsertText(pLinkItem->GetLinkString());
						pBox->Redraw();
					}
				}
				else
				if(pItem->GetTypeID() == typeid(UIEditBoxEx *))
				{
					UIEditBoxEx * pBox = (UIEditBoxEx *)gGlobal.pUIManager->GetKeyInItem();
					if (pBox)
					{
						pBox->InsertText(pLinkItem->GetLinkString());
						pBox->Redraw();
					}
				}
				else
				if(pItem->GetTypeID() == typeid(UIEditBoxEx *))
				{
					UIEditBoxEx * pBox = (UIEditBoxEx *)gGlobal.pUIManager->GetKeyInItem();
					if (pBox)
					{
						pBox->InsertText(pLinkItem->GetLinkString());
						pBox->Redraw();
					}
				}
				else
				if(pItem->GetTypeID() == typeid(UIChatEditPanel *))
				{
					UIChatEditPanel * pBox = (UIChatEditPanel *)gGlobal.pUIManager->GetKeyInItem();
					if (pBox)
					{
						String	S = pLinkItem->GetLinkString();
						for(UInt i = 0; i < S.size(); ++i)
							pBox->AddCharW(S[i]);
						pBox->Redraw();
					}
				}
			}
		}
	}
}

void ChangeEditLineColor(int index)
{
	gGlobal.g_SelectTextColorIndex = index - 1;
	if (gGlobal.g_SelectTextColorIndex < 0  ||
		gGlobal.g_SelectTextColorIndex >= MaxTextColor)
		return;

	gGlobal.pUIManager->DeletePanel("ColorPanel");
	gGlobal.pUIManager->ClearStateItem();
	UIEditBox * pBox = gGlobal.pUIManager->FindUIEditBox("EditLine");
	if (pBox)
	{
		pBox->SetFontColor(TextColor[gGlobal.g_SelectTextColorIndex]);
		pBox->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pBox);
	}

	pBox = gGlobal.pUIManager->FindUIEditBox("ChatEditLine");
	if(pBox == NULL)
		pBox = gGlobal.pUIManager->FindUIEditBoxEx("ChatEditLine");
	if (pBox)
	{
		pBox->SetFontColor(TextColor[gGlobal.g_SelectTextColorIndex]);
		pBox->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pBox);
	}

}

void UpdateClockPanel(bool bInit)
{
	gGlobal.UpdateClockPanel((bInit?BOOLEAN_TRUE:BOOLEAN_FALSE));
}

void UpdateRightUpTimeInfo()
{
	gGlobal.UpdateRightUpTimeInfo();
}

VOID Global::UpdateClockPanel(Boolean bInit)
{
	UIItem * mainPanel = gGlobal.pUIManager->GetUIItem("ClockPanel");
	if ( !mainPanel || !mainPanel->IsEnable() ) return;
	if ( bInit == BOOLEAN_FALSE &&  m_PreClockCount == m_NextClockCount )return;

	m_NextClockCount %= 12;

	UInt32 dwW = 0, dwH = 0;
	UInt32 dwFH = 70;
	//dwW = mainPanel->GetX();
	//dwH = mainPanel->GetY();

	UIItem * uim = NULL;
	StringA imname("");

	// - Invisiable all image
	for ( Index i = C_INDEX(0); i < 12; ++i )
	{
		imname.Format("Time%d", i);
		uim = gGlobal.pUIManager->GetUIItem(imname.c_str());
		if ( uim )
		{
			uim->SetEnable(false);
			gGlobal.pUIManager->AddDirtyUIItem(uim);
		}
	}

	Int tempHr = m_NextClockCount;
	imname.Format("Time%d", tempHr);
	uim = gGlobal.pUIManager->GetUIItem(imname.c_str());
	if ( uim )
	{
		uim->SetEnable(true);

		switch ( tempHr )
		{
		case 0: 
			uim->SetOffsetXY(dwW + dwFH - 7, dwH + dwFH - 35); 
			break;
		case 1:
			uim->SetOffsetXY(dwW + dwFH - 8, dwH + dwFH - 31); 
			break;
		case 2:
			uim->SetOffsetXY(dwW + dwFH - 4, dwH + dwFH - 19); 
			break;
		case 3:
			uim->SetOffsetXY(dwW + dwFH - 2, dwH + dwFH - 7); 
			break;
		case 4:
			uim->SetOffsetXY(dwW + dwFH - 6, dwH + dwFH - 7); 
			break;
		case 5:
			uim->SetOffsetXY(dwW + dwFH - 6, dwH + dwFH - 6); 
			break;
		case 6:
			uim->SetOffsetXY(dwW + dwFH - 6, dwH + dwFH - 1); 
			break;
		case 7:
			uim->SetOffsetXY(dwW + dwFH - 21, dwH + dwFH - 4); 
			break;
		case 8:
			uim->SetOffsetXY(dwW + dwFH - 31, dwH + dwFH - 8); 
			break;
		case 9:
			uim->SetOffsetXY(dwW + dwFH - 36, dwH + dwFH - 6); 
			break;
		case 10:
			uim->SetOffsetXY(dwW + dwFH - 32, dwH + dwFH - 21); 
			break;
		case 11:
			uim->SetOffsetXY(dwW + dwFH - 19, dwH + dwFH - 32); 
			break;
		}
	}

	bool bDay(false);
	if ( tempHr >= 3 && tempHr <= 9 )
		bDay = true;

	UIAniIcon* pDayIcon = gGlobal.pUIManager->FindUIAniIcon("ClockUICenter_DAYICON");
	if  (pDayIcon)
		pDayIcon->SetEnable(bDay);

	UIAniIcon* pNightIcon = gGlobal.pUIManager->FindUIAniIcon("ClockUICenter_NIGHTICON");
	if (pNightIcon)
		pNightIcon->SetEnable(!bDay);


	//panel->AddUIItem(pAniIcon);
	gGlobal.pUIManager->AddDirtyUIItem(uim);

	gGlobal.pUIManager->AddDirtyUIItem(pDayIcon);
	gGlobal.pUIManager->AddDirtyUIItem(pNightIcon);
}

VOID Global::CloseQGL()
{
	//CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "DisconnectConfirm");
	PostQuitMessage(0);
}



VOID Global::UpdateRightUpTimeInfo()
{
	CGameStringTable* stringTable = gGlobal.GetStringTable();
	UIItem * uim = gGlobal.pUIManager->GetUIItem("Time");
	if ( !uim || uim->IsEnable() == false ) return;

	m_NextClockCount %= 12;

	String Text(_T(""));
	switch ( m_NextClockCount )
	{
	case 0: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_1")); break;
	case 1: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_2")); break;
	case 2: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_3")); break;
	case 3: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_4")); break;
	case 4: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_5")); break;
	case 5: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_6")); break;
	case 6: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_7")); break;
	case 7: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_8")); break;
	case 8: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_9")); break;
	case 9: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_10")); break;
	case 10: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_11")); break;
	case 11: Text = stringTable->Get(_T("MSG_INTERFACE_CLOCK_HR_12")); break;
	}


	UIImage* pDay = gGlobal.pUIManager->FindUIImage("MainUI_DAYICON");
	UIImage* pNight = gGlobal.pUIManager->FindUIImage("MainUI_NIGHTICON");
	Int tempHr = m_NextClockCount;
	bool bDay(false);
	if ( tempHr >= 3 && tempHr <= 9 )
		bDay = true;

	if ( bDay )
	{// day
		if ( pDay && pDay->IsEnable() == false )
			pDay->SetEnable(true);

		if ( pNight && pNight->IsEnable() == true )
			pNight->SetEnable(false);
	}
	else
	{// night
		if ( pDay && pDay->IsEnable() == true )
			pDay->SetEnable(false);

		if ( pNight && pNight->IsEnable() == false )
			pNight->SetEnable(true);
	}

	gGlobal.pUIManager->AddDirtyUIItem(pDay);
}

VOID Global::UpdateNightAlpha()
{
	if (m_mapdb && m_mapdb->m_basicinfo->m_mapdaynight == 0 && gGlobal.NightMask)
	{
		if (gGlobal.g_TimeMinute >= 180 && gGlobal.g_TimeMinute <= 300)
		{
			if (!gGlobal.Night)
			{
				gGlobal.Night = true;
				gGlobal.NightMask->m_Updated = TRUE;
				gGlobal.g_DrawAll = TRUE;
				QGLApplication::GetInstance().CreateNightData();
			}
			Int alpha = (UInt8)(255 * ( 1.0f - (Real32)(gGlobal.g_TimeMinute - 180) / 120));
			if (gGlobal.g_NightMaskAlpha != alpha)
			{
				gGlobal.g_NightMaskAlpha = alpha;
				gGlobal.g_DrawAll = TRUE;
			}
		}
		else if (gGlobal.g_TimeMinute >= 1140 && gGlobal.g_TimeMinute <= 1260)
		{
			if (!gGlobal.Night)
			{
				gGlobal.Night = true;
				gGlobal.NightMask->m_Updated = TRUE;
				gGlobal.g_DrawAll = TRUE;
				QGLApplication::GetInstance().CreateNightData();
			}
			Int alpha = (UInt8)(255 * ((Real32)(gGlobal.g_TimeMinute - 1140) / 120));
			if (gGlobal.g_NightMaskAlpha != alpha)
			{
				gGlobal.g_NightMaskAlpha = alpha;
				gGlobal.g_DrawAll = TRUE;
			}
		}
		else if (gGlobal.g_TimeMinute > 1260 || gGlobal.g_TimeMinute < 180)
		{
			if (!gGlobal.Night)
			{
				gGlobal.Night = true;
				gGlobal.NightMask->m_Updated = TRUE;
				gGlobal.g_DrawAll = TRUE;
				QGLApplication::GetInstance().CreateNightData();
			}
			if (gGlobal.g_NightMaskAlpha != 255)
				gGlobal.g_DrawAll = TRUE;
			gGlobal.g_NightMaskAlpha = 255;
		}
		else
		{
			if (gGlobal.Night)
			{
				gGlobal.Night = false;
				gGlobal.g_DrawAll = TRUE;
				QGLApplication::GetInstance().AddAllDirtyRect();
			}
		}
	}
}

VOID Global::UpdateClockHandle()
{
	if ( gGlobal.m_logined == LOGIN_WAITING_CHANGESTATE ) return;

	UInt32 currtime = timeGetTime();

	Int TimeDelay = 0;
#ifndef REUBEN_PUBLISH
	//TimeDelay = GetPrivateProfileInt(_T("LOADFILE"), _T("TIMEDELAY"), 0, CONFIGURATION_INI_FILE);
#endif
	/*SYSTEMTIME st;
	ZeroMemory(&st, sizeof(SYSTEMTIME));
	::GetLocalTime(&st);

	Int16 wDH = 0;
	if ( gGlobal.m_TimeStartService.wHour > st.wHour )
	wDH = 24 - (gGlobal.m_TimeStartService.wHour - st.wHour);
	else
	wDH = (st.wHour - gGlobal.m_TimeStartService.wHour);		
	//current minute
	gGlobal.g_TimeMinute = (((wDH % 2) * 60 + st.wMinute + TimeDelay) * 12) % 1440;
	*/
	UInt32 temp = ::timeGetTime();
	UInt32 interval = ( temp - m_PreGameTimeInterval );

	// realtime 00:00 = gametime 23:00			(Hour 1)
	// realtime 00:05 = gametime 24:00/00:00	(Hour 1)
	// realtime 00:10 = gametime 01:00			(Hour 2)
	// ...
	// realtime 01:50 = gametime 21:00			(Hour 12)
	// realtime 01:55 = gametime 22:00			(Hour 12)
	// realtime 02:00 = gametime 23:00			(Hour 1)
	gGlobal.g_TimeMinute = (1380 + ((m_NextClockCount * 10) + ((interval / 1000) / 60)) * 12 + TimeDelay) % 1440;

	if (g_pSystemSetting && g_pSystemSetting->IsNightModeEnable() )
		UpdateNightAlpha();

	if ( (currtime - gGlobal.m_ClockStartCount) < gConfigure.uClockInteval ) return;
	gGlobal.m_ClockStartCount = currtime;

	{
		if ( (m_RemainCalcMS + interval ) > 600000 )
		{
			m_RemainCalcMS = m_RemainCalcMS + interval - 600000;

			m_NextClockCount = (m_PreClockCount + 1) % 12;

			m_PreGameTimeInterval = temp;

			//gGlobal.g_TimeMinute = (10 + m_NextClockCount * 10) + ((interval / 1000) / 60);
			//UpdateNightAlpha();
		}
		//m_NextClockCount %= 12;	
	}
	//temp
	//m_NextClockCount += TimeDelay;

	UIItem * uim = gGlobal.pUIManager->GetUIItem("Time");
	if ( uim && uim->IsEnable())
		gGlobal.UpdateRightUpTimeInfo();

	uim = gGlobal.pUIManager->GetUIItem("ClockPanel");
	if ( uim && uim->IsEnable() )
		gGlobal.UpdateClockPanel();

	UIImage *pImage = gGlobal.pUIManager->FindUIImage("ClockTransparentButton");
	UIStaticText *pTimerText = gGlobal.pUIManager->FindUIStaticText("TimerString");

	if(pImage && pTimerText)
	{
		String Text, Format;

		Format = pTimerText->GetString();
		Text = pImage->GetTips();
		Text = Text.substr(0, 5);
		Text += Format.at(m_NextClockCount);

		pImage->SetTips(Text.c_str());
	}

	m_PreClockCount = m_NextClockCount;



}

VOID Global::TestChangeMap(UInt16 MapID)
{
	/*if (m_CurrMapView->GetFileName() ==  gConfigure.strMapFile)
	{
	ChangeMap(gConfigure.strMapFile2.c_str(), gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX,
	gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY);
	}
	else
	{
	ChangeMap(gConfigure.strMapFile.c_str(), gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX,
	gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY);
	}*/
}

VOID Global::ShowHisInput()
{
	/*
	UIEditBox * pEditbox = dynamic_cast<UIEditBox *>(pUIManager->GetKeyInItem());

	if (pEditbox == NULL)
	pEditbox = dynamic_cast<UIEditBox *>(pUIManager->GetFocusItem());

	if (pEditbox == NULL)
	return;

	if (g_pastHisIndex >= (Int)g_pastInputMsgHis.size() ||
	g_pastHisIndex == -1)
	return;

	if (pEditbox && pEditbox->GetReadPastStr())
	{
	pEditbox->ClearText();
	pEditbox->SetText(g_pastInputMsgHis[g_pastHisIndex]);

	pEditbox->End();
	pEditbox->FillImage();
	pEditbox->DrawFont();
	pUIManager->AddDirtyUIItem(pEditbox);
	}
	*/
}
VOID Global::ShowItemEffectInMap(UInt16 item_id, UInt32 srcID, UInt32 targetID)
{


	const ItemData * pItemData = m_resource->GetItemData(item_id);
	if (pItemData)
	{

		CCharacterControl * pSrcCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(srcID);
		CCharacterControl * pTargetCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(targetID);

		UInt32 CurrTime = ::timeGetTime();
		UInt hurtStart = 0;
		UInt sparkStart = 0;
		UInt32Vector tempHitLstT;
		Int rotation = SOUTHEAST;
		if (pSrcCtrl && pTargetCtrl)
			rotation = QGLMath::GetDiaRotationFromTwoPoint(pSrcCtrl->GetCharMainView()->GetCharPos().PosX,
			pSrcCtrl->GetCharMainView()->GetCharPos().PosY, pTargetCtrl->GetCharMainView()->GetCharPos().PosX,
			pTargetCtrl->GetCharMainView()->GetCharPos().PosY);

		if (pSrcCtrl)
		{
			if (pSrcCtrl->GetCharMainView()->GetAnimationIndex() != ANIM_RUN)
			{
				pSrcCtrl->GetCharMainView()->SetRotation(rotation);
				pSrcCtrl->GetCharMainView()->SetAnimationIndexByIndex(ANIM_USE, FALSE);
				UInt32 animTime = pSrcCtrl->GetCharMainView()->GetTotalTimeOfGivenAnimation(ANIM_USE, rotation);
				pSrcCtrl->GetCharMainView()->AppendAnimationSet(ANIM_IDLE1, 0, 0, 0, 0, TRUE, rotation, CurrTime + animTime, 0,
					FALSE, FALSE, 0, 0, 0, 0, 0, 0, 0);//, 0,0,
			}
			if (pItemData->effect_Atk && pItemData->effect_Atk != _T("0"))
			{
				if (pItemData->item_type == ITEMTYPE_FIREWORKS)
				{
					CAnimatedEntity ent;
					ent.LoadEntityFile(pItemData->effect_Atk, gGlobal.pD3DDevice, EQUIP_OTHERS);

					UIEffectData data;
					data.effectFileName = pItemData->effect_Atk;
					data.PixelPosX = pTargetCtrl->GetCharMainView()->GetEntPixelPos().PixelPosX;
					data.PixelPosY = pTargetCtrl->GetCharMainView()->GetEntPixelPos().PixelPosY;

					for (UInt i = 0; i < pItemData->rare; i++)
					{
						QGLApplication::GetInstance().AddUIEffectToLst(CurrTime + i * ent.GetTotalTimeOfGivenAnimation(), data);
						QGLApplication::GetInstance().AddSoundToLst(CurrTime + i * ent.GetTotalTimeOfGivenAnimation(), pItemData->sound_Atk);
					}

				}
				else
				{
					CAnimatedEntity EffectAtk;
					EffectAtk.LoadEntityFile(pItemData->effect_Atk, gGlobal.pD3DDevice, EQUIP_OTHERS);
					EffectAtk.SetOffsetType(ENTITYOFFSETTYPE_FULLSCREEN);

					tempHitLstT =  EffectAtk.FindHitTime(ANIM_IDLE1, 
						ERROR_INDEX);
					if (tempHitLstT.size() > 0)
						hurtStart = tempHitLstT[0];
					else
						hurtStart = EffectAtk.GetTotalTimeOfGivenAnimation() / 2;

					EffectData EData;
					if (pItemData->item_type == ITEMTYPE_FIREWORKS)
						EData.attach = FALSE;
					EData.effectFileName = pItemData->effect_Atk;
					EData.targetEnt = pSrcCtrl->GetCharMainView();
					EData.rotation = pSrcCtrl->GetCharMainView()->GetAnimationRotation();


					QGLApplication::GetInstance().AddEffectToLst(CurrTime, EData);
					QGLApplication::GetInstance().AddSoundToLst(CurrTime, pItemData->sound_Atk);

				}
			}
		}



		if (pTargetCtrl)
		{
			if (pItemData->effect_Hurt && pItemData->effect_Hurt != _T("0"))
			{
				if (pItemData->item_type == ITEMTYPE_FIREWORKS)
				{
					CAnimatedEntity ent;
					ent.LoadEntityFile(pItemData->effect_Hurt, gGlobal.pD3DDevice, EQUIP_OTHERS);

					UIEffectData data;
					data.effectFileName = pItemData->effect_Hurt;
					data.PixelPosX = pTargetCtrl->GetCharMainView()->GetEntPixelPos().PixelPosX;
					data.PixelPosY = pTargetCtrl->GetCharMainView()->GetEntPixelPos().PixelPosY;
					for (UInt i = 0; i < pItemData->rare; i++)
					{
						QGLApplication::GetInstance().AddUIEffectToLst(CurrTime + ent.GetTotalTimeOfGivenAnimation() * i, data);
						QGLApplication::GetInstance().AddSoundToLst(CurrTime + ent.GetTotalTimeOfGivenAnimation() * i, pItemData->sound_Hurt);
					}

				}
				else
				{
					CAnimatedEntity EffectHurt;
					EffectHurt.LoadEntityFile(pItemData->effect_Hurt, gGlobal.pD3DDevice, EQUIP_OTHERS);

					tempHitLstT =  EffectHurt.FindHitTime(ANIM_IDLE1, 
						ERROR_INDEX);
					if (tempHitLstT.size() > 0)
						sparkStart = tempHitLstT[0];
					else
						sparkStart = EffectHurt.GetTotalTimeOfGivenAnimation() / 2;

					EffectData EData;
					if (pItemData->item_type == ITEMTYPE_FIREWORKS)
						EData.attach = FALSE;
					EData.effectFileName = pItemData->effect_Hurt;
					EData.targetEnt = pTargetCtrl->GetCharMainView();
					EData.rotation = pTargetCtrl->GetCharMainView()->GetAnimationRotation();
					QGLApplication::GetInstance().AddEffectToLst(CurrTime + hurtStart, EData);
					QGLApplication::GetInstance().AddSoundToLst(CurrTime + hurtStart, pItemData->sound_Hurt);
					if(g_pSystemSetting->IsNightModeEnable() && pItemData->item_type == ITEMTYPE_FIREWORKS)
					{
						m_FireworksMode = true;
						m_FireworksModeRevertTime = CurrTime + EffectHurt.GetTotalTimeOfGivenAnimation();
						//g_pSystemSetting->SetNightMode(false);
					}
				}
			}



			if (pItemData->effect_Spark && pItemData->effect_Spark != _T("0"))
			{
				if (pItemData->item_type == ITEMTYPE_FIREWORKS)
				{
					CAnimatedEntity ent;
					ent.LoadEntityFile(pItemData->effect_Spark, gGlobal.pD3DDevice, EQUIP_OTHERS);

					UIEffectData data;
					data.effectFileName = pItemData->effect_Spark;
					data.PixelPosX = pTargetCtrl->GetCharMainView()->GetEntPixelPos().PixelPosX;
					data.PixelPosY = pTargetCtrl->GetCharMainView()->GetEntPixelPos().PixelPosY;
					for (UInt i = 0; i < pItemData->rare; i++)
					{
						QGLApplication::GetInstance().AddUIEffectToLst(CurrTime + ent.GetTotalTimeOfGivenAnimation() * i, data);
						QGLApplication::GetInstance().AddSoundToLst(CurrTime + ent.GetTotalTimeOfGivenAnimation() * i, pItemData->sound_Spark);
					}


				}
				else
				{
					EffectData EData;
					if (pItemData->item_type == ITEMTYPE_FIREWORKS)
					EData.attach = FALSE;
					EData.effectFileName = pItemData->effect_Spark;
					EData.targetEnt = pTargetCtrl->GetCharMainView();
					EData.rotation = pTargetCtrl->GetCharMainView()->GetAnimationRotation();
					QGLApplication::GetInstance().AddEffectToLst(CurrTime + hurtStart + sparkStart, EData);
					QGLApplication::GetInstance().AddSoundToLst(CurrTime + hurtStart + sparkStart, pItemData->sound_Spark);
				}
				}
			
		}
	}
}

VOID Global::ShowSkillEffectInMap(UInt16 skill_id, UInt32 srcID, UInt32 targetID)
{
	const SkillData * pSkillData = m_resource->GetSkillData(skill_id);
	if (pSkillData)
	{
		
		CCharacterControl * pSrcCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(srcID);
		CCharacterControl * pTargetCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(targetID);

		UInt32 CurrTime = ::timeGetTime();
		UInt hurtStart = 0;
		UInt sparkStart = 0;
		UInt32Vector tempHitLstT;
		Int rotation = SOUTHEAST;
		if (pSrcCtrl && pTargetCtrl)
			rotation = QGLMath::GetDiaRotationFromTwoPoint(pSrcCtrl->GetCharMainView()->GetCharPos().PosX,
			pSrcCtrl->GetCharMainView()->GetCharPos().PosY, pTargetCtrl->GetCharMainView()->GetCharPos().PosX,
			pTargetCtrl->GetCharMainView()->GetCharPos().PosY);

		if (pSrcCtrl)
		{
			if (pSkillData->form != SKILL_FORM_TYPE_MEDITATION)
			{
				pSrcCtrl->GetCharMainView()->SetRotation(rotation);
				pSrcCtrl->GetCharMainView()->SetAnimationIndexByIndex(ANIM_SKILL, FALSE);
		
				UInt32 animTime = pSrcCtrl->GetCharMainView()->GetTotalTimeOfGivenAnimation(ANIM_SKILL, rotation);
				pSrcCtrl->GetCharMainView()->AppendAnimationSet(ANIM_IDLE1, 0, 0, 0, 0, TRUE, rotation, CurrTime + animTime, 0,
					FALSE, FALSE, 0, 0, 0, 0, 0, 0, 0);//, 0,0,
			}
			if (pSkillData->effect_Atk)
			{
				CAnimatedEntity EffectAtk;
				EffectAtk.LoadEntityFile(pSkillData->effect_Atk, gGlobal.pD3DDevice, EQUIP_OTHERS);

				tempHitLstT =  EffectAtk.FindHitTime(ANIM_IDLE1, 
					ERROR_INDEX);
				if (tempHitLstT.size() > 0)
					hurtStart = tempHitLstT[0];
				else
					hurtStart = EffectAtk.GetTotalTimeOfGivenAnimation() / 2;

				EffectData EData;
				EData.effectFileName = pSkillData->effect_Atk;
				EData.targetEnt = pSrcCtrl->GetCharMainView();
				EData.rotation = pSrcCtrl->GetCharMainView()->GetAnimationRotation();
				QGLApplication::GetInstance().AddEffectToLst(CurrTime, EData);
				QGLApplication::GetInstance().AddSoundToLst(CurrTime, pSkillData->sound_Atk);
			}
		}



		if (pTargetCtrl)
		{
			if (pSkillData->effect_Hurt)
			{
				CAnimatedEntity EffectHurt;
				EffectHurt.LoadEntityFile(pSkillData->effect_Hurt, gGlobal.pD3DDevice, EQUIP_OTHERS);

				tempHitLstT =  EffectHurt.FindHitTime(ANIM_IDLE1, 
					ERROR_INDEX);
				if (tempHitLstT.size() > 0)
					sparkStart = tempHitLstT[0];
				else
					sparkStart = EffectHurt.GetTotalTimeOfGivenAnimation() / 2;

				EffectData EData;
				EData.effectFileName = pSkillData->effect_Hurt;
				EData.targetEnt = pTargetCtrl->GetCharMainView();
				EData.rotation = pTargetCtrl->GetCharMainView()->GetAnimationRotation();
				QGLApplication::GetInstance().AddEffectToLst(CurrTime + hurtStart, EData);
				QGLApplication::GetInstance().AddSoundToLst(CurrTime + hurtStart, pSkillData->sound_Hurt);
			}

			if (pSkillData->effect_Spark)
			{
				EffectData EData;
				EData.effectFileName = pSkillData->effect_Spark;
				EData.targetEnt = pTargetCtrl->GetCharMainView();
				EData.rotation = pTargetCtrl->GetCharMainView()->GetAnimationRotation();
				QGLApplication::GetInstance().AddEffectToLst(CurrTime + hurtStart + sparkStart, EData);
				QGLApplication::GetInstance().AddSoundToLst(CurrTime + hurtStart + sparkStart, pSkillData->sound_Spark);
			}
		}
	}
}

bool GetLisentToChannel(int index)
{
	if (index >= MAX_CHANNELTYPE)
		return false;
	return gGlobal.g_ClientistenToChannel[index];
}

void SendPrivateMessageTo(PCSTRINGW Name)
{
	UIEditBox * pEditBox = gGlobal.pUIManager->FindUIEditBox("EditLine");
	if (pEditBox)
	{
		String Str;
		Str.Format(_T("%s%s %s "), gConfigure.strMsgDel.c_str(), gConfigure.strChannelMsgCom[CHANNELTYPE_PRIVATE].c_str(),
			Name);
		pEditBox->SetText(Str.c_str());
		pEditBox->Redraw();
		gGlobal.SetCurrChannelType(CHANNELTYPE_PRIVATE);
		gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
	}
}

void InitRecentChatListBox()
{
	UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("PrivateMsgPopUp");
	if (pPopUpMenu)
	{
		pPopUpMenu->ClearData();
		pPopUpMenu->Redraw();

		if (gGlobal.g_RecentPriChatCharsLst.size() == 0)
			pPopUpMenu->AddString(_T(""), 0, "");
		else
		{

			for (Int i = 0; i < (Int)gGlobal.g_RecentPriChatCharsLst.size(); i++)
			{
				pPopUpMenu->AddString(gGlobal.g_RecentPriChatCharsLst[i].c_str(), i, "");
			}
		}

		pPopUpMenu->Redraw();
	}
}

void MBChangeNextPage()
{
	gGlobal.MBChangeNextPage();
}

void BatDialogNextPage()
{
	if ( gGlobal.g_pBatScriptControl )
	{
		UIItem* uim = gGlobal.pUIManager->GetUIItem("BDB_DialogueBox");
		if ( uim && uim->IsEnable() )
			gGlobal.g_pBatScriptControl->NextPageHandle();
	}
}

VOID Global::MBChangeNextPage()
{
	if ( gGlobal.g_pScriptControl )
	{
		UIItem* uim = gGlobal.pUIManager->GetUIItem("DialogueBox");
		if ( uim && uim->IsEnable() )
			gGlobal.g_pScriptControl->NextPageHandle();
	}
}

void PlayerChooseResult(int opt)
{
	VMProtectBegin; VM_START;

	if ( gGlobal.g_pScriptControl )
		gGlobal.g_pScriptControl->PlayerChooseResult(opt);

	VMProtectEnd; VM_END;
}

void PlayerChooseResultInBat(int opt)
{
	VMProtectBegin; VM_START;

	if ( gGlobal.g_pBatScriptControl )
		gGlobal.g_pBatScriptControl->OnBatPlayerChooseResult(opt);

	VMProtectEnd; VM_END;
}

bool HasParty()
{
	if (gGlobal.g_pParty == NULL )
		return false;
	return true;
}
bool HasTempParty()
{
	if (gGlobal.g_pTempLeaveParty == NULL) 
		return false;
	return true;
}

void UpdatePartyPanel()
{
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	gGlobal.g_pPartyProcess->UpdatePartyPanel(pParty);
}

void CopyImageForUIImage(PCSTRINGA src, PCSTRINGA des)
{
	UIImage* imageSrc = (UIImage*)gGlobal.pUIManager->FindUIImage(src);
	UIImage* imageDes = (UIImage*)gGlobal.pUIManager->FindUIImage(des);

	imageDes->SetImage(imageSrc->GetImage(), 0, 0, imageSrc->GetWidth(), imageSrc->GetHeight());

}

void BattleItemDrag(int itemPos)
{
	if ( itemPos == -1 ) return;

	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("BattleItemDragDrop");
	if ( pPanel )
	{

		UInt32 x = 50, y = 50;
		{
			StringA sztemp;

			sztemp.Format("BIS%03iStack", itemPos);
			UIItem * uim = gGlobal.pUIManager->GetUIItem(sztemp.c_str());
			if ( uim )
				uim->SetEnable(false);

			sztemp.Format("BIS%03iLvl", itemPos);
			uim = gGlobal.pUIManager->GetUIItem(sztemp.c_str());
			if ( uim )
				uim->SetEnable(false);

			sztemp.Format("BIS%03iIcon", itemPos);
			uim = gGlobal.pUIManager->GetUIItem(sztemp.c_str());
			if ( uim )
			{
				uim->SetEnable(false);
				x = uim->GetX();
				y = uim->GetY();
			}
		}
		//
		pPanel->SetEnable(true);
		pPanel->SetPos(x, y);
		
		UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("BIMDragDropIcon");
		if (pAniIcon)
		{
			CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);
			if ( pItem )
			{
				const ItemData* pItemData = gGlobal.m_resource->GetItemData(pItem->GetItem_id());
				if ( pItemData )
				{
					String temp(pItemData->icon2);
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(temp.c_str());
					pAniIcon->SetEnable(true);

					UIItem* pDrag = gGlobal.pUIManager->GetUIItem("BattleItemDragDrop");
					if ( pDrag )
						gGlobal.pUIManager->SetDragItemXY(pDrag, -20, -20);
				}
			}

		}
		pPanel->SetDetectRect();
	}

}

bool HasItem(int itemPos)
{
	CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);
	if (pItem)
		return true;
	else
		return false;

}

int GetMaxStack(int itemPos)
{
	CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);
	if (pItem)
		return pItem->GetStack();
	else 
		return 0;
}

void SetDropItemText(int itemPos, PCSTRINGA srcStaticText)
{
	CItem* pItem = gGlobal.m_pBag->GetItem(itemPos);
	UIStaticText* text = (UIStaticText*) gGlobal.pUIManager->FindUIStaticText(srcStaticText);

	if (pItem && text && pItem->m_data )
	{
		String str;
		str.Format(gGlobal.GetStringTable()->Get(_T("MSG_ITEM_DROP_ITEM")), pItem->m_data->name);

		text->SetText(str.c_str());
		text->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(text);//RenderAll();

	}
}

void ShowErrMessage(PCSTRINGA pText, bool center)
{
	if (center)
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(String(pText).c_str()));
	else
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(String(pText).c_str()), CHANNELTYPE_SYSTEM);
}

void BattleItemMove(int SrcBagIndex, int DesBagIndex)
{
	if ( SrcBagIndex == -1 || DesBagIndex == -1 ) return;
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("BattleItemDragDrop");
	if ( pPanel )
	{
		if ( gGlobal.m_pBag )
		{
			gGlobal.m_pBag->MoveItem(gGlobal.pPeerProxy, gGlobal.g_context, SrcBagIndex, DesBagIndex);

			CItem* swap = gGlobal.m_pBag->GetItem(DesBagIndex);
			CItem* pSrcItem = gGlobal.m_pBag->GetItem(SrcBagIndex);
			if ( pSrcItem )
			{
				gGlobal.m_pBag->SwapItem(SrcBagIndex, DesBagIndex);
				gGlobal.UpdateBattleItemMenu(SrcBagIndex);
				gGlobal.UpdateBattleItemMenu(DesBagIndex);
			}
		}
		gGlobal.pUIManager->RenderAll();
	}
}

VOID Global::SetDisplayHP(const UInt16 &hp, const UInt16 &mhp, const UInt16 &maxhp)
{
	UILinerImage  *Bar = SetUILinerImage("HPBar", maxhp, hp);
	SetUILinerImage("HPBarLim", maxhp, mhp);
	SetUILinerImage("CharHPBar", maxhp, hp);
	SetUILinerImage("CharHPBarLim", maxhp, mhp);
	AddDirtyUIItem("HPBar");
	AddDirtyUIItem("HPBarLim");
	AddDirtyUIItem("CharHPBar");
	AddDirtyUIItem("CharHPBarLim");
	String Text, Format;
	Format.Format(_T("%i/%i/%i"), hp, mhp, maxhp);

	Text = Bar->GetTips();
	Text = Text.substr(0, 3);
	Text += Format;

	SetUIStaticText("CharHP", Format, TRUE);
	SetUITips("HPBarLim", Text);
	SetUITips("HPBar", Text);
}

VOID Global::SetDisplaySP(const UInt16 &sp, const UInt16 &msp, const UInt16 &maxsp)
{
	UILinerImage  *Bar = SetUILinerImage("SPBar", msp, sp);
	//SetUILinerImage("SPBarLim", maxsp, msp);
	SetUILinerImage("CharSPBar", msp, sp);
	//SetUILinerImage("CharSPBarLim", maxsp, msp);
	AddDirtyUIItem("SPBar");
	//AddDirtyUIItem("SPBarLim");
	AddDirtyUIItem("CharSPBar");
	//AddDirtyUIItem("CharSPBarLim");
	String Text, Format;
	Format.Format(_T("%i/%i"), sp, msp);

	Text = Bar->GetTips();
	Text = Text.substr(0, 3);
	Text += Format;

	SetUIStaticText("CharSP", Format, TRUE);
	//SetUITips("SPBarLim", Text);
	SetUITips("SPBar", Text);
}

VOID Global::SetDisplayDP(const UInt16 &dp, const UInt16 &maxdp)
{
	UILinerImage  *Bar = SetUILinerImage("DPBar", maxdp, dp);
	SetUILinerImage("CharDPBar", maxdp, dp);
	AddDirtyUIItem("DPBar");
	AddDirtyUIItem("CharDPBar");
	String Text, Format;
	Format.Format(_T("%i/%i"), dp, maxdp);

	Text = Bar->GetTips();
	Text = Text.substr(0, 3);
	Text += Format;

	SetUIStaticText("CharDP", Format, TRUE);
	SetUITips("DPBar", Text);
}

VOID Global::SetDisplayWC(const UInt16 &wc, const UInt16 &maxwc, const UInt16 &WcExp, const UInt32 &NextWcExp)
{
	SetUILinerImage("CharWCBar", NextWcExp, WcExp);
	AddDirtyUIItem("CharWCBar");
	String Text;
	Text.Format(_T("%i"), wc);
	SetUIStaticText("CharWc", Text, TRUE);
}

VOID Global::SetDisplayVP(const UInt16 &vp, const UInt16 &maxvp)
{
	SetUILinerImage("CharVPBar", maxvp, vp);
	AddDirtyUIItem("CharVPBar");
	String Text;
	Text.Format(_T("%i/%i"), vp, maxvp);
	SetUIStaticText("CharVP", Text, TRUE);
}

VOID Global::AddDirtyUIItem(StringA component)
{
	UIItem* pIt = pUIManager->GetUIItem(component.c_str());
	if( pIt )
		pUIManager->AddDirtyUIItem(pIt);
}

VOID Global::NotifyNoMoreHelp()
{
	PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_REQUEST_HELP_FAIL")), CHANNELTYPE_SYSTEM, String());
	UIButton *pItem = pUIManager->FindUIButton("BattleHelpB");
	if (pItem)
	{
		pItem->SetDisableState();
		pUIManager->AddDirtyUIItem(pItem);
	}
	g_helpEnable = FALSE;
	g_NoMoreHelp = TRUE;
}

VOID Global::DisableJoinBattle()
{
	UIButton *pItem = pUIManager->FindUIButton("BattleJoinB");
	if (pItem)
	{
		pItem->SetDisableState();
		pUIManager->AddDirtyUIItem(pItem);
	}
}
bool InviteComing()
{
	return gGlobal.g_pPartyProcess->IsInviting();
}

void ShowPartyByIndex(int index)
{
	gGlobal.g_pPartyProcess->ShowPartyByIndex(index);
}

UInt GetBGMVolume()
{
	return gGlobal.GetBGMVolume();
}

UInt GetSFXVolume()
{
	return gGlobal.GetSFXVolume();
}

void InviteConfirm(bool confirm)
{
	if (confirm)
		gGlobal.g_pPartyProcess->InviteAccept();
	else
		gGlobal.g_pPartyProcess->InviteReject();
}

int GetPartyListSize()
{
	return gGlobal.g_pPartyProcess->GetPartyListSize();
}

void UpdateApplicationMenu()
{
	gGlobal.g_pPartyProcess->UpdateApplicantMenu();
}

void JoinConfirm(int index, bool confirm)
{
	if (confirm)
	{
		if (gGlobal.g_pParty->GetCount() < 5)
			gGlobal.g_pPartyProcess->JoinAccept(index);
		else
			ShowErrMessage("MSG_PARTY_FULL");
	}
	else
		gGlobal.g_pPartyProcess->JoinReject(index);
}

void BattleJoinConfirm(int index, bool confirm)
{
	if  (confirm)
		gGlobal.g_pJoinBattleProcess->AcceptJoin(index);
	else
		gGlobal.g_pJoinBattleProcess->RejectJoin(index);
}
void JoinBattleClearAllRequestUp()
{
	gGlobal.g_pJoinBattleProcess->ClearList();
}
void JoinBattleScroll(int scroll)
{
	if (scroll)
		gGlobal.g_pJoinBattleProcess->ScrollUp();
	else
		gGlobal.g_pJoinBattleProcess->ScrollDown();
}
void UpdateJoinBattleMenu()
{
	gGlobal.g_pJoinBattleProcess->UpdateBattleJoinMenu();
}
void ShowOtherParty(int next)
{
	gGlobal.g_pPartyProcess->ShowOtherParty(next);
}

void ClearAllRequest()
{
	gGlobal.g_pPartyProcess->ClearAllRequest();
}

void ApplicantPanelScroll(int scroll)
{
	gGlobal.g_pPartyProcess->ApplicantPanelScroll(scroll);
}

void Global::AddAlertBox(String msg, bool haveButton, UInt timeout, UInt startCounter)
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("AlertBoxPanel");

	if (!panel)	
	{
		XML_UI::GetInstance().AddFormFile("Data\\AlertBox.xml", gGlobal.pUIManager);
		panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("AlertBoxPanel");
		if (panel == NULL)
			return;
	}

	panel->SetEnableTimeOutEvent(false);

	UIButton* button = gGlobal.pUIManager->FindUIButton("ConfirmAlert");	
	if (button)
	{
		if (haveButton)
			button->SetEnable(true);
		else 
		{
			button->SetEnable(false);
			if (timeout > 0)
			{
				panel->SetTimeOutCount(timeout);
				panel->SetEnableTimeOutEvent(true);
			}
		}
	}

	UIStaticText* textField = gGlobal.pUIManager->FindUIStaticText("AlertText");
	if (textField)
	{
		textField->SetText(msg.c_str());
		textField->Redraw();
		if (startCounter)
			textField->SetOffsetXY(0, 25);
		else
			textField->SetOffsetXY(0, 35);
	}

	UIStaticText* countDownText = gGlobal.pUIManager->FindUIStaticText("AlertCountDownText");
	if (countDownText)
	{
		if (startCounter  > 0)
		{
			int value = startCounter / 1000;
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SetAlertCountDown", String(), value); //startCounter is ms but alertCountDown will show sec
			countDownText->SetEnableTimeOutEvent(true);
			countDownText->SetEnable(true);
		}
		else
		{
			countDownText->SetEnableTimeOutEvent(false);
			countDownText->SetEnable(false);
		}
	}
	gGlobal.pUIManager->SetMonoPanel(panel);
	gGlobal.pUIManager->AddDirtyUIItem(panel);
}

void Global::CloseAlertBox()
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("AlertBoxPanel");
	UIButton* button = gGlobal.pUIManager->FindUIButton("ConfirmAlert");
	if (panel)
		panel->SetEnable(false);
	if (button)
		button->SetEnable(false);
}

void ShowAlertOK()
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("AlertBoxPanel");
	UIButton* button = gGlobal.pUIManager->FindUIButton("ConfirmAlert");

	if (button && panel)
		button->SetEnable(true);
}

bool IsJoinRequestExist()
{
	return gGlobal.g_pPartyProcess->IsJoinRequestExist();
}

void ShowCreateCharacter(int sex, int cclass)
{
	gGlobal.g_pCreateCharProcess->SetClass(cclass);
	gGlobal.g_pCreateCharProcess->SetSex(sex);
	gGlobal.g_pCreateCharProcess->ShowCreateCharacter();
}	

void ChangeCharacterHairCol(bool next)
{
	gGlobal.g_pCreateCharProcess->ChangeCharacterHairCol(next);
}

void ChangeCharacterHead(bool next)
{
	gGlobal.g_pCreateCharProcess->ChangeCharacterHead(next);
}

void ChangeCharacterClothCol(bool next)
{
	gGlobal.g_pCreateCharProcess->ChangeCharacterClothCol(next);
}

void ChangeCharacterCloth(bool next)
{
	gGlobal.g_pCreateCharProcess->ChangeCharacterCloth(next);
}

void CreateCharacter()
{
	gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CREATE_PROCESS")), false);
	gGlobal.g_pAccountControl->CreateCharacter();
}


void LuaAddAlertBox(PCSTRINGA Msg, bool ButtonFlag)
{
	gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(String(Msg).c_str()), ButtonFlag);
}

PCSTRINGW GetTempMsg()
{
	return gGlobal.TempMsgForUI.c_str();
}

VOID Global::PrintPlayerMessage(String Msg)
{
	if (Msg.IsEmpty())
		return;

	TempMsgForUI = Msg;
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenPlayerMessage", String());
}


bool IsFullScreen()
{
	return QGLApplication::GetInstance().GetRenderClass()->IsFullScreen();
}


void SetHighLightInLogin(int index, bool highLight)
{
	gGlobal.g_pCreateCharProcess->SetAniIconHighLight(index, highLight);
}

bool IsBattle()
{
	if( QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
		return true;
	else
		return false;
}

void LoadLua(PCSTRINGA luafilename)
{
	if( g_GdfResource || g_GdfResourceEx )
	{
		String tempfilename = luafilename;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(tempfilename, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(tempfilename, beginOffset, fileSize);
		if( fbuf )
		{
			int error = 1;
			if( fileSize > 0 )
			{
				BYTE* buf = fbuf + beginOffset;
				lua_State* state = gGlobal.pUIManager->GetLuaState();
				error  = luaL_loadbuffer(state, (char*)buf, fileSize, "line") || lua_pcall(state, 0, 0, 0); // = buffered version of dofile
			}
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		luaL_dofile(gGlobal.pUIManager->GetLuaState(), luafilename);
	}
}

void Global::LoadLuaToParent(PCSTRINGA luafilename)
{
	if( g_GdfResource || g_GdfResourceEx )
	{
		String tempfilename = luafilename;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(tempfilename, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(tempfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			lua_State* state = g_pParentLua;
			int error  = luaL_loadbuffer(state, (char*)buf, fileSize, "line") || lua_pcall(state, 0, 0, 0); // = buffered version of dofile
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		int r = luaL_dofile(g_pParentLua, luafilename); // = luaL_loadfile + lua_pcall
		if (r != 0)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Error running Lua file: %s"), luafilename);
			TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(g_pParentLua, -1)).c_str());
		}
	}
}

VOID CharacterLevelUp()
{
	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_LV_UP")));
		return;
	}

	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
		gGlobal.g_pCharacter->GetCharNetObject()->LevelUp(gGlobal.pPeerProxy, gGlobal.g_context);


	if (gGlobal.pUIManager)
	{
		UIButton* button = gGlobal.pUIManager->FindUIButton("CharLevelUp");
		UIAniIcon* Aincon = gGlobal.pUIManager->FindUIAniIcon("LevelUpSpark");
		if (Aincon)
		{
			if (gGlobal.g_pCharacter->GetLevel() == 10 || gGlobal.g_pCharacter->GetExp() > gGlobal.g_pCharacter->GetNextLvExp())
			{
				if(Aincon->GetAniEnt()->HasAniFile())
					Aincon->ClearAniData();
				if(Aincon->IsEnable())
					Aincon->SetEnable(false);
			}
		}
		if (button)
		{
			button->SetDisableState();
			gGlobal.pUIManager->AddDirtyUIItem(button);
		}
	}

}

VOID CharAttOk()
{
	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_ADD_ATTRI_PT")));
		return;
	}

	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() && gGlobal.g_pMenuCharSimulator && gGlobal.g_pMenuCharSimulator->GetAttribMinus() > 0)
	{
		gGlobal.g_pCharacter->GetCharNetObject()->RaiseAttrib(gGlobal.pPeerProxy, gGlobal.g_context,
			gGlobal.g_pMenuCharSimulator->GetStaAdd(),
			gGlobal.g_pMenuCharSimulator->GetSprAdd(),
			gGlobal.g_pMenuCharSimulator->GetStrAdd(),
			gGlobal.g_pMenuCharSimulator->GetConAdd(),
			gGlobal.g_pMenuCharSimulator->GetAgiAdd()
			);
	}
}

VOID CharAttUp(Int Index, Int Count)
{
	if( gGlobal.g_pMenuCharSimulator )
	{
		gGlobal.g_pMenuCharSimulator->SetOriginalAttrib(gGlobal.g_pCharacter->GetCharNetObject()->GetAttrPoint());
		switch( Index )
		{
		case 0:
			gGlobal.g_pMenuCharSimulator->SetStaAdd(Count);
			break;
		case 1:
			gGlobal.g_pMenuCharSimulator->SetSprAdd(Count);
			break;
		case 2:
			gGlobal.g_pMenuCharSimulator->SetStrAdd(Count);
			break;
		case 3:
			gGlobal.g_pMenuCharSimulator->SetConAdd(Count);
			break;
		case 4:
			gGlobal.g_pMenuCharSimulator->SetAgiAdd(Count);
			break;
		}
		gGlobal.g_pCharProcess->UpdateCharAttr2(); //UpdateCharMenu();
		gGlobal.g_pCharProcess->UpdateAttrPoint();
	}
}

void ShowMidMap()
{
	gGlobal.g_pMiddleMap->ShowMidMap();
}

void MidMapMoveCharTo(int index)
{
	if ((gGlobal.g_pParty == NULL || gGlobal.g_pParty->IsLeader()) && !gGlobal.g_pHawkerProcess->IsOpenHawkMenu() &&
		!IsHawking() && gGlobal.g_pTargetHawkGroup == NULL && gGlobal.g_pScriptControl->IsFinished())
	{
		if (index == 0)
			gGlobal.g_pMiddleMap->MoveCharacterTo();
		else if (index == 1)
			gGlobal.g_pMiddleMap->MoveCharacterToJumpPoint();
		else if (index == 2)
			gGlobal.g_pSearchListProcess->MoveCharacterToNPC();
		else if (index == 3)
			gGlobal.g_pMiddleMap->MoveCharacterToNPC();
	}
}


void MidMapMoveOut()
{
	gGlobal.g_pMiddleMap->ResetText();
}

void ResetMidMapValue()
{
	gGlobal.g_pMiddleMap->ResetValue();
}

VOID CharAttDown(Int Index, Int Count)
{
	if( gGlobal.g_pMenuCharSimulator )
	{
		gGlobal.g_pMenuCharSimulator->SetOriginalAttrib(gGlobal.g_pCharacter->GetCharNetObject()->GetAttrPoint());
		switch( Index )
		{
		case 0:	gGlobal.g_pMenuCharSimulator->SetStaAdd(-Count);	break;
		case 1:	gGlobal.g_pMenuCharSimulator->SetSprAdd(-Count);	break;
		case 2:	gGlobal.g_pMenuCharSimulator->SetStrAdd(-Count);	break;
		case 3:	gGlobal.g_pMenuCharSimulator->SetConAdd(-Count);	break;
		case 4:	gGlobal.g_pMenuCharSimulator->SetAgiAdd(-Count);	break;
		}
		gGlobal.g_pCharProcess->UpdateCharAttr2(); //UpdateCharMenu();
		gGlobal.g_pCharProcess->UpdateAttrPoint();
	}
}

void MidMapShowName(int index)
{
	if (index == 0)
		gGlobal.g_pMiddleMap->ShowNpcName();
	else if (index ==1)
		gGlobal.g_pMiddleMap->ShowJumpPointName();
}


void MidMapClickNPC()
{
	//gGlobal.g_pMiddleMap->ClickNPC();
}

void UpdateSearchList(int index)
{
	switch (index)
	{
	case 0:	gGlobal.g_pSearchListProcess->UpdateSearchNPC();		break;
	case 1:	gGlobal.g_pSearchListProcess->UpdateSearchCharacter();	break;
	}
}

void NPCSearchListUpOrDown(int up)
{
	gGlobal.g_pSearchListProcess->ScrollUpOrDown(up);
}

void MidMapShowLocationBox()
{
	gGlobal.g_pMiddleMap->ShowLocationBox();
}

void PartySearchClickList(int index)
{
	gGlobal.g_pSearchListProcess->PartySearchClickList(index);
}

void CharSearchClickList(int index)
{
	gGlobal.g_pSearchListProcess->CharSearchClickList(index);
}

void ShowLeaderInfo()
{
	gGlobal.g_pSearchListProcess->ShowLeaderInfo();
}

void ShowLeaderInfoChar()
{
	gGlobal.g_pSearchListProcess->ShowLeaderInfoChar();
}

void ShowPartySearchList(int searchBy)
{
	gGlobal.g_pSearchListProcess->ShowPartySearchList(searchBy);
}

void ShowCharSearchList(int searchBy)
{
	gGlobal.g_pSearchListProcess->ShowCharSearchList(searchBy);
}

void OnPartySearchPagePrev()
{
	gGlobal.g_pSearchListProcess->OnPartyInfoPrevPage();
}

void OnCharSearchPagePrev()
{
	gGlobal.g_pSearchListProcess->OnCharInfoPrevPage();
}

void OnPartySearchPageNext()
{
	gGlobal.g_pSearchListProcess->OnPartyInfoNextPage();
}

void OnCharSearchPageNext()
{
	gGlobal.g_pSearchListProcess->OnCharInfoNextPage();
}

void OnJoinPartySearch()
{
	gGlobal.g_pSearchListProcess->OnJoinParty();
}

void OnJoinCharSearch()
{
	gGlobal.g_pSearchListProcess->OnJoinChar();
}

void OnShowPartyInfo()
{
	gGlobal.g_pSearchListProcess->OnShowPartyInfo();
}

void OnShowCharInfo()
{
	gGlobal.g_pSearchListProcess->OnShowCharInfo();
}

void NPCSearchClickList(int index)
{
	gGlobal.g_pSearchListProcess->NPCSearchListClick(index);
}

void SearchNPCList(int lang)
{
	gGlobal.g_pSearchListProcess->SearchNPCList(lang);
}

void WeaponsRankSearchList(int lang)
{
	gGlobal.g_pWeaponsRank->WeaponsRankSearchList(lang);
}

bool IsIndoor()
{
	if (gGlobal.m_mapdb && gGlobal.m_mapdb->m_basicinfo->m_mapinoutdoor == 1)
		return true;
	return false;
}

void ConfirmPK()
{
	if (gGlobal.g_pCharacter->GetCharNetObject())
	{
		if (gGlobal.g_pCharacter->GetAction() == CCharacter::ST_MONITOR)
			gGlobal.g_pCharacter->GetCharNetObject()->RequestJoinBattle(gGlobal.pPeerProxy, gGlobal.g_context, Boolean(TRUE));
		else
			gGlobal.g_pCharacter->GetCharNetObject()->PKRequest(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_TempPKTargetID, FORCE_PK_BATTLE);
	}
	gGlobal.g_PKRequestBox.HideMessageBox();
}

void CancelPK()
{
	gGlobal.g_PKRequestBox.HideMessageBox();
}

void ConfirmMatch()
{
	if (gGlobal.g_pCharacter->GetCharNetObject())
	{	
		gGlobal.g_pCharacter->GetCharNetObject()->PKRequest(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_TempPKTargetID, MATCH_BATTLE);
	}
	gGlobal.g_PKRequestBox.HideMessageBox();
}

void CancelMatch()
{
	if (gGlobal.g_pCharacter->GetCharNetObject())
	{
		gGlobal.g_pCharacter->GetCharNetObject()->PKRequest(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_TempPKTargetID, REJECT_MATCH_BATTLE);
	}
	gGlobal.g_PKRequestBox.HideMessageBox();
}
bool haveParty()
{
	return (gGlobal.g_pParty != NULL) || (gGlobal.g_pTempLeaveParty != NULL);
}
bool IsLeader()
{
	if (gGlobal.g_pParty)
		return (gGlobal.g_pParty->IsLeader() ? true : false);
	else if (gGlobal.g_pTempLeaveParty)
		return (gGlobal.g_pTempLeaveParty->IsLeader() ? true : false);

	return false;
}

bool GetHelpEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPlayer())
		return (gGlobal.g_helpEnable && gGlobal.g_pBattleControl->GetBattleNetObject() && 
		gGlobal.g_pBattleControl->GetBattleNetObject()->GetBattleType() == NPC_BATTLE
		&& gGlobal.g_pBattleControl->GetControlPlayer()->m_ActionCmdAva[BATCMD_HELP] && (gGlobal.g_pParty == NULL || gGlobal.g_pParty->GetCount() < MAX_PARTY_NUM));
	return true;
}

bool GetActionEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPlayer())
		return (gGlobal.g_pBattleControl->GetControlPlayer()->m_ActionCmdAva[BATCMD_SPECIAL]);
	return true;
}
bool GetSkillEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPlayer())
		return (gGlobal.g_pBattleControl->GetControlPlayer()->m_ActionCmdAva[BATCMD_SKILL]);
	return true;
}
bool GetItemEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPlayer())
		return (gGlobal.g_pBattleControl->GetControlPlayer()->m_ActionCmdAva[BATCMD_ITEM]);
	return true;
}
bool GetDefenceEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPlayer())
		return (gGlobal.g_pBattleControl->GetControlPlayer()->m_ActionCmdAva[BATCMD_DEFEND]);
	return true;
}
bool GetFriendEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPlayer())
		return (gGlobal.g_pBattleControl->GetControlPlayer()->m_ActionCmdAva[BATCMD_FRIEND]);
	return true;
}
bool GetProtectEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPlayer())
		return (gGlobal.g_pBattleControl->GetControlPlayer()->m_ActionCmdAva[BATCMD_PROTECT]);
	return true;
}
bool GetCatchEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPlayer())
		return (gGlobal.g_pBattleControl->GetControlPlayer()->m_ActionCmdAva[BATCMD_CATCH]);
	return true;
}
bool GetAutoEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPlayer())
		return (gGlobal.g_pBattleControl->GetControlPlayer()->m_ActionCmdAva[BATCMD_AUTO]);
	return true;
}
bool GetEscapeEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPlayer())
		return (gGlobal.g_pBattleControl->GetControlPlayer()->m_ActionCmdAva[BATCMD_ESCAPE]);
	return true;
}

bool GetPartnerSkillEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPartner())
		return (gGlobal.g_pBattleControl->GetControlPartner()->m_ActionCmdAva[BATCMD_SKILL]);
	return true;
}
bool GetPartnerItemEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPartner())
		return (gGlobal.g_pBattleControl->GetControlPartner()->m_ActionCmdAva[BATCMD_ITEM]);
	return true;
}
bool GetPartnerDefenceEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPartner())
		return (gGlobal.g_pBattleControl->GetControlPartner()->m_ActionCmdAva[BATCMD_DEFEND]);
	return true;
}
bool GetPartnerProtectEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPartner())
		return (gGlobal.g_pBattleControl->GetControlPartner()->m_ActionCmdAva[BATCMD_PROTECT]);
	return true;
}
bool GetPartnerEscapeEnable()
{
	if (gGlobal.g_pBattleControl->GetControlPartner())
		return (gGlobal.g_pBattleControl->GetControlPartner()->m_ActionCmdAva[BATCMD_ESCAPE]);
	return true;
}

void DeleteCharacter()
{
	UIEditBox* editBox = gGlobal.pUIManager->FindUIEditBox("DeleteCharacterEditBox");
	if (editBox)
	{
		String text(editBox->GetText());

		if (text.Equals(_T("Delete")))
		{
			if (!gGlobal.g_pAccountControl->IsSelectCharacterFrozen())
			{
				UInt16 charlevel = gGlobal.g_pAccountControl->GetSelectedCharacterLevel();
				/*if ( charlevel <= 10)
				{
					gGlobal.g_pAccountControl->DeleteCharacter();
				}
				else */
				if (charlevel < 60)
				{
					gGlobal.g_pAccountControl->FrozenCharacter();
				}
				else
				{
					gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHARACTER_IMMORTAL")));
				}
			}
			else
				gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHARACTER_FROZEN")));	
		}
		else
			gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_SYS_REMOVE_WRONG_TYPING")));
	}
}

void NPCSearchListScroll()
{
	gGlobal.g_pSearchListProcess->Scroll();
}

void UpdateShopItemDetail(int pos)
{
	if ( gGlobal.g_pShopProcess )
	{
		UInt32 itemid = gGlobal.g_pShopProcess->GetItemId((UInt8)pos);
		if ( itemid != 0 ) 
		{
			UIPanelEx* uPan = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
			if ( uPan )
				uPan->SetEnable(true);
			gGlobal.g_pItemProcess->ItemDetailPopUp(itemid, true);
		}
	}	
}

void ResetSearchValue()
{
	if (gGlobal.g_pSearchListProcess)
		gGlobal.g_pSearchListProcess->ResetValue();
}

void InitLoginScreen()
{
	if (gGlobal.g_pSystemSetting && gGlobal.g_pSystemSetting->GetUserName().length() > 0)
	{
		UIEditBox* pEb = gGlobal.pUIManager->FindUIEditBox("LoginName");
		if( pEb )
		{
			pEb->SetText(gGlobal.g_pSystemSetting->GetUserName().c_str());
			pEb->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(pEb);
		}
		UICheckBox* pCb = gGlobal.pUIManager->FindUICheckBox("SaveLogin");
		if( pCb )
		{
			pCb->SetState(true);
			gGlobal.pUIManager->AddDirtyUIItem(pCb);
		}
		gGlobal.pUIManager->ChangeFocus("Password");
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SelectPasswordField", String());
	}
}

void SaveLoginDefault()
{
	UIEditBox* editBox = gGlobal.pUIManager->FindUIEditBox("LoginName");
	UICheckBox* checkBox = gGlobal.pUIManager->FindUICheckBox("SaveLogin");
	if (gGlobal.g_pSystemSetting)
	{
		if (checkBox && checkBox->GetState() && editBox)
			gGlobal.g_pSystemSetting->SetUserName(editBox->GetText());
		else
			gGlobal.g_pSystemSetting->SetUserName(_T(""));

		gGlobal.g_pSystemSetting->WriteSystemSetting();
	}
}

UInt32 GetDefaultCharacter()
{
	return GetPrivateProfileInt(_T("LocalGameSetting"), _T("PrevCharacter"), 255, CONFIGURATION_IP_INI_FILE);
}

void SetDefaultCharacter(int pos)
{
	String charPos;
	charPos.Format(_T("%i"), pos);
	WritePrivateProfileString(_T("LocalGameSetting"), _T("PrevCharacter"), charPos.c_str(), CONFIGURATION_IP_INI_FILE);
}

UInt32 GetDefaultLine()
{
	return GetPrivateProfileInt(_T("LocalGameSetting"), _T("PrevLine"), 255, CONFIGURATION_IP_INI_FILE);
}

void SetDefaultLine(int pos)
{
	String line;
	line.Format(_T("%i"), pos);
	WritePrivateProfileString(_T("LocalGameSetting"), _T("PrevLine"), line.c_str(), CONFIGURATION_IP_INI_FILE);
}

void SetAniStateInLogin(int index, int state)
{
	gGlobal.g_pLoginProcess->SetAniState(index, state);
}

void ShowPartyButton()
{
	gGlobal.g_pPartyProcess->ShowPartyButton();
}

void ShopOpenConfirmBox(int pos, int nMode)
{
	if ( gGlobal.g_pShopProcess )
	{
		if ( nMode == 1 || nMode == 2 )
			gGlobal.g_pShopProcess->OpenConfirmBox(pos, nMode);
	}
}

void ShopEnterItemCount(bool bSell, int nPos)
{
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->OpenAdjustCountBox(bSell, nPos);
}

void ShopSetItemCount(bool bMax)
{
	if ( gGlobal.g_pShopProcess )
	{
		gGlobal.g_pShopProcess->SetMaxNum(bMax);
	}
}

void ShopAdjustItemCount(bool bInc)
{
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->IncCount(bInc);
}

void ShopConfirmAction()
{
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->ConfirmAction();
}

void ShopResetAction()
{
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->ResetParam();
}

void ShopDragItemAction(int pos)
{
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->DragItem(pos);
}

void ShopOnClickSellButton()
{
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->OnClickSellButton();
}

void ShopCloseMainMenu()
{
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->CloseShopMenu();
}

void ShopOnEditBoxKeyUp(int keyCode)
{
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->OnEditBoxKeyUp(keyCode);
}


void ShopUpdatePrice()
{
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->UpdatePrice();
}


void TestingFunction()
{
	RPCContext context;
	//gGlobal.g_pPartnerGroup->AddPartnerToServer(3);//->RemovePartnerToServer(0);//(gGlobal.pPeerProxy, context, gGlobal.g_pCharacter->GetCharNetObject()->GetNetId(), 3);
	//gGlobal.g_pPartnerGroup->GetPartnerByIndex(0)->RaiseFavor(gGlobal.pPeerProxy, context, 3, true);

}

void BattlePartnerEscape()
{
	if (gGlobal.g_pBattleControl->GetControlPartner() && gGlobal.g_pBattleControl->GetControlPartner()->GetBattleHp() > 0)
	{
		if (gGlobal.g_pBattleControl->GetControlPartner() && !gGlobal.g_pBattleControl->GetControlPartner()->m_ActionCmdAva[BATCMD_ESCAPE])
			return;
		gGlobal.g_pBattleControl->GetControlPartner()->SendBattleCommand(BATCMD_ESCAPE, 0, 0, 0);
	}
}

void FormatToSkillString(PCSTRINGA Text)
{
	UTF82StringW(gGlobal.g_TempSkillString, Text);
}

void SkillFail(PCSTRINGA Text)
{
	gGlobal.g_bSkillFail = TRUE;
	UTF82StringW(gGlobal.g_TempSkillString, Text);
}

bool IsBattlePartnerTurn()
{
	if ( gGlobal.g_pBattleControl->GetControlPartner() && gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() )
		return true;
	return false;
}

void CheckCharacterName()
{
	gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CREATE_CHECK")), false);
	gGlobal.g_pAccountControl->CheckCharacterName();
}

void SubChatScroll()
{
	UIScrollBarV* scrollBar = QGLApplication::GetInstance().GetSubWindow()->m_UIManager->FindUIScrollBarV("SubChatScrollButton");
	UIChatPanel * pSubChat = QGLApplication::GetInstance().GetSubWindow()->m_UIManager->FindUIChatPanel("SubChat");
	if (scrollBar && pSubChat)
	{
		pSubChat->SetRow( scrollBar->GetSelNum(), pSubChat->RowListCount());
		QGLApplication::GetInstance().GetSubWindow()->m_UIManager->AddDirtyUIItem(pSubChat);
	}
}

StringA	Global::GetPrefixStr(UInt16 ID, StringA pre)
{
	StringA Result;
	Result.Format("%s%04i", pre.c_str(), ID);
	return Result;
}

bool CanLevelUp()
{
	if (gGlobal.g_pCharacter->GetCharNetObject() == NULL)
		return false;
	
	if( gGlobal.g_pCharacter->GetCharNetObject()->GetExp() >= gGlobal.g_pCharacter->GetCharNetObject()->GetNextLvExp() )
		return true;
	return false;
}

void SelectItemPartner(UInt8 index)
{
	if (gGlobal.g_pItemProcess)
	{
		if (index < 6)
		{
			if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PARTNER_MENU)
				gGlobal.g_pItemProcess->UpdateItemPartner(index);
			else if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PET_MENU)
				gGlobal.g_pItemProcess->UpdateItemPet(index);
		}
	}
}

void UpdateChangeLine()
{
	if (gGlobal.g_pAccountControl)
	{
		if (!IsHawking())
			gGlobal.g_pAccountControl->UpdateMapLine();
		else
		{
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_CHANGE_LINE")));
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseChangeLine", String());
		}
	}
}

void ApplicantPrivateCall(int index)
{
	if (gGlobal.g_pPartyProcess)
		gGlobal.g_pPartyProcess->ApplicantPrivateCall(index);
}

bool IsTalking()
{
	if (gGlobal.g_pScriptControl)
		return gGlobal.g_pScriptControl->IsLockedWhenTalking();	
	return false;
}

void ChangeMapLine(UInt8 lineIndex)
{
	if (lineIndex > 0 && lineIndex <= 10 && gGlobal.g_pMap )
	{
		if (gGlobal.g_pMap->GetLine_id() == lineIndex)
		{
			String msg;
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_MAP_NO_NEED")), lineIndex);
			gGlobal.PrintPlayerMessage(msg);
		}
		else if (gGlobal.m_lineState[lineIndex] == LINE_STATE_SUPERFULL)
		{
			String msg;
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_MAP_SUPERFULL")), lineIndex);
			gGlobal.PrintPlayerMessage(msg);
		}
		else if (gGlobal.g_pParty == NULL && NULL == gGlobal.g_pTempLeaveParty) 
		{//single player change line
			gGlobal.g_pAccountControl->ChangeMapLine(lineIndex);
		}
		else if (gGlobal.g_pParty && gGlobal.g_pParty->IsLeader() && gGlobal.g_pParty->GetAvail())
		{//leader change line
			gGlobal.g_u8LineIndex = lineIndex;
			String strMsg;
			strMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_LINE_AS_LEADER")));
			gGlobal.g_FriendChatBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO, strMsg.c_str(),"FriendChat");
			UIItem * pItem = gGlobal.pUIManager->GetUIItem("FriendChatYes");
			if (pItem)
			{
				pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "ChangeLineConfirmSec");
			}
		}
		else if (NULL != gGlobal.g_pTempLeaveParty) 
		{//Temporarily leave member change line
			gGlobal.g_u8LineIndex = lineIndex;
			String strMsg;
			strMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_LINE_AS_TEMP_LEAVE")));
			gGlobal.g_FriendChatBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO, strMsg.c_str(), "FriendChat");
			UIItem * pItem = gGlobal.pUIManager->GetUIItem("FriendChatYes");
			if (pItem)
			{
				pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "ChangeLineConfirmSec");
			}
		}
	}
}

void ConfirmChangeLineSec()
{
	if (gGlobal.g_u8LineIndex > 0 && gGlobal.g_u8LineIndex <= 10 && gGlobal.g_pMap )
	{
		if (gGlobal.g_pMap->GetLine_id() == gGlobal.g_u8LineIndex)
		{
			String msg;
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_MAP_NO_NEED")), gGlobal.g_u8LineIndex);
			gGlobal.PrintPlayerMessage(msg);
		}
		else if (gGlobal.m_lineState[gGlobal.g_u8LineIndex] == LINE_STATE_SUPERFULL)
		{
			String msg;
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_MAP_SUPERFULL")), gGlobal.g_u8LineIndex);
			gGlobal.PrintPlayerMessage(msg);
		}
		else
			gGlobal.g_pAccountControl->ChangeMapLine(gGlobal.g_u8LineIndex);
		gGlobal.g_u8LineIndex = 0;
	}
}
void TeammatePrivateCall(int index)
{
	if (gGlobal.g_pPartyProcess)
		gGlobal.g_pPartyProcess->TeammatePrivateCall(index);
}

void TeammateKick(int index)
{
	if (gGlobal.g_pPartyProcess)
		gGlobal.g_pPartyProcess->TeammateKick(index);
}

void TeammatePromote(int index)
{
	if (gGlobal.g_pPartyProcess)
		gGlobal.g_pPartyProcess->TeammatePromote(index);
}

void ItemScroll(int index)
{
	if (gGlobal.g_pItemProcess)
		gGlobal.g_pItemProcess->Scroll(index);
}

void OnCloseBattleItem()
{
	if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_SKILL &&
		gGlobal.g_pBattleControl->GetBattleItemUseIndex() == -1)
	{
		gGlobal.g_CursorState->Unblock();
		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}
}

bool IsOpenSkillShop()
{
	return gGlobal.g_pSkillProcess->GetIsShop();
}

void QuestCancelCancel()
{
	gGlobal.g_QuestCancelBox.HideMessageBox();
}

void QuestCancelConfirm()
{
	if ( gGlobal.g_pQuestLogProcess )
		gGlobal.g_pQuestLogProcess->CancelQuestHandle();
}

void ShowRemoveCharacterName()
{
	if (gGlobal.g_pLoginProcess)
		gGlobal.g_pLoginProcess->ShowRemoveCharacterName();
}

void SetTempResult(double result)
{
	gGlobal.g_TempResult = result;
}

double GetTempResult()
{
	return gGlobal.g_TempResult;
}

bool IsPvP()
{
	return IsBattle() && gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->IsPKBattle();
}


void SaveQGLState()
{
	FILE *File = fopen("Data\\QGLState.xml", "w");
	if( File )
	{
		StringA	TempStr;

		TempStr = "<?xml version=\"1.0\" ?>\n";
		fwrite(TempStr.c_str(), TempStr.size(), 1, File);
		TempStr = "<!-- Our to do list data -->\n<Info>\n";
		fwrite(TempStr.c_str(), TempStr.size(), 1, File);

		TempStr.Format("    <WindowStart X=\"%d\" Y=\"%d\" />\n", gGlobal.g_StartWndX, gGlobal.g_StartWndY);
		fwrite(TempStr.c_str(), TempStr.size(), 1, File);
		TempStr.Format("    <SubChatPosition Value=\"%d\" />\n", gGlobal.g_pSystemSetting->GetSubChatPosition());
		fwrite(TempStr.c_str(), TempStr.size(), 1, File);
		TempStr.Format("    <SubWndWidth Value=\"%d\" />\n", gGlobal.g_SubWndWidth);
		fwrite(TempStr.c_str(), TempStr.size(), 1, File);

		TempStr = "</Info>\n";
		fwrite(TempStr.c_str(), TempStr.size(), 1, File);

		fclose(File);
	}
}

bool LoadQGLState()
{
	TiXmlDocument doc("Data\\QGLState.xml");

	doc.LoadFile();
	if(!doc.Error())
	{

		TiXmlHandle	docH( &doc );
		TiXmlElement	*root = doc.FirstChildElement( );

		TiXmlElement	*Element = root->FirstChildElement();
		TiXmlNode		*Node = Element;

		while (Element)
		{
			const char *NodeValue = Node->Value();

			if(NodeValue)
			{
				const char *SValue = Element->Attribute("Value");
				Int32	Value;

				Element->Attribute("Value", &Value);

				if(!strcmp(NodeValue, "WindowStart"))
				{
					Element->Attribute("X", &gGlobal.g_StartWndX);
					Element->Attribute("Y", &gGlobal.g_StartWndY);
				}
				else
					if(!strcmp(NodeValue, "SubChatPosition"))
					{
						gGlobal.g_pSystemSetting->SetSubChatPosition(Value);
					}
					else
						if(!strcmp(NodeValue, "SubWndWidth"))
						{
							gGlobal.g_SubWndWidth = Value;
						}

			}

			Element = (TiXmlElement *)Element->NextSibling();
			Node = Element;
		}
		return true;
	}
	return false;
}


bool CheckShiftDown()
{
	return ((GetKeyState(VK_LSHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0 ||
		(GetKeyState(VK_RSHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0);
}

void LuaSetBPShortCutFlag(bool Flag)
{
	gGlobal.g_BPShortCutFlag = Flag;
}

bool LuaGetBPShortCutFlag()
{
	return gGlobal.g_BPShortCutFlag;
}

UInt32 LuaGetChar_id()
{
	return (gGlobal.g_pCharacter ? gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id() : 0);
}

UInt32 LuaGetBag(UInt32 itemPos)
{
	CItem* item = gGlobal.m_pBag->GetItem(itemPos);
	
	return (item ? item->GetItem_uid() : 0);
}

PCSTRING LuaGetItemName(UInt32 itemPos)
{
	CItem* item = gGlobal.m_pBag->GetItem(itemPos);

	return PCSTRING(item->m_data->name);
}

UInt32 LuaGetItemColor(UInt32 itemPos)
{
	CItem* item = gGlobal.m_pBag->GetItem(itemPos);

	return (UInt32)(item->m_data->color);
}

UInt32 GetFirstNum(PCSTRINGW Str)
{
	UInt32  i = 0, Num = 0;
	if(Str[i] >= _T('0') && Str[i] <= _T('9'))
	{
		Num = Str[i] - _T('0');
	}
	else
		if(Str[i] >= _T('a') && Str[i] <= _T('f'))
		{
			Num = Str[i] - _T('a') + 10;
		}
		else
			if(Str[i] >= _T('A') && Str[i] <= _T('F'))
			{
				Num = Str[i] - _T('A') + 10;
			}
			else
				return 0;

	++i;
	for(; Str[i]; ++i)
	{
		if(Str[i] >= _T('0') && Str[i] <= _T('9'))
		{
			Num = Num * 16 + (Str[i] - _T('0'));
		}
		else
			if(Str[i] >= _T('a') && Str[i] <= _T('f'))
			{
				Num = Num * 16 + (Str[i] - _T('a') + 10);
			}
			else
				if(Str[i] >= _T('A') && Str[i] <= _T('F'))
				{
					Num = Num * 16 + (Str[i] - _T('A') + 10);
				}
				else
					break;
	}

	return Num;
}

UInt32 GetLastNum(PCSTRINGW Str)
{
	UInt32  Num = 0, Len;
	Int32	i;

	for(Len = 0; Str[Len]; ++Len);
	if(Len > 0)
		i = Len - 1;
	else
		return 0;

	if(Str[i] >= _T('0') && Str[i] <= _T('9'))
	{
		Num = Str[i] - _T('0');
	}
	else
		if(Str[i] >= _T('a') && Str[i] <= _T('f'))
		{
			Num = Str[i] - _T('a') + 10;
		}
		else
			if(Str[i] >= _T('A') && Str[i] <= _T('F'))
			{
				Num = Str[i] - _T('A') + 10;
			}
			else
				return 0;

	--i;
	UInt32		Opr = 16;
	for(; Str[i] && i >=0; --i)
	{
		if(Str[i] >= _T('0') && Str[i] <= _T('9'))
		{
			Num = Num + Opr * (Str[i] - _T('0'));
		}
		else
			if(Str[i] >= _T('a') && Str[i] <= _T('f'))
			{
				Num = Num + Opr * (Str[i] - _T('a') + 10);
			}
			else
				if(Str[i] >= _T('A') && Str[i] <= _T('F'))
				{
					Num = Num + Opr * (Str[i] - _T('A') + 10);
				}
				else
					break;
		Opr *= 16;
	}

	return Num;
}

void LuaSetShopSkillFlag(bool Flag)
{
	gGlobal.g_ShopSkillFlag = Flag;
}

bool LuaGetShopSkillFlag()
{
	return gGlobal.g_ShopSkillFlag != FALSE;
}

bool GetFirstNumOnNum(PCSTRINGW &Str, Int &Num)
{
	Num = 0;
	for(; *Str >= _T('0') && *Str <= _T('9'); ++Str)
	{
		Num *= 10;
		Num += *Str - _T('0');
	}

	while(*Str)
	{
		if(*Str >= _T('0') && *Str <= _T('9'))
			return true;
		++Str;
	}
	return false;
}

bool CharOpenQuestMenu(PCSTRINGW pStr)
{
	if (!gGlobal.g_QuestSystem)return false;
	if (!gGlobal.g_pQuestLogProcess)return false;

	String str = pStr;
	StlVector<UInt16> vec;
	TCHAR *token = _tcstok((wchar_t*)(str.c_str()), _T(","));
	while (token) {
		vec.push_back((UInt16) _ttoi(token));
		token = _tcstok(NULL, _T(","));
	}

	if (vec.size() != 2) return false;

	gGlobal.g_pQuestLogProcess->OnNoteToMenu(vec.front(), vec.back());

	return true;
}

bool CharSetMapPos(PCSTRINGW pStr)
{
	Int32	iNpcId=0,MapID = 0, PosX = 0, PosY = 0;
	GetFirstNumOnNum(pStr,iNpcId);
	gGlobal.m_u32DestNpcId = iNpcId;
	GetFirstNumOnNum(pStr, MapID);

	if (IsHawking())
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_MOVE")));
		return true;
	}
	
	GetFirstNumOnNum(pStr, PosX);
	GetFirstNumOnNum(pStr, PosY);
	if(gGlobal.m_mapdb->m_basicinfo->m_mapid == MapID )
	{
		gGlobal.g_pCharacter->SetEntityDesPos(PosX, PosY);
		PointList ptList;
		ptList = gGlobal.g_pCharacter->GetCharMainView()->m_PathFindData.m_ptList;

		if (ptList.size() > 10)
			gGlobal.g_pCharacter->SetEntityDesPos(ptList[ptList.size() - 10].PosX , ptList[ptList.size() - 10].PosY, true);
		else
			gGlobal.g_pCharacter->SetEntityDesPos(PosX , PosY, true);

		if (gGlobal.g_pMiddleMap->IsOpenMiddleMap())
			gGlobal.g_pMiddleMap->DrawPath(PosX, PosY);
		else
		{
			if (ptList.size() > 10)
				gGlobal.g_pMiddleMap->SetCharDes( ptList[ptList.size() - 10].PosX , ptList[ptList.size() - 10].PosY );
			else
				gGlobal.g_pMiddleMap->SetCharDes(PosX, PosY);
			gGlobal.g_pMiddleMap->SetPathEnd(PosX, PosY);
		}
		return true;
	}
	else
	{
		BlockPos bpDestPos;
		bpDestPos.PosX = PosX;
		bpDestPos.PosY = PosY;
		UInt32 uDestMap = MapID;
		BlockPos bpCurPos = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
		UInt32 uCurMap = gGlobal.m_mapdb->m_basicinfo->m_mapid;
		CCrossMapPathFinder* pcmpf=CCrossMapPathFinder::GetInstance();
		if (NULL != pcmpf) 
		{
			pcmpf->DestroyPathData();
			int iJumpPointNum = pcmpf->CreateBestPath(uCurMap,bpCurPos,uDestMap,bpDestPos);
			if (iJumpPointNum > 0)//Src and dest in different map
			{
				UInt32 uJumpPointsSeq = pcmpf->GetFirstPathPoint();
				const JUMPPOINT* pjp = gGlobal.m_resource->GetAutoPathData(uJumpPointsSeq);
				if (NULL != pjp) 
				{
					pcmpf->SetActiveState(TRUE);
					if (bpCurPos.PosX == pjp->u16PosX
						&& bpCurPos.PosY == pjp->u16PosY)//at to the jump point,jump directly 
					{
						if (pjp->uJumpType > 0)//npc jump
						{
							CCharacter* pSelf = gGlobal.g_pCharacter->GetCharNetObject();
							if (NULL != pSelf)//has character 
							{
								UInt32 uNextJumpSeq = pcmpf->GetNextPathPoint();
								if (UINT_MAX != uNextJumpSeq) 
								{
									pSelf->RequestCrossMapJump(gGlobal.pPeerProxy,gGlobal.g_context,uJumpPointsSeq,uNextJumpSeq);
								}
								else//wrong path
								{
									pcmpf->DestroyPathData();
								}
							}
							else//no character
							{
								pcmpf->DestroyPathData();
							}
						}
						else
						{
							pcmpf->GetNextPathPoint();
						}
					}
					else//far from jump point,run to jump point
					{
						if (0 == pjp->uJumpType) 
						{
							pcmpf->GetNextPathPoint();
						}
						gGlobal.g_pCharacter->SetEntityDesPos(pjp->u16PosX, pjp->u16PosY);
						PointList ptList;
						ptList = gGlobal.g_pCharacter->GetCharMainView()->m_PathFindData.m_ptList;

						/*if (ptList.size() > 10)
							gGlobal.g_pCharacter->SetEntityDesPos(ptList[ptList.size() - 10].PosX , ptList[ptList.size() - 10].PosY, true);
						else
							gGlobal.g_pCharacter->SetEntityDesPos(pjp->u16PosX , pjp->u16PosY, true);*/

						if (gGlobal.g_pMiddleMap->IsOpenMiddleMap())
							gGlobal.g_pMiddleMap->DrawPath(pjp->u16PosX, pjp->u16PosY);
						else
						{
							gGlobal.g_pMiddleMap->SetCharDes(pjp->u16PosX, pjp->u16PosY);
							gGlobal.g_pMiddleMap->SetPathEnd(pjp->u16PosX, pjp->u16PosY);
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}

void RunNewGame()
{
	gGlobal.g_NewGame = true;
}


void DoRunNewGame()
{
	//don't show web-site when user reopen the game
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));
	gConfigure.bShowWebSite =FALSE;
	STARTUPINFO startupInfo = {0};
	startupInfo.cb = sizeof(startupInfo);
	PROCESS_INFORMATION processInformation;// Try to start the process

	TCHAR pathFilename[MAX_PATH];
	GetModuleFileName(NULL, pathFilename, MAX_PATH);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Run process %s.."), pathFilename);
	StringA	SPathFilename = pathFilename;

	
	BOOL result = ::CreateProcess(  
		pathFilename,
		NULL, 
		NULL,  
		NULL,  
		FALSE,  
		NORMAL_PRIORITY_CLASS,  
		NULL,  
		NULL,  
		&startupInfo,  
		&processInformation);
		
		
	//WinExec(SPathFilename.c_str(), SW_NORMAL);
	//ShellExecute(NULL, _T("open"), SPathFilename.c_str(), NULL,NULL,SW_SHOWNORMAL); 


	TRACE_INFODTL(GLOBAL_LOGGER, _F("Run process finished."));
}

void ShowPartyPopUp(UInt8 index)//0: 1st member, 1:2nd member,..., 4:myself
{
	UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("PartyPopup");
	UIPanelEx* pPanel = gGlobal.pUIManager->FindUIPanelEx("PartyPanel");
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (pParty && pPopUpMenu && pPanel)
	{
		pPopUpMenu->ClearData();
		if (index == 4)
		{
			pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE4")), 0, "");
			pPopUpMenu->Redraw();
			pPanel->ChangeCount(1);
		}
		else
		{
			UInt8 selectIndex = 0;
			pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE1")), selectIndex, "");
			++selectIndex;

			if (pParty->IsLeader())
			{
				pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE2")), selectIndex, "");
				++selectIndex;
				pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE3")), selectIndex, "");
				++selectIndex;
			}

			pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE5")), selectIndex, "");
			++selectIndex;

			pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE6")), selectIndex, "");
			++selectIndex;

			pPopUpMenu->AddString(gGlobal.GetStringTable()->Get(_T("MSE_PARTY_POP_MESSAGE7")), selectIndex, "");
			++selectIndex;

			pPopUpMenu->Redraw();
			pPanel->ChangeCount(selectIndex);
		}
	}
}

void SelectPartyPopUp(UInt8 popSelect, UInt8 index)//0: 1st member, 1:2nd member,..., 4:myself
{
	UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("PartyPopup");
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (pParty && pPopUpMenu)
	{
		if (index == 4)
		{
			if (popSelect == 0)
				PartyQuit();
		}
		else
		{
			CCharacter * pchar;
			int memberpos = 0;
			int listIndex = -1;
			for( Index i = C_INDEX(0); i < gGlobal.g_vctPartyMemberPointer.size(); ++i )
			{
				pchar = CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[i]);
				if (NULL != pchar) 
				{
					if (pchar->GetChar_id() == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id()) 
						continue;
				}
				if (index == memberpos) 
				{
					listIndex = i;
					break;
				}
				memberpos++;
				/*pchar = pParty->GetMember(i);
				if( pchar && pchar->GetChar_id() != gGlobal.g_pCharacter->GetMapActorID() )
				{
					if( index == memberpos )
					{
						listIndex = i;
						break;
					}
					memberpos++;
				}	*/			
			}
			if (listIndex >= 0 )
				gGlobal.g_pPartyProcess->SelectTeammatePopUp(popSelect, listIndex);
			/*if (popSelect == 0)
			PartyPrivateCall(index);
			else if (gGlobal.g_pParty->IsLeader())
			{
			if (popSelect == 1)
			PartyKick(index);
			else if (popSelect == 2)
			PartyPromote(index);
			}*/
		}
	}
}

PCSTRING LuaGetPartnerName(UInt32 Index)
{
	CPartner *partner = gGlobal.g_pPartnerGroup->GetChild<CPartner>(Index);

	return PCSTRING(partner->GetRaname().c_str());
}

PCSTRING LuaGetPartnerOriginName(UInt32 Index)
{
	CPartner *partner = gGlobal.g_pPartnerGroup->GetChild<CPartner>(Index);

	if (partner->m_data)
		return PCSTRING(partner->m_data->m_basicinfo->mid_name);
	
	return PCSTRING(partner->GetRaname().c_str());
}


UInt32 LuaGetPartnerID(UInt32 Index)
{
	CPartner *partner = gGlobal.g_pPartnerGroup->GetChild<CPartner>(Index);
	return partner->GetPartner_id();
}


PCSTRING LuaGetPetName(UInt32 Index)
{
	CPet *pet = gGlobal.g_PetLst->GetChild<CPet>(Index);
	return PCSTRING(pet->GetName().c_str());
}

PCSTRING LuaGetPetOriginName(UInt32 Index)
{
	CPet *pet = gGlobal.g_PetLst->GetChild<CPet>(Index);

	const PetData* pData = gGlobal.m_resource->GetPetData(pet->GetPet_id());
	
	if (pData)
		return pData->pet_name;
	
	return PCSTRING(pet->GetName().c_str());
}

UInt32 LuaGetPetID(UInt32 Index)
{
	CPet *pet = gGlobal.g_PetLst->GetChild<CPet>(Index);
	return pet->GetPet_uid();
}

void ShowTeammatePopUp(UInt8 index)
{
	gGlobal.g_pPartyProcess->ShowTeammatePopUp(index);
}

void AutoAcceptCheckBox(bool on)
{
	gGlobal.g_pPartyProcess->AutoAcceptCheckBox(on);
}

void SelectTeammatePopUp(UInt8 popSelect, UInt8 index)
{
	gGlobal.g_pPartyProcess->SelectTeammatePopUp(popSelect, index);
}

void SetChangePos(UInt8 index)
{
	gGlobal.g_pPartyProcess->SetChangePos(index);
}

void ChangePartyDesc()
{
	gGlobal.g_pPartyProcess->ChangePartyDesc();
}

void StopCharacter()
{
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
		gGlobal.g_pCharacter->Stop();
}

PCSTRINGW GetLocalStr(const StringA &s)
{
	if (s.IsEmpty())
		return _T("");
	else
		return gGlobal.GetStringTable()->Get(String(s.c_str()).c_str());
}

PCSTRINGW GetLocalStr(PCSTRINGA s)
{
	if (s && s[0] != '\0')
		return gGlobal.GetStringTable()->Get(String(s).c_str());
	else
		return _T("");
}

std::map<String, UInt32> g_ProcessBuf;

BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam)
{
	CharW str[100];

	//::GetWindowText(hwnd, str, sizeof(str));
	::GetClassName(hwnd, str, sizeof(str));
	if(str[0])
	{
		UInt32 i = g_ProcessBuf[str];
		g_ProcessBuf[str] = i + 1;
	}
	return 1;
}

UInt32 CheckMyProcess(HWND hWnd)
{
	TCHAR	Str[128];
	GetClassName(hWnd, Str, 128);
	EnumWindows(lpEnumFunc, NULL);
	UInt32 r = g_ProcessBuf[Str];
	g_ProcessBuf.clear();

	return r;
}

void Global::CheckDownTime(UInt32 dt)
{
	if (m_downTime == 0)
		return;

	// show count down when shutting down
	static UInt32 showTime = 0;
	UInt32 tdiff = showTime - m_downTime;
	int sec = -1;
	int min = -1;

	// reset count down
	if (showTime == 0 || m_downTime > showTime)
		tdiff = MINUTE_TIME;

	// show only for each interval
	if (m_downTime >= MINUTE_TIME && tdiff >= MINUTE_TIME)
	{	// show count down by minute
		min = (m_downTime + MINUTE_TIME / 2) / MINUTE_TIME;
		showTime = min * MINUTE_TIME;
	} else if (m_downTime < MINUTE_TIME && m_downTime > 10 * SECOND_TIME &&
		tdiff >= 10 * SECOND_TIME)
	{	// show count down by 10 second after 1 minute count down
		sec = (m_downTime + SECOND_TIME / 2) / SECOND_TIME;
		if (sec % 10 == 0)
			showTime = sec / 10 * 10 * SECOND_TIME;
		else
			showTime = (sec / 10 + 1) * 10 * SECOND_TIME;
	} else if (m_downTime < 10 * SECOND_TIME && tdiff >= SECOND_TIME)
	{	// show count down by 10 second after 10 second count down
		sec = (m_downTime + SECOND_TIME / 2) / SECOND_TIME;
		showTime = sec * SECOND_TIME;
	} else if (m_downTime <= dt) {
		sec = 0;
		showTime = 0;
	}

	if (m_downTime > dt)
		m_downTime -= dt;
	else
		m_downTime = 0;

	if (min != -1) {
		String buf;
		buf.Format(GetLocalStr("MSG_SYS_SHUTDOWN_MIN"), min);
		PrintMessage(buf, CHANNELTYPE_SYSTEM);
		NotifyReceiveHorseLightMessage(NULL, RPCContext(), buf, 3);
	} else if (sec != -1) {
		String buf;
		buf.Format(GetLocalStr("MSG_SYS_SHUTDOWN_SEC"), sec);
		PrintMessage(buf, CHANNELTYPE_SYSTEM);
	}
}

void CreateCharRotate(bool left)
{
	gGlobal.g_pCreateCharProcess->RotateCharacter(left);
}


void SetHotKeyEnable(bool value)
{
	gGlobal.g_hotKeyEnable = value;
}

UInt32 LuaGetCharPosX()
{
	return gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
}

UInt32 LuaGetCharPosY()
{
	return gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;
}

PCSTRING LuaGetLineTextName()
{
	String lineTextName;
	lineTextName.Format(_T("MSG_INTERFACE_LINE_%d"), gGlobal.g_CurLineID);
	
	return gGlobal.GetStringTable()->Get(lineTextName.c_str());
}

static HHOOK g_fhkb = NULL;
static HHOOK g_fhms = NULL;
static HHOOK g_fhgm = NULL;
static HHOOK g_hkb = NULL;
static HHOOK g_hms = NULL;
static HHOOK g_hgm = NULL;

LRESULT CALLBACK HookKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return  CallNextHookEx(g_fhkb, nCode, wParam, lParam);
}

LRESULT CALLBACK HookMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return  CallNextHookEx(g_fhms, nCode, wParam, lParam);
}

LRESULT CALLBACK HookGetMessage(int nCode, WPARAM wParam, LPARAM lParam)
{
	return  CallNextHookEx(g_fhgm, nCode, wParam, lParam);
}

BOOL InstallHook()
{
	g_hkb = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)HookKeyboardProc, NULL, GetCurrentThreadId());
	g_hms = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)HookMouseProc, NULL, GetCurrentThreadId());
	g_hgm = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)HookGetMessage, NULL, GetCurrentThreadId());

	return TRUE;
}

BOOL UnHook()
{
	BOOL unhooked1 = UnhookWindowsHookEx(g_hkb);
	BOOL unhooked2 = UnhookWindowsHookEx(g_hms);
	BOOL unhooked3 = UnhookWindowsHookEx(g_hgm);


	return (unhooked1 && unhooked2 && unhooked3);
} 

LRESULT CALLBACK HookFirstKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return  CallNextHookEx(g_fhkb, nCode, wParam, lParam);
}

LRESULT CALLBACK HookFirstMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return  CallNextHookEx(g_fhms, nCode, wParam, lParam);
}

LRESULT CALLBACK HookFirstGetMessage(int nCode, WPARAM wParam, LPARAM lParam)
{
	return  CallNextHookEx(g_fhgm, nCode, wParam, lParam);
}

BOOL InstallFirstHook()
{
	g_fhkb = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)HookFirstKeyboardProc, NULL, GetCurrentThreadId());
	g_fhms = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)HookFirstMouseProc, NULL, GetCurrentThreadId());
	g_fhgm = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)HookFirstGetMessage, NULL, GetCurrentThreadId());

	return TRUE;
}

BOOL UnHookFirst()
{
	BOOL unhooked1 = UnhookWindowsHookEx(g_fhkb);
	BOOL unhooked2 = UnhookWindowsHookEx(g_fhms);
	BOOL unhooked3 = UnhookWindowsHookEx(g_fhgm);

	return (unhooked1 && unhooked2 && unhooked3);
} 


HWND MyFindWindow(PCSTRING ClassName)
{
	return FindWindow(ClassName, NULL);
}

Color32 GetColorByMoney(UInt32 gmoney)
{
	Color32 tempColor = 0;
	if(0 <= gmoney )
	{
		if ( gmoney < 10000)
			tempColor = TextColor[ColorIndex_White];
		else if ( 10000 <= gmoney && gmoney < 100000)
			tempColor = TextColor[ColorIndex_Green];
		else if ( 100000 <= gmoney && gmoney < 1000000)
			tempColor = TextColor[ColorIndex_Blue];
		else if ( 1000000 <= gmoney && gmoney < 10000000)
			tempColor = TextColor[ColorIndex_Violet];
		else
			tempColor = ItemTextColor[ItemColorIndex_Golden];	
	}
	return tempColor;
}

UInt GetFontColorIndexByMoney(UInt32 gmoney)
{
	//special for font color array;
	UInt tempColor = 0;
	if(0 <= gmoney )
	{
		if ( gmoney < 10000)
			tempColor = 0;
		else if ( 10000 <= gmoney && gmoney < 100000)
			tempColor = 7;
		else if ( 100000 <= gmoney && gmoney < 1000000)
			tempColor = 8;
		else if ( 1000000 <= gmoney && gmoney < 10000000)
			tempColor = 9;
		else
			tempColor = 10;	
	}
	return tempColor;
}

void SetTextMoneyColor(UIStaticText* gtext,UInt32 gmoney)
{
	if(gtext)
	{
		if(0 <= gmoney )
		{
			Color32 textColor = GetColorByMoney(gmoney);
			if (textColor != 0 )
				gtext->SetColor(textColor);
		}
	}
}

void SetEditBoxMoneyColor(UIEditBox* geditbox)
{
	if (geditbox)
	{
		int gmoney = geditbox->GetInt();
		Color32 textColor = GetColorByMoney(gmoney);
		if (textColor != 0 )
			geditbox->SetColor(textColor);
		geditbox->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(geditbox);
	}
}

#pragma warning(disable:4018)
#pragma warning(disable:4267)

const int g_LineupBtbltPos[20][2] ={{210,158},{179,174},{241,142},{148,190},{272,126},
{179,142},{148,158},{210,126},{117,174},{241,110},
{148,126},{117,142},{179,110},{85,158},{210,94},
{117,110},{85,126},{148,94},{54,142},{179,78}};

void PartySelectLineup()
{
	UIListBox* pListBox = (UIListBox*)gGlobal.pUIManager->GetUIItem("LearnedLineup");
	if (NULL != pListBox) {
		int iSel=pListBox->GetBarPos();
		if (NULL == gGlobal.g_pCharacter) 
			return;
		CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL == pChar) 
			return;
		int iTotalLineup = 0;
		UInt16 i=1;
		for (;i<MAXLINEUP;i++) {
			if (gGlobal.g_pCharacter->GetSkillById(i) == NULL)
				continue;
			if ( iTotalLineup == iSel)
				break;
			iTotalLineup++;
		}
		pChar->ModifyLineup(gGlobal.pPeerProxy,gGlobal.g_context,i);
	}
}
void PartySetKeyPos(int iKeyPos)
{
	if (NULL == gGlobal.g_pParty || !gGlobal.g_pParty->IsLeader() || NULL == gGlobal.g_pCharacter) 
		return;
	if (!gGlobal.g_pParty->IsLeader())
		return;
	if (NULL == gGlobal.g_pCharacter) 
		return;
	CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
	if (NULL == pChar) 
		return;
	UIButton* pCurKeyPos = NULL;
	for (int i = 0; i < 5; i++) 
	{
		StringA	KeyStr = "KeyPosSelect";
		KeyStr += 'A' + i;
		pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem(KeyStr.c_str());
		if (NULL != pCurKeyPos) {
			pCurKeyPos->SetState(_ButtonNormal);
			pCurKeyPos->SetNotDetected(false);
		}
	}

	pChar->ModifyLineupKeyPos(gGlobal.pPeerProxy,gGlobal.g_context,iKeyPos);
}

void UpdateLineupSelectMenu()
{
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (NULL == pParty )
		return;
	UIListBox* pListBox = (UIListBox*)gGlobal.pUIManager->GetUIItem("LearnedLineup");
	if (NULL != pListBox) {
		int iCurSel=0;
		if (NULL == gGlobal.g_pCharacter) 
			return;
		CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL == pChar)
			return;
		pListBox->Dispose();
		UInt32 uTotalLineup = 0;
		for (UInt16 i=1;i<MAXLINEUP;i++) {
			if (gGlobal.g_pCharacter->GetSkillById(i) == NULL)
				continue;
			uTotalLineup++;
			if ( i == pParty->GetM_uLineupId()) {
				iCurSel = uTotalLineup-1;
			}
			const LINEUPINFO* pLineupInfo=gGlobal.m_resource->GetLineupData(i);
			if (NULL != pLineupInfo) {
				pListBox->AddString(pLineupInfo->szName);
			}
			if (uTotalLineup >= 5) 
				break;
		}
		pListBox->DrawFont();

		UIPanelEx* pMenuLineupLearned= (UIPanelEx*)gGlobal.pUIManager->FindUIPanelEx("MenuLearnedLineup");
		if (NULL != pMenuLineupLearned) {
			pMenuLineupLearned->ChangeCount(uTotalLineup);
		}

		pListBox->DrawFont();
		pListBox->SetCurSel(iCurSel);
		pListBox->SetEnable(false);
		UIStaticText* pText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("CurrentLineup");
		if (NULL != pText) {
			pText->SetText(pListBox->GetBarString());
			pText->Redraw();
		}
		
		pText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("TeamMenuLineupKeyPos");
		if (NULL != pText) {
			String strTextKeyPos;
			strTextKeyPos.Format(_T("%d"),pParty->GetM_iLineupKeyPos()+1);
			pText->SetText(strTextKeyPos);
			pText->Redraw();
		}
		const LINEUPINFO* pSelLineup=gGlobal.m_resource->GetLineupData(pParty->GetM_uLineupId());
		if (NULL != pSelLineup) {
			UIStaticText* pLineupDes= (UIStaticText*)gGlobal.pUIManager->GetUIItem("CurrentLineupDesc");
			if (NULL != pLineupDes) {
				pLineupDes->SetText(pSelLineup->szDesc);
				pLineupDes->Redraw();
			}
			int iFrontPriv[5][2]={0};
			for (int j=0;j<5;j++) {
				iFrontPriv[j][0]=pSelLineup->pb[j].iPos/5;
				iFrontPriv[j][1]=pSelLineup->pb[j].iPosPartner/5;
			}

			UIPanel* pPanel=gGlobal.pUIManager->FindUIPanel("LineupSelectPanel");
			if (NULL == pPanel)
				return;
			for (Int k=3;k>-1;k--) {
				for (Int l=0;l<5;l++) {
					Int iRealX,iRealY;
					if (k == iFrontPriv[l][1]) {
						StringA szKeyWords;
						szKeyWords.Format("TeamLineupPartnerPos%d", l);
						iRealX=g_LineupBtbltPos[pSelLineup->pb[l].iPosPartner][0];
						iRealY=g_LineupBtbltPos[pSelLineup->pb[l].iPosPartner][1];
						UIImage* pImage=gGlobal.pUIManager->FindUIImage(szKeyWords.c_str());
						if (NULL != pImage) {
							pPanel->LeaveUIItem(pImage);
							pPanel->AddUIItem(pImage);
							pImage->SetOffsetXY(iRealX,iRealY);
						}
					}
					if (k == iFrontPriv[l][0]) {
						StringA szKeyWords;
						szKeyWords.Format("TeamLineupPos%d", l);
						iRealX=g_LineupBtbltPos[pSelLineup->pb[l].iPos][0];
						iRealY=g_LineupBtbltPos[pSelLineup->pb[l].iPos][1];
						UIImage* pImage=gGlobal.pUIManager->FindUIImage(szKeyWords.c_str());
						if (NULL != pImage) {
							pPanel->LeaveUIItem(pImage);
							pPanel->AddUIItem(pImage);
							pImage->SetOffsetXY(iRealX,iRealY);
						}
					}
				}
			}
		}

		UIButton* pCurKeyPos = NULL;
		Int	iRealKeyPos = pParty->GetM_iLineupKeyPos();
		if(iRealKeyPos >= 0 && iRealKeyPos < 5)
		{
			StringA KeyName = "KeyPosSelect";
			KeyName += 'A' + iRealKeyPos;
			pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem(KeyName.c_str());
		}
	}
}

void OpenLineupInfoView()
{
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	if (NULL == pParty )
		return;
	UIListBox* pListBox = (UIListBox*)gGlobal.pUIManager->GetUIItem("LearnedLineup");
	if (NULL != pListBox) {
		int iCurSel=0;
		if (NULL == gGlobal.g_pCharacter) 
			return;
		CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL == pChar) 
			return;
		UIStaticText* pText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("CurrentLineup");
		if (NULL != pText) {
			pText->SetText(pListBox->GetBarString());
			pText->Redraw();
		}

		const LINEUPINFO* pSelLineup=gGlobal.m_resource->GetLineupData(pParty->GetM_uLineupId());
		if (NULL != pSelLineup) {
			UIStaticText* pText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("CurrentLineup");
			if (NULL != pText) {
				pText->SetText(pSelLineup->szName);
				pText->Redraw();
			}

			UIStaticText* pLineupDes= (UIStaticText*)gGlobal.pUIManager->GetUIItem("CurrentLineupDesc");
			if (NULL != pLineupDes) {
				pLineupDes->SetText(pSelLineup->szDesc);
				pLineupDes->Redraw();
			}
			int iFrontPriv[5][2]={0};
			for (int j=0;j<5;j++) {
				iFrontPriv[j][0]=pSelLineup->pb[j].iPos/5;
				iFrontPriv[j][1]=pSelLineup->pb[j].iPosPartner/5;
			}

			UIPanel* pPanel=gGlobal.pUIManager->FindUIPanel("LineupSelectPanel");
			if (NULL == pPanel)
				return;
			for (int k=3;k>-1;k--) {
				for (int l=0;l<5;l++) {
					int iRealX,iRealY;
					if (k == iFrontPriv[l][1]) {
						char szKeyWords[32]={0};
						sprintf(szKeyWords,"TeamLineupPartnerPos%d",l);
						iRealX=g_LineupBtbltPos[pSelLineup->pb[l].iPosPartner][0];
						iRealY=g_LineupBtbltPos[pSelLineup->pb[l].iPosPartner][1];
						UIImage* pImage=gGlobal.pUIManager->FindUIImage(szKeyWords);
						if (NULL != pImage) {
							pPanel->LeaveUIItem(pImage);
							pPanel->AddUIItem(pImage);
							pImage->SetOffsetXY(iRealX,iRealY);
						}
					}
					if (k == iFrontPriv[l][0]) {
						char szKeyWords[32]={0};
						sprintf(szKeyWords,"TeamLineupPos%d",l);
						iRealX=g_LineupBtbltPos[pSelLineup->pb[l].iPos][0];
						iRealY=g_LineupBtbltPos[pSelLineup->pb[l].iPos][1];
						UIImage* pImage=gGlobal.pUIManager->FindUIImage(szKeyWords);
						if (NULL != pImage) {
							pPanel->LeaveUIItem(pImage);
							pPanel->AddUIItem(pImage);
							pImage->SetOffsetXY(iRealX,iRealY);
						}
					}
				}
			}
		}

		UIButton* pCurKeyPos = NULL;
		int iCurKeyPos = pParty->GetM_iLineupKeyPos();
		pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem("KeyPosSelectA");
		if (NULL != pCurKeyPos) {
			pCurKeyPos->SetState(_ButtonDisable);
			pCurKeyPos->SetNotDetected(true);
		}
		pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem("KeyPosSelectB");
		if (NULL != pCurKeyPos) {
			pCurKeyPos->SetState(_ButtonDisable);
			pCurKeyPos->SetNotDetected(true);
		}
		pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem("KeyPosSelectC");
		if (NULL != pCurKeyPos) {
			pCurKeyPos->SetState(_ButtonDisable);
			pCurKeyPos->SetNotDetected(true);
		}
		pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem("KeyPosSelectD");
		if (NULL != pCurKeyPos) {
			pCurKeyPos->SetState(_ButtonDisable);
			pCurKeyPos->SetNotDetected(true);
		}
		pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem("KeyPosSelectE");
		if (NULL != pCurKeyPos) {
			pCurKeyPos->SetState(_ButtonDisable);
			pCurKeyPos->SetNotDetected(true);
		}

		UIButton* pOperationBtn = (UIButton*)gGlobal.pUIManager->GetUIItem("DropLineupList");
		if (NULL != pOperationBtn) {
			pOperationBtn->SetState(_ButtonDisable);
			pOperationBtn->SetNotDetected(true);
		}
		pOperationBtn = (UIButton*)gGlobal.pUIManager->GetUIItem("ForgetLineup");
		if (NULL != pOperationBtn) {
			pOperationBtn->SetState(_ButtonDisable);
			pOperationBtn->SetNotDetected(true);
		}
		pOperationBtn = (UIButton*)gGlobal.pUIManager->GetUIItem("SelectLineup");
		if (NULL != pOperationBtn) {
			pOperationBtn->SetState(_ButtonDisable);
			pOperationBtn->SetNotDetected(true);
		}
	}
}
void ForgetLearnedLineup()
{
	UIListBox* pListBox = (UIListBox*)gGlobal.pUIManager->GetUIItem("LearnedLineup");
	if (NULL != pListBox) {
		int iSel=pListBox->GetBarPos();
		if (NULL == gGlobal.g_pCharacter) 
			return;
		CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL == pChar) 
			return;
		int iTotalLineup = 0;

		for (UInt16 i = 1; i<MAXLINEUP;i ++) {
			if (gGlobal.g_pCharacter->GetSkillById(i))
			{
				if ( iTotalLineup == iSel)
				{
					pChar->ForgiveLineup(gGlobal.pPeerProxy,gGlobal.g_context,i);
					break;
				}
				iTotalLineup++;
			}
		}
		/*
		UInt16 i=1;
		for (;i<MAXLINEUP;i++) {
			if (gGlobal.g_pCharacter->GetSkillById(i) == NULL)
				continue;
			if ( iTotalLineup == iSel)
				break;
			iTotalLineup++;
		}

		if (i<MAXLINEUP) {
			pChar->ForgiveLineup(gGlobal.pPeerProxy,gGlobal.g_context,i);
		}
		*/
	}
}
void PartySetSelectLineupResualt()
{
	UIListBox* pListBox = (UIListBox*)gGlobal.pUIManager->GetUIItem("LearnedLineup");
	if (NULL != pListBox) {
		UIStaticText* pText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("CurrentLineup");
		if (NULL != pText) {
			pText->SetText(pListBox->GetBarString());
			pText->Redraw();
		}

		if (NULL == gGlobal.g_pCharacter) 
			return;
		CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL == pChar) 
			return;

		int iTotalLineup = 0;
		int iCurSel = pListBox->GetBarPos();
		UInt16 i=1;
		for (;i<MAXLINEUP;i++) {
			if (gGlobal.g_pCharacter->GetSkillById(i) == NULL)
				continue;
			if ( iTotalLineup == iCurSel)
				break;
			iTotalLineup++;
		}

		if (i<MAXLINEUP) {
			const LINEUPINFO* pSelLineup=gGlobal.m_resource->GetLineupData(i);
			if (NULL != pSelLineup) {
				UIStaticText* pLineupDes= (UIStaticText*)gGlobal.pUIManager->GetUIItem("CurrentLineupDesc");
				if (NULL != pLineupDes) {
					pLineupDes->SetText(pSelLineup->szDesc);
					pLineupDes->Redraw();
				}

				int iFrontPriv[5][2]={0};
				for (int j=0;j<5;j++) {
					iFrontPriv[j][0]=pSelLineup->pb[j].iPos/5;
					iFrontPriv[j][1]=pSelLineup->pb[j].iPosPartner/5;
				}

				UIPanel* pPanel=gGlobal.pUIManager->FindUIPanel("LineupSelectPanel");
				if (NULL == pPanel)
					return;

				for (int k=3;k>-1;k--) {
					for (int l=0;l<5;l++) {
						int iRealX,iRealY;
						if (k == iFrontPriv[l][1]) {
							StringA szKeyWords;
							szKeyWords.Format("TeamLineupPartnerPos%d", l);
							iRealX=g_LineupBtbltPos[pSelLineup->pb[l].iPosPartner][0];
							iRealY=g_LineupBtbltPos[pSelLineup->pb[l].iPosPartner][1];
							UIImage* pImage=gGlobal.pUIManager->FindUIImage(szKeyWords.c_str());
							if (NULL != pImage) {
								pPanel->LeaveUIItem(pImage);
								pPanel->AddUIItem(pImage);
								pImage->SetOffsetXY(iRealX,iRealY);
							}
						}
						if (k == iFrontPriv[l][0]) {
							StringA szKeyWords;
							szKeyWords.Format("TeamLineupPos%d", l);
							iRealX=g_LineupBtbltPos[pSelLineup->pb[l].iPos][0];
							iRealY=g_LineupBtbltPos[pSelLineup->pb[l].iPos][1];
							UIImage* pImage=gGlobal.pUIManager->FindUIImage(szKeyWords.c_str());
							if (NULL != pImage) {
								pPanel->LeaveUIItem(pImage);
								pPanel->AddUIItem(pImage);
								pImage->SetOffsetXY(iRealX,iRealY);
							}
						}
					}
				}
			}
		}
	}
}

void OpenWebLink()
{
	if(!gConfigure.strWebLink.empty())
		ShellExecute(NULL, _T("open"), gConfigure.strWebLink.c_str(),
		NULL, NULL, SW_SHOWNORMAL);
}

void SetQGLTitle(PCSTRING strCaption, UInt32 char_id, 
					PCSTRING strNickname, PCSTRING strServerName, UInt8 lineId, UInt8 mapType
					)
{
	String lineTextName;
	if( gGlobal.g_CurLineID )
	{
		lineTextName.Format(_T("MSG_INTERFACE_LINE_%d"), gGlobal.g_CurLineID);
	}else
	{
		// instance type -> show its type instead of line number above
		switch( mapType )
		{
		case MAP_TYPE_INSTANCE_MANOR:
			lineTextName.Format(_T("MSG_INTERFACE_LINE_MANOR"));
			break;
		case MAP_TYPE_INSTANCE_DUNGEON:
			lineTextName.Format(_T("MSG_INTERFACE_LINE_INST"));
			break;
		case MAP_TYPE_INSTANCE_HOUSE:
			lineTextName.Format(_T("MSG_INTERFACE_LINE_HOUSE"));
			break;
		default:
			lineTextName.Format(_T("MSG_INTERFACE_LINE_%d"), gGlobal.g_CurLineID);
			break;
		}
	}

	String buf;
	if( strServerName )
	{
		buf.Format(_T("%s %s[UID: %d] (%s-%s)"),
			strCaption,	strNickname, char_id, strServerName,
			gGlobal.GetStringTable()->Get(lineTextName.c_str())
			);
	}else
	{
		buf.Format(_T("%s %s[UID: %d] (%s)"), 
			strCaption,	strNickname, char_id,
			gGlobal.GetStringTable()->Get(lineTextName.c_str())
			);
	}
	QGLApplication::GetInstance().GetWindow().SetTitle(buf.c_str());
}

void CharEatFastFood(UInt8 foodType)
{
	if (NULL == gGlobal.g_pCharacter) 
		return;
	CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
	if (NULL == pChar) 
		return;
	if (gGlobal.m_pBag)
	{
		gGlobal.m_pBag->UseFastFoodItem(gGlobal.pPeerProxy,gGlobal.g_context,foodType,0);
	}
}

void LeaveTemporarily()
{
	if (NULL == gGlobal.g_pParty)
		return;
	if (gGlobal.g_pParty->IsLeader())
		return;
	if (NULL != gGlobal.g_pCharacter) 
	{
		CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar) 
			pChar->LeaveTeamTemporarily(gGlobal.pPeerProxy,gGlobal.g_context);
	}
}
void ReturnParty()
{
	if(NULL != gGlobal.g_pParty || NULL == gGlobal.g_pTempLeaveParty)
		return;
	if (NULL != gGlobal.g_pCharacter) 
	{
		CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar)
			pChar->ReturnParty(gGlobal.pPeerProxy,gGlobal.g_context);
	}
}

void Global::ClearShortCutSlotAll()
{
	for (int i = 0; i < MAXSHOTNUM; i++)
	{
		ClearShortCutSlot(i);
	}
}

void SetWebLink(PCSTRINGA link, int typeno)
{
	switch(typeno)
	{
	case WEBLINK_TYPE_WEB:
		UTF82StringW(gConfigure.strWebLink, link);
		break;
	case WEBLINK_TYPE_CHARGE:
		UTF82StringW(gConfigure.strChargeLink, link);
		break;
	case WEBLINK_TYPE_TRANSFER:
		UTF82StringW(gConfigure.strTransferLink, link);
		break;
	case WEBLINK_TYPE_CS:
		UTF82StringW(gConfigure.strCSLink, link);
		break;
	}
}
#pragma warning(default:4018)
#pragma warning(default:4267)
