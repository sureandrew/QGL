#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

//define for detail db structure
#define LOG_FOR_DETAIL

#include <oledb.h>

class CDboSource;
class CdboLogAccount;
class CdboLogCharacter;
class CdboLogItem;
class CdboLogQuest;
class CdboLogPlayerCount;
class CdboLogGiftBox;
class CdboLogChargeShop;
class CdboLogExchange;
class CdboLogYuanBao;
class CdboLogCheat;
class CdboLogCharExp;
class CdboLogMoney;
class CdboLogSMoney;
class CdboLogMix;
class CdboLogGoodness;
class CdboLogHack;
class CdboLogSellYuanBao;
class CdboLogHonor;
class CdboLogPartner;
class CdboLogManor;
class CdboLogGenAcc;
class CdboLogGuildDonate;

struct DatabaseTables
{
	CdboLogAccount*		pLogAccount;
	CdboLogCharacter*	pLogCharacter;
	CdboLogItem*		pLogItem;
	CdboLogQuest*		pLogQuest;
	CdboLogPlayerCount*	pLogPlayerCount;
	CdboLogGiftBox*		pLogGiftBox;
	CdboLogChargeShop*	pLogChargeShop;
	CdboLogExchange*	pLogExchange;
	CdboLogYuanBao*		pLogYuanBao;
	CdboLogCheat*		pLogCheat;
	CdboLogCharExp*		pLogCharExp;
	CdboLogMoney*		pLogMoney;
	CdboLogSMoney*		pLogSMoney;
	CdboLogGoodness*	pLogGoodness;
	CdboLogHack*		pLogHack;
	CdboLogMix*			pLogMix;
	CdboLogSellYuanBao*	pLogSellYuanBao;
	CdboLogHonor*		pLogHonor;
	CdboLogPartner*		pLogPartner;
	CdboLogManor*		pLogManor;
	CdboLogGenAcc*      pLogGenAcc;
	CdboLogGuildDonate* pLogGDonate;

	DatabaseTables()
		: pLogAccount(NULL)
		, pLogCharacter(NULL)
		, pLogItem(NULL)
		, pLogQuest(NULL)
		, pLogPlayerCount(NULL)
		, pLogGiftBox(NULL)
		, pLogChargeShop(NULL)
		, pLogExchange(NULL)
		, pLogYuanBao(NULL)
		, pLogCheat(NULL)
		, pLogCharExp(NULL)
		, pLogMoney(NULL)
		, pLogSMoney(NULL)
		, pLogMix(NULL)
		, pLogGoodness(NULL)
		, pLogHack(NULL)
		, pLogSellYuanBao(NULL)
		, pLogHonor(NULL)
		, pLogPartner(NULL)
		, pLogManor(NULL)
		, pLogGenAcc(NULL)
		, pLogGDonate(NULL) 
	{}
};

struct Global
{
	DatabaseTables		gTables;
	CDboSource*			m_pLogDb;	// main thread Log DB source

	Boolean				m_bExit;		// service exit flag
	LPCPEER				pManagerPeer;	// manager peer

	Global();
	Boolean Initialize(VOID);
	VOID Uninitialize(VOID);
	Boolean ConnectDatabase();
	Boolean ReleaseOLEDB();
};

extern Global gGlobal;

#endif // _GLOBAL_H_
