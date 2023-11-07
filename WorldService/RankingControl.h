#pragma once
#ifndef _RANK_CONTROL_H_
#define _RANK_CONTROL_H_

#include "common_binary.h"
#include "Reuben/Simulation/Object.h"

class CRankingControl : 
	public Reuben::Simulation::Object
{
public:
	CRankingControl();
	~CRankingControl();

	VOID CreateCharacterRank();
	RPCResult CreateCharacterRank( RPCContext &context);

	VOID CreateRankByID(UInt8 rankType);
	RPCResult CreateAssignRank( RPCContext &context);

	void CalUpgradeChar(NetGroup * preRankGrp, NetGroup * currRankGrp, UInt8 rankType);
	void GenerateRankTitle();

	void CalNewUpgradeChar(NetGroup * preRankGrp, NetGroup * currRankGrp, UInt8 rankType);

	void ClearTempRank();

	DEFINE_PARENT_CLASS(Reuben::Simulation::Object);
	DEFINE_CLASS(0);			// don't care class id

	UInt32	m_upgradeCharID;
	UInt32	m_upgradeAccID;
	String 	m_upgradeName;
	UInt32	m_upgradeRankID;
	UInt32	m_upgradeRankSubID;
	UInt32	m_upgradeDiff;
	UInt32	m_currRank;
	UInt32  m_fristLevel;

	UInt32Vector m_char_ids;
	UInt16Vector m_title_ids;

	NetGroup*	m_tmpRankGrp[MAX_CHAR_RANK_TYPE];
};
#endif // _RANK_CONTROL_H_