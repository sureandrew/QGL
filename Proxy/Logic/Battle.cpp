//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Battle.h"
//-- Library
#include "Proxy/Logic/BatActor.h"
#include "Proxy/Logic/BatMob.h"
#include "Proxy/Logic/BatCharacter.h"
#include "Common/BattleCode.h"
#include "Common/StateCode.h"
#include "Resource/ResMob.h"
#include "QGL/Global.h"
#include "QGL/AntiHack.h"


REGISTER_CLASSTYPE(CBattle, Battle_CLASSID);

CBattle::CBattle()
{
	m_pBattleView = NULL;
	m_curTurn = 0;
}

CBattle::~CBattle()
{
}

RPCResult CBattle::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CB_Error: errCode %d, %s"), errCode, errMsg.c_str());
	if (errCode == BATTLE_SPECIAL_END_TIMEOUT)
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BATTLE_TIME_OUT")));
	if( errMsg.CompareNoCase(_T("MSG_MAP_FORCE_LEAVE"))==0 )
	{
		String text(gGlobal.GetStringTable()->Get(errMsg.c_str()));
		String empty;
		if (!text.Equals(empty.c_str()))
		{
			gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
			gGlobal.PrintPlayerMessage(text.c_str());
		}
	}
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_PlayTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum,
							   const ByteArray &array, const UInt32 &totalSize)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, 
		_T("CB_PlayTurn: battle %d,%d, turn %d, total size %d"), 
		GetNetId().srvId, GetNetId().objId, turnNum, totalSize);

	m_turnBuffer.ResetCursor();
	m_turnBuffer.WriteArray(array.GetData(), (UInt32) array.GetSize());
	if (m_pBattleView && m_turnBuffer.GetCursor() >= 
		totalSize + REUBEN_NETWORK_BUFFER_HEADER_SIZE)			// no more array
		m_pBattleView->NotifyPlayTurn(pPeer, context, m_turnBuffer, totalSize);

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_PlayTurnMore(LPCPEER pPeer, RPCContext &context, const ByteArray &array,
								   const UInt32 &totalSize)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, 
		_T("CB_PlayTurnMore: battle %d,%d, array size %d, total size %d"), 
		GetNetId().srvId, GetNetId().objId, array.GetSize(), totalSize);

	m_turnBuffer.WriteArray(array.GetData(), (UInt32) array.GetSize());
	if (m_pBattleView && m_turnBuffer.GetCursor() >= 
		totalSize + REUBEN_NETWORK_BUFFER_HEADER_SIZE)			// no more array
		m_pBattleView->NotifyPlayTurn(pPeer, context, m_turnBuffer, totalSize);

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_NextTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum,
							   const UInt32 &timeLeft, const UInt16 &autoCount, 
							   const Boolean &bInput, const Boolean &bPartnerInput)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("NextTurn %d: timeout %d, autoCount %d"), 
		turnNum, timeLeft, autoCount);

	m_curTurn = turnNum;
	if (m_pBattleView)
		m_pBattleView->NotifyNextTurn(pPeer, context, turnNum, timeLeft, autoCount, bInput, bPartnerInput);

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_EndBattle(LPCPEER pPeer, RPCContext &context, const UInt8 &endState)
{
	switch (endState)
	{
	case END_WIN:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_EndBattle: You have won in battle"));
		if (m_pBattleView)
			m_pBattleView->NotifyWinBattle(pPeer, context);

		break;

	case END_LOSE:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_EndBattle: You have lost in battle"));
		if (m_pBattleView)
			m_pBattleView->NotifyLoseBattle(pPeer, context);

		break;

	case END_ESCAPED:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_EndBattle: You have escaped from battle"));
		if (m_pBattleView)
			m_pBattleView->NotifyEscapeBattle(pPeer, context);

		break;
	case END_DRAW:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_EndBattle: Draw, no one win or lose"));
		if (m_pBattleView)
			m_pBattleView->NotifyDrawBattle(pPeer, context);

		break;

	case END_MONITOR:
		if (m_pBattleView)
			m_pBattleView->NotifyMonitorEnd(pPeer, context);
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_EndBattle: You monitor the battle which is ended"));
		break;

	default:
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_EndBattle Error: endState %d"), endState);
	}

	m_pBattleView->NotifyEndBattle();
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_AddActors(LPCPEER pPeer, RPCContext &context, NetGroup* actors, const UInt8 &Team)
{
	if (m_pBattleView)
		m_pBattleView->NotifyAddActors(pPeer, context, actors, Team);

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_AddPartners(LPCPEER pPeer, RPCContext &context, NetGroup* actors, const UInt8 &Team)
{
	if (m_pBattleView)
		m_pBattleView->StoreWaitingPartner(actors);

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_ControlCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx)
{
	if (m_pBattleView)
		m_pBattleView->NotifyControlCharacter(pPeer, context, charIdx);
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_ControlPartner(LPCPEER pPeer, RPCContext &context, 
									 const UInt8 &charIdx, const UInt8 &mobIdx)
{
	if (m_pBattleView)
		m_pBattleView->NotifyControlPartner(pPeer, context, charIdx, mobIdx);
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_Reward(LPCPEER pPeer, RPCContext &context, const Boolean &levelUp, 
							 const UInt32 &exp, const UInt32Vector &dropItems, 
							 const UInt32Vector &discardItems, const UInt32 &partnerExp, 
							 const Boolean &partnerLvlUp, const UInt32 &boundsExp, const UInt32 &boundsPartnerExp)
{
	/*
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Battle Won: You got %d exp."), exp);
	if (levelUp)
		TRACE_INFODTL(GLOBAL_LOGGER, _F("And you have level up!!!"));
	if (!dropItems.empty())
	{
		TRACE_INFODT(GLOBAL_LOGGER, _F("And you got items: "));
		for (UInt32Vector::const_iterator it = dropItems.begin(); it != dropItems.end(); it++)
		{
			TRACE_INFO_1(GLOBAL_LOGGER, _F("%d, "), *it);
		}
		TRACE_INFOL(GLOBAL_LOGGER, _F(""));
	}
	*/
	if (m_pBattleView)
		m_pBattleView->NotifyReward(pPeer, context, levelUp, exp, dropItems, discardItems, partnerExp, partnerLvlUp, boundsExp, boundsPartnerExp);
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_Penalty(LPCPEER pPeer, RPCContext &context, const UInt32 &exp, const UInt32 &money, const UInt32 &honor)
{
	/*
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Battle Lose: You have dead, and lose got %d exp, %d money, %d honor"), exp, money, honor);
	*/

	if (m_pBattleView)
		m_pBattleView->NotifyPenalty(pPeer, context, exp, money, honor);
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_NeedHelp(LPCPEER pPeer, RPCContext &context, const Boolean &bNeed)
{
	if (m_pBattleView && bNeed)
		m_pBattleView->NotifyNeedHelp();
	else if (!bNeed)
		gGlobal.NotifyNoMoreHelp();

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_WaitTalk(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum,
							   const UInt32 &turnTime)
{
	if (m_pBattleView) {
		m_pBattleView->NotifyWaitTalk(pPeer, context, turnNum, turnTime);
	}
	
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_CancelTalk(LPCPEER pPeer, RPCContext &context)
{
	if (m_pBattleView) {
		m_pBattleView->BatPushEndCmdMsg();
	}
	
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_Choose(LPCPEER pPeer, RPCContext &context, const UInt8 &askerType,
							 const UInt32 &askerId, const Boolean &bLeft, const UInt8 &total, 
							 const StringVector &msgList)
{
	if (m_pBattleView)
		m_pBattleView->BatChooseHandle(askerType, askerId, total, bLeft, msgList);
	// if the antihack panel has already popped up, clear the mono-panel status of "BDB_DialogueBox"
	// and "BDB_DialogImagePanel" while set the antihack panel as mono panel
	if (gGlobal.g_pAntiHack && gGlobal.g_pAntiHack->m_bEnterBattle)
	{
        gGlobal.pUIManager->ClearMonoPanel();
		UIPanel *hackpanel = gGlobal.pUIManager->FindUIPanel("AntiHackPanel");
		if (hackpanel)
		{
			gGlobal.pUIManager->ClearMonoPanel();
			gGlobal.pUIManager->SetPanel2Top(hackpanel);	
			gGlobal.pUIManager->SetMonoPanel(hackpanel);
			
		}
		gGlobal.g_pAntiHack->m_bBattleDlg = TRUE;
	}

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_Say(LPCPEER pPeer, RPCContext &context, const String &msg, 
						  const int &imageType, const Boolean &bLeft, 
						  const UInt32 &npc_id)
{
	if (m_pBattleView)
		m_pBattleView->BatSayHande(npc_id, msg, imageType, bLeft, BOOLEAN_FALSE);
	// if the antihack panel has already popped up, clear the mono-panel status of "BDB_DialogueBox"
	// and "BDB_DialogImagePanel" while set the antihack panel as mono panel
	if (gGlobal.g_pAntiHack && gGlobal.g_pAntiHack->m_bEnterBattle)
	{
        gGlobal.pUIManager->ClearMonoPanel();
		UIPanel *hackpanel = gGlobal.pUIManager->FindUIPanel("AntiHackPanel");
		if (hackpanel)
		{
			gGlobal.pUIManager->ClearMonoPanel();
			gGlobal.pUIManager->SetPanel2Top(hackpanel);	
			gGlobal.pUIManager->SetMonoPanel(hackpanel);	
		}
		gGlobal.g_pAntiHack->m_bBattleDlg = TRUE;	
	}	
	return RPC_RESULT_OK;
}

/*
RPCResult CBattle::CB_OnBatEndCmdMsg(LPCPEER pPeer, RPCContext &context)
{
	if (m_pBattleView)
		m_pBattleView->BatPushEndCmdMsg();
	return RPC_RESULT_OK;
}
*/

RPCResult CBattle::CB_DisableCmd(LPCPEER pPeer, RPCContext &context, 
								 const UInt32 &blockCmd)
{
	if (m_pBattleView)
		m_pBattleView->BatSetDisEnableCmd(blockCmd, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_HideCmd(LPCPEER pPeer, RPCContext &context, const UInt8 &index,
							  const Boolean &hide)
{
	if (m_pBattleView)
		m_pBattleView->BatHideBatCmdMenu(index, hide, BOOLEAN_TRUE);
	
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_MartialResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &money, const UInt8 &point)
{
	if (m_pBattleView)
		m_pBattleView->NotifyMartialResult(pPeer, context, result, money, point);
	return RPC_RESULT_OK;
}
void CBattle::OnUpdateBattleType(const UInt8 &battleType)
{
	if(battleType == PK_BATTLE || battleType == FORCE_PK_BATTLE)
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_ENTER_PK")));
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_ENTER_PK")),CHANNELTYPE_SYSTEM);
	}
}
