#include "pAnimation.h"

pAnimation::pAnimation(TextureType texture, OOUInt framecount, OOUInt fps, OOInt x, OOInt y, OOUInt width, OOUInt height, DrawOrigin origin, FieldType type, SDL_Color color, OOUShort alpha)
	: pSprite(texture, x, y, width, height, origin, type, color, alpha)
{
	mFrameCount = framecount;
	mFrameCurrent = 0;
	mOrigTexture = texture;
	
	mUpdatesPerFrame = 60.0 / (OOFloat)fps;
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