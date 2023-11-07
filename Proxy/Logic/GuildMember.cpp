//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/GuildMember.h"
#include "Proxy/Logic/GuildApplicant.h"
#ifdef QGL
#include "QGL/Global.h"
#include "QGL/MenuGuildProcess.h"
#endif
REGISTER_CLASSTYPE(CGuildMember, GuildMember_CLASSID);


CGuildMember::CGuildMember()
{
	m_post = 0;
}
RPCResult CGuildMember::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	return RPC_RESULT_OK;
}

void CGuildMember::OnUpdateDonate(const UInt32 &Donate)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshMemberMenu();
	if (this == gGlobal.g_pMenuGuildProcess->m_ownGuildMember)
		gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif
}
void CGuildMember::OnUpdateTotalDonate(const UInt32 &totalDonate)
{
	#ifdef QGL
	if (this == gGlobal.g_pMenuGuildProcess->m_ownGuildMember)
		gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif
}

void CGuildMember::OnUpdateLevel(const UInt16 &level)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshMemberMenu();	
	#endif
}
void CGuildMember::OnUpdateOfflineTime(const UInt32 &offlineTime)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshMemberMenu();
	#endif
}
void CGuildMember::OnUpdatePost(const UInt8 &Post)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->ConstructMemberOrder();

	Boolean needRefresh = BOOLEAN_FALSE;
	if (m_post != Post && m_post != 0)
	{
		
		if (Post == GUILD_POSITION_MASTER)
		{
			String content;
			content.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_MASTER_POSITION_CHANGE")), GetName().c_str());
			gGlobal.PrintMessage(content.c_str(), CHANNELTYPE_GUILD);
			needRefresh = BOOLEAN_TRUE;
		}
		if (gGlobal.g_pMenuGuildProcess->m_ownGuildMember == this)
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_POSITION_CHANGE_NOTIFY")), CHANNELTYPE_SYSTEM);
	}
	m_post = Post;
	if (this == gGlobal.g_pMenuGuildProcess->m_ownGuildMember || needRefresh)
        gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif
}

RPCResult CGuildMember::CB_PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &src, const UInt32 &target, const UInt8 &position)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->PositionChange(src, target, position);
	#endif
	return RPC_RESULT_OK;
}