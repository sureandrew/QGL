#pragma once
#ifndef _MAP_H_
#define _MAP_H_

#include "Stub/Logic/Map_Stub.h"
#include "Common/common_binary.h"
#include "Common/common_char_map.h"
#include "Common/PathFinder.h"
#include <Reuben/Utility/ProcessManager.h>

class CMapActor;
class CCharacter;
class CMapNPC;
struct QuestHint;

struct BuffInfo
{
	UInt16Vector	buffIds;		// buff id list
	UInt16			period;
	Boolean			bLeave;

	BuffInfo() : period(0), bLeave(BOOLEAN_FALSE)
	{
	}
};

struct Region
{
public:
	typedef StlMap<RPCNetID, CMapActor*, std::less<RPCNetID>,
			ManagedAllocator<std::pair<RPCNetID, CMapActor*> > > MapActorMap;
	UInt16		m_index;
	BlockPos	m_startCorner;
	BlockPos	m_endCorner;
	MapActorMap	m_actors;
	RPCNetIDVector	m_netIds;		// actor net ids

	void AddActor(CMapActor *actor);
	void RemoveActor(CMapActor *actor);

	INLINE void Set(UInt16 index, UInt16 sx, UInt16 sy, UInt16 ex, UInt16 ey)
	{
		m_index = index;
		m_startCorner.PosX = sx;
		m_startCorner.PosY = sy;
		m_endCorner.PosX = ex;
		m_endCorner.PosY = ey;
	}

	INLINE Boolean IsInside(BlockPos &pos)
	{
		return (pos.PosX >= m_startCorner.PosX && pos.PosX <= m_endCorner.PosX &&
			pos.PosY >= m_startCorner.PosY && pos.PosY <= m_endCorner.PosY);
	}

	INLINE Boolean IsIntersect(Region &region)
	{
		return ((region.m_startCorner.PosX >= m_endCorner.PosX && 
			region.m_startCorner.PosX <= m_endCorner.PosX ||
			region.m_endCorner.PosX >= m_endCorner.PosX && 
			region.m_endCorner.PosX <= m_endCorner.PosX) &&
			(region.m_startCorner.PosY >= m_endCorner.PosY && 
			region.m_startCorner.PosY <= m_endCorner.PosY ||
			region.m_endCorner.PosY >= m_endCorner.PosY && 
			region.m_endCorner.PosY <= m_endCorner.PosY));
	}
};

class CMap : public Map_Stub, public Reuben::Utility::Process,
	public Reuben::Simulation::IEventHandler<CMap>
{
public:

	DEFINE_CLASS(Map_CLASSID);

	CMap();
	virtual ~CMap();

	INLINE static CMap* NewInstance();
	virtual RPCResult GetCharInRange(LPCPEER pPeer, RPCContext &context, const UInt16 &centerX, 
									const UInt16 &centerY, const UInt16 &range);
	virtual RPCResult GetJumpPoints(LPCPEER pPeer, RPCContext &context);

	String GetNpcActorNickName(UInt32 id);
    
	virtual void OnUpdate(CONST UInt32 dt);
	INLINE UInt16 GetWidth() const { return m_width; };
	INLINE UInt16 GetHeight() const { return m_height; };
	void GetViewNetIds(UInt16 regionIdx, RPCNetIDVector& netIds);
	void AddActor(CMapActor *actor);
	void RemoveActor(CMapActor *actor);
	INLINE void AttachProcess(Reuben::Utility::Process* pProcess);
	INLINE void DetachProcess(Reuben::Utility::Process* pProcess);
	MapDB* GetMapInfo() { return m_mapdb; };
	int SetMapDb(MapDB* mapdb);
	String GetName() { return (m_mapdb ? m_mapdb->m_basicinfo->m_mapname : _T("")); };
	PathFinder* GetPathFinder(){ return m_pathfinder; };
	void SetPathFinder(PathFinder* pathfinder){ m_pathfinder = pathfinder; };
	int AddJumppointInstance(BinaryServerJumppointInstance* jp);
	void AddObjInstance(BinaryServerOBJInstance* obj);
	int GetObjState(UInt32 objId);
	void ChgObjInstance(UInt32 objId, UInt8 state);
	void RemoveObjInstance(UInt32 objId);
	Boolean IsBlocked(UInt16 x, UInt16 y);
	Boolean IsObjBlocked(BlockPos &pos);
	Boolean IsOccupied(UInt16 x, UInt16 y);
	Boolean IsHawkOccupied(UInt16 x, UInt16 y);
	int IsJumpPoint(UInt16 x, UInt16 y);
	BinaryServerJumppointInstance* GetJumpPoint(int index);
	int DestroyMapContext();
	CMapActor* FindMapActor(UInt16 classId, UInt32 actorId);
	UInt32 GetObjectCount();
	void SendAllObjects(CCharacter *pchar);
	Region* FindRegion(UInt16 x, UInt16 y);
	Region* FindRegion(UInt16 index);
	INLINE UInt16 GetRegionCount() { return m_regionTotal; }
	// notify map when actor moved
	void MoveNotify(CMapActor *actor, UInt16 x1, UInt16 y1, UInt16 x2, UInt16 y2);
	void AddNotify(CMapActor *actor);
	void RemoveNotify(CMapActor *actor);
	void SafeRemove(CMapActor *actor);
	void UpdateRegions();							// update moved actor in regions
	Boolean IsOpen() { return m_bOpen; }
	void SetOpen(Boolean bOpen);
	bool IsHawkZone(UInt16 x, UInt16 y);
	CMapNPC* CreateNpc(UInt32 npcId, UInt16 x, UInt16 y, UInt8 dir=0, 
						UInt32 lifeTime=UINT32_MAX_VALUE, UInt16 eventId=0, 
						UInt32 char_id=0,bool visble=true);
	void RemoveNpc(CMapNPC* pNpc);
	void RemoveNpcEvent(UInt16 eventId);
	void AddBuffInfo(UInt16 eventId, BuffInfo &buffInfo);
	void RemoveBuffInfo(UInt16 eventId);
	void TimeZoneChanged(UInt8 gameTime);
	void LoadAllHints();			// load all quest hints of current npc list
	void CheckAllHints(CCharacter* pChar, RPCNetIDVector &addNpcIds, UInt16Vector &addQuestIds,
						RPCNetIDVector &delNpcIds, UInt16Vector &delQuestIds);
	void ForceAllLeave();
	void ForceLeaveGuildWar(UInt16 map_id, UInt16 x, UInt16 y);

	static Boolean IsOpenMap(UInt16 map_id);
	static Boolean IsClosedMap(UInt16 map_id);
	static void AddClosedMap(UInt16 map_id);
	static void RemoveClosedMap(UInt16 map_id);
	void StartGuildWar();
	void CheckGuildWarResult(bool end);
	void GenGuildWarBox();
	void CalcRewards();
	void RandomPos(UInt16 & x, UInt16 & y);

	void CreateIncognitoBox();
	void RandomIncognitoBox(UInt8 boxType);
	void ShowIncognitoBoxEff(UInt32 pointx,UInt32 pointy);

	void UpdateMapNpcVisbleMask(CMapNPC* pMapNpc,Boolean visble);

	void RandomBox(UInt16 x,UInt16 y,UInt8 boxType);

	enum EventEnum
	{
		EVT_UPDATE_GUILD_WAR		= MAKE_EVENTID(Map_CLASSID,  1),
		EVT_UPDATE_GUILD_END		= MAKE_EVENTID(Map_CLASSID,  2),
		EVT_UPDATE_GUILD_KICK		= MAKE_EVENTID(Map_CLASSID,  3),
	};

	INLINE UInt32 GetGuildWarWinID(){return m_guildWarWinID;}
	INLINE bool	GetGuildWarKick(){return m_guildWarKick;}
	void GetAllPartyInfo(PartyInfoVector &PartyInfoList);

	void GetAllCharInfo(CharInfoVector &CharInfoList);
private:
	void DeleteRegions();
	void MoveToRegion(CMapActor *actor);
	void AddToRegion(CMapActor *actor);
	void RemoveFromRegion(CMapActor *actor);
	UInt16 RegionDistance(UInt16 region1, UInt16 region2);

	struct ActorIdType
	{
		UInt16 classId;
		UInt32 actorId;

		ActorIdType(UInt16 cid, UInt32 aid)
			: classId(cid)
			, actorId(aid)
		{}
		ActorIdType(CMapActor* actor);

		bool operator < (const ActorIdType& b) const
		{
			return (classId < b.classId || (classId == b.classId && actorId < b.actorId));
		}
	};

	struct ObjInst : public BinaryServerOBJInstance
	{
		UInt8			state;
		BlockPosVector	blockList;
	};

	typedef StlMap<ActorIdType, CMapActor*, std::less<ActorIdType>,
			ManagedAllocator<std::pair<ActorIdType, CMapActor*> > > ActorIdMap;
	typedef StlMap<UInt32, CMapNPC*, std::less<UInt32>,
			ManagedAllocator<std::pair<UInt32, CMapNPC*> > > NpcActorMap;
	typedef StlMap<UInt16, BuffInfo, std::less<UInt16>,
			ManagedAllocator<std::pair<UInt16, BuffInfo> > > BuffInfoMap;
	typedef StlSet<BlockPos, std::less<BlockPos>,
			ManagedAllocator<BlockPos> > BlockPosSet;
	typedef StlMap<UInt32, ObjInst, std::less<UInt32>,
			ManagedAllocator<std::pair<UInt32, ObjInst> > > ObjInstMap;


	void AddEventBuff(CCharacter* pChar);
	void RemoveEventBuff(CCharacter* pChar);

	typedef Map_Stub Parent;

	Reuben::Utility::ProcessManager m_updater;
	MapDB*			m_mapdb;
	NPCDB*			m_npcdb;
	JpInstList		m_jpList;				// jump point instance list
	PathFinder* 	m_pathfinder;
	Region**		m_regions;
	UInt16			m_regionCol;
	UInt16			m_regionRow;
	UInt16			m_regionTotal;
	UInt16			m_width;
	UInt16			m_height;
	Boolean			m_bOpen;				// map is open to enter
	ActorIdMap		m_actors;
	ActorIdMap		m_chgActors;			// region changed actor map
	NpcActorMap		m_npcActors;
	BuffInfoMap		m_eventBuffMap;
	BlockPosSet		m_objBlockSet;
	ObjInstMap		m_objMap;				// object instance map

	//guild war info
	UInt32			m_guildWarID1;
	UInt32			m_guildWarID2;
	UInt32			m_guildWarWinID;
	bool			m_guildWarKick;
	bool			m_inGuildWar;

	

	//////////////////////////////////////
	//		Event & State methods		//
	//////////////////////////////////////
	virtual void HandleEvent(CONST Event &event);
	
};

INLINE CMap* CMap::NewInstance()
{
	return CreateNetObject<CMap>();
}

INLINE void CMap::AttachProcess(Reuben::Utility::Process* pProcess)
{
	m_updater.Attach(pProcess);
}

INLINE void CMap::DetachProcess(Reuben::Utility::Process* pProcess)
{
	m_updater.Detach(pProcess);
}

INLINE Boolean CMap::IsObjBlocked(BlockPos &pos)
{
	return (!m_objBlockSet.empty() && m_objBlockSet.count(pos) > 0);
}

#endif //_MAP_H_
