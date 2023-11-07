//-- Common
#include "Common.h"
//-- Self
#include "SimClient/MessageCharacter.h"
//-- Library

REGISTER_CLASSTYPE(CMessageCharacter, MessageCharacter_CLASSID);

CMessageCharacter::CMessageCharacter()
{
}

CMessageCharacter::~CMessageCharacter()
{
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
}

void  CMessageCharacter::OnUpdateFriend_icon(const UInt8 &friend_icon)
{
}

void  CMessageCharacter::OnUpdateFriend_about(const String &friend_about)
{
}

void  CMessageCharacter::OnUpdateFriend_reply(const String &friend_onlineAutoReply, const String &friend_offlineAutoReply)
{
}


void CMessageCharacter::OnUpdateLine(const UInt8 &line_id)
{
}

void CMessageCharacter::OnUpdateState(const BYTE &state)
{
}
