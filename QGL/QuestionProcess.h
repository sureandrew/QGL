#pragma once
#ifndef _QUESTIONPROCESS_H_
#define _QUESTIONPROCESS_H_
#include <Reuben/Utility/Singleton.h>
#include "Common.h"
#include "NumberEnt.h"
class CQuestion;
class CQuestionProcess:public Reuben::Utility::Singleton<CQuestionProcess>
{
public:
	CQuestionProcess();
	virtual ~CQuestionProcess();

	void UpdateQuestion( const UInt32 &count,const UInt32 &question_id,const String &description, const String &option1, const String &option2, const String &option3, const String &option4);
	void UpdatePoint(UInt32 point);
	void UpdateRankList(const String &name1, const UInt32 &point1, const String &name2, const UInt32 &point2, const String &name3, const UInt32 &point3, const String &name4, const UInt32 &point4, const String &name5, const UInt32 &point5, const String &name6, const UInt32 &point6, const String &name7, const UInt32 &point7, const String &name8, const UInt32 &point8, const String &name9, const UInt32 &point9, const String &name10, const UInt32 &point10);
	void Answer(UInt32 index);
	void EnterQuestion();
	void ExitQuestion();
	void OnCB_ExitQuestion();
	void Update(UInt32 CurrTime);
	void SetStartCountDown(UInt32 min);
	void SetAnswerResult(Boolean bResult);
	inline CNumEnt * GetCountDownEnt(){return &m_CountDownEnt;}

	void SetQuestion(CQuestion*pQuestion){ m_pQuestion = pQuestion;	m_CountDownEnt.SetNumber(6);m_CountDownEnt.SetVisible(TRUE);}
	CQuestion* GetQuestion(){ return m_pQuestion;	}
private:
	CQuestion*         m_pQuestion;
	UInt32			   m_curQuestionId;
	UInt m_PreUpdateTime;
	UInt m_CountDownTime;
	UInt m_CountDownAllTime;
	CNumEnt m_CountDownEnt;

	UInt m_StartPreUpdateTime;
	UInt m_StartCountDownTime;
	UInt m_StartCountDownAllTime;

};


#endif // _QUESTIONPROCESS_H_