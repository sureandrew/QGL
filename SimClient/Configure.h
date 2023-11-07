#pragma once
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include <Reuben/Platform/File.h>

struct Configure
{
public:

	String						strConfigFile;
	String						strServiceName;
	UInt32						manage_interval;
	Reuben::Utility::ConfigFile config;

	String	logFile;
	Int		logLevel;
	Boolean	logAppend;
	Boolean	logConsole;
	Int		logSize;

	Reuben::Network::IP ipProxy[10];
	Reuben::Network::Port portProxy[10];
	UInt uProxyCount;
	Reuben::Network::Port managerPort;
	String	option;
	String	zoneDataPath;

	Boolean Initialize(PCSTRING pConfigFile, PCSTRING pServiceName)
	{
		strConfigFile = pConfigFile;
		strServiceName = pServiceName;

		if (!IsFileExist(strConfigFile.c_str()))
			return (BOOLEAN_FALSE);

		config.SetFileName(strConfigFile.c_str());

		manage_interval = (UInt32) config.GetInt(strServiceName.c_str(), _T("ManageInterval"), 60000);

		logFile = config.GetString(_T("SimClient"), _T("LogFile"), _T(""));
		logLevel = config.GetInt(_T("SimClient"), _T("LogLevel"), 4);
		logAppend = (config.GetInt(_T("SimClient"), _T("LogAppend"), 0)) > 0;
		logConsole = (config.GetInt(_T("SimClient"), _T("LogConsole"), 1)) > 0;
		logSize = config.GetInt(_T("SimClient"), _T("LogSize"), 0);

		// new format: multiple login
		uProxyCount = 0;
		while( uProxyCount < 10 )
		{
			String attribName;
			Reuben::Network::IP tmpIp;
			Reuben::Network::Port tmpPort;
			attribName.Format(_T("ProxyIP%d"), uProxyCount+1);
			tmpIp = Reuben::Network::StringToIP(config.GetString(_T("Client"), attribName.c_str(), _T("")).c_str());
			attribName.Format(_T("ProxyPort%d"), uProxyCount+1);
			tmpPort = Reuben::Network::HostToNet((Reuben::Network::Port)config.GetInt(_T("Client"), attribName.c_str(), 0));
			if( !tmpPort )
				break;
			ipProxy[uProxyCount] = tmpIp;
			portProxy[uProxyCount] = tmpPort;
			++uProxyCount;
		}
		// old config backward support
		if( !uProxyCount )
		{
			ipProxy[0] = Reuben::Network::StringToIP(config.GetString(_T("Client"), _T("ProxyIP"), _T("")).c_str());
			portProxy[0] = Reuben::Network::HostToNet((Reuben::Network::Port)config.GetInt(_T("Client"), _T("ProxyPort"), 0));
			uProxyCount = 1;
		}

		option = config.GetString(_T("SimClient"), _T("Option"), _T(""));
		managerPort = Reuben::Network::HostToNet((Reuben::Network::Port)config.GetInt(_T("SimClient"), _T("ManagerPort"), 0));
		zoneDataPath = config.GetString(_T("ExportFileConfiguration"), _T("ExportClientZoneDataDirectory"), _T("Data\\DAT"));

		return (BOOLEAN_TRUE);
	}

	VOID Uninitialize()
	{
		strConfigFile.Empty();
		strServiceName.Empty();
		logFile.Empty();
		option.Empty();
		zoneDataPath.Empty();
		config.~ConfigFile();
	}

};

extern Configure gConfigure;

#endif // _CONFIGURE_H_
