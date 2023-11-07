//-- Common
#include "Common.h"
//-- Self
#include "GameStringTable.h"
//-- Library
#include "QGLCSVTReader.h"



CGameStringTable::CGameStringTable()
{
}

CGameStringTable::~CGameStringTable()
{
	m_stringTable.clear();
}

#define MAX_TEXT_LEN 512

VOID CGameStringTable::Load(String filename)
{
	CQGLCSVTReader reader;
	TCHAR tmpchr[MAX_TEXT_LEN];
	TCHAR tmpchr2[MAX_TEXT_LEN];
	if( reader.Open(filename.c_str(), true) )
	{
		try{
			while( !reader.IsLineEmpty()  )
			{
				reader.ReadString(_T("MSG_KEY"), tmpchr, MAX_TEXT_LEN-1);
				reader.ReadString(_T("MSG_CONTENT"), tmpchr2, MAX_TEXT_LEN-1);
				m_stringTable.insert(std::make_pair(tmpchr, tmpchr2));
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("GameStringTable::Load Error (1) [CSV err due to column format or missing MoveNext()] filename=%s"), filename.c_str());
		}
		reader.Close();
	}else
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GameStringTable::Load Warning (2) [File cannot be opened] filename=%s"), filename.c_str());
	}
}

VOID CGameStringTable::LoadFromBuffer(BYTE* pData, UInt32 pDataLength)
{
	CQGLCSVTReader reader;
	TCHAR tmpchr[MAX_TEXT_LEN];
	TCHAR tmpchr2[MAX_TEXT_LEN];
	if( pDataLength > 0 )
	{
		if( reader.OpenBuf(pData, pDataLength, true) )
		{
			try{
				while( !reader.IsLineEmpty()  )
				{
					reader.ReadString(_T("MSG_KEY"), tmpchr, MAX_TEXT_LEN-1);
					reader.ReadString(_T("MSG_CONTENT"), tmpchr2, MAX_TEXT_LEN-1);
					m_stringTable.insert(std::make_pair(tmpchr, tmpchr2));
					reader.MoveNext();
				}
			}
			catch(...)
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("GameStringTable::LoadFromBuffer Error (1) [CSV err due to column format or missing MoveNext()] Length=%i"), pDataLength);
			}
			reader.Close();
		}else
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GameStringTable::LoadFromBuffer Warning (2) [CSV cannot be opened] Length=%i"), pDataLength);
		}
	}
}

PCSTRING CGameStringTable::Get(PCSTRING pMsgName)
{
	MapString::iterator itr = m_stringTable.find(pMsgName);
	if( itr!=m_stringTable.end() )
		return (itr->second.c_str());
	else
		return (pMsgName);
}