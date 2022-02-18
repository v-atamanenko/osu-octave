#include "TextManager.h"
#include "SDL_ttf.h"
#include "defines.h"

FC_Font* TextManager::mFonts[NUMBER_OF_FONTS];
FontName TextManager::currentFont;

template <class ... Args>
void valist_FC_Draw(FC_Font* font, FC_Target* dest, float x, float y, const char* formatted_text, Args ... args)
{
    FC_Draw(font, dest, x, y, formatted_text, args...);
}

void TextManager::Init() {
    AddFont(FONT_CONSOLE_BIG, "fonts/Roboto-Regular.ttf", 16);
    AddFont(FONT_CONSOLE_BIG_BOLD, "fonts/Roboto-Medium.ttf", 16);
    AddFont(FONT_CONSOLE, "fonts/Roboto-Regular.ttf", 14);
    AddFont(FONT_CONSOLE_BOLD, "fonts/Roboto-Medium.ttf", 14);
	AddFont(FONT_SCORE, "fonts/PressStart2P-Regular.ttf", 36, SDL_Color({250, 245, 239, 255}));

    AddFont(FONT_PIXEL, "fonts/PressStart2P-Regular.ttf", 20, SDL_Color({67, 19, 115, 255}));
    AddFont(FONT_PIXEL_ACTIVE, "fonts/PressStart2P-Regular.ttf", 20, SDL_Color({250, 245, 239, 255}));

    currentFont = FONT_CONSOLE;
}

void TextManager::AddFont(FontName font, const std::string& path, int ptsize, SDL_Color c) {
    mFonts[font] = FC_CreateFont();
    FC_LoadFont(mFonts[font], renderer, path.c_str(), ptsize, c, TTF_STYLE_NORMAL);
}

void TextManager::SetFont(FontName font) {
    currentFont = font;
}

void TextManager::Print(float x, float y, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    PrintLocate(x, y, ORIGIN_TOPLEFT, fmt, args);
    va_end(args);
}

void TextManager::PrintLocate(float x, float y, DrawOrigin origin, const char *fmt, ...) {
    if (mFonts[currentFont] == nullptr) {
        return;
    }

    unsigned int fc_buffer_size = 1024;
    char fc_buffer[fc_buffer_size];

    int w,h;
    va_list args;

    va_start(args, fmt);
    vsnprintf(fc_buffer, fc_buffer_size, fmt, args);
    va_end(args);

    w = FC_GetWidth(mFonts[currentFont], fc_buffer);
    h = FC_GetHeight(mFonts[currentFont], fc_buffer);

    switch (origin)
    {
        case ORIGIN_TOPLEFT:
            break; // do nothing

        case ORIGIN_CENTER:
            x -= (float)w/2;
            y -= (float)h/2;
            break;

        case ORIGIN_BOTTOMLEFT:
            y -= (float)h;
            break;

        case ORIGIN_TOPRIGHT:
            x -= (float)w;
            break;
    }

    valist_FC_Draw(mFonts[currentFont], renderer, x, y, fc_buffer);
}
