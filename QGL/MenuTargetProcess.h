#pragma once
#ifndef _MENUTARGET_H_
#define _MENUTARGET_H_

#include "QGL.h"
#include "CharacterControl.h"
#include "AnimatedEntity.h"

enum TARGET_POPUP_COMMAND
{
	TARGET_POPUP_COMMAND_JOIN_PARTY = 0,
	TARGET_POPUP_COMMAND_INVITE_PARTY,
	TARGET_POPUP_COMMAND_VIEW_INFO,
	TARGET_POPUP_COMMAND_PRIVATE_CALL,
	TARGET_POPUP_COMMAND_INVITE_FRIEND,
	TARGET_POPUP_COMMAND_COPY_NAME,
	TARGET_POPUP_COMMAND_ADD_BLOCK_LIST,
	TARGET_POPUP_COMMAND_CALL_GM,
	MAX_TARGET_POPUP_COMMAND,
};

class CMessageCharacter;
class CMenuTargetProcess
{
public:
	CMenuTargetProcess();
	virtual ~CMenuTargetProcess();

	VOID ShowTargetDetail(CCharacterControl * TargetCtrl);

	VOID ShowDetail();
	VOID PopUpFunctionCall();

	VOID ShowTargetPopUp();

	void HideTargetMenu();

	void SetFriendDetail(CMessageCharacter * pInfo);
	void SetFriendDetail(CCharacterControl * pInfo);

	//target Info
	UInt32 m_char_id;
	String m_char_name;
	String m_About;
	UInt8Vector m_relations;
	bool m_enablePK;
	String m_title;
	UInt16 m_level;
	UInt8	m_sexClass;
	String	m_guild;
	String	m_gang;
	UInt16	m_friendly;
	UInt8	m_faction;
	bool	m_hasParty;
	UInt32	m_guild_uid;
	String	m_guild_name;

	

	CAnimatedEntity m_Entity;

	StringA	m_TargetPopUpCommand[MAX_TARGET_POPUP_COMMAND];

	StlVector<StringA> m_CurrTextInPopUp;

	void UpdateInfo();
	void FindCharNameFriend(PCSTRINGW Name);
	void PrivateMsg();
	void AddToFriend();
	void ChangeGrp();
	void BreakRelation();
	void BlackList();
	bool IsOpenDetail();
	void ResetTargetDetail();

};




#endif 