//-- Common
#include "Common.h"
//-- Library
#include "BattleService/Global.h"
#include "Configure.h"
#include "Global.h"

void HelloWorld()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _T("Got Lua Call: Hello World"));
}

int GetGameTime(int Mode)
{
	Int16 wDH = 0;
	UInt32 daynight = 0;
	{
		SYSTEMTIME st;
		memset(&st, 0, sizeof(SYSTEMTIME));
		::GetLocalTime(&st);

		if ( gConfigure.g_GameDateTime.wHour > st.wHour )
			wDH = 24 - (gConfigure.g_GameDateTime.wHour - st.wHour);
		else
			wDH = (st.wHour - gConfigure.g_GameDateTime.wHour);
		 
		daynight = (UInt32)(((wDH % 2) * 60 + st.wMinute) / 10);
	}
	daynight %= 12;

	
	if ( Mode == 0 )
	{// Check Day , Night
		if ( daynight >= 3 && daynight <= 8 )
			return 0;
		else
			return 1;
	}
	else 
	{// Get Game Time
		return ((Int)daynight);
	}
}

void SetTempResult(double result)
{
	gGlobal.g_TempResult = result;
}

double GetTempResult()
{
	return gGlobal.g_TempResult;
}