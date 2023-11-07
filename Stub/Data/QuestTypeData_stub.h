///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTTYPEDATA_STUB_H_
#define _RPC_QUESTTYPEDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/QuestTypeData_enum.h"

class QuestTypeData_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		nameMask	= Parent::NextFreeMask + 1,
		roundMask	= Parent::NextFreeMask + 2,
		maxRoundMask	= Parent::NextFreeMask + 3,
		totalRoundMask	= Parent::NextFreeMask + 4,
		limitRoundMask	= Parent::NextFreeMask + 5,
		limitTypeMask	= Parent::NextFreeMask + 6,
		lastClearTimeMask	= Parent::NextFreeMask + 7,
		statusMask	= Parent::NextFreeMask + 8,
		NextFreeMask	= Parent::NextFreeMask + 9
	};
	const UInt32& GetChar_id() const { return m.char_id; }
	const UInt16& GetType_id() const { return m.Type_id; }
	const String& GetName() const { return m.name; }
	const UInt16& GetRound() const { return m.round; }
	const UInt16& GetMaxRound() const { return m.maxRound; }
	const UInt16& GetTotalRound() const { return m.totalRound; }
	const UInt16& GetLimitRound() const { return m.limitRound; }
	const UInt16& GetLimitType() const { return m.limitType; }
	const UInt32& GetLastClearTime() const { return m.lastClearTime; }
	const UInt8& GetStatus() const { return m.status; }
	void SetId(const UInt32 &char_id, const UInt16 &Type_id);
	void SetChar_id(const UInt32 &char_id);
	void SetType_id(const UInt16 &Type_id);
	void SetName(const String &name);
	void SetRound(const UInt16 &round);
	void SetMaxRound(const UInt16 &maxRound);
	void SetTotalRound(const UInt16 &totalRound);
	void SetLimitRound(const UInt16 &limitRound);
	void SetLimitType(const UInt16 &limitType);
	void SetLastClearTime(const UInt32 &lastClearTime);
	void SetStatus(const UInt8 &status);
	void Clone(QuestTypeData_Stub* obj, const RPCMask &mask);
	friend void QuestTypeData_Stub::Clone(QuestTypeData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 char_id;
	UInt16 Type_id;
	//name
	String name;
	//round
	UInt16 round;
	//maxRound
	UInt16 maxRound;
	//totalRound
	UInt16 totalRound;
	//limitRound
	UInt16 limitRound;
	//limitType
	UInt16 limitType;
	//lastClearTime
	UInt32 lastClearTime;
	//status
	UInt8 status;
} m;

public:
	INLINE QuestTypeData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_QUESTTYPEDATA_STUB_H_
