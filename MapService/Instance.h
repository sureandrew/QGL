#pragma once
#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#define Instance_CLASSID		16999	

#define INSTANCE_CREATE_FUNC	"CreateInstance"
#define INSTANCE_CAN_ENTER		"OnCanEnter"
#define INSTANCE_INIT			"OnInit"
#define INSTANCE_ENTER			"OnEnter"
#define INSTANCE_LEAVE			"OnLeave"
#define INSTANCE_CLOSE			"OnClose"

class CCharacter;
class CMap;
class CScriptSystem;

class CInstance : public Reuben::Simulation::Object,
					public Reuben::Simulation::IEventHandler<CInstance>
{
public:
	CInstance();
	virtual ~CInstance();
	UInt16 GetInstId() { return m_instId; }
	UInt32 GetOwnerId() { return m_ownerId; }
	void SetLuaFile(StringA luafile) { m_luaFile = luafile; }
	void Init(UInt16 instId, UInt32 ownerId, UInt8 instType);
	void Reset();
	void Close();
	void OnEnter(CCharacter* pchar);
	void OnPreEnter();
	void OnLeave(CCharacter* pchar);
	void AddMap(UInt16 map_id);
	UInt16 GetPlayerLimit() { return m_playerLimit; }
	void SetPlayerLimit(UInt16 playerMax) { m_playerLimit = playerMax; }
	UInt16 GetAllowCount();
	void SetLasting(int lasting);
	void ChgPlayerCount(Int16 chg);
	virtual void HandleEvent(CONST Event &event);

	static CInstance* CreateInstance(UInt16 instId, UInt32 owner_id, UInt8 inst_type);
	static void DirectCreateInstance(CCharacter* pchar, StringA luafile, UInt16 instId, 
									UInt32 ownerId);
	static void DestroyInstance(UInt16 instId, UInt32 owner_id);
	static CInstance* GetInstance(UInt16 instId, UInt32 ownerId);
	static RPCResult JumpInstance(RPCContext &context);
	static RPCResult JumpEmptyInstance(RPCContext &context);
	static void DeleteAll();

	typedef Reuben::Simulation::Object Parent;
	DEFINE_CLASS(Instance_CLASSID);

private:
	enum EventEnum
	{
		EVT_DESTROY		= MAKE_EVENTID(Instance_CLASSID,  1),
	};

	typedef StlMap<UInt16, CMap*, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, CMap*> > > MapList;

	UInt16			m_instId;
	UInt8			m_instType;
	UInt32			m_ownerId;
	StringA			m_luaFile;
	UInt16			m_playerLimit;
	UInt16			m_playerCount;
	UInt32			m_lastingTime;
	MapList			m_mapList;
	CScriptSystem*	m_pScriptSys;
};

#endif //_INSTANCE_H_