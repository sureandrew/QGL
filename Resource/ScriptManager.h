#pragma once
#ifndef _SCRIPT_MANAGER_H_
#define _SCRIPT_MANAGER_H_

class CScriptManager
{
public:
	CScriptManager();
	~CScriptManager();

	VOID Release();
	
	BOOL LoadFile(LPCTSTR szFolder = NULL);
	
	BOOL GetSelectFileTime(CONST String& szfilename, FILETIME & ft);
public:
	//lua
	BOOL LoadLuaFile(Int ReadType, BOOL bBatScript = FALSE);
	VOID ReleaseLuaLst();
	CONST char* GetLua(CONST StringA, size_t & len, BOOL bBatScript = FALSE);
private:

	//
	typedef struct _LuaBuff
	{
		size_t filesize;
		char*  Buff;
		_LuaBuff()
		{
			filesize = 0;
			Buff = NULL;
		};
	}
	LuaBuff;

	typedef StlMap<StringA, LuaBuff, std::less<StringA>, ManagedAllocator<std::pair<StringA, LuaBuff> > > LuaLst; 

	LuaLst						m_LuaLst;
	StlMap<StringA, FILETIME>	m_LuaFileTimeLst;
};

#endif // _SCRIPT_MANAGER_H_