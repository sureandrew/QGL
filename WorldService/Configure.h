
#pragma once
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include "Global.h"
#include "ServiceConfigure.h"
#include <set>

#define BASE_LOAD_LEVEL			2000
#define NEXT_LOAD_LEVEL			200
#define LOGIN_INTERVAL			50
#define BROADCAST_RANK_COUNT    6

typedef StlVector<UInt32, ManagedAllocator<UInt32> > GmIdList;
struct Configure : public ServiceConfigure
{
public:
	Int baseLoadLevel;
	Int nextLoadLevel;
	Int loginInterval;
	Int eventTimeTwistMultiply;
	UInt32 eventUpdatePeriod;
	UInt32 timeZoneUpdatePeriod;
	UInt32 rankUpdatePeriod;
	UInt32 manage_interval;
	UInt16 normalLineState;
	UInt16 busyLineState;
	UInt32 antiHackTimeout;
	UInt32 day;	
	UInt32 prisonMapId;
	Int	limitCharNameLen;
	Int limitHead;
	Int limitHeadColor;
	Int limitCloth;
	Int limitClothColor;

	Reuben::Utility::ConfigFile config;
	GmIdList	gmIdList;
	Boolean		bAllGMMode;
	Boolean		bGMCheckIP;
	StlVector<String>	strGMIPList;

	SYSTEMTIME GameDateTime;

	UInt32 timeNpcFlagUpdatePeriod;

	UInt  LevelCount;
	UInt  FamousCount;
	UInt  EvilCount;
	UInt  MoneyCount;
	POINT pLevelRank[BROADCAST_RANK_COUNT];
	POINT pFamousRank[BROADCAST_RANK_COUNT];
	POINT pEvilRank[BROADCAST_RANK_COUNT];
	POINT pMoneyRank[BROADCAST_RANK_COUNT];
	UInt32 LevelrankUpdatePeriod;
	UInt32 FamousrankUpdatePeriod;
	UInt32 EvilrankUpdatePeriod;
	UInt32 MoneyrankUpdatePeriod;


	Boolean Initialize(PCSTRING pConfigFile, PCSTRING pServiceName)
	{
		strConfigFile = pConfigFile;
		strServiceName = pServiceName;
		config.SetFileName(strConfigFile.c_str());
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
		String strGMLst;
		String strGMID;

		ZeroMemory(&GameDateTime, sizeof(SYSTEMTIME));
		GameDateTime.wYear = GetPrivateProfileInt(_T("ServiceTime"), _T("YEAR"), 0, _T("Config\\LoginSetting.ini"));
		GameDateTime.wMonth = GetPrivateProfileInt(_T("ServiceTime"), _T("MOUTH"), 0, _T("Config\\LoginSetting.ini"));
		GameDateTime.wDay = GetPrivateProfileInt(_T("ServiceTime"), _T("DAY"), 0, _T("Config\\LoginSetting.ini"));
		GameDateTime.wHour = GetPrivateProfileInt(_T("ServiceTime"), _T("HOUR"), 0, _T("Config\\LoginSetting.ini"));
		baseLoadLevel = GetInt(_T("BaseLoadLevel"), BASE_LOAD_LEVEL);
		nextLoadLevel = GetInt(_T("NextLoadLevel"), NEXT_LOAD_LEVEL);
		loginInterval = GetInt(_T("LoginInterval"), LOGIN_INTERVAL);
		eventTimeTwistMultiply = GetInt(_T("EventTimeTwistMultiply"), 1);
		eventUpdatePeriod = GetInt(_T("EventUpdatePeriod"), 300000);
		timeZoneUpdatePeriod = GetInt(_T("TimeZoneUpdatePeriod"), 600000);
		rankUpdatePeriod = GetInt(_T("RankUpdatePeriod"), 3600000);
		manage_interval = GetInt(_T("ManageInterval"), 60000);
		normalLineState = GetInt(_T("NormalLineState"), 1500);
		busyLineState = GetInt(_T("BusyLineState"), 2000);
		antiHackTimeout = GetInt(_T("AntiHackTimeout"), 20) * 1000;
		day = GetInt(_T("DayDefinition"), 1440) * 60; 	
		prisonMapId = GetInt(_T("PrisonMapId"), 20320);
		limitHead = GetInt(_T("CreateCharHead"), 3);
		limitHeadColor = GetInt(_T("CreateCharHeadColor"), 3);
		limitCloth = GetInt(_T("CreateCharCloth"), 1);
		limitClothColor = GetInt(_T("CreateCharClothColor"), 1);
		timeZoneUpdatePeriod = GetInt(_T("TimeNpcFlagUpdatePeriod"), 60*1000);


		LevelrankUpdatePeriod = GetInt(_T("LevelRankUpdatePeriod"), 3600000);
		FamousrankUpdatePeriod = GetInt(_T("FamousRankUpdatePeriod"), 3600000);
		EvilrankUpdatePeriod = GetInt(_T("EvilRankUpdatePeriod"), 3600000);
		MoneyrankUpdatePeriod = GetInt(_T("MoneyRankUpdatePeriod"), 3600000);

		LevelCount =  GetPrivateProfileInt(_T("LevelRankTime"), _T("Count"), BROADCAST_RANK_COUNT, _T("Config\\LoginSetting.ini"));
		for(UINT i = 0; i<LevelCount; i++)
		{
			String temp;
			temp.Format(_T("Hour%d"),i+1);
			UInt expertHour = GetPrivateProfileInt(_T("LevelRankTime"), temp.c_str(), 0, _T("Config\\LoginSetting.ini"));
			temp.Format(_T("Minute%d"),i+1);
			UInt expertMinute = GetPrivateProfileInt(_T("LevelRankTime"), temp.c_str(), 0, _T("Config\\LoginSetting.ini"));
			pLevelRank[i].x = expertHour;
			pLevelRank[i].y = expertMinute;
		}

		FamousCount =  GetPrivateProfileInt(_T("FamousRankTime"), _T("Count"), BROADCAST_RANK_COUNT, _T("Config\\LoginSetting.ini"));
		for(UINT i = 0; i<FamousCount; i++)
		{
			String temp;
			temp.Format(_T("Hour%d"),i+1);
			UInt celebrityHour = GetPrivateProfileInt(_T("FamousRankTime"), temp.c_str(), 0, _T("Config\\LoginSetting.ini"));
			temp.Format(_T("Minute%d"),i+1);
			UInt celebrityMinute = GetPrivateProfileInt(_T("FamousRankTime"), temp.c_str(), 0, _T("Config\\LoginSetting.ini"));
			pFamousRank[i].x = celebrityHour;
			pFamousRank[i].y = celebrityMinute;
		}

		EvilCount =  GetPrivateProfileInt(_T("EvilRankTime"), _T("Count"), BROADCAST_RANK_COUNT, _T("Config\\LoginSetting.ini"));
		for(UINT i = 0; i<EvilCount; i++)
		{
			String temp;
			temp.Format(_T("Hour%d"),i+1);
			UInt villainHour = GetPrivateProfileInt(_T("EvilRankTime"), temp.c_str(), 0, _T("Config\\LoginSetting.ini"));
			temp.Format(_T("Minute%d"),i+1);
			UInt villainCountMinute = GetPrivateProfileInt(_T("EvilRankTime"), temp.c_str(), 0, _T("Config\\LoginSetting.ini"));
			pEvilRank[i].x = villainHour;
			pEvilRank[i].y = villainCountMinute;
		}

		MoneyCount =  GetPrivateProfileInt(_T("MoneyRankTime"), _T("Count"), BROADCAST_RANK_COUNT, _T("Config\\LoginSetting.ini"));
		for(UINT i = 0; i<MoneyCount; i++)
		{
			String temp;
			temp.Format(_T("Hour%d"),i+1);
			UInt nawabHour = GetPrivateProfileInt(_T("MoneyRankTime"), temp.c_str(), 0, _T("Config\\LoginSetting.ini"));
			temp.Format(_T("Minute%d"),i+1);
			UInt nawabMinute = GetPrivateProfileInt(_T("MoneyRankTime"), temp.c_str(), 0, _T("Config\\LoginSetting.ini"));
			pMoneyRank[i].x = nawabHour;
			pMoneyRank[i].y = nawabMinute;
		}

		//get back the map gm account
		strGMLst = config.GetString(_T("Map"), _T("GM"), _T(""));
		if (!strGMLst.IsEmpty())
		{
			PSTRING pToken = _tcstok(strGMLst.GetBuffer(MAX_SIZE), _T(", \t"));
			while (pToken)
			{
				strGMID = pToken;
				strGMID.Trim();
				UInt32 GMID = (UInt32) StringToInt(strGMID.c_str());
				if (GMID != 0)
					gmIdList.push_back(GMID);
				pToken = _tcstok(NULL, _T(", \t"));
			}
		}

		strGMLst = config.GetString(_T("Map"),_T("GMIPList"), _T(""));
		if (!strGMLst.IsEmpty())
		{
			PSTRING pToken = _tcstok(strGMLst.GetBuffer(MAX_SIZE), _T(", \t"));
			while (pToken)
			{
				strGMID = pToken;
				strGMID.Trim();
				String temp = strGMID;
				strGMIPList.push_back(temp);				
				pToken = _tcstok(NULL, _T(", \t"));
			}
		}

		bGMCheckIP = (config.GetInt(_T("Map"),_T("GMCheckIP"), BOOLEAN_FALSE) > 0);

		bAllGMMode = (config.GetInt(_T("Map"),_T("AllGMMode"), BOOLEAN_FALSE) > 0);

		return (BOOLEAN_TRUE);
	}

};

extern Configure gConfigure;

#endif // _CONFIGURE_H_
