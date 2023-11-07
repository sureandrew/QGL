// dboLogMix.h : Declaration of the CdboLogMix

#pragma once
#ifndef _DBO_LOG_MIX_H_
#define _DBO_LOG_MIX_H_
#include "Dbo.h"

class CdboLogMixAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogMixAccessor)

	DEFINE_ENTRY_STR(sess_id, 13)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY(process_type, INT)
	DEFINE_ENTRY(result_item_id, INT)
	DEFINE_ENTRY(result_item_count, INT)	
	DEFINE_ENTRY(source_item_uid1, LARGE_INTEGER)
	DEFINE_ENTRY(source_item_id1, INT)
	DEFINE_ENTRY(source_item_count1, INT)	
	DEFINE_ENTRY(source_item_uid2, LARGE_INTEGER)
	DEFINE_ENTRY(source_item_id2, INT)
	DEFINE_ENTRY(source_item_count2, INT)	
	DEFINE_ENTRY(source_item_uid3, LARGE_INTEGER)
	DEFINE_ENTRY(source_item_id3, INT)
	DEFINE_ENTRY(source_item_count3, INT)	
	DEFINE_ENTRY(source_item_uid4, LARGE_INTEGER)
	DEFINE_ENTRY(source_item_id4, INT)
	DEFINE_ENTRY(source_item_count4, INT)	
	DEFINE_ENTRY(source_item_uid5, LARGE_INTEGER)
	DEFINE_ENTRY(source_item_id5, INT)
	DEFINE_ENTRY(source_item_count5, INT)	
	DEFINE_ENTRY(source_item_uid6, LARGE_INTEGER)
	DEFINE_ENTRY(source_item_id6, INT)
	DEFINE_ENTRY(source_item_count6, INT)	
	DEFINE_ENTRY(source_item_uid7, LARGE_INTEGER)
	DEFINE_ENTRY(source_item_id7, INT)
	DEFINE_ENTRY(source_item_count7, INT)	
	DEFINE_ENTRY(source_item_uid8, LARGE_INTEGER)
	DEFINE_ENTRY(source_item_id8, INT)
	DEFINE_ENTRY(source_item_count8, INT)	

	
	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_sess_id)
		COLUMN_ENTRY(2, m_char_id)
		COLUMN_ENTRY(3, m_process_type)
		COLUMN_ENTRY(4, m_result_item_id)
		COLUMN_ENTRY(5, m_result_item_count)
		COLUMN_ENTRY(6, m_source_item_uid1)
		COLUMN_ENTRY(7, m_source_item_id1)
		COLUMN_ENTRY(8, m_source_item_count1)
		COLUMN_ENTRY(9, m_source_item_uid2)
		COLUMN_ENTRY(10, m_source_item_id2)
		COLUMN_ENTRY(11, m_source_item_count2)
		COLUMN_ENTRY(12, m_source_item_uid3)
		COLUMN_ENTRY(13, m_source_item_id3)
		COLUMN_ENTRY(14, m_source_item_count3)
		COLUMN_ENTRY(15, m_source_item_uid4)
		COLUMN_ENTRY(16, m_source_item_id4)
		COLUMN_ENTRY(17, m_source_item_count4)
		COLUMN_ENTRY(18, m_source_item_uid5)
		COLUMN_ENTRY(19, m_source_item_id5)
		COLUMN_ENTRY(20, m_source_item_count5)
		COLUMN_ENTRY(21, m_source_item_uid6)
		COLUMN_ENTRY(22, m_source_item_id6)
		COLUMN_ENTRY(23, m_source_item_count6)
		COLUMN_ENTRY(24, m_source_item_uid7)
		COLUMN_ENTRY(25, m_source_item_id7)
		COLUMN_ENTRY(26, m_source_item_count7)
		COLUMN_ENTRY(27, m_source_item_uid8)
		COLUMN_ENTRY(28, m_source_item_id8)
		COLUMN_ENTRY(29, m_source_item_count8)			
	END_PARAM_MAP_FUNC()
	
	DEFINE_COMMAND_EX(CdboLogExchangeAccessor, L"SELECT * FROM LogMix")

};

class CdboLogMix : public Cdbo<CCommand<CAccessor<CdboLogMixAccessor> > >
{
public:
	HRESULT Log(PCSTRING pSessId, UInt32 char_id, UInt8 process_type, UInt32 result_item_id, UInt8 result_item_count, UInt32Vector source_item_uid, UInt32Vector source_item_id, UInt8Vector source_item_count)
	{
		for (UInt i = (UInt)source_item_uid.size(); i < 8; ++i)
		{
			source_item_uid.push_back(0);
			source_item_id.push_back(0);
			source_item_count.push_back(0);
		}

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		COLUMN_STRING_COPY(m_sess_id, pSessId)
		
		m_char_id.QuadPart = char_id;
		m_process_type = process_type;
				
		m_result_item_id = result_item_id;
		m_result_item_count = result_item_count;		

		m_source_item_uid1.QuadPart = source_item_uid[0];
		m_source_item_id1 = source_item_id[0];
		m_source_item_count1 = source_item_count[0];

		m_source_item_uid2.QuadPart = source_item_uid[1];
		m_source_item_id2 = source_item_id[1];
		m_source_item_count2 = source_item_count[1];

		m_source_item_uid3.QuadPart = source_item_uid[2];
		m_source_item_id3 = source_item_id[2];
		m_source_item_count3 = source_item_count[2];

		m_source_item_uid4.QuadPart = source_item_uid[3];
		m_source_item_id4 = source_item_id[3];
		m_source_item_count4 = source_item_count[3];

		m_source_item_uid5.QuadPart = source_item_uid[4];
		m_source_item_id5 = source_item_id[4];
		m_source_item_count5 = source_item_count[4];

		m_source_item_uid6.QuadPart = source_item_uid[5];
		m_source_item_id6 = source_item_id[5];
		m_source_item_count6 = source_item_count[5];

		m_source_item_uid7.QuadPart = source_item_uid[6];
		m_source_item_id7 = source_item_id[6];
		m_source_item_count7 = source_item_count[6];

		m_source_item_uid8.QuadPart = source_item_uid[7];
		m_source_item_id8 = source_item_id[7];
		m_source_item_count8 = source_item_count[7];
		
		return OpenCommand(_T("INSERT INTO LogMix (sess_id, char_id, process_type, result_item_id, result_item_count, ")
					_T("source_item_uid1, source_item_id1, source_item_count1, source_item_uid2, source_item_id2, source_item_count2,")
					_T("source_item_uid3, source_item_id3, source_item_count3, source_item_uid4, source_item_id4, source_item_count4,")
					_T("source_item_uid5, source_item_id5, source_item_count5, source_item_uid6, source_item_id6, source_item_count6,")
					_T("source_item_uid7, source_item_id7, source_item_count7, source_item_uid8, source_item_id8, source_item_count8,")
					_T("create_time ) VALUES (")
					_T("?, ?, ?, ?, ?,")
					_T("?, ?, ?, ?, ?, ?,")
					_T("?, ?, ?, ?, ?, ?,")
					_T("?, ?, ?, ?, ?, ?,")
					_T("?, ?, ?, ?, ?, ?,")
					_T("GetDate()")
					_T(")"));
	}
};

#endif //_DBO_LOG_EXCHANGE_H_