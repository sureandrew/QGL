//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "AppMapState.h"
//-- Library
#include "Global.h"
#include "Configure.h"
#include "QGLApplication.h"
#include "MenuPartyProcess.h"
#include "GenerationProcess.h"

VOID CMapState::Enter(VOID)
{
	if( gGlobal.g_CursorState )
		gGlobal.g_CursorState->Unblock();
	ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
}

VOID CMapState::Exit(VOID)
{
}

VOID CMapUIBaseState::ErrorState(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_ERROR)
	{
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorError.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = FALSE;
	m_SubState = ST_MAPUI_SUB_ERROR;
}

VOID CMapUIBaseState::NormalSate(VOID)
{
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_ERROR;
}
VOID CMapUINormalState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();

	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorNormal.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CMapUINormalState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorNormal.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

}
VOID CMapUINormalState::Exit(VOID)
{
}


VOID CMapUIPartyState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorParty.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_pPartyProcess->HideAllPanel();	
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	
	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(-16);
		gGlobal.pUIManager->SetCursorOffsetY(-16);
	}
	
}

VOID CMapUIPartyState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorParty.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_pPartyProcess->HideAllPanel();	
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
	
}
VOID CMapUIPartyState::Exit(VOID)
{
	gGlobal.g_pPartyProcess->ShowAllPanel();
}


VOID CBattleUIAttackState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorAttack.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CBattleUIAttackState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorAttack.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CBattleUIAttackState::Exit(VOID)
{
}

VOID CMapUINPCState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorNPC.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CMapUINPCState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorNPC.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CMapUINPCState::Exit(VOID)
{
}

VOID CMapUINPCTalkingState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.LeftClickStatus = LEFTCLICK_NORMAL;
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorNextPage.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CMapUINPCTalkingState::NormalSate(VOID)
{
	gGlobal.LeftClickStatus = LEFTCLICK_NORMAL;
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorNextPage.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CMapUINPCTalkingState::Exit(VOID)
{
}

VOID CBattleUIProtectState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorProtect.c_str(), gGlobal.pD3DDevice);
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CBattleUIProtectState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorProtect.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}
VOID CBattleUIProtectState::Exit(VOID)
{
}

VOID CBattleUIUseItemState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorBattleItem.c_str(), gGlobal.pD3DDevice);
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CBattleUIUseItemState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorBattleItem.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CBattleUIUseItemState::Exit(VOID)
{
}

VOID CBattleUISkillState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorSkill.c_str(), gGlobal.pD3DDevice);
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CBattleUISkillState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorSkill.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CBattleUISkillState::Exit(VOID)
{
}

VOID CMapAttackState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorAttack.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_pPartyProcess->HideAllPanel();
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}

}

VOID CMapAttackState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorAttack.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_pPartyProcess->HideAllPanel();
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

}

VOID CMapAttackState::Exit(VOID)
{
	gGlobal.g_pPartyProcess->ShowAllPanel();
}

VOID CBattleUICatchState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorCatch.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CBattleUICatchState::Exit(VOID)
{

}

VOID CBattleUICatchState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorCatch.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CShopSellState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorSell.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CShopSellState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorSell.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CShopSellState::Exit(VOID)
{
}

VOID CMapGivenState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorGiven.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_pPartyProcess->HideAllPanel();
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CMapGivenState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorGiven.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_pPartyProcess->HideAllPanel();
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CMapGivenState::Exit(VOID)
{
	gGlobal.g_pPartyProcess->ShowAllPanel();
}

VOID CBigMapDescState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorBigMapDesc.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CBigMapDescState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorBigMapDesc.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CBigMapDescState::Exit(VOID)
{
	
}

VOID CFriendState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorFriend.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CFriendState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorFriend.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CFriendState::Exit(VOID)
{
	
}

VOID CMapTradingState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorTrading.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_pPartyProcess->HideAllPanel();
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CMapTradingState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorTrading.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_pPartyProcess->HideAllPanel();
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CMapTradingState::Exit(VOID)
{
	gGlobal.g_pPartyProcess->ShowAllPanel();
}

VOID CAssetLockState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorAssetLock.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CAssetLockState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorAssetLock.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CAssetLockState::Exit(VOID)
{
}


VOID CAssetUnLockState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorAssetUnLock.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;
}

VOID CAssetUnLockState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorAssetUnLock.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(0);
		gGlobal.pUIManager->SetCursorOffsetY(0);
	}
}

VOID CAssetUnLockState::Exit(VOID)
{
}
VOID CMapUIGenerationState::Enter(VOID)
{
	gGlobal.MarkMouseDirtyRect();
	gGlobal.CursorEntity->InitEntPicInfo();
	gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorGeneration.c_str(), gGlobal.pD3DDevice);
	gGlobal.g_pGenerationProcess->HideAllPanel();	
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;


	if(gGlobal.pUIManager)
	{
		gGlobal.pUIManager->SetCursorOffsetX(-16);
		gGlobal.pUIManager->SetCursorOffsetY(-16);
	}

}

VOID CMapUIGenerationState::NormalSate(VOID)
{
	if (m_SubState != ST_MAPUI_SUB_NORMAL)
	{
		gGlobal.MarkMouseDirtyRect();
		gGlobal.CursorEntity->InitEntPicInfo();
		gGlobal.CursorEntity->LoadEntityFile(gConfigure.strCursorGeneration.c_str(), gGlobal.pD3DDevice);
	}
	gGlobal.g_pPartyProcess->HideAllPanel();	
	gGlobal.g_ClickEnable = TRUE;
	m_SubState = ST_MAPUI_SUB_NORMAL;

}
VOID CMapUIGenerationState::Exit(VOID)
{
	gGlobal.g_pGenerationProcess->ShowAllPanel();
}