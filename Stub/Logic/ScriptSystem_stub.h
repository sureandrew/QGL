///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SCRIPTSYSTEM_STUB_H_
#define _RPC_SCRIPTSYSTEM_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ScriptSystem_enum.h"

class ScriptSystem_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult ReplyAnswer(LPCPEER pPeer, RPCContext &context, const UInt8 &answer);
	virtual RPCResult ReplyAnswerString(LPCPEER pPeer, RPCContext &context, const String &answerStr);
	virtual RPCResult ReplyGiven(LPCPEER pPeer, RPCContext &context, const UInt8Vector &gtypes, const UInt32Vector &uids);
	virtual RPCResult StopTalk(LPCPEER pPeer, RPCContext &context, const Boolean &bForce);
***/

protected:
	typedef NetObject Parent;

public:
	virtual RPCResult ReplyAnswer(LPCPEER pPeer, RPCContext &context, const UInt8 &answer) = 0;
	virtual RPCResult ReplyAnswerString(LPCPEER pPeer, RPCContext &context, const String &answerStr) = 0;
	virtual RPCResult ReplyGiven(LPCPEER pPeer, RPCContext &context, const UInt8Vector &gtypes, const UInt32Vector &uids) = 0;
	virtual RPCResult StopTalk(LPCPEER pPeer, RPCContext &context, const Boolean &bForce) = 0;
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
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
} m;

public:
	INLINE ScriptSystem_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_SCRIPTSYSTEM_STUB_H_
