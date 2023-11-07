#include "Common.h"
#include "Common/Procedure.h"
#include "Proxy/Service/ObjectService.h"
#include "MapService/Global.h"
#include "Stub/Logic/GenerationGroup.h"
#include "Stub/Logic/Generation.h"
#include "Stub/Data/GenerationData.h"
#include "Proxy/Service/WorldService.h"

CGenerationGroup::CGenerationGroup()
{
}
CGenerationGroup::~CGenerationGroup()
{
	GenerationMap::iterator it = m_mapGeneration.begin();
	for (;it != m_mapGeneration.end();it++) {
		CGeneration* pGnt = it->second;
		if (NULL != pGnt) {
			DeleteNetObject(pGnt);
		}
	}
	m_mapGeneration.clear();
}
RPCResult CGenerationGroup::UpdateGenerationGroup(RPCContext &context)
{
	PROCEDURE_START_METHOD(CGenerationGroup, UpdateGenerationGroup, this, context)

		if (m_pOwner == NULL)
		{
			TRACE_ERRORDT(GLOBAL_LOGGER, _F("UpdateGenerationGroup Error: no owner belong to"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		UInt32 char_id = 0;
		if (m_pOwner)
			char_id = m_pOwner->GetChar_id();

		NetGroup gntGrp;
		for (GenerationMap::iterator it = m_mapGeneration.begin();
			it != m_mapGeneration.end(); it++)
		{
			CGeneration *pGnt = it->second;
			if (pGnt == NULL)
				continue;
			if (pGnt->IsModified())
			{
				CGenerationData* gntData = CreateNetObject<CGenerationData>();
				gntData->SetBYoung(pGnt->GetBYoung());
				gntData->SetStrNickName(pGnt->GetStrNickName());
				gntData->SetUiAccmulOrRecvExp(pGnt->GetUiAccmulOrRecvExp());
				gntData->SetUiGiftPointOrHistAccm(pGnt->GetUiGiftPointOrHistAccm());
				if (pGnt->GetBYoung())
				{
					gntData->SetUiElderId(pGnt->GetUiRelatedId());
					gntData->SetUiYoungerId(m_pOwner->GetChar_id());
				}
				else
				{
					gntData->SetUiYoungerId(pGnt->GetUiRelatedId());
					gntData->SetUiElderId(m_pOwner->GetChar_id());
				}
				gntData->SetSendMask(CGenerationData::AllMask);
				gntGrp.Add(gntData);
				pGnt->SetModifyState(FALSE);
			}
			if (0 == pGnt->GetUiOfflineTime()) {
				CCharacter* pTarget = gGlobal.m_charToSession.FindCharacter(pGnt->GetUiRelatedId());
				if (NULL != pTarget)
					pTarget->GenerationOffline(m_pOwner->GetChar_id());
				else
					WorldService_Proxy::GenerationOfflineNotifyTrans(gGlobal.pWorldService->pPeer,
						context,pGnt->GetUiRelatedId(),m_pOwner->GetChar_id());
			}
		}

		if (gntGrp.GetCount() > 0)
		{
			ObjectService_Proxy::UpdateGeneration(gGlobal.pObjectService->pPeer, context, &gntGrp);
			DeleteNetChilds(&gntGrp);
		}
		else
		{
			PROCEDURE_RETURN
		}

		PROCEDURE_WAIT(1, ObjectService, CB_UpdateGeneration)

			PROCEDURE_CATCH

			PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
			_T("CSkill UpdateToObject Error: ObjectService:CB_Error, errcode %d, %s"),
			err, errMsg.c_str());

		PROCEDURE_END_CATCH


	return RPC_RESULT_OK;
}