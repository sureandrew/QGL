//-- Common
#include "Common.h"
//-- Self
#include "Resource/NpcManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/BuffReader.h>
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResNpc.h"
#include "Common/ExportVersion.h"

#define NPC_DATA_VER 1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

CNpcManager::CNpcManager()
{
	m_idMap.clear();
	m_maleSndName.clear();
	m_femaleSndName.clear();		
}

CNpcManager::~CNpcManager()
{
	Release();
}

VOID CNpcManager::Release()
{
	for( NpcIdMap::iterator itr = m_idMap.begin(); itr!=m_idMap.end(); itr++ )
		SafeDeleteObject(itr->second);
	m_idMap.clear();
}

const NpcData* CNpcManager::GetNpc(UInt32 npcid)
{
	NpcIdMap::iterator itr = m_idMap.find(npcid);
	if( itr!=m_idMap.end() )
		return itr->second;
	return NULL;
}

BOOL CNpcManager::LoadCSV(const TCHAR* baseFile, const TCHAR* mobFile, const TCHAR* dropItemFile)
{
	CQGLCSVTReader reader;

	// load NPC basic info
	if( reader.Open(baseFile, true) )
	{
		NpcData* pNpc = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pNpc = SafeCreateObject(NpcData);
				if ( pNpc ) {
				reader.Read(_T("npc_id"),		pNpc->npc_id);
				reader.Read(_T("npc_type"),		pNpc->npc_type);
				reader.ReadString(_T("pre_name"),	pNpc->pre_name, 19);
				reader.ReadString(_T("mid_name"),	pNpc->mid_name, 19);
				reader.Read(_T("pos_name"),		pNpc->pos_name);
				reader.Read(_T("bFaction"),		pNpc->bFaction);
				reader.Read(_T("faction"),		pNpc->faction);
				reader.Read(_T("job"),			pNpc->job);
				reader.Read(_T("sex"),			pNpc->sex);
				reader.Read(_T("direction"),	pNpc->direction);
				UInt64 tempint;
				reader.Read(_T("appearTime"),	tempint);
				pNpc->appearTime = (DWORD)tempint;
				reader.Read(_T("squad"),		pNpc->squad);
				reader.Read(_T("minLackey"),	pNpc->minLackey);
				reader.Read(_T("maxLackey"),	pNpc->maxLackey);
				reader.ReadString(_T("script"),	pNpc->script, 31);
//				reader.Read(_T("shop_id"),		pNpc->shop_id);
//				reader.Read(_T("quest_id"),		pNpc->quest_id);
				reader.Read(_T("exp"),			pNpc->exp);
				reader.Read(_T("money"),		pNpc->money);
				reader.ReadString(_T("model"),	pNpc->model, 31);
				reader.ReadString(_T("image"),	pNpc->image, 31);
				reader.ReadString(_T("dropitemcsv"), pNpc->dropitemcsv, 31);
				reader.ReadString(_T("battleScript"), pNpc->battleScript, 31);

				m_idMap.insert(std::make_pair(pNpc->npc_id, pNpc));
				}
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNpcManager::LoadCSV, open error, CSV reading error due to column format or missing MoveNext(), %s"), baseFile );
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	// load MOB id for all NPC
	if( reader.Open(mobFile, true) )
	{
		NpcIdMap::iterator it;
		UInt32 npc_id = 0;
		NPCMobInfo mob_data;
		try{
			while( !reader.IsLineEmpty()  )
			{
				reader.Read(_T("npc_id"),		npc_id);
				reader.Read(_T("mob_id"),		mob_data.MobId);
				reader.Read(_T("encount"),		mob_data.Encount);
				it = m_idMap.find(npc_id);
				if (it != m_idMap.end())
				{
					it->second->mobLackey.push_back(mob_data);
				}
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNpcManager::LoadCSV, open error, CSV reading error due to column format or missing MoveNext(), %s"), mobFile );
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
}

BOOL CNpcManager::LoadRandomNameCSV(const TCHAR* maleFile, const TCHAR* femaleFile, const TCHAR* preNameFile)
{
	CQGLCSVTReader reader;

	// load NPC male name and the first name
	if( reader.Open(maleFile, true) )
	{
		TCHAR Name[10];
		
		try{
			while( !reader.IsLineEmpty()  )
			{
				UInt8 type = 2;
                reader.Read(_T("type"), type);
				reader.ReadString(_T("name"), Name, 9);
				String temp = String(Name);
				if (type == 0)
					m_fstName.push_back(temp);
				else if (type == 1)
					m_maleSndName.push_back(temp);
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNpcManager::LoadRandomNameCSV, open error, CSV reading error due to column format or missing MoveNext(), %s"), maleFile );
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	if( reader.Open(femaleFile, true) )
	{
		TCHAR Name[10];
		
		try{
			while( !reader.IsLineEmpty()  )
			{
				UInt8 type = 2;
                reader.Read(_T("type"), type);
				reader.ReadString(_T("name"), Name, 9);
				String temp = String(Name);
				if (type == 1)
					m_femaleSndName.push_back(temp);
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNpcManager::LoadRandomNameCSV, open error, CSV reading error due to column format or missing MoveNext(), %s"), femaleFile);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	if( reader.Open(preNameFile, true) )
	{
		TCHAR Name[10];
		
		try{
			while( !reader.IsLineEmpty()  )
			{
				reader.ReadString(_T("pre-name"), Name, 9);
				String temp = String(Name);
				m_npcPreName.push_back(temp);
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNpcManager::LoadRandomNameCSV, open error, CSV reading error due to column format or missing MoveNext(), %s"), preNameFile);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
}

String CNpcManager::GetNPCFstName(UInt16 id)
{
	if (id < m_fstName.size())
        return m_fstName[id]; 
	String empty = _T("");
    return empty;
}

UInt16 CNpcManager::GetTotalNPCFstName()
{
    return (UInt16)m_fstName.size();  
}


String CNpcManager::GetMaleNPCSndName(UInt16 id)
{
	if (id < m_maleSndName.size())
        return m_maleSndName[id]; 
	String empty = _T("");
    return empty;  
}

UInt16 CNpcManager::GetTotalNPCMaleSndName()
{
    return (UInt16)m_maleSndName.size();  
}

String CNpcManager::GetFemaleNPCSndName(UInt16 id)
{
	if (id < m_femaleSndName.size())
        return m_femaleSndName[id]; 
	String empty = _T("");
    return empty;  
}

UInt16 CNpcManager::GetTotalNPCFemaleSndName()
{
    return (UInt16)m_femaleSndName.size();  
}


BOOL CNpcManager::LoadDAT(const TCHAR* filename)
{
	File file;
	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			file.Read((BYTE*)&header, sizeof(DefaultInfoHeader));

			TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, header.version!=EXPORT_DATA_SERVER_NPC_TEMPLATE_FORMAT_VERSION, _T("NPCManager::LoadDAT from \"%s\", file version:%i, app. version:%i"), String(filename).c_str(), header.version, EXPORT_DATA_SERVER_NPC_TEMPLATE_FORMAT_VERSION);
			if( header.version!=EXPORT_DATA_SERVER_NPC_TEMPLATE_FORMAT_VERSION ) throw;

			UInt32 numnpc = 0;
			file.Read((BYTE*)&numnpc, sizeof(UInt32));

			UInt32 tmpsize, tmpint;
			UInt j;

			NpcData * npc = NULL;
			for( UInt i = 0; i<numnpc; i++ )
			{
				npc = SafeCreateObject(NpcData);
				BinaryServerNPCTemplate info;
				file.Read( (BYTE*)&info, sizeof(BinaryServerNPCTemplate) );
				npc->appearTime = info.m_npc_appeartime;
				npc->bFaction = info.m_npc_faction;
				npc->direction = info.m_npc_direction;
				npc->exp = info.m_npc_exp;
				npc->faction = info.m_npc_faction_name;
				npc->job = info.m_npc_job;
				npc->maxLackey = info.m_npc_max_lackey;
				_tcsncpy(npc->pre_name, info.m_npc_pre_name, 19);
				npc->pre_name[19] = 0;
				_tcsncpy(npc->mid_name, info.m_npc_mid_name, 19);
				npc->mid_name[19] = 0;
				npc->minLackey = info.m_npc_mini_lackey;
				npc->money = info.m_npc_money;
				npc->npc_id = info.m_npc_id;
				npc->npc_type = info.m_npc_type;
				npc->pos_name = info.m_npc_pos_name_type;
				
				_tcsncpy(npc->script, info.m_npc_script, 31);
				npc->script[31] = 0;
				npc->sex = info.m_npc_sex;
				npc->squad = info.m_npc_squad;
				_tcsncpy(npc->dropitemcsv, info.m_npc_dropitem, 31);
				npc->dropitemcsv[31] = 0;
				_tcsncpy(npc->battleScript, info.m_npc_battlescript, 31);
				npc->battleScript[31] = 0;

				file.Read( (BYTE*)&tmpsize, sizeof(UInt32) );
				for( j = 0; j<tmpsize; j++ )
				{
					NPCMobInfo tmp;
					file.Read( (BYTE*)&tmp.MobId, sizeof(UInt32) );
					file.Read( (BYTE*)&tmp.Encount, sizeof(UInt32) );
					npc->mobLackey.push_back(tmp);
				}
				file.Read( (BYTE*)&tmpsize, sizeof(UInt32) );
				for( j = 0; j<tmpsize; j++ )
				{
					NPCMobInfo tmp;
					file.Read( (BYTE*)&tmp.MobId, sizeof(UInt32) );
					file.Read( (BYTE*)&tmp.Encount, sizeof(UInt32) );
					npc->mobLeader.push_back(tmp);
				}
				file.Read( (BYTE*)&tmpsize, sizeof(UInt32) );
				for( j = 0; j<tmpsize; j++ )
				{
					file.Read( (BYTE*)&tmpint, sizeof(UInt32) );
					npc->shop_id.push_back(tmpint);
				}
				file.Read( (BYTE*)&tmpsize, sizeof(UInt32) );
				for( j = 0; j<tmpsize; j++ )
				{
					file.Read( (BYTE*)&tmpint, sizeof(UInt32) );
					npc->quest_id.push_back(tmpint);
				}
				m_idMap.insert(std::make_pair(npc->npc_id, npc));
			}
		}
	}
	catch(FileException Er)
	{
		file.Close();
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNpcManager::LoadDAT, open error, %s"), filename);
		return FALSE;
	}
	file.Close();
	return TRUE;
}



BOOL CNpcManager::SaveDAT(const TCHAR* filename)
{
/*	File file;
	try
	{
		file.Open(filename, File::MODE_WRITE);
		if (file.IsOpened())
		{
			BinaryBlockInfoHeader header;
			header.version = NPC_DATA_VER;
			file.Write( (BYTE*)&header, sizeof(BinaryBlockInfoHeader) );

			UInt32 numnpc = (UInt)m_idMap.size();
			file.Write((BYTE*)&numnpc, sizeof(UInt32));

			UInt32 tmpsize, tmpint;
			UInt j;

			NpcData* npc = NULL;
			for( NpcIdMap::iterator itr = m_idMap.begin(); itr!=m_idMap.end(); itr++ )
			{
				npc = itr->second;
				file.Write( (BYTE*)npc->m_basicinfo, sizeof(BinaryServerNPCTemplate) );

				tmpsize = (UInt)npc->m_mob_lackey.size();
				file.Write( (BYTE*)&tmpsize, sizeof(UInt32) );
				for( j = 0; j<npc->m_mob_lackey.size(); j++ )
				{
					tmpint = npc->m_mob_lackey[j];
					file.Write( (BYTE*)&tmpint, sizeof(UInt32) );
				}

				tmpsize = (UInt)npc->m_mob_leader.size();
				file.Write( (BYTE*)&tmpsize, sizeof(UInt32) );
				for( j = 0; j<npc->m_mob_leader.size(); j++ )
				{
					tmpint = npc->m_mob_leader[j];
					file.Write( (BYTE*)&tmpint, sizeof(UInt32) );
				}

				tmpsize = (UInt)npc->m_shopid.size();
				file.Write( (BYTE*)&tmpsize, sizeof(UInt32) );
				for( j = 0; j<npc->m_shopid.size(); j++ )
				{
					tmpint = npc->m_shopid[j];
					file.Write( (BYTE*)&tmpint, sizeof(UInt32) );
				}

				tmpsize = (UInt)npc->m_quest.size();
				file.Write( (BYTE*)&tmpsize, sizeof(UInt32) );
				for( j = 0; j<npc->m_quest.size(); j++ )
				{
					tmpint = npc->m_quest[j];
					file.Write( (BYTE*)&tmpint, sizeof(UInt32) );
				}

				tmpsize = (UInt)npc->m_item.size();
				file.Write( (BYTE*)&tmpsize, sizeof(UInt32) );
				for( j = 0; j<npc->m_item.size(); j++ )
				{
					tmpint = npc->m_item[j];
					file.Write( (BYTE*)&tmpint, sizeof(UInt32) );
				}
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CNpcManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
*/
	return TRUE;
}

