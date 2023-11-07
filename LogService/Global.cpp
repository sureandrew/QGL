//-- Common
#include "Common.h"
#include "Common/RPCFunc.h"
//-- Self
#include "Global.h"
//-- Library
#include <algorithm>

#include "Configure.h"

#include "dboLogAccount.h"
#include "dboLogCharacter.h"
#include "dboLogItem.h"
#include "dboLogQuest.h"
#include "dboLogPlayerCount.h"
#include "dboLogGiftBox.h"
#include "dboLogChargeShop.h"
#include "dboLogExchange.h"
#include "dboLogYuanBao.h"
#include "dboLogCheat.h"
#include "dboLogCharExp.h"
#include "dboLogMoney.h"
#include "dboLogSMoney.h"
#include "dboLogMix.h"
#include "dboLogGoodness.h"
#include "dboLogHack.h"
#include "dboLogSellYuanBao.h"
#include "dboLogHonor.h"
#include "dboLogPartner.h"
#include "dboLogManor.h"
#include "dboLogGenerationAcc.h"
#include "dboLogGuildDonate.h"

#include "DBPWDecrypt.h"

Global::Global()
{
	m_bExit = BOOLEAN_FALSE;
	pManagerPeer = NULL;
	m_pLogDb = NULL;
}

Boolean Global::Initialize(VOID)
{
	return (BOOLEAN_TRUE);
}

VOID Global::Uninitialize(VOID)
{
	ReleaseOLEDB();
}

Boolean Global::ConnectDatabase()
{
	HRESULT hr;

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Begin to Connect Databasae"));

	if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("oledb com initialization failed, hr 0x%08X"), hr);
		return BOOLEAN_FALSE;
	}

	// OLEDB object MUST create using 'new' instead of our ManagedAllocator
	const Database& db = gConfigure.dbLog;
	if (m_pLogDb == NULL)
		m_pLogDb = SafeCreateObject(CDboSource);
#ifdef CONNECT_STR
	String cstr1;
	cstr1.Format(CONNECT_STR, db.strServer.c_str(), db.strDatabase.c_str(), 
		db.strLogin.c_str(), DBPWDecrypt(db.strPassword).c_str());
	if (DB_FAILED(hr = m_pLogDb->Open(cstr1.c_str())))
#else
	if (DB_FAILED(hr = m_pLogDb->Open(db.strServer.c_str(), db.strDatabase.c_str(), 
		db.strLogin.c_str(), DBPWDecrypt(db.strPassword).c_str())))
#endif	// CONNECT_STR
	{
		TRACE_ERRORDTL_5(GLOBAL_LOGGER, _FE("cannot connect to log database, hr 0x%08X, %s, %s, %s, %s, %s"), hr, db.strServer.c_str(), db.strDatabase.c_str(), db.strLogin.c_str(), db.strPassword.c_str());
		m_pLogDb->ShowError(hr);
		ReleaseOLEDB();
		return BOOLEAN_FALSE;
	}

	gTables.pLogAccount = SafeCreateObject(CdboLogAccount);
	gTables.pLogAccount->SetDataSource(m_pLogDb);
	gTables.pLogCharacter = SafeCreateObject(CdboLogCharacter);
	gTables.pLogCharacter->SetDataSource(m_pLogDb);
	gTables.pLogItem = SafeCreateObject(CdboLogItem);
	gTables.pLogItem->SetDataSource(m_pLogDb);
	gTables.pLogQuest = SafeCreateObject(CdboLogQuest);
	gTables.pLogQuest->SetDataSource(m_pLogDb);
	gTables.pLogPlayerCount = SafeCreateObject(CdboLogPlayerCount);
	gTables.pLogPlayerCount->SetDataSource(m_pLogDb);
	gTables.pLogGiftBox = SafeCreateObject(CdboLogGiftBox);
	gTables.pLogGiftBox->SetDataSource(m_pLogDb);
	gTables.pLogChargeShop = SafeCreateObject(CdboLogChargeShop);
	gTables.pLogChargeShop->SetDataSource(m_pLogDb);
	gTables.pLogExchange = SafeCreateObject(CdboLogExchange);
	gTables.pLogExchange->SetDataSource(m_pLogDb);
	gTables.pLogYuanBao = SafeCreateObject(CdboLogYuanBao);
	gTables.pLogYuanBao->SetDataSource(m_pLogDb);
	gTables.pLogCheat = SafeCreateObject(CdboLogCheat);
	gTables.pLogCheat->SetDataSource(m_pLogDb);
	gTables.pLogCharExp = SafeCreateObject(CdboLogCharExp);
	gTables.pLogCharExp->SetDataSource(m_pLogDb);
	gTables.pLogMoney = SafeCreateObject(CdboLogMoney);
	gTables.pLogMoney->SetDataSource(m_pLogDb);
	gTables.pLogSMoney = SafeCreateObject(CdboLogSMoney);
	gTables.pLogSMoney->SetDataSource(m_pLogDb);
	gTables.pLogMix = SafeCreateObject(CdboLogMix);
	gTables.pLogMix->SetDataSource(m_pLogDb);
	gTables.pLogGoodness = SafeCreateObject(CdboLogGoodness);
	gTables.pLogGoodness->SetDataSource(m_pLogDb);
	gTables.pLogHack = SafeCreateObject(CdboLogHack);
	gTables.pLogHack->SetDataSource(m_pLogDb);
	gTables.pLogSellYuanBao = SafeCreateObject(CdboLogSellYuanBao);
	gTables.pLogSellYuanBao->SetDataSource(m_pLogDb);
	gTables.pLogHonor = SafeCreateObject(CdboLogHonor);
	gTables.pLogHonor->SetDataSource(m_pLogDb);
	gTables.pLogPartner = SafeCreateObject(CdboLogPartner);
	gTables.pLogPartner->SetDataSource(m_pLogDb);
	gTables.pLogManor = SafeCreateObject(CdboLogManor);
	gTables.pLogManor->SetDataSource(m_pLogDb);
	gTables.pLogGenAcc = SafeCreateObject(CdboLogGenAcc);
	gTables.pLogGenAcc->SetDataSource(m_pLogDb);
	gTables.pLogGDonate = SafeCreateObject(CdboLogGuildDonate);
	gTables.pLogGDonate->SetDataSource(m_pLogDb);

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Connect Database Success"));

	return (BOOLEAN_TRUE);
}

Boolean Global::ReleaseOLEDB()
{
	// release COM objects
	if (gTables.pLogAccount)
	{
		gTables.pLogAccount->Close();
		SafeDeleteObject(gTables.pLogAccount);		// don't use SafeDelete for COM object
	}

	if (gTables.pLogCharacter)
	{
		gTables.pLogCharacter->Close();
		SafeDeleteObject(gTables.pLogCharacter);
	}

	if (gTables.pLogItem)
	{
		gTables.pLogItem->Close();
		SafeDeleteObject(gTables.pLogItem);		// don't use SafeDelete for COM object
	}

	if (gTables.pLogQuest)
	{
		gTables.pLogQuest->Close();
		SafeDeleteObject(gTables.pLogQuest);		// don't use SafeDelete for COM object
	}

	if (gTables.pLogPlayerCount)
	{
		gTables.pLogPlayerCount->Close();
		SafeDeleteObject(gTables.pLogPlayerCount);
	}

	if (gTables.pLogGiftBox)
	{
		gTables.pLogGiftBox->Close();
		SafeDeleteObject(gTables.pLogGiftBox);
	}

	if (gTables.pLogChargeShop)
	{
		gTables.pLogChargeShop->Close();
		SafeDeleteObject(gTables.pLogChargeShop);
	}

	if (gTables.pLogExchange)
	{
		gTables.pLogExchange->Close();
		SafeDeleteObject(gTables.pLogExchange);
	}

	if (gTables.pLogYuanBao)
	{
		gTables.pLogYuanBao->Close();
		SafeDeleteObject(gTables.pLogYuanBao);
	}

	if (gTables.pLogCheat)
	{
		gTables.pLogCheat->Close();
		SafeDeleteObject(gTables.pLogCheat);
	}

	if (gTables.pLogCharExp)
	{
		gTables.pLogCharExp->Close();
		SafeDeleteObject(gTables.pLogCharExp);
	}

	if (gTables.pLogMoney)
	{
		gTables.pLogMoney->Close();
		SafeDeleteObject(gTables.pLogMoney);
	}

	if (gTables.pLogSMoney)
	{
		gTables.pLogSMoney->Close();
		SafeDeleteObject(gTables.pLogSMoney);
	}

	if (gTables.pLogMix)
	{
		gTables.pLogMix->Close();
		SafeDeleteObject(gTables.pLogMix);
	}

	if (gTables.pLogGoodness)
	{
		gTables.pLogGoodness->Close();
		SafeDeleteObject(gTables.pLogGoodness);
	}

	if (gTables.pLogHack)
	{
		gTables.pLogHack->Close();
		SafeDeleteObject(gTables.pLogHack);
	}

	if (gTables.pLogSellYuanBao)
	{
		gTables.pLogSellYuanBao->Close();
		SafeDeleteObject(gTables.pLogSellYuanBao);
	}

	if (gTables.pLogHonor)
	{
		gTables.pLogHonor->Close();
		SafeDeleteObject(gTables.pLogHonor);
	}

	if (gTables.pLogPartner)
	{
		gTables.pLogPartner->Close();
		SafeDeleteObject(gTables.pLogPartner);
	}

	if (gTables.pLogManor)
	{
		gTables.pLogManor->Close();
		SafeDeleteObject(gTables.pLogManor);
	}
	
	if (NULL != gTables.pLogGenAcc) {
		gTables.pLogGenAcc->Close();
		SafeDeleteObject(gTables.pLogGenAcc);
	}

	if (gTables.pLogGDonate)
	{
		gTables.pLogGDonate->Close();
		SafeDeleteObject(gTables.pLogGDonate);
	}

	if (m_pLogDb)
	{
		m_pLogDb->Close();
		SafeDeleteObject(m_pLogDb);
	}

	CoUninitialize();

	return (BOOLEAN_TRUE);
}
