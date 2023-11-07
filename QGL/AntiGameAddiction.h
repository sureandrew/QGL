#pragma once
#ifndef _ANTIADDICTION_H_
#define _ANTIADDICTION_H_

class CAntiGameAddiction
{
public:
	enum GHealthStateEnum
	{
		GHEALTH_GOOD = 0,
		GHEALTH_AVERAGE = 1,
		GHEALTH_POOR = 2,
		GHEALTH_REST = 3,
	};
	CAntiGameAddiction(void);
	~CAntiGameAddiction(void);

	void SetInitialTime(UInt32 time);
	void UpdateGameHealthStatus(UInt32 curTime);

private:
	UInt32 m_timeZero;
	UInt16 m_gameHealthStatus;
	UInt32 m_timeWarning;



};

#endif

