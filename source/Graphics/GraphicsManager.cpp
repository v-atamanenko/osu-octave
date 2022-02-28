#include "GraphicsManager.h"
#include <string>
#include <algorithm>

GraphicsManager GraphicsManager::sGraphicsManager;

bool GraphicsManager::LoadTexture(TextureType texid, const std::string& path) {
    std::unique_lock lock_pbs(mut_maptextures);

    if (maptextures[texid] != nullptr)
        SDL_DestroyTexture(maptextures[texid]);

    SDL_Texture* newTexture = nullptr;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if (loadedSurface == nullptr) {
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }

    if (renderer == nullptr) {
        fprintf(stderr, "Unable to create texture! Renderer is null.\n");
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if (newTexture == nullptr) {
        fprintf(stderr,  "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    SDL_FreeSurface( loadedSurface );
    maptextures[texid] = newTexture;
    return true;
}

SDL_Texture* GraphicsManager::LoadSquareTexture(const std::string& path) {
    SDL_Texture* newTexture = nullptr;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if (loadedSurface == nullptr) {
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return newTexture;
    }

    if (renderer == nullptr) {
        fprintf(stderr, "Unable to create texture! Renderer is null.\n");
        SDL_FreeSurface(loadedSurface);
        return newTexture;
    }

    SDL_Rect srcrect;
    srcrect.h = srcrect.w = loadedSurface->h;
    srcrect.y = 0;
    srcrect.x = floor(((float)loadedSurface->w / 2.f) - ((float)loadedSurface->h / 2.f));

    SDL_Rect dstrect;
    dstrect.h = dstrect.w = srcrect.h;
    dstrect.y = dstrect.x = 0;

    uint32_t pf = SDL_GetWindowPixelFormat(window);

    SDL_Surface* cropped = SDL_CreateRGBSurfaceWithFormat(0, dstrect.w, dstrect.h, 8, pf);

    SDL_BlitSurface(loadedSurface,&srcrect,cropped,&dstrect);
    SDL_FreeSurface(loadedSurface);

    newTexture = SDL_CreateTextureFromSurface( renderer, cropped );
    if (newTexture == nullptr) {
        fprintf(stderr,  "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }

    SDL_FreeSurface(cropped);
    return newTexture;
}

void GraphicsManager::LoadBeatmapPicTexture(TextureType texid, SDL_Surface *tex) {
    std::unique_lock lock_pbs(mut_maptextures);
    if (mapsurfaces.count(texid) > 0) {
        if (mapsurfaces.at(texid) == tex) {
            // Texture's already using this surface.
            return;
        }
    }

    mapsurfaces[texid] = tex;
    if (maptextures.count(texid) > 0) {
        if (maptextures[texid] != nullptr) {
            SDL_DestroyTexture(maptextures[texid]);
        }
    }
    SDL_Texture * newTexture = SDL_CreateTextureFromSurface( renderer, tex );
    if (newTexture == nullptr) {
        fprintf(stderr,  "Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
    }
    maptextures[texid] = newTexture;
}

SDL_Texture * GraphicsManager::GetTexture(TextureType texid) {
    std::shared_lock lock_pbs(mut_maptextures);
    SDL_Texture * x =maptextures[texid];
    return x;
}

void GraphicsManager::LoadBeatmapBackground(const std::string& path) {
    LoadTexture(TX_CURRENT_BG, path);
}

void GraphicsManager::CreateTextureFromSurface(SDL_Surface* bg, TextureType texid) {
    if (!bg) {
        fprintf(stderr, "CreateTextureFromSurface called with empty argument.\n");
        return;
    }

    SDL_Surface* optimizedSurface = nullptr;
    SDL_Texture* newTexture = nullptr;

    Uint32 u_format = SDL_GetWindowPixelFormat(window);
    SDL_PixelFormat* format = SDL_AllocFormat(u_format);

    optimizedSurface = SDL_ConvertSurface(bg, format, 0 );

    SDL_FreeFormat(format);
    SDL_FreeSurface(bg);

    if (optimizedSurface == nullptr) {
        fprintf(stderr, "Unable to optimize background! SDL Error: %s\n", SDL_GetError());
        return;
    }

    newTexture = SDL_CreateTextureFromSurface( renderer, optimizedSurface );
    if (newTexture == nullptr) {
        fprintf( stderr, "Unable to create texture for bg! SDL Error: %s\n", SDL_GetError() );
    }

    SDL_FreeSurface( optimizedSurface );

    if (maptextures[texid] != nullptr) {
        SDL_DestroyTexture(maptextures[texid]);
    }
    maptextures[texid] = newTexture;
}

void GraphicsManager::DrawBeatmapBackground() {
    std::shared_lock lock_pbs(mut_maptextures);

    if (maptextures[TX_CURRENT_BG] != nullptr) {
        if (SDL_RenderCopy(renderer, maptextures[TX_CURRENT_BG], nullptr, nullptr) < 0) {
            fprintf(stderr, "DrawBeatmapBackground failed with: %s\n", SDL_GetError());
        }
    }
}

void GraphicsManager::Clear() {
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
}

void GraphicsManager::Present() {
    SDL_RenderPresent(renderer);
}

void GraphicsManager::LoadTexturesForMode(ModeType mod) {
    switch (mod) {
        case MODE_PLAYER:
            LoadTexture(TX_PLAY_CIRCLE, "data/textures/circle.png");
            LoadTexture(TX_PLAY_CIRCLEOVERLAY, "data/textures/circleoverlay.png");
            LoadTexture(TX_PLAY_CIRCLEAPPROACH, "data/textures/circleapproach.png");

            LoadTexture(TX_PLAY_DISC, "data/textures/disc.png");
            LoadTexture(TX_PLAY_SLIDERTICK, "data/textures/slidertick.png");
            LoadTexture(TX_PLAY_SLIDERREVERSE, "data/textures/sliderreverse.png");

            LoadTexture(TX_WHITE, "data/textures/white.png");

            LoadTexture(TX_PLAY_SPINNER, "data/textures/spinner.png");
            LoadTexture(TX_PLAY_SPINNERBARS, "data/textures/spinnerbars.png");
            LoadTexture(TX_PLAY_SCOREBAR_BAR, "data/textures/scorebar_colour.png");

            LoadTexture(TX_PLAY_SPINNERBG, "data/textures/spinnerbg.png");

            LoadTexture(TX_PLAY_SLIDERB0, "data/textures/sliderb0.png");
            LoadTexture(TX_PLAY_SLIDERB1, "data/textures/sliderb1.png");
            LoadTexture(TX_PLAY_SLIDERB2, "data/textures/sliderb2.png");
            LoadTexture(TX_PLAY_SLIDERB3, "data/textures/sliderb3.png");
            LoadTexture(TX_PLAY_SLIDERB4, "data/textures/sliderb4.png");
            LoadTexture(TX_PLAY_SLIDERB5, "data/textures/sliderb5.png");
            LoadTexture(TX_PLAY_SLIDERB6, "data/textures/sliderb6.png");
            LoadTexture(TX_PLAY_SLIDERB7, "data/textures/sliderb7.png");
            LoadTexture(TX_PLAY_SLIDERB8, "data/textures/sliderb8.png");
            LoadTexture(TX_PLAY_SLIDERB9, "data/textures/sliderb9.png");
            LoadTexture(TX_PLAY_SLIDERFOLLOW, "data/textures/sliderfollow.png");

            LoadTexture(TX_PLAY_HIT0, "data/textures/hit0.png");
            LoadTexture(TX_PLAY_HIT300, "data/textures/hit300.png");
            LoadTexture(TX_PLAY_HIT300K, "data/textures/hit300k.png");
            LoadTexture(TX_PLAY_HIT300G, "data/textures/hit300g.png");

            LoadTexture(TX_PLAY_HIT50, "data/textures/hit50.png");
            LoadTexture(TX_PLAY_HIT100, "data/textures/hit100.png");
            LoadTexture(TX_PLAY_HIT100K, "data/textures/hit100k.png");

            LoadTexture(TX_PLAY_SLIDER30, "data/textures/slider30.png");
            LoadTexture(TX_PLAY_SLIDER10, "data/textures/slider10.png");

            LoadTexture(TX_PLAY_SCOREBAR_KI, "data/textures/scorebar_ki.png");
            LoadTexture(TX_PLAY_SCOREBAR_KIDANGER, "data/textures/scorebar_kidanger.png");
            LoadTexture(TX_PLAY_SCOREBAR_KIDANGER2, "data/textures/scorebar_kidanger2.png");

            LoadTexture(TX_PLAY_SCOREBAR, "data/textures/scorebar.png");

            LoadTexture(TX_RANKING_A_LG, "data/textures/ranking-a-lg.png");
            LoadTexture(TX_RANKING_B_LG, "data/textures/ranking-b-lg.png");
            LoadTexture(TX_RANKING_C_LG, "data/textures/ranking-c-lg.png");
            LoadTexture(TX_RANKING_D_LG, "data/textures/ranking-d-lg.png");
            LoadTexture(TX_RANKING_S_LG, "data/textures/ranking-s-lg.png");
            LoadTexture(TX_RANKING_SS_LG, "data/textures/ranking-ss-lg.png");
            LoadTexture(TX_RANKING_F_LG, "data/textures/ranking-f-lg.png");
            LoadTexture(TX_BUTTON_BIG, "data/ui/button-big.png");

            LoadTexture(TX_GAMEOVER_BG, "data/ui/gameover-bg.png");
            LoadTexture(TX_PAUSE_BG, "data/ui/pause-bg.png");

            CreateRectangularTexture(TX_PLAY_DARKEN_OVERLAY, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_Color({0, 0, 0, 150}));
            break;
        case MODE_SONGSELECT:
            LoadTexture(TX_SONGSELECT_BG, "data/ui/song-selector.png");
            LoadTexture(TX_BUTTON_BIG, "data/ui/button-big.png");
            LoadTexture(TX_BUTTON_MED, "data/ui/button-med.png");
            LoadTexture(TX_BUTTON_SM, "data/ui/button-sm.png");
            LoadTexture(TX_BUTTON_SM_ACTIVE, "data/ui/button-sm-active.png");
            LoadTexture(TX_BUTTON_XS, "data/ui/button-xs.png");
            LoadTexture(TX_BUTTON_ARROW, "data/ui/button-arrow.png");
            LoadTexture(TX_BUTTON_PLAY, "data/ui/button-play.png");

            LoadTexture(TX_RANKING_UNK_SM, "data/textures/ranking-unk-sm.png");
            LoadTexture(TX_RANKING_A_SM, "data/textures/ranking-a-sm.png");
            LoadTexture(TX_RANKING_B_SM, "data/textures/ranking-b-sm.png");
            LoadTexture(TX_RANKING_C_SM, "data/textures/ranking-c-sm.png");
            LoadTexture(TX_RANKING_D_SM, "data/textures/ranking-d-sm.png");
            LoadTexture(TX_RANKING_S_SM, "data/textures/ranking-s-sm.png");
            LoadTexture(TX_RANKING_SS_SM, "data/textures/ranking-ss-sm.png");

            LoadTexture(TX_STARS, "data/ui/stars.png");
            LoadTexture(TX_STARS_FILL, "data/ui/stars-fill.png");

            CreateRectangularTexture(TX_BEATMAP_ENTRY_BG, 609, 80, SDL_Color({199, 190, 235, 127}));
            CreateRectangularTexture(TX_BEATMAP_ENTRY_EXPANDED_BG, 609, 174, SDL_Color({199, 190, 235, 127}));

            break;
        case MODE_WELCOME:
            LoadTexture(TX_WELCOME_BG, "data/ui/welcome.png");
            LoadTexture(TX_TAP_TO_START, "data/ui/tap-to-start.png");
            break;
    }
}

void GraphicsManager::UnloadTextures() {
    std::unique_lock lock_pbs(mut_maptextures);

    auto it = maptextures.begin();
    // Iterate through the map
    while (it != maptextures.end())
    {
        // Check if value of this entry matches with given value
        if (it->first == TX_BEATMAP_ENTRY_PREVIEW ||
            it->first == TX_BEATMAP_ENTRY_PIC_0 ||
            it->first == TX_BEATMAP_ENTRY_PIC_1 ||
            it->first == TX_BEATMAP_ENTRY_PIC_2 ||
            it->first == TX_BEATMAP_ENTRY_PIC_3) {
            it++;
        }
        else
        {
            SDL_DestroyTexture(it->second);
            it = maptextures.erase(it);
        }
    }
}

void GraphicsManager::Draw(TextureType tex, int32_t x, int32_t y, uint32_t width, uint32_t height, DrawOrigin origin, FieldType fieldtype, SDL_Color color, uint32_t alpha, int32_t angle, float z, const SDL_Rect* uv) {
    std::unique_lock lock_pbs(mut_maptextures);
    int32_t x1, y1, x2, y2;

    // We use "UV Coordinates" as a source rect for render copy, thus being able to draw textures partly.
    if (uv != nullptr) {
        width = uv->w;
        height = uv->h;
    }

    if (fieldtype == FIELD_PLAY) {
        x += (int32_t)PlayXOffset;
        y += (int32_t)PlayYOffset;
    }

    switch (origin) {
        case ORIGIN_TOPLEFT:
            x1 = ForceBounds(x);
            y1 = ForceBounds(y);
            x2 = ForceBounds(x + (int32_t)width);
            y2 = ForceBounds(y + (int32_t)height);
            break;

        case ORIGIN_CENTER:
            width /= 2;
            height /= 2;

            x1 = ForceBounds(x - (int32_t)width);
            x2 = ForceBounds(x + (int32_t)width);
            y1 = ForceBounds(y - (int32_t)height);
            y2 = ForceBounds(y + (int32_t)height);
            break;

        case ORIGIN_BOTTOMLEFT:
            x1 = ForceBounds(x);
            x2 = ForceBounds(x + (int32_t)width);
            y1 = ForceBounds((SCREEN_HEIGHT) - y - (int32_t)height);
            y2 = ForceBounds((SCREEN_HEIGHT) - y);
            break;

        case ORIGIN_TOPRIGHT:
            x1 = ForceBounds(x - (int32_t)width);
            x2 = ForceBounds(x);
            y1 = ForceBounds(y);
            y2 = ForceBounds(y + (int32_t)height);
    }


    //don't draw things out of the screen
    if (x1 > SCREEN_WIDTH || x2 < 0 || y1 > SCREEN_HEIGHT || y2 < 0)
        return;
    SDL_Rect dst = {x1, y1, x2-x1, y2-y1};

    if ((color.r != 0) && (color.g != 0) && (color.b != 0)) {
        SDL_SetTextureColorMod(maptextures[tex], color.r, color.g, color.b);
    }

    SDL_SetTextureAlphaMod(maptextures[tex], alpha);
    SDL_RenderCopyEx(renderer, maptextures[tex], uv, &dst, angle, nullptr, SDL_FLIP_NONE);

    if ((color.r != 0) && (color.g != 0) && (color.b != 0)) {
        SDL_SetTextureColorMod(maptextures[tex], 255, 255, 255);
    }
}

int32_t GraphicsManager::ForceBounds(int32_t value) {
    if (value < -200) {
        return -200;
    }

    if (value > std::max(((SCREEN_HEIGHT)+200), ((SCREEN_WIDTH)+200))) {
        return std::max(((SCREEN_HEIGHT)+200), ((SCREEN_WIDTH)+200));
    }

    return value;
}

//TODO: Move to sprites (?)
void GraphicsManager::DrawFullScreenRectangle(SDL_Color c) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_Rect r = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &r);
}

void GraphicsManager::CreateRectangularTexture(TextureType texid, uint32_t width, uint32_t height, SDL_Color c) {
    std::unique_lock lock_pbs(mut_maptextures);

    SDL_Texture* tex;

    uint32_t pf = SDL_GetWindowPixelFormat(window);
    tex = SDL_CreateTexture(renderer, pf, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget( renderer, tex );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget( renderer, nullptr );

    if (maptextures[texid] != nullptr) {
        SDL_DestroyTexture(maptextures[texid]);
    }
    maptextures[texid] = tex;
}
