#pragma once
#ifndef _WORLDSERVICE_H_
#define _WORLDSERVICE_H_

#include "Stub/Service/WorldService_Stub.h"

// public functions here
namespace WorldService_Stub
{
	void ReturnAccount(SessionID &sessionId);
	Boolean DoGlobalCheat(LPCPEER pPeer, RPCContext &context, UInt32 exe_id, UInt32 src_acct_id, 
						 UInt32 target_acct_id, UInt32 target_char_id, const String &code);
};

#endif //_WORLDSERVICE_H_
