//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "QGLWindow.h"
//-- Local
#include "Render.h"
#include "Camera.h"
#include "QGLApplication.h"
#include "Configure.h"
#include "Math.h"
#include "QGL/Global.h"
#include "CharacterControl.h"
#include "ScriptControl.h"
#include "BattleControl.h"
#include "Common/PathFinder.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "MenuShopProcess.h"
#include "MenuSkillProcess.h"
#include "MenuPartnerProcess.h"
#include "BigMap.h"
#include "QGL/AntiHack.h"
#include "MenuBankService.h"
#include "MenuHawkerProcess.h"

#include <CommCtrl.h>

struct QGLWindow::PImpl
{
	CRender* pRender;

	PImpl(VOID);
	~PImpl(VOID);
};

QGLWindow::PImpl::PImpl(VOID)
: pRender(NULL)
{
}

QGLWindow::PImpl::~PImpl(VOID)
{
}

QGLWindow::QGLWindow(VOID)
: _pImpl(new PImpl())
{
}

QGLWindow::~QGLWindow(VOID)
{
	if (_pImpl->pRender)
		SafeDeleteObject(_pImpl->pRender);
	SafeDelete(_pImpl);
}

Boolean QGLWindow::OnCreate(VOID)
{
	_pImpl->pRender = SafeCreateObject5(CRender,(HINSTANCE)NULL,m_hWnd, DefaultWinWidth, DefaultWinHeight, 
		(BOOL)gConfigure.nFullScreen);
	if (_pImpl->pRender->GetDevice() == NULL)
		return (BOOLEAN_FALSE);
	SetIcon(IDI_ICON1, IDI_ICON1);

	return (BOOLEAN_TRUE);
}

VOID QGLWindow::OnDispose(VOID)
{
	if (m_hWnd)
		DestroyWindow(m_hWnd);
}

Boolean QGLWindow::ProcessWindowMessage(UInt &uMsg, VOID * wParam, VOID * lParam, VOID * & lResult)
{
	if (_pImpl == NULL ||
		_pImpl->pRender == NULL ||
		_pImpl->pRender->GetDevice() == NULL)
		return FALSE;

	static BOOL	bTracking = FALSE;

	/*if (uMsg == WM_KEYUP && (WPARAM)wParam == VK_SNAPSHOT)
	{
		GetRenderPtr()->OutputBackBufferToFile(PICTURE_OUTPUT_FOLDER);
		if (QGLApplication::GetInstance().GetSubWindow()->pRender)
			QGLApplication::GetInstance().GetSubWindow()->pRender->OutputBackBufferToFile(PICTURE_OUTPUT_SUBWINDOW_FOLDER);

		return BOOLEAN_TRUE;    
	}*/

	//TRACE_INFODTL_3(GLOBAL_LOGGER, _F("uMsg:%d,wParam:%d,lParam:%d"),uMsg,wParam,lParam);
	switch( uMsg)
	{
	case WM_MOUSEMOVE:
		if (!bTracking)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = m_hWnd;
			tme.dwFlags = TME_LEAVE|TME_HOVER;
			tme.dwHoverTime = 1;
			bTracking = _TrackMouseEvent(&tme);
		}

		gGlobal.MarkMouseDirtyRect();

		gGlobal.pUIManager->SetCursorX((Int16)(LOWORD(lParam)));
		gGlobal.pUIManager->SetCursorY((Int16)(HIWORD(lParam)));
		//m_CursorX = (Int16)(LOWORD(lParam)); m_CursorY = (Int16)(HIWORD(lParam));
		gGlobal.pUIManager->SetMouseMoveFlag(true);
		break;
		
	case WM_LBUTTONDOWN:
		{
			if (gGlobal.LeftClickStatus == LEFTCLICK_AUTO)
			{
				gGlobal.LeftClickStatus = LEFTCLICK_NORMAL;
				gGlobal.LeftClickStatusCounter = 0;	
			}

			if (gGlobal.g_CursorState && (gGlobal.g_CursorState->GetStateID() == ST_APP_SHOP_SELL) )
			{
				if ( gGlobal.g_pShopProcess && gGlobal.g_pShopProcess->OnSellClickInValid() == BOOLEAN_FALSE )
				{
					gGlobal.LeftClickStatus = LEFTCLICK_DOWN;
					gGlobal.LeftClickStatusCounter = 0;	
					QGLApplication::GetInstance().UpdateLeftClick(LOWORD(lParam), HIWORD(lParam));
					return (BOOLEAN_FALSE);
				}
			}
			break;
		}
	case WM_LBUTTONDBLCLK:
		{
			if (gGlobal.LeftClickStatus == LEFTCLICK_AUTO)
			{
				gGlobal.LeftClickStatus = LEFTCLICK_NORMAL;
				gGlobal.LeftClickStatusCounter = 0;	
			}

			if (gGlobal.g_CursorState && (gGlobal.g_CursorState->GetStateID() == ST_APP_SHOP_SELL) )
			{
				if ( gGlobal.g_pShopProcess && gGlobal.g_pShopProcess->OnSellClickInValid() == BOOLEAN_FALSE )
				{
					gGlobal.LeftClickStatus = LEFTCLICK_DOWN;
					gGlobal.LeftClickStatusCounter = 0;	
					QGLApplication::GetInstance().UpdateLeftClick(LOWORD(lParam), HIWORD(lParam));
					return (BOOLEAN_FALSE);
				}
			}
			break;
		}
	case WM_LBUTTONUP:
		{
			if (gGlobal.LeftClickStatus == LEFTCLICK_DOWN)
			{
				gGlobal.LeftClickStatus = LEFTCLICK_NORMAL;
				gGlobal.LeftClickStatusCounter = 0;
			}
			break;
		}
	case WM_RBUTTONDOWN:
		{
			if (gGlobal.g_CursorState && ((gGlobal.g_CursorState->GetStateID() == ST_APP_SHOP_SELL) || 
				(gGlobal.g_CursorState->GetStateID() == ST_APP_ASSET_LOCK) || (gGlobal.g_CursorState->GetStateID() == ST_APP_ASSET_UNLOCK)))
			{
				
				QGLApplication::GetInstance().UpdateRightClick(LOWORD(lParam), HIWORD(lParam));

				return (BOOLEAN_FALSE);
			}
		}
		break;
		
	case WM_SHORTCUTKEY:
		{
			TRACE_ALWAYSDTL_1(GLOBAL_LOGGER, _F("ShortKey: %x"), (Int)wParam);
			if ((WPARAM)wParam == 1001)
			{
				if (QGLApplication::GetInstance().m_mapState->GetStateID() != ST_APP_LOGIN)
				{
					gGlobal.g_HideOtherPlayers ++;
					if (gGlobal.g_HideOtherPlayers >= 3)
						gGlobal.g_HideOtherPlayers = 0;
					QGLApplication::GetInstance().AddAllDirtyRect();
				}
			}
			else
			if((WPARAM)(wParam) >= VK_F1 && (WPARAM)(wParam) <= VK_F10)
			{
				bool Flag = false;

				switch(QGLApplication::GetInstance().GetMapStateID())
				{
				case ST_APP_BATTLE:
					switch(gGlobal.g_CursorState->GetStateID())
					{
					case ST_APP_BATTLE_ATTACK:
					case ST_APP_BATTLE_PROTECT:
					case ST_APP_BATTLE_USEITEM:
					case ST_APP_BATTLE_SKILL:
					case ST_APP_BATTLE_CATCH:
						break;
					default:
						if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer() && 
							(!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() || 
							(gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() && gGlobal.g_pBattleControl->GetControlPartner() && 
							!gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand())))
							Flag = true;
					}
					break;
				case ST_APP_MAP:
					Flag = true;
					break;
				}

				if(Flag)
				{
					UInt32 Index = (UInt32)wParam - VK_F1;
					

					if(gGlobal.g_BPShortCutFlag)
					{
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SCPartnerUseSkillGo", String(), Index);
					}
					else
					{
						UInt32 SlotPos = gGlobal.g_ShortCutSlot[Index].m_RefNum;

						if(gGlobal.g_ShortCutSlot[Index].m_RefType > 0)
						{
							UIItem *Panel = gGlobal.pUIManager->GetUIItem("BattlePartnerPanel");
							if(Panel)
							{
								if(!Panel->IsEnable())
									CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "UseShortCutHotKey", String(), Index);
							}
							else
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "UseShortCutHotKey", String(), Index);
						}
					}
				}
			}
		}
		break;
	case WM_KEYDOWN:              // Is A Key Being Held Down?
		{
			
			if ((WPARAM)wParam ==  VK_UP)
			{
				UIEditBox * pEditBox = dynamic_cast<UIEditBox *>(gGlobal.pUIManager->GetKeyInItem());

				if (pEditBox == NULL)
					pEditBox = dynamic_cast<UIEditBox *>(gGlobal.pUIManager->GetFocusItem());

				if (pEditBox == NULL)
					break;

				if (pEditBox->GetTypeID() == typeid(UIEditBoxEx *))
					break;

				pEditBox->UpPastMsg();

				if(pEditBox->ShowHisInput())
				{
					gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
				}
				/*
				gGlobal.g_pastHisIndex --;
				if (gGlobal.g_pastHisIndex < 0)
					gGlobal.g_pastHisIndex = 0;

				gGlobal.ShowHisInput();
				*/
			}
			else if ((WPARAM)wParam ==  VK_DOWN)
			{
				if (::timeGetTime() < gConfigure.uDBClkDown + gGlobal.g_dbClkDownTime &&
					gGlobal.g_DownButtonUp)
					QGLApplication::GetInstance().ClearMessage();
				else
				{
					UIEditBox * pEditBox = dynamic_cast<UIEditBox *>(gGlobal.pUIManager->GetKeyInItem());

					if (pEditBox == NULL)
						pEditBox = dynamic_cast<UIEditBox *>(gGlobal.pUIManager->GetFocusItem());

					if (pEditBox == NULL)
						break;

					if (pEditBox->GetTypeID() == typeid(UIEditBoxEx *))
						break;

					pEditBox->DownPastMsg();

					if(pEditBox->ShowHisInput())
					{
						gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
					}

					/*
					gGlobal.g_pastHisIndex ++;
					if (gGlobal.g_pastHisIndex >= (Int)gGlobal.g_pastInputMsgHis.size())
					{
						gGlobal.g_pastHisIndex = (Int)gGlobal.g_pastInputMsgHis.size() - 1;
					}
					
					gGlobal.ShowHisInput();
					*/
				}
				gGlobal.g_dbClkDownTime = ::timeGetTime();
				gGlobal.g_DownButtonUp = FALSE;
			}
			else if ((WPARAM)wParam ==  VK_CONTROL)
			{
				gGlobal.g_bCtrlButtonUp = false;
			}
			else if ((WPARAM)wParam == VK_TAB)
			{
				if (!gGlobal.g_pBigMap->IsOpenBigMap() && !gGlobal.pUIManager->IsFreeze())
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowMidMap", String());
			}
			break;
		}
	case WM_KEYUP:                // Has A Key Been Released?
		{
			switch((WPARAM)wParam)
			{
			case VK_NUMPAD4:
				break;
			case VK_DOWN:
				gGlobal.g_DownButtonUp = TRUE;
				break;
			case VK_CONTROL:
				gGlobal.g_bCtrlButtonUp =true;
				break;
			case VK_SNAPSHOT:
				GetRenderPtr()->OutputBackBufferToFile(PICTURE_OUTPUT_FOLDER);
				if (QGLApplication::GetInstance().GetSubWindow()->pRender)
					QGLApplication::GetInstance().GetSubWindow()->pRender->OutputBackBufferToFile(PICTURE_OUTPUT_SUBWINDOW_FOLDER);
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayCapScreenEffect", String());
				break;
			default:
				break;
			}
		}
		
	}

	
	if(gGlobal.pUIManager)
	{
		if(gGlobal.pUIManager->MsgProc(m_hWnd, uMsg, (WPARAM)wParam, (LPARAM)lParam) == 0)
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
				LRESULT  LT = ::DefWindowProc(m_hWnd, uMsg, (WPARAM)wParam, (LPARAM)lParam);
				DWORD DT = (DWORD)LT;
				if(DT == HTCLIENT)
				{					
					while(gGlobal.WinCursorCount > -1)
						gGlobal.WinCursorCount = ShowCursor(FALSE);
					gGlobal.ShowCursor = true;
					QGLApplication::GetInstance().GetSubWindow()->ShowCursor = false;
					gGlobal.LeftClickUpdate = TRUE;
				
					if(gGlobal.pUIManager->GetDragNow())
					{
						if(gGlobal.pUIManager->GetDragPanel())
						{
							gGlobal.pUIManager->GetDragPanel()->SetEnable(true);
							gGlobal.pUIManager->AddDirtyUIItem(gGlobal.pUIManager->GetDragPanel());
						}
					}
				}
				else
				{
					while(gGlobal.WinCursorCount < 0)
						gGlobal.WinCursorCount = ShowCursor(TRUE);
					gGlobal.ShowCursor = false;
					gGlobal.LeftClickUpdate = FALSE;
					if(gGlobal.pUIManager->GetDragNow())
					{
						UIPanel *Panel = gGlobal.pUIManager->GetDragPanel();
						if(Panel->GetCanDrag() == 2 || Panel->GetCanDrag() == 12)
						{
							Panel->SetEnable(false);
							gGlobal.pUIManager->AddDirtyUIItem(Panel);
						}
					}
				}
				return	DT <= 0;
			}
			
		case WM_MOUSEHOVER:
			while(gGlobal.WinCursorCount > -1)
				gGlobal.WinCursorCount = ShowCursor(FALSE);
			gGlobal.ShowCursor = true;
			QGLApplication::GetInstance().GetSubWindow()->ShowCursor = false;
			gGlobal.LeftClickUpdate = TRUE;
			if(gGlobal.pUIManager->GetDragNow())
			{
				UIPanel *Panel = gGlobal.pUIManager->GetDragPanel();
				if(Panel->GetCanDrag() == 2 || Panel->GetCanDrag() == 12)
				{
					Panel->SetEnable(false);
					gGlobal.pUIManager->AddDirtyUIItem(Panel);
				}
			}
			break;
			
		case WM_MOUSELEAVE:
			gGlobal.LeftClickUpdate = FALSE;
			while(gGlobal.WinCursorCount < 0)
				gGlobal.WinCursorCount = ShowCursor(TRUE);
			gGlobal.ShowCursor = false;
			bTracking = FALSE;
			gGlobal.pUIManager->MouseLeave();
			if(gGlobal.pUIManager->GetDragNow())
			{
				gGlobal.pUIManager->GetDragPanel()->SetEnable(false);
				gGlobal.pUIManager->AddDirtyUIItem(gGlobal.pUIManager->GetDragPanel());
			}
			break;

		case WM_ACTIVATE:              // Watch For Window Activate Message
		{
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

		

		case WM_CLOSE:                // Did We Receive A Close Message?
		{
			//PostQuitMessage(0);            // Send A Quit Message
			if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_LOGIN && gGlobal.WindowShow)
			{
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenConfirmQuitGame", String());
				uMsg = WM_NULL;
			}
			else
			{
				PostQuitMessage(0);
			}

			return BOOLEAN_TRUE;                // Jump Back
		}
		



		case WM_MOUSEMOVE:
		{
			
			break;
		}
		case WM_EXITSIZEMOVE:
			{
				QGLApplication::GetInstance().ResetSubWindowPos();
				QGLApplication::GetInstance().RestoreSubWindowData();
				break;
			}
		case WM_LBUTTONDOWN:
			{
				gGlobal.LeftClickStatus = LEFTCLICK_DOWN;
				gGlobal.LeftClickStatusCounter = 0;	
				QGLApplication::GetInstance().UpdateLeftClick(LOWORD(lParam), HIWORD(lParam));
				break;
			}
		case WM_LBUTTONDBLCLK:
			{
				gGlobal.LeftClickStatus = LEFTCLICK_DOWN;
				gGlobal.LeftClickStatusCounter = 0;
				QGLApplication::GetInstance().UpdateLeftClick(LOWORD(lParam), HIWORD(lParam));
				break;
			}
		case WM_RBUTTONDOWN:
			{
				QGLApplication::GetInstance().UpdateRightClick(LOWORD(lParam), HIWORD(lParam));
				break;
			}

		case WM_KEYDOWN:              // Is A Key Being Held Down?
			{
				//keys[wParam] = TRUE;          // If So, Mark It As TRUE

				MainKeyDown((WPARAM)wParam);
				return BOOLEAN_TRUE;                // Jump Back
			}

		case WM_MOVE:
			{
				RECT	rect;
				::GetWindowRect(GetWindow(), &rect);
				gGlobal.g_StartWndX = rect.left;
				gGlobal.g_StartWndY = rect.top;
	
				if (gGlobal.WindowShow)
					QGLApplication::GetInstance().ResetSubWindowPos();
				return BOOLEAN_TRUE;
			}
		case WM_SYSCOMMAND:              // Int32ercept System Commands
			{
				switch ((WPARAM)wParam)              // Check System Calls
				{
				case SC_SCREENSAVE:          // Screensaver Trying To Start?
				case SC_MONITORPOWER:        // Monitor Trying To Enter Powersave?
				{
					gGlobal.WindowShow = FALSE;
					return BOOLEAN_TRUE;              // Prevent From Happening
				}
				case   SC_MAXIMIZE:   
					{   
						gGlobal.WindowShow = TRUE;
						QGLApplication::GetInstance().AddAllDirtyRect();
						gGlobal.g_DrawAll = TRUE;
						break;
					}   
				case   SC_RESTORE:   
					{   
						gGlobal.WindowShow = TRUE; 
						QGLApplication::GetInstance().AddAllDirtyRect();
						gGlobal.g_DrawAll = TRUE;
						break;
					}   
				case   SC_MINIMIZE:   
					{   
						gGlobal.WindowShow = FALSE;
						break;
					} 

				}
				break;                  // Exit
			}
		case WM_SYSCHAR:
			return BOOLEAN_TRUE;    
			break;
		case WM_SYSKEYDOWN:				//key down the alt
			{
				if ((WPARAM)wParam == VK_F10)
					PostMessage(m_hWnd, WM_KEYDOWN, (WPARAM)VK_F10, 0);
				else
					MainKeyDown((WPARAM)wParam);
				return BOOLEAN_TRUE;    
			}
			break;
		case WM_KEYUP:                // Has A Key Been Released?
		{
			switch((WPARAM)wParam)
			{
			case VK_NUMPAD4:
				break;
			default:
				break;
			}
			return BOOLEAN_TRUE;                // Jump Back
		}

		case WM_SIZE:                // Resize The Direct3D Window
		{
			switch((WPARAM)wParam)
			{
			case SIZE_MAXHIDE:		// Message is sent to all pop-up windows when some other window is maximized.
				Sleep(10);
				break;
			case SIZE_MAXIMIZED:	// The window has been maximized.
				Sleep(10);
				break;
			case SIZE_MAXSHOW:		// Message is sent to all pop-up windows when some other window has been restored to its former size.
				Sleep(10);
				break;
			case SIZE_MINIMIZED:	// The window has been minimized.
				//::GetOggPlayer()->Pause();
				::GetFSoundManager()->PauseMusic();
				::GetFSoundManager()->PauseAll();
				break;
			case SIZE_RESTORED:		
				//::GetOggPlayer()->Resume();
				::GetFSoundManager()->ResumeMusic();
				::GetFSoundManager()->ResumeAll();
				break;
			}
			return BOOLEAN_TRUE;                // Jump Back
		}
		break;
	}
	

	// Pass All Unhandled Messages To DefWindowProc
	return BOOLEAN_FALSE;
	//return (BOOLEAN_TRUE);
}
VOID QGLWindow::MainKeyDown(WPARAM  wParam)
{
	static bool ChangeOggFlag = false;
	static bool ChangeMapFlag = false;
	ChatLst Lst;
	if (wParam == VK_ESCAPE)
	{
		if (QGLApplication::GetInstance().m_mapState->GetStateID() == ST_APP_LOGIN)
			return;
		if (gGlobal.g_CursorState->GetStateID() != ST_APP_MAP_NORMAL &&
			gGlobal.g_CursorState->GetStateID() != ST_APP_MAP_NPC &&
			gGlobal.g_CursorState->GetStateID() != ST_APP_MAP_TALKING)
		{
			QGLApplication::GetInstance().UpdateRightClick(0,0);
		}
		else
		{
			if(!gGlobal.pUIManager->KeyOnEsc())
				PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		}

		return;
	}
	
	/*else if (wParam == VK_SNAPSHOT)
	{
		GetRenderPtr()->OutputBackBufferToFile(PICTURE_OUTPUT_FOLDER);
		if (QGLApplication::GetInstance().GetSubWindow()->pRender)
			QGLApplication::GetInstance().GetSubWindow()->pRender->OutputBackBufferToFile(PICTURE_OUTPUT_SUBWINDOW_FOLDER);
		return;
	}*/

	/*
	if ((GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 &&
		(GetKeyState(VK_MENU) & (1 << (sizeof(SHORT)*8-1))) != 0)
	{
		gGlobal.g_HideOtherPlayers = !gGlobal.g_HideOtherPlayers;
		QGLApplication::GetInstance().AddAllDirtyRect();
		return;
	}
	*/
	if ((GetKeyState(VK_MENU) & (1 << (sizeof(SHORT)*8-1))) != 0)
	{
		if (wParam ==  VK_F4)
			PostQuitMessage(0);
		else if (gGlobal.g_hotKeyEnable && QGLApplication::GetInstance().GetMapStateID() != ST_APP_LOGIN)
		{
			switch(wParam)
			{
				/*
			case 'I':
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && !gGlobal.g_pBigMap->IsOpenBigMap() )
				{
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenPointFlagPanel", String());
				}
				break;
				*/
			case 'A':
				//ATK
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && !gGlobal.g_pBigMap->IsOpenBigMap() )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBAttack", String());
				}
				else if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE )
				{
					if (!gGlobal.g_pAntiHack->m_bEnterBattle)
					{
						if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
						{
							if (!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() || 
								(gGlobal.g_pBattleControl->GetControlPartner() && !gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand() ) )
								gGlobal.g_pBattleControl->RandAttack();
						}
					}

				}

				break;
			case 'E':
				//Item
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && !gGlobal.g_pBigMap->IsOpenBigMap() )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBItem", String());
				}
				else if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					if (!gGlobal.g_pAntiHack->m_bEnterBattle)
					{
						if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
						{
							if (!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() || 
								(gGlobal.g_pBattleControl->GetControlPartner() && !gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand() ) )
							{
								bool hasOpen = false;
								UIPanel* panel = gGlobal.pUIManager->FindUIPanel("BattleItemMenuPanel");
								if (panel && panel->IsEnable())
									hasOpen = true;
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideAllBattleMenu", String());
								if (!hasOpen)
									CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenBattleItemMenu", String());
							}
						}
					}
				}
				break;
			case 'G':
				//give
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && !gGlobal.g_pBigMap->IsOpenBigMap() )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBGive", String());
				}
				else if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					//catch
					if (!gGlobal.g_pAntiHack->m_bEnterBattle)
					{
						if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
						{
							if (!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand())
							{
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideAllBattleMenu", String());
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BattleCatch", String());
							}
						}
					}
				}
				break;
			case 'P':
				//partner
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && !gGlobal.g_pBigMap->IsOpenBigMap() )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBFriend", String());
				}
				else if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					if (!gGlobal.g_pAntiHack->m_bEnterBattle)
					{
						if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
						{
							if (!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand())
							{
								bool hasOpen = false;

								UIPanel* panel = gGlobal.pUIManager->FindUIPanel("PartnerMainPanel");
								if (panel && panel->IsEnable())
									hasOpen = true;
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideAllBattleMenu", String());
								if (!hasOpen)
									CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BattleFriend", String());
							}
						}
					}
				}
				break;
			case 'C':
				//character
				//if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				//	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideAllBattleMenu");
				if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze() && !gGlobal.g_pBigMap->IsOpenBigMap())
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBChar", String());

				break;
			case 'W':
				//skill
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && !gGlobal.g_pBigMap->IsOpenBigMap() )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBSkill", String());
				}
				else if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					if (!gGlobal.g_pAntiHack->m_bEnterBattle)
					{
						if (gGlobal.g_pBattleControl)
						{
							if (gGlobal.g_pBattleControl->GetControlPlayer() && !gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand())
							{
								UIPanel* panel = gGlobal.pUIManager->FindUIPanel("SkillPanel");
								if (panel && panel->IsEnable())
								{
									CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseSkillPanel", String());
								}
								else
								{
									CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideAllBattleMenu", String());
									CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BattleSkill", String());
								}
							}
							else
							{
								if (gGlobal.g_pBattleControl->GetControlPartner() && 
									!gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand() &&
									gGlobal.g_pPartnerProcess->IsBattleSkillExist() )
								{

									UIPanel* panel = gGlobal.pUIManager->FindUIPanel("PartnerBattleSkillPanel");
									if (panel && panel->IsEnable())
									{
										CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ClosePartnerBattleSkill", String());								
									}
									else
									{
										CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideAllBattleMenu", String());
										CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenBattlePartnerSkillMenu", String());
									}
								}
							}
						}
					}
				}
				break;
			case 'Q':
				//quest
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && !gGlobal.g_pBigMap->IsOpenBigMap() )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBMission", String());
				}
				else if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					if (!gGlobal.g_pAntiHack->m_bEnterBattle)
					{
						if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
						{
							if (!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() || 
								(gGlobal.g_pBattleControl->GetControlPartner() && !gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand() ) )
							{
								gGlobal.g_pBattleControl->RepeatRound();
							}
						}
					}
				}
				break;
			case 'X':
				//trading
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && !gGlobal.g_pBigMap->IsOpenBigMap() )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBTrade", String());
				}
				break;
			case 'S':
				//system
				if (!gGlobal.g_pBigMap->IsOpenBigMap() && 
					(gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NORMAL || gGlobal.g_CursorState->GetStateID() == ST_APP_BATTLE_ATTACK))
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBSystem", String());
					else if (gGlobal.pUIManager->CheckPanelOnTop(gGlobal.pUIManager->FindUIPanel("SystemSettingPanel")))
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseSystemSetting", String());
				}	
				break;
			case 'F':
				//friend
				if (!gGlobal.g_pBigMap->IsOpenBigMap() )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBFriendShip", String());
				}	
				break;
			case 'T':
				//party
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP)
				{
					if (gGlobal.g_pScriptControl &&  
						!gGlobal.g_pScriptControl->IsOpenDialogue() && !gGlobal.g_pBigMap->IsOpenBigMap() && 
						gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())	
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBParty", String());
				}
				else if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					//protect in battle
					if (!gGlobal.g_pAntiHack->m_bEnterBattle)
					{
						if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
						{
							if (!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() || 
								(gGlobal.g_pBattleControl->GetControlPartner() && !gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand() ) )
							{
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideAllBattleMenu", String());
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BattleProtect", String());
							}
						}
					}
				}
				//team
				break;
			case VK_OEM_3:
				//show emotion icon
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PopChatIconPanel", String());
				}
				break;
			case 'B':
				//
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BBHousen", String());
				break;
			case 'Z':
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					if (!gGlobal.g_pAntiHack->m_bEnterBattle)
					{
						if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
						{
							if (!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand())
							{
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideAllBattleMenu", String());
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BattleAuto", String());
							}
						}
					}
				}
				break;
			case 'M':
				//big map
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NORMAL)
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenBigMap", String());
				}
				break;
			case 'H':
				//help in battle
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					if (!gGlobal.g_pAntiHack->m_bEnterBattle)
					{
						if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
						{
							if (!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() && GetHelpEnable())
							{
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideAllBattleMenu", String());
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BattleHelp", String());
							}
						}
					}
				}
				else if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenHelperPanel", String());
				}
				break;
			case 'O':
				//pet 
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenPet", String());
				}
				break;
			case 'N':
				//chargeShop
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "AskForOnlineShop", String());
				}
				break;
			case 'D':
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenQuestJournal", String());
				}
				else if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					//defend in battle
					if (!gGlobal.g_pAntiHack->m_bEnterBattle)
					{
						if (gGlobal.g_pBattleControl && gGlobal.g_pBattleControl->GetControlPlayer())
						{
							if (!gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand() || 
								(gGlobal.g_pBattleControl->GetControlPartner() && !gGlobal.g_pBattleControl->GetControlPartner()->IsSentCommand() ) )
							{
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideAllBattleMenu", String());
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BattleDefence", String());
							}
						}
					}
				}
				break;
			case 'R':
				//special skill
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && !gGlobal.g_pBigMap->IsOpenBigMap() )
				{
					if (gGlobal.g_pHawkerProcess)
					{ 
						if(gGlobal.g_pHawkerProcess->GetAllHawkEnable())
							gGlobal.g_pHawkerProcess->SetAllHawkEnable(false);
						else
							gGlobal.g_pHawkerProcess->SetAllHawkEnable(true);
					}

				}
				else if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
				{
					if (!gGlobal.g_pAntiHack->m_bEnterBattle)
					{
						if (gGlobal.g_pBattleControl)
						{
							if (gGlobal.g_pBattleControl->GetControlPlayer() && !gGlobal.g_pBattleControl->GetControlPlayer()->IsSentCommand())
							{
								UIPanel* panel = gGlobal.pUIManager->FindUIPanel("SkillPanel");
								if (panel && panel->IsEnable())
								{
									CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseSkillPanel", String());
								}
								else
								{
									CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideAllBattleMenu", String());
									CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BattleSpecial", String());
								}
							}
						}
					}
				}
				break;
			case 'L':
				//clear message in chat panel
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ClearMsgDown", String());
				break;
			case 'V':
				{	
					if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP)
					{
						UIPanel* hShortCut = gGlobal.pUIManager->FindUIPanel("HShortCutPanel");
						//UIPanel* vShortCut = gGlobal.pUIManager->FindUIPanel("VShortCutPanel", String());
						if (!IsTalking() && hShortCut)
						{
							if (hShortCut->IsEnable() )
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "EnableHideShortCut", String());
							else 
								CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "EnableHShortCut", String());
						}
					}
				}
				break;
			case 'Y':
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NORMAL )
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenChangeLine", String());
					else if (gGlobal.pUIManager->CheckPanelOnTop(gGlobal.pUIManager->FindUIPanel("ChangeLinePanel")))
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseChangeLine", String());
				}
				break;
			case 'J':
				if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_MAP && gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NORMAL)
				{
					if (gGlobal.pUIManager->GetMonoItem() == NULL && !gGlobal.pUIManager->IsFreeze())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowSearchNPC", String());
				}
				break;
			case VK_OEM_PERIOD:
				{
					int currChannelType = (int)gGlobal.GetCurrChannelType();
					for (UInt8 i = 0; i < MAX_CHANNELTYPE; ++i)
					{
						++currChannelType;

						if (currChannelType >= MAX_CHANNELTYPE)
							currChannelType = 0;

						CChannelControl * pChannelCtrl = gGlobal.GetChannelControl((BYTE)currChannelType);
						if (pChannelCtrl || currChannelType == CHANNELTYPE_MAP)
						{
							gGlobal.SetCurrChannelType((BYTE)currChannelType);
							break;
						}
					}				
				}
				break;
			case VK_OEM_COMMA:
				{
					int currChannelType = (int)gGlobal.GetCurrChannelType();
					for (UInt8 i = 0; i < MAX_CHANNELTYPE; ++i)
					{
						if (currChannelType == 0)
							currChannelType = MAX_CHANNELTYPE;

						--currChannelType;

						CChannelControl * pChannelCtrl = gGlobal.GetChannelControl((BYTE)currChannelType);
						if (pChannelCtrl || currChannelType == CHANNELTYPE_MAP)
						{
							gGlobal.SetCurrChannelType((BYTE)currChannelType);						
							break;
						}
					}		
				}
				break;
			case VK_OEM_MINUS:
				{
					if (!IsTalking() && !gGlobal.g_pBigMap->IsOpenBigMap())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CutChatRow", String());
				}
				break;
			case VK_OEM_PLUS:
				{
					if (!IsTalking() && !gGlobal.g_pBigMap->IsOpenBigMap())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "AddChatRow", String());
				}
				break;
			case '1':
				break;
			case '2':
				break;
			case '3':
				break;
			case '4':
				break;
			case '5':
				break;
			case '6':
				break;
			case '7':
				break;
			case '8':
				break;
			case '9':
				break;
			case VK_F1:
				break;
			case VK_F2:
				break;
			case VK_F3:
				if(CheckMyProcess(m_hWnd) == 1)
				{
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseSubWindowNow", String());  
					QGLApplication::GetInstance().GetRenderClass()->ToggleFullscreen();
				}
				break;

			case VK_F5:
				break;
			case VK_F6:
				break;
			case VK_F7:
				break;
			case VK_F8:

				break;
			case VK_F9:
				break;
			case VK_F10:
				break;
			case VK_F11:
				{
					gConfigure.bEnableNight = !gConfigure.bEnableNight;
					gGlobal.g_DrawAll = TRUE;
				}
				break;
			case VK_ADD:
				{
					if (!IsTalking() && !gGlobal.g_pBigMap->IsOpenBigMap())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "AddChatRow", String());
				}
				break;
			case VK_SUBTRACT:
				{
					if (!IsTalking() && !gGlobal.g_pBigMap->IsOpenBigMap())
						CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CutChatRow", String());
				}
				break;

			case VK_SEPARATOR:
				{

				}
				break;
			case VK_DECIMAL:
				{

				}
				break;
			}



		}
	}
	
	if (gConfigure.uNetwork)
	{
#ifdef REUBEN_PUBLISH
		return;
#endif
	}
	
	if ((GetKeyState(VK_RCONTROL) & (1 << (sizeof(SHORT)*8-1))) == 0)
	{
		return;
	}

	
	//cheat key
	switch(wParam)
	{ 
	case VK_F1:
		gGlobal.BlockingActive = !gGlobal.BlockingActive;
		gGlobal.g_PathFinder->SetBlockingActive(gGlobal.BlockingActive);
		break;
	case VK_F2:
		QGLApplication::GetInstance().ChangeViewingTarget();
		break;
	case VK_F3:
		{
		// this code will be deleted after the testing
        //gGlobal.g_pMenuBankService->OpenDepositInterface();
		}
			
		break;
	case VK_F4:
		// this code will be deleted after the testing
		gGlobal.g_pMenuBankService->OpenWithdrawalInterface();
		break;
	case VK_SPACE:
		QGLApplication::GetInstance().JumpEntity(gGlobal.g_pCharacter->GetCharMainView());
		break;
	case VK_ADD:
		if ((GetKeyState(VK_LCONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0)
		{
			int NoOfTestPlayer = 10;
			for (int i = 0; i < NoOfTestPlayer; i++)
				QGLApplication::GetInstance().AddTestPlayer();
		}
		else
			QGLApplication::GetInstance().AddTestPlayer();
		break;
	case VK_SUBTRACT:
		if ((GetKeyState(VK_LCONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0)
		{
			int NoOfTestPlayer = 10;
			for (int i = 0; i < NoOfTestPlayer; i++)
				QGLApplication::GetInstance().RemoveTestPlayer();
		}
		else
			QGLApplication::GetInstance().RemoveTestPlayer();
		break;

	case 'Q':
		gGlobal.Raining = !gGlobal.Raining;
		if (gGlobal.Raining)
			QGLApplication::GetInstance().AddParticleSystem(
			gGlobal.g_ParticleEffectRain);
		else
			QGLApplication::GetInstance().RemoveParticleSystem(
			gGlobal.g_ParticleEffectRain);

		break;
	case 'W':
		gGlobal.Snowing = !gGlobal.Snowing;
		if (gGlobal.Snowing)
			QGLApplication::GetInstance().AddParticleSystem(gGlobal.g_ParticleEffectSnow);
		else
			QGLApplication::GetInstance().RemoveParticleSystem(gGlobal.g_ParticleEffectSnow);
		break;
	case 'E':
		gGlobal.ShowCloud = !gGlobal.ShowCloud;
		break;
	
	case 'T':
		gGlobal.ShowFog = !gGlobal.ShowFog;
		if (gGlobal.ShowFog)
		{
			gGlobal.g_ParticleEffectFog->SetTarget(gGlobal.g_pCharacter->GetCharMainView());
			QGLApplication::GetInstance().AddParticleSystem(
			gGlobal.g_ParticleEffectFog);
		}
		else
			QGLApplication::GetInstance().RemoveParticleSystem(
			gGlobal.g_ParticleEffectFog);
		break;
	case 'Y':
		if (gGlobal.NightMask)
		{
			gGlobal.Night = !gGlobal.Night;
			gGlobal.g_DrawAll = TRUE;
			QGLApplication::GetInstance().ReleaseNightData();
			if (gGlobal.Night)
			{
				QGLApplication::GetInstance().CreateNightData();
			}
		}

		break;
	case 'A':
		//skill
		gGlobal.g_pBattleControl->SetAnimation(ANIM_SKILL);
		break;
	case 'K':
		//hurt
		gGlobal.g_pBattleControl->SetAnimation(ANIM_HURT);
		break;
	case 'D':
		//die
		gGlobal.g_pBattleControl->SetAnimation(ANIM_DIE);
		break;
	case 'F':
		//def
		gGlobal.g_pBattleControl->SetAnimation(ANIM_DEF);
		break;
	case 'G':
		//use item
		gGlobal.g_pBattleControl->SetAnimation(ANIM_USE);
		break;
	case 'H':
		//sit
		gGlobal.g_pBattleControl->SetAnimation(ANIM_SIT);
		gGlobal.g_pCharacter->GetCharMainView()->SetAnimationIndexByIndex(ANIM_SIT);
		break;
	case 'J':
		QGLApplication::GetInstance().JumpEntity(gGlobal.g_pCharacter->GetCharMainView());
		break;
	case 'L':
		//def
		gGlobal.g_pBattleControl->ResetAnimation();
		break;
	case 'V':
		gGlobal.AddFootStep = !gGlobal.AddFootStep;
		break;
	case 'P':
		if (gGlobal.g_pParty || gGlobal.g_pTempLeaveParty)
			QGLApplication::GetInstance().DisplayPartyInfo();
		else
			QGLApplication::GetInstance().CreateParty();
		break;
	case 'M':
		QGLApplication::GetInstance().ChangePlayerColor256();
		break;
	case 'N':
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenRankPanel", String());
		gGlobal.g_pCharacter->LoadDefaultHeadIcon();
		break;
	case 'O':
		QGLApplication::GetInstance().ResetMapData();
		break;
	case VK_F6:
		gGlobal.DisposalEffect = !gGlobal.DisposalEffect;
		break;
	case VK_F8:
		if ((GetKeyState(VK_LCONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0)
		{
			gGlobal.RandomAccessBattle = !gGlobal.RandomAccessBattle;
		}
		else
		{
			QGLApplication::GetInstance().ChangeBattleMode();
		}
		
		break;
	case VK_F9:
		gGlobal.m_AutoJumppointWalk = !gGlobal.m_AutoJumppointWalk;
		break;
	case 'U':
		gGlobal.g_pCharacter->GetCharMainView()->SetEntityMoveSpeed(gGlobal.g_pCharacter->GetCharMainView()->GetEntityMoveSpeed() + 10);
		//EntityMoveSpeed += 10;
		break;
	case 'B':
		gGlobal.g_pCharacter->GetCharMainView()->SetEntityMoveSpeed(gGlobal.g_pCharacter->GetCharMainView()->GetEntityMoveSpeed() - 10);
		break;
	case '1':
		
		break;
	case '2':
		if (!gConfigure.uNetwork)
		{
			if (ChangeOggFlag)
				::GetFSoundManager()->SetMusic("Data\\music0002_MainTheme02.ogg");
			else
				::GetFSoundManager()->SetMusic("Data\\music0003_MainTheme03.ogg");
			ChangeOggFlag = !ChangeOggFlag;
		}
		
	case '3':
		if (!gConfigure.uNetwork)
		{
			if (ChangeMapFlag)
				gGlobal.TestChangeMap(1);
			else
				gGlobal.TestChangeMap(47);
				
			ChangeMapFlag = !ChangeMapFlag;
		}
		
		break;
	case '4':
		{
			Char buf[50];
			GetPrivateProfileString(_T("LOADFILE"), _T("FIREWORKS"), _T("A"), buf, 50, CONFIGURATION_INI_FILE);
			QGLApplication::GetInstance().PlayFireWorks(buf, gGlobal.g_pCharacter);
			
			
		}
		break;
	case VK_ESCAPE:
		break;
	case VK_NUMPAD5:
		{
			if(strcmp(gGlobal.pUIManager->GetIDName(), "Map") == 0)
			{
				UIPanel *VItemPanel = reinterpret_cast<UIPanel *>(gGlobal.pUIManager->GetUIItem("VItem"));
				UIPanel *HItemPanel = reinterpret_cast<UIPanel *>(gGlobal.pUIManager->GetUIItem("HItem"));


				VItemPanel->SetEnable(!VItemPanel->IsEnable());
				HItemPanel->SetEnable(!VItemPanel->IsEnable());
				
				gGlobal.pUIManager->RenderAll();
			}
		}
		break;
	case VK_NUMPAD4:
		gGlobal.g_pCharacter->GetCharMainView()->SetRotation(WEST);
		break;
	case VK_NUMPAD6:
		gGlobal.g_pCharacter->GetCharMainView()->SetRotation(EAST);
		break;
	case VK_NUMPAD8:
		gGlobal.g_pCharacter->GetCharMainView()->SetRotation(NORTH);
		break;
	case VK_NUMPAD2:
		gGlobal.g_pCharacter->GetCharMainView()->SetRotation(SOUTH);
		break;
	case VK_NUMPAD7:
		gGlobal.g_pCharacter->GetCharMainView()->SetRotation(NORTHWEST);
		break;
	case VK_NUMPAD9:
		gGlobal.g_pCharacter->GetCharMainView()->SetRotation(NORTHEAST);
		break;
	case VK_NUMPAD3:
		gGlobal.g_pCharacter->GetCharMainView()->SetRotation(SOUTHEAST);
		break;
	case VK_NUMPAD1:
		gGlobal.g_pCharacter->GetCharMainView()->SetRotation(SOUTHWEST);
		break;
	case 'R':
		QGLApplication::GetInstance().AddTestImage();
		break;
	}
}



CRender * QGLWindow::GetRenderPtr()
{
	return _pImpl->pRender;
}



