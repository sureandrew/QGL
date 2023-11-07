#include <string>
#include <map>
#include <iostream>

class FileTemplate
{
private:
	struct MarkerRange
	{
		int start;
		int end;
	};
	typedef std::vector<string> Lines;
	typedef std::map<string, MarkerRange> Markers;
	int			m_lastIndex;
	Lines		m_lines;
	Markers		m_markerMap;

public:
	int getCount() { return m_markerMap.size(); }
	void clear();
	void read(string &filename);		// read template file
	bool prewrite(ostream &os, string marker);		// read marker
};

inline void FileTemplate::clear()
{
	m_lastIndex = 0;
	m_lines.clear();
	m_markerMap.clear();
}