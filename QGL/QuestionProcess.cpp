//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "QuestionProcess.h"
//-- Library
#include "Global.h"
#include "QGLApplication.h"
#include "CharacterControl.h"
#include "CallLuaFunc.h"
#include "Proxy/Logic/Question.h"

CQuestionProcess::CQuestionProcess()
{
	m_pQuestion = NULL;
	m_curQuestionId = 0;
	m_PreUpdateTime = 0;
	m_CountDownTime = 0;
	m_CountDownAllTime = 0;

	m_StartPreUpdateTime = 0;
	m_StartCountDownTime = 0;
	m_StartCountDownAllTime = 0;

	m_CountDownEnt.SetFileNameW(_T("font_count.ent"));
	for (Int i = 0; i < (Int)m_CountDownEnt.m_NumEntLst.size(); i ++)
	{
		m_CountDownEnt.m_NumEntLst[i]->LockResources();
	}
	m_CountDownEnt.SetExtraZValue(0x20000000);
	m_CountDownEnt.SetVisible(FALSE);

}

CQuestionProcess::~CQuestionProcess()
{
	DeleteNetObject(m_pQuestion);
}

void CQuestionProcess::UpdateQuestion( const UInt32 &count,const UInt32 &question_id,const String &description, const String &option1, const String &option2, const String &option3, const String &option4)
{
	m_curQuestionId = question_id;

	m_CountDownEnt.SetNumber(5);
	m_CountDownEnt.SetVisible(TRUE);
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenQuestionPanel", String());

	UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("Question_Description");
	if (pText)
	{
		String str;
		str.Format(_T("  %d.%s"), count,description.c_str());
		pText->SetText(str.c_str());
		pText->Redraw();
		pText->SetEnable(true);
	}

	pText = gGlobal.pUIManager->FindUIStaticText("Question_Option1");
	if (pText)
	{
		pText->SetText(option1.c_str());
		pText->Redraw();
		pText->SetEnable(true);
	}

	pText = gGlobal.pUIManager->FindUIStaticText("Question_Option2");
	if (pText)
	{
		pText->SetText(option2.c_str());
		pText->Redraw();
		pText->SetEnable(true);
	}

	pText = gGlobal.pUIManager->FindUIStaticText("Question_Option3");
	if (pText)
	{
		pText->SetText(option3.c_str());
		pText->Redraw();
		pText->SetEnable(true);
	}

	pText = gGlobal.pUIManager->FindUIStaticText("Question_Option4");
	if (pText)
	{
		pText->SetText(option4.c_str());
		pText->Redraw();
		pText->SetEnable(true);
	}

	m_PreUpdateTime = ::timeGetTime();
	m_CountDownTime = 15 * 1000;
	m_CountDownAllTime = 15 * 1000;
	gGlobal.SetUILinerImage("Question_CountDownBar", m_CountDownAllTime / 1000, m_CountDownTime / 1000 );
	pText = gGlobal.pUIManager->FindUIStaticText("Question_CountDownText");
	if (pText)
	{
		String str;
		str.Format(_T("%d%s"), m_CountDownTime / 1000, gGlobal.GetStringTable()->Get(_T("MSG_SECOND")));
		pText->SetText(str.c_str());
		pText->Redraw();
		pText->SetEnable(true);
	}
}
void CQuestionProcess::UpdatePoint(UInt32 point)
{

	UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("Question_Score");
	if (pText)
	{
		String str;
		str.Format(_T("%d"), point);
		pText->SetText(str.c_str());
		pText->Redraw();
		pText->SetEnable(true);
	}
}
void CQuestionProcess::UpdateRankList(const String &name1, const UInt32 &point1, const String &name2, const UInt32 &point2, const String &name3, const UInt32 &point3, const String &name4, const UInt32 &point4, const String &name5, const UInt32 &point5, const String &name6, const UInt32 &point6, const String &name7, const UInt32 &point7, const String &name8, const UInt32 &point8, const String &name9, const UInt32 &point9, const String &name10, const UInt32 &point10)
{

	UIStaticText * pTextName = gGlobal.pUIManager->FindUIStaticText("QuestionRankName1");
	if (pTextName)
	{
		pTextName->SetText(name1.c_str());
		pTextName->Redraw();
		pTextName->SetEnable(true);
	}

	UIStaticText * pTextScore = gGlobal.pUIManager->FindUIStaticText("QuestionRankScore1");
	if (pTextScore)
	{
		String str;
		if(point1 > 0)
		{
			str.Format(_T("%d"),point1);
		}
		pTextScore->SetText(str.c_str());
		pTextScore->Redraw();
		pTextScore->SetEnable(true);
	}

	pTextName = gGlobal.pUIManager->FindUIStaticText("QuestionRankName2");
	if (pTextName)
	{
		pTextName->SetText(name2.c_str());
		pTextName->Redraw();
		pTextName->SetEnable(true);
	}

	pTextScore = gGlobal.pUIManager->FindUIStaticText("QuestionRankScore2");
	if (pTextScore)
	{
		String str;
		if(point2 > 0)
		{
			str.Format(_T("%d"),point2);
		}
		pTextScore->SetText(str.c_str());
		pTextScore->Redraw();
		pTextScore->SetEnable(true);
	}

	pTextName = gGlobal.pUIManager->FindUIStaticText("QuestionRankName3");
	if (pTextName)
	{
		pTextName->SetText(name3.c_str());
		pTextName->Redraw();
		pTextName->SetEnable(true);
	}

	pTextScore = gGlobal.pUIManager->FindUIStaticText("QuestionRankScore3");
	if (pTextScore)
	{
		String str;
		if(point3 > 0)
		{
			str.Format(_T("%d"),point3);
		}
		pTextScore->SetText(str.c_str());
		pTextScore->Redraw();
		pTextScore->SetEnable(true);
	}

	pTextName = gGlobal.pUIManager->FindUIStaticText("QuestionRankName4");
	if (pTextName)
	{
		pTextName->SetText(name4.c_str());
		pTextName->Redraw();
		pTextName->SetEnable(true);
	}

	pTextScore = gGlobal.pUIManager->FindUIStaticText("QuestionRankScore4");
	if (pTextScore)
	{
		String str;
		if(point4 > 0)
		{
			str.Format(_T("%d"),point4);
		}
		pTextScore->SetText(str.c_str());
		pTextScore->Redraw();
		pTextScore->SetEnable(true);
	}

	pTextName = gGlobal.pUIManager->FindUIStaticText("QuestionRankName5");
	if (pTextName)
	{
		pTextName->SetText(name5.c_str());
		pTextName->Redraw();
		pTextName->SetEnable(true);
	}

	pTextScore = gGlobal.pUIManager->FindUIStaticText("QuestionRankScore5");
	if (pTextScore)
	{
		String str;
		if(point5 > 0)
		{
			str.Format(_T("%d"),point5);
		}
		pTextScore->SetText(str.c_str());
		pTextScore->Redraw();
		pTextScore->SetEnable(true);
	}

	pTextName = gGlobal.pUIManager->FindUIStaticText("QuestionRankName6");
	if (pTextName)
	{
		pTextName->SetText(name6.c_str());
		pTextName->Redraw();
		pTextName->SetEnable(true);
	}

	pTextScore = gGlobal.pUIManager->FindUIStaticText("QuestionRankScore6");
	if (pTextScore)
	{
		String str;
		if(point6 > 0)
		{
			str.Format(_T("%d"),point6);
		}
		pTextScore->SetText(str.c_str());
		pTextScore->Redraw();
		pTextScore->SetEnable(true);
	}

	pTextName = gGlobal.pUIManager->FindUIStaticText("QuestionRankName7");
	if (pTextName)
	{
		pTextName->SetText(name7.c_str());
		pTextName->Redraw();
		pTextName->SetEnable(true);
	}

	pTextScore = gGlobal.pUIManager->FindUIStaticText("QuestionRankScore7");
	if (pTextScore)
	{
		String str;
		if(point7 > 0)
		{
			str.Format(_T("%d"),point7);
		}
		pTextScore->SetText(str.c_str());
		pTextScore->Redraw();
		pTextScore->SetEnable(true);
	}

	pTextName = gGlobal.pUIManager->FindUIStaticText("QuestionRankName8");
	if (pTextName)
	{
		pTextName->SetText(name8.c_str());
		pTextName->Redraw();
		pTextName->SetEnable(true);
	}

	pTextScore = gGlobal.pUIManager->FindUIStaticText("QuestionRankScore8");
	if (pTextScore)
	{
		String str;
		if(point8 > 0)
		{
			str.Format(_T("%d"),point8);
		}
		pTextScore->SetText(str.c_str());
		pTextScore->Redraw();
		pTextScore->SetEnable(true);
	}

	pTextName = gGlobal.pUIManager->FindUIStaticText("QuestionRankName9");
	if (pTextName)
	{
		pTextName->SetText(name9.c_str());
		pTextName->Redraw();
		pTextName->SetEnable(true);
	}

	pTextScore = gGlobal.pUIManager->FindUIStaticText("QuestionRankScore9");
	if (pTextScore)
	{
		String str;
		if(point9 > 0)
		{
			str.Format(_T("%d"),point9);
		}
		pTextScore->SetText(str.c_str());
		pTextScore->Redraw();
		pTextScore->SetEnable(true);
	}

	pTextName = gGlobal.pUIManager->FindUIStaticText("QuestionRankName10");
	if (pTextName)
	{
		pTextName->SetText(name10.c_str());
		pTextName->Redraw();
		pTextName->SetEnable(true);
	}

	pTextScore = gGlobal.pUIManager->FindUIStaticText("QuestionRankScore10");
	if (pTextScore)
	{
		String str;
		if(point10 > 0)
		{
			str.Format(_T("%d"),point10);
		}
		pTextScore->SetText(str.c_str());
		pTextScore->Redraw();
		pTextScore->SetEnable(true);
	}
}

void CQuestionProcess::Answer(UInt32 index)
{

	if(m_pQuestion != NULL)
	{
		m_pQuestion->Anwser(gGlobal.pPeerProxy,gGlobal.g_context,m_curQuestionId,index);
	}
}
void CQuestionProcess::ExitQuestion()
{
	gGlobal.g_pCharacter->GetCharNetObject()->ExitQuestion(gGlobal.pPeerProxy,gGlobal.g_context);
}
void CQuestionProcess::EnterQuestion()
{
	gGlobal.g_pCharacter->GetCharNetObject()->EnterQuestion(gGlobal.pPeerProxy,gGlobal.g_context);
}
void CQuestionProcess::OnCB_ExitQuestion()
{
	gGlobal.g_pCharacter->RemoveIcon();
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseQuestionPanel", String());
	DeleteNetObject(m_pQuestion);
	m_StartCountDownTime = 0;
	m_CountDownTime = 0;
	m_CountDownEnt.SetVisible(FALSE);
}
void CQuestionProcess::Update(UInt32 CurrTime)
{
	//check the count down time
	if(m_CountDownTime != 0)
	{

		if (CurrTime > m_PreUpdateTime)
		{
			if (m_CountDownTime > CurrTime - m_PreUpdateTime)
				m_CountDownTime -= CurrTime - m_PreUpdateTime;
			else
				m_CountDownTime = 0;
		}

		m_PreUpdateTime = CurrTime;
		gGlobal.SetUILinerImage("Question_CountDownBar", m_CountDownAllTime / 1000, m_CountDownTime / 1000 );
		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("Question_CountDownText");
		if (pText)
		{
			String str;
			str.Format(_T("%d%s"), m_CountDownTime / 1000, gGlobal.GetStringTable()->Get(_T("MSG_SECOND")));
			pText->SetText(str.c_str());
			pText->Redraw();
			pText->SetEnable(true);
		}
	}

	if(m_StartCountDownTime != 0)
	{

		if (CurrTime > m_StartPreUpdateTime)
		{
			if (m_StartCountDownTime > CurrTime - m_StartPreUpdateTime)
				m_StartCountDownTime -= CurrTime - m_StartPreUpdateTime;
			else
				m_StartCountDownTime = 0;
		}

		m_CountDownEnt.SetNumber(m_StartCountDownTime / 1000);
		m_StartPreUpdateTime = CurrTime;
	}
	else
	{
		m_CountDownEnt.SetVisible(FALSE);
	}

}
void CQuestionProcess::SetStartCountDown(UInt32 min)
{
	m_CountDownEnt.SetNumber(min);
	m_CountDownEnt.SetVisible(TRUE);
	m_StartPreUpdateTime = ::timeGetTime();
	m_StartCountDownTime = min * 1000;
	m_StartCountDownAllTime = min * 1000;
}

void CQuestionProcess::SetAnswerResult(Boolean bResult)
{
	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("AnswerResultImage");
	//if (!pAniIcon->GetAniEnt()->HasAniFile())

	if(pAniIcon)
	{
		pAniIcon->ClearAniData();
		pAniIcon->SetAnimationData(bResult?_T("QuestionAnswerRight.ent"):_T("QuestionAnswerError.ent"));
		pAniIcon->SetEnable(true);
	}
}