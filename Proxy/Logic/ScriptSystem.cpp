//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/ScriptSystem.h"
//-- Library
#ifdef QGL
#include "QGL/ScriptControl.h"
#include "QGL/Global.h"
#endif

REGISTER_CLASSTYPE(CScriptSystem, ScriptSystem_CLASSID);

CScriptSystem::CScriptSystem()
{
	m_pScriptControl = NULL;
}

CScriptSystem::~CScriptSystem()
{
	m_pScriptControl = NULL;
}

RPCResult CScriptSystem::CB_TalkEnd(LPCPEER pPeer, RPCContext &context, 
									const Boolean &bWait)
{
	if (m_pScriptControl) {
		m_pScriptControl->SetWaitEnd(bWait);
		if (bWait) {
			if (!m_pScriptControl->IsOpen())
				m_pScriptControl->TerminateTalkingDialogue(BOOLEAN_FALSE);
		} else {
			m_pScriptControl->OnEndScriptHandle();
			m_pScriptControl->TerminateTalkingDialogue(BOOLEAN_FALSE);
		}
	}

	return RPC_RESULT_OK; 
}

RPCResult CScriptSystem::CB_TalkNext(LPCPEER pPeer, RPCContext &context)
{
	if (m_pScriptControl && m_pScriptControl->IsOpen())
		m_pScriptControl->ChangeNextPage();

	return RPC_RESULT_OK; 
}

RPCResult CScriptSystem::CB_Choose(LPCPEER pPeer, RPCContext &context, 
								   const UInt8 &askerType, const UInt32 &askerId, 
								   const UInt8 &total, const Boolean &waitOther, 
								   const UInt16 &timeout, const StringVector &msgList)
{
	if (m_pScriptControl)
		m_pScriptControl->OnChooseHandle(askerType, askerId, total, waitOther, timeout, msgList);

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_Say(LPCPEER pPeer, RPCContext &context, const String &msg,
								const int &imageType, const Boolean &bLeft, 
								const UInt32 &npc_id)
{
	if (msg.Left(2) == _T("MS"))
	{
		String text(gGlobal.GetStringTable()->Get(msg.c_str()));
		if (!text.IsEmpty())
		{
			m_pScriptControl->OnSayHandle(npc_id, text, imageType, bLeft);
			return RPC_RESULT_OK;
		}
	}

	m_pScriptControl->OnSayHandle(npc_id, msg, imageType, bLeft);
	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_OpenInterface(LPCPEER pPeer, RPCContext &context, 
										  const StringA &menu, const Int32 &param1)
{
	if (m_pScriptControl)
		m_pScriptControl->CB_OnOpenInterfaceHandle(menu, param1);

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_OpenShopMenu(LPCPEER pPeer, RPCContext &context, const UInt32 &shopId, 
						  const UInt8 &title, const UInt8 &enable, const UInt16 &buyDc, 
						  const UInt16 &sellDc, const UInt16 &repairDc, const UInt16 &uiShopType,
						  const UInt32Vector &itemLst)
{
	if (m_pScriptControl)
		m_pScriptControl->OnOpenShopMenuHandle(shopId, title, enable, buyDc, sellDc, 
			repairDc, uiShopType, itemLst);

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_ShowHint(LPCPEER pPeer, RPCContext &context, const String &hint)
{
#ifdef QGL
	if (!hint.IsEmpty() && m_pScriptControl) 
	{
		if (hint.Left(2) == _T("MS")) {
			String text(gGlobal.GetStringTable()->Get(hint.c_str()));
			if (!text.IsEmpty())
				m_pScriptControl->OnPopUpHint(text);
			else
				m_pScriptControl->OnPopUpHint(hint);
		}
		else 
		{

			if (m_pScriptControl)
				m_pScriptControl->OnPopUpHint(hint);
		}
	}
#endif
	return RPC_RESULT_OK;
}


RPCResult CScriptSystem::CB_OnPlaySound(LPCPEER pPeer, RPCContext &context, const StringA &Name)
{
	if ( m_pScriptControl )
		m_pScriptControl->OnPlaySound(Name);
	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_AskGiven(LPCPEER pPeer, RPCContext &context, 
									 const String &msg, const UInt32 &npc_id, 
									 const UInt8Vector &gtypes, 
									 const UInt32Vector &gids)
{
	if (m_pScriptControl)
		m_pScriptControl->OnNpcGivenHandle(npc_id, gids, gtypes, msg);

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_GivenFinish(LPCPEER pPeer, RPCContext &context)
{
	if (m_pScriptControl)
		m_pScriptControl->OnNpcGivenFinish();

	return RPC_RESULT_OK;
}

RPCResult CScriptSystem::CB_AskString(LPCPEER pPeer, RPCContext &context, 
									  const String &text, const String &defAns, 
									  const Boolean &bLeft, const UInt8 &askerType,
									  const UInt32 &askerId)
{
	if (m_pScriptControl)
		m_pScriptControl->OnAskString(text, defAns, bLeft, askerType, askerId);
	return RPC_RESULT_OK;
}
