//-- Common
#include "Common.h"
//-- Self
#include "ClientResManager.h"
//-- Library
#include "GDFManager.h"
#include "FileManager.h"
#include "OBJBlockManager.h"
#include "ItemManager.h"
#include "SkillManager.h"
#include "MobManager.h"
#include "MapManager.h"
#include "NpcManager.h"
#include "StartCharManager.h"
#include "BuffManager.h"
#include "SkillShopManager.h"
#include "BigMapManager.h"
#include "MobDescManager.h"
#include "Common/common_binary.h"
#include "BlockWordManager.h"
#include "NewBieManager.h"
#include "PetManager.h"
#include "WeaponsRankData.h"
#include "NewBieTipsManager.h"
#include "SkillCombinationManager.h"
#include "MixShopManager.h"
#include "TitleManager.h"
#include "FactionManager.h"
#include "PartnerCollectionManager.h"
#include "ItemSetManager.h"
#include "Lineup.h"
#include "ManorManager.h"
#include "AccmulationShop.h"
#include "PointsShopManager.h"
#include "AutoPath.h"
#include "NewGuideManager.h"

#define CONFIGURATION_INI_FILE		_T(".\\Config\\Setting.ini")
#define FILENAME_LENGTH				1024

CClientResManager::CClientResManager()
{
	m_fileManager = SafeCreateObject(CFileManager);
	m_objBlockManager = SafeCreateObject(COBJBlockManager);
	m_itemManager = SafeCreateObject(CItemManager);
	m_skillManager = SafeCreateObject(CSkillManager);
	m_mobManager = SafeCreateObject(CMobManager);
	m_mapManager = SafeCreateObject(CMapManager);
	m_npcManager = SafeCreateObject(CNpcManager);
	m_StartCharManager = SafeCreateObject(CStartCharManager);
	m_BuffManager = SafeCreateObject(CBuffManager);
	m_SkillShopManager = SafeCreateObject(CSkillShopManager);
	m_BigMapManager = SafeCreateObject(CBigMapManager);
	m_pBlockWordManager = SafeCreateObject(CBlockWordManager);
	m_mobDescManager = SafeCreateObject(CMobDescManager);
	m_newBieManager = SafeCreateObject(CNewBieManager);
	m_petManager = SafeCreateObject(CPetManager);
	m_pWeaponsRankData = SafeCreateObject(CWeaponsRankData);
	m_newBieTipsManager = SafeCreateObject(CNewbieTipsManager);
	m_newGuideManager = SafeCreateObject(CNewGuideManager);
	m_SkillCombinationManager = SafeCreateObject(CSkillCombinationManager);
	m_mixShopManager = SafeCreateObject(CMixShopManager);
	m_TitleManager = SafeCreateObject(CTitleManager);
	m_FactionManager = SafeCreateObject(CFactionManager);
	m_pPartnerCollectionManager = SafeCreateObject(CPartnerCollectionManager);
	m_pItemSetManager = SafeCreateObject(CItemSetManager);
	m_GdfResource = NULL;
	m_GdfResourceEx = NULL;
	m_pLineupManager = NULL;
	m_manorManager = NULL;		
	m_pAccShopManager = NULL;
	m_pPointsShopManager = NULL;
	m_pAutoPathManager = SafeCreateObject(CAutoPathManager);
}

CClientResManager::CClientResManager(GDFManager* mgr, GDFManager* mgr2)
{
	m_fileManager = SafeCreateObject(CFileManager);
	m_objBlockManager = SafeCreateObject(COBJBlockManager);
	m_itemManager = SafeCreateObject(CItemManager);
	m_skillManager = SafeCreateObject(CSkillManager);
	m_mobManager = SafeCreateObject(CMobManager);
	m_mapManager = SafeCreateObject(CMapManager);
	m_npcManager = SafeCreateObject(CNpcManager);
	m_StartCharManager = SafeCreateObject(CStartCharManager);
	m_BuffManager = SafeCreateObject(CBuffManager);
	m_SkillShopManager = SafeCreateObject(CSkillShopManager);
	m_BigMapManager = SafeCreateObject(CBigMapManager);
	m_pBlockWordManager = SafeCreateObject(CBlockWordManager);
	m_mobDescManager = SafeCreateObject(CMobDescManager);
	m_newBieManager = SafeCreateObject(CNewBieManager);
	m_petManager = SafeCreateObject(CPetManager);
	m_pWeaponsRankData = SafeCreateObject(CWeaponsRankData);
	m_newBieTipsManager = SafeCreateObject(CNewbieTipsManager);
	m_newGuideManager = SafeCreateObject(CNewGuideManager);
	m_SkillCombinationManager = SafeCreateObject(CSkillCombinationManager);
	m_mixShopManager = SafeCreateObject(CMixShopManager);
	m_TitleManager = SafeCreateObject(CTitleManager);
	m_FactionManager = SafeCreateObject(CFactionManager);
	m_pPartnerCollectionManager = SafeCreateObject(CPartnerCollectionManager);
	m_pItemSetManager = SafeCreateObject(CItemSetManager);
	m_GdfResource = mgr;
	m_GdfResourceEx = mgr2;
	m_pLineupManager=NULL;
	m_manorManager = NULL;
	m_pAccShopManager = NULL;
	m_pPointsShopManager = NULL;
	m_pAutoPathManager = SafeCreateObject(CAutoPathManager);
}

CClientResManager::~CClientResManager()
{
	SafeDeleteObject(m_fileManager);
	SafeDeleteObject(m_objBlockManager);
	SafeDeleteObject(m_itemManager);
	SafeDeleteObject(m_skillManager);
	SafeDeleteObject(m_mobManager);
	SafeDeleteObject(m_mapManager);
	SafeDeleteObject(m_npcManager);
	SafeDeleteObject(m_StartCharManager);
	SafeDeleteObject(m_BuffManager);
	SafeDeleteObject(m_SkillShopManager);
	SafeDeleteObject(m_BigMapManager);
	SafeDeleteObject(m_pBlockWordManager);
	SafeDeleteObject(m_mobDescManager);
	SafeDeleteObject(m_newBieManager);
	SafeDeleteObject(m_petManager);
	SafeDeleteObject(m_pWeaponsRankData);
	SafeDeleteObject(m_newBieTipsManager);
	SafeDeleteObject(m_newGuideManager);
	SafeDeleteObject(m_SkillCombinationManager);
	SafeDeleteObject(m_mixShopManager);
	SafeDeleteObject(m_TitleManager);
	SafeDeleteObject(m_FactionManager);
	SafeDeleteObject(m_pPartnerCollectionManager);
	SafeDeleteObject(m_pItemSetManager);
	SafeDeleteObject(m_pLineupManager);
	SafeDeleteObject(m_manorManager);
	SafeDeleteObject(m_pAccShopManager);
	SafeDeleteObject(m_pPointsShopManager);
	SafeDeleteObject(m_pAutoPathManager);
}

BYTE* CClientResManager::LoadFileData(const TCHAR *filename, UInt64 & size)
{
	return m_fileManager->Load(filename, size);
}

BOOL CClientResManager::FreeFileData(const TCHAR *filename)
{
	return m_fileManager->Free(filename);
}

BlockPosVector* CClientResManager::LoadOBJBlocking(const TCHAR *filename, Int state)
{
	return m_objBlockManager->Load(filename, state);
}

BlockPosVector* CClientResManager::LoadOBJBlocking_FromEntFile(const TCHAR *filename, Int state)
{
	return m_objBlockManager->LoadEnt(filename, state);
}

BOOL CClientResManager::FreeOBJBlocking(const TCHAR *filename)
{
	return m_objBlockManager->Free(filename);
}

const ItemData* CClientResManager::GetItemData(UInt32 itemid)
{
	return m_itemManager->GetItem(itemid);
}

void CClientResManager::GetItemList(ItemDataList &itemList, UInt8 filterType, UInt8 item_type)
{
	if (m_itemManager)
		m_itemManager->GetItemList(itemList, filterType, item_type);
}

const SkillData* CClientResManager::GetSkillData(UInt16 skillid)
{
	return m_skillManager->GetSkill(skillid);
}

void CClientResManager::GetSkillList(SkillDataList &skillList)
{
	if (m_skillManager)
		m_skillManager->GetSkillList(skillList);
}

void CClientResManager::GetSkillLink(UInt16 skill_id, SkillDataList &skillList)
{
	if (m_skillManager)
		m_skillManager->GetSkillLink(skill_id, skillList);
}

void CClientResManager::GetSkillLink(UInt16 skill_id, UInt16Vector &skillIds)
{
	if (m_skillManager)
		m_skillManager->GetSkillLink(skill_id, skillIds);
}

const MobData* CClientResManager::GetMobData(UInt32 mob_id)
{
	return m_mobManager->GetMob(mob_id);
}

const MapDB* CClientResManager::GetMapData(UInt32 mapid)
{
	return m_mapManager->GetMap(mapid);
}

BOOL CClientResManager::ActivateMap(UInt32 mapid, PathFindLoadData& pPathFindData, UInt32& pNumGuide)
{
	TCHAR dir[FILENAME_LENGTH];
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportServerZoneDataDirectory"), 
		_T("\0"), dir, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	return m_mapManager->LoadZoneDAT(mapid, dir) && m_mapManager->LoadPFDAT(mapid, dir, pPathFindData, pNumGuide);
}

const NpcData* CClientResManager::GetNpcData(UInt32 npcid)
{
	return m_npcManager->GetNpc(npcid);
}

void CClientResManager::LoadItemCSV()
{
	TCHAR itempath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("ItemCsv"), 
		_T("\0"), itempath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = itempath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_itemManager->LoadCSV(itempath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		// CSV Default Path
		m_itemManager->LoadCSV(itempath_csv);
	}	
}

void CClientResManager::LoadSkillCSV()
{
	TCHAR skillpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("SkillCsv"), 
		_T("\0"), skillpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = skillpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_skillManager->LoadCSV(skillpath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		// CSV Default Path
		m_skillManager->LoadCSV(skillpath_csv);
	}	
}

void CClientResManager::LoadStartCharCSV()
{
	TCHAR starCharPath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("StartCharCsv"), 
		_T("\0"), starCharPath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = starCharPath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_StartCharManager->LoadCSV(starCharPath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_StartCharManager->LoadCSV(starCharPath_csv);
	}
}

const StartCharData* CClientResManager::GetStartChar(UInt32 sexClass)
{
	return m_StartCharManager->GetStartChar(sexClass);
}

void CClientResManager::LoadBigMapCSV()
{
	TCHAR bigMapPath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("BigMapCsv"), 
		_T("\0"), bigMapPath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = bigMapPath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_BigMapManager->LoadCSV(bigMapPath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_BigMapManager->LoadCSV(bigMapPath_csv);
	}

	GetPrivateProfileString(_T("DataSourcePath"), _T("BigMapBelongCsv"), 
		_T("\0"), bigMapPath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = bigMapPath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_BigMapManager->loadBelongCSV(bigMapPath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_BigMapManager->loadBelongCSV(bigMapPath_csv);
	}

}

const BigMapData* CClientResManager::GetBigMap(UInt16 map_id)
{
	return m_BigMapManager->GetBigMap(map_id);
}

const BigMapBelongData* CClientResManager::GetBigMapBelong(UInt16 map_id)
{
	return m_BigMapManager->GetBigMapBelong(map_id);
}

const BigMapData* CClientResManager::GetBigMapByIndex(UInt32 index)
{
	return m_BigMapManager->GetBigMapByIndex(index);
}

UInt32 CClientResManager::GetMapCount()
{
	return m_BigMapManager->GetMapCount();
}

void CClientResManager::LoadMobDescCSV()
{
	TCHAR MobDescpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("MobDescCsv"), 
		_T("\0"), MobDescpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = MobDescpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_mobDescManager->LoadCSV(MobDescpath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_mobDescManager->LoadCSV(MobDescpath_csv);
	}
}

String CClientResManager::GetMobDesc(const UInt32& ref_Id)
{
	if  (m_mobDescManager)
		return m_mobDescManager->GetDesc(ref_Id);
	return String(_T(""));
}

void CClientResManager::LoadItemDAT()
{
//	TCHAR itempath[FILENAME_LENGTH];
//	GetPrivateProfileString(_T("DataSourcePath"), _T("PATH_ITEMDATA_FILE"), 
//		_T("\0"), itempath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
//	m_itemManager->LoadDAT(itempath);
}

void CClientResManager::LoadSkillDAT()
{
//	TCHAR skillpath[FILENAME_LENGTH];
//	GetPrivateProfileString(_T("DataSourcePath"), _T("PATH_SKILLDATA_FILE"), 
//		_T("\0"), skillpath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
//	m_skillManager->LoadDAT(skillpath);
}

void CClientResManager::LoadMobCSV()
{
	TCHAR Mobpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("MobCsv"), 
		_T("\0"), Mobpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	m_mobManager->LoadCSV(Mobpath_csv);
}

void CClientResManager::LoadMobDAT()
{
	TCHAR Mobpath[FILENAME_LENGTH];
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportServerMOBInfoFilename"), 
		_T("\0"), Mobpath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	m_mobManager->LoadDAT(Mobpath);
}



VOID CClientResManager::LoadMapDAT()
{
	TCHAR mappath_dat[FILENAME_LENGTH];
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportServerMapInfoFilename"), 
		_T("\0"), mappath_dat, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	m_mapManager->LoadDAT(mappath_dat);
	m_mapManager->SetNpcManager(m_npcManager);
}

VOID CClientResManager::LoadClientMapDAT()
{
	TCHAR path[FILENAME_LENGTH] = _T("");
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportClientMapInfoFilename"), 
		_T("\0"), path, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = path;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_mapManager->LoadClientMapInfo(path, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		// CSV Default Path
		m_mapManager->LoadClientMapInfo(path);
	}	
}

const BinaryClientMapInfo* CClientResManager::GetCliMapData(UInt32 mapid)
{
	if ( m_mapManager )
		return m_mapManager->GetClientMap(mapid);
	return NULL;
}

void CClientResManager::LoadNpcCSV()
{
	TCHAR npc_csv[FILENAME_LENGTH];
	TCHAR npcMob_csv[FILENAME_LENGTH];
	TCHAR npcDrop_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcCsv"), 
		_T("\0"), npc_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcMobCsv"), 
		_T("\0"), npcMob_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	GetPrivateProfileString(_T("DataSourcePath"), _T("NpcDropCsv"), 
		_T("\0"), npcDrop_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	m_npcManager->LoadCSV(npc_csv, npcMob_csv, npcDrop_csv);
}

VOID CClientResManager::LoadNpcDAT()
{
	TCHAR npcpath_dat[FILENAME_LENGTH];
	GetPrivateProfileString(_T("ExportFileConfiguration"), _T("ExportServerNPCTemplateFilename"), 
		_T("\0"), npcpath_dat, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	m_npcManager->LoadDAT(npcpath_dat);
}

const BuffData* CClientResManager::GetBuffData(UInt32 buffid)
{
	if (m_BuffManager)
		return m_BuffManager->GetBuff(buffid);
	return NULL;
}

void CClientResManager::GetBuffList(BuffDataList &buffList)
{
	if (m_BuffManager)
		m_BuffManager->GetBuffList(buffList);
}

void CClientResManager::LoadBuffCSV()
{
	TCHAR buffpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("BuffCsv"), 
		_T("\0"), buffpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = buffpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_BuffManager->LoadCSV(buffpath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		// CSV Default Path
		m_BuffManager->LoadCSV(buffpath_csv);
	}	
}
void CClientResManager::LoadBuffDAT()
{
}

//SkillShop Manager
const SkillShopData* CClientResManager::GetSkillShop(UInt32 skillShop_id)
{
	if (m_SkillShopManager)
		return m_SkillShopManager->GetSkillShop(skillShop_id);
	return NULL;
}
void CClientResManager::LoadSkillShopCSV()
{
	TCHAR skillShoppath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("SkillShopCsv"), 
		_T("\0"), skillShoppath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = skillShoppath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_SkillShopManager->LoadCSV(skillShoppath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		// CSV Default Path
		m_SkillShopManager->LoadCSV(skillShoppath_csv);
	}	
}
void CClientResManager::LoadSkillShopDAT()
{
}

//SkillShop Manager
const NewbieData* CClientResManager::GetNewBieData(UInt16 newbie_id)
{
	if (m_newBieManager)
		return m_newBieManager->GetNewBieData(newbie_id);
	return NULL;
}
void CClientResManager::LoadNewBieCSV()
{
	TCHAR newBiepath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("NewBieCsv"), 
		_T("\0"), newBiepath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = newBiepath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_newBieManager->LoadCSV(newBiepath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		// CSV Default Path
		m_newBieManager->LoadCSV(newBiepath_csv);
	}	
}

const NewbieTipsData* CClientResManager::GetNewBieTipsData(UInt16 newbietips_id)
{
	if (m_newBieTipsManager)
		return m_newBieTipsManager->GetNewbieTipsData(newbietips_id);
	return NULL;
}
void CClientResManager::LoadNewBieTipsCSV()
{
	TCHAR newBiepath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("NewBieTipsCsv"), 
		_T("\0"), newBiepath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = newBiepath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_newBieTipsManager->LoadCSV(newBiepath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		// CSV Default Path
		m_newBieTipsManager->LoadCSV(newBiepath_csv);
	}	
}

const NewGuideData* CClientResManager::GetNewGuideData(UInt16 newGuide_id)
{
	if (m_newGuideManager)
		return m_newGuideManager->GetNewGuideData(newGuide_id);
	return NULL;
}
void CClientResManager::LoadNewGuideCSV()
{
	TCHAR newGuidePath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("NewGuideCsv"), 
		_T("\0"), newGuidePath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = newGuidePath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_newGuideManager->LoadCSV(newGuidePath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		// CSV Default Path
		m_newGuideManager->LoadCSV(newGuidePath_csv);
	}	
}

void CClientResManager::LoadBlockWordCSV()
{
	TCHAR BlockWordpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("BlockWordCsv"), 
		_T("\0"), BlockWordpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_pBlockWordManager == NULL)
		m_pBlockWordManager = SafeCreateObject(CBlockWordManager);
	
	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = BlockWordpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_pBlockWordManager->LoadCSV(BlockWordpath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_pBlockWordManager->LoadCSV(BlockWordpath_csv);
	}
}

const FactionData* CClientResManager::GetFactionData(UInt32 faction_id)
{
	if (m_FactionManager)
		return m_FactionManager->GetFaction(faction_id);
	return NULL;
}

void CClientResManager::LoadFactionsCSV()
{
	TCHAR Factionpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("FactionCsv"), 
		_T("\0"), Factionpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_FactionManager == NULL)
		m_FactionManager = SafeCreateObject(CFactionManager);
	
	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = Factionpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_FactionManager->LoadCSV(Factionpath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_FactionManager->LoadCSV(Factionpath_csv);
	}
}
//Title Manager
const TitleData* CClientResManager::GetTitleData(UInt32 title_id)
{
	if (m_TitleManager)
		return m_TitleManager->GetTitle(title_id);

	return NULL;
}
const TitleData* CClientResManager::GetTitleDataByRank(UInt8	Title_RankType, UInt8	Title_RankSubType, 
													   UInt8	Title_StartRank, UInt8	Title_EndRank)
{
	if (m_TitleManager)
		return m_TitleManager->GetTitleByRank(Title_RankType, Title_RankSubType, 
										Title_StartRank, Title_EndRank);
	return NULL;
}
void CClientResManager::LoadTitleCSV()
{
	TCHAR Titlepath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("TitleCsv"), 
		_T("\0"), Titlepath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_TitleManager == NULL)
		m_TitleManager = SafeCreateObject(CTitleManager);
	
	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = Titlepath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_TitleManager->LoadCSV(Titlepath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_TitleManager->LoadCSV(Titlepath_csv);
	}
}

void CClientResManager::LoadPetCSV()
{
	TCHAR Petpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("PetCsv"), 
		_T("\0"), Petpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_petManager == NULL)
		m_petManager = SafeCreateObject(CPetManager);
	
	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = Petpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_petManager->LoadCSV(Petpath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_petManager->LoadCSV(Petpath_csv);
	}
}

void CClientResManager::LoadWeaponsRankDataCSV()
{
	TCHAR Petpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("WeaponsRankDataCsv"), 
		_T("\0"), Petpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_pWeaponsRankData == NULL)
		m_pWeaponsRankData = SafeCreateObject(CWeaponsRankData);
	
	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = Petpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_pWeaponsRankData->LoadCSV(Petpath_csv , buf , fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_pWeaponsRankData->LoadCSV(Petpath_csv);
	}
}

void CClientResManager::LoadMixShopCSV()
{
	TCHAR mixShoppath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("MixShopCsv"), 
		_T("\0"), mixShoppath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_mixShopManager == NULL)
		m_mixShopManager = SafeCreateObject(CMixShopManager);
	
	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = mixShoppath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_mixShopManager->LoadMixShopCSV(mixShoppath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_mixShopManager->LoadMixShopCSV(mixShoppath_csv);
	}

	TCHAR itemMixpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("CSVSrcPath"), 
		_T(".\\Data\\CSV\\"), itemMixpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	StringVector itemMixFileList;
	m_mixShopManager->GetAllItemMixFileName(itemMixFileList);
	for (Index i = 0; i < itemMixFileList.size(); ++i)
	{
		if( m_GdfResource || m_GdfResourceEx )
		{
			// CSV Default Path
			String csvfilename = itemMixpath_csv;
			csvfilename.Format(_T("%s%s"), csvfilename.c_str(), itemMixFileList[i].c_str());
			UInt32 beginOffset, fileSize;
			BYTE* fbuf = NULL;
			if ( m_GdfResourceEx )
				fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
			if ( (!fbuf) && m_GdfResource )
				fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
			if( fbuf )
			{
				BYTE* buf = fbuf + beginOffset;
				m_mixShopManager->LoadItemMixCSV(itemMixFileList[i].c_str(), buf, fileSize);
				UnmapViewOfFile(fbuf);
			}
		}
		else
		{
			m_mixShopManager->LoadItemMixCSV(itemMixFileList[i].c_str());
		}
	}

	
	/*GetPrivateProfileString(_T("DataSourcePath"), _T("CSVSrcPath"), 
		_T(".\\Data\\CSV\\"), itemMixpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if( m_GdfResource )
	{
		// CSV Default Path
		String csvfilename = itemMixpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_mixShopManager->LoadItemMixCSV(itemMixpath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_mixShopManager->LoadItemMixCSV(itemMixpath_csv);
	}

	GetPrivateProfileString(_T("DataSourcePath"), _T("ItemMixClothCSV "), 
		_T("\0"), itemMixpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if( m_GdfResource )
	{
		// CSV Default Path
		String csvfilename = itemMixpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_mixShopManager->LoadItemMixCSV(itemMixpath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_mixShopManager->LoadItemMixCSV(itemMixpath_csv);
	}

	GetPrivateProfileString(_T("DataSourcePath"), _T("ItemMixOrnamentCSV "), 
		_T("\0"), itemMixpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if( m_GdfResource )
	{
		// CSV Default Path
		String csvfilename = itemMixpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_mixShopManager->LoadItemMixCSV(itemMixpath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_mixShopManager->LoadItemMixCSV(itemMixpath_csv);
	}*/

	GetPrivateProfileString(_T("DataSourcePath"), _T("MixProbCSV"), 
		_T("\0"), itemMixpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = itemMixpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_mixShopManager->LoadMixProbCSV(itemMixpath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_mixShopManager->LoadMixProbCSV(itemMixpath_csv);
	}
}

const MixShopData* CClientResManager::GetMixShopData(UInt32 shopID)
{
	if (m_mixShopManager)
		return m_mixShopManager->GetMixShopData(shopID);
	return NULL;
}

void CClientResManager::GetSuitableItemList(String filename, UInt16 skillLv_low, UInt16 skillLv_up, ItemMixDataList &itemList)
{
	if (m_mixShopManager)
		m_mixShopManager->GetSuitableItemList(filename, skillLv_low, skillLv_up, itemList);
}

const PetData* CClientResManager::GetPetData(UInt32 pet_id)
{
	if (m_petManager)
		return m_petManager->GetPet(pet_id);
	return NULL;
}
const WeaponsRankInfo * CClientResManager::GetWeaponsRankData(UInt32 weaponsrank_id)
{
	if (m_pWeaponsRankData)
		return m_pWeaponsRankData->GetWeaponsRankData(weaponsrank_id);
	return NULL;
}

const BlockWordData* CClientResManager::GetBlockWordData(UInt32 index)
{
	if (m_pBlockWordManager)
		return m_pBlockWordManager->GetBlockWordData(index);
	return NULL;
}

VOID CClientResManager::GetBlockWordList(BlockWordDataList &blockWordList)
{
	if (m_pBlockWordManager)
		m_pBlockWordManager->GetBlockWordList(blockWordList);
}



BlockWordDataList* CClientResManager::GetMsgBlockWordList()
{
	if (m_pBlockWordManager)
		return m_pBlockWordManager->GetMsgBlockWordList();

	return NULL;
}

BlockWordDataList* CClientResManager::GetNameBlockWordList()
{
	if (m_pBlockWordManager)
		return m_pBlockWordManager->GetNameBlockWordList();
	return NULL;
}

void CClientResManager::GetMobList(MobDataList &mobList)
{
	if (m_mobManager)
		m_mobManager->GetMobList(mobList);
}

CONST MobSkillCombination* CClientResManager::GetMobSkillCombination(String szeFileName)
{
	if ( m_SkillCombinationManager )
		return m_SkillCombinationManager->GetSkillCombination(szeFileName);
	return NULL;
}

VOID CClientResManager::LoadSkillCombinationCSV()
{
	if ( m_SkillCombinationManager == NULL )
		m_SkillCombinationManager = SafeCreateObject(CSkillCombinationManager);

	m_SkillCombinationManager->SearchClientFolder(this);
}

void CClientResManager::GetStoneProbList(UInt32 stoneID, UInt8Vector& stoneProb)
{
	if (m_mixShopManager)
		m_mixShopManager->GetStoneProbList(stoneID, stoneProb);
}

UInt CClientResManager::GetMaxStoneMixNum(UInt32 stoneID)
{
	if (m_mixShopManager)
		return m_mixShopManager->GetMaxStoneMixNum(stoneID);
	return 0;
}

UInt CClientResManager::GetMinStoneMixNum(UInt32 stoneID)
{
	if (m_mixShopManager)
		return m_mixShopManager->GetMinStoneMixNum(stoneID);
	return 0;
}

UInt CClientResManager::GetStoneMixProb(UInt32 stoneID, UInt stack)
{
	if (m_mixShopManager)
		return m_mixShopManager->GetStoneMixProb(stoneID, stack);
	return 0;
}

UInt CClientResManager::GetResultStoneID(UInt32 stoneID)
{
	if (m_mixShopManager)
		return m_mixShopManager->GetResultStoneID(stoneID);
	return 0;
}
// PartnerCollection Data Manager

void CClientResManager::GetPartnerCollectionData(PartnerCollectionDataMap &PartnerCollectionList, UInt8 Type)
{
	if (m_pPartnerCollectionManager)
		m_pPartnerCollectionManager->GetPartnerCollectionData(PartnerCollectionList, Type);
}
void CClientResManager::GetPartnerCollectionData(PartnerCollectionDataMap &PartnerCollectionList, UInt8 Type, UInt8 Group)
{
	if (m_pPartnerCollectionManager)
		m_pPartnerCollectionManager->GetPartnerCollectionData(PartnerCollectionList, Type, Group);
}
const PartnerCollectionData* CClientResManager::GetPartnerCollectionData(UInt32 collection_id)
{
	if (m_pPartnerCollectionManager)
		return m_pPartnerCollectionManager->GetPartnerCollectionData(collection_id);
	return NULL;
}

size_t CClientResManager::GetPartnerCollectionDataSize()
{
	return m_pPartnerCollectionManager->GetSize();
}

void CClientResManager::LoadPartnerCollectionCSV()
{
	TCHAR PartnerCollectionpath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("PartnerCollectionCsv"), 
		_T("\0"), PartnerCollectionpath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (m_pPartnerCollectionManager == NULL)
		m_pPartnerCollectionManager = SafeCreateObject(CPartnerCollectionManager);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = PartnerCollectionpath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_pPartnerCollectionManager->LoadCSV(PartnerCollectionpath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_pPartnerCollectionManager->LoadCSV(PartnerCollectionpath_csv);
	}
}

// ItemSet
const ItemSetData* CClientResManager::GetItemSetData(UInt32 itemset_id)
{
	if (m_pItemSetManager)
		return m_pItemSetManager->GetItemSetData(itemset_id);
	return NULL;
}

size_t CClientResManager::GetItemSetDataSize()
{
	if (m_pItemSetManager)
		return m_pItemSetManager->GetSize();
	return 0;
}

void CClientResManager::LoadItemSetCSV()
{
	TCHAR ItemSetPath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("ItemSetCsv"), 
		_T("\0"), ItemSetPath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (m_pItemSetManager == NULL)
		m_pItemSetManager = SafeCreateObject(CItemSetManager);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = ItemSetPath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_pItemSetManager->LoadCSV(ItemSetPath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_pItemSetManager->LoadCSV(ItemSetPath_csv);
	}
}

const LINEUPINFO* CClientResManager::GetLineupData(UInt32 uiLineupId)
{
	if (NULL != m_pLineupManager) {
		return m_pLineupManager->GetLineupData(uiLineupId);
	}
	return NULL;
}
size_t CClientResManager::GetLineupDataSize()
{
	if (NULL != m_pLineupManager)
		return m_pLineupManager->GetLineupDataSize();
	return 0;
}

void CClientResManager::LoadLineupCSV()
{
	TCHAR LinePath[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("LineupCsv"), 
		_T("\0"), LinePath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (NULL == m_pLineupManager)
		m_pLineupManager = SafeCreateObject(CLineupManager);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = LinePath;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_pLineupManager->LoadCSV(LinePath, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_pLineupManager->LoadCSV(LinePath);
	}
}

void CClientResManager::LoadManorCSV()
{
	TCHAR ManorPath_csv[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("ManorCsv"), 
		_T("\0"), ManorPath_csv, FILENAME_LENGTH, CONFIGURATION_INI_FILE);
	
	if (m_manorManager == NULL)
		m_manorManager = SafeCreateObject(CManorManager);
	
	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = ManorPath_csv;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_manorManager->LoadCSV(ManorPath_csv, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
        m_manorManager->LoadCSV(ManorPath_csv);
	}
	
}

const ManorData* CClientResManager::GetManorData(UInt8 manor_scale)
{
	if (m_manorManager)
		return m_manorManager->GetManor(manor_scale); 
	return NULL;
}

const ACCSHOPITEM* CClientResManager::GetAccShopData(UInt32 uiIndex)
{
	if (m_pAccShopManager)
		return m_pAccShopManager->GetAccmulationShopData(uiIndex);
	return NULL;
}

const POINTSSHOPITEM* CClientResManager::GetPointsShopData(UInt32 uiItemID)
{
	if (m_pPointsShopManager)
		return m_pPointsShopManager->GetData(uiItemID);
	return NULL;
}

size_t CClientResManager::GetAccShopItemDataSize()
{
	if (NULL != m_pAccShopManager)
		return m_pAccShopManager->GetCAccmulationShopDataSize();
	return 0;
}

size_t CClientResManager::GetPointsShopItemDataSize()
{
	if (NULL != m_pPointsShopManager)
		return m_pPointsShopManager->GetSize();
	return 0;
}

void CClientResManager::LoadPointsShopItemCSV()
{
	TCHAR PointsShopPath[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("PointsShopItemCsv"), 
		_T("\0"), PointsShopPath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (NULL == m_pPointsShopManager)
		m_pPointsShopManager = SafeCreateObject(CPointsShopManager);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = PointsShopPath;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_pPointsShopManager->LoadCSV(PointsShopPath, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_pPointsShopManager->LoadCSV(PointsShopPath);
	}
}

void CClientResManager::LoadAccShopItemCSV()
{
	TCHAR AccShopPath[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("AccShopItemCsv"), 
		_T("\0"), AccShopPath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (NULL == m_pAccShopManager)
		m_pAccShopManager = SafeCreateObject(CAccmulationShopManager);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = AccShopPath;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_pAccShopManager->LoadCSV(AccShopPath, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_pAccShopManager->LoadCSV(AccShopPath);
	}
}

const JUMPPOINT* CClientResManager::GetAutoPathData(UInt32 uSeq)
{
	if (NULL != m_pAutoPathManager) {
		return m_pAutoPathManager->GetAutoPathData(uSeq);
	}
	return NULL;
}
size_t CClientResManager::GetAutoPathDataSize()
{
	if (NULL != m_pAutoPathManager)
		return m_pAutoPathManager->GetAutoPathDataSize();
	return 0;
}

void CClientResManager::LoadAutoPathCSV()
{
	TCHAR szAutoPath[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("AutoPathCsv"), 
		_T("\0"), szAutoPath, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	// CSV Default Path
	if (NULL == m_pAutoPathManager)
		m_pAutoPathManager = SafeCreateObject(CAutoPathManager);

	if( m_GdfResource || m_GdfResourceEx )
	{
		// CSV Default Path
		String csvfilename = szAutoPath;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( m_GdfResourceEx )
			fbuf = m_GdfResourceEx->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && m_GdfResource )
			fbuf = m_GdfResource->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			m_pAutoPathManager->LoadCSV(szAutoPath, buf, fileSize);
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		m_pAutoPathManager->LoadCSV(szAutoPath);
	}
}

int CClientResManager::GetMapJumpPoints(UInt32 uMapId,UInt32Vector& uvPoints)
{
	if (NULL != m_pAutoPathManager)
		return m_pAutoPathManager->GetMapJumpPoints(uMapId,uvPoints);
	return 0;
}
