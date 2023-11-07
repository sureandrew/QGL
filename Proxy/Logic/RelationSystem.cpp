//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/RelationSystem.h"
#include "QGL/RelationSystemProcess.h"

REGISTER_CLASSTYPE(CRelationSystem, RelationSystem_CLASSID);
RPCResult CRelationSystem::CB_RequestSetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &thirdWord)
{
	CRelationSystemProcess::GetInstance().OpenSetSiblingTitlePrefixPanel(thirdWord);
	return RPC_RESULT_OK;
}

RPCResult CRelationSystem::CB_RequestSetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &prefix)
{
	CRelationSystemProcess::GetInstance().OpenSetSiblingTitleSuffixPanel(prefix);
	return RPC_RESULT_OK;
}

RPCResult CRelationSystem::CB_RequestSelectMarriageTime(LPCPEER pPeer, RPCContext &context)
{
	CRelationSystemProcess::GetInstance().OpenSelectMarriageTimePanel();
	return RPC_RESULT_OK;
}

