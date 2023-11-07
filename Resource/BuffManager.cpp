//-- Common
#include "Common.h"
#include "Common/common_binary.h"
//-- Self
#include "Resource/BuffManager.h"
//-- Library
#include <Reuben/Platform/BuffReader.h>
#include "Resource/ResBuff.h"
#include "Resource/QGLCSVTReader.h"

#define SKILL_DATA_VER		1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

////////////// Buff Manager //////////////////////////////////

CBuffManager::CBuffManager()
{
	m_buffs.clear();
}

CBuffManager::~CBuffManager()
{
	Release();
}

VOID CBuffManager::Release()
{
	for (BuffMap::iterator it = m_buffs.begin(); it != m_buffs.end(); it++)
		SafeDeleteObject(it->second);
	m_buffs.clear();
}

const BuffData* CBuffManager::GetBuff(UInt32 buff_id)
{
	BuffMap::iterator it = m_buffs.find(buff_id);
	if( it!=m_buffs.end() )
		return it->second;
	else
		return NULL;
}

void CBuffManager::GetBuffList(BuffDataList &buffList)
{
	for (BuffMap::iterator it = m_buffs.begin(); it != m_buffs.end(); it++)
		buffList.push_back(it->second);
}

BOOL CBuffManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: BuffManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		BuffData* pBuff = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				TCHAR buf[512];

				pBuff = SafeCreateObject(BuffData);
				reader.Read(_T("Buff_id"),		pBuff->id);
				reader.ReadString(_T("Buff_name"),		pBuff->name, 11);
				reader.ReadString(_T("Buff_Descript"),		pBuff->buff_description, 99);
				reader.Read(_T("Buff_type"),		pBuff->Buff_type);
				reader.Read(_T("Buff_school"),		pBuff->Buff_school);
				reader.Read(_T("Buff_endtype"),		pBuff->Buff_endtype);
				reader.Read(_T("Buff_attacktimes"),		pBuff->Buff_attacktimes);
				reader.Read(_T("Buff_starttime"),		pBuff->Buff_starttime);
				
				//reader.Read(_T("Buff_acttime"),		pBuff->Buff_acttime);

				TCHAR *delim = _T("|");
				TCHAR *cmd = NULL; 
				reader.ReadString(_T("Buff_acttime"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					pBuff->Buff_acttimes.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				reader.Read(_T("Buff_probability"),		pBuff->Buff_probability);
				reader.Read(_T("Buff_nextBuff"),		pBuff->Buff_nextBuff);
				reader.Read(_T("Buff_period_type"),		pBuff->Buff_period_type);
				reader.Read(_T("Buff_Mod_Type_1"),		pBuff->Buff_Mod_Type_1);
				reader.Read(_T("Buff_Mod_Type_2"),		pBuff->Buff_Mod_Type_2);
				reader.Read(_T("Buff_Mod_Type_3"),		pBuff->Buff_Mod_Type_3);
				reader.Read(_T("Buff_DOTtype"),		pBuff->Buff_DOTtype);
				reader.Read(_T("Buff_DOTattribute"),		pBuff->Buff_DOTattribute);
				reader.Read(_T("Buff_RemoveAftDead"),		pBuff->Buff_removeAfterDead);
				reader.Read(_T("Buff_HideEffect"),		pBuff->Buff_HideEffect);
				
				reader.ReadString(_T("Buff_Detect"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					if (value != 0)
						pBuff->Buff_DetectBuffs.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				reader.ReadString(_T("Buff_Blockcommand"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					if (value != 0)
						pBuff->Buff_Blockcommands.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				reader.ReadString(_T("Buff_BlockAction"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					if (value != 0)
						pBuff->Buff_BlockActions.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				reader.ReadString(_T("Buff_BlockSkill"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					if (value != 0)
						pBuff->Buff_BlockSkills.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				reader.ReadString(_T("Buff_AntiBuff"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					if (value != 0)
						pBuff->Buff_AntiBuffs.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				reader.ReadString(_T("Buff_ClearBuff"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					if (value != 0)
						pBuff->Buff_ClearBuffs.push_back(value);
					cmd = _tcstok(NULL, delim);
				}
				
				/*reader.Read(_T("Buff_Blockcommand"),		pBuff->Buff_Blockcommand);
				reader.Read(_T("Buff_BlockSkill"),		pBuff->Buff_BlockSkill);
				reader.Read(_T("Buff_AntiBuff"),		pBuff->Buff_AntiBuff);
				reader.Read(_T("Buff_ClearBuff"),		pBuff->Buff_ClearBuff);
				*/
				
				
				reader.ReadString(_T("Buff_IconFile"),		pBuff->Buff_IconFile, 99);
				reader.Read(_T("Buff_IconID"),		pBuff->Buff_IconID);
				reader.ReadString(_T("Buff_EffectFile"),		pBuff->Buff_EffectFile, 99);
				reader.Read(_T("Buff_EddectDisplay"),		pBuff->Buff_EddectDisplay);
				reader.ReadString(_T("Sound File"),		pBuff->Buff_SFX, 99);
				reader.ReadString(_T("Buff_SparkFile"),		pBuff->Buff_SparkFile, 99);

				reader.Read(_T("Buff_RemoveAftOffline"),		pBuff->Buff_RemoveAftOffline);
				reader.Read(_T("Buff_RemoveAftBattle"),		pBuff->Buff_RemoveAftBattle);
				reader.Read(_T("Buff_ProtectPK"),		pBuff->Buff_ProtectPK);
				reader.Read(_T("Buff_BlockPK"),		pBuff->Buff_BlockPK);
				reader.ReadString(_T("Buff_BlockChat"),	buf, 511);
				reader.Read(_T("Buff_KeepBody"),		pBuff->Buff_KeepBody);
				reader.Read(_T("Buff_Priority"),		pBuff->Buff_Priority);
				reader.Read(_T("Buff_TimeOut"),		    pBuff->Buff_TimeOut);
				
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					if (value != 0)
						pBuff->Buff_BlockChat.push_back((UInt8)value);
					cmd = _tcstok(NULL, delim);
				}
	
				reader.ReadString(_T("Buff_MoveSpeed"),	buf, 511);
				String temp = buf;
				if (temp.GetAt(0) == '-')
				{
					temp = temp.substr(1);
					UInt16 value = _ttoi(temp.c_str());
					pBuff->Buff_Speed = -1 * value;
				}
				else
				{
					UInt16 value = _ttoi(buf);
					pBuff->Buff_Speed = value;
				}
	
	
				reader.ReadString(_T("Buff_BlockItemType"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					if (value != 0)
						pBuff->Buff_BlockItemType.push_back(value);
					cmd = _tcstok(NULL, delim);
				}

				BuffMap::iterator Itr = m_buffs.find(pBuff->id);
				if (Itr != m_buffs.end())
				{
					SafeDeleteObject(Itr->second);
					m_buffs.erase(Itr);
				}
				reader.ReadString(_T("Buff_Value1String"),	pBuff->Buff_Value1String, 99);
				reader.ReadString(_T("Buff_Value2String"),	pBuff->Buff_Value2String, 99);
				reader.ReadString(_T("Buff_Value3String"),	pBuff->Buff_Value3String, 99);

				m_buffs.insert(std::make_pair(pBuff->id, pBuff));
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %i buff(s) read"), (UInt)m_buffs.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: BuffManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CBuffManager::LoadDAT(const TCHAR *filename)
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

			BuffData* pBuff;
			for (UInt32 i = 0; i < numBuff; i++)
			{
				pBuff = SafeCreateObject(BuffData);
				file.Read( (BYTE*)pBuff, sizeof(BuffData) );
				m_buffs[pBuff->id] = pBuff;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CBuffManager::LoadDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();

	return TRUE;
}

BOOL CBuffManager::SaveDAT(const TCHAR *filename)
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
			UInt32 numBuff = (UInt32)m_buffs.size();
			file.Write( (BYTE*)&numBuff, sizeof(UInt32) );

			for (BuffMap::iterator it = m_buffs.begin(); it != m_buffs.end(); it++)
			{
				file.Write( (BYTE*)it->second, sizeof(BuffData) );
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CBuffManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}