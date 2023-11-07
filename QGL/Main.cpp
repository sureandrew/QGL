#include "Common.h"
#include "QGL.h"
#include "LocalRPCManager.h"
#include "QGLApplication.h"
#include "Math.h"
#include "Global.h"
#include "Configure.h"
#include "Proxy/View/GlobalView.h"
#include "Misc/Stackwalker.h"
#include "Version.h"

#include "ApolloClient.h"
#include "Proxy/Service/WorldService.h"

#include <windows.h>
#include <windowsx.h>

#define LOG_NAME		_T("QGL")
#define LOG_CONSOLE		BOOLEAN_TRUE

#define APOLLO_INIT_FAIL_UPDATE_FAIL_CREATE_PROCESS	-100
#define APOLLO_INIT_FAIL_UPDATE_FAIL_WAIT_TIMEOUT	-101
#define APOLLO_INIT_FAIL_UPDATE_FAIL_UPDATER_RETURN	-102
#define APOLLO_INIT_FAIL_UPDATE_FAIL_WAIT_UNKNOWN	-103 

#ifndef DEBUG
//	#define APOLLO_ENABLE
#endif

//QGLPU.BIN Version
#ifdef REUBEN_PUBLISH
#define APOLLO_KEY_CODE "49783698-D13E-4809-B847-987372D1F595"
#else
//No Check Name
#define APOLLO_KEY_CODE "6D95972E-47EF-43B2-805C-6C2F3CC28525"
#endif

/*
//ApolloDemo_CLI.exe Version
#ifdef DEBUG
#define APOLLO_KEY_CODE "147A6B73-3CB5-403B-B4AC-669D10DB0495");
#endif
*/

Configure gConfigure;
Global gGlobal;
GlobalView * g_pGlobalView = &gGlobal;

void InitLogger()
{
	gGlobalLogger = SafeCreateObject1(Logger, GLOBAL_NAME);
	TRACE_CHECK(gGlobalLogger->Open(GLOBAL_CONFIG));	

	// set object manager logger
	Reuben::Simulation::SetObjectLogger(gGlobalLogger);
}

void CloseLogger()
{
	SafeDeleteObject(gGlobalLogger);
}

int CountWindowClass()
{
	HWND hWndT;
	int Count = 0;
	TCHAR szBuf[1024];
	    
	for (hWndT = ::GetFirstChild(::GetDesktopWindow()); ::IsWindow(hWndT); hWndT = ::GetNextSibling(hWndT)) 
	{
		if (::IsWindowVisible(hWndT)) 
		{
			::GetClassName(hWndT, szBuf, 1024);
			if(!wcscmp(_T("QGLWindow"), szBuf))
			{
				++Count;
			}
		}
	}

	return Count;
}


int __stdcall ApolloCTCallback(int MsgId, unsigned long Arg1, unsigned long Arg2)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ApolloCTCallback(%d %d %d) "), MsgId, Arg1, Arg2);
	return WorldService_Proxy::SendApolloMsg(MsgId, Arg1, Arg2);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpstrCmdLine, int nCmdShow)
{
	if (!Reuben::System::Initialize())
		return (-1);

	if (!gConfigure.Initialize(CONFIGURATION_INI_FILE, _T("Client")))
		return (-2);

	Int Count = CountWindowClass();
	if(Count >= 5)
		return (-3);

	/*
	{
		UInt Times = 0;

		while (true)
		{
			Int Count = CountWindowClass();
			if(Count < 5)
				break;

			if(++Times >= 30)
				return (-3);
			Sleep(500);
		}

	}
	*/
	
	//InstallFirstHook();

	
#ifdef APOLLO_ENABLE

	{
		UInt Times = 0;
		UInt apolloInitValue;
		// while loop exit on: 1) Succeeded; 2) Auto-update failed; 3) 30 trials failed
		while (true)
		{
			apolloInitValue = InitApolloCT(APOLLO_KEY_CODE);
			if (apolloInitValue == APOLLO_INIT_OK)
				break;

			++Times;

			bool bFailAtOnce = false;
			switch(apolloInitValue)
			{
			case APOLLO_INIT_FAIL_UPDATE_FAIL:
			case APOLLO_INIT_FAIL_UPDATE_FAIL_CREATE_PROCESS:
			case APOLLO_INIT_FAIL_UPDATE_FAIL_WAIT_TIMEOUT:
			case APOLLO_INIT_FAIL_UPDATE_FAIL_UPDATER_RETURN:
			case APOLLO_INIT_FAIL_UPDATE_FAIL_WAIT_UNKNOWN:
				bFailAtOnce = true;
				break;
			}

			if(bFailAtOnce || Times >= 5)
			{
				String msg;
				msg.Format(_T("Apollo Init Error.  Return Value = %d"), apolloInitValue);
				MessageBox(NULL, msg.c_str(), _T("Error"), MB_OK);
				return (-5);
			}
			Sleep(3000);
		}
	}
//	SetApolloCTCallBack(ApolloCTCallback);
#endif // APOLLO_ENABLE
	

	InitAllocCheck(ACOutput_XML);

	gGlobalName = gConfigure.strServiceName.c_str();
	gGlobalConfig = &gConfigure.config;
	InitLogger();
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status after init, %s"), MemoryManager::GetInstance().GetStatus().c_str());
	TRACE_ALWAYSDTL_1(GLOBAL_LOGGER, _F("Service %s Startup: Version ") VERSION_STRING _T(", Reuben ") REUBEN_VER_STRING, GLOBAL_NAME);

	QGLApplication::GetInstance().Run();

	if(gGlobal.g_NewGame)
	{
		DoRunNewGame();
#ifdef APOLLO_ENABLE
		Sleep(5000);
#endif
	}

#ifdef APOLLO_ENABLE
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Start UninitApolloCT."));		
	UninitApolloCT();
	TRACE_INFODTL(GLOBAL_LOGGER, _F("UninitApolloCT OK."));		
#endif

	

	TRACE_INFODTL(GLOBAL_LOGGER, _F("QGLApplication DestroyInstance..."));
	QGLApplication::GetInstance().DestroyInstance();
	TRACE_INFODTL(GLOBAL_LOGGER, _F("LocalRPCManager DestroyInstance..."));
	LocalRPCManager::GetInstance().DestroyInstance();

	

	//UnHookFirst();

	TRACE_ALWAYSDTL_1(GLOBAL_LOGGER, _F("Service %s Shutdown"), GLOBAL_NAME);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status after main loop, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	gConfigure.Uninitialize();
	gGlobal.Uninitialize();
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status after config uninit, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	REUEBN_SYSTEM_MEMORY_REGISTER_LEAK_REPORT(GLOBAL_LOGGER.GetLogFileName().c_str());

	CloseLogger();

	Reuben::System::Uninitialize();
	DeInitAllocCheck();

	
	return 0;	
}
