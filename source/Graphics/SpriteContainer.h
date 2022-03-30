#pragma once

#include <cstdio>
#include <vector>

#include "pSprite.h"
#include "SpriteManager.h"

/*
 * A generic base class for all objects that require sprites.
 * Sprite cleanup is handled automatically.
 */
class SpriteContainer
{
	public:
		virtual ~SpriteContainer();
		void AddToSpriteManager(SpriteManager& spriteManager);
        void AddToSpriteManager(SpriteManager* spriteManager);
	
	protected:
		std::vector<pDrawable*> mSprites;
	
	private:
		bool mSpriteOwner;
};
