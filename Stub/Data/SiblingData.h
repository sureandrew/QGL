#pragma once
#ifndef _SIBLINGDATA_H_
#define _SIBLINGDATA_H_

#include "Stub/Data/SiblingData_Stub.h"
class CdboSibling;
class CSiblingData : public SiblingData_Stub
{
public:

	DEFINE_CLASS(SiblingData_CLASSID);

	static CSiblingData * NewInstance();
	void SetData(CdboSibling* dboSibling);
	void PreSend();
	HRESULT UpdateDbo(CdboSibling* dboSibling);
	HRESULT DeleteDbo(CdboSibling* dboSibling);


	static HRESULT DoCreateSibling(const String &sibling_title_prefix, const UInt32 &member_id1
		, const String &member_title_suffix1, const UInt32 &member_id2
		, const String &member_title_suffix2, const UInt32 &member_id3
		, const String &member_title_suffix3, const UInt32 &member_id4
		, const String &member_title_suffix4, const UInt32 &member_id5
		, const String &member_title_suffix5, const UInt32 &member_id6
		, const String &member_title_suffix6, const UInt32 &member_id7
		, const String &member_title_suffix7, const UInt32 &member_id8
		, const String &member_title_suffix8, const UInt32 &member_id9
		, const String &member_title_suffix9, const UInt32 &member_id10
		, const String &member_title_suffix10,CSiblingData* &pSiblingData);
	static DWORD WINAPI QueueGetSibling(LPVOID lpParam);
	static CSiblingData* FindSibling(UInt32 sibling_id);
	static HRESULT RemoveSibling(UInt32 sibling_id);
	static CSiblingData* NewSibling();
	static void ReleaseSibling(UInt32 char_id);

private:
	typedef SiblingData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access

};

inline CSiblingData * CSiblingData::NewInstance()
{
	return CreateNetObject<CSiblingData>();
}

#endif //_SIBLINGDATA_H_
