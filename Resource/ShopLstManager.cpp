//-- Common
#include "Common.h"
//-- Self
#include "ShopLstManager.h"
//-- Library
#include "Resource/ResShop.h"
#include "Resource/QGLCSVTReader.h"

CShopLstManager::CShopLstManager()
{
	m_ShopList.clear();
}


CShopLstManager::~CShopLstManager()
{
	Release();

}

VOID CShopLstManager::Release()
{
	for ( CSVShopTable::iterator itr = m_ShopList.begin(); itr != m_ShopList.end(); ++itr )
		SafeDeleteObject(itr->second);
	m_ShopList.clear();
}

BOOL CShopLstManager::LoadCSV(LPCTSTR szfilename)
{
	CQGLCSVTReader reader;
	String srcfile(_T(""));
	srcfile.Format(_T("%s"), szfilename);

	if( reader.Open(srcfile.c_str(), true) == false ) 
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CShopLstManager::LoadCSV: file error, %s"), srcfile.c_str());
		return FALSE;
	}


	try{
		while( !reader.IsLineEmpty()  )
		{
			ShopLstData* pNewShopInfo = SafeCreateObject(ShopLstData);
			if ( pNewShopInfo )
			{
				reader.Read(_T("shop_id"),	pNewShopInfo->ShopId);
				reader.Read(_T("ShopName"),	pNewShopInfo->ShopName);
				reader.Read(_T("CanSell"),	pNewShopInfo->CanSell);
				reader.Read(_T("CanRepair"), pNewShopInfo->CanRepair);
				reader.Read(_T("BuyDiscount"), pNewShopInfo->BuyDiscount);
				reader.Read(_T("SellDiscount"), pNewShopInfo->SellDiscount);
				reader.Read(_T("RepairDiscount"), pNewShopInfo->RepairDiscount);
				reader.Read(_T("ShopType"), pNewShopInfo->uiShopType);

				Int itemcount = 0;
				reader.Read(_T("NumOfItem"), itemcount);

				UInt32 tempid = 0;
				String sztemp("");

				for ( Int i = 1; i <= itemcount; ++i )
				{
					sztemp.Format(_T("Item%02d"), i);
					reader.Read(sztemp, tempid);
					pNewShopInfo->ItemLst.push_back( tempid );
				}

				m_ShopList.insert( std::make_pair(pNewShopInfo->ShopId, pNewShopInfo) );
			}
			reader.MoveNext();
		}
	}catch(...)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CShopLstManager::LoadCSV: CSV reading error due to \
										   column format or missing MoveNext() %d "), srcfile.c_str());
		reader.Close();
		return FALSE;
	}
	reader.Close();
	return TRUE;
}

CONST ShopLstData* CShopLstManager::GetShop(UInt32 shopId)
{
	CSVShopTable::iterator itr = m_ShopList.find(shopId);
	if ( itr != m_ShopList.end() )
		return (itr->second);
	return NULL;
}
