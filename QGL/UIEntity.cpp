//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "UIEntity.h"
//-- Library
#include "Global.h"
#include "Math.h"
#include "Render.h"
#include "XMLPanel.h"
#include "QGLApplication.h"

CUIEntity::CUIEntity()
{
	//get the cache memory
	m_CurrTexture = CResourcesManager::GetInstance().GetEntPicCacheTexture();
	//m_pUIManager = SafeCreateObject(UIManager);
	//m_pUIManager->SetRender(QGLApplication::GetInstance().GetRenderClass());
	//m_pUIManager->GetDirtyRectMan()->SetBoundRect(0, 0, 800, 600);
	m_pChatPanel = NULL;
	m_pPanelEx = NULL;
}

CUIEntity::~CUIEntity()
{
	/*
	if (m_pUIManager)
		SafeDeleteObject(m_pUIManager);
	*/
	//m_XMLFileNameLst.clear();
	SafeDeleteObject(m_pPanelEx);
}

Int CUIEntity::GetTotalRenderPart()
{
	//image only have one content
	return 1;
}
BOOL CUIEntity::GetEntityPart(size_t index, Int ImageType, RECT * LockRegion)
{
 	RECT EntLockRegion = m_EntityTotalRect;

	if (LockRegion)
	{
		EntLockRegion = *LockRegion;
		EntLockRegion.left -= GetEntLeftUpPos().PixelPosX;
		EntLockRegion.right -= GetEntLeftUpPos().PixelPosX;
		EntLockRegion.top -= GetEntLeftUpPos().PixelPosY;
		EntLockRegion.bottom -= GetEntLeftUpPos().PixelPosY;
	}

	if (EntLockRegion.top < 0)
	{
		EntLockRegion.bottom -= EntLockRegion.top;
		EntLockRegion.top = 0;
	}

	RECT ResultRect;

	if (QGLMath::CalculateOverlappedRect(EntLockRegion, m_EntityTotalRect, ResultRect))
	{
		UInt Len = (ResultRect.right - ResultRect.left) * 4;
		BYTE * pDesData = m_CurrTexture + (ResultRect.top * MaxEntSizeX + ResultRect.left) * 4;
		m_CurrPicData = pDesData;
		for (Int y = 0; y < ResultRect.bottom - ResultRect.top; y ++)
		{
			ZeroMemory(pDesData, Len);
			pDesData += MaxEntSizeX * 4;
		}
		CImage * pIm = SafeCreateObject4(CImage, m_CurrTexture, MaxEntSizeX, MaxEntSizeY, 8888);
		/*
		for(std::vector<UIPanel *>::iterator i = m_pUIManager->GetUIPanelListPtr()->begin(); i != m_pUIManager->GetUIPanelListPtr()->end(); ++i)
		{
			if((*i)->IsEnable())
				(*i)->Render(pIm, ResultRect);
		}
		*/
		
		m_pPanelEx->Render(pIm, ResultRect);

		SafeDeleteObject(pIm);
		return TRUE;
	}
	return FALSE;
}

BOOL CUIEntity::GetHaveEffect()
{
	return FALSE;
}

VOID CUIEntity::UpDate(UInt32 CurrentTime)
{
	/*
	if (m_pUIManager == NULL)
		return;
	*/
	if(!m_pPanelEx)
		return;

	CEntityBase::UpDate(CurrentTime);
	if(m_pChatPanel->Update(CurrentTime))
		m_Updated = TRUE;
	/*
	m_pUIManager->Update(CurrentTime);
	if (m_pUIManager->GetDirtyRectLst().size() > 0)
		m_Updated = TRUE;
	m_pUIManager->ClearDirtyRectLst();
	*/
}

VOID CUIEntity::ReadXMLData(PCSTRINGA FileName)
{
	//m_XMLFileNameLst.push_back(StringA(FileName));
	//m_pUIManager->SetupLua(gGlobal.pUIManager->GetLuaState(), true);
	XML_UI::GetInstance().Init();
	//XML_UI::GetInstance().ReadFormFile(FileName, m_pUIManager);
	XML_UI::GetInstance().ReadFormFile(FileName);
	m_pPanelEx = (UIPanelEx *)XML_UI::GetInstance().GetUI(0);
	m_pPanelEx->SetTexture(QGLApplication::GetInstance().GetRenderClass()->GetDevice());
	m_pChatPanel = (UIChatPanel *)m_pPanelEx->GetSubItem(0);
	XML_UI::GetInstance().ClearPanelList();
	m_Updated = TRUE;
	ResetRect();
}

VOID CUIEntity::ResetRect()
{
	SetRect(&m_CurrRect, 0, 0, 0, 0);

	/*
	for(std::vector<UIPanel *>::iterator i = m_pUIManager->GetUIPanelListPtr()->begin(); i != m_pUIManager->GetUIPanelListPtr()->end(); ++i)
	{
		if (m_CurrRect.right < (LONG)(*i)->GetWidth())
			m_CurrRect.right = (LONG)(*i)->GetWidth();

		if (m_CurrRect.bottom < (LONG)(*i)->GetHeight())
			m_CurrRect.bottom = (LONG)(*i)->GetHeight();
	}
	*/

	
	if(m_pPanelEx)
	{
		if (m_CurrRect.right < (LONG)m_pPanelEx->GetWidth())
			m_CurrRect.right = (LONG)m_pPanelEx->GetWidth();

		if (m_CurrRect.bottom < (LONG)m_pPanelEx->GetHeight())
			m_CurrRect.bottom = (LONG)m_pPanelEx->GetHeight();
	}

	SetRect(&m_EntityTotalRect, 0, 0, m_CurrRect.right, m_CurrRect.bottom);
	m_EntOrgHeight = (UInt16)m_CurrRect.bottom;
	m_EntOrgWidth = (UInt16)m_CurrRect.right;
}


VOID CUIEntity::SetupPanel()
{
	if(m_pChatPanel)
	{
		m_pChatPanel->ClearAllRow();
		m_pChatPanel->AddStringWNoPrefix(m_Msg.c_str());
		m_pChatPanel->Start2BottomRow();

		if (m_pChatPanel->GetEndRowIndex() != m_pChatPanel->RowListCount() - 1)
		{
			UIChatRow * pChatRow = m_pChatPanel->GetRow(m_pChatPanel->GetEndRowIndex());
			
			if (pChatRow)
			{
				pChatRow->ResetChatItemsPos();
				//remove last two chat item and insert two . .

				UIChatItem * pChatItem = pChatRow->GetChatItem((UInt32)pChatRow->GetRowItemList()->size() - 1);
				if (pChatItem)
				{
					UIChatAniIcon * pChatAniIcon = dynamic_cast<UIChatAniIcon *>(pChatItem);
					if (pChatAniIcon)
					{
						//Char FileName[50];
						//FileName = _T("..");
						//GetPrivateProfileString(_T("LOADFILE"), _T("MESSAGEOVER"), _T(".."), FileName, 50, CONFIGURATION_INI_FILE);

						StringW WStr;
						UInt32 Len;
						m_pChatPanel->GetFont()->DrawFontStrC(_T(".........................................."), WStr, pChatItem->GetPosX() + pChatAniIcon->GetWidth() + 5, 0, 
							m_pChatPanel->GetWidth(), m_pChatPanel->GetPitch(), m_pChatPanel->GetFontStyle(), Len);

						//pChatRow->GetRowItemList()->pop_back();
						UIChatString *MyStr = SafeCreateObject(UIChatString);
						MyStr->SetString(WStr.c_str());
						MyStr->SetFontColor(m_pChatPanel->GetFontColor());
						MyStr->SetShadowColor(m_pChatPanel->GetShadowColor());
						MyStr->SetFontStyle(m_pChatPanel->GetFontHeight());
						MyStr->SetWidth(m_pChatPanel->GetFont()->GetLastDrawWidth() - pChatItem->GetPosX());

						UInt32	PitchHeight, RealFontHeight;
						m_pChatPanel->GetFont()->TakePitchHeight(m_pChatPanel->GetFontStyle(), PitchHeight, RealFontHeight);

						MyStr->SetHeight(m_pChatPanel->GetFont()->GetHeight() + PitchHeight);

						pChatRow->AddItem(MyStr);
					}
					else
					{
						UIChatString * pChatString = dynamic_cast<UIChatString *>(pChatItem);
						if (pChatString)
						{
							StringW OrgText = pChatString->GetString();

							if (OrgText.size() >= 3)
								OrgText.erase(OrgText.size() - 3);
							else
								OrgText.clear();

							//Char FileName[50];
							//FileName =  _T("..");
							//GetPrivateProfileString(_T("LOADFILE"), _T("MESSAGEOVER"), _T(".."), FileName, 50, CONFIGURATION_INI_FILE);
							OrgText.append(_T(".........................................."));

							StringW WStr;
							UInt32 Len;
							m_pChatPanel->GetFont()->DrawFontStrC(OrgText.c_str(), WStr, pChatItem->GetPosX(), 0, 
								m_pChatPanel->GetWidth(), m_pChatPanel->GetPitch(), m_pChatPanel->GetFontStyle(), Len);
							pChatString->SetString(WStr.c_str());

						}
					}

				}
			}
		}
		m_pChatPanel->HomeRow();
	}

	if (m_pPanelEx && (m_pPanelEx->GetLineRect()->bottom > m_pPanelEx->GetLineRect()->top))
	{

		UInt32 LineCount = (UInt32)ceil((Real32)m_pChatPanel->GetTotalHeight() / 
			(m_pPanelEx->GetLineRect()->bottom - m_pPanelEx->GetLineRect()->top));
		m_pPanelEx->ChangeCount(LineCount);
		ResetRect();
	}

	m_pPanelEx->SetUIManager(NULL);
}


