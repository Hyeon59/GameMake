#include "stdafx.h"
#include "SoundSystem.h"
#include <FMod/fmod.h>

SoundSystem::SoundSystem()
{
	FMOD_RESULT result = FMOD_System_Create(&system);
	if (result != FMOD_OK)
		ASSERT(false);

	FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, nullptr);
}

SoundSystem::~SoundSystem()
{
	FMOD_Sound_Release(sound);
	FMOD_System_Close(system);
	FMOD_System_Release(system);
}

//배경음악용 사운드 객체생성
void SoundSystem::CreateBGSound(const string & fileName)
{
	FMOD_RESULT result = FMOD_System_CreateSound(system, fileName.c_str(), FMOD_LOOP_NORMAL, 0, &sound);
	if (result != FMOD_OK)
		ASSERT(false);

	bLoop = true;
}

void SoundSystem::CreateEffectSound(const string & fileName)
{
	FMOD_RESULT result = FMOD_System_CreateSound(system, fileName.c_str(), FMOD_DEFAULT, 0, &sound);
	if (result != FMOD_OK)
		ASSERT(false);

	bLoop = false;
}

void SoundSystem::Play()
{
	FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, 0, &channel);
}

void SoundSystem::Stop()
{
	FMOD_Channel_Stop(channel);
}

void SoundSystem::Update()
{
	if (system)
	{
		FMOD_System_Update(system);
	}

	FMOD_MODE mode = bLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	FMOD_Channel_SetMode(channel, mode);
}

void SoundSystem::SetVolume()
{
	FMOD_Channel_SetVolume(channel, volume);
}

void SoundSystem::SetMute()
{
	FMOD_Channel_SetMute(channel, bMute);
}

void SoundSystem::GUI()
{
	static bool bOpen = true;
	ImGui::Begin("SoundSystem", &bOpen);
	{
		if (ImGui::VSliderFloat("Volume", ImVec2(30, 50), &volume, 0, 1.0f, "%.2f"))
			SetVolume();
		
		if (ImGui::Checkbox("Mute", &bMute))
			SetMute();
		ImGui::Checkbox("Loop", &bLoop);
			
	}
	ImGui::End();
}
