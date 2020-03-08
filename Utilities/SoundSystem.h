#pragma once

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	void CreateBGSound(const string& fileName);
	void CreateEffectSound(const string& fileName);

	void Play();
	void Stop();

	void Update();

	void SetVolume();
	void SetMute();

	void GUI();

private:
	struct FMOD_SYSTEM* system = nullptr;
	struct FMOD_SOUND* sound = nullptr;
	struct FMOD_CHANNEL* channel = nullptr;

	float volume = 1.0f;
	bool bMute = false;
	bool bLoop = false;
};