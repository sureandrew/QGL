#pragma once
#ifndef _CHARACTERCONTROL_H_
#define _CHARACTERCONTROL_H_

#include "QGL.h"
#include "Reuben/Simulation/Object.h"
#include "Proxy/Logic/Character.h"
#include "CharacterView.h"
#include "MapActorControl.h"
#include "FontEntity.h"
#include "UIEntity.h"
#include "common_binary.h"
#include "Common/BattleCode.h"
#include "QGL.h"

class CCharacter;
class CPartner;
class CAnimatedEntity;
class CFontEntity;
class CBatActor;
class CSkill;
class CSkillGroup;
struct SkillData;
class UIPanel;

struct EntityView
{
	CAnimatedEntity	main;
	CAnimatedEntity	shadow;
	CFontEntity		name;
};

typedef StlVector<const BuffData*, ManagedAllocator<const BuffData* > > BuffDataList;

class CCharacterControl : public  CMapActorControl/*
,
						  public Reuben::Simulation::Object*/

{
/*public:


	void OpenPointsShop(UInt32Vector& Lst, UInt32 &uiTypeID);

	RPCResult OpenPointsShop(RPCContext &context);


	DEFINE_PARENT_CLASS(Reuben::Simulation::Object);
	DEFINE_CLASS(0);			// don't care class id
*/

protected:

	CCharacter* m_pCharacter;
	CBatActor* m_pBatActor;
	CCharacterControl* m_pTarget;

	EntityView	m_PartnerView;
	EntityView	m_PetView;
	EntityView  m_PetTwoView;

	typedef StlVector<EntityView*, ManagedAllocator<EntityView*> > EntityViewList;
	EntityViewList m_FollowerViews;

	UIPanel* m_pHawkBubble;

	bool m_bHasPartner;
	bool m_bIsSentCommand;
	bool m_bHasFollowPet;
	bool m_bHasFollowTwoPet;

	UInt32 m_AnimIdlePartnerChangeCounter;
	UInt32 m_AnimIdlePartnerChangeTime;

	UInt32 m_AnimIdlePetChangeCounter;
	UInt32 m_AnimIdlePetChangeTime;

	UInt32 m_FollowerIdleCounter;
	UInt32 m_FollowerIdleTime;

	typedef StlMap<Equip_Type, String, std::less<Equip_Type>,ManagedAllocator<std::pair<Equip_Type, String> > > CharModelFile;
	typedef StlMap<Equip_Type, ColorPaletteStruct*, std::less<Equip_Type>,ManagedAllocator<std::pair<Equip_Type, ColorPaletteStruct*> > > CharModelFileClour;

	CharModelFile m_charModelLst;
	CharModelFileClour m_charModelColor;

	StlMap<UInt16, const SkillData *, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, const SkillData *> > > m_BlockSkills;

public:

	const BuffData *	m_tempBuffData;
	BuffMap	m_BuffMap;

	//buff that only active during online
	BuffMap		m_BuffCacheMap;
	BuffDataList addBuffList;
	BuffDataList removeBuffList;

	INLINE void ClearBuffChange(){addBuffList.clear(); removeBuffList.clear();}

	Boolean		m_ActionCmdAva[MAX_ACTION_CMD];

	CCharacterControl();
	virtual ~CCharacterControl();

	
	State* m_mapState;			// [leave, enter, inside]
	BOOL m_IsListen;

	INLINE CCharacter* GetCharNetObject(){ return m_pCharacter;}
	INLINE CBatActor* GetBatActorObject(){ return m_pBatActor;}
	VOID SetTarget(CCharacterControl* pTarget);
	INLINE CCharacterControl* GetTarget(VOID) { return (m_pTarget); }
	VOID SetBatActor(CBatActor * pBatActor);
	
	VOID SetEntityDesPos(UInt16 x, UInt16 y, BOOL SendToServer = TRUE);
	VOID Stop();			// stop moving character
	VOID SendBattleCommand(UInt8 cmd, UInt8 target, UInt32 sub_type, UInt32 additional);
	VOID CheatCode(const String &code);
	//get battle hp and max hp
	UInt16 GetBattleHp();
	UInt16 GetBattleMaxHp();
	UInt16 GetBattleMHp();

	String GetCharName();
	UInt8 GetJobType();
	virtual UInt32 GetMapActorID();

	virtual VOID SetMapActorObject(CMapActor* pMapActor);

	virtual VOID OnUpdatePartyInfo(const Boolean &hasParty, const Boolean &isLeader);
	virtual VOID OnUpdateAction(const UInt16 &action, const UInt32 &actionTime);
	VOID OnUpdateTitle(String title, UInt16 title_id);

	VOID ChangeWeapon(String Name);
	VOID ChangeBody(UInt8 SexClass, UInt16 cloth, UInt16 clothColor);
	VOID ChangeHeadAccessory(String Name, String colorFile);

	VOID CheckTalking(UInt16 CurrX, UInt16 CurrY);

	VOID RemoveHair();
	VOID AddHair(UInt ModelID);

	VOID OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor, UInt8 sexClass);

	VOID OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor, UInt8 sexClass);

	VOID OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor, UInt8 sexClass);

	VOID OnUpdateBattleInfo(const Boolean &EnablePK);

	VOID OnUpdatePKInfo(const UInt16 &PK_Value);

	VOID OnUpdateHead(const UInt32 &head, const UInt16 &headColor, UInt8 sexClass);

	VOID OnUpdatePet(const UInt32 &pet, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex);
	VOID OnUpdateFollowerList(const UInt32Vector &followerList);
	
	VOID OnUpdatePettwo(const UInt32 &pet_uid,const UInt32 &pet_id,const String &petName,const UInt16 &petColorIndex);

	VOID OnUpdateMark(const UInt32  &mark);
	
	VOID OnUpdateTP(const UInt16 &TP,const UInt32 &Model_mobid,const Boolean &isBattle);


	UInt32 GetCharMark();

	virtual VOID SetFontColor(Color32 Color);

	inline CAnimatedEntity* GetPartnerMainView(){ return &m_PartnerView.main;};
	inline CAnimatedEntity* GetPartnerShadowView(){ return &m_PartnerView.shadow;};
	inline CFontEntity* GetPartnerNameView(){ return &m_PartnerView.name;};

	inline CAnimatedEntity* GetPetMainView(){ return &m_PetView.main;};
	inline CAnimatedEntity* GetPetShadowView(){ return &m_PetView.shadow;};
	inline CFontEntity* GetPetNameView(){ return &m_PetView.name;};

	inline CAnimatedEntity* GetTwoPetMainView(){return &m_PetTwoView.main;};
	inline CAnimatedEntity* GetTwoPetShadowView(){return &m_PetTwoView.shadow;};
	inline CFontEntity* GetTwoPetNameView(){ return &m_PetTwoView.name;};

	CAnimatedEntity* GetFollowerMainView(int i);
	CAnimatedEntity* GetFollowerShadowView(int i);
	CFontEntity* GetFollowerNameView(int i);

	void SetEntityView(EntityView* pView, const String& image, const String& name, 
						Byte eType, UInt16 colorIndex=0);
	bool HasPartner(){return m_bHasPartner;}
	void SetHasPartner(bool hasPartner){ m_bHasPartner = hasPartner;}
	void SetHasFollowPet(bool hasPet){m_bHasFollowPet = hasPet;}
	bool HasFollowPet(){return m_bHasFollowPet;}
	void SetHasFollowPetTwo(bool hasTwoPet){m_bHasFollowTwoPet = hasTwoPet;}
	bool HasFollowTwoPet(){return m_bHasFollowTwoPet;}

	void SetPartner(UInt32 mob_id, String partnerName, UInt16 colorIndex);

	bool HasFollower();
	int GetFollowerCount();
	void SetFollower(UInt32Vector followerList);
	void ClearFollower();
	virtual void RefreshFollower();
	
	INLINE void ResetHawkBubble(){ m_pHawkBubble = NULL; }

	void UpdatePartnerView(UInt32 CurrTime);
	void UpdatePetView(UInt32 CurrTime);
	void UpdatePetTwoView(UInt32 CurrTime);
	void UpdateHawkBubbleView(UInt32 CurrTime);
	void UpdateFollowerView(UInt32 CurrTime);

	bool IsSentCommand(){ return m_bIsSentCommand;}
	void SetIsSentCommand(bool sent){ m_bIsSentCommand = sent;}

	void OnUpdateBuff(const String & buff);
	void OnUpdateBuffCache(const String & buffCache);

	void UpdateBuffTime(UInt32 curTime);
	VOID AddBuff(const BuffData * pBuff);
	VOID RemoveBuff(const BuffData * pBuff);
	void UpdateBlockCmd();
	void UpdateBlockSkill();
	void BlockAllCmd();

	bool IsPartner();
	CPartner* GetPartner();
	CSkillGroup* GetSkillGroup();
	UInt16 GetSkillCount();
	CSkill* GetSkill(UInt16 index);
	CSkill* GetSkillById(UInt16 skill_id);
	int GetSkillLv(int index);
	bool IsBuffExist(int Buff_id);

	Int GetBatPosition();
	UInt8 GetBatTeam();

	UInt32 GetChar_id() { return GetMapActorID(); }
	UInt16 GetLevel();
	UInt32 GetNextLvExp();
	UInt32 GetExp();
	UInt16 GetSTA();
	UInt16 GetSPR();
	UInt16 GetSTR();
	UInt16 GetCON();
	UInt16 GetAGI();
	UInt16 GetATK();
	UInt16 GetDEF();
	UInt16 GetSPD();
	UInt16 GetPOW();
	UInt16 GetHIT();
	UInt16 GetEVA();
	UInt16 GetHP();
	UInt16 GetMHP();
	UInt16 GetMaxHP();
	UInt16 GetSP();
	UInt16 GetMSP();
	UInt16 GetMaxSP();
	UInt16 GetDP();
	UInt16 GetMDP();
	UInt16 GetMaxDP();
	UInt16 GetWC();
	UInt16 GetWcExp();
	UInt32 GetNextWcExp();
	UInt16 GetMaxWC();
	UInt16 GetVP();
	UInt16 GetMaxVP();
	UInt16 GetAttrPoint();

	UInt32 GetYuanBao();
	UInt32 GetMoney();
	UInt32 GetBankMoney();

	Int32 GetGoodness();
	UInt32 GetHonor();
	UInt32 GetManor();
	UInt8 GetMaxPartner();
	UInt8 GetMaxPet();
	UInt16 GetPK_Value();
	UInt16 GetPKValue();
	Boolean GetEnablePK();

	bool IsVisibleToOthers();

	BOOL DetectBuff(UInt16 Buff_ID);

	UInt8 GetIndex();
	UInt16 GetTeam();

	bool HasParty();
	virtual bool IsLeader();
	bool IsMember();

	bool CheckProtectedPK();
	bool CheckBlockPK();
	bool CheckBlockItem(UInt8 itemType);
	bool CheckBlockChat(UInt8 ChannelType);

	bool AllCmdBlock();

	UInt32 GetWeaponBuffModValue(UInt8 type);
	VOID RemoveBuffAfterBattle();
	VOID RemoveHideEffectBuff();

	VOID ChangeModel(UInt32 mob_id,Boolean isBattle);

	bool HasEqModel(Equip_Type EQUIPTYPE);
	bool HasEqModelClour(Equip_Type EQUIPTYPE);
	void RemoveEqModel(Equip_Type EQUIPTYPE);
	void RemoveEqModelClour(Equip_Type EQUIPTYPE);
	String GetEqModelFileName(Equip_Type EQUIPTYPE);
};

#endif //_CHARACTER_H_
