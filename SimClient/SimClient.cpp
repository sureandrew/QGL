//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
#include "Common/BattleCode.h"
#include "Common/StateCode.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "SimClient/SimClient.h"
//-- Library
#include "SimClient/Global.h"
#include "SimClient/Configure.h"
#include "Proxy/Service/WorldService_proxy.h"
#include "Proxy/Service/MapService_proxy.h"
#include "SimClient/MessageService.h"
#include "SimClient/Account.h"
#include "SimClient/Character.h"
#include "SimClient/CharacterInfo.h"
#include "SimClient/Map.h"
#include "Proxy/Logic/MapActor.h"
#include "SimClient/Battle.h"
#include "Proxy/Logic/BatActor.h"
#include "SimClient/BatCharacter.h"
#include "SimClient/BatMob.h"
#include "SimClient/Party.h"
#include "SimClient/Channel.h"
#include "SimClient/ItemBag.h"
#include "SimClient/ScriptSystem.h"
#include "SimClient/PartnerGroup.h"
#include "SimClient/Partner.h"
#include "SimClient/SkillGroup.h"
#include "SimClient/QuestSystem.h"
#include "SimClient/QuestLog.h"
#include "SimClient/Messenger.h"
#include "SimClient/MessageCharacter.h"
#include "SimClient/Item.h"
#include "SimClient/ChargeShop.h"
#include "SimClient/ChargeShopItem.h"
#include "SimClient/Trading.h"
#include "SimClient/Pet.h"
#include "SimClient/PetGroup.h"
#include "SimClient/HawkItem.h"
#include "SimClient/HawkGroup.h"
#include "SimClient/GuildMember.h"
#include "SimClient/Guild.h"
#include "SimClient/GuildApplicant.h"
#include "SimClient/Question.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "Version.h"
#include "SimClient/SellYuanBaoGroup.h"

#include <Reuben/Math/Hex.h>
#include <vector>

#define CHATMSG_TAG			_T("\\m")
#define RECONNECT_TIME_MIN	100
#define RECONNECT_TIME_MAX	40000
#define USER_CONNECT_TIME	30000
#define RESPOND_INTERVAL	10000
#define HASH_PASSWD			_T("64B138032C26059879DFFB7009979136A2D1ADC5785BED0C1DD29B5661DDBE43")

PROTO_LuaPush(CMap)
PROTO_LuaPush(CCharacter)
PROTO_LuaPush(CScriptSystem)
PROTO_LuaPush(CChargeShop)
PROTO_LuaPush(CTrading)
PROTO_LuaPush(CHawkGroup)
PROTO_LuaPush(CQuestSystem)
PROTO_LuaPush(CGuild)

RPCResult TestLock(RPCContext &context);

inline bool StrEqual(Char* &token, const Char* cmd)
{
	if (token == NULL || cmd == NULL)
		return false;

	if (_tcsicmp(token, cmd) == 0) {
		token = NULL;
		return true;
	} else {
		return false;
	}
}

CSimClient::CSimClient()
{
	m_pProxyPeer = NULL;
	m_pAccount = NULL;
	m_pCharacter = NULL;
	m_characters = NULL;
	m_pMap = NULL;
	m_pBag = NULL;
	m_pBattle = NULL;
	m_pBatChar = NULL;
	m_pPartner = NULL;
	m_pPartnerGroup = NULL;
	m_pSkillGroup = NULL;
	m_pQuestSys = NULL;
	m_pScriptSys = NULL;
	m_pMessengerGroup = NULL;
	//m_pMsgCharGroup = NULL;
	m_bRobot = false;
	m_lastMoveX = 0;
	m_lastMoveY = 0;
	m_login = LS_LOGOUT;
	m_reconnect = false;
	m_pLogger = NULL;
	m_lastActTime = 0;
	m_actionTime = gGlobal.m_robotInterval;
	m_bDelete = BOOLEAN_FALSE;
	m_resTestTime = 0;
	m_startTestTime = 0;
	m_lastRespond = 0;
	m_minRespond = UINT32_MAX_VALUE;
	m_maxRespond = 0;
	m_avgRespond = 0.0f;
	m_respondCount = 0;
	m_batDeadlock = 0;
	m_bTradReqCfm = FALSE;	
	m_pTrading = NULL;
	m_TarTradeID = 0;
	m_getTradReq = FALSE;
	m_btradelock = FALSE;	
	m_btarlock = FALSE;	
	m_pPetGroup = NULL;
	m_pshop = NULL;
	m_bShopOpen = FALSE;
	m_chShopItem.clear();	
	m_talkInProcess = FALSE;
	m_talkStep = 0;	
	m_pMyHawkGroup = NULL;	
	m_pTargetHawkGroup = NULL;	
	m_pLuaState = NULL;
	m_luaRef = 0;
	QuestJumped = FALSE;
	ZeroMemory(m_loginName, sizeof(m_loginName));
	ZeroMemory(m_pChannel, sizeof(m_pChannel));
	m_guildMap.clear();
	m_ownGuild = NULL;
	m_ownGuildMember = NULL;
	m_pQuestion = NULL;
	m_objId = GetID();
	m_context = RPCContext();
	m_pSellYuanBaoGroup = NULL;
}

CSimClient::~CSimClient()
{
	Activate();
	if (m_pLogger)
		gGlobal.SetLogger(m_pLogger);
	ProcedureManager::GetInstance().RemoveByObject(this);
	ProcedureManager::GetInstance().RemoveBySession(m_context.sessionId);
	DeleteObjects();
	if (m_pLuaState)
		gGlobal.ReleaseLuaThread(m_pLuaState, gGlobal.m_pLuaState, m_luaRef);

	if (m_pLogger != gGlobal.m_defaultLogger)
		SafeDeleteObject(m_pLogger);

	SafeRelease(m_pProxyPeer);
}

void CSimClient::DeleteObjects()
{
	//Activate();

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SimClient '%s' DeleteObjects ..."), m_loginName);

	if (m_pProxyPeer) {
		//LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();
		//rpcmgr.GetModel().CloseSocket(m_pProxyPeer->GetSocketID());
		m_pProxyPeer = NULL;
	}

	DeleteNetObject(m_pAccount);
	DeleteNetObject(m_pCharacter);
	//DeleteNetGroup(m_characters, BOOLEAN_TRUE);
	DeleteNetObject(m_pBag);
	DeleteNetObject(m_pMap);
	DeleteNetObject(m_pBattle);
	DeleteNetObject(m_pBatChar);
	DeleteNetObject(m_pPartner);
	DeleteNetGroup(m_pPartnerGroup, BOOLEAN_TRUE);
	DeleteNetGroup(m_pSkillGroup, BOOLEAN_TRUE);
	DeleteNetObject(m_pQuestSys);
	DeleteNetObject(m_pScriptSys);
	DeleteNetGroup(m_pMessengerGroup, BOOLEAN_TRUE);
	//DeleteNetGroup(m_pMsgCharGroup, BOOLEAN_TRUE);
	DeleteNetGroup(m_pPetGroup, BOOLEAN_TRUE);
	DeleteNetObject(m_pTrading);
	DeleteNetObject(m_pshop);		
	DeleteNetGroup(m_pMyHawkGroup, BOOLEAN_TRUE);	
	DeleteNetGroup(m_pTargetHawkGroup, BOOLEAN_TRUE);
	DeleteNetObject(m_pQuestion);

	if (m_pSellYuanBaoGroup)
		DeleteNetGroup(m_pSellYuanBaoGroup, BOOLEAN_TRUE);

	IniAfterDisconnect();		
	DeleteChannels();
	m_login = LS_LOGOUT;
	m_resTestTime = 0;
	m_batDeadlock = 0;

	// delete all global NetObjects
	LocalRPCManager::GetInstance().SetNetIdMap(NULL);
	LocalRPCManager::GetInstance().DeleteAll();
	// delete all own NetObjects
	LocalRPCManager::GetInstance().SetNetIdMap(&m_custIdMap);
	LocalRPCManager::GetInstance().DeleteAll();
	ProcedureManager::GetInstance().RemoveByObject(this);
	ProcedureManager::GetInstance().RemoveBySession(m_context.sessionId);
}

void CSimClient::Init()
{
}

void CSimClient::IniAfterDisconnect()
{
	////intialize trading variables
	m_TarTradeID = 0;
	m_getTradReq = FALSE;
	m_btradelock = FALSE;	
	m_btarlock = FALSE;
	m_bTradReqCfm = FALSE;

	////initialize online shop variables
	m_bShopOpen = FALSE;	
	while (m_chShopItem.size() > 0)
	{
		SellItemMap::iterator iter = m_chShopItem.begin(); 	
		CChargeShopItem *temp = iter->second;
		if (temp)
		{
			DeleteNetObject(temp);
		}
		m_chShopItem.erase(iter);
	}
	////initialize NPC talking parameters
	m_talkInProcess = FALSE;
	m_talkStep = 0;		
	////initialize the quest parameters
	QuestJumped = FALSE;
	//////initialize the guild variables

	for (GuildMap::iterator Iter = m_guildMap.begin(); Iter != m_guildMap.end(); ++Iter)
		DeleteNetObject(Iter->second);
	m_guildMap.clear();
	m_ownGuild = NULL;
	m_ownGuildMember = NULL;
}

void CSimClient::Activate()
{
	if (!gGlobal.m_bMulti)				// no need to switch if not multiple client
		return;

	gGlobal.m_pSimClient = this;		// switch current SimClient

	// change LocalRPCManager NetID map
	LocalRPCManager::GetInstance().SetNetIdMap(&m_custIdMap);

	// change global logger to that SimClient logger
	if (m_pLogger)
		gGlobal.SetLogger(m_pLogger);
	else
		gGlobal.SetLogger(gGlobal.m_defaultLogger);
}

void CSimClient::WaitExecute(PCSTRING cmd)
{
	ThreadGuard guard(m_key);
	m_commandList.push_back(cmd);
}

#pragma optimize("", off)		// turn off "if-else" optimization here
void CSimClient::Execute(PCSTRING cmd)
{
	if (cmd == NULL)
		return;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Execute %s ..."), cmd);
	size_t cmdLen = _tcslen(cmd);
	Char buf[COMMAND_LENGTH] = _T("");

	buf[COMMAND_LENGTH - 1] = '\0';
	if (cmdLen < COMMAND_LENGTH)
		_tcscpy(buf, cmd);
	else
		_tcsncpy(buf, cmd, COMMAND_LENGTH - 1);

	Char *delim = _T(", \t");
	Char *token = _tcstok(buf, delim);
	Char *arg1, *arg2, *arg3, *arg4, *arg5;

	if (token == NULL)
		return;

	/************************************************************************/
	/*                       command no need connection                     */
	/************************************************************************/
	///////////////////////////
	//    general command    //
	///////////////////////////
	else if (StrEqual(token, _T("exit")) || StrEqual(token, _T("quit")))
	{
		//if (m_pAccount)
		//{
		//	PROCEDURE_DO_METHOD(CSimClient, Logout, this, m_context)
		//}
		gGlobal.m_bExit = true;
	} 
	else if (StrEqual(token, _T("connect")))
	{
		ConnectServer();
	} 
	else if (StrEqual(token, _T("disconnect")))
	{
		m_reconnect = false;
		DisconnectServer();
	} 
	else if (StrEqual(token, _T("reconnect")))
	{
		m_reconnect = true;
		if (m_pProxyPeer)
			DisconnectServer();
		else
			ConnectServer();
	}
	else if (StrEqual(token, _T("switch")))
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) {
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SimClient %s: try to switch to SimClient %s"), 
				m_loginName, arg1);
			gGlobal.SwitchClient(arg1);
		}
	} 
	else if (StrEqual(token, _T("restart")))
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SimClient %s: try to restart SimClient"), 
			m_loginName);
		gGlobal.RestartClient(1000);
	} 
	else if (StrEqual(token, _T("option")))
	{
		arg1 = _tcstok(NULL, _T(""));
		if (arg1) {							// set/unset (+/-) option 
			gGlobal.ReadOption(arg1);
		} else {
			gGlobal.ShowOption();
		}
	}
	else if (token && (token[0] == _T('+') || token[0] == _T('-')))	// set/unset (+/-) option 
	{
		gGlobal.ReadOption(token);
		arg1 = _tcstok(NULL, _T(""));
		if (arg1)
			gGlobal.ReadOption(arg1);
		gGlobal.ShowOption();
	}
	else if (StrEqual(token, _T("loglevel")))
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) {
			int level = _ttoi(arg1);
			if (level >= 0 && level < 5) {
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SimClient %s: log level set to %d"), 
					m_loginName, level);
				GLOBAL_LOGGER.SetLevel((Reuben::Diagnostics::Logger::LevelEnum) level);
			}
		}
	}

	///////////////////////////
	//      Robot / Lua      //
	///////////////////////////
	else if (StrEqual(token, _T("robot")))
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Robot: %s"), arg1);
			if (StrEqual(arg1, _T("start")))
			{
				m_bRobot = true;
			}
			else if (StrEqual(arg1, _T("stop")))
			{
				m_bRobot = false;
			}
		}
	}
	else if (StrEqual(token, _T("lua")))
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			if (StrEqual(arg1, _T("load")))
			{
				arg2 = _tcstok(NULL, delim);
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Lua load script: %s"), arg2);
				String filename(_T("./"));
				filename += arg2;
				gGlobal.LoadRobotLua(filename.c_str());
			}
			else if (StrEqual(arg1, _T("func")))
			{
				arg2 = _tcstok(NULL, delim);
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Lua call function: %s"), arg2);
				StringA func = arg2;
				CallLuaFunc(m_pLuaState, func.c_str(), String());
			}
			else if (StrEqual(arg1, _T("func2")))
			{
				arg2 = _tcstok(NULL, delim);
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Lua call function: %s"), arg2);
				StringA func = arg2;
				CallLuaFunc(m_pLuaState, func.c_str(), String(),// robot action in Map
				m_pMap, m_pCharacter);
			}	
		}
	}
	else if (StrEqual(token, _T("login")))
	{
		if (m_pAccount) {
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Login Failed: account %s already logined"), 
				m_pAccount->GetLoginName().c_str());
			return;
		}
		Char* acctName = (Char*) m_accountName.c_str();
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (arg1)
			acctName = arg1;
		if (m_login == LS_LOGOUT)
		{
			if (m_pProxyPeer == NULL && !ConnectServer())
				return;
			if (m_pProxyPeer && acctName[0] != '\0')
			{
				if (!Reuben::Math::AES::KeyChanged())
				{
					String s;
					s.Format(_T("login %s"), acctName);
					WaitExecute(s.c_str());
					return;
				}
				gGlobal.m_pSimClient->m_login = LS_LOGINING;	
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Login to server: loginName %s"), 
					acctName);
				Size size = 32;
				ByteArray password(size);
				PCSTRING hashed_pw = HASH_PASSWD;
				Reuben::Math::HexToBytes(&password, size, hashed_pw, 64);
				//PROCEDURE_DO_METHOD2(CSimClient, Login, this, m_context, String, m_loginName, ByteArray, password)
				WorldService_Proxy::LoginAccount(m_pProxyPeer, m_context, 0, acctName,
					password);	
			}
		}
	}
	else if (StrEqual(token, _T("testlock")))
	{
		TestLock(m_context);
	}

	// stop here when command is executed (token == NULL), or no proxy peer
	if (token == NULL || m_pProxyPeer == NULL) return;

	/************************************************************************/
	/*                       command need connection                        */
	/************************************************************************/
	/////////////////////////////////
	//    account login command    //
	/////////////////////////////////
	else if (StrEqual(token, _T("logout")))
	{
		if (m_pAccount)
		{
			PROCEDURE_DO_METHOD(CAccount, Logout, m_pAccount, m_context)
		}
	}
	else if (StrEqual(token, _T("kick")))
	{
		if (m_login == LS_LOGINED) {
			arg1 = _tcstok(NULL, delim);
			bool bKick = !(StrEqual(arg1, _T("no")));
			if (!bKick)
				m_login = LS_LOGOUT;
			WorldService_Proxy::KickAccount(m_pProxyPeer, m_context, bKick);
		}
	}
	else if (StrEqual(token, _T("select")))
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (m_pAccount && arg1)
		{
			UInt32 char_id = _ttoi(arg1);
			UInt8 line_id = arg2 ? _ttoi(arg2) : 0;
			PROCEDURE_DO_METHOD2(CAccount, SelectCharacter, m_pAccount, m_context, 
								UInt32, char_id, UInt8, line_id)
		}
	}

	//////////////////////////////////////////////////
	//         Friend Command             //
    //////////////////////////////////////////////////
	else if (StrEqual(token, _T("friend")) && m_pCharacter && m_pMessengerGroup && m_pMap)
	{   
		arg1 = _tcstok(NULL, delim);// add or delete
        arg2 = _tcstok(NULL, delim);// target ID
		arg3 = _tcstok(NULL, delim);// group number
		arg4 = _tcstok(NULL, delim);// target name
		if (arg1)
		{
			if (StrEqual(arg1, _T("add_nearby"))) // add a friend nearby, or add any friend online
			{
				if (arg2 && arg3 && arg4)// if all information are provided for adding a friend
				{
					UInt32 targetId = _ttoi(arg2);
					UInt8 group = _ttoi(arg3); 
					 // if the friend, whom we want to add, 
					 // is currently near to player (roughly within the same view)
					CCharacter *pFriend = m_pMap->FindCharacter(targetId);
                    if (pFriend)
						MessageService_Proxy::AddFriend(targetId, group, arg4);
					else // if we cannot find the friend online.
						TRACE_INFODTL_1(GLOBAL_LOGGER, 
							_T("AddFriend: character not exist, char_id %d"), targetId);
				}
			}
			else if (StrEqual(arg1, _T("add_online")))
			{
				if (arg2 && arg3 && arg4) {
					UInt32 targetId = _ttoi(arg2);
					UInt8 group = _ttoi(arg3); 
					PROCEDURE_DO_FUNC2(MessageService_Proxy::SearchFriend, m_context, 
										UInt8, group, String, arg4)
				}
			}
			else if (StrEqual(arg1, _T("delete")))// delete a friend
			{
				if (arg2)
				{
					UInt32 targetId = _ttoi(arg2);
					MessageService_Proxy::RemoveFriend(targetId);
				}
			}
			else if (StrEqual(arg1, _T("send_msg"))) // send message to friends
			{
				if (arg2 && arg3 && arg4) {
					UInt32 targetId = _ttoi(arg2);
					bool bPrivate = StrEqual(arg3, _T("private"));
					MessageService_Proxy::SendFriendMsg(targetId, arg4, bPrivate);
				}
			}
			else if (StrEqual(arg1, _T("move_group"))) // send message to friends
			{
				UInt32 targetId = _ttoi(arg2);
				UInt8 group = _ttoi(arg3); 
				MessageService_Proxy::ChangeGroup(targetId, group);
			}
		}
		else // if the input is "friend" alone without any parameter, then show all the friends information. 
		{
			MessageService_Proxy::ShowFriends();
		}
  	}

	////////////////////////////////
	//      Giving Command        //    
	////////////////////////////////
	else if  (StrEqual(token, _T("give")) && m_pCharacter && m_pMap &&
				m_pBag && m_pBatChar == NULL)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (arg1 && arg2)
		{
			UInt32 targetId = _ttoi(arg2);
            CCharacter *pTarget = m_pMap->FindCharacter(targetId);
			if (pTarget) {// if the guy who receive the item is nearby
				TRACE_INFODTL_1(GLOBAL_LOGGER,_F("Given: no target nearby, char_id %d"),
					targetId);
				return;
			}
			UInt8Vector bagIndices, stackList;
			UInt32Vector parterIdList, petIdList;
			UInt32 value = 0;
			UInt32 money = 0;
			do {
				arg3 = _tcstok(NULL, delim);
				arg4 = _tcstok(NULL, delim);
				if (arg4 == NULL)
					break;
				value = _ttoi(arg4);
				if (_tcsicmp(arg3, _T("money")) == 0) {
					if (value <= m_pCharacter->GetMoney()) {
						money = value;
					} else {
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Given: no enough money %u"),
							value);
					}
				} else if (_tcsicmp(arg3, _T("item")) == 0) {
					TCHAR *p = _tcschr(arg4, _T('x'));
					UInt8 count = 1;
					if (p)
					{
						// set count by 'x'
						*p = '\0';
						count = (UInt8) _ttoi(p + 1);
						count = MAX(count, 1);
					}
					UInt8 bagIndex = (UInt8) _ttoi(arg4);
					CItem* pItem = m_pBag->GetItem(bagIndex);
					if (pItem == NULL) {
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Given: item not found, bagIndex %d"),
							value);
					} else if (pItem->IsBound()) {
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Given: item already bounded, bagIndex %d"),
							value);
					} else if (count <= pItem->GetStack()) {
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Given: excess item stack, stack %d"),
							count);
					} else {
						bagIndices.push_back(value);
						stackList.push_back(count);
					}
				} else if (_tcsicmp(arg3, _T("partner")) == 0) {
					CPartner* pPartner = m_pPartnerGroup->GetPartnerByIndex((UInt16) value);
					if (pPartner == NULL)
					{
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Given: partner not found, index %d"),
							value);
					}
					else if (pPartner->GetSoulboundState() == SOULBOUND_ALREADY_BOUND)
					{
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Given: partner already bounded, index %d"),
							value);
					}
					else
						parterIdList.push_back(pPartner->GetPartner_id());
				} else if (_tcsicmp(arg3, _T("pet")) == 0) {
					CPet* pPet = m_pPetGroup->GetPetByIndex((int) value);
					if (pPet == NULL)
					{
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Given: pet not found, index %d"),
							value);
					}
					else if (pPet->GetSoulbound() == SOULBOUND_ALREADY_BOUND)
					{
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Given: pet already bounded, index %d"),
							value);
					}
					else
						parterIdList.push_back(pPet->GetPet_id());
				}
			} while (arg3 && arg4);
			if (money != 0 || !bagIndices.empty() || !parterIdList.empty() ||
				!petIdList.empty())
				m_pCharacter->GivenProcess(pTarget->GetNetId(), bagIndices, stackList,
					parterIdList, petIdList, money);
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Given: no valid given object"));
		}
		else		// if just "give"
		{ 
			TRACE_INFODTL_1(GLOBAL_LOGGER,_F("You have: money $%d"),
				            m_pCharacter->GetMoney());	
			m_pBag->ShowItems(BOOLEAN_TRUE);
			m_pPartnerGroup->ShowAllPartner();
			m_pPetGroup->ShowAllPet();
		}
	}

	/////////////////////////////////////////////////
	//    Trading(with other players)  Command     //
	/////////////////////////////////////////////////
	else if (StrEqual(token, _T("trade")))
	{
		arg1 = _tcstok(NULL, delim);
		if ((m_pMap == NULL)||(m_pBag == NULL)||(m_pCharacter == NULL))
			return;

		if (m_pCharacter && m_pBatChar == NULL && arg1)
		{
            if (StrEqual(arg1, _T("request")))// raise trade request to another player
			{
                arg2 = _tcstok(NULL, delim);
				if (arg2)
				{
                    UInt32 targetID = _ttoi(arg2);
					CCharacter *targetChar = m_pMap->FindCharacter(targetID);
					if (targetChar)// if the trade player is nearby
					{
						if ((!m_bTradReqCfm)&&(!m_getTradReq))// if the trade request is not confirmed by opposite side, ask for trade
						{	
							m_TarTradeID = targetID;
							m_getTradReq = TRUE;
                            m_pCharacter->AskForTrading(m_pProxyPeer, m_context, targetChar->GetNetId());
							TRACE_INFODTL_1(GLOBAL_LOGGER, _F("A trade request is sent to character with ID:%d"), 
											targetID);	
						}
						else
							TRACE_INFODTL(GLOBAL_LOGGER, _F("Trade Request is already confirmed"));	
					}
					else
						TRACE_INFODTL(GLOBAL_LOGGER, _F("Your target trade person is not nearby or not online. Trading request failed"));
				}
				
			}
			else if (StrEqual(arg1, _T("accept")))// confirm yes or no to trade request
			{
				if (m_getTradReq)// if get the trade request from the opposite side
				{
					m_getTradReq = FALSE;
					CCharacter *targetChar = m_pMap->FindCharacter(m_TarTradeID);	
					if (targetChar)
					{
						m_pCharacter->ReplyReqTrading(m_pProxyPeer, m_context, TRUE, targetChar->GetNetId());
						TRACE_INFODTL(GLOBAL_LOGGER, _F("You accept a trade request"));	
					}
				}
			}
			else if (StrEqual(arg1, _T("reject")))
			{
				if (m_getTradReq)
				{
					m_getTradReq = FALSE;
					CCharacter *targetChar = m_pMap->FindCharacter(m_TarTradeID);
					if (targetChar)
					{
						m_pCharacter->ReplyReqTrading(m_pProxyPeer, m_context, FALSE, targetChar->GetNetId());
						TRACE_INFODTL(GLOBAL_LOGGER, _F("You reject a trade request"));	
					}
				}
			}
			else if (StrEqual(arg1, _T("cancel")))
			{
				if (m_bTradReqCfm)
				{
					if (m_pTrading)
					{
						m_pTrading->CancelTrading(m_pProxyPeer, m_context);
						TRACE_INFODTL(GLOBAL_LOGGER, _F("You cancel a trade request"));	
					}
				}
			}
			else if (StrEqual(arg1, _T("show")))
			{
				if (m_bTradReqCfm)
				{
					if (m_pTrading)
					{
						showSalesSlot();
					}
				}
			}
			else if (StrEqual(arg1, _T("add")))
			{
				Char *token = _tcstok(NULL, delim);
				if (m_pTrading == NULL)
					return;
				while ((token)&&(m_bTradReqCfm))
				{
					Char *num1 = _tcstok(NULL, delim);
					Char *num2 = _tcstok(NULL, delim);
					if (num2 == NULL)
						break;
                    UInt32 index = _ttoi(num1);// index
					UInt32 slot = _ttoi(num2);// trade slot

					if (StrEqual(token, _T("item")))
						AddRemoveTradItem(ITEM_TRADE, index, TRUE, slot);
					else if (StrEqual(token, _T("partner")))
						AddRemoveTradItem(PARTNER_TRADE, index, TRUE, slot);
					else if (StrEqual(token, _T("pet")))
						AddRemoveTradItem(PET_TRADE, index, TRUE, slot);
					else if (StrEqual(token, _T("money")))
						TransferMoney(index);
					else
						break;

					token = _tcstok(NULL, delim);
				}
				if (!m_bRobot)
				{
                    m_pTrading->SetTradingLock(m_pProxyPeer, m_context, FALSE);
					m_btradelock = FALSE;
				}

 			}
			else if (StrEqual(arg1, _T("remove")))
			{
				if (m_pTrading == NULL)
					return;	
				Char *token = _tcstok(NULL, delim);	
				while ((token)&&(m_bTradReqCfm))
				{
					UInt32 index = 0;
                    Char *num = _tcstok(NULL, delim);
					if (num)
					{
                        UInt32 slot = _ttoi(num);
						if (StrEqual(token, _T("item")))
							AddRemoveTradItem(ITEM_TRADE, index, FALSE, slot);
						else if (StrEqual(token, _T("partner")))
							AddRemoveTradItem(PARTNER_TRADE, index, FALSE, slot);
						else if (StrEqual(token, _T("pet")))
							AddRemoveTradItem(PET_TRADE, index, FALSE, slot);
						token = _tcstok(NULL, delim);
					}
				}
				if (!m_bRobot)
				{
                    m_pTrading->SetTradingLock(m_pProxyPeer, m_context, FALSE);	
					m_btradelock = FALSE;
				}
            
			}
			else if (StrEqual(arg1, _T("lock")))
			{
				if (m_pTrading == NULL)
					return;		
				// set lock
				if (m_bTradReqCfm)
				{
                    m_pTrading->SetTradingLock(m_pProxyPeer, m_context, TRUE);
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Trade is locked"));	
					m_btradelock = TRUE;	
				}

			}
			else if (StrEqual(arg1, _T("unlock")))
			{		//set unlock
				if (m_pTrading == NULL)
					return;		
				if (m_bTradReqCfm)
				{
                    m_pTrading->SetTradingLock(m_pProxyPeer, m_context, FALSE);
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Trade is unlocked"));	
					m_btradelock = FALSE;	
				}
			}

			else if (StrEqual(arg1, _T("confirm")))
			{
				if (m_pTrading == NULL)
					return;		
				if (m_btradelock && m_bTradReqCfm)
				{
                    m_pTrading->ConfirmTrading(m_pProxyPeer, m_context);
					m_btradelock = false;
					m_bTradReqCfm = false;
					TRACE_INFODTL(GLOBAL_LOGGER, _F("You confirm the trade"));		
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Confirm Failed. The opposite side is not locked"));	

			}
		}
	}

	//////////////////////////////
	//    online shop command   // 
	//////////////////////////////
	else if (StrEqual(token, _T("shop")))
	{
		arg1 = _tcstok(NULL, delim);
		if (m_pCharacter && m_pBatChar == NULL && arg1)
		{
			if (StrEqual(arg1, _T("open")))
			{
				if (!m_bShopOpen)
				{
                    m_bShopOpen = TRUE;	
					m_chShopItem.clear();	
					MapService_Proxy::OpenChargeShop(m_pProxyPeer, m_context);         
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The shop is already opened"));	

			}
			else if (StrEqual(arg1, _T("close")))
			{
				if (m_bShopOpen)
				{
					m_bShopOpen = FALSE;
					resetShopData();
					MapService_Proxy::CloseChargeShop(m_pProxyPeer, m_context); 

					if (m_pshop)
					{
						DeleteNetGroup(m_pshop, BOOLEAN_TRUE);
						m_pshop = NULL;
					}
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The shop is closed"));		
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The shop is already closed"));	
                             
			}
			else if (StrEqual(arg1, _T("buy")))
			{
				if (m_bShopOpen)
				{
					arg2 = _tcstok(NULL, delim);
					arg3 = _tcstok(NULL, delim);
					if (arg2 && arg3)
					{
						UInt16 pos = _ttoi(arg2);
						UInt16 stack = _ttoi(arg3);
						CChargeShopItem *selectItem = NULL;	
                        if (SelectShopItem(pos, selectItem))
						{
							UInt32 totalsum = stack * selectItem->GetYuanBao();
							if (!m_pCharacter->m_bIsBuyingChargeItem)
							{
								if ((m_pCharacter->GetYuanBao() > totalsum) || (m_pCharacter->GetYuanBao() == totalsum))
								{
									m_pshop->BuyItemInChargeShop(m_pProxyPeer, m_context, 
									selectItem->GetChargeShopID(), stack);
									m_pCharacter->m_bIsBuyingChargeItem = true;
								}
								else
								{
									if (m_bRobot)
									{
										CallLuaFunc(m_pLuaState, "CloseShop", String());
									}
									TRACE_INFODTL(GLOBAL_LOGGER, _F("Not enough money to buy"));	
						  
								}
							}
						}
						else
						{
							if (m_bRobot)
							{
                                 CallLuaFunc(m_pLuaState, "CloseShop", String());
							}
							TRACE_INFODTL(GLOBAL_LOGGER, _F("Selection of sales item failed. Perhaps position/item type is a invalid value"));
						 }
					}			
					else
					{
						if (m_bRobot)
						{
                           CallLuaFunc(m_pLuaState, "CloseShop", String());
						}
						TRACE_INFODTL(GLOBAL_LOGGER, _F("Invalid argument"));		
					}
				}
			}
			else if (StrEqual(arg1, _T("show")))
			{
				arg2 = _tcstok(NULL, delim);
				if ((arg2)&&m_bShopOpen)
				{
					  UInt32 itemType;
					  Char* cpytype = arg2;
					  if (GetSoldType(arg2, itemType))
					  {
						  int count = 0;
						  TRACE_INFODTL_1(GLOBAL_LOGGER, _F("The sale items under the category of %s"), cpytype);
						  for (SellItemMap::iterator iter = m_chShopItem.begin(); iter != m_chShopItem.end(); ++iter)
						  {
							  if (iter->first == itemType)
							  {
                                  CChargeShopItem *item = NULL;
								  item = iter->second;
								  if (item)
							      {
									 TRACE_INFODTL_2(GLOBAL_LOGGER, _F("item %d:, cost $%d"),
										 count, item->GetYuanBao());
								  }
							  }
							  ++count;
						  }
					  }
					  else
						  TRACE_INFODTL(GLOBAL_LOGGER, _F("Your specified type does not exist"));		
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Show failed. Shop is closed or incorrect argument"));	
				
			}
			else if (StrEqual(arg1, _T("money")))
			{
				if (m_bShopOpen)
				{
                    TRACE_INFODTL_1(GLOBAL_LOGGER, _F("You got $%d"), m_pCharacter->GetYuanBao());  
				}
				else
				{
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Show failed. Shop is closed"));	
				}
			}
		}
	}

	/////////////////////////////
	//     Pet Command         //
	/////////////////////////////
	else if (StrEqual(token, _T("pet")))
	{
		arg1 = _tcstok(NULL, delim);
		if ((m_pPetGroup == NULL)||(m_pCharacter == NULL))
			return;

		if (arg1 && m_pCharacter && m_pBatChar == NULL)
		{
			if (StrEqual(arg1, _T("show")))
			{
                bool ownPet = FALSE;	
				for (int i = 0; i <	m_pPetGroup->GetCount(); ++i)
				{
					CPet *pet = m_pPetGroup->GetChild<CPet>(i);
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Pet %d"), i);
					ownPet = TRUE;
				}
				if (!ownPet)
					TRACE_INFODTL(GLOBAL_LOGGER, _F("You got no pet"));
			}
			else if (StrEqual(arg1, _T("use")))
			{
				arg2 = _tcstok(NULL, delim);
                if (arg2)
				{
					UInt32 petUID = 0;
					UInt8 index = _ttoi(arg2);
					if (CheckPetState(index, petUID, PET_STATE_TYPE_REST))
					{
						m_pPetGroup->ChangePetState(m_pProxyPeer, m_context, petUID, PET_STATE_TYPE_USE);
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Pet %d is in use state"), index);	
					}
					else
						TRACE_INFODTL(GLOBAL_LOGGER, _F("Set Pet Use failed. No pet is found with specified index or the Pet is already in use."));
				}

			}
			else if (StrEqual(arg1, _T("standby")))
			{
				arg2 = _tcstok(NULL, delim);
                if (arg2)
				{
					UInt32 petUID = 0;
					UInt8 index = _ttoi(arg2);
					if (CheckPetState(index, petUID, PET_STATE_TYPE_USE))
					{
						m_pPetGroup->ChangePetState(m_pProxyPeer, m_context, petUID, PET_STATE_TYPE_REST);
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Pet %d is in standby mode"), index);	
					}
					else
						TRACE_INFODTL(GLOBAL_LOGGER, _F("Set Pet standby failed. No pet is found with specified index or the Pet is already standby."));
				}
			}
			else if (StrEqual(arg1, _T("remove")))
			{
                arg2 = _tcstok(NULL, delim);
                if (arg2)
				{
                    UInt8 index = _ttoi(arg2);
					UInt32 petUID = 0;
					if (CheckPetState(index, petUID, 88)) //value 88 means the check will be done regardless of
						                                  //pet state
					{
						m_pPetGroup->DeletePet(m_pProxyPeer, m_context, petUID);
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Pet %d is deleted by you"), index);	
					}
					else
						TRACE_INFODTL(GLOBAL_LOGGER, _F("Pet failed to be deleted. No pet is found with specified index."));	
				}
                
			}
			else if (StrEqual(arg1, _T("name_chg")))
			{
				arg2 = _tcstok(NULL, delim);  //index
				arg3 = _tcstok(NULL, delim);  //name
				if ((arg2)&&(arg3))
				{
					UInt8 index = _ttoi(arg2);
					String name;
					name.append(arg3);
                    CPet *pet = m_pPetGroup->GetChild<CPet>(index);
					if (pet)
					{
                        pet->ChangePetName(m_pProxyPeer, m_context, name);
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Pet %d with name changed"), index);
					}
					else
						TRACE_INFODTL(GLOBAL_LOGGER, _F("Pet name change failed. No pet is found with specified index."));		
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Incorrect Argument. Name Change failed"));			
			}
		}
	
	}
	/////////////////////////////
	//   Quest Command         //
	/////////////////////////////

	else if ((StrEqual(token, _T("quest"))))
	{
		arg1 = _tcstok(NULL, delim);
		if (!(arg1) || !(m_pCharacter) || m_pBatChar)
			return;
		if (StrEqual(arg1, _T("show")))
		{
			if (m_pQuestSys)
                m_pQuestSys->ShowQuestLog();
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("No quest system is loaded"));
		}
		else if (StrEqual(arg1, _T("cancel")))
		{
			arg2 =  _tcstok(NULL, delim);

			if (arg2)
			{
				UInt16 questID = (UInt16) _ttoi(arg2);

				if ((m_pQuestSys)&&(m_pQuestSys->CheckQuestValid(questID)))
				{
					m_pQuestSys->CancelQuest(m_pProxyPeer, m_context, questID);
					
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Quest Cancelation failed. Either specified quest ID is invalid or quest system is not loaded."));
			}
			else // randomly cancel a quest
			{
				if (!(gGlobal.m_pSimClient->m_pQuestSys))
					return;
				UInt32 NPCRandUID = gGlobal.m_pSimClient->m_pQuestSys->GetRandomNPCuid();
				if (NPCRandUID > 0)
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("The quest %d is canceled"), NPCRandUID);
					m_pQuestSys->CancelQuest(m_pProxyPeer, m_context, NPCRandUID);	
						
             	}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to random quest cancellation. You got no quest to cancel."));	
			}

		}
		else
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Quest Command failed. Invalid argument is input")); 
	}
	else if ((StrEqual(token, _T("journal"))) && m_pQuestSys)
	{
		m_pQuestSys->ShowJournals();
	}

	/////////////////////////////
	//   NPC Talking Command   //
	/////////////////////////////
	else if (StrEqual(token, _T("npctalk"))) 
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1 == NULL || m_pMap == NULL || m_pCharacter == NULL || 
			m_pBatChar || m_pScriptSys == NULL) 
			return;
		if (arg1[0] >= _T('0') && arg1[0] <= _T('9'))
		{
			////do the chating
			UInt8 option = (UInt8) _ttoi(&arg1[0]);
			m_pScriptSys->ReplyAnswer(m_pProxyPeer, m_context, option);
		}
		else if (StrEqual(arg1, _T("start"))) 
		{
			/////initize the chatting with NPC
			arg2 = _tcstok(NULL, delim);
			// talk to a NPC with specified UID
			if (arg2 && arg2[0] >= _T('0') && arg2[0] <= _T('9'))
			{
				UInt32 npcUID = _ttoi(arg2);
				CMapNPC* pNpc = m_pMap->FindNpc(npcUID);	
				if (pNpc)
				{
					//m_talkInProcess = TRUE;
					m_talkStep = 0;			
					m_pCharacter->TalkToNPC(m_pProxyPeer, m_context, npcUID);
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Talk failed. No specified NPC around"));
			}
			else // talk to a randomly selected NPC
			{
				UInt32Vector npcIds = m_pMap->FindNpcNearby();
				if (npcIds.size() > 0)
				{
					int i = GetRand((int)npcIds.size());
					//m_talkInProcess = TRUE;
					m_talkStep = 0;	
					m_pCharacter->TalkToNPC(m_pProxyPeer, m_context, npcIds[i]);
					TRACE_INFODTL_1(GLOBAL_LOGGER, 
						_T("Talk to a randomly selected NPC with uid %d"),
						npcIds[i]);	
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("No NPC nearby to talk to."));
			}
		}
		else
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Incorrect Argument. Talk failed"));		

	}
	else if (StrEqual(token, _T("answer"))) 
	{
		if (m_pScriptSys->IsAskString())
			arg1 = _tcstok(NULL, _T(""));
		else
			arg1 = _tcstok(NULL, delim);
		if (arg1 == NULL || m_pCharacter == NULL || m_pBatChar || m_pScriptSys == NULL) 
			return;
		if (m_pScriptSys->IsAskString()) {
			if (_tcsicmp(arg1, _T("cancel")) == 0)
				arg1 = _T("");
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Reply Answer String: %s"), arg1);
			m_pScriptSys->ClearAskString();
			m_pScriptSys->ReplyAnswerString(m_pProxyPeer, m_context, arg1);
		}
		else if (arg1[0] >= _T('0') && arg1[0] <= _T('9'))
		{
			////do the chating
			UInt8 option = (UInt8) _ttoi(&arg1[0]);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Reply Answer: index %d"), option);
			m_pScriptSys->ReplyAnswer(m_pProxyPeer, m_context, option);
		}
	}
	/////////////////////////////
	//   NPC Given Command   //
	/////////////////////////////
	else if (StrEqual(token, _T("npcgive"))) 
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (arg1 == NULL || arg2 == NULL || m_pCharacter == NULL || 
			m_pBatChar || m_pScriptSys == NULL) 
			return;
		if (StrEqual(arg1, _T("cancel"))) {
			m_pScriptSys->ReplyGiven();
			return;
		}
		do {
			UInt8 index = _ttoi(arg2);
			UInt8 gtype = 0;
			UInt32 gid = 0;
			UInt32 uid = 0;
			if (StrEqual(arg1, _T("item")) && m_pBag) {
				gtype = CScriptSystem::GIVEN_ITEM;
				CItem* pItem = m_pBag->GetItem(index);
				if (pItem) {
					gid = pItem->GetItem_id();
					uid = pItem->GetItem_uid();
				}
			} else if (StrEqual(arg1, _T("partner")) && m_pPartnerGroup) {
				gtype = CScriptSystem::GIVEN_PARTNER;
				CPartner* pPartner = m_pPartnerGroup->GetPartnerByIndex(index);
				if (pPartner) {
					gid = pPartner->GetMob_id();
					uid = pPartner->GetPartner_id();
				}
			} else if (StrEqual(arg1, _T("pet")) && m_pPetGroup) {
				gtype = CScriptSystem::GIVEN_PET;
				CPet* pPet = m_pPetGroup->GetPetByIndex(index);
				if (pPet) {
					gid = pPet->GetPet_id();
					uid = pPet->GetPet_uid();
				}
			} else {		// wrong input
				TRACE_INFODTL(GLOBAL_LOGGER, _F("NpcGive: wrong input argument"));
				break;
			}
			if (uid != 0) {
				if (!m_pScriptSys->AddGivenObject(gtype, gid, uid))
					break;
			}
			arg1 = _tcstok(NULL, delim);
			arg2 = _tcstok(NULL, delim);
		} while (arg2);

		if (m_pScriptSys->GetGivenCount() > 0)
			m_pScriptSys->ReplyGiven();
	}
	////////////////////////////////
	//         NPC Shop           //  
	////////////////////////////////
	else if (StrEqual(token, _T("npcshop")))
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (!(arg1) || !(m_pCharacter) || m_pBatChar || !(arg2) || m_pScriptSys == NULL || m_pBag == NULL) 
			return;
		if (StrEqual(arg1, _T("buy")))
		{
			arg3 = _tcstok(NULL, delim);
			if (!(arg3))
				return;
			UInt16 stack = (UInt16) _ttoi(arg3);  
			UInt8 buySlot = (UInt8) _ttoi(arg2);// the slot position of the sale item that you intend to buy.
			if ((gGlobal.m_resource == NULL)||(m_pScriptSys == NULL))
				return;
			const ItemData* pItemData = gGlobal.m_resource->GetItemData(m_pScriptSys->m_NPCShopItem[buySlot]);	
			if (pItemData)// if there is a corresponding sale item with the specified slot number
			{
				UInt32 sum = pItemData->g_price * stack * 
							m_pScriptSys->m_buyDiscount / 100;// total $$ needed
				if (m_pCharacter->GetMoney() >= sum) // if enough money
				{   //buy sale item
					gGlobal.m_pSimClient->m_pBag->BuyItemFromShop(m_pProxyPeer, m_context, 
						m_pScriptSys->m_NPCShopItem[buySlot],m_pScriptSys->m_NPCShop, buySlot, stack);	
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to buy. Not enough money."));	
			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to buy. Invalid item slot number"));
		}
		else if (StrEqual(arg1, _T("sell")))
		{
			UInt8 bagItemNo = (UInt8) _ttoi(arg2);
			CItem *item = gGlobal.m_pSimClient->m_pBag->GetItem(bagItemNo);	
			if (item)
			{
                gGlobal.m_pSimClient->m_pBag->SellItemToShop(m_pProxyPeer, m_context, m_pScriptSys->m_NPCShop,
					  bagItemNo, 1);
			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to sell. Invalid item slot number"));	
		}
	}


	/////////////////////////////
	//    character command    //
	/////////////////////////////
	else if (StrEqual(token, _T("move"))) {
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (m_pCharacter && m_pCharacter->IsLeader() && m_pBatChar == NULL && arg1)
		{
			if (StrEqual(arg1, _T("stop")))
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Character: Stop move"));
				m_pCharacter->StopMove(m_pProxyPeer, m_context);
			}
			else if (StrEqual(arg1, _T("again")))
			{
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Character: Move again: %d, %d"), 
					m_lastMoveX, m_lastMoveY);
				m_pCharacter->Move(m_pProxyPeer, m_context, m_lastMoveX, m_lastMoveY);
			}
			else if (arg1[0] == _T('g') && m_pMap && m_pMap->GetGuideCount() > 0)
			{
				int g = _tstoi(arg1+1) % m_pMap->GetGuideCount();
				BlockPos* pPos = m_pMap->GetGuidePoint(g);
				if (pPos)
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Character: Move guide point %d: %d, %d"), 
						g, pPos->PosX, pPos->PosY);
					m_lastMoveX = pPos->PosX;
					m_lastMoveY = pPos->PosY;
					m_pCharacter->Move(m_pProxyPeer, m_context, pPos->PosX, pPos->PosY);
				}
			}
			else if (arg1[0] == _T('j') && m_pMap && m_pMap->GetJumpCount() > 0)
			{
				int j = _tstoi(arg1+1) % m_pMap->GetJumpCount();
				JumpInfo* pInfo = m_pMap->GetJumpPoint(j);
				if (pInfo)
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Character: Move jump point %d: %d, %d"), 
						j, pInfo->jpPosX, pInfo->jpPosY);
					m_lastMoveX = pInfo->jpPosX;
					m_lastMoveY = pInfo->jpPosY;
					m_pCharacter->Move(m_pProxyPeer, m_context, pInfo->jpPosX, pInfo->jpPosY);
				}
			}
			else if (arg2)
			{
				UInt16 x = _tstoi(arg1);
				UInt16 y = _tstoi(arg2);
				m_lastMoveX = x;
				m_lastMoveY = y;
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Character: Move to: %d, %d"), x, y);
				m_pCharacter->Move(m_pProxyPeer, m_context, x, y);
			}
		}
	}
	else if (StrEqual(token, _T("monitor")))
	{
		arg1 = _tcstok(NULL, delim);
		if (StrEqual(arg1, _T("leave")))
		{
			if (m_pBattle && m_pCharacter && 
				m_pCharacter->GetAction() == CCharacter::ST_MONITOR)
			{
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Monitor Leave: battle %d,%d"), 
					m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId);
				m_pCharacter->LeaveWatching(m_pProxyPeer, m_context);
			}
		} else if (m_pCharacter && m_pMap && arg1) {
			UInt32 targetId = _ttoi(arg1);
			CCharacter* pchar = m_pMap->FindCharacter(targetId);
			if (pchar) {
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Monitor Battle: target char_id %d, action %d"),
					targetId, pchar->GetAction());
				m_pCharacter->MonitorBattle(m_pProxyPeer, m_context, targetId);
			}
		}
	}
	else if (StrEqual(token, _T("set_pk")))
	{
		arg1 = _tcstok(NULL, delim);
		if (m_pCharacter && arg1)
		{
			Boolean enablePK = (_ttoi(arg1) > 0);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Set Enable PK: %d"), enablePK);
			//m_pCharacter->SetEnablePK(m_pProxyPeer, m_context, enablePK);
		}
	}
	else if (StrEqual(token, _T("battle")))
	{
		arg1 = _tcstok(NULL, delim);
		if (StrEqual(arg1, _T("join")))
		{
			if (m_pBattle && m_pCharacter && 
				m_pCharacter->GetAction() == CCharacter::ST_MONITOR)
			{
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Battle Join: battle %d,%d"), 
					m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId);
				m_pCharacter->RequestJoinBattle(m_pProxyPeer, m_context, BOOLEAN_FALSE);
			}
		} else if (StrEqual(arg1, _T("pk"))) {
			arg2 = _tcstok(NULL, delim);
			if (m_pCharacter && m_pMap && arg2) 
			{
				UInt32 targetId = _ttoi(arg2);
				CCharacter* pchar = m_pMap->FindCharacter(targetId);
				if (pchar && pchar->GetAction() != CCharacter::ST_BATTLE)
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("PK Battle: target char_id %d"),
						targetId);
					m_pCharacter->PKRequest(m_pProxyPeer, m_context, targetId, PK_BATTLE);
				}
			}
		} else if (m_pCharacter) {
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Test Battle Start:"));
			m_pCharacter->TestBattle(m_pProxyPeer, m_context);
		}
	}
	else if (StrEqual(token, _T("levelup")))
	{
		if (m_pCharacter)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Level Up:"));
			m_pCharacter->LevelUp(m_pProxyPeer, m_context);
		}
	}
	else if (StrEqual(token, _T("ap_add")))
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);
		arg4 = _tcstok(NULL, delim);
		arg5 = _tcstok(NULL, delim);
		if (m_pCharacter && arg1 && arg2 && arg3 && arg4 && arg5)
		{
			UInt16 sta = _ttoi(arg1);
			UInt16 spr = _ttoi(arg2);
			UInt16 str = _ttoi(arg3);
			UInt16 con = _ttoi(arg4);
			UInt16 agi = _ttoi(arg5);
			TRACE_INFODTL_5(GLOBAL_LOGGER, _F("Attributes Add: %d, %d, %d, %d, %d"),
				sta, spr, str, con, agi);
			m_pCharacter->RaiseAttrib(m_pProxyPeer, m_context, sta, spr, str, con, agi);
		}
	}


	////////////////////////////
	//    map item command    //
	////////////////////////////
	else if (StrEqual(token, _T("equip")))
	{			// equip item
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (m_pCharacter && m_pBag && 
			m_pBatChar == NULL && arg1)
		{
			UInt8 bagIndex = _tstoi(arg1);
			UInt8 place = (arg2 ? _tstoi(arg2) : 255);
			if (bagIndex < 120)
			{
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Character: Equip item index %d on place %d"), bagIndex, place);
			}
			else
			{
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Character: Unequip item index %d and put at %d"), bagIndex, place);
			}
			m_pBag->EquipItem(m_pProxyPeer, m_context, bagIndex, place);
		}
	}
	else if (StrEqual(token, _T("put")))
	{
		// put item
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (m_pCharacter && m_pBag && 
			m_pBatChar == NULL && arg1 && arg2)
		{
			UInt8 srcIdx = _tstoi(arg1);
			UInt8 dstIdx = _tstoi(arg2);
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Character: put item from index %d to %d"), srcIdx , dstIdx);
			m_pBag->MoveItem(m_pProxyPeer, m_context, srcIdx, dstIdx);
		}
	}
	else if (StrEqual(token, _T("drop")))
	{
		// drop item
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (m_pCharacter && m_pBag && 
			m_pBatChar == NULL && arg1) 
		{
			UInt8 bagIndex = _tstoi(arg1);
			UInt8 count = (arg2 ? _tstoi(arg2) : 0);
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Character: drop item index %d with count %d"), bagIndex, count);
			m_pBag->DropItem(m_pProxyPeer, m_context, bagIndex, count);
		}
	}
	else if (StrEqual(token, _T("bag")))
	{
		// show bag
		if (m_pCharacter && m_pBag)
		{
			m_pBag->ShowItems();
		}
	}
	
	//////////////////////////
	//    battle command    //
	//////////////////////////
	else if (StrEqual(token, _T("attack")))
	{
		arg1 = _tcstok(NULL, delim);
		if (m_pBatChar && m_pBatChar->GetState() == CHAR_ST_NORM && arg1)
		{
			UInt8 i = _tstoi(arg1);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Battle Command: Attack %d"), i);
			m_pBatChar->SendCommand(m_pProxyPeer, m_context, BATCMD_ATTACK, i, 0, 0);
		}
	}
	else if (StrEqual(token, _T("skill")))
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (m_pBatChar && m_pBatChar->GetState() == CHAR_ST_NORM && arg1 && arg2)
		{
			UInt32 skillId = _tstoi(arg1);
			UInt8 target = _tstoi(arg2);
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Battle Command: Use skill %d on target %d"), 
				skillId, target);
			m_pBatChar->SendCommand(m_pProxyPeer, m_context, BATCMD_SKILL, target, skillId, 0);
		}
		else if (m_pCharacter && m_pSkillGroup)
		{
			if (arg1 == NULL)
				m_pSkillGroup->ShowSkills();
		}
	}
	else if (StrEqual(token, _T("use")))
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (m_pBatChar && m_pBatChar->GetState() == CHAR_ST_NORM && arg1)
		{
			if (arg2 == NULL) {
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Battle Command: missing target to use item index %d"), arg1);
			} else {
				UInt32 bagIndex = _tstoi(arg1);
				UInt8 target = _tstoi(arg2);
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Battle Command: Use item index %d on target %d"), bagIndex, target);
				m_pBatChar->SendCommand(m_pProxyPeer, m_context, BATCMD_ITEM, target, bagIndex, 0);
			}
		}
		else if (m_pCharacter && m_pBag && arg1)
		{
			UInt32 bagIndex = _tstoi(arg1);
			Boolean bPartner = (arg2 != NULL ? _tstoi(arg2) > 0 : BOOLEAN_FALSE);
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Character: Use item index %d (partner %d)"), bagIndex, bPartner);
			
			UInt32 tempid = m_pBag->GetSlotItemId(bagIndex);

			CItem *pitem = m_pBag->GetItem(bagIndex);
			const ItemData* pItemData = NULL;	
			if (pitem)
                pItemData = pitem->m_data;
			Boolean itemScript = BOOLEAN_FALSE;
			if ( pItemData && pItemData->script && (_tcsicmp(pItemData->script, _T("")) != 0) )
				itemScript = BOOLEAN_TRUE;

			m_pBag->UseItem(m_pProxyPeer, m_context, bagIndex, tempid, itemScript, bPartner, 0);
		}
	}
	else if (StrEqual(token, _T("defend")))
	{
		if (m_pBatChar && m_pBatChar->GetState() == CHAR_ST_NORM)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Battle Command: Defend"));
			m_pBatChar->SendCommand(m_pProxyPeer, m_context, BATCMD_DEFEND, 0, 0, 0);
		}
	}
	else if (StrEqual(token, _T("protect")))
	{
		arg1 = _tcstok(NULL, delim);
		if (m_pBatChar && m_pBatChar->GetState() == CHAR_ST_NORM && arg1)
		{
			UInt8 i = _tstoi(arg1);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Battle Command: Protect %d"), i);
			m_pBatChar->SendCommand(m_pProxyPeer, m_context, BATCMD_PROTECT, i, 0, 0);
		}
	}
	else if (StrEqual(token, _T("auto")))
	{
		if (m_pBatChar && m_pBatChar->GetState() == CHAR_ST_NORM)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Battle Command: Auto Play"));
			m_pBatChar->SendCommand(m_pProxyPeer, m_context, BATCMD_AUTO, 0, 0, 0);
		} else if (m_pBatChar == NULL && m_pCharacter) {
			arg1 = _tcstok(NULL, delim);
			if (StrEqual(arg1, _T("off"))) {
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Character Command: Cancel Auto"));
				m_pCharacter->CancelAuto(m_pProxyPeer, m_context);
			}
		}
	}
	else if (StrEqual(token, _T("escape")))
	{
		if (m_pBatChar && m_pBatChar->GetState() == CHAR_ST_NORM)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Battle Command: Escape"));
			m_pBatChar->SendCommand(m_pProxyPeer, m_context, BATCMD_ESCAPE, 0, 0, 0);
		}
	}
	else if (StrEqual(token, _T("repeat")))
	{
		if (m_pBatChar && m_pBatChar->GetState() == CHAR_ST_NORM)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Battle Command: Repeat command"));
			m_pBatChar->SendCommand(m_pProxyPeer, m_context, BATCMD_REPEAT, 0, 0, 0);
		}
	} 
	else if (StrEqual(token, _T("catch")))
	{
		arg1 = _tcstok(NULL, delim);
		if (m_pBatChar && m_pBatChar->GetState() == CHAR_ST_NORM)
		{
			UInt8 i = _tstoi(arg1);
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Battle Command: Catch command"));
			m_pBatChar->SendCommand(m_pProxyPeer, m_context, BATCMD_CATCH, i, 0, 0);
		}
	} 
	else if (StrEqual(token, _T("call")))
	{
		arg1 = _tcstok(NULL, delim);
		if (m_pBatChar && m_pBatChar->GetState() == CHAR_ST_NORM)
		{
			UInt8 i = _tstoi(arg1);
			CPartner* partner = m_pPartnerGroup->GetChild<CPartner>(i);
			if (partner && partner->GetLevel() < (m_pBatChar->GetLevel() + 10) )
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Battle Command: Call Friend command"));
				m_pBatChar->SendCommand(m_pProxyPeer, m_context, BATCMD_FRIEND, 0, partner->GetPartner_id(), 0);
			}
			else
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Battle Command: Call Friend command fail: no such partner or partner level higher than character level + 10."));
			}
		}
	} 
	else if (StrEqual(token, _T("help")))
	{
		if (m_pBatChar && m_pBatChar->GetState() == CHAR_ST_NORM)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Battle Command: Help command"));
			m_pBatChar->SendCommand(m_pProxyPeer, m_context, BATCMD_HELP, 0, 0, 0);
		}
	} 
	else if (StrEqual(token, _T("p_attack")))
	{
		arg1 = _tcstok(NULL, delim);
		
		if (m_pPartner && m_pPartner->GetState() == CHAR_ST_NORM && arg1)
		{
			UInt8 i = _tstoi(arg1);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Battle Command: Partner Attack %d"), i);
			m_pPartner->SendCommand(m_pProxyPeer, m_context, BATCMD_ATTACK, i, 0, 0);
		}
	}
	else if (StrEqual(token, _T("p_skill")))
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		
		if (m_pPartner && m_pPartner->GetState() == CHAR_ST_NORM && arg1 && arg2)
		{
			UInt16 skill_id = _tstoi(arg1);
			UInt8 target = _tstoi(arg2);
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Battle Command: Partner Skill %d on Target %d"),
				skill_id, target);
			m_pPartner->SendCommand(m_pProxyPeer, m_context, BATCMD_SKILL, target, skill_id, 0);
		}
	}
	else if (StrEqual(token, _T("p_defend")))
	{
		if (m_pPartner && m_pPartner->GetState() == CHAR_ST_NORM)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Battle Command: Partner Defend "));
			m_pPartner->SendCommand(m_pProxyPeer, m_context, BATCMD_DEFEND, 0, 0, 0);
		}
	}
	else if (StrEqual(token, _T("p_protect")))
	{
		arg1 = _tcstok(NULL, delim);
		if (m_pPartner && m_pPartner->GetState() == CHAR_ST_NORM && arg1)
		{
			UInt8 i = _tstoi(arg1);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Battle Command: Partner Protect %d"), i);
			m_pPartner->SendCommand(m_pProxyPeer, m_context, BATCMD_PROTECT, i, 0, 0);
		}
	}
	else if (StrEqual(token, _T("p_escape")))
	{
		if (m_pPartner && m_pPartner->GetState() == CHAR_ST_NORM)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Battle Command: Partner Escape"));
			m_pPartner->SendCommand(m_pProxyPeer, m_context, BATCMD_ESCAPE, 0, 0, 0);
		}
	}
	
	////////////////////////
	//    chat command    //
	////////////////////////
	else if (StrEqual(token, _T("whisper")))
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (m_pCharacter && arg1 && arg2)
		{
			UInt32 targetId = _ttoi(arg1);
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Whisper %s: %s"), arg1, arg2);
			if (!gGlobal.m_msgList.empty() && _tcsnicmp(arg2, CHATMSG_TAG, 2) == 0)
			{
				int idx = _ttoi(arg2 + 2);
				if (idx >= 0)
					arg2 = (Char *) gGlobal.m_msgList[idx % (int) gGlobal.m_msgList.size()].c_str();
			}
			if (targetId != 0)				// whisper target by ID
				MessageService_Proxy::SendPrivateMessage(m_pProxyPeer, m_context, 
					arg2, m_pCharacter->GetChar_id(), targetId, String(), false);
			else							// whisper target by Name
				MessageService_Proxy::SendPrivateMessage(m_pProxyPeer, m_context, 
					arg2, m_pCharacter->GetChar_id(), 0, arg1, false);
		}
	}
	else if (StrEqual(token, _T("say")))
	{
		arg1 = _tcstok(NULL, delim);
		if (m_pCharacter && m_pChannel && arg1)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("say: %s"), arg1);
			if (!gGlobal.m_msgList.empty() && _tcsnicmp(arg1, CHATMSG_TAG, 2) == 0)
			{
				int idx = _ttoi(arg1 + 2);
				if (idx >= 0)
					arg1 = (Char *) gGlobal.m_msgList[idx % (int) gGlobal.m_msgList.size()].c_str();
			}
			//if (m_pChannel[CHANNELTYPE_MAP])	// say to map channel
			//	m_pChannel[CHANNELTYPE_MAP]->SendGrpMessage(m_pProxyPeer, 
			//		m_context, arg1, m_pCharacter->GetChar_id());
			if (m_pCharacter)
				MapService_Proxy::SendMapMessage(m_pProxyPeer, m_context, m_pCharacter->GetNetId(), arg1);
			else if (m_pChannel[CHANNELTYPE_GLOBAL])	// say to global channel
				m_pChannel[CHANNELTYPE_GLOBAL]->SendGrpMessage(m_pProxyPeer, 
					m_context, arg1, m_pCharacter->GetChar_id(), BOOLEAN_FALSE);
		}
	} 
	
	/////////////////////////
	//    party command    //
	/////////////////////////
	else if (StrEqual(token, _T("party")))
	{
		if (!m_pCharacter)
			return;

		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (arg1 == NULL)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Party: missing party command"));
			return;
		}
		UInt32 char_id = (arg2 ? _ttoi(arg2) : 0);
		if (m_pCharacter->m_pParty == NULL)
		{
			// party command when haven't party
			if (StrEqual(arg1, _T("create")))
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Party Create:"));
				m_pCharacter->CreateParty(m_pProxyPeer, m_context);
			}
			else if (StrEqual(arg1, _T("join")) && arg2)
			{
				//PROCEDURE_DO_METHOD1(CSimClient, JoinParty, this, m_context, UInt32, char_id)
				CCharacter* pchar = m_pMap->FindCharacter(char_id);
				if (pchar)
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Party Join: char_id %d"), char_id);
					m_pCharacter->PartyJoinRequest(m_pProxyPeer, m_context, pchar->GetNetId());
				} else {
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Party Join: char id %d not found"), char_id);
				}
			}
			else
			{
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Party: invalid command \"%s\" when you haven't party"), arg1);
			}
		}
		else
		{			// party command when have party
			CParty *pParty = m_pCharacter->m_pParty;
			if (StrEqual(arg1, _T("invite")) && arg2)
			{
				//PROCEDURE_DO_METHOD1(CSimClient, InviteParty, this, m_context, UInt32, char_id)
				CCharacter* pchar = m_pMap->FindCharacter(char_id);
				if (pchar)
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Party Invite: char_id %d"), char_id);
					m_pCharacter->PartyInviteRequest(m_pProxyPeer, m_context, pchar->GetNetId());
				} else {
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Party Invite: char id %d not found"), char_id);
				}
			}
			else if (StrEqual(arg1, _T("kick")) && arg2)
			{
				CCharacter *target = pParty->FindMember(char_id);
				if (target)
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Party Kick: char_id %d"), char_id);
					pParty->Kick(m_pProxyPeer, m_context, target->GetNetId());
				}
				else
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Party Kick Error: char id %d not found"), char_id);
			}
			else if (StrEqual(arg1, _T("promote")) && arg2)
			{
				CCharacter *target = pParty->FindMember(char_id);
				if (target)
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Party Promote: char_id %d"), char_id);
					pParty->Promote(m_pProxyPeer, m_context, target->GetNetId());
				}
				else
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Party Promote Error: char id %d not found"), char_id);
			}
			else if (StrEqual(arg1, _T("leave")))
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Party Leave:"));
				pParty->Leave(m_pProxyPeer, m_context);
			}
			else if (StrEqual(arg1, _T("status")))
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Party Status:"));
				pParty->ShowMember();
			}
			else
			{
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Party: invalid command \"%s\" when you already have party"), arg1);
			}
		}
	} 

	//////////////////////////////
	///		partner command		//
	//////////////////////////////
	else if (StrEqual(token, _T("partner")))
	{
		arg1 = _tcstok(NULL, delim);

		if (!m_pPartnerGroup || m_pPartnerGroup->GetCount() == 0)
			return;

		UInt8 index = 255;
		if (arg1 == NULL) {
			m_pPartnerGroup->ShowAllPartner();
			if (m_pCharacter && m_pCharacter->GetPartner_id() > 0) {
				index = m_pCharacter->GetPartner_id();
				CPartner *partner = m_pPartnerGroup->GetPartner(m_pCharacter->GetPartner_id());
				if (partner)
					partner->ShowDetail();
			}
			return;
		}

		if (!(arg1[0] >= _T('0') && arg1[0] <= _T('9'))) {
			if (index == 255) {
				TRACE_INFODTL_1(GLOBAL_LOGGER, 
					_T("Partner: wrong or missing partner index: %s"), arg1);
				return;
			}
			arg2 = arg1;
		} else {
			index = (arg1 ? _ttoi(arg1) : 0);
			arg2 = _tcstok(NULL, delim);
		}

		CPartner* partner = m_pPartnerGroup->GetChild<CPartner>(index);
		if (partner == NULL) {
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Partner: index %d not found in partner count %d"), 
				index, m_pPartnerGroup->GetCount());
			return;
		}

		if (arg2 == NULL) {
			partner->ShowDetail();
		} else if (StrEqual(arg2, _T("follow"))) {
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Partner Follow: partner index %d"), index);
			m_pCharacter->SetFollowPartner(m_pProxyPeer, m_context, partner->GetPartner_id());
		}
		else if (StrEqual(arg2, _T("stop")))
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Partner ClearFollow: partner index %d"), index);
			m_pCharacter->SetFollowPartner(m_pProxyPeer, m_context, 0);
		} 
		else if (StrEqual(arg2, _T("fight")))
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Partner SetFight: partner index %d"), index);
			if (partner->GetState() != 1)
				m_pPartnerGroup->ChangePartnerState(m_pProxyPeer, m_context, 
					partner->GetPartner_id(), 1);
			else
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Partner SetFight: partner %d is already in fighting state"), index);
		}
		else if (StrEqual(arg2, _T("standby")))
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Partner SetStandBy: partner index %d"), index);
			if (partner->GetState() != 0)
				partner->ChangePartnerState(m_pProxyPeer, m_context, 0);
			else
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Partner SetStandby: partner %d is already in standby state"), index);
		}
		else if (StrEqual(arg2, _T("delete")))
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Partner Delete: partner index %d"), index);
			m_pPartnerGroup->DeletePartner(m_pProxyPeer, m_context, partner->GetPartner_id());
		}
		else if (StrEqual(arg2, _T("attr_chg"))) /// add or remove the partner attribute 
		{
			bool wrongArg = FALSE;
			Char *word = _tcstok(NULL, delim);
			while (word)
			{
				if (!ReadOutAttrValue(word, partner))// read each command and check its validity
				{   // if the input argument is incorrect
					wrongArg = TRUE;
					break;
				}
                word = _tcstok(NULL, delim);	          
			}
			if ((!wrongArg) && ConfirmAttr(partner))
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Attribute is successfully changed"));
			}
			else
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("FAILED: No change is made to partners' attribute"));	
			}
			ResetAttri(partner); 

		}
		else if (StrEqual(arg2, _T("attr_show")))
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("The attribute point is %d"), partner->GetAttrPoint());
		}


	}

	/////////////////////////////////////
	//         Hawker Command          //
	/////////////////////////////////////
	else if (StrEqual(token, _T("hawker")))
	{
		if ((m_pCharacter == NULL)||(m_pMap == NULL)||(m_pBatChar))
			return;
		int status = m_pCharacter->GetAction();
		switch (status)
		{
            case CMapActor::ST_ENTER_MAP:
			case CMapActor::ST_LEAVE_MAP:
			case CMapActor::ST_JUMP_MAP:
			case CMapActor::ST_BATTLE:
			case CMapActor::ST_MOVE:
				return;
				break;
		}

//      check whether players is in Hawker Zone
	//  if (!(m_pMap->IsHawkZone(m_pCharacter->GetCurPosX(), m_pCharacter->GetCurPosY())))
//			return;

        arg1 = _tcstok(NULL, delim);
		if (StrEqual(arg1, _T("open"))) // start your own hawking business (so you are hawker)
		{
			if (m_pMyHawkGroup)
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("The hawking business is already open"));
				return;
			}	
			Char *word;
			String hawkName;
			arg2 = _tcstok(NULL, delim);
			arg3 = _tcstok(NULL, delim);
            
			if ((arg2 == NULL)||(arg3 == NULL))
				return;
			else
			{
				word = arg3;
				hawkName.append(arg2);	
			}
			int count = 0;
			int incorrect = 0;
			HawkerItemList itemlist, partnerlist, petlist;
			itemlist.clear(); partnerlist.clear(); petlist.clear();
            while (word)
			{
				if (!ReadOutHawker(word, itemlist, partnerlist, petlist))
					++incorrect;
                ++count;
				word = _tcstok(NULL, delim);	
            }
            if (count == incorrect)
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to open a hawking business because of invalid argument"));
			}
			else
			{
				//check if the level less then 30 minutes, failed to open hawk business 	
				//check if the hawker time is equal to 0
				if (m_pCharacter->GetLevel() < 30)
				{
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to open a hawking business because of level < 30"));
				}
				else
				{
					if (m_pCharacter->GetHawkerTime() == 0)
					{ //if hawker time equal to 0, add time to 60 minutes
						TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to open a hawking business because your hawker time = 0"));		
					}
					else
                        m_pCharacter->StartHawk(m_pProxyPeer, m_context, hawkName, itemlist, partnerlist, petlist); 
				}

			}
 
		}
		else if (StrEqual(arg1, _T("close")))// close the hawking business owned by you (you, a hawker, packed up)
		{
			if (m_pMyHawkGroup == NULL)
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("You haven't opened hawking business so you can't close it"));
				return;
			}
			m_pMyHawkGroup->StopHawk(m_pProxyPeer, m_context);

		}
		else if (StrEqual(arg1, _T("start_shop"))) // deal and shop with a specified hawker (so you are a customer)
		{
			if (m_pTargetHawkGroup)
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("You already got a target hawker!"));
				return;
			}

			Char* arg2 = _tcstok(NULL, delim);
			if ((m_pMap == NULL)||(arg2 == NULL))
				return;	
			
			UInt32 hawkerID = (UInt32) _ttoi(arg2);// the character ID of a hawker		
			CCharacter *hawker = m_pMap->FindCharacter(hawkerID);
			if (hawker)
			{
				if (hawker->GetAction() == CMapActor::ST_HAWK)
                    m_pCharacter->GetHawkFromTarget(m_pProxyPeer, m_context, hawkerID);
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to shop with a hawker: The target character haven't open hawking business"));	

			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to get a target hawker because the specified hawker ID doesn't exist or invalid"));	

		}
		else if (StrEqual(arg1, _T("stop_shop"))) // you, a customer, stop to shop with a specified hawker 
		{
			if (m_pTargetHawkGroup == NULL)
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to stop to deal with a hawker because you don't even get the hawker object"));	
				return;
			}

			m_pTargetHawkGroup->CloseHawkForUser(m_pProxyPeer, m_context, m_pCharacter->GetChar_id());
            
		}
		else if (StrEqual(arg1, _T("buy")))  // you, a customer, buy things provided by the hawker 
		{
			if (m_pTargetHawkGroup == NULL)
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to buy because you don't get a target hawker"));
				return;
			}
			Char* arg2 = _tcstok(NULL, delim);
			Char* arg3 = _tcstok(NULL, delim);
			Char* arg4 = _tcstok(NULL, delim);

			if (!(arg2 && arg3 && arg4))
				return;

			UInt8 itemtype = (UInt8) _ttoi(arg2); //1st input: the item type
			UInt8 itemBagID = (UInt8) _ttoi(arg3); //2nd input: the slot number of item/partner/pet
			UInt8 stack = (UInt8) _ttoi(arg4); //3rd input: the stack
	
			bool buySucess = FALSE;
			if (itemtype == 0) // hawker item
			{

				HawkItemMap itemMap = m_pTargetHawkGroup->GetItemList();

                int count = 0;
				for (HawkItemMap::iterator itr = itemMap.begin(); itr != itemMap.end(); ++itr)
				{
					if (itemBagID == count)
					{
                        if (itr->second->GetItem_stack() < stack)
						{
							TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to buy from a hawker: The specified stack is greater than the qty that offered by hawker."));
							return;
						}
						else
						{
							UInt32 cost = stack * itr->second->GetItem_price();
							if (m_pCharacter->GetMoney() < cost)
							{
								TRACE_INFODTL(GLOBAL_LOGGER, _F("You don't have enough money to buy"));
							}
							else
							{
								m_pTargetHawkGroup->BuyHawkItem(m_pProxyPeer, m_context, itr->first, 0, stack);
								buySucess = TRUE;
								break;
							}
						}

					}
					++count;
				}
			}
			else if (itemtype == 1) // hawker partner
			{
				HawkItemMap itemMap = m_pTargetHawkGroup->GetPartnerList();

                int count = 0;
				for (HawkItemMap::iterator itr = itemMap.begin(); itr != itemMap.end(); ++itr)
				{
					if (itemBagID == count)
					{
						UInt32 cost = itr->second->GetItem_price();
						if (m_pCharacter->GetMoney() < cost)
						{
							TRACE_INFODTL(GLOBAL_LOGGER, _F("You don't have enough money to buy"));
						}
						else
						{
							m_pTargetHawkGroup->BuyHawkItem(m_pProxyPeer, m_context, itr->first, 1, 1);
							buySucess = TRUE;
							break;
						}
					}
					++count;	
				}

			}
			else if (itemtype == 2) // hawker pet
			{
				HawkItemMap itemMap = m_pTargetHawkGroup->GetPetList();

                int count = 0;
				for (HawkItemMap::iterator itr = itemMap.begin(); itr != itemMap.end(); ++itr)
				{
					if (itemBagID == count)
					{
						UInt32 cost = itr->second->GetItem_price();
						if (m_pCharacter->GetMoney() < cost)
						{
							TRACE_INFODTL(GLOBAL_LOGGER, _F("You don't have enough money to buy"));
						}
						else
						{
							m_pTargetHawkGroup->BuyHawkItem(m_pProxyPeer, m_context, itr->first, 2, 1);
							buySucess = TRUE;
							break;
						}
					}
					++count;	
				}
			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to trade because of incorrect item type"));	

			if (!buySucess)
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to trade because of incorrect item ID"));
				if (gGlobal.m_pSimClient->m_bRobot)// if robot, stop to shop
					CallLuaFunc(m_pLuaState, "StopShopOverHawker", String());		
			}
                    
		}
		else if (StrEqual(arg1, _T("display_sale")))
		{
			if (m_pMyHawkGroup)
			{
				HawkItemMap itemlist = m_pMyHawkGroup->GetItemList();
				if (itemlist.size() > 0)
				{
					int count = 0;
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The items that I put in hawking for sale"));	
					for (HawkItemMap::iterator itr = itemlist.begin(); itr != itemlist.end(); ++itr)
					{
						TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Item Slot: %d  Item ID: %d Stack:%d"), count, 
							                      itr->second->GetItem_uid(), itr->second->GetItem_stack());
						++count;
					}
				}
				itemlist = m_pMyHawkGroup->GetPartnerList();	
				if (itemlist.size() > 0)
				{
					int count = 0;
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The partners that I put in hawking for sale"));	
					for (HawkItemMap::iterator itr = itemlist.begin(); itr != itemlist.end(); ++itr)
					{
						TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Partner Slot: %d  Partner ID: %d Stack:%d"), count,
							             itr->second->GetItem_uid(), itr->second->GetItem_stack());
						++count;
					}
				}
				itemlist = m_pMyHawkGroup->GetPetList();	
				if (itemlist.size() > 0)
				{
					int count = 0;
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The pets that I put in hawking for sale"));	
					for (HawkItemMap::iterator itr = itemlist.begin(); itr != itemlist.end(); ++itr)
					{
						TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Pets Slot: %d  Pet ID: %d Stack %d"), count,
							                     itr->second->GetItem_uid(), itr->second->GetItem_stack());
						++count;
					}
							
				}

			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to display the items that you (hawker) put on sales because you haven't opened the hawking business yet"));	

		}
		else if (StrEqual(arg1, _T("display_buy")))
		{
			if (m_pTargetHawkGroup)
			{
				HawkItemMap itemlist = m_pTargetHawkGroup->GetItemList();
				if (itemlist.size() > 0)
				{
					int count = 0;
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The items that I can buy from a hawker"));	
					for (HawkItemMap::iterator itr = itemlist.begin(); itr != itemlist.end(); ++itr)
					{
						TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Item Slot: %d  Item ID: %d Stack:%d"), count,
							                   itr->second->GetItem_uid(), itr->second->GetItem_stack());
						++count;
					}
				}
				itemlist = m_pTargetHawkGroup->GetPartnerList();	
				if (itemlist.size() > 0)
				{
					int count = 0;
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The partners that I can buy from a hawker"));	
					for (HawkItemMap::iterator itr = itemlist.begin(); itr != itemlist.end(); ++itr)
					{
						TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Partner Slot: %d  Partner ID: %d Stack:%d"), count,
							                       itr->second->GetItem_uid(), itr->second->GetItem_stack());
						++count;
					}
				}
				itemlist = m_pTargetHawkGroup->GetPetList();
				if (itemlist.size() > 0)	
				{
					int count = 0;
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The pets that can buy from a hawker"));	
					for (HawkItemMap::iterator itr = itemlist.begin(); itr != itemlist.end(); ++itr)
					{
						TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Pet Slot: %d  Pet ID: %d Stack: %d"), count,
							                     itr->second->GetItem_uid(), itr->second->GetItem_stack());
						++count;
					}
				}

			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to display the available items from hawker because you haven't got the hawker item"));	
					


		}

	}

	////////////////////////////
	//       AntiHack         //
	////////////////////////////
	else if (StrEqual(token, _T("antihack")))
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);	
		if ((!arg1)||(!arg2))
			return;		
		UInt32 Xcor = _ttoi(arg2);	
		UInt32 Ycor = _ttoi(arg3);

		if (StrEqual(arg1, _T("login")))
		{
			WorldService_Proxy::AnswerAntiHack(m_LoginProxyPeer, m_LoginContext, Xcor);	
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Reply the antihack answer during login"));
        }


	}
	////////////////////////////
	//    Guild Command       //
	////////////////////////////
	else if (StrEqual(token, _T("guild")))
	{
		if (!m_pCharacter)
			return;
		arg1 = _tcstok(NULL, delim);
		if (arg1 == NULL && m_ownGuild)
		{
			m_ownGuild->ShowDetail();	
		}
		else if (StrEqual(arg1, _T("create")))
		{
			arg2 = _tcstok(NULL, delim);
			if (arg2)
				MessageService_Proxy::CreateGuild(arg2);
        }
		else if (StrEqual(arg1, _T("apply")))
		{
			arg2 = _tcstok(NULL, delim);
			if (arg2)
			{
				UInt32 guildIndex = _ttoi(arg2);
				MessageService_Proxy::ApplyGuildMembership(guildIndex);	
			}
		}
		else if (StrEqual(arg1, _T("addmember")))
		{
			arg2 = _tcstok(NULL, delim);
            if (arg2)
			{	
				UInt32 applicantIndex = _ttoi(arg2);	
				MessageService_Proxy::ApprovalGuildMembership(applicantIndex);	
			}
		}
		else if (StrEqual(arg1, _T("rejectmember")))
		{
			arg2 = _tcstok(NULL, delim);
            if (arg2)
			{	
				UInt32 applicantIndex = _ttoi(arg2);	
				MessageService_Proxy::RejectGuildMembership(applicantIndex);	
			}           
		}
		else if (StrEqual(arg1, _T("kickmember")))
		{
			arg2 = _tcstok(NULL, delim);
            if (arg2)
			{	
				UInt32 memberIndex = _ttoi(arg2);	
				MessageService_Proxy::KickGuildMember(memberIndex);	
			} 
		}
		else if (StrEqual(arg1, _T("changepost")))
		{
			arg2 = _tcstok(NULL, delim);
			arg3 = _tcstok(NULL, delim);
            if (arg2 && arg3)
			{	
				UInt32 memberIndex = _ttoi(arg2);	
				UInt32 postIndex = _ttoi(arg3);
				MessageService_Proxy::ChangeGuildPost(memberIndex, postIndex);	
			} 
		}
		else if (StrEqual(arg1, _T("leave")))
		{
			MessageService_Proxy::LeaveGuild();
		}
		else if (StrEqual(arg1, _T("aim")))
		{
			Char *token = _tcstok(NULL, delim);	
			String msg;
			while (token)
			{
				msg.Format(_T("%s%s "), msg.c_str(), token);
				token = _tcstok(NULL, delim);
			}			
			MessageService_Proxy::SetGuildAim(msg);
		}
		else if (StrEqual(arg1, _T("rule")))
		{
			Char *token = _tcstok(NULL, delim);	
			String msg;
			while (token)
			{
				msg.Format(_T("%s%s "), msg.c_str(), token);
				token = _tcstok(NULL, delim);
			}
			MessageService_Proxy::SetGuildRule(msg);
		}
		else if (StrEqual(arg1, _T("donate")))
		{
			arg2 = _tcstok(NULL, delim);
			if (arg2 && m_ownGuild)
			{
				UInt32 guildMoney = _ttoi(arg2);
				if (guildMoney >= 0 && guildMoney <= m_pCharacter->GetMoney())
					m_pCharacter->DonateMoney(m_pProxyPeer, m_context, m_ownGuild->GetGuild_uid(), guildMoney);
			}
		}
	}
	/////////////////////////
	///    misc command    //
	/////////////////////////
	else if (StrEqual(token, _T("char")))		// show character detail
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1 && m_pMap) {		// show other char_id
			UInt32 char_id = _ttoi(arg1);
			CCharacter *pchar = m_pMap->FindCharacter(char_id);
			if (pchar)
				pchar->ShowDetail();
		} else {		// show self
			if (m_pBatChar)
				m_pBatChar->ShowDetail();
			else if (m_pCharacter)
				m_pCharacter->ShowDetail();
		}
	}
	else if (StrEqual(token, _T("map")))		// show map detail
	{	
		if (m_pMap)
			m_pMap->ShowObjects();
	}
	else if (StrEqual(token, _T("mapline")))	// show map lines and jump
	{	
		if (m_pMap && m_pCharacter) {
			arg1 = _tcstok(NULL, delim);
			if (arg1 == NULL) {
				WorldService_Proxy::GetMapLines(m_pProxyPeer, m_context, 
					m_pMap->GetMap_id());
			} else {
				UInt8 line_id = _ttoi(arg1);
				m_pCharacter->ChangeMapLine(m_pProxyPeer, m_context, line_id);
			}
		}
	}
	else if (StrEqual(token, _T("cheat")))
	{
		// cheat code in battle
		arg1 = _tcstok(NULL, _T(""));
		if (m_pBattle && m_pBatChar && arg1)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Battle Cheat Code: %s"), arg1);
			m_pBatChar->CheatCode(m_pProxyPeer, m_context, arg1);
		}
		else if (m_pCharacter && arg1)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Map Cheat Code: %s"), arg1);
			m_pCharacter->CheatCode(m_pProxyPeer, m_context, m_pCharacter->GetChar_id(), arg1);
		}
	}
	else if (StrEqual(token, _T("ping")))
	{
		if (m_pCharacter)
			RespondTest();
	} 

}
#pragma optimize("", on)			// resume optimization here

bool CSimClient::ReadOutHawker(Char *word, HawkerItemList &itemlist, HawkerItemList &partnerlist, HawkerItemList &petlist)
{

	Char *delim = _T(", \t");
	if (StrEqual(word, _T("item")))
	{
		if (m_pBag == NULL)
			return FALSE;
        
		Char* index = _tcstok(NULL, delim);
		Char* pri = _tcstok(NULL, delim);
		Char* stk = _tcstok(NULL, delim);

		if ((index == NULL)||(pri == NULL)||(stk == NULL))
			return FALSE;
		UInt8 itemBagIndex = (UInt8) _ttoi(index);// 1st arg:the input data is the bag index
		UInt32 price = (UInt32) _ttoi(pri);// 2nd arg:the input data is the price
		UInt8 stack = (UInt8) _ttoi(stk);// 3rd arg:the input data is the stack 

		CItem* item = m_pBag->GetItem(itemBagIndex);
		if (item)
		{    
			if (item->GetStack() < stack)
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to place an item for sales in hawking: The specified stack is greater than what you actually have"));
				return FALSE;
			}
			if (item->GetItem_soulboundState() != SOULBOUND_ALREADY_BOUND)
			{
				HawkerItemInfo hawkerItem;
				hawkerItem.item_id = item->GetItem_id();
				hawkerItem.item_index = itemBagIndex;
				hawkerItem.item_price = price;
				hawkerItem.item_stack = stack;
				hawkerItem.item_uid = item->GetItem_uid();
				itemlist.push_back(hawkerItem);	
			}
			else
			{	
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to place an item for sales in hawking-The item is bound"));
				return FALSE;
			}

		}
		else
			return FALSE;

	}
	else if (StrEqual(word, _T("partner")))
	{
		if (m_pPartnerGroup == NULL)
			return FALSE;
		
		Char* index = _tcstok(NULL, delim);
		Char* pri = _tcstok(NULL, delim);
	
		if ((index == NULL)||(pri == NULL))
			return FALSE;		

		UInt8 itemBagIndex = (UInt8) _ttoi(index);// 1st arg:the input data is the bag index (0,1,2...)
		UInt32 price = (UInt32) _ttoi(pri);// 2nd arg:the input data is the price

     	CPartner* partner = m_pPartnerGroup->GetPartnerByIndex(itemBagIndex);
		
		if (partner)
		{
			if (partner->GetSoulboundState() != SOULBOUND_ALREADY_BOUND)
			{
				HawkerItemInfo hawkerPartner;
				hawkerPartner.item_id = partner->GetMob_id();
				hawkerPartner.item_index = itemBagIndex;
				hawkerPartner.item_price = price;
				hawkerPartner.item_stack = 1;
				hawkerPartner.item_uid = partner->GetPartner_id();
				partnerlist.push_back(hawkerPartner);
			}
			else
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to place an partner for sales in hawking-The partner is bound"));	
				return FALSE;
			}

		} 
		else
			return FALSE;

	}
	else if (StrEqual(word, _T("pet")))
	{
		if (m_pPetGroup == NULL)
			return FALSE;
		
		Char* index = _tcstok(NULL, delim);
		Char* pri = _tcstok(NULL, delim);
	
		if ((index == NULL)||(pri == NULL))
			return FALSE;		

		UInt8 itemBagIndex = (UInt8) _ttoi(index);// 1st arg:the input data is the bag index (0,1,2...)
		UInt32 price = (UInt32) _ttoi(pri);// 2nd arg:the input data is the price

		CPet* pet = m_pPetGroup->GetPetByIndex(itemBagIndex);
		if (pet)
		{
			if (pet->GetSoulbound() != SOULBOUND_ALREADY_BOUND)
			{
				HawkerItemInfo hawkerPet;
				hawkerPet.item_id = pet->GetPet_id();
				hawkerPet.item_index = itemBagIndex;
				hawkerPet.item_price = price;
				hawkerPet.item_stack = 1;
				hawkerPet.item_uid = pet->GetPet_uid();
				petlist.push_back(hawkerPet);
			}
			else
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to place an pet for sales in hawking-The pet is bound"));	
				return FALSE;
			}

		}
		else
			return FALSE;
	}

	return TRUE;

}

void CSimClient::ResetAttri(CPartner* partner)
{
	partner->m_STA = 0;
	partner->m_STR = 0;
	partner->m_CON = 0;
	partner->m_SPR = 0;
	partner->m_AGI = 0;
    
}

bool CSimClient::ConfirmAttr(CPartner* partner)
{
    bool confirm = FALSE;
	if (partner)
	{
		if ((partner->GetAttrPoint() > (partner->m_STA + partner->m_STR + partner->m_SPR + partner->m_AGI + partner->m_CON))&&(partner->GetAttrPoint()>0))
			if ((partner->m_STA + partner->m_STR + partner->m_SPR + partner->m_AGI + partner->m_CON) > 0)
			{
				partner->RaiseAttrib(m_pProxyPeer, m_context, partner->m_STA, partner->m_SPR, partner->m_STR,
					                 partner->m_CON, partner->m_AGI);
				confirm = TRUE;
			}
	}

	return confirm;
   
}


bool CSimClient::ReadOutAttrValue(Char *cmd, CPartner* partner)
{
	bool read = FALSE;
	bool fstIndex = FALSE;

    if (cmd[0] == _T('-'))
	{
		++cmd;
		partner->attriSign = -1;
		fstIndex = TRUE;	
	
	}
	else if (cmd[0] == _T('+'))
	{
		++cmd;
		partner->attriSign = 1;
		fstIndex = TRUE;	
	}

	Char *delim = _T(", \t");

    if (fstIndex)
	{
        if (_tcsicmp(cmd, _T("STA")) == 0)
		{
			Char *input = _tcstok(NULL, delim);
			if (input)
			{
				UInt32 value = _ttoi(input);	
				read = SetAttri(_T("STA"), value, partner);
			}
		}
		else if (_tcsicmp(cmd, _T("SPR")) == 0)
		{
			Char *input = _tcstok(NULL, delim);	
			if (input)
			{
				UInt32 value = _ttoi(input);	
				read = SetAttri(_T("SPR"), value, partner);
			}
		}
		else if (_tcsicmp(cmd, _T("STR")) == 0)
		{
			Char *input = _tcstok(NULL, delim);	
			if (input)
			{
                UInt32 value = _ttoi(input);	
				read = SetAttri(_T("STR"), value, partner);
			}
		}
		else if (_tcsicmp(cmd, _T("CON")) == 0)
		{
			Char *input = _tcstok(NULL, delim);	
			if (input)
			{
               	UInt32 value = _ttoi(input);	
				read = SetAttri(_T("CON"), value, partner);
			}
		}
		else if (_tcsicmp(cmd, _T("AGI")) == 0)
		{
			Char *input = _tcstok(NULL, delim);	
			if (input)
			{
               	UInt32 value = _ttoi(input);	
				read = SetAttri(_T("AGI"), value, partner);
			}
		}
	}

	return read;

}

bool CSimClient::SetAttri(Char *attri, UInt32 val, CPartner* partner)
{
	bool aboveZero = TRUE;
	if (StrEqual(attri, _T("STA")))
	{
		partner->m_STA = partner->m_STA + partner->attriSign * val;
		if (partner->m_STA < 0)
			aboveZero = FALSE;
	}
	else if (StrEqual(attri, _T("SPR")))
	{
		partner->m_SPR = partner->m_SPR + partner->attriSign * val;
		if (partner->m_SPR < 0)
			aboveZero = FALSE;	
		
	}
	else if (StrEqual(attri, _T("STR")))
	{
		partner->m_STR = partner->m_STR + partner->attriSign * val;
		if (partner->m_STR < 0)
			aboveZero = FALSE;	
	}
	else if (StrEqual(attri, _T("CON")))
	{
		partner->m_CON = partner->m_CON + partner->attriSign * val;
		if (partner->m_CON < 0)
			aboveZero = FALSE;	
	}
	else if (StrEqual(attri, _T("AGI")))
	{
		partner->m_AGI = partner->m_AGI + partner->attriSign * val;
		if (partner->m_AGI < 0)
			aboveZero = FALSE;	
	}	
	return aboveZero;
    
}

bool CSimClient::CheckPetState(UInt8 petIndex, UInt32 &petUID, UInt32 state)
{
	bool havePet = FALSE;
	CPet *pet = NULL;
	pet = m_pPetGroup->GetChild<CPet>(petIndex);
	if ((pet)&&((pet->GetState() == state)||(state == 88)))// value 88 means the check will be done irregardless 
		                                                       // of any pet state
	{
        petUID = pet->GetPet_uid();
		havePet = TRUE;
	}
    return havePet;
}

bool CSimClient::GetSoldType(Char *input, UInt32 &type)
{
	bool assign = FALSE;

    if (StrEqual(input, _T("mix")))
	{
		assign = TRUE;
		type = MIX;
	}
	else if (StrEqual(input, _T("assc")))
	{
		assign = TRUE;
		type = ASSC;	

	}
	else if (StrEqual(input, _T("pet")))
	{
		assign = TRUE;
		type = PET;	

	}
	else if (StrEqual(input, _T("partner")))
	{
		assign = TRUE;
		type = PARTNER;	

	}
	else if (StrEqual(input, _T("cloth")))
	{
		assign = TRUE;
		type = CLOTH;	

	}
	else if (StrEqual(input, _T("other")))
	{
		assign = TRUE;
		type = OTHER;	

	}
   	else if (StrEqual(input, _T("hotsale")))
	{
		assign = TRUE;
		type = HOTSALE;	

	}
	else if (StrEqual(input, _T("limit")))
	{
		assign = TRUE;
		type = LIMITED;	

	}
	else if (StrEqual(input, _T("discount")))
	{
		assign = TRUE;
		type = DISCOUNT;	

	}
	return assign;
}


bool CSimClient::SelectShopItem(UInt32 pos, CChargeShopItem* &select)
{
	bool assign = FALSE;

	if ((pos > m_chShopItem.size())||pos < 0)
	{ 
		select = NULL;
		return FALSE;
	}

	int count = 0;
	for (SellItemMap::iterator iter = m_chShopItem.begin(); iter != m_chShopItem.end(); ++iter)
	{
		if (pos == count)
		{
			select = iter->second;
			assign = TRUE;
			break;
		}
		++count;
	}

	return assign;
  

}

void CSimClient::resetShopData()
{
	
	while (m_chShopItem.size() > 0)
	{
		SellItemMap::iterator iter = m_chShopItem.begin(); 	
		CChargeShopItem *temp = iter->second;
		if (temp)
		{
			DeleteNetObject(temp);
		}
		m_chShopItem.erase(iter);
	}

}


void CSimClient::showSalesSlot()
{
	if (!m_pTrading)
		return;
 
	TRACE_INFODTL(GLOBAL_LOGGER, _F("The Following is the things that\n you put on the trading slot:"));
	TRACE_INFODTL(GLOBAL_LOGGER, _F("-----------------------------------"));
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Items on your sale slot"));

	for (itemMap::iterator iter = m_pTrading->m_tradeItemPos.begin(); iter != m_pTrading->m_tradeItemPos.end(); ++iter)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Sales slot position: %d: item slot=%d"), iter->first, iter->second);	
	}


	TRACE_INFODTL(GLOBAL_LOGGER, _F("Partners on your sale slot"));
	for (itemMap::iterator iter = m_pTrading->m_tradePartnerPos.begin(); iter != m_pTrading->m_tradePartnerPos.end(); ++iter)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Sales slot position:%d: Partner ID =%d"), iter->first, iter->second);	
	}

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Pet on your sale slot"));
	for (itemMap::iterator iter = m_pTrading->m_tradePetPos.begin(); iter != m_pTrading->m_tradePetPos.end(); ++iter)
	{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Sales Slot position: %d: item slot=%d"), iter->first, iter->second);	
	}
	TRACE_INFODTL(GLOBAL_LOGGER, _F("The Following is the things that \nthe opposite side put on the trading slot:"));
	TRACE_INFODTL(GLOBAL_LOGGER, _F("-----------------------------------"));

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Items on the opposite party's sale slot"));
	for (itemMap::iterator iter = m_pTrading->m_oppItem.begin(); iter != m_pTrading->m_oppItem.end(); ++iter)
	{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Sales slot position: %d: item ID =%d"), iter->first, iter->second);	

	}

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Partners on the opposite party's sale slot"));
	for (itemMap::iterator iter = m_pTrading->m_oppPartner.begin(); iter != m_pTrading->m_oppPartner.end(); ++iter)
	{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Sales slot position:%d: Partner ID =%d"), iter->first, iter->second);	
	}

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Pet on the opposite party's sale slot"));
	for (itemMap::iterator iter = m_pTrading->m_oppPet.begin(); iter != m_pTrading->m_oppPet.end(); ++iter)
	{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Sales Slot position: %d: item slot=%d"), iter->first, iter->second);	
	}
}


bool CSimClient::RemoveSlotItem(itemMap &itemlist, UInt32 slot)
{
	bool remove = FALSE;

	if (itemlist.size() > 0)
	{
		for (itemMap::iterator it = itemlist.begin(); it != itemlist.end(); ++it)
		{
			if (it->first == slot)
			{
				remove = TRUE;
				itemlist.erase(it);
				break;
			}
		}
	}
    return remove;
     
}

void CSimClient::tradeSlotValid(itemMap &itemlist, UInt32 bagindex, UInt8 &slot)
{
	bool valid = FALSE;
	bool slotOccupied = FALSE; 

	if (itemlist.size() > 0)
	{
		for (itemMap::iterator it = itemlist.begin(); it != itemlist.end(); ++it)
		{
			if (slot == it->first)
			{
				it->second = bagindex;
				slotOccupied = TRUE;
				valid = TRUE;
				break;
			}
		}
	}
	if (!slotOccupied)
	{
		itemlist.insert(std::make_pair(slot, bagindex));
		valid = TRUE;
	}

}

void CSimClient::TransferMoney(UInt32 money)
{
	
	if (m_pTrading == NULL)
		return;

	if (money <= m_pCharacter->GetMoney())
	{
		m_pTrading->SetTradingMoney(m_pProxyPeer, m_context, money);
		TRACE_INFODTL(GLOBAL_LOGGER, _F("You have successfully transfered a sum of money"));
	}
	else
		TRACE_INFODTL(GLOBAL_LOGGER, _F("You don't get enough $$$ to trade."));

    
}

void CSimClient::AddRemoveTradItem(TradeType tradItem, UInt32 bagindex, bool add, UInt8 slot)
{

	if (m_pTrading == NULL)
		return;
	if (add)
	{	//set item
        if (tradItem == ITEM_TRADE)
		{
            CItem *item = gGlobal.m_pSimClient->m_pBag->GetItem(bagindex);
			if (item)
			{		
                if ((slot < 6) && (slot >= 0))
				{	
					tradeSlotValid(m_pTrading->m_tradeItemPos, bagindex, slot);
					m_pTrading->SetTradingItemSlot(m_pProxyPeer, m_context, bagindex, 1, slot);
					TRACE_INFODTL(GLOBAL_LOGGER, _F("You has put an item on the slot for sales"));
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The trading slot you provided is invalid"));	
			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("No such item you specified"));	

		}
        else if (tradItem == PARTNER_TRADE)
		{
			bool valid = FALSE;
			if (gGlobal.m_pSimClient->m_pPartnerGroup)
			{
					
                for (int i = 0; i < gGlobal.m_pSimClient->m_pPartnerGroup->GetCount(); ++i)
				{
					CPartner *partner = gGlobal.m_pSimClient->m_pPartnerGroup->GetChild<CPartner>(i);	
					if (partner)
					{
						if (bagindex == partner->GetPartner_id())
						{
							valid = TRUE;
							break;
						}
					}
				}
			}
            // set partner
			if (valid)
			{
                if ((slot < 3) && (slot >=0))
				{
					tradeSlotValid(m_pTrading->m_tradePartnerPos, bagindex, slot);	
					m_pTrading->SetTradingPartnerSlot(m_pProxyPeer, m_context,bagindex, slot);//bagindex refer to partnerID
					TRACE_INFODTL(GLOBAL_LOGGER, _F("You has put an partner on the slot for sales")); 
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The trading slot you provided is invalid"));	
			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("No such partner as specified"));	

		}
		else if (tradItem == PET_TRADE)
		{
            // set pet
			bool valid = FALSE;
			if (gGlobal.m_pSimClient->m_pPetGroup)
			{
                for (int i = 0; i < gGlobal.m_pSimClient->m_pPetGroup->GetCount(); ++i)
				{
					CPet *pet = gGlobal.m_pSimClient->m_pPetGroup->GetChild<CPet>(i);
					if (pet)
						if (bagindex == pet->GetPet_uid())
						{
							valid = TRUE;
							break;
						}
				}
			}
            if (valid)
			{
                if ((slot < 3) && (slot >=0))
				{
					tradeSlotValid(m_pTrading->m_tradePetPos, bagindex, slot);	
					m_pTrading->SetTradingPetSlot(m_pProxyPeer, m_context, bagindex, slot);//bagindex refer to pet UID
					TRACE_INFODTL(GLOBAL_LOGGER, _F("You has put an pet on the slot for sales"));
				}
				else
					TRACE_INFODTL(GLOBAL_LOGGER, _F("The trading slot you provided is invalid"));	
			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("No such pet as specified"));	

		}
	}
	else
	{
        if (tradItem == ITEM_TRADE)// remove item
		{
			if (RemoveSlotItem(m_pTrading->m_tradeItemPos, slot))
			{
                m_pTrading->RemoveTradingItemSlot(m_pProxyPeer, m_context, slot);
				TRACE_INFODTL(GLOBAL_LOGGER, _F("You has removed an item from trading slot"));
			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("The trading slot that you want to remove is invalid"));	
		}
        else if (tradItem == PARTNER_TRADE)
		{
            // remove partner
            if (RemoveSlotItem(m_pTrading->m_tradePartnerPos, slot))
			{
                m_pTrading->RemoveTradingPartnerSlot(m_pProxyPeer, m_context, slot);
				TRACE_INFODTL(GLOBAL_LOGGER, _F("You has removed an partner from trading slot"));
			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("The trading slot that you want to remove is invalid"));	
		}
		else if (tradItem == PET_TRADE)
		{
            // remove pet
			if (RemoveSlotItem(m_pTrading->m_tradePetPos, slot))
			{
                m_pTrading->RemoveTradingPetSlot(m_pProxyPeer, m_context, slot);
				TRACE_INFODTL(GLOBAL_LOGGER, _F("You has removed a pet from trading slot"));
			}
			else
				TRACE_INFODTL(GLOBAL_LOGGER, _F("The trading slot that you want to remove is invalid"));	
		}
	}

}

VOID CSimClient::OnUpdate(CONST UInt32 dt)
{
	// safety check objId if SimClient is not crashed
	if (m_objId != GetID())
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
			_T("SimClient Error: object dead, SimClient %d, objId %d"), 
			GetID(), m_objId);
		ProcessManager::GetInstance().Detach(this);	// detach from process manager
		return;
	}
	
	ThreadGuard guard(m_key);

	Activate();
	if (m_bDelete) {
		m_bDelete = BOOLEAN_FALSE;
		DeleteObjects();
		if (m_reconnect) {
			//Sleep(1000);					// delay before reconnect
			m_reconnect = false;
			ConnectServer();
		}
	}

	if (m_bRobot)
		RobotAction(dt);

	// check if battle lock
	if (m_pCharacter) {
		if (m_batDeadlock != 0) {
			if (m_batDeadlock <= dt) {
				TRACE_ERRORDTL_4(GLOBAL_LOGGER, 
					_T("SimClient Error: Battle Deadlock, char_id %d, name %s, battle %d, action %d"), 
					m_pCharacter->GetChar_id(), m_loginName, m_pBattle, m_pCharacter->GetAction());
				m_batDeadlock = BATTLE_DEADLOCK_TIME;
				//m_commandList.push_back(_T("move j0"));
			} else {
				m_batDeadlock -= dt;
			}
		} else if (m_pBattle == NULL) {
			if (m_pCharacter->IsInBattle())
				m_batDeadlock = BATTLE_DEADLOCK_TIME;
			else
				m_batDeadlock = 0;
		}
	}

	if (m_pCharacter && m_pCharacter->IsMoving())
		m_pCharacter->NextMove();

	// execute waiting commands
	StringVector executeList;
	{
		ThreadGuard guard(m_key);
		executeList.swap(m_commandList);
	}

	if (!executeList.empty()) {
		for (StringVector::iterator it = executeList.begin(); it != executeList.end(); ++it)
			Execute(it->c_str());
	}

	// respond test
	if (IsOption(OPT_AUTO_PING) && m_resTestTime != UINT32_MAX_VALUE) {
		m_resTestTime += dt;
		if (m_pProxyPeer && m_pCharacter && m_resTestTime >= RESPOND_INTERVAL)
			RespondTest();
	}
}

void CSimClient::RobotAction(CONST UInt32 dt)
{
	if (m_lastActTime >= m_actionTime)
	{
		m_lastActTime = 0;

		// adjust robot action time when respond not good
		if (m_actionTime != gGlobal.m_robotInterval)
			m_actionTime = gGlobal.m_robotInterval + m_lastRespond;

		// try connect if not connected
		if (m_pProxyPeer == NULL) {
			if (!ConnectServer())
				return;
		}
		if (m_login == LS_LOGINED && m_pAccount &&
			m_pMap && m_pCharacter &&			// valid character
			m_pBatChar == NULL)
		{ 
			if (QuestJumped && m_pQuestSys && m_pQuestSys->IsOutstandingQuestTask())
			{
				QuestJumped = FALSE;
				int questID = m_pQuestSys->FindQuestIDByLocation();
		      //CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "NPCQuestTalkStart", questID, this); 
				CallLuaFunc(m_pLuaState, "QuestStartRepeatNPCTalk", String(), questID);
			}
			CallLuaFunc(m_pLuaState, "MapAction", String(),	// robot action in Map
				m_pMap, m_pCharacter);
		} else if (m_login == LS_LOGOUT) {					// robot action in Account
			StringA name;

			m_batDeadlock = 0;
			if (m_pLuaState)
			{
				StrToUTF8(m_accountName.c_str(), name);
				CallLuaFunc(m_pLuaState, "AccountAction", String(), 
					name.c_str(), gGlobal.m_shadowNum, gGlobal.m_clientTotal); 
			}
			else
				WaitExecute(_T("login"));
		}
	}
	else
		m_lastActTime += dt;
}

bool CSimClient::ConnectServer()
{
	if (gGlobal.m_bExit)
		return false;

	m_reconnect = false;
	if (m_pProxyPeer)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Already connected to Proxy Service, socket %d"), 
			m_pProxyPeer->GetSocketID());
		return true;
	}

	LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();

	// connect to object service
	DWORD startTime = ::timeGetTime();
	Boolean bConnect = false;
	//while (!gGlobal.m_bExit && ::timeGetTime() - startTime < USER_CONNECT_TIME)
	do
	{
		{
			ThreadGuard guard(gGlobal.m_key);
			m_pProxyPeer = rpcmgr.GetModel().Connect(
				Address(gConfigure.ipProxy[0], gConfigure.portProxy[0]), 
				Reuben::Network::PROTOCOL_TCP, BOOLEAN_FALSE);
		}
		if (m_pProxyPeer) {
			bConnect = BOOLEAN_TRUE;
			break;
		} else if (m_bRobot) {
			break;
		}
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SimClient %s: Connecting Proxy Service ..."), m_loginName);
		if (!bConnect)
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("SimClient %s: Connecting Proxy Service Failed : WSAGetLastError=%d"), m_loginName, ::WSAGetLastError());
		}
		Sleep(200);
	} while(false);
	
	if (bConnect)
	{
		ThreadGuard guard(gGlobal.m_key);

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SimClient %s: Connected to Proxy Service, socket %d"), 
			m_loginName, m_pProxyPeer->GetSocketID());
		PeerData* pData = SafeCreateObject(PeerData);
		pData->pSimClient = this;
		m_pProxyPeer->SetData(pData);
		UInt32 version = VERSION_NUMBER;
		LocalRPCManager::GetInstance().GetModel().Send(m_pProxyPeer, (CONST Byte*)&version, sizeof(UInt32));
		m_context.sessionId.serviceId = 0;
		m_context.sessionId.socketId = m_pProxyPeer->GetSocketID();
		m_context.sessionId.ticketId = 0; //::timeGetTime();
		gGlobal.m_clientSockets[m_pProxyPeer->GetSocketID()] = this;
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("SimClient %s: Failed to Connect to Proxy Service. : WSAGetLastError=%d"),
			m_loginName, ::WSAGetLastError());
		if (m_bRobot)
			m_actionTime = 1000;		// 1 second to retry for robot
		return false;
	}

	return true;
}

void CSimClient::DisconnectServer()
{
	if (m_pProxyPeer == NULL) {
		TRACE_INFODTL(GLOBAL_LOGGER, _F("DisconnectServer Warning: no connection to disconnect"));
		return;
	}

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Try to disconnect Proxy Service, socket %d"), 
		m_pProxyPeer->GetSocketID());

	LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();
	rpcmgr.GetModel().CloseSocket(m_pProxyPeer->GetSocketID());

	if (m_bRobot) {		// robot login action
		m_actionTime = GetRand(RECONNECT_TIME_MIN, RECONNECT_TIME_MAX);
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SimClient %s: robot login again after %.2fs"), 
			m_loginName, m_actionTime / 1000.0f);
	}
}

void CSimClient::OnDisconnect()
{
	m_bDelete = BOOLEAN_TRUE;
	if (m_pProxyPeer)
	{
		PeerData* pData = (PeerData*) m_pProxyPeer->GetData();
		SafeDeleteObject(pData);
		m_pProxyPeer->SetData(NULL);
	}
	SafeRelease(m_pProxyPeer);
}

void CSimClient::DeleteChannels()
{
	for (int i = 0; i < MAX_CHANNELTYPE; i++)
	{
		if (m_pChannel[i]) {
			DeleteNetObject(m_pChannel[i]);
			m_pChannel[i] = NULL;
		}
	}
}

/*
RPCResult CSimClient::Login(RPCContext &context)
{
	Activate();

	if (m_pProxyPeer == NULL)		// still cannot login
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)

	PROCEDURE_START_METHOD2(CSimClient, Login, this, context, String, name, ByteArray, password)
		
		m_login = LS_LOGINING;
		WorldService_Proxy::LoginAccount(m_pProxyPeer, context, 0, name, password);

	PROCEDURE_WAIT3(1, WorldService, CB_AntiHack, ByteArray, image, UInt32, x, UInt32, y)

		WorldService_Proxy::AnswerAntiHack(m_pProxyPeer, context, x, y);
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Login: auto-reply antihack answer X:%d Y:%d "), x, y);	

	PROCEDURE_WAIT1(2, WorldService, CB_LoginResult, UInt8, result)

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("result %d"), result);
		if (result == LOGIN_SUCCESSED_NEED_KICK)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Need to kick another player? [yes/no]"));
			if (m_bRobot)
				WorldService_Proxy::KickAccount(m_pProxyPeer, context, BOOLEAN_TRUE);
		}
		m_login = LS_LOGINED;

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			m_login = LS_LOGOUT;
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Login Error: WorldService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH 

	return RPC_RESULT_OK;
}
*/

/*
RPCResult CSimClient::Logout(RPCContext &context)
{
	Activate();

	if (m_pProxyPeer == NULL)
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)

	PROCEDURE_START_METHOD(CSimClient, Logout, this, context)

		if (m_pAccount)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Logout from server"));
			m_pAccount->Logout(m_pProxyPeer, context);
		} else {
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

	PROCEDURE_WAIT_OBJ(2, Account, CB_LogoutSuccess, account)

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Logout successful"));
		DeleteObjects();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(MapService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Logout: MapService::CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

		PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Logout: Account::CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}
*/

void CSimClient::RespondTest(UInt16 playerCount)
{
	if (playerCount == 0) {						// start respond test
		if (m_pCharacter == NULL || m_pProxyPeer == NULL)
			return;
		m_resTestTime = UINT32_MAX_VALUE;
		m_startTestTime = ::timeGetTime();
		m_pCharacter->RespondTest(m_pProxyPeer, m_context, m_minRespond, m_maxRespond,
			m_avgRespond, m_respondCount);
	} else {									// end respond test
		m_lastRespond = ::timeGetTime() - m_startTestTime;
		if (m_lastRespond < m_minRespond)
			m_minRespond = m_lastRespond;
		if (m_lastRespond > m_maxRespond)
			m_maxRespond = m_lastRespond;
		m_avgRespond = (m_avgRespond * m_respondCount + m_lastRespond) / (m_respondCount + 1);
		m_respondCount++;
		m_resTestTime = 0;

		TRACE_ALWAYSDTL_6(GLOBAL_LOGGER, 
			_T("RespondTest: count %d(L%d), last %d, avg %.2f, min %d, max %d"), 
			playerCount, (m_pMap ? m_pMap->GetLine_id() : 0), m_lastRespond, m_avgRespond,
			m_minRespond, m_maxRespond);
	}
}

RPCResult CSimClient::TestLock1(RPCContext &context)
{
	PROCEDURE_START_FUNC1(TestLock1, context, int, i)
	
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CSimClient TestLock1 %d: pass 1"), i);

	PROCEDURE_WAIT_TIMEOUT(1, 4000)

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CSimClient TestLock1 %d: pass 2"), i);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT
			
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CSimClient TestLock1 %d: exception"), i);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult TestLock1(RPCContext &context);
RPCResult TestLock2(RPCContext &context);

RPCResult TestLock(RPCContext &context)
{
	RPCContext context2;
	context2.sessionId.serviceId = 1;
	context2.sessionId.socketId = 0;
	context2.sessionId.ticketId = 0;

	CSimClient* pSimClient = SafeCreateObject(CSimClient);
	PROCEDURE_DO_FUNC2(TestLock1, context2, int, 1, CSimClient*, pSimClient)
	PROCEDURE_DO_METHOD1(CSimClient, TestLock1, pSimClient, context2, int, 2)
	PROCEDURE_DO_FUNC1(TestLock2, context2, int, 3)

	return RPC_RESULT_OK;
}

RPCResult TestLock1(RPCContext &context)
{
	PROCEDURE_START_FUNC2(TestLock1, context, int, i, CSimClient*, pSimClient)

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock1 %d: pass 1"), i);

	PROCEDURE_WAIT_TIMEOUT(1, 2000)

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock1 %d: pass 2"), i);
		ProcedureManager::GetInstance().RemoveByObject(pSimClient);
		SafeDeleteObject(pSimClient);

	PROCEDURE_END

	return RPC_RESULT_OK;
}

RPCResult TestLock2(RPCContext &context)
{
	PROCEDURE_START_FUNC1(TestLock2, context, int, i)

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock2 %d: pass 1"), i);

	PROCEDURE_WAIT_TIMEOUT(1, 6000)

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock2 %d: pass 2"), i);

	PROCEDURE_END

	return RPC_RESULT_OK;
}

/*
RPCResult TestLock1(RPCContext &context);
RPCResult TestLock2(RPCContext &context);
RPCResult TestSubLock(RPCContext &context);
RPCResult TestSubLock2(RPCContext &context, int i);

void TestLock(RPCContext &context)
{
	RPCContext context2;
	PROCEDURE_DO_FUNC1(TestLock1, context2, int, 100);
	PROCEDURE_DO_FUNC1(TestLock2, context2, int, 200);
	PROCEDURE_DO_FUNC1(TestLock1, context2, int, 300);
}

RPCResult TestLock1(RPCContext &context)
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pProxyPeer == NULL) {
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_FUNC1(TestLock1, context, int, i)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock1 %d: pass 1"), i);

	WorldService_Proxy::GetMapLines(gGlobal.m_pSimClient->m_pProxyPeer, context, 100);

	PROCEDURE_WAIT2(1, WorldService, CB_GetMapLines, Int32Vector, countList, UInt16Vector, stateList)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock1 %d: pass 2"), i);

	PROCEDURE_DO_FUNC2(TestSubLock, context, int, i, Boolean, BOOLEAN_FALSE)

	PROCEDURE_WAIT_RETURN(2)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock1 %d: pass 3"), i);

	WorldService_Proxy::GetMapLines(gGlobal.m_pSimClient->m_pProxyPeer, context, 100);

	PROCEDURE_WAIT2(3, WorldService, CB_GetMapLines, Int32Vector, countList, UInt16Vector, stateList)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock1 %d: pass end"), i);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("TestLock1 Error: WorldService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult TestLock2(RPCContext &context)
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pProxyPeer == NULL) {
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_FUNC(TestLock2, context)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock2 %d: pass 1"), 200);

	TestSubLock2(context, 210);

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock2 %d: pass 2"), 200);

	TestSubLock2(context, 220);

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock2 %d: pass end"), 200);

	PROCEDURE_END

	return RPC_RESULT_OK;
}

RPCResult TestSubLock(RPCContext &context)
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pProxyPeer == NULL) {
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_FUNC2(TestSubLock, context, int, i, Boolean, locked)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestSubLock %d: pass 1"), i);

	WorldService_Proxy::GetMapLines(gGlobal.m_pSimClient->m_pProxyPeer, context, 100);

	PROCEDURE_WAIT2(1, WorldService, CB_GetMapLines, Int32Vector, countList, UInt16Vector, stateList)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestSubLock %d: pass 2"), i);

	WorldService_Proxy::GetMapLines(gGlobal.m_pSimClient->m_pProxyPeer, context, 100);

	PROCEDURE_WAIT2(2, WorldService, CB_GetMapLines, Int32Vector, countList, UInt16Vector, stateList)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestSubLock %d: pass end"), i);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("TestSubLock Error: WorldService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult TestSubLock2(RPCContext &context, int i)
{
	RPCContext context2(context.sessionId);
	PROCEDURE_DO_FUNC2(TestSubLock, context2, int, i, Boolean, BOOLEAN_FALSE)

	return RPC_RESULT_OK;
}

RPCResult TestLock(RPCContext &context);
RPCResult TestNoLock(RPCContext &context);
RPCResult TestSubLock(RPCContext &context);

void TestLock()
{
	RPCContext context;
	PROCEDURE_DO_FUNC1(TestLock, context, int, 100);
	PROCEDURE_DO_FUNC1(TestNoLock, context, int, 200);
	PROCEDURE_DO_FUNC1(TestLock, context, int, 300);
}

RPCResult TestLock(RPCContext &context)
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pProxyPeer == NULL) {
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_FUNC1(TestLock, context, int, i)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock %d: locking ..."), i);

	PROCEDURE_LOCK

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock %d: pass 1"), i);

	WorldService_Proxy::GetMapLines(gGlobal.m_pSimClient->m_pProxyPeer, context, 100);

	PROCEDURE_WAIT1(1, WorldService, CB_GetMapLines, Int32Vector, countList)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock %d: pass 2"), i);

	PROCEDURE_DO_FUNC2(TestSubLock, context, int, i, Boolean, BOOLEAN_FALSE)

	PROCEDURE_WAIT_RETURN(2)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock %d: pass 3"), i);

	WorldService_Proxy::GetMapLines(gGlobal.m_pSimClient->m_pProxyPeer, context, 100);

	PROCEDURE_WAIT1(3, WorldService, CB_GetMapLines, Int32Vector, countList)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestLock %d: pass end"), i);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("TestLock Error: WorldService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult TestNoLock(RPCContext &context)
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pProxyPeer == NULL) {
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_FUNC1(TestNoLock, context, int, i)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestNoLock %d: pass 1"), i);

	WorldService_Proxy::GetMapLines(gGlobal.m_pSimClient->m_pProxyPeer, context, 100);

	PROCEDURE_WAIT1(1, WorldService, CB_GetMapLines, Int32Vector, countList)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestNoLock %d: pass 2"), i);

	PROCEDURE_DO_FUNC2(TestSubLock, context, int, i, Boolean, BOOLEAN_TRUE)

	PROCEDURE_WAIT_RETURN(2)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestNoLock %d: pass 3"), i);

	WorldService_Proxy::GetMapLines(gGlobal.m_pSimClient->m_pProxyPeer, context, 100);

	PROCEDURE_WAIT1(3, WorldService, CB_GetMapLines, Int32Vector, countList)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestNoLock %d: pass end"), i);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("TestNoLock Error: WorldService:CB_Error, errcode %d, %s"),	
				err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult TestSubLock(RPCContext &context)
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pProxyPeer == NULL) {
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_FUNC2(TestSubLock, context, int, i, Boolean, locked)

	if (locked) {
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestSubLock %d: locking ..."), i);
		PROCEDURE_LOCK
	}

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestSubLock %d: pass 1"), i);

	WorldService_Proxy::GetMapLines(gGlobal.m_pSimClient->m_pProxyPeer, context, 100);

	PROCEDURE_WAIT1(1, WorldService, CB_GetMapLines, Int32Vector, countList)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestSubLock %d: pass 2"), i);

	WorldService_Proxy::GetMapLines(gGlobal.m_pSimClient->m_pProxyPeer, context, 100);

	PROCEDURE_WAIT1(2, WorldService, CB_GetMapLines, Int32Vector, countList)

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("TestSubLock %d: pass end"), i);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("TestSubLock Error: WorldService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}
*/
