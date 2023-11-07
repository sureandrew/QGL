//-- Common
#include "Common.h"
#include "Common/DataCode.h"
//-- Self
#include "Stub/Logic/BatItem.h"
//-- Library
#include "Proxy/Data/ItemData.h"
#include "Proxy/Service/ObjectService.h"
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/BatCharacter.h"
#include "Stub/Logic/BatMob.h"
#include "BattleService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResItem.h"
#include "Resource/ResMob.h"

#define BAG_MAX_ITEM	120

REGISTER_CLASSTYPE(CBatItem, BatItem_CLASSID);

PROTO_LuaPush(CBatActor);
PROTO_LuaPush(CBatItem);

CBatItem::CBatItem()
{
	m_data = NULL;
	m_tryRemain = 0;
	m_tryStack = 0;
}

CBatItem::~CBatItem()
{
	m_tryRemain = 0;
}

UInt8 CBatItem::GetEquipIndex()
{
	if (m_data == NULL || m_data->item_type != ITEMTYPE_EQUIP)
		return 0;

	UInt8 ei = 0;

	if (m_data->eqType < EQTYPE_GEAR)
		ei = EQPLACE_WEAPON + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_GEAR)
		ei = EQPLACE_GEAR + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_ARMOR)
		ei = EQPLACE_ARMOR + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_SHOE)
		ei = EQPLACE_SHOE + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_BELT)
		ei = EQPLACE_BELT + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_RING)
		ei = EQPLACE_RING + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_AMULET)
		ei = EQPLACE_AMULET + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_CLOTH)
		ei = EQPLACE_CLOTH + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_HAT_WITH_HAIR)
		ei = EQPLACE_HEAD_ACCESSORY + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_HAT_WITHOUT_HAIR)
		ei = EQPLACE_HEAD_ACCESSORY + BAG_MAX_ITEM;

	return ei;
}

Boolean CBatItem::AllowSexClass(UInt8 sex, UInt8 cclass)
{
	if (!m_data) return BOOLEAN_FALSE;

	UInt8 sexClass = (cclass > 0 ? ((cclass - 1) * 2 + sex + 1) : 100 + sex);
	return (m_data->reqSexClass1 == SEXCLASS_NONE || 
			sex + 100 == m_data->reqSexClass1 || sexClass == m_data->reqSexClass1 ||
			sex + 100 == m_data->reqSexClass2 || sexClass == m_data->reqSexClass2);
}

Boolean CBatItem::CanUse(CBatActor* pUser, CBatActor* pTarget, String &errMsg)
{
	if (pUser == NULL || m_data == NULL) {
		errMsg.Format(_T("item_uid %d null user or data"), GetItem_uid());
		return BOOLEAN_FALSE;
	}

	if (pTarget == NULL || !pTarget->IsValidTarget()) {
		errMsg.Format(_T("item_uid %d null or invalid target"), GetItem_uid());
		return BOOLEAN_FALSE;
	}

	// is block?
	if (pUser->CheckBlockItem(m_data->item_type))
	{
		pUser->CB_BattleMsg(pUser->m_pClient->pPeer, pUser->m_pClient->context, BATTLE_ERROR_ITEMBLOCK, 
			String(), pUser->GetSubType());
		return BOOLEAN_FALSE;
	}
	// can use in battle?
	if (m_data->usePlace != 0 && m_data->usePlace != FIELD_BATTLE) {
		errMsg.Format(_T("item_uid %d, item_id %d cannot use in battle"), GetItem_uid(),
			GetItem_id());
		return BOOLEAN_FALSE;
	}

	// use count > 0
	if (m_data->useCount == 0 || GetStack() == 0 || GetRemain() == 0) {
		errMsg.Format(_T("item_uid %d no more stack %d or remain %d"), GetItem_uid(),
			GetStack(), GetRemain());
		return BOOLEAN_FALSE;
	}

	// valid form
	if (m_data->form == ITEMFORM_RESURRECT && pTarget->GetState() != CHAR_ST_DEAD ||
		m_data->form != ITEMFORM_RESURRECT && pTarget->GetState() == CHAR_ST_DEAD)
	{
		errMsg.Format(_T("item_uid %d, item_id %d non-resurrect/resurrect on wrong target"),
			GetItem_uid(), GetItem_id());
		return BOOLEAN_FALSE;
	}

	CBatCharacter *pCharUser = CastNetObject<CBatCharacter>(pUser);
	CBatMob *pMobUser = CastNetObject<CBatMob>(pUser);
	CBatCharacter *pCharTarget = CastNetObject<CBatCharacter>(pTarget);
	CBatMob *pMobTarget = CastNetObject<CBatMob>(pTarget);

	// valid target type
	if (/*(m_data->targetType == ITEM_TTYPE_SELF && pUser != pTarget) ||
		(m_data->targetType == ITEM_TTYPE_PARTNER && (pMobTarget == NULL || pCharUser == NULL || pMobTarget->GetOwner_id() != pCharUser->GetChar_id())) ||
		(m_data->targetType == ITEM_TTYPE_SELF_P && pUser != pTarget && (pMobTarget == NULL || pCharUser == NULL || pMobTarget->GetOwner_id() != pCharUser->GetChar_id())) ||
		(m_data->targetType == ITEM_TTYPE_US && pUser->GetTeam() != pTarget->GetTeam()) ||
		(m_data->targetType == ITEM_TTYPE_THEM && pUser->GetTeam() == pTarget->GetTeam())*/
		!gGlobal.IsValidTargetType(m_data->targetType, pUser, pTarget))
	{
		errMsg.Format(_T("item_uid %d, item_id %d invalid target type %d, target %d"), GetItem_uid(), 
			GetItem_id(), m_data->targetType, pTarget->GetIndex());
		return BOOLEAN_FALSE;
	}
	
	// fulfill all requirement?
	if (m_data->reqFaction != 0 && m_data->reqFaction != pUser->GetFaction() ||	// req faction
		m_data->reqLv > pUser->GetLevel() ||						// req level
		!AllowSexClass(pUser->GetSex(), pUser->GetCclass()))		// req sex and class
	{
		errMsg.Format(_T("item_uid %d, item_id %d requirement not fulfill"), GetItem_uid(), 
			GetItem_id());
		return BOOLEAN_FALSE;
	}

	//check lua
	if (m_data)
	{
		StringA luaName = m_data->battleScript;
		if (!luaName.IsEmpty())
		{
			pUser->ClearReturnType();
			size_t pos = luaName.find(".");
			if (pos != std::string::npos)
			{
				luaName.Format("%s%s", luaName.substr(0, pos).c_str(), "CanUse");
				CallLuaFunc2(gGlobal.m_luaState, luaName.c_str(), pUser, pTarget);
				if (pUser->GetReturnType() > 0 && pUser->m_pClient->pPeer)
				{
					pUser->CB_BattleMsg(pUser->m_pClient->pPeer, pUser->m_pClient->context, BATTLE_ERROR_MSG_GIVEN, 
						pUser->m_Msg, pUser->GetSubType());
					return BOOLEAN_FALSE;
				}
			}
		}
	}
	return BOOLEAN_TRUE;
}

Boolean CBatItem::UseOn(CBatActor* pUser, CBatActor* pTarget, String &errMsg,
						Boolean bTry)
{
	if (!CanUse(pUser, pTarget, errMsg))
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _T("BatItem UseOn Warning: %s"), errMsg.c_str());
		return BOOLEAN_FALSE;
	}

	if (bTry && m_tryRemain != 255) {
		// mark try use and reduce use count or stack count
		m_tryRemain--;							// reduce use count
		if (m_tryRemain == 0) {
			m_tryStack--;						// reduce stack
			if (m_tryStack > 0) {				// still here after use
				m_tryRemain = m_data->useCount;	// reset use count
			}
		}
		return BOOLEAN_TRUE;
	}

	StringA luaName = m_data->battleScript;
	if (!luaName.IsEmpty())
	{
		pUser->ClearReturnType();
		size_t pos = luaName.find(".");
		if (pos != std::string::npos)
		{
			luaName.Format("%s%s", luaName.substr(0, pos).c_str(), "Use");
			CallLuaFunc3(gGlobal.m_luaState, luaName.c_str(), pUser, pTarget, this);
			if (!pUser->m_Msg.IsEmpty() && pUser->m_pClient->pPeer)
			{
				if (pUser->m_ReturnType > 0)
					pUser->CB_BattleMsg(pUser->m_pClient->pPeer, pUser->m_pClient->context, BATTLE_ERROR_MSG_GIVEN, 
					pUser->m_Msg, pUser->GetSubType());
				else
					pUser->CB_BattleMsg(pUser->m_pClient->pPeer, pUser->m_pClient->context, BATTLE_USE_MSG_GIVEN, 
					pUser->m_Msg, pUser->GetSubType());
			}
		}
	}
	else
	{
		// apply item effect
		switch (m_data->item_type)
		{
		case ITEMTYPE_DRUG:
			if (m_data->restore_HP > 0) {
				UInt16 hp = pTarget->GetHP() + m_data->restore_HP;
				pTarget->SetHP(MIN(hp, pTarget->GetMHP()), m_data->restore_HP, m_data->restore_HP);
			}
			if (m_data->restore_mHP > 0) {
				UInt16 mhp = pTarget->GetMHP() + m_data->restore_mHP;
				if (mhp > pTarget->GetMaxHP())
					mhp = pTarget->GetMaxHP();
				pTarget->SetMHP(mhp, m_data->restore_mHP, m_data->restore_mHP);
			}
			if (m_data->restore_SP > 0) {
				UInt16 sp = pTarget->GetSP() + m_data->restore_SP;
				pTarget->SetSP(MIN(sp, pTarget->GetMSP()), m_data->restore_SP, m_data->restore_SP);
			}
			if (m_data->restore_mSP > 0) {
				UInt16 msp = pTarget->GetMSP() + m_data->restore_mSP;
				if (msp > pTarget->GetMaxSP())
					msp = pTarget->GetMaxSP();
				pTarget->SetMSP(msp, m_data->restore_mSP, m_data->restore_mSP);
			}
			break;
		}
	}

	// reduce use count or stack count
	ReduceCount();

	return BOOLEAN_TRUE;
}

void CBatItem::ReduceCount()
{
	// reduce use count or stack count
	if (GetRemain() != 255 && GetRemain() > 0 && GetStack() > 0) {
		SetRemain(GetRemain() - 1);				// reduce use count
		if (GetRemain() == 0) {
			SetStack(GetStack() - 1);			// reduce stack
			if (GetStack() > 0) {				// still here after use
				SetRemain(m_data->useCount);	// reset use count
			}
		}
	}
}

void CBatItem::SetData(CItemData *pData)
{
	TRACE_ENSURE(pData);

	if (pData->IsUpdated(CItemData::idMask)) {
		SetId(pData->GetItem_uid(), pData->GetItem_id());
		m_data = gGlobal.m_resource->GetItemData(GetItem_id());
		if (m_data == NULL)
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _T("CItem Warning: cannot find item_id %d in resource"), 
				GetItem_id());
	}
	if (pData->IsUpdated(CItemData::owner_idMask)) {
		SetOwner_id(pData->GetOwner_id());
	}
	if (pData->IsUpdated(CItemData::bagIndexMask)) {
		SetBagIndex(pData->GetBagIndex());
	}
	if (pData->IsUpdated(CItemData::eq_elementMask)) {
		SetEq_element(pData->GetEq_element());
	}
	if (pData->IsUpdated(CItemData::endureMask)) {
		SetEndure(pData->GetEndure());
	}
	if (pData->IsUpdated(CItemData::gradeMask)) {
		SetGrade(pData->GetGrade());
	}
	if (pData->IsUpdated(CItemData::remainMask)) {
		SetRemain(pData->GetRemain());
		m_tryRemain = pData->GetRemain();
	}
	if (pData->IsUpdated(CItemData::stackMask)) {
		SetStack(pData->GetStack());
		m_tryStack = pData->GetStack();
	}
	if (pData->IsUpdated(CItemData::baseAttrMask) ||
		pData->IsUpdated(CItemData::upgAttrMask) ||
		pData->IsUpdated(CItemData::inlayAttrMask)) {
		SetAttrib(	pData->GetBase_STA() + pData->GetUpg_STA() + pData->GetInlay_STA(),
					pData->GetBase_SPR() + pData->GetUpg_SPR() + pData->GetInlay_SPR(), 
					pData->GetBase_STR() + pData->GetUpg_STR() + pData->GetInlay_STR(),
					pData->GetBase_CON() + pData->GetUpg_CON() + pData->GetInlay_CON(), 
					pData->GetBase_AGI() + pData->GetUpg_AGI() + pData->GetInlay_AGI(), 
					pData->GetBase_ATK() + pData->GetUpg_ATK() + pData->GetInlay_ATK(),
					pData->GetBase_DEF() + pData->GetUpg_DEF() + pData->GetInlay_DEF(), 
					pData->GetBase_HIT() + pData->GetUpg_HIT() + pData->GetInlay_HIT(), 
					pData->GetBase_EVA() + pData->GetUpg_EVA() + pData->GetInlay_EVA(),
					pData->GetBase_POW() + pData->GetUpg_POW() + pData->GetInlay_POW(), 
					pData->GetBase_SPD() + pData->GetUpg_SPD() + pData->GetInlay_SPD(), 
					pData->GetBase_maxHP() + pData->GetUpg_maxHP() + pData->GetInlay_maxHP(),
					pData->GetBase_maxSP() + pData->GetUpg_maxSP() + pData->GetInlay_maxSP(),
					pData->GetBase_maxVP() + pData->GetUpg_maxVP() + pData->GetInlay_maxVP());
	}
	if (pData->IsUpdated(CItemData::addonMask)) {
		SetAddon(pData->GetAddon());
	}
	if (pData->IsUpdated(CItemData::buffMask)) {
		SetBuff(pData->GetBuff());
	}

	if (pData->IsUpdated(CItemData::qualityMask))
	{
		SetQuality(pData->GetItem_quality());
	}

	if (pData->IsUpdated(CItemData::soulboundStateMask))
	{
		SetSoulboundState(pData->GetItem_soulboundState());
	}

	if (pData->IsUpdated(CItemData::ExpireDateMask))
	{
		SetExpireDate(pData->GetItem_expireDate());
		time_t uTime;
		time(&uTime);
		UInt32 timediff;
		if( pData->GetItem_expireDate() > (UInt32)uTime )
			timediff = pData->GetItem_expireDate() - (UInt32)uTime;
		else
			timediff = 0;
		SetTimeLeft(timediff);
	}

	if (pData->IsUpdated(CItemData::ValueMask))
	{
		SetValue(pData->GetItemValue1(), pData->GetItemValue2(), pData->GetItemValue3());
	}
}

void CBatItem::WriteData(CItemData *pData)
{
	TRACE_ENSURE(pData);

	if (IsUpdated(idMask)) {
		pData->SetId(GetItem_uid(), GetItem_id());
	}
//	if (IsUpdated(owner_idMask)) {
//		pData->SetOwner_id(GetOwner_id());
//	}
	if (IsUpdated(bagIndexMask)) {
		pData->SetBagIndex(GetBagIndex());
	}
	if (IsUpdated(endureMask)) {
		pData->SetEndure(GetEndure());
	}
	if (IsUpdated(remainMask)) {
		pData->SetRemain(GetRemain());
	}
	if (IsUpdated(stackMask)) {
		pData->SetStack(GetStack());
	}
	if (IsUpdated(buffMask)) {
		pData->SetBuff(GetBuff());
	}

	if (IsUpdated(qualityMask))
	{
		pData->SetQuality(GetItem_quality());
	}

	if (pData->IsUpdated(soulboundStateMask))
	{
		pData->SetSoulboundState(GetItem_soulboundState());
	}

	if (IsUpdated(ExpireDateMask))
	{
		pData->SetExpireDate(GetItem_expireDate());
	}

	if (IsUpdated(ValueMask))
	{
		pData->SetValue(GetItemValue1(), GetItemValue2(), GetItemValue3());
	}

	pData->SetBagIndex(GetBagIndex());
	pData->SetSendMask(pData->GetUpdateMask());
}

/*
void CItem::GetUpdate(Buffer &buffer)
{
	if (IsUpdated(idMask)) {
		buffer->Write<UInt8>(CItemData::idMask);
		buffer->Write<UInt32>(GetItem_uid());
		buffer->Write<UInt32>(GetItem_id());
	}

	if (IsUpdated(owner_idMask)) {
		buffer->Write<UInt8>(CItemData::owner_idMask);
		buffer->Write<UInt32>(GetOwner_id());
	}

	if (IsUpdated(bagIndexMask)) {
		buffer->Write<UInt8>(CItemData::bagIndexMask);
		buffer->Write<UInt8>(GetBagIndex());
	}

	if (IsUpdated(makerMask)) {
		buffer->Write<UInt8>(CItemData::makerMask);
		buffer->Write<String>(GetMaker());
	}
	if (IsUpdated(eq_elementMask)) {
		buffer->Write<UInt8>(CItemData::eq_elementMask);
		buffer->Write<UInt8>(GetEq_element());
	}
	if (IsUpdated(endureMask)) {
		buffer->Write<UInt8>(CItemData::endureMask);
		buffer->Write<UInt16>(GetEndure());
	}
	if (IsUpdated(mEndureMask)) {
		buffer->Write<UInt8>(CItemData::mEndureMask);
		buffer->Write<UInt16>(GetMEndure());
	}
	if (IsUpdated(gradeMask)) {
		buffer->Write<UInt8>(CItemData::gradeMask);
		buffer->Write<UInt16>(GetGrade());
	}
	if (IsUpdated(upgExpMask)) {
		buffer->Write<UInt8>(CItemData::mEndureMask);
		buffer->Write<UInt16>(GetUpg_exp());
	}
	if (IsUpdated(remainMask)) {
		buffer->Write<UInt8>(CItemData::remainMask);
		buffer->Write<UInt8>(GetRemain());
	}
	if (IsUpdated(stackMask)) {
		buffer->Write<UInt8>(CItemData::stackMask);
		buffer->Write<UInt8>(GetStack());
	}
	if (IsUpdated(baseAttrMask)) {
		buffer->Write<UInt8>(CItemData::baseAttrMask);
		buffer->Write<UInt16>(GetBase_STA());
		buffer->Write<UInt16>(GetBase_SPR());
		buffer->Write<UInt16>(GetBase_STR());
		buffer->Write<UInt16>(GetBase_CON());
		buffer->Write<UInt16>(GetBase_AGI());
		buffer->Write<UInt16>(GetBase_ATK());
		buffer->Write<UInt16>(GetBase_DEF());
		buffer->Write<UInt16>(GetBase_HIT());
		buffer->Write<UInt16>(GetBase_EVA());
		buffer->Write<UInt16>(GetBase_POW());
		buffer->Write<UInt16>(GetBase_SPD());
		buffer->Write<UInt16>(GetBase_maxHP());
		buffer->Write<UInt16>(GetBase_maxSP());
		buffer->Write<UInt16>(GetBase_maxVP());
	}
	if (IsUpdated(upgAttrMask)) {
		buffer->Write<UInt8>(CItemData::baseAttrMask);
		buffer->Write<UInt16>(GetUpg_STA());
		buffer->Write<UInt16>(GetUpg_SPR());
		buffer->Write<UInt16>(GetUpg_STR());
		buffer->Write<UInt16>(GetUpg_CON());
		buffer->Write<UInt16>(GetUpg_AGI());
		buffer->Write<UInt16>(GetUpg_ATK());
		buffer->Write<UInt16>(GetUpg_DEF());
		buffer->Write<UInt16>(GetUpg_HIT());
		buffer->Write<UInt16>(GetUpg_EVA());
		buffer->Write<UInt16>(GetUpg_POW());
		buffer->Write<UInt16>(GetUpg_SPD());
		buffer->Write<UInt16>(GetUpg_maxHP());
		buffer->Write<UInt16>(GetUpg_maxSP());
		buffer->Write<UInt16>(GetUpg_maxVP());
	}
	if (IsUpdated(inlayGemMask)) {
		buffer->Write<UInt8>(CItemData::inlayGemMask);
		buffer->Write<UInt8>(GetEq_gem());
		buffer->Write<UInt16>(GetInlay_gem(0));
		buffer->Write<UInt16>(GetInlay_gem(1));
		buffer->Write<UInt16>(GetInlay_gem(2));
		buffer->Write<UInt16>(GetInlay_gem(3));
		buffer->Write<UInt16>(GetInlay_gem(4));
		buffer->Write<UInt16>(GetInlay_gem(5));
		buffer->Write<UInt16>(GetInlay_gem(6));
		buffer->Write<UInt16>(GetInlay_gem(7));
		buffer->Write<UInt16>(GetInlay_gem(8));
		buffer->Write<UInt16>(GetInlay_gem(9));
	}
	if (IsUpdated(inlayAttrMask)) {
		buffer->Write<UInt8>(CItemData::inlayAttrMask);
		buffer->Write<UInt16>(GetInlay_STA());
		buffer->Write<UInt16>(GetInlay_SPR());
		buffer->Write<UInt16>(GetInlay_STR());
		buffer->Write<UInt16>(GetInlay_CON());
		buffer->Write<UInt16>(GetInlay_AGI());
		buffer->Write<UInt16>(GetInlay_ATK());
		buffer->Write<UInt16>(GetInlay_DEF());
		buffer->Write<UInt16>(GetInlay_HIT());
		buffer->Write<UInt16>(GetInlay_EVA());
		buffer->Write<UInt16>(GetInlay_POW());
		buffer->Write<UInt16>(GetInlay_SPD());
		buffer->Write<UInt16>(GetInlay_maxHP());
		buffer->Write<UInt16>(GetInlay_maxSP());
		buffer->Write<UInt16>(GetInlay_maxVP());
	}
	if (IsUpdated(addonMask)) {
		buffer->Write<UInt8>(CItemData::addonMask);
		buffer->Write<UInt16>(GetAddon());
	}
	if (IsUpdated(buffMask)) {
		buffer->Write<UInt8>(CItemData::buffMask);
		buffer->Write<String>(GetBuff());
	}
}

void CBatItem::GetUpdate(Buffer &buffer)
{
	if (IsUpdated(endureMask)) {
		buffer->Write<UInt8>(CItemData::endureMask);
		buffer->Write<UInt16>(GetEndure());
	}
	if (IsUpdated(remainMask)) {
		buffer->Write<UInt8>(CItemData::remainMask);
		buffer->Write<UInt8>(GetRemain());
	}
	if (IsUpdated(stackMask)) {
		buffer->Write<UInt8>(CItemData::stackMask);
		buffer->Write<UInt8>(GetStack());
	}
	if (IsUpdated(buffMask)) {
		buffer->Write<UInt8>(CItemData::buffMask);
		buffer->Write<String>(GetBuff());
	}
}

void CBatItem::Update(Buffer &buffer, Size size)
{
	UInt8 mask = 0xff;

	// mask 0 is mean for next record
	while (mask != 0 && buffer->GetCursor() < size)
	{
		buffer->Read<UInt8>(mask);
		switch (mask) {
		case CItemData::eq_elementMask:
			{
			UInt8 eq_element;
			buffer->Read<UInt8>(eq_element);
			SetEq_element(eq_element);
			}
			break;
		case CItemData::endureMask:
			{
			UInt16 endure;
			buffer->Read<UInt16>(endure);
			SetEndure(endure);
			}
			break;
		case CItemData::remainMask:
			{
			UInt8 remain;
			buffer->Read<UInt8>(remain);
			SetRemain(remain);
			}
			break;
		case CItemData::stackMask:
			{
			UInt8 stack;
			buffer->Read<UInt8>(stack);
			SetStack(stack);
			}
			break;
		case CItemData::buffMask:
			{
			String buff;
			buffer->Read<String>(buff);
			SetBuff(buff);
			}
			break;
		default:
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _T("CBatItem Update Warning: invalid mask %d"), mask);
		}
	}
}
*/

void CBatItem::PreSend()
{
	//m_sendMask.Clear();
	//m_sendMask.MaskBits(makerMask, eq_elementMask, endureMask, 
	//	mEndureMask, gradeMask, upgExpMask, remainMask, 
	//	baseAttrMask, upgAttrMask, inlayGemMask, inlayAttrMask,
	//	addonMask, buffMask, -1);
}

int CBatItem::GetQuality()
{
	return 0;
}