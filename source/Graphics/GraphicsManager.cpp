#include "GraphicsManager.h"

GraphicsManager GraphicsManager::sGraphicsManager;

bool GraphicsManager::LoadTexture(TextureType texid) {
    std::unique_lock lock_pbs(mut_maptextures);

    std::string path = Skins::get_res_path(texid);

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

    if (tex == nullptr) {
        return;
    }

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
    srcrect.w = (OOInt)round((OOFloat)(loadedSurface->h) / ((OOFloat)SCREEN_HEIGHT/(OOFloat)SCREEN_WIDTH));

    if (loadedSurface->w > loadedSurface->h && srcrect.w <= loadedSurface->w) {
        srcrect.h = loadedSurface->h;
        srcrect.x = floor(((OOFloat)loadedSurface->w - (OOFloat)srcrect.w) / 2.f);
        srcrect.y = 0;
    } else {
        srcrect.w = loadedSurface->w;
        srcrect.h = (OOInt)round((OOFloat)(loadedSurface->w) / ((OOFloat)SCREEN_WIDTH/(OOFloat)SCREEN_HEIGHT));
        srcrect.x = 0;
        srcrect.y = floor(((OOFloat)loadedSurface->h - (OOFloat)srcrect.h) / 2.f);
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
            LoadTexture(TX_PLAY_CURSOR);
            LoadTexture(TX_PLAY_SPINNER_RPM_BG);

            LoadTexture(TX_PLAY_CIRCLE);
            LoadTexture(TX_PLAY_CIRCLEOVERLAY);
            LoadTexture(TX_PLAY_CIRCLEAPPROACH);

            LoadTexture(TX_PLAY_SLIDERBODY);
            LoadTexture(TX_PLAY_SLIDERTICK);
            LoadTexture(TX_PLAY_SLIDERREVERSE);
            LoadTexture(TX_PLAY_SLIDERBORDER);

            LoadTexture(TX_PLAY_SPINNERBG);
            LoadTexture(TX_PLAY_SPINNER);
            LoadTexture(TX_PLAY_SPINNERBARS);

            LoadTexture(TX_PLAY_SLIDERB0);
            LoadTexture(TX_PLAY_SLIDERB1);
            LoadTexture(TX_PLAY_SLIDERB2);
            LoadTexture(TX_PLAY_SLIDERB3);
            LoadTexture(TX_PLAY_SLIDERB4);
            LoadTexture(TX_PLAY_SLIDERB5);
            LoadTexture(TX_PLAY_SLIDERB6);
            LoadTexture(TX_PLAY_SLIDERB7);
            LoadTexture(TX_PLAY_SLIDERB8);
            LoadTexture(TX_PLAY_SLIDERB9);
            LoadTexture(TX_PLAY_SLIDERFOLLOW);

            LoadTexture(TX_PLAY_NUMBER_0);
            LoadTexture(TX_PLAY_NUMBER_1);
            LoadTexture(TX_PLAY_NUMBER_2);
            LoadTexture(TX_PLAY_NUMBER_3);
            LoadTexture(TX_PLAY_NUMBER_4);
            LoadTexture(TX_PLAY_NUMBER_5);
            LoadTexture(TX_PLAY_NUMBER_6);
            LoadTexture(TX_PLAY_NUMBER_7);
            LoadTexture(TX_PLAY_NUMBER_8);
            LoadTexture(TX_PLAY_NUMBER_9);

            LoadTexture(TX_PLAY_HIT0);
            LoadTexture(TX_PLAY_HIT300);
            LoadTexture(TX_PLAY_HIT300K);
            LoadTexture(TX_PLAY_HIT300G);

            LoadTexture(TX_PLAY_HIT50);
            LoadTexture(TX_PLAY_HIT100);
            LoadTexture(TX_PLAY_HIT100K);

            LoadTexture(TX_PLAY_SLIDER30);
            LoadTexture(TX_PLAY_SLIDER10);

            LoadTexture(TX_PLAY_SCOREBAR);
            LoadTexture(TX_PLAY_SCOREBAR_BAR);
            LoadTexture(TX_PLAY_SCOREBAR_KI);
            LoadTexture(TX_PLAY_SCOREBAR_KIDANGER);
            LoadTexture(TX_PLAY_SCOREBAR_KIDANGER2);

            LoadTexture(TX_RANKING_A_LG);
            LoadTexture(TX_RANKING_B_LG);
            LoadTexture(TX_RANKING_C_LG);
            LoadTexture(TX_RANKING_D_LG);
            LoadTexture(TX_RANKING_S_LG);
            LoadTexture(TX_RANKING_SS_LG);
            LoadTexture(TX_RANKING_F_LG);
            LoadTexture(TX_BUTTON_BIG);

            LoadTexture(TX_GAMEOVER_BG);
            LoadTexture(TX_PAUSE_BG);

            CreateRectangularTexture(TX_PLAY_DARKEN_OVERLAY, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_Color({0, 0, 0, floor(Settings::get_float("bgdim") / 100 * 255)}));
            break;
        case MODE_SONGSELECT:
            LoadTexture(TX_LOGO);
            LoadTexture(TX_MENU_BG);
            LoadTexture(TX_BUTTON_BIG);
            LoadTexture(TX_BUTTON_MED);
            LoadTexture(TX_BUTTON_SM);
            LoadTexture(TX_BUTTON_SM_ACTIVE);
            LoadTexture(TX_BUTTON_XS);
            LoadTexture(TX_BUTTON_XS_ACTIVE);
            LoadTexture(TX_BUTTON_ARROW);
            LoadTexture(TX_BUTTON_PLAY);

            LoadTexture(TX_RANKING_UNK_SM);
            LoadTexture(TX_RANKING_A_SM);
            LoadTexture(TX_RANKING_B_SM);
            LoadTexture(TX_RANKING_C_SM);
            LoadTexture(TX_RANKING_D_SM);
            LoadTexture(TX_RANKING_S_SM);
            LoadTexture(TX_RANKING_SS_SM);

            LoadTexture(TX_STARS);
            LoadTexture(TX_STARS_FILL);

            CreateRectangularTexture(TX_BEATMAP_ENTRY_BG, 609, 80, SDL_Color({199, 190, 235, 127}));
            CreateRectangularTexture(TX_BEATMAP_ENTRY_EXPANDED_BG, 609, 174, SDL_Color({199, 190, 235, 127}));
            CreateRectangularTexture(TX_SONGLESECT_LOADING_BG, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_Color({255, 255, 255, 229}));
            break;
        case MODE_WELCOME:
            LoadTexture(TX_LOGO);
            LoadTexture(TX_WELCOME_BG);
            break;
        case MODE_SETTINGS:
            LoadTexture(TX_MENU_BG);
            LoadTexture(TX_LOGO);

            LoadTexture(TX_BUTTON_BIG);
            LoadTexture(TX_BUTTON_BIG_ACTIVE);
            LoadTexture(TX_BUTTON_RADIO);
            LoadTexture(TX_BUTTON_SETTINGS_ARROW);
            LoadTexture(TX_BUTTON_SETTINGS_ARROW_ACTIVE);
            LoadTexture(TX_BUTTON_SLIDER_CIRCLE);
            LoadTexture(TX_BUTTON_SLIDER_CIRCLE_ACTIVE);
            LoadTexture(TX_SETTINGS_CONTROL_SELECTOR_1);
            LoadTexture(TX_SETTINGS_CONTROL_SELECTOR_2);
            LoadTexture(TX_SETTINGS_CONTROL_SELECTOR_3);
            LoadTexture(TX_SETTINGS_SLIDER_LINE);
            LoadTexture(TX_SETTINGS_STRING_SELECTOR);
            LoadTexture(TX_SETTINGS_TAB);
            LoadTexture(TX_SETTINGS_TAB_ACTIVE);
            LoadTexture(TX_SETTINGS_PANEL);
            LoadTexture(TX_SETTINGS_PANEL_BG_1);
            LoadTexture(TX_SETTINGS_PANEL_BG_2);
            break;
        case MODE_ABOUT:
            LoadTexture(TX_LOGO);
            LoadTexture(TX_MENU_BG);
            LoadTexture(TX_BUTTON_BIG);
            LoadTexture(TX_BUTTON_BIG_ACTIVE);
            LoadTexture(TX_ABOUT);
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

void GraphicsManager::Draw(TextureType tex, OOInt x, OOInt y, OOUInt width, OOUInt height, DrawOrigin origin, FieldType fieldtype, SDL_Color color, OOUInt alpha, OOInt angle, OOFloat z, const SDL_Rect* uv) {
    std::unique_lock lock_pbs(mut_maptextures);
    OOInt x1, y1, x2, y2;

    // We use "UV Coordinates" as a source rect for render copy, thus being able to draw textures partly.
    if (uv != nullptr) {
        width = uv->w;
        height = uv->h;
    }

    if (fieldtype == FIELD_PLAY) {
        x += (OOInt)PlayXOffset;
        y += (OOInt)PlayYOffset;
    }

    switch (origin) {
        case ORIGIN_TOPLEFT:
            x1 = ForceBounds(x);
            y1 = ForceBounds(y);
            x2 = ForceBounds(x + (OOInt)width);
            y2 = ForceBounds(y + (OOInt)height);
            break;

        case ORIGIN_CENTER:
            width /= 2;
            height /= 2;

            x1 = ForceBounds(x - (OOInt)width);
            x2 = ForceBounds(x + (OOInt)width);
            y1 = ForceBounds(y - (OOInt)height);
            y2 = ForceBounds(y + (OOInt)height);
            break;

        case ORIGIN_CENTERLEFT:
            height /= 2;
            x1 = ForceBounds(x);
            x2 = ForceBounds(x + (OOInt)width);
            y1 = ForceBounds(y - (OOInt)height);
            y2 = ForceBounds(y + (OOInt)height);
            break;

        case ORIGIN_BOTTOMLEFT:
            x1 = ForceBounds(x);
            x2 = ForceBounds(x + (OOInt)width);
            y1 = ForceBounds((SCREEN_HEIGHT) - y - (OOInt)height);
            y2 = ForceBounds((SCREEN_HEIGHT) - y);
            break;

        case ORIGIN_TOPRIGHT:
            x1 = ForceBounds(x - (OOInt)width);
            x2 = ForceBounds(x);
            y1 = ForceBounds(y);
            y2 = ForceBounds(y + (OOInt)height);
    }


    //don't draw things out of the screen
    if (x1 > SCREEN_WIDTH || x2 < 0 || y1 > SCREEN_HEIGHT || y2 < 0)
        return;
    SDL_Rect dst = {x1, y1, x2-x1, y2-y1};

    SDL_Texture* tex_to_draw = maptextures[tex];

    if ((color.r != 0) && (color.g != 0) && (color.b != 0)) {
        SDL_SetTextureColorMod(tex_to_draw, color.r, color.g, color.b);
    }

    SDL_SetTextureAlphaMod(tex_to_draw, alpha);
    SDL_RenderCopyEx(renderer, tex_to_draw, uv, &dst, angle, nullptr, SDL_FLIP_NONE);

    if ((color.r != 0) && (color.g != 0) && (color.b != 0)) {
        SDL_SetTextureColorMod(tex_to_draw, 255, 255, 255);
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

void GraphicsManager::CreateRectangularTexture(TextureType texid, OOUInt width, OOUInt height, SDL_Color c) {
    std::unique_lock lock_pbs(mut_maptextures);

    SDL_Texture* tex;

    uint32_t pf = SDL_GetWindowPixelFormat(window);
    tex = SDL_CreateTexture(renderer, pf, SDL_TEXTUREACCESS_TARGET, (OOInt)width, (OOInt)height);
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

bool GraphicsManager::ScaleSurface(SDL_Surface*& pSurface, const OOInt limit) {
    bool res = true;
    OOInt width = pSurface->w;
    OOInt height = pSurface->h;

    SDL_Rect sourceDimensions;
    sourceDimensions.x = 0;
    sourceDimensions.y = 0;
    sourceDimensions.w = width;
    sourceDimensions.h = height;

    OOFloat scale = (OOFloat)limit / (OOFloat)width;
    OOFloat scaleH = (OOFloat)limit / (OOFloat)height;

    if (scaleH < scale) {
        scale = scaleH;
    }

    SDL_Rect targetDimensions;
    targetDimensions.x = 0;
    targetDimensions.y = 0;
    targetDimensions.w = (OOInt)floor(((OOFloat)width * scale));
    targetDimensions.h = (OOInt)floor(((OOFloat)height * scale));

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

    if (SDL_BlitSurface(pSurface, nullptr, p32BPPSurface, nullptr) < 0) {
        fprintf(stderr, "Error did not blit surface: %s\n", SDL_GetError());
        res = false;
    } else {
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

        if (SDL_BlitScaled(p32BPPSurface, nullptr, pScaleSurface, nullptr) < 0) {
            fprintf(stderr, "Error did not scale surface: %s\n", SDL_GetError());

            SDL_FreeSurface(pScaleSurface);
            pScaleSurface = nullptr;
            res = false;
        } else {
            SDL_FreeSurface(pSurface);

            pSurface = pScaleSurface;
            width = pSurface->w;
            height = pSurface->h;
        }
    }

    SDL_FreeSurface(p32BPPSurface);
    p32BPPSurface = nullptr;
    return res;
}

bool GraphicsManager::CropSurfaceToSquare(SDL_Surface*& loadedSurface) {
    SDL_Rect srcrect;
    srcrect.h = srcrect.w = loadedSurface->h;
    srcrect.y = 0;
    srcrect.x = floor(((OOFloat)loadedSurface->w / 2.f) - ((OOFloat)loadedSurface->h / 2.f));

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