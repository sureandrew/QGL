#pragma once
#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include "Stub/Logic/Messenger_Stub.h"
class CMessengerData;
class CMessenger : public Messenger_Stub
{
private:
	typedef Messenger_Stub Parent;

public:

	CMessenger();
	virtual ~CMessenger();
	INLINE static CMessenger* NewInstance();
	DEFINE_CLASS(Messenger_CLASSID);

	void SetData(CMessengerData * pMData);
	void WriteData(CMessengerData * pMData);

	void PreSend();
	bool IsRelation(UInt16 relMask);

	virtual RPCResult MoveGroup(LPCPEER pPeer, RPCContext &context, const UInt8 &group);
	virtual RPCResult ModifyRelation(LPCPEER pPeer, RPCContext &context, const UInt8Vector &relations);
	void UpdateToObject();
};

INLINE CMessenger* CMessenger::NewInstance()
{
	return CreateNetObject<CMessenger>();
}

#endif //_MESSENGER_H_
