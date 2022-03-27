#ifndef _MODESETTINGS_H
#define _MODESETTINGS_H

#include "DataStorage/Settings.h"
#include "Graphics/pText.h"
#include "Graphics/SpriteManager.h"
#include "System/TextManager.h"
#include "Graphics/SpriteContainer.h"
#include "Helpers/AudioManager.h"
#include "UIElements/Logo.h"

class StringSelector : public SpriteContainer {
public:
    int32_t mX;
    int32_t mY;
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
        selectedString = new pText("default", FONT_CONSOLE, mX+18, mY+8);
        selectedString->Z = -0.6f;
        arrowLeft = new pSprite(TX_BUTTON_SETTINGS_ARROW, mX+232+4, mY, 47, 32, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.5);
        arrowRight = new pSprite(TX_BUTTON_SETTINGS_ARROW, mX+232+47, mY, 47, 32, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.5);
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

class RadioButton : public SpriteContainer {
public:
    int32_t mX;
    int32_t mY;
    const int32_t mWidth = 258;
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

        bg = new pSprite(TX_BUTTON_RADIO, mX, mY, mWidth, mHeight, ORIGIN_TOPLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.5f);
        falseValueLabel = new pText(false_value_label, FONT_PIXEL, mX+67, mY+16);
        falseValueLabel->Z = -0.6f;
        falseValueLabel->Origin = ORIGIN_CENTER;
        trueValueLabel = new pText(true_value_label, FONT_PIXEL, mX+192, mY+16);
        trueValueLabel->Z = -0.6f;
        trueValueLabel->Origin = ORIGIN_CENTER;

        if (mValue) {
            bg->Angle = 180;
            trueValueLabel->Font = FONT_PIXEL_ACTIVE;
        } else {
            falseValueLabel->Font = FONT_PIXEL_ACTIVE;
        }

        mSprites.push_back(falseValueLabel);
        mSprites.push_back(trueValueLabel);
        mSprites.push_back(bg);
    }

    bool InBounds(int32_t x, int32_t y) {
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
            falseValueLabel->Font = FONT_PIXEL;
            trueValueLabel->Font = FONT_PIXEL_ACTIVE;
        } else {
            AudioManager::Engine().PlayUISound(UISOUND_CHECK_OFF);
            bg->Angle = 0;
            falseValueLabel->Font = FONT_PIXEL_ACTIVE;
            trueValueLabel->Font = FONT_PIXEL;
        }

        value_callback(mValue);

        mTouchActive = false;
        return true;
    }
};

class ValueSlider : public SpriteContainer {
public:
    int32_t mX;
    int32_t mY;
    int32_t mSliderPosition;
    int32_t mWidth;
    bool mTouchActive = false;
    std::function<void(float)> value_change_callback;
    std::function<void(float)> value_callback;

    ValueSlider(int32_t x, int32_t y, int32_t width=206) {
        mX = mSliderPosition = x;
        mY = y;
        mWidth = width;

        pDrawable* spr;
        spr = new pSprite(TX_BUTTON_SLIDER_CIRCLE, mX, mY, slider_control_width, slider_control_height, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color(), 255, -1);
        mSprites.push_back(spr);
    }

    void Init(float value) {
        int pos = round((value / 100.f * mWidth) + mX);
        mSliderPosition = (pos < mX) ? mX : ((pos > mX + mWidth) ? mX+mWidth : pos);
        mSprites[0]->X = mSliderPosition;
    }

    bool InBounds(int32_t x, int32_t y) {
        SDL_Rect r = {mSliderPosition - (slider_control_touchwidth / 2), mY - (slider_control_touchheight / 2),
                      slider_control_touchwidth, slider_control_touchheight};
        SDL_Point p = {x,y};
        return SDL_PointInRect(&p, &r);
    }

    void OnTouchDown(const touchPosition& touch) {
        mTouchActive = true;
    }
    bool OnTouch(const touchPosition& touch) {
        if (!mTouchActive) {
            return false;
        }
        mSliderPosition = (touch.px < mX) ? mX : ((touch.px > mX + mWidth) ? mX+mWidth : touch.px);
        mSprites[0]->Move(mSliderPosition, mY);
        mSprites[0]->Texture = TX_BUTTON_SLIDER_CIRCLE_ACTIVE;
        value_change_callback(((float)mSliderPosition - (float)mX) / (float)mWidth * 100.f);
        return true;
    }
    bool OnTouchUp(const touchPosition& touch) {
        if (!mTouchActive) {
            return false;
        }

        AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);

        mSprites[0]->Texture = TX_BUTTON_SLIDER_CIRCLE;
        value_callback(((float)mSliderPosition - (float)mX) / (float)mWidth * 100.f);
        mTouchActive = false;
        return true;
    }

private:
    const int slider_control_width = 22;
    const int slider_control_height = 22;
    const int slider_control_touchwidth = slider_control_width * 1.3;
    const int slider_control_touchheight = slider_control_height * 1.3;
};

class ModeSettings : public Mode {
public:
    ModeSettings();

    ~ModeSettings();

    void TabGeneral();

    void TabGameplay();

    void Update();

    void Clear();

    void InitCommonSprites();

    // 0 — Unset; 1 — TabGeneral; 2 — TabGameplay
    static uint8_t SwitchTabTo;

protected:
    std::vector<ValueSlider *> valueSliders;
    std::vector<StringSelector *> stringSelectors;
    std::vector<RadioButton *> radioButtons;

    void HandleInput() override;

    SpriteManager mSpriteManager;

    Logo* mLogo = nullptr;


    void CreateValueSlider(int32_t x, int32_t y, const std::string &setting_name);

    void CreateStringSelector(int32_t x, int32_t y, const std::string &setting_name);

    void CreateRadioButton(int32_t x, int32_t y, const std::string &setting_name,
                           const std::string &false_value_label, const std::string &true_value_label);
};

#endif //_MODESETTINGS_H
