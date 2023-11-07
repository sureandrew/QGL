#pragma once
#ifndef _CHARACTERINFO_H_
#define _CHARACTERINFO_H_

#include "Stub/Logic/CharacterInfo_Stub.h"

class CCharacterData;
class CCharacterInfo : public CharacterInfo_Stub
{
public:
	DEFINE_CLASS(CharacterInfo_CLASSID);
	inline static CCharacterInfo* NewInstance();
	void SetData(CCharacterData *pCharData);	// set character by CCharacterData
	void WriteData(CCharacterData *pCharData);	// write character to CCharacterData
	void PackBuffer(Buffer &buffer);			// pack buffer using CCharacterData
	void Connect(const SessionID& sid);
	void Disconnect(const SessionID& sid);
	void Release(const SessionID& sid);
	Boolean IsLeave() { return GetStatus() == ST_LEAVE; }
	Boolean IsEnter() { return GetStatus() == ST_ENTER; }
	Boolean IsDisconnect() { return GetStatus() == ST_DISCONNECT; }
	Boolean ValidateMap();						// validate current map available
	CCharacterInfo();
	virtual ~CCharacterInfo();

	enum CharStatus {
		ST_LEAVE = 1,
		ST_ENTER,
		ST_DISCONNECT,
		ST_NOT_FOUND,
		ST_NOT_AVAIL,
	};

private:
	typedef CharacterInfo_Stub Parent;
};

inline CCharacterInfo* CCharacterInfo::NewInstance()
{
	return CreateNetObject<CCharacterInfo>();
}

#endif //_CHARACTERINFO_H_
