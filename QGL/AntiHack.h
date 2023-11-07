#pragma once
#ifndef _ANTIHACK_H_
#define _ANTIHACK_H_

const UInt16 hackImage[49] = { 1, 3, 6, 12, 14, 15, 17, 20, 21, 22,
							  23, 24, 25, 26, 29, 30, 31, 33, 34, 37, 
							  38, 40, 41, 44, 46, 48, 50, 51, 52, 54, 
							  57, 58, 60, 63, 66, 67, 68, 72, 76, 77, 
							  78, 79, 81, 82, 83, 84, 86, 90, 94};

class CAntiHack
{
public:
	CAntiHack(void);
	~CAntiHack(void);

	////////////// the anti-hack function and data members////////// 
	typedef StlVector<Byte, ManagedAllocator<Byte> > ByteVector; 
	void  SetHack(UInt8 answer);
	void OpenAntiHackPopUp(ByteArray &picture, AntiHackData &data);	
	void CloseAntiHackPanel();
	void DrawAntiHackImage(UInt8 &picIndex, const UInt16 &mobID1, const UInt16 &mobID2, Boolean overlap);	

	Boolean m_bEnterBattle;	
	Boolean m_bBattleDlg;
	Boolean m_firstLogin;
	
};


#endif
