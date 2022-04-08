#pragma once

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

// OOTime / Osu-Octave Time, the type to be used to represent time.
//#define OOTime int32_t
typedef int32_t OOTime;
typedef uint8_t OOUShort;
typedef int8_t OOShort;
typedef int32_t OOInt;
typedef uint32_t OOUInt;
typedef double OOFloat;

typedef enum BeatmapFilter {
    FILTER_NONE,
    FILTER_ALPHA_A_E,
    FILTER_ALPHA_F_J,
    FILTER_ALPHA_K_O,
    FILTER_ALPHA_P_T,
    FILTER_ALPHA_U_Z
} BeatmapFilter;

/*
 * InputHelper Types
 */

struct touchPosition {
    OOInt px;
    OOInt py;
};

typedef enum KeyType {
    IH_KEY_MOUSE = 0,
    IH_KEY_CONTROLLER = 1,
    IH_KEY_KEYBOARD = 2
} KeyType;

struct RawKey {
    OOInt key;
    KeyType type;
};

typedef enum Control {
    IH_CONTROL_ACTION = 0, // (SDL_BUTTON_LEFT, IH_KEY_MOUSE) || (SDLK_z, IH_KEY_KEYBOARD) || (SDLK_x, IH_KEY_KEYBOARD)
    IH_CONTROL_SKIP = 1, // (SDLK_SPACE, IH_KEY_KEYBOARD) || (SDL_CONTROLLER_BUTTON_A, IH_KEY_CONTROLLER)
    IH_CONTROL_QUIT = 2 // (SDLK_ESCAPE, IH_KEY_KEYBOARD) || (SDL_CONTROLLER_BUTTON_START, IH_KEY_CONTROLLER)
} Control;

/*
 * Score types
 */

typedef enum {
    SCORE_300 = 300,
    SCORE_GEKI = 301,
    SCORE_100 = 100,
    SCORE_KATU = 102,
    SCORE_50 = 50,
    SCORE_TICK_30 = 30,
    SCORE_TICK_10 = 10,
    SCORE_SPIN_100 = 101, //should be 100 but conflicts with SCORE_100
    SCORE_SPIN_1000 = 1000,
    SCORE_MISS = 0,
    SCORE_COMBOBREAK
} ScoreType;

/*
 * Graphics types
 */

typedef enum DrawOrigin {
    ORIGIN_CENTER,
    ORIGIN_TOPLEFT,
    ORIGIN_BOTTOMLEFT,
    ORIGIN_TOPRIGHT
} DrawOrigin;

typedef enum FieldType {
    FIELD_SCREEN,
    FIELD_PLAY
} FieldType;

/*
 * Resources types (fonts, etc)
 */

typedef enum FontName {
    FONT_CONSOLE,
    FONT_CONSOLE_BOLD,
    FONT_CONSOLE_BIG,
    FONT_CONSOLE_BIG_BOLD,
    FONT_SCORE,
    FONT_NUMBERING,
    FONT_PIXEL,
} FontName;

typedef enum UISoundName {
    UISOUND_APPLAUSE,
    UISOUND_BGM,
    UISOUND_CHECK_OFF,
    UISOUND_CHECK_ON,
    UISOUND_CLICK_CLOSE,
    UISOUND_CLICK_SHORT_CONFIRM,
    UISOUND_COMBOBREAK,
    UISOUND_FAILSOUND,
    UISOUND_MENUBACK,
    UISOUND_MENUCLICK,
    UISOUND_MENUHIT,
    UISOUND_SEEYA,
    UISOUND_WELCOME,
    UISOUND_WELCOME_PIANO
} UISoundName;

typedef enum HitSoundName {
    HITSOUND_NORMAL_HITNORMAL,
    HITSOUND_NORMAL_HITWHISTLE,
    HITSOUND_NORMAL_HITFINISH,
    HITSOUND_NORMAL_HITCLAP,
    HITSOUND_NORMAL_SLIDERTICK,
    HITSOUND_NORMAL_SLIDERSLIDE,
    HITSOUND_NORMAL_SLIDERWHISTLE,
    HITSOUND_NORMAL_SPINNERSPIN,
    HITSOUND_NORMAL_SPINNERBONUS,
    HITSOUND_SOFT_HITNORMAL,
    HITSOUND_SOFT_HITWHISTLE,
    HITSOUND_SOFT_HITFINISH,
    HITSOUND_SOFT_HITCLAP,
    HITSOUND_SOFT_SLIDERTICK,
    HITSOUND_SOFT_SLIDERSLIDE,
    HITSOUND_SOFT_SLIDERWHISTLE,
    HITSOUND_SOFT_SPINNERSPIN,
    HITSOUND_SOFT_SPINNERBONUS
} HitSoundName;
