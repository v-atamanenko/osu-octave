#include "pDrawable.h"

#ifndef __PSPRITE_H__
#define __PSPRITE_H__

class pSprite : public pDrawable {
public:
    pSprite(TextureType texture, s32 x, s32 y, u32 width, u32 height, DrawOrigin origin, FieldType fieldtype, rgb color, u32 alpha, float z = 0);

    void Draw() override;
};

#endif