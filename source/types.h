#ifndef __TYPES_H__
#define __TYPES_H__

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

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
    uint32_t px;
    uint32_t py;
};

typedef enum KeyType {
    IH_KEY_MOUSE = 0,
    IH_KEY_CONTROLLER = 1,
    IH_KEY_KEYBOARD = 2
} KeyType;

struct RawKey {
    int key;
    KeyType type;
};

typedef enum Control {
    IH_CONTROL_ACTION = 0, // (SDL_BUTTON_LEFT, IH_KEY_MOUSE) || (SDLK_z, IH_KEY_KEYBOARD) || (SDLK_x, IH_KEY_KEYBOARD)
    IH_CONTROL_SKIP = 1, // (SDLK_SPACE, IH_KEY_KEYBOARD) || (SDL_CONTROLLER_BUTTON_A, IH_KEY_CONTROLLER)
    IH_CONTROL_QUIT = 2 // (SDLK_ESCAPE, IH_KEY_KEYBOARD) || (SDL_CONTROLLER_BUTTON_START, IH_KEY_CONTROLLER)
} Control;

#endif // __TYPES_H__
