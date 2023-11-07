// dboItem.h : Declaration of the CdboItem

#pragma once
#ifndef _DBO_ITEM_H_
#define _DBO_ITEM_H_
#include "Dbo.h"

// code generated on Thursday, March 22, 2007, 11:14 AM

class CdboItemAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboItemAccessor)

	DEFINE_ENTRY(item_uid, LARGE_INTEGER)
	DEFINE_ENTRY(item_id, LARGE_INTEGER)
	DEFINE_ENTRY(owner_id, LARGE_INTEGER)
	DEFINE_ENTRY(bagIndex, INT)
	DEFINE_ENTRY(eq_element, LONG)
	DEFINE_ENTRY(endure, LONG)
	DEFINE_ENTRY(mEndure, LONG)
	DEFINE_ENTRY_STR(maker, 21)
	DEFINE_ENTRY(grade, INT)
	DEFINE_ENTRY(upg_exp, LONG)
	DEFINE_ENTRY(remain, INT)
	DEFINE_ENTRY(stack, INT)
	DEFINE_ENTRY(base_STA, LONG)
	DEFINE_ENTRY(base_SPR, LONG)
	DEFINE_ENTRY(base_STR, LONG)
	DEFINE_ENTRY(base_CON, LONG)
	DEFINE_ENTRY(base_AGI, LONG)
	DEFINE_ENTRY(base_ATK, LONG)
	DEFINE_ENTRY(base_DEF, LONG)
	DEFINE_ENTRY(base_HIT, LONG)
	DEFINE_ENTRY(base_EVA, LONG)
	DEFINE_ENTRY(base_POW, LONG)
	DEFINE_ENTRY(base_SPD, LONG)
	DEFINE_ENTRY(base_maxHP, LONG)
	DEFINE_ENTRY(base_maxSP, LONG)
	DEFINE_ENTRY(base_maxVP, LONG)
	DEFINE_ENTRY(upg_STA, LONG)
	DEFINE_ENTRY(upg_SPR, LONG)
	DEFINE_ENTRY(upg_STR, LONG)
	DEFINE_ENTRY(upg_CON, LONG)
	DEFINE_ENTRY(upg_AGI, LONG)
	DEFINE_ENTRY(upg_ATK, LONG)
	DEFINE_ENTRY(upg_DEF, LONG)
	DEFINE_ENTRY(upg_HIT, LONG)
	DEFINE_ENTRY(upg_EVA, LONG)
	DEFINE_ENTRY(upg_POW, LONG)
	DEFINE_ENTRY(upg_SPD, LONG)
	DEFINE_ENTRY(upg_maxHP, LONG)
	DEFINE_ENTRY(upg_maxSP, LONG)
	DEFINE_ENTRY(upg_maxVP, LONG)
	DEFINE_ENTRY(eq_gem, INT)
	DEFINE_ENTRY(inlay_gem0, LONG)
	DEFINE_ENTRY(inlay_gem1, LONG)
	DEFINE_ENTRY(inlay_gem2, LONG)
	DEFINE_ENTRY(inlay_gem3, LONG)
	DEFINE_ENTRY(inlay_gem4, LONG)
	DEFINE_ENTRY(inlay_gem5, LONG)
	DEFINE_ENTRY(inlay_gem6, LONG)
	DEFINE_ENTRY(inlay_gem7, LONG)
	DEFINE_ENTRY(inlay_gem8, LONG)
	DEFINE_ENTRY(inlay_gem9, LONG)
	DEFINE_ENTRY(inlay_STA, LONG)
	DEFINE_ENTRY(inlay_SPR, LONG)
	DEFINE_ENTRY(inlay_STR, LONG)
	DEFINE_ENTRY(inlay_CON, LONG)
	DEFINE_ENTRY(inlay_AGI, LONG)
	DEFINE_ENTRY(inlay_ATK, LONG)
	DEFINE_ENTRY(inlay_DEF, LONG)
	DEFINE_ENTRY(inlay_HIT, LONG)
	DEFINE_ENTRY(inlay_EVA, LONG)
	DEFINE_ENTRY(inlay_POW, LONG)
	DEFINE_ENTRY(inlay_SPD, LONG)
	DEFINE_ENTRY(inlay_maxHP, LONG)
	DEFINE_ENTRY(inlay_maxSP, LONG)
	DEFINE_ENTRY(inlay_maxVP, LONG)
	DEFINE_ENTRY(addon, LONG)
	DEFINE_ENTRY(eqColor, LONG)
	DEFINE_ENTRY(quality, LONG)
	DEFINE_ENTRY(soulboundState, INT)
	DEFINE_ENTRY(IdentifyState, INT)
	DEFINE_ENTRY(ExpireDate, DBTIMESTAMP)
	DEFINE_ENTRY(Value1, LONG)
	DEFINE_ENTRY(Value2, LONG)
	DEFINE_ENTRY(Value3, LONG)
	DEFINE_ENTRY(buff_id, LONG)
	DEFINE_ENTRY(buff_Value1, LARGE_INTEGER)
	DEFINE_ENTRY(buff_Value2, LARGE_INTEGER)
	DEFINE_ENTRY(buff_Value3, LARGE_INTEGER)
	DEFINE_ENTRY(buff_CoolDown, LARGE_INTEGER)
	DEFINE_ENTRY(buff_Pro, INT)
	DEFINE_ENTRY(inWareHouse, INT)
	DEFINE_ENTRY(lock, INT)
	DEFINE_ENTRY(skill_id, LONG)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
//	ENABLE_COLUMN_MAP(CdboItemAccessor)
//	ENABLE_PARAM_MAP(CdboItemAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		COLUMN_ENTRY_LS( 1, m_item_uid)
		COLUMN_ENTRY_LS( 2, m_item_id)
		COLUMN_ENTRY_LS( 3, m_owner_id)
		COLUMN_ENTRY_LS( 4, m_bagIndex)
		COLUMN_ENTRY_LS( 5, m_eq_element)
		COLUMN_ENTRY_LS( 6, m_endure)
		COLUMN_ENTRY_LS( 7, m_mEndure)
		COLUMN_ENTRY_LS( 8, m_maker)
		COLUMN_ENTRY_LS( 9, m_grade)
		COLUMN_ENTRY_LS(10, m_upg_exp)
		COLUMN_ENTRY_LS(11, m_remain)
		COLUMN_ENTRY_LS(12, m_stack)
		COLUMN_ENTRY_LS(13, m_base_STA)
		COLUMN_ENTRY_LS(14, m_base_SPR)
		COLUMN_ENTRY_LS(15, m_base_STR)
		COLUMN_ENTRY_LS(16, m_base_CON)
		COLUMN_ENTRY_LS(17, m_base_AGI)
		COLUMN_ENTRY_LS(18, m_base_ATK)
		COLUMN_ENTRY_LS(19, m_base_DEF)
		COLUMN_ENTRY_LS(20, m_base_HIT)
		COLUMN_ENTRY_LS(21, m_base_EVA)
		COLUMN_ENTRY_LS(22, m_base_POW)
		COLUMN_ENTRY_LS(23, m_base_SPD)
		COLUMN_ENTRY_LS(24, m_base_maxHP)
		COLUMN_ENTRY_LS(25, m_base_maxSP)
		COLUMN_ENTRY_LS(26, m_base_maxVP)
		COLUMN_ENTRY_LS(27, m_upg_STA)
		COLUMN_ENTRY_LS(28, m_upg_SPR)
		COLUMN_ENTRY_LS(29, m_upg_STR)
		COLUMN_ENTRY_LS(30, m_upg_CON)
		COLUMN_ENTRY_LS(31, m_upg_AGI)
		COLUMN_ENTRY_LS(32, m_upg_ATK)
		COLUMN_ENTRY_LS(33, m_upg_DEF)
		COLUMN_ENTRY_LS(34, m_upg_HIT)
		COLUMN_ENTRY_LS(35, m_upg_EVA)
		COLUMN_ENTRY_LS(36, m_upg_POW)
		COLUMN_ENTRY_LS(37, m_upg_SPD)
		COLUMN_ENTRY_LS(38, m_upg_maxHP)
		COLUMN_ENTRY_LS(39, m_upg_maxSP)
		COLUMN_ENTRY_LS(40, m_upg_maxVP)
		COLUMN_ENTRY_LS(41, m_eq_gem)
		COLUMN_ENTRY_LS(42, m_inlay_gem0)
		COLUMN_ENTRY_LS(43, m_inlay_gem1)
		COLUMN_ENTRY_LS(44, m_inlay_gem2)
		COLUMN_ENTRY_LS(45, m_inlay_gem3)
		COLUMN_ENTRY_LS(46, m_inlay_gem4)
		COLUMN_ENTRY_LS(47, m_inlay_gem5)
		COLUMN_ENTRY_LS(48, m_inlay_gem6)
		COLUMN_ENTRY_LS(49, m_inlay_gem7)
		COLUMN_ENTRY_LS(50, m_inlay_gem8)
		COLUMN_ENTRY_LS(51, m_inlay_gem9)
		COLUMN_ENTRY_LS(52, m_inlay_STA)
		COLUMN_ENTRY_LS(53, m_inlay_SPR)
		COLUMN_ENTRY_LS(54, m_inlay_STR)
		COLUMN_ENTRY_LS(55, m_inlay_CON)
		COLUMN_ENTRY_LS(56, m_inlay_AGI)
		COLUMN_ENTRY_LS(57, m_inlay_ATK)
		COLUMN_ENTRY_LS(58, m_inlay_DEF)
		COLUMN_ENTRY_LS(59, m_inlay_HIT)
		COLUMN_ENTRY_LS(60, m_inlay_EVA)
		COLUMN_ENTRY_LS(61, m_inlay_POW)
		COLUMN_ENTRY_LS(62, m_inlay_SPD)
		COLUMN_ENTRY_LS(63, m_inlay_maxHP)
		COLUMN_ENTRY_LS(64, m_inlay_maxSP)
		COLUMN_ENTRY_LS(65, m_inlay_maxVP)
		COLUMN_ENTRY_LS(66, m_addon)
		COLUMN_ENTRY_LS(67, m_eqColor)
		COLUMN_ENTRY_LS(68, m_quality)
		COLUMN_ENTRY_LS(69, m_soulboundState)
		COLUMN_ENTRY_LS(70, m_IdentifyState)
		COLUMN_ENTRY_LS(71, m_ExpireDate)
		COLUMN_ENTRY_LS(72, m_Value1)
		COLUMN_ENTRY_LS(73, m_Value2)
		COLUMN_ENTRY_LS(74, m_Value3)
		COLUMN_ENTRY_LS(75, m_buff_id)
		COLUMN_ENTRY_LS(76, m_buff_Value1)
		COLUMN_ENTRY_LS(77, m_buff_Value2)
		COLUMN_ENTRY_LS(78, m_buff_Value3)
		COLUMN_ENTRY_LS(79, m_buff_CoolDown)
		COLUMN_ENTRY_LS(80, m_buff_Pro)
		COLUMN_ENTRY_LS(81, m_inWareHouse)
		COLUMN_ENTRY_LS(82, m_lock)	
		COLUMN_ENTRY_LS(83, m_skill_id)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetIdentity)
		COLUMN_ENTRY_LS(1, m_item_uid)
	END_COLUMN_MAP()


	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, item_uid)
		COLUMN_ENTRY_TEST_ELSE(item_id)
		COLUMN_ENTRY_TEST_ELSE(owner_id)
		COLUMN_ENTRY_TEST_ELSE(bagIndex)
		COLUMN_ENTRY_TEST_ELSE(eq_element)
		COLUMN_ENTRY_TEST_ELSE(endure)
		COLUMN_ENTRY_TEST_ELSE(mEndure)
		COLUMN_ENTRY_TEST_ELSE(maker)
		COLUMN_ENTRY_TEST_ELSE(grade)
		COLUMN_ENTRY_TEST_ELSE(upg_exp)
		COLUMN_ENTRY_TEST_ELSE(remain)
		COLUMN_ENTRY_TEST_ELSE(stack)
		COLUMN_ENTRY_TEST_ELSE(base_STA)
		COLUMN_ENTRY_TEST_ELSE(base_SPR)
		COLUMN_ENTRY_TEST_ELSE(base_STR)
		COLUMN_ENTRY_TEST_ELSE(base_CON)
		COLUMN_ENTRY_TEST_ELSE(base_AGI)
		COLUMN_ENTRY_TEST_ELSE(base_ATK)
		COLUMN_ENTRY_TEST_ELSE(base_DEF)
		COLUMN_ENTRY_TEST_ELSE(base_HIT)
		COLUMN_ENTRY_TEST_ELSE(base_EVA)
		COLUMN_ENTRY_TEST_ELSE(base_POW)
		COLUMN_ENTRY_TEST_ELSE(base_SPD)
		COLUMN_ENTRY_TEST_ELSE(base_maxHP)
		COLUMN_ENTRY_TEST_ELSE(base_maxSP)
		COLUMN_ENTRY_TEST_ELSE(base_maxVP)
		COLUMN_ENTRY_TEST_ELSE(upg_STA)
		COLUMN_ENTRY_TEST_ELSE(upg_SPR)
		COLUMN_ENTRY_TEST_ELSE(upg_STR)
		COLUMN_ENTRY_TEST_ELSE(upg_CON)
		COLUMN_ENTRY_TEST_ELSE(upg_AGI)
		COLUMN_ENTRY_TEST_ELSE(upg_ATK)
		COLUMN_ENTRY_TEST_ELSE(upg_DEF)
		COLUMN_ENTRY_TEST_ELSE(upg_HIT)
		COLUMN_ENTRY_TEST_ELSE(upg_EVA)
		COLUMN_ENTRY_TEST_ELSE(upg_POW)
		COLUMN_ENTRY_TEST_ELSE(upg_SPD)
		COLUMN_ENTRY_TEST_ELSE(upg_maxHP)
		COLUMN_ENTRY_TEST_ELSE(upg_maxSP)
		COLUMN_ENTRY_TEST_ELSE(upg_maxVP)
		COLUMN_ENTRY_TEST_ELSE(eq_gem)
		COLUMN_ENTRY_TEST_ELSE(inlay_gem0)
		COLUMN_ENTRY_TEST_ELSE(inlay_gem1)
		COLUMN_ENTRY_TEST_ELSE(inlay_gem2)
		COLUMN_ENTRY_TEST_ELSE(inlay_gem3)
		COLUMN_ENTRY_TEST_ELSE(inlay_gem4)
		COLUMN_ENTRY_TEST_ELSE(inlay_gem5)
		COLUMN_ENTRY_TEST_ELSE(inlay_gem6)
		COLUMN_ENTRY_TEST_ELSE(inlay_gem7)
		COLUMN_ENTRY_TEST_ELSE(inlay_gem8)
		COLUMN_ENTRY_TEST_ELSE(inlay_gem9)
		COLUMN_ENTRY_TEST_ELSE(inlay_STA)
		COLUMN_ENTRY_TEST_ELSE(inlay_SPR)
		COLUMN_ENTRY_TEST_ELSE(inlay_STR)
		COLUMN_ENTRY_TEST_ELSE(inlay_CON)
		COLUMN_ENTRY_TEST_ELSE(inlay_AGI)
		COLUMN_ENTRY_TEST_ELSE(inlay_ATK)
		COLUMN_ENTRY_TEST_ELSE(inlay_DEF)
		COLUMN_ENTRY_TEST_ELSE(inlay_HIT)
		COLUMN_ENTRY_TEST_ELSE(inlay_EVA)
		COLUMN_ENTRY_TEST_ELSE(inlay_POW)
		COLUMN_ENTRY_TEST_ELSE(inlay_SPD)
		COLUMN_ENTRY_TEST_ELSE(inlay_maxHP)
		COLUMN_ENTRY_TEST_ELSE(inlay_maxSP)
		COLUMN_ENTRY_TEST_ELSE(inlay_maxVP)
		COLUMN_ENTRY_TEST_ELSE(addon)
		COLUMN_ENTRY_TEST_ELSE(eqColor)
		COLUMN_ENTRY_TEST_ELSE(quality)
		COLUMN_ENTRY_TEST_ELSE(soulboundState)
		COLUMN_ENTRY_TEST_ELSE(IdentifyState)
		COLUMN_ENTRY_TEST_ELSE(ExpireDate)
		COLUMN_ENTRY_TEST_ELSE(Value1)
		COLUMN_ENTRY_TEST_ELSE(Value2)
		COLUMN_ENTRY_TEST_ELSE(Value3)
		COLUMN_ENTRY_TEST_ELSE(buff_id)
		COLUMN_ENTRY_TEST_ELSE(buff_Value1)
		COLUMN_ENTRY_TEST_ELSE(buff_Value2)
		COLUMN_ENTRY_TEST_ELSE(buff_Value3)
		COLUMN_ENTRY_TEST_ELSE(buff_CoolDown)
		COLUMN_ENTRY_TEST_ELSE(buff_Pro)
		COLUMN_ENTRY_TEST_ELSE(inWareHouse)
		COLUMN_ENTRY_TEST_ELSE(lock)	
		COLUMN_ENTRY_TEST_ELSE(skill_id)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByUID)
		COLUMN_ENTRY(1, m_item_uid)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_GetByCID)
		COLUMN_ENTRY(1, m_owner_id)
		COLUMN_ENTRY(2, m_inWareHouse)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_item_id)
		COLUMN_ENTRY(2, m_owner_id)
		COLUMN_ENTRY(3, m_bagIndex)
		COLUMN_ENTRY(4, m_eq_element)
		COLUMN_ENTRY(5, m_remain)
		COLUMN_ENTRY(6, m_stack)
		COLUMN_ENTRY(7, m_soulboundState)
		COLUMN_ENTRY(8, m_IdentifyState)
		COLUMN_ENTRY(9, m_maker)
		COLUMN_ENTRY(10, m_inWareHouse)
		COLUMN_ENTRY(11, m_quality)
		COLUMN_ENTRY(12, m_lock)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_InsertWithTimeLimit)
		COLUMN_ENTRY(1, m_item_id)
		COLUMN_ENTRY(2, m_owner_id)
		COLUMN_ENTRY(3, m_bagIndex)
		COLUMN_ENTRY(4, m_eq_element)
		COLUMN_ENTRY(5, m_remain)
		COLUMN_ENTRY(6, m_stack)
		COLUMN_ENTRY(7, m_soulboundState)
		COLUMN_ENTRY(8, m_IdentifyState)
		COLUMN_ENTRY(9, m_maker)
		COLUMN_ENTRY(10, m_ExpireDate)
		COLUMN_ENTRY(11, m_inWareHouse)
		COLUMN_ENTRY(12, m_quality)
		COLUMN_ENTRY(13, m_lock)	
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_InsertEq)
		COLUMN_ENTRY(1, m_item_id)
		COLUMN_ENTRY(2, m_owner_id)
		COLUMN_ENTRY(3, m_bagIndex)
		COLUMN_ENTRY(4, m_eq_element)
		COLUMN_ENTRY(5, m_endure)
		COLUMN_ENTRY(6, m_mEndure)
		COLUMN_ENTRY(7, m_base_STA)
		COLUMN_ENTRY(8, m_base_SPR)
		COLUMN_ENTRY(9, m_base_STR)
		COLUMN_ENTRY(10, m_base_CON)
		COLUMN_ENTRY(11, m_base_AGI)
		COLUMN_ENTRY(12, m_base_ATK)
		COLUMN_ENTRY(13, m_base_DEF)
		COLUMN_ENTRY(14, m_base_HIT)
		COLUMN_ENTRY(15, m_base_EVA)
		COLUMN_ENTRY(16, m_base_POW)
		COLUMN_ENTRY(17, m_base_SPD)
		COLUMN_ENTRY(18, m_base_maxHP)
		COLUMN_ENTRY(19, m_base_maxSP)
		COLUMN_ENTRY(20, m_base_maxVP)
		COLUMN_ENTRY(21, m_soulboundState)
		COLUMN_ENTRY(22, m_IdentifyState)
		COLUMN_ENTRY(23, m_maker)
		COLUMN_ENTRY(24, m_inWareHouse)
		COLUMN_ENTRY(25, m_eqColor)
		COLUMN_ENTRY(26, m_quality)
		COLUMN_ENTRY(27, m_skill_id)
		COLUMN_ENTRY(28, m_lock)	

	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_InsertEqWithTimeLimit)
		COLUMN_ENTRY(1, m_item_id)
		COLUMN_ENTRY(2, m_owner_id)
		COLUMN_ENTRY(3, m_bagIndex)
		COLUMN_ENTRY(4, m_eq_element)
		COLUMN_ENTRY(5, m_endure)
		COLUMN_ENTRY(6, m_mEndure)
		COLUMN_ENTRY(7, m_base_STA)
		COLUMN_ENTRY(8, m_base_SPR)
		COLUMN_ENTRY(9, m_base_STR)
		COLUMN_ENTRY(10, m_base_CON)
		COLUMN_ENTRY(11, m_base_AGI)
		COLUMN_ENTRY(12, m_base_ATK)
		COLUMN_ENTRY(13, m_base_DEF)
		COLUMN_ENTRY(14, m_base_HIT)
		COLUMN_ENTRY(15, m_base_EVA)
		COLUMN_ENTRY(16, m_base_POW)
		COLUMN_ENTRY(17, m_base_SPD)
		COLUMN_ENTRY(18, m_base_maxHP)
		COLUMN_ENTRY(19, m_base_maxSP)
		COLUMN_ENTRY(20, m_base_maxVP)
		COLUMN_ENTRY(21, m_soulboundState)
		COLUMN_ENTRY(22, m_IdentifyState)
		COLUMN_ENTRY(23, m_maker)
		COLUMN_ENTRY(24, m_ExpireDate)
		COLUMN_ENTRY(25, m_inWareHouse)
		COLUMN_ENTRY(26, m_eqColor)
		COLUMN_ENTRY(27, m_quality)
		COLUMN_ENTRY(28, m_skill_id)
		COLUMN_ENTRY(29, m_lock)

	END_PARAM_MAP_FUNC()

/*
	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY( 1, m_item_id)
		COLUMN_ENTRY( 2, m_owner_id)
		COLUMN_ENTRY( 3, m_bagIndex)
		COLUMN_ENTRY( 4, m_remain)
		COLUMN_ENTRY( 5, m_stack)
		COLUMN_ENTRY( 6, m_eq_element)
		COLUMN_ENTRY( 7, m_endure)
		COLUMN_ENTRY( 8, m_mEndure)
		COLUMN_ENTRY( 9, m_base_STA)
		COLUMN_ENTRY(10, m_base_SPR)
		COLUMN_ENTRY(11, m_base_STR)
		COLUMN_ENTRY(12, m_base_CON)
		COLUMN_ENTRY(13, m_base_AGI)
		COLUMN_ENTRY(14, m_base_ATK)
		COLUMN_ENTRY(15, m_base_DEF)
		COLUMN_ENTRY(16, m_base_HIT)
		COLUMN_ENTRY(17, m_base_EVA)
		COLUMN_ENTRY(18, m_base_POW)
		COLUMN_ENTRY(19, m_base_SPD)
		COLUMN_ENTRY(20, m_base_maxHP)
		COLUMN_ENTRY(21, m_base_maxSP)
		COLUMN_ENTRY(22, m_base_maxVP)
		COLUMN_ENTRY(23, m_soulboundState)
		COLUMN_ENTRY(24, m_IdentifyState)
		COLUMN_ENTRY(25, m_maker)
		COLUMN_ENTRY(26, m_inWareHouse)
		COLUMN_ENTRY(27, m_eqColor)
		COLUMN_ENTRY(28, m_quality)
		COLUMN_ENTRY(29, m_skill_id)
		COLUMN_ENTRY(30, m_lock)
	END_PARAM_MAP_FUNC()


	BEGIN_PARAM_MAP_FUNC(_InsertExpire)
		COLUMN_ENTRY( 1, m_item_id)
		COLUMN_ENTRY( 2, m_owner_id)
		COLUMN_ENTRY( 3, m_bagIndex)
		COLUMN_ENTRY( 4, m_remain)
		COLUMN_ENTRY( 5, m_stack)
		COLUMN_ENTRY( 6, m_eq_element)
		COLUMN_ENTRY( 7, m_endure)
		COLUMN_ENTRY( 8, m_mEndure)
		COLUMN_ENTRY( 9, m_base_STA)
		COLUMN_ENTRY(10, m_base_SPR)
		COLUMN_ENTRY(11, m_base_STR)
		COLUMN_ENTRY(12, m_base_CON)
		COLUMN_ENTRY(13, m_base_AGI)
		COLUMN_ENTRY(14, m_base_ATK)
		COLUMN_ENTRY(15, m_base_DEF)
		COLUMN_ENTRY(16, m_base_HIT)
		COLUMN_ENTRY(17, m_base_EVA)
		COLUMN_ENTRY(18, m_base_POW)
		COLUMN_ENTRY(19, m_base_SPD)
		COLUMN_ENTRY(20, m_base_maxHP)
		COLUMN_ENTRY(21, m_base_maxSP)
		COLUMN_ENTRY(22, m_base_maxVP)
		COLUMN_ENTRY(23, m_soulboundState)
		COLUMN_ENTRY(24, m_IdentifyState)
		COLUMN_ENTRY(25, m_maker)
		COLUMN_ENTRY(26, m_inWareHouse)
		COLUMN_ENTRY(27, m_eqColor)
		COLUMN_ENTRY(28, m_quality)
		COLUMN_ENTRY(29, m_skill_id)
		COLUMN_ENTRY(30, m_lock)
		COLUMN_ENTRY(31, m_ExpireDate)
	END_PARAM_MAP_FUNC()
*/

	BEGIN_PARAM_MAP_FUNC(_Delete)
		COLUMN_ENTRY(1, m_item_uid)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ResetLock)
		COLUMN_ENTRY(1, m_lock)	
		COLUMN_ENTRY(2, m_item_uid)
	END_PARAM_MAP_FUNC()

	DEFINE_COMMAND_EX(CdboItemAccessor, L"SELECT * FROM Item");

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboItem : public Cdbo<CCommand<CAccessor<CdboItemAccessor> > >
{
public:

	HRESULT Insert(DWORD item_id, DWORD owner_id, int bagIndex, int element, int useCount, int stack, int soulboundState, int IdentifyState, const String& maker, int inWareHouse, int quality, Boolean lock)
	{
		ClearRecord();
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);
		m_item_id.QuadPart = item_id;
		m_owner_id.QuadPart = owner_id;
		m_bagIndex = bagIndex;
		m_eq_element = element;
		m_remain = useCount;
		m_stack = stack;
		m_soulboundState = soulboundState;
		m_IdentifyState = IdentifyState;
		m_inWareHouse = inWareHouse;
		m_quality = quality;
		m_lock = lock? 1:0;
		COLUMN_STRING_COPY(m_maker, maker.c_str() );
		return OpenCommand(_T("INSERT INTO Item (item_id, owner_id, bagIndex, eq_element, remain, stack, soulboundState, IdentifyState, maker, inWareHouse, quality, lock) ")
								_T("OUTPUT INSERTED.* VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
	}

	HRESULT InsertWithTimeLimit(DWORD item_id, DWORD owner_id, int bagIndex, int element, int useCount, int stack, int soulboundState, int IdentifyState, const String& maker, DBTIMESTAMP dbtime, int inWareHouse, int quality, Boolean lock)
	{
		ClearRecord();
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_InsertWithTimeLimit);
		m_item_id.QuadPart = item_id;
		m_owner_id.QuadPart = owner_id;
		m_bagIndex = bagIndex;
		m_eq_element = element;
		m_remain = useCount;
		m_stack = stack;
		m_soulboundState = soulboundState;
		m_IdentifyState = IdentifyState;
		COLUMN_STRING_COPY(m_maker, maker.c_str() );
		m_ExpireDate = dbtime;
		m_inWareHouse = inWareHouse;
		m_quality = quality;
		m_lock = lock? 1:0;
		return OpenCommand(_T("INSERT INTO Item (item_id, owner_id, bagIndex, eq_element, remain, stack, soulboundState, IdentifyState, maker, ExpireDate, inWareHouse, quality, lock) ")
								_T("OUTPUT INSERTED.* VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
	}

	HRESULT InsertEq(DWORD item_id, DWORD owner_id, int bagIndex, int element, int endure, 
					int STA, int SPR, int STR, int CON, int AGI, int ATK, int DEF, int HIT,
					int EVA, int POW, int SPD, int maxHP, int maxSP, int maxVP, int soulboundState, 
					int IdentifyState, const String& maker, int inWareHouse, int eqColor, int quality, int skill_id, Boolean lock)
	{
		ClearRecord();
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_InsertEq);
		m_item_id.QuadPart = item_id;
		m_owner_id.QuadPart = owner_id;
		m_bagIndex = bagIndex;
		m_eq_element = element;
		m_endure = endure;
		m_mEndure = endure;
		m_base_STA = STA;
		m_base_SPR = SPR;
		m_base_STR = STR;
		m_base_CON = CON;
		m_base_AGI = AGI;
		m_base_ATK = ATK;
		m_base_DEF = DEF;
		m_base_HIT = HIT;
		m_base_EVA = EVA;
		m_base_POW = POW;
		m_base_SPD = SPD;
		m_base_maxHP = maxHP;
		m_base_maxSP = maxSP;
		m_base_maxVP = maxVP;
		m_soulboundState = soulboundState;
		m_IdentifyState = IdentifyState;
		COLUMN_STRING_COPY(m_maker, maker.c_str() );
		m_inWareHouse = inWareHouse;
		m_eqColor = eqColor;
		m_quality = quality;
		m_skill_id = skill_id;
		m_lock = lock? 1:0; 
		

		return OpenCommand(_T("INSERT INTO Item (item_id, owner_id, bagIndex, eq_element, endure, mEndure, ")
								_T("base_STA, base_SPR, base_STR, base_CON, base_AGI, base_ATK, base_DEF, ")
								_T("base_HIT, base_EVA, base_POW, base_SPD, base_maxHP, base_maxSP, base_maxVP, ")
								_T("soulboundState, IdentifyState, maker, inWareHouse, eqColor, quality, skill_id, lock) ")
								_T("OUTPUT INSERTED.* VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
	}

	HRESULT InsertEqWithTimeLimit(DWORD item_id, DWORD owner_id, int bagIndex, int element, int endure, 
					int STA, int SPR, int STR, int CON, int AGI, int ATK, int DEF, int HIT,
					int EVA, int POW, int SPD, int maxHP, int maxSP, int maxVP, int soulboundState, 
					int IdentifyState, const String& maker, DBTIMESTAMP dbtime, int inWareHouse, int eqColor, int quality, int skill_id, Boolean lock)

	{
		ClearRecord();
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_InsertEqWithTimeLimit);
		m_item_id.QuadPart = item_id;
		m_owner_id.QuadPart = owner_id;
		m_bagIndex = bagIndex;
		m_eq_element = element;
		m_endure = endure;
		m_mEndure = endure;
		m_base_STA = STA;
		m_base_SPR = SPR;
		m_base_STR = STR;
		m_base_CON = CON;
		m_base_AGI = AGI;
		m_base_ATK = ATK;
		m_base_DEF = DEF;
		m_base_HIT = HIT;
		m_base_EVA = EVA;
		m_base_POW = POW;
		m_base_SPD = SPD;
		m_base_maxHP = maxHP;
		m_base_maxSP = maxSP;
		m_base_maxVP = maxVP;
		m_soulboundState = soulboundState;
		m_IdentifyState = IdentifyState;
		m_ExpireDate = dbtime;
		COLUMN_STRING_COPY(m_maker, maker.c_str() );
		m_inWareHouse = inWareHouse;
		m_eqColor = eqColor;
		m_quality = quality;
		m_skill_id = skill_id;
		m_lock = lock? 1:0;

		return OpenCommand(_T("INSERT INTO Item (item_id, owner_id, bagIndex, eq_element, endure, mEndure, ")
								_T("base_STA, base_SPR, base_STR, base_CON, base_AGI, base_ATK, base_DEF, ")
								_T("base_HIT, base_EVA, base_POW, base_SPD, base_maxHP, base_maxSP, base_maxVP, ")
								_T("soulboundState, IdentifyState, maker, ExpireDate, inWareHouse, eqColor, quality, skill_id, lock) ")
								_T("OUTPUT INSERTED.* VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
	}

	/*
	HRESULT Insert(DWORD item_id, DWORD owner_id, UInt16 bagIndex, UInt8 stack, UInt8 useCount,
		UInt16 element, UInt16 endure, UInt16 STA, UInt16 SPR, UInt16 STR, UInt16 CON, 
		UInt16 AGI, UInt16 ATK, UInt16 DEF, UInt16 HIT, UInt16 EVA, UInt16 POW, 
		UInt16 SPD, UInt16 maxHP, UInt16 maxSP, UInt16 maxVP, UInt8 soulboundState, 
		UInt8 IdentifyState, const String& maker, UInt16 inWareHouse, UInt16 eqColor, 
		UInt16 quality, UInt16 skill_id, Boolean lock, DBTIMESTAMP* expire)
	{
		ClearRecord();
		m_item_id.QuadPart = item_id;
		m_owner_id.QuadPart = owner_id;
		m_bagIndex = bagIndex;
		m_remain = useCount;
		m_stack = stack;
		m_eq_element = element;
		m_endure = endure;
		m_mEndure = endure;
		m_base_STA = STA;
		m_base_SPR = SPR;
		m_base_STR = STR;
		m_base_CON = CON;
		m_base_AGI = AGI;
		m_base_ATK = ATK;
		m_base_DEF = DEF;
		m_base_HIT = HIT;
		m_base_EVA = EVA;
		m_base_POW = POW;
		m_base_SPD = SPD;
		m_base_maxHP = maxHP;
		m_base_maxSP = maxSP;
		m_base_maxVP = maxVP;
		m_soulboundState = soulboundState;
		m_IdentifyState = IdentifyState;
		COLUMN_STRING_COPY(m_maker, maker.c_str())
		m_inWareHouse = inWareHouse;
		m_eqColor = eqColor;
		m_quality = quality;
		m_skill_id = skill_id;
		m_lock = lock? 1:0;

		String fields = 
			_T("item_id, owner_id, bagIndex, remain, stack, eq_element, endure, mEndure, ")
			_T("base_STA, base_SPR, base_STR, base_CON, base_AGI, base_ATK, base_DEF, ")
			_T("base_HIT, base_EVA, base_POW, base_SPD, base_maxHP, base_maxSP, base_maxVP, ")
			_T("soulboundState, IdentifyState, maker, inWareHouse, eqColor, quality, ")
			_T("skill_id, lock");
		String values = _T("?, ?, ?, ?, ?, ?, ?, ?, ")
			_T("?, ?, ?, ?, ?, ?, ?, ")
			_T("?, ?, ?, ?, ?, ?, ?, ")
			_T("?, ?, ?, ?, ?, ?, ")
			_T("?, ?");
		String sql;

		SetGetBindFunc(_GetAll);
		//SetGetBindFunc(NULL);
		if (expire != NULL)
		{
			SetGetParamsFunc(_InsertExpire);
			fields += _T(", ExpireDate");
			values += _T(", ?");
			m_ExpireDate = *expire;
		}
		else
		{
			SetGetParamsFunc(_Insert);
		}
		sql.Format(_T("INSERT INTO Item (%s) OUTPUT INSERTED.* VALUES (%s)"), fields.c_str(),
			values.c_str());
		return OpenCommand(sql.c_str());
	}
*/

	HRESULT Delete(DWORD uid)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Delete);
		m_item_uid.QuadPart = uid;
		return OpenCommand(_T("DELETE FROM Item WHERE item_uid = ?"));
	}

	HRESULT RSByOwnerID(DWORD cid, int inWareHouse)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByCID);
		m_owner_id.QuadPart = cid;
		m_inWareHouse = inWareHouse;
		return OpenCommand(_T("SELECT * FROM Item WHERE owner_id = ? AND inWareHouse = ?"));
	}

	HRESULT RSByUID(DWORD uid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_GetByUID);
		m_item_uid.QuadPart = uid;
		return OpenCommand(_T("SELECT * FROM Item WHERE item_uid = ?"));
	}

	HRESULT SetLock(DWORD uid, Boolean lockValue)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ResetLock);	
		m_item_uid.QuadPart = uid;
		m_lock = lockValue? 1:0;
		return OpenCommand(_T("UPDATE Item SET lock = ? WHERE item_uid = ?"));	
	}

	HRESULT UpdateByAny(DWORD uid)
	{
		if (m_params.empty())
			return S_FALSE;

		String sql(_T("UPDATE Item SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE item_uid = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_item_uid.QuadPart = uid;
		m_params.push_back(_T("item_uid"));		// add item_uid at the end of params
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}
};

#endif // _DBO_ITEM_H_
