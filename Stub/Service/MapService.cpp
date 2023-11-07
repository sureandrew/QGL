//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/common_binary.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Service/MapService.h"
//-- Library
#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/WorldService_proxy.h"
#include "Proxy/Service/MessageService_proxy.h"
#include "Proxy/Data/SkillData.h"
#include "Proxy/Data/TitleData.h"
#include "Proxy/Data/BuffData.h"
#include "Proxy/Data/ItemData.h"
#include "Proxy/Data/MsgCharacterData.h"
#include "Proxy/Logic/Guild.h"
#include "Proxy/Logic/GuildMember.h"
#include "Proxy/Logic/MessageCharacter.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/MapNPC.h"
#include "Stub/Logic/Map.h"
#include "Stub/Logic/Party.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Item.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/ScriptSystem.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/SkillGroup.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/QuestSystem.h"
#include "Stub/Logic/Battle.h"
#include "Proxy/Logic/Messenger.h"
#include "MapService/Global.h"
#include "MapService/MapApplication.h"
#include "MapService/ViewManager.h"
#include "MapService/Instance.h"
#include "MapService/antigameaddiction.h"
#include "MapService/Configure.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResSkill.h"
#include "Resource/ResItem.h"
#include "Resource/ResMob.h"
#include "Resource/ResManor.h"
#include "Proxy/Data/PetData.h"
#include "Stub/Logic/Pet.h"
#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/HawkGroup.h"
#include "Resource/ResTitle.h"
#include "Proxy/Service/LogService.h"
#include "Stub/Logic/ChargeShop.h"
#include "Stub/Logic/ChargeShopItem.h"
#include "Proxy/Data/ChargeShopData.h"
#include "MapService/QuestionControl.h"
#include "Stub/Logic/Question.h"
#include "Proxy/Data/TitleData.h"
#include "Proxy/Data/PartnerCollectionData.h"
#include "Proxy/Data/SiblingData.h"
#include "Stub/Logic/RelationSystem.h"
#include "Proxy/Data/GenerationData.h"
#include "Stub/Logic/Generation.h"
#include "Stub/Logic/GenerationGroup.h"
#include "Proxy/Data/MarriageData.h"
#include "Stub/Logic/PartyCharInfo.h"

#define INVITE_PARTY_OVERTIME		3600
#define LOGOUT_SESSION_TIME			20000

enum LOGCHEAT_MESSAGE
{
	LOGCHEAT_SUCCEED = 0,
	LOGCHEAT_ERR_CHEATFAIL,
	LOGCHEAT_ERR_CHARNOTFOUNDINMAP,
	LOGCHEAT_ERR_CHARNETIDNOTFOUND,
	LOGCHEAT_ERR_CHARINFONOTFOUND,
	LOGCHEAT_ERR_MAPNOTFOUND,
};

PROTO_LuaPush(CCharacter)
PROTO_LuaPush(CPartner)

// private function and namespace global variable here
namespace MapService_Stub
{
	//RPCResult DoCharacterLeave(RPCContext &context);
	RPCResult LocalCharJump(RPCContext &context);
	RPCResult RemoteCharJump(RPCContext &context);
	//RPCResult LocalPartyJump(RPCContext &context);
	//RPCResult RemotePartyJump(RPCContext &context);
	RPCResult LocalNpcJump(RPCContext &context);
	RPCResult RemoteNpcJump(RPCContext &context);
};

Boolean OpenLogger(Logger& logger)
{
	String strLogFile = GLOBAL_CONFIG.GetString(GLOBAL_NAME, logger.GetName().c_str(), NULL);
	if (strLogFile.GetLength() == 0)
		return (BOOLEAN_FALSE);
	Int size = GLOBAL_CONFIG.GetInt(GLOBAL_NAME, _T("LogSize"), 0);
	if (size == 0)
		GLOBAL_CONFIG.GetInt(_T("General"), _T("LogSize"), 200);
	String strArchive = GLOBAL_CONFIG.GetString(GLOBAL_NAME, _T("LogArchive"), NULL);
	if (strArchive.GetLength() == 0)
		strArchive = GLOBAL_CONFIG.GetString(_T("General"), _T("LogArchive"), NULL);
	return (logger.Open(strLogFile.c_str(),
		BOOLEAN_FALSE, BOOLEAN_FALSE, BOOLEAN_TRUE, Logger::LEVEL_VERBOSE,
		size, strArchive.c_str()));
}

RPCResult MapService_Stub::RegisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("RegisterProxy: serviceId %d"), serviceId);

	MapApplication::GetInstance().RegisterProxy(pPeer, serviceId);

	return (RPC_RESULT_OK);
}

RPCResult MapService_Stub::UnregisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UnregisterProxy: serviceId %d"), serviceId);

	if (!MapApplication::GetInstance().UnregisterProxy(serviceId))
		return (RPC_RESULT_FAIL);

	return (RPC_RESULT_OK);
}

RPCResult MapService_Stub::SyncProxy(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::ReleaseSession(LPCPEER pPeer, RPCContext &context, 
										  const SessionID &sid, 
										  const Boolean &bDelete)
{
	Session* psess = GetSession(context.sessionId);
	CCharacter* pchar = (psess ? psess->pchar : NULL);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ReleaseSession: sessionId %s, char_id %d"), context.sessionId.GetString().c_str(), (pchar ? pchar->GetChar_id() : -1));

	if (psess == NULL || pchar == NULL)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("ReleaseSession: [no session or character] session %d, character %d"), 
			psess, pchar);
		// remove procedure may lead to procedure dead lock
		//ProcedureManager::GetInstance().RemoveBySession(context.sessionId);
		return RPC_RESULT_FAIL;
	}
	
	/// anti-addict: upload the anti-addict data to DB
//	if ((gConfigure.antiAddict == 1) && pchar->m_pAntiGameAddict && pchar->m_pAntiGameAddict->m_startGame)
	if (pchar->m_pAntiGameAddict)
		pchar->m_pAntiGameAddict->UpdateAddictInfoToDB(pchar, gConfigure.antiAddict);	// now, write down the disconnect time no matter what

	// clear client peer
	pchar->ClearPeer();
	
	// stop moving when disconnect
	if (pchar->IsLeader() && pchar->IsState(CCharacter::ST_MOVE))
		pchar->Stop();
	if (pchar->GetScript()->IsRunning())
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("call stop script, sessionId %s, char_id %d"),
			pchar->GetContext().sessionId.GetString().c_str(), pchar->GetChar_id());
		pchar->GetScript()->StopScript();
	}

	// release battle session
	CBattle * pBattle = FindNetObject<CBattle>(pchar->m_BattleNetID);
	if (pBattle)
		pBattle->ClientDisconnect(context.sessionId);

	if (pchar->IsTrading())
		pchar->ClearTrading();

	bool needLogout = true;
	if (pchar->GetMap() && pchar->IsInMap())
	{
		pchar->SetMap(pchar->GetMap()->GetMap_id(), pchar->GetCurPosX(), pchar->GetCurPosY(),
			0, pchar->GetMap_ownerId());
		time_t now;
		time(&now);
		pchar->SetOfflineTime((UInt32)now);
		pchar->RaiseUpdate();

		if (pchar->IsHawking() && pchar->GetHawkerTime() > 0)
		{
			pchar->m_pHawkGroup->StartOfflineCount();
			needLogout = false;
		}
	}

	// stop robot if turned on
	if (pchar->IsRobot())
		pchar->SetRobot(BOOLEAN_FALSE, 0);

	if (needLogout)
		pchar->ScheduleLogout(LOGOUT_SESSION_TIME);

	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::ChangeSessionId(LPCPEER pPeer, RPCContext &context, 
										   const SessionID &oldSessId)
{
	Session* psess = GetSession(oldSessId);
	if (psess == NULL)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ChangeSessionId: [old session no longer available] old session ID %s"), 
			oldSessId.GetString().c_str());
		return RPC_RESULT_FAIL;
	}
	// change session ID
	if (oldSessId != context.sessionId)
	{
		ChangeSession(oldSessId, context.sessionId);
		ProcedureManager::GetInstance().ChangeSessionId(oldSessId, context.sessionId);

		CCharacter* pchar = psess->pchar;
		if (pchar) {
			pchar->SetSessionId(context.sessionId);
			//pchar->ClearPeer();
			CBattle *pBattle = FindNetObject<CBattle>(pchar->m_BattleNetID);
			if (pBattle)
				pBattle->ChangeSessionId(oldSessId, context.sessionId);
		}
		//else if (pchar && pchar->IsState(CCharacter::ST_BATTLE))
		//{
		//	if (!pchar->ChangeState(CCharacter::ST_NOBATTLE))
		//	{
		//		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("ChangeSessionId: Error pchar->ChangeState(CCharacter::ST_NOBATTLE) char_id %d, m_pBattle = %d"), pchar->GetChar_id(), pchar->m_pBattle);
		//	}
		//}
	}
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("MapService_Stub::ChangeSessionId Error (oldSessId == context.sessionId) sid = %s"), context.sessionId.GetString().c_str());
	}

	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::ClientDisconnect(LPCPEER pPeer, RPCContext &context)
{
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("[session not found], sessionId %s"),
			context.sessionId.GetString().c_str());
		return RPC_RESULT_FAIL;
	}

	UInt32 acct_id = psess->pchar ? psess->pchar->GetAcct_id() : -1;
	UInt32 char_id = psess->pchar ? psess->pchar->GetChar_id() : -1;
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("sessionId %s, acct_id %d, char_id %d"),
		context.sessionId.GetString().c_str(), acct_id, char_id);

	if (psess->pchar)
		psess->pchar->ClearPeer();

	// check if lock too long, then timeout fail the locked procedure
	ProcedureManager::GetInstance().CheckLockTimeout(context.sessionId);

	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::ResumeSession(LPCPEER pPeer, RPCContext &context)
{
	PROCEDURE_START_RPC(MapService, ResumeSession, NULL)

		Session* psess = GetSession(context.sessionId);
		CCharacter* pchar = NULL;
		Boolean valid = BOOLEAN_FALSE;
		do
		{
			if (psess == NULL)
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("ResumeSession Warning: sessionId %s not found"), 
					context.sessionId.GetString().c_str());
				break;
			}

			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ResumeSession: session ID %s"), 
				context.sessionId.GetString().c_str());

			pchar = psess->pchar;
			TRACE_ENSURE(pchar);

			pchar->SetSessionId(context.sessionId);
			pchar->SetPeer(context.sessionId);
			pchar->CancelLogout();					// cancel logout here also
			pchar->m_autoCount = 0;					// reset auto battle count
			pchar->m_bAssetPasswordVerified = BOOLEAN_FALSE;		// reset the assetlock

			// invalid session when character already leave world
			if (pchar->GetPeer() == NULL || pchar->IsState(CCharacter::ST_LEAVE_MAP) ||
				pchar->GetMap() == NULL || pchar->m_pBag == NULL || 
				pchar->GetScriptSystem() == NULL)
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("ResumeSession Warning: sessionId %s, null peer, map, bag or script system"), 
					context.sessionId.GetString().c_str());
				break;
			}
	
			// send detail character and other stuff to client
			pchar->SendClient(BOOLEAN_TRUE, BOOLEAN_TRUE);

			if (pchar->GetParty())
			{
				pchar->GetParty()->PreSend(BOOLEAN_FALSE);
				pchar->GetParty()->JoinTeamChannel(pchar);
				CParty* pParty = pchar->GetParty();
				CPartyCharInfo cpci;
				for (UInt16 i=0;i<pParty->GetCount();i++) 
				{
					CCharacter* pCharMem = pParty->GetChild<CCharacter>(i);
					if (NULL != pCharMem && pParty->GetM_bTempLeave(i)) 
					{
						cpci.SetData(pCharMem);
						cpci.SetU16Index(i);
						cpci.SetSendMask(CPartyCharInfo::AllMask);
						pchar->CB_RefreshPartyCharInfo(pchar->GetPeer(),pchar->GetContext(),&cpci);
					}
				}
				pchar->CB_JoinParty(pchar->GetPeer(), pchar->GetContext(), 
					pchar->GetParty());
			}
			//Restore temporarily leave information
			if (NULL != pchar->GetTempParty()) 
			{
				pchar->GetTempParty()->PreSend(BOOLEAN_FALSE);
				pchar->GetTempParty()->JoinTeamChannel(pchar);
				CParty* pTempParty = pchar->GetTempParty();
				CPartyCharInfo cpci;
				for (UInt16 i=0;i<pTempParty->GetCount();i++) 
				{
					CCharacter* pCharMem = pTempParty->GetChild<CCharacter>(i);
					if (NULL != pCharMem) 
					{
						cpci.SetData(pCharMem);
						cpci.SetU16Index(i);
						cpci.SetSendMask(CPartyCharInfo::AllMask);
						pchar->CB_RefreshPartyCharInfo(pchar->GetPeer(),pchar->GetContext(),&cpci);
					}
				}
				//notify the client in the temporarily party
				pchar->CB_RestorePartyAsTemp(pchar->GetPeer(),pchar->GetContext(),pTempParty);
			}
			if(pchar->m_pQuestion)
			{
				CQuestionControl::GetInstance().OnQuestionExit(pchar->m_pQuestion,FALSE);
			}

			if(pchar->GetMap_id()  == CQuestionControl::GetInstance().GetQuestionMapID())
			{
				pchar->JumpReborn();
			}

			//pchar->JoinMapChannel();
			pchar->m_viewManager->ResendView(); // resend character view
			valid = BOOLEAN_TRUE;
		}
		while (false);

		CB_ResumeSession(pPeer, context, valid);
		if (!valid || pchar == NULL)
		{
			// do not treat as error here, this will affect World SelectCharacter procedure
			PROCEDURE_RETURN
			//PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CBattle * pBattle = FindNetObject<CBattle>(pchar->m_BattleNetID);
		if (pchar->IsStartBattle())
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("starting battle, char_id %d(%s)"), pchar->GetChar_id(), context.sessionId.GetString().c_str());
		}
		else if (pchar->IsState(CCharacter::ST_BATTLE) && pBattle)
		{
			if (!pBattle->ResumeClient(context.sessionId))
			{
				pchar->ChangeState(CCharacter::ST_NOBATTLE);
				pchar->m_autoCount = 0;
				pchar->m_BattleNetID = RPCNetID();
				pchar->m_pBatChar = NULL;
				gGlobal.m_charToSession.InsertCharToSession(pchar->GetChar_id(), psess);
			}
		}
		else
		{
			// battle state lock, set back no battle
			if (pchar->IsState(CCharacter::ST_BATTLE) || pchar->IsState(CCharacter::ST_MONITOR))
			{
				TRACE_WARNDTL_3(GLOBAL_LOGGER,
					_FW("battle state locked, session_id %s, battle_state %d, battle %d"),
					context.sessionId.GetString().c_str(), pchar->GetBattleStateID(), pchar->m_BattleNetID);
				pchar->ChangeState(CCharacter::ST_NOBATTLE);
				pchar->m_autoCount = 0;
			}
		}

	PROCEDURE_END

	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::LeaveWorld(RPCContext &context)
{
	PROCEDURE_START_FUNC(MapService_Stub::LeaveWorld, context)

		Session* psess = GetSession(context.sessionId);
		if (psess == NULL || psess->pchar == NULL)
		{
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("invalid session, session_id %s(%0xp), char %d"),
				context.sessionId.GetString().c_str(), psess, (psess) ? psess->pchar : 0);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		CCharacter* pchar = psess->pchar;

		if (pchar->GetBattleStateID() == CCharacter::ST_BATTLE || pchar->IsStartBattle())
		{
			TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("in battle, char_id %d(%s), battle state %d, starting battle %d"),
				pchar->GetChar_id(), pchar->GetSessionId().GetString().c_str(), pchar->GetBattleStateID(), pchar->IsStartBattle());
			pchar->SetLogout(BOOLEAN_TRUE);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		// if cannot leave party when leave world, try leave world later (5s)
		if (pchar->HasParty() && !pchar->GetParty()->Leave(pchar, BOOLEAN_FALSE, BOOLEAN_FALSE))
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("cannot leave party, char_id %d(%s)"),
				pchar->GetChar_id(), pchar->GetSessionId().GetString().c_str());
			pchar->ScheduleLogout(5000);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		if (pchar->HasTempParty() && !pchar->GetTempParty()->Leave(pchar, BOOLEAN_FALSE, BOOLEAN_FALSE))
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("cannot leave  temporary party, char_id %d(%s)"),
				pchar->GetChar_id(), pchar->GetSessionId().GetString().c_str());
			pchar->ScheduleLogout(5000);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("char_id %d(%s)"),
			pchar->GetChar_id(), context.sessionId.GetString().c_str());

		PROCEDURE_STORE_VALUE2(UInt32, pchar->GetAcct_id(), UInt32, pchar->GetChar_id())

		PROCEDURE_DO_METHOD2(CCharacter, DoCharacterLeave, pchar, context, 
			Boolean, BOOLEAN_TRUE, Boolean, BOOLEAN_FALSE)

	PROCEDURE_WAIT_RETURN(1)

		PROCEDURE_RESTORE_VALUE2(UInt32, acct_id, UInt32, char_id)

		if (gGlobal.pWorldService->pPeer)
			WorldService_Proxy::CharacterLeave(gGlobal.pWorldService->pPeer, context, acct_id, char_id);

		RemoveSession(context.sessionId);			// remove session

	PROCEDURE_END

	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::RankTitleUpdate(LPCPEER pPeer, RPCContext &context, 
										   const UInt32Vector &char_ids, 
										   const UInt16Vector &title_ids)
{
	//for the list
	CharIdTitleListMap list;
	
	for (RankTitleMap::iterator Itr = gGlobal.g_RankTitleLst.begin(); 
		Itr != gGlobal.g_RankTitleLst.end(); ++Itr)
	{
		//find out wether the same people get the same title 
		bool contin = false;
		for (UInt i = 0; i < char_ids.size(); i++)
		{
			if (i < title_ids.size())
			{
				if (char_ids[i] == Itr->first && title_ids[i] == Itr->second)
				{
					contin = true;
					break;
				}
			}
		}

		if (contin)
			continue;

		CharIdTitleListMap::iterator ItrList = list.find(Itr->first);
		if (ItrList == list.end())
		{
			UInt16Vector titleList;
			titleList.push_back(Itr->second);
			list.insert(std::make_pair(Itr->first, titleList));
		}
		else
		{
			ItrList->second.push_back(Itr->second);
		}
		//CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(Itr->first);
		//if (pChar)
		//	pChar->RemoveTitle(Itr->second);
	}

	for (CharIdTitleListMap::iterator ItrList = list.begin();
		ItrList != list.end(); ++ItrList)
	{
		CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(ItrList->first);
		if (pChar)
		{
			UInt16Vector &titleList = ItrList->second;
			for (UInt16Vector::iterator it = titleList.begin(); it != titleList.end(); ++it)
				pChar->RemoveTitle(*it);
		}
	}

	list.clear();
	gGlobal.g_RankTitleLst.clear();

	for (UInt i = 0; i < char_ids.size(); i++)
	{
		if (i < title_ids.size())
		{
			//CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(char_ids[i]);
			//if (pChar)
			//{
				//pChar->AddTitle(title_ids[i]);
			//}
			CharIdTitleListMap::iterator ItrList = list.find(char_ids[i]);
			if (ItrList == list.end())
			{
				UInt16Vector titleList;
				titleList.push_back(title_ids[i]);
				list.insert(std::make_pair(char_ids[i], title_ids[i]));
			}
			else
			{
				ItrList->second.push_back(title_ids[i]);
			}
			gGlobal.g_RankTitleLst.insert(std::make_pair(char_ids[i], title_ids[i]));
		}
	}

	for (CharIdTitleListMap::iterator ItrList = list.begin();
		ItrList != list.end(); ++ItrList)
	{
		CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(ItrList->first);
		if (pChar)
		{
			UInt16Vector &titleList = ItrList->second;
			for (UInt16Vector::iterator it = titleList.begin(); it != titleList.end(); ++it)
				pChar->AddTitle(*it);
		}
	}

	return RPC_RESULT_OK;
}


#pragma optimize("", off)		// turn off "if-else" optimization here
RPCResult MapService_Stub::CharacterEnter(LPCPEER pPeer, RPCContext &context, 
										  const EnterInfo &info,
										  const UInt32Vector &memberIdList,
										  const AntiAddictInfo &addictInfo,
										  const String &buffCache,
										  const UInt32 &uiLineupId,
										  const int &iKeyPos)
{
	Session* psess = GetSession(context.sessionId);
	CCharacter *pchar = psess ? psess->pchar : NULL;
	EnterInfo chgInfo = info;
	PROCEDURE_START_RPC6(MapService, CharacterEnter, NULL, EnterInfo, info,
						UInt32Vector, memberIdList, AntiAddictInfo, addictInfo,
						String, buffCache,UINT32,uiLineupId,int,iKeyPos)

		TRACE_INFODTL_7(GLOBAL_LOGGER, 
			_F("session %s, acct_id %d, char_id %d, map_id %d, line_id %d, x %d, y %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, info.map_id, 
			info.line_id, info.x, info.y);
		UInt32 curTime = ::GetTickCount();
		PROCEDURE_STORE_VALUE2(UInt32, curTime, UInt32, curTime)

		if (psess == NULL)
		{
			// add new session
			AddSession(context.sessionId, Session());
		}
		else
		{
			TRACE_WARNDTL_3(GLOBAL_LOGGER, 
				_T("CharacterEnter Warning: [session exist], sessionId %s, acct_id %d, char_id %d"), 
				context.sessionId.GetString().c_str(), info.acct_id, info.char_id);
		}

		// request entire character data from object service
		ObjectService_Proxy::GetCharacters(gGlobal.pObjectService->pPeer, context, 
										0, info.char_id, CCharacterData::DETAIL_MAP);

	PROCEDURE_WAIT2(1, ObjectService, CB_GetCharacterGroup, NetGroup*, charGroup, BYTE, detail)

		TRACE_ENSURE(charGroup);
		
		//// check if character is located inside a unqualified manor instance with respect to the scale of Manor he belongs to
		CCharacterData *pCharData = charGroup->GetChild<CCharacterData>(0);
		TRACE_ENSURE(pCharData);	
		if (IsInManorInstance(info))
		{
			Boolean bReborn = BOOLEAN_TRUE;
            GuildMemberMap::iterator Itr = gGlobal.g_AllGuildMembers.find(info.char_id);
			if (Itr != gGlobal.g_AllGuildMembers.end())
			{
				CGuildMember* pGuildMember = Itr->second;
				if (pGuildMember){
					GuildMap::iterator ItrGuild = gGlobal.g_AllGuilds.find(pGuildMember->GetGuild_uid());
					if (ItrGuild != gGlobal.g_AllGuilds.end()){
						CGuild* pGuild = ItrGuild->second;
						if (pGuild){
							const ManorData *manorData = gGlobal.m_resource->GetManorData(pGuild->GetManorScale());
							if (manorData)
							{
								for (UInt j = 0; j < manorData->manor_map_id.size(); ++j)
								{
									if (manorData->manor_map_id[j] == info.map_id)
                                        bReborn = BOOLEAN_FALSE;		
								}
							}
						}
					}
				}
			}
			if (bReborn)
			{
				chgInfo.map_id = pCharData->GetReborn_map();
				chgInfo.x = pCharData->GetReborn_x();
				chgInfo.y = pCharData->GetReborn_y();
				chgInfo.ownerId = 0;
				chgInfo.instId = 0;
				chgInfo.line_id = 1;	
			}
		}
		////////////////////
		
		CMap* pmap = gGlobal.GetMap(chgInfo.map_id, chgInfo.line_id, chgInfo.ownerId);
		if (pmap == NULL)
		{
			CB_Error(pPeer, context, 1, _T("CharacterEnter Error: The map does not exist"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (1): CB_GetCharacterGroup, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);
		if (charGroup->GetCount() == 0) {
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, 
				_T("CharacterEnter Error: character not found, char_id %d"), info.char_id);
			CB_Error(pPeer, context, 1, _T("CharacterEnter Error: character not found"));
			DeleteNetGroup(charGroup, BOOLEAN_TRUE);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		

		// check if character exist
		if (psess->pchar == NULL)
		{
			psess->pchar = CCharacter::NewInstance();
			psess->pchar->m_viewManager->Init();
		}
		pchar = psess->pchar;
		
		// make sure can change to enter state
		if (!pchar->TryState(CCharacter::ST_ENTER_MAP))
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, 
				_T("CharacterEnter Error: character %d cannot change to ENTER state"),
				pchar->GetChar_id());
			CB_Error(pPeer, context, 1, _T("CharacterEnter Error: character cannot change to ENTER state"));
			DeleteNetGroup(charGroup, BOOLEAN_TRUE);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		// character enter process
        pchar->m_infoNetId = info.charNetId;
		pchar->SetSessionId(context.sessionId);
		pchar->SetData(pCharData);
		pchar->ExtractBuffString();
		pchar->SetBuffCache(buffCache);
		pchar->ExtractBuffCacheString();
		pchar->SetPeer(context.sessionId);
		pchar->CheckBuffUpdate();
		pchar->UpdateBuffBlockChannel();
		pchar->CalcAddiction(addictInfo);
		pchar->ClearAllMask();

		DeleteNetGroup(charGroup, BOOLEAN_TRUE);
		gGlobal.m_charToSession.InsertCharToSession(pchar->GetChar_id(), psess);
		
		// return success to world service after step 1
		CB_CharacterEnter(pPeer, context);

		if (!pchar->PreCharacterEnter(pmap, chgInfo, memberIdList, BOOLEAN_FALSE,uiLineupId,iKeyPos))
		{
			CB_Error(pPeer, context, 1, _T("CharacterEnter Error: PreCharacterEnter failed"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		// load character inventory bag
		PROCEDURE_DO_METHOD(CItemBag, LoadItems, pchar->m_pBag, context);

	PROCEDURE_WAIT_RETURN(2)

		if (psess == NULL || psess->pchar == NULL)
		{
			CB_Error(pPeer, context, 1, _T("CharacterEnter Error: session not found"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (2): LoadItems, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		pchar->m_pBag->SendAll();

		TRACE_INFODTL(GLOBAL_LOGGER, _F("SendAll finished"));

		PROCEDURE_DO_METHOD(CPartnerGroup, LoadPartners, pchar->m_pPartnerGroup, context);

	PROCEDURE_WAIT_RETURN(3)
		
		if (psess == NULL || psess->pchar == NULL)
		{
			CB_Error(pPeer, context, 1, _T("CharacterEnter Error: session not found"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (3): LoadPartners, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		PROCEDURE_DO_METHOD(CPetGroup, LoadPets, pchar->m_pPetGroup, context);

	PROCEDURE_WAIT_RETURN(4)
		
		if (psess == NULL || psess->pchar == NULL)
		{
			CB_Error(pPeer, context, 1, _T("CharacterEnter Error: session not found"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (4): LoadPets, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		UInt32Vector owner_ids;
		UInt8Vector owner_types;
		UInt32Vector partner_ids;
		UInt32Vector pet_ids;

		// get character, partners and pets skills
		owner_ids.push_back(pchar->GetChar_id());
		owner_types.push_back(SKILL_OWNER_TYPE_CHAR);
		pchar->m_pPartnerGroup->GetPartnerIdList(partner_ids);
		for (UInt32Vector::iterator it = partner_ids.begin(); it != partner_ids.end(); ++it)
		{
			owner_ids.push_back(*it);
			owner_types.push_back(SKILL_OWNER_TYPE_PARTNER);
		}
		pchar->m_pPetGroup->GetPetIdList(pet_ids);
		for (UInt32Vector::iterator it = pet_ids.begin(); it != pet_ids.end(); ++it)
		{
			owner_ids.push_back(*it);
			owner_types.push_back(SKILL_OWNER_TYPE_PET);
		}

		ObjectService_Proxy::GetSkillList(gGlobal.pObjectService->pPeer, context,
			owner_ids, owner_types);

	PROCEDURE_WAIT1(5, ObjectService, CB_GetSkillList, NetGroup*, skillDataGroups)
		
		if (psess == NULL || psess->pchar == NULL)
		{
			CB_Error(pPeer, context, 1, _T("CharacterEnter Error: session not found"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		TRACE_ENSURE(pchar && pchar->m_pSkillGroup && skillDataGroups);
		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (5): CB_GetSkillList, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		for (Int i = 0; i < skillDataGroups->GetCount(); i++)
		{
			CSkillData * pSkillData = skillDataGroups->GetChild<CSkillData >(i);
			if (pSkillData && pSkillData->GetOwner_type() == SKILL_OWNER_TYPE_CHAR &&
				gGlobal.m_resource->GetSkillData(pSkillData->GetSkill_id()))
			{
				CSkill * pSkill = CSkill::NewInstance();
				pSkill->SetData(pSkillData);
				pSkill->PreSend();
				pchar->m_pSkillGroup->Add(pSkill);
				pchar->m_skillMap.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));
			}
		}

		//add back base formation skill
		const UInt16 baseFmtSkill = 13;
		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(baseFmtSkill);
		if (pSkillData)
		{
			if (!pchar->HasSkill(baseFmtSkill))
			{
				CSkill * pSkill = CSkill::NewInstance();
				pSkill->SetInfo(baseFmtSkill);
				pSkill->m_data = pSkillData;				
				pSkill->SetOwner_info(pchar->GetChar_id(), SKILL_OWNER_TYPE_CHAR);			
				pSkill->SetLevel(1);			
				pSkill->SetCoolDown(0);
				pSkill->ClearAllMask();
				pSkill->PreSend();
				pchar->m_pSkillGroup->Add(pSkill);
				pchar->m_skillMap.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));
			}
		}
		
		pchar->AddChildSkill(NULL);
		if (skillDataGroups->GetCount() > 0)
		{
			pchar->m_pPartnerGroup->SetAllPartnerSkills(skillDataGroups, NULL);
			pchar->m_pPetGroup->SetAllPetSkills(skillDataGroups, NULL);
		}

		DeleteNetGroup(skillDataGroups, BOOLEAN_TRUE);

		const UInt16 winebowlSkillBase(380);
		if (!pchar->HasSkill(winebowlSkillBase))
		{
			pSkillData = gGlobal.m_resource->GetSkillData(winebowlSkillBase);
			if (pSkillData)
			{
				CSkill * pSkill = CSkill::NewInstance();
				pSkill->SetInfo(winebowlSkillBase);
				pSkill->m_data = pSkillData;				
				pSkill->SetOwner_info(pchar->GetChar_id(), SKILL_OWNER_TYPE_CHAR);			
				pSkill->SetLevel(1);			
				pSkill->SetCoolDown(0);
				pSkill->ClearAllMask();
				pSkill->PreSend();
				pchar->m_pSkillGroup->Add(pSkill);
				pchar->m_skillMap.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));
			}
			pSkillData = gGlobal.m_resource->GetSkillData(winebowlSkillBase + 1);
			if (pSkillData)
			{
				CSkill * pSkill = CSkill::NewInstance();
				pSkill->SetInfo(winebowlSkillBase + 1);
				pSkill->m_data = pSkillData;				
				pSkill->SetOwner_info(pchar->GetChar_id(), SKILL_OWNER_TYPE_CHAR);			
				pSkill->SetLevel(1);			
				pSkill->SetCoolDown(0);
				pSkill->ClearAllMask();
				pSkill->PreSend();
				pchar->m_pSkillGroup->Add(pSkill);
				pchar->m_skillMap.insert(std::make_pair(pSkill->GetSkill_id(), pSkill));
			}
		}

		pchar->m_pBag->ActivateEquipedSkill();
		pchar->m_pPartnerGroup->SendAll();
		pchar->m_pPetGroup->SendAll();
		pchar->m_pSkillGroup->SendAll();

		// load quest logs, types and flag from quest system
		PROCEDURE_DO_METHOD(CQuestSystem, LoadQuests, pchar->m_pQuestSys, context);

	PROCEDURE_WAIT_RETURN(6)

		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (6): CQuestSystem LoadQuests, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		pchar->m_pQuestSys->SendAll();

		ObjectService_Proxy::GetYaunBaoByAcctID(gGlobal.pObjectService->pPeer, context, pchar->GetAcct_id());

	PROCEDURE_WAIT4(7, ObjectService, CB_GetYuanBaoByAcctID, UInt32, yuanbao, UInt32, yuanbaoTicket, Boolean, yuanbaoLock, UInt32, locktime)

		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_5(GLOBAL_LOGGER, 
			_T("CharacterEnter (7): CB_GetYuanBaoByAcctID, session %s, acct_id %d, char_id %d yuanbao %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, yuanbao, 
			curTime - lastTime);
		pchar->SetYuanBao(yuanbao, yuanbaoTicket);
		pchar->SetYuanbaoLock(yuanbaoLock);
		pchar->SetUnlock_time(locktime);

	ObjectService_Proxy::GetTitleList(gGlobal.pObjectService->pPeer, context, pchar->GetChar_id());

	PROCEDURE_WAIT1(8, ObjectService, CB_TitleList, NetGroup*, titleDataGroups)

		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (8): CB_TitleList, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		for (Int i = 0; i < titleDataGroups->GetCount(); i++)
		{
			CTitleData * pTitleData = titleDataGroups->GetChild<CTitleData>(i);
			if (pTitleData)
			{
				String text;
				const TitleData * titleDataCSV = gGlobal.m_resource->GetTitleData(pTitleData->GetTitle_id());
				if (titleDataCSV)
				{
					text = titleDataCSV->Title_Name;
				}
				pchar->AddTitle(pTitleData->GetTitle_id(), pTitleData->GetTitle_uid(),
					pTitleData->GetExpireDate(), text);
			}
		}
		DeleteNetGroup(titleDataGroups, BOOLEAN_TRUE);
		
		//add rank title
		for (RankTitleMap::iterator ItrRank = gGlobal.g_RankTitleLst.find(info.char_id);
			ItrRank != gGlobal.g_RankTitleLst.end() && ItrRank->first == info.char_id; ++ItrRank)
		{
			pchar->AddTitle(ItrRank->second);
		}
		
		//add guild stuffs
		GuildMemberMap::iterator it = gGlobal.g_AllGuildMembers.find(info.char_id);
		if (it != gGlobal.g_AllGuildMembers.end())
		{
			pchar->m_ownGuildMember = it->second;
			GuildMap::iterator itGuild = gGlobal.g_AllGuilds.find(it->second->GetGuild_uid());
			if (itGuild != gGlobal.g_AllGuilds.end())
			{
				pchar->m_ownGuild = itGuild->second;

				const TitleData * pTitleData = gGlobal.m_resource->GetTitleDataByType(TITLE_TYPE_GUILD, 0, it->second->GetPost(), it->second->GetPost());
				if (pTitleData)
				{
					pchar->AddTitle(pTitleData->Title_id);
				}
			}
		}

		//add student title
		MessageService_Proxy::GetMessengersName(gGlobal.pMessageService->pPeer,context, pchar->GetChar_id(), FRIEND_RELATION_MASTER);

	PROCEDURE_WAIT2(9, MessageService, CB_GetMessengersName, UInt32, owner_id, 
						StringVector, messagers)

		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (9): CB_GetMessengersName, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		if (messagers.size() > 0)
		{
			const TitleData * titleDataCSV = gGlobal.m_resource->GetTitleData(gConfigure.studentTitle);
			if (titleDataCSV && messagers.size() == 1)
			{
				String text;
				text.Format(_T("%s%s"), messagers[0].c_str(), titleDataCSV->Title_Name);
				pchar->AddTitle(gConfigure.studentTitle, 0, 0, text);
			}
		}

		//add husband title
		MessageService_Proxy::GetMessengersName(gGlobal.pMessageService->pPeer,context, pchar->GetChar_id(), FRIEND_RELATION_HUSBAND);

	PROCEDURE_WAIT2(10, MessageService, CB_GetMessengersName, UInt32, owner_id, 
			StringVector, messagers)

		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
			PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
			TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (10): CB_GetMessengersName, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		if (messagers.size() > 0)
		{
			const TitleData * titleDataCSV = gGlobal.m_resource->GetTitleData(gConfigure.husbandTitle);
			if (titleDataCSV && messagers.size() == 1)
			{
				String text;
				text.Format(_T("%s%s"), messagers[0].c_str(), titleDataCSV->Title_Name);
				pchar->AddTitle(gConfigure.husbandTitle, 0, 0, text);
			}
		}

		//add wife title
		MessageService_Proxy::GetMessengersName(gGlobal.pMessageService->pPeer,context, pchar->GetChar_id(), FRIEND_RELATION_WIFE);

	PROCEDURE_WAIT2(11, MessageService, CB_GetMessengersName, UInt32, owner_id, 
			StringVector, messagers)

			UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
			PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
			TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (11): CB_GetMessengersName, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		if (messagers.size() > 0)
		{
			const TitleData * titleDataCSV = gGlobal.m_resource->GetTitleData(gConfigure.wifeTitle);
			if (titleDataCSV && messagers.size() == 1)
			{
				String text;
				text.Format(_T("%s%s"), messagers[0].c_str(), titleDataCSV->Title_Name);
				pchar->AddTitle(gConfigure.wifeTitle, 0, 0, text);
			}
		}

		//marriage information
		if(pchar->GetSex() == 0)
			ObjectService_Proxy::GetMarriage(gGlobal.pObjectService->pPeer, context, pchar->GetChar_id(),0);
		else
			ObjectService_Proxy::GetMarriage(gGlobal.pObjectService->pPeer, context, 0,pchar->GetChar_id());

		PROCEDURE_WAIT1(12, ObjectService, CB_GetMarriage, NetGroup*, grp)

			UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
			PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
			TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (12): CB_GetMarriage, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		CMarriageData * pMarriageData = grp->GetChild<CMarriageData>(0);
		CRelationSystem* pRelationSystem = pchar->GetRelationSystem();
		if(pMarriageData != NULL)
		{
			pRelationSystem->SetMarriageData(pMarriageData);
			SetOwnObject(pchar->GetContext().sessionId,pMarriageData);
		}
		else
		{
			TRACE_INFODTL_4(GLOBAL_LOGGER, 
				_T("CharacterEnter (12): CB_GetMarriage, session %s, acct_id %d, char_id %d, utime %d pMarriageData == NULL"), 
				context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);
		}
		DeleteNetGroup(grp);

		//sibling information
		ObjectService_Proxy::GetSibling(gGlobal.pObjectService->pPeer, context, pchar->GetSibling_id());

	PROCEDURE_WAIT1(13, ObjectService, CB_GetSibling, NetGroup*, grp)

		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (13): CB_GetSibling, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		CSiblingData * pSiblingData = grp->GetChild<CSiblingData>(0);
		CRelationSystem* pRelationSystem = pchar->GetRelationSystem();
		if(pSiblingData != NULL)
		{
			pRelationSystem->SetSiblingData(pSiblingData);
			SetOwnObject(pchar->GetContext().sessionId,pSiblingData);
		}
		DeleteNetGroup(grp);

		PROCEDURE_DO_METHOD(CRelationSystem, CheckSiblingDataValidate, pRelationSystem, 
			context)
	PROCEDURE_WAIT_RETURN(14)

		CRelationSystem* pRelationSystem = pchar->GetRelationSystem();
		PROCEDURE_DO_METHOD(CRelationSystem, CheckMasterAndStudentDataValidate, pRelationSystem, 
			context)
	PROCEDURE_WAIT_RETURN(15)

		CRelationSystem* pRelationSystem = pchar->GetRelationSystem();
		PROCEDURE_DO_METHOD(CRelationSystem, CheckHusbandAndWifeDataValidate, pRelationSystem, 
			context)
	PROCEDURE_WAIT_RETURN(16)
	
		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (16): CheckSiblingDataValidate, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		pchar->GetRelationSystem()->UpdateSiblingTitle();
		pchar->CalTitleExpire();
		pchar->CheckTitle();
		pchar->SendTitle();

		ObjectService_Proxy::GetPartnerCollection(gGlobal.pObjectService->pPeer, context, pchar->GetChar_id());//before GetPartnersByCharID

	PROCEDURE_WAIT1(17, ObjectService, CB_GetPartnerCollection, NetGroup*, collectionGroup)

		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter (17): CB_GetPartnerCollection, session %s, acct_id %d, char_id %d, utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

		UInt32Vector collectionIds;
		for (Int i = 0; i < collectionGroup->GetCount(); i++)
		{
			CPartnerCollectionData * pPartnerCollection = 
				collectionGroup->GetChild<CPartnerCollectionData >(i);
			if (pPartnerCollection)
			{
				CCharacter::partnerCollectionData * pCollectionData = SafeCreateObject(CCharacter::partnerCollectionData);
				pCollectionData->collection_uid = pPartnerCollection->GetCollection_uid();
				pCollectionData->collection_id = pPartnerCollection->GetCollection_id();
				pCollectionData->char_id = pPartnerCollection->GetChar_id();
				collectionIds.push_back(pCollectionData->collection_id);
				pchar->m_PartnerCollectionLst.insert(std::make_pair(pCollectionData->collection_id, pCollectionData));
				//TRACE_INFODTL_3(GLOBAL_LOGGER,_F("PartnerCollection::%d,%d,%d"),
				//	pCollectionData->collection_uid, pCollectionData->collection_id,
				//	pCollectionData->char_id);
			}
		}
		pchar->ActivatePartnerCollectionBuff(info.partnerCollection);
		if (!collectionIds.empty())
		{
			pchar->CB_GetPartnerCollectionList(pchar->GetPeer(), pchar->GetContext(),
				collectionIds);
		}
		DeleteNetGroup(collectionGroup, BOOLEAN_TRUE);
	///////////////////////////////////////
		BOOL bYoung=FALSE;
		if (pchar->GetLevel() < 50) 
			bYoung = TRUE;
		ObjectService_Proxy::GetGeneration(gGlobal.pObjectService->pPeer, context,
			pchar->GetChar_id(), bYoung);

	PROCEDURE_WAIT1(18, ObjectService, CB_GetGeneration, NetGroup*,gtnDataGroups)

			if (psess == NULL || psess->pchar == NULL)
			{
				CB_Error(pPeer, context, 1, _T("CharacterEnter Error: session not found"));
				PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
			}
			TRACE_ENSURE(pchar && gtnDataGroups);
			UInt32 curTime = ::GetTickCount();
			PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
				PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
				TRACE_INFODTL_4(GLOBAL_LOGGER, 
				_T("CharacterEnter (18): CB_GetGeneration, session %s, acct_id %d, char_id %d, utime %d"), 
				context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - lastTime);

			for (Int i = 0; i < gtnDataGroups->GetCount(); i++)
			{
				CGenerationData* pGntData = gtnDataGroups->GetChild<CGenerationData>(i);
				if (pGntData)
				{
					CGeneration* pNewGeneration = CreateNetObject<CGeneration>();
					if (NULL != pNewGeneration) {
						pNewGeneration->SetBYoung(pGntData->GetBYoung());
						pNewGeneration->SetStrNickName(pGntData->GetStrNickName());
						pNewGeneration->SetUiAccmulOrRecvExp(pGntData->GetUiAccmulOrRecvExp());
						if (pGntData->GetBYoung()){
							pNewGeneration->SetUiRelatedId(pGntData->GetUiElderId());
						}
						else{
							pNewGeneration->SetUiRelatedId(pGntData->GetUiYoungerId());
							pNewGeneration->SetUiGiftPointOrHistAccm(pGntData->GetUiGiftPointOrHistAccm());
						}
						pNewGeneration->SetModifyState(FALSE);
						pchar->m_pGeneration->m_mapGeneration.insert(std::make_pair(pNewGeneration->GetUiRelatedId(), pNewGeneration));
						CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(pNewGeneration->GetUiRelatedId());
						if (NULL != pTargetChar)
							pTargetChar->GenerationOnline(pchar->GetChar_id());
						else
							WorldService_Proxy::GenerationOnlineNotifyTrans(gGlobal.pWorldService->pPeer,context,pNewGeneration->GetUiRelatedId(),pchar->GetChar_id());
					}
				}
			}

			DeleteNetGroup(gtnDataGroups, BOOLEAN_TRUE);
	//////////////////////////////////////////////////////////////////////////
	
		MessageService_Proxy::GetTargetInfo(gGlobal.pMessageService->pPeer,context, pchar->GetChar_id());
		PROCEDURE_WAIT1(19, MessageService, CB_GetTargetInfo, NetObject*, msgChar)

			UInt32 curTime = ::GetTickCount();
			PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
				PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
			CMessageCharacter * pMsgChar = CastNetObject<CMessageCharacter>(msgChar);
		if (pMsgChar)
		{
			pchar->SetData(pMsgChar);
		}
		DeleteNetObject(msgChar);
		///////////////////////////////	
		CMap* pmap = gGlobal.GetMap(chgInfo.map_id, chgInfo.line_id, chgInfo.ownerId);
		if (pmap == NULL)
		{
			CB_Error(pPeer, context, 1, _T("CharacterEnter Error: The map does not exist"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		if (!pchar->PostCharacterEnter(pmap, chgInfo, memberIdList, BOOLEAN_FALSE))
		{
			CB_Error(pPeer, context, 1, _T("CharacterEnter Error: PostCharacterEnter failed"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		//// AntiAddiction system part 2 kicks in /// 
		if (pchar->m_pAntiGameAddict && gConfigure.antiAddict == 1 &&
			addictInfo.needAntiAddict == 1)
			pchar->m_pAntiGameAddict->RaiseAllAntiAddictionEvent(pchar);

		//////////////////////////////////////////
		//partner collection

		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("CharacterEnter END: CB_CharacterEnter, session %s, acct_id %d, char_id %d, total utime %d"), 
			context.sessionId.GetString().c_str(), info.acct_id, info.char_id, curTime - firstTime);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("CharacterEnter Error: ObjectService:CB_Error, errcode %d, %s, acct_id %d, char_id %d"),
				err, errMsg.c_str(), info.acct_id, info.char_id);
			CB_Error(pPeer, context, err, errMsg);

			Session* psess = GetSession(context.sessionId);
			CCharacter* pchar = (psess ? psess->pchar : NULL);
			if (pchar)
				pchar->ClearPeer();
			WorldService_Proxy::ForceLogout(gGlobal.pWorldService->pPeer, context, info.acct_id,
				info.char_id, _T("CharacterEnter Error: DB Error"));
			RemoveSession(context.sessionId);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}
#pragma optimize("", on)			// resume optimization here

RPCResult MapService_Stub::CharacterLeave(LPCPEER pPeer, RPCContext &context, 
										  const Boolean &bCallback)
{
	// get session
	PROCEDURE_START_RPC1(MapService, CharacterLeave, NULL, Boolean, bCallback)

		Session* psess = GetSession(context.sessionId);
		if (psess == NULL || psess->pchar == NULL)
		{
			CB_Error(pPeer, context, 1, _T("CharacterLeave: Client session or character not registered."));
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_T("CharacterLeave Error: Client session %s(%x) or character not registered"),
				context.sessionId.GetString().c_str(), psess);
			RPC_RESULT_FAIL;
		}


		PROCEDURE_DO_METHOD2(CCharacter, DoCharacterLeave, psess->pchar, context, Boolean, bCallback, Boolean, BOOLEAN_FALSE);

	PROCEDURE_WAIT_RETURN(1)

		RemoveSession(context.sessionId);

	PROCEDURE_END

	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::NpcEnter(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, 
									const UInt16 &map_id, const UInt8 &line_id, 
									const UInt16 &map_x, const UInt16 &map_y, 
									const UInt32 &followId)
{
	return RPC_RESULT_OK;
}

// do character jump map
void MapService_Stub::JumpMap(CMapActor *pActor, UInt16 map_id, UInt8 line_id, UInt32 owner_id,
							  UInt16 map_x, UInt16 map_y, UInt16 instId)
{
	TRACE_ENSURE(pActor != NULL);

	CMap* pmap = gGlobal.GetMap(map_id, line_id, owner_id);

	if (pmap && !pmap->IsOpen()) {
		TRACE_VERBOSEDTL_7(GLOBAL_LOGGER, 
			_T("JumpMap: [map not open], actor %d:%d, map_id %d, line_id %d, owner_id %d, x %d, y %d"), 
			pActor->GetNetId().srvId, pActor->GetNetId().objId, map_id, line_id, 
			owner_id, map_x, map_y);
		return;
	}

	TRACE_INFODTL_7(GLOBAL_LOGGER, 
		_T("JumpMap: actor %d:%d, map_id %d, line_id %d, owner_id %d, x %d, y %d"), 
		pActor->GetNetId().srvId, pActor->GetNetId().objId, map_id, line_id, owner_id,
		map_x, map_y);

	RPCContext context;
	Boolean bLocal = (pmap != NULL && pmap->IsOpen());
	EnterInfo info;
	info.map_id = map_id;
	info.line_id = line_id;
	info.ownerId = owner_id;
	info.x = map_x;
	info.y = map_y;
	info.instId = instId;
	UINT32 uiLineupId =13;
	int iKeyPos = 0;

	if (pActor->GetClassID() == Character_CLASSID)
	{
		CCharacter* pchar = (CCharacter*) pActor;
		context.sessionId = pchar->GetSessionId();
		if (pchar == NULL)
		{
			TRACE_WARNDTL(GLOBAL_LOGGER, _F("JumpMap Warning: invalid character actor"));
			return;
		}

		NetGroup grp;
		UInt32Vector memberIdList;

		if (pchar->HasParty()) {//Current in party
			CParty *pParty = pchar->GetParty();
			//Get All On Line Party Member
			StlVector<CCharacter*> vctForceLeaveMem;
			for (UInt16 i = 0; i < pParty->GetCount(); ++i) {
				CCharacter* pGroupMem = pParty->GetChild<CCharacter>(i);
				if (NULL != pGroupMem && NULL != pGroupMem->GetPeer())
				{
					if (!pParty->GetM_bTempLeave(i))//get current in party member 
					{
						memberIdList.push_back(pGroupMem->GetChar_id());
						grp.Add(pGroupMem);
					}
					else if (!bLocal)//remote jump,force temporarily leave member leave party
						vctForceLeaveMem.push_back(pGroupMem);
				}
			}

			for (int j=0; j < vctForceLeaveMem.size(); j++ ) 
			{//forec member leave
				CCharacter* pLeaveChar = vctForceLeaveMem[j];
				if (NULL != pLeaveChar) 
					pParty->Leave(pLeaveChar);
			}

			uiLineupId = pParty->GetM_uLineupId();
			iKeyPos = pParty->GetM_iLineupKeyPos();
		} 
		else if (NULL != pchar->GetTempParty()) //temporarily leave member jump
		{
			CParty *pParty = pchar->GetTempParty();
			if (bLocal)//local jump record party information
			{
				for (UInt16 i = 0; i < pParty->GetCount(); ++i) {
					CCharacter* pGroupMem = pParty->GetChild<CCharacter>(i);
					if (NULL != pGroupMem && NULL != pGroupMem->GetPeer())
						memberIdList.push_back(pGroupMem->GetChar_id());
				}
				uiLineupId = pParty->GetM_uLineupId();
				iKeyPos = pParty->GetM_iLineupKeyPos();
			}
			else//RemoteCharJump() leave party first
				pParty->Leave(pchar);
			grp.Add(pchar);
		}
		else{
			grp.Add(pchar);
		}

		for (UInt16 i = 0; i < grp.GetCount(); ++i)
		{
			pchar = grp.GetChild<CCharacter>(i);
			if (pchar == NULL) {
				TRACE_WARNDTL(GLOBAL_LOGGER, _F("JumpMap Warning: null character"));
				continue;
			}
			context.sessionId = pchar->GetSessionId();
			if (!pchar->ChangeState(CCharacter::ST_JUMP_MAP))	// set to jump state first
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, 
					_T("JumpMap Warning: Character %d cannot change to JUMP state"),
					pchar->GetChar_id());
				continue;
			}

			info.acct_id = pchar->GetAcct_id();
			info.char_id = pchar->GetChar_id();
			info.charNetId = pchar->m_infoNetId;
			info.followId = pchar->GetFollowId();
			info.partnerCollection = pchar->m_bUsePartnerCollectionBuff;

			if (bLocal)
			{
				pchar->CB_ResetChangeLine(pchar->GetPeer(),pchar->GetContext());
				PROCEDURE_DO_FUNC6(MapService_Stub::LocalCharJump, context, 
					CCharacter*, pchar, CMap*, pmap, EnterInfo, info,
					UInt32Vector, memberIdList,UINT32,uiLineupId,int,iKeyPos)
			}
			else
			{
				PROCEDURE_DO_FUNC5(MapService_Stub::RemoteCharJump, context, 
					CCharacter*, pchar, EnterInfo, info,
					UInt32Vector, memberIdList,UINT32,uiLineupId,int,iKeyPos)
			}
		}
	}
	else if (pActor->GetClassID() == MapNPC_CLASSID)
	{
		/*
		// npc jump map
		CMapNPC* npc = (CMapNPC*) pActor;
		if (npc == NULL)
		{
			pActor->ChangeState(CMapActor::ST_ENTER_MAP);
			TRACE_WARNDTL(GLOBAL_LOGGER, _F("JumpMap Warning: invalid NPC actor"));
			return;
		}
		if (bLocal)
		{
			PROCEDURE_DO_FUNC4(MapService_Stub::LocalNpcJump, context, 
				CMapNPC*, npc, CMap*, pmap, UInt16, map_x, UInt16, map_y)
		}
		else
		{
			PROCEDURE_DO_FUNC5(MapService_Stub::RemoteNpcJump, context, 
				CMapNPC*, npc, UInt16, map_id, UInt8, line_id, 
				UInt16, map_x, UInt16, map_y)
		}
		*/
	}
}

RPCResult MapService_Stub::LocalCharJump(RPCContext &context)
{
	PROCEDURE_START_FUNC6(MapService_Stub::LocalCharJump, context, CCharacter*, pchar, 
		CMap*, pmap, EnterInfo, info, UInt32Vector, memberIdList,
		UINT32,uiLineupId,int,iKeyPos)

		TRACE_ENSURE(pchar && pmap);

	TRACE_INFODTL_8(GLOBAL_LOGGER, _F("LocalCharJump: char_id %d, netId %d:%d, map_id %d, line_id %d, owner_id %d, x %d, y %d"), 
			pchar->GetChar_id(), pchar->GetNetId().srvId, pchar->GetNetId().objId, 
			info.map_id, info.line_id, info.ownerId, info.x, info.y);

		if (pchar->GetMap() == NULL)
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("LocalCharJump Warning: character %d already leave map"),
				pchar->GetChar_id());
			PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
		}
			
		// can't jump when script running
		if (pchar->GetScript()->IsRunning())
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("LocalCharJump Warning: character %d script is running"),
				pchar->GetChar_id());
			CScriptSystem* pScript = pchar->GetScript();
			if (pScript->GetNpcId() == 0) 
			{					// stop item script running
				TRACE_INFODTL_3(GLOBAL_LOGGER, _F("stop item script %s %d:%d"),
					pchar->GetContext().sessionId.GetString().c_str(),
					pchar->GetContext().pcdId.srvId, pchar->GetContext().pcdId.subId);
				pScript->StopScript();
			} 
			else 
			{
				PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
			}
		}

		uiLineupId = 13;
		iKeyPos = 0;

		// local jump,keep party
		if (pchar->HasParty())
		{
			CParty* pParty= pchar->GetParty();
			uiLineupId = pParty->GetM_uLineupId();
			iKeyPos = pParty->GetM_iLineupKeyPos();
			//if (pParty->GetLeader()) // try to leave instance if inside for party
			//	pParty->GetLeader()->OnLeaveInst(info.instId, info.ownerId);
			//pParty->Destroy(BOOLEAN_FALSE);
		}
		else if (memberIdList.empty())
		{
			pchar->OnLeaveInst(info.instId, info.ownerId); 
			// try to leave instance if inside for single
		}
		else//temporarily leave member jump
		{
			/*CParty* pParty= pchar->GetTempParty();
			if (NULL != pParty) 
			{
				CCharacter* pLeader=pParty->GetLeader();
				if (NULL != pLeader) 
				{
					if (NULL != pLeader->m_viewManager) 
					{
						pLeader->m_viewManager->RemoveLink();
					}
				}
			}*/
			pchar->OnLeaveInst(info.instId, info.ownerId); 
		}

		if (pmap && pmap->GetInst_id() != 0 && pmap->GetOwner_id() != 0)
		{
			CInstance* pInst = CInstance::GetInstance(pmap->GetInst_id(), pmap->GetOwner_id());
			if (pInst)
				pInst->OnPreEnter();
		}
 
		// character leave map event handle
		pchar->OnLeaveMap();

		//call the message server to leave the current map channel
		if (pchar->GetPeer())
			MapService_Stub::CB_LeaveSuccess(pchar->GetPeer(), pchar->GetContext(), BOOLEAN_FALSE);

		// update world service for new location
		WorldService_Proxy::CharUpdateLocation(gGlobal.pWorldService->pPeer, context, info);

	PROCEDURE_WAIT(1, WorldService, CB_UpdateLocation)

		TRACE_ENSURE(pchar && pmap);

		UInt32 owner_id = pchar->GetMap_ownerId();

		// update to object service
		CParty* pTempParty = pchar->GetParty();
		if (NULL == pTempParty)
			pTempParty = pchar->GetTempParty();
		if (!pchar->PreCharacterEnter(pmap, info, memberIdList, BOOLEAN_TRUE,uiLineupId,iKeyPos,pTempParty))
		{
			PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
		}

		// try to enter instance from outside
		if (owner_id == 0 && info.ownerId != 0)
			pchar->OnEnterInst(info.instId, info.ownerId);
		
		CPartyCharInfo Cpc;
		Cpc.SetData(pchar);
		if (NULL != pchar->HasParty())
		{//Current In party
			pchar->GetParty()->RefreshPartyCharInfo(&Cpc,BOOLEAN_FALSE);
			CParty* pParty = pchar->GetParty();
			for (UInt16 k = 0;k < pParty->GetCount();k++) 
			{
				if (pParty->GetM_bTempLeave(k)) 
				{
					CCharacter* pMember = pParty->GetChild<CCharacter>(k);
					if (NULL != pMember && pMember != pchar) 
					{
						Cpc.SetData(pMember);
						Cpc.SetSendMask(CPartyCharInfo::AllMask);
						pchar->CB_RefreshPartyCharInfo(pchar->GetPeer(),pchar->GetContext(),&Cpc);
					}
				}
			}
		}
		else if (NULL != pchar->GetTempParty()) 
		{//current temporarily leave party
			pchar->GetTempParty()->RefreshPartyCharInfo(&Cpc,BOOLEAN_TRUE);
			CParty* pParty = pchar->GetTempParty();
			for (UInt16 k = 0;k < pParty->GetCount();k++) 
			{
				if (pParty->GetM_bTempLeave(k)) 
				{
					CCharacter* pMember = pParty->GetChild<CCharacter>(k);
					if (NULL != pMember && pMember != pchar) 
					{
						Cpc.SetData(pMember);
						Cpc.SetSendMask(CPartyCharInfo::AllMask);
						pchar->CB_RefreshPartyCharInfo(pchar->GetPeer(),pchar->GetContext(),&Cpc);
					}
				}
				else if (0 == k) 
				{
					CCharacter* pLeader = pParty->GetLeader();
					if (NULL != pLeader) 
					{
						Cpc.SetData(pLeader);
						Cpc.SetSendMask(CPartyCharInfo::AllMask);
						pchar->CB_RefreshPartyCharInfo(pchar->GetPeer(),pchar->GetContext(),&Cpc);
					}
				}
			}
		}


	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("LocalCharJump Error: WorldService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

		PROCEDURE_EXCEPTION_DEFAULT

			if (pchar->GetMap())
			{
				pchar->ChangeState(CCharacter::ST_ENTER_MAP);
				pchar->SetMap(pchar->GetMap()->GetMap_id(), pchar->GetCurPosX(), 
					pchar->GetCurPosY(), 0, pchar->GetMap_ownerId());
			}

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::RemoteCharJump(RPCContext &context)
{
	PROCEDURE_START_FUNC5(MapService_Stub::RemoteCharJump, context, CCharacter*, pchar, 
		EnterInfo, info, UInt32Vector, memberIdList,
		UINT32,uiLineupId,int,iKeyPos)

		TRACE_ENSURE(pchar);

		if (pchar->GetMap() == NULL)
		{
 			TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("RemoteCharJump Warning: character %d already leave map"),
				pchar->GetChar_id());
			PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)
		}

		// verify map in world service
		WorldService_Proxy::ValidateMap(gGlobal.pWorldService->pPeer, context, 
			info.map_id, info.line_id, info.ownerId);

	PROCEDURE_WAIT1(1, WorldService, CB_ValidMap, Boolean, bOpen)

		TRACE_ENSURE(pchar);
		if (!bOpen)
		{				// map not opened
			TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, 
				_T("RemoteCharJump: [map not open] map_id %d, line_id %d, owner_id %d"), 
				info.map_id, info.line_id, info.ownerId);
			pchar->ChangeState(CCharacter::ST_ENTER_MAP);
			PROCEDURE_RETURN
		}

		TRACE_VERBOSEDTL_8(GLOBAL_LOGGER, _F("RemoteCharJump: char_id %d, netId %d:%d, map_id %d, line_id %d, owner_id %d, x %d, y %d"), 
			pchar->GetChar_id(), pchar->GetNetId().srvId, pchar->GetNetId().objId, 
			info.map_id, info.line_id, info.ownerId, info.x, info.y);

		// can't jump when script running
		if (pchar->GetScript()->IsRunning())
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("LocalCharJump Warning: character %d script is running"),
				pchar->GetChar_id());
			CScriptSystem* pScript = pchar->GetScript();
			if (pScript->GetNpcId() == 0) 
			{					// stop item script running
				TRACE_INFODTL_3(GLOBAL_LOGGER, _F("2 stop item script %s %d:%d"),
					pchar->GetContext().sessionId.GetString().c_str(),
					pchar->GetContext().pcdId.srvId, pchar->GetContext().pcdId.subId);
				pScript->StopScript();
			} 
			else 
			{
				PROCEDURE_ERROR_BREAK(PCD_ERR_GENERAL)		// otherwise jump failed 
			}
		}
		uiLineupId = 13;
		iKeyPos = 0;

		// destroy party before jump map
		if (pchar->HasParty())
		{
			CParty* pParty = pchar->GetParty();
			uiLineupId = pParty->GetM_uLineupId();
			iKeyPos = pParty->GetM_iLineupKeyPos();
			if (pParty->GetLeader()) // try to leave instance if inside for party
				pParty->GetLeader()->OnLeaveInst(info.instId, info.ownerId);
			pParty->Destroy(BOOLEAN_FALSE);
		}
		else if (memberIdList.empty())
		{
			// try to leave instance if inside for single
			pchar->OnLeaveInst(info.instId, info.ownerId); 
		}

		// character leave first
		PROCEDURE_DO_METHOD2(CCharacter, DoCharacterLeave, pchar, context, 
							Boolean, BOOLEAN_TRUE, Boolean, BOOLEAN_TRUE);

	PROCEDURE_WAIT_RETURN(2)

		// ask world service to jump map
		TRACE_ENSURE(pchar);
	
		///// anti-addiction information passed to the different map
		AntiAddictInfo addictData;
		addictData.onlineTime = pchar->m_pAntiGameAddict->m_onlineTime;
		addictData.offlineTime = pchar->m_pAntiGameAddict->m_offlineTime;
		addictData.disconnectTime = 0;
		addictData.loginTime = pchar->m_pAntiGameAddict->m_loginTime;
		addictData.startHack = pchar->m_pAntiGameAddict->m_startGame;
		addictData.stopPlay = pchar->m_pAntiGameAddict->m_bStopPlay;
		addictData.needAntiAddict = pchar->m_pAntiGameAddict->m_needAntiAddict;
		
		WorldService_Proxy::CharJumpMap(gGlobal.pWorldService->pPeer, context, 
			info, memberIdList, addictData, pchar->GetBuffCache(),uiLineupId,iKeyPos);

	//PROCEDURE_WAIT(3, WorldService, CB_JumpSuccess)

		// delete character after jump
		TRACE_ENSURE(pchar);

		RemoveSession(context.sessionId);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("RemoteCharJump Error: WorldService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("RemoteCharJump Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

		PROCEDURE_EXCEPTION_DEFAULT

			pchar->ChangeState(CCharacter::ST_ENTER_MAP);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}


RPCResult MapService_Stub::LocalNpcJump(RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::RemoteNpcJump(RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::CharacterCheat(LPCPEER pPeer, RPCContext &context, const UInt32 &exe_id,
										  const UInt32 &SrcAcct_id, const UInt32 &target_acct_id,
										  const UInt32 &target_char_id, const String &command)
{
	CCharacter* pchar = gGlobal.m_charToSession.FindCharacter(target_char_id);
	if( pchar )
	{
		if( pchar->CheatCode(pPeer, context, SrcAcct_id, command) == RPC_RESULT_OK )
		{
			LogService_Proxy::LogCheat(gGlobal.pLogService->pPeer, pchar->GetContext(), 
				exe_id, SrcAcct_id, target_acct_id, target_char_id, 
				command.c_str(), LOGCHEAT_SUCCEED);
			return RPC_RESULT_OK;
		}else
		{
			LogService_Proxy::LogCheat(gGlobal.pLogService->pPeer, pchar->GetContext(),
				exe_id, SrcAcct_id, target_acct_id, target_char_id, 
				command.c_str(), LOGCHEAT_ERR_CHEATFAIL);
			return RPC_RESULT_FAIL;
		}
	}else
	{
		LogService_Proxy::LogCheat(gGlobal.pLogService->pPeer, RPCContext(),
			exe_id, SrcAcct_id, target_acct_id, target_char_id, 
			command.c_str(), LOGCHEAT_ERR_CHARNOTFOUNDINMAP);
		return RPC_RESULT_FAIL;
	}
}

RPCResult MapService_Stub::ChangeLoadLevel(LPCPEER pPeer, RPCContext &context, const UInt8 &level)
{
	gGlobal.ChgLoadLevel(level);

	return RPC_RESULT_FAIL;
}


RPCResult MapService_Stub::ShowPublicItemDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &item_uid, const UInt32 &char_idTo)
{
	CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(char_id);
	if (pChar)
	{
		CItem* item = pChar->m_pBag->GetItemAndEquipByUID(item_uid);
		if (item)
		{
			
			CItem temp;
			CItemData tempData;
			
			item->SetUpdateMask(CItem::AllMask);
			item->WriteData(&tempData);
			item->ClearAllMask();
			
			temp.SetData(&tempData);
			temp.SetId(0, temp.GetItem_id());
			temp.SetSendMask(CItem::AllMask);

			CB_ShowPublicItemDetail(pPeer, context, &temp);
		}
		else
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("Cannot Find item_uid  %d"), item_uid);
		}
	}
	else if (gGlobal.g_bShowItemPartnerPetOfOtherLine)
	{
		MessageService_Proxy::ShowPublicItemDetailFromOtherLine(gGlobal.pMessageService->pPeer, 
			context, char_id, item_uid, char_idTo);
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("Cannot Find Char ID %d"), char_id);
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &item_uid, const UInt32 &char_idTo)
{
	CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(char_id);
	if (pChar)
	{
		CItem* item = pChar->m_pBag->GetItemAndEquipByUID(item_uid);
		if (item)
		{

			CItem temp;
			CItemData tempData;

			item->SetUpdateMask(CItem::AllMask);
			item->WriteData(&tempData);
			item->ClearAllMask();

			temp.SetData(&tempData);
			temp.SetId(0, temp.GetItem_id());
			temp.SetSendMask(CItem::AllMask);

			CB_ShowPublicItemDetailFromOtherLine(pPeer, context, &temp, char_idTo);
		}
		else
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("Cannot Find item_uid  %d, char_idTo %d"), item_uid, char_idTo);
		}
	}
	else
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("Cannot Find Char ID %d, char_idTo %d"), char_id, char_idTo);
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::ShowPublicPartnerDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &partner_id,const UInt32 &char_idTo)
{
	CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(char_id);
	if (pChar)
	{
		CPartner *pPartner = pChar->m_pPartnerGroup->GetPartner(partner_id);
		if(pPartner)
		{
			CPartner temp;				
			pPartner->ClonePartner(&temp);
			temp.CalcAttrib2();
			
			temp.SetId(0, temp.GetMob_id());
			temp.SetSendMask(CPartner::AllMask);
			UInt16Vector skillIDs;
			if (pPartner->GetSkillGroup())
			{
				for (UInt8 i = 0; i < pPartner->GetSkillGroup()->GetCount(); ++i)
				{
					CSkill* skill = pPartner->GetSkillGroup()->GetChild<CSkill>(i);
					if (skill)
						skillIDs.push_back(skill->GetSkill_id());
				}
			}

			CB_ShowPublicPartnerDetail(pPeer, context, &temp, skillIDs);
		}
		else
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("Cannot Find partner_id  %d"), partner_id);
		}
	}
	else if (gGlobal.g_bShowItemPartnerPetOfOtherLine)
	{
		MessageService_Proxy::ShowPublicPartnerDetailFromOtherLine(gGlobal.pMessageService->pPeer, 
			context, char_id, partner_id, char_idTo);
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("Cannot Find Char ID %d"), char_id);
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &partner_id, const UInt32 &char_idTo)
{
	CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(char_id);
	if (pChar)
	{
		CPartner *pPartner = pChar->m_pPartnerGroup->GetPartner(partner_id);
		if(pPartner)
		{
			CPartner temp;				
			pPartner->ClonePartner(&temp);
			temp.CalcAttrib2();

			temp.SetId(0, temp.GetMob_id());
			temp.SetSendMask(CPartner::AllMask);
			UInt16Vector skillIDs;
			if (pPartner->GetSkillGroup())
			{
				for (UInt8 i = 0; i < pPartner->GetSkillGroup()->GetCount(); ++i)
				{
					CSkill* skill = pPartner->GetSkillGroup()->GetChild<CSkill>(i);
					if (skill)
						skillIDs.push_back(skill->GetSkill_id());
				}
			}

			CB_ShowPublicPartnerDetailFromOtherLine(pPeer, context, &temp, skillIDs, char_idTo);
		}
		else
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("Cannot Find partner_id  %d"), partner_id);
		}
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("Cannot Find Char ID %d"), char_id);
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::ShowPublicPetDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &pet_id, const UInt32 &char_idTo)
{
	CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(char_id);
	if (pChar)
	{
		CPet *pPet = pChar->m_pPetGroup->GetPet(pet_id);
		if(pPet)
		{
			CPet temp;				
			pPet->ClonePet(&temp);
			
			//temp.SetId(0, temp.GetMob_id());
			temp.SetSendMask(CPartner::AllMask);
			UInt16Vector skillIDs;
			if (pPet->GetSkillGroup())
			{
				for (UInt8 i = 0; i < pPet->GetSkillGroup()->GetCount(); ++i)
				{
					CSkill* skill = pPet->GetSkillGroup()->GetChild<CSkill>(i);
					if (skill)
						skillIDs.push_back(skill->GetSkill_id());
				}
			}

			CB_ShowPublicPetDetail(pPeer, context, &temp, skillIDs);
		}
		else
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("Cannot Find pet_id  %d"), pet_id);
		}
	}
	else if (gGlobal.g_bShowItemPartnerPetOfOtherLine)
	{
		MessageService_Proxy::ShowPublicPetDetailFromOtherLine(gGlobal.pMessageService->pPeer, 
			context, char_id, pet_id, char_idTo);
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("Cannot Find Char ID %d"), char_id);
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &pet_id, const UInt32 &char_idTo)
{
	CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(char_id);
	if (pChar)
	{
		CPet *pPet = pChar->m_pPetGroup->GetPet(pet_id);
		if(pPet)
		{
			CPet temp;				
			pPet->ClonePet(&temp);

			//temp.SetId(0, temp.GetMob_id());
			temp.SetSendMask(CPartner::AllMask);
			UInt16Vector skillIDs;
			if (pPet->GetSkillGroup())
			{
				for (UInt8 i = 0; i < pPet->GetSkillGroup()->GetCount(); ++i)
				{
					CSkill* skill = pPet->GetSkillGroup()->GetChild<CSkill>(i);
					if (skill)
						skillIDs.push_back(skill->GetSkill_id());
				}
			}

			CB_ShowPublicPetDetailFromOtherLine(pPeer, context, &temp, skillIDs, char_idTo);
		}
		else
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("Cannot Find pet_id  %d"), pet_id);
		}
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("Cannot Find Char ID %d"), char_id);
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::GuildUpdate(LPCPEER pPeer, RPCContext &context, NetGroup* addGrp)
{
	for (Int i = 0; i < addGrp->GetCount(); i++)
	{
		CGuild * pGuild = addGrp->GetChild<CGuild >(i);
		if (pGuild)
		{
			GuildMap::iterator Itr = gGlobal.g_AllGuilds.find(pGuild->GetGuild_uid());
			if (Itr != gGlobal.g_AllGuilds.end() && Itr->second != pGuild)
			{
				TRACE_INFODTL_3(GLOBAL_LOGGER, _F("delete AllGuilds guild_uid %d, objid %d:%d"),
					pGuild->GetGuild_uid(), Itr->second->GetNetId().srvId, Itr->second->GetNetId().objId
					);
				DeleteNetObject(Itr->second);
				gGlobal.g_AllGuilds.erase(Itr);
			}
			gGlobal.g_AllGuilds.insert(std::make_pair(pGuild->GetGuild_uid(), pGuild));
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("add AllGuilds guild_uid %d, objid %d:%d"),
				pGuild->GetGuild_uid(), pGuild->GetNetId().srvId, pGuild->GetNetId().objId
				);
		}
	}

	DeleteNetGroup(addGrp);
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::ManorForceOut(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_UID, const UInt32Vector &guild_charUIDs, const Boolean &memberKick)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ManorForceOut: all members in the guild with uid = %d need to be expeled out of all manor instances"), guild_UID);
	GuildMap::iterator ItrGuild = gGlobal.g_AllGuilds.find(guild_UID);
	if (ItrGuild == gGlobal.g_AllGuilds.end()) // no such guild exist
		return RPC_RESULT_OK; 

	for (Int i = 0; i < (Int)guild_charUIDs.size(); ++i)
	{
		if (memberKick) // if the member is kicked out of guild, he has to leave the manor instance immediately
		{
			CCharacter *pChar = gGlobal.m_charToSession.FindCharacter(guild_charUIDs[i]);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ManorForceOut: character with uid = %d got his guild membership removed"), guild_charUIDs[i]);
			if (!pChar)
				continue;
			if (pChar->GetMap())
			{
				UInt32 instantID = pChar->GetMap()->GetInst_id();
				EnterInfo info;
				info.map_id = pChar->GetMap()->GetMap_id();
				TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ManorForceOut: Guild membership removed, character with uid = %d is located in mapId = %d and instance id = %d"), guild_charUIDs[i], info.map_id, instantID);
				if (instantID != 0 && IsInManorInstance(info)) // kick the people inside the manor instance
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ManorForceOut: Guild membership removed, character with uid = %d is forced to leave instance located in mapId = %d and instance id = %d"), guild_charUIDs[i], info.map_id, instantID);
					if (pChar->GetParty()&& !pChar->IsState(CCharacter::ST_BATTLE) && !pChar->IsStartBattle()) 
					{	// leave the party first,if any, before leave the instance
						pChar->GetParty()->Leave(pChar);
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ManorForceOut: Guild membership removed, character with uid = %d leave the party before leave the instance"), guild_charUIDs[i]);
					}
					else if (pChar->GetParty() && (pChar->IsState(CCharacter::ST_BATTLE) || pChar->IsStartBattle()))
					{
						pChar->m_bManorExpel = BOOLEAN_TRUE; // need to expel from manor after the battle, 
					}
					if (pChar->IsLeader())
	                    pChar->LeaveInstance(instantID, guild_UID, pChar->GetReborn_map(), pChar->GetReborn_x(), pChar->GetReborn_y(), 1);
				}
			}
		}
		else
		{   // the member has to leave all manor instance just because the manor is upgrade or downgrade
			GuildMemberMap::iterator Itr = gGlobal.g_AllGuildMembers.find(guild_charUIDs[i]);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ManorForceOut: character with uid = %d got his guild upgraded or downgraded"), guild_charUIDs[i]);
			if (Itr != gGlobal.g_AllGuildMembers.end())
			{
				CGuildMember *pGuildMember = NULL;
				CCharacter *pChar = NULL;	
				pGuildMember = Itr->second;
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ManorForceOut: Manor scale change, character with uid = %d is found"), guild_charUIDs[i]);
				if (!pGuildMember || (pGuildMember->GetGuild_uid() != guild_UID)) // check if the the guild members exist or does it belong the intended guild
					continue;
				pChar = gGlobal.m_charToSession.FindCharacter(pGuildMember->GetChar_id()); // check if the corresponding character exist
				if (!pChar)
					continue;
				// now ask character to jump to birth place
				if (pChar->GetMap())
				{
					UInt32 instantID = pChar->GetMap()->GetInst_id();
					EnterInfo info;
					info.map_id = pChar->GetMap()->GetMap_id();
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ManorForceOut: Manor scale change, character with uid = %d is located in mapId = %d and instance id = %d"), guild_charUIDs[i], info.map_id, instantID);
					if (instantID != 0 && IsInManorInstance(info)) // kick the people inside the manor instance
					{
						TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ManorForceOut: Manor scale change, character with uid = %d have to leave instance with mapId = %d and instance id = %d"), guild_charUIDs[i], info.map_id, instantID);
						if (pChar->GetParty() && !pChar->IsState(CCharacter::ST_BATTLE) && !pChar->IsStartBattle()) 
						{   // leave the party first,if any, before leave the instance
							pChar->GetParty()->Leave(pChar);
							TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ManorForceOut: Manor scale change, character with uid = %d leave the party before leave the instance"), guild_charUIDs[i]);
						}
						else if (pChar->GetParty() && (pChar->IsState(CCharacter::ST_BATTLE) || pChar->IsStartBattle()))
						{
							pChar->m_bManorExpel = BOOLEAN_TRUE; // need to expel from manor after the battle, 
						}
						if (pChar->IsLeader())
                            pChar->LeaveInstance(instantID, pGuildMember->GetGuild_uid(), pChar->GetReborn_map(), pChar->GetReborn_x(), pChar->GetReborn_y(), 1);
					}
				}
			}
		}
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::GuildRemove(LPCPEER pPeer, RPCContext &context, const UInt32Vector &guild_uids)
{
	for (UInt i = 0; i < guild_uids.size(); i ++)
	{
		GuildMap::iterator Itr = gGlobal.g_AllGuilds.find(guild_uids[i]);
		if (Itr != gGlobal.g_AllGuilds.end())
		{
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("delete AllGuilds guild_uid %d, objid %d:%d"),
				guild_uids[i], Itr->second->GetNetId().srvId, Itr->second->GetNetId().objId
				);
			DeleteNetObject(Itr->second);
			gGlobal.g_AllGuilds.erase(Itr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::GuildMemberUpdate(LPCPEER pPeer, RPCContext &context, NetGroup* addGrp)
{
	for (Int i = 0; i < addGrp->GetCount(); i++)
	{
		CGuildMember * pGuildMember = addGrp->GetChild<CGuildMember>(i);
		if (pGuildMember)
		{
			GuildMemberMap::iterator Itr = gGlobal.g_AllGuildMembers.find(pGuildMember->GetChar_id());
			if (Itr != gGlobal.g_AllGuildMembers.end() && Itr->second != pGuildMember)
			{
				DeleteNetObject(Itr->second);
				gGlobal.g_AllGuildMembers.erase(Itr);
			}
			gGlobal.g_AllGuildMembers.insert(std::make_pair(pGuildMember->GetChar_id(), pGuildMember));

			CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(pGuildMember->GetChar_id());
			if (pChar)
			{
				if (pGuildMember->IsUpdated(CGuildMember::postMask))
					pChar->RemoveGuildTitle();
				pChar->m_ownGuildMember = pGuildMember;
				pChar->SetGuildUid(pGuildMember->GetGuild_uid());
				GuildMap::iterator ItrGuild = gGlobal.g_AllGuilds.find(pGuildMember->GetGuild_uid());
				if (ItrGuild != gGlobal.g_AllGuilds.end())
				{
					pChar->m_ownGuild = ItrGuild->second;
				}
				if (pGuildMember->IsUpdated(CGuildMember::postMask))
				{
					const TitleData * pTitleData = gGlobal.m_resource->GetTitleDataByType(TITLE_TYPE_GUILD, 0, pGuildMember->GetPost(), pGuildMember->GetPost());
					if (pTitleData)
					{
						pChar->AddTitle(pTitleData->Title_id);
					}
					if (pGuildMember->GetPost() == GUILD_POSITION_TRADER && pChar->GetBag() && pChar->GetBag()->GetBusinessMoney() <= 0)
					{
						MessageService_Proxy::ChangeBusinessProfit(pPeer, context, pChar->GetGuildUid(), pChar->GetChar_id(), 0);
					}
				}
			}
		}
	}
	DeleteNetGroup(addGrp);
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::GuildMemberRemove(LPCPEER pPeer, RPCContext &context, const UInt32Vector &char_ids)
{
	for (UInt i = 0; i < char_ids.size(); i ++)
	{
		GuildMemberMap::iterator Itr = gGlobal.g_AllGuildMembers.find(char_ids[i]);
		if (Itr != gGlobal.g_AllGuildMembers.end())
		{
			DeleteNetObject(Itr->second);
			gGlobal.g_AllGuildMembers.erase(Itr);
			CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(char_ids[i]);
			if (pChar)
			{
				pChar->SetGuildUid(0);
				pChar->RemoveGuildTitle();
				pChar->m_ownGuildMember = NULL;
				pChar->m_ownGuild = NULL;
			}
		}
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::MsgCharacterUpdate(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	if (grp == NULL)
		return RPC_RESULT_OK;
	for (Int i = 0; i < grp->GetCount(); i++)
	{
		CMessageCharacter * pMsgData = grp->GetChild<CMessageCharacter >(i);
		if (pMsgData && pMsgData->IsUpdated(CMessageCharacter::speakCountMask))
		{
			CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(pMsgData->GetChar_id());
			if (pChar)
			{
				pChar->SetSpeakCount(pMsgData->GetGlobalSpeakCount());
			}			
		}
	}
	DeleteNetGroup(grp , BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}
RPCResult MapService_Stub::SendMapMessage(LPCPEER pPeer, RPCContext &context, const RPCNetID &charNetID, const String &Msg)
{
	static Logger logger(_T("LogCurrentMessage"));
	static Boolean bOpen = OpenLogger(logger);

	UInt sizeofMsg= NoCodeStringSize(String(Msg));
	if (sizeofMsg > gConfigure.maxMsgLen)
	{
		CB_Error(pPeer, context, 0, _T("MSG_MESSAGE_TOO_LONG"));
		return RPC_RESULT_OK;
	}
	CCharacter* pOwner = FindNetObject<CCharacter>(charNetID);
	if (pOwner && pOwner->GetPeer())
	{
		UInt32 curTime = (UInt32) time(NULL);
		if (curTime < SPEAK_REST_TIME + pOwner->m_PreSpeakTime)
		{
			pOwner->CB_SendGrpMessage(pOwner->GetPeer(), pOwner->GetContext(), MSG_RETURN_FAIL_TOO_FREQUENT, SPEAK_REST_TIME + pOwner->m_PreSpeakTime - curTime);
			return RPC_RESULT_OK;
		}
		else if (pOwner->GetSpeakBlock() > (UInt32)curTime)
		{
			pOwner->CB_SendGrpMessage(pOwner->GetPeer(), pOwner->GetContext(), MSG_RETURN_FAIL_NO_RIGHT_BLOCK, 0);
			return RPC_RESULT_OK;
		}
		if (pOwner->m_viewManager)
		{
			CharacterPtrList chars;
			pOwner->m_viewManager->GetCharacters(chars);

			ProxyListenersMap proxys;
			for (CharacterPtrList::iterator it = chars.begin(); it != chars.end(); ++it)
			{
				if (!(*it)->m_Listen || (*it)->IsState(CCharacter::ST_BATTLE)
					|| (*it)->IsState(CCharacter::ST_MONITOR))
					continue;

				if ((*it)->GetPeer())
					proxys[(*it)->GetPeer()].push_back((*it)->GetContext().sessionId);
			}

			if (pOwner->GetPeer())
				proxys[pOwner->GetPeer()].push_back(pOwner->GetContext().sessionId);
			String blockedMsg = Msg;
			gGlobal.BlockWord(blockedMsg);

			for (ProxyListenersMap::iterator it = proxys.begin(); it != proxys.end(); ++it)
				CB_ReceiveMapMessage(it->first, RPCContext(), blockedMsg, pOwner->GetNickName(), it->second);
			pOwner->m_PreSpeakTime = curTime;

			if (bOpen)
			{				
				time_t nowTime;
				time(&nowTime);
				struct tm *newtime = localtime(&nowTime);
				String timeStr;
				timeStr.Format(_T("[%04d-%02d-%02d %02d.%02d.%02d]"), newtime->tm_year + 1900, newtime->tm_mon + 1, 
					newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
				TRACE_ALWAYSL_5(logger,
					_T("%s session_id %s, char_id %d, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), pOwner->GetChar_id(), String(ConvAnsiString(pOwner->GetNickName().c_str())).c_str(), 
					String(ConvAnsiString(blockedMsg.c_str())).c_str());
			}
		}
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::SendMapEmotion(LPCPEER pPeer, RPCContext &context, const RPCNetID &charNetID, const UInt32 &emo_id, const RPCNetID &TargetNetID, const bool &isPet)
{
	CCharacter * pOwner = FindNetObject<CCharacter>(charNetID);
	if (pOwner && pOwner->GetPeer())
	{
		if (pOwner->m_viewManager)
		{
			CCharacter * pChar = FindNetObject<CCharacter>(TargetNetID);
			if (pChar && pChar->GetPeer())
			{
				MapService_Stub::CB_ReceiveMapEmotion(pChar->GetPeer(), pChar->GetContext(), emo_id, pOwner->GetNickName(), pChar->GetNickName(),isPet);
			} 
		}
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::OpenChargeShop(LPCPEER pPeer, RPCContext &context)
{
	Session *psess = GetSession(context.sessionId);
	if (gGlobal.m_pChargeShop && psess && psess->pchar)
	{
		gGlobal.m_pChargeShop->PreSend();
		NetGroup temp = gGlobal.m_pChargeShop->GetDiscountItem();
		CChargeShopItem* temp1 = gGlobal.m_pChargeShop->GetLimitedItem();
		if (temp1)
			temp1->PreSend();

		gGlobal.m_pChargeShop->AddCharID(psess->pchar->GetChar_id(), context);
		SetOwnObject(context.sessionId, gGlobal.m_pChargeShop);
		CB_OpenChargeShop(pPeer, context, gGlobal.m_pChargeShop, &temp, temp1);
	}

	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::CloseChargeShop(LPCPEER pPeer, RPCContext &context)
{
	Session *psess = GetSession(context.sessionId);
	UnsetOwnObject(context.sessionId, gGlobal.m_pChargeShop);
	if (gGlobal.m_pChargeShop && psess && psess->pchar)
	{
		gGlobal.m_pChargeShop->RemoveCharID(psess->pchar->GetChar_id());
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::UpdateLimitedItemRemain(LPCPEER pPeer, RPCContext &context, 
												   NetGroup* limitedItemData)
{
	if (limitedItemData)
	{
		for (Index i = C_INDEX(0); i < limitedItemData->GetCount(); ++i)
		{
			CChargeShopData* pData = limitedItemData->GetChild<CChargeShopData>(i);
			if (pData)
				gGlobal.m_pChargeShop->UpdateLimitedItem(pData);
		}
		DeleteNetGroup(limitedItemData, BOOLEAN_TRUE);
	}
	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::RemoteCreateNpc(LPCPEER pPeer, RPCContext &context, 
										   const UInt16 &map_id, const UInt8 &line_id, 
										   const UInt32 &owner_id, 
										   const NpcCreateList &npcList)
{
	CMap* pMap = gGlobal.GetMap(map_id, line_id, owner_id);
	if (pMap == NULL)
		return RPC_RESULT_FAIL;

	for (NpcCreateList::const_iterator it = npcList.begin(); it != npcList.end(); ++it)
	{
		const NpcCreateType &npc = *it;
		TRACE_INFODTL_6(GLOBAL_LOGGER, 
			_F("npc_id %d, map_id %d, line_id %d, owner_id %d, x %d, y %d"), 
			npc.npc_id, map_id, line_id, owner_id, npc.x, npc.y);
		pMap->CreateNpc(npc.npc_id, npc.x, npc.y, 0, npc.lifeTime, npc.eventId, npc.char_id);
	}

	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::QueryInstance(LPCPEER pPeer, RPCContext &context, 
										 const UInt16 &instId, const UInt32 &ownerId)
{
	CInstance* pInst = CInstance::GetInstance(instId, ownerId);
	Boolean bExist = (pInst != NULL);
	UInt16 allowCount = (pInst ? pInst->GetAllowCount() : 0);

	CB_QueryInstance(pPeer, context, bExist, allowCount);

	return RPC_RESULT_OK;
}

RPCResult MapService_Stub::ChangeMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const Int32 &change)
{
	CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(owner_id);
	if (pChar)
	{
		pChar->ChangeMoney(change);
	}
	return RPC_RESULT_OK;
}
RPCResult MapService_Stub::GetPosition(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	CCharacter * pChar = gGlobal.m_charToSession.FindCharacter(targetId);
	if (pChar)
	{
		CB_GetPosition(pPeer,context,pChar->GetCurPosX(),pChar->GetCurPosY());
		return RPC_RESULT_OK;
	}
	CB_GetPosition(pPeer,context,0,0);
	return RPC_RESULT_OK;
}
RPCResult MapService_Stub::UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money)
{
	CCharacter* pTarget = gGlobal.m_charToSession.FindCharacter(targetId);
	if(pTarget)
		pTarget->SetWantedReward(pTarget->GetWantedReward()+money);
	return RPC_RESULT_OK;
}

Boolean MapService_Stub::IsInManorInstance(const EnterInfo &info)
{
	for (UInt i = 1; i <= 5; i++)
	{
        const ManorData *manorData = gGlobal.m_resource->GetManorData(i);
		if (!manorData)
			break;
		for (UInt j = 0; j < manorData->manor_map_id.size(); j++)
		{
			if (info.map_id == manorData->manor_map_id[j])
				return BOOLEAN_TRUE;
		}
	}

	return BOOLEAN_FALSE;
}
RPCResult MapService_Stub::DeleteGenerationFromWorld(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(uiTargetId);
	if (NULL != pTargetChar) {
		pTargetChar->DeleteGenerationLocal(uiRelatedId);
	}
	return RPC_RESULT_OK;
}
RPCResult MapService_Stub::RecvGenerationAccmulation(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId,const UInt32 &uiAcc)
{
	CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(uiTargetId);
	if (NULL != pTargetChar) {
		pTargetChar->RecvGenerationAcc(uiRelatedId,uiAcc);
	}
	return RPC_RESULT_OK;
}
RPCResult MapService_Stub::GenerationOnlineNotify(LPCPEER pPeer, RPCContext &context,const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(uiTargetId);
	if (NULL != pTargetChar) {
		pTargetChar->GenerationOnline(uiRelatedId);
	}
	return RPC_RESULT_OK;
}
RPCResult MapService_Stub::GenerationOfflineNotify(LPCPEER pPeer, RPCContext &context,const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	CCharacter* pTargetChar = gGlobal.m_charToSession.FindCharacter(uiTargetId);
	if (NULL != pTargetChar) {
		pTargetChar->GenerationOffline(uiRelatedId);
	}
	return RPC_RESULT_OK;
}
RPCResult MapService_Stub::GetGenenrationCharsResult(LPCPEER pPeer, RPCContext &context, NetGroup* pResult)
{
	Session *psess = GetSession(context.sessionId);
	if (NULL != psess) {
		if (NULL != psess->pchar) {
			psess->pchar->InitGenerationInfo(pResult);
		}
	}
	return RPC_RESULT_OK;
}
