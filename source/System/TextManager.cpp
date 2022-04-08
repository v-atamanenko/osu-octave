#include "TextManager.h"

FC_Font* TextManager::mFonts[NUMBER_OF_FONTS];
FontName TextManager::currentFont;

void TextManager::Init() {
    std::string path = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/fonts/";
    AddFont(FONT_PIXEL, Skins::get_res_path(FONT_PIXEL), 20, SDL_Color({67, 19, 115, 255}));
    currentFont = FONT_PIXEL;
}

void TextManager::InitDeferred() {
    std::string path = std::string(DEF_DataDirectory) + std::string(DEF_SkinsSubdirectory) + Settings::get_str("skin") + "/fonts/";
    AddFont(FONT_CONSOLE_BIG, Skins::get_res_path(FONT_CONSOLE_BIG), 16);
    AddFont(FONT_CONSOLE_BIG_BOLD,Skins::get_res_path(FONT_CONSOLE_BIG_BOLD), 16);
    AddFont(FONT_CONSOLE, Skins::get_res_path(FONT_CONSOLE), 14);
    AddFont(FONT_CONSOLE_BOLD, Skins::get_res_path(FONT_CONSOLE_BOLD), 14);
    AddFont(FONT_SCORE, Skins::get_res_path(FONT_SCORE), 36, SDL_Color({250, 245, 239, 255}));

    AddFont(FONT_NUMBERING, Skins::get_res_path(FONT_NUMBERING), 60, SDL_Color({67, 19, 155, 255}));

    currentFont = FONT_CONSOLE;
}

void TextManager::AddFont(FontName font, const std::string& path, int ptsize, SDL_Color c) {
    mFonts[font] = FC_CreateFont();
    FC_LoadFont(mFonts[font], renderer, path.c_str(), ptsize, c, TTF_STYLE_NORMAL);
}

void TextManager::SetFont(FontName font) {
    currentFont = font;
}

void TextManager::Print(OOInt x, OOInt y, const char *fmt, ...) {
    unsigned int fc_buffer_size = 1024;
    char fc_buffer[fc_buffer_size];

    va_list args;

    va_start(args, fmt);
    vsnprintf(fc_buffer, fc_buffer_size, fmt, args);
    va_end(args);

    PrintLocate(x, y, ORIGIN_TOPLEFT, "%s", fc_buffer);
}

void TextManager::PrintLocate(OOInt x, OOInt y, DrawOrigin origin, const char *fmt, ...) {
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
            x -= w/2;
            y -= h/2;
            break;

        case ORIGIN_BOTTOMLEFT:
            y -= h;
            break;

        case ORIGIN_TOPRIGHT:
            x -= w;
            break;
    }
    FC_Draw(mFonts[currentFont], renderer, (float)x, (float)y, "%s", fc_buffer);
    //valist_FC_Draw(mFonts[currentFont], renderer, x, y, fc_buffer);
}

void TextManager::PrintLocateColor(OOInt x, OOInt y, DrawOrigin origin, SDL_Color clr, const char *fmt, ...) {
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
            x -= w/2;
            y -= h/2;
            break;

        case ORIGIN_BOTTOMLEFT:
            y -= h;
            break;

        case ORIGIN_TOPRIGHT:
            x -= w;
            break;
    }

    FC_DrawColor(mFonts[currentFont], renderer, (float)x, (float)y, clr, "%s", fc_buffer);
}
