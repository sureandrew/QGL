///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTLOG_PROXY_H_
#define _RPC_QUESTLOG_PROXY_H_

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
#define Q_TARGET_FOLLOWER	9
#endif
#include "RPCEnum/QuestLog_enum.h"

class QuestLog_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &char_id, const UInt16 &quest_id);
	virtual void OnUpdateInfo(const UInt16 &Type_id, const String &name, const String &descript);
	virtual void OnUpdateBeginNpc(const UInt32 &beginNpc, const UInt16 &beginNpc_Map, const UInt16 &beginNpc_x, const UInt16 &beginNpc_y);
	virtual void OnUpdateFinishNpc(const UInt32 &finishNpc, const UInt16 &finishNpc_Map, const UInt16 &finishNpc_x, const UInt16 &finishNpc_y);
	virtual void OnUpdateTarget1(const UInt32 &target1_id, const UInt16 &target1_v1, const UInt16 &target1_v2, const UInt16 &target1_v3, const UInt8 &target1_vtype, const UInt16 &target1_num, const UInt16 &target1_max);
	virtual void OnUpdateTarget2(const UInt32 &target2_id, const UInt16 &target2_v1, const UInt16 &target2_v2, const UInt16 &target2_v3, const UInt8 &target2_vtype, const UInt16 &target2_num, const UInt16 &target2_max);
	virtual void OnUpdateTarget3(const UInt32 &target3_id, const UInt16 &target3_v1, const UInt16 &target3_v2, const UInt16 &target3_v3, const UInt8 &target3_vtype, const UInt16 &target3_num, const UInt16 &target3_max);
	virtual void OnUpdateTarget4(const UInt32 &target4_id, const UInt16 &target4_v1, const UInt16 &target4_v2, const UInt16 &target4_v3, const UInt8 &target4_vtype, const UInt16 &target4_num, const UInt16 &target4_max);
	virtual void OnUpdateTarget5(const UInt32 &target5_id, const UInt16 &target5_v1, const UInt16 &target5_v2, const UInt16 &target5_v3, const UInt8 &target5_vtype, const UInt16 &target5_num, const UInt16 &target5_max);
	virtual void OnUpdateTimer(const UInt32 &timeLimit, const UInt16 &interval);
	virtual void OnUpdateBuff(const UInt16 &buff);
	virtual void OnUpdateState(const UInt8 &state);
	virtual void OnUpdateFeature(const UInt16 &feature);
	virtual void OnUpdateStatus(const UInt8 &status);
	virtual void OnUpdateTarget1_name(const String &target1_name);
	virtual void OnUpdateTarget2_name(const String &target2_name);
	virtual void OnUpdateTarget3_name(const String &target3_name);
	virtual void OnUpdateTarget4_name(const String &target4_name);
	virtual void OnUpdateTarget5_name(const String &target5_name);
	virtual void OnUpdateUpdateIcon(const Boolean &updateIcon);
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
		target1_nameMask	= Parent::NextFreeMask + 14,
		target2_nameMask	= Parent::NextFreeMask + 15,
		target3_nameMask	= Parent::NextFreeMask + 16,
		target4_nameMask	= Parent::NextFreeMask + 17,
		target5_nameMask	= Parent::NextFreeMask + 18,
		updateIconMask	= Parent::NextFreeMask + 19,
		NextFreeMask	= Parent::NextFreeMask + 20
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
	const String& GetTarget1_name() const { return m.target1_name; }
	const String& GetTarget2_name() const { return m.target2_name; }
	const String& GetTarget3_name() const { return m.target3_name; }
	const String& GetTarget4_name() const { return m.target4_name; }
	const String& GetTarget5_name() const { return m.target5_name; }
	const Boolean& GetUpdateIcon() const { return m.updateIcon; }
	virtual void OnUpdateId(const UInt32 &char_id, const UInt16 &quest_id) {};
	virtual void OnUpdateInfo(const UInt16 &Type_id, const String &name, const String &descript) {};
	virtual void OnUpdateBeginNpc(const UInt32 &beginNpc, const UInt16 &beginNpc_Map, const UInt16 &beginNpc_x, const UInt16 &beginNpc_y) {};
	virtual void OnUpdateFinishNpc(const UInt32 &finishNpc, const UInt16 &finishNpc_Map, const UInt16 &finishNpc_x, const UInt16 &finishNpc_y) {};
	virtual void OnUpdateTarget1(const UInt32 &target1_id, const UInt16 &target1_v1, const UInt16 &target1_v2, const UInt16 &target1_v3, const UInt8 &target1_vtype, const UInt16 &target1_num, const UInt16 &target1_max) {};
	virtual void OnUpdateTarget2(const UInt32 &target2_id, const UInt16 &target2_v1, const UInt16 &target2_v2, const UInt16 &target2_v3, const UInt8 &target2_vtype, const UInt16 &target2_num, const UInt16 &target2_max) {};
	virtual void OnUpdateTarget3(const UInt32 &target3_id, const UInt16 &target3_v1, const UInt16 &target3_v2, const UInt16 &target3_v3, const UInt8 &target3_vtype, const UInt16 &target3_num, const UInt16 &target3_max) {};
	virtual void OnUpdateTarget4(const UInt32 &target4_id, const UInt16 &target4_v1, const UInt16 &target4_v2, const UInt16 &target4_v3, const UInt8 &target4_vtype, const UInt16 &target4_num, const UInt16 &target4_max) {};
	virtual void OnUpdateTarget5(const UInt32 &target5_id, const UInt16 &target5_v1, const UInt16 &target5_v2, const UInt16 &target5_v3, const UInt8 &target5_vtype, const UInt16 &target5_num, const UInt16 &target5_max) {};
	virtual void OnUpdateTimer(const UInt32 &timeLimit, const UInt16 &interval) {};
	virtual void OnUpdateBuff(const UInt16 &buff) {};
	virtual void OnUpdateState(const UInt8 &state) {};
	virtual void OnUpdateFeature(const UInt16 &feature) {};
	virtual void OnUpdateStatus(const UInt8 &status) {};
	virtual void OnUpdateTarget1_name(const String &target1_name) {};
	virtual void OnUpdateTarget2_name(const String &target2_name) {};
	virtual void OnUpdateTarget3_name(const String &target3_name) {};
	virtual void OnUpdateTarget4_name(const String &target4_name) {};
	virtual void OnUpdateTarget5_name(const String &target5_name) {};
	virtual void OnUpdateUpdateIcon(const Boolean &updateIcon) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

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
	//target1_name
	String target1_name;
	//target2_name
	String target2_name;
	//target3_name
	String target3_name;
	//target4_name
	String target4_name;
	//target5_name
	String target5_name;
	//updateIcon
	Boolean updateIcon;
} m;

public:
	INLINE QuestLog_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult QuestLog_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_QuestLog_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<350555199>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(786441412)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(350555199)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_QUESTLOG_PROXY_H_
