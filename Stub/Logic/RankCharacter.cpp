#include "Common.h"

#include "Stub/Logic/RankCharacter.h"
#include "Proxy/Data/CharacterData.h"
#include "Common/datacode.h"
REGISTER_CLASSTYPE(CRankCharacter, RankCharacter_CLASSID);

CRankCharacter::CRankCharacter()
{
}

CRankCharacter::~CRankCharacter()
{
}

void CRankCharacter::SetData(CCharacterData *pCharData)
{
	if (pCharData->IsUpdated(CCharacterData::idMask)) {
		SetId(pCharData->GetAcct_id(), pCharData->GetChar_id());
	}

	if (pCharData->IsUpdated(CCharacterData::infoMask)) {
		UInt8 SexClass = (pCharData->GetCclass() > 0 ? 
			((pCharData->GetCclass() - 1) * MAX_SEXTYPE + pCharData->GetSex() + 1) : 0);

		SetInfo(pCharData->GetNickName(), pCharData->GetSex(),
				pCharData->GetCclass(), SexClass, 
				 pCharData->GetFaction());
	}

	if (pCharData->IsUpdated(CCharacterData::levelInfoMask)) {
		SetLevel(pCharData->GetLevel());
	}

	if (pCharData->IsUpdated(CCharacterData::moneyMask)) {
		SetMoney(pCharData->GetMoney());
	}

	if (pCharData->IsUpdated(CCharacterData::bankMoneyMask)) {
		SetBankMoney(pCharData->GetBankMoney());
	}

	if (pCharData->IsUpdated(CCharacterData::honorMask)) {
		SetHonor(pCharData->GetHonor());
	}

	if (pCharData->IsUpdated(CCharacterData::PKInfoMask)) {
		SetPKInfo(pCharData->GetPK_Value(),pCharData->GetWantedReward());
	}

	if (pCharData->IsUpdated(CCharacterData::wantedRewardMask)) {
		SetWantedReward(pCharData->GetWantedReward());
	}

	if (pCharData->IsUpdated(CCharacterData::PvPScoreSingleMask)) {
		SetPvPScoreSingle(pCharData->GetPvPScoreSingle());
	}
	TRACE_INFODTL_1(GLOBAL_LOGGER,_F("CRankCharacter::SetData pCharData->GetWantedReward():%d"),pCharData->GetWantedReward());

	
}

void CRankCharacter::PreSend()
{
	SetSendMask(AllMask);
}