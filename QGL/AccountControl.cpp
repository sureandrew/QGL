//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "AccountControl.h"
//-- Library
#include "Configure.h"
#include "Global.h"
#include "Common/Procedure.h"
#include "Proxy/Service/WorldService.h"
#include "Proxy/Service/MapService.h"
#include "Proxy/Service/MessageService.h"
#include "Reuben/Simulation/ScheduleManager.h"
#include "Reuben/Utility/ProcessManager.h"
#include "CharacterControl.h"
#include "QGLApplication.h"
#include "CreateCharacterProcess.h"
#include "Common/Version.h"
#include "LoginProcess.h"
#include "Common/CallLuaFunc.h"
#include "NewBieHintCtrl.h"
#include "AntiHack.h"

#define CHANGELINE_TIME_LIMIT 180000

CAccountControl::CAccountControl()
{
	m_characters = NULL;
	m_pAccount = NULL;
	m_needReLogin = false;
	m_iChangeMapLineTimer = 0;
}

CAccountControl::~CAccountControl()
{
	DeleteNetObject(m_pAccount);
	DeleteNetGroup(m_characters, BOOLEAN_TRUE);
	m_needReLogin = false;
}

VOID CAccountControl::Login(const String &name, const ByteArray &password)
{
	WorldService_Proxy::LoginAccount(gGlobal.pPeerProxy, RPCContext(), 0, name, password);
}

RPCResult CAccountControl::LoginAccount(RPCContext &context)
{
	PROCEDURE_START_METHOD2(CAccountControl, LoginAccount, this, context, CAccount*, account, SYSTEMTIME, sysTime)

		TRACE_ENSURE(account);
		m_pAccount = account;
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_LoginSuccesss: acct_id %d"), m_pAccount->GetAcct_id());
		gGlobal.m_nAccountID = m_pAccount->GetAcct_id();
		{
			gGlobal.m_TimeStartService = sysTime;
			UInt32 totalMin = (UInt32)((gGlobal.m_TimeStartService.wHour % 2) * 60 + gGlobal.m_TimeStartService.wMinute);
			gGlobal.m_PreClockCount = gGlobal.m_NextClockCount = totalMin / 10;  
			gGlobal.m_RemainCalcMS = (totalMin % 10) * 60000 + (gGlobal.m_TimeStartService.wSecond * 1000) + gGlobal.m_TimeStartService.wMilliseconds;
			gGlobal.m_InitGameTimeInterval = gGlobal.m_PreGameTimeInterval = ::timeGetTime();
		}
		//gGlobal.m_logined = true;
		m_pAccount->GetOwnCharacters(gGlobal.pPeerProxy, context);
		
	PROCEDURE_WAIT_OBJ1(2, Account, CB_GetOwnCharacters, account, NetGroup*, characters)

		TRACE_ENSURE(characters);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_GetOwnCharacters: count %d"), characters->GetCount());

		if (m_characters && m_characters->GetID() != characters->GetID())
		{
			DeleteNetGroup(m_characters);
		}
		m_characters = characters;

		UInt32 charIds[3];
		ZeroMemory(charIds, sizeof(UInt32) * 3);
		for (Index i = C_INDEX(0); i < characters->GetCount(); ++i)
		{
			CCharacterInfo* pchar = characters->GetChild<CCharacterInfo>(i);
			
			if (pchar)
			{
				TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CharacterInfo %d: acct_id %d, char_id %d, name %s"), i, 
					pchar->GetAcct_id(), pchar->GetChar_id(), pchar->GetNickName().c_str());
				
				if (i > 2)
					continue;

				charIds[i] = pchar->GetChar_id();
			}
		}		

		if (m_pAccount)
			gGlobal.g_pNewBieHintCtrl->ReadSetting(m_pAccount->GetAcct_id());

		Boolean bCleanUpSomething = gGlobal.g_pNewBieHintCtrl->CleanupNonExistBie(charIds[0], charIds[1], charIds[2]);
		if( bCleanUpSomething )
			gGlobal.g_pNewBieHintCtrl->WriteSetting(m_pAccount->GetAcct_id());

		/*int autoCharacter = GetPrivateProfileInt(_T("AUTOCHARACTER"), _T("AUTO"), 1, CONFIGURATION_INI_FILE);
		if (autoCharacter)
		{
			gGlobal.g_pLoginProcess->SetCharacterIndex(0);
			gGlobal.g_pLoginProcess->SetLineIndex(1);
			PROCEDURE_DO_METHOD1(CAccountControl, EnterGame, this, context, 
				Int32, gGlobal.g_pLoginProcess->GetCharacterIndex())
		}
		else */
		{
			gGlobal.pUIManager->SetEvent(105);
			PROCEDURE_GO_STEP(3)
		}

	PROCEDURE_WAIT_RETURN(3)

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Login Error: ObjectService:CB_CharacterError, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

VOID CAccountControl::Logout()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CAccountControl, Logout, this, context)
}

RPCResult CAccountControl::Logout(RPCContext &context)
{
	PROCEDURE_START_METHOD(CAccountControl, Logout, this, context)

		if (m_pAccount == NULL)
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("GetOwnCharacters Error: null account net object"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Character Logout:"));
		if (m_pAccount)
			m_pAccount->Logout(gGlobal.pPeerProxy, context);

//	PROCEDURE_WAIT_OBJ(1, Account, CB_LogoutSuccess, account)

		// delete net object after leave map
		DeleteNetGroup(m_characters, BOOLEAN_TRUE);
		if (gGlobal.g_pCharacter)
			gGlobal.g_pCharacter->SetMapActorObject(NULL);
		DeleteNetObject(gGlobal.g_pMap);

		TRACE_INFODTL(GLOBAL_LOGGER, _F("Logout successful"));
		gGlobal.m_nAccountID = 0;
		//gGlobal.m_logined = false;
		DeleteNetObject(m_pAccount);
		
	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Logout: Account::CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

VOID CAccountControl::EnterGame(const Int &Index)
{
	RPCContext context;
	PROCEDURE_DO_METHOD1(CAccountControl, EnterGame, this, context, Int, Index)
}

RPCResult CAccountControl::EnterGame(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CAccountControl, EnterGame, this, context, Int, Index)

		if (m_pAccount == NULL)
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("GetOwnCharacters Error: null account net object"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		// Select first character in World in Line 1
		if (Index >= 0 && Index < m_characters->GetCount()) 
		{
			CCharacterInfo *pchar = m_characters->GetChild<CCharacterInfo>(Index);
			if (pchar)
			{
				UInt8 selectedLine = gGlobal.g_pLoginProcess->GetLineIndex() > 0 ? gGlobal.g_pLoginProcess->GetLineIndex() : pchar->GetLine_id();

				m_pAccount->SelectCharacter(gGlobal.pPeerProxy, context, pchar->GetChar_id(), selectedLine);

				// Write to config file
				::SetDefaultCharacter(Index);
				if (gGlobal.g_pLoginProcess->GetLineIndex() > 0) // if not server resume login
					::SetDefaultLine(gGlobal.g_pLoginProcess->GetLineIndex() - 1);
			}
			else
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("EnterGame: [not character info] index %d character count %d"),
					Index, m_characters->GetCount());
				PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
			}
		}
		else
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("EnterGame: invalid index %d for character count %d"),
				Index, m_characters->GetCount());
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		PROCEDURE_SET_TIMEOUT(60000, BOOLEAN_TRUE);	// 1 min timeout for select character

	PROCEDURE_WAIT_OBJ(1, Account, CB_SelectCharacter, account)

		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_SelectCharacter: successful to select character"));

		gGlobal.pUIManager->DeletePanel("CharacterChosenPanel");
		gGlobal.pUIManager->DeletePanel("ServerChosenPanel");
		gGlobal.pUIManager->DeletePanel("ForceLogoutPanel");
		gGlobal.pUIManager->DeletePanel("LoginEditPanel");
		gGlobal.pUIManager->DeletePanel("LoginButtonPanel");
		gGlobal.pUIManager->DeletePanel("LoginCheckBoxPanel");
		gGlobal.pUIManager->DeletePanel("MiniKeyBoardPanel");
		gGlobal.pUIManager->DeletePanel("LoginBackground");
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());
		gGlobal.pUIManager->RenderAll();

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("EnterGame: Account::CB_Error, errcode %d, %s"),	err, errMsg.c_str());
			if (err == 2)
			{
				gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_SYS_LINE_FULL")), false, 3000, 3000);
				UIPanel* serverChosenPanel = gGlobal.pUIManager->FindUIPanel("ServerChosenPanel");
				UIPanel* loginBackgroup = gGlobal.pUIManager->FindUIPanel("LoginBackground");
				if (serverChosenPanel && loginBackgroup)
				{
					serverChosenPanel->SetEnable(true);
					loginBackgroup->SetEnable(true);
				}
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SetLoginState", String(), 2);
				gGlobal.pUIManager->RenderAll();

				
			}

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

Boolean CAccountControl::IsSelectCharacterFrozen()
{
	if ((gGlobal.g_pLoginProcess->GetCharacterIndex() < 0) || (gGlobal.g_pLoginProcess->GetCharacterIndex() >= m_characters->GetCount()))
		return BOOLEAN_TRUE;

	CCharacterInfo *pchar = m_characters->GetChild<CCharacterInfo>(gGlobal.g_pLoginProcess->GetCharacterIndex());
	if (pchar)
	{
		return (pchar->GetDelete_date() != 0); 
	}
	else
		return BOOLEAN_TRUE;
}

Boolean CAccountControl::CheckCharacterFrozenStatus(UInt32 index)
{
	if ((index < 0) || (index >= m_characters->GetCount()))
		return BOOLEAN_TRUE;

	CCharacterInfo *pchar = m_characters->GetChild<CCharacterInfo>(index);
	if (pchar)
	{
		return (pchar->GetDelete_date() != 0); 
	}
	else
		return BOOLEAN_TRUE;

}

UInt16 CAccountControl::GetSelectedCharacterLevel()
{
	if (gGlobal.g_pLoginProcess->GetCharacterIndex() < 0 || gGlobal.g_pLoginProcess->GetCharacterIndex() >= m_characters->GetCount())
		return 0;

	CCharacterInfo *pchar = m_characters->GetChild<CCharacterInfo>(gGlobal.g_pLoginProcess->GetCharacterIndex());
	if (pchar)
	{
		return pchar->GetLevel(); 
	}
	else
		return 0;
}

VOID CAccountControl::FrozenCharacter()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CAccountControl, FrozenCharacter, this, context)
}

RPCResult CAccountControl::FrozenCharacter(RPCContext &context)
{
	PROCEDURE_START_METHOD(CAccountControl, FrozenCharacter, this, context)

		if (m_pAccount == NULL)
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("FrozenCharacter Error: null account net object"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		if (gGlobal.g_pLoginProcess->GetCharacterIndex() < 0 || gGlobal.g_pLoginProcess->GetCharacterIndex() >= m_characters->GetCount())
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("FrozenCharacter Error: null account net object"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CCharacterInfo *pCharData = m_characters->GetChild<CCharacterInfo>(gGlobal.g_pLoginProcess->GetCharacterIndex());

		m_pAccount->FreezeCharacter(gGlobal.pPeerProxy, context, pCharData->GetChar_id());

	//	gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_REMOVE_PROCESS")), false);


	PROCEDURE_WAIT_OBJ1(1, Account, CB_FreezeCharSuccess, account, NetObject*, charInfo )
		
		TRACE_ENSURE(charInfo);	
		CCharacterInfo *pCharData = CastNetObject<CCharacterInfo>(charInfo);
		for (int i = 0; i < m_characters->GetCount(); ++i)
		{
			CCharacterInfo *pChar = m_characters->GetChild<CCharacterInfo>(i);
			if (pChar && (pChar->GetChar_id() == pCharData->GetChar_id()))
			{
				m_characters->Replace(i, pCharData);
				break;
			}
		}
		gGlobal.pUIManager->SetEvent(105);	
		
	PROCEDURE_CATCH
			PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("FrozenCharacter: Account::CB_Error, errcode %d, %s"), err, errMsg.c_str());	

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

VOID CAccountControl::RecoverCharacter()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CAccountControl, RecoverCharacter, this, context)
}

RPCResult CAccountControl::RecoverCharacter(RPCContext &context)
{
	PROCEDURE_START_METHOD(CAccountControl, RecoverCharacter, this, context)

		if (m_pAccount == NULL)
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("RecoverCharacters Error: null account net object"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		if (gGlobal.g_pLoginProcess->GetCharacterIndex() < 0 || gGlobal.g_pLoginProcess->GetCharacterIndex() >= m_characters->GetCount())
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("RecoverCharacters Error: null account net object"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CCharacterInfo *pCharData = m_characters->GetChild<CCharacterInfo>(gGlobal.g_pLoginProcess->GetCharacterIndex());

		m_pAccount->RecoverCharacter(gGlobal.pPeerProxy, context, pCharData->GetChar_id());

	//	gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_REMOVE_PROCESS")), false);

	PROCEDURE_WAIT_OBJ1(1, Account, CB_RecoverCharSuccess, account, NetObject*, charInfo )

		TRACE_ENSURE(charInfo);	
		CCharacterInfo *pCharInfo = CastNetObject<CCharacterInfo>(charInfo);
		for (int i = 0; i < m_characters->GetCount(); ++i)
		{
			CCharacterInfo *pChar = m_characters->GetChild<CCharacterInfo>(i);
			if (pChar && (pChar->GetChar_id() == pCharInfo->GetChar_id()))
			{
				m_characters->Replace(i, pCharInfo);
				break;
			}
		}
		gGlobal.pUIManager->SetEvent(105);
		
	PROCEDURE_CATCH
			PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("RecoverCharacter: Account::CB_Error, errcode %d, %s"), err, errMsg.c_str());	

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;	
}

VOID CAccountControl::CheckCharacter()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CAccountControl, CheckCharacter, this, context)
}

RPCResult CAccountControl::CheckCharacter(RPCContext &context)
{
	PROCEDURE_START_METHOD(CAccountControl, CheckCharacter, this, context)

		if (m_pAccount == NULL) {
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("SelectCharacter Error: account not logined"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		CCharacterInfo *pchar = m_characters->GetChild<CCharacterInfo>(gGlobal.g_pLoginProcess->GetCharacterIndex());

		if (pchar == NULL) {
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("SelectCharacter Error: null character"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		m_pAccount->CheckCharacter(gGlobal.pPeerProxy, context, pchar->GetChar_id());

	PROCEDURE_WAIT_OBJ3(1, Account, CB_CheckCharacter, account, UInt8, charStatus, Int32Vector, lineCounts, UInt16Vector, lineState)

		if (charStatus == CCharacterInfo::ST_NOT_FOUND) {
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("SelectCharacter Error: character not found"));
			gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CHAR_MISSING")));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		} else if (charStatus == CCharacterInfo::ST_NOT_AVAIL) {
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("SelectCharacter Error: character not availiable"));
			gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CHAR_UNAVAIL")));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		} else if (charStatus == CCharacterInfo::ST_ENTER) {
			TRACE_INFODTL(GLOBAL_LOGGER, _F("SelectCharacter: force out other client playing character"));
			gGlobal.pUIManager->SetEvent(107);
		} else if (charStatus == CCharacterInfo::ST_DISCONNECT) {
			TRACE_INFODTL(GLOBAL_LOGGER, _F("SelectCharacter: resume playing character"));
			gGlobal.pUIManager->SetEvent(104);
		} else {
			TRACE_INFODT(GLOBAL_LOGGER, _F("SelectCharacter: lines can selected: "));
			for (UInt16 i = 0; i < (UInt16) lineCounts.size(); ++i)
			{
				UInt16 tempValue = lineState[i];
				TRACE_INFO_2(GLOBAL_LOGGER, _F("L%d=%d, "), i, lineCounts[i]);
				TRACE_INFO_2(GLOBAL_LOGGER, _F("Lstate %d=%d, "), i, lineState[i]);
			}
			TRACE_INFOL(GLOBAL_LOGGER, _F(""));

			gGlobal.m_LineExist.resize(lineCounts.size());
			gGlobal.m_lineState.resize(lineState.size());
			std::copy(lineCounts.begin(), lineCounts.end(), gGlobal.m_LineExist.begin());
			std::copy(lineState.begin(), lineState.end(), gGlobal.m_lineState.begin());
			gGlobal.pUIManager->SetEvent(106);
		}

	PROCEDURE_CATCH
		
		PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CheckCharacter Error: Account:CB_Error, errcode %d, %s"), err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

VOID CAccountControl::DeleteCharacter()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CAccountControl, DeleteCharacter, this, context)
}

RPCResult CAccountControl::DeleteCharacter(RPCContext &context)
{
	PROCEDURE_START_METHOD(CAccountControl, DeleteCharacter, this, context)

		if (m_pAccount == NULL)
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("DeleteCharacters Error: null account net object"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		if (gGlobal.g_pLoginProcess->GetCharacterIndex() < 0 || gGlobal.g_pLoginProcess->GetCharacterIndex() >= m_characters->GetCount())
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("DeleteCharacters Error: null account net object"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CCharacterInfo *pCharData = m_characters->GetChild<CCharacterInfo>(gGlobal.g_pLoginProcess->GetCharacterIndex());

		m_pAccount->CheckCharacter(gGlobal.pPeerProxy, context, pCharData->GetChar_id());

		gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_REMOVE_PROCESS")), false);

	PROCEDURE_WAIT_OBJ3(1, Account, CB_CheckCharacter, account, UInt8, charStatus, Int32Vector, lineCounts, UInt16Vector, lineState)

		if (charStatus == CCharacterInfo::ST_NOT_FOUND || charStatus == CCharacterInfo::ST_NOT_AVAIL || 
			charStatus == CCharacterInfo::ST_ENTER || charStatus == CCharacterInfo::ST_DISCONNECT)
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("RemoveCharacter Error: character is playing the game"));
			gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_REMOVE_CHECK_FAIL")));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CCharacterInfo *pCharData = m_characters->GetChild<CCharacterInfo>(gGlobal.g_pLoginProcess->GetCharacterIndex());

		m_pAccount->RemoveCharacter(gGlobal.pPeerProxy, context, pCharData->GetChar_id());

	PROCEDURE_WAIT(2, Account, CB_RemoveCharSuccess)

		CCharacterInfo *pCharData = m_characters->GetChild<CCharacterInfo>(gGlobal.g_pLoginProcess->GetCharacterIndex());
		if (pCharData)
		{
			gGlobal.g_pNewBieHintCtrl->ClearNewBie(pCharData->GetChar_id());
			gGlobal.g_pNewBieHintCtrl->WriteSetting(pCharData->GetAcct_id());
            m_characters->Remove(pCharData);
		}
		gGlobal.g_pLoginProcess->ShowCharacterInLogin();
		gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_REMOVE_SUCCESS")));

	PROCEDURE_CATCH
		
		PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)

			gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CREATE_ERR")));
			gGlobal.pUIManager->SetEvent(105);

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("DeleteCharacter: Account::CB_Error, errcode %d, %s"), err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

VOID CAccountControl::CreateCharacter()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CAccountControl, CreateCharacter, this, context)
}

RPCResult CAccountControl::CreateCharacter(RPCContext &context)
{
	PROCEDURE_START_METHOD(CAccountControl, CreateCharacter, this, context)

		if (m_pAccount == NULL) {
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CreateCharacters Error: null account net object"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		UIEditBox* nickName = (UIEditBox*)gGlobal.pUIManager->GetUIItem("CharacterName");
		String name(nickName->GetText());

		m_pAccount->AddCharacter(gGlobal.pPeerProxy, context, name.c_str(), gGlobal.g_pCreateCharProcess->GetSex(), 
			gGlobal.g_pCreateCharProcess->GetClass(), gGlobal.g_pCreateCharProcess->GetHeadIndex(), gGlobal.g_pCreateCharProcess->GetHairColIndex(), 
			gGlobal.g_pCreateCharProcess->GetClothIndex(), gGlobal.g_pCreateCharProcess->GetClothColIndex() );

	PROCEDURE_WAIT(1, Account, CB_CreateSuccess)

		m_pAccount->GetOwnCharacters(gGlobal.pPeerProxy, context);
		
	PROCEDURE_WAIT_OBJ1(2, Account, CB_GetOwnCharacters, account, NetGroup*, characters)

		TRACE_ENSURE(characters);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_GetOwnCharacters: count %d"), characters->GetCount());

		if (m_characters && m_characters->GetID() != characters->GetID())
		{
			DeleteNetGroup(m_characters);
		}
		m_characters = characters;

		for (UInt16 i = 0; i < characters->GetCount(); i++)
		{
			CCharacterInfo *pchar = characters->GetChild<CCharacterInfo>(i);
			
			if (pchar && gGlobal.g_pNewBieHintCtrl && !gGlobal.g_pNewBieHintCtrl->IsExistBie(pchar->GetChar_id()))
			{
				gGlobal.g_pNewBieHintCtrl->SetBie(pchar->GetChar_id(), BOOLEAN_TRUE);
				gGlobal.g_pNewBieHintCtrl->WriteSetting(pchar->GetAcct_id());
			}
		
			TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CharacterInfo %d: acct_id %d, char_id %d, name %s"), i, 
				pchar->GetAcct_id(), pchar->GetChar_id(), pchar->GetNickName().c_str());
		}
		
		
		gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CREATE_SUCCESS")));
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseCreateCharacterPanel", String());
		gGlobal.pUIManager->SetEvent(105);
		
	PROCEDURE_CATCH
		
		PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)
		
			switch(err)
			{
			case CHECKNAME_TOO_LONG:
				gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CHECK_NAME_TOO_LONG")));
				break;
			case CHECKNAME_CANT_USE:
				gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CHECK_NAME_CANT_USE")));
				break;
			case CHECKNAME_HAS_CHARACTER:
				gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CHECK_NAME_HAVE_CHARACTER")));
				break;
			default:
				gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CREATE_ERR")));
			}					
			
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CreateCharacter: Account::CB_Error, errcode %d, %s"), err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

VOID CAccountControl::CheckVersion()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CAccountControl, CheckVersion, this, context)
}

RPCResult CAccountControl::CheckVersion(RPCContext &context)
{
	/*PROCEDURE_START_METHOD(CAccountControl, CheckVersion, this, context)

		WorldService_Proxy::VersionChecking(gGlobal.pPeerProxy, context, VERSION_NUMBER);

	PROCEDURE_WAIT1(1, WorldService, CB_VersionCorrect, Boolean, equal)

		if (equal)
		{

		}
		else
		{
			gGlobal.AddAlertBox(_T("Version is not eq"));
		}

	PROCEDURE_CATCH
		
		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)
			
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CheckVersion: WorldService::CB_Error, errcode %d, %s"),	err, errMsg.c_str());

	PROCEDURE_END_CATCH*/

	return RPC_RESULT_OK;
}

VOID CAccountControl::CheckCharacterName()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CAccountControl, CheckCharacterName, this, context)
}

RPCResult CAccountControl::CheckCharacterName(RPCContext &context)
{
	PROCEDURE_START_METHOD(CAccountControl, CheckCharacterName, this, context)

		UIEditBox* nickName = (UIEditBox*)gGlobal.pUIManager->GetUIItem("CharacterName");
		StringW name = nickName->GetText();

		m_pAccount->CheckCharacterName(gGlobal.pPeerProxy, context, name.c_str() );

	PROCEDURE_WAIT_OBJ1(1, Account, CB_CheckCharacterName, account, UInt8, msgIndex)

		if (msgIndex != CHECKNAME_OK)
		{
			switch(msgIndex)
			{
			case CHECKNAME_TOO_LONG:
				gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CHECK_NAME_TOO_LONG")));
				break;
			case CHECKNAME_CANT_USE:
				gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CHECK_NAME_CANT_USE")));
				break;
			case CHECKNAME_HAS_CHARACTER:
				gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CHECK_NAME_HAVE_CHARACTER")));
				break;
			default:
				gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CREATE_CHECK_FAIL")));
			}			
		}
		else
		{
			gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_CREATE_CHECK_OK")));
		}
			
	PROCEDURE_CATCH
		
		PROCEDURE_EXCEPTION_OBJ2(Account, CB_Error, account, UInt32, err, String, errMsg)
			
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CheckCharacterName: Account::CB_Error, errcode %d, %s"), err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

VOID CAccountControl::UpdateMapLine()
{
	if (gGlobal.g_pMap == NULL && gGlobal.pUIManager)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseChangeLine", String());
		return;
	}

	RPCContext context;
	PROCEDURE_DO_METHOD(CAccountControl, UpdateMapLine, this, context)
}

RPCResult CAccountControl::UpdateMapLine(RPCContext &context)
{
	PROCEDURE_START_METHOD(CAccountControl, UpdateMapLine, this, context)

		WorldService_Proxy::GetMapLines(gGlobal.pPeerProxy, context, gGlobal.g_pMap->GetMap_id());

	PROCEDURE_WAIT2(1, WorldService, CB_GetMapLines, Int32Vector, lineCounts, UInt16Vector, stateList)

		gGlobal.m_LineExist.resize(lineCounts.size());
		gGlobal.m_lineState.resize(stateList.size());
		std::copy(lineCounts.begin(), lineCounts.end(), gGlobal.m_LineExist.begin());
		std::copy(stateList.begin(), stateList.end(), gGlobal.m_lineState.begin());
		
		if (gGlobal.g_pLoginProcess)
		{
			gGlobal.g_pLoginProcess->UpdateChangeLine();
		}

	PROCEDURE_CATCH
		
		PROCEDURE_EXCEPTION2(WorldService, CB_Error, UInt32, err, String, errMsg)
			
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("UpdateMapLine WorldService::CB_Error, errcode %d, %s"), err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

VOID CAccountControl::ChangeMapLine(UInt8 lineIndex)
{
	if (gGlobal.m_mapdb->m_basicinfo->m_changelineenabled)
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_THIS_MAP_CANT_CHANGE_LINE")));
		return;
	}
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	{
		//if (gGlobal.g_pParty)
		//{
		//	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_SYS_HAS_PARTY_CANT_CHANGE_LINE")));
		//}
		//else
		//{
#ifdef REUBEN_PUBLISH
			UInt diff = GetTickCount() - m_iChangeMapLineTimer;

			if (diff > 60000) //1 min
			{
				gGlobal.g_pCharacter->GetCharNetObject()->ChangeMapLine(gGlobal.pPeerProxy,
					gGlobal.g_context, lineIndex);
				m_iChangeMapLineTimer = GetTickCount();
				if (NULL != gGlobal.pUIManager)
				{
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "FunAccShopClose", String());
					CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "FunCloseGenerationPanel", String());
				}
				gGlobal.m_bChangeLine = true;
			}
			else
			{
				String msg;
				msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANGE_LINE_TOO_FREQ")), (CHANGELINE_TIME_LIMIT - diff) / 1000);
				gGlobal.PrintPlayerMessage(msg);
			}
#else
			gGlobal.g_pCharacter->GetCharNetObject()->ChangeMapLine(gGlobal.pPeerProxy,
				gGlobal.g_context, lineIndex);
			gGlobal.m_bChangeLine = true;
#endif // REUBEN_PUBLISH
			
			/*String msg;
			String lineName;
			lineName.Format(_T("MSG_INTERFACE_LINE_%d"), lineIndex);
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANGE_LINE")), 
				gGlobal.GetStringTable()->Get(lineName.c_str()) );
			gGlobal.PrintPlayerMessage(msg);*/
		//}
	}
}

RPCResult CAccountControl::ConfirmKick(RPCContext &context)
{
	// default kick is 'yes'
	WorldService_Proxy::KickAccount(gGlobal.pPeerProxy, context, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}
