#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "UI.h"
#include "PlayOggVorbis.h"
#include "common_binary.h"
#include "Proxy/View/GlobalView.h"
#include "Common/ChannelType.h"
#include "AppMapState.h"
#include "Resource/GameStringTable.h"
#include "NumberEnt.h"
#include "MessageBox.h"
#include "Reuben/Math/Random.h"

#define SPEAKER_DISAPPEAR_TIME	15000
CONST Int MaxTextColor = 13;
CONST Int MaxItemTextColor = 9;
CONST Int MaxHis = 50;
CONST Int InvisibleAlphaOffset = 170;

class CAccountControl;
class CBattleControl;
class CBatBubbleControl;
class CCharacterControl;
class CMapControl;
class CMap;
class CChannelControl;
class CParty;
class CSkillGroup;
class CQuestSystem;
class CClientResManager;
class CItemBag;
class CScriptControl;
class CBatScriptControl;
class PathFinder;
struct DrawEntityTextureData;
class CMenuItemProcess;
class CMenuPartyProcess;
class CMessageBox;
class CCreateCharacterProcess;
class CMiddleMap;
class CMenuCharacterSimulator;
class CPartnerGroup;
class CMenuJoinBattleProcess;
class CMenuPartnerProcess;
class CLoginProcess;
class CMenuSkillProcess;
class CMenuShopProcess;
class CMenuSearchListProcess;
class CMenuQuestLogProcess;
class CBuff;
class CRumorMsgProcess;
class CMenuCharProcess;
class CMenuGivenProcess;
class CDirtyRectManager;
class CBigMap;
class CMenuTargetProcess;
class CMenuBuffIconProcess;
class CNewBieHintCtrl;
class CMenuSystemSetting;
class CMenuTradingProcess;
class CMenuFriendProcess;
class CTrading;
class CMenuHelpProcess;
class CMenuOnlineShopProcess;
class CMenuRankProcess;
class CPetGroup;
class CMenuPetProcess;
class CMenuMixProcess;
class CMenuTitleProcess;
class CAntiHack;
class CMenuHawkerProcess;
class CHawkGroup;
class CMenuGuildProcess;
class CMenuBankService;
class CGiftBox;
class CMenuGiftBox;
class CMenuQuestJournal;
class CWeaponsRank;
class CTeamDescription;
class CMenuWareHouseProcess;
class CMenuAutoWalkProcess;
class CMenuSellYuanBaoProcess;
class CMenuTargetEquipProcess;
class CSellYuanBaoGroup;
class CGenerationProcess;
class CMenuPartnerCultivationUpProcess;
class CAccShopProcess;
class CPointsShopProcess;
class CMessageCharacter;
class CPartyCharInfoClient;

#define WAV_FILE_PATH	_T("Data\\Wav\\")
#define EMOTION_ICON_PATH	_T("EmotionIcon\\Emotion")

CONST Int MaxBattleEnt = 40;
CONST Int BattlePos[MaxBattleEnt][2] = {
	{11,2}, {7,2},{15,2},{3,2},{19,2},
	{11,6}, {7,6},{15,6},{3,6},{19,6},
	{11,10},{7,10},{15,10},{3,10},{19,10},
	{11,14},{7,14},{15,14},{3,14},{19,14},
	{15,36},{11,36},{19,36},{7,36},{23,36},
	{15,32},{11,32},{19,32},{7,32},{23,32},
	{15,28},{11,28},{19,28},{7,28},{23,28},
	{15,24},{11,24},{19,24},{7,24},{23,24},
	};

const Color32 RedColor = D3DCOLOR_ARGB(255, 255, 60, 60);
const Color32 PurpleColor = D3DCOLOR_ARGB(255, 128, 0, 128);
const String RedColorCmd = "R";

const UInt32 SpeakRestTime[MAX_CHANNELTYPE] = {2, 2, 30, 5, 2, 90, 2, 2, 2, 2, 2,2,0,2,2};

const PSTRINGA ChannelXMLName[MAX_CHANNELTYPE] = {
	"ChannelMapName",
	"ChannelPartyName",
	"ChannelGangName",
	"ChannelGuildName",
	"ChannelPrivateName",
	"ChannelGlobalName",
	"ChannelCoupleName",
	"ChannelSystemName",
	"ChannelRumorName",
	"",
	"",
	"",
	"ChannelSpeakerName",
	"ChannelFriendName",//friend
	"ChannelInstanceName",//instance
};

const PSTRINGA ChannelBkXMLName[MAX_CHANNELTYPE] = {
	"ChannelMapNameBk",
	"ChannelPartyNameBk",
	"ChannelGangNameBk",
	"ChannelGuildNameBk",
	"ChannelPrivateNameBk",
	"ChannelGlobalNameBk",
	"ChannelCoupleNameBk",
	"ChannelSystemNameBk",
	"ChannelRumorNameBk",
	"",
	"",
	"",
	"ChannelSpeakerNameBk",
	"ChannelFriendNameBk",//friend
	"ChannelInstanceNameBk",//instance
};

const PSTRINGA ReceiveChannelXMLName[MAX_CHANNELTYPE] = {
	"ReceiveMapChannel",
	"ReceivePartyChannel",
	"ReceiveGangChannel",
	"ReceiveGuildChannel",
	"ReceivePrivateChannel",
	"ReceiveGlobalChannel",
	"ReceiveCoupleChannel",
	"",
	"ReceiveRumorChannel",
	"",
	"",
	"",
	"",
	"ReceiveFriendChannel",//friend
	"ReceiveInstanceChannel",//instance
};

const PSTRINGA SubReceiveChannelXMLName[MAX_CHANNELTYPE] = {
	"SubChannelMapButton",
	"SubChannelPartyButton",
	"SubChannelGangButton",
	"SubChannelGuildButton",
	"SubChannelPrivateButton",
	"SubChannelGlobalButton",
	"SubChannelCoupleButton",
	"",
	"",
	"",
	"",
	"",
	"",
	"",//friend
	"",//instance
};

const PSTRINGA ChannelColor[MAX_CHANNELTYPE] ={ 
		"W",
		"A",
		"B",
		"G",
		"P",
		"Y",
		"V",		
		"R",
		"O",
		"W",
		"W",
		"W",
		"G",
		"E",
		"G",
		};


const Color32 TextColor[MaxTextColor] ={ 
		D3DCOLOR_ARGB(255, 248, 144, 0),
		D3DCOLOR_ARGB(255, 240, 248, 0),
		D3DCOLOR_ARGB(255, 56, 248, 56),
		D3DCOLOR_ARGB(255, 0, 248, 240),
		D3DCOLOR_ARGB(255, 120, 128, 240),
		D3DCOLOR_ARGB(255, 176, 84, 208),
		D3DCOLOR_ARGB(255, 240, 4, 240),		
		D3DCOLOR_ARGB(255, 255, 255, 255),
		//D3DCOLOR_ARGB(255, 255, 174, 0),
		//D3DCOLOR_ARGB(255, 0, 0, 0),
		D3DCOLOR_ARGB(255, 180, 190, 170),
		D3DCOLOR_ARGB(255, 199, 178, 153),
		D3DCOLOR_ARGB(255, 10, 12, 68),
		D3DCOLOR_ARGB(255, 200, 0, 0),
		D3DCOLOR_ARGB(255, 180, 70, 35),
		};
const Color32 ItemTextColor[MaxItemTextColor]={
		D3DCOLOR_ARGB(255, 180, 190, 170),  //ItemColorIndex_Grey
		D3DCOLOR_ARGB(255, 165, 255, 90),	//ItemColorIndex_LightGreen
		D3DCOLOR_ARGB(255, 30, 160, 255),	//ItemColorIndex_LightBlue
		D3DCOLOR_ARGB(255, 230, 150, 255),	//ItemColorIndex_LightPurple	
		D3DCOLOR_ARGB(255, 255, 190, 90),	//ItemColorIndex_LightGolden	
		D3DCOLOR_ARGB(255, 233, 227, 1),	//ItemColorIndex_GodGolden
		D3DCOLOR_ARGB(255, 248, 144, 0),	//ItemColorIndex_Golden
		D3DCOLOR_ARGB(255, 240, 32, 24),	//ItemColorIndex_Red
		D3DCOLOR_ARGB(255, 102, 102, 102),	//ItemColorIndex_LightGrey	
};
const PSTRINGA TextColorStr[MaxTextColor] ={ 
		"O",
		"Y",
		"G",
		"A",
		"B",
		"V",
		"P",		
		"W",
		//"G",
		//"Bl",
		"H",
		"E",
		"X",
		"R",
		"C",
		};

struct DrawEntityTextureData;

struct emotionData
{
	UInt32 emo_id;
	UInt32 SrcID; 
	UInt32 TargetID;
	bool isPet;
	emotionData()
	{
		emo_id = 0;
		isPet = false;
	}

	emotionData(UInt32 giv_emo_id, UInt32 giv_SrcID, UInt32 giv_TargetID, bool giv_isPet)
	{
		emo_id = giv_emo_id;
		SrcID = giv_SrcID; 
		TargetID = giv_TargetID;
		isPet = giv_isPet;
	}
};

struct UIEffectData
{
	String	AniFileName;
	String	effectFileName;
	Int		PixelPosX;
	Int		PixelPosY;
	UIEffectData()
	{
		PixelPosX = 0;
		PixelPosY = 0;
	}
	~UIEffectData()
	{
		effectFileName.Empty();
		AniFileName.Empty();
	}
};

struct EffectData
{
	String	effectFileName;
	CAnimatedEntity	*	targetEnt;
	BYTE rotation;
	BOOL attach;
	UInt32 DeleteTime;
	Int OffsetX;
	Int OffsetY;

	EffectData()
	{
		targetEnt = NULL;
		rotation = 0;
		attach = TRUE;
		DeleteTime = 0;
		OffsetX = 0;
		OffsetY = 0;
	}
};

enum MESSAGEBOX_TYPE
{
	MESSAGEBOX_TYPE_YES_NO = 0,
	MESSAGEBOX_TYPE_CONFIRM,
	MESSAGEBOX_TYPE_CANCEL,
	MESSAGEBOX_TYPE_ARGREE_REFUSE,
	MESSAGEBOX_TYPE_ARGREE_REFUSE_CANCEL,
};
enum SPEAKER_TYPE
{
	SPEAKER_CHAR = 0,
	SPEAKER_MOB, 
	SPEAKER_PARTNER,
};
enum NUM_ENTITY_INDEX
{
	NUM_ENTITY_INDEX_VP_SUB,
	NUM_ENTITY_INDEX_DP_ADD,
	NUM_ENTITY_INDEX_SP_ADD,
	NUM_ENTITY_INDEX_VP_ADD,
	NUM_ENTITY_INDEX_DP_SUB,
	NUM_ENTITY_INDEX_SP_SUB,
};
enum ColorIndex
{
	ColorIndex_Orange = 0,
	ColorIndex_Yellow,
	ColorIndex_Green,
	ColorIndex_LightBlue,
	ColorIndex_Blue,
	ColorIndex_Violet,
	ColorIndex_LightPurple,
	ColorIndex_White,
	//ColorIndex_Golden,
	//ColorIndex_Black,
	ColorIndex_LightGrey,
};

enum SPEAKER_STATE
{
	SPEAKER_STATE_HIDE = 0,
	SPEAKER_STATE_INIT, 
	SPEAKER_STATE_OPENING, 
	SPEAKER_STATE_SHOW,
	SPEAKER_STATE_REFRESH,
	SPEAKER_STATE_CLOSING, 
};

typedef StlMap<BYTE, CChannelControl *, std::less<BYTE>, ManagedAllocator<std::pair<BYTE, CChannelControl *> > > BYTEChannelCtrlMap;

//all global data
class Global : public GlobalView
{
public:
typedef StlMap<Reuben::Network::ServiceID, Peer*, std::less<ServiceID>, 
			ManagedAllocator<std::pair<Reuben::Network::ServiceID, Peer*> > > MapPeers;
typedef StlMap<UInt32, MapDB_Client*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, MapDB_Client*> > > MapDBMap;
typedef StlMap<UInt32, NPCDB_Client*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, NPCDB_Client*> > > NpcDBMap;
typedef StlMap<UInt32, MobDB_Client*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, MobDB_Client*> > > MobDBMap;
typedef StlVector<BinaryClientNPCInstance*, ManagedAllocator<BinaryClientNPCInstance*> > NpcInstList;
typedef StlVector<BinaryClientJumppointInstance*, ManagedAllocator<BinaryClientJumppointInstance*> > JpInstList;
typedef StlVector<BinaryClientOBJInstance*, ManagedAllocator<BinaryClientOBJInstance*> > ObjInstList;
typedef StlVector<String, ManagedAllocator<String> > SpeakerList;
typedef StlMap<UInt16, CPartyCharInfoClient*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, CPartyCharInfoClient*> > > PartyCharInfoMap;

	LPDIRECT3DDEVICE9 pD3DDevice;
	CBattleControl * g_pBattleControl;
	CParticleSystem * g_ParticleEffectRain;
	CParticleSystem * g_ParticleEffectSnow;
	CParticleSystem * g_ParticleEffectFog;
	CParticleSystem * g_DisposalEntityEffect;
	CAnimatedEntity * NightMask;

	CMenuItemProcess*	g_pItemProcess;
	CMenuPartyProcess*	g_pPartyProcess;
	CCreateCharacterProcess* g_pCreateCharProcess;
	CMiddleMap*			g_pMiddleMap;
	CPartnerGroup*		g_pPartnerGroup;
	CMenuPartnerProcess* g_pPartnerProcess;
	CLoginProcess*		g_pLoginProcess;
	CMenuSearchListProcess*	g_pSearchListProcess;

	CMenuCharProcess*	g_pCharProcess;
	CMenuShopProcess*	g_pShopProcess;
	CRumorMsgProcess*	g_pRumorMsgProcess;
	CMenuGivenProcess*	g_pGivenProcess;
	CMenuBuffIconProcess*	g_pMenuBuffIconProcess;
	CMenuSystemSetting*	g_pSystemSetting;
	CBigMap*	g_pBigMap;
	CMenuTradingProcess* g_pTradingProcess;
	CMenuFriendProcess*	g_pMenuFriendProcess;
	CMenuHelpProcess*	g_pMenuHelpProcess;
	CMenuRankProcess*	g_pMenuRankProcess;
	CMenuPetProcess*	g_pMenuPetProcess;
	CMenuGuildProcess*	g_pMenuGuildProcess;
	CAntiHack*          g_pAntiHack;
	CMenuBankService*   g_pMenuBankService;	

	CMenuWareHouseProcess* g_pWareHouseProcess;
	CMenuAutoWalkProcess*	g_pAutoWalkProcess;
	CMenuTargetEquipProcess* g_pTargetEquipProcess;
	CMenuPartnerCultivationUpProcess* g_pPartnerCultivationUpProcess;

	bool Snowing;
	bool Raining;
	bool ShowCloud;
	bool ShowLeaf;
	bool ShowFog;
	bool Night;
	bool DisposalEffect;
	bool AddFootStep;
	bool ServerConnected;
	bool WindowShow; 
	bool RandomAccessBattle;
	bool m_AutoJumppointWalk;
	bool LeftFoot;
	bool BlockingActive;
	bool g_bOpenSubWindow;
	bool g_bCtrlButtonUp;
	bool g_NewGame;
	
	LPCPEER pPeerProxy;
	RPCContext g_context;

	UIMiniMap *MiniMap;
	UIStaticText	*PositionText;
	UIManager* pUIManager;

	DrawEntityTextureData CursorData;

	CAnimatedEntity * CursorEntity;
	bool ShowCursor;

	CCharacterControl * g_pCharacter;

	CMap* g_pMap;

	CMapControl * g_pMapView;

	CAccountControl * g_pAccountControl;
	PathFinder * g_PathFinder;

	FSound* ClickSnd;

	DefaultInfoHeader* m_mapdb_header;
//	DefaultInfoHeader* m_npcdb_header;
	DefaultInfoHeader* m_mobdb_header;
	DefaultInfoHeader* m_npclist_header;
	DefaultInfoHeader* m_objlist_header;
	DefaultInfoHeader* m_jplist_header;

	MapDB_Client* m_mapdb; // no need to store in memory; loaded on demand
	std::map<UInt32, NPCDB_Client*> m_npcdb; // should be stored in memory
	std::map<UInt32, MobDB_Client*> m_mobdb; // no big reason to store in memory
	NpcInstList m_npclist; // loaded on demand
	ObjInstList m_objlist; // loaded on demand
	JpInstList m_jplist; // loaded on demand

	//channel map list
	BYTEChannelCtrlMap m_MapChannelLst;

	//party list for this player
	CParty*			g_pParty;
	CParty*			g_pTempLeaveParty;
	StlVector<NetObject*,ManagedAllocator<NetObject*> > g_vctPartyMemberPointer;
	PartyCharInfoMap g_mapPartyChar;
	State*			g_CursorState;
	BOOL			g_ClickEnable;
	CClientResManager *m_resource;

	//Int32			m_nCharacterIndex;
	Int32Vector		m_LineExist;
	UInt16Vector	m_lineState;
	//Int32			m_nLineIndex;
	UInt32			m_nAccountID;
	UInt			m_logined;

	CItemBag*		m_pBag;

	CTrading*		g_pTrading;
	CHawkGroup*		g_pTargetHawkGroup;
	CHawkGroup*		g_pMyHawkGroup;
	CGiftBox*		g_pGiftBox;
	CMenuQuestJournal*	g_pMenuQuestJournal;

	CWeaponsRank*	g_pWeaponsRank;

	CTeamDescription*	g_pTeamDescription;

	CGenerationProcess* g_pGenerationProcess;

	CSellYuanBaoGroup* g_pSellYuanBaoGroup;

	CAccShopProcess* g_pAccShopProcess;
	CPointsShopProcess *g_pPointsShopProcess;

	BYTE LeftClickStatus;
	UInt32 LeftClickStatusCounter;
	BOOL LeftClickUpdate;

	//script system
	CScriptControl* g_pScriptControl;
	CBatScriptControl * g_pBatScriptControl;

	int g_SelectTextColorIndex;

	bool g_ClientistenToChannel[MAX_CHANNELTYPE]; 

	// autowalk
	UInt m_autoX;
	UInt m_autoY;

	String g_RecentPriChatChar;
	String g_ForceOutMsg;
	StlVector<String> g_RecentPriChatCharsLst;

	Int				m_PreClockCount;
	Int				m_NextClockCount;
	UInt32			m_ClockStartCount;
	UInt32			m_PreGameTimeInterval;
	UInt32			m_InitGameTimeInterval;
	UInt32			m_RemainCalcMS;
	SYSTEMTIME		m_TimeStartService;

	Int WinCursorCount;

	BOOL			g_bClickNPCFlag;
	UInt16			m_OpenDialoguePosX;
	UInt16			m_OpenDialoguePosY;

	String			TempMsgForUI;

	CAnimatedEntity * g_TempTargetEnt;

	BOOL ChangeMapFlag;

	CNumEnt			m_ChooseCountDown;
	CMenuCharacterSimulator*	g_pMenuCharSimulator;

	//table for additive and substractive color
	Byte AddtitiveTable[256][256];
	Byte SubtractiveTable[256][256];
	Byte BgDDSTable1[4][256][256];
	Byte BgDDSTable2[4][256][256];
	Byte AlphaOffsetTable[256][256];

	DrawEntityTextureData g_NightMaskTextureData;

	CMessageBox g_PKRequestBox;
	UInt32		g_TempPKTargetID;
	
	CMenuJoinBattleProcess * g_pJoinBattleProcess;
	Reuben::Math::Random m_rand;
	BOOL g_helpEnable;
	BOOL g_NoMoreHelp;

	CSkillGroup * g_SkillLst;
	CSkillGroup * g_ChangeModelSkillLst;
	CMenuSkillProcess * g_pSkillProcess;
	

	CQuestSystem * g_QuestSystem;
	CMenuQuestLogProcess * g_pQuestLogProcess;
	CMenuOnlineShopProcess* g_pOnlineShopProcess;
	CMenuMixProcess* g_pMenuMixProcess;
	CMenuTitleProcess* g_pMenuTitleProcess;
	CMenuHawkerProcess* g_pHawkerProcess;
	CMenuGiftBox*	g_pGiftBoxProcess;
	CMenuSellYuanBaoProcess* g_pMenuSellYBProcess;

	//StlVector<String> g_pastInputMsgHis;
	//Int	g_pastHisIndex;

	String	g_TempSkillString;

	lua_State *		g_pParentLua;
	lua_State *		g_pRobotLua;

	BOOL	g_bSkillFail;

	UInt32	g_dbClkDownTime;
	BOOL	g_DownButtonUp;

	Int		g_CurLineID;
	BOOL	g_IsChangeLine;

	UInt32 g_HorseLightCount;
	StringVector g_HorseLightMsgLst;
	UInt32Vector g_HorseLightCountLst;
	BOOL	g_HorseLightDisplay;

	CMessageBox g_QuestCancelBox;

	CMessageBox g_FriendChatBox;

	UInt8 g_u8LineIndex;

	CMessageBox g_GuildBox;

	CMessageBox g_QuestionBox;

	UInt8 g_NightMaskAlpha;
	UInt32 g_TimeMinute;
	BOOL g_DrawAll;
	BOOL g_MouseMove;

	BOOL g_ShopSkillFlag;

	CDirtyRectManager * g_AllDirtyRectMan;

	CMenuTargetProcess * g_MenuTargetProcess;

	double g_TempResult;

	CNewBieHintCtrl* g_pNewBieHintCtrl;

	UIShortCutNode	g_ShortCutSlot[10];

	UInt8 g_HideOtherPlayers;

	Int32	g_StartWndX, g_StartWndY, g_SubWndWidth;

	CPetGroup * g_PetLst;

	UInt32	g_OldScreenX, g_OldScreenY;
	bool	g_BPShortCutFlag;
	

	StlMultiMap<BYTE, emotionData> g_EmotionList;

	UInt32 g_PreSpeakTime[MAX_CHANNELTYPE];
	UInt32 m_downTime;			// services shutting down time

	bool m_FireworksMode;
	UInt32 m_FireworksModeRevertTime;

	bool g_hotKeyEnable;

	//partner Collection
	bool	m_bUsePartnerCollectionBuff;

	//speaker channel
	UInt32	m_uSpeakerDisappearTime;
	UInt32	m_uSpeakerLastRenderTime;
	UInt8	m_SpeakerState;
	SpeakerList	m_SpeakerLst;
	bool	m_bChangeLine;

	UInt32	g_balloonCount;

	//path find used for open npc dialog when path end
	UInt32 m_u32DestNpcId;

private:
	
	BYTE m_CurrChannelType;
	CGameStringTable * m_pStringTable;
	StringA m_CurrMusicFilename;

	UInt m_bgmVolume;
	UInt m_sfxVolume;

public:
	Int ImportClient_MapInfo();
//	Int ImportClient_NPCDB();
	NPCDB_Client* LoadNpcTemplate(UInt32 npcid);
	MobDB_Client* LoadMobTemplate(UInt32 mobid);
	Int ImportClient_ZoneData(UInt32 mapid);
	Int ImportClient_PFData(UInt32 mapid, PathFindLoadData& pPathFindData, UInt32& pNumGuide);
	Int ImportClientAllZoneData(PathFindLoadData& pPathFindData, UInt32& pNumGuide);
	BOOL ImportClient_Instance();
	VOID DestroyGameContext();
	BOOL FreeClientMapData();
	VOID SetCombinedBlock(PathFindLoadData& pPathFindData, UInt32 pNumGuide); // Reset blocking by reading all OBJ Blocking in Map
	BOOL SetPathFinderData(PathFinder &pf, WORD* block, Int width, Int height);

	BOOL CreateServer_MapPathFinder(PathFindLoadData& pPathFindData, UInt32 pNumGuide); // Run only after Map and Zone are read

	BOOL InitClientData();
	BOOL ReinitClientMapData(PathFindLoadData& pPathFindData, UInt32& pNumGuide);

	virtual VOID NotifyEnterSuccess(LPCPEER pPeer, RPCContext &context, NetObject * map, NetObject * character);
	virtual VOID NotifyEnterBattle(LPCPEER pPeer, RPCContext &context, NetObject * battle, NetGroup * grp, const UInt8 & PlayerTeam
		,const BYTE & battleState, const UInt32 & npcId);
	virtual VOID NotifyReceiveMessageChannel(LPCPEER pPeer, RPCContext &context, NetObject * channel, const BYTE &Type);
	virtual VOID NotifyReceivePrivateMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName
		, const String &TargetName);
	virtual VOID NotifyReceiveHorseLightMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &count);
	virtual VOID NotifyCB_JoinMessageChannel(LPCPEER pPeer, RPCContext &context, const BYTE &Result);
	virtual VOID NotifyLeaveSuccess(Boolean bDelete);
	virtual VOID NotifyNoMoreHelp();

	VOID NotifyCB_ReceiveEmotion(const UInt32 &emo_id, const String &SrcName, const String &TargetName, const bool &isPet);
	CChannelControl * GetChannelControl(BYTE Type);
	VOID LeaveChannel(BYTE Type);

	VOID NotifyCB_SendPrivateMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const boolean &isFriend, const UInt32 &resultFlag);

	VOID PrintMessage(String Msg, BYTE Type, String SrcName = "", String TargetName = "", BYTE speaker_type = SPEAKER_CHAR);
	VOID AddChatBalloon(String Msg, BYTE Type, String SrcName, Int Index = -1, UInt32 delay = 0, BYTE speaker_type = SPEAKER_CHAR);

	VOID InitChannelInfo();

	CChannelControl * GetCurrChannelControl();
	VOID SetCurrChannelType(BYTE Type);
	VOID RemoveChannelCtrl(CChannelControl * pChannelCtrl);

	NPCDB_Client* FindNPCByTemplateID(UInt32 TemplateID);


	VOID UpdateBattleItemMenu(Boolean bCheck = BOOLEAN_FALSE, UInt8 nbagindex = 0, bool wine = false);
	VOID UpdateBattleItemMenu(Int BagIndex);

	CCharacterControl * GetBattleController();

	String GetChatIconCodebyNum(UInt Code);

	String GetChatIconFilePathbyNum(UInt Code);
	String GetEmotionFilePathbyNum(UInt Code);


	VOID CloseQGL();
	VOID UpdateClockHandle();
	VOID UpdateClockPanel(Boolean bInit = BOOLEAN_FALSE);
	VOID UpdateRightUpTimeInfo();
	VOID MBChangeNextPage();

	VOID SetDisplayHP(const UInt16 &hp, const UInt16 &mhp, const UInt16 &maxhp);
	VOID SetDisplaySP(const UInt16 &sp, const UInt16 &msp, const UInt16 &maxsp);
	VOID SetDisplayDP(const UInt16 &dp, const UInt16 &maxdp);
	VOID SetDisplayWC(const UInt16 &wc, const UInt16 &maxwc, const UInt16 &WcExp, const UInt32 &NextWcExp);
	VOID SetDisplayVP(const UInt16 &vp, const UInt16 &maxvp);
	VOID AddDirtyUIItem(StringA component);
	VOID SetUIStaticText(StringA component, String text, BOOL addDirty = FALSE);
	VOID SetUITips(StringA component, String text);
	UILinerImage *SetUILinerImage(StringA component, UInt32 denumerator, UInt32 numerator);

	VOID SetAutoPos(UInt x, UInt y) {m_autoX = x; m_autoY = y;};
	UInt GetAutoX() {return m_autoX;};
	UInt GetAutoY() {return m_autoY;};

	VOID AddColorPrefix(String & Msg);

	void AddAlertBox(String msg, bool haveButton = true, UInt timeout = 0, UInt startCounter = 0);
	void CloseAlertBox();

	void UpdateChannelButtonState(int Index);
	inline BYTE GetCurrChannelType(){ return m_CurrChannelType;}

	CGameStringTable* GetStringTable() {return m_pStringTable;};

	UInt GetBGMVolume() {return m_bgmVolume;};
	UInt GetSFXVolume() {return m_sfxVolume;};

	VOID AddNameToRecentChatList(String Name);
	VOID PrintPlayerMessage(String Msg);

	VOID TestChangeMap(UInt16 MapID);
	VOID MisAdjust();
	VOID DisableJoinBattle();

	VOID ShowHisInput();

	VOID InitParentLua();
	VOID ReleaseParentLua();
	VOID InitRobotLua(const String &luaFile);
	VOID ReleaseRobotLua();
	VOID InitUILua(lua_State* &pState);
	VOID ReleaseUILua(lua_State* &pState);
	Boolean RunLua(lua_State* luaState, const String &filename);
	Boolean LoadLua(lua_State* lstate, const String &filename);

	StringA	GetPrefixStr(UInt16 ID, StringA pre);

	CAnimatedEntity GetCharModel(UInt8 sexClass, UInt16 head, UInt16 headColor, UInt32 cloth, UInt16 clothColor, UInt32 eq_headBand, UInt32 eq_weapon);

	VOID LoadLuaToParent(PCSTRINGA luafilename);

	VOID UpDateHorseLightMsg(UInt32 CurrTime);

	VOID UpdateNightAlpha();

	VOID MarkMouseDirtyRect();

	VOID ShowSkillEffectInMap(UInt16 skill_id, UInt32 srcID, UInt32 targetID);
	VOID ShowItemEffectInMap(UInt16 item_id, UInt32 srcID, UInt32 targetID);

	BOOL CanUsedName(String name);
	VOID ModifyMessage(String &message);
	VOID BlockColorWord(String &Msg);

	bool IsSuitableName(String name);
	void CheckDownTime(UInt32 dt);

	Color32 GetDisplayMoneyColor(UInt32 money);
	
	VOID UpdateSpeaker();

	void ClearShortCutSlotAll();
	Global()
		: pUIManager(NULL)
		, MiniMap(NULL)
		, PositionText(NULL)
		, g_pBattleControl(NULL)
		, g_ParticleEffectRain(NULL)
		, g_ParticleEffectSnow(NULL)
		, g_ParticleEffectFog(NULL)
		, g_DisposalEntityEffect(NULL)
		, NightMask(NULL)
		, Snowing(FALSE)
		, Raining(FALSE)
		, ShowCloud(FALSE)
		, ShowLeaf(FALSE)
		, ShowFog(FALSE)
		, Night(FALSE)
		, DisposalEffect(false)
		, AddFootStep(false)
		, LeftFoot(false)
		, ServerConnected(false)
		, WindowShow(TRUE)
		, RandomAccessBattle(false)
		, m_AutoJumppointWalk(false)
		, ShowCursor(NULL)
		, g_pCharacter(NULL)
		, g_pMap(NULL)
		, ClickSnd(NULL)
		, pD3DDevice(NULL)
		, BlockingActive(TRUE)
		, g_pMapView(NULL)
		, g_pAccountControl(NULL)
		, g_pParty(NULL)
		, g_pTempLeaveParty(NULL)
		, g_pPartnerGroup(NULL)
		, CursorEntity(NULL)
		, m_resource(NULL)
		, g_pScriptControl(NULL)
		, g_pBatScriptControl(NULL)
		, m_ClockStartCount(0)
		, m_PreClockCount(0)
		, m_NextClockCount(0)
		, m_PreGameTimeInterval(0)
		, m_InitGameTimeInterval(0)
		, m_RemainCalcMS(0)
		, WinCursorCount(0)
		, LeftClickUpdate(TRUE)
		, g_bClickNPCFlag(FALSE)
		, m_OpenDialoguePosX(0)
		, m_OpenDialoguePosY(0)
		, g_TempTargetEnt(NULL)
		, g_helpEnable(TRUE)
		, g_ClickEnable(TRUE)
		, g_SkillLst(NULL)
		, g_pSkillProcess(NULL)
		//, g_pastHisIndex(0)
		, g_NoMoreHelp(FALSE)
		, g_pShopProcess(NULL)
		, g_pParentLua(NULL)
		, g_bSkillFail(FALSE)
		, g_QuestSystem(NULL)
		, g_pQuestLogProcess(NULL)
		, g_dbClkDownTime(0)
		, g_DownButtonUp(FALSE)
		, g_CurLineID(-1)
		, g_IsChangeLine(FALSE)
		, g_NightMaskAlpha(255)
		, g_TimeMinute(0)
		, g_DrawAll(FALSE)
		, g_MouseMove(FALSE)
		, g_ShopSkillFlag(FALSE)
		, g_AllDirtyRectMan(NULL)
		, g_TempResult(0)
		, g_pMenuBuffIconProcess(NULL)
		, g_pNewBieHintCtrl(NULL)
		, g_HideOtherPlayers(0)
		, g_pSystemSetting(NULL)
		, g_StartWndX(0)
		, g_StartWndY(0)
		, g_SubWndWidth(150)
		, g_OldScreenX(0) 
		, g_OldScreenY(0)
		, g_pMenuPetProcess(NULL)
		, g_BPShortCutFlag(false)
		, m_downTime(0)
		, m_FireworksMode(false)
		, m_FireworksModeRevertTime(0)
		, g_hotKeyEnable(true)
		, m_bUsePartnerCollectionBuff(true)
		, m_SpeakerState(SPEAKER_STATE_HIDE)
		, m_uSpeakerDisappearTime(0)
		, g_NewGame(false)
		, m_bChangeLine(false)
		, g_balloonCount(0)
		, g_u8LineIndex(0)
		, g_ChangeModelSkillLst(NULL)
	{
	g_SelectTextColorIndex = -1;
	for (Int i = 0; i < MAX_CHANNELTYPE ; i ++)
	{
		g_ClientistenToChannel[i] = true;
		g_PreSpeakTime[i] = 0;
	}
	

		ZeroMemory(&m_TimeStartService, sizeof(SYSTEMTIME));
	}

	Boolean Initialize();

	VOID Uninitialize();

};


extern Global gGlobal;

INLINE Boolean IsFileExist(PCSTRINGA filename)
{
	DWORD attr = GetFileAttributesA(filename);
	return (attr != INVALID_FILE_ATTRIBUTES && attr != FILE_ATTRIBUTE_DIRECTORY);
}

UIManager* GetUIManager(VOID);

// private function
void SetUIStaticText(StringA component, String text, bool enabled = true);
void SetUILinerImage(StringA component, UInt32 denumerator, UInt32 numerator);
void SetUIEnable(StringA component, bool enabled);
void SetUIPos(StringA component, UInt posX, UInt posY);
BOOL GetUIPos(StringA component, UInt& posX, UInt& posY);
void SetUIItemVerticalPosAndEnable(StringA component, UInt curY, bool enabled);

// LUA function interface
void BattleAction();
void BattleSkill();
void BattleItem();
void BattleDefence();
void BattleFriend();
void BattleProtect();
void BattleCatch();
void BattleHelp();
void BattleAuto();
void BattleEscape();
void PartyPrivateCall(int memPos);
void PartyKick(int memPos);
void PartyPromote(int memPos);
void PartyQuit();
void UpdateCharMenu();
void PartySelectLineup();
void PartySetKeyPos(int iKeyPos);
void UpdateLineupSelectMenu();
void ForgetLearnedLineup();
void PartySetSelectLineupResualt();
void OpenLineupInfoView();
void LeaveTemporarily();
void ReturnParty();

void UpdateBattleItemMenu(bool wine = false);
void BattleItemUse(int itemPos);
void SetCharacterIndex(Int32 Index);
bool HasCharacter(Int32 Index);
void SetLineIndex(Int32 Index);
void EquipItemUse(int itemPos);


Int32 GetShortCutRefType(Int32 Index);
Int32 GetShortCutRefNum(Int32 Index);
void ClearShortCutSlot(Int32 Index);
void CopyToShortCut(int itemPos, int targetPos);
void MoveEquipToBag(int itemPos, int targetPos);
void MoveToBag(int itemPos, int bag, int target = -1);
void DropItem(int itemPos, int dropSize = 0);
void MBChangeNextPage();
void BatDialogNextPage();
void PlayerChooseResult(int opt);
void PlayerChooseResultInBat(int opt);
bool HasParty();
bool HasTempParty();
void UpdatePartyPanel();
void CopyImageForUIImage(PCSTRINGA src, PCSTRINGA des);
void BattleItemDrag(int itemPos);
bool HasItem(int itemPos);
void BattleItemMove(int SrcBagIndex, int DesBagIndex);
int GetMaxStack(int itemPos);
UInt32 GetBGMVolume();
UInt32 GetSFXVolume();

void SetDropItemText(int itemPos, PCSTRINGA srcStaticText);
void ShowErrMessage(PCSTRINGA pText, bool center = false);
bool InviteComing();
void ShowPartyByIndex(int index=-1);

void InviteConfirm(int index, bool confirm); // true: accept join, false: reject join
int GetPartyListSize();
void UpdateApplicantMenu();
void JoinConfirm(int index, bool confirm); // true: accept join, false: reject join

void BattleJoinConfirm(int index, bool confirm);
void JoinBattleClearAllRequestUp();
void JoinBattleScroll(int scroll);
void UpdateJoinBattleMenu();

void ClearAllRequest();
void ShowOtherParty(int next); //1: show next party, 0: show prev party
void ApplicantPanelScroll(int scroll); //1: scroll up, 0: scroll down
bool IsJoinRequestExist();
void ShowCreateCharacter(int sex = 0, int cclass = 1);

void ChangeCharacterHairCol(bool next); //true: next hair color, false:prev hair color
void ChangeCharacterHead(bool next);
void ChangeCharacterCloth(bool next);
void ChangeCharacterClothCol(bool next);

void CreateCharacter();
void SetHighLightInLogin(int index, bool highLight);
bool IsBattle();

bool IsFullScreen();
void LoadLua(PCSTRINGA luafilename);

void ShowMidMap();
void MidMapMoveCharTo(int index); //0: only move to, 1: move to jumppoint, 2: move to npc in npclist

void MidMapMoveOut();
void ResetMidMapValue();
void MidMapShowName(int index); //0: show npcName, 1: show jumppoint name
void MidMapClickNPC();
void UpdateSearchList(int index); //0: npc  1:char

void NPCSearchListUpOrDown(int up); //0: up, 1: down

void MidMapShowLocationBox();
void NPCSearchClickList(int index);
void SearchNPCList(int lang);//0: eng, 1: chinese word

void WeaponsRankSearchList(int lang);//0: eng, 1: chinese word


void PartySearchClickList(int index);
void CharSearchClickList(int index);
void ShowLeaderInfo();
void ShowLeaderInfoChar();

void ShowPartySearchList(int searchBy);
void ShowCharSearchList(int searchBy);
void OnPartySearchPagePrev();
void OnCharSearchPagePrev();
void OnPartySearchPageNext();
void OnCharSearchPageNext();

void OnJoinPartySearch();
void OnJoinCharSearch();

void OnShowPartyInfo();
void OnShowCharInfo();

bool IsIndoor();


void ConfirmPK();
void CancelPK();

void DeleteCharacter();

void NPCSearchListScroll();
void ResetSearchValue();

void UpdateShopItemDetail(int pos);
void ShopOpenConfirmBox(int pos = -1, int nMode = 2);
void ShopEnterItemCount(bool bSell, int nPos);
void ShopSetItemCount(bool bMax);
void ShopAdjustItemCount(bool bInc);
void ShopConfirmAction();
void ShopResetAction();
void ShopDragItemAction(int pos);
void ShopOnClickSellButton();
void ShopCloseMainMenu();
void ShopOnEditBoxKeyUp(int keyCode);

void InitLoginScreen();
void SaveLoginDefault();
UInt32 GetDefaultCharacter();
void SetDefaultCharacter(int pos);
UInt32 GetDefaultLine();
void SetDefaultLine(int pos);

void SetAniStateInLogin(int index, int state); //0: idle state, 1: battle state
void ShowPartyButton();

void BattlePartnerEscape();
bool IsBattlePartnerTurn();
void CheckCharacterName();



void TestingFunction();
void SelectItemPartner(UInt8 index);
void UpdateChangeLine();
void ApplicantPrivateCall(int index);
bool IsTalking();
void ChangeMapLine(UInt8 lineIndex);
void ConfirmChangeLineSec();

void TeammatePrivateCall(int index);
void TeammateKick(int index);
void TeammatePromote(int index);
void ItemScroll(int index); //0: up, 1: down, 2: scroll

void QuestCancelCancel();
void QuestCancelConfirm();
void ShowRemoveCharacterName();

Int32 GetShortCutRef(UInt32 Index);

bool GetHelpEnable();

bool IsPvP();
void SaveQGLState();
bool LoadQGLState();

bool CheckShiftDown();
UInt32 LuaGetChar_id();
PCSTRING LuaGetItemName(UInt32 itemPos);
UInt32 LuaGetItemColor(UInt32 itemPos);
UInt32 GetFirstNum(PCSTRINGW Str);
UInt32 GetLastNum(PCSTRINGW Str);
void LuaSetShopSkillFlag(bool flag);
bool LuaGetShopSkillFlag();
UInt32 LuaGetCharPosX();
UInt32 LuaGetCharPosY();
bool CharSetMapPos(PCSTRINGW Str);
bool IsHawking();
bool IsPetUse();
void ShowPartyPopUp(UInt8 index);
void SelectPartyPopUp(UInt8 popSelect, UInt8 index);
void RunNewGame();
void DoRunNewGame();
void ShowTeammatePopUp(UInt8 index);
void AutoAcceptCheckBox(bool on);
void SelectTeammatePopUp(UInt8 popSelect, UInt8 index);
void SetChangePos(UInt8 index);
void ChangePartyDesc();
void StopCharacter();
PCSTRINGW GetLocalStr(const StringA &s);
PCSTRINGW GetLocalStr(PCSTRINGA s);

UInt32 CheckMyProcess(HWND hWnd);
void ShowAlertOK();
void CreateCharRotate(bool left);

BOOL InstallFirstHook();
BOOL UnHookFirst();
BOOL InstallHook();
BOOL UnHook();
UInt GetFontColorIndexByMoney(UInt32 gmoney);
Color32 GetColorByMoney(UInt32 gmoney);
void SetTextMoneyColor(UIStaticText* gtext,UInt32 gmoney);
void SetEditBoxMoneyColor(UIEditBox* geditbox);

void OpenWebLink();
void SetWebLink(PCSTRINGA link, int typeno);
void CharEatFastFood(UInt8 foodType);

void SetQGLTitle(PCSTRING strCaption, UInt32 char_id, 
	PCSTRING strNickname, PCSTRING strServerName, UInt8 lineId, UInt8 mapType
	);

#endif // _GLOBAL_H_

