#pragma once
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include "Common.h"

#include <Reuben/Utility/ConfigFile.h>
#include <Reuben/Platform/File.h>
#include <Reuben\Network\Network.h>
#include <Reuben/Diagnostics/Logger.h>

#include "Common/ChannelType.h"

#define CONFIGURATION_GA_INI_FILE _T(".\\Config\\AntiAddict.ini")
#define CONFIGURATION_IP_INI_FILE _T(".\\Config\\ipconfig.ini")
#define CONFIGURATION_PATCHSAV_INI_FILE _T(".\\Config\\patchsav.ini")
#define MARRIAGETIME_MAXSIZE 48

enum HEADICONTYPE
{
	PARTY_LEADER_ICON = 0,
	TRADING_ICON,
	BATTLE_ICON,
	OBSERVE_BATTLE_ICON,
	MISSION_ICON_EXCLAMATION_GOLD,
	MISSION_ICON_EXCLAMATION_SLIVER,
	MISSION_ICON_QUESTION_GOLD,
	MISSION_ICON_QUESTION_SLIVER,
	HELP_ICON,
	QUESTION_ICON_A,
	QUESTION_ICON_B,
	QUESTION_ICON_C,
	QUESTION_ICON_D,
	MAX_HEADICONTYPE,
};

struct Configure
{
	String						strConfigFile;
	String						strServiceName;
	UInt32						manage_interval;
	Reuben::Utility::ConfigFile config;

	Reuben::Network::IP ipProxy[10];
	Reuben::Network::Port portProxy[10];
	UInt uProxyCount;
	UInt nFullScreen;

	String strWorkingDir;
	String strPSDFile;
	String strMapFile;
	String strMapFile2;
	String strHead;
	String strBody;
	String strObject;
	String strEffect;
	String strEffectBody;
	String strWeapon;
	String strNightMaskName;
	String strShadowName;
	String strBattleEntName;
	String strBattleEntName2;
	String strHair;
	String strExtra;
	String strBattleFileName;
	String strPlayerName;

	String strHead2;
	String strBody2;
	String strWeapon2;
	String strHair2;
	String strExtra2;
	

	String strNPC;

	String strPathFindMap;

	String strFontStyle;
	String strCharName;

	String strMsgDel;

	String strChannelMsgDel[MAX_CHANNELTYPE];
	String strChannelMsgCom[MAX_CHANNELTYPE];

	String strCreateChatRoomCom;
	String strJoinChatRoomCom;
	String strLeaveChatRoomCom;
	String strCheatCom;						// cheat code command

	String HeadIconName[MAX_HEADICONTYPE];

	String strCursorNormal;
	String strCursorAttack;
	String strCursorNPC;
	String strCursorParty;
	String strCursorNextPage;
	String strCursorProtect;
	String strCursorBattleItem;
	String strCursorSkill;
	String strCursorError;
	String strCursorGeneration;

	String strCursorClick;
	String strCursorCatch;

	String strCursorSell;
	String strCursorGiven;
	String strCursorBigMapDesc;
	String strCursorTrading;
	String strCursorFriend;
	String strCursorAssetLock;
	String strCursorAssetUnLock;	

	UInt16 uPosX;
	UInt16 uPosY;

	UInt16 uNPCPosX;
	UInt16 uNPCPosY;

	UInt16 uObjectPosX;
	UInt16 uObjectPosY;
	UInt16 uNeedToExport;

	UInt uMaxSnowSpeed;
	UInt uMaxNumOfSnow;
	UInt uMaxCloudSpeed;
	UInt uMaxNumOfCloud;
	UInt uMaxRainSpeed;
	UInt uMaxNumOfRain;
	UInt uMaxFogSpeed;
	UInt uMaxNumOfFog;
	UInt uFootStepDistance;
	UInt uFootDisposalOffset;
	UInt uNetwork;

	

	UInt16 uNameFontSize;
	UInt16 uNameFontColorR;
	UInt16 uNameFontColorG;
	UInt16 uNameFontColorB;

	UInt16 uHLNameFontColorR;
	UInt16 uHLNameFontColorG;
	UInt16 uHLNameFontColorB;

	

	UInt32 uAutoLeftClickTime;
	UInt32 uAutoLeftClickFeq;

	UInt32 uClockInteval;

	UInt uSubWndMinWidth;
	UInt uSubWndMaxWidth;

	UInt uSubWndMinHeight;
	UInt uSubWndMaxHeight;

	UInt uBattleFontSyle;

	Int NameOffsetY;

	Int nValidRangeOfTalk;
	UInt32 uChooseRemainTime;

	UInt uMapID1;
	UInt uMapID2;

	UInt uSubPicSizeHalfX;
	UInt uSubPicSizeHalfY;
	BOOL RenderNightByHardware;

	UInt	uMaxSkillID;
	UInt32	uDBClkDown;

	UInt32 uMinCurPosSync;

	UInt32 uRumorMsgAppearTime;
	UInt32 uRumorMsgExistTime;
	UInt32 uRumorMsgDisposalTime;

	UInt32 uFrameRate;

	UInt32 uFollowerDiff;

	UInt32 uDisAbleSound;

	UInt	uEnableMapFrame;

	UInt	uMiniMapShow;

	UInt	uCompressedMap;
	
	UInt	uFileDataCopy;

	UInt	uCameraMaxDiff;
	UInt	uCamUpDateMin;
	UInt	uCamSpeedFactor;

	UInt32  uClientHackFreq;

	bool	bEnableNight;

	UInt	bShowWebSite;
	UInt	bAntiGameAddict;

	UInt	uStopShake;
	UInt	uShakeTime;
	UInt	uMoveHurtTime1;
	UInt	uMoveHurtTime2;
	UInt	uMoveHurtTime3;
	UInt	uHouldHurtTime;
	UInt	uHurtDistance;
	UInt	uShakeX;
	UInt	uShakeY;
	UInt	uShakeSpeedX;
	UInt	uShakeSpeedY;
	UInt	uShakeWidth;
	UInt	uShakeHeight;
	UInt	uRunSpeed;
	UInt	uMissDistance;
	UInt	uMissRunTime;
	UInt	uMissLifeTime;
	UInt	uMissAlpha;
	UInt	uMissTimeToProduce;
	UInt	uMissRate;
	UInt	uLiveTimeBase;
	UInt	uMoveSpeed;
	UInt	uDisposalTime;
	UInt	uWaitTime;
	UInt	uJumpTogether;
	UInt	uNumJumpHeight;
	UInt	uNumShake;
	UInt	uDieWidthSpeed;
	UInt	uDieWidth;
	UInt	uJumpSpeed;
	UInt	uJumpHeight;
	Int		sHurtCount;
	UInt	uEscapeFailTime;
	UInt	uEscapeFailAnim;
	UInt	uJumpEnterTime;
	UInt	uJumpEnterHeight;
	UInt	uMovingTime;
	UInt	uChatWaitCount;
	UInt	uChatBalloonSpeed;
	UInt	uNumShakeX;
	UInt	uNumShakeY;
	UInt	uNumShakeSpeedX;
	UInt	uNumShakeSpeedY;
	UInt	uNumShakeWidth;
	UInt	uNumShakeHeight;
	UInt	uNumShakeTime;
	UInt	uNumJumpDelay;
	UInt	uNumJumpTime;
	UInt	ufireWorkWidth;
	UInt	ufireWorkHeight;
	UInt	ufireWorkDelay;

	UInt	maxMsgLen;
	bool	Wrap;

	String	strServerName;

	String  strBowWait;
	String  strBowHeavenEarth;
	String  strBowForefather;
	String  strBowCouple;
	String  strBowFinish;

	UInt	uMarriageTimeCount;
	POINT   pMarriageTime[MARRIAGETIME_MAXSIZE];

	String	strWebLink;
	String	strChargeLink;
	String	strTransferLink;
	String	strCSLink;
	UInt	uballonLimit;

	String  strIncognitoBoxRed;
	String  strIncognitoBoxYellow;
	String  strIncognitoBoxGreen;

public:

	Boolean Initialize(PCSTRING pConfigFile, PCSTRING pServiceName)
	{
		strConfigFile = pConfigFile;
		strServiceName = pServiceName;
	
		if (!IsFileExist(strConfigFile.c_str()))
			return (BOOLEAN_FALSE);

		config.SetFileName(strConfigFile.c_str());

		manage_interval = (UInt32) config.GetInt(strServiceName.c_str(), _T("ManageInterval"), 60000);

		Char buf[256];
		// new format: multiple login
		uProxyCount = 0;
		while( uProxyCount < 10 )
		{
			String attribName;
			Reuben::Network::IP tmpIp;
			Reuben::Network::Port tmpPort;
			attribName.Format(_T("ProxyIP%d"), uProxyCount+1);
			//tmpIp = Reuben::Network::StringToIP(config.GetString(_T("Client"), attribName.c_str(), _T("")).c_str());
			GetPrivateProfileString(_T("Client"), attribName.c_str(), _T(""),buf , 256, CONFIGURATION_IP_INI_FILE);
			tmpIp = Reuben::Network::StringToIP(buf);
			attribName.Format(_T("ProxyPort%d"), uProxyCount+1);
			//tmpPort = Reuben::Network::HostToNet((Reuben::Network::Port)config.GetInt(_T("Client"), attribName.c_str(), 0));
			tmpPort = Reuben::Network::HostToNet((Reuben::Network::Port)GetPrivateProfileInt(_T("Client"), attribName.c_str(),0, CONFIGURATION_IP_INI_FILE));
			if( !tmpPort )
				break;
			ipProxy[uProxyCount] = tmpIp;
			portProxy[uProxyCount] = tmpPort;
			++uProxyCount;
		}
		// old config backward support
		if( !uProxyCount )
		{
			//ipProxy[0] = Reuben::Network::StringToIP(config.GetString(_T("Client"), _T("ProxyIP"), _T("")).c_str());
			//portProxy[0] = Reuben::Network::HostToNet((Reuben::Network::Port)config.GetInt(_T("Client"), _T("ProxyPort"), 0));
			GetPrivateProfileString(_T("Client"), _T("ProxyIP"), _T(""),buf , 256, CONFIGURATION_IP_INI_FILE);
			ipProxy[0] = Reuben::Network::StringToIP(buf);
			portProxy[0] = Reuben::Network::HostToNet((Reuben::Network::Port)GetPrivateProfileInt(_T("Client"), _T("ProxyPort"),0, CONFIGURATION_IP_INI_FILE));
			uProxyCount = 1;
		}

		nFullScreen = config.GetInt(_T("Client"), _T("FullScreen"), 0);

		strWorkingDir = config.GetString(_T("LOADFILE"), _T("WORKINGDIR"), _T(""));
		strPSDFile = config.GetString(_T("LOADFILE"), _T("EXPORTMAPNAME"), _T(""));
		strMapFile = config.GetString(_T("LOADFILE"), _T("INPUTMAPNAME1"), _T(""));
		strMapFile2 = config.GetString(_T("LOADFILE"), _T("INPUTMAPNAME2"), _T(""));
		strHead = config.GetString(_T("LOADFILE"), _T("HEAD"), _T(""));
		strBody = config.GetString(_T("LOADFILE"), _T("BODY"), _T(""));
		strNPC = config.GetString(_T("LOADFILE"), _T("NPC"), _T(""));
		strObject = config.GetString(_T("LOADFILE"), _T("OBJECT"), _T(""));
		strWeapon = config.GetString(_T("LOADFILE"), _T("WEAPON"), _T(""));
		strEffect = config.GetString(_T("LOADFILE"), _T("EFFECT"), _T(""));
		strEffectBody = config.GetString(_T("LOADFILE"), _T("EFFECTBODY"), _T(""));
		strNightMaskName = config.GetString(_T("LOADFILE"), _T("NIGHTMASK"), _T(""));
		strShadowName = config.GetString(_T("LOADFILE"), _T("SHADOW"), _T(""));
		strPlayerName = config.GetString(_T("LOADFILE"), _T("TESTPLAYER"), _T(""));
		strHair = config.GetString(_T("LOADFILE"), _T("HAIR"), _T(""));
		strExtra = config.GetString(_T("LOADFILE"), _T("EXTRA"), _T(""));
		uNeedToExport = config.GetInt(_T("LOADFILE"), _T("NEEDTOEXPORT"), 0);

		strHead2 = config.GetString(_T("LOADFILE"), _T("HEAD2"), _T(""));
		strBody2 = config.GetString(_T("LOADFILE"), _T("BODY2"), _T(""));
		strHair2 = config.GetString(_T("LOADFILE"), _T("HAIR2"), _T(""));
		strExtra2 = config.GetString(_T("LOADFILE"), _T("EXTRA2"), _T(""));
		strWeapon2 = config.GetString(_T("LOADFILE"), _T("WEAPON2"), _T(""));

		uPosX = config.GetInt(_T("CHAR_SETTING"), _T("POSX"), 0);
		uPosY = config.GetInt(_T("CHAR_SETTING"), _T("POSY"), 0);
		uNPCPosX = config.GetInt(_T("CHAR_SETTING"), _T("NPCPOSX"), 0);
		uNPCPosY = config.GetInt(_T("CHAR_SETTING"), _T("NPCPOSY"), 0);

		uObjectPosX = config.GetInt(_T("OBJECT_SETTING"), _T("POSX"), 0);
		uObjectPosY = config.GetInt(_T("OBJECT_SETTING"), _T("POSY"), 0);

		strBattleEntName = config.GetString(_T("BATTLESETTING"), _T("CHAR1"), _T(""));
		strBattleEntName2 = config.GetString(_T("BATTLESETTING"), _T("CHAR2"), _T(""));
		strBattleFileName = config.GetString(_T("BATTLESETTING"), _T("BATTLEBG"), _T(""));

		uNetwork = config.GetInt(_T("NETWORK"), _T("NETWORKMODE"), 0);

		uMaxSnowSpeed = config.GetInt(_T("PARTICLESYSTEM"), _T("MAXSNOWSPEED"), 0);
		uMaxNumOfSnow = config.GetInt(_T("PARTICLESYSTEM"), _T("MAXNUMOFSNOW"), 0);
		uMaxRainSpeed = config.GetInt(_T("PARTICLESYSTEM"), _T("MAXRAINSPEED"), 0);
		uMaxNumOfRain = config.GetInt(_T("PARTICLESYSTEM"), _T("MAXNUMOFRAIN"), 0);
		uMaxCloudSpeed = config.GetInt(_T("PARTICLESYSTEM"), _T("MAXCLOUDSPEED"), 0);
		uMaxNumOfCloud = config.GetInt(_T("PARTICLESYSTEM"), _T("MAXNUMOFCLOUD"), 0);
		uMaxFogSpeed = config.GetInt(_T("PARTICLESYSTEM"), _T("MAXFOGSPEED"), 0);
		uMaxNumOfFog = config.GetInt(_T("PARTICLESYSTEM"), _T("MAXNUMOFFOG"), 0);
		uNameFontSize = config.GetInt(_T("FONT"), _T("NAMEFONTSIZE"), 14);
		uNameFontColorR = config.GetInt(_T("FONT"), _T("NAMECOLORR"), 255);
		uNameFontColorG = config.GetInt(_T("FONT"), _T("NAMECOLORG"), 255);
		uNameFontColorB = config.GetInt(_T("FONT"), _T("NAMECOLORB"), 255);

		uHLNameFontColorR = config.GetInt(_T("FONT"), _T("HLNAMECOLORR"), 255);
		uHLNameFontColorG = config.GetInt(_T("FONT"), _T("HLNAMECOLORG"), 255);
		uHLNameFontColorB = config.GetInt(_T("FONT"), _T("HLNAMECOLORB"), 255);
		

		strPathFindMap = config.GetString(_T("PATHFIND"), _T("MAPFILE"), _T(""));

		strFontStyle = config.GetString(_T("FONT"), _T("FONTSTYLE"), _T(""));
		strCharName = config.GetString(_T("FONT"), _T("CHARNAME"), _T(""));

		strMsgDel = config.GetString(_T("MESSAGE"), _T("MSGDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_GLOBAL] = config.GetString(_T("MESSAGE"), _T("GLOBALMSGDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_CHATROOM] = config.GetString(_T("MESSAGE"), _T("CHATROOMDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_MAP] = config.GetString(_T("MESSAGE"), _T("MAPMSGDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_PRIVATE] = config.GetString(_T("MESSAGE"), _T("PRIVATEMSGDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_PARTY] = config.GetString(_T("MESSAGE"), _T("PARTYMSGDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_SYSTEM] = config.GetString(_T("MESSAGE"), _T("SYSTEMMSGDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_GUILD] = config.GetString(_T("MESSAGE"), _T("GUILDMSGDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_GANG] = config.GetString(_T("MESSAGE"), _T("GANGMSGDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_COUPLE] = config.GetString(_T("MESSAGE"), _T("COUPLEMSGDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_NOTICE] = config.GetString(_T("MESSAGE"), _T("SYSTEMMSGDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_FRIEND] = config.GetString(_T("MESSAGE"), _T("FRIENDMSGDEL"), _T(""));
		strChannelMsgDel[CHANNELTYPE_INSTANCE] = config.GetString(_T("MESSAGE"), _T("INSTANCEMSGDEL"), _T(""));
		

		strChannelMsgCom[CHANNELTYPE_GLOBAL] = config.GetString(_T("MESSAGE"), _T("GLOBALMSGCOM"), _T("w"));
		strChannelMsgCom[CHANNELTYPE_CHATROOM] = config.GetString(_T("MESSAGE"), _T("CHATROOMCOM"), _T("C"));
		strCreateChatRoomCom = config.GetString(_T("MESSAGE"), _T("CREATECHATROOMCOM"), _T("OC"));
		strJoinChatRoomCom = config.GetString(_T("MESSAGE"), _T("JOINGCHATROOMCOM"), _T("EC"));
		strLeaveChatRoomCom = config.GetString(_T("MESSAGE"), _T("LEAVECHATROOMCOM"), _T("LC"));
		strChannelMsgCom[CHANNELTYPE_PARTY] = config.GetString(_T("MESSAGE"), _T("PARTYMSGCOM"), _T("t"));
		strChannelMsgCom[CHANNELTYPE_GUILD] = config.GetString(_T("MESSAGE"), _T("GUILDMSGCOM"), _T("g"));
		strChannelMsgCom[CHANNELTYPE_GANG] = config.GetString(_T("MESSAGE"), _T("GANGMSGCOM"), _T("b"));
		strChannelMsgCom[CHANNELTYPE_COUPLE] = config.GetString(_T("MESSAGE"), _T("COUPLEMSGCOM"), _T("f"));
		strChannelMsgCom[CHANNELTYPE_PRIVATE] = config.GetString(_T("MESSAGE"), _T("PRIVATEMSGCOM"), _T("p"));
		strChannelMsgCom[CHANNELTYPE_MAP] = config.GetString(_T("MESSAGE"), _T("MAPMSGCOM"), _T("s"));
		strChannelMsgCom[CHANNELTYPE_SYSTEM] = config.GetString(_T("MESSAGE"), _T("SYSTEMMSGCOM"), _T("o"));
		strChannelMsgCom[CHANNELTYPE_NOTICE] = config.GetString(_T("MESSAGE"), _T("SYSTEMMSGCOM"), _T("o"));
		strChannelMsgCom[CHANNELTYPE_RUMOR] = config.GetString(_T("MESSAGE"), _T("RUMORMSGCOM"), _T("r"));
		strChannelMsgCom[CHANNELTYPE_FRIEND] = config.GetString(_T("MESSAGE"), _T("FRIENDMSGCOM"), _T("d"));
		strChannelMsgCom[CHANNELTYPE_INSTANCE] = config.GetString(_T("MESSAGE"), _T("INSTANCEMSGCOM"), _T("i"));

		strCheatCom = config.GetString(_T("MESSAGE"), _T("CHEATCOM"), _T("cheat"));

		HeadIconName[PARTY_LEADER_ICON] = config.GetString(_T("HEADICON"), _T("PARTYLEADERICON"), _T(""));
		HeadIconName[TRADING_ICON] = config.GetString(_T("HEADICON"), _T("TRADINGICON"), _T(""));
		HeadIconName[BATTLE_ICON] = config.GetString(_T("HEADICON"), _T("BATTLEICON"), _T(""));
		HeadIconName[OBSERVE_BATTLE_ICON] = config.GetString(_T("HEADICON"), _T("OBSERVEBATTLEICON"), _T(""));
		HeadIconName[MISSION_ICON_EXCLAMATION_GOLD] = config.GetString(_T("HEADICON"), _T("EXCLAMATIONGOLDICON"), _T(""));
		HeadIconName[MISSION_ICON_EXCLAMATION_SLIVER] = config.GetString(_T("HEADICON"), _T("EXCLAMATIONSLIVERICON"), _T(""));
		HeadIconName[MISSION_ICON_QUESTION_GOLD] = config.GetString(_T("HEADICON"), _T("QUESTIONGOLDICON"), _T(""));
		HeadIconName[MISSION_ICON_QUESTION_SLIVER] = config.GetString(_T("HEADICON"), _T("QUESTIONSLIVERICON"), _T(""));
		HeadIconName[HELP_ICON] = config.GetString(_T("HEADICON"), _T("HELPICON"), _T("HeadIconJ.ent"));

		HeadIconName[QUESTION_ICON_A] = config.GetString(_T("HEADICON"), _T("QUESTIONAICON"), _T(""));
		HeadIconName[QUESTION_ICON_B] = config.GetString(_T("HEADICON"), _T("QUESTIONBICON"), _T(""));
		HeadIconName[QUESTION_ICON_C] = config.GetString(_T("HEADICON"), _T("QUESTIONCICON"), _T(""));
		HeadIconName[QUESTION_ICON_D] = config.GetString(_T("HEADICON"), _T("QUESTIONDICON"), _T(""));


		strCursorNormal = config.GetString(_T("CURSOR"), _T("NOMRAL"), _T(""));
		strCursorAttack = config.GetString(_T("CURSOR"), _T("ATTACK"), _T(""));
		strCursorNPC = config.GetString(_T("CURSOR"), _T("NPC"), _T(""));
		strCursorParty = config.GetString(_T("CURSOR"), _T("PARTY"), _T(""));
		strCursorNextPage = config.GetString(_T("CURSOR"), _T("NEXTPAGE"), _T("CursorK.ent"));
		strCursorProtect = config.GetString(_T("CURSOR"), _T("PROTECT"), _T("CursorH.ent"));
		strCursorBattleItem = config.GetString(_T("CURSOR"), _T("BATTLEITEM"), _T("CursorI.ent"));
		strCursorSkill = config.GetString(_T("CURSOR"), _T("SKILLCURSOR"), _T("CursorJ.ent"));
		strCursorClick = config.GetString(_T("CURSOR"), _T("SKILLCURSOR"), _T("CursorI.ent"));
		strCursorError = config.GetString(_T("CURSOR"), _T("ERRORCURSOR"), _T("CursorL.ent"));
		strCursorGeneration = config.GetString(_T("CURSOR"),_T("GENERATIONCURSOR"),_T(""));

		strCursorCatch = config.GetString(_T("CURSOR"), _T("CATCH"), _T("CursorI.ent"));

		strCursorSell = config.GetString(_T("CURSOR"), _T("SELL"), _T("CursorO.ent"));
		strCursorGiven = config.GetString(_T("CURSOR"), _T("GIVEN"), _T("CursorE.ent"));
		strCursorBigMapDesc = config.GetString(_T("CURSOR"), _T("BIGMAPDESC"), _T("CursorS.ent"));
		strCursorTrading = config.GetString(_T("CURSOR"), _T("TRADING"), _T("CursorC.ent"));

		strCursorFriend = config.GetString(_T("CURSOR"), _T("FRIEND"), _T("CursorT.ent"));

		strCursorAssetLock = config.GetString(_T("CURSOR"), _T("ASSETLOCK"), _T("CursorW.ent"));

		strCursorAssetUnLock = config.GetString(_T("CURSOR"), _T("ASSETUNLOCK"), _T("CursorU.ent")); 	

		uAutoLeftClickTime = config.GetInt(_T("LOADFILE"), _T("AUTOLEFTCLICKTIME"), 1500);
		uAutoLeftClickFeq = config.GetInt(_T("LOADFILE"), _T("AUTOLEFTCLICKFEQ"), 75);

		uClockInteval = config.GetInt(_T("CLOCK"), _T("INTERVAL"), 5000);

		uSubWndMinWidth = config.GetInt(_T("SUBWINDOW"), _T("MINWIDTH"), 150);
		uSubWndMaxWidth = config.GetInt(_T("SUBWINDOW"), _T("MAXWIDTH"), 450);

		uSubWndMinHeight = config.GetInt(_T("SUBWINDOW"), _T("MINHEIGHT"), 125);
		uSubWndMaxHeight = config.GetInt(_T("SUBWINDOW"), _T("MAXHEIGHT"), 250);

		uBattleFontSyle = config.GetInt(_T("FONT"), _T("BATTLEFONTSTYLE"), 6);
		
		NameOffsetY = config.GetInt(_T("LOADFILE"), _T("NAMEOFFSETY"), 10);

		nValidRangeOfTalk = 101;
		uChooseRemainTime = 30000;

		uMapID1 = config.GetInt(_T("LOADFILE"), _T("INPUTMAPID1"), 0);
		uMapID2 = config.GetInt(_T("LOADFILE"), _T("INPUTMAPID2"), 0);
		uSubPicSizeHalfX = config.GetInt(_T("LOADFILE"), _T("SUBPICSIZEX"), 512);
		uSubPicSizeHalfY = config.GetInt(_T("LOADFILE"), _T("SUBPICSIZEY"), 512);

		uMaxSkillID = config.GetInt(_T("LOADFILE"), _T("MAXSKILL"), 400);

		uDBClkDown = config.GetInt(_T("LOADFILE"), _T("DBCLKDOWNTIME"), 250);

		uMinCurPosSync = config.GetInt(_T("LOADFILE"), _T("MINCURPOSSYNC"), 300);

		uRumorMsgAppearTime = config.GetInt(_T("LOADFILE"), _T("RUMORAPPEAR"), 500);
		uRumorMsgExistTime = config.GetInt(_T("LOADFILE"), _T("RUMOREXIST"), 5000);
		uRumorMsgDisposalTime = config.GetInt(_T("LOADFILE"), _T("RUMORDISPOSAL"), 2000);

		uFrameRate =  config.GetInt(_T("LOADFILE"), _T("FRAMERATE"), 60);
		uFollowerDiff =  config.GetInt(_T("CHAR_SETTING"), _T("FOLLOWERPIXELDIFF"), 40);

		uDisAbleSound =  config.GetInt(_T("SOUND"), _T("Disable"), 0);

		uEnableMapFrame = config.GetInt(_T("MAP"), _T("Update"), 1);
		uMiniMapShow = config.GetInt(_T("MAP"), _T("MiniMap"), 1);
		uCompressedMap = config.GetInt(_T("MAP"), _T("CompressedMap"), 1);

		uFileDataCopy = config.GetInt(_T("LOADFILE"), _T("COPYFILEDATA"), 0);

		uCameraMaxDiff = config.GetInt(_T("CAMERA"), _T("CAMERAMAXDIFF"), 1500);
		uCamUpDateMin = config.GetInt(_T("CAMERA"), _T("CAMERAMINDIFF"), 100);
		uCamSpeedFactor = config.GetInt(_T("CAMERA"), _T("CAMERASPEEDFACTOR"), 75);

		uClientHackFreq = config.GetInt(_T("ANTIHACK"), _T("CLIENTHACKFREQ"), 0);	

		bShowWebSite = config.GetInt(_T("WEBSITE"), _T("SHOWWEB"), 0);	
		bEnableNight = true;

		uStopShake = config.GetInt(_T("SHAKE"), _T("STOPSHAKE"), 0);	
		uShakeTime = config.GetInt(_T("SHAKE"), _T("SHAKETIME"), 0);	
		uMoveHurtTime1 = config.GetInt(_T("BATTLESETTING"), _T("HURTTIME1"), 250);	
		uMoveHurtTime2 = config.GetInt(_T("BATTLESETTING"), _T("HURTTIME2"), 500);	
		uMoveHurtTime3 = config.GetInt(_T("BATTLESETTING"), _T("HURTTIME3"), 250);
		uHouldHurtTime = config.GetInt(_T("BATTLESETTING"), _T("HOLDHURTTIME"), 250);
		uHurtDistance = config.GetInt(_T("BATTLESETTING"), _T("HURTDISTANCE"), 1);
		uShakeX = config.GetInt(_T("SHAKE"), _T("SHAKEX"), 0);
		uShakeY = config.GetInt(_T("SHAKE"), _T("SHAKEY"), 0);
		uShakeSpeedX = config.GetInt(_T("SHAKE"), _T("SHAKESPEEDX"), 0);
		uShakeSpeedY = config.GetInt(_T("SHAKE"), _T("SHAKESPEEDY"), 0);
		uShakeWidth = config.GetInt(_T("SHAKE"), _T("SHAKEWIDTH"), 0);
		uShakeHeight = config.GetInt(_T("SHAKE"), _T("SHAKEHEIGHT"), 0);
		uRunSpeed  = config.GetInt(_T("BATTLESETTING"), _T("RUNSPEED"), 0);
		uMissDistance  = config.GetInt(_T("BATTLESETTING"), _T("MISSDISTANCE"), 1);
		uMissRunTime  = config.GetInt(_T("BATTLESETTING"), _T("MISSRUNTIME"), 200);
		uMissLifeTime  = config.GetInt(_T("BATTLESETTING"), _T("MISSLIFETIME"), 200);
		uMissAlpha  = config.GetInt(_T("BATTLESETTING"), _T("MISSALPHA"), 150);
		uMissTimeToProduce = config.GetInt(_T("BATTLESETTING"), _T("MISSTIMETOPRODUCE"), 150);
		uMissRate = config.GetInt(_T("BATTLESETTING"), _T("MISSRATE"), 1);
		uLiveTimeBase = config.GetInt(_T("BATTLESETTING"), _T("LIVETIMEBASE"), 0);
		uMoveSpeed = config.GetInt(_T("BATTLESETTING"), _T("MOVESPEED"), 0);
		uDisposalTime = config.GetInt(_T("BATTLESETTING"), _T("DISPOSALTIME"), 0);
		uWaitTime = config.GetInt(_T("BATTLESETTING"), _T("WAITTIME"), 0);
		uJumpTogether = config.GetInt(_T("BATTLESETTING"), _T("JUMPTOGETHER"), 0);
		uNumJumpHeight = config.GetInt(_T("BATTLESETTING"), _T("NUMJUMPHEIGHT"), 20);
		uNumShake = config.GetInt(_T("SHAKE"), _T("NUMSHAKE"), 0);
		uDieWidthSpeed = config.GetInt(_T("BATTLESETTING"), _T("DIEWIDTHSPEED"), 400);
		uDieWidth = config.GetInt(_T("BATTLESETTING"), _T("DIEWIDTH"), 6);
		uDisposalTime = config.GetInt(_T("BATTLESETTING"), _T("DISPOSALTIME"), 0);
		uJumpSpeed = config.GetInt(_T("BATTLESETTING"), _T("JUMPTIME"), 0);
		uJumpHeight = config.GetInt(_T("BATTLESETTING"), _T("JUMPHEIGHT"), 0);
		sHurtCount = config.GetInt(_T("BATTLESETTING"), _T("HURTCOUNT"), 0);
		uEscapeFailTime = config.GetInt(_T("BATTLESETTING"), _T("ESCAPEFAILTIME"), 1000);
		uEscapeFailAnim = config.GetInt(_T("BATTLESETTING"), _T("ESCAPEFAILANIM"), 10);
		uJumpEnterTime = config.GetInt(_T("BATTLESETTING"), _T("JUMPENTERTIME"), 500);
		uJumpEnterHeight = config.GetInt(_T("BATTLESETTING"), _T("JUMPENTERTIME"), 100);
		uMovingTime = config.GetInt(_T("CATCH"), _T("MOVINGTIME"), 2000);
		uChatWaitCount = config.GetInt(_T("LOADFILE"), _T("CHATWAITCOUNT"), 500);
		uChatBalloonSpeed = config.GetInt(_T("LOADFILE"), _T("CHATBALLOONSPEED"), 200);
		uNumShakeX = config.GetInt(_T("SHAKE"), _T("NUMSHAKEX"), 0);
		uNumShakeY = config.GetInt(_T("SHAKE"), _T("NUMSHAKEY"), 0);
		uNumShakeSpeedX = config.GetInt(_T("SHAKE"), _T("NUMSHAKESPEEDX"), 200);
		uNumShakeSpeedY = config.GetInt(_T("SHAKE"), _T("NUMSHAKESPEEDY"), 0);
		uNumShakeWidth = config.GetInt(_T("SHAKE"), _T("NUMSHAKEWIDTH"), 2);
		uNumShakeHeight = config.GetInt(_T("SHAKE"), _T("NUMSHAKEHEIGHT"), 0);
		uNumShakeTime = config.GetInt(_T("SHAKE"), _T("NUMSHAKETIME"), 0);
		uNumJumpDelay =  config.GetInt(_T("BATTLESETTING"), _T("NUMJUMPDELAY"), 100);
		uNumJumpTime =  config.GetInt(_T("BATTLESETTING"), _T("NUMJUMPTIME"), 250);
		ufireWorkWidth =  config.GetInt(_T("LOADFILE"), _T("FIREWORKSWIDTH"), 90);
		ufireWorkHeight =  config.GetInt(_T("LOADFILE"), _T("FIREWORKSHEIGHT"), 90);
		ufireWorkDelay =  config.GetInt(_T("LOADFILE"), _T("FIREWORKSDELAY"), 200);

		UINT gameAddict = GetPrivateProfileInt(_T("ANTIADDICT"), _T("ENABLE"), 0, CONFIGURATION_GA_INI_FILE);
		bAntiGameAddict = (gameAddict==1 ? UInt(1) : UInt(0));

		TCHAR serverName[50];
		GetPrivateProfileString(_T("APP"), _T("lastservername"), _T(""), serverName, 50, CONFIGURATION_PATCHSAV_INI_FILE);
		serverName[49] = _T('\0');
		strServerName = serverName;

		maxMsgLen = config.GetInt(_T("Message"), _T("MaxMsgLen"), 40);
		UInt	MyWrap = config.GetInt(_T("Message"), _T("Wrap"), 0);
		Wrap = (MyWrap != 0);

		strBowWait = config.GetString(_T("MARRIAGE"), _T("BOW_WAIT"), _T("effFireworks_z0018.ent"));
		strBowHeavenEarth = config.GetString(_T("MARRIAGE"), _T("BOW_HEAVEN_EARTH"), _T("effFireworks_z0012.ent"));
		strBowForefather = config.GetString(_T("MARRIAGE"), _T("BOW_FOREFATHER"), _T("effFireworks_z0013.ent"));
		strBowCouple = config.GetString(_T("MARRIAGE"), _T("BOW_COUPLE"), _T("effFireworks_z0014.ent"));
		strBowFinish = config.GetString(_T("MARRIAGE"), _T("BOW_FINISH"), _T("effFireworks_z0015.ent"));

		strIncognitoBoxRed = config.GetString(_T("INCOGNITOPK"), _T("BOX_RED"), _T("gift1.ent"));
		strIncognitoBoxYellow = config.GetString(_T("INCOGNITOPK"), _T("BOX_YELLOW"), _T("gift2.ent"));
		strIncognitoBoxGreen = config.GetString(_T("INCOGNITOPK"), _T("BOX_GREEN"), _T("gift3.ent"));


		uMarriageTimeCount = config.GetInt(_T("MARRIAGETIME"), _T("Count"), 29);
		for(UINT i = 0; i<uMarriageTimeCount; i++)
		{
			String temp;
			temp.Format(_T("Hour%d"),i+1);
			UInt marriageHour = config.GetInt(_T("MARRIAGETIME"), temp.c_str(), 0);
			temp.Format(_T("Minute%d"),i+1);
			UInt marriageMinute = config.GetInt(_T("MARRIAGETIME"), temp.c_str(), 0);
			pMarriageTime[i].x = marriageHour;
			pMarriageTime[i].y = marriageMinute;
		}
		strWebLink = config.GetString(_T("WEBLINK"), _T("WEBSITE"), _T("http://register.51hit.com/zhidao/index.do"));
		uballonLimit =  config.GetInt(_T("LOADFILE"), _T("BALLONLIMIE"), 100);
		strChargeLink = _T("http://register.51hit.com/billing/mode.do");
		strTransferLink = _T("http://register.51hit.com/billing/transfer.do");
		strCSLink = _T("http://mg.51hit.com");

		return (BOOLEAN_TRUE);
	}

	VOID Uninitialize()
	{
		strConfigFile.Empty();
		strServiceName.Empty();
		strWorkingDir.Empty();
		strPSDFile.Empty();
		strMapFile.Empty();
		strMapFile2.Empty();
		strHead.Empty();
		strBody.Empty();
		strObject.Empty();
		strEffect.Empty();
		strEffectBody.Empty();
		strWeapon.Empty();
		strNightMaskName.Empty();
		strShadowName.Empty();
		strBattleEntName.Empty();
		strBattleEntName2.Empty();
		strHair.Empty();
		strExtra.Empty();
		strBattleFileName.Empty();
		strPlayerName.Empty();

		strHead2.Empty();
		strBody2.Empty();
		strWeapon2.Empty();
		strHair2.Empty();
		strExtra2.Empty();


		strNPC.Empty();

		strPathFindMap.Empty();

		strFontStyle.Empty();
		strCharName.Empty();

		strMsgDel.Empty();

		for (UInt i = 0; i < MAX_CHANNELTYPE; i++)
			strChannelMsgDel[i].Empty();
		for (UInt i = 0; i < MAX_CHANNELTYPE; i++)
			strChannelMsgCom[i].Empty();

		strCreateChatRoomCom.Empty();
		strJoinChatRoomCom.Empty();
		strLeaveChatRoomCom.Empty();
		strCheatCom.Empty();						// cheat code command

		for (UInt i = 0; i < MAX_HEADICONTYPE; i++)
			HeadIconName[i].Empty();

		strCursorNormal.Empty();
		strCursorAttack.Empty();
		strCursorNPC.Empty();
		strCursorParty.Empty();
		strCursorNextPage.Empty();
		strCursorProtect.Empty();
		strCursorBattleItem.Empty();
		strCursorSkill.Empty();
		strCursorError.Empty();

		strCursorClick.Empty();
		strCursorCatch.Empty();

		strCursorSell.Empty();
		strCursorGiven.Empty();
		strCursorBigMapDesc.Empty();
		strCursorTrading.Empty();
		strCursorFriend.Empty();
		strCursorAssetLock.Empty();
		strCursorAssetUnLock.Empty();	
		strBowWait.Empty();
		strBowHeavenEarth.Empty();
		strBowForefather.Empty();
		strBowCouple.Empty();
		strBowFinish.Empty();
		strWebLink.Empty();
		config.~ConfigFile();
	}
};

extern Configure gConfigure;

#endif // _CONFIGURE_H_
