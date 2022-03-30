#pragma once

#include <vector>
#include "HitObject.h"
#include "Graphics/pSprite.h"

typedef struct {
	pSprite* Tick;
	pSprite* Score;
} TickSprites;

class HitSlider : public HitObject
{
	public:
		HitSlider(OOInt x, OOInt y, OOTime time, OOTime lengthtime, std::vector<HitObjectPoint*>& points, std::vector<HitObjectPoint*>& ticks, OOUInt repeats, HitObjectType type, HitObjectSound sound, bool combo, OOInt number_in_combo);
		~HitSlider();
		
		bool InBounds(OOInt x, OOInt y);
		
		void Update();
		
		void OnTouchDown(const touchPosition& touch);
		void OnTouch(const touchPosition& touch);
		void OnTouchUp(const touchPosition& touch);
		
		void Hit();

        HitObjectPoint mEndPoint;
	
	protected:
		static void MapSliderPath(pSprite* spr, std::vector<HitObjectPoint*>& points, OOTime time, OOTime lengthtime, OOUInt repeats);
		
		bool fTouching, fStarted, fFinished;
        bool mCreatedFadeoutTransform = false;
		TickSprites* mTicks;
		OOTime * mTickTimes;
		OOTime mTickTime, mLengthTime;
		
		OOUInt mTickCount, mTicksHit, mTicksTarget;
		OOUInt mRepeats, mRepeatCurrent;
		OOTime mTimeLast;
		
		OOUInt mSecondaryScoreSpriteId;
		
		OOInt mChannel;
};
