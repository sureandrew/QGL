#pragma once
#ifndef _ANIM_TIMER_H_
#define _ANIM_TIMER_H_

class CBatCharacter;
class CBatMob;
class CBatActor;

#define NUM_DISPLAY_TIME	900
class CAnimTimer
{
public:
	CAnimTimer() : m_actMoving(0), m_escapeMoving(0), m_escaped(0), m_catchMoving(0) {}
	~CAnimTimer() { Uninitialize(); }
	void Reset();
	void Start();
	void Stop();
	UInt32 GetActionTime(CBatCharacter *pSrcChar, CBatMob *pSrcMob, CBatActor * pTarget, UInt8 action, 
		UInt16 subId=0, UInt16 multi=1);
	void Initialize();
	void Uninitialize();

private:

	enum AnimType
	{
		ANIM_IDLE1 = 0,
		ANIM_RUN,
		ANIM_SIT,
		ANIM_W1BAT,
		ANIM_W1ATK1,
		ANIM_W1ATK2,
		ANIM_W2BAT,
		ANIM_W2ATK1,
		ANIM_W2ATK2,
		ANIM_SKILL,
		ANIM_HURT,
		ANIM_DIE,
		ANIM_DEF,
		ANIM_USE,
		ANIM_JUMP_START,
		ANIM_JUMP,
		ANIM_JUMP_END,
		ANIM_IDLE2,
		ANIM_WALK,
		ANIM_MOUNT_IDLE1,
		ANIM_MOUNT_RUN,
		ANIM_MOUNT_IDLE2,
		ANIM_TYPE_MAX,						// original max
		ANIM_CATCH,							// additional action
		ANIM_W1ATK1_S1,						// only attack step 1 time
		ANIM_W2ATK1_S1,
	};

	struct BodyAction
	{
		union {
			UInt16 sexClass;
			UInt16 mob_id;
		};
		UInt16 action;

		BodyAction()
			: sexClass(0)
			, action(0)
		{}

		BodyAction(UInt16 s, UInt16 a)
			: sexClass(s)
			, action(a)
		{}

		bool operator < (const BodyAction& b) const
		{
			return (sexClass < b.sexClass || (sexClass == b.sexClass && action < b.action));
		}
	};

	struct WeaponType
	{
		UInt16 item_id;
		UInt16 sexClass;

		WeaponType()
			: item_id(0)
			, sexClass(0)
		{}

		WeaponType(UInt16 id, UInt16 sc)
			: item_id(id)
			, sexClass(sc)
		{}

		bool operator < (const WeaponType& b) const
		{
			return (item_id < b.item_id || (item_id == b.item_id && sexClass < b.sexClass));
		}
	};

	struct SkillAction
	{
		UInt8 animType;
		UInt8 motionType;
		UInt16 reactTime;

		SkillAction()
			: animType(0)
			, motionType(0)
			, reactTime(0)
		{}
	};

	struct ItemAction
	{
		UInt16 reactTime;

		ItemAction()
			: reactTime(0)
		{}
	};

	typedef StlMap<UInt8, UInt16Vector, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt8, UInt16Vector> > > DelayMap;
	typedef StlMap<String, DelayMap, std::less<String>, 
		ManagedAllocator<std::pair<String, DelayMap> > > EntStateMap;
	typedef StlMap<BodyAction, UInt16, std::less<BodyAction>, 
		ManagedAllocator<std::pair<BodyAction, UInt16> > > BodyActionMap;
	typedef StlMap<WeaponType, UInt8, std::less<WeaponType>, 
		ManagedAllocator<std::pair<WeaponType, UInt8> > > WeaponTypeMap;
	typedef StlMap<UInt16, SkillAction, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, SkillAction> > > SkillActionMap;

	typedef StlMap<UInt16, ItemAction, std::less<UInt16>, 
		ManagedAllocator<std::pair<UInt16, ItemAction> > > ItemActionMap;

	void GlobFiles(const TCHAR *filename);
	Boolean LoadCSV(const TCHAR *pattern);
	void SetSingleDelay(const TCHAR *filename, UInt16 action);
	void SetBodyDelay(BodyActionMap &bodyMap, const TCHAR *filename, UInt16 sexClass, bool bChar);
	void SetWeaponType();
	void SetSkillDelay();
	void SetItemDelay();
	void DebugInfo();

	String			m_path;
	EntStateMap		m_entStateMap;
	BodyActionMap	m_charMap;
	BodyActionMap	m_mobMap;
	WeaponTypeMap	m_weaponMap;
	SkillActionMap	m_skillMap;
	ItemActionMap	m_itemMap;
	UInt32			m_actMoving;
	UInt32			m_hurtMoving;
	UInt32			m_escapeMoving;
	UInt32			m_escaped;
	UInt32			m_catchMoving;
};

#endif // _ANIM_TIMER_H_
