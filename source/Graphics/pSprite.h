#include "pDrawable.h"

#ifndef __PSPRITE_H__
#define __PSPRITE_H__

class pSprite : public pDrawable {
public:
    pSprite(TextureType texture, int32_t x, int32_t y, uint32_t width, uint32_t height, DrawOrigin origin, FieldType fieldtype, SDL_Color color, uint32_t alpha, float z = 0);

    void Draw() override;
    SDL_Rect GetRect() {
        return SDL_Rect({X, Y, Width, Height});
    }
};

#endif