#include "stdafx.h"
#include "TaskList.h"

#define DEFAULT_SECTION			"[Default]"

void TaskList::LoadTask(string filename)
{
	ifstream is;
	const char ws[] = " \t=";
	char buf[4096];
	char *token;
	Section* psect;

	// set default
	m_defSect.rootPath = "../";
	m_defSect.enumPath = "RPCEnum/";
	m_defSect.proxyPath = "Proxy/";
	m_defSect.stubPath = "Stub/";

	is.open(filename.c_str());
	while (!is.fail()) {
		is.getline(buf, 4096);
		token = strtok(buf, ws);
		if (token == NULL || token[0] == '#')
			continue;
		if (token[0] == '[' && token[strlen(token) - 1] == ']') {
			if (stricmp(token, DEFAULT_SECTION) == 0) {
				psect = &m_defSect;
			} else {
				Section section;
				m_sections.push_back(section);
				psect = &(m_sections.back());
			}

			token[strlen(token) - 1] = '\0';
			psect->name = token + 1;
			psect->path = "";
			psect->rootPath = "";
			psect->enumPath = "";
			psect->proxyPath = "";
			psect->stubPath = "";
			psect->proxyNeed = false;
			psect->stubNeed = false;
			psect->version = 0;
		} else if (stricmp(token, "path") == 0) {
			token = strtok(NULL, ws);
			psect->path = token;
		} else if (stricmp(token, "rootPath") == 0) {
			token = strtok(NULL, ws);
			psect->rootPath = token;
		} else if (stricmp(token, "enumPath") == 0) {
			token = strtok(NULL, ws);
			psect->enumPath = token;
		} else if (stricmp(token, "proxyPath") == 0) {
			token = strtok(NULL, ws);
			psect->proxyPath = token;
		} else if (stricmp(token, "stubPath") == 0) {
			token = strtok(NULL, ws);
			psect->stubPath = token;
		} else if (stricmp(token, "version") == 0) {
			token = strtok(NULL, ws);
			psect->version = _ttoi(token);
		} else if (stricmp(token, "proxy") == 0) {
			token = strtok(NULL, ws);
			while (token) {
				int fstat = AddIDL(token);
				if (fstat != IDL_INVALID)
					psect->proxyList.push_back(token);
				if (fstat == IDL_MODIFIED)
					psect->proxyNeed = true;
				token = strtok(NULL, ws);
			}
		} else if (stricmp(token, "stub") == 0) {
			token = strtok(NULL, ws);
			while (token) {
				int fstat = AddIDL(token);
				if (fstat != IDL_INVALID)
					psect->stubList.push_back(token);
				if (fstat == IDL_MODIFIED)
					psect->stubNeed = true;
				token = strtok(NULL, ws);
			}
		} else if (stricmp(token, "include") == 0) {
			token = strtok(NULL, ws);
			while (token) {
				psect->incList.push_back(token);
				token = strtok(NULL, ws);
			}
		}
	}
	is.close();

	// set default value
	for (Sections::iterator iter = m_sections.begin(); iter != m_sections.end(); iter++)
	{
		Section &sect = *iter;

		if (sect.path == "")
			sect.path = m_defSect.path;
		if (sect.rootPath == "")
			sect.rootPath = m_defSect.rootPath;
		if (sect.enumPath == "")
			sect.enumPath = m_defSect.enumPath;
		if (sect.proxyPath == "")
			sect.proxyPath = m_defSect.proxyPath;
		if (sect.stubPath == "")
			sect.stubPath = m_defSect.stubPath;
	}
}

int TaskList::AddIDL(string idl)
{
	int fstat = IDL_INVALID;

	// remove special character from IDL name
	if (idl.compare(0, 1, "-") == 0)
		idl = idl.substr(1);

	if (IsFileExist(idl)) {
		fstat = IsFileModified(idl) ? IDL_MODIFIED : IDL_UNCHANGED;
		m_idlMap[idl] = fstat;
	}

	return fstat;
}