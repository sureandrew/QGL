#pragma once
#ifndef _TEAMDESCRIPTION_H_
#define _TEAMDESCRIPTION_H_

class CTeamDescription
{
public:
	CTeamDescription();
	~CTeamDescription();

	void ClearTeamDescriptionData();

	void CloseTeamDescription();
	void ShowTeamDescription();
	void SetTeamDescriptionData();
	void TeamDescriptionTabControl(int index);

	void TeamDescriptionInput(PCSTRINGW text);

	void TeamDescriptionLevelDown(int nNum);

	void TeamDescriptionLevelUp(int nNum);

	UInt16 GetDescriptionLevelDown(){return m_nDescriptionLevelDown;}

	UInt16 GetDescriptionLevelUp(){return m_nDescriptionLevelUp;}

private:
	int m_iDescriptionNum;

	UInt16 m_nDescriptionLevelDown;
	UInt16 m_nDescriptionLevelUp;
	std::wstring m_strInput;
};

#endif _TEAMDESCRIPTION_H_