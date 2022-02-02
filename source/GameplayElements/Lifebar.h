#include <cstdio>

#include "System/GameClock.h"
#include "Graphics/SpriteContainer.h"
#include "Beatmaps/BeatmapManager.h"

#ifndef __LIFEBAR_H__
#define __LIFEBAR_H__

class Lifebar : public SpriteContainer
{
	public:
		Lifebar();
		void Initialize();
		
		void Update();
		void Set(float value, bool limit = true);
		void Increase(float value);
		void ClearTransforms();
		
		static const uint32_t MAXHP = mapXToScreen_noFloor(418); //matches with actual width of sprite
		static const uint32_t MAXCHANGE = mapXToScreen_noFloor(10);
		
		//values for increase
		static const uint32_t HP_300 = mapXToScreen_noFloor(12);
		static const uint32_t HP_100 = mapXToScreen_noFloor(5);
		static const uint32_t HP_50 = mapXToScreen_noFloor(1);
		static const uint32_t HP_GEKI = mapXToScreen_noFloor(28);
		static const uint32_t HP_KATSU = mapXToScreen_noFloor(20);
		static const uint32_t HP_SLIDER_REPEAT = mapXToScreen_noFloor(8);
		static const uint32_t HP_SLIDER_TICK = mapXToScreen_noFloor(6);
		static const uint32_t HP_SPINNER_SPIN = mapXToScreen_noFloor(4);
		static const uint32_t HP_SPINNER_BONUS = mapXToScreen_noFloor(5);
	
	protected:
		float mHpCurrent;
		float mHpDisplay;
		float mHpLossPerMs;
		int32_t mFillTime;
		float mFillRate;
		int32_t mTimeLastUpdate;
		
		uint32_t* mUV;
		
		void Bulge();
};

#endif

