#pragma once
#ifndef _RESOURCE_QUESTION_H_
#define _RESOURCE_QUESTION_H_

#include <Reuben\System\Reuben.h>

struct QuestionData
{
	UInt32	question_id;
	UInt8	answer;			//index
	TCHAR	description[129];
	TCHAR	option1[65];
	TCHAR	option2[65];
	TCHAR	option3[65];
	TCHAR	option4[65];

	QuestionData()
		: question_id(C_UINT32(0))
		, answer(C_UINT8(0))
	{
		ZeroMemory(description, 129*sizeof(TCHAR));
		ZeroMemory(option1, 65*sizeof(TCHAR));
		ZeroMemory(option2, 65*sizeof(TCHAR));
		ZeroMemory(option3, 65*sizeof(TCHAR));
		ZeroMemory(option4, 65*sizeof(TCHAR));
	}

	~QuestionData()	{}
};

#endif // _RESOURCE_QUESTION_H_
