#include "Lifebar.h"

Lifebar::Lifebar() {
	pSprite* spr;
	
	spr = new pSprite(TX_PLAY_SCOREBAR, 0, 0, 960, 60, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color({0, 0, 0}), 255);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SCOREBAR_BAR, 0, 0, 960, 60, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color({0, 0, 0}), 255, -0.01f);
	mSprites.push_back(spr);
	
	spr = new pSprite(TX_PLAY_SCOREBAR_KI, 640, 28, 36, 36, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color({0, 0, 0}), 255, -0.02f);
	mSprites.push_back(spr);

    delete mSprites[1]->UV;
    mSprites[1]->UV = new SDL_Rect({0, 0, 960, 60});
}

// Can only be called after beatmap has been loaded
void Lifebar::Initialize() {
	mHpLossPerMs = DifficultyManager::GetHpDrainRate() * 2; //units are hp/ms based off a 200 point scale
	mHpCurrent = 0;
	mHpDisplay = 0;

	mTimeLastUpdate = GameClock::Clock().Time();
	mFillTime = MathHelper::Min(10000, (BeatmapManager::Current().StartTime() - BeatmapManager::Current().AudioLeadIn()));
	mFillRate = (OOFloat)MAXHP / ( (OOFloat)(mFillTime-700) / (OOFloat)(1000*60) );

	for (OOTime time = BeatmapManager::Current().StartTime() - mFillTime;
	     time < MathHelper::Max(BeatmapManager::Current().StartTime(), 701) - 700; time += 150)
	{
		mSprites[2]->Scale(time, time + 90, 1.5, 1);
	}
}

void Lifebar::Update()
{
    OOTime now = GameClock::Clock().Time();
    OOTime startTime = BeatmapManager::Current().StartTime();
	
	if (now > startTime && !BeatmapElements::Element().IsBreak()) {
		mHpCurrent -= (now - mTimeLastUpdate) * mHpLossPerMs;
		if (mHpCurrent < 0)
			mHpCurrent = 0;
	}
	// intro animation
	else if ((now > (startTime - mFillTime)) && (now < startTime))
	{
		Set(mHpCurrent + mFillRate, false);
	}
	
	OOFloat dhp = mHpCurrent - mHpDisplay;
	//if (MathHelper::Abs(dhp) > MAXCHANGE)
	//	dhp = MathHelper::Sgn(dhp) * MAXCHANGE;
	mHpDisplay += dhp;
	
	//mHpDisplay represents the required width of the sprite
	mSprites[2]->Move(floor(mHpDisplay), 28);
	if (mHpDisplay >= 345)
		mSprites[2]->Texture = TX_PLAY_SCOREBAR_KI;
	else if (mHpDisplay >= 172)
		mSprites[2]->Texture = TX_PLAY_SCOREBAR_KIDANGER;
	else
		mSprites[2]->Texture = TX_PLAY_SCOREBAR_KIDANGER2;

    delete mSprites[1]->UV;
    mSprites[1]->UV = new SDL_Rect({0, 0, floor(mHpDisplay), 60});

	mTimeLastUpdate = now;
}

void Lifebar::Set(OOFloat value, bool limit)
{
	if (value < 0) value = 0;
	if (value > MAXHP) value = MAXHP;
	
	mHpCurrent = value;
	if (!limit)
		mHpDisplay = value;
	mTimeLastUpdate = GameClock::Clock().Time();
}

void Lifebar::Increase(OOFloat value)
{
	Set(mHpCurrent + value);
	
	//animate ki icon :D
	if (value > 0)
		Bulge();
}

void Lifebar::Bulge()
{
	OOTime now = GameClock::Clock().Time();
	mSprites[2]->Scale(now, now+90, 1.5, 1);
}

void Lifebar::ClearTransforms() {
	for (auto & mSprite : mSprites) {
		mSprite->ClearTransforms();
	}
}

void Lifebar::Kill() {
    OOTime mt = GameClock::Clock().Time();
    for (auto &spr : mSprites) {
        spr->Kill(mt);
    }
}
