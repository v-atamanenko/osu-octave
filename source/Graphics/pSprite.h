#pragma once

#include "pDrawable.h"

class pSprite : public pDrawable {
public:
    pSprite(TextureType texture, OOInt x, OOInt y, OOUInt width, OOUInt height, DrawOrigin origin, FieldType fieldtype, SDL_Color color, OOUShort alpha, OOFloat z = 0);

    void Draw() override;
    SDL_Rect GetRect() {
        return SDL_Rect({X, Y, (OOInt)Width, (OOInt)Height});
    }
};
