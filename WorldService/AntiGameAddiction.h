#pragma once
#ifndef _ANTIADDICTION_H_
#define _ANTIADDICTION_H_

class CAccount;

class CAntiGameAddiction
{
public:

	CAntiGameAddiction(void);
	~CAntiGameAddiction(void);

	void LoginInitialization(CAccount *acct, UInt32 onlinTm, UInt32 oflinTm, UInt32 dislinTm);
	void AddictionSwitchOff(CAccount *acct);	
	void EventHandler(LPCPEER pPeer, RPCContext &context, CAccount *acct, UInt32 event);
	Boolean AntiAddictActivate();
	

	UInt32 m_onlineTime;
	UInt32 m_offlineTime;
	UInt32 m_loginTime;
	float m_playerInterest;
	Boolean m_startGame;	
		

};

#endif

