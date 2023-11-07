#pragma once
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include <Reuben\Platform\File.h>

#define GLOBAL_SECTION _T("ListServer")
#define RESULT_FILE_KEY _T("result_file")
#define RESULT_FILE_DEFAULT _T("srvlist.txt")
#define UL_FILES_KEY _T("ul_files")
#define UL_FILES_DEFAULT _T("srvlist.txt")
#define UL_COMMAND_KEY _T("ul_command")
#define UL_COMMAND_DEFAULT _T("pscp")
#define UL_FLAGS_KEY _T("ul_flags")
#define UL_FLAGS_DEFAULT _T("-P 2222")
#define UL_COUNT_KEY _T("ul_count")
#define UL_COUNT_DEFAULT 0
#define SECTIONS_STRING_SIZE 8192

struct Database
{
	String server;
	String database;
	String login;
	String password;

	VOID Empty()
	{
		server.Empty();
		database.Empty();
		login.Empty();
		password.Empty();
	}
};

struct Configure
{
public:

	Reuben::Utility::ConfigFile config;
	String result_file;
	StringVector ul_files;
	String ul_command;
	String ul_flags;
	StringVector ul_paths;
	StringVector worlds;

	Boolean Initialize(PCSTRING list_server_ini)
	{
		if (!Reuben::Platform::File::IsExists(list_server_ini))
			return (BOOLEAN_FALSE);

		config.SetFileName(list_server_ini);

		// result file name
		result_file = config.GetString(GLOBAL_SECTION, RESULT_FILE_KEY, RESULT_FILE_DEFAULT);
		// upload files list
		String ul_files_temp = config.GetString(GLOBAL_SECTION, UL_FILES_KEY, UL_FILES_DEFAULT);
		int upper = 0;
		int lower = 0;
		while ((lower = ul_files_temp.Find(_T(','), upper)) != -1)
		{
			ul_files.push_back(ul_files_temp.Mid(upper, lower - upper).Trim());
			upper = lower + 1;
		}
		lower = ul_files_temp.GetLength();
		ul_files.push_back(ul_files_temp.Mid(upper, lower - upper).Trim());
		// upload command
		ul_command = config.GetString(GLOBAL_SECTION, UL_COMMAND_KEY, UL_COMMAND_DEFAULT);
		ul_flags = config.GetString(GLOBAL_SECTION, UL_FLAGS_KEY, UL_FLAGS_DEFAULT);
		// upload paths
		int ul_count = config.GetInt(GLOBAL_SECTION, UL_COUNT_KEY, UL_COUNT_DEFAULT);
		for (int i = 0; i < ul_count; ++i)
		{
			String key;
			key.Format(_T("ul_%d"), i + 1);
			ul_paths.push_back(config.GetString(GLOBAL_SECTION, key.c_str(), _T("")));
		}
		// get worlds section
		TCHAR sections_temp[SECTIONS_STRING_SIZE];
		::GetPrivateProfileSectionNames(sections_temp, SECTIONS_STRING_SIZE, list_server_ini);
		size_t len = 0;
		TCHAR* section_temp_ptr = sections_temp;
		while ((len = ::_tcslen(section_temp_ptr)) != 0)
		{
			if (::_tcscmp(section_temp_ptr, GLOBAL_SECTION) != 0)
				worlds.push_back(section_temp_ptr);
			section_temp_ptr += len + 1;
		}
		return (BOOLEAN_TRUE);
	}

	VOID Uninitialize()
	{
		for (unsigned int i = 0; i < ul_paths.size(); ++i)
			ul_paths[i].Empty();
		ul_paths.clear();
		ul_command.Empty();
		for (unsigned int i = 0; i < ul_files.size(); ++i)
			ul_files[i].Empty();
		ul_files.clear();
		result_file.Empty();
		config.~ConfigFile();
	}

protected:

};

extern Configure gConfigure;

#endif // _CONFIGURE_H_
