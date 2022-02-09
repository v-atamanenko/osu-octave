#include <cstring>
#include <unistd.h>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

#include "BeatmapElements.h"

#include "System/GameClock.h"
#include "Helpers/FileReader.h"
#include "Helpers/MathHelper.h"

#include "HitObjects/HitObject.h"
#include "HitObjects/HitCircle.h"
#include "HitObjects/HitSlider.h"
#include "HitObjects/HitSpinner.h"

#include "Graphics/SpriteManager.h"
#include "Parser/OsuParser.h"
#include "Helpers/OsuSliderCurves.h"

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

		void InitBG();
		
		void Buffer(std::list<HitObject*>& hitObjectList);
		bool GameOver() { return mHitObjectRead == mHitObjectCount && GameClock::Clock().Time() >= mLastObjectEndTime + 3000; }
		
		std::string& Filename() { return mFilename; }
		std::string& Title() { return mTitle; }
		std::string& Artist() { return mArtist; }
		std::string& Creator() { return mCreator; }
		std::string& Version() { return mVersion; }
		std::string& AudioFilename() { return mAudioFilename; }
		
		std::string& BaseDir() { return mBaseDir; }
		
		int32_t SkipTime() { return mSkipTime; }
        int32_t StartTime() { return mFirstObjectTime; }

		std::string& BeatmapChecksum();
	
	protected:
        osuParser::OsuParser* mParser;
		
		uint32_t mHitObjectCount, mHitObjectRead;
        int32_t mFirstObjectTime, mLastObjectEndTime;
		
		void ReadNextObject();
        int32_t mNextObjectTime;
        int32_t mNextObjectX, mNextObjectY;
        HitObjectSound mNextObjectSound;
		HitObjectType mNextObjectType;
        bool mNextObjectIsNewCombo;

        int32_t mSkipTime;
		bool mForceNewCombo;
		
		bool fReady, fLoadable;
		
		std::string mFilename;
		std::string mTitle;
		std::string mArtist;
		std::string mCreator;
		std::string mVersion;
		std::string mAudioFilename;
        std::string mBackgroundFilename;
		
		std::string mBaseDir;
	private:
		std::string mChecksumString;
		std::string mBeatmapChecksum;
};

#endif

