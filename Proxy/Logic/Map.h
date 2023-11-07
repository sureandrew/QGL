#pragma once
#ifndef _MAP_H_
#define _MAP_H_

#include "Proxy/Logic/Map_Proxy.h"
#include "Proxy/View/MapView.h"

class CMapNPC;
class CCharacter;
class CCharacterControl;
class CMapNPCControl;
struct BlockPos;

class CMap : public Map_Proxy
{
public:
	DEFINE_CLASS(Map_CLASSID);
	CMap();
	virtual~CMap();
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_GetCharInRange(LPCPEER pPeer, RPCContext &context, NetGroup* grp);
	virtual RPCResult CB_GetJumpPoints(LPCPEER pPeer, RPCContext &context, const JumpInfoVector &jumpInfoList);
	virtual RPCResult CB_ObjectChanged(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const RPCNetIDVector &delIds, const RPCNetIDVector &curIds);

	inline void SetMapView(CMapView * pGivMaprView){ m_pMapView = pGivMaprView;};
	CMapNPC* FindNpc(UInt32 npc_id);
	CCharacter* FindCharacter(UInt32 Char_id);
	UInt16 GetWidth();
	UInt16 GetHeight();
	UInt16 GetBlock(UInt16 x, UInt16 y);
	bool IsBlocked(UInt16 x, UInt16 y);
	UInt16 GetActorCount();
	CCharacterControl* GetNearCharacter(UInt16 x, UInt16 y, UInt16 radius, UInt8 flag);

	void MoveToRandomGuidePos();

private:
	typedef Map_Proxy Parent;
	typedef StlMap<RPCNetID, CMapActor*, std::less<RPCNetID>,
			ManagedAllocator<std::pair<RPCNetID, CMapActor*> > > MapActorMap;
	MapActorMap		m_objects;
	CMapView * m_pMapView;
};

#endif //_MAP_H_
