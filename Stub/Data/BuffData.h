#pragma once
#ifndef _BUFFDATA_H_
#define _BUFFDATA_H_

#include "Stub/Data/BuffData_Stub.h"

enum BUFF_OWNER_TYPE
{
	BUFF_OWNER_TYPE_CHAR = 0,
	BUFF_OWNER_TYPE_PARNTER,
};

class CBuffData : public BuffData_Stub
{
private:
	typedef BuffData_Stub Parent;

public:
	DEFINE_CLASS(BuffData_CLASSID);
	static CBuffData * NewInstance();
	void SetData();

	void PreSend();
	static void PreSendGroup(NetGroup * buffGroup);

	void UpdateDbo();
};

inline CBuffData * CBuffData::NewInstance()
{
	return dynamic_cast<CBuffData *>(CreateNetObject(BuffData_CLASSID));
}
#endif //_BUFFDATA_H_
