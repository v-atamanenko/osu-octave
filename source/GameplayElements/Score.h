#include <cstdio>
#include <string>

#include "Helpers/MathHelper.h"
#include "Beatmaps/DifficultyManager.h"

#ifndef __SCOREMANAGER_H__
#define __SCOREMANAGER_H__

#ifndef GRADE_LEN
#define GRADE_LEN 3
#endif

typedef enum {
	SCORE_300 = 300,
	SCORE_GEKI = 301,
	SCORE_100 = 100,
	SCORE_KATU = 102,
	SCORE_50 = 50,
	SCORE_TICK_30 = 30,
	SCORE_TICK_10 = 10,
	SCORE_SPIN_100 = 101, //should be 100 but conflicts with SCORE_100
	SCORE_SPIN_1000 = 1000,
	SCORE_MISS = 0
} ScoreType;

class Score
{
	public:
		Score();
		
		void Add(ScoreType score, bool forceNoCombo = false, bool gekiKatu = false);
		
		uint32_t CurrentScore()            { return mScore; }
		uint32_t CurrentCombo()            { return mCombo; }
		uint32_t MaxCombo()                { return mMaxCombo; }
		uint32_t Count300()                { return mCount300; }
		uint32_t Count100()                { return mCount100; }
		uint32_t Count50()                 { return mCount50; }
		uint32_t CountMiss()               { return mCountMiss; }
		uint32_t CountGeki()               { return mCountGeki; }
		uint32_t CountKatu()               { return mCountKatu; }
        float CountAccuracy();
        char * GetGrade()                  { return mGrade; };
        void CalculateGrade();
		std::string BeatmapChecksum() { return mBeatmapChecksum; }
	
	protected:
		//The Final score
		uint32_t mScore;
		//The Ending combo
		uint32_t mCombo;
        //The Final grade letter
        char mGrade[GRADE_LEN];
		//The Maximum combo achieved
		uint32_t mMaxCombo;
		//Amount of 300s
		uint32_t mCount300;
		//Amount of 100s
		uint32_t mCount100;
		//Amount of 50s 
		uint32_t mCount50;
		//Amount of Misses
		uint32_t mCountMiss;
		//Amount of Gekis
		uint32_t mCountGeki;
		//Amount of Katus
		uint32_t mCountKatu;
		//Checksum of the Map this play belongs to
		std::string mBeatmapChecksum;
};

#endif

