#pragma once
#ifndef _QUESTION_MANAGER_H_
#define _QUESTION_MANAGER_H_

struct QuestionData;

class CQuestionManager
{
public:
	CQuestionManager();
	~CQuestionManager();

	const QuestionData* GetQuestionData(UInt32 question_id);
	size_t GetSize(){return m_questions.size();};

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
//	BOOL SaveCSV(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	VOID Release();

private:
	typedef StlMap<UInt32, QuestionData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, QuestionData*> > > QuestionMap;

	QuestionMap m_questions;
};

#endif // _QUESTION_MANAGER_H_
