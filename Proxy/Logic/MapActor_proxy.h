///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MAPACTOR_PROXY_H_
#define _RPC_MAPACTOR_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/MapActor_enum.h"

class MapActor_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction);
	virtual void OnUpdateState(const UInt8 &state);
	virtual void OnUpdateMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt8 &direction, const UInt32 &map_ownerId);
	virtual void OnUpdateReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y);
	virtual void OnUpdateCurPos(const UInt16 &curPosX, const UInt16 &curPosY);
	virtual void OnUpdateDstPos(const UInt16 &dstPosX, const UInt16 &dstPosY);
	virtual void OnUpdateAction(const UInt16 &action, const UInt32 &actionTime);
	virtual void OnUpdateSpeed(const UInt32 &speed);
	virtual void OnUpdateFollowId(const UInt32 &followId, const UInt8 &followOrder);
	virtual void OnUpdateAppeared(const Boolean &appeared);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		stateMask	= Parent::NextFreeMask + 1,
		mapMask	= Parent::NextFreeMask + 2,
		rebornMask	= Parent::NextFreeMask + 3,
		curPosMask	= Parent::NextFreeMask + 4,
		dstPosMask	= Parent::NextFreeMask + 5,
		actionMask	= Parent::NextFreeMask + 6,
		speedMask	= Parent::NextFreeMask + 7,
		followIdMask	= Parent::NextFreeMask + 8,
		appearedMask	= Parent::NextFreeMask + 9,
		NextFreeMask	= Parent::NextFreeMask + 10
	};
	const String& GetNickName() const { return m.nickName; }
	const UInt8& GetSex() const { return m.sex; }
	const UInt8& GetCclass() const { return m.cclass; }
	const UInt8& GetSexClass() const { return m.sexClass; }
	const UInt8& GetFaction() const { return m.faction; }
	const UInt8& GetState() const { return m.state; }
	const UInt16& GetMap_id() const { return m.map_id; }
	const UInt16& GetMap_x() const { return m.map_x; }
	const UInt16& GetMap_y() const { return m.map_y; }
	const UInt8& GetDirection() const { return m.direction; }
	const UInt32& GetMap_ownerId() const { return m.map_ownerId; }
	const UInt16& GetReborn_map() const { return m.reborn_map; }
	const UInt16& GetReborn_x() const { return m.reborn_x; }
	const UInt16& GetReborn_y() const { return m.reborn_y; }
	const UInt16& GetCurPosX() const { return m.curPosX; }
	const UInt16& GetCurPosY() const { return m.curPosY; }
	const UInt16& GetDstPosX() const { return m.dstPosX; }
	const UInt16& GetDstPosY() const { return m.dstPosY; }
	const UInt16& GetAction() const { return m.action; }
	const UInt32& GetActionTime() const { return m.actionTime; }
	const UInt32& GetSpeed() const { return m.speed; }
	const UInt32& GetFollowId() const { return m.followId; }
	const UInt8& GetFollowOrder() const { return m.followOrder; }
	const Boolean& GetAppeared() const { return m.appeared; }
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction) {};
	virtual void OnUpdateState(const UInt8 &state) {};
	virtual void OnUpdateMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt8 &direction, const UInt32 &map_ownerId) {};
	virtual void OnUpdateReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y) {};
	virtual void OnUpdateCurPos(const UInt16 &curPosX, const UInt16 &curPosY) {};
	virtual void OnUpdateDstPos(const UInt16 &dstPosX, const UInt16 &dstPosY) {};
	virtual void OnUpdateAction(const UInt16 &action, const UInt32 &actionTime) {};
	virtual void OnUpdateSpeed(const UInt32 &speed) {};
	virtual void OnUpdateFollowId(const UInt32 &followId, const UInt8 &followOrder) {};
	virtual void OnUpdateAppeared(const Boolean &appeared) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//info
	String nickName;
	UInt8 sex;
	UInt8 cclass;
	UInt8 sexClass;
	UInt8 faction;
	//state
	UInt8 state;
	//map
	UInt16 map_id;
	UInt16 map_x;
	UInt16 map_y;
	UInt8 direction;
	UInt32 map_ownerId;
	//reborn
	UInt16 reborn_map;
	UInt16 reborn_x;
	UInt16 reborn_y;
	//curPos
	UInt16 curPosX;
	UInt16 curPosY;
	//dstPos
	UInt16 dstPosX;
	UInt16 dstPosY;
	//action
	UInt16 action;
	UInt32 actionTime;
	//speed
	UInt32 speed;
	//followId
	UInt32 followId;
	UInt8 followOrder;
	//appeared
	Boolean appeared;
} m;

public:
	INLINE MapActor_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult MapActor_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapActor_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1964846251>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1882347406)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1964846251)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_MAPACTOR_PROXY_H_
