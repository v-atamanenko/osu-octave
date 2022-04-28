#pragma once

#include "pDrawable.h"

class pSprite : public pDrawable {
public:
    pSprite(TextureType texture, OOInt x, OOInt y, OOUInt width, OOUInt height, DrawOrigin origin, FieldType fieldtype, SDL_Color color, OOUShort alpha, OOFloat z = 0);

    void Draw() override;
    SDL_Rect GetRect() {
        OOInt rect_x = X, rect_y = Y, rect_w = (OOInt)Width, rect_h = (OOInt)Height;
        switch (Origin) {
            case ORIGIN_TOPLEFT:
                break;
            case ORIGIN_CENTER:
                rect_x -= rect_w/2;
                rect_y -= rect_h/2;
                break;
            case ORIGIN_CENTERLEFT:
                rect_y -= rect_h/2;
                break;
            case ORIGIN_TOPRIGHT:
                rect_x -= rect_w;
                break;
            case ORIGIN_BOTTOMLEFT:
                rect_y -= rect_h;
                break;
        }
        return SDL_Rect({rect_x, rect_y, rect_w, rect_h});
    }
};
