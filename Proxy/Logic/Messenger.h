#pragma once
#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include "Proxy/Logic/Messenger_Proxy.h"

class CMessenger : public Messenger_Proxy
{
private:
	typedef Messenger_Proxy Parent;
// OnUpdate can be implemented selectively
	virtual void OnUpdateKey(const UInt32 &player_char_id, const UInt32 &friend_char_id);
	virtual void OnUpdateRelation(const String &friend_relation);
	virtual void OnUpdateGroup(const UInt8 &friend_group);
	virtual void OnUpdateFriendly(const UInt16 &friend_friendly);

public:
	DEFINE_CLASS(Messenger_CLASSID);
	CMessenger();
	virtual ~CMessenger();

	bool IsRelation(UInt16 relMask);
};

#endif //_MESSENGER_H_
