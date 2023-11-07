///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MAPACTOR_STUB_H_
#define _RPC_MAPACTOR_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/MapActor_enum.h"

class MapActor_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	RPCMask m_modifyMask;
	INLINE Boolean IsModified() { return (!m_modifyMask.IsZero()); }
	INLINE Boolean IsModified(UInt bit) { return (m_modifyMask.IsBit(bit)); }
	INLINE Boolean IsModified(CONST RPCMask& mask) { return (m_modifyMask.IsMask(mask)); }
	INLINE VOID SetModifyMask(CONST RPCMask& mask) { m_modifyMask = mask; }
	INLINE CONST RPCMask& GetModifyMask() CONST { return m_modifyMask; }
	INLINE VOID ClearModifyMask() { m_modifyMask.Clear(); }
	INLINE VOID MarkModify(UInt bit) { m_modifyMask.MaskBit(bit); }
	INLINE VOID MarkModify(CONST RPCMask& mask) { m_modifyMask.Mask(mask); }
	INLINE VOID UnmarkModify(UInt bit) { m_modifyMask.UnmaskBit(bit); }
	INLINE VOID UnmarkModify(CONST RPCMask& mask) { m_modifyMask.Unmask(mask); }
	RPCMask m_viewMask;
	INLINE VOID SetViewMask(CONST RPCMask& mask) { m_viewMask = mask; }
	INLINE CONST RPCMask& GetViewMask() CONST { return m_viewMask; }
	INLINE VOID ClearViewMask() { m_viewMask.Clear(); }
	INLINE VOID MarkView(UInt bit) { m_viewMask.MaskBit(bit); }
	INLINE VOID MarkView(CONST RPCMask& mask) { m_viewMask.Mask(mask); }
	INLINE VOID UnmarkView(UInt bit) { m_viewMask.UnmaskBit(bit); }
	INLINE VOID UnmarkView(CONST RPCMask& mask) { m_viewMask.Unmask(mask); }
	INLINE virtual VOID ClearAllMask() {
		Parent::ClearAllMask();
		m_modifyMask.Clear();
		m_viewMask.Clear();
	}
	INLINE virtual VOID MarkUpdateAll(UInt bit, bool modified) {
		Parent::MarkUpdateAll(bit, modified);
		if (modified) m_modifyMask.MaskBit(bit);
		if (modified) m_viewMask.MaskBit(bit);
	}
	void SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction);
	void SetNickName(const String &nickName);
	void SetSex(const UInt8 &sex);
	void SetCclass(const UInt8 &cclass);
	void SetSexClass(const UInt8 &sexClass);
	void SetFaction(const UInt8 &faction);
	void SetState(const UInt8 &state);
	void SetMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt8 &direction, const UInt32 &map_ownerId);
	void SetMap_id(const UInt16 &map_id);
	void SetMap_x(const UInt16 &map_x);
	void SetMap_y(const UInt16 &map_y);
	void SetDirection(const UInt8 &direction);
	void SetMap_ownerId(const UInt32 &map_ownerId);
	void SetReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y);
	void SetReborn_map(const UInt16 &reborn_map);
	void SetReborn_x(const UInt16 &reborn_x);
	void SetReborn_y(const UInt16 &reborn_y);
	void SetCurPos(const UInt16 &curPosX, const UInt16 &curPosY);
	void SetCurPosX(const UInt16 &curPosX);
	void SetCurPosY(const UInt16 &curPosY);
	void SetDstPos(const UInt16 &dstPosX, const UInt16 &dstPosY);
	void SetDstPosX(const UInt16 &dstPosX);
	void SetDstPosY(const UInt16 &dstPosY);
	void SetAction(const UInt16 &action, const UInt32 &actionTime);
	void SetAction(const UInt16 &action);
	void SetActionTime(const UInt32 &actionTime);
	void SetSpeed(const UInt32 &speed);
	void SetFollowId(const UInt32 &followId, const UInt8 &followOrder);
	void SetFollowId(const UInt32 &followId);
	void SetFollowOrder(const UInt8 &followOrder);
	void SetAppeared(const Boolean &appeared);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE MapActor_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_MAPACTOR_STUB_H_
