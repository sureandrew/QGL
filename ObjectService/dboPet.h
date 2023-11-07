#pragma once
#ifndef _DBO_PET_H_
#define _DBO_PET_H_
#include "Dbo.h"

class CdboPetAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboPetAccessor)

	DEFINE_ENTRY(pet_uid, LARGE_INTEGER)
	DEFINE_ENTRY(pet_id, LARGE_INTEGER)
	DEFINE_ENTRY(owner_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(name, 21)
	DEFINE_ENTRY(sex, INT)
	DEFINE_ENTRY(level, LONG)
	DEFINE_ENTRY(exp, LARGE_INTEGER)
	DEFINE_ENTRY(state, INT)
	DEFINE_ENTRY(birthCount, INT)
	DEFINE_ENTRY_STR(firstMaster, 21)
	DEFINE_ENTRY(father_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(father_name, 21)
	DEFINE_ENTRY(mother_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(mother_name, 21)
	DEFINE_ENTRY(happiness, INT)
	DEFINE_ENTRY(VP, LONG)
	DEFINE_ENTRY(maxVP, LONG)
	DEFINE_ENTRY(element, INT)
	DEFINE_ENTRY(expireDate, DBTIMESTAMP)
	DEFINE_ENTRY(soulbound, INT)
	DEFINE_ENTRY(color, LONG)
	DEFINE_ENTRY(spirit, LONG)
	DEFINE_ENTRY(depute_master, LARGE_INTEGER)
	DEFINE_ENTRY(depute_time, LONG)
	DEFINE_ENTRY(depute_useTime, LONG)
	DEFINE_ENTRY(depute_exp, LARGE_INTEGER)
	DEFINE_ENTRY(inWareHouse, INT)
	DEFINE_ENTRY(lock, INT)	

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboPetAccessor)
	//ENABLE_PARAM_MAP(CdboPetAccessor)




	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS( 1, m_pet_uid)
		COLUMN_ENTRY_LS( 2, m_pet_id)
		COLUMN_ENTRY_LS( 3, m_owner_id)
		COLUMN_ENTRY_LS( 4, m_name)
		COLUMN_ENTRY_LS( 5, m_sex)
		COLUMN_ENTRY_LS( 6, m_level)
		COLUMN_ENTRY_LS( 7, m_exp)
		COLUMN_ENTRY_LS( 8, m_state)
		COLUMN_ENTRY_LS( 9, m_birthCount)
		COLUMN_ENTRY_LS(10, m_firstMaster)
		COLUMN_ENTRY_LS(11, m_father_id)
		COLUMN_ENTRY_LS(12, m_father_name)
		COLUMN_ENTRY_LS(13, m_mother_id)
		COLUMN_ENTRY_LS(14, m_mother_name)
		COLUMN_ENTRY_LS(15, m_happiness)
		COLUMN_ENTRY_LS(16, m_VP)
		COLUMN_ENTRY_LS(17, m_maxVP)
		COLUMN_ENTRY_LS(18, m_element)
		COLUMN_ENTRY_LS(19, m_expireDate)
		COLUMN_ENTRY_LS(20, m_soulbound)
		COLUMN_ENTRY_LS(21, m_color)
		COLUMN_ENTRY_LS(22, m_spirit)
		COLUMN_ENTRY_LS(23, m_depute_master)
		COLUMN_ENTRY_LS(24, m_depute_time)
		COLUMN_ENTRY_LS(25, m_depute_useTime)
		COLUMN_ENTRY_LS(26, m_depute_exp)
		COLUMN_ENTRY_LS(27, m_inWareHouse)
		COLUMN_ENTRY_LS(28, m_lock)	
	END_COLUMN_MAP()
	
	DEFINE_COMMAND_EX(CdboPetAccessor, L"SELECT * FROM Pet")

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, pet_uid)
		COLUMN_ENTRY_TEST_ELSE(pet_id)
		COLUMN_ENTRY_TEST_ELSE(owner_id)
		COLUMN_ENTRY_TEST_ELSE(name)
		COLUMN_ENTRY_TEST_ELSE(sex)
		COLUMN_ENTRY_TEST_ELSE(level)
		COLUMN_ENTRY_TEST_ELSE(exp)
		COLUMN_ENTRY_TEST_ELSE(state)
		COLUMN_ENTRY_TEST_ELSE(birthCount)
		COLUMN_ENTRY_TEST_ELSE(firstMaster)
		COLUMN_ENTRY_TEST_ELSE(father_id)
		COLUMN_ENTRY_TEST_ELSE(father_name)
		COLUMN_ENTRY_TEST_ELSE(mother_id)
		COLUMN_ENTRY_TEST_ELSE(mother_name)
		COLUMN_ENTRY_TEST_ELSE(happiness)
		COLUMN_ENTRY_TEST_ELSE(VP)
		COLUMN_ENTRY_TEST_ELSE(maxVP)
		COLUMN_ENTRY_TEST_ELSE(element)
		COLUMN_ENTRY_TEST_ELSE(expireDate)
		COLUMN_ENTRY_TEST_ELSE(soulbound)
		COLUMN_ENTRY_TEST_ELSE(color)
		COLUMN_ENTRY_TEST_ELSE(spirit)
		COLUMN_ENTRY_TEST_ELSE(depute_master)
		COLUMN_ENTRY_TEST_ELSE(depute_time)
		COLUMN_ENTRY_TEST_ELSE(depute_useTime)
		COLUMN_ENTRY_TEST_ELSE(depute_exp)
		COLUMN_ENTRY_TEST_ELSE(inWareHouse)
		COLUMN_ENTRY_TEST_ELSE(lock)	
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ResetLock)
		COLUMN_ENTRY(1, m_lock)	
		COLUMN_ENTRY(2, m_pet_uid)
	END_PARAM_MAP_FUNC()	

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_pet_id)
		COLUMN_ENTRY(2, m_owner_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_InsertPet)
		COLUMN_ENTRY( 1, m_pet_id)
		COLUMN_ENTRY( 2, m_owner_id)
		COLUMN_ENTRY( 3, m_name)
		COLUMN_ENTRY( 4, m_sex)
		COLUMN_ENTRY( 5, m_level)
		COLUMN_ENTRY( 6, m_exp)
		COLUMN_ENTRY( 7, m_state)
		COLUMN_ENTRY( 8, m_birthCount)
		COLUMN_ENTRY( 9, m_firstMaster)
		COLUMN_ENTRY(10, m_father_id)
		COLUMN_ENTRY(11, m_father_name)
		COLUMN_ENTRY(12, m_mother_id)
		COLUMN_ENTRY(13, m_mother_name)
		COLUMN_ENTRY(14, m_happiness)
		COLUMN_ENTRY(15, m_VP)
		COLUMN_ENTRY(16, m_maxVP)
		COLUMN_ENTRY(17, m_element)
		COLUMN_ENTRY(18, m_expireDate)
		COLUMN_ENTRY(19, m_soulbound)
		COLUMN_ENTRY(20, m_color)
		COLUMN_ENTRY(21, m_spirit)
		COLUMN_ENTRY(22, m_depute_master)
		COLUMN_ENTRY(23, m_depute_time)
		COLUMN_ENTRY(24, m_depute_useTime)
		COLUMN_ENTRY(25, m_depute_exp)
		
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByPID)
		COLUMN_ENTRY(1, m_pet_uid)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByOID)
		COLUMN_ENTRY(1, m_owner_id)
		COLUMN_ENTRY(2, m_inWareHouse)
	END_PARAM_MAP_FUNC()
};

class CdboPet : public Cdbo<CCommand<CAccessor<CdboPetAccessor> > >
{
public:
	HRESULT Insert(DWORD petId, DWORD ownerId)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);
		
		m_pet_id.QuadPart = petId;
		m_owner_id.QuadPart = ownerId;

		return OpenCommand(_T("INSERT INTO Pet (pet_id, owner_id) ")
			_T("OUTPUT INSERTED.* VALUES (?, ?)"));
	}

	HRESULT InsertPet(DWORD petId, DWORD ownerId, const String& raname)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_InsertPet);
		
		m_pet_id.QuadPart = petId;
		m_owner_id.QuadPart = ownerId;
		COLUMN_STRING_COPY(m_name, raname.c_str());

		return OpenCommand(
			_T("INSERT INTO Pet (pet_id, owner_id, name, sex, \"level\", exp, state, ")
			_T("birthCount, firstMaster, father_id, father_name, mother_id, mother_name, ")
			_T("happiness, VP, maxVP, element, expireDate, soulbound, color, spirit, ")
			_T("depute_master, depute_time, depute_useTime, depute_exp) ")
			_T("OUTPUT INSERTED.* VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
			_T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
	}

	HRESULT Delete(DWORD pid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByPID);
		m_pet_uid.QuadPart = pid;
		return OpenCommand(_T("DELETE FROM Pet WHERE pet_uid = ?"));
	}

	HRESULT SetLock(DWORD pid, Boolean lockValue)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ResetLock);	
		m_pet_uid.QuadPart = pid;
		m_lock = lockValue? 1:0;
		return OpenCommand(_T("UPDATE Pet SET lock = ? WHERE pet_uid = ?"));	
	}

	HRESULT RSByOwnerID(DWORD oid, int inWareHouse)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByOID);
		m_owner_id.QuadPart = oid;
		m_inWareHouse = inWareHouse;
		return OpenCommand(_T("SELECT * FROM Pet WHERE owner_id = ? and inWareHouse = ?"));
	}

	/*HRESULT RSByID(DWORD pid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByPID);
		m_pet_uid.QuadPart = pid;
		return OpenCommand(_T("SELECT * FROM Pet WHERE pet_uid = ?"));
	}*/

	HRESULT UpdateByAny(DWORD pid)
	{
		if (m_params.empty())
			return S_FALSE;

		String sql(_T("UPDATE Pet SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE pet_uid = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_pet_uid.QuadPart = pid;
		m_params.push_back(_T("pet_uid"));		// add partner_id at the end of params
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}

};

#endif // _DBO_PET_H_