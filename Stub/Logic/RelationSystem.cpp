//-- Common
#include "Common.h"
#include "Common/procedure.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Proxy/Service/ObjectService.h"
#include "Stub/Logic/RelationSystem.h"
#include "Stub/Logic/Character.h"
#include "Proxy/Service/MessageService_proxy.h"
#include "MapService/Global.h"
#include "MapService/Configure.h"
#include "Proxy/Data/SiblingData.h"
#include "Proxy/Data/MarriageData.h"
#include "Resource/GameStringTable.h"
#include "Stub/Logic/MapNPC.h"

#define MARRIAGE_WAIT_INTERVAL 10 * 1000
#define MARRIAGE_BOW_INTERVAL 10 * 1000
#define MARRIAGE_BOW_STOP 8 * 1000
#define MARRIAGE_INTERVAL 20 //minutes
REGISTER_CLASSTYPE(CRelationSystem, RelationSystem_CLASSID);
PROTO_LuaPush(CCharacter)

CRelationSystem::CRelationSystem()
{
	m_pOwner = NULL;
	m_state = ST_NONE;
	m_pSiblingData = NULL;
	m_pMarriageData = NULL;
	m_tmpHusbandId = 0;
	m_tmpWifeId = 0;
	m_bWedding = BOOLEAN_FALSE;
	m_siblingDataNetID = RPCNetID();
}
UInt32 CRelationSystem::m_allMarriageCount = 0;
CRelationSystem::~CRelationSystem()
{
	if (hasSiblingData() && m_pSiblingData)
	{
		bool bDelete = true;
		do 
		{
			UInt32 char_id =  m_pOwner->GetChar_id();
			if(m_pSiblingData->GetMember_id1() != char_id && gGlobal.m_charToSession.FindCharacter(m_pSiblingData->GetMember_id1())) 
			{
				bDelete = false;
				break;
			}
			if(m_pSiblingData->GetMember_id2() != char_id && gGlobal.m_charToSession.FindCharacter(m_pSiblingData->GetMember_id2())) 
			{
				bDelete = false;
				break;
			}
			if(m_pSiblingData->GetMember_id3() != char_id && gGlobal.m_charToSession.FindCharacter(m_pSiblingData->GetMember_id3())) 
			{
				bDelete = false;
				break;
			}
			if(m_pSiblingData->GetMember_id4() != char_id && gGlobal.m_charToSession.FindCharacter(m_pSiblingData->GetMember_id4())) 
			{
				bDelete = false;
				break;
			}
			if(m_pSiblingData->GetMember_id5() != char_id && gGlobal.m_charToSession.FindCharacter(m_pSiblingData->GetMember_id5())) 
			{
				bDelete = false;
				break;
			}
			if(m_pSiblingData->GetMember_id6() != char_id && gGlobal.m_charToSession.FindCharacter(m_pSiblingData->GetMember_id6())) 
			{
				bDelete = false;
				break;
			}
			if(m_pSiblingData->GetMember_id7() != char_id && gGlobal.m_charToSession.FindCharacter(m_pSiblingData->GetMember_id7())) 
			{
				bDelete = false;
				break;
			}
			if(m_pSiblingData->GetMember_id8() != char_id && gGlobal.m_charToSession.FindCharacter(m_pSiblingData->GetMember_id8())) 
			{
				bDelete = false;
				break;
			}
			if(m_pSiblingData->GetMember_id9() != char_id && gGlobal.m_charToSession.FindCharacter(m_pSiblingData->GetMember_id9())) 
			{
				bDelete = false;
				break;
			}
			if(m_pSiblingData->GetMember_id10() != char_id && gGlobal.m_charToSession.FindCharacter(m_pSiblingData->GetMember_id10())) 
			{
				bDelete = false;
				break;
			}
		} while(false);

		if(bDelete)
			DeleteNetObject(m_pSiblingData);
	}

	m_pSiblingData = NULL;

	if (m_pOwner != NULL && m_pMarriageData != NULL && m_bWedding)
	{
		CCharacter* pHusband = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetHusband_id());
		CCharacter* pWife = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetWife_id());
		if( pHusband != NULL || pWife != NULL )
		{
			if(HasLuaFunc(gGlobal.g_pLuaState, "OnMarriageFinish"))
			{
				CallLuaFunc(gGlobal.g_pLuaState, "OnMarriageFinish", String(), pHusband,pWife);
			}
		}
	}

	if (m_pMarriageData)
	{
		bool bDelete = true;
		do 
		{
			UInt32 char_id =  m_pOwner->GetChar_id();
			if(m_pMarriageData->GetHusband_id() != char_id && gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetHusband_id())) 
			{
				bDelete = false;
				break;
			}
			if(m_pMarriageData->GetWife_id() != char_id && gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetWife_id())) 
			{
				bDelete = false;
				break;
			}
		} while(false);

		if(bDelete)
			DeleteNetObject(m_pMarriageData);
	}

	m_pMarriageData = NULL;

	m_pOwner = NULL;
	m_state = ST_NONE;
	CancelAllEvent();
}

void CRelationSystem::CreateMasterAndStudent(UInt32 master_id, UInt32 student_id)
{
	if (m_pOwner == NULL)
		return;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("master_id %d, student_id %d"), master_id, student_id);
	CCharacter* pMaster = gGlobal.m_charToSession.FindCharacter(master_id);
	CCharacter* pStudent = gGlobal.m_charToSession.FindCharacter(student_id);
	if( pMaster == NULL || pStudent == NULL )
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER,
			_FE("[master or student not found], master_id %d, student_id %d"),
			master_id, student_id);
		return;
	}

	UInt16Vector relations;
	relations.push_back(FRIEND_RELATION_FRIEND);
	relations.push_back(FRIEND_RELATION_MASTER);
	MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), student_id, master_id,
		relations, FRIEND_GROUP_ONE, 0, pMaster->GetNickName());

	relations.clear();
	relations.push_back(FRIEND_RELATION_FRIEND);
	relations.push_back(FRIEND_RELATION_STUDENT);
	MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), master_id, student_id,
		relations, FRIEND_GROUP_ONE, 0, pStudent->GetNickName());
}

void CRelationSystem::DestroyMasterAndStudent(UInt32 master_id,UInt32 student_id)
{
	if (m_pOwner == NULL)
		return;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("master_id %d, student_id %d"), master_id, student_id);
	MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), student_id, master_id,FRIEND_RELATION_MASTER);
	MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), master_id, student_id,FRIEND_RELATION_STUDENT);
}
void CRelationSystem::CheckMasterAndStudentDataValidate()
{
	if (m_pOwner == NULL)
		return;

	PROCEDURE_DO_METHOD(CRelationSystem, CheckMasterAndStudentDataValidate, this, m_pOwner->GetContext())
}
RPCResult CRelationSystem::CheckMasterAndStudentDataValidate(RPCContext &context)
{
	if (m_pOwner == NULL)
	{
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL);
	}

	PROCEDURE_START_METHOD(CRelationSystem, CheckSiblingDataValidate, this, context)
	MessageService_Proxy::GetMessengers(gGlobal.pMessageService->pPeer,context,
		m_pOwner->GetChar_id(), FRIEND_RELATION_STUDENT);

	PROCEDURE_WAIT2(1, MessageService, CB_GetMessengers, UInt32, owner_id, UInt32Vector, messagers)
	UInt32Vector::iterator iter = messagers.begin();
	UInt32Vector::iterator iterend = messagers.end();
	for( ; iter != iterend; ++iter )
	{
		UInt16Vector relations;
		relations.push_back(FRIEND_RELATION_FRIEND);
		relations.push_back(FRIEND_RELATION_MASTER);
		MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
			m_pOwner->GetContext(), *iter, m_pOwner->GetChar_id(),
			relations, FRIEND_GROUP_ONE, 0, String());
	}

	MessageService_Proxy::GetMessengers(gGlobal.pMessageService->pPeer,context,
		m_pOwner->GetChar_id(), FRIEND_RELATION_MASTER);

	PROCEDURE_WAIT2(2, MessageService, CB_GetMessengers, UInt32, owner_id, UInt32Vector, messagers)
	UInt32Vector::iterator iter = messagers.begin();
	UInt32Vector::iterator iterend = messagers.end();
	for( ; iter != iterend; ++iter )
	{
		UInt16Vector relations;
		relations.push_back(FRIEND_RELATION_FRIEND);
		relations.push_back(FRIEND_RELATION_STUDENT);
		MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
			m_pOwner->GetContext(), *iter, m_pOwner->GetChar_id(),
			relations, FRIEND_GROUP_ONE, 0, String());
	}

	PROCEDURE_CATCH
	PROCEDURE_EXCEPTION_DEFAULT;
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CheckMasterAndStudentDataValidate: session_id %s, m_pOwner = %p"),
		m_pOwner->GetContext().sessionId.GetString().c_str(), m_pOwner);
	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}
//sibling
UInt32 CRelationSystem::GetSiblingRank(UInt32 sibling_id)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("m_pOwner %p, m_pSiblingData %p, sibling_id, %d"),
		m_pOwner, m_pSiblingData, sibling_id);

	if (m_pOwner == NULL || !hasSiblingData() || m_pSiblingData == NULL || sibling_id == 0)
		return 0;

	SiblingIdVec siblingIdVec;
	SiblingSuffixVec siblingSuffixVec;
	GetSiblingIdAndSuffix(siblingIdVec,siblingSuffixVec);
	if(siblingIdVec.size() != siblingSuffixVec.size() || siblingIdVec.size() <= 0)
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("GetSiblingRank: session_id %s, siblingIdVec.size() = %d, siblingSuffixVec.size() = %d"),
			m_pOwner->GetContext().sessionId.GetString().c_str(), siblingIdVec.size(), siblingSuffixVec.size());
		return 0;
	}
	UInt32 nRank = 0;

	SiblingIdVec::iterator iterId = siblingIdVec.begin();
	SiblingIdVec::iterator iterIdEnd = siblingIdVec.end();
	for(; iterId != iterIdEnd; ++iterId)
	{ 
		nRank++;
		if(*iterId == sibling_id)
			return nRank;
	}
	return 0;
}
void CRelationSystem::UpdateAllMemberTitle()
{
	if (m_pOwner == NULL || !hasSiblingData() || m_pSiblingData == NULL)
		return;

	SiblingIdVec siblingIdVec;
	SiblingSuffixVec siblingSuffixVec;
	GetSiblingIdAndSuffix(siblingIdVec,siblingSuffixVec);
	if(siblingIdVec.size() != siblingSuffixVec.size() || siblingIdVec.size() <= 0)
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("UpdateAllMemberTitle: session_id %s, siblingIdVec.size() = %d, siblingSuffixVec.size() = %d"),
			m_pOwner->GetContext().sessionId.GetString().c_str(), siblingIdVec.size(), siblingSuffixVec.size());
		return;
	}

	SiblingIdVec::iterator iterId = siblingIdVec.begin();
	SiblingIdVec::iterator iterIdEnd = siblingIdVec.end();
	for(; iterId != iterIdEnd; ++iterId)
	{ 
		CCharacter* pSibling = gGlobal.m_charToSession.FindCharacter(*iterId);
		if( pSibling != NULL)
		{
			CRelationSystem* pRelationSystem = pSibling->GetRelationSystem();
			if(pRelationSystem != NULL)
			{
				pRelationSystem->UpdateSiblingTitle();
			}
		}
	}
}
void CRelationSystem::GetSiblingIdAndSuffix(SiblingIdVec& siblingIdVec,SiblingSuffixVec &siblingSuffixVec)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("start m_pSiblingData %p, siblingIdVec size %d, siblingSuffixVec size %d"),
		m_pSiblingData, (UInt32)siblingIdVec.size(), (UInt32)siblingSuffixVec.size());

	if (!hasSiblingData() || m_pSiblingData == NULL)
		return;

	if(m_pSiblingData->GetMember_id1() != 0)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("id1 id %d"), m_pSiblingData->GetMember_id1());
		siblingIdVec.push_back(m_pSiblingData->GetMember_id1());
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("id1 suffix %d"), m_pSiblingData->GetMember_title_suffix1().c_str());
		siblingSuffixVec.push_back(m_pSiblingData->GetMember_title_suffix1());
	}
	if(m_pSiblingData->GetMember_id2() != 0)
	{
		siblingIdVec.push_back(m_pSiblingData->GetMember_id2());
		siblingSuffixVec.push_back(m_pSiblingData->GetMember_title_suffix2());
	}
	if(m_pSiblingData->GetMember_id3() != 0)
	{
		siblingIdVec.push_back(m_pSiblingData->GetMember_id3());
		siblingSuffixVec.push_back(m_pSiblingData->GetMember_title_suffix3());
	}
	if(m_pSiblingData->GetMember_id4() != 0)
	{
		siblingIdVec.push_back(m_pSiblingData->GetMember_id4());
		siblingSuffixVec.push_back(m_pSiblingData->GetMember_title_suffix4());
	}
	if(m_pSiblingData->GetMember_id5() != 0)
	{
		siblingIdVec.push_back(m_pSiblingData->GetMember_id5());
		siblingSuffixVec.push_back(m_pSiblingData->GetMember_title_suffix5());
	}
	if(m_pSiblingData->GetMember_id6() != 0)
	{
		siblingIdVec.push_back(m_pSiblingData->GetMember_id6());
		siblingSuffixVec.push_back(m_pSiblingData->GetMember_title_suffix6());
	}
	if(m_pSiblingData->GetMember_id7() != 0)
	{
		siblingIdVec.push_back(m_pSiblingData->GetMember_id7());
		siblingSuffixVec.push_back(m_pSiblingData->GetMember_title_suffix7());
	}
	if(m_pSiblingData->GetMember_id8() != 0)
	{
		siblingIdVec.push_back(m_pSiblingData->GetMember_id8());
		siblingSuffixVec.push_back(m_pSiblingData->GetMember_title_suffix8());
	}
	if(m_pSiblingData->GetMember_id9() != 0)
	{
		siblingIdVec.push_back(m_pSiblingData->GetMember_id9());
		siblingSuffixVec.push_back(m_pSiblingData->GetMember_title_suffix9());
	}
	if(m_pSiblingData->GetMember_id10() != 0)
	{
		siblingIdVec.push_back(m_pSiblingData->GetMember_id10());
		siblingSuffixVec.push_back(m_pSiblingData->GetMember_title_suffix10());
	}

}

void CRelationSystem::SetSiblingIdAndSuffix(const SiblingIdVec& siblingIdVec,const SiblingSuffixVec &siblingSuffixVec)
{
	if (!hasSiblingData() || m_pSiblingData == NULL)
		return;
	if(siblingIdVec.size() != siblingSuffixVec.size())
		return;
	size_t nSize = siblingIdVec.size();
	if(nSize > 0)
	{
		m_pSiblingData->SetMember_id1(siblingIdVec[0]);
		m_pSiblingData->SetMember_title_suffix1(siblingSuffixVec[0]);
	}
	if(nSize > 1)
	{
		m_pSiblingData->SetMember_id2(siblingIdVec[1]);
		m_pSiblingData->SetMember_title_suffix2(siblingSuffixVec[1]);
	}
	if(nSize > 2)
	{
		m_pSiblingData->SetMember_id3(siblingIdVec[2]);
		m_pSiblingData->SetMember_title_suffix3(siblingSuffixVec[2]);
	}
	if(nSize > 3)
	{
		m_pSiblingData->SetMember_id4(siblingIdVec[3]);
		m_pSiblingData->SetMember_title_suffix4(siblingSuffixVec[3]);
	}
	if(nSize > 4)
	{
		m_pSiblingData->SetMember_id5(siblingIdVec[4]);
		m_pSiblingData->SetMember_title_suffix5(siblingSuffixVec[4]);
	}
	if(nSize > 5)
	{
		m_pSiblingData->SetMember_id6(siblingIdVec[5]);
		m_pSiblingData->SetMember_title_suffix6(siblingSuffixVec[5]);
	}
	if(nSize > 6)
	{
		m_pSiblingData->SetMember_id7(siblingIdVec[6]);
		m_pSiblingData->SetMember_title_suffix7(siblingSuffixVec[6]);
	}
	if(nSize > 7)
	{
		m_pSiblingData->SetMember_id8(siblingIdVec[7]);
		m_pSiblingData->SetMember_title_suffix8(siblingSuffixVec[7]);
	}
	if(nSize > 8)
	{
		m_pSiblingData->SetMember_id9(siblingIdVec[8]);
		m_pSiblingData->SetMember_title_suffix9(siblingSuffixVec[8]);
	}
	if(nSize > 9)
	{
		m_pSiblingData->SetMember_id10(siblingIdVec[9]);
		m_pSiblingData->SetMember_title_suffix10(siblingSuffixVec[9]);
	}
}

void CRelationSystem::SetSiblingData(CSiblingData *pSiblingData)
{
	if(m_pSiblingData == pSiblingData) 
		return;
	m_pSiblingData = pSiblingData;
	if (pSiblingData)
		m_siblingDataNetID = m_pSiblingData->GetNetId();
	else
		m_siblingDataNetID = RPCNetID();
};

RPCResult CRelationSystem::CreateSibling(RPCContext &context)
{

	if (m_pOwner == NULL || m_tmpSiblingVec.size() < 5)
	{
		// check is already  no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	TmpSiblingVec::iterator iter1 = m_tmpSiblingVec.begin();
	TmpSiblingVec::iterator iterend1 = m_tmpSiblingVec.end();
	UInt16Vector relations;

	//make sure that all character is online
	for(; iter1 != iterend1; ++iter1)
	{
		UInt32 tmpCharId = *iter1;
		CCharacter* pSibling = gGlobal.m_charToSession.FindCharacter(*iter1);
		if( tmpCharId != 0 && pSibling == NULL)
		{
			// check is already  no owner
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
				context.sessionId.GetString().c_str());
			PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
		}
	}

	PROCEDURE_START_METHOD1(CRelationSystem, CreateSibling, this, context,String, prefix)

	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, 
	_T("CreateSibling: char_id %d, sessionId %s "),
	m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str());

	ObjectService_Proxy::CreateSibling(gGlobal.pObjectService->pPeer,context,prefix,
		m_tmpSiblingVec[0],""
		,m_tmpSiblingVec[1],""
		,m_tmpSiblingVec[2],""
		,m_tmpSiblingVec[3],""
		,m_tmpSiblingVec[4],""
		,0,"",0,"",0,"",0,"",0,"");

	PROCEDURE_WAIT1(1, ObjectService, CB_CreateSibling, NetGroup*, grp)
	CSiblingData * pSiblingData = grp->GetChild<CSiblingData>(0);
	if(pSiblingData == NULL || pSiblingData->GetSibling_id() == 0)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER,
			_T("CreateSibling: char_id %d, sessionId %s,CB_CreateSibling error pSiblingData is NULL"),
			m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str());
		if(HasLuaFunc(gGlobal.g_pLuaState,"OnCreateSiblingResult"))
			CallLuaFunc(gGlobal.g_pLuaState, "OnCreateSiblingResult", String(), m_pOwner,false);
		DeleteNetObject(pSiblingData);
		PROCEDURE_RETURN
	}

	// set sibling id
	TmpSiblingVec::iterator iter2;
	TmpSiblingVec::iterator iterend2;
	String strSiblingNames;

	iter1 = m_tmpSiblingVec.begin();
	iterend1 = m_tmpSiblingVec.end();
	for(; iter1 != iterend1; ++iter1)
	{
		if(*iter1 == 0) 
			continue;
		CCharacter* pSibling = gGlobal.m_charToSession.FindCharacter(*iter1);
		if( pSibling != NULL) 
		{
			pSibling->SetSibling_id(pSiblingData->GetSibling_id());
			if(!strSiblingNames.IsEmpty())
			{
				strSiblingNames += " ";
			}
			strSiblingNames += pSibling->GetCharNameLink();
			CRelationSystem * pRelationSystem = pSibling->GetRelationSystem();
			if (pRelationSystem)
			{
				pRelationSystem->SetSiblingData(pSiblingData);
				pRelationSystem->UpdateSiblingTitle();
			}
			
			iter2 = m_tmpSiblingVec.begin();
			iterend2 = m_tmpSiblingVec.end();
			for(; iter2 != iterend2; ++iter2)
			{
				if(*iter2 == 0) 
					continue;
				if(*iter1 != *iter2)
				{
					relations.clear();
					relations.push_back(FRIEND_RELATION_FRIEND);
					relations.push_back(FRIEND_RELATION_SIBLING);
					MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
						m_pOwner->GetContext(), *iter1, *iter2,
						relations, FRIEND_GROUP_ONE, 0, String());
				}
			}
		}
	}

	if(HasLuaFunc(gGlobal.g_pLuaState,"OnCreateSiblingResult"))
		CallLuaFunc(gGlobal.g_pLuaState, "OnCreateSiblingResult", String(), m_pOwner,true);
	String msg;
	msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_SUCCESS_RUMOR")),strSiblingNames.c_str());
	MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, RPCContext(), msg, 0, 0);

	m_tmpSiblingVec.clear();
	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, 
	_T("CreateSibling success: char_id %d, sessionId %s"),
	m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str());

	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION_DEFAULT
	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CRelationSystem::UpdateSiblingTitle()
{
	if (m_pOwner == NULL) 
		return;
	UInt32 nPreTitleId = m_pOwner->GetTitle_id();
	if(m_pOwner->HasTitle(gConfigure.siblingTitle))
		m_pOwner->RemoveTitle(gConfigure.siblingTitle);
	if (!hasSiblingData() || m_pSiblingData == NULL) 
		return;
	String prefix = m_pSiblingData->GetSibling_title_prefix();
	String strThirdWord = GetSiblingTitlePrefixThirdWord();
	if(strThirdWord.IsEmpty())
		return;
	prefix.Insert(2,strThirdWord.c_str());
	String suffix = GetSiblingTitleSuffix();
	if(suffix.IsEmpty())
		return;
	prefix = prefix + gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_PREFIX_MIDDLE")) + suffix;
	m_pOwner->AddTitle(gConfigure.siblingTitle, 0, 0, prefix);
	if(nPreTitleId == gConfigure.siblingTitle)
	{
		m_pOwner->SetTitle(prefix,gConfigure.siblingTitle);
	}

}

void CRelationSystem::CheckSiblingDataValidate()
{
	if (m_pOwner == NULL)
		return;
	
	PROCEDURE_DO_METHOD(CRelationSystem, CheckSiblingDataValidate, this, m_pOwner->GetContext())
}

RPCResult CRelationSystem::CheckSiblingDataValidate(RPCContext &context)
{
	if (m_pOwner == NULL)
	{
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL);
	}

	PROCEDURE_START_METHOD(CRelationSystem, CheckSiblingDataValidate, this, context)

	TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, 
		_T("GetMessengers: char_id %d, sessionId %s, target_id %d, relation %d"),
		m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), m_pOwner->GetChar_id(), 
		FRIEND_RELATION_SIBLING);
	MessageService_Proxy::GetMessengers(gGlobal.pMessageService->pPeer,context,
		m_pOwner->GetChar_id(), FRIEND_RELATION_SIBLING);
	//PROCEDURE_SET_TIMEOUT(2000, BOOLEAN_TRUE)

	PROCEDURE_WAIT2(1, MessageService, CB_GetMessengers, UInt32, owner_id, UInt32Vector, messagers)

	SiblingIdVec siblingIdVec;
	SiblingSuffixVec siblingSuffixVec;
	GetSiblingIdAndSuffix(siblingIdVec,siblingSuffixVec);
	if(siblingIdVec.size() != siblingSuffixVec.size())
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("CheckSiblingDataValidate: session_id %s, siblingIdVec.size() = %d, siblingSuffixVec.size() = %d"),
			m_pOwner->GetContext().sessionId.GetString().c_str(), siblingIdVec.size(), siblingSuffixVec.size());
		PROCEDURE_RETURN;
	}

	if(!hasSiblingData() || m_pSiblingData == NULL || siblingIdVec.size() == 0 || std::find(siblingIdVec.begin(),siblingIdVec.end(),m_pOwner->GetChar_id()) == siblingIdVec.end())
	{
		if(messagers.size() > 1)
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CheckSiblingDataValidate: session_id %s, m_pSiblingData == NULL || siblingIdVec.size() == 0 but messagers.size() > 1"),
				m_pOwner->GetContext().sessionId.GetString().c_str());
		}
		UInt32Vector::iterator iterMessager = messagers.begin();
		UInt32Vector::iterator iterMessagerEnd = messagers.end();
		for(; iterMessager != iterMessagerEnd; ++iterMessager)
		{ 
			if( *iterMessager != 0 )
			{
				MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
					m_pOwner->GetContext(), m_pOwner->GetChar_id(), *iterMessager,FRIEND_RELATION_SIBLING);
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CheckSiblingDataValidate: remove sibling relation, session_id %s, char id = %d"),
					m_pOwner->GetContext().sessionId.GetString().c_str(), *iterMessager);
			}
		}
		m_pOwner->SetSibling_id(0);
		PROCEDURE_RETURN;
	}


	UInt32Vector::iterator iterMessager = messagers.begin();
	UInt32Vector::iterator iterMessagerEnd = messagers.end();
	for(; iterMessager != iterMessagerEnd; ++iterMessager)
	{ 
		if(std::find(siblingIdVec.begin(),siblingIdVec.end(),*iterMessager) == siblingIdVec.end())
		{
			MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
				m_pOwner->GetContext(), m_pOwner->GetChar_id(), *iterMessager,FRIEND_RELATION_SIBLING);
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CheckSiblingDataValidate: remove sibling relation, session_id %s, char id = %d"),
				m_pOwner->GetContext().sessionId.GetString().c_str(), *iterMessager);
		}
	}

	UInt16Vector relations;
	SiblingIdVec::iterator iterId = siblingIdVec.begin();
	SiblingIdVec::iterator iterIdEnd = siblingIdVec.end();
	for(; iterId != iterIdEnd; ++iterId)
	{ 
		if(*iterId != 0 && std::find(messagers.begin(),messagers.end(),*iterId) == messagers.end())
		{
			relations.clear();
			relations.push_back(FRIEND_RELATION_FRIEND);
			relations.push_back(FRIEND_RELATION_SIBLING);
			MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
				m_pOwner->GetContext(), m_pOwner->GetChar_id(), *iterId,
				relations, FRIEND_GROUP_ONE, 0, String());
			MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
				m_pOwner->GetContext(), *iterId, m_pOwner->GetChar_id(),
				relations, FRIEND_GROUP_ONE, 0, String());
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CheckSiblingDataValidate: create sibling relation, session_id %s, char id = %d"),
				m_pOwner->GetContext().sessionId.GetString().c_str(), *iterId);
		}
	}

	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, 
		_T("CheckSiblingDataValidate: char_id %d, sessionId %s"),
		m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str());

	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION_DEFAULT;
	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}
RPCResult CRelationSystem::SetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &prefix)
{
	if(GetState() == ST_CREATE_SIBLING)
	{
		if (!gGlobal.CanUsedName(prefix))
		{
			m_pOwner->CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), 1, _T("MSG_SIBLING_TITLE_PREFIX_ERROR"));
			SetState(ST_NONE);
			return RPC_RESULT_FAIL;
		}

		PROCEDURE_DO_METHOD1(CRelationSystem, CreateSibling, this, context,String, prefix)
		SetState(ST_NONE);
	}
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SetSiblingTitlePrefix: session_id %s, prefix %s"),
		context.sessionId.GetString().c_str(), prefix.c_str());
	return RPC_RESULT_OK;
}

RPCResult CRelationSystem::SetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &suffix)
{
	bool bResult = false;
	do 
	{
		if (m_pOwner == NULL)
			break;
		if (!gGlobal.CanUsedName(suffix))
		{
			m_pOwner->CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), 1, _T("MSG_SIBLING_TITLE_SUFFIX_ERROR"));
			break;
		}
		if (hasSiblingData() && m_pSiblingData)
		{
			do 
			{
				UInt32 char_id =  m_pOwner->GetChar_id();
				if(m_pSiblingData->GetMember_id1() == char_id) 
				{
					m_pSiblingData->SetMember_title_suffix1(suffix);
					break;
				}
				if(m_pSiblingData->GetMember_id2() == char_id) 
				{
					m_pSiblingData->SetMember_title_suffix2(suffix);
					break;
				}
				if(m_pSiblingData->GetMember_id3() == char_id) 
				{
					m_pSiblingData->SetMember_title_suffix3(suffix);
					break;
				}
				if(m_pSiblingData->GetMember_id4() == char_id) 
				{
					m_pSiblingData->SetMember_title_suffix4(suffix);
					break;
				}
				if(m_pSiblingData->GetMember_id5() == char_id) 
				{
					m_pSiblingData->SetMember_title_suffix5(suffix);
					break;
				}
				if(m_pSiblingData->GetMember_id6() == char_id) 
				{
					m_pSiblingData->SetMember_title_suffix6(suffix);
					break;
				}
				if(m_pSiblingData->GetMember_id7() == char_id) 
				{
					m_pSiblingData->SetMember_title_suffix7(suffix);
					break;
				}
				if(m_pSiblingData->GetMember_id8() == char_id) 
				{
					m_pSiblingData->SetMember_title_suffix8(suffix);
					break;
				}
				if(m_pSiblingData->GetMember_id9() == char_id) 
				{
					m_pSiblingData->SetMember_title_suffix9(suffix);
					break;
				}
				if(m_pSiblingData->GetMember_id10() == char_id) 
				{
					m_pSiblingData->SetMember_title_suffix10(suffix);
					break;
				}
			} while(false);

			ObjectService_Proxy::UpdateSibling(gGlobal.pObjectService->pPeer,context,m_pSiblingData);
			UpdateSiblingTitle();
			bResult = true;
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SetSiblingTitleSuffix: session_id %s, suffix %s"),
				context.sessionId.GetString().c_str(), suffix.c_str());
		}

	} while(false);

	if(HasLuaFunc(gGlobal.g_pLuaState,"OnSetSiblingTitleSuffixResult"))
		CallLuaFunc(gGlobal.g_pLuaState, "OnSetSiblingTitleSuffixResult", String(), m_pOwner,bResult);
	return RPC_RESULT_OK;
}


void CRelationSystem::RequestSetSiblingTitlePrefix()
{
	if(m_pOwner == NULL) return;
	m_pOwner->CB_RelationSystem(m_pOwner->GetPeer(),m_pOwner->GetContext(),this);
	UInt32 nCount = 0;
	TmpSiblingVec::iterator iter = m_tmpSiblingVec.begin();
	TmpSiblingVec::iterator iterend = m_tmpSiblingVec.end();
	for(; iter != iterend; ++iter)
	{
		if(*iter != 0)
			nCount++;
	}
	CB_RequestSetSiblingTitlePrefix(m_pOwner->GetPeer(),m_pOwner->GetContext(),GetSiblingTitlePrefixThirdWord(nCount));
}

void CRelationSystem::RequestSetSiblingTitleSuffix()
{
	if (m_pOwner == NULL || !hasSiblingData() || m_pSiblingData == NULL) 
		return;
	String prefix = GetSiblingTitlePrefix();
	if(prefix.IsEmpty())
		return;

	m_pOwner->CB_RelationSystem(m_pOwner->GetPeer(),m_pOwner->GetContext(),this);
	CB_RequestSetSiblingTitleSuffix(m_pOwner->GetPeer(),m_pOwner->GetContext(),prefix);
}

void CRelationSystem::RequestSelectMarriageTime(UInt32 husband_id,UInt32 wife_id)
{
	if (m_pOwner == NULL || husband_id == 0 || wife_id == 0) 
		return;

	m_tmpHusbandId = husband_id;
	m_tmpWifeId = wife_id;
	m_pOwner->CB_RelationSystem(m_pOwner->GetPeer(),m_pOwner->GetContext(),this);
	CB_RequestSelectMarriageTime(m_pOwner->GetPeer(),m_pOwner->GetContext());
}

void CRelationSystem::CreateSibling(UInt32 sibling_id1,UInt32 sibling_id2,UInt32 sibling_id3,UInt32 sibling_id4,UInt32 sibling_id5)
{
	if(m_state != ST_NONE) 
		return;
	m_tmpSiblingVec.clear();
	m_tmpSiblingVec.push_back(sibling_id1);
	m_tmpSiblingVec.push_back(sibling_id2);
	m_tmpSiblingVec.push_back(sibling_id3);
	m_tmpSiblingVec.push_back(sibling_id4);
	m_tmpSiblingVec.push_back(sibling_id5);
	RequestSetSiblingTitlePrefix();
	m_state = ST_CREATE_SIBLING;
}
void CRelationSystem::RemoveSibling(UInt32 sibling_id)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("m_pOwner %p, m_pSiblingData %p, sibling_id %d"),
		m_pOwner, m_pSiblingData, sibling_id);

	if ( m_pOwner == NULL || !hasSiblingData() || m_pSiblingData == NULL )
		return;

	SiblingIdVec siblingIdVec;
	SiblingSuffixVec siblingSuffixVec;
	GetSiblingIdAndSuffix(siblingIdVec,siblingSuffixVec);
	if(siblingIdVec.size() != siblingSuffixVec.size() || siblingIdVec.size() <= 0)
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("RemoveSibling: session_id %s, siblingIdVec.size() = %d, siblingSuffixVec.size() = %d"),
			m_pOwner->GetContext().sessionId.GetString().c_str(), siblingIdVec.size(), siblingSuffixVec.size());
		return;
	}

	SiblingIdVec::iterator iterId = siblingIdVec.begin();
	SiblingIdVec::iterator iterIdEnd = siblingIdVec.end();
	SiblingSuffixVec::iterator iterSuffix = siblingSuffixVec.begin();
	for(; iterId != iterIdEnd; ++iterId,++iterSuffix)
	{ 
		if(*iterId == sibling_id)
		{
			siblingIdVec.erase(iterId);
			siblingSuffixVec.erase(iterSuffix);
			SiblingIdVec::iterator iterId1 = siblingIdVec.begin();
			SiblingIdVec::iterator iterIdEnd1 = siblingIdVec.end();
			for(; iterId1 != iterIdEnd1; ++iterId1)
			{ 
				if(*iterId1 == 0) continue;
				MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
					m_pOwner->GetContext(), sibling_id, *iterId1,FRIEND_RELATION_SIBLING);
				MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
					m_pOwner->GetContext(), *iterId1, sibling_id,FRIEND_RELATION_SIBLING);

				CCharacter* pSibling = gGlobal.m_charToSession.FindCharacter(*iterId1);
				if( pSibling != NULL)
				{
					CRelationSystem* pRelationSystem = pSibling->GetRelationSystem();
					if(pRelationSystem != NULL)
					{
						pRelationSystem->UpdateSiblingTitle();
					}
				}
			}
			break;
		}
	}

	if (siblingIdVec.size() <= 1)
	{
		ObjectService_Proxy::DeleteSibling(gGlobal.pObjectService->pPeer,m_pOwner->GetContext(),m_pSiblingData->GetSibling_id());
		DeleteNetObject(m_pSiblingData);
		SetSiblingData(NULL);
		iterId = siblingIdVec.begin();
		iterIdEnd = siblingIdVec.end();
		for(; iterId != iterIdEnd; ++iterId)
		{ 
			CCharacter* pSibling = gGlobal.m_charToSession.FindCharacter(*iterId);
			if( pSibling != NULL)
			{
				pSibling->SetSibling_id(0);
				CRelationSystem* pRelationSystem = pSibling->GetRelationSystem();
				if(pRelationSystem != NULL)
				{
					pRelationSystem->SetSiblingData(NULL);
					pRelationSystem->UpdateSiblingTitle();
				}
			}
		}
	}
	else
	{
		siblingIdVec.push_back(0);
		siblingSuffixVec.push_back("");
		SetSiblingIdAndSuffix(siblingIdVec,siblingSuffixVec);
		ObjectService_Proxy::UpdateSibling(gGlobal.pObjectService->pPeer,m_pOwner->GetContext(),m_pSiblingData);
		UpdateAllMemberTitle();
	}

	CCharacter* pChar = gGlobal.m_charToSession.FindCharacter(sibling_id);
	if( pChar != NULL)
	{
		CRelationSystem* pRelationSystem = pChar->GetRelationSystem();
		if(pRelationSystem != NULL)
		{
			pRelationSystem->SetSiblingData(NULL);
			pRelationSystem->UpdateSiblingTitle();
		}
		pChar->SetSibling_id(0);
	}
}
void CRelationSystem::AddSibling(UInt32 sibling_id1,UInt32 sibling_id2,UInt32 sibling_id3,UInt32 sibling_id4)
{
	if (m_pOwner == NULL || !hasSiblingData() || m_pSiblingData == NULL)
		return;

	SiblingIdVec siblingIdVec;
	SiblingSuffixVec siblingSuffixVec;
	GetSiblingIdAndSuffix(siblingIdVec,siblingSuffixVec);
	if(siblingIdVec.size() != siblingSuffixVec.size() || siblingIdVec.size() <= 0)
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("AddSibling: session_id %s, siblingIdVec.size() = %d, siblingSuffixVec.size() = %d"),
			m_pOwner->GetContext().sessionId.GetString().c_str(), siblingIdVec.size(), siblingSuffixVec.size());
		return;
	}

	UInt16Vector relations;
	TmpSiblingVec tmpSiblingVec;
	if(sibling_id1 != 0 && find(siblingIdVec.begin(),siblingIdVec.end(),sibling_id1) == siblingIdVec.end()) tmpSiblingVec.push_back(sibling_id1);
	if(sibling_id2 != 0 && find(siblingIdVec.begin(),siblingIdVec.end(),sibling_id2) == siblingIdVec.end()) tmpSiblingVec.push_back(sibling_id2);
	if(sibling_id3 != 0 && find(siblingIdVec.begin(),siblingIdVec.end(),sibling_id3) == siblingIdVec.end()) tmpSiblingVec.push_back(sibling_id3);
	if(sibling_id4 != 0 && find(siblingIdVec.begin(),siblingIdVec.end(),sibling_id4) == siblingIdVec.end()) tmpSiblingVec.push_back(sibling_id4);

	if(siblingIdVec.size() + tmpSiblingVec.size() > 10)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("AddSibling: session_id %s, siblingIdVec.size() + tmpSiblingVec.size() > 10"),
			m_pOwner->GetContext().sessionId.GetString().c_str());
		return;
	}
	

	TmpSiblingVec::iterator iter = tmpSiblingVec.begin();
	TmpSiblingVec::iterator iterend = tmpSiblingVec.end();
	for(; iter != iterend; ++iter)
	{
		if(find(siblingIdVec.begin(),siblingIdVec.end(),*iter) != siblingIdVec.end()) 
			continue;
		CCharacter* pSibling = gGlobal.m_charToSession.FindCharacter(*iter);
		if( pSibling == NULL) 
			continue;

		siblingIdVec.push_back(*iter);
		siblingSuffixVec.push_back("");

		pSibling->SetSibling_id(m_pOwner->GetSibling_id());
		CRelationSystem* pRelationSystem = pSibling->GetRelationSystem();
		if(pRelationSystem == NULL) continue;
		pRelationSystem->SetSiblingData(m_pSiblingData);
		
		SiblingIdVec::iterator iterId1 = siblingIdVec.begin();
		SiblingIdVec::iterator iterIdEnd1 = siblingIdVec.end();
		for(; iterId1 != iterIdEnd1; ++iterId1)
		{ 
			if(*iterId1 == 0 || *iterId1 == *iter) continue;
			relations.clear();
			relations.push_back(FRIEND_RELATION_FRIEND);
			relations.push_back(FRIEND_RELATION_SIBLING);
			MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
				m_pOwner->GetContext(), *iter, *iterId1,
				relations, FRIEND_GROUP_ONE, 0, String());
			MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
				m_pOwner->GetContext(), *iterId1, *iter,
				relations, FRIEND_GROUP_ONE, 0, String());
		}
	}

	SetSiblingIdAndSuffix(siblingIdVec,siblingSuffixVec);

	ObjectService_Proxy::UpdateSibling(gGlobal.pObjectService->pPeer,m_pOwner->GetContext(),m_pSiblingData);
	UpdateAllMemberTitle();
	
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("AddSibling: session_id %s, sibling_id1 %d, sibling_id2 %d, sibling_id3 %d, sibling_id4 %d"),
		m_pOwner->GetContext().sessionId.GetString().c_str(), sibling_id1,sibling_id2,sibling_id3,sibling_id4);
	return;
}

String CRelationSystem::GetSiblingTitlePrefixThirdWord(UInt32 count)
{
	switch(count)
	{
	case 0:
	case 1:
		break;
	case 2: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_PREFIX2"));
	case 3: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_PREFIX3"));
	case 4: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_PREFIX4"));
	case 5: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_PREFIX5"));
	case 6: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_PREFIX6"));
	case 7: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_PREFIX7"));
	case 8: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_PREFIX8"));
	case 9: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_PREFIX9"));
	case 10: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_PREFIX10"));
	default:
		break;
	}

	return String();
}

String CRelationSystem::GetSiblingTitlePrefixThirdWord()
{
	if (m_pOwner == NULL || !hasSiblingData() || m_pSiblingData == NULL)
		return String();

	UInt32 nCount = 0;
	if(m_pSiblingData->GetMember_id1() != 0) nCount++;
	if(m_pSiblingData->GetMember_id2() != 0) nCount++;
	if(m_pSiblingData->GetMember_id3() != 0) nCount++;
	if(m_pSiblingData->GetMember_id4() != 0) nCount++;
	if(m_pSiblingData->GetMember_id5() != 0) nCount++;
	if(m_pSiblingData->GetMember_id6() != 0) nCount++;
	if(m_pSiblingData->GetMember_id7() != 0) nCount++;
	if(m_pSiblingData->GetMember_id8() != 0) nCount++;
	if(m_pSiblingData->GetMember_id9() != 0) nCount++;
	if(m_pSiblingData->GetMember_id10() != 0) nCount++;

	return GetSiblingTitlePrefixThirdWord(nCount);
}
String CRelationSystem::GetSiblingTitlePrefix()
{
	if (m_pOwner == NULL || !hasSiblingData() || m_pSiblingData == NULL) 
		return String();
	String prefix = m_pSiblingData->GetSibling_title_prefix();
	String strThirdWord = GetSiblingTitlePrefixThirdWord();
	if(strThirdWord.IsEmpty())
		return String();
	prefix.Insert(2,strThirdWord.c_str());
	return prefix;
}
String CRelationSystem::GetSiblingTitleSuffix()
{
	if (m_pOwner == NULL || !hasSiblingData() || m_pSiblingData == NULL)
		return String();
	String suffix;
	UInt32 nRank = 0;
	do 
	{
		if(m_pSiblingData->GetMember_id1() == m_pOwner->GetChar_id()) 
		{
			suffix = m_pSiblingData->GetMember_title_suffix1();
			if(!suffix.IsEmpty()) return suffix;
			nRank = 1;
			break;
		}
		if(m_pSiblingData->GetMember_id2() == m_pOwner->GetChar_id()) 
		{
			suffix = m_pSiblingData->GetMember_title_suffix2();
			if(!suffix.IsEmpty()) return suffix;
			nRank = 2;
			break;
		}
		if(m_pSiblingData->GetMember_id3() == m_pOwner->GetChar_id()) 
		{
			suffix = m_pSiblingData->GetMember_title_suffix3();
			if(!suffix.IsEmpty()) return suffix;
			nRank = 3;
			break;
		}
		if(m_pSiblingData->GetMember_id4() == m_pOwner->GetChar_id()) 
		{
			suffix = m_pSiblingData->GetMember_title_suffix4();
			if(!suffix.IsEmpty()) return suffix;
			nRank = 4;
			break;
		}
		if(m_pSiblingData->GetMember_id5() == m_pOwner->GetChar_id()) 
		{
			suffix = m_pSiblingData->GetMember_title_suffix5();
			if(!suffix.IsEmpty()) return suffix;
			nRank = 5;
			break;
		}
		if(m_pSiblingData->GetMember_id6() == m_pOwner->GetChar_id()) 
		{
			suffix = m_pSiblingData->GetMember_title_suffix6();
			if(!suffix.IsEmpty()) return suffix;
			nRank = 6;
			break;
		}
		if(m_pSiblingData->GetMember_id7() == m_pOwner->GetChar_id()) 
		{
			suffix = m_pSiblingData->GetMember_title_suffix7();
			if(!suffix.IsEmpty()) return suffix;
			nRank = 7;
			break;
		}
		if(m_pSiblingData->GetMember_id8() == m_pOwner->GetChar_id()) 
		{
			suffix = m_pSiblingData->GetMember_title_suffix8();
			if(!suffix.IsEmpty()) return suffix;
			nRank = 8;
			break;
		}
		if(m_pSiblingData->GetMember_id9() == m_pOwner->GetChar_id()) 
		{
			suffix = m_pSiblingData->GetMember_title_suffix9();
			if(!suffix.IsEmpty()) return suffix;
			nRank = 9;
			break;
		}
		if(m_pSiblingData->GetMember_id10() == m_pOwner->GetChar_id()) 
		{
			suffix = m_pSiblingData->GetMember_title_suffix10();
			if(!suffix.IsEmpty()) return suffix;
			nRank = 10;
			break;
		}
	} while(false);

	switch(nRank)
	{
	case 1: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_SUFFIX1"));
	case 2: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_SUFFIX2"));
	case 3: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_SUFFIX3"));
	case 4: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_SUFFIX4"));
	case 5: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_SUFFIX5"));
	case 6: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_SUFFIX6"));
	case 7: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_SUFFIX7"));
	case 8: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_SUFFIX8"));
	case 9: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_SUFFIX9"));
	case 10: return gGlobal.GetStringTable()->Get(_T("MSG_SIBLING_TITLE_SUFFIX10"));
	default:
		break;
	}
	return String();
}

//husband and wife
void CRelationSystem::SetMarriageData(CMarriageData *pMarriageData)
{
	if(m_pMarriageData == pMarriageData) 
		return;
	m_pMarriageData = pMarriageData;
}
void CRelationSystem::CreateMarriageRelationship(UInt32 husband_id,UInt32 wife_id)
{
	if (m_pOwner == NULL || m_pMarriageData == NULL)
		return;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateMarriageRelationship husband_id %d, wife_id %d"), husband_id, wife_id);
	CCharacter* pHusband = gGlobal.m_charToSession.FindCharacter(husband_id);
	CCharacter* pWife = gGlobal.m_charToSession.FindCharacter(wife_id);
	if( pHusband == NULL || pWife == NULL )
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER,_FE("CreateMarriageRelationship [husband or wife not found], husband_id %d, wife_id %d"),husband_id, wife_id);
		return;
	}

	UInt16Vector relations;
	relations.push_back(FRIEND_RELATION_FRIEND);
	relations.push_back(FRIEND_RELATION_HUSBAND);
	MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), wife_id, husband_id,
		relations, FRIEND_GROUP_ONE, 0, pHusband->GetNickName());

	relations.clear();
	relations.push_back(FRIEND_RELATION_FRIEND);
	relations.push_back(FRIEND_RELATION_WIFE);
	MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), husband_id, wife_id,
		relations, FRIEND_GROUP_ONE, 0, pWife->GetNickName());

	m_pMarriageData->SetState(1);
	m_allMarriageCount++;
	ObjectService_Proxy::UpdateMarriage(gGlobal.pObjectService->pPeer,m_pOwner->GetContext(),m_pMarriageData);
}

void CRelationSystem::DestroyMarriage(UInt32 husband_id,UInt32 wife_id)
{
	if (m_pOwner == NULL || m_pMarriageData == NULL)
		return;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("DestroyMarriage husband_id %d, wife_id %d"), husband_id, wife_id);
	MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), wife_id, husband_id,FRIEND_RELATION_HUSBAND);
	MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
		m_pOwner->GetContext(), husband_id, wife_id,FRIEND_RELATION_WIFE);

	ObjectService_Proxy::DeleteMarriage(gGlobal.pObjectService->pPeer,m_pOwner->GetContext(),m_pMarriageData->GetMarriage_id());
	DeleteNetObject(m_pMarriageData);
	CCharacter* pHusband = gGlobal.m_charToSession.FindCharacter(husband_id);
	if( pHusband != NULL) 
	{
		CRelationSystem * pRelationSystem = pHusband->GetRelationSystem();
		if (pRelationSystem)
		{
			pRelationSystem->SetMarriageData(NULL);
		}
	}

	CCharacter* pWife = gGlobal.m_charToSession.FindCharacter(wife_id);
	if( pWife != NULL) 
	{
		CRelationSystem * pRelationSystem = pWife->GetRelationSystem();
		if (pRelationSystem)
		{
			pRelationSystem->SetMarriageData(NULL);
		}
	}
	m_allMarriageCount--;
}
RPCResult CRelationSystem::CreateMarriage(RPCContext &context)
{
	if (m_pOwner == NULL)
	{
		// check is already  no owner
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("sessionId %s"),
			context.sessionId.GetString().c_str());
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}
	PROCEDURE_START_METHOD3(CRelationSystem, CreateMarriage, this, context,UInt32, husband_id,UInt32,wife_id,UInt32,marriage_time)
	ObjectService_Proxy::CreateMarriage(gGlobal.pObjectService->pPeer,context,husband_id,wife_id,marriage_time,0);
	PROCEDURE_WAIT1(1, ObjectService, CB_CreateMarriage, NetGroup*, grp)
	CMarriageData * pMarriageData = grp->GetChild<CMarriageData>(0);
	if(pMarriageData == NULL)
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER,
			_T("CreateMarriage: char_id %d, sessionId %s,CB_CreateMarriage error pMarriageData:%p"),
			m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(),pMarriageData);
		if(HasLuaFunc(gGlobal.g_pLuaState,"OnCreateMarriageResult"))
			CallLuaFunc(gGlobal.g_pLuaState, "OnCreateMarriageResult", String(), m_pOwner,false);
		DeleteNetObject(pMarriageData);
		PROCEDURE_RETURN
	}

	if(pMarriageData->GetMarriage_id() == 0)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER,
			_T("CreateMarriage: char_id %d, sessionId %s,CB_CreateMarriage error pMarriageData->GetMarriage_id() == 0"),
			m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str());
		if(HasLuaFunc(gGlobal.g_pLuaState,"OnCreateMarriageResult"))
			CallLuaFunc(gGlobal.g_pLuaState, "OnCreateMarriageResult", String(), m_pOwner,false);
		DeleteNetObject(pMarriageData);
		PROCEDURE_RETURN
	}

	CCharacter* pHusband = gGlobal.m_charToSession.FindCharacter(husband_id);
	if( pHusband != NULL) 
	{
		CRelationSystem * pRelationSystem = pHusband->GetRelationSystem();
		if (pRelationSystem)
		{
			pRelationSystem->SetMarriageData(pMarriageData);
		}
	}

	CCharacter* pWife = gGlobal.m_charToSession.FindCharacter(wife_id);
	if( pWife != NULL) 
	{
		CRelationSystem * pRelationSystem = pWife->GetRelationSystem();
		if (pRelationSystem)
		{
			pRelationSystem->SetMarriageData(pMarriageData);
		}
	}

	if(HasLuaFunc(gGlobal.g_pLuaState,"OnCreateMarriageResult"))
		CallLuaFunc(gGlobal.g_pLuaState, "OnCreateMarriageResult", String(), m_pOwner,true);
	//m_pOwner->CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), 1, _T("MSG_MARRIAGE_CREATE_SUCCESS"));
	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION_DEFAULT
	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}

void CRelationSystem::CreateMarriage(UInt32 husband_id,UInt32 wife_id,UInt32 marriage_time)
{
	if (m_pOwner == NULL)
		return;
	time_t tmp_time = (time_t)marriage_time;
	struct tm* ptime = localtime(&tmp_time);
	ptime->tm_min += MARRIAGE_INTERVAL;
	time_t over_time = mktime(ptime);

	if(::time(NULL) >= over_time)
	{
		if(HasLuaFunc(gGlobal.g_pLuaState,"OnCreateMarriageResult"))
			CallLuaFunc(gGlobal.g_pLuaState, "OnCreateMarriageResult", String(), m_pOwner,false);
		return;
	}
	if (m_pMarriageData != NULL)
	{
		ObjectService_Proxy::DeleteMarriage(gGlobal.pObjectService->pPeer,m_pOwner->GetContext(),m_pMarriageData->GetMarriage_id());
		CCharacter* pHusband = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetHusband_id());
		if( pHusband != NULL && m_pMarriageData->GetHusband_id() != m_pOwner->GetChar_id()) 
		{
			CRelationSystem * pRelationSystem = pHusband->GetRelationSystem();
			if (pRelationSystem)
			{
				pRelationSystem->SetMarriageData(NULL);
			}
		}

		CCharacter* pWife = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetWife_id());
		if( pWife != NULL && m_pMarriageData->GetWife_id() != m_pOwner->GetChar_id() )
		{
			CRelationSystem * pRelationSystem = pWife->GetRelationSystem();
			if (pRelationSystem)
			{
				pRelationSystem->SetMarriageData(NULL);
			}
		}
		DeleteNetObject(m_pMarriageData);
		SetMarriageData(NULL);
	}
	PROCEDURE_DO_METHOD3(CRelationSystem, CreateMarriage, this, m_pOwner->GetContext(),UInt32, husband_id,UInt32, wife_id,UInt32, marriage_time);
}

void CRelationSystem::ExecuteMarriage(UInt32 husband_id,UInt32 wife_id)
{
	if (m_pOwner == NULL || m_pMarriageData == NULL)
		return;

	time_t marriage_time = m_pMarriageData->GetMarriage_time();
	struct tm* ptime = localtime(&marriage_time);
	ptime->tm_min += MARRIAGE_INTERVAL;
	time_t over_time = mktime(ptime);

	if(m_pMarriageData->GetState() == 0 && ::time(NULL) >= over_time)
	{
		ObjectService_Proxy::DeleteMarriage(gGlobal.pObjectService->pPeer,m_pOwner->GetContext(),m_pMarriageData->GetMarriage_id());
		bool bDelete = true;
		do 
		{
			UInt32 char_id =  m_pOwner->GetChar_id();
			if(m_pMarriageData->GetHusband_id() != char_id && gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetHusband_id())) 
			{
				bDelete = false;
				break;
			}
			if(m_pMarriageData->GetWife_id() != char_id && gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetWife_id())) 
			{
				bDelete = false;
				break;
			}
		} while(false);

		if(bDelete)
			DeleteNetObject(m_pMarriageData);
		SetMarriageData(NULL);
		return;
	}

	if(husband_id != m_pMarriageData->GetHusband_id() || wife_id != m_pMarriageData->GetWife_id())
		return;
	CreateMarriageRelationship(husband_id,wife_id);
	RaiseUniqEventID(EVT_WAIT1,1000);
	m_bWedding = BOOLEAN_TRUE;
}

UInt32 CRelationSystem::GetMarriageTime()
{
	if(m_pMarriageData != NULL)
		return m_pMarriageData->GetMarriage_time();
	return 0;
}

void CRelationSystem::HandleEvent(CONST Event& event)
{
	switch (event.GetID())
	{
	case EVT_WAIT1:	
	case EVT_WAIT2:	
	case EVT_WAIT3:	
		OnMarriageWaitEvent(event.GetID());
		break;
	case EVT_WEDDING1:
	case EVT_WEDDING2:	
	case EVT_WEDDING3:	
	case EVT_WEDDING4:		
	case EVT_RESET:		
	case EVT_WEDDING_STOP:
		OnMarriageEvent(event.GetID());
		break;
	default:
		break;
	}
}
void CRelationSystem::OnMarriageWaitEvent(UInt32 state)
{
	if (m_pOwner == NULL || m_pMarriageData == NULL)
		return;
	CCharacter* pHusband = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetHusband_id());
	CCharacter* pWife = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetWife_id());
	if( pHusband == NULL || pWife == NULL )
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER,
			_FE("[husband or wife not found], husband_id %d, wife_id %d"),
			m_pMarriageData->GetHusband_id(), m_pMarriageData->GetWife_id());
		return;
	}

	switch(state)
	{
	case EVT_WAIT1:
		pHusband->SetMarriageState(CCharacter::MARRIAGE_INIT);
		pWife->SetMarriageState(CCharacter::MARRIAGE_INIT);
		pHusband->ShowMarriageEffect(BOW_WAIT);
		RaiseUniqEventID(EVT_WAIT2,MARRIAGE_WAIT_INTERVAL);
		break;
	case EVT_WAIT2:
		pHusband->ShowMarriageEffect(BOW_WAIT);
		RaiseUniqEventID(EVT_WAIT3,MARRIAGE_WAIT_INTERVAL);
		break;
	case EVT_WAIT3:
		pHusband->ShowMarriageEffect(BOW_WAIT);
		RaiseUniqEventID(EVT_WEDDING1,MARRIAGE_WAIT_INTERVAL);
		break;
	default:
		break;
	}
}
void CRelationSystem::OnMarriageEvent(UInt32 state)
{
	if (m_pOwner == NULL || m_pMarriageData == NULL)
		return;

	CCharacter* pHusband = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetHusband_id());
	CCharacter* pWife = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetWife_id());
	if( pHusband == NULL || pWife == NULL )
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER,
			_FE("[husband or wife not found], husband_id %d, wife_id %d"),
			m_pMarriageData->GetHusband_id(), m_pMarriageData->GetWife_id());
		return;
	}
	switch(state)
	{
	case EVT_WEDDING1:
		pHusband->SetMarriageState(CCharacter::MARRIAGE_HEAVEN_EARTH);
		pWife->SetMarriageState(CCharacter::MARRIAGE_HEAVEN_EARTH);
		pHusband->ShowMarriageEffect(BOW_HEAVEN_EARTH);
		RaiseUniqEventID(EVT_WEDDING2,MARRIAGE_BOW_INTERVAL);
		RaiseUniqEventID(EVT_WEDDING_STOP,MARRIAGE_BOW_STOP);
		break;
	case EVT_WEDDING2:
		pHusband->SetMarriageState(CCharacter::MARRIAGE_MASTER);
		pWife->SetMarriageState(CCharacter::MARRIAGE_MASTER);
		pHusband->ShowMarriageEffect(BOW_FOREFATHER);
		RaiseUniqEventID(EVT_WEDDING3,MARRIAGE_BOW_INTERVAL);
		RaiseUniqEventID(EVT_WEDDING_STOP,MARRIAGE_BOW_STOP);
		break;
	case EVT_WEDDING3:
		pHusband->SetMarriageState(CCharacter::MARRIAGE_FACETOFACE_BOW);
		pWife->SetMarriageState(CCharacter::MARRIAGE_FACETOFACE_BOW);
		pHusband->ShowMarriageEffect(BOW_COUPLE);
		RaiseUniqEventID(EVT_WEDDING4,MARRIAGE_BOW_INTERVAL);
		RaiseUniqEventID(EVT_WEDDING_STOP,MARRIAGE_BOW_STOP);
		break;
	case EVT_WEDDING4:
		{
			pHusband->SetMarriageState(CCharacter::MARRIAGE_FACETOFACE_STAND);
			pWife->SetMarriageState(CCharacter::MARRIAGE_FACETOFACE_STAND);
			pHusband->ShowMarriageEffect(BOW_FINISH);
			RaiseUniqEventID(EVT_RESET,MARRIAGE_BOW_INTERVAL);
			RaiseUniqEventID(EVT_WEDDING_STOP,MARRIAGE_BOW_STOP);

			String msg;
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_MARRIAGE_SUCCESS_RUMOR")),pHusband->GetNickName().c_str(),pWife->GetNickName().c_str(),m_allMarriageCount);
			MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, RPCContext(), msg, 0, 0);

			if(HasLuaFunc(gGlobal.g_pLuaState, "OnMarriageFinish"))
			{
				CallLuaFunc(gGlobal.g_pLuaState, "OnMarriageFinish", String(), pHusband,pWife);
			}
			m_bWedding = BOOLEAN_FALSE;
			break;
		}
	case EVT_RESET:
		pHusband->SetMarriageState(CCharacter::MARRIAGE_RESET);
		pWife->SetMarriageState(CCharacter::MARRIAGE_RESET);
		break;
	case EVT_WEDDING_STOP:
		{
			CMapNPC * pCharNpc = pHusband->GetCharNpc();
			if(pCharNpc != NULL)
				pHusband->SetCharNpcState(pCharNpc->GetDirection(),0);
			pCharNpc = pWife->GetCharNpc();
			if(pCharNpc != NULL)
				pWife->SetCharNpcState(pCharNpc->GetDirection(),0);
			break;
		}
	default:break;
	}
}

void CRelationSystem::CheckHusbandAndWifeDataValidate()
{
	if (m_pOwner == NULL)
		return;

	PROCEDURE_DO_METHOD(CRelationSystem, CheckHusbandAndWifeDataValidate, this, m_pOwner->GetContext())
}

RPCResult CRelationSystem::CheckHusbandAndWifeDataValidate(RPCContext &context)
{
	if (m_pOwner == NULL)
	{
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL);
	}

	PROCEDURE_START_METHOD(CRelationSystem, CheckHusbandAndWifeDataValidate, this, context)

	MessageService_Proxy::GetMessengers(gGlobal.pMessageService->pPeer,context,
		m_pOwner->GetChar_id(), FRIEND_RELATION_HUSBAND);

	PROCEDURE_WAIT2(1, MessageService, CB_GetMessengers, UInt32, owner_id, UInt32Vector, messagers)
	if(m_pMarriageData == NULL && messagers.size() > 0)
	{
		UInt32Vector::iterator iter = messagers.begin();
		UInt32Vector::iterator iterend = messagers.end();
		for(; iter != iterend; ++iter)
		{
			MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
				m_pOwner->GetContext(), m_pOwner->GetChar_id(), *iter,FRIEND_RELATION_HUSBAND);
			m_pOwner->RemoveTitle(gConfigure.wifeTitle);
			m_pOwner->RemoveTitle(gConfigure.husbandTitle);
		}
	}

	MessageService_Proxy::GetMessengers(gGlobal.pMessageService->pPeer,context,
		m_pOwner->GetChar_id(), FRIEND_RELATION_WIFE);

	PROCEDURE_WAIT2(2, MessageService, CB_GetMessengers, UInt32, owner_id, UInt32Vector, messagers)
	if(m_pMarriageData == NULL && messagers.size() > 0)
	{
		UInt32Vector::iterator iter = messagers.begin();
		UInt32Vector::iterator iterend = messagers.end();
		for(; iter != iterend; ++iter)
		{
			MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
				m_pOwner->GetContext(), m_pOwner->GetChar_id(), *iter,FRIEND_RELATION_WIFE);
			m_pOwner->RemoveTitle(gConfigure.wifeTitle);
			m_pOwner->RemoveTitle(gConfigure.husbandTitle);
		}
	}

	if(m_pMarriageData != NULL)
	{
		time_t marriage_time = m_pMarriageData->GetMarriage_time();
		struct tm* ptime = localtime(&marriage_time);
		ptime->tm_min += MARRIAGE_INTERVAL;
		time_t over_time = mktime(ptime);
		if(m_pMarriageData->GetState() == 0 && ::time(NULL) >= over_time)
		{
			ObjectService_Proxy::DeleteMarriage(gGlobal.pObjectService->pPeer,m_pOwner->GetContext(),m_pMarriageData->GetMarriage_id());
			bool bDelete = true;
			do 
			{
				UInt32 char_id =  m_pOwner->GetChar_id();
				if(m_pMarriageData->GetHusband_id() != char_id && gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetHusband_id())) 
				{
					bDelete = false;
					break;
				}
				if(m_pMarriageData->GetWife_id() != char_id && gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetWife_id())) 
				{
					bDelete = false;
					break;
				}
			} while(false);

			if(bDelete)
				DeleteNetObject(m_pMarriageData);
			SetMarriageData(NULL);
			PROCEDURE_RETURN;
		}
		else if(m_pMarriageData->GetState() == 1)
		{
			UInt16Vector relations;
			relations.push_back(FRIEND_RELATION_FRIEND);
			relations.push_back(FRIEND_RELATION_HUSBAND);
			MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
				m_pOwner->GetContext(), m_pMarriageData->GetWife_id(), m_pMarriageData->GetHusband_id(),
				relations, FRIEND_GROUP_ONE, 0, String());

			relations.clear();
			relations.push_back(FRIEND_RELATION_FRIEND);
			relations.push_back(FRIEND_RELATION_WIFE);
			MessageService_Proxy::CreateMessenger(gGlobal.pMessageService->pPeer, 
				m_pOwner->GetContext(), m_pMarriageData->GetHusband_id(), m_pMarriageData->GetWife_id(),
				relations, FRIEND_GROUP_ONE, 0, String());
		}
	}

	if(m_allMarriageCount != 0)
	{
		PROCEDURE_RETURN;
	}
	ObjectService_Proxy::GetMarriageCount(gGlobal.pObjectService->pPeer,context);
	PROCEDURE_WAIT1(3, ObjectService, CB_GetMarriageCount, UInt32, count)
	m_allMarriageCount = count;

	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, 
		_T("CheckHusbandAndWifeDataValidate: GetMarriageCount:%d"),count);

	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, 
		_T("CheckHusbandAndWifeDataValidate: char_id %d, sessionId %s"),
		m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str());

	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION_DEFAULT;
	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}
//RPCResult CRelationSystem::SendMarriageRumor(RPCContext &context)
//{
//	if (m_pOwner == NULL || m_pMarriageData == NULL)
//	{
//		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL);
//	}
//	CCharacter* pHusband = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetHusband_id());
//	CCharacter* pWife = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetWife_id());
//	if( pHusband == NULL || pWife == NULL )
//	{
//		TRACE_ERRORDTL_2(GLOBAL_LOGGER,
//			_FE("[husband or wife not found], husband_id %d, wife_id %d"),
//			m_pMarriageData->GetHusband_id(), m_pMarriageData->GetWife_id());
//		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL);
//	}
//
//	PROCEDURE_START_METHOD(CRelationSystem, SendMarriageRumor, this, context)
//
//	ObjectService_Proxy::GetMarriageCount(gGlobal.pObjectService->pPeer,context);
//	PROCEDURE_WAIT1(1, ObjectService, CB_GetMarriageCount, UInt32, count)
//	if(count != 0 && count > m_allMarriageCount)
//		m_allMarriageCount = count;
//	String msg;
//	msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_MARRIAGE_SUCCESS_RUMOR")),pHusband->GetNickName().c_str(),pWife->GetNickName().c_str(),m_allMarriageCount);
//	MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, RPCContext(), msg, 0, 1);
//
//	PROCEDURE_CATCH
//		PROCEDURE_EXCEPTION_DEFAULT;
//	PROCEDURE_END_CATCH
//	return RPC_RESULT_OK;
//}
UInt32 CRelationSystem::GetHusbandId()
{
	if (m_pMarriageData != NULL && m_pMarriageData->GetState() == 1)
		return m_pMarriageData->GetHusband_id();
	return 0;
}

UInt32 CRelationSystem::GetWifeId()
{
	if (m_pMarriageData != NULL && m_pMarriageData->GetState() == 1)
		return m_pMarriageData->GetWife_id();
	return 0;
}

UInt32 CRelationSystem::GetMarriageId()
{
	if (m_pMarriageData != NULL && m_pMarriageData->GetState() == 1)
		return m_pMarriageData->GetMarriage_id();
	return 0;
}

RPCResult CRelationSystem::SelectMarriageTime(LPCPEER pPeer, RPCContext &context, const UInt32 &hour,const UInt32 &minute)
{
	time_t now = ::time(NULL);
	struct tm* ptime = localtime(&now);
	ptime->tm_hour = hour;
	ptime->tm_min = minute;
	ptime->tm_sec = 0;
	time_t time = mktime(ptime);
	TRACE_INFODTL_1(GLOBAL_LOGGER,_F("time:%d"),time);

	if (m_pOwner == NULL)
	{
		return RPC_RESULT_OK;
	}
	if(!FindMarriageTime((time_t)time))
	{
		return RPC_RESULT_OK;
	}
	if(std::find(gGlobal.g_MarriageTimeLst.begin(),gGlobal.g_MarriageTimeLst.end(),time)!= gGlobal.g_MarriageTimeLst.end())
	{
		m_pOwner->CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), 1, _T("MSG_MARRIAGE_CREATE_FAILED_EXIST"));
		return RPC_RESULT_OK;
	}
	gGlobal.g_MarriageTimeLst.push_back(time);
	CreateMarriage(m_tmpHusbandId,m_tmpWifeId,time);
	return RPC_RESULT_OK;
}

Boolean CRelationSystem::FindMarriageTime(time_t marriagetime)
{
	struct tm* ptime = localtime(&marriagetime);
	for (UInt32 i = 0 ; i<gConfigure.uMarriageTimeCount; i++)
	{
		if (ptime->tm_hour == gConfigure.pMarriageTime[i].x && ptime->tm_min == gConfigure.pMarriageTime[i].y)
			return true;
	}
	return false;
}

UInt32 CRelationSystem::GetRealMarriageTime()
{
	if(m_pMarriageData != NULL)
	{
		time_t marriageTime =  (time_t)m_pMarriageData->GetMarriage_time();
		if (marriageTime == 0)
			return 0;
		struct tm* ptime = localtime(&marriageTime);
		return ptime->tm_min+ptime->tm_hour*60;
	}
	return 0;
}
void CRelationSystem::OnCompanionExit(UInt32 char_id)
{
	if (m_pOwner == NULL || m_pMarriageData == NULL)
	{
		return;
	}

	CCharacter* pHusband = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetHusband_id());
	CCharacter* pWife = gGlobal.m_charToSession.FindCharacter(m_pMarriageData->GetWife_id());
	if( pHusband == NULL || pWife == NULL )
	{
		return;
	}

	if(m_bWedding)
	{
		CancelAllEvent();
		if(HasLuaFunc(gGlobal.g_pLuaState, "OnMarriageFinish"))
		{
			CallLuaFunc(gGlobal.g_pLuaState, "OnMarriageFinish", String(), pHusband,pWife);
		}
		m_bWedding = BOOLEAN_FALSE;
	}
}

bool CRelationSystem::hasSiblingData()
{
	m_pSiblingData = FindNetObject<CSiblingData>(m_siblingDataNetID);
	if (m_pSiblingData)
		return true;
	return false;
}