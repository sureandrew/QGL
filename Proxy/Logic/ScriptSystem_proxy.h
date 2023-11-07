///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SCRIPTSYSTEM_PROXY_H_
#define _RPC_SCRIPTSYSTEM_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ScriptSystem_enum.h"

class ScriptSystem_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_TalkEnd(LPCPEER pPeer, RPCContext &context, const Boolean &bWait);
	virtual RPCResult CB_TalkNext(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_Choose(LPCPEER pPeer, RPCContext &context, const UInt8 &askerType, const UInt32 &askerId, const UInt8 &total, const Boolean &waitOther, const UInt16 &timeout, const StringVector &msgList);
	virtual RPCResult CB_Say(LPCPEER pPeer, RPCContext &context, const String &msg, const int &imageType, const Boolean &bLeft, const UInt32 &npc_id);
	virtual RPCResult CB_OpenInterface(LPCPEER pPeer, RPCContext &context, const StringA &menu, const Int32 &param1);
	virtual RPCResult CB_OpenShopMenu(LPCPEER pPeer, RPCContext &context, const UInt32 &shopId, const UInt8 &title, const UInt8 &enable, const UInt16 &buyDc, const UInt16 &sellDc, const UInt16 &repairDc, const UInt16 &uiShopType, const UInt32Vector &itemLst);
	virtual RPCResult CB_ShowHint(LPCPEER pPeer, RPCContext &context, const String &hint);
	virtual RPCResult CB_OnPlaySound(LPCPEER pPeer, RPCContext &context, const StringA &Name);
	virtual RPCResult CB_AskGiven(LPCPEER pPeer, RPCContext &context, const String &msg, const UInt32 &npc_id, const UInt8Vector &gtypes, const UInt32Vector &gids);
	virtual RPCResult CB_GivenFinish(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AskString(LPCPEER pPeer, RPCContext &context, const String &text, const String &defAns, const Boolean &bLeft, const UInt8 &askerType, const UInt32 &askerId);
	// OnUpdate can be implemented selectively
***/

protected:
	typedef NetObject Parent;

public:
	virtual RPCResult ReplyAnswer(LPCPEER pPeer, RPCContext &context, const UInt8 &answer);
	virtual RPCResult ReplyAnswerString(LPCPEER pPeer, RPCContext &context, const String &answerStr);
	virtual RPCResult ReplyGiven(LPCPEER pPeer, RPCContext &context, const UInt8Vector &gtypes, const UInt32Vector &uids);
	virtual RPCResult StopTalk(LPCPEER pPeer, RPCContext &context, const Boolean &bForce);
	virtual RPCResult CB_TalkEnd(LPCPEER pPeer, RPCContext &context, const Boolean &bWait) = 0;
	virtual RPCResult CB_TalkNext(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_Choose(LPCPEER pPeer, RPCContext &context, const UInt8 &askerType, const UInt32 &askerId, const UInt8 &total, const Boolean &waitOther, const UInt16 &timeout, const StringVector &msgList) = 0;
	virtual RPCResult CB_Say(LPCPEER pPeer, RPCContext &context, const String &msg, const int &imageType, const Boolean &bLeft, const UInt32 &npc_id) = 0;
	virtual RPCResult CB_OpenInterface(LPCPEER pPeer, RPCContext &context, const StringA &menu, const Int32 &param1) = 0;
	virtual RPCResult CB_OpenShopMenu(LPCPEER pPeer, RPCContext &context, const UInt32 &shopId, const UInt8 &title, const UInt8 &enable, const UInt16 &buyDc, const UInt16 &sellDc, const UInt16 &repairDc, const UInt16 &uiShopType, const UInt32Vector &itemLst) = 0;
	virtual RPCResult CB_ShowHint(LPCPEER pPeer, RPCContext &context, const String &hint) = 0;
	virtual RPCResult CB_OnPlaySound(LPCPEER pPeer, RPCContext &context, const StringA &Name) = 0;
	virtual RPCResult CB_AskGiven(LPCPEER pPeer, RPCContext &context, const String &msg, const UInt32 &npc_id, const UInt8Vector &gtypes, const UInt32Vector &gids) = 0;
	virtual RPCResult CB_GivenFinish(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_AskString(LPCPEER pPeer, RPCContext &context, const String &text, const String &defAns, const Boolean &bLeft, const UInt8 &askerType, const UInt32 &askerId) = 0;
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
	INLINE ScriptSystem_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult ScriptSystem_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<103765836>(pPeer, pBuf);
}

INLINE RPCResult ScriptSystem_Proxy::ReplyAnswer(LPCPEER pPeer, RPCContext &context, const UInt8 &answer)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_ReplyAnswer);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(answer);
	return RPCSend<474753136>(pPeer, pBuf);
}

INLINE RPCResult ScriptSystem_Proxy::ReplyAnswerString(LPCPEER pPeer, RPCContext &context, const String &answerStr)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_ReplyAnswerString);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(answerStr);
	return RPCSend<283642252>(pPeer, pBuf);
}

INLINE RPCResult ScriptSystem_Proxy::ReplyGiven(LPCPEER pPeer, RPCContext &context, const UInt8Vector &gtypes, const UInt32Vector &uids)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_ReplyGiven);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8Vector>(gtypes);
	pBuf->Write<UInt32Vector>(uids);
	return RPCSend<1548887685>(pPeer, pBuf);
}

INLINE RPCResult ScriptSystem_Proxy::StopTalk(LPCPEER pPeer, RPCContext &context, const Boolean &bForce)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_StopTalk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(bForce);
	return RPCSend<1260455611>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(271208504)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(103765836)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(474753136)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(283642252)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1548887685)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1260455611)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1665618048)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(64504023)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(687694366)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(913659639)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(79063645)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(131608988)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(185803571)
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1454594331)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(518091936)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(439170370)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(480398226)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_SCRIPTSYSTEM_PROXY_H_
