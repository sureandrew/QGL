#pragma once
#ifndef _MAPNPC_H_
#define _MAPNPC_H_

#include "Stub/Logic/MapActor.h"
#include "Stub/Logic/MapNPC_Stub.h"

struct NpcData;
class CCharacter;

typedef StlMap<UInt16, UInt32, std::less<UInt16>,
		ManagedAllocator<std::pair<UInt16, UInt32> > > FlagReqMap;

struct QuestHint
{
	UInt16 quest_id;
	UInt16 minLv;
	UInt16 maxLv;
	UInt8 sex;
	UInt8 job;
	UInt8 faction;
	FlagReqMap* pFlagMap;

	INLINE QuestHint() : quest_id(0), minLv(0), maxLv(0), sex(0), 
				job(0), faction(0), pFlagMap(NULL)
	{
	}

	INLINE ~QuestHint()
	{
		SafeDeleteObject(pFlagMap);
	}

	INLINE void AddFlag(UInt16 flag_id, UInt32 value)
	{
		if (pFlagMap == NULL)
			pFlagMap = SafeCreateObject(FlagReqMap);
		(*pFlagMap)[flag_id] = value;
	}
};

typedef StlMap<UInt16, QuestHint, std::less<UInt16>,
		ManagedAllocator<std::pair<UInt16, QuestHint> > > QuestHintMap;

class CMapNPC : public MapNPC_Stub,
				public Reuben::Simulation::IEventHandler<CMapNPC>
{
public:
	CMapNPC();
	~CMapNPC();
	DEFINE_CLASS(MapNPC_CLASSID);
	
	enum EventEnum
	{
		EVT_DESTROY		= MAKE_EVENTID(MapNPC_CLASSID,  1),
		EVT_RELEASE		= MAKE_EVENTID(MapNPC_CLASSID,  2),
		EVT_DISPLAY		= MAKE_EVENTID(MapNPC_CLASSID,  3),
	};

	Boolean CanTalk(CCharacter* pChar);
	void TalkEnd();
	void AddAcceptId(UInt32 char_id);
	void ClearAcceptId();

	INLINE static CMapNPC* NewInstance();
	virtual UInt32 GetActorId()  { return GetNpc_uid(); }
	virtual String DebugInfo();
	void SetData(const NpcData* pData);
	void OnLeaveMap();
	void Destroy();

	void CheckAppear(CMap* pMap, UInt8 gameTime);
	Boolean GetAppearWorld() { return m_appearWorld; }
	void SetAppearWorld(Boolean appear) { m_appearWorld = appear; }
	INLINE void SetPermanent() { m_lifeTime = UINT32_MAX_VALUE; }
	void SetLifeTime(UInt32 lifeTime);
	INLINE UInt32 GetLifeTime() { return m_lifeTime; }
	INLINE void SetEventId(UInt16 evenId) { m_eventId = evenId; }
	INLINE UInt16 GetEventId() { return m_eventId; }

	Boolean Link(CMapActor* pActor);
	Boolean Unlink(CMapActor* pActor);
	void RemoveAllLink(void);
	void LoadQuestHint();			// load quest hint from quest and npc scripts
	void SetQuestHint(UInt16 quest_id, UInt16 minLv, UInt16 maxLv, 
						UInt8 sex, UInt8 job, UInt8 faction);
	void AddHintFlag(UInt16 quest_id, UInt16 flag_id, UInt16 value);
	INLINE Boolean HasQuestHint()		{ return !m_questHintMap.empty(); }
	QuestHintMap& GetHintMap()			{ return m_questHintMap; }

	virtual void HandleEvent(CONST Event &event);

	const NpcData* m_pData;

	void SetEnableBattle(Boolean enable){m_enableBattle = enable;};
	Boolean GetEnableBattle(){return m_enableBattle;};

	void SetCreaterCharId(UInt32 CharId) { m_createrCharId = CharId; }
	UInt32 GetCreaterCharId() { return m_createrCharId; }

	void DisPlayNpc();
private:
	typedef MapNPC_Stub Parent;
	typedef StlSet<UInt32, std::less<UInt32>, ManagedAllocator<UInt32> > AcceptSet;

	typedef StlMap<RPCNetID, CMapActor*, std::less<RPCNetID>,
			ManagedAllocator<std::pair<RPCNetID, CMapActor*> > > ActorMap;

	

	AcceptSet		m_acceptIdSet;
	ActorMap		m_actorMap;
	Boolean			m_appearWorld;
	UInt32			m_appearTime;
	UInt32			m_lifeTime;
	UInt32			m_lastCharId;
	UInt16			m_eventId;
	QuestHintMap	m_questHintMap;
	Boolean         m_enableBattle;

	UInt32			m_createrCharId;
};

INLINE CMapNPC* CMapNPC::NewInstance()
{
	return CreateNetObject<CMapNPC>();
}

#endif //_MAPNPC_H_