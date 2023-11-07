#pragma once

#ifndef _SETTING_TABLE_H_
#define _SETTING_TABLE_H_ 

// character attribute setting table
//#define CHAR_CLASS_NUM		4
/*struct CharAttrSet
{
	float hp_sta;
	float sp_spr;
	float atk_str;
	float atk_spr;
	float def_con;
	float hit_str;
	float eva_agi;
	float pow_spr;
	float pow_sta;
	float pow_con;
	float pow_str;
	float spd_agi;
	float spd_sta;
	float spd_str;
};
extern CharAttrSet gCharAttrSet[CHAR_CLASS_NUM];*/

// Mob potential setting table
#define MOB_POTENT_NUM		10
struct MobPotentSet
{
	unsigned short sta;
	unsigned short spr;
	unsigned short str;
	unsigned short con;
	unsigned short agi;
};
extern MobPotentSet gMobPotentSet[MOB_POTENT_NUM];

// Character level exp setting table
#define CHAR_MAX_LEVEL		(140 + 1)
extern unsigned int gCharExpSet[CHAR_MAX_LEVEL];

// Partner level exp setting table
#define PARTNER_MAX_LEVEL		(140 + 1)
extern unsigned int gPartnerExpSet[PARTNER_MAX_LEVEL];

#endif // _SETTING_TABLE_H_
