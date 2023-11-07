//-- Common
#include "Common.h"
//-- Library
#include "LocalRPCManager.h"
#include "LogApplication.h"
#include <tchar.h>
#include "Configure.h"
#include "Global.h"
#include "Misc/Stackwalker.h"
#include "Version.h"

Configure gConfigure;
Global gGlobal;

void InitLogger()
{
	gGlobalLogger = SafeCreateObject1(Logger, GLOBAL_NAME);
	TRACE_CHECK(gGlobalLogger->Open(gConfigure.logFile.c_str(), gConfigure.logConsole, 
		BOOLEAN_TRUE, gConfigure.logAppend, (Logger::LevelEnum) gConfigure.logLevel,
		gConfigure.logSize, gConfigure.logArchive.c_str()));

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
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Program terminate ..."));
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

	InitAllocCheck(ACOutput_XML);

	// handler function 
	SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE);

	if (argc > 4 && gConfigure.Initialize(argv[1], argv[2], argv[3], argv[4]) || 
		argc <= 4 && gConfigure.Initialize(argv[1], argv[2]))
	{
		gGlobalName = gConfigure.strServiceName.c_str();
		gGlobalConfig = &gConfigure.config;
		InitLogger();
		DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status after init, %s"), MemoryManager::GetInstance().GetStatus().c_str());
		TRACE_ALWAYSDTL_1(GLOBAL_LOGGER, _F("Service %s Startup: Version ") VERSION_STRING _T(", Reuben ") REUBEN_VER_STRING, GLOBAL_NAME);
		LocalRPCManager::GetInstance().SetServiceId(gConfigure.serviceId);
		LogApplication::GetInstance().Run();
		LogApplication::DestroyInstance();
		LocalRPCManager::GetInstance().DestroyInstance();
	}

	TRACE_ALWAYSDTL_1(GLOBAL_LOGGER, _F("Service %s Shutdown"), GLOBAL_NAME);
	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status after main loop, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	gConfigure.Uninitialize();
	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status after config uninit, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	gGlobalLogger->ForceArchive();	// force to archive log file
	REUEBN_SYSTEM_MEMORY_REGISTER_LEAK_REPORT(GLOBAL_LOGGER.GetLogFileName().c_str());
	CloseLogger();
	Reuben::System::Uninitialize();
	DeInitAllocCheck();

	return (0);
}
