#pragma once
#ifndef AUTOWALK_H
#define AUTOWALK_H


class CMenuAutoWalkProcess : public Reuben::Utility::Singleton<CMenuAutoWalkProcess>
{
public:
	CMenuAutoWalkProcess();
	~CMenuAutoWalkProcess(){};

	INLINE bool IsEnable(){return m_bEnable;}
	INLINE void SetEnable(bool bEnable){m_bEnable = bEnable;}

	void Init();
	void AutoWalk();
	void CancelAutoWalk();
	void SetAutoTime(UInt32 lastTime);
	void OnUpdate(UInt32 uTime);
	void SetHitBoss(Boolean hit);
	void GetOverTime();
	void RunAutoWalk();
	void CloseAutoWalk();
public:
	void StartAutoWalk(UInt32 lastTime);
	void StopAutoWalk();
	void ChangeHitBoss();
	void OnOpenAutoWalk();
	void SetRunning();
	void ShowTime();
private:
	bool		m_bEnable;
	bool 		m_hitboss;
	UInt8		m_overhour;
	UInt8		m_overminute;
	UInt8		m_oversecond;
	UInt16		m_curMapId;
	time_t		m_startTime;
	DOUBLE		m_AutoTime;
	bool        m_showAutoBox;
	bool		m_startAuto;
};

#endif