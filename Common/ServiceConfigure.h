#pragma once
#ifndef _SERVICE_CONFIGURE_H_
#define _SERVICE_CONFIGURE_H_

#include <Reuben/Utility/ConfigFile.h>

#ifndef _NO_LOCAL_RPC_MANAGER_
	#include "Common/LocalRPCManager.h"
#endif

typedef StlMap<Reuben::Network::ServiceID, Service, std::less<Reuben::Network::ServiceID>, ManagedAllocator<std::pair<Reuben::Network::ServiceID, Service> > > ServiceMap;

#define GENERAL_SECT			_T("General")
#define FILE_CONFIG				_T(".\\Data\\Setting\\fileconfig.ini")

struct ServiceConfigure
{
public:
	Reuben::Utility::ConfigFile config;

	String						strConfigFile;
	String						strServiceName;
	Reuben::Network::ServiceID	serviceId;
	ServiceType					type;
	String						typeName;
	Reuben::Network::IP			ip;
	Reuben::Network::Port		port;
	ServiceMap					services;
	Int							maxInQueue;
	Int							maxConnection;
	UInt32						manage_interval;
	String						logFile;
	Boolean						logConsole;
	Boolean						logAppend;
	UInt8						logLevel;
	Int							logSize;
	String						logArchive;

private:

	Boolean LoadCommonConfig(VOID)
	{
		//-- specified service setting
		serviceId = (Reuben::Network::ServiceID)config.GetInt(strServiceName.c_str(), _T("ServiceID"), 0);
		typeName = config.GetString(strServiceName.c_str(), _T("Type"), _T(""));
		type = StringToServiceType(typeName.c_str());

		ip = Reuben::Network::StringToIP(
			config.GetString(strServiceName.c_str(), _T("IP"), _T("")).c_str());
		port = Reuben::Network::HostToNet((Reuben::Network::Port)config.GetInt(
			strServiceName.c_str(), _T("Port"), 0));

		// settings can specify in general section
		maxInQueue = GetInt(strServiceName.c_str(), typeName.c_str(), GENERAL_SECT, 
							_T("MaxInQueue"), 0);
		maxConnection = GetInt(strServiceName.c_str(), typeName.c_str(), GENERAL_SECT, 
							_T("MaxConnection"), 0);
		manage_interval = GetInt(strServiceName.c_str(), typeName.c_str(), GENERAL_SECT,
						_T("ManageInterval"), 60000);

		// log file setting
		String defLogFile;
		defLogFile.Format(_T("Log\\%s.log"), strServiceName);
		logFile = GetString(strServiceName.c_str(), NULL, _T("LogFile"), defLogFile.c_str());
		logConsole = GetBoolean(strServiceName.c_str(), typeName.c_str(), GENERAL_SECT, 
					_T("LogConsole"), BOOLEAN_FALSE);
		logAppend = GetBoolean(strServiceName.c_str(), typeName.c_str(), GENERAL_SECT,
					_T("LogAppend"), BOOLEAN_FALSE);
		logLevel = GetInt(strServiceName.c_str(), typeName.c_str(), GENERAL_SECT,
					_T("LogLevel"), 4);
		logSize = GetInt(strServiceName.c_str(), typeName.c_str(), GENERAL_SECT,
					_T("LogSize"), 100);
		logArchive = GetString(strServiceName.c_str(), typeName.c_str(), GENERAL_SECT,
					_T("LogArchive"), NULL);
	
		//-- Load Other Services for Connect
		String strName;
		String strServices = GetString(_T("Services"), _T(""));
		PSTRING pToken = ::_tcstok(strServices.GetBuffer(MAX_SIZE), _T(", \t"));
		while (pToken != NULL)
		{
			strName = pToken;
			strName.Trim();
			Service service;
			service.strName = pToken;
			service.serviceId = (Reuben::Network::ServiceID)config.GetInt(strName.c_str(), _T("ServiceID"), 0);
			service.type = StringToServiceType(config.GetString(strName.c_str(), _T("Type"), _T("")).c_str());;
			service.protocol = Reuben::Network::PROTOCOL_TCP;
			service.bTrust = BOOLEAN_TRUE;
			service.address = Address(Reuben::Network::StringToIP(config.GetString(pToken, _T("IP"), _T("")).c_str()),
				Reuben::Network::HostToNet((Reuben::Network::Port)config.GetInt(pToken, _T("Port"), 0)));
			service.pPeer = NULL;
			service.bConnected = BOOLEAN_FALSE;
			services[service.serviceId] = service;
			pToken = ::_tcstok(NULL, _T(", \t"));
		}

		return (BOOLEAN_TRUE);
	}

protected:

	virtual Boolean LoadLocalConfig(VOID) = 0;

public:

	Boolean Load(VOID)
	{
		if (!IsFileExist(strConfigFile.c_str()))
			return (BOOLEAN_FALSE);

		config.SetFileName(strConfigFile.c_str());

		if (!LoadCommonConfig())
			return (BOOLEAN_FALSE);

		if (!LoadLocalConfig())
			return (BOOLEAN_FALSE);

		return (BOOLEAN_TRUE);
	}

	void Unload(VOID)
	{
		strConfigFile.Empty();
		strServiceName.Empty();
		typeName.Empty();
		logFile.Empty();
		logArchive.Empty();
		services.clear();
		config.~ConfigFile();
	}

	///////////////////////////////////////////////
	// reading integer or string from World.ini

	Boolean HasKey(PCSTRING pSection, PCSTRING pKey)
	{
		String nv("_N*E_");
		return (config.GetString(pSection, pKey, nv.c_str()) != nv);
	}

	template <typename T>
	INLINE T GetInt(PCSTRING pSection, PCSTRING pDefSect, PCSTRING pKey, CONST T defValue)
	{
		Int v;

		if (pDefSect != NULL)
		{
			Int nv = ~defValue;
			v = config.GetInt(pSection, pKey, nv);
			if (v == nv)
				v = config.GetInt(pDefSect, pKey, (Int) defValue);
		}
		else
			v = config.GetInt(pSection, pKey, (Int) defValue);
		return ((T) v);
	}

	template <typename T>
	INLINE T GetInt(PCSTRING pKey, CONST T defValue)
	{
		return GetInt(strServiceName.c_str(), 
			strServiceName != typeName ? typeName.c_str() : NULL, pKey, defValue);
	}

	template <typename T>
	INLINE T GetInt(PCSTRING pSection, PCSTRING pSect2, PCSTRING pSect3, PCSTRING pKey, 
					CONST T defValue)
	{
		Boolean hasKey = HasKey(pSection, pKey);
		return GetInt(hasKey ? pSection : pSect2, hasKey ? NULL : pSect3, pKey, defValue);
	}

	INLINE Boolean GetBoolean(PCSTRING pSection, PCSTRING pDefSect, PCSTRING pKey, 
								CONST Boolean defValue)
	{
		return (GetInt<Int>(pSection, pDefSect, pKey, defValue) > 0);
	}

	INLINE Boolean GetBoolean(PCSTRING pKey, CONST Boolean defValue)
	{
		return (GetInt<Int>(strServiceName.c_str(), 
			strServiceName != typeName ? typeName.c_str() : NULL, pKey, defValue) > 0);
	}

	INLINE Boolean GetBoolean(PCSTRING pSection, PCSTRING pSect2, PCSTRING pSect3, 
							PCSTRING pKey, CONST Boolean defValue)
	{
		Boolean hasKey = HasKey(pSection, pKey);
		return GetBoolean(hasKey ? pSection : pSect2, hasKey ? NULL : pSect3, pKey, defValue);
	}

	INLINE String GetString(PCSTRING pSection, PCSTRING pDefSect, PCSTRING pKey, 
							PCSTRING defValue)
	{
		String v;

		if (pDefSect != NULL)
		{
			String nv("_N*E_");
			v = config.GetString(pSection, pKey, nv.c_str());
			if (v == nv)
				v = config.GetString(pDefSect, pKey, defValue);
		}
		else
			v = config.GetString(pSection, pKey, defValue);
		return v;
	}

	INLINE String GetString(PCSTRING pKey, PCSTRING defValue)
	{
		return GetString(strServiceName.c_str(), 
			strServiceName != typeName ? typeName.c_str() : NULL, pKey, defValue);
	}

	INLINE String GetString(PCSTRING pSection, PCSTRING pSect2, PCSTRING pSect3, 
							PCSTRING pKey, PCSTRING defValue)
	{
		Boolean hasKey = HasKey(pSection, pKey);
		return GetString(hasKey ? pSection : pSect2, hasKey ? NULL : pSect3, pKey, defValue);
	}

	///////////////////////////////////////////////////
	// reading integer or string from fileconfig.ini

	template <typename T>
	INLINE T GetFcInt(PCSTRING pSection, PCSTRING pKey, CONST T defValue)
	{
		Int v;

		v = ::GetPrivateProfileInt(pSection, pKey, defValue, FILE_CONFIG);
		return ((T) v);
	}

	INLINE Boolean GetFcBoolean(PCSTRING pSection, PCSTRING pKey, CONST Boolean defValue)
	{
		return (::GetPrivateProfileInt(pSection, pKey, defValue, FILE_CONFIG) > 0);
	}

	INLINE String GetFcString(PCSTRING pSection, PCSTRING pKey, PCSTRING defValue)
	{
		String strValue;
		::GetPrivateProfileString(pSection, pKey, defValue, 
			strValue.GetBuffer(MAX_LINE_STRING_SIZE), MAX_LINE_STRING_SIZE, FILE_CONFIG);
		int pos = strValue.Find(_T(';'));
		if (pos >= 0)
			strValue = strValue.substr(0, pos);
		strValue.Trim();
		strValue.ReleaseBuffer();
		return (strValue);
	}

	template <typename T>
	bool ReadIntList(StlVector<T, ManagedAllocator<T> >& intList, PCSTRING filename)
	{
		FILE *fp;
		TCHAR line[1024];

		fp = _tfopen(filename, _T("rt"));
		if (fp == NULL)
			return false;

		intList.clear();
		while (!feof(fp))
		{
			_fgetts(line, 1024, fp);
			String s(line);
			s.Trim();
			if (!s.IsEmpty())
				intList.push_back((T) _ttoi(s.c_str()));
		}
		fclose(fp);
		return true;
	}
};

#endif // _SERVICE_CONFIGURE_H_
