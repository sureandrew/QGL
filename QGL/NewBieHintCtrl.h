#pragma once
#ifndef _NEWBIEHINTCTRL_H_
#define _NEWBIEHINTCTRL_H_

struct SNewBieCtrlSetting
{
	UInt32		CharId_1;
	Boolean		bNewBie_1;
	UInt32		CharId_2;
	Boolean		bNewBie_2;
	UInt32		CharId_3;
	Boolean		bNewBie_3;
	/*
	SNewBieCtrlSetting()
	{
		ZeroMemory(this, sizeof(SNewBieCtrlSetting));
	};
	
	~SNewBieCtrlSetting()
	{
	};
	*/
};

class CNewBieHintCtrl
{
public:
	CNewBieHintCtrl(void);
	virtual ~CNewBieHintCtrl(void);

	VOID ReadSetting(UInt32 accountID);
	VOID WriteSetting(UInt32 accountID);

	Boolean IsNewBie(Int SelCharNum);
	Boolean IsNewBieByChar_id(UInt32 char_id);
	VOID SetNewBie(Int SelCharNum);
	VOID SetBie(UInt32 char_id, Boolean isNew);
	Boolean IsExistBie(UInt32 char_id);
	VOID ResetBie();
	VOID ClearNewBie(UInt32 char_id);
	Boolean CleanupNonExistBie(UInt32 char_id1, UInt32 char_id2, UInt32 char_id3);

private:
	//struct SNewBieCtrlSetting;
	Boolean				m_bWriteFlag;	
	SNewBieCtrlSetting*	m_pSetting;
	
				
};

#endif // _NEWBIEHINTCTRL_H_