///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MESSAGECHARACTER_STUB_H_
#define _RPC_MESSAGECHARACTER_STUB_H_

#include "Common/RPCFunc.h"
#include "Common/ChannelType.h"
#include "RPCEnum/MessageCharacter_enum.h"

class MessageCharacter_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult SelfSettingChange(LPCPEER pPeer, RPCContext &context, const String &about, const UInt8 &OnlineState, const String &Reply);
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
	virtual RPCResult SelfSettingChange(LPCPEER pPeer, RPCContext &context, const String &about, const UInt8 &OnlineState, const String &Reply) = 0;
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
	RPCMask m_modifyMask;
	INLINE Boolean IsModified() { return (!m_modifyMask.IsZero()); }
	INLINE Boolean IsModified(UInt bit) { return (m_modifyMask.IsBit(bit)); }
	INLINE Boolean IsModified(CONST RPCMask& mask) { return (m_modifyMask.IsMask(mask)); }
	INLINE VOID SetModifyMask(CONST RPCMask& mask) { m_modifyMask = mask; }
	INLINE CONST RPCMask& GetModifyMask() CONST { return m_modifyMask; }
	INLINE VOID ClearModifyMask() { m_modifyMask.Clear(); }
	INLINE VOID MarkModify(UInt bit) { m_modifyMask.MaskBit(bit); }
	INLINE VOID MarkModify(CONST RPCMask& mask) { m_modifyMask.Mask(mask); }
	INLINE VOID UnmarkModify(UInt bit) { m_modifyMask.UnmaskBit(bit); }
	INLINE VOID UnmarkModify(CONST RPCMask& mask) { m_modifyMask.Unmask(mask); }
	INLINE virtual VOID ClearAllMask() {
		Parent::ClearAllMask();
		m_modifyMask.Clear();
	}
	INLINE virtual VOID MarkUpdateAll(UInt bit, bool modified) {
		Parent::MarkUpdateAll(bit, modified);
		if (modified) m_modifyMask.MaskBit(bit);
	}
	void SetId(const UInt32 &char_id);
	void SetChar_id(const UInt32 &char_id);
	void SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction);
	void SetNickName(const String &nickName);
	void SetSex(const UInt8 &sex);
	void SetCclass(const UInt8 &cclass);
	void SetSexClass(const UInt8 &sexClass);
	void SetFaction(const UInt8 &faction);
	void SetLevel(const UInt16 &level);
	void SetTitle(const String &title);
	void SetStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor);
	void SetPhoto_id(const UInt16 &photo_id);
	void SetHairStyle1(const UInt16 &hairStyle1);
	void SetHairStyle2(const UInt16 &hairStyle2);
	void SetHairColor(const UInt16 &hairColor);
	void SetEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);
	void SetEq_weapon(const UInt32 &eq_weapon);
	void SetEq_weaponColor(const UInt16 &eq_weaponColor);
	void SetEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	void SetEq_headBand(const UInt32 &eq_headBand);
	void SetEq_headColor(const UInt16 &eq_headColor);
	void SetEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor);
	void SetEq_cloth(const UInt32 &eq_cloth);
	void SetEq_clothColor(const UInt16 &eq_clothColor);
	void SetBattleInfo(const Boolean &EnablePK);
	void SetEnablePK(const Boolean &EnablePK);
	void SetSettingInfo(const Boolean &EnableBlockMakeFriend, const Boolean &EnableBlockMsg);
	void SetEnableBlockMakeFriend(const Boolean &EnableBlockMakeFriend);
	void SetEnableBlockMsg(const Boolean &EnableBlockMsg);
	void SetOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor);
	void SetOrg_hairStyle1(const UInt16 &org_hairStyle1);
	void SetOrg_hairColor(const UInt16 &org_hairColor);
	void SetOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor);
	void SetOrg_eq_cloth(const UInt32 &org_eq_cloth);
	void SetOrg_eq_clothColor(const UInt16 &org_eq_clothColor);
	void SetFriend_status(const UInt8 &friend_status);
	void SetFriend_icon(const UInt16 &friend_icon);
	void SetFriend_about(const String &friend_about);
	void SetFriend_reply(const String &friend_onlineAutoReply, const String &friend_offlineAutoReply);
	void SetFriend_onlineAutoReply(const String &friend_onlineAutoReply);
	void SetFriend_offlineAutoReply(const String &friend_offlineAutoReply);
	void SetLine(const UInt8 &line_id);
	void SetLine_id(const UInt8 &line_id);
	void SetSpeakCount(const UInt16 &globalSpeakCount);
	void SetGlobalSpeakCount(const UInt16 &globalSpeakCount);
	void SetSpeakBlock(const UInt32 &speakBlock);
	void SetOfflineTime(const UInt32 &offlineTime);
	void SetGuild(const UInt32 &guild_uid, const String &guild_name);
	void SetGuild_uid(const UInt32 &guild_uid);
	void SetGuild_name(const String &guild_name);
	void SetMoney(const UInt32 &money);
	void SetInactive(const Boolean &inactive);
	void SetGenerationAcc(const UInt32 &uiHisGenAcc);
	void SetUiHisGenAcc(const UInt32 &uiHisGenAcc);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE MessageCharacter_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_MESSAGECHARACTER_STUB_H_
