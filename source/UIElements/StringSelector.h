#pragma once

#include "UIElements/UIElement.h"
#include "Graphics/pText.h"

class StringSelector : public UIElement {
public:
    std::function<void(const std::string&)> value_callback;
    pText* selectedString;
    pSprite* arrowLeft;
    pSprite* arrowRight;
    std::string mValue;
    uint8_t clickedArrowId = 0; // 0 - unset, 1 - left, 2 - right
    bool mTouchActive = false;
    std::vector<std::string> mValues;
    int mValueIndex;

    StringSelector(int32_t x, int32_t y) {
        mX = x;
        mY = y;
        selectedString = new pText("default", FONT_PRIMARY_SMALL, mX + 13, mY, Skins::get_options().FontColor_SettingsLabel);
        selectedString->Origin = ORIGIN_CENTERLEFT;
        selectedString->Z = -0.6f;
        arrowLeft = new pSprite(TX_BUTTON_SETTINGS_ARROW, mX+217+4, mY, 47, 32, ORIGIN_CENTERLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.5);
        arrowRight = new pSprite(TX_BUTTON_SETTINGS_ARROW, mX+217+47, mY, 47, 32, ORIGIN_CENTERLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.5);
        arrowRight->Rotate(180);

        mSprites.push_back(selectedString);
        mSprites.push_back(arrowLeft);
        mSprites.push_back(arrowRight);
    }

    void Init(int valueIndex, const std::vector<std::string>& values) {
        mValues = values;
        mValueIndex = valueIndex;

        mValue = mValues.at(mValueIndex);
        selectedString->Text = mValue;
    }

    bool InBounds(int32_t x, int32_t y) {
        SDL_Rect left_arr = arrowLeft->GetRect();
        SDL_Rect right_arr = arrowRight->GetRect();
        SDL_Point p = {x,y};

        if (SDL_PointInRect(&p, &left_arr)) {
            clickedArrowId = 1;
            return true;
        }

        if (SDL_PointInRect(&p, &right_arr)) {
            clickedArrowId = 2;
            return true;
        }

        clickedArrowId = 0;
        return false;
    }

    void OnTouchDown(const touchPosition& touch) {
        mTouchActive = true;
    }

    bool OnTouch(const touchPosition& touch) {
        if (!mTouchActive) {
            return false;
        }
        if (clickedArrowId == 1) {
            arrowLeft->Texture = TX_BUTTON_SETTINGS_ARROW_ACTIVE;
        } else if (clickedArrowId == 2) {
            arrowRight->Texture = TX_BUTTON_SETTINGS_ARROW_ACTIVE;
        }
        return true;
    }

    bool OnTouchUp(const touchPosition& touch) {
        if (!mTouchActive) {
            return false;
        }

        AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);

        arrowLeft->Texture = TX_BUTTON_SETTINGS_ARROW;
        arrowRight->Texture = TX_BUTTON_SETTINGS_ARROW;

        if (clickedArrowId == 1) {
            mValueIndex = ((mValueIndex - 1) < 0) ? (mValues.size() - 1) : mValueIndex - 1;
        } else if (clickedArrowId == 2) {
            mValueIndex = ((mValueIndex + 1) >= mValues.size()) ? 0 : mValueIndex + 1;
        }

        mValue = mValues.at(mValueIndex);
        selectedString->Text = mValue;

        value_callback(mValue);

        clickedArrowId = 0;
        mTouchActive = false;
        return true;
    }
};
