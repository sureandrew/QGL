#pragma once
#ifndef _SELLYUANBAO_H_
#define _SELLYUANBAO_H_

#include "Stub/Logic/SellYuanBao_Stub.h"

class CSellYuanBaoData;
class CSellYuanBaoGroup;

class CSellYuanBao : public SellYuanBao_Stub,
					public Reuben::Simulation::IEventHandler<CSellYuanBao>
{

private:
	typedef SellYuanBao_Stub Parent;	
	CSellYuanBaoGroup* m_pOwnerGroup;
	UInt m_iReserve;
public:

	DEFINE_CLASS(SellYuanBao_CLASSID);

	CSellYuanBao();
	virtual ~CSellYuanBao();

	inline void SetOwnerGroup(CSellYuanBaoGroup* grp) { m_pOwnerGroup = grp;}

	inline static CSellYuanBao* NewInstance();
	void PreSend();
	void SetData(CSellYuanBaoData* pSellYuanBaoData);

	virtual void HandleEvent(CONST Event &event);
	UInt GetRemain();

	void Reserve(Int reserve);
	void AddSold(UInt solded, bool reduceReserve = true);

	enum EventEnum
	{		
		EVT_UPDATE_EXPIRE = MAKE_EVENTID(SellYuanBao_CLASSID, 1),
	};
};

inline CSellYuanBao* CSellYuanBao::NewInstance()
{
	return CreateNetObject<CSellYuanBao>();
}

#endif //_SELLYUANBAO_H_
