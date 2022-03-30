#include "pSprite.h"

pSprite::pSprite(TextureType texture, OOInt x, OOInt y, OOUInt width, OOUInt height, DrawOrigin origin, FieldType fieldtype, SDL_Color color, OOUShort alpha, OOFloat z) {
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
