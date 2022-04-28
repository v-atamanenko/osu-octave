#pragma once

#include "Graphics/SpriteContainer.h"

class UIElement : public SpriteContainer {
public:
    OOInt mX;
    OOInt mY;

    virtual void Update() {}
    virtual bool InBounds(OOInt x, OOInt y) { return false; }

    virtual void OnTouchDown(const touchPosition& touch) {}
    virtual bool OnTouch(const touchPosition& touch) { return false; }
    virtual bool OnTouchUp(const touchPosition& touch) { return false; }
};
