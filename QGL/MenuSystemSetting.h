#pragma once
#ifndef _MENUSYSTEMSETTING_H_
#define _MENUSYSTEMSETTING_H_

CONST UInt SYSTEM_SETTING_VERSION_NUM = 10003; // add ShowEquip

struct SystemSettingData10002
{
	bool WindowMode;
	bool NightMode;
	bool MapAnimeMode;
	bool WeatherMode;

	bool BlockGivenMode;
	bool BlockPartyMode;
	bool BlockMessageMode;
	bool BlockTradingMode;
	bool BlockPKMode;
	bool BlockMakeFriendMode;
	bool MusicMode;
	UInt8 MusicVolume;
	bool SoundEffectMode;
	UInt8 SoundEffectVolume;
	bool SubChatEnable;
	UInt8 SubChatPosition;
	bool SaveChatHis;
	Char Username[51];

	SystemSettingData10002()
	{
		ZeroMemory(Username, sizeof(Char) * 51);
		SaveChatHis = true;
	}

	~SystemSettingData10002()
	{

	}
};

struct SystemSettingData10001
{
	bool WindowMode;
	bool NightMode;
	bool MapAnimeMode;
	bool WeatherMode;

	bool BlockGivenMode;
	bool BlockPartyMode;
	bool BlockMessageMode;
	bool BlockTradingMode;
	bool BlockPKMode;
	bool BlockMakeFriendMode;
	bool MusicMode;
	UInt8 MusicVolume;
	bool SoundEffectMode;
	UInt8 SoundEffectVolume;
	bool SubChatEnable;
	UInt8 SubChatPosition;
	bool SaveChatHis;
	Char Username[11];

	SystemSettingData10001()
	{
		ZeroMemory(Username, sizeof(Char) * 11);
		SaveChatHis = true;
	}

	~SystemSettingData10001()
	{

	}
};

struct SystemSettingData
{
	bool WindowMode;
	bool NightMode;
	bool MapAnimeMode;
	bool WeatherMode;

	bool BlockGivenMode;
	bool BlockPartyMode;
	bool BlockMessageMode;
	bool BlockTradingMode;
	bool BlockPKMode;
	bool BlockMakeFriendMode;
	bool MusicMode;
	UInt8 MusicVolume;
	bool SoundEffectMode;
	UInt8 SoundEffectVolume;
	bool SubChatEnable;
	UInt8 SubChatPosition;
	bool SaveChatHis;
	Char Username[51];
	bool ShowEquipMode; // 10003

	SystemSettingData()
	{
		ZeroMemory(Username, sizeof(Char) * 51);
		SaveChatHis = true;
		ShowEquipMode = true;
	}

	~SystemSettingData()
	{}

	void ConvertFrom(SystemSettingData10001* oldData)
	{
		WindowMode = oldData->WindowMode;
		NightMode = oldData->NightMode;
		MapAnimeMode = oldData->MapAnimeMode;
		WeatherMode = oldData->WeatherMode;

		BlockGivenMode = oldData->BlockGivenMode;
		BlockPartyMode = oldData->BlockPartyMode;
		BlockMessageMode = oldData->BlockMessageMode;
		BlockTradingMode = oldData->BlockTradingMode;
		BlockPKMode = oldData->BlockPKMode;
		BlockMakeFriendMode = oldData->BlockMakeFriendMode;
		MusicMode = oldData->MusicMode;
		MusicVolume = oldData->MusicVolume;
		SoundEffectMode = oldData->SoundEffectMode;
		SoundEffectVolume = oldData->SoundEffectVolume;
		SubChatEnable = oldData->SubChatEnable;
		SubChatPosition = oldData->SubChatPosition;

		for( Index i = C_INDEX(0); i < 11; ++i )
		{
			if( oldData->Username[i]=='\0' )
			{
				_tcsncpy(Username, oldData->Username, 11);
				break;
			}
		}
	}

	void ConvertFrom(SystemSettingData10002* oldData)
	{
		WindowMode = oldData->WindowMode;
		NightMode = oldData->NightMode;
		MapAnimeMode = oldData->MapAnimeMode;
		WeatherMode = oldData->WeatherMode;

		BlockGivenMode = oldData->BlockGivenMode;
		BlockPartyMode = oldData->BlockPartyMode;
		BlockMessageMode = oldData->BlockMessageMode;
		BlockTradingMode = oldData->BlockTradingMode;
		BlockPKMode = oldData->BlockPKMode;
		BlockMakeFriendMode = oldData->BlockMakeFriendMode;
		MusicMode = oldData->MusicMode;
		MusicVolume = oldData->MusicVolume;
		SoundEffectMode = oldData->SoundEffectMode;
		SoundEffectVolume = oldData->SoundEffectVolume;
		SubChatEnable = oldData->SubChatEnable;
		SubChatPosition = oldData->SubChatPosition;

		for( Index i = C_INDEX(0); i < 51; ++i )
		{
			if( oldData->Username[i]=='\0' )
			{
				_tcsncpy(Username, oldData->Username, 51);
				break;
			}
		}
	}
};
/*
enum SubChatPositionEnum
{
	SUBCHAT_NORMAL = 0,
	SUBCHAT_LEFT,
	SUBCHAT_RIGHT,
	SUBCHAT_MAX_POSITION,
};*/

class CMenuSystemSetting
{
private:
	SystemSettingData* m_pSystemSetting;
	bool m_bWriteFlag;
	UInt8 m_iTempMusicVolume;
	UInt8 m_iTempSoundEffectVolume;
	UInt16Vector	m_finishedHelpEvent;
public:
	CMenuSystemSetting();
	~CMenuSystemSetting();
	void ReadSystemSetting();
	void WriteSystemSetting();
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
	void SetUserName(String userName);
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
	UInt8 GetMusicVolume();
	UInt8 GetTempMusicVolume();
	bool IsSoundEffectModeEnable();
	UInt8 GetSoundEffectVolume();
	UInt8 GetTempSoundEffectVolume();
	String GetUserName();
	bool IsShowEquipModeEnable();

	void InitSystemSetting();
	void AddMusicVolume();
	void SubMusicVolume();

	void AddSoundEffectVolume();
	void SubSoundEffectVolume();

	void ResetTempVolume();
	void RestoreTempVolumeToDefault();
	

	void SetSubChatEnable(bool enable);
	bool IsSubChatEnable();
	void SetSubChatPosition(UInt8 position);
	UInt8 GetSubChatPosition();

	void SaveBlockSetting();
	Int GetSoundEffectVolForFmod();
	Int GetMusicVolForFmod();
};


#endif 