#include "SpriteManager.h"
#include <algorithm>

SpriteManager::SpriteManager() = default;

SpriteManager::~SpriteManager() {
	for (auto & mSprite : mSprites) {
		delete mSprite;
	}
}

void SpriteManager::SortZ() {
    std::sort(mSprites.begin(), mSprites.end(), [](const pDrawable* first, const pDrawable* second) {
        return first->Z > second->Z;
    });
}

void SpriteManager::Draw() {
	SortZ();

	for (auto it = mSprites.begin(); it != mSprites.end();) {
        pDrawable *spr = *it;

        //if for some reason sprite is nonexistent then mark for deletion
        if (spr == nullptr) {
            it = mSprites.erase(it);
            continue;
        }

        spr->Update();

        //if sprite is dead then mark for deletion
        if (!spr->Alive()) {
            delete spr;
            it = mSprites.erase(it);
            continue;
        }

        //no need to draw things you can't see
        if (spr->Width == 0 || spr->Height == 0 || spr->Alpha == 0) {
            ++it;
            continue;
        }

		spr->Draw();
        ++it;
	}

	this->HandleTouchInput();
}

void SpriteManager::Add(pDrawable* spr) {
	mSprites.push_back(spr);
}

void SpriteManager::Add(const std::vector<pDrawable*>& spr) {
	for (auto it : spr) {
		Add(it);
	}
}

void SpriteManager::HandleTouchInput() {
	if (!InputHelper::KeyDown(SDL_BUTTON_LEFT, IH_KEY_MOUSE))
		return;
	touchPosition touchPos = InputHelper::TouchRead();

    SortZ();
    int i = 0;
    for (auto current : mSprites) {
        if (current == nullptr) {
            fprintf(stderr, "Null %i\n", i);
            i++;
            continue;
        }
        if (current->InBounds(touchPos.px, touchPos.py) && current->Clickable) {
			current->OnClick(current, touchPos.px, touchPos.py);
		}
	}
}