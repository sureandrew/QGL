#pragma once
#ifndef _DBO_PARTNER_H_
#define _DBO_PARTNER_H_
#include "Dbo.h"

class CdboPartnerAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboPartnerAccessor)

	DEFINE_ENTRY(partner_id, LARGE_INTEGER)
	DEFINE_ENTRY(mob_id, LARGE_INTEGER)
	DEFINE_ENTRY(owner_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(raname, 21)
	DEFINE_ENTRY(level, LONG)
	DEFINE_ENTRY(exp, LARGE_INTEGER)
	DEFINE_ENTRY(STA, LONG)
	DEFINE_ENTRY(SPR, LONG)
	DEFINE_ENTRY(STR, LONG)
	DEFINE_ENTRY(CON, LONG)
	DEFINE_ENTRY(AGI, LONG)
	DEFINE_ENTRY(attrPoint, LONG)
	DEFINE_ENTRY(HP_native, LONG)
	DEFINE_ENTRY(SP_native, LONG)
	DEFINE_ENTRY(ATK_native, LONG)
	DEFINE_ENTRY(DEF_native, LONG)
	DEFINE_ENTRY(SPD_native, LONG)
	DEFINE_ENTRY(POW_native, LONG)
	DEFINE_ENTRY(HIT_native, LONG)
	DEFINE_ENTRY(EVA_native, LONG)
	DEFINE_ENTRY(grow, FLOAT)
	DEFINE_ENTRY(HP, LONG)
	DEFINE_ENTRY(mHP, LONG)
	DEFINE_ENTRY(maxHP, LONG)
	DEFINE_ENTRY(SP, LONG)
	DEFINE_ENTRY(mSP, LONG)
	DEFINE_ENTRY(maxSP, LONG)
	DEFINE_ENTRY(favour, FLOAT)
	DEFINE_ENTRY(state, INT)
	DEFINE_ENTRY_STR(buff, 801)
	DEFINE_ENTRY(health, LONG)
	DEFINE_ENTRY(goodness, LONG)
	DEFINE_ENTRY(soulboundState, INT)
	DEFINE_ENTRY(expireDate, DBTIMESTAMP)
	DEFINE_ENTRY(element, INT)
	DEFINE_ENTRY(outLookColor, LONG)
	DEFINE_ENTRY(inWareHouse, INT)
	DEFINE_ENTRY(lock, INT)	
	DEFINE_ENTRY(cultivation, INT)
	DEFINE_ENTRY(practice, INT)


	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboPartnerAccessor)
	//ENABLE_PARAM_MAP(CdboPartnerAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS( 1, m_partner_id)
		COLUMN_ENTRY_LS( 2, m_mob_id)
		COLUMN_ENTRY_LS( 3, m_owner_id)
		COLUMN_ENTRY_LS( 4, m_raname)
		COLUMN_ENTRY_LS( 5, m_level)
		COLUMN_ENTRY_LS( 6, m_exp)
		COLUMN_ENTRY_LS( 7, m_STA)
		COLUMN_ENTRY_LS( 8, m_SPR)
		COLUMN_ENTRY_LS( 9, m_STR)
		COLUMN_ENTRY_LS(10, m_CON)
		COLUMN_ENTRY_LS(11, m_AGI)
		COLUMN_ENTRY_LS(12, m_attrPoint)
		COLUMN_ENTRY_LS(13, m_HP_native)
		COLUMN_ENTRY_LS(14, m_SP_native)
		COLUMN_ENTRY_LS(15, m_ATK_native)
		COLUMN_ENTRY_LS(16, m_DEF_native)
		COLUMN_ENTRY_LS(17, m_SPD_native)
		COLUMN_ENTRY_LS(18, m_POW_native)
		COLUMN_ENTRY_LS(19, m_HIT_native)
		COLUMN_ENTRY_LS(20, m_EVA_native)
		COLUMN_ENTRY_LS(21, m_grow)
		COLUMN_ENTRY_LS(22, m_HP)
		COLUMN_ENTRY_LS(23, m_mHP)
		COLUMN_ENTRY_LS(24, m_maxHP)
		COLUMN_ENTRY_LS(25, m_SP)
		COLUMN_ENTRY_LS(26, m_mSP)
		COLUMN_ENTRY_LS(27, m_maxSP)
		COLUMN_ENTRY_LS(28, m_favour)
		COLUMN_ENTRY_LS(29, m_state)
		COLUMN_ENTRY_LS(30, m_buff)
		COLUMN_ENTRY_LS(31, m_health)
		COLUMN_ENTRY_LS(32, m_goodness)
		COLUMN_ENTRY_LS(33, m_soulboundState)
		COLUMN_ENTRY_LS(34, m_expireDate)
		COLUMN_ENTRY_LS(35, m_element)
		COLUMN_ENTRY_LS(36, m_outLookColor)
		COLUMN_ENTRY_LS(37, m_inWareHouse)
		COLUMN_ENTRY_LS(38, m_lock)	
		COLUMN_ENTRY_LS(39, m_cultivation)
		COLUMN_ENTRY_LS(40, m_practice)
	END_COLUMN_MAP()
	
	DEFINE_COMMAND_EX(CdboPartnerAccessor, L"SELECT * FROM Partner")


	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, partner_id)
		COLUMN_ENTRY_TEST_ELSE(mob_id)
		COLUMN_ENTRY_TEST_ELSE(owner_id)
		COLUMN_ENTRY_TEST_ELSE(raname)
		COLUMN_ENTRY_TEST_ELSE(level)
		COLUMN_ENTRY_TEST_ELSE(exp)
		COLUMN_ENTRY_TEST_ELSE(STA)
		COLUMN_ENTRY_TEST_ELSE(SPR)
		COLUMN_ENTRY_TEST_ELSE(STR)
		COLUMN_ENTRY_TEST_ELSE(CON)
		COLUMN_ENTRY_TEST_ELSE(AGI)
		COLUMN_ENTRY_TEST_ELSE(attrPoint)
		COLUMN_ENTRY_TEST_ELSE(HP_native)
		COLUMN_ENTRY_TEST_ELSE(SP_native)
		COLUMN_ENTRY_TEST_ELSE(ATK_native)
		COLUMN_ENTRY_TEST_ELSE(DEF_native)
		COLUMN_ENTRY_TEST_ELSE(SPD_native)
		COLUMN_ENTRY_TEST_ELSE(POW_native)
		COLUMN_ENTRY_TEST_ELSE(HIT_native)
		COLUMN_ENTRY_TEST_ELSE(EVA_native)
		COLUMN_ENTRY_TEST_ELSE(grow)
		COLUMN_ENTRY_TEST_ELSE(HP)
		COLUMN_ENTRY_TEST_ELSE(mHP)
		COLUMN_ENTRY_TEST_ELSE(maxHP)
		COLUMN_ENTRY_TEST_ELSE(SP)
		COLUMN_ENTRY_TEST_ELSE(mSP)
		COLUMN_ENTRY_TEST_ELSE(maxSP)
		COLUMN_ENTRY_TEST_ELSE(favour)
		COLUMN_ENTRY_TEST_ELSE(state)
		COLUMN_ENTRY_TEST_ELSE(buff)
		COLUMN_ENTRY_TEST_ELSE(health)
		COLUMN_ENTRY_TEST_ELSE(goodness)
		COLUMN_ENTRY_TEST_ELSE(soulboundState)
		COLUMN_ENTRY_TEST_ELSE(expireDate)
		COLUMN_ENTRY_TEST_ELSE(element)
		COLUMN_ENTRY_TEST_ELSE(outLookColor)
		COLUMN_ENTRY_TEST_ELSE(inWareHouse)
		COLUMN_ENTRY_TEST_ELSE(lock)	
		COLUMN_ENTRY_TEST_ELSE(cultivation)
		COLUMN_ENTRY_TEST_ELSE(practice)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()


	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_mob_id)
		COLUMN_ENTRY(2, m_owner_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ResetLock)
		COLUMN_ENTRY(1, m_lock)	
		COLUMN_ENTRY(2, m_partner_id)
	END_PARAM_MAP_FUNC()


	BEGIN_PARAM_MAP_FUNC(_InsertMob)
		COLUMN_ENTRY(1, m_mob_id)
		COLUMN_ENTRY(2, m_owner_id)
		COLUMN_ENTRY(3, m_raname)
		COLUMN_ENTRY(4, m_level)
		COLUMN_ENTRY(5, m_STA)
		COLUMN_ENTRY(6, m_SPR)
		COLUMN_ENTRY(7, m_STR)
		COLUMN_ENTRY(8, m_CON)
		COLUMN_ENTRY(9, m_AGI)
		COLUMN_ENTRY(10, m_HP_native)
		COLUMN_ENTRY(11, m_SP_native)
		COLUMN_ENTRY(12, m_ATK_native)
		COLUMN_ENTRY(13, m_DEF_native)
		COLUMN_ENTRY(14, m_SPD_native)
		COLUMN_ENTRY(15, m_POW_native)
		COLUMN_ENTRY(16, m_HIT_native)
		COLUMN_ENTRY(17, m_EVA_native)
		COLUMN_ENTRY(18, m_grow)
		COLUMN_ENTRY(19, m_goodness)
		COLUMN_ENTRY(20, m_health)
		COLUMN_ENTRY(21, m_HP)
		COLUMN_ENTRY(22, m_mHP)
		COLUMN_ENTRY(23, m_maxHP)
		COLUMN_ENTRY(24, m_SP)
		COLUMN_ENTRY(25, m_mSP)
		COLUMN_ENTRY(26, m_maxSP)
		COLUMN_ENTRY(27, m_favour)
		COLUMN_ENTRY(28, m_soulboundState)
		COLUMN_ENTRY(29, m_outLookColor)
		COLUMN_ENTRY(30, m_cultivation)
		COLUMN_ENTRY(31, m_practice)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_InsertMobWithTimeLimit)
		COLUMN_ENTRY(1, m_mob_id)
		COLUMN_ENTRY(2, m_owner_id)
		COLUMN_ENTRY(3, m_raname)
		COLUMN_ENTRY(4, m_level)
		COLUMN_ENTRY(5, m_STA)
		COLUMN_ENTRY(6, m_SPR)
		COLUMN_ENTRY(7, m_STR)
		COLUMN_ENTRY(8, m_CON)
		COLUMN_ENTRY(9, m_AGI)
		COLUMN_ENTRY(10, m_HP_native)
		COLUMN_ENTRY(11, m_SP_native)
		COLUMN_ENTRY(12, m_ATK_native)
		COLUMN_ENTRY(13, m_DEF_native)
		COLUMN_ENTRY(14, m_SPD_native)
		COLUMN_ENTRY(15, m_POW_native)
		COLUMN_ENTRY(16, m_HIT_native)
		COLUMN_ENTRY(17, m_EVA_native)
		COLUMN_ENTRY(18, m_grow)
		COLUMN_ENTRY(19, m_goodness)
		COLUMN_ENTRY(20, m_health)
		COLUMN_ENTRY(21, m_HP)
		COLUMN_ENTRY(22, m_mHP)
		COLUMN_ENTRY(23, m_maxHP)
		COLUMN_ENTRY(24, m_SP)
		COLUMN_ENTRY(25, m_mSP)
		COLUMN_ENTRY(26, m_maxSP)
		COLUMN_ENTRY(27, m_favour)
		COLUMN_ENTRY(28, m_soulboundState)
		COLUMN_ENTRY(29, m_expireDate)
		COLUMN_ENTRY(30, m_outLookColor)
		COLUMN_ENTRY(31, m_cultivation)
		COLUMN_ENTRY(32, m_practice)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByPID)
		COLUMN_ENTRY(1, m_partner_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByOID)
		COLUMN_ENTRY(1, m_owner_id)
		COLUMN_ENTRY(2, m_inWareHouse)
	END_PARAM_MAP_FUNC()
};

class CdboPartner : public Cdbo<CCommand<CAccessor<CdboPartnerAccessor> > >
{
public:
	HRESULT Insert(DWORD mobId, DWORD ownerId)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);
		
		m_mob_id.QuadPart = mobId;
		m_owner_id.QuadPart = ownerId;

		return OpenCommand(_T("INSERT INTO Partner (mob_id, owner_id) ")
			_T("OUTPUT INSERTED.* VALUES (?, ?)"));
	}

	HRESULT InsertMob(DWORD mobId, DWORD ownerId, const String& raname)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_InsertMob);
		
		m_mob_id.QuadPart = mobId;
		m_owner_id.QuadPart = ownerId;
		//m_raname = _T("testing");//raname.c_str();
		COLUMN_STRING_COPY(m_raname, raname.c_str());

		return OpenCommand(_T("INSERT INTO Partner (mob_id, owner_id, raname, ")
			_T("\"level\", STA, SPR, STR, CON, AGI, HP_native, SP_native, ")
			_T("ATK_native, DEF_native, SPD_native, POW_native, HIT_native,")
			_T("EVA_native, grow, goodness, health, HP, mHP, maxHP, SP, mSP, maxSP, ")
			_T("favour, soulboundState, outLookColor, cultivation, practice) ")
			_T("OUTPUT INSERTED.* VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
			_T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
	}

	HRESULT InsertMobWithTimeLimit(DWORD mobId, DWORD ownerId, const String& raname)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_InsertMobWithTimeLimit);
		
		m_mob_id.QuadPart = mobId;
		m_owner_id.QuadPart = ownerId;
		//m_raname = _T("testing");//raname.c_str();
		COLUMN_STRING_COPY(m_raname, raname.c_str());

		return OpenCommand(_T("INSERT INTO Partner (mob_id, owner_id, raname, ")
			_T("\"level\", STA, SPR, STR, CON, AGI, HP_native, SP_native, ")
			_T("ATK_native, DEF_native, SPD_native, POW_native, HIT_native,")
			_T("EVA_native, grow, goodness, health, HP, mHP, maxHP, SP, mSP, maxSP, ")
			_T("favour, soulboundState, expireDate, outLookColor, cultivation, practice) ")
			_T("OUTPUT INSERTED.* VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ")
			_T("?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
	}

	HRESULT Delete(DWORD pid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByPID);
		m_partner_id.QuadPart = pid;
		return OpenCommand(_T("DELETE FROM Partner WHERE partner_id = ?"));
	}

	HRESULT SetLock(DWORD pid, Boolean lockValue)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ResetLock);	
		m_partner_id.QuadPart = pid;
		m_lock = lockValue? 1:0;
		return OpenCommand(_T("UPDATE Partner SET lock = ? WHERE partner_id = ?"));	
	}

	HRESULT RSByOwnerID(DWORD oid, int inWareHouse)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByOID);
		m_owner_id.QuadPart = oid;
		m_inWareHouse = inWareHouse;
		return OpenCommand(_T("SELECT * FROM Partner WHERE owner_id = ? and inWareHouse = ?"));
	}

	/*HRESULT RSByID(DWORD pid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByPID);
		m_partner_id.QuadPart = pid;
		return OpenCommand(_T("SELECT * FROM Partner WHERE partner_id = ?"));
	}*/
	
	
	HRESULT UpdateByAny(DWORD pid)
	{
		if (m_params.empty())
			return S_FALSE;

		String sql(_T("UPDATE Partner SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE partner_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_partner_id.QuadPart = pid;
		m_params.push_back(_T("partner_id"));		// add partner_id at the end of params
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}




};

#endif // _DBO_PARTNER_H_