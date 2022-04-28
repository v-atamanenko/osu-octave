#pragma once

#include "UIElements/UIElement.h"

class TernaryButton : public UIElement {
public:
    const int32_t mWidth = 270;
    const int32_t mHeight = 32;

    std::function<void(uint8_t)> value_callback;

    pSprite* bg;

    TextureType bgtex1;
    TextureType bgtex2;
    TextureType bgtex3;

    uint8_t mValue;
    uint8_t mClickedValue;
    bool mTouchActive = false;

    TernaryButton(int32_t x, int32_t y, uint8_t value, TextureType bg1, TextureType bg2, TextureType bg3) {
        mX = x;
        mY = y;
        mValue = value;

        bgtex1 = bg1;
        bgtex2 = bg2;
        bgtex3 = bg3;

        TextureType currentBG;
        switch (value) {
            case 0: currentBG = bgtex1; break;
            case 1: currentBG = bgtex2; break;
            case 2: currentBG = bgtex3; break;
        }

        bg = new pSprite(currentBG, mX, mY, mWidth, mHeight, ORIGIN_CENTERLEFT, FIELD_SCREEN, SDL_Color(), 255, -0.5f);
        mSprites.push_back(bg);
    }

    bool InBounds(int32_t x, int32_t y) {
        int32_t width_1_3 = floor(mWidth / 3);
        SDL_Rect r_a = SDL_Rect({mX, mY, width_1_3, mHeight});
        SDL_Rect r_b = SDL_Rect({mX+width_1_3, mY, width_1_3, mHeight});
        SDL_Rect r_c = SDL_Rect({mX+width_1_3+width_1_3, mY, width_1_3, mHeight});
        SDL_Point p = {x,y};

        if (SDL_PointInRect(&p, &r_a)) {
            mClickedValue = 0;
            return true;
        }

        if (SDL_PointInRect(&p, &r_b)) {
            mClickedValue = 1;
            return true;
        }

        if (SDL_PointInRect(&p, &r_c)) {
            mClickedValue = 2;
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

        mValue = mClickedValue;

        TextureType currentBG;
        switch (mValue) {
            case 0: currentBG = bgtex1; break;
            case 1: currentBG = bgtex2; break;
            case 2: currentBG = bgtex3; break;
        }

        bg->Texture = currentBG;
        AudioManager::Engine().PlayUISound(UISOUND_CHECK_ON);

        value_callback(mValue);

        mTouchActive = false;
        return true;
    }
};
