///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_RANKCHARACTER_PROXY_H_
#define _RPC_RANKCHARACTER_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/RankCharacter_enum.h"

class RankCharacter_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &acct_id, const UInt32 &char_id);
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateMoney(const UInt32 &money);
	virtual void OnUpdatePKInfo(const UInt16 &PK_Value, const UInt32 &wantedReward);
	virtual void OnUpdateHonor(const UInt32 &honor);
	virtual void OnUpdateBankMoney(const UInt32 &bankMoney);
	virtual void OnUpdatePvPScoreSingle(const UInt32 &PvPScoreSingle);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		infoMask	= Parent::NextFreeMask + 1,
		levelMask	= Parent::NextFreeMask + 2,
		moneyMask	= Parent::NextFreeMask + 3,
		PKInfoMask	= Parent::NextFreeMask + 4,
		honorMask	= Parent::NextFreeMask + 5,
		bankMoneyMask	= Parent::NextFreeMask + 6,
		PvPScoreSingleMask	= Parent::NextFreeMask + 7,
		NextFreeMask	= Parent::NextFreeMask + 8
	};
	const UInt32& GetAcct_id() const { return m.acct_id; }
	const UInt32& GetChar_id() const { return m.char_id; }
	const String& GetNickName() const { return m.nickName; }
	const UInt8& GetSex() const { return m.sex; }
	const UInt8& GetCclass() const { return m.cclass; }
	const UInt8& GetSexClass() const { return m.sexClass; }
	const UInt8& GetFaction() const { return m.faction; }
	const UInt16& GetLevel() const { return m.level; }
	const UInt32& GetMoney() const { return m.money; }
	const UInt16& GetPK_Value() const { return m.PK_Value; }
	const UInt32& GetWantedReward() const { return m.wantedReward; }
	const UInt32& GetHonor() const { return m.honor; }
	const UInt32& GetBankMoney() const { return m.bankMoney; }
	const UInt32& GetPvPScoreSingle() const { return m.PvPScoreSingle; }
	virtual void OnUpdateId(const UInt32 &acct_id, const UInt32 &char_id) {};
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction) {};
	virtual void OnUpdateLevel(const UInt16 &level) {};
	virtual void OnUpdateMoney(const UInt32 &money) {};
	virtual void OnUpdatePKInfo(const UInt16 &PK_Value, const UInt32 &wantedReward) {};
	virtual void OnUpdateHonor(const UInt32 &honor) {};
	virtual void OnUpdateBankMoney(const UInt32 &bankMoney) {};
	virtual void OnUpdatePvPScoreSingle(const UInt32 &PvPScoreSingle) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 acct_id;
	UInt32 char_id;
	//info
	String nickName;
	UInt8 sex;
	UInt8 cclass;
	UInt8 sexClass;
	UInt8 faction;
	//level
	UInt16 level;
	//money
	UInt32 money;
	//PKInfo
	UInt16 PK_Value;
	UInt32 wantedReward;
	//honor
	UInt32 honor;
	//bankMoney
	UInt32 bankMoney;
	//PvPScoreSingle
	UInt32 PvPScoreSingle;
} m;

public:
	INLINE RankCharacter_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult RankCharacter_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_RankCharacter_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1978038091>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(307298890)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1978038091)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_RANKCHARACTER_PROXY_H_
