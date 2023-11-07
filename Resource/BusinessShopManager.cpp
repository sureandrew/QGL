//-- Common
#include "Common.h"
//-- Self
#include "Resource/BusinessShopManager.h"
//-- Library
#include "Reuben/Platform/BuffReader.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResBusinessShop.h"

CBusinessShopManager::CBusinessShopManager()
{
	m_BusinessShop.clear();
}

CBusinessShopManager::~CBusinessShopManager()
{
	Release();
}

void CBusinessShopManager::Release()
{
	for (BusinessShopDataMap::iterator itr = m_BusinessShop.begin(); itr != m_BusinessShop.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}

	m_BusinessShop.clear();

}

BOOL CBusinessShopManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: BusinessShopManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		BusinessShopData* pBusinessShopData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pBusinessShopData = SafeCreateObject(BusinessShopData);
				
				reader.Read(_T("shop_id"),		pBusinessShopData->shop_id);
				reader.Read(_T("Sellable"),		pBusinessShopData->sell);
				reader.Read(_T("ShopName"),		pBusinessShopData->shopName);
				reader.Read(_T("NumOfItem"),	pBusinessShopData->numOfItem);
				UInt32 tempid(0);
				UInt16 tempNum(0);
				String sztemp("");

				for ( UInt16 i = 1; i <= pBusinessShopData->numOfItem; ++i )
				{
					sztemp.Format(_T("Item%02d"), i);
					reader.Read(sztemp, tempid);
					pBusinessShopData->item_idLst.push_back( tempid );
					sztemp.Format(_T("MinPrice%02d"), i);
					reader.Read(sztemp, tempid);
					pBusinessShopData->minPriceLst.push_back( tempid );
					sztemp.Format(_T("MaxPrice%02d"), i);
					reader.Read(sztemp, tempid);
					pBusinessShopData->maxPriceLst.push_back( tempid );
					sztemp.Format(_T("Count%02d"), i);
					reader.Read(sztemp, tempNum);
					pBusinessShopData->countLst.push_back( tempNum );
				}
				BusinessShopDataMap::iterator Itr = m_BusinessShop.find(pBusinessShopData->shop_id);
				if (Itr != m_BusinessShop.end())
				{
					SafeDeleteObject(Itr->second);
					m_BusinessShop.erase(Itr);
				}
				m_BusinessShop.insert(std::make_pair(pBusinessShopData->shop_id, pBusinessShopData));
				
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: BusinessShopManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;	
}


const BusinessShopData* CBusinessShopManager::GetBusinessShopData(UInt32 shop_id)
{
	BusinessShopDataMap::iterator itr = m_BusinessShop.find(shop_id);
	if (itr != m_BusinessShop.end())
		return itr->second;
	else
		return NULL;
}