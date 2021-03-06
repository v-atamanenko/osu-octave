#pragma once

#include "SDL.h"
#include <cstdio>

#include "Mode.h"
#include "Beatmaps/BeatmapManager.h"
#include "Graphics/SpriteManager.h"
#include "System/TextManager.h"
#include "Graphics/pText.h"
#include "UIElements/Logo.h"

#include "Graphics/pText.h"
#include "Helpers/PreviewBuffer.h"
#include "DataStorage/Settings.h"
#include "DataStorage/Scores.h"

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
    ~Welcome() override { delete mLogo; };

    void Update() override;
    void HandleInput() override;
    void Redraw();

protected:
    SpriteManager mSpriteManager;
    pSprite* mBG;
    Logo* mLogo;
    pText* mStatus;
    LoadingStage mStage = STAGE_LOAD_SETTINGS;
};
