#include "Score.h"
#include <cstring>

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

void Score::Add(ScoreType score, bool forceNoCombo, bool gekiKatu) {
    // Special case: SCORE_MISS
	if (score == SCORE_MISS) {
		mCombo = 0;
		this->mCountMiss++;
        return;
	}

    // Special case: SCORE_COMBOBREAK
    if (score == SCORE_COMBOBREAK) {
        mCombo = 0;
        return;
    }

    // Special case: SCORE_SPIN_100 / SCORE_SPIN_1000
	if (score == SCORE_SPIN_100 || score == SCORE_SPIN_1000) {
		if (score == SCORE_SPIN_100) mScore += 100;
		else mScore += score;
        return;
	}

    // Special case: SCORE_TICK_10 / SCORE_TICK_30
	if (score == SCORE_TICK_30 || score == SCORE_TICK_10) {
		mScore += score;

		if (!forceNoCombo) {
			++mCombo;

			// Increase max combo if mCombo is greater than it
			if(mCombo > mMaxCombo)
				mMaxCombo = mCombo;
		}
        return;
	}

    // Common case: SCORE_50, SCORE_100, SCORE_300
    mScore += score + MathHelper::Max(0, (int32_t)mCombo-1) * (score/25) * DifficultyManager::DifficultyPeppyStars;

    if (!forceNoCombo) {
        ++mCombo;

        // Increase max combo if mCombo is greater than it
        if(mCombo > mMaxCombo)
            mMaxCombo = mCombo;
    }

    //Increase the respective count
    switch(score) {
        case ScoreType::SCORE_50:
            this->mCount50++;
            break;
        case ScoreType::SCORE_100:
            if (gekiKatu) this->mCountKatu++;
            else this->mCount100++;
            break;
        case ScoreType::SCORE_300:
            if (gekiKatu) this->mCountGeki++;
            else this->mCount300++;
            break;
        default:
            fprintf(stderr, "[WARNING] Unexpected score type.\n");
    }
}

float Score::CountAccuracy() const {
    return ((float)((300*(mCount300+mCountGeki)) + (100*(mCount100+mCountKatu)) + (50*mCount50)) /
           (float)(300*(mCount300+mCountGeki+mCount100+mCountKatu+mCount50+mCountMiss)))*100;
}

void Score::CalculateGrade() {
    strncpy(mGrade, "D", GRADE_LEN);

    if (CountAccuracy() == 100.f) {
        strncpy(mGrade, "SS", GRADE_LEN);
        return;
    }

    uint32_t total_count = mCount300 + mCount100 + mCount50 + mCountMiss + mCountGeki + mCountKatu;
    float p300 = (((float)mCount300+(float)mCountGeki) / (float)total_count) * 100;
    float p50 = ((float)mCount50 / (float)total_count) * 100;
    if (p300 > 90.f && p50 < 1.f && mCountMiss == 0) {
        strncpy(mGrade, "S", GRADE_LEN);
        return;
    }

    if ((p300 > 80.f && mCountMiss == 0) || p300 > 90.f) {
        strncpy(mGrade, "A", GRADE_LEN);
        return;
    }

    if ((p300 > 70.f && mCountMiss == 0) || p300 > 80.f) {
        strncpy(mGrade, "B", GRADE_LEN);
        return;
    }

    if (p300 > 60.f) {
        strncpy(mGrade, "C", GRADE_LEN);
        return;
    }
}