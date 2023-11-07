//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Guild.h"

#ifdef MAP_SERVICE	
#include "MapService/Global.h"
#include "Proxy/Service/MessageService.h"
#endif
#ifdef QGL
#include "QGL/Global.h"
#include "QGL/MenuGuildProcess.h"
#include "QGL/CharacterControl.h"
#include "Proxy/Logic/GuildMember.h"
#include "Proxy/Logic/GuildApplicant.h"
#endif // QGL

REGISTER_CLASSTYPE(CGuild, Guild_CLASSID);


void CGuild::ChangeFund(Int32 change, UInt8 source)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	changes.push_back(change);
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::fundMask, changes, source);
	#endif
}
void CGuild::ChangeGuildPvPBid(Int32 change)
{
	#ifdef MAP_SERVICE
	Int32Vector changes;
	changes.push_back(change);
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::guildPvPBidMask, changes, 0);	
	#endif
}
void CGuild::ChangeProsper(Int32 change, UInt8 source)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	changes.push_back(change);
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::prosperMask, changes, source);
	#endif
}
void CGuild::ChangeMaterial(Int32 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	changes.push_back(change);
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorMaterialMask, changes, 0);
	#endif
}
void CGuild::ChangeRoom(Int8 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	changes.push_back(change);
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorRoomMask, changes, 0);
	#endif
}
void CGuild::SetAuraValue(Int8 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	changes.push_back(change);
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorAuraMask, changes, 0);
	#endif
}
void CGuild::ChangeTrainCharEx(Int8 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	changes.push_back(change);
	changes.push_back(0);
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorTrainPlayerPowMask, changes, 0);
	#endif
}
void CGuild::ChangeTrainCharIn(Int8 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	changes.push_back(0);
	changes.push_back(change);
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorTrainPlayerPowMask, changes, 0);
	#endif
}
void CGuild::ChangeTrainPartnerEx(Int8 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	changes.push_back(change);
	changes.push_back(0);
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorTrainPartnerPowMask, changes, 0);
	#endif
}
void CGuild::ChangeTrainPartnerIn(Int8 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
    changes.push_back(0);
	changes.push_back(change);
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorTrainPartnerPowMask, changes, 0);
	#endif
}
void CGuild::ChangeDrug(Int8 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	for (UInt8 i = 0; i < 5; ++i)
		changes.push_back(0);
	changes[0] = change;
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorRoomPropertyMask, changes, 0);
	#endif
}
void CGuild::ChangeCook(Int8 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	for (UInt8 i = 0; i < 5; ++i)
		changes.push_back(0);
	changes[1] = change;
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorRoomPropertyMask, changes, 0);
	#endif
}
void CGuild::ChangeWine(Int8 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	for (UInt8 i = 0; i < 5; ++i)
		changes.push_back(0);
	changes[2] = change;
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorRoomPropertyMask, changes, 0);
	#endif
}
void CGuild::ChangeManorHP(Int8 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	for (UInt8 i = 0; i < 5; ++i)
		changes.push_back(0);
	changes[3] = change;
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorRoomPropertyMask, changes, 0);
	#endif
}
void CGuild::ChangeManorSP(Int8 change)
{
	#ifdef MAP_SERVICE	
	Int32Vector changes;
	for (UInt8 i = 0; i < 5; ++i)
		changes.push_back(0);
	changes[4] = change;
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::manorRoomPropertyMask, changes, 0);
	#endif
}
void CGuild::SetNewManorScale(UInt8 scale)
{
	#ifdef MAP_SERVICE	
	if (scale >= 0 && scale <= 5)
        MessageService_Proxy::ManorScaleChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), scale);
	#endif
}
void CGuild::ChangeAutoManageBusiness(Boolean bOpen)
{
#ifdef MAP_SERVICE	
	Int32Vector changes;
	changes.push_back(bOpen);
	MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,RPCContext(), GetGuild_uid(), CGuild::autoManageBusinessMask, changes, 0);
#endif
}
RPCResult CGuild::CB_AddMember(LPCPEER pPeer, RPCContext &context, NetObject* pMember)
{
	#ifdef QGL

	CGuildMember * pGuildMember = CastNetObject<CGuildMember>(pMember);
	if (pGuildMember)
	{
		gGlobal.g_pMenuGuildProcess->InsertGuildMember(pGuildMember);
		String text;
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_JOIN_WELCOME")), pGuildMember->GetName().c_str());
		gGlobal.PrintMessage(text, CHANNELTYPE_GUILD);

		if (gGlobal.g_pCharacter && 
			gGlobal.g_pCharacter->GetChar_id() == pGuildMember->GetChar_id())
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_JOIN_ACCEPT")), GetName().c_str());
			gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
		}
			

	}
	else
		DeleteNetObject(pMember);
	#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult CGuild::CB_RemoveMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason)
{
	#ifdef QGL
		gGlobal.g_pMenuGuildProcess->RemoveMember(char_id, reason);
	
	#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult CGuild::CB_AddApplicant(LPCPEER pPeer, RPCContext &context, NetObject* pObj)
{
	#ifdef QGL
	CGuildApplicant * pGuildApplicant = CastNetObject<CGuildApplicant>(pObj);
	if (pGuildApplicant)
	{
		gGlobal.g_pMenuGuildProcess->InsertGuildApplicant(pGuildApplicant);

		String text;
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_APPLY_REQUEST")), pGuildApplicant->GetName().c_str());
		gGlobal.PrintMessage( text, CHANNELTYPE_GUILD);
	}
	else
		DeleteNetObject(pObj);
	#endif
	return RPC_RESULT_OK;
}


RPCResult CGuild::CB_RemoveApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	#ifdef QGL
		gGlobal.g_pMenuGuildProcess->RemoveApplicant(char_id);
	#endif
	return RPC_RESULT_OK;
}

void CGuild::OnUpdateAlliance(const UInt32 &alliance_id)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}
void CGuild::OnUpdateEnemy(const UInt32 &enemy_id)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}
void CGuild::OnUpdateSecMaster(const String &secMasterName)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}

void CGuild::OnUpdateLevel(const UInt16 &level)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}
void CGuild::OnUpdateProsper(const UInt32 &prosper, const UInt16 &max_prosper)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}
void CGuild::OnUpdateFund(const UInt32 &fund)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}
void CGuild::OnUpdateMemberNum(const UInt16 &numOfMember, const UInt16 &numOfMaxMember)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}

void CGuild::OnUpdateGuildPvPScore(const UInt32 &guildPvPScore)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}

void CGuild::OnUpdateGuildPvPBid(const UInt32 &guildPvPBid)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}

void CGuild::OnUpdateGroupPvPVSGuild(const String &groupPvPVSGuild)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}
void CGuild::OnUpdateGuildPvPGroup(const UInt8 &guildPvPGroup)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}

void CGuild::OnUpdateAim(const String &Aim)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshRuleMenu();
	if (!Aim.IsEmpty())
		gGlobal.g_pMenuGuildProcess->ShowAim(gGlobal.g_pMenuGuildProcess->m_curApplyIndex);
	#endif // QGL
}
void CGuild::OnUpdateRule(const String &Rule)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshRuleMenu();
	#endif // QGL
}

void CGuild::OnUpdateManorScale(const UInt8 &manorScale)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshManorMenu();
	#endif // QGL
}

void CGuild::OnUpdateManorRoom(const UInt8 &room)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshManorMenu();
	gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	#endif // QGL
}

void CGuild::OnUpdateManorMaterial(const UInt32 &material)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshManorMenu();
	#endif // QGL
}

void CGuild::OnUpdateManorGuard(const UInt8 &guard)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshManorMenu();
	#endif // QGL
}

void CGuild::OnUpdateManorTrainPlayerPow(const UInt8 &trainCharExPow, const UInt8 &trainCharInPow)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshManorMenu();
	#endif // QGL
}

void CGuild::OnUpdateManorTrainPartnerPow(const UInt8 &trainPartnerExPow, const UInt8 &trainPartnerInPow)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshManorMenu();
	#endif // QGL
}

void CGuild::OnUpdateManorRoomProperty(const UInt8 &drugLv, const UInt8 &cookLv, const UInt8 &wineLv, const UInt8 &hpLv, const UInt8 &spLv)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshManorMenu();
	#endif // QGL
}

void CGuild::OnUpdateManorMaintainNum(const UInt32 &maintainNum)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshManorMenu();
	#endif // QGL
}

void CGuild::OnUpdateManorAura(const UInt8 &aura)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshManorMenu();
	#endif // QGL
}

void CGuild::OnUpdateFailMantainPayNum(const UInt32 &failPayNum)
{
	#ifdef QGL
	gGlobal.g_pMenuGuildProcess->RefreshManorMenu();
	#endif // QGL
}























