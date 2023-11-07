#include "Common.h"
#include "QGL.h"
#include "UI.h"
#include "Global.h"
#include "Configure.h"
#include "CharacterControl.h"

#include "QGLApplication.h"
#include "Resource/ClientResManager.h"
#include "Proxy/Logic/Character.h"
#include "Proxy/Logic/Generation.h"
#include "GenerationProcess.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "MenuHelpProcess.h"
#include "Resource/ResNewbie.h"
#include "Resource/ResNewbieTips.h"
#include "Proxy/Logic/MessageCharacter.h"
#include "MenuFriendProcess.h"
#include "MenuTargetProcess.h"
#include "Proxy/Service/MessageService.h"

GENERATIONSORTTYPE  CGenerationProcess::m_gstCurSortType = SORT_MAX;
const int GENERATION_PER_PAGE_NUM =10;
CGenerationProcess::CGenerationProcess()
{
	m_gstCurSortType = SORT_MAX;
	m_iSelIndex = -1;
	m_ui32StarIndex = 0;
	m_uiTotalRecord = 0;
	m_ui32CurRequestId =0;
	m_bOpened = FALSE;
}
CGenerationProcess::~CGenerationProcess()
{
}
bool CGenerationProcess::CompareGeneration(CGeneration* pFrt,CGeneration* pSnd)
{
	bool bRet = false;
	switch(m_gstCurSortType) {
	case SORT_LEVEL:
		if (pFrt->GetUi16Level() > pSnd->GetUi16Level() 
			&& 0 == pFrt->GetUiOfflineTime())
			bRet = true;
		break;
	case SORT_CLASS:
		if (pFrt->GetUi8Class() > pSnd->GetUi8Class()
			&& 0 == pFrt->GetUiOfflineTime())
			bRet = true;
		break;
	case SORT_OFFLINE:
		if (pFrt->GetUiOfflineTime() < pSnd->GetUiOfflineTime()
			&& 0 == pFrt->GetUiOfflineTime())
			bRet = true;
		break;
	case SORT_ACCOREXP:
		if (pFrt->GetUiAccmulOrRecvExp() > pSnd->GetUiAccmulOrRecvExp()
			&& 0 == pFrt->GetUiOfflineTime())
			bRet = true;
		break;
	case SORT_GIFTORHISACC:
		{
			if (NULL != gGlobal.g_pCharacter) {
				CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
				if (NULL != pChar) {
					if (pChar->GetLevel() < 50) {
						if (pFrt->GetUiGiftPointOrHistAccm() > pSnd->GetUiGiftPointOrHistAccm()
							&& 0 == pFrt->GetUiOfflineTime())
							bRet = true;
					}
					else{
						if (pFrt->GetUiAccmulOrRecvExp() > pSnd->GetUiAccmulOrRecvExp()
							&& 0 == pFrt->GetUiOfflineTime())
							bRet = true;
					}
				}
			}
			break;
		}
	default:
		break;
	}

	return bRet;
}
void CGenerationProcess::SortListByLevel()
{
	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar) {
			m_gstCurSortType = SORT_LEVEL;
			std::sort(pChar->m_svtGeneration.begin(),
				pChar->m_svtGeneration.end(),
				CGenerationProcess::CompareGeneration);
			ShowGenerations();
		}
	}
}
void CGenerationProcess::SortListByClass()
{
	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar) {
			m_gstCurSortType = SORT_CLASS;
			std::sort(pChar->m_svtGeneration.begin(),
				pChar->m_svtGeneration.end(),
				CGenerationProcess::CompareGeneration);
			ShowGenerations();
		}
	}
}
void CGenerationProcess::SortListByOffline()
{
	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar) {
			m_gstCurSortType = SORT_OFFLINE;
			std::sort(pChar->m_svtGeneration.begin(),
				pChar->m_svtGeneration.end(),
				CGenerationProcess::CompareGeneration);
			ShowGenerations();
		}
	}
}
void CGenerationProcess::SortListByAccOrExp()
{
	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar) {
			m_gstCurSortType = SORT_ACCOREXP;
			std::sort(pChar->m_svtGeneration.begin(),
				pChar->m_svtGeneration.end(),
				CGenerationProcess::CompareGeneration);
			ShowGenerations();
		}
	}
}
void CGenerationProcess::SortListByGiftOrHisAcc()
{
	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar) {
			m_gstCurSortType = SORT_GIFTORHISACC;
			std::sort(pChar->m_svtGeneration.begin(),
				pChar->m_svtGeneration.end(),
				CGenerationProcess::CompareGeneration);
			ShowGenerations();
		}
	}
}
void CGenerationProcess::GetSavedExp()
{
	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar)
		{
			pChar->GetSavedExp(gGlobal.pPeerProxy,gGlobal.g_context);
		}
	}
}
void CGenerationProcess::DisbandGeneration()
{
	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar)
		{
			if (m_iSelIndex >=0 && m_iSelIndex < pChar->m_svtGeneration.size()) {
				CGeneration* pGnt = pChar->m_svtGeneration[m_iSelIndex];
				if (NULL != pGnt) {
					pChar->DeleteGeneration(gGlobal.pPeerProxy,gGlobal.g_context,pGnt->GetUiRelatedId());
				}
			}
		}
	}
}
void CGenerationProcess::RequestCreateGeneration(UInt32 ui32TargetId)
{
	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar)
		{
			pChar->RequestGeneration(gGlobal.pPeerProxy,gGlobal.g_context,ui32TargetId);
		}
	}
}
void CGenerationProcess::RequestGenerationAns(bool bAllow)
{
	if (m_ui32CurRequestId != 0) {
		BOOL bAllowTmp;
		if (bAllow) 
			bAllowTmp=TRUE;
		else
			bAllowTmp=FALSE;
		if (NULL != gGlobal.g_pCharacter) {
			CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
			if (NULL != pChar)
			{
				pChar->RequestGenerationAns(gGlobal.pPeerProxy,gGlobal.g_context,bAllowTmp,m_ui32CurRequestId);
			}
		}
		m_ui32CurRequestId = 0;
	}
}
void CGenerationProcess::ShowGenerations()
{
	if (!m_bOpened) 
		return;
	if (NULL == gGlobal.g_pCharacter) 
		return;
	CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
	if (NULL != pChar)
	{
		UIImage* pTitleImg = NULL,*pTitleImgInvisible=NULL;
		if (m_gstCurSortType == SORT_LEVEL){
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalLevelSel");
			pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalLevel");
		}
		else{
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalLevel");
			pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalLevelSel");
		}
		if (NULL != pTitleImg) 
			pTitleImg->SetEnable(true);
		if (NULL != pTitleImgInvisible) 
			pTitleImgInvisible->SetEnable(false);

		if (m_gstCurSortType == SORT_CLASS){
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalClassSel");
			pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalClass");
		}
		else{
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalClass");
			pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalClassSel");
		}
		if (NULL != pTitleImg) 
			pTitleImg->SetEnable(true);
		if (NULL != pTitleImgInvisible) 
			pTitleImgInvisible->SetEnable(false);

		if (m_gstCurSortType == SORT_OFFLINE){
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalOfflineSel");
			pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalOffline");
		}
		else{
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalOffline");
			pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalOfflineSel");
		}
		if (NULL != pTitleImg) 
			pTitleImg->SetEnable(true);
		if (NULL != pTitleImgInvisible) 
			pTitleImgInvisible->SetEnable(false);

		if (m_gstCurSortType == SORT_ACCOREXP){
			if (pChar->GetLevel() < 50) {
				pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalExpSel");
				pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalExp");
			}
			else
			{
				pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalAccSel");
				pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalAcc");
			}
		}
		else{
			if (pChar->GetLevel() < 50) {
				pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalExp");
				pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalExpSel");
			}
			else
			{
				pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalAcc");
				pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalAccSel");
			}
		}
		if (NULL != pTitleImg) 
			pTitleImg->SetEnable(true);
		if (NULL != pTitleImgInvisible) 
			pTitleImgInvisible->SetEnable(false);

		if (m_gstCurSortType == SORT_GIFTORHISACC){
			if (pChar->GetLevel() < 50) {
				pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalHisAccSel");
				pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalHisAcc");
			}
			else
			{
				pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalGiftSel");
				pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalGift");
			}
		}
		else{
			if (pChar->GetLevel() < 50) {
				pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalHisAcc");
				pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalHisAccSel");
			}
			else
			{
				pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalGift");
				pTitleImgInvisible = gGlobal.pUIManager->FindUIImage("GenerationTitalGiftSel");
			}
		}
		if (NULL != pTitleImg) 
			pTitleImg->SetEnable(true);
		if (NULL != pTitleImgInvisible) 
			pTitleImgInvisible->SetEnable(false);
		
		if (pChar->GetLevel() < 50) {
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalNameElder");
			if (NULL != pTitleImg) 
				pTitleImg->SetEnable(true);
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationPanelTitalYoung");
			if (NULL != pTitleImg) 
				pTitleImg->SetEnable(true);
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationSavedExp");
			if (NULL != pTitleImg) 
				pTitleImg->SetEnable(true);
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationElderNum");
			if (NULL != pTitleImg) 
				pTitleImg->SetEnable(true);
			UIButton* pButton = gGlobal.pUIManager->FindUIButton("GetSavedExp");
			if (NULL != pButton) 
				pButton->SetEnable(true);
			pButton = gGlobal.pUIManager->FindUIButton("RequestElder");
			if (NULL != pButton) 
				pButton->SetEnable(true);
			UIStaticText* pAccOrExpText = gGlobal.pUIManager->FindUIStaticText("GenerationAccOrExpValue");
			if (NULL != pAccOrExpText) {
				String strAccOrExp;
				strAccOrExp.Format(_T("%d"),pChar->GetCurAcc());
				pAccOrExpText->SetText(strAccOrExp);
				pAccOrExpText->Redraw();
			}
		}
		else{
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationTitalNameYoung");
			if (NULL != pTitleImg) 
				pTitleImg->SetEnable(true);
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationPanelTitalElder");
			if (NULL != pTitleImg) 
				pTitleImg->SetEnable(true);
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationCurAcc");
			if (NULL != pTitleImg) 
				pTitleImg->SetEnable(true);
			pTitleImg = gGlobal.pUIManager->FindUIImage("GenerationYoungNum");
			if (NULL != pTitleImg) 
				pTitleImg->SetEnable(true);
			UIButton* pButton = gGlobal.pUIManager->FindUIButton("AccToExtEqu");
			if (NULL != pButton) 
				pButton->SetEnable(true);
			pButton = gGlobal.pUIManager->FindUIButton("RequestYounger");
			if (NULL != pButton) 
				pButton->SetEnable(true);
			UIStaticText* pAccOrExpText = gGlobal.pUIManager->FindUIStaticText("GenerationAccOrExpValue");
			if (NULL != pAccOrExpText) {
				String strAccOrExp;
				strAccOrExp.Format(_T("%d"),pChar->GetCurAcc());
				pAccOrExpText->SetText(strAccOrExp);
				pAccOrExpText->Redraw();
			}
		}
		
		UIScrollBarV* pScrollBar = gGlobal.pUIManager->FindUIScrollBarV("GenerationScroll");
		if (NULL != pScrollBar) {
			if (pChar->m_svtGeneration.size() < GENERATION_PER_PAGE_NUM) 
				pScrollBar->SetEnable(false);
			else{
				pScrollBar->SetNumCount(pChar->m_svtGeneration.size()-GENERATION_PER_PAGE_NUM);
				pScrollBar->SetEnable(true);
			}
		}
		
		UIButton* pDisbandButton = gGlobal.pUIManager->FindUIButton("DisbandGeneration");
		if (NULL != pDisbandButton) {
			if (m_iSelIndex < 0 || m_iSelIndex >= pChar->m_svtGeneration.size()) {
				pDisbandButton->SetState(_ButtonDisable);
				pDisbandButton->SetNotDetected(true);
			}
		}
		m_uiTotalRecord = pChar->m_svtGeneration.size();
		int i = m_ui32StarIndex,iCnt=0;
		for (;i<pChar->m_svtGeneration.size();i++) {
			CGeneration* pGnt = pChar->m_svtGeneration[i];
			if (NULL != pGnt) {
				StringA strItemName,strText;
				strItemName.Format("GenerationName%02d",iCnt);
				UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
				if (NULL != pText) {
					if (0 == pGnt->GetUiOfflineTime()) 
						pText->SetFontColor(D3DCOLOR(0x0000FF00));
					else
						pText->SetFontColor(D3DCOLOR(0xFFFFFFFF));
					pText->SetText(pGnt->GetStrNickName().c_str());
					pText->Redraw();
				}
				if (pGnt->GetUiOfflineTime() == 0) {
					strItemName.Format("GenerationLevel%02d",iCnt);
					pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
					if (NULL != pText) {
						pText->SetFontColor(D3DCOLOR(0x0000FF00));
						strText.Format("%d",pGnt->GetUi16Level());
						pText->SetTextA(strText.c_str());
						pText->Redraw();
					}
					strItemName.Format("GenerationClass%02d",iCnt);
					pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
					if (NULL != pText) {
						String tempClassName;
						tempClassName.Format(_T("MSG_ACTOR_CLASS_%d"), pGnt->GetUi8Class());
						pText->SetFontColor(D3DCOLOR(0x0000FF00));
						pText->SetText(gGlobal.GetStringTable()->Get(tempClassName.c_str()));
						pText->Redraw();
					}
					strItemName.Format("GenerationTime%02d",iCnt);
					pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
					if (NULL != pText) {
						time_t now = time(NULL);
						UInt32 diff = (UInt32)(now - pGnt->GetUiOfflineTime());
						String strTimeText;
						strTimeText=gGlobal.GetStringTable()->Get(_T("MSG_ONLINE"));
						pText->SetFontColor(D3DCOLOR(0x0000FF00));
						pText->SetText(strTimeText);
						pText->Redraw();
					}
					strItemName.Format("GenerationAccOrExp%02d",iCnt);
					pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
					if (NULL != pText) {
						strText.Format("%d",pGnt->GetUiAccmulOrRecvExp());
						pText->SetFontColor(D3DCOLOR(0x0000FF00));
						pText->SetTextA(strText.c_str());
						pText->Redraw();
					}
					strItemName.Format("GenerationGiftOrAcc%02d",iCnt);
					pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
					if (NULL != pText) {
						String strStarText;
						if (pGnt->GetBYoung()) {
							UInt32 uHis=pGnt->GetUiGiftPointOrHistAccm();
							if (uHis < 1000) {
								strStarText.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_STAR_1")));
							}
							else if (uHis < 5000) {
								strStarText.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_STAR_2")));
							}
							else if (uHis < 30000) {
								strStarText.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_STAR_3")));
							}
							else if (uHis < 100000) {
								strStarText.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_STAR_4")));
							}
							else
								strStarText.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_STAR_5")));
						}
						else{
							UInt32 uHis=pGnt->GetUiAccmulOrRecvExp();
							if (uHis < 1000) {
								strStarText.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_STAR_1")));
							}
							else if (uHis < 3000) {
								strStarText.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_STAR_2")));
							}
							else if (uHis < 7000) {
								strStarText.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_STAR_3")));
							}
							else if (uHis < 130000) {
								strStarText.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_STAR_4")));
							}
							else
								strStarText.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_STAR_5")));
						}
						pText->SetFontColor(D3DCOLOR(0x0000FF00));
						pText->SetText(strStarText);
						pText->Redraw();
					}
				}
				else{
					String strOffline;
					strOffline.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_OFFLINE_DESC")));
					strItemName.Format("GenerationLevel%02d",iCnt);
					pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
					if (NULL != pText) {
						pText->SetFontColor(D3DCOLOR(0xFFFFFFFF));
						pText->SetText(strOffline);
						pText->Redraw();
					}
					strItemName.Format("GenerationClass%02d",iCnt);
					pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
					if (NULL != pText) {
						pText->SetFontColor(D3DCOLOR(0xFFFFFFFF));
						pText->SetText(strOffline);
						pText->Redraw();
					}
					strItemName.Format("GenerationTime%02d",iCnt);
					pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
					if (NULL != pText) {
						pText->SetFontColor(D3DCOLOR(0xFFFFFFFF));
						pText->SetText(strOffline);
						pText->Redraw();
					}
					strItemName.Format("GenerationAccOrExp%02d",iCnt);
					pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
					if (NULL != pText) {
						pText->SetFontColor(D3DCOLOR(0xFFFFFFFF));
						pText->SetText(strOffline);
						pText->Redraw();
					}
					strItemName.Format("GenerationGiftOrAcc%02d",iCnt);
					pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
					if (NULL != pText) {
						pText->SetFontColor(D3DCOLOR(0xFFFFFFFF));
						pText->SetText(strOffline);
						pText->Redraw();
					}
				}
				iCnt++;
				if (iCnt >= GENERATION_PER_PAGE_NUM)
					break;
			}
		}

		if (iCnt < GENERATION_PER_PAGE_NUM) {
			for (;iCnt < GENERATION_PER_PAGE_NUM;iCnt++) {
				StringA strItemName,strText("");
				strItemName.Format("GenerationName%02d",i);
				UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
				if (NULL != pText) {
					pText->SetTextA(strText.c_str());
					pText->Redraw();
				}
				strItemName.Format("GenerationLevel%02d",i);
				pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
				if (NULL != pText) {
					pText->SetTextA(strText.c_str());
					pText->Redraw();
				}
				strItemName.Format("GenerationClass%02d",i);
				pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
				if (NULL != pText) {
					pText->SetTextA(strText.c_str());
					pText->Redraw();
				}
				strItemName.Format("GenerationTime%02d",i);
				pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
				if (NULL != pText) {
					pText->SetTextA(strText.c_str());
					pText->Redraw();
				}
				strItemName.Format("GenerationAccOrExp%02d",i);
				pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
				if (NULL != pText) {
					pText->SetTextA(strText.c_str());
					pText->Redraw();
				}
				strItemName.Format("GenerationGiftOrAcc%02d",i);
				pText = gGlobal.pUIManager->FindUIStaticText(strItemName.c_str());
				if (NULL != pText) {
					pText->SetTextA(strText.c_str());
					pText->Redraw();
				}
			}
		}

		UIStaticText* pGeneratioNumText = gGlobal.pUIManager->FindUIStaticText("GenerationNumValue");
		if (NULL != pGeneratioNumText) {
			String strGenerationNumText;
			strGenerationNumText.Format(_T("%d/30"),pChar->m_svtGeneration.size());
			pGeneratioNumText->SetText(strGenerationNumText);
			pGeneratioNumText->Redraw();
		}
	}
}
void CGenerationProcess::SelectGeneration(int index)
{
	if (index >= 0 && index < GENERATION_PER_PAGE_NUM  && m_uiTotalRecord != 0 && index < m_uiTotalRecord)
	{
		StringA iconName;
		iconName.Format("GenerationSelect%02d", index);
		UIImage* pSelectImg = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		UIImage* pHighLight = gGlobal.pUIManager->FindUIImage("GenerationSelected");

		if (pSelectImg && pHighLight)
		{
			pHighLight->SetOffsetXY(pHighLight->GetOffsetX(), pSelectImg->GetOffsetY());
			pHighLight->SetEnable(true);
		}
		m_iSelIndex = m_ui32StarIndex+index;
		UIButton* pDisbandButton = gGlobal.pUIManager->FindUIButton("DisbandGeneration");
		if (NULL != pDisbandButton) 
			pDisbandButton->SetState(_ButtonNormal);
			pDisbandButton->SetNotDetected(false);
	}
}
void CGenerationProcess::GenerationScroll(int scroll)
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("GenerationScroll");
	if (scrollBar)
	{
		switch (scroll)
		{
		case 0:
			{
				if (m_ui32StarIndex > 0)
					--m_ui32StarIndex;
			}
			break;
		case 1:
			{
				if (scrollBar->GetNumCount() > m_ui32StarIndex)
					++m_ui32StarIndex;
			}
			break;
		case 2:
			{
				if (scrollBar->GetNumCount() > 0)
					m_ui32StarIndex = scrollBar->GetSelNum();
			}
			break;
		}
		ShowGenerations();
	}
}
void CGenerationProcess::UpdateDisbandPanel()
{
	if (NULL == gGlobal.g_pCharacter) 
		return;
	CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
	if (NULL != pChar)
	{
		if (m_iSelIndex >=0 && m_iSelIndex < pChar->m_svtGeneration.size()) {
			CGeneration* pGnt = pChar->m_svtGeneration[m_iSelIndex];
			if (NULL != pGnt) {
				UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText("GenerationDisbandDesc");
				if (NULL != pText) {
					String strText;
					strText.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_DISBAND_PANEL_TEXT")),pGnt->GetStrNickName());
					pText->SetText(strText);
					pText->Redraw();
				}
			}
		}
	}
}
void CGenerationProcess::UpDateRequestPanel()
{
}
void CGenerationProcess::HideAllPanel()
{
	gGlobal.pUIManager->SetAllPanelDisable();
}
void CGenerationProcess::ShowAllPanel()
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(),"SetCloseAllPanelFlag", String(), false);

	gGlobal.pUIManager->RestoreAllPanelEnable();

	lua_getglobal(gGlobal.pUIManager->GetLuaState(), "g_CurrentShortCut"); 
	UIItem * item = NULL;
	if (lua_isnumber(gGlobal.pUIManager->GetLuaState(), -1)) 
	{
		Int CurrentShortCut = (Int)lua_tonumber(gGlobal.pUIManager->GetLuaState(), -1); 
		switch(CurrentShortCut)
		{
		case 0:
			item = gGlobal.pUIManager->GetUIItem("HShortCutPanel");
			item->SetEnable(true);
			break;
		case 1:
			item = (UIItem*)gGlobal.pUIManager->GetUIItem("VShortCutPanel");
			item->SetEnable(true);
			break;
		case 2:
			item = (UIItem*)gGlobal.pUIManager->GetUIItem("HideShortCutPanel");
			item->SetEnable(true);
			break;
		}
	}
}
void CGenerationProcess::UpdateGeneration(bool bForceOpen)
{
	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar)
		{
			pChar->GetGenerationGroup(gGlobal.pPeerProxy,gGlobal.g_context);
		}

		if (bForceOpen)
			m_bOpened = TRUE;
	}
}
void CGenerationProcess::SystemDesc(UInt16 uiContextId)
{
	if(NULL != gGlobal.g_pMenuHelpProcess)
	{
		gGlobal.g_pMenuHelpProcess->ShowContent(uiContextId,0,0);
	}
}
void CGenerationProcess::OpenGenCharDetail()
{
	if (NULL == gGlobal.g_pCharacter) 
		return;
	CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
	if (NULL != pChar)
	{
		if (m_iSelIndex >=0 && m_iSelIndex < pChar->m_svtGeneration.size()) {
			CGeneration* pGnt = pChar->m_svtGeneration[m_iSelIndex];
			if (NULL != pGnt) {
				if (pGnt->GetUiOfflineTime() == 0) {
					CMenuFriendProcess* pPprocessFrd = gGlobal.g_pMenuFriendProcess;
					if (NULL != pPprocessFrd) {
						CMessageCharacter * pInfo = pPprocessFrd->FindInfo(pGnt->GetUiRelatedId());
						if (pInfo)
						{
							gGlobal.g_MenuTargetProcess->ResetTargetDetail();
							gGlobal.g_MenuTargetProcess->SetFriendDetail(pInfo);
							gGlobal.g_MenuTargetProcess->ShowDetail();
							MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, pGnt->GetUiRelatedId(), String(), true, false);
							return;
						}
					}

					CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pGnt->GetUiRelatedId());
					if (pCtrl != NULL)
					{
						gGlobal.g_MenuTargetProcess->ResetTargetDetail();
						gGlobal.g_MenuTargetProcess->SetFriendDetail(pCtrl);
						gGlobal.g_MenuTargetProcess->ShowDetail();
						MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, pGnt->GetUiRelatedId(), String(), true, false);
						return;
					}

					MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, pGnt->GetUiRelatedId(), String(), true, false);
				}
				else{//Charactor Offline
					String msg;
					msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHAT_MSG_ERROR_NOT_ONLINE_UPDATE")));
					gGlobal.PrintPlayerMessage(msg);
				}
			}
		}
	}
}