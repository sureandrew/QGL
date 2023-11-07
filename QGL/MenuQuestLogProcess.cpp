//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuQuestLogProcess.h"
//-- Library
#include "Global.h"
#include "XMLPanel.h"
#include "Configure.h"
#include "Proxy/Logic/QuestLog.h"
#include "Proxy/Logic/QuestType.h"
#include "Proxy/Logic/QuestSystem.h"
#include "UI.h"
#include "Font.h"
#include "QGLApplication.h"
#include "CharacterControl.h"
#include "AccountControl.h"
#include "BattleControl.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Resource/QGLCSVTReader.h"

#define SZ_QLM_CHECKBOX_MAINQUESTLIST					"QuestLineBtn"
#define SZ_QLM_STATICTEXT_QUESTNAME						"QuestLineText"
#define SZ_QLM_IMAGE_EXPENDSTATE						"QuestLineState"
#define SZ_QM_TITLE										"QuestMenuTitle"
#define SZ_QM_CHATPANEL_QUESTDESC						"QuestMenuDesc"
#define SZ_QM_CHATPANEL_NOTEDESC						"QuestNoteDesc"
#define SZ_QM_SCROLLV_QUESTDESC							"QMDesc_ScrollBarV"
#define SZ_QM_SCROLLV_QUESTNOTEDESC						"QMNoteDesc_ScrollBarV"
#define SZ_QM_SCROLLV_QUESTLIST							"QMQList_ScrollV"
#define SZ_QM_PANEL_NAME								"QuestLogMenuPanel"
#define SZ_QN_PANEL_NAME								"QuestLogNotePanel"
#define SZ_QM_CHECKBOX_GETTING							"GettingQuestLog"
#define SZ_QM_CHECKBOX_NOGETTING						"NoGettingQuestLog"

#define DATE_T_20070101				1167494400
#define FILENAME_LENGTH				1024

CMenuQuestLogProcess::CMenuQuestLogProcess()
{
	m_MenuRowLst.clear();
	m_DisplayRowLst.clear();
	m_tempSwitch.clear();
	m_Qtracelst.clear();
	
	m_preUpdate = 0;
	m_bSetDescFlag = BOOLEAN_FALSE;
	m_bSetMenuLstFlag = BOOLEAN_FALSE;
	m_bSetDisplayFlag = BOOLEAN_FALSE;
	m_bNoticeFail = BOOLEAN_FALSE;
	m_bQuestHelp = BOOLEAN_FALSE;
	m_nLastType = QUESTLOGTYPE_UNKNOWN;
	m_selQuest = NULL;
	m_selType = NULL;

	ReInit();
}

CMenuQuestLogProcess::~CMenuQuestLogProcess()
{
	Release();
}

void CMenuQuestLogProcess::ReInit()
{
	m_CurrSelQuestId = 0;
	m_CurrSelTypeId = 0;
	m_nSelStartIndex = 0;
	m_nCurrSelIndex = -1;
}

VOID CMenuQuestLogProcess::Release()
{
	ClearQuestMenuLstData();
	ClearDisplayLstData();
	ClearNoteDisplayLstData();
	ClearNoFinishCsvData();
	SaveHistory();
}

VOID CMenuQuestLogProcess::ClearQuestMenuLstData()
{
	m_tempSwitch.clear();
	for (QMenuRowLst::iterator it = m_MenuRowLst.begin(); it != m_MenuRowLst.end(); ++it )
	{
		if ( it->second )
		{
			it->second->itemLst.clear();
		}
		SafeDeleteObject(it->second);
	}
	m_MenuRowLst.clear();
}

VOID CMenuQuestLogProcess::HighLightSelectRow(Int row)
{
	StringA temp("");
	temp.Format("%s%d", SZ_QLM_CHECKBOX_MAINQUESTLIST, row);

	UICheckBox * pChkBox = gGlobal.pUIManager->FindUICheckBox(temp.c_str());
	if (pChkBox)
		pChkBox->SetState(true);
}

VOID CMenuQuestLogProcess::ReDrawAllMenuItem()
{
	Int showNum = 0;
	{
		Int nTotalItem = (Int)m_DisplayRowLst.size();
		if ( m_nSelStartIndex < nTotalItem )
		{
			Int diffNum = nTotalItem - m_nSelStartIndex;
			if ( diffNum > MAX_MAIN_QUESTLOG )
				showNum = MAX_MAIN_QUESTLOG;
			else
				showNum = diffNum;
		}
	}

	QMDisPlayRow* pDisplayRow = NULL;
	Int qindex = 0;
	for(Int i = 0; i < showNum; ++i )
	{
		qindex = m_nSelStartIndex + i;
		pDisplayRow = m_DisplayRowLst[qindex];
		if( pDisplayRow )
		{
			switch( pDisplayRow->type )
			{
			case QUESTMENU_FOLDER:
				{
					if ( pDisplayRow->bExpended == false )
						SelectMainTypeHandle(true, i+1, qindex);
					else
						SelectMainTypeHandle(false, i+1, qindex);
				}
				break;
			case QUESTMENU_ITEM:
				{
					ReDrawSelMenuItem(i, pDisplayRow);
				}
				break;
			}
		}
	}
}

VOID CMenuQuestLogProcess::ReDrawSelMenuItem(Int displayIndex, const QMDisPlayRow* pRow, bool bRedraw)
{
	if ( !pRow ) return;
	StringA compName(_T(""));
	compName.Format("%s%i", SZ_QLM_STATICTEXT_QUESTNAME, displayIndex+1);
	UIStaticText* pST = gGlobal.pUIManager->FindUIStaticText(compName.c_str());
	if( pST )
	{
		String text(_T(""));
		text.Format(_T("%s"), pRow->title.c_str());
		if( IsQuestCompleted(pRow->questId) )
		{
			text.append(gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_COMPLETED")));
			pST->SetColor(TextColor[ColorIndex_Green]);
		}
		else if( IsQuestFailed(pRow->questId) )
		{
			text.append(gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_FAILED")));
			pST->SetColor(RedColor);
		}
		else if ( pST->GetColor() != TextColor[ColorIndex_White] )
		{
			pST->SetColor(TextColor[ColorIndex_White]);
		}
		pST->SetText(text);

		pST->Redraw();

		if ( bRedraw )
			gGlobal.pUIManager->AddDirtyUIItem(pST);
	}
}

void CMenuQuestLogProcess::UpdateMainQuestLstUI(int index, bool bClearDesc)
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel(SZ_QM_PANEL_NAME); 
	if ( !pPanel ) return;
	if ( pPanel->IsEnable() == false )	return;

	ResetMainCheckBoxState();

	if ( m_DisplayRowLst.empty() )
	{
		m_nCurrSelIndex = -1;
		ResetMainQuestLstUI();
		RefreshMainLstScrollV();
		UpdateSubDescUI(true);
		DisEnableQuestRemainTime();
		RefreshQuestMenu();
		return;
	}
	else if (((index >= 0) && (!m_DisplayRowLst.empty()) && (index + m_nSelStartIndex) >= (int)m_DisplayRowLst.size()) )
	{
		m_nCurrSelIndex = -1;	
		RefreshQuestMenu();
		return;
	}

	ResetMainQuestLstUI();

	if (index == -1) {
		UICheckBox *pChkBox = gGlobal.pUIManager->FindUICheckBox("QTraceCheckBox");
		if (pChkBox){
			pChkBox->SetState(true);
			gGlobal.pUIManager->AddDirtyUIItem(pChkBox);
		}

		if (m_CurrSelQuestId != 0)
			SelectQuest(m_CurrSelQuestId, m_CurrSelTypeId);
		if (m_nCurrSelIndex != -1)
			HighLightSelectRow(m_nCurrSelIndex + 1);
	}
	//	index = m_nCurrSelIndex;

	if ( index >= 0 && index < MAX_MAIN_QUESTLOG )
	{
		SelectRow(index);
		// highlight the selected check box
		HighLightSelectRow(index + 1);
	}

	ReDrawAllMenuItem();
	/*
	if ( index == -1 )
	{
		ReDrawAllMenuItem();
	}
	else if ( index >= 0 && index < MAX_MAIN_QUESTLOG )
	{
		SelectRow(index);
		// highlight the selected check box
		HighLightSelectRow(index + 1);
	}
	*/
	{
		bool bClearRhs(false);
		if ( !m_tempSwitch.empty() && m_selQuest )
		{
			QID2TypeID::iterator f = m_tempSwitch.find(m_selQuest->GetQuest_id());
			if ( f == m_tempSwitch.end() )
				bClearRhs = true;
		}

		if ( bClearDesc || bClearRhs )
		{
			UpdateSubDescUI(true);
			DisEnableQuestRemainTime();
		}
		else if ( m_nCurrSelIndex != -1 && m_CurrSelQuestId > 0 )
		{
			UpdateSubDescUI();
		}

	}
	/*
	if ( m_nCurrSelIndex != -1 && m_CurrSelQuestId > 0 )
		UpdateSubDescUI();
	else 
	{
		bool bRemoveClear(false);
		if ( !m_tempSwitch.empty() && m_selQuest )
		{
			QID2TypeID::iterator f = m_tempSwitch.find(m_selQuest->GetQuest_id());
			if ( f == m_tempSwitch.end() )
				bRemoveClear = true;
		}
		if ( bClearDesc || bRemoveClear )
		{
			UpdateSubDescUI(true);
			DisEnableQuestRemainTime();
		}
	}
	*/
	RefreshMainLstScrollV();
	RefreshQuestMenu();
}

void CMenuQuestLogProcess::UpdateTargetUI(UIChatPanel* pChat, UInt32 tid, 
										  UInt16 v1, UInt16 v2, UInt16 v3, 
										  UInt8 vtype, UInt16 num, UInt16 max,
										  const String &tname, UInt8 otype)
{
	if (pChat == NULL)
		return;

	String text = _T("\\T014");
		//gGlobal.GetStringTable()->Get(_T("MSG_SPACE"));

	if (num >= max)
		text += _T("#G");

	switch (vtype) {
	case Q_TARGET_NPC:
		{
//			if (vtype != otype) {
//				text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_TARGETNPC"));
//				pChat->AddStringW(text.c_str());
//			}
			NPCDB_Client* tempnpc = NULL;
			tempnpc = gGlobal.LoadNpcTemplate(tid);

			if( tempnpc )
			{
				String tempname;
				tempname.empty();	
				const BinaryClientMapInfo* pCliMapInfo = gGlobal.m_resource->GetCliMapData(v1);
				if ( pCliMapInfo && pCliMapInfo->m_mapname)
					tempname.Format(_T("%s"), pCliMapInfo->m_mapname);
				text.AppendFormat(_T("%s \\u\\[POSI:%d,%d,%d,%d\\]%s (%s:%i,%i)\\U"),
					GetLocalStr("MSG_QUESTLOG_TARGETNPC"),
					tempnpc->m_basicinfo->m_npc_id,
					pCliMapInfo->m_mapid, v2, v3, 
					tempnpc->m_basicinfo->m_npc_mid_name, tempname.c_str(), 
					v2, v3);
				PostfixTargetNum(text, num, 1); 
			}
		}
		break;

	case Q_TARGET_MOB:
		{
			//if (vtype != otype) {
			//	text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_TARGETMOB"));
			//	pChat->AddStringW(text.c_str());
			//}
			MobDB_Client* tempmob = NULL;
			tempmob = gGlobal.LoadMobTemplate(tid);
			if( tempmob )
			{
				text.AppendFormat(_T("%s %s"), GetLocalStr("MSG_QUESTLOG_TARGETMOB"),
					tname.c_str());
				PostfixTargetNum(text, num, max);
			}
		}
		break;

	case Q_TARGET_ITEM:
		{
			UInt8 t1 = v3 & 0xff;				// item info type 1
			UInt8 t2 = (v3 >> 8) & 0x0f;		// item info type 2
			Int count = CalcTargetItemCount(tid, t1, v1, t2, v2);
		
			//if (vtype != otype) {
			//	text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_TARGETITEM"));
			//	pChat->AddStringW(text.c_str());
			//}
			const ItemData* pItem = gGlobal.m_resource->GetItemData(tid);
			if( pItem )
			{
				StringA tcode;
				if (count >= max)
					text += _T("#G");
				text.AppendFormat(_T("%s %s"), 
					GetLocalStr("MSG_QUESTLOG_TARGETITEM"), pItem->name);
				if (t1 > 0)
				{
					tcode.Format("MSG_QUESTLOG_TARGETITEM_TYPE%02i", t1);
					text.AppendFormat(_T("%s%s%i"), GetLocalStr(tcode), 
						GetLocalStr("MSG_QUESTLOG_BIGGERTHAN"), v1);
				}
				if (t2 > 0)
				{
					tcode.Format("MSG_QUESTLOG_TARGETITEM_TYPE%02i", t2);
					text.AppendFormat(_T(" %s%s%i"), GetLocalStr(tcode),
						GetLocalStr("MSG_QUESTLOG_BIGGERTHAN"), v2);
				}
				PostfixTargetNum(text, count, max);
			}
		}
		break;

	case Q_TARGET_PARTNER:
		{
			const MobDB_Client* pMob = gGlobal.LoadMobTemplate(tid);
			if (pMob)
			{
				UInt partnercount = 0;
				CPartner* partner;
				UInt32 compareValue1;
				//					UInt32 compareValue2;
				for( Index i = C_INDEX(0); i < gGlobal.g_pPartnerGroup->GetCount(); ++i )
				{
					partner = gGlobal.g_pPartnerGroup->GetChild<CPartner>(i);
					if (partner && partner->GetMob_id() == tid)
					{
						compareValue1 = GetCompareValue(v2, partner);
						if (v2 == 0)
							++partnercount;
						else if (v2 == 1)
						{
							if (compareValue1 == v1)
								++partnercount;
						}else
						{
							if (compareValue1 > v1)
								++partnercount;
						}
						// TYPE 2 MISSING!
						//	partner->GetTalent(); (??) 
					}
				}
				//if (vtype != otype) {
				//	text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_TARGETPARTNER"));
				//	pChat->AddStringW(text.c_str());
				//}
				if (partnercount >= max)
					text += _T("#G");
				text.AppendFormat(_T("%s %s"), 
					GetLocalStr("MSG_QUESTLOG_TARGETPARTNER"), tname.c_str());
				if (v2 > 0)
				{
					StringA stringtext;
					stringtext.Format("MSG_QUESTLOG_TARGETPARTNER_TYPEA%02i", v2);
					if (v2 == 1) // special case for "element"
						text.AppendFormat(_T("%s%s%i"), GetLocalStr(stringtext),
						GetLocalStr("MSG_QUESTLOG_EQUALTO"), v3);
					else
						text.AppendFormat(_T("%s%s%i"), GetLocalStr(stringtext),
						GetLocalStr("MSG_QUESTLOG_BIGGERTHAN"), v3);
				}
				PostfixTargetNum(text, partnercount, max);
			}
		}
		break;

	case Q_TARGET_PET:
		{
		}
		break;

	case Q_TARGET_MAP:
		{
			//if (vtype != otype) {
			//	text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_TARGETMAP"));
			//	pChat->AddStringW(text.c_str());
			//}
			text.AppendFormat(_T("%s %s"), GetLocalStr("MSG_QUESTLOG_TARGETMAP"),
				tname.c_str());
			if ( (v1 != 0) || (v2 != 0) )
				text.AppendFormat(_T(" (%i,%i)"), v1, v2);
		}
		break;
	case Q_TARGET_ENCOUNT:
		{
			//if (vtype != otype) {
			//	text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_ENCOUNTNPC"));
			//	pChat->AddStringW(text.c_str());
			//}
			if (max == UINT16_MAX_VALUE)	// no display for 65535 encounter count
				return;

			NPCDB_Client* tempnpc = gGlobal.LoadNpcTemplate(tid);
			if (tempnpc)
			{
				text.AppendFormat(_T("%s %s"), GetLocalStr("MSG_QUESTLOG_ENCOUNTNPC"),
					tempnpc->m_basicinfo->m_npc_mid_name);
				if (v1 != 0) {
					const BinaryClientMapInfo* pCliMapInfo = 
						gGlobal.m_resource->GetCliMapData(v1);
					if ( pCliMapInfo && pCliMapInfo->m_mapname )
						text.AppendFormat(_T("(%s)"), pCliMapInfo->m_mapname);
				}
				PostfixTargetNum(text, num, max);
			}
		}
		break;

	case Q_TARGET_NEWBIE:
		{
			//if (vtype != otype) {
			//	text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_TARGETNEWBIE"));
			//	pChat->AddStringW(text.c_str());
			//}
			text.AppendFormat(_T("%s (%i ~ %i)"), 
				GetLocalStr("MSG_QUESTLOG_TARGETNEWBIE"), v1, v2);
			
			PostfixTargetNum(text, num, max);
		}
		break;

	case Q_TARGET_FOLLOWER:
		{
			MobDB_Client* tempmob = NULL;
			tempmob = gGlobal.LoadMobTemplate(tid);
			if( tempmob )
			{
				text.AppendFormat(_T("%s %s"), GetLocalStr("MSG_QUESTLOG_TARGETFOLLOWER"),
					tname.c_str());
				PostfixTargetNum(text, num, max);
			}
		}
		break;

	default:	// no match case
		return;
	}
	pChat->AddStringW(text.c_str());
}

void CMenuQuestLogProcess::UpdateSubDescUI(bool bClear, bool bHome)
{
	if ( m_bSetDescFlag ) return;

	UIStaticText* pTitleText = gGlobal.pUIManager->FindUIStaticText(SZ_QM_TITLE);
	UIChatPanel* pChat = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_QUESTDESC);
	if ( !pChat ) return;
	if (!pTitleText) return;

	if(IsShowQuestHelp())
	{
		UICheckBox *pChkBox = gGlobal.pUIManager->FindUICheckBox("QTraceCheckBox");
		if (pChkBox)
		{
			if (pChkBox->GetState())
			{
				pChkBox->SetState(false);
			}

			gGlobal.pUIManager->AddDirtyUIItem(pChkBox);
		}

		pChat->ClearAllRow();
		pTitleText->ClearText();
		pTitleText->Redraw();

		CSVQuestCase* pCase = NULL;
		// Show Title
		String StrName;
		if (m_CsvRowLst.find(m_CurrSelQuestId) == m_CsvRowLst.end())
		{
			return;
		}
		else
		{
			pCase = m_CsvRowLst[m_CurrSelQuestId];
			if (!pCase)return;

			if (!IsShowQuest(pCase)) return;


			StrName = pCase->strQuestName;
		}

		// - Set Item Icon
		StringA IconName("");
		StringA strItemCount("");
		for ( int i = 0; i < 6; ++i )
		{
			IconName.Format("Quest%03dIcon", i);
			strItemCount.Format("Quest%03dCount", i);
			//UIImage* pim = gGlobal.pUIManager->FindUIImage(IconName.c_str());
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(IconName.c_str());
			//if ( pim )
			if (pAniIcon)
			{
				pAniIcon->SetEnable(false);
			}

			UIStaticText* pStaticText = gGlobal.pUIManager->FindUIStaticText(strItemCount.c_str());
			if (pStaticText)
			{
				pStaticText->ClearText();
				pStaticText->Redraw();
				pStaticText->SetEnable(false);
			}

		}

		for ( int i = 0; i < (int)MIN(pCase->Itemlst.size(),pCase->ItemCountlst.size()); ++i )
		{
			IconName.Format("Quest%03dIcon", i);
			//UIImage* pim = gGlobal.pUIManager->FindUIImage(IconName.c_str());
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(IconName.c_str());
			//if ( pim )
			if (pAniIcon)
			{
				const ItemData* pItemData = gGlobal.m_resource->GetItemData(pCase->Itemlst[i]);
				if( pItemData )
				{
					String temp(pItemData->icon2);
					/*StringA imageFilename;
					imageFilename.Format("Data/%s", temp.c_str());
					pim->SetEnable(true);
					pim->SetImageFileA(imageFilename.c_str());
					pim->SetDetectRect();*/
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(temp.c_str());
					pAniIcon->SetEnable(true);
				}
			}

			if (pCase->ItemCountlst[i])
			{
				strItemCount.Format("Quest%03dCount", i);
				UIStaticText* pStaticText = gGlobal.pUIManager->FindUIStaticText(strItemCount.c_str());
				if (pStaticText)
				{
					String strCount;
					strCount.Format(_T("%d"),pCase->ItemCountlst[i]);
					pStaticText->SetText(strCount);
					pStaticText->Redraw();
					pStaticText->SetEnable(true);
				}
			}
		}

		//set text
		pTitleText->SetText(StrName.c_str());
		pTitleText->Redraw();

		String text;
		// Show Description
		text.Format(_T("#Y%s"), GetLocalStr("MSG_QUESTLOG_DESCRIPT"));
		pChat->AddStringW(text.c_str());
		text.Format(_T("%s"), pCase->strDesc.c_str());
		pChat->AddStringW(text.c_str());
		pChat->AddStringW(_T(" "));

		// Show Begin Npc
		if(pCase->nBeginNpcID)
		{
			text.Format(_T("#Y%s"), gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_BEGINNPC")));
			pChat->AddStringW(text.c_str());
			NPCDB_Client* tempnpc = gGlobal.LoadNpcTemplate(pCase->nBeginNpcID);
			if( tempnpc )
			{
				String tempname;
				const BinaryClientMapInfo* pCliMapInfo = gGlobal.m_resource->GetCliMapData(pCase->nBeginNpcMapID);
				if ( pCliMapInfo && pCliMapInfo->m_mapname )
				{
					tempname.Format(_T("%s"), pCliMapInfo->m_mapname);
					text.Format(_T("\\u\\[POSI:%d,%d,%d,%d\\]%s (%s:%i,%i)\\U"), 
						tempnpc->m_basicinfo->m_npc_id,pCliMapInfo->m_mapid, pCase->nBeginNpcX, pCase->nBeginNpcY,
						tempnpc->m_basicinfo->m_npc_mid_name, tempname.c_str(), pCase->nBeginNpcX, pCase->nBeginNpcY);
				}
				else
				{
					text = _T("???");
				}
			}
			pChat->AddStringW(text.c_str());
			pChat->AddStringW(_T("\\r"));
		}

		// FinishDesc
		if (!pCase->strFinishDesc.empty()) {
			text.Format(_T("#Y%s"), GetLocalStr("MSG_QUESTLOG_BOUNTY"));
			pChat->AddStringW(text.c_str());
			pChat->AddStringW(pCase->strFinishDesc.c_str());
		}

		// tips
		if (!pCase->strTitle.empty()) {
			pChat->AddStringW(_T(" "));
			text.Format(_T("%s%s"), GetLocalStr("MSG_QUESTLOG_TIPS"), pCase->strTitle.c_str());
			pChat->AddStringW(text.c_str());
		}
	}
	else
	{
		SetCurQuestTrace();

		if ( !m_selQuest || !m_selType || bClear )
		{
			pChat->ClearAllRow();
			pChat->Redraw();
			if (pTitleText) {
				pTitleText->SetText(_T(""));
				pTitleText->Redraw();
			}
		}
		else
		{
			pChat->ClearAllRow();
			const String& origDescript = m_selQuest->GetDescript();
			int tipsPos = origDescript.Find(_T("##"));
			String descipt = (tipsPos == -1 ? origDescript : origDescript.Left(tipsPos));
			String tips = (tipsPos == -1 ? _T("") : origDescript.Mid(tipsPos + 2));
			String text;
			String tempname;

			// Show Title
			if (pTitleText) {
				pTitleText->SetText(m_selQuest->GetName().c_str());
				pTitleText->Redraw();
			}

			// Show Description
			text.Format(_T("#Y%s"), GetLocalStr("MSG_QUESTLOG_DESCRIPT"));
			pChat->AddStringW(text.c_str());
			text.Format(_T("%s"), descipt.c_str());
			pChat->AddStringW(text.c_str());
			pChat->AddStringW(_T(" "));

			// Show Begin Npc 
			/*
			if( m_selQuest->GetBeginNpc() )
			{
				text.Format(_T("#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_BEGINNPC"));
				pChat->AddStringW(text.c_str());
				NPCDB_Client* tempnpc = gGlobal.LoadNpcTemplate(m_selQuest->GetBeginNpc());
				if( tempnpc )
				{
					tempname.empty();
					const BinaryClientMapInfo* pCliMapInfo = gGlobal.m_resource->GetCliMapData(m_selQuest->GetBeginNpc_Map());
					if ( pCliMapInfo && pCliMapInfo->m_mapname )
						tempname.Format(_T("%s"), pCliMapInfo->m_mapname);
					text.Format(_T("\\u\\[POSI:%d,%d,%d\\]%s (%s:%i,%i)\\U"), 
						pCliMapInfo->m_mapid, m_selQuest->GetBeginNpc_x(), m_selQuest->GetBeginNpc_y(),
						tempnpc->m_basicinfo->m_npc_mid_name, tempname.c_str(), m_selQuest->GetBeginNpc_x(), m_selQuest->GetBeginNpc_y());
				}
				pChat->AddStringW(text.c_str());
				pChat->AddStringW(_T("\\r"));
			}

			text.Format(_T("#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_FINISHSUITATION"));
			pChat->AddStringW(text.c_str());
			*/

			text.Format(_T("#Y%s"), GetLocalStr("MSG_QUESTLOG_TARGET"));
			pChat->AddStringW(text.c_str());
			if (m_selQuest->GetTarget1_vtype() != Q_TARGET_NONE)
				UpdateTargetUI(pChat, m_selQuest->GetTarget1_id(), m_selQuest->GetTarget1_v1(),
								m_selQuest->GetTarget1_v2(), m_selQuest->GetTarget1_v3(),
								m_selQuest->GetTarget1_vtype(), m_selQuest->GetTarget1_num(),
								m_selQuest->GetTarget1_max(), m_selQuest->GetTarget1_name(),
								0);
			if (m_selQuest->GetTarget2_vtype() != Q_TARGET_NONE)
				UpdateTargetUI(pChat, m_selQuest->GetTarget2_id(), m_selQuest->GetTarget2_v1(),
								m_selQuest->GetTarget2_v2(), m_selQuest->GetTarget2_v3(),
								m_selQuest->GetTarget2_vtype(), m_selQuest->GetTarget2_num(),
								m_selQuest->GetTarget2_max(), m_selQuest->GetTarget2_name(),
								m_selQuest->GetTarget1_vtype());
			if (m_selQuest->GetTarget3_vtype() != Q_TARGET_NONE)
				UpdateTargetUI(pChat, m_selQuest->GetTarget3_id(), m_selQuest->GetTarget3_v1(),
								m_selQuest->GetTarget3_v2(), m_selQuest->GetTarget3_v3(),
								m_selQuest->GetTarget3_vtype(), m_selQuest->GetTarget3_num(),
								m_selQuest->GetTarget3_max(), m_selQuest->GetTarget3_name(),
								m_selQuest->GetTarget2_vtype());
			if (m_selQuest->GetTarget4_vtype() != Q_TARGET_NONE)
				UpdateTargetUI(pChat, m_selQuest->GetTarget4_id(), m_selQuest->GetTarget4_v1(),
								m_selQuest->GetTarget4_v2(), m_selQuest->GetTarget4_v3(),
								m_selQuest->GetTarget4_vtype(), m_selQuest->GetTarget4_num(),
								m_selQuest->GetTarget4_max(), m_selQuest->GetTarget4_name(),
								m_selQuest->GetTarget3_vtype());
			if (m_selQuest->GetTarget5_vtype() != Q_TARGET_NONE)
				UpdateTargetUI(pChat, m_selQuest->GetTarget5_id(), m_selQuest->GetTarget5_v1(),
								m_selQuest->GetTarget5_v2(), m_selQuest->GetTarget5_v3(),
								m_selQuest->GetTarget5_vtype(), m_selQuest->GetTarget5_num(),
								m_selQuest->GetTarget5_max(), m_selQuest->GetTarget5_name(),
								m_selQuest->GetTarget4_vtype());

			if( m_selType->GetMaxRound() > 0 )
			{
				//text.Format(_T("%s%s"), GetLocalStr("MSG_SPACE"), GetLocalStr("MSG_QUESTLOG_ROUND"));
				text.Format(_T("\\T014%s"), GetLocalStr("MSG_QUESTLOG_ROUND"));
				PostfixTargetNum(text, m_selType->GetRound(), m_selType->GetMaxRound());
				pChat->AddStringW(text.c_str());
			}

			if( m_selType->GetLimitRound() > 0 )
			{
				//text.Format(_T("%s%s"), GetLocalStr("MSG_SPACE"), GetLocalStr("MSG_QUESTLOG_REMAININGROUND"));
				text.Format(_T("\\T014%s"), GetLocalStr("MSG_QUESTLOG_REMAININGROUND"));
				PostfixTargetNum(text, m_selType->GetTotalRound(), m_selType->GetLimitRound());
				pChat->AddStringW(text.c_str());
			}

			RefreshQuestTimeLimit(false);

			// Show Finish Npc 
			if( m_selQuest->GetFinishNpc() )
			{
				pChat->AddStringW(_T(" "));
				text.Format(_T("#Y%s"), GetLocalStr("MSG_QUESTLOG_FINISHNPC"));
				pChat->AddStringW(text.c_str());
				NPCDB_Client* tempnpc = gGlobal.LoadNpcTemplate(m_selQuest->GetFinishNpc());
				if( tempnpc )
				{
					tempname.empty();
					const BinaryClientMapInfo* pCliMapInfo = gGlobal.m_resource->GetCliMapData(m_selQuest->GetFinishNpc_Map());
					if ( pCliMapInfo && pCliMapInfo->m_mapname )
						tempname.Format(_T("%s"), pCliMapInfo->m_mapname);
					text.Format(_T("\\u\\[POSI:%d,%d,%d,%d\\]%s (%s:%i,%i)\\U"), 
						tempnpc->m_basicinfo->m_npc_id,pCliMapInfo->m_mapid, m_selQuest->GetFinishNpc_x(), m_selQuest->GetFinishNpc_y(),
						tempnpc->m_basicinfo->m_npc_mid_name, tempname.c_str(), m_selQuest->GetFinishNpc_x(), m_selQuest->GetFinishNpc_y());
				}
				pChat->AddStringW(text.c_str());
			}

			// tips
			if (!tips.IsEmpty()) {
				pChat->AddStringW(_T(" "));
				text.Format(_T("%s%s"), GetLocalStr("MSG_QUESTLOG_TIPS"), tips.c_str());
				pChat->AddStringW(text.c_str());
			}

		}
	}

	if ( bHome )
		pChat->HomeRow();
	RefreshSubDescScrollV();

	pChat->Redraw();
}

void CMenuQuestLogProcess::UpdateQuestLogMenu()
{
	UpdateMainQuestLstUI(-1, true);
}

void CMenuQuestLogProcess::LuaMainLstUpRow()
{
	if ( m_nSelStartIndex > 0 )
	{
		--m_nSelStartIndex;
		UpdateMainQuestLstUI(-1);
	}
}

void CMenuQuestLogProcess::LuaMainLstDownRow()
{
	Int nTotalItem = (Int)m_DisplayRowLst.size();
	if ( nTotalItem > MAX_MAIN_QUESTLOG )
	{
		if ( (nTotalItem - m_nSelStartIndex) > MAX_MAIN_QUESTLOG )
		{
			++m_nSelStartIndex;
			UpdateMainQuestLstUI(-1);
		}	
	}
}

void CMenuQuestLogProcess::LuaSubDescUpRow()
{
	UIChatPanel* pChat = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_QUESTDESC);
	if ( pChat )
	{
		pChat->UpRow();
		RefreshSubDescScrollV();
	}
}

void CMenuQuestLogProcess::LuaUpdateSubDescScrollV()
{
	/*
	UIScrollBarV* pScrollV = gGlobal.pUIManager->FindUIScrollBarV(SZ_QM_SCROLLV_QUESTDESC); 
	UIChatPanel* pChat = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_QUESTDESC);
	if ( pChat && pScrollV )
	{
		UInt32 nTotalRow = pChat->RowListCount();
		UInt32 selNum = ( nTotalRow > MAX_SUBDESC_ROW )?nTotalRow:0;
		pScrollV->SetNumCount(selNum);
		pScrollV->SetSelNum(pChat->
		gGlobal.pUIManager->AddDirtyUIItem(pScrollV);
	}
	*/
}

void CMenuQuestLogProcess::LuaSubDescDownRow()
{
	UIChatPanel* pChat = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_QUESTDESC);
	if ( pChat )
	{
		pChat->DownRow();
		RefreshSubDescScrollV();
	}
}

void CMenuQuestLogProcess::LuaSubDescScrollMove()
{
	UIChatPanel* pChat = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_QUESTDESC);
	if ( pChat )
	{
		UIScrollBarV* pScrollV = gGlobal.pUIManager->FindUIScrollBarV(SZ_QM_SCROLLV_QUESTDESC); 
		if ( pScrollV )
		{
			pChat->SetRow( pScrollV->GetSelNum(), pChat->RowListCount() );
		}
	}
}

void CMenuQuestLogProcess::LuaMainLstScrollMove()
{
	UIScrollBarV* pScrollV = gGlobal.pUIManager->FindUIScrollBarV(SZ_QM_SCROLLV_QUESTLIST); 
	if ( pScrollV )
	{
		UInt32 currSel = pScrollV->GetSelNum();
		UInt32 nTotal = (UInt32)m_DisplayRowLst.size();
		if ( currSel >= 0 && currSel < nTotal )
		{
			if ( nTotal < MAX_MAIN_QUESTLOG )
			{
				if ( m_nSelStartIndex != 0 )
				{
					m_nSelStartIndex = 0;
					UpdateMainQuestLstUI(-1);
				}	
			}
			else 
			{
				if ( (nTotal - currSel) > MAX_MAIN_QUESTLOG )
					m_nSelStartIndex = currSel;
				else
					m_nSelStartIndex = nTotal - MAX_MAIN_QUESTLOG;

				UpdateMainQuestLstUI(-1);
			}
		}
	}
}

bool CMenuQuestLogProcess::SetMenuLstInfo()
{
	if ( m_bSetMenuLstFlag ) return false;

	m_bSetMenuLstFlag = BOOLEAN_TRUE;
	// Data input
	m_tempSwitch.clear();

	QMenuRow* pCol = NULL;
	CQuestLog* pQuestLog = NULL;
	CSVQuestCase* pQuestCase = NULL;

	if (IsShowQuestHelp())
	{
		for (QCsvRowLst::iterator iter = m_CsvRowLst.begin(); iter != m_CsvRowLst.end(); ++iter)
		{
			pQuestCase = iter->second;

			if (!IsShowQuest(pQuestCase))continue;

			UInt32 mainTypdId = pQuestCase->nTypeID;  
			QMenuRowLst::iterator f = m_MenuRowLst.find(mainTypdId);
			if ( f != m_MenuRowLst.end() )
			{// find 
				pCol = f->second;
				if ( pCol )
				{
					String title(_T(""));
					if( !pQuestCase->strTypeName.empty() )
						title = pQuestCase->strTypeName;
					else
						title =gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_NONAME_QUEST"));

					QMRowChildLst::iterator f2 = pCol->itemLst.find(pQuestCase->nQuestID);
					if ( f2 == pCol->itemLst.end() )
					{
						pCol->itemLst.insert(std::make_pair(pQuestCase->nQuestID, pQuestCase->strQuestName));
					}
				}
			}
			else
			{// not find , new one
				pCol = SafeCreateObject(QMenuRow);
				if ( pCol )
				{
					if (!pQuestCase->strTypeName.empty())
						pCol->folderName = pQuestCase->strTypeName;
					else
						pCol->folderName =gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_NONAME_TYPE"));

					pCol->expended = false;

					String title(_T(""));
					if (!pQuestCase->strQuestName.empty())
						title = pQuestCase->strQuestName;
					else
						title =gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_NONAME_QUEST"));

					QMRowChildLst::iterator f2 = pCol->itemLst.find(pQuestCase->nQuestID);
					if ( f2 == pCol->itemLst.end() )
					{
						pCol->itemLst.insert(std::make_pair(pQuestCase->nQuestID, title));
						m_MenuRowLst.insert(std::make_pair(mainTypdId, pCol));
					}
					else
						SafeDeleteObject(pCol);
				}
			}	
		}
	}
	else
	{
		UInt16 logCount = gGlobal.g_QuestSystem->GetQuestCount();
		for (UInt16 i = 0; i < logCount; ++i)
		{
			pQuestLog = gGlobal.g_QuestSystem->GetEachQuest(i);
			if (pQuestLog)
			{
				CQuestType* pType = gGlobal.g_QuestSystem->GetType(pQuestLog->GetType_id());
				bool bUpdateSuccess(false);
				UInt32 mainTypdId = pQuestLog->GetType_id();  
				QMenuRowLst::iterator f = m_MenuRowLst.find(mainTypdId);
				if ( f != m_MenuRowLst.end() )
				{// find 
					pCol = f->second;
					if ( pCol )
					{
						String title(_T(""));
						if( pQuestLog->GetName().GetLength() )
							title = pQuestLog->GetName();
						else
							title =gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_NONAME_QUEST"));
						
						QMRowChildLst::iterator f2 = pCol->itemLst.find(pQuestLog->GetQuest_id());
						if ( f2 == pCol->itemLst.end() )
						{
							pCol->itemLst.insert(std::make_pair(pQuestLog->GetQuest_id(), title));
							bUpdateSuccess = true;
						}
					}
				}
				else
				{// not find , new one
					pCol = SafeCreateObject(QMenuRow);
					if ( pCol )
					{
						if (pType && !pType->GetName().IsEmpty())
							pCol->folderName = pType->GetName();
						else
							pCol->folderName =gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_NONAME_TYPE"));

						pCol->expended = false;

						String title(_T(""));
						if (!pQuestLog->GetName().IsEmpty())
							title = pQuestLog->GetName();
						else
							title =gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_NONAME_QUEST"));

						QMRowChildLst::iterator f2 = pCol->itemLst.find(pQuestLog->GetQuest_id());
						if ( f2 == pCol->itemLst.end() )
						{
							pCol->itemLst.insert(std::make_pair(pQuestLog->GetQuest_id(), title));

							m_MenuRowLst.insert(std::make_pair(mainTypdId, pCol));

							bUpdateSuccess = true;
						}
						else
							SafeDeleteObject(pCol);
					}
				}
			
				if ( bUpdateSuccess )
				{
					m_tempSwitch.insert(std::make_pair(pQuestLog->GetQuest_id(), mainTypdId));
				}
			}
		}

	}

	m_bSetMenuLstFlag = BOOLEAN_FALSE;
	return (true);
}

void CMenuQuestLogProcess::ResetQuestLstData()
{
	if ( !gGlobal.g_QuestSystem ) return;

	ReInit();

	//Clear Main Interface Menu Data
	ClearQuestMenuLstData();

	if ( SetMenuLstInfo() )
	{
		CreateDisplayLst();
		CreateNoteDisplayLst();
	}

}

void CMenuQuestLogProcess::LuaCancelQuestClient()
{
	if (IsShowQuestHelp()) return;

	if ( !gGlobal.g_QuestSystem ) return; 

	if ( m_DisplayRowLst.empty() ) return;

	if ( m_nCurrSelIndex == -1 ) return;

	Int tempIndex = m_nSelStartIndex + m_nCurrSelIndex;
	if ( tempIndex < (Int)m_DisplayRowLst.size() )
	{
		if ( m_DisplayRowLst[tempIndex] )
		{
			UInt32 delQuestId = m_DisplayRowLst[tempIndex]->questId;
			CQuestLog* ql = gGlobal.g_QuestSystem->GetQuest(delQuestId);
			if (ql && ql->IsCancel())
			{
				m_CurrSelQuestId = delQuestId;
				String delDesc;
				CQuestType* pType = gGlobal.g_QuestSystem->GetType(ql->GetType_id());
				delDesc.Format(gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_CANCEL_MSG")), 
					(pType ? pType->GetName() : _T("")), ql->GetName().c_str());
				gGlobal.g_QuestCancelBox.ShowMessageBox(0, delDesc, "QuestCancel");
			}
			else
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_CANNOT_CANCELQUEST")), CHANNELTYPE_SYSTEM);
			}
		}
	}
}

bool CMenuQuestLogProcess::SetSubDescInfo(CQuestLog* log)
{
	if (!log || m_bSetDescFlag) return false;

	m_selQuest = log;
	SetSubDescRoundInfo(log->GetType_id());
	m_bSetDescFlag = BOOLEAN_FALSE;
	return true;
}

void CMenuQuestLogProcess::SetSubDescRoundInfo(UInt32 typeId)
{
	if (gGlobal.g_QuestSystem) {
		CQuestType *pType = gGlobal.g_QuestSystem->GetType((UInt16) typeId);
		if (pType)
			m_selType = pType;
	}
}

UInt32 CMenuQuestLogProcess::GetCompareValue(UInt32 typeInfo, CItem* item)
{
	switch( typeInfo )
	{
	case 1:
//		return item->GetRare();
		return 0;
	case 2:
//		return item->GetQuality();
		return 0;
	case 3:
		return item->GetGrade();
	case 4:
		return item->GetBase_STA();
	case 5:
		return item->GetBase_SPR();
	case 6:
		return item->GetBase_STR();
	case 7:
		return item->GetBase_CON();
	case 8:
		return item->GetBase_AGI();
	case 9:
		return item->GetBase_ATK();
	case 10:
		return item->GetBase_HIT();
	case 11:
		return item->GetBase_DEF();
	case 12:
		return item->GetBase_EVA();
	case 13:
		return item->GetBase_POW();
	case 14:
		return item->GetBase_maxHP();
	case 15:
		return item->GetBase_maxSP();
	case 16:
		return item->GetBase_maxVP();
	}
	return 0;
}

UInt32 CMenuQuestLogProcess::GetCompareValue(UInt32 typeInfo, CPartner* partner)
{
	switch( typeInfo )
	{
	case 0:
		return partner->GetLevel();
	case 1:
//		return partner->GetElement();
		return 0;
	case 2:
		return partner->GetGoodness();
	case 3:
		return partner->GetHealth();
	}
	return 0;
}

CQuestLog* CMenuQuestLogProcess::FindQuestLog(UInt16 questId) const
{
	if( (questId == 0) || !gGlobal.g_QuestSystem ) return NULL;

	return gGlobal.g_QuestSystem->GetQuest(questId);
}

BOOL CMenuQuestLogProcess::IsQuestCompleted(UInt16 quest_id)
{
	return IsQuestCompleted(FindQuestLog(quest_id));
}

BOOL CMenuQuestLogProcess::IsQuestCompleted(CQuestLog* ql)
{
	return (ql && ql->IsCompleted());

	//if( ql==NULL )	return FALSE;
	//if( ql->IsFailed())
	//	return FALSE;

	//if (ql->GetTarget1_vtype() != Q_TARGET_NONE && ql->GetTarget1_num() < ql->GetTarget1_max() ||
	//	ql->GetTarget2_vtype() != Q_TARGET_NONE && ql->GetTarget2_num() < ql->GetTarget2_max() ||
	//	ql->GetTarget3_vtype() != Q_TARGET_NONE && ql->GetTarget3_num() < ql->GetTarget3_max() ||
	//	ql->GetTarget4_vtype() != Q_TARGET_NONE && ql->GetTarget4_num() < ql->GetTarget4_max() ||
	//	ql->GetTarget5_vtype() != Q_TARGET_NONE && ql->GetTarget5_num() < ql->GetTarget5_max())
	//{
	//	return FALSE;
	//}
	/*
	if( ql->GetEncountNPC() && ql->GetEncountNPC_Num() < ql->GetEncountNPC_MaxNum())
		return FALSE;

	if( ql->GetTargetItem() )
	{
		Int itemcount = CalcTargetItemCount(ql->GetTargetItem(), ql->GetTargetItem_InfoType01(),
			ql->GetTargetItem_InfoValue01(), ql->GetTargetItem_InfoType02(), ql->GetTargetItem_InfoValue02());
		
		if( itemcount < ql->GetTargetItem_MaxNum() )
			return FALSE;
	}
	if( ql->GetTargetPartner() )
	{
		UInt partnercount = 0;
		CPartner* partner;
		UInt32 compareValue1;
		//	UInt32 compareValue2;
		for( Index i = C_INDEX(0); i < gGlobal.g_pPartnerGroup->GetCount(); ++i )
		{
			partner = gGlobal.g_pPartnerGroup->GetChild<CPartner>(i);
			if( partner && partner->GetMob_id()==ql->GetTargetPartner() )
			{
				compareValue1 = GetCompareValue(ql->GetTargetPartner_InfoType01(), partner);
				if( ql->GetTargetPartner_InfoType01()==1 ) // special case "element"
				{
					if( compareValue1 == ql->GetTargetPartner_InfoValue01() )
						++partnercount;
				}else
					if( compareValue1 > ql->GetTargetPartner_InfoValue01() )
						++partnercount;

				// TYPE 2 MISSING!
				//	partner->GetTalent(); (??) 
			}
		}
		if( partnercount < ql->GetTargetPartner_MaxNum() )
			return FALSE;
	}
	if( ql->GetTargetNpc01() && ql->GetTargetNpc01_Num()==0 )
		return FALSE;
	if( ql->GetTargetNpc02() && ql->GetTargetNpc02_Num()==0 )
		return FALSE;
	if( ql->GetTargetMob01() && ql->GetTargetMob01_Num() < ql->GetTargetMob01_MaxNum() )
		return FALSE;
	if( ql->GetTargetMob02() && ql->GetTargetMob02_Num() < ql->GetTargetMob02_MaxNum() )
		return FALSE;
	if( ql->GetTargetMob03() && ql->GetTargetMob03_Num() < ql->GetTargetMob03_MaxNum() )
		return FALSE;
	if( ql->GetEncountNPC() && ql->GetEncountNPC_Num() < ql->GetEncountNPC_MaxNum() )
		return FALSE;
	*/

	return TRUE;
}

bool CMenuQuestLogProcess::IsQuestFailed(UInt32 questId)
{
	CQuestLog* pLog = FindQuestLog(questId);
	if ( pLog )
	{
		if ( pLog->IsFailed() )
			return true;
		if ( pLog->IsFailOverTime() )
		{
			if ( pLog->GetTimeLimit() == 0 )
				return true;
			else 
			{
				time_t now;
				time(&now);
				if ( (UInt32)now > pLog->GetRealTime() )
					return true;
			}
		}
	}
	return false;
}

VOID CMenuQuestLogProcess::ResetMainCheckBoxState()
{
	StringA temp("");
	UICheckBox * pChkBox = NULL;
	for ( Int i = 1; i <= MAX_MAIN_QUESTLOG; ++i )
	{
		temp.Format("%s%d", SZ_QLM_CHECKBOX_MAINQUESTLIST, i);

		pChkBox = gGlobal.pUIManager->FindUICheckBox(temp.c_str());
		if (pChkBox)
		{
			pChkBox->SetState(false);
		}
	}
}

VOID CMenuQuestLogProcess::ResetMainQuestLstUI()
{
	StringA temp("");
	UIStaticText* pText = NULL;
	UICheckBox * pChkBox = NULL;
	UIImage* pIm = NULL;
	for( Int i = 1; i <= MAX_MAIN_QUESTLOG; ++i )
	{
		temp.Format("%s%d", SZ_QLM_STATICTEXT_QUESTNAME, i);
		pText = gGlobal.pUIManager->FindUIStaticText(temp.c_str());
		if( pText )
		{
			pText->SetText(_T(""));
			pText->SetColor(TextColor[ColorIndex_White]);	
			pText->Redraw();
		}

		temp.Format("%s%d", SZ_QLM_CHECKBOX_MAINQUESTLIST, i);
		pChkBox = gGlobal.pUIManager->FindUICheckBox(temp.c_str());
		if (pChkBox)
			pChkBox->SetState(false);

		temp.Format("%s%d", SZ_QLM_IMAGE_EXPENDSTATE, i);
		pIm = gGlobal.pUIManager->FindUIImage(temp.c_str());
		if ( pIm )
		{
			pIm->SetEnable(false);
		}
	}
}

VOID CMenuQuestLogProcess::SelectMainTypeHandle(bool bExtend, int nSel, UInt questIndex)
{
	StringA temp("");
	temp.Format("%s%i", SZ_QLM_STATICTEXT_QUESTNAME, nSel);
	UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText(temp.c_str());
	if ( pText )
	{
		UIImage* pOriIcon = NULL;
		if ( bExtend )
		{
			pOriIcon = gGlobal.pUIManager->FindUIImage("QuestLogExtendIcon");
		}
		else
		{
			pOriIcon = gGlobal.pUIManager->FindUIImage("QuestLogNotExtendIcon");
		}

		if ( pOriIcon )
		{
			temp.Format("%s%i", SZ_QLM_IMAGE_EXPENDSTATE, nSel);
			UIImage* pselim = gGlobal.pUIManager->FindUIImage(temp.c_str());
			if ( pselim )
			{
				pselim->SetImage(pOriIcon);
				pselim->SetEnable(true);
			}
		}

		if ( m_DisplayRowLst[questIndex] && (m_DisplayRowLst[questIndex]->type == QUESTMENU_FOLDER) )
		{
			String szText(_T(""));
			szText.Format(_T("%s"), m_DisplayRowLst[questIndex]->title.c_str());
			pText->SetText(szText);
			pText->Redraw();
		}
	}
}

VOID CMenuQuestLogProcess::RefreshSubDescScrollV()
{
	UIScrollBarV* pScrollV = gGlobal.pUIManager->FindUIScrollBarV(SZ_QM_SCROLLV_QUESTDESC); 
	UIChatPanel* pChat = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_QUESTDESC);
	if ( pChat && pScrollV )
	{
		UInt32 nTotalRow = pChat->RowListCount();
		UInt32 selNum = ( nTotalRow > MAX_SUBDESC_ROW )?nTotalRow:0;
		pScrollV->SetNumCount(selNum);
		gGlobal.pUIManager->AddDirtyUIItem(pScrollV);
	}
}

VOID CMenuQuestLogProcess::RefreshMainLstScrollV()
{
	UIScrollBarV* pScrollV = gGlobal.pUIManager->FindUIScrollBarV(SZ_QM_SCROLLV_QUESTLIST); 
	if ( pScrollV )
	{
		UInt32 nTotalRow = (UInt32)m_DisplayRowLst.size();
		UInt32 selNum = ( nTotalRow > MAX_MAIN_QUESTLOG )?nTotalRow:0;
		pScrollV->SetNumCount(selNum);
		gGlobal.pUIManager->AddDirtyUIItem(pScrollV);
	}
}

VOID CMenuQuestLogProcess::RefreshQuestMenu()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel(SZ_QM_PANEL_NAME); 
	if ( pPanel )
		gGlobal.pUIManager->AddDirtyUIItem(pPanel);
}

bool CMenuQuestLogProcess::NeedToUpdateQuestRemainTime()
{
	UInt32 now = timeGetTime();
	if ( now - m_preUpdate > 500 )
	{
		m_preUpdate = now;
		UIChatPanel* pChatNoteDesc = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_NOTEDESC);
		if ( pChatNoteDesc && (pChatNoteDesc->RowListCount() > 0))
		{
			CalcQuestRemainTime();
			return true;
		}		

		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel(SZ_QM_PANEL_NAME); 
		if ( pPanel && pPanel->IsEnable() == true )
		{
			if ( m_CurrSelQuestId > 0 && !m_bSetDescFlag )
			{
				UIChatPanel* pChat = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_QUESTDESC);
				if ( pChat && (pChat->RowListCount() > 0) && m_selQuest && m_selQuest->GetTimeLimit() > DATE_T_20070101)
				{
					CalcQuestRemainTime();
					return true;
				}
			}


		}
	}
	return false;
}

VOID CMenuQuestLogProcess::CancelQuestHandle()
{
	if ( !m_bSetMenuLstFlag && (m_CurrSelQuestId > 0) )
	{
		UInt32 temp = m_CurrSelQuestId;

		if ( m_selQuest && (m_selQuest->GetQuest_id() == temp) )
		{
			m_CurrSelQuestId = 0;
			UpdateSubDescUI(true);
			DisEnableQuestRemainTime();
		}
		if (gGlobal.g_QuestSystem)
		{
			gGlobal.g_QuestSystem->CancelQuest(gGlobal.pPeerProxy, gGlobal.g_context, (UInt16)temp);
		}			
	}

	gGlobal.g_QuestCancelBox.HideMessageBox();
}

VOID CMenuQuestLogProcess::PostfixTargetNum(String& szText, UInt32 curr, UInt32 max)
{
	String temp = szText;
	temp.Replace(_T("\\#G"), _T(""));
	UInt32 totalLen = 0;
	{
		UIChatPanel* pChat = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_QUESTDESC);
		if ( pChat )
		{
			UInt32	PitchHeight = 0, RealFontHeight = 0;
			pChat->GetFont()->TakePitchHeight(pChat->GetFontStyle(), PitchHeight, RealFontHeight);

			for(UInt32 i = 0; i < temp.size(); ++i)
			{
				totalLen += pChat->GetFont()->GetCharWidth(temp[i]) + PitchHeight;
			}
		}
	}

	UInt32 templen = totalLen;
	if ( (templen % 14 ) > 0 )
	    templen = (templen / 14) * 2 +1;
	else
		templen = (templen / 14) * 2 ;
	if ( templen <= 30 )
	{
		templen = 30 - templen;
		size_t count = templen;
		for ( size_t j = 0; j < count; ++j )
			szText.AppendFormat(_T(" "));
		szText.AppendFormat(_T("(%i/%i)"), curr, max);
	}
	else
	{
		szText.AppendFormat(_T(" (%i/%i)"), curr, max);
	}
}

VOID CMenuQuestLogProcess::CalcQuestRemainTime()
{
	if (m_selQuest && m_selQuest->GetTimeLimit() > DATE_T_20070101)
	{
		{
			time_t now;
			time(&now);
			if( m_selQuest->GetTimeLimit() < (UInt32)now )
			{
				if ( m_bNoticeFail )
				{
					m_bNoticeFail = BOOLEAN_FALSE;
					TestNoticeFail(m_selQuest->GetQuest_id());
				}
				return;
			}
		}

		m_bNoticeFail = BOOLEAN_TRUE;

		RefreshQuestTimeLimit();
	}
	else
	{
		DisEnableQuestRemainTime();
	}

	if (IsNoteTimeCount())
	{
		OnUpdateNoteUI();
	}
}

VOID CMenuQuestLogProcess::RefreshQuestTimeLimit(bool bRedraw)
{
	if (m_selQuest && m_selQuest->GetTimeLimit() > DATE_T_20070101)
	{
		UIImage* pIm = gGlobal.pUIManager->FindUIImage("QuestMenuTimeImage");
		if ( pIm )
		{
			pIm->SetEnable(true);
			
			UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText("QuestMenuTimeTitle");
			if ( pText )
			{
				String text(_T(""));
				text.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_TIMEELAPSED")));
				pText->SetColor(TextColor[ColorIndex_Yellow]);
				pText->SetText(text.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}

			pText = gGlobal.pUIManager->FindUIStaticText("QuestMenuTimeText");
			if ( pText )
			{	
				time_t now;
				time(&now);
				if( m_selQuest->GetTimeLimit() > (UInt32)now )
				{
					String text(_T(""));
					UInt timediff = m_selQuest->GetTimeLimit() - (UInt32)now;
					text.AppendFormat(_T("%02d:%02d:%02d"), timediff/3600, timediff/60%60, timediff%60);

					if ( timediff < 180 )
						pText->SetColor(RedColor);
					else if ( pText->GetColor() != TextColor[ColorIndex_White] )
						pText->SetColor(TextColor[ColorIndex_White]);

					pText->SetText(text.c_str());
				}
				else
				{
					String text(_T("00:00:00"));
					if ( pText->GetColor() != RedColor )
						pText->SetColor(RedColor);

					pText->SetText(text.c_str());
				}
				pText->SetEnable(true);
				pText->Redraw();
			}

			if ( bRedraw )
				gGlobal.pUIManager->AddDirtyUIItem(pIm);
		}
	}
	else
	{
		DisEnableQuestRemainTime();
	}
}

VOID CMenuQuestLogProcess::ClearDisplayLstData()
{
	for ( QMDisPlayRowLst::iterator it = m_DisplayRowLst.begin(); it != m_DisplayRowLst.end(); ++it )
		SafeDeleteObject(*it);
	m_DisplayRowLst.clear();
}

VOID CMenuQuestLogProcess::CreateDisplayLst()
{
	if ( m_bSetDisplayFlag ) return;

	m_bSetDisplayFlag = BOOLEAN_TRUE;

	ClearDisplayLstData();

	QMDisPlayRow* pDisplayCol = NULL;

	for (QMenuRowLst::iterator it = m_MenuRowLst.begin(); it != m_MenuRowLst.end(); ++it )
	{
		QMenuRow* pCol = it->second;
		if ( pCol )
		{
			pDisplayCol = SafeCreateObject(QMDisPlayRow);
			if ( pDisplayCol )
			{
				pDisplayCol->title = pCol->folderName;
				pDisplayCol->type = QUESTMENU_FOLDER;
				pDisplayCol->bExpended = pCol->expended;
				pDisplayCol->typeId = it->first;

				m_DisplayRowLst.push_back(pDisplayCol);

				if ( pCol->expended )
				{// expended child item
					for ( QMRowChildLst::iterator f =  pCol->itemLst.begin(); f !=  pCol->itemLst.end(); ++f )
					{
						pDisplayCol = SafeCreateObject(QMDisPlayRow);	
						if ( pDisplayCol )
						{
							pDisplayCol->type = QUESTMENU_ITEM;
							pDisplayCol->bExpended = false;
							pDisplayCol->title = f->second;

							pDisplayCol->questId = f->first;
							pDisplayCol->typeId = it->first;

							m_DisplayRowLst.push_back(pDisplayCol);
						}
					}
				}
			}
		}
	}

	m_bSetDisplayFlag = BOOLEAN_FALSE;
}
VOID CMenuQuestLogProcess::CreateNoteDisplayLst()
{
	if (IsShowQuestHelp()) return;

	ClearNoteDisplayLstData();

	for (QMenuRowLst::iterator it = m_MenuRowLst.begin(); it != m_MenuRowLst.end(); ++it )
	{
		QMenuRow* pCol = it->second;
		if ( pCol )
		{
			for ( QMRowChildLst::iterator f =  pCol->itemLst.begin(); f !=  pCol->itemLst.end(); ++f )
			{
				AddNoteInfo(f->first, it->first);
			}
		}
	}
}

VOID CMenuQuestLogProcess::SelectRow(Int selIndex)
{
	if ( m_DisplayRowLst.empty() ) return;

	Int realIndex = selIndex + m_nSelStartIndex;
	if ( realIndex  > (Int)m_DisplayRowLst.size() ) 
	{
		m_nCurrSelIndex = -1;
		return;
	}

	m_nCurrSelIndex = selIndex;
	
	if( realIndex < (Int)m_DisplayRowLst.size() )
	{
		QMDisPlayRow* pSelectRow = m_DisplayRowLst[realIndex];
		if ( !pSelectRow ) return;

		if ( pSelectRow->type == QUESTMENU_FOLDER ) 
		{
			if ( pSelectRow->bExpended )
			{// collapse it
				pSelectRow->bExpended = false;
				QMenuRowLst::iterator it = m_MenuRowLst.find(pSelectRow->typeId);
				if ( it != m_MenuRowLst.end() )
				{
					it->second->expended = false;
				}
			}
			else
			{// expand it
				pSelectRow->bExpended = true;
				QMenuRowLst::iterator it = m_MenuRowLst.find(pSelectRow->typeId);
				if ( it != m_MenuRowLst.end() )
				{
					it->second->expended = true;
				}
			}

			CreateDisplayLst();
			CreateNoteDisplayLst();

			//ReDrawAllMenuItem();
		}
		else if ( pSelectRow->type == QUESTMENU_ITEM ) 
		{// Set Right Hand Side Quest Desc
			if ( gGlobal.g_QuestSystem )
			{
				if (IsShowQuestHelp())
				{
					m_bSetDescFlag = BOOLEAN_FALSE;
					m_CurrSelQuestId = pSelectRow->questId;
				}
				else
				{
					CQuestLog* pQuestLog = gGlobal.g_QuestSystem->GetQuest(pSelectRow->questId);
					if (pQuestLog)
					{
						if ( SetSubDescInfo(pQuestLog) )
							m_CurrSelQuestId = pSelectRow->questId;

						//ReDrawSelMenuItem(selIndex, pSelectRow);
					}
				}
			}
		}
	}
}

VOID CMenuQuestLogProcess::SelectQuest(UInt16 questId, UInt16 typeId)
{
	int i = 0;
	// expand type id rows
	for (QMDisPlayRowLst::iterator it = m_DisplayRowLst.begin(); 
		it != m_DisplayRowLst.end(); ++it, ++i)
	{
		QMDisPlayRow* pRow = *it;
		if (pRow && pRow->type == QUESTMENU_FOLDER && pRow->typeId == typeId) {
			if (!pRow->bExpended)
				SelectRow(i);
			break;
		}
	}
	// find current quest_id row index
	i = 0;
	for (QMDisPlayRowLst::iterator it = m_DisplayRowLst.begin(); 
		it != m_DisplayRowLst.end(); ++it, ++i)
	{
		QMDisPlayRow* pRow = *it;
		if (pRow && pRow->type == QUESTMENU_ITEM && pRow->questId == questId) {
			SelectRow(i);
			break;
		}
	}
}

VOID CMenuQuestLogProcess::UpdateCurrDescByType(UInt32 mainTypeId)
{
	if ( !m_DisplayRowLst.empty() && (m_selQuest && m_selQuest->GetType_id() == mainTypeId) && (m_CurrSelQuestId != 0))
	{
		SetSubDescRoundInfo(mainTypeId);

		UpdateSubDescUI();
		RefreshQuestMenu();
	}
}

VOID CMenuQuestLogProcess::DisEnableQuestRemainTime()
{
	bool bDirty(false);
	UIImage* pIm = gGlobal.pUIManager->FindUIImage("QuestMenuTimeImage");
	if ( pIm && pIm->IsEnable() ) 
	{
		bDirty = true;
		pIm->SetEnable(false);
	}
	UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText("QuestMenuTimeTitle");
	if ( pText && pText->IsEnable() )
	{
		bDirty = true;
		pText->SetEnable(false);
	}
	pText = gGlobal.pUIManager->FindUIStaticText("QuestMenuTimeText");
	if ( pText && pText->IsEnable() )
	{
		bDirty = true;
		pText->SetEnable(false);
	}

	if ( bDirty )
		gGlobal.pUIManager->AddDirtyUIItem(pIm);
}

VOID CMenuQuestLogProcess::ModifyQuest(UInt32 questId, UInt32 typeId, CQuestLog* pLog, Boolean bUpdateIcon)
{
	if (IsShowQuestHelp() || m_MenuRowLst.empty() || !pLog || (questId == 0) ) return;

	bool bMatch(false), bRedrawRhs(false);
	QID2TypeID::iterator it = m_tempSwitch.find(questId);
	if ( it != m_tempSwitch.end() )
	{// same quest id 
		if ( it->second == typeId )
		{// same type
			if ( m_selQuest && m_selQuest->GetQuest_id() == questId )
			{
				{
					String tempname = pLog->GetName();
					if ( tempname.CompareNoCase(m_selQuest->GetName().c_str()) != 0 )
					{
						bMatch = false;
						bRedrawRhs = true;
					}
				}

				if ( SetSubDescInfo(pLog) )
				{
					m_CurrSelQuestId = questId;
					UpdateMainQuestLstUI(m_nCurrSelIndex);
					if ( !bRedrawRhs )
						bMatch = true;
				}
			}
		}
	} else if (bUpdateIcon) {		// select last updated quest
		SelectQuest(questId, typeId);
	}

	if ( !bMatch )
	{
		TestReConstructAllData();
	}

	if ( bUpdateIcon )
		CallMissionButtonEffect();

	//OnUpdateNoteUI();
}

VOID CMenuQuestLogProcess::TestReConstructAllData(Boolean bClear)
{
	if ( !gGlobal.g_QuestSystem ) return;

	if (bClear)
	{
		m_bQuestHelp = BOOLEAN_FALSE;
		m_nCurrSelIndex = -1;
	}

	if ( m_MenuRowLst.empty() )
	{
		LoadHistory();
		LoadCsv();
		ResetQuestLstData();
		UpdateQuestLogMenu();
		CreateNoteDisplayLst();
		OnUpdateNoteUI();
	}
	else
	{
		// clear current selected row
		UInt32 OldSelQuestId = 0;
		if ( m_selQuest )	
			OldSelQuestId = m_selQuest->GetQuest_id();
		Int OldStartIndex = m_nSelStartIndex;
		Int OldSelIndex = m_nCurrSelIndex;
		
		StlMap<UInt32, bool> expendlst;
		expendlst.clear();

		QMenuRow* pCol = NULL;
		for ( QMenuRowLst::iterator it = m_MenuRowLst.begin(); it != m_MenuRowLst.end(); ++it )
		{
			pCol = it->second;
			if ( pCol )
				expendlst.insert(std::make_pair(it->first, pCol->expended));
		}

		ClearQuestMenuLstData();

		if ( SetMenuLstInfo() )
		{
			for ( QMenuRowLst::iterator it = m_MenuRowLst.begin(); it != m_MenuRowLst.end(); ++it )
			{
				pCol = it->second;
				if ( pCol )
				{
					StlMap<UInt32, bool>::iterator f = expendlst.find(it->first);
					if ( f != expendlst.end() )
						pCol->expended = f->second;
				}
			}

			expendlst.clear();

			CreateDisplayLst();
			CreateNoteDisplayLst();

			Int nTotal = (Int)m_DisplayRowLst.size();
			{// recalc the first start index (which ref to " m_DisplayRowLst")

				if ( OldStartIndex >= 0 && OldStartIndex < nTotal )
				{
					if ( nTotal < MAX_MAIN_QUESTLOG )
					{
						if ( OldStartIndex != 0 )
							m_nSelStartIndex = 0;
					}
					else
					{
						m_nSelStartIndex = ( (nTotal - OldStartIndex) > MAX_MAIN_QUESTLOG )?OldStartIndex:(nTotal - MAX_MAIN_QUESTLOG);
					}
				}
			}

			{// recalc the current selected row index(which ref to menu list)
				Int showNum = 0;
				Int startindex = m_nSelStartIndex;
				if ( startindex < nTotal )
				{
					Int diffNum = nTotal - startindex;
					showNum = ( diffNum > MAX_MAIN_QUESTLOG )?MAX_MAIN_QUESTLOG:diffNum;
				}

				if ( !(showNum > 0 && m_nCurrSelIndex < showNum) )
					m_nCurrSelIndex = -1;	
			}
			
			{
				QID2TypeID::iterator it2 = m_tempSwitch.find(m_CurrSelQuestId);
				if ( it2 == m_tempSwitch.end() )
					m_CurrSelQuestId = 0;
			}

			UpdateMainQuestLstUI(m_nCurrSelIndex);
		}
	}
}

VOID CMenuQuestLogProcess::TestNoticeFail(UInt32 questId)
{
	if ( m_bSetMenuLstFlag || m_bSetDisplayFlag || m_DisplayRowLst.empty() ) return;

	Int nTotal = (Int)m_DisplayRowLst.size();
	Int showNum = 0;
	Int startindex = m_nSelStartIndex;
	if ( startindex < nTotal )
	{
		Int diffNum = nTotal - startindex;
		showNum = ( diffNum > MAX_MAIN_QUESTLOG )?MAX_MAIN_QUESTLOG:diffNum;
	}

	for ( Int i = 0; i < showNum; ++i )
	{
		Int index = startindex + i;
		if ( index < nTotal )
		{
			QMDisPlayRow* pRow = m_DisplayRowLst[index] ;
			if ( pRow && (pRow->questId == questId))
			{
				ReDrawSelMenuItem(i, pRow, true); 
				break;
			}
		}
	}
}

VOID CMenuQuestLogProcess::RefreshTargetItemCount()
{
	UIPanel* pPanelNote = gGlobal.pUIManager->FindUIPanel(SZ_QN_PANEL_NAME); 
	if (pPanelNote)
	{
		OnUpdateNoteUI();
	}

	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel(SZ_QM_PANEL_NAME); 
	if ( pPanel && pPanel->IsEnable() )
	{
		if ( !(m_bSetDescFlag || m_bSetMenuLstFlag || m_bSetDisplayFlag) )
		{
			if (!m_DisplayRowLst.empty() && m_selQuest && (m_CurrSelQuestId != 0))
			{
				UpdateSubDescUI();
				DetectSelMenuItemStatus();
				RefreshQuestMenu();
			}
		}
	}
}

VOID CMenuQuestLogProcess::DetectSelMenuItemStatus()
{
	if ( !m_selQuest || m_DisplayRowLst.empty() ) return;

	Int showNum = 0;
	{
		Int nTotalItem = (Int)m_DisplayRowLst.size();
		if ( m_nSelStartIndex < nTotalItem )
		{
			Int diffNum = nTotalItem - m_nSelStartIndex;
			if ( diffNum > MAX_MAIN_QUESTLOG )
				showNum = MAX_MAIN_QUESTLOG;
			else
				showNum = diffNum;
		}
	}

	QMDisPlayRow* pDisplayRow = NULL;
	Int qindex = 0;
	for(Int i = 0; i < showNum; ++i )
	{
		qindex = m_nSelStartIndex + i;
		pDisplayRow = m_DisplayRowLst[qindex];
		if( pDisplayRow && (QUESTMENU_ITEM == pDisplayRow->type) )
		{
			if  ( (m_selQuest->GetQuest_id() == pDisplayRow->questId) )
			{
				UIChatPanel* pChat = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_QUESTDESC);
				if ( pChat && pChat->RowListCount() > 0 )
				{
					ReDrawSelMenuItem(i, pDisplayRow);
				}
				break;
			}
		}
	}
}

Int CMenuQuestLogProcess::CalcTargetItemCount(UInt32 itemId, UInt32 type01, UInt32 value01, UInt32 type02, UInt32 value02)
{
	Int totalCount = 0;
	if ( gGlobal.m_pBag )
	{
		if ( (type01==0) && (value01==0) && (type02==0) && (value02==0) )
		{
			totalCount = gGlobal.m_pBag->CalcItemTotalCount(itemId);
		}
		else
		{
			CItem *item = NULL;
			UInt32 compType1 = type01;
			UInt32 compType2 = type02;

			for ( UInt8 i = BAG_CURRENT; i < BAG_TYPE_COUNT; ++i )
			{
				if ( gGlobal.m_pBag->IsBagEnabled(i) )
				{
					for ( int j = (i * BAG_ITEM_COUNT); j < ((i + 1) * BAG_ITEM_COUNT); ++j )
					{
						item = gGlobal.m_pBag->GetItem((UInt8)j);
						if( item && item->GetItem_id() == itemId )
						{
							UInt itemValue1 = 0;
							UInt itemValue2 = 0;
							if( compType1 > 0 )
								itemValue1 = GetCompareValue(compType1, item);
							if( compType2 > 0 )
								itemValue2 = GetCompareValue(compType2, item);

							if( (compType1==0 || (compType1 > 0 && itemValue1 > value01) ) &&
								(compType2==0 || (compType2 > 0 && itemValue2 > value02) )
								)
							{
								totalCount += item->GetStack();
							}
						}
					}
				}
			}
		}
	}
	return totalCount;
}

void CMenuQuestLogProcess::CallMissionButtonEffect()
{
	bool bEffect(false);
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel(SZ_QM_PANEL_NAME);
	if ( pPanel )
	{
		if (pPanel->IsEnable() == false)
			bEffect = true;
	}
	else
	{
		bEffect = true;
	}
	if ( bEffect )
	{
		UIButton* pButton = (UIButton*)gGlobal.pUIManager->GetUIItem("DownBMission");
		if (pButton)
		{
			pButton->SetState(_ButtonNotice);
			pButton->SetLockState(true);
			//pButton->SetNormalState();
		}
	}
}

VOID CMenuQuestLogProcess::RemoveQuestHandle(UInt16 questId)
{
	if( m_selQuest && m_selQuest->GetQuest_id()==questId )
		m_selQuest = NULL;
}

void CMenuQuestLogProcess::OnUpdateNoteUI()
{
	UIItem *panel1 = gGlobal.pUIManager->GetUIItem(SZ_QN_PANEL_NAME);

	if (IsNullQuestNote())
	{
		if (panel1)
		{
			gGlobal.pUIManager->AddDirtyUIItem(panel1);
			panel1->SetEnable(false);
			gGlobal.pUIManager->DeletePanel(SZ_QN_PANEL_NAME);
		}

		return;
	}
	else
	{
		if (gGlobal.g_pBattleControl)
		if (!gGlobal.g_pBattleControl->GetLeaveBattle()) 
			return;

		if (!panel1)
		{
			XML_UI::GetInstance().AddFormFile("Data\\MenuQuestLogNote.xml", gGlobal.pUIManager);
			UIPanel *panel = gGlobal.pUIManager->FindUIPanel(SZ_QN_PANEL_NAME);
			
			if (panel)
			{
				gGlobal.pUIManager->SetPanel2Bottom(panel);
			}
		}
	}

	UIChatPanel* pChat = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_NOTEDESC);
	if (!pChat) return;

	pChat->ClearAllRow();
	CQuestLog* pQuestLog = NULL;
	CQuestType* pType = NULL;

	UInt16 nLineCount = 0;
	for (QMDisPlayRowLst::reverse_iterator iter = m_NoteDisplayRowLst.rbegin(); iter != m_NoteDisplayRowLst.rend(); ++iter)
	{
		QMDisPlayRow *pQMDisPlayRow = (*iter);

		if(!pQMDisPlayRow)
			continue;

		pType = gGlobal.g_QuestSystem->GetType(pQMDisPlayRow->typeId);
		pQuestLog = FindQuestLog(pQMDisPlayRow->questId);

		if (!pType || !pQuestLog)
		{
			continue;
		}

		if (!IsTraceSelect(pQMDisPlayRow->questId))
		{
			continue;
		}

		nLineCount += pQMDisPlayRow->nLineNum;

		if (nLineCount > 15)
		{
			break;
		}

		String text;
		text.AppendFormat(_T("\\u\\[MENU:%d,%d\\]¡¾%s¡¿%s\\U"), 
			pQMDisPlayRow->typeId, pQuestLog->GetQuest_id(), pType->GetName().c_str(), pQuestLog->GetName().c_str() );

		String strTemp;
		if( IsQuestCompleted(pQMDisPlayRow->questId) )
		{
			strTemp = _T("#G");
			strTemp += text;
			strTemp.append(gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_COMPLETED")));
			text = strTemp;
		}
		else if( IsQuestFailed(pQMDisPlayRow->questId) )
		{
			strTemp = _T("#R");
			strTemp += text;
			strTemp.append(gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_FAILED")));
			text = strTemp;
		}
		else
		{
			strTemp = _T("#Y");
			strTemp += text;
			text = strTemp;
		}


		pChat->AddStringW(text.c_str());

		// Show Finish Npc 
		if (IsQuestCompleted(pQMDisPlayRow->questId))
		{
			if( pQuestLog->GetFinishNpc() )
			{
				text.Format(_T("#W%s "), GetLocalStr("MSG_QUESTLOG_FINISHNPC"));
				NPCDB_Client* tempnpc = gGlobal.LoadNpcTemplate(pQuestLog->GetFinishNpc());
				if( tempnpc )
				{
					String tempname;
					tempname.empty();
					const BinaryClientMapInfo* pCliMapInfo = gGlobal.m_resource->GetCliMapData(pQuestLog->GetFinishNpc_Map());
 					if ( pCliMapInfo && pCliMapInfo->m_mapname )
 						tempname.Format(_T("%s"), pCliMapInfo->m_mapname);
					text.AppendFormat(_T("\\u\\[POSI:%d,%d,%d,%d\\]%s(%s)\\U"), 
						tempnpc->m_basicinfo->m_npc_id,
						pCliMapInfo->m_mapid, pQuestLog->GetFinishNpc_x(), pQuestLog->GetFinishNpc_y(),
						tempnpc->m_basicinfo->m_npc_mid_name, tempname.c_str());
				}
				pChat->AddStringW(text.c_str());
			}

		}
		else
		{
			for (UInt8 i = 1; i <= 5; ++i)
			{
				text.Empty();
				UInt32 tid = pQuestLog->GetTarget_id(i);
				UInt16 vtype = pQuestLog->GetTarget_vtype(i);

				if (Q_TARGET_NONE == vtype)
				{
					continue;
				}

				text.append(_T("\\T014"));

				UInt16 max = pQuestLog->GetTarget_max(i);
				UInt16 num = pQuestLog->GetTarget_num(i);
				UInt16 v1 = pQuestLog->GetTarget_v1(i);
				UInt16 v2 = pQuestLog->GetTarget_v2(i);
				UInt16 v3 = pQuestLog->GetTarget_v3(i);
				String tname = pQuestLog->GetTarget_name(i);


				if (num >= max)
					text.append(_T("#G"));

				switch (vtype) 
				{
					case Q_TARGET_NPC:
						{
							NPCDB_Client* tempnpc = NULL;
							tempnpc = gGlobal.LoadNpcTemplate(tid);

							if( tempnpc )
							{
								String tempname;
								tempname.empty();	
								const BinaryClientMapInfo* pCliMapInfo = gGlobal.m_resource->GetCliMapData(v1);
								if ( pCliMapInfo && pCliMapInfo->m_mapname)
									tempname.Format(_T("%s"), pCliMapInfo->m_mapname);
								text.AppendFormat(_T("%s \\u\\[POSI:%d,%d,%d,%d\\]%s(%s)\\U"),//
									GetLocalStr("MSG_QUESTLOG_TARGETNPC"),
									tempnpc->m_basicinfo->m_npc_id,
									pCliMapInfo->m_mapid, v2, v3, 
									tempnpc->m_basicinfo->m_npc_mid_name, tempname.c_str());

								text.AppendFormat(_T("(%d/%d)"), num, 1);
								//PostfixTargetNum(text, num, 1); 
							}
						}
						break;

					case Q_TARGET_MOB:
						{
							MobDB_Client* tempmob = NULL;
							tempmob = gGlobal.LoadMobTemplate(tid);
							if( tempmob )
							{
								text.AppendFormat(_T("%s %s"), GetLocalStr("MSG_QUESTLOG_TARGETMOB"),
									tname.c_str());
								text.AppendFormat(_T("(%d/%d)"), num, max);
								//PostfixTargetNum(text, num, max);
							}
						}
						break;

					case Q_TARGET_ITEM:
						{
							UInt8 t1 = v3 & 0xff;				// item info type 1
							UInt8 t2 = (v3 >> 8) & 0x0f;		// item info type 2
							Int count = CalcTargetItemCount(tid, t1, v1, t2, v2);

							//if (vtype != otype) {
							//	text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_TARGETITEM"));
							//	pChat->AddStringW(text.c_str());
							//}
							const ItemData* pItem = gGlobal.m_resource->GetItemData(tid);
							if( pItem )
							{
								StringA tcode;
								if (count >= max)
									text += _T("#G");
								text.AppendFormat(_T("%s %s"), 
									GetLocalStr("MSG_QUESTLOG_TARGETITEM"), pItem->name);
								if (t1 > 0)
								{
									tcode.Format("MSG_QUESTLOG_TARGETITEM_TYPE%02i", t1);
									text.AppendFormat(_T("%s %s%i"), GetLocalStr(tcode), 
										GetLocalStr("MSG_QUESTLOG_BIGGERTHAN"), v1);
								}
								if (t2 > 0)
								{
									tcode.Format("MSG_QUESTLOG_TARGETITEM_TYPE%02i", t2);
									text.AppendFormat(_T(" %s %s%i"), GetLocalStr(tcode),
										GetLocalStr("MSG_QUESTLOG_BIGGERTHAN"), v2);
								}
								text.AppendFormat(_T("(%d/%d)"), count, max);
								//PostfixTargetNum(text, count, max);
							}
						}
						break;

					case Q_TARGET_PARTNER:
						{
							const MobDB_Client* pMob = gGlobal.LoadMobTemplate(tid);
							if (pMob)
							{
								UInt partnercount = 0;
								CPartner* partner;
								UInt32 compareValue1;
								//					UInt32 compareValue2;
								for( Index i = C_INDEX(0); i < gGlobal.g_pPartnerGroup->GetCount(); ++i )
								{
									partner = gGlobal.g_pPartnerGroup->GetChild<CPartner>(i);
									if (partner && partner->GetMob_id() == tid)
									{
										compareValue1 =  gGlobal.g_pQuestLogProcess->GetCompareValue(v2, partner);
										if (v2 == 0)
											++partnercount;
										else if (v2 == 1)
										{
											if (compareValue1 == v1)
												++partnercount;
										}else
										{
											if (compareValue1 > v1)
												++partnercount;
										}
										// TYPE 2 MISSING!
										//	partner->GetTalent(); (??) 
									}
								}
								//if (vtype != otype) {
								//	text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_TARGETPARTNER"));
								//	pChat->AddStringW(text.c_str());
								//}
								if (partnercount >= max)
									text += _T("#G");
								text.AppendFormat(_T("%s %s"), 
									GetLocalStr("MSG_QUESTLOG_TARGETPARTNER"), tname.c_str());
								if (v2 > 0)
								{
									StringA stringtext;
									stringtext.Format("MSG_QUESTLOG_TARGETPARTNER_TYPEA%02i", v2);
									if (v2 == 1) // special case for "element"
										text.AppendFormat(_T("%s %s%i"), GetLocalStr(stringtext),
										GetLocalStr("MSG_QUESTLOG_EQUALTO"), v3);
									else
										text.AppendFormat(_T("%s %s%i"), GetLocalStr(stringtext),
										GetLocalStr("MSG_QUESTLOG_BIGGERTHAN"), v3);
								}
								text.AppendFormat(_T("(%d/%d)"), partnercount, max);
								//PostfixTargetNum(text, partnercount, max);
							}
						}
						break;

					case Q_TARGET_PET:
						{
						}
						break;

					case Q_TARGET_MAP:
						{
							//if (vtype != otype) {
							//	text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_TARGETMAP"));
							//	pChat->AddStringW(text.c_str());
							//}
							text.AppendFormat(_T("%s%s"), GetLocalStr("MSG_QUESTLOG_TARGETMAP"),
								tname.c_str());
							if ( (v1 != 0) || (v2 != 0) )
								text.AppendFormat(_T(" (%i,%i)"), v1, v2);
						}
						break;
					case Q_TARGET_ENCOUNT:
						{
							//if (vtype != otype) {
							//	text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_ENCOUNTNPC"));
							//	pChat->AddStringW(text.c_str());
							//}
							if (max == UINT16_MAX_VALUE)	// no display for 65535 encounter count
								continue;

							NPCDB_Client* tempnpc = gGlobal.LoadNpcTemplate(tid);
							if (tempnpc)
							{
								text.AppendFormat(_T("%s %s"), GetLocalStr("MSG_QUESTLOG_ENCOUNTNPC"),
									tempnpc->m_basicinfo->m_npc_mid_name);
								if (v1 != 0) {
									const BinaryClientMapInfo* pCliMapInfo = 
										gGlobal.m_resource->GetCliMapData(v1);
									if ( pCliMapInfo && pCliMapInfo->m_mapname )
										text.AppendFormat(_T("(%s)"), pCliMapInfo->m_mapname);
								}
								text.AppendFormat(_T("(%d/%d)"), num, max);
								//PostfixTargetNum(text, num, max);
							}
						}
						break;

					case Q_TARGET_NEWBIE:
						{
							//if (vtype != otype) {
							//	text.Format(_T("\\r#Y%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_TARGETNEWBIE"));
							//	pChat->AddStringW(text.c_str());
							//}
							text.AppendFormat(_T("%s(%i ~ %i)"), 
								GetLocalStr("MSG_QUESTLOG_TARGETNEWBIE"), v1, v2);

							text.AppendFormat(_T("(%d/%d)"), num, max);
							//PostfixTargetNum(text, num, max);
						}
						break;

					case Q_TARGET_FOLLOWER:
						{
							MobDB_Client* tempmob = NULL;
							tempmob = gGlobal.LoadMobTemplate(tid);
							if( tempmob )
							{
								text.AppendFormat(_T("%s %s"), GetLocalStr("MSG_QUESTLOG_TARGETFOLLOWER"),
									tname.c_str());

								text.AppendFormat(_T("(%d/%d)"), num, max);
								//PostfixTargetNum(text, num, max);
							}
						}
						break;

					default:	// no match case
						continue;
						break;
				}
				pChat->AddStringW(text.c_str());
			}

			if (pQuestLog->GetTimeLimit() > DATE_T_20070101)
			{
				time_t now;
				time(&now);
				if( pQuestLog->GetTimeLimit() > (UInt32)now )
				{
					String text(_T("  "));
					UInt timediff = pQuestLog->GetTimeLimit() - (UInt32)now;
					text.AppendFormat(_T("%02d:%02d:%02d"), timediff/3600, timediff/60%60, timediff%60);

					if ( timediff < 180 )
					{
						text = "#R" + text;
					}
					else
					{
						text = "#W" + text;
					}
					pChat->AddStringW(text.c_str());
				}
				else
				{
					String text(_T("  #R00:00:00"));
					pChat->AddStringW(text.c_str());
				}
			}

		}
	}

	
	pChat->HomeRow();

	RefreshNoteScrollV();
	RefreshQuestNote();
}

VOID CMenuQuestLogProcess::ClearNoteDisplayLstData()
{
	for ( QMDisPlayRowLst::iterator it = m_NoteDisplayRowLst.begin(); it != m_NoteDisplayRowLst.end(); ++it )
		SafeDeleteObject(*it);
	m_NoteDisplayRowLst.clear();
}

void CMenuQuestLogProcess::RefreshQuestNote()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel(SZ_QN_PANEL_NAME); 
	if ( pPanel )
		gGlobal.pUIManager->AddDirtyUIItem(pPanel);
}

void CMenuQuestLogProcess::RefreshNoteScrollV()
{
	UIScrollBarV* pScrollV = gGlobal.pUIManager->FindUIScrollBarV(SZ_QM_SCROLLV_QUESTNOTEDESC); 
	UIChatPanel* pChat = gGlobal.pUIManager->FindUIChatPanel(SZ_QM_CHATPANEL_NOTEDESC);
	if ( pChat && pScrollV )
	{
		UInt32 nTotalRow = pChat->RowListCount();
		UInt32 selNum = ( nTotalRow > MAX_SUBDESC_ROW )?nTotalRow:0;
		pScrollV->SetNumCount(selNum);
		gGlobal.pUIManager->AddDirtyUIItem(pScrollV);
	}
}

VOID CMenuQuestLogProcess::AddNoteInfo( UInt16 typeId, UInt16 questId )
{
	QMDisPlayRow *pDisplayCol = SafeCreateObject(QMDisPlayRow);	

	if (!pDisplayCol)
		return;

	pDisplayCol->questId = typeId;
	pDisplayCol->typeId = questId;

	pDisplayCol->nLineNum++; // name

	CQuestType* pType = gGlobal.g_QuestSystem->GetType(pDisplayCol->typeId);
	CQuestLog* pQuestLog = FindQuestLog(pDisplayCol->questId);

	if (!pType || !pQuestLog)
	{
		return;
	}

	// Show Finish Npc 
	if (IsQuestCompleted(pDisplayCol->questId))
	{
		if( pQuestLog->GetFinishNpc() )
		{
			pDisplayCol->nLineNum++; // name
		}
	}
	else
	{
		for (UInt8 i = 0; i < 5; ++i)
		{
			UInt16 tid = pQuestLog->GetTarget_id(i);
			UInt16 vtype = pQuestLog->GetTarget_vtype(i);

			if (tid == 0 || Q_TARGET_NONE == vtype)
			{
				continue;
			}

			pDisplayCol->nLineNum++; // name
		}
	}
	m_NoteDisplayRowLst.push_back(pDisplayCol);
}

BOOL CMenuQuestLogProcess::DelNoteInfo( UInt16 typeId, UInt16 questId )
{
	for (QMDisPlayRowLst::iterator iter = m_NoteDisplayRowLst.begin(); iter != m_NoteDisplayRowLst.end(); ++iter)
	{
		QMDisPlayRow *pQMDisPlayRow = (*iter);
		if (typeId == pQMDisPlayRow->typeId && questId == pQMDisPlayRow->questId)
		{
			SafeDeleteObject(pQMDisPlayRow);
			m_NoteDisplayRowLst.erase(iter);
			return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}

bool CMenuQuestLogProcess::IsNullQuestNote()
{
	if (m_NoteDisplayRowLst.empty()) return true;

	UInt16 nCount = 0;
	for (QTraceLst::iterator it = m_Qtracelst.begin(); it != m_Qtracelst.end(); ++it)
	{
		if (gGlobal.g_QuestSystem->HasQuest(it->first))
		{
			nCount += it->second;
		}
	}
	 
	if (!nCount) return true;

	return false;
}

BOOL CMenuQuestLogProcess::LoadCSV(LPCTSTR szPath, const BYTE* pData, UInt32 pDataLen)
{
	String srcfile(_T(""));
	srcfile.Format(_T("%s"), szPath);
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(srcfile.c_str(), true) )
		openSrcSucceeded = TRUE;
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMenuQuestLogProcess::LoadCSV: file error, %s"), srcfile.c_str());
	}


	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		ClearNoFinishCsvData();

		try{
				while( !reader.IsLineEmpty()  )
				{
					CSVQuestCase* pQuestCase = SafeCreateObject(CSVQuestCase);

					if (pQuestCase)
					{
						reader.Read(_T("TypeID"),	pQuestCase->nTypeID);
						reader.ReadString(_T("TypeName"), pQuestCase->strTypeName);
						reader.Read(_T("QuestID"), pQuestCase->nQuestID);
						reader.ReadString(_T("QuestName"), pQuestCase->strQuestName);
						reader.Read(_T("BeginNpcID"), pQuestCase->nBeginNpcID);
						reader.Read(_T("BeginNpcX"), pQuestCase->nBeginNpcX);
						reader.Read(_T("BeginNpcY"), pQuestCase->nBeginNpcY);
						reader.Read(_T("BeginNpcMapID"), pQuestCase->nBeginNpcMapID);
						reader.ReadString(_T("Desc"), pQuestCase->strDesc);
						reader.ReadString(_T("Title"), pQuestCase->strTitle);
						reader.ReadString(_T("FinishDesc"), pQuestCase->strFinishDesc);
						reader.Read(_T("Level"), pQuestCase->nLevel);
						reader.Read(_T("GetFlagQuestID"), pQuestCase->nGetFlagQuestID);
						reader.Read(_T("GetFlag"), pQuestCase->nGetFlag);
						reader.Read(_T("FinishFlagQuestID"), pQuestCase->nFinishFlagQuestID);
						reader.Read(_T("FinishFlag"), pQuestCase->nFinishFlag);
						reader.Read(_T("RequireQuestID"), pQuestCase->nRequireQuestID);
						reader.Read(_T("RequireFlag"), pQuestCase->nRequireFlag);
						reader.Read(_T("ItemNum"), pQuestCase->uiItemNum);

						for (int i = 0; i < pQuestCase->uiItemNum; ++i)
						{
							String str;
							UInt16 uiItemID = 0;
							UInt16 uiItemCount = 0;
							str.Format(_T("Item%d"), i + 1);
							reader.Read(str.c_str(), uiItemID);
							pQuestCase->Itemlst.push_back(uiItemID);
							str.Format(_T("Item%dCount"), i + 1);
							reader.Read(str.c_str(), uiItemCount);
							pQuestCase->ItemCountlst.push_back(uiItemCount);
						}

						m_CsvRowLst.insert(QCsvRowLst::value_type(pQuestCase->nQuestID, pQuestCase));
					}
					reader.MoveNext();
				}
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMenuQuestLogProcess::LoadCSV: CSV reading error due to \
											   column format or missing MoveNext() %d "), srcfile.c_str());
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}
	return TRUE;
}

void CMenuQuestLogProcess::LoadCsv()
{
	TCHAR szCsvFile[FILENAME_LENGTH];
	GetPrivateProfileString(_T("DataSourcePath"), _T("NoFinishTaskCsv"), 
		_T("\0"), szCsvFile, FILENAME_LENGTH, CONFIGURATION_INI_FILE);

	bool bOk = false;
	if(gGlobal.m_resource && (gGlobal.m_resource->GetGDFManager() || gGlobal.m_resource->GetGDFExManager()))
	{
		// CSV Default Path
		String csvfilename = szCsvFile;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( gGlobal.m_resource->GetGDFExManager() )
			fbuf = gGlobal.m_resource->GetGDFExManager()->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if ( (!fbuf) && gGlobal.m_resource->GetGDFManager() )
			fbuf = gGlobal.m_resource->GetGDFManager()->GetMapViewOfFile(csvfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			LoadCSV(szCsvFile, buf, fileSize);
			UnmapViewOfFile(fbuf);
			bOk = true;
		}
	}

	if (!bOk)
	{
		LoadCSV(szCsvFile);
	}
}

void CMenuQuestLogProcess::ClearNoFinishCsvData()
{
	for (QCsvRowLst::iterator it = m_CsvRowLst.begin(); it != m_CsvRowLst.end(); ++it)
		SafeDeleteObject((*it).second);
	m_CsvRowLst.clear();
}

void CMenuQuestLogProcess::OnSelectLogDType(UInt16 nType)
{
	switch(nType)
	{
	case QUESTLOGTYPE_NOGETTING:
		m_bQuestHelp = BOOLEAN_TRUE;
		break;

	case QUESTYPE_GETTING:
		m_bQuestHelp = BOOLEAN_FALSE;
		break;

	default:
		return;
		break;
	}

	if (nType != m_nLastType)
	{
		ResetQuestLstData();
		m_nLastType = nType;
	}

	UpdateMainQuestLstUI(-1);
}

VOID CMenuQuestLogProcess::SetCurQuest(UInt16 questId, UInt16 typeId)
{
	m_CurrSelQuestId = questId; m_CurrSelTypeId = typeId;

	SetQuestTrace(m_CurrSelQuestId, true);

	if (m_NoteDisplayRowLst.size() < 2)
		return;

	QMDisPlayRowLst::iterator it = m_NoteDisplayRowLst.begin();
	for (; it != m_NoteDisplayRowLst.end(); ++it)
	{
		QMDisPlayRow* pDislay = *it;
		if (!pDislay)
			continue;

		if (pDislay->questId == questId)
		{
			if (pDislay != m_NoteDisplayRowLst.back())
			{
				std::swap(*it, m_NoteDisplayRowLst.back());
			}

			break;
		}
	}
}

void CMenuQuestLogProcess::LoadHistory()
{
	String temp;

	TCHAR szBuffer[_MAX_PATH] = {0};
	//::GetCurrentDirectory(_MAX_PATH, szBuffer);
	::GetModuleFileName(::GetModuleHandle(NULL), szBuffer, _MAX_PATH);
	*(::_tcsrchr(szBuffer, _T('\\'))) =_T('\0');

	temp.Format(_T("%s\\QuestHistory"), szBuffer);

	DWORD attr = ::GetFileAttributes(temp.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES)
		::CreateDirectory(temp.c_str(), NULL);

	FILE* pFile = NULL;

	m_OpenFileName.Format(_T("%s\\%s%s.his"),
		temp.c_str(), 
		gGlobal.g_pAccountControl->m_pAccount->GetAccName().c_str(), 
		gGlobal.g_pCharacter->GetCharName().c_str());

	pFile = _tfopen(m_OpenFileName.c_str(), _T("rb"));
	ULONGLONG size = 0;
	if (!pFile)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMenuQuestLogProcess::LoadHistory:  %d "), m_OpenFileName.c_str());
		return;
	}

	fseek (pFile, 0, SEEK_END);
	size =	ftell (pFile);
	fseek (pFile, 0, SEEK_SET);
	UInt16 * pQuestIDs = SafeAllocate(UInt16, (Reuben::System::Size)size);

	ZeroMemory(pQuestIDs, (size_t)(size) * sizeof(UInt16));
	fread(pQuestIDs, (size_t)size, 1,  pFile);

	m_Qtracelst.clear();
	for (UInt8 i = 0; i < size/sizeof(UInt16); ++i)
	{
		m_Qtracelst[pQuestIDs[i]] = true;
	}

	SafeDeallocate(pQuestIDs);

	fclose(pFile);

}

void CMenuQuestLogProcess::SaveHistory()
{
	FILE* pFile = NULL;

	pFile = _tfopen(m_OpenFileName.c_str(), _T("wb"));

	if (!pFile)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMenuQuestLogProcess::SaveHistory:  %d "), m_OpenFileName.c_str());
		return;
	}

	for (QTraceLst::iterator it = m_Qtracelst.begin(); it != m_Qtracelst.end(); ++it)
	{
		if (it->second)
		{
			fwrite((char *)&(it->first), sizeof(UInt16), 1,  pFile);
		}
	}

	fclose(pFile);
}

void CMenuQuestLogProcess::OnSelectQuestTrace(bool bSel)
{
	if (IsShowQuestHelp()) return;

	if (m_CurrSelQuestId <= 0)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CMenuQuestLogProcess::OnSelectQuetTrace: m_CurrSelQuestId  error, %d"), m_CurrSelQuestId);
		return;
	}

	m_Qtracelst[m_CurrSelQuestId] = bSel;

	OnUpdateNoteUI();
}

bool CMenuQuestLogProcess::IsTraceSelect(UInt16 questId)
{
	QTraceLst::iterator itFind = m_Qtracelst.find(questId);
	if (itFind == m_Qtracelst.end())return false;

	return m_Qtracelst[questId];
}

void CMenuQuestLogProcess::SetCurQuestTrace()
{
	UICheckBox *pChkBox = gGlobal.pUIManager->FindUICheckBox("QTraceCheckBox");

	if (!pChkBox) return;

	QTraceLst::iterator itFind = m_Qtracelst.find(m_CurrSelQuestId);
	if (itFind == m_Qtracelst.end())
	{
		pChkBox->SetState(false);
	}
	else
	{
		pChkBox->SetState(itFind->second);
	}

	gGlobal.pUIManager->AddDirtyUIItem(pChkBox);
}

void CMenuQuestLogProcess::SetQuestTrace( UInt16 questId, bool bSel )
{
	if (IsShowQuestHelp()) return;

	if (!bSel)
	{
		QTraceLst::iterator it = m_Qtracelst.find(questId);
		if (it == m_Qtracelst.end())return;
	}
	m_Qtracelst[questId] = bSel;
}

void CMenuQuestLogProcess::OnNoteToMenu( UInt16 nTypeID, UInt16 nQuestID )
{
	UIItem *panel1 = gGlobal.pUIManager->GetUIItem(SZ_QM_PANEL_NAME);
	if (!panel1) 
		XML_UI::GetInstance().AddFormFile("Data\\MenuQuestLog.xml", gGlobal.pUIManager);

	UInt16 nIndex = 0;
	m_CurrSelTypeId = nTypeID;
	m_CurrSelQuestId = nQuestID;
	m_nCurrSelIndex = -1;
	UpdateMainQuestLstUI(-1);
}

bool CMenuQuestLogProcess::IsShowQuest( CSVQuestCase* pQuestCase)
{
	return false; // turnoff
	if (!pQuestCase)return false;

	UInt16 nLevel = 0;
	CCharacter * pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar)
	{
		nLevel = pchar->GetLevel();
	}
	else
	{
		return false;
	}


	if (pQuestCase->nLevel > nLevel)return false;

	return true;
}

BOOL CMenuQuestLogProcess::IsNoteTimeCount() const
{
	CQuestLog* pQuestLog = NULL;
	for (QMDisPlayRowLst::const_reverse_iterator iter = m_NoteDisplayRowLst.rbegin(); iter != m_NoteDisplayRowLst.rend(); ++iter)
	{
		QMDisPlayRow *pQMDisPlayRow = (*iter);

		if(!pQMDisPlayRow)
			continue;

		pQuestLog = FindQuestLog(pQMDisPlayRow->questId);
		if (pQuestLog)
		{
			if (pQuestLog->GetTimeLimit() > DATE_T_20070101)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

Reuben::System::UInt32 CMenuQuestLogProcess::GetItemId( const UInt8 SlotIndex )
{
	CSVQuestCase* pCase = NULL;
	if (m_CsvRowLst.find(m_CurrSelQuestId) == m_CsvRowLst.end())
	{
		return 0;
	}
	else
	{
		pCase = m_CsvRowLst[m_CurrSelQuestId];
		if (!pCase)return 0;
	}
	Int size = (Int)pCase->Itemlst.size();
	if ( SlotIndex < size )
		return pCase->Itemlst[SlotIndex];
	return 0; // failed
}