#pragma once
#ifndef _REUBEN_PLATFORM_FILE_H_
#define _REUBEN_PLATFORM_FILE_H_

#include <shlwapi.h>
#pragma comment(lib, "shlwapi")

namespace Reuben { namespace Platform {

	class FileException : public Exception
	{
	private:

		String m_strFileName;

	public:

		FileException(ErrorCode ec, PCSTRING pSourceLine, PCSTRING pFileName)
			: Exception(ec, pSourceLine)
			, m_strFileName(pFileName)
		{
		}

		FileException(ErrorCode ec, PCSTRING pSourceLine, PCSTRING pFileName, PCSTRING pMessage)
			: Exception(ec, pSourceLine, pMessage)
			, m_strFileName(pFileName)
		{
		}
		
		INLINE virtual CONST String GetErrorMessage(VOID);
		INLINE CONST String & GetFileName(VOID) CONST;
	};

	INLINE CONST String FileException::GetErrorMessage(VOID)
	{
		String strErrorMessage;
		strErrorMessage.Format(_T("%s [%s 0x%08X] File Name = {%s} %s"), m_strSourceLine.c_str(), NULL2NONE(GetErrorCodeString(m_ec)), m_ec, m_strFileName.c_str(), m_strMessage.c_str());
		return (strErrorMessage);
	}

	INLINE CONST String & FileException::GetFileName(VOID) CONST
	{
		return(m_strFileName);
	}

	class File
	{
	public:
		
		enum ModeEnum
		{
			MODE_ACCESS_READ	= 0x01,
			MODE_ACCESS_WRITE	= 0x02,
			MODE_ACCESS_EXECUTE	= 0x04,
			MODE_ACCESS_ALL		= MODE_ACCESS_READ | MODE_ACCESS_WRITE | MODE_ACCESS_EXECUTE,

			MODE_SHARE_READ		= 0x08,
			MODE_SHARE_WRITE	= 0x10,
			MODE_SHARE_DELETE	= 0x20,
			MODE_SHARE_ALL		= MODE_SHARE_READ | MODE_SHARE_WRITE | MODE_SHARE_DELETE,

			MODE_OPEN_NEW		= 0x40,
			MODE_OPEN_ALWAYS	= 0x80,
			MODE_OPEN_EXISTING	= 0x100,
			
			MODE_READ		= MODE_ACCESS_READ | MODE_SHARE_READ | MODE_OPEN_ALWAYS,
			MODE_WRITE		= MODE_ACCESS_WRITE | MODE_SHARE_READ | MODE_OPEN_ALWAYS,
			MODE_READ_WRITE	= MODE_READ | MODE_WRITE,

			MODE_READ_EXISTING = MODE_ACCESS_READ | MODE_SHARE_READ | MODE_OPEN_EXISTING,
			MODE_WRITE_EXISTING = MODE_ACCESS_WRITE | MODE_SHARE_READ | MODE_OPEN_EXISTING,
			MODE_READ_WRITE_EXISTING	= MODE_READ_EXISTING | MODE_WRITE_EXISTING
		};

		enum MoveEnum
		{
			MOVE_BEGIN,
			MOVE_CURRENT,
			MOVE_END
		};

		enum AttributeEnum
		{
			ATTRIBUTE_NORMAL	= 0x01,
			ATTRIBUTE_READONLY	= 0x02,
			ATTRIBUTE_HIDDEN	= 0x04,
		};

		enum OperationEnum
		{
			OPERATION_READ,
			OPERATION_WRITE
		};

		typedef VOID (__STDCALL * PASYNC_CALLBACK)(CONST OperationEnum eOp, CONST Count cBytesNeed, CONST Count cBytesTransfered, VOID * pParam);

	private:

		String m_strFileName;
		Bit32 m_biMode;
		Bit32 m_biAttributes;
		UInt64 m_uPosition;
		Boolean m_bAsynchronous;

		Boolean m_bOpened;
		HANDLE m_hFile;

	public:

		File(VOID);
		File(PCSTRING pFileName, CONST Bit32 biMode, CONST Boolean bAsynchronous = BOOLEAN_FALSE, CONST Bit32 biAttributes = ATTRIBUTE_NORMAL);
		virtual ~File(VOID);

		VOID Open(PCSTRING pFileName, CONST Bit32 biMode, CONST Boolean bAsynchronous = BOOLEAN_FALSE, CONST Bit32 biAttributes = ATTRIBUTE_NORMAL);
		VOID Close(VOID);

		CONST Count Read(Reuben::System::Byte * pBuffer, CONST Count cSize, PASYNC_CALLBACK pAsyncCallback = NULL, VOID * pParam = NULL);
		CONST Count Write(CONST Reuben::System::Byte * pBuffer, CONST Count cSize, PASYNC_CALLBACK pAsyncCallback = NULL, VOID * pParam = NULL);
		VOID Flush(VOID);

		CONST UInt64 SetPosition(CONST UInt64 uOffset, MoveEnum eMove);

		VOID SetLength(CONST UInt64 uSize);
		CONST UInt64 GetLength(VOID);

		INLINE CONST String & GetFileName(VOID) CONST;
		INLINE CONST String GetFilePath(VOID) CONST;
		INLINE CONST String GetFileTitle(VOID) CONST;
		INLINE CONST Bit32 GetMode(VOID) CONST;
		INLINE CONST UInt64 GetPosition(VOID) CONST;
		INLINE CONST Boolean IsOpened(VOID) CONST;
		INLINE CONST Boolean IsAsynchronous(VOID) CONST;

		static INLINE CONST Boolean IsExists(PCSTRING pFileName);
		static INLINE CONST Boolean Delete(PCSTRING pFileName);
		static INLINE CONST Boolean Rename(PCSTRING pFileName, PCSTRING pNewFileName);
		static INLINE CONST Boolean SetAttributes(PCSTRING pFileName, CONST Bit32 biAttributes);

	private:

		static VOID __STDCALL CompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

	};

	INLINE CONST String & File::GetFileName(VOID) CONST
	{
		return (m_strFileName);
	}

	INLINE CONST String File::GetFilePath(VOID) CONST
	{
		String strPath(m_strFileName);
		Int nFound = strPath.ReverseFind(PATH_SLASH_CHAR);
		strPath.Delete(nFound, strPath.GetLength() - nFound);
		return (strPath);
	}

	INLINE CONST String File::GetFileTitle(VOID) CONST
	{
		String strTitle(m_strFileName);
		Int nFound = strTitle.ReverseFind(PATH_SLASH_CHAR);
		strTitle.Delete(0, nFound + 1);
		return (strTitle);
	}

	INLINE CONST Bit32 File::GetMode(VOID) CONST
	{
		return (m_biMode);
	}

	INLINE CONST UInt64 File::GetPosition(VOID) CONST
	{
		return (m_uPosition);
	}

	INLINE CONST Boolean File::IsOpened(VOID) CONST
	{
		return (m_bOpened);
	}

	INLINE CONST Boolean File::IsAsynchronous(VOID) CONST
	{
		return (m_bAsynchronous);
	}

	INLINE CONST Boolean File::IsExists
		(
		PCSTRING pFileName
		)
	{
		return (PathFileExists(pFileName) == TRUE);
	}

	INLINE CONST Boolean File::Delete
		(
		PCSTRING pFileName
		)
	{
		return (DeleteFile(pFileName) == TRUE);
	}

	INLINE CONST Boolean File::Rename
		(
		PCSTRING pFileName,
		PCSTRING pNewFileName
		)
	{
		return (MoveFile(pFileName, pNewFileName) == TRUE);
	}

	INLINE CONST Boolean File::SetAttributes
		(
		PCSTRING pFileName,
		CONST Bit32 biAttributes
		)
	{
		DWORD dwAttributes = C_BIT32(0);
		if (biAttributes & ATTRIBUTE_NORMAL)
			dwAttributes |= FILE_ATTRIBUTE_NORMAL;
		if (biAttributes & ATTRIBUTE_READONLY)
			dwAttributes |= FILE_ATTRIBUTE_READONLY;
		if (biAttributes & ATTRIBUTE_HIDDEN)
			dwAttributes |= FILE_ATTRIBUTE_HIDDEN;

		return (SetFileAttributes(pFileName, dwAttributes) == TRUE);
	}

}; }; // Reuben::Platform

#endif // _REUBEN_PLATFORM_FILE_H_
