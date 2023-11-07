#pragma once
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include "Global.h"
#include "ServiceConfigure.h"
#include <set>

#define MAP_LINE_COUNT		50
#define MAX_LINE_STRING_SIZE 8192
#define MARRIAGETIME_MAXSIZE 48

typedef StlSet<MapIndex, std::less<MapIndex>, ManagedAllocator<MapIndex> > MapIndexSet;
typedef StlVector<UInt32, ManagedAllocator<UInt32> > GmIdList;

struct Configure : public ServiceConfigure
{
public:

	MapIndexSet	mapIndices;
	Boolean		bBattleEncount;
	SYSTEMTIME	gameDate;
	GmIdList	gmIdList;
	UInt16		maxMonitors;
	UInt32		petUpdateInterval;
	UInt32		petUpdateTimePoint;
	UInt16		petMaxLevel;
	UInt32		hackFreq;
	Boolean		bAllGMMode;
	UInt16		viewLimit;
	UInt16		viewRing;
	UInt16		viewSyncLimit;
	UInt32		viewAgeTime;
	UInt16		iHawkerTaxRate;
	UInt8       antiAddict;
	UInt32      antiAddictTime;
	UInt32		antiHackTimeout;
	UInt8		learnSkillMaxNormMobSkillA;
	UInt8		learnSkillMaxNormMobSkillB;
	UInt8		learnSkillMaxNormMobSkillC;
	UInt32		learnSkillSuccessRate;
	UInt8		changeLineTimeLimit; //in min
	UInt32		guildWarCheckTime;	//in minute
	UInt32		guildWarRebornMapID;
	UInt32		guildWarRebornMapX;	
	UInt32		guildWarRebornMapY;	
	UInt32		partyRegroupTime;
	UInt32		guildWarBoxID;
	UInt32		guildWarBoxNum;
	UInt8		expBindFactor;
	UInt32		resetAssetPassTime;	
	UInt32      studentTitle;
	UInt32		husbandTitle;
	UInt32		wifeTitle;
	UInt32		robotInterval;
	UInt32      siblingTitle;
	UInt32      antiAddictResetPoint;
	Boolean		vietnamVerAddict;
	UInt32		maxMsgLen;
	UInt32      ui32GenerationTimeMark;
	UInt16      ui16GenerationTimeLong;

	UInt32		marriageHusbandMapX;	
	UInt32		marriageHusbandMapY;
	UInt32		marriageWifeMapX;	
	UInt32		marriageWifeMapY;
	UInt32		marriageHusbandNpcId;
	UInt32		marriageWifeNpcId;	
	UInt32		marriageCharMapX;	
	UInt32		marriageCharMapY;

	UInt		uMarriageTimeCount;
	POINT		pMarriageTime[MARRIAGETIME_MAXSIZE];
	UInt32		forcePkHonorConsume;

	UInt32		IncognitoBoxID;
	UInt32		IncognitoCheckTime;
	UInt32		uGiftBoxRefreshTime;
	UInt32		noAntiHackTime;


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
		mapIndices.clear();
		gmIdList.clear();
		Unload();
	}

protected:

	virtual Boolean LoadLocalConfig(VOID)
	{
		MapIndex mapIndex;
		String strLine;
		String strLineMaps;
		String strMapId;
		String strGMLst;
		String strGMID;

		for (int i = 1; i <= MAP_LINE_COUNT; ++i)
		{
			strLine.Format(_T("Line%d"), i);
			strLineMaps = config.GetString(strServiceName.c_str(), strLine.c_str(), _T(""));
			if (strLineMaps.IsEmpty())
				continue;
			mapIndex.line_id = i;
			if (strLineMaps[0] >= _T('0') && strLineMaps[0] <= _T('9'))
			{
				// parse map id number list
				PSTRING pToken = _tcstok(strLineMaps.GetBuffer(MAX_LINE_STRING_SIZE), _T(", \t"));
				while (pToken)
				{
					strMapId = pToken;
					strMapId.Trim();

					mapIndex.map_id = (UInt16) StringToInt(strMapId.c_str());
					if (mapIndex.map_id != 0)
						mapIndices.insert(mapIndex);
					pToken = _tcstok(NULL, _T(", \t"));
				}
			}
			else
			{
				// read map id list from file
				UInt16Vector mapIdList;
				if (ReadIntList(mapIdList, strLineMaps.c_str()))
				{
					for (UInt16Vector::iterator it = mapIdList.begin(); 
						it != mapIdList.end(); ++it)
					{
						mapIndex.map_id = *it;
						mapIndices.insert(mapIndex);
					}
				}
			}
		}

		strGMLst = GetString(_T("GM"), _T(""));
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

		bBattleEncount = GetBoolean(_T("BattleEncounter"), BOOLEAN_TRUE);
		hackFreq = GetInt(_T("ServerHackFreq"), 0);
		resetAssetPassTime = 60 * GetInt(_T("ResetAssetPasswordTime"), 21600); 
		::memset(&gameDate, 0, sizeof(SYSTEMTIME));
		gameDate.wYear = GetPrivateProfileInt(_T("ServiceTime"), _T("YEAR"), 0, _T("Config\\LoginSetting.ini"));
		gameDate.wMonth = GetPrivateProfileInt(_T("ServiceTime"), _T("MOUTH"), 0, _T("Config\\LoginSetting.ini"));
		gameDate.wDay = GetPrivateProfileInt(_T("ServiceTime"), _T("DAY"), 0, _T("Config\\LoginSetting.ini"));
		gameDate.wHour = GetPrivateProfileInt(_T("ServiceTime"), _T("HOUR"), 0, _T("Config\\LoginSetting.ini"));

		maxMonitors = GetInt(_T("MaxMonitors"), 50);
		petUpdateInterval = GetInt(_T("PetUpdateInterval"), 300000);
		petUpdateTimePoint = GetInt(_T("PetUpdateTimePoint"), 60000);
		petMaxLevel = GetInt(_T("PetMaxLevel"), 10);

		bAllGMMode = GetBoolean(_T("AllGMMode"), BOOLEAN_FALSE);
		viewLimit = GetInt(_T("ViewLimit"), 100);
		viewRing = GetInt(_T("ViewRing"), 10);
		viewSyncLimit = GetInt(_T("ViewSyncLimit"), 10);
		viewAgeTime = GetInt(_T("ViewAgeTime"), 10) * 1000;
		iHawkerTaxRate = GetInt(_T("HawkerTaxRate"), 100);
		antiAddict = GetInt(_T("AntiAddiction"), 0);	
		antiAddictTime = GetInt(_T("AntiAddictTime"), 60);
		changeLineTimeLimit = GetInt(_T("ChangeLineTimeLimit"), 5);
		antiHackTimeout = GetInt(_T("AntiHackTimeout"), 20) * 1000;

		learnSkillMaxNormMobSkillA = GetFcInt(_T("LoadLearnSkill"), _T("TalentAMaxNormalSkill"), 7);
		learnSkillMaxNormMobSkillB = GetFcInt(_T("LoadLearnSkill"), _T("TalentBMaxNormalSkill"), 5);
		learnSkillMaxNormMobSkillC = GetFcInt(_T("LoadLearnSkill"), _T("TalentCMaxNormalSkill"), 3);
		learnSkillSuccessRate = GetFcInt(_T("LoadLearnSkill"), _T("SuccessRate"), 5000);

		guildWarCheckTime = GetInt(_T("GuildWarCheckTime"), 15);
		guildWarRebornMapID = GetFcInt(_T("GuildWar"), _T("RebornMapID"), 20300);
		guildWarRebornMapX = GetFcInt(_T("GuildWar"), _T("RebornMapX"), 170);
		guildWarRebornMapY = GetFcInt(_T("GuildWar"), _T("RebornMapY"), 140);
		guildWarBoxID = GetFcInt(_T("GuildWar"), _T("BoxID"), 725);
		guildWarBoxNum = GetFcInt(_T("GuildWar"), _T("BoxNum"), 20);

		partyRegroupTime = GetInt(_T("PartyRegroupTime"), 60) * 1000;
		expBindFactor = GetInt(_T("ExpBindFactor"), 8);

		studentTitle = GetInt(_T("StudentTitle"), 355);
		husbandTitle = GetInt(_T("HusbandTitle"), 357);
		wifeTitle = GetInt(_T("WifeTitle"), 356);

		robotInterval = GetFcInt(_T("Robot"), _T("Interval"), 2) * 1000;

		siblingTitle = GetInt(_T("SiblingTitle"), 358);
		antiAddictResetPoint = GetInt(_T("ResetPoint"), 0);
		vietnamVerAddict = (GetInt(_T("VietnamVersion"), 0) == 1);	
		
		ui32GenerationTimeMark = GetInt(_T("GenerationTimeMark"),101);
		ui16GenerationTimeLong = GetInt(_T("GenerationTimeLong"),360);

		maxMsgLen = GetInt(_T("MaxMsgLen"), 40);

		marriageHusbandMapX = GetInt(_T("marriageHusbandMapX"), 18);	
		marriageHusbandMapY = GetInt(_T("marriageHusbandMapY"), 57);
		marriageWifeMapX = GetInt(_T("marriageWifeMapX"), 21);	
		marriageWifeMapY = GetInt(_T("marriageWifeMapY"), 52);
		marriageHusbandNpcId = GetInt(_T("marriageHusbandNpcId"), 1102);
		marriageWifeNpcId = GetInt(_T("marriageWifeNpcId"), 1101);
		marriageCharMapX = GetInt(_T("marriageCharMapX"), 24);	
		marriageCharMapY = GetInt(_T("marriageCharMapY"), 66);

		uMarriageTimeCount = config.GetInt(_T("MARRIAGETIME"), _T("Count"), MARRIAGETIME_MAXSIZE);
		for(UINT i = 0; i<uMarriageTimeCount; i++)
		{
			String temp;
			temp.Format(_T("Hour%d"),i+1);
			UInt marriageHour = config.GetInt(_T("MARRIAGETIME"), temp.c_str(), i/2);
			temp.Format(_T("Minute%d"),i+1);
			UInt marriageMinute = config.GetInt(_T("MARRIAGETIME"), temp.c_str(), (i%2) * 30);
			pMarriageTime[i].x = marriageHour;
			pMarriageTime[i].y = marriageMinute;
		}

		forcePkHonorConsume = GetInt(_T("forcePkHonorConsume"), 1000);

		IncognitoBoxID = GetFcInt(_T("IncognitoPK"), _T("BoxID"), 314);
		IncognitoCheckTime = GetInt(_T("IncognitoCheckTime"), 15);

		uGiftBoxRefreshTime = GetInt(_T("GiftBoxRefreshTime"), 60000);

		noAntiHackTime = 60 * GetInt(_T("NoAntiHackTime"), 0);

		return (BOOLEAN_TRUE);
	}

};

extern Configure gConfigure;

#endif // _CONFIGURE_H_
