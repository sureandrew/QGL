//-- Common
#include "Common.h"
//-- Self
#include "Resource/QuestionManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/BuffReader.h>
#include "Resource/ResQuestion.h"
#include "Resource/QGLCSVTReader.h"

#define ITEM_DATA_VER		1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

////////////// Item Manager //////////////////////////////////

CQuestionManager::CQuestionManager()
{
	m_questions.clear();
}

CQuestionManager::~CQuestionManager()
{
	Release();
}

VOID CQuestionManager::Release()
{
	for (QuestionMap::iterator it = m_questions.begin(); it != m_questions.end(); it++)
		SafeDeleteObject(it->second);
	m_questions.clear();
}

const QuestionData* CQuestionManager::GetQuestionData(UInt32 question_id)
{
	QuestionMap::iterator it = m_questions.find(question_id);
	if( it!=m_questions.end() )
		return it->second;
	else
		return NULL;
}

BOOL CQuestionManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(filename, true) )
		openSrcSucceeded = TRUE;
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		QuestionMap::iterator it;
		QuestionData* pQuestion = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pQuestion = SafeCreateObject(QuestionData);
				reader.Read(_T("question_id"),		pQuestion->question_id);
				reader.Read(_T("answer"),	pQuestion->answer);
				reader.ReadString(_T("description"),	pQuestion->description, 128);
				reader.ReadString(_T("option1"),	pQuestion->option1, 64);
				reader.ReadString(_T("option2"),	pQuestion->option2, 64);
				reader.ReadString(_T("option3"),	pQuestion->option3, 64);
				reader.ReadString(_T("option4"),	pQuestion->option4, 64);
				
				it = m_questions.find(pQuestion->question_id);
				if (it != m_questions.end())
				{
					TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("QuestionManager LoadCSV: question_id %d already loaded"), pQuestion->question_id);
					SafeDeleteObject(it->second);
					m_questions.erase(it);
				}

				m_questions.insert(std::make_pair(pQuestion->question_id, pQuestion));
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %i question(s) read"), (UInt)m_questions.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: QuestionManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CQuestionManager::LoadDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			file.Read( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numItem = 0;
			file.Read( (BYTE*)&numItem, sizeof(UInt32) );

			QuestionData* pQuestion;
			for (UInt32 i = 0; i < numItem; i++)
			{
				pQuestion = SafeCreateObject(QuestionData);
				file.Read( (BYTE*)pQuestion, sizeof(QuestionData) );
				m_questions[pQuestion->question_id] = pQuestion;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CItemManager::LoadDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();

	return TRUE;
}

BOOL CQuestionManager::SaveDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_WRITE);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			header.version = ITEM_DATA_VER;
			file.Write( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numItem = (UInt32)m_questions.size();
			file.Write( (BYTE*)&numItem, sizeof(UInt32) );

			for (QuestionMap::iterator it = m_questions.begin(); it != m_questions.end(); it++)
			{
				file.Write( (BYTE*)it->second, sizeof(QuestionData) );
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CQuestionManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}
