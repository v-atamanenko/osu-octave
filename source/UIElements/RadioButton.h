#pragma once

#include "UIElements/UIElement.h"
#include "Graphics/pText.h"

class RadioButton : public UIElement {
public:
    const int32_t mWidth = 270;
    const int32_t mHeight = 32;

    std::function<void(bool)> value_callback;

    pText* falseValueLabel;
    pText* trueValueLabel;

    pSprite* bg;

    bool mValue;
    bool mTouchActive = false;

    RadioButton(int32_t x, int32_t y, const std::string& false_value_label, const std::string& true_value_label, bool value) {
        mX = x;
        mY = y;
        mValue = value;

        bg = new pSprite(TX_BUTTON_RADIO, mX, mY, mWidth, mHeight, ORIGIN_CENTERLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.5f);
        falseValueLabel = new pText(false_value_label, FONT_PRIMARY, mX + 70, mY, Skins::get_options().FontColor_SettingsLabel);
        falseValueLabel->Z = -0.6f;
        falseValueLabel->Origin = ORIGIN_CENTER;
        trueValueLabel = new pText(true_value_label, FONT_PRIMARY, mX + 200, mY, Skins::get_options().FontColor_SettingsLabel);
        trueValueLabel->Z = -0.6f;
        trueValueLabel->Origin = ORIGIN_CENTER;

        if (mValue) {
            bg->Angle = 180;
            trueValueLabel->Color = Skins::get_options().FontColor_SettingsLabelActive;
        } else {
            falseValueLabel->Color = Skins::get_options().FontColor_SettingsLabelActive;
        }

        mSprites.push_back(falseValueLabel);
        mSprites.push_back(trueValueLabel);
        mSprites.push_back(bg);
    }

    bool InBounds(OOInt x, OOInt y) {
        SDL_Rect r = SDL_Rect({mX, mY, mWidth, mHeight});
        SDL_Point p = {x,y};

        if (SDL_PointInRect(&p, &r)) {
            return true;
        }

        return false;
    }

    void OnTouchDown(const touchPosition& touch) {
        mTouchActive = true;
    }

    bool OnTouchUp(const touchPosition& touch) {
        if (!mTouchActive) {
            return false;
        }

        mValue = !mValue;

        if (mValue) {
            AudioManager::Engine().PlayUISound(UISOUND_CHECK_ON);
            bg->Angle = 180;
            trueValueLabel->Color = Skins::get_options().FontColor_SettingsLabelActive;
            falseValueLabel->Color = Skins::get_options().FontColor_SettingsLabel;
        } else {
            AudioManager::Engine().PlayUISound(UISOUND_CHECK_OFF);
            bg->Angle = 0;
            trueValueLabel->Color = Skins::get_options().FontColor_SettingsLabel;
            falseValueLabel->Color = Skins::get_options().FontColor_SettingsLabelActive;
        }

        value_callback(mValue);

        mTouchActive = false;
        return true;
    }
};
