//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Logic/Account.h"
//-- Library
#include "Global.h"
#include "Stub/Logic/CharacterInfo.h"
#include "Stub/Service/WorldService.h"
#include "Proxy/Service/LogService.h"
#include "Proxy/Service/ObjectService.h"
#include "Proxy/Service/MessageService.h"
#include "Proxy/Data/AccountData.h"
#include "Proxy/Data/CharacterData.h"
#include "Proxy/Data/ItemData.h"
#include "Proxy/Service/MapService_proxy.h"
#include "WorldService/Global.h"
#include "WorldService/Configure.h"
#include "WorldService/WorldApplication.h"
#include "WorldService/AntiGameAddiction.h"
#include "Common/SettingTable.h"
#include "Resource/ResCharacter.h"
#include "Resource/ResBlockWord.h"
#include "Resource/ServerResManager.h"
#include <algorithm>

//#include "Stub/Logic/ChargeShop.h"

REGISTER_CLASSTYPE(CAccount, Account_CLASSID);

CAccount::CAccount()
{
	//m_pCharInfo = NULL;
	m_bConnect = BOOLEAN_FALSE;
	m_lastSessId.serviceId = INVALID_SERVICE_ID;
	m_waitSessId.serviceId = INVALID_SERVICE_ID;

}

CAccount::~CAccount()
{
	DeleteNetChilds(&m_characters);
}

void CAccount::SetSessionId(const SessionID& sessId)
{
	if (m_lastSessId.serviceId != INVALID_SERVICE_ID && m_lastSessId != sessId) {
		//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Remove Old SessionId: sessionId %s, old sessionId %s"), 
		//	sessId.GetString().c_str(), m_lastSessId.GetString().c_str());
		//RemoveSession(m_lastSessId);
		//ProcedureManager::GetInstance().RemoveBySession(m_lastSessId);
		//if (m_pCharInfo) {
		//	LPCPEER pMapPeer = NULL;
		//	RPCContext context(sessId);
		//
		//	pMapPeer = gGlobal.GetMapInfo(MapIndex(m_pCharInfo->GetMap_id(),
		//								m_pCharInfo->GetLine_id())).pPeer;
		//	if (pMapPeer)
		//		MapService_Proxy::ReleaseSession(pMapPeer, context, m_lastSessId, BOOLEAN_FALSE);
		//}
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ChangeSessionId: sessionId %s, old sessionId %s"), 
			sessId.GetString().c_str(), m_lastSessId.GetString().c_str());

		ChangeSession(m_lastSessId, sessId);
		ProcedureManager::GetInstance().ChangeSessionId(m_lastSessId, sessId);
		Session* psess = GetSession(sessId);
		if (psess)
			psess->SetSessionId(sessId);
	}
	m_lastSessId = sessId;
}

void CAccount::ClearSessionId()
{
	m_lastSessId.serviceId = 0;
}

void CAccount::ClearCharacters()
{
	DeleteNetChilds(&m_characters);
}

void CAccount::Connect(const SessionID& sid)
{
	if (!m_bConnect)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Account Connect: acct_id %d, sessionId %s"), 
			GetAcct_id(), sid.GetString().c_str());
		m_bConnect = BOOLEAN_TRUE;
	}
}

void CAccount::Disconnect(const SessionID& sid)
{
	if (m_bConnect)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Account Disconnect: acct_id %d, sessionId %s"), 
			GetAcct_id(), sid.GetString().c_str());
		SetDisconnect_time((UInt32) ::time(NULL));
		m_bConnect = BOOLEAN_FALSE;
		CCharacterInfo* pCharInfo = GetLoginChar();
		if (pCharInfo)
			pCharInfo->Disconnect(sid);
	}
}

RPCResult CAccount::Logout(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Account Logout: acct_id %d, sessionId %s"), 
		GetAcct_id(), context.sessionId.GetString().c_str());

	// same as release session
	CB_LogoutSuccess(pPeer, context);
	WorldService_Stub::ReleaseSession(pPeer, context, context.sessionId,
		BOOLEAN_FALSE);

	return RPC_RESULT_OK;
}

RPCResult CAccount::GetOwnCharacters(LPCPEER pPeer, RPCContext &context)
{
	PROCEDURE_START_RPC(Account, GetOwnCharacters, this)

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GetOwnCharacters: session %s, acct_id %d"),
			context.sessionId.GetString().c_str(), GetAcct_id());

		ObjectService_Proxy::GetCharacters(gGlobal.pObjectService->pPeer, 
			context, GetAcct_id(), 0, CCharacterData::DETAIL_INFO);

	PROCEDURE_WAIT2(1, ObjectService, CB_GetCharacterGroup, NetGroup*, charGroup, UInt8, detail)

		RPCMask mask;

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GetOwnCharacters: sessionId %s, count %d"),
			context.sessionId.GetString().c_str(), charGroup->GetCount());
		mask.MaskBit(CCharacterInfo::AllMask);

		for (Index i = C_INDEX(0); i < charGroup->GetCount(); ++i)
		{
			CCharacterData* pcharData = charGroup->GetChild<CCharacterData>(i);
			TRACE_ENSURE(pcharData);
			CCharacterInfo* pchar = NULL;
			for (Index j = C_INDEX(0); j < m_characters.GetCount(); ++j)
			{
				CCharacterInfo* oldchar = m_characters.GetChild<CCharacterInfo>(j);
				if (oldchar && oldchar->GetChar_id() == pcharData->GetChar_id())
				{
					pchar = oldchar;
					break;
				}
			}
			if (pchar == NULL)
			{
				pchar = CCharacterInfo::NewInstance();
				m_characters.Add(pchar);
			}
			pchar->SetData(pcharData);
			pchar->SetSendMask(mask);
			pchar->ValidateMap();			// validate map available
		}
		// remove data object group after used
		DeleteNetGroup(charGroup, BOOLEAN_TRUE);

		// send new characters group back to client
		CB_GetOwnCharacters(pPeer, context, &m_characters);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("GetOwnCharacters Error: ObjectService:CB_Error, errcode %d, %s"), 
				err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CAccount::CheckCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	UInt8 charStatus = CCharacterInfo::ST_LEAVE; 

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CheckCharacter: sessionId %s, char_id %d"), 
		context.sessionId.GetString().c_str(), char_id);

	Session *psess = GetSession(context.sessionId);
	if (psess == NULL)
	{
		CB_Error(pPeer, context, 1, _T("CheckCharacter: invalid session\n"));
		return RPC_RESULT_FAIL;
	}

	CCharacterInfo *pEnterChar = GetLoginChar();
	CCharacterInfo *pCharInfo = GetCharInfo(char_id);;
	if (pCharInfo == NULL)
	{
		// character not found
		charStatus = CCharacterInfo::ST_NOT_FOUND;
	}
	else
	{
		pCharInfo->ValidateMap();		// validate map available

		// character exist
		if (pEnterChar && pCharInfo != pEnterChar)
		{
			charStatus = CCharacterInfo::ST_NOT_AVAIL;			// cannot select other
		}
		else
		{
			charStatus = pCharInfo->GetStatus();				// original character status
		}
	}

	// return map line count when character leave
	Int32Vector countList;
	UInt16Vector lineState;
	if (charStatus == CCharacterInfo::ST_LEAVE)
	{
		MapIndex mi(pCharInfo->GetMap_id(), 0, pCharInfo->GetMap_ownerId());
		LPCPEER pMapPeer;
		for (UInt8 i = 0; i < (UInt8) gGlobal.m_lineCounts.size(); ++i)
		{
			mi.line_id = i;
			pMapPeer = gGlobal.GetMapInfo(mi).pPeer;
			if (pMapPeer == NULL && mi.owner_id == 0)
			{
				countList.push_back(-1);
				lineState.push_back(LINE_STATE_REPAIR);
			}
			else
			{
				countList.push_back(gGlobal.m_lineCounts[i]);

				if (gGlobal.m_lineCounts[i] < gConfigure.normalLineState)
					lineState.push_back(LINE_STATE_NORMAL);
				else if (gGlobal.m_lineCounts[i] < gConfigure.busyLineState)
					lineState.push_back(LINE_STATE_BUSY);
				else
					lineState.push_back(LINE_STATE_SUPERFULL);
			}	
		}
	}

	CB_CheckCharacter(pPeer, context, charStatus, countList, lineState);

	return RPC_RESULT_OK;
}

RPCResult CAccount::SelectCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id,
									const UInt8 &line_id)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC2(Account, SelectCharacter, this, UInt32, char_id, UInt8, line_id)

		if (line_id >= gGlobal.m_lineCounts.size())
		{
			CB_Error(pPeer, context, 2, _T("SelectCharacter Error: line not available"));
			TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("[line not available] char_id %d, SessionID %s, line_id %d, total %d"), char_id, context.sessionId.GetString().c_str(), line_id, (int)gGlobal.m_lineCounts.size());
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		else if (gGlobal.m_lineCounts[line_id] == -1)
		{
			CB_Error(pPeer, context, 2, _T("SelectCharacter Error: line not available"));
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("SelectCharacter Error: [line not available] char_id %d, SessionID %s, line_id %d"), char_id, context.sessionId.GetString().c_str(), line_id);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		else if (gGlobal.m_lineCounts[line_id] >= gConfigure.busyLineState)
		{
			CB_Error(pPeer, context, 2, _T("SelectCharacter Error: too many people in line"));
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("SelectCharacter Error: [too many people in line] char_id %d, SessionID %s, line_id %d"), char_id, context.sessionId.GetString().c_str(), line_id);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("SelectCharacter: sessionId %s, last sessionId %s, char_id %d, line_id %d"), 
			context.sessionId.GetString().c_str(), m_lastSessId.GetString().c_str(), 
			char_id, line_id);

		Session *psess = GetSession(context.sessionId);
		if (psess == NULL)
		{
			CB_Error(pPeer, context, 1, _T("SelectCharacter: invalid session"));
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, 
				_T("SelectCharacter Error: [invalid session], sessionId %s"), 
				context.sessionId.GetString().c_str());
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		CCharacterInfo *pCharInfo = GetCharInfo(char_id);
		if (pCharInfo == NULL)
		{
			CB_Error(pPeer, context, 1, _T("SelectCharacter Error(1): char_id not found"));
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_T("SelectCharacter Error(1): [char_id not found], sessionId %s, char_id %d"), 
				context.sessionId.GetString().c_str(), char_id);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CCharacterInfo* pLoginChar = GetLoginChar();
		if (pLoginChar && pCharInfo != pLoginChar)
		{
			CB_Error(pPeer, context, 1, _T("SelectCharacter Error: [character not available]"));
			TRACE_INFODTL_3(GLOBAL_LOGGER, 
				_T("SelectCharacter: [character not available], sessionId %s, char_id %d, login char_id %d"), 
				context.sessionId.GetString().c_str(), char_id, pLoginChar->GetChar_id());
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		// if pCharInfo is left then just go to step 1 for process character enter
		if (pCharInfo->IsLeave())
		{
			TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("SelectCharacter: (Go Step 1) sessionId %s"), context.sessionId.GetString().c_str());
			PROCEDURE_GO_STEP1(1, Boolean, BOOLEAN_FALSE)
		}

		LPCPEER pMapPeer = gGlobal.GetMapInfo(MapIndex(
							pCharInfo->GetMap_id(), pCharInfo->GetLine_id(), 
							pCharInfo->GetMap_ownerId())).pPeer;
		if (pMapPeer == NULL)
		{
			PROCEDURE_GO_STEP1(1, Boolean, BOOLEAN_FALSE)
		}
		else
		{
			MapService_Proxy::ChangeSessionId(pMapPeer, context, m_selSessId);
		}

		MessageService_Proxy::ResumeMessageChannel(gGlobal.pMessageService->pPeer, context, 
			char_id);
		MapService_Proxy::ResumeSession(pMapPeer, context);

		// set 1 min to timeout for character enter
		PROCEDURE_SET_TIMEOUT(60000, BOOLEAN_FALSE)

		//RemoveSessionId(m_lastSessId);
		//m_lastSessId = context.sessionId;

	PROCEDURE_WAIT1(1, MapService, CB_ResumeSession, Boolean, valid)

		TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, 
			_T("SelectCharacter: (CB_ResumeSession) sessionId %s, valid %d"), 
			context.sessionId.GetString().c_str(), valid);
		
		CCharacterInfo *pCharInfo = GetCharInfo(char_id);
		if (pCharInfo == NULL)
		{
			CB_Error(pPeer, context, 1, _T("SelectCharacter Error(2): char_id not found"));
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_T("SelectCharacter Error(2): [char_id not found] char_id %d, sessionId %s"), 
				char_id, context.sessionId.GetString().c_str());
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		if (valid)
		{
			// resume character
			TRACE_INFODTL_2(GLOBAL_LOGGER, 
				_T("SelectCharacter: [resume ok], sessionId %s, old session Id %s"), 
				context.sessionId.GetString().c_str(), m_lastSessId.GetString().c_str());
			m_selSessId = context.sessionId;
			if (m_bConnect)
				pCharInfo->Connect(context.sessionId);
			else
				pCharInfo->Disconnect(context.sessionId);
			CB_SelectCharacter(pPeer, context);		// callback to client
			PROCEDURE_RETURN
		}

		// select character normally
		UInt8 rline_id = (pCharInfo->GetMap_ownerId() == 0 ? line_id : 0);	// real line id
		const MapInfo &mInfo = gGlobal.GetMapInfo(MapIndex(pCharInfo->GetMap_id(), rline_id,
			pCharInfo->GetMap_ownerId()));
		LPCPEER pMapPeer;
		UInt16 map_id, x, y;
		UInt32 map_ownerId;
		if (mInfo.pPeer && mInfo.bOpen) {
			pMapPeer = mInfo.pPeer;
			map_id = pCharInfo->GetMap_id();
			x = pCharInfo->GetMap_x();
			y = pCharInfo->GetMap_y();
			map_ownerId = pCharInfo->GetMap_ownerId();
		} else {
			TRACE_WARNDTL_6(GLOBAL_LOGGER, _FW("[no map peer or closed, try reborn], sessionId %s, map_id %d, line_id %d, owner_id %d, pPeer %d, bOpen %d"), 
				context.sessionId.GetString().c_str(), pCharInfo->GetMap_id(), rline_id, 
				pCharInfo->GetMap_ownerId(), mInfo.pPeer, mInfo.bOpen);
			rline_id = line_id;
			map_ownerId = 0;
			const MapInfo &mInfo2 = gGlobal.GetMapInfo(MapIndex(pCharInfo->GetReborn_map(),
														line_id, 0));
			if (mInfo2.pPeer) {
				pMapPeer = mInfo2.pPeer;
				map_id = pCharInfo->GetReborn_map();
				x = pCharInfo->GetReborn_x();
				y = pCharInfo->GetReborn_y();
			} else {
				TRACE_ERRORDTL_4(GLOBAL_LOGGER, 
					_T("SelectCharacter Error(2): [reborn map no peer], sessionId %s, map_id %d, line_id %d, pPeer %d"), 
					context.sessionId.GetString().c_str(), pCharInfo->GetReborn_map(), rline_id, 
					mInfo2.pPeer);
				CB_Error(pPeer, context, 1, _T("SelectCharacter Error: no reborn map peer"));
				PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
			}
		}

		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, 
			_T("SelectCharacter: [normal select], sessionId %s, char_id %d, map_id %d, line_id %d"), 
			context.sessionId.GetString().c_str(), char_id, pCharInfo->GetMap_id(), rline_id);
		pCharInfo->SetMap(map_id, rline_id, map_ownerId, x, y);

		EnterInfo enterInfo;
		enterInfo.acct_id = GetAcct_id();
		enterInfo.charNetId = pCharInfo->GetNetId();
		enterInfo.char_id = pCharInfo->GetChar_id();
		enterInfo.map_id = map_id;
		enterInfo.line_id = rline_id;
		enterInfo.ownerId = map_ownerId;
		enterInfo.x = x;
		enterInfo.y = y;

		UInt32Vector dummyList;
		AntiAddictInfo addictData;	
		addictData.onlineTime = GetOnLnTime();
		addictData.offlineTime = GetOffLnTime();
		addictData.disconnectTime = GetDisconnect_time();
		addictData.loginTime = 0;
		addictData.startHack = BOOLEAN_FALSE;
		addictData.stopPlay = BOOLEAN_FALSE;
		addictData.needAntiAddict = GetAddictType();

		// join global channels
		MessageService_Proxy::InitMessageChannel(gGlobal.pMessageService->pPeer, context, 
			char_id);
		ObjectService_Proxy::KeepCharacter(gGlobal.pObjectService->pPeer, context, 
			pCharInfo->GetAcct_id(), pCharInfo->GetChar_id());
		gGlobal.SetCharacterEnterLock(pCharInfo->GetChar_id());
		MapService_Proxy::CharacterEnter(pMapPeer, context, enterInfo, dummyList, 
			addictData, String(),13,0);

		// set 1 min to timeout for character enter
		PROCEDURE_SET_TIMEOUT(60000, BOOLEAN_FALSE)

	PROCEDURE_WAIT(2, MapService, CB_CharacterEnter)

		gGlobal.RemoveCharacterEnterLock(char_id);
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SelectCharacter: [CB_CharacterEnter success], sessionId %s, char_id %d"), context.sessionId.GetString().c_str(), char_id);
		CCharacterInfo *pCharInfo = GetCharInfo(char_id);
		if (pCharInfo == NULL)
		{
			CB_Error(pPeer, context, 1, _T("SelectCharacter Error(3): char_id not found"));
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_T("SelectCharacter Error(3): [char_id not found] char_id %d, sessionId %s"), 
				char_id, context.sessionId.GetString().c_str());
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		m_selSessId = context.sessionId;
		if (m_bConnect)
			pCharInfo->Connect(context.sessionId);
		else
			pCharInfo->Disconnect(context.sessionId);
		CB_SelectCharacter(pPeer, context);		// callback to client

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			gGlobal.RemoveCharacterEnterLock(char_id);
			TRACE_ERRORDTL_5(GLOBAL_LOGGER, 
				_T("SelectCharacter Error: (ObjectService:CB_Error), sessiondId %s, acct_id %d, char_id %d, errcode %d, %s"), 
				context.sessionId.GetString().c_str(), GetAcct_id(), char_id, err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

		PROCEDURE_EXCEPTION2(MapService, CB_Error, UInt32, err, String, errMsg)

			gGlobal.RemoveCharacterEnterLock(char_id);
			TRACE_ERRORDTL_5(GLOBAL_LOGGER, 
				_T("SelectCharacter Error: (MapService:CB_Error), sessiondId %s, acct_id %d, char_id %d, errcode %d, %s"), 
				context.sessionId.GetString().c_str(), GetAcct_id(), char_id, err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

		PROCEDURE_EXCEPTION_TIMEOUT

			gGlobal.RemoveCharacterEnterLock(char_id);
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_T("SelectCharacter Error: [procedure timeout], sessionId %s, acct_id %d, char_id %d"), 
				context.sessionId.GetString().c_str(), GetAcct_id(), char_id);

		PROCEDURE_EXCEPTION_DEFAULT

			gGlobal.RemoveCharacterEnterLock(char_id);
			WorldService_Stub::CB_ForceLogout(pPeer, context, _T("MSG_VERIFY_FAILED"));

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CAccount::AddCharacter(LPCPEER pPeer, RPCContext &context, const String &nickName, const UInt8 &sex, const UInt8 &cclass,  const UInt16 &hairStyle, const UInt16 &hairColor, const UInt16 &clothStyle, const UInt16 &clothColor)
{

	PROCEDURE_START_RPC7(Account, AddCharacter, this, String, nickName, UInt8, sex, UInt8, cclass, UInt16, hairStyle, UInt16, hairColor, UInt16, clothStyle, UInt16, clothColor)

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CreateCharacter: sessionId %s, last sessionId %s, acct_id %d"), 
			context.sessionId.GetString().c_str(), m_lastSessId.GetString().c_str(), GetAcct_id());

		if (m_characters.GetCount() >= 3)
		{
			CB_Error(pPeer, context, 0, _T("CreateCharacter: it is the limit of the character number \n"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		if (nickName.size() > 6)
		{
			CB_Error(pPeer, context, 1, _T("This Name cannot be used"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		if ( (hairStyle==0 || hairStyle > gConfigure.limitHead) ||
			(hairColor==0 || hairColor > gConfigure.limitHeadColor) ||
			(clothStyle==0 || clothStyle > gConfigure.limitCloth) ||
			(clothColor==0 || clothColor > gConfigure.limitClothColor)
			)
		{
			CB_Error(pPeer, context, 2, _T("Char Data Error"));
			TRACE_WARNDTL_6(GLOBAL_LOGGER,
				_FW("invalid CharData maybe hack, hairStyle %d, hairColor %d, clothStyle %d, clothColor %d, acct_id %d, sess_id %s"),
				hairStyle, hairColor, clothStyle, clothColor, GetAcct_id(), context.sessionId.GetString().c_str()
				);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		if (!gGlobal.CanUsedName(nickName))
		{
			CB_Error(pPeer, context, 2, _T("This Name cannot be used"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		const StartCharData* charData = gGlobal.m_resource->GetStartCharData(1 + (cclass - 1) * 2 + sex);	
		if (!charData)
		{
			CB_Error(pPeer, context, 2, _T("Char Data Error"));
			TRACE_WARNDTL_4(GLOBAL_LOGGER,
				_FW("invalid CharData maybe hack, cclass %d, sex %d, acct_id %d, sess_id %s"),
				cclass, sex, GetAcct_id(), context.sessionId.GetString().c_str()
				);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}
		
		//this formula need to be changed
		float hp = charData->MaxHP;
		float sp = charData->MaxSP;

		CCharacterData newCharData;
		newCharData.SetAttrib(charData->STA, charData->SPR, charData->STR, charData->CON, charData->AGI);
		newCharData.SetHP((UInt16)hp, (UInt16)hp, (UInt16)hp);
		newCharData.SetSP((UInt16)sp, (UInt16)sp, (UInt16)sp);
		newCharData.SetLevel(0);

		newCharData.SetVP(0, charData->MaxVP);
		newCharData.SetDP(150, charData->MaxDP, charData->MaxDP);
		newCharData.SetWC(0, charData->MaxWC, 0);

		newCharData.SetMoney(0);
		newCharData.SetBankMoney(0);

		newCharData.SetMaxOther(3, 1);
		newCharData.SetOrgEqCloth(clothStyle, clothColor);
		newCharData.SetOrgHairStyle(hairStyle, hairColor);

		UInt16 map_id = GetPrivateProfileInt(_T("CreateCharacter"), _T("InitMapID"), 20102, CONFIGURATION_INI_FILE);
		UInt16 map_x = GetPrivateProfileInt(_T("CreateCharacter"), _T("InitMapX"), 21, CONFIGURATION_INI_FILE);
		UInt16 map_y = GetPrivateProfileInt(_T("CreateCharacter"), _T("InitMapY"), 86, CONFIGURATION_INI_FILE);

		newCharData.SetMap(charData->StartMap, charData->StartX, charData->StartY, 0);
		newCharData.SetReborn(map_id, map_x, map_y);
		newCharData.SetSendMask(newCharData.GetUpdateMask());

		ObjectService_Proxy::CreateCharacter(gGlobal.pObjectService->pPeer, context, GetAcct_id(), nickName, sex, cclass, &newCharData, CCharacterData::DETAIL_MAP);
	
	PROCEDURE_WAIT1(1, ObjectService, CB_CreateCharacterSuccess, UInt32, char_id)

		const StartCharData* charData = gGlobal.m_resource->GetStartCharData(1 + (cclass - 1) * 2 + sex);

		UInt32Vector itemList;
		UInt8Vector itemBagIndex;
		
		if (charData->Weapon1 > 0)
			itemList.push_back(charData->Weapon1);
		
		if (charData->Weapon2 > 0)
			itemList.push_back(charData->Weapon2);
		
		if (charData->Cloth > 0)
			itemList.push_back(charData->Cloth);
		
		if (charData->HeadAs > 0)
			itemList.push_back(charData->HeadAs);
			
		if (charData->Foot > 0)
			itemList.push_back(charData->Foot);
		
		if (charData->A1 > 0)
			itemList.push_back(charData->A1);

		if (charData->A2 > 0)
			itemList.push_back(charData->A2);

		if (gGlobal.pLogService)
			LogService_Proxy::LogCharacter(gGlobal.pLogService->pPeer, context, GetAcct_id(), char_id, 0, LogService_Proxy::Character::ACT_CREATE);

		if (itemList.size() > 0)
		{
			for (UInt8 i = 0; i < itemList.size(); ++i)
				itemBagIndex.push_back(i);
			ObjectService_Proxy::CreateItemList(gGlobal.pObjectService->pPeer, context, itemList, char_id, itemBagIndex, CItemData::DETAIL_NONE);
		}
		else
		{
			PROCEDURE_GO_STEP(2)
		}
	
	PROCEDURE_WAIT(2, ObjectService, CB_CreateItemDone)

		CB_CreateSuccess(pPeer, context);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("AddCharacter Error: ObjectService:CB_Error, errcode %d, %s"), 
				err, errMsg.c_str());
			if( !errMsg.CompareNoCase(_T("ObjectService_Stub::CreateCharacter Warning : a player already used the name")) )
				CB_Error(pPeer, context, 3, errMsg);	// CHECKNAME_HAS_CHARACTER
			else
				CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH
	
	return RPC_RESULT_OK;
}

RPCResult CAccount::FreezeCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	PROCEDURE_START_RPC1(Account, FreezeCharacter, this, UInt32, char_id)

		Session *psess = GetSession(context.sessionId);
		if (psess == NULL)
		{
			CB_Error(pPeer, context, 1, _T("FreezeCharacter: invalid session\n"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		CCharacterInfo *pCharInfo = NULL;
		for (UInt16 i = 0; i < m_characters.GetCount(); ++i)
		{
			 CCharacterInfo* child = m_characters.GetChild<CCharacterInfo >(i);
			 if (child && child->GetChar_id() == char_id)
			 {
				pCharInfo = child;
				break;
			}
		}
		if (pCharInfo == NULL)
		{
			CB_Error(pPeer, context, 1, _T("FreezeCharacter Error: char_id not found"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		
		UInt16 level = pCharInfo->GetLevel();
		UInt32 extendPeriod = 0;
		if ((level >= 0) && (level < 30))
			extendPeriod = gConfigure.day;
		else if ((level >= 30) && (level < 40))
			extendPeriod = 3 * gConfigure.day;
		else if ((level >= 40) && (level < 50))
			extendPeriod = 7 * gConfigure.day;
		else if ((level >= 50) && (level < 60))
			extendPeriod = 30 * gConfigure.day;

		ObjectService_Proxy::SetCharacterExpiryDate(gGlobal.pObjectService->pPeer, context, char_id, extendPeriod);

	PROCEDURE_WAIT1(1, ObjectService, CB_SetCharExpiryDateSuccess, UInt32, deletetime)
		
		CCharacterInfo *pchar = NULL;
		for (int i = 0; i < m_characters.GetCount(); ++i)
		{
			pchar = m_characters.GetChild<CCharacterInfo >(i);
			if (pchar && (pchar->GetChar_id() == char_id))
			{
				pchar->SetDelete_date(deletetime);
				break;
			}
		}
		RPCMask mask;
		//mask.MaskBits(CCharacterInfo::idMask, pchar->GetUpdateMask(), -1);
		mask.MaskBits(AllMask, -1);
		if (pchar)
            pchar->SetSendMask(mask);

        CB_FreezeCharSuccess(pPeer, context, pchar);

	PROCEDURE_CATCH
	
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
		
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("FreezeCharacter Error: ObjectService:CB_Error, errcode %d, %s"), 
					err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;

}

RPCResult CAccount::RecoverCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	PROCEDURE_START_RPC1(Account, RecoverCharacter, this, UInt32, char_id)

		Session *psess = GetSession(context.sessionId);
		if (psess == NULL)
		{
			CB_Error(pPeer, context, 1, _T("RecoverCharacter: invalid session\n"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		CCharacterInfo *pCharInfo = NULL;
		for (UInt16 i = 0; i < m_characters.GetCount(); ++i)
		{
			 CCharacterInfo* child = m_characters.GetChild<CCharacterInfo >(i);
			 if (child && child->GetChar_id() == char_id)
			 {
				pCharInfo = child;
				break;
			}
		}
		if (pCharInfo == NULL)
		{
			CB_Error(pPeer, context, 1, _T("RecoverCharacter Error: char_id not found"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		ObjectService_Proxy::RestoreCharacter(gGlobal.pObjectService->pPeer, context, char_id);

	PROCEDURE_WAIT(1, ObjectService, CB_RestoreCharacterSuccess)

		CCharacterInfo *pchar = NULL;
		for (int i = 0; i < m_characters.GetCount(); ++i)
		{
			pchar = m_characters.GetChild<CCharacterInfo >(i);
			if (pchar && (pchar->GetChar_id() == char_id))
			{
				pchar->SetDelete_date(0);
				break;
			}
		}
		RPCMask mask;
		//mask.MaskBits(CCharacterInfo::idMask, pchar->GetUpdateMask(), -1);
		mask.MaskBits(AllMask, -1);
		if (pchar)
            pchar->SetSendMask(mask);

		CB_RecoverCharSuccess(pPeer, context, pchar);	

	PROCEDURE_CATCH
	
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
		
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("RestoreCharacter Error: ObjectService:CB_Error, errcode %d, %s"), 
					err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;	

}


RPCResult CAccount::RemoveCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	
	PROCEDURE_START_RPC1(Account, RemoveCharacter, this, UInt32, char_id)

		Session *psess = GetSession(context.sessionId);
		if (psess == NULL)
		{
			CB_Error(pPeer, context, 1, _T("RemoveCharacter: invalid session"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

		CCharacterInfo *pCharInfo = GetCharInfo(char_id);
		CCharacterInfo *pLoginChar = GetLoginChar();
		if (pCharInfo == NULL)
		{
			CB_Error(pPeer, context, 1, _T("RemoveCharacter Error: char_id not found"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		else if (pLoginChar && pCharInfo == pLoginChar)
		{
			CB_Error(pPeer, context, 1, _T("RemoveCharacter Error: cannot remove logined character"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		
		MessageService_Proxy::RemoveMessengersByOwner(gGlobal.pMessageService->pPeer, context, char_id);

	PROCEDURE_WAIT(1, MessageService, CB_RemoveMessengersByOwner)
		
		ObjectService_Proxy::DeleteCharacter(gGlobal.pObjectService->pPeer, context, char_id, GetAcct_id());

	PROCEDURE_WAIT(2, ObjectService, CB_DeleteCharacterSuccess)

		CCharacterInfo *pchar = GetCharInfo(char_id);
		if (pchar)
			m_characters.Remove(pchar);

		CB_RemoveCharSuccess(pPeer, context);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("DeleteCharacter Error: ObjectService:CB_Error, errcode %d, %s"), 
				err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

		PROCEDURE_EXCEPTION2(MessageService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("DeleteCharacter Error: (MessageService:CB_Error) errcode %d, %s"), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH


	return RPC_RESULT_OK;
}

RPCResult CAccount::CheckCharacterName(LPCPEER pPeer, RPCContext &context, const String &nickName)
{
	PROCEDURE_START_RPC1(Account, CheckCharacterName, this, String, nickName)

		UInt8 msgIndex = CHECKNAME_OK;
		if (nickName.length() > 6 )
		{
			msgIndex = CHECKNAME_TOO_LONG;
			CB_CheckCharacterName(pPeer, context, msgIndex);
			PROCEDURE_RETURN
		}

		if (gGlobal.CanUsedName(nickName))
		{
			ObjectService_Proxy::HasCharacter(gGlobal.pObjectService->pPeer, context, nickName);
		}
		else
		{
			msgIndex = CHECKNAME_CANT_USE;
			PROCEDURE_STORE_VALUE1(UInt8, msgIndex)
			PROCEDURE_GO_STEP1(1, BOOL, BOOLEAN_TRUE)
		}

		PROCEDURE_STORE_VALUE1(UInt8, msgIndex)
	
	PROCEDURE_WAIT1(1, ObjectService, CB_HasCharacter, BOOL, hasChar)
		
		PROCEDURE_RESTORE_VALUE1(UInt8, msgIndex)

		if (msgIndex == CHECKNAME_OK && hasChar)
			msgIndex = CHECKNAME_HAS_CHARACTER;
		CB_CheckCharacterName(pPeer, context, msgIndex);

	PROCEDURE_CATCH
	
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CheckCharacterName Error: ObjectService:CB_Error, errcode %d, %s"), 
			err, errMsg.c_str());
		CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH


	return RPC_RESULT_OK;

}

void CAccount::SetData(CAccountData* pAcctData)
{
	SetInfo(pAcctData->GetAcct_id(), pAcctData->GetLoginName());
	SetAddict(pAcctData->GetDisconnect_time(), pAcctData->GetOffLnTime(), pAcctData->GetOnLnTime());
	SetBanTime(pAcctData->GetBanTime());
	SetAddictType(pAcctData->GetAddictType());
}

String CAccount::DebugInfo()
{
	String s(Parent::DebugInfo());
	s.AppendFormat(_T(", acct_id %d, last sid %s, wait sid %s, sel sid %d"), 
		GetAcct_id(), 
		(m_lastSessId.serviceId == INVALID_SERVICE_ID ? 
			_T("") : m_lastSessId.GetString().c_str()),
		(m_waitSessId.serviceId == INVALID_SERVICE_ID ? 
			_T("") : m_waitSessId.GetString().c_str()),
		(m_selSessId.serviceId == INVALID_SERVICE_ID ? 
			_T("") : m_selSessId.GetString().c_str()));

	return s;
}

CCharacterInfo* CAccount::GetCharInfo(UInt32 char_id)
{
	CCharacterInfo* child = NULL;

	for (UInt16 i = C_INDEX(0); i < m_characters.GetCount(); ++i)
	{
		child = m_characters.GetChild<CCharacterInfo>(i);
		if (child && child->GetChar_id() == char_id)
			break;
	}

	return child;
}


void CAccount::UpdateToObject(RPCContext &context, UInt32 actID, UInt32 offTm)
{
//	PROCEDURE_START_METHOD(CAccount, UpdateToObject, this, context)

/*		ObjectService_Proxy::UpdateAccount(gGlobal.pObjectService->pPeer, context, 
			actID, offTm);

	PROCEDURE_WAIT(1, ObjectService, CB_UpdateAccountSuccess)

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CAccount UpdateToObject Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

		// restore update mask after failed
		//PROCEDURE_RESTORE_VALUE1(RPCMask, oldMask)
		//MarkModify(oldMask);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK; */
}

CCharacterInfo* CAccount::GetLoginChar()
{
	LPCPEER pMapPeer;
	CCharacterInfo* pCharInfo;

	for (UInt16 i = 0; i < m_characters.GetCount(); ++i)
	{
		pCharInfo = m_characters.GetChild<CCharacterInfo>(i);
		if (pCharInfo && (pCharInfo->IsEnter() || pCharInfo->IsDisconnect()))
		{
			// check and change status to leave if map not available
			pMapPeer = gGlobal.GetMapInfo(MapIndex(pCharInfo->GetMap_id(), 
							pCharInfo->GetLine_id(), pCharInfo->GetMap_ownerId())).pPeer;
			if (pMapPeer)
				return pCharInfo;
			else
				pCharInfo->SetStatus(CCharacterInfo::ST_LEAVE);
		}
	}

	return NULL;
}
