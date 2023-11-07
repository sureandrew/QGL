#pragma once
#ifndef _SELLYUANBAOGROUP_H_
#define _SELLYUANBAOGROUP_H_

#include "Stub/Logic/SellYuanBaoGroup_Stub.h"

class CSellYuanBao;

class CSellYuanBaoGroup : public SellYuanBaoGroup_Stub
{
public:

	DEFINE_CLASS(SellYuanBaoGroup_CLASSID);

	INLINE static CSellYuanBaoGroup* NewInstance();

	CSellYuanBaoGroup();
	virtual ~CSellYuanBaoGroup();

	void InitSellYuanBao();
	RPCResult InitSellYuanBao(RPCContext &context);

	void MoveToExpireList(UInt32 sellYuanBao_id);

	void AddSellYuanBao(NetGroup* sellYBDataGrp);
	bool CanAddSellYuanBao(UInt32 char_id);

	void InsertCharContext(UInt32 char_id, RPCContext& context);
	void GetSendGroup(NetGroup* grp);

	CSellYuanBao* GetSellYuanBaoByID(UInt32 sellYuanBaoID);
	void UpdateSellYBToClient(UInt32 sellYuanBaoID);
	void RemoveExpireList(UInt32 char_id, UInt32Vector &removeIDsList);

	virtual RPCResult GetTempBank(LPCPEER pPeer, RPCContext &context);
	//virtual RPCResult GetAllYBTAndGMoney(LPCPEER pPeer, RPCContext &context);
	//virtual RPCResult BuyYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBao_id, const UInt32 &num);
	virtual RPCResult CloseSellYuanBao(LPCPEER pPeer, RPCContext &context);

private:
	typedef SellYuanBaoGroup_Stub Parent;
	typedef StlMap<UInt32, RPCNetID, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, RPCNetID> > > SellYuanBaoMap;
	typedef StlMap<UInt32, SellYuanBaoMap, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, SellYuanBaoMap> > > CharYuanBaoMap;
	typedef StlMap<UInt32, RPCContext, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, RPCContext> > > CharContextList;
	typedef StlMap<UInt32, UInt8, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, UInt8> > > CharRecordCountMap;

	SellYuanBaoMap m_availSellYBRecord;
	CharYuanBaoMap m_expireSellYBRecord;

	CharContextList m_charIDList;
	CharRecordCountMap m_charRecordCount;

};

INLINE CSellYuanBaoGroup* CSellYuanBaoGroup::NewInstance()
{
	return CreateNetObject<CSellYuanBaoGroup>();
}

#endif //_SELLYUANBAOGROUP_H_
