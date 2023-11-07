#pragma once
#ifndef _QGL_APPLICATION_H_
#define _QGL_APPLICATION_H_

#include "Global.h"
#include "QGLWindow.h"
#include "XMLPanel.h"
#include "PlayOggVorbis.h"
#include "common_binary.h"
#include "AppMapState.h"
#include "Proxy/Logic/Account.h"
#include "Proxy/Logic/Map.h"
#include "Proxy/Logic/Character.h"
#include "Proxy/Service/MapService.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"

#include <Reuben\Utility\Singleton.h>
#include <Reuben\Platform\Application.h>
#include <Reuben\Utility\ProcessManager.h>
#include <Reuben\Simulation\ScheduleManager.h>
#include <Reuben\Network\Network.h>
#include <Reuben\Network\Peer.h>
#include <Reuben\Diagnostics\Logger.h>
#include <Reuben\Diagnostics\Trace.h>

#include "LoginProcess.h"

class CCharacterControl;
struct SkillData;

enum SubWindowAlignType
{
	WINDOWALIGN_RIGHT = 0,
	WINDOWALIGN_LEFT,
	WINDOWALIGN_TOP,
	WINDOWALIGN_DOWN,
};
struct SubWindowStr
{
	CRender * pRender;
	HWND	m_SubhWnd;
	RECT WindowRect;
	//LPDIRECT3DTEXTURE9 BgTexture;
	DrawEntityTextureData BgTextureData;
	UIManager *m_UIManager;
	BYTE m_AlignType;
	BOOL SizeChange;
	Int PreMouseX;
	Int PreMouseY;
	BOOL ShowCursor;
	CAnimatedEntity Cusor;

	DrawEntityTextureData CusorTextureData;

	SubWindowStr()
	{
		pRender = NULL;
		m_SubhWnd = NULL;
		//BgTexture= NULL;
		m_UIManager = NULL;
		SetRect(&WindowRect ,0,0,0,0);
		m_AlignType = WINDOWALIGN_RIGHT;
		SizeChange = FALSE;
		PreMouseX = 0;
		PreMouseY = 0;
		ShowCursor = FALSE;
	}


	LRESULT CALLBACK WndProc(  HWND  hWnd, 
              UInt  uMsg, 
              WPARAM  wParam, 
              LPARAM  lParam );

};

struct DisposalEffectData
{
	UInt32 PreUpdateTime; //starting time
	UInt32 EndTime; //ending time
	UInt32 TimeToProduce; //time to produce one effect
	UInt LifeTime; //lift time of one effect
	Byte StartAlpha; //starting alpha value of one effect
	Int DisposalTime; //alpha disposal time if any
	CAnimatedEntity * pEnt; //target entity
	UInt32 TimePass; //time pass

	DisposalEffectData()
	{
		memset(this, 0, sizeof(DisposalEffectData));
	}
};

typedef StlMap<UInt32, CCharacterControl *, std::less<UInt32>,
	ManagedAllocator<std::pair<UInt32, CCharacterControl *> > > UInt32CharCtrlPtrMap;

class QGLApplication
	: public Reuben::Platform::Application<QGLApplication>
	, public Reuben::Utility::Singleton<QGLApplication>
	, public Reuben::Network::ISocketHandler
{
public:

	//map state
	State*		m_mapState;

private:

	UInt32 m_manage_interval;

	Reuben::Utility::ProcessManager m_ProcessManager;

	QGLWindow m_Window;

	//map data in this rendering class
	CMapObjectView * m_CurrMapView;

	CCamera  m_CurrCam;
	//particle system list in this class
	StlVector<CParticleSystem *> pParSysLst;

	//background map texture for battle mode, it should be system memory
	LPDIRECT3DTEXTURE9 m_BattleBgTexture;

	//Mask texture it should be managed texture
	MaskTextureData m_MaskTextureData;

	//Mask Entity(e.g night mask)
	CAnimatedEntity * m_pMaskEntity;

	BOOL m_RenderMap;

	StlMultiMap<UInt32, CEntityBase *>  m_EntRenderLst;
	StlMap<CEntityBase *,  CMapActorControl *>	m_RenderCMapCtrlLst;

	//disposal effect
	StlMultiMap<UInt, DisposalEffectData> m_EntityDisposalLst;

	UInt32CharCtrlPtrMap m_MapCharacterLst;

	//test player entity list
	StlVector<CEntityBase *> TestPlayerLst;

	//back up entity for battle mode
	StlVector<CEntityBase *> TempEntityLst;

	BOOL m_ListenToPrivateMessage;
	
	BOOL m_ListenToFriendMessage;
	//Sub window
	SubWindowStr m_SubWindow;

	UInt32 m_prevwalktime;

	CMapActorControl * m_SelectedMapControl;
	CMapActorControl * m_TalkingMapControl;
	CMapActorControl * m_ClickNPCMapControl;

	//current ZValue
	UInt32  ZValue;

	CAnimatedEntity m_LoginEnt;

	UInt32	m_TotalDelTime;

	StlMultiMap<UInt32, EffectData> m_EffectDataLst;

	StlMultiMap<UInt32, UIEffectData> m_UIEffectDataLst;

	StlMultiMap<UInt32, String> m_SoundDataLst;

	CParticleSystem * m_MapParticle;

private:

	INLINE QGLApplication(VOID)
		: m_manage_interval(C_UINT32(0))
	{}
	SINGLETON(QGLApplication);

public:

	CMapActorControl* GetSelectedMapControl() {return m_SelectedMapControl;}

	UInt32CharCtrlPtrMap GetMapCharacterLst(){ return  m_MapCharacterLst;}
	INLINE CMapActorControl * GetSelectedControl(){ return m_SelectedMapControl;}
	INLINE CMapActorControl * GetTalkingMapControl() { return m_TalkingMapControl;}
	INLINE VOID SetTalkingMapControl(CMapActorControl* ctrl) { m_TalkingMapControl = ctrl;}
	INLINE VOID SetClickNPCMapControl(CMapActorControl* ctrl) { m_ClickNPCMapControl = ctrl;}

	//inline StlVector<CEntityBase *> GetRenderEntityLst(){ return pEntityLst;};
	INLINE CMapObjectView * GetRenderMap() { return m_CurrMapView;};
	INLINE StlVector<CParticleSystem *> GetParSysLst() { return pParSysLst;};

	//add entity to Z-Buffer list to sort their order
	BOOL AddEntityToLst(CEntityBase * pEnt, StlMultiMap<UInt32, CEntityBase *> & EntRenderLst);

		//get background texture
	INLINE LPDIRECT3DTEXTURE9 GetBattleBgTexture(){ return m_BattleBgTexture;};

	//set background texture
	INLINE void SetBgTexture(LPDIRECT3DTEXTURE9 GivenBgTexture){ m_BattleBgTexture = GivenBgTexture;};

	//remove background texture
	INLINE void ResetBgTexture(){ m_BattleBgTexture = NULL;};

	//get mask texture
	INLINE LPDIRECT3DTEXTURE9 GetMaskTexture(){ return m_MaskTextureData.MaskTexture;};

	//set mask texture
	VOID CreateMaskTexture(Byte * GivenMaskData, UInt Width, UInt PixelSize, RECT  SrcLockRect, Byte MaskTextureType = MASKTEXTURETYPE_NO_EFFECT,
		UInt32 FadeInOutTime = 0);

	//setter and getter of Mask Entity
	INLINE void SetMaskEntity(CAnimatedEntity * GivenEntity){m_pMaskEntity = GivenEntity;};

	INLINE void RemoveMaskEntity(){m_pMaskEntity = NULL;};

	INLINE CAnimatedEntity * GetMaskEntity(){return m_pMaskEntity;};

	//getter and setter of the render map flag
	INLINE void SetRenderMapFlag(BOOL Value){ m_RenderMap = Value;}
	INLINE BOOL GetRenderMapFlag(){ return m_RenderMap;}

	Boolean OnCreate(VOID);
	VOID OnDispose(VOID);
	VOID OnFatalError(Exception& ex);

	VOID OnUpdate(UInt32 uTimeDelta);
	INLINE QGLWindow& GetWindow(VOID);

	Boolean Start(VOID);
	Boolean TryConnect(VOID);
	Boolean Stop(VOID);

	virtual VOID OnAccept(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnFull(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener);
	virtual VOID OnOpen(CONST Listener* listener, CONST Peer* pPeer);
	virtual VOID OnClose(CONST Listener* listener);
	virtual VOID OnClose(CONST Listener* listener, CONST Peer* pPeer);

	//init the particle system
	CParticleSystem * InitSnow( CRender * pRender);
	CParticleSystem * InitRain( CRender * pRender);
	CParticleSystem * InitFog( CRender * pRender);

	//update the game loop
	VOID UpdateGameLoop(CRender * pRender);
	VOID UpdateShortCutKey();

	VOID LeaveBattle();
	VOID EnterBattle();

	VOID ChangeBattleUI();

	VOID ChangeMap(LPCTSTR filename, UInt X, UInt Y);
	VOID ReadChangeMapInfo();
	BOOL MergeOBJBlockingToMap(CMapObjectView& objview, WORD* outputblock);

	VOID AddTestPlayer();
	VOID RemoveTestPlayer();

	VOID DrawScene();
	VOID ResetMapData();

	VOID SelectEntity(Int PosX, Int PosY);

	//add and remove particle systems
	VOID AddParticleSystem(CParticleSystem * pParSys);
	VOID RemoveParticleSystem(CParticleSystem * pParSys);

	void InsertDisposalEffect(DisposalEffectData Data);

	//jump the entity
	VOID JumpEntity(CAnimatedEntity * pEnt);

	//update the left click event
	VOID UpdateLeftClick(LONG x, LONG y);

	VOID UpdateNormalClick(LONG x, LONG y);
	VOID UpdatePartyClick(LONG x, LONG y);
	VOID UpdateBattleClick(LONG x, LONG y);
	VOID UpdateNPCClick(LONG x, LONG y);
	VOID TalkToNpcByPathFinder(CMapNPCControl* pMapNpc);
	VOID UpdatePKAttackClick(LONG x, LONG y);
	VOID UpdateBattleCatchClick(LONG x, LONG y);
	VOID UpdateShopSellClick(LONG x, LONG y);
	VOID UpdateGivenClick(LONG x, LONG y);
	VOID UpdateTradingClick(LONG x, LONG y);
	VOID UpdateFriendClick(LONG x, LONG y);
	VOID UpdateGenerationClick(LONG x,LONG y);
	

	VOID OnClickNPC();
	VOID ResumeClickNPCAction(UInt16 x, UInt16 y);
	VOID OnDisappearNPCDialogue();
	VOID CountDownForPartyChoose();
	VOID UpdateWholeMemberClickNPC(String filename);
	
	VOID UpdateNPCtalkingClick(LONG x, LONG y);
	VOID UpdateProtectClick(LONG x, LONG y);

	VOID UpdateBattleUseItemClick(LONG x, LONG y, Boolean bCancel = BOOLEAN_FALSE);

	VOID UpdateBattleSkillClick(LONG x, LONG y);

	//update the right click event
	VOID UpdateRightClick(LONG x, LONG y);

	//change the viewing target
	VOID ChangeViewingTarget(CEntityBase * pEnt = NULL, BOOL MoveImm = TRUE);

	//change battle mode
	VOID ChangeBattleMode();

	inline CRender * GetRenderClass(){ return m_Window.GetRenderPtr();}

	CMapActorControl * FindMapActCtrlInWindow(Int PosX, Int PosY, Int MapWidth);

	CCharacterControl * FindCharCtrlByNetObject(CCharacter * pChar);
	CCharacterControl * FindCharCtrlByChar_id(UInt32 Char_id);
	CCharacterControl * FindCharCtrlByName(String Name);
	CCharacterControl * FindCharCtrlByNameInBattle(String Name, BYTE speaker_type);
	CMapActorControl * FindMapActorCtrlByNetID(RPCNetID ID);
	CMapNPCControl * FindNPCCtrlByNPCTemplateID(UInt32 NPCTemplateID);
	CMapNPCControl * FindNPCCtrlByID(UInt32 NPCID);
	CMapNPCControl* FindNPCCtrlByBaseID(UInt32 uNpcId);
	VOID DeleteCharacter(CCharacterControl * pCharCtrl);
	VOID DeleteAllCharacters();
	VOID ResetAllCharactersNetObject();

	//remove the net object
	VOID DeleteCharacterByNetObject(CCharacter * pChar);

	VOID AddCharCtrlToMap(CCharacterControl * pCharCtrl);
	VOID RemoveCharCtrlFromWorld(CCharacterControl * pCharCtrl);

	BOOL IsValidTargetType(UInt8 targetType, CCharacterControl * pSrcCtrl, CCharacterControl * pTargetCtrl);

//	BOOL LoadPathFindData(PathFinder &pf, StringA filename);
//	BOOL SetPathFinderData(PathFinder &pf, const StlVector<ClientMapZoneData*, ManagedAllocator<ClientMapZoneData*> > &block, Int width, Int height);
	VOID RemoveEntityInLst(CEntityBase * pEntBase);

	ApplicationStateEnum GetMapStateID();

	VOID AddNumberEnt(CEntityBase * pTarget, PixelDirection MoveDir, Int LiveBase, Int MoveSpeed,
								  Int GivenDisposalTime, Int GivenWaitTime, BOOL Destory, Int64 ZValue, Int NoOfParticle,
								  Int Num, BYTE AddSymbolIndex, CParticleSystem * pParSys, BOOL JumpTogether, String FileName, Int JumpHeight,
								  BOOL Shake, UInt32 GivenStartDelay, String SymbolFileName );

	VOID ClearMessage();
	//test change color
	VOID ChangePlayerColor256();

	//change map ui
	VOID ChangeMapUI();

	//init the mini map
	VOID InitMiniMap(String filename);

	//send the message
	void SendMessage();

	CCamera * GetRenderCamera(){ return &m_CurrCam;}

	VOID SetListenToPrivateMessage(BOOL Listen);
	BOOL GetListenToPrivateMessage(){return m_ListenToPrivateMessage;}

	VOID SetListenToFriendMessage(BOOL Listen);
	BOOL GetListenToFriendMessage(){return m_ListenToFriendMessage;}
	//open and close subwindow
	VOID OpenSubWindow();
	VOID CloseSubWindow();

	BOOL IsOpenSubWindow(){return m_SubWindow.pRender != NULL;}

	//draw sub window
	VOID DrawSubWindow();

	VOID ResetSubWindowPos();

	BOOL SendMessageByChannelType(String OrgMsg, BYTE Type, String Command);
	
	VOID ResizeSubWindow();


	SubWindowStr *GetSubWindow()
	{
		return &m_SubWindow;
	}

	VOID RemoveMapActorRenderLst(CMapActorControl * pMapCtrlActor);

	//party function
	VOID CreateParty();
	void JoinParty(UInt charid);
	void InviteParty(UInt charid);
	void DisplayPartyInfo();

	//check follower status
	BOOL CheckEntityFollower(CEntityBase * pEntBase, UInt32 CurrTime, UInt32  ParentPreMoveTime);

	VOID AddMapCtrlToRenderLst(CMapActorControl * pMapCtrlActor, bool showFlag = true);

	VOID DrawUITexture();

	VOID TestDirtyRect();

	VOID AddLeftClickEntity();

	VOID MoveToScreenPos(LONG x, LONG y, BOOL SetDes);

	VOID UpdateSceneData();

	VOID AddTestImage();

	CCharacterControl * FindChatCtrlByName(String Name);

	BOOL CheckSubWindowAlign();

	VOID SetUILinerImage(StringA component, UInt32 denumerator, UInt32 numerator);

	VOID RestoreSubWindowData();

	VOID TestShake();

	VOID ReleaseNightData();
	VOID CreateNightData();

	VOID AddAllDirtyRect();

	BOOL IsValidAttackTarget(CCharacterControl * pSrcCtrl, CCharacterControl * pTargetCtrl);

	bool IsValidCatchTarget(CCharacterControl * pSrcCtrl, CCharacterControl * pTargetCtrl);

	VOID AddEffectToLst(UInt32 startTime, EffectData effectData);
	VOID AddUIEffectToLst(UInt32 startTime, UIEffectData uiEffectData);
	VOID AddSoundToLst(UInt32 startTime, String soundData);

	VOID AddParticle(LPCTSTR filename, Int LiveBase, BOOL Destory, Int NoOfParticle,
									   CEntityBase * pTargetEntity, BOOL SetPartent, Int64 ZValue, Int Rotation, BOOL Free, Int OffsetX, Int OffsetY);
	VOID LookAtMousePos(Int x, Int y);

	VOID PlayFireWorks(String word, CCharacterControl * pTarget);

	VOID RemoveEntFromDisposalList(CAnimatedEntity * pEnt);



private:

	VOID UpdateEffectDataLst(UInt32 CurrTime);
	VOID UpateSoundDataLst(UInt32 CurrTime);
	VOID UpdateDisposalEffect(UInt32 CurrTime);
	
	VOID UpdateMaskTexture(UInt32 CurrTime);

	VOID ResetUpdatedStatus();

	VOID ResourcesUpdate();

	//update entity move data
	VOID UpdateEntityMove(CEntityBase * pEnt, UInt32 CurrTime);

	//init entity data
	VOID InitEntity();
	VOID InitBattle();
	
	VOID RandomAccessBattle(CEntityBase * pEntBase);
	VOID AddFootStep(CEntityBase *pEntBase);

	//update map scene data
	VOID UpDateMapScene(UInt32 CurrentTime);

	//update character scene data
	VOID UpDateCharScene(UInt32 CurrentTime);

	//update map data
	VOID UpDateParSysScene(UInt32 CurrentTime);

	//update mask entity
	VOID UpDateMaskEntScene(UInt32 CurrentTime);

	BOOL ConnectServer();

	VOID UpdateBattleScene(UInt32 CurrentTime);

public:	//For Lua

	VOID Login();
	Boolean		m_bWaitLogin;
};

INLINE QGLWindow& QGLApplication::GetWindow(VOID)
{
	return (m_Window);
}

#endif // _QGL_APPLICATION_H_
