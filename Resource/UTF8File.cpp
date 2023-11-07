//-- Common
#include "Common.h"
//-- Self
#include "UTF8File.h"
//-- Library
#include <stdio.h>

UTF8File::UTF8File()
{
	f0			= NULL;
	m_Data		= NULL;
	m_DataSize	= 0;
	m_CurPos	= 0;
	HeaderWritten = false;
	buf			= SafeAllocate(char, 4);
	m_readmode	= UTF8_DATA_MODE_NULL;

	m_filepos	= 0;
	m_bufpos	= READBUF_SIZE_MAX;
	m_bufsize	= READBUF_SIZE_MAX;
	m_tbufpos	= 0;
	m_tbufsize	= 0;
}

UTF8File::~UTF8File()
{
	Close();
	SafeDeallocate(buf);
}

int UTF8File::Read(TCHAR &out)
{
	if( m_tbufpos >= m_tbufsize )
	{
		int counter		= -1;
		int tcounter	= 0;
		// read into buffer
		switch(m_readmode)
		{
		case UTF8_DATA_MODE_FILE_R:
		case UTF8_DATA_MODE_FILE_RW:
			{
				while( tcounter < READBUF_SIZE_MAX)
				{
					if( BufferedRead(&m_convbuf[++counter], 1, sizeof(char), f0) )
						if( (BYTE)m_convbuf[counter] >> 3 == 0x1E )
							if( BufferedRead(&m_convbuf[++counter], 1, sizeof(char), f0) )
								if( (BYTE)m_convbuf[counter] >> 6 == 0x02 )
									if( BufferedRead(&m_convbuf[++counter], 1, sizeof(char), f0) )
										if( (BYTE)m_convbuf[counter] >> 6 == 0x02 )
											if( BufferedRead(&m_convbuf[++counter], 1, sizeof(char), f0) )
												if( (BYTE)m_convbuf[counter] >> 6 == 0x02 )
												{
													++tcounter;
												}else
													return 0;
											else
												return 0;
										else
											return 0;
									else
										return 0;
								else
									return 0;
							else
								return 0;
						else if( (BYTE)m_convbuf[counter] >> 4 == 0x0E )
							if( BufferedRead(&m_convbuf[++counter], 1, sizeof(char), f0) )
								if( (BYTE)m_convbuf[counter] >> 6 == 0x02 )
									if( BufferedRead(&m_convbuf[++counter], 1, sizeof(char), f0) )
										if( (BYTE)m_convbuf[counter] >> 6 == 0x02 )
										{
											++tcounter;
										}else
											return 0;
									else
										return 0;
								else
									return 0;
							else
								return 0;
						else if( (BYTE)m_convbuf[counter] >> 5 == 0x06 )
							if( BufferedRead(&m_convbuf[++counter], 1, sizeof(char), f0) )
								if( (BYTE)m_convbuf[counter] >> 6 == 0x02 )
								{
									++tcounter;
								}else
									return 0;
							else
								return 0;
						else if( (BYTE)m_convbuf[counter] >> 7 == 0x00 )
						{
							++tcounter;
						}else
							return 0;
					else
						break;
				}
			}
			break;
		case UTF8_DATA_MODE_DATA_R:
			{
				while( tcounter < READBUF_SIZE_MAX && m_CurPos < m_DataSize )
				{
					m_convbuf[++counter] = m_Data[m_CurPos++];
					if( (BYTE)m_convbuf[counter] >> 3 == 0x1E && m_CurPos < m_DataSize )
					{
						m_convbuf[++counter] = m_Data[m_CurPos++];
						if( (BYTE)m_convbuf[counter] >> 6 == 0x02 && m_CurPos < m_DataSize )
						{
							m_convbuf[++counter] = m_Data[m_CurPos++];
							if( (BYTE)m_convbuf[counter] >> 6 == 0x02 && m_CurPos < m_DataSize )
							{
								m_convbuf[++counter] = m_Data[m_CurPos++];
								if( (BYTE)m_convbuf[counter] >> 6 == 0x02 )
								{
									++tcounter;
								}else
									return 0;
							}else
								return 0;
						}else
							return 0;
					}else if( (BYTE)m_convbuf[counter] >> 4 == 0x0E && m_CurPos < m_DataSize )
					{
						m_convbuf[++counter] = m_Data[m_CurPos++];
						if( (BYTE)m_convbuf[counter] >> 6 == 0x02 && m_CurPos < m_DataSize )
						{
							m_convbuf[++counter] = m_Data[m_CurPos++];
							if( (BYTE)m_convbuf[counter] >> 6 == 0x02 )
							{
								++tcounter;
							}else
								return 0;
						}else
							return 0;
					}else if( (BYTE)m_convbuf[counter] >> 5 == 0x06 && m_CurPos < m_DataSize )
					{
						m_convbuf[++counter] = m_Data[m_CurPos++];
						if( (BYTE)m_convbuf[counter] >> 6 == 0x02 )
						{
							++tcounter;
						}else
							return 0;
					}else if( (BYTE)m_convbuf[counter] >> 7 == 0x00 )
					{
						++tcounter;
					}else
						return 0;
				}
			}
			break;
		}
		counter++;
		// convert from UTF8(multibyte) to UTF16(TCHAR)
		MultiByteToWideChar(CP_UTF8, 0, m_convbuf, counter, m_tbuf, READBUF_SIZE_MAX);
		m_tbufpos	= 0;
		m_tbufsize	= tcounter;
	}
	// output one TCHAR
	if( m_tbufpos < m_tbufsize )
	{
		out = m_tbuf[m_tbufpos++];
		return 1;
	}else
	{
		out = _T('\0');
		return 0;
	}
}

int UTF8File::Write(TCHAR* src)
{
	if( m_readmode == UTF8_DATA_MODE_FILE_RW )
	{
		if( !HeaderWritten )
		{
			fprintf(f0, "%c%c%c", 0xEF, 0xBB, 0xBF); // UTF-8
			HeaderWritten = true;
		}
		int len = WideCharToMultiByte(CP_UTF8, 0, src, -1, NULL, 0, NULL, NULL);
		char* buffer = SafeAllocate(char, len);
		WideCharToMultiByte(CP_UTF8, 0, src, -1, buffer, len, NULL, NULL);
		fwrite( buffer, len-1, sizeof(char), f0 );
		SafeDeallocate(buffer);
		return 1;
	}
	return 0;
}

int UTF8File::Open(const char* filename, const char* mode)
{
	if( f0 )
		throw;
	else
	{
		if( strcmp(mode, "rt")==0 )
		{
			if( f0 = fopen( filename, "rb" ) )
			{
				char* temp = SafeAllocate(char, 4);
				temp[3] = '\0';
				if( BufferedRead(&temp[0], 1, sizeof(char), f0) && 
					BufferedRead(&temp[1], 1, sizeof(char), f0) && 
					BufferedRead(&temp[2], 1, sizeof(char), f0) )
				{
					char* abc = SafeAllocate(char, 4);
					abc[0] = (char)0xef;
					abc[1] = (char)0xbb;
					abc[2] = (char)0xbf;
					abc[3] = '\0';
					if( strcmp(temp, abc)==0 )
					{
						m_readmode = UTF8_DATA_MODE_FILE_R;
						SafeDeallocate(abc);
						SafeDeallocate(temp);
						return 1;
					}
					SafeDeallocate(abc);
				}
				SafeDeallocate(temp);
			}
		}else if( strcmp(mode, "wt")==0 )
		{
			if( f0 = fopen( filename, "wb" ) )
			{
				m_readmode = UTF8_DATA_MODE_FILE_R;
				return 1;
			}
		}
	}
	return 0;
}

int UTF8File::Open(const TCHAR* filename, const char* mode)
{
	if( f0 )
		throw;
	else
	{
		if( strcmp(mode, "rt")==0 )
		{
			int len = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
			char* ansifilename = SafeAllocate(char, len+1);
			WideCharToMultiByte(CP_ACP, 0, filename, -1, ansifilename, len, NULL, NULL);
			ansifilename[len] = 0;

			if( ansifilename[0]!=0 && (f0 = fopen( ansifilename, "rb" )) )
			{
				char* temp = SafeAllocate(char, 4);
				temp[3] = '\0';
				if( BufferedRead(&temp[0], 1, sizeof(char), f0) && 
					BufferedRead(&temp[1], 1, sizeof(char), f0) && 
					BufferedRead(&temp[2], 1, sizeof(char), f0) )
				{
					char* abc = SafeAllocate(char, 4);
					abc[0] = (char)0xef;
					abc[1] = (char)0xbb;
					abc[2] = (char)0xbf;
					abc[3] = '\0';
					if( strcmp(temp, abc)==0 )
					{
						m_readmode = UTF8_DATA_MODE_FILE_R;
						SafeDeallocate(abc);
						SafeDeallocate(temp);
						SafeDeallocate(ansifilename);
						return 1;
					}
					SafeDeallocate(abc);
				}
				SafeDeallocate(temp);
			}
			SafeDeallocate(ansifilename);
		}else if( strcmp(mode, "wt")==0 )
		{
			int len = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
			char* ansifilename = SafeAllocate(char, len+1);
			WideCharToMultiByte(CP_ACP, 0, filename, -1, ansifilename, len, NULL, NULL);
			ansifilename[len] = 0;

			if( f0 = fopen( ansifilename, "wb" ) )
			{
				m_readmode = UTF8_DATA_MODE_FILE_RW;
				SafeDeallocate(ansifilename);
				return 1;
			}
			SafeDeallocate(ansifilename);
		}
	}
	return 0;
}

int UTF8File::Close()
{
	if( f0 )
	{
		fflush(f0);
		int succeed = fclose(f0);
		f0 = NULL;
		m_readmode = UTF8_DATA_MODE_NULL;
		return succeed;
	}else
	{
		return 0;
	}
}

int UTF8File::Rewind()
{
	switch(m_readmode)
	{
	case UTF8_DATA_MODE_FILE_R:
	case UTF8_DATA_MODE_FILE_RW:
		{
			RewindBuffer();
			char* temp = SafeAllocate(char, 3);
			if( BufferedRead(&temp[0], 1, sizeof(char), f0) && 
				BufferedRead(&temp[1], 1, sizeof(char), f0) && 
				BufferedRead(&temp[2], 1, sizeof(char), f0) )
			{
				SafeDeallocate(temp);
				return true;
			}
			SafeDeallocate(temp);
			return false;
		}
		break;
	case UTF8_DATA_MODE_DATA_R:
		{
			m_CurPos = 3;
			return 1;
		}
		break;
	}
	return (0);
}

int UTF8File::OpenBuf(const BYTE* pData, unsigned int pDataSize)
{
	m_Data = const_cast<BYTE*>(pData);
	m_DataSize = pDataSize;

	if( m_Data[0]==0xef && m_Data[1]==0xbb && m_Data[2]==0xbf )
	{
		m_CurPos = 3;
		m_readmode = UTF8_DATA_MODE_DATA_R;
		return 1;
	}
	return 0;
}

void UTF8File::RewindBuffer()
{
	m_bufpos	= READBUF_SIZE_MAX;
	m_bufsize	= READBUF_SIZE_MAX;
	m_filepos	= 0;
	rewind(f0);
}

size_t UTF8File::BufferedRead(char* buf, unsigned int size, unsigned int count, FILE* fstream)
{
	size_t totalcopied = 0;
	size_t remained = size*count;
	size_t cur_read;
	while( remained > 0 )
	{
		if( m_bufpos>=m_bufsize )
		{
			if( fstream==NULL )	return 0;
			if( feof(fstream) )	return 0;

			size_t readCount = fread(m_readbuf, 1, READBUF_SIZE_MAX, fstream);
			m_filepos	+= readCount;
			m_bufpos	= 0;
			m_bufsize	= readCount;
			
			if( ferror(fstream) )	return 0;
		}
		cur_read = remained;
		if( cur_read > m_bufsize-m_bufpos )
			cur_read = m_bufsize-m_bufpos;
		memcpy(buf, &m_readbuf[m_bufpos], cur_read);
		m_bufpos += cur_read;
		totalcopied	+= cur_read;
		remained -= cur_read; 
	}
	return totalcopied;
}
