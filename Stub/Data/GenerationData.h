#pragma once
#ifndef _GENERATION_DATA_H_
#define _GENERATION_DATA_H_

#include "Stub/Data/GenerationData_Stub.h"

class CdboGeneration;

class CGenerationData : public GenerationData_Stub
{
public:

	DEFINE_CLASS(GenerationData_CLASSID);

	static CGenerationData* NewInstance();
	void SetData(CdboGeneration* dboGeneration,BOOL bYoung,BOOL bCreate);
	void PreSend();
	HRESULT UpdateDbo(CdboGeneration* dboGeneration);
	HRESULT DeleteDbo(CdboGeneration* dboGeneration);

private:
	typedef GenerationData_Stub Parent;
	static ThreadKey m_keyGeneration;
};

inline CGenerationData * CGenerationData::NewInstance()
{
	return CreateNetObject<CGenerationData>();
}
#endif