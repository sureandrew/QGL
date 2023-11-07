#pragma once
#include <Reuben/Utility/Singleton.h>

#ifndef _WANTEDPROCESS_H_
#define _WANTEDPROCESS_H_
class CRankCharacter;
class CWantedProcess:public Reuben::Utility::Singleton<CWantedProcess>
{
private:

	UInt m_iStartIndex;
	int m_iSelectedIndex;

	NetGroup	* m_grpWantedClass3;
	NetGroup	* m_grpWantedClass2;
	NetGroup	* m_grpWantedClass1;
	UInt32 m_nSelectedClass;//1,2,3
	

public:
	CWantedProcess();
	~CWantedProcess();

	void ClearAllWanted();
	void RefreshWantedList();
	void SelectWanted(int index);
	void WantedScroll(int scroll); //0: up, 1:down, 2: scroll
	NetGroup* GetWantedList();
	void SelectWantedClass(UInt32 nClass);
	void OpenPanel(NetGroup	* netGroup);
	void RequestWantedReward();
	void WantedReward(UInt32 money);
	void WantedDetail();
	void WantedJailDelivery(UInt32 char_id);
	void WantedBail(UInt32 char_id);
	void WantedTrace(UInt32 char_id);
};

#endif 
