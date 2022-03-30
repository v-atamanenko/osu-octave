#pragma once

#include <cstdio>
#include <string>

#include "SDL.h"
#include "SDL_mixer.h"

#include <unistd.h>
#include "defines.h"
#include "DataStorage/Settings.h"
#include "Beatmaps/BeatmapElements.h"

typedef enum {
	SND_NORMAL = 0,
	SND_WHISTLE = 1,
	SND_FINISH = 2,
	SND_CLAP = 3,
    SND_BONUS = 32 // Spinner only
} HitObjectSound;

typedef struct {
	std::string filename;
    Mix_Chunk * chunk;
} SampleSetInfo;

typedef struct {
	SampleSetInfo hitnormal;
	SampleSetInfo hitwhistle;
	SampleSetInfo hitfinish;
	SampleSetInfo hitclap;
	SampleSetInfo slidertick;
	SampleSetInfo sliderslide;
	SampleSetInfo sliderwhistle;
    SampleSetInfo spinnerspin;
    SampleSetInfo spinnerbonus;
} SampleSet;

typedef struct {
    SampleSetInfo applause;
    SampleSetInfo check_off;
    SampleSetInfo check_on;
    SampleSetInfo click_close;
    SampleSetInfo click_short_confirm;
    SampleSetInfo combobreak;
    SampleSetInfo failsound;
    SampleSetInfo menuback;
    SampleSetInfo menuclick;
    SampleSetInfo menuhit;
    SampleSetInfo seeya;
    SampleSetInfo welcome;
    SampleSetInfo welcome_piano;
} UISounds;

typedef enum UISoundName {
    UISOUND_APPLAUSE,
    UISOUND_CHECK_OFF,
    UISOUND_CHECK_ON,
    UISOUND_CLICK_CLOSE,
    UISOUND_CLICK_SHORT_CONFIRM,
    UISOUND_COMBOBREAK,
    UISOUND_FAILSOUND,
    UISOUND_MENUBACK,
    UISOUND_MENUCLICK,
    UISOUND_MENUHIT,
    UISOUND_SEEYA,
    UISOUND_WELCOME,
    UISOUND_WELCOME_PIANO
} UISoundName;

class AudioManager
{
	public:
		static AudioManager& Engine() { return sEngine; }

        void Initialize();
		
		int PlaySample(const SampleSetInfo& info, bool loop = false, OOInt channel = -1);
        //FIXME: void SetChannelFreq(int channel, uint16_t freq);
		void StopChannel(OOInt channel);
		
		//sounds
		void ResetSamples();
		void PlayHitSound(HitObjectSound sound);
		int PlaySliderSound(HitObjectSound sound);
		void PlaySliderTick();
        int PlaySpinnerSound(HitObjectSound sound);
        void PlayUISound(UISoundName n);
        static void PlayWelcome();
		
		//music
        int MusicLoad(std::string& filename);
		int MusicPlay(OOFloat volume=100.0); // volume: 0-100.f
		int MusicSkipTo(OOTime milliseconds);
        void UpdateMusicVolume(OOFloat volume);
		void MusicStop();

        void PlayBGM();

        void MusicPause() const;
        void MusicResume() const;
	
	protected:
		static AudioManager sEngine;
		
		SampleSet mSampleNormal;
		SampleSet mSampleSoft;
		SampleSet* mSampleSets[3];
        UISounds mUISounds;

        bool mBGMStarted = false;
		
		OOInt mChannel;

	private:
		AudioManager();
		~AudioManager() = default;
};
