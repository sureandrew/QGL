//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Question.h"
#include "MapService/Global.h"
#include "Common/common_binary.h"
#include "stub/Logic/Question.h"
#include "Resource/ServerResManager.h"
#include "Resource/QuestionManager.h"
#include "Resource/ResQuestion.h"
#include "stub/Logic/Map.h"
#include "Common/CallLuaFunc.h"
#include "QuestionControl.h"
#include "MapService/ViewManager.h"
#include "ScriptSystem.h"

REGISTER_CLASSTYPE(CQuestion, Question_CLASSID);
PROTO_LuaPush(CCharacter)
CQuestion::CQuestion()
{
	m_pOwner = NULL;
	m_curQuestionId = 0;
	m_curAnswerIndex = 0;
	m_curAnswerIndexRight = 0;
	m_oldMapId = 0;
	m_oldMapX = 0;
	m_oldMapY = 0;
	m_questionIndex = 0;
	m_state = 0;
}
CQuestion::~CQuestion()
{
	CancelAllEvent();
}
void CQuestion::BroadcastAnwser(UInt32 anwerIndex)
{
	if (m_pOwner->m_viewManager)
	{
		CharacterPtrList chars;
		m_pOwner->m_viewManager->GetCharacters(chars);

		for (CharacterPtrList::iterator it = chars.begin(); it != chars.end(); ++it)
		{
			if (*it && !(*it)->IsInBattle() )
			{
				(*it)->CB_AnswerQuestion((*it)->GetPeer(), (*it)->GetContext(),anwerIndex,m_pOwner->GetChar_id());
			}
		}
	}

	if (m_pOwner->GetPeer() && !m_pOwner->IsInBattle())
		m_pOwner->CB_AnswerQuestion(m_pOwner->GetPeer(), m_pOwner->GetContext(),anwerIndex,m_pOwner->GetChar_id());

}
RPCResult CQuestion::Anwser(LPCPEER pPeer, RPCContext &context, const UInt32 &question_id,const UInt16 &anwerIndex)
{
	if(GetState() != ST_QUESTION)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestion::Anwser GetState() != ST_QUESTION"));
		return RPC_RESULT_OK;
	}
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestion::Anwser"));
	if(question_id != m_curQuestionId)
	{
		return RPC_RESULT_OK;
	}
	m_curAnswerIndex = anwerIndex;
	m_curAnswerTime = ::time(NULL);
	BroadcastAnwser(anwerIndex);
	return RPC_RESULT_OK;
}
void CQuestion::OnStart()
{
	RaiseUniqEventID(EVT_START_QUESTION, 60 * 1000);
	CB_StartCountDown(m_pOwner->GetPeer(),m_pOwner->GetContext(),60);
}
void CQuestion::OnEnter()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestion::OnEnter"));
	if(m_pOwner != NULL && GetState() != ST_ENTER)
	{
		m_oldMapId = m_pOwner->GetMap_id();
		m_oldMapX = m_pOwner->GetMap_x();
		m_oldMapY = m_pOwner->GetMap_y();
		m_pOwner->JumpMap(CQuestionControl::GetInstance().GetQuestionMapID(),CQuestionControl::GetInstance().GetQuestionMapX()
			,CQuestionControl::GetInstance().GetQuestionMapY());
		m_pOwner->GetScript()->CB_ShowHint(m_pOwner->GetPeer(), m_pOwner->GetContext(), _T("MSG_QUESTION_WELCOME"));

		SetState(ST_ENTER);
	}

}
void CQuestion::OnExit()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestion::OnExit"));
	if(m_pOwner != NULL && m_oldMapId != 0)
	{
		m_pOwner->JumpMap(m_oldMapId,m_oldMapX,m_oldMapY);
		m_oldMapId = 0;
		m_oldMapX = 0;
		m_oldMapY = 0;
	}
	OnQuestionEnd();
}

void CQuestion::UpdatePoint()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestion::UpdatePoint"));
	do
	{
		if(m_curAnswerIndexRight == m_curAnswerIndex && m_curAnswerTime != 0 && m_curAnswerIndexRight != 0)
		{
			if(m_curQuestionTime > m_curAnswerTime)
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestion::UpdatePoint m_curQuestionTime > m_curAnswerTime"));
				break;
			}
			UInt32 tDiff = (UInt32)difftime(m_curAnswerTime,m_curQuestionTime);
			UInt32 nPoint = 0;
			if(tDiff <= 5)//in 1 to 5 seconds
			{
				nPoint = 5;
			}
			else if(tDiff <= 10)//in 6 to 10 seconds
			{
				nPoint = 4;
			}
			else//in 11 to 15 seconds and delay
			{
				nPoint = 3;
			}
			SetPoint(GetPoint() + nPoint);
			//m_pOwner->GetScript()->CB_ShowHint(m_pOwner->GetPeer(), m_pOwner->GetContext(), _T("你答对了"));
			//String str;
			//str.Format(_T("大李小百科,第%d题回答正确,用时%d秒,得到积分%d."),m_questionIndex,tDiff,nPoint);
			m_pOwner->CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), 1, _T("MSG_QUESTION_ANSWER_RIGHT"));
			CB_AnswerResult(m_pOwner->GetPeer(), m_pOwner->GetContext(),BOOLEAN_TRUE);
			SendAttr(m_pOwner->GetPeer(),m_pOwner->GetContext(),GetUpdateMask());
			break;
		}
		else if(m_curAnswerIndexRight != m_curAnswerIndex && m_curAnswerTime != 0 && m_curAnswerIndexRight != 0)
		{
			//m_pOwner->GetScript()->CB_ShowHint(m_pOwner->GetPeer(), m_pOwner->GetContext(), _T("你答错了"));
			//String str;
			//str.Format(_T("大李小百科,第%d题回答错误!"),m_questionIndex);
			m_pOwner->CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), 1, _T("MSG_QUESTION_ANSWER_ERROR"));
			CB_AnswerResult(m_pOwner->GetPeer(), m_pOwner->GetContext(),BOOLEAN_FALSE);
		}
		else if(m_curAnswerTime == 0 && m_curAnswerIndexRight != 0 && m_curAnswerIndex == 0)
		{
			//m_pOwner->GetScript()->CB_ShowHint(m_pOwner->GetPeer(), m_pOwner->GetContext(), _T("你答错了"));
			//String str;
			//str.Format(_T("大李小百科,第%d题回答超时!"),m_questionIndex);
			m_pOwner->CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), 1, _T("MSG_QUESTION_ANSWER_TIMEOUT"));
			CB_AnswerResult(m_pOwner->GetPeer(), m_pOwner->GetContext(),BOOLEAN_FALSE);
		}
	}while(false);

	m_curAnswerTime = 0;
	m_curAnswerIndex = 0;
	m_curQuestionId = 0;
	m_curQuestionTime = 0;
	m_curAnswerIndexRight = 0;

}
void CQuestion::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_START_QUESTION:		
		OnEventStartQuestion();	
		break;
	case EVT_NEXT_QUESTION:		
		OnEventNextQuestion();	
		break;
	case EVT_UPDATE_RANK:		
		OnEventUpdateRank();	
		break;
	case EVT_EXIT:		
		OnEventExit();	
		break;
	default:
		break;
	}
}
void CQuestion::OnEventExit()
{
	CQuestionControl::GetInstance().OnQuestionExit(m_pOwner->m_pQuestion);
}
void CQuestion::OnEventUpdateRank()
{
	if(GetState() != ST_QUESTION)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestion::OnEventUpdateRank GetState() != ST_QUESTION"));
		return;
	}
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestionControl::OnEventUpdateRank"));
	CQuestionControl::GetInstance().SendRankList(this);
}
void CQuestion::OnEventStartQuestion()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestion::OnEventStartQuestion"));
	SetState(ST_QUESTION);
	SetPoint(0);
	m_questionIndex = 0;
	QuestionData* pQuestionData = CQuestionControl::GetInstance().GetQuestionData(m_questionIndex++);
	if(pQuestionData == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestion::OnStart pQuestionData == NULL"));
		return;
	}

	m_curAnswerIndexRight = pQuestionData->answer;
	m_curQuestionId = pQuestionData->question_id;
	CB_NextQuestion(m_pOwner->GetPeer(),m_pOwner->GetContext(),m_questionIndex,pQuestionData->question_id,pQuestionData->description
		,pQuestionData->option1,pQuestionData->option2,pQuestionData->option3,pQuestionData->option4);
	m_curQuestionTime = ::time(NULL);
	RaiseUniqEventID(EVT_UPDATE_RANK, 3 * 1000);
	RaiseUniqEventID(EVT_NEXT_QUESTION, 15 * 1000);
	//BroadcastAnwser(0);
}
void CQuestion::OnEventNextQuestion()
{
	if(GetState() != ST_QUESTION)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestion::OnEventNextQuestion GetState() != ST_QUESTION"));
		return;
	}

	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestionControl::OnEventNextQuestion"));
	UpdatePoint();
	if (GetUpdateMask().IsBit(CQuestion::pointMask) && GetPoint() > 0)
	{
		CQuestionControl::GetInstance().UpdateQuestionRankList(GetOwner()->GetNickName(),GetPoint());
	}

	QuestionData* pQuestionData = CQuestionControl::GetInstance().GetQuestionData(m_questionIndex++);
	if(pQuestionData == NULL)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestionControl::OnEventNextQuestion pQuestionData == NULL"));
		m_pOwner->GetScript()->CB_ShowHint(m_pOwner->GetPeer(), m_pOwner->GetContext(), _T("MSG_QUESTION_END"));
		CancelAllEvent();
		RaiseUniqEventID(EVT_EXIT, 3 * 60 * 1000);
		SetState(ST_NONE);
		CQuestionControl::GetInstance().NotifyEnd(this);

		return;
	}

	m_curAnswerIndexRight = pQuestionData->answer;
	m_curQuestionId = pQuestionData->question_id;
	CB_NextQuestion(m_pOwner->GetPeer(),m_pOwner->GetContext(),m_questionIndex,pQuestionData->question_id,pQuestionData->description
		,pQuestionData->option1,pQuestionData->option2,pQuestionData->option3,pQuestionData->option4);
	m_curQuestionTime = ::time(NULL);
	RaiseUniqEventID(EVT_UPDATE_RANK, 3 * 1000);
	RaiseUniqEventID(EVT_NEXT_QUESTION, 15 * 1000);
	//BroadcastAnwser(0);
}

void CQuestion::OnQuestionEnd()
{
	
	//BroadcastAnwser(0);
	CalQuestionRewards();
	//m_pOwner->CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), 1, _T("MSG_QUESTION_EXIT"));
	SetState(ST_NONE);
	CancelAllEvent();
}

void CQuestion::CalQuestionRewards()
{
	if(GetPoint() > 0 && CQuestionControl::GetInstance().IsOver())
	{
		if(!HasLuaFunc(gGlobal.g_pLuaState,"CalQuestionRewards"))
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CQuestionControl::OnQuestionEnd !HasLuaFunc gGlobal.g_pLuaState,CalQuestionRewards"));
			return;
		}

		CallLuaFunc(gGlobal.g_pLuaState,"CalQuestionRewards",String(),m_pOwner,GetPoint(),CQuestionControl::GetInstance().GetRank(m_pOwner->GetNickName()));

		SetPoint(0);
	}
}