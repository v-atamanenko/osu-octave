#include "HitSpinner.h"

#ifndef TEXTURE_PACK
#define TEXTURE_PACK(u, v) (((u) & 0xFFFF) | ((v) << 16))
#endif

HitSpinner::HitSpinner(int32_t time, int32_t endtime, HitObjectSound sound, bool combo) : HitObject(osuPixelsXtoScreenX(256), osuPixelsYtoScreenY(192), time, HIT_SPINNER, sound, combo)
{
	mEndTime = endtime;
	fSpinning = false;
	
	mDirection = 0; //0 = not set, values -1 and 1 used
	mZeroPoint = 0; //point to start counting rotations from
	mTotalRotation = 0; //counts rotations in current direction (resets)
	mCurrentRotation = 0; //keeps track of which rotation we are at to count
	mTotalSpins = 0; //counts total number of spins
	mRequiredSpins = (mEndTime - mTime) / DifficultyManager::GetSpinnerTime(); //total spins required
	
	//mUV = SDL_Rect({0, 0, 256, 192});

	pSprite* spr;
	
	spr = new pSprite(TX_PLAY_CIRCLEAPPROACH, 479, 302, 460, 460, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color({0, 0, 0}), 0);
	spr->Show(time-300, time);
	spr->Hide(endtime, endtime+300);
	spr->Scale(time-300, endtime, 1, 0);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SPINNER, 479, 302, 440, 440, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color({0, 0, 0}), 0, 0.03f);
	spr->Show(time-300, time);
	spr->Hide(endtime, endtime+300);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SPINNERBARS, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_BOTTOMLEFT, FIELD_SCREEN, SDL_Color({0, 0, 0}), 0, 0.03f);
	spr->Show(time-300, time);
	spr->Hide(endtime, endtime+300);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SPINNERBG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color({0, 0, 0}), 0, 0.05f);
	spr->Show(time-300, time+300);
	spr->Hide(endtime, endtime+300);
	spr->Kill(endtime+300);
	mSprites.push_back(spr);
	
	mScoreSpriteId = 1;

    delete mSprites[2]->UV;
    mSprites[2]->UV = new SDL_Rect({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});

    mChannel = -1;
    mChannelBonus = -1;
}

HitSpinner::~HitSpinner()
{
	if (mChannel != -1)
        AudioManager::Engine().StopChannel(mChannel);
    if (mChannelBonus != -1)
        AudioManager::Engine().StopChannel(mChannelBonus);
}

void HitSpinner::Update()
{
	float ratio = ((float)mTotalSpins + MathHelper::Frc(mTotalRotation)) / (float)mRequiredSpins;
	
	//set spinner bars
	uint32_t height = MathHelper::Max(0, MathHelper::Min(SCREEN_HEIGHT, (int32_t)floor(ratio*(float)(SCREEN_HEIGHT))) - (int32_t)MathHelper::Random(0, ScreenY(20)));

    delete mSprites[2]->UV;
    mSprites[2]->UV = new SDL_Rect({0, SCREEN_HEIGHT-height, SCREEN_WIDTH, height});

    //set spinner sound
	if (mChannel == -1 && GameClock::Clock().Time() >= mTime)
		mChannel = AudioManager::Engine().PlaySpinnerSound(SND_NORMAL);
	
	//if (mChannel != -1)
        //TODO: changing freq
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
		if (MathHelper::Abs((int32_t)touch.px - (SCREEN_WIDTH / 2)) < 10 && MathHelper::Abs((int32_t)touch.py - (SCREEN_HEIGHT/2)) < 10)
		{
			fSpinning = false;
			return;
		}
		
		//work out the new angle
		int32_t newAngle = GetAngle((int32_t)touch.px, (int32_t)touch.py);
		
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
				mChannelBonus = AudioManager::Engine().PlaySpinnerSound(SND_BONUS);
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

    if (mChannelBonus != -1)
        AudioManager::Engine().StopChannel(mChannelBonus);
	
	mHit = true;
}

int32_t HitSpinner::GetAngle(int32_t x, int32_t y)
{
	float theta = atan((float)((float)y-(SCREEN_HEIGHT/2.f))/((float)x-(SCREEN_WIDTH/2.f)));
	int32_t angle = floor(theta*32768/6.2832);
	
	//hack - let's hope this won't come back to haunt me
	if (x < floor(SCREEN_WIDTH/2.f))
		angle += 16384;

	return angle;
}