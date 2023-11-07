#pragma once
#ifndef _DBO_H_
#define _DBO_H_

#include "atldbcli.h"
#include <algorithm>

#define DB_SUCCEEDED(X)			((X) == S_OK)
#define DB_FAILED(X)			((X) != S_OK)
#define DB_ERR_UNEXPECTED		0x80004005
#define DB_ERR_SERVERFAULT		0x80010105
#define DB_ERR_CMDDISABLED		0x80040101
//#define DB_MYSQL
#ifdef DB_MYSQL
	#define DB_PROVIDER				_T("OleMySql.MySqlSource")
#else
	#define DB_PROVIDER				_T("SQLNCLI")
#endif

#define DEFINE_DBOCLASS(_class_)									\
	typedef _class_ _classtype;										\
	typedef _class_ _ParamClass;									\
	typedef _class_ _OutputColumnsClass;							\
	_class_() { ClearRecord(); }									\
	INLINE void ClearRecord() { memset(this, 0, sizeof(*this)); }	\
	static const Char* GetDboClass(VOID) { return _T(#_class_); }	\
	static StringVector m_params;									\
	INLINE void AddParam(const String &param)						\
	{																\
		if (std::find(m_params.begin(), m_params.end(), param) == m_params.end())	\
			m_params.push_back(param);								\
	}																\
	INLINE void ClearParam() { m_params.clear(); }

class CDboSource : public CDataSource
{
public:
	CDboSource()
	{
		m_dbStr = NULL;
		m_pDboSrc = NULL;
	}

	~CDboSource()
	{
		m_session.Close();
		Close();
		if (m_dbStr)
			SysFreeString(m_dbStr);
	}

	const CSession& GetSession() { return m_session; }

	HRESULT OpenSession(CDboSource* dbsrc)
	{
		CDBPropSet	propset(DBPROPSET_SESSION);

		// change auto commit level to faster read uncommitted
		propset.AddProperty(DBPROP_SESS_AUTOCOMMITISOLEVELS, DBPROPVAL_TI_READUNCOMMITTED,
			DBPROPOPTIONS_OPTIONAL);
		return m_session.Open(*dbsrc, &propset);
	}
	
	HRESULT Open(LPCTSTR cszDbServer, LPCTSTR cszDbName, LPCTSTR cszAccount, LPCTSTR cszPassword)
	{
		m_pDboSrc = NULL;
		m_dbinit.SetGUID(DBPROPSET_DBINIT);
		m_dbinit.AddProperty(DBPROP_INIT_DATASOURCE, cszDbServer);
		m_dbinit.AddProperty(DBPROP_INIT_CATALOG, cszDbName);
		m_dbinit.AddProperty(DBPROP_AUTH_USERID, cszAccount);
		m_dbinit.AddProperty(DBPROP_AUTH_PASSWORD, cszPassword);
		//m_dbinit.AddProperty(DBPROP_INIT_LCID, (long)2052);
		//m_dbinit.AddProperty(DBPROP_INIT_PROMPT, (short)4);
		m_dbinit.AddProperty(DBPROP_INIT_OLEDBSERVICES, LONG(DBPROPVAL_OS_ENABLEALL));
		m_dbinit.AddProperty(DBPROP_INIT_PROVIDERSTRING, L"MarsConn=yes");

		HRESULT hr = CDataSource::OpenWithServiceComponents(DB_PROVIDER, &m_dbinit);
		if (DB_SUCCEEDED(hr))
			hr = OpenSession(this);
		return hr;
	}

	HRESULT Open(LPCOLESTR szInitializationString)
	{
		HRESULT	hr;

		m_pDboSrc = NULL;
		hr = CDataSource::OpenFromInitializationString(szInitializationString);
		m_dbStr = SysAllocString(szInitializationString);
		if (DB_SUCCEEDED(hr))
			hr = OpenSession(this);
		return hr;
	}

	HRESULT Open(CDboSource* pDbSrc)
	{
		m_pDboSrc = pDbSrc;
		if (pDbSrc)
			return OpenSession(pDbSrc);
		else
			return S_FALSE;
	}

	HRESULT Reopen()
	{
		HRESULT hr;

		if (m_pDboSrc)
			return S_OK;

		m_session.Close();
		Close();
		if (m_dbStr != NULL) {
			hr = CDataSource::OpenFromInitializationString(m_dbStr);
		} else {
			hr = CDataSource::Open(DB_PROVIDER, &m_dbinit);
		}
		if (DB_SUCCEEDED(hr))
			hr = OpenSession(this);
		return hr;
	}

	void ShowError(HRESULT hr)
	{
		USES_CONVERSION;
		String errMsg;
		CDBErrorInfo einfo;
		BSTR bstrDescription, bstrSource;
		ULONG nRecords = 0;
		HRESULT hr2 = einfo.GetErrorRecords(&nRecords);
		DWORD MYLOCALEID = 0x0409;		// Hard-code an American English locale

		if(SUCCEEDED(hr2) && nRecords > 0)
		{
			for(ULONG nIndex = 0; nIndex < nRecords; nIndex++)
			{
				hr2 = einfo.GetAllErrorInfo(nIndex, MYLOCALEID, &bstrDescription, &bstrSource);
				if(SUCCEEDED(hr2))
				{
					errMsg += _T("\n") + String(OLE2T(bstrSource)) + _T(":") + 
						String(OLE2T(bstrDescription));
					SysFreeString(bstrSource);
					SysFreeString(bstrDescription);
				}
			}
		}
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("CDboSource OLE Error: ID %ld, hr 0x%08x: %s"), 
			this, hr, errMsg.c_str());
	}

private:
	//-- Variables
	CDBPropSet		m_dbinit;
	BSTR			m_dbStr;
	CSession		m_session;
	CDboSource*		m_pDboSrc;
};

/*DBO*/
template <typename _TYPE_>
class Cdbo : public _TYPE_
{
public:
	//-- Operations
	Cdbo()
	{
		m_rowAffected = 0;
		m_db = NULL;
		m_opened = BOOLEAN_FALSE;
	}

	~Cdbo()
	{
	}

	void SetDataSource(CDboSource* src)
	{
		m_db = src;
	}

	HRESULT ReopenDataSource()
	{
		if (m_db) {
			return (m_db->Reopen());
		} else {
			return 0;
		}
	}

	void GetRowsetProperties(CDBPropSet * pPropSet)
	{
		pPropSet->AddProperty(DBPROP_CANFETCHBACKWARDS, false, DBPROPOPTIONS_OPTIONAL);
		pPropSet->AddProperty(DBPROP_CANSCROLLBACKWARDS, false, DBPROPOPTIONS_OPTIONAL);
		pPropSet->AddProperty(DBPROP_APPENDONLY, true, DBPROPOPTIONS_OPTIONAL);
		//pPropSet->AddProperty(DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL);
		//pPropSet->AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
	}

	HRESULT OpenRowset(void)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(NULL);
		return OpenCommand();
	}

	HRESULT OpenCommand(LPCTSTR cszSQL = NULL, bool async=false)
	{
		// Set properties for open
		Close();
		CDBPropSet	propset(DBPROPSET_ROWSET);
		GetRowsetProperties(&propset);
		if (m_db == NULL || m_db->GetSession().m_spOpenRowset == NULL)
			return DB_ERR_UNEXPECTED;
		return _TYPE_::Open(m_db->GetSession(), cszSQL, &propset, &m_rowAffected);
	}

	LONG GetRowAffected()
	{
		return m_rowAffected;
	}

	String GetErrorMsg(HRESULT hr)
	{
		if (SUCCEEDED(hr))
			return _T("");

		USES_CONVERSION;
		String errMsg;
		CDBErrorInfo einfo;
		BSTR bstrDescription, bstrSource;
		ULONG nRecords = 0;
		HRESULT hr2 = einfo.GetErrorRecords(&nRecords);
		DWORD MYLOCALEID = 0x0409;		// Hard-code an American English locale

		if(SUCCEEDED(hr2) && nRecords > 0)
		{
			for(ULONG nIndex = 0; nIndex < nRecords; nIndex++)
			{
				hr2 = einfo.GetAllErrorInfo(nIndex, MYLOCALEID, &bstrDescription, &bstrSource);
				if(SUCCEEDED(hr2))
				{
					errMsg += _T("\n") + String(OLE2T(bstrSource)) + _T(":") + 
						String(OLE2T(bstrDescription));
					SysFreeString(bstrSource);
					SysFreeString(bstrDescription);
				}
			}
		}

		return errMsg;
	}

	void ShowError(HRESULT hr)
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("%s DB Error: hr 0x%08x: %s"), _TYPE_::GetDboClass(), 
			hr, GetErrorMsg(hr).c_str());
		if (hr >= DB_ERR_UNEXPECTED || hr == DB_ERR_CMDDISABLED)
		{
			// try to reopen OLEDB when unexpected error
			if (DB_FAILED(hr = m_db->Reopen()))
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("%s OLE Error: failed to reopen OLE DB"), 
					_TYPE_::GetDboClass());
			}
		}
	}

private:
	//-- Variables
	Boolean		m_opened;
	CDboSource*	m_db;
	DBROWCOUNT	m_rowAffected;
};

class CdboAccessor
{
public:
	typedef HRESULT (*GetParamsFunc)(LPOLESTR* pColumnNames, DBORDINAL* pColumns, DBBINDING *pBinding, BYTE* pBuffer, bool bClearOnly) throw ();
	typedef HRESULT (*GetBindFunc)(LPOLESTR* pColumnNames, DBORDINAL* pColumns, DBBINDING *pBinding, ULONG nAccessor, bool* pAuto, BYTE* pBuffer, bool bClearOnly) throw();

	//-- Operations
	CdboAccessor()
	{
		fpGetBindFunc = NULL;
		fpGetParams = NULL;
	}

	~CdboAccessor()
	{
	}

	ULONG _GetNumAccessors() { return 1; }
	void SetGetParamsFunc(GetParamsFunc fp) { fpGetParams = fp; }
	inline bool HasParameters() { return (fpGetParams != NULL); }
	inline HRESULT _GetParamEntries(LPOLESTR* pColumnNames, DBORDINAL* pColumns, DBBINDING *pBinding, BYTE* pBuffer = NULL, bool bClearOnly = false) throw ()
	{
		ATLASSERT(fpGetParams != NULL);
		return (fpGetParams(pColumnNames, pColumns, pBinding, pBuffer, bClearOnly));
	}

	inline void SetGetBindFunc(GetBindFunc fp) { fpGetBindFunc = fp; }
	inline bool HasOutputColumns() { return (fpGetBindFunc != NULL); }

	inline HRESULT _GetBindEntries(LPOLESTR* pColumnNames, DBORDINAL* pColumns, DBBINDING *pBinding, ULONG nAccessor, bool* pAuto, BYTE* pBuffer = NULL, bool bClearOnly = false) throw()
	{
		ATLASSERT(fpGetBindFunc != NULL);
		return (fpGetBindFunc(pColumnNames, pColumns, pBinding, nAccessor, pAuto, pBuffer, bClearOnly));
	}

private:
	GetParamsFunc fpGetParams;
	GetBindFunc fpGetBindFunc;
};

/*Params Map*/
#define ENABLE_PARAM_MAP(x) \
	public: \
	typedef HRESULT (*GetParamsFunc)(LPOLESTR* pColumnNames, DBORDINAL* pColumns, DBBINDING *pBinding, BYTE* pBuffer, bool bClearOnly) throw (); \
	typedef x _classtype; \
	typedef x _ParamClass; \
	static bool * _HasParametersPtr() \
	{ \
		static bool bHasParameters = false; \
		return &bHasParameters; \
	} \
	static GetParamsFunc * _GetParamsFuncPtr() \
	{ \
		static GetParamsFunc fpGetParams = NULL; \
		return &fpGetParams; \
	} \
	void SetGetParamsFunc(GetParamsFunc fp) \
	{ \
		if (fp != NULL) \
		{ \
			*_GetParamsFuncPtr() = fp; \
			*_HasParametersPtr() = true; \
		} \
		else \
		{ \
			*_GetParamsFuncPtr() = NULL; \
			*_HasParametersPtr() = false; \
		} \
	} \
	bool HasParameters() \
	{ \
		return *_HasParametersPtr(); \
	} \
	inline HRESULT _GetParamEntries(LPOLESTR* pColumnNames, DBORDINAL* pColumns, DBBINDING *pBinding, BYTE* pBuffer = NULL, bool bClearOnly = false) throw () \
	{ \
		ATLASSERT(*_GetParamsFuncPtr() != NULL); \
		return (*_GetParamsFuncPtr())(pColumnNames, pColumns, pBinding, pBuffer, bClearOnly); \
	}

#define BEGIN_PARAM_MAP_FUNC(fun) \
	public: \
	static HRESULT fun (LPOLESTR* pColumnNames, DBORDINAL* pColumns, DBBINDING *pBinding, BYTE* pBuffer = NULL, bool bClearOnly = false) throw () \
	{ \
		ATLASSERT(pColumns != NULL); \
		DBPARAMIO eParamIO = DBPARAMIO_INPUT; \
		int nColumns = 0; \
		pBuffer;

#define END_PARAM_MAP_FUNC() \
		*pColumns = nColumns; \
		return S_OK; \
	}

/*Columns Map*/
#define ENABLE_ACCESSOR_MAP(x, num) \
	public: \
	typedef HRESULT (*GetBindFunc)(LPOLESTR* pColumnNames, DBORDINAL* pColumns, DBBINDING *pBinding, ULONG nAccessor, bool* pAuto, BYTE* pBuffer, bool bClearOnly) throw(); \
	typedef x _classtype; \
	typedef x _OutputColumnsClass; \
	static bool * _HasOutputColumnsPtr() \
	{ \
		static bool bHasOutputColumns = false; \
		return &bHasOutputColumns; \
	} \
	static GetBindFunc * _GetBindFuncPtr() \
	{ \
		static GetBindFunc fpGetBindFunc = NULL; \
		return &fpGetBindFunc; \
	} \
	void SetGetBindFunc(GetBindFunc fp) \
	{ \
		if (fp != NULL) \
		{ \
			*_GetBindFuncPtr() = fp; \
			*_HasOutputColumnsPtr() = true; \
		} \
		else \
		{ \
			*_GetBindFuncPtr() = NULL; \
			*_HasOutputColumnsPtr() = false; \
		} \
	} \
	static ULONG _GetNumAccessors() { return num; } \
	bool HasOutputColumns() \
	{ \
		return *_HasOutputColumnsPtr(); \
	} \
	inline HRESULT _GetBindEntries(LPOLESTR* pColumnNames, DBORDINAL* pColumns, DBBINDING *pBinding, ULONG nAccessor, bool* pAuto, BYTE* pBuffer = NULL, bool bClearOnly = false) throw() \
	{ \
		ATLASSERT(*_GetBindFuncPtr() != NULL); \
		return ((*_GetBindFuncPtr())(pColumnNames, pColumns, pBinding, nAccessor, pAuto, pBuffer, bClearOnly)); \
	}

#define BEGIN_ACCESSOR_MAP_FUNC(fun) \
	inline static HRESULT fun (LPOLESTR* pColumnNames, DBORDINAL* pColumns, DBBINDING *pBinding, ULONG nAccessor, bool* pAuto, BYTE* pBuffer = NULL, bool bClearOnly = false) throw() \
	{ \
		ATLASSERT(pColumns != NULL); \
		DBPARAMIO eParamIO = DBPARAMIO_NOTPARAM; \
		ULONG nColumns = 0; \
		(pBuffer);

#define BEGIN_ACCESSOR_FUNC(num, bAuto) \
	if (nAccessor == num) \
	{ \
		if (pBinding != NULL) \
			*pAuto = bAuto;

#define END_ACCESSOR_FUNC() \
	} \
	else

#define END_ACCESSOR_MAP_FUNC() \
		; \
		*pColumns = nColumns; \
		return S_OK; \
	}

#define ENABLE_COLUMN_MAP(x) \
	ENABLE_ACCESSOR_MAP(x, 1)

#define BEGIN_COLUMN_MAP_FUNC(fun) \
	BEGIN_ACCESSOR_MAP_FUNC(fun) \
		BEGIN_ACCESSOR_FUNC(0, true)

#define END_COLUMN_MAP_FUNC() \
		END_ACCESSOR_FUNC() \
	END_ACCESSOR_MAP_FUNC()

#define DEFINE_ENTRY(name, type)	\
	type m_##name;					\
	DBSTATUS m_##name##Length;		\
	DBLENGTH m_##name##Status;

#define DEFINE_ENTRY_STR(name, len) \
	TCHAR m_##name[len];			\
	DBSTATUS m_##name##Length;		\
	DBLENGTH m_##name##Status;

#define COLUMN_ENTRY_LS(nOrdinal, name) \
	COLUMN_ENTRY_LENGTH_STATUS(nOrdinal, name, name##Length, name##Status)

#define COLUMN_ENTRY_TEST(params, name)			\
	int ordinal = 1;							\
	for (StringVector::iterator it = m_params.begin(); it != m_params.end(); ++it)	\
	{											\
		String &param = *it;					\
		if (param == _T(#name)) {				\
			COLUMN_ENTRY(ordinal, m_##name)

#define COLUMN_ENTRY_TEST_ELSE(name)			\
		} else if (param == _T(#name)) {		\
			COLUMN_ENTRY(ordinal, m_##name)

#define COLUMN_ENTRY_TEST_END()					\
		} else {							\
			ordinal--;						\
		}									\
		ordinal++;							\
	}

#define COLUMN_UPDATE(dbo, param, var)				\
	{												\
		dbo->m_##param## = var;						\
		dbo->AddParam(_T(#param));					\
	}

#define COLUMN_UPDATE_LARGE(dbo, param, var)		\
	{												\
		dbo->m_##param.LowPart = var;				\
		dbo->m_##param.HighPart = (var >= 0 ? 0 : 0xFFFFFFFF);	\
		dbo->AddParam(_T(#param));					\
	}

#define COLUMN_UPDATE_LARGE_QUA(dbo, param, var)	\
	{												\
		dbo->m_##param.QuadPart = var;				\
		dbo->AddParam(_T(#param));					\
	}

#define COLUMN_UPDATE_STRING(dbo, param, var) \
	{ \
		Size srclen = var.size(); \
		Size dstlen = (sizeof(dbo->m_##param) / sizeof(TCHAR)); \
		Size len = (srclen < dstlen) ? srclen : dstlen - 1; \
		::_tcsncpy(dbo->m_##param, var.c_str(), len); \
		dbo->m_##param[len] = _T('\0'); \
		dbo->AddParam(_T(#param)); \
	}

#define COLUMN_STRING_COPY(dst, src) \
	{ \
		Size srclen = ::_tcslen(src); \
		Size dstlen = (sizeof(dst) / sizeof(TCHAR)); \
		Size len = (srclen < dstlen) ? srclen : dstlen - 1; \
		::_tcsncpy(dst, src, len); \
		dst[len] = _T('\0'); \
	}

#define FIELD_FIRST(_prefix_, name)		_T(_prefix_) L#name
#define FIELD_NEXT(_prefix_, name)		L", " _T(_prefix_) L#name

#endif // _DBO_H_
