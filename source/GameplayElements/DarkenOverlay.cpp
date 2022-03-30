#include "DarkenOverlay.h"

DarkenOverlay::DarkenOverlay() {
    auto* spr = new pSprite(TX_PLAY_DARKEN_OVERLAY, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 0);
    mSprites.push_back(spr);
}

// Can only be called after the beatmap has been loaded
void DarkenOverlay::Initialize() {
    OOTime mFillTime = MathHelper::Min(10000, BeatmapManager::Current().StartTime() - BeatmapManager::Current().AudioLeadIn());

    OOTime start = BeatmapManager::Current().StartTime() - mFillTime;
    OOTime end = MathHelper::Max(BeatmapManager::Current().StartTime(), 701) - 700;

    mSprites[0]->Z = (OOFloat)(BeatmapManager::Current().EndTime() + 6000);
    mSprites[0]->Show(start,end);

    for (const BreakPoint& bp : BeatmapManager::Current().Breakpoints()) {
        int quarter =  MathHelper::Min(floor(((float)bp.EndTime-(float)bp.StartTime) / 4.f), 700);
        mSprites[0]->Hide(bp.StartTime,bp.StartTime+quarter);
        mSprites[0]->Show(bp.EndTime-quarter,bp.EndTime);
    }
}
