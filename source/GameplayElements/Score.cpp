#include "Score.h"
#include "Beatmaps/BeatmapManager.h"
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

void Score::Increase(float base_value) {
    // Score v1 calculation (c) McKay42/McOsu
    const unsigned long breakTimeMS = BeatmapManager::Current().BreakDurationTotal();
    const unsigned long lengthPlayable = BeatmapManager::Current().LengthPlayable();
    const unsigned long drainLength = std::max(lengthPlayable - std::min(breakTimeMS, lengthPlayable), (unsigned long)1000) / 1000;

    const int scoreComboMultiplier = std::max((int)mCombo-1, 0); // current combo, excluding the current hitobject which caused the addHitResult() call
    const int difficultyMultiplier = (int)std::round((DifficultyManager::GetCircleSize() + (float)DifficultyManager::DifficultyHpDrain + DifficultyManager::DifficultyOverall + std::clamp<float>((float)BeatmapManager::Current().HitObjectCount() / (float)drainLength * 8.0f, 0.0f, 16.0f)) / 38.0f * 5.0f);

    mScore += floor((float)base_value + ((base_value * ((float)scoreComboMultiplier * (float)difficultyMultiplier * mScoreMultiplier)) / 25));
}

void Score::Add(ScoreType score, bool forceNoCombo, bool gekiKatu) {
    int32_t score_increase = 0;
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
		if (score == SCORE_SPIN_100) score_increase += 100;
		else score_increase += score;
        Increase((float)score_increase);
        return;
	}

    // Special case: SCORE_TICK_10 / SCORE_TICK_30
	if (score == SCORE_TICK_30 || score == SCORE_TICK_10) {
		score_increase += score;

		if (!forceNoCombo) {
			++mCombo;

			// Increase max combo if mCombo is greater than it
			if(mCombo > mMaxCombo)
				mMaxCombo = mCombo;
		}

        Increase((float)score_increase);
        return;
	}

    // Common case: SCORE_50, SCORE_100, SCORE_300
    if (!forceNoCombo) {
        ++mCombo;

        // Increase max combo if mCombo is greater than it
        if(mCombo > mMaxCombo)
            mMaxCombo = mCombo;
    }

    Increase((float)score);

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
    if ( mCount300 + mCount100 + mCount50 + mCountMiss == 0 ) {
        return 100.0f;
    }

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