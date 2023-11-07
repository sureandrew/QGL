#pragma once
#ifndef _SERVERRESMANAGER_H_
#define _SERVERRESMANAGER_H_

#include <vector>

struct ItemData;
struct SkillData;
struct MobData;
struct MapDB;
struct NpcData;
struct NPCDB;
struct StartCharData;
struct ShopLstData;
struct SkillMap;
struct CSVItemData;
struct CSVMobData;
struct CSVNpcData;
struct CSVQuestRow;
struct BuffData;
struct MobSkillCombination;
struct SkillShopData;
struct BlockWordData;
struct EventScheduleData;
struct ChargeShopData;
struct PetData;
struct MixShopData;
struct ItemMixShop;
struct ItemMixData;
struct AntiHackData;
struct JournalData;
struct QuestionData;
struct PartnerCollectionData;
struct ItemSetData;
struct LINEUPINFO;
struct ManorData;
struct BusinessShopData;
struct ACCSHOPITEM;
struct POINTSSHOPITEM;
struct JUMPPOINT;

class COBJBlockManager;
class CFileManager;
class CItemManager;
class CSkillManager;
class CMobManager;
class CMapManager;
class CNpcManager;
class CScriptManager;
class CStartCharManager;
class CShopLstManager;
class CBuffManager;
class CSkillCombinationManager;
class CSkillShopManager;
class CBlockWordManager;
class CRandTableManager;
class CEventScheduleManager;
class CChargeShopManager;
class CPetManager;
class CMixShopManager;
class CTitleManager;
class CAntiHackManager;
class CJournalManager;
class CQuestionManager;
class CPartnerCollectionManager;
class CItemSetManager;
class CLineupManager;
class CManorManager;
class CBusinessShopManager;
class CAccmulationShopManager;
class CPointsShopManager;
class CAutoPathManager;

struct BlockPos;
struct PathFindLoadData;
struct TitleData;
struct BinaryServerNPCInstance;

typedef StlVector<const ItemData*, ManagedAllocator<const ItemData* > > ItemDataList;
typedef StlVector<const SkillData*, ManagedAllocator<const SkillData* > > SkillDataList;
typedef StlVector<const MobData*, ManagedAllocator<const MobData* > > MobDataList;
typedef StlVector<const BuffData*, ManagedAllocator<const BuffData* > > BuffDataList;
typedef StlVector<String, ManagedAllocator<String > > BlockWordDataList;
typedef StlMap<UInt32, EventScheduleData*, std::less<UInt32>, ManagedAllocator<std::pair<Int32, EventScheduleData*> > > EventScheduleMap;
typedef StlMap<UInt32, MapDB*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, MapDB*> > > MapMapInt;
typedef StlVector<const ChargeShopData*, ManagedAllocator<const ChargeShopData* > > ChargeShopDataList;
typedef StlVector<const ItemMixData*, ManagedAllocator<const ItemMixData* > > ItemMixDataList;
typedef StlVector<const JournalData*, ManagedAllocator<const JournalData*> > JournalDataList;
typedef StlVector<BlockPos, ManagedAllocator<BlockPos> > BlockPosVector;
typedef StlMap<UInt16, BusinessShopData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, BusinessShopData*> > > BusinessShopDataMap;

class CServerResManager
{
public:
	CServerResManager();
	~CServerResManager();

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
	void LoadSkillCSV();
	void LoadSkillDAT();
	void GetSkillLink(UInt16 skill_id, SkillDataList &skillList);
	void GetSkillLink(UInt16 skill_id, UInt16Vector &skillIds);

	// Mob Manager
	const MobData* GetMobData(UInt32 mob_id);
	void GetMobList(MobDataList &mobList);
	void LoadMobCSV();
	void LoadMobDAT();

	// Map Manager
	const MapDB* GetMapData(UInt32 mapid);
	BOOL ActivateMap(UInt32 mapid, PathFindLoadData& pPathFindData, UInt32& pNumGuide); // load block and npc/obj/jumppoint instance info
	VOID LoadMapDAT();
	VOID LoadMapNpc();
	void GetMapList(MapMapInt& mapList);
	BOOL GetNpcPosition(UInt32 map_id, UInt32 npc_id, UInt16 &x, UInt16 &y);
	BinaryServerNPCInstance* GetNpcInstance(UInt32 map_id, UInt32 npc_id);

	// NPC Manager
	const NpcData* GetNpcData(UInt32 npcid);
	void LoadNpcCSV();
	VOID LoadNpcDAT();
	void LoadNpcNameCSV();

	UInt16 GetNPCFstNameNum();
	UInt16 GetNPCMaleSndNameNum();
	UInt16 GetNPCFemaleSndNameNum();

	String GetNPCMaleName(UInt16 id);
	String GetNPCFemaleName(UInt16 id);
	String GetNPCSurname(UInt16 id);	

	// Script Manager
	CONST char* GetLuaData(CONST StringA &filetitle, size_t & len, BOOL bBatScript = FALSE); 
	VOID LoadLuaData();

	const StartCharData* GetStartCharData(UInt32 sexClass);
	void LoadStartCharCSV();

	// Shop List Manager
	CONST ShopLstData* GetShopInfo(UInt32 shopId);
	VOID LoadShopCSV();

	// CSV Item List Manager
	CONST CSVItemData* GetCSVItemInfo(String szfilename);
	CONST CSVMobData* GetCSVRandomMob(String szfilename);
	CONST CSVNpcData* GetCSVRandomNpc(String szfilename);
	CONST CSVQuestRow* GetCSVRandomQuest(UInt32 Uid);
	VOID LoadCSVItemTable();
	VOID LoadCSVQuestTable();

	//Buff Manager
	const BuffData* GetBuffData(UInt16 buffid);
	void GetBuffList(BuffDataList &buffList);
	void LoadBuffCSV();
	void LoadBuffDAT();

	// skill combination manager
	CONST MobSkillCombination* GetMobSkillCombination(String szeFileName);
	VOID LoadSkillCombinationCSV();

	//SkillShop Manager
	const SkillShopData* GetSkillShop(UInt32 skillShop_id);
	void LoadSkillShopCSV();
	void LoadSkillShopDAT();

	//Random Table Manager
	VOID LoadAllRandTable();

	//BlockWordManager
	VOID LoadBlockWordCSV();
	const BlockWordData* GetBlockWordData(UInt32 index);
	VOID GetBlockWordList(BlockWordDataList &blockWordList);
	BlockWordDataList* GetMsgBlockWordList();
	BlockWordDataList* GetNameBlockWordList();

	//EventScheduleManager
	VOID LoadEventScheduleCSV();
	const EventScheduleData* GetEventScheduleData(UInt32 eventId);
	BOOL GetEventScheduleVector(EventScheduleMap& schedule);

	//ChargeShoManager
	VOID LoadChargeShopCSV();
	const ChargeShopData* GetChargeShopData(UInt32 index);
	ChargeShopDataList GetChargeShopList();
	UInt32 GetChargeShopItemCount();

	//Pet Manager
	const PetData* GetPetData(UInt32 pet_id);
	void LoadPetCSV();

	// mix shop manager
	VOID LoadMixShopCSV();
	const MixShopData* GetMixShopData(UInt32 shopID);
	//void GetSuitableItemList(String filename, UInt16 skillLv_low, UInt16 skillLv_up, ItemMixDataList &itemList);
	const ItemMixData* GetItemMixData(String filename, UInt32 productID);
	void GetStoneProbList(UInt32 stoneID, UInt8Vector& stoneProb);
	UInt GetMaxStoneMixNum(UInt32 stoneID);
	UInt GetMinStoneMixNum(UInt32 stoneID);
    UInt GetStoneMixProb(UInt32 stoneID, UInt stack);
	UInt GetResultStoneID(UInt32 stoneID);

	//Title Manager
	const TitleData* GetTitleData(UInt32 title_id);
	const TitleData* GetTitleDataByRank(UInt8	Title_RankType, UInt8	Title_RankSubType, UInt8	Title_StartRank, UInt8	Title_EndRank);
	const TitleData* GetTitleDataByType(UInt8	Title_Type, UInt8	Title_RankSubType, UInt8	Title_StartRank, UInt8	Title_EndRank);
	void LoadTitleCSV();

	//Anti-Hack Manager
	VOID LoadAntihackDAT();
	VOID GetAntiHackInfo(UInt32 hackID, BYTE*& mapViewPtr, BYTE*& imagePtr, UInt32& imageSize);
	VOID ReleaseHackImagePtr(BYTE* mapViewPtr);
	UInt32 GetHackInfoSize();

	// Quest Journal Manager
	VOID LoadJournalCSV();
	CONST JournalData* GetJournalData(UInt16 journal_id);
	void GetJournalList(JournalDataList &journalList);

	// Question Data Manager
	const QuestionData* GetQuestionData(UInt32 question_id);
	size_t GetQuestionDataSize();
	void LoadQuestionCSV();
	void LoadQuestionDAT();

	// PartnerCollection Data Manager
	const PartnerCollectionData* GetPartnerCollectionData(UInt32 collection_id);
	size_t GetPartnerCollectionDataSize();
	void LoadPartnerCollectionCSV();
	UInt16 GetPartnerCollectionBuffID(UInt16 Count, UInt8 Type);

	// ItemSet Manager
	const ItemSetData* GetItemSetData(UInt32 itemset_id);
	size_t GetItemSetDataSize();
	void LoadItemSetCSV();

	const LINEUPINFO* GetLineupData(UInt32 uiLineupId);
	size_t GetLineupDataSize();
	void LoadLineupCSV();

	// Manor Manager
	void LoadManorCSV();	
	const ManorData* GetManorData(UInt8 manor_scale);

	// Business Shop Manager
	const BusinessShopData* GetBusinessShopData(UInt32 shop_id);
	BusinessShopDataMap* GetBusinessShopDataMap();
	BOOL LoadBusinessShopCSV();

	//Accmulation Shop Manager
	const ACCSHOPITEM* GetAccShopData(UInt32 uiIndex);
	size_t GetAccShopItemDataSize();
	void LoadAccShopItemCSV();

	//Points Shop Manger
	const POINTSSHOPITEM* GetPointsShopData(UInt32 uiItemID);
	size_t GetPointshopItemDataSize();
	void LoadPointsShopItemCSV();

	//cross map manager
	const JUMPPOINT* GetAutoPathData(UInt32 uSeq);
	size_t GetAutoPathDataSize();
	int   GetMapJumpPoints(UInt32 uMapId,UInt32Vector& uvPoints);
	void LoadAutoPathCSV();

private:
	COBJBlockManager*	m_objBlockManager;
	CFileManager*		m_fileManager;
	CItemManager*		m_itemManager;
	CSkillManager*		m_skillManager;
	CMobManager*		m_mobManager;
	CMapManager*		m_mapManager;
	CNpcManager*		m_npcManager;
	CScriptManager*		m_pScriptManager;
	CStartCharManager*	m_startCharManager;
	CShopLstManager*	m_pShopLstManager;
	CBuffManager*		m_BuffManager;
	CSkillCombinationManager*	m_SkillCombinationManager;
	CSkillShopManager*	m_SkillShopManager;
	CBlockWordManager*	m_pBlockWordManager;
	CRandTableManager*  m_pRandTableManager;
	CEventScheduleManager*		m_EventScheduleManager;
	CChargeShopManager* m_pChargeShopManager;
	CPetManager*		m_petManager;
	CMixShopManager*	m_mixShopManager;
	CTitleManager*		m_TitleManager;
	CAntiHackManager*   m_pHackManager;	
	CJournalManager*	m_pJournalManager;	
	CQuestionManager*	m_pQuestionManager;
	CPartnerCollectionManager*	m_pPartnerCollectionManager;
	CItemSetManager*	m_pItemSetManager;
	CLineupManager*     m_pLineupManager;
	CManorManager*		m_manorManager;	
	CBusinessShopManager*	m_pBusinessShopManager;
	CAccmulationShopManager* m_pAccmulationShopManager;
	CPointsShopManager	*m_pPointsShopManager;
	CAutoPathManager* m_pAutoPathManager;
};

#endif	// _SERVERRESMANAGER_H_
