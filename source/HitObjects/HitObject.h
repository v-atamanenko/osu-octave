#pragma once

#include <cstdio>
#include "SDL.h"

#include "Beatmaps/BeatmapElements.h"
#include "Beatmaps/DifficultyManager.h"
#include "Graphics/pSprite.h"
#include "Graphics/pAnimation.h"
#include "Graphics/GfxInfo.h"
#include "Graphics/SpriteContainer.h"
#include "Helpers/AudioManager.h"
#include "System/ICallback.h"

typedef struct HitObjectPoint {
	OOInt x, y;
    OOInt angle = 0;
} HitObjectPoint;

typedef enum {
	HIT_NORMAL = 0,
	HIT_SLIDER = 1,
	HIT_SPINNER = 3
} HitObjectType;

//typedef void (*HitHandler)(ScoreType score, bool forceNoCombo, bool forceNoAnimation, HitObjectPoint point);

class HitObject : public SpriteContainer
{
	public:
		virtual ~HitObject() override {};
		
		virtual bool InBounds(OOInt x, OOInt y) { return true; }
		
		bool GetHit() { return mHit; }
        OOInt GetEndTime() { return mEndTime; }
		
		virtual void Update() {}
		
		virtual void OnTouchDown(const touchPosition& touch) {}
		virtual void OnTouch(const touchPosition& touch) {}
		virtual void OnTouchUp(const touchPosition& touch) {}
		
		virtual void Hit();
		
		void SetPostCreateOptions(bool comboend, OOInt nextx, OOInt nexty);
		
		static void SetScoreCallback(ICallback* scoreCallback);

        OOInt mX;
        OOInt mY;
    protected:
		HitObject(OOInt x, OOInt y, OOTime time, HitObjectType type, HitObjectSound sound, bool combo, OOInt number_in_combo);

        OOTime mTime, mEndTime;
		HitObjectType mType;
		HitObjectSound mSound;
		static SDL_Color mColour;

		bool mHit;
		bool mComboEnd;
        OOInt mNextObjectNumberInCombo;

		OOUInt mScoreSpriteId;
		
		void IncreaseScore(ScoreType score, bool forceNoCombo = false, bool forceNoAnimation = false);
		void IncreaseScore(ScoreType score, bool forceNoCombo, bool forceNoAnimation, OOUInt spriteId);
	
	private:
		OOFloat mScoreDeltaZ;
		
		static OOFloat sScoreDeltaZ;
		static OOFloat sSliderDeltaZ;
		static OOInt sLastSliderTime;
		
		static ICallback* mScoreCallback;
};
