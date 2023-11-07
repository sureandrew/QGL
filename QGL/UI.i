%module LuaUI
%{
// LxC: fix link error LNK2005: _SWIG_init_user already defined ...
// http://lua.2524044.n2.nabble.com/Using-multiple-modules-with-Swig-gt-Lua-td7585945.html
#define SWIG_init_user SWIG_init_user_LuaUI

#include "Common.h"
#include "QGL.h"
#include "UI.h"
#include "PlayOggVorbis.h"
#include "EntityBase.h"
#include "AnimatedEntity.h"
#include "MenuSkillProcess.h"
#include "MenuQuestLogProcess.h"
#include "BatBubbleControl.h"
#include "CharacterControl.h"
#include "MenuPartnerProcess.h"
#include "MenuGivenProcess.h"
#include "MenuItemProcess.h"
#include "MenuTargetProcess.h"
#include "BigMap.h"
#include "MenuBuffIconProcess.h"
#include "MenuSystemSetting.h"
#include "MenuFriendProcess.h"
#include "MenuTradingProcess.h"
#include "MenuHelpProcess.h"
#include "MenuOnlineShopProcess.h"
#include "MenuRankProcess.h"
#include "MenuPetProcess.h"
#include "MenuMixProcess.h"
#include "MenuTitleProcess.h"
#include "MenuCharProcess.h"
#include "AntiHack.h"
#include "MenuHawkerProcess.h"
#include "MenuGuildProcess.h"
#include "MenuBankService.h"
#include "Common/common_binary.h"
#include "MenuGiftBox.h"
#include "MenuQuestJournal.h"

#include "WeaponsRank.h"
#include "TeamDescription.h"
//#include "WeaponsPointSystem.h"


#include "QuestionProcess.h"
#include "MenuWareHouseProcess.h"
#include "MenuAutoWalkProcess.h"
#include "MenuSellYuanBaoProcess.h"
#include "MenuTargetEquipProcess.h"
#include "RelationSystemProcess.h"
#include "WantedProcess.h"
#include "GenerationProcess.h"
#include "MenuPartnerCultivationUpProcess.h"
#include "AccShopProcess.h"
#include "PointsShopProcess.h"

UIManager *GetUIManager();
void GetPlayerPos(int &X, int &Y);

UInt32 GetMapWidth();
UInt32 GetMapHeight();

void QGLApplicationLogin();
void OpenSubWindow();
void CloseSubWindow();

void BattleAction();
void BattleSkill();
void BattleSpecial();
void BattleItem();
void BattleDefence();
void BattleFriend();
void BattleProtect();
void BattleCatch();
void BattleHelp();
void BattleAuto();
void BattleEscape();
void CancelAuto();
void BattleJoin();
void BattleWatchLeave();

void SetCharacterIndex(Int32 Index);
void SetLineIndex(Int32 Index);

void BBAttack();
void BBTrade();
void BBGive();
void BBParty();
void BBChar();
void BBFriend();
void BBItem();
void BBSkill();
void BBHousen();
void BBMission();
void BBFriendShip();
void BBEmotion();
void BBSystem();
void BBGeneration();

void PartyPrivateCall(int memPos);
void PartyKick(int memPos);
void PartyPromote(int memPos);
void PartyQuit();

void PartySelectLineup();
void PartySetKeyPos(int iKeyPos);
void UpdateLineupSelectMenu();
void ForgetLearnedLineup();
void PartySetSelectLineupResualt();
void OpenLineupInfoView();
void LeaveTemporarily();
void ReturnParty();

void UpdateCharMenu();

void UpdateBattleItemMenu(bool wine = false);

void CopyToShortCut(int itemPos, int targetPos);
void CopySkillToShortCut(int SkillID, int targetPos);
void  DelShortCutSlot(int Ref, int Type);
Int32 GetShortCutRefType(Int32 Index);
Int32 GetShortCutRefNum(Int32 Index);
void EquipItemUse(int itemPos);
void BattleItemUse(int itemPos);
void MoveEquipToBag(int itemPos, int targetPos);
void MoveToBag(int itemPos, int bag, int target = -1);
void DropItem(int itemPos, int dropSize);

void ChangeChannel(int i);
void ReceiveChannel(int i);
void RejectChannel(int i);
void InitChannelEnable();
bool IsChannelMsg(PCSTRING msg, int Type);
void AddChatIconToPanel();
void AddChatIconToEditLine();
void ChangeEditLineColor(int index);
bool GetLisentToChannel(int index);
void SendPrivateMessageTo(PCSTRINGW Name);
void InitRecentChatListBox();

void UpdateClockPanel(bool bInit);
void UpdateRightUpTimeInfo();

void MBChangeNextPage();
void PlayerChooseResult(int opt);

void BatDialogNextPage();
void PlayerChooseResultInBat(int opt);

bool HasParty();
bool HasTempParty();
void UpdatePartyPanel();
void CopyImageForUIImage(PCSTRINGA src, PCSTRINGA des);

void BattleItemDrag(int itemPos);
void BattleItemMove(int SrcBagIndex, int DesBagIndex);

bool HasItem(int itemPos);
int GetMaxStack(int itemPos);
void SetDropItemText(int itemPos, PCSTRINGA srcStaticText);
void ShowErrMessage(PCSTRINGA pText, bool center = false);
bool InviteComing();

bool HasCharacter(Int32 Index);
void ShowPartyByIndex(int index=0);
void InviteConfirm(bool confirm);
int GetPartyListSize();
void UpdateApplicationMenu();
void JoinConfirm(int index, bool confirm);
void ShowOtherParty(int next);
void ClearAllRequest();
void ApplicantPanelScroll(int scroll);

void BattleJoinConfirm(int index, bool confirm);
void JoinBattleClearAllRequestUp();
void JoinBattleScroll(int scroll);
void UpdateJoinBattleMenu();

bool IsJoinRequestExist();
void ShowCreateCharacter(int sex = 0, int cclass = 1);

void ChangeCharacterHairCol(bool next);
void ChangeCharacterHead(bool next);
void ChangeCharacterCloth(bool next);
void ChangeCharacterClothCol(bool next);

void CreateCharacter();
PCSTRINGW GetTempMsg();
bool IsFullScreen();
void SetHighLightInLogin(int index, bool highLight);
bool IsBattle();
bool IsPvP();

UInt32 GetBGMVolume();
UInt32 GetSFXVolume();

void LoadLua(PCSTRINGA luafilename);

void CharacterLevelUp();
void CharAttOk();
void CharAttUp(int Index, int Count);
void CharAttDown(int Index, int Count);

void ShowMidMap();
void MidMapMoveCharTo(int index);

void MidMapMoveOut();
void ResetMidMapValue();
void MidMapShowName(int index);
void MidMapClickNPC();
void UpdateSearchList(int index);

void NPCSearchListUpOrDown(int up);

void MidMapShowLocationBox();
void NPCSearchClickList(int index);
void PartySearchClickList(int index);
void CharSearchClickList(int index);
void ShowLeaderInfo();
void ShowLeaderInfoChar();

void SearchNPCList(int lang);

void WeaponsRankSearchList(int lang);

void ShowPartySearchList(int searchBy);
void ShowCharSearchList(int searchBy);

void OnPartySearchPagePrev();
void OnCharSearchPagePrev();

void OnPartySearchPageNext();
void OnCharSearchPageNext();

void OnJoinPartySearch();
void OnJoinCharSearch();

void OnShowPartyInfo();
void OnShowCharInfo();
bool IsIndoor();


void ConfirmPK();
void CancelPK();
void ConfirmMatch();
void CancelMatch();

void PartnerAttrUp(int index);
void PartnerAttrDown(int index);
void PartnerAddConfirm();
bool haveParty();
bool IsLeader();
bool GetHelpEnable();
bool GetActionEnable();
bool GetSkillEnable();
bool GetItemEnable();
bool GetDefenceEnable();
bool GetFriendEnable();
bool GetProtectEnable();
bool GetCatchEnable();
bool GetAutoEnable();
bool GetEscapeEnable();

bool GetPartnerSkillEnable();
bool GetPartnerItemEnable();
bool GetPartnerDefenceEnable();
bool GetPartnerProtectEnable();
bool GetPartnerEscapeEnable();

void DeleteCharacter();
void RetrieveCharacter(UInt32 Index);
Boolean CheckCharacterFrozen(UInt32 index);

void NPCSearchListScroll();

void UpdateShopItemDetail(int pos);
//void UpdateQuestItemDetail(int pos);
void ResetSearchValue();
void ShopOpenConfirmBox(int pos = -1, int nMode = 2);
void ShopEnterItemCount(bool bSell, int nPos);
void ShopSetItemCount(bool bMax);
void ShopAdjustItemCount(bool bInc);
void ShopConfirmAction();
void ShopResetAction();
void ShopDragItemAction(int pos);
void ShopOnClickSellButton();
void ShopCloseMainMenu();
void ShopOnEditBoxKeyUp(int keyCode);
void ShopUpdatePrice();


void InitLoginScreen();
void SaveLoginDefault();
UInt32 GetDefaultCharacter();
void SetDefaultCharacter(int pos);
UInt32 GetDefaultLine();
void SetDefaultLine(int pos);

void SetAniStateInLogin(int index, int state);
void ShowPartyButton();

void BattlePartnerEscape();
void TestingFunction();
bool IsBattlePartnerTurn();

void LockItem(UInt32 index, UInt32 assetType);
void UnlockItem(UInt32 index, UInt32 assetType);
void SetLockCursor(Boolean lock);
void SetUnLockCursor(Boolean unlock);

UIManager* GetSubUIManager();
CMenuSkillProcess * GetSkillProcess();
void CheckCharacterName();
CMenuQuestLogProcess * GetQuestLogProcess();
CMenuPartnerProcess * GetPartnerProcess();
CMenuGivenProcess* GetGivenProcess();
CMenuItemProcess* GetItemProcess();
CMenuTargetProcess* GetMenuTargetProcess();
CBigMap* GetBigMap();
CMenuBuffIconProcess* GetMenuBuffIconProcess();
CMenuSystemSetting* GetMenuSystemSetting();
CMenuFriendProcess* GetMenuFriendProcess();
CMenuTradingProcess* GetTradingProcess();
CMenuBankService* GetBankProcess();
CMenuHelpProcess* GetMenuHelpProcess();
CMenuOnlineShopProcess* GetOnlineShopProcess();
CMenuRankProcess* GetMenuRankProcess();
CMenuPetProcess* GetMenuPetProcess();
CMenuMixProcess* GetMixProcess();
CMenuTitleProcess* GetTitleProcess();
CAntiHack* GetAntiHackProcess();
CMenuHawkerProcess* GetHawkerProcess();
CMenuGuildProcess* GetMenuGuildProcess();
CMenuGiftBox* GetGiftBoxProcess();
CMenuQuestJournal* GetQuestJournalProcess();

CWeaponsRank* GetWeaponsRankProcess();
CTeamDescription* GetTeamDescriptionProcess();
//CWeaponsPointSystem* GetWeaponsPointSystemProcess();


CQuestionProcess* GetQuestionProcess();
CMenuWareHouseProcess* GetWareHouseProcess();
CMenuAutoWalkProcess* GetAutoWalkProcess();
CMenuSellYuanBaoProcess* GetSellYBProcess();
CMenuTargetEquipProcess* GetTargetEquipProcess();
CRelationSystemProcess* GetRelationSystemProcess();
CWantedProcess* GetWantedProcess();
CGenerationProcess* GetGenerationProcess();
CMenuPartnerCultivationUpProcess* GetPartnerCultivationUpProcess();
CAccShopProcess* GetAccShopProcess();
CPointsShopProcess* GetPointsShopProcess();


void AddXMLFile2UIManger(PCSTRINGA Filename);
void UIMangerLoadUI(UIManager *pUIManager, PCSTRINGA Filename);

void FormatToSkillString(PCSTRINGA Text);

void SkillFail(PCSTRINGA Text);

void SubChatScroll();

bool CanLevelUp();

void SelectItemPartner(UInt8 index);

void UpdateChangeLine();
void ApplicantPrivateCall(int index);
bool IsTalking();

void ChangeMapLine(UInt8 lineIndex);
void ConfirmChangeLineSec();
void TeammatePrivateCall(int index);
void TeammateKick(int index);
void TeammatePromote(int index);
void ItemScroll(int index);
void OnCloseBattleItem();

void QuestCancelCancel();
void QuestCancelConfirm();
void ShowRemoveCharacterName();

bool IsOpenSkillShop();

void SetTempResult(double result);
double GetTempResult();

void ChangeFriendIcon();
void CloseFriendBox();
void CloseQuestionBox();
void OpenFactionDetail();
void LuaSetBPShortCutFlag(bool Flag);
bool LuaGetBPShortCutFlag();
void CreateCharRotate(bool left);

void OpenFriendChatBox(PSTRINGA msg, PSTRINGA function);
void OpenGuildBox();
void OpenDeleteMemberGuildBox();
void OpenLeaveGuildBox();
void ClsoeGuildBox();
UIPanel *LuaCreateUIPanel();
UIPanelEx *LuaCreateUIPanelEx();
UIImage *LuaCreateUIImage();
UIStaticText *LuaCreateUIStaticText();
UIButton *LuaCreateUIButton();
UICheckBox *LuaCreateUICheckBox();
UIListBox *LuaCreateUIListBox();
UIPopUpMenu *LuaCreateUIPopUpMenu();
UIScrollBarH *LuaCreateUIScrollBarH();
UIScrollBarV *LuaCreateUIScrollBarV();
UIScrollListBox *LuaCreateUIScrollListBox();
UIEditBox *LuaCreateUIEditBox();
UIChatPanel *LuaCreateUIChatPanel();
UIChatString *LuaCreateUIChatString();
UIChatLink *LuaCreateUIChatLink();
UIMiniMap *LuaCreateUIMiniMap();
UILinerImage *LuaCreateUILinerImage();
UILinerImageV *LuaCreateUILinerImageV();
UIAniIcon *LuaCreateUIAniIcon();

void RunNewGame();
void OpenHomePage();
void ChangeChatIcon(int index);

UInt32 LuaGetChar_id();
UInt32 LuaGetBag(UInt32 itemPos);
PCSTRING LuaGetItemName(UInt32 itemPos);
UInt32 LuaGetItemColor(UInt32 itemPos);
UInt32 GetFirstNum(PCSTRINGW Str);
UInt32 GetLastNum(PCSTRINGW Str);
void LuaSetShopSkillFlag(bool flag);
bool LuaGetShopSkillFlag();
bool CharSetMapPos(PCSTRINGW pStr);
bool CharOpenQuestMenu(PCSTRINGW pStr);
bool IsHawking();
UInt32 LuaGetCharPosX();
UInt32 LuaGetCharPosY();
PCSTRING LuaGetPartnerName(UInt32 Index);
PCSTRING LuaGetPartnerOriginName(UInt32 Index);
UInt32 LuaGetPartnerID(UInt32 Index);
PCSTRING LuaGetPetName(UInt32 Index);
PCSTRING LuaGetPetOriginName(UInt32 Index);
UInt32 LuaGetPetID(UInt32 Index);
void LuaAddAlertBox(PCSTRINGA Msg, bool ButtonFlag);
PCSTRING LuaGetLineTextName();

void ShowPartyPopUp(UInt8 index);
void SelectPartyPopUp(UInt8 popSelect, UInt8 index);
void ShowTeammatePopUp(UInt8 index);
void AutoAcceptCheckBox(bool on);

void SelectTeammatePopUp(UInt8 popSelect, UInt8 index);
void SetChangePos(UInt8 index);
void ChangePartyDesc();
void StopCharacter();
void ShowAlertOK();
void ConfirmReplyString();
void ReplyStringNULL();
bool IsCurrMsgIcon(int index);
void SetHotKeyEnable(bool value);
void OpenWebLink();
void SetWebLink(PCSTRINGA link, int typeno);
void CharEatFastFood(UInt8 foodType);

#pragma warning( disable : 4101 4102 4800 4101)

bool CheckShiftDown();


template <typename T>
T ConvertVoidto(void *Ptr)
{
	return (T)(Ptr);
}



int Div(int a, int b)
{
	return (a / b);
}

inline UInt32 DivUInt32(UInt32 a, UInt32 b)
{
	return (a / b);
}


inline bool LAnd(bool a, bool b)
{
	return (a && b);
}


inline bool LOr(bool a, bool b)
{
	return (a || b);
}

inline int LMod(int a, int b)
{
	return (a % b);
}


#define  DEF_LuaPush(type)									\
void LuaPush(lua_State *L, type *Ptr)						\
{															\
	SWIG_NewPointerObj(L, Ptr, SWIGTYPE_p_##type, 0);		\
}


DEF_LuaPush(UIItem)
DEF_LuaPush(UIStaticText)
DEF_LuaPush(UIPopUpMenu)
DEF_LuaPush(UIButton)
DEF_LuaPush(UIEditBox)
DEF_LuaPush(UIEditBoxEx)
DEF_LuaPush(UIScrollBarV)
DEF_LuaPush(UIChatItem)
DEF_LuaPush(UIChatLink)
DEF_LuaPush(UIChatRow)
DEF_LuaPush(UIChatPanel)
DEF_LuaPush(UIChatEditPanel)
DEF_LuaPush(UIMiniMap)
DEF_LuaPush(UIPanel)
DEF_LuaPush(UIImage)
DEF_LuaPush(UILinerImage)
DEF_LuaPush(UIPanelEx)
DEF_LuaPush(UICheckBox)
DEF_LuaPush(UIManager)
DEF_LuaPush(UIAniIcon);

//DEF_LuaPush(ALSound)
DEF_LuaPush(FSOUND_Manager)
//DEF_LuaPush(ALOggPlayer)

DEF_LuaPush(CEntityBase)
DEF_LuaPush(CAnimatedEntity)
DEF_LuaPush(ChatLst)

DEF_LuaPush(CMenuSkillProcess)
DEF_LuaPush(CMenuQuestLogProcess)
DEF_LuaPush(CMenuPartnerProcess)
DEF_LuaPush(CMenuGivenProcess)
DEF_LuaPush(CMenuItemProcess)
DEF_LuaPush(CBigMap)


DEF_LuaPush(CBatBubbleControl)

DEF_LuaPush(CMenuBuffIconProcess)
DEF_LuaPush(CMenuSystemSetting)
DEF_LuaPush(CMenuTradingProcess)
DEF_LuaPush(CMenuBankService)
DEF_LuaPush(CMenuOnlineShopProcess)
DEF_LuaPush(CMenuMixProcess)
DEF_LuaPush(CMenuTitleProcess)
DEF_LuaPush(CAntiHack)

DEF_LuaPush(CMenuHawkerProcess)
DEF_LuaPush(CMenuGiftBox)
DEF_LuaPush(CMenuQuestJournal)

DEF_LuaPush(CWeaponsRank)
DEF_LuaPush(CTeamDescription)
//DEF_LuaPush(CWeaponsPointSystem)


DEF_LuaPush(CQuestionProcess)
DEF_LuaPush(CMenuWareHouseProcess)
DEF_LuaPush(CMenuAutoWalkProcess)
DEF_LuaPush(CMenuSellYuanBaoProcess)
DEF_LuaPush(CMenuTargetEquipProcess)
DEF_LuaPush(CRelationSystemProcess)
DEF_LuaPush(CWantedProcess)
DEF_LuaPush(CGenerationProcess)
DEF_LuaPush(CMenuPartnerCultivationUpProcess)
%}

%import (module = "CommonI") "CommonI.i"

#define DWORD	 unsigned int
#define D3DCOLOR unsigned int

#define VK_RETURN	0x0D
#define VK_TAB		0x09
#define VK_DELETE		0x2E

#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101
#define WM_CHAR                         0x0102

#define WM_MOUSEMOVE                    0x0200

#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203

#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206


enum CFONT_STYLE
{
	EDGE_FONT1  = 0x1 << 1,
	EDGE_FONT2  = 0x2 << 1,
	EDGE_SHADOW = 0x3 << 1,
	EDGE_LIGHT	= 0x4 << 1,
};

//entity animation type
enum StateType
{
	ANIM_IDLE1 = 0,
	ANIM_RUN,
	ANIM_SIT,
	ANIM_W1BAT,
	ANIM_W1ATK1,
	ANIM_W1ATK2,
	ANIM_W2BAT,
	ANIM_W2ATK1,
	ANIM_W2ATK2,
	ANIM_SKILL,
	ANIM_HURT,
	ANIM_DIE,
	ANIM_DEF,
	ANIM_USE,
	ANIM_JUMP_START,
	ANIM_JUMP,
	ANIM_JUMP_END,
	ANIM_IDLE2,
	ANIM_WALK,
	MAX_ANIM_TYPE,
};


enum BUTTON_STATE
{
	_ButtonNormal = 0,
	_ButtonMouseOver,
	_ButtonClick,
	_ButtonDisable,
	_MaxButton_State
};



//frame rotation
enum FrameRotation
{
	SOUTH = 0,
	SOUTHEAST,
	EAST,
	NORTHEAST,
	NORTH,
	NORTHWEST,
	WEST,
	SOUTHWEST,
	MAXROTATION,
};

UInt32	HexToDWORD(PCSTRINGA HexStr);

class UIItem
{
public:
	UIItem();
	~UIItem();
	
	void Create();
	void Dispose();
	void Validate(CImage *pImage);
	void Invalidate();
	void Update(UInt32 dwDelta);
	void Render(CImage *pImage);

	void SetPos(Int32 pX, Int32 pY);
	void MovePos(Int32 pX, Int32 pY);
	

	Int32 GetX();
	Int32 GetY();
	UInt32 GetOffsetX();
	UInt32 GetOffsetY();
	UInt32 GetWidth();
	UInt32 GetHeight();
	UInt32 IsPickUp();
	void PickUp();
	void  UnPickUp();
	void SetOffsetXY(Int32 pX, Int32 pY);
	void SetOffsetXYOnly(Int32 pX, Int32 pY);
	void SetOffsetPos(Int32 pX, Int32 pY);
	void SetLuaState(lua_State *pLuaState);
	
	void SetFocus(bool pState);
	bool GetFocus();
	
	UIItem *GetHotSpot();
	UIItem *GetMouseOver();

	void SetName(const char *pStr);
	
	 
	void SetLastKey(WPARAM Key);
	
	WPARAM GetLastKey();
	
	void SetLastWndMsg(UInt32 Msg);
	
	UInt32 GetLastWndMsg();
	
	void SetEnable(bool Flag);
	bool IsEnable();
	
	bool CheckName(StringA &Name);
	bool CheckName(PCSTRINGA Name);
	
	bool Equal(UIItem *pItem);
	
	void SetNotDetected(bool value);
	
	void SetDetectRect();
	void SetDetectRect(UInt32 Left, UInt32 Top, UInt32 Right, UInt32 Bottom);
	
	void SetDragRect(UInt32 xLeft, UInt32 yTop, UInt32 xRight, UInt32 yBottom);
	
	void SetColor(UInt32 Color);
	void SetNext(PCSTRINGA pStr);
	PCSTRINGA GetNext();
	void SetPrev(PCSTRINGA pStr);
	PCSTRINGA GetPrev();
	void SetCanDrag(UInt32 Code);
	
	void SetDetectImage(bool Flag);
	bool IsDelectImage();
	
	void SetRenderStyle(UInt32 Style);
	UInt32 GetRenderStyle();
	
	void SetTimeOutCount(UInt32 Value);
	void SetTimeOutPreTime(UInt32 Value);
	void SetTimeOutState(Int Value);
	void SetEnableTimeOutEvent(bool Value);

	void ResetTimer();
	UInt32 GetTimeOutCount();
	UInt32 GetTimeOutPreTime();
	Int GetTimeOutState();
	bool GetEnableTimeOutEvent();
	
	UIItem *GetUIItemPtr();
	
	lua_State *GetLuaState();
	
	void SetLuaFunc(UInt32 pIndex, PCSTRINGA pFuncStr);
	void ChangeParent(UIItem *Parent);
};


class UIStaticText : public UIItem
{
public:
	void SetFont(PCSTRINGA pName, UInt32 pWidth, UInt32 pHeight);
	
	void SetText(PCSTRINGW pText);
	void LuaSetText(PCSTRINGA pText);
	void SetFontStyle(UInt32 pFontStyle);
	
	void SetFontColor(D3DCOLOR pColor);
	void SetShadowColor(D3DCOLOR pColor);
	void SetImage(UInt32 pRow, UInt32 pColumn);
	void SetImageWH(UInt32 Width, UInt32 Height);
	void FillImage(UInt32 pColor);
	void DrawFont();
	void Redraw();
	
	void SetPassword(Boolean Flag);
	
	void ClearText();
	PCSTRINGW GetText();
	UInt32 GetTextSize();
	bool Empty();
	
	void SetAlign(UInt32 Align);
	void SetLinePitch(UInt32 LinePitch);
	
	void SetRenderStyle(UInt32 Style);
	UInt32 GetRenderStyle();
	
	UIStaticText *GetUIStaticText();
	void SetNumOnly(Boolean Flag);
	
	void SetSelColor(UInt32 SelColor);
	UInt32 GetSelColor();
	void SetSelStart(UInt32 SelStart);
	UInt32 GetSelStart();
	void SetSelEnd(UInt32 SelEnd);
	UInt32 GetSelEnd();
};


class UIButton : public UIItem
{
public:
	void SetDetectRect();
	void SetDetectRect(UInt32 Left, UInt32 Top, UInt32 Right, UInt32 Bottom);
	void SetStateDetectRect(BUTTON_STATE pState);
	
	Int32 GetX();
	Int32 GetY();
	void SetPos(Int32 pX, Int32 pY);
	void SetNormalState();
	void SetDisableState();
	void SetMouseOverState();
	void SetClickState();
	void SetNoticeState();
	void SetSubOffSet(Int32 pIndex, Int32 OffX, Int32 OffY);
	void SetOffsetPos(Int32 pX, Int32 pY);
	void SetLockState(bool State);
	void SetRenderStyle(UInt32 Style);
	UInt32 GetRenderStyle();
	bool IsLockState();
	
	void SetState(BUTTON_STATE pState);
	void SetImageFileA(PCSTRINGA	ImageName);
	void SetRect(Int32 pIndex, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	
	void SetAniIcon(Int32 pIndex, UIAniIcon * pAniIcon);
	Int32 GetState();
	
};

class UICheckBox : public UIItem
{
	public:
	void	SetState(bool pState);
	
	bool GetState();
	
	void SetRenderStyle(UInt32 Style);
	UInt32 GetRenderStyle();
	
	void SetCheckXY(UInt32 X, UInt32 Y);
	void SetImageFileA(PCSTRINGA ImageName);
	void SetRect(Int32 pIndex, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
};

class UIScrollBarV : public UIItem
{
public:
	void InitButton();
	UIButton *GetPrevBtn();
	UIButton *GetNextBtn();
	UIButton *GetMoveBtn();
	void SetPrevButton(UIButton *pButton);
	void SetNextButton(UIButton *pButton);
	void SetMoveButton(UIButton *pButton);
	
	
	void SetBarRect(Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	void SetBarWH(UInt32 pWidth, UInt16 pHeight);
	void SetBarHeight(UInt32 pHeight);
	

	void SetNumCount(UInt32	pNumCount);
	UInt32 GetNumCount();
	UInt32 GetSelNum();

	void SetSelNum(UInt32 SelNum);
	
	void MoveUp();
	void MoveDown();

	void HomeScroll();
	void EndScroll();

	void ResetNumCount(UInt32	pNumCount);

	void SetBarRenderType(UInt32 pType);
	void SetBarColor(D3DCOLOR pBarColor);
	void SetBarInfo();
	
	Int32	OnMouseWheel(Int32 pX, Int32 pY, Int32 pZ);

};


class UIScrollBarH : public UIScrollBarV
{
	
};


///////////////////////////////////////////////////////////////////////////


class UIScrollListBox : public UIItem
{
public:
	void SetupScrollBarV();
	void SetupScrollBarH();
	void SetPos(Int32 pX, Int32 pY);
	void MovePos(Int32 pX, Int32 pY);
	UIListBox	 *GetListBox();
	UIScrollBarV *GetScrollBarV();
	UIScrollBarH *GetScrollBarH();
	void UnPickUp();

};


class UIListBox : public UIStaticText
{
public:
	void SetBarWidth(UInt32	Width);
	void SetDetectRect();
	void SetBarRect(RECT &rect, Int32 pIndex);

	void AddString(PCSTRINGW pString);
	void AddStringA(PCSTRINGA Text);
	void DelString(UInt32 pIndex);
	void DrawFont();

	void UpSel();
	void DownSel();
	void Home();
	void End();
	void PageUp();
	void PageDown();
	
	
	Int32 GetBarPos();
	PCSTRINGW GetBarString();
	UInt32 GetStringListSize();

	
};


class UIPopUpMenu : public UIListBox
{
public:
	
	Int32 GetBarPos();
	PCSTRINGW GetBarString();
	Int32 GetBarCode();
	
	UInt32 GetStringListSize();
	
	void SetRenderStyle(UInt32 Style);
	UInt32 GetRenderStyle();
	
	UIStaticText *GetUIStaticText();
	
	void SetLinePitch(UInt32 LinePitch);
	void SetBarWidth(UInt32	Width);
	void SetSelBarColor(D3DCOLOR pSelBarColor);
	void SetSelFontColor(D3DCOLOR pSelFontColor);
};

class UIDynamicText : public UIStaticText
{
public:
	void SetSpeed(Int32 value);
};

class UIEditBox : public UIStaticText 
{
public:
	
	void SetText(PCSTRINGW pText);
	void ClearText();
	UInt32 GetEditLimit();
	PCSTRINGW GetText();	
	UInt32 GetTextSize();
	bool Empty();
	void BackSpace();
	void InsertText(PCSTRINGA Text);
	void SetText(PCSTRINGA pText);
	void AddText(PCSTRINGW pText);
	void AddText(PCSTRINGA pText);
	void SetText(StringW	&pText);
	
	void ClearWaitText();
	void AddWaitText(PCSTRINGW pText);
	void AddWaitText(PCSTRINGA pText);
	void SendWaitText();

	
	void SetFontColor(D3DCOLOR pColor);	
	void Redraw();
	
	void SetRenderStyle(UInt32 Style);
	UInt32 GetRenderStyle();
	
	int GetInt();
	void SetEditLimit(UInt32 Limit);
	
	UInt32 Home();
	UInt32 End();
	void CursorMoveLeft(UInt Count);
	void CursorMoveRight(UInt Count);
	
};

class UIEditBoxEx  : public UIEditBox
{
public:	
	PCSTRINGW GetText();	
	UInt32 GetTextSize();
};



class UIChatItem
{
public:
	void SetPos(Int32 pX, Int32 pY);	
	UInt32 GetWidth();	
	UInt32 GetHeight();	
	
	bool Detect(Int32 pX, Int32 pY);	
	void UpdateDetectRect(Int32 pStartX, Int32 pStartY);
	
	PCSTRINGW GetString();
	PCSTRINGW GetLinkString();
	PCSTRINGW GetLinkSubString(UInt32 Start);
	
	void SetLinkType(UInt32	LinkType);
	UInt32 GetLinkType();
	
};


class UIChatString : public UIChatItem
{
public:
	void SetString(PCSTRINGW pStr);
	void SetStringA(PCSTRINGA pStr);
	UIChatItem *GetUIChatItemPtr();
	PCSTRINGW GetString();
};



class UIChatLink : public UIChatString
{
public:
	PCSTRINGW GetLinkString();
	
	UIChatItem *GetUIChatItemPtr();
	UIChatString *GetUIChatStringPtr();
};


class UIChatRow
{
public:
	void AddItem(UIChatItem *pItem);
	UIChatItem *GetChatItem(UInt32 pIndex);

	void AddStringW(PCSTRINGW pString, D3DCOLOR Color, D3DCOLOR ShadowColor, UInt32 FontStyle, UInt32 m_Height);
	void AddStringA(PCSTRINGA pString, D3DCOLOR Color, D3DCOLOR ShadowColor, UInt32 FontStyle, UInt32 m_Height);

	UInt32 GetHeight();
	UInt32 GetWidth();	
};



class UIChatPanel : public UIStaticText
{
public:
	UIChatRow *CreateRow();
	UIChatRow *GetRow(UInt32 pIndex);
	UIChatRow *GetLastRow();
	UIChatRow *GetSafeLastRow();
	
	UIButton *CreatePrevBtn();
	UIButton *CreateNextBtn();
	UIButton *CreateHomeBtn();
	UIButton *CreateEndBtn();
	
	void Start2BottomRow();
	void End2TopRow();
	void UpRow();
	void DownRow();
	void HomeRow();
	void EndRow();
	void SetRow(UInt32 a, UInt32 b);

	UInt32 GetFontHeight();
	
	void SetEnableScroll(Boolean EnableScroll);
	Boolean IsEnableScroll();
	void ClearAllRow();
	
	void SetDetectRect();
	void SetImage(UInt32 pWidth, UInt32 pHeight);
	
	void Redraw();
	void FillImage(UInt32 pColor);
	
	UInt32 GetWidth();
	UInt32 GetHeight();
	
	void SetFontColor(D3DCOLOR pColor);	
	
	void AddStringA(PCSTRINGA pString);
	void AddStringW(PCSTRINGW pString);
	void AddStringWNoPrefix(PCSTRINGW pString);
	
	UInt32 RowListCount();
	UIChatRow *GetHomeRow();
	
	UInt32 GetTotalHeight();
	UInt32 GetAllRowHeight();
	
	Int32 GetStartRowIndex();
	Int32 GetEndRowIndex();
	
	void SetImageWH(UInt32 Width, UInt32 Height);
	
	void SetContinueRun(bool Flag);
	
	void LockChatPanel();
	void UnLockChatPanel();
	
	bool IsEndRow();
	bool IsStartRow();
	
	void UpdateDetectRect();
	
	UIChatItem	*GetLinkItem();
	void		ClearLinkItem();
};



class UIChatEditPanel : public UIChatPanel
{
public:
	void LoadText();
	void SaveText();
};



class UIMiniMap : public UIItem
{
public:
	UIMiniMap();
	~UIMiniMap();

	void SetupRatio(UInt32 MapWidth, UInt32 MapHeight);
	
	UInt ResetPlayerPos(UInt X, UInt Y);
	void SetImageA(PCSTRINGA	Filename);
	
	void SetDefaultMask();
	void SetPlayerImageA(PCSTRINGA	Filename);
	void SetPlayerImageRect(Int Left, Int Top, Int Right, Int Bottom);
};


class UIPanel : public UIItem
{
public:
	void SetDetectRect();
	
	Int32 GetX();
	Int32 GetY();
	
	void SetPos(Int32 pX, Int32 pY);
	
	void SetRenderStyle(UInt32 Style);
	UInt32 GetRenderStyle();
	
	void SetEnable(bool Flag);
	void SetAllColor(UInt32 Color);
	UIItem *GetMouseOver();
	
	void SetImageFileA(PCSTRINGA ImageName, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	
	void SetTextureUI(UIManager *pUIManager);
	void SetTextureWH(UInt32 TWidth, UInt32 THeight);
	void SetWH(UInt32 TWidth, UInt32 THeight);
	void SetImageNil(Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	
	void AddUIItem(UIItem *pItem);
	
	void SetAutoDelete(Boolean flag);
	
	void SetLuaPerLButtonUp(PCSTRINGA	LuaPerLButtonUp);
	void SetLuaPerRButtonUp(PCSTRINGA	LuaPerRButtonUp);
	
	void Freeze();
	void Unfreeze();
	bool IsFreeze();
};


class UIImage : public UIItem
{
public:
	void SetDetectRect();
	
	Int32 GetX();
	Int32 GetY();
	
	void SetPos(Int32 pX, Int32 pY);
	
	void SetImageFileA(PCSTRINGA Filename, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	void SetImageFile(PCSTRING Filename, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	void SetImage(CImage *pImage, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	
	void FillImage(UInt32 pColor);

	void SetWidth(UInt32 Width);
	void SetHeight(UInt32 Height);

	void Set2TopLeft();
	void Set2BottomLeft();
	void Set2TopRight();
	void Set2BottomRight();
	
	void SetAutoDelete(Boolean flag);
	
	UIImage *GetUIImagePtr();
};

class UILinerImage : public UIImage
{
public:
	void SetDetectRect();
	
	Int32 GetX();
	Int32 GetY();
	
	void SetPos(Int32 pX, Int32 pY);
	
	void SetImageFileA(PCSTRINGA Filename, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	void SetImageFile(PCSTRING Filename, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);

	void SetWidth(UInt32 Width);
	void SetHeight(UInt32 Height);
	
	void SetLinerWidth(UInt32 Width);
	UInt32 GetLinerWidth();
	void SetBaseWidth(UInt32 Width);
	void SetDenumerator(UInt32 Denumerator);
	void SetNumerator(UInt32 Numerator);
	void FullWidth();
	
	
	
	void SetRenderStyle(UInt32 Style);
	UInt32 GetRenderStyle();
	
	void Set2Top();
	void Set2Bottom();
};


class UILinerImageV : public UILinerImage
{
public:
	void SetDetectRect();
	
	Int32 GetX();
	Int32 GetY();
	
	void SetPos(Int32 pX, Int32 pY);
	
	void SetImageFileA(PCSTRINGA Filename, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	void SetImageFile(PCSTRING Filename, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);

	void SetWidth(UInt32 Width);
	void SetHeight(UInt32 Height);
	
	void SetLinerHeight(UInt32 Height);
	UInt32 GetLinerHeight();
	void SetBaseHeight(UInt32 Height);
	void SetDenumerator(UInt32 Denumerator);
	void SetNumerator(UInt32 Numerator);
	void FullWidth();
	
	void SetRenderStyle(UInt32 Style);
	UInt32 GetRenderStyle();
	
	void Set2Left();
	void Set2Right();
	
};


class UIPanelEx : public UIPanel
{
public:
	UInt32 GetLineCount();
	void ChangeCount(UInt32 LineCount);
	void SetDetectRect();
	
	Int32 GetX();
	Int32 GetY();
	
	void SetPos(Int32 pX, Int32 pY);
	
	void SetRenderStyle(UInt32 Style);
	UInt32 GetRenderStyle();
	
	void SetAllColor(UInt32 Color);
	
	RECT * GetLineRect();
	UInt32 GetLineRectHeight();
	void SetEnable(bool Flag);
	bool IsEnable();
	
	void SetLineRect(Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom, UInt32 Count);
	void SetBottomRect(Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
};


class ChatPool
{
public:
	void Clear();
	void Add(PCSTRING Msg);
	PCSTRING GetStartMsg();
	PCSTRING GetLastMsg();
	PCSTRING GetNextMsg();
	PCSTRING GetMsgFormIndex(UInt Index);
	UInt32 GetIndex();
	void ResetCopyIndex();
	UInt32 GetCopyIndex();
};

class UIAniIcon : public UIImage
{
public:
	void SetAnimationData(UIAniIcon * pAniIcon);
	void SetAnimationIndex(Int AnimationIndex);
	void SetAnimationDataA(PCSTRINGA Filename, int equipType = 0);
	void SetRotation(Int Rotation);
	void ClearAniData();
	void LockResources();
	
	void SetDetect(Int Value);
	void SetAlign(Int Value);
	virtual UInt32 Update(UInt32 dwDelta);
	virtual UInt32 UpdateNow();
	bool GetHasAni();
};


class UIManager
{
public:
	UIManager();
	~UIManager();
	Int32 GetCursorX();
	Int32 GetCursorY();
	
	bool  Detect(PCSTRINGA Name, Int32 CursorX, Int32 CursorY);

	void RenderAll();	

	UIItem *GetUIItem(PCSTRINGA pName);
	UIItem *GetWaitItem();
	
	void SetBackImageA(PCSTRINGA ImageName, UInt32 Left, UInt32 Top, UInt32 Right, UInt32 Bottom);
	
	void RegisterImage(PCSTRING pStr);
	void RegisterImageA(PCSTRINGA ImageName);
	
	void SetIDName(PCSTRINGA pStr);
	
	const char *GetIDName();

	UIPanel *FindUIPanel(PCSTRINGA pName);	
	UIPanelEx *FindUIPanelEx(PCSTRINGA pName);	
	UIStaticText *FindUIStaticText(PCSTRINGA pName);	
	UIButton *FindUIButton(PCSTRINGA pName);	
	UIEditBox *FindUIEditBox(PCSTRINGA pName);	
	UIEditBoxEx *FindUIEditBoxEx(PCSTRINGA pName);
	UIPopUpMenu *FindUIPopUpMenu(PCSTRINGA pName);	
	UIScrollBarV *FindUIScrollBarV(PCSTRINGA pName);
	
	UIChatPanel *FindUIChatPanel(PCSTRINGA pName);	
	UIMiniMap *FindUIMiniMap(PCSTRINGA pName);
	UIImage	*FindUIImage(PCSTRINGA pName);
	UILinerImage *FindUILinerImage(PCSTRINGA pName);
	UICheckBox *FindUICheckBox(PCSTRINGA pName);	
	UIAniIcon *FindUIAniIcon(PCSTRINGA pName);
	
	
	void AddLockUIPanel(UIPanel *pUIPanel);
	void ClearLockUIPanel();
	void LockPanelActive();
	void BasePanelActive();
	
	void SetMonoItem(UIItem *MonoPanel);
	void SetMonoPanel(UIPanel *MonoPanel);
	void SetMonoPanelEx(UIPanelEx *MonoPanel);
	UIItem *GetMonoItem();
	UIPanel *GetMonoPanel();
	UIPanelEx *GetMonoPanelEx();
	void ClearMonoPanel();
	void LockMonoPanel(PCSTRINGA Pass);
	void UnlockMonoPanel(PCSTRINGA Pass);
	void SetLockMonoPanel(bool Flag);
	bool IsLockMonoPanel();
	void SetFollowPanel(PCSTRINGA PanelA, PCSTRINGA PanelB);
	
	void ChangeFocus(PCSTRINGA pName);
	
	void  SetEvent(Int32 Event);
	Int32 GetEvent();
	Int32 CatchEvent();
	
	void SetLeaveTimerItem(UIItem *Item, UInt32 ShiftTime);
	void ClearLeaveTimerItem();
	
	void SetMoveInTimerItem(UIItem *Item, UInt32 ShiftTime);
	void ClearMoveInTimerItem();
	void ClearStateItem();
	
	void SetDragPanel2Top();
	bool CheckPanelOnTop(UIPanel *pPanel);
	UIPanel *GetPanelOnTop(UIPanel *pPanel);
	void ChangeParent();
	
	void DeletePanel(PCSTRINGA ItemName);
	
	void AddDirtyUIItem(UIItem * pItem);
	
	void SetDefaultEnterFunc(PCSTRINGA pFuncName);
	
	void AddUIID(PCSTRINGA pName, UIItem *pUIItem);
	
	void SetLuaFuncName(PCSTRINGA	LuaFuncName);
	
	void ResetTipsPanel(UIItem *Item);
	
	void AddUIPanel(UIPanel *pUIPanel);
	void AddUpdateUIItem(UIItem *pUIItem);
	
	void ResetDragItem();
	void StopDrag();
	bool DragNow();
	void SetDragItemXY(UIItem *Item, Int32 OffsetX, Int32 OffsetY);
	UIItem *GetKeyInItem();
	void SetKeyInItem(PCSTRINGA pName);
	UIItem *GetDropPanel();
	
	void CloseTips();
	
	void InsertToInOutEvent(UIItem * pItem);
	void RemoveFromInOutEvent(UIItem * pItem);
	void SetPanel2Top(UIPanel * pPanel);
	void SetPanelEx2Top(UIPanelEx * pPanel);
	void SetPanel2Bottom(UIPanel *pPanel);
	void SetPanelEx2Bottom(UIPanelEx *pPanel);
	
	void SetBackImage(CImage *pImage, UInt32 Left, UInt32 Top, UInt32 Right, UInt32 Bottom);
	CImage * GetBackImage();
	
	lua_State *GetLuaState();
	void LoadLuaUI(PCSTRINGA Filename);
	
	void SetAllPanelDisable();
	void RestoreAllPanelEnable();
	
	UIItem * GetFocusItem();
	void UIItemSetEnable(PCSTRINGA Name, bool Flag);
	void RemoveUpdateUIItem(UIItem *pUIItem);
	
	void FreezeAll();
	void UnfreezeAll();
	bool IsFreeze();
};

UIManager *GetUIManager();


class FSound
{
public:
	void Close();
	void Play();
	void Stop();
	void Pause();
	void Resume();
	void SetVolume(int Vol);
};


class FSOUND_Manager
{
public:
	void Init();
	void Close();
	
	void SetMusic(PCSTRINGA pFileName);
	void PlayMusic();
	void StopMusic();
	void PauseMusic();
	void ResumeMusic();
	bool MusicIsPlaying();
	void MusicSetVolume(int Vol);
	
	
	FSound *CreateSoundA(PCSTRINGA pFileName);
	FSound *CreateSound(PCSTRINGW pFileName);
	FSound *GetSound(int Channel);
	FSound *GetSound(PCSTRINGA pFilename);

	void PauseAll();
	void ResumeAll();
};

FSOUND_Manager *GetFSoundManager();




class CEntityBase
{
public:
	void SetEntOffsetJumpY(int Value);
};

class CAnimatedEntity
{
public:
	void SetEntOffsetJumpY(int Value);
	void AppendAnimationSet(int AnimationIndex, int UpX, int UpY, int height, double TimeInMS,
	bool Loop, int Rotation, DWORD StartTime, DWORD JumpDelay, bool StopAnim, 
	bool IsPixel,
	int ShakeX, int ShakeY, int ShakeSpeedX, int ShakeSpeedY, WORD ShakeWidth, WORD ShakeHeight,WORD ShakeTime);
	
	void InitChainData();
	void PushChainData();
	void SetAnimIndexChain(int AnimationIndex, int Rotation, bool Loop, bool StopAnim);
	void SetSharkChain(int ShakeX, int ShakeY, int ShakeSpeedX, int ShakeSpeedY, WORD ShakeWidth, WORD ShakeHeight,WORD ShakeTime);
	void SetJumpDataChain(int height, int BkPosX, int BkPosY, double timeInMS, DWORD StartDelay, bool IsPixel);
	void SetChainStartTime(DWORD Time);
	
	int GetAnimationIndex();
	int GetAnimationRotation();
	
};

class CMenuSkillProcess
{
public:
	void UpdateSkillMenu();
	void UpdateSubSkillMenu(int Index);
	void UpdateMainSkillMenu(int Index);
	void SkillMenuMainUp();
	void SkillMenuMainDown();
	void SkillMenuMainSubUp();
	void SkillMenuMainSubDown();
	void SkillMenuMainScroll();
	void SkillMenuSubScroll();
	Int32 GetUseSkillID(Int Index);
	bool UseSkill(int Index);
	bool UseSkillID(Int ID);
	void OpenSkillDetail();
	void SetSkillDetailIndex(int Index);
	void SetSkillShop(int Value);
	bool GetIsShop();
	void ShopLearn();
	void ResetPosition();
};

class CMenuQuestLogProcess
{
public:
	void CallMissionButtonEffect();
	void OnSelectQuestTrace(bool bSel);
	void OnSelectLogDType(UInt16 nType);
	void UpdateMainQuestLstUI(int index);
	void OnUpdateNoteUI();
	
	void LuaMainLstUpRow();
	void LuaMainLstDownRow();
	void LuaMainLstScrollMove();
	
	void LuaSubDescUpRow();
	void LuaSubDescDownRow();
	void LuaSubDescScrollMove();
	void LuaUpdateSubDescScrollV();
	
	void LuaCancelQuestClient();
	
};

class CMenuPartnerProcess
{
public:
	void UpdatePartnerMenu();
	void AttrUp(int index);
	void AttrDown(int index);
	void AddConfirm();
	void SelectPartner(int index);
	void ScrollPartner(int index); // 0: up 1: down 2: scollmove
	void SetFollow();
	void ResetValue();
	void ChangePartnerName();
	void SetPartnerState(int state); //0: standby 1: fight 2:rest
	void ShowRemovePartnerName();
	void DeleteOnShowPartner();
	void UpdateStateButton();
	void ShowSkillDetail();
	void UpdateBattleSkillMenu();
	bool SelectSkill();
	bool ShortCutSelectSkill(UInt32 Index);
	bool IsBattleSkillExist();
	UInt8 GetSelectIndex();
	void ShowPublicPartnerDetail(UInt32 char_id, UInt32 Partner_id);
	bool HasPartner();
	
	void UpdatePartnerCollectionMenu(UInt8 race, UInt8 page);
	void ShowPartnerCollectionDetail(UInt8 race);
	void PartnerCollectionNextPage(UInt8 race, bool bNext);
	void PartnerCollectionUseBuffTemp(bool bUse);
	void DragPartner();
	void AddPartnerCollection();
	
	void PartnerEatFastFood(UInt8 eatType);
};


class CAntiHack
{
    	
public :
	void SetHack(UInt8 answer);
	
};

class CMenuBankService
{
public:


	void ConfirmBankAction(Boolean credit);
	Boolean CheckDepositAmount(); 
	Boolean CheckWithdrawalAmount();
	void RefreshInputValue(UInt32 money, Boolean saving);	
	void OpenDepositInterface();
	void OpenWithdrawalInterface();
	void SetInputMoneyColour();

};


class CMenuGivenProcess
{

public:
	void ResetAll();
	void SetTargetCharIDNameToPanel();
	void SetItem(int givenIndex, int itemSlot, int stack);
	void RemoveItem(int givenIndex);
	void SetPartner(int givenIndex, UInt32 partnerID);
	void RemovePartner(int givenIndex);
	void SetPet(int givenIndex, UInt32 petID);
	void RemovePet(int givenIndex);
	void ConfirmGiven();
	bool IsNpcGiven();
	bool IsSuitableNpcGivenItem(int itemSlot);
	bool IsSuitableNpcGivenPartner(UInt32 partnerID);
	bool IsSuitableNpcGivenPet(UInt32 petID);
	void ShowMoneyColour();
};

class CMenuItemProcess
{

public:
	void NewUpdateItemMenu(int bag = -1);
	void NewUpdateItemCharacter();
	void UpdateItemPartner(int partnerIndex=-1);
	void Scroll(int up); //0:up, 1:down, 2:scroll
	void UpdateItemMenu(int bag = -1);
	void ResetScroll();
	void ItemUse(int itemPos);
	void DragPartner();
	UInt32 GetSelectedPartnerID();
	bool IsGivenPartner();
	UInt8 GetItemStack(UInt8 index);
	void UpdateItemDetail(int itemPos);
	void ShowPublicItemDetail(UInt32 char_id, UInt32 item_uid);
	bool IsItemBind(UInt8 index);
	bool IsItemGiven(UInt8 index);
	bool IsItemSent(UInt8 index);
	bool NeedRedBox(UInt8 index);
	void BindCantTrading(UInt8 type = 0);
	void UpdateItemPet(UInt8 petIndex=0);
	void UpdateShortCutSlot(int itemPos);
	void DragPet();
	bool IsGivenPet();
	UInt32 GetSelectedPetID();
	bool IsItemSellable(UInt8 index);
	UInt8 GetMenuSelectedPartnerIndex();
	UInt8 GetMenuSelectedPetIndex();
	void ResetIdentify();
	void IdentifyItem();
	void ConfirmAssetLockPassword(Int lockType);
	void ConfirmAssetUnLockPassword(Int index, UInt32 assetType);
	bool IsPartnerLocked(UInt32 partnerID);
	bool IsPetLocked(UInt32 petID);	
	void ForceResetAssetPassword();
	void JpackBag(UInt8 bagType);
	
};

class CBigMap
{
public:
	void DrawButton();
	UInt32 GetButtonCount(int index);
	void ShowPath();
	void StartBigMapDescMode(bool start);
	void ShowDesc();
	void DisableAllFlag();
	void ResetDes();
	void SetOpenBigMap(bool open);
};

class CMenuOnlineShopProcess
{
public:
	void ShowCap(UInt8 pageNo);
	void ShowHotSale(UInt8 pageNo);
	void ShowMix(UInt8 pageNo);
	void ShowAssc(UInt8 pageNo);
	void ShowPet(UInt8 pageNo);
	void ShowDiamond(UInt8 pageNo);
	void ShowBook(UInt8 pageNo);
	void ShowMould(UInt8 pageNo);
	void ShowFireworks(UInt8 pageNo);
	void ShowPartner(UInt8 pageNo);
	void ShowCloth(UInt8 pageNo);
	void ShowOther(UInt8 pageNo);
	void ShowLimitedItem();
	void ShowDiscountItem();
	UInt8 GetShowIndex();
	UInt8 GetPageNo();
	int GetSelectedIndex();
	int GetSelectedDiscountIndex();
	int GetSelectedLimitedIndex();
	void ResetSelectedIndex();
	void ResetSelectedDiscountIndex();
	void ResetSelectedLimitedIndex();
	void SetSelectedIndex(int sel);
	void SetSelectedDiscountIndex(int sel);
	void SetSelectedLimitedIndex(int sel);
	void AskForOnlineShop();
	void CloseOnlineShop();
	void NextPage();
	void PrevPage();
	void ShowItemDetail();
	void ShowDiscountItemDetail();
	void ShowLimitedItemDetail();
	int GetSelectedItemType();
	UInt32 GetSelectedItemPrice(UInt16 stack);
	UInt16 GetMaxSelectedItemStack();
	void SetBuyItemStack(UInt16 stack);
	void ConfirmBuyItem();
	UInt8 GetSelectedItemOverLap();
	void Preview();
	void PreviewRotate(bool left);
	void ShowLackMoneyMessage();
	void OpenChargeShopWebSite(int index);
	
	void UpdateYuanBao(bool fromServer = false);
};

class CMenuGiftBox
{

public:
	void ShowGiftBoxItem();
	void ShowGiftBoxPartner();
	void ShowGiftBoxPet();
	void ShowConfirmGetGiftName(int slot);
	void ConfirmGetGift(int slot);
	void ShowDetail(int slot);
	void CloseGiftBox();
	void UpdateGiftBoxFromServer();
};

struct ChatLst
{	
	CAnimatedEntity * GetEntity();
	ChatLst * GetNext();
	void Init();
};

class CBatBubbleControl
{
public:
	void LuaAddBatBubble(Int pos, Int msgType, PCSTRINGA text);
	void LuaAddEnemyDialogue(UInt32 mobId, int msgType, PCSTRINGA text, int per);

	int LuaGetMobIdCount();
	UInt32 LuaGetMobId(int index);
	PCSTRINGA LuaGetMobDialgoueFile(int index);
};

class CMenuTargetProcess
{
public:
	void PopUpFunctionCall();

	void ShowTargetPopUp();
	void HideTargetMenu();
	
	void UpdateInfo();
	void FindCharNameFriend(PCSTRINGW Name);
	void PrivateMsg();
	void AddToFriend();
	void ChangeGrp();
	void BreakRelation();
	void BlackList();

};

class CMenuBuffIconProcess
{
public:
	void HideAllBuff();
	void ShowBuffIcon();
	void OpenBuffDetail(int index);
	void HideBuffDetail();
};


class CMenuFriendProcess
{
public:
	void ConstructData();
	void RefreshMenu();
	void PageUp();
	void PageDown();
	void ChangeGroup(int index);
	UInt16 GetStartIndex();
	void OpenFriendChat(int index);
	void SendMessageToFriend();
	void RefreshSelfSetting();
	void OpenHistory();
	void FindFriend(PCSTRINGW char_id, PCSTRINGW name);
	void RefreshPopUp(int index);
	void MoveGroup(int memInx, int grpIdx);
	void MoveGroupImm();
	void SetChangeGroup(int index);
	void OpenFriendDetail(int index);
	void OpenTalkingFriendDetail();
	void ConfirmChangeToGroup(int grpIndex);
	void ConfirmSettingChange();
	void NextChatMessage();
	bool HasNextMessage();
	void FriendScroll();
	void FriendChatScroll();
	void FriendHisScroll();
	void ConfirmDelete();
	void ResetTalkingTarget();
	void ClearHistory();
};

class CMenuGuildProcess
{
public:
	void RefreshInfoMenu();
	void RefreshMemberMenu();
	void RefreshRuleMenu();
	void RefreshApplicantMenu();
	void RefreshManorMenu();
	void RequestMemberInfo();
	void RequestApplicantInfo();
	void RequestGuildInfo();
	void RefreshPostMenu();
	
	void RefreshGuildAimEdit();
	void RefreshGuildRuleEdit();
	void ConfirmGuildAimChange();
	void ConfirmGuildRuleChange();
	bool HasGuild();
	void RefreshGuildApplyPanel();
	
	void ShowAim(int index);
	void ApplyConfirm();
	void AddGuildMember();
	void RejectGuildMember();
	void SetMenuIndex(int index);
	void SetGuildPostSelectIndex(int index);	
	void DeleteGuildMember();
	void ConfirmPostChange();
	void ShowInfo();
	void RefreshGuildButton();
	void ConfirmDonate();
	
	void ConfirmGuildNameInput();
	void CancelGuildNameInput();
	bool GetSelectedMember(int index);
	
	void ScrollMember();
	void MemberPageUp();
	void MemberPageDown();
	
	void ScrollGuildApply();
	void GuildApplyPageUp();
	void GuildApplyPageDown();
	
	void LeaveGuild();
	bool IsSelectedMember(bool member);
	void FreeGuild();
	
	void SortMember(int i);
	
	void RefreshGuildBusinessLogPanel();
	void ScrollGuildBusinessLog();
	void GuildBusinessLogPageUp();
	void GuildBusinessLogPageDown();
	void ConfirmGuildBusinessLogSearchYes();
	void SetGuildBusinessLogSelectIndex(int index);
	void ClearGuildBusinessLog();
};

class CMenuPetProcess
{
public:

	void ResetMenu();
	void RefreshMenu();
	void PageUp();
	void PageDown();
	void Scroll();
	void SetSelIndex(Int index);
	void UsePet();
	void PetRest();
	void DumpPet();
	void ChangePetName();
	void ShowRemovePetName();
	void OpenPetSkillDetail(int index);
	void ClosePetSkillDetail();
	void ShowPublicPetDetail(UInt32 char_id, UInt32 pet_id);
	
	Int GetPetSelectIndex();
	bool HasPet();
	bool PetCanUseItem(UInt16 itemSlot);
	void SetUseFoodItem(UInt16 itemSlot);	
	bool CanEatFood();	
	void ReSetFoodUseItem();	
	void PetUseFood();	
	void InitUsePetFoodPanel();
};

class CMenuHelpProcess
{
public:
	void RefreshMenu();
	void PageUp();
	void PageDown();
	void IsOpenChange(int index);
	void ShowTextContent(int index, size_t contentLinePos = 0, UInt len = 0);
	void SearchContent(PCSTRINGW text);
	void ResetSearch();
	void SetIndex(Int index);
	void CalOpenCount();
	void HandleEvent(UInt8 event);
	void HandleGuideEvent(UInt16 ID);
	void CloseNewGuide(UInt8 flag);
};

class CMenuRankProcess
{
public:
	void RefreshMenu();
	void PageUp();
	void PageDown();
	void SetIndex(Int index);
	void RankScroll();
	void FindRank(PCSTRINGW name);
	void SetSelIndex(Int index);
	void SetRankSubType(Int index);
};
class CMenuSystemSetting
{
public:
	
	void SetWindowMode(bool on);
	void SetNightMode(bool on);
	void SetMapAnimeMode(bool on);
	void SetWeatherMode(bool on);
	void SetBlockGivenMode(bool on);
	void SetBlockPartyMode(bool on);
	void SetBlockMessageMode(bool on);
	void SetBlockPKMode(bool on);
	void SetBlockMakeFriendMode(bool on);
	void SetBlockTradingMode(bool on);
	void SetMusicMode(bool on);
	void SetMusicVolume(UInt8 volume);
	void SetSoundEffectMode(bool on);
	void SetSoundEffectVolume(UInt8 volume);
	void SetSaveChatHistory(bool on);
	void SetShowEquipMode(bool on);
	
	bool IsWindowModeEnable();
	bool IsNightModeEnable();
	bool IsMapAnimeModeEnable();
	bool IsWeatherModeEnable();
	bool IsBlockGivenModeEnable();
	bool IsBlockPartyModeEnable();
	bool IsBlockMessageModeEnable();
	bool IsBlockPKModeEnable();
	bool IsBlockMakeFriendModeEnable();
	bool IsBlockTradingModeEnable();
	bool IsMusicModeEnable();
	bool IsSaveChatHistory();
	UInt8 GetTempMusicVolume();
	UInt8 GetMusicVolume();
	bool IsSoundEffectModeEnable();
	UInt8 GetTempSoundEffectVolume();
	UInt8 GetSoundEffectVolume();
	Int GetSoundEffectVolForFmod();
	Int GetMusicVolForFmod();
	bool IsShowEquipModeEnable();
	
	void InitSystemSetting();
	
	void AddMusicVolume();
	void SubMusicVolume();
	void AddSoundEffectVolume();
	void SubSoundEffectVolume();
	void ResetTempVolume();
	void RestoreTempVolumeToDefault();
	
	void WriteSystemSetting();
	void SetSubChatPosition(UInt8 position);
	void SetSubChatEnable(bool enable);
	void SaveBlockSetting();
};

class CMenuTradingProcess
{
public:
	void ConfirmReqTrading();
	void RejectReqTrading();
	void SetItem(int tradingIndex, int itemSlot, int stack);
	void RemoveItem(int tradingIndex);
	void SetPartner(int tradingIndex, UInt32 partnerID);
	void RemovePartner(int tradingIndex);
	void SetLock(bool lock);
	void CancelTrading();
	void SetMoney();
	void CheckMoney();
	void ConfirmTrading();
	bool IsConfirm();
	void ShowItemDetail();
	void ShowPartnerDetail();
	void SetPet(int tradingIndex, UInt32 petID);
	void RemovePet(int tradingIndex);
	void ShowPetDetail();
	void ShowMoneyColour();
};

class CMenuAutoWalkProcess
{
public:
	void StartAutoWalk(UInt32 lastTime);
	void StopAutoWalk();
	void ChangeHitBoss();
	void OnOpenAutoWalk();
	void SetRunning();
	void ShowTime();
};

class CMenuMixProcess
{
public:
	void SetSelectedIndex(int index);
	void ResetMixShopMake();	
	void MixShopMakeProductScroll(UInt8 up);
	void MixShopMakeSourceScroll(UInt8 up);
	bool HaveSelectedItemMix();
	bool IsSuitableItemMix(int srcIconIndex, int itemSlot);
	void SetSourceItem(int srcIconIndex, int itemSlot);
	void SetIronShopItem(int srcIconIndex, int itemSlot, int stack);
	bool IsSuitableIronShopItem(int picIndex, int itemSlot);
	bool IsIronShopMake();
	void RemoveIronItem(int picIndex);
	void ResetIronShop();
	bool IsIronShopRepair();
	bool IsStartMaking();
	void CancelMake();
	void RemoveMakeItem(int sourceIndex);
	void StartServerTime();
	void ResetMixShop();
	bool IsSuitableMixShopItem(int picIndex, int itemSlot);
	void SetMixShopItem(int srcIconIndex, int itemSlot, int stack);
	void RemoveMixItem(int picIndex);
	void SetRemainItemCount(int remainCount);
	void SetMakeAll(bool makeAll);
	bool IsRepairShop();
	bool IsStoneMixShop();
	int GetMaxStoneNumByItemSlot(int itemSlot);
	bool HasResultStone(int itemSlot, bool showErrMessage);
	bool IsUnSoulBoundShop();
	int GetUnSoulBoundStoneNum();
	bool IsSuitableGemMixShopItem(int picIndex, int itemSlot);
	bool IsGemMixShop();
	void SetGemMixShopItem(int srcIconIndex, int itemSlot, int stack);
	bool IsGemInlayShop();
	bool IsGemRemoveShop();
	bool IsGemMeltingShop();
	bool IsGemPolishingShop();
	bool IsColorStoneMixShop();
	void SetGemRemoveIndex(int index);
	void UpdateMixItemDetail(int itemPos);
	bool isShowItem(int index);
};

class CMenuTitleProcess
{
public:
	void RefreshMenu();
	void PageUp();
	void PageDown();
	void Sroll();
	void SetSelectIndex(Int index);
	void ConfirmChange();

};

class CMenuHawkerProcess
{
public:
	void UpdateOwnerItem(int itemBag = -1);
	void UpdateOwnerItemSlot(UInt8 itemSlot, bool addDirty = false);
	void UpdateOwnerEqItem();
	void UpdateOwner();
	void UpdateOwnerModel();
	void UpdateOwnerPartner(int index = -1);
	void UpdateOwnerPet(int petIndex = -1);
	void SetIconOff(int iconPos, bool eq);
	void UpdateOwnerPartnerNameList();
	
	void SelectHawkItem();
	void SelectHawkPartner();
	void SelectHawkPet();
	void SelectHawkExchangeRecord();
	
	void SetItemHawk(UInt8 itemIndex, UInt8 stack, UInt32 price);
	void SetPartnerHawk(UInt32 price);
	void SetPetHawk(UInt32 price);
	
	void TurnPage(bool next);
	void SelectPartnerOrPet(UInt8 index);
	
	bool NotForSellPartner(bool showMsg = false);
	bool NotForSellPet(bool showMsg = false);

	void ResetHawkerOwner();
	void DragPartner();
	void DragPet();
	void ResetSellItem(UInt8 index);
	UInt8 GetHawkMenuIndex();
	void StartHawk();
	void StopHawk();
	bool IsStartHawk();
	void ClickHawkAlertPanel();
	
	void SelectHawkSellItem();
	void SelectHawkSellPartner();
	void SelectHawkSellPet();
	
	void ShowHawkMessage(UInt8 index);
	void ChangeHawkName();
	void CloseHawkFromTarget();
	
	int GetMaxItemStackForHawk(UInt8 index);
	void BuyHawkItem(UInt8 itemIndex, UInt8 itemMenuIndex, UInt8 itemStack);
	void ShowHawkSelltemDetail();
	
	int GetItemPriceForHawk(UInt8 index);
	void ShowItemPriceForHawk(UInt8 index, UInt8 stack);
	void HawkerOwnerNameListScroll(UInt8 index);
	void ResetOwnerScroll();
	void SetItemMoneyColour();
	void InitItemPartnerPet();
	void SetConcernTarget();
};

class CMenuQuestJournal
{
public:
	void CloseQuestJournal();
	void ShowQuestJournal();
	void SelectJournal(int index);
	void JournalScroll(int scroll);
	void DBClickJournal(int index);
};

class CWeaponsRank
{
public:
	void CloseWeaponsRank();
	void ShowWeaponsRank();
	void SelectWeaponsRank(int index);
	void WeaponsRankScroll(int scroll);
	void DBClickWeaponsRank(int index);
};


class CTeamDescription
{
public:
	void CloseTeamDescription();
	void ShowTeamDescription();
	void SetTeamDescriptionData();
	void TeamDescriptionTabControl(int index);
	void TeamDescriptionInput(PCSTRINGW text);
	void TeamDescriptionLevelDown(int nNum);
	void TeamDescriptionLevelUp(int nNum);
	UInt16 GetDescriptionLevelDown();
	UInt16 GetDescriptionLevelUp();
};

//class CWeaponsPointSystem
//{
//public:
//	void GetPlayerEquip(const UInt32 & nIndex);
//};



class CQuestionProcess
{
public:
	void Answer(UInt32 index);
	void EnterQuestion();
	void ExitQuestion();
};

class CMenuWareHouseProcess
{
public:
	void ShowWareHouseItem(int pageNo = 0);
	void ShowWareHousePartner();
	void ShowWareHousePet();
	void CloseWareHouse();
	void PutToWareHouse(int desIndex, int srcIndex, int stack);
	int GetMaxStack(int index);
	void GetFromWareHouse(int desIndex, int srcIndex, int stack);
	bool CheckCanSwap(int itemBagIndex, int wareHouseIndex);
	bool AvailSlot(int wareHouseIndex);
	void ShowItemDetail();
	void MoveItem(int srcIndex, int desIndex);
	void PutPartnerToWareHouse(UInt32 partnerID);
	void GetPartnerFromWareHouse(int wareHouseIndex);
	void ShowPartnerDetail();
	void PutPetToWareHouse(UInt32 petUID);
	void GetPetFromWareHouse(int wareHouseIndex);
	void ShowPetDetail();
	void JpackWareHouse(UInt8 itemPage);
};

class CMenuSellYuanBaoProcess
{
public:
	void OpenSellYBMenuChoice();
	void OpenChangeYBSelect();
	void OpenExchangeYBYBT(int type);
	void CloseSellYuanBao();
	void CheckYBTAmount();
	void ConfirmChangeYBYBT(int amount);
	void InitStartPos();
	void OpenSellYBBuySell(int type);
	void PageUpAndDown(bool up);
	void ConfirmSellYB(int amount, int pricePerUnit);
	void SetConfirmBoxText(int YBT, int pricePerUnit);
	void SelectBuyYBT(int index);
	void ConfirmBuyYB();
	void DisplayBuyYBConfirmText();
	void ResetSelectBuyYBT();
	void GetSellYBTempBank();
	void GetAllTBMoneyYBT();
	void SetEditBoxColor(PCSTRINGA staticTextName, UInt32 value);
};

class CMenuTargetEquipProcess
{
public:
	void RequestTargetEquip();
	void FreeTargetEquip();
	void UpdateTargetEquipDetail(int bagIndex);
	bool HasTargetEquip(int bagIndex);
};

class CRelationSystemProcess
{
public:
	void SetSiblingTitlePrefix();
	void SetSiblingTitleSuffix();
	void ChooseTimeScroll(UInt16 scroll);
	void ShowChooseTime();
	void SelectChooseTime(int index);
	void EstablishChooseTime();
};

class CWantedProcess
{
public:
	void ClearAllWanted();
	void SelectWanted(int index);
	void WantedScroll(int scroll); //0: up, 1:down, 2: scroll
	void SelectWantedClass(UInt32 nClass);
	void RequestWantedReward();
	void WantedReward(UInt32 money);
	void WantedDetail();
	void WantedJailDelivery(UInt32 char_id);
	void WantedBail(UInt32 char_id);
	void WantedTrace(UInt32 char_id);
};

class CGenerationProcess
{
public:
	void SortListByLevel();
	void SortListByClass();
	void SortListByOffline();
	void SortListByAccOrExp();
	void SortListByGiftOrHisAcc();
	void GetSavedExp();
	void DisbandGeneration();
	void RequestCreateGeneration(UInt32 ui32TargetId);
	void RequestGenerationAns(bool bAllow);
	void ClearGenerationRequest();
	void SystemDesc(UInt16 uiContextId);
	void UpdateGeneration(bool bForceOpen);
	void UpdateDisbandPanel();
	void UpDateRequestPanel();
	void ShowGenerations();
	void SelectGeneration(int index);
	void GenerationScroll(int scroll);
	void HideAllPanel();
	void ShowAllPanel();
	void OpenGenCharDetail();
};

class CMenuPartnerCultivationUpProcess
{
public:
	void OpenPartnerCultivationUp(int type);
	int GetPartnerCultivationUpType();
	void ResetPartnerCultivationUp();
	void ClosePartnerCultivationUp();
	
	void SetSrcPartner(UInt32 Uid);
	void SetDestPartner(UInt32 Uid);
	
	bool IsSuitableSrcPartner(UInt32 PartnerID);
	bool IsSuitableDestPartner(UInt32 PartnerID);

	void PartnerCultivationUpByPartner();
	void PartnerCultivationUpByItem();
};

class CAccShopProcess
{
public:
	void ShowAccShop();
	void SelectAccShopItem(int index);
	void AccShopPrevPage();
	void AccShopNextPage();
	void AccShopBuyItem();
	void OnMouseMoveOver(UInt32 uiIndex);
};

class CPointsShopProcess
{
public:
	void ShowPointsShop();
	void SelectPointsShopItem(int index);
	void PointsShopPrevPage();
	void PointsShopNextPage();
	void PointsShopBuyItem();
	void OnMouseMoveOver(UInt32 uiIndex);
};
void GetPlayerPos(int &X, int &Y);
UInt32 GetMapWidth();
UInt32 GetMapHeight();
void AddXMLFile2UIManger(PCSTRINGA Filename);
void UIMangerLoadUI(UIManager *pUIManager, PCSTRINGA Filename);

void OpenSubWindow();
void CloseSubWindow();

void PostQuitMessage(int ExitCode);

void OutputDebugStringA(const char *Str);

bool CheckShiftDown();

int Div(int a, int b);
UInt32 DivUInt32(UInt32 a, UInt32 b);
bool LAnd(bool a, bool b);
bool LOr(bool a, bool b);
int LMod(int a, int b);

void ChangeChannel(int i);
void ReceiveChannel(int i);
void RejectChannel(int i);
void InitChannelEnable();
bool IsChannelMsg(PCSTRING msg, int Type);
void AddChatIconToPanel();
void AddChatIconToEditLine();
void ChangeEditLineColor(int index);
bool GetLisentToChannel(int index);
void SendPrivateMessageTo(PCSTRINGW Name);
void InitRecentChatListBox();
void PlayerChooseResult(int opt);

void UpdateClockPanel(bool bInit);
void UpdateRightUpTimeInfo();

UIManager* GetSubUIManager();
CMenuSkillProcess * GetSkillProcess();
CMenuQuestLogProcess * GetQuestLogProcess();
ChatPool *GetChatPool();
CMenuPartnerProcess * GetPartnerProcess();

CMenuGivenProcess* GetGivenProcess();
CMenuItemProcess* GetItemProcess();
CBigMap* GetBigMap();
CMenuTargetProcess* GetMenuTargetProcess();
CMenuBuffIconProcess* GetMenuBuffIconProcess();
CMenuSystemSetting* GetMenuSystemSetting();
CMenuFriendProcess* GetMenuFriendProcess();
CMenuTradingProcess* GetTradingProcess();
CMenuBankService* GetBankProcess();
CMenuHelpProcess* GetMenuHelpProcess();
CMenuOnlineShopProcess* GetOnlineShopProcess();
CMenuRankProcess* GetMenuRankProcess();
CMenuPetProcess* GetMenuPetProcess();
CMenuMixProcess* GetMixProcess();
CMenuTitleProcess* GetTitleProcess();
CAntiHack* GetAntiHackProcess();
CMenuHawkerProcess* GetHawkerProcess();
CMenuGuildProcess* GetMenuGuildProcess();
CMenuGiftBox* GetGiftBoxProcess();
CMenuQuestJournal* GetQuestJournalProcess();

CWeaponsRank* GetWeaponsRankProcess();
CTeamDescription* GetTeamDescriptionProcess();
//CWeaponsPointSystem* GetWeaponsPointSystemProcess();


CQuestionProcess* GetQuestionProcess();
CMenuWareHouseProcess* GetWareHouseProcess();
CMenuAutoWalkProcess* GetAutoWalkProcess();
CMenuSellYuanBaoProcess* GetSellYBProcess();
CMenuTargetEquipProcess* GetTargetEquipProcess();
CRelationSystemProcess* GetRelationSystemProcess();
CWantedProcess* GetWantedProcess();
CGenerationProcess* GetGenerationProcess();
CMenuPartnerCultivationUpProcess* GetPartnerCultivationUpProcess();
CAccShopProcess* GetAccShopProcess();
CPointsShopProcess* GetPointsShopProcess();
///////////////////////////////////////////////////


void BattleAction();
void BattleSkill();
void BattleSpecial();
void BattleItem();
void BattleDefence();
void BattleFriend();
void BattleProtect();
void BattleCatch();
void BattleHelp();
void BattleAuto();
void CancelAuto();
void BattleEscape();
void BattleJoin();
void BattleWatchLeave();


void BBAttack();
void BBTrade();
void BBGive();
void BBParty();
void BBChar();
void BBFriend();
void BBItem();
void BBSkill();
void BBHousen();
void BBMission();
void BBFriendShip();
void BBEmotion();
void BBSystem();
void BBGeneration();

void PartyPrivateCall(int memPos);
void PartyKick(int memPos);
void PartyPromote(int memPos);
void PartyQuit();

void PartySelectLineup();
void PartySetKeyPos(int iKeyPos);
void UpdateLineupSelectMenu();
void ForgetLearnedLineup();
void PartySetSelectLineupResualt();
void OpenLineupInfoView();
void LeaveTemporarily();
void ReturnParty();
void UpdateCharMenu();

void UpdateBattleItemMenu(bool wine = false);

void MBChangeNextPage();
void BatDialogNextPage();
void PlayerChooseResultInBat(int opt);

void BattleItemUse(int itemPos);

void SetCharacterIndex(Int32 Index);
void SetLineIndex(Int32 Index);

void CopyToShortCut(int itemPos, int targetPos);
void CopySkillToShortCut(int SkillID, int targetPos);
void  DelShortCutSlot(int Ref, int Type);
Int32 GetShortCutRefType(Int32 Index);
Int32 GetShortCutRefNum(Int32 Index);
void EquipItemUse(int itemPos);
void MoveEquipToBag(int itemPos, int targetPos);
void MoveToBag(int itemPos, int bag, int target = -1);
void DropItem(int itemPos, int dropSize);

bool HasParty();
bool HasTempParty();
void UpdatePartyPanel();
void CopyImageForUIImage(PCSTRINGA src, PCSTRINGA des);
void BattleItemDrag(int itemPos);
bool HasItem(int itemPos);

int GetMaxStack(int itemPos);
void SetDropItemText(int itemPos, PCSTRINGA srcStaticText);
void ShowErrMessage(PCSTRINGA pText, bool center = false);
void BattleItemMove(int SrcBagIndex, int DesBagIndex);
bool HasCharacter(Int32 Index);
bool InviteComing();
UInt32 GetBGMVolume();
void ShowPartyByIndex(int index=-1);
UInt32 GetSFXVolume();
void InviteConfirm(bool confirm);
int GetPartyListSize();

void UpdateApplicationMenu();
void JoinConfirm(int index, bool confirm);
void ClearAllRequest();
void ShowOtherParty(int next);
void ApplicantPanelScroll(int scroll);

void BattleJoinConfirm(int index, bool confirm);
void JoinBattleClearAllRequestUp();
void JoinBattleScroll(int scroll);
void UpdateJoinBattleMenu();

bool IsJoinRequestExist();
void ShowCreateCharacter(int sex = 0, int cclass = 1);

void ChangeCharacterHairCol(bool next);
void ChangeCharacterHead(bool next);
void ChangeCharacterCloth(bool next);
void ChangeCharacterClothCol(bool next);

void CreateCharacter();
PCSTRINGW GetTempMsg();
bool IsFullScreen();
void SetHighLightInLogin(int index, bool highLight);
bool IsBattle();
bool IsPvP();

void LoadLua(PCSTRINGA luafilename);

void CharacterLevelUp();
void CharAttOk();
void CharAttUp(int Index, int Count);
void CharAttDown(int Index, int Count);
void ShowMidMap();
void MidMapMoveCharTo(int index);

void MidMapMoveOut();
void ResetMidMapValue();
void MidMapShowName(int index);

void MidMapClickNPC();
void UpdateSearchList(int index);

void NPCSearchListUpOrDown(int up);

void MidMapShowLocationBox();
void NPCSearchClickList(int index);
void PartySearchClickList(int index);
void CharSearchClickList(int index);
void ShowLeaderInfo();
void ShowLeaderInfoChar();

void SearchNPCList(int lang);

void WeaponsRankSearchList(int lang);

void ShowPartySearchList(int searchBy);
void ShowCharSearchList(int searchBy);

void OnPartySearchPagePrev();
void OnCharSearchPagePrev();

void OnPartySearchPageNext();
void OnCharSearchPageNext();

void OnJoinPartySearch();
void OnJoinCharSearch();

void OnShowPartyInfo();
void OnShowCharInfo();

bool IsIndoor();

void ConfirmPK();
void CancelPK();
void ConfirmMatch();
void CancelMatch();
bool haveParty();

bool IsLeader();
bool GetHelpEnable();
bool GetActionEnable();
bool GetSkillEnable();
bool GetItemEnable();
bool GetDefenceEnable();
bool GetFriendEnable();
bool GetProtectEnable();
bool GetCatchEnable();
bool GetAutoEnable();
bool GetEscapeEnable();

bool GetPartnerSkillEnable();
bool GetPartnerItemEnable();
bool GetPartnerDefenceEnable();
bool GetPartnerProtectEnable();
bool GetPartnerEscapeEnable();


void DeleteCharacter();
void RetrieveCharacter(UInt32 Index);
Boolean CheckCharacterFrozen(UInt32 index);	

void NPCSearchListScroll();

void UpdateShopItemDetail(int pos);
//void UpdateQuestItemDetail(int pos);
void ResetSearchValue();
void InitLoginScreen();
void SaveLoginDefault();
UInt32 GetDefaultCharacter();
void SetDefaultCharacter(int pos);
UInt32 GetDefaultLine();
void SetDefaultLine(int pos);
void ShopOpenConfirmBox(int pos = -1, int nMode = 2);
void ShopEnterItemCount(bool bSell, int nPos);
void ShopSetItemCount(bool bMax);
void ShopAdjustItemCount(bool bInc);
void ShopConfirmAction();
void ShopResetAction();
void ShopDragItemAction(int pos);
void ShopOnClickSellButton();
void ShopCloseMainMenu();
void ShopOnEditBoxKeyUp(int keyCode);
void ShopUpdatePrice();

void SetAniStateInLogin(int index, int state);
void ShowPartyButton();

void BattlePartnerEscape();

void TestingFunction();

void FormatToSkillString(PCSTRINGA Text);
bool IsBattlePartnerTurn();
void CheckCharacterName();
void SkillFail(PCSTRINGA Text);

void SubChatScroll();

bool CanLevelUp();

void SelectItemPartner(UInt8 index);

void UpdateChangeLine();
void ApplicantPrivateCall(int index);

bool IsTalking();
void ChangeMapLine(UInt8 lineIndex);
void ConfirmChangeLineSec();
void TeammatePrivateCall(int index);
void TeammateKick(int index);
void TeammatePromote(int index);
void ItemScroll(int index);
void OnCloseBattleItem();

void QuestCancelCancel();
void QuestCancelConfirm();

bool IsOpenSkillShop();
void ShowRemoveCharacterName();

void SetTempResult(double result);
double GetTempResult();

void ChangeFriendIcon();
void CloseFriendBox();
void CloseQuestionBox();

void LuaSetBPShortCutFlag(bool Flag);
bool LuaGetBPShortCutFlag();

void LockItem(UInt32 index, UInt32 assetType);
void UnlockItem(UInt32 index, UInt32 assetType);
void SetLockCursor(Boolean lock);
void SetUnLockCursor(Boolean unlock);

void OpenFriendChatBox(PSTRINGA msg, PSTRINGA function);
void OpenGuildBox();
void OpenDeleteMemberGuildBox();
void OpenLeaveGuildBox();
void ClsoeGuildBox();
UIPanel *LuaCreateUIPanel();
UIPanelEx *LuaCreateUIPanelEx();
UIImage *LuaCreateUIImage();
UIStaticText *LuaCreateUIStaticText();
UIButton *LuaCreateUIButton();
UICheckBox *LuaCreateUICheckBox();
UIListBox *LuaCreateUIListBox();
UIPopUpMenu *LuaCreateUIPopUpMenu();
UIScrollBarH *LuaCreateUIScrollBarH();
UIScrollBarV *LuaCreateUIScrollBarV();
UIScrollListBox *LuaCreateUIScrollListBox();
UIEditBox *LuaCreateUIEditBox();
UIChatPanel *LuaCreateUIChatPanel();
UIChatString *LuaCreateUIChatString();
UIChatLink *LuaCreateUIChatLink();
UIMiniMap *LuaCreateUIMiniMap();
UILinerImage *LuaCreateUILinerImage();
UILinerImageV *LuaCreateUILinerImageV();
UIAniIcon *LuaCreateUIAniIcon();
UIDynamicText *LuaCreateUIDynamicText();

void RunNewGame();
void OpenHomePage();
void ChangeChatIcon(int index);

UInt32 LuaGetChar_id();
UInt32 LuaGetBag(UInt32 itemPos);
PCSTRING LuaGetItemName(UInt32 itemPos);
UInt32 LuaGetItemColor(UInt32 itemPos);
UInt32 GetFirstNum(PCSTRINGW Str);
UInt32 GetLastNum(PCSTRINGW Str);
void LuaSetShopSkillFlag(bool flag);
bool LuaGetShopSkillFlag();
bool CharSetMapPos(PCSTRINGW pStr);
bool CharOpenQuestMenu(PCSTRINGW pStr);
void OpenFactionDetail();
PCSTRING LuaGetPartnerName(UInt32 Index);
PCSTRING LuaGetPartnerOriginName(UInt32 Index);
UInt32 LuaGetPartnerID(UInt32 Index);
PCSTRING LuaGetPetName(UInt32 Index);
PCSTRING LuaGetPetOriginName(UInt32 Index);
UInt32 LuaGetPetID(UInt32 Index);
void LuaAddAlertBox(PCSTRINGA Msg, bool ButtonFlag);
PCSTRING LuaGetLineTextName();

bool IsHawking();
void ShowPartyPopUp(UInt8 index);
void SelectPartyPopUp(UInt8 popSelect, UInt8 index);
void ShowTeammatePopUp(UInt8 index);
void AutoAcceptCheckBox(bool on);

void SelectTeammatePopUp(UInt8 popSelect, UInt8 index);
void SetChangePos(UInt8 index);
void ChangePartyDesc();
void StopCharacter();
void ShowAlertOK();
void CreateCharRotate(bool left);
void ConfirmReplyString();
void ReplyStringNULL();
bool IsCurrMsgIcon(int index);
void SetHotKeyEnable(bool value);
void OpenWebLink();
void SetWebLink(PCSTRINGA link, int typeno);
void CharEatFastFood(UInt8 foodType);
