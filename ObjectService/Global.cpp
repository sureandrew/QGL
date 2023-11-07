//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/RPCFunc.h"
//-- Self
#include "ObjectService/Global.h"
//-- Library
#include "Stub/Data/AccountData.h"
#include "Stub/Data/CharacterData.h"
#include "Stub/Data/ItemData.h"
#include "Stub/Data/SkillData.h"
#include "Stub/Data/PartnerData.h"
#include "Stub/Data/QuestLogData.h"
#include "Stub/Data/QuestFlagData.h"
#include "Stub/Data/QuestNpcFlagData.h"
#include "Stub/Data/QuestTypeData.h"
#include "Stub/Data/MessengerData.h"
#include "Stub/Data/PetData.h"
#include "Stub/Data/ChargeShopData.h"
#include "Stub/Data/TitleData.h"
#include "Stub/Data/GuildData.h"
#include "Stub/Data/GuildMemberData.h"
#include "ObjectService/Configure.h"
#include "ObjectService/dboAccount.h"
#include "ObjectService/dboCharacter.h"
#include "ObjectService/dboItem.h"
#include "ObjectService/dboSkill.h"
#include "ObjectService/dboPartner.h"
#include "ObjectService/dboQuestType.h"
#include "ObjectService/dboQuestLog.h"
#include "ObjectService/dboQuestFlag.h"
#include "ObjectService/dboMessenger.h"
#include "ObjectService/dboPet.h"
#include "ObjectService/dboChargeShop.h"
#include "ObjectService/dboTitle.h"
#include "ObjectService/dboMsgCharacter.h"
#include "ObjectService/dboGuildMember.h"
#include "ObjectService/dboGuild.h"
#include "ObjectService/dboGift.h"
#include "ObjectService/dboHawkerHistory.h"
#include "ObjectService/dboSellYuanBao.h"
#include "ObjectService/dboGeneration.h"
#include "ObjectService/DboSourcePool.h"
#include "RPCEnum/ObjectService_enum.h"
#include "Resource/ServerResManager.h"
#include "Stub/Data/PetData.h"
#include "Stub/Data/MsgCharacterData.h"
#include "Stub/Data/GiftData.h"
#include "Stub/Data/HawkerHistoryData.h"
#include "Stub/Data/SellYuanBaoData.h"
#include "Resource/ResBuff.h"
#include "Resource/ServerResManager.h"
#include "DBPWDecrypt.h"
#include <algorithm>
#include <Reuben/Math/Hex.h>
#include "Stub/Data/PartnerCollectionData.h"
#include "ObjectService/dboPartnerCollection.h"
#include "Stub/Data/SiblingData.h"
#include "ObjectService/dboSibling.h"
#include "Stub/Data/GenerationData.h"
#include "ObjectService/dboGeneration.h"
#include "Stub/Data/MarriageData.h"
#include "ObjectService/dboMarriage.h"
#include "ObjectService/dboQuestNpcFlag.h"

#define CONFIGURATION_INI_FILE		_T(".\\Data\\Setting\\fileconfig.ini")
//#define UPDATEDB_INTERVAL		60000			// update database interval time (1 min)
//#define CACHE_TIME				60000			// data object cache time (1 min)
//#define MSG_CHAR_CACHE_TIME		60000 //600000	// data object cache time (1 min)
//#define BAD_NAME_TIME			60000			// bad name cache time (1 min)
#define DB_ACCT_COUNT			10				// account ole-db pool count
#define DB_GAME_COUNT			10				// game ole-db pool count

struct UpdateRequest
{
	RPCNetIDVector	updateIds;
	RPCNetIDVector	releaseIds;
	RPCNetIDVector	deleteIds;
	UInt16			idCount;

	UpdateRequest() : idCount(0) {}
};

Global::Global()
{
	pManagerPeer = NULL;
	pLocalPeer = NULL;
	m_bExit = BOOLEAN_FALSE;
	m_pAccountDb = NULL;
	m_pGameDb = NULL;
	workCount = 0;
	completeEvent = NULL;
	QueueUserWorkItem = NULL;
	m_badNameTime = ::GetTickCount();
}

Boolean Global::Initialize(VOID)
{
	m_rand.Randomize();
	m_resource = SafeCreateObject(CServerResManager);
	m_resource->LoadItemCSV();
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Mob..."));
	Int bReadMobDat = GetPrivateProfileInt(_T("ReadDataOption"), _T("ReadMobDat"), 0, CONFIGURATION_INI_FILE);
	if( bReadMobDat )
		m_resource->LoadMobDAT();
	else
		m_resource->LoadMobCSV();

	m_resource->LoadPetCSV();

	m_resource->LoadTitleCSV();

	if (gConfigure.checkInvalidData)
	{
		//load all csv and map data
		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Npc..."));
		
		m_resource->LoadNpcCSV();
	
		m_resource->LoadMobCSV();

		m_resource->LoadSkillCSV();

		m_resource->LoadBuffCSV();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Map..."));
		m_resource->LoadMapDAT(); // LoadNPC() must be executed before.

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Shop List..."));
		m_resource->LoadShopCSV();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading All Random Table ..."));
		m_resource->LoadAllRandTable();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Skill Shop List..."));
		m_resource->LoadSkillShopCSV();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading MobSkill List..."));
		m_resource->LoadSkillCombinationCSV();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Start Character List..."));
		m_resource->LoadStartCharCSV();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading World Event Schedule List..."));
		m_resource->LoadEventScheduleCSV();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading BlockWord List..."));
		m_resource->LoadBlockWordCSV();

		//TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading ChargeShop List..."));
		//m_resource->LoadChargeShopCSV();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Title List..."));
		m_resource->LoadTitleCSV();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Pet List..."));
		m_resource->LoadPetCSV();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Mix Shop..."));
		m_resource->LoadMixShopCSV();

		//	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Anti-Hack Image..."));
		//	m_resource->LoadAntihackDAT();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading NPC Name..."));
		m_resource->LoadNpcNameCSV();	
	}
	completeEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	WorkQueueBegin();				// treat as first queue begin
	HINSTANCE hInst = ::LoadLibraryA("Kernel32.DLL");
	if (hInst) {
		QueueUserWorkItem = (LPQueueWork) ::GetProcAddress(hInst, "QueueUserWorkItem");
		if (QueueUserWorkItem == NULL)
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Initialize Error: cannot get 'QueueUserWorkItem' from kernel32.dll"));
			return BOOLEAN_FALSE;
		}
	}

	
	TCHAR emptyPass[65];
	for (int i = 0; i < 65; ++i)
		emptyPass[i] = 32;	
	emptyPass[64] = NULL;
	Size size = 0;
	Reuben::Math::HexToBytes(NULL, size, emptyPass, 64);
	m_hashed_empty_pw.SetSize(size);
	Reuben::Math::HexToBytes(&m_hashed_empty_pw, size, emptyPass, 64);

	return (BOOLEAN_TRUE);
}

VOID Global::Uninitialize(VOID)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Uninitialize: update db and delete all objects ..."));

	m_acctRelMap.clear();
	m_charRelMap.clear();
	m_msgCharRelMap.clear();

	for(DataIdMap::iterator iter = acctIdMap.begin(); iter != acctIdMap.end(); iter++)
	{
		DeleteNetObject(iter->second.pobj);
	}
	acctIdMap.clear();
	acctNameMap.clear();

	for(DataIdMap::iterator iter = charIdMap.begin(); iter != charIdMap.end(); iter++)
	{
		DeleteNetObject(iter->second.pobj);
	}
	charIdMap.clear();
	charNameMap.clear();

	for(DataIdMap::iterator iter = itemIdMap.begin(); iter != itemIdMap.end(); iter++)
	{
		DeleteNetObject(iter->second.pobj);
	}
	itemIdMap.clear();

	for(GroupMap::iterator iter = itemGroupMap.begin(); iter != itemGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	itemGroupMap.clear();

	for(GroupMap::iterator iter = wareHouseItemGroupMap.begin(); iter != wareHouseItemGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	wareHouseItemGroupMap.clear();

	for(DataIdMap::iterator iter = partnerIdMap.begin(); iter != partnerIdMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	partnerIdMap.clear();

	for(GroupMap::iterator iter = partnerGroupMap.begin(); iter != partnerGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	partnerGroupMap.clear();

	for(GroupMap::iterator iter = wareHousePartnerGroupMap.begin(); iter != wareHousePartnerGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	wareHousePartnerGroupMap.clear();

	for(DataIdMap::iterator iter = petIdMap.begin(); iter != petIdMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	petIdMap.clear();

	for(GroupMap::iterator iter = petGroupMap.begin(); iter != petGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	petGroupMap.clear();

	for(GroupMap::iterator iter = wareHousePetGroupMap.begin(); iter != wareHousePetGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	wareHousePetGroupMap.clear();

	for(SkillDataMap::iterator iter = skillMap.begin(); iter != skillMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	skillMap.clear();

	for(SkillGroupMap::iterator iter = skillGroupMap.begin(); iter != skillGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	skillGroupMap.clear();

	for(MessengerDataMap::iterator iter = messengerDataMap.begin(); iter != messengerDataMap.end(); iter++)
	{
		DeleteNetObject(iter->second.pobj);
	}
	messengerDataMap.clear();

	for(GroupMap::iterator iter = messengerGroupMap.begin(); iter != messengerGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	messengerGroupMap.clear();

	for(QuestLogDataMap::iterator iter = questLogMap.begin(); iter != questLogMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	questLogMap.clear();

	for(GroupMap::iterator iter = questLogGroupMap.begin(); iter != questLogGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	questLogGroupMap.clear();

	for(QuestFlagDataMap::iterator iter = questFlagMap.begin(); iter != questFlagMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	questFlagMap.clear();
	ClearAllUnreleasedFlagId();

	for(GroupMap::iterator iter = questFlagGroupMap.begin(); iter != questFlagGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	questFlagGroupMap.clear();

	for(QuestTypeDataMap::iterator iter = questTypeMap.begin(); iter != questTypeMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	questTypeMap.clear();
	ClearAllUnreleasedTypeId();

	for(GroupMap::iterator iter = questTypeGroupMap.begin(); iter != questTypeGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	questTypeGroupMap.clear();

	for(DataIdMap::iterator iter = chargeShopIdMap.begin(); iter != chargeShopIdMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	chargeShopIdMap.clear();

	for(DataIdMap::iterator iter = titleIdMap.begin(); iter != titleIdMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	titleIdMap.clear();

	for(GroupMap::iterator iter = titleGroupMap.begin(); iter != titleGroupMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	titleGroupMap.clear();

	for(DataIdMap::iterator iter = msgCharMap.begin(); iter != msgCharMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	msgCharMap.clear();

	for(DataIdMap::iterator iter = guildMap.begin(); iter != guildMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	guildMap.clear();

	for(DataIdMap::iterator iter = guildMemberMap.begin(); iter != guildMemberMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	guildMemberMap.clear();

	for(GroupMap::iterator iter = guildMemberGrpMap.begin(); iter != guildMemberGrpMap.end(); iter++)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	guildMemberGrpMap.clear();

	for(DataIdMap::iterator iter = giftIdMap.begin(); iter != giftIdMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	giftIdMap.clear();

	/*for(GroupMap::iterator iter = giftGroupMap.begin(); iter != giftGroupMap.end(); ++iter)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	giftGroupMap.clear();*/

	for (GroupMap::iterator iter = existItemGift.begin(); iter != existItemGift.end(); ++iter)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	existItemGift.clear();

	for (GroupMap::iterator iter = existPartnerGift.begin(); iter != existPartnerGift.end(); ++iter)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	existPartnerGift.clear();

	for (GroupMap::iterator iter = existPetGift.begin(); iter != existPetGift.end(); ++iter)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	existPetGift.clear();

	remainItemGift.clear();
	remainPartnerGift.clear();
	remainPetGift.clear();

	giftDelMap.clear();

	for(DataIdMap::iterator iter = hawkerHistoryIdMap.begin(); iter != hawkerHistoryIdMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	hawkerHistoryIdMap.clear();

	for(GroupMap::iterator iter = hawkerHistoryGroupMap.begin(); iter != hawkerHistoryGroupMap.end(); ++iter)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	hawkerHistoryGroupMap.clear();

	for(DataIdMap::iterator iter = partnerCollectionIdMap.begin(); iter != partnerCollectionIdMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	partnerCollectionIdMap.clear();

	for(GroupMap::iterator iter = partnerCollectionGroupMap.begin(); iter != partnerCollectionGroupMap.end(); ++iter)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	partnerCollectionGroupMap.clear();

	for(DataIdMap::iterator iter = sellYBIdMap.begin(); iter != sellYBIdMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	sellYBIdMap.clear();

	for(DataIdMap::iterator iter = siblingIdMap.begin(); iter != siblingIdMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	siblingIdMap.clear();

	for(DataIdMap::iterator iter = marriageIdMap.begin(); iter != marriageIdMap.end(); ++iter)
	{
		DeleteNetObject(iter->second.pobj);
	}
	marriageIdMap.clear();

	/*for(GroupMap::iterator iter = sellYBGroupMap.begin(); iter != sellYBGroupMap.end(); ++iter)
	{
		NetGroup *grp = CastNetObject<NetGroup>(iter->second.pobj);
		SafeDeleteObject(grp);
	}
	sellYBGroupMap.clear();*/

	SafeDeleteObject(m_resource);
	ReleaseOLEDB();
	m_hashed_empty_pw.Clear();
}

Boolean Global::ConnectDatabase()
{
	HRESULT hr;

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Begin to Connect Databasae"));

	if (DB_FAILED(hr = CoInitializeEx(NULL, COINIT_FLAG | COINIT_SPEED_OVER_MEMORY)))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("oledb com initialization failed, hr 0x%08X"), hr);
		return BOOLEAN_FALSE;
	}

	// OLEDB object MUST create using 'new' instead of our ManagedAllocator
	const Database& db = gConfigure.dbAccount;
	if (m_pAccountDb == NULL)
		m_pAccountDb = SafeCreateObject(CDboSource);
#ifdef CONNECT_STR
	String cstr1;
	cstr1.Format(CONNECT_STR, db.strServer.c_str(), db.strDatabase.c_str(), 
		db.strLogin.c_str(), DBPWDecrypt(db.strPassword).c_str());
	if (DB_FAILED(hr = m_pAccountDb->Open(cstr1.c_str())))
#else
	if (DB_FAILED(hr = m_pAccountDb->Open(db.strServer.c_str(), db.strDatabase.c_str(), 
		db.strLogin.c_str(), DBPWDecrypt(db.strPassword).c_str())))
#endif	// CONNECT_STR
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("cannot connect to account database, hr 0x%08X"), hr);
		m_pAccountDb->ShowError(hr);
		ReleaseOLEDB();
		return BOOLEAN_FALSE;
	}

	const Database &db2 = gConfigure.dbGame;
	if (m_pGameDb == NULL)
		m_pGameDb = SafeCreateObject(CDboSource);
#ifdef CONNECT_STR
	String cstr2;
	cstr2.Format(CONNECT_STR, db2.strServer.c_str(), db2.strDatabase.c_str(), 
		db2.strLogin.c_str(), DBPWDecrypt(db2.strPassword).c_str());
	if (DB_FAILED(hr = m_pGameDb->Open(cstr2.c_str())))
#else
	if (DB_FAILED(hr = m_pGameDb->Open(db2.strServer.c_str(), db2.strDatabase.c_str(), 
		db2.strLogin.c_str(), DBPWDecrypt(db2.strPassword).c_str())))
#endif	// CONNECT_STR
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("cannot connect to game database, hr 0x%08X"), hr);
		m_pGameDb->ShowError(hr);
		ReleaseOLEDB();
		return BOOLEAN_FALSE;
	}

	gTables.pAccount = SafeCreateObject(CdboAccount);
	gTables.pAccount->SetDataSource(m_pAccountDb);
	gTables.pCharacter = SafeCreateObject(CdboCharacter);
	gTables.pCharacter->SetDataSource(m_pGameDb);
	gTables.pItem = SafeCreateObject(CdboItem);
	gTables.pItem->SetDataSource(m_pGameDb);
	gTables.pPartner = SafeCreateObject(CdboPartner);
	gTables.pPartner->SetDataSource(m_pGameDb);
	gTables.pSkill = SafeCreateObject(CdboSkill);
	gTables.pSkill->SetDataSource(m_pGameDb);
	gTables.pQuestLog = SafeCreateObject(CdboQuestLog);
	gTables.pQuestLog->SetDataSource(m_pGameDb);
	gTables.pQuestFlag = SafeCreateObject(CdboQuestFlag);
	gTables.pQuestFlag->SetDataSource(m_pGameDb);
	gTables.pQuestType = SafeCreateObject(CdboQuestType);
	gTables.pQuestType->SetDataSource(m_pGameDb);
	gTables.pMessenger = SafeCreateObject(CdboMessenger);
	gTables.pMessenger->SetDataSource(m_pGameDb);
	gTables.pPet = SafeCreateObject(CdboPet);
	gTables.pPet->SetDataSource(m_pGameDb);
	gTables.pChargeShop = SafeCreateObject(CdboChargeShop);
	gTables.pChargeShop->SetDataSource(m_pGameDb);
	gTables.pTitle = SafeCreateObject(CdboTitle);
	gTables.pTitle->SetDataSource(m_pGameDb);
	gTables.pMsgCharacter = SafeCreateObject(CdboMsgCharacter);
	gTables.pMsgCharacter->SetDataSource(m_pGameDb);
	gTables.pGuild = SafeCreateObject(CdboGuild);
	gTables.pGuild->SetDataSource(m_pGameDb);
	gTables.pGuildMember = SafeCreateObject(CdboGuildMember);
	gTables.pGuildMember->SetDataSource(m_pGameDb);
	gTables.pGift = SafeCreateObject(CdboGift);
	gTables.pGift->SetDataSource(m_pAccountDb);
	gTables.pHawkerHistory = SafeCreateObject(CdboHawkerHistory);
	gTables.pHawkerHistory->SetDataSource(m_pGameDb);
	gTables.pPartnerCollection = SafeCreateObject(CdboPartnerCollection);
	gTables.pPartnerCollection->SetDataSource(m_pGameDb);
	gTables.pSellYuanBao = SafeCreateObject(CdboSellYuanBao);
	gTables.pSellYuanBao->SetDataSource(m_pGameDb);
	gTables.pSibling = SafeCreateObject(CdboSibling);
	gTables.pSibling->SetDataSource(m_pGameDb);
	gTables.pGeneration = SafeCreateObject(CdboGeneration);
	gTables.pGeneration->SetDataSource(m_pGameDb);
	gTables.pMarriage = SafeCreateObject(CdboMarriage);
	gTables.pMarriage->SetDataSource(m_pGameDb);
	gTables.pQuestNpcFlag = SafeCreateObject(CdboQuestNpcFlag);
	gTables.pQuestNpcFlag->SetDataSource(m_pGameDb);



	acctDbPool.SetAccountDb(BOOLEAN_TRUE);
	acctDbPool.InitSource(DB_ACCT_COUNT);
	gameDbPool.SetAccountDb(BOOLEAN_FALSE);
	gameDbPool.InitSource(DB_GAME_COUNT);
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Connect Database Success"));

	//check valid data
	if (gConfigure.checkInvalidData)
		CheckValidData();
	return (BOOLEAN_TRUE);
}
VOID Global::CheckValidData()
{
	CheckValidBuffData();
	CheckValidMapData();
}

//extract the buff string
VOID Global::ExtractBuffString(String buff, UInt32 char_id)
{
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	TCHAR *value = NULL; 
	TCHAR *buf = SafeAllocate(TCHAR, buff.size() + 1);
	_tcsncpy(buf, buff.c_str(), buff.size());
	buf[buff.size()] = _T('\0');
	_tcstok(buf, valueDelim);	
	bool first = true;
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Start check buff character: %d buff is %s\n"),  char_id, buf);
	while (true)
	{
		UInt32 ID = 0;
		UInt32 Value1 = 0;
		UInt32 Value2 = 0;
		UInt32 Value3 = 0;
		UInt32 Pro = 0;
		UInt32 CoolDown = 0;
		//buff id
		if (first)
		{
			if (buf)
			{
				ID = _ttoi(buf);
				if (ID == 0 && buf != NULL && wcscmp(buf, _T("0")) != 0 && wcscmp(buf, _T(" ")) != 0)
				{
					TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d ID = 0 but given value is %s\n"),  char_id, buf);
				}
			}
			else
				break;
			first = false;
		}
		else
		{

			value = _tcstok(NULL, valueDelim);
			if (value)
			{
				ID = _ttoi(value);
				if (ID == 0 && value != NULL && wcscmp(value, _T("0")) != 0 && wcscmp(value, _T(" ")) != 0 && value[0] != 60909)
				{
					TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d ID = 0 but given value is %s\n"),  char_id, value);
				}
			}
			else
				break;
		}

		
		if (ID == 0)
			break;

		//buff Value1
		value = _tcstok(NULL, valueDelim );
		if (value)
		{
			Value1 = _ttoi(value);

			if (Value1 == 0 && value != NULL && wcscmp(value, _T("0")) != 0)
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d Value1 = 0 but given value is %s\n"),  char_id, value);
			}
		}
		else
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d no buff value 1 buff id: %d\n"),  char_id, ID);
			break;
		}

		//buff Value2
		value = _tcstok(NULL, valueDelim );
		if (value)
		{
			Value2 = _ttoi(value);
			if (Value2 == 0 && value != NULL && wcscmp(value, _T("0")) != 0)
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d Value2 = 0 but given value is %s\n"),  char_id, value);
			}
		}
		else
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d no buff value 2 buff id: %d\n"),  char_id, ID);
			break;
		}

		//DOT Value3
		value = _tcstok(NULL, valueDelim );
		if (value)
		{
			Value3 = _ttoi(value);
			if (Value3 == 0 && value != NULL && wcscmp(value, _T("0")) != 0)
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d Value3 = 0 but given value is %s\n"),  char_id, value);
			}
		}
		else
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d no buff value 3 buff id: %d\n"),  char_id, ID);
			break;
		}

		//Pro
		value = _tcstok(NULL, valueDelim );
		if (value)
		{
			Pro = _ttoi(value);
			if (Pro == 0 && value != NULL && wcscmp(value, _T("0")) != 0)
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d Pro = 0 but given value is %s\n"),  char_id, value);
			}
		}
		else
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d no buff Pro buff id: %d\n"),  char_id, ID);
			break;
		}

		//buff CoolDown
		value = _tcstok(NULL, buffDelim);
		if (value)
		{
			CoolDown = _ttoi(value);
			if (CoolDown == 0 && value != NULL && wcscmp(value, _T("0")) != 0)
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d CoolDown = 0 but given value is %s\n"),  char_id, value);
			}
		}
		else
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d no buff CoolDown buff id: %d\n"),  char_id, ID);
			break;
		}
		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(ID);
		if (pBuffData == NULL)
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error character: %d invalid buff id: %d\n"),  char_id, ID);
		if(pBuffData && pBuffData->Buff_RemoveAftOffline)
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("error buff should not store in DB character: %d buff id: %d\n"),  char_id, ID);


	}
	SafeDeallocate(buf);
}

VOID Global::CheckValidBuffData()
{
	HRESULT hr = S_OK;
	if (DB_SUCCEEDED(hr = gTables.pCharacter->RSBuff()))
	{
		if (!FAILED(hr = gTables.pCharacter->MoveFirst()))
		{
			while (hr == S_OK)
			{
				ExtractBuffString(gTables.pCharacter->m_buff, gTables.pCharacter->m_char_id.LowPart);
				hr = gTables.pCharacter->MoveNext();
			}
		}
	}

	gTables.pCharacter->Close();
}

VOID Global::CheckValidMapData()
{
	HRESULT hr = S_OK;
	if (DB_SUCCEEDED(hr = gTables.pCharacter->RSMap()))
	{
		if (!FAILED(hr = gTables.pCharacter->MoveFirst()))
		{
			while (hr == S_OK)
			{
				MapDB* tempmap = const_cast<MapDB*>(m_resource->GetMapData(gTables.pCharacter->m_map_id));
				if (tempmap)
				{
					if (gTables.pCharacter->m_map_x >= tempmap->m_zonedata_header->width ||
						gTables.pCharacter->m_map_y >= tempmap->m_zonedata_header->height)
					{
						TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("MapData X Y Error: char_id %d, map_id %d, map_x %d, map_y %d"), 
							gTables.pCharacter->m_char_id.LowPart, gTables.pCharacter->m_map_id
							,  gTables.pCharacter->m_map_x
							,  gTables.pCharacter->m_map_y);
					}
				}
				else
				{
					TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("MapData not found Error: char_id %d, map_id %d, map_x %d, map_y %d"), 
							gTables.pCharacter->m_char_id.LowPart, gTables.pCharacter->m_map_id
							,  gTables.pCharacter->m_map_x
							,  gTables.pCharacter->m_map_y);
				}

				tempmap = const_cast<MapDB*>(m_resource->GetMapData(gTables.pCharacter->m_reborn_map));
				if (tempmap)
				{
					if (gTables.pCharacter->m_map_x >= tempmap->m_zonedata_header->width ||
						gTables.pCharacter->m_map_y >= tempmap->m_zonedata_header->height)
					{
						TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("MapData X Y Error: char_id %d, reborn map_id %d, reborn map_x %d, reborn map_y %d"), 
							gTables.pCharacter->m_char_id.LowPart, gTables.pCharacter->m_reborn_map
							,  gTables.pCharacter->m_reborn_x
							,  gTables.pCharacter->m_reborn_y);
					}
				}
				else
				{
					TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("MapData not found Error: char_id %d, reborn map_id %d, reborn map_x %d, reborn map_y %d"), 
							gTables.pCharacter->m_char_id.LowPart, gTables.pCharacter->m_reborn_map
							,  gTables.pCharacter->m_reborn_x
							,  gTables.pCharacter->m_reborn_y);
				}

				hr = gTables.pCharacter->MoveNext();
			}
		}
	}

	gTables.pCharacter->Close();
}

Boolean Global::ReleaseOLEDB()
{
	// release COM objects
	if (gTables.pAccount) {
		gTables.pAccount->Close();
		SafeDeleteObject(gTables.pAccount);		// don't use SafeDelete for COM object
	}

	if (gTables.pCharacter) {
		gTables.pCharacter->Close();
		SafeDeleteObject(gTables.pCharacter);		// don't use SafeDelete for COM object
	}

	if (gTables.pItem) {
		gTables.pItem->Close();
		SafeDeleteObject(gTables.pItem);		// don't use SafeDelete for COM object
	}

	if (gTables.pPartner) {
		gTables.pPartner->Close();
		SafeDeleteObject(gTables.pPartner);
	}

	if (gTables.pSkill) {
		gTables.pSkill->Close();
		SafeDeleteObject(gTables.pSkill);
	}

	if (gTables.pQuestLog) {
		gTables.pQuestLog->Close();
		SafeDeleteObject(gTables.pQuestLog);
	}

	if (gTables.pQuestFlag) {
		gTables.pQuestFlag->Close();
		SafeDeleteObject(gTables.pQuestFlag);
	}

	if (gTables.pQuestType) {
		gTables.pQuestType->Close();
		SafeDeleteObject(gTables.pQuestType);
	}

	if (gTables.pMessenger) {
		gTables.pMessenger->Close();
		SafeDeleteObject(gTables.pMessenger);
	}

	if (gTables.pChargeShop) {
		gTables.pChargeShop->Close();
		SafeDeleteObject(gTables.pChargeShop);
	}

	if (gTables.pPet) {
		gTables.pPet->Close();
		SafeDeleteObject(gTables.pPet);
	}

	if (gTables.pTitle) {
		gTables.pTitle->Close();
		SafeDeleteObject(gTables.pTitle);
	}
	if (gTables.pMsgCharacter) {
		gTables.pMsgCharacter->Close();
		SafeDeleteObject(gTables.pMsgCharacter);
	}
	if (gTables.pGuild) {
		gTables.pGuild->Close();
		SafeDeleteObject(gTables.pGuild);
	}
	if (gTables.pGuildMember) {
		gTables.pGuildMember->Close();
		SafeDeleteObject(gTables.pGuildMember);
	}
	if (gTables.pGift)
	{
		gTables.pGift->Close();
		SafeDeleteObject(gTables.pGift);
	}
	if (gTables.pHawkerHistory)
	{
		gTables.pHawkerHistory->Close();
		SafeDeleteObject(gTables.pHawkerHistory);
	}
	if (gTables.pPartnerCollection)
	{
		gTables.pPartnerCollection->Close();
		SafeDeleteObject(gTables.pPartnerCollection);
	}

	if (gTables.pSellYuanBao)
	{
		gTables.pSellYuanBao->Close();
		SafeDeleteObject(gTables.pSellYuanBao);
	}

	if (gTables.pSibling)
	{
		gTables.pSibling->Close();
		SafeDeleteObject(gTables.pSibling);
	}

	if (gTables.pMarriage)
	{
		gTables.pMarriage->Close();
		SafeDeleteObject(gTables.pMarriage);
	}

	if (gTables.pGeneration)
	{
		gTables.pGeneration->Close();
		SafeDeleteObject(gTables.pGeneration);
	}

	if (gTables.pQuestNpcFlag)
	{
		gTables.pQuestNpcFlag->Close();
		SafeDeleteObject(gTables.pQuestNpcFlag);
	}

	if (m_pAccountDb)
	{
		m_pAccountDb->Close();
		SafeDeleteObject(m_pAccountDb);
	}

	if (m_pGameDb)
	{
		m_pGameDb->Close();
		SafeDeleteObject(m_pGameDb);
	}

	gGlobal.acctDbPool.ClearAll();
	gGlobal.gameDbPool.ClearAll();

	//Database objects must be released before uninitialized the COM
	CoUninitialize();

	return (BOOLEAN_TRUE);
}

CDboSource* Global::CreateSource(Boolean bAccount)
{
	HRESULT hr;

	CDboSource* dbsrc = SafeCreateObject(CDboSource);
	TRACE_ENSURE(dbsrc);
	Database* pdb;
	if (bAccount)
		pdb = &gConfigure.dbAccount;
	else
		pdb = &gConfigure.dbGame;

#ifdef CONNECT_STR
	String cstr;
	cstr.Format(CONNECT_STR, pdb->strServer.c_str(), pdb->strDatabase.c_str(), 
		pdb->strLogin.c_str(), DBPWDecrypt(pdb->strPassword).c_str());
	if (DB_FAILED(hr = dbsrc->Open(cstr.c_str())))
#else
	if (DB_FAILED(hr = dbsrc->Open(pdb->strServer.c_str(), pdb->strDatabase.c_str(), 
		pdb->strLogin.c_str(), DBPWDecrypt(pdb->strPassword).c_str())))
#endif	// CONNECT_STR
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
			_FE("cannot connect to %s database, hr 0x%08X, is_account %d"),
			pdb->strDatabase.c_str(), hr, bAccount);
		dbsrc->ShowError(hr);
		SafeDeleteObject(dbsrc);
		dbsrc = NULL;
	}

/*
	// use old dbo source
	if (bAccount)
		hr = dbsrc->Open(m_pAccountDb);
	else
		hr = dbsrc->Open(m_pGameDb);
	if (DB_FAILED(hr))
		dbsrc->ShowError(hr);
*/

	return dbsrc;
}

void Global::ReleaseSource(CDboSource* dbsrc)
{
	if (dbsrc == NULL)
		return;

	dbsrc->Close();
	SafeDeleteObject(dbsrc);
}

void Global::EnqueueUpdate(DataObject *dobj, bool atNow)
{
	if (dobj == NULL || dobj->pobj == NULL) return;

	UpdateQueue::iterator it = std::find(m_updateQueue.begin(), m_updateQueue.end(), dobj);

	if (it != m_updateQueue.end()) {		// already exist
		if (atNow)
			dobj->updateTime = 0;
		return;
	}

	// at now or at update interval time /w 5 seconds random
	dobj->updateTime = (atNow ? 0 : 
		::GetTickCount() + gConfigure.updateInterval + GetRand(5000));
	m_updateQueue.push_back(dobj);
}

void Global::DequeueUpdate(DataObject *dobj)
{
	if (dobj == NULL) return;

	UpdateQueue::iterator it = std::find(m_updateQueue.begin(), m_updateQueue.end(), dobj);
	if (it != m_updateQueue.end())
		m_updateQueue.erase(it);
}

DWORD WINAPI QueueUpdateDb(LPVOID lpParam)
{
	ThreadGuard guard(gGlobal.m_updateKey);		// only allow one update thread
	UInt32 curTime = ::GetTickCount();
	UpdateRequest* pRequest = (UpdateRequest*) lpParam;
	TRACE_ENSURE(pRequest);
	UInt16 updateCount = (UInt16) pRequest->updateIds.size();
	UInt16 releaseCount = (UInt16) pRequest->releaseIds.size();
	UInt16 deleteCount = (UInt16) pRequest->deleteIds.size();
	CDboSource* acctDbSrc = NULL; // gGlobal.acctDbPool.GetAvailSource();
	CDboSource* gameDbSrc = NULL; // gGlobal.gameDbPool.GetAvailSource();
	NetObject *pobj;
	int pass = 0;
	UInt16 i = 0;
	UInt16 j = 0;
	UInt16 k = 0;
	CdboAccount*	dboAcct = NULL;
	CdboCharacter*	dboChar = NULL;
	CdboItem*		dboItem = NULL;
	CdboPartner*	dboPartner = NULL;
	CdboSkill*		dboSkill = NULL;
	CdboQuestLog*	dboQuestLog = NULL;
	CdboQuestType*	dboQuestType = NULL;
	CdboQuestFlag*	dboQuestFlag = NULL;
	CdboMessenger*	dboMessenger = NULL;
	CdboChargeShop*	dboChargeShop = NULL;
	CdboPet*		dboPet = NULL;
	CdboTitle*		dboTitle = NULL;
	CdboMsgCharacter* dboMsgChar = NULL;
	CdboGuild*		dboGuild = NULL;
	CdboGuildMember* dboGuildMember = NULL;
	CdboGift*		dboGift = NULL;
	CdboHawkerHistory* dboHawkHist = NULL;
	CdboPartnerCollection*	dboPartnerCollection = NULL;
	CdboSibling* dboSibling = NULL;
	CdboMarriage* dboMarriage = NULL;
	CdboGeneration* dboGeneration =NULL;
	CdboQuestNpcFlag* dboQuestNpcFlag = NULL;

	TRACE_INFODTL_4(GLOBAL_LOGGER, 
		_F("started, pRequest %d, updateCount %d, releaseCount %d, deleteCount %d"), 
		pRequest, updateCount, releaseCount, deleteCount);

	while (pass < 4)
	{
		bool valid = true;
		if (i < updateCount) 
		{
			pass = 1;
			pobj = FindNetObject(pRequest->updateIds[i]);
			i++;
		} 
		else if (j < releaseCount) 
		{
			pass = 2;
			pobj = FindNetObject(pRequest->releaseIds[j]);
			j++;
		} 
		else if (k < deleteCount) 
		{
			pass = 3;
			pobj = FindNetObject(pRequest->deleteIds[k]);
			k++;
		} 
		else 
		{
			break;
		}

		if (pobj == NULL)
			continue;

		switch (pobj->GetClassID())
		{
		case AccountData_CLASSID:
			{
				CAccountData* pAcct = CastNetObject<CAccountData>(pobj);
				if (pAcct) {
					if (dboAcct == NULL) {
						if (acctDbSrc == NULL) {
							acctDbSrc = gGlobal.acctDbPool.GetAvailSource();
							if (acctDbSrc == NULL)
								break;
						}
						dboAcct = SafeCreateObject(CdboAccount);
						dboAcct->SetDataSource(acctDbSrc);
					}
					if (pass < 3)
						pAcct->UpdateDbo(dboAcct);
					else
						pAcct->DeleteDbo(dboAcct);
				} else
					valid = false;	
			}
			break;

		case CharacterData_CLASSID:
			{
				CCharacterData* pchar = CastNetObject<CCharacterData>(pobj);
				if (pchar) {
					if (dboChar == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboChar = SafeCreateObject(CdboCharacter);
						dboChar->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pchar->UpdateDbo(dboChar);
					else
						pchar->DeleteDbo(dboChar);
				} else
					valid = false;	
			}
			break;

		case ItemData_CLASSID:
			{
				CItemData* pItem = CastNetObject<CItemData>(pobj);
				if (pItem) {
					if (dboItem == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboItem = SafeCreateObject(CdboItem);
						dboItem->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pItem->UpdateDbo(dboItem);
					else
						pItem->DeleteDbo(dboItem);
				} else
					valid = false;	
			}
			break;

		case PartnerData_CLASSID:
			{
				CPartnerData* pPartner = CastNetObject<CPartnerData>(pobj);
				if (pPartner) {
					if (dboPartner == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboPartner = SafeCreateObject(CdboPartner);
						dboPartner->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pPartner->UpdateDbo(dboPartner);
					else
						pPartner->DeleteDbo(dboPartner);
				} else
					valid = false;	
			}
			break;

		case SkillData_CLASSID:
			{
				CSkillData* pSkill = CastNetObject<CSkillData>(pobj);
				if (pSkill) {
					if (dboSkill == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboSkill = SafeCreateObject(CdboSkill);
						dboSkill->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pSkill->UpdateDbo(dboSkill);
					else
						pSkill->DeleteDbo(dboSkill);
				} else
					valid = false;	
			}
			break;

		case QuestLogData_CLASSID:
			{
				CQuestLogData* pQuestLog = CastNetObject<CQuestLogData>(pobj);
				if (pQuestLog) {
					if (dboQuestLog == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboQuestLog = SafeCreateObject(CdboQuestLog);
						dboQuestLog->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pQuestLog->UpdateDbo(dboQuestLog);
					else
						pQuestLog->DeleteDbo(dboQuestLog);
				} else
					valid = false;	
			}
			break;

		case QuestTypeData_CLASSID:
			{
				CQuestTypeData* pQuestType = CastNetObject<CQuestTypeData>(pobj);
				if (pQuestType) {
					if (dboQuestType == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboQuestType = SafeCreateObject(CdboQuestType);
						dboQuestType->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pQuestType->UpdateDbo(dboQuestType);
					else
						pQuestType->DeleteDbo(dboQuestType);
				} else
					valid = false;	
			}
			break;

		case QuestFlagData_CLASSID:
			{
				CQuestFlagData* pQuestFlag = CastNetObject<CQuestFlagData>(pobj);
				if (pQuestFlag) {
					if (dboQuestFlag == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboQuestFlag = SafeCreateObject(CdboQuestFlag);
						dboQuestFlag->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pQuestFlag->UpdateDbo(dboQuestFlag);
					else
						pQuestFlag->DeleteDbo(dboQuestFlag);
				} else
					valid = false;	
			}
			break;

		case MessengerData_CLASSID:
			{
				CMessengerData* pMessenger = CastNetObject<CMessengerData>(pobj);
				if (pMessenger) {
					if (dboMessenger == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboMessenger = SafeCreateObject(CdboMessenger);
						dboMessenger->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pMessenger->UpdateDbo(dboMessenger);
					else
						pMessenger->DeleteDbo(dboMessenger);
				} else
					valid = false;	
			}
			break;

		case ChargeShopData_CLASSID:
			{
				CChargeShopData* pChargeShop = CastNetObject<CChargeShopData>(pobj);
				if (pChargeShop) {
					if (dboChargeShop == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboChargeShop = SafeCreateObject(CdboChargeShop);
						dboChargeShop->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pChargeShop->UpdateDbo(dboChargeShop);
					else
						pChargeShop->DeleteDbo(dboChargeShop);
				} else
					valid = false;	
			}
			break;

		case PetData_CLASSID:
			{
				CPetData* pPet = CastNetObject<CPetData>(pobj);
				if (pPet) {
					if (dboPet == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboPet = SafeCreateObject(CdboPet);
						dboPet->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pPet->UpdateDbo(dboPet);
					else
						pPet->DeleteDbo(dboPet);
				} else
					valid = false;	
			}
			break;

		case TitleData_CLASSID:
			{
				CTitleData* pTitle = CastNetObject<CTitleData>(pobj);
				if (pTitle) {
					if (dboTitle == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboTitle = SafeCreateObject(CdboTitle);
						dboTitle->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pTitle->UpdateDbo(dboTitle);
					else
						pTitle->DeleteDbo(dboTitle);
				} else
					valid = false;	
			}
			break;

		case MsgCharacterData_CLASSID:
			{
				CMsgCharacterData* pMsgChar = CastNetObject<CMsgCharacterData>(pobj);
				if (pMsgChar) {
					if (dboMsgChar == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboMsgChar = SafeCreateObject(CdboMsgCharacter);
						dboMsgChar->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pMsgChar->UpdateDbo(dboMsgChar);
					else
						pMsgChar->DeleteDbo(dboMsgChar);
				} else
					valid = false;	
			}
			break;

		case GuildData_CLASSID:
			{
				CGuildData* pGuild = CastNetObject<CGuildData>(pobj);
				if (pGuild) {
					if (dboGuild == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboGuild = SafeCreateObject(CdboGuild);
						dboGuild->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pGuild->UpdateDbo(dboGuild);
					else
						pGuild->DeleteDbo(dboGuild);
				} else
					valid = false;	
			}
			break;

		case GuildMemberData_CLASSID:
			{
				CGuildMemberData* pGuildMember = CastNetObject<CGuildMemberData>(pobj);
				if (pGuildMember) {
					if (dboGuildMember == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboGuildMember = SafeCreateObject(CdboGuildMember);
						dboGuildMember->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pGuildMember->UpdateDbo(dboGuildMember);
					else
						pGuildMember->DeleteDbo(dboGuildMember);
				} else
					valid = false;	
			}
			break;

		case GiftData_CLASSID:
			{
				CGiftData* pGift = CastNetObject<CGiftData>(pobj);
				if (pGift) {
					if (dboGift == NULL) {
						if (acctDbSrc == NULL) {
							acctDbSrc = gGlobal.acctDbPool.GetAvailSource();
							if (acctDbSrc == NULL)
								break;
						}
						dboGift = SafeCreateObject(CdboGift);
						dboGift->SetDataSource(acctDbSrc);
					}
					if (pass < 3)
						pGift->UpdateDbo(dboGift);
					else
						pGift->DeleteDbo(dboGift);
				} else
					valid = false;	
			}
			break;

		case HawkerHistoryData_CLASSID:
			{
				CHawkerHistoryData* pHawkHist = CastNetObject<CHawkerHistoryData>(pobj);
				if (pHawkHist) {
					if (dboHawkHist == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboHawkHist = SafeCreateObject(CdboHawkerHistory);
						dboHawkHist->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pHawkHist->UpdateDbo(dboHawkHist);
					else
						pHawkHist->DeleteDbo(dboHawkHist);
				} else
					valid = false;	
			}
			break;

		case PartnerCollectionData_CLASSID:
			{
				CPartnerCollectionData* pPartnerCollection = CastNetObject<CPartnerCollectionData>(pobj);
				if (pPartnerCollection) {
					if (dboPartnerCollection == NULL) {
						if (gameDbSrc == NULL) {
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboPartnerCollection = SafeCreateObject(CdboPartnerCollection);
						dboPartnerCollection->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pPartnerCollection->UpdateDbo(dboPartnerCollection);
					else
						pPartnerCollection->DeleteDbo(dboPartnerCollection);
				} else
					valid = false;	
			}
			break;

		case SiblingData_CLASSID:
			{
				CSiblingData* pSibling = CastNetObject<CSiblingData>(pobj);
				if (pSibling) 
				{
					if (dboSibling == NULL) 
					{
						if (gameDbSrc == NULL) 
						{
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboSibling = SafeCreateObject(CdboSibling);
						dboSibling->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pSibling->UpdateDbo(dboSibling);
					else
						pSibling->DeleteDbo(dboSibling);
				} 
				else
					valid = false;	
			}
			break;
		case MarriageData_CLASSID:
			{
				CMarriageData* pMarriage = CastNetObject<CMarriageData>(pobj);
				if (pMarriage) 
				{
					if (dboMarriage == NULL) 
					{
						if (gameDbSrc == NULL) 
						{
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboMarriage = SafeCreateObject(CdboMarriage);
						dboMarriage->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pMarriage->UpdateDbo(dboMarriage);
					else
						pMarriage->DeleteDbo(dboMarriage);
				} 
				else
					valid = false;	
			}
			break;
		case GenerationData_CLASSID:
			{
				CGenerationData* pGeneration = CastNetObject<CGenerationData>(pobj);
				if (pGeneration) 
				{
					if (dboGeneration == NULL) 
					{
						if (gameDbSrc == NULL) 
						{
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboGeneration = SafeCreateObject(CdboGeneration);
						dboGeneration->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pGeneration->UpdateDbo(dboGeneration);
					else
						pGeneration->DeleteDbo(dboGeneration);
				} 
				else
					valid = false;	
			}
			break;
		case QuestNpcFlagData_CLASSID:
			{
				CQuestNpcFlagData* pQuestNpcFlagData = CastNetObject<CQuestNpcFlagData>(pobj);
				if (pQuestNpcFlagData) 
				{
					if (dboQuestNpcFlag == NULL) 
					{
						if (gameDbSrc == NULL) 
						{
							gameDbSrc = gGlobal.gameDbPool.GetAvailSource();
							if (gameDbSrc == NULL)
								break;
						}
						dboQuestNpcFlag = SafeCreateObject(CdboQuestNpcFlag);
						dboQuestNpcFlag->SetDataSource(gameDbSrc);
					}
					if (pass < 3)
						pQuestNpcFlagData->UpdateDbo(dboQuestNpcFlag);
					else
						pQuestNpcFlagData->DeleteDbo(dboQuestNpcFlag);
				} 
				else
					valid = false;	
			}
			break;
		default:
			TRACE_WARNDTL_2(GLOBAL_LOGGER, 
				_T("UpdateDb Warning: [invalid NetObject classId], classId %d, pobj %d"),
				pobj->GetClassID(), pobj);
		}

		if (!valid)
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("UpdateDb Warning: [invalid CastNetObject], classId %d, ID %d"), pobj->GetClassID(), pobj);
		if (pass > 1)
			DeleteNetObject(pobj);
	}
	SafeDeleteObject(dboAcct);
	SafeDeleteObject(dboChar);
	SafeDeleteObject(dboItem);
	SafeDeleteObject(dboPartner);
	SafeDeleteObject(dboSkill);
	SafeDeleteObject(dboQuestLog);
	SafeDeleteObject(dboQuestType);
	SafeDeleteObject(dboQuestFlag);
	SafeDeleteObject(dboMessenger);
	SafeDeleteObject(dboChargeShop);
	SafeDeleteObject(dboPet);
	SafeDeleteObject(dboTitle);
	SafeDeleteObject(dboMsgChar);
	SafeDeleteObject(dboGuild);
	SafeDeleteObject(dboGuildMember);
	SafeDeleteObject(dboGift);
	SafeDeleteObject(dboHawkHist);
	SafeDeleteObject(dboPartnerCollection);
	//SafeDeleteObject(dboSellYuanBao);
	SafeDeleteObject(dboSibling);
	SafeDeleteObject(dboMarriage);
	SafeDeleteObject(dboGeneration);
	SafeDeleteObject(dboQuestNpcFlag);

	if (acctDbSrc)
		gGlobal.acctDbPool.ReleaseSource(acctDbSrc);
	if (gameDbSrc)
		gGlobal.gameDbPool.ReleaseSource(gameDbSrc);

	// show update db time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
		TRACE_WARNDTL_4(GLOBAL_LOGGER, 
			_T("UpdateDb Profile: time %d, update %d, release %d, delete %d"), dt, 
			updateCount, releaseCount, deleteCount);

	TRACE_INFODTL_4(GLOBAL_LOGGER, 
		_F("ended, pRequest %d, updateCount %d, releaseCount %d, deleteCount %d"), 
		pRequest, updateCount, releaseCount, deleteCount);
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

void Global::UpdateDb(UInt32 curTime, Boolean bForce)
{
	UpdateRequest* pRequest = NULL;
	NetObject* pobj;
	UInt16 maxCount = gConfigure.updateCount;

	if (bForce)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("UpdateDb: force update"));
	}

	// get update or delete NetObject
	if (m_releaseGroup.GetCount() > 0 || m_deleteGroup.GetCount() > 0) {
		pRequest = SafeCreateObject(UpdateRequest);
		// must delete and release all objects for safety synchronization in DB
		for (UInt16 i = 0; i < m_deleteGroup.GetCount(); i++)
		{
			pobj = m_deleteGroup.GetChild(i);
			if (pobj)
			{
				pRequest->deleteIds.push_back(pobj->GetNetId());
				pRequest->idCount++;
			}
		}
		m_deleteGroup.Clear();			// remove all deleted NetObject
		for (UInt16 i = 0; i < m_releaseGroup.GetCount(); i++)
		{
			pobj = m_releaseGroup.GetChild(i);
			if (pobj)
			{
				pRequest->releaseIds.push_back(pobj->GetNetId());
				pRequest->idCount++;
			}
		}
		m_releaseGroup.Clear();			// remove all released NetObject
	}

	// updated objects within limitation to maintain object service performance
	if (!m_updateQueue.empty()) {
		DataObject *dobj = m_updateQueue.front();
		while (dobj && (bForce ||									// force update or 
			((pRequest == NULL || pRequest->idCount < maxCount) &&	// under max count and
			curTime >= dobj->updateTime)))							// reach update time
		{
			if (dobj->pobj)
			{
				if (pRequest == NULL)
					pRequest = SafeCreateObject(UpdateRequest);
				pRequest->updateIds.push_back(dobj->pobj->GetNetId());
				pRequest->idCount++;
				dobj->updateTime = UINT32_MAX_VALUE;		// reset update time
			}
			else
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, 
					_T("UpdateDb Warning: null NetObject for DataObject 0x%p"), dobj);
			}
			m_updateQueue.pop_front();
			if (m_updateQueue.empty())
				break;
			else
				dobj = m_updateQueue.front();		// next data object
		}
	}

	if (pRequest) {
		WorkQueueBegin();
#ifdef DB_THREAD_POOL
		if (bForce || !gConfigure.multiUpdate)
			QueueUpdateDb(pRequest);
		else
			QueueUserWorkItem(QueueUpdateDb, pRequest, DB_WORKER_FLAG);
#else
		QueueUpdateDb(pRequest);
#endif
	}
}

void Global::CB_QueueNetGroup(String funcName, HRESULT hr, String &dbErrMsg, RPCContext &context,
							  NetGroup *grp)
{
	if (pLocalPeer)
	{
		context.bCallback = BOOLEAN_FALSE;
		if (DB_SUCCEEDED(hr) || hr == DB_S_ENDOFROWSET)
		{
			Buffer* pBuf = SafeCreateObject(Buffer);
			Reuben::Network::RPCHeader header(RPC_ObjectService_CB_QueueNetGroup);
			pBuf->Write<Reuben::Network::RPCHeader>(header);
			pBuf->Write<RPCContext>(context);
			pBuf->Write<NetGroup*>(grp);
			pBuf->ResetCursor();
			Reuben::Network::DefaultMiddle::UpdateCRC(pBuf);
			LocalRPCManager::GetInstance().OnReceived(pLocalPeer, 
				&pLocalPeer->GetAddressRemote(), pBuf->AddRef());
		}
		else
		{
			String errMsg;

			errMsg.Format(_T("%s DB Error: %s"), funcName.c_str(), dbErrMsg.c_str());
			Buffer* pBuf = SafeCreateObject(Buffer);
			Reuben::Network::RPCHeader header(RPC_ObjectService_CB_Error);
			pBuf->Write<Reuben::Network::RPCHeader>(header);
			pBuf->Write<RPCContext>(context);
			pBuf->Write<UInt32>(hr);
			pBuf->Write<String>(errMsg);
			pBuf->ResetCursor();
			Reuben::Network::DefaultMiddle::UpdateCRC(pBuf);
			LocalRPCManager::GetInstance().OnReceived(pLocalPeer, 
				&pLocalPeer->GetAddressRemote(), pBuf->AddRef());
		}
	}

	// delete all temp objects
	NetObject *obj;
	for (UInt16 i = 0; i < grp->GetCount(); ++i) {
		obj = grp->GetChild(i);
		SafeDeleteObject(obj);
	}
	grp->Clear();
}

void Global::CacheKeepAcct(UInt32 acct_id)
{
	m_acctRelMap.erase(acct_id);
}

void Global::CacheReleaseAcct(UInt32 acct_id)
{
	ReleaseMap::iterator it = m_acctRelMap.find(acct_id);
	if (it == m_acctRelMap.end())
		m_acctRelMap[acct_id] = ::GetTickCount() + gConfigure.cacheTime;
}

void Global::CacheKeepChar(UInt32 char_id)
{
	m_charRelMap.erase(char_id);
}

void Global::CacheReleaseChar(UInt32 char_id)
{
	ReleaseMap::iterator it = m_charRelMap.find(char_id);
	if (it == m_charRelMap.end())
		m_charRelMap[char_id] = ::GetTickCount() + gConfigure.cacheTime;
}

void Global::CacheKeepMsgChar(UInt32 char_id)
{
	m_msgCharRelMap.erase(char_id);
}

void Global::CacheReleaseMsgChar(UInt32 char_id)
{
	ReleaseMap::iterator it = m_msgCharRelMap.find(char_id);
	if (it == m_msgCharRelMap.end())
		m_msgCharRelMap[char_id] = ::GetTickCount() + gConfigure.cacheTime;
}

void Global::CacheExtendMsgChar(UInt32 char_id)
{
	ReleaseMap::iterator it = m_msgCharRelMap.find(char_id);
	if (it != m_msgCharRelMap.end())
		it->second = ::GetTickCount() + gConfigure.cacheTime;
}

void Global::ReduceCache(UInt32 curTime)
{
	if (m_badNameTime <= curTime) {
		if (!badAcctNames.empty())
			badAcctNames.clear();
		m_badNameTime += gConfigure.cacheTime;
	}

	for (ReleaseMap::iterator it = m_acctRelMap.begin(); it != m_acctRelMap.end();)
	{
		if (it->second <= curTime) {
			ReleaseAccount(it->first);
			it = m_acctRelMap.erase(it);
		} else {
			++it;
		}
	}

	for (ReleaseMap::iterator it = m_charRelMap.begin(); it != m_charRelMap.end();)
	{
		if (it->second <= curTime) {
			ReleaseCharacter(it->first);
			it = m_charRelMap.erase(it);
		} else {
			++it;
		}
	}

	for (ReleaseMap::iterator it = m_msgCharRelMap.begin(); it != m_msgCharRelMap.end();)
	{
		if (it->second <= curTime) {
			ReleaseMsgChar(it->first);
			it = m_msgCharRelMap.erase(it);
		} else {
			++it;
		}
	}
}

void Global::WorkQueueBegin()
{
	::InterlockedIncrement(&workCount);
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("workCount %d"), workCount);
}

void Global::WorkQueueEnd()
{
	if (::InterlockedDecrement(&workCount) == 0)
		::SetEvent(completeEvent);
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("workCount %d"), workCount);
}

void Global::WorkQueueStop()
{
	// force update object in queue
	UpdateDb(::GetTickCount(), BOOLEAN_TRUE);
	m_updateQueue.clear();

	// wait for complete event
	WorkQueueEnd();									// treat as last queue end to complete
	::WaitForSingleObject(completeEvent, 10000);	// wait for 10 seconds to complete
	::CloseHandle(completeEvent);
}

void Global::AddUnreleasedId(UInt32 char_id, UInt16 element_id, CharToUInt16MapMap& unreleasedMap)
{
	CharToUInt16MapMap::iterator itr = unreleasedMap.find(char_id);
	if( itr!=unreleasedMap.end() )
	{
		UInt16Map& element_idMap = itr->second;
		element_idMap[element_id] = element_id;
	}else
	{
		UInt16Map element_idMap;
		element_idMap[element_id] = element_id;
		unreleasedMap[char_id] = element_idMap;
	}
}

UInt16 Global::PopUnreleasedId(UInt32 char_id, CharToUInt16MapMap& unreleasedMap)
{
	UInt16 popValue = 0;
	CharToUInt16MapMap::iterator itrParent = unreleasedMap.find(char_id);
	if( itrParent != unreleasedMap.end() )
	{
		UInt16Map& element_idMap = itrParent->second;
		UInt16Map::iterator itrChild = element_idMap.begin();
		if( itrChild != element_idMap.end() )
		{
			popValue = itrChild->second;
			element_idMap.erase(itrChild);
		}else
		{
			// CHILD has no element, parent should erase this MAP child
			unreleasedMap.erase(itrParent);
		}
	}
	return popValue;
}

void Global::ClearAllUnreleasedId(CharToUInt16MapMap& unreleasedMap)
{
	for( CharToUInt16MapMap::iterator itr = unreleasedMap.begin();
		itr != unreleasedMap.end(); ++itr )
	{
		itr->second.clear();
	}
	unreleasedMap.clear();
}

void Global::AddUnreleasedTypeId(UInt32 char_id, UInt16 type_id)
{
	AddUnreleasedId(char_id, type_id, m_charToUnreleasedTypeId);
}

UInt16 Global::PopUnreleasedTypeId(UInt32 char_id)
{
	return PopUnreleasedId(char_id, m_charToUnreleasedTypeId);
}

void Global::ClearAllUnreleasedTypeId()
{
	ClearAllUnreleasedId(m_charToUnreleasedTypeId);
}

void Global::AddUnreleasedFlagId(UInt32 char_id, UInt16 flag_id)
{
	AddUnreleasedId(char_id, flag_id, m_charToUnreleasedFlagId);
}

UInt16 Global::PopUnreleasedFlagId(UInt32 char_id)
{
	return PopUnreleasedId(char_id, m_charToUnreleasedFlagId);
}

void Global::ClearAllUnreleasedFlagId()
{
	ClearAllUnreleasedId(m_charToUnreleasedFlagId);
}

/////////////////////////////////

void TimeConvert(const DBTIMESTAMP &fromTime, time_t &toTime)
{
	tm tmTime;

	tmTime.tm_year = fromTime.year - 1900;
	tmTime.tm_mon = fromTime.month - 1;
	tmTime.tm_mday = fromTime.day;
	tmTime.tm_hour = fromTime.hour;
	tmTime.tm_min = fromTime.minute;
	tmTime.tm_sec = fromTime.second;
	toTime = mktime(&tmTime);

	if (toTime == UINT32_MAX_VALUE)
		toTime = 0;
};

void TimeConvert(const time_t &fromTime, DBTIMESTAMP &toTime)
{
	if(fromTime < 0)
	{
		TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("fromTime < 0 fromTime %d"), fromTime);
		return;
	}
	struct tm *newtime = localtime(&fromTime);
	toTime.year		= newtime->tm_year + 1900;
	toTime.month	= newtime->tm_mon + 1;
	toTime.day		= newtime->tm_mday;
	toTime.hour		= newtime->tm_hour;
	toTime.minute	= newtime->tm_min;
	toTime.second	= newtime->tm_sec;
	toTime.fraction	= 0;
};
