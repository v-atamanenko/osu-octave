#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "SDL.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Surface *screen_surface;

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544

#define mapXToScreen(x) (int32_t)(floor((float)(x) / (640.f/((float)(SCREEN_WIDTH)))))
#define mapYToScreen(x) (int32_t)(floor((float)(x) / (480.f/((float)(SCREEN_HEIGHT)))))

#ifdef VITA
#define DATA_DIR "ux0:/data/osu/"
#endif

#ifdef DESKTOP
#define DATA_DIR "/Users/v/Repos/osu-ds/cmake-build-debug"
#endif

#endif
