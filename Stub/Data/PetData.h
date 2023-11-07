#pragma once
#ifndef _PETDATA_H_
#define _PETDATA_H_

#include "Stub/Data/PetData_Stub.h"

class CdboPet;
class CPetData : public PetData_Stub
{
public:
	DEFINE_CLASS(PetData_CLASSID);
	static CPetData * NewInstance();
	void SetData(CdboPet* dboPet);
	void PreSend();
	HRESULT UpdateDbo(CdboPet* dboPet);
	HRESULT DeleteDbo(CdboPet* dboPet);

private:
	typedef PetData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CPetData * CPetData::NewInstance()
{
	return CreateNetObject<CPetData>();
}

#endif //_PETDATA_H_
