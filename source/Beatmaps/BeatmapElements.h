#include "SDL.h"
#include <vector>

#include "Helpers/MathHelper.h"
#include "System/GameClock.h"

#ifndef __BEATMAPELEMENTS_H__
#define __BEATMAPELEMENTS_H__

typedef struct {
	int32_t Time;
	float BeatTime;
	uint8_t SampleSetId;
} TimingPoint;

typedef struct {
	int32_t StartTime;
	int32_t EndTime;
} BreakPoint;

class BeatmapElements
{
	public:
		static BeatmapElements& Element() { return sInstance; }
		
		const TimingPoint& GetTimingPoint(int32_t time);
		const TimingPoint& GetTimingPoint();
		
		const bool IsBreak();

        SDL_Color GetNextColour();
		
		void AddTimingPoint(int32_t time, float beattime, uint8_t samplesetid);
		void AddBreakPoint(int32_t start, int32_t end);
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
