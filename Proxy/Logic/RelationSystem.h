#pragma once
#ifndef _RELATIONSYSTEM_H_
#define _RELATIONSYSTEM_H_

#include "Proxy/Logic/RelationSystem_Proxy.h"
class CRelationSystemProcess;
class CRelationSystem : public RelationSystem_Proxy
{
public:
	virtual RPCResult CB_RequestSetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &thirdWord);
	virtual RPCResult CB_RequestSetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &prefix);
	virtual RPCResult CB_RequestSelectMarriageTime(LPCPEER pPeer, RPCContext &context);
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(RelationSystem_CLASSID);

private:
	typedef RelationSystem_Proxy Parent;

};

#endif //_RELATIONSYSTEM_H_
