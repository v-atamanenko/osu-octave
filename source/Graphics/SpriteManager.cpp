#include "SpriteManager.h"

#include <algorithm>

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
	for (auto & mSprite : mSprites)
	{
		delete mSprite;
	}
}

void SpriteManager::Draw()
{
	uint32_t i = 0;
	std::vector<uint32_t> deadSprites;

	std::sort(mSprites.begin(), mSprites.end(), [](const pDrawable* first, const pDrawable* second) {
		return first->Z > second->Z;
	});
	
	for (auto it = mSprites.begin(); it != mSprites.end(); ++it, ++i)
	{
		pDrawable* spr = *it;
		
		//if for some reason sprite is nonexistent then mark for deletion
		if (spr == nullptr)
		{
			deadSprites.push_back(i);
			continue;
		}
		
		spr->Update();
		
		//if sprite is dead then mark for deletion
		if (!spr->Alive())
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
	while (!deadSprites.empty())
	{
		auto it = mSprites.begin() + deadSprites.back();
		
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
	for (auto it : spr)
	{
		Add(it);
	}
}

void SpriteManager::HandleTouchInput() {
	if (!InputHelper::KeyDown(SDL_BUTTON_LEFT, IH_KEY_MOUSE))
		return;

	touchPosition touchPos = InputHelper::TouchRead();

	std::vector<pDrawable*> sprites = this->mSprites;

	std::sort(sprites.begin(), sprites.end(), [](const pDrawable* first, const pDrawable* second) {
		return first->Z > second->Z;
	});

	for(auto current : sprites) {
        if(current->InBounds(touchPos.px, touchPos.py) && current->Clickable) {
			current->OnClick(current, touchPos.px, touchPos.py);
		}
	}
}