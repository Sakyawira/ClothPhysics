#pragma once

//#pragma comment(lib, "fmodex_vc.lib" ) // fmod library

#include "fmod.hpp" //fmod c++ header

//Sound array size
#define NUM_SOUNDS 15

//Sound identifiers
#define	SOUND_BGM1			0
#define	SOUND_BGM2			1

#define SOUND_GAMEOVER      2
#define	SOUND_MOVE			3
#define	SOUND_CONSUME		4
#define	SOUND_HIT           5
#define	SOUND_EXPLOSION     6
#define	SOUND_FIREBALL      7
#define	SOUND_SUPERFIREBALL 8
#define	SOUND_ARROW         9
#define	SOUND_DEATH		    10
#define	SOUND_TOKEN		    11
#define	SOUND_FAIL		    12
#define	SOUND_CHARGED		13
#define SOUND_VICTORY       14

class Audio
{
public:
	Audio(void);
	virtual ~Audio(void);

	bool Load();
	void Play(int sound_id);
	void Stop(/*int sound_id*/);
	void SmoothStop(int sound_id);
	void StopAll();
	void Update();

	FMOD::System*		audioSystem;			//handle to FMOD engine
	FMOD::Sound*		sounds[NUM_SOUNDS];		//sound that will be loaded and played
	FMOD::Channel*		ambient1Channel;
	FMOD::Channel*		ambient2Channel;
	FMOD::Channel*		sfxChannel;
	FMOD::DSP*			dspSmoothStop;

	FMOD::Sound*		fxThump;
	FMOD::Sound*		trackBackground;
	FMOD_RESULT			result;
};

//bool AudioInit()
//{
//	result = FMOD::System_Create(&audioSystem);
//
//	if (result != FMOD_OK)
//	{
//		return false;
//	}
//
//	result = audioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
//
//	if (result != FMOD_OK)
//	{
//		return false;
//	}
//
//	result = audioSystem->createSound("Resources/Audio/Thump.wav", FMOD_DEFAULT, 0, &fxThump);
//
//	if (result != FMOD_OK)
//	{
//		return false;
//	}
//
//	return true;
//}