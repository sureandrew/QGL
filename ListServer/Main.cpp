//-- Common
#include "Common.h"
#include "Version.h"
#include "Configure.h"
#include "Misc/Stackwalker.h"
#include "dboServiceStatus.h"
#include "DBPWDecrypt.h"

Configure gConfigure;

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

typedef struct proxy 
{
	int service_id;
	String name;
	String conn_string;
	int conn_count;
	bool matched;

} proxy, * PPROXY;

typedef std::vector<proxy> proxy_vector;

bool proxy_lesser(proxy& p1, proxy& p2)
{
	return (p1.matched != p2.matched) ? p1.matched == true : p1.conn_count < p2.conn_count;
}

int _tmain(int argc, TCHAR* argv[])
{
	if (!Reuben::System::Initialize())
		return (-1);

	InitAllocCheck(ACOutput_XML);

	if (argc < 2)
	{
		::_putts(_FE("no parameter found"));
		::_putts(_T("Usage: ListServer.exe INI_FILE"));
		return (-2);
	}

	if (!gConfigure.Initialize(argv[1]))
	{
		::_putts(_FE("initialize ini configure file failed"));
		return (-3);
	}

	gGlobalName = GLOBAL_SECTION;
	gGlobalConfig = &gConfigure.config;
	InitLogger();

	HRESULT hr = S_OK;
	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("oledb com initialization failed, hr 0x%08X"), hr);
		return (-3);
	}

	if (gConfigure.result_file.IsEmpty())
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _FE("result file is empty"));
		return (-4);
	}

	// attach result file to upload list
	gConfigure.ul_files.push_back(gConfigure.result_file);

	TRACE_ALWAYSDTL(GLOBAL_LOGGER, _F("-- start to update server list --"));

	Reuben::Utility::ConfigFile result_config(gConfigure.result_file.c_str());
	for (unsigned int i = 0; i < gConfigure.worlds.size(); ++i)
	{
		String& world = gConfigure.worlds[i];
		TRACE_ALWAYSDTL_1(GLOBAL_LOGGER, _F("processing world %s"), world.c_str());
		// name
		result_config.SetString(world.c_str(), _T("name"), gConfigure.config.GetString(world.c_str(), _T("name"), _T("")).c_str());	
		// zone
		result_config.SetString(world.c_str(), _T("zone"), gConfigure.config.GetString(world.c_str(), _T("zone"), _T("")).c_str());
		// download
		int dl_count = gConfigure.config.GetInt(world.c_str(), _T("dl_count"), 0);
		result_config.SetInt(world.c_str(), _T("dl_count"), dl_count);
		for (int i = 0; i < dl_count; ++i)
		{
			String dl_key;
			dl_key.Format(_T("dl_%d"), i + 1);
			result_config.SetString(world.c_str(), dl_key.c_str(), gConfigure.config.GetString(world.c_str(), dl_key.c_str(), _T("")).c_str());
		}
		// status
		int enable = gConfigure.config.GetInt(world.c_str(), _T("enable"), 0);
		if (!enable)
		{
			result_config.SetInt(world.c_str(), _T("status"), 0);		
			TRACE_ALWAYSDTL(GLOBAL_LOGGER, _F("done"));
			continue;
		}
		// rank
		result_config.SetString(world.c_str(), _T("rank"), gConfigure.config.GetString(world.c_str(), _T("rank"), _T("0")).c_str());
		int busy_state = gConfigure.config.GetInt(world.c_str(), _T("busy_state"), 0);
		int stuck_state = gConfigure.config.GetInt(world.c_str(), _T("stuck_state"), 0);
		// proxy
		String db_ini = gConfigure.config.GetString(world.c_str(), _T("db_ini"), _T(""));
		String db_name = gConfigure.config.GetString(world.c_str(), _T("db_name"), _T(""));
		String world_ini = gConfigure.config.GetString(world.c_str(), _T("world_ini"), _T(""));
		if (!Reuben::Platform::File::IsExists(db_ini.c_str()) || db_name.IsEmpty() || !Reuben::Platform::File::IsExists(world_ini.c_str()))
		{
			TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("db_ini not exists or db_name is empty or world_ini is not exists, world %s, db_ini %s, db_name %s, world_ini %s"), world.c_str(), db_ini.c_str(), db_name.c_str(), world_ini.c_str());
			result_config.SetInt(world.c_str(), _T("status"), 0);		
			TRACE_ALWAYSDTL(GLOBAL_LOGGER, _F("done"));
			continue;
		}
		// retrieve proxy(s) in world ini file
		Reuben::Utility::ConfigFile world_config(world_ini.c_str());
		TCHAR sections_temp[SECTIONS_STRING_SIZE];
		::GetPrivateProfileSectionNames(sections_temp, SECTIONS_STRING_SIZE, world_ini.c_str());
		size_t len = 0;
		TCHAR* section_temp_ptr = sections_temp;
		proxy_vector pv;
		while ((len = ::_tcslen(section_temp_ptr)) != 0)
		{
			if (StringToServiceType(world_config.GetString(section_temp_ptr, _T("Type"), _T("")).c_str()) == SERVICE_PROXY)
			{
				proxy p;
				p.service_id = world_config.GetInt(section_temp_ptr, _T("ServiceID"), 0);
				p.name = section_temp_ptr;
				p.conn_string = world_config.GetString(section_temp_ptr, _T("ConnString"), _T(""));
				if (p.conn_string.IsEmpty())
					p.conn_string.Format(_T("%s:%d"), world_config.GetString(section_temp_ptr, _T("PublicIP"), _T("")).c_str(), world_config.GetInt(section_temp_ptr, _T("PublicPort"), 0));
				p.conn_count = 0;
				p.matched = false;
				pv.push_back(p);
			}
			section_temp_ptr += len + 1;
		}
		if (pv.empty())
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("dbo no proxy available, world %s"), world.c_str());
			result_config.SetInt(world.c_str(), _T("status"), 0);		
			TRACE_ALWAYSDTL(GLOBAL_LOGGER, _F("done"));
			continue;
		}
		// retrieve connection count status for each proxy in database
		Reuben::Utility::ConfigFile db_config(db_ini.c_str());
		Database db;
		db.server = db_config.GetString(db_name.c_str(), _T("Server"), _T(""));
		db.database = db_config.GetString(db_name.c_str(), _T("Database"), _T(""));
		db.login = db_config.GetString(db_name.c_str(), _T("Login"), _T(""));
		db.password = db_config.GetString(db_name.c_str(), _T("Password"), _T(""));
		if (db.server.IsEmpty() || db.database.IsEmpty() || db.login.IsEmpty() || db.password.IsEmpty())
		{
			TRACE_WARNDTL_5(GLOBAL_LOGGER, _FW("db parameter(s) may empty, world %s, db_server %s, db_databasae %s, db_login %s, db_password %s"),
				world.c_str(), db.server.c_str(), db.database.c_str(), db.login.IsEmpty() ? _T("empty") : _T("not empty"), db.password.IsEmpty() ? _T("empty") : _T("not empty"));
			result_config.SetInt(world.c_str(), _T("status"), 0);		
			TRACE_ALWAYSDTL(GLOBAL_LOGGER, _F("done"));
			continue;
		}
		CDboSource* dbs = SafeCreateObject(CDboSource);
#ifdef CONNECT_STR
		String cstr1;
		cstr1.Format(CONNECT_STR, db.server.c_str(), db.database.c_str(), db.login.c_str(), DBPWDecrypt(db.password).c_str());
		if (DB_FAILED(hr = dbs->Open(cstr1.c_str())))
#else
		if (DB_FAILED(hr = dbs->Open(db.server.c_str(), db.database.c_str(), db.login.c_str(), DBPWDecrypt(db.password).c_str())))
#endif	// CONNECT_STR
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("connect to database failed, world %s, hr 0x%08X"), world.c_str(), hr);
			SafeDeleteObject(dbs);
			result_config.SetInt(world.c_str(), _T("status"), 0);		
			TRACE_ALWAYSDTL(GLOBAL_LOGGER, _F("done"));
			continue;
		}
		CdboServiceStatus* dbo = SafeCreateObject(CdboServiceStatus);
		dbo->SetDataSource(dbs);
		if (FAILED(hr = dbo->Select()))
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("dbo select failed, world %s, hr 0x%08X"), world.c_str(), hr);
			SafeDeleteObject(dbo);
			SafeDeleteObject(dbs);
			result_config.SetInt(world.c_str(), _T("status"), 0);		
			TRACE_ALWAYSDTL(GLOBAL_LOGGER, _F("done"));
			continue;
		}
		hr = dbo->MoveFirst();
		int player_sum = 0;
		while (hr == S_OK)
		{
			for (size_t i = 0; i < pv.size(); ++i)
			{
				if (dbo->m_service_id == pv[i].service_id)
				{
					pv[i].conn_count = dbo->m_state;
					pv[i].matched = true;
					player_sum += dbo->m_state;
					TRACE_ALWAYSDTL(GLOBAL_LOGGER, _F("done"));
					continue;
				}
			}
			hr = dbo->MoveNext();
		}
		dbo->Close();
		SafeDeleteObject(dbo);
		dbs->Close();
		SafeDeleteObject(dbs);
		int status = 1;
		if (player_sum >= busy_state)
			status = 2;
		if (player_sum >= stuck_state)
			status = 3;
		result_config.SetInt(world.c_str(), _T("status"), status);		
		result_config.SetInt(world.c_str(), _T("proxy_count"), (int)pv.size());
		std::sort(pv.begin(), pv.end(), proxy_lesser);
		for (size_t i = 0; i < pv.size(); ++i)
		{
			proxy& p = pv[i];
			String key;
			key.Format(_T("proxy_%d"), i + 1);
			result_config.SetString(world.c_str(), key.c_str(), p.conn_string.c_str());
			if (p.matched == false)
				TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("proxy not matched with datatbase record, service_id %d, service_name %s, conn_string %s"), p.service_id, p.name.c_str(), p.conn_string.c_str());
		}
		TRACE_ALWAYSDTL(GLOBAL_LOGGER, _F("done"));
	}

	for (size_t i = 0; i < gConfigure.ul_paths.size(); ++i)
	{
		for (size_t j = 0; j < gConfigure.ul_files.size(); ++j)
		{
			if (!Reuben::Platform::File::IsExists(gConfigure.ul_files[j].c_str()))
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("_tmain warning : file not exists, file %s"), gConfigure.ul_files[j].c_str());
				continue;
			}
			String params;
			params.Format(_T("%s %s %s"), gConfigure.ul_flags.c_str(), gConfigure.ul_files[j].c_str(), gConfigure.ul_paths[i].c_str());
			TRACE_ALWAYSDTL_2(GLOBAL_LOGGER, _F("execute : %s %s"), gConfigure.ul_command.c_str(), params.c_str());
			HINSTANCE result = ShellExecute(NULL, NULL, gConfigure.ul_command.c_str(), params.c_str(), NULL, SW_SHOW);
			if ((int)result <= 32)
				TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("_tmain warning : shell execute failed, result %d, command %s %s"), result, gConfigure.ul_command.c_str(), params.c_str());
		}
	}

	result_config.SetInt(_T("FINISH"), _T("finish"), 1);
	
	TRACE_ALWAYSDTL(GLOBAL_LOGGER, _F("-- finish to update server list --"));

	CoUninitialize();

	gConfigure.Uninitialize();
	gGlobalLogger->ForceArchive();	// force to archive log file
	REUEBN_SYSTEM_MEMORY_REGISTER_LEAK_REPORT(GLOBAL_LOGGER.GetLogFileName().c_str());
	CloseLogger();
	Reuben::System::Uninitialize();
	DeInitAllocCheck();

	return 0;
}
