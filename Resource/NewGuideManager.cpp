//-- Common
#include "Common.h"
//-- Self
#include "Resource/NewGuideManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/BuffReader.h>
#include "Resource/ResNewGuide.h"
#include "Resource/QGLCSVTReader.h"	

#define NEWBIE_DATA_VER 1
typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

CNewGuideManager::CNewGuideManager()
{
	m_NewGuideMap.clear();
}

CNewGuideManager::~CNewGuideManager()
{
	Release();
}

VOID CNewGuideManager::Release()
{
	for (NewGuideMap::iterator it = m_NewGuideMap.begin(); it != m_NewGuideMap.end(); ++it)
		SafeDeleteObject(it->second);
	m_NewGuideMap.clear();
}

const NewGuideData* CNewGuideManager::GetNewGuideData(UInt16 NewGuide_id)
{
	NewGuideMap::iterator it = m_NewGuideMap.find(NewGuide_id);
	if( it!=m_NewGuideMap.end() )
		return it->second;
	else
		return NULL;
}

BOOL CNewGuideManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _T("Err: NewGuideManager::LoadCSV: file error, %s"), filename);
	}

	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		NewGuideData* pNewGuideData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pNewGuideData = SafeCreateObject(NewGuideData);
				reader.Read(_T("Guide_ID"),		pNewGuideData->guide_id);					
				reader.Read(_T("Arr_Left"),		pNewGuideData->ArrLeft);
				reader.Read(_T("Arr_Left_X"),	pNewGuideData->ArrLeft_X);
				reader.Read(_T("Arr_Left_Y"),	pNewGuideData->ArrLeft_Y);
				reader.Read(_T("Arr_Right"),	pNewGuideData->ArrRight);
				reader.Read(_T("Arr_Right_X"),	pNewGuideData->ArrRight_X);
				reader.Read(_T("Arr_Right_Y"),	pNewGuideData->ArrRight_Y);
				reader.Read(_T("Arr_Down"),		pNewGuideData->ArrDown);
				reader.Read(_T("Arr_Down_X"),	pNewGuideData->ArrDown_X);
				reader.Read(_T("Arr_Down_Y"),	pNewGuideData->ArrDown_Y);
				reader.Read(_T("Arr_Left02"),  pNewGuideData->ArrLeft02);
				reader.Read(_T("Arr_Left02_X"),	pNewGuideData->ArrLeft02_X);
				reader.Read(_T("Arr_Left02_Y"),	pNewGuideData->ArrLeft02_Y);
				reader.ReadString(_T("Img1_Name"),	pNewGuideData->img1_name, 32);
				reader.Read(_T("Img1_X"),		pNewGuideData->img1_X);
				reader.Read(_T("Img1_Y"),		pNewGuideData->img1_Y);
				reader.ReadString(_T("Img2_Name"),	pNewGuideData->img2_name, 32);
				reader.Read(_T("Img2_X"),		pNewGuideData->img2_X);
				reader.Read(_T("Img2_Y"),		pNewGuideData->img2_Y);
				reader.ReadString(_T("Img3_Name"),	pNewGuideData->img3_name, 32);
				reader.Read(_T("Img3_X"),		pNewGuideData->img3_X);
				reader.Read(_T("Img3_Y"),		pNewGuideData->img3_Y);
				reader.ReadString(_T("Img4_Name"),	pNewGuideData->img4_name, 32);
				reader.Read(_T("Img4_X"),		pNewGuideData->img4_X);
				reader.Read(_T("Img4_Y"),		pNewGuideData->img4_Y);
				reader.ReadString(_T("Img5_Name"),	pNewGuideData->img5_name, 32);
				reader.Read(_T("Img5_X"),		pNewGuideData->img5_X);
				reader.Read(_T("Img5_Y"),		pNewGuideData->img5_Y);
				reader.ReadString(_T("Context"),	pNewGuideData->context, 512);
	
				NewGuideMap::iterator Itr = m_NewGuideMap.find(pNewGuideData->guide_id);

				if (Itr != m_NewGuideMap.end())
				{
					SafeDeleteObject(Itr->second);
					m_NewGuideMap.erase(Itr);
				}
				m_NewGuideMap.insert(std::make_pair(pNewGuideData->guide_id, pNewGuideData));
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _T("Initialization: %i newGuide(s) read"), (UInt)m_NewGuideMap.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _T("Err: NewGuideManager::LoadCSV: CSV reading error due to \
											   column format or missing MoveNext(), %s"), filename);
			reader.Close();
			if (pNewGuideData)
				SafeDeleteObject(pNewGuideData);
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
}

BOOL CNewGuideManager::LoadDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			file.Read( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numNewGuideData = 0;
			file.Read( (BYTE*)&numNewGuideData, sizeof(UInt32) );

			NewGuideData* pNewGuideData;
			for (UInt32 i = 0; i < numNewGuideData; ++i)
			{
				pNewGuideData = SafeCreateObject(NewGuideData);
				file.Read( (BYTE*)pNewGuideData, sizeof(NewGuideData) );
				m_NewGuideMap[pNewGuideData->guide_id] = pNewGuideData;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _T("Err: CNewGuideManager::LoadDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();

	return TRUE;
}

BOOL CNewGuideManager::SaveDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_WRITE);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			header.version = NEWBIE_DATA_VER;
			file.Write( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numNewGuide = (UInt32)m_NewGuideMap.size();
			file.Write( (BYTE*)&numNewGuide, sizeof(UInt32) );

			for (NewGuideMap::iterator it = m_NewGuideMap.begin(); it != m_NewGuideMap.end(); ++it)
			{
				file.Write( (BYTE*)it->second, sizeof(NewGuideMap) );
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _T("Err: CNewGuideManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}
