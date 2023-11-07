#ifndef _PLAYOGGVORBIS_H
#define _PLAYOGGVORBIS_H

#include "fmod.h"

#include <windows.h>
#include <process.h>

#include <string>


class FSound
{
	FSOUND_SAMPLE	*m_Sample;
	StringA			m_Name;
	int				m_Channel;
	int				m_Vol;
	bool			m_Reserved;

public:

	FSound();
	~FSound();

	int Load(PCSTRINGA Filename, int Channel);
	void Close();
	void Play();
	void Stop();
	void Pause();
	void Resume();
	void SetVolume(int Vol);
	
	bool IsPlaying();

	int	GetChannel(){return m_Channel;}
	StringA	&GetName(){return m_Name;}
	FSOUND_SAMPLE	*GetSample(){return m_Sample;}

	void SetReserved(bool reserved){ m_Reserved = reserved;}
	bool GetReserved(){	return m_Reserved; }
};


class FSOUND_Manager : public Reuben::Utility::Singleton<FSOUND_Manager>
{
	signed char			m_OKFlag;
	FSOUND_STREAM		*m_MusicHandle;
	StringA				m_MusicName;

	FSound				*m_ChannelSocket[32];
	int					m_SocketIndex;
	StlMap<StringA, FSound *>	m_SoundMap;

public:
	FSOUND_Manager();
	~FSOUND_Manager();

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
	FSound *CreateSound(PCSTRINGW pFileName){return CreateSoundA(StringA(pFileName).c_str());}
	FSound *GetSound(int Channel){return m_ChannelSocket[Channel];};
	FSound *GetSound(PCSTRINGA pFilename);

	void PauseAll();
	void ResumeAll();

};

inline FSOUND_Manager *GetFSoundManager()
{
	return &(FSOUND_Manager::GetInstance());
}


#endif
