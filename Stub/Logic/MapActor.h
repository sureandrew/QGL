#pragma once
#ifndef _MAPACTOR_H_
#define _MAPACTOR_H_

#include "Stub/Logic/MapActor_Stub.h"
#include "Common/common_char_map.h"

class CMap;

class CMapActor : public MapActor_Stub
{
public:
	typedef StlDeque<BlockPos, ManagedAllocator<BlockPos> > PointList;
	typedef StlVector<CMapActor*, ManagedAllocator<CMapActor*> > MapActorList;

	DEFINE_CLASS(MapActor_CLASSID);

	CMapActor();
	virtual ~CMapActor();
	virtual UInt8 GetLevel() { return 0; }
	virtual UInt32 GetActorId() { return 0; }
	//CMap* GetMap() { return m_pMap; }					// get current map
	CMap* GetMap();
	PointList* GetPath() { return &m_path; }			// get current moving path
	Boolean Move(const UInt16 &posX, const UInt16 &posY, const UInt16 &hint1, const UInt16 &hint2);
	Boolean Stop();
	void ChangeCurPos(UInt16 x, UInt16 y);
	void FollowTarget(CMapActor *target);
	void StopFollow();
	void FollowUpdate(CMapActor *target);
	CMapActor* GetFollowTarget() { return m_pFollowTarget; }
	UInt8 GetFollowerCount();
	Boolean AddFollower(CMapActor *follower);
	Boolean RemoveFollower(CMapActor *follower);
	void ClearFollower();
	void UnBlockMapState();
	INLINE void SetCurPos(const UInt16 &curPosX, const UInt16 &curPosY)
	{
		Parent::SetCurPos(curPosX, curPosY);
		UnmarkUpdate(curPosMask);
		UnmarkView(curPosMask);
		UnmarkModify(curPosMask);
	}
	INLINE void SetAppeared(const Boolean &appeared)
	{
		Parent::SetAppeared(appeared);
		UnmarkUpdate(appearedMask);
		UnmarkView(appearedMask);
		UnmarkModify(appearedMask);
	}

	virtual void OnEnterMap(CMap* pmap);						// enter map event
	virtual void OnLeaveMap();						// leave map event
	virtual Boolean ChangeState(CONST StateID state);			// try to change state
	virtual Boolean IsState(CONST StateID state);				// is that state now?
	virtual Boolean TryState(CONST StateID state);				// can change to state?
	virtual String DebugInfo();

	enum StateEnum
	{
		ST_ENTER_MAP = 1,
		ST_LEAVE_MAP,
		ST_JUMP_MAP,
		ST_STOP,
		ST_MOVE,
		ST_FOLLOW,
		ST_NOBATTLE,
		ST_BATTLE,
		ST_NEEDHELP,
		ST_MONITOR,
		ST_TALK,
		ST_HAWK,
		ST_SIT,
		LAST_STATE,
	};

	enum EventEnum
	{
		EVT_ATTRIB_UPDATE	= MAKE_EVENTID(MapActor_CLASSID, 1),	// attribute update
		EVT_CHANGE_FOLLOW	= MAKE_EVENTID(MapActor_CLASSID, 2),
		EVT_FOLLOW_MOVE		= MAKE_EVENTID(MapActor_CLASSID, 3),
		EVT_JUMP_MAP		= MAKE_EVENTID(MapActor_CLASSID, 4),
	};

	class CEnterState;
	class CLeaveState;
	class CJumpState;
	class CStopState;
	class CMoveState;
	class CFollowState;

	RPCMask			m_oldViewMask;			// updated mask for old view
	UInt16			m_curRegion;			// current region index
	UInt16			m_newRegion;			// new region index

	virtual void ChangeBaseSpeed(UInt16 speed);

protected:
	virtual UInt32 CheckBattle() { return 0; }
	virtual void EncounterBattle(UInt32 targetNpc) {}

	void NextMove();

	State*			m_mapState;			// [leave map, enter map, jump map]
	State*			m_moveState;		// [stop, move, follow, map talking]

	CMap*			m_pMap;
	PointList		m_path;				// current path
	BlockPos		m_nextStep;			// next move step
	UInt32			m_preMoveTime;		// previous move time
	CMapActor*		m_pFollowTarget;	// target to follow
	MapActorList	m_followerList;		// follower list of this target

	UInt16		m_baseSpeed;

private:

	typedef MapActor_Stub Parent;
	UInt32 GetMoveTime(UInt16 x, UInt16 y, UInt32 speed);	// get next move step time
};

class CMapActor::CEnterState : public State
{
public:
	DEFINE_STATE(ST_ENTER_MAP);
};

class CMapActor::CLeaveState : public State
{
public:
	DEFINE_STATE(ST_LEAVE_MAP);
	CMapActor* m_actor;
	CMapActor::CLeaveState(CMapActor* actor) : m_actor(actor)	{}

protected:
	virtual void Enter(VOID);
	virtual void Exit(VOID);
};

class CMapActor::CJumpState : public State
{
public:
	DEFINE_STATE(ST_JUMP_MAP);
	CMapActor* m_actor;
	CMapActor::CJumpState(CMapActor* actor) : m_actor(actor) {}

protected:
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST
	{
		return (currentId == ST_ENTER_MAP);
	}
	virtual void Enter(VOID);
	virtual void Exit(VOID);
};

class CMapActor::CStopState : public State
{
public:
	DEFINE_STATE(ST_STOP)
};

class CMapActor::CMoveState : public State
{
public:
	DEFINE_STATE(ST_MOVE)
	CMoveState(CMapActor* actor) : m_actor(actor) {}
	virtual void OnUpdate(CONST UInt32 dt);
	CMapActor* m_actor;

protected:
	virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST;
	virtual void Enter(VOID);
	virtual void Exit(VOID);
};

class CMapActor::CFollowState : public State
{
public:
	DEFINE_STATE(ST_FOLLOW)
};

#endif //_MAPACTOR_H_
