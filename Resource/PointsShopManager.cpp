#include "Common.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResPointsShop.h"
#include "Resource/PointsShopManager.h"

#define POINTS_SHOP_ITEM_VERSION 1

int CPointsShopManager::LoadCSV(const TCHAR *szFileName, const BYTE* pData, UInt32 pDataLen)
{
	if (NULL == szFileName) {
		return POINTS_SHOP_PARAM;
	}

	CQGLCSVTReader reader;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(szFileName, true) )
		openSrcSucceeded = TRUE;

	if( openSrcSucceeded )
	{
		POINTSSHOPITEM* pItem = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pItem = SafeCreateObject(POINTSSHOPITEM);
				reader.Read(_T("Index"),pItem->uiIndex);
				reader.Read(_T("ItemId"),pItem->uiItemId);
				reader.Read(_T("AccCost"),pItem->uiPointsCost);
				m_mapPointsShopItems.insert(std::make_pair(pItem->uiIndex,pItem));
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CPointsShopManager::LoadCSV: CSV reading error due to \
											   column format or missing MoveNext(), %s"), szFileName);
			reader.Close();
			return POINTS_SHOP_FILE_CONTEXT_ERROR;
		}
		reader.Close();
	}
	return POINTS_SHOP_OK;
}
VOID CPointsShopManager::Release()
{
	for (PointsShopMap::iterator it = m_mapPointsShopItems.begin(); it != m_mapPointsShopItems.end(); it++)
		SafeDeleteObject(it->second);
	m_mapPointsShopItems.clear();
}
POINTSSHOPITEM* CPointsShopManager::GetData(UInt32 uiItemID)
{
	for (PointsShopMap::iterator it = m_mapPointsShopItems.begin(); it != m_mapPointsShopItems.end(); it++)
	{
		POINTSSHOPITEM* pItem = it->second;
		if (!pItem) continue;

		if (pItem->uiItemId == uiItemID)
		{
			return pItem;
		}
	}

	return NULL;
}
size_t CPointsShopManager::GetSize()
{
	return m_mapPointsShopItems.size();
}