//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Party.h"
//-- Library
#include "QGL/Global.h"
#include "QGL/MapControl.h"
#include "QGL/CharacterControl.h"
#include "QGL/QGLApplication.h"
#include "QGL/CallLuaFunc.h"
#include "QGL/MenuPartyProcess.h"
#include "QGL/MenuFriendProcess.h"
#include "QGL/Configure.h"
#include "QGL/MiddleMap.h"
#include "QGL/CrossMapPathFinder.h"
#include "Proxy/Logic/PartyCharInfoClient.h"

#define MAX_PARTY_NUM 5

REGISTER_CLASSTYPE(CParty, Party_CLASSID);

CParty::CParty()
{
}

CParty::~CParty()
{

}

RPCResult CParty::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, 
						   const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CParty CB_Error: err %d, %s"), err, errMsg.c_str());

	return RPC_RESULT_OK;
}

/*
void CParty::OnUpdateInfo(const UInt32 &ownerId)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty::OnUpdateInfo(owner)"));

	NetObject * netobj;
	CCharacter * pchar;
	UInt size = GetCount();
	UInt32 memberUIPos = 0;
	for( Index i = C_INDEX(0); i < size; ++i)
	{
		netobj = GetChild(i);
		if (netobj->GetClassID()==Character_CLASSID)
		{
			pchar = CastNetObject<CCharacter>(netobj);
			if (GetOwnerId()==pchar->GetChar_id())
			{
				//m_TeamLeader = QGLApplication::GetInstance().FindCharCtrlByNetObject(pchar);
				CCharacterControl * pCharCtrl = QGLApplication::GetInstance().FindCharCtrlByNetObject(pchar);
				if (pCharCtrl)
					pCharCtrl->GetCharMainView()->SetExtraZBuffer(1);

				if (pchar->GetChar_id()==gGlobal.g_pCharacter->GetCharID())
					SetUILeader(true, true, 0);
				else
					SetUILeader(true, false, memberUIPos+1);
			}
			else
			{
				CCharacterControl * pCharCtrl = QGLApplication::GetInstance().FindCharCtrlByNetObject(pchar);
				if (pCharCtrl)
					pCharCtrl->GetCharMainView()->SetExtraZBuffer(0);

				if (pchar->GetChar_id()==gGlobal.g_pCharacter->GetCharID())
					SetUILeader(false, true, 0);
				else
					SetUILeader(false, false, memberUIPos+1);
			}
			if (pchar->GetChar_id()!=gGlobal.g_pCharacter->GetCharID())
				memberUIPos++;
		}
	}
	for( UInt i = memberUIPos; i<MAX_PARTY_NUM-1; i++)
		SetUILeader(false, false, i+1);
	gGlobal.pUIManager->RenderAll();

	ResetZValueForTeam();
}
*/

RPCResult CParty::CB_LeaderChanged(LPCPEER pPeer, RPCContext &context, 
								   const UInt32 &memberId, const UInt8 &memberIdx,
								   const Boolean &shown)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty::CB_LeaderChanged"));

	if (memberIdx == 0 
		|| memberIdx >= gGlobal.g_vctPartyMemberPointer.size() 
		|| gGlobal.g_vctPartyMemberPointer.size() < 1)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty CB_LeaderChanged Error: member index out range"));
		return RPC_RESULT_FAIL;
	}
	
	String strNewLeaderName;
	if (NULL != gGlobal.g_pParty) 
	{//in party
		NetObject* oldObject = gGlobal.g_vctPartyMemberPointer[0];
		NetObject* newObject = gGlobal.g_vctPartyMemberPointer[memberIdx];
		gGlobal.g_vctPartyMemberPointer[0]=newObject;
		gGlobal.g_vctPartyMemberPointer[memberIdx] = oldObject;

		Global::PartyCharInfoMap::iterator itOld = gGlobal.g_mapPartyChar.find(memberIdx);
		CPartyCharInfoClient* pCpcic = NULL;
		if (itOld != gGlobal.g_mapPartyChar.end()) 
		{
			pCpcic = itOld->second;
			gGlobal.g_mapPartyChar.erase(itOld);
			gGlobal.g_mapPartyChar.insert(std::make_pair(0,pCpcic));
			if (NULL != pCpcic) 
				strNewLeaderName = pCpcic->GetStrNickName();

			if (oldObject == gGlobal.g_pCharacter->GetCharNetObject()) 
			{//self is prev leader
				gGlobal.g_pTempLeaveParty = gGlobal.g_pParty;
				gGlobal.g_pParty = NULL;
			}
		}
		else
		{
			CCharacter* pLeader = CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[0]);
			if (NULL != pLeader) 
				strNewLeaderName = pLeader->GetNickName();
		}
	}
	else if (NULL != gGlobal.g_pTempLeaveParty) 
	{//temporarily leave party
		if (memberId == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id()) 
		{//self become leader
			gGlobal.g_vctPartyMemberPointer[0]=gGlobal.g_pCharacter->GetCharNetObject();
			gGlobal.g_vctPartyMemberPointer[memberIdx] = NULL;
			Global::PartyCharInfoMap::iterator itOldLeader = gGlobal.g_mapPartyChar.find(0);
			if (itOldLeader != gGlobal.g_mapPartyChar.end()) 
			{
				CPartyCharInfoClient* pCpcic = itOldLeader->second;
				gGlobal.g_mapPartyChar.erase(itOldLeader);
				gGlobal.g_mapPartyChar.insert(std::make_pair(memberIdx,pCpcic));
			}
			gGlobal.g_pParty = gGlobal.g_pTempLeaveParty;
			gGlobal.g_pTempLeaveParty = NULL;
		}
		else
		{
			Global::PartyCharInfoMap::iterator itOldLeader = gGlobal.g_mapPartyChar.find(0);
			Global::PartyCharInfoMap::iterator itOld = gGlobal.g_mapPartyChar.find(memberIdx);
			CPartyCharInfoClient* pCpcic = NULL;
			if (itOld != gGlobal.g_mapPartyChar.end()) 
			{
				pCpcic = itOld->second;
				if (itOldLeader != gGlobal.g_mapPartyChar.end()) 
				{
					itOld->second = itOldLeader->second;
					itOldLeader->second = pCpcic;
				}
				else
				{
					gGlobal.g_mapPartyChar.erase(itOld);
					gGlobal.g_mapPartyChar.insert(std::make_pair(0,pCpcic));
				}
				if (NULL != pCpcic) 
					strNewLeaderName = pCpcic->GetStrNickName();
			}
			NetObject* oldObject = gGlobal.g_vctPartyMemberPointer[0];
			NetObject* newObject = gGlobal.g_vctPartyMemberPointer[memberIdx];
			gGlobal.g_vctPartyMemberPointer[0]=newObject;
			gGlobal.g_vctPartyMemberPointer[memberIdx] = oldObject;
		}
	}
	RedrawUI();

	//print message
	if (shown)
	{
		String text(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_LEADER")));

		if (IsLeader())
			text =gGlobal.GetStringTable()->Get(_T("MSG_PARTY_LEADERU"));
		else
			text.Format(text.c_str(), strNewLeaderName.c_str());
		gGlobal.PrintPlayerMessage(text);
	}

	if (!IsLeader())
	{
		gGlobal.g_pMiddleMap->ResetCharDes();
		CCrossMapPathFinder::GetInstance()->DestroyPathData();
		gGlobal.m_u32DestNpcId = 0;
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseMiddleMap", String());
	}
	
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	gGlobal.g_pPartyProcess->UpdatePartyPanel(pParty);
	gGlobal.pUIManager->RenderAll();

	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
	{
		if (IsLeader())
		{
			if (!gGlobal.g_NoMoreHelp)
			gGlobal.g_helpEnable = true;
			UIButton *pItem = gGlobal.pUIManager->FindUIButton("DownBParty");
			if (pItem)
				pItem->SetNormalState();
		}
		else
		{
			gGlobal.g_helpEnable = false;
			UIButton *pItem = gGlobal.pUIManager->FindUIButton("BattleHelpB");
			if (pItem)
				pItem->SetDisableState();
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CParty::CB_Join(LPCPEER pPeer, RPCContext &context, NetObject* member, 
						  const Boolean &shown)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty::CB_Enter"));
	Add(member);
	
	if (NULL != gGlobal.g_pParty) 
		gGlobal.g_vctPartyMemberPointer.push_back(member);
	else if (NULL != gGlobal.g_pTempLeaveParty)
		gGlobal.g_vctPartyMemberPointer.push_back(NULL);

	CCharacter* pMember = CastNetObject<CCharacter>(member);
	if (pMember)
	{
		CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByNetObject(pMember);
		if (pCtrl)
		{
			pCtrl->SetFontColor(TextColor[ColorIndex_LightBlue]);
			pCtrl->GetCharNameView()->SetFontColor(pCtrl->GetFontColor());
			//pCtrl->GetCharFrontNameView()->SetFontColor(pCtrl->GetFontColor());
		}
		//create temp friend if not exist
		gGlobal.g_pMenuFriendProcess->InserTempFriend(pMember->GetChar_id(), pMember->GetNickName(), FRIENC_CHAT_STATUS_ONLINE);
		
	}
	//pCtrl->GetCharNameView()->SetFontColor(TextColor[ColorIndex_LightBlue]);
	RedrawUI();
	
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	gGlobal.g_pPartyProcess->UpdatePartyPanel(pParty);

	//print message
	if (shown)
	{
		CCharacter* pchar =CastNetObject<CCharacter>(member);
		if (pchar)
		{
			String text(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_JOIN")));
			text.Format(text.c_str(), pchar->GetNickName().c_str());
			gGlobal.PrintPlayerMessage(text);
		}
	}

	gGlobal.pUIManager->RenderAll();

	return RPC_RESULT_OK;
}

RPCResult CParty::CB_Leave(LPCPEER pPeer, RPCContext &context, NetObject* member, 
						   const bool &kick, const Boolean &shown)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty CB_Leave:"));
	CCharacter* pMember = CastNetObject<CCharacter>(member);

	if (pMember == NULL)
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CParty CB_Leave Error: invalid member"));
		return RPC_RESULT_FAIL;
	}

	if (pMember == gGlobal.g_pCharacter->GetCharNetObject()) {
		return CB_Destroy(pPeer, context, kick, shown);
	}
	
	Boolean bTempLeave =BOOLEAN_FALSE;
	UInt16 u16Count = 0;
	for (Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.begin();it != gGlobal.g_mapPartyChar.end();it++) 
	{
		CPartyCharInfoClient* pCpc = it->second;
		if (NULL != pCpc) 
		{
			if (pCpc->GetU32CharId() == pMember->GetChar_id()) 
			{
				bTempLeave=BOOLEAN_TRUE;
				u16Count = it->first;
				break;
			}
		}
	}
	
	if (bTempLeave) 
	{
		Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(u16Count);
		if (it != gGlobal.g_mapPartyChar.end())
		{
			SafeDeleteObject(it->second);
			gGlobal.g_mapPartyChar.erase(it);
		}
		Remove(GetChild(u16Count));
		StlVector<NetObject*,ManagedAllocator<NetObject*> >::iterator itVct = gGlobal.g_vctPartyMemberPointer.begin();
		for (UInt16 k=0;k<gGlobal.g_vctPartyMemberPointer.size();k++) 
		{
			if (itVct != gGlobal.g_vctPartyMemberPointer.end()) 
			{
				if (k == u16Count) 
				{
					gGlobal.g_vctPartyMemberPointer.erase(itVct);
					break;
				}
				else
					itVct++;
			}
			else
				break;
		}
		u16Count++;
		CPartyCharInfoClient* pcpic=NULL;
		for (;u16Count < 5;u16Count++) 
		{
			Global::PartyCharInfoMap::iterator itTmp = gGlobal.g_mapPartyChar.find(u16Count);
			if (itTmp != gGlobal.g_mapPartyChar.end())
			{
				pcpic = itTmp->second;
				gGlobal.g_mapPartyChar.erase(itTmp);
			}
			if (NULL != pcpic)
			{
				gGlobal.g_mapPartyChar.insert(std::make_pair(u16Count-1,pcpic));
				pcpic = NULL;
			}
		}
	}
	else
	{
		for (;u16Count<gGlobal.g_vctPartyMemberPointer.size();u16Count++) 
		{
			CCharacter* pChild = CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[u16Count]);
			if (pChild == pMember) 
				break;
		}
		StlVector<NetObject*,ManagedAllocator<NetObject*> >::iterator itVct = gGlobal.g_vctPartyMemberPointer.begin();
		for (UInt16 k=0;k<gGlobal.g_vctPartyMemberPointer.size();k++) 
		{
			if (itVct != gGlobal.g_vctPartyMemberPointer.end()) 
			{
				if (k == u16Count) 
				{
					gGlobal.g_vctPartyMemberPointer.erase(itVct);
					break;
				}
				else
					itVct++;
			}
			else
				break;
		}
		u16Count++;
		CPartyCharInfoClient* pcpic=NULL;
		for (;u16Count < 5;u16Count++) 
		{
			Global::PartyCharInfoMap::iterator itTmp = gGlobal.g_mapPartyChar.find(u16Count);
			if (itTmp != gGlobal.g_mapPartyChar.end())
			{
				pcpic = itTmp->second;
				gGlobal.g_mapPartyChar.erase(itTmp);
			}
			if (NULL != pcpic)
			{
				gGlobal.g_mapPartyChar.insert(std::make_pair(u16Count-1,pcpic));
				pcpic = NULL;
			}
		}
		Remove(pMember);
	}
	
	CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByNetObject(pMember);
	if (pCtrl)
	{
		if (!bTempLeave) 
		{
			if (pCtrl != gGlobal.g_pCharacter)
			{
				pCtrl->SetFontColor(D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
					gConfigure.uNameFontColorG, gConfigure.uNameFontColorB));
				pCtrl->GetCharNameView()->SetFontColor(pCtrl->GetFontColor());
			}
			pCtrl->GetCharMainView()->RemoveFollowTarget();
		}
	}
		//pCtrl->GetCharNameView()->SetFontColor(D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
		//	gConfigure.uNameFontColorG, gConfigure.uNameFontColorB));

	RedrawUI();

	//print message
	if (shown)
	{
		if (pCtrl)
		{
			if (kick)
			{
				String text(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_KICK")));

				if (pCtrl == gGlobal.g_pCharacter)
					text =gGlobal.GetStringTable()->Get(_T("MSG_PARTY_KICKU"));
				else
					text.Format(text.c_str(), pCtrl->GetCharName().c_str());
				gGlobal.PrintPlayerMessage(text);
			}
			else
			{
				String text(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_LEAVE")));
				text.Format(text.c_str(), pCtrl->GetCharName().c_str());
				gGlobal.PrintPlayerMessage(text);
			}
		}
	}
	
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	gGlobal.g_pPartyProcess->UpdatePartyPanel(pParty);
	gGlobal.pUIManager->RenderAll();

	return RPC_RESULT_OK;
}

RPCResult CParty::CB_Destroy(LPCPEER pPeer, RPCContext &context, const bool &kick, 
							 const Boolean &shown)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty::CB_Destroy"));

	CCharacter * pchar = NULL;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		pchar = GetChild<CCharacter>(i);
		if (pchar == NULL) continue;
		CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByNetObject(pchar);
		if (pCtrl && !GetM_bTempLeave(i))
		{
			if (pCtrl != gGlobal.g_pCharacter)
			{
				pCtrl->SetFontColor(D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
					gConfigure.uNameFontColorG, gConfigure.uNameFontColorB));
				pCtrl->GetCharNameView()->SetFontColor(pCtrl->GetFontColor());
			}
			if (i != 0)
				pCtrl->GetCharMainView()->RemoveFollowTarget();
			//pCtrl->GetCharNameView()->SetFontColor(D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
			//gConfigure.uNameFontColorG, gConfigure.uNameFontColorB));
		}
	}

	if (shown)
	{
		if (kick)
		{
			String text(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_KICKU")));
			//text.Format(text.c_str(), gGlobal.g_pCharacter->GetCharName().c_str());
			gGlobal.PrintPlayerMessage(text);
		}
		else
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_DESTROY")));
	}

	SetUILeader(false, true, 0);					// hide leader icon of player
	for(UInt i = 1; i < MAX_PARTY_NUM; ++i) {
		SetUILeader(false, false, i);				// hide leader icon
		SetUIMemberPanel(false, i);					// hide non-existing party member
	}
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseTeammateMenu", String(), 0, 0);

	for (Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.begin();it != gGlobal.g_mapPartyChar.end();it++)
	{
		SafeDeleteObject(it->second);
	}
	gGlobal.g_mapPartyChar.clear();

	gGlobal.pUIManager->RenderAll();
	
	CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
	DeleteNetObject(pParty);
	gGlobal.g_vctPartyMemberPointer.clear();
	gGlobal.g_pParty = NULL;
	gGlobal.g_pTempLeaveParty = NULL;

	gGlobal.g_pPartyProcess->ShowPartyButton();

	return RPC_RESULT_OK;
}

RPCResult CParty::CB_Regroup(LPCPEER pPeer, RPCContext &context, NetObject* member, 
							 const UInt8 &memberIdx)
{
	CCharacter* pMember = CastNetObject<CCharacter>(member);
	if (pMember)
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, 
			_T("CParty::CB_Regroup: char_id %d, index %d, avail %d"),
			pMember->GetChar_id(), memberIdx, GetAvail());
		Insert(pMember, memberIdx);
		if (gGlobal.g_vctPartyMemberPointer.size() > memberIdx) 
		{
			NetObject* pObjExch = gGlobal.g_vctPartyMemberPointer[memberIdx];
			gGlobal.g_vctPartyMemberPointer[memberIdx] = member;
			for (UInt8 i= memberIdx;i<gGlobal.g_vctPartyMemberPointer.size() -1;i++) 
			{
				NetObject* pObjNext = gGlobal.g_vctPartyMemberPointer[i+1];
				gGlobal.g_vctPartyMemberPointer[i+1] = pObjExch;
				pObjExch = pObjNext;
			}
			gGlobal.g_vctPartyMemberPointer.push_back(pObjExch);
		}
		else
			gGlobal.g_vctPartyMemberPointer.push_back(member);

		if (gGlobal.g_pMapView)
			gGlobal.g_pMapView->NotifyObjectAppeared(pMember);
		CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByNetObject(pMember);
		if (pCtrl)
		{
			pCtrl->SetFontColor(TextColor[ColorIndex_LightBlue]);
			pCtrl->GetCharNameView()->SetFontColor(pCtrl->GetFontColor());
			//pCtrl->GetCharFrontNameView()->SetFontColor(pCtrl->GetFontColor());
		}
		//create temp friend if not exist
		gGlobal.g_pMenuFriendProcess->InserTempFriend(pMember->GetChar_id(), pMember->GetNickName(), FRIENC_CHAT_STATUS_ONLINE);

	}
	//pCtrl->GetCharNameView()->SetFontColor(TextColor[ColorIndex_LightBlue]);

	RedrawUI();
	gGlobal.g_pPartyProcess->UpdatePartyPanel(gGlobal.g_pParty);
	gGlobal.pUIManager->RenderAll();

	return RPC_RESULT_OK;
}

BOOL CParty::IsLeader()
{
	if (NULL != gGlobal.g_pParty) 
	{
		if (gGlobal.g_vctPartyMemberPointer.size() > 0) 
			return (gGlobal.g_pCharacter->GetCharNetObject() && 
			gGlobal.g_pCharacter->GetCharNetObject() == gGlobal.g_vctPartyMemberPointer[0]);
	}

	return FALSE;
}

CCharacter* CParty::GetMember(UInt16 index)
{
	if (index < gGlobal.g_vctPartyMemberPointer.size())
		return CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[index]);
	else
		return NULL;
}

CCharacter* CParty::FindMember(UInt32 char_id)
{
	CCharacter* pchar;
	for (UInt16 i = 0; i < gGlobal.g_vctPartyMemberPointer.size(); ++i)
	{
		 pchar = CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[i]);
		 if (pchar && pchar->GetChar_id() == char_id)
			 return pchar;
	}
	return NULL;
}

VOID CParty::ShowMember()
{
	TCHAR buf[256];
	CCharacter *pchar;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		 pchar = GetChild<CCharacter>(i);
		if (pchar) {
			_stprintf(buf,gGlobal.GetStringTable()->Get(_T("MSG_PARTY_MEMBERSHOW")),
				i+1, pchar->GetNickName().c_str(), pchar->GetChar_id());
			gGlobal.PrintMessage(buf, CHANNELTYPE_SYSTEM, _T(""));
		}
	}
}

VOID CParty::RedrawUI()
{
	CCharacter *pchar;
	String imageFile;
	UInt16 otherIdx = 1;
	UInt16 hideStart = 0;
	UInt16 action = gGlobal.g_pCharacter->GetCharNetObject()->GetAction();

	// not in battle
	Boolean bTempLeave =(gGlobal.g_pParty == NULL && gGlobal.g_pTempLeaveParty != NULL);
	if (action != CCharacter::ST_BATTLE) {
		for (UInt16 i = 0; i < gGlobal.g_vctPartyMemberPointer.size(); ++i)
		{
			pchar =CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[i]);
			if (pchar == gGlobal.g_pCharacter->GetCharNetObject()) {		// myself
				SetUILeader(i == 0, true, 0);				// party leader: i == 0
			} 
			else if (!bTempLeave && !GetM_bTempLeave(i))
			{												//self in party && other member in party
				//imageFile.Format("Data/faceS%05ia.tga", pchar->GetSexClass());
				imageFile.Format(_T("faceS%05ia.ent"), pchar->GetSexClass());
				SetUIMemberHPBar(otherIdx, pchar->GetHP(), pchar->GetMHP(), pchar->GetMaxHP());
				SetUIMemberSPBar(otherIdx, pchar->GetSP(), pchar->GetMSP(), pchar->GetMaxSP());
				SetUIMemberImage(otherIdx, imageFile.c_str(), pchar->GetNickName().c_str());
				//SetUIMemberName(otherIdx, pchar->GetNickName().c_str());
				SetUIMemberPanel(true, otherIdx);
				SetUILeader((i == 0), false, otherIdx);		// party leader: i == 0
				otherIdx++;
			}
			else//Temporary leave member user interface draw here
			{//self temporarily leave or member temporarily leave
				Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(i);
				if (it != gGlobal.g_mapPartyChar.end()) 
				{
					CPartyCharInfoClient* pCpc = it->second;
					if (NULL != pCpc) 
					{
						if (GetM_bTempLeave(i)) 
							imageFile.Format(_T("faceS%05iah.ent"), pCpc->GetU8SexClass());
						else
							imageFile.Format(_T("faceS%05ia.ent"), pCpc->GetU8SexClass());
						SetUIMemberHPBar(otherIdx, pCpc->GetU16Hp(), pCpc->GetU16MHp(), pCpc->GetU16MaxHp());
						SetUIMemberSPBar(otherIdx, pCpc->GetU16Sp(), pCpc->GetU16MSp(), pCpc->GetU16MaxSp());
						SetUIMemberImage(otherIdx, imageFile.c_str(), pCpc->GetStrNickName().c_str());
						//SetUIMemberName(otherIdx, pchar->GetNickName().c_str());
						SetUIMemberPanel(true, otherIdx);
						SetUILeader((i == 0), false, otherIdx);		// party leader: i == 0
						otherIdx++;
					}
				}
			}
		}
		hideStart = gGlobal.g_vctPartyMemberPointer.size();
	} else {
		SetUILeader(false, true, 0);
	}

	for (UInt16 i = hideStart; i < MAX_PARTY_NUM; ++i)
	{
		SetUILeader(false, false, i);		// hide non-existing leader icon
		SetUIMemberPanel(false, i);			// hide non-existing party member
	}

	//ResetZValueForTeam();
}

VOID CParty::SetUILeader(bool display, bool myself, UInt16 otherPos)
{
	//don't display if it is in battle
	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
		display = false;

	StringA targetName;
	if (myself)
		targetName = "FriendLeaderMe";
	else
		targetName.Format("FriendLeader%01i", otherPos);
	UIItem * uiitem = gGlobal.pUIManager->GetUIItem(targetName.c_str());
	if (uiitem)
		uiitem->SetEnable(display);
}

VOID CParty::SetUIMemberPanel(bool display, UInt16 otherPos)
{
	//don't display if it is in battle
	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
		display = false;

	StringA targetName;
	targetName.Format("Friend%01i", otherPos);
	UIItem * uip = gGlobal.pUIManager->GetUIItem(targetName.c_str());
	if (uip)
		uip->SetEnable(display);
}

VOID CParty::SetUIMemberImage(UInt16 otherPos, PCSTRING imageName, PCSTRING charName)
{
	StringA targetName;
	targetName.Format("FriendPhoto%01i", otherPos);
	/*UIImage * uim = gGlobal.pUIManager->FindUIImage(targetName.c_str());
	if (uim)
		uim->SetImageFileA(imageName.c_str(), 0, 0, 0, 0);
	*/
	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(targetName.c_str());
	if (pAniIcon)
	{
		pAniIcon->ClearAniData();
		pAniIcon->SetAnimationData(imageName);
		pAniIcon->SetTips(charName);
	}
}

VOID CParty::SetUIMemberName(UInt16 otherPos, PCSTRING charName)
{
	StringA targetName;
	targetName.Format("FriendName%01i", otherPos);
	UIStaticText * uit = gGlobal.pUIManager->FindUIStaticText(targetName.c_str());
	if (uit)
	{
		uit->SetText(charName);
		uit->Redraw();
	}
}

VOID CParty::SetUIMemberHPBar(UInt16 pos, UInt16 hp, UInt16 mhp, UInt16 maxhp)
{
	StringA targetName;
	UILinerImage * uLI;
	targetName.Format("FriendHPBarLim%01i", pos);
	uLI = gGlobal.pUIManager->FindUILinerImage(targetName.c_str());
	if (uLI)
	{
		if (maxhp > 0)
		{
			if( mhp > maxhp )
			{
				uLI->SetDenumerator(maxhp);
				uLI->SetNumerator(maxhp);
			} else {
				uLI->SetDenumerator(maxhp);
				uLI->SetNumerator(mhp);
			}
		} else {
			uLI->SetDenumerator(1);
			uLI->SetNumerator(0);
		}
	}
	targetName.Format("FriendHPBar%01i", pos);
	uLI = gGlobal.pUIManager->FindUILinerImage(targetName.c_str());
	if (uLI)
	{
		if (maxhp > 0)
		{
			if (hp > maxhp)
			{
				uLI->SetDenumerator(maxhp);
				uLI->SetNumerator(maxhp);
			} else {
				uLI->SetDenumerator(maxhp);
				uLI->SetNumerator(hp);
			}
		} else {
			uLI->SetDenumerator(1);
			uLI->SetNumerator(0);
		}
	}
}

VOID CParty::SetUIMemberSPBar(UInt16 pos, UInt16 sp, UInt16 msp, UInt16 maxsp)
{
	StringA targetName;
	UILinerImage * uLI;
	targetName.Format("FriendSPBarLim%01i", pos);
	uLI = gGlobal.pUIManager->FindUILinerImage(targetName.c_str());
	if (uLI)
	{
		if (maxsp > 0)
		{
			if (msp > maxsp)
			{
				uLI->SetDenumerator(maxsp);
				uLI->SetNumerator(maxsp);
			} else {
				uLI->SetDenumerator(maxsp);
				uLI->SetNumerator(msp);
			}
		} else {
			uLI->SetDenumerator(1);
			uLI->SetNumerator(0);
		}
	}
	targetName.Format("FriendSPBar%01i", pos);
	uLI = gGlobal.pUIManager->FindUILinerImage(targetName.c_str());
	if (uLI)
	{
		if (maxsp > 0)
		{
			if (sp > maxsp)
			{
				uLI->SetDenumerator(maxsp);
				uLI->SetNumerator(maxsp);
			} else {
				uLI->SetDenumerator(maxsp);
				uLI->SetNumerator(sp);
			}
		} else {
			uLI->SetDenumerator(1);
			uLI->SetNumerator(0);
		}
	}
}

VOID CParty::ResetNameColor()
{
	CCharacter * pchar = NULL;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		pchar = GetChild<CCharacter>(i);
		if (pchar == NULL) continue;
		CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByNetObject(pchar);
		if (pCtrl)
		{
			pCtrl->SetFontColor(TextColor[ColorIndex_LightBlue]);
			pCtrl->GetCharNameView()->SetFontColor(pCtrl->GetFontColor());
			//pCtrl->GetCharNameView()->SetFontColor(TextColor[ColorIndex_LightBlue]);
		}
	}
	/*for (StlMap<UInt32, CCharacterControl *>::iterator Itr = m_MemberList.begin();
		Itr != m_MemberList.end();
		Itr ++)
	{
		Itr->second->GetCharMainView()->SetExtraZBuffer(0);
	}

	if (m_TeamLeader)
		m_TeamLeader->GetCharMainView()->SetExtraZBuffer(1);
		*/
		
}

UInt32 CParty::FindMemberPos(String SrcName)
{
	CCharacter *pchar;
	UInt16 otherIdx = 1;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		pchar = GetChild<CCharacter>(i);
		if (pchar == NULL) continue;
		if (pchar == gGlobal.g_pCharacter->GetCharNetObject()) continue;
		if (pchar != gGlobal.g_pCharacter->GetCharNetObject() && pchar->GetNickName().compare(SrcName)==0 )
			return otherIdx;
		otherIdx++;
	}
	return 0;
}

int CParty::SetChangePos(UInt8 ui8PreIndex,UInt8 ui8DestIndex)
{
	UInt16 iCurMemNum = gGlobal.g_vctPartyMemberPointer.size();
	if (ui8PreIndex != 0 
		&& ui8PreIndex != ui8DestIndex 
		&& ui8DestIndex != 0
		&& ui8PreIndex < iCurMemNum
		&& ui8DestIndex < iCurMemNum) 
	{
		NetObject* pPreChar = gGlobal.g_vctPartyMemberPointer[ui8PreIndex];
		NetObject* pDestChar = gGlobal.g_vctPartyMemberPointer[ui8DestIndex];
		gGlobal.g_vctPartyMemberPointer[ui8PreIndex] = pDestChar;
		gGlobal.g_vctPartyMemberPointer[ui8DestIndex] = pPreChar;

		CPartyCharInfoClient* pPre=NULL;
		CPartyCharInfoClient* pDest=NULL;
		Global::PartyCharInfoMap::iterator itPre = gGlobal.g_mapPartyChar.find(ui8PreIndex);
		if (itPre != gGlobal.g_mapPartyChar.end())
		{
			pPre = itPre->second;
			gGlobal.g_mapPartyChar.erase(itPre);
		}
		Global::PartyCharInfoMap::iterator itDest = gGlobal.g_mapPartyChar.find(ui8DestIndex);
		if (itDest != gGlobal.g_mapPartyChar.end()) 
		{
			pDest = itDest->second;
			gGlobal.g_mapPartyChar.erase(itDest);
		}
		if (NULL != pPre) 
			gGlobal.g_mapPartyChar.insert(std::make_pair(ui8DestIndex,pPre));
		if (NULL != pDest)
			gGlobal.g_mapPartyChar.insert(std::make_pair(ui8PreIndex,pDest));
	}
	RedrawUI();
	if (NULL != gGlobal.g_pPartyProcess) 
		gGlobal.g_pPartyProcess->UpdatePartyPanel(this);
	return 0;
}
UInt16 CParty::GetRealMemNum()
{
	return GetCount();
}
CCharacter* CParty::GetInPartyMember(UInt16 u16Index)
{
	if (u16Index < GetCount())
		return GetChild<CCharacter>(u16Index);
	else
		return NULL;
}