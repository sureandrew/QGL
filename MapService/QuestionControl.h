#pragma once
#ifndef _QUESTION_CONTROL_H_
#define _QUESTION_CONTROL_H_
#include <Reuben/Utility/Singleton.h>
#include "Reuben/Simulation/Object.h"
#include "RPCEnum/Question_enum.h"
#include "Stub/Logic/Character.h"
class CQuestion;
class CCharacter;
struct QuestionData;
class CQuestionControl : 
	public Reuben::Simulation::Object
	,public Reuben::Utility::Singleton<CQuestionControl>
{
public:
	enum StateEnum {
		ST_NONE = 1,
		ST_PREPARE,
		LAST_STATE,	
	};

	struct CharInfo
	{
		String name;
		UInt16 point;
		bool operator==(const CharInfo&b) const
		{
			return name == b.name;
		}
	};
	typedef StlVector<CharInfo, ManagedAllocator<CharInfo> > CharInfoVec;
	typedef StlVector<CQuestion*, ManagedAllocator<CQuestion*> > QuestionVec;
	typedef StlVector<QuestionData*, ManagedAllocator<QuestionData*> > QuestionDataVec;
public:
	CQuestionControl();
	~CQuestionControl();

	static bool point_compare(CQuestionControl::CharInfo& p1, CQuestionControl::CharInfo& p2);

	UInt16 GetQuestionMapID(){return m_QuestionMapID;}
	UInt16 GetQuestionMapX(){return m_QuestionMapX;}
	UInt16 GetQuestionMapY(){return m_QuestionMapY;}

	QuestionData* GetQuestionData(UInt32 index)
	{
		if(index >= m_questionDataVec.size())
		{
			return NULL;
		}
		return m_questionDataVec[index];
	}
	void UpdateQuestionRankList(String name,UInt32 point);
	void OnQuestionEnter(CCharacter* pOwner);
	void OnQuestionExit(CQuestion*& pQuestion,BOOL bNormal = TRUE);
	void Remove(CQuestion*& pQuestion);
	UInt32 GetRank(String name);
	
	void Prepare();
	void Start();
	
	void SetState(UInt8 nState){m_state = nState;}
	UInt8 GetState(void){return m_state;}
	
	void NotifyEnd(const CQuestion* pQuestion);

	DEFINE_PARENT_CLASS(Reuben::Simulation::Object);
	DEFINE_CLASS(0);			// don't care class id
	void SendRankList(CQuestion*pQuestion);
	BOOL IsOver(CQuestion* pQuestion = NULL);
private:
	BOOL CanEnter(CCharacter* pCharacter);
	
	

private:
	UInt16 m_QuestionMapID;
	UInt16 m_QuestionMapX;
	UInt16 m_QuestionMapY;
	UInt16 m_QuestionCount;
	CharInfoVec m_charInfoVec;	
	QuestionVec m_questionVec;
	QuestionDataVec m_questionDataVec;
	UInt8 m_state;
	

	
};
#endif // _QUESTION_CONTROL_H_