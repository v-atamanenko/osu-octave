#pragma once

#include "SDL.h"
#include <vector>

#include "Helpers/MathHelper.h"
#include "System/GameClock.h"

typedef struct {
    OOTime Time;
    OOFloat BeatTime;
	OOUShort SampleSetId;
} TimingPoint;

typedef struct {
    OOTime StartTime;
    OOTime EndTime;
} BreakPoint;

class BeatmapElements
{
	public:
		static BeatmapElements& Element() { return sInstance; }
		
		const TimingPoint& GetTimingPoint(OOTime time);
		const TimingPoint& GetTimingPoint();
		
		bool IsBreak();

        SDL_Color GetNextColour();
		
		void AddTimingPoint(OOTime time, OOFloat beattime, OOUShort samplesetid);
		void AddBreakPoint(OOTime start, OOTime end);
        void AddColor(SDL_Color c);
		void ResetColours(bool fill);
	
	protected:
		static BeatmapElements sInstance;
		
		std::vector<TimingPoint> mTimingPoints;
		std::vector<BreakPoint> mBreakPoints;
		std::vector<SDL_Color> mColours;
		OOInt mCurrentColour = 0;
	
	private:
		BeatmapElements() = default;
		~BeatmapElements() = default;
};
