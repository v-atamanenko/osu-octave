#pragma once

#include "Graphics/SpriteContainer.h"

class Logo : public SpriteContainer {
public:
    int32_t mX;
    int32_t mY;
    OOTime mAnimLastStartedAt = 0; //ms

    const int32_t mWidth = 213;
    const int32_t mHeight = 116;
    const OOTime mAnimLength = 900; //ms
    const float minScale = 0.96f;
    const float maxScale = 1.04f;

    bool mAnimStarted = false;

    pSprite* mLogo;

    Logo(int32_t x, int32_t y, bool startAnimation=true) {
        mX = x;
        mY = y;
        mLogo = new pSprite(TX_LOGO, mX, mY, mWidth, mHeight, ORIGIN_CENTER, FIELD_SCREEN, SDL_Color(), 255, -0.5f);
        mSprites.push_back(mLogo);
        if (startAnimation) { StartAnimation(); }
    }

    void StartAnimation(OOTime delay = 0) {
        mAnimStarted = true;
        mAnimLastStartedAt = GameClock::Clock().Time() + delay;
        mLogo->Heartbeat(mAnimLastStartedAt, mAnimLength, minScale, maxScale);
    }

    void Update() {
        if (!mAnimStarted) return;

        OOTime now = GameClock::Clock().Time();
        if (now >= (mAnimLastStartedAt + mAnimLength)) {
            mAnimLastStartedAt = now;
            mLogo->Heartbeat(mAnimLastStartedAt, mAnimLength, minScale, maxScale);
        }
    }
};
