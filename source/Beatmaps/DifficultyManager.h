#ifndef _H__DIFFICULTYMANAGER_H_
#define _H__DIFFICULTYMANAGER_H_

class DifficultyManager
{
	public:
		static uint8_t DifficultyHpDrain;
		static uint8_t DifficultyCircleSize;
		static uint8_t DifficultyOverall;
		static float SliderMultiplier;
		static float SliderTickRate;

		static uint8_t DifficultyPeppyStars;

		//inline
		static float GetCircleSize()   { return circle_size_new; }
		static long GetPreemptTime()  { return preempt_time_new; }
		static long GetHitWindow300() { return hit_window_300; }
		static long GetHitWindow100() { return hit_window_100; }
		static long GetHitWindow50()  { return hit_window_50; }
		static long GetHitWindow()    { return hit_window; }
		static float GetSpinnerRPS()  { return spinner_rps; }
		static float  GetMissHpDrain()  { return missHpDrain[DifficultyHpDrain]; }
		static float    GetHpDrainRate()  { return 1.f / (110.f + missHpDrain[DifficultyHpDrain]); }

        static long preempt_time_new;
        static float circle_size_new;
        static float spinner_rps;
        static long fadeInMs;
        static long hit_window_300;
        static long hit_window_100;
        static long hit_window_50;
        static long hit_window;
        static long spinner_time;
        static long miss_hp_drain;
	protected:


		static uint32_t preemptTime[];
		static uint32_t circleSize[];
		static uint32_t hitWindow300[];
		static uint32_t hitWindow100[];
		static uint32_t hitWindow50[];
		static uint32_t spinnerTime[];
		static float missHpDrain[];
};

#endif // _H__DIFFICULTYMANAGER_H_
