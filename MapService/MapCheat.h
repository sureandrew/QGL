#pragma once
#ifndef _MAPCHEAT_H_
#define _MAPCHEAT_H_

class CCharacter;

class CMapCheat
{
public:
	static bool IsGameMaster(UInt32 acct_id);
	static Boolean DoCheat(CCharacter* pChar, LPCPEER pPeer, RPCContext &context, 
							UInt32 acct_id, const String &code);
	static Boolean DoGlobalCheat(LPCPEER pPeer, RPCContext &context, 
							UInt32 acct_id, const String &code);

};

#endif //_MAPCHEAT_H_
