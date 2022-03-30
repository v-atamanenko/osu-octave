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
	SCORE_MISS = 0,
    SCORE_COMBOBREAK
} ScoreType;

class Score
{
	public:
		Score();
		
		void Add(ScoreType score, bool forceNoCombo = false, bool gekiKatu = false);
		
		uint32_t CurrentScore() const            { return mScore; }
        std::string CurrentScoreFormatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%08u", mScore);
            std::string str(fc_buffer);
            return str;
        }
		uint32_t CurrentCombo() const            { return mCombo; }
        std::string CurrentComboFormatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCombo);
            std::string str(fc_buffer);
            return str;
        }
		uint32_t MaxCombo() const                { return mMaxCombo; }
		uint32_t Count300() const                { return mCount300; }
        std::string Count300Formatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCount300);
            std::string str(fc_buffer);
            return str;
        }
		uint32_t Count100() const                { return mCount100; }
        std::string Count100Formatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCount100);
            std::string str(fc_buffer);
            return str;
        }
		uint32_t Count50() const                 { return mCount50; }
        std::string Count50Formatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCount50);
            std::string str(fc_buffer);
            return str;
        }
		uint32_t CountMiss() const               { return mCountMiss; }
        std::string CountMissFormatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCountMiss);
            std::string str(fc_buffer);
            return str;
        }
		uint32_t CountGeki() const               { return mCountGeki; }
        std::string CountGekiFormatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCountGeki);
            std::string str(fc_buffer);
            return str;
        }
		uint32_t CountKatu() const               { return mCountKatu; }
        std::string CountKatuFormatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCountKatu);
            std::string str(fc_buffer);
            return str;
        }
        float CountAccuracy() const;
        std::string CountAccuracyFormatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%.2f%%", CountAccuracy());
            std::string str(fc_buffer);
            return str;
        }
        char * GetGrade()                  { return mGrade; };
        void CalculateGrade();
		std::string BeatmapChecksum() const { return mBeatmapChecksum; }
	
	protected:
		//The Final score
		uint32_t mScore;
        //Score multiplier, can change for different mods
        float mScoreMultiplier = 1.0f;
		//The Ending combo
		uint32_t mCombo;
        //The Final grade letter
        char mGrade[GRADE_LEN];
		//The Maximum combo achieved
		uint32_t mMaxCombo;
		//Amount of 300s
		uint32_t mCount300 = 0;
		//Amount of 100s
		uint32_t mCount100 = 0;
		//Amount of 50s 
		uint32_t mCount50 = 0;
		//Amount of Misses
		uint32_t mCountMiss = 0;
		//Amount of Gekis
		uint32_t mCountGeki;
		//Amount of Katus
		uint32_t mCountKatu;
		//Checksum of the Map this play belongs to
		std::string mBeatmapChecksum;

    void Increase(float base_value);
};

#endif

