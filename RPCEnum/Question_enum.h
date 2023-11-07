#pragma once
#ifndef RPC_QUESTION_ENUM
#define RPC_QUESTION_ENUM

enum RPC_Question_Enum
{
	Question_CLASSID	= 56000,
	RPC_Question_ENUM_ST	= 56000,
	RPC_Question_UP_STATE	= 56693,
	RPC_Question_REQ_STATE	= 56202,
	RPC_Question_CB_NextQuestion	= 56861,
	RPC_Question_Anwser	= 56515,
	RPC_Question_CB_AnswerResult	= 56437,
	RPC_Question_CB_StartCountDown	= 56220,
	RPC_Question_ENUM_ED	= 56999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Question()
{
	RPC_ENUM_MAP[RPC_Question_UP_STATE] = 1031225953;
	RPC_ENUM_MAP[RPC_Question_REQ_STATE] = 2060584342;
	RPC_ENUM_MAP[RPC_Question_CB_NextQuestion] = 1688412626;
	RPC_ENUM_MAP[RPC_Question_Anwser] = 482361586;
	RPC_ENUM_MAP[RPC_Question_CB_AnswerResult] = 529400276;
	RPC_ENUM_MAP[RPC_Question_CB_StartCountDown] = 922421266;
	return 0;
}
static int _addrpc_Question = _AddRpcEnum_Question();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Question()
{
	RPC_NONBLOCK_MAP[RPC_Question_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_Question_CB_NextQuestion] = true;
	RPC_NONBLOCK_MAP[RPC_Question_CB_AnswerResult] = true;
	RPC_NONBLOCK_MAP[RPC_Question_CB_StartCountDown] = true;
	return 0;
}
static int _nonblock_Question = _NonblockMap_Question();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_QUESTION_ENUM
