#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "Common.h"
#include <oledb.h>
#include "Reuben/Math/Random.h"

#define OPT_AUTO_RESTART	0x0001		// auto restart all services when abnormal shutdown
#define OPT_AUTO_START		0x0002		// auto start all services when all managers connected
#define OPT_AUTO_STOP		0x0004		// auto stop all services when manager terminated
#define OPT_ALL				0xFFFF		// all options

#if defined(COMPILER_DEBUG)
	static const TCHAR* SRVMODE = _T("UD");
#elif defined(REUBEN_PUBLISH) 
	static const TCHAR* SRVMODE = _T("PU");
#else
	static const TCHAR* SRVMODE = _T("U");
#endif

#define COMMAND_LENGTH	1024

struct ServiceEx : public Service
{
	Boolean		bLocal;					// is local service?
	StatusesMap statuses;
	UInt32 next_running_status;
	UInt8Vector	dependList;				// dependence list
	HANDLE		hproc;					// process handle

	INLINE ServiceEx()
		: bLocal(BOOLEAN_FALSE)
		, next_running_status(SRS_UNKNOWN)
		, hproc(NULL)
	{
		serviceId = INVALID_SERVICE_ID;
		type = SERVICE_NONE;
		protocol = Reuben::Network::PROTOCOL_NONE;
		bTrust = BOOLEAN_FALSE;
		pPeer = NULL;
		bConnected = BOOLEAN_FALSE;
	}

	INLINE void Copy(const Service& service)
	{
		strName = service.strName;
		serviceId = service.serviceId;
		type = service.type;
		address = service.address;
		protocol = service.protocol;
		bTrust = service.bTrust;
		pPeer = service.pPeer;
		bConnected = service.bConnected;
	}
};

struct ProgInfo
{
	String prog;
	String title;
	String workDir;
	String cmd;
};

struct CheatCommandData
{
	UInt32 exeId;
	UInt32 acctId;
	UInt32 targetAcctId;
	UInt32 targetCharId;
	BOOL requestSent;
	String command;
};

typedef StlMap<ServiceID, ServiceEx, std::less<ServiceID>, 
			ManagedAllocator<std::pair<ServiceID, ServiceEx> > > ServiceExMap;
typedef StlMap<UInt32, Service, std::less<UInt32>, 
			ManagedAllocator<std::pair<UInt32, Service> > > ManagerMap;
typedef StlMap<LPCPEER, Service*, std::less<LPCPEER>, 
			ManagedAllocator<std::pair<LPCPEER, Service*> > > ServicePeerMap;
typedef StlMap<LPCPEER, ProgInfo, std::less<LPCPEER>, 
			ManagedAllocator<std::pair<LPCPEER, ProgInfo> > > SimClientMap;
typedef StlMap<UInt32, CheatCommandData, std::less<UInt32>,
			ManagedAllocator<std::pair<UInt32, CheatCommandData> > > CheatCommandMap;

class CDboSource;
class CdboServiceStatus;
class CdboExecuteCheat;
class CExecuteCheat;

struct Global
{
	Reuben::Math::Random	m_rand;				// random number
	ThreadKey				m_key;				// thread key
	ServiceExMap			m_srvExMap;			// service extend map
	ManagerMap				m_managerMap;		// manager service map
	ServicePeerMap			m_srvPeerMap;		// service peer map
	SimClientMap			m_simClientMap;		// SimClient peer map
	StringVector			m_commandList;		// command waiting to execute
	CheatCommandMap			m_cheatCommandMap;	// cheat command waiting to execute
	RPCContext				m_context;
	UInt32					m_option;			// manager option flag
	ServiceID				m_errServiceId;		// error service ID
	UInt32					m_schedTime;		// schedule time to do schedule command
	Char					m_schedCmd[COMMAND_LENGTH];		// schedule command to execute
	Boolean					m_bExit;			// exit flag
	Boolean					m_bCheck;			// checking flag for starting service
	Boolean					m_bWillStop;		// willing to stop flag
	Boolean					m_bStopClose;		// terminate manager when all closed
	Boolean					m_bAllStarted;		// all service start flag
	CDboSource*				m_pManageDb;			// main thread Manage DB source
	CdboServiceStatus*		m_pdboServiceStatus;	// for master manager service connect to manage database for report service status
	CdboExecuteCheat*		m_pdboExecuteCheat;	// for master manager service connect to manage database for retrival of cheat command
	CExecuteCheat*			m_pExecuteCheat;

	Global(VOID) :	m_option(0), m_errServiceId(0), m_schedTime(0),
					m_bExit(BOOLEAN_FALSE), m_bCheck(BOOLEAN_FALSE), 
					m_bWillStop(BOOLEAN_FALSE), m_bStopClose(BOOLEAN_FALSE),
					m_bAllStarted(BOOLEAN_FALSE), m_pManageDb(NULL),
					m_pdboServiceStatus(NULL), m_pdboExecuteCheat(NULL)
	{
		m_schedCmd[0] = _T('\0');
	}

	Boolean Initialize(VOID);
	VOID Uninitialize(VOID);
	Boolean ConnectDatabase();
	Boolean ReleaseOLEDB();
	void RegisterManager(LPCPEER pPeer);
	void UnregisterManager(LPCPEER pPeer);
	void AddSrvPeer(LPCPEER pPeer, Service* pService);
	Service* RemoveSrvPeer(LPCPEER pPeer);
	ServiceEx* GetServiceEx(const ServiceID serviceId);
	ServiceEx* GetServiceEx(const String &srvName);
	ServiceEx* GetServiceEx(const ServiceType type);
	void ReadDependList(ServiceEx* pService, Reuben::Utility::ConfigFile &config);
	void ReadConfig(PCSTRING configFile);
	void AllReadConfig(PCSTRING configFile);
	void ReadOption(const TCHAR *optLine);
	void ShowOption();
	void ReportStatus();
	void ResetStatus();
	Boolean ReadyStart(ServiceEx &service);
	Boolean ReadyStop(ServiceEx &service);
	void CheckAutoStart();
	void CheckAutoStop();
	Boolean CheckAllStarted();
	Boolean CheckAllStopped();
	void AddCommand(PCSTRING cmd);
	void ExecuteCommand();
	void CheckSchedule(UInt32 dt);
	HANDLE CreateProcess(LPCTSTR progName, LPCTSTR title, LPTSTR cmdLine, LPCTSTR curDir=NULL);
	Boolean LocalExecute(ServiceEx &service);
	void LocalShutdown(ServiceEx &service);
	void KickWaiting(ServiceID serviceId, UInt32 state);
	void ErrorArchive(ServiceID serviceId, Boolean localOnly);
	void CheckAllServices(Boolean localOnly);
	void StartAllServices(Boolean localOnly);
	void StopAllServices(Boolean localOnly);
	void RestartAllServices(Boolean localOnly, Boolean bErr=BOOLEAN_FALSE);
	void CheckService(ServiceID serviceId, Boolean localOnly);
	void StartService(ServiceID serviceId, Boolean localOnly);
	void StopService(ServiceID serviceId, Boolean localOnly);
	void RestartService(ServiceID serviceId, Boolean localOnly);
	void KillService(ServiceID serviceId);
	void GetDBCheatCommand();
	void MarkDBExecuteCheatRead();
	void CheatCommandDBFail();

private:
	void ScheduleCommand(UInt32 atTime, PCSTRING cmd);
	void CancelSchedule();
	void SendShutdownTime(UInt32 downTime);
};

extern Global gGlobal;

// random functions
INLINE int GetRand(int imax)
{
	return gGlobal.m_rand.GetRandom(imax);
}

INLINE int GetRand(int imin, int imax)
{
	return (gGlobal.m_rand.GetRandom(imax - imin + 1) + imin);
}

INLINE float GetRand(float fmax)
{
	float r = (float) gGlobal.m_rand.GetRandom(INT32_MAX_VALUE);

	return ((r / INT32_MAX_VALUE) * fmax);
}

INLINE float GetRand(float fmin, float fmax)
{
	float r = (float) gGlobal.m_rand.GetRandom(INT32_MAX_VALUE);

	return ((r / INT32_MAX_VALUE) * (fmax - fmin) + fmin);
}

INLINE void SetOption(UInt32 optvalue, bool enable)
{
	if (enable)
		gGlobal.m_option |= optvalue;
	else
		gGlobal.m_option &= ~optvalue;
}

INLINE bool IsOption(UInt32 optvalue)
{
	return ((gGlobal.m_option & optvalue) == optvalue);
}

#endif // _GLOBAL_H_