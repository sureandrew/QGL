//-- Common
#include "Common.h"
#include "Common/Procedure.h"

//-- Self
#include "WorldService/antigameaddiction.h"
//-- Library
#include "WorldService/Global.h"
#include "Stub/Logic/Account.h"
#include "WorldService/Configure.h"


CAntiGameAddiction::CAntiGameAddiction(void)
{

	m_onlineTime = 0;
	m_offlineTime = 0;
	m_playerInterest = 1;
	m_startGame = FALSE;	
    

}

CAntiGameAddiction::~CAntiGameAddiction(void)
{
}

void CAntiGameAddiction::LoginInitialization(CAccount *acct, UInt32 onlinTm, UInt32 oflinTm, UInt32 dislinTm)
{
	TRACE_ENSURE(acct);
	UInt32 minute = gConfigure.antiAddictTime;
	m_startGame = TRUE;	
	// set current value
	m_loginTime = (UInt32) time(NULL); 
	TRACE_INFODTL_1(GLOBAL_LOGGER, _T("CAntiGameAddiction::LoginInitialization - online time from DB: %d"), onlinTm);		
	if (onlinTm < (200 * minute))
	{
		UInt32 curTime = (UInt32) time(NULL);
		UInt32 timelapse = curTime - dislinTm;
		UInt32 demin = timelapse/(20*minute);
		timelapse = demin * (20 * minute);
		if (onlinTm > timelapse)
		{
			m_onlineTime = onlinTm - timelapse;
		}
		else
		{
			m_onlineTime = 0;
		
		}
		m_offlineTime = 0;	

	}
	else if (onlinTm >= (200 * minute))
	{
		UInt32 curTime = (UInt32) time(NULL);
		UInt32 timelapse = curTime - dislinTm;
		m_offlineTime = oflinTm + timelapse;	
		if (m_offlineTime >= (300*minute))
		{
			m_onlineTime = 0;
			m_offlineTime = 0;
		}
		else
		{
			m_onlineTime = onlinTm;		
				
		}

	}
	TRACE_INFODTL_1(GLOBAL_LOGGER, _T("CAntiGameAddiction::LoginInitialization - online time after modification: %d"), m_onlineTime);	

	if (m_onlineTime < (200 * minute)) // check when to reduce the player interest 
	{	
		acct->RaiseInterestReduction(CAccount::EVT_GHEALTH_INTEREST_HALF, 1000*(200*minute - m_onlineTime));	
		acct->RaiseInterestReduction(CAccount::EVT_GHEALTH_INTEREST_ZERO, 1000*(300*minute - m_onlineTime));
		m_playerInterest = 1;
	}
	else if (m_onlineTime < (300 * minute))
	{
		acct->RaiseInterestReduction(CAccount::EVT_GHEALTH_INTEREST_ZERO, 1000*(300*minute - m_onlineTime));
        m_playerInterest = 0.5;			
	}
	else if (m_onlineTime >= (300 * minute))
	{
		m_playerInterest = 0;
	}


	if (m_onlineTime < (180*minute)) 
	{
			
		// check when to issue the next gaming health change notice	
		acct->RaiseGameHealthChg(CAccount::EVT_GHEALTH_CHG_GDAVG, 1000*(180*minute - m_onlineTime));

		//check when to issue the next game health warning (pop up constantly for every, say, 20 or 10 minute)           
		if (m_onlineTime < (60 * minute))
		{
			acct->RaiseGameHealthWarn(CAccount::EVT_GHEALTH_FST_WARN, 1000*(60*minute - m_onlineTime));
		}
		else if (m_onlineTime < (120 * minute))
		{
			acct->RaiseGameHealthWarn(CAccount::EVT_GHEALTH_SND_WARN, 1000*(120*minute - m_onlineTime));
		}
		
		acct->RaiseCurrentHealth(CAccount::EVT_GHEALTH_GOOD, 1000*(5)); // 5 seconds

	}
	else if (m_onlineTime < (300 *minute))
	{
		
		// check when to issue the next gaming health change notice		
		acct->RaiseGameHealthChg(CAccount::EVT_GHEALTH_CHG_AVGBAD, 1000*(300*minute - m_onlineTime));

		//check when to issue the next game health warning (pop up constantly for every, say, 20 or 10 minute) 	
		UInt32 n = (m_onlineTime - 180 * minute) % (20*minute);
		acct->RaiseGameHealthWarn(CAccount::EVT_GHEALTH_THIRD_WARN, 1000*(20*minute - n));
		acct->RaiseCurrentHealth(CAccount::EVT_GHEALTH_AVG, 1000*(5));//5 second	

	}
	else if (m_onlineTime >= (300 * minute))
	{   //check when to issue the next game health warning (pop up constantly for every, say, 20 or 10 minute)
			
		UInt32 n = (m_onlineTime - 300 * minute) % (10*minute);	
		acct->RaiseGameHealthWarn(CAccount::EVT_GHEALTH_FOURTH_WARN, 1000*(10*minute - n));
		acct->RaiseCurrentHealth(CAccount::EVT_GHEALTH_BAD, 1000*(5));	 // 5 seconds
	}

	// sink the current status to client for current health status and player interest

	if (m_onlineTime >= (200 * minute))
	{
	// advise player to shut down within 5 minute, otherwise the offline time set to zero 
		acct->RaiseStopAddiction(1000*(2)); 
	}

}

void CAntiGameAddiction::AddictionSwitchOff(CAccount *acct)
{
	acct->RaiseInterestReduction(CAccount::EVT_GHEALTH_INTEREST_NORM, 5000);

}
void CAntiGameAddiction::EventHandler(LPCPEER pPeer, RPCContext &context, CAccount *acct, UInt32 event)
{
   
	UInt32 minute = gConfigure.antiAddictTime;
	switch (event)
	{
	//////////////////// for anti-game addiction event/////////////
	case CAccount::EVT_GHEALTH_FST_WARN:
		{
			acct->RaiseGameHealthWarn(CAccount::EVT_GHEALTH_SND_WARN, 1000*(60*minute)); 
			acct->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_FST_WARN")); 	

		}
		break;
	case CAccount::EVT_GHEALTH_SND_WARN:
		{

			acct->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_SND_WARN"));		
		}
		break;
	case CAccount::EVT_GHEALTH_THIRD_WARN:
		{
			UInt32 latestOnLnTm = (UInt32) time(NULL) - m_loginTime + m_onlineTime;	
			if (latestOnLnTm < (285*minute))
			{
				acct->RaiseGameHealthWarn(CAccount::EVT_GHEALTH_THIRD_WARN, 1000*(20*minute));  
				acct->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_THIRD_WARN"));	
			}

		}
		break;
	case CAccount::EVT_GHEALTH_FOURTH_WARN:
		{
			acct->RaiseGameHealthWarn(CAccount::EVT_GHEALTH_FOURTH_WARN, 1000*(10*minute)); 
			acct->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_FOURTH_WARN"));	
		}
		break;
	case CAccount::EVT_GHEALTH_CHG_GDAVG:
		{
			acct->RaiseGameHealthWarn(CAccount::EVT_GHEALTH_THIRD_WARN, 1000*(20*minute)); 
			acct->RaiseGameHealthChg(CAccount::EVT_GHEALTH_CHG_AVGBAD, 1000*(120*minute)); 
			acct->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_CHG_GDAVG"));	
			
		}
		break;
	case CAccount::EVT_GHEALTH_CHG_AVGBAD:
		{
			acct->RaiseGameHealthWarn(CAccount::EVT_GHEALTH_FOURTH_WARN, 1000*(10*minute));
			acct->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_CHG_AVGBAD"));	
			
		}
		break;
	case CAccount::EVT_GHEALTH_INTEREST_HALF:
		{
			m_playerInterest = 0.5;
			m_offlineTime = 0;	
			acct->CB_AddictionInterestChg(pPeer, context, m_playerInterest);

		}
		break;
	case CAccount::EVT_GHEALTH_INTEREST_ZERO:
		{
			m_playerInterest = 0;	
			acct->CB_AddictionInterestChg(pPeer, context, m_playerInterest);	
			
		}
		break;
	case CAccount::EVT_GHEALTH_FIVE_MIN_SHUTDWN:
		{
			acct->RaiseOffTimeClear(1000*(5*minute));
			acct->CB_AddictionShutDown(pPeer, context, _T("MSG_GAME_ADDICTION_SHUTDWN"), m_offlineTime);
		}
        break;
	case CAccount::EVT_GHEALTH_OFFTIME_CLEAR:
		{
			
			acct->CB_AddictionWarning(pPeer, context, _T("MSG_GAME_ADDICTION_OFFTIME_CLEAR"));
			m_offlineTime = 0;	
		}
        break;	
	case CAccount::EVT_GHEALTH_GOOD:
		{
			
			acct->CB_AddictCurStatus(pPeer, context, _T("MSG_GAME_HEALTH_GOOD"), m_playerInterest);
				
		}
        break;
	case CAccount::EVT_GHEALTH_AVG:
		{
			
			acct->CB_AddictCurStatus(pPeer, context, _T("MSG_GAME_HEALTH_AVG"), m_playerInterest);
			
		}
        break;
	case CAccount::EVT_GHEALTH_BAD:
		{
			
			acct->CB_AddictCurStatus(pPeer, context, _T("MSG_GAME_HEALTH_BAD"), m_playerInterest);
			
		}
        break;
	case CAccount::EVT_GHEALTH_INTEREST_NORM:
		{
			
			acct->CB_AddictionInterestChg(pPeer, context, 1);
			
		}
        break;

	}
}


Boolean CAntiGameAddiction::AntiAddictActivate()
{
    return m_startGame;

}
