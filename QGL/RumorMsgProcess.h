#pragma once
#ifndef _RUMORMSG_H_
#define _RUMORMSG_H_

#include "QGL.h"

#define MAX_RUMOR_MSG	3
struct RumorMsgData
{
	UInt32	m_StartTime;
	BYTE	m_AlphaValue;
	String	Msg;

	RumorMsgData()
	{
		m_StartTime = 0;
		m_AlphaValue = 0;
	}
};

class CRumorMsgProcess
{
public:
	CRumorMsgProcess();
	virtual ~CRumorMsgProcess();

	RumorMsgData	m_MsgDataLst[MAX_RUMOR_MSG];

	VOID ShowRumorMessage(String Msg);
	VOID Update(UInt32 CurrTime);

	BYTE	m_CurrCount;

};




#endif 