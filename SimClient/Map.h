#pragma once
#ifndef _MAP_H_
#define _MAP_H_

#include "Proxy/Logic/Map_Proxy.h"
#include "Common/common_char_map.h"

class CMapActor;
class CCharacter;
class CMapNPC;
class PathFinder;
struct MapDB_Client;

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

	virtual void OnUpdateInfo(const UInt16 &map_id, const UInt8 &line_id, const String &name);

	Boolean LoadData();
	UInt16 GetWidth() { return m_width; }
	UInt16 GetHeight() { return m_height; }
	UInt16 GetBlock(UInt16 x, UInt16 y);
	UInt16 GetObjectCount();
	BlockPos* GetGuidePoint(int i);
	UInt16 GetGuideCount();
	JumpInfo* GetJumpPoint(int i);
	UInt16 GetJumpCount();
	CCharacter* FindCharacter(UInt32 char_id);
	UInt32 FindRandomHawker();
	CMapNPC* FindNpc(UInt32 npc_id);
	CCharacter* GetNearCharacter(UInt16 x, UInt16 y, UInt16 radius, UInt8 type);
	RPCResult GetNearCharacter(RPCContext &context);
	PathFinder* GetPathFinder() { return m_pathFinder; }
	void ShowObjects();
	UInt32Vector FindNpcNearby();
	UInt32 FindNpcUID(UInt32 npc_id);
	UInt16 GetMapLineID();
	CCharacter* FindEnemyCharacterNearby();
    CCharacter* FindGuildMemberNearby();
	Boolean IsGuildNameIdentical(String name1, String name2);	

private:
	typedef Map_Proxy Parent;
	typedef StlMap<RPCNetID, CMapActor*, std::less<RPCNetID>,
			ManagedAllocator<std::pair<RPCNetID, CMapActor*> > > MapActorMap;

	MapDB_Client*	m_mapdb;
	MapActorMap		m_objects;
	UInt16*			m_blockData;
	UInt16			m_width;
	UInt16			m_height;
	JumpInfoVector	m_jumpInfoList;
	PathFinder*		m_pathFinder;

	//Boolean ImportClient_ZoneData(UInt16 mapid);
	//Boolean ImportClient_PFData(UInt32 mapid);
};

INLINE UInt16 CMap::GetObjectCount()
{
	return (UInt16) m_objects.size();
}

INLINE UInt16 CMap::GetBlock(UInt16 x, UInt16 y)
{
	if (x < m_width && y < m_height)
		return m_blockData[y * m_width + x];
	else
		return 0xFFFF;
}

INLINE JumpInfo* CMap::GetJumpPoint(int i)
{
	if (i >= 0  && i < (int) m_jumpInfoList.size())
		return &m_jumpInfoList[i];
	else
		return NULL;
}

INLINE UInt16 CMap::GetJumpCount()
{
	return (UInt16) m_jumpInfoList.size();
}

#endif //_MAP_H_