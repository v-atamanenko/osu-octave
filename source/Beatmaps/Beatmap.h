#include <nds.h>
#include <string.h>
#include <unistd.h>
#include <list>
#include <vector>
#include <string>

#include "BeatmapElements.h"

#include "System/GameClock.h"
#include "Helpers/FileReader.h"
#include "Helpers/MathHelper.h"

#include "HitObjects/HitObject.h"
#include "HitObjects/HitCircle.h"
#include "HitObjects/HitSlider.h"
#include "HitObjects/HitSpinner.h"

#include "Graphics/SpriteManager.h"

#ifndef __BEATMAP_H__
#define __BEATMAP_H__



//typedef list<HitObject*>::iterator hitObjectIterator;

class Beatmap
{
	public:
		Beatmap(const char* filename, const char* basedir);
		virtual ~Beatmap();
		
		void Initialize();
		void CleanUp();
		
		void Buffer(std::list<HitObject*>& hitObjectList);
		bool GameOver() { return mHitObjectRead == mHitObjectCount && GameClock::Clock().Time() >= mLastObjectEndTime + 3000; }
		
		std::string& Filename() { return mFilename; }
		std::string& Title() { return mTitle; }
		std::string& Artist() { return mArtist; }
		std::string& Creator() { return mCreator; }
		std::string& Version() { return mVersion; }
		std::string& AudioFilename() { return mAudioFilename; }
		
		std::string& BaseDir() { return mBaseDir; }
		
		s32 SkipTime() { return mSkipTime; }
		s32 StartTime() { return mFirstObjectTime; }

		std::string& BeatmapChecksum();
	
	protected:
		FileReader* mReader;
		
		u32 mHitObjectCount, mHitObjectRead;
		s32 mFirstObjectTime, mLastObjectEndTime;
		
		void ReadNextObject();
		s32 mNextObjectTime;
		s32 mNextObjectX, mNextObjectY;
		HitObjectType mNextObjectType;
		
		s32 mSkipTime;
		bool mForceNewCombo;
		
		bool fReady, fLoadable;
		
		std::string mFilename;
		std::string mTitle;
		std::string mArtist;
		std::string mCreator;
		std::string mVersion;
		std::string mAudioFilename;
		
		std::string mBaseDir;
	private:
		std::string mChecksumString;
		std::string mBeatmapChecksum;
};

#endif

