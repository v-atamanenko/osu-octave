#include "AudioManager.h"

AudioManager AudioManager::sEngine;
Mix_Music *music;

AudioManager::AudioManager() {
#ifdef VITA
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
#else
    Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 1024);
#endif
    Mix_AllocateChannels(52);
}

inline void loadSample(SampleSetInfo* s, const std::string &file, OOInt volume) {
    if (s->chunk != nullptr) {
        Mix_FreeChunk(s->chunk);
    }

    s->filename = file;
    s->chunk = Mix_LoadWAV(s->filename.c_str());

    if (s->chunk != nullptr)
        Mix_VolumeChunk(s->chunk, volume);
}


void AudioManager::Initialize() {
    ResetSamples();

    mSampleSets[0] = nullptr; //"none" sample set >.>
    mSampleSets[1] = &mSampleNormal;
    mSampleSets[2] = &mSampleSoft;
}

void AudioManager::ResetSamples() {
    OOInt volume = (OOInt)round(Settings::get_float("volume_hitsounds") / 100 * 128);

    loadSample(&mSampleNormal.hitnormal, Skins::get_res_path(HITSOUND_NORMAL_HITNORMAL), volume);
    loadSample(&mSampleNormal.hitwhistle, Skins::get_res_path(HITSOUND_NORMAL_HITWHISTLE), volume);
    loadSample(&mSampleNormal.hitfinish, Skins::get_res_path(HITSOUND_NORMAL_HITFINISH), volume);
    loadSample(&mSampleNormal.hitclap, Skins::get_res_path(HITSOUND_NORMAL_HITCLAP), volume);
    loadSample(&mSampleNormal.slidertick, Skins::get_res_path(HITSOUND_NORMAL_SLIDERTICK), volume);
    loadSample(&mSampleNormal.sliderslide, Skins::get_res_path(HITSOUND_NORMAL_SLIDERSLIDE), volume);
    loadSample(&mSampleNormal.sliderwhistle, Skins::get_res_path(HITSOUND_NORMAL_SLIDERWHISTLE), volume);
    loadSample(&mSampleNormal.spinnerspin, Skins::get_res_path(HITSOUND_NORMAL_SPINNERSPIN), volume);
    loadSample(&mSampleNormal.spinnerbonus, Skins::get_res_path(HITSOUND_NORMAL_SPINNERBONUS), volume);
    loadSample(&mSampleSoft.hitnormal, Skins::get_res_path(HITSOUND_SOFT_HITNORMAL), volume);
    loadSample(&mSampleSoft.hitwhistle, Skins::get_res_path(HITSOUND_SOFT_HITWHISTLE), volume);
    loadSample(&mSampleSoft.hitfinish, Skins::get_res_path(HITSOUND_SOFT_HITFINISH), volume);
    loadSample(&mSampleSoft.hitclap, Skins::get_res_path(HITSOUND_SOFT_HITCLAP), volume);
    loadSample(&mSampleSoft.slidertick, Skins::get_res_path(HITSOUND_SOFT_SLIDERTICK), volume);
    loadSample(&mSampleSoft.sliderslide, Skins::get_res_path(HITSOUND_SOFT_SLIDERSLIDE), volume);
    loadSample(&mSampleSoft.sliderwhistle, Skins::get_res_path(HITSOUND_SOFT_SLIDERWHISTLE), volume);
    loadSample(&mSampleSoft.spinnerspin, Skins::get_res_path(HITSOUND_SOFT_SPINNERSPIN), volume);
    loadSample(&mSampleSoft.spinnerbonus, Skins::get_res_path(HITSOUND_SOFT_SPINNERBONUS), volume);

    OOInt uivolume = (OOInt)round(Settings::get_float("volume_uisounds") / 100 * 128);

    loadSample(&mUISounds.applause, Skins::get_res_path(UISOUND_APPLAUSE), uivolume);
    loadSample(&mUISounds.check_off, Skins::get_res_path(UISOUND_CHECK_OFF), uivolume);
    loadSample(&mUISounds.check_on, Skins::get_res_path(UISOUND_CHECK_ON), uivolume);
    loadSample(&mUISounds.click_close, Skins::get_res_path(UISOUND_CLICK_CLOSE), uivolume);
    loadSample(&mUISounds.click_short_confirm, Skins::get_res_path(UISOUND_CLICK_SHORT_CONFIRM), uivolume);
    loadSample(&mUISounds.combobreak, Skins::get_res_path(UISOUND_COMBOBREAK), MathHelper::Max((OOInt)(round(volume*1.33)), 100));
    loadSample(&mUISounds.failsound, Skins::get_res_path(UISOUND_FAILSOUND), MathHelper::Max((OOInt)(round(volume*1.33)), 100));
    loadSample(&mUISounds.menuback, Skins::get_res_path(UISOUND_MENUBACK), uivolume);
    loadSample(&mUISounds.menuclick, Skins::get_res_path(UISOUND_MENUCLICK), uivolume);
    loadSample(&mUISounds.menuhit, Skins::get_res_path(UISOUND_MENUHIT), uivolume);
    loadSample(&mUISounds.seeya, Skins::get_res_path(UISOUND_SEEYA), uivolume);
    loadSample(&mUISounds.welcome, Skins::get_res_path(UISOUND_WELCOME), uivolume);
    loadSample(&mUISounds.welcome_piano, Skins::get_res_path(UISOUND_WELCOME_PIANO), uivolume);
}

int AudioManager::PlaySample(const SampleSetInfo& info, bool loop, OOInt channel) {
   return Mix_PlayChannel(channel, info.chunk, loop ? -1 : 0);
}

static int PlayWelcome_Thread(void*) {
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

void AudioManager::PlayWelcome() {
    auto *thread = SDL_CreateThread(PlayWelcome_Thread, "PlayWelcome_Thread", (void *)nullptr);
    SDL_DetachThread(thread);
}

//TODO: Implement SetChannelFreq
//void AudioManager::SetChannelFreq(int channel, u16 freq)
//{
//	soundSetFreq(channel, freq);
//}

void AudioManager::StopChannel(OOInt channel) {
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

void AudioManager::PlayHitSound(HitObjectSound sound) {
	SampleSet* current = mSampleSets[BeatmapElements::Element().GetTimingPoint().SampleSetId];
	
	PlaySample(current->hitnormal);
	
	if (sound & SND_WHISTLE)
		PlaySample(current->hitwhistle);
	
	if (sound & SND_FINISH)
		PlaySample(current->hitfinish);
	
	if (sound & SND_CLAP)
		PlaySample(current->hitclap);
}

int AudioManager::PlaySliderSound(HitObjectSound sound) {
	SampleSet* current = mSampleSets[BeatmapElements::Element().GetTimingPoint().SampleSetId];
	
	if (sound & SND_WHISTLE)
		return PlaySample(current->sliderwhistle, true);
	else
		return PlaySample(current->sliderslide, true);
}

int AudioManager::PlaySpinnerSound(HitObjectSound sound) {
    SampleSet* current = mSampleSets[BeatmapElements::Element().GetTimingPoint().SampleSetId];

    if (sound & SND_BONUS) {
        // channel=5 is a hacky solution for spinner bonus sound being played forever otherwise.
        return PlaySample(current->spinnerbonus, true, 5);
    } else {
        return PlaySample(current->spinnerspin, true);
    }
}

void AudioManager::PlaySliderTick() {
	SampleSet* current = mSampleSets[BeatmapElements::Element().GetTimingPoint().SampleSetId];
	
	PlaySample(current->slidertick);
}

void AudioManager::PlayBGM() {
    if (mBGMStarted) return;

    MusicLoad(Skins::get_res_path(UISOUND_BGM));
    mBGMStarted = true;
    MusicPlay(Settings::get_float("volume_menumusic"));
}

void AudioManager::UpdateMusicVolume(OOFloat volume) {
    Mix_VolumeMusic((OOInt)round(volume / 100 * 128));
}

int AudioManager::MusicLoad(const std::string& filename) {
    mBGMStarted = false;

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

int AudioManager::MusicPlay(OOFloat volume) {
    if (music != nullptr) {
        Mix_VolumeMusic((OOInt)round(volume/100*128));
        mChannel = Mix_PlayMusic(music, -1);
    }
    return mChannel;
}

int AudioManager::MusicSkipTo(OOTime milliseconds) {
    if (mChannel == -1) {
        fprintf(stderr, "Mix_SetMusicPosition failed: mChannel is -1\n");
		return -1;
    }

    if (Mix_SetMusicPosition((double)((OOFloat)milliseconds / (OOFloat)1000)) == -1) {
        fprintf(stderr, "Mix_SetMusicPosition failed: %s\n", Mix_GetError());
        mChannel = -1;
    }

    return mChannel;
}

void AudioManager::MusicStop() {
	if (mChannel == -1)
		return;

    Mix_HaltMusic();
    mChannel = -1;
}

void AudioManager::MusicPause() const {
    if (mChannel == -1)
        return;
    Mix_PauseMusic();
}

void AudioManager::MusicResume() const {
    if (mChannel == -1)
        return;
    Mix_ResumeMusic();
}
