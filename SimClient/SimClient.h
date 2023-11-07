#pragma once
#ifndef _SIM_CLIENT_H_
#define _SIM_CLIENT_H_

#include "Common.h"
#include "Common/ChannelType.h"



#define COMMAND_LENGTH				1024
#define BATTLE_DEADLOCK_TIME		5000

enum LoginState {
	LS_LOGOUT = 0,
	LS_LOGINING,
	LS_LOGINED,
};

enum TradeType {
	ITEM_TRADE = 0,
	PARTNER_TRADE,
	PET_TRADE,
};

class CAccount;
class CCharacter;
class CMap;
class CBatCharacter;
class CBatMob;
class CBattle;
class CChannel;
class CItemBag;
class CClientResManager;
class CScriptSystem;
class CPartnerGroup;
class CSkillGroup;
class CQuestSystem;
class CMessenger;
class CMessageCharacter;
class CPetGroup;
class CTrading;
class CChargeShop;
class CChargeShopItem;
class CPartner;
class CHawkGroup;
class CHawkItem;
struct HawkerItemInfo;
struct lua_State;
class CGuildMember;
class CGuildApplicant;
class CGuild;
class CQuestion;
class CSellYuanBaoGroup;

typedef StlMap<UInt32, CGuild*, std::less<UInt32>, 
	ManagedAllocator<std::pair<UInt32, CGuild*> > > GuildMap;

class CSimClient : 
	public Reuben::Simulation::Object, 
	public Reuben::Utility::Process
{
public:
	typedef StlMap<RPCNetID, NetObject*, std::less<RPCNetID>, 
		ManagedAllocator<std::pair<RPCNetID, NetObject*> > > NetIDMap;
	typedef StlMap<UInt16, UInt32, std::less<UInt16>, 
		      ManagedAllocator<std::pair<UInt16, UInt32> > > itemMap;
	typedef StlMultiMap<UInt32, CChargeShopItem *, std::greater_equal<UInt32>, 
		  ManagedAllocator<std::pair<UInt32, CChargeShopItem *> > > SellItemMap;
	typedef StlVector<HawkerItemInfo, ManagedAllocator<HawkerItemInfo> > HawkerItemList;	
	typedef StlMap<UInt32, CGuildApplicant*, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, CGuildApplicant*> > > GuildApplicantMap;

	NetIDMap		m_custIdMap;		// own customized RPCNetID -> NetObject map

	LPCPEER			m_pProxyPeer;		// proxy peer connection
	RPCContext		m_context;
	RPCContext		m_servContext;
	CAccount*		m_pAccount;
	CCharacter*		m_pCharacter;
	CMap*			m_pMap;
	NetGroup*		m_characters;
	CBatCharacter*	m_pBatChar;
	CBatMob*		m_pPartner;
	CBattle*		m_pBattle;
	CPartnerGroup*	m_pPartnerGroup;
	CSkillGroup*	m_pSkillGroup;
	CQuestSystem*	m_pQuestSys;  // for quest command
	NetGroup*		m_pMessengerGroup; // for friend command
	//NetGroup*		m_pMsgCharGroup;
	CQuestion*		m_pQuestion;
	CSellYuanBaoGroup* m_pSellYuanBaoGroup;

	/////members for trading command
	CTrading*       m_pTrading; // a trade object	
	UInt32          m_TarTradeID; // trade target character
	bool            m_bTradReqCfm; // Trade request confirmation 
	bool			m_getTradReq; //request for a trade	
	bool            m_btradelock;// whether trade is locked
	bool            m_btarlock; // whether the trade is locked by the other side.
	/////members for shopping command
	CChargeShop*    m_pshop;  // a shop
	SellItemMap     m_chShopItem; // the shop items 	
	bool            m_bShopOpen; // open a shop or not
	/////members for pet command
	CPetGroup*      m_pPetGroup;// the pet list
	/////members for NPC talk command 
	bool m_talkInProcess;
	UInt32  m_talkStep;
	/////members for hawker command// the hawker
	CHawkGroup*     m_pMyHawkGroup;	// the hawking business owned by yourself
	CHawkGroup*     m_pTargetHawkGroup; //the target hawker who you want to buy items from
	/////members for quest command
	bool QuestJumped;
	/////members for guild///////////
	GuildMap		m_guildMap;				// all guilds in the world	
	CGuild*			m_ownGuild;
	CGuildMember*	m_ownGuildMember;
	//////members for guild//////
	LPCPEER			m_LoginProxyPeer;		// proxy peer connection
	RPCContext		m_LoginContext;	



	CChannel*		m_pChannel[MAX_CHANNELTYPE];
	CItemBag*		m_pBag;
	CScriptSystem*	m_pScriptSys;
	UInt16			m_lastMoveX;		// last move X
	UInt16			m_lastMoveY;		// last move Y
	LoginState		m_login;			// login flag
	Boolean			m_reconnect;		// reconnect flag
	Boolean			m_bRobot;			// is robot
	Char			m_loginName[17];	// main login name
	String			m_accountName;		// last account name
	Logger*			m_pLogger;			// SimClient own logger
	Boolean			m_bDelete;			// delete object flag
	UInt32			m_actionTime;		// next action time
	UInt32			m_lastActTime;		// last action time
	UInt32			m_batDeadlock;		// time test for battle dead lock
	StringVector	m_commandList;		// command waiting to execute
	ThreadKey		m_key;				// safe thread key

	UInt32			m_resTestTime;			// respond testing time
	UInt32			m_startTestTime;		// respond start testing time
	UInt32			m_lastRespond;			// last respond time
	UInt32			m_minRespond;			// minimum respond time
	UInt32			m_maxRespond;			// maximum respond time
	float			m_avgRespond;			// average respond time
	UInt32			m_respondCount;			// respond count
	UInt8Vector		m_maplines;				// map line number
	lua_State*		m_pLuaState;			// lua thread for each robot
	int				m_luaRef;				// lua thread reference key
	UInt32			m_objId;				// SimClient object ID
	

	CSimClient();
	~CSimClient();
	void IniAfterDisconnect();	
	void Init();
	void Release();
	void DeleteObjects();
	bool ConnectServer();
	void DisconnectServer();
	void OnDisconnect();
	void WaitExecute(PCSTRING cmd);
	void Execute(PCSTRING cmd);
	virtual VOID OnUpdate(CONST UInt32 dt);
	void RobotAction(CONST UInt32 dt);
	//RPCResult GetAccount(RPCContext &context);
	//RPCResult Login(RPCContext &context);
	//RPCResult Logout(RPCContext &context);
	void RespondTest(UInt16 playerCount=0);
	void DeleteChannels();
	void Activate();
	///trading command
	bool RemoveSlotItem(itemMap &itemList, UInt32 slot);
	void AddRemoveTradItem(TradeType tradItem, UInt32 bagindex, bool add, UInt8 slot);
	void tradeSlotValid(itemMap &itemlist, UInt32 bagindex, UInt8 &slot);
    void showSalesSlot();
	void TransferMoney(UInt32 money);	
		
	///online shop command
	bool SelectShopItem(UInt32 itemType, CChargeShopItem* &select);	
	void resetShopData();
	bool GetSoldType(Char *input, UInt32 &type);	
	///pet command
	bool CheckPetState(UInt8 petIndex, UInt32 &petUID, UInt32 state);

	/// partner command
	bool ReadOutAttrValue(Char *cmd, CPartner* partner);
    bool ConfirmAttr(CPartner* partner);
	bool SetAttri(Char* attri, UInt32 value, CPartner* partner);
	void ResetAttri(CPartner* partner);	

	/// hawker command
	bool ReadOutHawker(Char *word, HawkerItemList &itemlist, HawkerItemList &partnerlist, HawkerItemList &petlist);	

	RPCResult TestLock1(RPCContext &context);
	DEFINE_PARENT_CLASS(Reuben::Simulation::Object);
	DEFINE_CLASS(0);			// don't care class id
};

#endif // _SIM_CLIENT_H_
