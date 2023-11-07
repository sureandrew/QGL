#include <iostream>
#include <map>
#include "Backend.h"

typedef std::vector<string> IdlList;

struct Section
{
	string name;
	string path;
	string rootPath;
	string enumPath;
	string proxyPath;
	string stubPath;
	int version;
	bool proxyNeed;
	bool stubNeed;
	IdlList proxyList;
	IdlList stubList;
	IdlList incList;

	Section() : version(0), proxyNeed(false), stubNeed(false) {}
};

class TaskList
{
public:
	enum {
		IDL_INVALID		= 0,
		IDL_DONE		= 1,
		IDL_UNCHANGED	= 2,
		IDL_MODIFIED	= 3
	};

private:
	typedef std::vector<Section> Sections;
	typedef std::map<string, int> FileMap;
	Sections m_sections;
	Section m_defSect;			// default section
	FileMap m_doneMap;
	FileMap	m_idlMap;
	int AddIDL(string idl);

public:
	void LoadTask(string filename);
	int CheckIDL(string idl);
	Section& GetSection(int i);
	int GetSectionCount();
	int GetVersion() { return m_defSect.version; }
	int IsDone(string filename);
	void SetDone(string filename);
	void SetInvalidIDL(string idl);
	void ClearAllArchive();
};

inline Section& TaskList::GetSection(int i)
{
	return m_sections[i];
}

inline int TaskList::GetSectionCount()
{
	return (int) m_sections.size();
}

inline int TaskList::IsDone(string filename)
{
	if (m_doneMap.find(filename) != m_doneMap.end())
		return m_doneMap[filename];
	else
		return IDL_INVALID;
}

inline void TaskList::SetDone(string filename)
{
	m_doneMap[filename] = IDL_DONE;
}

inline int TaskList::CheckIDL(string idl)
{
	if (m_idlMap.find(idl) != m_idlMap.end())
		return m_idlMap[idl];
	return IDL_INVALID;
}
inline void TaskList::SetInvalidIDL(string idl)
{
	if (m_idlMap.find(idl) != m_idlMap.end())
		m_idlMap[idl] = IDL_INVALID;
}

inline void TaskList::ClearAllArchive()
{
	for (FileMap::iterator iter = m_idlMap.begin(); iter != m_idlMap.end(); iter++)
		ClearArchive(iter->first);
}