#pragma once

#include <cstdio>

#include "SDL.h"
#include "System/GameClock.h"
#include "Graphics/SpriteContainer.h"
#include "Beatmaps/BeatmapManager.h"

class Lifebar : public SpriteContainer
{
	public:
		Lifebar();
		void Initialize();
		
		void Update();
		void Set(OOFloat value, bool limit = true);
		void Increase(OOFloat value);
		void ClearTransforms();
        [[nodiscard]] OOFloat GetCurrentHP() const { return mHpCurrent; }
        void Kill();
		
		static const OOUInt MAXHP = 662; //matches with actual width of sprite
		static const OOUInt MAXCHANGE = 10;
		
		// Values for increase
		static const OOUInt HP_300 = 12;
		static const OOUInt HP_100 = 5;
		static const OOUInt HP_50 = 1;
		static const OOUInt HP_GEKI = 28;
		static const OOUInt HP_KATSU = 20;
		static const OOUInt HP_SLIDER_REPEAT = 8;
		static const OOUInt HP_SLIDER_TICK = 6;
		static const OOUInt HP_SPINNER_SPIN = 4;
		static const OOUInt HP_SPINNER_BONUS = 5;
	
	protected:
		OOFloat mHpCurrent;
        OOFloat mHpDisplay;
        OOFloat mHpLossPerMs;
		OOTime mFillTime;
		OOFloat mFillRate;
		OOTime mTimeLastUpdate;
		
		void Bulge();
};
