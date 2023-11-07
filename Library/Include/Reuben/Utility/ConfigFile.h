#pragma once
#ifndef _REUBEN_UTILITY_CONFIG_FILE_H_
#define _REUBEN_UTILITY_CONFIG_FILE_H_

#define MAX_LINE_STRING_SIZE 8192

namespace Reuben { namespace Utility {

	class ConfigFile
	{
	public:

		INLINE ConfigFile(PCSTRING pFileName = STRING_EMPTY);
		INLINE ~ConfigFile();

		INLINE VOID SetFileName(PCSTRING pFileName);
		INLINE CONST String & GetFileName(VOID) CONST;

		INLINE CONST Int GetInt(PCSTRING pSection, PCSTRING pKey, CONST Int nDefault) CONST;
		INLINE String GetString(PCSTRING pSection, PCSTRING pKey, PCSTRING pDefault) CONST;
		INLINE CONST Boolean GetBinary(PCSTRING pSection, PCSTRING pKey, PVOID pData, Size size) CONST;

		INLINE CONST Boolean SetInt(PCSTRING pSection, PCSTRING pKey, CONST Int nValue);
		INLINE CONST Boolean SetString(PCSTRING pSection, PCSTRING pKey, PCSTRING pValue);
		INLINE CONST Boolean SetBinary(PCSTRING pSection, PCSTRING pKey, CONST PVOID pData, Size size);
		
	private:

		String m_strFileName;

	};

	INLINE ConfigFile::ConfigFile(
		PCSTRING pFileName // STRING_EMPTY
		)
		: m_strFileName(GetFullFileName(pFileName))
	{}

	INLINE ConfigFile::~ConfigFile()
	{
		m_strFileName.Empty();
	}

	INLINE VOID ConfigFile::SetFileName(PCSTRING pFileName)
	{
		DEBUG_CHECK(pFileName != NULL);
		m_strFileName = GetFullFileName(pFileName);
		DEBUG_ENSURE(m_strFileName.GetLength());
	}

	INLINE CONST String& ConfigFile::GetFileName(VOID) CONST
	{
		return (m_strFileName);
	}

	INLINE CONST Int ConfigFile::GetInt(PCSTRING pSection, PCSTRING pKey, CONST Int nDefault) CONST
	{
		DEBUG_CHECK(m_strFileName.GetLength());
		return ((Int)::GetPrivateProfileInt(pSection, pKey, nDefault, m_strFileName.c_str()));
	}

	INLINE String ConfigFile::GetString(PCSTRING pSection, PCSTRING pKey, PCSTRING pDefault) CONST
	{
		DEBUG_CHECK(m_strFileName.GetLength());
		String strValue;
		::GetPrivateProfileString(pSection, pKey, pDefault, strValue.GetBuffer(MAX_LINE_STRING_SIZE), MAX_LINE_STRING_SIZE, m_strFileName.c_str());
		int pos = strValue.Find(_T(';'));
		if (pos >= 0)
			strValue = strValue.substr(0, pos);
		strValue.Trim();
		strValue.ReleaseBuffer();
		return (strValue);
	}

	INLINE CONST Boolean ConfigFile::GetBinary(PCSTRING pSection, PCSTRING pKey, PVOID pData, Size size) CONST
	{
		DEBUG_CHECK(m_strFileName.GetLength());
		return (::GetPrivateProfileStruct(pSection, pKey, pData, (UINT)size, m_strFileName.c_str()) == TRUE);
	}

	INLINE CONST Boolean ConfigFile::SetInt(PCSTRING pSection, PCSTRING pKey, CONST Int nValue)
	{
		DEBUG_CHECK(m_strFileName.GetLength());
		return (::WritePrivateProfileString(pSection, pKey, IntToString(nValue).c_str(), m_strFileName.c_str()) == TRUE);
	}

	INLINE CONST Boolean ConfigFile::SetString(PCSTRING pSection, PCSTRING pKey, PCSTRING pValue)
	{
		DEBUG_CHECK(m_strFileName.GetLength());
		return (::WritePrivateProfileString(pSection, pKey, pValue, m_strFileName.c_str()) == TRUE);
	}

	INLINE CONST Boolean ConfigFile::SetBinary(PCSTRING pSection, PCSTRING pKey, CONST PVOID pData, Size size)
	{
		DEBUG_CHECK(m_strFileName.GetLength());
		return (::WritePrivateProfileStruct(pSection, pKey, pData, (UINT)size, m_strFileName.c_str()) == TRUE);
	}

};}; // Reuben::Utility

#endif // _REUBEN_UTILITY_CONFIG_FILE_H_

