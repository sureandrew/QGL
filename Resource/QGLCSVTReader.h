#pragma once
#ifndef _CQGLCSVTREADER_H
#define _CQGLCSVTREADER_H

#include "UTF8File.h"
#include "Reuben\System\Reuben.h"

typedef StlMap<String, int, std::less<String>, ManagedAllocator<std::pair<String, int> > > MapNameColumn;
//typedef StlMap<int, String, std::less<int>, ManagedAllocator<std::pair<int, String> > > MapColumnValue;
typedef StlVector<String, ManagedAllocator<String> > MapColumnValue;

class CQGLCSVTReader
{
public:
	CQGLCSVTReader();
	~CQGLCSVTReader();

	bool Open(const TCHAR* csvfilename, bool hasMetaData = false);
	bool OpenBuf(const BYTE* pData, unsigned int pDataSize, bool hasMetaData = false);
	bool Close();

	bool IsLineEmpty();
	bool ReadString(const String &fieldid, TCHAR *out, int stringLength);
	bool ReadString(const String &fieldid, String& outStr);
	bool Read(const String &fieldid, Int64 &out);
	bool Read(const String &fieldid, Int32 &out);
	bool Read(const String &fieldid, Int16 &out);
	bool Read(const String &fieldid, Int8  &out);
	bool Read(const String &fieldid, UInt64 &out);
	bool Read(const String &fieldid, UInt32 &out);
	bool Read(const String &fieldid, UInt16 &out);
	bool Read(const String &fieldid, UInt8  &out);
	bool Read(const String &fieldid, Boolean &out);
	bool Read(const String &fieldid, Real32 &out);
	bool Read(const String &fieldid, Real64 &out);
	bool ReadVector(const String &fieldid, UInt8Vector &vec);
	bool ReadVector(const String &fieldid, UInt16Vector &vec);
	bool ReadVector(const String &fieldid, UInt32Vector &vec);
	bool MoveNext();

private:
	void AddColumnName(const String &name, int columnno);
	bool BufferCurrLine();
	bool ReadMetaData();
	void ResetFieldValue();
	void SetFieldValue(int column_id, TCHAR* value);
	void SkipNext();
	bool AnalyseCurrString(TCHAR* srcString, int len, TCHAR* composedString, int& outlen);
	const String* GetValue(const String &fieldName);

	int colid;
	int quotationmarkfound;
	int justquotationmark;
	int currentwordreadpos;
	TCHAR* a;
	TCHAR* b;
	bool isnewline;
	bool hasmetadata;
	Int32 m_tempint32;

	char* filename;
	UTF8File* utf8file;

	bool isFirstLine;
    
	MapNameColumn m_namecolumn;
	MapColumnValue m_columnvalue;
};

#endif