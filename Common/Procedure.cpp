//-- Common
#include "Common.h"
#include "Common/SessionManager.h"
#include "Common/LocalRPCManager.h"
#include "Common/RPCFunc.h"
//-- Self
#include "Common/Procedure.h"

#ifdef SERVICE
	//#define PCD_WAIT_TIMEOUT	0		// no timeout inside sevices
	#define PCD_WAIT_TIMEOUT	30000	// 30 seconds for procedure for waiting
#else
	#define PCD_WAIT_TIMEOUT	30000	// 30 second for procedure for client timeout
#endif // SERVICE

#define PCD_LOCK_TIMEOUT	30000		// 30 seconds for procedure waiting unlock timeout

#define PCD_LOCKQUEUE_WARN		30		// number of lock queue warning when too much lock waiting

UInt32 ProcedureManager::_timeout = PCD_LOCK_TIMEOUT;
bool ProcedureManager::_timeoutFail = PCD_DEFAULT_TIMEOUT_FAIL;
StringA ProcedureManager::_debugMsg = "";

ProcedureManager::ProcedureManager()
{
	m_nextPcdId = 0;
	m_bNonBlock = BOOLEAN_FALSE;
}

ProcedureManager::~ProcedureManager()
{
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("ProcedureManager Ended: remain procedures %d"), 
		m_pcdMap.size());

	ProcedureMap::iterator it;
	while (!m_pcdMap.empty())
	{
		it = m_pcdMap.begin();
		ProcedureID pid = it->first;
		ProcedureInfo* pinfo = it->second;

		TRACE_VERBOSEL_8(GLOBAL_LOGGER, _F("Remained Procedure %d:%d:%s, step %d,")
			_T(" trapRpcId %d, errRpcId %d, oldPcdId %d:%d"),
			pid.srvId, pid.subId, pinfo->label.c_str(), pinfo->step, pinfo->trapRpcId, 
			pinfo->errRpcId, pinfo->oldPcdId.srvId, pinfo->oldPcdId.subId);

		SafeRelease(pinfo->pSrcPeer);
		SafeDeleteObject(pinfo->pSrcBuf);
		SafeDeleteObject(pinfo->pParamBuf);
		SafeDeleteObject(pinfo->pCountBuf);
		SafeDeleteObject(pinfo);
		m_pcdMap.erase(it);
	}
	m_methodSrcMap.clear();
}

const ServiceID ProcedureManager::GetServiceId() const
{
	return LocalRPCManager::GetInstance().GetServiceId();
}

RPCResult ProcedureManager::DoProcess(RPCID rpcid, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	pBuf->ResetCursor();
	return LocalRPCManager::GetInstance().DoProcess(rpcid, pPeer, context, pBuf);
}

ProcedureID ProcedureManager::GetNextPcdId()
{
	// find a available procedure ID
	ProcedureID pcdId;

	pcdId.srvId = GetServiceId();
	// no need checking now for 32-bit procedure ID
	//Count counter = C_COUNT(0);

	//do
	{
		m_nextPcdId ++;
		// rotated
		if (m_nextPcdId == 0)
			m_nextPcdId ++;
		pcdId.subId = m_nextPcdId;
		//++counter;
		//TRACE_ENSURE(counter < UINT32_MAX_VALUE);
	}
	//while (m_pcdMap.find(pcdId) != m_pcdMap.end());
	return pcdId;
}

ProcedureInfo* ProcedureManager::NewProcedure(RPCContext &context)
{
	ProcedureID pcdId = GetNextPcdId();

	if (m_pcdMap.find(pcdId) != m_pcdMap.end())
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("pcd %d:%d already exist"), 
			pcdId.srvId, pcdId.subId);
	}

	ProcedureInfo *parentPinfo = NULL;
	ProcedureInfo* pinfo = SafeCreateObject(ProcedureInfo);
	TRACE_ENSURE(pinfo);
	pinfo->pSrcBuf = SafeCreateObject1(Buffer, 50);
	pinfo->origContext = context;
	pinfo->timeoutFail = PCD_DEFAULT_TIMEOUT_FAIL;
	m_pcdMap[pcdId] = pinfo;

	if (context.pcdId.srvId == GetServiceId() && context.pcdId.subId != 0) {
		ProcedureMap::iterator iter = m_pcdMap.find(context.pcdId);
		if (iter != m_pcdMap.end()) {
			parentPinfo = iter->second;
			// don't set parent pcdId for single RPC method
			if (parentPinfo->pcdType != ProcedureInfo::PCD_RPC || !parentPinfo->singleRpc) {
				pinfo->oldPcdId = context.pcdId;
				parentPinfo->trapRpcId = 0;	// reset parent trapRpcId
				parentPinfo->needResume = BOOLEAN_FALSE;	// parent no need resume first
				parentPinfo->step++;					// increase to next step when do sub call
			}
		}
	}
	context.pcdId = pcdId;

	return pinfo;
}

void ProcedureManager::WaitResume(ProcedureID pcdId)
{
#ifdef SERVICE
	// put to resume set for services
	m_resumeSet.insert(pcdId);
#else
	// direct resume for client
	ProcedureInfo* pinfo = Get(pcdId);
	if (pinfo)
		Resume(pinfo, pcdId);
#endif // SERVICE
}

void ProcedureManager::CancelResume(ProcedureID pcdId)
{
#ifdef SERVICE
	if (!m_resumeSet.empty())
		m_resumeSet.erase(pcdId);
#endif // SERVICE
}

void ProcedureManager::Resume(ProcedureInfo *pinfo, ProcedureID pcdId)
{
	if (pinfo == NULL || pcdId.subId == 0)
		return;

//	if (pinfo->singleRpc && pinfo->step > 0)	// no resume for single RPC function after step 0
//		return;

	RPCContext context(pinfo->origContext.sessionId);
	context.pcdId = pcdId;
	
	// redirect to new session Id before resume
	SessionManager::GetInstance().RedirectSessionId(context.sessionId);

	TRACE_VERBOSEDT_9(GLOBAL_LOGGER, 
		_F("sessionId %s, pcdId %d:%d, pinfo %p, label %s, pcdType %d, objId %d, method %p, func %p"),
		context.sessionId.GetString().c_str(), pcdId.srvId, pcdId.subId, pinfo,
		pinfo->label.c_str(), pinfo->pcdType, pinfo->psrc, pinfo->cbMethod, pinfo->cbFunc);

	TRACE_VERBOSEL_9(GLOBAL_LOGGER,
		_T(", step %d, trapRpcID %d, errRpcId %d, oldPcdId %d, srcRpcId %d, pSrcPeer %d, pSrcBuf %d, Cursor %d, Size %d"),
		pinfo->step, pinfo->trapRpcId, pinfo->errRpcId, pinfo->oldPcdId.subId, pinfo->srcRpcId, pinfo->pSrcPeer,
		pinfo->pSrcBuf, (pinfo->pSrcBuf) ? pinfo->pSrcBuf->GetCursor() : 0, (pinfo->pSrcBuf) ? pinfo->pSrcBuf->GetSize() : 0);

	//if (pinfo->pSubBuf != &m_tmpBuf)		// reset sub message for safety
	//	pinfo->pSubBuf = NULL;

	// if LocalRPCManager has receive handler, do that first
#ifdef SIMCLIENT
	HandleFunc hf = LocalRPCManager::GetInstance().GetRecvHandle();
	if (hf)
		(*hf) (NULL, context);
#endif // SIMCLIENT

	static bool threw = false;
	if (pinfo->pSrcBuf)
		pinfo->pSrcBuf->ResetCursor();
	if (pinfo->pcdType == ProcedureInfo::PCD_RPC)
	{
		bool bException = false;
		try
		{
			LocalRPCManager::GetInstance().DoProcess(pinfo->srcRpcId, pinfo->pSrcPeer, 
				context, pinfo->pSrcBuf);
			// check if it is a remained single step procedure
			pinfo = Get(context.pcdId);
			if (pinfo && pinfo->singleRpc)
				EndReturn(pinfo, pcdId, PCD_RETURN);
		}
		catch (Reuben::System::Exception e)
		{
			TRACE_ERRORDTL_9(GLOBAL_LOGGER,
				_FE("exception, sessionId %s, pid %d:%d, pinfo %p, label %s, srcRpcId %d, cursor %d, size %d, %s"),
				context.sessionId.GetString().c_str(), pcdId.srvId, pcdId.subId, pinfo, 
				String(pinfo->label).c_str(), pinfo->srcRpcId, 
				(pinfo->pSrcBuf) ? pinfo->pSrcBuf->GetCursor() : 0, 
				(pinfo->pSrcBuf) ? pinfo->pSrcBuf->GetSize() : 0,
				e.GetErrorMessage().c_str());
			TRACE_ERRORDTL_2(REUBEN_LOGGER, _FE("exception occurs at %s, %s"), GLOBAL_NAME,
				e.GetErrorMessage().c_str());
			bException = true;
		}
		catch (...)
		{
			TRACE_ERRORDTL_9(GLOBAL_LOGGER,
				_FE("exception, sessionId %s, pid %d:%d, pinfo %p, label %s, srcRpcId %d, srcBuf %p, cursor %d, size %d"),
				context.sessionId.GetString().c_str(), pcdId.srvId, pcdId.subId, pinfo, 
				String(pinfo->label).c_str(), pinfo->srcRpcId, pinfo->pSrcBuf,
				(pinfo->pSrcBuf) ? pinfo->pSrcBuf->GetCursor() : 0, 
				(pinfo->pSrcBuf) ? pinfo->pSrcBuf->GetSize() : 0);
			TRACE_ERRORDTL_1(REUBEN_LOGGER, _FE("exception occurs at %s"), GLOBAL_NAME);
			bException = true;
		}

		try
		{
			if (bException)
			{
				if (pinfo->pSrcBuf)
				{
					for (Size i = 0; i < pinfo->pSrcBuf->GetSize(); ++i)
					{
						TRACE_ERROR_1(GLOBAL_LOGGER, _F("%02X "), *(pinfo->pSrcBuf->GetData() + i));
					}
				}
				EndReturn(pinfo, pcdId, PCD_ERR_EXCEPTION);
			}
		}
		catch (...)
		{
			TRACE_ERRORDTL_7(GLOBAL_LOGGER, 
				_FE("double exception, sessionId %s, pid %d:%d, pinfo %p, label %s, pcdType %d, objId %d"),
				context.sessionId.GetString().c_str(), pcdId.srvId, pcdId.subId, pinfo,
				String(pinfo->label).c_str(), pinfo->pcdType, pinfo->psrc);
		}
	}
	else if (pinfo->pcdType == ProcedureInfo::PCD_METHOD)
	{
		bool bException = false;
		try
		{
			bool found = (pinfo->psrc != NULL);
			if (pinfo->psrc && pinfo->psrc->IsSubClass(NetObject_CLASSID))
			{
				NetObject* pobj = (NetObject*)(pinfo->psrc);
				if (pobj && pobj->GetNetId().IsValid())
					found = (FindNetObject(pobj->GetNetId()) == pobj);
			}
			if (found)
				(pinfo->psrc->*(pinfo->cbMethod)) (context);
			else
			{
				TRACE_WARNDTL_7(GLOBAL_LOGGER, 
					_FW("[NetObject not found], sessionId %s, pid %d:%d, pinfo %p, label %s, pcdType %d, objId %d"),
					context.sessionId.GetString().c_str(), pcdId.srvId, pcdId.subId, pinfo,
					String(pinfo->label).c_str(), pinfo->pcdType, pinfo->psrc);
				EndReturn(pinfo, pcdId, PCD_ERR_NETOBJ);
			}
		}
		catch (Reuben::System::Exception e)
		{
			TRACE_ERRORDTL_9(GLOBAL_LOGGER, 
				_FE("exception, sessionId %s, pid %d:%d, pinfo %p, label %s, pcdType %d, objId %d, method %p, %s"),
				context.sessionId.GetString().c_str(), pcdId.srvId, pcdId.subId, pinfo,
				String(pinfo->label).c_str(), pinfo->pcdType, pinfo->psrc, pinfo->cbMethod,
				e.GetErrorMessage().c_str());
			TRACE_ERRORDTL_2(REUBEN_LOGGER, _FE("exception occurs at %s, %s"), GLOBAL_NAME,
				e.GetErrorMessage().c_str());
			bException = true;
		}
		catch (...)
		{
			TRACE_ERRORDTL_8(GLOBAL_LOGGER, 
				_FE("exception, sessionId %s, pid %d:%d, pinfo %p, label %s, pcdType %d, objId %d, method %p"),
				context.sessionId.GetString().c_str(), pcdId.srvId, pcdId.subId, pinfo,
				String(pinfo->label).c_str(), pinfo->pcdType, pinfo->psrc, pinfo->cbMethod);
			TRACE_ERRORDTL_1(REUBEN_LOGGER, _FE("exception occurs at %s"), GLOBAL_NAME);
			bException = true;
		}
		try
		{
			if (bException)
				EndReturn(pinfo, pcdId, PCD_ERR_EXCEPTION);
		}
		catch (...)
		{
			TRACE_ERRORDTL_8(GLOBAL_LOGGER, 
				_FE("double exception, sessionId %s, pid %d:%d, pinfo %p, label %s, pcdType %d, objId %d, method %p"),
				context.sessionId.GetString().c_str(), pcdId.srvId, pcdId.subId, pinfo,
				String(pinfo->label).c_str(), pinfo->pcdType, pinfo->psrc, pinfo->cbMethod);
		}
	}
	else if (pinfo->pcdType == ProcedureInfo::PCD_FUNC)
	{
		(*pinfo->cbFunc) (context);
	}
}

ProcedureInfo* ProcedureManager::NewRPC(LPCPEER pPeer, RPCContext &context, RPCID rpcid)
{
	ProcedureInfo *pinfo = NewProcedure(context);
	pinfo->pcdType = ProcedureInfo::PCD_RPC;
	pinfo->srcRpcId = rpcid;
	pinfo->pSrcPeer = (pPeer ? pPeer->AddRef() : NULL);

	return pinfo;
}

ProcedureInfo* ProcedureManager::NewMethod(RPCContext &context, Object *obj, 
										   RPCResult (Object::*callMethod) (RPCContext &))
{
	if (!obj)
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("ProcedureManager NewProcedureMethod Warning: object cannot null"));
		return NULL;
	}
	ProcedureInfo *pinfo = NewProcedure(context);
	pinfo->pcdType = ProcedureInfo::PCD_METHOD;
	pinfo->psrc = obj;
	pinfo->cbMethod = callMethod;
	m_methodSrcMap.insert(std::make_pair(obj, context.pcdId));

	return pinfo;
}

ProcedureInfo* ProcedureManager::NewFunc(RPCContext &context, 
										 RPCResult (*callFunc) (RPCContext &))
{
	ProcedureInfo *pinfo = NewProcedure(context);
	pinfo->pcdType = ProcedureInfo::PCD_FUNC;
	pinfo->cbFunc = callFunc;

	return pinfo;
}

ProcedureInfo* ProcedureManager::Get(const ProcedureID &pcdId) const
{
	ProcedureMap::const_iterator iter = m_pcdMap.find(pcdId);
	if (iter != m_pcdMap.end())
		return iter->second;
	else
		return NULL;
}

void ProcedureManager::Delete(ProcedureID pcdId)
{
	ProcedureMap::iterator iter = m_pcdMap.find(pcdId);
	if (iter != m_pcdMap.end()) {
		ProcedureInfo *pinfo = iter->second;
		if (pinfo)
		{
			SafeRelease(pinfo->pSrcPeer);
			SafeDeleteObject(pinfo->pSrcBuf);
			SafeDeleteObject(pinfo->pParamBuf);
			SafeDeleteObject(pinfo->pCountBuf);
			if (pinfo->pcdType == ProcedureInfo::PCD_METHOD)
				RemoveMethodSrc(pinfo->psrc, pcdId);
			SafeDeleteObject(pinfo);
			m_pcdMap.erase(pcdId);
		}
	}
}

void ProcedureManager::RemoveBySession(const SessionID &sessId)
{
	ProcedureIDQueue pcdList;
	ProcedureIDQueue::iterator it2;
	ProcedureInfo* pinfo;

	// use locking queue for EndReturn, clear all locking procedure to prevent normal resume
	LockerMap::iterator itfind = m_lockerMap.find(sessId);
	if (itfind != m_lockerMap.end())
	{
		ProcedureIDQueue& queue = itfind->second;
		queue.swap(pcdList);
	}

	if (!pcdList.empty())
		TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("Procedure remained for session %s: size %d"),
			sessId.GetString().c_str(), pcdList.size());

	// EndReturn error to remove procedure
	for (ProcedureIDQueue::iterator it = pcdList.begin(); it != pcdList.end(); ++it)
	{
		ProcedureMap::iterator iter = m_pcdMap.find(*it);
		if (iter != m_pcdMap.end()) {
			pinfo = iter->second;
			pinfo->lockState = PCD_NONE;
			EndReturn(pinfo, *it, PCD_ERR_REMOVED);
		}
	}
}

// remove all procedure by method source objects
void ProcedureManager::RemoveByObject(Object* obj)
{
	std::pair <MethodSrcMap::iterator, MethodSrcMap::iterator> range;
	range = m_methodSrcMap.equal_range(obj);
	ProcedureIDQueue pidQueue;

	// get all pcdId for that object first
	for (;range.first != range.second; ++range.first)
		pidQueue.push_back(range.first->second);
	m_methodSrcMap.erase(obj);		// remove all pcdId in map after got all pcdId

	// remove procedure one by one
	for (ProcedureIDQueue::iterator it = pidQueue.begin(); it != pidQueue.end(); ++it)
	{
		ProcedureID &pid = *it;
		ProcedureMap::iterator it2 = m_pcdMap.find(pid);

		// EndReturn error to remove procedure
		if (it2 != m_pcdMap.end()) {
			ProcedureInfo* pinfo = it2->second;
			EndReturn(pinfo, pid, PCD_ERR_REMOVED);
		}
	}
}

// remove pcdId in method source map
void ProcedureManager::RemoveMethodSrc(Object* obj, ProcedureID pcdId)
{
	std::pair <MethodSrcMap::iterator, MethodSrcMap::iterator> range;
	range = m_methodSrcMap.equal_range(obj);
	for (;range.first != range.second; ++range.first) {
		if (range.first->second == pcdId) {
			m_methodSrcMap.erase(range.first);
			break;
		}
	}
}

void ProcedureManager::CheckLockTimeout(const SessionID &sessId)
{
	// check if lock too long, then timeout fail the locked procedure
	bool bResume = false;
	LockerMap::iterator itfind = m_lockerMap.find(sessId);
	if (itfind != m_lockerMap.end())
	{
		ProcedureIDQueue& queue = itfind->second;
		if (!queue.empty())
		{
			UInt32 curTime = ::GetTickCount();
			ProcedureID pcdId = queue.front();
			ProcedureInfo* pinfo = Get(pcdId);
			if (pinfo)
			{
				if (pinfo->timeoutTime != 0 && pinfo->timeoutTime < curTime)
				{
					TRACE_WARNDTL_8(GLOBAL_LOGGER, 
						_T("CheckLockTimeout Warning: [lock too long], %s, sessionId %s, pcdId %d:%d, curTime %u, lockTime %u, timeoutFail %d, trapRpcId %d"),
						pinfo->label.c_str(), pinfo->origContext.sessionId.GetString().c_str(), 
						pcdId.srvId, pcdId.subId, curTime, pinfo->timeoutTime, pinfo->timeoutFail,
						pinfo->trapRpcId);
					// timeout fail to error
					//pinfo->trapRpcId = 0;
					//pinfo->pSubBuf = NULL;
					pinfo->errRpcId = PCD_ERR_TIMEOUT;
					WaitResume(pcdId);
					bResume = true;
				}
				else
				{
					TRACE_VERBOSEDTL_7(GLOBAL_LOGGER, 
						_T("CheckLockTimeout: [lock set to fail], %s, sessionId %s, pcdId %d:%d, curTime %u, lockTime %u, timeoutFail %d"),
						pinfo->label.c_str(), pinfo->origContext.sessionId.GetString().c_str(), 
						pcdId.srvId, pcdId.subId, curTime, pinfo->timeoutTime, pinfo->timeoutFail);
					if (pinfo->timeoutTime == 0)
						pinfo->timeoutTime = ::GetTickCount() + PCD_WAIT_TIMEOUT;
					pinfo->timeoutFail = BOOLEAN_TRUE;
				}
			}
		}
	}

#ifdef SERVICE
	if (bResume)
		OnUpdate(0);
#endif //SERVICE
}

// sub procedure return
void ProcedureManager::EndReturn(ProcedureInfo* &pinfo, ProcedureID pcdId, RPCID rpcId)
{
	if (pinfo == NULL)
		return;

	// if error or warning
	String errMsg;
	String warnMsg;
	bool errCb = false;

	switch (rpcId)
	{
	case PCD_ERR_GENERAL:
		//errMsg.Format(_T("[general error], errRpcId %d, trapRpcId %d"), pinfo->errRpcId, pinfo->trapRpcId);
		errCb = true;
		break;

	case PCD_ERR_STEP:
		errMsg.Format(_T("[wrong step %d]"), pinfo->step);
		errCb = true;
		break;

	case PCD_ERR_NOTFOUND:
		errMsg.Format(_T("[pcdId not found]"));
		errCb = true;
		break;

	case PCD_ERR_NETOBJ:
		errMsg.Format(_T("[NetObject not found]"));
		errCb = true;
		break;

	case PCD_ERR_TIMEOUT:
		warnMsg.Format(_T("[procedure timeout], timeoutTime %u"), pinfo->timeoutTime);
		if (pinfo->timeoutFail)
			errCb = true;
		break;

	case PCD_ERR_REMOVED:
		warnMsg.Format(_T("[pcdId removed]"));
		errCb = true;
		break;

	case PCD_ERR_EXCEPTION:
		warnMsg.Format(_T("[pcdId exception]"));
		errCb = true;
		break;

	case PCD_ERR_BUF_SRC:
		errMsg.Format(_T("[wrong buffer src], cursor %d, size %d, %s"), 
			pinfo->pSrcBuf->GetCursor(), pinfo->pSrcBuf->GetSize(), 
			String(_debugMsg.c_str()).c_str());
		errCb = true;
		break;

	case PCD_ERR_BUF_SUB:
		errMsg.Format(_T("[wrong buffer sub], cursor %d, size %d, %s"), 
			pinfo->pSubBuf ? pinfo->pSubBuf->GetCursor() : 0,
			pinfo->pSubBuf ? pinfo->pSubBuf->GetSize() : 0,
			String(_debugMsg.c_str()).c_str());
		errCb = true;
		break;

	case PCD_ERR_BUF_PARAM:
		errMsg.Format(_T("[wrong buffer param], cursor %d, size %d, %s"), 
			pinfo->pParamBuf ? pinfo->pParamBuf->GetCursor() : 0,
			pinfo->pParamBuf ? pinfo->pParamBuf->GetSize() : 0,
			String(_debugMsg.c_str()).c_str());
		errCb = true;
		break;

	case PCD_ERR_BUF_COUNT:
		errMsg.Format(_T("[wrong buffer counter], cursor %d, size %d, %s"), 
			pinfo->pCountBuf ? pinfo->pCountBuf->GetCursor() : 0,
			pinfo->pCountBuf ? pinfo->pCountBuf->GetSize() : 0,
			String(_debugMsg.c_str()).c_str());
		errCb = true;
		break;
	}

	if (!errMsg.IsEmpty())
	{
		TRACE_ERRORDTL_6(GLOBAL_LOGGER, 
			_T("Procedure Error: %s, %s, pcdId %d:%u, sessionId %s, step %d"),
			errMsg.c_str(), pinfo->label.c_str(), pcdId.srvId, pcdId.subId,
			pinfo->origContext.sessionId.GetString().c_str(), pinfo->step);
	}
	else if (!warnMsg.IsEmpty())
	{
		TRACE_WARNDTL_6(GLOBAL_LOGGER, 
			_T("Procedure Warning: %s, %s, pcdId %d:%u, sessionId %s, step %d"),
			warnMsg.c_str(), pinfo->label.c_str(), pcdId.srvId, pcdId.subId,
			pinfo->origContext.sessionId.GetString().c_str(), pinfo->step);
	}

	// callback ERR_RPCMANAGER RPC to source peer if it is waiting
	if (errCb && pinfo->pcdType == ProcedureInfo::PCD_RPC && pinfo->pSrcPeer &&
		pinfo->origContext.pcdId.srvId != 0 && 
		pinfo->origContext.pcdId.srvId != GetServiceId())
	{
		String msg = (errMsg.IsEmpty() ? warnMsg : errMsg);
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("Procedure Warning: send back error, %s, sessionId %s"),
			pinfo->label.c_str(), pinfo->origContext.sessionId.GetString().c_str());
		SendError(pinfo->pSrcPeer, pinfo->origContext, rpcId, msg);
	}

	if (pinfo->lockState != PCD_NONE)
	{
		// Unlock procedure and resume another lock waiting procedure or parent procedure
		Unlock(pinfo, pcdId, NULL);
	}

	// find parent procedure if have
	ProcedureInfo* parentPinfo = NULL;
	ProcedureID nextPcdId;
	if (pinfo->oldPcdId.srvId == GetServiceId() && pinfo->oldPcdId.subId != 0)
	{
		ProcedureMap::iterator iter = m_pcdMap.find(pinfo->oldPcdId);
		if (iter != m_pcdMap.end()) {
			parentPinfo = iter->second;

			// set parent pcd return if parent no need resume
			if (!parentPinfo->needResume)
				parentPinfo->trapRpcId = PCD_RETURN;
			// pass error rpcId back to parent (don't pass, will exception)
			//if (pinfo->errRpcId != 0)
			//	parentPinfo->errRpcId = pinfo->errRpcId;
			//// return rpcId not match
			//else if (parentPinfo->trapRpcId != 0 && parentPinfo->trapRpcId != rpcId)
			if (pinfo->errRpcId != 0 || errCb)
				parentPinfo->errRpcId = parentPinfo->trapRpcId;

			// set sub buf of child if error or return with parameters
			if (rpcId == PCD_RETURN) // || pinfo->errRpcId != 0)
				parentPinfo->pSubBuf = pinfo->pSubBuf;
			nextPcdId = pinfo->oldPcdId;
		}
	}

	// delete this procedure first
	SafeRelease(pinfo->pSrcPeer);
	SafeDeleteObject(pinfo->pSrcBuf);
	SafeDeleteObject(pinfo->pParamBuf);
	SafeDeleteObject(pinfo->pCountBuf);
	if (pinfo->pcdType == ProcedureInfo::PCD_METHOD)
		RemoveMethodSrc(pinfo->psrc, pcdId);
	SafeDeleteObject(pinfo);
	m_pcdMap.erase(pcdId);
	pinfo = NULL;
	pcdId.srvId = 0;
	pcdId.subId = 0;

	if (parentPinfo && rpcId != PCD_ERR_REMOVED)
	{
		if (parentPinfo->needResume)				// Resume parent procedure
			Resume(parentPinfo, nextPcdId);
		else									// just return with trapRpcId
			parentPinfo->trapRpcId = PCD_RETURN;
	}
}

NetObject* ProcedureManager::ReadNetObject(ProcedureInfo *pinfo)
{
	TRACE_ENSURE(pinfo);

	NetObject* netobj = NULL;
	RPCNetID netId;
	pinfo->pSubBuf->Read<RPCNetID>(netId);
	netobj = FindNetObject(netId);
	if (netobj == NULL)
	{
		TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("Procedure Error: NetObject %d:%d not found, %s, step %d"),
			netId.srvId, netId.objId, pinfo->label.c_str(), pinfo->step);
	}

	return netobj;
}

void ProcedureManager::ChangeSessionId(const SessionID& oldId, const SessionID& newId)
{
	// change procedure sessionId
	for (ProcedureMap::iterator it = m_pcdMap.begin(); it != m_pcdMap.end(); ++it)
	{
		ProcedureInfo* pinfo = it->second;
		if (pinfo->origContext.sessionId == oldId) {
			pinfo->origContext.sessionId = newId;
		}
	}

	// change locking map sesssionId
	LockerMap::iterator it2 = m_lockerMap.find(oldId);
	if (it2 != m_lockerMap.end()) {
		m_lockerMap.insert(std::make_pair(newId, it2->second));
		m_lockerMap.erase(oldId);
	}
}

Boolean ProcedureManager::IsSessionLocked(const SessionID& sid)
{
	LockerMap::iterator it = m_lockerMap.find(sid);

	if (it != m_lockerMap.end()) {
		ProcedureIDQueue& queue = it->second;
		if (!queue.empty())
			return BOOLEAN_TRUE;
	}

	return BOOLEAN_FALSE;
}

VOID ProcedureManager::OnUpdate(CONST UInt32 dt)
{
	// check any timeout procedure
	UInt32 curTime = ::GetTickCount();
	ProcedureInfo* pinfo;
	ProcedureIDQueue timeoutList;

	for (TimeoutMap::iterator it = m_timeoutMap.begin(); it != m_timeoutMap.end();)
	{
		if (curTime > it->first) {		// timeout time reached
			pinfo = Get(it->second);
			if (pinfo && pinfo->timeoutTime == it->first) {	// timeout happen
				timeoutList.push_back(it->second);
			}
			it = m_timeoutMap.erase(it);
		} else {						// shouldn't timeout for further
			break;
		}
	}

	ProcedureID pcdId;

	// return timeout event to procedure
	for (ProcedureIDQueue::iterator it = timeoutList.begin(); it != timeoutList.end(); ++it)
	{
		pcdId = *it;

		pinfo = Get(pcdId);
		if (pinfo && pinfo->trapRpcId == PCD_ERR_TIMEOUT)
		{
			WaitResume(pcdId);
		}
		else if (pinfo && pinfo->errRpcId == 0)
		{
			if (pinfo->timeoutFail)
			{
				// timeout fail to error
				pinfo->errRpcId = PCD_ERR_TIMEOUT;
				pinfo->pSubBuf = NULL;
				WaitResume(pcdId);
			}
			else
			{
				// timeout just give warning
				TRACE_WARNDTL_5(GLOBAL_LOGGER, _F("Procedure Warning: timeoutTime %u, %s, sessionId %s, step %d, psrc %d"),
					pinfo->timeoutTime, pinfo->label.c_str(), pinfo->origContext.sessionId.GetString().c_str(), 
					pinfo->step, pinfo->psrc);
				//pinfo->timeoutTime = 0;
			}
		}
	}

#ifdef SERVICE
	if (!m_resumeSet.empty())
	{
		// resume waiting procedure
		ProcedureIDSet resumeSet;

		resumeSet.swap(m_resumeSet);		// swap resume set to prevent new wait resume
		for (ProcedureIDSet::iterator it = resumeSet.begin(); it != resumeSet.end(); ++it)
		{
			pcdId = *it;
			pinfo = Get(pcdId);
			if (pinfo)
				Resume(pinfo, pcdId);
		}
	}
#endif // SERVICE
}

void ProcedureManager::AddTimeout(ProcedureInfo* pinfo, ProcedureID pcdId)
{
	if (pinfo == NULL)
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("Procedure AddTimeout Error: invalid procedure"));
		return;
	}

	// reset parent procedure timeout if child set timeout
	if (pinfo->oldPcdId.srvId == GetServiceId() && pinfo->oldPcdId.subId != 0)
	{
		ProcedureInfo* parentPinfo = Get(pinfo->oldPcdId);
		if (parentPinfo)
			parentPinfo->timeoutTime = 0;
	}

	if (_timeout != 0)
	{
		pinfo->timeoutTime = ::GetTickCount() + _timeout;
		pinfo->timeoutFail = _timeoutFail;
		m_timeoutMap.insert(std::make_pair(pinfo->timeoutTime, pcdId));
	}
	// reset timeout values
	_timeout = PCD_WAIT_TIMEOUT;
	_timeoutFail = PCD_DEFAULT_TIMEOUT_FAIL;
}

void ProcedureManager::Lock(ProcedureInfo *pinfo, ProcedureID pcdId, const SessionID* psid,
							Boolean bFront)
{
	if (pinfo == NULL)
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("Procedure Lock Error: invalid procedure or sessionID"));
		return;
	}

	if (pinfo->lockState != PCD_NONE)			// don't do double locking
		return;

	SessionID sid = pinfo->origContext.sessionId;
	if (psid)
		sid = *psid;

	if (sid.serviceId == INVALID_SERVICE_ID)	// no locking for invalid service id
		return;

	ProcedureIDQueue& queue = m_lockerMap[sid];

	if (queue.empty())
	{
		queue.push_back(pcdId);
		pinfo->lockState = PCD_LOCKED;
		m_lockPcdId = pcdId;
		AddTimeout(pinfo, pcdId);
		TRACE_VERBOSEDTL_7(GLOBAL_LOGGER, 
			_T("Procedure Locked: %s, sessionId %s, pcdId %d:%d, curTime %u, lockTime %u, timeoutFail %d"),
			pinfo->label.c_str(), sid.GetString().c_str(), pcdId.srvId, pcdId.subId,
			::GetTickCount(), pinfo->timeoutTime, pinfo->timeoutFail);
	}
	else
	{
		// child lock
		if (pinfo->oldPcdId == queue.front())
		{
			ProcedureInfo* parentPinfo = Get(pinfo->oldPcdId);
			if (parentPinfo)
			{
				TRACE_VERBOSEDTL_9(GLOBAL_LOGGER, 
					_T("Procedure Parent: %s, sessionId %s, pcdId %d:%d, oldPcdId %d:%d, curTime %u, lockTime %u, timeoutFail %d"),
					parentPinfo->label.c_str(), sid.GetString().c_str(), 
					pinfo->oldPcdId.srvId, pinfo->oldPcdId.subId,
					parentPinfo->oldPcdId.srvId, parentPinfo->oldPcdId.subId, 
					::GetTickCount(), parentPinfo->timeoutTime, parentPinfo->timeoutFail);
			}
			queue.push_front(pcdId);
			pinfo->lockState = PCD_LOCKED;
			m_lockPcdId = pcdId;
			AddTimeout(pinfo, pcdId);
			TRACE_VERBOSEDTL_9(GLOBAL_LOGGER, 
				_T("Procedure Child Locked: %s, sessionId %s, pcdId %d:%d, oldPcdId %d:%d, curTime %u, lockTime %u, timeoutFail %d"),
				pinfo->label.c_str(), sid.GetString().c_str(), pcdId.srvId, pcdId.subId,
				pinfo->oldPcdId.srvId, pinfo->oldPcdId.subId, ::GetTickCount(), pinfo->timeoutTime, 
				pinfo->timeoutFail);
		}
		else // new lock
		{
			int qpos;
			if (bFront)
			{
				queue.insert(queue.begin() + 1, pcdId);	// insert at second position
				qpos = 2;
			}
			else
			{
				queue.push_back(pcdId);					// insert at last position
				qpos = (int) queue.size();
			}
			pinfo->lockState = PCD_LOCK_WAITING;
			pinfo->needResume = BOOLEAN_TRUE;
			TRACE_VERBOSEDTL_6(GLOBAL_LOGGER, _F("Procedure Lock Waiting: %s, sessionId %s, pcdId %d:%d, qpos %d/%d"),
				pinfo->label.c_str(), sid.GetString().c_str(), pcdId.srvId, pcdId.subId,
				qpos, queue.size());

			if (queue.size() == PCD_LOCKQUEUE_WARN)
			{
				TRACE_WARNDTL_6(GLOBAL_LOGGER, 
					_T("Procedure Lock Warning: [too many locks], %s, sessionId %s, pcdId %d:%d, qpos %d/%d"),
					pinfo->label.c_str(), sid.GetString().c_str(), pcdId.srvId, pcdId.subId,
					qpos, queue.size());
			}
		}
	}
}

void ProcedureManager::Unlock(ProcedureInfo *pinfo, ProcedureID pcdId, const SessionID* psid)
{
	if (pinfo == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Procedure Unlock Error: procedure info or procedure id NULL"));
		return;
	}

	SessionID sid = pinfo->origContext.sessionId;
	if (psid)
		sid = *psid;

	if (pinfo->lockState == PCD_NONE) // no locking
	{
		TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("Procedure Unlock Error: [procedure not locked], %s, sessionId %s, pcdId %d:%d"),
			pinfo->label.c_str(),sid.GetString().c_str(), pcdId.srvId, pcdId.subId);
		return;
	}

	LockerMap::iterator itfind = m_lockerMap.find(sid);
	if (itfind == m_lockerMap.end())
	{
		TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("Procedure Unlock Error: [session no lock queue], %s, sessionId %s, pcdId %d:%d"),
			pinfo->label.c_str(),sid.GetString().c_str(), pcdId.srvId, pcdId.subId);
		return;
	}

	ProcedureIDQueue& queue = itfind->second;

	// just release lock if lock waiting
	if (pinfo->lockState == PCD_LOCK_WAITING)
	{
		TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("Procedure Unlock Release: %s, sessionId %s, pcdId %d:%d, step %d"),
			pinfo->label.c_str(), sid.GetString().c_str(), pcdId.srvId, pcdId.subId, pinfo->step);
		pinfo->lockState = PCD_NONE;
		m_lockPcdId.srvId = 0;
		m_lockPcdId.subId = 0;
			
		ProcedureIDQueue::iterator it = std::find(queue.begin(), queue.end(), pcdId);
		if (it != queue.end())
			queue.erase(it);
		return;
	}

	if (queue.empty())
	{
		TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("Procedure Unlock Error: [session queue is empty], %s, sessionId %s, pcdId %d:%d"),
			pinfo->label.c_str(), sid.GetString().c_str(), pcdId.srvId, pcdId.subId);
		return;
	}

	bool atFront = true;
	if (queue.front() == pcdId)
	{
		// try to remove from queue front
		queue.pop_front();
	}
	else
	{
		// try to remove from queue at other place
		TRACE_ERRORDTL_6(GLOBAL_LOGGER, _F("Procedure Unlock Error: [not at queue front], %s, sessionId %s, pcdId %d:%d, queue.front() %d:%d"),
			pinfo->label.c_str(), sid.GetString().c_str(), pcdId.srvId, pcdId.subId, 
			queue.front().srvId, queue.front().subId);
		ProcedureIDQueue::iterator it = std::find(queue.begin(), queue.end(), pcdId);
		if (it != queue.end())
		{
			queue.erase(it);
			atFront = false;
		}
		else
		{
			TRACE_ERRORDTL_6(GLOBAL_LOGGER, _F("Procedure Unlock Error: [procedure id not match], %s, sessionId %s, pcdId %d:%d, queue.front() %d:%d"),
				pinfo->label.c_str(),sid.GetString().c_str(), pcdId.srvId, pcdId.subId, 
				queue.front().srvId, queue.front().subId);
			return;
		}
	}

	TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("Procedure Unlock Successful: %s, sessionId %s, pcdId %d:%d, step %d"),
		pinfo->label.c_str(), sid.GetString().c_str(), pcdId.srvId, pcdId.subId, pinfo->step);
	pinfo->lockState = PCD_NONE;
	m_lockPcdId.srvId = 0;
	m_lockPcdId.subId = 0;

	if (queue.size() > 0)
	{
		ProcedureID frontPid = queue.front();
		ProcedureInfo* pinfoResume = Get(frontPid);

		if (frontPid.subId == 0 || pinfoResume == NULL) {
			TRACE_ERRORDTL_6(GLOBAL_LOGGER, _F("Procedure Unlock Error: [invalid procedure id], %s, sessionId %s, pcdId %d:%d, queue.front() %d:%d"),
				pinfo->label.c_str(), sid.GetString().c_str(), pcdId.srvId, pcdId.subId, 
				frontPid.srvId, frontPid.subId);
			// remove deleted or invalid procedure id
			while (queue.size() > 0)
			{
				frontPid = queue.front();
				if (frontPid.subId != 0 && (pinfoResume = Get(frontPid)) != NULL)
					break;
				queue.pop_front();
			}
		}

		if (pinfoResume == NULL)
		{
			TRACE_ERRORDTL_5(GLOBAL_LOGGER, _F("Procedure Resume Error: [procedure cannot find info data], %s, sessionId %s, queue.front() %d:%d, step %d"),
				pinfo->label.c_str(), sid.GetString().c_str(), frontPid.srvId, frontPid.subId, 
				pinfo->step);
			return;
		}

		if (pinfoResume->lockState == PCD_NONE)
		{
			TRACE_ERRORDTL_5(GLOBAL_LOGGER, _F("Procedure Resume Error: [procedure in lock none state], %s, sessionId %s, step %d, queue.front() %d:%d"),
				pinfoResume->label.c_str(), sid.GetString().c_str(), pinfoResume->step, 
				frontPid.srvId, frontPid.subId);
			return;
		}

		pinfoResume->lockState = PCD_LOCKED;
		m_lockPcdId = frontPid;
		if (frontPid != pinfo->oldPcdId && 	// no need resume here for parent procedure
			(atFront || pinfoResume->oldPcdId != pcdId))
			WaitResume(frontPid);
	}
	else
	{
		m_lockerMap.erase(itfind);
	}
}
