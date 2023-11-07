#pragma once
#ifndef _GENERATION_H_
#define _GENERATION_H_

#include "Stub/Logic/Generation_Stub.h"

class CGeneration : public Generation_Stub
{
public:

	DEFINE_CLASS(Generation_CLASSID);
	
	BOOL IsModified() { return m_bModified;}
	void SetModifyState(BOOL bState) { m_bModified = bState;}
private:
	typedef Generation_Stub Parent;
	BOOL m_bModified;

};

#endif //_GENERATION_H_
