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

	VOID Empty()
	{
		strName.Empty();
		strServer.Empty();
		strDatabase.Empty();
		strLogin.Empty();
		strPassword.Empty();
	}
};

struct Configure : public ServiceConfigure
{
public:

	String strDbConfigFile;
	Database dbManage;
	Reuben::Utility::ConfigFile configdb;
	String strOption;
	UInt32 graceStopTime;
	UInt32 shutdownTime;
	UInt32 cheatRetrievalTime;

	Boolean Initialize(PCSTRING pConfigFile, PCSTRING pServiceName)
	{
		strConfigFile = pConfigFile;
		strServiceName = pServiceName;
		cheatRetrievalTime = (UINT32)UINT32_MAX_VALUE;

		
		return (Load());
	}

	VOID Uninitialize()
	{
		strConfigFile.Empty();
		strServiceName.Empty();
		strDbConfigFile.Empty();
		dbManage.Empty();
		configdb.~ConfigFile();
		strOption.Empty();
		Unload();
	}

protected:

	virtual Boolean LoadLocalConfig(VOID)
	{
		// get database.ini
		if (strDbConfigFile.IsEmpty())
			strDbConfigFile = config.GetString(strServiceName.c_str(), _T("DbIni"), _T("Config/Database.ini"));
		if (dbManage.strName.IsEmpty())
			dbManage.strName = config.GetString(strServiceName.c_str(), _T("ManageDB"), _T("ManageDB"));

		if (!Reuben::Platform::File::IsExists(strDbConfigFile.c_str()))
			return(BOOLEAN_FALSE);
		
		configdb.SetFileName(strDbConfigFile.c_str());

		dbManage.strServer = configdb.GetString(dbManage.strName.c_str(), _T("Server"), _T(""));
		dbManage.strDatabase = configdb.GetString(dbManage.strName.c_str(), _T("Database"), _T(""));
		dbManage.strLogin = configdb.GetString(dbManage.strName.c_str(), _T("Login"), _T(""));
		dbManage.strPassword = configdb.GetString(dbManage.strName.c_str(), _T("Password"), _T(""));

		strOption = GetString(_T("option"), _T(""));
		graceStopTime = GetInt(_T("GraceStopTime"), 10) * 1000;
		shutdownTime = GetInt(_T("ShutdownTime"), 10) * 1000;
		cheatRetrievalTime = GetInt(_T("CheatRetrievalTime"), (UINT32_MAX_VALUE/1000)) * 1000;

		return (BOOLEAN_TRUE);
	}

};

extern Configure gConfigure;

#endif // _CONFIGURE_H_
