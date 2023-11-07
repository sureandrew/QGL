#pragma once
#ifndef _QUESTION_H_
#define _QUESTION_H_

#include "Proxy/Logic/Question_Proxy.h"

class CQuestion : public Question_Proxy
{
public:
	virtual RPCResult CB_NextQuestion(LPCPEER pPeer, RPCContext &context,  const UInt32 &count,const UInt32 &question_id, const String &description, const String &option1, const String &option2, const String &option3, const String &option4);
	virtual RPCResult CB_StartCountDown(LPCPEER pPeer, RPCContext &context, const UInt32 &count);
	virtual RPCResult CB_AnswerResult(LPCPEER pPeer, RPCContext &context, const Boolean &bRight);
	// OnUpdate can be implemented selectively
	virtual void OnUpdatePoint(const UInt32 &point);
	virtual void OnUpdateRanklist(const String &name1, const UInt32 &point1, const String &name2, const UInt32 &point2, const String &name3, const UInt32 &point3, const String &name4, const UInt32 &point4, const String &name5, const UInt32 &point5, const String &name6, const UInt32 &point6, const String &name7, const UInt32 &point7, const String &name8, const UInt32 &point8, const String &name9, const UInt32 &point9, const String &name10, const UInt32 &point10);

	DEFINE_CLASS(Question_CLASSID);

private:
	typedef Question_Proxy Parent;

};

#endif //_QUESTION_H_
