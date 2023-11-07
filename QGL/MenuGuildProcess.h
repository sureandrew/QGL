#pragma once
#ifndef _MENUGUILDPROCESS_H_
#define _MENUGUILDPROCESS_H_

#include "Common.h"

class CGuild;
class CGuildMember;
class CGuildApplicant;

struct memberOrder
{
	UInt16 level;
	UInt32 donate;
	UInt32 totalDonate;
	UInt8 jobClass;
	UInt8 post;
	UInt32 joinDate;
	UInt32 offlineTime;
	bool online;

	memberOrder()
	{
		level = 0;
		donate = 0;
		totalDonate = 0;
		jobClass = 0;
		post = 0;
		joinDate = 0;
		offlineTime = 0;
		online = true;
	}
	bool operator >= (const memberOrder& b) const
	{		
		bool result = offlineTime > b.offlineTime;
		bool checkContinue = offlineTime == b.offlineTime;

		if (checkContinue)
		{
			result = joinDate < b.joinDate;
			checkContinue = joinDate == b.joinDate;
		}

		if (checkContinue)
		{
			result = donate > b.donate;
			checkContinue = donate == b.donate;
		}

		if (checkContinue)
		{
			result = totalDonate > b.totalDonate;
			checkContinue = totalDonate == b.totalDonate;
		}

		if (checkContinue)
		{
			result = post < b.post;
			checkContinue = post == b.post;
		}

		if (checkContinue)
		{
			result = jobClass < b.jobClass;
			checkContinue = jobClass == b.jobClass;
		}

		if (checkContinue)
		{
			result = (!b.online && online);
			checkContinue = online == b.online;
		}

		if (checkContinue)
		{
			result = level > b.level;
		}

		return result;
	}
};
class CMenuGuildProcess
{
public:
	CMenuGuildProcess();
	~CMenuGuildProcess();

	StlMap<UInt32, CGuild*> m_guildMap;

	//only own member
	StlMap<UInt32, CGuildMember *>	m_guildMemberMap;
	typedef StlMultiMap<memberOrder, CGuildMember *, std::greater_equal<memberOrder>, 
		  ManagedAllocator<std::pair<memberOrder, CGuildMember *> > > GuildMemSortMap;
	GuildMemSortMap	m_guildMemberOrder;
	//only own applicant
	StlMap<UInt32, CGuildApplicant *>	m_guildApplicantMap;
	StlMultiMap<UInt32, CGuildApplicant *>	m_guildApplicantDayOrder;

	CGuild*		m_ownGuild;
	CGuildMember*		m_ownGuildMember;

	UInt	m_StartIndex;
	UInt	m_curIndex;
	UInt	m_applyStartIndex;
	UInt	m_curApplyIndex;
	UInt	m_businessLogStartIndex;
	UInt	m_curBusinessLogIndex;
	

	UInt	m_guildPostSelIndex;
	String	m_kickReason;
	void RefreshInfoMenu();
	void RefreshMemberMenu();
	void RefreshRuleMenu();
	void RefreshApplicantMenu();
	void RefreshManorMenu();

	void RequestMemberInfo();
	void RequestApplicantInfo();
	void RequestGuildInfo();

	void RefreshGuildAimEdit();
	void RefreshGuildRuleEdit();
	void ConfirmGuildAimChange();
	void ConfirmGuildRuleChange();
	void RefreshGuildApplyPanel();

	void RefreshPostMenu();

	void ConstructMemberOrder();
	void ConstructApplicantOrder();

	void SetGuildGrp(NetGroup * pGuildGrp);
	void SetGuildMemberGrp(NetGroup * pGuildMemberGrp);
	void SetGuildApplicantGrp(NetGroup * pGuildApplicantGrp);

	void ShowAim(int index);
	void SetMenuIndex(int index);
	void SetGuildPostSelectIndex(int index);
	void ApplyConfirm();
	void AddGuildMember();
	void RejectGuildMember();
	void DeleteGuildMember();
	void ShowInfo();

	void FindOwnGuild();

	void RefreshGuildButton();
	INLINE bool HasGuild(){return m_ownGuild != NULL && m_ownGuildMember != NULL;}

	void ConfirmPostChange();
	void ConfirmDonate();

	void ConfirmGuildNameInput();
	void CancelGuildNameInput();

	void InsertGuildMember(CGuildMember * pMember);
	void InsertGuildApplicant(CGuildApplicant * pApplicant);
	void RemoveApplicant(UInt32 char_id);
	bool IsSelectedMember(bool member);
	bool GetSelectedMember(int index);
	void RemoveMember(UInt32 char_id, String reason);

	CGuild * FindGuild(UInt32 guild_uid);

	void ScrollMember();
	void MemberPageUp();
	void MemberPageDown();

	void ScrollGuildApply();
	void GuildApplyPageUp();
	void GuildApplyPageDown();

	void LeaveGuild();
	void RemoveAllMember();

	void FreeGuild();

	void PositionChange(UInt32 src, UInt32 target, UInt8 position);

	void SortMember(int i);

	void SetGuildUIText(PCSTRINGA name, String &text);

	memberOrder GetSortData(UInt method, CGuildMember * pMember);

	UInt m_sortMethod;

	void RefreshGuildBusinessLogPanel();
	void ScrollGuildBusinessLog();
	void GuildBusinessLogPageUp();
	void GuildBusinessLogPageDown();
	void ConfirmGuildBusinessLogSearchYes();
	void SetGuildBusinessLogSelectIndex(int index);
	void ClearGuildBusinessLog();
private:
	
	
};


#endif // _MENUSHOPPROCESS_H_
