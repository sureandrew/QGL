#pragma once
#ifndef _CUTF8FILE_H
#define _CUTF8FILE_H

#include <stdio.h>
#include <tchar.h>

#include "Reuben\System\Reuben.h"
#define READBUF_SIZE_MAX	4096
#define READBUF_SIZE_MAX_X4	16384

enum UTF8_DATA_MODE
{
	UTF8_DATA_MODE_NULL = 0,
	UTF8_DATA_MODE_FILE_R,
	UTF8_DATA_MODE_FILE_RW,
	UTF8_DATA_MODE_DATA_R
};

class UTF8File
{
public:
	UTF8File();
	~UTF8File();

	int Read(TCHAR &out);
	int Write(TCHAR* src);
	
	int Open(const TCHAR* filename, const char* mode);
	int Open(const char* filename, const char* mode);
	int OpenBuf(const BYTE* pData, unsigned int pDataSize); // read only
	int Close();

	int Rewind();

private:
	size_t BufferedRead(char* buf, unsigned int size, unsigned int count, FILE* fstream);
	void RewindBuffer();

private:
	int m_readmode; // 0: not initialized; 1: read-only from file; 2: read-write of file; 3: read-only from data stream

	// File mode
	FILE* f0;
	bool HeaderWritten;
	char* buf;

	// buffered stream
	char m_readbuf[READBUF_SIZE_MAX];
	size_t m_filepos;	// current file position	(0 ~ file end)
	size_t m_bufpos;	// current buffer position	(0 ~ READBUF_SIZE_MAX)
	size_t m_bufsize;	// current buffer position	(0 ~ READBUF_SIZE_MAX)
	
	char m_convbuf[READBUF_SIZE_MAX_X4];
	TCHAR m_tbuf[READBUF_SIZE_MAX];
	size_t m_tbufpos;
	size_t m_tbufsize;

	// Data stream mode
	BYTE* m_Data; // for data stream only
	unsigned int m_DataSize;
	unsigned int m_CurPos;

};

#endif
