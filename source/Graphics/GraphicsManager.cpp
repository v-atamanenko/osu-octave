#include "GraphicsManager.h"
#include <string>

GraphicsManager GraphicsManager::sGraphicsManager;

SDL_Texture* GraphicsManager::loadTexture( int texid, const std::string& path )
{
    if (textures[texid] != nullptr)
        SDL_DestroyTexture(textures[texid]);

    //The final texture
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == nullptr )
    {
        fprintf( stderr, "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return nullptr;
    }

    if( renderer == nullptr ) {
        fprintf( stderr, "Unable to create texture! Renderer is null.\n" );
        SDL_FreeSurface( loadedSurface );
        return nullptr;
    }

    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if( newTexture == nullptr )
    {
        fprintf( stderr,  "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        SDL_FreeSurface( loadedSurface );
        return nullptr;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );

    textures[texid] = newTexture;
}

void GraphicsManager::resetBg() {
    loadTexture( TX_CURRENT_BG, "gfx/menuBG.png" );
}

void GraphicsManager::loadBgFromSurface(SDL_Surface* bg ) {
    SDL_Surface* optimizedSurface = nullptr;
    SDL_Texture* newTexture = nullptr;

    if (textures[TX_CURRENT_BG] != nullptr) {
        SDL_DestroyTexture(textures[TX_CURRENT_BG]);
    }

    if( !bg )
    {
        fprintf( stderr, "Unable to update background!\n");
        return;
    }

    uint32_t pf = SDL_GetWindowPixelFormat(window);

    optimizedSurface = SDL_ConvertSurface(bg, SDL_AllocFormat(pf), 0 );
    if( optimizedSurface == nullptr )
    {
        fprintf( stderr, "Unable to optimize background! SDL Error: %s\n", SDL_GetError() );
        SDL_FreeSurface(bg);
        return;
    }

    SDL_FreeSurface( bg );

    newTexture = SDL_CreateTextureFromSurface( renderer, optimizedSurface );
    if( newTexture == nullptr )
    {
        fprintf( stderr, "Unable to create texture for bg! SDL Error: %s\n", SDL_GetError() );
    }

    SDL_FreeSurface( optimizedSurface );

    textures[TX_CURRENT_BG] = newTexture;
}

void GraphicsManager::bgDraw() {

    if (textures[TX_CURRENT_BG] != nullptr) {
        if (SDL_RenderCopy(renderer, textures[TX_CURRENT_BG], nullptr, nullptr) < 0) {
            fprintf(stderr, "bgDraw failed with: %s \n", SDL_GetError());
        }
    }
}

void GraphicsManager::clear() {
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
}

void GraphicsManager::present() {
    SDL_RenderPresent(renderer);
}

GraphicsManager::GraphicsManager()
{
    //// LOAD TEXTURES ////
    resetBg();

    loadTexture( TX_PLAY_CIRCLE, "data/textures/circle.png" );
    loadTexture( TX_PLAY_CIRCLEOVERLAY, "data/textures/circleoverlay.png" );
    loadTexture( TX_PLAY_CIRCLEAPPROACH, "data/textures/circleapproach.png" );

    loadTexture( TX_PLAY_DISC, "data/textures/disc.png" );
    loadTexture( TX_PLAY_SLIDERTICK, "data/textures/slidertick.png" );
    loadTexture( TX_PLAY_SLIDERREVERSE, "data/textures/sliderreverse.png" );

    loadTexture( TX_WHITE, "data/textures/white.png" );

    loadTexture( TX_PLAY_SPINNER, "data/textures/spinner.png" );
    loadTexture( TX_PLAY_SPINNERBARS, "data/textures/spinnerbars.png" );
    loadTexture( TX_PLAY_SCOREBAR_BAR, "data/textures/scorebar_colour.png" );

    loadTexture( TX_PLAY_SPINNERBG, "data/textures/spinnerbg.png" );

    loadTexture( TX_PLAY_SLIDERB0, "data/textures/sliderb0.png" );
    loadTexture( TX_PLAY_SLIDERB1, "data/textures/sliderb1.png" );
    loadTexture( TX_PLAY_SLIDERB2, "data/textures/sliderb2.png" );
    loadTexture( TX_PLAY_SLIDERB3, "data/textures/sliderb3.png" );
    loadTexture( TX_PLAY_SLIDERB4, "data/textures/sliderb4.png" );
    loadTexture( TX_PLAY_SLIDERB5, "data/textures/sliderb5.png" );
    loadTexture( TX_PLAY_SLIDERB6, "data/textures/sliderb6.png" );
    loadTexture( TX_PLAY_SLIDERB7, "data/textures/sliderb7.png" );
    loadTexture( TX_PLAY_SLIDERB8, "data/textures/sliderb8.png" );
    loadTexture( TX_PLAY_SLIDERB9, "data/textures/sliderb9.png" );
    loadTexture( TX_PLAY_SLIDERFOLLOW, "data/textures/sliderfollow.png" );

    loadTexture( TX_PLAY_HIT0, "data/textures/hit0.png" );
    loadTexture( TX_PLAY_HIT300, "data/textures/hit300.png" );
    loadTexture( TX_PLAY_HIT300K, "data/textures/hit300k.png" );
    loadTexture( TX_PLAY_HIT300G, "data/textures/hit300g.png" );

    loadTexture( TX_PLAY_HIT50, "data/textures/hit50.png" );
    loadTexture( TX_PLAY_HIT100, "data/textures/hit100.png" );
    loadTexture( TX_PLAY_HIT100K, "data/textures/hit100k.png" );

    loadTexture( TX_PLAY_SLIDER30, "data/textures/slider30.png" );
    loadTexture( TX_PLAY_SLIDER10, "data/textures/slider10.png" );

    loadTexture( TX_PLAY_SCOREBAR_KI, "data/textures/scorebar_ki.png" );
    loadTexture( TX_PLAY_SCOREBAR_KIDANGER, "data/textures/scorebar_kidanger.png" );
    loadTexture( TX_PLAY_SCOREBAR_KIDANGER2, "data/textures/scorebar_kidanger2.png" );

    loadTexture( TX_PLAY_SCOREBAR, "data/textures/scorebar.png" );

    // 16 bit textures

    //LoadGLTexture(TX_SONGSELECT_SONGBG,	GL_RGB, TEXTURE_SIZE_64, TEXTURE_SIZE_512, -1, NULL, songbg_osu_bin);
}

void GraphicsManager::Draw(TextureType tex, int32_t x, int32_t y, uint32_t width, uint32_t height, DrawOrigin origin, FieldType fieldtype, SDL_Color color, uint32_t alpha, int32_t angle, float z, const uint32_t* uv)
{
    if (uv == nullptr)
        uv = textureInfo[tex].uv;

    int32_t x1 = SCREEN_WIDTH, x2 = (SCREEN_WIDTH+100), y1 = SCREEN_HEIGHT, y2 = (SCREEN_HEIGHT+100);
    //float z = zvalue[tex] + deltaz;

    if (fieldtype == FIELD_PLAY)
    {
        x += PlayXOffset;
        y += PlayYOffset;
    }

    switch (origin)
    {
        case ORIGIN_TOPLEFT:
            x1 = ForceBounds(x);
            x2 = ForceBounds(x + width);
            y1 = ForceBounds(y);
            y2 = ForceBounds(y + height);
            break;

        case ORIGIN_CENTER:
            width >>= 1;
            height >>= 1;

            x1 = ForceBounds(x - width);
            x2 = ForceBounds(x + width);
            y1 = ForceBounds(y - height);
            y2 = ForceBounds(y + height);
            break;

        case ORIGIN_BOTTOMLEFT:
            x1 = ForceBounds(x);
            x2 = ForceBounds(x + width);
            y1 = ForceBounds(y - height);
            y2 = ForceBounds(y);
            break;

        case ORIGIN_TOPRIGHT:
            x1 = ForceBounds(x - width);
            x2 = ForceBounds(x);
            y1 = ForceBounds(y);
            y2 = ForceBounds(y + height);
    }

    //don't draw things out of the screen
    if (x1 > SCREEN_WIDTH || x2 < 0 || y1 > SCREEN_HEIGHT || y2 < 0)
        return;
    SDL_Rect dst = {x1, y1, x2-x1, y2-y1};
    SDL_RenderCopyEx( renderer, textures[tex], nullptr, &dst, angle, nullptr, SDL_FLIP_NONE );

    //if (angle != 0)
    //{
        // //Render m_texture to the target texture with an angle
        //   SDL_RenderCopyEx( renderer, m_texture, NULL, NULL, angle, NULL, SDL_FLIP_NONE );
        //
    //}

    /*

    GFX_TEX_COORD = uv[0];
    glVertex2lu1f(x1, y1, z);

    GFX_TEX_COORD = uv[1];
    glVertex2lu1f(x2, y1, z);

    GFX_TEX_COORD = uv[2];
    glVertex2lu1f(x2, y2, z);

    GFX_TEX_COORD = uv[3];
    glVertex2lu1f(x1, y2, z);
    */
}

int32_t GraphicsManager::ForceBounds(int32_t value)
{
    if (value < -200)
        return -200;

    if (value > 1099)
        return 1099;

    return value;
}

