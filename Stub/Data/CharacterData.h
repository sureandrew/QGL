#pragma once
#ifndef _CHARACTERDATA_H_
#define _CHARACTERDATA_H_

#include "Stub/Data/CharacterData_Stub.h"

class CdboCharacter;
class CCharacterData : public CharacterData_Stub
{
public:
	DEFINE_CLASS(CharacterData_CLASSID);
	CCharacterData() : m_detail(DETAIL_NONE) {}
	static CCharacterData * NewInstance();
	void SetData(CdboCharacter* dboChar);
	void SetDetail(UInt8 detail);
	HRESULT UpdateDbo(CdboCharacter* dboChar);
	HRESULT DeleteDbo(CdboCharacter* dboChar);
	void ReleaseBelong();
	static void SetGroupDetail(NetGroup *charGroup, UInt8 detail);
	virtual String DebugInfo();

	enum DataDetail {
		DETAIL_NONE		= 0,
		DETAIL_INFO,
		DETAIL_LOC,
		DETAIL_MAP,
		DETAIL_BATTLE,
		DETAIL_MESSAGE,
		DETAIL_BATTLE_SYNC,
		DETAIL_FOLLOW,
		DETAIL_PARTY,
		DETAIL_RANK,
		DETAIL_ALL,
	};

private:
	typedef CharacterData_Stub Parent;
	UInt8	m_detail;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CCharacterData * CCharacterData::NewInstance()
{
	return CreateNetObject<CCharacterData>();
}

CCharacterData* FindChar(UInt32 char_id);
void ReleaseCharacter(UInt32 char_id);

#endif //_CHARACTERDATA_H_
