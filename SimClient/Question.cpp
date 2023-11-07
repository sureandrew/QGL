//-- Common
#include "Common.h"
//-- Self
#include "SimClient/Question.h"
#include "SimClient/Global.h"
#include "SimClient/Character.h"

REGISTER_CLASSTYPE(CQuestion, Question_CLASSID);

CQuestion::CQuestion()
{
	m_curQuestionId = 0;
}
CQuestion::~CQuestion()
{
}

RPCResult CQuestion::CB_NextQuestion(LPCPEER pPeer, RPCContext &context,  const UInt32 &count,const UInt32 &question_id, const String &description, const String &option1, const String &option2, const String &option3, const String &option4)
{
	//UInt32 r = ::GetRand(100) % 100;
	//if(r < 5)
	//{
	//	gGlobal.m_pSimClient->m_pCharacter->ExitQuestion(pPeer,context);
	//	return RPC_RESULT_OK;
	//}
	m_curQuestionId = question_id;
	Anwser(gGlobal.m_pSimClient->m_pProxyPeer, 
		gGlobal.m_pSimClient->m_context,m_curQuestionId,::GetRand(1,4));
	//RaiseUniqEventID(EVT_ANSWER, ::GetRand(0,14) * 1000);
	return RPC_RESULT_OK;
}
RPCResult CQuestion::CB_StartCountDown(LPCPEER pPeer, RPCContext &context, const UInt32 &count)
{
	return RPC_RESULT_OK;
}
// OnUpdate can be implemented selectively
void CQuestion::OnUpdatePoint(const UInt32 &point)
{
}
void CQuestion::OnUpdateRanklist(const String &name1, const UInt32 &point1, const String &name2, const UInt32 &point2, const String &name3, const UInt32 &point3, const String &name4, const UInt32 &point4, const String &name5, const UInt32 &point5, const String &name6, const UInt32 &point6, const String &name7, const UInt32 &point7, const String &name8, const UInt32 &point8, const String &name9, const UInt32 &point9, const String &name10, const UInt32 &point10)
{
}
void CQuestion::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_ANSWER:	
		{
			//UInt32 r = ::GetRand(100) % 100;
			//if(r < 5)
			//{
			//	gGlobal.m_pSimClient->m_pCharacter->ExitQuestion(gGlobal.m_pSimClient->m_pProxyPeer, 
			//		gGlobal.m_pSimClient->m_context);
			//	return;
			//}
			Anwser(gGlobal.m_pSimClient->m_pProxyPeer, 
				gGlobal.m_pSimClient->m_context,m_curQuestionId,1);
		}	
		break;
	default:
		break;
	}
}
