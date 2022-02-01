#include "Lifebar.h"
#include "SDL.h"

#ifndef TEXTURE_PACK
#define TEXTURE_PACK(u, v) (((u) & 0xFFFF) | ((v) << 16))
#endif

Lifebar::Lifebar()
{
	pSprite* spr;
	
	spr = new pSprite(TX_PLAY_SCOREBAR, 0, 0, mapXToScreen(640), mapYToScreen(42), ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color({31, 31, 31}), 31);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SCOREBAR_BAR, 0, 0, mapXToScreen(400), mapYToScreen(40), ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color({31, 31, 31}), 31, -0.01f);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SCOREBAR_KI, mapXToScreen(400), mapYToScreen(18), mapXToScreen(80), mapXToScreen(80), ORIGIN_CENTER, FIELD_SCREEN, SDL_Color({31, 31, 31}), 31, -0.02f);
	mSprites.push_back(spr);
	
	mUV = new uint32_t[4]; //deleted by pSprite
	mUV[0] = TEXTURE_PACK(0, 0);
	mUV[1] = TEXTURE_PACK(mapXToScreen(160), 0);
	mUV[2] = TEXTURE_PACK(mapXToScreen(160), mapYToScreen(16));
	mUV[3] = TEXTURE_PACK(0, mapYToScreen(16));
	
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

	for (uint32_t time = BeatmapManager::Current().StartTime() - mFillTime;
	     time < MathHelper::Max(BeatmapManager::Current().StartTime(), 701) - 700; time += 150)
	{
		mSprites[2]->Scale(time, time + 90, 1.5, 1);
	}
}

void Lifebar::Update()
{
	int32_t now = GameClock::Clock().Time();
	int32_t startTime = BeatmapManager::Current().StartTime();
	
	if (now > startTime && !BeatmapElements::Element().IsBreak())
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
	
	mSprites[1]->Width = (uint32_t)mHpDisplay;
	mUV[1] = TEXTURE_PACK((uint32_t)(mHpDisplay/2.5),0);
	mUV[2] = TEXTURE_PACK((uint32_t)(mHpDisplay/2.5),16);
	
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
	int32_t now = GameClock::Clock().Time();
	
	mSprites[2]->Scale(now, now+90, 1.5, 1);
}

void Lifebar::ClearTransforms()
{
	for (auto & mSprite : mSprites)
	{
		mSprite->ClearTransforms();
	}
}
