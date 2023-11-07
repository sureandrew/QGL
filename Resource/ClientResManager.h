#pragma once
#ifndef _CLIENTRESMANAGER_H_
#define _CLIENTRESMANAGER_H_

#include <vector>

struct ItemData;
struct SkillData;
struct MobData;
struct MapDB;
struct NpcData;
struct NPCDB;
struct StartCharData;
struct BuffData;
struct SkillShopData;
struct BigMapData;
struct BigMapBelongData;
struct NewbieData;
struct PetData;
struct WeaponsRankInfo;
struct NewbieTipsData;
struct NewGuideData;
struct MobSkillCombination;
struct MixShopData;
struct ItemMixShop;
struct FactionData;
struct PartnerCollectionData;
struct ItemSetData;
struct LINEUPINFO;
struct ACCSHOPITEM;
struct POINTSSHOPITEM;
struct JUMPPOINT;

class GDFManager;
class COBJBlockManager;
class CFileManager;
class CItemManager;
class CSkillManager;
class CMobManager;
class CMapManager;
class CNpcManager;
class CStartCharManager;
class CBuffManager;
class CSkillShopManager;
class CBigMapManager;
class CBlockWordManager;
class CMobDescManager;
class CNewBieManager;
class CNewGuideManager;
class CPetManager;
class CWeaponsRankData;
class CNewbieTipsManager;
class CNewGuideManager;
class CSkillCombinationManager;
class CMixShopManager;
class CTitleManager;
class CFactionManager;
class CPartnerCollectionManager;
class CItemSetManager;
class CLineupManager;
class CManorManager;
class CAccmulationShopManager;
class CPointsShopManager;
class CAutoPathManager;

struct BlockPos;
struct PathFindLoadData;
struct BinaryClientMapInfo;
struct BlockWordData;
struct MobDescData;
struct ItemMixData;
struct TitleData;
struct ManorData;

typedef StlVector<const ItemData*, ManagedAllocator<const ItemData* > > ItemDataList;
typedef StlVector<const SkillData*, ManagedAllocator<const SkillData* > > SkillDataList;
typedef StlVector<const MobData*, ManagedAllocator<const MobData* > > MobDataList;
typedef StlVector<const BuffData*, ManagedAllocator<const BuffData* > > BuffDataList;
typedef StlVector<String, ManagedAllocator<String > > BlockWordDataList;
typedef StlVector<const ItemMixData*, ManagedAllocator<const ItemMixData* > > ItemMixDataList;
typedef StlVector<BlockPos, ManagedAllocator<BlockPos> > BlockPosVector;
typedef StlMap<UInt32,const PartnerCollectionData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, const PartnerCollectionData*> > > PartnerCollectionDataMap;

class CClientResManager
{
public:
	CClientResManager();
	CClientResManager(GDFManager* gdfmgr, GDFManager* gdfmgr2);
	~CClientResManager();

	// File Manager
	BYTE* LoadFileData(const TCHAR *filename,  UInt64 & size);
	BOOL FreeFileData(const TCHAR *filename);

	// OBJ Blocking Manager
	BlockPosVector* LoadOBJBlocking(const TCHAR *filename, Int state); // .ENT.BLK extension
	BlockPosVector* LoadOBJBlocking_FromEntFile(const TCHAR *filename, Int state); // .ENT extension
	BOOL FreeOBJBlocking(const TCHAR *filename);

	// Item Data Manager
	const ItemData* GetItemData(UInt32 itemid);
	void GetItemList(ItemDataList &itemList, UInt8 filterType = 0, UInt8 item_type = 0);
	void LoadItemCSV();
	void LoadItemDAT();

	// Skill Data Manager
	const SkillData* GetSkillData(UInt16 skillid);
	void GetSkillList(SkillDataList &skillList);
	void GetSkillLink(UInt16 skill_id, SkillDataList &skillList);
	void GetSkillLink(UInt16 skill_id, UInt16Vector &skillIds);
	void LoadSkillCSV();
	void LoadSkillDAT();

	// Mob Manager
	const MobData* GetMobData(UInt32 mob_id);
	void LoadMobCSV();
	void LoadMobDAT();
	void GetMobList(MobDataList &mobList);

	// Map Manager
	const MapDB* GetMapData(UInt32 mapid);

	const BinaryClientMapInfo* GetCliMapData(UInt32 mapid);

	BOOL ActivateMap(UInt32 mapid, PathFindLoadData& pPathFindData, UInt32& pNumGuide); // load block and npc/obj/jumppoint instance info
	VOID LoadMapDAT();
	VOID LoadClientMapDAT();

	// NPC Manager
	const NpcData* GetNpcData(UInt32 npcid);
	void LoadNpcCSV();
	VOID LoadNpcDAT();

	//Start Character Manager
	const StartCharData* GetStartChar(UInt32 sexClass);
	void LoadStartCharCSV();

	//Buff Manager
	const BuffData* GetBuffData(UInt32 buffid);
	void GetBuffList(BuffDataList &buffList);
	void LoadBuffCSV();
	void LoadBuffDAT();

	//SkillShop Manager
	const SkillShopData* GetSkillShop(UInt32 skillShop_id);
	void LoadSkillShopCSV();
	void LoadSkillShopDAT();

	//BigMap Manager
	const BigMapData* GetBigMap(UInt16 map_id);
	void LoadBigMapCSV();
	const BigMapData* GetBigMapByIndex(UInt32 index);
	UInt32 GetMapCount();
	const BigMapBelongData* GetBigMapBelong(UInt16 map_id);
	//Mob Desc Manager
	void LoadMobDescCSV();
	String GetMobDesc(const UInt32& ref_Id);

	//BlockWordManager
	VOID LoadBlockWordCSV();
	const BlockWordData* GetBlockWordData(UInt32 index);
	VOID GetBlockWordList(BlockWordDataList &blockWordList);
	BlockWordDataList* GetMsgBlockWordList();
	BlockWordDataList* GetNameBlockWordList();

	//NewBie Manager
	const NewbieData* GetNewBieData(UInt16 newbie_id);
	void LoadNewBieCSV();

	//Pet Manager
	const PetData* GetPetData(UInt32 pet_id);
	void LoadPetCSV();

	//WeaponsRankData
	const WeaponsRankInfo * GetWeaponsRankData(UInt32 weaponsrank_id);
	void LoadWeaponsRankDataCSV();
	
	//NewBieTips Manager
	const NewbieTipsData* GetNewBieTipsData(UInt16 newbietips_id);
	void LoadNewBieTipsCSV();

	//NewGuide Manager
	const NewGuideData* GetNewGuideData(UInt16 newGuide_id);
	void LoadNewGuideCSV();

	// skill combination manager
	CONST MobSkillCombination* GetMobSkillCombination(String szeFileName);
	VOID LoadSkillCombinationCSV();

	// mix shop manager
	VOID LoadMixShopCSV();
	const MixShopData* GetMixShopData(UInt32 shopID);
	void GetSuitableItemList(String filename, UInt16 skillLv_low, UInt16 skillLv_up, ItemMixDataList &itemList);
	void GetStoneProbList(UInt32 stoneID, UInt8Vector& stoneProb);
	UInt GetMaxStoneMixNum(UInt32 stoneID);
	UInt GetMinStoneMixNum(UInt32 stoneID);
    UInt GetStoneMixProb(UInt32 stoneID, UInt stack);
	UInt GetResultStoneID(UInt32 stoneID);
	
	//Title Manager
	const TitleData* GetTitleData(UInt32 title_id);
	const TitleData* GetTitleDataByRank(UInt8	Title_RankType, UInt8	Title_RankSubType, UInt8	Title_StartRank, UInt8	Title_EndRank);
	void LoadTitleCSV();

	//Faction Manager
	const FactionData* GetFactionData(UInt32 faction_id);
	void LoadFactionsCSV();

	INLINE GDFManager* GetGDFManager(){ return m_GdfResource;}
	INLINE GDFManager* GetGDFExManager(){ return m_GdfResourceEx;}

	// PartnerCollection Data Manager
	void GetPartnerCollectionData(PartnerCollectionDataMap &PartnerCollectionList, UInt8 Type);
	void GetPartnerCollectionData(PartnerCollectionDataMap &PartnerCollectionList, UInt8 Type, UInt8 Group);
	const PartnerCollectionData* GetPartnerCollectionData(UInt32 collection_id);
	size_t GetPartnerCollectionDataSize();
	void LoadPartnerCollectionCSV();

	// ItemSet Manager
	const ItemSetData* GetItemSetData(UInt32 itemset_id);
	size_t GetItemSetDataSize();
	void LoadItemSetCSV();

	//lineup data manager
	const LINEUPINFO* GetLineupData(UInt32 uiLineupId);
	size_t GetLineupDataSize();
	void LoadLineupCSV();

	// Manor Manager
	void LoadManorCSV();	
	const ManorData* GetManorData(UInt8 manor_scale);

	//Accmulation Shop Manager
	const ACCSHOPITEM* GetAccShopData(UInt32 uiIndex);
	size_t GetAccShopItemDataSize();
	void LoadAccShopItemCSV();

	//Points Shop Manger
	const POINTSSHOPITEM* GetPointsShopData(UInt32 uiItemID);
	size_t GetPointsShopItemDataSize();
	void LoadPointsShopItemCSV();

	//AutoPath Manager
	const JUMPPOINT* GetAutoPathData(UInt32 uSeq);
	size_t GetAutoPathDataSize();
	int   GetMapJumpPoints(UInt32 uMapId,UInt32Vector& uvPoints);
	void LoadAutoPathCSV();
private:
	GDFManager*			m_GdfResource;
	GDFManager*			m_GdfResourceEx;
	COBJBlockManager*	m_objBlockManager;
	CFileManager*		m_fileManager;
	CItemManager*		m_itemManager;
	CSkillManager*		m_skillManager;
	CMobManager*		m_mobManager;
	CMapManager*		m_mapManager;
	CNpcManager*		m_npcManager;
	CStartCharManager*	m_StartCharManager;
	CBuffManager*		m_BuffManager;
	CSkillShopManager*	m_SkillShopManager;
	CBigMapManager*		m_BigMapManager;
	CBlockWordManager*	m_pBlockWordManager;
	CMobDescManager*	m_mobDescManager;
	CNewBieManager*		m_newBieManager;
	CPetManager*		m_petManager;
	CWeaponsRankData*	m_pWeaponsRankData;
	CNewbieTipsManager*		m_newBieTipsManager;
	CNewGuideManager*	m_newGuideManager;
	CSkillCombinationManager*	m_SkillCombinationManager;
	CMixShopManager*	m_mixShopManager;
	CTitleManager*		m_TitleManager;
	CFactionManager*	m_FactionManager;
	CPartnerCollectionManager*	m_pPartnerCollectionManager;
	CItemSetManager*	m_pItemSetManager;
	CLineupManager*     m_pLineupManager;
	CManorManager*		m_manorManager;
	CAccmulationShopManager* m_pAccShopManager;
	CPointsShopManager	*m_pPointsShopManager;
	CAutoPathManager* m_pAutoPathManager;
};

#endif	// _CLIENTRESMANAGER_H_
