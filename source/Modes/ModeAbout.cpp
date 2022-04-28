#include "ModeAbout.h"
#include "Helpers/AudioManager.h"
#include "DataStorage/I18n.h"

void ModeAbout_OnBtnQuitClick(pDrawable* self, OOInt x, OOInt y) {
    SDL_Event sdlevent;
    sdlevent.type = SDL_QUIT;
    SDL_PushEvent(&sdlevent);
}

void ModeAbout_OnBtnSettingsClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_MENUCLICK);
    ChangeModeOnFrameEnd(MODE_SETTINGS);
}

void ModeAbout_OnBtnAboutClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_CLICK_CLOSE);
    ChangeModeOnFrameEnd(MODE_SONGSELECT);
}

ModeAbout::ModeAbout() {
    GraphicsManager::Graphics().LoadTexturesForMode(MODE_ABOUT);
    pDrawable* spr;
    spr = new pSprite(TX_MENU_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, 0);
    mSpriteManager.Add(spr);

    mLogo = new Logo(176, 145);
    mLogo->AddToSpriteManager(mSpriteManager);

    spr = new pSprite(TX_BUTTON_BIG_ACTIVE, 37, 281, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01);
    spr->OnClick = ModeAbout_OnBtnAboutClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText(I18n::get("btn_about"), FONT_PRIMARY, 175, 308, Skins::get_options().FontColor_MenuButtonActive);
    spr->Z = -0.02;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG, 37, 349, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01);
    spr->OnClick = ModeAbout_OnBtnSettingsClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText(I18n::get("btn_settings"), FONT_PRIMARY, 175, 376, Skins::get_options().FontColor_MenuButton);
    spr->Z = -0.02;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG, 37, 418, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01);
    spr->OnClick = ModeAbout_OnBtnQuitClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText(I18n::get("btn_quit"), FONT_PRIMARY, 175, 445, Skins::get_options().FontColor_MenuButton);
    spr->Z = -0.02;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_ABOUT, 400, 46, 512, 453, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.05);
    mSpriteManager.Add(spr);
}

void ModeAbout::Update() {
    mSpriteManager.Draw();
    mLogo->Update();
}
