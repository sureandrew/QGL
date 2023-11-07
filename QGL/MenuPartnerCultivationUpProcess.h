#pragma once
#ifndef PARTNERCULTIVATIONUP_H
#define PARTNERCULTIVATIONUP_H


class CMenuPartnerCultivationUpProcess : public Reuben::Utility::Singleton<CMenuPartnerCultivationUpProcess>
{
public:
	CMenuPartnerCultivationUpProcess();
	~CMenuPartnerCultivationUpProcess();

public:
	void OpenPartnerCultivationUp(int type);
	void UpdatePartnerCultivationUp();
	void ResetPartnerCultivationUp();
	void ClosePartnerCultivationUp();

	int GetPartnerCultivationUpType() { return m_nType; }
	void SetSrcPartner(UInt32 Id);
	void SetDestPartner(UInt32 Id);
	//void SetDestItem(int itemSlot);

	bool IsSuitableSrcPartner(UInt32 partnerID);
	bool IsSuitableDestPartner(UInt32 partnerID);
	//bool IsSuitableDestItem(int itemSlot);
	UInt32 GetSuitableDestItem();

	void PartnerCultivationUpByPartner();
	void PartnerCultivationUpByItem();

	void DisappearPartnerCultivationUp();

	void PartnerCultivationUpReply(const UInt8& msgIndex, const UInt8& msgValue, const UInt32& src_partnerID);
	void ShowResult(const UInt8& msgIndex, const UInt8& msgValue, const UInt32& src_partnerID);

	inline UInt32 GetSrcPartnerID() { return m_SrcPartnerID; }
private:
	int m_nType;
	UInt32 m_SrcPartnerID;
	UInt32 m_DestPartnerID;
	//UInt32 m_DestItemUid;
	//UInt8 m_DestItemSlot;
	bool m_bCanStart;
	UInt m_Rate;
	UInt32 m_ConsumeMoney;

	UInt m_iStartMixShopX;
	UInt m_iStartMixShopY;
};

#endif