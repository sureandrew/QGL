#pragma once
#ifndef _MENUFRIEND_H_
#define _MENUFRIEND_H_

#include "QGL.h"
#include "common_binary.h"

class CMessenger;
class CMessageCharacter;
#define MAX_FRIEND_IN_ONE_PAGE	7

struct MessengerKey
{
	BYTE state;
	UInt16 friendly;
	MessengerKey()
	{
		friendly = 0;
		state = 0;
	}
	bool operator < (const MessengerKey& b) const
	{
		return (state < b.state || (state == b.state && friendly > b.friendly));
	}
};

struct MessageString
{
	StringVector MsgLst;
	String SrcName; 
	String TargetName;
	
	String TargetCharName;
	UInt32 TargetCharID;

	UInt8 type;
	
	MessageString()
	{
		type = 0;
	}
	~MessageString()
	{
		for (Int i = 0; i < (Int)MsgLst.size(); i ++)
		{
			MsgLst[i].Empty();
		}
		MsgLst.clear();
		SrcName.Empty();
		TargetName.Empty();
		TargetCharName.Empty();
	}
};

struct tempFriendData
{
	String name;
	BYTE state;
	tempFriendData()
	{
		state = 0;
	}
	~tempFriendData()
	{
		name.Empty();
	}
};

typedef StlMap<UInt32, tempFriendData, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, tempFriendData> > > CharTempFriendDataMap;
typedef StlMap<UInt32, RPCNetID, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, RPCNetID> > > CharMsgCharPtrMap;

class CMenuFriendProcess
{
public:
	UInt32 m_TargetCharID;
	String m_TargetCharName;

	NetGroup* m_Messengers;

	UInt16	m_StartIndex;
	UInt8	m_CurrGroup;

	CMessenger * m_pTargetMessager;
	UInt8	m_ChangeGroup;
	UInt32 m_changeGrpTempID;
	
	UInt32 m_TargetTalkingCharID;
	String m_TargetTalkingCharName;

	String m_OpenFileName;

	CharMsgCharPtrMap m_MessageCharacters;
	StlVector<MessageString> m_nextMessageQue;
	std::multimap<MessengerKey, CMessenger *> m_MessengersList[MAX_FRIEND_GROUP_TYPE];

	CharTempFriendDataMap m_TempFriendList;

	CMessageCharacter * m_pOwnChar;


	bool	m_bInit;

public:
	CMenuFriendProcess();
	virtual ~CMenuFriendProcess();

	void AddFriend(UInt8 group);
	void ConstructData();
	void RefreshMenu();
	void PageUp();
	void PageDown();
	void ChangeGroup(Int index);

	inline UInt16 GetStartIndex(){return m_StartIndex;}

	void OpenFriendChat(int index);

	void SendMessageToFriend();

	void PrintMessage(String Msg, String SrcName, String TargetName);

	void RefreshSelfSetting();

	void WriteHistory(String Msg, String MyName, String TargetName, String speaker);
	

	void OpenHistory();
	
	void FindFriend(PCSTRINGW char_id, PCSTRINGW name);

	void RefreshChatMenu();

	void RefreshPopUp(int memInx);
	void MoveGroup(int memInx, int grpIdx);

	void MoveGroupImm();

	inline void SetChangeGroup(int index){m_ChangeGroup = (UInt8)index;}

	CMessageCharacter * FindInfo(UInt32 char_id);
	CMessageCharacter * FindInfo(String char_name);
	CMessenger * FindMessenger(UInt32 char_id);

	void OpenFriendDetail(int index);

	void HideAllFriend();

	
	void SetMoveGroupTarget(UInt32 targetID, UInt8 grpIndex);
	void CheckChangeGroup();

	void ConfirmChangeToGroup(int grpIndex);

	void ConfirmSettingChange();

	void NextChatMessage();

	void PushNextMessage(String Msg, String TargetName, String SrcName, String TargetCharName, UInt32 TargetCharID, UInt8 type);

	inline bool HasNextMessage(){return m_nextMessageQue.size() > 0; }

	void FriendScroll();
	void FriendChatScroll();
	void FriendHisScroll();

	void ConfirmDelete();

	void RemoveMessenger(const UInt32 &owner_id, const UInt32 &target_id);

	inline void ResetTalkingTarget(){
		m_TargetTalkingCharID = 0;
		m_TargetTalkingCharName = _T("");
	}

	void ClearHistory();

	void InserTempFriend(UInt32 char_id, String NickName, BYTE state, bool force = false);

	void SetInfo(UInt32 friend_char_id, int index, UINT16 relation,UInt friendly, String charName, BYTE state);
	bool IsCurrMsgIcon(UInt iconID);

	void OpenTalkingFriendDetail();

	void ToPageTop();

	void FindOwnCharacter();
	
	
};




#endif 
