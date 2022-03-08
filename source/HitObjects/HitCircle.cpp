#include "HitCircle.h"

HitCircle::HitCircle(int32_t x, int32_t y, int32_t time, HitObjectType type, HitObjectSound sound, bool combo, int32_t number_in_combo) : HitObject(x, y, time, type, sound, combo, number_in_combo)
{
	uint32_t size = DifficultyManager::GetCircleSize();
	
	uint32_t preempt = DifficultyManager::GetPreemptTime();
	int32_t fadeInStart = (int32_t)time - (int32_t)preempt;
	int32_t fadeInEnd = fadeInStart + (preempt >> 3);
	mEndTime = time + DifficultyManager::GetHitWindow50();
	
	pSprite* spr;
	
	spr = new pSprite(TX_PLAY_CIRCLEOVERLAY, x, y, size, size, ORIGIN_CENTER, FIELD_PLAY, SDL_Color({0, 0, 0}), 0);
	spr->Show(fadeInStart, fadeInEnd);
	spr->Hide(time, mEndTime);
	spr->Kill(mEndTime+1000);
    spr->Z = (float)time;
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_CIRCLE, x, y, size, size, ORIGIN_CENTER, FIELD_PLAY, mColour, 0);
	spr->Show(fadeInStart, fadeInEnd);
	spr->Hide(time, mEndTime);
	spr->Kill(mEndTime+1000);
    spr->Z = (float)time - 0.5f;
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_CIRCLEAPPROACH, x, y, size, size, ORIGIN_CENTER, FIELD_PLAY, mColour, 0);
	spr->Show(fadeInStart, fadeInEnd);
	spr->Hide(time, mEndTime);
	spr->Scale(fadeInStart, time, 4, 1);
	spr->Kill(mEndTime+1000);
    spr->Z = (float)time;
	mSprites.push_back(spr);

    TextureType numbertex;
    switch (mNextObjectNumberInCombo) {
        case 1: numbertex = TX_PLAY_NUMBER_1; break;
        case 2: numbertex = TX_PLAY_NUMBER_2; break;
        case 3: numbertex = TX_PLAY_NUMBER_3; break;
        case 4: numbertex = TX_PLAY_NUMBER_4; break;
        case 5: numbertex = TX_PLAY_NUMBER_5; break;
        case 6: numbertex = TX_PLAY_NUMBER_6; break;
        case 7: numbertex = TX_PLAY_NUMBER_7; break;
        case 8: numbertex = TX_PLAY_NUMBER_8; break;
        case 9: numbertex = TX_PLAY_NUMBER_9; break;
        default: numbertex = TX_PLAY_NUMBER_0; break;
    }
    int h = (int)round((float)size*0.44f);
    int w = (int)round((float)h*(35.f/52.f));
    spr = new pSprite(numbertex, x, y, w, h, ORIGIN_CENTER, FIELD_PLAY, SDL_Color(), 0);
    spr->Show(fadeInStart, fadeInEnd);
    spr->Hide(time, mEndTime);
    spr->Kill(mEndTime+1000);
    spr->Z = (float)time-0.6f;
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
	long now = GameClock::Clock().Time();
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

