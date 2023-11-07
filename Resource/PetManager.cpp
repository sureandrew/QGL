//-- Common
#include "Common.h"
//-- Self
#include "Resource/PetManager.h"
//-- Library
#include "Reuben/Platform/BuffReader.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResPet.h"

CPetManager::CPetManager()
{
	m_Pet.clear();
}

CPetManager::~CPetManager()
{
	Release();
}

void CPetManager::Release()
{
	for (PetDataMap::iterator itr = m_Pet.begin(); itr != m_Pet.end(); ++itr)
	{
		SafeDeleteObject(itr->second);
	}

	m_Pet.clear();

}

BOOL CPetManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
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
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: PetManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		PetData* pPetData = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				TCHAR buf[512];

				pPetData = SafeCreateObject(PetData);
				reader.Read(_T("pet_id"),		pPetData->pet_id);
				reader.ReadString(_T("pet_Name"),		pPetData->pet_name, 19);
				reader.Read(_T("pet_type"),		pPetData->pet_type);
				reader.Read(_T("pet_breed_type"),		pPetData->pet_breed_type);
				reader.Read(_T("pet_lv"),		pPetData->pet_lv);
				reader.Read(_T("pet_ReqLV"),		pPetData->pet_ReqLV);
				reader.Read(_T("pet_rare"),		pPetData->pet_rare);
				reader.Read(_T("pet_race"),		pPetData->pet_race);
				reader.Read(_T("pet_sex"),		pPetData->pet_sex);
				reader.Read(_T("pet_canbirth"),		pPetData->pet_canbirth);
				reader.Read(_T("pet_useVP"),		pPetData->pet_useVP);
				reader.Read(_T("pet_element"),		pPetData->pet_element);
				reader.Read(_T("pet_MaxVP"),		pPetData->pet_MaxVP);
				reader.Read(_T("pet_StartHappiness"),		pPetData->pet_StartHappiness);
				reader.Read(_T("pet_element"),		pPetData->pet_element);
				
				reader.ReadString(_T("pet_Speed"),	buf, 511);
				String temp = buf;
				if (temp.GetAt(0) == '-')
				{
					temp = temp.substr(1);
					UInt16 value = _ttoi(temp.c_str());
					pPetData->pet_Speed = -1 * value;
				}
				else
				{
					UInt16 value = _ttoi(buf);
					pPetData->pet_Speed = value;
				}


				reader.Read(_T("pet_Height"),		pPetData->pet_Height);
				reader.Read(_T("pet_TimeLimitType"),		pPetData->pet_TimeLimitType);
				reader.Read(_T("pet_TimeLimit"),		pPetData->pet_TimeLimit);
				reader.Read(_T("pet_soulbound"),		pPetData->pet_soulbound);
				reader.Read(_T("pet_unique"),		pPetData->pet_unique);
				reader.Read(_T("pet_SkillList"),		pPetData->pet_SkillList);
				reader.Read(_T("pet_color"),		pPetData->pet_color);

				reader.ReadString(_T("pet_description"),		pPetData->pet_description, 255);
				reader.ReadString(_T("pet_model"),		pPetData->pet_model, 19);
				
				reader.ReadString(_T("pet_faceL"),		pPetData->pet_faceL, 19);
				reader.ReadString(_T("pet_faceS"),		pPetData->pet_faceS, 19);
				reader.ReadString(_T("pet_sound_idle1"),		pPetData->pet_sound_idle1, 19);
				reader.ReadString(_T("pet_sound_idle2"),		pPetData->pet_sound_idle2, 19);

				reader.ReadString(_T("pet_sound_happy"),		pPetData->pet_sound_happy, 19);
				reader.ReadString(_T("pet_sound_unhappy"),		pPetData->pet_sound_unhappy, 19);
				reader.ReadString(_T("pet_sound_atk1"),		pPetData->pet_sound_atk1, 19);
				reader.ReadString(_T("pet_sound_atk2"),		pPetData->pet_sound_atk2, 19);
				reader.ReadString(_T("pet_sound_skill1"),		pPetData->pet_sound_skill1, 19);
				reader.ReadString(_T("pet_sound_skill2"),		pPetData->pet_sound_skill2, 19);
				reader.ReadString(_T("pet_sound_hurt"),		pPetData->pet_sound_hurt, 19);
				reader.ReadString(_T("pet_sound_die"),		pPetData->pet_sound_die, 19);

				reader.Read(_T("pet_reduce_vp_min"),		pPetData->pet_reduce_vp_min);
				reader.Read(_T("pet_reduce_vp_max"),		pPetData->pet_reduce_vp_max);
				
				PetDataMap::iterator Itr = m_Pet.find(pPetData->pet_id);
				if (Itr != m_Pet.end())
				{
					SafeDeleteObject(Itr->second);
					m_Pet.erase(Itr);
				}
				m_Pet.insert(std::make_pair(pPetData->pet_id, pPetData));
				
				reader.MoveNext();
			}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: PetManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
	
}


const PetData* CPetManager::GetPet(UInt32 pet_id)
{
	PetDataMap::iterator itr = m_Pet.find(pet_id);
	if (itr != m_Pet.end())
		return itr->second;
	else
		return NULL;
}
