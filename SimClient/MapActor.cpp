//-- Common
#include "Common.h"
#include "Common/PathFinder.h"
//-- Self
#include "Proxy/Logic/MapActor.h"
#include "SimClient/Map.h"
//-- Library
#include "SimClient.h"
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CMapActor, MapActor_CLASSID);

const Int DefaultSpeed = 150;
const Int BlockSizeX = 32;
const Int BlockSizeY = 16;
const UInt32 X_MOVE_TIME = 32000;					// move time (ms) along x 
const UInt32 Y_MOVE_TIME = 16000;					// move time (ms) along y
const UInt32 XY_MOVE_TIME = 17000;					// move time (ms) along x-y

#define ABS_DIFF(X, Y) ((X) > (Y) ? (X) - (Y) : (Y) - (X))

CMapActor::CMapActor()
{
	m_pMap = NULL;
	m_nextStep.PosX = 0;
	m_nextStep.PosY = 0;
	m_preMoveTime = ::GetTickCount();
}

CMapActor::~CMapActor()
{
}

void CMapActor::OnUpdateInfo(const String &nickName, const UInt8 &sex, 
							 const UInt8 &cclass, const UInt8 &sexClass, 
							 const UInt8 &faction)
{
	if (this == (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
	{
		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("Update MapActor %d: classID %d, netId %d,%d, objId %d"), 
			GetActorId(), GetClassID(), GetNetId().srvId, GetNetId().objId, GetID());
		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("Update MapActor %d: sex %d, cclass %d, sc %d, faction %d"), 
			GetActorId(), sex, cclass, sexClass, faction);
	}
}

void CMapActor::OnUpdateState(const UInt8 &state)
{
	if (this == (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Update MapActor %d: state %d"),
			GetActorId(), state);
}

void CMapActor::OnUpdateMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt8 &direction, const UInt32 &map_ownerId)
{
	if (this == (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Update MapActor %d: map %d, x %d, y %d"), GetActorId(),
			map_id, map_x, map_y);
}

void CMapActor::OnUpdateReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y)
{
	if (this == (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Update MapActor %d: reborn map %d, x %d, y %d"), 
			GetActorId(), reborn_map, reborn_x, reborn_y);
}

void CMapActor::OnUpdateCurPos(const UInt16 &curPosX, const UInt16 &curPosY)
{
	if (this == (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Update MapActor %d: curPos %d,%d"), 
			GetActorId(), curPosX, curPosY);
	m_curPos.PosX = curPosX;
	m_curPos.PosY = curPosY;
}

void CMapActor::OnUpdateDstPos(const UInt16 &dstPosX, const UInt16 &dstPosY)
{
	if (this == (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Update MapActor %d: dstPos %d,%d"), 
			GetActorId(), dstPosX, dstPosY);
	m_preMoveTime = ::GetTickCount();
}

void CMapActor::OnUpdateAction(const UInt16 &action, const UInt32 &actionTime)
{
	if (this == (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Update MapActor %d: action %d, time %d"), 
			GetActorId(), action, actionTime);

	// detect battle lock count down time
	if (this == (CMapActor*) gGlobal.m_pSimClient->m_pCharacter &&
		gGlobal.m_pSimClient->m_pBattle == NULL)
	{
		if (action == ST_BATTLE || action == ST_MONITOR)
			gGlobal.m_pSimClient->m_batDeadlock = BATTLE_DEADLOCK_TIME;
		else
			gGlobal.m_pSimClient->m_batDeadlock = 0;
	}
}

void CMapActor::OnUpdateSpeed(const UInt32 &speed)
{
	if (this == (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Update MapActor %d: speed %d"), 
			GetActorId(), speed);
}

void CMapActor::OnUpdateFollowId(const UInt32 &followId, const UInt8 &followOrder)
{
	if (this == (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Update MapActor %d: followId %d, order %d"),
			GetActorId(), followId, followOrder);
	if (followId > 0)
		m_path.clear();
}

void CMapActor::OnUpdateAppeared(const Boolean &appeared)
{
	if (this == (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != (CMapActor*) gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Update MapActor %d: %s"),
			GetActorId(), appeared ? _T("Appeared") : _T("Disappeared"));
}

UInt16 CMapActor::GetHint1() const
{
	if (m_pMap && m_pMap->GetPathFinder())
		return m_pMap->GetPathFinder()->GetMinGuild1();
	else
		return 0;
}

UInt16 CMapActor::GetHint2() const
{
	if (m_pMap && m_pMap->GetPathFinder())
		return m_pMap->GetPathFinder()->GetMinGuild2();
	else
		return 0;
}

void CMapActor::Move(LPCPEER pPeer, RPCContext &context, UInt16 x, UInt16 y)
{
	if (m_pMap == NULL || (GetAction() != ST_MOVE && GetAction() != ST_STOP)) {
		return;
	}

	PathFinder *pf = m_pMap->GetPathFinder();
	if (pf)
		pf->GetWholePath(m_curPos.PosX, m_curPos.PosY, x, y, m_path);
}

INLINE UInt32 CMapActor::GetMoveTime(UInt16 x, UInt16 y, UInt32 speed)
{
	UInt16 dx = ABS_DIFF(x, m_nextStep.PosX);
	UInt16 dy = ABS_DIFF(y, m_nextStep.PosY);

	if (dx == 1 && dy == 0)					// move x
		return X_MOVE_TIME / speed;
	else if (dx == 0 && dy == 2)			// move y
		return Y_MOVE_TIME / speed;
	else 									// move diagonal
		return XY_MOVE_TIME / speed;
}

void CMapActor::NextMove()
{
	// move state
	if (m_pMap == NULL || GetAction() != ST_MOVE || m_path.empty())
		return;

	bool moved = false;
	UInt32 curTime = ::GetTickCount();
	UInt32 dt = curTime - m_preMoveTime;
	UInt32 mt = GetMoveTime(m_curPos.PosX, m_curPos.PosY, GetSpeed());
	int jumpIdx = -1;
	UInt32 targetNpc = 0;
	BlockPos newPos;

	while (dt >= mt)
	{
		newPos = m_nextStep;
		if (!moved)
			moved = true;
		m_preMoveTime += mt;

		// if reach destination or no move path to move
		if (newPos.PosX == GetDstPosX() && newPos.PosY == GetDstPosY() ||
			m_path.empty())
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CMapActor Stopped: %d,%d"), newPos.PosX, newPos.PosY);
			m_path.clear();
			return;
		}

		// set next move position
		if (!m_path.empty())
		{
			m_nextStep = m_path.front();
			m_path.pop_front();
		}
		dt -= mt;
		mt = GetMoveTime(newPos.PosX, newPos.PosY, GetSpeed());
	}

	if (moved) {
		m_curPos = newPos;		// update to next position
		//TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CMapActor %d Moved: %d,%d"), 
		//	GetActorId(), newPos.PosX, newPos.PosY);
	}
}
