#include "Common.h"
#include "SimClient/Messenger.h"
#include "SimClient/Global.h"

REGISTER_CLASSTYPE(CMessenger, Messenger_CLASSID);

CMessenger::CMessenger()
{
}

CMessenger::~CMessenger()
{
}

void CMessenger::OnUpdateKey(const UInt32 &player_char_id, const UInt32 &friend_char_id) 
{
}
void CMessenger::OnUpdateRelation(const String &friend_relation) 
{
}
void CMessenger::OnUpdateGroup(const UInt8 &friend_group) 
{
}
void CMessenger::OnUpdateFriendly(const UInt16 &friend_friendly) 
{
}
void CMessenger::OnUpdateStatus(const BYTE &state) 
{
	
}
void CMessenger::OnUpdateName(const String &nickName) 
{
}
void CMessenger::OnUpdateIcon(const UInt8 &icon) 
{
}
void CMessenger::OnUpdateAbout(const String &about) 
{
}

bool CMessenger::IsRelation(UInt16 relMask)
{
	return (GetFriend_relation() & relMask) > 0;
}
