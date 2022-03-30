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
#include "Parser/OsuParser.h"
#include "Helpers/OsuSliderCurves.h"

#include "DataStorage/Betmaps.h"

#include "../lib/osu_sr_calculator_cpp/osu_sr_calculator/osu_sr_calculator.h"

#ifndef __BEATMAP_H__
#define __BEATMAP_H__

//typedef list<HitObject*>::iterator hitObjectIterator;

class Beatmap
{
	public:
		Beatmap(const std::string &filename, const std::string &basedir);
		virtual ~Beatmap();
		
		void Initialize();
		void CleanUp();

		void InitBG();
		
		void Buffer(std::list<HitObject*>& hitObjectList);
		bool GameOver() { return mHitObjectRead == mHitObjectCount && GameClock::Clock().Time() >= mLastObjectEndTime + 3000; }
		
		std::string& Filename() { return mFilename; }
        std::string& BackgroundFilename() { return mBackgroundFilename; }
		std::string& Title() { return mTitle; }
		std::string& Artist() { return mArtist; }
		std::string& Creator() { return mCreator; }
		std::string& Version() { return mVersion; }
		std::string& AudioFilename() { return mAudioFilename; }
        std::vector<BreakPoint>& Breakpoints() { return mBreakPoints; }
		
		std::string& BaseDir() { return mBaseDir; }

        // -1000 is a hack for beatmaps that for some reason start at 0 ms.
        [[nodiscard]] int32_t AudioLeadIn() const { return (mAudioLeadIn > 0) ? (mAudioLeadIn * -1) : (-1000); }

		int32_t SkipTime() { return mSkipTime; }
        int32_t StartTime() { return mFirstObjectTime; }
        int32_t EndTime() { return mLastObjectEndTime; }

        [[nodiscard]] long BreakDurationTotal() const
        {
            long breakDurationTotal = 0;

            for (auto &bp : mBreakPoints) {
                breakDurationTotal += (bp.EndTime - bp.StartTime);
            }

            return breakDurationTotal;
        }

        [[nodiscard]] long LengthPlayable() const
        {
            if (mParser->hitObjects.empty()) { return 0; }
            osuParser::HitObject last_ho = mParser->hitObjects[mParser->hitObjects.size()-1];
            long last_ho_dur = 0;
            if (last_ho.type == osuParser::HitObjectType::oSlider) {
                last_ho_dur = last_ho.slider.duration;
            } else if (last_ho.type == osuParser::HitObjectType::oSpinner) {
                last_ho_dur = last_ho.spinner.duration;
            }
            return (last_ho.time + last_ho_dur) - mParser->hitObjects[0].time;
        }

        uint32_t HitObjectCount() const {
            return mHitObjectCount;
        }

		std::string& BeatmapChecksum();

        bool Validate() { return mValid; };
        static bool LoadEntryData(const std::string &filename, const std::string &basedir, BeatmapEntry& bm);
	
	protected:
        osuParser::OsuParser* mParser;
		
		uint32_t mHitObjectCount, mHitObjectRead;
        int32_t mFirstObjectTime, mLastObjectEndTime;

        // Stacking (experimental)
        bool mStackingEnabled;
        int mStackSize = 0;
		
		void ReadNextObject();
        int32_t mNextObjectTime;
        int32_t mNextObjectX, mNextObjectY;
        int32_t mNextObjectNumberInCombo = 1;
        HitObjectSound mNextObjectSound;
		HitObjectType mNextObjectType;
        bool mForceNewCombo;

        int32_t mAudioLeadIn;
        int32_t mSkipTime;
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

#endif

