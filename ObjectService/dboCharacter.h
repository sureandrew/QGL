// dboCharacter.h : Declaration of the CdboCharacter

#pragma once
#ifndef _DBO_CHARACTER_H_
#define _DBO_CHARACTER_H_
#include "Dbo.h"
#include "ObjectService/Global.h"

// code generated on Thursday, March 22, 2007, 11:14 AM

class CdboCharacterAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboCharacterAccessor)
	DEFINE_ENTRY(acct_id, LARGE_INTEGER)
	DEFINE_ENTRY(char_id, LARGE_INTEGER)
	DEFINE_ENTRY_STR(nickName, 21)
	DEFINE_ENTRY(sex, INT)
	DEFINE_ENTRY(cclass, INT)
	DEFINE_ENTRY(faction, INT)
	DEFINE_ENTRY(level, LONG)
	DEFINE_ENTRY(exp, LARGE_INTEGER)
	DEFINE_ENTRY(STA, LONG)
	DEFINE_ENTRY(SPR, LONG)
	DEFINE_ENTRY(STR, LONG)
	DEFINE_ENTRY(CON, LONG)
	DEFINE_ENTRY(AGI, LONG)
	DEFINE_ENTRY(attrPoint, LONG)
	DEFINE_ENTRY(HP, LONG)
	DEFINE_ENTRY(mHP, LONG)
	DEFINE_ENTRY(maxHP, LONG)
	DEFINE_ENTRY(SP, LONG)
	DEFINE_ENTRY(mSP, LONG)
	DEFINE_ENTRY(maxSP, LONG)
	DEFINE_ENTRY(DP, LONG)
	DEFINE_ENTRY(mDP, LONG)
	DEFINE_ENTRY(maxDP, LONG)
	DEFINE_ENTRY(WC, LONG)
	DEFINE_ENTRY(maxWC, LONG)
	DEFINE_ENTRY(WcExp, LONG)
	DEFINE_ENTRY(VP, LONG)
	DEFINE_ENTRY(maxVP, LONG)

	DEFINE_ENTRY(goodness, LARGE_INTEGER)
	DEFINE_ENTRY(honor, LARGE_INTEGER)
	DEFINE_ENTRY_STR(title, 21)
	DEFINE_ENTRY(manor, LARGE_INTEGER)
	DEFINE_ENTRY(fDonate, LARGE_INTEGER)
	DEFINE_ENTRY(totalFDonate, LARGE_INTEGER)
	DEFINE_ENTRY(mDonate, LARGE_INTEGER)
	DEFINE_ENTRY(totalMDonate, LARGE_INTEGER)
	DEFINE_ENTRY(money, LARGE_INTEGER)
	DEFINE_ENTRY(bankMoney, LARGE_INTEGER)
	DEFINE_ENTRY(SMoney, LARGE_INTEGER)
	DEFINE_ENTRY(partner, LARGE_INTEGER)
	DEFINE_ENTRY(pet, LARGE_INTEGER)
	DEFINE_ENTRY(maxPartner, INT)
	DEFINE_ENTRY(maxPet, INT)

	DEFINE_ENTRY(map_id, LONG)
	DEFINE_ENTRY(map_x, LONG)
	DEFINE_ENTRY(map_y, LONG)
	DEFINE_ENTRY(map_ownerId, LONG)
	DEFINE_ENTRY(reborn_map, LONG)
	DEFINE_ENTRY(reborn_x, LONG)
	DEFINE_ENTRY(reborn_y, LONG)

	DEFINE_ENTRY(photo_id, LONG)
	DEFINE_ENTRY(hairStyle1, LONG)
	DEFINE_ENTRY(hairStyle2, LONG)
	DEFINE_ENTRY(hairColor, LONG)

	DEFINE_ENTRY(eq_weapon, LARGE_INTEGER)
	DEFINE_ENTRY(eq_weaponColor, LONG)
	DEFINE_ENTRY(eq_headBand, LARGE_INTEGER)
	DEFINE_ENTRY(eq_headColor, LONG)
	DEFINE_ENTRY(eq_cloth, LARGE_INTEGER)
	DEFINE_ENTRY(eq_clothColor, LONG)

	DEFINE_ENTRY(state, INT)
	DEFINE_ENTRY_STR(buff, 1101)
	
	DEFINE_ENTRY(shortcut0, LONG)
	DEFINE_ENTRY(shortcut1, LONG)
	DEFINE_ENTRY(shortcut2, LONG)
	DEFINE_ENTRY(shortcut3, LONG)
	DEFINE_ENTRY(shortcut4, LONG)
	DEFINE_ENTRY(shortcut5, LONG)
	DEFINE_ENTRY(shortcut6, LONG)
	DEFINE_ENTRY(shortcut7, LONG)
	DEFINE_ENTRY(shortcut8, LONG)
	DEFINE_ENTRY(shortcut9, LONG)	
	DEFINE_ENTRY(shortcuttype0, INT)
	DEFINE_ENTRY(shortcuttype1, INT)
	DEFINE_ENTRY(shortcuttype2, INT)
	DEFINE_ENTRY(shortcuttype3, INT)
	DEFINE_ENTRY(shortcuttype4, INT)
	DEFINE_ENTRY(shortcuttype5, INT)
	DEFINE_ENTRY(shortcuttype6, INT)
	DEFINE_ENTRY(shortcuttype7, INT)
	DEFINE_ENTRY(shortcuttype8, INT)
	DEFINE_ENTRY(shortcuttype9, INT)

	DEFINE_ENTRY(PK_Value, LONG)

	DEFINE_ENTRY(org_hairStyle1, LONG)
	DEFINE_ENTRY(org_hairColor, LONG)
	DEFINE_ENTRY(org_eq_cloth, LARGE_INTEGER)
	DEFINE_ENTRY(org_eq_clothColor, LONG)

	DEFINE_ENTRY(speakCount, LONG)
	DEFINE_ENTRY(offlineTime, DBTIMESTAMP)
	DEFINE_ENTRY(TP, INT)
	DEFINE_ENTRY(speakBlock, DBTIMESTAMP)
	DEFINE_ENTRY(hawkerTime, LARGE_INTEGER)
	DEFINE_ENTRY(availBackPack, INT)
	DEFINE_ENTRY(availItemWareHouse, INT)
	DEFINE_ENTRY(availPartnerWareHouse, INT)
	DEFINE_ENTRY(availPetWareHouse, INT)
	DEFINE_ENTRY(PvPScoreSingle, LARGE_INTEGER)
	DEFINE_ENTRY(deleteExpiryDate, DBTIMESTAMP)	
	DEFINE_ENTRY(bankLock, INT)	
	DEFINE_ENTRY(siblingId, LARGE_INTEGER)
	DEFINE_ENTRY(mark, INT)
	DEFINE_ENTRY(wantedReward, LARGE_INTEGER)
	DEFINE_ENTRY(guild_uid, LARGE_INTEGER)
	DEFINE_ENTRY(HistoryGenAccmulation,LARGE_INTEGER)
	DEFINE_ENTRY(CurGenAccmulation,LARGE_INTEGER)

	// In order to fix several issues with some providers, the code below may bind
	// columns in a different order than reported by the provider
	//ENABLE_COLUMN_MAP(CdboCharacterAccessor)
	//ENABLE_PARAM_MAP(CdboCharacterAccessor)

	BEGIN_COLUMN_MAP_FUNC(_GetAll)
		int i = 0;
		COLUMN_ENTRY_LS(++i, m_acct_id)
		COLUMN_ENTRY_LS(++i, m_char_id)
		COLUMN_ENTRY_LS(++i, m_nickName)
		COLUMN_ENTRY_LS(++i, m_sex)
		COLUMN_ENTRY_LS(++i, m_cclass)
		COLUMN_ENTRY_LS(++i, m_faction)
		COLUMN_ENTRY_LS(++i, m_level)
		COLUMN_ENTRY_LS(++i, m_exp)
		COLUMN_ENTRY_LS(++i, m_STA)
		COLUMN_ENTRY_LS(++i, m_SPR)
		COLUMN_ENTRY_LS(++i, m_STR)
		COLUMN_ENTRY_LS(++i, m_CON)
		COLUMN_ENTRY_LS(++i, m_AGI)
		COLUMN_ENTRY_LS(++i, m_attrPoint)
		COLUMN_ENTRY_LS(++i, m_HP)
		COLUMN_ENTRY_LS(++i, m_mHP)
		COLUMN_ENTRY_LS(++i, m_maxHP)
		COLUMN_ENTRY_LS(++i, m_SP)
		COLUMN_ENTRY_LS(++i, m_mSP)
		COLUMN_ENTRY_LS(++i, m_maxSP)
		COLUMN_ENTRY_LS(++i, m_DP)
		COLUMN_ENTRY_LS(++i, m_mDP)
		COLUMN_ENTRY_LS(++i, m_maxDP)
		COLUMN_ENTRY_LS(++i, m_WC)
		COLUMN_ENTRY_LS(++i, m_maxWC)
		COLUMN_ENTRY_LS(++i, m_WcExp)
		COLUMN_ENTRY_LS(++i, m_VP)
		COLUMN_ENTRY_LS(++i, m_maxVP)
		COLUMN_ENTRY_LS(++i, m_goodness)
		COLUMN_ENTRY_LS(++i, m_honor)
		COLUMN_ENTRY_LS(++i, m_title)
		COLUMN_ENTRY_LS(++i, m_manor)
		COLUMN_ENTRY_LS(++i, m_fDonate)
		COLUMN_ENTRY_LS(++i, m_totalFDonate)
		COLUMN_ENTRY_LS(++i, m_mDonate)
		COLUMN_ENTRY_LS(++i, m_totalMDonate)
		COLUMN_ENTRY_LS(++i, m_money)
		COLUMN_ENTRY_LS(++i, m_bankMoney)
		COLUMN_ENTRY_LS(++i, m_SMoney)
		COLUMN_ENTRY_LS(++i, m_partner)
		COLUMN_ENTRY_LS(++i, m_pet)
		COLUMN_ENTRY_LS(++i, m_maxPartner)
		COLUMN_ENTRY_LS(++i, m_maxPet)
		COLUMN_ENTRY_LS(++i, m_map_id)
		COLUMN_ENTRY_LS(++i, m_map_x)
		COLUMN_ENTRY_LS(++i, m_map_y)
		COLUMN_ENTRY_LS(++i, m_map_ownerId)
		COLUMN_ENTRY_LS(++i, m_reborn_map)
		COLUMN_ENTRY_LS(++i, m_reborn_x)
		COLUMN_ENTRY_LS(++i, m_reborn_y)
		COLUMN_ENTRY_LS(++i, m_photo_id)
		COLUMN_ENTRY_LS(++i, m_hairStyle1)
		COLUMN_ENTRY_LS(++i, m_hairStyle2)
		COLUMN_ENTRY_LS(++i, m_hairColor)
		COLUMN_ENTRY_LS(++i, m_eq_weapon)
		COLUMN_ENTRY_LS(++i, m_eq_weaponColor)
		COLUMN_ENTRY_LS(++i, m_eq_headBand)
		COLUMN_ENTRY_LS(++i, m_eq_headColor)
		COLUMN_ENTRY_LS(++i, m_eq_cloth)
		COLUMN_ENTRY_LS(++i, m_eq_clothColor)
		COLUMN_ENTRY_LS(++i, m_state)
		COLUMN_ENTRY_LS(++i, m_buff)
		COLUMN_ENTRY_LS(++i, m_shortcut0)
		COLUMN_ENTRY_LS(++i, m_shortcut1)
		COLUMN_ENTRY_LS(++i, m_shortcut2)
		COLUMN_ENTRY_LS(++i, m_shortcut3)
		COLUMN_ENTRY_LS(++i, m_shortcut4)
		COLUMN_ENTRY_LS(++i, m_shortcut5)
		COLUMN_ENTRY_LS(++i, m_shortcut6)
		COLUMN_ENTRY_LS(++i, m_shortcut7)
		COLUMN_ENTRY_LS(++i, m_shortcut8)
		COLUMN_ENTRY_LS(++i, m_shortcut9)
		COLUMN_ENTRY_LS(++i, m_shortcuttype0)
		COLUMN_ENTRY_LS(++i, m_shortcuttype1)
		COLUMN_ENTRY_LS(++i, m_shortcuttype2)
		COLUMN_ENTRY_LS(++i, m_shortcuttype3)
		COLUMN_ENTRY_LS(++i, m_shortcuttype4)
		COLUMN_ENTRY_LS(++i, m_shortcuttype5)
		COLUMN_ENTRY_LS(++i, m_shortcuttype6)
		COLUMN_ENTRY_LS(++i, m_shortcuttype7)
		COLUMN_ENTRY_LS(++i, m_shortcuttype8)
		COLUMN_ENTRY_LS(++i, m_shortcuttype9)
		COLUMN_ENTRY_LS(++i, m_PK_Value)
		COLUMN_ENTRY_LS(++i, m_org_hairStyle1)
		COLUMN_ENTRY_LS(++i, m_org_hairColor)
		COLUMN_ENTRY_LS(++i, m_org_eq_cloth)
		COLUMN_ENTRY_LS(++i, m_org_eq_clothColor)
		COLUMN_ENTRY_LS(++i, m_speakCount)
		COLUMN_ENTRY_LS(++i, m_offlineTime)
		COLUMN_ENTRY_LS(++i, m_TP)
		COLUMN_ENTRY_LS(++i, m_speakBlock)
		COLUMN_ENTRY_LS(++i, m_hawkerTime)
		COLUMN_ENTRY_LS(++i, m_availBackPack)
		COLUMN_ENTRY_LS(++i, m_availItemWareHouse)
		COLUMN_ENTRY_LS(++i, m_availPartnerWareHouse)
		COLUMN_ENTRY_LS(++i, m_availPetWareHouse)
		COLUMN_ENTRY_LS(++i, m_PvPScoreSingle)
		COLUMN_ENTRY_LS(++i, m_deleteExpiryDate)
		COLUMN_ENTRY_LS(++i, m_bankLock)
		COLUMN_ENTRY_LS(++i, m_siblingId)
		COLUMN_ENTRY_LS(++i, m_mark)
		COLUMN_ENTRY_LS(++i, m_wantedReward)
		COLUMN_ENTRY_LS(++i, m_guild_uid)
		COLUMN_ENTRY_LS(++i, m_HistoryGenAccmulation)
		COLUMN_ENTRY_LS(++i, m_CurGenAccmulation)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetName)
		COLUMN_ENTRY_LS( 1, m_nickName)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetByID)
		COLUMN_ENTRY_LS( 1, m_char_id)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetBuff)
		COLUMN_ENTRY_LS( 1, m_char_id)
		COLUMN_ENTRY_LS( 2, m_buff)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetMap)
		COLUMN_ENTRY_LS( 1, m_char_id)
		COLUMN_ENTRY_LS( 2, m_map_id)
		COLUMN_ENTRY_LS( 3, m_map_x)
		COLUMN_ENTRY_LS( 4, m_map_y)
		COLUMN_ENTRY_LS( 5, m_map_ownerId)
		COLUMN_ENTRY_LS( 6, m_reborn_map)
		COLUMN_ENTRY_LS( 7, m_reborn_x)
		COLUMN_ENTRY_LS( 8, m_reborn_y)
	END_COLUMN_MAP()

	BEGIN_COLUMN_MAP_FUNC(_GetRank)
		int i = 0;
		COLUMN_ENTRY_LS(++i, m_char_id)
		COLUMN_ENTRY_LS(++i, m_nickName)
		COLUMN_ENTRY_LS(++i, m_sex)
		COLUMN_ENTRY_LS(++i, m_cclass)
		COLUMN_ENTRY_LS(++i, m_faction)
		COLUMN_ENTRY_LS(++i, m_level)
		COLUMN_ENTRY_LS(++i, m_money)
		COLUMN_ENTRY_LS(++i, m_bankMoney)
		COLUMN_ENTRY_LS(++i, m_SMoney)
		COLUMN_ENTRY_LS(++i, m_honor)
		COLUMN_ENTRY_LS(++i, m_PK_Value)
		COLUMN_ENTRY_LS(++i, m_PvPScoreSingle)
		COLUMN_ENTRY_LS(++i, m_wantedReward)
	END_COLUMN_MAP()

	BEGIN_PARAM_MAP_FUNC(_ParamByName)
		COLUMN_ENTRY(1, m_nickName)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAID)
		COLUMN_ENTRY(1, m_acct_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByID)
		COLUMN_ENTRY(1, m_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_UpdateLoc)
		COLUMN_ENTRY(1, m_map_id)
		COLUMN_ENTRY(2, m_map_x)
		COLUMN_ENTRY(3, m_map_y)
		COLUMN_ENTRY(4, m_map_ownerId)
		COLUMN_ENTRY(5, m_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_UpdateGeneration)
	COLUMN_ENTRY(1, m_HistoryGenAccmulation)
	COLUMN_ENTRY(2, m_CurGenAccmulation)
	COLUMN_ENTRY(3, m_char_id)
	END_PARAM_MAP_FUNC()
	
	BEGIN_PARAM_MAP_FUNC(_UpdateWantedReward)
	COLUMN_ENTRY(1, m_wantedReward)
	COLUMN_ENTRY(2, m_char_id)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ParamByAny)
		COLUMN_ENTRY_TEST(m_params, acct_id)
		COLUMN_ENTRY_TEST_ELSE(char_id)
		COLUMN_ENTRY_TEST_ELSE(nickName)
		COLUMN_ENTRY_TEST_ELSE(sex)
		COLUMN_ENTRY_TEST_ELSE(cclass)
		COLUMN_ENTRY_TEST_ELSE(faction)
		COLUMN_ENTRY_TEST_ELSE(level)
		COLUMN_ENTRY_TEST_ELSE(exp)
		COLUMN_ENTRY_TEST_ELSE(STA)
		COLUMN_ENTRY_TEST_ELSE(SPR)
		COLUMN_ENTRY_TEST_ELSE(STR)
		COLUMN_ENTRY_TEST_ELSE(CON)
		COLUMN_ENTRY_TEST_ELSE(AGI)
		COLUMN_ENTRY_TEST_ELSE(attrPoint)
		COLUMN_ENTRY_TEST_ELSE(HP)
		COLUMN_ENTRY_TEST_ELSE(mHP)
		COLUMN_ENTRY_TEST_ELSE(maxHP)
		COLUMN_ENTRY_TEST_ELSE(SP)
		COLUMN_ENTRY_TEST_ELSE(mSP)
		COLUMN_ENTRY_TEST_ELSE(maxSP)
		COLUMN_ENTRY_TEST_ELSE(DP)
		COLUMN_ENTRY_TEST_ELSE(mDP)
		COLUMN_ENTRY_TEST_ELSE(maxDP)
		COLUMN_ENTRY_TEST_ELSE(WC)
		COLUMN_ENTRY_TEST_ELSE(maxWC)
		COLUMN_ENTRY_TEST_ELSE(WcExp)
		COLUMN_ENTRY_TEST_ELSE(VP)
		COLUMN_ENTRY_TEST_ELSE(maxVP)
		COLUMN_ENTRY_TEST_ELSE(goodness)
		COLUMN_ENTRY_TEST_ELSE(honor)
		COLUMN_ENTRY_TEST_ELSE(title)
		COLUMN_ENTRY_TEST_ELSE(manor)
		COLUMN_ENTRY_TEST_ELSE(fDonate)
		COLUMN_ENTRY_TEST_ELSE(totalFDonate)
		COLUMN_ENTRY_TEST_ELSE(mDonate)
		COLUMN_ENTRY_TEST_ELSE(totalMDonate)
		COLUMN_ENTRY_TEST_ELSE(money)
		COLUMN_ENTRY_TEST_ELSE(bankMoney)
		COLUMN_ENTRY_TEST_ELSE(SMoney)
		COLUMN_ENTRY_TEST_ELSE(partner)
		COLUMN_ENTRY_TEST_ELSE(pet)
		COLUMN_ENTRY_TEST_ELSE(maxPartner)
		COLUMN_ENTRY_TEST_ELSE(maxPet)
		COLUMN_ENTRY_TEST_ELSE(map_id)
		COLUMN_ENTRY_TEST_ELSE(map_x)
		COLUMN_ENTRY_TEST_ELSE(map_y)
		COLUMN_ENTRY_TEST_ELSE(map_ownerId)
		COLUMN_ENTRY_TEST_ELSE(reborn_map)
		COLUMN_ENTRY_TEST_ELSE(reborn_x)
		COLUMN_ENTRY_TEST_ELSE(reborn_y)
		COLUMN_ENTRY_TEST_ELSE(photo_id)
		COLUMN_ENTRY_TEST_ELSE(hairStyle1)
		COLUMN_ENTRY_TEST_ELSE(hairStyle2)
		COLUMN_ENTRY_TEST_ELSE(hairColor)
		COLUMN_ENTRY_TEST_ELSE(eq_weapon)
		COLUMN_ENTRY_TEST_ELSE(eq_weaponColor)
		COLUMN_ENTRY_TEST_ELSE(eq_headBand)
		COLUMN_ENTRY_TEST_ELSE(eq_headColor)
		COLUMN_ENTRY_TEST_ELSE(eq_cloth)
		COLUMN_ENTRY_TEST_ELSE(eq_clothColor)
		COLUMN_ENTRY_TEST_ELSE(state)
		COLUMN_ENTRY_TEST_ELSE(buff)
		COLUMN_ENTRY_TEST_ELSE(shortcut0)
		COLUMN_ENTRY_TEST_ELSE(shortcut1)
		COLUMN_ENTRY_TEST_ELSE(shortcut2)
		COLUMN_ENTRY_TEST_ELSE(shortcut3)
		COLUMN_ENTRY_TEST_ELSE(shortcut4)
		COLUMN_ENTRY_TEST_ELSE(shortcut5)
		COLUMN_ENTRY_TEST_ELSE(shortcut6)
		COLUMN_ENTRY_TEST_ELSE(shortcut7)
		COLUMN_ENTRY_TEST_ELSE(shortcut8)
		COLUMN_ENTRY_TEST_ELSE(shortcut9)
		COLUMN_ENTRY_TEST_ELSE(shortcuttype0)
		COLUMN_ENTRY_TEST_ELSE(shortcuttype1)
		COLUMN_ENTRY_TEST_ELSE(shortcuttype2)
		COLUMN_ENTRY_TEST_ELSE(shortcuttype3)
		COLUMN_ENTRY_TEST_ELSE(shortcuttype4)
		COLUMN_ENTRY_TEST_ELSE(shortcuttype5)
		COLUMN_ENTRY_TEST_ELSE(shortcuttype6)
		COLUMN_ENTRY_TEST_ELSE(shortcuttype7)
		COLUMN_ENTRY_TEST_ELSE(shortcuttype8)
		COLUMN_ENTRY_TEST_ELSE(shortcuttype9)
		COLUMN_ENTRY_TEST_ELSE(PK_Value)
		COLUMN_ENTRY_TEST_ELSE(org_hairStyle1)
		COLUMN_ENTRY_TEST_ELSE(org_hairColor)
		COLUMN_ENTRY_TEST_ELSE(org_eq_cloth)
		COLUMN_ENTRY_TEST_ELSE(org_eq_clothColor)
		COLUMN_ENTRY_TEST_ELSE(speakCount)
		COLUMN_ENTRY_TEST_ELSE(offlineTime)
		COLUMN_ENTRY_TEST_ELSE(TP)
		COLUMN_ENTRY_TEST_ELSE(speakBlock)
		COLUMN_ENTRY_TEST_ELSE(hawkerTime)
		COLUMN_ENTRY_TEST_ELSE(availBackPack)
		COLUMN_ENTRY_TEST_ELSE(availItemWareHouse)
		COLUMN_ENTRY_TEST_ELSE(availPartnerWareHouse)
		COLUMN_ENTRY_TEST_ELSE(availPetWareHouse)
		COLUMN_ENTRY_TEST_ELSE(PvPScoreSingle)		
		COLUMN_ENTRY_TEST_ELSE(deleteExpiryDate)
		COLUMN_ENTRY_TEST_ELSE(bankLock)	
		COLUMN_ENTRY_TEST_ELSE(siblingId)
		COLUMN_ENTRY_TEST_ELSE(mark)
		COLUMN_ENTRY_TEST_ELSE(wantedReward)
		COLUMN_ENTRY_TEST_ELSE(HistoryGenAccmulation)
		COLUMN_ENTRY_TEST_ELSE(CurGenAccmulation)
		COLUMN_ENTRY_TEST_END()
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_acct_id)
		COLUMN_ENTRY(2, m_nickName)
		COLUMN_ENTRY(3, m_sex)
		COLUMN_ENTRY(4, m_cclass)
		COLUMN_ENTRY(5, m_faction)
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ResetDeleteTime)
		COLUMN_ENTRY(1, m_deleteExpiryDate)
		COLUMN_ENTRY(2, m_char_id)	
	END_PARAM_MAP_FUNC()

	BEGIN_PARAM_MAP_FUNC(_ResetBankLock)
		COLUMN_ENTRY(1, m_bankLock)
		COLUMN_ENTRY(2, m_char_id)	
	END_PARAM_MAP_FUNC()	

	#define CHAR_FIELDS(_prefix_)					\
		FIELD_FIRST(_prefix_, acct_id)				\
		FIELD_NEXT(_prefix_, char_id)				\
		FIELD_NEXT(_prefix_, nickName)				\
		FIELD_NEXT(_prefix_, sex)					\
		FIELD_NEXT(_prefix_, cclass)				\
		FIELD_NEXT(_prefix_, faction)				\
		FIELD_NEXT(_prefix_, level)					\
		FIELD_NEXT(_prefix_, exp)					\
		FIELD_NEXT(_prefix_, STA)					\
		FIELD_NEXT(_prefix_, SPR)					\
		FIELD_NEXT(_prefix_, STR)					\
		FIELD_NEXT(_prefix_, CON)					\
		FIELD_NEXT(_prefix_, AGI)					\
		FIELD_NEXT(_prefix_, attrPoint)				\
		FIELD_NEXT(_prefix_, HP)					\
		FIELD_NEXT(_prefix_, mHP)					\
		FIELD_NEXT(_prefix_, maxHP)					\
		FIELD_NEXT(_prefix_, SP)					\
		FIELD_NEXT(_prefix_, mSP)					\
		FIELD_NEXT(_prefix_, maxSP)					\
		FIELD_NEXT(_prefix_, DP)					\
		FIELD_NEXT(_prefix_, mDP)					\
		FIELD_NEXT(_prefix_, maxDP)					\
		FIELD_NEXT(_prefix_, WC)					\
		FIELD_NEXT(_prefix_, maxWC)					\
		FIELD_NEXT(_prefix_, WcExp)					\
		FIELD_NEXT(_prefix_, VP)					\
		FIELD_NEXT(_prefix_, maxVP)					\
		FIELD_NEXT(_prefix_, goodness)				\
		FIELD_NEXT(_prefix_, honor)					\
		FIELD_NEXT(_prefix_, title)					\
		FIELD_NEXT(_prefix_, manor)					\
		FIELD_NEXT(_prefix_, fDonate)				\
		FIELD_NEXT(_prefix_, totalFDonate)			\
		FIELD_NEXT(_prefix_, mDonate)				\
		FIELD_NEXT(_prefix_, totalMDonate)			\
		FIELD_NEXT(_prefix_, money)					\
		FIELD_NEXT(_prefix_, bankMoney)				\
		FIELD_NEXT(_prefix_, SMoney)				\
		FIELD_NEXT(_prefix_, partner)				\
		FIELD_NEXT(_prefix_, pet)					\
		FIELD_NEXT(_prefix_, maxPartner)			\
		FIELD_NEXT(_prefix_, maxPet)				\
		FIELD_NEXT(_prefix_, map_id)				\
		FIELD_NEXT(_prefix_, map_x)					\
		FIELD_NEXT(_prefix_, map_y)					\
		FIELD_NEXT(_prefix_, map_ownerId)			\
		FIELD_NEXT(_prefix_, reborn_map)			\
		FIELD_NEXT(_prefix_, reborn_x)				\
		FIELD_NEXT(_prefix_, reborn_y)				\
		FIELD_NEXT(_prefix_, photo_id)				\
		FIELD_NEXT(_prefix_, hairStyle1)			\
		FIELD_NEXT(_prefix_, hairStyle2)			\
		FIELD_NEXT(_prefix_, hairColor)				\
		FIELD_NEXT(_prefix_, eq_weapon)				\
		FIELD_NEXT(_prefix_, eq_weaponColor)		\
		FIELD_NEXT(_prefix_, eq_headBand)			\
		FIELD_NEXT(_prefix_, eq_headColor)			\
		FIELD_NEXT(_prefix_, eq_cloth)				\
		FIELD_NEXT(_prefix_, eq_clothColor)			\
		FIELD_NEXT(_prefix_, state)					\
		FIELD_NEXT(_prefix_, buff)					\
		FIELD_NEXT(_prefix_, shortcut0)				\
		FIELD_NEXT(_prefix_, shortcut1)				\
		FIELD_NEXT(_prefix_, shortcut2)				\
		FIELD_NEXT(_prefix_, shortcut3)				\
		FIELD_NEXT(_prefix_, shortcut4)				\
		FIELD_NEXT(_prefix_, shortcut5)				\
		FIELD_NEXT(_prefix_, shortcut6)				\
		FIELD_NEXT(_prefix_, shortcut7)				\
		FIELD_NEXT(_prefix_, shortcut8)				\
		FIELD_NEXT(_prefix_, shortcut9)				\
		FIELD_NEXT(_prefix_, shortcuttype0)			\
		FIELD_NEXT(_prefix_, shortcuttype1)			\
		FIELD_NEXT(_prefix_, shortcuttype2)			\
		FIELD_NEXT(_prefix_, shortcuttype3)			\
		FIELD_NEXT(_prefix_, shortcuttype4)			\
		FIELD_NEXT(_prefix_, shortcuttype5)			\
		FIELD_NEXT(_prefix_, shortcuttype6)			\
		FIELD_NEXT(_prefix_, shortcuttype7)			\
		FIELD_NEXT(_prefix_, shortcuttype8)			\
		FIELD_NEXT(_prefix_, shortcuttype9)			\
		FIELD_NEXT(_prefix_, PK_Value)				\
		FIELD_NEXT(_prefix_, org_hairStyle1)		\
		FIELD_NEXT(_prefix_, org_hairColor)			\
		FIELD_NEXT(_prefix_, org_eq_cloth)			\
		FIELD_NEXT(_prefix_, org_eq_clothColor)		\
		FIELD_NEXT(_prefix_, speakCount)			\
		FIELD_NEXT(_prefix_, offlineTime)			\
		FIELD_NEXT(_prefix_, TP)					\
		FIELD_NEXT(_prefix_, speakBlock)			\
		FIELD_NEXT(_prefix_, hawkerTime)			\
		FIELD_NEXT(_prefix_, availBackPack)			\
		FIELD_NEXT(_prefix_, availItemWareHouse)	\
		FIELD_NEXT(_prefix_, availPartnerWareHouse) \
		FIELD_NEXT(_prefix_, availPetWareHouse)		\
		FIELD_NEXT(_prefix_, PvPScoreSingle)		\
		FIELD_NEXT(_prefix_, deleteExpiryDate)		\
		FIELD_NEXT(_prefix_, bankLock)				\
		FIELD_NEXT(_prefix_, siblingId)				\
		FIELD_NEXT(_prefix_, mark)					\
		FIELD_NEXT(_prefix_, wantedReward)			\
		FIELD_NEXT(_prefix_, guild_uid)				\
		FIELD_NEXT(_prefix_, HistoryGenAccmulation)	\
		FIELD_NEXT(_prefix_, CurGenAccmulation)

	DEFINE_COMMAND_EX(CdboCharacterAccessor, 
		_T("SELECT ") CHAR_FIELDS("") _T(" FROM CharacterView"))

	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
};

class CdboCharacter : public Cdbo<CCommand<CAccessor<CdboCharacterAccessor> > >
{
public:
	HRESULT Insert(DWORD aid, LPCTSTR nickName, int sex, int cclass, int faction)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_Insert);
		m_acct_id.QuadPart = aid;
		COLUMN_STRING_COPY(m_nickName, nickName)
		m_sex = sex;
		m_cclass = cclass;
		m_faction = faction;
		return OpenCommand(_T("INSERT INTO CharacterView (acct_id, nickName, sex, cclass, faction)")
			_T("OUTPUT ") CHAR_FIELDS("INSERTED.") _T(" VALUES (?, ?, ?, ?, ?)"));
	}

	HRESULT Delete(DWORD cid)
	{
		//HRESULT hr;
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ResetDeleteTime);
		m_char_id.QuadPart = cid;
		time_t zeroTime = 0;
		TimeConvert(zeroTime, m_deleteExpiryDate);	
		// set void to remove in view, real delete character by DB maintaince
		return OpenCommand(_T("UPDATE Character SET deleteExpiryDate = ?, void = 1  WHERE char_id = ?"));
	}

	HRESULT Restore(DWORD cid)
	{
		//HRESULT hr;
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ResetDeleteTime);
		m_char_id.QuadPart = cid;
		time_t zeroTime = 0;
		TimeConvert(zeroTime, m_deleteExpiryDate);	
		
		return OpenCommand(_T("UPDATE Character SET deleteExpiryDate = ? WHERE char_id = ?"));
	}

	HRESULT SetDeleteTime(DWORD cid, DBTIMESTAMP deleteTime)
	{
		//HRESULT hr;
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ResetDeleteTime);
		m_char_id.QuadPart = cid;
		m_deleteExpiryDate = deleteTime;
		// set void to remove in view, real delete character by DB maintaince
		return OpenCommand(_T("UPDATE Character SET deleteExpiryDate = ? WHERE char_id = ?"));
	}

	HRESULT SetBankLock(DWORD cid, Boolean lock)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ResetBankLock);
		m_char_id.QuadPart = cid;
		m_bankLock = lock? 1:0;	
		return OpenCommand(_T("UPDATE Character SET bankLock = ? WHERE char_id = ?"));
	}

	HRESULT RSByAccountID(DWORD aid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByAID);
		m_acct_id.QuadPart = aid;
		return OpenCommand(_T("SELECT ") CHAR_FIELDS("") _T(" FROM CharacterView WHERE acct_id = ?"));
	}

	HRESULT RSByID(DWORD cid)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByID);
		m_char_id.QuadPart = cid;
		return OpenCommand(_T("SELECT ") CHAR_FIELDS("") _T(" FROM CharacterView WHERE char_id = ?"));
	}

	HRESULT RSByIDstr(LPCTSTR idStr)
	{
		String sql;

		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(NULL);
		sql.Format(_T("SELECT ") CHAR_FIELDS("") _T(" FROM CharacterView WHERE char_id IN (%s)"), 
			idStr);
		
		return OpenCommand(sql.c_str());
	}

	HRESULT RSByName(LPCTSTR cszName)
	{
		SetGetBindFunc(_GetAll);
		SetGetParamsFunc(_ParamByName);
		COLUMN_STRING_COPY(m_nickName, cszName)
		return OpenCommand(_T("SELECT ") CHAR_FIELDS("") _T(" FROM CharacterView WHERE nickName = ?"));
	}

	HRESULT RSNameByName(LPCTSTR cszName)
	{
		SetGetBindFunc(_GetName);
		SetGetParamsFunc(_ParamByName);
		COLUMN_STRING_COPY(m_nickName, cszName)
		return OpenCommand(_T("SELECT nickName FROM Character WHERE nickName = ?"));
	}

	HRESULT InactiveName(LPCTSTR cszName)
	{
		SetGetBindFunc(_GetName);
		SetGetParamsFunc(_ParamByName);
		COLUMN_STRING_COPY(m_nickName, cszName)
		return OpenCommand(_T("SELECT nickName FROM InactiveCharacter WHERE nickName = ?"));
	}

	HRESULT RSIDByAccountID(DWORD aid)
	{
		SetGetBindFunc(_GetByID);
		SetGetParamsFunc(_ParamByAID);
		m_acct_id.QuadPart = aid;
		return OpenCommand(_T("SELECT char_id FROM CharacterView WHERE acct_id = ?"));
	}

	HRESULT RSTopCase(LPCTSTR whereStr, int top)
	{
		SetGetBindFunc(_GetRank);
		SetGetParamsFunc(NULL);
		String sql;

#ifdef DB_MYSQL
		sql.Format(_T("SELECT char_id, nickName, sex, cclass, faction, \"level\", ")
					_T("money, bankMoney, SMoney, honor, PK_Value, PvPScoreSingle,wantedReward ")
					_T("FROM CharacterView WHERE %s LIMIT %d"), whereStr, top);
#else
		sql.Format(_T("SELECT TOP %d char_id, nickName, sex, cclass, faction, \"level\", ")
					_T("money, bankMoney, SMoney, honor, PK_Value, PvPScoreSingle,wantedReward ")
					_T("FROM CharacterView WHERE %s"), top, whereStr);
#endif // MYSQL

		return OpenCommand(sql.c_str());
	}

	HRESULT UpdateLocation(DWORD cid, unsigned int map_id, unsigned int x, unsigned int y,
							unsigned int map_ownerId)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_UpdateLoc);
		m_char_id.QuadPart = cid;
		m_map_id = map_id;
		m_map_x = x;
		m_map_y = y;
		m_map_ownerId = map_ownerId;
		return OpenCommand(_T("UPDATE CharacterView SET map_id = ?, map_x = ?, map_y = ?, map_ownerId = ? WHERE char_id = ?"));
	}

	HRESULT UpdateGeneration(UInt32 uiCharId, UInt32 uiHistoryAcc,UInt32 uiCurAcc)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_UpdateGeneration);
		m_char_id.QuadPart = uiCharId;
		m_HistoryGenAccmulation.QuadPart = uiHistoryAcc;
		m_CurGenAccmulation.QuadPart = uiCurAcc;
		return OpenCommand(_T("UPDATE CharacterView SET HistoryGenAccmulation = ?, CurGenAccmulation = ? WHERE char_id = ?"));
	}

	HRESULT UpdateWantedReward(DWORD cid,DWORD money)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_UpdateWantedReward);
		m_char_id.QuadPart = cid;
		m_wantedReward.QuadPart = money;
		return OpenCommand(_T("UPDATE CharacterView SET wantedReward = wantedReward + ? WHERE char_id = ?"));
	}

	HRESULT UpdateByAny(DWORD cid)
	{
		if (m_params.empty()) {
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UpdateByAny Error: empty paramater for char_id %d"),
				cid);
			return S_FALSE;
		}

		String sql(_T("UPDATE CharacterView SET \""));

		sql += m_params.front() + _T("\" = ?");
		for (StringVector::iterator it = m_params.begin() + 1; it != m_params.end(); ++it)
		{
			String &param = *it;
			sql += _T(", \"") + param + _T("\" = ?");
		}
		sql += _T(" WHERE char_id = ?");

		SetGetBindFunc(NULL);
		SetGetParamsFunc(_ParamByAny);
		m_char_id.QuadPart = cid;
		m_params.push_back(_T("char_id"));		// add char_id at the end of params
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateByAny SQL: %s"), sql.c_str());

		return OpenCommand(sql.c_str());
	}

	HRESULT RSBuff()
	{
		SetGetBindFunc(_GetBuff);
		return OpenCommand(_T("Select char_id, buff From CharacterView"));
	}

	HRESULT RSMap()
	{
		SetGetBindFunc(_GetMap);
		return OpenCommand(_T("Select map_id, map_x, map_y, map_ownerId, reborn_map, reborn_x, reborn_y From CharacterView"));
	}
};

#endif // _DBO_CHARACTER_H_

