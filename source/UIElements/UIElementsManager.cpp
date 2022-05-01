#include "UIElements/UIElementsManager.h"

#include "UIElements/Logo.h"
#include "UIElements/RadioButton.h"
#include "UIElements/StringSelector.h"
#include "UIElements/TernaryButton.h"
#include "UIElements/ValueSlider.h"

#include "DataStorage/I18n.h"

#include "Modes/ModeSettings.h"

void TernaryButton_saveDisplayedValue (uint8_t val, const std::string& setting_name);
void RadioButton_saveDisplayedValue (bool val, const std::string& setting_name);
void StringSelector_saveDisplayedValue (const std::string& val, const std::string& setting_name);
void ValueSlider_saveDisplayedValue (OOFloat val, const std::string& setting_name);
void ValueSlider_updateDisplayedValue (OOFloat val, pText* valLabel, bool multiplier_mode=false);

UIElementsManager::UIElementsManager(SpriteManager* sprite_manager) {
    mSpriteManager = sprite_manager;
}

UIElementsManager::~UIElementsManager() {
    Clear();
}

void UIElementsManager::Update() {
    for (auto ele : mUIElements) {
        ele->Update();
    }
}

void UIElementsManager::Clear() {
    for (auto ele : mUIElements) {
        delete ele;
    }

    mUIElements.clear();
}

void UIElementsManager::HandleInput() {
    touchPosition touch = InputHelper::TouchRead();

    for(auto v : mUIElements) {
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
}

void UIElementsManager::CreateLogo(OOInt x, OOInt y, bool start_animation) {
    auto* logo = new Logo(176, 145, start_animation);
    logo->AddToSpriteManager(mSpriteManager);
}

void UIElementsManager::CreateRadioButton(OOInt x, OOInt y, const std::string& label, const std::string& setting_name,
                                     const std::string& false_value_label, const std::string& true_value_label) {
    auto* rb = new RadioButton(x+159, y, false_value_label, true_value_label, Settings::get_bool(setting_name));

    std::function<void(bool)> saveval_lambda = [setting_name](bool s) { return RadioButton_saveDisplayedValue(s, setting_name); };
    rb->value_callback = saveval_lambda;

    mUIElements.emplace_back(rb);
    rb->AddToSpriteManager(mSpriteManager);

    pText* spr;

    spr = new pText(label, FONT_PRIMARY, x, y, Skins::get_options().FontColor_SettingsLabel);
    spr->Z = -1.0;
    spr->Origin = ORIGIN_CENTERLEFT;
    mSpriteManager->Add(spr);
}

void UIElementsManager::CreateTernaryButton(OOInt x, OOInt y, const std::string& label, const std::string& setting_name,
                                       TextureType bg1, TextureType bg2, TextureType bg3) {
    auto* rb = new TernaryButton(x+159, y, Settings::get_int(setting_name), bg1, bg2, bg3);

    std::function<void(uint8_t)> saveval_lambda = [setting_name](uint8_t s) { return TernaryButton_saveDisplayedValue(s, setting_name); };
    rb->value_callback = saveval_lambda;

    mUIElements.emplace_back(rb);
    rb->AddToSpriteManager(mSpriteManager);

    pText* spr;

    spr = new pText(label, FONT_PRIMARY, x, y, Skins::get_options().FontColor_SettingsLabel);
    spr->Z = -1.0;
    spr->Origin = ORIGIN_CENTERLEFT;
    mSpriteManager->Add(spr);
}

void UIElementsManager::CreateStringSelector(OOInt x, OOInt y, const std::string& label, const std::string& setting_name) {
    OOInt current_value_index;

    std::vector<std::string> available_values;
    if (setting_name == "skin") {
        current_value_index = Skins::get_available_skins(available_values);
    } else if (setting_name == "language") {
        current_value_index = I18n::get_available_langs(available_values);
    } else {
        assert(false); // StringSelectors are only used for skin selection for now.
    }

    auto* ss = new StringSelector(x+176, y);
    ss->Init(current_value_index, available_values);

    std::function<void(const std::string& s)> saveval_lambda = [setting_name](const std::string& s) { return StringSelector_saveDisplayedValue(s, setting_name); };
    ss->value_callback = saveval_lambda;

    mUIElements.emplace_back(ss);
    ss->AddToSpriteManager(mSpriteManager);

    pText* spr;

    spr = new pText(label, FONT_PRIMARY, x, y, Skins::get_options().FontColor_SettingsLabel);
    spr->Z = -1.0;
    spr->Origin = ORIGIN_CENTERLEFT;
    mSpriteManager->Add(spr);

    auto* bg = new pSprite(TX_SETTINGS_STRING_SELECTOR, x+176, y, 200, 32, ORIGIN_CENTERLEFT, FIELD_SCREEN, {}, 255, -0.11);
    mSpriteManager->Add(bg);
}

void UIElementsManager::CreateValueSlider(OOInt x, OOInt y, const std::string& label, const std::string& setting_name, bool multiplier_mode) {
    OOFloat lastValue = Settings::get_float(setting_name);
    auto* vs = new ValueSlider(x+176, y);
    vs->Init(lastValue);
    pText* spr;

    spr = new pText(label, FONT_PRIMARY, x, y, Skins::get_options().FontColor_SettingsLabel);
    spr->Z = -1.0;
    spr->Origin = ORIGIN_CENTERLEFT;
    mSpriteManager->Add(spr);

    auto* line = new pSprite(TX_SETTINGS_SLIDER_LINE, x+176, y-1, 199, 4, ORIGIN_TOPLEFT, FIELD_SCREEN, {}, 255, -0.2);
    mSpriteManager->Add(line);

    if (multiplier_mode) {
        char value_str[8];
        snprintf(value_str, 8, "%.2fx", (OOFloat)((OOInt)((lastValue+100) * 1 + .5)) / 100.f);
        spr = new pText(value_str, FONT_PRIMARY, x + 176 + 266, y, Skins::get_options().FontColor_SettingsLabel);
    } else {
        spr = new pText(std::to_string((OOInt)(round(lastValue))), FONT_PRIMARY, x + 176 + 266, y, Skins::get_options().FontColor_SettingsLabel);
    }

    spr->Z = -1.0;
    spr->Origin = ORIGIN_CENTER;
    mSpriteManager->Add(spr);

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

    mUIElements.emplace_back(vs);
    vs->AddToSpriteManager(mSpriteManager);
}


void ValueSlider_updateDisplayedValue (OOFloat val, pText* valLabel, bool multiplier_mode) {
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
    std::string val_old = Settings::get_str(setting_name);
    Settings::set_str(setting_name, val);

    if (setting_name == "skin") {
        Skins::init_skin();
        AudioManager::Engine().ResetSamples();
    }

    if (setting_name == "language") {
        I18n::load();

        // Chinese language uses its own font, so we force font reload here
        if (val_old == "Chinese" || val == "Chinese") {
            TextManager::FullReload();
        }
    }

    // FIXME: This is very hacky way to force redraw settings
    ModeSettings::SwitchTabTo = SETTINGS_TAB_GENERAL;
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
