//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "TextEntManager.h"
//-- Library
#include "AnimatedEntity.h"

VOID TextEntManager::CreateEntityNumber(StlVector<CAnimatedEntity *> & NumLst, Int Num, LPDIRECT3DDEVICE9	pD3DDevice,
										BYTE AddSymbolIndex, PCSTRINGW NumFileName, PCSTRINGW SymbolFileName)
{
	UInt Index = 0;
	CAnimatedEntity * pSysmbolEnt = NULL;
	if (AddSymbolIndex > 0)
	{
		if (NumLst.size() > Index)
			pSysmbolEnt = NumLst[Index];
		else
		{
			pSysmbolEnt  = SafeCreateObject(CAnimatedEntity);
		}
		pSysmbolEnt->LoadEntityFile(SymbolFileName, pD3DDevice);
		pSysmbolEnt->SetAnimationIndexByIndex(AddSymbolIndex);
		pSysmbolEnt->UpDate(::timeGetTime());
		Index ++;
		/*if (Num > 0)
		{
			if (NumLst.size() > Index)
				pSysmbolEnt = NumLst[Index];
			else
			{
				pSysmbolEnt  = SafeCreateObject(CAnimatedEntity);
			}
			Char FileName[50];
			GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUMADD"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
			pSysmbolEnt->LoadEntityFile(FileName, pD3DDevice);
			pSysmbolEnt->UpDate(::timeGetTime());

			Index ++;
		}
		else if (Num < 0)
		{
			if (NumLst.size() > Index)
				pSysmbolEnt = NumLst[Index];
			else
			{
				pSysmbolEnt  = SafeCreateObject(CAnimatedEntity);
			}
			Char FileName[50];
			GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUMSUB"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
			pSysmbolEnt->LoadEntityFile(FileName, pD3DDevice);
			pSysmbolEnt->UpDate(::timeGetTime());
			
			Index ++;
		}*/
	}

	if (Num == 0)
	{
		CAnimatedEntity * pEnt = NULL;
		if (NumLst.size() > Index)
			pEnt = NumLst[Index];
		else
		{
			pEnt  = SafeCreateObject(CAnimatedEntity);
			NumLst.push_back(pEnt);
			pEnt->LoadEntityFile(NumFileName, pD3DDevice);
		}
		//pEnt->SetCurrFrameIndex(0);
		//pEnt->SetStopAnimation(TRUE);
		pEnt->SetAnimationIndexByIndex(0);
		pEnt->SetAnimationLoop(FALSE);
		Index ++;
		
	}
	else
	{
		for (;Num != 0; Num /= 10)
		{
			Int Digit = Num % 10;
			CAnimatedEntity * pEnt = NULL;
			if (NumLst.size() > Index)
				pEnt = NumLst[Index];
			else
			{
				pEnt  = SafeCreateObject(CAnimatedEntity);
				NumLst.push_back(pEnt);
				pEnt->LoadEntityFile(NumFileName, pD3DDevice);
			}
			//pEnt->SetStopAnimation(FALSE);
			pEnt->SetAnimationIndexByIndex(abs(Digit));
			/*switch(abs(Digit))
			{
			case 0:
				//pEnt->SetCurrFrameIndex(0);
				pEnt->SetAnimationIndexByIndex(0);
				//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUM0"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
				break;
			case 1:
				//pEnt->SetCurrFrameIndex(1);
				pEnt->SetAnimationIndexByIndex(1);
				//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUM1"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
				break;
			case 2:
				//pEnt->SetCurrFrameIndex(2);
				pEnt->SetAnimationIndexByIndex(2);
				//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUM2"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
				break;
			case 3:
				//pEnt->SetCurrFrameIndex(3);
				pEnt->SetAnimationIndexByIndex(3);
				//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUM3"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
				break;
			case 4:
				//pEnt->SetCurrFrameIndex(4);
				pEnt->SetAnimationIndexByIndex(4);
				//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUM4"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
				break;
			case 5:
				//pEnt->SetCurrFrameIndex(5);
				pEnt->SetAnimationIndexByIndex(5);
				//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUM5"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
				break;
			case 6:
				//pEnt->SetCurrFrameIndex(6);
				pEnt->SetAnimationIndexByIndex(6);
				//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUM6"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
				break;
			case 7:
				//pEnt->SetCurrFrameIndex(7);
				pEnt->SetAnimationIndexByIndex(7);
				//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUM7"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
				break;
			case 8:
				//pEnt->SetCurrFrameIndex(8);
				pEnt->SetAnimationIndexByIndex(8);
				//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUM8"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
				break;
			case 9:
				//pEnt->SetCurrFrameIndex(9);
				pEnt->SetAnimationIndexByIndex(9);
				//GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("NUM9"), _T(""), FileName, 50, _T(".\\Config\\Setting.ini"));
				break;
			}*/
			//pEnt->UpDate(::timeGetTime());
			//pEnt->LoadEntityFile(FileName, pD3DDevice);

			pEnt->SetAnimationLoop(FALSE);
			//pEnt->SetStopAnimation(TRUE);
			Index ++;
			//NumLst.push_back(pEnt);
		}

	}
	//perform swapping so that the number is in order
	if (NumLst.size() > 0)
	{
		for (Int i = 0; i < (Int)NumLst.size() / 2; i ++)
		{
			CAnimatedEntity * pEnt = NumLst[i];
			NumLst[i] = NumLst[NumLst.size() - 1 - i]; 
			NumLst[NumLst.size() - 1 - i] = pEnt;
		}
	}
	
	Int TotalWidth = 0;
	for (Int i = 0; i < (Int)NumLst.size(); i++)
	{
		TotalWidth += NumLst[i]->m_EntOrgWidth;
	}

	for (Int i = 0; i < (Int)NumLst.size(); i++)
	{
		//if (AddSymbolIndex && i == 0)
		//	NumLst[i]->SetEntOffsetX((Int)- TotalWidth / 2);
		//else
			NumLst[i]->SetEntOffsetX((Int)((i + 0.5)* TotalWidth / (Int)NumLst.size() - TotalWidth / 2));
	}

	if (pSysmbolEnt)
	{
		Int Left = (Int)((0.5)* TotalWidth / (Int)NumLst.size() - TotalWidth / 2);
		NumLst.push_back(pSysmbolEnt);
		for (Int i = (Int)NumLst.size() - 1; i >= 1 ; i--)
		{
			CAnimatedEntity * pEnt = NumLst[i];
			NumLst[i] = NumLst[i - 1]; 
			NumLst[i - 1] = pEnt;
		}

		NumLst[0]->SetEntOffsetX(- 1 * pSysmbolEnt->m_EntOrgWidth / 2 + Left - 10);
	}

}