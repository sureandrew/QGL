#ifndef _DBO_GENERATION_H_
#define _DBO_GENERATION_H_

#include "Dbo.h"

class CdboGenerationAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboGenerationAccessor)

		DEFINE_ENTRY(i64YoungerId, LARGE_INTEGER)
		DEFINE_ENTRY(i64ElderId, LARGE_INTEGER)
		DEFINE_ENTRY(iAccOrRecvExp, INT)
		DEFINE_ENTRY(iGiftPointOrHisAcc, INT)
		DEFINE_ENTRY_STR(strYoungerName, 21)
		DEFINE_ENTRY_STR(strElderName, 21)
		DEFINE_ENTRY(iRecvExpForCrt,INT)

		// In order to fix several issues with some providers, the code below may bind
		// columns in a different order than reported by the provider
		//ENABLE_COLUMN_MAP(CdboSkillAccessor)
		//ENABLE_PARAM_MAP(CdboSkillAccessor)
		BEGIN_COLUMN_MAP_FUNC(_GetAllForCreate)
		COLUMN_ENTRY_LS(1, m_i64YoungerId)
		COLUMN_ENTRY_LS(2, m_i64ElderId)
		COLUMN_ENTRY_LS(3, m_iAccOrRecvExp)
		COLUMN_ENTRY_LS(4, m_iGiftPointOrHisAcc)
		COLUMN_ENTRY_LS(5, m_iRecvExpForCrt)
		COLUMN_ENTRY_LS(6, m_strYoungerName)
		COLUMN_ENTRY_LS(7, m_strElderName)
		END_COLUMN_MAP()

		BEGIN_COLUMN_MAP_FUNC(_GetAllAsYounger)
		COLUMN_ENTRY_LS(1, m_i64YoungerId)
		COLUMN_ENTRY_LS(2, m_i64ElderId)
		COLUMN_ENTRY_LS(3, m_iAccOrRecvExp)
		COLUMN_ENTRY_LS(4, m_strElderName)
		END_COLUMN_MAP()

		BEGIN_COLUMN_MAP_FUNC(_GetAllAsElder)
		COLUMN_ENTRY_LS(1, m_i64YoungerId)
		COLUMN_ENTRY_LS(2, m_i64ElderId)
		COLUMN_ENTRY_LS(3, m_iAccOrRecvExp)
		COLUMN_ENTRY_LS(4, m_iGiftPointOrHisAcc)
		COLUMN_ENTRY_LS(5, m_strYoungerName)
		END_COLUMN_MAP()

		BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_i64YoungerId)
		COLUMN_ENTRY(2, m_i64ElderId)
		COLUMN_ENTRY(3, m_strYoungerName)
		COLUMN_ENTRY(4, m_strElderName)
		END_PARAM_MAP_FUNC()
		
		BEGIN_PARAM_MAP_FUNC(_Delete)
		COLUMN_ENTRY(1, m_i64YoungerId)
		COLUMN_ENTRY(2, m_i64ElderId)
		END_PARAM_MAP_FUNC()

		BEGIN_PARAM_MAP_FUNC(_DeleteAsYoung)
		COLUMN_ENTRY(1, m_i64YoungerId)
		END_PARAM_MAP_FUNC()

		BEGIN_PARAM_MAP_FUNC(_DeleteAsElder)
		COLUMN_ENTRY(1, m_i64ElderId)
		END_PARAM_MAP_FUNC()

		BEGIN_PARAM_MAP_FUNC(_GetByIDAsYoung)
		COLUMN_ENTRY(1, m_i64YoungerId)
		END_PARAM_MAP_FUNC()

		BEGIN_PARAM_MAP_FUNC(_GetByIDAsElder)
		COLUMN_ENTRY(1, m_i64ElderId)
		END_PARAM_MAP_FUNC()

		BEGIN_PARAM_MAP_FUNC(_GetSpecialByIDAsYoung)
		COLUMN_ENTRY(1, m_i64ElderId)
		COLUMN_ENTRY(2, m_i64YoungerId)
		END_PARAM_MAP_FUNC()

		BEGIN_PARAM_MAP_FUNC(_GetSpecialByIDAsElder)
		COLUMN_ENTRY(1, m_i64ElderId)
		COLUMN_ENTRY(2, m_i64YoungerId)
		END_PARAM_MAP_FUNC()

		BEGIN_PARAM_MAP_FUNC(_UpdateGenerationAsYoung)
		COLUMN_ENTRY(1, m_iAccOrRecvExp)
		COLUMN_ENTRY(2, m_i64YoungerId)
		COLUMN_ENTRY(3, m_i64ElderId)
		END_PARAM_MAP_FUNC()

		BEGIN_PARAM_MAP_FUNC(_UpdateGenerationAsElder)
		COLUMN_ENTRY(1, m_iAccOrRecvExp)
		COLUMN_ENTRY(2, m_iGiftPointOrHisAcc)
		COLUMN_ENTRY(3, m_i64YoungerId)
		COLUMN_ENTRY(4, m_i64ElderId)
		END_PARAM_MAP_FUNC()

		DEFINE_COMMAND_EX(CdboGenerationAccessor, L"SELECT * FROM Generation")
};

class CdboGeneration : public Cdbo<CCommand<CAccessor<CdboGenerationAccessor> > >
{
public:
	HRESULT Insert(DWORD dwYoungerId, 
				DWORD dwElderId,
				BOOL bAsYounger,
				LPCTSTR cszYoungerName,
				LPCTSTR cszElderName)
	{
		SetGetBindFunc(_GetAllForCreate);
		SetGetParamsFunc(_Insert);
		m_i64YoungerId.QuadPart = dwYoungerId;
		m_i64ElderId.QuadPart = dwElderId;
		COLUMN_STRING_COPY(m_strYoungerName, cszYoungerName)
		COLUMN_STRING_COPY(m_strElderName, cszElderName)
		return OpenCommand(_T("INSERT INTO Generation (YoungerId, ElderId,YoungerName,ElderName) OUTPUT INSERTED.* VALUES (?,?,?,?)"));
	}

	HRESULT Delete(DWORD dwYoungerId, DWORD dwElderId)
	{
		if (0 == dwYoungerId && 0 != dwElderId) 
		{
			SetGetBindFunc(NULL);
			SetGetParamsFunc(_DeleteAsElder);
			m_i64ElderId.QuadPart = dwElderId;
			return OpenCommand(_T("DELETE FROM Generation WHERE ElderId = ?"));
		}
		else if (0 == dwElderId && 0 != dwYoungerId) 
		{
			SetGetBindFunc(NULL);
			SetGetParamsFunc(_DeleteAsYoung);
			m_i64YoungerId.QuadPart = dwYoungerId;
			return OpenCommand(_T("DELETE FROM Generation WHERE YoungerId = ?"));
		}

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Delete);
		m_i64YoungerId.QuadPart = dwYoungerId;
		m_i64ElderId.QuadPart = dwElderId;
		// delete Generation
		return OpenCommand(_T("DELETE FROM Generation WHERE YoungerId = ? AND ElderId = ?"));
	}

	HRESULT RSByID(DWORD dwYoungerId, DWORD dwElderId)
	{
		if (0 == dwYoungerId && 0 != dwElderId) 
		{
			SetGetBindFunc(_GetAllAsElder);
			SetGetParamsFunc(_GetByIDAsElder);
			m_i64ElderId.QuadPart = dwElderId;
			return OpenCommand(_T("SELECT YoungerId, ElderId, DevoteAccmulation,GiftPoint,YoungerName FROM Generation WHERE ElderId = ?"));
		}
		else if (0 == dwElderId && 0 != dwYoungerId) 
		{
			SetGetBindFunc(_GetAllAsYounger);
			SetGetParamsFunc(_GetByIDAsYoung);
			m_i64YoungerId.QuadPart = dwYoungerId;
			return OpenCommand(_T("SELECT YoungerId, ElderId, RecvExp,ElderName FROM Generation WHERE YoungerId = ?"));
		}
		return HRESULT(0);
	}

	HRESULT UpdateGeneration(DWORD dwYoungerId,DWORD dwElderId,int iAccOrRecvExp,
							UInt32 iGiftPointOrHisAcc,BOOL bAsYounger)
	{
		if (bAsYounger) {
			SetGetBindFunc(NULL);
			SetGetParamsFunc(_UpdateGenerationAsYoung);
			m_iAccOrRecvExp = iAccOrRecvExp;
			m_i64YoungerId.QuadPart = dwYoungerId;
			m_i64ElderId.QuadPart = dwElderId;
			return OpenCommand(_T("UPDATE Generation SET RecvExp =? WHERE YoungerId = ? AND ElderId =?"));
		}
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_UpdateGenerationAsElder);
		m_iAccOrRecvExp = iAccOrRecvExp;
		m_iGiftPointOrHisAcc = iGiftPointOrHisAcc;
		m_i64YoungerId.QuadPart = dwYoungerId;
		m_i64ElderId.QuadPart = dwElderId;
		return OpenCommand(_T("UPDATE Generation SET DevoteAccmulation =?,GiftPoint=? WHERE YoungerId = ? AND ElderId =?"));
	}
};

#endif