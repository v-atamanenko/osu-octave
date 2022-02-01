#include "HitSpinner.h"

#ifndef TEXTURE_PACK
#define TEXTURE_PACK(u, v) (((u) & 0xFFFF) | ((v) << 16))
#endif

HitSpinner::HitSpinner(int32_t time, int32_t endtime, HitObjectSound sound) : HitObject(mapXToScreen(256), mapYToScreen(192), time, HIT_SPINNER, sound)
{
	mEndTime = endtime;
	fSpinning = false;
	
	mDirection = 0; //0 = not set, values -1 and 1 used
	mZeroPoint = 0; //point to start counting rotations from
	mTotalRotation = 0; //counts rotations in current direction (resets)
	mCurrentRotation = 0; //keeps track of which rotation we are at to count
	mTotalSpins = 0; //counts total number of spins
	mRequiredSpins = (mEndTime - mTime) / DifficultyManager::GetSpinnerTime(); //total spins required
	
	mUV = new uint32_t[4]; //deleted by pSprite
	mUV[0] = TEXTURE_PACK(0,0);
	mUV[1] = TEXTURE_PACK(mapXToScreen(256),0);
	mUV[2] = TEXTURE_PACK(mapXToScreen(256), mapYToScreen(192));
	mUV[3] = TEXTURE_PACK(0, mapYToScreen(192));
	
	pSprite* spr;
	
	spr = new pSprite(TX_PLAY_CIRCLEAPPROACH, mapXToScreen(256), mapYToScreen(192), mapYToScreen(440), mapYToScreen(440), ORIGIN_CENTER, FIELD_PLAY, SDL_Color({10, 10, 31}), 0);
	spr->Show(time-300, time);
	spr->Hide(endtime, endtime+300);
	spr->Scale(time-300, endtime, 1, 0);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SPINNER, mapXToScreen(256), mapYToScreen(192), mapYToScreen(400), mapYToScreen(400), ORIGIN_CENTER, FIELD_PLAY, SDL_Color({31, 31, 31}), 0, 0.03f);
	spr->Show(time-300, time);
	spr->Hide(endtime, endtime+300);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SPINNERBARS, 0, mapYToScreen(480), mapXToScreen(640), mapYToScreen(480), ORIGIN_BOTTOMLEFT, FIELD_SCREEN, SDL_Color({31, 31, 31}), 0, 0.03f);
	spr->Show(time-300, time);
	spr->Hide(endtime, endtime+300);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SPINNERBG, mapXToScreen(320), mapYToScreen(240), mapXToScreen(640), mapYToScreen(480), ORIGIN_CENTER, FIELD_SCREEN, SDL_Color({31, 31, 31}), 0, 0.05f);
	spr->Show(time-300, time+300);
	spr->Hide(endtime, endtime+300);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);
	
	mScoreSpriteId = 1;
	
	mSprites[2]->UV = mUV;
	mChannel = -1;
}

HitSpinner::~HitSpinner()
{
	if (mChannel != -1)
			AudioManager::Engine().StopChannel(mChannel);
}

void HitSpinner::Update()
{
	float ratio = (mTotalSpins + MathHelper::Frc(mTotalRotation)) / mRequiredSpins;
	
	//set spinner bars
	uint32_t height = MathHelper::Max(0, MathHelper::Min(mapYToScreen(192), ratio*mapYToScreen(192)) - MathHelper::Random(0,10));
	
	mUV[0] = TEXTURE_PACK(0,mapYToScreen(192)-height);
	mUV[1] = TEXTURE_PACK(mapXToScreen(256),mapYToScreen(192)-height);
	mSprites[2]->Height = height*2.5;
	
	//set spinner sound
	if (mChannel == -1 && GameClock::Clock().Time() >= mTime)
		mChannel = AudioManager::Engine().PlaySpinnerSound(SND_NORMAL);
	
	if (mChannel != -1)
		//AudioManager::Engine().SetChannelFreq(mChannel, MathHelper::Min((uint32_t)(10000 + (ratio * 30000)), 65535));
	
	//trigger score calculation once slider is finished
	if (GameClock::Clock().Time() >= mEndTime)
		Hit();
}

void HitSpinner::OnTouchDown(const touchPosition& touch)
{
	if (GameClock::Clock().Time() >= mTime && GameClock::Clock().Time() <= mEndTime)
	{
		mLastAngle = GetAngle(touch.px, touch.py);
		fSpinning = true;
	}
}

void HitSpinner::OnTouch(const touchPosition& touch)
{
	if (GameClock::Clock().Time() >= mTime && GameClock::Clock().Time() <= mEndTime)
	{
		if (MathHelper::Abs(touch.px - 320) < 10 && MathHelper::Abs(touch.py - 265) < 10)
		{
			fSpinning = false;
			return;
		}
		
		//work out the new angle
		int32_t newAngle = GetAngle(touch.px, touch.py);
		
		if (!fSpinning)
		{
			mLastAngle = newAngle;
			fSpinning = true;
			return;
		}
		
		if (newAngle == mLastAngle)
			return;
		
		int32_t deltaAngle = newAngle - mLastAngle;
		
		//hack for passing through line x=320 where y<265
		if (deltaAngle > 16384)
			deltaAngle -= 32768;
		else if (deltaAngle < -16384)
			deltaAngle += 32768;
		
		mSprites[1]->Angle += deltaAngle;
		
		//if player changes direction add to total rotations and start count again
		if (mDirection != MathHelper::Sgn(deltaAngle))
		{
			mDirection = MathHelper::Sgn(deltaAngle);
			mZeroPoint = mSprites[1]->Angle;
			mTotalRotation = 0;
			mCurrentRotation = 0;
		}
		
		mTotalRotation = MathHelper::Abs(mSprites[1]->Angle - mZeroPoint) / 32768.0;
		
		//if we have made an extra circle (or more) add to total
		if (mCurrentRotation < (uint32_t)mTotalRotation)
		{
			mTotalSpins += (uint32_t)mTotalRotation - mCurrentRotation;
			mCurrentRotation = (uint32_t)mTotalRotation;
			
			IncreaseScore(SCORE_SPIN_100, true, true);
			if (mTotalSpins > mRequiredSpins)
			{
				IncreaseScore(SCORE_SPIN_1000, true, true);
				AudioManager::Engine().PlaySpinnerSound(SND_BONUS);
			}
		}
		
		mLastAngle = newAngle;
	}
}

void HitSpinner::Hit()
{
	if (mRequiredSpins > 0 && (mTotalSpins == 0 || mTotalSpins < MathHelper::Max(0, (int32_t)(mRequiredSpins - 2))))
	{
		IncreaseScore(SCORE_MISS);
	}
	else
	{
		//compatibility with osu!pc
		if (mRequiredSpins == 0)
		{
			IncreaseScore(SCORE_50, false, true); //score sprite doesn't show if you score 50 on spinners
		}
		else if (mTotalSpins >= mRequiredSpins)
		{
			IncreaseScore(SCORE_300);
		}
		else if (mTotalSpins == MathHelper::Max(0, (int32_t)(mRequiredSpins - 1)))
		{
			IncreaseScore(SCORE_100);
		}
		else if (mTotalSpins == MathHelper::Max(0, (int32_t)(mRequiredSpins - 2)))
		{
			IncreaseScore(SCORE_50, false, true); //score sprite doesn't show if you score 50 on spinners
		}
		
		AudioManager::Engine().PlayHitSound(mSound);
	}
	
	if (mChannel != -1)
		AudioManager::Engine().StopChannel(mChannel);
	
	mHit = true;
}

int32_t HitSpinner::GetAngle(int32_t x, int32_t y)
{
	float theta = atan((float)(y-265)/(x-320));
	int32_t angle = theta*32768/6.2832;
	
	//hack - let's hope this won't come back to haunt me
	if (x < 320)
		angle += 16384;
	
	return angle;
}