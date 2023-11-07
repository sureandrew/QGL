//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Question_proxy.h"


void Question_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(pointMask))
	{
		buf.Read<UInt32>(m.point);
	}
	if (m_updateMask.IsBit(ranklistMask))
	{
		buf.Read<String>(m.name1);
		buf.Read<UInt32>(m.point1);
		buf.Read<String>(m.name2);
		buf.Read<UInt32>(m.point2);
		buf.Read<String>(m.name3);
		buf.Read<UInt32>(m.point3);
		buf.Read<String>(m.name4);
		buf.Read<UInt32>(m.point4);
		buf.Read<String>(m.name5);
		buf.Read<UInt32>(m.point5);
		buf.Read<String>(m.name6);
		buf.Read<UInt32>(m.point6);
		buf.Read<String>(m.name7);
		buf.Read<UInt32>(m.point7);
		buf.Read<String>(m.name8);
		buf.Read<UInt32>(m.point8);
		buf.Read<String>(m.name9);
		buf.Read<UInt32>(m.point9);
		buf.Read<String>(m.name10);
		buf.Read<UInt32>(m.point10);
	}
}

void Question_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Question_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(pointMask)) OnUpdatePoint(m.point);
	if (IsUpdated(ranklistMask)) OnUpdateRanklist(m.name1, m.point1, m.name2, m.point2, m.name3, m.point3, m.name4, m.point4, m.name5, m.point5, m.name6, m.point6, m.name7, m.point7, m.name8, m.point8, m.name9, m.point9, m.name10, m.point10);
}

#pragma optimize("", off)
RPCResult Question_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Question_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_Question_CB_NextQuestion:
		{
			UInt32 count;
			pBuf->Read<UInt32>(count);
			UInt32 question_id;
			pBuf->Read<UInt32>(question_id);
			String description;
			pBuf->Read<String>(description);
			String option1;
			pBuf->Read<String>(option1);
			String option2;
			pBuf->Read<String>(option2);
			String option3;
			pBuf->Read<String>(option3);
			String option4;
			pBuf->Read<String>(option4);
			_result = CB_NextQuestion(pPeer, context, count, question_id, description, option1, option2, option3, option4);
		}
		break;
	case RPC_Question_CB_AnswerResult:
		{
			Boolean bRight;
			pBuf->Read<Boolean>(bRight);
			_result = CB_AnswerResult(pPeer, context, bRight);
		}
		break;
	case RPC_Question_CB_StartCountDown:
		{
			UInt32 count;
			pBuf->Read<UInt32>(count);
			_result = CB_StartCountDown(pPeer, context, count);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Question_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Question_UP_STATE:
			_result = RPCDecode<1031225953>(pPeer, pBuf);
		break;
	case RPC_Question_CB_NextQuestion:
			_result = RPCDecode<1688412626>(pPeer, pBuf);
		break;
	case RPC_Question_CB_AnswerResult:
			_result = RPCDecode<529400276>(pPeer, pBuf);
		break;
	case RPC_Question_CB_StartCountDown:
			_result = RPCDecode<922421266>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
