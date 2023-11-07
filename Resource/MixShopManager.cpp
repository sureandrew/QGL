//-- Common
#include "Common.h"
//-- Self
#include "Resource/MixShopManager.h"
//-- Library
#include "Reuben/Platform/BuffReader.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResMixShop.h"

#define CONFIGURATION_INI_FILE		_T(".\\Config\\Setting.ini")
#define FILENAME_LENGTH				1024

CMixShopManager::CMixShopManager()
{
	m_mixShop.clear();
	m_mixProb.clear();
	m_mixStone.clear();
	m_fileItemMix.clear();
}

CMixShopManager::~CMixShopManager()
{
	Release();
}

void CMixShopManager::Release()
{
	for (MixShopMap::iterator itr = m_mixShop.begin(); itr != m_mixShop.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}

	for (MixStoneProb::iterator itr = m_mixStone.begin(); itr != m_mixStone.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}

	m_mixShop.clear();
	m_mixStone.clear();

	for(FileItemMixMap::iterator itr = m_fileItemMix.begin(); itr != m_fileItemMix.end(); ++itr)
	{
		for (ItemMixMap::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
		{
			itr2->second->materials.clear();
			itr2->second->numOfMaterials.clear();
			itr2->second->output.clear();
			itr2->second->addon.clear();
			
			SafeDeleteObject(itr2->second);
		}	
		itr->second.clear();
	}
	m_fileItemMix.clear();

	m_mixProb.clear();
}

BOOL CMixShopManager::LoadMixShopCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(filename, true) )
		openSrcSucceeded = TRUE;
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMixShopManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		MixShopData* pMixShopData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pMixShopData = SafeCreateObject(MixShopData);
				reader.Read(_T("MixShop_ShopID"),	pMixShopData->shopID	);
				reader.Read(_T("MixShop_Type"),		pMixShopData->type		);
				reader.ReadString(_T("MixShop_ShopName"),	pMixShopData->shopName, 20);
				reader.Read(_T("MixShop_skillLv_low"),	pMixShopData->skilllvl_low	);
				reader.Read(_T("MixShop_skillLv_up"),	pMixShopData->skilllvl_up	);
				reader.ReadString(_T("MixShop_MixItemTable"),	pMixShopData->mixItemTable, 30);
				reader.Read(_T("MixShop_SlotSwitch01"),	pMixShopData->slotSwitch01	);
				reader.Read(_T("MixShop_SlotItemType01"),	pMixShopData->slotItemType01	);
				reader.Read(_T("MixShop_SlotItemID01"),		pMixShopData->slotItemID01	);
				reader.ReadString(_T("MixShop_SlotName01"),	pMixShopData->slotItemName01, 20);

				reader.Read(_T("MixShop_SlotSwitch02"),		pMixShopData->slotSwitch02	);
				reader.Read(_T("MixShop_SlotItemType02"),	pMixShopData->slotItemType02		);
				reader.Read(_T("MixShop_SlotItemID02"),		pMixShopData->slotItemID02);
				reader.ReadString(_T("MixShop_SlotName02"),	pMixShopData->slotItemName02, 20);

				reader.Read(_T("MixShop_SlotSwitch03"),		pMixShopData->slotSwitch03	);
				reader.Read(_T("MixShop_SlotItemType03"),	pMixShopData->slotItemType03		);
				reader.Read(_T("MixShop_SlotItemID03"),		pMixShopData->slotItemID03);
				reader.ReadString(_T("MixShop_SlotName03"),	pMixShopData->slotItemName03, 20);

				reader.Read(_T("MixShop_SlotSwitch04"),		pMixShopData->slotSwitch04	);
				reader.Read(_T("MixShop_SlotItemType04"),	pMixShopData->slotItemType04);
				reader.Read(_T("MixShop_SlotItemID04"),		pMixShopData->slotItemID04);
				reader.ReadString(_T("MixShop_SlotName04"),	pMixShopData->slotItemName04, 20);

				reader.Read(_T("MixShop_SlotSwitch05"),		pMixShopData->slotSwitch05);
				reader.Read(_T("MixShop_SlotItemType05"),	pMixShopData->slotItemType05 );
				reader.Read(_T("MixShop_SlotItemID05"),		pMixShopData->slotItemID05);
				reader.ReadString(_T("MixShop_SlotName05"),	pMixShopData->slotItemName05, 20);

				reader.Read(_T("MixShop_SlotSwitch06"),		pMixShopData->slotSwitch06	);
				reader.Read(_T("MixShop_SlotItemType06"),	pMixShopData->slotItemType06		);
				reader.Read(_T("MixShop_SlotItemID06"),		pMixShopData->slotItemID06);
				reader.ReadString(_T("MixShop_SlotName06"),	pMixShopData->slotItemName06, 20);

				reader.ReadString(_T("MixShop_Suggestion"),	pMixShopData->suggestion, 100);
				reader.Read(_T("MixShop_ConsumeTime"),		pMixShopData->consumeTime	);
				reader.ReadString(_T("MixShop_Lua"),	pMixShopData->lua, 20);

				UInt8 temp8 = 0;
				UInt32 temp32 = 0;
				reader.Read(_T("MixShop_ConsumeType01"),	temp8	);
				reader.Read(_T("MixShop_ConsumeAmount01"),	temp32	);
				pMixShopData->consumeType.push_back(temp8);
				pMixShopData->consumeAmount.push_back(temp32);
				
				reader.Read(_T("MixShop_ConsumeType02"),	temp8	);
				reader.Read(_T("MixShop_ConsumeAmount02"),	temp32  );
				pMixShopData->consumeType.push_back(temp8);
				pMixShopData->consumeAmount.push_back(temp32);
				
				reader.Read(_T("MixShop_ConsumeType03"),	temp8	);
				reader.Read(_T("MixShop_ConsumeAmount03"),	temp32	);
				pMixShopData->consumeType.push_back(temp8);
				pMixShopData->consumeAmount.push_back(temp32);
				
				reader.Read(_T("MixShop_ConsumeType04"),	temp8	);
				reader.Read(_T("MixShop_ConsumeAmount04"),	temp32	);
				pMixShopData->consumeType.push_back(temp8);
				pMixShopData->consumeAmount.push_back(temp32);

				MixShopMap::iterator Itr = m_mixShop.find(pMixShopData->shopID);
				if (Itr != m_mixShop.end())
				{
					SafeDeleteObject(Itr->second);
					m_mixShop.erase(Itr);
				}
				m_mixShop.insert(std::make_pair(pMixShopData->shopID, pMixShopData));
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: MixShopManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
	
}

BOOL CMixShopManager::LoadAllItemMixCSV()
{
	for (MixShopMap::iterator itr = m_mixShop.begin(); itr != m_mixShop.end(); ++itr)
	{
		BOOL success = LoadItemMixCSV(itr->second->mixItemTable);
		if (!success)
			return FALSE;
	}
	return TRUE;
}

void CMixShopManager::GetAllItemMixFileName(StringVector& fileList)
{
	for (MixShopMap::iterator itr = m_mixShop.begin(); itr != m_mixShop.end(); ++itr)
	{
		fileList.push_back(String(itr->second->mixItemTable));
	}
}

BOOL CMixShopManager::LoadItemMixCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	String strFileName(filename);
	String itemMixpath_csv;

	TCHAR itemMixpath_csv_tchar[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("CSVSrcPath"), 
		_T(".\\Data\\CSV\\"), itemMixpath_csv_tchar, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	itemMixpath_csv.Format(_T("%s%s"), String(itemMixpath_csv_tchar).c_str(), strFileName.c_str());
	
	if (strFileName.Equals(_T("0"))) //no csv need to read
		return TRUE;

	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(itemMixpath_csv.c_str(), true) )
		openSrcSucceeded = TRUE;
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMixShopManager::LoadItemMixCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		ItemMixData* pItemMixData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pItemMixData = SafeCreateObject(ItemMixData);
				reader.Read(_T("Mix_ProductID"),	pItemMixData->productID);
				reader.ReadString(_T("Mix_ProductName"),	pItemMixData->productName, 20);
				reader.ReadString(_T("Mix_ProductDescription"),		pItemMixData->proDescription, 255);
				reader.ReadString(_T("Mix_ProductFunctionDes"),		pItemMixData->proFunctionDes, 255);
				reader.Read(_T("Mix_skillLv"),	pItemMixData->skilllvl		);
				reader.Read(_T("Mix_LiveSkllID"),	pItemMixData->liveSkillID		);
				reader.Read(_T("Mix_LiveSkllLV"),	pItemMixData->liveSkillLv		);
				reader.Read(_T("Mix_ProductType"),	pItemMixData->productType		);
				reader.Read(_T("Mix_Quality"),	pItemMixData->Quality		);

				UInt32 temp32 = 0;
				UInt8 temp8 = 0;
				pItemMixData->materials.clear();
				pItemMixData->numOfMaterials.clear();
				pItemMixData->addon.clear();
				pItemMixData->output.clear();

				reader.Read(_T("Mix_Materials01"),			temp32);
				pItemMixData->materials.push_back(temp32);

				reader.Read(_T("Mix_NumOfMaterials01"),		temp8);
				pItemMixData->numOfMaterials.push_back(temp8);

				reader.Read(_T("Mix_Materials01_Switch"),	temp8);
				pItemMixData->materialSwitch.push_back(temp8);

				reader.Read(_T("Mix_Materials02"),			temp32);
				pItemMixData->materials.push_back(temp32);

				reader.Read(_T("Mix_NumOfMaterials02"),		temp8);
				pItemMixData->numOfMaterials.push_back(temp8);

				reader.Read(_T("Mix_Materials02_Switch"),	temp8);
				pItemMixData->materialSwitch.push_back(temp8);

				reader.Read(_T("Mix_Materials03"),			temp32);
				pItemMixData->materials.push_back(temp32);

				reader.Read(_T("Mix_NumOfMaterials03"),		temp8);
				pItemMixData->numOfMaterials.push_back(temp8);

				reader.Read(_T("Mix_Materials03_Switch"),	temp8);
				pItemMixData->materialSwitch.push_back(temp8);

				reader.Read(_T("Mix_Materials04"),			temp32);
				pItemMixData->materials.push_back(temp32);

				reader.Read(_T("Mix_NumOfMaterials04"),		temp8);
				pItemMixData->numOfMaterials.push_back(temp8);

				reader.Read(_T("Mix_Materials04_Switch"),	temp8);
				pItemMixData->materialSwitch.push_back(temp8);

				reader.Read(_T("Mix_Materials05"),			temp32);
				pItemMixData->materials.push_back(temp32);

				reader.Read(_T("Mix_NumOfMaterials05"),		temp8);
				pItemMixData->numOfMaterials.push_back(temp8);

				reader.Read(_T("Mix_Materials05_Switch"),	temp8);
				pItemMixData->materialSwitch.push_back(temp8);

				reader.Read(_T("Mix_Materials06"),			temp32);
				pItemMixData->materials.push_back(temp32);

				reader.Read(_T("Mix_NumOfMaterials06"),		temp8);
				pItemMixData->numOfMaterials.push_back(temp8);

				reader.Read(_T("Mix_Materials06_Switch"),	temp8);
				pItemMixData->materialSwitch.push_back(temp8);

				reader.Read(_T("Mix_Materials07"),			temp32);
				pItemMixData->materials.push_back(temp32);

				reader.Read(_T("Mix_NumOfMaterials07"),		temp8);
				pItemMixData->numOfMaterials.push_back(temp8);

				reader.Read(_T("Mix_Materials07_Switch"),	temp8);
				pItemMixData->materialSwitch.push_back(temp8);

				reader.Read(_T("Mix_Materials08"),			temp32);
				pItemMixData->materials.push_back(temp32);

				reader.Read(_T("Mix_Materials08_Switch"),	temp8);
				pItemMixData->materialSwitch.push_back(temp8);

				reader.Read(_T("Mix_NumOfMaterials08"),		temp8);
				pItemMixData->numOfMaterials.push_back(temp8);
				
				reader.Read(_T("Mix_Output01"),			temp32);
				pItemMixData->output.push_back(temp32);

				reader.Read(_T("Mix_Output01_Number"),		temp8);
				pItemMixData->outputNum.push_back(temp8);

				reader.Read(_T("Mix_Output02"),			temp32);
				pItemMixData->output.push_back(temp32);
				
				reader.Read(_T("Mix_Output02_Number"),		temp8);
				pItemMixData->outputNum.push_back(temp8);

				reader.Read(_T("Mix_Output03"),			temp32);
				pItemMixData->output.push_back(temp32);

				reader.Read(_T("Mix_Output03_Number"),		temp8);
				pItemMixData->outputNum.push_back(temp8);

				reader.Read(_T("Mix_Output04"),			temp32);
				pItemMixData->output.push_back(temp32);

				reader.Read(_T("Mix_Output04_Number"),		temp8);
				pItemMixData->outputNum.push_back(temp8);

				reader.Read(_T("Mix_Output05"),			temp32);
				pItemMixData->output.push_back(temp32);

				reader.Read(_T("Mix_Output05_Number"),		temp8);
				pItemMixData->outputNum.push_back(temp8);

				reader.Read(_T("Mix_Output06_EpicWeapon01"),	temp32	);
				pItemMixData->output.push_back(temp32);

				reader.Read(_T("Mix_Output06_Number"),		temp8);
				pItemMixData->outputNum.push_back(temp8);

				reader.Read(_T("Mix_Output07_EpicWeapon02"),	temp32	);
				pItemMixData->output.push_back(temp32);

				reader.Read(_T("Mix_Output07_Number"),		temp8);
				pItemMixData->outputNum.push_back(temp8);
				
				reader.Read(_T("Mix_Addon01"),			temp32);
				pItemMixData->addon.push_back(temp32);

				reader.Read(_T("Mix_Addon02"),			temp32);
				pItemMixData->addon.push_back(temp32);

				reader.Read(_T("Mix_Addon03"),			temp32);
				pItemMixData->addon.push_back(temp32);

				reader.Read(_T("Mix_Addon04"),			temp32);
				pItemMixData->addon.push_back(temp32);

				reader.Read(_T("Mix_Addon05"),			temp32);
				pItemMixData->addon.push_back(temp32);

				reader.Read(_T("Mix_Addon06"),			temp32);
				pItemMixData->addon.push_back(temp32);

				reader.Read(_T("Mix_Addon07"),			temp32);
				pItemMixData->addon.push_back(temp32);

				reader.Read(_T("Mix_Addon08"),			temp32);
				pItemMixData->addon.push_back(temp32);

				reader.Read(_T("Mix_Addon09"),			temp32);
				pItemMixData->addon.push_back(temp32);

				reader.Read(_T("Mix_Addon10"),			temp32);
				pItemMixData->addon.push_back(temp32);

				reader.Read(_T("Mix_Addon_Base"),			pItemMixData->addon_Base);

				reader.Read(_T("Mix_ConsumeTime"),		pItemMixData->consumeTime);
				reader.ReadString(_T("Mix_Lua"),	pItemMixData->lua, 20);

				reader.Read(_T("Mix_ConsumeType01"),	temp8	);
				reader.Read(_T("Mix_ConsumeAmount01"),	temp32	);
				pItemMixData->consumeType.push_back(temp8);
				pItemMixData->consumeAmount.push_back(temp32);

				reader.Read(_T("Mix_ConsumeType02"),	temp8	);
				reader.Read(_T("Mix_ConsumeAmount02"),	temp32	);
				pItemMixData->consumeType.push_back(temp8);
				pItemMixData->consumeAmount.push_back(temp32);
				
				reader.Read(_T("Mix_ConsumeType03"),	temp8	);
				reader.Read(_T("Mix_ConsumeAmount03"),	temp32	);
				pItemMixData->consumeType.push_back(temp8);
				pItemMixData->consumeAmount.push_back(temp32);
				
				reader.Read(_T("Mix_ConsumeType04"),	temp8	);
				reader.Read(_T("Mix_ConsumeAmount04"),	temp32	);
				pItemMixData->consumeType.push_back(temp8);
				pItemMixData->consumeAmount.push_back(temp32);
				
				FileItemMixMap::iterator itr = m_fileItemMix.find(strFileName);
				if (itr != m_fileItemMix.end())
				{
					ItemMixMap::iterator itr2 = itr->second.find(pItemMixData->productID);
					if (itr2 != itr->second.end())
					{
						SafeDeleteObject(itr2->second);
						itr->second.erase(itr2);
					}
					itr->second.insert(std::make_pair(pItemMixData->productID, pItemMixData));
				}
				else
				{
					ItemMixMap itemMix;
					itemMix.clear();
					itemMix.insert(std::make_pair(pItemMixData->productID, pItemMixData));
					m_fileItemMix.insert(std::make_pair(strFileName, itemMix));
				}
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: MixShopManager::LoadItemMixCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
}

const MixShopData* CMixShopManager::GetMixShopData(UInt32 shopID)
{
	MixShopMap::iterator itr = m_mixShop.find(shopID);
	if (itr != m_mixShop.end())
		return itr->second;
	else
		return NULL;
}

void CMixShopManager::GetSuitableItemList(String filename, UInt16 skillLv_low, UInt16 skillLv_up, ItemMixDataList &itemList)
{
	FileItemMixMap::iterator itr = m_fileItemMix.find(filename);
	if (itr != m_fileItemMix.end())
	{
		for(ItemMixMap::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
		{
			if (itr2->second->skilllvl <= skillLv_up && itr2->second->skilllvl >= skillLv_low)
				itemList.push_back(itr2->second);
		}
	}
}

const ItemMixData* CMixShopManager::GetItemMixData(String filename, UInt32 productID)
{
	const ItemMixData* itemMix = NULL;
	FileItemMixMap::iterator itr = m_fileItemMix.find(filename);
	if (itr != m_fileItemMix.end())
	{
		ItemMixMap::iterator itr2 = itr->second.find(productID); 
		if (itr2 != itr->second.end())
		{
			itemMix = itr2->second;
		}
	}
	return itemMix;
}

BOOL CMixShopManager::LoadMixProbCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(filename, true) )
		openSrcSucceeded = TRUE;
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMixShopManager::LoadMixProbCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		
		try{
			while( !reader.IsLineEmpty()  )
			{
				UInt32 stoneID = 0;
				UInt8 tempProb = 0;
				UInt8Vector prob;
				prob.clear();

				reader.Read(_T("stone_id"),	stoneID);
				
				reader.Read(_T("fail"),		tempProb);
				prob.push_back(tempProb);
				
				reader.Read(_T("r0"),		tempProb);
				prob.push_back(tempProb);

				reader.Read(_T("r1"),		tempProb);
				prob.push_back(tempProb);

				reader.Read(_T("r2"),		tempProb);
				prob.push_back(tempProb);

				reader.Read(_T("r3"),		tempProb);
				prob.push_back(tempProb);

				reader.Read(_T("r4"),		tempProb);
				prob.push_back(tempProb);

				TCHAR *delim = _T("|");
				TCHAR *cmd = NULL; 
				TCHAR buf[512];
				TCHAR buf2[512];

				StoneMixData* stoneMix = NULL;//SafeCreateObject(StoneMixData);
				MixStoneProb::iterator itr1 = m_mixStone.find(stoneID);

				if (itr1 != m_mixStone.end())
				{
					stoneMix = itr1->second;
					stoneMix->stoneNum.clear();
					stoneMix->stoneProb.clear();
				}
				else
				{
					stoneMix = SafeCreateObject(StoneMixData);
					stoneMix->stoneNum.clear();
					stoneMix->stoneProb.clear();
					m_mixStone.insert(std::make_pair(stoneID, stoneMix));
				}

				reader.ReadString(_T("StoneMixNum"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt8 value = _ttoi(cmd);
					if (value != 0)
						stoneMix->stoneNum.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				reader.ReadString(_T("StoneMixProb"),	buf2, 511);
				cmd = _tcstok(buf2, delim);
				while (cmd != NULL)
				{
					UInt8 value = _ttoi(cmd);
					if (value != 0)
						stoneMix->stoneProb.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				reader.Read(_T("Result"),		stoneMix->successItemID);

				MixProb::iterator Itr = m_mixProb.find(stoneID);
				if (Itr != m_mixProb.end())
				{
					Itr->second.clear();
					m_mixProb.erase(Itr);
				}
				m_mixProb.insert(std::make_pair(stoneID, prob));
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: MixShopManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
	
}

void CMixShopManager::GetStoneProbList(UInt32 stoneID, UInt8Vector &probList)
{
	MixProb::iterator itr = m_mixProb.find(stoneID);
	if (itr != m_mixProb.end())
	{
		probList.resize(itr->second.size());
		std::copy(itr->second.begin(), itr->second.end(), probList.begin());	
	}
}

UInt CMixShopManager::GetMaxStoneMixNum(UInt32 stoneID)
{
	MixStoneProb::iterator itr = m_mixStone.find(stoneID);
	if (itr != m_mixStone.end() && itr->second->stoneNum.size() > 0)
	{
		UInt value = *max_element(itr->second->stoneNum.begin(), itr->second->stoneNum.end());
		return value;
	}
	return 0;
}

UInt CMixShopManager::GetMinStoneMixNum(UInt32 stoneID)
{
	MixStoneProb::iterator itr = m_mixStone.find(stoneID);
	if (itr != m_mixStone.end() && itr->second->stoneNum.size() > 0)
	{
		UInt value = *min_element(itr->second->stoneNum.begin(), itr->second->stoneNum.end());
		return value;
	}
	return 0;
}
 
UInt CMixShopManager::GetStoneMixProb(UInt32 stoneID, UInt stack)
{
	MixStoneProb::iterator itr = m_mixStone.find(stoneID);
	
	if (itr != m_mixStone.end() )
	{
		for (Index j = C_INDEX(0); j < itr->second->stoneNum.size(); ++j)
		{
			if (itr->second->stoneNum[j] == stack)
				return itr->second->stoneProb[j];
		}
	}
	return 0;
}

UInt CMixShopManager::GetResultStoneID(UInt32 stoneID)
{
	MixStoneProb::iterator itr = m_mixStone.find(stoneID);
	if (itr != m_mixStone.end() )
	{
		return itr->second->successItemID;
	}
	return 0;
}