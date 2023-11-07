//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/BatCharacter.h"

#ifdef QGL
#include "QGL/Global.h"
#include "QGL/BattleControl.h"
#include "Resource/ResSkill.h"
#include "Resource/ClientResManager.h"
#endif // QGL
REGISTER_CLASSTYPE(CBatCharacter, BatCharacter_CLASSID);

VOID CBatCharacter::OnUpdateInfo(const UInt32 &acct_id, const UInt32 &char_id, const String &nickName, const UInt8 &sex)
{
}

VOID CBatCharacter::OnUpdateExp(const UInt32 &exp)
{
}

VOID CBatCharacter::OnUpdateMDP(const UInt16 &mDP)
{
}

VOID CBatCharacter::OnUpdateMWC(const UInt16 &mWC)
{
}

VOID CBatCharacter::OnUpdateMVP(const UInt16 &mVP)
{
}

VOID CBatCharacter::OnUpdateDP(const UInt16 &DP)
{
}

VOID CBatCharacter::OnUpdateWC(const UInt16 &WC)
{
}

VOID CBatCharacter::OnUpdateVP(const UInt16 &VP)
{
}

VOID CBatCharacter::OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor, const UInt16 &clothType, const UInt16 &clothColor)
{
}

VOID CBatCharacter::OnUpdateEquipment(const UInt32 &eq_weapon, const UInt32 &eq_head, const UInt32 &eq_headBand, const UInt32 &eq_body, const UInt32 &eq_foot, const UInt32 &eq_tinker1, const UInt32 &eq_tinker2)
{
}


RPCResult CBatCharacter::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Error: errcode %d, %s"), errCode, errMsg.c_str());
	if (!errMsg.IsEmpty()) {
		if (errMsg.Left(2) == _T("MS")) {
			String text(gGlobal.GetStringTable()->Get(errMsg.c_str()));
			if (!text.IsEmpty())
				gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
			else
				gGlobal.PrintMessage(errMsg, CHANNELTYPE_SYSTEM);
		} 
	}
	if (gGlobal.g_pBattleControl)
	{
		CCharacterControl * pCharCtrl = gGlobal.g_pBattleControl->FindCharCtrlByBatActor(this);
		if (pCharCtrl)
			pCharCtrl->SetIsSentCommand(false);
		gGlobal.g_pBattleControl->OnCancelUseItemInBattle();
	}
	return RPC_RESULT_OK;
}
