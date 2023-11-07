//-- Common
#include "Common.h"
//-- Self
#include "RumorMsgProcess.h"
//-- Library
#include "ui.h"
#include "Global.h"
#include "XMLPanel.h"
#include "Configure.h"

PSTRINGA RunmorMsgPanel = "RumorMessage";
PSTRINGA RunmorMsgContent = "RumorMessageContent";
PSTRINGA RunmorMsgXML = "Data\\RumorMessage.xml";

CRumorMsgProcess::CRumorMsgProcess()
{
	m_CurrCount = 0;

	
}

CRumorMsgProcess::~CRumorMsgProcess()
{
}

VOID CRumorMsgProcess::ShowRumorMessage(String GivMsg)
{
	UIPanel * pPanel = gGlobal.pUIManager->FindUIPanel(RunmorMsgPanel);
	if (pPanel == NULL)
	{
		XML_UI::GetInstance().Init();
		XML_UI::GetInstance().ReadFormFile(RunmorMsgXML, gGlobal.pUIManager);
	}

	//format message
	String Msg;
	UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("RumorMessageChat");
	if (pChat)
	{
		pChat->ClearAllRow();
		pChat->AddStringW(GivMsg.c_str());

		UInt index = 0;
		while (true)
		{
			UIChatRow * pChatRow = pChat->GetRow(index);
			if (pChatRow && pChatRow->GetRowItemList())
			{
				if (index != 0)
					Msg += _T("\r");
				for (UInt i = 0; i < pChatRow->GetRowItemList()->size(); i ++)
				{
					UIChatString * pChatString = (UIChatString * )pChatRow->GetChatItem(i);
					if (pChatString)
					{
						Msg += pChatString->GetString();
					}
				}
			}
			else
			{
				break;
			}
			index ++;
		}
	}

	m_CurrCount ++;
	if (m_CurrCount > MAX_RUMOR_MSG)
		m_CurrCount = MAX_RUMOR_MSG;

	UInt8 count = 0;
	if (m_CurrCount > 0)
	{
		//shift up
		for (Int i = m_CurrCount - 1; i > 0; i--)
		{
			//m_MsgDataLst[i].m_StartTime = 1;
			m_MsgDataLst[i].Msg = _T(" ");

			int shiftRow = 0;
			if (m_MsgDataLst[i - 1].Msg.length() > 20 && i < (MAX_RUMOR_MSG - 1) )
			{
				++shiftRow;
				if ((i + shiftRow) > (m_CurrCount - 1))
					count = i + shiftRow + 1;
			}
	
			m_MsgDataLst[i + shiftRow].m_StartTime = m_MsgDataLst[i - 1].m_StartTime;
			m_MsgDataLst[i + shiftRow].Msg = m_MsgDataLst[i - 1].Msg;
			m_MsgDataLst[i + shiftRow].m_AlphaValue = 0;
		}
	}
	if (count > 0)
		m_CurrCount = count;
	m_MsgDataLst[0].m_StartTime = ::timeGetTime();
	m_MsgDataLst[0].m_AlphaValue = 0;
	m_MsgDataLst[0].Msg = Msg;


	
	
	/*UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(RunmorMsgContent.c_str());
	if (pText)
	{
	pText->SetColor(D3DCOLOR_ARGB(m_AlphaValue, 255, 255, 255));
	if (!Msg.IsEmpty())
	{
	pText->SetText(Msg);
	pText->Redraw();

	}
	}*/

	for (Int i = 0; i < m_CurrCount; i++)
	{
		StringA TempName;
		TempName.Format("%s%d", RunmorMsgContent, i);
		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(TempName.c_str());
		if (pText)
		{
			if (!m_MsgDataLst[i].Msg.IsEmpty())
			{
				pText->SetText(m_MsgDataLst[i].Msg);
				pText->Redraw();
			}

			pText->SetEnable(true);
			//gGlobal.pUIManager->AddDirtyUIItem(pText);
		}
	}

	Update(m_MsgDataLst[0].m_StartTime);
}
VOID CRumorMsgProcess::Update(UInt32 CurrTime)
{
	for (Int i = 0; i < MAX_RUMOR_MSG; i++)
	{
		if (m_MsgDataLst[i].m_StartTime == 0)
			continue;

		if (m_MsgDataLst[i].m_StartTime > CurrTime)
			m_MsgDataLst[i].m_StartTime = CurrTime;


		if (CurrTime > m_MsgDataLst[i].m_StartTime + gConfigure.uRumorMsgAppearTime + gConfigure.uRumorMsgExistTime)
		{
			Real32 alphaFactor = (Real32)(CurrTime - (m_MsgDataLst[i].m_StartTime + gConfigure.uRumorMsgAppearTime + gConfigure.uRumorMsgExistTime)) 
				/ (Real32)gConfigure.uRumorMsgDisposalTime;

			if (alphaFactor >= 1)
			{
				m_MsgDataLst[i].m_StartTime = 0;
				m_MsgDataLst[i].m_AlphaValue = 0;
				//gGlobal.pUIManager->AddDirtyUIItem(pPanel);
				//gGlobal.pUIManager->DeletePanel(RunmorMsgPanel.c_str());
				//XML_UI::GetInstance().ReadFormFile(FileName, m_pUIManager);
				StringA TempName;
				TempName.Format("%s%d", RunmorMsgContent, i);
				UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(TempName.c_str());
				if (pText)
				{
					pText->SetEnable(false);
					gGlobal.pUIManager->AddDirtyUIItem(pText);
				}
				m_CurrCount --;
			}
			else
			{
				BYTE Temp = 255 - (BYTE)(255 * alphaFactor);
				if (Temp != m_MsgDataLst[i].m_AlphaValue)
				{
					m_MsgDataLst[i].m_AlphaValue = Temp;
					StringA TempName;
					TempName.Format("%s%d", RunmorMsgContent, i);
					UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(TempName.c_str());
					if (pText)
					{
						pText->SetColor(D3DCOLOR_ARGB(m_MsgDataLst[i].m_AlphaValue, 255, 255, 255));
						gGlobal.pUIManager->AddDirtyUIItem(pText);
					}
				}
			}

		}
		else if (CurrTime > m_MsgDataLst[i].m_StartTime + gConfigure.uRumorMsgAppearTime)
		{
			if ( m_MsgDataLst[i].m_AlphaValue != 255)
			{
				m_MsgDataLst[i].m_AlphaValue = 255;
				StringA TempName;
				TempName.Format("%s%d", RunmorMsgContent, i);
				UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(TempName.c_str());
				if (pText)
				{
					pText->SetColor(D3DCOLOR_ARGB(m_MsgDataLst[i].m_AlphaValue, 255, 255, 255));
					gGlobal.pUIManager->AddDirtyUIItem(pText);
				}
			}


		}
		else
		{
			Real32 alphaFactor = (Real32)(CurrTime - m_MsgDataLst[i].m_StartTime) / (Real32)gConfigure.uRumorMsgAppearTime;

			if (alphaFactor >= 1)
				alphaFactor = 1;

			BYTE Temp = (BYTE)(255 * alphaFactor);
			if (Temp != m_MsgDataLst[i].m_AlphaValue)
			{
				m_MsgDataLst[i].m_AlphaValue = Temp;
				StringA TempName;
				TempName.Format("%s%d", RunmorMsgContent, i);
				UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(TempName.c_str());
				if (pText)
				{
					pText->SetColor(D3DCOLOR_ARGB(m_MsgDataLst[i].m_AlphaValue, 255, 255, 255));
					gGlobal.pUIManager->AddDirtyUIItem(pText);
				}
			}

		}
	}
}