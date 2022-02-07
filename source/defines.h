#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "SDL.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544

//FIXME: The following three mapping functions are used to ease transfer from 640x490 to 960x544. Must be eliminated
//in future.
#define mapXToScreen(x) (int32_t)(floor((float)(x) / (640.f/((float)(SCREEN_WIDTH)))))
#define mapYToScreen(x) (int32_t)(floor((float)(x) / (480.f/((float)(SCREEN_HEIGHT)))))

#define mapXToScreen_noFloor(x) (uint32_t)((float)(x) / (640.f/((float)(SCREEN_WIDTH))))

#define ScreenX(x) (int32_t)(x)
#define ScreenY(y) (int32_t)(y)

#define PlayfieldX(x) ((x)-64)
#define PlayfieldY(y) ((y)-73)

//PlayXOffset = 64;
//PlayYOffset = 73;

#ifdef VITA
#define DATA_DIR "ux0:/data/osu/"
#endif

#ifdef DESKTOP
#define DATA_DIR "/Users/v/Repos/osu-ds/cmake-build-debug"
#endif

#endif
