///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTSYSTEM_STUB_H_
#define _RPC_QUESTSYSTEM_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/QuestSystem_enum.h"

class QuestSystem_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CancelQuest(LPCPEER pPeer, RPCContext &context, const UInt16 &quest_id);
***/

protected:
	typedef NetObject Parent;

public:
	virtual RPCResult CancelQuest(LPCPEER pPeer, RPCContext &context, const UInt16 &quest_id) = 0;
	virtual RPCResult CB_QuestLogLst(LPCPEER pPeer, RPCContext &context, NetGroup* questLogGrp, NetGroup* questTypeGrp);
	virtual RPCResult CB_UpdateQuests(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
	virtual RPCResult CB_AddQuestLogs(LPCPEER pPeer, RPCContext &context, NetGroup* questLogGrp);
	virtual RPCResult CB_RemoveQuestLogs(LPCPEER pPeer, RPCContext &context, const UInt16Vector &quest_ids);
	virtual RPCResult CB_AddQuestTypes(LPCPEER pPeer, RPCContext &context, NetGroup* questTypeGrp);
	virtual RPCResult CB_RemoveQuestTypes(LPCPEER pPeer, RPCContext &context, const UInt16Vector &gtype_ids);
	virtual RPCResult CB_AddQuestHints(LPCPEER pPeer, RPCContext &context, const RPCNetIDVector &npcIds, const UInt16Vector &questIds);
	virtual RPCResult CB_RemoveQuestHints(LPCPEER pPeer, RPCContext &context, const RPCNetIDVector &npcIds, const UInt16Vector &questIds);
	virtual RPCResult CB_AddJournals(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
	virtual RPCResult CB_RemoveJournals(LPCPEER pPeer, RPCContext &context, const UInt16Vector &journal_ids);
	virtual RPCResult CB_QuestLogFlagChangeLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags, const UInt16Vector &Flagdel_ids);
	virtual RPCResult CB_QuestLogFlagLoadLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
} m;

public:
	INLINE QuestSystem_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_QUESTSYSTEM_STUB_H_
