#include "DarkenOverlay.h"

DarkenOverlay::DarkenOverlay()
{
    pSprite* spr;

    spr = new pSprite(TX_PLAY_DARKEN_OVERLAY, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color({0, 0, 0}), 0, 1.0f);
    mSprites.push_back(spr);
}

//can only be called after beatmap has been loaded
void DarkenOverlay::Initialize()
{
    mHpLossPerMs = DifficultyManager::GetHpDrainRate() * 2; //units are hp/ms based off a 200 point scale
    mHpCurrent = 0;
    mHpDisplay = 0;

    mTimeLastUpdate = GameClock::Clock().Time();
    mFillTime = MathHelper::Min(10000, BeatmapManager::Current().StartTime());
    mFillRate = MAXHP/((mFillTime-700)/(float)1000*60);

    int start = BeatmapManager::Current().StartTime() - mFillTime;
    int end = MathHelper::Max(BeatmapManager::Current().StartTime(), 701) - 700;

    mSprites[0]->Show(start,end);

    for (const BreakPoint& bp : BeatmapManager::Current().Breakpoints()) {
        int quarter =  MathHelper::Min(floor(((float)bp.EndTime-(float)bp.StartTime) / 4.f), 700);
        mSprites[0]->Hide(bp.StartTime,bp.StartTime+quarter);
        mSprites[0]->Show(bp.EndTime-quarter,bp.EndTime);
    }
}

void DarkenOverlay::ClearTransforms()
{
    for (auto & mSprite : mSprites)
    {
        mSprite->ClearTransforms();
    }
}