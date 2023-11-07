#pragma once
#ifndef _REUBEN_PLATFORM_BUFF_READER_H_
#define _REUBEN_PLATFORM_BUFF_READER_H_

#include "File.h"

namespace Reuben { namespace Platform {

	class BuffReader : public File
	{
	public:
		
	private:
		typedef File Parent;
		Reuben::System::Byte* m_pBuffer;
		Count m_cursor;
		Count m_size;

	public:

		BuffReader(VOID);
		BuffReader(PCSTRING pFileName, CONST Bit32 biMode, CONST Boolean bAsynchronous = BOOLEAN_FALSE, CONST Bit32 biAttributes = ATTRIBUTE_NORMAL);
		virtual ~BuffReader(VOID);

		VOID Close(VOID);
		CONST Count Read(Reuben::System::Byte * pBuffer, CONST Count cSize, PASYNC_CALLBACK pAsyncCallback = NULL, VOID * pParam = NULL);
		CONST UInt64 SetPosition(CONST UInt64 uOffset, MoveEnum eMove);
	};

}; }; // Reuben::Platform

#endif // _REUBEN_PLATFORM_BUFF_READER_H_
