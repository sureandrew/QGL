///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_HAWKGROUP_STUB_H_
#define _RPC_HAWKGROUP_STUB_H_

#include "Common/RPCFunc.h"
#include "Reuben/Network/NetGroup.h"
#include "RPCEnum/HawkGroup_enum.h"

class HawkGroup_Stub : public NetGroup
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult BuyHawkItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemStack);
	virtual RPCResult ChangeHawkName(LPCPEER pPeer, RPCContext &context, const String &hawkName);
	virtual RPCResult CloseHawkForUser(LPCPEER pPeer, RPCContext &context, const UInt32 &charID);
	virtual RPCResult StopHawk(LPCPEER pPeer, RPCContext &context);
***/

protected:
	typedef NetGroup Parent;

public:
	enum UpdateMask {
		ownerInfoMask	= Parent::NextFreeMask + 0,
		hawkNameInfoMask	= Parent::NextFreeMask + 1,
		NextFreeMask	= Parent::NextFreeMask + 2
	};
	virtual RPCResult BuyHawkItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemStack) = 0;
	virtual RPCResult CB_UpdateHawkItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemID, const UInt8 &itemType);
	virtual RPCResult CB_BuyHawkItemSuccess(LPCPEER pPeer, RPCContext &context, const Boolean &owner, const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemStack, const UInt32 &tax);
	virtual RPCResult ChangeHawkName(LPCPEER pPeer, RPCContext &context, const String &hawkName) = 0;
	virtual RPCResult CB_ChangeHawkName(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CloseHawkForUser(LPCPEER pPeer, RPCContext &context, const UInt32 &charID) = 0;
	virtual RPCResult CB_CloseHawkForUser(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult StopHawk(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_StopHawk(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AddSellStartHawkItemInfo(LPCPEER pPeer, RPCContext &context, NetGroup* addHawkItem);
	const UInt32& GetOwnerID() const { return m.ownerID; }
	const String& GetHawkName() const { return m.hawkName; }
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
	INLINE virtual VOID ClearAllMask() {
		Parent::ClearAllMask();
		m_modifyMask.Clear();
	}
	INLINE virtual VOID MarkUpdateAll(UInt bit, bool modified) {
		Parent::MarkUpdateAll(bit, modified);
		if (modified) m_modifyMask.MaskBit(bit);
	}
	void SetOwnerInfo(const UInt32 &ownerID);
	void SetOwnerID(const UInt32 &ownerID);
	void SetHawkNameInfo(const String &hawkName);
	void SetHawkName(const String &hawkName);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//ownerInfo
	UInt32 ownerID;
	//hawkNameInfo
	String hawkName;
} m;

public:
	INLINE HawkGroup_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_HAWKGROUP_STUB_H_
