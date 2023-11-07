//-- Common
#include "Common.h"
//-- Self
#include "Resource/MobManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/BuffReader.h>
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResMob.h"
#include "Common/ExportVersion.h"

#define mob_DATA_VER 1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

CMobManager::CMobManager()
{
	m_mobint.clear();
}

CMobManager::~CMobManager()
{
	Release();
}

VOID CMobManager::Release()
{
	for( MobMapInt::iterator itr = m_mobint.begin(); itr!=m_mobint.end(); itr++ )
		SafeDeleteObject(itr->second);
	
	m_mobint.clear();
}

const MobData* CMobManager::GetMob(UInt32 Mobid)
{
	MobMapInt::iterator itr = m_mobint.find(Mobid);
	if( itr!=m_mobint.end() )
		return itr->second;
	return NULL;
}

void CMobManager::GetMobList(MobDataList &mobList)
{
	for (MobMapInt::iterator it = m_mobint.begin(); it != m_mobint.end(); it++)
		mobList.push_back(it->second);
}

BOOL CMobManager::LoadCSV(const TCHAR* filename)
{
	CQGLCSVTReader reader;
	if( reader.Open(filename, true) )
	{
		MobData* pData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pData = SafeCreateObject(MobData);
				if ( pData ) {
				reader.Read(_T("mob_id"),		pData->mob_id);
				reader.Read(_T("mob_type"),		pData->mob_type);
				reader.ReadString(_T("pre_name"),	pData->pre_name,	19);
				reader.ReadString(_T("mid_name"),	pData->mid_name,	19);
				reader.Read(_T("pos_name"),		pData->pos_name);
				reader.Read(_T("bFaction"),		pData->bFaction);
				reader.Read(_T("race"),			pData->race);
				reader.Read(_T("cclass"),		pData->cclass);
				reader.Read(_T("faction"),		pData->faction);
				reader.Read(_T("sex"),			pData->sex);
				reader.Read(_T("goodness"),		pData->goodness);
				reader.Read(_T("boss"),			pData->boss);
				reader.Read(_T("elite_type"),	pData->elite_type);
				reader.Read(_T("element"),		pData->element);
				reader.Read(_T("partner_type"),	pData->partner_type);
				reader.Read(_T("req_lv"),		pData->req_lv);
				reader.Read(_T("talent"),		pData->talent);
				reader.Read(_T("potential"),	pData->potential);
				reader.Read(_T("lv_d"),			pData->lv_d);
				reader.Read(_T("lv_u"),			pData->lv_u);
				reader.Read(_T("STA_base_d"),	pData->STA_base_d);
				reader.Read(_T("STA_base_u"),	pData->STA_base_u);
				reader.Read(_T("SPR_base_d"),	pData->SPR_base_d);
				reader.Read(_T("SPR_base_u"),	pData->SPR_base_u);
				reader.Read(_T("STR_base_d"),	pData->STR_base_d);
				reader.Read(_T("STR_base_u"),	pData->STR_base_u);
				reader.Read(_T("CON_base_d"),	pData->CON_base_d);
				reader.Read(_T("CON_base_u"),	pData->CON_base_u);
				reader.Read(_T("AGI_base_d"),	pData->AGI_base_d);
				reader.Read(_T("AGI_base_u"),	pData->AGI_base_u);
				reader.Read(_T("grow_d"),		pData->grow_d);
				reader.Read(_T("grow_u"),		pData->grow_u);
				reader.Read(_T("HP_native_d"),	pData->HP_native_d);
				reader.Read(_T("HP_native_u"),	pData->HP_native_u);
				reader.Read(_T("SP_native_d"),	pData->SP_native_d);
				reader.Read(_T("SP_native_u"),	pData->SP_native_u);
				reader.Read(_T("ATK_native_d"),	pData->ATK_native_d);
				reader.Read(_T("ATK_native_u"),	pData->ATK_native_u);
				reader.Read(_T("DEF_native_d"),	pData->DEF_native_d);
				reader.Read(_T("DEF_native_u"),	pData->DEF_native_u);
				reader.Read(_T("SPD_native_d"),	pData->SPD_native_d);
				reader.Read(_T("SPD_native_u"),	pData->SPD_native_u);
				reader.Read(_T("POW_native_d"),	pData->POW_native_d);
				reader.Read(_T("POW_native_u"),	pData->POW_native_u);
				reader.Read(_T("HIT_native_d"),	pData->HIT_native_d);
				reader.Read(_T("HIT_native_u"),	pData->HIT_native_u);
				reader.Read(_T("EVA_native_d"),	pData->EVA_native_d);
				reader.Read(_T("EVA_native_u"),	pData->EVA_native_u);
				reader.ReadString(_T("model"),	pData->model,	31);
				reader.ReadString(_T("color"),	pData->color,	31);
				reader.Read(_T("Health_f"),		pData->Health_f);
				reader.Read(_T("skill_file_id"),		pData->skill_file_id);
				reader.Read(_T("rare"),			pData->rare);
				reader.Read(_T("TimeLimit"),	pData->TimeLimit);
				reader.Read(_T("TimeLimitType"),	pData->TimeLimitType);
				reader.Read(_T("soulbound"),	pData->soulbound);
				reader.Read(_T("unique"),		pData->unique);

				m_mobint.insert(std::make_pair(pData->mob_id, pData));
				}
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: LoadMobCSVData::Load, open error, CSV reading error due to column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
}

BOOL CMobManager::LoadDAT(const TCHAR* filename)
{
	File file;
	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			file.Read((BYTE*)&header, sizeof(DefaultInfoHeader));

			TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, header.version!=EXPORT_DATA_SERVER_MOB_TEMPLATE_FORMAT_VERSION, _T("NPCManager::LoadDAT from \"%s\", file version:%i, app. version:%i"), String(filename).c_str(), header.version, EXPORT_DATA_SERVER_MOB_TEMPLATE_FORMAT_VERSION);
			if( header.version!=EXPORT_DATA_SERVER_MOB_TEMPLATE_FORMAT_VERSION ) throw;

			UInt32 numMob = 0;
			file.Read((BYTE*)&numMob, sizeof(UInt32));

			BinaryServerMOBInfo * Mob = NULL;
			for( UInt i = 0; i<numMob; i++ )
			{
				Mob = SafeCreateObject(BinaryServerMOBInfo);
				file.Read( (BYTE*)Mob, sizeof(BinaryServerMOBInfo) );
				MobData * tmpmob = SafeCreateObject(MobData);
				if ( tmpmob ) {
					tmpmob->AGI_base_d = Mob->AGI_base_d;
					tmpmob->AGI_base_u = Mob->AGI_base_u;
					tmpmob->ai_type = Mob->ai_type;
					tmpmob->ATK_native_d = Mob->ATK_native_d;
					tmpmob->ATK_native_u = Mob->ATK_native_u;
					tmpmob->bFaction = Mob->bFaction;
					tmpmob->boss = Mob->boss;
					tmpmob->cclass = Mob->cclass;
					tmpmob->CON_base_d = Mob->CON_base_d;
					tmpmob->CON_base_u = Mob->CON_base_u;
					tmpmob->DEF_native_d = Mob->DEF_native_d;
					tmpmob->DEF_native_u = Mob->DEF_native_u;
					tmpmob->element = Mob->element;
					tmpmob->elite_type = Mob->elite_type;
					tmpmob->EVA_native_d = Mob->EVA_native_d;
					tmpmob->EVA_native_u = Mob->EVA_native_u;
					tmpmob->faction = Mob->faction;
					tmpmob->goodness = Mob->goodness;
					tmpmob->grow_d = Mob->grow_d;
					tmpmob->grow_u = Mob->grow_u;
					tmpmob->HIT_native_d = Mob->HIT_native_d;
					tmpmob->HIT_native_u = Mob->HIT_native_u;
					tmpmob->HP_native_d = Mob->HP_native_d;
					tmpmob->HP_native_u = Mob->HP_native_u;
					tmpmob->lv_d = Mob->lv_d;
					tmpmob->lv_u = Mob->lv_u;
					_tcsncpy(tmpmob->pre_name, Mob->pre_name, 19);
					tmpmob->pre_name[19] = 0;
					_tcsncpy(tmpmob->mid_name, Mob->mid_name, 19);
					tmpmob->mid_name[19] = 0;
			
					tmpmob->mob_id = Mob->mob_id;
					tmpmob->mob_type = Mob->mob_type;
					_tcscpy(tmpmob->model, Mob->model);
					tmpmob->model[31] = 0;
					//_tcscpy(tmpmob->color, Mob->color);
					tmpmob->partner_type = Mob->partner_type;
					tmpmob->pos_name = Mob->pos_name;
					tmpmob->potential = Mob->potential;
					tmpmob->POW_native_d = Mob->POW_native_d;
					tmpmob->POW_native_u = Mob->POW_native_u;
					
					tmpmob->race = Mob->race;
					tmpmob->req_lv = Mob->req_lv;
					tmpmob->sex = Mob->sex;
					tmpmob->SP_native_d = Mob->SP_native_d;
					tmpmob->SP_native_u = Mob->SP_native_u;
					tmpmob->SPD_native_d = Mob->SPD_native_d;
					tmpmob->SPD_native_u = Mob->SPD_native_u;
					tmpmob->SPR_base_d = Mob->SPR_base_d;
					tmpmob->SPR_base_u = Mob->SPR_base_u;
					tmpmob->STA_base_d = Mob->STA_base_d;
					tmpmob->STA_base_u = Mob->STA_base_u;
					tmpmob->STR_base_d = Mob->STR_base_d;
					tmpmob->STR_base_u = Mob->STR_base_u;
					tmpmob->talent = Mob->talent;
					tmpmob->Health_f = Mob->maxHealth;
					tmpmob->skill_file_id = Mob->skill_file_id;

					tmpmob->rare = Mob->rare;
					tmpmob->TimeLimit = Mob->TimeLimit;
					tmpmob->TimeLimitType = Mob->TimeLimitType;
					tmpmob->soulbound = Mob->soulbound;
					tmpmob->unique = Mob->unique;
					tmpmob->outlookColor = Mob->outlookColor;

					m_mobint.insert(std::make_pair(tmpmob->mob_id, tmpmob));
				}
				SafeDeleteObject(Mob);
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: LoadDAT, open error, %s"), filename);
		return FALSE;
	}
	return TRUE;
}

BOOL CMobManager::SaveDAT(const TCHAR* filename)
{
	File file;
	try
	{
		file.Open(filename, File::MODE_WRITE);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			header.version = mob_DATA_VER;
			file.Write( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numMob = (UInt32)m_mobint.size();
			file.Write( (BYTE*)&numMob, sizeof(UInt32) );

			MobMapInt::iterator itr = m_mobint.begin();
			while( itr!=m_mobint.end() )
			{
				file.Write( (BYTE*)itr->second, sizeof(MobData) );
				itr++;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMobManager::SaveDAT, open error, %s"), filename);
		return FALSE;
	}
	return TRUE;
}


UInt32 CMobManager::GetMobSize()
{
	return (UInt32)m_mobint.size();
}
