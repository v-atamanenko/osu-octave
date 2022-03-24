#ifndef _DARKENOVERLAY_H
#define _DARKENOVERLAY_H

#include "System/GameClock.h"
#include "Graphics/SpriteContainer.h"
#include "Beatmaps/BeatmapManager.h"

class DarkenOverlay : public SpriteContainer
{
    public:
        DarkenOverlay();
        void Initialize();
};

#endif //_DARKENOVERLAY_H
