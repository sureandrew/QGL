//-- Common
#include "Common.h"
//-- Self
#include "MobDescManager.h"
//-- Library
#include "Resource/QGLCSVTReader.h"

CMobDescManager::CMobDescManager()
{
	m_MobDescList.clear();
}

CMobDescManager::~CMobDescManager()
{
	Release();
}

VOID CMobDescManager::Release()
{
	for ( MobDescMap::iterator it = m_MobDescList.begin(); it != m_MobDescList.end(); ++it )
	{
		it->second.Empty();
	}
	m_MobDescList.clear();
}

String CMobDescManager::GetDesc(UInt32 ref_id)
{
	MobDescMap::iterator it = m_MobDescList.find(ref_id);
	if ( it != m_MobDescList.end() ) 
		return it->second;
	return _T("");
}

Boolean CMobDescManager::LoadCSV(LPCTSTR szPath, const BYTE* pData, UInt32 pDataLen)
{
	if ( !szPath ) return BOOLEAN_FALSE;

	Release();

	String srcfile(_T(""));
	srcfile.Format(_T("%s"), szPath);
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(srcfile.c_str(), true) )
		openSrcSucceeded = TRUE;
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMobDescManager::LoadCSV: file error, %s"), srcfile.c_str());
	}

	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{

		UInt32 ref_Id = 0;
		TCHAR* szBuff = SafeAllocate(TCHAR, 257);
		try{
			while( !reader.IsLineEmpty()  )
			{
				reader.Read(_T("id"),	ref_Id);
				if  (szBuff)
				{
					ZeroMemory(szBuff, sizeof(TCHAR) * 257);
					reader.ReadString(_T("description"), szBuff, 256);

					String desc(_T(""));
					desc.Format(_T("%s"), szBuff);
					m_MobDescList.insert(std::make_pair(ref_Id, desc));
				}

				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMobDescManager::LoadCSV: CSV reading error due to \
											column format or missing MoveNext() %d "), srcfile.c_str());
			reader.Close();
			SafeDeallocate(szBuff);
			return BOOLEAN_FALSE;
		}
		reader.Close();
		SafeDeallocate(szBuff);
	}
	return BOOLEAN_TRUE;
}