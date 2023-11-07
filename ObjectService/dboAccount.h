// dboAccount.h : Declaration of the CdboAccount

#pragma once
#ifndef _DBO_ACCOUNT_H_
#define _DBO_ACCOUNT_H_
#include "Dbo.h"
#include "ObjectService/Global.h"

// code generated on Thursday, March 22, 2007, 11:14 AM

class CdboAccountAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboAccountAccessor)

	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(loginName, 17)
	DEFINE_ENTRY_STR(password_hash, 65)
	DEFINE_ENTRY(disconnect_time, DBTIMESTAMP)
	DEFINE_ENTRY(offLnTime, LONG)
	DEFINE_ENTRY(onLnTime, LONG)	
	DEFINE_ENTRY(yuanbao, LARGE_INTEGER)
	DEFINE_ENTRY(banTime, DBTIMESTAMP)
	DEFINE_ENTRY(addictType, INT)
	DEFINE_ENTRY(yuanbaoLock, INT)
	DEFINE_ENTRY_STR(assetLockPassword, 65)
	DEFINE_ENTRY(forceUnlockDate, DBTIMESTAMP)	
	DEFINE_ENTRY(yuanbaoTicket, LARGE_INTEGER)

	DEFINE_ENTRY(canupdate, INT)
	DEFINE_ENTRY(changeyuanbao, LARGE_INTEGER)
	DEFINE_ENTRY(changeyuanbaoticket, LARGE_INTEGER)
	DEFINE_ENTRY(ratioPerYBT, INT)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider

	//ENABLE_COLUMN_MAP(CdboAccountAccessor)
	//ENABLE_PARAM_MAP(CdboAccountAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS(1, m_acct_id)
		COLUMN_ENTRY_LS(2, m_loginName)
		COLUMN_ENTRY_LS(3, m_password_hash)
		COLUMN_ENTRY_LS(4, m_disconnect_time)	
		COLUMN_ENTRY_LS(5, m_offLnTime)
		COLUMN_ENTRY_LS(6, m_onLnTime)
		COLUMN_ENTRY_LS(7, m_yuanbao)
		COLUMN_ENTRY_LS(8, m_banTime)
		COLUMN_ENTRY_LS(9, m_addictType)
		COLUMN_ENTRY_LS(10, m_yuanbaoLock)
		COLUMN_ENTRY_LS(11, m_assetLockPassword)
		COLUMN_ENTRY_LS(12, m_forceUnlockDate)	
		COLUMN_ENTRY_LS(13, m_yuanbaoTicket)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetInactive)
		COLUMN_ENTRY_LS(1, m_acct_id)
		COLUMN_ENTRY_LS(2, m_loginName)
		COLUMN_ENTRY_LS(3, m_password_hash)
		COLUMN_ENTRY_LS(4, m_banTime)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_GetByName)
		COLUMN_ENTRY(1, m_loginName)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByID)
		COLUMN_ENTRY(1, m_acct_id)
	END_PARAM_MAP_FUNC()

	/*BEGIN_PARAM_MAP_FUNC(_UpdateYuanBao)
		COLUMN_ENTRY(1, m_changeyuanbao)
		COLUMN_ENTRY(2, m_acct_id)
		COLUMN_ENTRY(3, m_changeyuanbao)
	END_PARAM_MAP_FUNC()*/

	BEGIN_COLUMN_MAP_FUNC(_GetUpdatedYuanBao)
		COLUMN_ENTRY_LS(1, m_yuanbao)
		COLUMN_ENTRY_LS(2, m_canupdate)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetUpdatedYuanBaoTicket)
		COLUMN_ENTRY_LS(1, m_yuanbao)
		COLUMN_ENTRY_LS(2, m_yuanbaoTicket)
		COLUMN_ENTRY_LS(3, m_canupdate)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_UpdateYuanBao)
		COLUMN_ENTRY(1, m_acct_id)
		COLUMN_ENTRY(2, m_changeyuanbao)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_UpdateYuanBaoTicket)
		COLUMN_ENTRY(1, m_acct_id)
		COLUMN_ENTRY(2, m_changeyuanbaoticket)
		COLUMN_ENTRY(3, m_ratioPerYBT)
	END_PARAM_MAP_FUNC()

	BEGIN_COLUMN_MAP_FUNC(_GetIdentity)
		COLUMN_ENTRY_LS(1, m_acct_id)
	END_COLUMN_MAP()

	//BEGIN_COLUMN_MAP_FUNC(_GetYuanBao)
	//	COLUMN_ENTRY_LS(1, m_yuanbao)
	//END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetYuanBao)
		COLUMN_ENTRY_LS(1, m_yuanbao)
		COLUMN_ENTRY_LS(2, m_yuanbaoLock)
		COLUMN_ENTRY_LS(3, m_yuanbaoTicket)
		COLUMN_ENTRY_LS(4, m_forceUnlockDate)	
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ResetAssetPassDate)
		COLUMN_ENTRY(1, m_forceUnlockDate)
		COLUMN_ENTRY(2, m_acct_id)	
	END_PARAM_MAP_FUNC()

	BEGIN_COLUMN_MAP_FUNC(_GetAssetPass)
		COLUMN_ENTRY_LS(1, m_assetLockPassword)
		COLUMN_ENTRY_LS(2, m_forceUnlockDate)	
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ResetAssetPass)
		COLUMN_ENTRY(1, m_assetLockPassword)
		COLUMN_ENTRY(2, m_forceUnlockDate)
		COLUMN_ENTRY(3, m_acct_id)	
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_loginName)
		COLUMN_ENTRY(2, m_password_hash)
		COLUMN_ENTRY(3, m_disconnect_time)	
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Delete)
		COLUMN_ENTRY(1, m_acct_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, acct_id)
		COLUMN_ENTRY_TEST_ELSE(disconnect_time)
		COLUMN_ENTRY_TEST_ELSE(offLnTime)
		COLUMN_ENTRY_TEST_ELSE(onLnTime)
		COLUMN_ENTRY_TEST_ELSE(banTime)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_UpdateAddict)
		COLUMN_ENTRY(1, m_disconnect_time)
		COLUMN_ENTRY(2, m_offLnTime)
		COLUMN_ENTRY(3, m_onLnTime)	
		COLUMN_ENTRY(4, m_acct_id)	
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_UpdateYuanBaoLock)
		COLUMN_ENTRY(1, m_yuanbaoLock)
		COLUMN_ENTRY(2, m_acct_id)	
	END_PARAM_MAP_FUNC()	


	DEFINE_COMMAND_EX(CdboAccountAccessor, L"SELECT * FROM Account")

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboAccount : public Cdbo<CCommand<CAccessor<CdboAccountAccessor> > >
{
public: 
	
	HRESULT Insert(PCSTRING loginName, PCSTRING password_hash, DBTIMESTAMP discontTime)
	{
		SetGetBindFunc(_GetIdentity);
		SetGetParamsFunc(_Insert);
		COLUMN_STRING_COPY(m_loginName, loginName)
		COLUMN_STRING_COPY(m_password_hash, password_hash)
		m_disconnect_time = discontTime;
		
		return OpenCommand(_T("INSERT INTO Account (loginName, password_hash, disconnect_time) " 
			_T("OUTPUT INSERTED.acct_id VALUES (?, ?, ?)")));
	}

	HRESULT Delete(DWORD aid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Delete);
		m_acct_id.QuadPart = aid;
		return OpenCommand(_T("DELETE FROM Account WHERE acct_id = ?"));

		// void all account characters
		//return OpenCommand(_T("UPDATE CharacterView SET void = 1 WHERE acct_id = ?"));
	}

	HRESULT RSByID(DWORD aid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByID);
		m_acct_id.QuadPart = aid;
		return OpenCommand(_T("SELECT * FROM Account WHERE acct_id = ?"));
	}

	HRESULT GetYuaoBaoByID(DWORD aid)
	{
		SetGetBindFunc(_GetYuanBao);
		SetGetParamsFunc(_GetByID);
		m_acct_id.QuadPart = aid;
		return OpenCommand(_T("SELECT yuanbao, yuanbaoLock, yuanbaoTicket, forceUnlockDate FROM Account WHERE acct_id = ?"));
	}

	HRESULT GetAssetPasswordByID(DWORD aid)
	{
		SetGetBindFunc(_GetAssetPass);
		SetGetParamsFunc(_GetByID);
		m_acct_id.QuadPart = aid;
		return OpenCommand(_T("SELECT assetLockPassword, forceUnlockDate FROM Account WHERE acct_id = ?"));
	}

	HRESULT SetAssetPassword(DWORD aid, TCHAR pass[])
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ResetAssetPass);
		m_acct_id.QuadPart = aid;
		COLUMN_STRING_COPY(m_assetLockPassword, pass)	
		time_t zeroTime = 0;
		TimeConvert(zeroTime, m_forceUnlockDate);	
		return OpenCommand(_T("UPDATE Account SET assetLockPassword = ?,  forceUnlockDate = ? WHERE acct_id = ?"));
	}

	HRESULT ResetAssetDate(DWORD aid, UInt32 date)
	{
		//HRESULT hr;
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ResetAssetPassDate);
		m_acct_id.QuadPart = aid;
		TimeConvert((time_t) date, m_forceUnlockDate);	

		return OpenCommand(_T("UPDATE Account SET forceUnlockDate = ? WHERE acct_id = ?"));
	}

	HRESULT RSByName(LPCTSTR cszName)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByName);
		COLUMN_STRING_COPY(m_loginName, cszName)
		return OpenCommand(_T("SELECT * FROM Account WHERE loginName = ?"));
	}

	HRESULT RSByInactiveName(LPCTSTR cszName)
	{
		SetGetBindFunc(_GetInactive);
		SetGetParamsFunc(_GetByName);
		COLUMN_STRING_COPY(m_loginName, cszName)
		return OpenCommand(_T("SELECT acct_id, loginName, password_hash, banTime ")
			_T("FROM InactiveAccount WHERE loginName = ?"));
	}

	HRESULT UpdateYaunBao(UInt32 aid, Int32 change)
	{
		SetGetBindFunc(_GetUpdatedYuanBao);
		SetGetParamsFunc(_UpdateYuanBao);
		m_acct_id.QuadPart = aid;
		m_changeyuanbao.QuadPart = change;
		
#ifdef DB_MYSQL
		SetGetBindFunc(NULL);
		// some error reading problems for store procedure in mysql
		//return OpenCommand(_T("CALL sp_UpdateYuanBao(?, ?);"));
		return OpenCommand(_T("UPDATE Account SET yuanbao = yuanbao + ? WHERE acct_id = ?;"));
#else
		return OpenCommand(_T("EXEC sp_UpdateYuanBao ?, ?;"));
#endif // DB_MYSQL
	}

	HRESULT UpdateYaunBaoTicket(UInt32 aid, Int32 change, Int32 ratioPerYBT)
	{
		SetGetBindFunc(_GetUpdatedYuanBaoTicket);
		SetGetParamsFunc(_UpdateYuanBaoTicket);
		m_acct_id.QuadPart = aid;		
		m_changeyuanbaoticket.QuadPart = change;
		m_ratioPerYBT = ratioPerYBT;
		
#ifdef DB_MYSQL
		SetGetBindFunc(NULL);
		// some error reading problems for store procedure in mysql
		//return OpenCommand(_T("CALL sp_UpdateYuanBao(?, ?);"));
		//return OpenCommand(_T("UPDATE Account SET yuanbao = yuanbao + ? WHERE acct_id = ?;"));
#else
		return OpenCommand(_T("EXEC sp_UpdateYBT ?, ?, ?;"));
#endif // DB_MYSQL
	}

	HRESULT UpdateAddictGameInfo(UInt32 offTm, UInt32 onTm, DBTIMESTAMP disConTm, DWORD aid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_UpdateAddict);
		m_acct_id.QuadPart = aid;
		m_disconnect_time = disConTm;
		m_offLnTime = offTm;
		m_onLnTime = onTm;
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("UPDATE Account SET disconnect_time = ?, offLnTime = %d, onLnTime = %d WHERE acct_id = %d"),
			                              m_offLnTime, m_onLnTime, m_acct_id.QuadPart);	
		return OpenCommand(_T("UPDATE Account SET disconnect_time = ?, offLnTime = ?, onLnTime = ? WHERE acct_id = ?"));
	}

	HRESULT SetYuanbaoLock(UInt32 acctID, Boolean lock)
	{
		SetGetBindFunc(NULL);	
		SetGetParamsFunc(_UpdateYuanBaoLock);
		m_acct_id.QuadPart = acctID;
		m_yuanbaoLock = lock? 1:0;
		return OpenCommand(_T("UPDATE Account SET yuanbaoLock = ? WHERE acct_id = ?"));
	}
   
	HRESULT Reactivate(PCSTRING curAcctDb, PCSTRING curGameDb, PCSTRING inAcctDb, 
						PCSTRING inGameDb, PCSTRING cszName)
	{
		String sql;
		
		sql.Format(_T("EXEC sp_ReactiveAccount '%s', '%s', '%s', '%s', ?"), 
			curAcctDb, curGameDb, inAcctDb, inGameDb);
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_GetByName);
		COLUMN_STRING_COPY(m_loginName, cszName)
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Reactive SQL: %s"), sql.c_str())
		return OpenCommand(sql.c_str());
	}

	HRESULT UpdateByAny(DWORD acctid)
	{
		if (m_params.empty()) {
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UpdateByAny Error (1) [empty paramater] AccountID=%d"),
				acctid);
			return S_FALSE;
		}

		String sql(_T("UPDATE Account SET \""));
		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE acct_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_acct_id.QuadPart	= acctid;
		m_params.push_back(_T("acct_id"));
		
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}

};

#endif // _DBO_ACCOUNT_H_
