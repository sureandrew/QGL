//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MapNPCControl.h"
//-- Library
#include "QGLApplication.h"
#include "CharacterControl.h"
#include "Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "common_binary.h"
#include "UI.h"
#include "AnimatedEntity.h"
#include "Math.h"
#include "Proxy/Logic/Party.h"
#include "ScriptControl.h"

CMapNPCControl::CMapNPCControl()
{
	m_NpCData = NULL;
	m_MapActorType = MAPACTORTYPE_NPC;
}

CMapNPCControl::~CMapNPCControl()
{
	if (m_pMapNPC)
		m_pMapNPC->SetMapNPCControl(NULL);
	
	if( this == QGLApplication::GetInstance().GetSelectedMapControl() )
	{
		gGlobal.g_CursorState->Unblock();
		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}
}


VOID CMapNPCControl::SetMapActorObject(CMapActor* pMapActor)
{
	m_pMapActor = pMapActor;
	m_pMapNPC = CastNetObject<CMapNPC>(pMapActor);
	CMapActorControl::SetMapActorObject(pMapActor);
	if (m_pMapNPC)
		m_pMapNPC->SetMapNPCControl(this);
}

UInt32 CMapNPCControl::GetMapActorID()
{
	if (m_pMapNPC)
	{
		return m_pMapNPC->GetNpc_uid();
	}
	return 0;
}

UInt32 CMapNPCControl::GetTemplateID()
{
	if (m_pMapNPC)
	{
		return m_pMapNPC->GetNpc_id();
	}
	return 0;
}

Boolean CMapNPCControl::OnClickOnCtrl()
{
	if (IsHawking() || gGlobal.g_pParty && !gGlobal.g_pParty->GetAvail())
		return BOOLEAN_FALSE;

	if ( gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NPC || 
			gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NORMAL )
	{
		{
			// change npc direction
			Int direction = QGLMath::GetRotationFromTwoPoint(this->GetCharMainView()->GetCharPos().PosX,
				this->GetCharMainView()->GetCharPos().PosY, gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX,
				gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY);
			this->ChangeRotation(direction);

			// change player direction 
			gGlobal.g_pCharacter->ChangeRotation(QGLMath::GetOppRotation(direction));
		}

		if ( m_pMapNPC )
		{
			if ( State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINPCTalkingState)) )
			{
				gGlobal.g_CursorState->Block();

				// lock pos 
				if (gGlobal.g_pCharacter)
					gGlobal.g_pCharacter->Stop();

				gGlobal.g_pScriptControl->StartUp();
				gGlobal.g_pScriptControl->OnLockPosHandle(BOOLEAN_TRUE);

				//m_pMapNPC->OnClick(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_pCharacter->GetMapActorID());
				gGlobal.g_pCharacter->GetCharNetObject()->TalkToNPC(gGlobal.pPeerProxy, gGlobal.g_context, m_pMapNPC->GetNpc_uid());
				return BOOLEAN_TRUE;
			}
		}
	}

	return BOOLEAN_FALSE;
}

VOID CMapNPCControl::OnCB_ClickOnCtrl(Boolean bResult)
{
	if ( !bResult )
	{// click npc reply failed
		StopNPCTalkingState();

		if ( gGlobal.g_pScriptControl ) 
		{
			gGlobal.g_pScriptControl->ResetNPCDirection();
			gGlobal.g_pScriptControl->OnLockPosHandle(BOOLEAN_FALSE);
		}

		{
			String Msg =gGlobal.GetStringTable()->Get(_T("MSG_SCRIPT_NPCNOTREPLY"));
			gGlobal.PrintPlayerMessage(Msg);
			//MSG_SCRIPT_NPCNOTREPLY
			//gGlobal.PrintMessage(Msg, CHANNELTYPE_SYSTEM);
		}
	}
}

VOID CMapNPCControl::StopNPCTalkingState()
{
	if ( gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_TALKING )
	{
		if ( gGlobal.g_CursorState->IsBlocked() )
			gGlobal.g_CursorState->Unblock();

		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}

	gGlobal.g_pScriptControl->SetFinish();
}


VOID CMapNPCControl::ResponseExecuteAllCtrl(String filename)
{
	gGlobal.g_pScriptControl->SetDisappearCenter(this->GetCharMainView()->GetCharPos().PosX,
		this->GetCharMainView()->GetCharPos().PosY);
	//gGlobal.m_OpenDialoguePosX = this->GetCharMainView()->GetCharPos().PosX;
	//gGlobal.m_OpenDialoguePosY = this->GetCharMainView()->GetCharPos().PosY;

	CMapActorControl* pControl = dynamic_cast<CMapActorControl*>(this);
	if ( pControl )
	{
		gGlobal.g_bClickNPCFlag = TRUE;
		QGLApplication::GetInstance().SetClickNPCMapControl(pControl);
		QGLApplication::GetInstance().UpdateWholeMemberClickNPC(filename);
	}
}

VOID CMapNPCControl::CancelTalkingTargetHandle()
{
	if ( m_pMapNPC )
	{
		CMapActorControl* pcontrol = QGLApplication::GetInstance().GetTalkingMapControl();
		if ( pcontrol )
		{
			CMapNPCControl* pnpccontrol = dynamic_cast<CMapNPCControl*>(pcontrol);
			if ( pnpccontrol )
			{
				if ( m_pMapNPC->GetNpc_uid() == pnpccontrol->GetMapActorID() )
				{
					QGLApplication::GetInstance().SetTalkingMapControl(NULL);
				}
			}
		}
	}
}

void CMapNPCControl::SetEnable(Boolean enable)
{
	if (m_pMapNPC)
	{
		GetCharMainView()->SetVisible(enable);
		GetCharNameView()->SetVisible(enable);
		GetCharShadowView()->SetVisible(enable);
		GetHeadIconView()->SetVisible(enable);
		GetCharFrontNameView()->SetVisible(enable);
	}
}