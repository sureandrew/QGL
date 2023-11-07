#pragma once
#ifndef _SELLYUANBAO_H_
#define _SELLYUANBAO_H_

#include "Proxy/Logic/SellYuanBao_Proxy.h"

class CSellYuanBao : public SellYuanBao_Proxy
{
public:
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(SellYuanBao_CLASSID);
	UInt GetRemain();

	virtual void OnUpdateSoldYBT(const UInt32 &soldYBT);

private:
	typedef SellYuanBao_Proxy Parent;

};

#endif //_SELLYUANBAO_H_
