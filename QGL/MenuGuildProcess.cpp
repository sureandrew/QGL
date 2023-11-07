//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "QGL.h"
//-- Self
#include "MenuGuildProcess.h"
//-- Library
#include "Global.h"
#include "QGLApplication.h"
#include "ScriptControl.h"
#include "CharacterControl.h"
#include "Resource/ClientResManager.h"
#include "Proxy/Logic/Character.h"
#include "Proxy/Logic/Guild.h"
#include "Proxy/Logic/GuildMember.h"
#include "Proxy/Logic/GuildApplicant.h"
#include "Proxy/Service/MessageService_proxy.h"
#include "UI.h"
#include "Common/CallLuaFunc.h"
#include "MenuHelpProcess.h"
#include "Resource/ResManor.h"


#define MAX_MEM_NUM	10
#define MAX_GUILD_NUM 60
#define MAX_FAIL_MANOR_MAINTAIN 200
const UInt16 GuildPostLimit[MAX_GUILD_POSITION] = {1, 1, 1, 1, 1, 4, 4, 10, 10, 4, 10000};
#define MAX_BUSINESS_LOG_NUM	8

enum GUILD_MENU_INDEX
{
	GUILD_MENU_INDEX_INFO = 0,
	GUILD_MENU_INDEX_MEMBER,
	GUILD_MENU_INDEX_RULE,
	GUILD_MENU_INDEX_APPLY,
	GUILD_MENU_INDEX_HOUSE,

};

//1 level, 2 character, 3 post, 4 donate, 5 join time, 6 offline time
enum GUILD_SORT_METHOD
{
	GUILD_SORT_METHOD_NIL = 0,
	GUILD_SORT_METHOD_LEVEL,
	GUILD_SORT_METHOD_SECCLASS,
	GUILD_SORT_METHOD_POST,
	GUILD_SORT_METHOD_DONATE,
	GUILD_SORT_METHOD_JOINTIME,
	GUILD_SORT_METHOD_OFFLINETIME,

};
CMenuGuildProcess::CMenuGuildProcess()
{
	m_ownGuild = NULL;
	m_ownGuildMember = NULL;
	m_StartIndex = 0;
	m_applyStartIndex = 0;
	m_businessLogStartIndex = 0;
	m_curIndex = 0;
	m_curIndex = 0;
	m_guildPostSelIndex = 0;
	m_curBusinessLogIndex = 0;
	m_sortMethod = GUILD_SORT_METHOD_POST;
}

CMenuGuildProcess::~CMenuGuildProcess()
{
	for (StlMap<UInt32, CGuild*>::iterator Itr = m_guildMap.begin(); Itr != m_guildMap.end(); ++Itr)
	{
		DeleteNetObject(Itr->second);
	}
	m_guildMap.clear();

	RemoveAllMember();

	for (StlMap<UInt32, CGuildApplicant*>::iterator Itr = m_guildApplicantMap.begin(); Itr != m_guildApplicantMap.end(); ++Itr)
	{
		DeleteNetObject(Itr->second);
	}
	m_guildApplicantMap.clear();

	m_kickReason.Empty();
}

void CMenuGuildProcess::SetGuildApplicantGrp(NetGroup * pGuildApplicantGrp)
{
	StlMap<UInt32, CGuildApplicant *> tempMap = m_guildApplicantMap;
	for (Int i = 0; i < pGuildApplicantGrp->GetCount(); i ++)
	{
		CGuildApplicant * pGuildApp = CastNetObject<CGuildApplicant>(pGuildApplicantGrp->GetChild(i));
		if (pGuildApp)
		{
			StlMap<UInt32, CGuildApplicant*>::iterator Itr = m_guildApplicantMap.find(pGuildApp->GetChar_id());
			if (Itr != m_guildApplicantMap.end())
			{
				if (Itr->second != pGuildApp)
				{
					DeleteNetObject(Itr->second);
					m_guildApplicantMap.erase(Itr);
				}

				tempMap.erase(tempMap.find(pGuildApp->GetChar_id()));
			}	
			m_guildApplicantMap.insert(std::make_pair(pGuildApp->GetChar_id(), pGuildApp));
		}
	}

	//delete the unused applicant
	for (StlMap<UInt32, CGuildApplicant *>::iterator ItrTemp = tempMap.begin();
		ItrTemp != tempMap.end(); ++ItrTemp)
	{
		StlMap<UInt32, CGuildApplicant*>::iterator Itr = m_guildApplicantMap.find(ItrTemp->second->GetChar_id());
		if (Itr != m_guildApplicantMap.end())
		{
			DeleteNetObject(Itr->second);
			m_guildApplicantMap.erase(Itr);
		}
	}


	ConstructApplicantOrder();
	
}

void CMenuGuildProcess::SetGuildGrp(NetGroup * pGuildGrp)
{
	for (Int i = 0; i < pGuildGrp->GetCount(); i ++)
	{
		CGuild * pGuild = CastNetObject<CGuild>(pGuildGrp->GetChild(i));
		if (pGuild)
		{
			if (m_ownGuildMember && m_ownGuildMember->GetGuild_uid() == pGuild->GetGuild_uid())
				m_ownGuild = pGuild;
			StlMap<UInt32, CGuild*>::iterator Itr = m_guildMap.find(pGuild->GetGuild_uid());
			if (Itr != m_guildMap.end())
			{
				if (Itr->second != pGuild)
				{
					DeleteNetObject(Itr->second);
					m_guildMap.erase(Itr);
				}
			}	
			m_guildMap.insert(std::make_pair(pGuild->GetGuild_uid(), pGuild));
		}
	}
	RefreshGuildApplyPanel();
}

void CMenuGuildProcess::FindOwnGuild()
{
	if (gGlobal.g_pCharacter)
	{
		for (StlMap<UInt32, CGuildMember *>::iterator Itr = m_guildMemberMap.begin();
			Itr != m_guildMemberMap.end(); ++Itr)
		{
			if (Itr->second->GetChar_id() == gGlobal.g_pCharacter->GetMapActorID())
			{
				m_ownGuildMember = Itr->second;
				StlMap<UInt32, CGuild*>::iterator Itr = m_guildMap.find(m_ownGuildMember->GetGuild_uid());
				if (Itr != m_guildMap.end())
					m_ownGuild = Itr->second;
			}
		}
	}
}
void CMenuGuildProcess::ConstructApplicantOrder()
{
	m_guildApplicantDayOrder.clear();
	//sort the list
	for (StlMap<UInt32, CGuildApplicant *>::iterator Itr = m_guildApplicantMap.begin(); 
		Itr != m_guildApplicantMap.end(); ++Itr)
	{
		m_guildApplicantDayOrder.insert(std::make_pair(Itr->second->GetApplyDate() * -1, Itr->second));
	}
	UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildApplicantCheck");

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("GuildMemInfoScrollBar");
	if (scrollBar)
	{
		scrollBar->SetNumCount((UInt32)m_guildApplicantMap.size() + 1);
	}
	
	if (pchkBox && pchkBox->GetState())
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "GuildApplicantCheckLua", String());
}

void CMenuGuildProcess::ScrollGuildApply()
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("GuildApplyScrollBar");
	if (scrollBar)
	{
		UInt index = scrollBar->GetSelNum();
		if (index + MAX_MEM_NUM > m_guildMap.size())
		{
			if (m_guildMap.size() < MAX_MEM_NUM)
				m_applyStartIndex = 0;
			else
				m_applyStartIndex = (UInt32)m_guildMap.size() - MAX_MEM_NUM;
		}
		else 
			m_applyStartIndex = index;

		RefreshGuildApplyPanel();
	}

}
void CMenuGuildProcess::GuildApplyPageUp()
{
	if (m_applyStartIndex > 0)
	{
		m_applyStartIndex -- ;
		RefreshGuildApplyPanel();
	}
}
void CMenuGuildProcess::GuildApplyPageDown()
{
	if (m_applyStartIndex  + MAX_MEM_NUM < m_guildMap.size())
	{
		m_applyStartIndex ++;
		RefreshGuildApplyPanel();
	}
}
void CMenuGuildProcess::ScrollMember()
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("GuildMemInfoScrollBar");
	if (scrollBar)
	{
		UInt index = scrollBar->GetSelNum();
		UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildApplicantCheck");
		if (pchkBox && pchkBox->GetState())
		{
			if (index > m_guildApplicantDayOrder.size() - MAX_MEM_NUM )
			{
				if (m_guildApplicantDayOrder.size() < MAX_MEM_NUM)
					m_StartIndex = 0;
				else
					m_StartIndex = (UInt32)m_guildApplicantDayOrder.size() - MAX_MEM_NUM;
			}
			else 
				m_StartIndex = index;

			RefreshApplicantMenu();
		}

		pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildMemberCheck");
		if (pchkBox && pchkBox->GetState())
		{
			if (index > m_guildMemberOrder.size() - MAX_MEM_NUM )
			{
				if (m_guildMemberOrder.size() < MAX_MEM_NUM)
					m_StartIndex = 0;
				else
					m_StartIndex = (UInt32)m_guildMemberOrder.size() - MAX_MEM_NUM;
			}
			else 
				m_StartIndex = index;
			

			RefreshMemberMenu();
		}
	}
}
void CMenuGuildProcess::MemberPageUp()
{
	if (m_StartIndex > 0)
	{
		m_StartIndex -- ;
		UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildApplicantCheck");
		if (pchkBox && pchkBox->GetState())
		{
			RefreshApplicantMenu();
		}

		pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildMemberCheck");
		if (pchkBox && pchkBox->GetState())
		{
			RefreshMemberMenu();
		}
	}
	
}
void CMenuGuildProcess::MemberPageDown()
{
	
		UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildApplicantCheck");
		if (pchkBox && pchkBox->GetState())
		{
			if (m_StartIndex  + MAX_MEM_NUM < m_guildApplicantDayOrder.size())
			{
				m_StartIndex ++;
				RefreshApplicantMenu();
			}
		}

		pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildMemberCheck");
		if (pchkBox && pchkBox->GetState())
		{
			if (m_StartIndex  + MAX_MEM_NUM < m_guildMemberOrder.size())
			{
				m_StartIndex ++;
				RefreshMemberMenu();
			}
		}
	
}
void CMenuGuildProcess::ConstructMemberOrder()
{
	m_guildMemberOrder.clear();
	//sort the list
	for (StlMap<UInt32, CGuildMember *>::iterator Itr = m_guildMemberMap.begin(); 
		Itr != m_guildMemberMap.end(); ++Itr)
	{		
		//sort.
		m_guildMemberOrder.insert(std::make_pair(GetSortData(m_sortMethod, Itr->second), Itr->second));
	}

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("GuildMemInfoScrollBar");
	if (scrollBar)
	{
		scrollBar->SetNumCount((UInt32)m_guildMemberMap.size() + 1);
	}
	UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildMemberCheck");
	if (pchkBox && pchkBox->GetState())
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "GuildMemberCheckLua", String());
}

void CMenuGuildProcess::RemoveAllMember()
{
	for (StlMap<UInt32, CGuildMember*>::iterator Itr = m_guildMemberMap.begin(); Itr != m_guildMemberMap.end(); ++Itr)
	{
		DeleteNetObject(Itr->second);
	}
	m_guildMemberMap.clear();
}

memberOrder CMenuGuildProcess::GetSortData(UInt method, CGuildMember * pMember)
{
	memberOrder sort;
	if (pMember)
	{
		sort.level = pMember->GetLevel();
		if (method == GUILD_SORT_METHOD_LEVEL)
		{					
			sort.online = pMember->GetOfflineTime() == 0;
		}
		else if (method == GUILD_SORT_METHOD_SECCLASS)
		{
			sort.jobClass = pMember->GetSexClass();
		}
		else if (method == GUILD_SORT_METHOD_POST)
		{
			sort.post = pMember->GetPost();
		}
		else if (method == GUILD_SORT_METHOD_DONATE)
		{
			sort.post = pMember->GetPost();
			sort.donate = pMember->GetDonate();
			sort.totalDonate = pMember->GetTotalDonate();
		}
		else if (method == GUILD_SORT_METHOD_JOINTIME)
		{
			sort.joinDate = pMember->GetJoinDate();
		}
		else if (method == GUILD_SORT_METHOD_OFFLINETIME)
		{
			sort.post = pMember->GetPost();
			sort.offlineTime = pMember->GetOfflineTime();
			if (sort.offlineTime == 0)
				sort.offlineTime = (UInt32)-1;
		}
	}
	return sort;
}
void CMenuGuildProcess::SortMember(int i)
{
	if (i == 0)
		return;

	m_guildMemberOrder.clear();
	for (StlMap<UInt32, CGuildMember *>::iterator Itr = m_guildMemberMap.begin();
		Itr != m_guildMemberMap.end(); Itr ++)
	{
		m_guildMemberOrder.insert(std::make_pair(GetSortData(i, Itr->second), 	Itr->second));
	}

	m_sortMethod = i;

	RefreshMemberMenu();
}
void CMenuGuildProcess::RemoveMember(UInt32 char_id, String reason)
{
	StlMap<UInt32, CGuildMember *>::iterator Itr = m_guildMemberMap.find(char_id);
	if (Itr != m_guildMemberMap.end())
	{
		if (Itr->second == m_ownGuildMember)
		{
			m_ownGuildMember = NULL;
			m_ownGuild = NULL;
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseGuildPanel", String());
			RemoveAllMember();
			if (reason.GetLength() > 0)
			{
                String text;
				text.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_KICK_BYOTHER_OWN_MSG")),reason.c_str());
				gGlobal.PrintMessage(text, CHANNELTYPE_GUILD);
			}
			else
                gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_KICK_BYSELF_OWN_MSG")), CHANNELTYPE_SYSTEM);
			return;
		}
		else if (reason.GetLength() > 0)
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_KICK_BYOTHER_OTHER_MSG")), Itr->second->GetName().c_str(), reason.c_str());
			gGlobal.PrintMessage(text, CHANNELTYPE_GUILD);
		}
		else
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_KICK_BYSELF_OTHER_MSG")), Itr->second->GetName().c_str());
			gGlobal.PrintMessage(text, CHANNELTYPE_GUILD);
		}
		DeleteNetObject(Itr->second);
		m_guildMemberMap.erase(Itr);
	}
	ConstructMemberOrder();
}
void CMenuGuildProcess::RemoveApplicant(UInt32 char_id)
{

	StlMap<UInt32, CGuildApplicant *>::iterator Itr = m_guildApplicantMap.find(char_id);
	if (Itr != m_guildApplicantMap.end())
	{
		DeleteNetObject(Itr->second);
		m_guildApplicantMap.erase(Itr);
	}
	ConstructApplicantOrder();
}

void CMenuGuildProcess::InsertGuildApplicant(CGuildApplicant * pApplicant)
{
	m_guildApplicantMap.insert(std::make_pair(pApplicant->GetChar_id(), pApplicant));
	ConstructApplicantOrder();
}
void CMenuGuildProcess::InsertGuildMember(CGuildMember * pMember)
{
	if (gGlobal.g_pCharacter)
	{
		if (pMember->GetChar_id() == gGlobal.g_pCharacter->GetMapActorID())
		{
			m_ownGuildMember = pMember;
			gGlobal.g_pMenuHelpProcess->HandleEvent(HELP_TIPS_FIRST_JOINGUILD);
			StlMap<UInt32, CGuild*>::iterator Itr = m_guildMap.find(m_ownGuildMember->GetGuild_uid());
			if (Itr != m_guildMap.end())
				m_ownGuild = Itr->second;
		}
	}	

	m_guildMemberMap.insert(std::make_pair(pMember->GetChar_id(), pMember));

	ConstructMemberOrder();
	
}

void CMenuGuildProcess::SetGuildMemberGrp(NetGroup * pGuildMemberGrp)
{
	//StlMap<UInt32, CGuildMember *> tempMap = m_guildMemberMap;

	for (Int i = 0; i < pGuildMemberGrp->GetCount(); i ++)
	{
		CGuildMember * pGuildMember = CastNetObject<CGuildMember>(pGuildMemberGrp->GetChild(i));
		if (pGuildMember)
		{
			StlMap<UInt32, CGuildMember*>::iterator Itr = 
				m_guildMemberMap.find(pGuildMember->GetChar_id());
			//if (Itr != m_guildMemberMap.end())
			//{
			//	if (Itr->second != pGuildMember)
			//	{
			//		DeleteNetObject(Itr->second);
			//		m_guildMemberMap.erase(Itr);
			//	}

			//	//tempMap.erase(tempMap.find(pGuildMember->GetChar_id()));
			//}		
			if (Itr == m_guildMemberMap.end())
			{
				m_guildMemberMap.insert(std::make_pair(pGuildMember->GetChar_id(), pGuildMember));
				if (gGlobal.g_pCharacter)
				{
					if (pGuildMember->GetChar_id() == gGlobal.g_pCharacter->GetMapActorID())
					{
						m_ownGuildMember = pGuildMember;
						StlMap<UInt32, CGuild*>::iterator Itr = 
							m_guildMap.find(m_ownGuildMember->GetGuild_uid());
						if (Itr != m_guildMap.end())
							m_ownGuild = Itr->second;
					}
				}
			}
		}
	}

	//delete the unused member
	//for (StlMap<UInt32, CGuildMember *>::iterator ItrTemp = tempMap.begin();
	//	ItrTemp != tempMap.end(); ++ItrTemp)
	//{
	//	StlMap<UInt32, CGuildMember*>::iterator Itr = m_guildMemberMap.find(ItrTemp->second->GetChar_id());
	//	if (Itr != m_guildMemberMap.end())
	//	{
	//		DeleteNetObject(Itr->second);
	//		m_guildMemberMap.erase(Itr);
	//	}
	//}
	ConstructMemberOrder();
}

void CMenuGuildProcess::RequestGuildInfo()
{
	
}

void CMenuGuildProcess::RequestMemberInfo()
{
	
}
void CMenuGuildProcess::RequestApplicantInfo()
{
	
}

void CMenuGuildProcess::RefreshPostMenu()
{
	UInt16 counter[MAX_GUILD_POSITION];
	ZeroMemory(counter, sizeof(UInt16) * MAX_GUILD_POSITION);

	for (StlMap<UInt32, CGuildMember *>::iterator Itr = m_guildMemberMap.begin();
		Itr != m_guildMemberMap.end(); ++ Itr)
	{
		if (Itr->second->GetPost() < MAX_GUILD_POSITION)
			counter[Itr->second->GetPost()] ++;
	}
	StringA name;
	
	for (UInt i = GUILD_POSITION_MASTER; i < GUILD_POSITION_MEMBER; i ++)
	{
		name.Format("GuildPostName%d", i);
		UIStaticText * pText = NULL;			
		pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		if (pText)
		{
			String name;
			name.Format(_T("MSG_GUILD_POSITION_%d"), i);

			String fullText;
			fullText.Format(_T("%s(%d/%d)"), gGlobal.GetStringTable()->Get(name.c_str()), counter[i], GuildPostLimit[i]);
			pText->SetText(fullText.c_str());
			pText->Redraw();
		}
	}
}

void CMenuGuildProcess::RefreshMemberMenu()
{
	UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildMemberCheck");
	if (pchkBox && !pchkBox->GetState())
		return;

	CGuildMember* pMember;
	UInt i = 0;
	for (GuildMemSortMap::iterator Itr = m_guildMemberOrder.begin();
		Itr != m_guildMemberOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
	{
		pMember = Itr->second;
		if (i >= m_StartIndex && pMember)
		{
			Color32 givColor = TextColor[ColorIndex_White];
			if (pMember->GetOfflineTime() == 0)
				givColor = TextColor[ColorIndex_Green];
			
			StringA name;
			UIStaticText * pText = NULL;
			name.Format("GuildMem%dName", i + 1 - m_StartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				pText->SetText(pMember->GetName().c_str());
				
				pText->SetColor(givColor);
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildMem%dLevel", i + 1 - m_StartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				String text;
				if (pMember->IsActive())
					text.Format(_T("%d"), pMember->GetLevel());
				else
					text = _T("?");
				pText->SetText(text.c_str());
				pText->SetColor(givColor);
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildMem%dClass", i + 1 - m_StartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				String text;
				if (pMember->IsActive())
				{
					String tempClassName;
					tempClassName.Format(_T("MSG_ACTOR_CLASS_%d"), pMember->GetSexClass());
					text = GetLocalStr(tempClassName.c_str());
				}
				else
					text = _T("?");
				pText->SetText(text.c_str());
				pText->SetColor(givColor);
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildMem%dPosition", i + 1 - m_StartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				String name;
				name.Format(_T("MSG_GUILD_POSITION_%d"), pMember->GetPost());
				pText->SetText(GetLocalStr(name.c_str()));
				pText->SetColor(givColor);
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildMem%dDonation", i + 1 - m_StartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				String text;
				text.Format(_T("%d/%d"), pMember->GetDonate(), pMember->GetTotalDonate());				
				pText->SetText(text.c_str());
				pText->SetColor(givColor);
				pText->Redraw();
				pText->SetEnable(TRUE);
			}
			
			name.Format("GuildMem%dJoinTime", i + 1 - m_StartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				struct tm * dayTime;
				time_t day = pMember->GetJoinDate();
				dayTime = localtime(&day);
				String text;
				text.Format(_T("%d-%d-%d"), dayTime->tm_year + 1900 , dayTime->tm_mon + 1, dayTime->tm_mday);
				pText->SetText(text.c_str());
				pText->SetColor(givColor);
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildMem%dOfflineTime", i + 1 - m_StartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				time_t now = time(NULL);
				UInt32 diff = (UInt32)(now - pMember->GetOfflineTime());
				String text;
			
				if (!pMember->IsActive())
					text = _T("?");
				else if (pMember->GetOfflineTime() == 0)
					text =GetLocalStr(_T("MSG_ONLINE"));
				else if (diff >= 86400)
					text.Format(_T("%d%s"), diff / 86400, GetLocalStr(_T("MSG_DAY")));
				else if (diff >= 3600)
					text.Format(_T("%d%s"), diff / 3600, GetLocalStr(_T("MSG_HOUR")));
				else
					text.Format(_T("<1%s"), GetLocalStr(_T("MSG_HOUR")));
				
				pText->SetColor(givColor);
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}
		}
	}

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("GuildMemInfoScrollBar");
	if (scrollBar)
	{
		scrollBar->SetNumCount((UInt32)m_guildMemberMap.size());
	}
}

void CMenuGuildProcess::RefreshGuildAimEdit()
{

	if (m_ownGuild)
	{
		UIEditBoxEx * pEdit = gGlobal.pUIManager->FindUIEditBoxEx("MenuGuildAimEditLine");
		if (pEdit)
		{
			pEdit->SetText(m_ownGuild->GetAim().c_str());
			pEdit->Redraw();
		}
		else
		{
			UIChatEditPanel *pEdit = gGlobal.pUIManager->FindUIChatEditPanel("MenuGuildAimEditLine");

			if(pEdit)
			{
				PCSTRING	StringPtr = m_ownGuild->GetAim().c_str();
				Int			Len = m_ownGuild->GetAim().size();
				String	MyString;

				for(Int i = 0; i < m_ownGuild->GetAim().size(); ++i)
				{
					if(StringPtr[Len - 1] == 32)
						--Len;
					else
						break;
				}

				MyString = m_ownGuild->GetAim().substr(0, Len);
		
				pEdit->LoadText(MyString.c_str());
				pEdit->FillImage();
				pEdit->Redraw();
			}
		}
	}
}
void CMenuGuildProcess::ConfirmGuildAimChange()
{
	UIEditBoxEx * pEdit = gGlobal.pUIManager->FindUIEditBoxEx("MenuGuildAimEditLine");
	if (pEdit)
	{
		m_ownGuildMember->ChangeAim(gGlobal.pPeerProxy, RPCContext(), pEdit->GetText());
	}
	else
	{
		UIChatEditPanel *pEdit = gGlobal.pUIManager->FindUIChatEditPanel("MenuGuildAimEditLine");

		pEdit->SaveText();
		m_ownGuildMember->ChangeAim(gGlobal.pPeerProxy, RPCContext(), pEdit->GetText());
	}
}
void CMenuGuildProcess::RefreshGuildRuleEdit()
{
	if (m_ownGuild)
	{
		UIEditBoxEx * pEdit = gGlobal.pUIManager->FindUIEditBoxEx("MenuGuildRuleEditLine");
		if (pEdit)
		{
			pEdit->SetText(m_ownGuild->GetRule().c_str());
			pEdit->Redraw();
		}
		else
		{
			UIChatEditPanel *pEdit = gGlobal.pUIManager->FindUIChatEditPanel("MenuGuildRuleEditLine");

			if(pEdit)
			{
				PCSTRING	StringPtr = m_ownGuild->GetRule().c_str();
				Int			Len = m_ownGuild->GetRule().size();
				String	MyString;

				for(Int i = 0; i < m_ownGuild->GetRule().size(); ++i)
				{
					if(StringPtr[Len - 1] == 32)
						--Len;
					else
						break;
				}

				MyString = m_ownGuild->GetRule().substr(0, Len);

			
				pEdit->LoadText(MyString.c_str());
				pEdit->FillImage();
				pEdit->Redraw();
			}
		}
	}
}
void CMenuGuildProcess::ConfirmGuildRuleChange()
{
	UIEditBoxEx * pEdit = gGlobal.pUIManager->FindUIEditBoxEx("MenuGuildRuleEditLine");
	if (pEdit)
	{
		m_ownGuildMember->ChangeRule(gGlobal.pPeerProxy, RPCContext(), pEdit->GetText());
	}
	else
	{
		UIChatEditPanel *pEdit = gGlobal.pUIManager->FindUIChatEditPanel("MenuGuildRuleEditLine");

		pEdit->SaveText();
		m_ownGuildMember->ChangeRule(gGlobal.pPeerProxy, RPCContext(), pEdit->GetText());
	}
}
void CMenuGuildProcess::RefreshRuleMenu()
{
	UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildRuleCheck");
	if (pchkBox && !pchkBox->GetState())
		return;

	if (m_ownGuild)
	{
		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("GuildRuleChat");
		if (pText)
		{
			pText->SetText(m_ownGuild->GetRule().c_str());
			pText->Redraw();
		}
		else
		{
			UIChatPanel *pChatPanel = gGlobal.pUIManager->FindUIChatPanel("GuildRuleChat");

			if(pChatPanel)
			{
				PCSTRING	StringPtr = m_ownGuild->GetRule().c_str();
				Int			Len = m_ownGuild->GetRule().size();
				String	MyString;

				for(Int i = 0; i < m_ownGuild->GetRule().size(); ++i)
				{
					if(StringPtr[Len - 1] == 32)
						--Len;
					else
						break;
				}

				MyString = m_ownGuild->GetRule().substr(0, Len);

				/*
				for(i = 0; i < Len; ++i)
				{
					if(StringPtr[i] == '\r')
						MyString.append(_T("\\r"));
					else
						MyString.push_back(StringPtr[i]);
				}
				*/
				pChatPanel->ClearAllRow();
				pChatPanel->AddStringW(MyString.c_str());
				pChatPanel->HomeRow();
				pChatPanel->Redraw();
			}
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildAimChat");
		if (pText)
		{
			pText->SetText(m_ownGuild->GetAim().c_str());
			pText->Redraw();
		}
		else
		{
			UIChatPanel *pChatPanel = gGlobal.pUIManager->FindUIChatPanel("GuildAimChat");

			if(pChatPanel)
			{
				PCSTRING	StringPtr = m_ownGuild->GetAim().c_str();
				Int			Len = m_ownGuild->GetAim().size();
				String	MyString;

				for(Int i = 0; i < m_ownGuild->GetAim().size(); ++i)
				{
					if(StringPtr[Len - 1] == 32)
						--Len;
					else
						break;
				}
				MyString = m_ownGuild->GetAim().substr(0, Len);

				/*
				for(i = 0; i < Len; ++i)
				{
					if(StringPtr[i] == '\r')
						MyString.append(_T("\\r"));
					else
						MyString.push_back(StringPtr[i]);
				}
				*/
				pChatPanel->ClearAllRow();
				pChatPanel->AddStringW(MyString.c_str());
				pChatPanel->HomeRow();
				pChatPanel->Redraw();
			}
		}
	}
}

void CMenuGuildProcess::FreeGuild()
{
	StlMap<UInt32, CGuild *>::iterator Itr = m_guildMap.begin();
	while (Itr != m_guildMap.end())
	{
		if (Itr->second == m_ownGuild)
			++Itr;
		else
		{
			DeleteNetObject(Itr->second);
			Itr = m_guildMap.erase(Itr);
		}
	}
	
}

void CMenuGuildProcess::ShowAim(int index)
{
	m_curApplyIndex = index;
	UInt i = 0;
	for (StlMap<UInt32, CGuild *>::iterator Itr = m_guildMap.begin();
		Itr != m_guildMap.end() && i < m_applyStartIndex + MAX_MEM_NUM; ++Itr, i++)
	{
		if (i == m_applyStartIndex + index)
		{			
			UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("GuildApplyAim");
			if (pText)
			{
				pText->SetText(Itr->second->GetAim().c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}			
			if (Itr->second->GetAim().IsEmpty())
				MessageService_Proxy::RequestGuildAim(gGlobal.pPeerProxy, RPCContext(), Itr->second->GetGuild_uid());
			break;
		}
	}
}
void CMenuGuildProcess::ApplyConfirm()
{
	UInt i = 0;
	for (StlMap<UInt32, CGuild *>::iterator Itr = m_guildMap.begin();
		Itr != m_guildMap.end() && i < m_applyStartIndex + MAX_MEM_NUM; ++Itr, i++)
	{
		if (i == m_applyStartIndex + m_curApplyIndex)
		{
			MessageService_Proxy::ApplyGuild(gGlobal.pPeerProxy, RPCContext(), gGlobal.g_pCharacter->GetMapActorID(), Itr->second->GetGuild_uid());
		}
	}
}



void CMenuGuildProcess::AddGuildMember()
{
	UInt i = 0;
	for (StlMultiMap<UInt32, CGuildApplicant *>::iterator Itr = m_guildApplicantDayOrder.begin();
		Itr != m_guildApplicantDayOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
	{
		if (i == m_StartIndex + m_curIndex)
		{
			if (m_ownGuildMember)
				m_ownGuildMember->ResponseApplicant(gGlobal.pPeerProxy, RPCContext(), Itr->second->GetChar_id(), true);
		}
	}
}

void CMenuGuildProcess::LeaveGuild()
{
	if (gGlobal.g_pCharacter->GetCharNetObject())
		gGlobal.g_pCharacter->GetCharNetObject()->KickMember(gGlobal.pPeerProxy, RPCContext(), m_ownGuildMember->GetChar_id(), String());
}
void CMenuGuildProcess::DeleteGuildMember()
{
	UInt i = 0;
	for (GuildMemSortMap::iterator Itr = m_guildMemberOrder.begin();
		Itr != m_guildMemberOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
	{
		if (i == m_StartIndex + m_curIndex)
		{
			if (gGlobal.g_pCharacter->GetCharNetObject())
				gGlobal.g_pCharacter->GetCharNetObject()->KickMember(gGlobal.pPeerProxy, RPCContext(), Itr->second->GetChar_id(), m_kickReason);
		}
	}
}
void CMenuGuildProcess::RejectGuildMember()
{
	UInt i = 0;
	for (StlMultiMap<UInt32, CGuildApplicant *>::iterator Itr = m_guildApplicantDayOrder.begin();
		Itr != m_guildApplicantDayOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
	{
		if (i == m_StartIndex + m_curIndex)
		{
			if (m_ownGuildMember)
				m_ownGuildMember->ResponseApplicant(gGlobal.pPeerProxy, RPCContext(), Itr->second->GetChar_id(), false);
		}
	}
}

void CMenuGuildProcess::SetMenuIndex(int index)
{
	m_curIndex = index;
}
void CMenuGuildProcess::RefreshGuildApplyPanel()
{
	UInt i = 0;
	for (StlMap<UInt32, CGuild *>::iterator Itr = m_guildMap.begin();
		Itr != m_guildMap.end() && i < m_applyStartIndex + MAX_MEM_NUM; ++Itr, i++)
	{
		if (i >= m_applyStartIndex)
		{
			StringA name;
			UIStaticText * pText = NULL;
			name.Format("GuildApplyName%d", i + 1 - m_applyStartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				pText->SetText(Itr->second->GetName().c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildApplyID%d", i + 1 - m_applyStartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				String text;
				text.Format(_T("%d"), Itr->second->GetGuild_uid());
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildApplyMaster%d", i + 1 - m_applyStartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				pText->SetText(Itr->second->GetMaster().c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildApplyMemberNum%d", i + 1 - m_applyStartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				String text;
				text.Format(_T("%d/%d"), Itr->second->GetTotalMember(), MAX_GUILD_NUM + 10 * Itr->second->GetRoom());
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}
		}
	}


	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("GuildApplyScrollBar");
	if (scrollBar)
	{
		scrollBar->SetNumCount((UInt32)m_guildMap.size());
	}

	ShowAim(m_curApplyIndex);
}

void CMenuGuildProcess::RefreshApplicantMenu()
{
	UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildApplicantCheck");
	if (pchkBox && !pchkBox->GetState())
		return;

	
	UInt i = 0;
	for (StlMultiMap<UInt32, CGuildApplicant *>::iterator Itr = m_guildApplicantDayOrder.begin();
		Itr != m_guildApplicantDayOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
	{
		if (i >= m_StartIndex)
		{
			StringA name;
			UIStaticText * pText = NULL;
			name.Format("GuildApply%dName", i + 1 - m_StartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				pText->SetText(Itr->second->GetName().c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildApply%dID", i + 1 - m_StartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				String text;
				text.Format(_T("%d"), Itr->second->GetChar_id());
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildApply%dLevel", i + 1 - m_StartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				String text;
				text.Format(_T("%d"), Itr->second->GetLevel());
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildApply%dDate", i + 1 - m_StartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				struct tm * dayTime;
				time_t day = Itr->second->GetApplyDate();
				dayTime = localtime(&day);
				String text;
				text.Format(_T("%d-%d-%d"), dayTime->tm_year + 1900 , dayTime->tm_mon + 1, dayTime->tm_mday);
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}
		}
	}

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("GuildMemInfoScrollBar");
	if (scrollBar)
	{
		scrollBar->SetNumCount((UInt32)m_guildApplicantMap.size());
	}
}
void CMenuGuildProcess::RefreshManorMenu()
{
	UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildManorCheck");
	if (pchkBox && !pchkBox->GetState())
		return;

	if (m_ownGuild && m_ownGuildMember && m_ownGuild->GetManorScale() > 0)
	{
		const ManorData* manorDat = gGlobal.m_resource->GetManorData(m_ownGuild->GetManorScale());
		if (!manorDat)
			return;
		UIImage* manorImg = gGlobal.pUIManager->FindUIImage("GuildManorIm");
		if (manorImg)
			manorImg->SetEnable(true);
		String text, temp;
		text.Format(_T("%d %s"), m_ownGuild->GetManorScale(), gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
		SetGuildUIText("GuildManorScale", text);
		temp.Format(_T("MSG_MANOR_AURA%d"), m_ownGuild->GetAura());
		text.Format(_T("%s"), gGlobal.GetStringTable()->Get(temp.c_str()));
		SetGuildUIText("GuildManorAura", text);
		temp.clear();
		temp.Format(_T("MSG_MANOR_TIGER%d"), manorDat->manor_tigerRuneLv);
		text.Format(_T("%s"), gGlobal.GetStringTable()->Get(temp.c_str())); 
		SetGuildUIText("GuildTigerLevel", text);
		text.Format(_T("%3.2f"), manorDat->manor_meditationLv);
		SetGuildUIText("GuildSitIndex", text);
		text.Format(_T("%d/%d"), m_ownGuild->GetMaintainNum(), manorDat->manor_maxMaintain);
		SetGuildUIText("GuildMaintainSuccess", text);
		text.Format(_T("%d"), manorDat->manor_maintainCost);
		SetGuildUIText("GuildMaintainFee", text);
		text.Format(_T("%d"), m_ownGuild->GetRoom());
		SetGuildUIText("GuildManorRoom", text);
		text.Format(_T("%d"), m_ownGuild->GetGuard());
		SetGuildUIText("GuildManorGuard", text);
		text.Format(_T("%d/%d"), m_ownGuild->GetFailPayNum(), MAX_FAIL_MANOR_MAINTAIN);
		SetGuildUIText("GuildMaintainFail", text);	
		text.Format(_T("%d %s"), m_ownGuild->GetTrainCharExPow(), gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
		SetGuildUIText("ManorCharExPow", text);
		text.Format(_T("%d %s"), m_ownGuild->GetTrainCharInPow(), gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
		SetGuildUIText("ManorCharInPow", text);
		text.Format(_T("%d %s"), m_ownGuild->GetTrainPartnerExPow(), gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
		SetGuildUIText("ManorPartnerExPow", text);
		text.Format(_T("%d %s"), m_ownGuild->GetTrainPartnerInPow(), gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
		SetGuildUIText("ManorPartnerInPow", text);
		text.Format(_T("%d %s"), m_ownGuild->GetHpLv(), gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
		SetGuildUIText("ManorHP", text);
		text.Format(_T("%d %s"), m_ownGuild->GetSpLv(), gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
		SetGuildUIText("ManorSP", text);
		text.Format(_T("%d %s"), m_ownGuild->GetDrugLv(), gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
		SetGuildUIText("ManorDrug", text);
		text.Format(_T("%d %s"), m_ownGuild->GetCookLv(), gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
		SetGuildUIText("ManorCook", text);
		text.Format(_T("%d %s"), m_ownGuild->GetWineLv(), gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
		SetGuildUIText("ManorWine", text);
		text.Format(_T("%d"), m_ownGuild->GetMaterial());
		SetGuildUIText("GuildMaterial", text);
		UIStaticText *pText = NULL;	
		pText = gGlobal.pUIManager->FindUIStaticText("GuildNoManor");
		if (pText)
			pText->SetEnable(false);	
	}
	else
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SetOnOffManor", String(), false);	
		UIStaticText *pText = NULL;		
        pText = gGlobal.pUIManager->FindUIStaticText("GuildNoManor");
		if (pText)
		{
			pText->SetEnable(true);
			String text;
			text.Format(_T("%s"), gGlobal.GetStringTable()->Get(_T("MSG_MANOR_NONE")));
			pText->SetText(text);
			pText->Redraw();
		}
	}
    	
}

bool CMenuGuildProcess::GetSelectedMember(int index)
{
	UInt i = 0;

	UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildMemberCheck");
	if (pchkBox && pchkBox->GetState())
	{
		for (GuildMemSortMap::iterator Itr = m_guildMemberOrder.begin();
			Itr != m_guildMemberOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
		{
			if (i == m_StartIndex + index)
			{
				return true;
			}
		}
	}

	pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildApplicantCheck");
	if (pchkBox && pchkBox->GetState())
	{
		for (StlMultiMap<UInt32, CGuildApplicant *>::iterator Itr = m_guildApplicantDayOrder.begin();
			Itr != m_guildApplicantDayOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
		{
			if (i == m_StartIndex + index)
			{
				return true;
			}
		}
	}
	return false;
}
bool CMenuGuildProcess::IsSelectedMember(bool member)
{
	UInt i = 0;

	if (member)
	{

		UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildMemberCheck");
		if (pchkBox && pchkBox->GetState())
		{
			for (GuildMemSortMap::iterator Itr = m_guildMemberOrder.begin();
				Itr != m_guildMemberOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
			{
				if (i == m_StartIndex + m_curIndex)
				{
					return true;
				}
			}
		}
	}
	else
	{

		UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildApplicantCheck");
		if (pchkBox && pchkBox->GetState())
		{
			for (StlMultiMap<UInt32, CGuildApplicant *>::iterator Itr = m_guildApplicantDayOrder.begin();
				Itr != m_guildApplicantDayOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
			{
				if (i == m_StartIndex + m_curIndex)
				{
					return true;
				}
			}
		}
	}
	return false;
}
void CMenuGuildProcess::ShowInfo()
{
	UInt i = 0;

	UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildMemberCheck");
	if (pchkBox && pchkBox->GetState())
	{
		for (GuildMemSortMap::iterator Itr = m_guildMemberOrder.begin();
			Itr != m_guildMemberOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
		{
			if (i == m_StartIndex + m_curIndex)
			{
				MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, RPCContext(), Itr->second->GetChar_id(), String(), false, true);
			}
		}
	}

	pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildApplicantCheck");
	if (pchkBox && pchkBox->GetState())
	{
		for (StlMultiMap<UInt32, CGuildApplicant *>::iterator Itr = m_guildApplicantDayOrder.begin();
			Itr != m_guildApplicantDayOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
		{
			if (i == m_StartIndex + m_curIndex)
			{
				MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, RPCContext(), Itr->second->GetChar_id(), String(), false, true);
			}
		}
	}
}
void CMenuGuildProcess::ConfirmPostChange()
{
	if (m_ownGuildMember == NULL)
		return;

	UInt i = 0;
	for (GuildMemSortMap::iterator Itr = m_guildMemberOrder.begin();
		Itr != m_guildMemberOrder.end() && i < m_StartIndex + MAX_MEM_NUM; ++Itr, i++)
	{
		if (i == m_StartIndex + m_curIndex)
		{
			if (m_ownGuildMember->GetPost() != GUILD_POSITION_MASTER &&   m_guildPostSelIndex + 1 <= m_ownGuildMember->GetPost())
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_POST_CHANGE_RIGHT")));			
			else if (Itr->second->GetLevel() < 30 && m_guildPostSelIndex + 1 == GUILD_POSITION_MASTER)
				gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_POST_MASTER_LV_FAIL")));
			else if (Itr->second->GetPost() == m_guildPostSelIndex + 1)
			{
				String posName;
				posName.Format(_T("MSG_GUILD_POSITION_%d"), Itr->second->GetPost());			
				String text;
				text.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_POSITION_SAME_ERR")), Itr->second->GetName().c_str(), 
					gGlobal.GetStringTable()->Get(posName.c_str()));
				gGlobal.PrintMessage(text.c_str(), CHANNELTYPE_SYSTEM);
			}
			else if (m_ownGuildMember)
				m_ownGuildMember->PositionChange(gGlobal.pPeerProxy, RPCContext(), Itr->second->GetChar_id(), m_guildPostSelIndex + 1);
		}
	}
}
void CMenuGuildProcess::SetGuildPostSelectIndex(int index)
{
	m_guildPostSelIndex = index;
}

void CMenuGuildProcess::RefreshGuildButton()
{
	UIButton * pBu = gGlobal.pUIManager->FindUIButton("GuildUpgradeBtn");
	if (pBu)
		pBu->SetDisableState();
	
	pBu = gGlobal.pUIManager->FindUIButton("GuildAddMemBtn");
	if (pBu)
		pBu->SetDisableState();

	pBu = gGlobal.pUIManager->FindUIButton("GuildAimBtn");
	if (pBu)
		pBu->SetDisableState();

	pBu = gGlobal.pUIManager->FindUIButton("GuildKickMemBtn");
	if (pBu)
		pBu->SetDisableState();

	pBu = gGlobal.pUIManager->FindUIButton("GuildMovePositionBtn");
	if (pBu)
		pBu->SetDisableState();

	pBu = gGlobal.pUIManager->FindUIButton("GuildRuleBtn");
	if (pBu)
		pBu->SetDisableState();

	if (m_ownGuildMember)
	{
		if (m_ownGuildMember->GetPost() <= GUILD_POSITION_DEPTHEAD)
		{
			pBu = gGlobal.pUIManager->FindUIButton("GuildAddMemBtn");
			if (pBu)
				pBu->SetNormalState();
		}

		if (m_ownGuildMember->GetPost() <= GUILD_POSITION_ELDER)
		{
			pBu = gGlobal.pUIManager->FindUIButton("GuildKickMemBtn");
			if (pBu)
				pBu->SetNormalState();
		}

		if (m_ownGuildMember->GetPost() <= GUILD_POSITION_ADMIN02)
		{
			pBu = gGlobal.pUIManager->FindUIButton("GuildKickMemBtn");
			if (pBu)
				pBu->SetNormalState();

			pBu = gGlobal.pUIManager->FindUIButton("GuildMovePositionBtn");
			if (pBu)
				pBu->SetNormalState();

			pBu = gGlobal.pUIManager->FindUIButton("GuildAimBtn");
			if (pBu)
				pBu->SetNormalState();
		}

		if (m_ownGuildMember->GetPost() <= GUILD_POSITION_SECMASTER)
		{
			pBu = gGlobal.pUIManager->FindUIButton("GuildRuleBtn");
			if (pBu)
				pBu->SetNormalState();

		}
	}
}

void CMenuGuildProcess::ConfirmDonate()
{
	UIEditBox * pBox = gGlobal.pUIManager->FindUIEditBox("NumberOfGuildDonate");
	if (pBox)
	{
		Int money = pBox->GetInt();
		if (money > 0 && money <= (Int)gGlobal.g_pCharacter->GetCharNetObject()->GetMoney())
			gGlobal.g_pCharacter->GetCharNetObject()->DonateMoney(gGlobal.pPeerProxy, RPCContext(), m_ownGuild->GetGuild_uid(), money);
		else
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_DONATE_MONEY_FAIL")));
	}
}

CGuild * CMenuGuildProcess::FindGuild(UInt32 guild_uid)
{
	StlMap<UInt32, CGuild*>::iterator Itr = m_guildMap.find(guild_uid);
	if (Itr != m_guildMap.end())
	{
		return Itr->second;
	}
	return NULL;
}
void CMenuGuildProcess::ConfirmGuildNameInput()
{
	UIEditBox * pBox = gGlobal.pUIManager->FindUIEditBox("NumberOfGuildNameInput");
	if (pBox)
	{
		String text = pBox->GetText();
		gGlobal.g_pCharacter->GetCharNetObject()->CreateGuild(gGlobal.pPeerProxy, RPCContext(), text, false);
		//Int money = pBox->GetInt();
		//if (money >= 0 && money <= gGlobal.g_pCharacter->GetCharNetObject()->GetMoney())
		//	gGlobal.g_pCharacter->GetCharNetObject()->DonateMoney(gGlobal.pPeerProxy, RPCContext(), m_ownGuild->GetGuild_uid(), money);
	}
}

void CMenuGuildProcess::CancelGuildNameInput()
{
	gGlobal.g_pCharacter->GetCharNetObject()->CreateGuild(gGlobal.pPeerProxy, RPCContext(), String(), true);
}

void CMenuGuildProcess::RefreshInfoMenu()
{
	UICheckBox * pchkBox = gGlobal.pUIManager->FindUICheckBox("GuildInfoCheck");
	if (pchkBox && !pchkBox->GetState())
		return;


	if (m_ownGuild && m_ownGuildMember)
	{
		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoName");
		if (pText)
		{
			pText->SetText(m_ownGuild->GetName().c_str());
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoFirstMaster");
		if (pText)
		{
			pText->SetText(m_ownGuild->GetCreator().c_str());
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoCurMaster");
		if (pText)
		{
			pText->SetText(m_ownGuild->GetMaster().c_str());
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoVice");
		if (pText)
		{
			pText->SetText(m_ownGuild->GetSecMasterName().c_str());
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoNum");
		if (pText)
		{
			String text;
			text.Format(_T("%d"), m_ownGuild->GetTotalMember());
			pText->SetText(text);
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoNum");
		if (pText)
		{
			String text;
			text.Format(_T("%d/%d"), m_ownGuild->GetTotalMember(), MAX_GUILD_NUM + 10 * m_ownGuild->GetRoom());
			pText->SetText(text);
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoNum");
		if (pText)
		{
			String text;
			text.Format(_T("%d"), m_ownGuild->GetGuild_uid());
			pText->SetText(text);
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoPerName");
		if (pText)
		{
			pText->SetText(gGlobal.g_pCharacter->GetCharName());
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoPerLevel");
		if (pText)
		{
			String text;
			text.Format(_T("%d"), gGlobal.g_pCharacter->GetLevel());
			pText->SetText(text);
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoMoney");
		if (pText)
		{
			String text;
			text.Format(_T("%d"), m_ownGuild->GetFund());
			pText->SetText(text);
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoRich");
		if (pText)
		{
			String text;
			if (m_ownGuild->GetManorScale() > 0)
			{
				const ManorData* manorDat = gGlobal.m_resource->GetManorData(m_ownGuild->GetManorScale());
				if (manorDat)
					text.Format(_T("%d/%d"), m_ownGuild->GetProsper(), manorDat->manor_maxProsper);
			}
			else
				text.Format(_T("0/0"));	
			pText->SetText(text);
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoMemNum");
		if (pText)
		{
			String text;
			text.Format(_T("%d/%d"), m_ownGuild->GetTotalMember(), MAX_GUILD_NUM + 10 * m_ownGuild->GetRoom());
			pText->SetText(text);
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoAlly");
		if (pText)
		{
			//find back the ally 
			StlMap<UInt32, CGuild*>::iterator Itr2 = m_guildMap.find(m_ownGuild->GetAlliance_id());
			if (Itr2 != m_guildMap.end())
			{
				pText->SetText(Itr2->second->GetName().c_str());
				pText->Redraw();
			}
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoEnemy");
		if (pText)
		{
			//find back the ally 
			StlMap<UInt32, CGuild*>::iterator Itr2 = m_guildMap.find(m_ownGuild->GetEnemy_id());
			if (Itr2 != m_guildMap.end())
			{
				pText->SetText(Itr2->second->GetName().c_str());
				
			}
			else
				pText->SetText(String());

			pText->Redraw();
		}
		
		
		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoJob");
		if (pText)
		{
			String name;
			name.Format(_T("MSG_GUILD_POSITION_%d"), m_ownGuildMember->GetPost());			
			pText->SetText(gGlobal.GetStringTable()->Get(name.c_str()));
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoDonation");
		if (pText)
		{
			String text;
			text.Format(_T("%d/%d"), m_ownGuildMember->GetDonate(), m_ownGuildMember->GetTotalDonate());
			pText->SetText(text.c_str());
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoBid");
		if (pText)
		{
			String text;
			text.Format(_T("%d"), m_ownGuild->GetGuildPvPBid());
			pText->SetText(text.c_str());
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoGroup");
		if (pText)
		{
			String text;
			const PSTRINGW GroupName[9] ={ 
				_T(""),
				_T("A"),
				_T("B"),
				_T("C"),
				_T("D"),
				_T("E"),
				_T("F"),
				_T("G"),		
				_T("H"),
			};
			if (m_ownGuild->GetGuildPvPGroup() < 9)
			{
				text = GroupName[m_ownGuild->GetGuildPvPGroup()];
			}
			
			pText->SetText(text.c_str());
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoGroupVSGuild");
		if (pText)
		{
			pText->SetText(m_ownGuild->GetGroupPvPVSGuild().c_str());
			pText->Redraw();
		}
		
		pText = gGlobal.pUIManager->FindUIStaticText("GuildInfoPvPScore");
		if (pText)
		{
			String text;
			text.Format(_T("%d"), m_ownGuild->GetGuildPvPScore());
			pText->SetText(text.c_str());
			pText->Redraw();
		}
	}
}

void CMenuGuildProcess::PositionChange(UInt32 src, UInt32 target, UInt8 position)
{
	StlMap<UInt32, CGuildMember *>::iterator ItrSrc = m_guildMemberMap.find(src);
	StlMap<UInt32, CGuildMember *>::iterator ItrTarget = m_guildMemberMap.find(target);
	if (ItrSrc != m_guildMemberMap.end() && ItrTarget != m_guildMemberMap.end())
	{
		String postName;
		postName.Format(_T("MSG_GUILD_POSITION_%d"), position);
		postName = gGlobal.GetStringTable()->Get(postName.c_str());

		String msg;
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_POSITION_CHANGE")), ItrSrc->second->GetName().c_str(), ItrTarget->second->GetName().c_str(), postName.c_str());

		gGlobal.PrintMessage(msg, CHANNELTYPE_GUILD);
	}
}

void CMenuGuildProcess::SetGuildUIText(PCSTRINGA name, String &text)
{
	if (!name)
		return;

    UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(name);
	if (pText)
	{
		pText->SetText(text);
		pText->Redraw();
		pText->SetEnable(true);
		text.clear();
	}
}


void CMenuGuildProcess::RefreshGuildBusinessLogPanel()
{
	UIButton * pBu = gGlobal.pUIManager->FindUIButton("GuildBusinessLogClear");
	if (pBu)
		pBu->SetDisableState();
	if (m_ownGuildMember)
	{
		if (m_ownGuildMember->GetPost() <= GUILD_POSITION_MASTER)
		{
			pBu = gGlobal.pUIManager->FindUIButton("GuildBusinessLogClear");
			if (pBu)
				pBu->SetNormalState();
		}
	}
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideGuildBusinessLogStuffs", String());
	StlMultiMap<UInt16, CGuildMember *> businessLogMap;
	for (GuildMemSortMap::iterator Itr = m_guildMemberOrder.begin();
		Itr != m_guildMemberOrder.end(); ++Itr)
	{
		if (Itr->second && Itr->second->GetBusinessCount() > 0)
		{
			businessLogMap.insert(std::make_pair(Itr->second->GetBusinessCount(), Itr->second));
		}
	}
	UInt i = 0;
	for (StlMultiMap<UInt16, CGuildMember *>::reverse_iterator Itr = businessLogMap.rbegin();
		Itr != businessLogMap.rend() && i < m_businessLogStartIndex + MAX_BUSINESS_LOG_NUM; ++Itr, i++)
	{
		if (i >= m_businessLogStartIndex)
		{
			StringA name;
			UIStaticText * pText = NULL;
			name.Format("GuildBusinessLogName%d", i + 1 - m_businessLogStartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				pText->SetText(Itr->second->GetName().c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildBusinessLogLV%d", i + 1 - m_businessLogStartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				String text;
				text.Format(_T("%d"), Itr->second->GetLevel());
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildBusinessLogCount%d", i + 1 - m_businessLogStartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				String text;
				text.Format(_T("%d"), Itr->second->GetBusinessCount());
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}

			name.Format("GuildBusinessLogProfit%d", i + 1 - m_businessLogStartIndex);
			pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				String text;
				text.Format(_T("%d"), Itr->second->GetBusinessProfit());
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(TRUE);
			}
		}
	}


	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("GuildBusinessLogScrollBar");
	if (scrollBar)
	{
		scrollBar->SetNumCount(businessLogMap.size());
	}
	businessLogMap.clear();
}

void CMenuGuildProcess::ScrollGuildBusinessLog()
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("GuildBusinessLogScrollBar");
	if (scrollBar)
	{
		UInt size(0);
		for (GuildMemSortMap::iterator Itr = m_guildMemberOrder.begin();
			Itr != m_guildMemberOrder.end(); ++Itr)
		{
			if (Itr->second && Itr->second->GetBusinessCount() > 0)
			{
				++size;
			}
		}
		UInt index = scrollBar->GetSelNum();
		if (index + MAX_BUSINESS_LOG_NUM > size)
		{
			if (size < MAX_BUSINESS_LOG_NUM)
				m_businessLogStartIndex = 0;
			else
				m_businessLogStartIndex = size - MAX_BUSINESS_LOG_NUM;
		}
		else 
			m_businessLogStartIndex = index;

		RefreshGuildBusinessLogPanel();
	}
}

void CMenuGuildProcess::GuildBusinessLogPageUp()
{
	if (m_businessLogStartIndex > 0)
	{
		m_businessLogStartIndex -- ;
		RefreshGuildBusinessLogPanel();
	}
}

void CMenuGuildProcess::GuildBusinessLogPageDown()
{
	UInt size(0);
	for (GuildMemSortMap::iterator Itr = m_guildMemberOrder.begin();
		Itr != m_guildMemberOrder.end(); ++Itr)
	{
		if (Itr->second && Itr->second->GetBusinessCount() > 0)
		{
			++size;
		}
	}
	if (m_businessLogStartIndex  + MAX_BUSINESS_LOG_NUM < size)
	{
		m_businessLogStartIndex ++;
		RefreshGuildBusinessLogPanel();
	}
}

void CMenuGuildProcess::ConfirmGuildBusinessLogSearchYes()
{
	UIEditBox * pBox = gGlobal.pUIManager->FindUIEditBox("NumberOfGuildBusinessLogSearch");
	if (!pBox || pBox->GetInt() <= 0)
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BUSINESS_LOG_SEARCH_FAIL")));	
		return;
	}
	UInt32 SearchID = (UInt32)pBox->GetInt();
	StlMultiMap<UInt16, CGuildMember *> businessLogMap;
	for (GuildMemSortMap::iterator Itr = m_guildMemberOrder.begin();
		Itr != m_guildMemberOrder.end(); ++Itr)
	{
		if (Itr->second && Itr->second->GetBusinessCount() > 0)
		{
			businessLogMap.insert(std::make_pair(Itr->second->GetBusinessCount(), Itr->second));
		}
	}
	bool bFind(false);
	UInt32 Count(0);
	for (StlMultiMap<UInt16, CGuildMember *>::reverse_iterator Itr = businessLogMap.rbegin();
		Itr != businessLogMap.rend(); ++Itr, Count++)
	{
		if (Itr->second->GetChar_id() == SearchID)
		{
			bFind = true;
			if (Count < MAX_BUSINESS_LOG_NUM)
			{
				m_businessLogStartIndex = 0;
				m_curBusinessLogIndex = Count;
			}
			else
			{
				m_businessLogStartIndex = Count - MAX_BUSINESS_LOG_NUM + 1;
				m_curBusinessLogIndex = MAX_BUSINESS_LOG_NUM - 1;
			}
			break;
		}
	}
	if (!bFind)
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_BUSINESS_LOG_SEARCH_FAIL")));
		return;
	}
	RefreshGuildBusinessLogPanel();
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HightLightGuildBusinessLog", String(), m_curBusinessLogIndex);
}

void CMenuGuildProcess::SetGuildBusinessLogSelectIndex(int index)
{
	m_curBusinessLogIndex = index;
}

void CMenuGuildProcess::ClearGuildBusinessLog()
{
	if (m_ownGuildMember && m_ownGuildMember->GetPost() <= GUILD_POSITION_MASTER)
	{
		m_ownGuildMember->ClearGuildBusinessLog(gGlobal.pPeerProxy, RPCContext());
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideGuildBusinessLogStuffs", String());
	}
}
