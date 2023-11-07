//-- Common
#include "Common.h"
#include "Common/common_binary.h"
//-- Self
#include "Resource/SkillShopManager.h"
//-- Library
#include <Reuben/Platform/BuffReader.h>
#include "Resource/ResSkillShop.h"
#include "Resource/QGLCSVTReader.h"

#define SKILL_DATA_VER		1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

////////////// Buff Manager //////////////////////////////////

CSkillShopManager::CSkillShopManager()
{
	m_skillShops.clear();
}

CSkillShopManager::~CSkillShopManager()
{
	Release();
}

VOID CSkillShopManager::Release()
{
	for (SkillShopMap::iterator it = m_skillShops.begin(); it != m_skillShops.end(); it++)
		SafeDeleteObject(it->second);
	m_skillShops.clear();
}

const SkillShopData* CSkillShopManager::GetSkillShop(UInt32 skillShop_id)
{
	SkillShopMap::iterator it = m_skillShops.find(skillShop_id);
	if( it!=m_skillShops.end() )
		return it->second;
	else
		return NULL;
}

BOOL CSkillShopManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		SkillShopData* pSkillShop = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				TCHAR buf[512];

				pSkillShop = SafeCreateObject(SkillShopData);
				reader.Read(_T("SkillShopID"),		pSkillShop->id);
				
				TCHAR *delim = _T("|");
				TCHAR *cmd = NULL; 

				reader.ReadString(_T("SkillID"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					if (value != 0)
						pSkillShop->SkillShopIDLst.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				reader.Read(_T("SkillBaseType"),		pSkillShop->SkillBaseType);
				reader.Read(_T("UseSmoney"),		pSkillShop->UseSmoney);
				reader.Read(_T("UseMoney"),		pSkillShop->UseMoney);
				reader.Read(_T("UseEXP"),		pSkillShop->UseEXP);
				reader.Read(_T("UseHonor"),		pSkillShop->UseHonor);

				reader.Read(_T("UseDonateFaction"),		pSkillShop->UseDonateFaction);
				reader.Read(_T("UseDonateManor"),		pSkillShop->UseDonateManor);
				reader.Read(_T("UseYuanBao"),		pSkillShop->UseYuanBao);
				
				m_skillShops.insert(std::make_pair(pSkillShop->id, pSkillShop));
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %i skillshop(s) read"), (UInt)m_skillShops.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: SkillShopManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CSkillShopManager::LoadDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			file.Read( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numBuff = 0;
			file.Read( (BYTE*)&numBuff, sizeof(UInt32) );

			SkillShopData* pSkillShop;
			for (UInt32 i = 0; i < numBuff; i++)
			{
				pSkillShop = SafeCreateObject(SkillShopData);
				file.Read( (BYTE*)pSkillShop, sizeof(SkillShopData) );
				m_skillShops[pSkillShop->id] = pSkillShop;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CSkillShopManager::LoadDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();

	return TRUE;
}

BOOL CSkillShopManager::SaveDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_WRITE);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			header.version = SKILL_DATA_VER;
			file.Write( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numBuff = (UInt32)m_skillShops.size();
			file.Write( (BYTE*)&numBuff, sizeof(UInt32) );

			for (SkillShopMap::iterator it = m_skillShops.begin(); it != m_skillShops.end(); it++)
			{
				file.Write( (BYTE*)it->second, sizeof(SkillShopData) );
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CSkillShopManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}

