#include <nds.h>
#include <vector>

#include "Helpers/MathHelper.h"
#include "System/GameClock.h"

#ifndef __BEATMAPELEMENTS_H__
#define __BEATMAPELEMENTS_H__



typedef struct {
	s32 Time;
	float BeatTime;
	u8 SampleSetId;
} TimingPoint;

typedef struct {
	s32 StartTime;
	s32 EndTime;
} BreakPoint;

class BeatmapElements
{
	public:
		static BeatmapElements& Element() { return sInstance; }
		
		const TimingPoint& GetTimingPoint(s32 time);
		const TimingPoint& GetTimingPoint();
		
		const bool IsBreak();
		
		rgb GetNextColour();
		
		void AddTimingPoint(s32 time, float beattime, u8 samplesetid);
		void AddBreakPoint(s32 start, s32 end);
		void ResetColours(bool fill);
	
	protected:
		static BeatmapElements sInstance;
		
		std::vector<TimingPoint> mTimingPoints;
		std::vector<BreakPoint> mBreakPoints;
		std::vector<rgb> mColours;
		u32 mCurrentColour;
	
	private:
		BeatmapElements() {}
		~BeatmapElements() {}
};

#endif

