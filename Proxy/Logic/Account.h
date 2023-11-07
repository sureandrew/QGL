#pragma once
#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include "Reuben/Simulation/State.h"
#include "Proxy/Logic/Account_Proxy.h"
#include "Proxy/Logic/CharacterInfo.h"
#include <vector>

class CAccount : public Account_Proxy
{
public:
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_LogoutSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_GetOwnCharacters(LPCPEER pPeer, RPCContext &context, NetGroup* charInfos);
	virtual RPCResult CB_CheckCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &status, const Int32Vector &lineCounts, const UInt16Vector &lineState);
	virtual RPCResult CB_SelectCharacter(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_CheckCharacterName(LPCPEER pPeer, RPCContext &context, const UInt8 &index);

	virtual void OnUpdateInfo(const UInt32 &acct_id, const String &loginName);
	virtual void OnUpdateStatus(const UInt8 &status);

	// anti-addiction

	CAccount();
	~CAccount();
	DEFINE_CLASS(Account_CLASSID);

	enum {		// account status
		ST_CHAR_LEAVE = 1,
		ST_CHAR_ENTER,
		ST_DISCONNECT,
	};

	const String &GetAccName() const{ return m_AccName;}

private:
	typedef Account_Proxy Parent;
	virtual RPCResult CB_CreateSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_RemoveCharSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_RecoverCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo);
	virtual RPCResult CB_FreezeCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo);

private:
	String m_AccName;
};

#endif //_ACCOUNT_H_