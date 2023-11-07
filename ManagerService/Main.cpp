#include "Common.h"
#include "LocalRPCManager.h"
#include "ManagerApplication.h"
#include "Configure.h"
#include "Global.h"
#include "Misc/Stackwalker.h"
#include "Version.h"
#include "DBPWDecrypt.h"

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

			if (IsOption(OPT_AUTO_STOP)) {
				gGlobal.CheckAutoStop();
			} else {
				_ungetwch(_T('\n'));
				fclose(stdin);
				gGlobal.m_bExit = BOOLEAN_TRUE;
			}
			break;
 
        default: 
            return FALSE; 
    } 

	return TRUE;
} 

void Input(void *)
{
	Char buf[COMMAND_LENGTH] = _T("      ");
	char buf2[COMMAND_LENGTH] = "        ";
	char* rtn;
	DWORD dwRead;
	HANDLE hStdin;

	// handler function 
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	try
	{
		while (!gGlobal.m_bExit)
		{
			TRACE_INFO(GLOBAL_LOGGER, _F("\nInput: "));
			//_getts(buf);
			ReadFile(hStdin, buf2, COMMAND_LENGTH, &dwRead, NULL);
			buf2[dwRead+1] = '\0';
			if (rtn = strchr(buf2, '\r'))		// trim to 'return' character
				*rtn = '\0';
			if (strlen(buf2) < 2)				// empty input
				continue;
			MultiByteToWideChar(CP_ACP, 0, buf2, (int) strlen(buf2) + 1, buf, COMMAND_LENGTH);
			gGlobal.AddCommand(buf);
		}
	}
	catch (Exception ex)
	{

	}

	_endthread();
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

	if (gConfigure.Initialize(argv[1], argv[2]))
	{
		if (argc > 3)
		{
			int argi = 3;
			while (argi < argc)
			{
				gGlobal.ReadOption(argv[argi]);
				argi++;
			}
		}
		gGlobalName = gConfigure.strServiceName.c_str();
		gGlobalConfig = &gConfigure.config;
		InitLogger();
		DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status after init, %s"), MemoryManager::GetInstance().GetStatus().c_str());
		TRACE_ALWAYSDTL_1(GLOBAL_LOGGER, _F("Service %s Startup: Version ") VERSION_STRING _T(", Reuben ") REUBEN_VER_STRING, GLOBAL_NAME);
		LocalRPCManager::GetInstance().SetServiceId(gConfigure.serviceId);
		_beginthread(Input, 0, NULL);				// start input thread
		ManagerApplication::GetInstance().Run();
		ManagerApplication::DestroyInstance();
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