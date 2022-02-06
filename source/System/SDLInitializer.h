#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#ifndef NANODOSU_SDLINITIALIZER
#define NANODOSU_SDLINITIALIZER

class SDLInitializer
{
public:
    static SDLInitializer& initSDL() { return SDLI; }
protected:
    static SDLInitializer SDLI;
private:
    SDLInitializer();
};

#endif //NANODOSU_SDLINITIALIZER
