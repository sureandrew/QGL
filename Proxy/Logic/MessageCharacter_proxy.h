///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MESSAGECHARACTER_PROXY_H_
#define _RPC_MESSAGECHARACTER_PROXY_H_

#include "Common/RPCFunc.h"
#include "Common/ChannelType.h"
#include "RPCEnum/MessageCharacter_enum.h"

class MessageCharacter_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &char_id);
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateTitle(const String &title);
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor);
	virtual void OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);
	virtual void OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	virtual void OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor);
	virtual void OnUpdateBattleInfo(const Boolean &EnablePK);
	virtual void OnUpdateSettingInfo(const Boolean &EnableBlockMakeFriend, const Boolean &EnableBlockMsg);
	virtual void OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor);
	virtual void OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor);
	virtual void OnUpdateFriend_status(const UInt8 &friend_status);
	virtual void OnUpdateFriend_icon(const UInt16 &friend_icon);
	virtual void OnUpdateFriend_about(const String &friend_about);
	virtual void OnUpdateFriend_reply(const String &friend_onlineAutoReply, const String &friend_offlineAutoReply);
	virtual void OnUpdateLine(const UInt8 &line_id);
	virtual void OnUpdateSpeakCount(const UInt16 &globalSpeakCount);
	virtual void OnUpdateSpeakBlock(const UInt32 &speakBlock);
	virtual void OnUpdateOfflineTime(const UInt32 &offlineTime);
	virtual void OnUpdateGuild(const UInt32 &guild_uid, const String &guild_name);
	virtual void OnUpdateMoney(const UInt32 &money);
	virtual void OnUpdateInactive(const Boolean &inactive);
	virtual void OnUpdateGenerationAcc(const UInt32 &uiHisGenAcc);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		infoMask	= Parent::NextFreeMask + 1,
		levelMask	= Parent::NextFreeMask + 2,
		titleMask	= Parent::NextFreeMask + 3,
		styleMask	= Parent::NextFreeMask + 4,
		eqWeaponMask	= Parent::NextFreeMask + 5,
		eqHeadMask	= Parent::NextFreeMask + 6,
		eqClothMask	= Parent::NextFreeMask + 7,
		BattleInfoMask	= Parent::NextFreeMask + 8,
		SettingInfoMask	= Parent::NextFreeMask + 9,
		orgHairStyleMask	= Parent::NextFreeMask + 10,
		orgEqClothMask	= Parent::NextFreeMask + 11,
		friend_statusMask	= Parent::NextFreeMask + 12,
		friend_iconMask	= Parent::NextFreeMask + 13,
		friend_aboutMask	= Parent::NextFreeMask + 14,
		friend_replyMask	= Parent::NextFreeMask + 15,
		lineMask	= Parent::NextFreeMask + 16,
		speakCountMask	= Parent::NextFreeMask + 17,
		speakBlockMask	= Parent::NextFreeMask + 18,
		offlineTimeMask	= Parent::NextFreeMask + 19,
		guildMask	= Parent::NextFreeMask + 20,
		moneyMask	= Parent::NextFreeMask + 21,
		inactiveMask	= Parent::NextFreeMask + 22,
		GenerationAccMask	= Parent::NextFreeMask + 23,
		NextFreeMask	= Parent::NextFreeMask + 24
	};
	virtual RPCResult SelfSettingChange(LPCPEER pPeer, RPCContext &context, const String &about, const UInt8 &OnlineState, const String &Reply);
	const UInt32& GetChar_id() const { return m.char_id; }
	const String& GetNickName() const { return m.nickName; }
	const UInt8& GetSex() const { return m.sex; }
	const UInt8& GetCclass() const { return m.cclass; }
	const UInt8& GetSexClass() const { return m.sexClass; }
	const UInt8& GetFaction() const { return m.faction; }
	const UInt16& GetLevel() const { return m.level; }
	const String& GetTitle() const { return m.title; }
	const UInt16& GetPhoto_id() const { return m.photo_id; }
	const UInt16& GetHairStyle1() const { return m.hairStyle1; }
	const UInt16& GetHairStyle2() const { return m.hairStyle2; }
	const UInt16& GetHairColor() const { return m.hairColor; }
	const UInt32& GetEq_weapon() const { return m.eq_weapon; }
	const UInt16& GetEq_weaponColor() const { return m.eq_weaponColor; }
	const UInt32& GetEq_headBand() const { return m.eq_headBand; }
	const UInt16& GetEq_headColor() const { return m.eq_headColor; }
	const UInt32& GetEq_cloth() const { return m.eq_cloth; }
	const UInt16& GetEq_clothColor() const { return m.eq_clothColor; }
	const Boolean& GetEnablePK() const { return m.EnablePK; }
	const Boolean& GetEnableBlockMakeFriend() const { return m.EnableBlockMakeFriend; }
	const Boolean& GetEnableBlockMsg() const { return m.EnableBlockMsg; }
	const UInt16& GetOrg_hairStyle1() const { return m.org_hairStyle1; }
	const UInt16& GetOrg_hairColor() const { return m.org_hairColor; }
	const UInt32& GetOrg_eq_cloth() const { return m.org_eq_cloth; }
	const UInt16& GetOrg_eq_clothColor() const { return m.org_eq_clothColor; }
	const UInt8& GetFriend_status() const { return m.friend_status; }
	const UInt16& GetFriend_icon() const { return m.friend_icon; }
	const String& GetFriend_about() const { return m.friend_about; }
	const String& GetFriend_onlineAutoReply() const { return m.friend_onlineAutoReply; }
	const String& GetFriend_offlineAutoReply() const { return m.friend_offlineAutoReply; }
	const UInt8& GetLine_id() const { return m.line_id; }
	const UInt16& GetGlobalSpeakCount() const { return m.globalSpeakCount; }
	const UInt32& GetSpeakBlock() const { return m.speakBlock; }
	const UInt32& GetOfflineTime() const { return m.offlineTime; }
	const UInt32& GetGuild_uid() const { return m.guild_uid; }
	const String& GetGuild_name() const { return m.guild_name; }
	const UInt32& GetMoney() const { return m.money; }
	const Boolean& GetInactive() const { return m.inactive; }
	const UInt32& GetUiHisGenAcc() const { return m.uiHisGenAcc; }
	virtual void OnUpdateId(const UInt32 &char_id) {};
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction) {};
	virtual void OnUpdateLevel(const UInt16 &level) {};
	virtual void OnUpdateTitle(const String &title) {};
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor) {};
	virtual void OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor) {};
	virtual void OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor) {};
	virtual void OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor) {};
	virtual void OnUpdateBattleInfo(const Boolean &EnablePK) {};
	virtual void OnUpdateSettingInfo(const Boolean &EnableBlockMakeFriend, const Boolean &EnableBlockMsg) {};
	virtual void OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor) {};
	virtual void OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor) {};
	virtual void OnUpdateFriend_status(const UInt8 &friend_status) {};
	virtual void OnUpdateFriend_icon(const UInt16 &friend_icon) {};
	virtual void OnUpdateFriend_about(const String &friend_about) {};
	virtual void OnUpdateFriend_reply(const String &friend_onlineAutoReply, const String &friend_offlineAutoReply) {};
	virtual void OnUpdateLine(const UInt8 &line_id) {};
	virtual void OnUpdateSpeakCount(const UInt16 &globalSpeakCount) {};
	virtual void OnUpdateSpeakBlock(const UInt32 &speakBlock) {};
	virtual void OnUpdateOfflineTime(const UInt32 &offlineTime) {};
	virtual void OnUpdateGuild(const UInt32 &guild_uid, const String &guild_name) {};
	virtual void OnUpdateMoney(const UInt32 &money) {};
	virtual void OnUpdateInactive(const Boolean &inactive) {};
	virtual void OnUpdateGenerationAcc(const UInt32 &uiHisGenAcc) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 char_id;
	//info
	String nickName;
	UInt8 sex;
	UInt8 cclass;
	UInt8 sexClass;
	UInt8 faction;
	//level
	UInt16 level;
	//title
	String title;
	//style
	UInt16 photo_id;
	UInt16 hairStyle1;
	UInt16 hairStyle2;
	UInt16 hairColor;
	//eqWeapon
	UInt32 eq_weapon;
	UInt16 eq_weaponColor;
	//eqHead
	UInt32 eq_headBand;
	UInt16 eq_headColor;
	//eqCloth
	UInt32 eq_cloth;
	UInt16 eq_clothColor;
	//BattleInfo
	Boolean EnablePK;
	//SettingInfo
	Boolean EnableBlockMakeFriend;
	Boolean EnableBlockMsg;
	//orgHairStyle
	UInt16 org_hairStyle1;
	UInt16 org_hairColor;
	//orgEqCloth
	UInt32 org_eq_cloth;
	UInt16 org_eq_clothColor;
	//friend_status
	UInt8 friend_status;
	//friend_icon
	UInt16 friend_icon;
	//friend_about
	String friend_about;
	//friend_reply
	String friend_onlineAutoReply;
	String friend_offlineAutoReply;
	//line
	UInt8 line_id;
	//speakCount
	UInt16 globalSpeakCount;
	//speakBlock
	UInt32 speakBlock;
	//offlineTime
	UInt32 offlineTime;
	//guild
	UInt32 guild_uid;
	String guild_name;
	//money
	UInt32 money;
	//inactive
	Boolean inactive;
	//GenerationAcc
	UInt32 uiHisGenAcc;
} m;

public:
	INLINE MessageCharacter_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult MessageCharacter_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageCharacter_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<276770739>(pPeer, pBuf);
}

INLINE RPCResult MessageCharacter_Proxy::SelfSettingChange(LPCPEER pPeer, RPCContext &context, const String &about, const UInt8 &OnlineState, const String &Reply)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageCharacter_SelfSettingChange);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(about);
	pBuf->Write<UInt8>(OnlineState);
	pBuf->Write<String>(Reply);
	return RPCSend<537623301>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(444148693)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(276770739)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(537623301)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_MESSAGECHARACTER_PROXY_H_
