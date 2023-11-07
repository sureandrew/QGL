#pragma once
#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include "Proxy/Logic/Account_Proxy.h"

class CSimClient;

class CAccount : public Account_Proxy
{
public:
	//callback method
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_CheckCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &status, const Int32Vector &lineCounts, const UInt16Vector &lineState);
	virtual RPCResult CB_GetOwnCharacters(LPCPEER pPeer, RPCContext &context, NetGroup* charInfos);
	virtual RPCResult CB_SelectCharacter(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_LogoutSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_CreateSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_RemoveCharSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_CheckCharacterName(LPCPEER pPeer, RPCContext &context, const UInt8 &index);
	virtual RPCResult CB_RecoverCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo);
	virtual RPCResult CB_FreezeCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo);	


	virtual RPCResult CB_AddictionWarning(LPCPEER pPeer, RPCContext &context, const String &warnMsg);
	virtual RPCResult CB_AddictCurStatus(LPCPEER pPeer, RPCContext &context, const String &statusMsg, const float &plyInterest);
	virtual RPCResult CB_AddictionShutDown(LPCPEER pPeer, RPCContext &context, const String &warnMsg, const UInt32 &accOffTime);
	virtual RPCResult CB_AddictionInterestChg(LPCPEER pPeer, RPCContext &context, const float &interestChg);	

	virtual void OnUpdateInfo(const UInt32 &acct_id, const String &loginName);
	virtual void OnUpdateStatus(const UInt8 &status);

	CAccount();
	virtual ~CAccount();
	DEFINE_CLASS(Account_CLASSID);
	RPCResult LoginAccount(RPCContext &context);
	RPCResult Logout(RPCContext &context);
	RPCResult SelectCharacter(RPCContext &context);

	enum {
		ST_CHAR_LEAVE = 1,
		ST_CHAR_ENTER,
		ST_DISCONNECT,
	};

private:
	typedef Account_Proxy Parent;
};

#endif //_ACCOUNT_H_
