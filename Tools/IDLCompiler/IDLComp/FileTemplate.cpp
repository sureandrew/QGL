#include "stdafx.h"
#include "FileTemplate.h"

void FileTemplate::read(string &filename)
{
	ifstream is;
	try 
	{
		char buf[1024];

		is.open(filename.c_str());
		while (!is.fail()) {
			is.getline(buf, 1024);
			// find marker
			if (strncmp(buf, "@@", 2) == 0) {
				string s = string(buf + 2);
				MarkerRange r = { m_lastIndex, m_lines.size() };
				m_markerMap[s] = r;
				m_lastIndex = m_lines.size();
			} else {
				m_lines.push_back(string(buf));
			}
		}
		is.close();
	}
	catch(exception& e)
	{
		cerr << "exception: " << e.what() << endl;
		return;
	}
}

bool FileTemplate::prewrite(ostream &os, string marker)
{
	Markers::iterator iter = m_markerMap.find(marker);
	if (iter == m_markerMap.end()) {
		//cout << "Warning: no marker '" << marker << "' found!\n";
		return false;
	}

	// prewrite lines before marker
	for (int i = iter->second.start; i < iter->second.end; i++) {
		os << m_lines[i] << endl;
	}

	return true;
}