//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Logic/SellYuanBaoGroup.h"
#include "Stub/Logic/SellYuanBao.h"
#include "Proxy/Data/SellYuanBaoData.h"

#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/LogService.h"
#include "WorldService/Global.h"
#include "Stub/Logic/Account.h"
#include "Stub/Logic/CharacterInfo.h"

REGISTER_CLASSTYPE(CSellYuanBaoGroup, SellYuanBaoGroup_CLASSID);

#define MAX_SELL_YUANBAO_RECORD_NUM 5

CSellYuanBaoGroup::CSellYuanBaoGroup()
{
	m_availSellYBRecord.clear();
	m_expireSellYBRecord.clear();
	m_charIDList.clear();
	m_charRecordCount.clear();
}

CSellYuanBaoGroup::~CSellYuanBaoGroup()
{
	for (SellYuanBaoMap::iterator itr = m_availSellYBRecord.begin(); itr != m_availSellYBRecord.end(); ++itr)
	{
		CSellYuanBao* temp = FindNetObject<CSellYuanBao>(itr->second);
		if (temp)
			DeleteNetObject(temp);
	}
	m_availSellYBRecord.clear();

	for (CharYuanBaoMap::iterator itr = m_expireSellYBRecord.begin(); itr != m_expireSellYBRecord.end(); ++itr)
	{
		for (SellYuanBaoMap::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
		{
			CSellYuanBao* temp = FindNetObject<CSellYuanBao>(itr2->second);
			if (temp)
				DeleteNetObject(temp);
		}
		itr->second.clear();
	}
	m_expireSellYBRecord.clear();

	m_charIDList.clear();
	m_charRecordCount.clear();
}

void CSellYuanBaoGroup::InitSellYuanBao()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CSellYuanBaoGroup, InitSellYuanBao, this, context)
}

RPCResult CSellYuanBaoGroup::InitSellYuanBao(RPCContext &context)
{
	PROCEDURE_START_METHOD(CSellYuanBaoGroup, InitSellYuanBao, this, context)
	
		TRACE_INFODTL(GLOBAL_LOGGER, _F("Start init"));

		ObjectService_Proxy::GetAllSellYuanBao(gGlobal.pObjectService->pPeer, context);

	PROCEDURE_WAIT2(1, ObjectService, CB_GetSellYuanBao, NetGroup*, sellYuanBaoDataGroup, UInt8, msgIndex)

		time_t now =  ::time(NULL);
		CSellYuanBaoData* pData = NULL;		
		if (sellYuanBaoDataGroup)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("All record count %d"), sellYuanBaoDataGroup->GetCount());
			for (Index i = C_INDEX(0); i < sellYuanBaoDataGroup->GetCount(); ++i)
			{
				pData = sellYuanBaoDataGroup->GetChild<CSellYuanBaoData>(i);
				CSellYuanBao* pSellYuanBao = CSellYuanBao::NewInstance();
				if (pData && pSellYuanBao)
				{
					pSellYuanBao->SetData(pData);
					if (pSellYuanBao->GetExpireDate() > (UInt32)now && pSellYuanBao->GetTotalYBT() > pSellYuanBao->GetSoldYBT())
						m_availSellYBRecord.insert(std::make_pair(pSellYuanBao->GetSellYuanBao_id(), pSellYuanBao->GetNetId()));
					else
					{
						CharYuanBaoMap::iterator itr = m_expireSellYBRecord.find(pSellYuanBao->GetChar_id());
						if (itr != m_expireSellYBRecord.end())
							itr->second.insert(std::make_pair(pSellYuanBao->GetSellYuanBao_id(), pSellYuanBao->GetNetId()));
						else
						{
							SellYuanBaoMap tempMap;
							tempMap.insert(std::make_pair(pSellYuanBao->GetSellYuanBao_id(), pSellYuanBao->GetNetId()));
							m_expireSellYBRecord.insert(std::make_pair(pSellYuanBao->GetChar_id(), tempMap));
						}
					}
					pSellYuanBao->SetOwnerGroup(this);
					CharRecordCountMap::iterator itr = m_charRecordCount.find(pSellYuanBao->GetChar_id());
					if (itr != m_charRecordCount.end())
						itr->second = itr->second + 1;
					else
						m_charRecordCount.insert(std::make_pair(pSellYuanBao->GetChar_id(), 1));
				}
			}
		}

		DeleteNetGroup(sellYuanBaoDataGroup, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CSellYuanBaoGroup::MoveToExpireList(UInt32 sellYuanBao_id)
{	
	SellYuanBaoMap::iterator itr = m_availSellYBRecord.find(sellYuanBao_id);
	if (itr != m_availSellYBRecord.end())
	{
		CSellYuanBao* temp = FindNetObject<CSellYuanBao>(itr->second);
		if (temp)
		{
			CharYuanBaoMap::iterator itr2 = m_expireSellYBRecord.find(temp->GetChar_id());
			if (itr2 != m_expireSellYBRecord.end())
				itr2->second.insert(std::make_pair(temp->GetSellYuanBao_id(), temp->GetNetId()));
			else
			{
				SellYuanBaoMap tempMap;
				tempMap.insert(std::make_pair(temp->GetSellYuanBao_id(), temp->GetNetId()));
				m_expireSellYBRecord.insert(std::make_pair(temp->GetChar_id(), tempMap));
			}
		}
		m_availSellYBRecord.erase(itr);

		//tell client to erase it
		for (CharContextList::iterator itr = m_charIDList.begin(); itr != m_charIDList.end(); ++itr)
		{
			Session* psess = GetSession(itr->second.sessionId);
			if (psess)
				CB_RemoveSellYuanBao(psess->GetPeer(), psess->GetContext(), sellYuanBao_id);
		}		
	}
}

void CSellYuanBaoGroup::AddSellYuanBao(NetGroup* sellYBGrp)
{
	CSellYuanBaoData* pData = NULL;
	NetGroup sendGrp;
	for (Index i = C_INDEX(0); i < sellYBGrp->GetCount(); ++i)
	{
		pData = sellYBGrp->GetChild<CSellYuanBaoData>(i);
		CSellYuanBao* pSellYuanBao = CSellYuanBao::NewInstance();
		if (pData && pSellYuanBao)
		{
			pSellYuanBao->SetData(pData);
			pSellYuanBao->PreSend();
			pSellYuanBao->SetOwnerGroup(this);
			m_availSellYBRecord.insert(std::make_pair(pSellYuanBao->GetSellYuanBao_id(), pSellYuanBao->GetNetId()));

			CharRecordCountMap::iterator itr = m_charRecordCount.find(pSellYuanBao->GetChar_id());
			if (itr != m_charRecordCount.end())
				itr->second = itr->second + 1;
			else
				m_charRecordCount.insert(std::make_pair(pSellYuanBao->GetChar_id(), 1));

			sendGrp.Add(pSellYuanBao);
		}
	}
	
	if (sendGrp.GetCount() > 0)
	{
		for (CharContextList::iterator itr = m_charIDList.begin(); itr != m_charIDList.end(); ++itr)
		{
			Session* psess = GetSession(itr->second.sessionId);
			if (psess)
				CB_AddSellYuanBao(psess->GetPeer(), psess->GetContext(), &sendGrp, 0);
		}
	}
}

bool CSellYuanBaoGroup::CanAddSellYuanBao(UInt32 char_id)
{
	CharRecordCountMap::iterator itr = m_charRecordCount.find(char_id);
	if (itr != m_charRecordCount.end() && itr->second >= MAX_SELL_YUANBAO_RECORD_NUM)
		return false;
	return true;
}

/*RPCResult CSellYuanBaoGroup::GetAllYBTAndGMoney(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}*/

/*RPCResult CSellYuanBaoGroup::BuyYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBao_id, const UInt32 &num)
{
	return RPC_RESULT_OK;
}*/

void CSellYuanBaoGroup::InsertCharContext(UInt32 char_id, RPCContext& context)
{
	CharContextList::iterator itr = m_charIDList.find(char_id);
	if (itr != m_charIDList.end())
		itr->second = context;
	else
		m_charIDList.insert(std::make_pair(char_id, context));
}

void CSellYuanBaoGroup::GetSendGroup(NetGroup* grp)
{
	if (grp)
	{
		CSellYuanBao* pSellYB = NULL;
		for (SellYuanBaoMap::iterator itr = m_availSellYBRecord.begin(); itr != m_availSellYBRecord.end(); ++itr)
		{
			pSellYB = FindNetObject<CSellYuanBao>(itr->second);
			if (pSellYB)
			{
				pSellYB->PreSend();
				grp->Add(pSellYB);
			}
		}
	}
}

RPCResult CSellYuanBaoGroup::CloseSellYuanBao(LPCPEER pPeer, RPCContext &context)
{
	Session* psess = GetSession(context.sessionId);
	CAccount* pAccount = psess ? psess->pAccount : NULL;
	CCharacterInfo* pCharInfo = pAccount ? pAccount->GetLoginChar() : NULL;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("char_id %d, sessionId %s"), 
		pCharInfo ? pCharInfo->GetChar_id() : 0, context.sessionId.GetString().c_str());
	if (pCharInfo)
	{
		CharContextList::iterator itr = m_charIDList.find(pCharInfo->GetChar_id());
		if (itr != m_charIDList.end())
			m_charIDList.erase(itr);
	}
	UnsetOwnObject(psess->GetSessionId(), this);
	return RPC_RESULT_OK;
}

CSellYuanBao* CSellYuanBaoGroup::GetSellYuanBaoByID(UInt32 sellYuanBaoID)
{
	SellYuanBaoMap::iterator itr = m_availSellYBRecord.find(sellYuanBaoID);
	if (itr != m_availSellYBRecord.end())
		return FindNetObject<CSellYuanBao>(itr->second);

	return NULL;
}

void CSellYuanBaoGroup::UpdateSellYBToClient(UInt32 sellYuanBaoID)
{
	CSellYuanBao* pData = GetSellYuanBaoByID(sellYuanBaoID);
	if (pData)
	{
		for (CharContextList::iterator itr = m_charIDList.begin(); itr != m_charIDList.end(); ++itr)
		{
			Session* psess = GetSession(itr->second.sessionId);
			if (psess)				
				pData->SendAttr(psess->GetPeer(), psess->GetContext(), pData->GetUpdateMask());
		}
	}
}

RPCResult CSellYuanBaoGroup::GetTempBank(LPCPEER pPeer, RPCContext &context)
{
	Session* psess = GetSession(context.sessionId);
	CAccount* pAccount = psess ? psess->pAccount : NULL;
	CCharacterInfo* pCharInfo = pAccount ? pAccount->GetLoginChar() : NULL;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("char_id %d, sessionId %s"), 
		pCharInfo ? pCharInfo->GetChar_id() : 0, context.sessionId.GetString().c_str());

	UInt32 totalMoney = 0;
	UInt32 totalYBT = 0;
	if (pCharInfo)
	{
		CharYuanBaoMap::iterator itr = m_expireSellYBRecord.find(pCharInfo->GetChar_id());
		if (itr != m_expireSellYBRecord.end())
		{
			for (SellYuanBaoMap::iterator itr1 = itr->second.begin(); itr1 != itr->second.end(); ++itr1)
			{
				CSellYuanBao* temp = FindNetObject<CSellYuanBao>(itr1->second);
				if (temp)
				{
					totalMoney += temp->GetMoneyPerYBT() * temp->GetSoldYBT();
					totalYBT += temp->GetTotalYBT() - temp->GetSoldYBT();
				}
			}
		}
	}

	CB_GetTempBank(pPeer, context, totalYBT, totalMoney);

	return RPC_RESULT_OK;
}

void CSellYuanBaoGroup::RemoveExpireList(UInt32 char_id, UInt32Vector &removeIDsList)
{
	UInt count = 0;
	CharYuanBaoMap::iterator itr = m_expireSellYBRecord.find(char_id);	
	if (itr != m_expireSellYBRecord.end())
	{
		for (Index i = C_INDEX(0); i < removeIDsList.size(); ++i)
		{
			SellYuanBaoMap::iterator itr1 = itr->second.find(removeIDsList[i]);
			if (itr1 != itr->second.end())
			{
				CSellYuanBao* temp = FindNetObject<CSellYuanBao>(itr1->second);
				if (temp)
					DeleteNetObject(temp);
				itr->second.erase(itr1);
				++count;
			}
		}
	}

	CharRecordCountMap::iterator itr2 = m_charRecordCount.find(char_id);
	if (itr2 != m_charRecordCount.end())
	{
		if (itr2->second >= count)
			itr2->second = itr2->second - count;
		else
			itr2->second = 0;
	}
}