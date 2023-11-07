#pragma once
#ifndef _SELLYUANBAOGROUP_H_
#define _SELLYUANBAOGROUP_H_

#include "Proxy/Logic/SellYuanBaoGroup_Proxy.h"
class CSellYuanBao;

class CSellYuanBaoGroup : public SellYuanBaoGroup_Proxy
{
public:
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(SellYuanBaoGroup_CLASSID);

	CSellYuanBaoGroup();
	virtual ~CSellYuanBaoGroup();

	void InitSellRecord(NetGroup* record);
	void ReOrderRecordList();
	static bool CostHigher(CSellYuanBao* yb1, CSellYuanBao* yb2);
	virtual RPCResult CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* sellYuanBaoGrp, const UInt8 &msgIndex);
	virtual RPCResult CB_GetAllYBTAndGMoney(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt32 &yuanbaoTicket, const UInt32 &gmoney);
	virtual RPCResult CB_BuyYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &msgIndex, const UInt32 &num);
	virtual RPCResult CB_RemoveSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID);
	virtual RPCResult CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &money);

	UInt GetRecordSize() { return (UInt)m_recordList.size();}
	CSellYuanBao* GetSellYBRecord(UInt index);
	//bool IsRecordExist(UInt recordID);
	CSellYuanBao* GetSellYBRecordByID(UInt recordID);

private:
	typedef SellYuanBaoGroup_Proxy Parent;
	typedef StlVector<CSellYuanBao*, ManagedAllocator<CSellYuanBao*> > YuanBaoRecordList;

	YuanBaoRecordList m_recordList;

};

#endif //_SELLYUANBAOGROUP_H_
