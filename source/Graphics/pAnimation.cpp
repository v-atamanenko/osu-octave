#include "pAnimation.h"

pAnimation::pAnimation(TextureType texture, uint32_t framecount, uint32_t fps, int32_t x, int32_t y, uint32_t width, uint32_t height, DrawOrigin origin, FieldType type, SDL_Color color, uint32_t alpha)
	: pSprite(texture, x, y, width, height, origin, type, color, alpha)
{
	mFrameCount = framecount;
	mFrameCurrent = 0;
	mOrigTexture = texture;
	
	mUpdatesPerFrame = 60.0f / (float)fps;
	mUpdatesWaited = 0;
}

void pAnimation::Update()
{
	while (mUpdatesWaited >= mUpdatesPerFrame)
	{
		mUpdatesWaited -= mUpdatesPerFrame;
		
		++mFrameCurrent;
		if (mFrameCurrent >= mFrameCount)
			mFrameCurrent = 0;
		
		Texture = (TextureType)(mOrigTexture + mFrameCurrent);
	}
	
	++mUpdatesWaited;
	
	pSprite::Update();
}