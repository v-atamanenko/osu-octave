#include <unistd.h>
#include "defines.h"
#include "DataStorage/Settings.h"
#include "AudioManager.h"

AudioManager AudioManager::sEngine;
Mix_Music *music;

AudioManager::AudioManager() {
#ifdef VITA
    Mix_OpenAudio(44100, AUDIO_S16SYS, 6, 1024);
#else
    Mix_OpenAudio(44100, AUDIO_S32SYS, 6, 1024);
#endif
}

inline void loadSample(SampleSetInfo* s, const std::string &file, int volume) {
    s->filename = file;
    s->chunk = Mix_LoadWAV(s->filename.c_str());
    Mix_VolumeChunk(s->chunk, volume);
}

void AudioManager::Initialize() {
    ResetSamples();

    mSampleSets[0] = nullptr; //"none" sample set >.>
    mSampleSets[1] = &mSampleNormal;
    mSampleSets[2] = &mSampleSoft;
}

void AudioManager::ResetSamples()
{
    std::string path = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/sounds/";
    int volume = (int)round(Settings::get_float("volume_hitsounds") / 100 * 128);

    loadSample(&mSampleNormal.hitnormal, path + "normal-hitnormal.wav", volume);
    loadSample(&mSampleNormal.hitwhistle, path + "normal-hitwhistle.wav", volume);
    loadSample(&mSampleNormal.hitfinish, path + "normal-hitfinish.wav", volume);
    loadSample(&mSampleNormal.hitclap, path + "normal-hitclap.wav", volume);
    loadSample(&mSampleNormal.slidertick, path + "normal-slidertick.wav", volume);
    loadSample(&mSampleNormal.sliderslide, path + "normal-sliderslide.wav", volume);
    loadSample(&mSampleNormal.sliderwhistle, path + "normal-sliderwhistle.wav", volume);
    loadSample(&mSampleNormal.spinnerspin, path + "spinnerspin.wav", volume);
    loadSample(&mSampleNormal.spinnerbonus, path + "spinnerbonus.wav", volume);
    loadSample(&mSampleSoft.hitnormal, path + "soft-hitnormal.wav", volume);
    loadSample(&mSampleSoft.hitwhistle, path + "soft-hitwhistle.wav", volume);
    loadSample(&mSampleSoft.hitfinish, path + "soft-hitfinish.wav", volume);
    loadSample(&mSampleSoft.hitclap, path + "soft-hitclap.wav", volume);
    loadSample(&mSampleSoft.slidertick, path + "soft-slidertick.wav", volume);
    loadSample(&mSampleSoft.sliderslide, path + "soft-sliderslide.wav", volume);
    loadSample(&mSampleSoft.sliderwhistle, path + "soft-sliderwhistle.wav", volume);
    loadSample(&mSampleSoft.spinnerspin, path + "spinnerspin.wav", volume);
    loadSample(&mSampleSoft.spinnerbonus, path + "spinnerbonus.wav", volume);

    std::string uipath = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/ui-sounds/";
    int uivolume = (int)round(Settings::get_float("volume_uisounds") / 100 * 128);

    loadSample(&mUISounds.applause, uipath + "applause.wav", uivolume);
    loadSample(&mUISounds.check_off, uipath + "check-off.wav", uivolume);
    loadSample(&mUISounds.check_on, uipath + "check-on.wav", uivolume);
    loadSample(&mUISounds.click_close, uipath + "click-close.wav", uivolume);
    loadSample(&mUISounds.click_short_confirm, uipath + "click-short-confirm.wav", uivolume);
    loadSample(&mUISounds.combobreak, uipath + "combobreak.wav",  MathHelper::Max((int)(round(volume*1.33)), 100));
    loadSample(&mUISounds.failsound, uipath + "failsound.wav", MathHelper::Max((int)(round(volume*1.33)), 100));
    loadSample(&mUISounds.menuback, uipath + "menuback.wav", uivolume);
    loadSample(&mUISounds.menuclick, uipath + "menuclick.wav", uivolume);
    loadSample(&mUISounds.menuhit, uipath + "menuhit.wav", uivolume);
    loadSample(&mUISounds.seeya, uipath + "seeya.wav", uivolume);
    loadSample(&mUISounds.welcome, uipath + "welcome.wav", uivolume);
    loadSample(&mUISounds.welcome_piano, uipath + "welcome_piano.wav", uivolume);
}

int AudioManager::PlaySample(SampleSetInfo info, bool loop, int channel)
{
   return Mix_PlayChannel(channel, info.chunk, loop ? -1 : 0);
}

static int PlayWelcome_Thread(void*)
{
    AudioManager::Engine().PlayUISound(UISOUND_WELCOME);
    SDL_Delay(1220);
    if (music != nullptr) {
        // Beatmap started, don't continue with welcome sounds
        return 0;
    }
    AudioManager::Engine().PlayUISound(UISOUND_WELCOME_PIANO);
    SDL_Delay(2142);
    if (music != nullptr) {
        // Beatmap started, don't continue with welcome sounds
        return 0;
    }
    AudioManager::Engine().PlayBGM();
    return 0;
}

void AudioManager::PlayWelcome()
{
    auto *thread = SDL_CreateThread(PlayWelcome_Thread, "PlayWelcome_Thread", (void *)nullptr);
    SDL_DetachThread(thread);
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

void AudioManager::PlayUISound(UISoundName n) {
    switch (n) {
        case (UISOUND_APPLAUSE):
            PlaySample(mUISounds.applause);
            break;
        case UISOUND_CHECK_OFF:
            PlaySample(mUISounds.check_off);
            break;
        case UISOUND_CHECK_ON:
            PlaySample(mUISounds.check_on);
            break;
        case UISOUND_CLICK_CLOSE:
            PlaySample(mUISounds.click_close);
            break;
        case UISOUND_CLICK_SHORT_CONFIRM:
            PlaySample(mUISounds.click_short_confirm);
            break;
        case UISOUND_COMBOBREAK:
            PlaySample(mUISounds.combobreak);
            break;
        case UISOUND_FAILSOUND:
            PlaySample(mUISounds.failsound);
            break;
        case UISOUND_MENUBACK:
            PlaySample(mUISounds.menuback);
            break;
        case UISOUND_MENUCLICK:
            PlaySample(mUISounds.menuclick);
            break;
        case UISOUND_MENUHIT:
            PlaySample(mUISounds.menuhit);
            break;
        case UISOUND_SEEYA:
            PlaySample(mUISounds.seeya);
            break;
        case UISOUND_WELCOME:
            PlaySample(mUISounds.welcome);
            break;
        case UISOUND_WELCOME_PIANO:
            PlaySample(mUISounds.welcome_piano);
            break;
    }
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

void AudioManager::PlayBGM() {
    std::string music_path = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/ui-sounds/bgm.mp3";
    MusicLoad(music_path);
    MusicPlay(Settings::get_float("volume_menumusic"));
}

void AudioManager::UpdateMusicVolume(float volume) {
    Mix_VolumeMusic((int)round(volume / 100 * 128));
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

int AudioManager::MusicPlay(float volume) {
    if (music != nullptr) {
        Mix_VolumeMusic((int)round(volume/100*128));
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