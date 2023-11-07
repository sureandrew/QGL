///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_SELLYUANBAOGROUP_STUB_H_
#define _RPC_SELLYUANBAOGROUP_STUB_H_

#include "Common/RPCFunc.h"
#include "Reuben/Network/NetGroup.h"
#include "RPCEnum/SellYuanBaoGroup_enum.h"

class SellYuanBaoGroup_Stub : public NetGroup
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult GetTempBank(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CloseSellYuanBao(LPCPEER pPeer, RPCContext &context);
***/

protected:
	typedef NetGroup Parent;

public:
	virtual RPCResult GetTempBank(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CloseSellYuanBao(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* sellYuanBaoGrp, const UInt8 &msgIndex);
	virtual RPCResult CB_GetAllYBTAndGMoney(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt32 &yuanbaoTicket, const UInt32 &gmoney);
	virtual RPCResult CB_BuyYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &msgIndex, const UInt32 &num);
	virtual RPCResult CB_RemoveSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID);
	virtual RPCResult CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &money);
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
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
} m;

public:
	INLINE SellYuanBaoGroup_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_SELLYUANBAOGROUP_STUB_H_
