#pragma once
#ifndef _ANTIADDICTION_H_
#define _ANTIADDICTION_H_

class CCharacter;
struct AntiAddictInfo;
#include <oledb.h>

class CAntiGameAddiction
{
public:

	CAntiGameAddiction(void);
	~CAntiGameAddiction(void);

	void LoginInitialization(LPCPEER pPeer, RPCContext &context, CCharacter *pchar, const AntiAddictInfo addictInfo);
	void RaiseAllAntiAddictionEvent(CCharacter *pchar);
	void EventHandler(LPCPEER pPeer, RPCContext &context, CCharacter *pchar, UInt32 event);
	void SetAddictData(AntiAddictInfo addictInfo);
	void UpdateAddictInfoToDB(CCharacter *pchar, UInt8 &addition);
	void SetCurrentAddictionHealth();
	UInt32 FindNextResetTime(UInt32 fromTime);	
	void TimeConvert(const time_t &fromTime, DBTIMESTAMP &toTime);	

	UInt32 m_onlineTime;
	UInt32 m_offlineTime;
	UInt32 m_loginTime;
	UInt8 m_healthstatus;
	float m_playerInterest;
	Boolean m_startGame;
	Boolean m_bStopPlay;
	Boolean m_bSentShutDownMsg;
	Boolean m_needAntiAddict;
	Boolean m_bClearOnlineTime;

		

};

#endif

