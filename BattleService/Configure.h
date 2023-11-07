#pragma once
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include "ServiceConfigure.h"

struct Configure : public ServiceConfigure
{
public:

	SYSTEMTIME g_GameDateTime;
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

	virtual Boolean LoadServiceConfig(VOID)
	{
		ZeroMemory(&g_GameDateTime, sizeof(SYSTEMTIME));
		g_GameDateTime.wYear = GetPrivateProfileInt(_T("ServiceTime"), _T("YEAR"), 0, _T("Config\\LoginSetting.ini"));
		g_GameDateTime.wMonth = GetPrivateProfileInt(_T("ServiceTime"), _T("MOUTH"), 0, _T("Config\\LoginSetting.ini"));
		g_GameDateTime.wDay = GetPrivateProfileInt(_T("ServiceTime"), _T("DAY"), 0, _T("Config\\LoginSetting.ini"));
		g_GameDateTime.wHour = GetPrivateProfileInt(_T("ServiceTime"), _T("HOUR"), 0, _T("Config\\LoginSetting.ini"));

		return (BOOLEAN_TRUE);
	}

};

extern Configure gConfigure;

#endif // _CONFIGURE_H_
