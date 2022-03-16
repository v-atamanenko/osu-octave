#ifndef _MODESETTINGS_H
#define _MODESETTINGS_H

#include "DataStorage/Settings.h"
#include "Graphics/pText.h"
#include "Graphics/SpriteManager.h"
#include "System/TextManager.h"
#include "Graphics/SpriteContainer.h"

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
    void OnTouch(const touchPosition& touch) {
        if (!mTouchActive) {
            return;
        }
        mSliderPosition = (touch.px < mX) ? mX : ((touch.px > mX + mWidth) ? mX+mWidth : touch.px);
        mSprites[0]->Move(mSliderPosition, mY);
        value_change_callback(((float)mSliderPosition - (float)mX) / (float)mWidth * 100.f);
    }
    void OnTouchUp(const touchPosition& touch) {
        if (!mTouchActive) {
            return;
        }
        value_callback(((float)mSliderPosition - (float)mX) / (float)mWidth * 100.f);
        mTouchActive = false;
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

    pDrawable* mHitSoundsVolumeText;
    pDrawable* mMusicVolumeText;
    pDrawable* mMenuMusicVolumeText;
    pDrawable* mUISoundsVolumeText;
    pDrawable* mCurrentSkinText;
    pDrawable* mBGDimLevelText;

    ValueSlider* mHitSoundsVolumeSlider;
    ValueSlider* mMusicVolumeSlider;
    ValueSlider* mMenuMusicVolumeSlider;
    ValueSlider* mUISoundsVolumeSlider;
    ValueSlider* mBGDimLevelSlider;

protected:
    std::vector<ValueSlider*> valueSliders;
    void HandleInput();
    SpriteManager mSpriteManager;

    void CreateValueSlider(int32_t x, int32_t y, const std::string& setting_name);
};

#endif //_MODESETTINGS_H
