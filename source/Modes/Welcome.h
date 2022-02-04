#include "SDL.h"
#include <cstdio>

#include "Mode.h"
#include "Beatmaps/BeatmapManager.h"
#include "Graphics/SpriteManager.h"
#include "System/TextManager.h"

#ifndef __WELCOME_H__
#define __WELCOME_H__

class Welcome : public Mode
{
public:
    Welcome();
    ~Welcome();

    void Update();
    void HandleInput();

protected:
    SpriteManager mSpriteManager;
};

#endif

