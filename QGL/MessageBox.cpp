//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MessageBox.h"
//-- Library
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "XMLPanel.h"

void CMessageBox::FreeString()
{
	m_PanelRealName.Empty();
	m_FilePathRealName.Empty();
	m_ChatPanelRealName.Empty();
	m_YesButtonRealName.Empty();
	m_NoButtonRealName.Empty();
	m_ConfirmButtonRealName.Empty();
	m_CancelButtonRealName.Empty();
	m_AgreeButtonRealName.Empty();
	m_RefuseButtonRealName.Empty();
}

void CMessageBox::CreateMessageBox(StringA Prefix)
{
	m_PanelRealName.Format("%sBoxPanel", Prefix.c_str());
	m_ChatPanelRealName.Format("%sChat", Prefix.c_str());
	m_YesButtonRealName.Format("%sYes", Prefix.c_str());
	m_NoButtonRealName.Format("%sNo", Prefix.c_str());
	m_ConfirmButtonRealName.Format("%sConfirm", Prefix.c_str());
	m_CancelButtonRealName.Format("%sCancel", Prefix.c_str());
	m_AgreeButtonRealName.Format("%sAgree", Prefix.c_str());
	m_RefuseButtonRealName.Format("%sRefuse", Prefix.c_str());
	m_FilePathRealName.Format("Data\\%sBox.xml", Prefix.c_str());
	
	UIPanelEx* panel = (UIPanelEx*)gGlobal.pUIManager->FindUIPanelEx(m_PanelRealName.c_str());
	if (panel == NULL)
	{
		XML_UI::GetInstance().AddFormFile(m_FilePathRealName.c_str(), gGlobal.pUIManager);
	}
}

void CMessageBox::HideMessageBox()
{
	UIPanelEx* panel = (UIPanelEx*)gGlobal.pUIManager->FindUIPanelEx(m_PanelRealName.c_str());
	if (panel)
		panel->SetEnable(false);
}

void CMessageBox::ShowMessageBox(int state, String msg, StringA Prefix)
{

	UIChatPanel* pChatPanel = (UIChatPanel*)gGlobal.pUIManager->FindUIChatPanel(m_ChatPanelRealName.c_str());
	UIPanelEx* panel = (UIPanelEx*)gGlobal.pUIManager->FindUIPanelEx(m_PanelRealName.c_str());

	if (pChatPanel == NULL || panel == NULL)
	{
		CreateMessageBox(Prefix);
		pChatPanel = (UIChatPanel*)gGlobal.pUIManager->FindUIChatPanel(m_ChatPanelRealName.c_str());
		panel = (UIPanelEx*)gGlobal.pUIManager->FindUIPanelEx(m_PanelRealName.c_str());
	}
	if (pChatPanel && panel)
	{
		panel->SetEnable(true);
		pChatPanel->ClearAllRow();
		pChatPanel->AddStringW(msg.c_str());
		pChatPanel->EndRow();
		//pChatPanel->SetPos(pChatPanel->GetX(), panel->GetY()+30);
		pChatPanel->Redraw();
		panel->ChangeCount(pChatPanel->RowListCount() + 1);

		panel->SetTextureWH(panel->GetWidth(), panel->GetHeight());
		panel->SetDragRect(0, 0, panel->GetWidth(), panel->GetHeight());
	

		UIButton* yes = (UIButton*)gGlobal.pUIManager->FindUIButton(m_YesButtonRealName.c_str());
		UIButton* no = (UIButton*)gGlobal.pUIManager->FindUIButton(m_NoButtonRealName.c_str());
		UIButton* confirm = (UIButton*)gGlobal.pUIManager->FindUIButton(m_ConfirmButtonRealName.c_str());
		UIButton* cancel = (UIButton*)gGlobal.pUIManager->FindUIButton(m_CancelButtonRealName.c_str());
		UIButton* agree = (UIButton*)gGlobal.pUIManager->FindUIButton(m_AgreeButtonRealName.c_str());
		UIButton* refuse = (UIButton*)gGlobal.pUIManager->FindUIButton(m_RefuseButtonRealName.c_str());

		if (yes && no && confirm)
		{
			UInt32 topY = panel->GetY();
			UInt32 lineCount =  pChatPanel->RowListCount() + 1;
			RECT lineRec = *panel->GetLineRect();
			int height = (lineRec.bottom - lineRec.top) * lineCount + 6;
			

			//yes->SetPos(yes->GetX(), (topY + height));
			
			for (Int i = _ButtonNormal; i < _MaxButton_State; i++)
				yes->SetSubOffSet(i, yes->GetOffsetX(), (height));
			yes->SetOffsetXY(yes->GetOffsetX(), (height));
			yes->SetOffsetPos(panel->GetX(), panel->GetY());
			
			//no->SetPos(no->GetX(), (topY + height ) );
			
			for (Int i = _ButtonNormal; i < _MaxButton_State; i++)
				no->SetSubOffSet(i, no->GetOffsetX(), (height));
			no->SetOffsetXY(no->GetOffsetX(), (height));
			no->SetOffsetPos(panel->GetX(), panel->GetY());

			//confirm->SetOffsetXY(confirm->GetOffsetX(), (height));
			for (Int i = _ButtonNormal; i < _MaxButton_State; i++)
				confirm->SetSubOffSet(i, confirm->GetOffsetX(), (height));
			confirm->SetOffsetXY(confirm->GetOffsetX(), (height));
			confirm->SetOffsetPos(panel->GetX(), panel->GetY());

			for (Int i = _ButtonNormal; i < _MaxButton_State; i++)
				cancel->SetSubOffSet(i, cancel->GetOffsetX(), (height));
			cancel->SetOffsetXY(cancel->GetOffsetX(), (height));
			cancel->SetOffsetPos(panel->GetX(), panel->GetY());

			if (agree)
			{
				for (Int i = _ButtonNormal; i < _MaxButton_State; i++)
					agree->SetSubOffSet(i, agree->GetOffsetX(), (height));
				agree->SetOffsetXY(agree->GetOffsetX(), (height));
				agree->SetOffsetPos(panel->GetX(), panel->GetY());
			}

			if (refuse)
			{
				for (Int i = _ButtonNormal; i < _MaxButton_State; i++)
					refuse->SetSubOffSet(i, refuse->GetOffsetX(), (height));
				refuse->SetOffsetXY(refuse->GetOffsetX(), (height));
				refuse->SetOffsetPos(panel->GetX(), panel->GetY());
			}

			if (state == 1)
			{
				yes->SetEnable(false);
				no->SetEnable(false);
				confirm->SetEnable(true);
				cancel->SetEnable(false);

				if (agree)
					agree->SetEnable(false);

				if (refuse)
					refuse->SetEnable(false);
			}
			else if (state == 0)
			{
		
				yes->SetEnable(true);
				no->SetEnable(true);
				confirm->SetEnable(false);
				cancel->SetEnable(false);

				if (agree)
					agree->SetEnable(false);

				if (refuse)
					refuse->SetEnable(false);

			}
			else if (state == 2)
			{
				yes->SetEnable(false);
				no->SetEnable(false);
				confirm->SetEnable(false);
				cancel->SetEnable(true);
			}
			else if (state == 3)
			{
				yes->SetEnable(false);
				no->SetEnable(false);
				confirm->SetEnable(false);
				cancel->SetEnable(false);
				if (agree)
					agree->SetEnable(true);

				if (refuse)
					refuse->SetEnable(true);
			}
			else if (state == 4)
			{
				yes->SetEnable(false);
				no->SetEnable(false);
				confirm->SetEnable(false);
				cancel->SetEnable(true);
				if (agree)
					agree->SetEnable(true);

				if (refuse)
					refuse->SetEnable(true);
			}
		}
		gGlobal.pUIManager->SetPanelEx2Top(panel);
		gGlobal.pUIManager->RenderAll();
	}
}

void CMessageBox::DeleteMessageBox()
{
	if (gGlobal.pUIManager == NULL)
		return;

	UIPanelEx* panel = (UIPanelEx*)gGlobal.pUIManager->FindUIPanelEx(m_PanelRealName.c_str());
	if (panel)
		gGlobal.pUIManager->DeletePanel(m_PanelRealName.c_str());
}

void CMessageBox::AddLuaFunc(int button, int func_code, StringA pFuncStr)
{
	UIButton* pButton = NULL;

	switch(button)
	{
	case BUTTON_YES:
		pButton = (UIButton*)gGlobal.pUIManager->FindUIButton(m_YesButtonRealName.c_str());
		break;
	case BUTTON_NO:
		pButton = (UIButton*)gGlobal.pUIManager->FindUIButton(m_NoButtonRealName.c_str());
		break;
	case BUTTON_CONFIRM:
		pButton = (UIButton*)gGlobal.pUIManager->FindUIButton(m_ConfirmButtonRealName.c_str());
		break;
	case BUTTON_CANCEL:
		pButton = (UIButton*)gGlobal.pUIManager->FindUIButton(m_CancelButtonRealName.c_str());
		break;
	}

	if (pButton)
	{
		pButton->SetLuaFunc(func_code, pFuncStr.c_str());
	}
}

void CMessageBox::SetPos(UInt32 x, UInt32 y)
{
	UIPanelEx* panel = (UIPanelEx*)gGlobal.pUIManager->FindUIPanelEx(m_PanelRealName.c_str());
	if (panel)
		panel->SetPos(x, y);
}

void CMessageBox::SetMono()
{
	UIPanelEx* panel = (UIPanelEx*)gGlobal.pUIManager->FindUIPanelEx(m_PanelRealName.c_str());
	if (panel)
	{
		gGlobal.pUIManager->ClearMonoPanel();
		gGlobal.pUIManager->SetMonoPanelEx(panel);
	}	
}

void CMessageBox::ReleaseMono()
{
	gGlobal.pUIManager->ClearMonoPanel();
}

void CMessageBox::ChangeMonoFocus(StringA panelName)
{
	ReleaseMono();
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem(panelName.c_str());
	if (panel)
		gGlobal.pUIManager->SetMonoPanel(panel);
}

/*
void CMessageBox::SetCanDrag(int code)
{
	UIPanelEx* panel = (UIPanelEx*)gGlobal.pUIManager->FindUIPanelEx(m_PanelRealName.c_str());
	if (panel)
	{
		switch(code)
		{
		case 0:
		case 1:
		case 2:
		case 12:
			panel->SetDragRect(0,0, panel->GetWidth(), panel->GetHeight());
			panel->SetCanDrag(code);
			break;
		}
	}
}

*/

CMessageBox::~CMessageBox()
{
	DeleteMessageBox();

	
}
