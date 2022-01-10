#include <nds.h>
#include <stdio.h>
#include "Helpers/MathHelper.h"
#include "Beatmaps/DifficultyManager.h"

#ifndef __SCOREMANAGER_H__
#define __SCOREMANAGER_H__

typedef enum {
	SCORE_300 = 300,
	SCORE_100 = 100,
	SCORE_50 = 50,
	SCORE_TICK_30 = 30,
	SCORE_TICK_10 = 10,
	SCORE_SPIN_100 = 101,//should be 100 but conflicts with SCORE_100
	SCORE_SPIN_1000 = 1000,
	SCORE_MISS = 0
} ScoreType;

class Score
{
	public:
		Score();
		
		void Add(ScoreType score, bool forceNoCombo = false);
		
		u32 CurrentScore() { return mScore; }
		u32 CurrentCombo() { return mCombo; }
	
	protected:
		u32 mScore;
		u32 mCombo;
};

#endif

