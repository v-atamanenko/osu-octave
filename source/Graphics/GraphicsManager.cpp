#include "GraphicsManager.h"
#include "DataStorage/Settings.h"
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
    if (maptextures[TX_CURRENT_BG] != nullptr)
        SDL_DestroyTexture(maptextures[TX_CURRENT_BG]);

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if (loadedSurface == nullptr) {
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return;
    }

    if (renderer == nullptr) {
        fprintf(stderr, "Unable to create texture! Renderer is null.\n");
        SDL_FreeSurface(loadedSurface);
        return ;
    }

    SDL_Rect srcrect;
    srcrect.w = (int)round((float)(loadedSurface->h) / ((float)SCREEN_HEIGHT/(float)SCREEN_WIDTH));

    if (loadedSurface->w > loadedSurface->h && srcrect.w <= loadedSurface->w) {
        srcrect.h = loadedSurface->h;
        srcrect.x = floor(((float)loadedSurface->w - (float)srcrect.w) / 2.f);
        srcrect.y = 0;
    } else {
        srcrect.w = loadedSurface->w;
        srcrect.h = (int)round((float)(loadedSurface->w) / ((float)SCREEN_WIDTH/(float)SCREEN_HEIGHT));
        srcrect.x = 0;
        srcrect.y = floor(((float)loadedSurface->h - (float)srcrect.h) / 2.f);
    }

    SDL_Rect dstrect;
    dstrect.h = srcrect.h;
    dstrect.w = srcrect.w;
    dstrect.y = dstrect.x = 0;

    uint32_t pf = SDL_GetWindowPixelFormat(window);

    SDL_Surface* cropped = SDL_CreateRGBSurfaceWithFormat(0, dstrect.w, dstrect.h, 8, pf);

    SDL_BlitSurface(loadedSurface,&srcrect,cropped,&dstrect);
    SDL_FreeSurface(loadedSurface);

    maptextures[TX_CURRENT_BG] = SDL_CreateTextureFromSurface( renderer, cropped );
    if (maptextures[TX_CURRENT_BG] == nullptr) {
        fprintf(stderr,  "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }

    SDL_FreeSurface(cropped);
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
    std::string skin_path = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/";

    switch (mod) {
        case MODE_PLAYER:
            LoadTexture(TX_PLAY_CURSOR, skin_path+"ui/cursor.png");

            LoadTexture(TX_PLAY_CIRCLE, skin_path+"gameplay/circle.png");
            LoadTexture(TX_PLAY_CIRCLEOVERLAY, skin_path+"gameplay/circleoverlay.png");
            LoadTexture(TX_PLAY_CIRCLEAPPROACH, skin_path+"gameplay/circleapproach.png");

            LoadTexture(TX_PLAY_DISC, skin_path+"gameplay/disc.png");
            LoadTexture(TX_PLAY_SLIDERTICK, skin_path+"gameplay/slidertick.png");
            LoadTexture(TX_PLAY_SLIDERREVERSE, skin_path+"gameplay/sliderreverse.png");

            LoadTexture(TX_WHITE, skin_path+"gameplay/white.png");

            LoadTexture(TX_PLAY_SPINNER, skin_path+"gameplay/spinner.png");
            LoadTexture(TX_PLAY_SPINNERBARS, skin_path+"gameplay/spinnerbars.png");

            LoadTexture(TX_PLAY_SPINNERBG, skin_path+"gameplay/spinnerbg.png");

            LoadTexture(TX_PLAY_SLIDERB0, skin_path+"gameplay/sliderb0.png");
            LoadTexture(TX_PLAY_SLIDERB1, skin_path+"gameplay/sliderb1.png");
            LoadTexture(TX_PLAY_SLIDERB2, skin_path+"gameplay/sliderb2.png");
            LoadTexture(TX_PLAY_SLIDERB3, skin_path+"gameplay/sliderb3.png");
            LoadTexture(TX_PLAY_SLIDERB4, skin_path+"gameplay/sliderb4.png");
            LoadTexture(TX_PLAY_SLIDERB5, skin_path+"gameplay/sliderb5.png");
            LoadTexture(TX_PLAY_SLIDERB6, skin_path+"gameplay/sliderb6.png");
            LoadTexture(TX_PLAY_SLIDERB7, skin_path+"gameplay/sliderb7.png");
            LoadTexture(TX_PLAY_SLIDERB8, skin_path+"gameplay/sliderb8.png");
            LoadTexture(TX_PLAY_SLIDERB9, skin_path+"gameplay/sliderb9.png");
            LoadTexture(TX_PLAY_SLIDERFOLLOW, skin_path+"gameplay/sliderfollow.png");

            LoadTexture(TX_PLAY_HIT0, skin_path+"gameplay/hit0.png");
            LoadTexture(TX_PLAY_HIT300, skin_path+"gameplay/hit300.png");
            LoadTexture(TX_PLAY_HIT300K, skin_path+"gameplay/hit300k.png");
            LoadTexture(TX_PLAY_HIT300G, skin_path+"gameplay/hit300g.png");

            LoadTexture(TX_PLAY_HIT50, skin_path+"gameplay/hit50.png");
            LoadTexture(TX_PLAY_HIT100, skin_path+"gameplay/hit100.png");
            LoadTexture(TX_PLAY_HIT100K, skin_path+"gameplay/hit100k.png");

            LoadTexture(TX_PLAY_SLIDER30, skin_path+"gameplay/slider30.png");
            LoadTexture(TX_PLAY_SLIDER10, skin_path+"gameplay/slider10.png");

            LoadTexture(TX_PLAY_SCOREBAR, skin_path+"ui/scorebar.png");
            LoadTexture(TX_PLAY_SCOREBAR_BAR, skin_path+"ui/scorebar_colour.png");
            LoadTexture(TX_PLAY_SCOREBAR_KI, skin_path+"ui/scorebar_ki.png");
            LoadTexture(TX_PLAY_SCOREBAR_KIDANGER, skin_path+"ui/scorebar_kidanger.png");
            LoadTexture(TX_PLAY_SCOREBAR_KIDANGER2, skin_path+"ui/scorebar_kidanger2.png");

            LoadTexture(TX_RANKING_A_LG, skin_path+"ui/ranking-a-lg.png");
            LoadTexture(TX_RANKING_B_LG, skin_path+"ui/ranking-b-lg.png");
            LoadTexture(TX_RANKING_C_LG, skin_path+"ui/ranking-c-lg.png");
            LoadTexture(TX_RANKING_D_LG, skin_path+"ui/ranking-d-lg.png");
            LoadTexture(TX_RANKING_S_LG, skin_path+"ui/ranking-s-lg.png");
            LoadTexture(TX_RANKING_SS_LG, skin_path+"ui/ranking-ss-lg.png");
            LoadTexture(TX_RANKING_F_LG, skin_path+"ui/ranking-f-lg.png");
            LoadTexture(TX_BUTTON_BIG, skin_path+"ui/button-big.png");

            LoadTexture(TX_GAMEOVER_BG, skin_path+"ui/gameover-bg.png");
            LoadTexture(TX_PAUSE_BG, skin_path+"ui/pause-bg.png");

            CreateRectangularTexture(TX_PLAY_DARKEN_OVERLAY, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_Color({0, 0, 0, 150}));
            break;
        case MODE_SONGSELECT:
            LoadTexture(TX_SONGSELECT_BG, skin_path+"ui/song-selector.png");
            LoadTexture(TX_BUTTON_BIG, skin_path+"ui/button-big.png");
            LoadTexture(TX_BUTTON_MED, skin_path+"ui/button-med.png");
            LoadTexture(TX_BUTTON_SM, skin_path+"ui/button-sm.png");
            LoadTexture(TX_BUTTON_SM_ACTIVE, skin_path+"ui/button-sm-active.png");
            LoadTexture(TX_BUTTON_XS, skin_path+"ui/button-xs.png");
            LoadTexture(TX_BUTTON_ARROW, skin_path+"ui/button-arrow.png");
            LoadTexture(TX_BUTTON_PLAY, skin_path+"ui/button-play.png");

            LoadTexture(TX_RANKING_UNK_SM, skin_path+"ui/ranking-unk-sm.png");
            LoadTexture(TX_RANKING_A_SM, skin_path+"ui/ranking-a-sm.png");
            LoadTexture(TX_RANKING_B_SM, skin_path+"ui/ranking-b-sm.png");
            LoadTexture(TX_RANKING_C_SM, skin_path+"ui/ranking-c-sm.png");
            LoadTexture(TX_RANKING_D_SM, skin_path+"ui/ranking-d-sm.png");
            LoadTexture(TX_RANKING_S_SM, skin_path+"ui/ranking-s-sm.png");
            LoadTexture(TX_RANKING_SS_SM, skin_path+"ui/ranking-ss-sm.png");

            LoadTexture(TX_STARS, skin_path+"ui/stars.png");
            LoadTexture(TX_STARS_FILL, skin_path+"ui/stars-fill.png");

            CreateRectangularTexture(TX_BEATMAP_ENTRY_BG, 609, 80, SDL_Color({199, 190, 235, 127}));
            CreateRectangularTexture(TX_BEATMAP_ENTRY_EXPANDED_BG, 609, 174, SDL_Color({199, 190, 235, 127}));
            CreateRectangularTexture(TX_SONGLESECT_LOADING_BG, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_Color({255, 255, 255, 229}));
            break;
        case MODE_WELCOME:
            LoadTexture(TX_WELCOME_BG, skin_path+"ui/welcome.png");
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

bool GraphicsManager::ScaleSurface(SDL_Surface*& pSurface, const int limit) {
    bool res = true;
    int width = pSurface->w;
    int height = pSurface->h;

    SDL_Rect sourceDimensions;
    sourceDimensions.x = 0;
    sourceDimensions.y = 0;
    sourceDimensions.w = width;
    sourceDimensions.h = height;

    float scale = (float)limit / (float)width;
    float scaleH = (float)limit / (float)height;

    if (scaleH < scale) {
        scale = scaleH;
    }

    SDL_Rect targetDimensions;
    targetDimensions.x = 0;
    targetDimensions.y = 0;
    targetDimensions.w = (int)((float)width * scale);
    targetDimensions.h = (int)((float)height * scale);

    // create a 32 bits per pixel surface to Blit the image to first, before BlitScaled
    // https://stackoverflow.com/questions/33850453/sdl2-blit-scaled-from-a-palettized-8bpp-surface-gives-error-blit-combination/33944312
    SDL_Surface *p32BPPSurface = SDL_CreateRGBSurface(
            pSurface->flags,
            sourceDimensions.w,
            sourceDimensions.h,
            32,
            pSurface->format->Rmask,
            pSurface->format->Gmask,
            pSurface->format->Bmask,
            pSurface->format->Amask);

    if (SDL_BlitSurface(pSurface, NULL, p32BPPSurface, NULL) < 0) {
        fprintf(stderr, "Error did not blit surface: %s\n", SDL_GetError());
        res = false;
    }
    else {
        // create another 32 bits per pixel surface are the desired scale
        SDL_Surface *pScaleSurface = SDL_CreateRGBSurface(
                p32BPPSurface->flags,
                targetDimensions.w,
                targetDimensions.h,
                p32BPPSurface->format->BitsPerPixel,
                p32BPPSurface->format->Rmask,
                p32BPPSurface->format->Gmask,
                p32BPPSurface->format->Bmask,
                p32BPPSurface->format->Amask);

        // 32 bit per pixel surfaces (loaded from the original file) won't scale down with BlitScaled, suggestion to first fill the surface
        // 8 and 24 bit depth pngs did not require this
        // https://stackoverflow.com/questions/20587999/sdl-blitscaled-doesnt-work
        SDL_FillRect(pScaleSurface, &targetDimensions, SDL_MapRGBA(pScaleSurface->format, 255, 0, 0, 255));

        if (SDL_BlitScaled(p32BPPSurface, NULL, pScaleSurface, NULL) < 0) {
            fprintf(stderr, "Error did not scale surface: %s\n", SDL_GetError());

            SDL_FreeSurface(pScaleSurface);
            pScaleSurface = NULL;
            res = false;
        }
        else {
            SDL_FreeSurface(pSurface);

            pSurface = pScaleSurface;
            width = pSurface->w;
            height = pSurface->h;
        }
    }

    SDL_FreeSurface(p32BPPSurface);
    p32BPPSurface = NULL;
    return res;
}

bool GraphicsManager::CropSurfaceToSquare(SDL_Surface*& loadedSurface) {
    SDL_Rect srcrect;
    srcrect.h = srcrect.w = loadedSurface->h;
    srcrect.y = 0;
    srcrect.x = floor(((float)loadedSurface->w / 2.f) - ((float)loadedSurface->h / 2.f));

    SDL_Rect dstrect;
    dstrect.h = dstrect.w = srcrect.h;
    dstrect.y = dstrect.x = 0;

    SDL_Surface* cropped = SDL_CreateRGBSurface(0, dstrect.w, dstrect.h, 32, rmask, gmask, bmask, amask);
    if (cropped == nullptr) {
        return false;
    }

    if (SDL_BlitSurface(loadedSurface,&srcrect,cropped,&dstrect) < 0) {
        SDL_FreeSurface(cropped);
        return false;
    }
    SDL_FreeSurface(loadedSurface);
    loadedSurface = cropped;
    cropped = nullptr;
    return true;
}