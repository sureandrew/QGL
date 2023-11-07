#include "Common.h"
// Self
#include "NewBieHintCtrl.h"



CNewBieHintCtrl::CNewBieHintCtrl(void)
{
	m_pSetting = NULL;
	m_bWriteFlag = BOOLEAN_FALSE;
}

CNewBieHintCtrl::~CNewBieHintCtrl(void)
{
	SafeDeallocate(m_pSetting);
}

VOID CNewBieHintCtrl::ReadSetting(UInt32 accountID)
{
	if ( !m_bWriteFlag && accountID > 0)
	{
		if ( m_pSetting == NULL )
		{
			//WriteSetting(accountID);
			m_pSetting = SafeAllocate(SNewBieCtrlSetting, 1);
			m_pSetting->CharId_1 = 0;
			m_pSetting->CharId_2 = 0;
			m_pSetting->CharId_3 = 0;

			m_pSetting->bNewBie_1 = BOOLEAN_FALSE;
			m_pSetting->bNewBie_2 = BOOLEAN_FALSE;
			m_pSetting->bNewBie_3 = BOOLEAN_FALSE;
		}

		if ( m_pSetting )
		{
			FILE* pFile = NULL;
			String temp;
			temp.Format(_T(".\\Config\\account%d.nbs"), accountID);
			pFile = _tfopen(temp.c_str(), _T("rb"));
			if ( pFile )
			{
				fread(m_pSetting, sizeof(SNewBieCtrlSetting), 1, pFile);

				fclose(pFile);
			}
		}
	}
}

VOID CNewBieHintCtrl::WriteSetting(UInt32 accountID)
{
	if ( !m_bWriteFlag && accountID > 0)
	{
		m_bWriteFlag = BOOLEAN_TRUE;

		if ( !m_pSetting )
		{
			m_pSetting = SafeAllocate(SNewBieCtrlSetting, 1);
			m_pSetting->CharId_1 = 0;
			m_pSetting->CharId_2 = 0;
			m_pSetting->CharId_3 = 0;

			m_pSetting->bNewBie_1 = BOOLEAN_FALSE;
			m_pSetting->bNewBie_2 = BOOLEAN_FALSE;
			m_pSetting->bNewBie_3 = BOOLEAN_FALSE;
		}

		if ( m_pSetting )
		{
			FILE* pFile = NULL;
			String temp;
			temp.Format(_T(".\\Config\\account%d.nbs"), accountID);
			pFile = _tfopen(temp.c_str(), _T("wb"));
			if ( pFile )
			{
				fwrite(m_pSetting, sizeof(SNewBieCtrlSetting), 1, pFile);

				fclose(pFile);
			}
		}

		m_bWriteFlag = BOOLEAN_FALSE;
	}
}

Boolean CNewBieHintCtrl::IsNewBie(Int SelCharNum)
{
	if ( m_pSetting && !m_bWriteFlag )
	{
		if ( SelCharNum == 1 )
		{
			return m_pSetting->bNewBie_1;
		}
		else if ( SelCharNum == 2 )
		{
			return m_pSetting->bNewBie_2;
		}
		else if ( SelCharNum == 3 )
		{
			return m_pSetting->bNewBie_3;
		}
	}

	return BOOLEAN_FALSE;
}

VOID CNewBieHintCtrl::SetNewBie(Int SelCharNum)
{
	if ( !m_bWriteFlag )
	{
		m_bWriteFlag = BOOLEAN_TRUE;

		if ( !m_pSetting )
		{
			m_pSetting = SafeAllocate(SNewBieCtrlSetting, 1);
			m_pSetting->CharId_1 = 0;
			m_pSetting->CharId_2 = 0;
			m_pSetting->CharId_3 = 0;

			m_pSetting->bNewBie_1 = BOOLEAN_FALSE;
			m_pSetting->bNewBie_2 = BOOLEAN_FALSE;
			m_pSetting->bNewBie_3 = BOOLEAN_FALSE;
		}

		if ( m_pSetting )
		{
			if ( SelCharNum == 1 )
			{
				m_pSetting->bNewBie_1 = BOOLEAN_TRUE;
			}
			else if ( SelCharNum == 2 )
			{
				m_pSetting->bNewBie_2 = BOOLEAN_TRUE;
			}
			else if ( SelCharNum == 3 )
			{
				m_pSetting->bNewBie_3 = BOOLEAN_TRUE;
			}
		}

		m_bWriteFlag = BOOLEAN_FALSE;
	}
}

VOID CNewBieHintCtrl::ResetBie()
{
	if (m_pSetting)
	{
		m_pSetting->CharId_1 = 0;
		m_pSetting->CharId_2 = 0;
		m_pSetting->CharId_3 = 0;

		m_pSetting->bNewBie_1 = BOOLEAN_FALSE;
		m_pSetting->bNewBie_2 = BOOLEAN_FALSE;
		m_pSetting->bNewBie_3 = BOOLEAN_FALSE;
	}
}

VOID CNewBieHintCtrl::SetBie(UInt32 char_id, Boolean isNew)
{
	if (m_pSetting)
	{
		if (m_pSetting->CharId_1 == char_id)
		{
			m_pSetting->bNewBie_1 = isNew;
		}
		else if (m_pSetting->CharId_2 == char_id)
		{
			m_pSetting->bNewBie_2 = isNew;
		}
		else if (m_pSetting->CharId_3 == char_id)
		{
			m_pSetting->bNewBie_3 = isNew;
		}
		else if (m_pSetting->CharId_1 == 0)
		{
			m_pSetting->CharId_1 = char_id;
			m_pSetting->bNewBie_1 = isNew;
		}
		else if (m_pSetting->CharId_2 == 0)
		{
			m_pSetting->CharId_2 = char_id;
			m_pSetting->bNewBie_2 = isNew;
		}
		else if (m_pSetting->CharId_3 == 0)
		{
			m_pSetting->CharId_3 = char_id;
			m_pSetting->bNewBie_3 = isNew;
		}
	}
}

Boolean CNewBieHintCtrl::IsExistBie(UInt32 char_id)
{
	if (m_pSetting)
	{
		if (m_pSetting->CharId_1 == char_id || m_pSetting->CharId_2 == char_id || m_pSetting->CharId_3 == char_id )
			return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

Boolean CNewBieHintCtrl::IsNewBieByChar_id(UInt32 char_id)
{
	if (m_pSetting->CharId_1 == char_id)
		return m_pSetting->bNewBie_1;
	else if (m_pSetting->CharId_2 == char_id)
		return m_pSetting->bNewBie_2;
	else if (m_pSetting->CharId_3 == char_id)
		return m_pSetting->bNewBie_3;

	// if char_id is not found in .nbs, it should be new.
	return BOOLEAN_TRUE;
}

VOID CNewBieHintCtrl::ClearNewBie(UInt32 char_id)
{
	if (m_pSetting)
	{
		if (m_pSetting->CharId_1 == char_id)
		{
			m_pSetting->CharId_1 = 0;
			m_pSetting->bNewBie_1 = BOOLEAN_FALSE;
			return;
		}
		else if (m_pSetting->CharId_2 == char_id)
		{
			m_pSetting->CharId_2 = 0;
			m_pSetting->bNewBie_2 = BOOLEAN_FALSE;
			return;
		}
		else if (m_pSetting->CharId_3 == char_id)
		{
			m_pSetting->CharId_3 = 0;
			m_pSetting->bNewBie_3 = BOOLEAN_FALSE;
			return;
		}
	}
}

Boolean CNewBieHintCtrl::CleanupNonExistBie(UInt32 char_id1, UInt32 char_id2, UInt32 char_id3)
{
	Boolean bReset = BOOLEAN_FALSE;
	if (m_pSetting)
	{
		UInt32 testChar;
		testChar = m_pSetting->CharId_1;
		if (testChar != 0 &&
			testChar != char_id1 && 
			testChar != char_id2 && 
			testChar != char_id3
			)
		{
			bReset = BOOLEAN_TRUE;
			ClearNewBie(testChar);
		}
		testChar = m_pSetting->CharId_2;
		if (testChar != 0 &&
			testChar != char_id1 && 
			testChar != char_id2 && 
			testChar != char_id3
			)
		{
			bReset = BOOLEAN_TRUE;
			ClearNewBie(testChar);
		}
		testChar = m_pSetting->CharId_3;
		if (testChar != 0 &&
			testChar != char_id1 && 
			testChar != char_id2 && 
			testChar != char_id3
			)
		{
			bReset = BOOLEAN_TRUE;
			ClearNewBie(testChar);
		}
	}
	return bReset;
}
