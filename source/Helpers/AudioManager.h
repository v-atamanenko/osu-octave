#include <nds.h>
#include <stdio.h>
#include <string>

#include "Beatmaps/BeatmapElements.h"

#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

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



typedef enum {
	SND_NORMAL = 1,
	SND_WHISTLE = 2,
	SND_FINISH = 4,
	SND_CLAP = 8
} HitObjectSound;

typedef struct {
	const u8* data;
	u32 size;
} SampleSetInfo;

typedef struct {
	SampleSetInfo hitnormal;
	SampleSetInfo hitwhistle;
	SampleSetInfo hitfinish;
	SampleSetInfo hitclap;
	SampleSetInfo slidertick;
	SampleSetInfo sliderslide;
	SampleSetInfo sliderwhistle;
} SampleSet;

//intended usage:
//AudioManager::Engine().PlaySample(SOUND_DATA(sound_name), loop)
#define SOUND_DATA(name) name, name##_size

class AudioManager
{
	public:
		static AudioManager& Engine() { return sEngine; }
		
		int PlaySample(const u8* data, u32 size, bool loop = false);
		int PlaySample(SampleSetInfo info, bool loop = false);
		void SetChannelFreq(int channel, u16 freq);
		void StopChannel(int channel);
		
		//sounds
		void ResetSamples();
		void PlayHitSound(HitObjectSound sound);
		int PlaySliderSound(HitObjectSound sound);
		void PlaySliderTick();
		
		//music
		friend void MusicTimerHandler();
		int MusicPlay(std::string& filename);
		int MusicSkipTo(u32 milliseconds);
		void MusicStop();
		void MusicUpdate(); //must be called frequently
	
	protected:
		static AudioManager sEngine;
		
		void MusicBuffer();
		
		SampleSet mSampleNormal;
		SampleSet mSampleSoft;
		SampleSet* mSampleSets[3];
		
		//music
		static const u32 SIZE = 11025; //size of each HALF of the buffer
		u8* mBuffer;
		FILE* mFile;
		bool fSwap, fFill;
		u32 fEof;
		int mChannel;
		u16 mTimerData;
	
	private:
		AudioManager();
		~AudioManager() {}
};

void MusicTimerHandler();

#endif

