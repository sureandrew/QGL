///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_RANKCHARACTER_STUB_H_
#define _RPC_RANKCHARACTER_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/RankCharacter_enum.h"

class RankCharacter_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	void SetId(const UInt32 &acct_id, const UInt32 &char_id);
	void SetAcct_id(const UInt32 &acct_id);
	void SetChar_id(const UInt32 &char_id);
	void SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction);
	void SetNickName(const String &nickName);
	void SetSex(const UInt8 &sex);
	void SetCclass(const UInt8 &cclass);
	void SetSexClass(const UInt8 &sexClass);
	void SetFaction(const UInt8 &faction);
	void SetLevel(const UInt16 &level);
	void SetMoney(const UInt32 &money);
	void SetPKInfo(const UInt16 &PK_Value, const UInt32 &wantedReward);
	void SetPK_Value(const UInt16 &PK_Value);
	void SetWantedReward(const UInt32 &wantedReward);
	void SetHonor(const UInt32 &honor);
	void SetBankMoney(const UInt32 &bankMoney);
	void SetPvPScoreSingle(const UInt32 &PvPScoreSingle);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE RankCharacter_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_RANKCHARACTER_STUB_H_
