//-- Common
#include "Common.h"
#include "QGL.h"
#include "Common.h"
//-- Self
#include ".\antigameaddiction.h"
//-- Library
#include "QGL/Global.h"
#include "LuaPush.h"
#include "CallLuaFunc.h"

CAntiGameAddiction::CAntiGameAddiction(void)
{




}

CAntiGameAddiction::~CAntiGameAddiction(void)
{
}

void CAntiGameAddiction::SetInitialTime(UInt32 time)
{
    m_timeZero = time;
	m_gameHealthStatus = GHEALTH_GOOD;	
	m_timeWarning = 0;	

}


void CAntiGameAddiction::UpdateGameHealthStatus(UInt32 curTime)
{
	UInt32 timelapse = curTime - m_timeZero;
	UInt32 minute = 1000 * 60;
	UInt16 nowState;
	bool popMsg = FALSE; 

	if (timelapse <= (60*minute))//within 1 hour
	{
		nowState = GHEALTH_GOOD;
	}
	else if (timelapse <= (120*minute))// 1 hour < playing time < 2 hours
	{
		nowState = GHEALTH_AVERAGE;
	}
	else if (timelapse <= (180*minute))//  2 hours < playing time < 3 hours
	{
		nowState = GHEALTH_POOR;
	}
	else if (timelapse > (180*minute)) // above 3 hours
	{
		nowState = GHEALTH_REST;
		popMsg = TRUE;

	}

	if (nowState != m_gameHealthStatus)
	{
		switch(nowState)
		{
		case GHEALTH_AVERAGE:
			{
				/// 1) change the icon to average icon 
				UIImage* gdHealthIcon = gGlobal.pUIManager->FindUIImage("PlayerHeALThgood");
				if (gdHealthIcon)
                    gdHealthIcon->SetEnable(false);
				UIImage* avgeHealthIcon = gGlobal.pUIManager->FindUIImage("PlayerHeALThaverage");
				if (avgeHealthIcon)
					avgeHealthIcon->SetEnable(true);	
				gGlobal.pUIManager->AddDirtyUIItem(avgeHealthIcon);	
				/// 2) pop up the messagebox and the system channel
				String msg; 
				msg = gGlobal.GetStringTable()->Get(_T("MSG_ANTIADDICT_ONE_HOUR"));
				gGlobal.PrintPlayerMessage(msg);	
				break;
			}
		case GHEALTH_POOR:
			{
				// 1) change the icon to poor icon 
				UIImage* avgeHealthIcon = gGlobal.pUIManager->FindUIImage("PlayerHeALThaverage");
				if (avgeHealthIcon)
					avgeHealthIcon->SetEnable(false);   
				UIImage* poorHealthIcon = gGlobal.pUIManager->FindUIImage("PlayerHeALThpoor");
				if (poorHealthIcon)
					poorHealthIcon->SetEnable(true); 
				gGlobal.pUIManager->AddDirtyUIItem(poorHealthIcon);	
				/// 2) pop up the messagebox and the system channel
				String msg;
				msg = gGlobal.GetStringTable()->Get(_T("MSG_ANTIADDICT_TWO_HOUR"));
				gGlobal.PrintPlayerMessage(msg);	
				break;
			}
		case GHEALTH_REST:
			{
			    // pop up the message box  
				String msg;
				msg = gGlobal.GetStringTable()->Get(_T("MSG_ANTIADDICT_THREE_HOUR"));
				gGlobal.PrintPlayerMessage(msg);
				break;
			}
		}
		m_gameHealthStatus = nowState;
	}

	if (popMsg)
	{
		// pop up the message box every 20 minute after 3 hours of game playing
		UInt32 timeLeft = (timelapse - 180*minute)/(20*minute);
		if (m_timeWarning != timeLeft)
		{	
			m_timeWarning = timeLeft;
			// pop up the message box
			String msg;
			msg = gGlobal.GetStringTable()->Get(_T("MSG_ANTIADDICT_TWENTY_MINUTE"));
			gGlobal.PrintPlayerMessage(msg);	
            
		}

	}


}