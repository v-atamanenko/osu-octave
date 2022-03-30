#pragma once

#include <cstdio>
#include <cstring>
#include <string>

#include "Helpers/MathHelper.h"
#include "Beatmaps/DifficultyManager.h"
#include "Beatmaps/BeatmapManager.h"

#ifndef GRADE_LEN
#define GRADE_LEN 3
#endif

class Score
{
	public:
		Score();
		
		void Add(ScoreType score, bool forceNoCombo = false, bool gekiKatu = false);
		
		OOUInt CurrentScore() const            { return mScore; }
        std::string CurrentScoreFormatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%08u", mScore);
            std::string str(fc_buffer);
            return str;
        }
		OOUInt CurrentCombo() const            { return mCombo; }
        std::string CurrentComboFormatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCombo);
            std::string str(fc_buffer);
            return str;
        }
		OOUInt MaxCombo() const                { return mMaxCombo; }
		OOUInt Count300() const                { return mCount300; }
        std::string Count300Formatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCount300);
            std::string str(fc_buffer);
            return str;
        }
		OOUInt Count100() const                { return mCount100; }
        std::string Count100Formatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCount100);
            std::string str(fc_buffer);
            return str;
        }
		OOUInt Count50() const                 { return mCount50; }
        std::string Count50Formatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCount50);
            std::string str(fc_buffer);
            return str;
        }
		OOUInt CountMiss() const               { return mCountMiss; }
        std::string CountMissFormatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCountMiss);
            std::string str(fc_buffer);
            return str;
        }
		OOUInt CountGeki() const               { return mCountGeki; }
        std::string CountGekiFormatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCountGeki);
            std::string str(fc_buffer);
            return str;
        }
		OOUInt CountKatu() const               { return mCountKatu; }
        std::string CountKatuFormatted() const{
            unsigned int fc_buffer_size = 32;
            char fc_buffer[fc_buffer_size];
            snprintf(fc_buffer, fc_buffer_size, "%ix", mCountKatu);
            std::string str(fc_buffer);
            return str;
        }
        OOFloat CountAccuracy() const;
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
		OOUInt mScore;
        //Score multiplier, can change for different mods
        OOFloat mScoreMultiplier = 1.0f;
		//The Ending combo
		OOUInt mCombo;
        //The Final grade letter
        char mGrade[GRADE_LEN];
		//The Maximum combo achieved
		OOUInt mMaxCombo;
		//Amount of 300s
		OOUInt mCount300 = 0;
		//Amount of 100s
		OOUInt mCount100 = 0;
		//Amount of 50s 
		OOUInt mCount50 = 0;
		//Amount of Misses
		OOUInt mCountMiss = 0;
		//Amount of Gekis
		OOUInt mCountGeki;
		//Amount of Katus
		OOUInt mCountKatu;
		//Checksum of the Map this play belongs to
		std::string mBeatmapChecksum;

        void Increase(OOFloat base_value);
};
