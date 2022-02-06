#include "TextManager.h"
#include "SDL_ttf.h"
#include "defines.h"

TextManager TextManager::sTM;

TextManager::TextManager()
{
}

void TextManager::initTex()
{
    int w_w, w_h;
    uint32_t pf;

    SDL_GetWindowSize(window, &w_w, &w_h);
    pf = SDL_GetWindowPixelFormat(window);
    sTM.mConsole = SDL_CreateTexture(renderer, pf, SDL_TEXTUREACCESS_TARGET, w_w, w_h);
    SDL_SetTextureBlendMode(sTM.mConsole, SDL_BLENDMODE_BLEND);
    sTM.Clear();
}

void TextManager::drawTex()
{
    if (sTM.mConsole == nullptr) {
        sTM.initTex();
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, sTM.mConsole, nullptr, nullptr);
}

void TextManager::updateTex(char *text, SDL_Color color, int x, int y, DrawOrigin origin)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sTM.mFonts[sTM.currentFont], text, color);
    free(text);

    if (surfaceMessage == nullptr) {
        fprintf(stderr, "Failed to create surfaceMessage: %s\n", SDL_GetError());
    }

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if (Message == nullptr) {
        fprintf(stderr, "Failed to create Message texture: %s\n", SDL_GetError());
    }

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(Message, NULL, NULL, &texW, &texH);

    switch (origin)
    {
        case ORIGIN_TOPLEFT:
            break; // do nothing

        case ORIGIN_CENTER:
            x = (int) floor((float)(((float)SCREEN_WIDTH / 2) - ((float)texW / 2)))+x;
            y = (int) floor((float)(((float)SCREEN_HEIGHT / 2) - ((float)texH / 2)))+y;
            break;

        case ORIGIN_BOTTOMLEFT:
            y = SCREEN_HEIGHT - texH - y;
            break;

        case ORIGIN_TOPRIGHT:
            x -= texW;
            break;
    }

    SDL_Rect Message_rect = {x,y,texW,texH};

    if (sTM.mConsole == nullptr) {
        sTM.initTex();
    }

    SDL_SetRenderTarget( renderer, sTM.mConsole );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_RenderCopy(renderer, Message, nullptr, &Message_rect);
    SDL_SetRenderTarget( renderer, nullptr );

    // Don't forget to free your surface and texture
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(surfaceMessage);
}

void TextManager::Init()
{
	//TODO: Use different fonts or remove rendundant font types
	AddFont(FONT_CONSOLE, "fonts/verdana.ttf");
	AddFont(FONT_SCORE, "fonts/verdana.ttf", 32);
	AddFont(FONT_NUMBERING, "fonts/verdana.ttf");
	AddFont(FONT_VERDANA, "fonts/verdana.ttf");

    AddFont(FONT_PIXEL, "fonts/PressStart2P-Regular.ttf", 14);


    sTM.currentFont = FONT_VERDANA;
}

void TextManager::AddFont(FONT font, std::string path, int ptsize)
{
    sTM.mFonts[font] = TTF_OpenFont(path.c_str(), ptsize);
}

void TextManager::Warn(char* format, ...)
{
    char* message;
    va_list args;
    va_start(args, format);
    SDL_asprintf(&message, format, args);
    va_end(args);

    updateTex(message, SDL_Color({255, 25, 25, 255}), 0, 0);
}

void TextManager::SetFont(FONT font)
{
    currentFont = font;
}

void TextManager::Print(const char* format, ...)
{
	char* message;
	va_list args;
	va_start(args, format);
	SDL_asprintf(&message, format, args);
	va_end(args);

    updateTex(message, SDL_Color(), 0, 0);
}

void TextManager::Clear()
{
    SDL_SetRenderTarget( renderer, mConsole );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, NULL);
    SDL_SetRenderTarget( renderer, nullptr );
}

void TextManager::PrintLocate(int x, int y, DrawOrigin origin, SDL_Color clr, const char* txt)
{
    char* message;
    SDL_asprintf(&message, "%s", txt);
    updateTex(message, clr, x, y, origin);
}


void TextManager::PrintScore(int x, int y, DrawOrigin origin, const char* format, uint32_t score)
{
    char* message;
    SDL_asprintf(&message, format, score);
    updateTex(message, SDL_Color({255, 255, 255, 255}), x, y, origin);
}

void TextManager::PrintScore(int x, int y, DrawOrigin origin, const char* format, float score)
{
    char* message;
    SDL_asprintf(&message, format, score);
    updateTex(message, SDL_Color({255, 255, 255, 255}), x, y, origin);
}

void TextManager::PrintScore(int x, int y, DrawOrigin origin, const char* format, char * score)
{
    char* message;
    SDL_asprintf(&message, format, score);
    updateTex(message, SDL_Color({255, 255, 255, 255}), x, y, origin);
}

