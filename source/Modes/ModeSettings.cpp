#include "ModeSettings.h"

uint8_t ModeSettings::SwitchTabTo = 0;

void ModeSettings_OnBtnQuitClick(pDrawable* self, uint16_t x, uint16_t y) {
    SDL_Event sdlevent;
    sdlevent.type = SDL_QUIT;
    SDL_PushEvent(&sdlevent);
}

void ModeSettings_OnBtnSettingsClick(pDrawable* self, uint16_t x, uint16_t y) {
    ChangeModeOnFrameEnd(MODE_SONGSELECT);
}

void ModeSettings_OnBtnAboutClick(pDrawable* self, uint16_t x, uint16_t y) {
    ChangeModeOnFrameEnd(MODE_ABOUT);
}

void ModeSettings_SwitchTabToGameplay(pDrawable* self, uint16_t x, uint16_t y) {
    ModeSettings::SwitchTabTo = 2;
}

void ModeSettings_SwitchTabToGeneral(pDrawable* self, uint16_t x, uint16_t y) {
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

    spr = new pSprite(TX_BUTTON_BIG, 37, 281, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    spr->OnClick = ModeSettings_OnBtnAboutClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText("About", FONT_PIXEL, 175, 308, SDL_Color({67,19,115}));
    spr->Z = -0.02f;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG_ACTIVE, 37, 349, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    spr->OnClick = ModeSettings_OnBtnSettingsClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText("Settings", FONT_PIXEL_ACTIVE, 175, 376, SDL_Color({67,19,115}));
    spr->Z = -0.02f;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG, 37, 418, 277, 55, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.01f);
    spr->OnClick = ModeSettings_OnBtnQuitClick;
    spr->Clickable = true;
    mSpriteManager.Add(spr);
    spr = new pText("Quit", FONT_PIXEL, 175, 445, SDL_Color({67,19,115}));
    spr->Z = -0.02f;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);
}

void ValueSlider_updateDisplayedValue (float val, pText* valLabel) {
    valLabel->Text = std::to_string((int)round(val));
}

void ValueSlider_saveDisplayedValue (float val, const std::string& setting_name) {
    Settings::set_float(setting_name, val);
}

void StringSelector_saveDisplayedValue (const std::string& val, const std::string& setting_name) {
    Settings::set_str(setting_name, val);
}

void RadioButton_saveDisplayedValue (bool val, const std::string& setting_name) {
    Settings::set_bool(setting_name, val);
}

void ModeSettings::CreateRadioButton(int32_t x, int32_t y, const std::string& setting_name,
                                     const std::string& false_value_label, const std::string& true_value_label) {
    auto* rb = new RadioButton(x, y, false_value_label, true_value_label, Settings::get_bool(setting_name));

    std::function<void(bool)> saveval_lambda = [setting_name](bool s) { return RadioButton_saveDisplayedValue(s, setting_name); };
    rb->value_callback = saveval_lambda;

    radioButtons.push_back(rb);
    rb->AddToSpriteManager(mSpriteManager);
}

void ModeSettings::CreateStringSelector(int32_t x, int32_t y, const std::string& setting_name) {
    int current_value_index;

    std::vector<std::string> available_values;
    if (setting_name == "skin") {
        current_value_index = Settings::get_available_skins(available_values);
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

void ModeSettings::CreateValueSlider(int32_t x, int32_t y, const std::string& setting_name) {
    float lastValue = Settings::get_float(setting_name);
    auto* vs = new ValueSlider(x, y);
    vs->Init(lastValue);
    auto* spr = new pText(std::to_string((int)(round(lastValue))),
                          FONT_PIXEL, x+281, y, SDL_Color());
    spr->Z = -1.f;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager.Add(spr);

    std::function<void(float)> updateval_lambda = [spr](float f) { return ValueSlider_updateDisplayedValue(f, spr); };
    vs->value_change_callback = updateval_lambda;

    std::function<void(float)> saveval_lambda = [setting_name](float f) { return ValueSlider_saveDisplayedValue(f, setting_name); };
    vs->value_callback = saveval_lambda;

    valueSliders.push_back(vs);
    vs->AddToSpriteManager(mSpriteManager);
}

void ModeSettings::TabGeneral() {
    InitCommonSprites();

    pDrawable* spr;
    spr = new pSprite(TX_SETTINGS_PANEL_GENERAL, 400, 46, 512, 453, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.1f);
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG, 558, 42, 158, 44, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 0);
    spr->Clickable = true;
    spr->OnClick = ModeSettings_SwitchTabToGameplay;
    mSpriteManager.Add(spr);

    CreateValueSlider(562, 170, "volume_hitsounds");
    CreateValueSlider(562, 214, "volume_music");
    CreateValueSlider(562, 258, "volume_menumusic");
    CreateValueSlider(562, 302, "volume_uisounds");
    CreateStringSelector(562, 393, "skin");
    CreateValueSlider(562, 452, "bgdim");
}

void ModeSettings::TabGameplay() {
    InitCommonSprites();

    pDrawable* spr;
    spr = new pSprite(TX_SETTINGS_PANEL_GAMEPLAY, 400, 46, 512, 453, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.1f);
    mSpriteManager.Add(spr);

    spr = new pSprite(TX_BUTTON_BIG, 393, 42, 158, 44, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 0);
    spr->Clickable = true;
    spr->OnClick = ModeSettings_SwitchTabToGeneral;
    mSpriteManager.Add(spr);

    CreateRadioButton(562, 155, "noFail", "Disable", "Enable");
    CreateRadioButton(562, 302, "vitaUseBackTouch", "Front touch", "Back touch");
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
}

void ModeSettings::HandleInput() {
    touchPosition touch = InputHelper::TouchRead();

    for(auto v : valueSliders) {
        if (InputHelper::KeyDown(Control::IH_CONTROL_ACTION) && v->InBounds(touch.px, touch.py) && ! InputHelper::BlockKeydown) {
            v->OnTouchDown(touch);
            InputHelper::BlockKeydown = true;
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
        if (InputHelper::KeyDown(Control::IH_CONTROL_ACTION) && v->InBounds(touch.px, touch.py) && ! InputHelper::BlockKeydown) {
            v->OnTouchDown(touch);
            InputHelper::BlockKeydown = true;
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

    for(auto v : radioButtons) {
        if (InputHelper::KeyDown(Control::IH_CONTROL_ACTION) && v->InBounds(touch.px, touch.py) && ! InputHelper::BlockKeydown) {
            v->OnTouchDown(touch);
            InputHelper::BlockKeydown = true;
            return;
        }

        if (InputHelper::KeyUp(Control::IH_CONTROL_ACTION)) {
            if (v->OnTouchUp(touch))
                return;
        }
    }
}

ModeSettings::~ModeSettings() {
    Clear();
    Settings::save();
}
