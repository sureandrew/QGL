#include "common.h"
#include "QGLCSVTReader.h"

CQGLCSVTReader::CQGLCSVTReader()
{
	a = SafeAllocate(TCHAR, 2048);
	b = SafeAllocate(TCHAR, 2048);
	filename = SafeAllocate(char, 1024);
	utf8file = SafeCreateObject(UTF8File);
}

CQGLCSVTReader::~CQGLCSVTReader()
{
	m_columnvalue.clear();
	m_namecolumn.clear();
	SafeDeallocate(a);
	SafeDeallocate(b);
	SafeDeallocate(filename);
	SafeDeleteObject(utf8file);
}

bool CQGLCSVTReader::Open(const TCHAR* csvfilename, bool hasMetaData)
{
	if (!utf8file )
		return false;

	isFirstLine = true;
	colid = 0;
	quotationmarkfound = 0;
	justquotationmark = 0;
	currentwordreadpos = 0;
	a[0] = 0;
	isnewline = false;
	hasmetadata = false;

	m_columnvalue.clear();
	m_namecolumn.clear();

	hasmetadata = hasMetaData;

	if( utf8file->Open(csvfilename, "rt") )
	{
		ReadMetaData();
		BufferCurrLine();
		return true;
	}else
		return false;
}

bool CQGLCSVTReader::OpenBuf(const BYTE* pData, unsigned int pDataSize, bool hasMetaData)
{
	if (!utf8file )
		return false;

	isFirstLine = true;
	colid = 0;
	quotationmarkfound = 0;
	justquotationmark = 0;
	currentwordreadpos = 0;
	a[0] = 0;
	isnewline = false;
	hasmetadata = false;

	m_columnvalue.clear();
	m_namecolumn.clear();

	hasmetadata = hasMetaData;

	if( utf8file->OpenBuf(pData, pDataSize) )
	{
		ReadMetaData();
		BufferCurrLine();
		return true;
	}else
		return false;
}

bool CQGLCSVTReader::Close()
{
	if( !utf8file )
	{
		throw;
		return false;
	}

	utf8file->Close();
	m_columnvalue.clear();
	m_namecolumn.clear();
	return true;
}

const String* CQGLCSVTReader::GetValue(const String &fieldName)
{
	MapNameColumn::iterator it1 = m_namecolumn.find(fieldName);
	if (it1 != m_namecolumn.end()) {
		return &m_columnvalue[it1->second];
	}
	return NULL;
}

bool CQGLCSVTReader::ReadString(const String &fieldid, TCHAR *out, int stringLength)
{
	const String *pstr = GetValue(fieldid);
	if (pstr == NULL)
		return false;

	// choose smaller length for copying
	int len = MIN(pstr->GetLength(), stringLength);
	_tcsncpy(out, pstr->c_str(), len);
	out[len] = 0;
	return true;
}

bool CQGLCSVTReader::ReadString(const String &fieldid, String& outStr)
{
	const String *pstr = GetValue(fieldid);
	if (pstr == NULL)
		return false;

	outStr = *pstr;
	return true;
}

bool CQGLCSVTReader::Read(const String &fieldid, Int64 &out)
{
	const String *pstr = GetValue(fieldid);
	if (pstr) {
		out = _ttol(pstr->c_str());
		return true;
	} else {
		return false;
	}
}

bool CQGLCSVTReader::Read(const String &fieldid, UInt64 &out)
{
	return Read(fieldid, (Int64&)out);
}

bool CQGLCSVTReader::Read(const String &fieldid, Int32 &out)
{
	const String *pstr = GetValue(fieldid);
	if (pstr) {
		out = _ttoi(pstr->c_str());
		return true;
	} else {
		return false;
	}
}

bool CQGLCSVTReader::Read(const String &fieldid, UInt32 &out)
{
	return Read(fieldid, (Int32&)out);
}

bool CQGLCSVTReader::Read(const String &fieldid, Int16 &out)
{
	const String *pstr = GetValue(fieldid);
	if (pstr) {
		out = _ttoi(pstr->c_str());
		return true;
	} else {
		return false;
	}
}

bool CQGLCSVTReader::Read(const String &fieldid, UInt16 &out)
{
	return Read(fieldid, (Int16&)out);
}

bool CQGLCSVTReader::Read(const String &fieldid, Int8 &out)
{
	const String *pstr = GetValue(fieldid);
	if (pstr) {
		out = _ttoi(pstr->c_str());
		return true;
	} else {
		return false;
	}
}

bool CQGLCSVTReader::Read(const String &fieldid, UInt8 &out)
{
	return Read(fieldid, (Int8&)out);
}

bool CQGLCSVTReader::Read(const String &fieldid, Boolean &out)
{
	return Read(fieldid, (Int8&)out);
}

bool CQGLCSVTReader::Read(const String &fieldid, Real64 &out)
{
	const String *pstr = GetValue(fieldid);
	if (pstr) {
		out = _tstof(pstr->c_str());
		return true;
	} else {
		return false;
	}
}

bool CQGLCSVTReader::Read(const String &fieldid, Real32 &out)
{
	const String *pstr = GetValue(fieldid);
	if (pstr) {
		out = (Real32)_tstof(pstr->c_str());
		return true;
	} else {
		return false;
	}
}

bool CQGLCSVTReader::ReadVector(const String &fieldid, UInt8Vector &vec)
{
	vec.clear();

	const String *pstr = GetValue(fieldid);
	if (pstr == NULL)
		return false;

	TCHAR *delim = _T("|");
	TCHAR *buf = _tcsdup(pstr->c_str());
	TCHAR *token = _tcstok(buf, delim);
	while (token) {
		vec.push_back((UInt8) _ttoi(token));
		token = _tcstok(NULL, delim);
	}
	if (buf)
		free(buf);
	return true;
}


bool CQGLCSVTReader::ReadVector(const String &fieldid, UInt16Vector &vec)
{
	vec.clear();

	const String *pstr = GetValue(fieldid);
	if (pstr == NULL)
		return false;

	TCHAR *delim = _T("|");
	TCHAR *buf = _tcsdup(pstr->c_str());
	TCHAR *token = _tcstok(buf, delim);
	while (token) {
		vec.push_back((UInt16) _ttoi(token));
		token = _tcstok(NULL, delim);
	}
	if (buf)
		free(buf);
	return true;
}


bool CQGLCSVTReader::ReadVector(const String &fieldid, UInt32Vector &vec)
{
	vec.clear();

	const String *pstr = GetValue(fieldid);
	if (pstr == NULL)
		return false;

	TCHAR *delim = _T("|");
	TCHAR *buf = _tcsdup(pstr->c_str());
	TCHAR *token = _tcstok(buf, delim);
	while (token) {
		vec.push_back((UInt32) _ttoi(token));
		token = _tcstok(NULL, delim);
	}
	if (buf)
		free(buf);
	return true;
}

bool CQGLCSVTReader::MoveNext()
{
	SkipNext();

	ResetFieldValue();
	if( hasmetadata && isFirstLine )
		ReadMetaData();
	else
		BufferCurrLine();

	return true;
}

void CQGLCSVTReader::SkipNext()
{
	quotationmarkfound = 0;
	if( !isnewline )
	{
		while( (utf8file->Read(a[0]) && a[0]!=_T('\n')) || quotationmarkfound==1)
		{
			if( a[0]==_T('"') )
			{
				switch( quotationmarkfound )
				{
				case 0:
					quotationmarkfound = 1;
					break;
				case 1:
					quotationmarkfound = 0;
					break;
				}
			}
		}
	}
	isnewline = true;
	colid = 0;
	quotationmarkfound = 0;
	currentwordreadpos = 0;
	justquotationmark = 0;
}

bool CQGLCSVTReader::ReadMetaData()
{
	if( !hasmetadata )
		return false;

	if( !utf8file )
		throw;

	colid = 0;

	currentwordreadpos = 0;
	while( utf8file->Read(a[currentwordreadpos]) )
	{
		isnewline = false;
		
		if( a[0]==_T('#') ) // comment
			SkipNext();
		else
		{
			isFirstLine = false;
			if( a[currentwordreadpos]==_T('"') )
			{
				if( justquotationmark==1 )
				{
					justquotationmark = 0;
					a[currentwordreadpos] = _T('"');
					currentwordreadpos++;
				}else
				{
					justquotationmark = 1;
				}
				if( quotationmarkfound==0 )
					quotationmarkfound = 1;
				else
					quotationmarkfound = 0;
				continue;
			}else if( a[currentwordreadpos]==_T(',') )
			{
				justquotationmark = 0;
				if( !quotationmarkfound )
				{
					a[currentwordreadpos]= _T('\0');
					currentwordreadpos = 0;
					AddColumnName(a, colid);

					colid++;

				}else
					currentwordreadpos++;
			} else if( a[currentwordreadpos]==_T('\r') )
			{
				justquotationmark = 0;
				continue;
			} else if( a[currentwordreadpos]==_T('\n') )
			{
				if( quotationmarkfound==0 )
				{
					a[currentwordreadpos]=_T('\0');
					isnewline = true;
					quotationmarkfound = 0;
					currentwordreadpos = 0;

					AddColumnName(a, colid);
					colid = 0;
					break;
				}else
				{
					currentwordreadpos++;
				}
			}else
			{
				justquotationmark = 0;
				currentwordreadpos++;
			}
		}
	}

	// resize column value to name columns
	m_columnvalue.resize(m_namecolumn.size());
	return true;
}

void CQGLCSVTReader::AddColumnName(const String &name, int columnno)
{
	String s = name;
	m_namecolumn[s.Trim()] = columnno;
}

bool CQGLCSVTReader::AnalyseCurrString(TCHAR* srcString, int len, TCHAR* composedString, int& outlen)
{
	bool quoted = false;
	int numQuote = 0;
	if( srcString[0]==_T('"') && srcString[len-1]==_T('"') )
		quoted = true;

	for( int i = 0; i<len; i++ )
	{
		if( srcString[i]==_T('"') )
			++numQuote;
	}
	if( quoted && numQuote%2==0 )
	{
		// finished string.  Replace all "" and outputr
		//int newpos = 0;
		//for( int i = 1; i<len-1; ++i )
		//{
		//	if( srcString[i]==_T('"') && srcString[i+1]==_T('"') && i<len-2)
		//	{
		//		composedString[newpos] = _T('"');
		//		++i;
		//	}else
		//	{
		//		composedString[newpos] = srcString[i];
		//	}
		//	++newpos;
		//}
		//outlen = newpos;
		//composedString[outlen] = _T('\0');
		// trim left and right space
		int st, ed;
		for (st = 1; st < len - 1; ++st)
			if (srcString[st] != _T(' '))
				break;
		for (ed = len - 2; ed > 0; --ed)
			if (srcString[ed] != _T(' '))
				break;
		if (st <= ed) {
			outlen = ed - st + 1;
			_tcsncpy(composedString, srcString + st, outlen);
		} else {
			outlen = 0;
		}
		composedString[outlen] = _T('\0');

		return true;
	}else if( !quoted && numQuote==0 )
	{
		outlen = len;
		_tcsncpy(composedString, srcString, len);
		composedString[outlen] = _T('\0');
		return true;
	}else
		return false;
}

bool CQGLCSVTReader::BufferCurrLine()
{
	isFirstLine = false;
	if( !utf8file )
		throw;

	colid = 0;

	currentwordreadpos = 0;
	while( utf8file->Read(a[currentwordreadpos]) )
	{
		isnewline = false;
		
		if( a[0]==_T('#') ) // comment
			SkipNext();
		else
		{
			if( a[currentwordreadpos]==_T(',') )
			{
				a[currentwordreadpos] = _T('\0');
				int outlen = 0;
				if( AnalyseCurrString(a, currentwordreadpos, b, outlen) )
				{
					currentwordreadpos = 0;
					SetFieldValue(colid, b);
					colid++;
				}else
				{
					a[currentwordreadpos] = _T(',');
					currentwordreadpos++;
				}
			} else if( a[currentwordreadpos]==_T('\r') )
			{
				continue;
			} else if( a[currentwordreadpos]==_T('\n') )
			{
				a[currentwordreadpos]=_T('\0');
				int outlen = 0;
				if( AnalyseCurrString(a, currentwordreadpos, b, outlen) )
				{
					isnewline = true;
					currentwordreadpos = 0;
					SetFieldValue(colid, b);
					colid = 0;
					return true;
				}else
				{
					a[currentwordreadpos]=_T('\n');
					currentwordreadpos++;
				}
			}else
			{
				currentwordreadpos++;
			}
		}
	}
	return true;
}

void CQGLCSVTReader::SetFieldValue(int column_id, TCHAR* value)
{
	if (column_id < (int) m_columnvalue.size())
		m_columnvalue[column_id] = value;
}

void CQGLCSVTReader::ResetFieldValue()
{
	for (MapColumnValue::iterator it = m_columnvalue.begin(); 
		it != m_columnvalue.end(); ++it)
	{
		*it = _T("");
	}
}

bool CQGLCSVTReader::IsLineEmpty()
{
	return (m_columnvalue.empty() || m_columnvalue.front().IsEmpty());
}