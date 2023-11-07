///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTLOGDATA_STUB_H_
#define _RPC_QUESTLOGDATA_STUB_H_

#include "Common/RPCFunc.h"
#ifndef Q_TARGET_NONE
#define Q_TARGET_NONE		0
#define Q_TARGET_NPC		1
#define Q_TARGET_ITEM		2
#define Q_TARGET_MOB		3
#define Q_TARGET_PARTNER	4
#define Q_TARGET_PET		5
#define Q_TARGET_MAP		6
#define Q_TARGET_NEWBIE		7
#define Q_TARGET_ENCOUNT	8
#endif
#include "RPCEnum/QuestLogData_enum.h"

class QuestLogData_Stub : public NetObject
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
		beginNpcMask	= Parent::NextFreeMask + 2,
		finishNpcMask	= Parent::NextFreeMask + 3,
		target1Mask	= Parent::NextFreeMask + 4,
		target2Mask	= Parent::NextFreeMask + 5,
		target3Mask	= Parent::NextFreeMask + 6,
		target4Mask	= Parent::NextFreeMask + 7,
		target5Mask	= Parent::NextFreeMask + 8,
		timerMask	= Parent::NextFreeMask + 9,
		buffMask	= Parent::NextFreeMask + 10,
		stateMask	= Parent::NextFreeMask + 11,
		featureMask	= Parent::NextFreeMask + 12,
		statusMask	= Parent::NextFreeMask + 13,
		NextFreeMask	= Parent::NextFreeMask + 14
	};
	const UInt32& GetChar_id() const { return m.char_id; }
	const UInt16& GetQuest_id() const { return m.quest_id; }
	const UInt16& GetType_id() const { return m.Type_id; }
	const String& GetName() const { return m.name; }
	const String& GetDescript() const { return m.descript; }
	const UInt32& GetBeginNpc() const { return m.beginNpc; }
	const UInt16& GetBeginNpc_Map() const { return m.beginNpc_Map; }
	const UInt16& GetBeginNpc_x() const { return m.beginNpc_x; }
	const UInt16& GetBeginNpc_y() const { return m.beginNpc_y; }
	const UInt32& GetFinishNpc() const { return m.finishNpc; }
	const UInt16& GetFinishNpc_Map() const { return m.finishNpc_Map; }
	const UInt16& GetFinishNpc_x() const { return m.finishNpc_x; }
	const UInt16& GetFinishNpc_y() const { return m.finishNpc_y; }
	const UInt32& GetTarget1_id() const { return m.target1_id; }
	const UInt16& GetTarget1_v1() const { return m.target1_v1; }
	const UInt16& GetTarget1_v2() const { return m.target1_v2; }
	const UInt16& GetTarget1_v3() const { return m.target1_v3; }
	const UInt8& GetTarget1_vtype() const { return m.target1_vtype; }
	const UInt16& GetTarget1_num() const { return m.target1_num; }
	const UInt16& GetTarget1_max() const { return m.target1_max; }
	const UInt32& GetTarget2_id() const { return m.target2_id; }
	const UInt16& GetTarget2_v1() const { return m.target2_v1; }
	const UInt16& GetTarget2_v2() const { return m.target2_v2; }
	const UInt16& GetTarget2_v3() const { return m.target2_v3; }
	const UInt8& GetTarget2_vtype() const { return m.target2_vtype; }
	const UInt16& GetTarget2_num() const { return m.target2_num; }
	const UInt16& GetTarget2_max() const { return m.target2_max; }
	const UInt32& GetTarget3_id() const { return m.target3_id; }
	const UInt16& GetTarget3_v1() const { return m.target3_v1; }
	const UInt16& GetTarget3_v2() const { return m.target3_v2; }
	const UInt16& GetTarget3_v3() const { return m.target3_v3; }
	const UInt8& GetTarget3_vtype() const { return m.target3_vtype; }
	const UInt16& GetTarget3_num() const { return m.target3_num; }
	const UInt16& GetTarget3_max() const { return m.target3_max; }
	const UInt32& GetTarget4_id() const { return m.target4_id; }
	const UInt16& GetTarget4_v1() const { return m.target4_v1; }
	const UInt16& GetTarget4_v2() const { return m.target4_v2; }
	const UInt16& GetTarget4_v3() const { return m.target4_v3; }
	const UInt8& GetTarget4_vtype() const { return m.target4_vtype; }
	const UInt16& GetTarget4_num() const { return m.target4_num; }
	const UInt16& GetTarget4_max() const { return m.target4_max; }
	const UInt32& GetTarget5_id() const { return m.target5_id; }
	const UInt16& GetTarget5_v1() const { return m.target5_v1; }
	const UInt16& GetTarget5_v2() const { return m.target5_v2; }
	const UInt16& GetTarget5_v3() const { return m.target5_v3; }
	const UInt8& GetTarget5_vtype() const { return m.target5_vtype; }
	const UInt16& GetTarget5_num() const { return m.target5_num; }
	const UInt16& GetTarget5_max() const { return m.target5_max; }
	const UInt32& GetTimeLimit() const { return m.timeLimit; }
	const UInt16& GetInterval() const { return m.interval; }
	const UInt16& GetBuff() const { return m.buff; }
	const UInt8& GetState() const { return m.state; }
	const UInt16& GetFeature() const { return m.feature; }
	const UInt8& GetStatus() const { return m.status; }
	void SetId(const UInt32 &char_id, const UInt16 &quest_id);
	void SetChar_id(const UInt32 &char_id);
	void SetQuest_id(const UInt16 &quest_id);
	void SetInfo(const UInt16 &Type_id, const String &name, const String &descript);
	void SetType_id(const UInt16 &Type_id);
	void SetName(const String &name);
	void SetDescript(const String &descript);
	void SetBeginNpc(const UInt32 &beginNpc, const UInt16 &beginNpc_Map, const UInt16 &beginNpc_x, const UInt16 &beginNpc_y);
	void SetBeginNpc(const UInt32 &beginNpc);
	void SetBeginNpc_Map(const UInt16 &beginNpc_Map);
	void SetBeginNpc_x(const UInt16 &beginNpc_x);
	void SetBeginNpc_y(const UInt16 &beginNpc_y);
	void SetFinishNpc(const UInt32 &finishNpc, const UInt16 &finishNpc_Map, const UInt16 &finishNpc_x, const UInt16 &finishNpc_y);
	void SetFinishNpc(const UInt32 &finishNpc);
	void SetFinishNpc_Map(const UInt16 &finishNpc_Map);
	void SetFinishNpc_x(const UInt16 &finishNpc_x);
	void SetFinishNpc_y(const UInt16 &finishNpc_y);
	void SetTarget1(const UInt32 &target1_id, const UInt16 &target1_v1, const UInt16 &target1_v2, const UInt16 &target1_v3, const UInt8 &target1_vtype, const UInt16 &target1_num, const UInt16 &target1_max);
	void SetTarget1_id(const UInt32 &target1_id);
	void SetTarget1_v1(const UInt16 &target1_v1);
	void SetTarget1_v2(const UInt16 &target1_v2);
	void SetTarget1_v3(const UInt16 &target1_v3);
	void SetTarget1_vtype(const UInt8 &target1_vtype);
	void SetTarget1_num(const UInt16 &target1_num);
	void SetTarget1_max(const UInt16 &target1_max);
	void SetTarget2(const UInt32 &target2_id, const UInt16 &target2_v1, const UInt16 &target2_v2, const UInt16 &target2_v3, const UInt8 &target2_vtype, const UInt16 &target2_num, const UInt16 &target2_max);
	void SetTarget2_id(const UInt32 &target2_id);
	void SetTarget2_v1(const UInt16 &target2_v1);
	void SetTarget2_v2(const UInt16 &target2_v2);
	void SetTarget2_v3(const UInt16 &target2_v3);
	void SetTarget2_vtype(const UInt8 &target2_vtype);
	void SetTarget2_num(const UInt16 &target2_num);
	void SetTarget2_max(const UInt16 &target2_max);
	void SetTarget3(const UInt32 &target3_id, const UInt16 &target3_v1, const UInt16 &target3_v2, const UInt16 &target3_v3, const UInt8 &target3_vtype, const UInt16 &target3_num, const UInt16 &target3_max);
	void SetTarget3_id(const UInt32 &target3_id);
	void SetTarget3_v1(const UInt16 &target3_v1);
	void SetTarget3_v2(const UInt16 &target3_v2);
	void SetTarget3_v3(const UInt16 &target3_v3);
	void SetTarget3_vtype(const UInt8 &target3_vtype);
	void SetTarget3_num(const UInt16 &target3_num);
	void SetTarget3_max(const UInt16 &target3_max);
	void SetTarget4(const UInt32 &target4_id, const UInt16 &target4_v1, const UInt16 &target4_v2, const UInt16 &target4_v3, const UInt8 &target4_vtype, const UInt16 &target4_num, const UInt16 &target4_max);
	void SetTarget4_id(const UInt32 &target4_id);
	void SetTarget4_v1(const UInt16 &target4_v1);
	void SetTarget4_v2(const UInt16 &target4_v2);
	void SetTarget4_v3(const UInt16 &target4_v3);
	void SetTarget4_vtype(const UInt8 &target4_vtype);
	void SetTarget4_num(const UInt16 &target4_num);
	void SetTarget4_max(const UInt16 &target4_max);
	void SetTarget5(const UInt32 &target5_id, const UInt16 &target5_v1, const UInt16 &target5_v2, const UInt16 &target5_v3, const UInt8 &target5_vtype, const UInt16 &target5_num, const UInt16 &target5_max);
	void SetTarget5_id(const UInt32 &target5_id);
	void SetTarget5_v1(const UInt16 &target5_v1);
	void SetTarget5_v2(const UInt16 &target5_v2);
	void SetTarget5_v3(const UInt16 &target5_v3);
	void SetTarget5_vtype(const UInt8 &target5_vtype);
	void SetTarget5_num(const UInt16 &target5_num);
	void SetTarget5_max(const UInt16 &target5_max);
	void SetTimer(const UInt32 &timeLimit, const UInt16 &interval);
	void SetTimeLimit(const UInt32 &timeLimit);
	void SetInterval(const UInt16 &interval);
	void SetBuff(const UInt16 &buff);
	void SetState(const UInt8 &state);
	void SetFeature(const UInt16 &feature);
	void SetStatus(const UInt8 &status);
	void Clone(QuestLogData_Stub* obj, const RPCMask &mask);
	friend void QuestLogData_Stub::Clone(QuestLogData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 char_id;
	UInt16 quest_id;
	//info
	UInt16 Type_id;
	String name;
	String descript;
	//beginNpc
	UInt32 beginNpc;
	UInt16 beginNpc_Map;
	UInt16 beginNpc_x;
	UInt16 beginNpc_y;
	//finishNpc
	UInt32 finishNpc;
	UInt16 finishNpc_Map;
	UInt16 finishNpc_x;
	UInt16 finishNpc_y;
	//target1
	UInt32 target1_id;
	UInt16 target1_v1;
	UInt16 target1_v2;
	UInt16 target1_v3;
	UInt8 target1_vtype;
	UInt16 target1_num;
	UInt16 target1_max;
	//target2
	UInt32 target2_id;
	UInt16 target2_v1;
	UInt16 target2_v2;
	UInt16 target2_v3;
	UInt8 target2_vtype;
	UInt16 target2_num;
	UInt16 target2_max;
	//target3
	UInt32 target3_id;
	UInt16 target3_v1;
	UInt16 target3_v2;
	UInt16 target3_v3;
	UInt8 target3_vtype;
	UInt16 target3_num;
	UInt16 target3_max;
	//target4
	UInt32 target4_id;
	UInt16 target4_v1;
	UInt16 target4_v2;
	UInt16 target4_v3;
	UInt8 target4_vtype;
	UInt16 target4_num;
	UInt16 target4_max;
	//target5
	UInt32 target5_id;
	UInt16 target5_v1;
	UInt16 target5_v2;
	UInt16 target5_v3;
	UInt8 target5_vtype;
	UInt16 target5_num;
	UInt16 target5_max;
	//timer
	UInt32 timeLimit;
	UInt16 interval;
	//buff
	UInt16 buff;
	//state
	UInt8 state;
	//feature
	UInt16 feature;
	//status
	UInt8 status;
} m;

public:
	INLINE QuestLogData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_QUESTLOGDATA_STUB_H_
