//-- Common
//-- Common
#include "Common.h"
//-- Self
#include "AntiHackManager.h"
//-- Library
#include "Resource/ResHackImage.h"


#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <stdio.h>
#include "GDFManager.h"

#define CONFIGURATION_INI_FILE		_T(".\\Data\\Setting\\fileconfig.ini")
#define ANTIHACK_GDF_FILE			_T(".\\Data\\AntiHack\\AntiHack.gdf")

CAntiHackManager::CAntiHackManager()
{
	m_gdfManager = SafeCreateObject(GDFManager);
	String filename(ANTIHACK_GDF_FILE);
	m_gdfManager->LoadPatchFile(filename, GDFFILE_STATE_GAME);

	m_mapViewKey.clear();
	m_rand.Randomize();

	LoadData();
}

CAntiHackManager::~CAntiHackManager()
{
	Release();

	m_gdfManager->ClosePatchFile();
	SafeDeleteObject(m_gdfManager);
}

VOID CAntiHackManager::Release()
{

	for( MapViewKeyVector::iterator itr = m_mapViewKey.begin(); itr != m_mapViewKey.end(); ++itr )
		SafeDeleteObject(*itr);
	m_mapViewKey.clear();

}


BOOL CAntiHackManager::LoadData()
{
	UInt32 TotalImageNum = GetPrivateProfileInt(_T("AntiHackRes"), _T("TotalImage"), 40000, CONFIGURATION_INI_FILE);
	UInt32 gdfSize = 0;
	UInt32 gdfOffset = 0;

	for (UInt32 i = 0; i < TotalImageNum; ++i)
	{
	    // store positon and length of file in GDF
		String picFilename;
		picFilename.Format(_T("Pic%05d.czip"), i);
		GdfMapViewKey* key = SafeCreateObject(GdfMapViewKey);
		if (m_gdfManager->GetMapViewKeyOfFile(picFilename, *key))
            m_mapViewKey.push_back(key);
		else
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("antiHack key %d not found"), i);	
			return BOOLEAN_FALSE;
		}
	}
	return TRUE;
}


VOID CAntiHackManager::GetHackImageInfo(UInt32 picId, BYTE*& mapViewPtr, BYTE*& imagePtr, UInt32& imageSize)
{
	if (picId < m_mapViewKey.size())
	{
        GdfMapViewKey* gdfKey = m_mapViewKey[picId];
		UInt32 beginOffset = 0;
		if (gdfKey)
		{
			mapViewPtr	= m_gdfManager->GetMapViewByPosition(*gdfKey, beginOffset);
			imagePtr	= mapViewPtr + beginOffset;
			imageSize	= gdfKey->fileSize;
		}
		else
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("antiHack image %d not found"), picId);	
		}
	}
}

VOID CAntiHackManager::ReleaseHackImagePtr(BYTE* mapViewPtr)
{
	UnmapViewOfFile(mapViewPtr);
}

UInt32 CAntiHackManager::GetHackDataSize()
{
    return (UInt32)m_mapViewKey.size();
}
