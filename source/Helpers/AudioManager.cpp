#include <unistd.h>
#include "defines.h"
#include "DataStorage/Settings.h"
#include "AudioManager.h"

AudioManager AudioManager::sEngine;
Mix_Music *music;

AudioManager::AudioManager()
{
#ifdef VITA
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
#else
    Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 1024);
#endif
    int n = Mix_AllocateChannels(6);
    printf("Allocated %i channels for SDL_Mixer\n", n);

	//sound init
	ResetSamples();
	
	mSampleSets[0] = nullptr; //"none" sample set >.>
	mSampleSets[1] = &mSampleNormal;
	mSampleSets[2] = &mSampleSoft;
	
	//music init
	//mChannel = -1;
	//mBuffer = (u8*)new uint32_t[SIZE*2/4+1]; //div4 because uint32_t = 4 * u8
	//irqEnable(IRQ_TIMER0);
	//irqSet(IRQ_TIMER0, MusicTimerHandler);
}

void AudioManager::ResetSamples()
{
    std::string path = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/sounds/";
    mSampleNormal.hitnormal.filename = path + "normal-hitnormal.wav";
    mSampleNormal.hitnormal.chunk = Mix_LoadWAV(mSampleNormal.hitnormal.filename.c_str());

    mSampleNormal.hitwhistle.filename = path + "normal-hitwhistle.wav";
    mSampleNormal.hitwhistle.chunk = Mix_LoadWAV(mSampleNormal.hitwhistle.filename.c_str());

    mSampleNormal.hitfinish.filename = path + "normal-hitfinish.wav";
    mSampleNormal.hitfinish.chunk = Mix_LoadWAV(mSampleNormal.hitfinish.filename.c_str());

    mSampleNormal.hitclap.filename = path + "normal-hitclap.wav";
    mSampleNormal.hitclap.chunk = Mix_LoadWAV(mSampleNormal.hitclap.filename.c_str());

    mSampleNormal.slidertick.filename = path + "normal-slidertick.wav";
    mSampleNormal.slidertick.chunk = Mix_LoadWAV(mSampleNormal.slidertick.filename.c_str());

    mSampleNormal.sliderslide.filename = path + "normal-sliderslide.wav";
    mSampleNormal.sliderslide.chunk = Mix_LoadWAV(mSampleNormal.sliderslide.filename.c_str());

    mSampleNormal.sliderwhistle.filename = path + "normal-sliderwhistle.wav";
    mSampleNormal.sliderwhistle.chunk = Mix_LoadWAV(mSampleNormal.sliderwhistle.filename.c_str());

    mSampleNormal.spinnerspin.filename = path + "spinnerspin.wav";
    mSampleNormal.spinnerspin.chunk = Mix_LoadWAV(mSampleNormal.spinnerspin.filename.c_str());

    mSampleNormal.spinnerbonus.filename = path + "spinnerbonus.wav";
    mSampleNormal.spinnerbonus.chunk = Mix_LoadWAV(mSampleNormal.spinnerbonus.filename.c_str());

    mSampleSoft.hitnormal.filename = path + "soft-hitnormal.wav";
    mSampleSoft.hitnormal.chunk = Mix_LoadWAV(mSampleSoft.hitnormal.filename.c_str());

    mSampleSoft.hitwhistle.filename = path + "soft-hitwhistle.wav";
    mSampleSoft.hitwhistle.chunk = Mix_LoadWAV(mSampleSoft.hitwhistle.filename.c_str());

    mSampleSoft.hitfinish.filename = path + "soft-hitfinish.wav";
    mSampleSoft.hitfinish.chunk = Mix_LoadWAV(mSampleSoft.hitfinish.filename.c_str());

    mSampleSoft.hitclap.filename = path + "soft-hitclap.wav";
    mSampleSoft.hitclap.chunk = Mix_LoadWAV(mSampleSoft.hitclap.filename.c_str());

    mSampleSoft.slidertick.filename = path + "soft-slidertick.wav";
    mSampleSoft.slidertick.chunk = Mix_LoadWAV(mSampleSoft.slidertick.filename.c_str());

    mSampleSoft.sliderslide.filename = path + "soft-sliderslide.wav";
    mSampleSoft.sliderslide.chunk = Mix_LoadWAV(mSampleSoft.sliderslide.filename.c_str());

    mSampleSoft.sliderwhistle.filename = path + "soft-sliderwhistle.wav";
    mSampleSoft.sliderwhistle.chunk = Mix_LoadWAV(mSampleSoft.sliderwhistle.filename.c_str());

    mSampleSoft.spinnerspin.filename = path + "spinnerspin.wav";
    mSampleSoft.spinnerspin.chunk = Mix_LoadWAV(mSampleSoft.spinnerspin.filename.c_str());

    mSampleSoft.spinnerbonus.filename = path + "spinnerbonus.wav";
    mSampleSoft.spinnerbonus.chunk = Mix_LoadWAV(mSampleSoft.spinnerbonus.filename.c_str());
}

int AudioManager::PlaySample(SampleSetInfo info, bool loop, int channel)
{
   return Mix_PlayChannel(channel, info.chunk, loop ? -1 : 0);
}

//TODO: Implement SetChannelFreq
//void AudioManager::SetChannelFreq(int channel, u16 freq)
//{
//	soundSetFreq(channel, freq);
//}

void AudioManager::StopChannel(int channel)
{
    Mix_HaltChannel(channel);
}

void AudioManager::PlayHitSound(HitObjectSound sound)
{
	SampleSet* current = mSampleSets[BeatmapElements::Element().GetTimingPoint().SampleSetId];
	
	PlaySample(current->hitnormal);
	
	if (sound & SND_WHISTLE)
		PlaySample(current->hitwhistle);
	
	if (sound & SND_FINISH)
		PlaySample(current->hitfinish);
	
	if (sound & SND_CLAP)
		PlaySample(current->hitclap);
}

int AudioManager::PlaySliderSound(HitObjectSound sound)
{
	SampleSet* current = mSampleSets[BeatmapElements::Element().GetTimingPoint().SampleSetId];
	
	if (sound & SND_WHISTLE)
		return PlaySample(current->sliderwhistle, true);
	else
		return PlaySample(current->sliderslide, true);
}

int AudioManager::PlaySpinnerSound(HitObjectSound sound)
{
    SampleSet* current = mSampleSets[BeatmapElements::Element().GetTimingPoint().SampleSetId];

    if (sound & SND_BONUS) {
        // channel=5 is a hacky solution for spinner bonus sound being played forever otherwise.
        return PlaySample(current->spinnerbonus, true, 5);
    } else {
        return PlaySample(current->spinnerspin, true);
    }
}

void AudioManager::PlaySliderTick()
{
	SampleSet* current = mSampleSets[BeatmapElements::Element().GetTimingPoint().SampleSetId];
	
	PlaySample(current->slidertick);
}

int AudioManager::MusicLoad(std::string& filename) {
    if (music != nullptr) {
        MusicStop();
        Mix_FreeMusic(music);
    }

    mChannel = 0;

    music = Mix_LoadMUS(filename.c_str());
    if(!music) {
        char temp[1024];
        getcwd(temp, sizeof(temp));

        fprintf(stderr, "Mix_LoadMUS(\"%s\") failed: %s\n", filename.c_str(), Mix_GetError());
        fprintf(stderr, "filename: %s. cwd: %s.\n", filename.c_str(), temp);

        mChannel = -1;
    }

    return mChannel;
}

int AudioManager::MusicPlay() {
    if (music != nullptr) {
        mChannel = Mix_PlayMusic(music, -1);
        fprintf(stderr, "Started music on channel %i\n", mChannel);
    }
    return mChannel;
}

int AudioManager::MusicSkipTo(uint32_t milliseconds)
{
    if (mChannel == -1) {
        fprintf(stderr, "Mix_SetMusicPosition failed: mChannel is -1\n");
		return -1;
    }

    if (Mix_SetMusicPosition((double)((double)milliseconds / (double)1000)) == -1) {
        fprintf(stderr, "Mix_SetMusicPosition failed: %s\n", Mix_GetError());
        mChannel = -1;
    }

    return mChannel;
}

void AudioManager::MusicStop()
{
	if (mChannel == -1)
		return;

    Mix_HaltMusic();
    mChannel = -1;
}

void AudioManager::MusicPause() const
{
    if (mChannel == -1)
        return;
    Mix_PauseMusic();
}

void AudioManager::MusicResume() const
{
    if (mChannel == -1)
        return;
    Mix_ResumeMusic();
}