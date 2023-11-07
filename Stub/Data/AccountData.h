#pragma once
#ifndef _ACCOUNTDATA_H_
#define _ACCOUNTDATA_H_

#include "Stub/Data/AccountData_Stub.h"

class CdboAccount;

class CAccountData : public AccountData_Stub
{
public:
	DEFINE_CLASS(AccountData_CLASSID);
	static CAccountData * NewInstance();
	void SetData(CdboAccount* dboAccount, bool inactive=false);
	void SetLoaded(Boolean bLoad) { m_bLoad = bLoad; }
	void AddCharId(UInt32 char_id);
	void RemoveCharId(UInt32 char_id);
	void KeepCharId(UInt32 char_id);
	void ReleaseCharId(UInt32 char_id);
	void ReleaseAllChar();
	NetGroup* GetCharGroup();
	Boolean IsCharacterIDExist(UInt32 char_id);	
	void GetDbCharId(LPCPEER pPeer, RPCContext &context);
	HRESULT UpdateDbo(CdboAccount* dboAcct);
	HRESULT DeleteDbo(CdboAccount* dboAcct);

	CAccountData();
	virtual ~CAccountData();

private:
	typedef AccountData_Stub Parent;
	typedef StlSet<UInt32, std::less<UInt32>, ManagedAllocator<UInt32> >
		CharIdSet;

	Boolean			m_bLoad;
	CharIdSet		m_charIds;
	NetGroup		m_charGroup;
	UInt32			m_keepCharId;

	static ThreadKey m_key;					// safe thread key for multiple db access
};

CAccountData* FindAccount(UInt32 acct_id);
inline CAccountData * CAccountData::NewInstance()
{
	return CreateNetObject<CAccountData>();
}

#endif //_ACCOUNTDATA_H_
