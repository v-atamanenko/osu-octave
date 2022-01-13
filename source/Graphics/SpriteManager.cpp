#include "SpriteManager.h"

#include <algorithm>
#include <string>

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
	std::vector<u32> deadSprites;

	std::sort(mSprites.begin(), mSprites.end(), [](const pDrawable* first, const pDrawable* second) {
		return first->Z > second->Z;
	});
	
	for (spriteIterator it = mSprites.begin(); it != mSprites.end(); ++it, ++i)
	{
		pDrawable* spr = *it;
		
		//if for some reason sprite is nonexistent then mark for deletion
		if (spr == NULL)
		{
			deadSprites.push_back(i);
			continue;
		}
		
		spr->Update();
		
		//if sprite is dead then mark for deletion
		if (spr->Alive() == false)
		{
			deadSprites.push_back(i);
			continue;
		}
		
		//no need to draw things you can't see
		if (spr->Width == 0 || spr->Height == 0 || spr->Alpha == 0)
			continue;
		
		spr->Draw();
	}
	
	//delete dead sprites
	while (deadSprites.size() > 0)
	{
		spriteIterator it = mSprites.begin() + deadSprites.back();
		
		if (*it != NULL)
			delete *it;
		
		mSprites.erase(it);
		deadSprites.pop_back();
	}

	this->HandleTouchInput();
}

void SpriteManager::Add(pDrawable* spr)
{
	mSprites.push_back(spr);
}

void SpriteManager::Add(const std::vector<pDrawable*>& spr)
{
	for (spriteIteratorConst it = spr.begin(); it != spr.end(); ++it)
	{
		Add(*it);
	}
}

void SpriteManager::HandleTouchInput() {
	if (!InputHelper::KeyDown(KEY_TOUCH))
		return;

	touchPosition touchPos = InputHelper::TouchRead();

	std::vector<pDrawable*> sprites = this->mSprites;

	std::sort(sprites.begin(), sprites.end(), [](const pDrawable* first, const pDrawable* second) {
		return first->Z > second->Z;
	});

	for(int i = 0; i != sprites.size(); i++) {
		pDrawable* current = sprites[i];

		if(current->InBounds(touchPos.px, touchPos.py) && current->Clickable) {
			current->OnClick(current, touchPos.px, touchPos.py);
		}
	}
}