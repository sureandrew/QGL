//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/PathFinder.h"
#include "Common/CallLuaFunc.h"
#include "Common/Procedure.h"
#include "Common/ExportVersion.h"
//-- Self
#include "SimClient/Global.h"
//-- Library
#include "SimClient/SimClient.h"
#include "SimClient/Configure.h"
#include "Proxy/Service/ManagerService.h"
#include "Resource/ResNpc.h"
#include "Resource/ClientResManager.h"

#pragma warning(disable : 4099 4806)			// disable linking warning

extern "C"
{
	#include <lualib.h>
	#include <lauxlib.h>
	int luaopen_LuaI(lua_State* L);		// declare the wrapped module
}

#define FILENAME_LENGTH				1024
#define LUA_INIT_FILE				"./base_init.lua"
#define CHATMSG_FILE				_T("./ChatMsg.txt")
#define LOG_FILE_FORMAT				_T(".\\Log\\SimClient_%s.log")
#define MULTI_INTERVAL				100
//#define RESTART_TIME				1800000		// 0.5 hour shutdown
#define RESTART_TIME				0			// disable auto-restart
#define ROBOT_INTERVAL				5000		// default 5s robot interval
#define MINUTE_TIME					60000
#define SECOND_TIME					1000
#define LUA_GC_STEP					1000

Global::Global()
{
	m_pSimClient = NULL;
	m_controlClient = NULL;
	m_pLuaState = NULL;
	m_bMulti = BOOLEAN_FALSE;
	m_option = OPT_SHOW_SELF | OPT_SHOW_ITEM;
	m_multInterval = MULTI_INTERVAL;
	m_pManagerPeer = NULL;
	m_robotInterval = ROBOT_INTERVAL;
	m_downTime = 0;
	m_nextLine = 0;
	m_shadowNum = 0;
	m_clientTotal = 0;
}

Global::~Global()
{
}

Boolean Global::Initialize(VOID)
{
	m_rand.Randomize();

	// create default logger
	m_defaultLogger = CreateLogger(_T(""));
	m_defaultLogger->SetConsole(BOOLEAN_TRUE);
	SetLogger(m_defaultLogger);

	m_resource = SafeCreateObject(CClientResManager);
	m_resource->LoadMobCSV();
	m_resource->LoadItemCSV();
	m_resource->LoadSkillCSV();
//	m_resource->LoadNpcDAT();
//	m_resource->LoadClientMapDAT();
	LoadChatMessage();

	// read options
	ReadOption(gConfigure.option.c_str());

	// init lua library
	m_pLuaState = luaL_newstate();
	TRACE_ENSURE(m_pLuaState);
	luaL_openlibs(m_pLuaState);
	luaopen_LuaI(m_pLuaState);
	RunLua(m_pLuaState, LUA_INIT_FILE);

	LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();
	Reuben::Simulation::ScheduleManager &schmgr = Reuben::Simulation::ScheduleInstance::GetInstance();
	rpcmgr.GetModel().SetSocketHandler(this);
	rpcmgr.SetServiceId(0);
	ProcessManager::GetInstance().Attach(&rpcmgr);
	ProcessManager::GetInstance().Attach(&schmgr);
	ProcessManager::GetInstance().Attach(&ProcedureManager::GetInstance());
	m_startRunTime = ::timeGetTime();

	return (BOOLEAN_TRUE);
}

VOID Global::Uninitialize(VOID)
{
	// unregister manager if connected manager and exit normally
	if (m_bExit && m_pManagerPeer) {
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UnregisterSimClient: %s"), m_title.c_str());
		ManagerService_Proxy::UnregisterSimClient(m_pManagerPeer, RPCContext());
		Sleep(200);
		SafeRelease(m_pManagerPeer);
	}

	LocalRPCManager::GetInstance().GetModel().Shutdown();		// close socket
	ProcessManager::GetInstance().Detach(&LocalRPCManager::GetInstance());
	ProcessManager::GetInstance().Detach(&ProcedureManager::GetInstance());
	ProcessManager::GetInstance().Detach(&Reuben::Simulation::ScheduleInstance::GetInstance());

	SleepEx(200, BOOLEAN_TRUE);

	if (!m_bMulti) {
		// single client
		ProcessManager::GetInstance().Detach(m_pSimClient);
		SafeDeleteObject(m_pSimClient);
	} else {
		// multiple clients
		ThreadGuard guard(m_key);
		CSimClient* pSimClient;
		for (ClientNameMap::iterator it = m_clientNames.begin(); 
			it != m_clientNames.end(); ++it)
		{
			pSimClient = it->second;
			ProcessManager::GetInstance().Detach(pSimClient);
			SafeDeleteObject(pSimClient);
		}
		m_clientNames.clear();
	}
	m_clientSockets.clear();
	m_pSimClient = NULL;
	m_controlClient = NULL;
	SetLogger(m_defaultLogger);

	LocalRPCManager::GetInstance().SetNetIdMap(NULL);
	Reuben::Simulation::ObjectInstance::GetInstance().DeleteAll();

	lua_close(m_pLuaState);				// close lua library
	m_msgList.clear();

	for (PathFinderMap::iterator it = m_pathFinders.begin(); it != m_pathFinders.end(); ++it)
	{
		SafeDeleteObject(it->second);
	}
	m_pathFinders.clear();

	for (MapDbList::iterator it = m_mapDbList.begin(); it != m_mapDbList.end(); ++it)
	{
		SafeDeleteObject(it->second);
	}
	m_mapDbList.clear();

	SafeDeleteObject(m_resource);
	//SafeDeleteObject(m_defaultLogger);

	ProcessManager::DestroyInstance();
	ProcedureManager::DestroyInstance();
	LocalRPCManager::DestroyInstance();
	SessionManager::DestroyInstance();

	m_prog.Empty();
	m_title.Empty();
	m_cmdLine.Empty();
	m_workDir.Empty();
}

Boolean Global::RunLua(lua_State* luaState, const char* filename)
{
	if (luaState == NULL || filename == NULL)
		return (BOOLEAN_FALSE);

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Run Lua file: %s"), String(filename).c_str());
	int r = luaL_dofile(luaState, filename);

	if (r != 0)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Error running Lua file: %s"), String(filename).c_str());
		TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(luaState, -1)).c_str());
		return (BOOLEAN_FALSE);
	}

	//CallLuaFunc(gGlobal.m_pLuaState, "Hello");

	return (BOOLEAN_TRUE);
}

void Global::NewLuaThread(lua_State* &lstate, lua_State* parent, int &refKey)
{
	if (parent == NULL)
		return;

	//printf("NewLuaThread: garbage collection memory %d\n", lua_gc(parent, LUA_GCCOUNT, 0));
	//lua_gc(parent, LUA_GCCOLLECT, 0);
	int count = lua_gc(parent, LUA_GCCOUNTB, 0);

	lstate = lua_newthread(parent);
	if (lstate == NULL) {
		TRACE_ERRORDTL_1((*m_defaultLogger), _T("NewLuaThread Error: parent lua %d"), parent);
		TRACE_ERRORDTL((*m_defaultLogger), String(lua_tostring(parent, -1)).c_str());
		return;
	}

	lua_pushvalue(parent, -1);				// copy thread for setfenv
	refKey = luaL_ref(parent, LUA_REGISTRYINDEX);	// reference key for gc

	// copy parent state global table to separate global table for each thread:
    // local newgt = {__index = _G}			-- create new environment and act as metatable
    // setmetatable(newgt, newgt)
    // setfenv(thread, newgt)				-- set environment
	lua_newtable(parent);						// new newgt table
	lua_pushvalue(parent, -1);
	lua_pushvalue(parent, LUA_GLOBALSINDEX);
	lua_setfield(parent, -2, "__index");
	lua_setmetatable(parent, -2);				// set itself as metatable
	lua_setfenv(parent, -2);
	lua_pop(parent, 1);							// pop thread object

	//TRACE_VERBOSEDTL_2((*m_defaultLogger), _T("NewLuaThread: refKey %d, at lua memory %d"),
	//	refKey, count);
}

void Global::ReleaseLuaThread(lua_State* &lstate, lua_State* parent, int refKey)
{
	static int gc_step = 0;

	if (lstate == NULL || parent == NULL)
		return;

	int count = lua_gc(parent, LUA_GCCOUNTB, 0);
	luaL_unref(parent, LUA_REGISTRYINDEX, refKey);
	gc_step++;
	if (gc_step >= LUA_GC_STEP) {
		gc_step = 0;
		lua_gc(parent, LUA_GCCOLLECT, 0);
	}
	//TRACE_VERBOSEDTL_2((*m_defaultLogger), _T("ReleaseLuaThread: refKey %d, at lua memory %d"),
	//	refKey, count);

	lstate = NULL;
}

void Global::ReadOption(const TCHAR *optLine)
{
	if (optLine == NULL) return;

	TCHAR optstr[1024] = _T("");
	TCHAR *delim = _T("|, \t");
	TCHAR *token;
	Boolean enable;

	_tcscpy(optstr, optLine);
	token = _tcstok(optstr, delim);
	while (token)
	{
		if (token[0] == _T('-')) {
			++token;
			enable = false;
		} else if (token[0] == _T('+')) {
			++token;
			enable = true;
		} else {
			enable = true;
		}
		if (_tcsicmp(token, _T("show_self")) == 0)
			SetOption(OPT_SHOW_SELF, enable);
		else if (_tcsicmp(token, _T("show_other")) == 0)
			SetOption(OPT_SHOW_OTHER, enable);
		else if (_tcsicmp(token, _T("show_item")) == 0)
			SetOption(OPT_SHOW_ITEM, enable);
		else if (_tcsicmp(token, _T("show_move")) == 0)
			SetOption(OPT_SHOW_MOVE, enable);
		else if (_tcsicmp(token, _T("auto_select")) == 0)
			SetOption(OPT_AUTO_SELECT, enable);
		else if (_tcsicmp(token, _T("auto_ping")) == 0 || _tcsicmp(token, _T("ap")) == 0)
			SetOption(OPT_AUTO_PING, enable);
		token = _tcstok(NULL, delim);
	}
}

void Global::ShowOption()
{
	// show options
	TRACE_INFODT(GLOBAL_LOGGER, _F("Show Options: "));
	if (IsOption(OPT_SHOW_SELF))
		TRACE_INFO(GLOBAL_LOGGER, _F("show_self "));
	if (IsOption(OPT_SHOW_OTHER))
		TRACE_INFO(GLOBAL_LOGGER, _F("show_other "));
	if (IsOption(OPT_SHOW_ITEM))
		TRACE_INFO(GLOBAL_LOGGER, _F("show_item "));
	if (IsOption(OPT_SHOW_MOVE))
		TRACE_INFO(GLOBAL_LOGGER, _F("show_move "));
	if (IsOption(OPT_AUTO_SELECT))
		TRACE_INFO(GLOBAL_LOGGER, _F("auto_select "));
	if (IsOption(OPT_AUTO_PING))
		TRACE_INFO(GLOBAL_LOGGER, _F("auto_ping "));
	TRACE_INFOL(GLOBAL_LOGGER, _F(""));
}

// load chat message
void Global::LoadChatMessage()
{
	FILE *fp;
	TCHAR line[1024];

	fp = _tfopen(CHATMSG_FILE, _T("rb"));
	if (fp == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("LoadChatMessage: cannot open chat message file %d"),
			CHATMSG_FILE);
		m_msgList.push_back(_T("Error: no chat message"));
		return;
	}

	m_msgList.clear();
	while (!feof(fp))
	{
		_fgetts(line, 1024, fp);
		String s(line);
		s.Trim();
		if (s != _T(""))
			m_msgList.push_back(s);
	}
	fclose(fp);
}

void Global::Update(CONST UInt32 dt)
{
	ProcessManager::GetInstance().UpdateProcesses(dt);

	// auto restart in Manager Service after a period
	if (m_bMulti && m_pManagerPeer && RESTART_TIME != 0 && 
		::timeGetTime() - m_startRunTime >= RESTART_TIME)
		RestartClient(5000);			// restart after 5 second

	if (m_downTime > 0)
		CheckDownTime(dt);
}

MapDB_Client* Global::GetMapDb(UInt16 mapid)
{
	// check if map db already loaded
	MapDbList::iterator it = m_mapDbList.find(mapid);
	if (it != m_mapDbList.end()) {
		return it->second;
	}

	String dataFile;
	FILE *f0;

	dataFile.Format(_T("%sclient_MAP%08i_zonedata.dat"), gConfigure.zoneDataPath, mapid);

	m_mapDbList[mapid] = SafeCreateObject(MapDB_Client);
	MapDB_Client* &mapdb = m_mapDbList[mapid];

    if ((f0 = _tfopen(dataFile.c_str(), _T("rb"))) != NULL)
	{
		mapdb->m_zonedata_header = SafeCreateObject(BinaryBlockInfoHeader);
		fread(mapdb->m_zonedata_header, 1, sizeof(BinaryBlockInfoHeader), f0);

		TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, mapdb->m_zonedata_header->version!=EXPORT_DATA_CLIENT_MAP_ZONEDATA_FORMAT_VERSION, _T("GetMapDb from \"%s\", file version:%i, app. version:%i"), dataFile.c_str(), mapdb->m_zonedata_header->version, EXPORT_DATA_CLIENT_MAP_ZONEDATA_FORMAT_VERSION);

		if (mapdb->m_combinedblock)
			SafeDeallocate(mapdb->m_combinedblock);
		mapdb->m_combinedblock = SafeAllocate(WORD, mapdb->m_zonedata_header->width * mapdb->m_zonedata_header->height);

		UInt32 zoneSize;
		fread(&zoneSize, 1, sizeof(UInt32), f0);

		ClientMapZoneData* blockData = NULL;
		for(UInt i = 0; i < mapdb->m_zonedata_header->blocksizeinbyte; ++i)
		{
			blockData = SafeCreateObject(ClientMapZoneData);
			fread(&blockData->sBlockType, 1, sizeof(WORD), f0);
			mapdb->m_zonedata.push_back(blockData);
			mapdb->m_combinedblock[i] = blockData->sBlockType;
		}
		fclose(f0);

		return mapdb;
	}
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CMap ImportClient_ZoneData Error: cannot open zone data file %d"),
			dataFile.c_str());
		return NULL;
	}
}

PathFinder* Global::GetPathFinder(MapDB_Client* mapdb, UInt16 mapid)
{
	if (mapdb == NULL)
		return NULL;

	// check if path finder already loaded
	PathFinderMap::iterator it = m_pathFinders.find(mapid);
	if (it != m_pathFinders.end()) {
		return it->second;
	}

	String dataFile;
	PathFinder* &pf = m_pathFinders[mapid];
	pf = SafeCreateObject(PathFinder);
	pf->SetMap((UInt16*) mapdb->m_combinedblock, 
		mapdb->m_zonedata_header->width, mapdb->m_zonedata_header->height);

	dataFile.Format(_T("%sclient_MAP%08i_pathfind.dat"), gConfigure.zoneDataPath, mapid);

	FILE *f0;
	Real32* weightBuf	= NULL;
	UInt16* linkBuf		= NULL;
	Real32* distBuf		= NULL;
	DefaultPFHeader header;
	if ((f0 = _tfopen(dataFile.c_str(), _T("rb"))) != NULL)
	{
		fread(&header, 1, sizeof(DefaultPFHeader), f0);

		TRACE_ERRORDT_IF_3(GLOBAL_LOGGER, header.version!=EXPORT_DATA_MAP_PATHFIND_FORMAT_VERSION, _T("GetPathFinder from \"%s\", file version:%i, app. version:%i"), dataFile.c_str(), header.version, EXPORT_DATA_MAP_PATHFIND_FORMAT_VERSION);

		if( header.sizeWeightGraph > 0 )
		{
			weightBuf = SafeAllocate(Real32, header.sizeWeightGraph);
			fread(weightBuf, sizeof(Real32), header.sizeWeightGraph, f0);
		}
		if( header.sizeDistGraph > 0 )
		{
			distBuf = SafeAllocate(Real32, header.sizeDistGraph);
			fread(distBuf, sizeof(Real32), header.sizeDistGraph, f0);
		}
		if( header.sizeLinkGraph > 0 )
		{
			linkBuf = SafeAllocate(UInt16, header.sizeLinkGraph);
			fread(linkBuf, sizeof(UInt16), header.sizeLinkGraph, f0);
		}
		fclose(f0);
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CMap ImportClient_PFData Warning: cannot open pathfind data file %d"),
			dataFile.c_str());
	}

	// pre-calculate if neccessary
	if (header.numGuildPoint == 0) {
		pf->PreCalculate();
	} else {
		pf->PreCalculate(weightBuf, distBuf, linkBuf);
		SafeDeallocate(weightBuf);
		SafeDeallocate(distBuf);
		SafeDeallocate(linkBuf);
	}

	return pf;
}

void Global::SetLogger(Logger* pLogger)
{
	if (pLogger == NULL)
		return;

	gGlobalLogger = pLogger;
	Reuben::Simulation::SetObjectLogger(pLogger);
}

Logger* Global::CreateLogger(const Char *accountName)
{
	Logger* pLog = SafeCreateObject2(Logger, gConfigure.strServiceName.c_str(), accountName);
	TRACE_ENSURE(pLog);

	pLog->Open(gConfigure.logFile.c_str(), gConfigure.logConsole, BOOLEAN_TRUE, 
		gConfigure.logAppend, (Reuben::Diagnostics::Logger::LevelEnum) gConfigure.logLevel,
		gConfigure.logSize);

	return pLog;
}

void Global::SingleClient(const Char *accountName, const Char *password)
{
	m_pSimClient = SafeCreateObject(CSimClient);
	m_controlClient = m_pSimClient;
	ProcessManager::GetInstance().Attach(m_pSimClient);	// attach to process manager
	if (accountName && password) {
		_tcscpy(m_pSimClient->m_loginName, accountName);
		m_pSimClient->m_accountName = accountName;
		m_pSimClient->m_pLogger = CreateLogger(accountName);
		m_pSimClient->m_pLogger->SetConsole(BOOLEAN_TRUE);
		SetLogger(m_pSimClient->m_pLogger);
	} else {
		m_pSimClient->m_pLogger = m_defaultLogger;
	}

	if (m_pSimClient->ConnectServer() && accountName && password) {
		m_pSimClient->WaitExecute(_T("login"));		// login when connect to server
	}
}

void Global::MultiClient(const Char *listFile)
{
	FILE *fp;

	fp = _tfopen(listFile, _T("r"));
	if (fp == NULL) {
		SingleClient(NULL, NULL);
		return;
	}
		
	Char buf[256];
	Char *delim = _T(", \t\r\n");
	Char *token;
	Char *accountName, *password;
	CSimClient *pSimClient;
	UInt16 clientNum = 0;
	UInt32 sleepTime = m_multInterval;
	bool first = true;

	// read each line and trim for white space
	// skip empty line and comment with '#' begin
	while (!gGlobal.m_bExit && _fgetts(buf, 256, fp)) {
		token = _tcstok(buf, delim);
		if (token == NULL || token[0] == '#')
			continue;

		accountName = token;
		password = _tcstok(NULL, delim);
		if (accountName == NULL || password == NULL)
			continue;

		pSimClient = SafeCreateObject(CSimClient);
		_tcscpy(pSimClient->m_loginName, accountName);
		pSimClient->m_accountName = accountName;
		pSimClient->m_pLogger = CreateLogger(accountName);
		if (m_pSimClient == NULL) {
			m_controlClient = pSimClient;
		} else {
			// no console for other SimClient
			pSimClient->m_pLogger->SetConsole(BOOLEAN_FALSE);
		}
		m_pSimClient = pSimClient;
		m_clientNames[pSimClient->m_loginName] = pSimClient;
		pSimClient->m_actionTime = sleepTime;
		SetLogger(pSimClient->m_pLogger);
		if (first) {							// connect proxy for first one
			first = false;
			m_pSimClient->ConnectServer();
		}
		ProcessManager::GetInstance().Attach(pSimClient);	// attach to process manager
		sleepTime += m_multInterval + clientNum;
		clientNum++;
	}

	// if more than 1 SimClient
	if (m_clientNames.size() > 1)
	{
		m_bMulti = BOOLEAN_TRUE;
		LocalRPCManager::GetInstance().SetRecvHandle(RecvHandle);
	}

	fclose(fp);
}

void Global::MultiClient(const Char *startName, const Char *endName)
{
	bool finished = false;
	Char accountName[256];
	int len = (int) _tcslen(startName);
	CSimClient *pSimClient;
	UInt32 sleepTime = m_multInterval;
	bool first = true;

	// check startName with endName, must be same length
	if (len < 1 || len != _tcslen(endName) || _tcscmp(startName, endName) > 0) {
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("MultiClient Error: invalid startName %s or endName %s"),
			startName, endName);
		return;
	}

	_setmaxstdio(1100);			// extend maximum number of open files

	// check only allow alphabet and digits
	for (int i = 0; i < len; ++i) {
		if (!((startName[i] >= '0' && startName[i] <= '9') ||
			  (startName[i] >= 'A' && startName[i] <= 'z')))
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("MultiClient Error: invalid startName %s"),
				startName);
			return;
		}
		if (!((endName[i] >= '0' && endName[i] <= '9') ||
			  (endName[i] >= 'A' && endName[i] <= 'z')))
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("MultiClient Error: invalid endName %s"),
				endName);
			return;
		}
	}

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("MultiClient: startName %s to endName %s"),
		startName, endName);

	_tcscpy(accountName, startName);

	while (!gGlobal.m_bExit && !finished) {
		if (_tcscmp(accountName, endName) == 0)		// reach end name
			finished = true;

// 		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("MultiClient: accountName %s, password %s"),
// 			accountName, accountName);

		pSimClient = SafeCreateObject(CSimClient);
		_tcscpy(pSimClient->m_loginName, accountName);
		pSimClient->m_accountName = accountName;
		pSimClient->m_pLogger = CreateLogger(accountName);
		if (m_pSimClient == NULL) {
			m_controlClient = pSimClient;
		} else {
			// no console for other SimClient
			pSimClient->m_pLogger->SetConsole(BOOLEAN_FALSE);
		}
		m_pSimClient = pSimClient;
		m_clientNames[pSimClient->m_loginName] = pSimClient;
		SetLogger(pSimClient->m_pLogger);
		if (first) {							// connect proxy for first one
			first = false;
			m_pSimClient->ConnectServer();
		}
		ProcessManager::GetInstance().Attach(pSimClient);	// attach to process manager
		pSimClient->m_actionTime = sleepTime;

// 		if (pSimClient->ConnectServer()) {
// 			ProcessManager::GetInstance().Attach(pSimClient);	// attach to process manager
// 			pSimClient->m_actionTime = sleepTime;
// 		} else {
// 			// remove client if cannot connect
// 			TRACE_ERRORDTL_1((*m_defaultLogger), 
// 				_T("Connection Error: SimClient %s connect to Proxy Service failed"),
// 				accountName);
// 			SafeDeleteObject(pSimClient);
// 		}
		sleepTime += m_multInterval;

		// increase account name
		for (int i = len - 1; i >= 0; --i) {
			if (accountName[i] == '9') {			// digital overflow
				accountName[i] = '0';
			} else if (accountName[i] == 'z') {		// alphabet overflow
				accountName[i] = 'a';
			} else if (accountName[i] == 'Z') {		// alphabet overflow
				accountName[i] = 'A';
			} else {
				accountName[i] ++;
				break;
			}
		}
	}

	// if more than 1 SimClient
	if (m_clientNames.size() > 1)
	{
		m_bMulti = BOOLEAN_TRUE;
		LocalRPCManager::GetInstance().SetRecvHandle(RecvHandle);
	}
}

void Global::AddClient(CSimClient* pSimClient)
{
	ThreadGuard guard(gGlobal.m_key);

	m_clientSockets[pSimClient->m_pProxyPeer->GetSocketID()] = pSimClient;
	m_clientNames[pSimClient->m_loginName] = pSimClient;
}

void Global::SwitchClient(const Char *accountName)
{
	ClientNameMap::iterator it = m_clientNames.find(String(accountName));
	if (it != m_clientNames.end()) {
		CSimClient *pSimClient = it->second;
		m_controlClient->m_pLogger->SetConsole(BOOLEAN_FALSE);
		pSimClient->m_pLogger->SetConsole(BOOLEAN_TRUE);
		SetLogger(pSimClient->m_pLogger);
		TRACE_INFODTL_2((*pSimClient->m_pLogger), _T("SimClient %s: Switched to SimClient %s"), 
			m_controlClient->m_loginName, accountName);
		m_pSimClient = pSimClient;
		m_controlClient = pSimClient;
	}
}

void Global::LoadRobotLua(const Char *scriptFile)
{
	StringA filename = StringA(scriptFile);
	lua_State *luaState = NULL;

	if (m_pSimClient) {
		NewLuaThread(m_pSimClient->m_pLuaState, m_pLuaState, m_pSimClient->m_luaRef);
		if (m_pSimClient->m_pLuaState == NULL)
			return;
		if (!RunLua(m_pSimClient->m_pLuaState, filename.c_str())) {
			ReleaseLuaThread(m_pSimClient->m_pLuaState, m_pLuaState, 
				m_pSimClient->m_luaRef);
			return;
		}
		m_pSimClient->m_bRobot = true;
		luaState = m_pSimClient->m_pLuaState;
	}

	CSimClient* pClient;
	for (ClientNameMap::iterator it = m_clientNames.begin(); 
		it != m_clientNames.end(); ++it)
	{
		pClient = it->second;
		TRACE_ENSURE(pClient);
		if (pClient == m_pSimClient)
			continue;
		pClient->Activate();
		NewLuaThread(pClient->m_pLuaState, m_pLuaState, pClient->m_luaRef);
		if (pClient->m_pLuaState == NULL)
			break;
		if (!RunLua(pClient->m_pLuaState, filename.c_str())) {
			ReleaseLuaThread(pClient->m_pLuaState, m_pLuaState, 
				pClient->m_luaRef);
			break;
		}
		pClient->m_bRobot = true;
		if (luaState == NULL)
			luaState = pClient->m_pLuaState;
	}

	// get lua robot interval
	if (luaState) {
		lua_getglobal(luaState, "robotInterval");
		UInt32 interval = (UInt32) GetLuaInteger(luaState);
		if (interval > 0)
			m_robotInterval = interval;
	}
}

void Global::RegisterManager(const Char *prog, String &title, String &args)
{
	Char path[COMMAND_LENGTH];
	
	::GetCurrentDirectory(COMMAND_LENGTH, path);
	m_prog.Format(_T("%s\\%s"), path, prog);
	m_title = title;
	m_cmdLine = args;
	m_workDir = path;

	LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();
	Reuben::Network::IP localhost = Reuben::Network::StringToIP(_T("127.0.0.1"));

	m_pManagerPeer = rpcmgr.GetModel().Connect(Address(localhost, gConfigure.managerPort), 
		Reuben::Network::PROTOCOL_TCP, BOOLEAN_FALSE);

	if (m_pManagerPeer) {
		ManagerService_Proxy::RegisterSimClient(m_pManagerPeer, RPCContext(), m_prog, 
			m_title, m_workDir, m_cmdLine);
	}
}

void Global::RestartClient(UInt32 atTime)
{
	if (m_pManagerPeer == NULL)
		return;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Restart %s after %d second ..."), m_title.c_str(), atTime / 1000);

	ManagerService_Proxy::UnregisterSimClient(m_pManagerPeer, RPCContext());
	ManagerService_Proxy::StartProcess(m_pManagerPeer, RPCContext(), atTime, m_prog, m_title, 
		m_workDir, m_cmdLine);
	m_pManagerPeer = NULL;
	m_bExit = BOOLEAN_TRUE;
}

VOID Global::OnAccept(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID Global::OnOpen(CONST Listener* listener)
{
}

VOID Global::OnOpen(CONST Listener* listener, CONST Peer* pPeer)
{
}

VOID Global::OnFull(CONST Listener* listener)
{
}

VOID Global::OnClose(CONST Listener* listener)
{
}

VOID Global::OnClose(CONST Listener* listener, CONST Peer* pPeer)
{
	ThreadGuard guard(m_key);

	if (pPeer == NULL) return;

	//ClientSocketMap::iterator it = m_clientSockets.find(pPeer->GetSocketID());
	//if (it != m_clientSockets.end())
	PeerData* pData = (PeerData*) pPeer->GetData();
	if (pData && pData->pSimClient)
	{
		//CSimClient *pSimClient = it->second;
		CSimClient* pSimClient = pData->pSimClient;
		ThreadGuard guard(pSimClient->m_key);
		TRACE_INFODTL_2((*pSimClient->m_pLogger), _T("Socket OnClose: socket %d, sessionId %s"), 
			pPeer->GetSocketID(), pSimClient->m_servContext.sessionId.GetString().c_str());
		pSimClient->OnDisconnect();
		//m_clientSockets.erase(it);
		m_clientSockets.erase(pPeer->GetSocketID());
		// hope manager service restart this simclient when proxy server shutdown in multi-client 
		if (!m_bExit && m_clientSockets.empty() && m_bMulti && m_pManagerPeer) {
			m_pManagerPeer = NULL;
			m_bExit = BOOLEAN_TRUE;
		}
	}
	else if (m_defaultLogger)
	{
		if (m_pManagerPeer == pPeer)
			m_pManagerPeer = NULL;

		TRACE_INFODTL_1((*m_defaultLogger), _T("Socket OnClose: socket %d"), 
			pPeer->GetSocketID());
	}
}

Boolean RecvHandle(LPCPEER pPeer, RPCContext &context)
{
	ThreadGuard guard(gGlobal.m_key);
	Boolean success;
	bool needRelease = false;
	//SocketID socketId = (pPeer && pPeer->GetSocketID() != INVALID_SOCKET_ID) ? 
	//					pPeer->GetSocketID() : context.sessionId.socketId;

	//Global::ClientSocketMap::iterator it = gGlobal.m_clientSockets.find(socketId);
	if (pPeer == NULL)
	{
		pPeer = LocalRPCManager::GetInstance().GetModel().GetPeer(context.sessionId.socketId);
		needRelease = true;
	}

	//if (it != gGlobal.m_clientSockets.end() && it->second)
	PeerData* pData = pPeer ? (PeerData*) pPeer->GetData() : NULL;
	if (pData && pData->pSimClient)
	{	// activate SimClient
		//CSimClient *pSimClient = it->second;
		CSimClient* pSimClient = pData->pSimClient;
		if (pSimClient->m_servContext.sessionId.serviceId == 0)
			pSimClient->m_servContext.sessionId = context.sessionId;
		context.sessionId = pSimClient->m_context.sessionId;
		if (!pSimClient->m_bDelete && pSimClient->m_pProxyPeer && pPeer &&
			pPeer->GetSocketID() != INVALID_SOCKET_ID &&
			pSimClient->m_pProxyPeer->GetSocketID() != pPeer->GetSocketID())
		{
			TRACE_WARNDTL_2((*pSimClient->m_pLogger), 
				_T("RecHandle Warning: unmatched SimClient socketId %d to received %d"),
				pSimClient->m_pProxyPeer->GetSocketID(), pPeer->GetSocketID());
		}
		pSimClient->Activate();
		success = BOOLEAN_TRUE;
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, 
			_T("RecvHandle Warning: cannot find the simeclient SocketID %d"), 
			pPeer ? pPeer->GetSocketID() : context.sessionId.socketId);
		LocalRPCManager::GetInstance().SetNetIdMap(NULL);
		gGlobal.SetLogger(gGlobal.m_defaultLogger);
		gGlobal.m_pSimClient = NULL;
		success = BOOLEAN_FALSE;
	}
	if (needRelease && pPeer)
		pPeer->Release();

	return success;
}

// convert to ANSI string just for display
const char* ConvAnsiString(const String& s)
{
	static char buf[10][1024];			// 10 temp buffers
	static int bufIdx = 0;				// buffer index to rotate
	char *pBuf = buf[bufIdx++];
	int len;

	if (bufIdx >= 10)
		bufIdx = 0;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), (int)s.length(), 
			pBuf, 1024, 0, 0);
	if (len > 0)
		pBuf[len] = '\0';
	else
		pBuf[0] = '\0';

	return pBuf;
}

int UTF8ToStr(PCSTRINGA src, StringW &dst)
{
	if (src) {
		TCHAR buf[1024];
		int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, buf, 1024);
		buf[len] = _T('\0');
		dst.Format(_T("%s"), buf);
		return len;
	} else {
		dst = _T("");
		return 0;
	}
}

int StrToUTF8(PCSTRINGW src, StringA &dst)
{
	if (src)
	{
		char buf[1024];
		int len = WideCharToMultiByte(CP_UTF8, 0, src, -1, buf, 1024, NULL, NULL);
		buf[len] = '\0';
		dst.Format("%s", buf);
		return len;
	} else {
		dst = "";
		return 0;
	}
}

void RunNewGame()
{
	STARTUPINFO startupInfo = {0};
	startupInfo.cb = sizeof(startupInfo);
	PROCESS_INFORMATION processInformation;// Try to start the process
	
	TCHAR pathFilename[MAX_PATH];
	GetModuleFileName(NULL, pathFilename, MAX_PATH);
	BOOL result = ::CreateProcess(  
		pathFilename,

		NULL, 
		NULL,  
		NULL,  
		FALSE,  
		NORMAL_PRIORITY_CLASS,  
		NULL,  
		NULL,  
		&startupInfo,  
		&processInformation);
}

void Global::CheckDownTime(UInt32 dt)
{
	if (m_downTime == 0)
		return;

	// show count down when shutting down
	static UInt32 showTime = 0;
	UInt32 tdiff = showTime - m_downTime;
	int sec = -1;
	int min = -1;

	// reset count down
	if (showTime == 0 || m_downTime > showTime)
		tdiff = MINUTE_TIME;

	// show only for each interval
	if (m_downTime >= MINUTE_TIME && tdiff >= MINUTE_TIME)
	{	// show count down by minute
		min = (m_downTime + MINUTE_TIME / 2) / MINUTE_TIME;
		showTime = min * MINUTE_TIME;
	} else if (m_downTime < MINUTE_TIME && m_downTime > 10 * SECOND_TIME &&
				tdiff >= 10 * SECOND_TIME)
	{	// show count down by 10 second after 1 minute count down
		sec = (m_downTime + SECOND_TIME / 2) / SECOND_TIME;
		if (sec % 10 == 0)
			showTime = sec / 10 * 10 * SECOND_TIME;
		else
			showTime = (sec / 10 + 1) * 10 * SECOND_TIME;
	} else if (m_downTime < 10 * SECOND_TIME && tdiff >= SECOND_TIME)
	{	// show count down by 10 second after 10 second count down
		sec = (m_downTime + SECOND_TIME / 2) / SECOND_TIME;
		showTime = sec * SECOND_TIME;
	} else if (m_downTime <= dt) {
		sec = 0;
		showTime = 0;
	}

	if (m_downTime > dt)
		m_downTime -= dt;
	else
		m_downTime = 0;

	if (min != -1)
		_tprintf(_T("Services will shutdown in %d min ...\n"), min);
	else if (sec != -1)
		_tprintf(_T("Services will shutdown in %d sec ...\n"), sec);
}

PCSTRING Global::GetNpcName(UInt32 npc_id)
{
	const NpcData* npcData = m_resource->GetNpcData(npc_id);
	if (npcData) {
		return npcData->mid_name;
	} else {
		return _T("");
	}
}

PCSTRING Global::GetMapName(UInt16 map_id)
{
	const BinaryClientMapInfo* mapData = m_resource->GetCliMapData(map_id);
	if (mapData) {
		return mapData->m_mapname;
	} else {
		return _T("");
	}
}