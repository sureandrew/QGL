#include "Common.h"
#include "Common/common_binary.h"

#include "Proxy/Logic/Messenger.h"
#ifdef QGL
#include "QGL/Global.h"
#include "QGL/MenuFriendProcess.h"
#endif // QGL

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
	#ifdef QGL
	gGlobal.g_pMenuFriendProcess->ConstructData();
	#endif // QGL

}
void CMessenger::OnUpdateFriendly(const UInt16 &friend_friendly) 
{
	#ifdef QGL
	gGlobal.g_pMenuFriendProcess->ConstructData();
	#endif // QGL
}

bool CMessenger::IsRelation(UInt16 relMask)
{
	return (GetFriend_relation() & (1 << relMask)) > 0 ;
}
