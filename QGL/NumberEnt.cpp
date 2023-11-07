//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "NumberEnt.h"
//-- Library
#include "Math.h"
#include "Global.h"
#include "TextEntManager.h"
#include <string.h>

CNumEnt::CNumEnt()
{
	m_Number = -1;
	m_NumFileName[0] = 0;
}

CNumEnt::~CNumEnt()
{
	Release();
}

VOID CNumEnt::Release()
{
	for (size_t i = 0; i < m_NumEntLst.size(); i++)
	{
		SafeDeleteObject(m_NumEntLst[i]);
	}

	m_NumEntLst.clear();
	//m_NumFileName.clear();
}
VOID CNumEnt::SetNumber(UInt32 Num)
{
	if (m_Number == Num)
		return;

	m_Number = Num;
	UInt NumOfDigit = QGLMath::GetNumOfDigit(Num);

	if (NumOfDigit > m_NumEntLst.size())
	{
		UInt NumAdd = NumOfDigit - m_NumEntLst.size();
		for (UInt i = 0; i < NumAdd; i++)
		{
			CAnimatedEntity * pAnimedEnt = SafeCreateObject(CAnimatedEntity);
			pAnimedEnt->SetCanBeSelected(FALSE);
			pAnimedEnt->LoadEntityFile(m_NumFileName, gGlobal.pD3DDevice);
			pAnimedEnt->SetEntPixelPos(m_CurrPixelPos);
			m_NumEntLst.push_back(pAnimedEnt);
		}
	}
	else if (NumOfDigit < m_NumEntLst.size())
	{
		UInt NumDel = (UInt)m_NumEntLst.size() - NumOfDigit;

		for (UInt i = 0; i < NumDel; i++)
		{
			SafeDeleteObject(m_NumEntLst[m_NumEntLst.size() - 1]);
			m_NumEntLst.pop_back();
		}
	}

	TextEntManager::CreateEntityNumber(m_NumEntLst, m_Number, gGlobal.pD3DDevice, FALSE, m_NumFileName, NULL);

	for (size_t i = 0; i < m_NumEntLst.size(); i++)
	{
		m_NumEntLst[i]->SetExtraZBuffer(m_ExtraZValue);
		m_NumEntLst[i]->UpDate(::timeGetTime());
		m_NumEntLst[i]->m_Updated = TRUE;

	}
}

VOID CNumEnt::SetFileNameW(PCSTRINGW FileName)
{
	for (size_t i = 0; i < m_NumEntLst.size(); i++)
	{
		m_NumEntLst[i]->InitEntPicInfo();
		m_NumEntLst[i]->LoadEntityFile(FileName, gGlobal.pD3DDevice);
	}

	TextEntManager::CreateEntityNumber(m_NumEntLst, m_Number, gGlobal.pD3DDevice, FALSE, FileName, NULL);

	for (size_t i = 0; i < m_NumEntLst.size(); i++)
	{
		m_NumEntLst[i]->SetExtraZBuffer(m_ExtraZValue);
	}

	for(Int i = 0; (m_NumFileName[i] = FileName[i]) > 0; ++i);
}

VOID CNumEnt::SetEntPixelPos(PixelPos Pos)
{
	m_CurrPixelPos = Pos;
	for (size_t i = 0; i < m_NumEntLst.size(); i++)
	{
		m_NumEntLst[i]->SetEntPixelPos(m_CurrPixelPos);
	}
}

VOID CNumEnt::SetVisible(BOOL Value)
{
	for (size_t i = 0; i < m_NumEntLst.size(); i++)
	{
		m_NumEntLst[i]->SetVisible(Value);
	}
}

VOID CNumEnt::SetExtraZValue(Int64 Value)
{
	m_ExtraZValue = Value;
	for (size_t i = 0; i < m_NumEntLst.size(); i++)
	{
		m_NumEntLst[i]->SetExtraZBuffer(m_ExtraZValue);
	}
}