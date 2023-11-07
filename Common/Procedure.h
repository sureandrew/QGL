/********************************************************************
			PROCEDURE Macro Documentation
			=============================
			Written by Jacky Cheung

Description:
	Procedure macro is used for a very convenient way of cluster server side 
	programming.  It is ONLY used when one service is calling another service 
	RPC function or method and need to wait that service RPC callback.

!!!Warning!!!
	Although Procedure macro provide a very convenient way for cluster server side
	programming, any wrong structure or sequence will make the compiler failed 
	to compile and generate a numerical syntax errors. Therefore, please 
	understand well about how to use the procedure before using.

Procedure Block Structure:
	Without Exception Handling:
		PROCEDURE_START...
			<code here>
		PROCEDURE_WAIT...(1, ...)		// step 1
			<code here>
		...
		PROCEDURE_WAIT...(N, ...)		// step N
			<code here>
		PROCEDURE_END

	With Exception Handling:
		PROCEDURE_START...
			<code here>
		PROCEDURE_WAIT...(1, ...)		// step 1
			<code here>
		...
		PROCEDURE_WAIT...(N, ...)		// step N
			<code here>
		PROCEDURE_CATCH
			PROCEDURE_EXCEPTION...
				<code here>
			...
			PROCEDURE_EXCEPTION...
				<code here>
		PROCEDURE_END_CATCH

Starting Declaration:
	There are three types of declarating procedure starting block:
	1. Inside RPC Function/Method
		PROCEDURE_START_RPC[1-9](_class_, _method_, _netobj_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- _class_ is the RPC class name defined in IDL
		- _method_ is the function or method name defined in IDL
		- _netobj_ is the NetObject* for this method, usually is "this" or "NULL" for RPC function

		PROCEDURE_DO_RPC[1-9](_class_, _method_, _netobj_, _peer_, _context_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- is used inside any function or method and it will call that RPC method afterward
		- _class_ is the local class name
		- _method_ is the calling class method name
		- _netobj_ is the NetObject* for this method, usually is "this" or "NULL" for RPC function
		- _peer_ is the Peer provided
		- _context_ is the context provided

	2. Inside Local Class Method
		- Because procedure is using function pointer for doing callback, this
		  require the class inherited Reuben::Simulation::Object and have 
		  a method of RPCResult _class_::method_(RPCContext&).

		PROCEDURE_DO_METHOD[1-9](_class_, _method_, _pobj_, _context_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- is used inside any function or method and it will call _class_::_method_ afterward
		- _class_ is the local class name
		- _method_ is the calling class method name
		- _pobj_ is the Object* for this method, usually is "this"
		- _context is the context provided

		PROCEDURE_START_METHOD[1-9](_class_, _method_, _pobj_, _context_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- is ONLY used inside RPCResult _class_::method_(RPCContext&)
		- same parameters in PROCEDURE_DO_METHOD and similiar usage of 
		  PROCEDURE_START_RPC

	3. Inside Local Function
		- similiar to local method which require a function of
		  RPCResult _func_(RPCContext&).

		([1-9](_func_, _context_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- is used inside any function or method and it will call _func_ afterward
		- _func_ is the local function (include namespace if necessary)
		- _context is the context provided

		PROCEDURE_START_FUNC[1-9](_func_, _context_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- is ONLY used inside RPCResult _func_(RPCContext&)
		- same parameters in PROCEDURE_DO_FUNC and similiar usage of 
		  PROCEDURE_START_RPC

Waiting Next Step:
	Waiting macro is used after call other service RPC code and define
	another procedure step when that service RPC callback.  There are
	two types of waiting macro:
	1. Waiting RPC callback function
		PROCEDURE_WAIT[1-9](_num_, _class_, _method_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- _num_ is the defined procedure step, must be distinct and in
		  1, 2, 3... real number sequence
		- _class_ is the callback RPC namespace name defined in IDL
		- _method_ is the callback RPC function name defined in IDL

	2. Waiting RPC callback method
		PROCEDURE_WAIT_OBJ[1-9](_num_, _class_, _method_, _pobj_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- _num_ is the defined procedure step, must be distinct and in
		  1, 2, 3... real number sequence
		- _class_ is the callback RPC class name defined in IDL
		- _method_ is the callback RPC method name defined in IDL
		- _pobj_ is the callback RPC NetObject* object

	3. Waiting Local Sub Procedure
		PROCEDURE_WAIT_RETURN[1-9]([_type1_, _value1_ ... _typeN_, _valueN_])
		- wait local sub procedure return with some values
		- must be call the local sub procedure using PROCEDURE_DO_METHOD or PROCEDURE_DO_FUNC
		- _num_ is the defined procedure step, must be distinct and in
		  1, 2, 3... real number sequence

Sub Procedure Return:
	After local sub procedure finish and wait to return some values to parent
	procedure, you can use PROCEDURE_RETURN.
	1. PROCEDURE_RETURN[1-9]([_type1_, _value1_ ... _typeN_, _valueN_])

Ending Step:
	Procedure macro block must provided ending macro after starting macro.
	You can ending the macro block with or without exception handling.
	1. Without Exception Handling:
		PROCEDURE_END
	2. With Exception Handling:
		PROCEDURE_CATCH
			...
		PROCEDURE_END_CATCH

Error Return:
	When some logic error happen inside procedure block and need to immediately
	return from that function/method, use PROCEDURE_ERROR_RETURN instead just
	"return".
	1. PROCEDURE_ERROR_RETURN(_errcode_)
		- return with error code _errcode_

Exception Handling:
	Procedure exception handling is used for the other service RPC callback
	is unexpected error happened during waiting step.  The procedure
	exception macro must defined inside PROCEDURE_CATCH and 
	PROCEDURE_END_CATCH block. Similiar to Waiting Step, there are two
	types of Exception Handling macro:
	1. PROCEDURE_EXCEPTION[1-9](_class_, _method_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- _class_ is the callback RPC namespace name defined in IDL
		- _method_ is the callback RPC function name defined in IDL

	2. PROCEDURE_EXCEPTION_OBJ[1-9](_class_, _method_, _pobj_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- _class_ is the callback RPC class name defined in IDL
		- _method_ is the callback RPC method name defined in IDL
		- _pobj_ is the callback RPC NetObject* object

Flow Control:
	Procedure macro provide branching when waiting different other service
	RPC callback for some situations.
	1. PROCEDURE_WAIT_STEP(_num_, _class_, _method_)
		- will jump to wait callback for step _num_ instead of following step
		- _class_ is the callback RPC class name defined in IDL
		- _method_ is the callback RPC method name defined in IDL
	2. PROCEDURE_GO_STEP[1-9](_num_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- directly jump to step _num_ for waiting RPC callback function 
		  and provided necessary parameters
	3. PROCEDURE_GO_STEP_OBJ[1-9](_num_, _pobj_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- directly jump to step _num_ for waiting RPC callback method
		  and provided necessary parameters
		- _pobj_ is the callback RPC NetObject* object
	4. PROCEDURE_BREAK
		- break out the procedure
	5. PROCEDURE_FOR[1-5](_num_, _endnum_, _cond_, [_type1_, _value1_ ... _typeN_, _valueN_])
		- provide loop iteration with counter values
		- _num_ is the PROCEDURE_FOR step number
		- _cond_ is the condition for the loop to do
		- _endnum_ is the PROCEDURE_END_FOR step number to go when cond fail
	6. PROCEDURE_END_FOR[1-5](_num_, _fornum_, [_type1_, _value1_ ... _typeN_, _valueN_])
	    - pair up with PROCEDURE_FOR to provide a loop iteration with counter values
		- _num_ is the PROCEDURE_END_FOR step number
		- _fornum_ is the PROCEDURE_FOR step number to go back
	7. PROCEDURE_STORE_COUNTER[1-5]([_type1_, _value1_ ... _typeN_, _valueN_])
		- store counters after counters changed
	8. PROCEDURE_EXIT_FOR(_endnum_)
		- exit for loop and go to PROCEDURE_END_FOR step
		- _endnum_ is the PROCEDURE_END_FOR step number to go
	9. PROCEDURE_LABEL(_num_)
		- just a label step for jumping to

Utility Macro:
	1. PROCEDURE_GET_STEP
		- get the current step value
	2. PROCEDURE_STORE_VALUE[1-9]([_type1_, _value1_ ... _typeN_, _valueN_])
		- the procedure local variables inside step cannot be in other step.
		  When need to do so, use procedure local store/restore to do it.
		- _type_ is the local variable type
		- _value_ is the local variable name
	3. PROCEDURE_RESTORE_VALUE[1-9]([_type1_, _value1_ ... _typeN_, _valueN_])
		- restore variable must in the same sequence of storing variable
		- _type_ is the local variable type
		- _value_ is the local variable name
	4. PROCEDURE_RESTORE_CONTEXT
		- restore original context when first calling RPC functions

					=== THE END ===
*********************************************************************/

#pragma once
#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#define PCD_TIMEOUT_WARNONLY			// timeout just warning and reset timeout clock
#ifdef PCD_TIMEOUT_WARNONLY
	#define PCD_DEFAULT_TIMEOUT_FAIL BOOLEAN_FALSE
#else // PCD_TIMEOUT_WARNONLY
	#define PCD_DEFAULT_TIMEOUT_FAIL BOOLEAN_TRUE
#endif // PCD_TIMEOUT_WARNONLY

#define PCD_RETURN			1

#define ERR_RPCMANAGER		2
#define PCD_ERR_GENERAL		3
#define PCD_ERR_STEP		4
#define PCD_ERR_NOTFOUND	5
#define PCD_ERR_NETOBJ		6
#define PCD_ERR_TIMEOUT		7
#define PCD_ERR_REMOVED		8
#define PCD_ERR_EXCEPTION	9
#define PCD_ERR_BUF_SRC		10
#define PCD_ERR_BUF_SUB		11
#define PCD_ERR_BUF_PARAM	12
#define PCD_ERR_BUF_COUNT	13

enum LockType
{
	PCD_NONE = 0,
	PCD_LOCKED = 1,
	PCD_LOCK_WAITING = 2
};

struct ProcedureInfo
{
	enum PcdType {
		PCD_RPC = 1,
		PCD_METHOD = 2,
		PCD_FUNC = 3
	};
	UInt8		pcdType;		// pcdType : 1 = RPC, 2 = local Method, 3 = local Func
	UInt8		step;			// current procedure step
	union {
		struct {
			RPCID		srcRpcId;		// main RPC ID
			Boolean		singleRpc;		// single step procedure RPC function?
		};
		struct {
			Object *psrc;					// callback source object
			RPCResult (Object::*cbMethod) (RPCContext &);	// callback source method
		};
		RPCResult (*cbFunc) (RPCContext &);				// callback source function
	};
	RPCContext	origContext;	// original context
	RPCID		trapRpcId;		// trapped RPC ID
	RPCID		errRpcId;		// error RPC ID
	LPCPEER		pSrcPeer;		// source peer
	Buffer		*pSrcBuf;		// buffer from main RPC call
	Buffer		*pSubBuf;		// buffer from other RPC call
	Buffer		*pParamBuf;		// buffer for store and restore some variables
	Buffer		*pCountBuf;		// buffer for looping counters
	ProcedureID	oldPcdId;		// old procedure ID for return
	UInt32		timeoutTime;	// timeout time
	UInt8		lockState;		// procedure locked?
	Boolean		needResume;		// procedure need resume parent
	Boolean		timeoutFail;	// will timeout cause procedure exception?
	String		label;			// procedure label for debugging

	ProcedureInfo() : step(0), pcdType(0), srcRpcId(0), singleRpc(BOOLEAN_FALSE), 
		cbFunc(NULL), trapRpcId(0), errRpcId(0), pSrcPeer(NULL), pSubBuf(NULL), 
		pParamBuf(NULL), pCountBuf(NULL), timeoutTime(0), lockState(0), 
		needResume(BOOLEAN_FALSE), timeoutFail(BOOLEAN_FALSE)
	{
	}
};

class ProcedureManager : 
	public Reuben::Utility::Process,
	public Reuben::Utility::Singleton<ProcedureManager>
{
public:
	const ServiceID GetServiceId() const;
	ProcedureInfo* NewRPC(LPCPEER pPeer, RPCContext &context, RPCID rpcid);
	ProcedureInfo* NewMethod(RPCContext &context, Object *obj, 
								RPCResult (Object::*callMethod) (RPCContext &));
	ProcedureInfo* NewFunc(RPCContext &context, RPCResult (*callFunc) (RPCContext &));
	NetObject* ReadNetObject(ProcedureInfo* pinfo);
	ProcedureInfo* Get(const ProcedureID &pcdId) const;
	void Delete(ProcedureID pcdId);
	void RemoveBySession(const SessionID &sessId);
	void RemoveByObject(Object* obj);
	void CheckLockTimeout(const SessionID &sessId);
	void EndReturn(ProcedureInfo* &pinfo, ProcedureID pcdId, RPCID rpcId);
	RPCResult DoProcess(RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	void ChangeSessionId(const SessionID& oldId, const SessionID& newId);
	Boolean IsSessionLocked(const SessionID &sessId);
	void AddTimeout(ProcedureInfo* pinfo, ProcedureID pcdId);
	void Lock(ProcedureInfo *pinfo, ProcedureID pcdId, const SessionID* psid = NULL, 
				Boolean bFront=BOOLEAN_FALSE);
	void Unlock(ProcedureInfo *pinfo, ProcedureID pcdId, const SessionID* psid = NULL);
	void SetLocking(ProcedureID& pcdId) { m_lockPcdId = pcdId; }
	void SetNonBlock(Boolean bEnable) { m_bNonBlock = bEnable; }
	INLINE Boolean GetNonBlock()
	{
		if (m_bNonBlock)
		{
			m_bNonBlock = BOOLEAN_FALSE;
			return BOOLEAN_TRUE;
		}
		else
			return BOOLEAN_FALSE;
	}
	void SetFrontLock(Boolean bEnable) { m_bFrontLock = bEnable; }
	INLINE Boolean GetFrontLock()
	{
		if (m_bFrontLock)
		{
			m_bFrontLock = BOOLEAN_FALSE;
			return BOOLEAN_TRUE;
		}
		else
			return BOOLEAN_FALSE;
	}

	virtual VOID OnUpdate(CONST UInt32 dt);
	void CancelResume(ProcedureID pcdId);					// cancel wait resume
	Buffer* GetTempBuf() { m_tmpBuf.ResetCursor(); return &m_tmpBuf; }
	bool IsTempBuf(Buffer* buf) { return (&m_tmpBuf == buf); }
	static UInt32 _timeout;
	static bool _timeoutFail;
	static StringA _debugMsg;

private:
	typedef StlMap<ProcedureID, ProcedureInfo*, std::less<ProcedureID>, 
		ManagedAllocator<std::pair<ProcedureID, ProcedureInfo*> > > ProcedureMap;
	typedef StlDeque<ProcedureID, ManagedAllocator<ProcedureID> > ProcedureIDQueue;
	typedef StlSet<ProcedureID, std::less<ProcedureID>, 
		ManagedAllocator<ProcedureID> > ProcedureIDSet;
	typedef StlMap<SessionID, ProcedureIDQueue, std::less<SessionID>, 
		ManagedAllocator<std::pair<SessionID, ProcedureIDQueue> > > LockerMap;
	typedef StlMultiMap<UInt32, ProcedureID, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, ProcedureID> > > TimeoutMap;
	typedef StlMultiMap<Object*, ProcedureID, std::less<Object*>, 
		ManagedAllocator<std::pair<Object*, ProcedureID> > > MethodSrcMap;

	ServiceID		m_srvId;			// network service id
	Reuben::Network::SubID	m_nextPcdId;		// next Procedure Id
	ProcedureID		m_lockPcdId;		// last lock procedure Id
	ProcedureMap	m_pcdMap;			// procedure map
	LockerMap		m_lockerMap;		// procedure locker map
	TimeoutMap		m_timeoutMap;		// procedure timeout map
	MethodSrcMap	m_methodSrcMap;		// method source map
	ProcedureIDSet	m_resumeSet;		// procedure ID wait for resume
	Buffer			m_tmpBuf;			// temp buffer for jump step use
	Boolean			m_bNonBlock;		// temporary non block procedure for no locking
	Boolean			m_bFrontLock;		// temporary put sub procedure to 

	SINGLETON(ProcedureManager);
	ProcedureManager();
	virtual ~ProcedureManager();
	ProcedureID GetNextPcdId();
	ProcedureInfo* NewProcedure(RPCContext &context);
	void WaitResume(ProcedureID pcdId);					// wait for resume
	void Resume(ProcedureInfo *pinfo, ProcedureID pcdId);	// resume running procedure
	void RemoveMethodSrc(Object* obj, ProcedureID pcdId);	// remove pcdId in method src map
};

#define PROCEDURE_GET_STEP					pinfo->step

#define PROCEDURE_SET_NONBLOCK				\
	ProcedureManager::GetInstance().SetNonBlock(BOOLEAN_TRUE);

#define PROCEDURE_SET_FRONTLOCK				\
	ProcedureManager::GetInstance().SetFrontLock(BOOLEAN_TRUE);

#define PROCEDURE_DO_RPC_HEAD(_class_, _method_, _netObj_, _peer_, _context_)	\
	{																		\
		ProcedureManager &pcmgr = ProcedureManager::GetInstance();			\
		RPCContext _newContext(_context_.sessionId, _context_.pcdId);		\
		ProcedureInfo *pinfo = pcmgr.NewRPC(_peer_, _newContext, RPC_##_class_##_##_method_);	\
		ProcedureID pcdId = _newContext.pcdId;								\
		Boolean _nonBlock = pcmgr.GetNonBlock();							\
		Boolean _frontLock = pcmgr.GetFrontLock();							\
		TRACE_ENSURE(pinfo != NULL);										\
		pinfo->label.Format(_T("RPC C%s::%s(), ID=%d"), _T(#_class_), _T(#_method_), _netObj_);	\
		pinfo->singleRpc = BOOLEAN_TRUE;									\
		if (_netObj_ != NULL) {												\
			NetObject *netobj = _netObj_;									\
			pinfo->pSrcBuf->Write<RPCNetID>(netobj->GetNetId());			\
		}

#define PROCEDURE_DO_RPC_TAIL(_peer_, _context_)							\
		if (!_nonBlock) pcmgr.Lock(pinfo, pcdId, NULL, _frontLock);			\
		if (pinfo->lockState != PCD_LOCK_WAITING) {							\
			pcmgr.DoProcess(pinfo->srcRpcId, _peer_, _newContext, pinfo->pSrcBuf);	\
			pinfo = pcmgr.Get(pcdId);										\
			if (pinfo && pinfo->singleRpc)									\
				pcmgr.EndReturn(pinfo, pcdId, PCD_RETURN);					\
		}																	\
	}

#define PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)				\
	ProcedureManager &pcmgr = ProcedureManager::GetInstance();				\
	Boolean _nonBlock = pcmgr.GetNonBlock();								\
	ProcedureInfo *pinfo = NULL;											\
	bool _caseAgain = true;													\
	Size _subMsgCur = REUBEN_NETWORK_BUFFER_HEADER_SIZE;					\
	ProcedureID pcdId = context.pcdId;										\
	if (pcdId.srvId == pcmgr.GetServiceId() && pcdId.subId != 0) {			\
		pinfo = pcmgr.Get(pcdId);											\
		if (pinfo == NULL) {												\
			pcmgr.EndReturn(pinfo, pcdId, PCD_ERR_NOTFOUND);				\
			_caseAgain = false;												\
		}																	\
		if (pinfo->step == 0) {												\
			pinfo->label.Format(_T("RPC C%s::%s(), ID=%d"), _T(#_class_), _T(#_method_), _netObj_);	\
			pinfo->singleRpc = BOOLEAN_FALSE;								\
		}																	\
	} else {																\
		pinfo = pcmgr.NewRPC(pPeer, context, RPC_##_class_##_##_method_);	\
		pcdId = context.pcdId;												\
		TRACE_ENSURE(pinfo != NULL);										\
		pinfo->label.Format(_T("RPC C%s::%s(), ID=%d"), _T(#_class_), _T(#_method_), _netObj_);	\
		if (_netObj_ != NULL) {												\
			NetObject *netobj = _netObj_;									\
			pinfo->pSrcBuf->Write<RPCNetID>(netobj->GetNetId());			\
		}

#define PROCEDURE_START_RPC_TAIL											\
		if (!_nonBlock) pcmgr.Lock(pinfo, pcdId);							\
		if (pinfo->lockState == PCD_LOCK_WAITING) {							\
			_caseAgain = false;												\
		}																	\
	}

#define PROCEDURE_DO_METHOD_HEAD(_class_, _method_, _pobj_, _context_)		\
	{																		\
		ProcedureManager &pcmgr = ProcedureManager::GetInstance();			\
		Boolean _nonBlock = pcmgr.GetNonBlock();							\
		Boolean _frontLock = pcmgr.GetFrontLock();							\
		RPCContext _newContext(_context_.sessionId, _context_.pcdId);		\
		RPCResult (Object::*_callMethod) (RPCContext &) =					\
		static_cast<RPCResult (Object::*) (RPCContext &)>(&_class_::_method_);	\
		ProcedureInfo *pinfo = pcmgr.NewMethod(_newContext, _pobj_, _callMethod);	\
		TRACE_ENSURE(pinfo != NULL);										\
		pinfo->label.Format(_T("METHOD %s::%s(), ID=%d"), _T(#_class_), _T(#_method_), _pobj_ ? _pobj_ : 0);

#define PROCEDURE_DO_METHOD_TAIL(_class_, _method_, _pobj_, _context_)		\
		if (!_nonBlock) pcmgr.Lock(pinfo, _newContext.pcdId, NULL, _frontLock);	\
		if (pinfo->lockState != PCD_LOCK_WAITING) {							\
			_pobj_->_method_(_newContext);									\
		}																	\
	}

#define PROCEDURE_START_METHOD_HEAD(_class_, _method_, _pobj_, _context_)	\
	ProcedureManager &pcmgr = ProcedureManager::GetInstance();				\
	bool _caseAgain = true;													\
	Size _subMsgCur = REUBEN_NETWORK_BUFFER_HEADER_SIZE;					\
	ProcedureID pcdId = _context_.pcdId;									\
	ProcedureInfo *pinfo = pcmgr.Get(pcdId);								\
	if (pinfo == NULL) {													\
		pcmgr.EndReturn(pinfo, pcdId, PCD_ERR_NOTFOUND);					\
		_caseAgain = false;													\
		return RPC_RESULT_FAIL;												\
	}																		\
	pinfo->pSrcBuf->ResetCursor();

#define PROCEDURE_DO_FUNC_HEAD(_func_, _context_)							\
	{																		\
		ProcedureManager &pcmgr = ProcedureManager::GetInstance();			\
		Boolean _nonBlock = pcmgr.GetNonBlock();							\
		Boolean _frontLock = pcmgr.GetFrontLock();							\
		RPCContext _newContext(_context_.sessionId, _context_.pcdId);		\
		ProcedureInfo *pinfo = pcmgr.NewFunc(_newContext, &_func_);			\
		TRACE_ENSURE(pinfo != NULL);										\
		pinfo->label.Format(_T("FUNC %s()"), _T(#_func_));

#define PROCEDURE_DO_FUNC_TAIL(_func_, _context_)							\
		if (!_nonBlock) pcmgr.Lock(pinfo, _newContext.pcdId, NULL, _frontLock);	\
		if (pinfo->lockState != PCD_LOCK_WAITING) {							\
			_func_(_newContext);											\
		}																	\
	}

#define PROCEDURE_START_FUNC_HEAD(_func_, _context_)						\
	ProcedureManager &pcmgr = ProcedureManager::GetInstance();				\
	bool _caseAgain = true;													\
	Size _subMsgCur = REUBEN_NETWORK_BUFFER_HEADER_SIZE;					\
	ProcedureID pcdId = _context_.pcdId;									\
	ProcedureInfo *pinfo = pcmgr.Get(pcdId);								\
	if (pinfo == NULL) {													\
		pcmgr.EndReturn(pinfo, pcdId, PCD_ERR_NOTFOUND);					\
		_caseAgain = false;													\
		return RPC_RESULT_FAIL;												\
	}																		\
	pinfo->pSrcBuf->ResetCursor();

#define PROCEDURE_CASE_START												\
	while (_caseAgain) {													\
	_caseAgain = false;														\
	if (pinfo->pSrcBuf->GetCursor() != pinfo->pSrcBuf->GetSize())			\
	{																		\
		pcmgr._debugMsg.Format("%s:%d", __FILE__, __LINE__);				\
		pinfo->errRpcId = PCD_ERR_BUF_SRC;									\
	}																		\
	if (pinfo->errRpcId == 0)												\
		pinfo->trapRpcId = 0;												\
	if (pinfo->errRpcId == 0)												\
	switch (pinfo->step)													\
	{																		\
	case 0:																	\
		{

#define PROCEDURE_SRC_WRITE(_type_, _value_)								\
	pinfo->pSrcBuf->Write<_type_>(_value_);

#define PROCEDURE_SRC_READ(_type_, _value_)									\
		_type_ _value_;														\
		if (pinfo->pSrcBuf->GetCursor() < pinfo->pSrcBuf->GetSize())		\
			pinfo->pSrcBuf->Read<_type_>(_value_);							\
		else																\
		{																	\
			pcmgr._debugMsg.Format("%s:%d", __FILE__, __LINE__);			\
			pinfo->errRpcId = PCD_ERR_BUF_SRC;								\
		}

#define PROCEDURE_SUB_TEMPBUF												\
		pinfo->pSubBuf = pcmgr.GetTempBuf();

#define PROCEDURE_SUB_RESET													\
	if (pinfo->pSubBuf)														\
		if (_subMsgCur <= REUBEN_NETWORK_BUFFER_HEADER_SIZE)				\
			pinfo->pSubBuf->ResetCursor();									\
		else																\
			pinfo->pSubBuf->SetCursor(_subMsgCur);

#define PROCEDURE_SUB_WRITE(_type_, _value_)								\
	pinfo->pSubBuf->Write<_type_>(_value_);

#define PROCEDURE_SUB_READ(_type_, _value_)									\
		_type_ _value_;														\
		if (pinfo->pSubBuf->GetCursor() < pinfo->pSubBuf->GetSize())		\
			pinfo->pSubBuf->Read<_type_>(_value_);							\
		else																\
		{																	\
			pcmgr._debugMsg.Format("%s:%d", __FILE__, __LINE__);			\
			pinfo->errRpcId = PCD_ERR_BUF_SUB;								\
			_caseAgain = true;												\
			break;															\
		}

#define PROCEDURE_SUB_CHECK													\
		if (pinfo->pSubBuf && !pcmgr.IsTempBuf(pinfo->pSubBuf) &&			\
			pinfo->pSubBuf->GetCursor() != pinfo->pSubBuf->GetSize())		\
		{																	\
			pcmgr._debugMsg.Format("%s:%d", __FILE__, __LINE__);			\
			pinfo->errRpcId = PCD_ERR_BUF_SUB;								\
			_caseAgain = true;												\
			break;															\
		}																	\
		pinfo->pSubBuf = NULL;

#define PROCEDURE_INIT_VALUES												\
		if (pinfo->pParamBuf == NULL)										\
			pinfo->pParamBuf = SafeCreateObject1(Buffer, 24);				\
		pinfo->pParamBuf->ResetCursor();

#define PROCEDURE_STORE_VALUE(_type_, _value_)								\
		pinfo->pParamBuf->Write<_type_>(_value_);

#define PROCEDURE_RESTORE_VALUE(_type_, _value_)							\
		_type_ _value_;														\
		if (pinfo->pParamBuf->GetCursor() < pinfo->pParamBuf->GetSize())	\
			pinfo->pParamBuf->Read<_type_>(_value_);						\
		else																\
		{																	\
			pcmgr._debugMsg.Format("%s:%d", __FILE__, __LINE__);			\
			pinfo->errRpcId = PCD_ERR_BUF_PARAM;							\
			_caseAgain = true;												\
			break;															\
		}

#define PROCEDURE_STORE_VALUE1(_type1_, _value1_)			\
		PROCEDURE_INIT_VALUES								\
		PROCEDURE_STORE_VALUE(_type1_, _value1_)

#define PROCEDURE_STORE_VALUE2(_type1_, _value1_, _type2_, _value2_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_STORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_STORE_VALUE(_type2_, _value2_)

#define PROCEDURE_STORE_VALUE3(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_STORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_STORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_STORE_VALUE(_type3_, _value3_)

#define PROCEDURE_STORE_VALUE4(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_STORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_STORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_STORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_STORE_VALUE(_type4_, _value4_)

#define PROCEDURE_STORE_VALUE5(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_STORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_STORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_STORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_STORE_VALUE(_type4_, _value4_)				\
		PROCEDURE_STORE_VALUE(_type5_, _value5_)

#define PROCEDURE_STORE_VALUE6(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_STORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_STORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_STORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_STORE_VALUE(_type4_, _value4_)				\
		PROCEDURE_STORE_VALUE(_type5_, _value5_)				\
		PROCEDURE_STORE_VALUE(_type6_, _value6_)

#define PROCEDURE_STORE_VALUE7(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_STORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_STORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_STORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_STORE_VALUE(_type4_, _value4_)				\
		PROCEDURE_STORE_VALUE(_type5_, _value5_)				\
		PROCEDURE_STORE_VALUE(_type6_, _value6_)				\
		PROCEDURE_STORE_VALUE(_type7_, _value7_)

#define PROCEDURE_STORE_VALUE8(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_STORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_STORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_STORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_STORE_VALUE(_type4_, _value4_)				\
		PROCEDURE_STORE_VALUE(_type5_, _value5_)				\
		PROCEDURE_STORE_VALUE(_type6_, _value6_)				\
		PROCEDURE_STORE_VALUE(_type7_, _value7_)				\
		PROCEDURE_STORE_VALUE(_type8_, _value8_)

#define PROCEDURE_STORE_VALUE9(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_STORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_STORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_STORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_STORE_VALUE(_type4_, _value4_)				\
		PROCEDURE_STORE_VALUE(_type5_, _value5_)				\
		PROCEDURE_STORE_VALUE(_type6_, _value6_)				\
		PROCEDURE_STORE_VALUE(_type7_, _value7_)				\
		PROCEDURE_STORE_VALUE(_type8_, _value8_)				\
		PROCEDURE_STORE_VALUE(_type9_, _value9_)

#define PROCEDURE_RESTORE_VALUE1(_type1_, _value1_)			\
		PROCEDURE_INIT_VALUES								\
		PROCEDURE_RESTORE_VALUE(_type1_, _value1_)

#define PROCEDURE_RESTORE_VALUE2(_type1_, _value1_, _type2_, _value2_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_RESTORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_RESTORE_VALUE(_type2_, _value2_)

#define PROCEDURE_RESTORE_VALUE3(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_RESTORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_RESTORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_RESTORE_VALUE(_type3_, _value3_)

#define PROCEDURE_RESTORE_VALUE4(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_RESTORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_RESTORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_RESTORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_RESTORE_VALUE(_type4_, _value4_)

#define PROCEDURE_RESTORE_VALUE5(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_RESTORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_RESTORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_RESTORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_RESTORE_VALUE(_type4_, _value4_)				\
		PROCEDURE_RESTORE_VALUE(_type5_, _value5_)

#define PROCEDURE_RESTORE_VALUE6(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_RESTORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_RESTORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_RESTORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_RESTORE_VALUE(_type4_, _value4_)				\
		PROCEDURE_RESTORE_VALUE(_type5_, _value5_)				\
		PROCEDURE_RESTORE_VALUE(_type6_, _value6_)

#define PROCEDURE_RESTORE_VALUE7(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_RESTORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_RESTORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_RESTORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_RESTORE_VALUE(_type4_, _value4_)				\
		PROCEDURE_RESTORE_VALUE(_type5_, _value5_)				\
		PROCEDURE_RESTORE_VALUE(_type6_, _value6_)				\
		PROCEDURE_RESTORE_VALUE(_type7_, _value7_)

#define PROCEDURE_RESTORE_VALUE8(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_RESTORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_RESTORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_RESTORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_RESTORE_VALUE(_type4_, _value4_)				\
		PROCEDURE_RESTORE_VALUE(_type5_, _value5_)				\
		PROCEDURE_RESTORE_VALUE(_type6_, _value6_)				\
		PROCEDURE_RESTORE_VALUE(_type7_, _value7_)				\
		PROCEDURE_RESTORE_VALUE(_type8_, _value8_)

#define PROCEDURE_RESTORE_VALUE9(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
		PROCEDURE_INIT_VALUES									\
		PROCEDURE_RESTORE_VALUE(_type1_, _value1_)				\
		PROCEDURE_RESTORE_VALUE(_type2_, _value2_)				\
		PROCEDURE_RESTORE_VALUE(_type3_, _value3_)				\
		PROCEDURE_RESTORE_VALUE(_type4_, _value4_)				\
		PROCEDURE_RESTORE_VALUE(_type5_, _value5_)				\
		PROCEDURE_RESTORE_VALUE(_type6_, _value6_)				\
		PROCEDURE_RESTORE_VALUE(_type7_, _value7_)				\
		PROCEDURE_RESTORE_VALUE(_type8_, _value8_)				\
		PROCEDURE_RESTORE_VALUE(_type9_, _value9_)

#define PROCEDURE_COUNTER_INIT												\
		if (pinfo->pCountBuf == NULL)										\
			pinfo->pCountBuf = SafeCreateObject1(Buffer, 24);				\
		pinfo->pCountBuf->ResetCursor();

#define PROCEDURE_COUNTER_WRITE(_type_, _value_)							\
		pinfo->pCountBuf->Write<_type_>(_value_);

#define PROCEDURE_COUNTER_READ(_type_, _value_)								\
		_type_ _value_;														\
		if (pinfo->pCountBuf->GetCursor() < pinfo->pCountBuf->GetSize())	\
			pinfo->pCountBuf->Read<_type_>(_value_);						\
		else																\
		{																	\
			pcmgr._debugMsg.Format("%s:%d", __FILE__, __LINE__);			\
			pinfo->errRpcId = PCD_ERR_BUF_COUNT;							\
			_caseAgain = true;												\
			break;															\
		}

#define PROCEDURE_COUNTER_READLOOP											\
		if (pinfo->pCountBuf->GetCursor() < pinfo->pCountBuf->GetSize())	\
			pinfo->pCountBuf->Read<Boolean>(_loopEnd);						\
		else																\
		{																	\
			pcmgr._debugMsg.Format("%s:%d", __FILE__, __LINE__);			\
			pinfo->errRpcId = PCD_ERR_BUF_COUNT;							\
			_caseAgain = true;												\
			break;															\
		}

#define PROCEDURE_NETOBJ_WRITE(_pobj_)										\
		pinfo->pSubBuf->Write<RPCNetID>((_pobj_)->GetNetId());

#define PROCEDURE_NETOBJ_READ(_class_, _pobj_)								\
		C##_class_* _pobj_ = (C##_class_*)pcmgr.ReadNetObject(pinfo);		\
		if (_pobj_ == NULL) {												\
			pcmgr.EndReturn(pinfo, pcdId, PCD_ERR_NETOBJ);					\
			return RPC_RESULT_FAIL;											\
		}

#define PROCEDURE_ORIG_CONTEXT												\
		pinfo->origContext

#define PROCEDURE_WAIT_STEP(_num_, _class_, _method_)		\
	pinfo->trapRpcId = RPC_##_class_##_##_method_;			\
	pinfo->step = _num_;									\
	break;

#define PROCEDURE_ERROR_BREAK(_errno_)						\
	_caseAgain = true;										\
	pinfo->errRpcId = _errno_;								\
	break;

#define PROCEDURE_BREAK										\
	break;

#define PROCEDURE_RESUME(_context_)									\
	{																\
		ProcedureID pcdId = _context_.pcdId;						\
		ProcedureInfo *pinfo = pcmgr.Get(pcdId);					\
		if (pinfo) {												\
			pinfo->trapRpcId = PCD_RETURN;							\
			ProcedureManager::GetInstance().Resume(pinfo, pcdId);	\
		}															\
	}

#define PROCEDURE_LABEL(_num_)								\
			pinfo->step = _num_;							\
		}													\
	case _num_:												\
		{													\
			if (pinfo->pSubBuf) _subMsgCur = pinfo->pSubBuf->GetCursor();

#define PROCEDURE_GO_STEP(_num_)							\
		pinfo->step = _num_;								\
		_caseAgain = true;									\
		if (pinfo->pSubBuf == NULL)							\
			pinfo->pSubBuf = pcmgr.GetTempBuf();			\
		else												\
			pinfo->pSubBuf->ResetCursor();					\
		break;

#define PROCEDURE_GO_STEP1(_num_, _type1_, _value1_)		\
	PROCEDURE_SUB_TEMPBUF									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP2(_num_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_SUB_TEMPBUF									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP3(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_SUB_TEMPBUF									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP4(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_SUB_TEMPBUF									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP5(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_SUB_TEMPBUF									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP6(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_SUB_TEMPBUF									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_)					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP7(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_SUB_TEMPBUF									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_)					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_)					\
	PROCEDURE_SUB_WRITE(_type7_, _value7_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP8(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_SUB_TEMPBUF									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_)					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_)					\
	PROCEDURE_SUB_WRITE(_type7_, _value7_)					\
	PROCEDURE_SUB_WRITE(_type8_, _value8_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP9(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_SUB_TEMPBUF									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_)					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_)					\
	PROCEDURE_SUB_WRITE(_type7_, _value7_)					\
	PROCEDURE_SUB_WRITE(_type8_, _value8_)					\
	PROCEDURE_SUB_WRITE(_type9_, _value9_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP_OBJ(_num_, _class_, _pobj_)		\
	PROCEDURE_NETOBJ_WRITE(_pobj_)							\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP_OBJ1(_num_, _pobj_, _type1_, _value1_)		\
	PROCEDURE_NETOBJ_WRITE(_pobj_)							\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP_OBJ2(_num_, _pobj_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_NETOBJ_WRITE(_pobj_)							\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP_OBJ3(_num_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_NETOBJ_WRITE(_pobj_)							\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP_OBJ4(_num_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_NETOBJ_WRITE(_pobj_)							\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP_OBJ5(_num_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_NETOBJ_WRITE(_pobj_)							\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP_OBJ6(_num_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_NETOBJ_WRITE(_pobj_)							\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_)					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP_OBJ7(_num_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_NETOBJ_WRITE(_pobj_)							\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_)					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_)					\
	PROCEDURE_SUB_WRITE(_type7_, _value7_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP_OBJ8(_num_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_NETOBJ_WRITE(_pobj_)							\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_)					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_)					\
	PROCEDURE_SUB_WRITE(_type7_, _value7_)					\
	PROCEDURE_SUB_WRITE(_type8_, _value8_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_GO_STEP_OBJ9(_num_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_NETOBJ_WRITE(_pobj_)							\
	PROCEDURE_SUB_WRITE(_type1_, _value1_)					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_)					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_)					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_)					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_)					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_)					\
	PROCEDURE_SUB_WRITE(_type7_, _value7_)					\
	PROCEDURE_SUB_WRITE(_type8_, _value8_)					\
	PROCEDURE_SUB_WRITE(_type9_, _value9_)					\
	PROCEDURE_GO_STEP(_num_)

#define PROCEDURE_FOR_HEAD(_num_, _endnum_)					\
			pinfo->step = _num_;							\
			_caseAgain = true;								\
		}													\
		break;												\
	case _num_:												\
		{													\
			if (pinfo->pSubBuf) _subMsgCur = pinfo->pSubBuf->GetCursor();		\
			Boolean _loopEnd = true;						\
			if (pinfo->pCountBuf)							\
				pinfo->pCountBuf->ResetCursor();			\
			if (pinfo->pCountBuf == NULL ||										\
				pinfo->pCountBuf->GetCursor() == pinfo->pCountBuf->GetSize()) {	\
					_caseAgain = true;						\
					pinfo->step = _endnum_;					\
					break;									\
			}

#define PROCEDURE_FOR_TAIL(_endnum_, _cond_)				\
			if (!(_cond_)) {							\
				_caseAgain = true;						\
				pinfo->step = _endnum_;					\
				pinfo->pCountBuf->ResetCursor();		\
				pinfo->pCountBuf->Write<Boolean>(BOOLEAN_TRUE);	\
				break;									\
			}	

#define PROCEDURE_END_FOR_HEAD(_num_)						\
			pinfo->step = _num_;							\
		}													\
	case _num_:												\
		{													\
			if (pinfo->pSubBuf) _subMsgCur = pinfo->pSubBuf->GetCursor();		\
			Boolean _loopEnd = true;						\
			if (pinfo->pCountBuf)							\
				pinfo->pCountBuf->ResetCursor();			\
			if (pinfo->pCountBuf && pinfo->pCountBuf->GetCursor() < pinfo->pCountBuf->GetSize()) {

#define PROCEDURE_END_FOR_TAIL(_num_, _fornum_)				\
				if (!_loopEnd) {							\
					_caseAgain = true;						\
					pinfo->step = _fornum_;					\
					break;									\
				}											\
			}

#define PROCEDURE_EXIT_FOR(_endnum_)						\
		_caseAgain = true;									\
		pinfo->step = _endnum_;								\
		pinfo->pCountBuf->ResetCursor();						\
		pinfo->pCountBuf->Write<Boolean>(BOOLEAN_TRUE);		\
		break;

#define PROCEDURE_FOR(_num_, _endnum_, _cond_)				\
	PROCEDURE_COUNTER_INIT									\
	PROCEDURE_COUNTER_WRITE(Boolean, BOOLEAN_FALSE)			\
	PROCEDURE_FOR_HEAD(_num_, _endnum_)						\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_FOR_TAIL(_endnum_, _cond_)

#define PROCEDURE_FOR1(_num_, _endnum_, _cond_, _type1_, _value1_)	\
	PROCEDURE_COUNTER_INIT									\
	PROCEDURE_COUNTER_WRITE(Boolean, BOOLEAN_FALSE)			\
	PROCEDURE_COUNTER_WRITE(_type1_, _value1_)				\
	PROCEDURE_FOR_HEAD(_num_, _endnum_)						\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_COUNTER_READ(_type1_, _value1_)				\
	PROCEDURE_FOR_TAIL(_endnum_, _cond_)

#define PROCEDURE_FOR2(_num_, _endnum_, _cond_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_COUNTER_INIT									\
	PROCEDURE_COUNTER_WRITE(Boolean, BOOLEAN_FALSE)			\
	PROCEDURE_COUNTER_WRITE(_type1_, _value1_)				\
	PROCEDURE_COUNTER_WRITE(_type2_, _value2_)				\
	PROCEDURE_FOR_HEAD(_num_, _endnum_)						\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_COUNTER_READ(_type1_, _value1_)				\
	PROCEDURE_COUNTER_READ(_type2_, _value2_)				\
	PROCEDURE_FOR_TAIL(_endnum_, _cond_)

#define PROCEDURE_FOR3(_num_, _endnum_, _cond_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_COUNTER_INIT									\
	PROCEDURE_COUNTER_WRITE(Boolean, BOOLEAN_FALSE)			\
	PROCEDURE_COUNTER_WRITE(_type1_, _value1_)				\
	PROCEDURE_COUNTER_WRITE(_type2_, _value2_)				\
	PROCEDURE_COUNTER_WRITE(_type3_, _value3_)				\
	PROCEDURE_FOR_HEAD(_num_, _endnum_)						\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_COUNTER_READ(_type1_, _value1_)				\
	PROCEDURE_COUNTER_READ(_type2_, _value2_)				\
	PROCEDURE_COUNTER_READ(_type3_, _value3_)				\
	PROCEDURE_FOR_TAIL(_endnum_, _cond_)

#define PROCEDURE_FOR4(_num_, _endnum_, _cond_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_COUNTER_INIT									\
	PROCEDURE_COUNTER_WRITE(Boolean, BOOLEAN_FALSE)			\
	PROCEDURE_COUNTER_WRITE(_type1_, _value1_)				\
	PROCEDURE_COUNTER_WRITE(_type2_, _value2_)				\
	PROCEDURE_COUNTER_WRITE(_type3_, _value3_)				\
	PROCEDURE_COUNTER_WRITE(_type4_, _value4_)				\
	PROCEDURE_FOR_HEAD(_num_, _endnum_)						\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_COUNTER_READ(_type1_, _value1_)				\
	PROCEDURE_COUNTER_READ(_type2_, _value2_)				\
	PROCEDURE_COUNTER_READ(_type3_, _value3_)				\
	PROCEDURE_COUNTER_READ(_type4_, _value4_)				\
	PROCEDURE_FOR_TAIL(_endnum_, _cond_)

#define PROCEDURE_FOR5(_num_, _endnum_, _cond_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_COUNTER_INIT									\
	PROCEDURE_COUNTER_WRITE(Boolean, BOOLEAN_FALSE)			\
	PROCEDURE_COUNTER_WRITE(_type1_, _value1_)				\
	PROCEDURE_COUNTER_WRITE(_type2_, _value2_)				\
	PROCEDURE_COUNTER_WRITE(_type3_, _value3_)				\
	PROCEDURE_COUNTER_WRITE(_type4_, _value4_)				\
	PROCEDURE_COUNTER_WRITE(_type5_, _value5_)				\
	PROCEDURE_FOR_HEAD(_num_, _endnum_)						\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_COUNTER_READ(_type1_, _value1_)				\
	PROCEDURE_COUNTER_READ(_type2_, _value2_)				\
	PROCEDURE_COUNTER_READ(_type3_, _value3_)				\
	PROCEDURE_COUNTER_READ(_type4_, _value4_)				\
	PROCEDURE_COUNTER_READ(_type5_, _value5_)				\
	PROCEDURE_FOR_TAIL(_endnum_, _cond_)

#define PROCEDURE_STORE_COUNTER1(_type1_, _value1_)			\
	PROCEDURE_COUNTER_INIT									\
	PROCEDURE_COUNTER_WRITE(Boolean, _loopEnd)				\
	PROCEDURE_COUNTER_WRITE(_type1_, _value1_)

#define PROCEDURE_STORE_COUNTER2(_type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_COUNTER_INIT									\
	PROCEDURE_COUNTER_WRITE(Boolean, _loopEnd)				\
	PROCEDURE_COUNTER_WRITE(_type1_, _value1_)				\
	PROCEDURE_COUNTER_WRITE(_type2_, _value2_)

#define PROCEDURE_STORE_COUNTER3(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_COUNTER_INIT									\
	PROCEDURE_COUNTER_WRITE(Boolean, _loopEnd)				\
	PROCEDURE_COUNTER_WRITE(_type1_, _value1_)				\
	PROCEDURE_COUNTER_WRITE(_type2_, _value2_)				\
	PROCEDURE_COUNTER_WRITE(_type3_, _value3_)

#define PROCEDURE_STORE_COUNTER4(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_COUNTER_INIT									\
	PROCEDURE_COUNTER_WRITE(Boolean, _loopEnd)				\
	PROCEDURE_COUNTER_WRITE(_type1_, _value1_)				\
	PROCEDURE_COUNTER_WRITE(_type2_, _value2_)				\
	PROCEDURE_COUNTER_WRITE(_type3_, _value3_)				\
	PROCEDURE_COUNTER_WRITE(_type4_, _value4_)

#define PROCEDURE_STORE_COUNTER5(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_COUNTER_INIT									\
	PROCEDURE_COUNTER_WRITE(Boolean, _loopEnd)				\
	PROCEDURE_COUNTER_WRITE(_type1_, _value1_)				\
	PROCEDURE_COUNTER_WRITE(_type2_, _value2_)				\
	PROCEDURE_COUNTER_WRITE(_type3_, _value3_)				\
	PROCEDURE_COUNTER_WRITE(_type4_, _value4_)				\
	PROCEDURE_COUNTER_WRITE(_type5_, _value5_)

#define PROCEDURE_END_FOR(_num_, _fornum_)					\
	PROCEDURE_END_FOR_HEAD(_num_)							\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_END_FOR_TAIL(_num_, _fornum_)

#define PROCEDURE_END_FOR1(_num_, _fornum_, _type1_, _value1_)	\
	PROCEDURE_END_FOR_HEAD(_num_, _fornum_)					\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_COUNTER_READ(_type1_, _value1_)				\
	PROCEDURE_END_FOR_TAIL(_num_, _fornum_)

#define PROCEDURE_END_FOR2(_num_, _fornum_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_END_FOR_HEAD(_num_, _fornum_)					\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_COUNTER_READ(_type1_, _value1_)				\
	PROCEDURE_COUNTER_READ(_type2_, _value2_)				\
	PROCEDURE_END_FOR_TAIL(_num_, _fornum_)

#define PROCEDURE_END_FOR3(_num_, _fornum_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_END_FOR_HEAD(_num_, _fornum_)					\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_COUNTER_READ(_type1_, _value1_)				\
	PROCEDURE_COUNTER_READ(_type2_, _value2_)				\
	PROCEDURE_COUNTER_READ(_type3_, _value3_)				\
	PROCEDURE_END_FOR_TAIL(_num_, _fornum_)

#define PROCEDURE_END_FOR4(_num_, _fornum_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_END_FOR_HEAD(_num_, _fornum_)					\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_COUNTER_READ(_type1_, _value1_)				\
	PROCEDURE_COUNTER_READ(_type2_, _value2_)				\
	PROCEDURE_COUNTER_READ(_type3_, _value3_)				\
	PROCEDURE_COUNTER_READ(_type4_, _value4_)				\
	PROCEDURE_END_FOR_TAIL(_num_, _fornum_)

#define PROCEDURE_END_FOR5(_num_, _fornum_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_END_FOR_HEAD(_num_, _fornum_)					\
	PROCEDURE_COUNTER_READLOOP								\
	PROCEDURE_COUNTER_READ(_type1_, _value1_)				\
	PROCEDURE_COUNTER_READ(_type2_, _value2_)				\
	PROCEDURE_COUNTER_READ(_type3_, _value3_)				\
	PROCEDURE_COUNTER_READ(_type4_, _value4_)				\
	PROCEDURE_COUNTER_READ(_type5_, _value5_)				\
	PROCEDURE_END_FOR_TAIL(_num_, _fornum_)

#define PROCEDURE_ERROR_RETURN(_errcode_)					\
	pcmgr.EndReturn(pinfo, pcdId, PCD_ERR_GENERAL);			\
	return _errcode_;

#define PROCEDURE_ERROR_OUTSIDE(_errcode_)								\
	{																	\
		ProcedureManager &pcmgr = ProcedureManager::GetInstance();		\
		ProcedureID pcdId = context.pcdId;								\
		ProcedureInfo *pinfo = pcmgr.Get(pcdId);						\
		if (pinfo) {													\
			pcmgr.EndReturn(pinfo, pcdId, PCD_ERR_GENERAL);			\
		}																\
		return _errcode_;												\
	}

#define PROCEDURE_DO_RPC(_class_, _method_, _netObj_, _peer_, _context_)	\
	PROCEDURE_DO_RPC_HEAD(_class_, _method_, _netObj_, _peer_, _context_)	\
	PROCEDURE_DO_RPC_TAIL(_peer_, _context_)

#define PROCEDURE_DO_RPC1(_class_, _method_, _netObj_, _peer_, _context_, _type1_, _value1_)	\
	PROCEDURE_DO_RPC_HEAD(_class_, _method_, _netObj_, _peer_, _context_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_DO_RPC_TAIL(_peer_, _context_)

#define PROCEDURE_DO_RPC2(_class_, _method_, _netObj_, _peer_, _context_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_DO_RPC_HEAD(_class_, _method_, _netObj_, _peer_, _context_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_DO_RPC_TAIL(_peer_, _context_)

#define PROCEDURE_DO_RPC3(_class_, _method_, _netObj_, _peer_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_DO_RPC_HEAD(_class_, _method_, _netObj_, _peer_, _context_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_DO_RPC_TAIL(_peer_, _context_)

#define PROCEDURE_DO_RPC4(_class_, _method_, _netObj_, _peer_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_DO_RPC_HEAD(_class_, _method_, _netObj_, _peer_, _context_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_DO_RPC_TAIL(_peer_, _context_)

#define PROCEDURE_DO_RPC5(_class_, _method_, _netObj_, _peer_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_DO_RPC_HEAD(_class_, _method_, _netObj_, _peer_, _context_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_DO_RPC_TAIL(_peer_, _context_)

#define PROCEDURE_DO_RPC6(_class_, _method_, _netObj_, _peer_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_DO_RPC_HEAD(_class_, _method_, _netObj_, _peer_, _context_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_DO_RPC_TAIL(_peer_, _context_)

#define PROCEDURE_DO_RPC7(_class_, _method_, _netObj_, _peer_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_DO_RPC_HEAD(_class_, _method_, _netObj_, _peer_, _context_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_DO_RPC_TAIL(_peer_, _context_)

#define PROCEDURE_DO_RPC8(_class_, _method_, _netObj_, _peer_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_DO_RPC_HEAD(_class_, _method_, _netObj_, _peer_, _context_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_SRC_WRITE(_type8_, _value8_);					\
	PROCEDURE_DO_RPC_TAIL(_peer_, _context_)

#define PROCEDURE_DO_RPC9(_class_, _method_, _netObj_, _peer_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_DO_RPC_HEAD(_class_, _method_, _netObj_, _peer_, _context_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_SRC_WRITE(_type8_, _value8_);					\
	PROCEDURE_SRC_WRITE(_type9_, _value9_);					\
	PROCEDURE_DO_RPC_TAIL(_peer_, _context_)

#define PROCEDURE_START_RPC(_class_, _method_, _netObj_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_START_RPC1(_class_, _method_, _netObj_, _type1_, _value1_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_START_RPC2(_class_, _method_, _netObj_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_START_RPC3(_class_, _method_, _netObj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_START_RPC4(_class_, _method_, _netObj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_START_RPC5(_class_, _method_, _netObj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_START_RPC6(_class_, _method_, _netObj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_START_RPC7(_class_, _method_, _netObj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_START_RPC8(_class_, _method_, _netObj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_SRC_WRITE(_type8_, _value8_);					\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_START_RPC9(_class_, _method_, _netObj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_SRC_WRITE(_type8_, _value8_);					\
	PROCEDURE_SRC_WRITE(_type9_, _value9_);					\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_START_RPC10(_class_, _method_, _netObj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_, _type10_, _value10_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_SRC_WRITE(_type8_, _value8_);					\
	PROCEDURE_SRC_WRITE(_type9_, _value9_);					\
	PROCEDURE_SRC_WRITE(_type10_, _value10_);					\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_START_RPC11(_class_, _method_, _netObj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_, _type10_, _value10_, _type11_, _value11_)	\
	PROCEDURE_START_RPC_HEAD(_class_, _method_, _netObj_)	\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_SRC_WRITE(_type8_, _value8_);					\
	PROCEDURE_SRC_WRITE(_type9_, _value9_);					\
	PROCEDURE_SRC_WRITE(_type10_, _value10_);					\
	PROCEDURE_SRC_WRITE(_type11_, _value11_);					\
	PROCEDURE_START_RPC_TAIL								\
	PROCEDURE_CASE_START

#define PROCEDURE_DO_METHOD(_class_, _method_, _pobj_, _context_)				\
	PROCEDURE_DO_METHOD_HEAD(_class_, _method_, _pobj_, _context_)				\
	PROCEDURE_DO_METHOD_TAIL(_class_, _method_, _pobj_, _context_)

#define PROCEDURE_DO_METHOD1(_class_, _method_, _pobj_, _context_, _type1_, _value1_)	\
	PROCEDURE_DO_METHOD_HEAD(_class_, _method_, _pobj_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_DO_METHOD_TAIL(_class_, _method_, _pobj_, _context_)

#define PROCEDURE_DO_METHOD2(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_DO_METHOD_HEAD(_class_, _method_, _pobj_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_DO_METHOD_TAIL(_class_, _method_, _pobj_, _context_)

#define PROCEDURE_DO_METHOD3(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_DO_METHOD_HEAD(_class_, _method_, _pobj_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_DO_METHOD_TAIL(_class_, _method_, _pobj_, _context_)

#define PROCEDURE_DO_METHOD4(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_DO_METHOD_HEAD(_class_, _method_, _pobj_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_DO_METHOD_TAIL(_class_, _method_, _pobj_, _context_)

#define PROCEDURE_DO_METHOD5(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_DO_METHOD_HEAD(_class_, _method_, _pobj_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_DO_METHOD_TAIL(_class_, _method_, _pobj_, _context_)

#define PROCEDURE_DO_METHOD6(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_DO_METHOD_HEAD(_class_, _method_, _pobj_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_DO_METHOD_TAIL(_class_, _method_, _pobj_, _context_)

#define PROCEDURE_DO_METHOD7(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_DO_METHOD_HEAD(_class_, _method_, _pobj_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_DO_METHOD_TAIL(_class_, _method_, _pobj_, _context_)

#define PROCEDURE_DO_METHOD8(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_DO_METHOD_HEAD(_class_, _method_, _pobj_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_SRC_WRITE(_type8_, _value8_);					\
	PROCEDURE_DO_METHOD_TAIL(_class_, _method_, _pobj_, _context_)

#define PROCEDURE_DO_METHOD9(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_DO_METHOD_HEAD(_class_, _method_, _pobj_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_SRC_WRITE(_type8_, _value8_);					\
	PROCEDURE_SRC_WRITE(_type9_, _value9_);					\
	PROCEDURE_DO_METHOD_TAIL(_class_, _method_, _pobj_, _context_)

#define PROCEDURE_START_METHOD(_class_, _method_, _pobj_, _context_)	\
	PROCEDURE_START_METHOD_HEAD(_class_, _method_, _pobj_, _context_)	\
	PROCEDURE_CASE_START

#define PROCEDURE_START_METHOD1(_class_, _method_, _pobj_, _context_, _type1_, _value1_)	\
	PROCEDURE_START_METHOD_HEAD(_class_, _method_, _pobj_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_METHOD2(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_START_METHOD_HEAD(_class_, _method_, _pobj_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_METHOD3(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_START_METHOD_HEAD(_class_, _method_, _pobj_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_METHOD4(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_START_METHOD_HEAD(_class_, _method_, _pobj_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_METHOD5(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_START_METHOD_HEAD(_class_, _method_, _pobj_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_SRC_READ(_type5_, _value5_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_METHOD6(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_START_METHOD_HEAD(_class_, _method_, _pobj_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_SRC_READ(_type5_, _value5_);					\
	PROCEDURE_SRC_READ(_type6_, _value6_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_METHOD7(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_START_METHOD_HEAD(_class_, _method_, _pobj_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_SRC_READ(_type5_, _value5_);					\
	PROCEDURE_SRC_READ(_type6_, _value6_);					\
	PROCEDURE_SRC_READ(_type7_, _value7_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_METHOD8(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_START_METHOD_HEAD(_class_, _method_, _pobj_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_SRC_READ(_type5_, _value5_);					\
	PROCEDURE_SRC_READ(_type6_, _value6_);					\
	PROCEDURE_SRC_READ(_type7_, _value7_);					\
	PROCEDURE_SRC_READ(_type8_, _value8_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_METHOD9(_class_, _method_, _pobj_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_START_METHOD_HEAD(_class_, _method_, _pobj_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_SRC_READ(_type5_, _value5_);					\
	PROCEDURE_SRC_READ(_type6_, _value6_);					\
	PROCEDURE_SRC_READ(_type7_, _value7_);					\
	PROCEDURE_SRC_READ(_type8_, _value8_);					\
	PROCEDURE_SRC_READ(_type9_, _value9_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_DO_FUNC(_func_, _context_)				\
	PROCEDURE_DO_FUNC_HEAD(_func_, _context_)				\
	PROCEDURE_DO_FUNC_TAIL(_func_, _context_)

#define PROCEDURE_DO_FUNC1(_func_, _context_, _type1_, _value1_)	\
	PROCEDURE_DO_FUNC_HEAD(_func_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_DO_FUNC_TAIL(_func_, _context_)

#define PROCEDURE_DO_FUNC2(_func_, _context_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_DO_FUNC_HEAD(_func_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_DO_FUNC_TAIL(_func_, _context_)

#define PROCEDURE_DO_FUNC3(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_DO_FUNC_HEAD(_func_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_DO_FUNC_TAIL(_func_, _context_)

#define PROCEDURE_DO_FUNC4(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_DO_FUNC_HEAD(_func_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_DO_FUNC_TAIL(_func_, _context_)

#define PROCEDURE_DO_FUNC5(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_DO_FUNC_HEAD(_func_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_DO_FUNC_TAIL(_func_, _context_)

#define PROCEDURE_DO_FUNC6(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_DO_FUNC_HEAD(_func_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_DO_FUNC_TAIL(_func_, _context_)

#define PROCEDURE_DO_FUNC7(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_DO_FUNC_HEAD(_func_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_DO_FUNC_TAIL(_func_, _context_)

#define PROCEDURE_DO_FUNC8(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_DO_FUNC_HEAD(_func_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_SRC_WRITE(_type8_, _value8_);					\
	PROCEDURE_DO_FUNC_TAIL(_func_, _context_)

#define PROCEDURE_DO_FUNC9(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_DO_FUNC_HEAD(_func_, _context_)				\
	PROCEDURE_SRC_WRITE(_type1_, _value1_);					\
	PROCEDURE_SRC_WRITE(_type2_, _value2_);					\
	PROCEDURE_SRC_WRITE(_type3_, _value3_);					\
	PROCEDURE_SRC_WRITE(_type4_, _value4_);					\
	PROCEDURE_SRC_WRITE(_type5_, _value5_);					\
	PROCEDURE_SRC_WRITE(_type6_, _value6_);					\
	PROCEDURE_SRC_WRITE(_type7_, _value7_);					\
	PROCEDURE_SRC_WRITE(_type8_, _value8_);					\
	PROCEDURE_SRC_WRITE(_type9_, _value9_);					\
	PROCEDURE_DO_FUNC_TAIL(_func_, _context_)

#define PROCEDURE_START_FUNC(_func_, _context_)	\
	PROCEDURE_START_FUNC_HEAD(_func_, _context_)	\
	PROCEDURE_CASE_START

#define PROCEDURE_START_FUNC1(_func_, _context_, _type1_, _value1_)	\
	PROCEDURE_START_FUNC_HEAD(_func_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_FUNC2(_func_, _context_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_START_FUNC_HEAD(_func_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_FUNC3(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_START_FUNC_HEAD(_func_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_FUNC4(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_START_FUNC_HEAD(_func_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_FUNC5(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_START_FUNC_HEAD(_func_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_SRC_READ(_type5_, _value5_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_FUNC6(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_START_FUNC_HEAD(_func_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_SRC_READ(_type5_, _value5_);					\
	PROCEDURE_SRC_READ(_type6_, _value6_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_FUNC7(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_START_FUNC_HEAD(_func_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_SRC_READ(_type5_, _value5_);					\
	PROCEDURE_SRC_READ(_type6_, _value6_);					\
	PROCEDURE_SRC_READ(_type7_, _value7_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_FUNC8(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_START_FUNC_HEAD(_func_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_SRC_READ(_type5_, _value5_);					\
	PROCEDURE_SRC_READ(_type6_, _value6_);					\
	PROCEDURE_SRC_READ(_type7_, _value7_);					\
	PROCEDURE_SRC_READ(_type8_, _value8_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_START_FUNC9(_func_, _context_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type9_, _value9_)	\
	PROCEDURE_START_FUNC_HEAD(_func_, _context_)	\
	PROCEDURE_SRC_READ(_type1_, _value1_);					\
	PROCEDURE_SRC_READ(_type2_, _value2_);					\
	PROCEDURE_SRC_READ(_type3_, _value3_);					\
	PROCEDURE_SRC_READ(_type4_, _value4_);					\
	PROCEDURE_SRC_READ(_type5_, _value5_);					\
	PROCEDURE_SRC_READ(_type6_, _value6_);					\
	PROCEDURE_SRC_READ(_type7_, _value7_);					\
	PROCEDURE_SRC_READ(_type8_, _value8_);					\
	PROCEDURE_SRC_READ(_type9_, _value9_);					\
	PROCEDURE_CASE_START

#define PROCEDURE_RETURN_HEAD								\
	if (pinfo->oldPcdId.srvId == pcmgr.GetServiceId() && pinfo->oldPcdId.subId != 0) \
	{														\
		ProcedureInfo* tmpPinfo = pinfo;					\
		pinfo = pcmgr.Get(pinfo->oldPcdId);					\
		if (pinfo) {										\
			pinfo->pSubBuf = pcmgr.GetTempBuf();

#define PROCEDURE_RETURN_TAIL								\
			if (pinfo->pSubBuf->GetCursor() > 0)	\
				pinfo->pSubBuf->ResetCursor();		\
		}													\
		pinfo = tmpPinfo;									\
	}														\
	pcmgr.EndReturn(pinfo, pcdId, PCD_RETURN);				\
	return RPC_RESULT_OK;


#define PROCEDURE_RETURN									\
	PROCEDURE_RETURN_HEAD									\
	PROCEDURE_RETURN_TAIL

#define PROCEDURE_RETURN1(_type1_, _value1_)				\
	PROCEDURE_RETURN_HEAD									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_);					\
	PROCEDURE_RETURN_TAIL

#define PROCEDURE_RETURN2(_type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_RETURN_HEAD									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_);					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_);					\
	PROCEDURE_RETURN_TAIL

#define PROCEDURE_RETURN3(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_RETURN_HEAD									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_);					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_);					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_);					\
	PROCEDURE_RETURN_TAIL

#define PROCEDURE_RETURN4(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_RETURN_HEAD									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_);					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_);					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_);					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_);					\
	PROCEDURE_RETURN_TAIL

#define PROCEDURE_RETURN5(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_RETURN_HEAD									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_);					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_);					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_);					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_);					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_);					\
	PROCEDURE_RETURN_TAIL

#define PROCEDURE_RETURN6(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_RETURN_HEAD									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_);					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_);					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_);					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_);					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_);					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_);					\
	PROCEDURE_RETURN_TAIL

#define PROCEDURE_RETURN7(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_RETURN_HEAD									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_);					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_);					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_);					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_);					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_);					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_);					\
	PROCEDURE_SUB_WRITE(_type7_, _value7_);					\
	PROCEDURE_RETURN_TAIL

#define PROCEDURE_RETURN8(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_RETURN_HEAD									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_);					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_);					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_);					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_);					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_);					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_);					\
	PROCEDURE_SUB_WRITE(_type7_, _value7_);					\
	PROCEDURE_SUB_WRITE(_type8_, _value8_);					\
	PROCEDURE_RETURN_TAIL

#define PROCEDURE_RETURN9(_type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type9_, _value9_)	\
	PROCEDURE_RETURN_HEAD									\
	PROCEDURE_SUB_WRITE(_type1_, _value1_);					\
	PROCEDURE_SUB_WRITE(_type2_, _value2_);					\
	PROCEDURE_SUB_WRITE(_type3_, _value3_);					\
	PROCEDURE_SUB_WRITE(_type4_, _value4_);					\
	PROCEDURE_SUB_WRITE(_type5_, _value5_);					\
	PROCEDURE_SUB_WRITE(_type6_, _value6_);					\
	PROCEDURE_SUB_WRITE(_type7_, _value7_);					\
	PROCEDURE_SUB_WRITE(_type8_, _value8_);					\
	PROCEDURE_SUB_WRITE(_type9_, _value9_);					\
	PROCEDURE_RETURN_TAIL

#define PROCEDURE_WAIT_RETURN_HEAD(_num_)					\
			if (pinfo->trapRpcId == PCD_RETURN)	{			\
				_caseAgain = true;							\
			} else {										\
				pinfo->trapRpcId = PCD_RETURN;				\
				pinfo->needResume = BOOLEAN_TRUE;			\
				pcmgr.AddTimeout(pinfo, pcdId);				\
			}												\
			pinfo->step = _num_;							\
		}													\
		break;												\
	case _num_:												\
		{													\
			if (pinfo->pSubBuf) _subMsgCur = pinfo->pSubBuf->GetCursor();		\

#define PROCEDURE_WAIT_RETURN(_num_)						\
	PROCEDURE_WAIT_RETURN_HEAD(_num_)

#define PROCEDURE_WAIT_RETURN1(_num_, _type1_, _value1_)	\
	PROCEDURE_WAIT_RETURN_HEAD(_num_)						\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_RETURN2(_num_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_WAIT_RETURN_HEAD(_num_)						\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_RETURN3(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_WAIT_RETURN_HEAD(_num_)						\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_RETURN4(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_WAIT_RETURN_HEAD(_num_)						\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_RETURN5(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_WAIT_RETURN_HEAD(_num_)						\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_RETURN6(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_WAIT_RETURN_HEAD(_num_)						\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_RETURN7(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_WAIT_RETURN_HEAD(_num_)						\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_RETURN8(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_WAIT_RETURN_HEAD(_num_)						\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_READ(_type8_, _value8_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_RETURN9(_num_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_WAIT_RETURN_HEAD(_num_)						\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_READ(_type8_, _value8_)					\
	PROCEDURE_SUB_READ(_type9_, _value9_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)		\
			pinfo->trapRpcId = RPC_##_class_##_##_method_;	\
			pinfo->step = _num_;							\
			pinfo->needResume = BOOLEAN_TRUE;				\
			pcmgr.AddTimeout(pinfo, pcdId);					\
		}													\
		break;												\
	case _num_:												\
		{													\
			if (pinfo->pSubBuf) _subMsgCur = pinfo->pSubBuf->GetCursor();		\

#define PROCEDURE_WAIT(_num_, _class_, _method_)			\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)

#define PROCEDURE_WAIT1(_num_, _class_, _method_, _type1_, _value1_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT2(_num_, _class_, _method_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT3(_num_, _class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT4(_num_, _class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT5(_num_, _class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT6(_num_, _class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT7(_num_, _class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT8(_num_, _class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_READ(_type8_, _value8_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT9(_num_, _class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_READ(_type8_, _value8_)					\
	PROCEDURE_SUB_READ(_type9_, _value9_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_OBJ(_num_, _class_, _method_, _pobj_)			\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)				\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)

#define PROCEDURE_WAIT_OBJ1(_num_, _class_, _method_, _pobj_, _type1_, _value1_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_OBJ2(_num_, _class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_OBJ3(_num_, _class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_OBJ4(_num_, _class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_OBJ5(_num_, _class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_OBJ6(_num_, _class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_OBJ7(_num_, _class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_OBJ8(_num_, _class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_READ(_type8_, _value8_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_WAIT_OBJ9(_num_, _class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_WAIT_HEAD(_num_, _class_, _method_)			\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_READ(_type8_, _value8_)					\
	PROCEDURE_SUB_READ(_type9_, _value9_)					\
	PROCEDURE_SUB_CHECK

#define PROCEDURE_END														\
		}																	\
		pcmgr.EndReturn(pinfo, pcdId, PCD_RETURN);							\
		break;																\
	default:																\
		pcmgr.EndReturn(pinfo, pcdId, PCD_ERR_STEP);						\
	}																		\
	else																	\
	{																		\
		PROCEDURE_SUB_RESET													\
		pcmgr.EndReturn(pinfo, pcdId, pinfo->errRpcId);						\
	}																		\
	}																		\

#define PROCEDURE_CATCH														\
		}																	\
		pcmgr.EndReturn(pinfo, pcdId, PCD_RETURN);							\
		break;																\
	default:																\
		pcmgr.EndReturn(pinfo, pcdId, PCD_ERR_STEP);						\
	}																		\
	else																	\
	{																		\
		_caseAgain = false;													\
		switch (pinfo->errRpcId)											\
		{																	\
		case ERR_RPCMANAGER:												\
			{																\
			PROCEDURE_SUB_READ(UInt32, errCode)								\
			PROCEDURE_SUB_READ(String, errMsg)								\
			TRACE_ERRORDTL_3(GLOBAL_LOGGER,									\
				_FE("ERR_RPGMANAGER: sessionId %s, errCode %d, %s"),		\
				context.sessionId.GetString().c_str(), errCode, errMsg.c_str());

#define PROCEDURE_END_CATCH											\
			}														\
		}															\
		if (!_caseAgain && pinfo) {									\
			PROCEDURE_SUB_RESET										\
			pcmgr.EndReturn(pinfo, pcdId, pinfo->errRpcId);			\
		}															\
	}																\
	}

#define PROCEDURE_EXCEPTION_DEFAULT									\
			}														\
		}															\
		switch (0)													\
		{															\
		case 0:														\
			{

#define PROCEDURE_EXCEPTION_TIMEOUT									\
			}														\
			break;													\
		case PCD_ERR_TIMEOUT:										\
			{

#define PROCEDURE_ERROR_RESUME(_num_, _class_, _method_)			\
			pinfo->errRpcId = 0;									\
			pinfo->trapRpcId = RPC_##_class_##_##_method_;			\
			pinfo->step = _num_;									\
			_caseAgain = true;										\
			break;

#define PROCEDURE_ERROR_RESET										\
			pinfo->errRpcId = 0;									\

#define PROCEDURE_EXCEPTION_HEAD(_class_, _method_)					\
			}														\
			break;													\
		case RPC_##_class_##_##_method_:							\
			{														\
				if (pinfo->pSubBuf) _subMsgCur = pinfo->pSubBuf->GetCursor();		\

#define PROCEDURE_EXCEPTION(_class_, _method_)						\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)

#define PROCEDURE_EXCEPTION1(_class_, _method_, _type1_, _value1_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)						\
	PROCEDURE_SUB_READ(_type1_, _value1_)							\
	PROCEDURE_SUB_CHECK												\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION2(_class_, _method_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION3(_class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION4(_class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION5(_class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION6(_class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION7(_class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION8(_class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_READ(_type8_, _value8_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION9(_class_, _method_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_READ(_type8_, _value8_)					\
	PROCEDURE_SUB_READ(_type9_, _value9_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION_OBJ(_class_, _method_, _pobj_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION_OBJ1(_class_, _method_, _pobj_, _type1_, _value1_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION_OBJ2(_class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION_OBJ3(_class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION_OBJ4(_class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION_OBJ5(_class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION_OBJ6(_class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION_OBJ7(_class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION_OBJ8(_class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_READ(_type8_, _value8_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_EXCEPTION_OBJ9(_class_, _method_, _pobj_, _type1_, _value1_, _type2_, _value2_, _type3_, _value3_, _type4_, _value4_, _type5_, _value5_, _type6_, _value6_, _type7_, _value7_, _type8_, _value8_, _type9_, _value9_)	\
	PROCEDURE_EXCEPTION_HEAD(_class_, _method_)				\
	PROCEDURE_NETOBJ_READ(_class_, _pobj_)					\
	PROCEDURE_SUB_READ(_type1_, _value1_)					\
	PROCEDURE_SUB_READ(_type2_, _value2_)					\
	PROCEDURE_SUB_READ(_type3_, _value3_)					\
	PROCEDURE_SUB_READ(_type4_, _value4_)					\
	PROCEDURE_SUB_READ(_type5_, _value5_)					\
	PROCEDURE_SUB_READ(_type6_, _value6_)					\
	PROCEDURE_SUB_READ(_type7_, _value7_)					\
	PROCEDURE_SUB_READ(_type8_, _value8_)					\
	PROCEDURE_SUB_READ(_type9_, _value9_)					\
	PROCEDURE_SUB_CHECK										\
	PROCEDURE_SUB_RESET

#define PROCEDURE_SET_TIMEOUT(_time_, _fail_)				\
	ProcedureManager::_timeout = _time_;					\
	ProcedureManager::_timeoutFail = _fail_;

#define PROCEDURE_WAIT_TIMEOUT(_num_, _time_)				\
			ProcedureManager::_timeout = _time_;			\
			pinfo->trapRpcId = PCD_ERR_TIMEOUT;				\
			pinfo->step = _num_;							\
			pcmgr.AddTimeout(pinfo, pcdId);					\
		}													\
		break;												\
	case _num_:												\
		{													\
			if (pinfo->pSubBuf) _subMsgCur = pinfo->pSubBuf->GetCursor();		\

#define PROCEDURE_LOCK										\
	pcmgr.Lock(pinfo, pcdId);								\
	if (pinfo->lockState == PCD_LOCK_WAITING) {				\
		_caseAgain = false;									\
		break;												\
	}

#define PROCEDURE_UNLOCK									\
	pcmgr.Unlock(pinfo, pcdId);

#define PROCEDURE_LOCK_SESSION(_sid_)						\
	pcmgr.Lock(pinfo, pcdId, &_sid_);						\
	if (pinfo->lockState == PCD_LOCK_WAITING) {					\
		_caseAgain = false;									\
		break;												\
	}

#define PROCEDURE_UNLOCK_SESSION(_sid_)						\
	pcmgr.Unlock(pinfo, pcdId, &_sid_);

#define PROCEDURE_WAIT_SESSION(_num_, _sid_)				\
		if (pinfo->lockState == PCD_LOCKED)					\
			pcmgr.Unlock(pinfo, pcdId);						\
		pinfo->step = _num_;								\
		pcmgr.Lock(pinfo, pcdId, &_sid_);					\
		if (pinfo->lockState == PCD_LOCK_WAITING) {			\
			_caseAgain = false;								\
			break;											\
		}													\
		}													\
	case _num_:												\
		{													\
			if (pinfo->pSubBuf) _subMsgCur = pinfo->pSubBuf->GetCursor();		\

#endif	//_PROCEDURE_H_
