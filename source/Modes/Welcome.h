#include "SDL.h"
#include <cstdio>

#include "Mode.h"
#include "Beatmaps/BeatmapManager.h"
#include "Graphics/SpriteManager.h"
#include "System/TextManager.h"
#include "Graphics/pText.h"

#ifndef __WELCOME_H__
#define __WELCOME_H__

typedef enum LoadingStage {
    STAGE_LOAD_SETTINGS,
    STAGE_CHECK_INDEX,
    STAGE_LOAD_INDEX,
    STAGE_START_PREVIEWBUFFER,
    STAGE_LOAD_FONTS,
    STAGE_DONE
} LoadingStage;

class Welcome : public Mode
{
public:
    Welcome();

    void Update();
    void HandleInput();
    void Redraw();

protected:
    SpriteManager mSpriteManager;
    pSprite* mBG;
    pText* mStatus;
    LoadingStage mStage = STAGE_LOAD_SETTINGS;
};

#endif

