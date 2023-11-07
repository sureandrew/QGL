#pragma once
#ifndef _GIFT_H_
#define _GIFT_H_

#include "Stub/Logic/Gift_Stub.h"
class CGiftData;

class CGift : public Gift_Stub
{
private:
	typedef Gift_Stub Parent;

public:
	DEFINE_CLASS(Gift_CLASSID);
	void SetData(CGiftData* gData);
	INLINE static CGift* NewInstance();
	INLINE void PreSend(){ SetSendMask(CGift::AllMask);}
};

INLINE CGift* CGift::NewInstance()
{
	return CreateNetObject<CGift>();
}


#endif //_GIFT_H_
