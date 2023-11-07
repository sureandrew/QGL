//-- Common
#include "Common.h"
#include "QGL.h"
#include "MenuBuffIconProcess.h"
#include "Global.h"
#include "UI.h"
#include "CharacterControl.h"
#include "Resource/ResBuff.h"

#define MAX_BUFF_SHOW	9
PSTRINGA BuffIconName = "BuffIcon";
PSTRINGA BuffFlashIconName = "BuffFlashIcon";

CMenuBuffIconProcess::CMenuBuffIconProcess()
{
	m_pDetailMenuBuff = NULL;
}
CMenuBuffIconProcess::~CMenuBuffIconProcess()
{
}
void CMenuBuffIconProcess::HideAllBuff()
{
	for (int i = 1; i <= MAX_BUFF_SHOW; i++)
	{
		StringA name;
		name.Format("%s%d",BuffIconName, i);
		UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
		if (pAniIcon)
			pAniIcon->SetEnable(FALSE);

		name.Format("%s%d",BuffFlashIconName, i);
		pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
		if (pAniIcon)
			pAniIcon->SetEnable(FALSE);
	}
}
void CMenuBuffIconProcess::ShowBuffIcon()
{
	HideAllBuff();
	HideBuffDetail();

	int i = 1; 
	if (gGlobal.g_pCharacter->m_BuffMap.size() > 0)
	{
		for (BuffMap::iterator Itr = gGlobal.g_pCharacter->m_BuffMap.begin();
			Itr != gGlobal.g_pCharacter->m_BuffMap.end(); ++Itr)
		{
			StringA name;
			name.Format("%s%d",BuffIconName, i);
			UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
			if (pAniIcon && Itr->second.pBuffData->Buff_IconFile != _T("0"))
			{
				const BuffData * pBuff = Itr->second.pBuffData;
				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(Itr->second.pBuffData->Buff_IconFile, EQUIP_OTHERS);
				pAniIcon->SetEnable(TRUE);
				if (pAniIcon->GetAniEnt()->HasAniFile() && ++i > 25)
					break;
			}
		}
	}

	if ( i <= 25)
	{
		for (BuffMap::iterator Itr = gGlobal.g_pCharacter->m_BuffCacheMap.begin();
			Itr != gGlobal.g_pCharacter->m_BuffCacheMap.end(); ++Itr)
		{


			StringA name;
			name.Format("%s%d",BuffIconName, i);
			UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
			if (pAniIcon && Itr->second.pBuffData->Buff_IconFile != _T("0"))
			{
				const BuffData * pBuff = Itr->second.pBuffData;
				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(Itr->second.pBuffData->Buff_IconFile, EQUIP_OTHERS);
				pAniIcon->SetEnable(TRUE);
				if (pAniIcon->GetAniEnt()->HasAniFile() && ++i > 25)
					break;
			}

		}
	}
	if ( i > 1)
	{
		UIPanel * pPanel = gGlobal.pUIManager->FindUIPanel("BuffIconPanel");
		if (pPanel)
			pPanel->SetEnable(TRUE);
	}
	else
	{
		UIPanel * pPanel = gGlobal.pUIManager->FindUIPanel("BuffIconPanel");
		if (pPanel)
			pPanel->SetEnable(FALSE);
	}
}

void CMenuBuffIconProcess::HideBuffDetail()
{
	m_pDetailMenuBuff = NULL;
	UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("BuffDetailMenuPanel");
	if (pPanelEx)
		pPanelEx->SetEnable(FALSE);
}
void CMenuBuffIconProcess::OpenBuffDetail(int index)
{
	int i = 1;
	for (UInt k = 0; k < 2; k ++)
	{
		BuffMap tempMap;
		if ( k  == 0)
			tempMap = gGlobal.g_pCharacter->m_BuffMap;
		else
			tempMap = gGlobal.g_pCharacter->m_BuffCacheMap;

		for (BuffMap::iterator Itr = tempMap.begin();
			Itr != tempMap.end(); ++Itr)
		{

			//try add ani icon
			UIAniIcon AniIcon;
			AniIcon.ClearAniData();
			AniIcon.SetAnimationData(Itr->second.pBuffData->Buff_IconFile, EQUIP_OTHERS);
			AniIcon.SetEnable(TRUE);
			if (!AniIcon.GetAniEnt()->HasAniFile())
			{
				continue;
			}

				
			if (i == index)
			{

				UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("BuffDetailMenuPanel");
				UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("BuffDetailChat");
				if (pPanelEx && pChat)
				{
					Int MouseX = gGlobal.pUIManager->GetCursorX();
					Int MouseY = gGlobal.pUIManager->GetCursorY();
					pPanelEx->SetPos(MouseX + 20, MouseY);

					pChat->ClearAllRow();

					String name;
					name.Format(_T("#W%s"), Itr->second.pBuffData->name);
					pChat->AddStringW(name.c_str());

					String desc;
					desc.Format(_T("#Y%s"), Itr->second.pBuffData->buff_description);
					pChat->AddStringW(desc.c_str());
					String remain;

					pChat->SetFontColor(TextColor[ColorIndex_Yellow]);
					UInt32 collDown = Itr->second.CoolDown;
					if (Itr->second.pBuffData->Buff_endtype == BUFF_END_TYPE_TIME_OUT_END)
					{
						if (collDown >= 60)
						{
							UInt32 min = (UInt32)ceil((double)Itr->second.CoolDown / 60);
							String time;
							time.Format(_T("%d %s"), min,gGlobal.GetStringTable()->Get(_T("MSG_MINUTE")));
							remain.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_DETAIL_REMAIN_TIME")), time.c_str());
							pChat->AddStringW(remain.c_str());

						}
						else
						{
							UInt32 second = Itr->second.CoolDown;
							String time;
							time.Format(_T("%d %s"), second,gGlobal.GetStringTable()->Get(_T("MSG_SECOND")));
							remain.Format(gGlobal.GetStringTable()->Get(_T("MSG_BUFF_DETAIL_REMAIN_TIME")), time.c_str());
							pChat->AddStringW(remain.c_str());
							StringA name;
							name.Format("%s%d",BuffFlashIconName, i);
							UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
							if (pAniIcon)
								pAniIcon->SetEnable(TRUE);
						}
					}

					if (wcsncmp(Itr->second.pBuffData->Buff_Value1String, _T("0"), 1) != 0)
					{
						String text;
						text.Format(Itr->second.pBuffData->Buff_Value1String, Itr->second.GetBuffValue1());
						pChat->AddStringW(text.c_str());
					}
					if (wcsncmp(Itr->second.pBuffData->Buff_Value2String, _T("0"), 1) != 0)
					{
						String text;
						text.Format(Itr->second.pBuffData->Buff_Value2String, Itr->second.GetBuffValue2());
						pChat->AddStringW(text.c_str());
					}
					if (wcsncmp(Itr->second.pBuffData->Buff_Value3String, _T("0"), 1) != 0)
					{
						String text;
						text.Format(Itr->second.pBuffData->Buff_Value3String, Itr->second.GetBuffValue3());
						pChat->AddStringW(text.c_str());
					}
					pChat->EndRow();
					UInt32 LineCount = (UInt32)ceil((Real32)(pChat->GetTotalHeight())/ 
						(pPanelEx->GetLineRect()->bottom - pPanelEx->GetLineRect()->top));
					pPanelEx->ChangeCount(LineCount);
					gGlobal.pUIManager->SetPanelEx2Top(pPanelEx);
					pPanelEx->SetEnable(TRUE);
					m_pDetailMenuBuff = Itr->second.pBuffData;
				}

				k = 2;
				break;
			}

			i ++;
			
		}
	}
}