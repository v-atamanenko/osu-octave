#include "SDL.h"
#include <vector>

#include "Helpers/MathHelper.h"
#include "System/GameClock.h"

#ifndef __BEATMAPELEMENTS_H__
#define __BEATMAPELEMENTS_H__

typedef struct {
    long Time;
	float BeatTime;
	uint8_t SampleSetId;
} TimingPoint;

typedef struct {
    long StartTime;
    long EndTime;
} BreakPoint;

class BeatmapElements
{
	public:
		static BeatmapElements& Element() { return sInstance; }
		
		const TimingPoint& GetTimingPoint(long time);
		const TimingPoint& GetTimingPoint();
		
		bool IsBreak();

        SDL_Color GetNextColour();
		
		void AddTimingPoint(long time, float beattime, uint8_t samplesetid);
		void AddBreakPoint(long start, long end);
        void AddColor(SDL_Color c);
		void ResetColours(bool fill);
	
	protected:
		static BeatmapElements sInstance;
		
		std::vector<TimingPoint> mTimingPoints;
		std::vector<BreakPoint> mBreakPoints;
		std::vector<SDL_Color> mColours;
		uint32_t mCurrentColour;
	
	private:
		BeatmapElements() {}
		~BeatmapElements() {}
};

#endif
