#pragma once
#ifndef _QUESTION_H_
#define _QUESTION_H_

#include "Stub/Logic/Question_Stub.h"
class CCharacter;
class CQuestion : public Question_Stub
	,public Reuben::Simulation::IEventHandler<CQuestion>
{
public:

	enum StateEnum {
		ST_NONE = 1,
		ST_ENTER,
		ST_QUESTION,
		LAST_STATE,	
	};

	enum EventEnum
	{
		EVT_START_QUESTION	= MAKE_EVENTID(Question_CLASSID,  1),
		EVT_NEXT_QUESTION	= MAKE_EVENTID(Question_CLASSID,  2),
		EVT_UPDATE_RANK		= MAKE_EVENTID(Question_CLASSID,  3),
		EVT_EXIT			= MAKE_EVENTID(Question_CLASSID,  4),
	};
	virtual void HandleEvent(CONST Event &event);
	virtual RPCResult Anwser(LPCPEER pPeer, RPCContext &context, const UInt32 &question_id,const UInt16 &anwerIndex);
	void SetOwner(CCharacter* pCharacter){ m_pOwner = pCharacter;}
	CCharacter* GetOwner(void){ return m_pOwner;};
	void SetCurQuestionId( UInt32 question_id){ m_curQuestionId = question_id;}
	UInt32 GetCurQuestionId(void){ return m_curQuestionId;};
	CQuestion();
	virtual ~CQuestion();
	void OnEnter();
	void OnExit();
	void OnStart();
	void UpdatePoint();
	void OnQuestionEnd();
	void CalQuestionRewards();

	void SetState(UInt8 nState){m_state = nState;}
	UInt8 GetState(void){return m_state;}

	INLINE static CQuestion* NewInstance();
	DEFINE_CLASS(Question_CLASSID);
private:
	void OnEventStartQuestion();
	void OnEventNextQuestion();
	void OnEventUpdateRank();
	void OnEventExit();
	void BroadcastAnwser(UInt32 anwerIndex);
private:
	typedef Question_Stub Parent;
	CCharacter* m_pOwner;
	UInt32 m_curQuestionId;
	UInt32 m_curAnswerIndex;
	UInt32 m_curAnswerIndexRight;
	time_t m_curQuestionTime; //current quetion start time
	time_t m_curAnswerTime;
	UInt32 m_oldMapId;
	UInt32 m_oldMapX;
	UInt32 m_oldMapY;
	UInt8 m_questionIndex;
	UInt8 m_state;


};
INLINE CQuestion* CQuestion::NewInstance()
{
	return CreateNetObject<CQuestion>();
}
#endif //_QUESTION_H_
