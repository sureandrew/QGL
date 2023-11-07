//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "QGLApplication.h"
//-- Library
#include "Math.h"
#include "Image.h"
#include "Font.h"
#include "UI.h"
#include "Common/PathFinder.h"
#include "Common/Procedure.h"
#include "Common/CallLuaFunc.h"
#include "Global.h"
#include "Configure.h"
#include "FontEntity.h"
#include <Reuben/Simulation/ScheduleManager.h>
#include <fstream>
#include <iostream>
#include "CharacterControl.h"
#include "AccountControl.h"
#include "BattleControl.h"
#include "Proxy/Logic/BatActor.h"
#include "Proxy/Logic/BatMob.h"
#include "Proxy/Logic/BatCharacter.h"
#include "RPCEnum/BatCharacter_enum.h"
#include "RPCEnum/BatMob_enum.h"
#include "Proxy/Logic/item.h"
#include "Proxy/Logic/battle.h"
#include "TextEntManager.h"
#include "ChannelControl.h"
#include "Proxy/Service/MessageService.h"
#include "Proxy/Service/WorldService.h"
#include "Common/ChannelType.h"
#include "Proxy/Logic/Party.h"
//#include "Proxy/Logic/ScriptSystem.h"
#include "ScriptControl.h"
#include "BatScriptControl.h"
#include "ImageEntity.h"
#include "UIEntity.h"
#include "PathMove.h"
#include "MenuItemProcess.h"
#include "MenuShopProcess.h"
#include "Proxy/Logic/ItemBag.h"
#include "MiddleMap.h"
#include "MenuSkillProcess.h"
#include "MenuQuestLogProcess.h"
#include "LuaPush.h"
#include "MenuSearchListProcess.h"
#include "Resource/ResSkill.h"

#include "MenuPartnerProcess.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Proxy/Logic/QuestSystem.h"
#include "Proxy/Logic/Messenger.h"


#include "RumorMsgProcess.h"
#include "MenuGivenProcess.h"
#include "BigMap.h"


#include "MenuTargetProcess.h"
#include "MenuBuffIconProcess.h"
#include "Resource/ResBuff.h"
#include "MenuSystemSetting.h"
#include "MenuFriendProcess.h"
#include "MenuTradingProcess.h"
#include "MenuHelpProcess.h"
#include "MenuOnlineShopProcess.h"
#include "MenuPetProcess.h"
#include "MenuMixProcess.h"
#include "MenuTitleProcess.h"
#include "MenuHawkerProcess.h"
#include "MenuCharProcess.h"
#include "MenuGuildProcess.h"
#include "MenuGiftBox.h"
#include "QuestionProcess.h"
#include "AntiHack.h"
#include "MenuWareHouseProcess.h"
#include "MenuAutoWalkProcess.h"
#include "MenuSellYuanBaoProcess.h"
#include "MenuTargetEquipProcess.h"
#include "RelationSystemProcess.h"
#include "PointsShopProcess.h"
#include "WantedProcess.h"
#include "Proxy/Logic/PetGroup.h"
#include "Proxy/Logic/GuildMember.h"
#include "MenuPartnerCultivationUpProcess.h"
#include "CrossMapPathFinder.h"

#include "MapNPCControl.h"
#include "Version.h"

#include <WindowsX.h>
#include <CommCtrl.h>

//const UInt32 LOCKFRAME = 16;

const WORD TILE_IN_NORMAL_VALUE = 0x0000;
const WORD TILE_IN_BLOCK_VALUE = 0x0001;
const WORD TILE_IN_ALPHA_VALUE  = 0x0002;
const WORD TILE_IN_SHADOW_VALUE = 0x0004;
const WORD TILE_IN_SEADEPTH_VALUE = 0xFF00;
const WORD TILE_IN_FOOTPRINT_VALUE = 0x0010;
const WORD TILE_IN_NAVIGATION_VALUE = 0x0020;

#define MAX_PARTY_NUM 5
#define MAX_EMOTION	5
#define QGL_CONNECT_TIMEOUT		500

struct PeerData
	: public Reuben::Network::FilterData
{
	virtual VOID OnDispose(VOID)
	{
		PeerData* pData = const_cast<PeerData*>(this);
		SafeDeleteObject(pData);
	}
};

VOID GetPlayerPos(int &X, int &Y)
{
	PixelPos Pos =  gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos();

	X = Pos.PixelPosX;
	Y = Pos.PixelPosY;
}

UInt32 GetMapWidth()
{
	return QGLApplication::GetInstance().GetRenderMap()->GetMapWidth();
}

UInt32 GetMapHeight()
{
	return QGLApplication::GetInstance().GetRenderMap()->GetMapHeight();
}

VOID AddXMLFile2UIManger(PCSTRINGA Filename)
{
	XML_UI::GetInstance().AddFormFile(Filename, gGlobal.pUIManager);
}

void UIMangerLoadUI(UIManager *pUIManager, PCSTRINGA Filename)
{
	UINameList::GetInstance().LoadUIFile(pUIManager, Filename);
	//pUIManager->LoadLuaUI(UINameList::GetInstance().UIName(Filename));
}

VOID QGLApplication::TestShake()
{
	Int ShakeX = GetPrivateProfileInt(_T("SHAKE"), _T("SHAKEX"), 0, CONFIGURATION_INI_FILE);
	Int ShakeY = GetPrivateProfileInt(_T("SHAKE"), _T("SHAKEY"), 0, CONFIGURATION_INI_FILE);
	Int ShakeSpeedX = GetPrivateProfileInt(_T("SHAKE"), _T("SHAKESPEEDX"), 0, CONFIGURATION_INI_FILE);
	Int ShakeSpeedY = GetPrivateProfileInt(_T("SHAKE"), _T("SHAKESPEEDY"), 0, CONFIGURATION_INI_FILE);
	Int ShakeWidth = GetPrivateProfileInt(_T("SHAKE"), _T("SHAKEWIDTH"), 0, CONFIGURATION_INI_FILE);
	Int ShakeHeight = GetPrivateProfileInt(_T("SHAKE"), _T("SHAKEHEIGHT"), 0, CONFIGURATION_INI_FILE);
	Int ShakeTime = GetPrivateProfileInt(_T("SHAKE"), _T("SHAKETIME"), 0,CONFIGURATION_INI_FILE);

	gGlobal.g_pCharacter->GetCharMainView()->SetShakeInfo(ShakeX, ShakeY, ShakeSpeedX,ShakeSpeedY, ShakeWidth, ShakeHeight, ShakeTime);
}
VOID QGLApplication::InitEntity()
{

	CAnimatedEntity * NPCEnt = SafeCreateObject(CAnimatedEntity);
	
	NPCEnt->LoadEntityFile(gConfigure.strNPC.c_str(), gGlobal.pD3DDevice);
	NPCEnt->SetCharPos(gConfigure.uNPCPosX, gConfigure.uNPCPosY);

	CAnimatedEntity * ShadowNPC = SafeCreateObject(CAnimatedEntity);
	
	ShadowNPC->LoadEntityFile(gConfigure.strShadowName.c_str(), gGlobal.pD3DDevice);
	
	ShadowNPC->SetEntParent(NPCEnt);
	ShadowNPC->SetExtraZBuffer(-1);
	ShadowNPC->SetEntityType(ENTITY_TYPE_PARTICLE_WITHEFFECT);

	m_CurrMapView->AddEntityToMap(NPCEnt);
	m_CurrMapView->AddEntityToMap(ShadowNPC);
	
}

BOOL QGLApplication::ConnectServer()
{
	LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();
	Reuben::Simulation::ScheduleManager &schmgr = Reuben::Simulation::ScheduleInstance::GetInstance();
	rpcmgr.GetModel().SetSocketHandler(this);
	rpcmgr.SetServiceId(0);

	m_ProcessManager.Attach(&rpcmgr);
	m_ProcessManager.Attach(&schmgr);

	if (gGlobal.pPeerProxy)
	{
		rpcmgr.GetModel().CloseSocket(gGlobal.pPeerProxy->GetSocketID());
		SafeRelease(gGlobal.pPeerProxy);
	}
	
	Boolean bConnect = false;
	for( UInt32 i = 0; i < gConfigure.uProxyCount && !bConnect; ++i )
	{
		UInt32 startTime = ::timeGetTime();
		while (::timeGetTime() - startTime < QGL_CONNECT_TIMEOUT && !bConnect)
		{
			gGlobal.pPeerProxy = LocalRPCManager::GetInstance().GetModel().Connect(Address(gConfigure.ipProxy[i], gConfigure.portProxy[i]), Reuben::Network::PROTOCOL_TCP, BOOLEAN_FALSE);
			if (gGlobal.pPeerProxy)
			{
				bConnect = BOOLEAN_TRUE;
				PeerData* pData = SafeCreateObject(PeerData);
				gGlobal.pPeerProxy->SetData(pData);
				UInt32 version = VERSION_NUMBER;
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("version sent %d"), version);
				LocalRPCManager::GetInstance().GetModel().Send(gGlobal.pPeerProxy, (CONST Byte*)&version, sizeof(UInt32));
			}

			Sleep(100);
		}
	}
	
	if (!gGlobal.pPeerProxy)
	{
		// Please handle a interface here. after that remove below throw exception code.
		//MessageBox(m_Window.GetWindow(),gGlobal.GetStringTable()->Get(_T("MSG_CONNECT_SERVER_FAIL"), NULL, MB_OK);
		
		return FALSE;
	}
	return TRUE;
}
VOID QGLApplication::InitBattle()
{
	if (gGlobal.g_pBattleControl == NULL)
	{
		gGlobal.g_pBattleControl = SafeCreateObject(CBattleControl);
		gGlobal.g_pBattleControl->InitBattleData(GetRenderClass(), gConfigure.strBattleFileName.c_str());
	}

	CONST Int MaxBattleEnt = 20;
	CONST Int BattlePos[MaxBattleEnt][2] = {{11,6}, {7,6}, {15,6}, {3,6},{19,6},{11,10},{7,10}, {15,10}, {3,10},{19,10},
	{15,28},{11,28}, {19,28}, {7,28},{23,28},{15,32},{11,32}, {19,32},{7,32},{23,32}};
	
	for (Int i = 0; i < MaxBattleEnt; i++)
	{
		CCharacterControl * BattleEnt = SafeCreateObject(CCharacterControl);
		//CCharacter * pChar = SafeCreateObject(CCharacter);
		//BattleEnt->SetMapActorObject(pChar);
		BattleEnt->GetCharMainView()->SetEntityType(ENTITY_TYPE_BATTLE_PLAYER);
		BattleEnt->GetCharShadowView()->SetEntityType(ENTITY_TYPE_BATTLE_PLAYER);
		
		if (i == 7)
		{
			*BattleEnt->GetCharMainView() = *gGlobal.g_pCharacter->GetCharMainView();
			BattleEnt->GetCharMainView()->m_pChildrenEntLst.clear();
			BattleEnt->GetCharNameView()->SetEntParent(BattleEnt->GetCharMainView());
		}
		else if (i % 2 == 0)
			BattleEnt->LoadDefaultMob(gConfigure.strBattleEntName2, String());
		else
			BattleEnt->LoadDefaultMob(gConfigure.strBattleEntName, String());

		BattleEnt->InitShadow();
		StringA strTemp = gConfigure.strFontStyle.c_str();
		BattleEnt->GetCharNameView()->LoadText(gConfigure.strCharName.c_str(), 
			strTemp.c_str(), gConfigure.uNameFontSize, D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
			gConfigure.uNameFontColorG, gConfigure.uNameFontColorB), gConfigure.uBattleFontSyle);

		if ( i == 7)
		{
			gGlobal.g_pBattleControl->AddCharacterToBattleLst(BattleEnt, gGlobal.pD3DDevice, 
			BattlePos[i][0], BattlePos[i][1], gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MONITOR, NULL);
			gGlobal.g_pBattleControl->NotifyControlCharacter(NULL, gGlobal.g_context, 7);
		}
		else
			gGlobal.g_pBattleControl->AddCharacterToBattleLst(BattleEnt, gGlobal.pD3DDevice, 
			BattlePos[i][0], BattlePos[i][1], FALSE, NULL);

		State::ChangeState(BattleEnt->m_mapState, SafeCreateObject(CCharBattleState));
	}

}

VOID QGLApplication::TestDirtyRect()
{
	CDirtyRectManager Dir;

	RECT rect;
	rect.left = GetPrivateProfileInt(_T("DIRTYRECTTEST"), _T("RECTONEL"), 0, CONFIGURATION_INI_FILE);
	rect.top = GetPrivateProfileInt(_T("DIRTYRECTTEST"), _T("RECTONET"), 0, CONFIGURATION_INI_FILE);
	rect.bottom = GetPrivateProfileInt(_T("DIRTYRECTTEST"), _T("RECTONEB"), 0, CONFIGURATION_INI_FILE);
	rect.right = GetPrivateProfileInt(_T("DIRTYRECTTEST"), _T("RECTONER"), 0, CONFIGURATION_INI_FILE);
	Dir.AddDirtyRectToMan(rect);

	rect.left = GetPrivateProfileInt(_T("DIRTYRECTTEST"), _T("RECTTWOL"), 0, CONFIGURATION_INI_FILE);
	rect.top = GetPrivateProfileInt(_T("DIRTYRECTTEST"), _T("RECTTWOT"), 0, CONFIGURATION_INI_FILE);
	rect.bottom = GetPrivateProfileInt(_T("DIRTYRECTTEST"), _T("RECTTWOB"), 0, CONFIGURATION_INI_FILE);
	rect.right = GetPrivateProfileInt(_T("DIRTYRECTTEST"), _T("RECTTWOR"), 0, CONFIGURATION_INI_FILE);
	Dir.AddDirtyRectToMan(rect);

}

Boolean QGLApplication::OnCreate(VOID)
{
	if (!gGlobal.Initialize())
		return (BOOLEAN_FALSE);

	if (!Start())
		return (BOOLEAN_FALSE);

	m_mapState = NULL;
	m_TotalDelTime = 0;
	m_bWaitLogin = BOOLEAN_FALSE;

	State::ChangeState(m_mapState, (State *) SafeCreateObject(CLoginState));	

	TryConnect();

	int m_prevwalktime = 0;

	String title;
	if( gConfigure.strServerName.GetLength() )
		title.Format(_T("%s (%s)"), gGlobal.GetStringTable()->Get(_T("APP_QGL_CAPTION")), gConfigure.strServerName.c_str());
	else
		title = gGlobal.GetStringTable()->Get(_T("APP_QGL_CAPTION"));
	if( title.GetLength()==0 )
		title = _T("QGL");
	if (!m_Window.Create(title.c_str(), 800, 600, 0))//m_Window.SetTitle
		return (BOOLEAN_FALSE);

	//GetRenderClass()->GetSystemCombinedData()->m_DirRectMan.SetBoundRect(0, 0, 800, 600);
	GetRenderClass()->GetSystemCombinedData()->m_DirRectMan.SetBoundRect(0, 0, MaxEntSizeX, MaxEntSizeY);
	QGLMath::CalcBlock32X16Range();

	m_BattleBgTexture = NULL;

	m_pMaskEntity = NULL;
	m_SelectedMapControl = NULL;
	m_RenderMap = TRUE;
	m_TalkingMapControl = NULL;
	m_ClickNPCMapControl = NULL;

	
	m_CurrMapView = SafeCreateObject(CMapObjectView);
	//init render class
	gGlobal.pD3DDevice = GetRenderClass()->GetDevice();
	m_CurrCam.SetWindowSize(GetRenderClass()->GetWinWidth(), GetRenderClass()->GetWinHeight());

	//load login ent
	m_LoginEnt.LoadEntityFile(_T("login.ent"), gGlobal.pD3DDevice);

	//init battle system
	gGlobal.g_pBattleControl = SafeCreateObject(CBattleControl);
	gGlobal.g_pBattleControl->InitBattleData(GetRenderClass(), _T("Data\\eff_bat_background.pic"));

	//init battle script control
	if ( gGlobal.g_pBatScriptControl == NULL )
	{
		gGlobal.g_pBatScriptControl = SafeCreateObject(CBatScriptControl);
	}

	//init the disposal effect
	gGlobal.g_DisposalEntityEffect = SafeCreateObject(CParticleSystem);
	AddParticleSystem(gGlobal.g_DisposalEntityEffect);

	
	CImageManager::GetInstance().SetDevice(gGlobal.pD3DDevice);

	//create UI cursor Texture
	D3DXCreateTexture(gGlobal.pD3DDevice, 128, 128, 0, 0 ,
						D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &gGlobal.CursorData.Texture);
	SetRect(&gGlobal.CursorData.m_CurrDisArea, 0, 0, 128, 128);

	gGlobal.CursorEntity = SafeCreateObject(CAnimatedEntity);
	
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorNormal.c_str(), gGlobal.pD3DDevice);

	//State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));

	gGlobal.ShowCursor = true;
	
	CFontManager::GetInstance().InitFonts();

	
	FSOUND_Manager::GetInstance().Init();


	UIEditBox::SetupCursor(GetRenderClass());

	
	gGlobal.pUIManager = SafeCreateObject(UIManager);


	gGlobal.pUIManager->SetRender(GetRenderClass());

	//gGlobal.pUIManager->InitIME();

	gGlobal.pUIManager->GetDirtyRectMan()->SetBoundRect(0, 0, 800, 600);

	XML_UI::GetInstance().Init();
	XML_UI::GetInstance().ReadFormFile("Data\\LoginUI.xml", gGlobal.pUIManager);
	
	gGlobal.pUIManager->LoadLuaUI("Data\\LuaUI\\XLoginUI.Lua");	

	::InitLoginScreen();

	XML_UI::GetInstance().ReadFormFile("Data\\CharacterChosen.xml", gGlobal.pUIManager);
	XML_UI::GetInstance().ReadFormFile("Data\\ServerChosen.xml", gGlobal.pUIManager);

	gGlobal.pUIManager->RenderAll();


	if (!gConfigure.uDisAbleSound)
	{
		::GetFSoundManager()->SetMusic("Data\\music0005_MainTheme05.ogg");
		::GetFSoundManager()->PlayMusic();
		if (gGlobal.g_pSystemSetting->IsMusicModeEnable())
			::GetFSoundManager()->MusicSetVolume(gGlobal.g_pSystemSetting->GetMusicVolForFmod());
		else
			::GetFSoundManager()->MusicSetVolume(0);

		gGlobal.ClickSnd = FSOUND_Manager::GetInstance().CreateSound(_T("Data\\Wav\\SoundI0005_click.wav"));
		if (gGlobal.ClickSnd)
		{
			if (gGlobal.g_pSystemSetting->IsSoundEffectModeEnable())
				gGlobal.ClickSnd->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
			else
				gGlobal.ClickSnd->SetVolume(0);
			gGlobal.ClickSnd->SetReserved(true);
		}
	}

	FSound* sfx = FSOUND_Manager::GetInstance().CreateSoundA("Data\\Wav\\SoundI0006_choose.wav");
	if (sfx)
	{
		sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());//gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
		sfx->SetReserved(true);
	}

	gGlobal.NightMask = SafeCreateObject(CAnimatedEntity);
	gGlobal.NightMask->SetEntityType(ENTITY_TYPE_PARTICLE_WITHOUTEFFECT);
	gGlobal.NightMask->LoadEntityFile(gConfigure.strNightMaskName.c_str(), gGlobal.pD3DDevice);
	gGlobal.NightMask->LockResources();
	gGlobal.NightMask->SetEntOffsetY(-1 * gGlobal.NightMask->m_EntOrgHeight / 2 + (gGlobal.NightMask->m_EntOrgHeight - CharPicOffsetY));
	gGlobal.NightMask->SetExtraZBuffer(NightExtraZ);
	gGlobal.NightMask->SetStopAnimation(TRUE);

	if (!gConfigure.uNetwork)
	{
		//all these are testings in offline mode
		InitEntity();
		gGlobal.g_pCharacter = SafeCreateObject(CCharacterControl);
		gGlobal.g_pCharacter->LoadDefaultPart();
		gGlobal.g_pCharacter->InitShadow();
		gGlobal.g_pCharacter->GetCharMainView()->SetEntityMoveSpeed(150);
		gGlobal.g_pCharacter->SetEntityCurrPos(gConfigure.uPosX, gConfigure.uPosY);
		StringA strTemp = gConfigure.strFontStyle.c_str();
		gGlobal.g_pCharacter->GetCharNameView()->LoadText(gConfigure.strCharName.c_str(), 
			strTemp.c_str(), gConfigure.uNameFontSize, D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
			gConfigure.uNameFontColorG, gConfigure.uNameFontColorB), EDGE_SHADOW);	
		AddCharCtrlToMap(gGlobal.g_pCharacter);
		m_CurrCam.MoveToTargetImm();
		gGlobal.g_pCharacter->GetCharMainView()->SetEntityType(ENTITY_TYPE_MAP_CONTROL_PLAYER);
		gGlobal.g_pCharacter->GetCharShadowView()->SetEntityType(ENTITY_TYPE_MAP_CONTROL_PLAYER);
		gGlobal.g_pSkillProcess->ConstructData();
	}
	else
	{
		gGlobal.g_pAccountControl = SafeCreateObject(CAccountControl);
	}
	
	m_ListenToPrivateMessage = TRUE;

	m_ListenToFriendMessage = TRUE;

	m_prevwalktime = 0;

	m_LoginEnt.WaitResReady();

	CharW FileName[50];
	GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("COUNTNUM"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);

	gGlobal.m_ChooseCountDown.SetFileNameW(FileName);
	gGlobal.m_ChooseCountDown.SetExtraZValue(0x20000000);

	m_MapParticle = SafeCreateObject(CParticleSystem);

	CResourcesManager::GetInstance().OnLoadAllColorPalette();

	RECT	rect;
	::GetWindowRect(m_Window.GetWindow(), &rect);
	if(LoadQGLState() && gGlobal.g_StartWndX >= 0 && gGlobal.g_StartWndY >= 0 &&
		 gGlobal.g_StartWndX < (UInt)GetSystemMetrics(SM_CXSCREEN) - 10 &&
		 gGlobal.g_StartWndY < (UInt)GetSystemMetrics(SM_CYSCREEN) - 10)
	{
		m_SubWindow.m_AlignType = gGlobal.g_pSystemSetting->GetSubChatPosition();
		
		MoveWindow(m_Window.GetWindow(), 
			gGlobal.g_StartWndX, 
			gGlobal.g_StartWndY, 
			rect.right - rect.left, 
			rect.bottom - rect.top, TRUE);
	}
	else
	{
		gGlobal.g_StartWndX = rect.left; 
		gGlobal.g_StartWndY = rect.top; 
	}

	if (gGlobal.g_pSystemSetting && !gGlobal.g_pSystemSetting->IsWindowModeEnable() && CheckMyProcess(m_Window.GetWindow()) == 1)
	{
		QGLApplication::GetInstance().GetRenderClass()->FreeVolatileResources();
		QGLApplication::GetInstance().GetRenderClass()->ToggleFullscreen();
	}

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("QGLApplication::OnCreate : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	//for testing only
	/*rect.left = rect.top = 0;
	rect.right = rect.bottom = 800;

	HANDLE hFile = CreateFile(_T("Data\\mapD0001.map"), FILE_READ_DATA, FILE_SHARE_READ, 
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);


			HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 
				0, 0, _T("mapD0001.map"));

			Byte * pInFile = (Byte *)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);

			Byte * pData = pInFile;
			for (UInt i = 0; i < 1024 * 1024 * 4; i++)
			{
				Byte data = * pData;
				pData ++;
			}
			if (!UnmapViewOfFile(pInFile))
	{
		Int i = 0;
		i ++;
	}
	CloseHandle(hMapFile);
	CloseHandle(hFile);
	while(true)
	{
		/*UInt i = 0;
		String text;
		text.Format(_T("mapD000%d.map"), i);
		MapData * pMapData = CResourcesManager::GetInstance().LoadMapResourcesFile(_T("mapD0001.map"), gGlobal.pD3DDevice, rect);
		if (pMapData)
		CResourcesManager::GetInstance().ReleaseMapResources(pMapData);
		
		FSound* sfx = ::GetFSoundManager()->CreateSound(_T("Data\\Wav\\EffSound0027_hurt.wav"));
		if (sfx)
		{
			sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
			sfx->Play();
		}*/

		/*bool flag = false;
		if (flag)
			::GetFSoundManager()->SetMusic("Data\\music0009_Field01.ogg");
		else
			::GetFSoundManager()->SetMusic("Data\\music0008_Town03.ogg");
		flag = !flag;
		*/
	//}
	return (BOOLEAN_TRUE);
}

VOID QGLApplication::RemoveEntityInLst(CEntityBase * pEntBase)
{
	if( m_EntRenderLst.size()==0 )
		return;

	StlMultiMap<UInt32, CEntityBase *>::iterator EntItr = m_EntRenderLst.begin();
	
	while (EntItr != m_EntRenderLst.end())
	{
		if (EntItr->second == pEntBase)
		{
			m_EntRenderLst.erase(EntItr);
			return;
		}
		EntItr++;
	}
}

CMapActorControl * QGLApplication::FindMapActCtrlInWindow(Int PosX, Int PosY, Int MapWidth)
{
	if (m_RenderCMapCtrlLst.size() == 0)
		return NULL;

	if (GetRenderClass()->GetCurPosEntity() == NULL)
		return NULL;

	if (gGlobal.pUIManager && gGlobal.pUIManager->GetMouseOnUIFlag())
		return NULL;

	CMapActorControl * m_SelectedCtrl = NULL;
	
	StlMap<CEntityBase *,  CMapActorControl *>::iterator MapActCtrlItr = 
		m_RenderCMapCtrlLst.find(GetRenderClass()->GetCurPosEntity());
	if (MapActCtrlItr != m_RenderCMapCtrlLst.end())
		m_SelectedCtrl = MapActCtrlItr->second;
	return m_SelectedCtrl;
}


VOID QGLApplication::OnDispose(VOID)
{
	switch(GetMapStateID())
	{
	case ST_APP_MAP:
	case ST_APP_BATTLE:
		gGlobal.pUIManager->SavePanelPos(_T("Data\\MyPanelPos.XML"));
		;
		gGlobal.g_pMenuHelpProcess->WriteHelpTipsFile();
	}

	gGlobal.g_pOnlineShopProcess->CloseOnlineShop();
	if (gGlobal.g_pMenuMixProcess->IsStartMaking())
		gGlobal.g_pMenuMixProcess->CancelMake();
	
	SaveQGLState();


	//state release
	if (m_mapState)
		State::ChangeState(m_mapState, NULL);
	
	m_Window.OnDispose();

	if (gGlobal.g_pAccountControl)
	{
		DeleteNetGroup(gGlobal.g_pAccountControl->m_characters, BOOLEAN_TRUE);
		DeleteNetObject(gGlobal.g_pAccountControl->m_pAccount);
		SafeDeleteObject(gGlobal.g_pAccountControl);
	}

	if (m_CurrMapView)
	SafeDeleteObject(m_CurrMapView);
	//delete map
	DeleteNetObject(gGlobal.g_pMap);
	gGlobal.g_pMap = NULL;

	gGlobal.m_ChooseCountDown.Release();
	if (gGlobal.m_pBag)
	{
		DeleteNetObject(gGlobal.m_pBag);
		gGlobal.m_pBag = NULL;
	}

	//delete battle system
	if (gGlobal.g_pBattleControl) {
		gGlobal.g_pBattleControl->NotifyEndBattle();
		SafeDeleteObject(gGlobal.g_pBattleControl);
	}

	//delete script system
	SafeDeleteObject(gGlobal.g_pScriptControl);
	SafeDeleteObject(gGlobal.g_pBatScriptControl);

	//delete item menu process
	SafeDeleteObject(gGlobal.g_pItemProcess);

	//delete the message box before the uimanage deleted
	gGlobal.g_PKRequestBox.DeleteMessageBox();
	gGlobal.g_QuestCancelBox.DeleteMessageBox();
	gGlobal.g_QuestionBox.DeleteMessageBox();
	
	StlVector<CCharacterControl *> CharRmLst;
	
	UInt32CharCtrlPtrMap::iterator Itr = m_MapCharacterLst.begin();
	while (Itr != m_MapCharacterLst.end())
	{
		CharRmLst.push_back(Itr->second);
		Itr ++;
	}	

	for (Int i = 0; i < (Int)CharRmLst.size(); i++)
		SafeDeleteObject(CharRmLst[i]);
	
	m_MapCharacterLst.clear();

	CharRmLst.clear();	

	
	SafeDeleteObject(gGlobal.pUIManager);

	ChatPool::DestroyInstance();
	FSOUND_Manager::DestroyInstance();
	XML_UI::DestroyInstance();
	XMLBIN_UI::DestroyInstance();
	UINameList::DestroyInstance();
	CFontManager::DestroyInstance();
	CImageManager::DestroyInstance();
	CQuestionProcess::DestroyInstance();
	
	m_ProcessManager.Detach(&LocalRPCManager::GetInstance());
	m_ProcessManager.Detach(&Reuben::Simulation::ScheduleInstance::GetInstance());
	
	for (int i = 0; i < (int)TestPlayerLst.size(); i++)
		SafeDeleteObject(TestPlayerLst[i]);

	//delete particle system
	SafeDeleteObject(gGlobal.g_ParticleEffectRain);
	SafeDeleteObject(gGlobal.g_ParticleEffectSnow);
	SafeDeleteObject(gGlobal.g_ParticleEffectFog);
	SafeDeleteObject(gGlobal.g_DisposalEntityEffect);

	DeleteNetGroup(gGlobal.g_SkillLst, BOOLEAN_TRUE);
	DeleteNetObject(gGlobal.g_QuestSystem);
	DeleteNetGroup(gGlobal.g_PetLst, BOOLEAN_TRUE);

	if (gGlobal.NightMask)
	SafeDeleteObject(gGlobal.NightMask);
	//g_BattleInterfaceTexture->Release();

	if (gGlobal.CursorData.Texture)
		gGlobal.CursorData.Texture->Release();

	SafeDeleteObject(gGlobal.CursorEntity);
	CResourcesManager::DestroyInstance();

	Stop();
	gGlobal.Uninitialize();
	LocalRPCManager::GetInstance().DeleteAll();
	Reuben::Simulation::ObjectInstance::GetInstance().DeleteAll();
	ProcedureManager::DestroyInstance();
	LocalRPCManager::DestroyInstance();
	SessionManager::DestroyInstance();

	SafeRelease(m_MaskTextureData.MaskTexture);
	CloseSubWindow();

	SafeDeleteObject(gGlobal.g_CursorState);

	SafeDeleteObject(m_MapParticle);

//	if (gConfigure.bShowWebSite)
//		ShellExecute(NULL, _T("open"), _T("http://mg.51hit.com/launcher/quit.html"), NULL, NULL, SW_SHOWNORMAL);

	// create ending process
	if(!gGlobal.g_NewGame)
	{
		STARTUPINFO startupInfo = {0};
		startupInfo.cb = sizeof(startupInfo);
		PROCESS_INFORMATION processInformation;// Try to start the process
		BOOL result = ::CreateProcess(  
			_T("edbnr.bin"),
			NULL, 
			NULL,  
			NULL,  
			FALSE,  
			NORMAL_PRIORITY_CLASS,  
			NULL,  
			NULL,  
			&startupInfo,  
			&processInformation);
	}

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("QGLApplication::OnDispose : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());
}

VOID QGLApplication::CreateMaskTexture(Byte * GivenMaskData, UInt Width, UInt PixelSize, RECT SrcLockRect, Byte MaskTextureType, UInt32 FadeInOutTime)
{ 
	//D3DSURFACE_DESC  pDescSrc;

	//D3DLOCKED_RECT d3dltSrc;

	D3DSURFACE_DESC  pDescDes;

	D3DLOCKED_RECT d3dltDes;

	if (m_MaskTextureData.MaskTexture == NULL)
	{
			D3DXCreateTexture(gGlobal.pD3DDevice, BgTextureSizeX, BgTextureSizeY, 0, 0 ,
						D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_MaskTextureData.MaskTexture);
	}

		if(CRender::LockSystemTexture(pDescDes, d3dltDes, 0, m_MaskTextureData.MaskTexture))
	//if(CRender::LockSystemTexture(pDescSrc, d3dltSrc, SrcLockRect, GivenMaskTexture))
	//if (GivenMaskData)
	{
			BYTE * pDesTData = (BYTE *)d3dltDes.pBits;

			if (GivenMaskData)
			{
				BYTE * pSrcTData = GivenMaskData + (SrcLockRect.top * Width + SrcLockRect.left) * PixelSize;
			for ( Int i = 0; i <  (Int)SrcLockRect.bottom - SrcLockRect.top; i ++)
			{
				memcpy(pDesTData, pSrcTData, (SrcLockRect.right - SrcLockRect.left) * PixelSize);
				pDesTData += d3dltDes.Pitch;
				pSrcTData += Width * PixelSize;
			}
			}
			else
			{
				for (Int i = 0; i < (Int)(d3dltDes.Pitch * pDescDes.Height); i += 4)
				{
					*(DWORD *)(pDesTData) = D3DCOLOR_ARGB(255, 0,0,0);//(0xFF000000;
					pDesTData += 4;
				}
				/*for ( Int i = 0; i <  (Int)SrcLockRect.bottom - SrcLockRect.top; i ++)
				{
					memcpy(pDesTData, pSrcTData, (SrcLockRect.right - SrcLockRect.left) * PixelSize);
					pDesTData += d3dltDes.Pitch;
					pSrcTData += Width * PixelSize;
				}*/
			}
			//SafeRelease(m_MaskTextureData.MaskTexture);
			//m_MaskTextureData.MaskTexture = pTexture;
			
			m_MaskTextureData.Type = MaskTextureType;
			m_MaskTextureData.FadeInOutTime = FadeInOutTime;
			m_MaskTextureData.StartTime = 0;//::timeGetTime();
			if (MaskTextureType == MASKTEXTURETYPE_FADE_IN)
				m_MaskTextureData.Alpha = 0;
			else if (MaskTextureType == MASKTEXTURETYPE_FADE_OUT)
				m_MaskTextureData.Alpha = 255;

			m_MaskTextureData.MaskTexture->UnlockRect(0);

			m_MaskTextureData.MaskTexture->AddDirtyRect(NULL);
		
		//GivenMaskTexture->UnlockRect(0);
	}
}

VOID QGLApplication::OnFatalError(Exception& ex)
{
	int i = 0;
}

VOID QGLApplication::ReleaseNightData()
{
	gGlobal.g_NightMaskTextureData.m_DirRectMan.ClearDirtyRect();
	gGlobal.g_NightMaskTextureData.m_NextDirtyRegionLst.clear();
	gGlobal.g_NightMaskTextureData.m_PreDirtyRegionLst.clear();
	gGlobal.NightMask->LeaveParent();
	SafeRelease(gGlobal.g_NightMaskTextureData.Texture);
	gGlobal.g_NightMaskTextureData.Texture = NULL;
	gGlobal.g_NightMaskTextureData.m_DirRectMan.SetBoundRect(0, 0, GetRenderClass()->GetWinWidth(), GetRenderClass()->GetWinHeight());
	RemoveMaskEntity();
}
VOID QGLApplication::CreateNightData()
{
	SetMaskEntity(gGlobal.NightMask);
	gGlobal.NightMask->SetEntParent(gGlobal.g_pCharacter->GetCharMainView());gGlobal.NightMask->SetEntParent(gGlobal.g_pCharacter->GetCharMainView());
	if (gGlobal.g_NightMaskTextureData.Texture == NULL)
	D3DXCreateTexture(gGlobal.pD3DDevice, BgTextureSizeX, BgTextureSizeY, 0, 0 ,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &gGlobal.g_NightMaskTextureData.Texture);
	SetRect(&gGlobal.g_NightMaskTextureData.m_CurrDisArea, ExtraMapBuffX, ExtraMapBuffY, GetRenderClass()->GetWinWidth() + ExtraMapBuffX
		, GetRenderClass()->GetWinHeight() + ExtraMapBuffY);
	gGlobal.g_NightMaskTextureData.m_DirRectMan.AddDirtyRectToMan(GetRenderClass()->GetWinRect());
	gGlobal.g_NightMaskTextureData.m_DirRectMan.SetAddAvial(FALSE);
	
}

int CountWindowClass();

VOID QGLApplication::Login()
{
	
	{
		Int Count = CountWindowClass();
		if(Count >= 6)
		{
			PostQuitMessage(0);
			return;
		}
	}

	if (gConfigure.uNetwork)
	{
		if (gGlobal.g_pAccountControl)
		{
			UIEditBox* LoginNameEditBox = gGlobal.pUIManager->FindUIEditBox("LoginName");
			UIEditBox* PasswordEditBox = gGlobal.pUIManager->FindUIEditBox("Password");

			if (LoginNameEditBox && PasswordEditBox)
			{
				String strLoginName(LoginNameEditBox->GetText());
				String strPassword(PasswordEditBox->GetText());
				CONST ByteArray& password = PasswordEditBox->GetHashedPassword();

				if (strLoginName.IsEmpty() || strPassword.IsEmpty())
				{
					gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_LOGIN_ERR")));	
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SetLoginState", String(), 0);
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "InitLogin", String());
					return;
				}
				
				if (gGlobal.pPeerProxy == NULL)
				{
					if (!ConnectServer())
					{
						if (gGlobal.m_logined == LOGIN_VERSION_WRONG)
							PostQuitMessage(0);
						else
						{
							gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_CONNECT_SERVER_FAIL")), false, 3000, 3000);
							CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "InitLogin", String());
						}
						return;
					}
					else
						m_bWaitLogin = BOOLEAN_TRUE;
				}
				//gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_LOGIN"), false);
				if (gGlobal.pPeerProxy && Reuben::Math::AES::KeyChanged())
					gGlobal.g_pAccountControl->Login(strLoginName.c_str(), password);
			}

		}
	}
	else
	{	
		//try to load new map and old map to compear the time diff

		ChangeMapUI();
		ChangeMap(gConfigure.strMapFile.c_str(),gConfigure.uPosX , 
			gConfigure.uPosY);
		ReadChangeMapInfo();

		m_CurrMapView->AddMapObject(gConfigure.strObject.c_str(), gGlobal.pD3DDevice, gConfigure.uObjectPosX, gConfigure.uObjectPosY);

		//String miniFilename;
		//miniFilename.Format( _T("%s_mini.tga"),gConfigure.strMapFile.Left(gConfigure.strMapFile.GetLength()-4).c_str() );
		//InitMiniMap(miniFilename);
		InitBattle();
		State::ChangeState(m_mapState , SafeCreateObject(CMapState));
		gGlobal.pUIManager->RenderAll();
		gGlobal.g_pSkillProcess->ConstructDataAll();
	}
}


VOID QGLApplication::OnUpdate(UInt32 uTimeDelta)
{		
	UInt32 uLastTime = ::timeGetTime();

	if (GetRenderClass()->IsFullScreen())
		gGlobal.g_DrawAll = TRUE;

	static bool first = true;
	if (first)
	{
		m_TotalDelTime = uLastTime;
		first = false;
	}

	UInt lockFrame = 1000 / gConfigure.uFrameRate;
	if (uLastTime  < lockFrame + m_TotalDelTime &&
		lockFrame + m_TotalDelTime - uLastTime < 10000)
	{
		Sleep(lockFrame + m_TotalDelTime - uLastTime);
		m_TotalDelTime = ::timeGetTime();
	}
	else
	{
		m_TotalDelTime += lockFrame;
		
	}

	if(gGlobal.pUIManager)
	{
		if(gGlobal.pUIManager->GetEvent())
		{
			Int32 EventCode = gGlobal.pUIManager->CatchEvent();
			
			switch(m_mapState->GetStateID())
			{
			case ST_APP_LOGIN:
				{
					switch(EventCode)
					{
					case 100:
						Login();
						break;
					case 104:
							gGlobal.g_pLoginProcess->EnterGame();
						break;
					case 105:
							m_LoginEnt.SetStopAnimation(true);
							::SaveLoginDefault();
							gGlobal.g_pLoginProcess->ShowCharacterInLogin();
						break;
					case 106:
							gGlobal.g_pLoginProcess->ShowChooseLine();
						break;
					case 107:	// open force logout dialog
						{
								AddXMLFile2UIManger("Data\\ForceLogOut.xml");
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SetForceLogoutFlag", String(), 1);
								gGlobal.pUIManager->RenderAll();
						}
						break;
					case 108:	// close force logout dialog
							gGlobal.pUIManager->DeletePanel("ForceLogoutPanel");
							gGlobal.g_pLoginProcess->EnterGame();
						break;
					case 109:
						{
							gGlobal.g_pAccountControl->Logout();	
							m_LoginEnt.SetStopAnimation(false);
							gGlobal.g_pAccountControl->m_needReLogin = true;
							UIPanel* characterChosenPanel = (UIPanel*)gGlobal.pUIManager->GetUIItem("CharacterChosenPanel");
							UIPanel* loginBackground = (UIPanel*)gGlobal.pUIManager->GetUIItem("LoginBackground");
							if (characterChosenPanel && loginBackground)
							{
								characterChosenPanel->SetEnable(false);							
								loginBackground->SetEnable(false);
							}
						}
						break;
					case 110:
							PostQuitMessage(0);
						break;
					case 111:
						{
							if (!gGlobal.g_pAccountControl->IsSelectCharacterFrozen())
                                gGlobal.g_pAccountControl->CheckCharacter();
							else{
								gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHARACTER_FROZEN")));	
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SetGameState", String(), 1);
							}
						}
						break;
					}
				}
				break;
			case ST_APP_MAP:
				{
					switch(EventCode)
					{
					case 101:
						{
							CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseSubWindowNow", String());
							//CloseSubWindow();
						}
						break;
					case 104:
						{
							UIItem * uip = gGlobal.pUIManager->GetUIItem("ChannelHistoryPopPanel");
							if( uip )
							{
								uip->SetEnable(false);
								gGlobal.pUIManager->DeletePanel("ChannelHistoryPopPanel");
								gGlobal.pUIManager->RenderAll();
							}
						}
						break;
					case 105:
						{
							UIItem * uip = gGlobal.pUIManager->GetUIItem("BlockListPopPanel");
							if( uip )
							{
								uip->SetEnable(false);
								gGlobal.pUIManager->DeletePanel("BlockListPopPanel");
								gGlobal.pUIManager->RenderAll();
							}
						}
						break;
					case 106:
						{
							UIItem * uip = gGlobal.pUIManager->GetUIItem("ChatIconPopPanel");
							if( uip )
							{
								uip->SetEnable(false);
								gGlobal.pUIManager->DeletePanel("ChatIconPopPanel");
								gGlobal.pUIManager->RenderAll();
							}
						}
						break;
					case 107:
						{
							UIItem * uip = gGlobal.pUIManager->GetUIItem("ClockPanel");
							if ( uip )
							{
								uip->SetEnable(false);
								gGlobal.pUIManager->DeletePanel("ClockPanel");
								gGlobal.pUIManager->RenderAll();
							}
						}
						break;
					}
				}
				break;
			case ST_APP_BATTLE:
				switch (EventCode)
				{
				case 101:
					{
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseSubWindowNow", String());
						//CloseSubWindow();
						break;
					}
				case 104:
					{
						UIItem * uip = gGlobal.pUIManager->GetUIItem("ChannelHistoryPopPanel");
						if( uip )
						{
							uip->SetEnable(false);
							gGlobal.pUIManager->DeletePanel("ChannelHistoryPopPanel");
							gGlobal.pUIManager->RenderAll();
						}
					}
					break;
				case 105:
					{
						UIItem * uip = gGlobal.pUIManager->GetUIItem("BlockListPopPanel");
						if( uip )
						{
							uip->SetEnable(false);
							gGlobal.pUIManager->DeletePanel("BlockListPopPanel");
							gGlobal.pUIManager->RenderAll();
						}
					}
					break;
				case 106:
					{
						UIItem * uip = gGlobal.pUIManager->GetUIItem("ChatIconPopPanel");
						if( uip )
						{
							uip->SetEnable(false);
							gGlobal.pUIManager->DeletePanel("ChatIconPopPanel");
							gGlobal.pUIManager->RenderAll();
						}
					}
					break;
				}
				
				break;
			}
		}

		gGlobal.UpdateClockHandle();
			
		gGlobal.pUIManager->Update(uLastTime);
		
		GetRenderClass()->SetCursorPos(
			gGlobal.pUIManager->GetCursorX() + gGlobal.pUIManager->GetCursorOffsetX(), 
			gGlobal.pUIManager->GetCursorY() + gGlobal.pUIManager->GetCursorOffsetY()
			);
	}

	m_ProcessManager.UpdateProcesses(uTimeDelta);
	// update network object states first
	UpdateGameLoop(GetRenderClass());
	UpdateSceneData();

	//draw the middle map
	if (gGlobal.g_pMiddleMap)
		gGlobal.g_pMiddleMap->UpdatePosition();

	if (gGlobal.g_pSearchListProcess)
		gGlobal.g_pSearchListProcess->UpdateSearchTimer();

	if (gGlobal.g_pMenuMixProcess)
		gGlobal.g_pMenuMixProcess->UpdateTimeLine(uLastTime);

	if (gGlobal.g_pShopProcess)
		gGlobal.g_pShopProcess->UpdateBusinessShopItem(uLastTime);

	if (gGlobal.WindowShow)
		DrawScene();
	

	//draw sub scene
	DrawSubWindow();

	ResetUpdatedStatus();

	//update left click status
	if (gGlobal.LeftClickUpdate &&
		GetMapStateID() == ST_APP_MAP)
	{
		if (gGlobal.LeftClickStatus == LEFTCLICK_DOWN)
		{
			gGlobal.LeftClickStatusCounter += uTimeDelta;
			if (gGlobal.LeftClickStatusCounter > gConfigure.uAutoLeftClickTime)
			{
				gGlobal.LeftClickStatusCounter = 0;
				gGlobal.LeftClickStatus = LEFTCLICK_AUTO;
			}
		}
		else if (gGlobal.LeftClickStatus == LEFTCLICK_AUTO)
		{
			gGlobal.LeftClickStatusCounter += uTimeDelta;
			static Int LeftClickCount = 0;
			if (LeftClickCount * gConfigure.uAutoLeftClickFeq >= 500)
			{
				if ( gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NORMAL )
					MoveToScreenPos(gGlobal.pUIManager->GetCursorX(), gGlobal.pUIManager->GetCursorY(), TRUE);
				
				gGlobal.LeftClickStatusCounter = 0;
				LeftClickCount  = 0;
			}
			else if (gGlobal.LeftClickStatusCounter > gConfigure.uAutoLeftClickFeq)
			{
				AddLeftClickEntity();
				gGlobal.LeftClickStatusCounter = 0;
				LeftClickCount ++;
			}

		}
	}

	//update horse light message
	gGlobal.UpDateHorseLightMsg(uLastTime);
	for(BYTEChannelCtrlMap::iterator Itr = gGlobal.m_MapChannelLst.begin(); Itr != gGlobal.m_MapChannelLst.end();
		++Itr)
	{
		Itr->second->Update(uTimeDelta);
	}
	//update channel

	//update speaker channel
	gGlobal.UpdateSpeaker();

	// check down time
	if (gGlobal.m_downTime > 0)
		gGlobal.CheckDownTime(uTimeDelta);

	// wait login for AES key data
	if (m_bWaitLogin && Reuben::Math::AES::KeyChanged() && gGlobal.pUIManager)
	{
		m_bWaitLogin = BOOLEAN_FALSE;
		gGlobal.pUIManager->SetEvent(100);
	}

	if (m_manage_interval > gConfigure.manage_interval)
	{
		DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("QGLApplication::OnUpdate : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());
		m_manage_interval = 0;
	}
	m_manage_interval += uTimeDelta;

	Sleep(1);

	//adding pulse to server
	static UInt32 pulse_counter = 0;
	pulse_counter += uTimeDelta;
	if (pulse_counter > 10000)
	{
		if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			gGlobal.g_pCharacter->GetCharNetObject()->RespondTest(gGlobal.pPeerProxy,gGlobal.g_context, 1, 1, 1, 1);
		pulse_counter = 0;
	}
}

ApplicationStateEnum QGLApplication::GetMapStateID()
{
	return (ApplicationStateEnum)m_mapState->GetStateID();
}
VOID QGLApplication::ChangeMap(LPCTSTR filename, UInt X, UInt Y)
{
	m_CurrMapView->FreeMapData();
	DeleteAllCharacters();
	if (m_mapState->GetStateID() == ST_APP_BATTLE)
		LeaveBattle();

	//leave the map
	//GetRenderClass()->AddAllDirtyRect();
	
	//then copy the background with subtractive blend

	//CreateMaskTexture(CResourcesManager::GetInstance().GetWorldImage(), MaxEntSizeX, 4, GetRenderClass()->GetCurrDisRect(), MASKTEXTURETYPE_FADE_OUT, 1000);
	GetRenderClass()->LockTexture();
	if (gGlobal.g_IsChangeLine)
		CreateMaskTexture(NULL, MaxEntSizeX, 4, GetRenderClass()->GetCurrDisRect(), MASKTEXTURETYPE_FADE_OUT, 1000);
	else
	{
		CreateMaskTexture(GetRenderClass()->GetSystemCombinedData()->pData, MaxEntSizeX, 4, GetRenderClass()->GetCurrDisRect(), MASKTEXTURETYPE_FADE_OUT, 1000);
		if (gGlobal.Night && gGlobal.NightMask && gGlobal.g_NightMaskTextureData.Texture)
		{
			D3DSURFACE_DESC  pDescSrc;
			D3DLOCKED_RECT d3dltSrc;
			D3DSURFACE_DESC  pDescDes;
			D3DLOCKED_RECT d3dltDes;
			if(CRender::LockSystemTexture(pDescDes, d3dltDes, 0, m_MaskTextureData.MaskTexture))
			{
				if (CRender::LockSystemTexture(pDescSrc, d3dltSrc, &GetRenderClass()->GetCurrDisRect(), gGlobal.g_NightMaskTextureData.Texture))
				{
					CRender::BlitPic((Byte *)d3dltSrc.pBits, 0, GetRenderClass()->GetWinWidth(), GetRenderClass()->GetWinHeight(),
						pDescSrc.Width, (Byte *)d3dltDes.pBits, 0, pDescDes.Width, FALSE, gGlobal.g_NightMaskAlpha, 4, 4);
					gGlobal.g_NightMaskTextureData.Texture->UnlockRect(0);
				}
				m_MaskTextureData.MaskTexture->UnlockRect(0);
			}
		}
	}
	GetRenderClass()->UnLockTexture();

	GetRenderClass()->AddAllDirtyRect();
	//find the effective rectangle
	RECT EffectArea;
	Int PixelX = 0;
	Int PixelY = 0;
	QGLMath::BlockPosToPixel(X, Y, PixelX, PixelY);
	EffectArea.left = PixelX  - 800;
	EffectArea.right = PixelX + 800;
	EffectArea.top = PixelY  - 600;
	EffectArea.bottom = PixelY  + 600;
	if (EffectArea.left < 0)
	{
		EffectArea.right -= EffectArea.left;
		EffectArea.left = 0;
	}

	if (EffectArea.top < 0)
	{
		EffectArea.bottom -= EffectArea.top;
		EffectArea.top = 0;
	}

	
	m_CurrMapView->LoadMapData(filename, gGlobal.pD3DDevice, EffectArea);

	if (EffectArea.right > m_CurrMapView->GetMapWidth())
	{
		Int Diff = EffectArea.right - m_CurrMapView->GetMapWidth();
		EffectArea.right = m_CurrMapView->GetMapWidth();
		EffectArea.left -= Diff;
	}

	if (EffectArea.bottom > m_CurrMapView->GetMapHeight())
	{
		Int Diff = EffectArea.bottom - m_CurrMapView->GetMapHeight();
		EffectArea.bottom = m_CurrMapView->GetMapHeight();
		EffectArea.top -= Diff;
	}

	m_CurrMapView->SetEffectiveRect(EffectArea);

	gGlobal.g_pCharacter->SetEntityCurrPos(X, Y);
	gGlobal.g_pCharacter->GetCharMainView()->SetAnimationIndexByIndex(ANIM_IDLE1);
	gGlobal.g_pCharacter->GetCharMainView()->m_PathFindData.Clear();
	
	if (gGlobal.g_pCharacter->HasPartner())
	{
		gGlobal.g_pCharacter->GetPartnerMainView()->m_PathFindData.Clear();
		gGlobal.g_pCharacter->GetPartnerMainView()->SetCharPos(X, Y);
	}

	if (gGlobal.g_pCharacter->HasFollower())
		gGlobal.g_pCharacter->RefreshFollower();

	AddCharCtrlToMap(gGlobal.g_pCharacter);
	m_CurrCam.SetMapSize(m_CurrMapView->GetMapWidth(), m_CurrMapView->GetMapHeight());
	m_CurrCam.SetViewingTarget(gGlobal.g_pCharacter->GetCharMainView());
	m_CurrCam.MoveToTargetImm();

	if (!gGlobal.g_ParticleEffectRain)
		gGlobal.g_ParticleEffectRain = InitRain(GetRenderClass());
	if (!gGlobal.g_ParticleEffectSnow)
		gGlobal.g_ParticleEffectSnow = InitSnow(GetRenderClass());
	if (!gGlobal.g_ParticleEffectFog)
		gGlobal.g_ParticleEffectFog = InitFog(GetRenderClass());

	

	m_MaskTextureData.StartTime = 0;

	gGlobal.g_pMenuBuffIconProcess->ShowBuffIcon();

	if (gGlobal.g_MenuTargetProcess)
		gGlobal.g_MenuTargetProcess->HideTargetMenu();
}

VOID QGLApplication::ReadChangeMapInfo()
{
	//Assign Map/NPC/Obj/Jumppoint Info
	gGlobal.FreeClientMapData();
	PathFindLoadData pPathFindData;
	UInt32 numGuide;

	if (gGlobal.ReinitClientMapData(pPathFindData, numGuide))
	{
		if (gConfigure.uNetwork)
		QGLApplication::GetInstance().ChangeMap(gGlobal.m_mapdb->m_basicinfo->m_mapmapfile, 
			gGlobal.g_pCharacter->GetCharNetObject()->GetCurPosX(), 
			gGlobal.g_pCharacter->GetCharNetObject()->GetCurPosY());

		size_t sizeofjplist = gGlobal.m_jplist.size();
		BinaryClientJumppointInstance *tmpjp;
		CAnimatedEntity *jpent;
		for( size_t i = 0; i<sizeofjplist; i++ )
		{
			tmpjp = gGlobal.m_jplist[i];
			jpent = SafeCreateObject(CAnimatedEntity);
			jpent->LoadEntityFile(tmpjp->szFileName, gGlobal.pD3DDevice);
			jpent->SetExtraZBuffer(ObjExtraZ);
			jpent->SetCharPos(tmpjp->m_posx, tmpjp->m_posy);
			m_CurrMapView->AddEntityToMap(jpent);

			
		}

		size_t sizeofnpclist = gGlobal.m_npclist.size();
		

		size_t sizeofobjlist = gGlobal.m_objlist.size();
		BinaryClientOBJInstance *tmpobj;
		CAnimatedEntity *objent;
		for( size_t i = 0; i<sizeofobjlist; i++ )
		{
			tmpobj = gGlobal.m_objlist[i];
			objent = SafeCreateObject(CAnimatedEntity);
			objent->LoadEntityFile(tmpobj->m_filename, gGlobal.pD3DDevice);
			objent->SetExtraZBuffer(ObjExtraZ);
			objent->SetCharPos(tmpobj->m_posx, tmpobj->m_posy);
			m_CurrMapView->AddMapObject(objent);
		}

		gGlobal.SetCombinedBlock(pPathFindData, numGuide);
		
	}
	else
	{
		if (gGlobal.g_PathFinder == NULL)
			gGlobal.g_PathFinder = SafeCreateObject( PathFinder );

		gGlobal.g_PathFinder->SetBlockingActive(FALSE);
		InitMiniMap("mapD0108_mini.tga");
	}
}

VOID QGLApplication::ChangeBattleMode()
{
	if (m_mapState->GetStateID() == ST_APP_MAP)
	{
		if (gConfigure.uNetwork)
		{
			RPCContext context;
			gGlobal.g_pCharacter->GetCharNetObject()->TestBattle(gGlobal.pPeerProxy, context);
		}
		else
			EnterBattle();
	}
	else if (m_mapState->GetStateID() == ST_APP_BATTLE)
	{
		if (!gConfigure.uNetwork)
			LeaveBattle();
	}
}
Boolean QGLApplication::Start(VOID)
{
	return (BOOLEAN_TRUE);
}


Boolean QGLApplication::TryConnect(VOID)
{
	return (BOOLEAN_TRUE);
}

Boolean QGLApplication::Stop(VOID)
{
	return (BOOLEAN_TRUE);
}

VOID QGLApplication::OnAccept(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID QGLApplication::OnFull(CONST Listener* listener)
{
}

VOID QGLApplication::OnOpen(CONST Listener* listener)
{
}

VOID QGLApplication::OnOpen(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID QGLApplication::OnClose(CONST Listener* listener)
{
}

VOID QGLApplication::OnClose(CONST Listener* listener, CONST Peer* pPeer)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("disconnected"));

	if( gGlobal.pUIManager && !gGlobal.g_pAccountControl->m_needReLogin && 
		(gGlobal.m_logined == LOGIN_SUCCESSED || gGlobal.m_logined == LOGIN_SUCCESSED_NEED_KICK) )
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "DisconnectConfirm", String());
		if (!gGlobal.g_ForceOutMsg.IsEmpty())
		{
			UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("DisconnectConfirmText");
			if (pText)
			{
				pText->SetText(gGlobal.GetStringTable()->Get(gGlobal.g_ForceOutMsg.c_str()));
				pText->Redraw();
			}
		}
	}

	SafeRelease(gGlobal.pPeerProxy);

	if (gGlobal.g_pAccountControl && gGlobal.g_pAccountControl->m_needReLogin)
	{
		SessionID sessId;
		ProcedureManager::GetInstance().RemoveBySession(sessId);
		gGlobal.g_pLoginProcess->ShowLogin();
		gGlobal.g_pAccountControl->m_needReLogin = false;
	}

	Reuben::Math::AES::ResetKey();

	if (gGlobal.m_logined == LOGIN_WAITING_CHANGESTATE)
	{
		gGlobal.m_logined = LOGIN_VERSION_WRONG;
		STARTUPINFO startupInfo = {0};
		startupInfo.cb = sizeof(startupInfo);
		PROCESS_INFORMATION processInformation;// Try to start the process
		BOOL result = ::CreateProcess(  
			_T("QGLPatcherU.exe"),
			NULL, 
			NULL,  
			NULL,  
			FALSE,  
			NORMAL_PRIORITY_CLASS,  
			NULL,  
			NULL,  
			&startupInfo,  
			&processInformation);

		gGlobal.pUIManager->SetEvent(110);		
	}
	else if (gGlobal.m_logined == LOGIN_ANTIHACKING)
	{
		WorldService_Proxy::CB_LoginResult(gGlobal.pPeerProxy, RPCContext(), LOGIN_ANTIHACKING);
	}
}

CParticleSystem * QGLApplication::InitSnow( CRender * pRender)
{
	CParticleSystem * pParticle = SafeCreateObject(CParticleSystem);
	PixelDirection Dir;
	Dir.PixelX = 0.4f;
	Dir.PixelY = 1.0f;

	ParticleStructSetting PSS;
	PSS.Destory = FALSE;
	PSS.ExtraZValue = ParticleExtraZ;
	PSS.GivenDisposalAnim = 0;
	PSS.GivenDisposalTime = 1000;
	PSS.HaveEffect = FALSE;
	PSS.LiveTimeBase = 5000;
	PSS.LiveTimeVar = 5000;
	PSS.Loop = FALSE;
	PSS.MoveSpeed = gConfigure.uMaxSnowSpeed;
	PSS.NoOfParticle = gConfigure.uMaxNumOfSnow;
	PSS.Stop = FALSE;
	PSS.MoveDir = Dir;

	CAnimatedEntity TempEnt;
	Char FileName[50];
	
	GetPrivateProfileString(_T("PARTICLESYSTEM"), _T("SNOW1"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	TempEnt.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_OTHERS);
	pParticle->AddAnimatedEntityToSystem(&TempEnt, PSS, NULL);
	
	GetPrivateProfileString(_T("PARTICLESYSTEM"), _T("SNOW2"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	TempEnt.InitEntPicInfo();
	TempEnt.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_OTHERS);
	pParticle->AddAnimatedEntityToSystem(&TempEnt, PSS, NULL);
	
	GetPrivateProfileString(_T("PARTICLESYSTEM"), _T("SNOW2"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	TempEnt.InitEntPicInfo();
	TempEnt.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_OTHERS);
	pParticle->AddAnimatedEntityToSystem(&TempEnt, PSS, NULL);
	
	RECT EffectBound;
	EffectBound.right = DefaultWinWidth * 2;
	EffectBound.bottom = DefaultWinHeight * 2;
	EffectBound.left = 0;
	EffectBound.top = 0;
	pParticle->SetRelativeEffectBounding(EffectBound);
	pParticle->SetTarget(gGlobal.g_pCharacter->GetCharMainView());
	return pParticle;
}

VOID QGLApplication::AddLeftClickEntity()
{
	ParticleStructSetting PSS;
	PSS.Destory = TRUE;
	PSS.HaveEffect = FALSE;
	PSS.Loop = FALSE;
	PSS.NoOfParticle = 1;
	PSS.SubOffsetX = (Int)(m_CurrCam.GetLeftUpPos().x + gGlobal.pUIManager->GetCursorX());
	PSS.SubOffsetY = (Int)(m_CurrCam.GetLeftUpPos().y + gGlobal.pUIManager->GetCursorY());

	//Char FileName[50];
	//GetPrivateProfileString(_T("LOADFILE"), _T("CURSORCLK"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);

	CAnimatedEntity Ent;
	Ent.LoadEntityFile(_T("eff_scr_click.ent"), gGlobal.pD3DDevice, EQUIP_OTHERS);

	gGlobal.g_DisposalEntityEffect->AddAnimatedEntityToSystem(&Ent, PSS, NULL);
	
}

VOID QGLApplication::MoveToScreenPos(LONG x, LONG y, BOOL SetDes)
{
	//ENCODE_START;

	VMProtectBegin; VM_START;

	if (gGlobal.m_logined == LOGIN_WAITING_CHANGESTATE && gConfigure.uNetwork != 0)
		return;

	if ( gConfigure.uNetwork && gGlobal.g_pCharacter->GetCharNetObject() &&
		gGlobal.g_pCharacter->GetCharNetObject()->GetAction() == CMapActor::ST_BATTLE)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Err: Client Can't Move Since Action is BATTLE"));
		return;
	}

	if  ( gConfigure.uNetwork && 
			( ( gGlobal.g_pCharacter->GetCharNetObject() && gGlobal.g_pCharacter->GetCharNetObject()->GetAction() == CMapActor::ST_HAWK) ||
			  ( gGlobal.g_pHawkerProcess && gGlobal.g_pHawkerProcess->IsOpenHawkMenu() ) ) )
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Err: Client Can't Move Since Action is HAWK"));
		return;
	}
		
	if (gConfigure.uNetwork && gGlobal.g_pTargetHawkGroup)
		return;

	if ( gConfigure.uNetwork && gGlobal.g_pCharacter->GetCharNetObject() &&
		gGlobal.g_pSkillProcess->GetIsShop())
		return;

	if (!gGlobal.g_pCharacter->GetCharMainView()->GetVisible())
		return;
	VMProtectEnd; VM_END;

	

	if (m_mapState->GetStateID() == ST_APP_MAP && gGlobal.g_pCharacter->GetCharMainView()->GetFollowTarget() == NULL)
	{
		Int BlockPosX = ((Int)m_CurrCam.GetLeftUpPos().x +
			x) / BlockSizeX;
		Int BlockPosY = ((Int)m_CurrCam.GetLeftUpPos().y + 
			y) / (BlockSizeY / 2);
		BlockPos DesPos;
		UInt32 MapWidth = 10000;
		UInt32 MapHeight = 10000;
		UInt32 MaxBlockX = 10000;
		UInt32 MaxBlockY = 10000;

		if (m_CurrMapView->GetMapWidth())
			MapWidth = m_CurrMapView->GetMapWidth();

		if (m_CurrMapView->GetMapHeight())
			MapHeight = m_CurrMapView->GetMapHeight();

		if (m_CurrMapView->GetMapMaxBlockX())
			MaxBlockX = m_CurrMapView->GetMapMaxBlockX();

		if (m_CurrMapView->GetMapMaxBlockY())
			MaxBlockY = m_CurrMapView->GetMapMaxBlockY();

		if (QGLMath::FindBlockByPixel((DWORD)(m_CurrCam.GetLeftUpPos().x + x), 
			DWORD(m_CurrCam.GetLeftUpPos().y + y),
			DesPos, MapWidth, MapHeight
			,MaxBlockX, MaxBlockY))
		{
			if (gGlobal.g_pCharacter)
			{
				gGlobal.g_pCharacter->SetEntityDesPos(DesPos.PosX, DesPos.PosY, SetDes);
				
				if (gGlobal.g_pMiddleMap->IsOpenMiddleMap())
					gGlobal.g_pMiddleMap->DrawPath();
				else
				{
					CCrossMapPathFinder::GetInstance()->DestroyPathData();
					gGlobal.m_u32DestNpcId = 0;
					gGlobal.g_pMiddleMap->SetCharDes(DesPos.PosX, DesPos.PosY);
					gGlobal.g_pMiddleMap->SetPathEnd(DesPos.PosX, DesPos.PosY);
				}	

				AddLeftClickEntity();
			}
		}
	}

	//ENCODE_END;
}

VOID QGLApplication::UpdateBattleCatchClick(LONG x, LONG y)
{
	if (m_mapState->GetStateID() == ST_APP_BATTLE)
	{
		if (m_SelectedMapControl)
		{
			if (gGlobal.g_pBattleControl)
			{
				if(gGlobal.g_pBattleControl->GetBattleMapData().State == BATTLE_STATE_INWAR)
				{
					//check whether it is network mode
					if (gConfigure.uNetwork)
					{
						if (gGlobal.g_pBattleControl->IsSendCommandAvial())
						{
							CCharacterControl * pChar = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
							if (pChar)
							{
								//g_Global.g_pBattleControl->ProtectSeletedChar(pChar);
								gGlobal.g_pBattleControl->CatchSelectedChar(pChar);
								gGlobal.g_CursorState->Unblock();

								if (pChar->GetBatActorObject()->GetTeam() != gGlobal.g_pBattleControl->GetControlPlayerTeam())
								{
									State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
									State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIAttackState));
								}
								else
									State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
							}
							
						}

					}
				}
			}
		}

	}

}



VOID QGLApplication::UpdateBattleClick(LONG x, LONG y)
{
	if (m_mapState->GetStateID() == ST_APP_BATTLE)
	{
		if (m_SelectedMapControl)
		{
			if (gGlobal.g_pBattleControl)
			{
				//if(gGlobal.g_pBattleControl->GetBattleMapData().State == BATTLE_STATE_INWAR)
				{
					//check whether it is network mode
					if (gConfigure.uNetwork)
					{
						//if (gGlobal.g_pBattleControl->IsSendCommandAvial())
						{
						//	gGlobal.g_pBattleControl->SetCommandAvial(FALSE);
							CCharacterControl * pChar = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
							gGlobal.g_pBattleControl->AttackSeletedChar(pChar);
							return;
						}
						//else
						//	gGlobal.g_pBattleControl->SetCommandAvial(TRUE);
					}
					else
					{
						Int AttackType = GetPrivateProfileInt(_T("BATTLESETTING"), _T("TESTATTACKTYPE"), 0, CONFIGURATION_INI_FILE);
						if (AttackType == 0)
						{
							static Int AttackAnimation = 0;
							if (AttackAnimation == 0)
							{
								gGlobal.g_pBattleControl->TestAttack(GetSelectedControl()->GetCharMainView());
								AttackAnimation = 1;
							}
							else
							{
								gGlobal.g_pBattleControl->TestJumpAttack(GetSelectedControl()->GetCharMainView());
								AttackAnimation = 0;
							}
						}
						else if (AttackType == 1)
							gGlobal.g_pBattleControl->TestCombinedAttack(GetSelectedControl()->GetCharMainView());
						else if (AttackType == 2)
							gGlobal.g_pBattleControl->TestSkillAttack(GetSelectedControl()->GetCharMainView(), -1,
							SKILL_ANIM_TYPE_ORG_POS, TRUE, SKILL_ANIM_MOTION_SKILL, TRUE);
						else if (AttackType == 3)
							//gGlobal.g_pBattleControl->TestSingleSkillAttack(GetSelectedControl()->GetCharMainView(), 1);
							gGlobal.g_pBattleControl->TestSkillAttack(GetSelectedControl()->GetCharMainView(), -1,
							SKILL_ANIM_TYPE_ORG_POS, FALSE, SKILL_ANIM_MOTION_SKILL, TRUE);
						else if (AttackType == 4)
							//gGlobal.g_pBattleControl->TestAreaAttackCenter(GetSelectedControl()->GetCharMainView(), 1);
							gGlobal.g_pBattleControl->TestSkillAttack(GetSelectedControl()->GetCharMainView(), -1,
							SKILL_ANIM_TYPE_CENTER_POS, TRUE, SKILL_ANIM_MOTION_SKILL, TRUE);
						/*else if (AttackType == 5)
						{
						static Int SkillIndexCount = 1;

						if (SkillIndexCount >= FullscreenEffectIndex)
						gGlobal.g_pBattleControl->TestAreaAttack(GetSelectedControl()->GetCharMainView(), SkillIndexCount);
						else
						gGlobal.g_pBattleControl->TestSingleSkillAttack(GetSelectedControl()->GetCharMainView(), SkillIndexCount);

						SkillIndexCount ++;
						if (SkillIndexCount >= MaxTestEffect)
						SkillIndexCount = 1;
						}
						*/
						else if (AttackType == 5)
						{
							//gGlobal.g_pBattleControl->TestSkillRunAttack(GetSelectedControl()->GetCharMainView(), 1);
							gGlobal.g_pBattleControl->TestSkillAttack(GetSelectedControl()->GetCharMainView(), -1,
								SKILL_ANIM_TYPE_TARGET_POS, TRUE, SKILL_ANIM_MOTION_ATK2, FALSE);
						}
						else if (AttackType == 6)
						{
							//gGlobal.g_pBattleControl->TestSkillCenterAtkAnim(GetSelectedControl()->GetCharMainView(), 1);
							gGlobal.g_pBattleControl->TestSkillAttack(GetSelectedControl()->GetCharMainView(), -1,
								SKILL_ANIM_TYPE_CENTER_POS, TRUE, SKILL_ANIM_MOTION_ATK2, FALSE);
						}

						return;
					}

					FSound* sfx = NULL;
					String Temp;
					Temp.Format(_T("%sSoundI0026_atk_command.wav"), WAV_FILE_PATH);
					sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
					if (sfx)
					{
						sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
						sfx->Play();
					}
				}
			}
		}

		return;
	}
}

VOID QGLApplication::UpdateProtectClick(LONG x, LONG y)
{
	if (m_mapState->GetStateID() == ST_APP_BATTLE)
	{
		if (m_SelectedMapControl)
		{
			if (gGlobal.g_pBattleControl)
			{
				if(gGlobal.g_pBattleControl->GetBattleMapData().State == BATTLE_STATE_INWAR)
				{
					//check whether it is network mode
					if (gConfigure.uNetwork)
					{
						if (gGlobal.g_pBattleControl->IsSendCommandAvial())
						{
							CCharacterControl * pChar = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
							gGlobal.g_pBattleControl->ProtectSeletedChar(pChar);
							gGlobal.g_CursorState->Unblock();
							State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
							
							//gGlobal.g_pBattleControl->AttackSeletedEntity(GetSelectedControl()->GetCharMainView());
							return;
						}
					}

					FSound* sfx = NULL;
					String Temp;
					Temp.Format(_T("%sSoundI0026_atk_command.wav"), WAV_FILE_PATH);
					sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
					if (sfx)
					{
						sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
						sfx->Play();
					}
				}
			}
		}
	}
}

VOID QGLApplication::UpdateBattleSkillClick(LONG x, LONG y)
{
	if (m_SelectedMapControl)
	{
		if (m_mapState->GetStateID() == ST_APP_BATTLE &&
			gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MONITOR)
		{
			if (gGlobal.g_pBattleControl)
			{
				if(gGlobal.g_pBattleControl->GetBattleMapData().State == BATTLE_STATE_INWAR)
				{
					//check whether it is network mode
					if (gConfigure.uNetwork)
					{
						if (gGlobal.g_pBattleControl->IsSendCommandAvial())
						{
							if (gGlobal.g_pBattleControl->GetControlPlayer() && !gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand())
							{
								if (gGlobal.g_pSkillProcess->m_pTempSkillData && gGlobal.g_pSkillProcess->m_pTempSkillData->Skill_ThrowItemType != 255 &&
									gGlobal.g_pBattleControl->GetBattleItemUseIndex() == -1)
								{
									gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_ITEM_NOT_SELECT")), CHANNELTYPE_SYSTEM);
								}
								else
								{
									CCharacterControl * pChar = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
									if (pChar)
									{
										gGlobal.g_pBattleControl->SkillUseOnSeletedChar(pChar);
										gGlobal.g_CursorState->Unblock();
										FSound* sfx = NULL;
										String Temp;
										Temp.Format(_T("%sSoundI0026_atk_command.wav"), WAV_FILE_PATH);
										sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
										if (sfx)
										{
											sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
											sfx->Play();
										}

										if (pChar->GetBatActorObject()->GetTeam() != gGlobal.g_pBattleControl->GetControlPlayerTeam())
										{
											State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
											State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIAttackState));
										}
										else
											State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
										//gGlobal.g_pBattleControl->AttackSeletedEntity(GetSelectedControl()->GetCharMainView());

										CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePanel", String());
										return;
									}
								}
							}
							else if (gGlobal.g_pBattleControl->GetControlPartner() && gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() && !gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand())
							{
								if (gGlobal.g_pPartnerProcess->GetSelSkillData() && gGlobal.g_pPartnerProcess->GetSelSkillData()->Skill_ThrowItemType != 255 &&
									gGlobal.g_pBattleControl->GetBattleItemUseIndex() == -1)
								{
									gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_ITEM_NOT_SELECT")), CHANNELTYPE_SYSTEM);
								}
								else
								{
									CCharacterControl * pChar = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
									if (pChar)
									{
										gGlobal.g_pBattleControl->SkillUseOnSeletedChar(pChar);
										gGlobal.g_CursorState->Unblock();
										FSound* sfx = NULL;
										String Temp;
										Temp.Format(_T("%sSoundI0026_atk_command.wav"), WAV_FILE_PATH);
										sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
										if (sfx)
										{
											sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
											sfx->Play();
										}

										if (pChar->GetBatActorObject()->GetTeam() != gGlobal.g_pBattleControl->GetControlPlayerTeam())
										{
											State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
											State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIAttackState));
										}
										else
											State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
										//gGlobal.g_pBattleControl->AttackSeletedEntity(GetSelectedControl()->GetCharMainView());

										CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePartnerPanel", String());
										return;
									}
								}
							}
						}
					}
					else
					{
						
						{
							CCharacterControl * pChar = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
							gGlobal.g_pBattleControl->SkillUseOnSeletedChar(pChar);
							gGlobal.g_CursorState->Unblock();
							State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
							CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePanel", String());
						}
					}
				}
			}
		}
		else if (m_mapState->GetStateID() == ST_APP_MAP &&
			gGlobal.g_pSkillProcess->m_pTempSkillData)
		{
			CCharacterControl * pChar = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);

			if (pChar)
			{
				if (gGlobal.g_pCharacter->GetCharNetObject())
				{
					gGlobal.g_pCharacter->GetCharNetObject()->UseSkill(gGlobal.pPeerProxy, gGlobal.g_context, 
						gGlobal.g_pSkillProcess->m_pTempSkillData->id, pChar->GetMapActorID(), 0);
				}
				gGlobal.g_CursorState->Unblock();
				State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
			}
		}
	}

}
VOID QGLApplication::UpdateBattleUseItemClick(LONG x, LONG y, Boolean bCancel)
{
	Boolean cancel = bCancel;
	if ( !cancel )
	{// select use item target
		cancel = BOOLEAN_TRUE;
		if ( (m_mapState->GetStateID() == ST_APP_BATTLE) && m_SelectedMapControl && gGlobal.g_pBattleControl 
			&& gConfigure.uNetwork )
		{
			if( gGlobal.g_pBattleControl->GetBattleMapData().State == BATTLE_STATE_INWAR )
			{
				if ( gGlobal.g_pBattleControl->GetBattleItemUseIndex() != -1 )
				{
					CCharacterControl * pChar = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
					if ( pChar )
					{
						gGlobal.g_pBattleControl->UseBattleItemForSelectedChar(pChar);
						cancel = BOOLEAN_FALSE;
					}
				}
			}
		}
	}

	if ( cancel && gGlobal.g_pBattleControl )
	{// cancel use item in battle
		gGlobal.g_pBattleControl->OnCancelUseItemInBattle();
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CANCEL_BATTLEITEMUSE")), CHANNELTYPE_SYSTEM, String());
	}

	gGlobal.g_CursorState->Unblock();
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
}

VOID QGLApplication::UpdateShopSellClick(LONG x, LONG y)
{
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->OnLButtonUpForSell();
}

VOID QGLApplication::OnClickNPC()
{
	if ( m_ClickNPCMapControl )
	{
		CMapNPCControl * pMapNPCCtrl = dynamic_cast<CMapNPCControl *>(m_ClickNPCMapControl);
		if ( pMapNPCCtrl )
		{
			if ( pMapNPCCtrl->OnClickOnCtrl() )
				m_TalkingMapControl = m_ClickNPCMapControl;
			else
				pMapNPCCtrl->StopNPCTalkingState();

			m_ClickNPCMapControl = NULL;
			gGlobal.g_bClickNPCFlag = FALSE;
			gGlobal.g_pMiddleMap->ResetCharDes();
			CCrossMapPathFinder::GetInstance()->DestroyPathData();
			gGlobal.m_u32DestNpcId = 0;
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseMiddleMap", String());
		}		
	}
}

VOID QGLApplication::ResumeClickNPCAction(UInt16 x, UInt16 y)
{
	if ( !gGlobal.g_bClickNPCFlag ) return;

	if ( m_ClickNPCMapControl )
	{
		BlockPos charpos;
		charpos.PosX = x;
		charpos.PosY = y;

		PixelDirection d;
		UInt32 dis = QGLMath::CalculateBlockPosDistance(m_ClickNPCMapControl->GetCharMainView()->GetCharPos(), charpos, d);
		if ( dis < (UInt32) gConfigure.nValidRangeOfTalk )
		{
			if ( gGlobal.g_pCharacter->GetCharNetObject()->GetAction() != CMapActor::ST_BATTLE && 
				gGlobal.g_pCharacter->GetCharNetObject()->GetAction() != CMapActor::ST_JUMP_MAP &&
				( gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NPC || gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NORMAL ) )
			{
				OnClickNPC();
			}
			else 
			{
				gGlobal.g_bClickNPCFlag = FALSE;
				m_ClickNPCMapControl = NULL;
			}
		}
		else
		{
			//String WarningStr(_T(""));
			//WarningStr.Format(_T("at least 80 pixel, diff:%d"), dis);
			//gGlobal.PrintMessage(WarningStr, CHANNELTYPE_SYSTEM);
		}
	}
	else
		gGlobal.g_bClickNPCFlag = FALSE;

}

VOID QGLApplication::OnDisappearNPCDialogue()
{
	UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("DialogueBox");
	if ( pPanelEx && pPanelEx->IsEnable() )
	{
		if ( gGlobal.g_pScriptControl->IsLockedWhenTalking() == BOOLEAN_TRUE )
			return;

		// [calc by pixel]
		BlockPos charpos = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
		BlockPos center;
		center.PosX = gGlobal.g_pScriptControl->GetDisappearX();
		center.PosY = gGlobal.g_pScriptControl->GetDisappearY(); 
		//center.PosX = gGlobal.m_OpenDialoguePosX;
		//center.PosY = gGlobal.m_OpenDialoguePosY;
		PixelDirection dummy;

		if (QGLMath::CalculateBlockPosDistance(charpos, center, dummy) > 300)
		{
			gGlobal.g_pScriptControl->TerminateTalkingDialogue(BOOLEAN_TRUE); 
		}

		/*
		//[Calc by Block]
		Int32 charX = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
		Int32 charY = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;
		Int32 npcX = gGlobal.m_OpenDialoguePosX;
		Int32 npcY = gGlobal.m_OpenDialoguePosY;

		Int32 diffresult = sqrt( (double)((charX - npcX) * (charX - npcX) + (charY - npcY) * (charY - npcY)) );
		if ( diffresult > 25 )
		{
		gGlobal.g_pScriptControl->TerminateTalkingDialogue(BOOLEAN_TRUE);
		}
		*/
	}
}

VOID QGLApplication::CountDownForPartyChoose()
{
	if ( gGlobal.g_pScriptControl )
	{
		Int count = gGlobal.g_pScriptControl->CountDown();
		if ( count != -1 )
		{
			gGlobal.m_ChooseCountDown.SetNumber((UInt32)((gConfigure.uChooseRemainTime/1000) - count));
			gGlobal.m_ChooseCountDown.SetVisible(TRUE);
			gGlobal.m_ChooseCountDown.SetEntPixelPos(PixelPos((Int)m_CurrCam.GetLeftUpPos().x +
				GetRenderClass()->GetWinWidth() / 2, (Int)m_CurrCam.GetLeftUpPos().y +
				100));

			for (size_t i = 0; i < gGlobal.m_ChooseCountDown.m_NumEntLst.size(); i++)
			{
				//gGlobal.m_PartyCount.m_NumEntLst[i]->UpDate(::timeGetTime());
				AddEntityToLst(gGlobal.m_ChooseCountDown.m_NumEntLst[i], m_EntRenderLst);
			}
		}
	}
}

VOID QGLApplication::UpdateNPCClick(LONG x, LONG y)
{
	{// check follow
		CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
		if ( pchar && (pchar->GetFollowId() != 0) )
			return;
	}

	{// check shop menu open or not
		UIItem* uim = gGlobal.pUIManager->GetUIItem("ShopMenuPanel");
		if ( uim && uim->IsEnable() )
			return;
	}


	{// check talking 
		UIItem* uim = gGlobal.pUIManager->GetUIItem("DialogueBox");
		if ( uim && uim->IsEnable() )
			return;
	}

	CMapActorControl * pTempControl = FindMapActCtrlInWindow(x, y, m_CurrMapView->GetMapWidth());
	if ( !pTempControl ) return;

	//gGlobal.m_OpenDialoguePosX = pTempControl->GetCharMainView()->GetCharPos().PosX;
	//gGlobal.m_OpenDialoguePosY = pTempControl->GetCharMainView()->GetCharPos().PosY;
	
	if ( gGlobal.g_pCharacter->GetCharMainView()->GetFollowTarget() == NULL )
	{
		m_ClickNPCMapControl = pTempControl;

		if ( m_ClickNPCMapControl != NULL )
		{
			OnClickNPC();
		}
	}
}

VOID QGLApplication::TalkToNpcByPathFinder(CMapNPCControl* pMapNpc)
{
	if (NULL == pMapNpc) 
		return;
	{// check follow
		CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
		if ( pchar && (pchar->GetFollowId() != 0) )
			return;
	}

	{// check shop menu open or not
		UIItem* uim = gGlobal.pUIManager->GetUIItem("ShopMenuPanel");
		if ( uim && uim->IsEnable() )
			return;
	}


	{// check talking 
		UIItem* uim = gGlobal.pUIManager->GetUIItem("DialogueBox");
		if ( uim && uim->IsEnable() )
			return;
	}

	//gGlobal.m_OpenDialoguePosX = pTempControl->GetCharMainView()->GetCharPos().PosX;
	//gGlobal.m_OpenDialoguePosY = pTempControl->GetCharMainView()->GetCharPos().PosY;

	if ( gGlobal.g_pCharacter->GetCharMainView()->GetFollowTarget() == NULL )
	{
		m_ClickNPCMapControl = pMapNpc;

		if ( m_ClickNPCMapControl != NULL )
		{
			OnClickNPC();
		}
	}
}

VOID QGLApplication::UpdateWholeMemberClickNPC(String filename)
{
	if ( m_ClickNPCMapControl )
	{
		CMapNPCControl * pMapNPCCtrl = dynamic_cast<CMapNPCControl *>(m_ClickNPCMapControl);
		if ( pMapNPCCtrl )
		{
			Boolean bResult(BOOLEAN_FALSE);
			//if ( gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NPC || 
			//	gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NORMAL )
			{
				pMapNPCCtrl->ChangeRotation(QGLMath::GetRotationFromTwoPoint(pMapNPCCtrl->GetCharMainView()->GetCharPos().PosX,
					pMapNPCCtrl->GetCharMainView()->GetCharPos().PosY, gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX,
					gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY));

				//if ( State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINPCTalkingState)) )
				{
					//gGlobal.g_CursorState->Block();

					if (pMapNPCCtrl)
					{
						//gGlobal.g_pScriptControl->GetScriptSysNetObject()->OnResumeExecuteAll(gGlobal.pPeerProxy, gGlobal.g_context, 
						//	pMapNPCCtrl->GetTemplateID(), pMapNPCCtrl->GetMapActorID(), pMapNPCCtrl->GetNetObject()->GetMap_id(), filename);
						//if (gGlobal.g_pCharacter)
						//	gGlobal.g_pCharacter->Stop();

						//pMapNPCCtrl->GetNetObject()->OnResumeExecuteAll(gGlobal.pPeerProxy, RPCContext());
						//m_pMapNPC->OnClick(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_pCharacter->GetMapActorID());
						bResult = BOOLEAN_TRUE;
					}
				}
			}
			///
			if ( bResult )
				m_TalkingMapControl = m_ClickNPCMapControl;
			else
				pMapNPCCtrl->StopNPCTalkingState();

			m_ClickNPCMapControl = NULL;
			gGlobal.g_bClickNPCFlag = FALSE;
		}		
	}
}

VOID QGLApplication::UpdateNormalClick(LONG x, LONG y)
{
	CCharacterControl* pTarget = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
	if (pTarget)
	{
		gGlobal.g_pCharacter->SetTarget(pTarget);	
	}
	//else
        MoveToScreenPos(x, y, TRUE);

}

VOID QGLApplication::UpdateNPCtalkingClick(LONG x, LONG y)
{
	UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("DialogueBox");
	if ( pPanelEx && pPanelEx->IsEnable() )
	{
		MoveToScreenPos(x, y, TRUE);
	}
	else
	{// No Dialogue 
		
		// Is script running ?
		if ( gGlobal.g_pScriptControl->IsFinished() )
		{// finish 
			gGlobal.g_pScriptControl->TerminateTalkingDialogue();
		}
		else
		{// not finish, do nothing
		}	
	}
	//	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ClickNextMsg");
}

VOID QGLApplication::UpdatePartyClick(LONG x, LONG y)
{
	if (m_SelectedMapControl)
	{
		CCharacterControl * pCharCtrl = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
		if (pCharCtrl)
		{
			if( pCharCtrl->GetMapActorID()==gGlobal.g_pCharacter->GetMapActorID() )
				CreateParty();
			else
			{
				if (gGlobal.g_pParty && gGlobal.g_pParty->IsLeader())
					InviteParty(pCharCtrl->GetMapActorID());
				else
					JoinParty(pCharCtrl->GetMapActorID());
			}
			gGlobal.g_CursorState->Unblock();

			State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));

			return;
		}
	}
}

VOID QGLApplication::UpdatePKAttackClick(LONG x, LONG y)
{
	CCharacterControl * pCharCtrl = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
	if (pCharCtrl)
	{
		if (pCharCtrl->GetCharNetObject() &&
		gGlobal.g_pCharacter->GetCharNetObject())
		gGlobal.g_pCharacter->GetCharNetObject()->PKRequest(gGlobal.pPeerProxy, gGlobal.g_context, pCharCtrl->GetMapActorID(), PK_BATTLE);
		gGlobal.g_TempPKTargetID = pCharCtrl->GetMapActorID();
		gGlobal.g_CursorState->Unblock();

		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}
}

VOID QGLApplication::UpdateGivenClick(LONG x, LONG y)
{
	CCharacterControl * pCharCtrl = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
	if (pCharCtrl)
	{
		if (pCharCtrl->GetCharNetObject() && gGlobal.g_pCharacter->GetCharNetObject() && 
			pCharCtrl != gGlobal.g_pCharacter &&  gGlobal.g_pGivenProcess)
		{
			CMessenger* messenger = NULL;
			if (gGlobal.g_pMenuFriendProcess)
					messenger = gGlobal.g_pMenuFriendProcess->FindMessenger(pCharCtrl->GetCharNetObject()->GetChar_id() );
				
			if (pCharCtrl->GetCharNetObject()->GetEnableBlockGiven() )
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_BLOCK_GIVEN")), CHANNELTYPE_SYSTEM);
			else 
			{
				if (pCharCtrl->GetCharNetObject()->GetAction() != CCharacter::ST_BATTLE || (messenger && messenger->GetFriend_relation() > 0))
					gGlobal.g_pGivenProcess->OpenGivenPanel(pCharCtrl->GetCharName(), pCharCtrl->GetMapActorID(), pCharCtrl->GetCharNetObject()->GetNetId() );
				else
					gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_NOT_FRIEND")), CHANNELTYPE_SYSTEM);
			}
		}
		else
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_MYSELF")), CHANNELTYPE_SYSTEM);
		}
		
		gGlobal.g_CursorState->Unblock();

		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}
}

VOID QGLApplication::UpdateTradingClick(LONG x, LONG y)
{
	CCharacterControl * pCharCtrl = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
	if (pCharCtrl)
	{
		if (pCharCtrl->GetCharNetObject() && gGlobal.g_pCharacter->GetCharNetObject() && 
			pCharCtrl != gGlobal.g_pCharacter && pCharCtrl->GetCharNetObject()->GetAction() != CCharacter::ST_BATTLE)
		{
			if (pCharCtrl->GetCharNetObject()->GetEnableBlockTrading() )
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_BLOCK_TRADING")), CHANNELTYPE_SYSTEM);
			else
			{
				gGlobal.g_pCharacter->GetCharNetObject()->AskForTrading(gGlobal.pPeerProxy, RPCContext(), pCharCtrl->GetCharNetObject()->GetNetId());
				if (gGlobal.g_pTradingProcess)
					gGlobal.g_pTradingProcess->SetTargetNetID(pCharCtrl->GetCharNetObject()->GetNetId());

				if (gGlobal.g_pMenuFriendProcess)
				{
					CMessenger* messenger = gGlobal.g_pMenuFriendProcess->FindMessenger(pCharCtrl->GetCharNetObject()->GetChar_id() );
					if (messenger == NULL || messenger->GetFriend_relation() == 0 )
					{
						gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_NOT_FRIEND_REQ")));
					}
				}
			}
		}
		else
		{
			if (pCharCtrl->GetCharNetObject()->GetAction() == CCharacter::ST_BATTLE)
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_BATTLE_FALSE")), CHANNELTYPE_SYSTEM);
			else
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_MYSELF")), CHANNELTYPE_SYSTEM);
		}

		gGlobal.g_CursorState->Unblock();

		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}
}

VOID QGLApplication::UpdateFriendClick(LONG x, LONG y)
{
	CCharacterControl * pCharCtrl = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
	if (pCharCtrl)
	{
		if (GetMapStateID() == ST_APP_BATTLE )
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_ADD_FAIL_BATTLE")), CHANNELTYPE_SYSTEM);
		}
		else if (pCharCtrl->GetCharNetObject() && gGlobal.g_pCharacter->GetCharNetObject() && 
			pCharCtrl != gGlobal.g_pCharacter)
		{
			if (pCharCtrl->GetCharNetObject()->GetEnableBlockMakeFriend() )
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_BLOCK_FRIEND")), CHANNELTYPE_SYSTEM);
			else
			{
				gGlobal.g_pMenuFriendProcess->m_TargetCharID = pCharCtrl->GetMapActorID();
				gGlobal.g_pMenuFriendProcess->m_TargetCharName = pCharCtrl->GetCharName();
				if (gGlobal.g_pMenuFriendProcess->m_CurrGroup == FRIEND_GROUP_TEMP)
				{
					gGlobal.g_pMenuFriendProcess->InserTempFriend(pCharCtrl->GetMapActorID(),
					pCharCtrl->GetCharName(), FRIENC_CHAT_STATUS_ONLINE);
					String text;
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_ADD_SUCCESS")), pCharCtrl->GetCharName());
					gGlobal.PrintPlayerMessage(text);
				}
				else
					gGlobal.g_pMenuFriendProcess->AddFriend(gGlobal.g_pMenuFriendProcess->m_CurrGroup);
			}
		}
		else if (pCharCtrl == gGlobal.g_pCharacter)
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_ADD_YOURSELF")), CHANNELTYPE_SYSTEM);
		}
		else
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_ADD_FAIL_INVALID")), CHANNELTYPE_SYSTEM);
		
		gGlobal.g_CursorState->Unblock();

		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}
}
VOID QGLApplication::UpdateLeftClick(LONG x, LONG y)
{
	if (gGlobal.g_CursorState == NULL)
		return;

	if (!gGlobal.g_ClickEnable)
		return;

	gGlobal.g_bClickNPCFlag = FALSE;
	m_ClickNPCMapControl = NULL;

	if (gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NORMAL)
		UpdateNormalClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_PARTY)
		UpdatePartyClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_ATTACK)
		UpdateBattleClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NPC)
		UpdateNPCClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_TALKING)
		UpdateNPCtalkingClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_PROTECT)
		UpdateProtectClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_USEITEM)
		UpdateBattleUseItemClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_SKILL)
		UpdateBattleSkillClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_PK_ATTACK)
		UpdatePKAttackClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_CATCH)
		UpdateBattleCatchClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_SHOP_SELL)
		UpdateShopSellClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_GIVEN)
		UpdateGivenClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_TRADING)
		UpdateTradingClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_FRIEND)
		UpdateFriendClick(x, y);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_GENERATION)
		UpdateGenerationClick(x,y);


	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OnLeftClick", String());
}

VOID QGLApplication::JumpEntity(CAnimatedEntity * pEnt)
{
	if (pEnt == NULL)
		return;

	if (pEnt->m_animationset.size() == 0)
	{
		StlVector<animationsetData> Givenanimationset;
		pEnt->SetJumpData(100, 0, 0, 250);
		pEnt->SetAnimationIndexByIndex(ANIM_JUMP, FALSE);

		animationsetData AniData;
		AniData.AnimationIndex = ANIM_JUMP_END;
		AniData.StartTime = ::timeGetTime() + 250;
		AniData.Loop = FALSE;
		AniData.Rotation = -1;
		Givenanimationset.push_back(AniData);

		AniData.AnimationIndex = ANIM_IDLE1;
		AniData.StartTime = AniData.StartTime + 250 + pEnt->GetTotalTimeOfGivenAnimation(ANIM_JUMP_END, pEnt->GetAnimationRotation());
		AniData.Loop = TRUE;
		AniData.Rotation = -1;
		Givenanimationset.push_back(AniData);

		//pEnt->Setanimationset(Givenanimationset);
	}
}

CParticleSystem * QGLApplication::InitRain( CRender * pRender)
{
	CParticleSystem * pParticle = SafeCreateObject(CParticleSystem);
	PixelDirection Dir;
	Dir.PixelX = -0.4f;
	Dir.PixelY = 1.0f;

	ParticleStructSetting PSS;
	PSS.Destory = FALSE;
	PSS.ExtraZValue = ParticleExtraZ;
	PSS.GivenDisposalAnim = ANIM_DIE;
	PSS.GivenDisposalTime = 0;
	PSS.HaveEffect = FALSE;
	PSS.LiveTimeBase = 250;
	PSS.LiveTimeVar = 250;
	PSS.Loop = FALSE;
	PSS.MoveSpeed = gConfigure.uMaxRainSpeed;
	PSS.NoOfParticle = gConfigure.uMaxNumOfRain;

	PSS.Stop = FALSE;
	PSS.MoveDir = Dir;

	CAnimatedEntity pTempEnt;
	Char FileName[50];
	
	GetPrivateProfileString(_T("PARTICLESYSTEM"), _T("RAIN1"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	pTempEnt.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_OTHERS);
	pParticle->AddAnimatedEntityToSystem(&pTempEnt, PSS, NULL);
	
	GetPrivateProfileString(_T("PARTICLESYSTEM"), _T("RAIN2"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	pTempEnt.InitEntPicInfo();
	pTempEnt.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_OTHERS);
	pParticle->AddAnimatedEntityToSystem(&pTempEnt, PSS, NULL);
	
	GetPrivateProfileString(_T("PARTICLESYSTEM"), _T("RAIN3"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	pTempEnt.InitEntPicInfo();
	pTempEnt.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_OTHERS);
	pParticle->AddAnimatedEntityToSystem(&pTempEnt, PSS, NULL);
	
	RECT EffectBound;
	EffectBound.right = DefaultWinWidth * 2;
	EffectBound.bottom = DefaultWinHeight * 2;
	EffectBound.left = 0;
	EffectBound.top = 0;
	pParticle->SetRelativeEffectBounding(EffectBound);
	pParticle->SetTarget(gGlobal.g_pCharacter->GetCharMainView());
	return pParticle;
}

VOID QGLApplication::AddTestPlayer()
{
	if (m_mapState->GetStateID() == ST_APP_BATTLE)
		return;

	BlockPos BkCharPos;
	BlockPos BkCharDesPos;

	BkCharPos.PosX = (WORD)((rand() % DefaultWinWidth + (WORD)m_CurrCam.GetLeftUpPos().x) / BlockSizeX) ;
	BkCharPos.PosY = (WORD)((rand() % DefaultWinHeight + (WORD)m_CurrCam.GetLeftUpPos().y )/ (BlockSizeY / 2)) ;

	CAnimatedEntity * TestPlayer = SafeCreateObject(CAnimatedEntity);
	*TestPlayer = *gGlobal.g_pCharacter->GetCharMainView();
	TestPlayer->RemoveAllChildren();

	TestPlayer->SetCharPos(BkCharPos.PosX, BkCharPos.PosY);

	if (TestPlayerLst.size() == 0)
		TestPlayer->SetFollowTarget(gGlobal.g_pCharacter->GetCharMainView());
	else
		TestPlayer->SetFollowTarget(TestPlayerLst[TestPlayerLst.size() - 1]);
	TestPlayerLst.push_back(TestPlayer);


/*	CCharacterControl * pCrtlPlayer = SafeCreateObject(CCharacterControl);
	pCrtlPlayer->LoadDefaultPart2();
	pCrtlPlayer->InitShadow();
	Char FileName[50];
	GetPrivateProfileString(_T("FONT"), _T("CHARNAME"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	StringA strTemp = gConfigure.strFontStyle.c_str();
	pCrtlPlayer->GetCharNameView()->LoadText(FileName, 
		strTemp.c_str(), gConfigure.uNameFontSize, D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
			gConfigure.uNameFontColorG, gConfigure.uNameFontColorB), EDGE_SHADOW);

	AddCharCtrlToMap(pCrtlPlayer);

	//pCrtlPlayer->GetCharMainView()->SetCharPos(gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX,
	//	gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY);

	pCrtlPlayer->GetCharMainView()->SetCharPos(BkCharPos.PosX, BkCharPos.PosY);
	pCrtlPlayer->GetCharMainView()->SetRotation(gGlobal.g_pCharacter->GetCharMainView()->GetAnimationRotation());
	pCrtlPlayer->GetCharMainView()->SetAnimationIndexByIndex(gGlobal.g_pCharacter->GetCharMainView()->GetAnimationIndex());

	//pCrtlPlayer->FollowTarget(gGlobal.g_pCharacter);*/

}

VOID QGLApplication::RemoveTestPlayer()
{
	if (m_mapState->GetStateID() == ST_APP_BATTLE)
		return;

	if (TestPlayerLst.begin() != TestPlayerLst.end())
	{
		StlVector<CEntityBase *>::iterator TPItr = TestPlayerLst.begin();
		for (Int i = 0; i < (Int)TestPlayerLst.size() - 1; i++)
			TPItr ++;
		//RemoveEntity(*TPItr);
		SafeDeleteObject(*TPItr);
		TestPlayerLst.erase(TPItr);
	}
}

CParticleSystem * QGLApplication::InitFog( CRender * pRender)
{
	PixelDirection Dir;
	Dir.PixelX = 1.0f;
	Dir.PixelY = 0;

	CParticleSystem * pParticle = SafeCreateObject(CParticleSystem);//pRender->AddParticleSystem();

	ParticleStructSetting PSS;
	PSS.Destory = FALSE;
	PSS.ExtraZValue = ParticleExtraZ;
	PSS.GivenDisposalAnim = 0;
	PSS.GivenDisposalTime = 0;
	PSS.HaveEffect = FALSE;
	PSS.LiveTimeBase = 5000;
	PSS.LiveTimeVar = 5000;
	PSS.Loop = FALSE;
	PSS.MoveSpeed = gConfigure.uMaxFogSpeed;
	PSS.NoOfParticle = gConfigure.uMaxNumOfFog;
	PSS.Stop = FALSE;
	PSS.MoveDir = Dir;
	PSS.ResetOutSideScreen = TRUE;

	CAnimatedEntity TempEnt;
	Char FileName[50];
	GetPrivateProfileString(_T("PARTICLESYSTEM"), _T("FOG1"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	TempEnt.InitEntPicInfo();
	TempEnt.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_OTHERS);
	pParticle->AddAnimatedEntityToSystem(&TempEnt, PSS, NULL);
	
	GetPrivateProfileString(_T("PARTICLESYSTEM"), _T("FOG2"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	TempEnt.InitEntPicInfo();
	TempEnt.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_OTHERS);
	pParticle->AddAnimatedEntityToSystem(&TempEnt, PSS, NULL);
	
	GetPrivateProfileString(_T("PARTICLESYSTEM"), _T("FOG3"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	TempEnt.InitEntPicInfo();
	TempEnt.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_OTHERS);
	pParticle->AddAnimatedEntityToSystem(&TempEnt, PSS, NULL);
	
	RECT EffectBound;
	EffectBound.right = DefaultWinWidth * 2;
	EffectBound.bottom = DefaultWinHeight * 2;
	EffectBound.left = 0;
	EffectBound.top = 0;
	pParticle->SetRelativeEffectBounding(EffectBound);
	pParticle->SetTarget(gGlobal.g_pCharacter->GetCharMainView());

	return pParticle;

}
VOID QGLApplication::AddAllDirtyRect()
{
	GetRenderClass()->AddAllDirtyRect();
}
VOID QGLApplication::ChangeViewingTarget(CEntityBase * pEnt, BOOL MoveImm)
{
	if (pEnt)
	{
		m_CurrCam.SetViewingTarget(pEnt);
		if (MoveImm)
		{
			m_CurrCam.MoveToTargetImm();
			GetRenderClass()->AddAllDirtyRect();
		}
	}
	else
	{
		if (m_CurrCam.GetViewingTarget() == gGlobal.g_pCharacter->GetCharMainView())
		{
			if (TestPlayerLst.size() > 0)
			{
				m_CurrCam.SetViewingTarget(TestPlayerLst[TestPlayerLst.size() - 1]);
				if (MoveImm)
				{
					m_CurrCam.MoveToTargetImm();
					GetRenderClass()->AddAllDirtyRect();
				}
			}
		}
		else
		{
			m_CurrCam.SetViewingTarget(gGlobal.g_pCharacter->GetCharMainView());
			if(MoveImm)
			{
				m_CurrCam.MoveToTargetImm();
				GetRenderClass()->AddAllDirtyRect();
			}
		}
	}
}
VOID QGLApplication::EnterBattle()
{
	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status before battle, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	gGlobal.g_NoMoreHelp = FALSE;

	gGlobal.LeftClickUpdate = FALSE;
	gGlobal.LeftClickStatus = LEFTCLICK_NORMAL;

	// stop talking when entering battle
	if ( gGlobal.g_pScriptControl )
		gGlobal.g_pScriptControl->StopTalkingBeforeBattle();

	// test code
	//gGlobal.g_pScriptControl->BattleDefaultDialogue();

	//stop the entity
		
	gGlobal.BlockingActive = FALSE;
	gGlobal.g_pBattleControl->Init();
	gGlobal.g_pBattleControl->CreateBgTexture(m_CurrMapView, GetRenderClass());
	gGlobal.g_pBattleControl->SetCommandAvial(TRUE);
	gGlobal.g_pBattleControl->SetLeaveBattle(FALSE);
	PixelPos LeftUpPos;
	LeftUpPos.PixelPosX = (Int)m_CurrCam.GetLeftUpPos().x;
	LeftUpPos.PixelPosY = (Int)m_CurrCam.GetLeftUpPos().y ;
	gGlobal.g_pBattleControl->ResetBattlePos(LeftUpPos, GetRenderClass()->GetWinWidth(), GetRenderClass()->GetWinHeight());
	SetBgTexture(gGlobal.g_pBattleControl->GetBattleMapData().BgTexture);
	SetRenderMapFlag(FALSE);
	State::ChangeState(m_mapState, SafeCreateObject(CBattleState));

	if (gGlobal.g_pCharacter)
		gGlobal.g_pCharacter->Stop();
	
	//change mouse state
	if (gGlobal.g_CursorState)
		gGlobal.g_CursorState->Unblock();
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));

	gGlobal.LeftClickUpdate = FALSE;
	gGlobal.LeftClickStatus = LEFTCLICK_NORMAL;
	gGlobal.g_BPShortCutFlag = false;
	//DrawScene();
	ChangeBattleUI();

}

VOID QGLApplication::ChangeMapUI()
{
	SafeDeleteObject(gGlobal.pUIManager);

	gGlobal.pUIManager = SafeCreateObject(UIManager);

	gGlobal.pUIManager->GetDirtyRectMan()->SetBoundRect(0, 0, 800, 600);

	gGlobal.pUIManager->SetRender(GetRenderClass());

	//gGlobal.pUIManager->InitIME();

	DWORD DebugTime = GetTickCount();

	
	XML_UI::GetInstance().Init();
	gGlobal.pUIManager->LoadPanelPos(_T("Data\\MyPanelPos.XML"));
	XML_UI::GetInstance().ReadFormFile("Data\\MsgPanel.xml", gGlobal.pUIManager);


	UIEditBox *EditLine = gGlobal.pUIManager->FindUIEditBox("EditLine");
	if(EditLine)
		EditLine->SetEditLimit(gConfigure.maxMsgLen);
	
	
	gGlobal.g_pMenuBuffIconProcess->ShowBuffIcon();

	if (gGlobal.g_pSystemSetting && gGlobal.g_pSystemSetting->IsSubChatEnable())
	{
		m_SubWindow.m_AlignType = gGlobal.g_pSystemSetting->GetSubChatPosition();
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenSubWindow", String());
	}

	if( gConfigure.uNetwork && gGlobal.g_pCharacter )
	{
		CCharacter * pchar = CastNetObject<CCharacter>(gGlobal.g_pCharacter->GetCharNetObject());
		UIStaticText* pST;
		UIAniIcon* pAniIcon;

		pST = gGlobal.pUIManager->FindUIStaticText("MainCharName");
		if( pST )
		{
			pST->SetText(pchar->GetNickName().c_str());
			pST->Redraw();
		}
		/*pIm = gGlobal.pUIManager->FindUIImage("MyFace");
		if( pIm )
		{
			StringA imageFilename;
			imageFilename.Format("Data/faceM%05ia.tga", pchar->GetSexClass() );
			pIm->SetImageFileA(imageFilename.c_str());
		}*/
		pAniIcon = gGlobal.pUIManager->FindUIAniIcon("MyFace");
		if (pAniIcon)
		{
			String imageFilename;
			imageFilename.Format(_T("faceM%05ia.ent"), pchar->GetSexClass() );
			pAniIcon->ClearAniData();
			pAniIcon->SetAnimationData(imageFilename.c_str());
		}		

		gGlobal.SetDisplayHP(pchar->GetHP(), pchar->GetMHP(), pchar->GetMaxHP());
		gGlobal.SetDisplaySP(pchar->GetSP(), pchar->GetMSP(), pchar->GetMaxSP());
		gGlobal.SetDisplayDP(pchar->GetDP(), pchar->GetMaxDP());

		if (gGlobal.g_pParty)
		{ // Party UI
			gGlobal.g_pParty->RedrawUI();
		}
		else if (gGlobal.g_pTempLeaveParty) 
		{
			gGlobal.g_pTempLeaveParty->RedrawUI();
		}
		pST = gGlobal.pUIManager->FindUIStaticText("PlaceName");
		if( pST && 
			gGlobal.m_mapdb && 
			gGlobal.m_mapdb->m_basicinfo && 
			gGlobal.m_mapdb->m_basicinfo->m_minimapfile )
		{
			pST->SetText(String(gGlobal.m_mapdb->m_basicinfo->m_mapname).c_str());
			pST->Redraw();
		}

		for(Int i = 0; i < 10; ++i)
		{
			gGlobal.g_ShortCutSlot[i].m_RefNum = pchar->GetShortcut(i);
			gGlobal.g_ShortCutSlot[i].m_RefType = pchar->GetShortcuttype(i);
		}
		

		gGlobal.pUIManager->RenderAll();
	}

	gGlobal.PositionText = gGlobal.pUIManager->FindUIStaticText("Position");

	if (gGlobal.g_pMenuFriendProcess->m_nextMessageQue.size() > 0)
	{
		UIButton * pBu = gGlobal.pUIManager->FindUIButton("DownBFriendShip");
		if (pBu)
		{
			pBu->SetState(_ButtonNotice);
			pBu->SetLockState(true);
		}
	}
	/*
	if( gConfigure.uNetwork &&
		gGlobal.g_pCharacter &&
		gGlobal.m_mapdb && 
		gGlobal.m_mapdb->m_basicinfo && 
		gGlobal.m_mapdb->m_basicinfo->m_soundfile )
	{
		StringA soundfilename(gGlobal.m_mapdb->m_basicinfo->m_soundfile);
		if( soundfilename.GetLength()>0 )
		{
			soundfilename.Format("Data\\%s", StringA(gGlobal.m_mapdb->m_basicinfo->m_soundfile).c_str());
			::GetOggPlayer()->ChangeOgg(soundfilename.c_str());
		}
		else
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("ChangeMapUI: Music file %s not found in map-id=%i.  File \"Field\" is used instead."),
				gGlobal.m_mapdb->m_basicinfo->m_soundfile, gGlobal.m_mapdb->m_basicinfo->m_mapid );
			::GetOggPlayer()->ChangeOgg("Data\\music0009_Field01.ogg");
		}
	}else
		::GetOggPlayer()->ChangeOgg("Data\\music0009_Field01.ogg");
	*/

	//free login ent data
	m_LoginEnt.FreeEntityData();	
}

VOID QGLApplication::ChangeBattleUI()
{
	static bool VItemFlag = true;

	gGlobal.pUIManager->CloseTips();

//	if(strcmp(gGlobal.pUIManager->GetIDName(), "Map") == 0)
//	{
	if(m_mapState->GetStateID() == ST_APP_BATTLE)
	{
		if((gGlobal.g_pParty && !gGlobal.g_pParty->IsLeader()) ||
			gGlobal.g_pCharacter->GetAction() == CCharacter::ST_MONITOR)
			gGlobal.g_helpEnable = FALSE;
		else if (gGlobal.g_pTempLeaveParty) 
		{
			gGlobal.g_helpEnable = FALSE;
		}
		else
		{
			gGlobal.g_helpEnable = TRUE;
		}

		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "EnterBattle", String());
		//check whether it is leader or alone

		if (gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MONITOR)
		{
			if( gGlobal.g_pParty )
			{
				gGlobal.g_pParty->RedrawUI();
				if (gGlobal.g_pParty->IsLeader())
				{
					UIButton *pItem = gGlobal.pUIManager->FindUIButton("DownBParty");
					if (pItem)
						pItem->SetNormalState();
				}
				else
				{
					UIButton *pItem = gGlobal.pUIManager->FindUIButton("BattleHelpB");
					if (pItem)
						pItem->SetDisableState();
				}
			}
			else
			{
				UIButton *pItem = gGlobal.pUIManager->FindUIButton("DownBParty");
				if (pItem)
					pItem->SetNormalState();	
			}
		}
	
	}
	else
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "LeaveBattle", String());

		if( gConfigure.uNetwork && gGlobal.g_pCharacter )
		{
			CCharacter * pchar = CastNetObject<CCharacter>(gGlobal.g_pCharacter->GetCharNetObject());
			UIStaticText* pST;
			UIAniIcon* pAniIcon;

			pST = gGlobal.pUIManager->FindUIStaticText("MainCharName");
			if( pST )
			{
				pST->SetText(pchar->GetNickName().c_str());
				pST->Redraw();
			}
			
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon("MyFace");
			if (pAniIcon)
			{
				String imageFilename;
				imageFilename.Format(_T("faceM%05ia.ent"), pchar->GetSexClass() );
				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(imageFilename.c_str());
			}		

			if (gGlobal.g_pPartnerProcess && gGlobal.g_pPartnerGroup)
			{
				CPartner* partner = gGlobal.g_pPartnerGroup->GetCurrBattlePartner();
				if (partner)
					gGlobal.g_pPartnerProcess->UpdateSmallIcon(true, partner->GetPartner_id());
				else
					gGlobal.g_pPartnerProcess->UpdateSmallIcon(false);
			}

			if (gGlobal.g_pCharProcess)
			{
				gGlobal.g_pCharProcess->UpdateHPBar();
				gGlobal.g_pCharProcess->UpdateSPBar();
				gGlobal.g_pCharProcess->UpdateDPBar();
			}
			

			if (gGlobal.g_pParty)
			{ // Party UI
				gGlobal.g_pParty->RedrawUI();
				/*
				CParty* pParty = gGlobal.g_pParty;
				if( pParty )
				{
					UInt size = pParty->GetCount();
					NetObject * netobj;
					CCharacter * pchar;
					UInt32 memberUIPos = 0;

					for( Index i = C_INDEX(0); i < size; ++i )
					{
						netobj = pParty->GetChild(i);
						if( netobj->GetClassID()==Character_CLASSID )
						{
							pchar = CastNetObject<CCharacter>(netobj);
							if( pchar->GetChar_id()==pParty->GetOwnerId() )
							{
								CCharacterControl * pCharCtrl = QGLApplication::GetInstance().FindCharCtrlByNetObject(pchar);
								if (pCharCtrl)
									pCharCtrl->GetCharMainView()->SetExtraZBuffer(1);
								// Set Leader
								if( pchar->GetChar_id()==gGlobal.g_pCharacter->GetMapActorID() )
									pParty->SetUILeader(true, true, 0);
								else
									pParty->SetUILeader(true, false, memberUIPos+1);
							}
							else
							{
								CCharacterControl * pCharCtrl = QGLApplication::GetInstance().FindCharCtrlByNetObject(pchar);
								if (pCharCtrl)
									pCharCtrl->GetCharMainView()->SetExtraZBuffer(0);
								// Set Non-leader
								if( pchar->GetChar_id()==gGlobal.g_pCharacter->GetMapActorID() )
									pParty->SetUILeader(false, true, 0);
								else
									pParty->SetUILeader(false, false, memberUIPos+1);
							}
							if( pchar->GetChar_id()!=gGlobal.g_pCharacter->GetMapActorID() )
							{
								StringA imageFilename;
								imageFilename.Format("Data/faceS%05ia.tga", pchar->GetSexClass() );

								pParty->SetUIMemberHPBar(memberUIPos+1, pchar->GetHP(), pchar->GetMHP(), pchar->GetMaxHP());
								pParty->SetUIMemberSPBar(memberUIPos+1, pchar->GetSP(), pchar->GetMSP(), pchar->GetMaxSP());
								pParty->SetUIMemberImage(memberUIPos+1, imageFilename);
								pParty->SetUIMemberName(memberUIPos+1, pchar->GetNickName());
								pParty->SetUIMemberPanel(true, memberUIPos+1);

								memberUIPos++;
							}
						}
						else
							TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("Non-member object found: object %i"), i);
					}
					for( UInt i = memberUIPos; i<MAX_PARTY_NUM-1; i++ )
					{
						// hide leader icon
						pParty->SetUILeader(false, false, i+1);
						// hide non-existing party member
						pParty->SetUIMemberPanel(false, i+1);
					}
				}
				*/
			}
			else if (gGlobal.g_pTempLeaveParty) 
			{
				gGlobal.g_pTempLeaveParty->RedrawUI();
			}
			
			
			if( gGlobal.m_mapdb->m_basicinfo->m_soundfile )
			{
				StringA soundfilename(gGlobal.m_mapdb->m_basicinfo->m_soundfile);
				if( soundfilename.GetLength()>0 )
				{
					soundfilename.Format("Data\\%s", StringA(gGlobal.m_mapdb->m_basicinfo->m_soundfile).c_str());
					soundfilename.ToLower();					
					::GetFSoundManager()->SetMusic(soundfilename.c_str());
					if (gGlobal.g_pSystemSetting->IsMusicModeEnable())
						::GetFSoundManager()->MusicSetVolume(gGlobal.g_pSystemSetting->GetMusicVolForFmod() );
					else
						::GetFSoundManager()->MusicSetVolume(0);					
				}else
					::GetFSoundManager()->StopMusic();
			}else
				::GetFSoundManager()->StopMusic();

		}
		
	}

	gGlobal.pUIManager->RenderAll();
}


VOID QGLApplication::LeaveBattle()
{
	gGlobal.LeftClickUpdate = TRUE;
	gGlobal.LeftClickStatus = LEFTCLICK_NORMAL;
	gGlobal.g_NoMoreHelp = FALSE;
	if (gConfigure.uNetwork)
	{
		State::ChangeState(gGlobal.g_pCharacter->m_mapState, SafeCreateObject(CMapState));
// 		gGlobal.g_pCharacter->SetEntityCurrPos(gGlobal.g_pCharacter->GetCharNetObject()->GetCurPosX(), 
// 			gGlobal.g_pCharacter->GetCharNetObject()->GetCurPosY());
		gGlobal.g_pCharacter->GetCharMainView()->SetAnimationIndexByIndex(ANIM_IDLE1);
		gGlobal.g_pBattleControl->FreeBattleEntData();
		//check pet and say some emotion icon
		if (gGlobal.g_pCharacter->GetCharNetObject() && gGlobal.g_pCharacter->GetCharNetObject()->GetPet_uid() > 0)
		{
			CChannelControl * pChannelCtrl = gGlobal.GetChannelControl(CHANNELTYPE_MAP);
			if (pChannelCtrl)
			{
				static Int EmotionCount = 1;
				//pChannelCtrl->SendEmotion(EmotionCount, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID());

				EmotionCount ++;
				if (EmotionCount > MAX_EMOTION)
					EmotionCount = 1;
			}
		}
		gGlobal.g_pCharacter->RemoveBuffAfterBattle();
	}
	gGlobal.BlockingActive = TRUE;
	
	gGlobal.g_pBattleControl->SetBattleState(BATTLE_STATE_FADE_OUT);
	//SetBgTexture(gGlobal.g_pBattleControl->GetBattleMapData().BgMaskTexture);
	//SetBgTexture(NULL);
	//CreateMaskTexture(gGlobal.g_pBattleControl->GetBattleMapData().BgMaskTexture, MASKTEXTURETYPE_FADE_OUT, 1000, &GetRenderClass()->GetWinRect());
	GetRenderClass()->AddAllDirtyRect();
	gGlobal.g_DrawAll = TRUE;

	State::ChangeState(m_mapState, SafeCreateObject(CMapState));
	SetRenderMapFlag(TRUE);
	ChangeBattleUI();

	
	if (gGlobal.g_pMiddleMap->GetCharDesX() >= 0 && gGlobal.g_pMiddleMap->GetCharDesY() >= 0)
	{
		if ( (gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX != gGlobal.g_pMiddleMap->GetCharDesX()) || 
			(gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY != gGlobal.g_pMiddleMap->GetCharDesY()) )
		{
			//gGlobal.g_pMiddleMap->DrawPath();
			gGlobal.g_pCharacter->SetEntityDesPos(gGlobal.g_pMiddleMap->GetCharDesX(), gGlobal.g_pMiddleMap->GetCharDesY(), true);
		}

	}

	gGlobal.LeftClickUpdate = TRUE;
	gGlobal.LeftClickStatus = LEFTCLICK_NORMAL;

	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status after left battle, %s"), MemoryManager::GetInstance().GetStatus().c_str());
}

BOOL QGLApplication::CheckEntityFollower(CEntityBase * pEntBase, UInt32 CurrTime, UInt32  ParentPreMoveTime)
{
	if (pEntBase == NULL)
		return FALSE;
	if (pEntBase->GetFollowTarget() != NULL)
	{
		BlockPos CurrPos = pEntBase->GetCharPos();
		
		Int FollowerIndex = 1 + pEntBase->GetFollowTarget()->GetFollowerLstIndex(pEntBase);
		if (FollowerIndex == 0)
			return FALSE;
		//Int FollowIndex = GetPrivateProfileInt(_T("CHAR_SETTING"), _T("FOLLOWERINDEX"), 2, CONFIGURATION_INI_FILE) * 
		//	FollowerIndex;

		//UInt32 FollowDiff = GetPrivateProfileInt(_T("CHAR_SETTING"), _T("FOLLOWERPIXELDIFF"), 40, CONFIGURATION_INI_FILE) * FollowerIndex;
		BlockPos TargetPos = pEntBase->GetFollowTarget()->GetCharPos();
		//pEntBase->SetCharPos(TargetPos, FALSE);

		PixelPos ParentPixelPos;
		QGLMath::BlockPosToPixel(pEntBase->GetFollowTarget()->GetCharPos().PosX, pEntBase->GetFollowTarget()->GetCharPos().PosY,
			ParentPixelPos.PixelPosX, ParentPixelPos.PixelPosY);
		PixelDirection  dir;

		UInt32 ParentDiff = QGLMath::CalculatePixelPosDistance(ParentPixelPos, pEntBase->GetFollowTarget()->GetEntPixelPos(), dir);

		UInt32 TotatDiff = ParentDiff;
		
		Int i = 0;
		for (i = 0; i < (Int)pEntBase->GetFollowTarget()->m_PathFindData.PreSteps.size(); i++)
		{
			BlockPos TargetPos = pEntBase->GetFollowTarget()->GetPreStep(i);
			BlockPos NextPos = pEntBase->GetFollowTarget()->GetCharPos();
			
			if (i > 0 )
				NextPos = pEntBase->GetFollowTarget()->GetPreStep(i - 1);
			//BlockPos TargetCurrPos = pEntBase->GetFollowTarget()->GetCharPos();

			PixelPos DesPixelPos;
			QGLMath::BlockPosToPixel(NextPos.PosX, NextPos.PosY, DesPixelPos.PixelPosX, DesPixelPos.PixelPosY);

			PixelPos CurrPixelPos2;
			QGLMath::BlockPosToPixel(TargetPos.PosX, TargetPos.PosY, CurrPixelPos2.PixelPosX, CurrPixelPos2.PixelPosY);

			UInt32 TempDiff = QGLMath::CalculatePixelPosDistance(CurrPixelPos2, DesPixelPos, dir);
			TotatDiff += TempDiff;

			if (TotatDiff > gConfigure.uFollowerDiff * FollowerIndex)
			{
				
				pEntBase->SetCharPos(TargetPos, TRUE);

				UInt32 subdiff = TotatDiff - gConfigure.uFollowerDiff * FollowerIndex;
				Real32 MoveFactor = (Real32)subdiff / (Real32)TempDiff;

				BOOL Found = FALSE;
				for (Byte TempDir = SOUTH; TempDir < MAXROTATION; TempDir++)
				{
					Int TempMinX = TargetPos.PosX;
					Int TempMinY = TargetPos.PosY;
					QGLMath::MoveToBlock(TempMinX, TempMinY, TempDir, 1);
					if (TempMinX == NextPos.PosX && 
						TempMinY == NextPos.PosY)
					{
						pEntBase->SetRotation(TempDir);
						Found = TRUE;
						break;
					}
				}

				Int FinalPixelX = CurrPixelPos2.PixelPosX + (Int)((DesPixelPos.PixelPosX - CurrPixelPos2.PixelPosX) * MoveFactor);
				Int FinalPixelY = CurrPixelPos2.PixelPosY + (Int)((DesPixelPos.PixelPosY - CurrPixelPos2.PixelPosY) * MoveFactor);
				
				if ((pEntBase->GetFollowTarget()->GetAnimationIndex() == ANIM_MOUNT_RUN ||
					pEntBase->GetFollowTarget()->GetAnimationIndex() == ANIM_RUN) && 
					(FinalPixelX != pEntBase->GetEntPixelPos().PixelPosX || FinalPixelY != pEntBase->GetEntPixelPos().PixelPosY))
					pEntBase->SetAnimationIndexByIndex(ANIM_RUN);
				
				pEntBase->SetEntPixelPos(FinalPixelX, FinalPixelY);

				pEntBase->m_PathFindData.PreSteps = pEntBase->GetFollowTarget()->m_PathFindData.PreSteps;
				return TRUE;
			}
		}
	}
	return FALSE;
}
VOID QGLApplication::UpdateEntityMove(CEntityBase * pEntBase, UInt32 CurrTime)
{
	if (pEntBase == NULL)
		return ;

	//check follower
	BlockPos CurrPos = pEntBase->GetCharPos();
	BlockPos DesPos = pEntBase->GetCharDesPos();

	if (CurrPos == DesPos)
	{
		if (pEntBase->IsMount())
		{
			if (m_mapState->GetStateID() != ST_APP_BATTLE && pEntBase->GetAnimationIndex() == ANIM_MOUNT_RUN)
				pEntBase->SetAnimationIndexByIndex(ANIM_MOUNT_IDLE1);
		}
		else
		{
			if (m_mapState->GetStateID() != ST_APP_BATTLE && pEntBase->GetAnimationIndex() == ANIM_RUN)
				pEntBase->SetAnimationIndexByIndex(ANIM_IDLE1);
		}
		pEntBase->m_PreMoveTime = CurrTime;

		if (pEntBase->GetFollowerLst().size() > 0)
		{
			StlVector<CEntityBase *> FollowerLst = pEntBase->GetFollowerLst();
			for (Int i = 0; i < (Int)FollowerLst.size(); ++i)
			{
				CheckEntityFollower(FollowerLst[i], CurrTime, CurrTime);

				if ((FollowerLst[i]->GetAnimationIndex() == ANIM_MOUNT_RUN ||
					FollowerLst[i]->GetAnimationIndex() == ANIM_RUN))
					FollowerLst[i]->SetAnimationIndexByIndex(ANIM_IDLE1);
			}

		}

		return;
	}

	PixelPos CurrPixelPos = pEntBase->GetEntPixelPos();
	Int Rotation = 0; 
	Rotation = pEntBase->GetAnimationRotation();
	PathFindData TempPFData = pEntBase->m_PathFindData;

	UInt16 MaxBlockX = 0xFFFF;
	UInt16 MaxBlockY = 0xFFFF;
	if (m_CurrMapView->GetMapData())
	{
		MaxBlockX = m_CurrMapView->GetMapData()->MaxBlockX;
		MaxBlockY = m_CurrMapView->GetMapData()->MaxBlockY;
	}
	Real32 Movefactor = 0;
	UInt32 ParentPreMoveTime = pEntBase->m_PreMoveTime;
	CPathMove::UpDateMoveData(CurrTime, pEntBase->GetEntityMoveSpeed(), pEntBase->m_PreMoveTime,
		CurrPixelPos,CurrPos,DesPos,MaxBlockX, MaxBlockY, Rotation, TempPFData, gGlobal.g_PathFinder, Movefactor);
		
	pEntBase->m_PathFindData = TempPFData;
	//pEntBase->m_PathFindData.m_ptList
	//pEntBase->SetCharDesPos(DesPos.PosX, DesPos.PosY);
	pEntBase->m_DesPos = DesPos;

	if (CurrPos != pEntBase->GetCharPos())
	{
		pEntBase->SetCharPos(CurrPos, FALSE);
	}

	if (pEntBase->GetCharDesPos() == pEntBase->GetCharPos())
	{
		if (pEntBase->IsMount())
			pEntBase->SetAnimationIndexByIndex(ANIM_MOUNT_IDLE1);
		else
			pEntBase->SetAnimationIndexByIndex(ANIM_IDLE1);
	}

	if (CurrPixelPos != pEntBase->GetEntPixelPos())
	{
		pEntBase->SetEntPixelPos(CurrPixelPos);
		if (pEntBase->IsMount())
			pEntBase->SetAnimationIndexByIndex(ANIM_MOUNT_RUN);
		else
			pEntBase->SetAnimationIndexByIndex(ANIM_RUN);
		pEntBase->SetRotation(Rotation);
		
	}
	//check whether there is path remaining
	AddFootStep(pEntBase);

	if (pEntBase->GetFollowerLst().size() > 0)
	{
		StlVector<CEntityBase *> FollowerLst = pEntBase->GetFollowerLst();
		for (Int i = 0; i < (Int)FollowerLst.size(); ++i)
		{
			CheckEntityFollower(FollowerLst[i], CurrTime, ParentPreMoveTime);
			
		}
	}
}

VOID QGLApplication::RandomAccessBattle(CEntityBase * pEntBase)
{
	if (pEntBase == NULL)
		return;

	if ((pEntBase)->GetCharPos() != (pEntBase)->GetCharDesPos() && (pEntBase) == gGlobal.g_pCharacter->GetCharMainView())
		{
			if (gGlobal.RandomAccessBattle)
			{
				Int RandomAccesRate = GetPrivateProfileInt(_T("BATTLESETTING"), _T("RANDOMACCESSRATE"), 500, CONFIGURATION_INI_FILE);;
				if (rand() % RandomAccesRate == 0)
				{
					if (m_mapState->GetStateID() == ST_APP_MAP)
					{
						EnterBattle();
					}
				}
			}
		}

}
VOID QGLApplication::AddFootStep(CEntityBase * pEntBase)
{
	if (pEntBase == NULL)
		return;

	if (gGlobal.DisposalEffect)
	{
		static UInt32 DisposalUpdateTime = ::timeGetTime();
		static UInt32 EndTime = 2000;
		static UInt32 LifeTime = 500;
		UInt32 CurrTime = ::timeGetTime();

		if (CurrTime - DisposalUpdateTime > EndTime)
		{
			DisposalEffectData Data;
			Data.PreUpdateTime = CurrTime;
			Data.EndTime = CurrTime + EndTime;
			Data.LifeTime = LifeTime;
			Data.DisposalTime = LifeTime;
			Data.StartAlpha = 0;
			Data.TimeToProduce = EndTime / 10;
			Data.pEnt = gGlobal.g_pCharacter->GetCharMainView();

			InsertDisposalEffect(Data);

			DisposalUpdateTime = CurrTime;
		}
	}

	if ((pEntBase)->m_FootStepCount >= 100)
	{
		(pEntBase)->m_FootStepCount = 0;

		if (gGlobal.AddFootStep)
		{
			gGlobal.LeftFoot = !gGlobal.LeftFoot;
			PixelDirection Dir; 
			ParticleStructSetting PSS;
			PSS.Destory = TRUE;
			PSS.ExtraZValue = 0;
			PSS.GivenDisposalAnim = 0;
			PSS.GivenDisposalTime = 1000;
			PSS.HaveEffect = TRUE;
			PSS.LiveTimeBase = 1000;
			PSS.LiveTimeVar = 0;
			PSS.Loop = FALSE;
			PSS.MoveSpeed = 0;
			PSS.NoOfParticle = 1;
			PSS.Stop = FALSE;
			CAnimatedEntity * pEnt = dynamic_cast<CAnimatedEntity *>(pEntBase);
			if (pEnt)
				PSS.Rotation = pEnt->GetAnimationRotation();

			if (gGlobal.LeftFoot)
			{
				Char FileName[50];

				GetPrivateProfileString(_T("LOADFILE"), _T("FOOTPRINTL"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
				CAnimatedEntity TempEnt;
				TempEnt.LoadEntityFile(FileName, gGlobal.pD3DDevice);
				gGlobal.g_DisposalEntityEffect->AddAnimatedEntityToSystem(&TempEnt,PSS,(pEntBase));
			}
			else
			{
				Char FileName[50];
				GetPrivateProfileString(_T("LOADFILE"), _T("FOOTPRINTR"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
				CAnimatedEntity TempEnt;
				TempEnt.LoadEntityFile(FileName, gGlobal.pD3DDevice);
				gGlobal.g_DisposalEntityEffect->AddAnimatedEntityToSystem(&TempEnt,PSS,(pEntBase));
			}
		}
	}
}

VOID QGLApplication::UpdateShortCutKey()
{
	static BYTE s_Function[13];

	HWND hWnd; 
	hWnd = ::GetForegroundWindow();
	bool CheckWnd = (hWnd == m_Window.GetWindow());

	BYTE KeyState[256];
	
	GetKeyboardState(KeyState);
	for(Int i = 0; i < 10; ++i)
	{
		if(KeyState[VK_F1 + i] & 0xF0)
		{
			if(!s_Function[i])
			{
				if(CheckWnd && !(KeyState[VK_CONTROL] & 0xF0) && !(KeyState[VK_MENU] & 0xF0))
					PostMessage(m_Window.GetWindow(), WM_SHORTCUTKEY, (WPARAM)(VK_F1 + i), 0);
				s_Function[i] = 1;
			}
		}
		else
		{
			s_Function[i] = 0;
		}
	}

	if(KeyState[VK_F12] & 0xF0)
	{
		if(!s_Function[12])
		{
			if(CheckWnd && !(KeyState[VK_CONTROL] & 0xF0) && !(KeyState[VK_MENU] & 0xF0))
				PostMessage(m_Window.GetWindow(), WM_SHORTCUTKEY, 1001, 0);
			s_Function[12] = 1;
		}
	}
	else
		s_Function[12] = 0;
	
}

VOID QGLApplication::UpdateGameLoop(CRender * pRender)
{
	if (pRender == NULL)
		return;

	UpdateShortCutKey();
	
	UInt32 CurrentTime = ::timeGetTime();
	if( gGlobal.m_AutoJumppointWalk && m_mapState->GetStateID() != ST_APP_BATTLE )
	{
		UInt32 nowtime = ::timeGetTime();
		if( nowtime - m_prevwalktime > 500 ) 
		{
			if (gConfigure.uNetwork)
			{
				if( gGlobal.g_pCharacter->GetCharNetObject()->GetAction()!=CMapActor::ST_BATTLE )
				{
					if( gGlobal.GetAutoX()!=0 || gGlobal.GetAutoX()!=0 )
					{
						BlockPos pos = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
						if( pos.PosX==gGlobal.GetAutoX() && pos.PosY==gGlobal.GetAutoY() )
							gGlobal.g_pCharacter->SetEntityDesPos(gGlobal.GetAutoX()+1, gGlobal.GetAutoY()+1);
						else
							gGlobal.g_pCharacter->SetEntityDesPos(gGlobal.GetAutoX(), gGlobal.GetAutoY());
					}
				}
			}
			else
			{
				if (gGlobal.g_PathFinder)
					gGlobal.g_PathFinder->SetBlockingActive(FALSE);
				const int DesX = 79;
				const int DesY = 269;
				
				if (!((gGlobal.g_pCharacter->GetCharMainView()->GetCharDesPos().PosX == gConfigure.uPosX &&
					gGlobal.g_pCharacter->GetCharMainView()->GetCharDesPos().PosY == gConfigure.uPosY) ||
					(gGlobal.g_pCharacter->GetCharMainView()->GetCharDesPos().PosX == DesX &&
					gGlobal.g_pCharacter->GetCharMainView()->GetCharDesPos().PosY == DesY)))
				{
					gGlobal.g_pCharacter->GetCharMainView()->SetCharDesPos(gConfigure.uPosX, gConfigure.uPosY);
				}

				if (gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX == gConfigure.uPosX && 
					gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY == gConfigure.uPosY)
				{
					gGlobal.g_pCharacter->GetCharMainView()->SetCharDesPos(DesX,
						DesY);
				}
				else if (gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX == DesX &&
					gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY == DesY)
				{
					gGlobal.g_pCharacter->GetCharMainView()->SetCharDesPos(gConfigure.uPosX, gConfigure.uPosY);
				}

				//change map
				static int ChangeMapTime = ::timeGetTime();
				if (nowtime - ChangeMapTime > 1000)
				{
					QGLApplication::GetInstance().UpdateSceneData();
					QGLApplication::GetInstance().GetRenderClass()->AddAllDirtyRect();
					QGLApplication::GetInstance().DrawScene();
					QGLApplication::GetInstance().DeleteAllCharacters();

					gGlobal.ChangeMapFlag = !gGlobal.ChangeMapFlag;

					ReadChangeMapInfo();

					if (gGlobal.ChangeMapFlag)//m_CurrMapView->GetFileName() ==  gConfigure.strMapFile)
					{
						ChangeMap(gConfigure.strMapFile2.c_str(), gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX,
							gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY);
					}
					else
					{
						ChangeMap(gConfigure.strMapFile.c_str(), gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX,
							gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY);
					}
					ChangeMapTime = nowtime;

					gGlobal.MisAdjust();
				}
			}

			m_prevwalktime = nowtime;
		}
	}

	// disappear npc dialogue when over range
	//OnDisappearNPCDialogue();
	// count down for waiting party choose result
	
	// disappear shop menu
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->OnDisappear();

	if ( gGlobal.g_pQuestLogProcess )
		gGlobal.g_pQuestLogProcess->NeedToUpdateQuestRemainTime();

	if (gGlobal.g_pScriptControl && gGlobal.g_pScriptControl->NeedUpdate())
		gGlobal.g_pScriptControl->UpdateDialogue(CurrentTime);

	if (gGlobal.g_pHawkerProcess && gGlobal.g_pTargetHawkGroup)
		gGlobal.g_pHawkerProcess->DisappearHawkPanel();

	if (gGlobal.g_pGiftBoxProcess && gGlobal.g_pGiftBox)
		gGlobal.g_pGiftBoxProcess->DisappearGiftBox();

	if (gGlobal.g_pWareHouseProcess)
		gGlobal.g_pWareHouseProcess->DisappearWareHouse();

	if (gGlobal.g_pMenuMixProcess)
		gGlobal.g_pMenuMixProcess->DisappearMixShop();
	
	if (gGlobal.g_pAutoWalkProcess)
		gGlobal.g_pAutoWalkProcess->OnUpdate(CurrentTime);

	if (gGlobal.g_pMenuSellYBProcess)
		gGlobal.g_pMenuSellYBProcess->DisppearSellYB();

	if (gGlobal.g_pPartnerCultivationUpProcess)
		gGlobal.g_pPartnerCultivationUpProcess->DisappearPartnerCultivationUp();

	static bool dycShowSpark = true;
	if(dycShowSpark)
	{
		UIAniIcon* MyFaceSparkUi = gGlobal.pUIManager->FindUIAniIcon("MyFaceSpark");
		if(gGlobal.g_pCharacter && MyFaceSparkUi)
		{
			if(gGlobal.g_pCharacter->GetLevel() <= 10 && gGlobal.g_pCharacter->GetExp() >= gGlobal.g_pCharacter->GetNextLvExp() )
			{
				if (MyFaceSparkUi)
				{
					if(!MyFaceSparkUi->GetAniEnt()->HasAniFile())
						MyFaceSparkUi->SetAnimationData(String(_T("HeadIconS.ent")).c_str());
					if (!MyFaceSparkUi->IsEnable())
						MyFaceSparkUi->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(MyFaceSparkUi);
				}
			}
			dycShowSpark = false;
		}
	}
	//select the entity
	SelectEntity(pRender->GetCursorPosX(), pRender->GetCursorPosY());

	//sync by server later
	if (gGlobal.g_pBattleControl)
	{
		gGlobal.g_pBattleControl->UpdateBattle(CurrentTime);
		if (m_mapState->GetStateID() == ST_APP_BATTLE && gGlobal.g_pBattleControl->CheckBattleFinish())
		{
			LeaveBattle();
		}
	}

	UInt32CharCtrlPtrMap::iterator itr = m_MapCharacterLst.begin();
	//update the path
	while (itr != m_MapCharacterLst.end())
	{
		CEntityBase * pEntBase = itr->second->GetCharMainView();
		if (!pEntBase->GetFollowTarget())
			UpdateEntityMove(pEntBase, CurrentTime);
		//UpdateEntityMoveOffset(pEntBase, CurrentTime);		
		//pEntBase->UpdateEntityMoveOffset(CurrentTime);
		itr++;
	}

	//battle character data
	if (gGlobal.g_pBattleControl && m_mapState->GetStateID() == ST_APP_BATTLE)
	{
		for (Int i = 0; i < (Int)gGlobal.g_pBattleControl->m_BattlePlayerLst.size(); i++)
		{
			CEntityBase * pEntBase = gGlobal.g_pBattleControl->m_BattlePlayerLst[i].pChar->GetCharMainView();
			if (!pEntBase->GetFollowTarget())
			UpdateEntityMove(pEntBase, CurrentTime);
			//pEntBase->UpdateEntityMoveOffset(CurrentTime);
		}
	}

	//test player list
	for (Int i = 0; i < (Int)TestPlayerLst.size(); i++)
	{
		if (!TestPlayerLst[i]->GetFollowTarget())
		UpdateEntityMove(TestPlayerLst[i], CurrentTime);
		//TestPlayerLst[i]->UpdateEntityMoveOffset(CurrentTime);
	}

	//update UI cursor
	gGlobal.CursorEntity->UpDate(CurrentTime);

	if (gGlobal.CursorEntity->m_Updated)
	{
		Int32	CX = gGlobal.pUIManager->GetCursorX() + gGlobal.pUIManager->GetCursorOffsetX();
		Int32	CY = gGlobal.pUIManager->GetCursorY() + gGlobal.pUIManager->GetCursorOffsetY();
		RECT rect;
		SetRect(&rect, CX, CY, 
			CX + gGlobal.CursorEntity->m_EntityTotalRect.right - gGlobal.CursorEntity->m_EntityTotalRect.left,
			CY + gGlobal.CursorEntity->m_EntityTotalRect.bottom - gGlobal.CursorEntity->m_EntityTotalRect.top);

		gGlobal.pUIManager->GetDirtyRectMan()->AddDirtyRectToMan(rect);
	}

	//update Question
	if(CQuestionProcess::GetInstance().GetQuestion() != NULL)
	{
		CQuestionProcess::GetInstance().Update(CurrentTime);
	}
}

VOID QGLApplication::UpdateMaskTexture(UInt32 CurrTime)
{
	if (m_MaskTextureData.MaskTexture == NULL)
		return;

	if (m_MaskTextureData.FadeInOutTime == 0)
		return;

	if (m_MaskTextureData.Type == MASKTEXTURETYPE_NO_EFFECT)
		return;

	if (m_MaskTextureData.StartTime == 0)
			m_MaskTextureData.StartTime = CurrTime;

	if (CurrTime < m_MaskTextureData.FadeInOutTime + m_MaskTextureData.StartTime)
	{
		Byte Alpha = (Byte)(((Real32)(CurrTime - m_MaskTextureData.StartTime) / (Real32)m_MaskTextureData.FadeInOutTime) * 255);

		if (m_MaskTextureData.Type == MASKTEXTURETYPE_FADE_OUT)
		{
			Alpha = 255 - Alpha;
		}

		if (m_MaskTextureData.Alpha != Alpha)
		{
			m_MaskTextureData.Alpha = Alpha;
			gGlobal.g_DrawAll = TRUE;
		}

	}
	else
	{
		if (m_MaskTextureData.Type == MASKTEXTURETYPE_FADE_OUT)
		{
			//remove the texture
			gGlobal.g_DrawAll = TRUE;
			m_MaskTextureData.MaskTexture->Release();
			m_MaskTextureData.StartTime = 0;
			m_MaskTextureData.FadeInOutTime = 0;
			m_MaskTextureData.MaskTexture = NULL;
		}
		else if (m_MaskTextureData.Type == MASKTEXTURETYPE_FADE_IN)
		{
			if (m_MaskTextureData.Alpha != 255)
			{
				m_MaskTextureData.Alpha = 255;
				gGlobal.g_DrawAll = TRUE;
			}
		}
	}
}	
VOID QGLApplication::ResetMapData()
{
	CResourcesManager::GetInstance().ResetMapResourcesRequest(m_CurrMapView->GetMapData(), m_CurrMapView->GetEffectiveRect(), 
				gGlobal.pD3DDevice);
}
VOID QGLApplication::ResourcesUpdate()
{
	if (m_CurrMapView->GetMapData() && m_CurrMapView->GetMapData()->State == DATA_FINISH_LOADING &&
		m_CurrMapView->GetSubMapWidth() != 0
		&& m_CurrMapView->GetSubMapHeight() != 0)
	{
		Int MaxXPic = (Int)ceil((Real32)m_CurrMapView->GetMapWidth() / m_CurrMapView->GetSubMapWidth());
		Int MaxYPic = (Int)ceil((Real32)m_CurrMapView->GetMapHeight() / m_CurrMapView->GetSubMapHeight());

		RECT EnlargeEffectArea = m_CurrMapView->GetEffectiveRect();

		RECT CurrEnlargeEffectArea;// = pMapLoaderData->EffectArea;
		CurrEnlargeEffectArea.left = (LONG)m_CurrCam.GetLeftUpPos().x + GetRenderClass()->GetWinWidth() / 2 - 
			gConfigure.uSubPicSizeHalfX;
		CurrEnlargeEffectArea.right = (LONG)m_CurrCam.GetLeftUpPos().x + GetRenderClass()->GetWinWidth() / 2 + 
			gConfigure.uSubPicSizeHalfX;
		CurrEnlargeEffectArea.top = (LONG)m_CurrCam.GetLeftUpPos().y + GetRenderClass()->GetWinHeight() / 2 - 
			gConfigure.uSubPicSizeHalfY;
		CurrEnlargeEffectArea.bottom = (LONG)m_CurrCam.GetLeftUpPos().y + GetRenderClass()->GetWinHeight() / 2 + 
			gConfigure.uSubPicSizeHalfY;

		Int PreMinXIndex = (EnlargeEffectArea.left )/ m_CurrMapView->GetSubMapWidth();
		if (PreMinXIndex < 0)
			PreMinXIndex = 0;

		Int PreMinYIndex = (EnlargeEffectArea.top )/ m_CurrMapView->GetSubMapHeight() ;
		if (PreMinYIndex < 0)
			PreMinYIndex = 0;

		Int PreMaxXIndex = (EnlargeEffectArea.right )/ m_CurrMapView->GetSubMapWidth();
		if (PreMaxXIndex < 0)
			PreMaxXIndex = 0;

		Int PreMaxYIndex = (EnlargeEffectArea.bottom )/ m_CurrMapView->GetSubMapHeight();
		if (PreMaxYIndex < 0)
			PreMaxYIndex = 0;

		Int CurrMinXIndex = (CurrEnlargeEffectArea.left) / m_CurrMapView->GetSubMapWidth();
		if (CurrMinXIndex < 0)
			CurrMinXIndex = 0;

		Int CurrMinYIndex = (CurrEnlargeEffectArea.top) / m_CurrMapView->GetSubMapHeight();
		if (CurrMinYIndex < 0)
			CurrMinYIndex = 0;

		Int CurrMaxXIndex = (CurrEnlargeEffectArea.right) / m_CurrMapView->GetSubMapWidth();
		if (CurrMaxXIndex < 0)
			CurrMaxXIndex = 0;

		Int CurrMaxYIndex = (CurrEnlargeEffectArea.bottom) / m_CurrMapView->GetSubMapHeight();
		if (CurrMaxYIndex < 0)
			CurrMaxYIndex = 0;

		m_CurrMapView->SetEffectiveRect(CurrEnlargeEffectArea);

		if (CurrMinXIndex != PreMinXIndex || CurrMinYIndex != PreMinYIndex ||
			CurrMaxXIndex != PreMaxXIndex || CurrMaxYIndex != PreMaxYIndex)
		{
			CResourcesManager::GetInstance().ResetMapResourcesRequest(m_CurrMapView->GetMapData(), CurrEnlargeEffectArea, 
				gGlobal.pD3DDevice);		
		}

	}

	CResourcesManager::GetInstance().CheckResources();
}

VOID QGLApplication::ResetUpdatedStatus()
{

	for (StlMultiMap<UInt32, CEntityBase *>::iterator Itr = m_EntRenderLst.begin(); 
		Itr != m_EntRenderLst.end(); Itr ++)
	{
		Itr->second->m_Updated = FALSE;
	}
	
	m_CurrMapView->m_Updated = FALSE;
	m_CurrCam.ResetMoveOffset();
	gGlobal.CursorEntity->m_Updated = FALSE;
	gGlobal.NightMask->m_Updated = FALSE;
	gGlobal.g_NightMaskTextureData.m_DirRectMan.ClearDirtyRect();

	gGlobal.pUIManager->GetDirtyRectMan()->ClearDirtyRect();
	gGlobal.pUIManager->SetMouseMoveFlag(FALSE);
	gGlobal.g_DrawAll = FALSE;

	gGlobal.pUIManager->RemoveAllPanelsRects();

	if (m_SubWindow.m_UIManager)
	{
		m_SubWindow.m_UIManager->RemoveAllPanelsRects();
		m_SubWindow.m_UIManager->SetMouseMoveFlag(FALSE);
		m_SubWindow.m_UIManager->GetDirtyRectMan()->ClearDirtyRect();
	}

}

VOID QGLApplication::RemoveCharCtrlFromWorld(CCharacterControl * pCharCtrl)
{
	if (pCharCtrl)
	{
		StlVector<StlMultiMap<UInt32, EffectData>::iterator> RmLst;
		for (StlMultiMap<UInt32, EffectData>::iterator it = m_EffectDataLst.begin(); it != m_EffectDataLst.end(); ++it)
		{
			if (it->second.targetEnt == pCharCtrl->GetCharMainView())
				RmLst.push_back(it);
		}

		for (Int i = 0; i < (Int)RmLst.size(); i ++)
		{
			m_EffectDataLst.erase(RmLst[i]);
		}
		if (m_MapCharacterLst.find((UInt)pCharCtrl) != m_MapCharacterLst.end())
			m_MapCharacterLst.erase(m_MapCharacterLst.find((UInt)pCharCtrl));
		RemoveMapActorRenderLst(pCharCtrl);
		
		if (gGlobal.g_MenuTargetProcess->m_char_id == pCharCtrl->GetMapActorID())
			gGlobal.g_MenuTargetProcess->HideTargetMenu();
	}
}

VOID QGLApplication::RemoveMapActorRenderLst(CMapActorControl * pMapCtrlActor)
{
	if (m_RenderCMapCtrlLst.size() == 0)
		return;

	if (m_RenderCMapCtrlLst.find(pMapCtrlActor->GetCharMainView()) != m_RenderCMapCtrlLst.end())
		m_RenderCMapCtrlLst.erase(m_RenderCMapCtrlLst.find(pMapCtrlActor->GetCharMainView()));
	if (pMapCtrlActor == m_SelectedMapControl)
		m_SelectedMapControl = NULL;
}

VOID QGLApplication::AddCharCtrlToMap(CCharacterControl * pCharCtrl)
{
	if (pCharCtrl)
	{
		
		if (m_MapCharacterLst.size() == 0)
		{
			m_MapCharacterLst.insert(std::make_pair((UInt)pCharCtrl->GetCharNetObject(), pCharCtrl));
		}
		else
		{
			UInt32CharCtrlPtrMap::iterator Itr = m_MapCharacterLst.find((UInt)pCharCtrl->GetCharNetObject());
			if (Itr != m_MapCharacterLst.end())
			{
				if (gConfigure.uNetwork != 0)
					DeleteCharacterByNetObject(Itr->second->GetCharNetObject());
			}
			m_MapCharacterLst.insert(std::make_pair((UInt)pCharCtrl->GetCharNetObject(), pCharCtrl));
		}
	}
}

VOID QGLApplication::DeleteCharacterByNetObject(CCharacter * pChar)
{
	UInt32CharCtrlPtrMap::iterator Itr = m_MapCharacterLst.find((UInt)pChar);
	if (Itr != m_MapCharacterLst.end())
	{
		CCharacterControl * pCharCtrl = Itr->second;
		m_MapCharacterLst.erase(Itr);
		SafeDeleteObject(pCharCtrl);
	}
}

CCharacterControl * QGLApplication::FindCharCtrlByNetObject(CCharacter * pChar)
{
	if (pChar == NULL)
		return NULL;

	UInt32CharCtrlPtrMap::iterator Itr = m_MapCharacterLst.find((UInt)pChar);
	if (Itr != m_MapCharacterLst.end())
	{
		return Itr->second;	
	}
	
	return NULL;
}
VOID QGLApplication::ResetAllCharactersNetObject()
{
	for (UInt32CharCtrlPtrMap::iterator Itr = m_MapCharacterLst.begin();
		Itr != m_MapCharacterLst.end(); ++Itr)
	{
		if (gGlobal.g_pCharacter == Itr->second)
			continue;

		CCharacterControl * pChar = Itr->second;
		pChar->SetMapActorObject(NULL);
	}
}
VOID QGLApplication::DeleteAllCharacters()
{
	for (UInt32CharCtrlPtrMap::iterator Itr = m_MapCharacterLst.begin();
		Itr != m_MapCharacterLst.end(); ++Itr)
	{
		if (gGlobal.g_pCharacter == Itr->second)
			continue;
		CCharacterControl * pChar = Itr->second;
		SafeDeleteObject(pChar);
	}

	m_MapCharacterLst.clear();
	m_CurrMapView->FreeEntityData();
	//m_CurrMapView->FreeMapData();
}
VOID QGLApplication::DeleteCharacter(CCharacterControl * pChar)
{
	UInt32CharCtrlPtrMap::iterator Itr = m_MapCharacterLst.begin();

	while (Itr != m_MapCharacterLst.end())
	{
		if (Itr->second == pChar)
		{
			m_MapCharacterLst.erase(Itr);
			//find in battle char lst
			SafeDeleteObject(pChar);
			
			return;
		}
		Itr ++;
	}
}

VOID QGLApplication::AddParticleSystem(CParticleSystem * pParSys)
{
	if (pParSys == NULL)
		return;

	StlVector<CParticleSystem *>::iterator pParSysItr = pParSysLst.begin();
	while (pParSysItr != pParSysLst.end())
	{
		if (*pParSysItr == pParSys)
		{
			return;
		}
		pParSysItr++;
	}

	pParSys->Reset();
	pParSysLst.push_back(pParSys);
}

VOID QGLApplication::RemoveParticleSystem(CParticleSystem * pParSys)
{
	StlVector<CParticleSystem *>::iterator pParSysItr = pParSysLst.begin();
	while (pParSysItr != pParSysLst.end())
	{
		if (*pParSysItr == pParSys)
		{
			pParSysLst.erase(pParSysItr);
			break;
		}
		pParSysItr++;
	}
}


BOOL QGLApplication::AddEntityToLst(CEntityBase * pEnt, StlMultiMap<UInt32, CEntityBase *> & EntRenderLst)
{
	if (pEnt == NULL)
		return FALSE;

	if (!pEnt->GetVisible())
		return FALSE;

	CEntityBase * pCurrEntBase = pEnt;

	if (GetRenderClass()->IsEntityInsideRenderWindow(pCurrEntBase, &m_CurrCam))
	{
		DWORD MapWidth = 0;
		ZValue = 0;
		if (m_CurrMapView->GetMapData())
			MapWidth = m_CurrMapView->GetMapWidth();
		ZValue = pCurrEntBase->GetZValue(MapWidth);

		EntRenderLst.insert(std::make_pair(ZValue, pCurrEntBase));
		return TRUE;
	}
	return FALSE;
}
BOOL QGLApplication::IsValidTargetType(UInt8 targetType, CCharacterControl * pSrcCtrl, CCharacterControl * pTargetCtrl)
{
	if (pSrcCtrl == NULL ||
		pTargetCtrl == NULL)
		return FALSE;

	if (m_mapState->GetStateID() == ST_APP_MAP)
	{
		switch(targetType)
		{
		case TARGET_TYPE_ANY:
			return TRUE;
			break;
		case TARGET_TYPE_SELF:
			if (pSrcCtrl == pTargetCtrl)
				return TRUE;
			break;
		case TARGET_TYPE_ANY_SAME_TEAM:
			if (pSrcCtrl == pTargetCtrl 
				|| (gGlobal.g_pParty && gGlobal.g_pParty->FindMember(pTargetCtrl->GetMapActorID()))
				|| (gGlobal.g_pTempLeaveParty && gGlobal.g_pTempLeaveParty->FindMember(pTargetCtrl->GetMapActorID())))
				return TRUE;
			break;
		case TARGET_TYPE_ANY_ENEMY_TEAM:
			if ((gGlobal.g_pParty && gGlobal.g_pParty->FindMember(pTargetCtrl->GetMapActorID()) == NULL) ||
				(gGlobal.g_pTempLeaveParty && gGlobal.g_pTempLeaveParty->FindMember(pTargetCtrl->GetMapActorID())) ||
				(pSrcCtrl != pTargetCtrl && gGlobal.g_pParty == NULL) )
				return TRUE;
			break;
		}
		return FALSE;
	}
	CBatActor * pSrc = pSrcCtrl->GetBatActorObject();
	CBatActor * pTarget = pTargetCtrl->GetBatActorObject();

	if (pSrc == NULL || pTarget == NULL)
		return FALSE;

	switch(targetType)
	{
	case TARGET_TYPE_ANY:
		break;
	case TARGET_TYPE_SELF:
		if (pSrc != pTarget)
			return false;
		break;
	case TARGET_TYPE_ANY_SAME_TEAM:
		if (pSrc->GetTeam() != pTarget->GetTeam())
			return false;
		break;
	case TARGET_TYPE_ANY_ENEMY_TEAM:
		if (pSrc->GetTeam() == pTarget->GetTeam())
			return false;
		break;
	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_MOB_PARNTER:
		if (pSrc->GetTeam() == pTarget->GetTeam())
			return false;

		if (pTarget->IsParnter() ||
			pTarget->IsMob())
			return false;

		break;
	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_BOSS_PLAYER:
		if (pSrc->GetTeam() == pTarget->GetTeam())
			return false;
		if (pTarget->IsBoss() ||
			pTarget->IsPlayer())
			return false;
		
		break;
	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_MOB_PLAYER_PARNTER:
		if (pSrc->GetTeam() == pTarget->GetTeam())
			return false;
		if (pTarget->IsParnter() ||
			pTarget->IsPlayer() ||
			(pTarget->IsMob() && !pTarget->IsBoss()))
			return false;

			break;
	case TARGET_TYPE_ANY_PARNTER:
		{
			if (!pTarget->IsParnter())
				return false;
		}
		//pSrc->GetTeam() != pTarget->GetTeam())
		break;
	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_PLAYER_PARNTER:
		{
			if (pSrc->GetTeam() == pTarget->GetTeam())
				return false;
			if (pTarget->IsParnter() ||
				pTarget->IsPlayer())
				return false;
		}	
		break;
	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_BOSS:
		{
			if (pSrc->GetTeam() == pTarget->GetTeam())
				return false;
			if (pTarget->IsBoss())
				return false;
			break;
		}
	case TARGET_TYPE_ANY_PET:
		break;
	case TARGET_TYPE_ANY_SAME_TEAM_PLAYER:
		{
			if (pSrc->GetTeam() != pTarget->GetTeam())
				return false;
			if (!pTarget->IsPlayer())
				return false;
			break;
		}
	case TARGET_TYPE_ANY_SAME_TEAM_PARNTER:
		{
			if (pSrc->GetTeam() != pTarget->GetTeam())
				return false;
			if (!pTarget->IsParnter())
				return false;
			break;
		}
	}

	return TRUE;
}
BOOL QGLApplication::IsValidAttackTarget(CCharacterControl * pSrcCtrl, CCharacterControl * pTargetCtrl)
{
	
	if (pSrcCtrl == NULL ||
		pTargetCtrl == NULL)
		return FALSE;

	if (gConfigure.uNetwork == 0)
		return TRUE;

	CBatActor * pSrc = pSrcCtrl->GetBatActorObject();
	CBatActor * pTarget = pTargetCtrl->GetBatActorObject();

	if (pSrc == NULL || pTarget == NULL ||
		pTarget == pSrc)
		return FALSE;

	return TRUE;
}

VOID QGLApplication::SelectEntity(Int PosX, Int PosY)
{
	//return;

	CMapActorControl * OrgCtrl = m_SelectedMapControl;
	
	m_SelectedMapControl = FindMapActCtrlInWindow(PosX, PosY, m_CurrMapView->GetMapWidth());
	
	if (OrgCtrl != m_SelectedMapControl)
	{
		if (OrgCtrl)
		{
			OrgCtrl->GetCharMainView()->SetHighLight(FALSE);
			OrgCtrl->GetCharMainView()->m_Updated = TRUE;

			OrgCtrl->GetCharNameView()->SetFontColor(OrgCtrl->GetFontColor());
			OrgCtrl->GetCharFrontNameView()->SetFontColor(OrgCtrl->GetFrontFontColor());
			
		}

		CMapUIBaseState * Cursor = dynamic_cast<CMapUIBaseState *>(gGlobal.g_CursorState);

		if (m_SelectedMapControl)
		{
			m_SelectedMapControl->GetCharMainView()->SetHighLight(TRUE);
			m_SelectedMapControl->GetCharMainView()->m_Updated = TRUE;
			m_SelectedMapControl->GetCharNameView()->SetFontColor(TextColor[ColorIndex_White]);
			m_SelectedMapControl->GetCharFrontNameView()->SetFontColor(TextColor[ColorIndex_White]);

			if (m_mapState->GetStateID() == ST_APP_MAP)
			{
				//check whether it is npc
				if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_SKILL)
				{
					CCharacterControl * pCharCtrl = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
					if(pCharCtrl)
					{
						if (!IsValidTargetType(gGlobal.g_pSkillProcess->m_pTempSkillData->targetType, gGlobal.g_pCharacter ,pCharCtrl))
						{
							Cursor->ErrorState();
						}
						else
							Cursor->NormalSate();
					}
				}
				else if (m_SelectedMapControl->GetMapActorType() == MAPACTORTYPE_NPC)
					State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINPCState));
				else
					State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));

			}
			else if (m_mapState->GetStateID() == ST_APP_BATTLE )
			{
				if (!gConfigure.uNetwork)
				{
					if (m_SelectedMapControl)
					{		
						State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIAttackState));
					}
				}
				else if (gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MONITOR)
				{
					if (m_SelectedMapControl->GetMapActorType() == MAPACTORTYPE_PLAYER)
					{
						CCharacterControl * pCharCtrl = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
						if(pCharCtrl)
						{
							if (pCharCtrl->GetBatActorObject())
							{
								//check cursor status
								if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_PROTECT)
								{
									if (pCharCtrl->GetBatActorObject()->GetTeam() != gGlobal.g_pBattleControl->GetControlPlayerTeam() ||
										(pCharCtrl == gGlobal.g_pBattleControl->GetControlPlayer() && !gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand()) ||
										(pCharCtrl == gGlobal.g_pBattleControl->GetControlPartner() && gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand()) )
									{
											Cursor->ErrorState();									
									}
									else
										Cursor->NormalSate();
								}
								else if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_USEITEM)
								{
									CItem * pItem = gGlobal.m_pBag->GetItem(gGlobal.g_pBattleControl->GetBattleItemUseIndex());
									if (pItem)
									{
										const ItemData* pitemData = gGlobal.m_resource->GetItemData(pItem->GetItem_id());
										if (pitemData)
										{
											if (!IsValidTargetType(pitemData->targetType, gGlobal.g_pBattleControl->GetControlPlayer(), pCharCtrl))
											{
												Cursor->ErrorState();
											}
											else
												Cursor->NormalSate();
										}
									}
								}
								else if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_SKILL)
								{
									if (gGlobal.g_pBattleControl->GetControlPlayer() && !gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand())
									{
										if (!IsValidTargetType(gGlobal.g_pSkillProcess->m_pTempSkillData->targetType, gGlobal.g_pBattleControl->GetControlPlayer() ,pCharCtrl))
										{
											Cursor->ErrorState();
										}
										else
											Cursor->NormalSate();
									}
									else if (gGlobal.g_pBattleControl->GetControlPartner() && !gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand())
									{
										if (gGlobal.g_pPartnerProcess->GetSelSkillData() == NULL || !IsValidTargetType(gGlobal.g_pPartnerProcess->GetSelSkillData()->targetType, gGlobal.g_pBattleControl->GetControlPartner() ,pCharCtrl))
										{
											Cursor->ErrorState();
										}
										else
											Cursor->NormalSate();
									}
									
								}
								else if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_CATCH)
								{
									if (!IsValidCatchTarget( gGlobal.g_pBattleControl->GetControlPlayer(), pCharCtrl ))
									{
										Cursor->ErrorState();									
									}
									else
										Cursor->NormalSate();
								}
								else if (pCharCtrl->GetBatActorObject()->GetTeam() != gGlobal.g_pBattleControl->GetControlPlayerTeam())
								{
									if (gGlobal.g_pBattleControl->GetControlPlayer() && !gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand())
									{
										if ( !gGlobal.g_pBattleControl->GetControlPlayer()->m_ActionCmdAva[BATCMD_ATTACK] ||
											!IsValidAttackTarget(gGlobal.g_pBattleControl->GetControlPlayer(), pCharCtrl))
											Cursor->ErrorState();
										else
										{
											Cursor->NormalSate();
											State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIAttackState));
										}
									}
									else if (gGlobal.g_pBattleControl->GetControlPartner() && !gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand())
									{
										if (!gGlobal.g_pBattleControl->GetControlPartner()->m_ActionCmdAva[BATCMD_ATTACK] ||
											!IsValidAttackTarget(gGlobal.g_pBattleControl->GetControlPartner(), pCharCtrl))
											Cursor->ErrorState();
										else
										{
											Cursor->NormalSate();
											State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIAttackState));
										}
									}
									else
										State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIAttackState));
									//State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIAttackState));
								}
								else if (pCharCtrl->GetBatActorObject()->GetTeam() == gGlobal.g_pBattleControl->GetControlPlayerTeam() && !gGlobal.g_bCtrlButtonUp )
								{
									if ( (gGlobal.g_pBattleControl->GetControlPlayer() && !gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() 
										&& pCharCtrl != gGlobal.g_pBattleControl->GetControlPlayer()) || 
										(gGlobal.g_pBattleControl->GetControlPartner() && !gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand() 
										&& pCharCtrl != gGlobal.g_pBattleControl->GetControlPartner()))
									{
										Cursor->NormalSate();
										State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CBattleUIAttackState));
									}
									else
										State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
								}
								else
									State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
							}
						}
					}
					else
						State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
				}
			}
		}
		else
		{
			Cursor->NormalSate();
			State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
		}
		
	}
}

VOID QGLApplication::AddMapCtrlToRenderLst(CMapActorControl * pMapCtrlActor, bool showFlag)
{
	if (pMapCtrlActor == NULL)
		return;

	/*UInt32 ZValueShadow = pMapCtrlActor->GetCharShadowView()->GetZValue(7000);
	UInt32 ZValueChar = pMapCtrlActor->GetCharMainView()->GetZValue(7000);
	if (ZValueShadow >= ZValueChar)
	{
		ZValueChar = ZValueChar;
		ZValueShadow = ZValueShadow;
		pMapCtrlActor->GetCharShadowView()->GetZValue(7000);
		pMapCtrlActor->GetCharMainView()->GetZValue(7000);
	}*/

	if (showFlag)
	{
		if (AddEntityToLst(pMapCtrlActor->GetCharMainView(), m_EntRenderLst))
			m_RenderCMapCtrlLst.insert(std::make_pair(pMapCtrlActor->GetCharMainView(), pMapCtrlActor));
		AddEntityToLst(pMapCtrlActor->GetCharShadowView(), m_EntRenderLst);

		AddEntityToLst(pMapCtrlActor->GetHeadIconView(), m_EntRenderLst);
		if (pMapCtrlActor->GetCharFrontNameView()->haveContent())
			AddEntityToLst(pMapCtrlActor->GetCharFrontNameView(), m_EntRenderLst);
		for (StlVector<ChatBalloonData>::iterator Itr = pMapCtrlActor->GetChatBalloonLst()->begin();
			Itr != pMapCtrlActor->GetChatBalloonLst()->end(); ++Itr)
		{
			if (::timeGetTime() >= Itr->StartTime)
				AddEntityToLst(Itr->pBalloon, m_EntRenderLst);
		}
	}

	AddEntityToLst(pMapCtrlActor->GetCharNameView(), m_EntRenderLst);


}

VOID QGLApplication::UpDateMapScene(UInt32 CurrentTime)
{
	static UInt32 Time = 0;
	UInt32 NOW = GetTickCount();

	/*
	if(NOW - Time > 1000 * 3 * 1)
	{
		UnHook();
		InstallHook();
		Time = NOW;
	}
	*/

	m_CurrMapView->UpDate(CurrentTime);
	if (m_RenderMap)
	{
		m_CurrCam.UpDateCam(CurrentTime);

		GetRenderClass()->UpdateCameraMove(m_CurrMapView, &m_CurrCam);
		//add map object
		for (Int i = 0; i < (Int)m_CurrMapView->m_MapObjectLst.size(); i++)
		{
			AddEntityToLst(m_CurrMapView->m_MapObjectLst[i], m_EntRenderLst);
		}

		StlMap<UInt, CEntityBase * >::iterator MEItr = m_CurrMapView->m_MapEntLst.begin();
		while( MEItr != m_CurrMapView->m_MapEntLst.end())
		{
			AddEntityToLst(MEItr->second, m_EntRenderLst);
			MEItr++;
		}

		StlMap<UInt, CMapNPCControl * >::iterator MapNPCItr = m_CurrMapView->m_MapNPCCtrlLst.begin();
		while( MapNPCItr != m_CurrMapView->m_MapNPCCtrlLst.end())
		{
			AddMapCtrlToRenderLst(MapNPCItr->second);
			MapNPCItr++;
		}

		CQuestionProcess::GetInstance().GetCountDownEnt()->SetEntPixelPos(PixelPos((Int)m_CurrCam.GetLeftUpPos().x +
			GetRenderClass()->GetWinWidth() / 2, (Int)m_CurrCam.GetLeftUpPos().y + 40));
		for (size_t i = 0; i < CQuestionProcess::GetInstance().GetCountDownEnt()->m_NumEntLst.size(); i++)
		{
			CQuestionProcess::GetInstance().GetCountDownEnt()->m_NumEntLst[i]->UpDate(CurrentTime);
			AddEntityToLst(CQuestionProcess::GetInstance().GetCountDownEnt()->m_NumEntLst[i], m_EntRenderLst);
		}
	
	}
}

VOID QGLApplication::UpDateCharScene(UInt32 CurrentTime)
{
	UInt32CharCtrlPtrMap::iterator pCharItr = m_MapCharacterLst.begin();

	while (pCharItr != m_MapCharacterLst.end())
	{
		CCharacterControl* pCtrl = pCharItr->second;
		if (pCtrl)
		{
			pCtrl->UpdateBuffTime(CurrentTime);
			if (pCtrl->m_mapState->GetStateID() == m_mapState->GetStateID())
			{
				bool showFlag =  !pCtrl->IsHide();
				pCtrl->UpDateView(CurrentTime);				
				AddMapCtrlToRenderLst(pCtrl, showFlag);
				
				if (pCtrl->HasPartner())
				{
					pCtrl->UpdatePartnerView(CurrentTime);
					if (!pCtrl->IsHide())
					{
						AddEntityToLst(pCtrl->GetPartnerMainView(), m_EntRenderLst);
						AddEntityToLst(pCtrl->GetPartnerShadowView(), m_EntRenderLst);
					}
					AddEntityToLst(pCtrl->GetPartnerNameView(), m_EntRenderLst);
				}

				if (pCtrl->HasFollowPet())
				{
					pCtrl->UpdatePetView(CurrentTime);
					if (!pCtrl->IsHide())
					{
						AddEntityToLst(pCtrl->GetPetMainView(), m_EntRenderLst);
						AddEntityToLst(pCtrl->GetPetShadowView(), m_EntRenderLst);
					}
					AddEntityToLst(pCtrl->GetPetNameView(), m_EntRenderLst);
				}				
				if (pCtrl->HasFollowTwoPet())				{					pCtrl->UpdatePetTwoView(CurrentTime);					if (!pCtrl->IsHide())					{						AddEntityToLst(pCtrl->GetTwoPetMainView(), m_EntRenderLst);						AddEntityToLst(pCtrl->GetTwoPetShadowView(), m_EntRenderLst);					}					AddEntityToLst(pCtrl->GetTwoPetNameView(), m_EntRenderLst);				}				if (pCtrl->HasFollower())
				{
					pCtrl->UpdateFollowerView(CurrentTime);
					for (int i = 0; i < pCtrl->GetFollowerCount(); ++i)
					{
						if (!pCtrl->IsHide())
						{
							AddEntityToLst(pCtrl->GetFollowerMainView(i), m_EntRenderLst);
							AddEntityToLst(pCtrl->GetFollowerShadowView(i), m_EntRenderLst);
						}
						AddEntityToLst(pCtrl->GetFollowerNameView(i), m_EntRenderLst);
					}
				}				
			}
			if (pCtrl->GetAction() == CCharacter::ST_HAWK)
			{
				pCtrl->UpdateHawkBubbleView(CurrentTime);
			}
		}
		pCharItr++;
	}

	//add test player
	for (Int i = 0; i < (Int)TestPlayerLst.size(); i ++)
	{
		TestPlayerLst[i]->UpDate(CurrentTime);
		AddEntityToLst(TestPlayerLst[i], m_EntRenderLst);
	}

}

VOID QGLApplication::UpDateParSysScene(UInt32 CurrentTime)
{
	StlVector<CParticleSystem *>::iterator pParSysItr = pParSysLst.begin();

	while (pParSysItr != pParSysLst.end())
	{
		(*pParSysItr)->UpDate(CurrentTime);
		StlMap<CEntityBase *, ParticleStruct>::iterator PSItr = (*pParSysItr)->m_ParticleEntLst.begin();
		while (PSItr != (*pParSysItr)->m_ParticleEntLst.end())
		{
			if (CurrentTime >= PSItr->second.StartTime)
				AddEntityToLst(PSItr->first, m_EntRenderLst);
			/*else
			{
				int i = 0;
				i ++;
			}*/
			PSItr ++;
		}
		pParSysItr++;
	}

	if (m_MapParticle)
	{
		m_MapParticle->UpDate(CurrentTime);

		StlMap<CEntityBase *, ParticleStruct>::iterator PSItr = m_MapParticle->m_ParticleEntLst.begin();
		while (PSItr != m_MapParticle->m_ParticleEntLst.end())
		{
			if (CurrentTime >= PSItr->second.StartTime)
				AddEntityToLst(PSItr->first, m_EntRenderLst);
			/*else
			{
				int i = 0;
				i ++;
			}*/
			PSItr ++;
		}
	}
}

VOID QGLApplication::UpDateMaskEntScene(UInt32 CurrentTime)
{
	if (m_pMaskEntity && m_mapState->GetStateID() == ST_APP_MAP && gGlobal.Night)
	{
		m_pMaskEntity->UpDate(CurrentTime);

		Int MaskWidth = m_pMaskEntity->m_EntityTotalRect.right - m_pMaskEntity->m_EntityTotalRect.left;
		Int MaskHeight = m_pMaskEntity->m_EntityTotalRect.bottom - m_pMaskEntity->m_EntityTotalRect.top;

		RECT NextDirtyRect;
		//RECT NextDirtyRect = GetRenderClass()->FindNextDirtRectByEntity(gGlobal.g_pCharacter->GetCharMainView());
		
		NextDirtyRect.left = gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos().PixelPosX  - MaskWidth / 2 - (Int)m_CurrCam.GetLeftUpPos().x;
		NextDirtyRect.top = gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos().PixelPosY  - MaskHeight / 2 - (Int)m_CurrCam.GetLeftUpPos().y;
		NextDirtyRect.right = NextDirtyRect.left  + MaskWidth;
		NextDirtyRect.bottom = NextDirtyRect.top  + MaskHeight;



		QGLMath::CalculateOverlappedRect(GetRenderClass()->GetWinRect(), NextDirtyRect, NextDirtyRect);
		//center to player
		//GetRenderClass()->IsEntityInsideTexture(m_pMaskEntity, gGlobal.g_NightMaskTextureData, PixelPos((Int)m_CurrCam.GetLeftUpPos().x, 
		//	(Int)m_CurrCam.GetLeftUpPos().y), &NextDirtyRect);

		if (m_pMaskEntity->m_Updated)
		{
			
			StlMap<CEntityBase *, RECT>::iterator PreDirRectItr = gGlobal.g_NightMaskTextureData.m_PreDirtyRegionLst.find(m_pMaskEntity);
			if (PreDirRectItr != gGlobal.g_NightMaskTextureData.m_PreDirtyRegionLst.end())
			{
				gGlobal.g_NightMaskTextureData.m_DirRectMan.AddDirtyRectToMan(PreDirRectItr->second);
			}
			gGlobal.g_NightMaskTextureData.m_DirRectMan.AddDirtyRectToMan(NextDirtyRect);
		}

		gGlobal.g_NightMaskTextureData.m_PreDirtyRegionLst.clear();
		gGlobal.g_NightMaskTextureData.m_PreDirtyRegionLst.insert(std::make_pair(m_pMaskEntity, NextDirtyRect));
		gGlobal.g_NightMaskTextureData.m_NextDirtyRegionLst.clear();
		
		//get the bg
	}
}
VOID QGLApplication::UpdateSceneData()
{
	m_EntRenderLst.clear();
	m_RenderCMapCtrlLst.clear();
	DWORD CurrentTime = ::timeGetTime();
	
	if (m_mapState->GetStateID() == ST_APP_LOGIN)
	{
		m_LoginEnt.UpDate(CurrentTime);
		AddEntityToLst(&m_LoginEnt, m_EntRenderLst);
	}
	UpDateMapScene(CurrentTime);	
	
	UpDateCharScene(CurrentTime);
	
	//update BattleScene Data
	UpdateBattleScene(CurrentTime);

	UpDateParSysScene(CurrentTime);
	
	//add night mask entity
	UpDateMaskEntScene(CurrentTime);

	//update the masktexture
	UpdateMaskTexture(CurrentTime);

	UpdateDisposalEffect(CurrentTime);

	UpdateEffectDataLst(CurrentTime);

	UpateSoundDataLst(CurrentTime);

	gGlobal.g_pRumorMsgProcess->Update(CurrentTime);

	// count down for waiting party choose result
	CountDownForPartyChoose();

	ResourcesUpdate();

}

VOID QGLApplication::UpdateBattleScene(UInt32 CurrentTime)
{
	if (gGlobal.g_pBattleControl && m_mapState->GetStateID() == ST_APP_BATTLE)
	{
		for (Int i = 0; i < (Int)gGlobal.g_pBattleControl->m_BattlePlayerLst.size(); i++)
		{
			CCharacterControl * pCharCtrl = gGlobal.g_pBattleControl->m_BattlePlayerLst[i].pChar;
			pCharCtrl->UpDateView(CurrentTime);
			AddMapCtrlToRenderLst(pCharCtrl);

			pCharCtrl = gGlobal.g_pBattleControl->m_BattlePlayerLst[i].pPet;
			if (pCharCtrl)
			{
				pCharCtrl->UpDateView(CurrentTime);
				AddMapCtrlToRenderLst(pCharCtrl);
			}

		}


		CAnimatedEntityPtrList::iterator Itr = gGlobal.g_pBattleControl->m_BattlePlayerHPBarBackLst.begin();

		while (Itr != gGlobal.g_pBattleControl->m_BattlePlayerHPBarBackLst.end())
		{
			(*Itr)->UpDate(CurrentTime);
			AddEntityToLst((*Itr), m_EntRenderLst);
			Itr++;
		}

		Itr = gGlobal.g_pBattleControl->m_BattlePlayerHPBarLst.begin();

		while (Itr != gGlobal.g_pBattleControl->m_BattlePlayerHPBarLst.end())
		{
			(*Itr)->UpDate(CurrentTime);
			AddEntityToLst((*Itr), m_EntRenderLst);
			Itr++;
		}

		for (size_t i = 0; i < gGlobal.g_pBattleControl->GetCountDownEnt()->m_NumEntLst.size(); i++)
		{
			gGlobal.g_pBattleControl->GetCountDownEnt()->m_NumEntLst[i]->UpDate(CurrentTime);
			AddEntityToLst(gGlobal.g_pBattleControl->GetCountDownEnt()->m_NumEntLst[i], m_EntRenderLst);
		}
	}
}
VOID QGLApplication::DrawScene()
{
	if (!GetRenderClass()->IsDeviceValide())
		return;

	//GetRenderClass()->AddAllDirtyRect();

	GetRenderClass()->BeginScene();

	//BOOL DrawAll = FALSE;

	GetRenderClass()->LockTexture();

	//add dirty rect for cursor position
	GetRenderClass()->AddCursorDirtyRect();

	if (m_RenderMap)
	{
		//draw the whole map if the map data is updated
		
		GetRenderClass()->CameraMoveDraw(m_CurrMapView, m_EntRenderLst, &m_CurrCam, gGlobal.g_NightMaskTextureData);

		if (GetRenderClass()->GetDirtyRectManager()->GetAddAvial())
		{
			if (m_CurrMapView->m_Updated) 
			{
				GetRenderClass()->AddMapDirtyRect(m_CurrMapView, &m_CurrCam);
				//GetRenderClass()->AddAllDirtyRect();
				//GetRenderClass()->UpdateCameraMove(m_CurrMapView, &m_CurrCam);
				//DrawAll = TRUE;
			}
			//otherwise, just draw some updated part of the Map
			//else
			{
				//update the camera move 
				GetRenderClass()->AddEntityDirtyRect(m_EntRenderLst, m_pMaskEntity, *GetRenderClass()->GetSystemCombinedData());
			}
		}
		
		GetRenderClass()->DrawBgMapByCurrDirtyRect(m_CurrMapView, &m_CurrCam);
	}
	else
	{
		GetRenderClass()->AddEntityDirtyRect(m_EntRenderLst, m_pMaskEntity, *GetRenderClass()->GetSystemCombinedData());
	}
	
	//draw battle backgorund picture
	if (gGlobal.g_pBattleControl)
		GetRenderClass()->DrawGivenTextureByCurrDirtyRect(m_BattleBgTexture, m_RenderMap, 
		gGlobal.g_pBattleControl->GetBgAlphaValue());

	//draw all entities
	GetRenderClass()->DrawEntityLstToWorld(!GetRenderClass()->GetDirtyRectManager()->GetAddAvial(), m_EntRenderLst, m_CurrMapView
		, &m_CurrCam, m_pMaskEntity, m_mapState->GetStateID() == ST_APP_MAP);

	//draw masl entity (eg. night mask)
	//if (gGlobal.Night && !gConfigure.RenderNightByHardware)
	//	GetRenderClass()->DrawMaskEntity(DrawAll, m_pMaskEntity, m_CurrMapView, &m_CurrCam);

	GetRenderClass()->UnLockTexture();

	if (gGlobal.Night && gGlobal.g_NightMaskTextureData.Texture && m_mapState->GetStateID() == ST_APP_MAP && gConfigure.bEnableNight && gGlobal.g_pSystemSetting->IsNightModeEnable() )
			GetRenderClass()->DrawNightMask(gGlobal.g_NightMaskTextureData, gGlobal.NightMask, &m_CurrCam, m_CurrMapView);
	//update to default texture and to window back buffer
	//GetRenderClass()->DrawTextureBySpriteAndDirtyRect(GetRenderClass()->GetSystemCombinedTexture(), GetRenderClass()->GetDirtyRectManager());
	//GetRenderClass()->DrawTextureBySprite(GetRenderClass()->GetSystemCombinedTexture());
	
	//GetRenderClass()->UpDateTextureToWindowBufferByDirtyRect();
	//GetRenderClass()->UpDateTextureToWindowBuffer();
	

	if(m_mapState->GetStateID() == ST_APP_MAP && gConfigure.uMiniMapShow)
	{
		if(gGlobal.MiniMap && gGlobal.g_pCharacter)
		{
			static UInt16 CharX = 0;
			static UInt16 CharY = 0;
			UInt16	X = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
			UInt16	Y = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;

			if(X != CharX || Y  != CharY || gGlobal.MiniMap->GetRedrawFlag())
			{
				CharX = X;
				CharY = Y;
				
				gGlobal.PositionText->GetString().Format(_T("X:%4d  Y:%4d"), X, Y);
				gGlobal.PositionText->Redraw();
				gGlobal.PositionText->RenderParent();				
			
				gGlobal.MiniMap->SetRedrawFlag(false);
				gGlobal.MiniMap->ResetPlayerPos( gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos().PixelPosX,
					gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos().PixelPosY);
				
				gGlobal.MiniMap->RenderParent();									
			}
		}				
	}
	

	gGlobal.pUIManager->RenderAllPanelsByRects();

	if (m_SubWindow.m_UIManager)
		m_SubWindow.m_UIManager->RenderAllPanelsByRects();

	//GetRenderClass()->UpDateTextureFromSysToDea();
	*gGlobal.g_AllDirtyRectMan = *GetRenderClass()->GetDirtyRectManager();

	for (Int i = 0; i < (Int)gGlobal.pUIManager->GetDirtyRectMan()->GetDirtyRectLst().size(); i++)
		gGlobal.g_AllDirtyRectMan->AddDirtyRectToMan(gGlobal.pUIManager->GetDirtyRectMan()->GetDirtyRectLst()[i]);
	
	if (GetRenderClass()->UpDateTextureFromSysToDea())
	{
		//draw mask texture by sprite
		//if (gGlobal.Night && gConfigure.RenderNightByHardware)
		//	GetRenderClass()->DrawNightMaskByPS(m_CurrMapView, &m_CurrCam, gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos());

		GetRenderClass()->BeginSprite();

		if (gGlobal.Night && gGlobal.g_NightMaskTextureData.Texture && m_mapState->GetStateID() == ST_APP_MAP && gConfigure.bEnableNight)
		{
			//GetRenderClass()->DrawNightMask(gGlobal.g_NightMaskTextureData, gGlobal.NightMask, &m_CurrCam, m_CurrMapView);
			if (gGlobal.g_DrawAll)
			{
				//gGlobal.g_NightMaskTextureData.Texture->AddDirtyRectToMan(NULL);
				GetRenderClass()->DrawTextureBySprite(0, 0,
				gGlobal.g_NightMaskTextureData.m_CurrDisArea, gGlobal.g_NightMaskTextureData.Texture, gGlobal.g_NightMaskAlpha);
			}
			else
			{
				/*if (gGlobal.pUIManager->GetDirtyRectMan()->CheckOverlapped())
				{
					int i = 0;
					i ++;
				}*/
				for (Int i = 0; i < (Int)gGlobal.g_AllDirtyRectMan->GetDirtyRectLst().size(); i++)
				{
					//gGlobal.pUIManager->BlitAll(GetRenderClass()->GetSprite(), 0, 0, &gGlobal.pUIManager->GetDirtyRectMan()->GetDirtyRectLst()[i]);
					RECT ShiftRect = gGlobal.g_AllDirtyRectMan->GetDirtyRectLst()[i];
					ShiftRect.left += gGlobal.g_NightMaskTextureData.m_CurrDisArea.left;
					ShiftRect.right += gGlobal.g_NightMaskTextureData.m_CurrDisArea.left;
					ShiftRect.top += gGlobal.g_NightMaskTextureData.m_CurrDisArea.top;
					ShiftRect.bottom += gGlobal.g_NightMaskTextureData.m_CurrDisArea.top;
					//gGlobal.g_NightMaskTextureData.Texture->AddDirtyRectToMan(NULL);
					GetRenderClass()->DrawTextureBySprite( gGlobal.g_AllDirtyRectMan->GetDirtyRectLst()[i].left,
						gGlobal.g_AllDirtyRectMan->GetDirtyRectLst()[i].top,
						ShiftRect,
						gGlobal.g_NightMaskTextureData.Texture, gGlobal.g_NightMaskAlpha);
				}
			}
		}

		if (m_MaskTextureData.FadeInOutTime > 0)
		{
			if (gGlobal.g_DrawAll)
				GetRenderClass()->DrawTextureBySprite( m_MaskTextureData.MaskTexture, m_MaskTextureData.Alpha);
			else
			{
				/*for (Int i = 0; i < (Int)GetRenderClass()->GetDirtyRectManager()->GetDirtyRectLst().size(); i++)
				{
					//gGlobal.pUIManager->BlitAll(GetRenderClass()->GetSprite(), 0, 0, &GetRenderClass()->GetDirtyRectManager()->GetDirtyRectLst()[i]);
					GetRenderClass()->DrawTextureBySprite( GetRenderClass()->GetDirtyRectManager()->GetDirtyRectLst()[i].left,
						GetRenderClass()->GetDirtyRectManager()->GetDirtyRectLst()[i].top,
						GetRenderClass()->GetDirtyRectManager()->GetDirtyRectLst()[i],
						m_MaskTextureData.MaskTexture, m_MaskTextureData.Alpha);
				}*/

				for (Int i = 0; i < (Int)gGlobal.g_AllDirtyRectMan->GetDirtyRectLst().size(); i++)
				{
					//gGlobal.pUIManager->BlitAll(GetRenderClass()->GetSprite(), 0, 0, &gGlobal.pUIManager->GetDirtyRectMan()->GetDirtyRectLst()[i]);
					GetRenderClass()->DrawTextureBySprite( gGlobal.g_AllDirtyRectMan->GetDirtyRectLst()[i].left,
						gGlobal.g_AllDirtyRectMan->GetDirtyRectLst()[i].top,
						gGlobal.g_AllDirtyRectMan->GetDirtyRectLst()[i],
						m_MaskTextureData.MaskTexture, m_MaskTextureData.Alpha);
				}
			}
		}

		DrawUITexture();
		GetRenderClass()->EndSprite();		
	}

	GetRenderClass()->ClearDirtyRectLst();
	GetRenderClass()->EndScene();
}

VOID QGLApplication::DrawUITexture()
{

	//GetRenderClass()->DrawTextureBySprite(m_UITexture.Texture);

	BOOL mouseMove = gGlobal.pUIManager->GetMouseMoveFlag();
	
	if (gGlobal.g_DrawAll)
	{
		gGlobal.pUIManager->BlitAll(GetRenderClass()->GetSprite());
	}
	else
	{

		/*for (Int i = 0; i < (Int)GetRenderClass()->GetDirtyRectManager()->GetDirtyRectLst().size(); i++)
		{
			gGlobal.pUIManager->BlitAll(GetRenderClass()->GetSprite(), 0, 0, &GetRenderClass()->GetDirtyRectManager()->GetDirtyRectLst()[i]);
		}*/

		for (Int i = 0; i < (Int)gGlobal.g_AllDirtyRectMan->GetDirtyRectLst().size(); i++)
			gGlobal.pUIManager->BlitAll(GetRenderClass()->GetSprite(), 0, 0, &gGlobal.g_AllDirtyRectMan->GetDirtyRectLst()[i]);
	}


	//GetRenderClass()->DrawTextureBySpriteAndDirtyRect(m_UITexture.Texture, GetRenderClass()->GetDirtyRectManager());

	/*
	if(gGlobal.pUIManager->DragNow())
	{
		GetRenderClass()->DrawTextureBySprite(gGlobal.pUIManager->GetNowDragX(),
			gGlobal.pUIManager->GetNowDragY(), gGlobal.pUIManager->GetDragTexture());
	}
	*/

	if(gGlobal.ShowCursor)// && (mouseMove || ))
	{
			RECT PreDirtyRect;
			PreDirtyRect.left = PreDirtyRect.right = PreDirtyRect.top = PreDirtyRect.bottom = 0;
			RECT NextDirtyRect = PreDirtyRect;

			if (gGlobal.CursorData.m_PreDirtyRegionLst.find(gGlobal.CursorEntity) != gGlobal.CursorData.m_PreDirtyRegionLst.end())
				PreDirtyRect = gGlobal.CursorData.m_PreDirtyRegionLst.find(gGlobal.CursorEntity)->second;

			if (GetRenderClass()->IsEntityInsideTexture(gGlobal.CursorEntity, gGlobal.CursorData, PixelPos(0, 0), &NextDirtyRect))
			{
				Int32	CX = gGlobal.pUIManager->GetCursorX() + gGlobal.pUIManager->GetCursorOffsetX();
				Int32	CY = gGlobal.pUIManager->GetCursorY() + gGlobal.pUIManager->GetCursorOffsetY();

				if (!QGLMath::IsTwoRectEqual(PreDirtyRect, NextDirtyRect))
				{
					GetRenderClass()->ClearTexture(gGlobal.CursorData.Texture, &PreDirtyRect);
				}

				if (gGlobal.CursorEntity->m_Updated)
				{
					GetRenderClass()->DrawEntityToTexture(gGlobal.CursorEntity, gGlobal.CursorData, NULL, TRUE);
				}
				
				//finially draw the UI texture
				if (gGlobal.g_DrawAll || mouseMove)
					GetRenderClass()->DrawTextureBySprite(CX, CY, NextDirtyRect,  gGlobal.CursorData.Texture);
				else
				{
					RECT realRect;// = NextDirtyRect;
					SetRect(&realRect, CX, CY,
						CX + NextDirtyRect.right - NextDirtyRect.left,
						CY + NextDirtyRect.bottom - NextDirtyRect.top);
					//realRect.left += gGlobal.pUIManager->GetCursorX();
					//realRect.right += gGlobal.pUIManager->GetCursorX();
					//realRect.top += gGlobal.pUIManager->GetCursorY();
					//realRect.bottom += gGlobal.pUIManager->GetCursorY();

					/*for (Int i = 0; i < (Int)GetRenderClass()->GetDirtyRectManager()->GetDirtyRectLst().size(); i++)
					{
						RECT reaultRect;
						if (QGLMath::CalculateOverlappedRect(realRect, GetRenderClass()->GetDirtyRectManager()->GetDirtyRectLst()[i],
							reaultRect))
						{
							RECT DrawRect = reaultRect;
							DrawRect.left -= gGlobal.pUIManager->GetCursorX();
							DrawRect.right -= gGlobal.pUIManager->GetCursorX();
							DrawRect.top -= gGlobal.pUIManager->GetCursorY();
							DrawRect.bottom -= gGlobal.pUIManager->GetCursorY();
							
							GetRenderClass()->DrawTextureBySprite(reaultRect.left,
								reaultRect.top , DrawRect,  gGlobal.CursorData.Texture);
						}
					}*/

					for (Int i = 0; i < (Int)gGlobal.g_AllDirtyRectMan->GetDirtyRectLst().size(); i++)
					{
						RECT reaultRect;
						if (QGLMath::CalculateOverlappedRect(realRect, gGlobal.g_AllDirtyRectMan->GetDirtyRectLst()[i],
							reaultRect))
						{
							RECT DrawRect = NextDirtyRect;
							DrawRect.left += reaultRect.left - realRect.left;
							DrawRect.right -= realRect.right - reaultRect.right;
							DrawRect.top += reaultRect.top - realRect.top;
							DrawRect.bottom -= realRect.bottom - reaultRect.bottom;
					

							GetRenderClass()->DrawTextureBySprite(reaultRect.left,
								reaultRect.top , DrawRect,  gGlobal.CursorData.Texture);
						}
					}
				}
			}
			gGlobal.CursorData.m_PreDirtyRegionLst = gGlobal.CursorData.m_NextDirtyRegionLst;
			gGlobal.CursorData.m_NextDirtyRegionLst.clear();
			gGlobal.CursorData.m_DirRectMan.ClearDirtyRect();
	}

	//IME_Layout::GetInstance().Blit(GetRenderClass()->GetSprite());
}

VOID QGLApplication::RemoveEntFromDisposalList(CAnimatedEntity * pEnt)
{
	if (pEnt == NULL)
		return;

	for (StlMultiMap<UInt, DisposalEffectData>::iterator Itr = m_EntityDisposalLst.find((UInt)pEnt);
		Itr != m_EntityDisposalLst.end() && Itr->first == (UInt)pEnt; Itr ++)
	{
		Itr = m_EntityDisposalLst.erase(Itr);
	}
	
}

VOID QGLApplication::UpdateDisposalEffect(UInt32 CurrTime)
{
	StlMultiMap<UInt, DisposalEffectData>::iterator Itr = m_EntityDisposalLst.begin();

	StlVector<StlMultiMap<UInt, DisposalEffectData>::iterator> RmLst;
	while (Itr != m_EntityDisposalLst.end())
	{
		if (CurrTime > Itr->second.EndTime)
		{
			RmLst.push_back(Itr);
		}
		//produce the effect
		else if (Itr->second.TimePass >= Itr->second.TimeToProduce)
		{
			ParticleStructSetting PSS;
			PSS.Destory = TRUE;
			PSS.ExtraZValue = 0;
			PSS.GivenDisposalAnim = Itr->second.pEnt->GetAnimationIndex();
			PSS.GivenDisposalTime = Itr->second.DisposalTime;
			PSS.Rotation = Itr->second.pEnt->GetAnimationRotation();
			PSS.HaveEffect = FALSE;
			PSS.LiveTimeBase = Itr->second.LifeTime;
			PSS.LiveTimeVar = 0;
			PSS.Loop = FALSE;
			PSS.MoveSpeed = 0;
			PSS.NoOfParticle = 1;
			PSS.Stop = TRUE;
			PSS.StartAlphaOffset = Itr->second.StartAlpha;
			gGlobal.g_DisposalEntityEffect->AddAnimatedEntityToSystem(Itr->second.pEnt,PSS,Itr->second.pEnt);
			Itr->second.TimePass = 0;
		}

		if (CurrTime > Itr->second.PreUpdateTime)
		{
			Itr->second.TimePass += CurrTime - Itr->second.PreUpdateTime;
			Itr->second.PreUpdateTime = CurrTime;
		}
		Itr ++;
	}

	for (Int i = 0; i < (Int)RmLst.size(); i++)
	{
		m_EntityDisposalLst.erase(RmLst[i]);
		break;
	}
}

VOID QGLApplication::AddNumberEnt(CEntityBase * pTarget, PixelDirection MoveDir, Int LiveBase, Int MoveSpeed,
								  Int GivenDisposalTime, Int GivenWaitTime, BOOL Destory, Int64 ZValue, Int NoOfParticle,
								  Int Num, BYTE AddSymbolIndex, CParticleSystem * pParSys, BOOL JumpTogether, String FileName,
								  Int JumpHeight, BOOL Shake, UInt32 GivenStartDelay, String SymbolFileName)
{
	if (pTarget == NULL || pParSys == NULL)
		return;

	ParticleStructSetting ParSetting;
	//show the hurt point
	ParSetting.Destory = TRUE;
	ParSetting.LiveTimeBase = LiveBase;
	ParSetting.ExtraZValue = ZValue;
	ParSetting.NoOfParticle = NoOfParticle;
	ParSetting.MoveDir = MoveDir;
	ParSetting.MoveSpeed = MoveSpeed;
	ParSetting.GivenDisposalTime = GivenDisposalTime;
	ParSetting.GivenWaitTime = GivenWaitTime;
	ParSetting.StartDelay = GivenStartDelay;
	
	ParSetting.Loop = FALSE;
	//ParSetting.Stop = TRUE;

	ParSetting.SubOffsetY = 0;//-1 * (pTarget->GetBattleIdleRect().bottom - 
		//pTarget->GetBattleIdleRect().top );
	ParSetting.SubOffsetX = 0;

	StlVector<CAnimatedEntity *> NumLst;
	TextEntManager::CreateEntityNumber(NumLst, Num, 
		gGlobal.pD3DDevice, AddSymbolIndex, FileName.c_str(), SymbolFileName.c_str());

	
	for (Int i = 0; i < (Int)NumLst.size(); i++)
	{
		ParSetting.GivenDisposalAnim = NumLst[i]->GetAnimationIndex();
		/*NumLst[i]->SetCharPos(pTarget->GetCharPos());
		NumLst[i]->SetEntPixelPos(pTarget->GetEntPixelPos());
		NumLst[i]->SetEntOffsetX(NumLst[i]->GetEntOffsetX() + pTarget->GetEntOffsetX());
		//add the animation for the numbers
		NumLst[i]->SetEntOffsetY(-1 * (pTarget->GetBattleIdleRect().bottom - 
			pTarget->GetBattleIdleRect().top )  + pTarget->GetEntOffsetY());
			*/

		NumLst[i]->SetEntOffsetY(-1 * (pTarget->GetBattleIdleRect().bottom - 
			pTarget->GetBattleIdleRect().top ));
		
		if (Shake)
		{
			Int ShakeX = gConfigure.uNumShakeX;
			Int ShakeY = gConfigure.uNumShakeY;
			Int ShakeSpeedX = gConfigure.uNumShakeSpeedX;
			Int ShakeSpeedY = gConfigure.uNumShakeSpeedY;
			Int ShakeWidth = gConfigure.uNumShakeWidth;
			Int ShakeHeight = gConfigure.uNumShakeHeight;
			Int ShakeTime = gConfigure.uNumShakeTime;
			NumLst[i]->SetShakeInfo(ShakeX, ShakeY, ShakeSpeedX, ShakeSpeedY, ShakeWidth, ShakeHeight, ShakeTime);
		}
		UInt32 StartDelay = 0;
		if (!JumpTogether && !Shake)
			StartDelay = gConfigure.uNumJumpDelay * i;

		if (!Shake)
		NumLst[i]->SetJumpData(JumpHeight,
			0, 0, GetPrivateProfileInt(_T("BATTLESETTING"), _T("NUMJUMPTIME"), 250, CONFIGURATION_INI_FILE), StartDelay + GivenStartDelay);
			
		pParSys->AddAnimatedEntityToSystem(NumLst[i], ParSetting, pTarget, TRUE);
		SafeDeleteObject(NumLst[i]);
	}
	NumLst.clear();
}

VOID QGLApplication::ChangePlayerColor256()
{
	//Char FileName[50];
	//GetPrivateProfileString(_T("LOADFILE"), _T("Color256"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	gGlobal.g_pCharacter->GetCharMainView()->ChangeColorP(gConfigure.strBody, _T("Data\\d20002_t001.tga"));
}

BOOL QGLApplication::MergeOBJBlockingToMap(CMapObjectView& objview, WORD* outputblock)
{
	int maxwidth = objview.GetMapMaxBlockX();
	int maxheight = objview.GetMapMaxBlockY();
	int pos;
	BlockPos bpos;

	BOOL isblock;
	for( int i = 0; i<maxheight; i++ )
	{
		for( int j = 0; j<maxwidth; j++ )
		{
			bpos.PosX = j;
			bpos.PosY = i;
			pos = i * maxwidth + j;
			isblock = objview.IsBlcok(bpos);
			if( isblock )
				outputblock[pos] = 1;
			else
				outputblock[pos] = 0;
		}
	}
	return TRUE;
}

VOID QGLApplication::InitMiniMap(String filename)
{
	gGlobal.MiniMap = gGlobal.pUIManager->FindUIMiniMap("UseMap");

	if(gGlobal.MiniMap)
	{
		String strTempDir = gConfigure.strWorkingDir.c_str() + filename;
		gGlobal.MiniMap->SetImage(strTempDir.c_str());
		CImage* img = gGlobal.MiniMap->GetImage();
		if (img)
		{
			gGlobal.MiniMap->SetupRatio(m_CurrMapView->GetMapWidth(), m_CurrMapView->GetMapHeight());
			gGlobal.MiniMap->ResetPlayerPos( gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX,
				gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY);


			////////////////////////////////////
			String name = _T("Data\\interface0025_main_J.tga");
			CImage	*npcImage = CImageManager::GetInstance().CreateImage(name.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);

			if (npcImage )
			{
				RECT jumpIcon;
				SetRect(&jumpIcon, 346, 140, 358, 152);
				BinaryClientJumppointInstance *tmpjp;

				for (UInt i = 0; i < gGlobal.m_jplist.size(); ++i)
				{
					tmpjp = gGlobal.m_jplist[i];
					UInt32 pixelX = (UInt32)( tmpjp->m_posx * BlockSizeX + BlockSizeX / 2);
					UInt32 pixelY = (UInt32)( tmpjp->m_posy * BlockSizeY/2 + BlockSizeY / 2);
					gGlobal.MiniMap->DrawIcon(npcImage, jumpIcon, pixelX, pixelY);
				}

				//	CImageManager::GetInstance().DeleteImage(npcImage);

				////////////////////////////////////				
				gGlobal.g_pSearchListProcess->MakeNPCSearchList();

				for (UInt16 i = 0; i < gGlobal.g_pSearchListProcess->GetNPCCount(); ++i)
				{
					NPCInfo npc = gGlobal.g_pSearchListProcess->GetNPCInfo(i);
					if (npc.NPC_type != NPC_INVISIBLE_EMENY)
					{
						UInt32 pixelX = (UInt32)( npc.BlockPosX * BlockSizeX + BlockSizeX / 2);
						UInt32 pixelY = (UInt32)( npc.BlockPosY * BlockSizeY/2 + BlockSizeY / 2);

						RECT npcIcon;
						switch (npc.NPC_type)
						{
						case NPC_NORAML:		SetRect(&npcIcon, 301, 141, 310, 150);					break;
						case NPC_VISIBLE_EMENY_FOR_MANY_USER:	SetRect(&npcIcon, 385, 141, 394, 150);	break;
						case NPC_BUSINESSMAN:	SetRect(&npcIcon, 331, 141, 340, 150);					break;
						case NPC_QUEST:			SetRect(&npcIcon, 316, 141, 325, 150);					break;
						case NPC_VISIBLE_EMENY_FOR_ONE_USER:	SetRect(&npcIcon, 385, 141, 394, 150);	break;
						}
						
						gGlobal.MiniMap->DrawIcon(npcImage, npcIcon, pixelX, pixelY);
					}		
				}
		
				CImageManager::GetInstance().DeleteImage(npcImage);
				npcImage = NULL;
			}
			/////////////////////////////////////////
			/////////////////////////////////////////

			gGlobal.pUIManager->AddDirtyUIItem(gGlobal.MiniMap);
		}		
	}
}

void QGLApplication::InsertDisposalEffect(DisposalEffectData Data)
{
	m_EntityDisposalLst.insert(std::make_pair((UInt)Data.pEnt, Data));
}

VOID QGLApplication::ClearMessage()
{
	UIEditBox * pEditBox = gGlobal.pUIManager->GetUIThing<UIEditBox *>("EditLine");
	if (pEditBox)
	{
		pEditBox->ClearText();
		pEditBox->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
	}
}
BOOL QGLApplication::SendMessageByChannelType(String OrgMsg, BYTE Type, String Command)
{
	//check previous time
	UInt32 curTime = (UInt32)time(NULL);
	if (curTime < gGlobal.g_PreSpeakTime[Type] + SpeakRestTime[Type])
	{
		String text;
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_TOOFREQ")), (gGlobal.g_PreSpeakTime[Type] + SpeakRestTime[Type] - curTime));
		gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
		return FALSE;
	}
	
	String Msg;
	size_t pos = OrgMsg.find(Command, 0);
	if (pos != std::string::npos && pos == 1)
	{
		size_t spacePos = OrgMsg.find(_T(" "), 1);
		if (spacePos != std::string::npos)
		{
			Msg = OrgMsg.substr(spacePos + 1);
			CChannelControl * pChannelCtrl = gGlobal.GetChannelControl(Type);
			if (pChannelCtrl)
			{
				size_t notSpacePos = Msg.find_first_not_of(_T(" "));
				if (notSpacePos != std::string::npos)
				{
					if (gGlobal.g_pCharacter->CheckBlockChat(Type))
					{
						String temp;
						if (gGlobal.g_pCharacter->m_tempBuffData)
							temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_CHAT_BLOCK")),
							gGlobal.g_pCharacter->m_tempBuffData->name);
						else
							temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_CHAT_BLOCK")),
							_T(""));
						gGlobal.PrintMessage(temp.c_str(), CHANNELTYPE_SYSTEM);
					}
					else
					{
						pChannelCtrl->SendChannelMessage(Msg);
						gGlobal.g_PreSpeakTime[Type] = curTime;
					}
				}
			}
			else
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_NOTFOUND")), CHANNELTYPE_SYSTEM, String());
			}

			return TRUE;
		}
		else
			return TRUE;
	}
	return FALSE;
}
void QGLApplication::SendMessage()
{
	PCSTRING Space = _T(" ");

	UIEditBox * pEditBox = gGlobal.pUIManager->GetUIThing<UIEditBox *>("EditLine");
	
	if (pEditBox && !gGlobal.m_bChangeLine)
	{
		UIChatPanel *pChatPanel = gGlobal.pUIManager->FindUIChatPanel("Chat");
		if(pChatPanel)
			pChatPanel->UnLockChatPanel();
		if (m_SubWindow.m_UIManager)
		{
			pChatPanel = m_SubWindow.m_UIManager->FindUIChatPanel("SubChat");
			if(pChatPanel)
				pChatPanel->UnLockChatPanel();
		}
		
		String Temp, TempBase;
		PCSTRINGW TextPtr = pEditBox->GetText();

		pEditBox->AddPastStr();
	
		/*
		gGlobal.g_pastInputMsgHis.push_back(TextPtr);
		if (gGlobal.g_pastInputMsgHis.size() > MaxHis)
			gGlobal.g_pastInputMsgHis.erase(gGlobal.g_pastInputMsgHis.begin());
		gGlobal.g_pastHisIndex = (Int)gGlobal.g_pastInputMsgHis.size();
		*/

		TempBase = TextPtr;

		gGlobal.BlockColorWord(TempBase);

		bool	HideNow = false;
		for(UInt32 i = 0; i < TempBase.size();  ++i)
		{
			WCHAR W = TempBase[i];
			if(TempBase[i] == 20)
				Temp += _T("#:");
			else
			if(TempBase[i] == 19)
				HideNow = !HideNow;
			else
			if((TempBase[i] == 15 || TempBase[i] == 16 || TempBase[i] > 26) && !HideNow)
				Temp += TempBase[i];
		}

		

		if (!gConfigure.uNetwork)
			gGlobal.PrintMessage(Temp, CHANNELTYPE_MAP);
		if (Temp.IsEmpty())
			return;
		
		gGlobal.ModifyMessage(Temp);
		
		pEditBox->ClearText();
		pEditBox->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
		String Name;
		String Msg = Temp;
		
		size_t pos = Temp.find(gConfigure.strMsgDel, 0);

		if (pos == 0)
		{
			size_t cmdPos = gConfigure.strMsgDel.length();

			// cheat code
			if (Temp.find(gConfigure.strCheatCom, cmdPos) == cmdPos) {
				int index = (int)Temp.find(Space, 0);
				if (index >= 0)
				{
					Msg = Temp.substr(index);
					Msg.Trim();
					if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer()) {
						// battle cheat code
						gGlobal.g_pBattleControl->GetControlPlayer()->CheatCode(Msg);
					} else {
						// map cheat code
						gGlobal.g_pCharacter->CheatCode(Msg);
					}
				}
				return;
			}

			for (Int i = 0; i < MAX_CHANNELTYPE ; i++)
			{
				if ( i == CHANNELTYPE_PRIVATE)
					continue;
				if (SendMessageByChannelType(Temp, i, gConfigure.strChannelMsgCom[i]))
				{
					gGlobal.SetCurrChannelType(i);
					return;
				}
			}			//trading message

			//create ChatRoom
			if (Temp.find(gConfigure.strCreateChatRoomCom, cmdPos) == cmdPos)
			{
				size_t spacePos = Temp.find(Space, 0);
				if (spacePos != std::string::npos)
				{
					Name = Temp.substr(spacePos);
					String ChannelName = String(ChannelPrefix[CHANNELTYPE_CHATROOM]) + String(Del) + Name;
					if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
						MessageService_Proxy::JoinMessageChannel(gGlobal.pPeerProxy, gGlobal.g_context,  
						gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id(), gGlobal.g_pCharacter->GetCharNetObject()->GetNickName(),
						ChannelName, CHANNEL_JOIN_MODE_NOT_CREATE_IF_EXIST);
				}
				return;
			}

			//join channel
			if (Temp.find(gConfigure.strJoinChatRoomCom, cmdPos) == cmdPos)
			{
				size_t spacePos = Temp.find(Space, 0);
				if (spacePos != std::string::npos)
				{
					Name = Temp.substr(spacePos);
					String ChannelName = String(ChannelPrefix[CHANNELTYPE_CHATROOM]) + String(Del) + Name;
					if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
						MessageService_Proxy::JoinMessageChannel(gGlobal.pPeerProxy, gGlobal.g_context,  
						gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id(), gGlobal.g_pCharacter->GetCharNetObject()->GetNickName(),
						ChannelName, CHANNEL_JOIN_MODE_JOIN_ONLY_EXIST);
					return;
				}
			}

			//leave chat room
			if (Temp.find(gConfigure.strLeaveChatRoomCom, cmdPos) == cmdPos)
			{
				gGlobal.LeaveChannel(CHANNELTYPE_CHATROOM);	
				return;
			}

			
			//private message
			if (Temp.find(gConfigure.strChannelMsgCom[CHANNELTYPE_PRIVATE], cmdPos) == cmdPos)
			{
				size_t spacePosOne = Temp.find(Space, 0);
				if (spacePosOne != std::string::npos)
				{
					size_t spacePosTwo = Temp.find(Space, spacePosOne + 1);
					if (spacePosTwo != std::string::npos)
					{
						Name = Temp.substr(spacePosOne + 1, spacePosTwo - spacePosOne - 1);
						Msg = Temp.substr(spacePosTwo + 1);
						if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
						{
							size_t notSpacePos = Msg.find_first_not_of(_T(" "));
							if (notSpacePos != std::string::npos)
							{
								gGlobal.SetCurrChannelType(CHANNELTYPE_PRIVATE);
								//gGlobal.AddColorPrefix(Msg);

								if (gGlobal.g_pCharacter->CheckBlockChat(CHANNELTYPE_PRIVATE))
								{
									String temp;
									if (gGlobal.g_pCharacter->m_tempBuffData)
										temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_CHAT_BLOCK")),
										gGlobal.g_pCharacter->m_tempBuffData->name);
									else
										temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_CHAT_BLOCK")),
										_T(""));
									gGlobal.PrintMessage(temp.c_str(), CHANNELTYPE_SYSTEM);
								}
								else
								{
									UInt32 curTime = (UInt32)time(NULL);
									if (curTime >= gGlobal.g_PreSpeakTime[CHANNELTYPE_PRIVATE] + SpeakRestTime[CHANNELTYPE_PRIVATE])
									{
										
									
									MessageService_Proxy::SendPrivateMessage(gGlobal.pPeerProxy, gGlobal.g_context, Msg, 
										gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id(), 0, Name, false);
									}
									else
									{
										String text;
										text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_TOOFREQ")), (gGlobal.g_PreSpeakTime[CHANNELTYPE_PRIVATE] + SpeakRestTime[CHANNELTYPE_PRIVATE] - curTime));
			
										gGlobal.PrintMessage(text.c_str(), CHANNELTYPE_SYSTEM);
									}
									//gGlobal.PrintMessage(Msg, CHANNELTYPE_PRIVATE, gGlobal.g_pCharacter->GetCharName(), Name);

								}
							}
						}
						return;
					}
				}
			}
		}



		CChannelControl * pChannelCtrl = gGlobal.GetCurrChannelControl();
		if (pChannelCtrl)
		{
			size_t notSpacePos = Msg.find_first_not_of(_T(" "));
			if (notSpacePos != std::string::npos)
			{
				if (gGlobal.g_pCharacter->CheckBlockChat(pChannelCtrl->GetChannnelType()))
				{
					String temp;
					if (gGlobal.g_pCharacter->m_tempBuffData)
						temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_CHAT_BLOCK")),
						gGlobal.g_pCharacter->m_tempBuffData->name);
					else
						temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_CHAT_BLOCK")),
						_T(""));
					gGlobal.PrintMessage(temp.c_str(), CHANNELTYPE_SYSTEM);
				}
				else
					pChannelCtrl->SendChannelMessage(Msg);
				
			}
		}
		else if (gGlobal.GetCurrChannelType() == CHANNELTYPE_PRIVATE)
		{
			size_t notSpacePos = Msg.find_first_not_of(_T(" "));
			if (notSpacePos != std::string::npos)
			{
				//gGlobal.AddColorPrefix(Msg);
				if (gGlobal.g_pCharacter->CheckBlockChat(CHANNELTYPE_PRIVATE))
				{
					String temp;
					if (gGlobal.g_pCharacter->m_tempBuffData)
						temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_CHAT_BLOCK")),
						gGlobal.g_pCharacter->m_tempBuffData->name);
					else
						temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_CHAT_BLOCK")),
						_T(""));
					gGlobal.PrintMessage(temp.c_str(), CHANNELTYPE_SYSTEM);
				}
				else
					MessageService_Proxy::SendPrivateMessage(gGlobal.pPeerProxy, gGlobal.g_context, Msg, 
					gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id(), 0, gGlobal.g_RecentPriChatChar, false);
			}
		}
		else if (gGlobal.GetCurrChannelType() == CHANNELTYPE_MAP)
		{
			size_t notSpacePos = Msg.find_first_not_of(_T(" "));
			if (notSpacePos != std::string::npos)
			{
				if (gGlobal.g_pCharacter->CheckBlockChat(CHANNELTYPE_MAP))
				{
					String temp;
					if (gGlobal.g_pCharacter->m_tempBuffData)
						temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_CHAT_BLOCK")),
						gGlobal.g_pCharacter->m_tempBuffData->name);
					else
						temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_CHAT_BLOCK")),
						_T(""));
					gGlobal.PrintMessage(temp.c_str(), CHANNELTYPE_SYSTEM);
				}
				else
				{
					UInt32 curTime = (UInt32)time(NULL);
					if (curTime < gGlobal.g_PreSpeakTime[CHANNELTYPE_MAP] + SpeakRestTime[CHANNELTYPE_MAP])
					{
						String text;
						text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_TOOFREQ")), (gGlobal.g_PreSpeakTime[CHANNELTYPE_MAP] + SpeakRestTime[CHANNELTYPE_MAP] - curTime));
						gGlobal.PrintMessage(text.c_str(), CHANNELTYPE_SYSTEM);
					}
					else
					{
						if (GetMapStateID() == ST_APP_BATTLE && gGlobal.g_pBattleControl->GetControlPlayer() && 
							gGlobal.g_pBattleControl->GetBattleNetObject())
						{
							gGlobal.g_pBattleControl->GetBattleNetObject()->SendBattleMessage(gGlobal.pPeerProxy, gGlobal.g_context,gGlobal.g_pCharacter->GetCharNetObject()->GetNetId(),  Msg);
						}
						else if (gGlobal.g_pCharacter)
						{
							MapService_Proxy::SendMapMessage(gGlobal.pPeerProxy, gGlobal.g_context,gGlobal.g_pCharacter->GetCharNetObject()->GetNetId(), Msg);
						}
						gGlobal.g_PreSpeakTime[CHANNELTYPE_MAP] = curTime;
					}
				}
				
			}
		}
		else if(gGlobal.GetCurrChannelType() == CHANNELTYPE_FRIEND)
		{
			size_t notSpacePos = Msg.find_first_not_of(_T(" "));
			if (notSpacePos != std::string::npos)
			{
				UInt32 curTime = (UInt32)time(NULL);
				if (curTime < gGlobal.g_PreSpeakTime[CHANNELTYPE_FRIEND] + SpeakRestTime[CHANNELTYPE_FRIEND])
				{
					String text;
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_TOOFREQ")), (gGlobal.g_PreSpeakTime[CHANNELTYPE_FRIEND] + SpeakRestTime[CHANNELTYPE_FRIEND] - curTime));
					gGlobal.PrintMessage(text.c_str(), CHANNELTYPE_SYSTEM);
				}
				else if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
				{
					TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("MessageService_Proxy: SendFriendMessage g_pCharacterID =  %d,Msg = %s "),gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id(),Msg.c_str());
					MessageService_Proxy::SendFriendMessage(gGlobal.pPeerProxy, gGlobal.g_context, Msg, 
						gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id());
					//MapService_Proxy::SendMapMessage(gGlobal.pPeerProxy, gGlobal.g_context,gGlobal.g_pCharacter->GetCharNetObject()->GetNetId(), Msg);
				}
				else
				{
					TRACE_WARNDTL(GLOBAL_LOGGER, _F("JoinParty: gGlobal.g_pCharacter not found"));
				}
				gGlobal.g_PreSpeakTime[CHANNELTYPE_FRIEND] = curTime;
			}
		}
	}
}

VOID QGLApplication::SetListenToPrivateMessage(BOOL Listen)
{
	m_ListenToPrivateMessage = Listen;
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
		MessageService_Proxy::SetListen(gGlobal.pPeerProxy, gGlobal.g_context, m_ListenToPrivateMessage, 
		gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id());
}

VOID QGLApplication::SetListenToFriendMessage(BOOL Listen)
{
	m_ListenToFriendMessage = Listen;
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
		MessageService_Proxy::SetListen(gGlobal.pPeerProxy, gGlobal.g_context, m_ListenToFriendMessage, 
		gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id());
}

CCharacterControl * QGLApplication::FindCharCtrlByName(String Name)
{
	UInt32CharCtrlPtrMap::iterator Itr = m_MapCharacterLst.begin();
	while (Itr != m_MapCharacterLst.end())
	{
		if (Itr->second && Itr->second->GetCharName() == Name)
			return Itr->second;
		Itr++;
	}
	return NULL;
}

CCharacterControl * QGLApplication::FindCharCtrlByNameInBattle(String Name, BYTE speaker_type)
{
	for (Int i = 0; i < (Int) gGlobal.g_pBattleControl->m_BattlePlayerLst.size(); i++)
	{
		if (gGlobal.g_pBattleControl->m_BattlePlayerLst[i].pChar->GetCharName() == Name &&
			gGlobal.g_pBattleControl->m_BattlePlayerLst[i].pChar->GetBatActorObject() &&
			gGlobal.g_pBattleControl->m_BattlePlayerLst[i].pChar->GetBatActorObject()->IsPlayer())
		{
			if (gGlobal.g_pBattleControl->m_BattlePlayerLst[i].pChar->GetBatActorObject())
			{
				if (speaker_type == SPEAKER_CHAR && 
					gGlobal.g_pBattleControl->m_BattlePlayerLst[i].pChar->GetBatActorObject()->IsPlayer())
				{
					return gGlobal.g_pBattleControl->m_BattlePlayerLst[i].pChar;
				}
				else if (speaker_type == SPEAKER_MOB && 
					gGlobal.g_pBattleControl->m_BattlePlayerLst[i].pChar->GetBatActorObject()->IsMob())
				{
					return gGlobal.g_pBattleControl->m_BattlePlayerLst[i].pChar;
				}
			}
			
		}
	}
	return NULL;
}

VOID QGLApplication::CloseSubWindow()
{
	gGlobal.g_bOpenSubWindow = false;
	if (m_SubWindow.pRender)
	{
		if (m_SubWindow.m_SubhWnd)
		{
			DestroyWindow(m_SubWindow.m_SubhWnd);
			m_SubWindow.m_SubhWnd=NULL;										
		}
		UnregisterClass(_T("Sub Window"),NULL);

		SafeDeleteObject(m_SubWindow.m_UIManager);
		SafeDeleteObject(m_SubWindow.pRender);
		m_SubWindow.pRender = NULL;

		if (m_SubWindow.BgTextureData.Texture)
		{
			m_SubWindow.BgTextureData.Texture->Release();
			m_SubWindow.BgTextureData.Texture = NULL;
		}

	}
}

VOID QGLApplication::ResetSubWindowPos()
{
	Int cx = GetSystemMetrics(SM_CXSCREEN);
	Int cy = GetSystemMetrics(SM_CYSCREEN);

	RECT ParentWindowRect;
	GetWindowRect(m_Window.GetWindow(), &ParentWindowRect);

	RECT SubWindowRect = m_SubWindow.WindowRect;
	if (m_SubWindow.m_SubhWnd)
		GetWindowRect(m_SubWindow.m_SubhWnd, &SubWindowRect);

	Int LeftUpX = 0;
	Int LeftUpY = 0;

	Int Width = SubWindowRect.right - SubWindowRect.left;
	Int Height = SubWindowRect.bottom - SubWindowRect.top;
	if ( m_SubWindow.m_AlignType == WINDOWALIGN_RIGHT)
	{
		if (gGlobal.g_pSystemSetting)
				gGlobal.g_pSystemSetting->SetSubChatPosition(WINDOWALIGN_RIGHT);
		LeftUpY = ParentWindowRect.top;

		LeftUpX = ParentWindowRect.right;
		if (ParentWindowRect.right + Width > cx)
		{
			if (cx - ParentWindowRect.right < (Int)gConfigure.uSubWndMinWidth)
				Width = gConfigure.uSubWndMinWidth;
			else
				Width = cx - ParentWindowRect.right;
			LeftUpX = cx - Width;
		}
		Height = ParentWindowRect.bottom - ParentWindowRect.top;
		if(Width)
			gGlobal.g_SubWndWidth = Width;
	}
	else if ( m_SubWindow.m_AlignType == WINDOWALIGN_LEFT)
	{
		if (gGlobal.g_pSystemSetting)
				gGlobal.g_pSystemSetting->SetSubChatPosition(WINDOWALIGN_LEFT);
		LeftUpY = ParentWindowRect.top;

		LeftUpX = ParentWindowRect.left - Width;
		if (LeftUpX < 0)
		{

			if (ParentWindowRect.left < (Int)gConfigure.uSubWndMinWidth)
				Width = gConfigure.uSubWndMinWidth;
			else
				Width = ParentWindowRect.left;
			LeftUpX = 0;
		}
		Height = ParentWindowRect.bottom - ParentWindowRect.top;
		if(Width)
			gGlobal.g_SubWndWidth = Width;
	}
	/*else if ( m_SubWindow.m_AlignType == WINDOWALIGN_DOWN)
	{
	LeftUpX = ParentWindowRect.left;

	LeftUpY = ParentWindowRect.bottom;
		if (ParentWindowRect.bottom + Height > cy)
		{
			if (cy - ParentWindowRect.bottom < (Int)gConfigure.uSubWndMinWidth)
				Height = gConfigure.uSubWndMinHeight;
			else
				Height = cy - ParentWindowRect.bottom;
			LeftUpY = cy - Height;
		}
		Width = ParentWindowRect.right - ParentWindowRect.left;
	}
	else if ( m_SubWindow.m_AlignType == WINDOWALIGN_TOP)
	{
		LeftUpX = ParentWindowRect.left;
		LeftUpY = ParentWindowRect.top - Height;
		if (LeftUpY < 0)
		{
			if (ParentWindowRect.top < (Int)gConfigure.uSubWndMinHeight)
				Height = gConfigure.uSubWndMinHeight;
			else
				Height = ParentWindowRect.top;

			LeftUpY = 0;
		}
		Width = ParentWindowRect.right - ParentWindowRect.left;
	}*/

	if (!m_SubWindow.SizeChange)
		m_SubWindow.SizeChange = (m_SubWindow.WindowRect.right - m_SubWindow.WindowRect.left) != Width ||
		(m_SubWindow.WindowRect.bottom - m_SubWindow.WindowRect.top) != Height;

	if (m_SubWindow.m_SubhWnd)
	{
		SetWindowPos( m_SubWindow.m_SubhWnd, HWND_NOTOPMOST,
			LeftUpX, LeftUpY, 
			Width,  
			Height,
			SWP_NOACTIVATE);
		GetWindowRect(m_SubWindow.m_SubhWnd, &m_SubWindow.WindowRect);

	}
	else
	{
		SetRect(&m_SubWindow.WindowRect, LeftUpX, LeftUpY, LeftUpX + Width, LeftUpY + Height);
	}
	
	SetFocus(m_Window.GetWindow());
}
VOID QGLApplication::OpenSubWindow()
{
	if (m_SubWindow.pRender)
		return;

	m_SubWindow.Cusor.InitEntPicInfo();
	m_SubWindow.Cusor.LoadEntityFile(gConfigure.strCursorNormal.c_str(), gGlobal.pD3DDevice);

	RECT WindowRect; 
	GetWindowRect(m_Window.GetWindow(), &WindowRect);
	
	m_SubWindow.WindowRect = WindowRect;
	//hardcode width
	if(gGlobal.g_SubWndWidth < gConfigure.uSubWndMinWidth)
		gGlobal.g_SubWndWidth = gConfigure.uSubWndMinWidth;
	m_SubWindow.WindowRect.right = m_SubWindow.WindowRect.left + gGlobal.g_SubWndWidth;
		//gConfigure.uSubWndMinWidth;
	
	ResetSubWindowPos();
	UInt Width = m_SubWindow.WindowRect.right - m_SubWindow.WindowRect.left;
	UInt Height = m_SubWindow.WindowRect.bottom - m_SubWindow.WindowRect.top;
	//create window first
	WNDCLASS	wc;
	//fullscreen = TRUE;

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= NULL;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, NULL);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow PoInt32er
	wc.hbrBackground	= NULL;									// No Background Required For D3D
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	=  _T("Sub Window");							// Set The Class Name
	wc.lpfnWndProc		= (WNDPROC)(WndProc);


	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,_T("Failed To Register The Window Class."),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		return;											// Return FALSE
	}

	m_SubWindow.m_SubhWnd =CreateWindow(wc.lpszClassName,							// Class Name
			gGlobal.GetStringTable()->Get(_T("MSG_SUB_CHAT_TITLE")),								// Window Title
			WS_OVERLAPPED|WS_CAPTION|WS_THICKFRAME|
			WS_VISIBLE,					// Required Window Style
			m_SubWindow.WindowRect.left, m_SubWindow.WindowRect.top,								// Window Position
			Width,	// Calculate Window Width
			Height,	// Calculate Window Height
			m_Window.GetWindow(),								//Parent Window
			NULL,								// Menu
			(HINSTANCE)NULL,							// Instance
			0L);							// Dont Pass Anything To WM_CREATE


	if (m_SubWindow.m_SubhWnd != NULL)
	{
		m_SubWindow.m_UIManager = SafeCreateObject(UIManager);

		m_SubWindow.m_UIManager->SetupLua(gGlobal.pUIManager->GetLuaState(), true);

		m_SubWindow.SizeChange = TRUE;
		ResetSubWindowPos();
		RestoreSubWindowData();
		ResizeSubWindow();

		m_SubWindow.pRender->SetSubWindow(TRUE);
	}
}

VOID QGLApplication::DrawSubWindow()
{
	if (m_SubWindow.pRender && m_SubWindow.pRender->IsDeviceValide())
	{
		m_SubWindow.pRender->GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
			D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0 );

		m_SubWindow.pRender->BeginScene();
		m_SubWindow.pRender->BeginSprite();

		//m_SubWindow.pRender->UpDateTextureToWindowBuffer();

		m_SubWindow.m_UIManager->Update(GetTickCount());

		m_SubWindow.Cusor.UpDate(GetTickCount());
		
		//just call render all for temp
		
		//m_SubWindow.Cusor.SetEntOffsetX(m_SubWindow.m_UIManager->GetCursorX());
		//m_SubWindow.Cusor.SetEntOffsetY(m_SubWindow.m_UIManager->GetCursorY());
		/*else
		{
			StlMap<CEntityBase *, RECT>::iterator PreDirtyRectLst = m_SubWindow.BgTextureData.m_PreDirtyRegionLst.find(&m_SubWindow.Cusor);
			if (PreDirtyRectLst != m_SubWindow.BgTextureData.m_PreDirtyRegionLst.end())
				m_SubWindow.m_UIManager->GetDirtyRectMan()->AddDirtyRectToMan(PreDirtyRectLst->second);
		}
		*/
		

		//StlVector<RECT> DirRectLst = m_SubWindow.m_UIManager->GetDirtyRectLst();
		//for (size_t i = 0; i < DirRectLst.size(); i++)
		//	m_SubWindow.m_UIManager->RenderRect(DirRectLst[i]);

		
		//m_SubWindow.m_UIManager->ClearDirtyRectLst();
		m_SubWindow.pRender->DrawTextureBySprite(m_SubWindow.BgTextureData.Texture);

		m_SubWindow.m_UIManager->RenderAllPanelsByRects();
		m_SubWindow.m_UIManager->BlitAll(m_SubWindow.pRender->GetSprite());

		
		
		if (m_SubWindow.ShowCursor)
		{
			RECT NewDirtyRect;
			if (m_SubWindow.pRender->IsEntityInsideTexture(&m_SubWindow.Cusor, m_SubWindow.CusorTextureData, PixelPos(0, 0), &NewDirtyRect))
			{
				if (m_SubWindow.Cusor.m_Updated)
				{
					/*StlMap<CEntityBase *, RECT>::iterator PreDirtyRectLst = m_SubWindow.BgTextureData.m_PreDirtyRegionLst.find(&m_SubWindow.Cusor);
					if (PreDirtyRectLst != m_SubWindow.BgTextureData.m_PreDirtyRegionLst.end())
						m_SubWindow.m_UIManager->GetDirtyRectMan()->AddDirtyRectToMan(PreDirtyRectLst->second);
					m_SubWindow.m_UIManager->GetDirtyRectMan()->AddDirtyRectToMan(NewDirtyRect);
					*/
					m_SubWindow.pRender->ClearTexture(m_SubWindow.CusorTextureData.Texture, NULL); 
					m_SubWindow.pRender->DrawEntityToTexture(&m_SubWindow.Cusor, m_SubWindow.CusorTextureData, NULL);
					m_SubWindow.CusorTextureData.Texture->AddDirtyRect(NULL);
				}
			}
			/*else
			{
				StlMap<CEntityBase *, RECT>::iterator PreDirtyRectLst = m_SubWindow.BgTextureData.m_PreDirtyRegionLst.find(&m_SubWindow.Cusor);
				if (PreDirtyRectLst != m_SubWindow.BgTextureData.m_PreDirtyRegionLst.end())
					m_SubWindow.m_UIManager->GetDirtyRectMan()->AddDirtyRectToMan(PreDirtyRectLst->second);
			}
			*/
			m_SubWindow.pRender->DrawTextureBySprite(m_SubWindow.m_UIManager->GetCursorX(), m_SubWindow.m_UIManager->GetCursorY(),
			m_SubWindow.CusorTextureData.Texture	);
		}

		m_SubWindow.BgTextureData.m_PreDirtyRegionLst = m_SubWindow.BgTextureData.m_NextDirtyRegionLst;
		m_SubWindow.BgTextureData.m_NextDirtyRegionLst.clear();

		m_SubWindow.CusorTextureData.m_PreDirtyRegionLst = m_SubWindow.CusorTextureData.m_NextDirtyRegionLst;
		m_SubWindow.CusorTextureData.m_NextDirtyRegionLst.clear();
		
		m_SubWindow.m_UIManager->RemoveAllPanelsRects();
		m_SubWindow.pRender->EndSprite();
		m_SubWindow.pRender->EndScene();
	}
}

VOID QGLApplication::ResizeSubWindow()
{
	if (m_SubWindow.m_SubhWnd)
	{
		RECT ParentWindowRect;
		GetWindowRect(m_Window.GetWindow(), &ParentWindowRect);
		RECT SubWindowRect;
		GetWindowRect(m_SubWindow.m_SubhWnd, &SubWindowRect);

		RECT ResultRect;
		ResultRect = SubWindowRect;
		if (m_SubWindow.m_AlignType == WINDOWALIGN_LEFT ||
			m_SubWindow.m_AlignType == WINDOWALIGN_RIGHT)
		{
			
			if (ResultRect.right - ResultRect.left < (LONG)gConfigure.uSubWndMinWidth)
				ResultRect.right = ResultRect.left + (LONG)gConfigure.uSubWndMinWidth;
			ResultRect.bottom = ResultRect.top + (ParentWindowRect.bottom - ParentWindowRect.top);
		}
		/*else
		{
			if (ResultRect.bottom - ResultRect.top < (LONG)gConfigure.uSubWndMinHeight)
				ResultRect.bottom = ResultRect.top + (LONG)gConfigure.uSubWndMinHeight;
			ResultRect.right = ResultRect.left + (ParentWindowRect.right - ParentWindowRect.left);
		}*/

		//if (QGLMath::IsTwoRectEqual(ResultRect, m_SubWindow.WindowRect))
		//	return;
		if (!m_SubWindow.SizeChange)
			m_SubWindow.SizeChange = (m_SubWindow.WindowRect.right - m_SubWindow.WindowRect.left) != 
			(ResultRect.right - ResultRect.left)  ||
			(m_SubWindow.WindowRect.bottom - m_SubWindow.WindowRect.top) != (ResultRect.bottom - ResultRect.top);

		SetWindowPos( m_SubWindow.m_SubhWnd, HWND_NOTOPMOST,
			ResultRect.left, ResultRect.top, 
		(ResultRect.right -ResultRect.left),  
		(ResultRect.bottom - ResultRect.top),
		SWP_SHOWWINDOW);

		m_SubWindow.WindowRect = ResultRect;

		RECT	CRect;
		GetClientRect(m_SubWindow.m_SubhWnd, &CRect);

		UInt RenderWidth = CRect.right - CRect.left;
		UInt RenderHeight = CRect.bottom - CRect.top;
	}
}

BOOL QGLApplication::CheckSubWindowAlign()
{
	if (m_SubWindow.m_SubhWnd)
	{
		BYTE Org = m_SubWindow.m_AlignType;
		POINT MousePoint;
		::GetCursorPos(&MousePoint);
		RECT SubWinRect;
		GetWindowRect(m_SubWindow.m_SubhWnd, &SubWinRect);
		RECT ParentWindowRect;
		GetWindowRect(m_Window.GetWindow(), &ParentWindowRect);
		
		/*if (MousePoint.x > ParentWindowRect.left &&
			MousePoint.x < ParentWindowRect.right)
		{
			if (MousePoint.y < ParentWindowRect.top)
				m_SubWindow.m_AlignType = WINDOWALIGN_TOP;
			else if (MousePoint.y > ParentWindowRect.bottom)
				m_SubWindow.m_AlignType = WINDOWALIGN_DOWN;
		}
		else 
		*/if (MousePoint.x < ParentWindowRect.left)
		{
			m_SubWindow.m_AlignType = WINDOWALIGN_LEFT;
			if (gGlobal.g_pSystemSetting)
				gGlobal.g_pSystemSetting->SetSubChatPosition(WINDOWALIGN_LEFT);
		}
		else if (MousePoint.x > ParentWindowRect.right)
		{
			m_SubWindow.m_AlignType = WINDOWALIGN_RIGHT;
			if (gGlobal.g_pSystemSetting)
				gGlobal.g_pSystemSetting->SetSubChatPosition(WINDOWALIGN_RIGHT);
		}
		
		return Org != m_SubWindow.m_AlignType;
	}
	
	return FALSE;
}

VOID QGLApplication::RestoreSubWindowData()
{
	if (m_SubWindow.m_SubhWnd == NULL || !m_SubWindow.SizeChange)
		return;
	RECT	CRect;
	GetClientRect(m_SubWindow.m_SubhWnd, &CRect);


	UInt RenderWidth = CRect.right - CRect.left;
	UInt RenderHeight = CRect.bottom - CRect.top;

	if (m_SubWindow.pRender)
		SafeDeleteObject(m_SubWindow.pRender);

	m_SubWindow.pRender = SafeCreateObject6(CRender, (HINSTANCE)NULL,m_SubWindow.m_SubhWnd, 
		RenderWidth, RenderHeight, FALSE, FALSE);

	if (m_SubWindow.BgTextureData.Texture)
		m_SubWindow.BgTextureData.Texture->Release();

	UInt tempHeight = 2;
	while (true)
	{
		if (tempHeight >= RenderHeight + 10)
			break;
		tempHeight *= 2;
	}

	Int tempWidth = 2;
	while (true)
	{
		if (tempWidth >= RenderWidth + 10)
			break;
		tempWidth *= 2;
	}
	D3DXCreateTexture(m_SubWindow.pRender->GetDevice(), tempWidth//GetRenderClass()->GetWinWidth()
		, tempHeight, //GetRenderClass()->GetWinHeight(), 
		0, 0 , D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED, &m_SubWindow.BgTextureData.Texture);

	SafeRelease(m_SubWindow.CusorTextureData.Texture);

	D3DXCreateTexture(m_SubWindow.pRender->GetDevice(), 128//GetRenderClass()->GetWinWidth()
		, 128, //GetRenderClass()->GetWinHeight(), 
		0, 0 , D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED, &m_SubWindow.CusorTextureData.Texture);
	SetRect(&m_SubWindow.CusorTextureData.m_CurrDisArea, 0, 0, 100, 100);

	if (m_SubWindow.BgTextureData.Texture == NULL || m_SubWindow.CusorTextureData.Texture == NULL)
	{
		MessageBox(NULL,_T("Not Enough System Memory"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		CloseSubWindow();
		return;
	}

	m_SubWindow.m_UIManager->SetRender(m_SubWindow.pRender);
	m_SubWindow.m_UIManager->SetOutputTexture(m_SubWindow.BgTextureData.Texture);

	UIPanel * pPanel = m_SubWindow.m_UIManager->FindUIPanel("SubPanel");
	if (pPanel == NULL)
	{
		XML_UI::GetInstance().Init();
		XML_UI::GetInstance().ReadFormFile("Data\\SubChat.xml", m_SubWindow.m_UIManager);

		pPanel = m_SubWindow.m_UIManager->FindUIPanel("SubPanel");
	
		if (pPanel == NULL)
			return;
		//m_SubWindow.m_UIManager->LoadLuaUI("Data\\LuaUI\\XSubChat.Lua");
	}

	pPanel->SetTextureWH(RenderWidth + 10, RenderHeight + 10);
	pPanel->SetTexture(m_SubWindow.pRender->GetDevice());
	pPanel->SetDetectRect(0, 0, RenderWidth, RenderHeight);

	//adjust the position of the scroll bar and chat panel width
	/*UILinerImage * pPetMPBarLimUp = m_SubWindow.m_UIManager->FindUILinerImage("PetMPBarLimUp");
	if (pPetMPBarLimUp)
	{
		pPetMPBarLimUp->SetLinerWidth(RenderWidth);
	}

	UILinerImage * pPetMPBarLimDown = m_SubWindow.m_UIManager->FindUILinerImage("PetMPBarLimDown");
	if (pPetMPBarLimDown)
	{
		pPetMPBarLimDown->SetLinerWidth(RenderWidth);
		pPetMPBarLimDown->SetOffsetXY(0, RenderHeight - 34);
		pPetMPBarLimDown->SetOffsetPos(0, 0);
	}*/
	
	UIImage * pVerBarLim = m_SubWindow.m_UIManager->FindUIImage("VerBarLim");
	if (pVerBarLim)
	{
		pVerBarLim->SetOffsetXY(RenderWidth - pVerBarLim->GetWidth(), 0);
		pVerBarLim->SetOffsetPos(0, 0);
	}

	UIScrollBarV * pScroll = m_SubWindow.m_UIManager->FindUIScrollBarV("SubChatScrollButton");

	if (pScroll)
	{
		pScroll->SetOffsetXY(RenderWidth - pScroll->GetWidth(), pScroll->GetOffsetY());
		//pScroll->SetBarHeight(RenderHeight - 70);
		//pScroll->SetEnable(TRUE);

	}
	
	UIButton * pSubChatEndButton = m_SubWindow.m_UIManager->FindUIButton("SubChatEnableScroll");
	if (pSubChatEndButton)
	{
		for (Int i = _ButtonNormal; i < _MaxButton_State; i++)
			pSubChatEndButton->SetSubOffSet(i, RenderWidth - pSubChatEndButton->GetWidth() , pSubChatEndButton->GetOffsetY());
		pSubChatEndButton->SetOffsetXY(RenderWidth - pSubChatEndButton->GetWidth() , pSubChatEndButton->GetOffsetY());
		pSubChatEndButton->SetOffsetPos(0, 0);
	}
	
	
	UIChatPanel * pChatPanel = m_SubWindow.m_UIManager->FindUIChatPanel("SubChat");
	if (pChatPanel)
	{
		
		pChatPanel->SetImageWH(RenderWidth - 35, RenderHeight - 75);
		//pChatPanel->EndRow();
		PCSTRING MsgHistory = ChatPool::GetInstance().GetStartMsg();
		UInt	CopyIndex = ChatPool::GetInstance().GetCopyIndex();
		pChatPanel->ClearAllRow();

		///Ming Change
		while (MsgHistory)
		{
			if(ChatPool::GetInstance().GetIndex() >= CopyIndex)
				pChatPanel->AddStringW(MsgHistory);
			MsgHistory  = ChatPool::GetInstance().GetNextMsg();
		}
		pChatPanel->EndRow();

		if (pScroll)
		{
			if (pChatPanel->GetAllRowHeight() > pChatPanel->GetHeight())
			{
				pScroll->SetNumCount(pChatPanel->RowListCount());
			}
			else
				pScroll->SetNumCount(0);
		}
	}

	PSTRINGA DownButtonList[7] = {"BlankButton", "SubWindow", "HideChat", "MaskChat", "MsgRecord", 
		 "BlankButton2", "ClearMessage"};
	for (Int i = 0; i < 7 ; i++)
	{
		UIButton * pButton = m_SubWindow.m_UIManager->FindUIButton(DownButtonList[i]);
		if (pButton)
		{
			for (Int i = _ButtonNormal; i < _MaxButton_State; i++)
				pButton->SetSubOffSet(i, pButton->GetOffsetX(), RenderHeight - 30);
			pButton->SetOffsetXY(pButton->GetOffsetX(), RenderHeight - 30);
			pButton->SetOffsetPos(0, 0);
		}
	}
	
	m_SubWindow.pRender->GetSystemCombinedData()->m_DirRectMan.SetBoundRect(0, 0, RenderWidth, RenderHeight);
	m_SubWindow.m_UIManager->GetDirtyRectMan()->SetBoundRect(0, 0, RenderWidth, RenderHeight);
	m_SubWindow.m_UIManager->RenderAll();
	DrawSubWindow();
	m_SubWindow.SizeChange = FALSE;
}
LRESULT CALLBACK WndProc(  HWND  hWnd, 
              UInt  uMsg, 
              WPARAM  wParam, 
              LPARAM  lParam )
{
	static UInt32 Width = 0;
	static UInt32 Height = 0;
	static BOOL	bSubTracking = FALSE;

	SubWindowStr *SubWindow = QGLApplication::GetInstance().GetSubWindow();

	//TRACE_INFODTL_3(GLOBAL_LOGGER, _F("uMsg:%d,wParam:%d,lParam:%d"),uMsg,wParam,lParam);

	if( uMsg  == WM_MOUSEMOVE)
	{
		if (!bSubTracking)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = SubWindow->m_SubhWnd;
			tme.dwFlags = TME_LEAVE|TME_HOVER;
			tme.dwHoverTime = 1;
			bSubTracking = _TrackMouseEvent(&tme);
		}

		if(SubWindow->m_UIManager)
		{
			SubWindow->m_UIManager->SetCursorX((Int16)(LOWORD(lParam)));
			SubWindow->m_UIManager->SetCursorY((Int16)(HIWORD(lParam)));
			SubWindow->m_UIManager->SetMouseMoveFlag(true);
		}
	}
	if(uMsg  == WM_ACTIVATE || uMsg  == WM_NCACTIVATE)             // Watch For Window Activate Message
	{
		PostMessage(QGLApplication::GetInstance().GetWindow().GetWindow(), WM_SYSCOMMAND, (WPARAM)SC_RESTORE, 0);
	}

	if(SubWindow->m_UIManager&& gGlobal.WindowShow)
	{
		if(SubWindow->m_UIManager->MsgProc(SubWindow->m_SubhWnd, uMsg, (WPARAM)wParam, (LPARAM)lParam) == 0)
		{
			if(uMsg == WM_LBUTTONDOWN)
			{
//				gGlobal.ClickSnd->Play();
			}
			return BOOLEAN_TRUE;
		}
	}

	switch( uMsg )
	{
		case WM_NCMOUSEMOVE:
		case WM_NCHITTEST:
			{
				LRESULT  LT = ::DefWindowProc(SubWindow->m_SubhWnd, uMsg, (WPARAM)wParam, (LPARAM)lParam);
				DWORD DT = (DWORD)LT;
				if(DT == HTCLIENT)
				{
					while (gGlobal.WinCursorCount > -1)
						gGlobal.WinCursorCount = ShowCursor(FALSE);
					SubWindow->ShowCursor = true;
					gGlobal.ShowCursor = false;
					if(SubWindow->m_UIManager->GetDragNow())
					{
						SubWindow->m_UIManager->GetDragPanel()->SetEnable(true);
						SubWindow->m_UIManager->AddDirtyUIItem(SubWindow->m_UIManager->GetDragPanel());
					}
				}
				else
				{
					while (gGlobal.WinCursorCount < 0)
						gGlobal.WinCursorCount = ShowCursor(TRUE);
					SubWindow->ShowCursor = false;
					if(SubWindow->m_UIManager->GetDragNow())
					{
						SubWindow->m_UIManager->GetDragPanel()->SetEnable(false);
						SubWindow->m_UIManager->AddDirtyUIItem(SubWindow->m_UIManager->GetDragPanel());
					}
				}
				break;
			}

		case WM_MOUSEHOVER:
			while (gGlobal.WinCursorCount > -1)
				gGlobal.WinCursorCount = ShowCursor(FALSE);
			SubWindow->ShowCursor = true;
			gGlobal.ShowCursor = false;
			if(SubWindow->m_UIManager->GetDragNow())
			{
				SubWindow->m_UIManager->GetDragPanel()->SetEnable(true);
				SubWindow->m_UIManager->AddDirtyUIItem(SubWindow->m_UIManager->GetDragPanel());
			}
			break;

		case WM_MOUSELEAVE:
			while (gGlobal.WinCursorCount < 0)
				gGlobal.WinCursorCount = ShowCursor(TRUE);
			//SubWindow->ShowCursor = false;
			//SubWindow->m_UIManager->ClearDrag();
			SubWindow->m_UIManager->MouseLeave();
			if(SubWindow->m_UIManager->GetDragNow())
			{
				SubWindow->m_UIManager->GetDragPanel()->SetEnable(false);
				SubWindow->m_UIManager->AddDirtyUIItem(SubWindow->m_UIManager->GetDragPanel());
			}

			bSubTracking = FALSE;
		break;
	case WM_GETMINMAXINFO:
		{
			MINMAXINFO* pInfo = (MINMAXINFO*)lParam;

			RECT rect;
			SetRect(&rect, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			AdjustWindowRect(&rect, GetWindowStyle(hWnd), FALSE);

			pInfo->ptMaxSize.x = rect.right-rect.left;
			pInfo->ptMaxSize.y = rect.bottom-rect.top;

			pInfo->ptMaxTrackSize.x = rect.right-rect.left;
			pInfo->ptMaxTrackSize.y = rect.bottom-rect.top;
		}
		break;


	case WM_ACTIVATE:              // Watch For Window Activate Message
		{
#pragma omp parallel for schedule (dynamic)
			switch((WPARAM)wParam)
			{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				//::GetOggPlayer()->Resume();
				::GetFSoundManager()->ResumeMusic();
				::GetFSoundManager()->ResumeAll();
				break;
			case WA_INACTIVE:
				//::GetOggPlayer()->Pause();
				::GetFSoundManager()->PauseMusic();
				::GetFSoundManager()->PauseAll();
				break;
			}
			return BOOLEAN_TRUE;                // Return To The Message Loop
		}
	case WM_SIZE:
		{
			QGLApplication::GetInstance().ResizeSubWindow();
			break;
		}
	case WM_EXITSIZEMOVE:
	{
		QGLApplication::GetInstance().CheckSubWindowAlign();
		QGLApplication::GetInstance().ResetSubWindowPos();
		QGLApplication::GetInstance().RestoreSubWindowData();
		return 0;
	}
	case WM_MOVE:
		{
			QGLApplication::GetInstance().CheckSubWindowAlign();
			QGLApplication::GetInstance().ResetSubWindowPos();
			break;
		}
		
    case WM_CLOSE:                // Did We Receive A Close Message?
    {
	  QGLApplication::GetInstance().CloseSubWindow();
      return 0;                // Jump Back
	}
	case WM_SYSCHAR:
	case WM_CHAR:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	
	{
		LRESULT	lResult = 0;
		QGLApplication::GetInstance().GetWindow().
			ProcessWindowMessage(uMsg, (VOID*)wParam, (VOID*)lParam, (VOID*&)lResult);
			/*
		HWND	hWnd = QGLApplication::GetInstance().GetWindow().GetWindow();
		gGlobal.pUIManager->MsgProc(hWnd, uMsg, wParam, lParam);
		*/
		break;
	}
	break;
	case WM_DESTROY:
		{
			QGLApplication::GetInstance().CloseSubWindow();
			break;
		}
  }

  // Pass All Unhandled Messages To DefWindowProc
  return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

VOID QGLApplication::CreateParty()
{
	if (gGlobal.g_pCharacter->GetCharNetObject())
		gGlobal.g_pCharacter->GetCharNetObject()->CreateParty(gGlobal.pPeerProxy, RPCContext());
}

void QGLApplication::JoinParty(UInt charid)
{
	if( charid==0 )
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("JoinParty: char id 0"));
		return;
	}

	CCharacterControl* pchar = FindCharCtrlByChar_id(charid);
	if( !pchar )
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("JoinParty: ERR char id %d not found"), charid);
		return;
	}

	RPCNetID netid = pchar->GetCharNetObject()->GetNetId();

	if (gGlobal.g_pCharacter->GetCharNetObject())
		gGlobal.g_pCharacter->GetCharNetObject()->PartyJoinRequest(gGlobal.pPeerProxy, gGlobal.g_context, netid);

	
}

void QGLApplication::InviteParty(UInt charid)
{
	if( charid==0 )
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("InviteParty: char id 0"));
		return;
	}

	CCharacterControl* pchar = FindCharCtrlByChar_id(charid);
	if( !pchar )
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("InviteParty: ERR char id %d not found"), charid);
		return;
	}

	RPCNetID netid = pchar->GetCharNetObject()->GetNetId();

	if (gGlobal.g_pCharacter->GetCharNetObject())
		gGlobal.g_pCharacter->GetCharNetObject()->PartyInviteRequest(gGlobal.pPeerProxy, gGlobal.g_context, netid);

	
}

CCharacterControl * QGLApplication::FindCharCtrlByChar_id(UInt32 Char_id)
{
	for (UInt32CharCtrlPtrMap::iterator Itr = m_MapCharacterLst.begin(); 
		Itr != m_MapCharacterLst.end(); Itr++)
	{
		if (Itr->second && Itr->second->GetMapActorID() == Char_id)
			return Itr->second;
	}
	return NULL;
}

void QGLApplication::DisplayPartyInfo()
{
	if ((NULL == gGlobal.g_pParty) && (NULL == gGlobal.g_pTempLeaveParty))
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("DisplayPartyInfo: Player has no party"));
		return;
	}
	
	if (NULL == gGlobal.g_pParty) 
		gGlobal.g_pParty->ShowMember();
	else
		gGlobal.g_pTempLeaveParty->ShowMember();
}

VOID QGLApplication::LookAtMousePos(Int x, Int y)
{
	Int worldX = x + (Int)m_CurrCam.GetLeftUpPos().x;
	Int worldY = y + (Int)m_CurrCam.GetLeftUpPos().y;
	BlockPos Pos;
	QGLMath::FindBlockByPixel(worldX, worldY, Pos, GetRenderMap()->GetMapWidth(), GetRenderMap()->GetMapHeight(),
		GetRenderMap()->GetMapMaxBlockX(),GetRenderMap()->GetMapMaxBlockY() );
	Int direction = QGLMath::GetRotationFromTwoPoint(gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX,
		gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY, Pos.PosX,
		Pos.PosY);
	gGlobal.g_pCharacter->GetCharMainView()->SetRotation(direction);
}
VOID QGLApplication::UpdateRightClick(LONG x, LONG y)
{
	if (gGlobal.g_CursorState == NULL)
		return;

	if (gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_PARTY ||
		gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_PROTECT ||
		gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_SKILL ||
		gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_PK_ATTACK || 
		gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_CATCH ||
		gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_GIVEN ||
		gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_TRADING  ||
		gGlobal.g_CursorState->GetStateID() == ST_APP_FRIEND ||
		gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_GENERATION) 
	{
		if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_SKILL &&
			gGlobal.g_pSkillProcess->m_pTempSkillData &&
			gGlobal.g_pSkillProcess->m_pTempSkillData->Skill_ThrowItemType != 255 &&
			gGlobal.g_pBattleControl->GetBattleItemUseIndex() == -1)
			return;

		if ( (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_SKILL || gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_CATCH || 
			gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_PROTECT)	&& 
			gGlobal.g_pCharacter->GetAction() != CCharacter::ST_MONITOR && !gGlobal.g_pBattleControl->IsPlayingAnim())
		{
			if (IsBattlePartnerTurn())
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePartnerPanel", String());
			else
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowBattlePanel", String());
		}
		gGlobal.g_CursorState->Unblock();
		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));

		FSound* sfx = NULL;
		String Temp;
		sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
		if (sfx)
		{
			sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
			sfx->Play();
		}
		
	}
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_USEITEM)
		UpdateBattleUseItemClick(x, y, BOOLEAN_TRUE);
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_SHOP_SELL)
	{
		if ( gGlobal.g_pShopProcess )
			gGlobal.g_pShopProcess->OnRButtonUpForCancelSell();
	}
	else if (gGlobal.g_CursorState->GetStateID() == ST_APP_BIGMAP_DESC)
	{
		gGlobal.g_pBigMap->StartBigMapDescMode(false);
	}
	else if ((gGlobal.g_CursorState->GetStateID() == ST_APP_ASSET_LOCK) || (gGlobal.g_CursorState->GetStateID() == ST_APP_ASSET_UNLOCK))
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ResetAllLockStatus", String());
	}
	

	if (m_SelectedMapControl)
	{
		
		CCharacterControl * pCharCtrl = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
		if (pCharCtrl && m_SelectedMapControl != gGlobal.g_pCharacter)
		{
			gGlobal.g_MenuTargetProcess->ShowTargetDetail(pCharCtrl);				
		}
		else
		{
			LookAtMousePos(x, y);
			gGlobal.g_MenuTargetProcess->ShowTargetDetail(NULL);
		}
	}
	else
	{
		LookAtMousePos(x, y);
		gGlobal.g_MenuTargetProcess->ShowTargetDetail(NULL);
	}

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OnRightClick", String());
}


CMapActorControl * QGLApplication::FindMapActorCtrlByNetID(RPCNetID ID)
{
	for (UInt32CharCtrlPtrMap::iterator Itr = m_MapCharacterLst.begin(); 
		Itr != m_MapCharacterLst.end(); Itr++)
	{
		if (Itr->second && Itr->second->GetMapActorNetID() == ID)
			return Itr->second;
	}

	for (StlMap<UInt,  CMapNPCControl *>::iterator Itr = m_CurrMapView->m_MapNPCCtrlLst.begin(); 
		Itr != m_CurrMapView->m_MapNPCCtrlLst.end(); Itr++)
	{
		if (Itr->second && Itr->second->GetMapActorNetID() == ID)
			return Itr->second;
	}

	return NULL;
}

CMapNPCControl * QGLApplication::FindNPCCtrlByID(UInt32 NPCID)
{
	for (StlMap<UInt,  CMapNPCControl *>::iterator Itr = m_CurrMapView->m_MapNPCCtrlLst.begin(); 
		Itr != m_CurrMapView->m_MapNPCCtrlLst.end(); Itr++)
	{
		if (Itr->second && Itr->second->GetMapActorID() == NPCID)
			return Itr->second;
	}
	return NULL;
}
CMapNPCControl* QGLApplication::FindNPCCtrlByBaseID(UInt32 uNpcId)
{
	for (StlMap<UInt,  CMapNPCControl *>::iterator Itr = m_CurrMapView->m_MapNPCCtrlLst.begin(); 
		Itr != m_CurrMapView->m_MapNPCCtrlLst.end(); Itr++)
	{
		if (NULL != Itr->second )
		{
			NPCDB_Client* pNpcClt = Itr->second->GetNPCData();
			if ( NULL != pNpcClt) 
			{
				if (uNpcId == pNpcClt->m_basicinfo->m_npc_id) 
					return Itr->second;
			}
		}
	}
	return NULL;
}
CMapNPCControl * QGLApplication::FindNPCCtrlByNPCTemplateID(UInt32 NPCTemplateID)
{
	for (StlMap<UInt,  CMapNPCControl *>::iterator Itr = m_CurrMapView->m_MapNPCCtrlLst.begin(); 
		Itr != m_CurrMapView->m_MapNPCCtrlLst.end(); Itr++)
	{
		if (Itr->second && Itr->second->GetNPCData()->m_basicinfo->m_npc_id == NPCTemplateID)
			return Itr->second;
	}
	return NULL;
}
void OpenSubWindow()
{
	gGlobal.g_bOpenSubWindow = true;
	QGLApplication::GetInstance().OpenSubWindow();
}

CCharacterControl * QGLApplication::FindChatCtrlByName(String Name)
{
	for (UInt32CharCtrlPtrMap::iterator Itr = m_MapCharacterLst.begin();
		Itr != m_MapCharacterLst.end(); ++Itr)
	{
		if (Itr->second && Itr->second->GetCharName() == Name)
			return Itr->second;
	}
	return NULL;
}
VOID QGLApplication::AddTestImage()
{
	/*
	Char Message[512];
	GetPrivateProfileString(_T("LOADFILE"), _T("MESSAGE"), _T(""), Message, 512, CONFIGURATION_INI_FILE);
	gGlobal.TempMsgForUI = Message;
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenPlayerMessage", String());
	//return;
	CUIEntity * pImEnt = SafeCreateObject(CUIEntity);

	pImEnt->ReadXMLData("Data\\ChatBalloon.xml");

	UIChatPanel * pChatPanel = pImEnt->GetUIManager()->FindUIChatPanel("ChatBalloonContext");
	if (pChatPanel)
	{
		Char Message[512];
		GetPrivateProfileString(_T("LOADFILE"), _T("MESSAGE"), _T(""), Message, 512, CONFIGURATION_INI_FILE);
		pChatPanel->AddStringW(Message);
		pChatPanel->Start2BottomRow();
		if (pChatPanel->GetEndRowIndex() != pChatPanel->RowListCount() - 1)
		{
			UIChatRow * pChatRow = pChatPanel->GetRow(pChatPanel->GetEndRowIndex());
			pChatRow->ResetChatItemsPos();
			if (pChatRow)
			{
				//remove last two chat item and insert two . .

				UIChatItem * pChatItem = pChatRow->GetChatItem((UInt32)pChatRow->GetRowItemList()->size() - 1);
				if (pChatItem)
				{
					UIChatAniIcon * pChatAniIcon = dynamic_cast<UIChatAniIcon *>(pChatItem);
					if (pChatAniIcon)
					{
						Char FileName[50];
						GetPrivateProfileString(_T("LOADFILE"), _T("MESSAGEOVER"), _T(".."), FileName, 50, CONFIGURATION_INI_FILE);
						pChatRow->AddStringW(FileName, pChatPanel->GetFontColor(), pChatPanel->GetShadowColor(), pChatPanel->GetFontStyle(), 
							pChatPanel->GetFont()->GetHeight());

						StringW WStr;
						UInt32 Len;
						pChatPanel->GetFont()->DrawFontStrC(FileName, WStr, pChatItem->GetPosX(), 0, 
							pChatPanel->GetWidth(), pChatPanel->GetPitch(), pChatPanel->GetFontStyle(), Len);

						pChatRow->GetRowItemList()->pop_back();
						UIChatString *MyStr = SafeCreateObject(UIChatString);
						MyStr->SetString(WStr.c_str());
						MyStr->SetFontColor(pChatPanel->GetFontColor());
						MyStr->SetShadowColor(pChatPanel->GetShadowColor());
						MyStr->SetFontStyle(pChatPanel->GetFontHeight());
						MyStr->SetWidth(pChatPanel->GetFont()->GetLastDrawWidth() - pChatItem->GetPosX());
						
						UInt32	PitchHeight, RealFontHeight;
						pChatPanel->GetFont()->TakePitchHeight(pChatPanel->GetFontStyle(), PitchHeight, RealFontHeight);
						
						MyStr->SetHeight(pChatPanel->GetFont()->GetHeight() + PitchHeight);

						pChatRow->AddItem(MyStr);
					}
					else
					{
						UIChatString * pChatString = dynamic_cast<UIChatString *>(pChatItem);
						if (pChatString)
						{
							StringW OrgText = pChatString->GetString();

							if (OrgText.size() >= 2)
								OrgText.erase(OrgText.size() - 2);
							else
								OrgText.clear();

							Char FileName[50];
							GetPrivateProfileString(_T("LOADFILE"), _T("MESSAGEOVER"), _T(".."), FileName, 50, CONFIGURATION_INI_FILE);
							OrgText.append(FileName);

							StringW WStr;
							UInt32 Len;
							pChatPanel->GetFont()->DrawFontStrC(OrgText.c_str(), WStr, pChatItem->GetPosX(), 0, 
								pChatPanel->GetWidth(), pChatPanel->GetPitch(), pChatPanel->GetFontStyle(), Len);
							pChatString->SetString(WStr.c_str());

						}
					}

				}
			}
		}
		pChatPanel->HomeRow();
	}

	UIPanelEx * pPanelEx = pImEnt->GetUIManager()->FindUIPanelEx("ChatBalloon");
	if (pPanelEx)
	{
		
		UInt32 LineCount = (UInt32)ceil((Real32)pChatPanel->GetTotalHeight() / 
			(pPanelEx->GetLineRect()->bottom - pPanelEx->GetLineRect()->top));
		pPanelEx->ChangeCount(LineCount);
		pImEnt->ResetRect();
	}
	
	gGlobal.g_pCharacter->AddChatBalloon(pImEnt);

	gGlobal.g_pRumorMsgProcess->ShowRumorMessage(String());
	
	*/
}
void CloseSubWindow()
{
	QGLApplication::GetInstance().CloseSubWindow();
}



UIManager* GetSubUIManager()
{
	return QGLApplication::GetInstance().GetSubWindow()->m_UIManager;
}

CMenuSkillProcess * GetSkillProcess()
{
	return gGlobal.g_pSkillProcess;
}

CMenuQuestLogProcess * GetQuestLogProcess()
{
	return gGlobal.g_pQuestLogProcess;
}

CQuestionProcess* GetQuestionProcess()
{
	return &(CQuestionProcess::GetInstance());
}

CMenuPartnerProcess * GetPartnerProcess()
{
	return gGlobal.g_pPartnerProcess;
}

CMenuGivenProcess* GetGivenProcess()
{
	return gGlobal.g_pGivenProcess;
}

CMenuItemProcess* GetItemProcess()
{
	return gGlobal.g_pItemProcess;
}

CMenuTargetProcess * GetMenuTargetProcess()
{
	return gGlobal.g_MenuTargetProcess;
}


CAntiHack* GetAntiHackProcess()
{
	return gGlobal.g_pAntiHack;
}

CMenuBankService* GetBankProcess()
{
	return gGlobal.g_pMenuBankService;

}

CMenuBuffIconProcess * GetMenuBuffIconProcess()
{
	return gGlobal.g_pMenuBuffIconProcess;
}
CMenuSystemSetting* GetMenuSystemSetting()
{
	return gGlobal.g_pSystemSetting;
}
CMenuFriendProcess* GetMenuFriendProcess()
{
	return gGlobal.g_pMenuFriendProcess;
}
CMenuRankProcess* GetMenuRankProcess()
{
	return gGlobal.g_pMenuRankProcess;
}
CMenuPetProcess* GetMenuPetProcess()
{
	return gGlobal.g_pMenuPetProcess;
}

CMenuGuildProcess* GetMenuGuildProcess()
{
	return gGlobal.g_pMenuGuildProcess;
}
CMenuHelpProcess* GetMenuHelpProcess()
{
	return gGlobal.g_pMenuHelpProcess;
}
CBigMap* GetBigMap()
{
	return gGlobal.g_pBigMap;
}
CMenuTradingProcess* GetTradingProcess()
{
	return gGlobal.g_pTradingProcess;
}

CMenuOnlineShopProcess* GetOnlineShopProcess()
{
	return gGlobal.g_pOnlineShopProcess;
}

CMenuMixProcess* GetMixProcess()
{
	return gGlobal.g_pMenuMixProcess;
}

CMenuTitleProcess* GetTitleProcess()
{
	return gGlobal.g_pMenuTitleProcess;
}

CMenuHawkerProcess* GetHawkerProcess()
{
	return gGlobal.g_pHawkerProcess;
}

CMenuGiftBox* GetGiftBoxProcess()
{
	return gGlobal.g_pGiftBoxProcess;
}

CMenuQuestJournal* GetQuestJournalProcess()
{
	return gGlobal.g_pMenuQuestJournal;
}

CWeaponsRank* GetWeaponsRankProcess()
{
	return gGlobal.g_pWeaponsRank;
}

CTeamDescription* GetTeamDescriptionProcess()
{
	return gGlobal.g_pTeamDescription;
}

CGenerationProcess* GetGenerationProcess()
{
	return gGlobal.g_pGenerationProcess;
}
CMenuWareHouseProcess* GetWareHouseProcess()
{
	return gGlobal.g_pWareHouseProcess;
}

CMenuAutoWalkProcess* GetAutoWalkProcess()
{
	return gGlobal.g_pAutoWalkProcess;
}

CMenuSellYuanBaoProcess* GetSellYBProcess()
{
	return gGlobal.g_pMenuSellYBProcess;
}
CMenuTargetEquipProcess* GetTargetEquipProcess()
{
	return gGlobal.g_pTargetEquipProcess;
}

CRelationSystemProcess* GetRelationSystemProcess()
{
	return &(CRelationSystemProcess::GetInstance());
}
CWantedProcess* GetWantedProcess()
{
	return &(CWantedProcess::GetInstance());
}

CMenuPartnerCultivationUpProcess* GetPartnerCultivationUpProcess()
{
	return gGlobal.g_pPartnerCultivationUpProcess;
}
CAccShopProcess* GetAccShopProcess()
{
	return gGlobal.g_pAccShopProcess;
}
CPointsShopProcess* GetPointsShopProcess()
{	
	return gGlobal.g_pPointsShopProcess;
}
VOID QGLApplication::SetUILinerImage(StringA component, UInt32 denumerator, UInt32 numerator)
{
	UILinerImage* pLI = gGlobal.pUIManager->FindUILinerImage(component.c_str());
	if( pLI )
	{
		if( denumerator > 0 )
		{
			if( numerator > denumerator )
			{
				pLI->SetDenumerator(denumerator);
				pLI->SetNumerator(denumerator);
			}else
			{
				pLI->SetDenumerator(denumerator);
				pLI->SetNumerator(numerator);
			}
		}else
		{
			pLI->SetDenumerator(1);
			pLI->SetNumerator(0);
		}
	}
}

bool QGLApplication::IsValidCatchTarget(CCharacterControl * pSrcCtrl, CCharacterControl * pTargetCtrl)
{
	if (pTargetCtrl->GetBatActorObject()->GetTeam() != pSrcCtrl->GetBatActorObject()->GetTeam())
	{
		CBatMob* pmob = CastNetObject<CBatMob>(pTargetCtrl->GetBatActorObject());
		if (pmob && pmob->GetPartner_type() != CANNOT_BE_PARTNER && pmob->GetOwner_id() == 0 )
			return true;
		else
			return false;
	}

	return false;
}
VOID QGLApplication::UpateSoundDataLst(UInt32 CurrTime)
{
	UInt PlayTime = 0;
	StlMultiMap<UInt32, String>::iterator it;

	for (it = m_SoundDataLst.begin(); it != m_SoundDataLst.end();
		++ it)
	{
		if (CurrTime > it->first)
		{
			if (!it->second.IsEmpty())
			{
				FSound* sfx = NULL;
				String Temp;
				Temp.Format(_T("%s%s"), WAV_FILE_PATH, it->second.c_str());
				sfx = FSOUND_Manager::GetInstance().CreateSound(Temp.c_str());
				if (sfx)
				{
					sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
					sfx->Play();
				}
			}
			PlayTime = 1;
		}
		else
			break;
	}
	if (PlayTime > 0)
	{
		//m_NextAttackQueueTime = ::timeGetTime() + PlayTime;
		m_SoundDataLst.erase(m_SoundDataLst.begin(), it);
	}
}
VOID QGLApplication::UpdateEffectDataLst(UInt32 CurrTime)
{
	
	UInt PlayTime = 0;
	StlMultiMap<UInt32, EffectData>::iterator it;

	for (it = m_EffectDataLst.begin(); it != m_EffectDataLst.end();
		++ it)
	{
		if (CurrTime > it->first)
		{
			//String FileName = TestEffectAtkSuffix[EntityBeingAttackItr->second.AtkSkillIndex].c_str();
			//FileName.Format(_T("effAtk%s.ent"), TestEffectSuffix[EntityBeingAttackItr->second.AtkSkillIndex].c_str());
			CAnimatedEntity Effect;
			Effect.LoadEntityFile(it->second.effectFileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);
			Effect.SetRotation(it->second.rotation);
			if (Effect.GetOffsetType() == ENTITYOFFSETTYPE_FULLSCREEN ||
				!it->second.attach)
			{
				AddParticle(it->second.effectFileName.c_str(), 0, TRUE, 1, it->second.targetEnt, TRUE, ParticleExtraZ, Effect.GetAnimationRotation(), TRUE,
					it->second.OffsetX, it->second.OffsetY);
			}
			else
			{
				if (it->second.DeleteTime > 0)
					it->second.targetEnt->LoadEntityFile(it->second.effectFileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS, TRUE, CurrTime + it->second.DeleteTime, TRUE);
				else
					it->second.targetEnt->LoadEntityFile(it->second.effectFileName.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS, TRUE, 0, TRUE);
			}
			//EntityBeingAttackItr->second.AtkTimeToHitSkill = 0;
			if (Effect.GetTotalTimeOfGivenAnimation() > PlayTime)
				PlayTime = Effect.GetTotalTimeOfGivenAnimation();
		}
		else
			break;
	}
	if (PlayTime > 0)
	{
		//m_NextAttackQueueTime = ::timeGetTime() + PlayTime;
		m_EffectDataLst.erase(m_EffectDataLst.begin(), it);
	}

	static const Int ItemEffectOffset = 70;
	static UInt32 UIEffectIndex = 0;
	UIPanel * pPanel = gGlobal.pUIManager->FindUIPanel("UIEffectPanel");
	if (pPanel)
	{
		StlMultiMap<UInt32, UIEffectData>::iterator itUI = m_UIEffectDataLst.begin();
		while (itUI != m_UIEffectDataLst.end())
		{
			if (itUI->second.AniFileName.IsEmpty())
			{
				if (CurrTime > itUI->first)
				{
					UIAniIcon * pAniIcon = SafeCreateObject(UIAniIcon);
					pAniIcon->SetAlign(1);
					pAniIcon->SetRotation(0);
					pAniIcon->SetAnimationIndex(0);
					pAniIcon->SetAnimationData(itUI->second.effectFileName.c_str(), EQUIP_OTHERS);

					if (pAniIcon->GetAniEnt()->HasAniFile())
					{
						pAniIcon->SetOffsetXY(0, 0);
						String AniIconName;
						AniIconName.Format(_T("%s%dAni"), itUI->second.effectFileName.c_str(), UIEffectIndex);


						pAniIcon->GetAniEnt()->SetAnimationLoop(BOOLEAN_FALSE);						
						pAniIcon->SetNotDetected(TRUE);


						gGlobal.pUIManager->AddUIID(StringA(AniIconName.c_str()).c_str(), pAniIcon);

						pPanel->AddUIItem(pAniIcon);
						pPanel->SetEnable(TRUE);
						gGlobal.pUIManager->AddUpdateUIItem(pAniIcon);
						//panel->Render(NULL);

						Int32 posX = itUI->second.PixelPosX - (Int32)QGLApplication::GetInstance().GetRenderCamera()->GetLeftUpPos().x - pPanel->GetWidth() / 2 + (QGLApplication::GetInstance().GetRenderClass()->GetWinWidth() >> 1);
						Int32 posY = itUI->second.PixelPosY - (Int32)QGLApplication::GetInstance().GetRenderCamera()->GetLeftUpPos().y - pPanel->GetHeight() / 2 - ItemEffectOffset + (QGLApplication::GetInstance().GetRenderClass()->GetWinHeight() >> 1);
						pAniIcon->SetOffsetXY( posX, posY);

						pPanel->SetEnable(true);
						itUI->second.AniFileName = AniIconName;
						UIEffectIndex ++;
					}
					else
						SafeDeleteObject(pAniIcon);
				}
			}
			else
			{

				UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(StringA(itUI->second.AniFileName.c_str()).c_str());
				if (pAniIcon)
				{
					if (pAniIcon->GetAniEnt()->FrameEnd(CurrTime))
					{
						gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
						pPanel->LeaveUIItem(pAniIcon);					
						gGlobal.pUIManager->RemoveUpdateUIItem(pAniIcon);
						gGlobal.pUIManager->RemoveUIID(StringA(itUI->second.AniFileName.c_str()).c_str());
						SafeDeleteObject(pAniIcon);
						itUI = m_UIEffectDataLst.erase(itUI);
						continue;
					}
					Int32 posX = itUI->second.PixelPosX - (Int32)QGLApplication::GetInstance().GetRenderCamera()->GetLeftUpPos().x - pPanel->GetWidth() / 2 + (GetRenderClass()->GetWinWidth() >> 1);
					Int32 posY = itUI->second.PixelPosY - (Int32)QGLApplication::GetInstance().GetRenderCamera()->GetLeftUpPos().y - pPanel->GetHeight() / 2 - ItemEffectOffset + (GetRenderClass()->GetWinHeight() >> 1);
					pAniIcon->SetOffsetXY( posX, posY);
				}

				
			}
			++ itUI;
		}
		if (m_UIEffectDataLst.size() == 0)
			pPanel->SetEnable(false);
	}
}

VOID QGLApplication::AddParticle(LPCTSTR filename, Int LiveBase, BOOL Destory, Int NoOfParticle,
									   CEntityBase * pTargetEntity, BOOL SetPartent, Int64 ZValue, Int Rotation, BOOL Free,
									   Int OffsetX, Int OffsetY)
{
	ParticleStructSetting ParSetting;
	ParSetting.Destory = Destory;
	ParSetting.LiveTimeBase = LiveBase;
	ParSetting.ExtraZValue = ZValue;
	ParSetting.NoOfParticle = NoOfParticle;
	ParSetting.Rotation = Rotation;
	ParSetting.Loop = FALSE;
	ParSetting.SubOffsetY = OffsetY;
	ParSetting.SubOffsetX = OffsetX;
	ParSetting.HaveEffect = FALSE;
	ParSetting.FreeAfterFinish = Free;
	CAnimatedEntity TempEnt;
	TempEnt.LoadEntityFile(filename, gGlobal.pD3DDevice, EQUIP_OTHERS);
	m_MapParticle->AddAnimatedEntityToSystem(&TempEnt, ParSetting, 
		pTargetEntity, SetPartent);
}

VOID QGLApplication::AddUIEffectToLst(UInt32 startTime, UIEffectData uiEffectData)
{
	m_UIEffectDataLst.insert(std::make_pair(startTime, uiEffectData));
}

VOID QGLApplication::AddEffectToLst(UInt32 startTime, EffectData effectData)
{
	m_EffectDataLst.insert(std::make_pair(startTime, effectData));
}

VOID QGLApplication::AddSoundToLst(UInt32 startTime, String soundData)
{
	m_SoundDataLst.insert(std::make_pair(startTime, soundData));
}

VOID QGLApplication::PlayFireWorks(String word, CCharacterControl * pTarget)
{
	if (pTarget == NULL)
		return;
	
	UInt32 CurrTime = ::timeGetTime();
	String riseEnt = _T("eff_items_firework_arise.ent");

	EffectData EData;
	EData.attach = false;
	EData.effectFileName = riseEnt;
	EData.rotation = 0;
	EData.targetEnt = pTarget->GetCharMainView();

	m_EffectDataLst.insert(std::make_pair(CurrTime, EData));
	CAnimatedEntity ent;
	
	ent.LoadEntityFile(riseEnt.c_str(), gGlobal.pD3DDevice);

	UInt32 ariseTime = ent.GetTotalTimeOfGivenAnimation();

	Int width = GetPrivateProfileInt(_T("LOADFILE"), _T("FIREWORKSWIDTH"), 90, CONFIGURATION_INI_FILE);
	Int height = GetPrivateProfileInt(_T("LOADFILE"), _T("FIREWORKSHEIGHT"), 90, CONFIGURATION_INI_FILE);
	Int delay = GetPrivateProfileInt(_T("LOADFILE"), _T("FIREWORKSDELAY"), 200, CONFIGURATION_INI_FILE);
	
	Int TotalWidth = width * word.GetLength();
	for (Int i = 0; i < word.GetLength(); i++)
	{
		String buf;
		buf.Format(_T("eff_items_firework_word_%s.ent"), word.substr(i, 1).c_str());
		EffectData EData;
		EData.attach = false;
		EData.effectFileName = buf;
		EData.rotation = 0;
		EData.targetEnt = pTarget->GetCharMainView();
		EData.OffsetY = -1 * height;
		EData.OffsetX = (Int)((i + 0.5)* TotalWidth / (Int)word.GetLength() - TotalWidth / 2);

		m_EffectDataLst.insert(std::make_pair(CurrTime + ariseTime - delay, EData));
	}
}

VOID ChangeFriendIcon()
{
	if ( gConfigure.uNetwork && gGlobal.g_pCharacter->GetCharNetObject() &&
		gGlobal.g_pCharacter->GetCharNetObject()->GetAction() == CMapActor::ST_BATTLE)
	{
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_ADD_FRIEND_IN_BATTLE")), CHANNELTYPE_SYSTEM);
		return;
	}
	State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CFriendState));
	gGlobal.g_CursorState->Block();
}

VOID OpenHomePage()
{
	ShellExecute(NULL, _T("open"), gConfigure.strCSLink.c_str(),
                NULL, NULL, SW_SHOWNORMAL);
}

VOID OpenFriendChatBox(PSTRINGA msg, PSTRINGA function)
{
	gGlobal.g_FriendChatBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO, gGlobal.GetStringTable()->Get(String(msg).c_str()), "FriendChat");
	UIItem * pItem = gGlobal.pUIManager->GetUIItem("FriendChatYes");
	if (pItem)
	{
		pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, function);
	}
}

VOID OpenGuildBox()
{
	if (gGlobal.g_pMenuGuildProcess->IsSelectedMember(false))
	{
		gGlobal.g_GuildBox.ShowMessageBox(MESSAGEBOX_TYPE_ARGREE_REFUSE_CANCEL,gGlobal.GetStringTable()->Get(_T("MSG_GUILD_APPLY_BOX")), "GuildApply");	
		
	}
}

VOID OpenDeleteMemberGuildBox()
{
	if (gGlobal.g_pMenuGuildProcess->IsSelectedMember(true))
	{
		UIEditBoxEx * pEditBox = gGlobal.pUIManager->FindUIEditBoxEx("MenuGuildDeleteBoxEdit");
		if (pEditBox && pEditBox->GetTextSize() > 0)
		{
			gGlobal.g_pMenuGuildProcess->m_kickReason = pEditBox->GetText();
			gGlobal.g_GuildBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO,gGlobal.GetStringTable()->Get(_T("MSG_GUILD_APPLY_BOX_DELETE")), "GuildApply");	
			UIItem * pItem = gGlobal.pUIManager->GetUIItem("GuildApplyYes");
			if (pItem)
			{
				pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "ConfirmDelete");
			}
		}
		else
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("")), CHANNELTYPE_SYSTEM);
	}
}

VOID OpenLeaveGuildBox()
{
	if (gGlobal.g_pMenuGuildProcess->m_ownGuildMember && 
		gGlobal.g_pMenuGuildProcess->m_ownGuildMember->GetPost() == GUILD_POSITION_MASTER)
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_APPLY_BOX_LEAVE_FAIL")));
		return;
	}

	gGlobal.g_GuildBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO,gGlobal.GetStringTable()->Get(_T("MSG_GUILD_APPLY_BOX_LEAVE")), "GuildApply");	
	UIItem * pItem = gGlobal.pUIManager->GetUIItem("GuildApplyYes");
	if (pItem)
	{
		pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "ConfirmLeaveGuild");
	}
}

VOID ClsoeGuildBox()
{
	gGlobal.g_GuildBox.DeleteMessageBox();
}

VOID CloseQuestionBox()
{
	gGlobal.g_QuestionBox.DeleteMessageBox();
}

void ChangeChatIcon(int index)
{
	if (gGlobal.g_pCharacter->GetCharNetObject() && !gGlobal.g_pMenuFriendProcess->IsCurrMsgIcon(index) &&
		!gGlobal.g_pCharacter->GetCharNetObject()->GetYuanbaoLock())
		gGlobal.g_pCharacter->GetCharNetObject()->ChangeMsgIcon(gGlobal.pPeerProxy, gGlobal.g_context,  index);
	else
	{
		String text = gGlobal.GetStringTable()->Get(_T("MSG_YUANBAO_LOCK_CANT_USE"));
		gGlobal.PrintPlayerMessage(text.c_str());	
	}
}

void CloseFriendBox()
{
	gGlobal.g_FriendChatBox.HideMessageBox();
}

void OpenFactionDetail()
{
	gGlobal.g_pCharProcess->OpenFactionDetail();
}

void ConfirmReplyString()
{
	UIEditBox * pEdit = gGlobal.pUIManager->FindUIEditBox("StringReplyEdit");
	if (pEdit)
	{
		if (gGlobal.g_pScriptControl && gGlobal.g_pScriptControl->GetScriptSysNetObject())
			gGlobal.g_pScriptControl->GetScriptSysNetObject()->ReplyAnswerString(gGlobal.pPeerProxy, gGlobal.g_context, pEdit->GetText());
	}
}
void ReplyStringNULL()
{
	if (gGlobal.g_pScriptControl && gGlobal.g_pScriptControl->GetScriptSysNetObject())
			gGlobal.g_pScriptControl->GetScriptSysNetObject()->ReplyAnswerString(gGlobal.pPeerProxy, gGlobal.g_context, String());
}

bool IsCurrMsgIcon(int index)
{
	return gGlobal.g_pMenuFriendProcess->IsCurrMsgIcon(index);
}


void RetrieveCharacter(UInt32 Index)
{
	gGlobal.g_pLoginProcess->SetCharacterIndex(Index);
	gGlobal.g_pAccountControl->RecoverCharacter();	

}

Boolean CheckCharacterFrozen(UInt32 Index)
{
	return gGlobal.g_pAccountControl->CheckCharacterFrozenStatus(Index);

}

void LockItem(UInt32 index, UInt32 assetType)
{
	gGlobal.g_pItemProcess->LockItem(index, assetType);
}

void UnlockItem(UInt32 index, UInt32 assetType)
{
	gGlobal.g_pItemProcess->UnlockItem(index, assetType);

}

void SetLockCursor(Boolean lock)
{
	if (lock)
	{
		if ( gGlobal.g_CursorState->IsBlocked() )
			gGlobal.g_CursorState->Unblock();
		if (State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CAssetLockState)))
		{
			gGlobal.g_CursorState->Block();
			gGlobal.PrintMessage(String(gGlobal.GetStringTable()->Get(_T("MSG_ASSET_LOCK_TIPS6"))), CHANNELTYPE_SYSTEM);
		}
	}
	else if (gGlobal.g_CursorState && (gGlobal.g_CursorState->GetStateID() == ST_APP_ASSET_LOCK))
	{
		if ( gGlobal.g_CursorState->IsBlocked() )
			gGlobal.g_CursorState->Unblock();
		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}
}

void SetUnLockCursor(Boolean unlock)
{
	if (unlock)
	{
		if ( gGlobal.g_CursorState->IsBlocked() )
			gGlobal.g_CursorState->Unblock();
		if (State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CAssetUnLockState)))
		{
			gGlobal.g_CursorState->Block();
			gGlobal.PrintMessage(String(gGlobal.GetStringTable()->Get(_T("MSG_ASSET_LOCK_TIPS6"))), CHANNELTYPE_SYSTEM);	
		}
	}
	else if (gGlobal.g_CursorState && (gGlobal.g_CursorState->GetStateID() == ST_APP_ASSET_UNLOCK))
	{
		if ( gGlobal.g_CursorState->IsBlocked() )
			gGlobal.g_CursorState->Unblock();
		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}
}

VOID QGLApplication::UpdateGenerationClick(LONG x, LONG y)
{
	if (m_SelectedMapControl)
	{
		CCharacterControl * pCharCtrl = dynamic_cast<CCharacterControl *>(m_SelectedMapControl);
		if (NULL != pCharCtrl && NULL != gGlobal.g_pCharacter)
		{
			if( pCharCtrl->GetMapActorID() != gGlobal.g_pCharacter->GetMapActorID() )
			{
				CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
				if (NULL != pChar) {
					pChar->RequestGeneration(gGlobal.pPeerProxy,
						gGlobal.g_context,
						pCharCtrl->GetChar_id());
				}
			}
			gGlobal.g_CursorState->Unblock();

			State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
		}
	}
}