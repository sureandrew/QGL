#pragma once

#ifndef _BATTLECODE_H_
#define _BATTLECODE_H_ 

enum BATTLE_SPECIAL_END
{
	BATTLE_SPECIAL_END_TIMEOUT = 0,
};
// action and reaction command
enum BatCmdEnum {
	// action
	BATCMD_IDLE		= 0,		// idle
	BATCMD_ATTACK,				// physical attack
	BATCMD_SKILL,				// use skill
	BATCMD_SPECIAL,				// use special
	BATCMD_ITEM,				// use item
	BATCMD_DEFEND,				// defend
	BATCMD_FRIEND,				// call friend
	BATCMD_PROTECT,				// protect other
	BATCMD_CATCH,				// catch monster
	BATCMD_AUTO,				// auto command
	BATCMD_HELP,				// help command
	BATCMD_ESCAPE,				// escape battle
	BATCMD_WINE,				// drink wine
	MAX_ACTION_CMD,
	BATCMD_REPEAT,				// repeat command
	BATCMD_CRIT_ATTACK,			// critical attack
	BATCMD_ESCAPE_FAIL,			// escape battle failed
	

	// reaction
	BATCMD_MISS,				// missed
	BATCMD_NORM_HIT,			// normal hit
	BATCMD_CRIT_HIT,			// critical hit
	BATCMD_BREAK_HIT,			// break attack hit
	BATCMD_DEFEND_HIT,			// defend hit
	BATCMD_PROTECT_HIT,			// protect hit
	BATCMD_SKILL_HIT,			// skill hit
	BATCMD_SPECIAL_HIT,			// special hit
	BATCMD_KNOCKBACK,			// being knock back
	BATCMD_USED_ITEM,			// used item
	BATCMD_USED_SKILL,			// used skill
	BATCMD_CATCHED,				// catched
	BATCMD_CATCHED_FAIL,		// failed to be caught
	BATCMD_SHOW_FRIEND,			// the partner jump to the battle
	BATCMD_BUFF_EFFECT_HIT,		// buff efect active
	BATCMD_COUNTER_ATTACK_HIT,	// counter attack reaction
	BATCMD_COUNTER_ATTACK_DEF_HIT,

	BATCMD_CRIT_SKILL,
	BATCMD_DEFEND_CRIT_HIT,
	BATCMD_INVISIBLE,			// the org partner will invisible when new partner jump in
};

enum BatIndex
{
	// special index
	BATIDX_TEAM		= 0xf0,			// team base
	BATIDX_TEAM0	= 0xf0,			// team0
	BATIDX_TEAM1	= 0xf1,			// team1
	BATIDX_ALL		= 0xfe,			// all actors
	BATIDX_INVALID	= 0xff			// invalid index
};

enum BATSCRIPT_BLOCK_CMD_ENUM
{
	BATBLOCK_IDLE	 = 0x00000001,
	BATBLOCK_ATTACK	 = 0x00000002,
	BATBLOCK_SKILL	 = 0x00000003,
	BATBLOCK_SPECIAL = 0x00000008,
	BATBLOCK_ITEM	 = 0x00000010,
	BATBLOCK_DEFEND	 = 0x00000020,
	BATBLOCK_FRIEND	 = 0x00000040,
	BATBLOCK_PROTECT = 0x00000080,
	BATBLOCK_CATCH	 = 0x00000100,
	BATBLOCK_AUTO	 = 0x00000200,
	BATBLOCK_HELP	 = 0x00000400,
	BATBLOCK_ESCAPE	 = 0x00000800,
	BATBLOCK_WINE	 = 0x00001000,

	BATBLOCK_ALLCMD	 = 0xFFFFFFFF,
};

#endif // _BATTLECODE_H_
