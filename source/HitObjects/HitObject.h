#include <cstdio>
#include "SDL.h"

#include "Beatmaps/BeatmapElements.h"
#include "Beatmaps/DifficultyManager.h"
#include "GameplayElements/Score.h"
#include "Graphics/pSprite.h"
#include "Graphics/pAnimation.h"
#include "Graphics/GfxInfo.h"
#include "Graphics/SpriteContainer.h"
#include "Helpers/AudioManager.h"
#include "System/ICallback.h"

#ifndef __HITOBJECT_H__
#define __HITOBJECT_H__

typedef struct {
	int32_t x, y;
	int32_t angle;
} HitObjectPoint;

typedef enum {
	HIT_NORMAL = 1,
	HIT_SLIDER = 2,
	HIT_COMBO = 4,
	HIT_SPINNER = 8
} HitObjectType;

typedef void (*HitHandler)(ScoreType score, bool forceNoCombo, bool forceNoAnimation, HitObjectPoint point);

class HitObject : public SpriteContainer
{
	public:
		virtual ~HitObject();
		
		virtual bool InBounds(int32_t x, int32_t y) { return true; }
		
		bool GetHit() { return mHit; }
		int32_t GetEndTime() { return mEndTime; }
		
		virtual void Update() {}
		
		virtual void OnTouchDown(const touchPosition& touch) {}
		virtual void OnTouch(const touchPosition& touch) {}
		virtual void OnTouchUp(const touchPosition& touch) {}
		
		virtual void Hit();
		
		void SetPostCreateOptions(bool comboend, int32_t nextx, int32_t nexty);
		
		static void SetScoreCallback(ICallback* scoreCallback);
	
	protected:
		HitObject(int32_t x, int32_t y, int32_t time, HitObjectType type, HitObjectSound sound);
		
		int32_t mX, mY;
		int32_t mTime, mEndTime;
		HitObjectType mType;
		HitObjectSound mSound;
		static SDL_Color mColour;
		
		bool mHit;
		bool mComboEnd;
		
		uint32_t mScoreSpriteId;
		
		void IncreaseScore(ScoreType score, bool forceNoCombo = false, bool forceNoAnimation = false);
		void IncreaseScore(ScoreType score, bool forceNoCombo, bool forceNoAnimation, uint32_t spriteId);
	
	private:
		float mScoreDeltaZ;
		
		static float sScoreDeltaZ;
		static float sSliderDeltaZ;
		static int32_t sLastSliderTime;
		
		static ICallback* mScoreCallback;
};

#endif

