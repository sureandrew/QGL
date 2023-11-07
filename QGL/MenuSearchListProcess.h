#pragma once
#ifndef _MENUSEARCHLISTPROCESS_H_
#define _MENUSEARCHLISTPROCESS_H_

#include "Common/common_binary.h"
#include <Reuben\System\Reuben.h>
#include <vector>

struct NPCInfo
{
	String ChiName;
	String EngName;
	UInt32 NPC_id;
	UInt32 BlockPosX;
	UInt32 BlockPosY;
	UInt8 NPC_type;
	String PrevName;
};

class CMenuSearchListProcess
{
private:

	struct CharInfo
	{
		String CharName;
		UInt32 Char_id;
		UInt Char_ptr;
	};
	//1 desc, 2 faction, 3 cclass, 4 level, 5 count
	enum PARTY_SORT_METHOD
	{
		PARTY_SORT_METHOD_NIL = 0,
		PARTY_SORT_METHOD_DESC,
		PARTY_SORT_METHOD_FACTION,
		PARTY_SORT_METHOD_CCLASS,
		PARTY_SORT_METHOD_LEVEL,
		PARTY_SORT_METHOD_COUNT,
	};

	StlVector<NPCInfo, ManagedAllocator<NPCInfo> >	m_NPCInfoLst;
	StlVector<CharInfo, ManagedAllocator<CharInfo> >	m_CharInfoLst;
	int m_iSelectedNPCListIndex;
	UInt32 m_iStartListIndex;
	static bool Sorting(const NPCInfo &prev, const NPCInfo &next);
	DWORD m_iTimer;
	
	UInt32 m_mapID;
	bool m_bShowNPClist;
	UInt m_sortMethod;
	
	UInt m_sortMethodChar;

	static bool SortPatryByDesc(const PartyInfo &prev, const PartyInfo &next);
	static bool SortPatryByFaction(const PartyInfo &prev, const PartyInfo &next);
	static bool SortPatryByCclass(const PartyInfo &prev, const PartyInfo &next);
	static bool SortPatryByLevel(const PartyInfo &prev, const PartyInfo &next);
	static bool SortPatryByCount(const PartyInfo &prev, const PartyInfo &next);
	PartyInfoVector	m_PartyInfoLst;

	PartyInfoVector	m_SelectPartyInfoLst;
	UInt16 m_iSelectedPartyListIndex;
	UInt16 m_iSelectedPartyPageIndex;


	
	static bool SortCharByFaction(const CharInfo_new &prev, const CharInfo_new &next);
	static bool SortCharByCclass(const CharInfo_new &prev, const CharInfo_new &next);
	static bool SortCharByLevel(const CharInfo_new &prev, const CharInfo_new &next);

	CharInfoVector m_CharInfoLst_new;

	UInt16 m_iSelectedCharListIndex;
	UInt16 m_iSelectedCharPageIndex;

public:

	void UpdateSearchNPC();
	void NPCSearchListClick(int index);
	void MakeNPCSearchList();
	void MoveCharacterToNPC();
	bool FindNPC(UInt16 index, String keyword, int lang); //0: eng, 1: chinese word
	void SearchNPCList(int lang); //0: eng, 1: chinese word
	void ScrollUpOrDown(int up); // 0:up , 1: down
	void Scroll();
	void UpdateSearchTimer();
	void ResetValue();
	NPCInfo GetNPCInfo(int index){ return m_NPCInfoLst[index];}
	UInt16 GetNPCCount(){return (UInt16)m_NPCInfoLst.size();}
	void UpdateSearchCharacter();
	void MakeCharSearchList();
	bool NeedRemakeCharLst();
	bool FindChar(UInt16 index, String keyword); 

	void ShowPartySearchList(const PartyInfoVector &PartyInfoLst);
	void ShowPartySearchList(int searchBy);

	void ShowCharSearchList(const CharInfoVector &CharInfoList);
	void ShowCharSearchList(int searchBy);

	void ShowSelectPartyInfo(const PartyInfoVector &PartyInfoLst);
	void UpdateSearchParty();
	void UpdateSearchChar();
	void UpdateSelectPartyInfo();
	void PartySearchClickList(int index);
	void CharSearchClickList(int index);

	void ShowLeaderInfo();
	void ShowLeaderInfoChar();

	void OnPartyInfoNextPage();
	void OnCharInfoNextPage();

	void OnPartyInfoPrevPage();
	void OnCharInfoPrevPage();

	void OnJoinParty();
	void OnJoinChar();

	void OnShowPartyInfo();
	void OnShowCharInfo();


	CMenuSearchListProcess();
	~CMenuSearchListProcess();


};



#endif 