#include "Common.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "ResAccmulationShop.h"
#include "AccmulationShop.h"

#define ACC_SHOP_ITEM_VERSION 1

int CAccmulationShopManager::LoadCSV(const TCHAR *szFileName, const BYTE* pData, UInt32 pDataLen)
{
	if (NULL == szFileName) {
		return ACCMULATION_SHOP_PARAM;
	}

	CQGLCSVTReader reader;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(szFileName, true) )
		openSrcSucceeded = TRUE;

	if( openSrcSucceeded )
	{
		ACCSHOPITEM* pItem = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pItem = SafeCreateObject(ACCSHOPITEM);
				reader.Read(_T("Index"),pItem->uiIndex);
				reader.Read(_T("ItemId"),pItem->uiItemId);
				reader.Read(_T("AccCost"),pItem->uiAccmulationCost);
				m_mapAccShopItems.insert(std::make_pair(pItem->uiIndex,pItem));
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CAccmulationShopManager::LoadCSV: CSV reading error due to \
											   column format or missing MoveNext(), %s"), szFileName);
			reader.Close();
			return ACCMULATION_SHOP_FILE_CONTEXT_ERROR;
		}
		reader.Close();
	}
	return ACCMULATION_SHOP_OK;
}
VOID CAccmulationShopManager::Release()
{
	for (AccShopMap::iterator it = m_mapAccShopItems.begin(); it != m_mapAccShopItems.end(); it++)
		SafeDeleteObject(it->second);
	m_mapAccShopItems.clear();
}
ACCSHOPITEM* CAccmulationShopManager::GetAccmulationShopData(UInt32 uiIndex)
{
	AccShopMap::iterator it = m_mapAccShopItems.find(uiIndex);
	if( it!=m_mapAccShopItems.end() )
		return it->second;
	else
		return NULL;
}
size_t CAccmulationShopManager::GetCAccmulationShopDataSize()
{
	return m_mapAccShopItems.size();
}