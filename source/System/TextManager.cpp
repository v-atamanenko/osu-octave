#include "TextManager.h"

FC_Font* TextManager::mFonts[NUMBER_OF_FONTS];
std::map<std::string, InMemoryFont> TextManager::mTTF;
FontName TextManager::currentFont;

void TextManager::Init() {
    AddFont(FONT_PRIMARY, Skins::get_res_path(FONT_PRIMARY), 20, SDL_Color({67, 19, 115, 255}));
    currentFont = FONT_PRIMARY;
}

void TextManager::InitDeferred() {
    AddFont(FONT_PRIMARY_BOLD, Skins::get_res_path(FONT_PRIMARY_BOLD), 20, SDL_Color({67, 19, 115, 255}));
    AddFont(FONT_PRIMARY_BIG, Skins::get_res_path(FONT_PRIMARY_BIG), 28, SDL_Color({67, 19, 115, 255}));
    AddFont(FONT_PRIMARY_SMALLER, Skins::get_res_path(FONT_PRIMARY_SMALLER), 16);
    AddFont(FONT_PRIMARY_SMALLER_BOLD, Skins::get_res_path(FONT_PRIMARY_SMALLER_BOLD), 16);
    AddFont(FONT_PRIMARY_SMALL, Skins::get_res_path(FONT_PRIMARY_SMALL), 14);
    AddFont(FONT_PRIMARY_SMALL_BOLD, Skins::get_res_path(FONT_PRIMARY_SMALL_BOLD), 14);
    AddFont(FONT_SCORE, Skins::get_res_path(FONT_SCORE), 36, SDL_Color({250, 245, 239, 255}));
    AddFont(FONT_NUMBERING, Skins::get_res_path(FONT_NUMBERING), 60, SDL_Color({67, 19, 155, 255}));
}

void TextManager::FullReload() {
    Skins::init_skin();

    for (int i = 0; i < NUMBER_OF_FONTS; i++) {
        if (mFonts[i] == nullptr) {
            continue;
        }

        FC_ClearFont(mFonts[i]);
    }

    // No need to free buffers inside, FC_ClearFont does that
    mTTF.clear();

    Init();
    InitDeferred();
}

void TextManager::AddFont(FontName font, const std::string& path, int ptsize, SDL_Color c) {
    if (mTTF.find(path) == mTTF.end()) {
        SDL_RWops *file_rw = SDL_RWFromFile(path.c_str(), "rb");
        OOInt file_sz = file_rw->size(file_rw);
        void *membuf = malloc(file_sz);
        file_rw->read(file_rw, membuf, 1, file_sz);
        file_rw->close(file_rw);

        mTTF[path] = {membuf, file_sz};
    }

    SDL_RWops *frw = SDL_RWFromConstMem(mTTF[path].buf, mTTF[path].size);

    mFonts[font] = FC_CreateFont();
    FC_LoadFont_RW(mFonts[font], renderer, frw, 1, ptsize, c, TTF_STYLE_NORMAL);
    //FC_LoadFont(mFonts[font], renderer, path.c_str(), ptsize, c, TTF_STYLE_NORMAL);
}

void TextManager::SetFont(FontName font) {
    currentFont = font;
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

        case ORIGIN_CENTERLEFT:
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
