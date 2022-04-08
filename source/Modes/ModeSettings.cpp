#include "ModeSettings.h"
#include "Helpers/AudioManager.h"
#include "DataStorage/Skins.h"

uint8_t ModeSettings::SwitchTabTo = 0;

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
    ModeSettings::SwitchTabTo = 2;
}

void ModeSettings_SwitchTabToGeneral(pDrawable* self, OOInt x, OOInt y) {
    AudioManager::Engine().PlayUISound(UISOUND_MENUCLICK);
    ModeSettings::SwitchTabTo = 1;
}

ModeSettings::ModeSettings() {
    GraphicsManager::Graphics().LoadTexturesForMode(MODE_SETTINGS);
    TabGeneral();
}

void ModeSettings::InitCommonSprites() {
    Clear();

    pDrawable* spr;
    spr = new pSprite(TX_MENU_BG, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, 0);
    mSpriteManager.Add(spr);

    mLogo = new Logo(176, 145);
    mLogo->AddToSpriteManager(mSpriteManager);

    spr = new pSprite(TX_BUTTON_BIG, 37, 281, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01);
    spr->OnClick = ModeSettings_OnBtnAboutClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText("About", FONT_PIXEL, 175, 308, SDL_Color({67,19,115}));
    spr->Z = -0.02;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG_ACTIVE, 37, 349, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01);
    spr->OnClick = ModeSettings_OnBtnSettingsClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText("Settings", FONT_PIXEL, 175, 376, SDL_Color({255,255,255}));
    spr->Z = -0.02;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG, 37, 418, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01);
    spr->OnClick = ModeSettings_OnBtnQuitClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText("Quit", FONT_PIXEL, 175, 445, SDL_Color({67,19,115}));
    spr->Z = -0.02;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);
}

void ValueSlider_updateDisplayedValue (OOFloat val, pText* valLabel, bool multiplier_mode=false) {
    if (multiplier_mode) {
        char value_str[8];
        snprintf(value_str, 8, "%.2fx", (OOFloat)((OOInt)((val+100) * 1 + .5)) / 100);
        valLabel->Text = value_str;
    } else {
        valLabel->Text = std::to_string((OOInt)round(val));
    }
}

void ValueSlider_saveDisplayedValue (OOFloat val, const std::string& setting_name) {
    Settings::set_float(setting_name, val);
    if (setting_name == "volume_menumusic") {
        AudioManager::Engine().UpdateMusicVolume(val);
    }
}

void StringSelector_saveDisplayedValue (const std::string& val, const std::string& setting_name) {
    Settings::set_str(setting_name, val);

    if (setting_name == "skin") {
        Skins::init_skin();
        AudioManager::Engine().ResetSamples();
    }
}

void RadioButton_saveDisplayedValue (bool val, const std::string& setting_name) {
    Settings::set_bool(setting_name, val);
}


void TernaryButton_saveDisplayedValue (uint8_t val, const std::string& setting_name) {
    Settings::set_int(setting_name, val);

    if (setting_name == "controlScheme") {
        bool temp = InputHelper::vitaUseBackTouch;

        Settings::update_action_controls(val);
        Settings::save();
        InputHelper::InitInput();

        InputHelper::vitaUseBackTouch = temp;
    } else {
        // TernaryButtons are only used for control scheme selection for now.
    }
}



void ModeSettings::CreateRadioButton(OOInt x, OOInt y, const std::string& setting_name,
                                     const std::string& false_value_label, const std::string& true_value_label) {
    auto* rb = new RadioButton(x, y, false_value_label, true_value_label, Settings::get_bool(setting_name));

    std::function<void(bool)> saveval_lambda = [setting_name](bool s) { return RadioButton_saveDisplayedValue(s, setting_name); };
    rb->value_callback = saveval_lambda;

    radioButtons.push_back(rb);
    rb->AddToSpriteManager(mSpriteManager);
}

void ModeSettings::CreateTernaryButton(OOInt x, OOInt y, const std::string& setting_name,
                                       TextureType bg1, TextureType bg2, TextureType bg3) {
    auto* rb = new TernaryButton(x, y, Settings::get_int(setting_name), bg1, bg2, bg3);

    std::function<void(uint8_t)> saveval_lambda = [setting_name](uint8_t s) { return TernaryButton_saveDisplayedValue(s, setting_name); };
    rb->value_callback = saveval_lambda;

    ternaryButtons.push_back(rb);
    rb->AddToSpriteManager(mSpriteManager);
}

void ModeSettings::CreateStringSelector(OOInt x, OOInt y, const std::string& setting_name) {
    OOInt current_value_index;

    std::vector<std::string> available_values;
    if (setting_name == "skin") {
        current_value_index = Skins::get_available_skins(available_values);
    } else {
        assert(false); // StringSelectors are only used for skin selection for now.
    }

    auto* ss = new StringSelector(x, y);
    ss->Init(current_value_index, available_values);

    std::function<void(const std::string& s)> saveval_lambda = [setting_name](const std::string& s) { return StringSelector_saveDisplayedValue(s, setting_name); };
    ss->value_callback = saveval_lambda;

    stringSelectors.push_back(ss);
    ss->AddToSpriteManager(mSpriteManager);
}

void ModeSettings::CreateValueSlider(OOInt x, OOInt y, const std::string& setting_name, bool multiplier_mode) {
    OOFloat lastValue = Settings::get_float(setting_name);
    auto* vs = new ValueSlider(x, y);
    vs->Init(lastValue);
    pText* spr;

    if (multiplier_mode) {
        char value_str[8];
        snprintf(value_str, 8, "%.2fx", (OOFloat)((OOInt)((lastValue+100) * 1 + .5)) / 100.f);
        spr = new pText(value_str,FONT_PIXEL, x+266, y, SDL_Color({67,19,115}));
    } else {
        spr = new pText(std::to_string((OOInt)(round(lastValue))), FONT_PIXEL, x+266, y, SDL_Color({67,19,115}));
    }

    spr->Z = -1.0;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    if (multiplier_mode) {
        std::function<void(OOFloat)> updateval_lambda = [spr](OOFloat f) {
            return ValueSlider_updateDisplayedValue(f, spr, true);
        };
        vs->value_change_callback = updateval_lambda;
    } else {
        std::function<void(OOFloat)> updateval_lambda = [spr](OOFloat f) {
            return ValueSlider_updateDisplayedValue(f, spr);
        };
        vs->value_change_callback = updateval_lambda;
    }

    std::function<void(OOFloat)> saveval_lambda = [setting_name](OOFloat f) { return ValueSlider_saveDisplayedValue(f, setting_name); };
    vs->value_callback = saveval_lambda;

    valueSliders.push_back(vs);
    vs->AddToSpriteManager(mSpriteManager);
}

void ModeSettings::TabGeneral() {
    InitCommonSprites();

    pDrawable* spr;
    spr = new pSprite(TX_SETTINGS_PANEL_GENERAL, 390, 46, 531, 453, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.1);
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG, 558, 42, 158, 44, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 0);
    spr->Clickable = true;
    spr->OnClick = ModeSettings_SwitchTabToGameplay;
    mSpriteManager.Add(spr);

    CreateValueSlider(589, 153, "volume_hitsounds");
    CreateValueSlider(589, 197, "volume_music");
    CreateValueSlider(589, 241, "volume_menumusic");
    CreateValueSlider(589, 285, "volume_uisounds");
    CreateStringSelector(589, 360, "skin");
    CreateValueSlider(589, 419, "bgdim");
    CreateValueSlider(589, 463, "hoscale", true);
}

void ModeSettings::TabGameplay() {
    InitCommonSprites();

    pDrawable* spr;
    spr = new pSprite(TX_SETTINGS_PANEL_GAMEPLAY, 390, 46, 531, 453, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.1);
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG, 393, 42, 158, 44, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 0);
    spr->Clickable = true;
    spr->OnClick = ModeSettings_SwitchTabToGeneral;
    mSpriteManager.Add(spr);

    CreateRadioButton(572, 155, "noFail", "Disable", "Enable");
    CreateRadioButton(572, 280, "vitaUseBackTouch", "Front touch", "Back touch");
    CreateTernaryButton(572, 326, "controlScheme", TX_SETTINGS_CONTROL_SELECTOR_1, TX_SETTINGS_CONTROL_SELECTOR_2, TX_SETTINGS_CONTROL_SELECTOR_3);
    CreateRadioButton(572, 429, "enableStacking", "Disable", "Enable");
}

void ModeSettings::Update() {
    switch (ModeSettings::SwitchTabTo) {
        case 1:
            TabGeneral();
            SwitchTabTo = 0;
            break;
        case 2:
            TabGameplay();
            SwitchTabTo = 0;
            break;
        default:
            break;
    }

    mSpriteManager.Draw();
    mLogo->Update();
}

void ModeSettings::Clear() {
    mSpriteManager.Clear();
    for (auto v : valueSliders) {
        delete v;
    }
    valueSliders.clear();

    for (auto v : stringSelectors) {
        delete v;
    }
    stringSelectors.clear();

    for (auto v : radioButtons) {
        delete v;
    }
    radioButtons.clear();

    for (auto v : ternaryButtons) {
        delete v;
    }
    ternaryButtons.clear();

    delete mLogo;
}

void ModeSettings::HandleInput() {
    touchPosition touch = InputHelper::TouchRead();

    for(auto v : valueSliders) {
        if (InputHelper::KeyDown(Control::IH_CONTROL_ACTION) && v->InBounds(touch.px, touch.py)) {
            v->OnTouchDown(touch);
            return;
        }
        if (InputHelper::KeyHeld(Control::IH_CONTROL_ACTION)) {
            if (v->OnTouch(touch))
                return;
        }

        if (InputHelper::KeyUp(Control::IH_CONTROL_ACTION)) {
            if (v->OnTouchUp(touch))
                return;
        }
    }

    for(auto v : stringSelectors) {
        if (InputHelper::KeyDown(Control::IH_CONTROL_ACTION) && v->InBounds(touch.px, touch.py)) {
            v->OnTouchDown(touch);
            return;
        }

        if (InputHelper::KeyHeld(Control::IH_CONTROL_ACTION)) {
            if (v->OnTouch(touch))
                return;
        }

        if (InputHelper::KeyUp(Control::IH_CONTROL_ACTION) && v->InBounds(touch.px, touch.py)) {
            if (v->OnTouchUp(touch))
                return;
        }
    }

    for(auto v : radioButtons) {
        if (InputHelper::KeyDown(Control::IH_CONTROL_ACTION) && v->InBounds(touch.px, touch.py)) {
            v->OnTouchDown(touch);
            return;
        }

        if (InputHelper::KeyUp(Control::IH_CONTROL_ACTION) && v->InBounds(touch.px, touch.py)) {
            if (v->OnTouchUp(touch))
                return;
        }
    }

    for(auto v : ternaryButtons) {
        if (InputHelper::KeyDown(Control::IH_CONTROL_ACTION) && v->InBounds(touch.px, touch.py)) {
            v->OnTouchDown(touch);
            return;
        }

        if (InputHelper::KeyUp(Control::IH_CONTROL_ACTION) && v->InBounds(touch.px, touch.py)) {
            if (v->OnTouchUp(touch))
                return;
        }
    }
}

ModeSettings::~ModeSettings() {
    Clear();
    Settings::save();
}
