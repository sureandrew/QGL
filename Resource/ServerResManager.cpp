//-- Common
#include "Common.h"
//-- Self
#include "ServerResManager.h"
//-- Library
#include "FileManager.h"
#include "OBJBlockManager.h"
#include "ItemManager.h"
#include "SkillManager.h"
#include "MobManager.h"
#include "MapManager.h"
#include "NpcManager.h"
#include "ScriptManager.h"
#include "StartCharManager.h"
#include "ShopLstManager.h"
#include "BuffManager.h"
#include "SkillCombinationManager.h"
#include "SkillShopManager.h"
#include "BlockWordManager.h"
#include "EventScheduleManager.h"
#include "ChargeShopManager.h"
#include "AntiHackManager.h"
#include "JournalManager.h"
#include "QuestionManager.h"

#include "RandTableManager.h"
#include "PetManager.h"
#include "MixShopManager.h"
#include "TitleManager.h"
#include "PartnerCollectionManager.h"
#include "ItemSetManager.h"
#include "Lineup.h"
#include "ManorManager.h"
#include "BusinessShopManager.h"
#include "AccmulationShop.h"
#include "PointsShopManager.h"
#include "AutoPath.h"

#define CONFIGURATION_INI_FILE		_T(".\\Data\\Setting\\fileconfig.ini")
#define FILENAME_LENGTH				1024

CServerResManager::CServerResManager()
{
	m_fileManager = NULL;
	m_objBlockManager = NULL;
	m_itemManager = NULL;
	m_skillManager = NULL;
	m_mobManager = NULL;
	m_mapManager = NULL;
	m_npcManager = NULL;
	m_pScriptManager = NULL;
	m_startCharManager = NULL;
	m_pShopLstManager = NULL;
	//m_pCSVItemManager = NULL;
	m_BuffManager = NULL;
	m_SkillCombinationManager = NULL;
	m_SkillShopManager = NULL;
	m_pBlockWordManager = NULL;
	m_pRandTableManager = NULL;
	m_EventScheduleManager = NULL;
	m_pChargeShopManager = NULL;
	m_mixShopManager = NULL;
	m_petManager = NULL;
	m_TitleManager = NULL;
	m_pHackManager = NULL;
	m_pJournalManager = NULL;
	m_pQuestionManager = NULL;
	m_pPartnerCollectionManager = NULL;
	m_pItemSetManager = NULL;
	m_pLineupManager = NULL;
	m_manorManager = NULL;	
	m_pBusinessShopManager = NULL;
	m_pAccmulationShopManager =NULL;
	m_pPointsShopManager =NULL;
	m_pAutoPathManager = SafeCreateObject(CAutoPathManager);
}

CServerResManager::~CServerResManager()
{
	SafeDeleteObject(m_fileManager);
	SafeDeleteObject(m_objBlockManager);
	SafeDeleteObject(m_itemManager);
	SafeDeleteObject(m_skillManager);
	SafeDeleteObject(m_mobManager);
	SafeDeleteObject(m_mapManager);
	SafeDeleteObject(m_npcManager);
	SafeDeleteObject(m_pScriptManager);
	SafeDeleteObject(m_startCharManager);
	SafeDeleteObject(m_pShopLstManager);
	//SafeDeleteObject(m_pCSVItemManager);
	SafeDeleteObject(m_BuffManager);
	SafeDeleteObject(m_SkillShopManager);
	SafeDeleteObject(m_SkillCombinationManager);
	SafeDeleteObject(m_pBlockWordManager);
	SafeDeleteObject(m_pRandTableManager);
	SafeDeleteObject(m_EventScheduleManager);
	SafeDeleteObject(m_pChargeShopManager);
	SafeDeleteObject(m_petManager);
	SafeDeleteObject(m_mixShopManager);
	SafeDeleteObject(m_TitleManager);
	SafeDeleteObject(m_pHackManager);	
	SafeDeleteObject(m_pJournalManager);	
	SafeDeleteObject(m_pQuestionManager);
	SafeDeleteObject(m_pPartnerCollectionManager);
	SafeDeleteObject(m_pItemSetManager);
	SafeDeleteObject(m_pLineupManager);
	SafeDeleteObject(m_manorManager);	
	SafeDeleteObject(m_pBusinessShopManager);
	SafeDeleteObject(m_pAccmulationShopManager);
	SafeDeleteObject(m_pPointsShopManager);
	SafeDeleteObject(m_pAutoPathManager);
}

BYTE* CServerResManager::LoadFileData(const TCHAR *filename, UInt64 & size)
{
	if (m_fileManager == NULL)
		m_fileManager = SafeCreateObject(CFileManager);

	return m_fileManager->Load(filename, size);
}



BOOL CServerResManager::FreeFileData(const TCHAR *filename)
{
	if (m_fileManager)
		return m_fileManager->Free(filename);
	else
		return NULL;
}

BlockPosVector* CServerResManager::LoadOBJBlocking(const TCHAR *filename, Int state)
{
	if (m_objBlockManager == NULL)
		m_objBlockManager = SafeCreateObject(COBJBlockManager);

	return m_objBlockManager->Load(filename, state);
}

BlockPosVector* CServerResManager::LoadOBJBlocking_FromEntFile(const TCHAR *filename, Int state)
{
	if (m_objBlockManager)
		return m_objBlockManager->LoadEnt(filename, state);
	else
		return NULL;
}

BOOL CServerResManager::FreeOBJBlocking(const TCHAR *filename)
{
	if (m_objBlockManager)
		return m_objBlockManager->Free(filename);
	else
		return NULL;
}

const ItemData* CServerResManager::GetItemData(UInt32 itemid)
{
	if (m_itemManager)
		return m_itemManager->GetItem(itemid);
	else
		return NULL;
}

void CServerResManager::GetItemList(ItemDataList &itemList, UInt8 filterType, UInt8 item_type)
{
	if (m_itemManager)
		m_itemManager->GetItemList(itemList, filterType, item_type);
}

const SkillData* CServerResManager::GetSkillData(UInt16 skillid)
{
	if (m_skillManager)
		return m_skillManager->GetSkill(skillid);
	else
		return NULL;
}

void CServerResManager::GetSkillList(SkillDataList &skillList)
{
	if (m_skillManager)
		m_skillManager->GetSkillList(skillList);
}

void CServerResManager::GetSkillLink(UInt16 skill_id, SkillDataList &skillList)
{
	if (m_skillManager)
		m_skillManager->GetSkillLink(skill_id, skillList);
}

void CServerResManager::GetSkillLink(UInt16 skill_id, UInt16Vector &skillIds)
{
	if (m_skillManager)
		m_skillManager->GetSkillLink(skill_id, skillIds);
}

const MobData* CServerResManager::GetMobData(UInt32 mob_id)
{
	if (m_mobManager)
		return m_mobManager->GetMob(mob_id);
	else
		return NULL;
}

void CServerResManager::GetMobList(MobDataList &mobList)
{
	if (m_mobManager)
		m_mobManager->GetMobList(mobList);
}

const MapDB* CServerResManager::GetMapData(UInt32 mapid)
{
	if (m_mapManager)
		return m_mapManager->GetMap(mapid);
	else
		return NULL;
}

void CServerResManager::GetMapList(MapMapInt& mapList)
{
	if (m_mapManager)
		m_mapManager->GetMapList(mapList);
}

BOOL CServerResManager::GetNpcPosition(UInt32 map_id, UInt32 npc_id, UInt16 &x, UInt16 &y)
{
	if (m_mapManager)
		return m_mapManager->GetNpcPosition(map_id, npc_id, x, y);
	else
		return FALSE;
}

BinaryServerNPCInstance* CServerResManager::GetNpcInstance(UInt32 map_id, UInt32 npc_id)
{
	if (m_mapManager)
		return m_mapManager->GetNpcInstance(map_id, npc_id);
	else
		return NULL;
}

BOOL CServerResManager::ActivateMap(UInt32 mapid, PathFindLoadData& pPathFindData, UInt32& pNumGuide)
{
	TCHAR dir[FILENAME_LENGTH];
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportServerZoneDataDirectory"), 
		_T("\0"), dir, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if (!m_mapManager)
		return BOOLEAN_FALSE;

	BOOL bSuccess = m_mapManager->LoadZoneDAT(mapid, dir) && 
		m_mapManager->LoadPFDAT(mapid, dir, pPathFindData, pNumGuide);

	if( bSuccess )
		m_mapManager->TestMapData(mapid);

	return bSuccess;
}

const NpcData* CServerResManager::GetNpcData(UInt32 npcid)
{
	if (m_npcManager)
		return m_npcManager->GetNpc(npcid);
	else
		return NULL;
}

void CServerResManager::LoadItemCSV()
{
	TCHAR itempath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("ItemCsv"), 
		_T("\0"), itempath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (m_itemManager == NULL)
		m_itemManager = SafeCreateObject(CItemManager);

	m_itemManager->LoadCSV(itempath_csv);
}

void CServerResManager::LoadSkillCSV()
{
	TCHAR skillpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("SkillCsv"), 
		_T("\0"), skillpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (m_skillManager == NULL)
		m_skillManager = SafeCreateObject(CSkillManager);

	m_skillManager->LoadCSV(skillpath_csv);
}

//Title Manager
const TitleData* CServerResManager::GetTitleData(UInt32 title_id)
{
	if (m_TitleManager)
		return m_TitleManager->GetTitle(title_id);

	return NULL;
}

const TitleData* CServerResManager::GetTitleDataByType(UInt8	Title_Type, UInt8	Title_RankSubType, UInt8	Title_StartRank, UInt8	Title_EndRank)
{
	if (m_TitleManager)
		return m_TitleManager->GetTitleDataByType(Title_Type, Title_RankSubType, 
										Title_StartRank, Title_EndRank);
	return NULL;
}
const TitleData* CServerResManager::GetTitleDataByRank(UInt8	Title_RankType, UInt8	Title_RankSubType, 
													   UInt8	Title_StartRank, UInt8	Title_EndRank)
{
	if (m_TitleManager)
		return m_TitleManager->GetTitleByRank(Title_RankType, Title_RankSubType, 
										Title_StartRank, Title_EndRank);
	return NULL;
}
void CServerResManager::LoadTitleCSV()
{
	TCHAR Titlepath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("TitleCsv"), 
		_T("\0"), Titlepath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_TitleManager == NULL)
		m_TitleManager = SafeCreateObject(CTitleManager);
	
	
	m_TitleManager->LoadCSV(Titlepath_csv);
	
}
void CServerResManager::LoadPetCSV()
{
	TCHAR Petpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("PetCsv"), 
		_T("\0"), Petpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_petManager == NULL)
		m_petManager = SafeCreateObject(CPetManager);
	
	m_petManager->LoadCSV(Petpath_csv);
	
}

const PetData* CServerResManager::GetPetData(UInt32 pet_id)
{
	if (m_petManager)
		return m_petManager->GetPet(pet_id);
	return NULL;
}
void CServerResManager::LoadItemDAT()
{
//	TCHAR itempath[FILENAME_LENGTH];
//	GetPrivateProfileString(_T("DataSourcePath"), _T("PATH_ITEMDATA_FILE"), 
//		_T("\0"), itempath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
//	if (m_itemManager == NULL)
//		m_itemManager = SafeCreateObject(CItemManager);
//
//	m_itemManager->LoadDAT(itempath);
}

void CServerResManager::LoadSkillDAT()
{
//	TCHAR skillpath[FILENAME_LENGTH];
//	GetPrivateProfileString(_T("DataSourcePath"), _T("PATH_SKILLDATA_FILE"), 
//		_T("\0"), skillpath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
//	if (m_skillManager == NULL)
//		m_skillManager = SafeCreateObject(CSkillManager);
//
//	m_skillManager->LoadDAT(skillpath);
}

void CServerResManager::LoadMobCSV()
{
	TCHAR Mobpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("MobCsv"), 
		_T("\0"), Mobpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if (m_mobManager == NULL)
		m_mobManager = SafeCreateObject(CMobManager);

	m_mobManager->LoadCSV(Mobpath_csv);
}

void CServerResManager::LoadMobDAT()
{
	TCHAR Mobpath[FILENAME_LENGTH];
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportServerMOBInfoFilename"), 
		_T("\0"), Mobpath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if (m_mobManager == NULL)
		m_mobManager = SafeCreateObject(CMobManager);

	m_mobManager->LoadDAT(Mobpath);
}

VOID CServerResManager::LoadMapDAT()
{
	TCHAR mappath_dat[FILENAME_LENGTH];
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportServerMapInfoFilename"), 
		_T("\0"), mappath_dat, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if (m_mapManager == NULL)
		m_mapManager = SafeCreateObject(CMapManager);

	m_mapManager->LoadDAT(mappath_dat);
	m_mapManager->SetNpcManager(m_npcManager);
}

VOID CServerResManager::LoadMapNpc()
{
	TCHAR dir[FILENAME_LENGTH];
	GetPrivateProfileString(_T("ExportFileConfiguration"), 
		_T("ExportServerZoneDataDirectory"), _T("\0"), dir, 
		FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	m_mapManager->LoadAllNpc(dir);
}

void CServerResManager::LoadNpcCSV()
{
	TCHAR npc_csv[FILENAME_LENGTH];
	TCHAR npcMob_csv[FILENAME_LENGTH];
	TCHAR npcDrop_csv[FILENAME_LENGTH];
	TCHAR npcMaleName_csv[FILENAME_LENGTH];
	TCHAR npcFemaleName_csv[FILENAME_LENGTH];
	TCHAR npcPreName_csv[FILENAME_LENGTH];

	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcCsv"), 
		_T("\0"), npc_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcMobCsv"), 
		_T("\0"), npcMob_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcDropCsv"), 
		_T("\0"), npcDrop_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcMaleNameCsv"), 
		_T("\0"), npcMaleName_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcFemaleNameCsv"), 
		_T("\0"), npcFemaleName_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcPreNameCsv"), 
		_T("\0"), npcPreName_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);	

	if (m_npcManager == NULL)
		m_npcManager = SafeCreateObject(CNpcManager);

	m_npcManager->LoadCSV(npc_csv, npcMob_csv, npcDrop_csv);

}

void CServerResManager::LoadNpcNameCSV()
{
	TCHAR npcMaleName_csv[FILENAME_LENGTH];
	TCHAR npcFemaleName_csv[FILENAME_LENGTH];
	TCHAR npcPreName_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcMaleNameCsv"), 
		_T("\0"), npcMaleName_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcFemaleNameCsv"), 
		_T("\0"), npcFemaleName_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcPreNameCsv"), 
		_T("\0"), npcPreName_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);	

	if (m_npcManager == NULL)
		m_npcManager = SafeCreateObject(CNpcManager);

	m_npcManager->LoadRandomNameCSV(npcMaleName_csv, npcFemaleName_csv, npcPreName_csv);

}


UInt16 CServerResManager::GetNPCFstNameNum()
{

	if (m_npcManager)
		return m_npcManager->GetTotalNPCFstName();
	return 0;
    
}

UInt16 CServerResManager::GetNPCMaleSndNameNum()
{
	if (m_npcManager)
		return m_npcManager->GetTotalNPCMaleSndName();
	return 0;

}

UInt16 CServerResManager::GetNPCFemaleSndNameNum()
{
	if (m_npcManager)
		return m_npcManager->GetTotalNPCFemaleSndName();
	return 0;

}


String CServerResManager::GetNPCMaleName(UInt16 id)
{
	if (m_npcManager)
		return m_npcManager->GetMaleNPCSndName(id);
	String empty = _T("");
	return empty;

}

String CServerResManager::GetNPCFemaleName(UInt16 id)
{
	if (m_npcManager)
		return m_npcManager->GetFemaleNPCSndName(id);
	String empty = _T("");
	return empty;

}

String CServerResManager::GetNPCSurname(UInt16 id)
{
	if (m_npcManager)
		return m_npcManager->GetNPCFstName(id);
	String empty = _T("");
	return empty;

}


VOID CServerResManager::LoadNpcDAT()
{
	TCHAR npcpath_dat[FILENAME_LENGTH];
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportServerNPCTemplateFilename"), 
		_T("\0"), npcpath_dat, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if (m_npcManager == NULL)
		m_npcManager = SafeCreateObject(CNpcManager);

	m_npcManager->LoadDAT(npcpath_dat);
}

VOID CServerResManager::LoadLuaData()
{
	Char ScriptPath[_MAX_PATH] = _T("");
	GetPrivateProfileString(_T("Script"), _T("SrcPath"), _T("\0"), ScriptPath, _MAX_PATH, CONFIGURATION_INI_FILE);


	if (m_pScriptManager == NULL)
		m_pScriptManager = SafeCreateObject(CScriptManager);

	Int ReadType = GetPrivateProfileInt(_T("Script"), _T("ReadType"), 0, CONFIGURATION_INI_FILE);
	m_pScriptManager->LoadLuaFile(ReadType);
}

CONST char* CServerResManager::GetLuaData(CONST StringA &filetitle, size_t & len, BOOL bBatScript)
{
	if (m_pScriptManager)
		return m_pScriptManager->GetLua(filetitle, len, bBatScript);
	else
		return NULL;
}

const StartCharData* CServerResManager::GetStartCharData(UInt32 sexClass)
{
	if (m_startCharManager)
		return m_startCharManager->GetStartChar(sexClass);
	else
		return NULL;
}

void CServerResManager::LoadStartCharCSV()
{
	TCHAR startChar_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("StartCharCsv"), 
		_T("\0"), startChar_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if (m_startCharManager == NULL)
		m_startCharManager = SafeCreateObject(CStartCharManager);

	m_startCharManager->LoadCSV(startChar_csv);
}

CONST ShopLstData* CServerResManager::GetShopInfo(UInt32 shopId)
{
	if ( m_pShopLstManager )
		return m_pShopLstManager->GetShop(shopId);
	return NULL;
}

VOID CServerResManager::LoadShopCSV()
{
	TCHAR filename[FILENAME_LENGTH] = _T("");
	GetPrivateProfileString(_T("DataSourcePath"), _T("ShopListCsv"), _T("\0"), filename, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if (m_pShopLstManager == NULL)
		m_pShopLstManager = SafeCreateObject(CShopLstManager);

	m_pShopLstManager->LoadCSV(filename);
}

CONST CSVItemData* CServerResManager::GetCSVItemInfo(String szfilename)
{
	if ( m_pRandTableManager )
		return m_pRandTableManager->GetCSVItem(szfilename);
	return NULL;
}

CONST CSVMobData*  CServerResManager::GetCSVRandomMob(String szfilename)
{
	if ( m_pRandTableManager )
		return m_pRandTableManager->GetCSVMob(szfilename);
	return NULL;
}

CONST CSVNpcData* CServerResManager::GetCSVRandomNpc(String szfilename)
{
	if ( m_pRandTableManager )
		return m_pRandTableManager->GetCSVNpc(szfilename);
	return NULL;
}

CONST CSVQuestRow* CServerResManager::GetCSVRandomQuest(UInt32 Uid)
{
	if ( m_pRandTableManager )
		return m_pRandTableManager->GetCSVQuest(Uid);
	return NULL;
}

VOID CServerResManager::LoadCSVItemTable()
{
	if ( m_pRandTableManager == NULL )
		m_pRandTableManager = SafeCreateObject(CRandTableManager); 

	m_pRandTableManager->SearchItemFolder();
}

VOID CServerResManager::LoadCSVQuestTable()
{
	if ( m_pRandTableManager == NULL )
		m_pRandTableManager = SafeCreateObject(CRandTableManager); 

	TCHAR szPath[FILENAME_LENGTH] = _T("");
	GetPrivateProfileString(_T("DataSourcePath"), _T("QuestCsv"), 
		_T("\0"), szPath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	String filename;
	filename.Format(_T("%s"), szPath);
	m_pRandTableManager->LoadCSVQuest(filename);
}


VOID CServerResManager::LoadAllRandTable()
{
	if ( m_pRandTableManager == NULL )
		m_pRandTableManager = SafeCreateObject(CRandTableManager); 

	m_pRandTableManager->SearchFolder();

	LoadCSVQuestTable();
}

const BuffData* CServerResManager::GetBuffData(UInt16 buffid)
{
	if (m_BuffManager)
		return m_BuffManager->GetBuff(buffid);
	return NULL;
}

void CServerResManager::GetBuffList(BuffDataList &buffList)
{
	if (m_BuffManager)
		m_BuffManager->GetBuffList(buffList);
}

void CServerResManager::LoadBuffCSV()
{
	TCHAR buffpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("BuffCsv"), 
		_T("\0"), buffpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	if (m_BuffManager == NULL)
		m_BuffManager = SafeCreateObject(CBuffManager);
	
	// CSV Default Path
	m_BuffManager->LoadCSV(buffpath_csv);
	
}
void CServerResManager::LoadBuffDAT()
{
}

CONST MobSkillCombination* CServerResManager::GetMobSkillCombination(String szeFileName)
{
	if ( m_SkillCombinationManager )
		return m_SkillCombinationManager->GetSkillCombination(szeFileName);
	return NULL;
}

VOID CServerResManager::LoadSkillCombinationCSV()
{
	if ( m_SkillCombinationManager == NULL )
		m_SkillCombinationManager = SafeCreateObject(CSkillCombinationManager);

	
	m_SkillCombinationManager->SearchFolder();
}

const SkillShopData* CServerResManager::GetSkillShop(UInt32 skillShop_id)
{
	if (m_SkillShopManager)
		return m_SkillShopManager->GetSkillShop(skillShop_id);
	return NULL;
}
void CServerResManager::LoadSkillShopCSV()
{
	if ( m_SkillShopManager == NULL )
		m_SkillShopManager = SafeCreateObject(CSkillShopManager);

	TCHAR skillShoppath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("SkillShopCsv"), 
		_T("\0"), skillShoppath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

		// CSV Default Path
		m_SkillShopManager->LoadCSV(skillShoppath_csv);
		
}
void CServerResManager::LoadSkillShopDAT()
{
}

void CServerResManager::LoadBlockWordCSV()
{
	TCHAR BlockWordpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("BlockWordCsv"), 
		_T("\0"), BlockWordpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	//TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Finish GetPath..."));
	if (m_pBlockWordManager == NULL)
		m_pBlockWordManager = SafeCreateObject(CBlockWordManager);
	//TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Finish CreateObject..."));
	m_pBlockWordManager->LoadCSV(BlockWordpath_csv);
}

void CServerResManager::LoadChargeShopCSV()
{
	TCHAR ChargeShoppath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("ChargeShopCSV"), 
		_T("\0"), ChargeShoppath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_pChargeShopManager == NULL)
		m_pChargeShopManager = SafeCreateObject(CChargeShopManager);
	
	m_pChargeShopManager->LoadCSV(ChargeShoppath_csv);
}

const ChargeShopData* CServerResManager::GetChargeShopData(UInt32 index)
{
	if (m_pChargeShopManager)
		return m_pChargeShopManager->GetChargeShopItem(index);
	return NULL;

}

const BlockWordData* CServerResManager::GetBlockWordData(UInt32 index)
{
	if (m_pBlockWordManager)
		return m_pBlockWordManager->GetBlockWordData(index);
	return NULL;
}

VOID CServerResManager::GetBlockWordList(BlockWordDataList &blockWordList)
{
	if (m_pBlockWordManager)
		m_pBlockWordManager->GetBlockWordList(blockWordList);
}

VOID CServerResManager::LoadEventScheduleCSV()
{
	TCHAR espath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("EventScheduleCsv"), 
		_T("\0"), espath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	if (m_EventScheduleManager == NULL)
		m_EventScheduleManager = SafeCreateObject(CEventScheduleManager);
	
	// CSV Default Path
	m_EventScheduleManager->LoadCSV(espath_csv);

	m_EventScheduleManager->DebugPrint();
}

const EventScheduleData* CServerResManager::GetEventScheduleData(UInt32 eventId)
{
	if (m_EventScheduleManager)
		return m_EventScheduleManager->GetEventSchedule(eventId);
	return NULL;
}

BOOL CServerResManager::GetEventScheduleVector(EventScheduleMap& schedule)
{
	if (m_EventScheduleManager)
		return m_EventScheduleManager->GetEventScheduleVector(schedule);
	return FALSE;
}

BlockWordDataList* CServerResManager::GetMsgBlockWordList()
{
	if (m_pBlockWordManager)
		return m_pBlockWordManager->GetMsgBlockWordList();
	return NULL;
}

BlockWordDataList* CServerResManager::GetNameBlockWordList()
{
	if (m_pBlockWordManager)
		return m_pBlockWordManager->GetNameBlockWordList();
	return NULL;
}

ChargeShopDataList CServerResManager::GetChargeShopList()
{
	ChargeShopDataList temp;
	if (m_pChargeShopManager)
		return m_pChargeShopManager->GetChargeShopList();
	return temp;
}

UInt32 CServerResManager::GetChargeShopItemCount()
{
	if (m_pChargeShopManager)
		return m_pChargeShopManager->GetTotalItemCount();
	return 0;
}

void CServerResManager::LoadMixShopCSV()
{
	TCHAR mixShoppath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("MixShopCsv"), 
		_T("\0"), mixShoppath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_mixShopManager == NULL)
		m_mixShopManager = SafeCreateObject(CMixShopManager);
	
	m_mixShopManager->LoadMixShopCSV(mixShoppath_csv);
	

	TCHAR itemMixpath_csv[FILENAME_LENGTH];
	/*GetPrivateProfileString(_T("DataSourcePath"), _T("ItemMixWeaponCSV"), 
		_T("\0"), itemMixpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	m_mixShopManager->LoadItemMixCSV(itemMixpath_csv);
	

	GetPrivateProfileString(_T("DataSourcePath"), _T("ItemMixClothCSV "), 
		_T("\0"), itemMixpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	m_mixShopManager->LoadItemMixCSV(itemMixpath_csv);
	
	GetPrivateProfileString(_T("DataSourcePath"), _T("ItemMixOrnamentCSV "), 
		_T("\0"), itemMixpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	m_mixShopManager->LoadItemMixCSV(itemMixpath_csv);*/
	m_mixShopManager->LoadAllItemMixCSV();

	GetPrivateProfileString(_T("DataSourcePath"), _T("MixProbCSV"), 
		_T("\0"), itemMixpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	m_mixShopManager->LoadMixProbCSV(itemMixpath_csv);
}

const MixShopData* CServerResManager::GetMixShopData(UInt32 shopID)
{
	if (m_mixShopManager)
		return m_mixShopManager->GetMixShopData(shopID);
	return NULL;
}

const ItemMixData* CServerResManager::GetItemMixData(String filename, UInt32 productID)
{
	if (m_mixShopManager)
		return m_mixShopManager->GetItemMixData(filename, productID);
	return NULL;
}

void CServerResManager::GetStoneProbList(UInt32 stoneID, UInt8Vector& stoneProb)
{
	if (m_mixShopManager)
		m_mixShopManager->GetStoneProbList(stoneID, stoneProb);
}

UInt CServerResManager::GetMaxStoneMixNum(UInt32 stoneID)
{
	if (m_mixShopManager)
		return m_mixShopManager->GetMaxStoneMixNum(stoneID);
	return 0;
}

UInt CServerResManager::GetMinStoneMixNum(UInt32 stoneID)
{
	if (m_mixShopManager)
		return m_mixShopManager->GetMinStoneMixNum(stoneID);
	return 0;
}

UInt CServerResManager::GetStoneMixProb(UInt32 stoneID, UInt stack)
{
	if (m_mixShopManager)
		return m_mixShopManager->GetStoneMixProb(stoneID, stack);
	return 0;
}

UInt CServerResManager::GetResultStoneID(UInt32 stoneID)
{
	if (m_mixShopManager)
		return m_mixShopManager->GetResultStoneID(stoneID);
	return 0;
}

VOID CServerResManager::GetAntiHackInfo(UInt32 hackID, BYTE*& mapViewPtr, BYTE*& imagePtr, UInt32& imageSize)
{
	if (m_pHackManager)
		m_pHackManager->GetHackImageInfo(hackID, mapViewPtr, imagePtr, imageSize);
}


VOID CServerResManager::ReleaseHackImagePtr(BYTE* mapViewPtr)
{
	if (m_pHackManager)
		m_pHackManager->ReleaseHackImagePtr(mapViewPtr);
}

VOID CServerResManager::LoadAntihackDAT()
{
	if (m_pHackManager == NULL)
		m_pHackManager = SafeCreateObject(CAntiHackManager);
}

UInt32 CServerResManager::GetHackInfoSize()
{
	if (m_pHackManager)
		return m_pHackManager->GetHackDataSize();
	return 0;
}

VOID CServerResManager::LoadJournalCSV()
{
	TCHAR csvFile[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("JournalCsv"), 
		_T("\0"), csvFile, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if (m_pJournalManager == NULL)
		m_pJournalManager = SafeCreateObject(CJournalManager);
	m_pJournalManager->LoadCSV(csvFile);
}

CONST JournalData* CServerResManager::GetJournalData(UInt16 journal_id)
{
	if (m_pJournalManager)
		return m_pJournalManager->GetJournalData(journal_id);
	return NULL;
}

void CServerResManager::GetJournalList(JournalDataList &journalList)
{
	if (m_pJournalManager)
		m_pJournalManager->GetJournalList(journalList);
}

// Question Data Manager
const QuestionData* CServerResManager::GetQuestionData(UInt32 question_id)
{
	if (m_pQuestionManager)
		return m_pQuestionManager->GetQuestionData(question_id);
	return NULL;
}
void CServerResManager::LoadQuestionCSV()
{
	TCHAR quetionpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("QuestionCsv"), 
		_T("\0"), quetionpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (m_pQuestionManager == NULL)
		m_pQuestionManager = SafeCreateObject(CQuestionManager);

	m_pQuestionManager->LoadCSV(quetionpath_csv);
}
void CServerResManager::LoadQuestionDAT()
{
}
size_t CServerResManager::GetQuestionDataSize()
{
	return m_pQuestionManager->GetSize();
}

// PartnerCollection Data Manager
const PartnerCollectionData* CServerResManager::GetPartnerCollectionData(UInt32 collection_id)
{
	if (m_pPartnerCollectionManager)
		return m_pPartnerCollectionManager->GetPartnerCollectionData(collection_id);
	return NULL;
}

size_t CServerResManager::GetPartnerCollectionDataSize()
{
	return m_pPartnerCollectionManager->GetSize();
}

void CServerResManager::LoadPartnerCollectionCSV()
{
	TCHAR PartnerCollectionpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("PartnerCollectionCsv"), 
		_T("\0"), PartnerCollectionpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (m_pPartnerCollectionManager == NULL)
		m_pPartnerCollectionManager = SafeCreateObject(CPartnerCollectionManager);

	m_pPartnerCollectionManager->LoadCSV(PartnerCollectionpath_csv);
}

UInt16 CServerResManager::GetPartnerCollectionBuffID(UInt16 Count, UInt8 Type)
{
	if (m_pPartnerCollectionManager)
		return m_pPartnerCollectionManager->GetPartnerCollectionBuffID(Count, Type);
	return 0;
}

// ItemSet
const ItemSetData* CServerResManager::GetItemSetData(UInt32 itemset_id)
{
	if (m_pItemSetManager)
		return m_pItemSetManager->GetItemSetData(itemset_id);
	return NULL;
}

size_t CServerResManager::GetItemSetDataSize()
{
	if (m_pItemSetManager)
		return m_pItemSetManager->GetSize();
	return 0;
}

void CServerResManager::LoadItemSetCSV()
{
	TCHAR ItemSetPath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("ItemSetCsv"), 
		_T("\0"), ItemSetPath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (m_pItemSetManager == NULL)
		m_pItemSetManager = SafeCreateObject(CItemSetManager);

	m_pItemSetManager->LoadCSV(ItemSetPath_csv);
}

const LINEUPINFO* CServerResManager::GetLineupData(UInt32 uiLineupId)
{
	if (NULL != m_pLineupManager) {
		return m_pLineupManager->GetLineupData(uiLineupId);
	}
	return NULL;
}
size_t CServerResManager::GetLineupDataSize()
{
	if (NULL != m_pLineupManager)
		return m_pLineupManager->GetLineupDataSize();
	return 0;
}
void CServerResManager::LoadLineupCSV()
{
	TCHAR LinePath[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("LineupCsv"), 
		_T("\0"), LinePath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (NULL == m_pLineupManager)
		m_pLineupManager = SafeCreateObject(CLineupManager);

	m_pLineupManager->LoadCSV(LinePath);
}

void CServerResManager::LoadManorCSV()
{
	TCHAR ManorPath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("ManorCsv"), 
		_T("\0"), ManorPath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_manorManager == NULL)
		m_manorManager = SafeCreateObject(CManorManager);
	
	m_manorManager->LoadCSV(ManorPath_csv);
	
}

const ManorData* CServerResManager::GetManorData(UInt8 manor_scale)
{
	if (m_manorManager)
		return m_manorManager->GetManor(manor_scale); 
	return NULL;
}

// Business Shop Manager
const BusinessShopData* CServerResManager::GetBusinessShopData(UInt32 shop_id)
{
	if (m_pBusinessShopManager)
		return m_pBusinessShopManager->GetBusinessShopData(shop_id);
	return NULL;
}

BusinessShopDataMap* CServerResManager::GetBusinessShopDataMap()
{
	if (m_pBusinessShopManager)
		return m_pBusinessShopManager->GetBusinessShopDataMap();
	return NULL;
}

BOOL CServerResManager::LoadBusinessShopCSV()
{
	TCHAR BusinessShoppath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("BusinessShopCsv"), 
		_T("\0"), BusinessShoppath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (m_pBusinessShopManager == NULL)
		m_pBusinessShopManager = SafeCreateObject(CBusinessShopManager);

	m_pBusinessShopManager->LoadCSV(BusinessShoppath_csv);
	return TRUE;
}

const ACCSHOPITEM* CServerResManager::GetAccShopData(UInt32 uiIndex)
{
	if (NULL != m_pAccmulationShopManager) {
		return m_pAccmulationShopManager->GetAccmulationShopData(uiIndex);
	}
	return NULL;
}

const POINTSSHOPITEM* CServerResManager::GetPointsShopData(UInt32 uiItemID)
{
	if (m_pPointsShopManager)
		return m_pPointsShopManager->GetData(uiItemID);
	return NULL;
}

size_t CServerResManager::GetAccShopItemDataSize()
{
	if (NULL != m_pAccmulationShopManager)
		return m_pAccmulationShopManager->GetCAccmulationShopDataSize();
	return 0;
}

size_t CServerResManager::GetPointshopItemDataSize()
{
	if (m_pPointsShopManager)
		return m_pPointsShopManager->GetSize();

	return 0;
}

void CServerResManager::LoadAccShopItemCSV()
{
	TCHAR AccShopPath[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("AccShopItemCsv"), 
		_T("\0"), AccShopPath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (NULL == m_pAccmulationShopManager)
		m_pAccmulationShopManager = SafeCreateObject(CAccmulationShopManager);

	m_pAccmulationShopManager->LoadCSV(AccShopPath);
}

void CServerResManager::LoadPointsShopItemCSV()
{
	TCHAR PointsShopPath[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("PointsShopItemCsv"), 
		_T("\0"), PointsShopPath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (NULL == m_pPointsShopManager)
		m_pPointsShopManager = SafeCreateObject(CPointsShopManager);

	m_pPointsShopManager->LoadCSV(PointsShopPath);
}

const JUMPPOINT* CServerResManager::GetAutoPathData(UInt32 uSeq)
{
	if (NULL != m_pAutoPathManager) {
		return m_pAutoPathManager->GetAutoPathData(uSeq);
	}
	return NULL;
}
size_t CServerResManager::GetAutoPathDataSize()
{
	if (NULL != m_pAutoPathManager)
		return m_pAutoPathManager->GetAutoPathDataSize();
	return 0;
}

void CServerResManager::LoadAutoPathCSV()
{
	TCHAR szAutoPath[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("AutoPathCsv"), 
		_T("\0"), szAutoPath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (NULL == m_pAutoPathManager)
		m_pAutoPathManager = SafeCreateObject(CAutoPathManager);

	m_pAutoPathManager->LoadCSV(szAutoPath);
}

int CServerResManager::GetMapJumpPoints(UInt32 uMapId,UInt32Vector& uvPoints)
{
	if (NULL != m_pAutoPathManager)
		return m_pAutoPathManager->GetMapJumpPoints(uMapId,uvPoints);
	return 0;
}
