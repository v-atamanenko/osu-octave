#include "pSprite.h"

pSprite::pSprite(TextureType texture, s32 x, s32 y, u32 width, u32 height, DrawOrigin origin, FieldType fieldtype, rgb color, u32 alpha, float z)
{
	Texture = texture;
	X = x;
	Y = y;
	Width = mOrigWidth = width;
	Height = mOrigHeight = height;
	Origin = origin;
	Field = fieldtype;
	Color = color;
	Alpha = alpha;
	mAlive = true;
	Angle = 0;
	UV = NULL;
	Z = z;
    Clickable = false;
}

void pSprite::Draw() {
    
}