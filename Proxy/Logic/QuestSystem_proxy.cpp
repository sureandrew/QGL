//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/QuestSystem_proxy.h"


void QuestSystem_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void QuestSystem_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void QuestSystem_Proxy::OnUpdate()
{
	Parent::OnUpdate();
}

#pragma optimize("", off)
RPCResult QuestSystem_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestSystem_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_QuestSystem_CB_QuestLogLst:
		{
			NetGroup* questLogGrp;
			pBuf->Read<NetGroup*>(questLogGrp);
			NetGroup* questTypeGrp;
			pBuf->Read<NetGroup*>(questTypeGrp);
			_result = CB_QuestLogLst(pPeer, context, questLogGrp, questTypeGrp);
		}
		break;
	case RPC_QuestSystem_CB_UpdateQuests:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_UpdateQuests(pPeer, context, grp);
		}
		break;
	case RPC_QuestSystem_CB_AddQuestLogs:
		{
			NetGroup* questLogGrp;
			pBuf->Read<NetGroup*>(questLogGrp);
			_result = CB_AddQuestLogs(pPeer, context, questLogGrp);
		}
		break;
	case RPC_QuestSystem_CB_RemoveQuestLogs:
		{
			UInt16Vector quest_ids;
			pBuf->Read<UInt16Vector>(quest_ids);
			_result = CB_RemoveQuestLogs(pPeer, context, quest_ids);
		}
		break;
	case RPC_QuestSystem_CB_AddQuestTypes:
		{
			NetGroup* questTypeGrp;
			pBuf->Read<NetGroup*>(questTypeGrp);
			_result = CB_AddQuestTypes(pPeer, context, questTypeGrp);
		}
		break;
	case RPC_QuestSystem_CB_RemoveQuestTypes:
		{
			UInt16Vector gtype_ids;
			pBuf->Read<UInt16Vector>(gtype_ids);
			_result = CB_RemoveQuestTypes(pPeer, context, gtype_ids);
		}
		break;
	case RPC_QuestSystem_CB_AddQuestHints:
		{
			RPCNetIDVector npcIds;
			pBuf->Read<RPCNetIDVector>(npcIds);
			UInt16Vector questIds;
			pBuf->Read<UInt16Vector>(questIds);
			_result = CB_AddQuestHints(pPeer, context, npcIds, questIds);
		}
		break;
	case RPC_QuestSystem_CB_RemoveQuestHints:
		{
			RPCNetIDVector npcIds;
			pBuf->Read<RPCNetIDVector>(npcIds);
			UInt16Vector questIds;
			pBuf->Read<UInt16Vector>(questIds);
			_result = CB_RemoveQuestHints(pPeer, context, npcIds, questIds);
		}
		break;
	case RPC_QuestSystem_CB_AddJournals:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = CB_AddJournals(pPeer, context, grp);
		}
		break;
	case RPC_QuestSystem_CB_RemoveJournals:
		{
			UInt16Vector journal_ids;
			pBuf->Read<UInt16Vector>(journal_ids);
			_result = CB_RemoveJournals(pPeer, context, journal_ids);
		}
		break;
	case RPC_QuestSystem_CB_QuestLogFlagChangeLst:
		{
			UInt16Vector questIDs;
			pBuf->Read<UInt16Vector>(questIDs);
			UInt32Vector questFlags;
			pBuf->Read<UInt32Vector>(questFlags);
			UInt16Vector Flagdel_ids;
			pBuf->Read<UInt16Vector>(Flagdel_ids);
			_result = CB_QuestLogFlagChangeLst(pPeer, context, questIDs, questFlags, Flagdel_ids);
		}
		break;
	case RPC_QuestSystem_CB_QuestLogFlagLoadLst:
		{
			UInt16Vector questIDs;
			pBuf->Read<UInt16Vector>(questIDs);
			UInt32Vector questFlags;
			pBuf->Read<UInt32Vector>(questFlags);
			_result = CB_QuestLogFlagLoadLst(pPeer, context, questIDs, questFlags);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult QuestSystem_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_QuestSystem_UP_STATE:
			_result = RPCDecode<342585362>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_QuestLogLst:
			_result = RPCDecode<1979213267>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_UpdateQuests:
			_result = RPCDecode<1727944504>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_AddQuestLogs:
			_result = RPCDecode<1682118163>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_RemoveQuestLogs:
			_result = RPCDecode<485173062>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_AddQuestTypes:
			_result = RPCDecode<425151109>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_RemoveQuestTypes:
			_result = RPCDecode<1429745850>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_AddQuestHints:
			_result = RPCDecode<729164973>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_RemoveQuestHints:
			_result = RPCDecode<722804170>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_AddJournals:
			_result = RPCDecode<1919627079>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_RemoveJournals:
			_result = RPCDecode<86638933>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_QuestLogFlagChangeLst:
			_result = RPCDecode<2065201949>(pPeer, pBuf);
		break;
	case RPC_QuestSystem_CB_QuestLogFlagLoadLst:
			_result = RPCDecode<1884699685>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
