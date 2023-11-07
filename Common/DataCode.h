/*
	DataCode.h - common code in data structure in spec

	Please put common code that really all projects needed here.
*/


#pragma once

#ifndef _DATA_CODE_H_
#define _DATA_CODE_H_ 

///////////////////////////////
///			Common			///
///////////////////////////////

enum DC_Sex {						// Sex and Class Type
	SEX_MALE				= 0,		// male
	SEX_FEMALE				= 1,		// female
	MAX_SEXTYPE,
};

enum DC_Class {						// Class Type
	CLASS_NONE				= 0,		// none
	CLASS_PALADIN			= 1,		// paladin class
	CLASS_THIEF				= 2,		// thief class
	CLASS_DOCTOR			= 3,		// doctor class
	CLASS_ASSASSIN			= 4,		// assassin class
};

enum DC_SexClass {					// Sex and Class Type
	SEXCLASS_NONE			= 0,		// none
	SEXCLASS_M_PALADIN		= 1,		// male paladin class
	SEXCLASS_F_PALADIN		= 2,		// female paladin class
	SEXCLASS_M_THIEF		= 3,		// male thief class
	SEXCLASS_F_THIEF		= 4,		// female thief class
	SEXCLASS_M_DOCTOR		= 5,		// male doctor class
	SEXCLASS_F_DOCTOR		= 6,		// female doctor class
	SEXCLASS_M_ASSASSIN		= 7,		// male assassin class
	SEXCLASS_F_ASSASSIN		= 8,		// female assassin class
	SEXCLASS_MALE			= 100,		// all male class
	SEXCLASS_FEMALE			= 101,		// all female class
};

enum DC_Field {					// Field Type
	FIELD_ANY			= 0,		// any field
	FIELD_BATTLE		= 1,		// battle field
	FIELD_MAP			= 2,		// map field
};

///////////////////////////////
///			Item			///
///////////////////////////////
enum DC_ItemType {					// Item Type
	ITEMTYPE_MISC 		= 0,			// misc stuff
	ITEMTYPE_EQUIP		= 1,			// equipment
	ITEMTYPE_FOOD		= 2,			// food
	ITEMTYPE_FOODMAT	= 3,			// food material
	ITEMTYPE_DRUG		= 4,			// drug
	ITEMTYPE_DRUGMAT	= 5,			// drug material
	ITEMTYPE_WINE		= 6,			// wine
	ITEMTYPE_WINEMAT	= 7,			// wine material
	ITEMTYPE_EQUIPMAT	= 8,			// equipment material
	ITEMTYPE_CLOTHMAT	= 9,			// cloth material
	ITEMTYPE_GEM		= 10,			// gem
	ITEMTYPE_THROW		= 11,			// throw weapon
	ITEMTYPE_SKILLBOOK	= 12,			// skill book
	ITEMTYPE_SCRIPT		= 13,			// script item
	ITEMTYPE_ADDON 		= 14,			// addon item
	ITEMTYPE_ARRAYBOOK 	= 15,			// array book
	ITEMTYPE_SPECIAL 	= 16,			// special item
	ITEMTYPE_BAG 		= 17,			// bag
	ITEMTYPE_REPAIR		= 18,			// eq_repair
	ITEMTYPE_REBUILD	= 19,			// eq_rebuild
	ITEMTYPE_STONE		= 20,			// stone for mix item
	ITEMTYPE_ORNAMENTMAT= 21,			// ornament material
	ITEMTYPE_IDENTIFY	= 22,			// eq_identify
	ITEMTYPE_FIREWORKS	= 23,
	ITEMTYPE_LIGHTSTONE	= 24,
	ITEMTYPE_UNSOULBOUND	= 25,
	ITEMTYPE_BUSINESS_ITEM	= 26,		// business item
	ITEMTYPE_BUSINESS_MONEY	= 27,		// business money
	ITEMTYPE_FAST_FOOD = 28,
	ITEMTYPE_WEAPON_MODEL = 29,
	ITEMTYPE_FLOWER = 30,
	ITEMTYPE_IRON = 31,
	ITEMTYPE_SCROLL = 32,
	ITEMTYPE_PET_FOOD = 33,				//pet use food
};

enum DC_EqType {					// Equipment Type
	EQTYPE_SPEAR		= 0,			// spear
	EQTYPE_WIPE			= 1,			// wipe
	EQTYPE_FAN			= 2,			// fan weapon
	EQTYPE_PEN			= 3,			// pen weapon
	EQTYPE_SWORD		= 4,			// sword
	EQTYPE_DBSWORD		= 5,			// double sword
	EQTYPE_BLADE		= 6,			// blade
	EQTYPE_OTHER		= 7,			// other weapon
	EQTYPE_HANDRING		= 8,			// hand ring
	EQTYPE_CLAW			= 9,			// claw
	EQTYPE_GEAR			= 10,			// gear
	EQTYPE_ARMOR		= 11,			// body armor
	EQTYPE_SHOE			= 12,			// shoe
	EQTYPE_BELT			= 13,			// belt
	EQTYPE_RING			= 14,			// ring
	EQTYPE_AMULET		= 15,			// amulet
	EQTYPE_CLOTH		= 16,			// cloth
	EQTYPE_HAT_WITH_HAIR	= 17,
	EQTYPE_HAT_WITHOUT_HAIR	= 18,
	EQTYPE_ALL_WEAPON	= 19,			// all weapon
	EQTYPE_ALL_CLOTH	= 20,			// all cloth
	EQTYPE_ALL_ORNAMENT = 21,			// all ornament
};

enum DC_EqPlace {					// Equipment Place Index
	EQPLACE_WEAPON		= 0,			// weapon place
	EQPLACE_GEAR		= 1,			// gear place
	EQPLACE_ARMOR		= 2,			// body armor place
	EQPLACE_SHOE		= 3,			// shoe place
	EQPLACE_BELT		= 4,			// belt place
	EQPLACE_RING		= 5,			// ring place
	EQPLACE_AMULET		= 6,			// amulet place 1
	EQPLACE_HEAD_ACCESSORY		= 7,			// amulet place 2
	EQPLACE_CLOTH		= 8,			// cloth place
	EQPLACE_ANY			= 9,			// any place
};

enum DC_ItemForm {					// Item Form
	ITEMFORM_NONE			= 0,		// none
	ITEMFORM_ATTACK			= 1,		// attack form
	ITEMFORM_HEAL			= 2,		// heal form
	ITEMFORM_INDIRECT		= 3,		// indirect form
	ITEMFORM_ASSIST			= 4,		// assist form
	ITEMFORM_RESURRECT		= 5,		// resurrect form
	ITEMFORM_MISC			= 6,		// misc form
};

enum DC_ItemTarget {				// Item Target 
	ITEM_TARGET_SINGLE		= 0,		// single target
	ITEM_TARGET_HP_ASC		= 1,		// target sort by HP ascending
	ITEM_TARGET_HP_DESC		= 2,		// target sort by HP descending
	ITEM_TARGET_SP_ASC		= 3,		// target sort by SP ascending
	ITEM_TARGET_SP_DESC		= 4,		// target sort by SP descending
	ITEM_TARGET_SPD_ASC		= 5,		// target sort by SPD ascending
	ITEM_TARGET_SPD_DESC	= 6,		// target sort by SPD descending
	ITEM_TARGET_DEF_ASC		= 7,		// target sort by DEF ascending
	ITEM_TARGET_DEF_DESC	= 8,		// target sort by DEF descending
	ITEM_TARGET_POW_ASC		= 9,		// target sort by POW ascending
	ITEM_TARGET_POW_DESC	= 10,		// target sort by POW descending
	ITEM_TARGET_ATK_ASC		= 11,		// target sort by ATK ascending
	ITEM_TARGET_ATK_DESC	= 12,		// target sort by ATK descending
};


///////////////////////////////
///			Skill			///
///////////////////////////////

enum DC_SkillType {					// Item Form
	SKILLTYPE_FACTION		= 0,		// faction skill
	SKILLTYPE_FACTION_BASE	= 1,		// faction skill base
	SKILLTYPE_PARTNER		= 2,		// partner skill
	SKILLTYPE_PET			= 3,		// pet skill
	SKILLTYPE_MANOR			= 4,		// manor skill
	SKILLTYPE_MANOR_BASE	= 5,		// manor skill base
	SKILLTYPE_LIFE			= 6,		// life skill
	SKILLTYPE_LIFE_BASE		= 7,		// life skill base
};

enum DC_SkillTarget {				// Skill Target 
	SKILL_TARGET_SINGLE		= 0,		// single target
	SKILL_TARGET_MULTI		= 1,		// multiple target
	SKILL_TARGET_TEAM		= 2,		// all in team
	SKILL_TARGET_COL_US		= 3,		// everyone in column of target
	SKILL_TARGET_ROW_US		= 4,		// everyone in row of target
	SKILL_TARGET_ALL		= 5,		// all in battle
};

enum DC_SkillTargetType {			// Skill Target Type
	SKILL_TTYPE_ANY		= 0,		//  any type
	SKILL_TTYPE_SELF	= 1,		//  self only
	SKILL_TTYPE_PARTNER	= 2,		//  self partner only
	SKILL_TTYPE_PET		= 3,		//  self pet only
	SKILL_TTYPE_US		= 4,		//  any in our team
	SKILL_TTYPE_THEM	= 5,		//  any in enemy team
	SKILL_TTYPE_XPBM	= 6,		//  any in enemy team except partner, boss, mob
	SKILL_TTYPE_XCB		= 7,		//  any in enemy team except character, boss
	SKILL_TTYPE_XCPM	= 8,		//  any in enemy team except character, partner, mob
	SKILL_TTYPE_XCP		= 9,		//  any in enemy team except character, partner
	SKILL_TTYPE_XB		= 10,		//  any in enemy team except boss
	SKILL_TTYPE_ITEM	= 11,		//  item target
	SKILL_TTYPE_QUEST	= 12,		//  quest target
};

#endif // _DATA_CODE_H_
