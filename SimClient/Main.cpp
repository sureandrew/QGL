//-- Common
#include "Common.h"
//-- Library
#include "LocalRPCManager.h"
#include "SimClient/Configure.h"
#include "SimClient/Global.h"
#include "SimClient/SimClient.h"
#include <tchar.h>
#include <process.h>
#include "Misc/Stackwalker.h"
#include "Version.h"

#include "ApolloClient.h"
#include "Proxy/Service/WorldService.h"

#define LOG_NAME		_T("SimClient")
#define DEFAULT_INI		_T(".\\Config\\Setting.ini")
#define UPDATE_INTERVAL		10
#define LOGOUT_WAITTIME		1000

#ifndef DEBUG
//	#define APOLLO_ENABLE
#endif

//SimClientU.exe "0C88B2B6-694B-47C0-8FCB-3AF0B01FB505"
//SimClientUD.exe "4C81A1BC-6362-49A2-9AEE-8DF4D163124A"

//#ifdef REUBEN_PUBLISH
//SimClientPU.exe Version
//#define APOLLO_KEY_CODE "4C81A1BC-6362-49A2-9AEE-8DF4D163124A"
//#else
//No Check Name
#define APOLLO_KEY_CODE "6D95972E-47EF-43B2-805C-6C2F3CC28525"
//#endif

Configure gConfigure;
Global gGlobal;

UInt32 g_manage_interval = C_UINT32(0);

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
			gGlobal.m_controlClient->WaitExecute(buf);
		}
	}
	catch (Exception ex)
	{

	}

	_endthread();
}

void GameLoop()
{
	// key input thread
	_beginthread(Input, 0, NULL);
	//Input(NULL);

	DWORD curTime = ::timeGetTime();
	DWORD newTime;

	while (!gGlobal.m_bExit)
	{
		newTime = ::timeGetTime();
		UInt32 delta = newTime - curTime;
		gGlobal.Update(delta);
		if (g_manage_interval > gConfigure.manage_interval)
		{
			DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("GameLoop : memory status, %s"), MemoryManager::GetInstance().GetStatus().c_str());
			g_manage_interval = 0;
		}
		g_manage_interval += delta;
		curTime = newTime;
		Sleep(UPDATE_INTERVAL);
	}
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
			{
				TRACE_INFOL(GLOBAL_LOGGER, _F(""));
				TRACE_INFODTL(GLOBAL_LOGGER, _F("SimClient Terminated"));
				gGlobal.m_bExit = true;
				_ungetwch(_T('\n'));
				fclose(stdin);
			}
			break;
 
        default: 
            return FALSE; 
    } 

	return TRUE;
}

int __stdcall ApolloCTCallback(int MsgId, unsigned long Arg1, unsigned long Arg2)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ApolloCTCallback(%d) "), MsgId, Arg1, Arg2);
	return WorldService_Proxy::SendApolloMsg(MsgId, Arg1, Arg2);
}

int _tmain(int argc, TCHAR* argv[])
{
	if (!Reuben::System::Initialize())
		return (-1);

	int argi = 1;
	TCHAR* listFile = NULL;
	TCHAR* startName = NULL;
	TCHAR* endName = NULL;
	TCHAR* proxyIp = NULL;
	TCHAR* proxyPort = NULL;

	while (argi < argc && *argv[argi] == _T('-')) {
		if (_tcsicmp(argv[argi], _T("-l")) == 0 && argi + 2 < argc) {
			// multiple account/password list
			listFile = argv[argi+1];
			argi ++;
		} else if (_tcsicmp(argv[argi], _T("-a")) == 0 && argi + 3 < argc) {
			// multiple start and end index
			startName = argv[argi+1];
			endName = argv[argi+2];
			argi += 2;
		} else if (_tcsicmp(argv[argi], _T("-p")) == 0 && argi + 3 < argc) {
			// custom proxy IP and port
			proxyIp = argv[argi+1];
			proxyPort = argv[argi+2];
			argi += 2;
		} else if (_tcsicmp(argv[argi], _T("-i")) == 0 && argi + 2 < argc) {
			// multiple interval time
			UInt32 t = _ttoi(argv[argi+1]);
			if (t > 0)
				gGlobal.m_multInterval = t;
			argi ++;
		} else if (_tcsicmp(argv[argi], _T("-s")) == 0 && argi + 3 < argc) {
			// multiple interval time
			UInt8 n = (UInt8) _ttoi(argv[argi+1]);
			UInt32 total = (UInt32) _ttoi(argv[argi+2]);
			if (n > 0 && total > 0)
			{
				gGlobal.m_shadowNum = n;
				gGlobal.m_clientTotal = total;
			}
			argi += 2;
		} else {
			break;
		}
		argi ++;
	}

	TCHAR *configFile = DEFAULT_INI;
	if (argi < argc) {
		configFile = argv[argi];
		argi ++;
	}

	// handler function 
	SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE);

#ifdef APOLLO_ENABLE
	UInt apolloInitValue = InitApolloCT(APOLLO_KEY_CODE);
	if (apolloInitValue != APOLLO_INIT_OK)
	{
		String msg;
		msg.Format(_T("Apollo Init Error.  Return Value = %d"), apolloInitValue);
		MessageBox(NULL, msg.c_str(), _T("Error"), MB_OK);
		return (-5);
	}
#endif // DEBUG

	InitAllocCheck(ACOutput_XML);

	if (gConfigure.Initialize(configFile, LOG_NAME))
	{
		if (proxyIp && proxyPort)
		{
			gConfigure.ipProxy[0] = Reuben::Network::StringToIP(proxyIp);
			gConfigure.portProxy[0] = Reuben::Network::StringToPort(proxyPort);
			gConfigure.uProxyCount = 1;
		}

		gGlobalName = gConfigure.strServiceName.c_str();
		gGlobalConfig = &gConfigure.config;

		gGlobal.Initialize();
		
		TRACE_ALWAYSDTL_1(GLOBAL_LOGGER, _F("Service %s Startup: Version ") VERSION_STRING _T(", Reuben ") REUBEN_VER_STRING, GLOBAL_NAME);

		String title;
		if (listFile)
		{
			title.Format(_T("SimClient %s"), listFile);
			gGlobal.MultiClient(listFile);
		}
		else if (startName && endName)
		{
			title.Format(_T("SimClient %s - %s"), startName, endName);
			gGlobal.MultiClient(startName, endName);
		}
		else
		{
			if (argi + 1 < argc)
			{
				// login name and password provided
				title.Format(_T("SimClient %s"), argv[argi]);
				gGlobal.SingleClient(argv[argi], argv[argi + 1]);
				argi += 2;
			}
			else
			{
				title.Format(_T("SimClient"));
				gGlobal.SingleClient(NULL, NULL);
			}
		}
		if (argi < argc)
		{	// start lua script and robots
			gGlobal.LoadRobotLua(argv[argi]);

			String args;
			for (int i = 0; i < argc; i++)
			{
				args.AppendFormat(_T("%s "), argv[i]);
			}
			gGlobal.RegisterManager(argv[0], title, args);
		}

		Sleep(100);
		DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status after init, %s"), MemoryManager::GetInstance().GetStatus().c_str());
		GameLoop();

		// wait 1 second for logout
		//WaitLogout();

		gGlobal.Uninitialize();
		LocalRPCManager::GetInstance().DestroyInstance();
	}

	TRACE_ALWAYSDTL_1(GLOBAL_LOGGER, _F("SimClient %s Shutdown"), GLOBAL_NAME);
	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status after main loop, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	gConfigure.Uninitialize();
	DEBUG_INFODTL_1(GLOBAL_LOGGER, _F("_tmain : memory status after config uninit, %s"), MemoryManager::GetInstance().GetStatus().c_str());

	REUEBN_SYSTEM_MEMORY_REGISTER_LEAK_REPORT(GLOBAL_LOGGER.GetLogFileName().c_str());
	SafeDeleteObject(gGlobal.m_defaultLogger);
	Reuben::System::Uninitialize();
	DeInitAllocCheck();

	UninitApolloCT();
}
