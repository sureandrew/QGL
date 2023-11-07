//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "CreateCharacterProcess.h"
//-- Library
#include "QGL/Global.h"
#include "QGL/CharacterControl.h"
#include "Resource/ResCharacter.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "Resource/ClientResManager.h"



CCreateCharacterProcess::CCreateCharacterProcess()
{			
			m_iSex = 0;
			m_iClass = 1;
			m_iColorIndex = 1;
			m_iHeadIndex = 1;
			CalSexClass();
			m_iClothIndex = 1;
			m_iClothColorIndex = 1;
}

void CCreateCharacterProcess::CalSexClass()
{
	m_iSexClass = 1 + (m_iClass - 1) * MAX_SEXTYPE + m_iSex;
}

void CCreateCharacterProcess::ShowCreateCharacter()
{
	
	CAnimatedEntity temp;
	temp.InitEntPicInfo();
	
	temp.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
	StringA sexClassStr;

	char buf[256];
	sprintf(buf, "h%05d_t001.ent", m_iSexClass);
	temp.LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_HEAD);
	
	sprintf(buf, "j%05d_t001.ent", m_iSexClass);
	temp.LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_HAIR);
	
	sprintf(buf, "b%05d_t001.ent", m_iSexClass);
	temp.LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_BODY);
	
	UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("CharacterModel");

	if (pAniIcon)
	{
		pAniIcon->ClearAniData();
		pAniIcon->SetAnimationIndex(ANIM_RUN);
		pAniIcon->SetRotation(SOUTHEAST);
		pAniIcon->SetAnimationData(&temp);
		
	}

	gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);

	StringA charLImgName;

	charLImgName.Format("CharacterLImage%d", m_iSexClass);
	UIImage* characterLImage = (UIImage*) gGlobal.pUIManager->GetUIItem(charLImgName.c_str());
	if (characterLImage)
		characterLImage->SetEnable(true);

	StringA hairStyle;
	hairStyle.Format("HairStyle%d", m_iHeadIndex);
	UIImage* Image = (UIImage*) gGlobal.pUIManager->GetUIItem(hairStyle.c_str());
	if (Image)
		Image->SetEnable(true);

	StringA color;
	color.Format("HairColor%d", m_iColorIndex);
	Image = (UIImage*) gGlobal.pUIManager->GetUIItem(color.c_str());
	if (Image)
		Image->SetEnable(true);

	/*StringA cloth;
	cloth.Format("Cloth%d", 1);
	Image = (UIImage*) gGlobal.pUIManager->GetUIItem(cloth.c_str());
	if (Image)
		Image->SetEnable(true);

	color.Format("ClothColor%d", 1);
	Image = (UIImage*) gGlobal.pUIManager->GetUIItem(color.c_str());
	if (Image)
		Image->SetEnable(true);*/

	StringA charName;
	charName.Format("CharDefautName%d", m_iSexClass);
	Image = (UIImage*) gGlobal.pUIManager->GetUIItem(charName.c_str());
	if (Image)
		Image->SetEnable(true);

	TCHAR desc[301];
	TCHAR weapon[51];
	switch (m_iSexClass)
	{
	case 1:
		GetPrivateProfileString(_T("MALEPALADIN"), _T("DESC"), _T("\0"), desc, 300, _T(".\\Config\\Decs.ini"));
		GetPrivateProfileString(_T("MALEPALADIN"), _T("WEAPON"), _T("\0"), weapon, 50, _T(".\\Config\\Decs.ini"));
		break;
	case 2:
		GetPrivateProfileString(_T("FEMALEPALADIN"), _T("DESC"), _T("\0"), desc, 300, _T(".\\Config\\Decs.ini"));
		GetPrivateProfileString(_T("FEMALEPALADIN"), _T("WEAPON"), _T("\0"), weapon, 50, _T(".\\Config\\Decs.ini"));
		break;
	case 3:
		GetPrivateProfileString(_T("MALETHIEF"), _T("DESC"), _T("\0"), desc, 300, _T(".\\Config\\Decs.ini"));
		GetPrivateProfileString(_T("MALETHIEF"), _T("WEAPON"), _T("\0"), weapon, 50, _T(".\\Config\\Decs.ini"));
		break;
	case 4:
		GetPrivateProfileString(_T("FEMALETHIEF"), _T("DESC"), _T("\0"), desc, 300, _T(".\\Config\\Decs.ini"));
		GetPrivateProfileString(_T("FEMALETHIEF"), _T("WEAPON"), _T("\0"), weapon, 50, _T(".\\Config\\Decs.ini"));
		break;
	case 5:
		GetPrivateProfileString(_T("MALEDOCTOR"), _T("DESC"), _T("\0"), desc, 300, _T(".\\Config\\Decs.ini"));
		GetPrivateProfileString(_T("MALEDOCTOR"), _T("WEAPON"), _T("\0"), weapon, 50, _T(".\\Config\\Decs.ini"));
		break;
	case 6:
		GetPrivateProfileString(_T("FEMALEDOCTOR"), _T("DESC"), _T("\0"), desc, 300, _T(".\\Config\\Decs.ini"));
		GetPrivateProfileString(_T("FEMALEDOCTOR"), _T("WEAPON"), _T("\0"), weapon, 50, _T(".\\Config\\Decs.ini"));
		break;
	case 7:
		GetPrivateProfileString(_T("MALEASSASSIN"), _T("DESC"), _T("\0"), desc, 300, _T(".\\Config\\Decs.ini"));
		GetPrivateProfileString(_T("MALEASSASSIN"), _T("WEAPON"), _T("\0"), weapon, 50, _T(".\\Config\\Decs.ini"));
		break;
	case 8:
		GetPrivateProfileString(_T("FEMALEASSASSIN"), _T("DESC"), _T("\0"), desc, 300, _T(".\\Config\\Decs.ini"));
		GetPrivateProfileString(_T("FEMALEASSASSIN"), _T("WEAPON"), _T("\0"), weapon, 50, _T(".\\Config\\Decs.ini"));
		break;
	}
	desc[300] = _T('\0');
	weapon[50] = _T('\0');

	UIStaticText* descText = gGlobal.pUIManager->FindUIStaticText("CreateCharacterDESC");
	UIStaticText* weaponText = gGlobal.pUIManager->FindUIStaticText("CreateCharacterWPDESC");

	if (descText && weaponText)
	{
		//descText->SetImage(3, 25);
		descText->SetText(String(desc).c_str());
		descText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(descText);
		

		weaponText->SetText(String(weapon).c_str());
		weaponText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(weaponText);
	}
}	

void CCreateCharacterProcess::ChangeCharacterHairCol(bool next)
{
	UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("CharacterModel");

	StringA color;
	color.Format("HairColor%d", m_iColorIndex);
	UIImage* hairColor = (UIImage*) gGlobal.pUIManager->GetUIItem(color.c_str());
	if (hairColor)
		hairColor->SetEnable(false);

	Int headColLimit = GetPrivateProfileInt(_T("CREATECHARACTER"), _T("NOOFHEADCOL"), 2, CONFIGURATION_INI_FILE);

	if (next)
	{
		if (m_iColorIndex < headColLimit)
			++m_iColorIndex;
		else
			m_iColorIndex = 1;
	}
	else
	{	
		if (m_iColorIndex > 1)
			--m_iColorIndex;
		else
			m_iColorIndex = headColLimit;
	}

	CAnimatedEntity* characterModel;
	if (pAniIcon)
	{
		characterModel = pAniIcon->GetAniEnt();
		if (characterModel)
		{
			String buf, buf2;
			char abc = 96 + m_iColorIndex;

			buf.Format(_T("j%05d_t%03d.ent"), m_iSexClass, m_iHeadIndex);
			buf2.Format(_T("j%05d_t%03d%c.tga"), m_iSexClass, m_iHeadIndex, abc);
			characterModel->ChangeColorP(buf.c_str(), buf2.c_str());

			buf.Format(_T("h%05d_t%03d.ent"), m_iSexClass, m_iHeadIndex);
			buf2.Format(_T("h%05d_t%03d%c.tga"), m_iSexClass, m_iHeadIndex, abc);
			characterModel->ChangeColorP(buf.c_str(), buf2.c_str());

			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
		}

		color.Format("HairColor%d", m_iColorIndex);
		UIImage* hairColor = (UIImage*) gGlobal.pUIManager->GetUIItem(color.c_str());
		if (hairColor)
			hairColor->SetEnable(true);
	}
}

void CCreateCharacterProcess::ChangeCharacterHead(bool next)
{
	
	StringA hairStyle;
	hairStyle.Format("HairStyle%d", m_iHeadIndex);
	UIImage* head = (UIImage*) gGlobal.pUIManager->GetUIItem(hairStyle.c_str());
	if (head)
		head->SetEnable(false);

	Int headLimit = GetPrivateProfileInt(_T("CREATECHARACTER"), _T("NOOFHEAD"), 1, CONFIGURATION_INI_FILE);

	if (next)
	{
		if (m_iHeadIndex < headLimit)
			++m_iHeadIndex;
		else 
			m_iHeadIndex = 1;
	}
	else
	{	
		if (m_iHeadIndex > 1)
			--m_iHeadIndex;
		else
			m_iHeadIndex = headLimit;
	}
	
	UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("CharacterModel");
	if (pAniIcon)
	{
		CAnimatedEntity* characterModel = pAniIcon->GetAniEnt();
		if (characterModel)
		{
			characterModel->RemovePart(EQUIP_HEAD);
			characterModel->RemovePart(EQUIP_HAIR);
			String buf, buf2;
			char abc = 96 + m_iColorIndex;

			buf.Format(_T("h%05d_t%03d.ent"), m_iSexClass, m_iHeadIndex);
			characterModel->LoadEntityFile(buf.c_str(), gGlobal.pD3DDevice, EQUIP_HEAD);

			buf2.Format(_T("h%05d_t%03d%c.tga"), m_iSexClass, m_iHeadIndex, abc);
			characterModel->ChangeColorP(buf.c_str(), buf2.c_str());
			
			buf.Format(_T("j%05d_t%03d.ent"), m_iSexClass, m_iHeadIndex);
			characterModel->LoadEntityFile(buf.c_str(), gGlobal.pD3DDevice, EQUIP_HAIR);
		
			buf2.Format(_T("j%05d_t%03d%c.tga"), m_iSexClass, m_iHeadIndex, abc);
			characterModel->ChangeColorP(buf.c_str(), buf2.c_str());
			//characterModel->SetCurrFrameIndex(0);
			characterModel->SetAnimationIndexByIndex(ANIM_IDLE1);
			characterModel->SetAnimationIndexByIndex(ANIM_RUN);
			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
		}

		hairStyle.Format("HairStyle%d", m_iHeadIndex);
		head = (UIImage*) gGlobal.pUIManager->GetUIItem(hairStyle.c_str());
		if (head)
			head->SetEnable(true);
	}	
}

void CCreateCharacterProcess::SetAniIconHighLight(int index, bool highLight)
{
	StringA aniName;
	aniName.Format("ItemModel%d", index);

	UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(aniName.c_str());
	if (pAniIcon)
	{
		CAnimatedEntity* characterModel = pAniIcon->GetAniEnt();
		if (characterModel)
		{
			characterModel->SetHighLight(highLight);
			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
		}
	}
}

void CCreateCharacterProcess::ChangeCharacterCloth(bool next)
{
	UIAniIcon * pAniIcon;
	pAniIcon = (UIAniIcon*) gGlobal.pUIManager->GetUIItem("CharacterModel");

	StringA cloth;
	cloth.Format("Cloth%d", m_iClothIndex);
	UIImage* clothImage = (UIImage*)gGlobal.pUIManager->GetUIItem(cloth.c_str());
	if (clothImage)
		clothImage->SetEnable(false);

	Int clothLimit = GetPrivateProfileInt(_T("CREATECHARACTER"), _T("NOOFCLOTH"), 2, CONFIGURATION_INI_FILE);
	if (next)
	{
		if (m_iClothIndex < clothLimit)
			++m_iClothIndex;
		else
			m_iClothIndex = 1;
	}
	else 
	{
		if (m_iClothIndex > 1)
			--m_iClothIndex;
		else
			m_iClothIndex = clothLimit;
	}

	CAnimatedEntity* characterModel;
	if (pAniIcon)
	{
		characterModel = pAniIcon->GetAniEnt();
		if (characterModel)
		{
			characterModel->RemovePart(EQUIP_BODY);

			char buf[256];
			sprintf(buf, "b%05d_t%03d.ent", m_iSexClass, m_iClothIndex);
			characterModel->LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_BODY);

			char buf2[256];
			char abc = 96 + m_iClothColorIndex;
			
			sprintf(buf2, "b%05d_t%03d%c.tga", m_iSexClass, m_iClothIndex, abc);
			characterModel->ChangeColorP(String(buf).c_str(), String(buf2).c_str());
			characterModel->SetAnimationIndexByIndex(ANIM_RUN);
			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
		}

		cloth.Format("Cloth%d", m_iClothIndex);
		UIImage* clothImage = (UIImage*)gGlobal.pUIManager->GetUIItem(cloth.c_str());
		if (clothImage)
			clothImage->SetEnable(true);
	}
}

void CCreateCharacterProcess::ChangeCharacterClothCol(bool next)
{
	StringA color;
	color.Format("ClothColor%d", m_iClothColorIndex);
	UIImage* clothColor = (UIImage*) gGlobal.pUIManager->GetUIItem(color.c_str());
	if (clothColor)
		clothColor->SetEnable(false);

	Int clothColLimit = GetPrivateProfileInt(_T("CREATECHARACTER"), _T("NOOFCLOTHCOL"), 2, CONFIGURATION_INI_FILE);

	if (next)
	{
		if (m_iClothColorIndex < clothColLimit)
			++m_iClothColorIndex;
		else 
			m_iClothColorIndex = 1;
	}
	else
	{
		if (m_iClothColorIndex > 1)
			--m_iClothColorIndex;
		else
			m_iClothColorIndex = clothColLimit ;
	}

	UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("CharacterModel");
	if (pAniIcon)
	{
		CAnimatedEntity* characterModel = pAniIcon->GetAniEnt();
		if (characterModel)
		{
			char buf[256];
			sprintf(buf, "b%05d_t%03d.ent", m_iSexClass, m_iClothIndex);

			char buf2[256];
			char abc = 96 + m_iClothColorIndex;
			
			sprintf(buf2, "b%05d_t%03d%c.tga", m_iSexClass, m_iClothIndex, abc);
			characterModel->ChangeColorP(String(buf).c_str(), String(buf2).c_str());

			gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
		}

		color.Format("ClothColor%d", m_iClothColorIndex);
		UIImage* clothColor = (UIImage*) gGlobal.pUIManager->GetUIItem(color.c_str());
		if (clothColor)
			clothColor->SetEnable(true);
	}
}

void CCreateCharacterProcess::RotateCharacter(bool rotateLeft)
{
	UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("CharacterModel");
	if (pAniIcon)
	{
		int rotation = pAniIcon->GetRotation();
		if (rotateLeft)
		{
			++rotation;
			if ( rotation == MAXROTATION )
				rotation = SOUTH;
		}
		else
		{
			if (rotation == SOUTH)
				rotation = MAXROTATION ; 
			--rotation;
		}
		pAniIcon->SetRotation(rotation);
		gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
	}
}