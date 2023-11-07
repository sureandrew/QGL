#pragma once
#ifndef _MAPACTOR_H_
#define _MAPACTOR_H_

#include "Proxy/Logic/MapActor_Proxy.h"

#ifdef QGL
class CMapActorControl;
#endif	// QGL

#ifdef SIMCLIENT
#include "Common/common_char_map.h"
class CMap;
#endif // SIMCLIENT

class CMapActor : public MapActor_Proxy
{
protected:
	typedef MapActor_Proxy Parent;

#ifdef QGL
	CMapActorControl * m_pControl;
#endif	// QGL

public:

	enum StateEnum {
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
		ST_MAP_TALKING,
		ST_HAWK,
		ST_SIT,
		LAST_STATE,	
	};

	DEFINE_CLASS(MapActor_CLASSID);
// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction);
	virtual void OnUpdateState(const UInt8 &state);
	virtual void OnUpdateMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt8 &direction, const UInt32 &map_ownerId);
	virtual void OnUpdateReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y);
	virtual void OnUpdateCurPos(const UInt16 &curPosX, const UInt16 &curPosY);
	virtual void OnUpdateDstPos(const UInt16 &dstPosX, const UInt16 &dstPosY);
	virtual void OnUpdateAction(const UInt16 &action, const UInt32 &actionTime);
	virtual void OnUpdateSpeed(const UInt32 &speed);
	virtual void OnUpdateFollowId(const UInt32 &followId, const UInt8 &followOrder);
	virtual void OnUpdateAppeared(const Boolean &appeared);

	CMapActor();
	virtual ~CMapActor();
	virtual UInt32 GetActorId() { return 0; }

#ifdef QGL
	VOID SetControl(CMapActorControl * pCtrl);
#endif	// QGL

#ifdef SIMCLIENT
	void Move(LPCPEER pPeer, RPCContext &context, UInt16 x, UInt16 y);
	void SetMap(CMap* pMap) { m_pMap = pMap; }
	CMap* GetMap() { return m_pMap; }
	virtual UInt16 GetCurPosX() { return m_curPos.PosX; }
	virtual UInt16 GetCurPosY() { return m_curPos.PosY; }
	virtual void SetCurPos(UInt16 x, UInt16 y) { m_curPos.PosX = x; m_curPos.PosY = y; }
	UInt16 GetHint1() const;
	UInt16 GetHint2() const;
	Boolean IsMoving() { return (GetAction() == ST_MOVE); }
	void NextMove();

private:
	typedef StlDeque<BlockPos, ManagedAllocator<BlockPos> > PointList;
	UInt32 GetMoveTime(UInt16 x, UInt16 y, UInt32 speed);	// get next move step time

	CMap*			m_pMap;
	PointList		m_path;				// current path
	BlockPos		m_curPos;			// current calculated position
	BlockPos		m_nextStep;			// next move step
	UInt32			m_preMoveTime;		// previous move time
#endif // SIMCLIENT
};

#endif //_MAPACTOR_H_
