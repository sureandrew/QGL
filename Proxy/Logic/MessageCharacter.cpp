#include "Common.h"
#include "Proxy/Logic/MessageCharacter.h"

REGISTER_CLASSTYPE(CMessageCharacter, MessageCharacter_CLASSID);
#include "Proxy/Logic/Messenger.h"
#ifdef QGL
#include "QGL/Global.h"
#include "QGL/MenuFriendProcess.h"
#include "QGL/CharacterControl.h"
#endif // QGL

CMessageCharacter::CMessageCharacter()
{
	m_PreState = -1;
}

CMessageCharacter::~CMessageCharacter()
{
}

void CMessageCharacter::OnUpdateSpeakCount(const UInt16 &globalSpeakCount)
{
#ifdef QGL
	
#endif
}

void CMessageCharacter::OnUpdateId(const UInt32 &char_id)
{
}

void CMessageCharacter::OnUpdateLevel(const UInt16 &level)
{
}

void CMessageCharacter::OnUpdateTitle(const String &title)
{
}

void CMessageCharacter::OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor)
{
}

void CMessageCharacter::OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor)
{
}

void CMessageCharacter::OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor)
{
}

void CMessageCharacter::OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor)
{
}

void CMessageCharacter::OnUpdateBattleInfo(const Boolean &EnablePK)
{
}

void CMessageCharacter::OnUpdateSettingInfo(const Boolean &EnableBlockMakeFriend)
{
}

void CMessageCharacter::OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor)
{
}

void CMessageCharacter::OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor)
{
}

void  CMessageCharacter::OnUpdateFriend_status(const UInt8 &friend_status)
{
	#ifdef QGL
	if (m_PreState == -1 || m_PreState == FRIENC_CHAT_STATUS_OFFLINE)
	{
		CMessenger * pMes = gGlobal.g_pMenuFriendProcess->FindMessenger(GetChar_id());
		if (pMes)
		{
			if (friend_status != FRIENC_CHAT_STATUS_OFFLINE )
			{
				String text;
				if (pMes->IsRelation(FRIEND_RELATION_MASTER))
				{
					if (gGlobal.g_pMenuFriendProcess->m_bInit)
					{
						text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_MASTER_ONLINE")), GetNickName().c_str());
						gGlobal.PrintMessage(text, CHANNELTYPE_NOTICE);
					}
				}
				else if(pMes->IsRelation(FRIEND_RELATION_STUDENT))
				{
					if (gGlobal.g_pMenuFriendProcess->m_bInit)
					{
						text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_STUDENT_ONLINE")), GetNickName().c_str());
						gGlobal.PrintMessage(text, CHANNELTYPE_NOTICE);
					}
				}
				else if (pMes->IsRelation(FRIEND_RELATION_HUSBAND))
				{
					if (gGlobal.g_pMenuFriendProcess->m_bInit)
					{
						text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_HUSBAND_ONLINE_FIRST")), GetNickName().c_str());
						gGlobal.PrintMessage(text, CHANNELTYPE_NOTICE);
					}
				}
				else if (pMes->IsRelation(FRIEND_RELATION_WIFE))
				{
					if (gGlobal.g_pMenuFriendProcess->m_bInit)
					{
						text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_WIFE_ONLINE_FIRST")), GetNickName().c_str());
						gGlobal.PrintMessage(text, CHANNELTYPE_NOTICE);
					}
				}
				else
				{
					if (gGlobal.g_pMenuFriendProcess->m_bInit)
					{
						text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_ONLINE")), GetNickName().c_str());
						gGlobal.PrintMessage(text, CHANNELTYPE_NOTICE);
					}
				}
				gGlobal.g_pMenuFriendProcess->ConstructData();
			}
		}
	}
	else if (m_PreState == FRIENC_CHAT_STATUS_ONLINE || m_PreState == FRIENC_CHAT_STATUS_ONLINE_AWAY)
	{
		CMessenger * pMes = gGlobal.g_pMenuFriendProcess->FindMessenger(GetChar_id());
		if (pMes)
		{
			if (friend_status == FRIENC_CHAT_STATUS_OFFLINE )
			{
				String text;
				text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_OFFLINE")), GetNickName().c_str());
				gGlobal.PrintMessage(text, CHANNELTYPE_NOTICE);
				gGlobal.g_pMenuFriendProcess->ConstructData();
			}
		}
	}

	m_PreState = friend_status;

	#endif
}
void  CMessageCharacter::OnUpdateFriend_icon(const UInt16 &friend_icon)
{
	#ifdef QGL
	
	gGlobal.g_pMenuFriendProcess->RefreshMenu();
	gGlobal.g_pMenuFriendProcess->RefreshChatMenu();

	if (gGlobal.g_pMenuFriendProcess->m_pOwnChar == this)
	{
		gGlobal.g_pMenuFriendProcess->RefreshSelfSetting();
	}

	#endif
}
void  CMessageCharacter::OnUpdateFriend_about(const String &friend_about)
{
	#ifdef QGL
	if (gGlobal.g_pMenuFriendProcess->m_pOwnChar == this)
	{
		gGlobal.g_pMenuFriendProcess->RefreshSelfSetting();
	}
#endif
}
void  CMessageCharacter::OnUpdateFriend_reply(const String &friend_onlineAutoReply, const String &friend_offlineAutoReply)
{
	#ifdef QGL
	if (gGlobal.g_pMenuFriendProcess->m_pOwnChar == this)
	{
		gGlobal.g_pMenuFriendProcess->RefreshSelfSetting();
	}
#endif
}


void CMessageCharacter::OnUpdateLine(const UInt8 &line_id)
{
}



