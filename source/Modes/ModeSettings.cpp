#include "ModeSettings.h"
#include "Helpers/AudioManager.h"
#include "DataStorage/Skins.h"
#include "DataStorage/I18n.h"

#include "DataStorage/Settings.h"
#include "Graphics/pText.h"
#include "System/TextManager.h"
#include "Helpers/AudioManager.h"


SettingsTab ModeSettings::SwitchTabTo = SettingsTab::SETTINGS_TAB_UNSET;

void ModeSettings_OnBtnQuitClick(pDrawable* self, OOInt x, OOInt y) {
    SDL_Event sdlevent;
    sdlevent.type = SDL_QUIT;
    SDL_PushEvent(&sdlevent);
}

void ModeSettings_OnBtnSettingsClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_CLICK_CLOSE);
    ChangeModeOnFrameEnd(MODE_SONGSELECT);
}

void ModeSettings_OnBtnAboutClick(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_MENUCLICK);
    ChangeModeOnFrameEnd(MODE_ABOUT);
}

void ModeSettings_SwitchTabToGameplay(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_MENUCLICK);
    ModeSettings::SwitchTabTo = SettingsTab::SETTINGS_TAB_GAMEPLAY;
}

void ModeSettings_SwitchTabToGeneral(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_MENUCLICK);
    ModeSettings::SwitchTabTo = SettingsTab::SETTINGS_TAB_GENERAL;
}

void ModeSettings_SwitchTabToSound(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_MENUCLICK);
    ModeSettings::SwitchTabTo = SettingsTab::SETTINGS_TAB_SOUND;
}

ModeSettings::ModeSettings() {
    GraphicsManager::Graphics().LoadTexturesForMode(MODE_SETTINGS);
    TabGeneral();
}

void ModeSettings::InitCommonSprites(int use_bg) {
    Clear();

    pDrawable* spr;
    spr = new pSprite(TX_MENU_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.001);
    mSpriteManager.Add(spr);

    TextureType panelbg = TX_SETTINGS_PANEL;
    if (use_bg == 1) {
        panelbg = TX_SETTINGS_PANEL_BG_1;
    } else if (use_bg == 2) {
        panelbg = TX_SETTINGS_PANEL_BG_2;
    }
    spr = new pSprite(panelbg, 390, 86, 531, 413, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.002);
    mSpriteManager.Add(spr);

    mUIElementsManager.CreateLogo(176, 145);

    spr = new pSprite(TX_BUTTON_BIG, 37, 281, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01);
    spr->OnClick = ModeSettings_OnBtnAboutClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText(I18n::get("btn_about"), FONT_PRIMARY, 175, 308, Skins::get_options().FontColor_MenuButton);
    spr->Z = -0.02;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG_ACTIVE, 37, 349, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01);
    spr->OnClick = ModeSettings_OnBtnSettingsClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText(I18n::get("btn_settings"), FONT_PRIMARY, 175, 376, Skins::get_options().FontColor_MenuButtonActive);
    spr->Z = -0.02;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG, 37, 418, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01);
    spr->OnClick = ModeSettings_OnBtnQuitClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText(I18n::get("btn_quit"), FONT_PRIMARY, 175, 445, Skins::get_options().FontColor_MenuButton);
    spr->Z = -0.02;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);
}


void ModeSettings::InitTabs(SettingsTab active_tab) {
    void(*on_click)(pDrawable* self, OOInt x, OOInt y);
    std::string label;
    pDrawable* spr;

    for (int i = 1; i < SettingsTab::SETTINGS_TAB_MAX; i++) {
        auto t = static_cast<SettingsTab>(i);
        switch (t) {
            case SettingsTab::SETTINGS_TAB_GENERAL:
                on_click = ModeSettings_SwitchTabToGeneral;
                label = I18n::get("settings_tab_general");
                break;
            case SettingsTab::SETTINGS_TAB_GAMEPLAY:
                on_click = ModeSettings_SwitchTabToGameplay;
                label = I18n::get("settings_tab_gameplay");
                break;
            case SettingsTab::SETTINGS_TAB_SOUND:
                on_click = ModeSettings_SwitchTabToSound;
                label = I18n::get("settings_tab_sound");
                break;
            default:
                assert(false);
        }

        spr = new pSprite(t == active_tab ? TX_SETTINGS_TAB_ACTIVE : TX_SETTINGS_TAB, 390+(162*(i-1)), 46, 150, 40,
                          ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.1);
        if (t != active_tab) {
            spr->Clickable = true;
            spr->OnClick = on_click;
        }
        mSpriteManager.Add(spr);

        spr = new pText(label, FONT_PRIMARY, 465 + (162 * (i - 1)), 67,
                        t == active_tab ? Skins::get_options().FontColor_MenuButtonActive : Skins::get_options().FontColor_MenuButton);
        spr->Z = -0.2;
        spr->Origin = ORIGIN_CENTER;
        mSpriteManager.Add(spr);
    }
}

void ModeSettings::TabGeneral() {
    InitCommonSprites(1);
    InitTabs(SettingsTab::SETTINGS_TAB_GENERAL);

    auto * lbl = new pText(I18n::get("settings_appearance"), FONT_PRIMARY_BIG, 413, 108,
                           Skins::get_options().FontColor_SettingsLabel);
    mSpriteManager.Add(lbl);

    mUIElementsManager.CreateStringSelector(413, 175, I18n::get("settings_skin"), "skin");
    mUIElementsManager.CreateValueSlider(413, 175+43, I18n::get("settings_bgdim"), "bgdim");
    mUIElementsManager.CreateValueSlider(413, 175+43+43, I18n::get("settings_hoscale"), "hoscale", true);

    lbl = new pText(I18n::get("settings_system"), FONT_PRIMARY_BIG, 413, 298,
                           Skins::get_options().FontColor_SettingsLabel);
    mSpriteManager.Add(lbl);
    mUIElementsManager.CreateStringSelector(413, 365, I18n::get("settings_language"), "language");
}

void ModeSettings::TabGameplay() {
    InitCommonSprites();
    InitTabs(SettingsTab::SETTINGS_TAB_GAMEPLAY);

    auto * lbl = new pText(I18n::get("settings_mods"), FONT_PRIMARY_BIG, 413, 108,
                           Skins::get_options().FontColor_SettingsLabel);
    mSpriteManager.Add(lbl);
    mUIElementsManager.CreateRadioButton(413, 175, I18n::get("settings_nofail"), "noFail", I18n::get("btn_disable"), I18n::get("btn_enable"));

    lbl = new pText(I18n::get("settings_mods_notice"), FONT_PRIMARY_SMALLER, 413, 197,
                           Skins::get_options().FontColor_SettingsLabelNotice);
    mSpriteManager.Add(lbl);

    lbl = new pText(I18n::get("settings_controls"), FONT_PRIMARY_BIG, 413, 237,
                    Skins::get_options().FontColor_SettingsLabel);
    mSpriteManager.Add(lbl);

    mUIElementsManager.CreateRadioButton(413, 299, I18n::get("settings_vitaUseBackTouch"), "vitaUseBackTouch", I18n::get("btn_front_touch"), I18n::get("btn_back_touch"));
    mUIElementsManager.CreateTernaryButton(413, 299+43, I18n::get("settings_controlScheme"), "controlScheme", TX_SETTINGS_CONTROL_SELECTOR_1, TX_SETTINGS_CONTROL_SELECTOR_2, TX_SETTINGS_CONTROL_SELECTOR_3);

    lbl = new pText(I18n::get("settings_other"), FONT_PRIMARY_BIG, 413, 378,
                    Skins::get_options().FontColor_SettingsLabel);
    mSpriteManager.Add(lbl);

    mUIElementsManager.CreateRadioButton(413, 378+62, I18n::get("settings_enableStacking"), "enableStacking", I18n::get("btn_disable"), I18n::get("btn_enable"));
    lbl = new pText(I18n::get("settings_stacking_notice"), FONT_PRIMARY_SMALLER, 413, 378+62+9,
                    Skins::get_options().FontColor_SettingsLabelNotice);
    mSpriteManager.Add(lbl);
}

void ModeSettings::TabSound() {
    InitCommonSprites(2);
    InitTabs(SettingsTab::SETTINGS_TAB_SOUND);

    auto * lbl = new pText(I18n::get("settings_volume"), FONT_PRIMARY_BIG, 413, 108,
                           Skins::get_options().FontColor_SettingsLabel);
    mSpriteManager.Add(lbl);

    mUIElementsManager.CreateValueSlider(413, 130+41, I18n::get("settings_volume_hitsounds"), "volume_hitsounds");
    mUIElementsManager.CreateValueSlider(413, 197+18, I18n::get("settings_volume_music"), "volume_music");
    mUIElementsManager.CreateValueSlider(413, 241+18, I18n::get("settings_volume_menumusic"), "volume_menumusic");
    mUIElementsManager.CreateValueSlider(413, 285+18, I18n::get("settings_volume_uisounds"), "volume_uisounds");
}

void ModeSettings::Update() {
    switch (ModeSettings::SwitchTabTo) {
        case SETTINGS_TAB_GENERAL:
            TabGeneral();
            break;
        case SETTINGS_TAB_GAMEPLAY:
            TabGameplay();
            break;
        case SETTINGS_TAB_SOUND:
            TabSound();
            break;
        default:
            break;
    }
    SwitchTabTo = SETTINGS_TAB_UNSET;

    mSpriteManager.Draw();
    mUIElementsManager.Update();
}

void ModeSettings::Clear() {
    mSpriteManager.Clear();
    mUIElementsManager.Clear();
}

void ModeSettings::HandleInput() {
    mUIElementsManager.HandleInput();
}

ModeSettings::~ModeSettings() {
    Clear();
    Settings::save();
}
