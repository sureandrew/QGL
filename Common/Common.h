#pragma once
#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef REUBEN_LIB
#define REUBEN_LIB
#endif

///////////////////////////////////
//		Header Include			///
///////////////////////////////////
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#ifndef USES_CONVERSION
	#include <atlbase.h>
#endif

#define GLOBAL_NAME		GetGlobalName()
#define GLOBAL_CONFIG	GetGlobalConfig()
#define GLOBAL_LOGGER	GetGlobalLogger()

//#define LUA_STACK_TRACE_SERVER	
//#define LUA_STACK_TRACE_CLIENT
#include <Reuben/System/Reuben.h>
#include <Reuben/Simulation/Simulation.h>
#include <Reuben/Simulation/State.h>
#include <Reuben/Simulation/Event.h>
#include <Reuben/Simulation/Object.h>
#include <Reuben/Simulation/Group.h>
#include <Reuben/Simulation/ObjectManager.h>
#include <Reuben/Simulation/ScheduleManager.h>
#include <Reuben/Network/Network.h>
#include <Reuben/Network/Interface.h>
#include <Reuben/Network/Peer.h>
#include <Reuben/Network/RPC.h>
#include <Reuben/Network/NetObject.h>
#include <Reuben/Network/NetGroup.h>
#include <Reuben/Utility/ConfigFile.h>
#include <Reuben/Diagnostics/Logger.h>
#include <vector>
#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
#include <deque>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

//#define	USE_VMPROTECT	1

#ifdef	USE_VMPROTECT

#include <VMProtectSDK.h>

#define CODEREPLACE_START ;
#define CODEREPLACE_END ;
#define ENCODE_START ;
#define ENCODE_END ;
#define CLEAR_START ;
#define CLEAR_END ;
#define VM_START ;
#define VM_END ;
#define VM_START_WITHLEVEL(x) ;
#define UNPROTECTED_START ;
#define UNPROTECTED_END ;
#define CHECK_PROTECTION(var, val) ;
#define CHECK_CODE_INTEGRITY(var, val) ;

#else

#include <ThemidaSDK.h>

#define VMProtectBegin ;
#define VMProtectEnd ;

#endif

///////////////////////////////////////////
//		Typedef Class/Structure			///
///////////////////////////////////////////
typedef Reuben::Network::Peer Peer;
typedef Reuben::Network::Peer* LPPEER;
typedef CONST Reuben::Network::Peer* LPCPEER;
typedef Reuben::Network::Listener Listener;
typedef Reuben::Network::RPCHeader RPCHeader;
typedef Reuben::Network::RPCContext RPCContext;
typedef Reuben::Network::Buffer Buffer;
typedef Reuben::Network::RPCID RPCID;
typedef Reuben::Network::RPCResult RPCResult;
typedef Reuben::Network::RPCMask RPCMask;
typedef Reuben::Network::Address Address;
typedef Reuben::Network::SocketID SocketID;
typedef Reuben::Network::ServiceID ServiceID;
typedef Reuben::Network::RPCNetID RPCNetID;
typedef Reuben::Network::NetObject NetObject;
typedef Reuben::Network::NetGroup NetGroup;
typedef Reuben::Network::SessionID SessionID;
typedef Reuben::Network::ProcedureID ProcedureID;
typedef Reuben::Simulation::ClassID ClassID;
typedef Reuben::Simulation::EventID EventID;
typedef Reuben::Simulation::Object Object;
typedef Reuben::Simulation::ObjectID ObjectID;
typedef Reuben::Simulation::State State;
typedef Reuben::Simulation::StateID StateID;
typedef Reuben::Simulation::Event Event;
typedef Reuben::Utility::ConfigFile ConfigFile;
typedef Reuben::Diagnostics::Logger Logger;
typedef Reuben::Network::Protocol Protocol;

///////////////////////////////////
//		Enum Definition			///
///////////////////////////////////

enum ServiceType
{
	SERVICE_NONE,
	SERVICE_LOG,
	SERVICE_WORLD,
	SERVICE_OBJECT,
	SERVICE_MESSAGE,
	SERVICE_MANAGER,
	SERVICE_PROXY,
	SERVICE_LOGIN,
	SERVICE_MAP,
	SERVICE_BATTLE,
	SERVICE_MAX
};

enum ServiceStatuses
{
	SS_RUNNING,
	SS_CONNECTION_COUNT,
	SS_WORLD_PLAYER_COUNT,
	SS_MAP_PLAYER_COUNT,
	SS_MAX
};

enum ServiceRunningStatus
{
	SRS_UNKNOWN,
	SRS_STARTED,
	SRS_STOPPED,
	SRS_STARTING,
	SRS_STOPPING,
	SRS_WAITSTART,
	SRS_WAITSTOP,
	SRS_WAITRESTART,
	SRS_ERROR,
	SRS_MAX
};

enum LoginResultEnum
{
	LOGIN_SUCCESSED,
	LOGIN_SUCCESSED_NEED_KICK,
	LOGIN_FAILED_WRONG_PASSWORD,
	LOGIN_FAILED_ACC_NOT_EXISTS,
	LOGIN_FAILED_ANTIHACK_TIMEOUT,
	LOGIN_WAITING_CHANGESTATE,
	LOGIN_VERSION_WRONG,
	LOGIN_ANTIHACKING,
	LOGIN_FAILED_KICKED,
	LOGIN_REACTIVATE_ACCOUNT,
	LOGIN_REACTIVATE_SUCCESSED,
	LOGIN_REACTIVATE_FAILED,
};

///////////////////////////////////
//		Type Definition			///
///////////////////////////////////

typedef struct Service
{
	String strName;
	ServiceID serviceId;
	ServiceType type;
	Address address;
	Protocol protocol;
	Boolean bTrust;
	LPCPEER pPeer;
	Boolean bConnected;
}
Service;

typedef StlMap<UInt32, UInt32, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, UInt32> > > StatusesMap;
typedef StlMap<ServiceID, StatusesMap, std::less<ServiceID>, ManagedAllocator<std::pair<UInt32, StatusesMap> > > ServiceStatusesMap;
typedef StlVector<UInt32, ManagedAllocator<UInt32> > UInt32Vector;
typedef StlVector<UInt16, ManagedAllocator<UInt16> > UInt16Vector;
typedef StlVector<UInt8, ManagedAllocator<UInt8> > UInt8Vector;
typedef StlVector<Int32, ManagedAllocator<Int32> > Int32Vector;
typedef StlVector<Int16, ManagedAllocator<Int16> > Int16Vector;
typedef StlVector<Int8, ManagedAllocator<Int8> > Int8Vector;
typedef StlVector<float, ManagedAllocator<float> > FloatVector;
typedef StlVector<String, ManagedAllocator<String> > StringVector;
typedef StlVector<NetObject*, ManagedAllocator<NetObject*> > NetObjVector;
typedef StlVector<SessionID, ManagedAllocator<SessionID> > SessionIDVector;
typedef StlVector<RPCNetID, ManagedAllocator<RPCNetID> > RPCNetIDVector;

///////////////////////////////////
//		Macro Definition		///
///////////////////////////////////

#define SGN(X)	(((X) == 0) ? 0 : (((X) > 0) ? 1 : -1))
#define MIN(X, Y)	(((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y)	(((X) > (Y)) ? (X) : (Y))
#define WITHIN(A, X, Y)		((A) < (X) ? (X) : ((A) > (Y) ? (Y) : (A)))
#define KEY_SIZE		256
#define KEY_DATA_SIZE	(8 + KEY_SIZE)
#define RPC_KEY_DATA	UINT16_MAX_VALUE

///////////////////////////////////
//		Function Prototype		///
///////////////////////////////////

CONST ServiceType StringToServiceType(PCSTRING pType);
PCSTRING ServiceTypeToString(CONST ServiceType eType);
Boolean IsFileExist(PCSTRING pFilename);
UInt NoCodeStringSize(String &Text);
VOID SubNoCodeString(String &Text, Int Count);
PCSTRING GetServiceStatusesString(CONST UInt32 ss);
PCSTRING GetServiceRunningStatusString(CONST UInt32 srs);
void RemoveLink(String &modifyString);

extern PCSTRING gGlobalName;
INLINE PCSTRING GetGlobalName(VOID)
{
	return (gGlobalName);
}

extern ConfigFile* gGlobalConfig;
INLINE ConfigFile& GetGlobalConfig(VOID)
{
	return (*gGlobalConfig);
}

extern Logger* gGlobalLogger;
INLINE Logger& GetGlobalLogger(VOID)
{
	return (*gGlobalLogger);
}

#include "Exception.h"

#endif // _COMMON_H_
