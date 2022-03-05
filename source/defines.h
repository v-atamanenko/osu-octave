#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "SDL.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544

//FIXME: The following three mapping functions are used to ease transfer from 640x490 to 960x544. Must be eliminated
//in future.
#define osuPixelsXtoScreenX(x) (x)
#define osuPixelsYtoScreenY(x) (x)

#define mapXToScreen_noFloor(x) (x)

#define ScreenX(x) (int32_t)(x)
#define ScreenY(y) (int32_t)(y)


#define PLAYFIELD_X_OFFSET ((SCREEN_WIDTH-640)/2)
#define PLAYFIELD_Y_OFFSET 64

#define SCREEN_X_TO_PLAYFIELD_X(x) ((x)-PLAYFIELD_X_OFFSET)
#define SCREEN_Y_TO_PLAYFIELD_Y(y) ((y)-PLAYFIELD_Y_OFFSET)

/*
 * NOTE! In DEF_*Directory/DEF_*Subdirectory defines, trailing slash is mandatory!
 */

#ifdef VITA
#define DEF_DataDirectory "ux0:/data/osu/"
#define DEF_BeatmapsSubdirectory "maps/"
#define DEF_SettingsFilename "settings.json"
#define DEF_HighscoresFilename "settings.json"
#define DEF_BeatmapsIndexFilename "beatmaps.json"
#define DEF_BeatmapsStateFilename "beatmaps_state.json"
#endif

#ifdef DESKTOP
#define DEF_DataDirectory "./"
#define DEF_BeatmapsSubdirectory "maps/"
#define DEF_SettingsFilename "settings.json"
#define DEF_HighscoresFilename "highscores.json"
#define DEF_BeatmapsIndexFilename "beatmaps.json"
#define DEF_BeatmapsStateFilename "beatmaps_state.json"
#endif

#endif
