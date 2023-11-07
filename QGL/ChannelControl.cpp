//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "ChannelControl.h"
//-- Library
#include "Global.h"
#include "Configure.h"
#include "CharacterControl.h"
#include "Common/ChannelType.h"
#include "Proxy/Service/MessageService.h"
#include "QGL.h"
#include "QGLApplication.h"
#include "Math.h"
#include "Global.h"
#include "Resource/ResPet.h"

CChannelControl::CChannelControl()
{
	pChannel = NULL;
	m_Listen = TRUE;
}

CChannelControl::~CChannelControl()
{
	if (pChannel)
	{
		DeleteNetObject(pChannel);
	}
}

VOID CChannelControl::NotifyReceiveMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName)
{
	gGlobal.PrintMessage(Msg, m_ChannelType, SrcName);
}

VOID CChannelControl::SetChannelNetObject(CChannel * channel)
{
	if (channel == NULL)
		return;

	pChannel = channel;
	pChannel->SetView(this);
}

VOID CChannelControl::SendChannelMessage(String Msg)
{
	//check previous time
	UInt32 curTime = (UInt32)time(NULL);
	if (curTime < gGlobal.g_PreSpeakTime[m_ChannelType] + SpeakRestTime[m_ChannelType])
	{
		String text;
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_TOOFREQ")), (gGlobal.g_PreSpeakTime[m_ChannelType] + SpeakRestTime[m_ChannelType] - curTime));
		gGlobal.PrintMessage(text.c_str(), CHANNELTYPE_SYSTEM);
		return;
	}
	gGlobal.AddColorPrefix(Msg);

	if (pChannel && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	{
		if (!m_Listen)
		{
			m_Listen = TRUE;
			pChannel->SetListen(gGlobal.pPeerProxy, gGlobal.g_context, m_Listen,
			gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id());
			
			UICheckBox * pBuRec = gGlobal.pUIManager->FindUICheckBox(ReceiveChannelXMLName[m_ChannelType]);
			if (pBuRec)
			{
				pBuRec->SetState(FALSE);
				gGlobal.pUIManager->AddDirtyUIItem(pBuRec);
			}

			if (QGLApplication::GetInstance().GetSubWindow()->m_UIManager)
			{
				UICheckBox * pSubBuRec = QGLApplication::GetInstance().GetSubWindow()->m_UIManager
					->FindUICheckBox(SubReceiveChannelXMLName[m_ChannelType]);
				if (pSubBuRec)
				{
					pSubBuRec->SetState(FALSE);
					QGLApplication::GetInstance().GetSubWindow()->m_UIManager->AddDirtyUIItem(pSubBuRec);
				}
				
			}

		}

		gGlobal.g_PreSpeakTime[m_ChannelType] = curTime;
		pChannel->SendGrpMessage(gGlobal.pPeerProxy, gGlobal.g_context, Msg, gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id(), BOOLEAN_FALSE);
	}
}

VOID CChannelControl::NotifyCB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const UInt32  &resultFlag)
{
	if (Result == MSG_RETURN_FAIL_TOO_FREQUENT)
	{
		String text;
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_TOOFREQ")), resultFlag);
		gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM, String());
	}
	else if (Result == MSG_RETURN_FAIL_NO_RIGHT_NO_COUNT)
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_FAIL_NO_RIGHT_NO_COUNT")), CHANNELTYPE_SYSTEM, String());
	else if (Result == MSG_RETURN_FAIL_NO_RIGHT_LV)
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_FAIL_NO_RIGHT_LV")), CHANNELTYPE_SYSTEM, String());
	else if (Result == MSG_RETURN_FAIL_NO_RIGHT_MONEY)
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_FAIL_NO_RIGHT_MONEY")), CHANNELTYPE_SYSTEM, String());
	else if (Result == MSG_RETURN_FAIL_NO_RIGHT_MONEY_GANG)
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_FAIL_NO_RIGHT_MONEY_GANG")), CHANNELTYPE_SYSTEM, String());
	else if (Result == MSG_RETURN_SUCCESS_COST_MONEY)
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_SUCCESS_COST_MONEY")), CHANNELTYPE_SYSTEM, String());
	else if (Result == MSG_RETURN_SUCCESS_COST_MONEY_GANG)
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_SUCCESS_COST_MONEY_GANG")), CHANNELTYPE_SYSTEM, String());
	else if (Result == MSG_RETURN_FAIL_NO_RIGHT_BLOCK)
	{
		String text;
		UInt32 count = gGlobal.g_pCharacter->GetCharNetObject()->GetSpeakBlock();
		String remainText;

		if (count >= 86400)
			remainText.Format(_T("%d%s"), count / 86400,gGlobal.GetStringTable()->Get(_T("MSG_DAY")));
		else if (count >= 3600)
			remainText.Format(_T("%d%s"), count / 86400,gGlobal.GetStringTable()->Get(_T("MSG_HOUR")));
		else
			remainText.Format(_T("%d%s"), count / 86400,gGlobal.GetStringTable()->Get(_T("MSG_SECOND")));
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_FAIL_NO_RIGHT_BLOCK")), remainText.c_str());
		gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM, String());
	}
}


VOID CChannelControl::SendPrivateMessage(String Msg, String TargetName, bool IsFriend)
{
	if (pChannel && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	{
		MessageService_Proxy::SendPrivateMessage(gGlobal.pPeerProxy, gGlobal.g_context, Msg, 
		gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id(), 0, TargetName, IsFriend);
	}
}

VOID CChannelControl::SetListenToChannel(bool Listen)
{
	if (pChannel && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	{
		m_Listen = Listen;
		pChannel->SetListen(gGlobal.pPeerProxy, gGlobal.g_context, Listen,
		gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id());
	}
}

VOID CChannelControl::NotifyOnUpdateInfo(const BYTE &ChannelType)
{
	m_ChannelType = ChannelType;
}

VOID CChannelControl::NotifyCB_LeaveChannel()
{
	if (pChannel)
	{
		DeleteNetObject(pChannel);
		pChannel = NULL;

	}
	gGlobal.RemoveChannelCtrl(this);
}


void CChannelControl::Update(UInt32 CurrTime)
{
	UInt count = (UInt)gGlobal.g_EmotionList.count(m_ChannelType);
	StlVector<StlMultiMap<BYTE, emotionData>::iterator> rmLst;
	StlMultiMap<BYTE, emotionData>::iterator Itr = gGlobal.g_EmotionList.find(m_ChannelType);
	for (UInt i = 0; i < count && Itr != gGlobal.g_EmotionList.end(); ++i, ++Itr)
	{
		SendEmotion(Itr->second.emo_id, Itr->second.SrcID, Itr->second.TargetID, Itr->second.isPet);
		rmLst.push_back(Itr);
	}

	for (UInt i = 0; i < rmLst.size(); i++)
		gGlobal.g_EmotionList.erase(rmLst[i]);
}
VOID CChannelControl::SendEmotion(UInt32 emo_id, UInt32 SrcID, UInt32 TarID, bool isPet)
{
	if (pChannel)
	{
		pChannel->SendEmotion(gGlobal.pPeerProxy, gGlobal.g_context, emo_id, SrcID, TarID, isPet);
	}
}
