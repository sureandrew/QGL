//-- Common
#include "Common.h"
#include "Common/BattleCode.h"
#include "Common/DataCode.h"
//-- Self
#include "BattleService/AnimTimer.h"
//-- Library
#include "Stub/Logic/BatCharacter.h"
#include "Stub/Logic/BatMob.h"
#include "BattleService/Global.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResItem.h"
#include "Resource/ResSkill.h"
#include "Resource/ResMob.h"
#include "Resource/ServerResManager.h"
#include <Reuben/Utility/ConfigFile.h>

#define DELAY_FILE		_T("./Data/Setting/EntDelay.ini")
#define CSV_EXT			_T(".csv")
#define SEXCLASS_TOTAL	8

INLINE String GetBaseName(const TCHAR *filename, const TCHAR *trim=NULL)
{
	if (filename == NULL) return _T("");

	TCHAR fullPath[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	_tfullpath(fullPath, filename, _MAX_PATH);
	_tsplitpath(fullPath, drive, dir, fname, ext);

	if (trim) {
		size_t len1 = _tcslen(fname);
		size_t len2 = _tcslen(trim);
		if (len1 > len2 && _tcsnicmp(&fname[len1 - len2], trim, len2) == 0)
			fname[len1 - len2] = _T('\0');
	}

	String s(fname);
	s.ToLower();						// lower case for filename

	return s;
}

INLINE String GetTrimName(const TCHAR *filename, const TCHAR *trim)
{
	size_t len1 = _tcslen(filename);
	size_t len2 = _tcslen(trim);
	String s(filename);

	s.ToLower();						// lower case for filename
	if (len1 > len2 && _tcsnicmp(&filename[len1 - len2], trim, len2) == 0)
		return s.substr(0, len1 - len2);
	else
		return s;
}

INLINE UInt32 GetTotal(UInt16Vector &vlist)
{
	UInt32 total = 0;
	for (UInt16Vector::iterator it = vlist.begin(); it != vlist.end(); ++it)
		total += *it;

	return total;
}

INLINE UInt16 GetFirst(UInt16Vector &vlist)
{
	return (vlist.empty() ? 0 : vlist.front());
}

void CAnimTimer::Initialize()
{
	Reuben::Utility::ConfigFile config;
	String pattern;
	String file;

	config.SetFileName(DELAY_FILE);
	m_path = config.GetString(_T("General"), _T("path"), _T("./Data/EntDelay"));

	// load all body, skill and weapon related delay ent.csv first
	pattern = m_path + config.GetString(_T("Data"), _T("body"), _T("b*_t001.ent.csv"));
	GlobFiles(pattern.c_str());

	pattern = m_path + config.GetString(_T("Data"), _T("mob"), _T("d*_t*.ent.csv"));
	GlobFiles(pattern.c_str());

	pattern = m_path + config.GetString(_T("Data"), _T("weapon"), _T("w*_t*.ent.csv"));
	GlobFiles(pattern.c_str());

	pattern = m_path + config.GetString(_T("Data"), _T("effAtk"), _T("effAtk*.ent.csv"));
	GlobFiles(pattern.c_str());

	pattern = m_path + config.GetString(_T("Data"), _T("effHurt"), _T("effHurt*.ent.csv"));
	GlobFiles(pattern.c_str());

	pattern = m_path + config.GetString(_T("Data"), _T("effSpark"), _T("effSpark*.ent.csv"));
	GlobFiles(pattern.c_str());

	// for all default battle action delay of character in each sex class
	pattern = config.GetString(_T("Setting"), _T("bodyfmt"), _T("b%05d_t001.ent"));
	for (int i = 1; i <= SEXCLASS_TOTAL; i++) {
		file.Format(pattern.c_str(), i);
		file.ToLower();
		SetBodyDelay(m_charMap, file.c_str(), i, true);
	}

	// for all default battle action delay of all mobs
	MobDataList mobList;
	TCHAR mobEnt[32];
	gGlobal.m_resource->GetMobList(mobList);
	for (MobDataList::iterator it = mobList.begin(); it != mobList.end(); ++it)
	{
		const MobData* pMob = *it;
		if (pMob == NULL || pMob->model[0] == _T('\0')) continue;
		_tcscpy(mobEnt, pMob->model);
		_tcslwr(mobEnt);
		SetBodyDelay(m_mobMap, mobEnt, pMob->mob_id, false);
	}

	// catch delay
	file = config.GetString(_T("Delay"), _T("catchHurt"), _T("effCatch_atk.ent.csv"));
	file.ToLower();
	SetSingleDelay(file.c_str(), ANIM_CATCH);

	// other time setting
	m_actMoving = config.GetInt(_T("Setting"), _T("actMoving"), 2000);
	m_escapeMoving = config.GetInt(_T("Setting"), _T("escapeMoving"), 1000);
	m_escaped = config.GetInt(_T("Setting"), _T("escaped"), 1000);
	m_catchMoving = config.GetInt(_T("Setting"), _T("catchMoving"), 2600);

	// identify all weapon animation type: W1/W2
	SetWeaponType();	

	// pre-calculate all skill animation time
	SetSkillDelay();

	// entity state map no use now
	m_entStateMap.clear();

	//DebugInfo();
}

void CAnimTimer::Uninitialize()
{
	m_path.Empty();
	m_entStateMap.clear();
	m_charMap.clear();
	m_mobMap.clear();
	m_weaponMap.clear();
	m_skillMap.clear();
}

Boolean CAnimTimer::LoadCSV(const TCHAR *filename)
{
	CQGLCSVTReader reader;
	UInt8 state = 0;
	UInt8 step = 0;
	UInt8 rotation = 0;
	UInt16 delay = 0;
	UInt8 prevState = 255;
	UInt8 prevRot = 255;
	String fullname = m_path + filename;
	String entName = GetTrimName(filename, CSV_EXT);
	
	if (reader.Open(fullname.c_str(), true))
	{
		try {
			DelayMap &delayMap = m_entStateMap[entName.c_str()];
			while (!reader.IsLineEmpty())
			{
				reader.Read(_T("State"),	state);
				//reader.Read(_T("Step"),		step);		// assume step is one by one
				reader.Read(_T("Rotation"),	rotation);
				reader.Read(_T("Delay"),	delay);
				
				if (prevState == 255 || prevState != state)	// state changed
					prevRot = rotation;
				prevState = state;

				if (prevRot == rotation)		// only need delay for one rotation
					delayMap[state].push_back(delay);
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _T("AnimTimer LoadCSV Error: invalid column format, filename %s"), filename);
			reader.Close();
			m_entStateMap.erase(entName.c_str());
			return BOOLEAN_FALSE;
		}
		reader.Close();
		return BOOLEAN_TRUE;
	}

	return BOOLEAN_FALSE;
}

void CAnimTimer::GlobFiles(const TCHAR *pattern)
{
	HANDLE hFind;
	WIN32_FIND_DATA fileData;

	hFind = ::FindFirstFile(pattern, &fileData);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		LoadCSV(fileData.cFileName);
		if (! ::FindNextFile(hFind, &fileData))
			hFind = INVALID_HANDLE_VALUE;
	}
}

void CAnimTimer::SetSingleDelay(const TCHAR *filename, UInt16 action)
{
	EntStateMap::iterator it;
	String entName = GetTrimName(filename, CSV_EXT);

	LoadCSV(filename);
	it = m_entStateMap.find(entName.c_str());
	if (it != m_entStateMap.end()) {
		UInt16Vector &vlist = it->second[0];
		BodyAction ba;
		UInt32 totalTime = GetTotal(vlist);

		ba.action = action;
		ba.sexClass = 1;						// set sexClass = 1 for single delay
		m_charMap[ba] = totalTime;
	}
}

void CAnimTimer::SetBodyDelay(BodyActionMap &bodyMap, const TCHAR *filename, UInt16 sexClass,
							  bool bChar)
{
	if (m_entStateMap.find(filename) == m_entStateMap.end())
		return;

	DelayMap &delayMap = m_entStateMap[filename];
	BodyAction ba;
	UInt32 totalTime = 0;

	ba.sexClass = sexClass;

	// attack delay
	ba.action = ANIM_W1ATK1;
	bodyMap[ba] = GetTotal(delayMap[ANIM_W1ATK1]);
	ba.action = ANIM_W1ATK2;
	bodyMap[ba] = GetTotal(delayMap[ANIM_W1ATK2]);
	ba.action = ANIM_W1ATK1_S1;
	bodyMap[ba] = GetFirst(delayMap[ANIM_W1ATK1]);

	if (bChar) {
		ba.action = ANIM_W2ATK1;
		bodyMap[ba] = GetTotal(delayMap[ANIM_W2ATK1]);
		ba.action = ANIM_W2ATK2;
		bodyMap[ba] = GetTotal(delayMap[ANIM_W2ATK2]);
		ba.action = ANIM_W2ATK1_S1;
		bodyMap[ba] = GetFirst(delayMap[ANIM_W2ATK1]);
	}

	// use delay
	ba.action = ANIM_USE;
	bodyMap[ba] = GetTotal(delayMap[ANIM_USE]);

	// skill delay
	ba.action = ANIM_SKILL;
	bodyMap[ba] = GetTotal(delayMap[ANIM_SKILL]);

	// hurt delay
	ba.action = ANIM_HURT;
	bodyMap[ba] = GetTotal(delayMap[ANIM_HURT]);

}

void CAnimTimer::SetWeaponType()
{
	const ItemData *pItem;
	ItemDataList itemList;
	EntStateMap::iterator it2;
	TCHAR entName[32];
	UInt8 wt;
	
	// get only equipment
	gGlobal.m_resource->GetItemList(itemList, ITEM_FILTER_TYPE, ITEMTYPE_EQUIP);
	for (ItemDataList::iterator it = itemList.begin(); it != itemList.end(); ++it)
	{
		pItem = *it;
		if (pItem->eqType >= EQTYPE_GEAR)				// get only weapon
			continue;
		if (pItem->reqSexClass1 != 0 && pItem->eq_Outlook1[0] != _T('\0'))
		{
			_tcscpy(entName, pItem->eq_Outlook1);
			it2 = m_entStateMap.find(_tcslwr(entName));
			if (it2 != m_entStateMap.end())
			{
				DelayMap &delayMap = it2->second;
				if (delayMap.find(ANIM_W1ATK1) != delayMap.end())
					wt = 1;
				else if (delayMap.find(ANIM_W2ATK1) != delayMap.end())
					wt = 2;
				else
					wt = 0;
				m_weaponMap.insert(std::make_pair(
					WeaponType(pItem->item_id, pItem->reqSexClass1), wt));
			}
		}
		if (pItem->reqSexClass2 != 0 && pItem->eq_Outlook2[0] != _T('\0'))
		{
			_tcscpy(entName, pItem->eq_Outlook2);
			it2 = m_entStateMap.find(_tcslwr(entName));
			if (it2 != m_entStateMap.end()) {
				DelayMap &delayMap = it2->second;
				if (delayMap.find(ANIM_W1ATK1) != delayMap.end())
					wt = 1;
				else if (delayMap.find(ANIM_W2ATK1) != delayMap.end())
					wt = 2;
				else
					wt = 0;
				m_weaponMap.insert(std::make_pair(
					WeaponType(pItem->item_id, pItem->reqSexClass1), wt));
			}
		}
	}
}

void CAnimTimer::SetSkillDelay()
{
	const SkillData* pSkill;
	SkillAction sa;
	SkillDataList skillList;
	EntStateMap::iterator it2;
	TCHAR entName[32];
	UInt16 atkTime, atk1;
	UInt16 hurtTime, hurt1;
	UInt16 sparkTime, spark1;

	gGlobal.m_resource->GetSkillList(skillList);		// get all skill data

	for (SkillDataList::iterator it = skillList.begin(); it != skillList.end(); ++it)
	{
		pSkill = *it;
		// skip not displayable skill
		if ((pSkill->effect_Atk[0] == _T('\0') || pSkill->effect_Atk[0] == _T('0')) && 
			(pSkill->effect_Hurt[0] == _T('\0') || pSkill->effect_Hurt[0] == _T('0')) &&
			(pSkill->effect_Spark[0] == _T('\0') || pSkill->effect_Spark[0] == _T('0')))
			continue;

		atkTime = 0;	atk1 = 0;
		hurtTime = 0;	hurt1 = 0;
		sparkTime = 0;	spark1 = 0;

		// get skill attack time
		if (pSkill->effect_Atk[0] != _T('\0') && pSkill->effect_Atk[0] != _T('0'))
		{
			_tcscpy(entName, pSkill->effect_Atk);
			it2 = m_entStateMap.find(_tcslwr(entName));
			if (it2 != m_entStateMap.end()) {
				atk1 = GetFirst(it2->second[0]);
				atkTime = GetTotal(it2->second[0]);
			}
		}

		// get skill hurt time
		if (pSkill->effect_Hurt[0] != _T('\0') && pSkill->effect_Hurt[0] != _T('0')) {
			_tcscpy(entName, pSkill->effect_Hurt);
			it2 = m_entStateMap.find(_tcslwr(entName));
			if (it2 != m_entStateMap.end()) {
				hurt1 = GetFirst(it2->second[0]);
				hurtTime = atk1 + GetTotal(it2->second[0]);
			}
		}

		// get skill spark time
		if (pSkill->effect_Spark[0] != _T('\0') && pSkill->effect_Spark[0] != _T('0'))
		{
			_tcscpy(entName, pSkill->effect_Spark);
			it2 = m_entStateMap.find(_tcslwr(entName));
			if (it2 != m_entStateMap.end()) {
				spark1 = GetFirst(it2->second[0]);
				sparkTime = atk1 + hurt1 + GetTotal(it2->second[0]);
			}
		}

		sa.animType = pSkill->animation_Type;					// get animation type
		sa.motionType = pSkill->animation_Motion;				// get animation motion
		sa.reactTime = MAX(MAX(atkTime, hurtTime), sparkTime);	// get longest time
		if (sa.reactTime > 0)
			m_skillMap.insert(std::make_pair(pSkill->id, sa));
	}
}

UInt32 CAnimTimer::GetActionTime(CBatCharacter *pSrcChar, CBatMob *pSrcMob, UInt8 action,
								 UInt16 subId, UInt16 multi)
{
	if (pSrcChar == NULL && pSrcMob == NULL) return 0;

	UInt32 totalTime = 0;
	
	switch (action)
	{
	case BATCMD_ATTACK:
	case BATCMD_CRIT_ATTACK:
		{
			UInt8 animType;

			totalTime = m_actMoving;
			if (pSrcChar) {
				UInt8 wtype = 1;				// weapon type
				if (subId != 0) {
					WeaponTypeMap::iterator it2 = m_weaponMap.find(
						WeaponType(subId, pSrcChar->GetSexClass()));
					if (it2 != m_weaponMap.end())
						wtype = it2->second;
				}
				if (action == BATCMD_CRIT_ATTACK)
					animType = (wtype == 2 ? ANIM_W2ATK2 : ANIM_W1ATK2);
				else
					animType = (wtype == 2 ? ANIM_W2ATK1 : ANIM_W1ATK1);
				BodyActionMap::iterator it = m_charMap.find(
					BodyAction(pSrcChar->GetSexClass(), animType));
				if (it != m_charMap.end())
					totalTime += it->second * multi;
			} else if (pSrcMob) {
				animType = (action == BATCMD_CRIT_ATTACK ? ANIM_W1ATK2 : ANIM_W1ATK1);
				BodyActionMap::iterator it = m_mobMap.find(
					BodyAction(pSrcMob->GetMob_id(), animType));
				if (it != m_mobMap.end())
					totalTime += it->second * multi;
			}
		}
		break;

	case BATCMD_SKILL:
		{
			if (subId == 0)
				break;

			UInt8 animType;
			SkillActionMap::iterator it = m_skillMap.find(subId);
			if (it == m_skillMap.end())
				break;
			SkillAction &sa = it->second;

			totalTime = sa.reactTime;
			if (pSrcChar) {
				animType = (sa.animType == 1 ? ANIM_W1ATK1_S1 : 
						(sa.animType == 2 ? ANIM_W2ATK1_S1 : ANIM_SKILL));
				BodyActionMap::iterator it = m_charMap.find(
					BodyAction(pSrcChar->GetSexClass(), animType));
				if (it != m_charMap.end())
					totalTime += it->second;
			} else if (pSrcMob) {
				animType = (sa.animType != 0 ? ANIM_W1ATK1_S1 : ANIM_SKILL);
				BodyActionMap::iterator it = m_mobMap.find(
					BodyAction(pSrcMob->GetMob_id(), animType));
				if (it != m_mobMap.end())
					totalTime += it->second;
			}

			totalTime *= multi;

			if (sa.motionType == 1)				// motion time before casting skill
				totalTime += m_actMoving / 2;
			else if (sa.motionType == 2)
				totalTime += m_actMoving;
		}
		break;

	case BATCMD_ITEM:
		{
			if (pSrcChar) {
				BodyActionMap::iterator it = m_charMap.find(
					BodyAction(pSrcChar->GetSexClass(), ANIM_USE));
				if (it != m_charMap.end())
					totalTime += it->second;
			} else if (pSrcMob) {
				BodyActionMap::iterator it = m_mobMap.find(
					BodyAction(pSrcMob->GetMob_id(), ANIM_USE));
				if (it != m_mobMap.end())
					totalTime += it->second;
			}
		}
		break;

	case BATCMD_CATCH:
		{
			totalTime = m_catchMoving;
			/*
			if (pSrcChar) {
				BodyActionMap::iterator it = m_charMap.find(
					BodyAction(pSrcChar->GetSexClass(), ANIM_SKILL));
				if (it != m_charMap.end())
					totalTime += it->second;
			}
			*/
			BodyActionMap::iterator it2 = m_charMap.find(BodyAction(1, ANIM_CATCH));
			if (it2 != m_charMap.end())
				totalTime += it2->second;
		}
		break;

	case BATCMD_ESCAPE:
		{
			totalTime = m_escapeMoving + m_escaped;
		}
		break;

	case BATCMD_ESCAPE_FAIL:
		{
			totalTime = m_escapeMoving;
			if (pSrcChar) {
				BodyActionMap::iterator it = m_charMap.find(
					BodyAction(pSrcChar->GetSexClass(), ANIM_HURT));
				if (it != m_charMap.end())
					totalTime += it->second;
			} else if (pSrcMob) {
				BodyActionMap::iterator it = m_mobMap.find(
					BodyAction(pSrcMob->GetMob_id(), ANIM_HURT));
				if (it != m_mobMap.end())
					totalTime += it->second;
			}
		}
		break;
	case BATCMD_COUNTER_ATTACK_HIT:
	case BATCMD_COUNTER_ATTACK_DEF_HIT:
		{
			UInt8 animType = ANIM_W1ATK1;
			if (pSrcChar) {
				UInt8 wtype = 1;				// weapon type
				if (subId != 0) {
					WeaponTypeMap::iterator it2 = m_weaponMap.find(
						WeaponType(subId, pSrcChar->GetSexClass()));
					if (it2 != m_weaponMap.end())
						wtype = it2->second;
				}
				animType = (wtype == 2 ? ANIM_W2ATK1 : ANIM_W1ATK1);
				BodyActionMap::iterator it = m_charMap.find(
					BodyAction(pSrcChar->GetSexClass(), animType));
				if (it != m_charMap.end())
					totalTime += it->second;
			} else if (pSrcMob) {
				animType = ANIM_W1ATK1;
				BodyActionMap::iterator it = m_mobMap.find(
					BodyAction(pSrcMob->GetMob_id(), animType));
				if (it != m_mobMap.end())
					totalTime += it->second;
			}
			break;
		}
	}
	

	//if (totalTime > 0)
	//	TRACE_VERBOSEL_5(GLOBAL_LOGGER, _T("AnimTimer: time %d, actorId %d, action %d, subId %d, multi %d"), 
	//	totalTime, pSrcChar ? pSrcChar->GetChar_id() : pSrcMob->GetMob_id(), action, subId,
	//	multi);

	return totalTime;
}

void CAnimTimer::DebugInfo()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _T("CAnimTimer DebugInfo:"));
	TRACE_INFOL_4(GLOBAL_LOGGER, _T("actMoving: %d, escapeMoving %d, escaped %d, catchMoving %d"),
		m_actMoving, m_escapeMoving, m_escaped, m_catchMoving);

	TRACE_INFOL(GLOBAL_LOGGER, _T("charMap:"));
	for (BodyActionMap::iterator it = m_charMap.begin(); it != m_charMap.end(); ++it)
	{
		const BodyAction &ba = it->first;
		TRACE_INFOL_3(GLOBAL_LOGGER, _T("sexClass %d: action %d, time %d"), ba.sexClass,
			ba.action, it->second);
	}

	TRACE_INFOL(GLOBAL_LOGGER, _T("mobMap:"));
	for (BodyActionMap::iterator it = m_mobMap.begin(); it != m_mobMap.end(); ++it)
	{
		const BodyAction &ba = it->first;
		TRACE_INFOL_3(GLOBAL_LOGGER, _T("mob_id %d: action %d, time %d"), ba.mob_id,
			ba.action, it->second);
	}

	TRACE_INFOL(GLOBAL_LOGGER, _T("weaponMap:"));
	for (WeaponTypeMap::iterator it = m_weaponMap.begin(); it != m_weaponMap.end(); ++it)
	{
		const WeaponType &wt = it->first;
		TRACE_INFOL_3(GLOBAL_LOGGER, _T("item_id %d: sexClass %d, weaponType %d"), wt.item_id,
			wt.sexClass, it->second);
	}

	TRACE_INFOL(GLOBAL_LOGGER, _T("skillMap:"));
	for (SkillActionMap::iterator it = m_skillMap.begin(); it != m_skillMap.end(); ++it)
	{
		const SkillAction &sa = it->second;
		TRACE_INFOL_4(GLOBAL_LOGGER, _T("skill_id %d: animType %d, motionType %d, time %d"), 
			it->first, sa.animType, sa.motionType, sa.reactTime);
	}
}