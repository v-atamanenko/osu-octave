#include "SDLInitializer.h"

SDLInitializer SDLInitializer::SDLI;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

SDLInitializer::SDLInitializer() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError( ));
        SDL_Quit();
    }

    if (SDL_NumJoysticks() > 0) {
        SDL_JoystickOpen(0);
        SDL_GameControllerOpen(0);
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

#ifdef __vita__
    // VITA: Touch events will not generate mouse events
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
#endif

    window = SDL_CreateWindow("osu!octave",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_RESIZABLE); //SDL_WINDOW_FULLSCREEN_DESKTOP // SDL_WINDOW_OPENGL
    if (window == nullptr) {
        fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
        SDL_Quit();
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
        SDL_Quit();
    }

    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
    }

    int mixFlags = MIX_INIT_MP3;
    if (!(Mix_Init(mixFlags) & mixFlags)) {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_Quit();
    }
}
