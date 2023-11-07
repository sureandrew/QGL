//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuSystemSetting.h"
//-- Library
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "QGLApplication.h"
#include "Resource/ClientResManager.h"
#include "QGL/CharacterControl.h"
//#include "QGL/MapControl.h"

#define DEFAULT_MUSIC_EFFECT_VOLUME 8

CMenuSystemSetting::CMenuSystemSetting()
{
	m_pSystemSetting = NULL;
	m_bWriteFlag = false;
	m_iTempMusicVolume = 0;
	m_iTempSoundEffectVolume = 0;
}

CMenuSystemSetting::~CMenuSystemSetting()
{
	SafeDeleteObject(m_pSystemSetting);
}

void CMenuSystemSetting::ReadSystemSetting()
{
	if ( !m_bWriteFlag)
	{
		if ( m_pSystemSetting == NULL )
		{
			m_pSystemSetting = SafeCreateObject(SystemSettingData);	
			InitSystemSetting();
		}

		if ( m_pSystemSetting )
		{
			FILE* pFile = NULL;
			String temp = _T(".\\Config\\SystemSetting.ssb");
			pFile = _tfopen(temp.c_str(), _T("rb"));
			if ( pFile )
			{
				UInt versionNo = 0;
				fread(&versionNo, sizeof(UInt), 1, pFile);
				switch( versionNo )
				{
				case 10001:
					{
						SystemSettingData10001 tmpSetting;
						int rl = (int)fread(&tmpSetting, sizeof(SystemSettingData10001), 1, pFile);
						m_pSystemSetting->ConvertFrom(&tmpSetting);
					}
					break;
				case 10002:
					{
						SystemSettingData10002 tmpSetting;
						int rl = (int)fread(&tmpSetting, sizeof(SystemSettingData10002), 1, pFile);
						m_pSystemSetting->ConvertFrom(&tmpSetting);
					}
					break;
				case SYSTEM_SETTING_VERSION_NUM:
					{
						int rl = (int)fread(m_pSystemSetting, sizeof(SystemSettingData), 1, pFile);
					}
					break;
				}
				fclose(pFile);
			}
			m_iTempMusicVolume = m_pSystemSetting->MusicVolume;
			m_iTempSoundEffectVolume = m_pSystemSetting->SoundEffectVolume;
		}
	}
}

void CMenuSystemSetting::WriteSystemSetting()
{
	if ( !m_bWriteFlag )
	{
		m_bWriteFlag = BOOLEAN_TRUE;

		if ( m_pSystemSetting == NULL )
		{
			m_pSystemSetting = SafeCreateObject(SystemSettingData);	
			InitSystemSetting();
		}

		if ( m_pSystemSetting )
		{
			FILE* pFile = NULL;
			String temp = _T(".\\Config\\SystemSetting.ssb");
			pFile = _tfopen(temp.c_str(), _T("wb"));
			if ( pFile )
			{
				fwrite(&SYSTEM_SETTING_VERSION_NUM, sizeof(UInt), 1, pFile);
				fwrite(m_pSystemSetting, sizeof(SystemSettingData), 1, pFile);
				fclose(pFile);
			}
		}
		m_bWriteFlag = BOOLEAN_FALSE;
	}
}

void CMenuSystemSetting::SetWindowMode(bool on)
{
	if (m_pSystemSetting)
	{
		if (m_pSystemSetting->WindowMode != on)
		{
			m_pSystemSetting->WindowMode = on;

			if(CheckMyProcess(QGLApplication::GetInstance().GetWindow()) > 1)
				return;
		
			if (QGLApplication::GetInstance().GetRenderClass()->IsFullScreen() == on)
			{
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseSubWindowNow", String());  
				QGLApplication::GetInstance().GetRenderClass()->ToggleFullscreen();
			}
		}
	}
}

void CMenuSystemSetting::SetNightMode(bool on)
{
	if (m_pSystemSetting)
	{
		if (m_pSystemSetting->NightMode != on)
		{
			m_pSystemSetting->NightMode = on;

			if (on)
			{
				if (gGlobal.m_mapdb->m_basicinfo->m_mapdaynight == 2 && gGlobal.NightMask)
				{
					gGlobal.Night = true;
					gGlobal.NightMask->m_Updated = TRUE;
					gGlobal.g_DrawAll = TRUE;
					QGLApplication::GetInstance().CreateNightData();
					gGlobal.UpdateNightAlpha();
				}
				else if (gGlobal.m_mapdb->m_basicinfo->m_mapdaynight == 1)
				{
					gGlobal.Night = false;
					gGlobal.g_DrawAll = TRUE;
					gGlobal.UpdateNightAlpha();
				}
				else
					gGlobal.UpdateNightAlpha();
			}
			else
			{
				gGlobal.Night = false;
				gGlobal.g_DrawAll = TRUE;
				QGLApplication::GetInstance().ReleaseNightData();
			}
		}
	}
}

void CMenuSystemSetting::SetMapAnimeMode(bool on)
{
	if (m_pSystemSetting)
	{
		m_pSystemSetting->MapAnimeMode = on;
	}
}

void CMenuSystemSetting::SetWeatherMode(bool on)
{
	if (m_pSystemSetting)
	{
		if (m_pSystemSetting->WeatherMode != on)
		{
			m_pSystemSetting->WeatherMode = on;
			if (on)
			{
				if (gGlobal.m_mapdb->m_basicinfo->m_mapweathermode != 0 && gGlobal.g_pSystemSetting->IsWeatherModeEnable() )
				{
					if (gGlobal.m_mapdb->m_basicinfo->m_mapweathertype == 0)
						QGLApplication::GetInstance().AddParticleSystem(gGlobal.g_ParticleEffectRain);
					else if (gGlobal.m_mapdb->m_basicinfo->m_mapweathertype == 1)
						QGLApplication::GetInstance().AddParticleSystem(gGlobal.g_ParticleEffectFog);
					else if (gGlobal.m_mapdb->m_basicinfo->m_mapweathertype == 2)
						QGLApplication::GetInstance().AddParticleSystem(gGlobal.g_ParticleEffectSnow);
					else if (gGlobal.m_mapdb->m_basicinfo->m_mapweathertype == 3)
						QGLApplication::GetInstance().AddParticleSystem(gGlobal.g_ParticleEffectFog);
				}
			}
			else
			{
				QGLApplication::GetInstance().RemoveParticleSystem(gGlobal.g_ParticleEffectFog);
				QGLApplication::GetInstance().RemoveParticleSystem(gGlobal.g_ParticleEffectRain);
				QGLApplication::GetInstance().RemoveParticleSystem(gGlobal.g_ParticleEffectSnow);
			}
		}
	}
}

void CMenuSystemSetting::SetBlockGivenMode(bool on)
{
	if (m_pSystemSetting)
		m_pSystemSetting->BlockGivenMode = on;
}

void CMenuSystemSetting::SetBlockPartyMode(bool on)
{
	if (m_pSystemSetting)
		m_pSystemSetting->BlockPartyMode = on;
}

void CMenuSystemSetting::SetBlockMessageMode(bool on)
{
	if (m_pSystemSetting)
		m_pSystemSetting->BlockMessageMode = on;
}

void CMenuSystemSetting::SetBlockPKMode(bool on)
{
	if (m_pSystemSetting)
		m_pSystemSetting->BlockPKMode = on;
}

void CMenuSystemSetting::SetBlockMakeFriendMode(bool on)
{
	if (m_pSystemSetting)
		m_pSystemSetting->BlockMakeFriendMode = on;
}

void CMenuSystemSetting::SetMusicMode(bool on)
{
	if (m_pSystemSetting)
	{
		if (m_pSystemSetting->MusicMode != on || m_iTempMusicVolume != m_pSystemSetting->MusicVolume)
		{
			m_pSystemSetting->MusicMode = on;
			m_pSystemSetting->MusicVolume = m_iTempMusicVolume;
			if (on)
			{
				m_pSystemSetting->MusicVolume = m_iTempMusicVolume;
				Int temp = 255 * m_pSystemSetting->MusicVolume / 10;
				::GetFSoundManager()->MusicSetVolume(temp);
			}
			else
			{
				::GetFSoundManager()->MusicSetVolume(0);
			}
		}
	}
}

void CMenuSystemSetting::SetMusicVolume(UInt8 volume)
{
	if (m_pSystemSetting)
		m_pSystemSetting->MusicVolume = volume;
}

void CMenuSystemSetting::SetSoundEffectMode(bool on)
{
	if (m_pSystemSetting)
	{
		if (m_pSystemSetting->SoundEffectMode != on || m_pSystemSetting->SoundEffectVolume != m_iTempSoundEffectVolume)
		{
			m_pSystemSetting->SoundEffectMode = on;
			m_pSystemSetting->SoundEffectVolume = m_iTempSoundEffectVolume;
			if (on)
				m_pSystemSetting->SoundEffectVolume = m_iTempSoundEffectVolume;	
		}
	}
}

void CMenuSystemSetting::SetSoundEffectVolume(UInt8 volume)
{
	if (m_pSystemSetting)
		m_pSystemSetting->SoundEffectVolume = volume;
}

void CMenuSystemSetting::SetBlockTradingMode(bool on)
{
	if (m_pSystemSetting)
		m_pSystemSetting->BlockTradingMode = on;
}

void CMenuSystemSetting::SetShowEquipMode(bool on)
{
	if (m_pSystemSetting)
		m_pSystemSetting->ShowEquipMode = on;
}

void CMenuSystemSetting::SetSaveChatHistory(bool on)
{
	if (m_pSystemSetting)
		m_pSystemSetting->SaveChatHis = on;
}

void CMenuSystemSetting::SetUserName(String userName)
{
	if (m_pSystemSetting)
	{
		::_tcsncpy(m_pSystemSetting->Username, userName.c_str(), 50);
		m_pSystemSetting->Username[50] = '\0';
	}
}

bool CMenuSystemSetting::IsWindowModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->WindowMode;
	return false;
}

bool CMenuSystemSetting::IsNightModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->NightMode;
	return false;
}

bool CMenuSystemSetting::IsMapAnimeModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->MapAnimeMode;
	return false;
}

bool CMenuSystemSetting::IsWeatherModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->WeatherMode;
	return false;
}

bool CMenuSystemSetting::IsBlockGivenModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->BlockGivenMode;
	return false;
}

bool CMenuSystemSetting::IsBlockPartyModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->BlockPartyMode;
	return false;
}
	
bool CMenuSystemSetting::IsBlockMessageModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->BlockMessageMode;
	return false;
}
	
bool CMenuSystemSetting::IsBlockPKModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->BlockPKMode;
	return false;
}
	
bool CMenuSystemSetting::IsBlockMakeFriendModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->BlockMakeFriendMode;
	return false;
}
	
bool CMenuSystemSetting::IsMusicModeEnable()
{	
	if (m_pSystemSetting)
		return m_pSystemSetting->MusicMode;
	return false;
}

UInt8 CMenuSystemSetting::GetMusicVolume()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->MusicVolume;	
	return 0;
}

UInt8 CMenuSystemSetting::GetTempMusicVolume()
{
	if (m_pSystemSetting)
	{
		if (m_pSystemSetting->MusicVolume != m_iTempMusicVolume)
			return m_iTempMusicVolume;
		else
			return m_pSystemSetting->MusicVolume;
	}
	return 0;
}

bool CMenuSystemSetting::IsSoundEffectModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->SoundEffectMode;
	return false;
}

bool CMenuSystemSetting::IsSaveChatHistory()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->SaveChatHis;
	return false;
}

UInt8 CMenuSystemSetting::GetSoundEffectVolume()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->SoundEffectVolume;
	return 0;
}

Int CMenuSystemSetting::GetSoundEffectVolForFmod()
{
	if (m_pSystemSetting)
		return 255 * m_pSystemSetting->SoundEffectVolume / 10;
	return 0;
}

Int CMenuSystemSetting::GetMusicVolForFmod()
{
	if (m_pSystemSetting)
		return 255 * m_pSystemSetting->MusicVolume / 10;
	return 0;
}

UInt8 CMenuSystemSetting::GetTempSoundEffectVolume()
{
	if (m_pSystemSetting)
	{
		if (m_pSystemSetting->SoundEffectVolume != m_iTempSoundEffectVolume)
			return m_iTempSoundEffectVolume;
		else
			return m_pSystemSetting->SoundEffectVolume;
	}
	return 0;
}

bool CMenuSystemSetting::IsShowEquipModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->ShowEquipMode;
	return false;
}

String CMenuSystemSetting::GetUserName()
{
	if (m_pSystemSetting)
		return String(m_pSystemSetting->Username);

	String empty;
	return empty;
}

bool CMenuSystemSetting::IsBlockTradingModeEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->BlockTradingMode;
	return false;
}

void CMenuSystemSetting::InitSystemSetting()
{
	if (m_pSystemSetting)
	{
		m_pSystemSetting->WindowMode = true;
		m_pSystemSetting->MapAnimeMode = true;
		m_pSystemSetting->WeatherMode = true;
		m_pSystemSetting->NightMode = true;
		m_pSystemSetting->ShowEquipMode = false;

		m_pSystemSetting->BlockTradingMode = false;
		m_pSystemSetting->BlockGivenMode = false;
		m_pSystemSetting->BlockPartyMode = false;
		m_pSystemSetting->BlockMessageMode = false;
		m_pSystemSetting->BlockPKMode = false;
		m_pSystemSetting->BlockMakeFriendMode = false;

		m_pSystemSetting->MusicMode = true;
		m_pSystemSetting->MusicVolume = DEFAULT_MUSIC_EFFECT_VOLUME;
		m_pSystemSetting->SoundEffectMode = true;
		m_pSystemSetting->SoundEffectVolume = DEFAULT_MUSIC_EFFECT_VOLUME;
		m_iTempMusicVolume = DEFAULT_MUSIC_EFFECT_VOLUME;
		m_iTempSoundEffectVolume = DEFAULT_MUSIC_EFFECT_VOLUME;
		m_pSystemSetting->SubChatEnable = false;
		m_pSystemSetting->SubChatPosition = WINDOWALIGN_RIGHT;
	}
}

void CMenuSystemSetting::AddMusicVolume()
{
	if (m_iTempMusicVolume < 10)
		++m_iTempMusicVolume;
}

void CMenuSystemSetting::SubMusicVolume()
{
	if (m_iTempMusicVolume > 0)
		--m_iTempMusicVolume;
}

void CMenuSystemSetting::AddSoundEffectVolume()
{
	if (m_iTempSoundEffectVolume < 10)
		++m_iTempSoundEffectVolume;
}

void CMenuSystemSetting::SubSoundEffectVolume()
{
	if (m_iTempSoundEffectVolume > 0)
		--m_iTempSoundEffectVolume;
}

void CMenuSystemSetting::ResetTempVolume()
{
	if (m_pSystemSetting)
	{
		m_iTempMusicVolume = m_pSystemSetting->MusicVolume;
		m_iTempSoundEffectVolume = m_pSystemSetting->SoundEffectVolume;
	}
}

void CMenuSystemSetting::RestoreTempVolumeToDefault()
{
	if (m_pSystemSetting)
	{
		m_iTempMusicVolume = DEFAULT_MUSIC_EFFECT_VOLUME;
		m_iTempSoundEffectVolume = DEFAULT_MUSIC_EFFECT_VOLUME;
	}
}

//use enum SubWindowAlignType which in QGLApplication
void CMenuSystemSetting::SetSubChatPosition(UInt8 position)
{
	if (m_pSystemSetting)
		m_pSystemSetting->SubChatPosition = position;
}

void CMenuSystemSetting::SetSubChatEnable(bool enable)
{
	if (m_pSystemSetting)
	{
		if (m_pSystemSetting->SubChatEnable != enable)
			m_pSystemSetting->SubChatEnable = enable;
	}
}

UInt8 CMenuSystemSetting::GetSubChatPosition()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->SubChatPosition;	
	return 0;
}

bool CMenuSystemSetting::IsSubChatEnable()
{
	if (m_pSystemSetting)
		return m_pSystemSetting->SubChatEnable;	
	return false;
}

void CMenuSystemSetting::SaveBlockSetting()
{
	if (m_pSystemSetting && gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject() )
	{
		gGlobal.g_pCharacter->GetCharNetObject()->SaveSystemSetting(gGlobal.pPeerProxy, gGlobal.g_context, m_pSystemSetting->BlockGivenMode, m_pSystemSetting->BlockPartyMode,
			m_pSystemSetting->BlockMessageMode, m_pSystemSetting->BlockTradingMode, m_pSystemSetting->BlockMakeFriendMode, m_pSystemSetting->ShowEquipMode);
		//gGlobal.g_pCharacter->GetCharNetObject()->SetEnablePK(gGlobal.pPeerProxy, gGlobal.g_context, !m_pSystemSetting->BlockPKMode);
	}
}