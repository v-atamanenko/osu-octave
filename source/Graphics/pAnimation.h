#pragma once

#include "SDL.h"
#include "types.h"
#include "pSprite.h"

class pAnimation : public pSprite
{
	public:
		pAnimation(TextureType texture, OOUInt framecount, OOUInt fps, OOInt x, OOInt y, OOUInt width, OOUInt height, DrawOrigin origin, FieldType type, SDL_Color color, OOUShort alpha);
		void Update() override;
	
	protected:
		TextureType mOrigTexture;
		OOFloat mUpdatesPerFrame;
		OOFloat mUpdatesWaited;
		OOUInt mFrameCurrent;
		OOUInt mFrameCount;
};
