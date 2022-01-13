#include <nds.h>
#include <stdio.h>

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
		
		static const u32 MAXHP = 400; //matches with actual width of sprite
		static const u32 MAXCHANGE = 10;
		
		//values for increase
		static const u32 HP_300 = 12;
		static const u32 HP_100 = 5;
		static const u32 HP_50 = 1;
		static const u32 HP_GEKI = 28;
		static const u32 HP_KATSU = 20;
		static const u32 HP_SLIDER_REPEAT = 8;
		static const u32 HP_SLIDER_TICK = 6;
		static const u32 HP_SPINNER_SPIN = 4;
		static const u32 HP_SPINNER_BONUS = 5;
	
	protected:
		float mHpCurrent;
		float mHpDisplay;
		float mHpLossPerMs;
		s32 mFillTime;
		float mFillRate;
		s32 mTimeLastUpdate;
		
		u32* mUV;
		
		void Bulge();
};

#endif

