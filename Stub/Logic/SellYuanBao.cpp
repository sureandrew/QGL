//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Logic/SellYuanBao.h"
#include "Proxy/Data/SellYuanBaoData.h"
#include "Stub/Logic/SellYuanBaoGroup.h"

REGISTER_CLASSTYPE(CSellYuanBao, SellYuanBao_CLASSID);

CSellYuanBao::CSellYuanBao()
{
	m_pOwnerGroup = NULL;
	m_iReserve = 0;
}

CSellYuanBao::~CSellYuanBao()
{

}

void CSellYuanBao::PreSend()
{
	SetSendMask(CSellYuanBao::AllMask);
}

void CSellYuanBao::SetData(CSellYuanBaoData* pSellYuanBaoData)
{
	if (pSellYuanBaoData == NULL)
		return;

	if (pSellYuanBaoData->IsUpdated(CSellYuanBaoData::idMask))
	{
		SetId(pSellYuanBaoData->GetSellYuanBao_id());
	}

	if (pSellYuanBaoData->IsUpdated(CSellYuanBaoData::charInfoMask))
	{
		SetCharInfo(pSellYuanBaoData->GetChar_id(), pSellYuanBaoData->GetCharName());
	}

	if (pSellYuanBaoData->IsUpdated(CSellYuanBaoData::sellInfoMask))
	{
		SetSellInfo(pSellYuanBaoData->GetSellType(), pSellYuanBaoData->GetTotalYBT(), 
			pSellYuanBaoData->GetMoneyPerYBT(), pSellYuanBaoData->GetExpireDate());

		UInt32 now = (UInt32) time(NULL);
		if (GetExpireDate() > (UInt32)now)
			RaiseUniqEventID(EVT_UPDATE_EXPIRE, (GetExpireDate() - (UInt32)now) * 1000);
		else
			RaiseUniqEventID(EVT_UPDATE_EXPIRE);
	}

	if (pSellYuanBaoData->IsUpdated(CSellYuanBaoData::soldYBTMask))
	{
		SetSoldYBT(pSellYuanBaoData->GetSoldYBT());
	}
}

void CSellYuanBao::HandleEvent(CONST Event &event)
{
	switch(event.GetID())
	{
	case EVT_UPDATE_EXPIRE:
		{
			if (m_pOwnerGroup)
				m_pOwnerGroup->MoveToExpireList(GetSellYuanBao_id());
		}
		break;
	}
}

UInt CSellYuanBao::GetRemain()
{
	if(m_iReserve > GetTotalYBT() - GetSoldYBT())
		return 0;

	return GetTotalYBT() - GetSoldYBT() - m_iReserve;
}

void CSellYuanBao::Reserve(Int reserve)
{
	if (reserve > 0 || m_iReserve + reserve > 0)
		m_iReserve += reserve;
	else
		m_iReserve = 0;
}

void CSellYuanBao::AddSold(UInt solded, bool reduceReserve)
{
	SetSoldYBT(GetSoldYBT() + solded);
	if (reduceReserve)
		Reserve(-(Int)solded);

	if (m_pOwnerGroup)
	{
		if (GetSoldYBT() >= GetTotalYBT() )
			m_pOwnerGroup->MoveToExpireList(GetSellYuanBao_id());
		else
			m_pOwnerGroup->UpdateSellYBToClient(GetSellYuanBao_id());
	}
}