//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Question.h"
#include "QGL/QuestionProcess.h"
#include "QGL/QGLApplication.h"
#include "QGL/CharacterControl.h"

REGISTER_CLASSTYPE(CQuestion, Question_CLASSID);
RPCResult CQuestion::CB_NextQuestion(LPCPEER pPeer, RPCContext &context,  const UInt32 &count,const UInt32 &question_id, const String &description, const String &option1, const String &option2, const String &option3, const String &option4)
{
#ifdef QGL
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CQuestion::CB_NextQuestion %s"), description.c_str());
	CQuestionProcess::GetInstance().UpdateQuestion(count,question_id,description, option1, option2, option3, option4);
#endif
	return RPC_RESULT_OK;
}

RPCResult CQuestion::CB_StartCountDown(LPCPEER pPeer, RPCContext &context, const UInt32 &count)
{
	CQuestionProcess::GetInstance().SetStartCountDown(count);
	return RPC_RESULT_OK;
}
RPCResult CQuestion::CB_AnswerResult(LPCPEER pPeer, RPCContext &context, const Boolean &bRight)
{
	CQuestionProcess::GetInstance().SetAnswerResult(bRight);

	UInt32CharCtrlPtrMap mapCharacter;
	std::swap(mapCharacter, QGLApplication::GetInstance().GetMapCharacterLst() );
	UInt32CharCtrlPtrMap::iterator iter = mapCharacter.begin();
	UInt32CharCtrlPtrMap::iterator iterend = mapCharacter.end(); 
	for(; iter != iterend; ++iter)
	{
		CCharacterControl * pCharacterControl = iter->second;
		if(pCharacterControl)
		{
			pCharacterControl->RemoveIcon();
		}
	}
	return RPC_RESULT_OK;
}
// OnUpdate can be implemented selectively
void CQuestion::OnUpdatePoint(const UInt32 &point)
{
#ifdef QGL
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestion::OnUpdatePoint"));
	CQuestionProcess::GetInstance().UpdatePoint(point);
#endif
}

void CQuestion::OnUpdateRanklist(const String &name1, const UInt32 &point1, const String &name2, const UInt32 &point2, const String &name3, const UInt32 &point3, const String &name4, const UInt32 &point4, const String &name5, const UInt32 &point5, const String &name6, const UInt32 &point6, const String &name7, const UInt32 &point7, const String &name8, const UInt32 &point8, const String &name9, const UInt32 &point9, const String &name10, const UInt32 &point10)
{
#ifdef QGL
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CQuestion::OnUpdateRanklist"));
	CQuestionProcess::GetInstance().UpdateRankList(name1, point1, name2, point2, name3, point3, name4, point4, name5, point5, 
	name6, point6, name7, point7, name8, point8, name9, point9, name10, point10);
#endif
}

