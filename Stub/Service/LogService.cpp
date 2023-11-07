//-- Common
#include "Common.h"
//-- Self
#include "Stub/Service/LogService.h"
//-- Library
#include "Global.h"
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

// private function and namespace global variable here
namespace LogService_Stub
{
};

PCSTRING GetLocalTimeString(VOID)
{
	static TCHAR szBuffer[40];
	SYSTEMTIME st;
	::GetLocalTime(&st);
	::_stprintf(szBuffer, _T("%d/%d/%d %d:%d:%d"), st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
	return (szBuffer);
}

Boolean OpenLogger(Logger& logger)
{
	String strLogFile = GLOBAL_CONFIG.GetString(GLOBAL_NAME, logger.GetName().c_str(), NULL);
	if (strLogFile.GetLength() == 0)
		return (BOOLEAN_FALSE);
	Int size = GLOBAL_CONFIG.GetInt(GLOBAL_NAME, _T("LogSize"), 0);
	if (size == 0)
		GLOBAL_CONFIG.GetInt(_T("General"), _T("LogSize"), 200);
	String strArchive = GLOBAL_CONFIG.GetString(GLOBAL_NAME, _T("LogArchive"), NULL);
	if (strArchive.GetLength() == 0)
		strArchive = GLOBAL_CONFIG.GetString(_T("General"), _T("LogArchive"), NULL);
	return (logger.Open(strLogFile.c_str(),
		BOOLEAN_FALSE, BOOLEAN_FALSE, BOOLEAN_TRUE, Logger::LEVEL_VERBOSE,
		size, strArchive.c_str()));
}

RPCResult LogService_Stub::LogAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &ip, const UInt8 &action_type)
{
	static Logger logger(_T("LogAccount"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_5(logger,
			_T("INSERT INTO LogAccount (sess_id, acct_id, ip, action_type, create_time) VALUES ('%s', %d, '%s', %d, '%s');"),
			context.sessionId.GetString().c_str(), acct_id, Reuben::Network::IPToString(ip).c_str(), action_type, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogAccount->Log(context.sessionId.GetString().c_str(), acct_id, Reuben::Network::IPToString(ip).c_str(), action_type);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_5(GLOBAL_LOGGER, _FE("hr 0x%X, sess_id %s, acct_id %d, ip %s, action_type %d"),
				hr, context.sessionId.GetString().c_str(), acct_id, Reuben::Network::IPToString(ip).c_str(), action_type);
			gGlobal.gTables.pLogAccount->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt8 &line_id, const UInt8 &action_type)
{
	static Logger logger(_T("LogCharacter"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_6(logger,
			_T("INSERT INTO LogCharacter (sess_id, acct_id, char_id, line_id, action_type, create_time) VALUES ('%s', %d, %d, %d, %d, '%s');"),
			context.sessionId.GetString().c_str(), acct_id, char_id, line_id, action_type, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogCharacter->Log(context.sessionId.GetString().c_str(), acct_id, char_id, line_id, action_type);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_6(GLOBAL_LOGGER, _FE("hr 0x%X, sess_id %s, acct_id %d, char_id %d, line_id %d, action_type %d"),
				hr, context.sessionId.GetString().c_str(), acct_id, char_id, line_id, action_type);
			gGlobal.gTables.pLogCharacter->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogItem(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &target_type, const UInt32 &target_id, const UInt32 &item_uid, const UInt32 &item_id, const UInt16 &item_count, const UInt8 &action_type)
{
	static Logger logger(_T("LogItem"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_9(logger,
			_T("INSERT INTO LogItem (sess_id, char_id, target_type, target_id, item_uid, item_id, item_count, action_type, create_time) VALUES ('%s', %d, %d, %d, %d, %d, %d, %d, '%s');"),
			context.sessionId.GetString().c_str(), char_id, target_type, target_id, item_uid, item_id, item_count, action_type, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogItem->Log(context.sessionId.GetString().c_str(), char_id, target_type, target_id, item_uid, item_id, item_count, action_type);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_9(GLOBAL_LOGGER, _FE("hr 0x%X, sess_id %s, char_id %d, target_type %d, target_id %d, item_uid %d, item_id %d, item_count %d, action_type %d"),
				hr, context.sessionId.GetString().c_str(), char_id, target_type, target_id, item_uid, item_id, item_count, action_type);
			gGlobal.gTables.pLogItem->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogQuest(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &line_id, const UInt8 &source_type, const UInt32 &source_id, const UInt32 &quest_id, const UInt8 &action_type)
{
	static Logger logger(_T("LogQuest"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_8(logger,
			_T("INSERT INTO LogQuest (sess_id, char_id, line_id, source_type, source_id, quest_id, action_type, create_time) VALUES ('%s', %d, %d, %d, %d, %d, %d, '%s');"),
			context.sessionId.GetString().c_str(), char_id, line_id, source_type, source_id, quest_id, action_type, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogQuest->Log(context.sessionId.GetString().c_str(), char_id, line_id, source_type, source_id, quest_id, action_type);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_8(GLOBAL_LOGGER, _FE("hr 0x%X, sess_id %s, char_id %d, line_id %d, source_type %d, source_id %d, quest_id %d, action_type %d"),
				hr, context.sessionId.GetString().c_str(), char_id, line_id, source_type, source_id, quest_id, action_type);
			gGlobal.gTables.pLogQuest->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogPlayerCount(LPCPEER pPeer, RPCContext &context, const UInt8 &line_id, const UInt16 &player_count)
{
	static Logger logger(_T("LogPlayerCount"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_3(logger,
			_T("INSERT INTO LogPlayerCount (line_id, player_count, create_time) VALUES (%d, %d, '%s');"),
			line_id, player_count, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogPlayerCount->Log(line_id, player_count);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("hr 0x%X, line_id %d, player_count %d"),
				hr, line_id, player_count);
			gGlobal.gTables.pLogPlayerCount->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogGiftBox(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt32 &item_id, const UInt8 &itemType, const UInt8 &quantity, const String &serialNo)
{
	static Logger logger(_T("LogGiftBox"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_7(logger,
			_T("INSERT INTO LogGiftBox (acct_id, char_id, item_id, itemType, quantity, serialNo, create_time) VALUES (%d, %d, %d, %d, %d, '%s', '%s');"),
			acct_id, char_id, item_id, itemType, quantity, serialNo.c_str(), GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogGiftBox->Log(acct_id, char_id, item_id, itemType, quantity, serialNo.c_str());
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_7(GLOBAL_LOGGER, _FE("hr 0x%X, acct_id %d, char_id %d, item_id %d, itemType %d, quantity %d, serialNo %s"),
				hr, acct_id, char_id, item_id, itemType, quantity, serialNo.c_str());
			gGlobal.gTables.pLogGiftBox->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogChargeShop(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt32 &item_id, const UInt16 &item_count, const UInt8 &item_type, const UInt32 &yuanbao)
{
	static Logger logger(_T("LogChargeShop"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_8(logger,
			_T("INSERT INTO LogChargeShop (sess_id, acct_id, char_id, item_id, item_count, item_type, yuanbao, create_time) VALUES ('%s', %d, %d, %d, %d, %d, %d, '%s');"),
			context.sessionId.GetString().c_str(), acct_id, char_id, item_id, item_count, item_type, yuanbao, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogChargeShop->Log(context.sessionId.GetString().c_str(), acct_id, char_id, item_id, item_count, item_type, yuanbao);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_8(GLOBAL_LOGGER, _FE("hr 0x%X, sess_id %s, acct_id %d, char_id %d, item_id %d, item_count %d, item_type %d, yuanbao %d"),
				hr,context.sessionId.GetString().c_str(), acct_id, char_id, item_id, item_count, item_type, yuanbao);
			gGlobal.gTables.pLogChargeShop->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogExchange(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &target_char_id, const UInt8 &process_type, const UInt8 &item_type, const UInt32 &item_uid, const UInt32 &item_id, const UInt8 &item_count, const UInt32 &money, const UInt32 &tax)
{
	static Logger logger(_T("LogExchange"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYS_9(logger,
			_T("INSERT INTO LogExchange (sess_id, char_id, target_char_id, process_type, item_type, item_uid, item_id, item_count, money, tax, create_time) VALUES ('%s', %d, %d, %d, %d, %d, %d, %d, %d, "),
			context.sessionId.GetString().c_str(), char_id, target_char_id, process_type, item_type, item_uid, item_id, item_count, money);
		TRACE_ALWAYSL_2(logger, _T("%d, '%s'"), tax, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogExchange->Log(context.sessionId.GetString().c_str(), char_id, target_char_id, process_type, item_type, item_uid, item_id, item_count, money, tax);
		if (FAILED(hr))
		{
			TRACE_ERRORDT_4(GLOBAL_LOGGER, _FE("hr 0x%X, sess_id %s, char_id %d, target_char_id %d, "),
				hr,context.sessionId.GetString().c_str(), char_id, target_char_id);
			TRACE_ERRORL_7(GLOBAL_LOGGER, _F("process_type %d, item_type %d, item_uid %d, item_id %d, item_count %d, money %d, tax %d"),
				process_type, item_type, item_uid, item_id, item_count, money, tax);
			gGlobal.gTables.pLogExchange->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogExchangeForItemList(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &target_char_id, const UInt8 &process_type, const UInt8Vector &item_type, const UInt32Vector &item_uid, const UInt32Vector &item_id, const UInt8Vector &item_count)
{
	static Logger logger(_T("LogExchange"));
	static Boolean bOpen = OpenLogger(logger);
	for (Index i = C_INDEX(0); i < item_type.size(); ++i)
	{
		if (bOpen)
		{
			TRACE_ALWAYS_9(logger,
				_T("INSERT INTO LogExchange (sess_id, char_id, target_char_id, process_type, item_type, item_uid, item_id, item_count, money, tax, create_time) VALUES ('%s', %d, %d, %d, %d, %d, %d, %d, %d, "),
				context.sessionId.GetString().c_str(), char_id, target_char_id, process_type, item_type[i], item_uid[i], item_id[i], item_count[i], 0);
			TRACE_ALWAYSL_2(logger, _T("%d, '%s'"), 0, GetLocalTimeString());
		}
		else
		{
			HRESULT hr = gGlobal.gTables.pLogExchange->Log(context.sessionId.GetString().c_str(), char_id, target_char_id, process_type, item_type[i], item_uid[i], item_id[i], item_count[i], 0, 0);
			if (FAILED(hr))
			{
				TRACE_ERRORDT_4(GLOBAL_LOGGER, _FE("hr 0x%X, sess_id %s, char_id %d, target_char_id %d, "),
					hr,context.sessionId.GetString().c_str(), char_id, target_char_id);
				TRACE_ERRORL_7(GLOBAL_LOGGER, _F("process_type %d, item_type %d, item_uid %d, item_id %d, item_count %d, money %d, tax %d"),
					process_type, item_type, item_uid, item_id, item_count, 0, 0);
				gGlobal.gTables.pLogExchange->ShowError(hr);
			}	
		}
	}

	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt8 &useplace, const UInt32 &yuanbao)
{
	static Logger logger(_T("LogYuanBao"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_6(logger,
			_T("INSERT INTO LogYuanBao (sess_id, acct_id, char_id, useplace, yuanbao, create_time) VALUES ('%s', %d, %d, %d, %d, '%s');"),
			context.sessionId.GetString().c_str(), acct_id, char_id, useplace, yuanbao, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogYuanBao->Log(context.sessionId.GetString().c_str(), acct_id, char_id, useplace, yuanbao);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_6(GLOBAL_LOGGER, _FE("hr 0x%X, sess_id %s, acct_id %d, char_id %d, item_id %d, item_count %d, item_type %d, yuanbao %d"),
				hr,context.sessionId.GetString().c_str(), acct_id, char_id, useplace, yuanbao);
			gGlobal.gTables.pLogYuanBao->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogCheat(LPCPEER pPeer, RPCContext &context, const UInt32 &exe_id, const UInt32 &source_acct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command, const UInt8 &status)
{
	static Logger logger(_T("LogCheat"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_7(logger,
			_T("INSERT INTO LogCheat (exe_id, source_acct_id, target_acct_id, target_char_id, command, result, create_time) VALUES (%d, %d, %d, %d, '%s', %d, '%s');"),
			exe_id, source_acct_id, target_acct_id, target_char_id, command.c_str(), status, GetLocalTimeString());
	}
	else
	{
		if (gConfigure.do_log_cheat)
		{
			HRESULT hr = gGlobal.gTables.pLogCheat->Log(exe_id, source_acct_id, target_acct_id, target_char_id, command.c_str(), status);
			if (FAILED(hr))
			{
				TRACE_ERRORDTL_8(GLOBAL_LOGGER, 
					_FE("hr 0x%X, sess_id %s, exe_id %d, source_acct_id %d, target_acct_id %d, target_char_id %d, command %s, status %d"),
					hr, context.sessionId.GetString().c_str(), exe_id, source_acct_id,
					target_acct_id, target_char_id, command.c_str(), status);
				gGlobal.gTables.pLogCheat->ShowError(hr);
			}
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogCharExp(LPCPEER pPeer, RPCContext &context, const UInt16 &exp_type, const Int32 &exp_value, const UInt32 &acct_id, const UInt32 &char_id
									  , const UInt32 &total_exp, const UInt16 &level)
{
	static Logger logger(_T("LogCharExp"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_7(logger,
			_T("INSERT INTO LogCharExp (exp_type, exp_value, acct_id, char_id, create_time, total_exp_value, \"level\") VALUES (%d, %d, %d, %d, '%s', %d, %d);"),
			exp_type, exp_value, acct_id, char_id, GetLocalTimeString(), total_exp, level);
	}
	else
	{
		HRESULT hr = S_OK;
		#ifdef LOG_FOR_DETAIL
		hr = gGlobal.gTables.pLogCharExp->Log(exp_type, exp_value, acct_id, char_id, total_exp, level);
		#else
		hr = gGlobal.gTables.pLogCharExp->Log(exp_type, exp_value, acct_id, char_id);
		#endif
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_8(GLOBAL_LOGGER, 
				_FE("hr 0x%X, sess_id %s, exp_type %d, exp_value %d, acct_id %d, char_id %d, total_exp: %d, level: %d"),
				hr, context.sessionId.GetString().c_str(), exp_type, exp_value,
				acct_id, char_id, total_exp, level);
			gGlobal.gTables.pLogCharExp->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt16 &source, const UInt32 &money, const UInt32 &totalMoney)
{
	static Logger logger(_T("LogMoney"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_6(logger, _T("INSERT INTO LogMoney (acct_id, char_id, source, money, create_time, totalMoney) VALUES (%d, %d, %d, %d, '%s', %d);"),
			acct_id, char_id, source, money, GetLocalTimeString(), totalMoney);
	}
	else
	{
		HRESULT hr = S_OK;
		#ifdef LOG_FOR_DETAIL
		hr = gGlobal.gTables.pLogMoney->Log(acct_id, char_id, source, money, totalMoney);
		#else
		hr = gGlobal.gTables.pLogMoney->Log(acct_id, char_id, source, money);
		#endif
		
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_6(GLOBAL_LOGGER, _FE("hr 0x%X, acct_id %d, char_id %d, source %d, money %d, totalMoney %d"),
				hr, acct_id, char_id, source, money, totalMoney);
			gGlobal.gTables.pLogMoney->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogSMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt16 &source, const UInt32 &smoney)
{
	static Logger logger(_T("LogSMoney"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_5(logger, _T("INSERT INTO LogSMoney (acct_id, char_id, source, smoney, create_time) VALUES (%d, %d, %d, %d, '%s');"),
			acct_id, char_id, source, smoney, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogSMoney->Log(acct_id, char_id, source, smoney);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_5(GLOBAL_LOGGER, _FE("hr 0x%X, acct_id %d, char_id %d, source %d, smoney %d"),
				hr, acct_id, char_id, source, smoney);
			gGlobal.gTables.pLogSMoney->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogMix(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &process_type, const UInt32 &result_item_id, const UInt8 &result_item_count, const UInt32Vector &source_item_uid, const UInt32Vector &source_item_id, const UInt8Vector &source_item_count)
{
	static Logger logger(_T("LogMix"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYS_5(logger,
			_T("INSERT INTO LogMix (sess_id, char_id, process_type, result_item_id, result_item_count, ")
			_T("source_item_uid1, source_item_id1, source_item_count1, source_item_uid2, source_item_id2, source_item_count2, ")
			_T("source_item_uid3, source_item_id3, source_item_count3, source_item_uid4, source_item_id4, source_item_count4, ")
			_T("source_item_uid5, source_item_id5, source_item_count5, source_item_uid6, source_item_id6, source_item_count6, ")
			_T("source_item_uid7, source_item_id7, source_item_count7, source_item_uid8, source_item_id8, source_item_count8, ")
			_T("create_time) VALUES (")
			_T("'%s', %d, %d, %d, %d, "),
			context.sessionId.GetString().c_str(), char_id, process_type, result_item_id, result_item_count);
		TRACE_ALWAYS_9(logger, _T("%d, %d, %d, %d, %d, %d, %d, %d, %d, "),
			source_item_uid[0], source_item_id[0], source_item_count[0],
			source_item_uid[1], source_item_id[1], source_item_count[1],
			source_item_uid[2], source_item_id[2], source_item_count[2]);
		TRACE_ALWAYS_9(logger, _T("%d, %d, %d, %d, %d, %d, %d, %d, %d, "),
			source_item_uid[3], source_item_id[3], source_item_count[3],
			source_item_uid[4], source_item_id[4], source_item_count[4],
			source_item_uid[5], source_item_id[5], source_item_count[5]);
		TRACE_ALWAYSL_7(logger, _T("%d, %d, %d, %d, %d, %d, '%s');"),
			source_item_uid[6], source_item_id[6], source_item_count[6],
			source_item_uid[7], source_item_id[7], source_item_count[7],
			GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogMix->Log(context.sessionId.GetString().c_str(), char_id, process_type, result_item_id, result_item_count, 
			source_item_uid, source_item_id, source_item_count);
		if (FAILED(hr))
		{
			String source_item_str;
			for (Index i = C_INDEX(0); i < source_item_uid.size(); ++i)
			{
				if (source_item_uid[i] > 0)
					source_item_str.AppendFormat(_T("source_item %d , uid %d, id %d, count %d"), i+1, source_item_uid[i], source_item_id[i], source_item_count[i]);			
			}
			TRACE_ERRORDTL_7(GLOBAL_LOGGER, _FE("hr 0x%X, session_id %s, char_id %d, process_type %d, result_item_id %d, result_item_count %d, source_item %s"),
				hr, context.sessionId.GetString().c_str(), char_id, process_type, result_item_id, result_item_count, source_item_str.c_str());
			gGlobal.gTables.pLogMix->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogGoodness(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const UInt16 &source, const Int32 &goodness)
{
	static Logger logger(_T("LogGoodness"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_5(logger, _T("INSERT INTO LogGoodness (acct_id, char_id, source, goodness, create_time) VALUES (%d, %d, %d, %d, '%s');"),
			acct_id, char_id, source, goodness, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogGoodness->Log(acct_id, char_id, source, goodness);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_5(GLOBAL_LOGGER, _FE("hr 0x%X, acct_id %d, char_id %d, source %d, goodness %d"),
				hr, acct_id, char_id, source, goodness);
			gGlobal.gTables.pLogGoodness->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogHack(LPCPEER pPeer, RPCContext &context, 
								   const String &acctName, const String &charName, 
								   const UInt32 &msgTime, const UInt16 &msgType, 
								   const String &typeName, const String &msg)
{
	static Logger logger(_T("LogHack"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_7(logger,
			_T("INSERT INTO LogHack (Accounts, CharacterName, Time, Type, TypeName, LogInfo, logtime) VALUES ('%s', '%s', %d, %d, '%s', '%s', '%s');"),
			acctName.c_str(), charName.c_str(), msgTime, msgType, typeName.c_str(), msg.c_str(), GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogHack->Log(acctName.c_str(), charName.c_str(), 
			msgTime, msgType, typeName.c_str(), msg.c_str());
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_6(GLOBAL_LOGGER, 
				_FE("hr 0x%X, acct_name %s, char_name %s, msgType %d, typeName %s, msg size %d"),
				hr, acctName.c_str(), charName.c_str(), msgType, typeName.c_str(), msg.size());
			gGlobal.gTables.pLogHack->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogSellYuanBao(LPCPEER pPeer, RPCContext &context, 
										  const UInt32 &acct_id, const UInt32 &char_id, 
										  const UInt8 &process_type, const UInt32 &target_id, 
										  const UInt32 &YB, const UInt32 &YBT, 
										  const UInt32 &moneyPerUnit, const UInt32 &totalMoney, 
										  const UInt32 &tax, const UInt32 &expireDate)
{
	static Logger logger(_T("LogSellYuanBao"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		struct tm* lt = localtime((const time_t*)&expireDate);
		TCHAR szExpire[40];
		::_stprintf(szExpire, _T("%d/%d/%d %d:%d:%d"), lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900, lt->tm_hour, lt->tm_min, lt->tm_sec);

		TRACE_ALWAYS_9(logger,
			_T("INSERT INTO LogSellYuanBao (sess_id, acct_id, char_id, process_type, target_id, YB, YBT, moneyPerUnit, totalMoney, tax, expireDate, create_time) ")
			_T("VALUES ('%s', %d, %d, %d, %d, %d, %d, %d, %d, "),
			context.sessionId.GetString().c_str(), acct_id, char_id, process_type, target_id, YB, YBT, moneyPerUnit, totalMoney);
		TRACE_ALWAYSL_3(logger,
			_T("%d, '%s', '%s');"),
			tax, szExpire, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogSellYuanBao->Log(context.sessionId.GetString().c_str(), acct_id, 
			char_id, process_type, target_id, YB, YBT, moneyPerUnit, totalMoney, tax, expireDate);
		if (FAILED(hr))
		{
			TRACE_ERRORDT_7(GLOBAL_LOGGER, 
				_FE("hr 0x%X, acct_id %d, char_id %d, process_type %d, target_id %d, YB %d, YBT %d, "),
				hr, acct_id, char_id, process_type, target_id, YB, YBT);

			TRACE_ERRORL_4(GLOBAL_LOGGER,
				_T("moneyPerUnit %d, totalMoney %d, tax %d, expireDate %d"),
				moneyPerUnit, totalMoney, tax, expireDate);
			gGlobal.gTables.pLogSellYuanBao->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogHonor(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Int32 &change)
{
	static Logger logger(_T("LogHonor"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_4(logger, _T("INSERT INTO LogHonor (sess_id, char_id, change, create_time) VALUES ('%s', %d, %d, '%s');"),
			context.sessionId.GetString().c_str(), char_id, change, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogHonor->Log(context.sessionId.GetString().c_str(), char_id, change);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_4(GLOBAL_LOGGER,
				_FE("hr 0x%X, sessionId %s, char_id %d, change %d"),
				hr, context.sessionId.GetString().c_str(), char_id, change);
			gGlobal.gTables.pLogHonor->ShowError(hr);
		}
	}
	return (RPC_RESULT_OK);
}

RPCResult LogService_Stub::LogPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id, const UInt32 &mob_id, const UInt32 &char_id, const UInt8 &process_type)
{
	static Logger logger(_T("LogPartner"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_6(logger, _T("INSERT INTO LogPartner (sess_id, partner_id, mob_id, char_id, process_type, create_time) VALUES ('%s', %d, %d, %d, %d, '%s');"),
			context.sessionId.GetString().c_str(), partner_id, mob_id, char_id, process_type, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogPartner->Log(context.sessionId.GetString().c_str(), partner_id, mob_id, char_id, process_type);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_6(GLOBAL_LOGGER,
				_FE("hr 0x%X, sessionId %s, partner_id %d, mob_id %d, char_id %d, process_type %d"),
				hr, context.sessionId.GetString().c_str(), partner_id, mob_id, char_id, process_type);
			gGlobal.gTables.pLogPartner->ShowError(hr);
		}
	}
	return (RPC_RESULT_OK);
}

RPCResult LogService_Stub::LogManor(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &fund,
									const UInt32 &prosper,  const UInt8 &scale, const UInt8 &fundSrc, const UInt8 &prosperSrc)
{
	static Logger logger(_T("LogManor"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_7(logger,
			_T("INSERT INTO LogManor (guild_uid, guild_fund, guild_prosper, manor_scale, fund_source, prosper_source, create_time) VALUES (%d, %d, %d, %d, %d, %d, '%s');"),
			guild_uid, fund, prosper, scale, fundSrc, prosperSrc, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogManor->Log(guild_uid, fund, prosper, scale, fundSrc, prosperSrc);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_7(GLOBAL_LOGGER, _FE("hr 0x%X, guild_uid %d, guild_fund %d, guild_prosper %d, manor_scale %d fund_source %d prosper_source %d"),
				hr, guild_uid, fund, prosper, scale, fundSrc, prosperSrc);
			gGlobal.gTables.pLogManor->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}
RPCResult LogService_Stub::LogGenerationAcc(LPCPEER pPeer, RPCContext &context,
											const UInt32 &uiTargetId,
											const UInt32 &uiRelatedId, 
											const UInt32 &uiAcc, 
											const UInt16 &ui16TargetLevel,
											const UInt16 &ui16RelatedLevel, 
											const UInt32 &uiItemUId,
											const UInt32 &uiItemId)
{
	static Logger logger(_T("LogGenerationAcc"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_8(logger,
			_T("INSERT INTO LogGenerationAcc (target_id, related_id, accumulation, target_level, related_level,item_uid,item_id,create_time) VALUES (%u, %u, %u, %u, %u, %u,%u,'%s')"),
			uiTargetId, uiRelatedId, uiAcc, ui16TargetLevel,ui16RelatedLevel,uiItemUId,uiItemId, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogGenAcc->Log(uiTargetId, uiRelatedId, uiAcc, ui16TargetLevel,ui16RelatedLevel,uiItemUId,uiItemId);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_9(GLOBAL_LOGGER, _FE("hr 0x%X, sessionId %s, target_id %u, related_id %u, accumulation %u,target_level %u,related_level %u,item_uid %u,item_id %u"),
				hr, context.sessionId.GetString().c_str(),uiTargetId, uiRelatedId, uiAcc, ui16TargetLevel,ui16RelatedLevel,uiItemUId,uiItemId);
			gGlobal.gTables.pLogGenAcc->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult LogService_Stub::LogGuildDonate(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_uid,
									const Int32 &donate,  const UInt32 &total_donate, const UInt8 &source)
{
	static Logger logger(_T("LogGuildDonate"));
	static Boolean bOpen = OpenLogger(logger);
	if (bOpen)
	{
		TRACE_ALWAYSL_6(logger,
			_T("INSERT INTO LogManor (guild_uid, char_uid, donate, total_donate, source, create_time) VALUES (%d, %d, %d, %d, %d, '%s');"),
			guild_uid, char_uid, donate, total_donate, source, GetLocalTimeString());
	}
	else
	{
		HRESULT hr = gGlobal.gTables.pLogGDonate->Log(guild_uid, char_uid, donate, total_donate, source);
		if (FAILED(hr))
		{
			TRACE_ERRORDTL_6(GLOBAL_LOGGER, _FE("hr 0x%X, guild_uid %d, char_uid %d, donate %d, total_donate %d source %d"),
				hr, guild_uid, char_uid, donate, total_donate, source);
			gGlobal.gTables.pLogGDonate->ShowError(hr);
		}
	}
	return RPC_RESULT_OK;
}

