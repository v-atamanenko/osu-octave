#include "Score.h"

Score::Score()
{
	mScore = 0;
	mCombo = 0;
}

void Score::Add(ScoreType score, bool forceNoCombo)
{
	if (score == SCORE_MISS)
	{
		mCombo = 0;
	}
	else if (score == SCORE_SPIN_100 || score == SCORE_SPIN_1000)
	{
		if (score == SCORE_SPIN_100)
			mScore += 100;
		else
			mScore += score;
	}
	else if (score == SCORE_TICK_30 || score == SCORE_TICK_10)
	{
		mScore += score;
		if (!forceNoCombo)
			++mCombo;
	}
	else
	{
		mScore += score + MathHelper::Max(0, mCombo-1) * (score/25) * DifficultyManager::DifficultyPeppyStars;
		if (!forceNoCombo)
			++mCombo;
	}
}


