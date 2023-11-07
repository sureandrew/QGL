//-- Common
#include "Common.h"
//-- Self
#include "Resource/ChargeShopManager.h"
//-- Library
#include "Reuben/Platform/BuffReader.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResChargeShop.h"


//typedef Reuben::Platform::BuffReader File;
//typedef Reuben::Platform::FileException FileException;

CChargeShopManager::CChargeShopManager()
{
	m_ChargeShop.clear();
}

CChargeShopManager::~CChargeShopManager()
{
	Release();
}

void CChargeShopManager::Release()
{
	m_ChargeShopList.clear();
	for (ChargeShopMap::iterator itr = m_ChargeShop.begin(); itr != m_ChargeShop.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}

	m_ChargeShop.clear();

}

BOOL CChargeShopManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CChargeShopManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		ChargeShopData* pChargeShopData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pChargeShopData = SafeCreateObject(ChargeShopData);
				reader.Read(_T("ChargeShop_ID"),	pChargeShopData->chargeShop_ID);
				//ZeroMemory(pChargeShopData->chargeShop_ItemName, 20 * sizeof(TCHAR) );
				reader.ReadString(_T("ChargeShop_ItemName"),	pChargeShopData->chargeShop_ItemName, 20);
				reader.Read(_T("ChargeShop_Active"),			pChargeShopData->chargeShop_Active);
				reader.Read(_T("ChargeShop_Type"),			pChargeShopData->chargeShop_Type);
				reader.Read(_T("ChargeShop_ItemType"),			pChargeShopData->chargeShop_ItemType);
				reader.Read(_T("ChargeShop_ItemID"),			pChargeShopData->chargeShop_ItemID);
				reader.Read(_T("ChargeShop_LV"),			pChargeShopData->chargeShop_LV);
				reader.Read(_T("ChargeShop_PreviewType"),			pChargeShopData->chargeShop_PreviewType);			
				reader.Read(_T("ChargeShop_Mmoney"),		pChargeShopData->chargeShop_Mmoney);
				reader.Read(_T("ChargeShop_Discount"),		pChargeShopData->chargeShop_Discount);
				reader.Read(_T("ChargeShop_TotalStock"),		pChargeShopData->chargeShop_TotalStock);
				reader.Read(_T("ChargeShop_LimitType"),		pChargeShopData->chargeShop_LimitType);
				//ZeroMemory(pChargeShopData->chargeShop_StartTime, 14 * sizeof(TCHAR) );
				reader.ReadString(_T("ChargeShop_StartTime"),	pChargeShopData->chargeShop_StartTime, 14);
				//ZeroMemory(pChargeShopData->chargeShop_EndTime, 14 * sizeof(TCHAR) );
				reader.ReadString(_T("ChargeShop_EndTime"),	pChargeShopData->chargeShop_EndTime, 14);
				reader.Read(_T("ChargeShop_Restore"),		pChargeShopData->chargeShop_Restore);
				
				ChargeShopMap::iterator Itr = m_ChargeShop.find(pChargeShopData->chargeShop_ID);
				if (Itr != m_ChargeShop.end())
				{
					SafeDeleteObject(Itr->second);
					m_ChargeShop.erase(Itr);
				}
				m_ChargeShop.insert(std::make_pair(pChargeShopData->chargeShop_ID, pChargeShopData));
				m_ChargeShopList.push_back(pChargeShopData);
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: ChargeShopManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
	
}

const ChargeShopData* CChargeShopManager::GetChargeShopItem(UInt32 chargeShopID)
{
	ChargeShopMap::iterator itr = m_ChargeShop.find(chargeShopID);
	if (itr != m_ChargeShop.end())
		return itr->second;
	else
		return NULL;
}

UInt32 CChargeShopManager::GetTotalItemCount()
{
	return (UInt32)m_ChargeShop.size();
}

ChargeShopDataList CChargeShopManager::GetChargeShopList()
{
	return m_ChargeShopList;
}
