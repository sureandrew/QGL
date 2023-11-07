#pragma once
#ifndef _MENUCHAR_H_
#define _MENUCHAR_H_



class CMenuCharProcess
{
//private:
    	
public :
	CMenuCharProcess(){}
	virtual ~CMenuCharProcess(){}

	
	void UpdateCharMenu();
	void UpdateHPBar();
	void UpdateSPBar();
	void UpdateDPBar();
	void UpdateWCBar();
	void UpdateVPBar();

	void UpdateCharTitle();
	void UpdateCharClass();
	void UpdateCharGoodness();
	void UpdateCharFaction();
	void UpdateCharFactionDonate();
	void UpdateCharHonor();
	void UpdateCharLevel();

	void UpdateCharAttr1();
	void UpdateCharAttr2();
	void UpdateCharExp();
	void UpdateCharFace();
	void UpdateCharNickName();

	void UpdateAttrPoint();

	void UpdateAttriButton();
	void UpdateStaticText(StringA fieldName, String value, Color32 textColor = 0);

	void OpenFactionDetail();
	
};


#endif 