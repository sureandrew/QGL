//-- Common
#include "Common.h"
//-- Self
#include "SettingTable.h"

// Player attribute calucation
/*CharAttrSet gCharAttrSet[CHAR_CLASS_NUM] =
{
	{	// paladin
		4.5f, 4.1f, 
		1.0f, 
		0.2f, 1.3f, 2.0f, 
		1.1f, 1.0f, 
		0.3f, 0.2f, 0.2f, 
		0.6f, 
		0.1f, 
		0.1f
	},
	{	// thief
		4.2f, 3.4f, 
		1.2f, 
		0.2f, 1.2f, 2.2f, 
		1.8f, 0.6f, 
		0.3f, 0.2f, 0.2f, 
		1.0f, 
		0.1f, 
		0.1f
	},
	{	// doctor
		4.8f, 3.6f, 
		1.05f, 
		0.2f, 1.4f, 2.1f, 
		1.3f, 0.7f, 
		0.3f, 0.2f, 0.2f, 
		0.7f, 
		0.1f, 
		0.1f
	},

	{ 	// assisian
		4.3f, 3.2f, 
		1.35f, 
		0.2f, 1.1f, 2.5f, 
		1.2f, 0.8f, 
		0.3f, 0.2f, 0.2f, 
		0.8f, 
		0.1f, 
		0.1f
	},
};*/

// MOB level up pontential points
MobPotentSet gMobPotentSet[MOB_POTENT_NUM] =
{
	{ 2, 2, 2, 2, 2 }, 			// 0
	{ 5, 2, 1, 1, 1 }, 			// 1
	{ 1, 5, 1, 2, 1 }, 			// 2
	{ 1, 1, 5, 1, 2 }, 			// 3
	{ 2, 1, 1, 5, 1 }, 			// 4
	{ 1, 1, 2, 1, 5 }, 			// 5
	{ 2, 4, 1, 2, 1 }, 			// 6
	{ 2, 1, 2, 1, 4 }, 			// 7
	{ 4, 2, 1, 2, 1 }, 			// 8
	{ 2, 1, 4, 1, 2 }, 			// 9
};

// Char level exp setting table
unsigned int gCharExpSet[CHAR_MAX_LEVEL] =
{
0,	45,	144,	369,	774,	1413,
2340,	3609,	5274,	7389,	10008,						// 10
13185,	16974,	21429,	26604,	32553,
39330,	46989,	55584,	65169,	75798,						// 20
87525,	100404,	114489,	129834,	146493,
164520,	183969,	204894,	227349,	251388,						// 30
277065,	304434,	333549,	364464,	397233,
431910,	468549,	507204,	547929,	590778,						// 40
635805,	683064,	732609,	784494,	838773,
895500,	954729,	1016514,	1080909,	1147968,			// 50
1217745,	1290294,	1365669,	1443924,	1525113,
1609290,	1696509,	1786824,	1880289,	1976958,	// 60
2076885,	2180124,	2286729,	2396754,	2510253,
2627280,	2747889,	2872134,	3000069,	3131748,	// 70
3267225,	3406554,	3549789,	3696984,	3848193,
4003470,	4162869,	4326444,	4494249,	4666338,	// 80
4842765,	5023584,	5208849,	5398614,	5592933,
5791860,	5995449,	6203754,	6416829,	6634728,	// 90
6857505,	7085214,	7317909,	7555644,	7798473,
8046450,	8299629,	8558064,	8821809,	9090918,	// 100
9365445,	9645444,	9930969,	10222074,	10518813,
10821240,	11129409,	11443374,	11763189,	12088908,	// 110
12420585,	12758274,	13102029,	13451904,	13807953,
14170230,	14538789,	14913684,	15294969,	15682698,	// 120
16076925,	16477704,	16885089,	17299134,	17719893,
18147420,	18581769,	19022994,	19471149,	19926288,	// 130
20388465,	20857734,	21334149,	21817764,	22308633,
22806810,	23312349,	23825304,	24345729,	24873678,	// 140
};

// Partner level exp setting table
unsigned int gPartnerExpSet[PARTNER_MAX_LEVEL] =
{
0,	222,	284,	387,	536,	732,
980,	1281,	1640,	2058,	2540,						// 10
3087,	3704,	4392,	5156,	5997,                              
6920,	7926,	9020,	10203,	11480,                      // 20
12852,	14324,	15897,	17576,	19362,                             
21260,	23271,	25400,	27648,	30020,                      // 30
32517,	35144,	37902,	40796,	43827,                             
47000,	50316,	53780,	57393,	61160,                      // 40
65082,	69164,	73407,	77816,	82392,                             
87140,	92061,	97160,	102438,	107900,                     // 50
113547,	119384,	125412,	131636,	138057,                            
144680,	151506,	158540,	165783,	173240,                     // 60
180912,	188804,	196917,	205256,	213822,                            
222620,	231651,	240920,	250428,	260180,                     // 70
270177,	280424,	290922,	301676,	312687,                            
323960,	335496,	347300,	359373,	371720,                     // 80
384342,	397244,	410427,	423896,	437652,                            
451700,	466041,	480680,	495618,	510860,                     // 90
526407,	542264,	558432,	574916,	591717,                            
608840,	626286,	644060,	662163,	680600,                     // 100
699372,	718484,	737937,	757736,	777882,                            
798380,	819231,	840440,	862008,	883940,                     // 110
906237,	928904,	951942,	975356,	999147,                            
1023320,	1047876,	1072820,	1098153,	1123880,    // 120
1150002,	1176524,	1203447,	1230776,	1258512,           
1286660,	1315221,	1344200,	1373598,	1403420,    // 130
1433667,	1464344,	1495452,	1526996,	1558977,           
1591400,	1624266,	1657580,	1691343,	1725560,    // 140
};

