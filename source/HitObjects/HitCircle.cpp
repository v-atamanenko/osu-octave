#include "HitCircle.h"

HitCircle::HitCircle(int32_t x, int32_t y, int32_t time, HitObjectType type, HitObjectSound sound) : HitObject(x, y, time, type, sound)
{
	uint32_t size = DifficultyManager::GetCircleSize();
	
	uint32_t preempt = DifficultyManager::GetPreemptTime();
	int32_t fadeInStart = time - preempt;
	int32_t fadeInEnd = fadeInStart + (preempt >> 3);
	mEndTime = time + DifficultyManager::GetHitWindow50();
	
	pSprite* spr;
	
	spr = new pSprite(TX_PLAY_CIRCLEOVERLAY, x, y, size, size, ORIGIN_CENTER, FIELD_PLAY, SDL_Color({0, 0, 0}), 0);
	spr->Show(fadeInStart, fadeInEnd);
	spr->Hide(time, mEndTime);
	spr->Kill(mEndTime+1000);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_CIRCLE, x, y, size, size, ORIGIN_CENTER, FIELD_PLAY, mColour, 0);
	spr->Show(fadeInStart, fadeInEnd);
	spr->Hide(time, mEndTime);
	spr->Kill(mEndTime+1000);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_CIRCLEAPPROACH, x, y, size, size, ORIGIN_CENTER, FIELD_PLAY, mColour, 0);
	spr->Show(fadeInStart, fadeInEnd);
	spr->Hide(time, mEndTime);
	spr->Scale(fadeInStart, time, 4, 1);
	spr->Kill(mEndTime+1000);
	mSprites.push_back(spr);
	
	mScoreSpriteId = 1;
}

bool HitCircle::InBounds(int32_t x, int32_t y)
{
	//all sprites are the same, it doesn't matter which one
	return mSprites[1]->InBounds(x, y);
}

void HitCircle::OnTouchDown(const touchPosition& touch)
{
	if (InBounds(touch.px, touch.py))
	{
		Hit();
	}
}

void HitCircle::Hit()
{
	int32_t now = GameClock::Clock().Time();
	uint32_t delta = MathHelper::Abs(mTime - now);
	
	if (delta > DifficultyManager::GetHitWindow())
	{
		//too early, give the hitcircle a shake
		for (auto spr : mSprites)
		{
				spr->Move(now, now+20, mX+5, mY);
			spr->Move(now+20, now+40, mX-5, mY);
			spr->Move(now+40, now+60, mX+5, mY);
			spr->Move(now+60, now+80, mX, mY);
		}
	}
	else
	{
		if (delta < DifficultyManager::GetHitWindow50())
		{
			//if within the window for 50, the person hit it
			for (uint32_t i=0; i<2; ++i)
			{
				pDrawable* spr = mSprites[i];
				
				//circle explosion
				spr->Transform(TR_FADE, now, now+200, 31, 10);
				spr->Transform(TR_FADE, now+200, now+270, 10, 0);
				spr->Scale(now, now+150, 1, 1.7);
				spr->Scale(now+150, now+270, 1.7, 1.9);
			}
			//kill the approach circle
			mSprites[2]->Kill(now);
			
			if (delta < DifficultyManager::GetHitWindow300())
			{
				IncreaseScore(SCORE_300);
			}
			else if (delta < DifficultyManager::GetHitWindow100())
			{
				IncreaseScore(SCORE_100);
			}
			else
			{
				IncreaseScore(SCORE_50);
			}
			
			AudioManager::Engine().PlayHitSound(mSound);
		}
		else
		{
			//otherwise missed
			for (uint32_t i=0; i<3; ++i)
			{
				mSprites[i]->Kill(now);
			}
			
			IncreaseScore(SCORE_MISS);
		}
	}
	
	mHit = true;
}

