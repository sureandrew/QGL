//-- Common
#include "Common.h"
#include "Common/Procedure.h"

//-- Self
#include "MapService/antigameaddiction.h"
//-- Library
#include "MapService/Global.h"
#include "Stub/Logic/Character.h"
#include "MapService/Configure.h"
#include "Proxy/Service/ObjectService.h"


CAntiGameAddiction::CAntiGameAddiction(void)
{
	m_onlineTime = 0;
	m_offlineTime = 0;
	m_playerInterest = 1;
	m_loginTime = 0;
	m_healthstatus = 0;
	m_startGame = BOOLEAN_FALSE;	
	m_bStopPlay = BOOLEAN_FALSE;
	m_bSentShutDownMsg = BOOLEAN_FALSE;	
	m_needAntiAddict = BOOLEAN_FALSE;
	m_bClearOnlineTime = BOOLEAN_FALSE;
}

CAntiGameAddiction::~CAntiGameAddiction(void)
{


}

void CAntiGameAddiction::LoginInitialization(LPCPEER pPeer, RPCContext &context, CCharacter *pchar, const AntiAddictInfo addictInfo)
{
	TRACE_ENSURE(pchar);
	UInt32 minute = gConfigure.antiAddictTime;
	m_startGame = BOOLEAN_TRUE;	
	// set current value
	m_loginTime = (UInt32) time(NULL); 
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("online time %d, disconnect time %d, login time %d"), 
										addictInfo.onlineTime, addictInfo.disconnectTime, m_loginTime);	

	if (gConfigure.vietnamVerAddict)
	{
		////////////// Vietnam Version ////////////////////
		// Reset the accumlated online time to zero if the current time already passed the "ResetPoint"
		UInt32 resetPoint = FindNextResetTime(addictInfo.disconnectTime);	
		if (m_loginTime >= resetPoint)
			m_onlineTime = 0;

		m_bClearOnlineTime = BOOLEAN_TRUE;	
		///////////////////////////////////////////////////
	}
	else
	{
		///////////// Chinese and other Version /////////////////////
		if (addictInfo.onlineTime < (200 * minute))
		{
			UInt32 curTime = (UInt32) time(NULL);
			UInt32 timelapse = curTime - addictInfo.disconnectTime;
			UInt32 demin = timelapse/(20*minute);
			timelapse = demin * (20 * minute);
			if (addictInfo.onlineTime > timelapse)
			{
				m_onlineTime = addictInfo.onlineTime - timelapse;
			}
			else
			{
				m_onlineTime = 0;
			}
			m_offlineTime = 0;	
		}
		else if (addictInfo.onlineTime >= (200 * minute))
		{
			UInt32 curTime = (UInt32) time(NULL);
			UInt32 timelapse = curTime - addictInfo.disconnectTime;
			m_offlineTime = addictInfo.offlineTime + timelapse;	
			if (m_offlineTime >= (300*minute))
			{
				m_onlineTime = 0;
				m_offlineTime = 0;
			}
			else
			{
				m_onlineTime = addictInfo.onlineTime;		
			}
		}
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("online time after modification %d"), m_onlineTime);
		if (m_onlineTime >= (200 * minute))
		{
			// advise player to shut down within 5 minute, otherwise the offline time set to zero 
			//pchar->RaiseStopAddiction(1000*(2)); 
			m_bStopPlay = BOOLEAN_TRUE;
			m_bSentShutDownMsg = BOOLEAN_TRUE;	
		}
		/////////////////////////////////////////////////////////
	}
}

void CAntiGameAddiction::SetCurrentAddictionHealth()
{
	UInt32 minute = gConfigure.antiAddictTime;	
	UInt32 now = (UInt32) time(NULL);
	UInt32 currentOnlineTime = m_onlineTime + (now - m_loginTime);

	if (currentOnlineTime < (180 * minute))
		m_healthstatus = 0;	
	else if (currentOnlineTime < (300 * minute))
		m_healthstatus = 1;		
	else if (currentOnlineTime >= (300 * minute))
		m_healthstatus = 2;		
}

void CAntiGameAddiction::RaiseAllAntiAddictionEvent(CCharacter *pchar)
{
	LPCPEER pPeer = pchar->GetPeer();
	RPCContext context = pchar->GetContext();
	UInt32 minute = gConfigure.antiAddictTime;	
	UInt32 now = (UInt32) time(NULL);
	UInt32 currentOnlineTime = m_onlineTime + (now - m_loginTime);

	//// only for Vietnam version ////
	if (m_bClearOnlineTime)
	{
		m_bClearOnlineTime = BOOLEAN_FALSE;
		UInt32 nextResetTime = FindNextResetTime(now) - now;
        pchar->RaiseGeneralGameHealthEvt(CCharacter::EVT_GHEALTH_CLEAR_ONLINETIME, 1000*nextResetTime);
	}
	//// ask the client to take rest immediately if needed ////
	if (m_bSentShutDownMsg)
	{
		m_bSentShutDownMsg = BOOLEAN_FALSE;
		pchar->CB_AddictionShutDown(pPeer, context, m_offlineTime);	
	}
	///// raise Player Interest Change Event /////
	UInt16 safeTimeZone = gConfigure.vietnamVerAddict? 180:200; 
	if (currentOnlineTime < (safeTimeZone * minute))
	{
		m_playerInterest = 1;	
		pchar->RaiseInterestReduction(CCharacter::EVT_GHEALTH_INTEREST_HALF, 1000*(safeTimeZone*minute - currentOnlineTime));
	}
	else if (currentOnlineTime < (300 * minute))
	{
		m_playerInterest = 0.5;	
		pchar->RaiseInterestReduction(CCharacter::EVT_GHEALTH_INTEREST_ZERO, 1000*(300*minute - currentOnlineTime));	
	}
	else if (currentOnlineTime >= (300 * minute))
	{
		m_playerInterest = 0;	
	}
	///// raise Health Change Event /////
	if (currentOnlineTime < (180 * minute))
	{
		m_healthstatus = 0;	
		pchar->RaiseGameHealthChg(CCharacter::EVT_GHEALTH_CHG_GDAVG, 1000*(180*minute - currentOnlineTime));
		// tell the client that the current health is good
		pchar->CB_AddictCurrentStatus(pPeer, context, _T("MSG_GAME_HEALTH_GOOD"));	

	}
	else if (currentOnlineTime < (300 * minute))
	{
		m_healthstatus = 1;		
		pchar->RaiseGameHealthChg(CCharacter::EVT_GHEALTH_CHG_AVGBAD, 1000*(300*minute - currentOnlineTime));
		// tell the client that the current health is average
		pchar->CB_AddictCurrentStatus(pPeer, context, _T("MSG_GAME_HEALTH_AVG"));	
	}
	else if (currentOnlineTime >= (300 * minute))
	{
		m_healthstatus = 2;		
		// tell the client that the current health is bad
		pchar->CB_AddictCurrentStatus(pPeer, context, _T("MSG_GAME_HEALTH_BAD"));	

	}
	///// raise Health Constant Warning /////
	if ((currentOnlineTime < (300 * minute)) && (currentOnlineTime >= (180 * minute)))
	{
		UInt32 n = (currentOnlineTime - 180 * minute) % (20*minute);
		pchar->RaiseGameHealthWarn(CCharacter::EVT_GHEALTH_THIRD_WARN, 1000*(20*minute - n));
	}
	else if (currentOnlineTime >= (300 * minute))
	{
		UInt32 n = (currentOnlineTime - 300 * minute) % (10*minute);
		pchar->RaiseGameHealthWarn(CCharacter::EVT_GHEALTH_FOURTH_WARN, 1000*(10*minute - n));        
	}
    ///// raise Health Pre-Warning /////
	if (currentOnlineTime < (60 * minute))
		pchar->RaiseGameHealthWarn(CCharacter::EVT_GHEALTH_FST_WARN, 1000*(60*minute - currentOnlineTime));
	else if (currentOnlineTime < (120 * minute))
		pchar->RaiseGameHealthWarn(CCharacter::EVT_GHEALTH_SND_WARN, 1000*(120*minute - currentOnlineTime));
	//// raise a notice that inform player to reset the offline time to zero when player hasn't quited the game within 5 minutes after login
    if ((m_bStopPlay) && ((now - m_loginTime) < (5*minute)))   
		pchar->RaiseOffTimeClear(1000*(5*minute - (now - m_loginTime)));

}

void CAntiGameAddiction::UpdateAddictInfoToDB(CCharacter* pchar, UInt8 &addition)
{
	if (m_startGame)
	{
		if (addition == 1)
		{

			UInt32 now = (UInt32) time(NULL);
			UInt32 totalOnLineTime = 0;
			if (now > m_loginTime)
			{
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("online time before adding the time spent in game %d"), m_onlineTime);
				UInt32 n = now - m_loginTime;
				totalOnLineTime = m_onlineTime + n;
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("online time after adding the time spent in game %d"), totalOnLineTime);
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("try to update the disconnect timem to DB %d"), now);	
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("logintime %d"), m_loginTime);	
			ObjectService_Proxy::UpdateAccountAddictInfo(gGlobal.pObjectService->pPeer, pchar->GetContext(), pchar->GetAcct_id(), now,
														m_offlineTime, totalOnLineTime);
		}

	}
	else
		ObjectService_Proxy::UpdateAccountAddictInfo(gGlobal.pObjectService->pPeer, pchar->GetContext(), pchar->GetAcct_id(), (UInt32) time(NULL),
											0, 0);	


}

void CAntiGameAddiction::EventHandler(LPCPEER pPeer, RPCContext &context, CCharacter *pchar, UInt32 event)
{
   
	UInt32 minute = gConfigure.antiAddictTime;
	switch (event)
	{
	//////////////////// for anti-game addiction event/////////////
	case CCharacter::EVT_GHEALTH_CLEAR_ONLINETIME:
		{
			m_onlineTime = 0;	
			m_loginTime	= (UInt32) time(NULL);
			m_bClearOnlineTime = BOOLEAN_TRUE;	
			pchar->CancelAllGameHealthEvt();
			RaiseAllAntiAddictionEvent(pchar);	
		}
		break;
	case CCharacter::EVT_GHEALTH_FST_WARN:
		{
			pchar->RaiseGameHealthWarn(CCharacter::EVT_GHEALTH_SND_WARN, 1000*(60*minute)); 
			pchar->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_FST_WARN")); 	
		}
		break;
	case CCharacter::EVT_GHEALTH_SND_WARN:
		{
			pchar->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_SND_WARN"));		
		}
		break;
	case CCharacter::EVT_GHEALTH_THIRD_WARN:
		{
			UInt32 latestOnLnTm = (UInt32) time(NULL) - m_loginTime + m_onlineTime;	
			if (latestOnLnTm < (285*minute))
			{
				pchar->RaiseGameHealthWarn(CCharacter::EVT_GHEALTH_THIRD_WARN, 1000*(20*minute));  
				pchar->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_THIRD_WARN"));	
			}
		}
		break;
	case CCharacter::EVT_GHEALTH_FOURTH_WARN:
		{
			pchar->RaiseGameHealthWarn(CCharacter::EVT_GHEALTH_FOURTH_WARN, 1000*(10*minute)); 
			pchar->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_FOURTH_WARN"));	
		}
		break;
	case CCharacter::EVT_GHEALTH_CHG_GDAVG:
		{
			m_healthstatus = 1;	 	
			pchar->RaiseGameHealthWarn(CCharacter::EVT_GHEALTH_THIRD_WARN, 1000*(20*minute)); 
			pchar->RaiseGameHealthChg(CCharacter::EVT_GHEALTH_CHG_AVGBAD, 1000*(120*minute)); 
			pchar->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_CHG_GDAVG"));
			pchar->CB_AddictCurrentStatus(pPeer, context, _T("MSG_GAME_HEALTH_AVG"));
		}
		break;
	case CCharacter::EVT_GHEALTH_CHG_AVGBAD:
		{
			m_healthstatus = 2;		
			pchar->RaiseGameHealthWarn(CCharacter::EVT_GHEALTH_FOURTH_WARN, 1000*(10*minute));
			pchar->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_CHG_AVGBAD"));
			pchar->CB_AddictCurrentStatus(pPeer, context, _T("MSG_GAME_HEALTH_BAD"));
		}
		break;
	case CCharacter::EVT_GHEALTH_INTEREST_HALF:
		{
			m_playerInterest = 0.5;
			m_offlineTime = 0;	
			pchar->RaiseInterestReduction(CCharacter::EVT_GHEALTH_INTEREST_ZERO, 1000*(100*minute));
		}
		break;
	case CCharacter::EVT_GHEALTH_INTEREST_ZERO:
		{
			m_playerInterest = 0;	
		}
		break;
	case CCharacter::EVT_GHEALTH_OFFTIME_CLEAR:
		{
			pchar->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_OFFTIME_CLEAR"));
			m_offlineTime = 0;
			m_bStopPlay = BOOLEAN_FALSE;	
		}
        break;	
	}
}
void CAntiGameAddiction::SetAddictData(AntiAddictInfo addictInfo)
{
	m_onlineTime = addictInfo.onlineTime;
	m_offlineTime = addictInfo.offlineTime;
	m_loginTime = addictInfo.loginTime;
	m_startGame = addictInfo.startHack;
	m_bStopPlay = addictInfo.stopPlay;  
	m_needAntiAddict = addictInfo.needAntiAddict;
}

UInt32 CAntiGameAddiction::FindNextResetTime(UInt32 fromTime)
{
	UInt32 resetPoint = gConfigure.antiAddictResetPoint; // the time chosen for reseting accumulated online time to zero 
	DBTIMESTAMP beginTime;	
	TimeConvert((time_t) fromTime, beginTime);
	UInt32 hourInMinute = (60 * beginTime.hour) + beginTime.minute;
	// find the next reset point (in exact date with year, month, hour ...etc)
	if (hourInMinute >= resetPoint)
		resetPoint = ((60*24 - hourInMinute) + resetPoint)*60 + fromTime;
	else
		resetPoint = (resetPoint - hourInMinute)*60 + fromTime; 
	
	return resetPoint;
}

void CAntiGameAddiction::TimeConvert(const time_t &fromTime, DBTIMESTAMP &toTime)
{
	struct tm *newtime = localtime(&fromTime);
	toTime.year		= newtime->tm_year + 1900;
	toTime.month	= newtime->tm_mon + 1;
	toTime.day		= newtime->tm_mday;
	toTime.hour		= newtime->tm_hour;
	toTime.minute	= newtime->tm_min;
	toTime.second	= newtime->tm_sec;
	toTime.fraction	= 0;
};

