#pragma once

#include <cstring>
#include <unistd.h>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

#include "BeatmapElements.h"

#include "System/GameClock.h"
#include "Helpers/MathHelper.h"
#include "Helpers/MD5.h"

#include "HitObjects/HitObject.h"
#include "HitObjects/HitCircle.h"
#include "HitObjects/HitSlider.h"
#include "HitObjects/HitSpinner.h"

#include "Graphics/SpriteManager.h"
#include "../lib/OsuParser/OsuParser.h"
#include "Helpers/OsuSliderCurves.h"

#include "DataStorage/Betmaps.h"


class Beatmap
{
	public:
		Beatmap(const std::string &filename, const std::string &basedir);
		virtual ~Beatmap();
		
		void Initialize();
		void CleanUp();

		void InitBG();
		
		void Buffer(std::list<HitObject*>& hitObjectList);
		[[nodiscard]] bool GameOver() const { return mHitObjectRead == mHitObjectCount && GameClock::Clock().Time() >= mLastObjectEndTime + 3000; }
		
		std::string& Filename() { return mFilename; }
        [[maybe_unused]] std::string& BackgroundFilename() { return mBackgroundFilename; }
		std::string& Title() { return mTitle; }
        [[maybe_unused]] std::string& Artist() { return mArtist; }
        [[maybe_unused]] std::string& Creator() { return mCreator; }
        [[maybe_unused]] std::string& Version() { return mVersion; }
        [[maybe_unused]] std::string& AudioFilename() { return mAudioFilename; }
        std::vector<BreakPoint>& Breakpoints() { return mBreakPoints; }

        [[maybe_unused]] std::string& BaseDir() { return mBaseDir; }

        // -1000 is a hack for beatmaps that for some reason start at about 0 ms.
        [[nodiscard]] OOTime AudioLeadIn() const { return (mAudioLeadIn > 0) ? (mAudioLeadIn * -1) : (-1000); }

		[[nodiscard]] OOTime SkipTime() const { return mSkipTime; }
        [[nodiscard]] OOTime StartTime() const { return mFirstObjectTime; }
        [[nodiscard]] OOTime EndTime() const { return mLastObjectEndTime; }
        [[nodiscard]] OOTime LengthPlayable() const { return (mLastObjectEndTime - mFirstObjectTime); }
        [[nodiscard]] OOTime BreakDurationTotal() const { return mBreakDurationTotal; }

        [[nodiscard]] OOInt HitObjectCount() const { return mHitObjectCount; }

		std::string& BeatmapChecksum();

        [[nodiscard]] bool Validate() const { return mValid; };
	
	protected:
        osuParser::OsuParser* mParser;
		
		OOInt mHitObjectCount, mHitObjectRead;
        OOTime mFirstObjectTime, mLastObjectEndTime;
        OOTime mBreakDurationTotal;

        // Stacking (experimental)
        bool mStackingEnabled;
        OOInt mStackSize = 0;
		
		void ReadNextObject();
        OOTime mNextObjectTime;
        OOInt mNextObjectX, mNextObjectY;
        OOInt mNextObjectNumberInCombo = 1;
        HitObjectSound mNextObjectSound;
		HitObjectType mNextObjectType;
        bool mForceNewCombo;

        OOTime mAudioLeadIn;
        OOTime mSkipTime;
		bool mNextObjectCombo;
		
		bool fReady, fLoadable;
        bool mValid = true;
		
		std::string mFilename;
		std::string mTitle;
		std::string mArtist;
		std::string mCreator;
		std::string mVersion;
		std::string mAudioFilename;
        std::string mBackgroundFilename;
        std::string mBackgroundFilepath;
		
		std::string mBaseDir;
        std::vector<BreakPoint> mBreakPoints;
	private:
		std::string mBeatmapChecksum;
};
