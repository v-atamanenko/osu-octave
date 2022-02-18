#include <cstdio>
#include <string>

#include "SDL.h"
#include "SDL_mixer.h"

#include "Beatmaps/BeatmapElements.h"

#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

/*
#include "normal_hitnormal_bin.h"
#include "normal_hitwhistle_bin.h"
#include "normal_hitfinish_bin.h"
#include "normal_hitclap_bin.h"
#include "normal_slidertick_bin.h"
#include "normal_sliderslide_bin.h"
#include "normal_sliderwhistle_bin.h"

#include "soft_hitnormal_bin.h"
#include "soft_hitwhistle_bin.h"
#include "soft_hitfinish_bin.h"
#include "soft_hitclap_bin.h"
#include "soft_slidertick_bin.h"
#include "soft_sliderslide_bin.h"
#include "soft_sliderwhistle_bin.h"

#include "spinnerbonus_bin.h"
#include "spinnerspin_bin.h"
*/

typedef enum {
	SND_NORMAL = 0,
	SND_WHISTLE = 1,
	SND_FINISH = 2,
	SND_CLAP = 3,
    SND_BONUS = 32 // Spinner only
} HitObjectSound;

typedef struct {
	const char* filename;
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

//intended usage:
//AudioManager::Engine().PlaySample(SOUND_DATA(sound_name), loop)

class AudioManager
{
	public:
		static AudioManager& Engine() { return sEngine; }
		
		int PlaySample(SampleSetInfo info, bool loop = false);
        //FIXME: void SetChannelFreq(int channel, uint16_t freq);
		void StopChannel(int channel);
		
		//sounds
		void ResetSamples();
		void PlayHitSound(HitObjectSound sound);
		int PlaySliderSound(HitObjectSound sound);
		void PlaySliderTick();
        int PlaySpinnerSound(HitObjectSound sound);
		
		//music
        int MusicLoad(std::string& filename);
		int MusicPlay();
		int MusicSkipTo(uint32_t milliseconds);
		void MusicStop();
	
	protected:
		static AudioManager sEngine;
		
		SampleSet mSampleNormal;
		SampleSet mSampleSoft;
		SampleSet* mSampleSets[3];
		
		//music
		static const uint32_t SIZE = 11025; //size of each HALF of the buffer
		int mChannel;

	private:
		AudioManager();
		~AudioManager() {}
};

#endif

