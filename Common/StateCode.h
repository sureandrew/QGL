#pragma once

#ifndef _STATECODE_H_
#define _STATECODE_H_ 

// character status in map and battle
enum CharStateEnum {
	CHAR_ST_NORM		= 0,	// normal
	CHAR_ST_DEAD,				// dead
	CHAR_ST_DIZZY,				// dizzy
	CHAR_ST_SLEEP,				// sleep
	CHAR_ST_SURRENDER,			// surrender
	CHAR_ST_ESCAPED,			// escaped
	CHAR_ST_BACKUP,				// at backup stage
	CHAR_ST_CATCHED,			// be caught
	CHAR_ST_EXCHANGE,			// 
};

#endif // _STATECODE_H_
