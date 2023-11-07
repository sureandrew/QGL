#include "Stub/Service/LoginService.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "LoginService/LoginApplication.h"
#include "LoginService/Global.h"

RPCResult LoginService_Stub::RegisterProxy(const Peer &peer, RPCContext &context, const ServiceID &serviceId)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _T("RegisterProxy: serviceId %d"), serviceId);

	LoginApplication::GetInstance().RegisterProxy(peer, serviceId);

	return (RPC_RESULT_OK);
}

RPCResult LoginService_Stub::UnregisterProxy(const Peer &peer, RPCContext &context, const ServiceID &serviceId)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _T("UnregisterProxy: serviceId %d"), serviceId);

	if (!LoginApplication::GetInstance().UnregisterProxy(serviceId))
		return (RPC_RESULT_FAIL);

	return (RPC_RESULT_OK);
}

RPCResult LoginService_Stub::ReleaseSession(const Peer &peer, RPCContext &context)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _T("ReleaseSession: session ID %d, %d"), context.sessionId.serviceId,
		context.sessionId.socketId);

	gGlobal.m_sessionManager.Remove(context.sessionId);
	return RPC_RESULT_OK;
}

RPCResult ResumeSession(const Peer &peer, RPCContext &context, const SessionID &oldSessId)
{
	return RPC_RESULT_OK;
}

RPCResult LoginAccount(const Peer &peer, RPCContext &context, const String &loginName, const String &password)
{
	return RPC_RESULT_OK;
}

/*
RPCResult LoginService_Stub::GetAccount(const Peer &peer, RPCContext &context)
{
	CAccount* pAccount = NULL;

	Session* pSession = GetSession(context.sessionId);
	if (pSession == NULL)
	{
		pSession = gGlobal.m_sessionManager.Add(context.sessionId, Session());
	}

	if (pSession->pAccount == NULL)
	{
		pAccount = CAccount::NewInstance();
		pSession->pAccount = pAccount;
	}
	else
		pAccount = pSession->pAccount;

	pAccount->SetSendMask(NetObject::NoneMask);
	CB_GetAccount(peer, context, pAccount);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _T("Send Account: %d, %d"), pAccount->GetNetId().srvId, 
		pAccount->GetNetId().objId);

	return RPC_RESULT_OK;
}

RPCResult LoginService_Stub::DropAccount(const Peer &peer, RPCContext &context, NetObject *account)
{
	UnregisterNetObject(account);
	Reuben::Simulation::DeleteObject(account);

	return RPC_RESULT_OK;
}

RPCResult LoginService_Stub::LoginAccount(const Peer &peer, RPCContext &context,
										  const String &loginName, const String &password)
{
	PROCEDURE_START_RPC2(LoginService, LoginAccount, NULL, String, loginName, String, password)

		TRACE_INFODTL_2(GLOBAL_LOGGER, _T("login: name %s, password %s"), loginName.c_str(), password.c_str());
		ObjectService_Proxy::GetAccountByName(*gGlobal.pObjectServicePeer, context, loginName);

	PROCEDURE_WAIT1(1, ObjectService, CB_GetAccountData, NetObject*, acctData)

		CAccountData *pAcctData = dynamic_cast<CAccountData *>(acctData);
		TRACE_ENSURE(pAcctData != NULL);
		if (pAcctData->GetPassword() == password)		// password right
		{
			Session* psess = NULL;
			AccountMap::iterator it = gGlobal.m_accountMap.find(pAcctData->GetAcct_id());
			if (it != gGlobal.m_accountMap.end()) {		// account already exist
				psess = it->m_pSession;
				TRACE_ENSURE(psess);
				psess->pAccount->SetData(pAcctData);
				psess->pAccount->SendAttr(peer, context, CAccountData::AllMask);
				psess->pAccount->ChangeState(CAccount::ST_LOGIN);
				CB_LoginSuccess(peer, context, psess->pAccount, gGlobal.m_StartServiceTime, true);
				psess->Resume(context.sessionId);
			} else {
				psess = AddSession(context.sessionId, Session());
				psess->SetSessionId(context.sessionId);
				psess->pAccount = CAccount::NewInstance();		// new account
				psess->pAccount->SetData(pAcctData);
				psess->pAccount->SendAttr(peer, context, CAccountData::AllMask);
				psess->pAccount->ChangeState(CAccount::ST_LOGIN);
				CB_LoginSuccess(peer, context, psess->pAccount, gGlobal.m_StartServiceTime, false);
			}
			DeleteNetObject(acctData);
		} else {
			DeleteNetObject(acctData);
			CB_Error(peer, context, 1, "Login: wrong password\n");
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL);
		}

	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _T("Login Error: ObjectService:CB_Error, errcode %d, %s"), 
				err, errMsg.c_str());
			CB_Error(peer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}
*/