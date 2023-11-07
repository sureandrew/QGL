#pragma once
#ifndef _SCRIPTSYSTEM_H_
#define _SCRIPTSYSTEM_H_

#include "Proxy/Logic/ScriptSystem_Proxy.h"

class CScriptSystem : public ScriptSystem_Proxy
{
public:
	DEFINE_CLASS(ScriptSystem_CLASSID);
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

	CScriptSystem();
	//virtual ~CScriptSystem();
	bool GetTalkStatus();
	void ClearGiven();
	void ResetGiven();
	Boolean AddGivenObject(UInt8 gtype, UInt32 gid, UInt32 uid);
	Boolean ReplyGiven();
	UInt16 GetGivenCount() { return m_givenCount; }
	Boolean IsAskString() { return m_bAskString; }
	void ClearAskString() { m_bAskString = BOOLEAN_FALSE; }

	enum GivenEnum
	{
		GIVEN_ITEM			= 1,
		GIVEN_PARTNER		= 2,
		GIVEN_PET			= 3,
	};

	UInt32 m_NPCShop;
	UInt16 m_buyDiscount;
	UInt16 m_sellDiscount;
    UInt16 m_repairDiscount;
    UInt32Vector m_NPCShopItem;

private:
	typedef ScriptSystem_Proxy Parent;

	Boolean			m_bAskString;
	Boolean			m_bRequire;
	UInt8Vector		m_givenTypes;
	UInt32Vector	m_givenIds;
	UInt32Vector	m_givenUIds;
	UInt16			m_givenCount;
};

#endif //_SCRIPTSYSTEM_H_
