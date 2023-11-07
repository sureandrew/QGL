//-- Common
#include "Common.h"
//-- Self
#include "FileManager.h"
//-- Library
#include <Reuben/Platform/BuffReader.h>

CFileManager::CFileManager()
{
	m_fileDataList.clear();
	m_fileSizeList.clear();
	m_reservedSizeList.clear();
}

CFileManager::~CFileManager()
{
	for( std::map<String, BYTE*>::iterator itr_filedatalist = m_fileDataList.begin(); 
		itr_filedatalist!=m_fileDataList.end(); 
		itr_filedatalist++ )
	{
		SafeDeallocate(itr_filedatalist->second);
	}
	m_fileDataList.clear();
	m_fileSizeList.clear();
	m_reservedSizeList.clear();
}

BYTE* CFileManager::Load(String filename, UInt64 & size)
{
	std::map<String, BYTE*>::iterator itr = m_fileDataList.find(filename); 
	if( itr!=m_fileDataList.end() )
	{
		std::map<String, UInt64>::iterator itr2 = m_fileSizeList.find(filename); 
		if( itr2!=m_fileSizeList.end() )
		{
			std::map<String, UInt32>::iterator itr3 = m_reservedSizeList.find(filename); 
			if( itr3!=m_reservedSizeList.end() )
			{
				size = itr2->second;
				itr3->second++;
				return itr->second;
			}
		}
	}else
	{
		Reuben::Platform::File * file = SafeCreateObject(Reuben::Platform::File);
		try
		{
			file->Open(filename.c_str(), Reuben::Platform::File::MODE_READ_EXISTING);
			if (file->IsOpened())
			{
				UInt64 filesize = file->GetLength();
				BYTE* filedata = SafeAllocate(BYTE, (Reuben::System::Size)filesize);
				file->Read(filedata, sizeof(BYTE) * (Reuben::System::Count)filesize);
				file->Close();

				size = filesize;

				m_fileDataList.insert(std::make_pair(filename, filedata));
				m_fileSizeList.insert(std::make_pair(filename, filesize));
				m_reservedSizeList.insert(std::make_pair(filename, 1));
				SafeDeleteObject(file);
				return filedata;
			}
		}
		catch(Reuben::Platform::FileException Er)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CFileManager::Load, open error, %s"), filename.c_str());
			return 0;
		}
		SafeDeleteObject(file);

		return NULL;
	}
	return NULL;
}

BOOL CFileManager::Free(String filename)
{
	std::map<String, BYTE*>::iterator itr = m_fileDataList.find(filename); 
	if( itr!=m_fileDataList.end() )
	{
		std::map<String, UInt64>::iterator itr2 = m_fileSizeList.find(filename); 
		if( itr2!=m_fileSizeList.end() )
		{
			std::map<String, UInt32>::iterator itr3 = m_reservedSizeList.find(filename); 
			if( itr3!=m_reservedSizeList.end() )
			{
				itr3->second--;
				if( itr3->second<=0 )
				{
					SafeDeallocate(itr->second);
					m_fileDataList.erase(filename);
					m_fileSizeList.erase(filename);
					m_reservedSizeList.erase(filename);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}
