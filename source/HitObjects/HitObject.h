#include <nds.h>
#include <stdio.h>

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
	s32 x, y;
	s32 angle;
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
		
		virtual bool InBounds(s32 x, s32 y) { return true; }
		
		bool GetHit() { return mHit; }
		s32 GetEndTime() { return mEndTime; }
		
		virtual void Update() {}
		
		virtual void OnTouchDown(const touchPosition& touch) {}
		virtual void OnTouch(const touchPosition& touch) {}
		virtual void OnTouchUp(const touchPosition& touch) {}
		
		virtual void Hit();
		
		void SetPostCreateOptions(bool comboend, s32 nextx, s32 nexty);
		
		static void SetScoreCallback(ICallback* scoreCallback);
	
	protected:
		HitObject(s32 x, s32 y, s32 time, HitObjectType type, HitObjectSound sound);
		
		s32 mX, mY;
		s32 mTime, mEndTime;
		HitObjectType mType;
		HitObjectSound mSound;
		static rgb mColour;
		
		bool mHit;
		bool mComboEnd;
		
		u32 mScoreSpriteId;
		
		void IncreaseScore(ScoreType score, bool forceNoCombo = false, bool forceNoAnimation = false);
		void IncreaseScore(ScoreType score, bool forceNoCombo, bool forceNoAnimation, u32 spriteId);
	
	private:
		float mScoreDeltaZ;
		
		static float sScoreDeltaZ;
		static float sSliderDeltaZ;
		static s32 sLastSliderTime;
		
		static ICallback* mScoreCallback;
};

#endif


