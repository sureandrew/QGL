///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_ACCOUNT_STUB_H_
#define _RPC_ACCOUNT_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Account_enum.h"

class Account_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult GetOwnCharacters(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CheckCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CheckCharacterName(LPCPEER pPeer, RPCContext &context, const String &nickName);
	virtual RPCResult SelectCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &line_id);
	virtual RPCResult Logout(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult AddCharacter(LPCPEER pPeer, RPCContext &context, const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt16 &hairStyle, const UInt16 &hairColor, const UInt16 &clothStyle, const UInt16 &clothColor);
	virtual RPCResult RemoveCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult FreezeCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult RecoverCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		addictMask	= Parent::NextFreeMask + 1,
		banTimeMask	= Parent::NextFreeMask + 2,
		addictTypeMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	virtual RPCResult GetOwnCharacters(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CheckCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult CheckCharacterName(LPCPEER pPeer, RPCContext &context, const String &nickName) = 0;
	virtual RPCResult SelectCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &line_id) = 0;
	virtual RPCResult Logout(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult AddCharacter(LPCPEER pPeer, RPCContext &context, const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt16 &hairStyle, const UInt16 &hairColor, const UInt16 &clothStyle, const UInt16 &clothColor) = 0;
	virtual RPCResult RemoveCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult FreezeCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult RecoverCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
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
	const UInt32& GetAcct_id() const { return m.acct_id; }
	const String& GetLoginName() const { return m.loginName; }
	const UInt32& GetDisconnect_time() const { return m.disconnect_time; }
	const UInt32& GetOffLnTime() const { return m.offLnTime; }
	const UInt32& GetOnLnTime() const { return m.onLnTime; }
	const UInt32& GetBanTime() const { return m.banTime; }
	const Boolean& GetAddictType() const { return m.addictType; }
	void SetInfo(const UInt32 &acct_id, const String &loginName);
	void SetAcct_id(const UInt32 &acct_id);
	void SetLoginName(const String &loginName);
	void SetAddict(const UInt32 &disconnect_time, const UInt32 &offLnTime, const UInt32 &onLnTime);
	void SetDisconnect_time(const UInt32 &disconnect_time);
	void SetOffLnTime(const UInt32 &offLnTime);
	void SetOnLnTime(const UInt32 &onLnTime);
	void SetBanTime(const UInt32 &banTime);
	void SetAddictType(const Boolean &addictType);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//info
	UInt32 acct_id;
	String loginName;
	//addict
	UInt32 disconnect_time;
	UInt32 offLnTime;
	UInt32 onLnTime;
	//banTime
	UInt32 banTime;
	//addictType
	Boolean addictType;
} m;

public:
	INLINE Account_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_ACCOUNT_STUB_H_
