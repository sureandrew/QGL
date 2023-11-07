#pragma once
#ifndef _MESSAGESERVICE_H_
#define _MESSAGESERVICE_H_
#include "Stub/Service/MessageService_Stub.h"
#include <Reuben/Utility/Singleton.h>

// public functions here
namespace MessageService_Stub
{
};

class GuildGroupCW : public Reuben::Utility::Singleton<GuildGroupCW>
{
public:
	GuildGroupCW(){
		m_bStartTimer = FALSE;
		m_guildWarStatus = 0;
		m_TimeNow = 0;
		m_uiUpdatePeriod = 0;
	}
	~GuildGroupCW(){}
	Reuben::System::UInt8 GetGuildWarStatus() const { return m_guildWarStatus; }
	void SetGuildWarStatus(Reuben::System::UInt8 val) { m_guildWarStatus = val; }
	void OnUpdate()
	{
/*
		if (!StartTimer()) return;

		static int iTime = 0;
		int interval = GetGameTime() - iTime;

		if( interval < 0|| interval >= GetUpdatePeriod() )
		{
			timeNpcFlagUpdateTime = GetGameTime();
			gGlobal.UpdateGlobalNpcFlags();
		}
*/

	}
	Reuben::System::UInt32 GetUpdatePeriod() const { return m_uiUpdatePeriod; }
	void SetUpdatePeriod(Reuben::System::UInt32 val) { m_uiUpdatePeriod = val; }

protected:
	void StartTimer() {}
	void StopTimer() {}
private:
	UInt32		m_uiUpdatePeriod;
	UInt8		m_guildWarStatus;
	BOOL		m_bStartTimer;
	UInt8		m_TimeNow;

	SINGLETON(GuildGroupCW);
};

#endif //_MESSAGESERVICE_H_
