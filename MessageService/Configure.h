#pragma once
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include "ServiceConfigure.h"

struct Configure : public ServiceConfigure
{
public:

	UInt32		moneyChannelGlobal;
	UInt32		moneyChannelGang;
	UInt16		maxMsgLen;
	UInt32		maintainTime;
	UInt8		auraTime;
	UInt16		maxOffMsg;

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
		moneyChannelGlobal = GetFcInt(_T("Channel"), _T("MoneyChannelGlobal"), 1000);
		moneyChannelGang = GetFcInt(_T("Channel"), _T("MoneyChannelGang"), 200);
		maxMsgLen = GetInt(_T("MaxMsgLen"), 40);
		maintainTime = GetInt(_T("ManorMaintainTime"), 60) * 60;
		auraTime = GetInt(_T("AuraAssignmentTime"), 0) * 60; 	
		maxOffMsg = GetInt(_T("MaxOffMsg"), 20);
		return (BOOLEAN_TRUE);
	}

};

extern Configure gConfigure;

#endif // _CONFIGURE_H_
