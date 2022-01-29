#include "pSprite.h"

pSprite::pSprite(TextureType texture, int32_t x, int32_t y, uint32_t width, uint32_t height, DrawOrigin origin, FieldType fieldtype, SDL_Color color, uint32_t alpha, float z)
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
	UV = nullptr;
	Z = z;
    Clickable = false;
}

void pSprite::Draw() {
    GraphicsManager::Graphics().Draw(this->Texture,
									 this->X, this->Y,
									 this->Width, this->Height,
									 this->Origin, this->Field,
									 this->Color, this->Alpha, this->Angle,
									 this->Z, 
                                     this->UV
    );
}