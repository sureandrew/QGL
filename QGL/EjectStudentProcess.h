#pragma once
#include <Reuben/Utility/Singleton.h>

#ifndef _EJECTSTUDENTPROCESS_H_
#define _EJECTSTUDENTPROCESS_H_
class CMessageCharacter;
class CEjectStudentProcess:public Reuben::Utility::Singleton<CEjectStudentProcess>
{
private:
	typedef StlVector<UInt32>  EjectVector;

	EjectVector m_ejectVector;

	UInt m_iStartIndex;
	int m_iSelectedIndex;
	

public:
	CEjectStudentProcess();
	~CEjectStudentProcess();

	void ClearAllEject();

	void AddEjectToList(UInt32 student_id);
	void RemoveEjectFromList(UInt32 student_id);
	void ShowEjectStudent();
	void SelectEject(int index);
	void DBClickEject(int index);
	void EjectScroll(int scroll); //0: up, 1:down, 2: scroll
	bool IsOpenEject();
	inline void CloseEjectStudent()
	{
		m_iStartIndex = 0;
		m_iSelectedIndex = -1;
	}

	UInt32 GetEjectByMenuIndex(UInt index);
	void EjectStudentButtonOK();
	void EjectStudent();

};

#endif 
