#ifndef __DIFFICULTYMANAGER_H__
#define __DIFFICULTYMANAGER_H__

class DifficultyManager
{
	public:
		static uint8_t DifficultyHpDrain;
		static uint8_t DifficultyCircleSize;
		static uint8_t DifficultyOverall;
		static float SliderMultiplier;
		static float SliderTickRate;
		static float DifficultyHpDrainRate;
		static uint8_t DifficultyPeppyStars;
		static float DifficultyEyupStars;
		
		//inline
		static uint32_t GetCircleSize()		{ return circleSize[DifficultyCircleSize]; } //possibly *1.2
		static uint32_t GetPreemptTime()		{ return preemptTime[DifficultyOverall]; }
		static uint32_t GetHitWindow300()	{ return hitWindow300[DifficultyOverall]; }
		static uint32_t GetHitWindow100()	{ return hitWindow100[DifficultyOverall]; }
		static uint32_t GetHitWindow50()		{ return hitWindow50[DifficultyOverall]; }
		static uint32_t GetHitWindow()		{ return hitWindow50[DifficultyOverall] << 1; }
		static uint32_t GetSpinnerTime()		{ return spinnerTime[DifficultyOverall]; }
		static int32_t GetMissHpDrain()		{ return missHpDrain[DifficultyHpDrain]; }
	
	protected:
		static uint32_t preemptTime[];
		static uint32_t circleSize[];
		static uint32_t hitWindow300[];
		static uint32_t hitWindow100[];
		static uint32_t hitWindow50[];
		static uint32_t spinnerTime[];
		static int32_t missHpDrain[];
};

#endif

