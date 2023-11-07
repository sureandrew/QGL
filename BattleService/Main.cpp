//-- Common
#include "Common.h"
//-- Library
#include "BattleService/BattleApplication.h"
#include "BattleService/Global.h"
#include "BattleService/Configure.h"
#include "Misc/Stackwalker.h"

Configure gConfigure;
Global gGlobal;

void InitLogger()
{
	gGlobalLogger = SafeCreateObject1(Logger, gConfigure.strServiceName.c_str());
	TRACE_CHECK(gGlobalLogger->Open(gConfigure.m_config));

	// set object manager logger
	Reuben::Simulation::SetObjectLogger(gGlobalLogger);
}

void CloseLogger()
{
	SafeDeleteObject(gGlobalLogger);
}

BOOL CtrlHandler(DWORD ctrlType) 
{ 
    switch (ctrlType) 
    { 
        // Handle the CTRL+C signal. 
        case CTRL_C_EVENT: 

        // CTRL+CLOSE: confirm that the user wants to exit. 
        case CTRL_CLOSE_EVENT: 
 
        // Pass other signals to the next handler. 
        case CTRL_BREAK_EVENT: 
        case CTRL_LOGOFF_EVENT: 
        case CTRL_SHUTDOWN_EVENT: 
			TRACE_INFODTL(GLOBAL_LOGGER, _T("Program terminate ..."));
			gGlobal.m_bExit = BOOLEAN_TRUE;
			break;
 
        default: 
            return FALSE; 
    } 

	return TRUE;
} 

int _tmain(int argc, TCHAR* argv[])
{
	if (argc < 3)
		return (-1);

	if (!Reuben::System::Initialize())
		return (-2);

	REUBEN_SYSTEM_MEMORY_END_LEAK_CHECK();
	InitAllocCheck(ACOutput_XML);

	// handler function 
	SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE);

	if (gConfigure.Initialize(argv[1], argv[2])) {
		InitLogger();
		LocalRPCManager::GetInstance().SetServiceId(gConfigure.serviceId);
		BattleApplication::GetInstance().Run();
		BattleApplication::DestroyInstance();
	}

	gConfigure.Uninitialize();
	Reuben::System::Uninitialize();
	REUEBN_SYSTEM_MEMORY_REGISTER_LEAK_REPORT(GLOBAL_LOGGER.GetLogFileName().c_str());
	CloseLogger();

	DeInitAllocCheck();
	REUBEN_SYSTEM_MEMORY_END_LEAK_CHECK();

	return 0;
}