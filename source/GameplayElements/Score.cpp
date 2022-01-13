#include "Score.h"

Score::Score()
{
	mScore = 0;
	mCombo = 0;
	mScore = 0; 
	mCombo = 0; 
	mMaxCombo = 0; 
	mCount300 = 0; 
	mCount100 = 0; 
	mCount50 = 0; 
	mCountMiss = 0; 
	mCountGeki = 0; 
	mCountKatu = 0; 
	mBeatmapChecksum = "";
}

void Score::Add(ScoreType score, bool forceNoCombo, bool gekiKatu)
{
	if (score == SCORE_MISS)
	{
		mCombo = 0;

		this->mCountMiss++;
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

		if (!forceNoCombo) {
			++mCombo;

			//Increase max combo if mCombo is greater than it
			if(mCombo > mMaxCombo)
				mMaxCombo = mCombo;
		}
	}
	else
	{
		mScore += score + MathHelper::Max(0, mCombo-1) * (score/25) * DifficultyManager::DifficultyPeppyStars;

		if (!forceNoCombo) {
			++mCombo;

			//Increase max combo if mCombo is greater than it
			if(mCombo > mMaxCombo)
				mMaxCombo = mCombo;
		}

		//Increase the respective count
		switch(score) {
			case ScoreType::SCORE_100:
				if(gekiKatu)
					this->mCountKatu++;
				else
					this->mCount100++;
				break;
			case ScoreType::SCORE_300:
				if(gekiKatu)
					this->mCountGeki++;
				else
					this->mCount300++;
				break;
			case ScoreType::SCORE_50:
				this->mCount50++;
				break;
		}
	}
}


