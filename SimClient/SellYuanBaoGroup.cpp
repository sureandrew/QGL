//-- Common
#include "Common.h"
//-- Self
#include "SimClient/SellYuanBaoGroup.h"
#include "SimClient/SellYuanBao.h"

REGISTER_CLASSTYPE(CSellYuanBaoGroup, SellYuanBaoGroup_CLASSID);

RPCResult CSellYuanBaoGroup::CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* sellYuanBaoGrp, const UInt8 &msgIndex)
{
	if (sellYuanBaoGrp)
	{
		CSellYuanBao* pSellYB = NULL;
		for (Index i = C_INDEX(0); i < sellYuanBaoGrp->GetCount(); ++i)
		{
			pSellYB = sellYuanBaoGrp->GetChild<CSellYuanBao>(i);
			if (pSellYB)
				m_recordList.push_back(pSellYB);
		}
		ReOrderRecordList();
	}
	
	return RPC_RESULT_OK;
}

RPCResult CSellYuanBaoGroup::CB_GetAllYBTAndGMoney(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt32 &yuanbaoTicket, const UInt32 &gmoney)
{
	return RPC_RESULT_OK;
}

RPCResult CSellYuanBaoGroup::CB_BuyYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &msgIndex, const UInt32 &num)
{
	return RPC_RESULT_OK;
}

CSellYuanBaoGroup::CSellYuanBaoGroup()
{
	m_recordList.clear();
}


CSellYuanBaoGroup::~CSellYuanBaoGroup()
{
	CSellYuanBao* temp = NULL;
	for (Index i = C_INDEX(0); i < m_recordList.size(); ++i)
	{	
		temp = m_recordList[i];
		DeleteNetObject(temp);
	}

	m_recordList.clear();
}

void CSellYuanBaoGroup::InitSellRecord(NetGroup* record)
{
	if (record)
	{
		CSellYuanBao* pSellYB = NULL;
		for (Index i = C_INDEX(0); i < record->GetCount(); ++i)
		{
			pSellYB = record->GetChild<CSellYuanBao>(i);
			if (pSellYB)
				m_recordList.push_back(pSellYB);
		}
		ReOrderRecordList();
	}
}

void CSellYuanBaoGroup::ReOrderRecordList()
{
	std::sort(m_recordList.begin(), m_recordList.end(), CostHigher);
}

bool CSellYuanBaoGroup::CostHigher(CSellYuanBao* yb1, CSellYuanBao* yb2)
{
	if (yb1 && yb2)
		return yb1->GetMoneyPerYBT() < yb2->GetMoneyPerYBT();
	return false;
}

CSellYuanBao* CSellYuanBaoGroup::GetSellYBRecord(UInt index)
{
	if (m_recordList.size() <= index)
		return NULL;

	return m_recordList[index];
}

CSellYuanBao* CSellYuanBaoGroup::GetSellYBRecordByID(UInt recordID)
{
	for (Index i = C_INDEX(0); i < m_recordList.size(); ++i)
	{
		if (recordID == m_recordList[i]->GetSellYuanBao_id())
			return m_recordList[i];
	}
	return NULL;
}

RPCResult CSellYuanBaoGroup::CB_RemoveSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID)
{
	for (Index i = C_INDEX(0); i < m_recordList.size(); ++i)
	{
		if (sellYuanBaoID == m_recordList[i]->GetSellYuanBao_id())
		{
			CSellYuanBao* pData = m_recordList[i];
			m_recordList.erase(std::remove(m_recordList.begin(), m_recordList.end(), pData));
			
			DeleteNetObject(pData);
			break;
		}
	}	
	return RPC_RESULT_OK;
}

RPCResult CSellYuanBaoGroup::CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &money)
{
	

	return RPC_RESULT_OK;
}