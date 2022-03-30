#pragma once

#include "System/GameClock.h"
#include "Graphics/SpriteContainer.h"
#include "Beatmaps/BeatmapManager.h"

class DarkenOverlay : public SpriteContainer
{
    public:
        DarkenOverlay();
        void Initialize();
};
