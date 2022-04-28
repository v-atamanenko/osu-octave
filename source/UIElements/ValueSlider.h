#pragma once

#include "UIElements/UIElement.h"

class ValueSlider : public UIElement {
public:
    OOInt mSliderPosition;
    OOUInt mWidth;
    bool mTouchActive = false;
    std::function<void(OOFloat)> value_change_callback;
    std::function<void(OOFloat)> value_callback;

    ValueSlider(OOInt x, OOInt y, OOUInt width=206) {
        mX = mSliderPosition = x;
        mY = y;
        mWidth = width;

        pDrawable* spr;
        spr = new pSprite(TX_BUTTON_SLIDER_CIRCLE, mX, mY, slider_control_width, slider_control_height, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color(), 255, -1);
        mSprites.push_back(spr);
    }

    void Init(OOFloat value) {
        auto pos = (OOInt) round((value / 100.f * mWidth) + mX);
        mSliderPosition = (pos < mX) ? mX : ((pos > mX + mWidth) ? mX+mWidth : pos);
        mSprites[0]->X = mSliderPosition;
    }

    bool InBounds(OOInt x, OOInt y) {
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
        value_change_callback(((OOFloat)mSliderPosition - (OOFloat)mX) / (OOFloat)mWidth * 100.f);
        return true;
    }

    bool OnTouchUp(const touchPosition& touch) {
        if (!mTouchActive) {
            return false;
        }

        AudioManager::Engine().PlayUISound(UISOUND_CLICK_SHORT_CONFIRM);

        mSprites[0]->Texture = TX_BUTTON_SLIDER_CIRCLE;
        value_callback(((OOFloat)mSliderPosition - (OOFloat)mX) / (OOFloat)mWidth * 100.f);
        mTouchActive = false;
        return true;
    }

private:
    const OOInt slider_control_width = 22;
    const OOInt slider_control_height = 22;
    const OOInt slider_control_touchwidth = slider_control_width * 1.3;
    const OOInt slider_control_touchheight = slider_control_height * 1.3;
};
