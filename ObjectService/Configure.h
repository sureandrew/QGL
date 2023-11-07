#pragma once
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include <Reuben\Platform\File.h>
#include "ServiceConfigure.h"

struct Database
{
	String strName;
	String strServer;
	String strDatabase;
	String strLogin;
	String strPassword;
	String strInactiveDb;

	VOID Empty() {
		strName.Empty();
		strServer.Empty();
		strDatabase.Empty();
		strLogin.Empty();
		strPassword.Empty();
		strInactiveDb.Empty();
	}
};

struct Configure
	: public ServiceConfigure
{
public:

	String strDbConfigFile;
	Database dbAccount;
	Database dbGame;
	Reuben::Utility::ConfigFile configdb;
	UInt32 updateInterval;
	UInt32 updateCount;
	UInt32 cacheTime;
	UInt8 checkInvalidData;
	UInt32 ratioPerYBT;
	Boolean multiUpdate;


	Boolean Initialize(PCSTRING pConfigFile, PCSTRING pServiceName)
	{
		strConfigFile = pConfigFile;
		strServiceName = pServiceName;
		
		return (Load());
	}

	Boolean Initialize(PCSTRING pConfigFile, PCSTRING pServiceName, PCSTRING pDbFile, 
		PCSTRING pAccoutDb, PCSTRING pGameDb)
	{
		strConfigFile = pConfigFile;
		strServiceName = pServiceName;
		strDbConfigFile = pDbFile;
		dbAccount.strName = pAccoutDb;
		dbGame.strName = pGameDb;
		
		return (Load());
	}

	VOID Uninitialize()
	{
		strConfigFile.Empty();
		strServiceName.Empty();
		strDbConfigFile.Empty();
		dbAccount.Empty();
		dbGame.Empty();
		configdb.~ConfigFile();
		Unload();
	}

protected:

	virtual Boolean LoadLocalConfig(VOID)
	{
		// get database.ini
		if (strDbConfigFile.IsEmpty())
			strDbConfigFile = GetString(_T("DbIni"), _T("Config/Database.ini"));
		if (dbAccount.strName.IsEmpty())
			dbAccount.strName = GetString(_T("AccountDB"), _T("AccountDB"));
		if (dbGame.strName.IsEmpty())
			dbGame.strName = GetString(_T("GameDB"), _T("GameDB"));

		if (!Reuben::Platform::File::IsExists(strDbConfigFile.c_str()))
			return(BOOLEAN_FALSE);

		configdb.SetFileName(strDbConfigFile.c_str());

		dbAccount.strServer = configdb.GetString(dbAccount.strName.c_str(), _T("Server"), _T(""));
		dbAccount.strDatabase = configdb.GetString(dbAccount.strName.c_str(), _T("Database"), _T(""));
		dbAccount.strLogin = configdb.GetString(dbAccount.strName.c_str(), _T("Login"), _T(""));
		dbAccount.strPassword = configdb.GetString(dbAccount.strName.c_str(), _T("Password"), _T(""));
		dbAccount.strInactiveDb = configdb.GetString(dbAccount.strName.c_str(), _T("InactiveDb"), _T(""));
		if (dbAccount.strInactiveDb.IsEmpty())
			dbAccount.strInactiveDb.Format(_T("%s_Inactive"), dbAccount.strDatabase.c_str());

		dbGame.strServer = configdb.GetString(dbGame.strName.c_str(), _T("Server"), _T(""));
		dbGame.strDatabase = configdb.GetString(dbGame.strName.c_str(), _T("Database"), _T(""));
		dbGame.strLogin = configdb.GetString(dbGame.strName.c_str(), _T("Login"), _T(""));
		dbGame.strPassword = configdb.GetString(dbGame.strName.c_str(), _T("Password"), _T(""));
		dbGame.strInactiveDb = configdb.GetString(dbGame.strName.c_str(), _T("InactiveDb"), _T(""));
		if (dbGame.strInactiveDb.IsEmpty())
			dbGame.strInactiveDb.Format(_T("%s_Inactive"), dbGame.strDatabase.c_str());

		updateInterval = GetInt(_T("UpdateInterval"), 60) * 1000;
		updateCount = GetInt(_T("UpdateCount"), 300);
		cacheTime = GetInt(_T("CacheTime"), 60) * 1000;
		checkInvalidData = GetInt(_T("CheckInvalid"), 0);
		ratioPerYBT = GetInt(_T(""), 1000);
		multiUpdate = GetBoolean(_T("MultiUpdate"), false);

		return (BOOLEAN_TRUE);
	}

};

extern Configure gConfigure;

#endif // _CONFIGURE_H_
