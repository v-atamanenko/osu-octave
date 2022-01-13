#include "AudioManager.h"

AudioManager AudioManager::sEngine;

AudioManager::AudioManager()
{
	soundEnable();
	
	//sound init
	ResetSamples();
	
	mSampleSets[0] = NULL; //"none" sample set >.>
	mSampleSets[1] = &mSampleNormal;
	mSampleSets[2] = &mSampleSoft;
	
	//music init
	mChannel = -1;
	mBuffer = (u8*)new u32[SIZE*2/4+1]; //div4 because u32 = 4 * u8
	irqEnable(IRQ_TIMER0);
	irqSet(IRQ_TIMER0, MusicTimerHandler);
}

void AudioManager::ResetSamples()
{
	mSampleNormal.hitnormal.data = normal_hitnormal_bin;
	mSampleNormal.hitnormal.size = normal_hitnormal_bin_size;
	mSampleNormal.hitwhistle.data = normal_hitwhistle_bin;
	mSampleNormal.hitwhistle.size = normal_hitwhistle_bin_size;
	mSampleNormal.hitfinish.data = normal_hitfinish_bin;
	mSampleNormal.hitfinish.size = normal_hitfinish_bin_size;
	mSampleNormal.hitclap.data = normal_hitclap_bin;
	mSampleNormal.hitclap.size = normal_hitclap_bin_size;
	mSampleNormal.slidertick.data = normal_slidertick_bin;
	mSampleNormal.slidertick.size = normal_slidertick_bin_size;
	mSampleNormal.sliderslide.data = normal_sliderslide_bin;
	mSampleNormal.sliderslide.size = normal_sliderslide_bin_size;
	mSampleNormal.sliderwhistle.data = normal_sliderwhistle_bin;
	mSampleNormal.sliderwhistle.size = normal_sliderwhistle_bin_size;
	
	mSampleSoft.hitnormal.data = soft_hitnormal_bin;
	mSampleSoft.hitnormal.size = soft_hitnormal_bin_size;
	mSampleSoft.hitwhistle.data = soft_hitwhistle_bin;
	mSampleSoft.hitwhistle.size = soft_hitwhistle_bin_size;
	mSampleSoft.hitfinish.data = soft_hitfinish_bin;
	mSampleSoft.hitfinish.size = soft_hitfinish_bin_size;
	mSampleSoft.hitclap.data = soft_hitclap_bin;
	mSampleSoft.hitclap.size = soft_hitclap_bin_size;
	mSampleSoft.slidertick.data = soft_slidertick_bin;
	mSampleSoft.slidertick.size = soft_slidertick_bin_size;
	mSampleSoft.sliderslide.data = soft_sliderslide_bin;
	mSampleSoft.sliderslide.size = soft_sliderslide_bin_size;
	mSampleSoft.sliderwhistle.data = soft_sliderwhistle_bin;
	mSampleSoft.sliderwhistle.size = soft_sliderwhistle_bin_size;
}

int AudioManager::PlaySample(const u8* data, u32 size, bool loop)
{
	return soundPlaySample(data, SoundFormat_8Bit, size, 22050, 127, 64, loop, 0);
}

int AudioManager::PlaySample(SampleSetInfo info, bool loop)
{
	return soundPlaySample(info.data, SoundFormat_8Bit, info.size, 22050, 127, 64, loop, 0);
}

void AudioManager::SetChannelFreq(int channel, u16 freq)
{
	soundSetFreq(channel, freq);
}

void AudioManager::StopChannel(int channel)
{
	soundKill(channel);
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

void AudioManager::PlaySliderTick()
{
	SampleSet* current = mSampleSets[BeatmapElements::Element().GetTimingPoint().SampleSetId];
	
	PlaySample(current->slidertick);
}

//music
void MusicTimerHandler()
{
	AudioManager::Engine().fSwap = !AudioManager::Engine().fSwap;
	AudioManager::Engine().fFill = true;
}

int AudioManager::MusicPlay(std::string& filename)
{
	if (mChannel != -1)
		MusicStop();
	
	mFile = fopen(filename.c_str(), "rb");
	if (mFile == NULL) {
		return -1;
	}

	MusicBuffer();
	
	return mChannel;
}

int AudioManager::MusicSkipTo(u32 milliseconds)
{
	if (mChannel == -1)
		return -1;
	
	StopChannel(mChannel);
	TIMER0_CR = 0;
	fFill = false;
	
	fseek(mFile, milliseconds*22050/1000.0, SEEK_SET);
	MusicBuffer();
	return mChannel;
}

void AudioManager::MusicBuffer()
{
	fread(mBuffer, 1, SIZE, mFile);
	fFill = true;
	fSwap = false;
	fEof = 0;
	
	mChannel = soundPlaySample(mBuffer, SoundFormat_8Bit, SIZE*2, 22050, 127, 64, true, 0); //true indicates loop
	
	//TIMER_FREQ_1024(x) doesn't give required result
	mTimerData = -(((u32)(0x2000000*(SIZE/22050.0)))>>10);
	
	TIMER0_DATA = mTimerData;
	TIMER0_CR = TIMER_ENABLE | TIMER_IRQ_REQ | TIMER_DIV_1024;
}

void AudioManager::MusicStop()
{
	if (mChannel == -1)
		return;
	
	StopChannel(mChannel);
	TIMER0_CR = 0;
	fFill = false;
	
	fclose(mFile);
}

void AudioManager::MusicUpdate()
{
	if (fFill && mChannel != -1)
	{
		if (fEof > 0)
		{
			if (fEof == 2) //count 2 extra loops to allow buffered music to play
				MusicStop();
			
			++fEof;
			return;
		}
		
		u8* tBufferAddr = fSwap ? mBuffer : mBuffer+SIZE;
		
		fread(tBufferAddr, 1, SIZE, mFile);
		
		if (fEof == 0)
			fEof = feof(mFile);
		
		fFill = false;
		
		//iprintf("\x1b[23;20HVCOUNT:%i  ", REG_VCOUNT);
	}
}
