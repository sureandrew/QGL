//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/MapActor.h"
//-- Library
#include "Stub/Logic/Map.h"
#include "Stub/Service/MapService.h"
#include "MapService/Global.h"
#include "MapService/MapApplication.h"
#include "MapService/Math_QGL.h"
#include <algorithm>

#define INVALID_BLOCK_POS		UINT16_MAX_VALUE
const Int DefaultSpeed = 150;
const Int BlockSizeX = 32;
const Int BlockSizeY = 16;
const UInt32 X_MOVE_TIME = 32000;					// move time (ms) along x 
const UInt32 Y_MOVE_TIME = 16000;					// move time (ms) along y
const UInt32 XY_MOVE_TIME = 17000;					// move time (ms) along x-y

REGISTER_CLASSTYPE(CMapActor, MapActor_CLASSID);

#define ABS_DIFF(X, Y) ((X) > (Y) ? (X) - (Y) : (Y) - (X))

CONST Boolean CMapActor::CMoveState::CanTransitionFrom(CONST StateID currentId) CONST
{
	return (currentId != ST_FOLLOW && m_actor->GetMap() && m_actor->IsState(ST_ENTER_MAP));
}

void CMapActor::CLeaveState::Enter(VOID)
{
	if (m_actor->m_moveState) {
		//m_actor->Stop();
		if (m_actor->m_moveState->GetStateID() != ST_STOP)
			m_actor->ChangeState(ST_STOP);
		m_actor->m_moveState->Block();
	}
}

void CMapActor::CLeaveState::Exit(VOID)
{
	m_actor->m_moveState->Unblock();
}

void CMapActor::CJumpState::Enter(VOID)
{
	if (m_actor->m_moveState)
	{
		if (m_actor->m_moveState->GetStateID() != ST_STOP)
			m_actor->ChangeState(ST_STOP);
		m_actor->m_moveState->Block();
	}
}

void CMapActor::CJumpState::Exit(VOID)
{
	m_actor->m_moveState->Unblock();
}

void CMapActor::CMoveState::Enter(VOID)
{
	TRACE_ENSURE(m_actor->GetMap());
	m_actor->GetMap()->AttachProcess(this);
}

void CMapActor::CMoveState::Exit(VOID)
{
	TRACE_ENSURE(m_actor->GetMap());
	m_actor->GetMap()->DetachProcess(this);
}

void CMapActor::CMoveState::OnUpdate(CONST UInt32 dt)
{
	m_actor->NextMove();		// self move when not following
}

CMapActor::CMapActor()
{
	m_pMap = NULL;
	m_mapState = NULL;
	m_moveState = NULL;
	ChangeState(ST_LEAVE_MAP);
	ChangeState(ST_STOP);
	m_pFollowTarget = NULL;

	m_preMoveTime = 0;
	m_nextStep.PosX = 0;
	m_nextStep.PosY = 0;
	m_baseSpeed = DefaultSpeed;
	m_curRegion = UINT16_MAX_VALUE;
	m_newRegion = UINT16_MAX_VALUE;

	SetSpeed(DefaultSpeed);
	SetFollowId(0, 0);
}

CMapActor::~CMapActor()
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, 
		_T("CMapActor::~CMapActor: netid=(%d:%d)"), 
		GetNetId().srvId, GetNetId().objId);

	// unknown checking by payne
	//TRACE_ENSURE(m_moveState->GetStateID() != ST_MOVE);
	//OnLeaveMap(); only for safety
	State::ChangeState(m_mapState, NULL);
	State::ChangeState(m_moveState, NULL);
}

Boolean CMapActor::ChangeState(CONST StateID state)
{
	switch (state)
	{
	case ST_ENTER_MAP:
		return State::ChangeState(m_mapState, SafeCreateObject(CEnterState));
	case ST_LEAVE_MAP:
		return State::ChangeState(m_mapState, SafeCreateObject1(CLeaveState, this));
	case ST_JUMP_MAP:
		return State::ChangeState(m_mapState, SafeCreateObject1(CJumpState, this));
	case ST_STOP:
		return State::ChangeState(m_moveState, SafeCreateObject(CStopState));
	case ST_MOVE:
		return State::ChangeState(m_moveState, SafeCreateObject1(CMoveState, this));
	case ST_FOLLOW:
		return State::ChangeState(m_moveState, SafeCreateObject(CFollowState));
	}

	return BOOLEAN_FALSE;
}

Boolean CMapActor::IsState(CONST StateID state)
{
	switch (state)
	{
	case ST_ENTER_MAP:
	case ST_LEAVE_MAP:
	case ST_JUMP_MAP:
		return (m_mapState->GetStateID() == state);
	case ST_STOP:
	case ST_MOVE:
	case ST_FOLLOW:
		return (m_moveState->GetStateID() == state);
	}

	return BOOLEAN_FALSE;
}

Boolean CMapActor::TryState(CONST StateID state)
{
	switch (state)
	{
	case ST_ENTER_MAP:
		return State::TestChangeState(m_mapState, SafeCreateObject(CEnterState));
	case ST_LEAVE_MAP:
		return State::TestChangeState(m_mapState, SafeCreateObject1(CLeaveState, this));
	case ST_JUMP_MAP:
		return State::TestChangeState(m_mapState, SafeCreateObject1(CJumpState, this));
	case ST_STOP:
		return State::TestChangeState(m_moveState, SafeCreateObject(CStopState));
	case ST_MOVE:
		return State::TestChangeState(m_moveState, SafeCreateObject1(CMoveState, this));
	case ST_FOLLOW:
		return State::TestChangeState(m_moveState, SafeCreateObject(CFollowState));
	}

	return BOOLEAN_FALSE;
}

void CMapActor::OnEnterMap(CMap* pmap)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, 
		_T("CMapActor::OnEnterMap : [OnEnterMap AddActor] netid=(%d:%d)"), 
		GetNetId().srvId, GetNetId().objId);

	m_pMap = pmap;
	pmap->AddActor(this);
}

void CMapActor::OnLeaveMap()
{
	if (m_pMap == NULL) return;

	// invalid map
	if (m_pMap != FindNetObject<CMap>(m_pMap->GetNetId()))
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("[invalid map], map %u, netId %d:%d"),
			m_pMap, m_pMap->GetNetId().srvId, m_pMap->GetNetId().objId);
		m_pMap = NULL;
		return;
	}

	if (m_pFollowTarget)
		StopFollow();
	else
		ChangeState(ST_STOP);

	// clear all follower
	CMapActor* follower;
	MapActorList followerList;

	m_followerList.swap(followerList);
	for (MapActorList::iterator it = followerList.begin(); it != followerList.end(); ++it)
	{
		follower = *it;
		if (follower && follower->GetFollowTarget() == this)
			follower->StopFollow();
	}

 	m_followerList.clear();

	TRACE_INFODTL_2(GLOBAL_LOGGER, 
		_T("CMapActor::OnLeaveMap : [Removal RemoveActor] netid=(%d:%d)"), 
		GetNetId().srvId, GetNetId().objId);

	m_pMap->RemoveActor(this);
	m_pMap = NULL;
}

Boolean CMapActor::Move(const UInt16 &posX, const UInt16 &posY, 
						const UInt16 &hint1, const UInt16 &hint2)
{
	// check if move to same destination position, do nothing or try to stop
	if (IsState(ST_MOVE) && posX == GetDstPosX() && posY == GetDstPosY())
		return false;

	if (m_pMap == NULL || m_pMap->GetPathFinder() == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CMapActor MoveTo Error: invalid map or path finder"));
		return false;
	}

	// check if out range of map
	if (posX >= m_pMap->GetWidth() || posY >= m_pMap->GetHeight()) {
		TRACE_ERRORDTL_5(GLOBAL_LOGGER, _F("CMapActor MoveTo Error: move %d:%d out of map %d range %d:%d"), 
			posX, posY, m_pMap->GetMap_id(), m_pMap->GetWidth(), m_pMap->GetHeight());
		return false;
	}

	if (!TryState(ST_MOVE))
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CMapActor MoveTo Error: cannot move at this moment"));
		return false;
	}

	// change to new path
	m_pMap->GetPathFinder()->GetWholePath(GetCurPosX(), GetCurPosY(), 
		posX, posY, m_path, hint1, hint2);
	if (m_path.empty())				// empty new path
		return true;

	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("CMapActor %d MoveTo: %d:%d -> %d:%d (%d)"),
		GetActorId(), GetCurPosX(), GetCurPosY(), posX, posY, GetSpeed());
	
	if (!IsState(ST_MOVE))
	{		// when not moving before
		// can change to move state?
		UInt32 curTime = ::timeGetTime();
		if (!ChangeState(ST_MOVE))
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CMapActor MoveTo Error: cannot move at this moment"));
			return false;
		}

		if (curTime > m_preMoveTime)
			m_preMoveTime = curTime;
	}
	m_nextStep = m_path.front();			// take first position as next step
	m_path.erase(m_path.begin());

	SetDstPos(posX, posY);
	SetAction(ST_MOVE, ::timeGetTime());

	return true;
}

Boolean CMapActor::Stop()
{
	if (m_moveState->GetStateID() != ST_STOP)
	{
		if (!TryState(ST_STOP))			// test if can stop
			return BOOLEAN_FALSE;

		// advance step and time when stop
		UInt32 actionTime;
		if (GetCurPosX() != m_nextStep.PosX || GetCurPosY() != m_nextStep.PosY)
		{
			m_preMoveTime += GetMoveTime(GetCurPosX(), GetCurPosY(), GetSpeed());
			actionTime = m_preMoveTime;
			ChangeCurPos(m_nextStep.PosX, m_nextStep.PosY);
		}
		else
		{
			actionTime = ::timeGetTime();
		}
		// clear path to stop moving
		m_path.clear();

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CMapActor %d Stopped: %d:%d"), GetActorId(), GetCurPosX(), GetCurPosY());

		ChangeState(ST_STOP);
		SetDstPos(GetCurPosX(), GetCurPosY());
		SetAction(ST_STOP, actionTime);
	} 

	return BOOLEAN_TRUE;
}

UInt8 CMapActor::GetFollowerCount()
{
	return ((UInt8) m_followerList.size());
}

void CMapActor::FollowTarget(CMapActor *target)
{
	if (target == NULL) return;

	if (target->GetMap_id() != GetMap_id())
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("CMapActor FollowTarget Warning: cannot follow target(%d) in map %d but this actor(%d) in map %d"), 
			target->GetID(), target->GetMap_id(), GetID(), GetMap_id());
		return;
	}

	if (m_pFollowTarget)
	{
		m_pFollowTarget->RemoveFollower(this);
		m_pFollowTarget = NULL;
	}

	m_pFollowTarget = target;
	ChangeCurPos(target->GetCurPosX(), target->GetCurPosY());
	SetDstPos(target->GetCurPosX(), target->GetCurPosY());
	UnmarkUpdate(dstPosMask);						// won't update to client
	m_nextStep.PosX = target->GetCurPosX();
	m_nextStep.PosY = target->GetCurPosY();
	m_path.clear();
	target->AddFollower(this);
	SetFollowId(target->GetActorId(), target->GetFollowerCount());
	ChangeState(ST_FOLLOW);
}

void CMapActor::StopFollow()
{
	if (!m_pFollowTarget) return;

	Boolean changed = false;
	if (m_moveState->IsBlocked())
	{
		m_moveState->Unblock();
		changed = ChangeState(ST_STOP);
		m_moveState->Block();
	}
	else
	{
		changed = ChangeState(ST_STOP);
	}

	//if (!changed) return;
	if (m_pFollowTarget->GetMap_id() != GetMap_id())
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("CMapActor StopFollow Warning: follow target(%d) in map %d but this actor(%d) in map %d"), 
			m_pFollowTarget->GetID(), m_pFollowTarget->GetMap_id(), GetID(), GetMap_id());
	}
	else
	{
		ChangeCurPos(m_pFollowTarget->GetCurPosX(), m_pFollowTarget->GetCurPosY());
	}
	SetDstPos(GetCurPosX(), GetCurPosY());
	m_nextStep.PosX = GetCurPosX();
	m_nextStep.PosY = GetCurPosY();
	m_pFollowTarget->RemoveFollower(this);
	m_pFollowTarget = NULL;
	SetFollowId(0, 0);
	//RaiseUniqEventID(EVT_ATTRIB_UPDATE);
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
	// move blocked
	if (m_moveState->IsBlocked())
		return;

	TRACE_CHECK(m_pMap);

	bool moved = false;
	UInt32 curTime = ::timeGetTime();
	UInt32 dt = curTime - m_preMoveTime;
	UInt32 mt = GetMoveTime(GetCurPosX(), GetCurPosY(), GetSpeed());
	int jumpIdx = -1;
	UInt32 targetNpc = 0;
	BlockPos newPos;

	while (dt >= mt)
	{
		newPos = m_nextStep;
		if (!moved)
			moved = true;
		m_preMoveTime += mt;

		// check if reach jump point
		jumpIdx = m_pMap->IsJumpPoint(m_nextStep.PosX, m_nextStep.PosY);
		if (jumpIdx != -1)
			break;

		// check if battle
		targetNpc = CheckBattle();
		if (targetNpc > 0)
			break;
		
		// stop if reach destination, end of path or block by map object
		if (newPos.PosX == GetDstPosX() && newPos.PosY == GetDstPosY() ||
			m_path.empty() || m_pMap->IsObjBlocked(newPos))
		{
			if (!ChangeState(ST_STOP))
			{
				TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("CMapActor NextMove Error: cannot stop at last move %d:%d action %d"),
					newPos.PosX, newPos.PosY, GetAction());
				break;
			}
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CMapActor %d Stopped: %d:%d"), GetActorId(), newPos.PosX, newPos.PosY);
			SetAction(ST_STOP, ::timeGetTime());
			break;
		}

		// set next move position
		if (!m_path.empty())
		{
			m_nextStep = m_path.front();
			//m_path.erase(m_path.begin());
			m_path.pop_front();
		}
		dt -= mt;
		mt = GetMoveTime(newPos.PosX, newPos.PosY, GetSpeed());
	}

	// follower update
	if (moved)
	{
		ChangeCurPos(newPos.PosX, newPos.PosY);		// update to next position
		for (MapActorList::iterator it = m_followerList.begin(); it != m_followerList.end(); ++it)
			(*it)->FollowUpdate(this);

		if (jumpIdx != -1)
		{
			BinaryServerJumppointInstance* pJpInst = m_pMap->GetJumpPoint(jumpIdx);
			if (pJpInst) 
			{
				Stop();
				MapService_Stub::JumpMap(this, pJpInst->DesMapIdNum, m_pMap->GetLine_id(), 
					m_pMap->GetOwner_id(), pJpInst->DesBlockX, pJpInst->DesBlockY);
			}
		}
		else if (targetNpc > 0)
			EncounterBattle(targetNpc);
	}
}

void CMapActor::ChangeCurPos(UInt16 x, UInt16 y)
{
	if (GetCurPosX() == x && GetCurPosY() == y) return;

	if (m_pMap)
		m_pMap->MoveNotify(this, GetCurPosX(), GetCurPosY(), x, y);

	SetCurPos(x, y);
}

void CMapActor::UnBlockMapState()
{
	if (m_mapState && m_mapState->IsBlocked())
		m_mapState->Unblock();
}

Boolean CMapActor::AddFollower(CMapActor *follower)
{
	if (std::find(m_followerList.begin(), m_followerList.end(), follower) == m_followerList.end()) {
		m_followerList.push_back(follower);
		return (BOOLEAN_TRUE);
	} else
		return (BOOLEAN_FALSE);
}

Boolean CMapActor::RemoveFollower(CMapActor *follower)
{
	MapActorList::iterator it = std::find(m_followerList.begin(), m_followerList.end(), follower);

	if (it != m_followerList.end()) {
		m_followerList.erase(it);
		return (BOOLEAN_TRUE);
	} else
		return (BOOLEAN_FALSE);
}

void CMapActor::ClearFollower()
{
	m_followerList.clear();
}

void CMapActor::FollowUpdate(CMapActor *target)
{
	// check target and target map is same
	if (m_pMap && m_pMap == target->GetMap())
	{
		// follow the target current position
		ChangeCurPos(target->GetCurPosX(), target->GetCurPosY());
	}
}

void CMapActor::ChangeBaseSpeed(UInt16 speed)
{
	m_baseSpeed = speed;
	SetSpeed(m_baseSpeed);
}

String CMapActor::DebugInfo()
{
	String s(Parent::DebugInfo());
	s.AppendFormat(_T(", map_id %d, map_ownerId %d, pos %d:%d, region %d"), 
		GetMap_id(), GetMap_ownerId(), GetCurPosX(), GetCurPosY(), m_curRegion);
	return s;
}
CMap* CMapActor::GetMap() 
{ // invalid map
	if (m_pMap != NULL && m_pMap != FindNetObject<CMap>(m_pMap->GetNetId()))
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("[invalid map], map %u, netId %d:%d"),
			m_pMap, m_pMap->GetNetId().srvId, m_pMap->GetNetId().objId);
		m_pMap = NULL;
	}
	return m_pMap; 
}