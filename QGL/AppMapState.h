#pragma once
#ifndef _QGL_APPMAPSTATE_H_
#define _QGL_APPMAPSTATE_H_
#include "Common.h"

enum ApplicationStateEnum
{
	ST_APP_LOGIN = 0,
	ST_APP_MAP,
	ST_APP_BATTLE,
	ST_APP_HAWK,
};

enum ApplicationMapUIStateEnum
{
	ST_APP_MAP_BASE = 0,
	ST_APP_MAP_NORMAL,
	ST_APP_MAP_PARTY,
	ST_APP_BATTLE_ATTACK,
	ST_APP_MAP_NPC,
	ST_APP_MAP_TALKING,
	ST_APP_BATTLE_PROTECT,
	ST_APP_BATTLE_USEITEM,
	ST_APP_BATTLE_SKILL,
	ST_APP_BATTLE_CATCH,
	ST_APP_MAP_PK_ATTACK,
	ST_APP_SHOP_SELL,
	ST_APP_MAP_GIVEN,
	ST_APP_BIGMAP_DESC,
	ST_APP_MAP_TRADING,
	ST_APP_FRIEND,
	ST_APP_ASSET_LOCK,
	ST_APP_ASSET_UNLOCK,	
	ST_APP_MAP_GENERATION,
};

enum MapUISubState
{
	ST_MAPUI_SUB_ERROR = 0,
	ST_MAPUI_SUB_NORMAL,
};
class CLoginState : public State
{
	DEFINE_STATE(ST_APP_LOGIN);
};

class CMapState : public State
{
public:
	DEFINE_STATE(ST_APP_MAP);

protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
};

class CBattleState : public State
{
	DEFINE_STATE(ST_APP_BATTLE);
};

class CHawkState : public State
{
	DEFINE_STATE(ST_APP_HAWK);
};

class CCharLoginState : public State
{
	DEFINE_STATE(ST_APP_LOGIN);
};

class CCharMapState : public State
{
	DEFINE_STATE(ST_APP_MAP);
};

class CCharBattleState : public State
{
	DEFINE_STATE(ST_APP_BATTLE);
};

class CMapUIBaseState :public State
{
public:
	DEFINE_STATE(ST_APP_MAP_BASE);
	virtual VOID ErrorState(VOID);
	virtual VOID NormalSate(VOID);
protected:
	BYTE m_SubState;
	
};
class CMapUINormalState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_MAP_NORMAL);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);

	
};

class CMapUIPartyState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_MAP_PARTY);

	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL)
			return FALSE;
		return TRUE;
	}
	
};

class CBattleUIAttackState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_BATTLE_ATTACK);

	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL)
			return FALSE;
		return TRUE;
	}
	
};

class CMapUINPCState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_MAP_NPC);

	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL)
			return FALSE;
		return TRUE;
	}
	
};

class CMapUINPCTalkingState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_MAP_TALKING);

	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId == ST_APP_MAP_NPC || currentId == ST_APP_MAP_NORMAL)
			return TRUE;
		return FALSE;
	}
	
};

class CBattleUIProtectState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_BATTLE_PROTECT);

	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL && currentId != ST_APP_BATTLE_ATTACK)
			return FALSE;
		return TRUE;
	}
	
};

class CBattleUIUseItemState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_BATTLE_USEITEM);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL)
			return FALSE;
		return TRUE;
	}
};

class CBattleUISkillState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_BATTLE_SKILL);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL)
			return FALSE;
		return TRUE;
	}
};

class CBattleUICatchState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_BATTLE_CATCH);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL && currentId != ST_APP_BATTLE_ATTACK)
			return FALSE;
		return TRUE;
	}
};

class CMapAttackState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_MAP_PK_ATTACK);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL)
			return FALSE;
		return TRUE;
	}
};

class CShopSellState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_SHOP_SELL);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL)
			return FALSE;
		return TRUE;
	}
};

class CMapGivenState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_MAP_GIVEN);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL)
			return FALSE;
		return TRUE;
	}
};

class CBigMapDescState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_BIGMAP_DESC);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL)
			return FALSE;
		return TRUE;
	}
};

class CMapTradingState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_MAP_TRADING);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL)
			return FALSE;
		return TRUE;
	}
};
class CFriendState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_FRIEND);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if (currentId != ST_APP_MAP_NORMAL)
			return FALSE;
		return TRUE;
	}
};

class CAssetLockState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_ASSET_LOCK);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if ((currentId != ST_APP_MAP_NORMAL) && (currentId != ST_APP_ASSET_UNLOCK))
			return FALSE;
		return TRUE;
	}
};

class CAssetUnLockState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_ASSET_UNLOCK);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if ((currentId != ST_APP_MAP_NORMAL) && (currentId != ST_APP_ASSET_LOCK))
			return FALSE;
		return TRUE;
	}
};

class CMapUIGenerationState : public CMapUIBaseState
{
public:
	DEFINE_STATE(ST_APP_MAP_GENERATION);
	virtual VOID NormalSate(VOID);
protected:
	virtual VOID Enter(VOID);
	virtual VOID Exit(VOID);
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		if ((currentId != ST_APP_MAP_NORMAL) && (currentId != ST_APP_MAP_GENERATION))
			return FALSE;
		return TRUE;
	}
};
#endif
