#include "SpriteManager.h"

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
	for (spriteIterator it = mSprites.begin(); it != mSprites.end(); ++it)
	{
		if (*it != NULL)
			delete *it;
	}
}

void SpriteManager::Draw()
{
	u32 i = 0;
	std::vector<u32> list;
	
	for (spriteIterator it = mSprites.begin(); it != mSprites.end(); ++it, ++i)
	{
		pSprite* spr = *it;
		
		//if for some reason sprite is nonexistent then mark for deletion
		if (spr == NULL)
		{
			list.push_back(i);
			continue;
		}
		
		spr->Update();
		
		//if sprite is dead then mark for deletion
		if (spr->Draw() == false)
		{
			list.push_back(i);
			continue;
		}
		
		//no need to draw things you can't see
		if (spr->Width == 0 || spr->Height == 0 || spr->Alpha == 0)
			continue;
		
		GraphicsManager::Graphics().Draw(	spr->Texture,
											spr->X, spr->Y,
											spr->Width, spr->Height,
											spr->Origin, spr->Field,
											spr->Color, spr->Alpha, spr->Angle,
											spr->Z, spr->UV);
		
	}
	
	//delete dead sprites
	while (list.size() > 0)
	{
		spriteIterator it = mSprites.begin() + list.back();
		
		if (*it != NULL)
			delete *it;
		
		mSprites.erase(it);
		list.pop_back();
	}
}

void SpriteManager::Add(pSprite* spr)
{
	mSprites.push_back(spr);
}

void SpriteManager::Add(const std::vector<pSprite*>& spr)
{
	for (spriteIteratorConst it = spr.begin(); it != spr.end(); ++it)
	{
		Add(*it);
	}
}
