//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/datacode.h"
//-- Self
#include "Stub/Logic/Account.h"
#include "Stub/Logic/CharacterInfo.h"
#include "WorldService/RankingControl.h"
#include "WorldService/Global.h"
#include "Proxy/Data/CharacterData.h"
#include "Proxy/Data/GuildData.h"
#include "Common/common_binary.h"
#include "Stub/Logic/RankCharacter.h"
#include "Stub/Logic/RankGuild.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/MapService_proxy.h"
#include "Stub/Service/WorldService.h"
#include "Resource/ResTitle.h"
#include "Resource/ServerResManager.h"

#define  LASTRANKING 1000

CRankingControl::CRankingControl()
{
	m_upgradeDiff = 0;
	m_upgradeCharID = 0;
	m_upgradeRankID = 0;
	m_upgradeRankSubID = 0;
	m_upgradeAccID = 0;
	m_currRank = 0;
	m_fristLevel = LASTRANKING;
	for (UInt8 i = 0; i < MAX_CHAR_RANK_TYPE; i++)
	{
		m_tmpRankGrp[i] = NULL;
	}
}
CRankingControl::~CRankingControl()
{
	m_upgradeName.Empty();
	m_char_ids.clear();
	m_title_ids.clear();
}

void CRankingControl::CalUpgradeChar(NetGroup * preRankGrp, NetGroup * currRankGrp, UInt8 rankType)
{
	if (preRankGrp == NULL ||
		currRankGrp == NULL)
		return;

	UInt subType = 1;
	for (int i = 0; i < currRankGrp->GetCount(); ++i)
	{
		UInt diff = 0;
		bool found = false;
		UInt firstOne = 0;
		
		CRankCharacter *pRkChar = ((NetGroup*) currRankGrp)->GetChild<CRankCharacter>(i);
		if (pRkChar)
		{
			UInt currSubType = 1;
			if (rankType  == CHAR_RANK_TYPE_CLASS)
			{
				currSubType = (pRkChar->GetCclass() > 0 ? 
						((pRkChar->GetCclass() - 1) * MAX_SEXTYPE + pRkChar->GetSex() + 1) : 0);
			}
			else if (rankType  == CHAR_RANK_TYPE_FACTION)
			{
				currSubType = pRkChar->GetFaction();
			}

			if (currSubType != subType)
			{
				subType = currSubType;
				for (int k = 0; k < currRankGrp->GetCount(); ++k)
				{
					CRankCharacter *pRkChar3 = ((NetGroup*) currRankGrp)->GetChild<CRankCharacter>(k);
					if (pRkChar3)
					{
						if (rankType == CHAR_RANK_TYPE_CLASS)
						{
							if (pRkChar3->GetSex() == pRkChar->GetSex() && 
								pRkChar3->GetCclass() == pRkChar->GetCclass())
							{
								firstOne = k;
								break;
							}
						}
						else
						{
							if (pRkChar3->GetFaction() == pRkChar->GetFaction())
							{
								firstOne = k;
								break;
							}
						}
					}
				}

			}
			for (int j = 0; j < preRankGrp->GetCount(); ++j)
			{
				CRankCharacter *pRkChar2 = ((NetGroup*) preRankGrp)->GetChild<CRankCharacter>(j);
				if (pRkChar2 && pRkChar2->GetChar_id() == pRkChar->GetChar_id())
				{
					found = true;
					if (j > i)
					{
						diff = j - i;
						break;
					}
				}
			}

			if (!found)
			{
				UInt lastOne = currRankGrp->GetCount();
				bool firstEncounter = false;
				if (rankType == CHAR_RANK_TYPE_CLASS ||
					rankType == CHAR_RANK_TYPE_FACTION)
				{
					for (int k = firstOne; k < currRankGrp->GetCount(); ++k)
					{
						CRankCharacter *pRkChar3 = ((NetGroup*) currRankGrp)->GetChild<CRankCharacter>(k);
						if (pRkChar3)
						{
							if (rankType == CHAR_RANK_TYPE_FACTION)
							{
								if (pRkChar3->GetSex() != pRkChar->GetSex() ||
									pRkChar3->GetCclass() != pRkChar->GetCclass())
								{
									lastOne = k + 1;
									break;
								}
							}
							else
							{
								if (pRkChar3->GetFaction() != pRkChar->GetFaction())
								{
									lastOne = k + 1;
									break;
								}
							}
						}
					}
				}

				if (lastOne > (UInt) i + 1)
					diff = lastOne - i - 1;
			}

			if (diff > m_upgradeDiff)
			{
				m_upgradeCharID = pRkChar->GetChar_id();
				m_upgradeAccID = pRkChar->GetAcct_id();
				m_upgradeName = pRkChar->GetNickName();
				m_upgradeRankID = rankType;
				m_currRank = i + 1 - firstOne;
				if (rankType == CHAR_RANK_TYPE_CLASS)
				{
					UInt8 SexClass = (pRkChar->GetCclass() > 0 ? 
						((pRkChar->GetCclass() - 1) * MAX_SEXTYPE + pRkChar->GetSex() + 1) : 0);
					m_upgradeRankSubID = SexClass ;
				}
				else if (rankType == CHAR_RANK_TYPE_FACTION)
					m_upgradeRankSubID = pRkChar->GetFaction();
				else
					m_upgradeRankSubID = 0;
				m_upgradeDiff = diff;
			}
		}
	}
}

void CRankingControl::GenerateRankTitle()
{
	m_char_ids.clear();
	m_title_ids.clear();
	for (UInt i = 0; i < MAX_CHAR_RANK_TYPE; i++)
	{
		UInt subTypeStart = 0;
		UInt subTypeEnd = 0;
		if (gGlobal.m_Rankings[i])
		{
			if (i == CHAR_RANK_TYPE_CLASS ||
				i == CHAR_RANK_TYPE_FACTION)
			{
				subTypeStart = 1;
				subTypeEnd = 8;
			}
			else
			{
				subTypeStart = 0;
				subTypeEnd = 0;
			}
			for (UInt subType = subTypeStart; subType <= subTypeEnd; subType ++)
			{
				//find no 1, 2, 3 
				for (int k = 0; k < gGlobal.m_Rankings[i]->GetCount(); ++k)
				{
					CRankCharacter *pRkChar = ((NetGroup*) gGlobal.m_Rankings[i])->GetChild<CRankCharacter>(k);

					//CRankCharacter *pRkChar = ((NetGroup*) gGlobal.m_Rankings[i])->GetChild<CRankCharacter>(j - 1);
					if (pRkChar)
					{
						bool suit = false;
						if (i == CHAR_RANK_TYPE_CLASS)
						{
							UInt8 SexClass = (pRkChar->GetCclass() > 0 ? 
								((pRkChar->GetCclass() - 1) * MAX_SEXTYPE + pRkChar->GetSex() + 1) : 0);
							suit = SexClass == subType;
						}
						else if (i == CHAR_RANK_TYPE_FACTION)
						{
							suit = pRkChar->GetFaction() == subType;
						}
						else
							suit = true;

						if (suit)
						{
							/*for (UInt j = 1; j <=3; j ++)
							{
								const TitleData * pTitleData = gGlobal.m_resource->GetTitleDataByRank(i, subType, j, j);
								if (pTitleData)
								{
									CRankCharacter *pRkChar2 = ((NetGroup*) gGlobal.m_Rankings[i])->GetChild<CRankCharacter>(k + j - 1);
									if (pRkChar2)
									{
										if (i == CHAR_RANK_TYPE_CLASS)
										{
											UInt8 SexClass = (pRkChar2->GetCclass() > 0 ? 
												((pRkChar2->GetCclass() - 1) * MAX_SEXTYPE + pRkChar2->GetSex() + 1) : 0);
											if (SexClass != subType)
												break;
										}
										else if (i == CHAR_RANK_TYPE_FACTION)
										{
											if (pRkChar2->GetFaction() != subType)
												break;
										}
										m_char_ids.push_back(pRkChar2->GetChar_id());
										m_title_ids.push_back(pTitleData->Title_id);
									}
									
								}
							}

							*/

							//find range rank
							for (UInt endRank = 1; endRank <= 100; endRank ++)
							{
								for (UInt startRank = 1; startRank <= endRank; startRank ++)
								{
									const TitleData * pTitleData = gGlobal.m_resource->GetTitleDataByRank(i, subType, startRank, endRank);
									if (pTitleData)
									{
										for (UInt j = startRank - 1; j < endRank; j ++)
										{
											CRankCharacter *pRkChar2 = ((NetGroup*) gGlobal.m_Rankings[i])->GetChild<CRankCharacter>(j + k);
											if (pRkChar2)
											{
												if (i == CHAR_RANK_TYPE_CLASS)
												{
													UInt8 SexClass = (pRkChar2->GetCclass() > 0 ? 
														((pRkChar2->GetCclass() - 1) * MAX_SEXTYPE + pRkChar2->GetSex() + 1) : 0);
													if (SexClass != subType)
														break;
												}
												else if (i == CHAR_RANK_TYPE_FACTION)
												{
													if (pRkChar2->GetFaction() != subType)
														break;
												}
												m_char_ids.push_back(pRkChar2->GetChar_id());
												m_title_ids.push_back(pTitleData->Title_id);
											}
											else
												break;									
										}
									}
								}
							}
							break;
						}
					}
				}
			}


		}
	}
	//send to messengers all map service 
	for (PeerSet::iterator ItrMap = gGlobal.m_mapPeers.begin(); ItrMap != gGlobal.m_mapPeers.end(); ++ItrMap)
	{
		MapService_Proxy::RankTitleUpdate(*ItrMap, RPCContext(), m_char_ids, m_title_ids);
	}

}

VOID CRankingControl::CreateCharacterRank()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CRankingControl, CreateCharacterRank, this, context)
}


RPCResult CRankingControl::CreateCharacterRank( RPCContext &context)
{
	PROCEDURE_START_METHOD(CRankingControl, CreateCharacterRank, this, context)

		m_upgradeDiff = 0;
	m_upgradeCharID = 0;
	m_upgradeRankID = 0;
	m_upgradeRankSubID = 0;
	m_upgradeAccID = 0;
	m_currRank = 0;
	ClearTempRank();

	UInt8 i = 0;
	UInt8 j = 0;

	PROCEDURE_FOR2(1, 3, i < MAX_CHAR_RANK_TYPE, UInt8, i, UInt8, j)

		TRACE_INFODTL_2(GLOBAL_LOGGER, _T("CreateCharacterRank: rankType %d, subType %d"),
		i, j)
		ObjectService_Proxy::GetCharacterRank(gGlobal.pObjectService->pPeer, 
		context, i, j);

	if ((i == CHAR_RANK_TYPE_CLASS || i == CHAR_RANK_TYPE_FACTION) && j < 7)
	{
		++j;		// next sub type
	}
	else
	{
		++i;		// next rank type
		j = 0;
	}
	PROCEDURE_STORE_COUNTER2(UInt8, i, UInt8, j)

		PROCEDURE_WAIT3(2, ObjectService, CB_GetCharacterRank, UInt8, rankType,
		UInt8, subType, NetGroup*, charGroup)

		TRACE_ENSURE(charGroup != NULL);

	TRACE_INFODTL_3(GLOBAL_LOGGER, _T("CB_GetCharacterRank: rankType %d, subType %d, count %d"),
		rankType, subType, charGroup->GetCount())

		if(rankType != CHAR_RANK_TYPE_LEVEL && rankType != CHAR_RANK_TYPE_MONEY && rankType != CHAR_RANK_TYPE_FAMOUS && rankType != CHAR_RANK_TYPE_EVIL)
		{
			if (subType == 0)
				m_tmpRankGrp[rankType] = CreateNetGroup();
			NetGroup* newGrp = m_tmpRankGrp[rankType];

			for (int i = 0; i < charGroup->GetCount(); ++i)
			{
				CCharacterData *pCharData = charGroup->GetChild<CCharacterData>(i);
				TRACE_ENSURE(pCharData != NULL);

				CRankCharacter * pRankChar = CRankCharacter::NewInstance();
				pRankChar->SetData(pCharData);
				newGrp->Add(pRankChar);
			}
			DeleteNetGroup(charGroup, BOOLEAN_TRUE);

			if (rankType != CHAR_RANK_TYPE_CLASS && rankType != CHAR_RANK_TYPE_FACTION || subType == 7)
			{
				CalUpgradeChar(gGlobal.m_Rankings[rankType], newGrp, rankType);
				if (gGlobal.m_Rankings[rankType])
					DeleteNetGroup(gGlobal.m_Rankings[rankType], BOOLEAN_TRUE);
				gGlobal.m_Rankings[rankType] = newGrp;
				m_tmpRankGrp[rankType] = NULL;

				if (rankType == CHAR_RANK_TYPE_PVPSCROE)
				{
					GenerateRankTitle();

					//call back to all client
					if (m_upgradeCharID != 0 && m_upgradeCharID != CHAR_RANK_TYPE_PKVALUE)
					{
						PeerSet proxySet;
						SessionIDVector sessionList;
						String rankGroup;
						RPCContext context2;

						//find the name
						rankGroup.Format(_T("MSG_RANK_GROUP_%d_%d"), m_upgradeRankID, m_upgradeRankSubID);
						gGlobal.GetInMapSessions(sessionList, proxySet);
						for (PeerSet::iterator it = proxySet.begin(); it != proxySet.end(); ++it)
							WorldService_Stub::CB_RankChangeMsg(*it, context2, rankGroup,
							m_upgradeName, m_currRank + m_upgradeDiff, m_currRank, sessionList);
					}
				}
			}
		}
		PROCEDURE_END_FOR(3, 1)

			TRACE_INFODTL(GLOBAL_LOGGER, _T("CreateCharacterRank: guild rank"));

		ObjectService_Proxy::GetGuildRank(gGlobal.pObjectService->pPeer, context);

		PROCEDURE_WAIT1(4, ObjectService, CB_GetGuildRank, NetGroup *, guildGroup)
			TRACE_ENSURE(guildGroup != NULL);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _T("CB_GetGuildRank: count %d"), guildGroup->GetCount())

			NetGroup * newGrp = CreateNetGroup();
		for (int i = 0; i < guildGroup->GetCount(); ++i)
		{
			CGuildData *pGuildData = ((NetGroup*) guildGroup)->GetChild<CGuildData>(i);
			TRACE_ENSURE(pGuildData != NULL);

			CRankGuild * pRankGuild = CRankGuild::NewInstance();
			pRankGuild->SetData(pGuildData);
			newGrp->Add(pRankGuild);
		}

		if (gGlobal.m_GuildRanks)
			DeleteNetGroup(gGlobal.m_GuildRanks, BOOLEAN_TRUE);
		gGlobal.m_GuildRanks = newGrp;
		DeleteNetGroup(guildGroup, BOOLEAN_TRUE);

		PROCEDURE_CATCH

			PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
			err, errMsg.c_str());

		PROCEDURE_EXCEPTION_DEFAULT
			ClearTempRank();

		PROCEDURE_END_CATCH

			return RPC_RESULT_OK; 
}

void CRankingControl::ClearTempRank()
{
	// delete all temp ranking groups
	for (UInt8 i = 0; i < MAX_CHAR_RANK_TYPE; i++)
	{
		if (m_tmpRankGrp[i])
			DeleteNetGroup(m_tmpRankGrp[i], BOOLEAN_TRUE);
	}
}

/*
RPCResult CRankingControl::CreateCharacterRank( RPCContext &context)
{
	PROCEDURE_START_METHOD(CRankingControl, CreateCharacterRank, this, context)

		m_upgradeDiff = 0;
		m_upgradeCharID = 0;
		m_upgradeRankID = 0;
		m_upgradeRankSubID = 0;
		m_upgradeAccID = 0;
		m_currRank = 0;
		
		ObjectService_Proxy::GetCharacterRank(gGlobal.pObjectService->pPeer, context, CHAR_RANK_TYPE_LEVEL);

	PROCEDURE_WAIT1(1, ObjectService, CB_GetCharacterRank, NetGroup *, charGroup)
			TRACE_ENSURE(charGroup != NULL);
			
			NetGroup * newGrp = CreateNetGroup();
			for (int i = 0; i < charGroup->GetCount(); ++i)
			{
				CCharacterData *pCharData = charGroup->GetChild<CCharacterData>(i);
				TRACE_ENSURE(pCharData != NULL);

				CRankCharacter * pRankChar = CRankCharacter::NewInstance();
				pRankChar->SetData(pCharData);
				newGrp->Add(pRankChar);
			}

			DeleteNetGroup(charGroup, BOOLEAN_TRUE);

			CalUpgradeChar(gGlobal.m_Rankings[CHAR_RANK_TYPE_LEVEL], newGrp, CHAR_RANK_TYPE_LEVEL);
			if (gGlobal.m_Rankings[CHAR_RANK_TYPE_LEVEL])
				DeleteNetGroup(gGlobal.m_Rankings[CHAR_RANK_TYPE_LEVEL], BOOLEAN_TRUE);

			gGlobal.m_Rankings[CHAR_RANK_TYPE_LEVEL] = newGrp;

		ObjectService_Proxy::GetCharacterRank(gGlobal.pObjectService->pPeer, context, CHAR_RANK_TYPE_MONEY);

		PROCEDURE_WAIT1(2, ObjectService, CB_GetCharacterRank, NetGroup *, charGroup)
			TRACE_ENSURE(charGroup != NULL);
			
			NetGroup * newGrp = CreateNetGroup();
			for (int i = 0; i < charGroup->GetCount(); ++i)
			{
				CCharacterData *pCharData = ((NetGroup*) charGroup)->GetChild<CCharacterData>(i);
				TRACE_ENSURE(pCharData != NULL);

				CRankCharacter * pRankChar = CRankCharacter::NewInstance();
				pRankChar->SetData(pCharData);
				newGrp->Add(pRankChar);
			}

			DeleteNetGroup(charGroup, BOOLEAN_TRUE);

			CalUpgradeChar(gGlobal.m_Rankings[CHAR_RANK_TYPE_MONEY], newGrp, CHAR_RANK_TYPE_MONEY);
			if (gGlobal.m_Rankings[CHAR_RANK_TYPE_MONEY])
				DeleteNetGroup(gGlobal.m_Rankings[CHAR_RANK_TYPE_MONEY], BOOLEAN_TRUE);

			gGlobal.m_Rankings[CHAR_RANK_TYPE_MONEY] = newGrp;

		ObjectService_Proxy::GetCharacterRank(gGlobal.pObjectService->pPeer, context, CHAR_RANK_TYPE_CLASS);

		PROCEDURE_WAIT1(3, ObjectService, CB_GetCharacterRank, NetGroup *, charGroup)
			TRACE_ENSURE(charGroup != NULL);
			
			NetGroup * newGrp = CreateNetGroup();
			for (int i = 0; i < charGroup->GetCount(); ++i)
			{
				CCharacterData *pCharData = ((NetGroup*) charGroup)->GetChild<CCharacterData>(i);
				TRACE_ENSURE(pCharData != NULL);

				CRankCharacter * pRankChar = CRankCharacter::NewInstance();
				pRankChar->SetData(pCharData);
				newGrp->Add(pRankChar);
			}

			DeleteNetGroup(charGroup, BOOLEAN_TRUE);

			CalUpgradeChar(gGlobal.m_Rankings[CHAR_RANK_TYPE_CLASS], newGrp, CHAR_RANK_TYPE_CLASS);
			if (gGlobal.m_Rankings[CHAR_RANK_TYPE_CLASS])
				DeleteNetGroup(gGlobal.m_Rankings[CHAR_RANK_TYPE_CLASS], BOOLEAN_TRUE);

			gGlobal.m_Rankings[CHAR_RANK_TYPE_CLASS] = newGrp;

		ObjectService_Proxy::GetCharacterRank(gGlobal.pObjectService->pPeer, context, CHAR_RANK_TYPE_FAMOUS);

		PROCEDURE_WAIT1(4, ObjectService, CB_GetCharacterRank, NetGroup *, charGroup)
			TRACE_ENSURE(charGroup != NULL);
			
			NetGroup * newGrp = CreateNetGroup();
			for (int i = 0; i < charGroup->GetCount(); ++i)
			{
				CCharacterData *pCharData = ((NetGroup*) charGroup)->GetChild<CCharacterData>(i);
				TRACE_ENSURE(pCharData != NULL);

				CRankCharacter * pRankChar = CRankCharacter::NewInstance();
				pRankChar->SetData(pCharData);
				newGrp->Add(pRankChar);
			}

			DeleteNetGroup(charGroup, BOOLEAN_TRUE);
			
			CalUpgradeChar(gGlobal.m_Rankings[CHAR_RANK_TYPE_FAMOUS], newGrp, CHAR_RANK_TYPE_FAMOUS);
			if (gGlobal.m_Rankings[CHAR_RANK_TYPE_FAMOUS])
				DeleteNetGroup(gGlobal.m_Rankings[CHAR_RANK_TYPE_FAMOUS], BOOLEAN_TRUE);

			gGlobal.m_Rankings[CHAR_RANK_TYPE_FAMOUS] = newGrp;


		ObjectService_Proxy::GetCharacterRank(gGlobal.pObjectService->pPeer, context, CHAR_RANK_TYPE_EVIL);

		PROCEDURE_WAIT1(5, ObjectService, CB_GetCharacterRank, NetGroup *, charGroup)
			TRACE_ENSURE(charGroup != NULL);
			
			NetGroup * newGrp = CreateNetGroup();
			for (int i = 0; i < charGroup->GetCount(); ++i)
			{
				CCharacterData *pCharData = ((NetGroup*) charGroup)->GetChild<CCharacterData>(i);
				TRACE_ENSURE(pCharData != NULL);

				CRankCharacter * pRankChar = CRankCharacter::NewInstance();
				pRankChar->SetData(pCharData);
				newGrp->Add(pRankChar);
			}

			DeleteNetGroup(charGroup, BOOLEAN_TRUE);
			
			CalUpgradeChar(gGlobal.m_Rankings[CHAR_RANK_TYPE_EVIL], newGrp, CHAR_RANK_TYPE_EVIL);
			if (gGlobal.m_Rankings[CHAR_RANK_TYPE_EVIL])
				DeleteNetGroup(gGlobal.m_Rankings[CHAR_RANK_TYPE_EVIL], BOOLEAN_TRUE);

			gGlobal.m_Rankings[CHAR_RANK_TYPE_EVIL] = newGrp;

		ObjectService_Proxy::GetCharacterRank(gGlobal.pObjectService->pPeer, context, CHAR_RANK_TYPE_FACTION);

		PROCEDURE_WAIT1(6, ObjectService, CB_GetCharacterRank, NetGroup *, charGroup)
			TRACE_ENSURE(charGroup != NULL);

			NetGroup * newGrp = CreateNetGroup();
			for (int i = 0; i < charGroup->GetCount(); ++i)
			{
				CCharacterData *pCharData = ((NetGroup*) charGroup)->GetChild<CCharacterData>(i);
				TRACE_ENSURE(pCharData != NULL);

				CRankCharacter * pRankChar = CRankCharacter::NewInstance();
				pRankChar->SetData(pCharData);
				newGrp->Add(pRankChar);
			}

			DeleteNetGroup(charGroup, BOOLEAN_TRUE);

			CalUpgradeChar(gGlobal.m_Rankings[CHAR_RANK_TYPE_FACTION], newGrp, CHAR_RANK_TYPE_FACTION);
			if (gGlobal.m_Rankings[CHAR_RANK_TYPE_FACTION])
				DeleteNetGroup(gGlobal.m_Rankings[CHAR_RANK_TYPE_FACTION], BOOLEAN_TRUE);

			gGlobal.m_Rankings[CHAR_RANK_TYPE_FACTION] = newGrp;

		ObjectService_Proxy::GetCharacterRank(gGlobal.pObjectService->pPeer, context, CHAR_RANK_TYPE_PVPSCROE);

		PROCEDURE_WAIT1(7, ObjectService, CB_GetCharacterRank, NetGroup *, charGroup)
			TRACE_ENSURE(charGroup != NULL);
			
			NetGroup * newGrp = CreateNetGroup();
			for (int i = 0; i < charGroup->GetCount(); ++i)
			{
				CCharacterData *pCharData = ((NetGroup*) charGroup)->GetChild<CCharacterData>(i);
				TRACE_ENSURE(pCharData != NULL);

				CRankCharacter * pRankChar = CRankCharacter::NewInstance();
				pRankChar->SetData(pCharData);
				newGrp->Add(pRankChar);
			}

			DeleteNetGroup(charGroup, BOOLEAN_TRUE);
			
			CalUpgradeChar(gGlobal.m_Rankings[CHAR_RANK_TYPE_PVPSCROE], newGrp, CHAR_RANK_TYPE_PVPSCROE);
			if (gGlobal.m_Rankings[CHAR_RANK_TYPE_PVPSCROE])
				DeleteNetGroup(gGlobal.m_Rankings[CHAR_RANK_TYPE_PVPSCROE], BOOLEAN_TRUE);

			gGlobal.m_Rankings[CHAR_RANK_TYPE_PVPSCROE] = newGrp;

			GenerateRankTitle();

			//call back to all client
			if (m_upgradeCharID != 0 && m_upgradeCharID != CHAR_RANK_TYPE_PKVALUE)
			{
				PeerSet proxySet;
				SessionIDVector sessionList;
				String rankGroup;
				RPCContext context2;

				//find the name
				rankGroup.Format(_T("MSG_RANK_GROUP_%d_%d"), m_upgradeRankID, m_upgradeRankSubID);
				gGlobal.GetInMapSessions(sessionList, proxySet);
				for (PeerSet::iterator it = proxySet.begin(); it != proxySet.end(); ++it)
					WorldService_Stub::CB_RankChangeMsg(*it, context2, rankGroup,
						m_upgradeName, m_currRank + m_upgradeDiff, m_currRank, sessionList);
			}

			ObjectService_Proxy::GetCharacterRank(gGlobal.pObjectService->pPeer, context, CHAR_RANK_TYPE_PKVALUE);

			PROCEDURE_WAIT1(8, ObjectService, CB_GetCharacterRank, NetGroup *, charGroup)
				TRACE_ENSURE(charGroup != NULL);

			NetGroup * newGrp = CreateNetGroup();
			for (int i = 0; i < charGroup->GetCount(); ++i)
			{
				CCharacterData *pCharData = ((NetGroup*) charGroup)->GetChild<CCharacterData>(i);
				TRACE_ENSURE(pCharData != NULL);

				CRankCharacter * pRankChar = CRankCharacter::NewInstance();
				pRankChar->SetData(pCharData);
				newGrp->Add(pRankChar);
			}

			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("GetCharacterRank CHAR_RANK_TYPE_PKVALUE count:  %d"),
				charGroup->GetCount());

			DeleteNetGroup(charGroup, BOOLEAN_TRUE);

			if (gGlobal.m_Rankings[CHAR_RANK_TYPE_PKVALUE])
				DeleteNetGroup(gGlobal.m_Rankings[CHAR_RANK_TYPE_PKVALUE], BOOLEAN_TRUE);

			gGlobal.m_Rankings[CHAR_RANK_TYPE_PKVALUE] = newGrp;

			ObjectService_Proxy::GetGuildRank(gGlobal.pObjectService->pPeer, context);

		PROCEDURE_WAIT1(9, ObjectService, CB_GetGuildRank, NetGroup *, guildGroup)
			TRACE_ENSURE(guildGroup != NULL);

			NetGroup * newGrp = CreateNetGroup();
			for (int i = 0; i < guildGroup->GetCount(); ++i)
			{
				CGuildData *pGuildData = ((NetGroup*) guildGroup)->GetChild<CGuildData>(i);
				TRACE_ENSURE(pGuildData != NULL);

				CRankGuild * pRankGuild = CRankGuild::NewInstance();
				pRankGuild->SetData(pGuildData);
				newGrp->Add(pRankGuild);
			}

			if (gGlobal.m_GuildRanks)
				DeleteNetGroup(gGlobal.m_GuildRanks, BOOLEAN_TRUE);

			gGlobal.m_GuildRanks = newGrp;

			DeleteNetGroup(guildGroup, BOOLEAN_TRUE);


		PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CreateRanking Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
	PROCEDURE_END_CATCH
	
		return RPC_RESULT_OK; 
}
*/

VOID CRankingControl::CreateRankByID(UInt8 rankType)
{
	RPCContext context;
	PROCEDURE_DO_METHOD2(CRankingControl, CreateAssignRank, this, context, UInt8,rankType,UInt8,0)
}

RPCResult CRankingControl::CreateAssignRank(RPCContext &context)
{
	PROCEDURE_START_METHOD2(CRankingControl, CreateAssignRank, this, context, UInt8,rankType,UInt8,subType)

		m_upgradeDiff = 0;
	m_upgradeCharID = 0;
	m_upgradeRankID = 0;
	m_upgradeRankSubID = 0;
	m_upgradeAccID = 0;
	m_currRank = 0;
	m_fristLevel = LASTRANKING;
	ClearTempRank();

	TRACE_INFODTL_2(GLOBAL_LOGGER, _T("CreateCharacterRank: rankType %d, subType %d"),
		rankType, subType)
		ObjectService_Proxy::GetCharacterRank(gGlobal.pObjectService->pPeer, 
		context, rankType, subType);
	PROCEDURE_WAIT3(2, ObjectService, CB_GetCharacterRank, UInt8, rank,
		UInt8, sub, NetGroup*, charGroup)

		TRACE_ENSURE(charGroup != NULL);

	TRACE_INFODTL_3(GLOBAL_LOGGER, _T("CB_GetCharacterRank: rankType %d, subType %d, count %d"),
		rank, sub, charGroup->GetCount())

		if (subType == 0)
			m_tmpRankGrp[rankType] = CreateNetGroup();
	NetGroup* newGrp = m_tmpRankGrp[rankType];

	for (int i = 0; i < charGroup->GetCount(); ++i)
	{
		CCharacterData *pCharData = charGroup->GetChild<CCharacterData>(i);
		TRACE_ENSURE(pCharData != NULL);

		CRankCharacter * pRankChar = CRankCharacter::NewInstance();
		pRankChar->SetData(pCharData);
		newGrp->Add(pRankChar);
	}
	DeleteNetGroup(charGroup, BOOLEAN_TRUE);

	CalNewUpgradeChar(gGlobal.m_Rankings[rankType], newGrp, rankType);
	if (gGlobal.m_Rankings[rankType])
		DeleteNetGroup(gGlobal.m_Rankings[rankType], BOOLEAN_TRUE);
	gGlobal.m_Rankings[rankType] = newGrp;
	m_tmpRankGrp[rankType] = NULL;


	PeerSet proxySet;
	SessionIDVector sessionList;
	String rankGroup;
	RPCContext context2;

	//find the name
	rankGroup.Format(_T("MSG_RANK_GROUP_%d_%d"), rankType, subType);
	gGlobal.GetInMapSessions(sessionList, proxySet);
	for (PeerSet::iterator it = proxySet.begin(); it != proxySet.end(); ++it)
	{
		if(m_fristLevel == LASTRANKING)
		{
			WorldService_Stub::CB_RankChangeMsg(*it, context2, rankGroup,
				m_upgradeName, 0, 0, sessionList);
		}
		else
		{
			WorldService_Stub::CB_RankChangeMsg(*it, context2, rankGroup,
				m_upgradeName, m_currRank + m_upgradeDiff, m_currRank, sessionList);
		}
	}

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _T("CreateAssignRank Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());
	PROCEDURE_END_CATCH
		return RPC_RESULT_OK;
}

void CRankingControl::CalNewUpgradeChar(NetGroup * preRankGrp, NetGroup * currRankGrp, UInt8 rankType)
{
	if (preRankGrp == NULL ||
		currRankGrp == NULL)
		return;

	for (int i = 0; i < currRankGrp->GetCount(); ++i)
	{
		UInt diff = 0;
		bool found = false;

		CRankCharacter *pRkChar = ((NetGroup*) currRankGrp)->GetChild<CRankCharacter>(i);
		if (pRkChar)
		{
			for (int j = 0; j < preRankGrp->GetCount(); ++j)
			{
				CRankCharacter *pRkChar2 = ((NetGroup*) preRankGrp)->GetChild<CRankCharacter>(j);
				if (pRkChar2 && pRkChar2->GetChar_id() == pRkChar->GetChar_id())
				{
					found = true;
					if (j > i && m_fristLevel > i)
					{
						diff = j - i;
						m_fristLevel = i;
						break;
					}
				}
				TRACE_INFODTL_6(GLOBAL_LOGGER, _T("CalNewUpgradeChar:New CharId = %d, GetNew Rank = %d,OldCharID = %d,Old Rand = %d,i = %d,j = %d"), 
					pRkChar->GetChar_id(),pRkChar->GetLevel(),pRkChar2->GetChar_id(),pRkChar2->GetLevel(),i,j)
			}

			if(!found)
			{
				UInt lastOne = currRankGrp->GetCount();
				if (lastOne > (UInt) i + 1)
					diff = lastOne - i - 1;
				if(m_fristLevel > i)
					m_fristLevel = i;
			}

			if (diff > m_upgradeDiff && m_fristLevel < LASTRANKING)
			{
				m_upgradeCharID = pRkChar->GetChar_id();
				m_upgradeAccID = pRkChar->GetAcct_id();
				m_upgradeName = pRkChar->GetNickName();
				m_upgradeRankID = rankType;
				m_currRank = i + 1 ;

				m_upgradeRankSubID = 0;
				m_upgradeDiff = diff;
			}
		}
	}
	if (m_fristLevel == LASTRANKING)
	{
		CRankCharacter *pRkCharOld = ((NetGroup*) currRankGrp)->GetChild<CRankCharacter>(0);
		if(pRkCharOld)
			m_upgradeName = pRkCharOld->GetNickName();
		m_upgradeDiff = 0;
	}
}
