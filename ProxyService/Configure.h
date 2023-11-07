#pragma once
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#define _NO_LOCAL_RPC_MANAGER_
#include "ServiceConfigure.h"

struct Configure : public ServiceConfigure
{
public:

	Reuben::Network::IP			publicIp;
	Reuben::Network::Port		publicPort;
	UInt						publicMaxInQueue;
	UInt						publicMaxConnection;
	Boolean						giveHackAnswer;
	UInt						antihackStyle;	
	Int							max_out_pending;
	Int							apolloInteval;

	Boolean Initialize(PCSTRING pConfigFile, PCSTRING pServiceName)
	{
		strConfigFile = pConfigFile;
		strServiceName = pServiceName;

		return (Load());
	}

	VOID Uninitialize()
	{
		strConfigFile.Empty();
		strServiceName.Empty();
		Unload();
	}

protected:

	virtual Boolean LoadLocalConfig(VOID)
	{
		publicIp = Reuben::Network::StringToIP(config.GetString(strServiceName.c_str(), _T("PublicIP"), _T("")).c_str());
		publicPort = Reuben::Network::HostToNet((Reuben::Network::Port)config.GetInt(strServiceName.c_str(), _T("PublicPort"), 0));
		publicMaxInQueue = GetInt(_T("PublicMaxInQueue"), 0);
		publicMaxConnection = GetInt(_T("PublicMaxConnection"), 0);
		giveHackAnswer = GetBoolean(_T("GiveHackAnswer"), 0);
		antihackStyle = GetInt(_T("AntiHackStyle"), 0);	
		max_out_pending = GetInt(_T("MaxOutPending"), 20);
		apolloInteval = GetInt(_T("ApolloInterval"), 30);
		return (BOOLEAN_TRUE);
	}

};

extern Configure gConfigure;

#endif // _CONFIGURE_H_
