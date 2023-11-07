#pragma once
#ifndef _REUBEN_PLATFORM_MINI_VERSION_H_
#define _REUBEN_PLATFORM_MINI_VERSION_H_

namespace Reuben { namespace Platform {

	class MiniVersion
	{
	private:

		Byte* m_pData;
		UInt32 m_uHandle;
		UInt16 m_uFileVersion[4];
		UInt16 m_uProductVersion[4];
		UInt32 m_uFileFlags;
		UInt32 m_uFileOS;
		UInt32 m_uFileType;
		UInt32 m_uFileSubType;

		String m_strPath;
		String m_strCompanyName;
		String m_strProductName;
		String m_strFileDescription;

	public:

		MiniVersion(PCSTRING pPath = NULL);

		Boolean Create(VOID);
		VOID Dispose(VOID);

		UInt16* GetFileVersion(VOID);
		UInt16* GetProductVersion(VOID);
		CONST UInt32 GetFileFlags(VOID);
		CONST UInt32 GetFileOS(VOID);
		CONST UInt32 GetFileType(VOID);
		CONST UInt32 GetFileSubType(VOID);

		CONST String& GetCompanyName(VOID);
		CONST String& GetProductName(VOID);
		CONST String& GetFileDescription(VOID);

	private:
		
		Boolean GetFixedInfo(VS_FIXEDFILEINFO& info);
		Boolean GetStringInfo(PCSTRING pKey, String& strValue);

	};

}; }; // Reuben::Platform

#endif // _REUBEN_PLATFORM_MINI_VERSION_H_
