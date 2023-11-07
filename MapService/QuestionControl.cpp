//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self

#include "MapService/QuestionControl.h"
#include "MapService/Global.h"
#include "Common/common_binary.h"
#include "stub/Logic/Question.h"
#include "Resource/ServerResManager.h"
#include "Resource/QuestionManager.h"
#include "Resource/ResQuestion.h"
#include "stub/Logic/Map.h"
#include "Common/CallLuaFunc.h"
#include "Stub/Logic/ScriptSystem.h"
#include "Stub/Logic/Character.h"

#ifndef CONFIGURATION_INI_FILE
#define CONFIGURATION_INI_FILE		_T(".\\Data\\Setting\\fileconfig.ini")
#endif
PROTO_LuaPush(CCharacter)
CQuestionControl::CQuestionControl()
{
	m_QuestionMapID = GetPrivateProfileInt(_T("Question"), _T("QuestionMapID"), 20102, CONFIGURATION_INI_FILE);
	m_QuestionMapX = GetPrivateProfileInt(_T("Question"), _T("QuestionMapX"), 21, CONFIGURATION_INI_FILE);
	m_QuestionMapY = GetPrivateProfileInt(_T("Question"), _T("QuestionMapY"), 86, CONFIGURATION_INI_FILE);
	m_QuestionCount = GetPrivateProfileInt(_T("Question"), _T("QuestionCount"), 10, CONFIGURATION_INI_FILE);
}

CQuestionControl::~CQuestionControl()
{

}
bool CQuestionControl::point_compare(CQuestionControl::CharInfo& p1, CQuestionControl::CharInfo& p2)
{
	return p1.point > p2.point;
}

UInt32 CQuestionControl::GetRank(String name)
{
	if(!IsOver())
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::GetRank !IsOver()"));
		return 0;
	}

	bool bInList = false;
	for(int i = 0; i < m_charInfoVec.size(); i++)
	{
		if(m_charInfoVec[i].name == name)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CQuestionControl::GetRank rank = %d"),i+1);
			return i+1;
		}
	}

	return 0;
}

BOOL CQuestionControl::IsOver(CQuestion* pQuestion)
{
	if(m_questionVec.size() == 0)
	{
		return TRUE;
	}
	else
	{
		QuestionVec::iterator iter;
		QuestionVec::iterator iterend = m_questionVec.end();
		for( iter = m_questionVec.begin(); iter != iterend; ++iter)
		{
			
			CQuestion*pQ = *iter;
			
			if(pQuestion == pQ)
			{
				continue;
			}

			if(pQ->GetState() == CQuestion::ST_QUESTION)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
void CQuestionControl::UpdateQuestionRankList(String name,UInt32 point)
{
	CharInfo info;
	info.name = name;
	info.point = point;

	bool bInList = false;
	for(int i = 0; i < m_charInfoVec.size(); i++)
	{
		if(m_charInfoVec[i].name == name)
		{
			m_charInfoVec[i].point = point;
			bInList = true;
			break;
		}
	}

	if(!bInList)
	{
		m_charInfoVec.push_back(info);
	}

	sort(m_charInfoVec.begin(),m_charInfoVec.end(),point_compare);

	if(m_charInfoVec.size() == 11)
	{
		m_charInfoVec.pop_back();
	}


}

BOOL CQuestionControl::CanEnter(CCharacter* pCharacter)
{
	if(GetState() != ST_PREPARE)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::OnQuestionEnter GetState() != ST_PREPARE"));
		if(!IsOver())
			pCharacter->GetScript()->CB_ShowHint(pCharacter->GetPeer(), pCharacter->GetContext(), _T("MSG_QUESTION_ENTER_FAIL_RUNNING"));
		else
			pCharacter->GetScript()->CB_ShowHint(pCharacter->GetPeer(), pCharacter->GetContext(), _T("MSG_QUESTION_ENTER_FAIL"));

		return FALSE;
	}

	if(pCharacter->m_pQuestion == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::OnQuestionEnter pOwner->m_pQuestion == NULL"));
		return FALSE;
	}

	if(pCharacter->GetLevel() < 20)
	{
		pCharacter->GetScript()->CB_ShowHint(pCharacter->GetPeer(), pCharacter->GetContext(), _T("MSG_QUESTION_FAIL_LEVEL"));
		return FALSE;
	}

	if(pCharacter->GetParty() != NULL)
	{
		pCharacter->GetScript()->CB_ShowHint(pCharacter->GetPeer(), pCharacter->GetContext(), _T("MSG_QUESTION_FAIL_PARTY"));
		return FALSE;
	}

	if(pCharacter->GetState()==CCharacter::ST_BATTLE)
	{
		pCharacter->GetScript()->CB_ShowHint(pCharacter->GetPeer(), pCharacter->GetContext(), _T("MSG_QUESTION_FAIL_BATTLE"));
		return FALSE;
	}

	if(pCharacter->GetMap()&&pCharacter->GetMap()->GetLine_id() == 0)
	{
		pCharacter->GetScript()->CB_ShowHint(pCharacter->GetPeer(), pCharacter->GetContext(), _T("MSG_QUESTION_FAIL_INSTANCE"));
		return FALSE;
	}

	if(HasLuaFunc(gGlobal.g_pLuaState, "CheckJumpMap"))
	{
		CallLuaFunc(gGlobal.g_pLuaState, "CheckJumpMap", String(), pCharacter);
		UInt32 result = (UInt32)gGlobal.g_TempResult;
		if( result == 0 )
			return FALSE;
	}

	return TRUE;
}

void CQuestionControl::OnQuestionEnter(CCharacter* pOwner)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestionControl::OnQuestionEnter"));

	if(CanEnter(pOwner) == FALSE)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestionControl::OnQuestionEnter CanEnter(pOwner) == FALSE"));
		return;
	}
	
	pOwner->CB_EnterQuestion(pOwner->GetPeer(),pOwner->GetContext(),pOwner->m_pQuestion);
	Remove(pOwner->m_pQuestion);
	m_questionVec.push_back(pOwner->m_pQuestion);
	pOwner->m_pQuestion->OnEnter();
}
void CQuestionControl::OnQuestionExit(CQuestion*& pQuestion,BOOL bNormal)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestionControl::OnQuestionExit"));
	if(pQuestion == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::OnQuestionExit pQuestion == NULL"));
		return;
	}

	CCharacter* pCharacter = pQuestion->GetOwner();
	if(pCharacter == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::OnQuestionExit pCharacter == NULL"));
		return;
	}

	if(bNormal == TRUE)
	{	
		if(!IsOver(pQuestion))
		{
			pCharacter->GetScript()->CB_ShowHint(pCharacter->GetPeer(), pCharacter->GetContext(), _T("MSG_QUESTION_EXIT_NONORMAL"));
		}
		pQuestion->OnExit();
		CCharacter* pOwner = pQuestion->GetOwner();
		if(pOwner == NULL)
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::OnQuestionExit pOwner == NULL"));
			return;
		}
		pOwner->CB_ExitQuestion(pOwner->GetPeer(),pOwner->GetContext());
	}

	pQuestion->SetState(CQuestion::ST_NONE);
	pQuestion->CancelAllEvent();
	Remove(pQuestion);
	TRACE_INFODTL(GLOBAL_LOGGER, _F("end"));
}

void CQuestionControl::SendRankList(CQuestion*pQuestion)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestionControl::SendRankList"));

	if(pQuestion)
	{
		UInt32 nSize = m_charInfoVec.size();
		if(nSize >= 1)
		{
			pQuestion->SetName1(m_charInfoVec[0].name);
			pQuestion->SetPoint1(m_charInfoVec[0].point);
		}
		else
		{
			pQuestion->SetName1("");
			pQuestion->SetPoint1(0);
		}

		if(nSize >= 2)
		{
			pQuestion->SetName2(m_charInfoVec[1].name);
			pQuestion->SetPoint2(m_charInfoVec[1].point);
		}
		else
		{
			pQuestion->SetName2("");
			pQuestion->SetPoint2(0);
		}

		if(nSize >= 3)
		{
			pQuestion->SetName3(m_charInfoVec[2].name);
			pQuestion->SetPoint3(m_charInfoVec[2].point);
		}
		else
		{
			pQuestion->SetName3("");
			pQuestion->SetPoint3(0);
		}

		if(nSize >= 4)
		{
			pQuestion->SetName4(m_charInfoVec[3].name);
			pQuestion->SetPoint4(m_charInfoVec[3].point);
		}
		else
		{
			pQuestion->SetName4("");
			pQuestion->SetPoint4(0);
		}

		if(nSize >= 5)
		{
			pQuestion->SetName5(m_charInfoVec[4].name);
			pQuestion->SetPoint5(m_charInfoVec[4].point);
		}
		else
		{
			pQuestion->SetName5("");
			pQuestion->SetPoint5(0);
		}

		if(nSize >= 6)
		{
			pQuestion->SetName6(m_charInfoVec[5].name);
			pQuestion->SetPoint6(m_charInfoVec[5].point);
		}
		else
		{
			pQuestion->SetName6("");
			pQuestion->SetPoint6(0);
		}

		if(nSize >= 7)
		{
			pQuestion->SetName7(m_charInfoVec[6].name);
			pQuestion->SetPoint7(m_charInfoVec[6].point);
		}
		else
		{
			pQuestion->SetName7("");
			pQuestion->SetPoint7(0);
		}

		if(nSize >= 8)
		{
			pQuestion->SetName8(m_charInfoVec[7].name);
			pQuestion->SetPoint8(m_charInfoVec[7].point);
		}
		else
		{
			pQuestion->SetName8("");
			pQuestion->SetPoint8(0);
		}

		if(nSize >= 9)
		{
			pQuestion->SetName9(m_charInfoVec[8].name);
			pQuestion->SetPoint9(m_charInfoVec[8].point);
		}
		else
		{
			pQuestion->SetName9("");
			pQuestion->SetPoint9(0);
		}

		if(nSize >= 10)
		{
			pQuestion->SetName10(m_charInfoVec[9].name);
			pQuestion->SetPoint10(m_charInfoVec[9].point);
		}
		else
		{
			pQuestion->SetName10("");
			pQuestion->SetPoint10(0);
		}

		pQuestion->SendAttr(pQuestion->GetOwner()->GetPeer(),pQuestion->GetOwner()->GetContext(),pQuestion->GetUpdateMask());

	}

}


void CQuestionControl::Prepare()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestionControl::Prepare"));
	if(GetState() == ST_PREPARE)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::Prepare GetState() == ST_PREPARE"));
		return;
	}

	if(!IsOver())
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::Prepare !IsOver()"));
		return;
	}

	if(m_questionVec.size() > 0)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::Prepare m_questionVec.size() > 0"));
		QuestionVec::iterator iter;
		QuestionVec::iterator iterend = m_questionVec.end();
		for( iter = m_questionVec.begin(); iter != iterend; ++iter)
		{
			CQuestion*pQuestion = *iter;
			pQuestion->SetState(CQuestion::ST_NONE);
			//if(pQuestion)
			//{
			//	pQuestion->OnExit();
			//	CCharacter* pOwner = pQuestion->GetOwner();
			//	if(pOwner == NULL)
			//	{
			//		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::OnQuestionExit pOwner == NULL"));
			//		return;
			//	}
			//	pOwner->CB_ExitQuestion(pOwner->GetPeer(),pOwner->GetContext());
			//}
		}
		m_questionVec.clear();
		//return;
	}

	m_charInfoVec.clear();
	SetState(ST_PREPARE);

	m_QuestionMapID = GetPrivateProfileInt(_T("Question"), _T("QuestionMapID"), 20102, CONFIGURATION_INI_FILE);
	m_QuestionMapX = GetPrivateProfileInt(_T("Question"), _T("QuestionMapX"), 21, CONFIGURATION_INI_FILE);
	m_QuestionMapY = GetPrivateProfileInt(_T("Question"), _T("QuestionMapY"), 86, CONFIGURATION_INI_FILE);
	m_QuestionCount = GetPrivateProfileInt(_T("Question"), _T("QuestionCount"), 10, CONFIGURATION_INI_FILE);	
	
	m_questionDataVec.clear();
	int nQuesitonDataSize = gGlobal.m_resource->GetQuestionDataSize(); 
	if(nQuesitonDataSize < m_QuestionCount)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CQuestionControl::Prepare: not enough questions in question.csv current count %d,need%d "),nQuesitonDataSize,m_QuestionCount);
		SetState(ST_NONE);
		return;
	}
	UInt16 r = GetRand(nQuesitonDataSize);
	for (UInt16 i = r; i < r + nQuesitonDataSize; ++i)
	{
		QuestionData* pQuestionData = const_cast<QuestionData*>(gGlobal.m_resource->GetQuestionData(i % nQuesitonDataSize));
		if (pQuestionData != NULL && find(m_questionDataVec.begin(),m_questionDataVec.end(),pQuestionData) == m_questionDataVec.end())
		{
			m_questionDataVec.push_back(pQuestionData);
			if(m_questionDataVec.size() >= m_QuestionCount)
			{
				break;
			}
		}
	}
}
void CQuestionControl::Start()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestionControl::Start"));
	SetState(ST_NONE);

	QuestionVec::iterator iter;
	QuestionVec::iterator iterend = m_questionVec.end();
	for( iter = m_questionVec.begin(); iter != iterend; ++iter)
	{
		CQuestion*pQuestion = *iter;
		if(pQuestion)
		{
			pQuestion->OnStart();
		}
	}
}
void CQuestionControl::Remove(CQuestion*& pQuestion)
{
	CCharacter* pOwner = pQuestion->GetOwner();
	if(pOwner == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::Remove pOwner == NULL"));
		return;
	}
	CharInfo info;
	info.name = pOwner->GetNickName();
	info.point = pQuestion->GetPoint();
	CharInfoVec::iterator it = find(m_charInfoVec.begin(),m_charInfoVec.end(),info);
	if(it != m_charInfoVec.end())
	{
		m_charInfoVec.erase(it);
	}

	QuestionVec::iterator iter = find(m_questionVec.begin(),m_questionVec.end(),pQuestion);
	if(iter != m_questionVec.end())
	{
		m_questionVec.erase(iter);
	}
}
void CQuestionControl::NotifyEnd(const CQuestion* pQuestion)
{
	if(IsOver())
	{
		QuestionVec::iterator iter;
		QuestionVec::iterator iterend = m_questionVec.end();
		for( iter = m_questionVec.begin(); iter != iterend; ++iter)
		{
			CQuestion*pQuestion = *iter;
			if(pQuestion)
			{
				pQuestion->CalQuestionRewards();
			}
		}
	}
}
