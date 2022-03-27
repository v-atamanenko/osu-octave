#include "ModeAbout.h"
#include "Helpers/AudioManager.h"

void ModeAbout_OnBtnQuitClick(pDrawable* self, uint16_t x, uint16_t y) {
    SDL_Event sdlevent;
    sdlevent.type = SDL_QUIT;
    SDL_PushEvent(&sdlevent);
}

void ModeAbout_OnBtnSettingsClick(pDrawable* self, uint16_t x, uint16_t y) {
    AudioManager::Engine().PlayUISound(UISOUND_MENUCLICK);
    ChangeModeOnFrameEnd(MODE_SETTINGS);
}

void ModeAbout_OnBtnAboutClick(pDrawable* self, uint16_t x, uint16_t y) {
    AudioManager::Engine().PlayUISound(UISOUND_CLICK_CLOSE);
    ChangeModeOnFrameEnd(MODE_SONGSELECT);
}

ModeAbout::ModeAbout() {
    GraphicsManager::Graphics().LoadTexturesForMode(MODE_ABOUT);
    pDrawable* spr;
    spr = new pSprite(TX_MENU_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, 0);
    mSpriteManager.Add(spr);

    mLogo = new pSprite(TX_LOGO, 176, 145, 224, 123, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    mSpriteManager.Add(mLogo);

    spr = new pSprite(TX_BUTTON_BIG_ACTIVE, 37, 281, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    spr->OnClick = ModeAbout_OnBtnAboutClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText("About", FONT_PIXEL_ACTIVE, 175, 308, SDL_Color({67,19,115}));
    spr->Z = -0.02f;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG, 37, 349, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    spr->OnClick = ModeAbout_OnBtnSettingsClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText("Settings", FONT_PIXEL, 175, 376, SDL_Color({67,19,115}));
    spr->Z = -0.02f;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG, 37, 418, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    spr->OnClick = ModeAbout_OnBtnQuitClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText("Quit", FONT_PIXEL, 175, 445, SDL_Color({67,19,115}));
    spr->Z = -0.02f;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_ABOUT, 400, 46, 512, 453, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.05f);
    mSpriteManager.Add(spr);
}

void ModeAbout::Update() {
    mSpriteManager.Draw();
}