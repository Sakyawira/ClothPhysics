#include "Audio.h"

Audio::Audio(void)
{
	FMOD::System_Create(&audioSystem);// create an instance of the game engine
	audioSystem->init(32, FMOD_INIT_NORMAL, 0);// initialise the game engine with 32 channels (cantidad de sonido simultaneo que puede haber)
}

Audio::~Audio(void)
{
	for (int i = 0; i < NUM_SOUNDS; i++)
	{
		sounds[i]->release();
	}
	audioSystem->release();
}

bool Audio::Load()
{
	audioSystem->createStream("Resources/Audio/Chase LOOP.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &sounds[SOUND_BGM1]);
	audioSystem->createStream("Resources/Audio/Fire/FIRE_Campfire_Calm_Smooth_01_loop_mono.wav", FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &sounds[SOUND_BGM2]);
	audioSystem->createSound("Resources/Audio/GORE_SPLATS/GORE_Stab_Splat_Only_Bubbles_mono.wav", FMOD_DEFAULT, 0, &sounds[SOUND_GAMEOVER]);
	audioSystem->createSound("Resources/Audio/THUD_Bright_04_Movement_mono.wav", FMOD_DEFAULT, 0, &sounds[SOUND_MOVE]);
	audioSystem->createSound("Resources/Audio/EAT_Swallow_mono.wav", FMOD_DEFAULT, 0, &sounds[SOUND_CONSUME]);
	//audioSystem->createSound("Sounds/hit.wav", FMOD_DEFAULT, 0, &sounds[SOUND_HIT]);
	//audioSystem->createSound("Sounds/explosion.wav", FMOD_DEFAULT, 0, &sounds[SOUND_EXPLOSION]);
	//audioSystem->createSound("Sounds/fireball.wav", FMOD_DEFAULT, 0, &sounds[SOUND_FIREBALL]);
	//audioSystem->createSound("Sounds/superfireball.wav", FMOD_DEFAULT, 0, &sounds[SOUND_SUPERFIREBALL]);
	//audioSystem->createSound("Sounds/arrow.wav", FMOD_DEFAULT, 0, &sounds[SOUND_ARROW]);
	//audioSystem->createSound("Sounds/death.wav", FMOD_DEFAULT, 0, &sounds[SOUND_DEATH]);
	//audioSystem->createSound("Sounds/token.wav", FMOD_DEFAULT, 0, &sounds[SOUND_TOKEN]);
	//audioSystem->createSound("Sounds/fail.wav", FMOD_DEFAULT, 0, &sounds[SOUND_FAIL]);
	//audioSystem->createSound("Sounds/charged.wav", FMOD_DEFAULT, 0, &sounds[SOUND_CHARGED]);
	//audioSystem->createSound("Resources/Audio/Thump.wav", FMOD_DEFAULT, 0, &sounds[SOUND_VICTORY]);
	return true;
}

void Audio::Play(int sound_id)
{
	if (sound_id == SOUND_BGM1)
	{
		audioSystem->playSound(sounds[SOUND_BGM1], 0, false, &ambient1Channel);
		ambient1Channel->setVolume(0.25f); //between 0 and 1
	}
	else if (sound_id == SOUND_BGM2) 
	{
		audioSystem->playSound(sounds[SOUND_BGM2], 0, false, &ambient2Channel);
		ambient2Channel->setVolume(0.2f); //between 0 and 1
	}
	else
	{
		audioSystem->playSound(sounds[sound_id], 0, false, &sfxChannel);
		sfxChannel->setVolume(0.1f);
	}
}

void Audio::Stop(/*int sound_id*/)
{
	//sfxChannel->stop();
}

void Audio::StopAll()
{
	ambient1Channel->stop();
	ambient2Channel->stop();
}

void Audio::Update()
{
	audioSystem->update();
}