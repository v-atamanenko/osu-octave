#include "SpriteContainer.h"

SpriteContainer::~SpriteContainer()
{
	if (mSpriteOwner)
	{
		for (auto & mSprite : mSprites)
		{
            fprintf(stderr, "we're deleting something\n");
			delete mSprite;
		}
	}
}

void SpriteContainer::AddToSpriteManager(SpriteManager& spriteManager)
{
	spriteManager.Add(mSprites);
	
	//once sprites are added to spritemanager, the memory
	//belongs to the spritemanager, and should be deleted by it
	mSpriteOwner = false;
}

