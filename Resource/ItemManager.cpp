//-- Common
#include "Common.h"
//-- Self
#include "Resource/ItemManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/BuffReader.h>
#include "Resource/ResItem.h"
#include "Resource/QGLCSVTReader.h"

#define ITEM_DATA_VER		1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

////////////// Item Manager //////////////////////////////////

CItemManager::CItemManager()
{
	m_items.clear();
}

CItemManager::~CItemManager()
{
	Release();
}

VOID CItemManager::Release()
{
	for (ItemMap::iterator it = m_items.begin(); it != m_items.end(); it++)
		SafeDeleteObject(it->second);
	m_items.clear();
}

const ItemData* CItemManager::GetItem(UInt32 item_id)
{
	ItemMap::iterator it = m_items.find(item_id);
	if( it!=m_items.end() )
		return it->second;
	else
		return NULL;
}

void CItemManager::GetItemList(ItemDataList &itemList, UInt8 filterType, UInt8 item_type)
{
	////VMProtectBegin; VM_START;
	for (ItemMap::iterator it = m_items.begin(); it != m_items.end(); it++) {
		ItemData* pItem = it->second;
		if (filterType == ITEM_FILTER_ALL)
			itemList.push_back(pItem);
		else if (filterType == ITEM_FILTER_LUA && (pItem->battleScript[0] != _T('\0') ||
			pItem->battleScript[0] != _T('0')))
			itemList.push_back(pItem);
		else if (filterType == ITEM_FILTER_TYPE && pItem->item_type == item_type)
			itemList.push_back(pItem);
	}
	////VMProtectEnd; VM_END; 
}

BOOL CItemManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(filename, true) )
		openSrcSucceeded = TRUE;
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		ItemMap::iterator it;
		ItemData* pItem = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pItem = SafeCreateObject(ItemData);
				reader.Read(_T("item_id"),		pItem->item_id);
				reader.Read(_T("item_type"),	pItem->item_type);
				reader.ReadString(_T("name"),	pItem->name, 10);
				reader.Read(_T("eqType"),		pItem->eqType);
				reader.Read(_T("color"),		pItem->color);
				reader.Read(_T("rare"),			pItem->rare);
				reader.Read(_T("root_id"),		pItem->root_id);
				reader.ReadString(_T("description"),	pItem->description, 64);
				reader.ReadString(_T("functionDes"),	pItem->functionDes, 64);
				reader.Read(_T("g_price"),		pItem->g_price);
				reader.Read(_T("m_price"),		pItem->m_price);
				reader.Read(_T("memberUse"),		pItem->memberUse);
				reader.ReadString(_T("script"),		pItem->script, 32);
				reader.ReadString(_T("script_battle"),		pItem->battleScript, 32);

				reader.Read(_T("item_unique"),		pItem->item_unique);
				reader.Read(_T("item_TimeLimit"),		pItem->item_TimeLimit);
				reader.Read(_T("item_TimeLimitType"),	pItem->item_TimeLimitType);

				reader.Read(_T("reqSexClass1"),		pItem->reqSexClass1);
				reader.Read(_T("reqSexClass2"),		pItem->reqSexClass2);
				reader.Read(_T("reqFaction"),	pItem->reqFaction);
				reader.Read(_T("reqLv"),		pItem->reqLv);
				reader.Read(_T("consume"),		pItem->consume);
				reader.Read(_T("questItem"),	pItem->questItem);
				reader.Read(_T("soulBound"),	pItem->soulBound);
				reader.Read(_T("dumpable"),		pItem->dumpable);
				reader.Read(_T("droppable"),	pItem->droppable);
				reader.Read(_T("godWeapon"),	pItem->godWeapon);
				reader.Read(_T("steal"),		pItem->steal);
				reader.Read(_T("useCount"),		pItem->useCount);
				reader.Read(_T("overlap"),		pItem->overlap);
				reader.Read(_T("usePlace"),		pItem->usePlace);
				reader.Read(_T("bBuff"),		pItem->bBuff);
				reader.Read(_T("form"),			pItem->form);
				reader.Read(_T("targetType"),	pItem->targetType);
				reader.Read(_T("target"),		pItem->target);
				reader.Read(_T("stateCaster"),	pItem->stateCaster);
				reader.Read(_T("stateTarget"),	pItem->stateTarget);
				reader.Read(_T("stateClear"),	pItem->stateClear);
				reader.Read(_T("coolType"),		pItem->coolType);
				reader.Read(_T("coolTime"),		pItem->coolTime);
				reader.Read(_T("periodType"),	pItem->periodType);
				reader.Read(_T("period"),		pItem->period);
				reader.Read(_T("nTarget"),		pItem->nTarget);
				reader.Read(_T("restoreType"),	pItem->restoreType);
				reader.Read(_T("restore_HP"),	pItem->restore_HP);
				reader.Read(_T("restore_mHP"),	pItem->restore_mHP);
				reader.Read(_T("restore_SP"),	pItem->restore_SP);
				reader.Read(_T("restore_mSP"),	pItem->restore_mSP);
				reader.Read(_T("restore_VP"),	pItem->restore_VP);
				reader.Read(_T("restore_DP"),	pItem->restore_DP);
				reader.Read(_T("restore_WC"),	pItem->restore_WC);
				reader.Read(_T("restore_favor"),	pItem->restore_favor);
				reader.Read(_T("bagSlot"),		pItem->bagSlot);
				reader.Read(_T("eq_baseElement"),	pItem->eq_baseElement);
				reader.Read(_T("eq_maxEnduring"),		pItem->eq_maxEndure);
				reader.Read(_T("eq_maxUpgrade"),	pItem->eq_maxUpgrade);
				reader.Read(_T("eq_minGem"),		pItem->eq_minGem);
				reader.Read(_T("eq_maxGem"),		pItem->eq_maxGem);
				reader.Read(_T("STA_d"),		pItem->STA_d);
				reader.Read(_T("STA_u"),		pItem->STA_u);
				reader.Read(_T("SPR_d"),		pItem->SPR_d);
				reader.Read(_T("SPR_u"),		pItem->SPR_u);
				reader.Read(_T("STR_d"),		pItem->STR_d);
				reader.Read(_T("STR_u"),		pItem->STR_u);
				reader.Read(_T("CON_d"),		pItem->CON_d);
				reader.Read(_T("CON_u"),		pItem->CON_u);
				reader.Read(_T("AGI_d"),		pItem->AGI_d);
				reader.Read(_T("AGI_u"),		pItem->AGI_u);
				reader.Read(_T("ATK_d"),		pItem->ATK_d);
				reader.Read(_T("ATK_u"),		pItem->ATK_u);
				reader.Read(_T("DEF_d"),		pItem->DEF_d);
				reader.Read(_T("DEF_u"),		pItem->DEF_u);
				reader.Read(_T("HIT_d"),		pItem->HIT_d);
				reader.Read(_T("HIT_u"),		pItem->HIT_u);
				reader.Read(_T("EVA_d"),		pItem->EVA_d);
				reader.Read(_T("EVA_u"),		pItem->EVA_u);
				reader.Read(_T("PWD_d"),		pItem->POW_d);
				reader.Read(_T("PWD_u"),		pItem->POW_u);
				reader.Read(_T("SPD_d"),		pItem->SPD_d);
				reader.Read(_T("SPD_u"),		pItem->SPD_u);
				reader.Read(_T("maxHP_d"),		pItem->maxHP_d);
				reader.Read(_T("maxHP_u"),		pItem->maxHP_u);
				reader.Read(_T("maxSP_d"),		pItem->maxSP_d);
				reader.Read(_T("maxSP_u"),		pItem->maxSP_u);
				reader.Read(_T("maxVP_d"),		pItem->maxVP_d);
				reader.Read(_T("maxVP_u"),		pItem->maxVP_u);
				reader.Read(_T("iconLight"),		pItem->iconLight);
				reader.Read(_T("weaponLight"),		pItem->weaponLight);
				reader.ReadString(_T("icon1"),		pItem->icon1, 32);
				reader.ReadString(_T("icon2"),		pItem->icon2, 32);
				reader.Read(_T("icon2_id"),		pItem->icon2_id);
				reader.ReadString(_T("eq_Outlook1"),		pItem->eq_Outlook1, 32);
				reader.ReadString(_T("eq_Outlook2"),		pItem->eq_Outlook2, 32);
				reader.Read(_T("eq_Color"),		pItem->eq_Color);
				reader.ReadString(_T("eq_colorPalette1"),		pItem->eq_colorPalette1, 32);
				reader.ReadString(_T("eq_colorPalette2"),		pItem->eq_colorPalette2, 32);
				reader.ReadString(_T("eq_colorPalette3"),		pItem->eq_colorPalette3, 32);
				reader.ReadString(_T("eq_colorPalette4"),		pItem->eq_colorPalette4, 32);
				reader.ReadString(_T("eq_colorPalette5"),		pItem->eq_colorPalette5, 32);
				reader.ReadString(_T("eqo1_atk1_sound"),		pItem->eqo1_atk1_sound, 32);
				reader.ReadString(_T("eqo1_atk2_sound"),		pItem->eqo1_atk2_sound, 32);
				reader.ReadString(_T("eqo2_atk1_sound"),		pItem->eqo2_atk1_sound, 32);
				reader.ReadString(_T("eqo2_atk2_sound"),		pItem->eqo2_atk2_sound, 32);

				reader.Read(_T("sellable"),		pItem->sellable);
				reader.Read(_T("repair"),		pItem->repair);
				reader.Read(_T("Identify"),		pItem->Identify);
				reader.Read(_T("item_GenerateLimit"),		pItem->item_GenerateLimit);
				reader.Read(_T("item_GenerateLimit_Time"),		pItem->item_GenerateLimit_Time);
				reader.Read(_T("item_GenerateLimit_Count"),		pItem->item_GenerateLimit_Count);
				
				it = m_items.find(pItem->item_id);
				if (it != m_items.end())
				{
					TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("ItemManager LoadCSV: item_id %d already loaded"), pItem->item_id);
					SafeDeleteObject(it->second);
					m_items.erase(it);
				}

				reader.ReadString(_T("item_AtkEffect"), pItem->effect_Atk, 100);
				reader.ReadString(_T("item_HurtEffect"), pItem->effect_Hurt, 100);
				reader.ReadString(_T("item_SparkEffect"), pItem->effect_Spark, 100);
				reader.ReadString(_T("item_AtkSound"), pItem->sound_Atk, 100);
				reader.ReadString(_T("item_HurtSound"), pItem->sound_Hurt, 100);
				reader.ReadString(_T("item_SparkSound"), pItem->sound_Spark, 100);

				reader.Read(_T("probability"),		pItem->probability);
				reader.Read(_T("itemset_id"),		pItem->itemset_id);
				reader.Read(_T("add_friendly"),		pItem->add_friendly);
				TCHAR buf[512];
				TCHAR *delim = _T("|");
				TCHAR *cmd = NULL; 
				reader.ReadString(_T("skill_id"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					pItem->skill_ids.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				m_items.insert(std::make_pair(pItem->item_id, pItem));
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %i item(s) read"), (UInt)m_items.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: ItemManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CItemManager::LoadDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			file.Read( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numItem = 0;
			file.Read( (BYTE*)&numItem, sizeof(UInt32) );

			ItemData* pItem;
			for (UInt32 i = 0; i < numItem; i++)
			{
				pItem = SafeCreateObject(ItemData);
				file.Read( (BYTE*)pItem, sizeof(ItemData) );
				m_items[pItem->item_id] = pItem;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CItemManager::LoadDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();

	return TRUE;
}

BOOL CItemManager::SaveDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_WRITE);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			header.version = ITEM_DATA_VER;
			file.Write( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numItem = (UInt32)m_items.size();
			file.Write( (BYTE*)&numItem, sizeof(UInt32) );

			for (ItemMap::iterator it = m_items.begin(); it != m_items.end(); it++)
			{
				file.Write( (BYTE*)it->second, sizeof(ItemData) );
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CItemManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}
