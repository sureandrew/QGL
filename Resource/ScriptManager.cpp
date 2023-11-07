#include "common.h"
#include "ScriptManager.h"
#include <shlwapi.h>

#define SZ_MAPSCRIPT_PATHFOLDER						".\\Data\\Script\\"
#define SZ_BATSCRIPT_PATHFOLDER						".\\Data\\Script\\BatScript\\"
#define SZ_LOAD_ALL_LUASUBFILE						"*.lua"
#define SZ_LOAD_ALL_OUTSUBFILE						"*.out"

CScriptManager::CScriptManager()
{
	m_LuaLst.clear();
	m_LuaFileTimeLst.clear();
}

CScriptManager::~CScriptManager()
{
	Release();
}

VOID CScriptManager::Release()
{
	ReleaseLuaLst();
}

BOOL CScriptManager::GetSelectFileTime(CONST String& szfilename, FILETIME & ft)
{
	ft.dwHighDateTime = ft.dwLowDateTime = 0;
	BOOL nResult(FALSE);
	{
		//_stprintf(temp, _T("%s\\%s"), _T("C:\\GameProject\\QGLSource\\Bin\\Data\\Script"), ::PathFindFileName(szfilename.c_str()));
		//::GetFileTitle(szfilename.c_str(), temp, _MAX_PATH);

		HANDLE hFile = CreateFile(szfilename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		//TCHAR temp2[_MAX_PATH] = _T("");
        //GetCurrentDirectory(_MAX_PATH, temp2);
		//::GetFullPathName(temp, _MAX_PATH, temp2, NULL);

		if ( hFile && hFile != INVALID_HANDLE_VALUE ) 
		{
			nResult = GetFileTime(hFile, NULL, NULL, &ft);
		}
		CloseHandle(hFile);
	}

	return nResult;
}

BOOL CScriptManager::LoadLuaFile(Int ReadType, BOOL bBatScript)
{
	ReleaseLuaLst();

	// ReadType = 1 : read .out file , 0 : read .lua file
	StringA szSubFile = (ReadType)?SZ_LOAD_ALL_OUTSUBFILE:SZ_LOAD_ALL_LUASUBFILE;

	StringA szFolder = (bBatScript)?SZ_BATSCRIPT_PATHFOLDER:SZ_MAPSCRIPT_PATHFOLDER;

	StringA szPath("");
	szPath.Format("%s%s", szFolder.c_str(), szSubFile.c_str());

	StlVector<StringA> filequeue;
	filequeue.clear();

	WIN32_FIND_DATAA data = {0};
	HANDLE hFile = ::FindFirstFileA(szPath.c_str(), &data);
	if ( hFile && hFile != INVALID_HANDLE_VALUE )
	{
		do
		{
			StringA szfilename("");
			szfilename.Format("%s%s", szFolder.c_str(), data.cFileName);

			filequeue.push_back(szfilename);
		}
		while ( FindNextFileA(hFile, &data) );
		FindClose( hFile );
	}

	Int size = (Int)filequeue.size();
	//for ( StlVector<StringA>::iterator Itr = filequeue.begin(); Itr != filequeue.end(); ++Itr )
	for ( Int i = 0; i < size; ++i )
	{
		{
			FILE* pfile = NULL;
			if ( pfile = fopen(filequeue[i].c_str(), "rb") )
			{
				long cur_pos = 0, length = 0;
				cur_pos = ftell(pfile);
				fseek(pfile, 0, SEEK_END); // set pointer to end of file
				length=ftell(pfile); // offset in bytes from file's beginning
				fseek(pfile, 0, SEEK_SET); // restore original position

				char* pNewLua = SafeAllocate(char, length+1);
				if ( pNewLua )
				{
					memset(pNewLua, 0, length+1);
					pNewLua[length] = 0;
					fread(pNewLua, length, 1, pfile);
					fclose(pfile);
					StringA filetitle("");
					CharA tempfile[_MAX_PATH] = "";
					sprintf(tempfile, "%s", filequeue[i].c_str());
					filetitle.Format("%s", ::PathFindFileNameA(tempfile));

					LuaLst::iterator finditr = m_LuaLst.find(filetitle);
					if ( finditr == m_LuaLst.end() )
					{
						LuaBuff temp;
						temp.filesize = length;
						temp.Buff = pNewLua;
						m_LuaLst.insert(std::make_pair(filetitle, temp));

						{
							FILETIME ft;
							GetSelectFileTime(filequeue[i].c_str(), ft);

							StlMap<StringA, FILETIME>::iterator findItr2 = m_LuaFileTimeLst.find(filetitle);
							if ( findItr2 == m_LuaFileTimeLst.end() )
								m_LuaFileTimeLst.insert(std::make_pair(filetitle, ft));
						}
					}
					else
						SafeDeallocate(pNewLua);
				}
				fclose(pfile);
			}
		}
	}

	filequeue.clear();
	return TRUE;
}

VOID CScriptManager::ReleaseLuaLst()
{
	for ( LuaLst::iterator Itr = m_LuaLst.begin(); Itr != m_LuaLst.end(); ++Itr )
	{
		SafeDeallocate((Itr->second).Buff);
	}

	m_LuaLst.clear();
	m_LuaFileTimeLst.clear();
}

CONST char* CScriptManager::GetLua(CONST StringA filename, size_t & len, BOOL bBatScript)
{
	if ( filename.CompareNoCase("") == 0 )
		return NULL;

	{
		FILETIME ft;
		StringA SrcPath("");
		if ( bBatScript )
			SrcPath.Format("%s%s", SZ_BATSCRIPT_PATHFOLDER, filename.c_str());
		else
			SrcPath.Format("%s%s", SZ_MAPSCRIPT_PATHFOLDER, filename.c_str());
		GetSelectFileTime(SrcPath.c_str(), ft);

		StlMap<StringA, FILETIME>::iterator Itr = m_LuaFileTimeLst.find(filename);
		if ( Itr != m_LuaFileTimeLst.end() )
		{
			DWORD high = Itr->second.dwHighDateTime;
			DWORD low = Itr->second.dwLowDateTime;

			if( (high == ft.dwHighDateTime) && (low == ft.dwLowDateTime) )
			{
				LuaLst::iterator FindItr = m_LuaLst.find(filename);
				if ( FindItr != m_LuaLst.end() )
				{
					len = (FindItr->second).filesize;
					return (FindItr->second).Buff;
				}
				return NULL;
			}
			else if ( ( ft.dwHighDateTime == 0 ) && ( ft.dwLowDateTime == 0 ) )
			{
				// Remove Script
				LuaLst::iterator RemoveItr = m_LuaLst.find(filename);
				if ( RemoveItr != m_LuaLst.end() )
				{
					SafeDeallocate((RemoveItr->second).Buff);
					
					m_LuaLst.erase(RemoveItr);
				}
				m_LuaFileTimeLst.erase(Itr);
			}
			else
			{
				// Reload script with update ver
				FILE* pfile = NULL;
				if ( pfile = fopen(SrcPath.c_str(), "rb") )
				{
					long cur_pos = 0, length = 0;
					cur_pos = ftell(pfile);
					fseek(pfile, 0, SEEK_END); // set pointer to end of file
					length=ftell(pfile); // offset in bytes from file's beginning
					fseek(pfile, 0, SEEK_SET); // restore original position

					char* pNewLua = SafeAllocate(char, length+1);
					if ( pNewLua )
					{
						memset(pNewLua, 0, length+1);
						pNewLua[length] = 0;
						fread(pNewLua, length, 1, pfile);
						fclose(pfile);

						LuaLst::iterator SetItr = m_LuaLst.find(filename);
						if ( SetItr != m_LuaLst.end() )
						{
							SafeDeallocate((SetItr->second).Buff);

							(SetItr->second).filesize = length;
							(SetItr->second).Buff = pNewLua;

							len = length;

							Itr->second = ft;
						}
						else
							SafeDeallocate(pNewLua);
					}
					fclose(pfile);

					return pNewLua;
				}
			}
		}
		else if ( (ft.dwHighDateTime != 0) || (ft.dwLowDateTime != 0) )
		{
			// Add Non-existing Script
			FILE* pfile = NULL;
			if ( pfile = fopen(SrcPath.c_str(), "rb") )
			{
				long cur_pos = 0, length = 0;
				cur_pos = ftell(pfile);
				fseek(pfile, 0, SEEK_END); // set pointer to end of file
				length=ftell(pfile); // offset in bytes from file's beginning
				fseek(pfile, 0, SEEK_SET); // restore original position

				char* pNewLua = SafeAllocate(char, length+1);
				if ( pNewLua )
				{
					memset(pNewLua, 0, length+1);
					pNewLua[length] = 0;
					fread(pNewLua, length, 1, pfile);
					fclose(pfile);

					LuaLst::iterator AddItr = m_LuaLst.find(filename);
					if ( AddItr == m_LuaLst.end() )
					{
						LuaBuff temp;
						temp.filesize = length;
						temp.Buff = pNewLua;

						len = length;

						m_LuaLst.insert(std::make_pair(filename, temp));

						m_LuaFileTimeLst.insert(std::make_pair(filename, ft));
					}
					else
						SafeDeallocate(pNewLua);
				}
				fclose(pfile);

				return pNewLua;
			}
		}
	}
	/*
	LuaLst::iterator Itr = m_LuaLst.find(filename);
	if ( Itr != m_LuaLst.end() )
	{
		len = (Itr->second).filesize;
		return ((Itr->second).Buff);
	}
	*/
	return NULL;
}