#include "Lifebar.h"

Lifebar::Lifebar()
{
	pSprite* spr;
	
	spr = new pSprite(TX_PLAY_SCOREBAR, 0, 0, 640, 42, ORIGIN_TOPLEFT, FIELD_SCREEN, RGB15(31,31,31), 31);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SCOREBAR_BAR, 0, 0, 400, 40, ORIGIN_TOPLEFT, FIELD_SCREEN, RGB15(31,31,31), 31, -0.01f);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SCOREBAR_KI, 400, 18, 80, 80, ORIGIN_CENTER, FIELD_SCREEN, RGB15(31,31,31), 31, -0.02f);
	mSprites.push_back(spr);
	
	mUV = new u32[4]; //deleted by pSprite
	mUV[0] = TEXTURE_PACK(inttot16(0),inttot16(0));
	mUV[1] = TEXTURE_PACK(inttot16(160),inttot16(0));
	mUV[2] = TEXTURE_PACK(inttot16(160),inttot16(16));
	mUV[3] = TEXTURE_PACK(inttot16(0),inttot16(16));
	
	mSprites[1]->UV = mUV;
}

//can only be called after beatmap has been loaded
void Lifebar::Initialize()
{
	mHpLossPerMs = DifficultyManager::DifficultyHpDrainRate * 2; //units are hp/ms based off a 200 point scale
	mHpCurrent = 0;
	mHpDisplay = 0;

	mTimeLastUpdate = GameClock::Clock().Time();
	mFillTime = MathHelper::Min(10000, BeatmapManager::Current().StartTime());
	mFillRate = MAXHP/((mFillTime-700)/(float)1000*60);

	for (u32 time = BeatmapManager::Current().StartTime() - mFillTime;
	     time < MathHelper::Max(BeatmapManager::Current().StartTime(), 701) - 700; time += 150)
	{
		mSprites[2]->Scale(time, time + 90, 1.5, 1);
	}
}

void Lifebar::Update()
{
	s32 now = GameClock::Clock().Time();
	s32 startTime = BeatmapManager::Current().StartTime();
	
	if (now > startTime && BeatmapElements::Element().IsBreak() == false)
	{
		mHpCurrent -= (now - mTimeLastUpdate) * mHpLossPerMs;
		if (mHpCurrent < 0)
			mHpCurrent = 0;
	}
	//intro animation
	else if (now > startTime - mFillTime && now < startTime)
	{
		Set(mHpCurrent + mFillRate, false);
	}
	
	int dhp = mHpCurrent - mHpDisplay;
	if (MathHelper::Abs(dhp) > MAXCHANGE)
		dhp = MathHelper::Sgn(dhp) * MAXCHANGE;
	mHpDisplay += dhp;
	
	//mHpDisplay represents the required width of the sprite
	mSprites[2]->Move(mHpDisplay, 18);
	if (mHpDisplay >= 200)
		mSprites[2]->Texture = TX_PLAY_SCOREBAR_KI;
	else if (mHpDisplay >= 70)
		mSprites[2]->Texture = TX_PLAY_SCOREBAR_KIDANGER;
	else
		mSprites[2]->Texture = TX_PLAY_SCOREBAR_KIDANGER2;
	
	mSprites[1]->Width = (u32)mHpDisplay;
	mUV[1] = TEXTURE_PACK(inttot16((u32)(mHpDisplay/2.5)),inttot16(0));
	mUV[2] = TEXTURE_PACK(inttot16((u32)(mHpDisplay/2.5)),inttot16(16));
	
	mTimeLastUpdate = now;
}

void Lifebar::Set(float value, bool limit)
{
	if (value < 0) value = 0;
	if (value > MAXHP) value = MAXHP;
	
	mHpCurrent = value;
	if (!limit)
		mHpDisplay = value;
	mTimeLastUpdate = GameClock::Clock().Time();
}

void Lifebar::Increase(float value)
{
	Set(mHpCurrent + value);
	
	//animate ki icon :D
	if (value > 0)
		Bulge();
}

void Lifebar::Bulge()
{
	s32 now = GameClock::Clock().Time();
	
	mSprites[2]->Scale(now, now+90, 1.5, 1);
}

void Lifebar::ClearTransforms()
{
	for (spriteIterator it = mSprites.begin(); it != mSprites.end(); ++it)
	{
		(*it)->ClearTransforms();
	}
}

