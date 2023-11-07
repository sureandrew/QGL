#pragma once
#ifndef _MSGCHARACTERDATA_H_
#define _MSGCHARACTERDATA_H_

#include "Stub/Data/MsgCharacterData_Stub.h"

class CdboMsgCharacter;
class CCharacterData;
class CMsgCharacterData : public MsgCharacterData_Stub
{
public:
	DEFINE_CLASS(MsgCharacterData_CLASSID);
	CMsgCharacterData();
	~CMsgCharacterData();
	void SetData(CdboMsgCharacter* dboMsgChar, bool inactive=false);
	void SetData(CCharacterData* pData, BOOL setAll = FALSE);
	HRESULT UpdateDbo(CdboMsgCharacter* dboMsgChar);
	HRESULT DeleteDbo(CdboMsgCharacter* dboMsgChar);
	void PreSend();
	static CMsgCharacterData * NewInstance();

private:
	typedef MsgCharacterData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CMsgCharacterData * CMsgCharacterData::NewInstance()
{
	return CreateNetObject<CMsgCharacterData>();
}

CMsgCharacterData* FindMsgChar(UInt32 char_id);

#endif //_MSGCHARACTERDATA_H_
