#pragma once
#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include "Common.h"
#include "Stub/Logic/Account_stub.h"
#include <vector>

class CAccountData;
class CCharacterInfo;
class CAntiGameAddiction;

class CAccount : public Account_Stub
{
public:
	DEFINE_CLASS(Account_CLASSID);	
	CAccount();
	virtual ~CAccount();
	void ClearCharacters();
	void SetData(CAccountData* pAcctData);
	void SetSessionId(const SessionID& sessId);
	void ClearSessionId();
	INLINE Boolean IsConnected() { return m_bConnect; }
	void Connect(const SessionID& sid);
	void Disconnect(const SessionID& sid);
	CCharacterInfo* GetCharInfo(UInt32 char_id);
	CCharacterInfo* GetLoginChar();

	void UpdateToObject(RPCContext &context, UInt32 actID, UInt32 offTm);

	virtual RPCResult GetOwnCharacters(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CheckCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult SelectCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &line_id);
	virtual RPCResult AddCharacter(LPCPEER pPeer, RPCContext &context, const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt16 &hairStyle, const UInt16 &hairColor,  const UInt16 &clothStyle, const UInt16 &clothColor);
	virtual RPCResult RemoveCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult Logout(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CheckCharacterName(LPCPEER pPeer, RPCContext &context, const String &nickName);
	virtual RPCResult RecoverCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);	
	virtual RPCResult FreezeCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	
	static CAccount* NewInstance();
	virtual String DebugInfo();


	SessionID		m_lastSessId;			// last login session ID
	SessionID		m_waitSessId;			// waiting to login session ID
	SessionID		m_selSessId;			// last select character session ID
	//CCharacterInfo*	m_pCharInfo;			// last selected character

private:
	NetGroup		m_characters;
	Boolean			m_bConnect;				// client connected
};

INLINE CAccount* CAccount::NewInstance()
{
	return CreateNetObject<CAccount>();
}

#endif //_ACCOUNT_H_
