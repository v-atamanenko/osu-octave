#include <nds.h>
#include <stdio.h>
#include <string>

#include "Helpers/MathHelper.h"
#include "Beatmaps/DifficultyManager.h"

#ifndef __SCOREMANAGER_H__
#define __SCOREMANAGER_H__

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
		
		u32 CurrentScore()            { return mScore; }
		u32 CurrentCombo()            { return mCombo; }
		u32 MaxCombo()                { return mMaxCombo; }
		u32 Count300()                { return mCount300; }
		u32 Count100()                { return mCount100; }
		u32 Count50()                 { return mCount50; }
		u32 CountMiss()               { return mCountMiss; }
		u32 CountGeki()               { return mCountGeki; }
		u32 CountKatu()               { return mCountKatu; }
		std::string BeatmapChecksum() { return mBeatmapChecksum; }
	
	protected:
		//The Final score
		u32 mScore;
		//The Ending combo
		u32 mCombo;
		//The Maximum combo achieved
		u32 mMaxCombo;
		//Amount of 300s
		u32 mCount300;
		//Amount of 100s
		u32 mCount100;
		//Amount of 50s 
		u32 mCount50;
		//Amount of Misses
		u32 mCountMiss;
		//Amount of Gekis
		u32 mCountGeki;
		//Amount of Katus
		u32 mCountKatu;
		//Checksum of the Map this play belongs to
		std::string mBeatmapChecksum;
};

#endif

