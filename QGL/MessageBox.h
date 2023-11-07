#pragma once
#ifndef _MESSAGEBOX_H_
#define _MESSAGEBOX_H_


enum BUTTON
{
	BUTTON_YES = 0,
	BUTTON_NO,
	BUTTON_CONFIRM,
	BUTTON_CANCEL,
};


class CMessageBox
{
public:
	CMessageBox(){};
	virtual ~CMessageBox();
	void ShowMessageBox(int state,  String msg, StringA Prefix); //2: only have cancel button, 1: only have confirm button, 0: only have yes and no button
	void HideMessageBox();
	void DeleteMessageBox();
	void CreateMessageBox(StringA Prefix);
	void AddLuaFunc(int button, int func_code, StringA pFuncStr); //func_code please see the UI.h UI_LUA_FUNC_CODE
	void SetPos(UInt32 x, UInt32 y);
	void SetMono();
	void ReleaseMono();
	void ChangeMonoFocus(StringA panelName);
	void FreeString();
	//void SetCanDrag(int code);

protected:
	
	StringA m_PanelRealName;
	StringA m_FilePathRealName;
	StringA m_ChatPanelRealName;
	StringA m_YesButtonRealName;
	StringA m_NoButtonRealName;
	StringA m_ConfirmButtonRealName;
	StringA m_CancelButtonRealName;
	StringA m_AgreeButtonRealName;
	StringA m_RefuseButtonRealName;

};


#endif 