#pragma once
#ifndef _LUA_PUSH_H_
#define _LUA_PUSH_H_

#define  DEF_LuaPush(type)									\
void LuaPush(lua_State *L, type *Ptr);


DEF_LuaPush(UIItem)
DEF_LuaPush(UIStaticText)
DEF_LuaPush(UIPopUpMenu)
DEF_LuaPush(UIButton)
DEF_LuaPush(UIEditBox)
DEF_LuaPush(UIChatItem)
DEF_LuaPush(UIChatRow)
DEF_LuaPush(UIChatPanel)
DEF_LuaPush(UIMiniMap)
DEF_LuaPush(UIPanel)
DEF_LuaPush(UIImage)
DEF_LuaPush(UIButton)
DEF_LuaPush(UIPanelEx)
DEF_LuaPush(UIManager)
DEF_LuaPush(UIAniIcon)

//DEF_LuaPush(ALSound)
//DEF_LuaPush(ALOggPlayer)
DEF_LuaPush(FSOUND_Manager)

DEF_LuaPush(CEntityBase)
DEF_LuaPush(CAnimatedEntity)
DEF_LuaPush(ChatLst)
DEF_LuaPush(CMenuSkillProcess)
DEF_LuaPush(CMenuQuestLogProcess)
DEF_LuaPush(CCharacterControl)

DEF_LuaPush(CMenuPartnerProcess)
DEF_LuaPush(CMenuGivenProcess)
DEF_LuaPush(CMenuItemProcess)
DEF_LuaPush(CBigMap)
DEF_LuaPush(CBatBubbleControl)
DEF_LuaPush(CMenuSystemSetting)
DEF_LuaPush(CMenuTradingProcess)
DEF_LuaPush(CMenuBankService)
DEF_LuaPush(CMenuOnlineShopProcess)
DEF_LuaPush(CMenuMixProcess)
DEF_LuaPush(CAntiHack)

DEF_LuaPush(CMenuHawkerProcess)
DEF_LuaPush(CMenuGiftBox)
DEF_LuaPush(CMenuQuestJournal)
DEF_LuaPush(CMenuWareHouseProcess)
DEF_LuaPush(CMenuSellYuanBaoProcess)
DEF_LuaPush(CGenerationProcess)
DEF_LuaPush(CAccShopProcess)
#endif


