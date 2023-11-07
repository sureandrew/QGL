#include "Common.h"
#include "QGL.h"
#include "PlayOggVorbis.h"
#include <windows.h>
#include <process.h>
#include "Global.h"
#include "MenuSystemSetting.h"


GDFManager* g_GdfResource;
GDFManager* g_GdfResourceEx;


FSound::FSound()
{
	m_Sample = NULL;
	m_Name = "";
	m_Channel = -1;
	m_Reserved = false;
	m_Vol = 255;
}

FSound::~FSound()
{
	Close();
}

int FSound::Load(PCSTRINGA Filename, int Channel)
{
	if(m_Channel != -1)
	{
		Close();
	}

	m_Channel = Channel;
	m_Sample = FSOUND_Sample_Load(m_Channel, Filename, 0, 0, 0); 
	FSOUND_Sample_SetMode(m_Sample, FSOUND_LOOP_OFF);
	m_Name = Filename;
	m_Name.ToUpper();

	return 0;
}

void FSound::Close()
{
	if(m_Channel != -1)
	{
		FSOUND_StopSound (m_Channel);
		FSOUND_Sample_Free(m_Sample);

		m_Sample = NULL;
		m_Name = "";
		m_Channel = -1;
	}
}

void FSound::Play()
{
	if(m_Channel != -1)
	{
		
		FSOUND_PlaySound(m_Channel, m_Sample);
		FSOUND_SetVolume(m_Channel, m_Vol);
	}

}

void FSound::Stop()
{
	if(m_Channel != -1)
	{
		FSOUND_StopSound (m_Channel);
	}

}

void FSound::Pause()
{
	if(m_Channel != -1)
	{
		FSOUND_SetPaused (m_Channel, TRUE);
	}
}

void FSound::Resume()
{
	if(m_Channel != -1)
	{
		FSOUND_SetPaused (m_Channel, FALSE);
	}
}

void FSound::SetVolume(int Vol)
{
	if(m_Channel != -1)
	{
		if(Vol > 255)
			m_Vol = 1;
		else
		if(Vol < 0)
			m_Vol = 0;
		else
			m_Vol = Vol; 
	}
}


bool FSound::IsPlaying()
{
	if(m_Channel != -1)
	{
		return (FSOUND_IsPlaying(m_Channel) ? true : false);
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////


FSOUND_Manager::FSOUND_Manager()
{
	m_OKFlag = FALSE;
	m_MusicHandle = NULL;

	memset(m_ChannelSocket, 0, sizeof(m_ChannelSocket));
	m_SocketIndex = 1;
}


FSOUND_Manager::~FSOUND_Manager()
{
	Close();
}


void FSOUND_Manager::Init()
{
	m_OKFlag = FSOUND_Init (44100, 32, 0);
}


void FSOUND_Manager::Close()
{
	for(int i = 1; i < 32; ++i)
	{
		if(m_ChannelSocket[i])
		{
			SafeDeleteObject(m_ChannelSocket[i]);
		}
	}

	if(m_MusicHandle)
	{
		FSOUND_Stream_Close(m_MusicHandle);
		m_MusicHandle = NULL;
	}
	if(m_OKFlag)
	{
		FSOUND_Close();
		m_OKFlag = FALSE;
	}
}


void FSOUND_Manager::SetMusic(PCSTRINGA pFileName)
{
	if(m_OKFlag)
	{
		StringA	TempStr = pFileName;

		TempStr.ToUpper();
		if(TempStr == m_MusicName)
			return;

		int Vol = 255;

		if(m_MusicHandle)
		{
			Vol = FSOUND_GetVolume(0);
			FSOUND_StopSound(0);
			FSOUND_Stream_Close(m_MusicHandle);
		}

		m_MusicName = pFileName;
		m_MusicName.ToUpper();
		m_MusicHandle = FSOUND_Stream_Open(pFileName, 0, 0, 0);
		
		FSOUND_Stream_SetMode(m_MusicHandle, FSOUND_LOOP_NORMAL);
		FSOUND_Stream_SetLoopCount(m_MusicHandle, -1);
		FSOUND_Stream_Play (0, m_MusicHandle);
		FSOUND_SetVolume(0, Vol);
	}
}


void FSOUND_Manager::PlayMusic()
{
	if(m_OKFlag && m_MusicHandle)
	{
		FSOUND_Stream_Play (0, m_MusicHandle);
	}
}



void FSOUND_Manager::StopMusic()
{
	if(m_OKFlag && m_MusicHandle)
	{
		FSOUND_Stream_Stop (m_MusicHandle);
	}
}


void FSOUND_Manager::PauseMusic()
{
	if(m_OKFlag && m_MusicHandle)
	{
		FSOUND_SetPaused (0, TRUE);
	}
}


void FSOUND_Manager::ResumeMusic()
{
	if(m_OKFlag && m_MusicHandle)
	{
		FSOUND_SetPaused (0, FALSE);
	}
}


bool FSOUND_Manager::MusicIsPlaying()
{
	if(m_OKFlag && m_MusicHandle)
	{
		return (FSOUND_IsPlaying(0) ? true : false);
	}

	return false;
}


void FSOUND_Manager::MusicSetVolume(int Vol)
{
	if(m_OKFlag && m_MusicHandle)
	{
		if(Vol > 255)
			Vol = 255;
		else
		if(Vol < 0)
			Vol = 0;

		FSOUND_SetVolume(0, Vol);
	}
}


FSound *FSOUND_Manager::CreateSoundA(PCSTRINGA pFileName)
{
	StringA strFileName(pFileName);
	strFileName.ToUpper();
	if (gGlobal.g_pSystemSetting && !gGlobal.g_pSystemSetting->IsSoundEffectModeEnable())
	{
		return NULL;
	}


	StlMap<StringA, FSound *>::iterator it = m_SoundMap.find(strFileName);

	if(it != m_SoundMap.end())
	{
		return it->second;
	}
	else
	{
		if(m_ChannelSocket[m_SocketIndex])
		{
			StlMap<StringA, FSound *>::iterator it = m_SoundMap.find(m_ChannelSocket[m_SocketIndex]->GetName());
			if(it != m_SoundMap.end())
			{
				//it->first.Empty();
				m_SoundMap.erase(it);
			}
			SafeDeleteObject(m_ChannelSocket[m_SocketIndex]);
		}

		FSound *Sound = SafeCreateObject(FSound);
		Sound->Load(strFileName.c_str(), m_SocketIndex);
		m_ChannelSocket[m_SocketIndex] = Sound;
		//m_SoundMap[pFileName] = Sound;
		//m_SoundMap.
		StlMap<StringA, FSound *>::iterator it = m_SoundMap.find(strFileName);
		if(it != m_SoundMap.end())
		{
			SafeDeleteObject(it->second);
			m_SoundMap.erase(it);
		}
		m_SoundMap.insert(std::make_pair(strFileName, Sound));
		do
		{
			if(++m_SocketIndex >= 31)
				m_SocketIndex = 1;
		}while(m_ChannelSocket[m_SocketIndex] && m_ChannelSocket[m_SocketIndex]->GetReserved());

		return Sound;
	}

	return NULL;
}



FSound *FSOUND_Manager::GetSound(PCSTRINGA pFilename)
{
	StringA strFileName(pFilename);
	strFileName.ToUpper();
	StlMap<StringA, FSound *>::iterator it = m_SoundMap.find(strFileName);

	if(it != m_SoundMap.end())
		return it->second;

	return NULL;
}


void FSOUND_Manager::PauseAll()
{
	FSOUND_SetMute(FSOUND_ALL, TRUE);
}


void FSOUND_Manager::ResumeAll()
{
	FSOUND_SetMute(FSOUND_ALL, FALSE);
}

	

//////////////////////////////////////////////////////////////


