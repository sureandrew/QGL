///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTSYSTEM_PROXY_H_
#define _RPC_QUESTSYSTEM_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/QuestSystem_enum.h"

class QuestSystem_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	// OnUpdate can be implemented selectively
***/

protected:
	typedef NetObject Parent;

public:
	virtual RPCResult CancelQuest(LPCPEER pPeer, RPCContext &context, const UInt16 &quest_id);
	virtual RPCResult CB_QuestLogLst(LPCPEER pPeer, RPCContext &context, NetGroup* questLogGrp, NetGroup* questTypeGrp) = 0;
	virtual RPCResult CB_UpdateQuests(LPCPEER pPeer, RPCContext &context, NetGroup* grp) = 0;
	virtual RPCResult CB_AddQuestLogs(LPCPEER pPeer, RPCContext &context, NetGroup* questLogGrp) = 0;
	virtual RPCResult CB_RemoveQuestLogs(LPCPEER pPeer, RPCContext &context, const UInt16Vector &quest_ids) = 0;
	virtual RPCResult CB_AddQuestTypes(LPCPEER pPeer, RPCContext &context, NetGroup* questTypeGrp) = 0;
	virtual RPCResult CB_RemoveQuestTypes(LPCPEER pPeer, RPCContext &context, const UInt16Vector &gtype_ids) = 0;
	virtual RPCResult CB_AddQuestHints(LPCPEER pPeer, RPCContext &context, const RPCNetIDVector &npcIds, const UInt16Vector &questIds) = 0;
	virtual RPCResult CB_RemoveQuestHints(LPCPEER pPeer, RPCContext &context, const RPCNetIDVector &npcIds, const UInt16Vector &questIds) = 0;
	virtual RPCResult CB_AddJournals(LPCPEER pPeer, RPCContext &context, NetGroup* grp) = 0;
	virtual RPCResult CB_RemoveJournals(LPCPEER pPeer, RPCContext &context, const UInt16Vector &journal_ids) = 0;
	virtual RPCResult CB_QuestLogFlagChangeLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags, const UInt16Vector &Flagdel_ids) = 0;
	virtual RPCResult CB_QuestLogFlagLoadLst(LPCPEER pPeer, RPCContext &context, const UInt16Vector &questIDs, const UInt32Vector &questFlags) = 0;
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
} m;

public:
	INLINE QuestSystem_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult QuestSystem_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_QuestSystem_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1576818990>(pPeer, pBuf);
}

INLINE RPCResult QuestSystem_Proxy::CancelQuest(LPCPEER pPeer, RPCContext &context, const UInt16 &quest_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_QuestSystem_CancelQuest);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(quest_id);
	return RPCSend<39481760>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(342585362)
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1576818990)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(39481760)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1979213267)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1727944504)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1682118163)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(485173062)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(425151109)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1429745850)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(729164973)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(722804170)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1919627079)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(86638933)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(2065201949)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1884699685)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_QUESTSYSTEM_PROXY_H_
