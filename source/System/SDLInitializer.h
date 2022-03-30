#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "defines.h"

class SDLInitializer
{
public:
    static SDLInitializer& initSDL() { return SDLI; }
protected:
    static SDLInitializer SDLI;
private:
    SDLInitializer();
};
