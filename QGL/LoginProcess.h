#pragma once
#ifndef _LOGIN_H_
#define _LOGIN_H_

class CAnimatedEntity;

class CLoginProcess
{

private:
	Int32			m_nLineIndex;
	Int32			m_nCharacterIndex;
public:
	void EnterGame();
	void ShowLogin();
	void ShowCharacterInLogin();
	void ShowChooseLine();
	void SetCharacterIndex(Int32 charIndex);
	INLINE void SetLineIndex(Int32 lineIndex){ m_nLineIndex = lineIndex;}
	INLINE Int32 GetCharacterIndex(){ return m_nCharacterIndex;}
	INLINE Int32 GetLineIndex(){ return m_nLineIndex;}
	void ResetCharacterInLogin();
	void SetAniState(int index, int state); //0:idle state, 1: battle state

	void UpdateChangeLine();
	void ShowRemoveCharacterName();

	CLoginProcess();
	virtual ~CLoginProcess(){}


	
};



#endif 