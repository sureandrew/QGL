#include "Common.h"
#include "Common/common_binary.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/PetGroup.h"
#include "Proxy/Logic/Skill.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Proxy/Logic/Character.h"
#include "Proxy/Service/MapService_proxy.h"
#ifdef QGL
#include "QGL/Global.h"
#include "QGL/MenuPetProcess.h"
#include "QGL/MenuItemProcess.h"
#include "QGL/MenuHawkerProcess.h"
#include "QGL/CharacterControl.h"
#include "QGL/ChannelControl.h"
#include "Resource/ResPet.h"
#include "Resource/ClientResManager.h"
#endif // QGL
REGISTER_CLASSTYPE(CPet, Pet_CLASSID);

CPet::CPet()
{
	m_pSkillGrp = NULL;
	m_bGiven = false;
}

CPet::~CPet()
{
	m_pSkillGrp = NULL;
}

RPCResult CPet::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPet CB_Error: errcode %d: %s"), errCode, errMsg.c_str());

#ifdef QGL
	//StringA textA(errMsg.c_str());
	String text(gGlobal.GetStringTable()->Get(errMsg.c_str()));
	String empty;
	if (!text.Equals(empty.c_str()))
		gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
		
#endif // QGL

	return RPC_RESULT_OK;
}
void CPet::SetSkillGroup(CSkillGroup *grp)
{
	if (grp == NULL)
		return;
#ifdef QGL

	if (m_pSkillGrp && m_pSkillGrp != grp)
		gGlobal.g_PetLst->RemoveSkillGroup(GetPet_uid());

	m_pSkillGrp = grp;
	m_pSkillGrp->SetOwnerType(SKILL_OWNER_TYPE_PET);
	
	if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
		gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
#endif // QGL
}


void CPet::OnUpdateId(const UInt32 &pet_uid, const UInt32 &pet_id)
{
}

void CPet::OnUpdateOwner(const UInt32 &pet_master)
{
}

void CPet::OnUpdateInfo(const String &pet_Name)
{
	#ifdef QGL
	gGlobal.g_pMenuPetProcess->RefreshRightUpPanel();
	if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pItemProcess->UpdateItemPet(gGlobal.g_pItemProcess->GetMenuSelectedPetIndex());
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPet();
	#endif // QGL
}

void CPet::OnUpdateLevel(const UInt16 &pet_lv)
{
	#ifdef QGL
	if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
	gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pItemProcess->UpdateItemPet(gGlobal.g_pItemProcess->GetMenuSelectedPetIndex() - gGlobal.g_pItemProcess->GetMenuStartIndex());
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPet();

	if (gGlobal.g_PetLst != NULL)
	{
		if (GetState() == PET_STATE_TYPE_USE)
		{
			//CChannelControl * pChannelCtrl = gGlobal.GetChannelControl(CHANNELTYPE_MAP);
			//if (pChannelCtrl)
			//{
			//	pChannelCtrl->SendEmotion(PET_EMOTION_HAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true);
			//}
			MapService_Proxy::SendMapEmotion(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_pCharacter->GetCharNetObject()->GetNetId(), 
				PET_EMOTION_HAPPY, gGlobal.g_pCharacter->GetCharNetObject()->GetNetId(), true);
		}
	}
	#endif // QGL
}

void CPet::OnUpdateExp(const UInt32 &pet_exp)
{
	#ifdef QGL
	if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
	gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	#endif // QGL
}

void CPet::OnUpdateState(const UInt16 &pet_State)
{
	#ifdef QGL
	gGlobal.g_pMenuPetProcess->RefreshRightUpPanel();
	if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pItemProcess->UpdateItemPet(gGlobal.g_pItemProcess->GetMenuSelectedPetIndex() - gGlobal.g_pItemProcess->GetMenuStartIndex());
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPet();
	
	if (pet_State == PET_STATE_TYPE_USE)
	{

		if (gGlobal.g_PetLst != NULL)
		{
			/*
			if (GetHappiness() < 50 )
				gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_UNHAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
			else if (GetHappiness() < 70 )
				gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_BORING, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
			else
				gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HAPPY2, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
		*/
			const PetData * pPetData = gGlobal.m_resource->GetPetData(GetPet_id());
			if(pPetData && pPetData->pet_type == 0)
			{
				UInt8 PetFavor = GetHappiness()/10;
				if (PetFavor > 8)
					PetFavor = 8;
				else if (PetFavor < 2)
					PetFavor = 2;
				if (PetFavor == 2)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HUNGRY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetFavor == 3)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_ANGRY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetFavor  == 4)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_UNHAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetFavor  == 5)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_BORING, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetFavor  == 6)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_NAU, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetFavor  == 6)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetFavor == 8)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HAPPY2, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
			}
			else if(pPetData && pPetData->pet_type == 1)
			{
				UInt8 PetVP = GetVP()/10;
				if (PetVP > 8)
					PetVP = 8;
				else if (PetVP < 2)
					PetVP = 2;
				if (PetVP == 2)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HUNGRY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 3)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_ANGRY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 4)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_UNHAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 5)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_BORING, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 6)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_NAU, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 7)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 8)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HAPPY2, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
			}
		}
		else
		{
			if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
			{
				time_t now;
				time(&now);
				if (now - gGlobal.g_pCharacter->GetCharNetObject()->GetOfflineTime() >= 3600)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_UNHAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HAPPY2, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
			}
		}
	}

	#endif // QGL
}

void CPet::OnUpdateFather(const UInt32 &father_id, const String &father_name)
{
	#ifdef QGL
	if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
	gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	#endif // QGL
}

void CPet::OnUpdateMother(const UInt32 &mother_id, const String &mother_name)
{
	#ifdef QGL
	if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
	gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	#endif // QGL
}

void CPet::OnUpdateBirth(const UInt16 &birthCount)
{
	#ifdef QGL
	
	#endif // QGL
}

void CPet::OnUpdateFirstMaster(const String &pet_firstmaster_name)
{
	#ifdef QGL
	if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
	gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	#endif // QGL
}

void CPet::OnUpdateHappiness(const UInt16 &pet_Happiness)
{
	#ifdef QGL
	if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
	gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pItemProcess->UpdateItemPet(gGlobal.g_pItemProcess->GetMenuSelectedPetIndex() - gGlobal.g_pItemProcess->GetMenuStartIndex());
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPet();
	const PetData * pPetData = gGlobal.m_resource->GetPetData(GetPet_id());
	if(pPetData && pPetData->pet_type == 0)
	{
		UInt8 PetFavor = GetHappiness()/10;
		if (PetFavor > 8)
			PetFavor = 8;
		else if (PetFavor < 2)
			PetFavor = 2;
		if (PetFavor == 2)
			gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HUNGRY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
		else if (PetFavor == 3)
			gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_ANGRY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
		else if (PetFavor  == 4)
			gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_UNHAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
		else if (PetFavor  == 5)
			gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_BORING, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
		else if (PetFavor  == 6)
			gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_NAU, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
		else if (PetFavor  == 6)
			gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
		else if (PetFavor == 8)
			gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HAPPY2, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
	}
	#endif // QGL
}

void CPet::OnUpdateSpirit(const UInt16 &spirit)
{
	#ifdef QGL
		if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
			gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	#endif // QGL
}
void CPet::OnUpdateVP(const UInt32 &pet_VP, const UInt32 &pet_maxVP)
{
	#ifdef QGL
	if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
	gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pItemProcess->UpdateItemPet(gGlobal.g_pItemProcess->GetMenuSelectedPetIndex() - gGlobal.g_pItemProcess->GetMenuStartIndex());
	if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pHawkerProcess->UpdateOwnerPet();

	if (GetState() == PET_STATE_TYPE_USE)
	{

		if (gGlobal.g_PetLst != NULL)
		{/*
			if (this->GetVP() == 0)
				gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_UNHAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
			else if (this->GetVP() <= 30)
				gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HUNGRY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
			else
			{
				if (GetHappiness() == 100 )
				{
					if (rand() % 100 < 50)
						gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_BORING, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				}
				else
				{
					UInt num = rand() % 100;
					if (num >= 40 && num < 70)
						gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_NAU, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
					else if (num >= 70)
						gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HAPPY2, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
					
				}
			}*/
			const PetData * pPetData = gGlobal.m_resource->GetPetData(GetPet_id());
			if(pPetData &&	pPetData->pet_type == 1)
			{
				UInt8 PetVP = GetVP()/10;
				if (PetVP > 8)
					PetVP = 8;
				else if (PetVP < 2)
					PetVP = 2;
				if (PetVP == 2)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HUNGRY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 3)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_ANGRY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 4)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_UNHAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 5)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_BORING, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 6)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_NAU, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 7)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HAPPY, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
				else if (PetVP == 8)
					gGlobal.g_EmotionList.insert(std::make_pair(CHANNELTYPE_GLOBAL, emotionData(PET_EMOTION_HAPPY2, gGlobal.g_pCharacter->GetMapActorID(), gGlobal.g_pCharacter->GetMapActorID(), true)));
			}
		}
	}
	#endif // QGL
}

void CPet::OnUpdateElement(const UInt8 &pet_element)
{
	#ifdef QGL
	if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
	gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	#endif // QGL
}

void CPet::OnUpdateExpireDate(const UInt32 &pet_ExpireDate)
{
	#ifdef QGL
	if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
	gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	#endif // QGL
}

void CPet::OnUpdateSoulbound(const bool &pet_soulbound_state)
{
}

void CPet::OnUpdateOutlook(const UInt16 &pet_color)
{
	#ifdef QGL
	if (GetPet_uid() == gGlobal.g_pMenuPetProcess->m_currPetUid)
	gGlobal.g_pMenuPetProcess->ShowSelectedInfo();
	#endif // QGL
}

void CPet::OnUpdateDepute(const UInt32 &pet_depute_Master, const UInt32 &pet_depute_Time, const UInt32 &pet_depute_UseTime, const UInt32 &pet_depute_Exp)
{
}


void CPet::OnUpdateAssetLock(const Boolean &lock)
{
	#ifdef QGL
	gGlobal.g_pMenuPetProcess->RefreshMenu();
	if (gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_PET_MENU)
		gGlobal.g_pItemProcess->UpdateItemPetNameList();
	#endif // QGL
}


RPCResult CPet::CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CPet::CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID)
{
	return RPC_RESULT_OK;
}
RPCResult CPet::CB_ChangePetName(LPCPEER pPeer, RPCContext &context, const bool &success)
{
	return RPC_RESULT_OK;
}