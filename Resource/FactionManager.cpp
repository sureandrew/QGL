//-- Common
#include "Common.h"
//-- Self
#include "Resource/FactionManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/BuffReader.h>
#include "Resource/ResFaction.h"
#include "Resource/QGLCSVTReader.h"

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

#define FACTION_DATA_VER		1
////////////// Skill Manager //////////////////////////////////

CFactionManager::CFactionManager()
{
	m_Factions.clear();
}

CFactionManager::~CFactionManager()
{
	Release();
}

VOID CFactionManager::Release()
{
	for (FactionMap::iterator it = m_Factions.begin(); it != m_Factions.end(); it++)
		SafeDeleteObject(it->second);
	m_Factions.clear();

}

const FactionData* CFactionManager::GetFaction(UInt16 Faction_id)
{
	FactionMap::iterator it = m_Factions.find(Faction_id);
	if( it!=m_Factions.end() )
		return it->second;
	else
		return NULL;
}


BOOL CFactionManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: SkillManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		FactionData* pFaction = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{

				pFaction = SafeCreateObject(FactionData);
				reader.Read(_T("Faction_id"),		pFaction->id);
				reader.ReadString(_T("Faction_Name"),	pFaction->Faction_Name, 19);
				reader.ReadString(_T("Faction_Logo"),	pFaction->Faction_Logo, 19);
				reader.ReadString(_T("Faction_class1"),	pFaction->Faction_class1, 9);
				reader.ReadString(_T("Faction_class2"),	pFaction->Faction_class2, 9);
				reader.ReadString(_T("Faction_class3"),	pFaction->Faction_class3, 9);
				reader.ReadString(_T("Faction_class4"),	pFaction->Faction_class4, 9);
				reader.ReadString(_T("Faction_class5"),	pFaction->Faction_class5, 9);
				reader.ReadString(_T("Faction_class6"),	pFaction->Faction_class6, 9);
				reader.ReadString(_T("Faction_Talent"),	pFaction->Faction_Talent, 129);
				reader.ReadString(_T("Faction_Description"),	pFaction->Faction_Description, 199);

				m_Factions.insert(std::make_pair(pFaction->id, pFaction));
				
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %i faction(s) read"), (UInt)m_Factions.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: FactionManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CFactionManager::LoadDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			file.Read( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numFaction = 0;
			file.Read( (BYTE*)&numFaction, sizeof(UInt32) );

			FactionData* pFaction;
			for (UInt32 i = 0; i < numFaction; i++)
			{
				pFaction = SafeCreateObject(FactionData);
				file.Read( (BYTE*)pFaction, sizeof(FactionData) );
				m_Factions[pFaction->id] = pFaction;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CFactionManager::LoadDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();

	return TRUE;
}

BOOL CFactionManager::SaveDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_WRITE);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			header.version = FACTION_DATA_VER;
			file.Write( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numFaction = (UInt32)m_Factions.size();
			file.Write( (BYTE*)&numFaction, sizeof(UInt32) );

			for (FactionMap::iterator it = m_Factions.begin(); it != m_Factions.end(); it++)
			{
				file.Write( (BYTE*)it->second, sizeof(FactionData) );
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CFactionManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}

