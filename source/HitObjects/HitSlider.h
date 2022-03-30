#include <vector>
#include "HitObject.h"

#ifndef __HITSLIDER_H__
#define __HITSLIDER_H__

typedef std::vector<HitObjectPoint*>::iterator pointIterator;

typedef struct {
	pSprite* Tick;
	pSprite* Score;
} TickSprites;

class HitSlider : public HitObject
{
	public:
		HitSlider(int32_t x, int32_t y, int32_t time, uint32_t lengthtime, std::vector<HitObjectPoint*>& points, std::vector<HitObjectPoint*>& ticks, uint32_t repeats, HitObjectType type, HitObjectSound sound, bool combo, int32_t number_in_combo);
		~HitSlider();
		
		bool InBounds(int32_t x, int32_t y);
		
		void Update();
		
		void OnTouchDown(const touchPosition& touch);
		void OnTouch(const touchPosition& touch);
		void OnTouchUp(const touchPosition& touch);
		
		void Hit();

        HitObjectPoint mEndPoint;
	
	protected:
		static void MapSliderPath(pSprite* spr, std::vector<HitObjectPoint*>& points, int32_t time, uint32_t lengthtime, uint32_t repeats);
		
		bool fTouching, fStarted, fFinished;
		TickSprites* mTicks;
		int32_t* mTickTimes;
		uint32_t mTickTime, mLengthTime;
		
		uint32_t mTickCount, mTicksHit, mTicksTarget;
		uint32_t mRepeats, mRepeatCurrent;
		int32_t mTimeLast;
		
		uint32_t mSecondaryScoreSpriteId;
		
		int mChannel;
};

#endif

