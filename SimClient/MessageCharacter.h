#pragma once
#ifndef _MESSAGECHARACTER_H_
#define _MESSAGECHARACTER_H_

#include "Proxy/Logic/MessageCharacter_Proxy.h"

class CMessageCharacter : public MessageCharacter_Proxy
{
private:
	typedef MessageCharacter_Proxy Parent;
// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &char_id);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateTitle(const String &title);
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor);
	virtual void OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);
	virtual void OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	virtual void OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor);
	virtual void OnUpdateBattleInfo(const Boolean &EnablePK);
	virtual void OnUpdateSettingInfo(const Boolean &EnableBlockMakeFriend);
	virtual void OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor);
	virtual void OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor);
	virtual void OnUpdateFriend_status(const UInt8 &friend_status);
	virtual void OnUpdateFriend_icon(const UInt8 &friend_icon);
	virtual void OnUpdateFriend_about(const String &friend_about);
	virtual void OnUpdateFriend_reply(const String &friend_onlineAutoReply, const String &friend_offlineAutoReply);
	virtual void OnUpdateLine(const UInt8 &line_id);
	virtual void OnUpdateState(const BYTE &state);
public:
	DEFINE_CLASS(MessageCharacter_CLASSID);
	CMessageCharacter();
	virtual ~CMessageCharacter();
};

#endif //_MESSAGECHARACTER_H_
