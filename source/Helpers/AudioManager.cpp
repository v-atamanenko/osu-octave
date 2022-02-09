#include <unistd.h>
#include "AudioManager.h"

AudioManager AudioManager::sEngine;
Mix_Music *music;

AudioManager::AudioManager()
{
    Mix_OpenAudio(44100, AUDIO_S16SYS, 4, 640);

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
    mSampleNormal.hitnormal.filename = "data/sounds/normal_hitnormal.wav";
    mSampleNormal.hitnormal.chunk = Mix_LoadWAV(mSampleNormal.hitnormal.filename);

    mSampleNormal.hitwhistle.filename = "data/sounds/normal_hitwhistle.wav";
    mSampleNormal.hitwhistle.chunk = Mix_LoadWAV(mSampleNormal.hitwhistle.filename);

    mSampleNormal.hitfinish.filename = "data/sounds/normal_hitfinish.wav";
    mSampleNormal.hitfinish.chunk = Mix_LoadWAV(mSampleNormal.hitfinish.filename);

    mSampleNormal.hitclap.filename = "data/sounds/normal_hitclap.wav";
    mSampleNormal.hitclap.chunk = Mix_LoadWAV(mSampleNormal.hitclap.filename);

    mSampleNormal.slidertick.filename = "data/sounds/normal_slidertick.wav";
    mSampleNormal.slidertick.chunk = Mix_LoadWAV(mSampleNormal.slidertick.filename);

    mSampleNormal.sliderslide.filename = "data/sounds/normal_sliderslide.wav";
    mSampleNormal.sliderslide.chunk = Mix_LoadWAV(mSampleNormal.sliderslide.filename);

    mSampleNormal.sliderwhistle.filename = "data/sounds/normal_sliderwhistle.wav";
    mSampleNormal.sliderwhistle.chunk = Mix_LoadWAV(mSampleNormal.sliderwhistle.filename);

    mSampleNormal.spinnerspin.filename = "data/sounds/spinnerspin.wav";
    mSampleNormal.spinnerspin.chunk = Mix_LoadWAV(mSampleNormal.spinnerspin.filename);

    mSampleNormal.spinnerbonus.filename = "data/sounds/spinnerbonus.wav";
    mSampleNormal.spinnerbonus.chunk = Mix_LoadWAV(mSampleNormal.spinnerbonus.filename);


    mSampleSoft.hitnormal.filename = "data/sounds/soft_hitnormal.wav";
    mSampleSoft.hitnormal.chunk = Mix_LoadWAV(mSampleSoft.hitnormal.filename);

    mSampleSoft.hitwhistle.filename = "data/sounds/soft_hitwhistle.wav";
    mSampleSoft.hitwhistle.chunk = Mix_LoadWAV(mSampleSoft.hitwhistle.filename);

    mSampleSoft.hitfinish.filename = "data/sounds/soft_hitfinish.wav";
    mSampleSoft.hitfinish.chunk = Mix_LoadWAV(mSampleSoft.hitfinish.filename);

    mSampleSoft.hitclap.filename = "data/sounds/soft_hitclap.wav";
    mSampleSoft.hitclap.chunk = Mix_LoadWAV(mSampleSoft.hitclap.filename);

    mSampleSoft.slidertick.filename = "data/sounds/soft_slidertick.wav";
    mSampleSoft.slidertick.chunk = Mix_LoadWAV(mSampleSoft.slidertick.filename);

    mSampleSoft.sliderslide.filename = "data/sounds/soft_sliderslide.wav";
    mSampleSoft.sliderslide.chunk = Mix_LoadWAV(mSampleSoft.sliderslide.filename);

    mSampleSoft.sliderwhistle.filename = "data/sounds/soft_sliderwhistle.wav";
    mSampleSoft.sliderwhistle.chunk = Mix_LoadWAV(mSampleSoft.sliderwhistle.filename);

    mSampleSoft.spinnerspin.filename = "data/sounds/spinnerspin.wav";
    mSampleSoft.spinnerspin.chunk = Mix_LoadWAV(mSampleSoft.spinnerspin.filename);

    mSampleSoft.spinnerbonus.filename = "data/sounds/spinnerbonus.wav";
    mSampleSoft.spinnerbonus.chunk = Mix_LoadWAV(mSampleSoft.spinnerbonus.filename);
}

int AudioManager::PlaySample(SampleSetInfo info, bool loop)
{
   return Mix_PlayChannel(-1, info.chunk, loop ? -1 : 0);
}

//FIXME: Implement SetChannelFreq
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

    if (sound & SND_BONUS)
        return PlaySample(current->spinnerbonus, true);
    else
        return PlaySample(current->spinnerspin, true);
}

void AudioManager::PlaySliderTick()
{
	SampleSet* current = mSampleSets[BeatmapElements::Element().GetTimingPoint().SampleSetId];
	
	PlaySample(current->slidertick);
}

int AudioManager::MusicPlay(std::string& filename)
{
    if (music != nullptr)
        MusicStop();

    music = Mix_LoadMUS(filename.c_str());
    if(!music) {
        char temp[1024];
        getcwd(temp, sizeof(temp));

        fprintf(stderr, "Mix_LoadMUS(\"%s\") failed: %s\n", filename.c_str(), Mix_GetError());
        fprintf(stderr, "filename: %s. cwd: %s.\n", filename.c_str(), temp);

        mChannel = -1;
        return mChannel;
    }

    mChannel = Mix_PlayMusic(music, -1);
    fprintf(stderr, "Started music on channel %i\n", mChannel);
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
    Mix_FreeMusic(music);
	
	mChannel = -1;
}

