#pragma once
#ifndef _CREATECHARACTER_H_
#define _CREATECHARACTER_H_


class CCreateCharacterProcess
{
private:
	//int m_iHairIndex;
	int m_iHeadIndex;
	int m_iSex;
	int m_iClass;
	int m_iSexClass;
	int m_iColorIndex;
	void CalSexClass();
	int m_iClothIndex;
	int m_iClothColorIndex;

public:
	CCreateCharacterProcess();
	void ShowCreateCharacter();
	void ChangeCharacterHairCol(bool next); //1: next hair color, 0: prev hair color
	void ChangeCharacterHead(bool next);
	INLINE int	GetSex(){return m_iSex;}
	INLINE int GetClass(){return m_iClass;}
	INLINE int GetHeadIndex(){return m_iHeadIndex;}
	INLINE int GetHairColIndex(){return m_iColorIndex;}
	void SetAniIconHighLight(int index, bool highLight);

	void ChangeCharacterCloth(bool next);

	void ChangeCharacterClothCol(bool next);

	void RotateCharacter(bool rotateLeft); //true: rotate left, false: rotate right

	INLINE int GetClothColIndex(){return m_iClothColorIndex;}
	INLINE int GetClothIndex(){return m_iClothIndex;}
		
	void SetClass(int cclass)
	{ 
		m_iClass = cclass;
		//m_iHairIndex = 1;
		m_iColorIndex = 1;
		m_iHeadIndex = 1;
		m_iClothColorIndex = 1;
		m_iClothIndex = 1;
		CalSexClass();
	}

	void SetSex(int sex)
	{
		m_iSex = sex;
		//m_iHairIndex = 1;
		m_iColorIndex = 1;
		m_iHeadIndex = 1;
		m_iClothColorIndex = 1;
		m_iClothIndex = 1;
		CalSexClass();
	}

};



#endif