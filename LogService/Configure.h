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

struct Configure
	: public ServiceConfigure
{
public:

	String strDbConfigFile;
	Database dbLog;
	Reuben::Utility::ConfigFile configdb;
	Boolean do_log_cheat;

	Boolean Initialize(PCSTRING pConfigFile, PCSTRING pServiceName)
	{
		strConfigFile = pConfigFile;
		strServiceName = pServiceName;
		
		return (Load());
	}

	Boolean Initialize(PCSTRING pConfigFile, PCSTRING pServiceName, PCSTRING pDbFile, PCSTRING pLogDb)
	{
		strConfigFile = pConfigFile;
		strServiceName = pServiceName;
		strDbConfigFile = pDbFile;
		dbLog.strName = pLogDb;

		return (Load());
	}

	VOID Uninitialize()
	{
		strConfigFile.Empty();
		strServiceName.Empty();
		strDbConfigFile.Empty();
		dbLog.Empty();
		configdb.~ConfigFile();
		Unload();
	}

protected:

	virtual Boolean LoadLocalConfig(VOID)
	{
		// get database.ini
		if (strDbConfigFile.IsEmpty())
			strDbConfigFile = GetString(_T("DbIni"), _T("Config/Database.ini"));
		if (dbLog.strName.IsEmpty())
			dbLog.strName = GetString(_T("LogDB"), _T("LogDB"));
		
		if (!Reuben::Platform::File::IsExists(strDbConfigFile.c_str()))
			return(BOOLEAN_FALSE);
		
		configdb.SetFileName(strDbConfigFile.c_str());

		dbLog.strServer = configdb.GetString(dbLog.strName.c_str(), _T("Server"), _T(""));
		dbLog.strDatabase = configdb.GetString(dbLog.strName.c_str(), _T("Database"), _T(""));
		dbLog.strLogin = configdb.GetString(dbLog.strName.c_str(), _T("Login"), _T(""));
		dbLog.strPassword = configdb.GetString(dbLog.strName.c_str(), _T("Password"), _T(""));

		do_log_cheat = GetBoolean(_T("DoLogCheat"), 1);

		return (BOOLEAN_TRUE);
	}

};

extern Configure gConfigure;

#endif // _CONFIGURE_H_
