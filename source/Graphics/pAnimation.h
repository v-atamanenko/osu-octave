#include "pSprite.h"
#include "SDL.h"

#ifndef __PANIMATION_H__
#define __PANIMATION_H__

class pAnimation : public pSprite
{
	public:
		pAnimation(TextureType texture, uint32_t framecount, uint32_t fps, int32_t x, int32_t y, uint32_t width, uint32_t height, DrawOrigin origin, FieldType type, SDL_Color color, uint32_t alpha);
		void Update();
	
	protected:
		TextureType mOrigTexture;
		float mUpdatesPerFrame;
		float mUpdatesWaited;
		uint32_t mFrameCurrent;
		uint32_t mFrameCount;
};

#endif

